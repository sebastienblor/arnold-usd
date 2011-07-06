import maya.cmds as cmds
import mtoa.aovs as aovs
import utils
import mtoa.callbacks as callbacks
from mtoa.ui.ae.utils import aeCallback
from mtoa.ui.ae.shapeTemplate import BaseTemplate

global _aovCallbacks
_aovCallbacks = []

class AOVOptionMenuGrp(BaseTemplate):
    NEW_AOV_ITEM = "<Create New...>"
    EMTPY_AOV_ITEM = "<Select>"
    UNKNOWN_AOV_ITEM = "%s (Inactive)"
    BEAUTY_ITEM = "RGBA"
    _instances = []
    def __init__(self, nodeType, attr, label=None, allowCreation=True, includeBeauty=False):
        super(AOVOptionMenuGrp, self).__init__(nodeType)
        self.__class__._instances.append(self)
        self.attr = attr
        self.menuName = nodeType + attr + '_aovOptionMenu'
        self.activeNodes = None
        self.allowCreation = allowCreation
        self.includeBeauty = includeBeauty
        self.label = label if label else utils.interToUI(attr)

    @classmethod
    def globalAOVListChanged(cls):
        for inst in cls._instances:
            if inst.nodeName() is not None and cmds.objExists(inst.nodeName()):
                inst.update()

    def changeCallback(self, nodeAttr, newAOV):
        if newAOV == self.NEW_AOV_ITEM:
            # the the current value is inactive, fill this in as the starting text
            currVal = cmds.getAttr(nodeAttr)
            if currVal not in self.activeNames:
                text = currVal
            else:
                text = ''
            result = cmds.promptDialog(button=['Create', 'Cancel'],
                                       defaultButton='Create',
                                       cancelButton='Cancel',
                                       message='AOV Name',
                                       title='New AOV',
                                       text=text)
            if result == 'Create':
                newAOV = cmds.promptDialog(query=True, text=True)
                aovNode = aovs.getAOVNode().addAOV(newAOV)
                aovNode.attr('name').connect(nodeAttr, force=True)
                # attribute changed callback updates the UI
                #self.clear()
                #self.updateMenu(nodeAttr)
                # TODO: also set aov type
            else:
                # TODO: reset menu to previous value?
                return
        elif newAOV == self.BEAUTY_ITEM:
            conn = cmds.listConnections(nodeAttr, source=True, destination=False, plugs=True)
            if conn:
                cmds.disconnectAttr(conn[0], nodeAttr)
            cmds.setAttr(nodeAttr, self.BEAUTY_ITEM, type='string')
        else:
            dict(self.activeNodes)[newAOV].attr('name').connect(nodeAttr, force=True)
            # attribute changed callback updates the UI
            #cmds.optionMenuGrp(self.menuName, edit=True, value=newAOV)

    def updateMenu(self, nodeAttr):
        self.clear()
        currVal = cmds.getAttr(nodeAttr)
        self.activeNodes = aovs.getActiveAOVNodes(names=True)
        self.activeNames = [x[0] for x in self.activeNodes]
        if not currVal:
            currVal = self.EMTPY_AOV_ITEM
            cmds.menuItem(label=currVal, parent=(self.menuName + '|OptionMenu'))
        elif currVal not in self.activeNames and currVal != self.BEAUTY_ITEM:
            currVal = self.UNKNOWN_AOV_ITEM % currVal
            cmds.menuItem(label=currVal, parent=(self.menuName + '|OptionMenu'))

        if self.includeBeauty:
            self.activeNames.insert(0, self.BEAUTY_ITEM)

        # add items
        for aov in self.activeNames:
            cmds.menuItem(label=aov, parent=(self.menuName + '|OptionMenu'))
        if self.allowCreation:
            cmds.menuItem(label=self.NEW_AOV_ITEM, parent=(self.menuName + '|OptionMenu'))
        # set active
        cmds.optionMenuGrp(self.menuName, edit=True, value=currVal)

    def clear(self):
        for item in cmds.optionMenuGrp(self.menuName, query=True, itemListLong=True) or []:
            cmds.deleteUI(item)

    def build(self):
        nodeAttr = self.nodeAttr(self.attr)
        cmds.optionMenuGrp(self.menuName, label=self.label)
        self.updateMenu(nodeAttr)
        menu = cmds.optionMenuGrp(self.menuName, edit=True,
                                  changeCommand=lambda *args: self.changeCallback(nodeAttr, *args))

        # make sure the UI gets updated if the attribute changes while we have the AE open
        cmds.scriptJob(parent=menu,
                       attributeChange=(nodeAttr, lambda: self.updateMenu(nodeAttr)))

    def update(self):
        nodeAttr = self.nodeAttr(self.attr)
        self.updateMenu(nodeAttr)
        menu = cmds.optionMenuGrp(self.menuName, edit=True,
                           changeCommand=lambda *args: self.changeCallback(nodeAttr, *args))
        cmds.scriptJob(parent=menu, replacePrevious=True,
                       attributeChange=(nodeAttr, lambda: self.updateMenu(nodeAttr)))


def addAOVControl(nodeType, attr):
    aovSelect = AOVOptionMenuGrp(nodeType, attr)
    aovSelect.attachToAE()

def aovLayout(nodeType):
    '''Add an aov control for each aov registered for this node type'''
    aovAttrs = aovs.getNodeAOVAttrs(nodeType=nodeType)
    if aovAttrs:
        cmds.editorTemplate(beginLayout="AOVs", collapse=True)
        cmds.editorTemplate(beginNoOptimize=True)
        cmds.editorTemplate('enableAOVs', label='Enable AOVs', addControl=True)
        cmds.editorTemplate('overrideAOVs', label='Override AOV Names', addControl=True)
        cmds.editorTemplate(endNoOptimize=True)
        for name, attr in aovAttrs:
            addAOVControl(nodeType, attr)
        cmds.editorTemplate(endLayout=True)


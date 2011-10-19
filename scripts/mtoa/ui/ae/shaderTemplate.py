import pymel.core as pm
import mtoa.aovs as aovs
import utils
import mtoa.callbacks as callbacks
from mtoa.ui.ae.utils import aeCallback
import mtoa.ui.ae.aiSwatchDisplay as aiSwatchDisplay
from mtoa.ui.ae.shapeTemplate import BaseTemplate, AttributeEditorTemplate

global _aovCallbacks
_aovCallbacks = []

class AOVOptionMenuGrp(BaseTemplate):
    NEW_AOV_ITEM = "<Create New...>"
    EMTPY_AOV_ITEM = "<Select>"
    UNKNOWN_AOV_ITEM = "%s (Inactive)"
    BEAUTY_ITEM = "RGBA"
    _instances = []
    def __init__(self, nodeType, label=None, allowCreation=True, includeBeauty=False):
        super(AOVOptionMenuGrp, self).__init__(nodeType)
        self.__class__._instances.append(self)
        self.activeNodes = None
        self.allowCreation = allowCreation
        self.includeBeauty = includeBeauty
        self._label = label

    # TODO: convert to propertycache
    @property
    def menuName(self):
        return self.nodeType() + self.attr + '_aovOptionMenu'
    # TODO: convert to propertycache
    @property
    def label(self):
        return self._label if self._label else utils.interToUI(self.attr)
    
    @classmethod
    def globalAOVListChanged(cls):
        for inst in cls._instances:
            if inst.nodeName is not None and pm.objExists(inst.nodeName):
                inst.update()

    def changeCallback(self, nodeAttr, newAOV):
        if newAOV == self.NEW_AOV_ITEM:
            # the the current value is inactive, fill this in as the starting text
            currVal = pm.getAttr(nodeAttr)
            if currVal not in self.activeNames:
                text = currVal
            else:
                text = ''
            result = pm.promptDialog(button=['Create', 'Cancel'],
                                       defaultButton='Create',
                                       cancelButton='Cancel',
                                       message='AOV Name',
                                       title='New AOV',
                                       text=text)
            if result == 'Create':
                newAOV = pm.promptDialog(query=True, text=True)
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
            conn = pm.listConnections(nodeAttr, source=True, destination=False, plugs=True)
            if conn:
                pm.disconnectAttr(conn[0], nodeAttr)
            pm.setAttr(nodeAttr, self.BEAUTY_ITEM, type='string')
        else:
            dict(self.activeNodes)[newAOV].attr('name').connect(nodeAttr, force=True)
            # attribute changed callback updates the UI
            #pm.optionMenuGrp(self.menuName, edit=True, value=newAOV)

    def updateMenu(self, nodeAttr):
        self.clear()
        currVal = pm.getAttr(nodeAttr)
        self.activeNodes = aovs.getActiveAOVNodes(names=True)
        self.activeNames = [x[0] for x in self.activeNodes]
        if not currVal:
            currVal = self.EMTPY_AOV_ITEM
            pm.menuItem(label=currVal, parent=(self.menuName + '|OptionMenu'))
        elif currVal not in self.activeNames and currVal != self.BEAUTY_ITEM:
            currVal = self.UNKNOWN_AOV_ITEM % currVal
            pm.menuItem(label=currVal, parent=(self.menuName + '|OptionMenu'))

        if self.includeBeauty:
            self.activeNames.insert(0, self.BEAUTY_ITEM)

        # add items
        for aov in self.activeNames:
            pm.menuItem(label=aov, parent=(self.menuName + '|OptionMenu'))
        if self.allowCreation:
            pm.menuItem(label=self.NEW_AOV_ITEM, parent=(self.menuName + '|OptionMenu'))
        # set active
        pm.optionMenuGrp(self.menuName, edit=True, value=currVal)

    def clear(self):
        for item in pm.optionMenuGrp(self.menuName, query=True, itemListLong=True) or []:
            pm.deleteUI(item)

    def setup(self):
        nodeAttr = self.nodeAttr(self.attr)
        pm.optionMenuGrp(self.menuName, label=self.label)
        self.updateMenu(nodeAttr)
        menu = pm.optionMenuGrp(self.menuName, edit=True,
                                  changeCommand=lambda *args: self.changeCallback(nodeAttr, *args))

        # make sure the UI gets updated if the attribute changes while we have the AE open
        pm.scriptJob(parent=menu,
                     attributeChange=(nodeAttr, lambda: self.updateMenu(nodeAttr)))

    def update(self):
        nodeAttr = self.nodeAttr(self.attr)
        self.updateMenu(nodeAttr)
        menu = pm.optionMenuGrp(self.menuName, edit=True,
                           changeCommand=lambda *args: self.changeCallback(nodeAttr, *args))
        pm.scriptJob(parent=menu, replacePrevious=True,
                     attributeChange=(nodeAttr, lambda: self.updateMenu(nodeAttr)))

class ShaderMixin(object):
    def bumpNew(self, attrName):
        pm.setUITemplate('attributeEditorTemplate', pst=True)
        pm.attrNavigationControlGrp('bumpControl', label="Bump Mapping", at=attrName)
        pm.setUITemplate(ppt=True)

    def bumpReplace(self, attrName):
        pm.attrNavigationControlGrp('bumpControl', edit=True, at=attrName)

    def addBumpLayout(self):
        self.beginLayout("Bump Mapping", collapse=True)
        self.addCustom("normalCamera", self.bumpNew, self.bumpReplace)
        self.endLayout() # End Bump Layout

    def addSwatch(self):
        self.addCustom("message", aiSwatchDisplay.aiSwatchDisplayNew, aiSwatchDisplay.aiSwatchDisplayReplace)

    def addAOVControl(self, attr):
        menu = AOVOptionMenuGrp(self.nodeType())
        self.addChildTemplate(attr, menu)

    def addAOVLayout(self):
        '''Add an aov control for each aov registered for this node type'''
        aovAttrs = aovs.getNodeAOVAttrs(nodeType=self.nodeType())
        if aovAttrs:
            self.beginLayout("AOVs", collapse=True)
#            self.beginNoOptimize()
#            self.addControl('enableAOVs', label='Enable AOVs')
#            self.addControl('overrideAOVs', label='Override AOV Names')
#            self.endNoOptimize()
            dynamic = self.nodeType() not in set(pm.pluginInfo("mtoa", q=True, dependNode=True))
            for name, attr in aovAttrs:
                if dynamic:
                    attr = 'ai_' + attr
                self.addAOVControl(attr)
            self.endLayout()

class ShaderAETemplate(AttributeEditorTemplate, ShaderMixin):
    pass


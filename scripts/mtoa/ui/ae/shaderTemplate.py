import maya.cmds as cmds
import mtoa.aovs as aovs
import utils
import mtoa.callbacks as callbacks

global _aovCallbacks
_aovCallbacks = []

def shaderAOVChanged(nodeAttr):
    print "aov changed", nodeAttr
    nodeName, attr = nodeAttr.split('.', 1)
    aovNode = aovs.getAOVNode()
    newAovVal = cmds.getAttr(nodeAttr)
    aovNode.addAOV(newAovVal)

def syncAOVs():
    globalAOVs = set(aovs.getAOVNode().getAOVs())
    activeAOVs = aovs.getAllShaderAOVs()
    print "to add:", activeAOVs.difference(globalAOVs)
    print "to remove:", globalAOVs.difference(activeAOVs)

def aovLayout(nodeName):
    nodeType = cmds.objectType(nodeName.split('.')[0])
    aovAttrs = aovs.getNodeAOVAttrs(nodeType=nodeType)
    if aovAttrs:
        cmds.editorTemplate(beginLayout="AOVs", collapse=True)
        cmds.editorTemplate(beginNoOptimize=True)
        cmds.editorTemplate('enableAOVs', label='Enable AOVs', addControl=True)
        cmds.editorTemplate('overrideAOVs', label='Override AOV Names', addControl=True)
        cmds.editorTemplate(endNoOptimize=True)
        for name, attr in aovAttrs:
            cmds.editorTemplate(attr, label=utils.interToUI(name), addControl=True)
        cmds.editorTemplate(endLayout=True)
        if nodeType not in _aovCallbacks:
            _aovCallbacks.append(nodeType)
            callbacks.addAttributeChangedCallbacks(nodeType, 
                                                   [(attr, shaderAOVChanged) for name, attr in aovAttrs])

class AOVOptionMenuGrp(object):
    NEW_AOV_ITEM = "<Create New...>"
    UNKNOWN_AOV_ITEM = "<Select>"
    def __init__(self, nodeType, attr):
        self.nodeType = nodeType
        self.menuName = nodeType + attr + '_aovOptionMenu'

    def changeCallback(self, nodeAttr, newAOV):
        print newAOV
        if newAOV == self.NEW_AOV_ITEM:
            result = cmds.promptDialog(button=['Create', 'Cancel'],
                                       defaultButton='Create',
                                       cancelButton='Cancel',
                                       message='AOV Name',
                                       title='New AOV')
            if result == 'Create':
                newAOV = cmds.promptDialog(query=True, text=True)
                print "creating new AOV", newAOV
                aovs.getAOVNode().addAOV(newAOV)
                cmds.setAttr(nodeAttr, newAOV, type='string')
                # TODO: also set aov type
                self.clear()
                self.update(nodeAttr)
            else:
                # TODO: reset menu to previous value?
                return
        else:
            cmds.optionMenuGrp(self.menuName, edit=True, value=newAOV)
            cmds.setAttr(nodeAttr, newAOV, type='string')

    def update(self, nodeAttr):
        currVal = cmds.getAttr(nodeAttr)
        active = aovs.getActiveAOVs()
        if currVal not in active:
            cmds.setAttr(nodeAttr, '', type='string')
            cmds.menuItem(label=self.UNKNOWN_AOV_ITEM, parent=(self.menuName + '|OptionMenu'))
            currVal = self.UNKNOWN_AOV_ITEM
        # add items
        for aov in active:
            cmds.menuItem(label=aov, parent=(self.menuName + '|OptionMenu'))
        cmds.menuItem(label=self.NEW_AOV_ITEM, parent=(self.menuName + '|OptionMenu'))
        # set active
        cmds.optionMenuGrp(self.menuName, edit=True, value=currVal)

    def clear(self):
        for item in cmds.optionMenuGrp(self.menuName, query=True, itemListLong=True):
            cmds.deleteUI(item)

    def new(self, nodeAttr):
        #nodeAttr + '_aovOptionMenu', 
        attrName = nodeAttr.split('.')[-1]
        cmds.optionMenuGrp(self.menuName, label=utils.interToUI(attrName))
        self.update(nodeAttr)
        cmds.optionMenuGrp(self.menuName, edit=True, changeCommand=lambda *args: self.changeCallback(nodeAttr, *args))
    
    def replace(self, nodeAttr):
        self.clear()
        self.update(nodeAttr)
        cmds.optionMenuGrp(self.menuName, edit=True, changeCommand=lambda *args: self.changeCallback(nodeAttr, *args))


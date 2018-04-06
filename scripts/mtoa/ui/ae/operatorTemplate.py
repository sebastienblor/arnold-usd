from mtoa.ui.ae.shaderTemplate import ShaderAETemplate
import mtoa.melUtils as melUtils
import maya.cmds as cmds
from mtoa.callbacks import *


def changeOperator(node, nodeAttr):
    attrSize = melUtils.getAttrNumElements(*nodeAttr.split('.', 1))
    newItem = '{}[{}]'.format(nodeAttr, attrSize)
    cmds.connectAttr("%s.message"%node, newItem, force=True)
        
def createOperator(opType, nodeAttr):
    opNode = cmds.createNode(opType)
    changeOperator(opNode, nodeAttr)

def buildOperatorMenu(popup, attrName):
    nodeName = attrName.split('.')[0]
    cmds.popupMenu(popup, edit=True, deleteAllItems=True)
    operators = cmds.arnoldPlugins(listOperators=True) or []
    print operators
    for operator in operators:
        opNodes = cmds.ls(type=operator) or []
        for opNode in opNodes:
            if opNode == nodeName:
                continue
            cmds.menuItem(parent=popup, label=opNode, command=Callback(changeOperator, opNode, attrName))

    cmds.menuItem(parent=popup, divider=True)
    for operator in operators:
        cmdsLbl = 'Create {}'.format(operator)
        cmds.menuItem(parent=popup, label=cmdsLbl, command=Callback(createOperator, operator, attrName))


class OperatorMixin(object):

    def addInputOperator(self):
        attrName = '{}.inputs'.format(self.nodeName)
        attrSize = melUtils.getAttrNumElements(*attrName.split('.', 1))
        lastAttrName = '{}[{}]'.format(attrName, attrSize)
        cmds.setAttr(lastAttrName, "")

    def inputsReplace(self, nodeAttr):
        self._setActiveNodeAttr(nodeAttr)

        for ctrl in self._msgCtrls:
            cmds.deleteUI(ctrl)
        self._msgCtrls = []

        cmds.setUITemplate('attributeEditorTemplate', pushTemplate=True)
        cmds.setParent(self.otherCol)
        attrSize = melUtils.getAttrNumElements(*nodeAttr.split('.', 1))

        for i in range(attrSize):
            attrName = '{}[{}]'.format(nodeAttr, i)
            attrLabel = 'Inputs[{}]'.format(i)
            ctrl = cmds.attrNavigationControlGrp(at=attrName,
                                                    label=attrLabel, cn="createRenderNode -allWithShadersUp \"defaultNavigation -force true -connectToExisting -source %node -destination "+attrName+"\" \"\"")
   
            self._msgCtrls.append(ctrl) 

        cmds.setUITemplate('attributeEditorTemplate', popTemplate=True)


    def inputsNew(self, nodeAttr):
        
        # TODO: move this into AttributeEditorTemplate
        self._setActiveNodeAttr(nodeAttr)

        self._msgCtrls = []
        cmds.setUITemplate('attributeEditorTemplate', pushTemplate=True)

        cmds.frameLayout(label='Inputs', collapse=True)
        cmds.columnLayout(adjustableColumn=True)

        cmds.rowLayout(nc=2)
        cmds.text(label='')
        #cmds.text(label='')
        addInputButton = cmds.button(label='Add Input')

        self.oppopup = cmds.popupMenu(parent=addInputButton, button=1) 
        cmds.popupMenu(self.oppopup, edit=True, postMenuCommand=Callback(buildOperatorMenu, self.oppopup, nodeAttr))
        cmds.setParent('..') # rowLayout

        cmds.frameLayout(labelVisible=False, collapsable=False)
        self.otherCol = cmds.columnLayout(adjustableColumn=True)
        
        attrSize = melUtils.getAttrNumElements(*nodeAttr.split('.', 1))

        for i in range(attrSize):
            attrName = '{}[{}]'.format(nodeAttr, i)
            attrLabel = 'Inputs[{}]'.format(i)
            ctrl = cmds.attrNavigationControlGrp(at=attrName,
                                                    label=attrLabel, cn="createRenderNode -allWithShadersUp \"defaultNavigation -force true -connectToExisting -source %node -destination "+attrName+"\" \"\"")
   
            self._msgCtrls.append(ctrl) 

        cmds.setParent('..') # columnLayout
        cmds.setParent('..') # frameLayout

        cmds.setParent('..') # columnLayout
        cmds.setParent('..') # frameLayout
        
        cmds.setParent('..') # columnLayout
        cmds.setParent('..') # frameLayout
        cmds.setUITemplate('attributeEditorTemplate', popTemplate=True)

    def addOperatorInputs(self):
        self.addCustom("inputs", self.inputsNew, self.inputsReplace)

class OperatorAETemplate(ShaderAETemplate, OperatorMixin):
    pass


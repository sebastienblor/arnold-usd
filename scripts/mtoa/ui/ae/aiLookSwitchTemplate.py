import maya.mel
import maya.cmds as cmds
import mtoa.melUtils as melUtils
from mtoa.ui.ae.operatorTemplate import *
from mtoa.ui.ae.aiStandInTemplate import LookDialog
from mtoa.callbacks import *
import re


def varChangeOperator(node, nodeAttr):
    attrNode, attr, child = nodeAttr.split('.')
    m_idx = re.match(r'(\w+)\[(\d+)]', attr)
    if m_idx:
        parAttr, idx = m_idx.groups()
        inputsSize = melUtils.getChildNumberElements(attrNode, parAttr, int(idx), 1)
        newItem = '{}[{}]'.format(nodeAttr, inputsSize)
        cmds.connectAttr("{}.out".format(node), newItem, force=True)
        return True


def varCreateOperator(opType, nodeAttr):
    opNode = cmds.createNode(opType)
    varChangeOperator(opNode, nodeAttr)


def varBuildOperatorMenu(popup, attrName):
    nodeName = attrName.split('.')[0]
    cmds.popupMenu(popup, edit=True, deleteAllItems=True)
    operators = cmds.arnoldPlugins(listOperators=True) or []

    for operator in operators:
        opNodes = cmds.ls(type=operator) or []
        for opNode in opNodes:
            if opNode == nodeName:
                continue
            cmds.menuItem(parent=popup, label=opNode, command=Callback(varChangeOperator, opNode, attrName))

    cmds.menuItem(parent=popup, divider=True)
    for operator in operators:
        cmdsLbl = 'Create {}'.format(operator)
        cmds.menuItem(parent=popup, label=cmdsLbl, command=Callback(varCreateOperator, operator, attrName))


class AEaiLookSwitchTemplate(OperatorAETemplate):

    def addInputOperator(self, *args):
        attrName = '{}.inputs'.format(self.nodeName)
        attrSize = melUtils.getAttrNumElements(*attrName.split('.', 1))
        lastAttrName = '{}[{}]'.format(attrName, attrSize)
        cmds.setAttr(lastAttrName, "")

    def looksReplace(self, nodeAttr):
        self._setActiveNodeAttr(nodeAttr)

        for ctrl in self._msgCtrls:
            cmds.deleteUI(ctrl)
        self._msgCtrls = []

        cmds.setUITemplate('attributeEditorTemplate', pushTemplate=True)
        cmds.setParent(self.otherCol)
        indices = cmds.getAttr("{}.looks".format(self.nodeName), multiIndices=True) or []

        for i in indices:
            # add widgets for name and inputs list
            attrName = '{}[{}].inputs'.format(nodeAttr, i)

            this_frame = cmds.frameLayout(label="look {}".format(i),
                                          collapsable=False,
                                          marginHeight=2,
                                          borderVisible=True,
                                          backgroundShade=True)
            cmds.columnLayout(adjustableColumn=True)

            cmds.rowLayout(nc=1)
            addInputButton = cmds.button(label='Add Input')

            oppopup = cmds.popupMenu(parent=addInputButton, button=1) 
            cmds.popupMenu(oppopup, edit=True, postMenuCommand=Callback(varBuildOperatorMenu, oppopup, attrName))
            cmds.setParent('..')  # rowLayout

            cmds.attrControlGrp(attribute='{}[{}].name'.format(nodeAttr, i), label="Name")

            # inputsSize = melUtils.getChildNumberElements(self.nodeName, 'looks', i, 1)
            in_indices = cmds.getAttr("{}.looks[{}].inputs".format(self.nodeName, i), multiIndices=True) or []

            for c in in_indices:
                inputName = '{}[{}]'.format(attrName,c)
                inputLabel = 'Inputs[{}]'.format(c)

                cmds.attrNavigationControlGrp(at=inputName,
                                              label=inputLabel,
                                              cn="createRenderNode -allWithShadersUp \"defaultNavigation -force true -connectToExisting -source %node -destination "+inputName+"\" \"\"")

            cmds.setParent('..')  # columnLayout
            cmds.setParent('..')  # frameLayout
            self._msgCtrls.append(this_frame)

        cmds.setUITemplate('attributeEditorTemplate', popTemplate=True)

    def looksNew(self, nodeAttr):
        self._setActiveNodeAttr(nodeAttr)
        self._msgCtrls = []
        cmds.setUITemplate('attributeEditorTemplate', pushTemplate=True)

        cmds.frameLayout(label='Looks', collapse=True)
        cmds.columnLayout(adjustableColumn=True)

        cmds.rowLayout(nc=2)
        cmds.text(label='')

        addLookButton = cmds.button(label='Add Look', command=self.addLook)

        cmds.setParent('..')  # rowLayout

        cmds.frameLayout(labelVisible=False, marginHeight=2, collapsable=False)
        self.otherCol = cmds.columnLayout(adjustableColumn=True)

        cmds.setParent('..')  # columnLayout
        cmds.setParent('..')  # frameLayout

        cmds.setParent('..')  # columnLayout
        cmds.setParent('..')  # frameLayout

        cmds.setParent('..')  # columnLayout
        cmds.setParent('..')  # frameLayout
        cmds.setUITemplate('attributeEditorTemplate', popTemplate=True)

        self.looksReplace(nodeAttr)

    def newLookUI(self, defaultname):

        lookDialog = LookDialog(edit=True, lookName=defaultname)

        val = lookDialog.show()
        if val == "cancel":
            return False, False

        return lookDialog.look, lookDialog.duplicateCurrent

    def addLook(self, *args):

        current_index = cmds.getAttr('{}.index'.format(self.nodeName))
        next_index = melUtils.getAttrNumElements(self.nodeName, "looks")

        new_look_name, duplicate = self.newLookUI("pass{}".format(next_index))

        if new_look_name:
            cmds.setAttr('{}.looks[{}].name'.format(self.nodeName, next_index), new_look_name, type="string")

    def setup(self):
        self.beginScrollLayout()

        self.beginLayout('Operator', collapse=False)
        self.addControl("enable")
        self.endLayout()
        self.beginLayout('Switch', collapse=False)
        self.addControl("index")
        self.endLayout()
        self.addCustom("looks", self.looksNew, self.looksReplace)

        maya.mel.eval('AEdependNodeTemplate '+self.nodeName)

        self.addExtraControls()

        self.endScrollLayout()


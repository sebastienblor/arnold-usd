import maya.mel
import maya.cmds as cmds
import mtoa.melUtils as melUtils
from mtoa.ui.ae.operatorTemplate import OperatorAETemplate


class AEaiStringReplaceTemplate(OperatorAETemplate):

    def createExpression(self, nodeAttr):
        matchAttrName = '{}.match'.format(self.nodeName)
        replaceAttrName = '{}.replace'.format(self.nodeName)
        attrSize = melUtils.getAttrNumElements(*matchAttrName.split('.', 1))
        lastMatchAttrName = '{}[{}]'.format(matchAttrName, attrSize)
        lastReplaceAttrName = '{}[{}]'.format(replaceAttrName, attrSize)
        cmds.setAttr(lastMatchAttrName, "", type="string")
        cmds.setAttr(lastReplaceAttrName, "", type="string")

    def deleteExpression(self, index):
        matchAttrName = '{}.match'.format(self.nodeName)
        replaceAttrName = '{}.replace'.format(self.nodeName)
        # delet all the attributes and record the current expressions, skipping the deleted index
        exprs = []
        attrSize = melUtils.getAttrNumElements(*matchAttrName.split('.', 1))
        for i in range(0, attrSize):
            m_exp = cmds.getAttr("{}[{}]".format(matchAttrName, i))
            r_exp = cmds.getAttr("{}[{}]".format(replaceAttrName, i))
            if i != index:
                exprs.append((m_exp, r_exp))
            cmds.removeMultiInstance(matchAttrName+'['+str(i)+']')
            cmds.removeMultiInstance(replaceAttrName+'['+str(i)+']')

        # make the new attributes
        for i, v in enumerate(exprs):
            cmds.setAttr("{}[{}]".format(matchAttrName, i), v[0], type="string")
            cmds.setAttr("{}[{}]".format(replaceAttrName, i), v[1], type="string")

    def setExpression(self, index, value, matchAttr):
        attr = "{}.{}[{}]".format(self.nodeName, matchAttr, index)
        cmds.setAttr(attr, value, type="string")

    def expressionNew(self, nodeAttr):
        self._setActiveNodeAttr(nodeAttr)

        self._matchCtrls = []
        cmds.setUITemplate('attributeEditorTemplate', pushTemplate=True)
        cmds.frameLayout(label='Expressions', collapse=False)
        cmds.columnLayout(adjustableColumn=True)

        cmds.rowLayout(nc=2)
        cmds.text(label='')

        self.newExpButton = cmds.button(label='Add Expression', command=self.createExpression)
        cmds.setParent('..')  # rowLayout

        cmds.rowLayout(nc=2)
        cmds.text(label="")

        cmds.rowLayout(nc=2, columnAlign=[(1, 'left'), (2, 'left')], columnAttach=[(1, 'both', 0), (2, 'both', 0)])
        cmds.text(label="Match")
        cmds.text(label="Replace")
        cmds.setParent('..')  # rowLayout

        cmds.setParent("..")  # rowLayout

        cmds.frameLayout(labelVisible=False, collapsable=False)
        self.matchCol = cmds.columnLayout(adjustableColumn=True)

        cmds.setParent('..')  # columnLayout
        cmds.setParent('..')  # frameLayout

        cmds.setParent('..')  # columnLayout
        cmds.setParent('..')  # frameLayout

        self.expressionReplace(nodeAttr)

        cmds.setUITemplate('attributeEditorTemplate', popTemplate=True)

    def expressionReplace(self, nodeAttr):
        self._setActiveNodeAttr(nodeAttr)

        for ctrl in self._matchCtrls:
            cmds.deleteUI(ctrl)
        self._matchCtrls = []

        cmds.setUITemplate('attributeEditorTemplate', pushTemplate=True)
        cmds.setParent(self.matchCol)
        attrSize = melUtils.getAttrNumElements(*nodeAttr.split('.', 1))

        for i in range(attrSize):
            matchAttrName = '{}.match[{}]'.format(self.nodeName, i)
            replaceAttrName = '{}.replace[{}]'.format(self.nodeName, i)
            attrLabel = 'Expression[{}]'.format(i)
            layout = cmds.rowLayout(nc=3,
                                    columnWidth=[(3, 25)],
                                    adjustableColumn=2,
                                    parent=self.matchCol)
            cmds.text(label=attrLabel)

            cmds.rowLayout(nc=2, columnAlign=(1, 'left'), columnAttach=[(1, 'both', 0), (2, 'both', 0)])
            cmds.textField(text=cmds.getAttr(matchAttrName), tcc=lambda x, z=i: self.setExpression(z, x, "match"))
            cmds.textField(text=cmds.getAttr(replaceAttrName), tcc=lambda x, z=i: self.setExpression(z, x, "replace"))
            cmds.setParent("..")

            cmds.symbolButton(image='SP_TrashIcon.png', command=lambda x, z=i: self.deleteExpression(z))

            self._matchCtrls.append(layout)
            cmds.setParent("..")

    def setup(self):

        self.beginScrollLayout()

        self.beginLayout('Operator', collapse=False)
        self.addControl("enable")
        self.addControl("inputs")
        self.addControl("selection")
        self.endLayout()
        self.addCustom("match", self.expressionNew, self.expressionReplace)

        self.addOperatorInputs()
        maya.mel.eval('AEdependNodeTemplate '+self.nodeName)

        self.addExtraControls()

        self.endScrollLayout()

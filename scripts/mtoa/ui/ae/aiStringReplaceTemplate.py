import maya.mel
import maya.cmds as cmds
import mtoa.melUtils as melUtils
from mtoa.ui.ae.utils import AttrControlGrp
from mtoa.ui.ae.operatorTemplate import OperatorAETemplate


class AEaiStringReplaceTemplate(OperatorAETemplate):

    def setup(self):

        self.beginScrollLayout()

        self.beginLayout('Operator', collapse=False)
        self.addControl("enable")
        self.addControl("inputs")
        self.addControl("selection")
        self.addControl("os", label="OS")
        self.addSeparator()
        self.addControl("match")
        self.addControl("replace")
        self.endLayout()

        self.addOperatorInputs()
        maya.mel.eval('AEdependNodeTemplate '+self.nodeName)

        self.addExtraControls()

        self.endScrollLayout()

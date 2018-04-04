import maya.mel
from mtoa.ui.ae.operatorTemplate import OperatorAETemplate

class AEaiSetParameterTemplate(OperatorAETemplate):

    def setup(self):
    	self.beginScrollLayout()
        
        self.beginLayout('Operator', collapse=False)
        self.addControl("enable")
        self.addControl("selection")
        self.endLayout()
        self.beginLayout('Set Parameter', collapse=False)
        self.addControl("assignment")
        self.endLayout()
        self.addOperatorInputs()
        
        maya.mel.eval('AEdependNodeTemplate '+self.nodeName)

        self.addExtraControls()

        self.endScrollLayout()


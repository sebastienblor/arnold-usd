import maya.mel
from mtoa.ui.ae.operatorTemplate import OperatorAETemplate

class AEaiSwitchOperatorTemplate(OperatorAETemplate):

    def setup(self):
    	self.beginScrollLayout()
        
        self.beginLayout('Operator', collapse=False)
        self.addControl("enable")
        self.endLayout()
        self.beginLayout('Switch', collapse=False)
        self.addControl("index")
        self.endLayout()
        self.addOperatorInputs()

        maya.mel.eval('AEdependNodeTemplate '+self.nodeName)

        self.addExtraControls()

        self.endScrollLayout()


import maya.mel
from mtoa.ui.ae.operatorTemplate import OperatorAETemplate

class AEaiMergeTemplate(OperatorAETemplate):
    
    def setup(self):
        self.beginScrollLayout()
        
        self.beginLayout('Operator', collapse=False)
        self.addControl("enable")
        self.endLayout()
        self.addOperatorInputs()
        maya.mel.eval('AEdependNodeTemplate '+self.nodeName)

        self.addExtraControls()

        self.endScrollLayout()


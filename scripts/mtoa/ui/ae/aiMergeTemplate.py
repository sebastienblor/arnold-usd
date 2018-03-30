import maya.mel
from mtoa.ui.ae.shaderTemplate import ShaderAETemplate

class AEaiMergeTemplate(ShaderAETemplate):
	
    def setup(self):
    	self.beginScrollLayout()
        
        self.beginLayout('Operator', collapse=False)
        self.addControl("enable")
        self.addControl("inputs")
        self.endLayout()
        maya.mel.eval('AEdependNodeTemplate '+self.nodeName)

        self.addExtraControls()

        self.endScrollLayout()


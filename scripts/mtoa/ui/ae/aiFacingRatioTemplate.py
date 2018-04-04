import maya.mel
from mtoa.ui.ae.shaderTemplate import ShaderAETemplate

class AEaiFacingRatioTemplate(ShaderAETemplate):
    def setup(self):
    	self.addSwatch()
        self.beginScrollLayout()
        
        self.addControl('bias')
        self.addControl('gain')
        self.addControl('linear')
        self.addControl('invert')
        
        maya.mel.eval('AEdependNodeTemplate '+self.nodeName)
        
        self.addExtraControls()
        self.endScrollLayout()

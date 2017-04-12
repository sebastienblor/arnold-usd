import pymel.core as pm
from mtoa.ui.ae.shaderTemplate import ShaderAETemplate

class AEaiPowTemplate(ShaderAETemplate):
    def setup(self):
    	self.addSwatch()
       
        self.beginScrollLayout()

        self.addControl('base')
        self.addControl('exponent')
        
        pm.mel.AEdependNodeTemplate(self.nodeName)
        
        self.addExtraControls()
        self.endScrollLayout()

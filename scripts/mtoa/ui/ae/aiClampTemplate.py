import pymel.core as pm
from mtoa.ui.ae.shaderTemplate import ShaderAETemplate

class AEaiClampTemplate(ShaderAETemplate):
    def setup(self):
    	self.addSwatch()
        self.beginScrollLayout()

        self.addControl('input')
        self.addControl('min')
        self.addControl('max')
        
        pm.mel.AEdependNodeTemplate(self.nodeName)
        
        self.addExtraControls()
        self.endScrollLayout()

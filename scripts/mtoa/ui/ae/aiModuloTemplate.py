import pymel.core as pm
from mtoa.ui.ae.shaderTemplate import ShaderAETemplate

class AEaiModuloTemplate(ShaderAETemplate):
    def setup(self):
    	self.addSwatch()
        self.beginScrollLayout()

        self.addControl('input')
        self.addControl('divisor')
        
        pm.mel.AEdependNodeTemplate(self.nodeName)
        
        self.addExtraControls()
        self.endScrollLayout()

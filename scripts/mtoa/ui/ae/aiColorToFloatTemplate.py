import pymel.core as pm
from mtoa.ui.ae.shaderTemplate import ShaderAETemplate

class AEaiColorToFloatTemplate(ShaderAETemplate):

    def setup(self):
    	self.addSwatch()
        self.beginScrollLayout()
        
        self.addControl("input")
        self.addControl("mode")
        
        pm.mel.AEdependNodeTemplate(self.nodeName)
        self.addExtraControls()

        self.endScrollLayout()


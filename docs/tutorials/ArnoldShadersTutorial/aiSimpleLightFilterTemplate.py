import pymel.core as pm
from mtoa.ui.ae.shaderTemplate import ShaderAETemplate

class AEaiSimpleLightFilterTemplate(ShaderAETemplate):
    def setup(self):
        self.beginScrollLayout()

        self.beginLayout("Parameters", collapse=False)
        self.addControl('lightIntensity', label="Light Intensity")

        self.endLayout()
        
        pm.mel.AEdependNodeTemplate(self.nodeName)
        self.addExtraControls()
        self.endScrollLayout()
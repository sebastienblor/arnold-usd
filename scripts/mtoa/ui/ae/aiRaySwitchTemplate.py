import pymel.core as pm
from mtoa.ui.ae.shaderTemplate import ShaderAETemplate

class AEaiRaySwitchTemplate(ShaderAETemplate):
    def setup(self):
        self.addSwatch()

        self.beginScrollLayout()

        self.addCustom('message', 'AEshaderTypeNew', 'AEshaderTypeReplace')

        self.addControl('camera', label='Camera')
        self.addControl('shadow', label='Shadow')
        self.addControl('diffuse_reflection', label='Diffuse Reflection')
        self.addControl('specular_reflection', label='Specular Reflection')
        self.addControl('diffuse_transmission', label='Diffuse Transmission')
        self.addControl('specular_transmission', label='Specular Transmission')
        self.addControl('volume', label='Volume')
                
        pm.mel.AEdependNodeTemplate(self.nodeName)
        self.addExtraControls()
        self.endScrollLayout()

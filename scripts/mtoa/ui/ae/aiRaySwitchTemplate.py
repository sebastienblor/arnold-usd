import pymel.core as pm
from mtoa.ui.ae.shaderTemplate import ShaderAETemplate

class AEaiRaySwitchTemplate(ShaderAETemplate):
    def setup(self):
        self.addSwatch()

        self.beginScrollLayout()

        self.addCustom('message', 'AEshaderTypeNew', 'AEshaderTypeReplace')

        self.beginLayout("Ray Types", collapse=False)

        self.addControl('camera', label='Camera')
        self.addControl('shadow', label='Shadow')
        self.addControl('diffuseReflection', label='Diffuse Reflection')
        self.addControl('specularReflection', label='Specular Reflection')
        self.addControl('diffuseTransmission', label='Diffuse Transmission')
        self.addControl('specularTransmission', label='Specular Transmission')
        self.addControl('volume', label='Volume')
        self.endLayout()
        
        pm.mel.AEdependNodeTemplate(self.nodeName)
        self.addExtraControls()
        self.endScrollLayout()

import pymel.core as pm
from mtoa.ui.ae.shaderTemplate import ShaderAETemplate

class AEaiVolumeCollectorTemplate(ShaderAETemplate):
    def setup(self):
        self.beginScrollLayout()
        
        self.beginLayout('Scattering', collapse=False)
        self.addControl('scattering', label='Scattering')
        self.addControl('scattering_channel')
        self.addControl('scattering_color')
        self.addControl('scattering_intensity')
        self.addControl('anisotropy')
        self.endLayout()
        
        self.beginLayout('Attenuation', collapse=False)
        self.addControl('attenuation_mode', label='Attenuation Mode')
        self.addControl('attenuation_use_scattering')
        self.addControl('attenuation')
        self.addControl('attenuation_channel')
        self.addControl('attenuation_color')
        self.addControl('attenuation_intensity')
        self.endLayout()
        
        self.beginLayout('Emission', collapse=False)
        self.addControl('emission', label='Emission')
        self.addControl('emission_channel')
        self.addControl('emission_color')
        self.addControl('emission_intensity')
        self.endLayout()
        
        self.beginLayout('Sampling', collapse=False)
        self.addControl('position_offset')
        self.addControl('interpolation')
        self.endLayout()
               
        pm.mel.AEdependNodeTemplate(self.nodeName)

        self.addExtraControls()
        self.endScrollLayout()

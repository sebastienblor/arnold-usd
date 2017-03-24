import pymel.core as pm
from mtoa.ui.ae.shaderTemplate import ShaderAETemplate

class AEaiAtmosphereVolumeTemplate(ShaderAETemplate):
    def setup(self):
        self.addSwatch()
        
        self.beginScrollLayout()
        
        self.beginLayout('Volume Attributes', collapse=False)
        self.addControl('density', label='Density')
        self.addControl('rgbDensity', label='Color')
        self.addSeparator()
        self.addControl('attenuation', label='Attenuation')
        self.addControl('rgbAttenuation', label='Attenuation Color')
        self.addSeparator()
        self.addControl('eccentricity', label='Anisotropy')
        self.addSeparator()
        self.addControl('samples', label='Samples')
        self.endLayout()
        
        self.beginLayout('Contribution Attributes')
        self.addControl('affectCamera', label='Camera')
        self.addControl('affectDiffuse', label='Diffuse')
        self.addControl('affectSpecular', label='Specular')
        self.endLayout()
        
        pm.mel.AEdependNodeTemplate(self.nodeName)
        
        self.addExtraControls()
        self.endScrollLayout()


import pymel.core as pm
from mtoa.ui.ae.shaderTemplate import ShaderAETemplate

class AEaiDensityTemplate(ShaderAETemplate):
    def setup(self):
        self.beginScrollLayout()
        
        self.beginLayout('Attributes', collapse=False)
        self.addControl('scatter_channel', label='Scatter Channel')
        self.addControl('scatter_color', label='Scatter Color')
        self.addControl('scatter_g', label='Scatter Anisotropy')
        self.addSeparator()
        self.addControl('absorption_channel', label='Absorption Channel')
        self.addControl('absorption_color', label='Absorption Color')
        self.addSeparator()
        self.addControl('emission_channel', label='Emission Channel')
        self.addControl('emission_color', label='Emission Color')
        self.addSeparator()
        self.addControl('interpolation', label='Interpolation')
        self.addControl('position_offset', label='Position Offset')
        self.endLayout()
               
        pm.mel.AEdependNodeTemplate(self.nodeName)

        self.addExtraControls()
        self.endScrollLayout()

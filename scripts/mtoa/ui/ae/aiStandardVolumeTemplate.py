import pymel.core as pm
import mtoa.utils as utils
import mtoa.ui.ae.utils as aeUtils
from mtoa.ui.ae.shaderTemplate import ShaderAETemplate


class AEaiStandardVolumeTemplate(ShaderAETemplate):
    convertToMayaStyle = True
   
    def setup(self):
        self.addSwatch()

        self.beginScrollLayout()

        self.addCustom('message', 'AEshaderTypeNew', 'AEshaderTypeReplace')
        
        self.beginLayout("Volume", collapse=False)
        self.addControl("density",  label="Density", annotation="Volume Density")
        self.addControl("density_channel", label="Density Channel", annotation="Density Channel")
        self.addControl("interpolation", label="Interpolation", annotation="Interpolation")
        self.addControl("displacement", label="Displacement", annotation="Displacement")
        self.endLayout()

        self.beginLayout("Scatter", collapse=False)
        self.addControl("scatter",  label="Weight", annotation="Scatter")
        self.addControl("scatter_color", label="Color", annotation="Scatter Color")
        self.addControl("scatter_color_channel", label="Color Channel", annotation="Scatter Color Channel")
        self.addControl("scatter_anisotropy", label="Anisotropy", annotation="Scatter Anisotropy")
        self.endLayout()

        self.beginLayout("Transparent", collapse=False)
        self.addControl("transparent",  label="Weight", annotation="Transparent Weight")
        self.addControl("transparent_depth", label="Depth", annotation="Transparent Depth")
        self.addControl("transparent_channel", label="Channel", annotation="Transparent Channel")
        self.endLayout()

        self.beginLayout("Emission", collapse=False)
        self.addControl("emission_mode",  label="Mode", annotation="Emission Mode")
        self.addControl("emission", label="Weight", annotation="Emission Weight")
        self.addControl("emission_color", label="Color", annotation="Emission Color")
        self.addControl("emission_channel", label="Channel", annotation="Emission Channel")

        self.beginLayout("Temperature", collapse=False)
        self.addControl("temperature",  label="Temperature", annotation="Temperature")
        self.addControl("temperature_channel", label="Channel", annotation="Temperature Channel")
        self.addControl("blackbody_kelvin", label="Blackbody Kelvin", annotation="Blackbody Kelvin")
        self.addControl("blackbody_intensity", label="Blackbody Intensity", annotation="Blackbody Intensity")

        self.endLayout()
        self.endLayout()


        # include/call base class/node attributes
        pm.mel.AEdependNodeTemplate(self.nodeName)

        self.suppress('PhongExponent')
       
        self.addExtraControls()
        self.endScrollLayout()


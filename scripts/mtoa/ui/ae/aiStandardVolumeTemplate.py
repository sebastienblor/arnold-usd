import maya.mel
import mtoa.utils as utils
import mtoa.ui.ae.utils as aeUtils
from mtoa.ui.ae.shaderTemplate import ShaderAETemplate


class AEaiStandardVolumeTemplate(ShaderAETemplate):
    convertToMayaStyle = True
   
    def setup(self):
        self.addSwatch()

        self.beginScrollLayout()
        
        self.beginLayout("Volume", collapse=False)
        self.addControl("density",  label="Density", annotation="Volume Density")
        self.addControl("densityChannel", label="Density Channel", annotation="Density Channel")
        self.addControl("interpolation", label="Interpolation", annotation="Interpolation")
        self.addControl("displacement", label="Displacement", annotation="Displacement")
        self.endLayout()

        self.beginLayout("Scatter", collapse=False)
        self.addControl("scatter",  label="Weight", annotation="Scatter")
        self.addControl("scatterColor", label="Color", annotation="Scatter Color")
        self.addControl("scatterColorChannel", label="Color Channel", annotation="Scatter Color Channel")
        self.addControl("scatterAnisotropy", label="Anisotropy", annotation="Scatter Anisotropy")
        self.endLayout()

        self.beginLayout("Transparent", collapse=False)
        self.addControl("transparent",  label="Weight", annotation="Transparent Weight")
        self.addControl("transparentDepth", label="Depth", annotation="Transparent Depth")
        self.addControl("transparentChannel", label="Channel", annotation="Transparent Channel")
        self.endLayout()

        self.beginLayout("Emission", collapse=False)
        self.addControl("emissionMode",  label="Mode", annotation="Emission Mode")
        self.addControl("emission", label="Weight", annotation="Emission Weight")
        self.addControl("emissionColor", label="Color", annotation="Emission Color")
        self.addControl("emissionChannel", label="Channel", annotation="Emission Channel")

        self.beginLayout("Temperature", collapse=False)
        self.addControl("temperature",  label="Temperature", annotation="Temperature")
        self.addControl("temperatureChannel", label="Channel", annotation="Temperature Channel")
        self.addControl("blackbodyKelvin", label="Blackbody Kelvin", annotation="Blackbody Kelvin")
        self.addControl("blackbodyIntensity", label="Blackbody Intensity", annotation="Blackbody Intensity")

        self.endLayout()
        self.endLayout()


        # include/call base class/node attributes
        maya.mel.eval('AEdependNodeTemplate '+self.nodeName)

        self.suppress('PhongExponent')
       
        self.addExtraControls()
        self.endScrollLayout()


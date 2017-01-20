import pymel.core as pm
import mtoa.utils as utils
import mtoa.ui.ae.utils as aeUtils
from mtoa.ui.ae.shaderTemplate import ShaderAETemplate


class AEaiStandardHairTemplate(ShaderAETemplate):
    convertToMayaStyle = True
   
    def setup(self):
        self.addSwatch()

        self.beginScrollLayout()

        self.addCustom('message', 'AEshaderTypeNew', 'AEshaderTypeReplace')
        

        self.beginLayout("Color", collapse=False)
        self.addControl("base",  label="Base", annotation="Base Weight")
        self.addControl("base_color", label="Base Color", annotation="Base Color")
        self.addSeparator()
        self.addControl("melanin",  label="Melanin", annotation="Melanin")
        self.addControl("melanin_redness",  label="Melanin Redness", annotation="Melanin Redness")
        self.addControl("melanin_randomize",  label="Melanin Randomize", annotation="Melanin Randomize")
        self.endLayout()

        self.beginLayout("Specular", collapse=False)
        self.addControl("roughness",  label="Roughness", annotation="Roughness")
        self.addControl("ior",  label="IOR", annotation="IOR")
        self.addControl("shift",  label="Shift", annotation="IOR")
        self.beginLayout("Tint", collapse=True)
        self.addControl("specular",  label="Specular", annotation="Specular")
        self.addControl("specular_tint",  label="Specular Tint", annotation="Specular Tint")
        self.addSeparator()
        self.addControl("specular2",  label="2nd Specular", annotation="2nd Specular")
        self.addControl("specular2_tint",  label="2nd Specular Tint", annotation="2dn Specular Tint")
        self.addSeparator()
        self.addControl("transmission",  label="Transmission", annotation="Transmission")
        self.addControl("transmission_tint",  label="Transmission Tint", annotation="Transmission Tint")
        self.endLayout()
        self.endLayout()

        self.beginLayout("Diffuse", collapse=True)
        self.addControl("diffuse",  label="Diffuse", annotation="Specular")
        self.addControl("diffuse_color",  label="Diffuse Color", annotation="Specular Tint")
        self.endLayout()

        self.beginLayout("Emission", collapse=True)
        self.addControl("emission",  label="Emission", annotation="Emission")
        self.addControl("emission_color",  label="Emission Color", annotation="Emission Color")
        self.endLayout()

        self.addControl("opacity",  label="Opacity", annotation="Opacity")

        self.beginLayout("Matte", collapse=True)
        self.addControl("aiEnableMatte", label="Enable Matte")
        self.addControl("aiMatteColor", label="Matte Color")
        self.addControl("aiMatteColorA", label="Matte Opacity")
        self.endLayout()

        self.beginLayout("Advanced", collapse=True)
        self.addControl("indirect_diffuse",  label="Indirect Diffuse", annotation="Indirect Diffuse")
        self.addControl("indirect_specular",  label="Indirect Specular", annotation="Indirect Specular")
        self.endLayout()


        # include/call base class/node attributes
        pm.mel.AEdependNodeTemplate(self.nodeName)

        self.suppress('PhongExponent')
       
        self.addExtraControls()
        self.endScrollLayout()


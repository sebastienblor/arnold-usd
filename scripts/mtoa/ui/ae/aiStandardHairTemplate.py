import maya.mel
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
        self.addControl("baseColor", label="Base Color", annotation="Base Color")
        self.addSeparator()
        self.addControl("melanin",  label="Melanin", annotation="Melanin")
        self.addControl("melaninRedness",  label="Melanin Redness", annotation="Melanin Redness")
        self.addControl("melaninRandomize",  label="Melanin Randomize", annotation="Melanin Randomize")
        self.endLayout()

        self.beginLayout("Specular", collapse=False)
        self.addControl("roughness",  label="Roughness", annotation="Roughness")
        self.addControl("ior",  label="IOR", annotation="IOR")
        self.addControl("shift",  label="Shift", annotation="IOR")
        self.beginLayout("Tint", collapse=True)
        self.addControl("specularTint",  label="Specular Tint", annotation="Specular Tint")
        self.addSeparator()
        self.addControl("specular2Tint",  label="2nd Specular Tint", annotation="2dn Specular Tint")
        self.addSeparator()
        self.addControl("transmissionTint",  label="Transmission Tint", annotation="Transmission Tint")
        self.endLayout()
        self.endLayout()

        self.beginLayout("Diffuse", collapse=True)
        self.addControl("diffuse",  label="Diffuse", annotation="Specular")
        self.addControl("diffuseColor",  label="Diffuse Color", annotation="Specular Tint")
        self.endLayout()

        self.beginLayout("Emission", collapse=True)
        self.addControl("emission",  label="Emission", annotation="Emission")
        self.addControl("emissionColor",  label="Emission Color", annotation="Emission Color")
        self.endLayout()

        self.addControl("opacity",  label="Opacity", annotation="Opacity")

        self.beginLayout("Matte", collapse=True)
        self.addControl("aiEnableMatte", label="Enable Matte")
        self.addControl("aiMatteColor", label="Matte Color")
        self.addControl("aiMatteColorA", label="Matte Opacity")
        self.endLayout()

        self.beginLayout("Advanced", collapse=True)
        self.addControl("indirectDiffuse",  label="Indirect Diffuse", annotation="Indirect Diffuse")
        self.addControl("indirectSpecular",  label="Indirect Specular", annotation="Indirect Specular")
        self.addControl("extraDepth",  label="Extra Depth", annotation="Extra Depth")
        self.endLayout()


        # include/call base class/node attributes
        maya.mel.eval('AEdependNodeTemplate '+self.nodeName)

        self.suppress('PhongExponent')
       
        self.addExtraControls()
        self.endScrollLayout()


import maya.mel
import mtoa.utils as utils
import mtoa.ui.ae.utils as aeUtils
import maya.cmds as cmds
from mtoa.ui.ae.shaderTemplate import ShaderAETemplate


class AEaiStandardHairTemplate(ShaderAETemplate):
    convertToMayaStyle = True
   
    def updateAnisotropicRoughness(self, nodeName):
        anisoAttr = '%s.%s' % (nodeName, 'roughnessAnisotropic')
        anisoValue = cmds.getAttr(anisoAttr)
        cmds.editorTemplate(dimControl=(nodeName, 'roughnessAzimuthal', not anisoValue))

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
        self.addControl("roughnessAnisotropic",  label="Anisotropic Roughness", annotation="Anisotropic Roughness", changeCommand=self.updateAnisotropicRoughness)
        self.addControl("roughnessAzimuthal",  label="Azimuthal Roughness", annotation="Azimuthal Roughness")
        self.updateAnisotropicRoughness(self.nodeName)
        
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
        
        self.beginLayout("AOVs", collapse=True)
        self.addControl("aov_id1", label="ID 1 AOV")
        self.addControl("id1", label="ID 1")
        self.addSeparator()
        self.addControl("aov_id2", label="ID 2 AOV")
        self.addControl("id2", label="ID 2")
        self.addSeparator()
        self.addControl("aov_id3", label="ID 3 AOV")
        self.addControl("id3", label="ID 3")
        self.addSeparator()
        self.addControl("aov_id4", label="ID 4 AOV")
        self.addControl("id4", label="ID 4")
        self.addSeparator()
        self.addControl("aov_id5", label="ID 5 AOV")
        self.addControl("id5", label="ID 5")
        self.addSeparator()
        self.addControl("aov_id6", label="ID 6 AOV")
        self.addControl("id6", label="ID 6")
        self.addSeparator()
        self.addControl("aov_id7", label="ID 7 AOV")
        self.addControl("id7", label="ID 7")
        self.addSeparator()
        self.addControl("aov_id8", label="ID 8 AOV")
        self.addControl("id8", label="ID 8")
        self.endLayout()

        self.beginLayout("Advanced", collapse=True)
        self.addControl("indirectDiffuse",  label="Indirect Diffuse", annotation="Indirect Diffuse")
        self.addControl("indirectSpecular",  label="Indirect Specular", annotation="Indirect Specular")
        self.addControl("extraDepth",  label="Extra Depth", annotation="Extra Depth")
        self.addControl("extraSamples",  label="Extra Samples", annotation="Extra Samples")
        self.endLayout()


        # include/call base class/node attributes
        maya.mel.eval('AEdependNodeTemplate '+self.nodeName)

        self.suppress('PhongExponent')
       
        self.addExtraControls()
        self.endScrollLayout()


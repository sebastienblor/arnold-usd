import pymel.core as pm
from mtoa.ui.ae.shaderTemplate import ShaderAETemplate

class AEaiShadowMatteTemplate(ShaderAETemplate):
    
    def setup(self):
        self.addSwatch()
        self.beginScrollLayout()

        self.addCustom('message', 'AEshaderTypeNew', 'AEshaderTypeReplace')

        self.addControl("background", label="Background")
        self.addControl("backgroundType", label="Background Type")
        self.addControl("offscreenColor", label="Offscreen Color")

        self.beginLayout("Shadows", collapse=False)
        self.addControl("shadowColor", label="Shadow Color")
        self.addControl("shadowOpacity", label="Shadow Opacity")
        self.addControl("backlighting", label="Backlighting")
        self.addControl("enableTransparency", label="Enable Transparency")
        self.addControl("shadowTransparency", label="Shadow Transparency")
        self.endLayout()

        self.beginLayout("Diffuse", collapse=True)
        self.addControl("diffuseColor", label="Color")
        self.addControl("diffuseIntensity", label="Intensity")
        self.addControl("diffuseUseBackground", label="Use Background")
        self.addControl("indirectDiffuseEnable", label="Indirect Diffuse")
        self.endLayout()

        self.beginLayout("Specular", collapse=True)
        self.addControl("specularColor", label="Color")
        self.addControl("specularIntensity", label="Intensity")
        self.addControl("specularRoughness", label="Roughness")
        self.addControl("indirectSpecularEnable", label="Indirect Specular")
        self.endLayout()

        self.beginLayout("AOVs", collapse=True)
        self.addControl("aovShadow", label="Shadow")
        self.addControl("aovShadowDiff", label="Shadow Diff")
        self.addControl("aovShadowMask", label="Shadow Mask")
        self.addControl("aovIndirectDiffuse", label="Indirect Diffuse")
        self.addControl("aovIndirectSpecular", label="Indirect Specular")

        self.endLayout()

        pm.mel.AEdependNodeTemplate(self.nodeName)

        self.addExtraControls()
        self.endScrollLayout()


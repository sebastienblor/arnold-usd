import pymel.core as pm
import mtoa.utils as utils
import mtoa.ui.ae.utils as aeUtils
from mtoa.ui.ae.shaderTemplate import ShaderAETemplate


class AEaiSkinTemplate(ShaderAETemplate):

    def checkPrimarySpecularFresnel(self, nodeName):
        fullAttr = '%s.%s'%(nodeName, "primary_specular_enable_fresnel_falloff")
        useFresnel = pm.getAttr(fullAttr)
        dim = not useFresnel
        pm.editorTemplate(dimControl=(nodeName, "primarySpecularFresnelWeight", dim))
        pm.editorTemplate(dimControl=(nodeName, "primarySpecularIor", dim))

    def checkSecondarySpecularFresnel(self, nodeName):
        fullAttr = '%s.%s'%(nodeName, "secondary_specular_enable_fresnel_falloff")
        useFresnel = pm.getAttr(fullAttr)
        dim = not useFresnel
        pm.editorTemplate(dimControl=(nodeName, "secondarySpecularFresnelWeight", dim))
        pm.editorTemplate(dimControl=(nodeName, "secondarySpecularIor", dim))

    def setup(self):
        self.addSwatch()
        
        self.beginScrollLayout()
        
        self.addCustom('message', 'AEshaderTypeNew', 'AEshaderTypeReplace')
        
        self.beginLayout("Diffuse", collapse=False)     
        self.addControl("diffuse_color",  label="Color", annotation="Diffuse Color")
        self.addControl("diffuse_weight", label="Weight")
        self.addControl("diffuse_roughness", label="Roughness")
        self.endLayout()
        
        self.beginLayout("SSS", collapse=False)
        self.addControl("sss_weight", label="SSS Weight")
        self.endLayout()

        self.beginLayout("Shallow Scatter", collapse=False)
        self.addControl("shallow_scatter_color", label="Color")
        self.addControl("shallow_scatter_weight", label="Weight")
        self.addControl("shallow_scatter_radius", label="Radius")
        self.endLayout()
        
        self.beginLayout("Mid Scatter", collapse=False)
        self.addControl("mid_scatter_color", label="Color")
        self.addControl("mid_scatter_weight", label="Weight")
        self.addControl("mid_scatter_radius", label="Radius")
        self.endLayout()
        
        self.beginLayout("Deep Scatter", collapse=False)
        self.addControl("deep_scatter_color", label="Color")
        self.addControl("deep_scatter_weight", label="Weight")
        self.addControl("deep_scatter_radius", label="Radius")
        self.endLayout()
        
        self.beginLayout("Primary Specular", collapse=False)
        self.addControl("primary_specular_color", label="Color")
        self.addControl("primary_specular_weight", label="Weight")
        self.addControl("primary_specular_roughness", label="Roughness")
        self.beginLayout("Fresnel")
        self.addControl("primary_specular_enable_fresnel_falloff", changeCommand=self.checkPrimarySpecularFresnel, label="Enable")
        self.addControl("primary_specular_fresnel_weight", label="Weight")
        self.addControl("primary_specular_ior", label="IOR")
        self.endLayout()
        self.endLayout()
        
        self.beginLayout("Secondary Specular", collapse=False)
        self.addControl("secondary_specular_color", label="Color")
        self.addControl("secondary_specular_weight", label="Weight")
        self.addControl("secondary_specular_roughness", label="Roughness")
        self.beginLayout("Fresnel")
        self.addControl("secondary_specular_enable_fresnel_falloff", changeCommand=self.checkSecondarySpecularFresnel, label="Enable")
        self.addControl("secondary_specular_fresnel_weight", label="Weight")
        self.addControl("secondary_specular_ior", label="IOR")
        self.endLayout()
        self.endLayout()
        
        self.beginLayout("Options", collapse=True)
        self.addControl("global_sss_radius_multiplier", label="SSS Radius Multiplier")
        self.addControl("combine_sss_queries", label="Combine SSS Queries")
        self.addControl("sample_sss_only_in_gi_rays", label="Sample only SSS in GI rays")
        self.addControl("sample_sss_only_in_glossy_rays", label="Sample only SSS in Glossy rays")
        self.endLayout()

        self.addBumpLayout()
        
        self.beginLayout("Hardware Texturing", collapse=True)
        pm.mel.eval('AEhardwareTextureTemplate "%s"' % self.nodeName + r'("color")')
        self.endLayout()

        self.addAOVLayout()
        
        # include/call base class/node attributes
        pm.mel.AEdependNodeTemplate(self.nodeName)
        
        self.addExtraControls()
        
        self.endScrollLayout()

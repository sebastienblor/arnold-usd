import pymel.core as pm
import mtoa.utils as utils
import mtoa.ui.ae.utils as aeUtils
from mtoa.ui.ae.shaderTemplate import ShaderAETemplate


class AEaiSkinTemplate(ShaderAETemplate):

    def checkPrimarySpecularFresnel(self, nodeName):
        fullAttr = '%s.%s'%(nodeName, "specular_enable_fresnel_falloff")
        useFresnel = pm.getAttr(fullAttr)
        dim = not useFresnel
        pm.editorTemplate(dimControl=(nodeName, "specularIor", dim))

    def checkSecondarySpecularFresnel(self, nodeName):
        fullAttr = '%s.%s'%(nodeName, "coat_enable_fresnel_falloff")
        useFresnel = pm.getAttr(fullAttr)
        dim = not useFresnel
        pm.editorTemplate(dimControl=(nodeName, "coatIor", dim))

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

        self.endLayout()
        
        self.beginLayout("Specular", collapse=False)
        self.addControl("specular_color", label="Color")
        self.addControl("specular_weight", label="Weight")
        self.addControl("specular_roughness", label="Roughness")
        self.addControl("specular_ior", label="IOR")
        self.endLayout()
        
        self.beginLayout("Coat Layer", collapse=False)
        self.addControl("coat_color", label="Color")
        self.addControl("coat_weight", label="Weight")
        self.addControl("coat_roughness", label="Roughness")
        self.addControl("coat_ior", label="IOR")
        self.endLayout()
        
        self.beginLayout("Options", collapse=True)
        self.addControl("global_sss_radius_multiplier", label="SSS Radius Multiplier")
        self.addControl("combine_sss_queries", label="Combine SSS Queries")
        self.addControl("sample_sss_only_in_gi_rays", label="Sample only SSS in GI rays")
        self.addControl("sample_sss_only_in_glossy_rays", label="Sample only SSS in Glossy rays")
        self.endLayout()

        self.addBumpLayout()
        
        self.beginLayout("Hardware Texturing", collapse=True)
        
        self.endLayout()

        self.addAOVLayout(aovReorder = ['diffuse', 'specular', 'coat', 'sss'])
        
        # include/call base class/node attributes
        pm.mel.AEdependNodeTemplate(self.nodeName)
        
        self.addExtraControls()
        
        self.endScrollLayout()

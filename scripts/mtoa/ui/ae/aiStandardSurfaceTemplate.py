import pymel.core as pm
import mtoa.utils as utils
import mtoa.ui.ae.utils as aeUtils
from mtoa.ui.ae.shaderTemplate import ShaderAETemplate


class AEaiStandardSurfaceTemplate(ShaderAETemplate):
    convertToMayaStyle = True
   
    def setup(self):
        self.addSwatch()

        self.beginScrollLayout()

        self.addCustom('message', 'AEshaderTypeNew', 'AEshaderTypeReplace')
        

        self.beginLayout("Base", collapse=False)
        self.addControl("base",  label="Weight", annotation="Base Weight")
        self.addControl("base_color", label="Color", annotation="Base Color")
        self.addControl("diffuse_roughness", label="Diffuse Roughness", annotation="Diffuse Oren-Nayar Roughness")
        self.endLayout()

        self.beginLayout("Specular", collapse=False)
        self.addControl("specular",  label="Weight", annotation="Specular Weight")
        self.addControl("specular_color", label="Color", annotation="Specular Color")
        self.addControl("specular_roughness", label="Roughness", annotation="Specular Roughness")
        self.addSeparator()
        self.addControl("specular_IOR", label="IOR", annotation="Specular IOR")
        self.addSeparator()
        self.addControl("specular_anisotropy", label="Anisotropy", annotation="Specular Anisotropy")
        self.addControl("specular_rotation", label="Rotation", annotation="Specular Anisotropy Rotation")
        self.addSeparator()
        self.addControl("metalness", label="Metalness", annotation="Specular Metalness")
        self.endLayout()

        self.beginLayout("Coat", collapse=True)
        self.addControl("coat",  label="Weight", annotation="Coat Weight")
        self.addControl("coat_color", label="Color", annotation="Coat Color")
        self.addControl("coat_roughness", label="Roughness", annotation="Coat Roughness")
        self.addSeparator()
        self.addControl("coat_IOR", label="IOR", annotation="Coat IOR")
        self.addSeparator()
        self.addControl("coat_normal", label="Normal", annotation="Coat Normal")
        #self.addSeparator()
        #self.addControl("coat_affect_color", label="Affect Color", annotation="Coat Affect Color")
        #self.addControl("coat_affect_roughness", label="Affect Roughness", annotation="Coat Affect Roughness")
        self.endLayout()

        self.beginLayout("Transparent", collapse=True)
        self.addControl("opacity",  label="Opacity", annotation="Opacity")
        self.addControl("transparent",  label="Weight", annotation="Transparent Weight")
        self.addControl("transparent_color", label="Color", annotation="Transparent Color")
        self.addSeparator()
        self.addControl("transparent_depth", label="Depth", annotation="Transparent Depth")
        self.addControl("transparent_scatter", label="Scatter", annotation="Transparent Scatter")
        self.addControl("transparent_scatter_anisotropy", label="Scatter Anisotropy", annotation="Transparent Scatter Anisotropy")
        self.addSeparator()
        self.addControl("transparent_extra_roughness", label="Extra-Roughness", annotation="Transparent Extra-Roughness")
        self.addSeparator()
        self.addControl("transparent_dispersion", label="Dispersion", annotation="Transparent Dispersion")
        self.endLayout()

        self.beginLayout("Subsurface", collapse=True)
        self.addControl("subsurface",  label="Weight", annotation="SSS Weight")
        self.addControl("subsurface_color", label="Color", annotation="SSS Color")
        self.addSeparator()
        self.addControl("subsurface_radius", label="Radius", annotation="SSS Radius")
        self.addControl("subsurface_scale", label="Scale", annotation="SSS Scale")
        self.addSeparator()
        # FIXME Arnold5 is this parameter related to Scatter ?
        self.addControl("thin_walled", label="Thin Walled", annotation="Thin Walled") 
        self.endLayout() 

        self.beginLayout("Emission", collapse=True)
        self.addControl("emission",  label="Weight", annotation="Emission Weight")
        self.addControl("emission_color", label="Color", annotation="Emission Color")
        self.endLayout() 


        self.addBumpLayout()
# FIXME Arnold5 find how we should expose bump/normals
#        self.beginLayout("Normal", collapse=True)
#        self.addControl("normal", label="Normal", annotation="Normal Shader")
#        self.addControl("tangent", label="Tangent", annotation="Tangent Shader")
#        self.endLayout()

        self.beginLayout("Matte", collapse=True)
        self.addControl("aiEnableMatte", label="Enable Matte")
        self.addControl("aiMatteColor", label="Matte Color")
        self.addControl("aiMatteColorA", label="Matte Opacity")
        self.endLayout()

        self.beginLayout("Advanced", collapse=True)
        self.addControl("caustics", label="Caustics", annotation="Enable Caustics")
        self.addControl("internal_reflections", label="Internal Reflections", annotation="Enable Internal Reflections")
        self.addControl("exit_to_background", label="Exit To Background", annotation="When bounce depth is exceeded, use background color instead")
        self.addSeparator()
        self.addControl("indirect_diffuse", label="Indirect Diffuse", annotation="Indirect Diffuse")
        self.addControl("indirect_specular", label="Indirect Specular", annotation="Indirect Specular")
        self.endLayout()
        

        self.beginLayout("Hardware Texturing", collapse=True)
        pm.mel.eval('AEhardwareTextureTemplate "%s"' % self.nodeName + r'("color emission_color ")')
        self.endLayout()

        #FIXME Arnold5 which are the aovs ?
        self.addAOVLayout(aovReorder = ['direct_diffuse', 'indirect_diffuse', 'direct_specular', 'indirect_specular',
                                        'reflection', 'refraction', 'refraction_opacity', 'emission', 'sss', 'direct_sss', 'indirect_sss'])

        # include/call base class/node attributes
        pm.mel.AEdependNodeTemplate(self.nodeName)

        self.suppress('PhongExponent')
       
        self.addExtraControls()
        self.endScrollLayout()


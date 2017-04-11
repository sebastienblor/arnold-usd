import pymel.core as pm
import mtoa.utils as utils
import mtoa.ui.ae.utils as aeUtils
from mtoa.ui.ae.shaderTemplate import ShaderAETemplate


class AEaiStandardSurfaceTemplate(ShaderAETemplate):
    convertToMayaStyle = True
   
    def changeParams(self, nodeName):
        metal_0 = float(pm.getAttr(nodeName + '.metalness')) == 0.0
        metal_1 = float(pm.getAttr(nodeName + '.metalness')) == 1.0
        transmission_0 = float(pm.getAttr(nodeName + '.transmission')) == 0.0
        transmission_1 = float(pm.getAttr(nodeName + '.transmission')) == 1.0
        subsurface_0 = float(pm.getAttr(nodeName + '.subsurface')) == 0.0
        subsurface_1 = float(pm.getAttr(nodeName + '.subsurface')) == 1.0
        thin_walled = bool(pm.getAttr(nodeName + '.thin_walled'))

        pm.editorTemplate(dimControl=(nodeName, 'specularIOR', metal_1))
        pm.editorTemplate(dimControl=(nodeName, 'diffuseRoughness', metal_1 or transmission_1))

        dim_base = metal_0 and transmission_1
        pm.editorTemplate(dimControl=(nodeName, 'base', dim_base))
        pm.editorTemplate(dimControl=(nodeName, 'baseColor', dim_base))

        dim_transmission = metal_1 or transmission_0
        dim_transmission_interior = dim_transmission or thin_walled
        pm.editorTemplate(dimControl=(nodeName, 'transmission', metal_1))
        pm.editorTemplate(dimControl=(nodeName, 'transmissionColor', dim_transmission))
        pm.editorTemplate(dimControl=(nodeName, 'transmissionDepth', dim_transmission_interior))
        pm.editorTemplate(dimControl=(nodeName, 'transmissionScatter', dim_transmission_interior))
        pm.editorTemplate(dimControl=(nodeName, 'transmissionScatterAnisotropy', dim_transmission_interior))
        pm.editorTemplate(dimControl=(nodeName, 'transmissionDispersion', dim_transmission_interior))
        pm.editorTemplate(dimControl=(nodeName, 'transmissionExtraRoughness', dim_transmission_interior))

        dim_subsurface = metal_1 or transmission_1 or subsurface_0
        dim_subsurface_radius = dim_subsurface or thin_walled
        pm.editorTemplate(dimControl=(nodeName, 'subsurface', metal_1 or transmission_1))
        pm.editorTemplate(dimControl=(nodeName, 'subsurfaceColor', dim_subsurface))
        pm.editorTemplate(dimControl=(nodeName, 'subsurfaceRadius', dim_subsurface_radius))
        pm.editorTemplate(dimControl=(nodeName, 'subsurfaceScale', dim_subsurface_radius))

    def setup(self):
        self.addSwatch()

        self.beginScrollLayout()

        self.addCustom('message', 'AEshaderTypeNew', 'AEshaderTypeReplace')
        
        self.beginLayout("Base", collapse=False)
        self.addControl("base",  label="Weight", annotation="Label Weight")
        self.addControl("baseColor", label="Color", annotation="Label Color")
        self.addSeparator()
        self.addControl("diffuseRoughness", label="Diffuse Roughness", annotation="Diffuse Oren-Nayar Roughness")
        self.addSeparator()
        self.addControl("metalness", label="Metalness", annotation="Metalness Mix", changeCommand=self.changeParams)
        self.endLayout()

        self.beginLayout("Specular", collapse=False)
        self.addControl("specular",  label="Weight", annotation="Specular Weight")
        self.addControl("specularColor", label="Color", annotation="Specular Color")
        self.addControl("specularRoughness", label="Roughness", annotation="Specular Roughness")
        self.addSeparator()
        self.addControl("specularIOR", label="IOR", annotation="Specular IOR")
        self.addSeparator()
        self.addControl("specularAnisotropy", label="Anisotropy", annotation="Specular Anisotropy")
        self.addControl("specularRotation", label="Rotation", annotation="Specular Anisotropy Rotation")
        self.endLayout()

        self.beginLayout("Transmission", collapse=True)
        self.addControl("transmission",  label="Weight", annotation="Transmission Mix", changeCommand=self.changeParams)
        self.addControl("transmissionColor", label="Color", annotation="Transmission Color")
        self.addSeparator()
        self.addControl("transmissionDepth", label="Depth", annotation="Transmission Depth")
        self.addControl("transmissionScatter", label="Scatter", annotation="Transmission Scatter")
        self.addControl("transmissionScatterAnisotropy", label="Scatter Anisotropy", annotation="Transmission Scatter Anisotropy")
        self.addSeparator()
        self.addControl("transmissionDispersion", label="Dispersion Abbe", annotation="Transmission Dispersion Abbe Number")
        self.addControl("transmissionExtraRoughness", label="Extra Roughness", annotation="Transmission Extra Roughness")
        self.endLayout()

        self.beginLayout("Subsurface", collapse=True)
        self.addControl("subsurface",  label="Weight", annotation="Subsurface Scattering Mix", changeCommand=self.changeParams)
        self.addControl("subsurfaceColor", label="Color", annotation="Subsurface Scattering Color")
        self.addControl("subsurfaceRadius", label="Radius", annotation="Subsurface Scattering Radius");
        self.addControl("subsurfaceScale", label="Scale", annotation="Subsurface Scattering Scale");
        self.endLayout() 

        self.beginLayout("Coat", collapse=True)
        self.addControl("coat",  label="Weight", annotation="Coat Weight")
        self.addControl("coatColor", label="Color", annotation="Coat Color")
        self.addControl("coatRoughness", label="Roughness", annotation="Coat Roughness")
        self.addSeparator()
        self.addControl("coatIOR", label="IOR", annotation="Coat IOR")
        self.addSeparator()
        self.addControl("coatNormal", label="Normal", annotation="Coat Normal")
        self.endLayout()

        self.beginLayout("Emission", collapse=True)
        self.addControl("emission",  label="Weight", annotation="Emission Weight")
        self.addControl("emissionColor", label="Color", annotation="Emission Color")
        self.endLayout() 

        self.beginLayout("Geometry", collapse=True)
        self.addControl("thinWalled", label="Thin Walled", annotation="Thin Surface with no Interior", changeCommand=self.changeParams) 
        self.addControl("opacity",  label="Opacity", annotation="Cutout Opacity")
        self.addCustom("normalCamera", self.bumpNew, self.bumpReplace)
        self.addControl("tangent", label="Anisotropy Tangent", annotation="Tangent Shader")
        self.endLayout()

        self.beginLayout("Matte", collapse=True)
        self.addControl("aiEnableMatte", label="Enable Matte")
        self.addControl("aiMatteColor", label="Matte Color")
        self.addControl("aiMatteColorA", label="Matte Opacity")
        self.endLayout()

        self.beginLayout("Advanced", collapse=True)
        self.addControl("caustics", label="Caustics", annotation="Enable Caustics")
        self.addControl("internalReflections", label="Internal Reflections", annotation="Enable Internal Reflections")
        self.addControl("exitToBackground", label="Exit To Background", annotation="When bounce depth is exceeded, use background color instead")
        self.addSeparator()
        self.addControl("indirectDiffuse", label="Indirect Diffuse", annotation="Indirect Diffuse")
        self.addControl("indirectSpecular", label="Indirect Specular", annotation="Indirect Specular")
        self.endLayout()
        

        self.beginLayout("Hardware Texturing", collapse=True)
        pm.mel.eval('AEhardwareTextureTemplate "%s"' % self.nodeName + r'("baseColor emissionColor ")')
        self.endLayout()
        
        # include/call base class/node attributes
        pm.mel.AEdependNodeTemplate(self.nodeName)

        self.suppress('PhongExponent')
       
        self.addExtraControls()
        self.endScrollLayout()

        # update dimming
        self.changeParams(self.nodeName)


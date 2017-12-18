import maya.mel
import maya.cmds as cmds
import mtoa.utils as utils
import mtoa.ui.ae.utils as aeUtils
from mtoa.ui.ae.shaderTemplate import ShaderAETemplate

def setFloatValue(ctrl, value):
    cmds.setAttr(cmds.attrFieldSliderGrp(ctrl, query=True, attribute=True), value)

def setValueSSS(ctrl, value):
    attr = cmds.attrColorSliderGrp(ctrl, query=True, attribute=True)
    cmds.setAttr(attr + 'R', value[0])
    cmds.setAttr(attr + 'G', value[1])
    cmds.setAttr(attr + 'B', value[2])

    attrRadius = attr.replace("subsurfaceColor", "subsurfaceRadius")
    cmds.setAttr(attrRadius + 'R', value[3])
    cmds.setAttr(attrRadius + 'G', value[4])
    cmds.setAttr(attrRadius + 'B', value[5])


class AEaiStandardSurfaceTemplate(ShaderAETemplate):
    convertToMayaStyle = True
   
    def changeParams(self, nodeName):
        metal_0 = float(cmds.getAttr(nodeName + '.metalness')) == 0.0
        metal_1 = float(cmds.getAttr(nodeName + '.metalness')) == 1.0

        metalConnections = cmds.listConnections(nodeName + '.metalness', d=False,s=True)
        if (metalConnections is not None) and len(metalConnections) > 0:
            metal_0 = metal_1 = False

        transmission_0 = float(cmds.getAttr(nodeName + '.transmission')) == 0.0
        transmission_1 = float(cmds.getAttr(nodeName + '.transmission')) == 1.0

        transmissionConnections = cmds.listConnections(nodeName + '.transmission', d=False,s=True)
        if (transmissionConnections is not None) and len(transmissionConnections) > 0:
            transmission_0 = transmission_1 = False

        subsurface_0 = float(cmds.getAttr(nodeName + '.subsurface')) == 0.0
        subsurface_1 = float(cmds.getAttr(nodeName + '.subsurface')) == 1.0

        sssConnections = cmds.listConnections(nodeName + '.subsurface', d=False,s=True)
        if (sssConnections is not None) and len(sssConnections) > 0:
            subsurface_0 = subsurface_1 = False

        thin_walled = bool(cmds.getAttr(nodeName + '.thin_walled'))
        sss_diffusion = True
        if cmds.getAttr(nodeName + '.subsurfaceType') != 0:
            sss_diffusion = False

        cmds.editorTemplate(dimControl=(nodeName, 'specularIOR', metal_1))
        cmds.editorTemplate(dimControl=(nodeName, 'diffuseRoughness', metal_1 or transmission_1))

        dim_base = metal_0 and transmission_1
        cmds.editorTemplate(dimControl=(nodeName, 'base', dim_base))
        cmds.editorTemplate(dimControl=(nodeName, 'baseColor', dim_base))

        dim_transmission = metal_1 or transmission_0
        dim_transmission_interior = dim_transmission or thin_walled
        cmds.editorTemplate(dimControl=(nodeName, 'transmission', metal_1))
        cmds.editorTemplate(dimControl=(nodeName, 'transmissionColor', dim_transmission))
        cmds.editorTemplate(dimControl=(nodeName, 'transmissionDepth', dim_transmission_interior))
        cmds.editorTemplate(dimControl=(nodeName, 'transmissionScatter', dim_transmission_interior))
        cmds.editorTemplate(dimControl=(nodeName, 'transmissionScatterAnisotropy', dim_transmission_interior))
        cmds.editorTemplate(dimControl=(nodeName, 'transmissionDispersion', dim_transmission_interior))
        cmds.editorTemplate(dimControl=(nodeName, 'transmissionExtraRoughness', dim_transmission_interior))
        cmds.editorTemplate(dimControl=(nodeName, 'transmitAovs', dim_transmission))

        dim_subsurface = metal_1 or transmission_1 or subsurface_0
        dim_subsurface_radius = dim_subsurface or thin_walled
        dim_subsurface_anisotropy = dim_subsurface or sss_diffusion
        cmds.editorTemplate(dimControl=(nodeName, 'subsurface', metal_1 or transmission_1))
        cmds.editorTemplate(dimControl=(nodeName, 'subsurfaceColor', dim_subsurface))
        cmds.editorTemplate(dimControl=(nodeName, 'subsurfaceRadius', dim_subsurface_radius))
        cmds.editorTemplate(dimControl=(nodeName, 'subsurfaceScale', dim_subsurface_radius))
        cmds.editorTemplate(dimControl=(nodeName, 'subsurfaceType', dim_subsurface))
        cmds.editorTemplate(dimControl=(nodeName, 'subsurfaceAnisotropy', dim_subsurface_anisotropy))



    def createIOR(self, attr):
        tokens = attr.split('.')
        nodeName = tokens[0]        

        controlName = 'Ctrl' + tokens[1]
        cmds.attrFieldSliderGrp(controlName, attribute=attr, label='IOR <span>&#8801;</span>')
        
        attrChildren = cmds.layout(controlName, query=True, childArray=True)
        cmds.popupMenu(button=1, parent=attrChildren[0])

        presets={ 'Cornea': 1.37, 'Diamond': 2.42, 'Ethanol': 1.36, 'Flint glass': 1.6, 'Glass': 1.5, 'Ice' : 1.31, 'Olive Oil': 1.47, 'Plastic': 1.55, 'Saphire': 1.77, 'Skin': 1.4, 'Water': 1.33}
        for k in sorted(presets):
            cmds.menuItem(label=k, command= lambda arg=None,x=presets[k]:  setFloatValue(controlName, x))


    def updateIOR(self, attr):
        tokens = attr.split('.')
        nodeName = tokens[0]       
        controlName = 'Ctrl' + tokens[1]
        cmds.attrFieldSliderGrp(controlName, edit=True, attribute=attr)

    def createAbbe(self, attr):
        tokens = attr.split('.')
        nodeName = tokens[0]        

        controlName = 'Ctrl' + tokens[1]
        cmds.attrFieldSliderGrp(controlName, attribute=attr, label='Dispersion Abbe <span>&#8801;</span>')
        
        attrChildren = cmds.layout(controlName, query=True, childArray=True)
        cmds.popupMenu(button=1, parent=attrChildren[0])

        presets={'Diamond': 55, 'Sapphire': 72}
        for k in sorted(presets):
            cmds.menuItem(label=k, command= lambda arg=None, x=presets[k]: setFloatValue(controlName, x))


    def updateAbbe(self, attr):
        tokens = attr.split('.')
        nodeName = tokens[0]       
        controlName = 'Ctrl' + tokens[1]
        cmds.attrFieldSliderGrp(controlName, edit=True, attribute=attr)


    def createSSS(self, attr):
        tokens = attr.split('.')
        nodeName = tokens[0]        

        controlName = 'Ctrl' + tokens[1]
        cmds.attrColorSliderGrp(controlName, attribute=attr, label='SubSurface Color <span>&#8801;</span>')
        
        attrChildren = cmds.layout(controlName, query=True, childArray=True)
        cmds.popupMenu(button=1, parent=attrChildren[0])

        presets={'Apple': (0.43,0.21,0.17,11.61,3.88,1.75), 'Chicken':(0.44,0.22,0.14,9.44,3.35, 1.79) , 'Cream':(0.99,0.94,0.83,15.03,4.66,2.54), 'Ketchup': (0.22,0.01,0.00,4.76,0.57,0.39), 'Marble': (0.93,0.91,0.88,8.51,5.57,3.95), 'Potato':(0.86,0.74,0.29,14.27,7.23,2.04) , 'Skim Milk': (0.89,0.89,0.80,18.42,10.44,3.50), 'Skin 1':(0.57,0.31,0.17,3.67,1.37,0.68) , 'Skin 2':(0.75,0.57,0.47,4.82,1.69,1.09) , 'Whole Milk':(0.95,0.93,0.85,10.90,6.58,2.51) }
        for k in sorted(presets):
            cmds.menuItem(label=k, command= lambda arg=None, x=presets[k]: setValueSSS(controlName, x))

    def updateSSS(self, attr):
        tokens = attr.split('.')
        nodeName = tokens[0]       
        controlName = 'Ctrl' + tokens[1]
        cmds.attrColorSliderGrp(controlName, edit=True, attribute=attr)

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
        self.addCustom("specularIOR", self.createIOR, self.updateIOR)
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
        self.addCustom("transmissionDispersion", self.createAbbe, self.updateAbbe)
        self.addControl("transmissionExtraRoughness", label="Extra Roughness", annotation="Transmission Extra Roughness")
        self.addSeparator()
        self.addControl("transmitAovs", label="Transmit AOVs")
        self.endLayout()

        self.beginLayout("Subsurface", collapse=True)
        self.addControl("subsurface",  label="Weight", annotation="Subsurface Scattering Mix", changeCommand=self.changeParams)

        self.addCustom("subsurfaceColor", self.createSSS, self.updateSSS)
        #self.addControl("subsurfaceColor", label="Color", annotation="Subsurface Scattering Color")
        self.addControl("subsurfaceRadius", label="Radius", annotation="Subsurface Scattering Radius")
        self.addControl("subsurfaceScale", label="Scale", annotation="Subsurface Scattering Scale")
        self.addControl("subsurfaceType", label="Type", annotation="Subsurface Type",  changeCommand=self.changeParams)
        self.addControl("subsurfaceAnisotropy", label="Anisotropy", annotation="Subsurface Anisotropy")
        self.endLayout() 

        self.beginLayout("Coat", collapse=True)
        self.addControl("coat",  label="Weight", annotation="Coat Weight")
        self.addControl("coatColor", label="Color", annotation="Coat Color")
        self.addControl("coatRoughness", label="Roughness", annotation="Coat Roughness")
        self.addSeparator()
        self.addCustom("coatIOR", self.createIOR, self.updateIOR)
        self.addSeparator()
        self.addControl("coatNormal", label="Normal", annotation="Coat Normal")
        self.endLayout()

        self.beginLayout("Emission", collapse=True)
        self.addControl("emission",  label="Weight", annotation="Emission Weight")
        self.addControl("emissionColor", label="Color", annotation="Emission Color")
        self.endLayout() 

        self.beginLayout("Thin Film", collapse=True)
        self.addControl("thin_film_thickness",  label="Thickness", annotation="Thin Film Thickness")
        self.addControl("thin_film_IOR", label="IOR", annotation="Thin Film IOR")
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
        maya.mel.eval('AEhardwareTextureTemplate "%s"' % self.nodeName + r'("base_color emission_color ")')
        self.endLayout()
        
        # include/call base class/node attributes
        maya.mel.eval('AEdependNodeTemplate '+self.nodeName)

        self.suppress('PhongExponent')
       
        self.addExtraControls()
        self.endScrollLayout()

        # update dimming
        self.changeParams(self.nodeName)


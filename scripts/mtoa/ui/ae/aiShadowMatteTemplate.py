import maya.mel
import maya.cmds as cmds
from mtoa.ui.ae.shaderTemplate import ShaderAETemplate


class AEaiShadowMatteTemplate(ShaderAETemplate):

    def updateBackground(self, nodeName):
            
        backgroundAttr = '%s.%s' % (nodeName, 'background')
        value = cmds.getAttr(backgroundAttr)
        cmds.editorTemplate(dimControl=(nodeName, 'backgroundColor', not value))
    
    def updateSpecular(self, nodeName):            
        specularAttr = '%s.%s' % (nodeName, 'indirectSpecularEnable')
        value = cmds.getAttr(specularAttr)
        cmds.editorTemplate(dimControl=(nodeName, 'specularColor', not value))
        cmds.editorTemplate(dimControl=(nodeName, 'specularIntensity', not value))
        cmds.editorTemplate(dimControl=(nodeName, 'specularRoughness', not value))
        cmds.editorTemplate(dimControl=(nodeName, 'specularIOR', not value))
                

    def setup(self):
        self.addSwatch()
        self.beginScrollLayout()

        self.addCustom('message', 'AEshaderTypeNew', 'AEshaderTypeReplace')

        self.beginLayout("Background", collapse=False)
        self.addControl("background", changeCommand=self.updateBackground, label="Background")
        self.addControl("backgroundColor", label="Background Color")
        
        self.beginLayout("Shadows", collapse=False)
        self.addControl("shadowColor", label="Shadow Color")
        self.addControl("shadowOpacity", label="Shadow Opacity")
        self.addControl("backlighting", label="Backlighting")
        self.addControl("alphaMask", label="Alpha Mask")
        
        self.endLayout()

        self.beginLayout("Diffuse", collapse=True)
        self.addControl("diffuseColor", label="Color")
        self.addControl("diffuseIntensity", label="Intensity")
        self.addControl("diffuseUseBackground", label="Use Background")
        self.addControl("indirectDiffuseEnable", label="Indirect Diffuse")
        self.endLayout()

        self.beginLayout("Specular", collapse=True)
        self.addControl("indirectSpecularEnable", label="Indirect Specular", changeCommand=self.updateSpecular)
        self.addControl("specularColor", label="Color")
        self.addControl("specularIntensity", label="Intensity")
        self.addControl("specularRoughness", label="Roughness")
        self.addControl("specularIOR", label="IOR")        
        self.endLayout()
        self.beginLayout("Lights", collapse=True)
        self.addControl("aovGroup", label="Light Group")
        self.endLayout()
        self.beginLayout("AOVs", collapse=True)
        self.addControl("aovShadow", label="Shadow")
        self.addControl("aovShadowDiff", label="Shadow Diff")
        self.addControl("aovShadowMask", label="Shadow Mask")
        self.addControl("aovIndirectDiffuse", label="Indirect Diffuse")
        self.addControl("aovIndirectSpecular", label="Indirect Specular")

        self.endLayout()

        maya.mel.eval('AEdependNodeTemplate '+self.nodeName)

        self.addExtraControls()
        self.endScrollLayout()


import pymel.core as pm
import maya.cmds as cmds
from mtoa.ui.ae.shaderTemplate import ShaderAETemplate

def firstShaderAttrReplace(plugName):
    nodeAndAttrs = plugName.split(".")
    node = nodeAndAttrs[0]
    cmds.attrNavigationControlGrp("aiMixShaderFirstCtrl", edit=True, attribute=("%s.shader1" % node))

def firstShaderAttrNew(plugName):
    pm.setUITemplate('attributeEditorTemplate', pst=True)
    cmds.attrNavigationControlGrp("aiMixShaderFirstCtrl", label="Shader 1", cn="createRenderNode -allWithShadersUp \"defaultNavigation -force true -connectToExisting -source %node -destination "+plugName+"\" \"\"")
    pm.setUITemplate(ppt=True)
    firstShaderAttrReplace(plugName)


def secondShaderAttrReplace(plugName):
    nodeAndAttrs = plugName.split(".")
    node = nodeAndAttrs[0]
    cmds.attrNavigationControlGrp("aiMixShaderSecondCtrl", edit=True, attribute=("%s.shader2" % node))

def secondShaderAttrNew(plugName):
    pm.setUITemplate('attributeEditorTemplate', pst=True)
    cmds.attrNavigationControlGrp("aiMixShaderSecondCtrl", label="Shader 2", cn="createRenderNode -allWithShadersUp \"defaultNavigation -force true -connectToExisting -source %node -destination "+plugName+"\" \"\"")
    pm.setUITemplate(ppt=True)
    secondShaderAttrReplace(plugName)



class AEaiMixShaderTemplate(ShaderAETemplate):
    def setup(self):
        self.addSwatch()
        self.beginScrollLayout()
        
        self.addCustom('message', 'AEshaderTypeNew', 'AEshaderTypeReplace')

        self.addControl('mode', label='Mix Mode', annotation="Mix Mode")
        self.addControl('mix', label='Mix Weight', annotation="Mix Weight")

        self.addCustom("shader1", firstShaderAttrNew, firstShaderAttrReplace)
        self.addCustom("shader2", secondShaderAttrNew, secondShaderAttrReplace)

        # include/call base class/node attributes
        pm.mel.AEdependNodeTemplate(self.nodeName)
        
        self.addExtraControls()
        self.endScrollLayout()



import maya.mel
from mtoa.ui.ae.shaderTemplate import ShaderAETemplate
import maya.cmds as cmds

def aiMixInputReplace(plugName):
    nodeAndAttrs = plugName.split(".")
    node = nodeAndAttrs[0]
    ctrlName = "aiMix"
    ctrlName += nodeAndAttrs[1]

    cmds.attrNavigationControlGrp(ctrlName, edit=True, attribute=(plugName),  cn="createRenderNode -allWithShadersUp \"defaultNavigation -force true -connectToExisting -source %node -destination "+plugName+"\" \"\"")

def aiMixInputNew(plugName):
    cmds.setUITemplate('attributeEditorTemplate', pst=True)

    nodeAndAttrs = plugName.split(".")
    ctrlName = "aiMix"
    ctrlName += nodeAndAttrs[1]

    cmds.attrNavigationControlGrp(ctrlName, label=nodeAndAttrs[1], cn="createRenderNode -allWithShadersUp \"defaultNavigation -force true -connectToExisting -source %node -destination "+plugName+"\" \"\"")
    cmds.setUITemplate(ppt=True)
    aiMixInputReplace(plugName)


class AEaiMixShaderTemplate(ShaderAETemplate):
    def setup(self):
        self.addSwatch()
        self.beginScrollLayout()
        
        self.addCustom('message', 'AEshaderTypeNew', 'AEshaderTypeReplace')
       

        self.addControl('mode', label='Mix Mode', annotation="Mix Mode")
        self.addControl('mix', label='Mix Weight', annotation="Mix Weight")

        self.addCustom("shader1", aiMixInputNew, aiMixInputReplace)
        self.addCustom("shader2", aiMixInputNew, aiMixInputReplace)
        
        # include/call base class/node attributes
        maya.mel.eval('AEdependNodeTemplate '+self.nodeName)
        
        self.addExtraControls()
        self.endScrollLayout()


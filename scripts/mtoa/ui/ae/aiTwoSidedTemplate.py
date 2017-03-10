import pymel.core as pm
import maya.cmds as cmds
from mtoa.ui.ae.shaderTemplate import ShaderAETemplate

def TwoSidedAttrReplace(plugName):
    nodeAndAttrs = plugName.split(".")
    node = nodeAndAttrs[0]
    ctrlName = "aiTwoSided"
    ctrlName += nodeAndAttrs[1]

    cmds.attrNavigationControlGrp(ctrlName, edit=True, attribute=(plugName),  cn="createRenderNode -allWithShadersUp \"defaultNavigation -force true -connectToExisting -source %node -destination "+plugName+"\" \"\"")

def TwoSidedAttrNew(plugName):
    pm.setUITemplate('attributeEditorTemplate', pst=True)

    nodeAndAttrs = plugName.split(".")
    ctrlName = "aiTwoSided"
    ctrlName += nodeAndAttrs[1]

    cmds.attrNavigationControlGrp(ctrlName, label=nodeAndAttrs[1], cn="createRenderNode -allWithShadersUp \"defaultNavigation -force true -connectToExisting -source %node -destination "+plugName+"\" \"\"")
    pm.setUITemplate(ppt=True)
    TwoSidedAttrReplace(plugName)


class AEaiTwoSidedTemplate(ShaderAETemplate):
    def setup(self):
        self.addSwatch()
        self.beginScrollLayout()

        self.addCustom('message', 'AEshaderTypeNew', 'AEshaderTypeReplace')

        self.beginLayout("Two-Sided", collapse=False)
        self.addCustom("front", TwoSidedAttrNew, TwoSidedAttrReplace)
        self.addCustom("back", TwoSidedAttrNew, TwoSidedAttrReplace)
        self.endLayout()
        
        pm.mel.AEdependNodeTemplate(self.nodeName)

        self.addExtraControls()
        self.endScrollLayout()


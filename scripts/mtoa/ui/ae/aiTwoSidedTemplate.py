import pymel.core as pm
import maya.cmds as cmds
from mtoa.ui.ae.shaderTemplate import ShaderAETemplate

def frontAttrReplace(plugName):
    nodeAndAttrs = plugName.split(".")
    node = nodeAndAttrs[0]
    cmds.attrNavigationControlGrp("aiTwoSidedFrontCtrl", edit=True, attribute=("%s.front" % node))

def frontAttrNew(plugName):
    pm.setUITemplate('attributeEditorTemplate', pst=True)
    cmds.attrNavigationControlGrp("aiTwoSidedFrontCtrl", label="Front Shader", cn="createRenderNode -allWithShadersUp \"defaultNavigation -force true -connectToExisting -source %node -destination "+plugName+"\" \"\"")
    pm.setUITemplate(ppt=True)

    frontAttrReplace(plugName)


def backAttrReplace(plugName):
    nodeAndAttrs = plugName.split(".")
    node = nodeAndAttrs[0]
    cmds.attrNavigationControlGrp("aiTwoSidedBackCtrl", edit=True, attribute=("%s.back" % node))

def backAttrNew(plugName):
    pm.setUITemplate('attributeEditorTemplate', pst=True)

    cmds.attrNavigationControlGrp("aiTwoSidedBackCtrl", label="Back Shader", cn="createRenderNode -allWithShadersUp \"defaultNavigation -force true -connectToExisting -source %node -destination "+plugName+"\" \"\"")
    pm.setUITemplate(ppt=True)
    backAttrReplace(plugName)


class AEaiTwoSidedTemplate(ShaderAETemplate):
    def setup(self):
        self.addSwatch()
        self.beginScrollLayout()

        self.beginLayout("Two-Sided", collapse=False)
        self.addCustom("front", frontAttrNew, frontAttrReplace)
        self.addCustom("back", backAttrNew, backAttrReplace)
        self.endLayout()
        
        pm.mel.AEdependNodeTemplate(self.nodeName)

        self.addExtraControls()
        self.endScrollLayout()


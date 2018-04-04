import maya.mel
from mtoa.ui.ae.shaderTemplate import ShaderAETemplate
import maya.cmds as cmds


def PassthroughReplace(plugName):
    nodeAndAttrs = plugName.split(".")
    node = nodeAndAttrs[0]
    ctrlName = "aiMatte"
    ctrlName += nodeAndAttrs[1]
    cmds.attrNavigationControlGrp(ctrlName, edit=True, attribute=(plugName),  cn="createRenderNode -allWithShadersUp \"defaultNavigation -force true -connectToExisting -source %node -destination "+plugName+"\" \"\"")

def PassthroughNew(plugName):
    cmds.setUITemplate('attributeEditorTemplate', pst=True)

    nodeAndAttrs = plugName.split(".")
    ctrlName = "aiMatte"
    ctrlName += nodeAndAttrs[1]

    cmds.attrNavigationControlGrp(ctrlName, label='Passthrough', cn="createRenderNode -allWithShadersUp \"defaultNavigation -force true -connectToExisting -source %node -destination "+plugName+"\" \"\"")
    cmds.setUITemplate(ppt=True)
    PassthroughReplace(plugName)
class AEaiMatteTemplate(ShaderAETemplate):



    def setup(self):
        self.beginScrollLayout()
        
        self.addCustom("passthrough", PassthroughNew, PassthroughReplace)
        self.addSeparator()
        self.addControl("color")
        self.addControl("opacity")

        self.endLayout()

        maya.mel.eval('AEdependNodeTemplate '+self.nodeName)
        self.addExtraControls()

        self.endScrollLayout()


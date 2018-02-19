import maya.mel
import maya.cmds as cmds

from mtoa.ui.ae.shaderTemplate import ShaderAETemplate
def PassthroughAttrReplace(plugName):
    nodeAndAttrs = plugName.split(".")
    node = nodeAndAttrs[0]
    ctrlName = "aiTraceSet_passthrough"
    
    cmds.attrNavigationControlGrp(ctrlName, edit=True, attribute=(plugName),  cn="createRenderNode -allWithShadersUp \"defaultNavigation -force true -connectToExisting -source %node -destination "+plugName+"\" \"\"")

def PassthroughAttrNew(plugName):
    cmds.setUITemplate('attributeEditorTemplate', pst=True)
    nodeAndAttrs = plugName.split(".")
    ctrlName = "aiTraceSet_passthrough"
    
    cmds.attrNavigationControlGrp(ctrlName, label='Passthrough', cn="createRenderNode -allWithShadersUp \"defaultNavigation -force true -connectToExisting -source %node -destination "+plugName+"\" \"\"")
    cmds.setUITemplate(ppt=True)
    PassthroughAttrReplace(plugName)

class AEaiTraceSetTemplate(ShaderAETemplate):
    def setup(self):
        self.addSwatch()

        self.beginScrollLayout()

        self.addCustom("passthrough", PassthroughAttrNew, PassthroughAttrReplace)
        
        self.addControl('trace_set', label = 'Trace Set')
        self.addControl('inclusive', label = 'Inclusive')
        maya.mel.eval('AEdependNodeTemplate '+self.nodeName)
        self.addExtraControls()
        self.endScrollLayout()

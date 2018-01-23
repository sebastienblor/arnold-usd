import maya.mel
import maya.cmds as cmds

from mtoa.ui.ae.shaderTemplate import ShaderAETemplate
def PassthroughAttrReplace(plugName):
    nodeAndAttrs = plugName.split(".")
    node = nodeAndAttrs[0]
    ctrlName = "aiPassthrough"
    ctrlName += nodeAndAttrs[1]

    cmds.attrNavigationControlGrp(ctrlName, edit=True, attribute=(plugName),  cn="createRenderNode -allWithShadersUp \"defaultNavigation -force true -connectToExisting -source %node -destination "+plugName+"\" \"\"")

def PassthroughAttrNew(plugName):
    cmds.setUITemplate('attributeEditorTemplate', pst=True)
    nodeAndAttrs = plugName.split(".")
    ctrlName = "aiPassthrough"
    ctrlName += nodeAndAttrs[1]

    cmds.attrNavigationControlGrp(ctrlName, label=nodeAndAttrs[1], cn="createRenderNode -allWithShadersUp \"defaultNavigation -force true -connectToExisting -source %node -destination "+plugName+"\" \"\"")
    cmds.setUITemplate(ppt=True)
    PassthroughAttrReplace(plugName)

class AEaiPassthroughTemplate(ShaderAETemplate):
    def setup(self):
        self.addSwatch()

        self.beginScrollLayout()

        self.addCustom('message', 'AEshaderTypeNew', 'AEshaderTypeReplace')
        self.addCustom("passthrough", PassthroughAttrNew, PassthroughAttrReplace)
        
        self.beginLayout("Inputs", collapse=True)
        self.addCustom("eval1", PassthroughAttrNew, PassthroughAttrReplace)
        self.addCustom("eval2", PassthroughAttrNew, PassthroughAttrReplace)
        self.addCustom("eval3", PassthroughAttrNew, PassthroughAttrReplace)
        self.addCustom("eval4", PassthroughAttrNew, PassthroughAttrReplace)
        self.addCustom("eval5", PassthroughAttrNew, PassthroughAttrReplace)
        self.addCustom("eval6", PassthroughAttrNew, PassthroughAttrReplace)
        self.addCustom("eval7", PassthroughAttrNew, PassthroughAttrReplace)
        self.addCustom("eval8", PassthroughAttrNew, PassthroughAttrReplace)
        self.addCustom("eval9", PassthroughAttrNew, PassthroughAttrReplace)
        self.addCustom("eval10", PassthroughAttrNew, PassthroughAttrReplace)
        self.addCustom("eval11", PassthroughAttrNew, PassthroughAttrReplace)
        self.addCustom("eval12", PassthroughAttrNew, PassthroughAttrReplace)
        self.addCustom("eval13", PassthroughAttrNew, PassthroughAttrReplace)
        self.addCustom("eval14", PassthroughAttrNew, PassthroughAttrReplace)
        self.addCustom("eval15", PassthroughAttrNew, PassthroughAttrReplace)
        self.addCustom("eval16", PassthroughAttrNew, PassthroughAttrReplace)
        self.addCustom("eval17", PassthroughAttrNew, PassthroughAttrReplace)
        self.addCustom("eval18", PassthroughAttrNew, PassthroughAttrReplace)
        self.addCustom("eval19", PassthroughAttrNew, PassthroughAttrReplace)
        self.addCustom("eval20", PassthroughAttrNew, PassthroughAttrReplace)        
        self.endLayout()

        #self.beginLayout("Bump", collapse=False)
        #self.addControl("normalCamera")
        #self.endLayout()
        
        maya.mel.eval('AEdependNodeTemplate '+self.nodeName)
        self.addExtraControls()
        self.endScrollLayout()

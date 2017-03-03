import pymel.core as pm
from mtoa.ui.ae.shaderTemplate import ShaderAETemplate
import maya.cmds as cmds

def SwitchInputReplace(plugName):
    nodeAndAttrs = plugName.split(".")
    node = nodeAndAttrs[0]
    ctrlName = "aiSwitch"
    ctrlName += nodeAndAttrs[1]

    cmds.attrNavigationControlGrp(ctrlName, edit=True, attribute=(plugName),  cn="createRenderNode -allWithShadersUp \"defaultNavigation -force true -connectToExisting -source %node -destination "+plugName+"\" \"\"")

def SwitchInputNew(plugName):
    pm.setUITemplate('attributeEditorTemplate', pst=True)

    nodeAndAttrs = plugName.split(".")
    ctrlName = "aiSwitch"
    ctrlName += nodeAndAttrs[1]

    cmds.attrNavigationControlGrp(ctrlName, label=nodeAndAttrs[1], cn="createRenderNode -allWithShadersUp \"defaultNavigation -force true -connectToExisting -source %node -destination "+plugName+"\" \"\"")
    pm.setUITemplate(ppt=True)
    SwitchInputReplace(plugName)


class AEaiSwitchTemplate(ShaderAETemplate):
    def setup(self):
        self.addSwatch()

        self.beginScrollLayout()

        self.addCustom('message', 'AEshaderTypeNew', 'AEshaderTypeReplace')

        self.addControl('index', label='Index')

        self.beginLayout("Inputs", collapse=False)

        self.addCustom("input0", SwitchInputNew, SwitchInputReplace)
        self.addCustom("input1", SwitchInputNew, SwitchInputReplace)
        self.addCustom("input2", SwitchInputNew, SwitchInputReplace)
        self.addCustom("input3", SwitchInputNew, SwitchInputReplace)
        self.addCustom("input4", SwitchInputNew, SwitchInputReplace)
        self.addCustom("input5", SwitchInputNew, SwitchInputReplace)
        self.addCustom("input6", SwitchInputNew, SwitchInputReplace)
        self.addCustom("input7", SwitchInputNew, SwitchInputReplace)
        self.addCustom("input8", SwitchInputNew, SwitchInputReplace)
        self.addCustom("input9", SwitchInputNew, SwitchInputReplace)
        self.addCustom("input10", SwitchInputNew, SwitchInputReplace)
        self.addCustom("input11", SwitchInputNew, SwitchInputReplace)
        self.addCustom("input12", SwitchInputNew, SwitchInputReplace)
        self.addCustom("input13", SwitchInputNew, SwitchInputReplace)
        self.addCustom("input14", SwitchInputNew, SwitchInputReplace)
        self.addCustom("input15", SwitchInputNew, SwitchInputReplace)
        self.addCustom("input16", SwitchInputNew, SwitchInputReplace)
        self.addCustom("input17", SwitchInputNew, SwitchInputReplace)
        self.addCustom("input18", SwitchInputNew, SwitchInputReplace)
        self.addCustom("input19", SwitchInputNew, SwitchInputReplace)
        
        self.endLayout()

        pm.mel.AEdependNodeTemplate(self.nodeName)
        self.addExtraControls()
        self.endScrollLayout()

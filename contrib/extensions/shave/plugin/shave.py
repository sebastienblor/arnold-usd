import maya.cmds as cmds
from mtoa.ui.ae.customShapeAttributes import commonShapeAttributes, renderStatsAttributes
from mtoa.ui.ae.shapeTemplate import registerUI

def hairShaderNew(attrName):
    cmds.setUITemplate('attributeEditorTemplate', pst=True)
    cmds.attrNavigationControlGrp('aiHairShader', label="Hair Shader", at=attrName)
    cmds.setUITemplate(ppt=True)

def hairShaderReplace(attrName):
    cmds.attrNavigationControlGrp('aiHairShader', edit=True, at=attrName)


@registerUI("shaveHair")
def builtin_hairSystem(ui):
    renderStatsAttributes(ui)
    commonShapeAttributes(ui)
    ui.addSeparator()

    ui.addAttribute("aiOverrideHair")
    ui.addAttribute("aiHairShader")

    ui.addSeparator()

    ui.addAttribute("aiMinPixelWidth")
    ui.addAttribute("aiMode")
    cmds.editorTemplate("aiMinPixelWidth", label="Min Pixel Width", addDynamicControl=True)
    cmds.editorTemplate("aiMode", label="Mode", addDynamicControl=True)

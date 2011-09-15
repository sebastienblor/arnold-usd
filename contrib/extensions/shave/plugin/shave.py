import maya.cmds as cmds
from mtoa.ui.ae.customShapeAttributes import commonShapeAttributes, renderStatsAttributes
from mtoa.ui.ae.shapeTemplate import translatorUI

@translatorUI("shaveHair")
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

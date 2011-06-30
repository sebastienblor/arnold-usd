import maya.cmds as cmds
from mtoa.ui.ae.customShapeAttributes import commonShapeAttributes
from mtoa.ui.ae.shapeTemplate import registerUI

@registerUI("shaveHair")
def builtin_hairSystem(nodeName):
    commonShapeAttributes(nodeName)
    cmds.editorTemplate(addSeparator=True)

    cmds.editorTemplate("aiExportHairColors", label="Export Hair Colors", addDynamicControl=True)
    cmds.editorTemplate("aiExportHairIDs", label="Export Hair IDs", addDynamicControl=True)
    cmds.editorTemplate("aiOverrideHair", label="Override Hair", addDynamicControl=True)
    cmds.editorTemplate("aiHairShader", label="Hair Shader", addDynamicControl=True)

    cmds.editorTemplate(addSeparator=True)

    cmds.editorTemplate("aiMinPixelWidth", label="Min Pixel Width", addDynamicControl=True)
    cmds.editorTemplate("aiMode", label="Mode", addDynamicControl=True)

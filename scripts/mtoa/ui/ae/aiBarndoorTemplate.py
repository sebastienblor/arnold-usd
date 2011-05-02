import maya.cmds as cmds
import maya.mel as mel

def aiBarndoorTemplate(nodeName):

    aiSwatchDisplay(nodeName)
    
    cmds.editorTemplate(beginScrollLayout=True)
    cmds.editorTemplate(beginLayout="Barndoor Attributes", collapse=False)

    cmds.editorTemplate("barndoor_top_left", addControl=True, label="Top Left")
    cmds.editorTemplate("barndoor_top_right", addControl=True, label="Top Right")
    cmds.editorTemplate("barndoor_top_edge", addControl=True, label="Top Edge")

    cmds.editorTemplate(addSeparator=True)

    cmds.editorTemplate("barndoor_bottom_left", addControl=True, label="Bottom Left")
    cmds.editorTemplate("barndoor_bottom_right", addControl=True, label="Bottom Right")
    cmds.editorTemplate("barndoor_bottom_edge", addControl=True, label="Bottom Edge")

    cmds.editorTemplate(addSeparator=True)

    cmds.editorTemplate("barndoor_left_top", addControl=True, label="Left Top")
    cmds.editorTemplate("barndoor_left_bottom", addControl=True, label="Left Bottom")
    cmds.editorTemplate("barndoor_left_edge", addControl=True, label="Left Edge")

    cmds.editorTemplate(addSeparator=True)
    cmds.editorTemplate("barndoor_right_top", addControl=True, label="Right Top")
    cmds.editorTemplate("barndoor_right_bottom", addControl=True, label="Right Bottom")
    cmds.editorTemplate("barndoor_right_edge", addControl=True, label="Right Edge")

    cmds.editorTemplate(endLayout=True)

    # include/call base class/node attributes
    mel.eval('AEdependNodeTemplate "%s"'%nodeName)

    cmds.editorTemplate(addExtraControls=True)

    cmds.editorTemplate(endScrollLayout=True)

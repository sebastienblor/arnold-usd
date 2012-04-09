import maya.cmds as cmds
import maya.mel as mel
from mtoa.ui.ae.aiSwatchDisplay import aiSwatchDisplay

def aiDisplacementTemplate(nodeName):

    aiSwatchDisplay(nodeName)

    cmds.editorTemplate(beginScrollLayout=True)

    cmds.editorTemplate(beginLayout="Displacement Attributes", collapse=False)

    #cmds.editorTemplate("disp_height", addControl=True, label="Height")
    #cmds.editorTemplate("disp_padding", addControl=True, label="Padding")
    #cmds.editorTemplate("disp_zero_value", addControl=True, label="Zero Value")
    cmds.editorTemplate("disp_map", addControl=True, label="Map")
    #cmds.editorTemplate("disp_autobump", addControl=True, label="Auto Bump")
    cmds.editorTemplate("vector_displacement", addControl=True, label="Vector Displacement")
    cmds.editorTemplate("vector_displacement_scale", addControl=True, label="Vector Displacement Scale")

    cmds.editorTemplate(endLayout=True)

    # include/call base class/node attributes
    mel.eval('AEdependNodeTemplate "%s"'%nodeName)
    cmds.editorTemplate(addExtraControls=True)
    cmds.editorTemplate(endScrollLayout=True)


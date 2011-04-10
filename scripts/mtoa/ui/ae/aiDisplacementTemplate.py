import maya.cmds as cmds
import maya.mel as mel

def aiDisplacementTemplate(nodeName):

    mel.eval('AEswatchDisplay "%s"'%nodeName)

    cmds.editorTemplate(beginScrollLayout=True)

    cmds.editorTemplate(beginLayout="Displacement Attributes", collapse=False)

    cmds.editorTemplate("disp_height", addControl=True, label="Height")
    cmds.editorTemplate("disp_zero_value", addControl=True, label="Zero Value")
    cmds.editorTemplate("disp_map", addControl=True, label="Map")
    cmds.editorTemplate("autobump", addControl=True, label="Auto Bump")
    cmds.editorTemplate("vector_displacement", addControl=True, label="Vector Displacement")
    cmds.editorTemplate("vector_displacement_scale", addControl=True, label="Vector Displacement Scale")

    cmds.editorTemplate(endLayout=True)

    # include/call base class/node attributes
    mel.eval('AEdependNodeTemplate "%s"'%nodeName)
    cmds.editorTemplate(addExtraControls=True)
    cmds.editorTemplate(endScrollLayout=True)


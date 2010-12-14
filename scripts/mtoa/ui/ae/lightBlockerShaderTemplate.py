import maya.cmds as cmds
import maya.mel as mel

def lightBlockerShaderTemplate(nodeName):

    cmds.editorTemplate(beginScrollLayout=True)

    cmds.editorTemplate(beginLayout="Light Blocker Attributes", collapse=False)

    cmds.editorTemplate("geometry_type", addControl=True, label="Geometry Type")
    cmds.editorTemplate("density", addControl=True, label="Density")

    cmds.editorTemplate(addSeparator=True)

    cmds.editorTemplate("ramp_axis", addControl=True, label="Ramp Axis")
    cmds.editorTemplate("ramp", addControl=True, label="Ramp")
    cmds.editorTemplate("height_edge", addControl=True, label="Height Edge")
    cmds.editorTemplate("width_edge", addControl=True, label="Width Edge")
    cmds.editorTemplate("roundness", addControl=True, label="Roundness")

    editorTemplate(endLayout=True)

    cmds.editorTemplate(beginLayout="Transform Attributes", collapse=False)

    cmds.editorTemplate("geometry_matrix", addControl=True, label="Geometry Matrix")

    editorTemplate(endLayout=True)

    # include/call base class/node attributes
    mel.eval('AEdependNodeTemplate "%s"'%nodeName)
    cmds.editorTemplate(addExtraControls=True)
    cmds.editorTemplate(endScrollLayout=True)

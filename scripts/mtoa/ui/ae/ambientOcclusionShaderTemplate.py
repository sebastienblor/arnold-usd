import maya.cmds as cmds
import maya.mel as mel

def ambientOcclusionShaderTemplate(nodeName):

    mel.eval('AEswatchDisplay "%s"'%nodeName)

    cmds.editorTemplate(beginScrollLayout=True)
    cmds.editorTemplate('AEshaderTypeNew', 'AEshaderTypeReplace', 'message', callCustom=True)

    cmds.editorTemplate(beginLayout="Ambient Occlusion Attributes", collapse=False)

    cmds.editorTemplate("samples", addControl=True, label="Samples")
    cmds.editorTemplate("white", addControl=True, label="Bright")
    cmds.editorTemplate("black",  addControl=True, label="Dark")
    cmds.editorTemplate("spread", addControl=True, label="Spread")
    cmds.editorTemplate("falloff",  addControl=True, label="Falloff")
    cmds.editorTemplate("near_clip",  addControl=True, label="Near Clip")
    cmds.editorTemplate("far_clip", addControl=True, label="Far Clip")
    cmds.editorTemplate("opacity", addControl=True, label="Opacity")
    cmds.editorTemplate("invert_normals", addControl=True, label="Invert Normals")

    cmds.editorTemplate(endLayout=True)

    # include/call base class/node attributes
    mel.eval('AEdependNodeTemplate("%s")'%nodeName)
    cmds.editorTemplate(addExtraControls=True)

    cmds.editorTemplate(endScrollLayout=True)

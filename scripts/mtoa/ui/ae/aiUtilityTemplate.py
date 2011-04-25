import maya.cmds as cmds
import maya.mel as mel

def aiUtilityTemplate(nodeName):

    mel.eval('AEswatchDisplay "%s"'%nodeName)

    cmds.editorTemplate(beginScrollLayout=True)

    cmds.editorTemplate('AEshaderTypeNew', 'AEshaderTypeReplace', "message", callCustom=True)

    cmds.editorTemplate(beginLayout="Utility Attributes", collapse=False)
    cmds.editorTemplate("shade_mode", label="Shade Mode", addControl=True)
    cmds.editorTemplate("color_mode", label="Color Mode", addControl=True)
    cmds.editorTemplate("color", label="Color", addControl=True)
    cmds.editorTemplate("opacity", label="Opacity", addControl=True)
    cmds.editorTemplate(endLayout=True)

    # include/call base class/node attributes
    mel.eval('AEdependNodeTemplate "%s"'%nodeName)
    cmds.editorTemplate(addExtraControls=True)

    cmds.editorTemplate(endScrollLayout=True)

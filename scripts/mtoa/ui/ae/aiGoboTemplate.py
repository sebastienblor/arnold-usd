import maya.cmds as cmds
import maya.mel as mel

def aiGoboTemplate(nodeName):

    mel.eval('AEswatchDisplay "%s"'%nodeName)

    cmds.editorTemplate(beginScrollLayout=True)

    cmds.editorTemplate(beginLayout="Gobo Attributes", collapse=False)

    cmds.editorTemplate("filter_mode", addControl=True, label="Filter Mode")
    cmds.editorTemplate("slidemap", addControl=True, label="Slide Map")
    cmds.editorTemplate("density", addControl=True, label="Density")

    editorTemplate(endLayout=True)


    cmds.editorTemplate(beginLayout="Placement Attributes", collapse=False)

    cmds.editorTemplate("offset", addControl=True, label="Offset")
    cmds.editorTemplate("rotate", addControl=True, label="Rotate")
    cmds.editorTemplate("scale_s", addControl=True, label="Scale S")
    cmds.editorTemplate("scale_t", addControl=True, label="Scale T")
    cmds.editorTemplate("wrap_s", addControl=True, label="Wrap S")
    cmds.editorTemplate("wrap_t", addControl=True, label="Wrap T")

    editorTemplate(endLayout=True)

    # include/call base class/node attributes
    mel.eval('AEdependNodeTemplate "%s"'%nodeName)
    cmds.editorTemplate(addExtraControls=True)
    cmds.editorTemplate(endScrollLayout=True)



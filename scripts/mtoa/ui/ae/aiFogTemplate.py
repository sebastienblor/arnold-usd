import maya.cmds as cmds
import maya.mel as mel

def aiFogTemplate(nodeName):

    aiSwatchDisplay(nodeName)

    cmds.editorTemplate(beginScrollLayout=True)

    cmds.editorTemplate(beginLayout="Fog Attributes", collapse=False)
    cmds.editorTemplate("color", addControl=True, label="Color")
    cmds.editorTemplate("distance", addControl=True, label="Distance")
    cmds.editorTemplate("height", addControl=True, label="Height")

    cmds.editorTemplate(addSeparator=True)

    cmds.editorTemplate("ground_normal", addControl=True, label="Ground Normal")
    cmds.editorTemplate("ground_point", addControl=True, label="Ground Point")

    cmds.editorTemplate(endLayout=True)

    # include/call base class/node attributes
    mel.eval('AEdependNodeTemplate "%s"'%nodeName)
    
    cmds.editorTemplate(addExtraControls=True)
    cmds.editorTemplate(endScrollLayout=True)

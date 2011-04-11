import maya.cmds as cmds
import maya.mel as mel

def aiWireframeTemplate(nodeName):

    mel.eval('AEswatchDisplay "%s"'%nodeName)

    cmds.editorTemplate(beginScrollLayout=True)

    cmds.editorTemplate('AEshaderTypeNew', 'AEshaderTypeReplace', "message", callCustom=True)

    cmds.editorTemplate(beginLayout="Wireframe Attributes", collapse=False)
    cmds.editorTemplate("edge_type", label="Edge Type", addControl=True)
    cmds.editorTemplate("fill_color", label="Fill Color", addControl=True)
    cmds.editorTemplate("line_color", label="Line Color", addControl=True)
    cmds.editorTemplate("line_width", label="Line Width", addControl=True)
    cmds.editorTemplate("raster_space", label="Raster Space", addControl=True)
    cmds.editorTemplate(endLayout=True)

    # include/call base class/node attributes
    mel.eval('AEdependNodeTemplate "%s"'%nodeName)
    cmds.editorTemplate(addExtraControls=True)

    cmds.editorTemplate(endScrollLayout=True)



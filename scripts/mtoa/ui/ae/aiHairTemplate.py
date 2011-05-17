import maya.cmds as cmds
import maya.mel as mel
from mtoa.ui.ae.aiSwatchDisplay import aiSwatchDisplay

def aiHairTemplate(nodeName):

    aiSwatchDisplay(nodeName)

    cmds.editorTemplate(beginScrollLayout=True)
    
    cmds.editorTemplate('AEshaderTypeNew', 'AEshaderTypeReplace', "message", callCustom=True)

    cmds.editorTemplate(beginLayout="Color", collapse=False)

    cmds.editorTemplate("rootcolor", addControl=True, label="Root Color")
    cmds.editorTemplate("tipcolor", addControl=True, label="Tip Color")

    cmds.editorTemplate(endLayout=True)

    cmds.editorTemplate(beginLayout="Diffuse", collapse=False)

    cmds.editorTemplate("diffuse_cache", addControl=True, label="Diffuse Cache")
    cmds.editorTemplate("ambdiff", addControl=True, label="Ambient Diffuse")
    cmds.editorTemplate("kd_ind", addControl=True, label="Indirect Diffuse")

    cmds.editorTemplate(endLayout=True)

    cmds.editorTemplate(beginLayout="Specular", collapse=False)

    cmds.editorTemplate("gloss", addControl=True, label="Gloss")
    cmds.editorTemplate("spec", addControl=True, label="Specular")
    cmds.editorTemplate("spec_color", addControl=True, label="Specular Color")

    cmds.editorTemplate(endLayout=True)

    cmds.editorTemplate(beginLayout="Extra", collapse=False)

    cmds.editorTemplate("opacity", addControl=True, label="Opacity")

    cmds.editorTemplate(endLayout=True)

    # include/call base class/node attributes
    mel.eval('AEdependNodeTemplate "%s"'%nodeName)
    cmds.editorTemplate(addExtraControls=True)
    cmds.editorTemplate(endScrollLayout=True)


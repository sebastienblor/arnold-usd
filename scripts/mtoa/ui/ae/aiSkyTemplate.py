import maya.cmds as cmds
import maya.mel as mel
from mtoa.ui.ae.aiSwatchDisplay import aiSwatchDisplay

def aiSkyTemplate(nodeName):

    aiSwatchDisplay(nodeName)

    cmds.editorTemplate(beginScrollLayout=True)

    cmds.editorTemplate(beginLayout="Sky Attributes", collapse=False)

    cmds.editorTemplate("format", addControl=True, label="Format")
    cmds.editorTemplate("color", addControl=True, label="Color")
    cmds.editorTemplate("intensity", addControl=True, label="Intensity")

    cmds.editorTemplate(endLayout=True)

    cmds.editorTemplate(beginLayout="Render Stats", collapse=True)
    cmds.editorTemplate(beginNoOptimize=True)

    cmds.editorTemplate("castsShadows", addControl=True, label="Casts Shadows")
    cmds.editorTemplate("primaryVisibility", addControl=True, label="Primary Visibility")
    cmds.editorTemplate("visibleInDiffuse", addControl=True, label="Visible in Diffuse")
    cmds.editorTemplate("visibleInGlossy", addControl=True, label="Visible in Glossy")
    cmds.editorTemplate("visibleInReflections", addControl=True, label="Visible in Reflections")
    cmds.editorTemplate("visibleInRefractions", addControl=True, label="Visible in Refractions")
    cmds.editorTemplate(endNoOptimize=True)

    cmds.editorTemplate(endLayout=True)

    cmds.editorTemplate(beginLayout="Hardware Texturing", collapse=True)

    cmds.editorTemplate("sampling", addControl=True,  label="Texture Resolution")
    cmds.editorTemplate("hwtexalpha", addControl=True,  label="Opacity")

    cmds.editorTemplate(endLayout=True)

    cmds.editorTemplate(beginLayout="Viewport", collapse=True)

    cmds.editorTemplate("skyRadius", addControl=True,  label="Sky Radius")
    cmds.editorTemplate("skyFacing", addControl=True,  label="Facing")

    cmds.editorTemplate(endLayout=True)

    # Do not show extra attributes
    extras = ["visibility",
             "intermediateObject",
             "template",
             "ghosting",
             "instObjGroups",
             "useObjectColor",
             "objectColor",
             "drawOverride",
             "lodVisibility",
             "renderInfo",
             "renderLayerInfo",
             "ghostingControl",
             "ghostCustomSteps",
             "ghostFrames",
             "ghostRangeStart",
             "ghostRangeEnd",
             "ghostDriver",
             "motionBlur",
             "visibleInReflections",
             "visibleInRefractions",
             "castsShadows",
             "receiveShadows",
             "maxVisibilitySamplesOverride",
             "maxVisibilitySamples",
             "geometryAntialiasingOverride",
             "antialiasingLevel",
             "shadingSamplesOverride",
             "shadingSamples",
             "maxShadingSamples",
             "volumeSamplesOverride",
             "volumeSamples",
             "depthJitter",
             "ignoreSelfShadowing",
             "primaryVisibility",
             "compInstObjGroups",
             "localPosition",
             "localScale"]

    for extra in extras:
        cmds.editorTemplate(nodeName, suppress=extra)


    # include/call base class/node attributes
    mel.eval('AEdependNodeTemplate "%s"'%nodeName)
    cmds.editorTemplate(addExtraControls=True)
    cmds.editorTemplate(endScrollLayout=True)

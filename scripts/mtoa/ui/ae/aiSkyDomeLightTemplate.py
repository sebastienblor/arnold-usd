import maya.cmds as cmds
import maya.mel as mel
import mtoa.ui.ae.customShapeAttributes as customShapeAttributes

def aiSkyDomeLightTemplate(nodeName):

    cmds.editorTemplate(beginScrollLayout=True)

    cmds.editorTemplate(beginLayout="SkyDomeLight Attributes", collapse=False)

    cmds.editorTemplate("color", addControl=True, label="Color")
    cmds.editorTemplate("intensity", addControl=True, label="Intensity")
    cmds.editorTemplate(beginNoOptimize=True)
    cmds.editorTemplate("affect_diffuse", addControl=True, label="Emit Diffuse")
    cmds.editorTemplate("affect_specular",addControl=True, label="Emit Specular")
    cmds.editorTemplate(endNoOptimize=True)
    cmds.editorTemplate(addSeparator=True)
    cmds.editorTemplate("format", addControl=True, label="Format")
    cmds.editorTemplate("exposure", addControl=True, label="Exposure")
    cmds.editorTemplate("samples", addControl=True, label="Samples")
    cmds.editorTemplate("mis", addControl=True, label="Multiple Importance Sampling")
    cmds.editorTemplate(addSeparator=True)
    customShapeAttributes.commonLightAttributes(nodeName)

    cmds.editorTemplate(endLayout=True)

    cmds.editorTemplate(beginLayout="Shadows", collapse=True)

    cmds.editorTemplate("cast_shadows", addControl=True, label="Cast Shadows")
    cmds.editorTemplate("shadow_density", addControl=True, label="Shadow Density")
    cmds.editorTemplate("shadow_color", addControl=True, label="Shadow Color")

    cmds.editorTemplate(endLayout=True)

    cmds.editorTemplate(beginLayout="Hardware Texturing", collapse=True)

    cmds.editorTemplate("sampling", addControl=True, label="Texture Resolution")
    cmds.editorTemplate("hwtexalpha", addControl=True, label="Opacity")

    cmds.editorTemplate(endLayout=True)

    cmds.editorTemplate(beginLayout="Viewport", collapse=True)

    cmds.editorTemplate("skyRadius", addControl=True, label="Sky Radius")

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

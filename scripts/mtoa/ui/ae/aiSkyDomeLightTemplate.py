import maya.cmds as cmds
import maya.mel as mel
import mtoa.ui.ae.customShapeAttributes as customShapeAttributes
from mtoa.ui.ae.aiSwatchDisplay import aiSwatchDisplay

def aiSkyDomeLightTemplate(nodeName):

    aiSwatchDisplay(nodeName)

    cmds.editorTemplate(beginScrollLayout=True)

    cmds.editorTemplate(beginLayout="SkyDomeLight Attributes", collapse=False)

    cmds.editorTemplate("color", addControl=True, label="Color")
    cmds.editorTemplate("intensity", addControl=True, label="Intensity")
    cmds.editorTemplate(beginNoOptimize=True)
    cmds.editorTemplate("emitDiffuse", addControl=True, label="Emit Diffuse")
    cmds.editorTemplate("emitSpecular",addControl=True, label="Emit Specular")
    cmds.editorTemplate(endNoOptimize=True)
    cmds.editorTemplate(addSeparator=True)
    cmds.editorTemplate("format", addControl=True, label="Format")
    cmds.editorTemplate("aiExposure", addControl=True, label="Exposure")
    cmds.editorTemplate("aiSamples", addControl=True, label="Samples")
    cmds.editorTemplate("aiMis", addControl=True, label="Multiple Importance Sampling")

    cmds.editorTemplate(addSeparator=True)
    cmds.editorTemplate("aiCastShadows", addControl=True, label="Cast Shadows")
    cmds.editorTemplate("shadowDensity", addControl=True, label="Shadow Density")
    cmds.editorTemplate("shadowColor", addControl=True, label="Shadow Color")

    # r3D: The following line expects a template and not a string and was breaking the layout (trac #427)
    #customShapeAttributes.commonLightAttributes(nodeName)
    cmds.editorTemplate(addSeparator=True)
    cmds.editorTemplate("aiBounceFactor", addControl=True, label="Bounce Factor")
    cmds.editorTemplate("aiBounces", addControl=True, label="Bounces")
    
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

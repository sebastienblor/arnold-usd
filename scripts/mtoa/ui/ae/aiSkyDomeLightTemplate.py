import maya.cmds as cmds
import maya.mel as mel
from mtoa.ui.ae.aiSwatchDisplay import aiSwatchDisplay
from mtoa.ui.ae.utils import aeCallback

def ArnoldMakeLightExclusive(attr):
    lightName = attr.split(".")[0]
    cmds.rowLayout(nc=2, cal=[2, 'left'])
    cmds.text(label="")
    cmds.exclusiveLightCheckBox('exclusiveButton', light=lightName, label="Illuminates By Default")
    cmds.setParent('..')
    
def ArnoldReplaceLightExclusive(attr):
    lightName = attr.split(".")[0]
    cmds.exclusiveLightCheckBox('exclusiveButton', edit=True, light=lightName)

def aiSkyDomeLightTemplate(nodeName):

    aiSwatchDisplay(nodeName)

    cmds.editorTemplate(beginScrollLayout=True)

    cmds.editorTemplate(beginLayout="SkyDomeLight Attributes", collapse=False)

    cmds.editorTemplate("color", addControl=True, label="Color")
    cmds.editorTemplate("intensity", addControl=True, label="Intensity")
    cmds.editorTemplate("resolution", addControl=True, label="Resolution")
    cmds.editorTemplate(aeCallback(ArnoldMakeLightExclusive), aeCallback(ArnoldReplaceLightExclusive), "instObjGroups", callCustom=True)
    cmds.editorTemplate("emitDiffuse", addControl=True, label="Emit Diffuse")
    cmds.editorTemplate("emitSpecular",addControl=True, label="Emit Specular")
    cmds.editorTemplate(addSeparator=True)
    cmds.editorTemplate("format", addControl=True, label="Format")
    cmds.editorTemplate("aiExposure", addControl=True, label="Exposure")
    cmds.editorTemplate("aiSamples", addControl=True, label="Samples")
    cmds.editorTemplate("aiNormalize", addControl=True, label="Normalize")


    cmds.editorTemplate(addSeparator=True)
    cmds.editorTemplate("aiCastShadows", addControl=True, label="Cast Shadows")
    cmds.editorTemplate("shadowDensity", addControl=True, label="Shadow Density")
    cmds.editorTemplate("shadowColor", addControl=True, label="Shadow Color")

    # FIXME: use light template code here
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

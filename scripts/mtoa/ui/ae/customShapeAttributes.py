import maya.cmds as cmds
import mtoa.ui.ae.lightFiltersShaderTemplate as lightFiltersShaderTemplate
from mtoa.ui.ae.utils import aeCallback
from mtoa.ui.ae.shapeTemplate import registerUI

def commonLightAttributes(nodeName):
    cmds.editorTemplate("normalize", addDynamicControl=True)
    cmds.editorTemplate("bounceFactor", addDynamicControl=True)
    cmds.editorTemplate("bounces", addDynamicControl=True)

    cmds.editorTemplate(addSeparator=True)

    cmds.editorTemplate("sssSamples", addDynamicControl=True)

    cmds.editorTemplate(beginLayout="Light Filters")

    cmds.editorTemplate(aeCallback(lightFiltersShaderTemplate.customLightFiltersNew), aeCallback(lightFiltersShaderTemplate.customLightFiltersReplace), "light_filters", callCustom=True)

    cmds.editorTemplate(endLayout=True)


def commonShapeAttributes(nodeName):
    cmds.editorTemplate("selfShadows", addDynamicControl=True)
    cmds.editorTemplate("opaque", addDynamicControl=True)
    cmds.editorTemplate("visibleInDiffuse", addDynamicControl=True)
    cmds.editorTemplate("visibleInGlossy", addDynamicControl=True)    

    cmds.editorTemplate(addSeparator=True)

    cmds.editorTemplate("sssUseGi", addDynamicControl=True)
    cmds.editorTemplate("sssMaxSamples", addDynamicControl=True)
    cmds.editorTemplate("sssSampleSpacing", addDynamicControl=True)

@registerUI("mesh")
def builtin_mesh(nodeName):
    commonShapeAttributes(nodeName)
    cmds.editorTemplate(addSeparator=True)

    cmds.editorTemplate("subdivType", addDynamicControl=True)
    cmds.editorTemplate("subdivIterations", addDynamicControl=True)
    cmds.editorTemplate("subdivAdaptiveMetric", addDynamicControl=True)
    cmds.editorTemplate("subdivPixelError", addDynamicControl=True)
    cmds.editorTemplate("subdivDicingCamera", addDynamicControl=True)
    cmds.editorTemplate("subdivUvSmoothing", label="Subdiv UV Smoothing", addDynamicControl=True)

    cmds.editorTemplate(addSeparator=True)

    cmds.editorTemplate("exportTangents", addDynamicControl=True)
    cmds.editorTemplate("exportColors", addDynamicControl=True)

@registerUI("hairSystem")
def builtin_hairSystem(nodeName):
    commonShapeAttributes(nodeName)
    cmds.editorTemplate(addSeparator=True)

    cmds.editorTemplate("overrideHair", addDynamicControl=True)
    cmds.editorTemplate("hairShader", addDynamicControl=True)

    cmds.editorTemplate(addSeparator=True)

    cmds.editorTemplate("minPixelWidth", addDynamicControl=True)
    cmds.editorTemplate("mode", addDynamicControl=True)

@registerUI("ambientLight")
def builtin_ambientLight(nodeName):
    commonLightAttributes(nodeName);

@registerUI("directionalLight")
def builtin_directionalLight(nodeName):
    commonLightAttributes(nodeName);

@registerUI("pointLight")
def builtin_pointLight(nodeName):
    cmds.editorTemplate("affect_volumetrics", addControl=True)
    cmds.editorTemplate("cast_volumetric_shadows", addControl=True)

    cmds.editorTemplate(addSeparator=True)

    commonLightAttributes(nodeName);

@registerUI("spotLight")
def builtin_spotLight(nodeName):
    cmds.editorTemplate("affect_volumetrics", addControl=True)
    cmds.editorTemplate("cast_volumetric_shadows", addControl=True)

    cmds.editorTemplate(addSeparator=True)

    cmds.editorTemplate("aspect_ratio", addControl=True)
    cmds.editorTemplate( "lens_radius", addControl=True)

    cmds.editorTemplate(addSeparator=True)

    commonLightAttributes(nodeName);

@registerUI("areaLight")
def builtin_areaLight(nodeName):
    cmds.editorTemplate("resolution", addControl=True)
    cmds.editorTemplate("affect_volumetrics", addControl=True)
    cmds.editorTemplate("cast_volumetric_shadows", addControl=True)

    cmds.editorTemplate(addSeparator=True)

    cmds.editorTemplate("sidedness", addControl=True)
    cmds.editorTemplate("solid_angle", addControl=True)

    cmds.editorTemplate(addSeparator=True)

    commonLightAttributes(nodeName);


@registerUI("camera")
def builtin_camera(nodeName):
    cmds.editorTemplate("enableDOF", addDynamicControl=True)

    cmds.editorTemplate(addSeparator=True)

    cmds.editorTemplate("focalDistance", addDynamicControl=True)
    cmds.editorTemplate("apertureSize", addDynamicControl=True)

    cmds.editorTemplate(addSeparator=True)

    cmds.editorTemplate("apertureBlades", addDynamicControl=True)
    cmds.editorTemplate("apertureBladeCurvature", addDynamicControl=True)
    cmds.editorTemplate("apertureRotation", addDynamicControl=True)

    cmds.editorTemplate(addSeparator=True)


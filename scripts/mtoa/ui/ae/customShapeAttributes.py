import maya.cmds as cmds
import mtoa.ui.customAttrs as customAttrs
import mtoa.ui.ae.lightFiltersShaderTemplate as lightFiltersShaderTemplate
from mtoa.ui.ae.utils import aeCallback

__all__ = ['registerCustomAttrTemplate', 'customShapeAttributes', 'commonLightAttributes']
global _customAttrTemplates
_customAttrTemplates = {}

def registerCustomAttrTemplate(nodeType, func):
    global _customAttrTemplates
    assert callable(func), "you must pass a callable object"
    print "registering custom attr template for %s" % nodeType
    _customAttrTemplates[nodeType] = func

def customShapeAttributes(nodeName):
    global _customAttrTemplates
    nodeType = cmds.objectType(nodeName)
    if nodeType in _customAttrTemplates:
        cmds.editorTemplate(beginLayout="Arnold")
        _customAttrTemplates[nodeType](nodeName)
        cmds.editorTemplate(endLayout=True)

def commonLightAttributes(nodeName):
    cmds.editorTemplate("normalize", addControl=True)
    cmds.editorTemplate("bounce_factor", addControl=True)
    cmds.editorTemplate("bounces", addControl=True)

    cmds.editorTemplate(addSeparator=True)

    cmds.editorTemplate("sss_samples", addControl=True)

    cmds.editorTemplate(beginLayout="Light Filters")

    cmds.editorTemplate(aeCallback(lightFiltersShaderTemplate.customLightFiltersNew), aeCallback(lightFiltersShaderTemplate.customLightFiltersReplace), "light_filters", callCustom=True)

    cmds.editorTemplate(endLayout=True)

def builtin_mesh(nodeName):
    cmds.editorTemplate(aeCallback(customAttrs.boolNew), aeCallback(customAttrs.boolReplace), "self_shadows", callCustom=True)
    cmds.editorTemplate(aeCallback(customAttrs.boolNew), aeCallback(customAttrs.boolReplace), "opaque", callCustom=True)
    cmds.editorTemplate(addSeparator=True)
    cmds.editorTemplate(aeCallback(customAttrs.boolNew), aeCallback(customAttrs.boolReplace), "diffuse_visibility", callCustom=True)
    cmds.editorTemplate(aeCallback(customAttrs.boolNew), aeCallback(customAttrs.boolReplace), "glossy_visibility", callCustom=True)

    cmds.editorTemplate(addSeparator=True)

    cmds.editorTemplate(aeCallback(customAttrs.boolNew), aeCallback(customAttrs.boolReplace), "subdiv_type", callCustom=True)
    cmds.editorTemplate(aeCallback(customAttrs.intNew), aeCallback(customAttrs.intReplace), "subdiv_iterations", callCustom=True)
    cmds.editorTemplate(aeCallback(customAttrs.enumNew), aeCallback(customAttrs.enumReplace), "subdiv_adaptive_metric", callCustom=True)
    cmds.editorTemplate(aeCallback(customAttrs.floatNew), aeCallback(customAttrs.floatReplace), "subdiv_pixel_error", callCustom=True)
    cmds.editorTemplate(aeCallback(customAttrs.objectListNew), aeCallback(customAttrs.objectListReplace), "subdiv_dicing_camera", callCustom=True)

    cmds.editorTemplate(addSeparator=True)

    cmds.editorTemplate(aeCallback(customAttrs.boolNew), aeCallback(customAttrs.boolReplace), "sss_use_gi", callCustom=True)
    cmds.editorTemplate(aeCallback(customAttrs.intNew), aeCallback(customAttrs.intReplace), "sss_max_samples", callCustom=True)
    cmds.editorTemplate(aeCallback(customAttrs.floatNew), aeCallback(customAttrs.floatReplace), "sss_sample_spacing", callCustom=True)
    cmds.editorTemplate(addSeparator=True)

    cmds.editorTemplate(aeCallback(customAttrs.boolNew), aeCallback(customAttrs.boolReplace), "export_tangents", callCustom=True)
    cmds.editorTemplate(aeCallback(customAttrs.boolNew), aeCallback(customAttrs.boolReplace), "export_colors", callCustom=True)

def builtin_hairSystem(nodeName):
    cmds.editorTemplate(aeCallback(customAttrs.boolNew), aeCallback(customAttrs.boolReplace), "primary_visibility", callCustom=True)
    cmds.editorTemplate(aeCallback(customAttrs.boolNew), aeCallback(customAttrs.boolReplace), "receive_shadows", callCustom=True)
    cmds.editorTemplate(aeCallback(customAttrs.boolNew), aeCallback(customAttrs.boolReplace), "castsShadows", callCustom=True)
    cmds.editorTemplate(aeCallback(customAttrs.boolNew), aeCallback(customAttrs.boolReplace), "self_shadows", callCustom=True)
    cmds.editorTemplate(aeCallback(customAttrs.boolNew), aeCallback(customAttrs.boolReplace), "diffuse_visibility", callCustom=True)
    cmds.editorTemplate(aeCallback(customAttrs.boolNew), aeCallback(customAttrs.boolReplace), "glossy_visibility", callCustom=True)
    cmds.editorTemplate(aeCallback(customAttrs.boolNew), aeCallback(customAttrs.boolReplace), "visibleInReflections", callCustom=True)
    cmds.editorTemplate(aeCallback(customAttrs.boolNew), aeCallback(customAttrs.boolReplace), "visibleInRefractions", callCustom=True)

    cmds.editorTemplate(addSeparator=True)

    cmds.editorTemplate(aeCallback(customAttrs.boolNew), aeCallback(customAttrs.boolReplace), "override_hair", callCustom=True)
    cmds.editorTemplate(aeCallback(customAttrs.shaderNew), aeCallback(customAttrs.shaderReplace), "hair_shader", callCustom=True)

    cmds.editorTemplate(addSeparator=True)

    cmds.editorTemplate(aeCallback(customAttrs.floatNew), aeCallback(customAttrs.floatReplace), "min_pixel_width", callCustom=True)
    cmds.editorTemplate(aeCallback(customAttrs.enumNew), aeCallback(customAttrs.enumReplace), "mode", callCustom=True)
    cmds.editorTemplate(aeCallback(customAttrs.boolNew), aeCallback(customAttrs.boolReplace), "opaque", callCustom=True)

    cmds.editorTemplate(addSeparator=True)

    cmds.editorTemplate(aeCallback(customAttrs.intNew), aeCallback(customAttrs.intReplace), "sss_max_samples", callCustom=True)
    cmds.editorTemplate(aeCallback(customAttrs.floatNew), aeCallback(customAttrs.floatReplace), "sss_sample_spacing", callCustom=True)
    cmds.editorTemplate(aeCallback(customAttrs.boolNew), aeCallback(customAttrs.boolReplace), "sss_use_gi", callCustom=True)

def builtin_ambientLight(nodeName):
    commonLightAttributes(nodeName);

def builtin_directionalLight(nodeName):
    commonLightAttributes(nodeName);

def builtin_pointLight(nodeName):
    cmds.editorTemplate("affect_volumetrics", addControl=True)
    cmds.editorTemplate("cast_volumetric_shadows", addControl=True)

    cmds.editorTemplate(addSeparator=True)

    commonLightAttributes(nodeName);

def builtin_spotLight(nodeName):
    cmds.editorTemplate("affect_volumetrics", addControl=True)
    cmds.editorTemplate("cast_volumetric_shadows", addControl=True)

    cmds.editorTemplate(addSeparator=True)

    cmds.editorTemplate("aspect_ratio", addControl=True)
    cmds.editorTemplate( "lens_radius", addControl=True)

    cmds.editorTemplate(addSeparator=True)

    commonLightAttributes(nodeName);

def builtin_areaLight(nodeName):
    cmds.editorTemplate("affect_volumetrics", addControl=True)
    cmds.editorTemplate("cast_volumetric_shadows", addControl=True)

    cmds.editorTemplate(addSeparator=True)

    cmds.editorTemplate("sidedness", addControl=True)
    cmds.editorTemplate("solid_angle", addControl=True)

    cmds.editorTemplate(addSeparator=True)

    commonLightAttributes(nodeName);


def builtin_camera(nodeName):
    cmds.editorTemplate("enableDOF", addControl=True)

    cmds.editorTemplate(addSeparator=True)

    cmds.editorTemplate("focal_distance", addControl=True)
    cmds.editorTemplate("aperture_size", addControl=True)

    cmds.editorTemplate(addSeparator=True)

    cmds.editorTemplate("aperture_blades", addControl=True)
    cmds.editorTemplate("aperture_blade_curvature", addControl=True)
    cmds.editorTemplate("aperture_rotation", addControl=True)

    cmds.editorTemplate(addSeparator=True)

for name, obj in globals().items():
    if name.startswith('builtin_') and callable(obj):
        nodeType = name.split('_', 1)[1]
        registerCustomAttrTemplate(nodeType, obj)



import maya.cmds as cmds
import mtoa.ui.customAttrs as customAttrs
import mtoa.ui.ae.lightFiltersShaderTemplate as lightFiltersShaderTemplate
from mtoa.ui.ae.utils import aeCallback

#source "AEArnoldLightFiltersTemplate.mel", callCustom=True)

def commonLightAttributes(nodeName):
   cmds.editorTemplate(aeCallback(customAttrs.boolNew), aeCallback(customAttrs.boolReplace), "normalize", callCustom=True)
   cmds.editorTemplate(aeCallback(customAttrs.floatNew), aeCallback(customAttrs.floatReplace), "bounce_factor", callCustom=True)
   cmds.editorTemplate(aeCallback(customAttrs.intNew), aeCallback(customAttrs.intReplace), "bounces", callCustom=True)

   cmds.editorTemplate(addSeparator=True)

   cmds.editorTemplate(aeCallback(customAttrs.intNew), aeCallback(customAttrs.intReplace), "sss_samples", callCustom=True)

   cmds.editorTemplate(beginLayout="Light Filters")

   cmds.editorTemplate(aeCallback(lightFiltersShaderTemplate.customLightFiltersNew), aeCallback(lightFiltersShaderTemplate.customLightFiltersReplace), "light_filters", callCustom=True)

   cmds.editorTemplate(endLayout=True)


def customShapeAttributes(nodeName):
    nodeType = cmds.objectType(nodeName)

    if nodeType == "mesh":
        cmds.editorTemplate(beginLayout="Arnold")

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

        cmds.editorTemplate(endLayout=True)

    elif nodeType == "hairSystem":
        cmds.editorTemplate(beginLayout="Arnold")
        
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

        cmds.editorTemplate(endLayout=True)


    elif nodeType == "ambientLight":
        cmds.editorTemplate(beginLayout="Arnold")

        commonLightAttributes(nodeName);

        cmds.editorTemplate(endLayout=True)



    elif nodeType =="directionalLight":
        cmds.editorTemplate(beginLayout="Arnold")

        commonLightAttributes(nodeName);

        cmds.editorTemplate(endLayout=True)



    elif nodeType =="pointLight":
        cmds.editorTemplate(beginLayout="Arnold")

        cmds.editorTemplate(aeCallback(customAttrs.boolNew), aeCallback(customAttrs.boolReplace), "affect_volumetrics", callCustom=True)
        cmds.editorTemplate(aeCallback(customAttrs.boolNew), aeCallback(customAttrs.boolReplace), "cast_volumetric_shadows", callCustom=True)

        cmds.editorTemplate(addSeparator=True)

        commonLightAttributes(nodeName);

        cmds.editorTemplate(endLayout=True)



    elif nodeType =="spotLight":
        cmds.editorTemplate(beginLayout="Arnold")

        cmds.editorTemplate(aeCallback(customAttrs.boolNew), aeCallback(customAttrs.boolReplace), "affect_volumetrics", callCustom=True)
        cmds.editorTemplate(aeCallback(customAttrs.boolNew), aeCallback(customAttrs.boolReplace), "cast_volumetric_shadows", callCustom=True)

        cmds.editorTemplate(addSeparator=True)

        cmds.editorTemplate(aeCallback(customAttrs.floatNew), aeCallback(customAttrs.floatReplace), "aspect_ratio", callCustom=True)
        cmds.editorTemplate(aeCallback(customAttrs.floatNew), aeCallback(customAttrs.floatReplace), "lens_radius", callCustom=True)

        cmds.editorTemplate(addSeparator=True)

        commonLightAttributes(nodeName);

        cmds.editorTemplate(endLayout=True)


    elif nodeType =="areaLight":
        cmds.editorTemplate(beginLayout="Arnold")

        cmds.editorTemplate(aeCallback(customAttrs.boolNew), aeCallback(customAttrs.boolReplace), "affect_volumetrics", callCustom=True)
        cmds.editorTemplate(aeCallback(customAttrs.boolNew), aeCallback(customAttrs.boolReplace), "cast_volumetric_shadows", callCustom=True)

        cmds.editorTemplate(addSeparator=True)

        cmds.editorTemplate(aeCallback(customAttrs.enumNew), aeCallback(customAttrs.enumReplace), "sidedness", callCustom=True)
        cmds.editorTemplate(aeCallback(customAttrs.boolNew), aeCallback(customAttrs.boolReplace), "solid_angle", callCustom=True)

        cmds.editorTemplate(addSeparator=True)

        commonLightAttributes(nodeName);

        cmds.editorTemplate(endLayout=True)


    elif nodeType =="camera":
        cmds.editorTemplate(beginLayout="Arnold")

        cmds.editorTemplate(aeCallback(customAttrs.boolNew), aeCallback(customAttrs.boolReplace), "enable_dof", callCustom=True)

        cmds.editorTemplate(addSeparator=True)

        cmds.editorTemplate(aeCallback(customAttrs.floatNew), aeCallback(customAttrs.floatReplace), "focal_distance", callCustom=True)
        cmds.editorTemplate(aeCallback(customAttrs.floatNew), aeCallback(customAttrs.floatReplace), "aperture_size", callCustom=True)

        cmds.editorTemplate(addSeparator=True)

        cmds.editorTemplate(aeCallback(customAttrs.intNew), aeCallback(customAttrs.intReplace), "aperture_blades", callCustom=True)
        cmds.editorTemplate(aeCallback(customAttrs.floatNew), aeCallback(customAttrs.floatReplace), "aperture_blade_curvature", callCustom=True)
        cmds.editorTemplate(aeCallback(customAttrs.floatNew), aeCallback(customAttrs.floatReplace), "aperture_rotation", callCustom=True)

        cmds.editorTemplate(addSeparator=True)

        cmds.editorTemplate(endLayout=True)





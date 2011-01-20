import maya.cmds as cmds
from mtoa.ui.ae.customShapeAttributes import registerCustomAttrTemplate
from mtoa.ui.customAttrs import MESH_ATTRIBUTES
import mtoa.ui.customAttrs as customAttrs
from mtoa.ui.ae.utils import aeCallback

def shaveHairUI(nodeName):
    cmds.editorTemplate(aeCallback(customAttrs.boolNew), aeCallback(customAttrs.boolReplace), "primaryVisibility", callCustom=True)
    cmds.editorTemplate(aeCallback(customAttrs.boolNew), aeCallback(customAttrs.boolReplace), "receive_shadows", callCustom=True)
    cmds.editorTemplate(aeCallback(customAttrs.boolNew), aeCallback(customAttrs.boolReplace), "castsShadows", callCustom=True)
    cmds.editorTemplate(aeCallback(customAttrs.boolNew), aeCallback(customAttrs.boolReplace), "self_shadows", callCustom=True)
    cmds.editorTemplate(aeCallback(customAttrs.boolNew), aeCallback(customAttrs.boolReplace), "diffuse_visibility", callCustom=True)
    cmds.editorTemplate(aeCallback(customAttrs.boolNew), aeCallback(customAttrs.boolReplace), "glossy_visibility", callCustom=True)
    cmds.editorTemplate(aeCallback(customAttrs.boolNew), aeCallback(customAttrs.boolReplace), "visibleInReflections", callCustom=True)
    cmds.editorTemplate(aeCallback(customAttrs.boolNew), aeCallback(customAttrs.boolReplace), "visibleInRefractions", callCustom=True)
    cmds.editorTemplate(addSeparator=True)
    cmds.editorTemplate(aeCallback(customAttrs.boolNew), aeCallback(customAttrs.boolReplace), "override_shader", callCustom=True)
    cmds.editorTemplate(aeCallback(customAttrs.shaderNew), aeCallback(customAttrs.shaderReplace),"shave_shader", callCustom=True)
    cmds.editorTemplate(addSeparator=True)
    cmds.editorTemplate(aeCallback(customAttrs.floatNew), aeCallback(customAttrs.floatReplace), "min_pixel_width", callCustom=True)
    cmds.editorTemplate(aeCallback(customAttrs.enumNew), aeCallback(customAttrs.enumReplace), "mode", callCustom=True)
    cmds.editorTemplate(addSeparator=True)
    cmds.editorTemplate(aeCallback(customAttrs.intNew),  aeCallback(customAttrs.intReplace), "sss_max_samples", callCustom=True)
    cmds.editorTemplate(aeCallback(customAttrs.floatNew), aeCallback(customAttrs.floatReplace), "sss_sample_spacing", callCustom=True)
    cmds.editorTemplate(aeCallback(customAttrs.boolNew), aeCallback(customAttrs.boolReplace), "sss_use_gi", callCustom=True)

def OverrideCurveChange(*args):
    flag = cmds.checkBoxGrp('nurbsCurve_override_curve', query=True, value1=True) == True
    cmds.control('nurbsCurve_curve_shader', edit=True, enable=flag)

def OverrideShaveChange(*args):
    flag = cmds.checkBoxGrp('shaveHair_override_shader', query=True, value1=True) == True
    cmds.control('shaveHair_shave_shader', edit=True, enable=flag)

# temporary monkey-patch solution

MESH_ATTRIBUTES["override_shader"] = {       'label':"Override Shader",       'default':0,    'min':0,    'max':1,    'sMin':0,    'sMax':1,    'changeCallback':OverrideShaveChange }
MESH_ATTRIBUTES["shave_shader"] = {          'label':"Shader",                'default':None, 'min':None, 'max':None, 'sMin':None, 'sMax':None, 'changeCallback':None }


registerCustomAttrTemplate('shaveHair', shaveHairUI)

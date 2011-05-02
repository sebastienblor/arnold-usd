import maya.cmds as cmds
from mtoa.ui.ae.customShapeAttributes import registerCustomAttrTemplate, commonShapeAttributes
from mtoa.ui.customAttrs import MESH_ATTRIBUTES
import mtoa.ui.customAttrs as customAttrs
from mtoa.ui.ae.utils import aeCallback

def shaveHairUI(nodeName):
    cmds.editorTemplate(beginNoOptimize=True)
    cmds.editorTemplate("castsShadows", addDynamicControl=True)
    cmds.editorTemplate("receiveShadows", addDynamicControl=True)
    # motion blur should be here
    cmds.editorTemplate("primaryVisibility", addDynamicControl=True)

    cmds.editorTemplate(addSeparator=True)

    commonShapeAttributes(nodeName)

    cmds.editorTemplate(endNoOptimize=True)

    cmds.editorTemplate(addSeparator=True)

    cmds.editorTemplate(aeCallback(customAttrs.boolNew), aeCallback(customAttrs.boolReplace), "override_shader", callCustom=True)
    cmds.editorTemplate(aeCallback(customAttrs.shaderNew), aeCallback(customAttrs.shaderReplace),"shave_shader", callCustom=True)
    cmds.editorTemplate(addSeparator=True)

    cmds.editorTemplate("minPixelWidth", addDynamicControl=True)
    cmds.editorTemplate("mode", addDynamicControl=True)

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

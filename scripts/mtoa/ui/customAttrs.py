import maya.cmds as cmds
import mtoa.ui.ae.utils as aeUtils
from mtoa.ui.callback import *

MESH_ATTRIBUTES ={

   # Mesh
   #
   "self_shadows":{       'label':"Self Shadows",         'default':1,      'min':0, 'max':1,          'sMin':0, 'sMax':1,       'changeCallback':None },
   "opaque":{             'label':"Opaque",               'default':1,      'min':0, 'max':1,          'sMin':0, 'sMax':1,       'changeCallback':None },
   "diffuse_visibility":{ 'label':"Visible In Diffuse",   'default':1,      'min':0, 'max':1,          'sMin':0, 'sMax':1,       'changeCallback':None },
   "glossy_visibility":{  'label':"Visible In Glossy",    'default':1,      'min':0, 'max':1,          'sMin':0, 'sMax':1,       'changeCallback':None },
   "subdiv_type":{        'label':"Subdivision Surface",  'default':0,      'min':0, 'max':1,          'sMin':0, 'sMax':1,       'changeCallback':"subdivChange" },
   "subdiv_iterations":{  'label':"Iterations",           'default':1,      'min':0, 'max':100,        'sMin':0, 'sMax':10,      'changeCallback':None },
   "subdiv_pixel_error":{ 'label':"Pixel Error",          'default':0,      'min':0, 'max':50,         'sMin':0, 'sMax':10,      'changeCallback':None },
   "sss_use_gi":{         'label':"Use GI for SSS",       'default':1,      'min':0, 'max':1,          'sMin':0, 'sMax':1,       'changeCallback':None },
   "sss_max_samples":{    'label':"Max. Samples for SSS", 'default':100000, 'min':0, 'max':1000000000, 'sMin':0, 'sMax':1000000, 'changeCallback':None },
   "sss_sample_spacing":{ 'label':"SSS Sample Spacing",   'default':0.1,    'min':0, 'max':10,         'sMin':0, 'sMax':1,       'changeCallback':None },
   "export_tangents":{    'label':"Export tangents",      'default':0,      'min':0, 'max':0,          'sMin':0, 'sMax':1,       'changeCallback':None },

   # Lights
   #
   "normalize":{               'label':"Normalize",                'default':1,   'min':0, 'max':1,     'sMin':0,  'sMax':1,   'changeCallback':None },
   "bounce_factor":{           'label':"Bounce Factor",            'default':1,   'min':0, 'max':20,    'sMin':0,  'sMax':20,  'changeCallback':None },
   "bounces":{                 'label':"Bounces",                  'default':999, 'min':0, 'max':10000, 'sMin':0,  'sMax':999, 'changeCallback':None },
   "sss_samples":{             'label':"SSS Samples",              'default':-1,  'min':0, 'max':100,   'sMin':-1, 'sMax':100, 'changeCallback':None },
   "affect_volumetrics":{      'label':"Affects Volumetrics",      'default':1,   'min':0, 'max':1,     'sMin':0,  'sMax':1,   'changeCallback':"volumetricsChange" },
   "cast_volumetric_shadows":{ 'label':"Casts Volumetric Shadows", 'default':1,   'min':0, 'max':1,     'sMin':0,  'sMax':1,   'changeCallback':None },
   "aspect_ratio":{            'label':"Aspect Ratio",             'default':1,   'min':0, 'max':100,   'sMin':0,  'sMax':1,   'changeCallback':None },
   "lens_radius":{             'label':"Lens Radius",              'default':0,   'min':0, 'max':100,   'sMin':0,  'sMax':1,   'changeCallback':None },
   "solid_angle":{             'label':"Solid Angle Sampling",     'default':0,   'min':0, 'max':1,     'sMin':0,  'sMax':1,   'changeCallback':None },

   # Hairs
   #
   "override_hair":{        'label':"Override Hair Shader",   'default':0,    'min':0,    'max':1,    'sMin':0,    'sMax':1,    'changeCallback':"overrideHairChange" },
   "hair_shader":{          'label':"Hair Shader",            'default':None, 'min':None, 'max':None, 'sMin':None, 'sMax':None, 'changeCallback':None },
   "min_pixel_width":{      'label':"Minimum Pixel Width",    'default':0,    'min':0,    'max':1,    'sMin':0,    'sMax':1,    'changeCallback':None },
   "receive_shadows":{      'label':"Receive Shadows",        'default':1,    'min':0,    'max':1,    'sMin':0,    'sMax':1,    'changeCallback':None },
   "castsShadows":{         'label':"Cast Shadows",           'default':1,    'min':0,    'max':1,    'sMin':0,    'sMax':1,    'changeCallback':None },
   "visibleInReflections":{ 'label':"Visible in Reflections", 'default':1,    'min':0,    'max':1,    'sMin':0,    'sMax':1,    'changeCallback':None },
   "visibleInRefractions":{ 'label':"Visible in Refractions", 'default':1,    'min':0,    'max':1,    'sMin':0,    'sMax':1,    'changeCallback':None },
   "primary_visibility":{   'label':"Primary Visibility",     'default':1,    'min':0,    'max':1,    'sMin':0,    'sMax':1,    'changeCallback':None },
#   Some hair attributes already defined on Mesh, no need for redeclaration
#   This can lead to problems if different default values exist for diferent nodes
#   "sss_use_gi",              "Use GI for SSS",          "1",        "0",  "1",           "0",  "1",        "",
#   "sss_max_samples",         "Max. Samples for SSS",    "100000",   "0",  "1000000000",  "0",  "1000000",  "",
#   "sss_sample_spacing",      "SSS Sample Spacing",      "0.1",      "0",  "10",          "0",  "1",        "",
#   "self_shadows",            "Self Shadows",            "1",        "0",  "1",           "0",  "1",        "",
#   "opaque",                  "Opaque",                  "1",        "0",  "1",           "0",  "1",        "",
#   "diffuse_visibility",      "Visible In Diffuse",      "1",        "0",  "1",           "0",  "1",        "",
#   "glossy_visibility",       "Visible In Glossy",       "1",        "0",  "1",           "0",  "1",        "",

# Enums

#   Attribute                   Label                   Def   Enum values                             Change callback

   "subdiv_adaptive_metric":{ 'label':"Adaptive Metric", 'default':0,  'enumValues':"SUBDIV_METRIC_VALUES", 'changeCallback':None },
   "sidedness":{              'label':"Sidedness",       'default':1,  'enumValues':"SIDEDNESS_VALUES",     'changeCallback':None },
   "mode":{                   'label':"Hair Mode",       'default':0,  'enumValues':"HAIR_MODE_VALUES",     'changeCallback':None },

# Object lists

#   Attribute                   Label                   Def         Enum values                             Change callback

   "subdiv_dicing_camera":{ 'label':"Dicing Camera", 'default':"Default", 'enumValues':"dicingCameraGetList()", 'changeCallback':None },


# Light filters

   "light_filters":{ 'label':"Light Filters" },


#   Cameras

#   Attribute                   Label                   Def         Enum values                        Change callback

   "enable_dof":{               'label':"Depth of field",  'default':0,   'min':0,   'max':1,          'sMin':0,   'sMax':0,   'changeCallback':"dofChange" },
   "focal_distance":{           'label':"Distance",        'default':5,   'min':0,   'max':1000000000, 'sMin':0,   'sMax':200, 'changeCallback':None },
   "aperture_size":{            'label':"Size",            'default':0.2, 'min':0,   'max':20,         'sMin':0,   'sMax':2,   'changeCallback':None },
   "aperture_blades":{          'label':"Blades",          'default':20,  'min':3,   'max':40,         'sMin':0,   'sMax':20,  'changeCallback':None },
   "aperture_blade_curvature":{ 'label':"Blade_curvature", 'default':0,   'min':-20, 'max':20,         'sMin':-5,  'sMax':5,   'changeCallback':None },
   "aperture_rotation":{        'label':"Rotation",        'default':0,   'min':0,   'max':360,        'sMin':0,   'sMax':50,  'changeCallback':None }
}

SUBDIV_METRIC_VALUES = [ "Automatic", "Edge Length", "Flatness" ]
SIDEDNESS_VALUES = [ "Double", "Single" ]
HAIR_MODE_VALUES = [ "Ribbon", "Thick" ]

def overrideHairChange(*args):
    flag = cmds.checkBoxGrp('hairSystem_override_hair', query=True, value1=True)
    cmds.control('hairSystem_hair_shader', edit=True, enable=flag)


def subdivChange(*args):
    flag = cmds.checkBoxGrp('mesh_subdiv_type', query=True, value1=True)
    cmds.control('mesh_subdiv_iterations', edit=True, enable=flag)
    cmds.control('mesh_subdiv_adaptive_metric', edit=True, enable=flag)
    cmds.control('mesh_subdiv_pixel_error', edit=True, enable=flag)
    cmds.control('mesh_subdiv_dicing_camera', edit=True, enable=flag)


def dofChange(*args):
    flag = cmds.checkBoxGrp('camera_enable_dof', query=True, value1=True)
    cmds.control('camera_focal_distance', edit=True, enable=flag)
    cmds.control('camera_aperture_size', edit=True, enable=flag)
    cmds.control('camera_aperture_blades', edit=True, enable=flag)
    cmds.control('camera_aperture_blade_curvature', edit=True, enable=flag)
    cmds.control('camera_aperture_rotation', edit=True, enable=flag)

def volumetricsChange(*args):
    flag = cmds.checkBoxGrp('light_affect_volumetrics', query=True, value1=True)
    cmds.control('light_cast_volumetric_shadows', edit=True, enable=flag)


def dicingCameraGetList(*args):
    values = cmds.ls(type='camera')
    cmds.menuItem(label='Default')
    for value in values:
        perspective = not cmds.camera(value, query=True, orthographic=True)
        if perspective:
            cmds.menuItem(label=value)

def getCustomAttributeData(attrib):
    data = {}
    for j in MESH_ATTRIBUTES.keys():
        if j == attrib:
            data = MESH_ATTRIBUTES[j]
            if MESH_ATTRIBUTES[j]['changeCallback']:
                MESH_ATTRIBUTES[j]['changeCallback'] = getFunction(MESH_ATTRIBUTES[j]['changeCallback'])
    if not data:
        data = {'label':"<Unknown Attribute>", 'default':0, 'min':0, 'max':0, 'sMin':0, 'sMax':0, 'changeCallback':None }

    return data

def getFunction(callback):
    func = None
    l = globals()
    if callback in l.keys():
        func = l[callback]

    return func

def shaderNew(attr):
    nodeName, attribName = attr.split('.')
    nodeType = aeUtils.getNodeType(nodeName)
    attribData = getCustomAttributeData(attribName)

    if not aeUtils.attributeExists(attribName, nodeName):
        cmds.addAttr(nodeName, longName=attribName, usedAsColor=True, attributeType='float3')
        cmds.addAttr(nodeName, longName='%s.R'%attribName, usedAsColor=True, attributeType='float')
        cmds.addAttr(nodeName, longName='%s.G'%attribName, usedAsColor=True, attributeType='float')
        cmds.addAttr(nodeName, longName='%s.B'%attribName, usedAsColor=True, attributeType='float')

    cmds.attrNavigationControlGrp('%s_%s'%(nodeType, attribName), label=attribData['label'], attribute=attr)

def shaderReplace(attr):
    nodeName, attribName = attr.split('.')
    nodeType = aeUtils.getNodeType(nodeName)
    attribData = getCustomAttributeData(attribName)

    if not aeUtils.attributeExists(attribName, nodeName):
        cmds.addAttr(nodeName, ln=attribName, at='message')

    cmds.attrNavigationControlGrp('%s_%s'%(nodeType, attribName), edit=True, attribute=attr)

def boolNew(attr):
    nodeName, attribName = attr.split('.')
    nodeType = aeUtils.getNodeType(nodeName)
    attribData = getCustomAttributeData(attribName)

    if not aeUtils.attributeExists(attribName, nodeName):
        cmds.addAttr(nodeName, ln=attribName, at='bool', dv=attribData['default'])

    cmds.checkBoxGrp('%s_%s'%(nodeType, attribName), label1=attribData['label'])

    if attribData['changeCallback']:
        cmds.checkBoxGrp('%s_%s'%(nodeType, attribName), edit=True, cc=attribData['changeCallback'])
        cmds.evalDeferred(attribData['changeCallback'])

    cmds.connectControl('%s_%s'%(nodeType, attribName), attr, index=2)


def boolReplace(attr):
    nodeName, attribName = attr.split('.')
    nodeType = aeUtils.getNodeType(nodeName)
    attribData = getCustomAttributeData(attribName)

    if not aeUtils.attributeExists(attribName, nodeName):
        cmds.addAttr(nodeName, ln=attribName, at='bool', dv=int(attribData['default']))

    if attribData['changeCallback']:
        cmds.evalDeferred(attribData['changeCallback'])

    cmds.connectControl('%s_%s'%(nodeType, attribName), attr, index=2)


def intNew(attr):
    nodeName, attribName = attr.split('.')
    nodeType = aeUtils.getNodeType(nodeName)
    attribData = getCustomAttributeData(attribName)

    if not aeUtils.attributeExists(attribName, nodeName):
       cmds.addAttr(nodeName, ln=attribName, at='long', dv=int(attribData['default']), min=int(attribData['min']), max=int(attribData['max']))

    cmds.intSliderGrp( '%s_%s'%(nodeType, attribName), label=attribData['label'], min=attribData['sMin'], max=attribData['sMax'], fmn=attribData['min'], fmx=attribData['max'])

    if attribData['changeCallback']:
        cmds.intSliderGrp('%s_%s'%(nodeType, attribName), edit=True, cc=attribData['changeCallback'])
        cmds.evalDeferred(attribData['changeCallback'])

    cmds.connectControl('%s_%s'%(nodeType, attribName), attr)


def intReplace(attr):
    nodeName, attribName = attr.split('.')
    nodeType = aeUtils.getNodeType(nodeName)
    attribData = getCustomAttributeData(attribName)

    if not aeUtils.attributeExists(attribName, nodeName):
        cmds.addAttr(nodeName, ln=attribName, at='long', dv=int(attribData['default']), min=attribData['min'], max=attribData['max'])

    if attribData['changeCallback']:
        cmds.evalDeferred(attribData['changeCallback'])

    cmds.connectControl('%s_%s'%(nodeType, attribName), attr)

def floatNew(attr):
    nodeName, attribName = attr.split('.')
    nodeType = aeUtils.getNodeType(nodeName)
    attribData = getCustomAttributeData(attribName)

    if not aeUtils.attributeExists(attribName, nodeName):
      cmds.addAttr(nodeName, ln=attribName, at='float', dv=float(attribData['default']), min=attribData['min'], max=attribData['max'])

    cmds.floatSliderGrp( '%s_%s'%(nodeType, attribName), label=attribData['label'], min=attribData['sMin'], max=attribData['sMax'], fmn=attribData['min'], fmx=attribData['max'])

    if attribData['changeCallback']:
        cmds.floatSliderGrp('%s_%s'%(nodeType, attribName), edit=True, cc=attribData['changeCallback'])
        cmds.evalDeferred(attribData['changeCallback'])

    cmds.connectControl('%s_%s'%(nodeType, attribName), attr)

def floatReplace(attr):
    nodeName, attribName = attr.split('.')
    nodeType = aeUtils.getNodeType(nodeName)
    attribData = getCustomAttributeData(attribName)

    if not aeUtils.attributeExists(attribName, nodeName):
      cmds.addAttr(nodeName, ln=attribName, at='float', dv=attribData['default'], min=attribData['min'], max=attribData['max'])

    if attribData['changeCallback']:
        cmds.evalDeferred(attribData['changeCallback'])

    cmds.connectControl('%s_%s'%(nodeType, attribName), attr)

def enumChanged(attr, userChangeCB):
    nodeName, attribName = attr.split('.')
    nodeType = aeUtils.getNodeType(nodeName)

    val = cmds.optionMenuGrp('%s_%s'%(nodeType, attribName), query=True, select=True)

    cmds.setAttr(attr, (val - 1))

    if userChangeCB:
        cmds.evalDeferred(userChangeCB)


def enumNew(attr):
    nodeName, attribName = attr.split('.')
    nodeType = aeUtils.getNodeType(nodeName)
    attribData = getCustomAttributeData(attribName)

    if not aeUtils.attributeExists(attribName, nodeName):
      cmds.addAttr(nodeName, ln=attribName, at='long', dv=int(attribData['default']))

    val = cmds.getAttr(attr) + 1

    cmds.optionMenuGrp('%s_%s'%(nodeType, attribName), label=attribData['label'])

    for j in eval(attribData['enumValues']):
        cmds.menuItem(label=j)

    cmds.optionMenuGrp('%s_%s'%(nodeType, attribName), edit=True, select=val, cc=Callback(enumChanged, attr, attribData['changeCallback']))

    if attribData['changeCallback']:
        cmds.evalDeferred(attribData['changeCallback'])


def enumReplace(attr):
    nodeName, attribName = attr.split('.')
    nodeType = aeUtils.getNodeType(nodeName)
    attribData = getCustomAttributeData(attribName)

    if not aeUtils.attributeExists(attribName, nodeName):
      cmds.addAttr(nodeName, ln=attribName, at='long', dv=int(attribData['default']))

    val = cmds.getAttr(attr) + 1

    cmds.optionMenuGrp('%s_%s'%(nodeType, attribName), edit=True, select=val, cc=Callback(enumChanged, attr, attribData['changeCallback']))

    if attribData['changeCallback']:
        cmds.evalDeferred(attribData['changeCallback'])


def objectListChanged(attr, userChangeCB):
    nodeName, attribName = attr.split('.')
    nodeType = aeUtils.getNodeType(nodeName)

    val = cmds.optionMenuGrp('%s_%s'%(nodeType, attribName), query=True, value=True)

    cmds.setAttr(attr, val, type='string')

    if userChangeCB:
        cmds.evalDeferred(userChangeCB)


def objectListNew(attr):
    nodeName, attribName = attr.split('.')
    nodeType = aeUtils.getNodeType(nodeName)
    attribData = getCustomAttributeData(attribName)

    if not aeUtils.attributeExists(attribName, nodeName):
        cmds.addAttr(nodeName, ln=attribName, dt='string')
        cmds.setAttr(attr, attribData['default'], type='string')

    cmds.optionMenuGrp('%s_%s'%(nodeType, attribName), label=attribData['label'])

    eval(attribData['enumValues'])

    val = cmds.getAttr(attr)

    cmds.optionMenuGrp('%s_%s'%(nodeType, attribName), edit=True, value=val, cc=Callback(objectListChanged, attr, attribData['changeCallback']))

    if attribData['changeCallback']:
        cmds.evalDeferred(attribData['changeCallback'])


def objectListReplace(attr):
    nodeName, attribName = attr.split('.')
    nodeType = aeUtils.getNodeType(nodeName)
    attribData = getCustomAttributeData(attribName)

    if not aeUtils.attributeExists(attribName, nodeName):
        cmds.addAttr(nodeName, ln=attribName, dt='string')
        cmds.setAttr(attr, attribData['default'], type='string')

    cmds.deleteUI('%s_%s'%(nodeType, attribName))
    cmds.optionMenuGrp('%s_%s'%(nodeType, attribName), label=attribData['label'])

    eval(attribData['enumValues'])

    val = cmds.getAttr(attr)

    cmds.optionMenuGrp('%s_%s'%(nodeType, attribName), edit=True, value=val, cc=Callback(objectListChanged, attr, attribData['changeCallback']))

    if attribData['changeCallback']:
        cmds.evalDeferred(attribData['changeCallback'])




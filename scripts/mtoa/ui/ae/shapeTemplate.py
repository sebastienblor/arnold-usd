import maya.cmds as cmds
import maya.mel as mel

global _customAttrTemplates
_customAttrTemplates = {}

def registerCustomAttrTemplate(nodeType, func):
    global _customAttrTemplates
    assert callable(func), "you must pass a callable object"
    print "registering custom attr template for %s" % nodeType
    _customAttrTemplates[nodeType] = func

def registerUI(nodeType):
    "decorator for easily registering a UI function"
    def registerUIDecorator(func):
        registerCustomAttrTemplate(nodeType, func)
        return func
    return registerUIDecorator

def shapeTemplate(nodeName):

    global _customAttrTemplates
    nodeType = cmds.objectType(nodeName)
    if nodeType in _customAttrTemplates:
        cmds.editorTemplate(beginLayout="Arnold")
        _customAttrTemplates[nodeType](nodeName)
        cmds.editorTemplate(endLayout=True)

    cmds.editorTemplate(beginLayout=mel.eval('uiRes("m_AEshapeTemplate.kObjectDisplay")'))

    # include/call base class/node attributes
    mel.eval('AEdagNodeCommon "%s"'%nodeName)
    cmds.editorTemplate(endLayout=True)

    # include/call base class/node attributes
    mel.eval('AEdagNodeInclude "%s"'%nodeName)


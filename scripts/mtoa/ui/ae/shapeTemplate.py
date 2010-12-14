import maya.cmds as cmds
import maya.mel as mel

import mtoa.ui.ae.customShapeAttributes as customShapeAttributes

def shapeTemplate(nodeName):

    customShapeAttributes.customShapeAttributes(nodeName)

    cmds.editorTemplate(beginLayout=mel.eval('uiRes("m_AEshapeTemplate.kObjectDisplay")'))

    # include/call base class/node attributes
    mel.eval('AEdagNodeCommon "%s"'%nodeName)
    cmds.editorTemplate(endLayout=True)

    # include/call base class/node attributes
    mel.eval('AEdagNodeInclude "%s"'%nodeName)


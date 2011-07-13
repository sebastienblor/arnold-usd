import maya.cmds as cmds
import maya.mel as mel

import mtoa.utils as utils
import mtoa.ui.ae.utils as aeUtils
import mtoa.ui.ae.shaderTemplate as shaderTemplate

def aiWriteColorTemplate(nodeName):

    #mel.eval('AEswatchDisplay "%s"' % nodeName)

    cmds.editorTemplate(beginScrollLayout=True)
    cmds.editorTemplate(beginLayout="Write Color Attributes", collapse=False)

    cmds.editorTemplate("beauty", addControl=True)
    cmds.editorTemplate("input", addControl=True)
    shaderTemplate.addAOVControl('aiWriteColor', 'aovName')

    cmds.editorTemplate(endLayout=True)

    # include/call base class/node attributes
    mel.eval('AEdependNodeTemplate "%s"'%nodeName)
    cmds.editorTemplate(addExtraControls=True)

    cmds.editorTemplate(endScrollLayout=True)


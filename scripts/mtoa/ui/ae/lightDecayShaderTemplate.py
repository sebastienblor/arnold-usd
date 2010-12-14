import mtoa.ui.ae.utils as aeUtils
from mtoa.ui.ae.utils import aeCallback

def checkNearAtten(nodeName):
   aeUtils.arnoldDimControlIfFalse(nodeName, "near_start", "use_near_atten")
   aeUtils.arnoldDimControlIfFalse(nodeName, "near_end", "use_near_atten")

def checkFarAtten(nodeName):
   aeUtils.arnoldDimControlIfFalse(nodeName, "far_start", "use_far_atten")
   aeUtils.arnoldDimControlIfFalse(nodeName, "far_end", "use_far_atten")

def lightDecayShaderTemplate(nodeName):

    cmds.editorTemplate(beginScrollLayout=True)

    cmds.editorTemplate(beginLayout="Decay Attributes", collapse=False)

    cmds.editorTemplate("decay_type", addControl=True, label="Geometry Type")
    cmds.editorTemplate("decay_radius", addControl=True, label="Density")
    cmds.editorTemplate("decay_clamp", addControl=True, label="Geometry Type")

    editorTemplate(endLayout=True)

    cmds.editorTemplate(beginLayout="Attenuation", collapse=False)

    cmds.editorTemplate("use_near_atten", aeCallback(checkNearAtten), addControl=True, label="Use Near Attenuation")
    cmds.editorTemplate("near_start", addControl=True, label="Near Start")
    cmds.editorTemplate("near_end", addControl=True, label="Near End")

    cmds.editorTemplate(addSeparator=True)

    cmds.editorTemplate("use_far_atten", aeCallback(checkNearAtten), addControl=True, label="Use Far Attenuation")
    cmds.editorTemplate("far_start", addControl=True, label="Far Start")
    cmds.editorTemplate("far_end", addControl=True, label="Far End")

    editorTemplate(endLayout=True)

    cmds.editorTemplate(beginLayout="Transform Attributes", collapse=False)

    cmds.editorTemplate("geometry_matrix", addControl=True, label="Geometry Matrix")

    editorTemplate(endLayout=True)

    # include/call base class/node attributes
    mel.eval('AEdependNodeTemplate "%s"'%nodeName)
    cmds.editorTemplate(addExtraControls=True)
    cmds.editorTemplate(endScrollLayout=True)

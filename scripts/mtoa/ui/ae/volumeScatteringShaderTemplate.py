import maya.cmds as cmds
import maya.mel as mel

def volumeScatteringShaderTemplate(nodeName):

    cmds.editorTemplate(beginScrollLayout=True)

    cmds.editorTemplate(beginLayout="Volume Attributes", collapse=False)
    cmds.editorTemplate("rgb_density", label="Color", addControl=True)
    cmds.editorTemplate("density", label="Density", addControl=True)
    cmds.editorTemplate(addSeparator=True)
    cmds.editorTemplate("rgb_attenuation", label="Attenuation Color", addControl=True)
    cmds.editorTemplate("attenuation", label="Attenuation", addControl=True)
    cmds.editorTemplate(addSeparator=True)
    cmds.editorTemplate("phase_function", label="Phase Function", addControl=True)
    cmds.editorTemplate("eccentricity", label="Eccentricity", addControl=True)
    cmds.editorTemplate(addSeparator=True)
    cmds.editorTemplate("importance_sampling", label="Importance Sampling", addControl=True)
    cmds.editorTemplate("sampling_pattern", label="Sampling Pattern", addControl=True)
    cmds.editorTemplate("samples", label="Samples", addControl=True)
    cmds.editorTemplate(addSeparator=True)
    cmds.editorTemplate("mscattering_depth", label="Mscattering Depth", addControl=True)
    cmds.editorTemplate("mscattering_samples", label="Mscattering Samples", addControl=True)
    cmds.editorTemplate(endLayout=True)

    cmds.editorTemplate(beginLayout="Contribution Attributes", collapse=False)
    cmds.editorTemplate("affect_camera", label="Camera", addControl=True)
    cmds.editorTemplate("affect_diffuse", label="Diffuse", addControl=True)
    cmds.editorTemplate("affect_reflection", label="Reflection", addControl=True)
    cmds.editorTemplate(endLayout=True)

    # include/call base class/node attributes
    mel.eval('AEdependNodeTemplate "%s"'%nodeName)
    cmds.editorTemplate(addExtraControls=True)

    cmds.editorTemplate(endScrollLayout=True)

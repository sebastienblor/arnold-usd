import maya.cmds as cmds
import maya.mel as mel

import mtoa.utils as utils
import mtoa.ui.ae.utils as aeUtils
from mtoa.ui.ae.utils import aeCallback
from mtoa.ui.ae.aiSwatchDisplay import aiSwatchDisplay
from mtoa.ui.ae.shaderTemplate import aovLayout

def bumpNew(attrName):
    cmds.setUITemplate('attributeEditorTemplate', pst=True)
    cmds.attrNavigationControlGrp('bumpControl', label="Bump Mapping", at=attrName)
    cmds.setUITemplate(ppt=True)

def bumpReplace(attrName):
    cmds.attrNavigationControlGrp('bumpControl', edit=True, at=attrName)

def checkSpecularFresnel(nodeName):
    aeUtils.arnoldDimControlIfFalse(nodeName, "Ksn", "specular_Fresnel")

def checkReflectionFresnel(nodeName):
    aeUtils.arnoldDimControlIfFalse(nodeName, "Krn", "Fresnel")

def aiStandardTemplate(nodeName):

    aiSwatchDisplay(nodeName)

    cmds.editorTemplate(beginScrollLayout=True)

    cmds.editorTemplate('AEshaderTypeNew', 'AEshaderTypeReplace', "message", callCustom=True)

    cmds.editorTemplate(beginLayout="Diffuse", collapse=False)
    cmds.editorTemplate("Kd_color", label="Color", annotation="Diffuse Color", addControl=True)
    cmds.editorTemplate("Kd", label="Weight", addControl=True)
    cmds.editorTemplate("diffuse_roughness", label="Roughness", addControl=True)
    cmds.editorTemplate("Kb", label="Backlight", addControl=True)
    cmds.editorTemplate(addSeparator=True)
    cmds.editorTemplate("Fresnel_affect_diff", label="Fresnel affects Diffuse", addControl=True)

    cmds.editorTemplate(beginLayout="Extended Controls", collapse=True)
    cmds.editorTemplate("direct_diffuse", label="Direct Diffuse", addControl=True)
    cmds.editorTemplate("indirect_diffuse", label="Indirect Diffuse", addControl=True)
    cmds.editorTemplate(endLayout=True)
    cmds.editorTemplate(endLayout=True)#End Diffuse Layout

    cmds.editorTemplate(beginLayout="Specular", collapse=False)
    cmds.editorTemplate("Ks_color", label="Color", addControl=True)
    cmds.editorTemplate("Ks", label="Weight", addControl=True)
    cmds.editorTemplate("specular_brdf", label="Brdf", addControl=True)
    cmds.editorTemplate("specular_roughness", label="Roughness", addControl=True)
    cmds.editorTemplate("specular_anisotropy", label="Anisotropy", addControl=True)
    cmds.editorTemplate("specular_rotation", label="Rotation", addControl=True)        
    # depreciated
    # cmds.editorTemplate("Phong_exponent", label="Glossiness", addControl=True)
    cmds.editorTemplate(addSeparator=True)
    cmds.editorTemplate("specular_Fresnel", aeCallback(checkSpecularFresnel), label="Fresnel", addControl=True)
    cmds.editorTemplate("Ksn", label="Fresnel Coefficient", addControl=True)

    cmds.editorTemplate(beginLayout="Extended Controls", collapse=True)
    cmds.editorTemplate("direct_specular", label="Direct Specularity", addControl=True)
    cmds.editorTemplate("indirect_specular", label="Indirect Specularity", addControl=True)
    cmds.editorTemplate(endLayout=True)
    cmds.editorTemplate(endLayout=True)#End Specular Layout

    cmds.editorTemplate(beginLayout="Reflection", collapse=True)
    cmds.editorTemplate("Kr_color", label="Color", addControl=True)
    cmds.editorTemplate("Kr", label="Weight", addControl=True)
    cmds.editorTemplate(addSeparator=True)
    cmds.editorTemplate("Fresnel", aeCallback(checkReflectionFresnel), label="Fresnel", addControl=True)
    cmds.editorTemplate("Krn", label="Fresnel Coefficient", addControl=True)
    cmds.editorTemplate(addSeparator=True)
    cmds.editorTemplate("reflection_exit_use_environment", label="Reflection Exit Use Environment", addControl=True)
    cmds.editorTemplate("reflection_exit_color", label="Reflection Exit Color", addControl=True)
    cmds.editorTemplate(endLayout=True)

    cmds.editorTemplate(beginLayout="Refraction", collapse=True)
    cmds.editorTemplate("IOR", label="Index of Refraction", addControl=True)
    cmds.editorTemplate("transmittance", label="Transmittance", addControl=True)
    cmds.editorTemplate("Kt_color", label="Color", addControl=True)
    cmds.editorTemplate("Kt", label="Transparency", addControl=True)
    cmds.editorTemplate("refraction_roughness", label="Roughness", addControl=True)
    cmds.editorTemplate(addSeparator=True)
    cmds.editorTemplate("refraction_exit_use_environment", label="Refraction Exit Use Environment", addControl=True)
    cmds.editorTemplate("refraction_exit_color", label="Refraction Exit Color", addControl=True)
    cmds.editorTemplate(endLayout=True)

    cmds.editorTemplate(beginLayout="Bump Mapping", collapse=True)
    cmds.editorTemplate(aeCallback(bumpNew), aeCallback(bumpReplace), "normalCamera", callCustom=True)
    cmds.editorTemplate(endLayout=True)

    cmds.editorTemplate(beginLayout="Subsurface scattering", collapse=True)
    cmds.editorTemplate("Ksss_color", label="Color", addControl=True)
    cmds.editorTemplate("Ksss", label="Weight", addControl=True)
    cmds.editorTemplate("sss_radius", label="Radius", addControl=True)
    cmds.editorTemplate(endLayout=True)

    cmds.editorTemplate(beginLayout="Emission", collapse=True)
    cmds.editorTemplate("emission_color", label="Color", addControl=True)
    cmds.editorTemplate("emission", label="Weight", addControl=True)
    cmds.editorTemplate(endLayout=True)

    cmds.editorTemplate(beginLayout="Caustics", collapse=True)
    cmds.editorTemplate(beginNoOptimize=True)
    cmds.editorTemplate("enable_glossy_caustics", label="Enable Glossy Caustics", addControl=True)
    cmds.editorTemplate("enable_reflective_caustics", label="Enable Reflective Caustics", addControl=True)
    cmds.editorTemplate("enable_refractive_caustics", label="Enable Refractive Caustics", addControl=True)
    cmds.editorTemplate(endNoOptimize=True)
    cmds.editorTemplate(endLayout=True)

    cmds.editorTemplate(beginLayout="Advanced", collapse=True)

    cmds.editorTemplate("bounce_factor", label="Bounce Factor", addControl=True)
    cmds.editorTemplate("opacity", label="Opacity", addControl=True)

    cmds.editorTemplate(endLayout=True)

    cmds.editorTemplate(beginLayout="Hardware Texturing", collapse=True)
    # mel.eval('AEhardwareTextureTemplate "%s"' % nodeName + r'("Kd_color Kd Ks_color Ks")')
    mel.eval('AEhardwareTextureTemplate "%s"' % nodeName + r'("Kd_color Kd Ks_color Ks")')
    cmds.editorTemplate(endLayout=True)

    aovLayout(nodeName)

    # include/call base class/node attributes
    mel.eval('AEdependNodeTemplate "%s"'%nodeName)
    
    cmds.editorTemplate(addExtraControls=True)
    cmds.editorTemplate(endScrollLayout=True)


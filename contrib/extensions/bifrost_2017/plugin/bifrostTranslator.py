import mtoa.ui.ae.templates as templates
from mtoa.ui.ae.templates import AttributeTemplate, ShapeTranslatorTemplate
from mtoa.ui.ae.utils import aeCallback
import maya.cmds as cmds

def bifrostRenderExportShowBlock(controlName, condition):
    # we need to operate on the frame layout which is parent of parent of our parent
    if cmds.control(controlName, exists=True):
        parent1 = cmds.control(controlName, query=True, parent=True)
        parent2 = cmds.control(parent1, query=True, parent=True)
        parentFrameLayout = cmds.control(parent2, query=True, parent=True)
        cmds.control(parentFrameLayout, edit=True, manage=condition)

def Hide(controlName):
    control = controlName.split('.')[-1]
    cmds.paneLayout(control, edit=cmds.paneLayout(control, exists=True), visible=False)

def CheckRenderType( nodeName ):
    # this is to enable disable parts of the AE
    val = cmds.getAttr(nodeName+".bifrostRenderType")
    if val == 0:
        # This is Aero
        bifrostRenderExportShowBlock("AeroExportHideThis", True);
        bifrostRenderExportShowBlock("LiquidExportHideThis", False);
        bifrostRenderExportShowBlock("PointExportHideThis", False);
    if val == 1 or val == 2:
        # This is Liquid
        bifrostRenderExportShowBlock("AeroExportHideThis", False);
        bifrostRenderExportShowBlock("LiquidExportHideThis", True);
        bifrostRenderExportShowBlock("PointExportHideThis", False);
    if val == 3:
        # This is Foam
        bifrostRenderExportShowBlock("AeroExportHideThis", False)
        bifrostRenderExportShowBlock("LiquidExportHideThis", False)
        bifrostRenderExportShowBlock("PointExportHideThis", True)
    bifrostRenderExportShowBlock("HiddenHideThis", False);

def dimControls(nodeName, attrs, dim):
    for attr in attrs:
        cmds.editorTemplate(dimControl=(nodeName, attr, dim))


# ****** AERO AE CALLBACKS ******

def CheckAeroRenderComponents( nodeName ):
    attrs = ("aeroSkip", "splatSamples", "splatMinRadius", "splatMaxRadius", "splatSurfaceAttract", "splatFalloffType",
             "splatFalloffStart", "splatFalloffEnd", "splatDisplacement", "splatNoiseFreq")
    dimControls(nodeName, attrs, cmds.getAttr(nodeName+".aeroRenderData") != 1)

def CheckAeroSmoothing( nodeName ):
    attrs = ("aeroSmoothMode", "aeroSmoothKernelSize", "aeroSmoothIterations", "aeroSmoothChannel", "aeroSmoothRemapMin",
             "aeroSmoothRemapMax", "aeroSmoothRemapInvert")
    dimControls(nodeName, attrs, cmds.getAttr(nodeName+".aeroSmoothWeight") <= 0)

def CheckAeroClipping( nodeName ):
    dimControls(nodeName, ("aeroClipMin", "aeroClipMax"), not cmds.getAttr(nodeName+".aeroClipOn"))

# ****** LIQUID AE CALLBACKS ******

def CheckLiquidRenderComponents( nodeName ):
    attrs = ("implicitResolutionFactor", "implicitDropletRevealFactor", "implicitSurfaceRadius", "implicitDropletRadius",
                "implicitMaxVolumeOfHolesToClose", "doMorphologicalDilation", "doErodeSheetsAndDroplets")
    dimControls(nodeName, attrs, cmds.getAttr(nodeName+".renderData") != 1)

def CheckSmoothWeight( nodeName ):
    attrs = ("smoothMode", "smoothKernelSize", "smoothIterations", "filterBlendingChannel", "smoothRemapRange", "smoothRemapInvert")
    dimControls(nodeName, attrs, cmds.getAttr(nodeName+".smoothWeight") <= 0)

def CheckLiquidClipping( nodeName ):
    clipOff = not cmds.getAttr(nodeName+".liquidClipOn")
    dimControls(nodeName, ("liquidClipMin", "liquidClipMax", "infCubeBlendingOn"), clipOff)

    infBlendingOff = clipOff or not cmds.getAttr(nodeName+".infCubeBlendingOn")
    dimControls(nodeName,
                ("infCubeOutputType", "infCubeTopCenter", "infCubeDim", "blendType", "infCubeBlendRange", "infiniteSurfaceBlendingChannel", "blendingChannelRemapRange", "blendingChannelRemapInvert"),
                infBlendingOff)

def CheckCullSides( nodeName ):
    dimControls(nodeName, ("cullSidesRange", "cullDepthAtStartInVoxels"), not cmds.getAttr(nodeName+".cullSidesOn"))


# ******** FOAM AE CALLBACKS ********

def CheckFoamCameraEnabled( nodeName ):
    dimControls(nodeName,
                ("camRadiusStartDistance", "camRadiusEndDistance", "camRadiusStartFactor", "camRadiusEndFactor", "camRadiusFactorExponent"),
                not cmds.getAttr(nodeName+".camRadiusOn"))

def CheckPointClipping( nodeName ):
    dimControls(nodeName, ("pointClipMin", "pointClipMax"), not cmds.getAttr(nodeName+".pointClipOn"))

def CheckMpSamples( nodeName ):
    dimControls(nodeName,
                ("mpMinRadius", "mpMaxRadius", "mpSurfaceAttract", "mpFalloffType", "mpFalloffStart", "mpFalloffEnd", "mpDisplacementValue", "mpDisplacementNoiseFrequency"),
                cmds.getAttr(nodeName+".mpSamples") <= 1)

class BifrostTemplate(ShapeTranslatorTemplate):
    def setupAero(self):
        # this is for aero
        self.beginLayout("Aero Attributes", collapse=False)
        self.addCustom("AeroExportHideThis", Hide, Hide)

        self.beginLayout("Globals", collapse=False)
        self.addControl("aeroRenderData", label="Render Components", changeCommand=CheckAeroRenderComponents)
        self.addControl("aeroChannel", label="Density Channel")
        self.addControl("aeroChannelScale", label="Channel Scale")
        self.addControl("aeroVelocityScale", label="Velocity Scale")
        self.addControl("aeroSpaceScale", label="Space Scale")
        self.addControl("aeroPrimVars", label="Export Channels")
        self.beginLayout("Particle Splatting", collapse=True)
        self.addControl("aeroSkip")
        self.addControl("splatSamples")
        self.addControl("splatMinRadius")
        self.addControl("splatMaxRadius")
        self.addControl("splatSurfaceAttract")
        self.addControl("splatFalloffType")
        self.addControl("splatFalloffStart")
        self.addControl("splatFalloffEnd")
        self.addControl("splatDisplacement")
        self.addControl("splatNoiseFreq")
        self.endLayout() # Splat Particles to Voxels
        self.endLayout() # Globals

        self.beginLayout("Smoothing", collapse=False)
        self.addControl("aeroSmoothWeight", "Smoothing", changeCommand=CheckAeroSmoothing)
        self.addControl("aeroSmoothMode", "Mode")
        self.addControl("aeroSmoothKernelSize", "Kernel Size (voxels)")
        self.addControl("aeroSmoothIterations", "Iterations")

        self.beginLayout("Blend Raw And Smooth", collapse=True)
        self.addControl("aeroSmoothChannel", label="Smooth Channel")
        self.addControl("aeroSmoothRemapMin")
        self.addControl("aeroSmoothRemapMax")
        self.addControl("aeroSmoothRemapInvert", label="Invert")
        self.endLayout() # Blend Raw And Smooth
        self.endLayout() # Smoothing

        self.beginLayout("Clipping", collapse=True)
        self.addControl("aeroClipOn", label="Enable", changeCommand=CheckAeroClipping)
        self.addControl("aeroClipMin", label="Min")
        self.addControl("aeroClipMax", label="Max")
        self.endLayout() # Clipping

        self.endLayout() # Aero Attributes

    def setupLiquid(self):
        # this is for liquid
        self.beginLayout("Liquid Attributes", collapse=False)
        self.addCustom("LiquidExportHideThis", Hide, Hide)

        self.beginLayout("Globals", collapse=False)
        self.addControl("renderData", label="Render Components", changeCommand=CheckLiquidRenderComponents)
        self.addControl("distanceChannel")
        self.addControl("liquidPrimVars", label="Export Channels")
        self.addControl("exportUVs", label="Export UVs");
        self.addControl("liquidVelocityScale", label="Velocity Scale")
        self.addControl("liquidSpaceScale", label="Space Scale") # why????
        self.addControl("tesselation", label="Subdivisions (per voxel)")
        self.beginLayout("Particles Rendering", collapse=True)
        self.addControl("implicitResolutionFactor", label="Resolution Factor")
        self.addControl("implicitDropletRevealFactor", label="Droplet Reveal Factor")
        self.addControl("implicitSurfaceRadius", label="Surface Radius")
        self.addControl("implicitDropletRadius", label="Droplet Radius")
        self.addControl("implicitMaxVolumeOfHolesToClose", label="Max Volume of Holes To Close")
        self.addControl("doMorphologicalDilation")
        self.addControl("doErodeSheetsAndDroplets")
        self.endLayout() # Particles
        self.endLayout() # Globals

        self.beginLayout("Filtering", collapse=False)
        self.addControl("dilateAmount", label="Dilate")
        self.addControl("erodeAmount", label="Erode")
        self.addSeparator()
        self.addControl("smoothWeight", label="Smoothing", changeCommand=CheckSmoothWeight)
        self.addControl("smoothMode", label="Mode")
        self.addControl("smoothKernelSize", label="Kernel Size (voxels)")
        self.addControl("smoothIterations", label="Iterations")
        self.beginLayout("Blend Raw And Smooth Liquid", collapse=True)
        self.addControl("filterBlendingChannel")
        self.addControl("smoothRemapRange")
        self.addControl("smoothRemapInvert", label="Invert")
        self.endLayout() # BlendRawAndSmoothLiquid
        self.endLayout() # Filtering

        self.beginLayout("Clip And Extend", collapse=True)
        self.addControl("liquidClipOn", label="Enable", changeCommand=CheckLiquidClipping)
        self.addControl("liquidClipMin", label="Min")
        self.addControl("liquidClipMax", label="Max")
        self.beginLayout("Infinite Surface Blending", collapse=True)
        self.addControl("infCubeBlendingOn", label="Enable", changeCommand=CheckLiquidClipping)
        self.addControl("infCubeOutputType", label="Output Type")
        self.addControl("infCubeTopCenter", label="Top Center")
        self.addControl("infCubeDim", label="Dimension")
        self.addControl("blendType")
        self.addControl("infCubeBlendRange", label="Blend Range")
        self.addSeparator()
        self.addControl("infiniteSurfaceBlendingChannel", label="Blend Using Channel")
        self.addControl("blendingChannelRemapRange", label="Remap Range")
        self.addControl("blendingChannelRemapInvert", label="Invert")
        self.endLayout() # Infinite Blending
        self.endLayout() # LiquidClipping

        self.beginLayout("Cull Sides", collapse=True)
        self.addControl("cullSidesOn", label="Enable", changeCommand=CheckCullSides)
        self.addControl("cullSidesRange", label="Range")
        self.addControl("cullDepthAtStartInVoxels", label="Depth At Start (voxels)")
        self.endLayout() # CullSides

        self.endLayout() # Liquid Attributes

        self.suppress("renderMethod") # only meshing available for now
        self.suppress("narrowBandThicknessInVoxels")
        self.suppress("liquidStepSize")
        self.suppress("samples")

    def setupFoam(self):
        # this is for foam
        self.beginLayout("Foam Attributes", collapse=False)
        self.addCustom("PointExportHideThis", Hide, Hide)

        self.beginLayout("Globals", collapse=False)
        self.addControl("renderPrimitiveType", label="Primitive Type")
        self.addControl("exportNormalAsPrimvar", label="Export Normals")
        self.addControl("pointVelocityScale", label="Velocity Scale")
        self.addControl("pointSpaceScale", label="Space Scale")
        self.addControl("pointPrimVars", label="Export Channels")

        self.beginLayout("Render Channel", collapse=False)
        self.addControl("pointChannel")
        self.addControl("pointChannelScale", label="Channel Scale")
        self.endLayout() # Render Channel
        self.endLayout() # Globals

        self.beginLayout("Radius", collapse=False)
        self.addControl("pointRadius", label="Radius")
        self.addControl("stepSize")
        self.addControl("useChannelToModulateRadius")

        self.beginLayout("Camera Dependent Radius", collapse=True)
        self.addControl("camRadiusOn", label="Enable", changeCommand=CheckFoamCameraEnabled)
        self.addControl("camRadiusStartDistance")
        self.addControl("camRadiusEndDistance")
        self.addControl("camRadiusStartFactor")
        self.addControl("camRadiusEndFactor")
        self.addControl("camRadiusFactorExponent")
        self.endLayout() # Camera Dependent Radius
        self.endLayout() # Radius

        self.beginLayout("Clipping", collapse=True)
        self.addControl("pointClipOn", label="Enable", changeCommand=CheckPointClipping)
        self.addControl("pointClipMin", label="Min")
        self.addControl("pointClipMax", label="Max")
        self.endLayout() # Clipping

        self.beginLayout("Multi Pointing", collapse=True)
        self.addControl("mpSamples", changeCommand=CheckMpSamples)
        self.addControl("mpMinRadius")
        self.addControl("mpMaxRadius")
        self.addControl("mpSurfaceAttract")
        self.addControl("mpFalloffType")
        self.addControl("mpFalloffStart")
        self.addControl("mpFalloffEnd")
        self.addControl("mpDisplacementValue")
        self.addControl("mpDisplacementNoiseFrequency")
        self.endLayout() # Multi Pointing

        self.beginLayout("Other", collapse=True)
        self.addControl("pointSkip")
        self.addControl("chunkSize")
        self.addControl("useChannelGradientAsNormal")
        self.endLayout()

        self.endLayout() # Foam Attributes

    def setup(self):
        # FIXME: Suppress "bifrostRenderType" control
        self.setupAero()
        self.setupLiquid()
        self.setupFoam()

        self.beginLayout("Hidden")
        self.addCustom("HiddenHideThis", Hide, Hide)
        self.addControl("bifrostRenderType", changeCommand=CheckRenderType)
        self.endLayout()

        self.suppress("aiDebug")
        self.suppress("debug")
        self.suppress("silent")
        self.suppress("aiUserOptions")
        self.suppress("mesherAlgo")
        self.suppress("liquidDebug")

templates.registerAETemplate(BifrostTemplate, 'bifrostShape')

class BifrostAeroMaterialTemplate(AttributeTemplate):
    def setup(self):
        self.addControl("aiStepSize", label="Step Size")
        self.addControl("aiMaxSteps", label="Max Steps")
        self.addControl("aiShadowing", label="Shadowing")
        self.addControl("aiShadowingStepSize", label="Shadowing Step Size")
        self.addControl("aiShadowingMaxSteps", label="Shadowing Max Steps")

templates.registerAETemplate(BifrostAeroMaterialTemplate, 'bifrostAeroMaterial')


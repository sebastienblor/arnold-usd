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


# ****** LIQUID AE CALLBACKS ******

def CheckLiquidRenderComponents( nodeName ):
    usingVoxels = cmds.getAttr(nodeName+".renderData") != 1
    for attr in ("implicitResolutionFactor", "implicitDropletRevealFactor", "implicitSurfaceRadius", "implicitDropletRadius",
                 "implicitMaxVolumeOfHolesToClose", "doMorphologicalDilation", "doErodeSheetsAndDroplets"):
        cmds.editorTemplate(dimControl=(nodeName, attr, usingVoxels))

def CheckSmoothWeight( nodeName ):
    smoothOff = cmds.getAttr(nodeName+".smoothWeight") <= 0
    for attr in ("smoothMode", "smoothKernelSize", "smoothIterations", "filterBlendingChannel", "smoothRemapRange", "smoothRemapInvert"):
        cmds.editorTemplate(dimControl=(nodeName, attr, smoothOff))

def CheckLiquidClipping( nodeName ):
    clipOff = not cmds.getAttr(nodeName+".liquidClipOn")
    for attr in ("liquidClipMin", "liquidClipMax", "infCubeBlendingOn"):
        cmds.editorTemplate(dimControl=(nodeName, attr, clipOff))

    infBlendingOff = clipOff or not cmds.getAttr(nodeName+".infCubeBlendingOn")
    for attr in ("infCubeOutputType", "infCubeTopCenter", "infCubeDim", "blendType", "infCubeBlendRange",
                 "infiniteSurfaceBlendingChannel", "blendingChannelRemapRange", "blendingChannelRemapInvert"):
        cmds.editorTemplate(dimControl=(nodeName, attr, infBlendingOff))

def CheckCullSides( nodeName ):
    cullOff = not cmds.getAttr(nodeName+".cullSidesOn")
    for attr in ("cullSidesRange", "cullDepthAtStartInVoxels"):
        cmds.editorTemplate(dimControl=(nodeName, attr, cullOff))


class BifrostTemplate(ShapeTranslatorTemplate):
    def setupAero(self):
        # this is for aero
        self.beginLayout("Aero Attributes", collapse=False)
        self.addCustom("AeroExportHideThis", Hide, Hide)

        self.beginLayout("Globals", collapse=False)
        self.addControl("aeroRenderData", label="Render Components")
        self.addControl("aeroChannel", label="Density Channel")
        self.addControl("aeroChannelScale", label="Channel Scale")
        self.addControl("aeroVelocityScale", label="Velocity Scale")
        self.addControl("aeroSpaceScale", label="Space Scale")
        self.addControl("aeroPrimVars", label="Export Channels")
        self.endLayout() # Globals

        self.beginLayout("Smoothing", collapse=False)
        self.addControl("aeroSmoothMode", "Mode")
        self.addControl("aeroSmoothKernelSize", "Smoothing")
        self.addControl("aeroSmoothWeight", "Weight")
        self.addControl("aeroSmoothIterations", "Iterations")

        self.beginLayout("Blend Raw And Smooth", collapse=True)
        self.addControl("aeroSmoothChannel", label="Smooth Channel")
        self.addControl("aeroSmoothRemapMin")
        self.addControl("aeroSmoothRemapMax")
        self.addControl("aeroSmoothRemapInvert", label="Invert")
        self.endLayout() # Blend Raw And Smooth
        self.endLayout() # Smoothing

        self.beginLayout("Clipping", collapse=True)
        self.addControl("aeroClipOn", label="Enable")
        self.addControl("aeroClipMin", label="Min")
        self.addControl("aeroClipMax", label="Max")
        self.endLayout() # Clipping

        self.beginLayout("Advanced", collapse=True)
        self.beginLayout("Particle Splatting", collapse=False)
        self.addControl("splatResolutionFactor")
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
        self.endLayout() # Advanced
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
        self.addControl("camRadiusOn")
        self.addControl("camRadiusStartDistance")
        self.addControl("camRadiusEndDistance")
        self.addControl("camRadiusStartFactor")
        self.addControl("camRadiusEndFactor")
        self.addControl("camRadiusFactorExponent")
        self.endLayout() # Camera Dependent Radius
        self.endLayout() # Radius

        self.beginLayout("Clipping", collapse=True)
        self.addControl("pointClipOn", label="Enable")
        self.addControl("pointClipMin", label="Min")
        self.addControl("pointClipMax", label="Max")
        self.endLayout() # Clipping

        self.beginLayout("Multi Pointing", collapse=True)
        self.addControl("mpSamples")
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
        self.addControl("exportNormalAsPrimvar")
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


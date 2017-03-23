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
    cmds.paneLayout(control, visible=False)

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

class BifrostTemplate(ShapeTranslatorTemplate):
    def setupAero(self):
        # this is for aero
        self.beginLayout("Aero Attributes", collapse=False)
        self.addCustom("AeroExportHideThis", Hide, Hide)

        self.beginLayout("Globals", collapse=False)
        self.addControl("aeroRenderData", label="Components")
        self.addControl("aeroChannel")
        self.addControl("aeroChannelScale")
        self.addControl("aeroVelocityScale")
        self.addControl("aeroSpaceScale")
        self.addControl("aeroPrimVars")
        self.endLayout() # Globals

        self.beginLayout("Smoothing", collapse=False)
        self.addControl("aeroSmoothMode", "Mode")
        self.addControl("aeroSmoothWeight", "Weight")
        self.addControl("aeroSmoothIterations", "Iterations")

        self.beginLayout("Blend Raw And Smooth", collapse=False)
        self.addControl("aeroSmoothChannel")
        self.addControl("aeroSmoothRemapMin")
        self.addControl("aeroSmoothRemapMax")
        self.addControl("aeroSmoothRemapInvert")
        self.endLayout() # Blend Raw And Smooth
        self.endLayout() # Smoothing

        self.beginLayout("Clipping", collapse=True)
        self.addControl("aeroClipOn", label="Enable")
        self.addControl("aeroClipMin", label="Min")
        self.addControl("aeroClipMax", label="Max")
        self.endLayout() # Clipping

        self.beginLayout("Advanced", collapse=True)
        self.beginLayout("Particle Splatting", collapse=True)
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
        self.addControl("renderMethod")
        self.addControl("renderData", label="Components")
        self.addControl("distanceChannel")
        self.addControl("liquidPrimVars", label="Export Channels")
        self.addControl("liquidVelocityScale", label="Velocity Scale")
        self.addControl("liquidSpaceScale", label="Space Scale") # why????
        self.endLayout() # LiquidGlobals

        self.beginLayout("Filtering", collapse=False)
        self.addControl("dilateAmount", label="Dilate")
        self.addControl("smoothWeight", label="Smooth")
        self.addControl("erodeAmount", label="Erode")

        self.beginLayout("Smoothing", collapse=True)
        self.addControl("smoothMode", label="Mode")
        self.addControl("smoothAmount", label="Amount")
        self.addControl("smoothIterations", label="Iterations")
        self.endLayout() # Smoothing

        self.beginLayout("Blend Raw And Smooth Liquid", collapse=True)
        self.addControl("filterBlendingChannel")
        self.addControl("smoothRemapRange")
        self.addControl("smoothRemapInvert", label="Invert")
        self.endLayout() # BlendRawAndSmoothLiquid
        self.endLayout() # Filtering

        self.beginLayout("Clipping", collapse=True)
        self.addControl("liquidClipOn", label="Enable")
        self.addControl("liquidClipMin", label="Min")
        self.addControl("liquidClipMax", label="Max")
        self.endLayout() # LiquidClipping

        self.beginLayout("Cull Sides", collapse=True)
        self.addControl("cullSidesOn", label="Enable")
        self.addControl("cullSidesRange", label="Range")
        self.addControl("cullDepthAtStartInVoxels", label="Depth At Start In Voxels")
        self.endLayout() # CullSides

        self.beginLayout("ExtendSurface", collapse=True)
        self.addControl("infCubeBlendingOn")
        self.addControl("infCubeOutputType")
        self.addControl("simWaterLevel")
        self.addControl("infCubeTopCenter")
        self.addControl("infCubeDim")
        self.addControl("blendType")
        self.addControl("infCubeBlendRange")

        self.beginLayout("BlendUsingChannel", collapse=False)
        self.addControl("infiniteSurfaceBlendingChannel")
        self.addControl("blendingChannelRemapRange")
        self.addControl("blendingChannelRemapInvert", label="Invert")
        self.endLayout() # BlendUsingChannel
        self.endLayout() # ExtendSurface

        self.beginLayout("Advanced", collapse=True)
        self.beginLayout("Implicit Controls", collapse=True)
        self.addControl("narrowBandThicknessInVoxels", label="Narow Band Thickness")
        self.addControl("liquidStepSize", label="Step size")
        self.endLayout() # Implicit
        self.beginLayout("Mesh Controls", collapse=True)
        self.addControl("sampleRate", label="Tesselation")
        self.endLayout() # Mesh
        self.beginLayout("Particle To Voxels Controls", collapse=True)
        self.addControl("implicitResolutionFactor", label="Resolution Factor")
        self.addControl("implicitDropletRevealFactor", label="Droplet Reveal Factor")
        self.addControl("implicitSurfaceRadius", label="Surface Radius")
        self.addControl("implicitDropletRadius", label="Droplet Radius")
        self.addControl("implicitMaxVolumeOfHolesToClose", label="Max Volume of Holes To Close")
        self.addControl("doMorphologicalDilation")
        self.addControl("doErodeSheetsAndDroplets")
        self.endLayout() # Particles
        self.endLayout() # Advanced
        self.endLayout() # LiquidExportAttributes

    def setupFoam(self):
        # this is for foam
        self.beginLayout("Foam Attributes", collapse=False)
        self.addCustom("PointExportHideThis", Hide, Hide)

        self.beginLayout("Globals", collapse=False)
        self.addControl("renderPrimitiveType")
        self.addControl("pointSkip")
        self.addControl("chunkSize")
        self.addControl("pointVelocityScale")
        self.addControl("pointSpaceScale")
        self.addControl("pointPrimVars")

        self.beginLayout("Render Channel", collapse=False)
        self.addControl("pointChannel")
        self.addControl("pointChannelScale")
        self.addControl("useChannelGradientAsNormal")
        self.addControl("exportNormalAsPrimvar")
        self.endLayout() # Render Channel
        self.endLayout() # Globals

        self.beginLayout("Radius", collapse=False)
        self.addControl("pointRadius")
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
        self.endLayout() # Foam Attributes

    def setup(self):
        # FIXME: Suppress "bifrostRenderType" control
        self.setupAero()
        self.setupLiquid()
        self.setupFoam()

        self.beginLayout("Hidden")
        self.addCustom("HiddenHideThis", Hide, Hide)
        self.addControl("bifrostRenderType", changeCommand=CheckRenderType);
        self.endLayout()

        self.suppress("aeroSilent")
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


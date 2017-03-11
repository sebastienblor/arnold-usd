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

class BifrostTemplate(ShapeTranslatorTemplate):
    def setup(self):
        self.addControl("bifrostRenderType", changeCommand=CheckRenderType);
        # FIXME: Suppress "bifrostRenderType" control

        # this is for aero
        self.beginLayout("AeroExportAttributes", collapse=False)
        self.addCustom("AeroExportHideThis", Hide, Hide)

        self.beginLayout("AeroGlobals", collapse=False)
        self.addControl("aeroRenderData")
        self.addControl("aeroChannel")
        self.addControl("aeroChannelScale")
        self.addControl("aeroVelocityScale")
        self.addControl("aeroSpaceScale")
        self.addControl("aeroPrimVars")
        self.endLayout()

        self.beginLayout("AeroClipping", collapse=True)
        self.addControl("aeroClipOn")
        self.addControl("aeroClipMin")
        self.addControl("aeroClipMax")
        self.endLayout()

        self.beginLayout("Smooth", collapse=False)
        self.addControl("aeroSmoothOn")
        self.addControl("aeroSmoothMode")
        self.addControl("aeroSmoothWeight")
        self.addControl("aeroSmoothIterations")

        self.beginLayout("BlendRawAndSmooth", collapse=False)
        self.addControl("aeroSmoothChannel")
        self.addControl("aeroSmoothRemapMin")
        self.addControl("aeroSmoothRemapMax")
        self.addControl("aeroSmoothRemapInvert")
        self.endLayout()
        self.endLayout()

        self.beginLayout("SplatParticlesToVoxels", collapse=True)
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
        self.endLayout()
        self.endLayout()

        # this is for liquid
        self.beginLayout("LiquidExportAttributes", collapse=False)
        self.addCustom("LiquidExportHideThis", Hide, Hide)

        self.beginLayout("LiquidGlobals", collapse=False)
        self.addControl("renderMethod")
        self.addControl("liquidVelocityScale")
        self.addControl("liquidSpaceScale")
        self.addControl("liquidPrimVars")
        self.endLayout()

        self.beginLayout("Implicit", collapse=False)
        self.addCustom("ImplicitExportHideThis", Hide, Hide)

        self.addControl("renderData")
        self.addControl("distanceChannel")
        self.addControl("narrowBandThicknessInVoxels")
        self.addControl("liquidStepSize")
        self.addControl("liquidRaySamples")

        self.beginLayout("CullSides", collapse=False)
        self.addControl("cullSidesOn")
        self.addControl("cullSidesStart")
        self.addControl("cullSidesEnd")
        self.addControl("cullDepthAtStartInVoxels")
        self.endLayout()

        self.beginLayout("Filtering", collapse=False)
        self.addControl("dilateAmount")
        self.addControl("erodeAmount")

        self.beginLayout("Smoothing", collapse=False)
        self.addControl("smoothOn")
        self.addControl("smoothMode")
        self.addControl("smoothAmount")
        self.addControl("smoothIterations")
        self.addControl("smoothWeight")
        self.endLayout()

        self.beginLayout("BlendRawAndSmoothLiquid", collapse=False)
        self.addControl("filterBlendingChannel")
        self.addControl("smoothRemapMin")
        self.addControl("smoothRemapMax")
        self.addControl("smoothRemapInvert")
        self.endLayout()
        self.endLayout()

        self.beginLayout("LiquidClipping", collapse=True)
        self.addControl("liquidClipOn")
        self.addControl("liquidClipMin")
        self.addControl("liquidClipMax")
        self.endLayout()

        self.beginLayout("ExtendSurface", collapse=True)
        self.addControl("infCubeBlendingOn")
        self.addControl("infCubeOutputType")
        self.addControl("simWaterLevel")
        self.addControl("infCubeTopCenter")
        self.addControl("infCubeDim")
        self.addControl("blendType")
        self.addControl("infCubeBlendStart")
        self.addControl("infCubeBlendEnd")

        self.beginLayout("BlendUsingChannel", collapse=False)
        self.addControl("infiniteSurfaceBlendingChannel")
        self.addControl("blendingChannelRemapMin")
        self.addControl("blendingChannelRemapMax")
        self.addControl("blendingChannelRemapInvert")
        self.endLayout()
        self.endLayout()

        self.beginLayout("VoxelizeParticles", collapse=True)
        self.addControl("implicitResolutionFactor")
        self.addControl("implicitDropletRevealFactor")
        self.addControl("implicitSurfaceRadius")
        self.addControl("implicitDropletRadius")
        self.addControl("implicitMaxVolumeOfHolesToClose")
        self.addControl("doMorphologicalDilation")
        self.addControl("doErodeSheetsAndDroplets")
        self.endLayout()
        self.endLayout()
        self.endLayout()

        # this is for foam
        self.beginLayout("PointExportAttributes", collapse=False)
        self.addCustom("PointExportHideThis", Hide, Hide)

        self.beginLayout("PointGlobals", collapse=False)
        self.addControl("renderPrimitiveType")
        self.addControl("pointSkip")
        self.addControl("chunkSize")
        self.addControl("pointVelocityScale")
        self.addControl("pointSpaceScale")
        self.addControl("pointPrimVars")

        self.beginLayout("RenderChannel", collapse=False)
        self.addControl("pointChannel")
        self.addControl("pointChannelScale")
        self.addControl("useChannelGradientAsNormal")
        self.addControl("exportNormalAsPrimvar")
        self.endLayout()
        self.endLayout()

        self.beginLayout("Radius", collapse=False)
        self.addControl("pointRadius")
        self.addControl("useChannelToModulateRadius")

        self.beginLayout("CameraDependentRadius", collapse=True)
        self.addControl("camRadiusOn")
        self.addControl("camRadiusStartDistance")
        self.addControl("camRadiusEndDistance")
        self.addControl("camRadiusStartFactor")
        self.addControl("camRadiusEndFactor")
        self.addControl("camRadiusFactorExponent")
        self.endLayout()
        self.endLayout()

        self.beginLayout("PointClipping", collapse=True)
        self.addControl("pointClipOn")
        self.addControl("pointClipMin")
        self.addControl("pointClipMax")
        self.endLayout()

        self.beginLayout("MultiPointing", collapse=True)
        self.addControl("mpSamples")
        self.addControl("mpMinRadius")
        self.addControl("mpMaxRadius")
        self.addControl("mpSurfaceAttract")
        self.addControl("mpFalloffType")
        self.addControl("mpFalloffStart")
        self.addControl("mpFalloffEnd")
        self.addControl("mpDisplacementValue")
        self.addControl("mpDisplacementNoiseFrequency")
        self.endLayout()
        self.endLayout()

        self.suppress("aeroSilent")
        self.suppress("aiDebug")
        self.suppress("aiUserOptions")
        self.suppress("mesherAlgo")
        self.suppress("hot")
        self.suppress("liquidDebug")
        self.suppress("sampleRate")

templates.registerAETemplate(BifrostTemplate, 'bifrostShape')

class BifrostAeroMaterialTemplate(AttributeTemplate):
    def setup(self):
        self.addControl("aiStepSize", label="Step Size")
        self.addControl("aiMaxSteps", label="Max Steps")
        self.addControl("aiShadowing", label="Shadowing")
        self.addControl("aiShadowingStepSize", label="Shadowing Step Size")
        self.addControl("aiShadowingMaxSteps", label="Shadowing Max Steps")

templates.registerAETemplate(BifrostAeroMaterialTemplate, 'bifrostAeroMaterial')


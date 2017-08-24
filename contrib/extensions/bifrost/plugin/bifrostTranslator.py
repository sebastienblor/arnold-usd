import mtoa.ui.ae.templates as templates
from mtoa.ui.ae.templates import AttributeTemplate, ShapeTranslatorTemplate
from mtoa.ui.ae.utils import aeCallback
import maya.cmds as cmds
import maya.mel as mel
from functools import partial

def dimControls(nodeName, attrs, dim):
    for attr in attrs:
        cmds.editorTemplate(dimControl=(nodeName, attr, dim))

maya_api_version = cmds.about(apiVersion=True)
if maya_api_version > 20180100 or maya_api_version >= 20180000 and "Preview Release" in str(cmds.about(version=True)):
    def CheckSurfaceType( nodeName ):
        mesh = (cmds.getAttr(nodeName+".surfaceType") == 0)
        dimControls(nodeName, ("subdivisions", "smoothing"), not mesh)
        dimControls(nodeName, ("implicitStepSize", "implicitSamples"), mesh)

    def CheckRenderAs( nodeName ):
        surfaceControls = ("surfaceType", "subdivisions", "smoothing", "implicitStepSize", "implicitSamples")
        pointsControls = ("pointsType", "pointsStepSize", "chunkSize")
        volumeControls = ("volumeStepSize",)
        renderAs = cmds.getAttr(nodeName+".renderAs")
        if renderAs == 0:
            dimControls(nodeName, volumeControls , True)
            dimControls(nodeName, pointsControls , True)
            dimControls(nodeName, surfaceControls, False)
            CheckSurfaceType( nodeName )
        elif renderAs == 1:
            dimControls(nodeName, volumeControls , True)
            dimControls(nodeName, surfaceControls, True)
            dimControls(nodeName, pointsControls , False)
        else:
            dimControls(nodeName, surfaceControls, True)
            dimControls(nodeName, pointsControls , True)
            dimControls(nodeName, volumeControls , False)

    class BifrostTemplate(ShapeTranslatorTemplate):
        def setup(self):
            #self.addControl("renderAs", changeCommand=CheckRenderAs)
            self.addControl("opaque")

            self.beginLayout("Surface Controls", collapse=False)
            self.addControl("surfaceType", changeCommand=CheckSurfaceType)
            self.addSeparator()
            self.addControl("subdivisions", label="Mesh Subdivisions")
            self.addControl("smoothing", label="Mesh Smoothing")
            self.addSeparator()
            self.addControl("implicitStepSize", label="Implicit Step Size")
            self.addControl("implicitSamples", label="Implicit Samples")
            self.endLayout()

            self.beginLayout("Points Controls", collapse=False)
            self.addControl("pointsType", label="Type")
            self.addControl("pointsStepSize", label="Point Step Size")
            self.addControl("chunkSize", label="Chunk Size");
            self.endLayout()

            self.beginLayout("Volume Controls", collapse=False)
            self.addControl("volumeStepSize", label="Volume Step Size")
            self.endLayout()

            self.suppress("aiDebug")
            self.suppress("debug")
            self.suppress("aiUserOptions")
            self.suppress("matte")
            self.suppress("tileMode")

    templates.registerAETemplate(BifrostTemplate, 'bifrostShape')






else: # DIRTY BACKWARD COMPATIBILITY STUFF.... (For Maya 2018, Maya 2018 Update 1)

    def UseSelected(attr, b):
        sel = cmds.ls(sl=True) or []
        if len(sel) == 0:
            cmds.warning("no selection")
            return

        mesh = sel[0]
        if not cmds.objectType(mesh, isAType="mesh"):
            shapes = cmds.listRelatives(mesh, noIntermediate=True, shapes=True, type="mesh") or []
            if len(shapes) == 1:
                mesh = shapes[0]
            else:
                cmds.warning("invalid mesh")
                return
        cmds.connectAttr(mesh + ".worldMesh", attr, force=True)

    def InputMeshNew(label, clipInputMeshAttr):
        control = clipInputMeshAttr.split('.')[-1]
        clipInputMeshFormLayout = (control + "FormLayout")
        clipInputMeshField = (control + "Field")
        clipInputMeshUseSelected = (control + "UseSelected")

        cmds.formLayout(clipInputMeshFormLayout)
        cmds.attrNavigationControlGrp(clipInputMeshField, label=label, attribute=clipInputMeshAttr, cw3=(123, 0, 0), enable=False)
        cmds.button(clipInputMeshUseSelected, label="Use Selected", command=partial(UseSelected,clipInputMeshAttr), height=20)

        cmds.formLayout(clipInputMeshFormLayout, edit=True,
            attachForm=[(clipInputMeshField, "left", 8),
                        (clipInputMeshField, "top", 0),
                        (clipInputMeshUseSelected, "right", 40)],
            attachNone=[(clipInputMeshField, "bottom"),
                        (clipInputMeshUseSelected, "left"),
                        (clipInputMeshUseSelected, "bottom")],
            attachControl=[(clipInputMeshField, "right", 0, clipInputMeshUseSelected)])
        cmds.setParent('..')

    def InputMeshReplace(clipInputMeshAttr):
        pass

    def SurfaceControls():
        return ("surfaceType",
                "renderComponent",
                "distanceChannel",
                "levelsetDropletRevealFactor",
                "levelsetSurfaceRadius",
                "levelsetDropletRadius",
                "levelsetResolutionFactor",
                "levelsetMaxVolumeOfHolesToClose",
                "dilate",
                "smooth",
                "smoothMode",
                "smoothIterations",
                "erode",
                "enableOceanBlending",
                "oceanPlane",
                "oceanBlendingRadius",
                "oceanBlendingOffsets",
                "oceanBlendingOutChannel",
                "subdivisions",
                "smoothing",
                "implicitStepSize",
                "implicitSamples",
                "exportLaplacian",
                "exportCurvature" )

    def PointsControls():
        return ("pointsType",
                "radius",
                "pointsStepSize",
                "enableRadiusChannel",
                "radiusChannel",
                "chunkSize")

    def VolumeControls():
        return ("densityChannel",
                "volumeStepSize",
                "volumeSmooth",
                "volumeSmoothIterations")

    def CheckRenderAs( nodeName ):
        renderAs = cmds.getAttr(nodeName+".renderAs")
        if renderAs == 0:
            dimControls(nodeName, VolumeControls(), True)
            dimControls(nodeName, PointsControls(), True)
            dimControls(nodeName, SurfaceControls(), False)
            CheckRenderComponents( nodeName )
        elif renderAs == 1:
            dimControls(nodeName, VolumeControls(), True)
            dimControls(nodeName, SurfaceControls(), True)
            dimControls(nodeName, PointsControls(), False)
        else:
            dimControls(nodeName, SurfaceControls(), True)
            dimControls(nodeName, PointsControls(), True)
            dimControls(nodeName, VolumeControls(), False)

    def CheckRenderComponents( nodeName ):
        voxels = (cmds.getAttr(nodeName+".renderComponent") == 0) or (cmds.getAttr(nodeName+".renderAs") != 0)
        dimControls(nodeName, ("levelsetDropletRevealFactor", "levelsetSurfaceRadius", "levelsetDropletRadius", "levelsetResolutionFactor", "levelsetMaxVolumeOfHolesToClose"), voxels)

    class BifrostTemplate(ShapeTranslatorTemplate):
        def setup(self):
            self.addControl("opaque")
            self.addControl("matte")
            self.addControl("tileMode")
            self.addSeparator()

            self.addControl("renderAs", changeCommand=CheckRenderAs)
            self.addSeparator()
            self.addControl("velocityScale")
            self.addControl("spaceScale")
            self.addControl("channels")

            self.beginLayout("Clipping", collapse=True)
            self.addControl("clip", label="Enable")
            self.addCustom("clipBox", partial(InputMeshNew,"Bounding Mesh"), InputMeshReplace)
            self.endLayout()

            self.beginLayout("Surface Controls", collapse=False)
            self.addControl("surfaceType")
            self.addControl("renderComponent", changeCommand=CheckRenderComponents)
            self.addSeparator()
            self.addControl("levelsetDropletRevealFactor", label="Droplet Reveal Factor")
            self.addControl("levelsetSurfaceRadius", label="Surface Radius")
            self.addControl("levelsetDropletRadius", label="Droplet Radius")
            self.addControl("levelsetResolutionFactor", label="Resolution Factor")
            self.addControl("levelsetMaxVolumeOfHolesToClose", label="Max Volume Of Holes To Close")

            self.beginLayout("Filtering", collapse=True)
            self.addControl("dilate")
            self.addSeparator()
            self.addControl("smooth")
            self.addControl("smoothMode")
            self.addControl("smoothIterations")
            self.addSeparator()
            self.addControl("erode")
            self.endLayout()

            self.beginLayout("Ocean Blending", collapse=True)
            self.addControl("enableOceanBlending", label="Enable")
            self.addCustom("oceanPlane", partial(InputMeshNew,"Mesh Plane"), InputMeshReplace)
            self.addControl("oceanBlendingRadius", label="Boundary Radius")
            self.addControl("oceanBlendingOffsets", label="Offsets")
            self.addControl("oceanBlendingOutChannel", label="Output Channel")
            self.endLayout()

            self.beginLayout("Mesh Controls", collapse=True)
            self.addControl("subdivisions")
            self.addControl("smoothing")
            self.endLayout()
            self.beginLayout("Implicit Controls", collapse=True)
            self.addControl("implicitStepSize", label="Step Size")
            self.addControl("implicitSamples", label="Samples")
            self.endLayout()
            self.beginLayout("Advanced", collapse=True)
            self.addControl("exportLaplacian")
            self.addControl("exportCurvature")
            self.endLayout()
            self.endLayout()

            self.beginLayout("Points Controls", collapse=False)
            self.addControl("pointsType", label="Type")
            self.addControl("radius")
            self.addSeparator()
            self.addControl("enableRadiusChannel")
            self.addControl("radiusChannel")
            self.addSeparator()
            self.addControl("pointsStepSize", label="Step Size")
            self.addControl("chunkSize");
            self.endLayout()

            self.beginLayout("Volume Controls", collapse=False)
            self.addControl("volumeStepSize", label="Step Size")
            self.addSeparator()
            self.addControl("volumeSmooth", label="Smooth")
            self.addControl("volumeSmoothIterations", label="Smooth Iterations")
            self.endLayout()

            self.suppress("aiDebug")
            self.suppress("debug")
            self.suppress("aiUserOptions")
            self.suppress("densityChannel")
            self.suppress("distanceChannel")

    templates.registerAETemplate(BifrostTemplate, 'bifrostShape')

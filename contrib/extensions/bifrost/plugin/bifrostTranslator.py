import mtoa.ui.ae.templates as templates
from mtoa.ui.ae.templates import AttributeTemplate, ShapeTranslatorTemplate
from mtoa.ui.ae.utils import aeCallback
import maya.cmds as cmds
import maya.mel as mel
from functools import partial

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

def dimControls(nodeName, attrs, dim):
    for attr in attrs:
        cmds.editorTemplate(dimControl=(nodeName, attr, dim))

def SurfaceControls():
    return ("surface_type",
            "render_component",
            "distance_channel",
            "levelset_droplet_reveal_factor",
            "levelset_surface_radius",
            "levelset_droplet_radius",
            "levelset_resolution_factor",
            "levelset_max_volume_of_holes_to_close",
            "dilate",
            "smooth",
            "smooth_mode",
            "smooth_iterations",
            "erode",
            "enable_ocean_blending",
            "ocean_plane",
            "ocean_blending_radius",
            "subdivisions",
            "smoothing",
            "implicit_step_size",
            "implicit_samples")

def PointsControls():
    return ("points_type",
            "radius",
            "points_step_size")

def VolumeControls():
    return ("density_channel",)

def CheckRenderAs( nodeName ):
    render_as = cmds.getAttr(nodeName+".render_as")
    if render_as == 0:
        dimControls(nodeName, VolumeControls(), True)
        dimControls(nodeName, PointsControls(), True)
        dimControls(nodeName, SurfaceControls(), False)
        CheckRenderComponents( nodeName )
    elif render_as == 1:
        dimControls(nodeName, VolumeControls(), True)
        dimControls(nodeName, SurfaceControls(), True)
        dimControls(nodeName, PointsControls(), False)
    else:
        dimControls(nodeName, SurfaceControls(), True)
        dimControls(nodeName, PointsControls(), True)
        dimControls(nodeName, VolumeControls(), False)

def CheckRenderComponents( nodeName ):
    voxels = (cmds.getAttr(nodeName+".render_component") == 0)
    dimControls(nodeName, ("levelset_droplet_reveal_factor", "levelset_surface_radius", "levelset_droplet_radius", "levelset_resolution_factor", "levelset_max_volume_of_holes_to_close"), voxels)
    dimControls(nodeName, ("distance_channel"), not voxels)

class BifrostTemplate(ShapeTranslatorTemplate):
    def setup(self):
        self.addControl("opaque")
        self.addControl("matte")
        self.addSeparator()

        self.addControl("render_as", changeCommand=CheckRenderAs)
        self.addSeparator()
        self.addControl("velocity_scale")
        self.addControl("space_scale")
        self.addControl("channels")

        self.beginLayout("Clipping", collapse=True)
        self.addControl("clip", label="Enable")
        self.addCustom("clip_box", partial(InputMeshNew,"Bounding Mesh"), InputMeshReplace)
        self.endLayout()

        self.beginLayout("Surface Controls", collapse=False)
        self.addControl("surface_type")
        self.addControl("render_component", changeCommand=CheckRenderComponents)
        self.addSeparator()
        self.addControl("distance_channel")
        self.addSeparator()
        self.addControl("levelset_droplet_reveal_factor", label="Droplet Reveal Factor")
        self.addControl("levelset_surface_radius", label="Surface Radius")
        self.addControl("levelset_droplet_radius", label="Droplet Radius")
        self.addControl("levelset_resolution_factor", label="Resolution Factor")
        self.addControl("levelset_max_volume_of_holes_to_close", label="Max Volume Of Holes To Close")

        self.beginLayout("Filtering", collapse=True)
        self.addControl("dilate")
        self.addSeparator()
        self.addControl("smooth")
        self.addControl("smooth_mode")
        self.addControl("smooth_iterations")
        self.addSeparator()
        self.addControl("erode")
        self.endLayout()

        self.beginLayout("Ocean Blending", collapse=True)
        self.addControl("enable_ocean_blending", label="Enable")
        self.addCustom("ocean_plane", partial(InputMeshNew,"Mesh Plane"), InputMeshReplace)
        self.addControl("ocean_blending_radius", label="Boundary Radius")
        self.endLayout()

        self.beginLayout("Mesh Controls", collapse=True)
        self.addControl("subdivisions")
        self.addControl("smoothing")
        self.endLayout()
        self.beginLayout("Implicit Controls", collapse=True)
        self.addControl("implicit_step_size", label="Step Size")
        self.addControl("implicit_samples", label="Samples")
        self.endLayout()
        self.beginLayout("Advanced", collapse=True)
        self.addControl("export_laplacian")
        self.addControl("export_curvature")
        self.endLayout()
        self.endLayout()

        self.beginLayout("Points Controls", collapse=False)
        self.addControl("points_type", label="Type")
        self.addControl("radius")
        self.addControl("points_step_size", label="Step Size")
        self.endLayout()

        self.beginLayout("Volume Controls", collapse=False)
        self.addControl("density_channel")
        self.endLayout()

        self.suppress("aiDebug")
        self.suppress("debug")
        self.suppress("aiUserOptions")

templates.registerAETemplate(BifrostTemplate, 'bifrostShape')

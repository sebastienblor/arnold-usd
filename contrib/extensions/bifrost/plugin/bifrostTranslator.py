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

def CheckRenderComponents( nodeName ):
    voxels = (cmds.getAttr(nodeName+".render_component") == 0)
    dimControls(nodeName, ("levelset_droplet_reveal_factor", "levelset_surface_radius", "levelset_droplet_radius", "levelset_resolution_factor", "levelset_max_volume_of_holes_to_close"), voxels)
    dimControls(nodeName, ("distance_channel"), not voxels)

class BifrostTemplate(ShapeTranslatorTemplate):
    def setup(self):
        self.addControl("opaque")
        self.addControl("matte")
        self.addSeparator()

        self.addControl("render_as")
        self.addSeparator()
        self.addControl("render_component", changeCommand=CheckRenderComponents)
        self.addControl("velocity_scale")
        self.addControl("space_scale")
        self.addControl("channels")

        self.beginLayout("Surface Controls", collapse=False)
        self.addControl("distance_channel")
        self.addSeparator()
        self.addControl("levelset_droplet_reveal_factor", label="Droplet Reveal Factor")
        self.addControl("levelset_surface_radius", label="Surface Radius")
        self.addControl("levelset_droplet_radius", label="Droplet Radius")
        self.addControl("levelset_resolution_factor", label="Resolution Factor")
        self.addControl("levelset_max_volume_of_holes_to_close", label="Max Volume Of Holes To Close")

        self.beginLayout("Ocean Blending", collapse=False)
        self.addControl("enable_infinite_blending", label="Enable")
        self.addCustom("ocean_plane", partial(InputMeshNew,"Mesh Plane"), InputMeshReplace)
        self.addControl("infinite_blending_radius", label="Boundary Radius")
        self.endLayout()

        self.beginLayout("Mesh Controls", collapse=False)
        self.addControl("subdivisions")
        self.addControl("smoothing")
        self.endLayout()
        self.endLayout()

        self.beginLayout("Filtering", collapse=False)
        self.addControl("dilate")
        self.addSeparator()
        self.addControl("smooth")
        self.addControl("smooth_mode")
        self.addControl("smooth_iterations")
        self.addSeparator()
        self.addControl("erode")
        self.endLayout()

        self.beginLayout("Clipping", collapse=False)
        self.addControl("clip", label="Enable")
        self.addCustom("clip_box", partial(InputMeshNew,"Bounding Mesh"), InputMeshReplace)
        self.endLayout()

        self.suppress("aiDebug")
        self.suppress("debug")
        self.suppress("aiUserOptions")

templates.registerAETemplate(BifrostTemplate, 'bifrostShape')

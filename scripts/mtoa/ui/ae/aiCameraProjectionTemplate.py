import maya.mel
import maya.cmds as cmds
from mtoa.ui.ae.shaderTemplate import ShaderAETemplate

menu_str = "aiCameraProjecionCamMenu"


class AEaiCameraProjectionTemplate(ShaderAETemplate):

    def linkCamera(self, nodeAttr):
        camera = cmds.optionMenu(self.linkCameraMenu, q=True, v=True)
        connections = cmds.listConnections(nodeAttr, sh=True) or []
        if camera == 'None':
            # disconnect the attribute
            if len(connections):
                cmds.disconnectAttr(connections[0]+'.message', nodeAttr)

        elif cmds.objExists(camera) and cmds.objectType(camera, isa="camera"):
            cmds.connectAttr(camera+'.message', nodeAttr, f=True)
        else:
            msg = "{} is not a camera node. Cannot link {} to it"
            cmds.warning(msg.format(camera, nodeAttr))

    def newCameraControl(self, nodeAttr):
        # get all cmaeras
        allCameras = cmds.ls(ca=True)
        connections = cmds.listConnections(nodeAttr, sh=True) or []
        cmds.setUITemplate('attributeEditorTemplate', pushTemplate=True)

        linkProjLayout = cmds.rowLayout(nc=2)
        cmds.text(label="Link To Camera")
        self.linkCameraMenu = cmds.optionMenu(label="", changeCommand=lambda arg=None: self.linkCamera(nodeAttr))
        cmds.menuItem(label="None")

        i = 1
        for cam in allCameras:
            # only perspective cameras please...
            if not cmds.camera(cam, q=True, o=True):
                menu_item = cmds.menuItem("{}{:02d}".format(menu_str, i), label=cam)

                # see if this should be the selected menu item
                if len(connections) and connections[0] == cam:
                    cmds.optionMenu(self.linkCameraMenu, e=True, sl=i+1)
                i += 1

        cmds.setParent('..')

        cmds.setUITemplate(popTemplate=True)

    def updateCameraControl(self, nodeAttr):
        allCameras = cmds.ls(ca=True)
        connections = cmds.listConnections(nodeAttr, sh=True) or []

        cmds.setUITemplate('attributeEditorTemplate', pushTemplate=True)
        cmds.setParent(self.linkCameraMenu, m=True)
        cmds.optionMenu(self.linkCameraMenu, e=True, sl=1)

        i = 1
        for cam in allCameras:
            # only perspective cameras please...
            if not cmds.camera(cam, q=True, o=True):

                if cmds.menuItem("{}{:02d}".format(menu_str, i), exists=True):
                    menu_item = cmds.menuItem("{}{:02d}".format(menu_str, i), edit=True, label=cam)
                else:
                    menu_item = cmds.menuItem("{}{:02d}".format(menu_str, i), label=cam)

                # see if this should be the selected menu item
                if len(connections) and connections[0] == cam:
                    cmds.optionMenu(self.linkCameraMenu, e=True, sl=i+1)
                i += 1

        while cmds.menuItem("{}{:02d}".format(menu_str, i), exists=True):
            cmds.deleteUI("{}{:02d}".format(menu_str, i))
            i += 1

        cmds.setParent('..', m=True)

        cmds.setUITemplate(popTemplate=True)

        cmds.optionMenu(self.linkCameraMenu, edit=True, changeCommand=lambda arg=None: self.linkCamera(nodeAttr) )

    def setup(self):
        self.beginScrollLayout()
        self.beginLayout("Base", collapse=False)
        self.addControl('projectionColor')
        self.addControl('offscreenColor')
        self.addControl('mask')
        self.addControl('aspectRatio')
        self.addControl('frontFacing')
        self.addControl('backFacing')
        self.addControl('useShadingNormal')
        self.endLayout()

        self.beginLayout("Camera", collapse=False)
        self.addCustom('camera', self.newCameraControl, self.updateCameraControl)
        self.endLayout()

        self.beginLayout("Coordinates", collapse=False)
        self.addControl('coordSpace')
        self.addControl('prefName')
        self.addControl('P')
        self.endLayout()

        maya.mel.eval('AEdependNodeTemplate '+self.nodeName)
        self.addExtraControls()
        self.endScrollLayout()

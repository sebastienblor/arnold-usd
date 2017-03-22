import pymel.core as pm
import maya.cmds as cmds
import mtoa.ui.ae.templates as templates
import traceback

class hairPhysicalShaderTemplate(templates.AttributeTemplate):
    def setup(self):
        # Indirect Diffuse and Diffuse Cache
        self.addControl("ai_kd_ind", label="Indirect Diffuse")
        self.addSeparator()

        # UVs

        # AOVs

        # Custom Buttons
        self.addCustom("buttons", self.buttonsNew, self.buttonsReplace)

    def buttonsNew(self, nodeName):
        cmds.button("lockUnsupported", label="Lock Unsupported Attributes")
        self.buttonsReplace(nodeName)

    def buttonsReplace(self, nodeName):
        cmds.button("lockUnsupported", edit=True,
            command=pm.Callback(self.lockUnsupportedAttributes, nodeName))

    def lockUnsupportedAttributes(self, nodeName):
        nodeName = nodeName[:nodeName.index(".")]
        # Lock and reset unsupported attributes
        self.lockUnsupportedColorPlug(nodeName + ".ambientColor")
        self.lockUnsupportedColorPlug(nodeName + ".incandescence")
        self.lockUnsupportedColorPlug(nodeName + ".colorG")
        self.lockUnsupportedFloatPlug(nodeName + ".intensityG")
        self.lockUnsupportedFloatPlug(nodeName + ".azimuthalShiftG")
        self.lockUnsupportedFloatPlug(nodeName + ".azimuthalWidthG")

        # Transmission shift is roughly -4
        self.lockUnsupportedFloatPlug(nodeName + ".longitudinalShiftTT", -4.0)

        # Transmission width radio is 2:5
        try:
            if not cmds.getAttr(nodeName + ".azimuthalWidthTT", lock=True):
                source = cmds.listConnections(nodeName + ".azimuthalWidthTT", d=True, p=True)
                if source:
                    cmds.disconnectAttr(source, nodeName + ".azimuthalWidthTT")
                mulNode = cmds.createNode("multiplyDivide")
                cmds.connectAttr(nodeName + ".longitudinalWidthTT", mulNode + ".input1X", force=True)
                cmds.setAttr(mulNode + ".input2X", 2.5)
                cmds.connectAttr(mulNode + ".outputX", nodeName + ".azimuthalWidthTT", force=True)
                cmds.setAttr(nodeName + ".azimuthalWidthTT", lock=True)
        except:
            traceback.print_exc()

        cmds.select(nodeName)

    def lockUnsupportedColorPlug(self, plug):
        try:
            if not cmds.getAttr(plug, lock=True):
                source = cmds.listConnections(plug, d=True, p=True)
                if source:
                    cmds.disconnectAttr(source, plug)
                cmds.setAttr(plug, 0.0, 0.0, 0.0, lock=True)
        except:
            traceback.print_exc()

    def lockUnsupportedFloatPlug(self, plug, defaultValue = 0.0):
        try:
            if not cmds.getAttr(plug, lock=True):
                source = cmds.listConnections(plug, d=True, p=True)
                if source:
                    cmds.disconnectAttr(source, plug)
                cmds.setAttr(plug, defaultValue, lock=True)
        except:
            traceback.print_exc()

templates.registerAETemplate(hairPhysicalShaderTemplate, "hairPhysicalShader")

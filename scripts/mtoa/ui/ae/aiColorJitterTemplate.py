import maya.mel
import maya.cmds as cmds
from mtoa.ui.ae.shaderTemplate import ShaderAETemplate

PANELS = ['User Data', 'Procedural', 'Object', 'Face']

class AEaiColorJitterTemplate(ShaderAETemplate):

    def createSwitchAttr(self, nodeAttr):
        node, attr = nodeAttr.split('.')
        if not cmds.attributeQuery(attr, node=node, exists=True):
            # make the attr
            cmds.addAttr(node, longName=attr, at="enum", enumName=":".join(PANELS) )

    def changePanel(self, choice):
        if not self.typeCtrl or not cmds.attrEnumOptionMenu(self.typeCtrl, exists=True):
            # not built yet
            return

        fullpath = cmds.attrEnumOptionMenu(self.typeCtrl, query=True, fullPathName=True)

        gparent = fullpath.rsplit('|', 3)[0]
        frame = fullpath.rsplit('|', 4)[0]

        try:
            cmds.frameLayout(frame, edit=True, collapsable=False, labelVisible=False, borderVisible=False)
        except RuntimeError:
            # this is a little dirty: it will only succeed when attaching to AE
            pass

        children = cmds.layout(gparent, query=True, childArray=True)

        for child in children:
            objType = cmds.objectTypeUI(child)
            if objType == 'frameLayout':
                label = cmds.frameLayout(child, query=True, label=True)
                cmds.frameLayout(child, edit=True, collapsable=False, labelVisible=False,
                                     visible=(label == choice))

    def updateTypeSwitch(self, nodeAttr):
        self.createSwitchAttr(nodeAttr)
        cmds.attrEnumOptionMenu(self.typeCtrl, edit=True, attribute=nodeAttr, changeCommand=self.changePanel)

        value = cmds.getAttr(nodeAttr)
        self.changePanel(PANELS[value])

    def createTypeSwitch(self, nodeAttr):
        self.createSwitchAttr(nodeAttr)
        cmds.rowLayout()
        self.typeCtrl = cmds.attrEnumOptionMenu(label='Type', attribute=nodeAttr, changeCommand=self.changePanel)

    def initPanel(self):
        value = cmds.getAttr(self.nodeAttr("typeSwitch"))
        self.changePanel(PANELS[value])

    def setup(self):
        self.addSwatch()
        self.beginScrollLayout()

        self.beginLayout('hide', collapse=False)
        self.addControl('input', label='Input')

        # make switch for showing each PANEL
        self.typeCtrl = None
        self.addCustom("typeSwitch", self.createTypeSwitch, self.updateTypeSwitch)

        self.beginLayout('User Data', collapse=False)
        self.addControl('dataInput', label='Data')
        self.addControl('dataGainMin', label='Gain Min')
        self.addControl('dataGainMax', label='Gain Max')
        self.addControl('dataHueMin', label='Hue Min')
        self.addControl('dataHueMax', label='Hue Max')
        self.addControl('dataSaturationMin', label='Saturation Min')
        self.addControl('dataSaturationMax', label='Saturation Max')
        self.addControl('dataSeed', label='Seed')
        self.endLayout()

        self.beginLayout('Procedural', collapse=False)
        self.addControl('procGainMin', label='Gain Min')
        self.addControl('procGainMax', label='Gain Max')
        self.addControl('procHueMin', label='Hue Min')
        self.addControl('procHueMax', label='Hue Max')
        self.addControl('procSaturationMin', label='Saturation Min')
        self.addControl('procSaturationMax', label='Saturation Max')
        self.addControl('procSeed', label='Seed')
        self.endLayout()

        self.beginLayout('Object', collapse=False)
        self.addControl('objGainMin', label='Gain Min')
        self.addControl('objGainMax', label='Gain Max')
        self.addControl('objHueMin', label='Hue Min')
        self.addControl('objHueMax', label='Hue Max')
        self.addControl('objSaturationMin', label='Saturation Min')
        self.addControl('objSaturationMax', label='Saturation Max')
        self.addControl('objSeed', label='Seed')
        self.endLayout()

        self.beginLayout('Face', collapse=False)
        self.addControl('faceMode', label='Face Mode')
        self.addControl('faceGainMin', label='Gain Min')
        self.addControl('faceGainMax', label='Gain Max')
        self.addControl('faceHueMin', label='Hue Min')
        self.addControl('faceHueMax', label='Hue Max')
        self.addControl('faceSaturationMin', label='Saturation Min')
        self.addControl('faceSaturationMax', label='Saturation Max')
        self.addControl('faceSeed', label='Seed')
        self.endLayout()

        self.endLayout()

        maya.mel.eval('AEdependNodeTemplate '+self.nodeName)
        self.addExtraControls()
        self.endScrollLayout()

        cmds.evalDeferred(self.initPanel)



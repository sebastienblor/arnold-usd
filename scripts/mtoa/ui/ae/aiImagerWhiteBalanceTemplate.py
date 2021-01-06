import maya.mel
from mtoa.ui.ae.templates import TranslatorControl
from mtoa.ui.ae.shaderTemplate import ShaderAETemplate
import maya.cmds as cmds
from mtoa.ui.ae.aiImagersBaseTemplate import ImagerBaseUI, registerImagerTemplate


class AEaiImagerWhiteBalanceTemplate(ShaderAETemplate):

    def setup(self):
        self.beginScrollLayout()
        self.baseLayout = self.beginLayout("Main", collapse=False)
        currentWidget = cmds.setParent(query=True)
        self.ui = ImagerWhiteBalanceUI(parent=currentWidget, nodeName=self.nodeName, template=self)
        self.endLayout()
        maya.mel.eval('AEdependNodeTemplate '+self.nodeName)
        self.addExtraControls()
        self.endScrollLayout()


class ImagerWhiteBalanceUI(ImagerBaseUI):

    def __init__(self, parent=None, nodeName=None, template=None):
        super(ImagerWhiteBalanceUI, self).__init__(parent, nodeName, template)

    def setup(self):
        super(ImagerWhiteBalanceUI, self).setup()
        self.addControl('mode', label='Mode', changeCommand=lambda *args: self.updateParamsVisibility(self.nodeName), annotation='White balance mode. (illuminant, temperature, custom)')
        self.addControl('illuminant', annotation='Temperature for black body mode.')
        self.addControl('temperature', annotation='Standard illuminant name for illuminante mode.', hideMapButton = True)
        self.addControl('custom', annotation='Custom white balance color.')

        self.updateParamsVisibility(self.nodeName)

    def updateParamsVisibility(self, nodeName):
        modeAttr = '%s.%s' % (nodeName, 'mode')
        modeValue = cmds.getAttr(modeAttr)
        self.dimControl('illuminant', state=modeValue != 0)
        self.dimControl('temperature', state=modeValue != 1)
        self.dimControl('custom', state=modeValue != 2)


registerImagerTemplate("aiImagerWhiteBalance", ImagerWhiteBalanceUI)

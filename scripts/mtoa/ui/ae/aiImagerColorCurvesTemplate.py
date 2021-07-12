import maya.cmds as cmds
import maya.mel
from mtoa.ui.ae.templates import TranslatorControl
from mtoa.ui.ae.shaderTemplate import ShaderAETemplate
from mtoa.ui.ae.aiImagersBaseTemplate import ImagerBaseUI, registerImagerTemplate


class AEaiImagerColorCurvesTemplate(ShaderAETemplate):

    def setup(self):

        self.beginScrollLayout()
        currentWidget = cmds.setParent(query=True)
        self.ui = ImagerColorCurvesUI(parent=currentWidget, nodeName=self.nodeName, template=self)

        maya.mel.eval('AEdependNodeTemplate '+self.nodeName)

        self.addExtraControls()
        self.endScrollLayout()


class ImagerColorCurvesUI(ImagerBaseUI):
    def __init__(self, parent=None, nodeName=None, template=None):
        super(ImagerColorCurvesUI, self).__init__(parent, nodeName, template)

    def setup(self):
        super(ImagerColorCurvesUI, self).setup()

        self.addSeparator()
        self.beginLayout("RGB", collapse=False)
        # maya.mel.eval('AEaddRampControl '+self.nodeName + '.RGB')
        self.endLayout()


registerImagerTemplate("aiImagerColorCurves", ImagerColorCurvesUI)

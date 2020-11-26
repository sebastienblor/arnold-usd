import maya.mel
from mtoa.ui.ae.templates import TranslatorControl
from mtoa.ui.ae.shaderTemplate import ShaderAETemplate
from mtoa.ui.ae.aiImagersBaseTemplate import ImagerBaseUI
import maya.cmds as cmds


class AEaiImagerOptixDenoiserTemplate(ShaderAETemplate):

    def setup(self):
        self.beginScrollLayout()
        self.ui = None
        self.baseLayout = self.beginLayout("Main", collapse=False)
        currentWidget = cmds.setParent(query=True)
        self.ui = ImagerOptixDenoiserUI(parent=currentWidget, nodeName=self.nodeName, template=self)
        self.endLayout()
        maya.mel.eval('AEdependNodeTemplate '+self.nodeName)
        self.addExtraControls()
        self.endScrollLayout()


class ImagerOptixDenoiserUI(ImagerBaseUI):
    def __init__(self, parent=None, nodeName = None):
        super(ImagerOptixDenoiserUI, self).__init__(parent,nodeName)

    def setup(self):
        super(ImagerOptixDenoiserUI, self).setup()
        self.addControl('outputSuffix', annotation='')
        self.addControl('blend', annotation='')
        self.addControl('clamp', annotation='')
        self.addControl('clampMin', annotation='')
        self.addControl('clampMax', annotation='')
        self.addControl('useFeatureAovs', annotation='')

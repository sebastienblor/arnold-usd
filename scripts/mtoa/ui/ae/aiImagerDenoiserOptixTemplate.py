import maya.mel
from mtoa.ui.ae.templates import TranslatorControl
from mtoa.ui.ae.shaderTemplate import ShaderAETemplate
from mtoa.ui.ae.aiImagersBaseTemplate import ImagerBaseUI,registerImagerTemplate
import maya.cmds as cmds


class AEaiImagerDenoiserOptixTemplate(ShaderAETemplate):

    def setup(self):

        self.beginScrollLayout()
        currentWidget = cmds.setParent(query=True)
        self.ui = ImagerDenoiserOptixUI(parent=currentWidget, nodeName=self.nodeName, template=self)

        maya.mel.eval('AEdependNodeTemplate '+self.nodeName)

        self.addExtraControls()
        self.endScrollLayout()


class ImagerDenoiserOptixUI(ImagerBaseUI):
    def __init__(self, parent=None, nodeName = None, template=None):
        super(ImagerDenoiserOptixUI, self).__init__(parent, nodeName, template)

    def setup(self):
        super(ImagerDenoiserOptixUI, self).setup()
        self.beginLayout("Main", collapse=False)
        self.addControl('outputSuffix', annotation='')
        self.addControl('blend', annotation='', hideMapButton = True)
        self.addControl('clamp', annotation='')
        self.addControl('clampMin', annotation='', hideMapButton = True)
        self.addControl('clampMax', annotation='', hideMapButton = True)
        self.addControl('useFeatureAovs', annotation='')
        self.endLayout()


registerImagerTemplate("aiImagerDenoiserOptix", ImagerDenoiserOptixUI)

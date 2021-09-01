import maya.mel
from mtoa.ui.ae.templates import TranslatorControl
from mtoa.ui.ae.shaderTemplate import ShaderAETemplate
from mtoa.ui.ae.aiImagersBaseTemplate import ImagerBaseUI, registerImagerTemplate
import maya.cmds as cmds


class AEaiImagerDenoiserOidnTemplate(ShaderAETemplate):

    def setup(self):

        self.beginScrollLayout()
        currentWidget = cmds.setParent(query=True)
        self.ui = ImagerDenoiserOIDNUI(parent=currentWidget, nodeName=self.nodeName, template=self)

        maya.mel.eval('AEdependNodeTemplate '+self.nodeName)

        self.addExtraControls()
        self.endScrollLayout()


class ImagerDenoiserOIDNUI(ImagerBaseUI):
    def __init__(self, parent=None, nodeName = None, template=None):
        super(ImagerDenoiserOIDNUI, self).__init__(parent, nodeName,template)

    def setup(self):
        super(ImagerDenoiserOIDNUI, self).setup()
        self.beginLayout("Main", collapse=False)
        self.addControl('outputSuffix', annotation='')
        self.addControl('useFeatureAovs', annotation='')
        self.endLayout()


registerImagerTemplate("aiImagerDenoiserOidn", ImagerDenoiserOIDNUI)

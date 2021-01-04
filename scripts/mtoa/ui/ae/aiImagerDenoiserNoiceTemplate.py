import maya.mel
from mtoa.ui.ae.templates import TranslatorControl
from mtoa.ui.ae.shaderTemplate import ShaderAETemplate
from mtoa.ui.ae.aiImagersBaseTemplate import ImagerBaseUI, registerImagerTemplate
import maya.cmds as cmds


class AEaiImagerDenoiserTemplate(ShaderAETemplate):

    def setup(self):
        self.beginScrollLayout()
        self.ui = None
        self.baseLayout = self.beginLayout("Main", collapse=False)
        currentWidget = cmds.setParent(query=True)
        self.ui = ImagerDenoiserUI(parent=currentWidget, nodeName=self.nodeName, template=self)
        self.endLayout()
        maya.mel.eval('AEdependNodeTemplate '+self.nodeName)
        self.addExtraControls()
        self.endScrollLayout()


class ImagerDenoiserUI(ImagerBaseUI):
    def __init__(self, parent=None, nodeName = None, template=None):
        super(ImagerDenoiserUI, self).__init__(parent, nodeName,template)

    def setup(self):
        super(ImagerDenoiserUI, self).setup()
        self.addControl('patchRadius')
        self.addControl('searchRadius')
        self.addControl('tileSize')
        self.addControl('variance')


registerImagerTemplate("aiImagerDenoiserNoice", ImagerDenoiserUI)

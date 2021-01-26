import maya.mel
from mtoa.ui.ae.templates import TranslatorControl
from mtoa.ui.ae.shaderTemplate import ShaderAETemplate
from mtoa.ui.ae.aiImagersBaseTemplate import ImagerBaseUI, registerImagerTemplate
import maya.cmds as cmds


class AEaiImagerDenoiserNoiceTemplate(ShaderAETemplate):

    def setup(self):

        self.beginScrollLayout()
        currentWidget = cmds.setParent(query=True)
        self.ui = ImagerDenoiserUI(parent=currentWidget, nodeName=self.nodeName, template=self)

        maya.mel.eval('AEdependNodeTemplate '+self.nodeName)

        self.addExtraControls()
        self.endScrollLayout()


class ImagerDenoiserUI(ImagerBaseUI):
    def __init__(self, parent=None, nodeName = None, template=None):
        super(ImagerDenoiserUI, self).__init__(parent, nodeName,template)

    def setup(self):
        super(ImagerDenoiserUI, self).setup()
        self.beginLayout("Main", collapse=False)
        self.addControl('outputSuffix', annotation='')
        self.addControl('patchRadius', hideMapButton = True, annotation = "")
        self.addControl('searchRadius', hideMapButton = True, annotation = "")
        self.addControl('variance', hideMapButton = True, annotation = "")
        self.endLayout()


registerImagerTemplate("aiImagerDenoiserNoice", ImagerDenoiserUI)

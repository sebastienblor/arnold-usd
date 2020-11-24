import maya.mel
from mtoa.ui.ae.templates import TranslatorControl
from mtoa.ui.ae.shaderTemplate import ShaderAETemplate
from mtoa.ui.ae.aiImagersBaseTemplate import ImagerBaseUI
import maya.cmds as cmds

class AEaiImagerDenoiserTemplate(ShaderAETemplate):

    def createNewDenoiserUI(self,nodeAttr):
        currentWidget = cmds.setParent(query=True)
        self.ui = ImagerDenoiserUI(parent = currentWidget,nodeName = self.nodeName)
   

    def replaceDenoiserUI(self,nodeAttr):
        currentWidget = cmds.setParent(query=True)
        if self.ui:
            for item in cmds.layout( currentWidget, query=True, childArray=True) :
                cmds.deleteUI(item)
        self.ui = ImagerDenoiserUI(currentWidget, self.nodeName)

    def setup(self):
        self.beginScrollLayout()
        self.ui = None
        self.baseLayout = self.beginLayout("Main", collapse=False)
        self.addCustom('dummy', self.createNewDenoiserUI, self.replaceDenoiserUI)
        self.endLayout()
        maya.mel.eval('AEdependNodeTemplate '+self.nodeName)
        self.addExtraControls()
        self.endScrollLayout()

class ImagerDenoiserUI(ImagerBaseUI):
    def __init__(self, parent=None, nodeName = None):
        super(ImagerDenoiserUI, self).__init__(parent,nodeName)
        self.addControl('patchRadius')
        self.addControl('searchRadius')
        self.addControl('tileSize')
        self.addControl('variance')
        

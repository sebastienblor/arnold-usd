import maya.mel
from mtoa.ui.ae.templates import TranslatorControl
from mtoa.ui.ae.shaderTemplate import ShaderAETemplate
from mtoa.ui.ae.aiImagersBaseTemplate import ImagerBaseUI
import maya.cmds as cmds

class AEaiImagerLensEffectsTemplate(ShaderAETemplate):


    def createNewLensEffectUI(self,nodeAttr):
        currentWidget = cmds.setParent(query=True)
        self.ui = ImagerLensEffectUI(parent = currentWidget,nodeName = self.nodeName)
   

    def replaceLensEffectUI(self,nodeAttr):
        currentWidget = cmds.setParent(query=True)
        if self.ui:
            for item in cmds.layout( currentWidget, query=True, childArray=True) :
                cmds.deleteUI(item)
        self.ui = ImagerLensEffectUI(currentWidget, self.nodeName)

    def setup(self):
        self.beginScrollLayout()
        self.ui = None
        self.baseLayout = self.beginLayout("Main", collapse=False)
        self.addCustom('dummy', self.createNewLensEffectUI, self.replaceLensEffectUI)
        self.endLayout()
        maya.mel.eval('AEdependNodeTemplate '+self.nodeName)
        self.addExtraControls()
        self.endScrollLayout()

class ImagerLensEffectUI(ImagerBaseUI):
    def __init__(self, parent=None, nodeName = None):
        super(ImagerLensEffectUI, self).__init__(parent,nodeName)
        cmds.attrControlGrp( attribute=nodeName+'.vignetting')

import maya.mel
from mtoa.ui.ae.templates import TranslatorControl
from mtoa.ui.ae.shaderTemplate import ShaderAETemplate
from mtoa.ui.ae.aiImagersBaseTemplate import ImagerBaseUI, registerImagerTemplate
import maya.cmds as cmds


class AEaiImagerLensEffectsTemplate(ShaderAETemplate):

    def setup(self):
        self.beginScrollLayout()
        self.baseLayout = self.beginLayout("Main", collapse=False)
        currentWidget = cmds.setParent(query=True)
        self.ui = ImagerLensEffectUI(parent=currentWidget, nodeName=self.nodeName, template=self)
        self.endLayout()
        maya.mel.eval('AEdependNodeTemplate '+self.nodeName)
        self.addExtraControls()
        self.endScrollLayout()


class ImagerLensEffectUI(ImagerBaseUI):
    def __init__(self, parent=None, nodeName=None, template=None):
        super(ImagerLensEffectUI, self).__init__(parent, nodeName, template)

    def setup(self):
        super(ImagerLensEffectUI, self).setup()
        self.addSeparator()
        self.addControl('vignetting', label='Vignetting', annotation='Vignetting amount')
        self.addControl('bloom_threshold', label='Bloom Threshold', annotation='')
        self.addControl('bloom_tint', label='Bloom Tint', annotation='')
        self.addControl('bloom_radius', label='Bloom Radius', annotation='')
        self.addControl('bloom_strength', label='Bloom Strength', annotation='')



registerImagerTemplate("aiImagerLensEffects", ImagerLensEffectUI)

import maya.mel
from mtoa.ui.ae.templates import TranslatorControl
from mtoa.ui.ae.shaderTemplate import ShaderAETemplate
from mtoa.ui.ae.aiImagersBaseTemplate import ImagerBaseUI, registerImagerTemplate
import maya.cmds as cmds


class AEaiImagerLensEffectsTemplate(ShaderAETemplate):

    def setup(self):

        self.beginScrollLayout()

        currentWidget = cmds.setParent(query=True)
        self.ui = ImagerLensEffectUI(parent=currentWidget, nodeName=self.nodeName, template=self)

        maya.mel.eval('AEdependNodeTemplate '+self.nodeName)

        self.addExtraControls()
        self.endScrollLayout()


class ImagerLensEffectUI(ImagerBaseUI):
    def __init__(self, parent=None, nodeName=None, template=None):
        super(ImagerLensEffectUI, self).__init__(parent, nodeName, template)

    def setup(self):
        super(ImagerLensEffectUI, self).setup()
        self.beginLayout("Vignetting", collapse=False)
        self.addControl('vignetting', label='Vignetting', annotation='Vignetting amount', hideMapButton = True)
        self.endLayout()
        self.beginLayout("Bloom", collapse=False)
        self.addControl('bloom_strength', label='Strength', annotation='', hideMapButton = True)
        self.addControl('bloom_tint', label='Tint', annotation='')
        self.addControl('bloom_radius', label='Radius', annotation='', hideMapButton = True)
        self.addControl('bloom_threshold', label='Threshold', annotation='', hideMapButton = True)
        self.endLayout()

registerImagerTemplate("aiImagerLensEffects", ImagerLensEffectUI)

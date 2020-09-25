import maya.mel
from mtoa.ui.ae.templates import TranslatorControl
from mtoa.ui.ae.shaderTemplate import ShaderAETemplate

class AEaiImagerTonemapTemplate(ShaderAETemplate):

    def setup(self):
        #mel.eval('AEswatchDisplay "%s"' % nodeName)

        self.beginScrollLayout()
        self.addControl('enable', label='Enable')
        self.addSeparator()
        self.addControl('mode', label='Mode')
        self.beginLayout("Filmic", collapse=False)
        self.addControl('filmicToeStrength', label='Toe Strength')
        self.addControl('filmicToeLength', label='Toe Length')
        self.addSeparator()
        self.addControl('filmicShoulderStrength', label='Shoulder Strength')
        self.addControl('filmicShoulderLength', label='Shoulder Length')
        self.addControl('filmicShoulderAngle', label='Shoulder Angle')
        self.endLayout()
        self.beginLayout("Reinhard", collapse=False)
        self.addControl('reinhardHighlights', label='Highlights')
        self.addControl('reinhardShadows', label='Shadows')
        self.endLayout()
        self.addSeparator()
        self.addControl('preserveSaturation', label='Preserve Saturation')
        self.addControl('gamma', label='Gamma')
        maya.mel.eval('AEdependNodeTemplate '+self.nodeName)

        self.addExtraControls()
        self.endScrollLayout()

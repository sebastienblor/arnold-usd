import maya.mel
from mtoa.ui.ae.templates import TranslatorControl
from mtoa.ui.ae.shaderTemplate import ShaderAETemplate

class AEaiImagerColorCorrectTemplate(ShaderAETemplate):

    def setup(self):
        #mel.eval('AEswatchDisplay "%s"' % nodeName)

        self.beginScrollLayout()
        self.addControl('enable', label='Enable')
        self.addSeparator()
        self.beginLayout("Main", collapse=False)
        self.addControl('mainSaturation', label='Saturation')
        self.addControl('mainContrast', label='Contrast')
        self.addControl('mainGamma', label='Gamma')
        self.addControl('mainGain', label='Gain')
        self.addControl('mainOffset', label='Offset')
        self.endLayout()

        self.beginLayout("Shadows", collapse=True)
        self.addControl('shadowsSaturation', label='Saturation')
        self.addControl('shadowsContrast', label='Contrast')
        self.addControl('shadowsGamma', label='Gamma')
        self.addControl('shadowsGain', label='Gain')
        self.addControl('shadowsOffset', label='Offset')
        self.endLayout()

        self.beginLayout("Midtones", collapse=True)
        self.addControl('midtonesSaturation', label='Saturation')
        self.addControl('midtonesContrast', label='Contrast')
        self.addControl('midtonesGamma', label='Gamma')
        self.addControl('midtonesGain', label='Gain')
        self.addControl('midtonesOffset', label='Offset')
        self.endLayout()

        self.beginLayout("Highlights", collapse=True)
        self.addControl('highlightsSaturation', label='Saturation')
        self.addControl('highlightsContrast', label='Contrast')
        self.addControl('highlightsGamma', label='Gamma')
        self.addControl('highlightsGain', label='Gain')
        self.addControl('highlightsOffset', label='Offset')
        self.endLayout()
        
        maya.mel.eval('AEdependNodeTemplate '+self.nodeName)

        self.addExtraControls()
        self.endScrollLayout()

import maya.mel
from mtoa.ui.ae.templates import TranslatorControl
from mtoa.ui.ae.shaderTemplate import ShaderAETemplate

class AEaiImagerColorCorrectTemplate(ShaderAETemplate):

    def setup(self):
        #mel.eval('AEswatchDisplay "%s"' % nodeName)

        self.beginScrollLayout()
        self.addControl('enable', label='Enable', annotation='Enables this imager.')
        self.addSeparator()
        self.beginLayout("Main", collapse=False)
        self.addControl('mainSaturation', label='Saturation', annotation='Saturation multiplier for all values.')
        self.addControl('mainContrast', label='Contrast', annotation='Contrast multiplier for all values.')
        self.addControl('mainGamma', label='Gamma', annotation='Gamma amount for all values.')
        self.addControl('mainGain', label='Gain', annotation='Gain color multiplier for all values.')
        self.addControl('mainOffset', label='Offset', annotation='Offset color amount for all values.')
        self.endLayout()

        self.beginLayout("Shadows", collapse=True)
        self.addControl('shadowsSaturation', label='Saturation', annotation='Saturation multiplier for darker values.')
        self.addControl('shadowsContrast', label='Contrast', annotation='Contrast multiplier for darker values.')
        self.addControl('shadowsGamma', label='Gamma', annotation='Gamma amount for darker values.')
        self.addControl('shadowsGain', label='Gain', annotation='Gain color multiplier for darker values.')
        self.addControl('shadowsOffset', label='Offset', annotation='Offset color amount for darker values.')
        self.endLayout()

        self.beginLayout("Midtones", collapse=True)
        self.addControl('midtonesSaturation', label='Saturation', annotation='Saturation multiplier for midtone values.')
        self.addControl('midtonesContrast', label='Contrast', annotation='Contrast multiplier for midtone values.')
        self.addControl('midtonesGamma', label='Gamma', annotation='Gamma amount for midtone values.')
        self.addControl('midtonesGain', label='Gain', annotation='Gain color multiplier for midtone values.')
        self.addControl('midtonesOffset', label='Offset', annotation='Offset color amount for midtone values.')
        self.endLayout()

        self.beginLayout("Highlights", collapse=True)
        self.addControl('highlightsSaturation', label='Saturation', annotation='Saturation multiplier for brighter values.')
        self.addControl('highlightsContrast', label='Contrast', annotation='Contrast multiplier for brighter values.')
        self.addControl('highlightsGamma', label='Gamma', annotation='Gamma amount for brighter values.')
        self.addControl('highlightsGain', label='Gain', annotation='Gain color multiplier for brighter values.')
        self.addControl('highlightsOffset', label='Offset', annotation='Offset color amount for brighter values.')
        self.endLayout()
        
        maya.mel.eval('AEdependNodeTemplate '+self.nodeName)

        self.addExtraControls()
        self.endScrollLayout()

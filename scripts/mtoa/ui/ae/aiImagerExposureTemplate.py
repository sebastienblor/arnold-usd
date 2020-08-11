import maya.mel
from mtoa.ui.ae.templates import TranslatorControl
from mtoa.ui.ae.shaderTemplate import ShaderAETemplate

class AEaiImagerExposureTemplate(ShaderAETemplate):

    def setup(self):
        #mel.eval('AEswatchDisplay "%s"' % nodeName)

        self.beginScrollLayout()

        self.addControl('exposure', label='Exposure')
        self.addSeparator()
        self.addControl('highlights', label='Highlights')
        self.addControl('midtones', label='Midtones')
        self.addControl('shadows', label='Shadows')

        self.beginLayout("White Balance", collapse=False)

        self.addControl('whiteBalanceMode', label='Mode')
        self.addControl('whiteBalanceTemperature', label='Temperature')
        self.addControl('whiteBalanceIlluminant', label='Illuminant')
        self.endLayout()

        self.addSeparator()
        self.addControl('vignetting', label='Vignetting')
        self.addControl('saturation', label='Saturation')
        self.addControl('physicalScale', label='Physical Scale')
        
        maya.mel.eval('AEdependNodeTemplate '+self.nodeName)

        self.addExtraControls()
        self.endScrollLayout()

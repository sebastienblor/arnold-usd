import maya.mel
from mtoa.ui.ae.templates import TranslatorControl
from mtoa.ui.ae.shaderTemplate import ShaderAETemplate

class AEaiImagerExposureTemplate(ShaderAETemplate):

    def setup(self):
        #mel.eval('AEswatchDisplay "%s"' % nodeName)

        self.beginScrollLayout()
        self.addControl('enable', label='Enable', annotation='Enables this imager.')
        self.addSeparator()
        self.addControl('exposure', label='Exposure', annotation='Exposure compensation amount in f-stops.')
        
        maya.mel.eval('AEdependNodeTemplate '+self.nodeName)

        self.addExtraControls()
        self.endScrollLayout()

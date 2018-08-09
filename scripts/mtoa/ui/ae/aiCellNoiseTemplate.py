import maya.mel
from mtoa.ui.ae.shaderTemplate import ShaderAETemplate

class AEaiCellNoiseTemplate(ShaderAETemplate):
    def setup(self):
        self.addSwatch()
        self.beginScrollLayout()
        self.addControl('pattern')
        self.addControl('additive')
        self.addControl('octaves')
        self.addControl('lacunarity')
        self.addControl('amplitude')
        self.addControl('scale')
        self.addControl('offset')

        self.addControl('coordSpace')
        self.addControl('prefName')
        self.addControl('P')
        self.addControl('time')
        self.addControl('color')
        self.addControl('palette')
        self.addControl('density')
        self.addControl('randomness')
        
        self.beginLayout('User AOVs', collapse=True)
        self.endLayout()

        maya.mel.eval('AEdependNodeTemplate '+self.nodeName)

        self.addExtraControls()
        self.endScrollLayout()

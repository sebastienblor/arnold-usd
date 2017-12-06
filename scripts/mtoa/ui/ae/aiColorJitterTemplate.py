import maya.mel
from mtoa.ui.ae.shaderTemplate import ShaderAETemplate

class AEaiColorJitterTemplate(ShaderAETemplate):
    def setup(self):
        self.addSwatch()
        self.beginScrollLayout()

        self.addControl('input', label='Input')

        self.beginLayout('User Data', collapse=False)
        self.addControl('dataInput', label='Data')
        self.addControl('dataGainMin', label='Gain Min')
        self.addControl('dataGainMax', label='Gain Max')
        self.addControl('dataHueMin', label='Hue Min')
        self.addControl('dataHueMax', label='Hue Max')
        self.addControl('dataSaturationMin', label='Saturation Min')
        self.addControl('dataSaturationMax', label='Saturation Max')
        self.addControl('dataSeed', label='Seed')
        self.endLayout()

        self.beginLayout('Procedural', collapse=False)
        self.addControl('procGainMin', label='Gain Min')
        self.addControl('procGainMax', label='Gain Max')
        self.addControl('procHueMin', label='Hue Min')
        self.addControl('procHueMax', label='Hue Max')
        self.addControl('procSaturationMin', label='Saturation Min')
        self.addControl('procSaturationMax', label='Saturation Max')
        self.addControl('procSeed', label='Seed')
        self.endLayout()

        self.beginLayout('Object', collapse=False)
        self.addControl('objGainMin', label='Gain Min')
        self.addControl('objGainMax', label='Gain Max')
        self.addControl('objHueMin', label='Hue Min')
        self.addControl('objHueMax', label='Hue Max')
        self.addControl('objSaturationMin', label='Saturation Min')
        self.addControl('objSaturationMax', label='Saturation Max')
        self.addControl('objSeed', label='Seed')
        self.endLayout()

        self.beginLayout('Face', collapse=False)
        self.addControl('faceGainMin', label='Gain Min')
        self.addControl('faceGainMax', label='Gain Max')
        self.addControl('faceHueMin', label='Hue Min')
        self.addControl('faceHueMax', label='Hue Max')
        self.addControl('faceSaturationMin', label='Saturation Min')
        self.addControl('faceSaturationMax', label='Saturation Max')
        self.addControl('faceSeed', label='Seed')
        self.endLayout()

        maya.mel.eval('AEdependNodeTemplate '+self.nodeName)

        self.addExtraControls()
        self.endScrollLayout()



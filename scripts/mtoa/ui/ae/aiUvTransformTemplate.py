import pymel.core as pm
from mtoa.ui.ae.shaderTemplate import ShaderAETemplate

class AEaiUvTransformTemplate(ShaderAETemplate):
    def setup(self):
        self.addSwatch()
        self.beginScrollLayout()

        self.addControl('passthrough', label='Input')
        self.addSeparator()

        self.addControl('uvset')


        self.beginLayout('Frame', collapse=False)
        self.addControl('scaleFrame', label='Scale')
        self.addControl('translateFrame', label='Translate')
        self.addControl('rotateFrame', label='Rotate')
        self.addControl('pivotFrame', label='Pivot')
        self.addControl('wrapFrameU', label='Wrap U')
        self.addControl('wrapFrameV', label='Wrap V')
        self.addControl('wrapFrameColor', label='Wrap Color')
        self.endLayout()

        self.beginLayout('Transform', collapse=False)
        self.addControl('repeat')
        self.addControl('offset')
        self.addControl('rotate')
        self.addControl('pivot')
        self.addControl('coverage')
        self.endLayout()

        self.beginLayout('UV Modifier', collapse=False)
        self.addControl('noise')
        self.addControl('mirrorU')
        self.addControl('mirrorV')
        self.addControl('flipU')
        self.addControl('flipV')
        self.addControl('swapUv')
        self.addControl('stagger')
        self.endLayout()
        self.addControl('unit')

        pm.mel.AEdependNodeTemplate(self.nodeName)
        
        self.addExtraControls()
        self.endScrollLayout()

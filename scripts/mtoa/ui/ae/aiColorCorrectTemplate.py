import pymel.core as pm
from mtoa.ui.ae.shaderTemplate import ShaderAETemplate

import maya.mel
from mtoa.ui.ae.shaderTemplate import ShaderAETemplate

class AEaiColorCorrectTemplate(ShaderAETemplate):
    def setup(self):
        self.addSwatch()
        self.beginScrollLayout()

        self.addControl('input', label='Input')
        self.addControl('mask', label='Mask')
        self.beginLayout('Color Correction', collapse=False)

        self.addControl('gamma', label='Gamma')
        self.addControl('hueShift', label='Hue Shift')
        self.addControl('saturation', label='Saturation')
        self.addControl('contrast', label='Contrast')
        self.addControl('contrastPivot', label='Contrast Pivot')
        self.addControl('exposure', label='Exposure')
        self.addControl('multiply', label='Multiply')
        self.addControl('add', label='Add')
        self.addControl('invert', label='Invert')
        self.endLayout()

        self.beginLayout('Alpha', collapse=False)
        self.addControl('alphaIsLuminance', label='Is Luminance')
        self.addControl('alphaMultiply', label='Multiply')
        self.addControl('alphaAdd', label='Add')
        self.addControl('invertAlpha', label='Invert')
        self.endLayout()

        maya.mel.eval('AEdependNodeTemplate '+self.nodeName)

        self.addExtraControls()
        self.endScrollLayout()


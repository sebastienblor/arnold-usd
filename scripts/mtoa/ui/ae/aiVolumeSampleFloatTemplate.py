import pymel.core as pm
from mtoa.ui.ae.shaderTemplate import ShaderAETemplate

class AEaiVolumeSampleFloatTemplate(ShaderAETemplate):
    def setup(self):
        self.beginScrollLayout()
        
        self.beginLayout('Volume Sample Float Attributes', collapse=False)
        self.addControl('channel')
        self.addControl('positionOffset')
        self.addControl('interpolation')
        self.addControl('volumeType')
        self.endLayout()

        self.beginLayout('Signed Distance Function', collapse=False)
        self.addControl('sdfOffset', label='Offset')
        self.addControl('sdfBlend', label='Blend')
        self.addControl('sdfInvert', label='Invert')
        self.endLayout()

        
        self.beginLayout('Remap Attributes', collapse=False)
        self.addControl('inputMin')
        self.addControl('inputMax')
        self.addControl('contrast')
        self.addControl('contrastPivot')
        self.addControl('bias')
        self.addControl('gain')
        self.addControl('outputMin')
        self.addControl('outputMax')
        self.addControl('clampMin')
        self.addControl('clampMax')
        self.endLayout()
        
        pm.mel.AEdependNodeTemplate(self.nodeName)
        
        self.addExtraControls()
        self.endScrollLayout()


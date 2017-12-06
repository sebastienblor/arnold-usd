import maya.mel
from mtoa.ui.ae.shaderTemplate import ShaderAETemplate

class AEaiVolumeSampleRgbTemplate(ShaderAETemplate):
    def setup(self):
        self.beginScrollLayout()
        
        self.beginLayout('Volume Sample RGB Attributes', collapse=False)
        self.addControl('channel')
        self.addControl('positionOffset')
        self.addControl('interpolation')
        self.endLayout()
        
        self.beginLayout('Color Correction Attributes', collapse=False)
        
        self.addControl('gamma')
        self.addControl('hueShift')
        self.addControl('saturation')
        self.addControl('contrast')
        self.addControl('contrastPivot')
        self.addControl('exposure')
        self.addControl('multiply')
        self.addControl('add')

        self.endLayout()
        
        maya.mel.eval('AEdependNodeTemplate '+self.nodeName)
        
        self.addExtraControls()
        self.endScrollLayout()

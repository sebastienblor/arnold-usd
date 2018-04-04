import maya.mel
from mtoa.ui.ae.shaderTemplate import ShaderAETemplate

class AEaiFloatToRgbaTemplate(ShaderAETemplate):
    def setup(self):
    	self.addSwatch()
        self.beginScrollLayout()

        self.addControl('r')
        self.addControl('g')
        self.addControl('b')
        self.addControl('a')
        
        maya.mel.eval('AEdependNodeTemplate '+self.nodeName)
        
        self.addExtraControls()
        self.endScrollLayout()

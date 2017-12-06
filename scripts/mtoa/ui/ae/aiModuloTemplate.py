import maya.mel
from mtoa.ui.ae.shaderTemplate import ShaderAETemplate

class AEaiModuloTemplate(ShaderAETemplate):
    def setup(self):
    	self.addSwatch()
        self.beginScrollLayout()

        self.addControl('input')
        self.addControl('divisor')
        
        maya.mel.eval('AEdependNodeTemplate '+self.nodeName)
        
        self.addExtraControls()
        self.endScrollLayout()

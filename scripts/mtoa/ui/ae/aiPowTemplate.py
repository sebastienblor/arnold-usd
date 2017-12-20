import maya.mel
from mtoa.ui.ae.shaderTemplate import ShaderAETemplate

class AEaiPowTemplate(ShaderAETemplate):
    def setup(self):
    	self.addSwatch()
       
        self.beginScrollLayout()

        self.addControl('base')
        self.addControl('exponent')
        
        maya.mel.eval('AEdependNodeTemplate '+self.nodeName)
        
        self.addExtraControls()
        self.endScrollLayout()

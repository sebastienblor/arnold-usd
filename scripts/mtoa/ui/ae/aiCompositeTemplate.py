import maya.mel
from mtoa.ui.ae.shaderTemplate import ShaderAETemplate

class AEaiCompositeTemplate(ShaderAETemplate):
    def setup(self):
    	self.addSwatch()
        self.beginScrollLayout()
        
        self.addControl('A')
        self.addControl('B')
        self.addControl('operation')
        self.addControl('alphaOperation')
        
        maya.mel.eval('AEdependNodeTemplate '+self.nodeName)
        
        self.addExtraControls()
        self.endScrollLayout()

import maya.mel
from mtoa.ui.ae.shaderTemplate import ShaderAETemplate

class AEaiComplexIorTemplate(ShaderAETemplate):
    def setup(self):
        self.addSwatch()
        self.beginScrollLayout()
        
        self.addControl('material')
        self.addControl('mode')
        self.addControl('reflectivity')
        self.addControl('edgetint', label='Edge Tint')
        self.addControl('n')
        self.addControl('k')

        maya.mel.eval('AEdependNodeTemplate '+self.nodeName)
        
        self.addExtraControls()
        self.endScrollLayout()

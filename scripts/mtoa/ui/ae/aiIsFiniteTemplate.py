import maya.mel
from mtoa.ui.ae.shaderTemplate import ShaderAETemplate

class AEaiIsFiniteTemplate(ShaderAETemplate):
    def setup(self):
        self.addSwatch()
        self.beginScrollLayout()

        self.addControl('input')
        
        maya.mel.eval('AEdependNodeTemplate '+self.nodeName)
        
        self.addExtraControls()
        self.endScrollLayout()
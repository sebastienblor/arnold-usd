import maya.mel
from mtoa.ui.ae.shaderTemplate import ShaderAETemplate

class AEaiDivideTemplate(ShaderAETemplate):
    def setup(self):
        self.addSwatch()
        self.beginScrollLayout()

        self.addControl('input1')
        self.addControl('input2')
        
        maya.mel.eval('AEdependNodeTemplate '+self.nodeName)
        
        self.addExtraControls()
        self.endScrollLayout()

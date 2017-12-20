import maya.mel
from mtoa.ui.ae.shaderTemplate import ShaderAETemplate

class AEaiCompareTemplate(ShaderAETemplate):
    def setup(self):
        self.addSwatch()
        self.beginScrollLayout()
        
        self.addControl('test', label='Test')
        self.addControl('input1', label='Input 1')
        self.addControl('input2', label='Input 2')
        
        maya.mel.eval('AEdependNodeTemplate '+self.nodeName)

        self.addExtraControls()
        self.endScrollLayout()


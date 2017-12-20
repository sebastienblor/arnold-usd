import maya.mel
from mtoa.ui.ae.shaderTemplate import ShaderAETemplate

class AEaiBlackbodyTemplate(ShaderAETemplate):
    def setup(self):
        self.addSwatch()
        self.beginScrollLayout()
        
        self.addControl('temperature', label='Temperature')
        self.addControl('normalize', label='Normalize')
        self.addControl('intensity', label='Intensity')
        
        maya.mel.eval('AEdependNodeTemplate '+self.nodeName)

        self.addExtraControls()
        self.endScrollLayout()


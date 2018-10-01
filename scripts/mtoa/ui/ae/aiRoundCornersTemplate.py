import maya.mel
from mtoa.ui.ae.shaderTemplate import ShaderAETemplate

class AEaiRoundCornersTemplate(ShaderAETemplate):
    def setup(self):
        self.beginScrollLayout()
        
        self.addControl('samples')
        self.addControl('radius')
        self.addControl('normal')
        self.addControl('traceSet')
        self.addControl('inclusive')
        self.addControl('selfOnly')
        
        maya.mel.eval('AEdependNodeTemplate '+self.nodeName)
        
        self.addExtraControls()
        self.endScrollLayout()

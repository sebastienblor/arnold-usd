import maya.mel
from mtoa.ui.ae.shaderTemplate import ShaderAETemplate

class AEaiRoundCornersTemplate(ShaderAETemplate):
    def setup(self):
        self.beginScrollLayout()
        
        self.addControl('samples')
        self.addControl('radius')
        self.addControl('traceSet', label='Trace Set *') # Not supported in GPU
        self.addControl('inclusive', label='Inclusive *') # Not supported in GPU
        self.addControl('selfOnly')
        self.addControl('normal', label='Normal')
        
        maya.mel.eval('AEdependNodeTemplate '+self.nodeName)
        
        self.addExtraControls()
        self.endScrollLayout()

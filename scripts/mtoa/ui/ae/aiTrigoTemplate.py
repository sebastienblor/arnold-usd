import pymel.core as pm
from mtoa.ui.ae.shaderTemplate import ShaderAETemplate

class AEaiTrigoTemplate(ShaderAETemplate):
    def setup(self):
        self.addSwatch()
        self.beginScrollLayout()
        
        self.addControl('input', label='Input')
        self.addControl('function', label='Function')
        self.addControl('units', label='Units')
        self.addControl('frequency', label='Frequency')
        self.addControl('phase', label='Phase')
                
        pm.mel.AEdependNodeTemplate(self.nodeName)

        self.addExtraControls()
        self.endScrollLayout()


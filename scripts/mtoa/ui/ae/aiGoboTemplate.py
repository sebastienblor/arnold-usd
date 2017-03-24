import pymel.core as pm
from mtoa.ui.ae.shaderTemplate import ShaderAETemplate

class AEaiGoboTemplate(ShaderAETemplate):
    def setup(self):
        self.addSwatch()
        self.beginScrollLayout()
        
        self.beginLayout('Gobo Attributes', collapse=False)
        self.addControl('filterMode', label='Filter Mode')
        self.addControl('slidemap', label='Slide Map')
        self.addControl('density', label='Density')
        self.endLayout()
        
        self.beginLayout('Placement Attributes', collapse=False)
        self.addControl('offset', label='Offset')
        self.addControl('rotate', label='Rotate')
        self.addControl('scaleS', label='Scale S')
        self.addControl('scaleT', label='Scale T')
        self.addControl('wrapS', label='Wrap S')
        self.addControl('wrapT', label='Wrap T')
        self.endLayout()
        
        pm.mel.AEdependNodeTemplate(self.nodeName)

        self.addExtraControls()
        self.endScrollLayout()

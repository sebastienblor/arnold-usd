import maya.mel
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
        self.addControl('sscale', label='Scale S')
        self.addControl('tscale', label='Scale T')
        self.addControl('swrap', label='Wrap S')
        self.addControl('twrap', label='Wrap T')
        self.endLayout()
        
        maya.mel.eval('AEdependNodeTemplate '+self.nodeName)

        self.addExtraControls()
        self.endScrollLayout()

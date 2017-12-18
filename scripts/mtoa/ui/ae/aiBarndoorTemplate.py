import maya.mel
from mtoa.ui.ae.shaderTemplate import ShaderAETemplate

class AEaiBarndoorTemplate(ShaderAETemplate):
    def setup(self):
        self.addSwatch()
        
        self.beginScrollLayout()
        
        self.beginLayout('Barndoor Attributes', collapse=False)
        self.addControl('barndoorTopLeft', label='Top Left')
        self.addControl('barndoorTopRight', label='Top Right')
        self.addControl('barndoorTopEdge', label='Top Edge')        
        self.addSeparator()
        self.addControl('barndoorBottomLeft', label='Bottom Left')
        self.addControl('barndoorBottomRight', label='Bottom Right')
        self.addControl('barndoorBottomEdge', label='Bottom Edge')
        self.addSeparator()
        self.addControl('barndoorLeftTop', label='Left Top')
        self.addControl('barndoorLeftBottom', label='Left Bottom')
        self.addControl('barndoorLeftEdge', label='Left Edge')
        self.addSeparator()
        self.addControl('barndoorRightTop', label='Right Top')
        self.addControl('barndoorRightBottom', label='Right Bottom')
        self.addControl('barndoorRightEdge', label='Right Edge')
        self.endLayout()
        
        maya.mel.eval('AEdependNodeTemplate '+self.nodeName)
        
        self.addExtraControls()
        self.endScrollLayout()


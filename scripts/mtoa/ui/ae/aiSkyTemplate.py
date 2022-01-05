import maya.mel
from mtoa.ui.ae.shaderTemplate import ShaderAETemplate

class AEaiSkyTemplate(ShaderAETemplate):
    def setup(self):
        self.addSwatch()
        self.beginScrollLayout()
        
        self.beginLayout('Sky', collapse=False)
        self.addControl('format', label='Format')
        self.addControl('color', label='Color')
        self.addControl('intensity', label='Intensity')
        self.endLayout()
        self.beginLayout('Transform', collapse=True)
        self.addControl('X')
        self.addControl('Y')
        self.addControl('Z')
        self.addControl('X_angle')
        self.addControl('Y_angle')
        self.addControl('Z_angle')
        self.endLayout()
        
        maya.mel.eval('AEdependNodeTemplate '+self.nodeName)

        self.addExtraControls()
        self.endScrollLayout()


import maya.mel
from mtoa.ui.ae.shaderTemplate import ShaderAETemplate

class AEaiNormalMapTemplate(ShaderAETemplate):
    def setup(self):
        self.addSwatch()
        self.beginScrollLayout()
        
        self.addControl('input')
        self.addControl('strength')
        
        self.addControl('tangent')
        self.addControl('normal')
        self.addControl('order')
        self.addControl('invertX')
        self.addControl('invertY')
        self.addControl('invertZ')
        self.addControl('colorToSigned')
        self.addControl('tangentSpace')
                
        maya.mel.eval('AEdependNodeTemplate '+self.nodeName)
        
        self.addExtraControls()
        self.endScrollLayout()
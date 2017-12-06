import maya.mel
from mtoa.ui.ae.shaderTemplate import ShaderAETemplate

class AEaiVectorMapTemplate(ShaderAETemplate):
    def setup(self):
        self.addSwatch()
        self.beginScrollLayout()

        self.addControl('input')
        self.addControl('tangent')
        self.addSeparator()
        self.addControl('normal')
        self.addControl('order')
        self.addSeparator()
        self.addControl('invertX')
        self.addControl('invertY')
        self.addControl('invertZ')
        self.addControl('colorToSigned')
        self.addControl('tangentSpace')
        self.addControl('scale')

        maya.mel.eval('AEdependNodeTemplate '+self.nodeName)
        
        self.addExtraControls()
        self.endScrollLayout()

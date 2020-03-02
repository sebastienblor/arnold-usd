import maya.mel
from mtoa.ui.ae.shaderTemplate import ShaderAETemplate

class AEaiCheckerboardTemplate(ShaderAETemplate):
    def setup(self):
        self.addSwatch()
        self.beginScrollLayout()

        self.addControl('color1')
        self.addControl('color2')

        self.addControl('uFrequency')
        self.addControl('vFrequency')
        self.addControl('uOffset')
        self.addControl('vOffset')
        self.addControl('contrast')
        self.addControl('filterStrength')
        self.addControl('filterOffset')
        self.addControl('uvset')     
                
        maya.mel.eval('AEdependNodeTemplate '+self.nodeName)
        
        self.addExtraControls()
        self.endScrollLayout()

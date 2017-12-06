import maya.mel
from mtoa.ui.ae.shaderTemplate import ShaderAETemplate

class AEaiRangeTemplate(ShaderAETemplate):
    def setup(self):
        self.addSwatch()
        self.beginScrollLayout()
        
        self.addControl('input')
        self.addControl('inputMin', label='Input Min')
        self.addControl('inputMax', label='Input Max')
        self.addControl('outputMin', label='Output Min')
        self.addControl('outputMax', label='Output Max')
        self.addControl('smoothstep')        
        
        maya.mel.eval('AEdependNodeTemplate '+self.nodeName)
        
        self.addExtraControls()
        self.endScrollLayout()

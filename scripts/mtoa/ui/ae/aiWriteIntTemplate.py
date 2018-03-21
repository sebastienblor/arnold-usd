import maya.mel
from mtoa.ui.ae.shaderTemplate import ShaderAETemplate

class AEaiWriteIntTemplate(ShaderAETemplate):

    def setup(self):
        self.beginScrollLayout()
        self.beginLayout("Write Int Attributes", collapse=False)

        self.addControl("beauty")
        self.addControl("input")
        self.addAOVControl('aovName')
        
        self.endLayout()

        maya.mel.eval('AEdependNodeTemplate '+self.nodeName)
        self.addExtraControls()

        self.endScrollLayout()


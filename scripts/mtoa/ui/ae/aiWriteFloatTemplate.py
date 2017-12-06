import maya.mel
from mtoa.ui.ae.shaderTemplate import ShaderAETemplate

class AEaiWriteFloatTemplate(ShaderAETemplate):

    def setup(self):
        self.beginScrollLayout()
        self.beginLayout("Write Color Attributes", collapse=False)

        self.addControl("beauty")
        self.addControl("input")
        self.addAOVControl('aovName')
        self.addControl("blendOpacity")

        self.endLayout()

        maya.mel.eval('AEdependNodeTemplate '+self.nodeName)
        self.addExtraControls()

        self.endScrollLayout()


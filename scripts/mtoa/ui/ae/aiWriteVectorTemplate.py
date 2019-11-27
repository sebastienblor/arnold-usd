import maya.mel
from mtoa.ui.ae.shaderTemplate import ShaderAETemplate

class AEaiWriteVectorTemplate(ShaderAETemplate):

    def setup(self):
        self.addSwatch()
        self.beginScrollLayout()
        self.beginLayout("Write Vector Attributes", collapse=False)

        self.addControl("beauty")
        self.addControl("input")
        self.addAOVControl('aovName')
        self.addControl("blend")

        self.endLayout()

        maya.mel.eval('AEdependNodeTemplate '+self.nodeName)
        self.addExtraControls()

        self.endScrollLayout()


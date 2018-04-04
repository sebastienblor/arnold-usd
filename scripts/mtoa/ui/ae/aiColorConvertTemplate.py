import maya.mel
from mtoa.ui.ae.shaderTemplate import ShaderAETemplate

class AEaiColorConvertTemplate(ShaderAETemplate):
    def setup(self):
        self.addSwatch()
        self.beginScrollLayout()

        self.addControl('input', label='Input')
        self.addControl('from', label='From')
        self.addControl('to', label='To')

        maya.mel.eval('AEdependNodeTemplate '+self.nodeName)

        self.addExtraControls()
        self.endScrollLayout()


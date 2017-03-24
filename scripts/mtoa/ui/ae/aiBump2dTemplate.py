import pymel.core as pm
from mtoa.ui.ae.shaderTemplate import ShaderAETemplate

class AEaiBump2dTemplate(ShaderAETemplate):
    def setup(self):
        self.addSwatch()
        self.beginScrollLayout()

        self.addControl('bumpMap', label='Bump Map')
        self.addControl('bumpHeight', label='Bump Height')

        self.addSeparator()
        self.addControl('normal', label='Normal')
        pm.mel.AEdependNodeTemplate(self.nodeName)

        self.addExtraControls()
        self.endScrollLayout()


import maya.mel
from mtoa.ui.ae.shaderTemplate import ShaderAETemplate

class AEaiSpaceTransformTemplate(ShaderAETemplate):
    def setup(self):
        self.addSwatch()
        self.beginScrollLayout()

        self.addControl('input')
        self.addControl('type')
        self.addSeparator()
        self.addControl('from')
        self.addControl('to')
        self.addSeparator()
        self.addControl('tangent')
        self.addControl('normal')
        self.addSeparator()
        self.addControl('normalize')
        self.addControl('scale')

        maya.mel.eval('AEdependNodeTemplate '+self.nodeName)
        
        self.addExtraControls()
        self.endScrollLayout()

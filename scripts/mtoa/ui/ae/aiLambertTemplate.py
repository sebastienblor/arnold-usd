import maya.mel
from mtoa.ui.ae.shaderTemplate import ShaderAETemplate

class AEaiLambertTemplate(ShaderAETemplate):
    def setup(self):
        self.addSwatch()
        self.beginScrollLayout()
        self.addCustom('message', 'AEshaderTypeNew', 'AEshaderTypeReplace')
        self.addControl('Kd')
        self.addControl('KdColor')
        self.addControl('opacity')
        self.addCustom("normalCamera", self.bumpNew, self.bumpReplace)
        maya.mel.eval('AEdependNodeTemplate '+self.nodeName)
        self.addExtraControls()
        self.endScrollLayout()

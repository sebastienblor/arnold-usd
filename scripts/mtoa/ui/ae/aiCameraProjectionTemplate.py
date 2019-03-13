import maya.mel
from mtoa.ui.ae.shaderTemplate import ShaderAETemplate

class AEaiCameraProjectionTemplate(ShaderAETemplate):

    def setup(self):
        self.beginScrollLayout()
        self.addControl('projectionColor')
        self.addControl('offscreenColor')
        self.addControl('mask')
        self.addControl('aspectRatio')
        self.addControl('frontFacing')
        self.addControl('backFacing')
        self.addControl('useShadingNormal')
        self.addControl('coordSpace')
        self.addControl('prefName')
        self.addControl('P')
        self.endLayout()

        maya.mel.eval('AEdependNodeTemplate '+self.nodeName)
        self.addExtraControls()
        self.endScrollLayout()

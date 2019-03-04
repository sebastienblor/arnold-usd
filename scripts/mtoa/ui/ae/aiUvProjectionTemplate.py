import maya.mel
from mtoa.ui.ae.shaderTemplate import ShaderAETemplate

class AEaiUvProjectionTemplate(ShaderAETemplate):
    def setup(self):
        self.addSwatch()
        self.beginScrollLayout()

        self.addControl('projectionColor')
        self.addControl('projectionType')
        self.addControl('coordSpace')
        self.addControl('prefName')
        self.addControl('P')
        self.addControl('uAngle')
        self.addControl('vAngle')
        self.addControl('matrix')
        
        maya.mel.eval('AEdependNodeTemplate '+self.nodeName)
        
        self.addExtraControls()
        self.endScrollLayout()

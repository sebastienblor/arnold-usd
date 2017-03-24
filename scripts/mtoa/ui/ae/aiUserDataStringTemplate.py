import pymel.core as pm
from mtoa.ui.ae.shaderTemplate import ShaderAETemplate

class AEaiUserDataStringTemplate(ShaderAETemplate):
    def setup(self):
        self.beginScrollLayout()

        self.addControl('stringAttrName',label='Attribute')
        self.addControl('defaultValue')

        pm.mel.AEdependNodeTemplate(self.nodeName)
        
        self.addExtraControls()
        self.endScrollLayout()

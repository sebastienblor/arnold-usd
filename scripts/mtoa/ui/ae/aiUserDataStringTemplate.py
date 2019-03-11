import maya.mel
from mtoa.ui.ae.shaderTemplate import ShaderAETemplate

class AEaiUserDataStringTemplate(ShaderAETemplate):
    def setup(self):
        self.addSwatch()
        self.beginScrollLayout()

        self.addControl('stringAttrName',label='Attribute')
        self.addControl('defaultValue')

        maya.mel.eval('AEdependNodeTemplate '+self.nodeName)
        
        self.addExtraControls()
        self.endScrollLayout()

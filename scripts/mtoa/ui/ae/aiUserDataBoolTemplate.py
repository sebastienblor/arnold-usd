import maya.mel
from mtoa.ui.ae.shaderTemplate import ShaderAETemplate

class AEaiUserDataBoolTemplate(ShaderAETemplate):
    def setup(self):
        self.beginScrollLayout()

        self.addControl('boolAttrName',label='Attribute')
        self.addControl('defaultValue')

        maya.mel.eval('AEdependNodeTemplate '+self.nodeName)
        
        self.addExtraControls()
        self.endScrollLayout()

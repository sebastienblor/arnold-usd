import maya.mel
from mtoa.ui.ae.shaderTemplate import ShaderAETemplate

class AEaiUserDataFloatTemplate(ShaderAETemplate):
    def setup(self):
        self.beginScrollLayout()

        self.addControl('floatAttrName', label='Attribute')
        self.addControl('defaultValue')

        maya.mel.eval('AEdependNodeTemplate '+self.nodeName)
        
        self.addExtraControls()
        self.endScrollLayout()

import maya.mel
from mtoa.ui.ae.shaderTemplate import ShaderAETemplate

class AEaiUserDataVectorTemplate(ShaderAETemplate):
    def setup(self):
    	
        self.beginScrollLayout()

        self.addControl('vectorAttrName',label='Attribute')
        self.addControl('defaultValue')

        maya.mel.eval('AEdependNodeTemplate '+self.nodeName)
        
        self.addExtraControls()
        self.endScrollLayout()

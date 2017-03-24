import pymel.core as pm
from mtoa.ui.ae.shaderTemplate import ShaderAETemplate

class AEaiUserDataVectorTemplate(ShaderAETemplate):
    def setup(self):
    	
        self.beginScrollLayout()

        self.addControl('vectorAttrName',label='Attribute')
        self.addControl('defaultValue')

        pm.mel.AEdependNodeTemplate(self.nodeName)
        
        self.addExtraControls()
        self.endScrollLayout()

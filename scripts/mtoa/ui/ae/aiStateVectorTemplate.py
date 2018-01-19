import maya.mel
from mtoa.ui.ae.shaderTemplate import ShaderAETemplate

class AEaiStateVectorTemplate(ShaderAETemplate):

    def setup(self):
    	self.beginScrollLayout()
        
        self.addControl("variable")
        
        maya.mel.eval('AEdependNodeTemplate '+self.nodeName)

        self.addExtraControls()

        self.endScrollLayout()


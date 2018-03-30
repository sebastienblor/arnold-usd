import maya.mel
from mtoa.ui.ae.shaderTemplate import ShaderAETemplate

class AEaiSwitchOperatorTemplate(ShaderAETemplate):

    def setup(self):
    	self.beginScrollLayout()
        
        self.beginLayout('Operator', collapse=False)
        self.addControl("enable")
        self.addControl("inputs")
        self.endLayout()
        self.beginLayout('Switch', collapse=False)
        self.addControl("index")
        self.endLayout()

        maya.mel.eval('AEdependNodeTemplate '+self.nodeName)

        self.addExtraControls()

        self.endScrollLayout()


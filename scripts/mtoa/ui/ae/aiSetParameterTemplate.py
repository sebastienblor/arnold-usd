import maya.mel
from mtoa.ui.ae.shaderTemplate import ShaderAETemplate

class AEaiSetParameterTemplate(ShaderAETemplate):

    def setup(self):
    	self.beginScrollLayout()
        
        self.beginLayout('Operator', collapse=False)
        self.addControl("enable")
        self.addControl("inputs")
        self.addControl("selection")
        self.endLayout()
        self.beginLayout('Set Parameter', collapse=False)
        self.addControl("assignment")
        self.endLayout()
        
        maya.mel.eval('AEdependNodeTemplate '+self.nodeName)

        self.addExtraControls()

        self.endScrollLayout()


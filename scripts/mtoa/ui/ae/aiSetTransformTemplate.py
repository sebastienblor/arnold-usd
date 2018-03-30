import maya.mel
from mtoa.ui.ae.shaderTemplate import ShaderAETemplate

class AEaiSetTransformTemplate(ShaderAETemplate):

    def setup(self):
    	self.beginScrollLayout()
        
        self.beginLayout('Operator', collapse=False)
        self.addControl("enable")
        self.addControl("inputs")
        self.addControl("selection")
        self.endLayout()
        self.beginLayout('Set Transfom', collapse=False)
        self.addControl("translate")
        self.addControl("rotate")
        self.addControl("scale")
        self.addControl("transform_order")
        self.addControl("rotate_order")
        self.addControl("mode")
        self.endLayout()

        maya.mel.eval('AEdependNodeTemplate '+self.nodeName)

        self.addExtraControls()

        self.endScrollLayout()


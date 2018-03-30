import maya.mel
from mtoa.ui.ae.shaderTemplate import ShaderAETemplate

class AEaiMaterialxTemplate(ShaderAETemplate):

    def setup(self):
    	self.beginScrollLayout()
        
        self.beginLayout('Operator', collapse=False)
        self.addControl("enable")
        self.addControl("inputs")
        self.addControl("selection")
        self.endLayout()
        self.beginLayout('MaterialX', collapse=False)
        self.addControl("mtlx")
        self.addControl("look")
        self.addControl("mtlx")
        self.addSeparator()
        self.addControl("assign_materials")
        self.addControl("assign_properties")
        self.addControl("assign_visibilities")
        self.endLayout()

        maya.mel.eval('AEdependNodeTemplate '+self.nodeName)

        self.addExtraControls()

        self.endScrollLayout()


import pymel.core as pm
from mtoa.ui.ae.shaderTemplate import ShaderAETemplate

class AEaiTriplanarTemplate(ShaderAETemplate):
    def setup(self):
        self.addSwatch()
        self.beginScrollLayout()
        
        self.addControl("input", label="Input")
        self.addSeparator()
        self.addControl("coordSpace")
        self.addControl("P")
        self.addControl("blend")

        self.beginLayout("Transform", collapse=False)
        self.addControl("scale", label="Scale")
        self.addControl("rotate", label="Rotate")
        self.addControl("offset", label="Offset")
        self.endLayout()
        
        pm.mel.AEdependNodeTemplate(self.nodeName)

        self.addExtraControls()
        self.endScrollLayout()
    

import pymel.core as pm
from mtoa.ui.ae.shaderTemplate import ShaderAETemplate

class AEaiNoiseTemplate(ShaderAETemplate):
    def setup(self):
        self.beginScrollLayout()
        
        self.addControl("octaves", label="Octaves")
        self.addControl("distortion", label="Distortion")
        self.addControl("lacunarity", label="Lacunarity")
        self.addControl("amplitude", label="Amplitude")
        self.addControl("scale", label="Scale")
        self.addControl("offset", label="Offset")
        self.addControl("coordSpace", label="Coord Space")
        
        pm.mel.AEdependNodeTemplate(self.nodeName)

        self.addExtraControls()
        self.endScrollLayout()
    

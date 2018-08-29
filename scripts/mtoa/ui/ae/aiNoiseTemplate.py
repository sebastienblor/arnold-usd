import maya.mel
from mtoa.ui.ae.shaderTemplate import ShaderAETemplate

class AEaiNoiseTemplate(ShaderAETemplate):
    def setup(self):
        self.addSwatch()
        self.beginScrollLayout()
        
        self.beginLayout("Noise Attributes", collapse=False)
    
        
        self.addControl("octaves", label="Octaves")
        self.addControl("distortion", label="Distortion")
        self.addControl("lacunarity", label="Lacunarity")
        self.addControl("amplitude", label="Amplitude")
        self.addControl("scale", label="Scale")
        self.addControl("offset", label="Offset")
        self.addControl("coordSpace", label="Coord Space")
        self.addControl("P", label="P")
        self.addControl("time", label="Time")
        self.addControl("color1", label="Color 1")
        self.addControl("color2", label="Color 2")
        self.addControl("mode", label="Mode")
        self.addControl("prefName", label="Pref Name")
        
        self.endLayout()
        
        maya.mel.eval('AEdependNodeTemplate '+self.nodeName)

        self.addExtraControls()
        self.endScrollLayout()
    

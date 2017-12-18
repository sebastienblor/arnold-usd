import maya.mel
from mtoa.ui.ae.shaderTemplate import ShaderAETemplate

class AEaiFogTemplate(ShaderAETemplate):
    def setup(self):
        self.addSwatch()
        self.beginScrollLayout()
        
        self.beginLayout('Fog Attributes', collapse=False)
        self.addControl('color', label='Color')
        self.addControl('distance', label='Distance')
        self.addControl('height', label='Height')
        self.addSeparator()
        self.addControl('groundNormal', label='Ground Normal')
        self.addControl('groundPoint', label='Ground Point')
        self.endLayout()
        
        maya.mel.eval('AEdependNodeTemplate '+self.nodeName)

        self.addExtraControls()
        self.endScrollLayout()


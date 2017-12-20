import maya.mel
from mtoa.ui.ae.shaderTemplate import ShaderAETemplate

class AEaiCarPaintTemplate(ShaderAETemplate):
    def setup(self):
        self.addSwatch()
        self.beginScrollLayout()
        
        self.addCustom('message', 'AEshaderTypeNew', 'AEshaderTypeReplace')
        
        self.beginLayout("Base", collapse=False)
        self.addControl("base",  label="Weight", annotation="Label Weight")
        self.addControl("base_color", label='Color', annotation="Label Color")
        self.addControl("base_roughness", label='Roughness', annotation="Label Roughness")
        self.endLayout()

        self.beginLayout("Specular", collapse=False)
        self.addControl("specular", label="Weight", annotation="Label Weight")
        self.addControl("specular_color", label='Color', annotation="Label Color")
        self.addControl("specular_flip_flop", label='Flip-Flop')
        self.addControl("specular_light_facing", label='Light Facing Color')
        self.addControl("specular_falloff", label='Falloff')
        self.addControl("specular_roughness", label='Roughness', annotation="Label Roughness")
        self.addControl("specular_IOR", label='IOR')
        self.addSeparator()
        self.addControl("transmission_color")
        self.beginLayout("Flakes", collapse=True)
        self.addControl("flake_color", label='Color', annotation="Label Color")
        self.addControl("flake_flip_flop", label='Flip-Flop')
        self.addControl("flake_light_facing", label='Light Facing Color')
        self.addControl("flake_falloff", label='Falloff')
        self.addControl("flake_roughness", label='Roughness', annotation="Label Roughness")
        self.addControl("flake_IOR", label='IOR')
        self.addControl("flake_scale", label='Scale')
        self.addControl("flake_density", label='Density')
        self.addControl("flake_layers", label='Layers')
        self.addControl("flake_normal_randomize", label='Normal Randomize')
        self.addControl("flake_coord_space", label='Coord Space')
        self.endLayout()
        self.endLayout()

        self.beginLayout("Coat", collapse=False)
        self.addControl("coat", label="Weight", annotation="Label Weight")
        self.addControl("coat_color", label='Color', annotation="Label Color")
        self.addControl("coat_roughness", label='Roughness', annotation="Label Roughness")
        self.addControl("coat_IOR", label='IOR')
        self.addControl("coat_normal", label='Normal')
        self.endLayout()

        maya.mel.eval('AEdependNodeTemplate '+self.nodeName)

        self.addExtraControls()
        self.endScrollLayout()
    

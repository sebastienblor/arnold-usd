import pymel.core as pm
from mtoa.ui.ae.shaderTemplate import ShaderAETemplate

class AEaiToonTemplate(ShaderAETemplate):
    def setup(self):
        self.addSwatch()

        self.beginScrollLayout()

        self.beginLayout("Edge", collapse=False)
        self.addControl("mask_color", label="Mask Color")
        self.addControl("edge_color", label="Edge Color")
        self.addControl("edge_opacity", label="Edge Opacity")
        self.addControl("width_scale", label="Width Scaling")
        self.addControl("priority", label="Priority")
        self.beginNoOptimize();
        self.addSeparator()
        self.addControl("id_difference", label="ID Difference")
        self.addControl("uv_threshold", label="UV Threshold")
        self.addControl("angle_threshold", label="Angle Threshold")
        self.addSeparator()
        self.addControl("disable", label="Disable Edge")
        self.addControl("ignore_throughput", label="Ignore Throughput")
        self.endNoOptimize();
        self.endLayout()

        self.beginLayout("Base", collapse=False)
        self.addControl("base",  label="Weight")
        self.addControl("base_color", label="Color")
        self.addControl("base_tonemap", label="Tonemap")
        self.endLayout()

        self.beginLayout("Specular", collapse=False)
        self.addControl("specular", label="Weight")
        self.addControl("specular_color", label="Color")
        self.addControl("specular_roughness", label="Roughness")
        self.addControl("specular_anisotropy", label="Anisotropy")
        self.addControl("specular_rotation", label="Rotation")
        self.addControl("specular_tonemap", label="Tonemap")
        self.addSeparator()
        self.addControl("normal", label='Normal')
        self.addControl("tangent", label='Tangent')
        self.endLayout()

        self.beginLayout("Stylized Highlight", collapse=True)
        self.addControl("light", label="Light")
        self.addControl("highlight_color", label="Color")
        self.addControl("highlight_size", label="Size")
        self.endLayout()

        self.beginLayout("Transmission", collapse=True)
        self.addControl("transmission", label="Weight")
        self.addControl("transmission_color", label="Color")
        self.addControl("transmission_roughness", label="Roughness")
        self.addControl("transmission_anisotropy", label="Anisotropy")
        self.addControl("transmission_rotation", label="Rotation")
        self.addControl("IOR", label='IOR')
        self.endLayout()

        self.beginLayout("Emission", collapse=False)
        self.addControl("emission", label="Weight")
        self.addControl("emission_color", label="Color")
        self.endLayout()

        self.beginLayout("Advanced", collapse=True)
        self.addControl("indirect_diffuse", label="Indirect Diffuse")
        self.addControl("indirect_specular", label="Indirect Specular")
        self.endLayout()

        pm.mel.AEdependNodeTemplate(self.nodeName)

        self.addExtraControls()
        self.endScrollLayout()
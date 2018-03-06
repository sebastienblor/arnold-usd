from mtoa.ui.ae.shaderTemplate import ShaderAETemplate
import maya.mel

class AEaiToonTemplate(ShaderAETemplate):
    def setup(self):
        self.addSwatch()
        self.beginScrollLayout()
        self.addCustom('message', 'AEshaderTypeNew', 'AEshaderTypeReplace')

        self.beginLayout("Edge", collapse=False)
        self.addControl("enable", label="Edge (requires contour filter)")
        self.addSeparator()
        self.addControl("edge_color", label="Edge Color")
        self.addControl("edge_tonemap", label="Edge Tonemap")
        self.addControl("edge_opacity", label="Edge Opacity")
        self.addControl("width_scale", label="Width Scaling")

        self.beginLayout("Edge Detection", collapse=False)
        self.beginNoOptimize();
        self.addControl("id_difference", label="ID Difference")
        self.addControl("shader_difference", label="Shader Difference")
        self.addControl("mask_color", label="Mask Color")
        self.addControl("uv_threshold", label="UV Threshold")
        self.addControl("angle_threshold", label="Angle Threshold")
        self.addControl("normal_type", label="Normal Type")
        self.endNoOptimize();
        self.endLayout()

        self.beginLayout("Advanced Edge Control", collapse=True)
        self.beginNoOptimize();
        self.addControl("priority", label="Priority")
        self.addControl("ignore_throughput", label="Ignore Throughput")
        self.endNoOptimize();
        self.endLayout()
        self.endLayout()

        self.beginLayout("Silhouette", collapse=False)
        self.addControl("enable_silhouette", label="Enable")
        self.addControl("silhouette_color", label="Color")
        self.addControl("silhouette_tonemap", label="Tonemap")
        self.addControl("silhouette_opacity", label="Opacity")
        self.addControl("silhouette_width_scale", label="Width Scale")
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
        self.endLayout()

        self.beginLayout("Stylized Highlight", collapse=True)
        self.addControl("light", label="Light")
        self.addControl("highlight_color", label="Color")
        self.addControl("highlight_size", label="Size")
        self.endLayout()

        self.beginLayout("Rim Lighting", collapse=True)
        self.addControl("rim_light", label="Light")
        self.addControl("rim_light_color", label="Color")
        self.addControl("rim_light_width", label="Width")
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

        self.beginLayout("Geometry", collapse=True)
        self.addControl("normal", label='Normal')
        self.addControl("tangent", label='Tangent')
        self.addCustom("normalCamera", self.bumpNew, self.bumpReplace)
        self.addControl("bump_mode", label="Bump Mapping Mode")
        self.endLayout()

        self.beginLayout("AOVs", collapse=True)
        self.addControl("highlight_aov", label="Highlight")
        self.addControl("rim_light_aov", label="Rim Light")
        self.endLayout()

        self.beginLayout("Advanced", collapse=True)
        self.addControl("indirect_diffuse", label="Indirect Diffuse")
        self.addControl("indirect_specular", label="Indirect Specular")
        self.addControl("energy_conserving", label="Energy Conserving")
        self.endLayout()

        maya.mel.eval('AEdependNodeTemplate '+self.nodeName)

        self.addExtraControls()
        self.endScrollLayout()
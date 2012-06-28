import pymel.core as pm
import mtoa.aovs as aovs
import mtoa.ui.ae.templates as templates
import mtoa.ui.ae.shaderTemplate as shaderTemplate
import mtoa.ui.aoveditor as aoveditor
from collections import defaultdict

class DisplacementShaderTemplate(templates.AttributeTemplate):
    def setup(self):
        self.addControl("aiDisplacementPadding", label="Bounds Padding")
        self.addControl("aiDisplacementZeroValue", label="Scalar Zero Value")
        self.addControl("aiDisplacementAutoBump", label="Auto Bump")

templates.registerAETemplate(DisplacementShaderTemplate, "displacementShader")


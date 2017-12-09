import mtoa.ui.ae.templates as templates
from mtoa.ui.ae.templates import AttributeTemplate, ShapeMixin, registerTranslatorUI
from mtoa.ui.ae.shaderTemplate import ShaderMixin

class ApiShapeTemplate(AttributeTemplate, ShapeMixin, ShaderMixin):
    def setup(self):
        self.commonShapeAttributes()
        self.addSeparator()
        self.addAOVLayout()

templates.registerTranslatorUI(ApiShapeTemplate, "apiMesh", "apiShape")
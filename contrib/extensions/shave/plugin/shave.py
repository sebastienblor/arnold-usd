import pymel.core as pm
import mtoa.ui.ae.templates as templates
from mtoa.ui.ae.templates import AttributeTemplate, ShapeMixin, registerTranslatorUI
from mtoa.ui.ae.shaderTemplate import ShaderMixin

class ShaveHairTemplate(AttributeTemplate, ShapeMixin, ShaderMixin):
    def setup(self):
        self.commonShapeAttributes()
        self.addSeparator()

        self.addControl("aiOverrideHair")
        self.addControl("aiHairShader")
    
        self.addSeparator()
    
        self.addControl("aiMinPixelWidth")
        self.addControl("aiMode")

        self.addSeparator()

        self.addControl("aiDiffuseCache")
        self.addControl("aiIndirect")
        self.addControl("aiDirectDiffuse")

        self.addAOVLayout()

templates.registerTranslatorUI(ShaveHairTemplate, "shaveHair", "shave")
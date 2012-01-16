import pymel.core as pm
from mtoa.ui.ae.shaderTemplate import ShaderAETemplate
import mtoa.ui.ae.shapeTemplate as templates
from mtoa.ui.ae.shapeTemplate import ShapeTranslatorTemplate

class ShaveTemplate(ShapeTranslatorTemplate):
    def setup(self):
        self.addControl("aiOverrideHair")
        self.addControl("aiHairShader")
    
        self.addSeparator()
    
        self.addControl("aiMinPixelWidth")
        self.addControl("aiMode")

templates.registerTranslatorUI(ShaveTemplate, "shaveHair", "shave")        
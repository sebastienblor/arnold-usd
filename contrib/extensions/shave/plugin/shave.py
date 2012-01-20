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

        self.addSeparator()

        self.addControl("aiDiffuseCache")
        self.addControl("aiIndirect")
        self.addControl("aiDirectDiffuse")

templates.registerTranslatorUI(ShaveTemplate, "shaveHair", "shave")
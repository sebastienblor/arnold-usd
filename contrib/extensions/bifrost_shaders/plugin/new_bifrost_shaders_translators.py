import mtoa.ui.ae.templates as templates
from mtoa.ui.ae.templates import AttributeTemplate

class BifrostHydroTemplate(AttributeTemplate):
    def setup(self):
        self.addControl("aiRRDepth", label = "Russian Roulette Depth");
        self.addControl("aiRoughnessDepthScale", label = "Roughness Depth Scale");

templates.registerAETemplate(BifrostHydroTemplate, 'bifrostHydro')

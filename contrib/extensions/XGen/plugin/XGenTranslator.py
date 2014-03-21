import pymel.core as pm
import mtoa.ui.ae.templates as templates
from mtoa.ui.ae.templates import AttributeTemplate, registerTranslatorUI


class xgmDescriptionTemplate(templates.ShapeTranslatorTemplate):
    def setup(self):
        self.commonShapeAttributes()
        self.addSeparator()
        self.addControl("motionBlurOverride", label="Motion Blur Override")
        self.addControl("motionBlurMode", label="Motion Blur Mode")
        self.addControl("motionBlurSteps", label="Motion_Blur_Steps")
        self.addControl("motionBlurFactor", label="Motion Blur Factor")
        self.addControl("motionBlurMult", label="Motion Blur Multiplier")
        self.addSeparator()
        self.addControl("aiMinPixelWidth", label="Min Pixel Width")
        self.addControl("aiMode", label= "Curve Mode")

templates.registerTranslatorUI(xgmDescriptionTemplate, "xgmDescription", "XGenTranslator")

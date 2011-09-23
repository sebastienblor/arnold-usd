import pymel.core as pm
import mtoa.ui.ae.lightTemplate as lightTemplate
import mtoa.ui.ae.shapeTemplate as templates

class AEaiAreaLightTemplate(templates.AttributeEditorTemplate):
    def setup(self):
        # why does this break template whereas it works with AEaiStandardTemplate ?
        # self.addSwatch()
        # parent seems wrong. Controls / widget don't use the AETemplate style either (too wide)
        
        self.beginScrollLayout()

        self.addControl("color")
        self.addControl("intensity")
        self.addControl("emitDiffuse")
        self.addControl("emitSpecular")
                    
        self.beginLayout("Arnold Area Light Attributes", collapse=False)
        self.addChildTemplate('aiTranslator', templates.getNodeTemplate('aiAreaLight'))
        self.endLayout()

        # include/call base class/node attributes
        pm.mel.AEdependNodeTemplate(self.nodeName)

        self.addExtraControls()
        self.endScrollLayout()

class BaseAreaLightTemplate(lightTemplate.LightTemplate):
    def validFilters(self):
        return ['aiLightBlocker', 'aiLightDecay']

    def setup(self):
        self.addControl("aiCastShadows")
        self.addControl("aiExposure")
        self.addControl("aiSamples")
        self.addControl("aiMis", label="Multiple Importance Sampling")

        self.addSeparator()

        self.addControl("aiAffectVolumetrics")
        self.addControl("aiCastVolumetricShadows")

        self.addSeparator()

        self.commonLightAttributes()

class QuadAreaLightTemplate(BaseAreaLightTemplate):
    def setup(self):
        self.addControl("aiResolution")
        self.addSeparator()
        super(QuadAreaLightTemplate, self).setup()

templates.registerAETemplate(templates.TranslatorControl, "aiAreaLight", label="Light Shape")
templates.registerTranslatorUI(QuadAreaLightTemplate, "aiAreaLight", "quad")
templates.registerTranslatorUI(BaseAreaLightTemplate, "aiAreaLight", "cylinder")
templates.registerTranslatorUI(BaseAreaLightTemplate, "aiAreaLight", "disk")

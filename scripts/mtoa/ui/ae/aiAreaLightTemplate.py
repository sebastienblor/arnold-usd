import pymel.core as pm
import mtoa.ui.ae.lightTemplate as lightTemplate
from mtoa.ui.ae.shaderTemplate import ShaderAETemplate
import mtoa.ui.ae.templates as templates

class AEaiAreaLightTemplate(ShaderAETemplate):
    def makeLightExclusive(self, attr):
        lightName = attr.split(".")[0]
        pm.rowLayout(nc=2, cal=[2, 'left'])
        pm.text(label="")
        pm.exclusiveLightCheckBox('exclusiveButton', light=lightName, label="Illuminates By Default")
        pm.setParent('..')
        
    def replaceLightExclusive(self, attr):
        lightName = attr.split(".")[0]
        pm.exclusiveLightCheckBox('exclusiveButton', edit=True, light=lightName)
    
    def setup(self):
        self.addSwatch()

        self.beginScrollLayout()

        self.beginLayout("Arnold Area Light Attributes", collapse=False)

        self.addControl("color")
        self.addControl("intensity")
        self.addControl("aiExposure", label = "Exposure")        
        self.addCustom("instObjGroups", self.makeLightExclusive, self.replaceLightExclusive)
        self.addControl("emitDiffuse")
        self.addControl("emitSpecular")
        self.addControl("aiDecayType")
        
        self.addChildTemplate('aiTranslator', templates.getNodeTemplate('aiAreaLight'))
        self.endLayout()

        # include/call base class/node attributes
        pm.mel.AEdependNodeTemplate(self.nodeName)

        self.addExtraControls()
        self.endScrollLayout()

class BaseAreaLightTemplate(lightTemplate.LightTemplate):
    def setup(self):
        self.addControl("aiSamples")
        self.addControl("aiNormalize")

        self.addSeparator()

        self.addControl("aiCastShadows")
        self.addControl("aiShadowDensity")
        self.addControl("aiShadowColor")

        self.addSeparator()
        
        self.addControl("aiAffectVolumetrics")
        self.addControl("aiCastVolumetricShadows")

        self.addSeparator()

        self.commonLightAttributes()

class MeshLightTemplate(lightTemplate.LightTemplate):
    def setup(self):
        self.addControl("aiSamples")
        self.addControl("aiNormalize")

        self.addSeparator()

        self.addControl("aiCastShadows")
        self.addControl("aiShadowDensity")
        self.addControl("aiShadowColor")

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
templates.registerTranslatorUI(MeshLightTemplate, "aiAreaLight", "mesh")

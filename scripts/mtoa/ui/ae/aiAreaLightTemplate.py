import pymel.core as pm
import mtoa.ui.ae.lightTemplate as lightTemplate
import mtoa.ui.ae.aiSwatchDisplay as aiSwatchDisplay
import mtoa.ui.ae.templates as templates

class AEaiAreaLightTemplate(lightTemplate.LightTemplate):
    def validFilters(self):
        return ['aiLightBlocker', 'aiLightDecay']

    def addSwatch(self):
        self.addCustom("message", aiSwatchDisplay.aiSwatchDisplayNew, aiSwatchDisplay.aiSwatchDisplayReplace)

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

        self.addSeparator()

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

        # get the TranslatorControl template that was registered on startup (below)
        self.addTemplate('aiTranslator',
                         templates.getNodeTemplate('aiAreaLight'))

        self.addSeparator()

        self.commonLightAttributes()

        #templates.TranslatorControl('aiAreaLight', label="Light Shape"))
        self.endLayout()

        # include/call base class/node attributes
        pm.mel.AEdependNodeTemplate(self.nodeName)

        self.addExtraControls()
        self.endScrollLayout()


class QuadAreaLightTemplate(templates.AttributeTemplate):
    def setup(self):
        self.addControl("aiResolution")

class MeshLightTemplate(templates.AttributeTemplate):
    def setup(self):
        self.addControl("aiCastShadows")
        self.addControl("aiShadowDensity")
        self.addControl("aiShadowColor")
        self.addControl("lightVisible")

templates.registerAETemplate(templates.TranslatorControl, "aiAreaLight", label="Light Shape")
templates.registerTranslatorUI(QuadAreaLightTemplate, "aiAreaLight", "quad")
templates.registerTranslatorUI(MeshLightTemplate, "aiAreaLight", "mesh")

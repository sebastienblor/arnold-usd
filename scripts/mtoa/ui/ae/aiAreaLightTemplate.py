import pymel.core as pm
import mtoa.ui.ae.lightTemplate as lightTemplate
import mtoa.ui.ae.aiSwatchDisplay as aiSwatchDisplay
import mtoa.ui.ae.templates as templates

class AEaiAreaLightTemplate(lightTemplate.LightTemplate):
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
        self.addSeparator()        
        self.setupColorTemperature("ArnoldArea")
        self.addCustom("instObjGroups", self.makeLightExclusive, self.replaceLightExclusive)
        self.addControl("emitDiffuse")
        self.addControl("emitSpecular")
        self.addControl("aiDecayType")
        
        self.addChildTemplate('aiTranslator', templates.getNodeTemplate('aiAreaLight'))
        
        self.addSeparator()

        self.commonLightAttributes()
        
        self.endLayout()

        # include/call base class/node attributes
        pm.mel.AEdependNodeTemplate(self.nodeName)

        self.addExtraControls()
        self.endScrollLayout()

class BaseAreaLightTemplate(lightTemplate.LightTemplate):
    def addCommonParameters(self):
        self.addControl("aiSamples")
        self.addControl("aiNormalize")

        self.addSeparator()

        self.addControl("aiCastShadows")
        self.addControl("aiShadowDensity")
        self.addControl("aiShadowColor")

    def setup(self):
        self.addCommonParameters()       
        
class QuadAreaLightTemplate(BaseAreaLightTemplate):
    def setup(self):
        self.addControl("aiResolution")
        self.addSeparator()
        self.addCommonParameters()

class MeshLightTemplate(BaseAreaLightTemplate):
    def setup(self):
        self.addControl("lightVisible")
        
        self.addSeparator()
        
        self.addCommonParameters()

class MeshLightMeshTemplate(BaseAreaLightTemplate):
    def setup(self):
        self.addControl("color")
        self.addControl("intensity")
        self.addControl("aiExposure", label = "Exposure")
        self.addSeparator()        
        self.setupColorTemperature("ArnoldArea")
        self.addControl("emitDiffuse")
        self.addControl("emitSpecular")
        self.addControl("aiDecayType")

        self.addControl("lightVisible")
        
        self.addSeparator()
        
        self.addCommonParameters()

        self.addSeparator()

        self.commonLightAttributes()


templates.registerAETemplate(templates.TranslatorControl, "aiAreaLight", label="Light Shape")
templates.registerTranslatorUI(QuadAreaLightTemplate, "aiAreaLight", "quad")
templates.registerTranslatorUI(BaseAreaLightTemplate, "aiAreaLight", "cylinder")
templates.registerTranslatorUI(BaseAreaLightTemplate, "aiAreaLight", "disk")
templates.registerTranslatorUI(MeshLightTemplate, "aiAreaLight", "mesh")
templates.registerDefaultTranslator('aiAreaLight', "quad")
templates.registerTranslatorUI(MeshLightMeshTemplate, "mesh", "mesh_light")

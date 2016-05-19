import pymel.core as pm
import mtoa.ui.ae.lightTemplate as lightTemplate
import mtoa.ui.ae.aiSwatchDisplay as aiSwatchDisplay
import mtoa.ui.ae.templates as templates

class AEaiMeshLightTemplate(lightTemplate.LightTemplate):

    def addSwatch(self):
        self.addCustom("message", aiSwatchDisplay.aiSwatchDisplayNew, aiSwatchDisplay.aiSwatchDisplayReplace)

    def setup(self):
        self.addSwatch()
        self.beginScrollLayout()
        self.beginLayout("Arnold Mesh Light Attributes", collapse=False)
        self.addControl("inMesh")
        self.addSeparator()
        self.addControl("color")
        self.addControl("intensity")
        self.addControl("aiExposure", label = "Exposure")
        self.addSeparator()
        self.setupColorTemperature("ArnoldMesh")
        self.addSeparator()
        self.addControl("emitDiffuse")
        self.addControl("emitSpecular")
        self.addControl("aiDecayType")
        self.addSeparator()
        self.addControl("lightVisible")
        self.addSeparator()
        self.addControl("aiSamples")
        self.addControl("aiNormalize")
        self.addSeparator()
        self.addControl("aiCastShadows")
        self.addControl("aiShadowDensity")
        self.addControl("aiShadowColor")
        self.addSeparator()
        self.commonLightAttributes()
        self.endLayout()

        # Do not show extra attributes
        extras = ["visibility",
                  "intermediateObject",
                  "template",
                  "ghosting",
                  "instObjGroups",
                  "useObjectColor",
                  "objectColor",
                  "drawOverride",
                  "lodVisibility",
                  "renderInfo",
                  "renderLayerInfo",
                  "ghostingControl",
                  "ghostCustomSteps",
                  "ghostFrames",
                  "ghostRangeStart",
                  "ghostRangeEnd",
                  "ghostDriver",
                  "ghostColorPreA",
                  "ghostColorPre",
                  "ghostColorPostA",
                  "ghostColorPost",
                  "motionBlur",
                  "visibleInReflections",
                  "visibleInRefractions",
                  "castsShadows",
                  "receiveShadows",
                  "maxVisibilitySamplesOverride",
                  "maxVisibilitySamples",
                  "geometryAntialiasingOverride",
                  "antialiasingLevel",
                  "shadingSamplesOverride",
                  "shadingSamples",
                  "maxShadingSamples",
                  "volumeSamplesOverride",
                  "volumeSamples",
                  "depthJitter",
                  "ignoreSelfShadowing",
                  "primaryVisibility",
                  "compInstObjGroups",
                  "localPosition",
                  "localScale"]

        for extra in extras:
            self.suppress(extra)
        
        pm.mel.AEdependNodeTemplate(self.nodeName)

        self.addExtraControls()
        self.endScrollLayout()

import pymel.core as pm
import mtoa.ui.ae.lightTemplate as lightTemplate
import mtoa.ui.ae.aiSwatchDisplay as aiSwatchDisplay
import mtoa.ui.ae.templates as templates

class AEaiMeshLightTemplate(lightTemplate.LightTemplate):

    def setup(self):
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

        # include/call base class/node attributes
        pm.mel.AEdependNodeTemplate(self.nodeName)

        self.addExtraControls()
        suppressList = ['aiShadowDensity', 'aiCastShadows', 'update',
            'aiSamples', 'aiNormalize', 'aiColorTemperature',
            'aiShadowColor', 'aiResolution', 'ghostFrames',
            'motionBlur', 'visibleInReflections', 'visibleInRefractions',
            'castsShadows', 'receiveShadows', 'maxVisibilitySamplesOverride',
            'maxVisibilitySamples', 'geometryAntialiasingOverride', 'antialiasingLevel',
            'shadingSamplesOverride', 'shadingSamples', 'maxShadingSamples',
            'volumeSamplesOverride', 'volumeSamples', 'layerRenderable',
            'ghostingControl', 'ghostCustomSteps', 'ghostColorPreA',
            'ghostColorPre', 'ghostColorPostA', 'ghostColorPost',
            'ghostRangeStart', 'ghostRangeEnd', 'ghostDriver',
            'depthJitter', 'ignoreSelfShadowing', 'primaryVisibility',
            'localPosition', 'localScale', 'pointCamera', 'normalCamera',
            'visibility', 'intermediateObject', 'template', 'ghosting',
            'objectColorRGB', 'useObjectColor', 'objectColor',
            'containerType', 'creationDate', 'creator',
            'customTreatment', 'uiTreatment', 'templateVersion',
            'viewMode', 'iconName', 'viewName', 'templatePath',
            'rmbCommand', 'blackBox', 'drawOverride',
            'renderInfo', 'renderLayerInfo', 'compInstObjGroups',
            'lodVisibility', 'templateName', 'selectionChildHighlighting']
        for sup in suppressList:
            self.suppress(sup)
        self.endScrollLayout()

templates.registerAETemplate(templates.TranslatorControl, "aiMeshLight", label="Light Shape")

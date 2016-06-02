import maya.cmds as cmds
import pymel.core as pm
import mtoa.ui.ae.lightTemplate as lightTemplate
import mtoa.ui.ae.aiSwatchDisplay as aiSwatchDisplay
import mtoa.ui.ae.templates as templates
import mtoa.utils as utils

def revertToMeshReplace(plugName):
    nodeAndAttrs = plugName.split(".")
    node = nodeAndAttrs[0]
    connected = cmds.listConnections('%s.inMesh' % node, shapes=True, type='mesh')
    enabled = (connected and len(connected) > 0)
    cmds.button("aiMeshLightRevertButton", edit=True, enable=enabled, command="import mtoa.utils as utils; utils.revertMeshLight(\"%s\")" % node)

def revertToMeshNew(plugName):
    cmds.button("aiMeshLightRevertButton", label="Revert to Mesh")
    revertToMeshReplace(plugName)

class AEaiMeshLightTemplate(lightTemplate.LightTemplate):

    def addSwatch(self):
        self.addCustom("message", aiSwatchDisplay.aiSwatchDisplayNew, aiSwatchDisplay.aiSwatchDisplayReplace)

    def setup(self):
        self.addSwatch()
        self.beginScrollLayout()

        self.beginLayout("Mesh Attributes", collapse=False)
        self.addControl("inMesh")
        self.addCustom("message", revertToMeshNew, revertToMeshReplace)
        self.endLayout()

        self.beginLayout("Light Attributes", collapse=False)
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

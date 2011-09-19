import maya.cmds as cmds
import maya.mel as mel
import mtoa.ui.ae.customShapeAttributes as customShapeAttributes
from mtoa.ui.ae.aiSwatchDisplay import aiSwatchDisplay
import mtoa.ui.ae.lightTemplate as lightTemplate

class AEaiAreaLightTemplate(lightTemplate.LightTemplate):
    def validFilters(self):
        return ['aiLightBlocker', 'aiLightDecay']

    def setup(self):
        self.addControl("aiCastShadows")
        self.addControl("aiExposure")
        self.addControl("aiSamples")
        self.addControl("aiMis", label="Multiple Importance Sampling")

        self.addSeparator()

        self.addControl("aiResolution")
        self.addControl("aiAffectVolumetrics")
        self.addControl("aiCastVolumetricShadows")

        self.addSeparator()

        #self.commonLightAttributes()

import maya.mel
import maya.cmds as cmds
from mtoa.ui.ae.shaderTemplate import ShaderAETemplate


class aiClipGeoTemplate(ShaderAETemplate):
    def setup(self):
        self.addSwatch()
        self.beginScrollLayout()

        self.beginLayout('Main Attributes', collapse=False)

        self.addControl('intersection', label='Intersection')
        self.addControl('trace_set', label='Trace Set')
        self.addControl('inclusive', label='Inclusive')
        self.endLayout()

        maya.mel.eval('AEdependNodeTemplate '+self.nodeName)

        self.addExtraControls()
        self.endScrollLayout()

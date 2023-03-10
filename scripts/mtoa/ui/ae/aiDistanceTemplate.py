import maya.mel
import maya.cmds as cmds
from mtoa.ui.ae.shaderTemplate import ShaderAETemplate
import arnold as ai

class AEaiDistanceTemplate(ShaderAETemplate):
    def setup(self):
        self.addSwatch()
        self.beginScrollLayout()

        self.addCustom('message', 'AEshaderTypeNew', 'AEshaderTypeReplace')

        self.beginLayout('Distance Attributes', collapse=False)
        print('DEBUG: AEaiDistanceTemplate::setup()')
        self.addControl('samples', label='Samples')
        self.addControl('distance', label='Distance')
        self.addControl('nearColor', label='Near Color')
        self.addControl('farColor', label='Far Color')
        self.addControl('insideSeparate', label='Inside Separate')
        self.addControl('insideColor', label='Inside Color')
        self.addControl('outsideSeparate', label='Outside Separate')
        self.addControl('outsideColor', label='Outside Color')
        self.addControl('traceSet', label='Trace Set')
        self.addControl('inclusive', label='Inclusive')
        self.endLayout()

        # include/call base class/node attributes
        maya.mel.eval('AEdependNodeTemplate '+self.nodeName)
        self.addExtraControls()

        self.endScrollLayout()

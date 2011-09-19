import pymel.core as pm
from mtoa.ui.ae.shaderTemplate import ShaderAETemplate

class AEaiWriteColorTemplate(ShaderAETemplate):

    def setup(self):
        #mel.eval('AEswatchDisplay "%s"' % nodeName)

        self.beginScrollLayout()
        self.beginLayout("Write Color Attributes", collapse=False)

        self.addControl("beauty")
        self.addControl("input")
        self.addAOVControl('aovName')

        self.endLayout()

        # include/call base class/node attributes
        pm.mel.AEdependNodeTemplate(self.nodeName)
        self.addExtraControls()

        self.endScrollLayout()


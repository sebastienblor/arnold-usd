import pymel.core as pm
#from mtoa.ui.ae.aiSwatchDisplay import aiSwatchDisplay
from mtoa.ui.ae.shaderTemplate import ShaderAETemplate

def AEaiUtilityTemplate(ShaderAETemplate):

    def setup(self):
        #aiSwatchDisplay(nodeName)
        self.beginScrollLayout()

        #cmds.editorTemplate('AEshaderTypeNew', 'AEshaderTypeReplace', "message", callCustom=True)

        self.beginLayout("Utility Attributes", collapse=False)
        self.addControl("shade_mode", label="Shade Mode")
        self.addControl("color_mode", label="Color Mode")
        self.addControl("color", label="Color")
        self.addControl("opacity", label="Opacity")
        self.endLayout()

        # include/call base class/node attributes
        pm.mel.AEdependNodeTemplate(self.nodeName)
        self.addExtraControls()

        self.endScrollLayout()
    

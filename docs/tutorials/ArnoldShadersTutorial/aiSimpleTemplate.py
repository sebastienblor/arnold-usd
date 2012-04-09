import pymel.core as pm
import mtoa.utils as utils
import mtoa.ui.ae.utils as aeUtils
from mtoa.ui.ae.shaderTemplate import ShaderAETemplate


class AEaiSimpleTemplate(ShaderAETemplate):

    def setup(self):
        self.addSwatch()

        self.beginScrollLayout()

        self.addCustom('message', 'AEshaderTypeNew',
                       'AEshaderTypeReplace')

        self.beginLayout("Color Section", collapse=False)
        self.addControl("constantColor",  label="Color",
                        annotation="Constant Color")
        
        self.endLayout()

        pm.mel.AEdependNodeTemplate(self.nodeName)

        self.addExtraControls()
        self.endScrollLayout()

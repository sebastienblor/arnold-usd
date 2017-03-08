import pymel.core as pm
from mtoa.ui.ae.shaderTemplate import ShaderAETemplate

class AEaiParametersShaderTemplate(ShaderAETemplate):
    def setup(self):
        self.addSwatch()
        self.beginScrollLayout()
        self.addCustom('message', 'AEshaderTypeNew',
                       'AEshaderTypeReplace')

        self.beginLayout("Parameters list", collapse=False)
        self.addControl("integer",  label="Integer Param")
        self.addControl("uinteger",  label="Unsigned Int Param")
        self.addControl("bool",  label="Bool param")
        self.addControl("float",  label="Float param")
        self.addControl("color",  label="Color param")
        self.addControl("vector",  label="Vector param")
        self.addControl("vector2",  label="2D Vector param")
        self.addControl("string",  label="String param")
        self.addControl("matrix",  label="Matrix param")
        self.addControl("enumeration",  label="Enumeration param")
        self.addControl("colorarray",  label="Color Array param")
        self.endLayout()

        pm.mel.AEdependNodeTemplate(self.nodeName)

        self.addExtraControls()
        self.endScrollLayout()
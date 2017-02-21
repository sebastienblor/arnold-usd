import pymel.core as pm
from mtoa.ui.ae.shaderTemplate import ShaderAETemplate

class AEaiMixShaderTemplate(ShaderAETemplate):
    def setup(self):
        self.addSwatch()
        self.beginScrollLayout()
        
        self.addCustom('message', 'AEshaderTypeNew', 'AEshaderTypeReplace')


        

        self.addControl('mode', label='Mix Mode', annotation="Mix Mode")
        self.addControl('mix', label='Mix Weight', annotation="Mix Weight")
        self.addControl('shader1', label='Shader1', annotation="Shader 1")
        self.addControl('shader2', label='Shader2', annotation="Shader 2")

        # include/call base class/node attributes
        pm.mel.AEdependNodeTemplate(self.nodeName)
        
        self.addExtraControls()
        self.endScrollLayout()



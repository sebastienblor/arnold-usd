import pymel.core as pm
from mtoa.ui.ae.shaderTemplate import ShaderAETemplate

class AEaiAmbientOcclusionTemplate(ShaderAETemplate):
    def setup(self):
        self.addSwatch()
        self.beginScrollLayout()

        self.addCustom('message', 'AEshaderTypeNew', 'AEshaderTypeReplace')

        self.beginLayout('Ambient Occlusion Attributes', collapse=False)
        self.addControl('samples', label='Samples')
        self.addControl('spread', label='Spread')
        self.addControl('falloff', label='Falloff')
        self.addControl('nearClip', label='Near Clip')
        self.addControl('farClip', label='Far Clip')
        self.addSeparator()
        self.addControl('white', label='White')
        self.addControl('black', label='Black')        
        self.addSeparator()
        self.beginNoOptimize()
        self.addControl('invertNormals', label='Invert Normals')
        self.addControl('selfOnly', label='Self Only')
        self.endNoOptimize()
        self.addControl('normalCamera', label='Normal')
        self.endLayout()

        pm.mel.AEdependNodeTemplate(self.nodeName)

        self.addExtraControls()
        self.endScrollLayout()


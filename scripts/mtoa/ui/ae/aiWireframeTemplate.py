import maya.mel
from mtoa.ui.ae.shaderTemplate import ShaderAETemplate

class AEaiWireframeTemplate(ShaderAETemplate):
    def setup(self):
        self.addSwatch()

        self.beginScrollLayout()

        self.addCustom('message', 'AEshaderTypeNew', 'AEshaderTypeReplace')

        self.beginLayout('Wireframe Attributes', collapse=False)
        self.addControl('edgeType', label='Edge Type')
        self.addControl('fillColor', label='Fill Color')
        self.addControl('lineColor', label='Line Color')
        self.addControl('lineWidth', label='Line Width')
        self.addControl('rasterSpace', label='Raster Space')
        self.endLayout()

        maya.mel.eval('AEdependNodeTemplate '+self.nodeName)
        self.addExtraControls()
        self.endScrollLayout()


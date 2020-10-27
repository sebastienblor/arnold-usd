import maya.cmds as cmds
import maya.mel
from mtoa.ui.ae.templates import TranslatorControl
from mtoa.ui.ae.shaderTemplate import ShaderAETemplate

class AEaiImagerTonemapTemplate(ShaderAETemplate):

    def updateParamsVisibility(self, nodeName):
        modeAttr = '%s.%s' % (nodeName, 'mode')
        modeValue = cmds.getAttr(modeAttr)
        cmds.editorTemplate(dimControl=(nodeName, 'filmicToeStrength', modeValue != 0))
        cmds.editorTemplate(dimControl=(nodeName, 'filmicToeLength', modeValue != 0))
        cmds.editorTemplate(dimControl=(nodeName, 'filmicShoulderStrength', modeValue != 0))
        cmds.editorTemplate(dimControl=(nodeName, 'filmicShoulderLength', modeValue != 0))
        cmds.editorTemplate(dimControl=(nodeName, 'filmicShoulderAngle', modeValue != 0))
        cmds.editorTemplate(dimControl=(nodeName, 'reinhardHighlights', modeValue != 1))
        cmds.editorTemplate(dimControl=(nodeName, 'reinhardShadows', modeValue != 1))
    
    def setup(self):
        #mel.eval('AEswatchDisplay "%s"' % nodeName)

        self.beginScrollLayout()
        self.addControl('enable', label='Enable', annotation='Enables this imager.')
        self.addSeparator()
        self.addControl('mode', label='Mode', changeCommand=self.updateParamsVisibility, annotation='The mode used to perform tonemapping (filmic, reinhard')
        self.beginLayout("Filmic", collapse=False)
        self.addControl('filmicToeStrength', label='Toe Strength', annotation='Amount of curvature for the tonemap curve in the darker values.')
        self.addControl('filmicToeLength', label='Toe Length', annotation='Amount of darker values affected by the toe.')
        self.addSeparator()
        self.addControl('filmicShoulderStrength', label='Shoulder Strength', annotation='Amount of curvature for the tonemap curve in the brighter values.')
        self.addControl('filmicShoulderLength', label='Shoulder Length', annotation='Amount in f-stops of brighter values affected by the shoulder.')
        self.addControl('filmicShoulderAngle', label='Shoulder Angle', annotation='Curve slope when white is reached.')
        self.endLayout()
        self.beginLayout("Reinhard", collapse=False)
        self.addControl('reinhardHighlights', label='Highlights', annotation='Reinhard photographic tonemap operator strength.')
        self.addControl('reinhardShadows', label='Shadows', annotation='Additional tonemapping control for darker values.')
        self.endLayout()
        self.addSeparator()
        self.addControl('preserveSaturation', label='Preserve Saturation', annotation='Preserves color saturation for extreme bright values.')
        self.addControl('gamma', label='Gamma', annotation='Gamma curve exponent for midtones value control.')
        maya.mel.eval('AEdependNodeTemplate '+self.nodeName)

        self.addExtraControls()
        self.endScrollLayout()

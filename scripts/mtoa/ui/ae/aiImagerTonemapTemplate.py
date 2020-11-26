import maya.cmds as cmds
import maya.mel
from mtoa.ui.ae.templates import TranslatorControl
from mtoa.ui.ae.shaderTemplate import ShaderAETemplate
from mtoa.ui.ae.aiImagersBaseTemplate import ImagerBaseUI


class AEaiImagerTonemapTemplate(ShaderAETemplate):

    def setup(self):

        self.beginScrollLayout()
        self.baseLayout = self.beginLayout("Main", collapse=False)
        currentWidget = cmds.setParent(query=True)
        self.ui = ImagerTonemapUI(parent=currentWidget, nodeName=self.nodeName, template=self)
        self.endLayout()
        maya.mel.eval('AEdependNodeTemplate '+self.nodeName)

        self.addExtraControls()
        self.endScrollLayout()


class ImagerTonemapUI(ImagerBaseUI):
    def __init__(self, parent=None, nodeName=None, template=None):
        super(ImagerTonemapUI, self).__init__(parent, nodeName, template)

    def setup(self):
        super(ImagerTonemapUI, self).setup()

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

        self.updateParamsVisibility(self.nodeName)

    def updateParamsVisibility(self, *args):
        modeAttr = '%s.%s' % (self.nodeName, 'mode')
        modeValue = cmds.getAttr(modeAttr)
        self.dimControl('filmicToeStrength', state=modeValue != 0)
        self.dimControl('filmicToeLength', state=modeValue != 0)
        self.dimControl('filmicShoulderStrength', state=modeValue != 0)
        self.dimControl('filmicShoulderLength', state=modeValue != 0)
        self.dimControl('filmicShoulderAngle', state=modeValue != 0)
        self.dimControl('reinhardHighlights', state=modeValue != 1)
        self.dimControl('reinhardShadows', state=modeValue != 1)

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
        self.addControl('enable', label='Enable')
        self.addSeparator()
        self.addControl('mode', label='Mode', changeCommand=self.updateParamsVisibility)
        self.beginLayout("Filmic", collapse=False)
        self.addControl('filmicToeStrength', label='Toe Strength')
        self.addControl('filmicToeLength', label='Toe Length')
        self.addSeparator()
        self.addControl('filmicShoulderStrength', label='Shoulder Strength')
        self.addControl('filmicShoulderLength', label='Shoulder Length')
        self.addControl('filmicShoulderAngle', label='Shoulder Angle')
        self.endLayout()
        self.beginLayout("Reinhard", collapse=False)
        self.addControl('reinhardHighlights', label='Highlights')
        self.addControl('reinhardShadows', label='Shadows')
        self.endLayout()
        self.addSeparator()
        self.addControl('preserveSaturation', label='Preserve Saturation')
        self.addControl('gamma', label='Gamma')
        maya.mel.eval('AEdependNodeTemplate '+self.nodeName)

        self.addExtraControls()
        self.endScrollLayout()

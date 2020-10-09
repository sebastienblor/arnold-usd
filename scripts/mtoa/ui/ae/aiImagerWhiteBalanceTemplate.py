import maya.mel
from mtoa.ui.ae.templates import TranslatorControl
from mtoa.ui.ae.shaderTemplate import ShaderAETemplate
import maya.cmds as cmds

class AEaiImagerWhiteBalanceTemplate(ShaderAETemplate):

    def updateParamsVisibility(self, nodeName):
        modeAttr = '%s.%s' % (nodeName, 'mode')
        modeValue = cmds.getAttr(modeAttr)
        cmds.editorTemplate(dimControl=(nodeName, 'temperature', modeValue != 1))
        cmds.editorTemplate(dimControl=(nodeName, 'illuminant', modeValue != 0))
        cmds.editorTemplate(dimControl=(nodeName, 'custom', modeValue != 2))

    def setup(self):
    
        self.beginScrollLayout()
        self.addControl('enable', label='Enable', annotation='Enables this imager.')
        self.addSeparator()
        self.addControl('mode', label='Mode', changeCommand=self.updateParamsVisibility, annotation='EWhite balance mode. (illuminant, temperature, custom)')
        self.addControl('illuminant', label='Illuminant', annotation='Temperature for black body mode.')
        self.addControl('temperature', label='Temperature', annotation='Standard illuminant name for illuminante mode.')
        self.addControl('custom', label='Custom', annotation='Custom white balance color.')
        maya.mel.eval('AEdependNodeTemplate '+self.nodeName)

        self.addExtraControls()
        self.endScrollLayout()
        self.updateParamsVisibility(self.nodeName)

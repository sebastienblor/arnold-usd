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
        self.addControl('enable', label='Enable')
        self.addSeparator()
        self.addControl('mode', label='Mode', changeCommand=self.updateParamsVisibility)
        self.addControl('illuminant', label='Illuminant')
        self.addControl('temperature', label='Temperature')
        self.addControl('custom', label='Custom')
        maya.mel.eval('AEdependNodeTemplate '+self.nodeName)

        self.addExtraControls()
        self.endScrollLayout()
        self.updateParamsVisibility(self.nodeName)

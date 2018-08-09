import maya.mel
import maya.cmds as cmds
from mtoa.ui.ae.shaderTemplate import ShaderAETemplate

class AEaiClampTemplate(ShaderAETemplate):

    def updateParamsVisibility(self, nodeName):
        modeAttr = '%s.%s' % (nodeName, 'mode')
        modeValue = cmds.getAttr(modeAttr)
        if modeValue == 0:
            cmds.editorTemplate(dimControl=(nodeName, 'min', False))
            cmds.editorTemplate(dimControl=(nodeName, 'max', False))
            cmds.editorTemplate(dimControl=(nodeName, 'minColor', True))
            cmds.editorTemplate(dimControl=(nodeName, 'maxColor', True))
        else:
            cmds.editorTemplate(dimControl=(nodeName, 'min', True))
            cmds.editorTemplate(dimControl=(nodeName, 'max', True))
            cmds.editorTemplate(dimControl=(nodeName, 'minColor', False))
            cmds.editorTemplate(dimControl=(nodeName, 'maxColor', False))


    def setup(self):
        self.addSwatch()
        self.beginScrollLayout()

        self.addControl('input')
        self.addControl('mode', changeCommand=self.updateParamsVisibility)
        self.addControl('min')
        self.addControl('max')
        self.addControl('min_color')
        self.addControl('max_color')
        
        maya.mel.eval('AEdependNodeTemplate '+self.nodeName)
        
        self.addExtraControls()
        self.endScrollLayout()

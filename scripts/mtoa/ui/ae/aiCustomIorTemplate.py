import maya.mel
import mtoa.utils as utils
import mtoa.ui.ae.utils as aeUtils
import maya.cmds as cmds
from mtoa.ui.ae.shaderTemplate import ShaderAETemplate


class AEaiCustomIorTemplate(ShaderAETemplate):

    def updateParamsVisibility(self, nodeName):
        materialAttr = '%s.%s' % (nodeName, 'material')
        materialValue = cmds.getAttr(materialAttr)
        
        if materialValue != 0:
            cmds.editorTemplate(dimControl=(nodeName, 'mode', True))
            cmds.editorTemplate(dimControl=(nodeName, 'reflectivity', True))
            cmds.editorTemplate(dimControl=(nodeName, 'edgetint', True))
            cmds.editorTemplate(dimControl=(nodeName, 'eta', True))
            cmds.editorTemplate(dimControl=(nodeName, 'k', True))
            return

        # custom Material
        cmds.editorTemplate(dimControl=(nodeName, 'mode', False))

        modeAttr = '%s.%s' % (nodeName, 'mode')
        modeValue = cmds.getAttr(modeAttr)
        if modeAttr == 0:
            # artistic
            cmds.editorTemplate(dimControl=(nodeName, 'reflectivity', False))
            cmds.editorTemplate(dimControl=(nodeName, 'edgetint', False))
            cmds.editorTemplate(dimControl=(nodeName, 'eta', True))
            cmds.editorTemplate(dimControl=(nodeName, 'k', True))
        else:
            # physical
            cmds.editorTemplate(dimControl=(nodeName, 'reflectivity', True))
            cmds.editorTemplate(dimControl=(nodeName, 'edgetint', True))
            cmds.editorTemplate(dimControl=(nodeName, 'eta', False))
            cmds.editorTemplate(dimControl=(nodeName, 'k', False))


    def setup(self):
        self.addSwatch()
       
        self.beginScrollLayout()

        self.addControl('material', label='Material')
        self.addControl('mode', changeCommand=self.updateParamsVisibility, label='Mode')
        self.addControl('reflectivity', label='Reflectivity')
        self.addControl('edgetint', label='Edge Tint')
        self.addControl('eta', label='eta')
        self.addControl('k', label='k')       

        # include/call base class/node attributes
        maya.mel.eval('AEdependNodeTemplate '+self.nodeName)
        
        self.addExtraControls()
        
        self.endScrollLayout()

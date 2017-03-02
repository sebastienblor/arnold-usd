import pymel.core as pm
import mtoa.utils as utils
import mtoa.ui.ae.utils as aeUtils
from mtoa.ui.ae.shaderTemplate import ShaderAETemplate


class AEaiCustomIorTemplate(ShaderAETemplate):

    def updateParamsVisibility(self, nodeName):
        materialAttr = '%s.%s' % (nodeName, 'material')
        materialValue = pm.getAttr(materialAttr)
        
        if materialValue != 0:
            pm.editorTemplate(dimControl=(nodeName, 'mode', True))
            pm.editorTemplate(dimControl=(nodeName, 'reflectivity', True))
            pm.editorTemplate(dimControl=(nodeName, 'edgetint', True))
            pm.editorTemplate(dimControl=(nodeName, 'eta', True))
            pm.editorTemplate(dimControl=(nodeName, 'k', True))
            return

        # custom Material
        pm.editorTemplate(dimControl=(nodeName, 'mode', False))

        modeAttr = '%s.%s' % (nodeName, 'mode')
        modeValue = pm.getAttr(modeAttr)
        if modeAttr == 0:
            # artistic
            pm.editorTemplate(dimControl=(nodeName, 'reflectivity', False))
            pm.editorTemplate(dimControl=(nodeName, 'edgetint', False))
            pm.editorTemplate(dimControl=(nodeName, 'eta', True))
            pm.editorTemplate(dimControl=(nodeName, 'k', True))
        else:
            # physical
            pm.editorTemplate(dimControl=(nodeName, 'reflectivity', True))
            pm.editorTemplate(dimControl=(nodeName, 'edgetint', True))
            pm.editorTemplate(dimControl=(nodeName, 'eta', False))
            pm.editorTemplate(dimControl=(nodeName, 'k', False))


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
        pm.mel.AEdependNodeTemplate(self.nodeName)
        
        self.addExtraControls()
        
        self.endScrollLayout()

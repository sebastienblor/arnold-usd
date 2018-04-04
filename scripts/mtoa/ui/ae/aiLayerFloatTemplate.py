import maya.mel
from mtoa.ui.ae.shaderTemplate import ShaderAETemplate
import maya.cmds as cmds

class AEaiLayerFloatTemplate(ShaderAETemplate):
    def EnableLayerChange(self,plugName):
        enabledAttr = '{}.{}'.format(self.nodeName, plugName)
        dimLayer = (cmds.getAttr(enabledAttr) == 0)
        cmds.editorTemplate(dimControl=(self.nodeName, plugName.replace('enable', 'name'), dimLayer ))
        cmds.editorTemplate(dimControl=(self.nodeName, plugName.replace('enable', 'input'), dimLayer ))
        cmds.editorTemplate(dimControl=(self.nodeName, plugName.replace('enable', 'mix'), dimLayer ))

    def setup(self):
        self.addSwatch()
        self.beginScrollLayout()

        for i in range(1, 8):        
            self.beginLayout('Layer {}'.format(i), collapse=(i > 2))
            enableAttr = 'enable{}'.format(i)
            self.addControl(enableAttr, label='Enable', annotation='Enable Layer {}'.format(i),  changeCommand=lambda arg=None, x=enableAttr: self.EnableLayerChange(x))
        
            self.addControl('name{}'.format(i), label='Name', annotation='Name of Layer {}'.format(i))
            self.addControl('input{}'.format(i), label='Input', annotation='Input for Layer {}'.format(i))
            self.addControl('mix{}'.format(i), label='Mix', annotation='Mix weight for Layer {}'.format(i))
            self.endLayout()

        # include/call base class/node attributes
        maya.mel.eval('AEdependNodeTemplate '+self.nodeName)
        
        self.addExtraControls()
        self.endScrollLayout()


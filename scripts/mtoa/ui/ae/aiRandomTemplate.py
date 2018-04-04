import maya.mel
import maya.cmds as cmds
from mtoa.ui.ae.shaderTemplate import ShaderAETemplate

class AEaiRandomTemplate(ShaderAETemplate):
    def inputTypeChanged(self, nodeName):
        fullAttr = '%s.%s'%(nodeName, "inputType")
        inputTypeValue = cmds.getAttr(fullAttr)
        
        cmds.editorTemplate(dimControl=(nodeName, "inputInt", (False if inputTypeValue == 0  else True)))
        cmds.editorTemplate(dimControl=(nodeName, "inputFloat", (False if inputTypeValue == 1  else True)))
        cmds.editorTemplate(dimControl=(nodeName, "inputColor", (False if inputTypeValue == 2  else True)))
    
    def setup(self):
    	self.addSwatch()
        self.beginScrollLayout()

        self.beginLayout('Random', collapse=False)
        self.addControl('inputType', label='Type', changeCommand=self.inputTypeChanged)
        self.addControl('inputInt', label='Int')
        self.addControl('inputFloat', label='Float')
        self.addControl('inputColor', label='Color')
        self.endLayout()
        self.addControl('seed')
        self.addControl('grayscale')
        
        maya.mel.eval('AEdependNodeTemplate '+self.nodeName)
        
        self.addExtraControls()
        self.endScrollLayout()

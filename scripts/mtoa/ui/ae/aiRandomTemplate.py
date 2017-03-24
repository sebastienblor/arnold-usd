import pymel.core as pm
from mtoa.ui.ae.shaderTemplate import ShaderAETemplate

class AEaiRandomTemplate(ShaderAETemplate):
    def inputTypeChanged(self, nodeName):
        fullAttr = '%s.%s'%(nodeName, "inputType")
        inputTypeValue = pm.getAttr(fullAttr)
        
        pm.editorTemplate(dimControl=(nodeName, "inputInt", (False if inputTypeValue == 0  else True)))
        pm.editorTemplate(dimControl=(nodeName, "inputFloat", (False if inputTypeValue == 1  else True)))
        pm.editorTemplate(dimControl=(nodeName, "inputColor", (False if inputTypeValue == 2  else True)))
    
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
        
        pm.mel.AEdependNodeTemplate(self.nodeName)
        
        self.addExtraControls()
        self.endScrollLayout()

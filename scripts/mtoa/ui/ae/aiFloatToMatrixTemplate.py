import maya.mel
from mtoa.ui.ae.shaderTemplate import ShaderAETemplate

class AEaiFloatToMatrixTemplate(ShaderAETemplate):
    def setup(self):
        self.addSwatch()
        self.beginScrollLayout()

        self.addControl('input00', label="Input[0][0]")
        self.addControl('input01', label="Input[0][1]")
        self.addControl('input02', label="Input[0][2]")
        self.addControl('input03', label="Input[0][3]")
        
        self.addControl('input10', label="Input[1][0]")
        self.addControl('input11', label="Input[1][1]")
        self.addControl('input12', label="Input[1][2]")
        self.addControl('input13', label="Input[1][3]")

        self.addControl('input20', label="Input[2][0]")
        self.addControl('input21', label="Input[2][1]")
        self.addControl('input22', label="Input[2][2]")
        self.addControl('input23', label="Input[2][3]")
                
        self.addControl('input30', label="Input[3][0]")
        self.addControl('input31', label="Input[3][1]")
        self.addControl('input32', label="Input[3][2]")
        self.addControl('input33', label="Input[3][3]")
        
        maya.mel.eval('AEdependNodeTemplate '+self.nodeName)
        
        self.addExtraControls()
        self.endScrollLayout()

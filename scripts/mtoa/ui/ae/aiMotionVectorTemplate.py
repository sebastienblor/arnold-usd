import pymel.core as pm
from mtoa.ui.ae.shaderTemplate import ShaderAETemplate

class AEaiMotionVectorTemplate(ShaderAETemplate):
    def checkRaw(self, nodeName):
        fullAttr = '%s.%s'%(nodeName, "raw")
        rawValue = pm.getAttr(fullAttr)
        
        dim = rawValue
        pm.editorTemplate(dimControl=(nodeName, "maxDisplace", dim))
        
    def setup(self):
        self.addSwatch()
        self.beginScrollLayout()
        
        self.beginLayout("Motion Vector Attributes", collapse=False)
    
        self.addControl("time0", label="Time 0")
        self.addControl("time1", label="Time 1")
        self.addSeparator()
        self.addControl("raw", changeCommand=self.checkRaw, label="Raw")
        self.addControl("max_displace", label="Max Displace")
        
        self.endLayout()
        
        pm.mel.AEdependNodeTemplate(self.nodeName)

        self.addExtraControls()
        self.endScrollLayout()
    

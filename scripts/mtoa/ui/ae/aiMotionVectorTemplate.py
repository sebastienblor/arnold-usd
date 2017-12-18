import maya.mel
import maya.cmds as cmds
from mtoa.ui.ae.shaderTemplate import ShaderAETemplate

class AEaiMotionVectorTemplate(ShaderAETemplate):
    def checkRaw(self, nodeName):
        fullAttr = '%s.%s'%(nodeName, "raw")
        rawValue = cmds.getAttr(fullAttr)
        
        dim = rawValue
        cmds.editorTemplate(dimControl=(nodeName, "maxDisplace", dim))
        
    def setup(self):
        self.addSwatch()
        self.beginScrollLayout()
        
        self.beginLayout("Motion Vector Attributes", collapse=False)
    
        self.addControl("time0", label="Start Time")
        self.addControl("time1", label="End Time")
        self.addSeparator()
        self.addControl("raw", changeCommand=self.checkRaw, label="Encode Raw Vector")
        self.addControl("maxDisplace", label="Max Displace")
        
        self.endLayout()
        
        maya.mel.eval('AEdependNodeTemplate '+self.nodeName)

        self.addExtraControls()
        self.endScrollLayout()
    

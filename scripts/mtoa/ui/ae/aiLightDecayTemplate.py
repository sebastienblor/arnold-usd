import mtoa.ui.ae.utils as aeUtils
import maya.mel
import maya.cmds as cmds
from mtoa.ui.ae.shaderTemplate import ShaderAETemplate

class AEaiLightDecayTemplate(ShaderAETemplate):
    def checkNearAtten(self, nodeName):
        fullAttr = '%s.%s'%(nodeName, "use_near_atten")
        rawValue = cmds.getAttr(fullAttr)
        
        dim = not rawValue
        cmds.editorTemplate(dimControl=(nodeName, "nearStart", dim))
        cmds.editorTemplate(dimControl=(nodeName, "nearEnd", dim))

    def checkFarAtten(self, nodeName):
        fullAttr = '%s.%s'%(nodeName, "use_far_atten")
        rawValue = cmds.getAttr(fullAttr)
        
        dim = not rawValue
        cmds.editorTemplate(dimControl=(nodeName, "farStart", dim))
        cmds.editorTemplate(dimControl=(nodeName, "farEnd", dim))

    def setup(self):
        self.addSwatch()
        self.beginScrollLayout()
        
        self.beginLayout('Attenuation', collapse=False)

        self.addControl('useNearAtten', changeCommand=self.checkNearAtten, label='Use Near Attenuation')
        self.addControl('nearStart', label='Near Start')
        self.addControl('nearEnd', label='Near End')

        self.addSeparator()
        
        self.addControl('useFarAtten', changeCommand=self.checkFarAtten, label='Use Far Attenuation')
        self.addControl('farStart', label='Far Start')
        self.addControl('farEnd', label='Far End')
        
        self.endLayout()
        
        maya.mel.eval('AEdependNodeTemplate '+self.nodeName)

        self.addExtraControls()
        self.endScrollLayout()


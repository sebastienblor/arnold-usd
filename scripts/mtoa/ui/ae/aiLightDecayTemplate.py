import mtoa.ui.ae.utils as aeUtils
import pymel.core as pm
from mtoa.ui.ae.shaderTemplate import ShaderAETemplate
'''
def checkNearAtten(nodeName):
    aeUtils.arnoldDimControlIfFalse(nodeName, 'near_start', 'use_near_atten')
    aeUtils.arnoldDimControlIfFalse(nodeName, 'near_end', 'use_near_atten')

def checkFarAtten(nodeName):
    aeUtils.arnoldDimControlIfFalse(nodeName, 'far_start', 'use_far_atten')
    aeUtils.arnoldDimControlIfFalse(nodeName, 'far_end', 'use_far_atten')
'''

class AEaiLightDecayTemplate(ShaderAETemplate):
    def setup(self):
        self.addSwatch()
        self.beginScrollLayout()
        
        self.beginLayout('Attenuation', collapse=False)

        #cmds.editorTemplate("use_near_atten", aeCallback(checkNearAtten), addControl=True, label="Use Near Attenuation")
        self.addControl('use_near_atten', label='Use Near Attenuation')
        self.addControl('near_start', label='Near Start')
        self.addControl('near_end', label='Near End')

        self.addSeparator()
        
        #cmds.editorTemplate("use_far_atten", aeCallback(checkNearAtten), addControl=True, label="Use Far Attenuation")        
        self.addControl('use_far_atten', label='Use Far Attenuation')
        self.addControl('far_start', label='Far Start')
        self.addControl('far_end', label='Far End')
        
        self.endLayout()
        
        pm.mel.AEdependNodeTemplate(self.nodeName)

        self.addExtraControls()
        self.endScrollLayout()


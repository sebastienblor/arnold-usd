import pymel.core as pm
from mtoa.ui.ae.shaderTemplate import ShaderAETemplate

class AEaiShuffleTemplate(ShaderAETemplate):
    def setup(self):
        self.addSwatch()
        self.beginScrollLayout()

        self.addControl('color')
        self.addControl('alpha')
        
        self.beginLayout('Channels', collapse=False)
        self.addControl('channelR', label='Red')
        self.addControl('channelG', label='Green')
        self.addControl('channelB', label='Blue')
        self.addControl('channelA', label='Alpha')
        self.endLayout()

        self.beginLayout('Negate', collapse=False)
        self.addControl('negateR', label='Red')
        self.addControl('negateG', label='Green')
        self.addControl('negateB', label='Blue')
        self.addControl('negateA', label='Alpha')
        self.endLayout()
        
        pm.mel.AEdependNodeTemplate(self.nodeName)
        
        self.addExtraControls()
        self.endScrollLayout()

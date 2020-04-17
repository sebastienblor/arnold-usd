import maya.mel
from mtoa.ui.ae.shaderTemplate import ShaderAETemplate

class AEaiCollectionTemplate(ShaderAETemplate):

    def setup(self):
        self.beginScrollLayout()
        
        self.beginLayout('Operator', collapse=False)
        self.addControl("enable")
        self.addControl("inputs")
        self.addControl("selection")
        self.endLayout()
        self.beginLayout('Collection', collapse=False)
        self.addControl("collection")
        self.endLayout()
        maya.mel.eval('AEdependNodeTemplate '+self.nodeName)

        self.addExtraControls()

        self.endScrollLayout()


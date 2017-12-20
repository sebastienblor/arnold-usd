import maya.mel
import mtoa.utils as utils
import mtoa.ui.ae.utils as aeUtils
from mtoa.ui.ae.shaderTemplate import ShaderAETemplate


class AEaiFlakesTemplate(ShaderAETemplate):

   
    def setup(self):
        self.addSwatch()
       
        self.beginScrollLayout()
        
        self.beginLayout('Flakes', collapse=False)
        self.addControl('scale', label='Scale')
        self.addControl('density')
        self.addControl('step')
        self.addControl('depth')
        self.addControl('IOR')
        
        self.addControl('normalRandomize', label='Normal Randomize')        
        self.addControl('coordSpace', label='Coord Space')
        self.addControl('outputSpace', label='Output Space')
        
        #self.addControl('size', label='Size')
        self.endLayout()
  
        # include/call base class/node attributes
        maya.mel.eval('AEdependNodeTemplate '+self.nodeName)
        
        self.addExtraControls()
        
        self.endScrollLayout()

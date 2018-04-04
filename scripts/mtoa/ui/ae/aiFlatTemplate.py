import maya.mel
import mtoa.utils as utils
import mtoa.ui.ae.utils as aeUtils
from mtoa.ui.ae.shaderTemplate import ShaderAETemplate


class AEaiFlatTemplate(ShaderAETemplate):

    def setup(self):
        self.addSwatch()
       
        self.beginScrollLayout()
        
        self.addCustom('message', 'AEshaderTypeNew', 'AEshaderTypeReplace')

        self.beginLayout('Flat / Constant', collapse=False)
        self.addControl('color', label='Color')
        self.endLayout()

        self.beginLayout('Matte', collapse=True)
        self.addControl('aiEnableMatte', label='Enable Matte')
        self.addControl('aiMatteColor', label='Matte Color')
        self.addControl('aiMatteColorA', label='Matte Opacity')
        self.endLayout()
        
        # include/call base class/node attributes
        maya.mel.eval('AEdependNodeTemplate '+self.nodeName)
        
        self.addExtraControls()
        
        self.endScrollLayout()

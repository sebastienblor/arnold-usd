import pymel.core as pm
import mtoa.utils as utils
import mtoa.ui.ae.utils as aeUtils
from mtoa.ui.ae.shaderTemplate import ShaderAETemplate


class AEaiThinFilmTemplate(ShaderAETemplate):

   
    def setup(self):
        self.addSwatch()
       
        self.beginScrollLayout()
        
        self.addCustom('message', 'AEshaderTypeNew', 'AEshaderTypeReplace')

        self.beginLayout('Thickness', collapse=False)
        self.addControl('thickness_min', label='Min Thickness (nm)')
        self.addControl('thickness_max', label='Max Thickness (nm)')
        self.addControl('thickness', label='Thickness')
        self.endLayout()

        self.beginLayout('IOR', collapse=False)
        self.addControl('ior_medium', label='Medium IOR')
        self.addControl('ior_film', label='Film IOR')
        self.addControl('ior_internal', label='Internal IOR')       
        self.endLayout()

        # include/call base class/node attributes
        pm.mel.AEdependNodeTemplate(self.nodeName)
        
        self.addExtraControls()
        
        self.endScrollLayout()

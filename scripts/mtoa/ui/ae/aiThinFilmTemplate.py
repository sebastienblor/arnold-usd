import maya.mel
import mtoa.utils as utils
import mtoa.ui.ae.utils as aeUtils
from mtoa.ui.ae.shaderTemplate import ShaderAETemplate


class AEaiThinFilmTemplate(ShaderAETemplate):

   
    def setup(self):
        self.addSwatch()
       
        self.beginScrollLayout()
        
        self.beginLayout('Thickness', collapse=False)
        self.addControl('thicknessMin', label='Min Thickness (nm)')
        self.addControl('thicknessMax', label='Max Thickness (nm)')
        self.addControl('thickness', label='Thickness')
        self.endLayout()

        self.beginLayout('IOR', collapse=False)
        self.addControl('iorMedium', label='Medium IOR')
        self.addControl('iorFilm', label='Film IOR')
        self.addControl('iorInternal', label='Internal IOR')       
        self.endLayout()

        # include/call base class/node attributes
        maya.mel.eval('AEdependNodeTemplate '+self.nodeName)
        
        self.addExtraControls()
        
        self.endScrollLayout()

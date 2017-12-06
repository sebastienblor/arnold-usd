import pymel.core as pm
import mtoa.utils as utils
import mtoa.ui.ae.utils as aeUtils
import maya.cmds as cmds
import maya.mel
from mtoa.ui.ae.shaderTemplate import ShaderAETemplate

class AEaiPhysicalSkyTemplate(ShaderAETemplate):
    def setup(self):
        self.addSwatch()
        self.beginScrollLayout()
        
        self.beginLayout("Physical Sky Attributes", collapse=False)
        
        self.addControl("turbidity", label="Turbidity")
        self.addControl("groundAlbedo", label="Ground Albedo")
        
        self.addControl("elevation", label="Elevation", annotation="WARNING : Linking rendertime graphs are not supported!")
        self.addControl("azimuth", label="Azimuth", annotation="WARNING : Linking rendertime graphs are not supported!")
        self.addControl("intensity", label="Intensity")
        
        self.addSeparator()
        
        self.addControl("skyTint", label="Sky Tint")
        self.addControl("sunTint", label="Sun Tint")
        self.addControl("sunSize", label="Sun Size")
        self.addControl("enableSun", label="Enable Sun")
        
        self.endLayout()
        
        maya.mel.eval('AEdependNodeTemplate '+self.nodeName)
        self.addExtraControls()
        self.endScrollLayout()


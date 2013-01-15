import pymel.core as pm
from mtoa.ui.ae.shaderTemplate import ShaderAETemplate

class AEaiHairTemplate(ShaderAETemplate):
    def setup(self):
        self.addSwatch()
        self.beginScrollLayout()
        
        self.addCustom('message', 'AEshaderTypeNew', 'AEshaderTypeReplace')
        
        self.beginLayout('Color', collapse=False)
        self.addControl('rootcolor', label='Root Color')
        self.addControl('tipcolor', label='Tip Color')
        self.endLayout()
        
        self.beginLayout('Diffuse', collapse=False)
        self.addControl('diffuse_cache', label='Diffuse Cache')
        self.addControl('ambdiff', label='Ambient Diffuse')
        self.addControl('kd_ind', label='Indirect Diffuse')
        self.endLayout()
        
        self.beginLayout('Specular', collapse=False)
        self.addControl('gloss', label='Gloss')
        self.addControl('spec', label='Specular')
        self.addControl('spec_color', label='Specular Color')
        self.addControl('spec_shift', label='Specular Shift')
        
        self.beginLayout('Secondary', collapse=True)        
        self.addControl('gloss2', label='Gloss')
        self.addControl('spec2', label='Specular')
        self.addControl('spec2_color', label='Specular Color')
        self.addControl('spec2_shift', label='Specular Shift')
        self.endLayout()
        
        self.endLayout()
        
        self.beginLayout('Extra', collapse=False)
        self.addControl('opacity', label='Opacity')
        self.endLayout()
        
        pm.mel.AEdependNodeTemplate(self.nodeName)
        
        self.addExtraControls()
        self.endScrollLayout()


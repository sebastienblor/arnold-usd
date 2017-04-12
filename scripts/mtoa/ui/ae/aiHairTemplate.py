import pymel.core as pm
from mtoa.ui.ae.shaderTemplate import ShaderAETemplate
import maya.cmds as cmds
import mtoa.convertShaders as convertShaders

class AEaiHairTemplate(ShaderAETemplate):

    def convertToStandardHair(self, nodeName):
        convertShaders.doMapping(nodeName)
    

    def convertShaderNew(self, nodeName):
        tokens = nodeName.split('.')
        nodeName = tokens[0]        
        #cmds.rowLayout(nc=2, cw2=(200,140), cl2=('center', 'center'))
        cmds.button('aiHairConvertShaderButton',  label='Convert To New Shader', command=pm.Callback(self.convertToStandardHair, nodeName))
        #cmds.setParent( '..' )
   
    def convertShaderReplace(self, nodeName):
        tokens = nodeName.split('.')
        nodeName = tokens[0]
        cmds.button('aiHairConvertShaderButton',  edit=True, command=pm.Callback(self.convertToStandardHair, nodeName))
        

    def setup(self):
        self.addSwatch()
        self.beginScrollLayout()
        self.addCustom('convert_shader', self.convertShaderNew, self.convertShaderReplace)
        
        #self.addCustom('message', 'AEshaderTypeNew', 'AEshaderTypeReplace')

        self.beginLayout("Matte", collapse=True)
        self.addControl("aiEnableMatte", label="Enable Matte")
        self.addControl("aiMatteColor", label="Matte Color")
        self.addControl("aiMatteColorA", label="Matte Opacity")
        self.endLayout()
        
        self.beginLayout('Diffuse', collapse=False)
        self.addControl('rootcolor', label='Root Color')
        self.addControl('tipcolor', label='Tip Color')
        self.addSeparator()        
        self.addControl('ambdiff', label='Ambient Diffuse')
        self.addControl('kdInd', label='Indirect Diffuse')
        self.endLayout()
        
        self.beginLayout('Specular', collapse=False)
        self.addControl('gloss', label='Glossiness')
        self.addControl('spec', label='Weight')
        self.addControl('specColor', label='Color')
        self.addControl('specShift', label='Angular Shift')
        
        self.beginLayout('Secondary Specular', collapse=True)        
        self.addControl('gloss2', label='Glossiness')
        self.addControl('spec2', label='Weight')
        self.addControl('spec2Color', label='Color')
        self.addControl('spec2Shift', label='Angular Shift')
        self.endLayout()

        self.endLayout()
        
        self.beginLayout('Transmission', collapse=False)
        self.addControl('transmission', label='Weight')
        self.addControl('transmissionColor', label='Color')
        self.addControl('transmissionSpread', label='Spread')
        self.endLayout()

        self.beginLayout('Opacity', collapse=False)
        self.addControl('opacity', label='Color')
        self.endLayout()


#        self.addAOVLayout()
        
        # include/call base class/node attributes
        pm.mel.AEdependNodeTemplate(self.nodeName)
        
        self.addExtraControls()
        self.endScrollLayout()


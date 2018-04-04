import maya.cmds as cmds
import maya.mel
from mtoa.ui.ae.shaderTemplate import ShaderAETemplate

class AEaiVolumeCollectorTemplate(ShaderAETemplate):
    def scatteringSource(self, nodeName):
        source = self.nodeAttr('scattering_source')
        sourceVal = cmds.getAttr(source)
        print sourceVal
        if(sourceVal == 0):
            cmds.editorTemplate(dimControl=(nodeName, "scattering", False))
            cmds.editorTemplate(dimControl=(nodeName, "scatteringChannel", True))
        else:
            cmds.editorTemplate(dimControl=(nodeName, "scattering", True))
            cmds.editorTemplate(dimControl=(nodeName, "scatteringChannel", False))
            
    def attenuationSource(self, nodeName):
        source = self.nodeAttr('attenuation_source')
        sourceVal = cmds.getAttr(source)
        print sourceVal
        if(sourceVal == 0):
            cmds.editorTemplate(dimControl=(nodeName, "attenuation", False))
            cmds.editorTemplate(dimControl=(nodeName, "attenuationChannel", True))
        elif(sourceVal == 1):
            cmds.editorTemplate(dimControl=(nodeName, "attenuation", True))
            cmds.editorTemplate(dimControl=(nodeName, "attenuationChannel", False))
        else:
            cmds.editorTemplate(dimControl=(nodeName, "attenuation", True))
            cmds.editorTemplate(dimControl=(nodeName, "attenuationChannel", True))
            
    def emissionSource(self, nodeName):
        source = self.nodeAttr('emission_source')
        sourceVal = cmds.getAttr(source)
        print sourceVal
        if(sourceVal == 0):
            cmds.editorTemplate(dimControl=(nodeName, "emission", False))
            cmds.editorTemplate(dimControl=(nodeName, "emissionChannel", True))
        else:
            cmds.editorTemplate(dimControl=(nodeName, "emission", True))
            cmds.editorTemplate(dimControl=(nodeName, "emissionChannel", False))
            
    def setup(self):
        self.beginScrollLayout()
        
        self.beginLayout('Scattering', collapse=False)
        self.addControl('scatteringSource',changeCommand=self.scatteringSource)
        self.addControl('scattering', label='Scattering')
        self.addControl('scatteringChannel')
        self.addControl('scatteringColor')
        self.addControl('scatteringIntensity')
        self.addControl('anisotropy')
        self.endLayout()
        
        self.beginLayout('Attenuation', collapse=False)
        self.addControl('attenuationSource',changeCommand=self.attenuationSource)
        self.addControl('attenuation')
        self.addControl('attenuationChannel')
        self.addControl('attenuationColor')
        self.addControl('attenuationIntensity')
        self.addControl('attenuationMode', label='Attenuation Mode')
        self.endLayout()
        
        self.beginLayout('Emission', collapse=False)
        self.addControl('emissionSource',changeCommand=self.emissionSource)
        self.addControl('emission', label='Emission')
        self.addControl('emissionChannel')
        self.addControl('emissionColor')
        self.addControl('emissionIntensity')
        self.endLayout()
        
        self.beginLayout('Sampling', collapse=False)
        self.addControl('positionOffset')
        self.addControl('interpolation')
        self.endLayout()
               
        maya.mel.eval('AEdependNodeTemplate '+self.nodeName)

        self.addExtraControls()
        self.endScrollLayout()

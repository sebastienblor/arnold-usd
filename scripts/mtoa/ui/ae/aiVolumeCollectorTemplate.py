import pymel.core as pm
from mtoa.ui.ae.shaderTemplate import ShaderAETemplate

class AEaiVolumeCollectorTemplate(ShaderAETemplate):
    def scatteringSource(self, nodeName):
        source = self.nodeAttr('scattering_source')
        sourceVal = pm.getAttr(source)
        print sourceVal
        if(sourceVal == 0):
            pm.editorTemplate(dimControl=(nodeName, "scattering", False))
            pm.editorTemplate(dimControl=(nodeName, "scatteringChannel", True))
        else:
            pm.editorTemplate(dimControl=(nodeName, "scattering", True))
            pm.editorTemplate(dimControl=(nodeName, "scatteringChannel", False))
            
    def attenuationSource(self, nodeName):
        source = self.nodeAttr('attenuation_source')
        sourceVal = pm.getAttr(source)
        print sourceVal
        if(sourceVal == 0):
            pm.editorTemplate(dimControl=(nodeName, "attenuation", False))
            pm.editorTemplate(dimControl=(nodeName, "attenuationChannel", True))
        elif(sourceVal == 1):
            pm.editorTemplate(dimControl=(nodeName, "attenuation", True))
            pm.editorTemplate(dimControl=(nodeName, "attenuationChannel", False))
        else:
            pm.editorTemplate(dimControl=(nodeName, "attenuation", True))
            pm.editorTemplate(dimControl=(nodeName, "attenuationChannel", True))
            
    def emissionSource(self, nodeName):
        source = self.nodeAttr('emission_source')
        sourceVal = pm.getAttr(source)
        print sourceVal
        if(sourceVal == 0):
            pm.editorTemplate(dimControl=(nodeName, "emission", False))
            pm.editorTemplate(dimControl=(nodeName, "emissionChannel", True))
        else:
            pm.editorTemplate(dimControl=(nodeName, "emission", True))
            pm.editorTemplate(dimControl=(nodeName, "emissionChannel", False))
            
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
               
        pm.mel.AEdependNodeTemplate(self.nodeName)

        self.addExtraControls()
        self.endScrollLayout()

import maya.mel
from mtoa.ui.ae.shaderTemplate import ShaderAETemplate

class AEcryptomatteTemplate(ShaderAETemplate):
    def setup(self):
        self.beginScrollLayout()
        self.addControl('sidecarManifests')
        self.addControl('cryptomatteDepth')
        self.addControl('stripObjNamespaces')
        self.addControl('stripMatNamespaces')
        self.addControl('aovCryptoAsset')
        self.addControl('aovCryptoObject')
        self.addControl('aovCryptoMaterial')

        self.addControl('previewInExr')
        self.addControl('processMaya')
        self.addControl('processPaths')
        self.addControl('processObjPathPipes')
        self.addControl('processMatPathPipes')
        self.addControl('processLegacy')
        self.beginLayout('User AOVs', collapse=True)
        self.addControl('userCryptoAov0')
        self.addControl('userCryptoSrc0')
        self.addControl('userCryptoAov1')
        self.addControl('userCryptoSrc1')
        self.addControl('userCryptoAov2')
        self.addControl('userCryptoSrc2')
        self.addControl('userCryptoAov3')
        self.addControl('userCryptoSrc3')
        self.endLayout()
        maya.mel.eval('AEdependNodeTemplate '+self.nodeName)

        self.addExtraControls()
        self.endScrollLayout()

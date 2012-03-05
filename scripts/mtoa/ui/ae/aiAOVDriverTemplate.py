import pymel.core as pm
from mtoa.ui.ae.templates import TranslatorControl
from mtoa.ui.ae.shaderTemplate import ShaderAETemplate

class AEaiAOVDriverTemplate(ShaderAETemplate):

    def setup(self):
        #mel.eval('AEswatchDisplay "%s"' % nodeName)

        self.beginScrollLayout()

        self.beginLayout('Image Format', collapse=False)
        driverTemplate = TranslatorControl(
                              nodeType='aiAOVDriver',
                              label='')
        driverTemplate._doSetup(self.nodeName)
        #self.addChildTemplate('imageFormat', driverTemplate)
        self.endLayout()

        self.beginLayout("Advanced Output", collapse=False)
        self.addControl('prefix')
        self.endLayout()

        # include/call base class/node attributes
        pm.mel.AEdependNodeTemplate(self.nodeName)
        self.addExtraControls()

        self.endScrollLayout()

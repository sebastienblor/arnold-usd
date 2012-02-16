import pymel.core as pm
from mtoa.ui.ae.shapeTemplate import TranslatorControl, getTranslators
from mtoa.ui.ae.shaderTemplate import ShaderAETemplate

from mtoa.ui.ae.utils import aeCallback
class AOVTranslatorControl(TranslatorControl):
    '''
    Adds a "<Use Globals>" menu option
    '''
    USE_GLOBALS = '<Use Globals>'
    def getTranslators(self):
        if self._translators is None:
            self._translators = [self.USE_GLOBALS] + getTranslators(self.nodeType())
        return self._translators

    def getDefaultTranslator(self, nodeName):
        return self.USE_GLOBALS

class AEaiAOVTemplate(ShaderAETemplate):

    def aiAOV_defaultValueNew(self, nodeAttr):
        pm.attrNavigationControlGrp('aiAOVDefaultValue',
                                    label='Default',
                                    at=nodeAttr)

    def aiAOV_defaultValueReplace(self, nodeAttr):
        pm.attrNavigationControlGrp('aiAOVDefaultValue', edit=True, at=nodeAttr)

    def setup(self):
        #mel.eval('AEswatchDisplay "%s"' % nodeName)

        self.beginScrollLayout()
        self.beginLayout("AOV Attributes", collapse=False)

        #self.beginLayout("Primary Controls", collapse=False)
        self.addControl('enabled')
        self.addControl('name')
        self.addControl('type')
        self.addControl('prefix')
        #self.endLayout()
        self.addCustom('defaultValue', self.aiAOV_defaultValueNew, self.aiAOV_defaultValueReplace)

        self.beginLayout('Image Format', collapse=False)
        driverTemplate = AOVTranslatorControl(
                              nodeType='<driver>',
                              label='',
                              controlAttr='imageFormat')
        driverTemplate._doSetup(self.nodeName)
        #self.addChildTemplate('imageFormat', driverTemplate)
        self.endLayout()

        self.beginLayout('Filter', collapse=False)
        filterTemplate = AOVTranslatorControl(
                              nodeType='<filter>',
                              label='',
                              controlAttr='filterType')
        filterTemplate._doSetup(self.nodeName)
        #self.addChildTemplate('filterType', filterTemplate)
        self.endLayout()

        self.endLayout()

        # include/call base class/node attributes
        pm.mel.AEdependNodeTemplate(self.nodeName)
        self.addExtraControls()

        self.endScrollLayout()

        self.suppress('driverExrCompression')
        self.suppress('driverHalfPrecision')
        self.suppress('driverTiled')
        self.suppress('driverPreserveLayerName')
        self.suppress('driverQuality')
        self.suppress('driverOutputPadded')
        self.suppress('driverGamma')
        self.suppress('driverDitherAmplitude')
        self.suppress('driverDitherAmplitude')
        self.suppress('driverPngFormat')
        self.suppress('driverTiffCompression')
        self.suppress('driverTiffFormat')
        self.suppress('driverUnpremultAlpha')
        self.suppress('filterWidth')
        self.suppress('filterDomain')
        self.suppress('filterMinimum')
        self.suppress('filterMaximum')
        self.suppress('filterScalarMode')

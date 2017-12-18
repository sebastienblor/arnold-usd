import maya.mel
import maya.cmds as cmds
import mtoa.utils as utils
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
        self.addControl('prefix', label="Override Path Prefix")
        self.addControl('mergeAOVs', label="Merge AOVs")
        self.addControl('outputMode')
        maya_version = utils.getMayaVersion()
        if maya_version >= 2017:
            self.addControl('colorManagement', label = 'Color Space')

        self.addControl('lightPathExpression')
        self.addControl('lightGroups')


        #self.addControl('customAttributes')
        self.endLayout()

        # include/call base class/node attributes
        maya.mel.eval('AEdependNodeTemplate '+self.nodeName)
        
        self.suppress('customAttributes')
        self.suppress('layerHalfPrecision')
        self.suppress('layerTolerance')
        self.suppress('layerEnableFiltering')
        self.addExtraControls()

        self.endScrollLayout()

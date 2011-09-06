import maya.cmds as cmds
import maya.mel as mel
from mtoa.ui.ae.shapeTemplate import TranslatorControl
import mtoa.core as core

class AOVTranslatorControl(TranslatorControl):
    '''
    Adds a "<Use Globals>" menu option
    '''
    USE_GLOBALS = '<Use Globals>'
    def getTranslators(self):
        if self._translators is None:
            self._translators = [self.USE_GLOBALS] + [x[0] for x in core.listTranslators(self.nodeType())]
        return self._translators

    def getDefaultTranslator(self, nodeName):
        return self.USE_GLOBALS

def aiAOVTemplate(nodeName):

    #mel.eval('AEswatchDisplay "%s"' % nodeName)

    cmds.editorTemplate(beginScrollLayout=True)
    cmds.editorTemplate(beginLayout="AOV Attributes", collapse=False)

    #cmds.editorTemplate(beginLayout="Primary Controls", collapse=False)
    cmds.editorTemplate('enabled', addControl=True)
    cmds.editorTemplate('name', addControl=True)
    cmds.editorTemplate('type', addControl=True)
    cmds.editorTemplate('prefix', addControl=True)
    #cmds.editorTemplate(endLayout=True)

    driverCtrl = AOVTranslatorControl(nodeType='<driver>',
                                      label='',
                                      controlAttr='imageFormat')
    driverCtrl.attachToAE('Image Format', collapse=False)

    fitlerCtrl = AOVTranslatorControl(nodeType='<filter>',
                                      label='',
                                      controlAttr='filterType')
    fitlerCtrl.attachToAE('Filter', collapse=False)

    cmds.editorTemplate(endLayout=True)

    # include/call base class/node attributes
    mel.eval('AEdependNodeTemplate "%s"' % nodeName)
    cmds.editorTemplate(addExtraControls=True)

    cmds.editorTemplate(endScrollLayout=True)

    cmds.editorTemplate(nodeName, suppress='driverExrCompression')
    cmds.editorTemplate(nodeName, suppress='driverHalfPrecision')
    cmds.editorTemplate(nodeName, suppress='driverTiled')
    cmds.editorTemplate(nodeName, suppress='driverPreserveLayerName')
    cmds.editorTemplate(nodeName, suppress='driverQuality')
    cmds.editorTemplate(nodeName, suppress='driverOutputPadded')
    cmds.editorTemplate(nodeName, suppress='driverGamma')
    cmds.editorTemplate(nodeName, suppress='driverDitherAmplitude')
    cmds.editorTemplate(nodeName, suppress='driverDitherAmplitude')
    cmds.editorTemplate(nodeName, suppress='driverPngFormat')
    cmds.editorTemplate(nodeName, suppress='driverTiffCompression')
    cmds.editorTemplate(nodeName, suppress='driverTiffFormat')
    cmds.editorTemplate(nodeName, suppress='driverUnpremultAlpha')
    cmds.editorTemplate(nodeName, suppress='filterWidth')
    cmds.editorTemplate(nodeName, suppress='filterDomain')
    cmds.editorTemplate(nodeName, suppress='filterMinimum')
    cmds.editorTemplate(nodeName, suppress='filterMaximum')
    cmds.editorTemplate(nodeName, suppress='filterScalarMode')

import maya.cmds as cmds
import maya.mel as mel
from mtoa.ui.ae.shapeTemplate import TranslatorControl

class AOVTranslatorControl(TranslatorControl):
    '''
    Adds a "<Use Globals>" menu option
    '''
    USE_GLOBALS = '<Use Globals>'
    def getTranslators(self):
        if self._translators is None:
            self._translators = [self.USE_GLOBALS] + cmds.arnoldPlugins(listTranslators=self.nodeType())
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



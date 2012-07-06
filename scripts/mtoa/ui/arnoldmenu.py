import pymel.core as pm
from mtoa.core import createStandIn
from mtoa.ui.ae.aiStandInTemplate import LoadStandInButtonPush
import mtoa.utils as mutils

def doCreateStandInFile():
    node = createStandIn()
    LoadStandInButtonPush(node.name())

def doExportStandin():
    pm.mel.eval('ExportSelection')
    pm.mel.eval('setCurrentFileTypeOption ExportActive "" "ASS Export"')

def doExportOptionsStandin():
    pm.mel.eval('ExportSelectionOptions')
    pm.mel.eval('setCurrentFileTypeOption ExportActive "" "ASS Export"')
   
def createArnoldMenu():
    # Add an Arnold menu in Maya main window
    if not pm.about(b=1):
        pm.menu('ArnoldMenu', label='Arnold', parent='MayaWindow', tearOff=True )
        pm.menuItem('ArnoldStandIn', label='StandIn', parent='ArnoldMenu', subMenu=True)
        pm.menuItem('ArnoldCreateStandIn', parent='ArnoldStandIn', label="Create",
                    c=lambda *args: createStandIn())
        pm.menuItem('ArnoldCreateStandInFile', parent='ArnoldStandIn', optionBox=True,
                    c=lambda *args: doCreateStandInFile())
        pm.menuItem('ArnoldExportStandIn', parent='ArnoldStandIn', label='Export',
                    c=lambda *args: doExportStandin())
        pm.menuItem('ArnoldExportOptionsStandIn', parent='ArnoldStandIn', optionBox=True,
                    c=lambda *args: doExportOptionsStandin())
        pm.menuItem('ArnoldLights', label='Lights', parent='ArnoldMenu', subMenu=True)
        
        pm.menuItem('ArnoldAreaLights', parent='ArnoldLights', label="Area Light",
                    c=lambda *args: mutils.createLocator('aiAreaLight'))
        pm.menuItem('SkydomeLight', parent='ArnoldLights', label="Skydome Light",
                    c=lambda *args: mutils.createLocator('aiSkyDomeLight'))

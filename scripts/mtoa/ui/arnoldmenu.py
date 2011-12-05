import pymel.core as pm
import maya.cmds as cmds
from mtoa.ui.ae.aiStandInTemplate import LoadStandInButtonPush

def doCreateStandIn():
    if not cmds.objExists('ArnoldStandInDefaultLightSet'):
        cmds.createNode("objectSet", name="ArnoldStandInDefaultLightSet", shared=True)
        cmds.lightlink(object='ArnoldStandInDefaultLightSet', light='defaultLightSet')

    pm.createNode('aiStandIn', n='ArnoldStandInShape')
    cmds.sets(add='ArnoldStandInDefaultLightSet')
    
def doCreateStandInFile():
    if not cmds.objExists('ArnoldStandInDefaultLightSet'):
        cmds.createNode("objectSet", name="ArnoldStandInDefaultLightSet", shared=True)
        cmds.lightlink(object='ArnoldStandInDefaultLightSet', light='defaultLightSet')

    pm.createNode('aiStandIn', n='ArnoldStandInShape')
    cmds.sets(add='ArnoldStandInDefaultLightSet')
    LoadStandInButtonPush()

def doExportStandin():
    pm.mel.eval('ExportSelection')
    pm.mel.eval('setCurrentFileTypeOption ExportActive "" "ArnoldSceneSource Exporter"')

def doExportOptionsStandin():
    pm.mel.eval('ExportSelectionOptions')
    pm.mel.eval('setCurrentFileTypeOption ExportActive "" "ArnoldSceneSource Exporter"')
   
def createArnoldMenu():
    # Add an Arnold menu in Maya main window
    if not pm.about(b=1):
        pm.menu('ArnoldMenu', label='Arnold', parent='MayaWindow', tearOff=True )
        pm.menuItem('ArnoldStandIn', label='StandIn', parent='ArnoldMenu', subMenu=True)
        pm.menuItem('ArnoldCreateStandIn', parent='ArnoldStandIn', label="Create",
                    c=lambda *args: doCreateStandIn())
        pm.menuItem('ArnoldCreateStandInFile', parent='ArnoldStandIn', optionBox=True,
                    c=lambda *args: doCreateStandInFile())
        pm.menuItem('ArnoldExportStandIn', parent='ArnoldStandIn', label='Export',
                    c=lambda *args: doExportStandin())
        pm.menuItem('ArnoldExportOptionsStandIn', parent='ArnoldStandIn', optionBox=True,
                    c=lambda *args: doExportOptionsStandin())

        pm.menuItem('ArnoldLights', label='Lights', parent='ArnoldMenu', subMenu=True)
        pm.menuItem('ArnoldAreaLights', parent='ArnoldLights', label="Area Light",
                    c=lambda *args: pm.shadingNode('aiAreaLight', name='aiAreaLight', asLight=True))
        pm.menuItem('SkydomeLight', parent='ArnoldLights', label="Skydome Light",
                    c=lambda *args: pm.shadingNode('aiSkyDomeLight', name='aiSkyDomeLight', asLight=True))
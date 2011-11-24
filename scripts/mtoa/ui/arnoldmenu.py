import pymel.core as pm

def doExportStandin():
    pm.mel.eval('ExportSelectionOptions')
    pm.mel.eval('setCurrentFileTypeOption ExportActive "" "ArnoldSceneSource"')

   
def createArnoldMenu():
    # Add an Arnold menu in Maya main window
    if not pm.about(b=1):
        pm.menu('ArnoldMenu', label='Arnold', parent='MayaWindow', tearOff=True )
        pm.menuItem('ArnoldStandIn', label='StandIn', parent='ArnoldMenu', subMenu=True)
        pm.menuItem('ArnoldCreateStandIn', parent='ArnoldStandIn', label="Create",
                    c=lambda *args: pm.createNode('aiStandIn', n='ArnoldStandInShape'))
        pm.menuItem('ArnoldExportStandIn', parent='ArnoldStandIn', label='Export',
                    c=lambda *args: doExportStandin())

        pm.menuItem('ArnoldLights', label='Lights', parent='ArnoldMenu', subMenu=True)
        pm.menuItem('ArnoldAreaLights', parent='ArnoldLights', label="Area Light",
                    c=lambda *args: pm.shadingNode('aiAreaLight', name='aiAreaLight', asLight=True))
        pm.menuItem('SkydomeLight', parent='ArnoldLights', label="Skydome Light",
                    c=lambda *args: pm.shadingNode('aiSkyDomeLight', name='aiSkyDomeLight', asLight=True))
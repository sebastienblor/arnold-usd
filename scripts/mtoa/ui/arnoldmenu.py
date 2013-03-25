import pymel.core as pm
from mtoa.core import createStandIn
from mtoa.ui.ae.aiStandInTemplate import LoadStandInButtonPush
import mtoa.utils as mutils
import maya.cmds as cmds

def doCreateStandInFile():
    node = createStandIn()
    LoadStandInButtonPush(node.name())

def doExportStandin():
    pm.mel.eval('ExportSelection')
    pm.mel.eval('setCurrentFileTypeOption ExportActive "" "ASS Export"')

def doExportOptionsStandin():
    pm.mel.eval('ExportSelectionOptions')
    pm.mel.eval('setCurrentFileTypeOption ExportActive "" "ASS Export"')
    
def doCreateMeshLight():
    sls = cmds.ls(sl=True, et='transform')
    if len(sls) == 0:
        cmds.confirmDialog(title='Error', message='No transform is selected!', button='Ok')
        return
    shs = cmds.listRelatives(sls[0], type='mesh')
    if shs is None:
        cmds.confirmDialog(title='Error', message='The selected transform has no meshes', button='Ok')
        return
    elif len(shs) == 0:
        cmds.confirmDialog(title='Error', message='The selected transform has no meshes', button='Ok')
        return
    ll = mutils.createLocator('aiAreaLight', asLight=True)
    lShape = ll[0]
    lLocator = ll[1]
    wm = cmds.xform(sls[0], query=True, matrix=True, worldSpace=True)
    cmds.xform(lLocator, worldSpace=True, matrix=wm)
    cmds.connectAttr('%s.outMesh' % shs[0], '%s.inputMesh' % lShape, force=True)
    cmds.setAttr('%s.aiTranslator' % lShape, 'mesh', type='string')

    
def arnoldAboutDialog():
    arnoldAboutText =  u"Arnold for Maya\n\n\n"
    arnoldAboutText += u"(c) 2001-2009 Marcos Fajardo and (c) 2009-2013\nSolid Angle SL\n\n"
    arnoldAboutText += u"Developed by: Ángel Jimenez, Olivier Renouard,\nYannick Puech, Borja Morales, Nicolas Dumay,\nPedro Fernando Gomez, Pál Mezei\n\n"
    arnoldAboutText += u"Acknowledgements: Javier González, Miguel González, \nChad Dombrova, Gaetan Guidet, Gaël Honorez,\nDiego Garcés, Kevin Tureski, Frédéric Servant"

    if (cmds.window("AboutArnold", ex=True)):
        cmds.deleteUI("AboutArnold")
    w = cmds.window("AboutArnold", title="About")
    cmds.window("AboutArnold", edit=True, width=402, height=260)
    cmds.rowColumnLayout( numberOfColumns=4, columnWidth=[(1,20), (2, 52), (3, 50), (4, 280)] )

    cmds.text(label="");cmds.text(label="");cmds.text(label="");cmds.text(label="")

    cmds.text(label="")
    cmds.image(image="MtoA_Logo.png")
    cmds.text(label="")
    cmds.text(align="left",label=arnoldAboutText)

    cmds.text(label="");cmds.text(label="\n");cmds.text(label="");cmds.text(label="")

    cmds.text(label="")
    cmds.text(label="")
    cmds.button( width=150,label='OK', command=('import maya.cmds as cmds;cmds.deleteUI(\"' + w + '\", window=True)') )
    cmds.setParent( '..' )
    
    cmds.showWindow(w)

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
                    c=lambda *args: mutils.createLocator('aiAreaLight', asLight=True))
        pm.menuItem('SkydomeLight', parent='ArnoldLights', label="Skydome Light",
                    c=lambda *args: mutils.createLocator('aiSkyDomeLight', asLight=True))
        pm.menuItem('ArnoldMeshLight', parent='ArnoldLights', label='Mesh Light',
                    c=lambda *args: doCreateMeshLight())
                    
        pm.menuItem('ArnoldFlush', label='Flush Caches', parent='ArnoldMenu', subMenu=True)
        pm.menuItem('ArnoldFlushTexture', parent='ArnoldFlush', label="Textures",
                    c=lambda *args: cmds.arnoldFlushCache(textures=True))
        pm.menuItem('ArnoldFlushBackground', parent='ArnoldFlush', label="Skydome Lights",
                    c=lambda *args: cmds.arnoldFlushCache(skydome=True))
        pm.menuItem('ArnoldFlushQuads', parent='ArnoldFlush', label="Quad Lights",
                    c=lambda *args: cmds.arnoldFlushCache(quads=True))
        pm.menuItem('ArnoldFlushAll', parent='ArnoldFlush', label="All",
                    c=lambda *args: cmds.arnoldFlushCache(flushall=True))
                    
        pm.menuItem('ArnoldAbout', label='About', parent='ArnoldMenu',
                    c=lambda *args: arnoldAboutDialog())


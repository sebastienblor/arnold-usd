import maya.cmds as cmds
import maya
import mtoa.ui.arnoldmenu as arnoldmenu
import mtoa.utils as mutils

def removeArnoldShelf():
   if cmds.shelfLayout('MtoA', exists=True):
      cmds.deleteUI('MtoA')

def createArnoldShelf():
   removeArnoldShelf()
   shelfTab = maya.mel.eval('global string $gShelfTopLevel;')
   maya.mel.eval('global string $mtoaShelf;')
   maya.mel.eval('$mtoaShelf = `shelfLayout -cellWidth 33 -cellHeight 33 -p $gShelfTopLevel MtoA`;')
   cmds.shelfButton(command=lambda *args: cmds.arnoldFlushCache(textures=True), annotation='Flush Texture Cache', image='pythonFamily.png')
   cmds.shelfButton(command=lambda *args: cmds.arnoldFlushCache(skydome=True), annotation='Flush Background Cache', image='pythonFamily.png')
   cmds.shelfButton(command=lambda *args: cmds.arnoldFlushCache(quads=True), annotation='Flush Quad Caches', image='pythonFamily.png')
   cmds.shelfButton(command=lambda *args: cmds.arnoldFlushCache(flushall=True), annotation='Flush All Caches', image='pythonFamily.png')
   cmds.shelfButton(command=lambda *args: arnoldmenu.arnoldTxManager(), annotation='TX Manager', image='pythonFamily.png')
   cmds.shelfButton(command=lambda *args: mutils.createLocator('aiAreaLight', asLight=True), annotation='Create Area Light', image='pythonFamily.png')
   cmds.shelfButton(command=lambda *args: mutils.createLocator('aiSkyDomeLight', asLight=True), annotation='Create SkyDome Light', image='pythonFamily.png')
   cmds.shelfButton(command=lambda *args: arnoldmenu.doCreateMeshLight(), annotation='Create Mesh Light', image='pythonFamily.png')

import maya.cmds as cmds
import maya
import mtoa.ui.arnoldmenu as arnoldmenu
import mtoa.utils as mutils
import mtoa.ui.globals.settings as settings

def removeArnoldShelf():
   if cmds.shelfLayout('MtoA', exists=True):
      cmds.deleteUI('MtoA')

def createPhysicalSky():
   if cmds.objExists('defaultArnoldRenderOptions'):
      settings.createBackground('aiPhysicalSky', None)
   else:
      cmds.confirmDialog(message='The Arnold Render Options node does not exists!')

def createArnoldShelf():
   removeArnoldShelf()
   shelfTab = maya.mel.eval('global string $gShelfTopLevel;')
   maya.mel.eval('global string $mtoaShelf;')
   maya.mel.eval('$mtoaShelf = `shelfLayout -cellWidth 32 -cellHeight 32 -p $gShelfTopLevel MtoA`;')
   cmds.shelfButton(command=lambda *args: cmds.arnoldFlushCache(textures=True), annotation='Flush Texture Cache', image='FlushTextureShelf.png', style='iconOnly')
   cmds.shelfButton(command=lambda *args: cmds.arnoldFlushCache(skydome=True), annotation='Flush Background Cache', image='FlushBackgroundShelf.png', style='iconOnly')
   cmds.shelfButton(command=lambda *args: cmds.arnoldFlushCache(quads=True), annotation='Flush Quad Caches', image='FlushQuadLightShelf.png', style='iconOnly')
   cmds.shelfButton(command=lambda *args: cmds.arnoldFlushCache(flushall=True), annotation='Flush All Caches', image='FlushAllCachesShelf.png', style='iconOnly')
   cmds.shelfButton(command=lambda *args: arnoldmenu.arnoldTxManager(), annotation='TX Manager', image='TXManagerShelf.png', style='iconOnly')
   cmds.shelfButton(command=lambda *args: mutils.createLocator('aiAreaLight', asLight=True), annotation='Create Area Light', image='AreaLightShelf.png', style='iconOnly')
   cmds.shelfButton(command=lambda *args: mutils.createLocator('aiSkyDomeLight', asLight=True), annotation='Create SkyDome Light', image='SkydomeLightShelf.png', style='iconOnly')
   cmds.shelfButton(command=lambda *args: createPhysicalSky(), annotation='Create Physical Sky', image='PhysicalSkyShelf.png', style='iconOnly')
   cmds.shelfButton(command=lambda *args: arnoldmenu.doCreateMeshLight(), annotation='Create Mesh Light', image='MeshLightShelf.png', style='iconOnly')
   cmds.shelfButton(command=lambda *args: mutils.createLocator('aiPhotometricLight', asLight=True), annotation='Create Photometric Light', image='PhotometricLightShelf.png', style='iconOnly')

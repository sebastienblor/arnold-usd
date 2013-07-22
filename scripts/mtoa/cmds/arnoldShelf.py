import maya.cmds as cmds
import maya
import mtoa.ui.arnoldmenu as arnoldmenu
import mtoa.utils as mutils
import mtoa.ui.globals.settings as settings

def removeArnoldShelf():
   if cmds.shelfLayout('Arnold', exists=True):
      cmds.deleteUI('Arnold')

def createPhysicalSky():
   if cmds.objExists('defaultArnoldRenderOptions'):
      settings.createBackground('aiPhysicalSky', None)
   else:
      cmds.confirmDialog(message='The Arnold Render Options node does not exists!')

def createArnoldShelf():
   mtoaMercurialID = cmds.arnoldPlugins(getMercurialID=True)
   try:
      if cmds.optionVar(exists='mtoaMercurialID'):
         mercurialID = cmds.optionVar(query='mtoaMercurialID')
         if mtoaMercurialID == mercurialID:
            return
         else:
            cmds.optionVar(sv=('mtoaMercurialID', mtoaMercurialID))
      else:
         cmds.optionVar(sv=('mtoaMercurialID', mtoaMercurialID))
   except:
      pass
   removeArnoldShelf()
   shelfTab = maya.mel.eval('global string $gShelfTopLevel;')
   maya.mel.eval('global string $arnoldShelf;')
   maya.mel.eval('$arnoldShelf = `shelfLayout -cellWidth 32 -cellHeight 32 -p $gShelfTopLevel Arnold`;')
   cmds.shelfButton(label='Flush Texture Cache', command=lambda *args: cmds.arnoldFlushCache(textures=True), annotation='Flush Texture Cache', image='FlushTextureShelf.png', style='iconOnly')
   cmds.shelfButton(label='Flush Background Cache', command=lambda *args: cmds.arnoldFlushCache(skydome=True), annotation='Flush Background Cache', image='FlushBackgroundShelf.png', style='iconOnly')
   cmds.shelfButton(label='Flush Quad Caches', command=lambda *args: cmds.arnoldFlushCache(quads=True), annotation='Flush Quad Caches', image='FlushQuadLightShelf.png', style='iconOnly')
   cmds.shelfButton(label='Flush All Caches', command=lambda *args: cmds.arnoldFlushCache(flushall=True), annotation='Flush All Caches', image='FlushAllCachesShelf.png', style='iconOnly')
   cmds.shelfButton(label='TX Manager', command=lambda *args: arnoldmenu.arnoldTxManager(), annotation='TX Manager', image='TXManagerShelf.png', style='iconOnly')
   cmds.shelfButton(label='Create Area Light', command=lambda *args: mutils.createLocator('aiAreaLight', asLight=True), annotation='Create Area Light', image='AreaLightShelf.png', style='iconOnly')
   cmds.shelfButton(label='Create SkyDome Light', command=lambda *args: mutils.createLocator('aiSkyDomeLight', asLight=True), annotation='Create SkyDome Light', image='SkydomeLightShelf.png', style='iconOnly')
   cmds.shelfButton(label='Create Physical Sky', command=lambda *args: createPhysicalSky(), annotation='Create Physical Sky', image='PhysicalSkyShelf.png', style='iconOnly')
   cmds.shelfButton(label='Create Mesh Light', command=lambda *args: arnoldmenu.doCreateMeshLight(), annotation='Create Mesh Light', image='MeshLightShelf.png', style='iconOnly')
   cmds.shelfButton(label='Create Photometric Light', command=lambda *args: mutils.createLocator('aiPhotometricLight', asLight=True), annotation='Create Photometric Light', image='PhotometricLightShelf.png', style='iconOnly') 

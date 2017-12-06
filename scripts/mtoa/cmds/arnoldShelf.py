import maya.cmds as cmds
import maya
import mtoa.core as core
import mtoa.ui.arnoldmenu as arnoldmenu
import mtoa.utils as mutils
import mtoa.ui.globals.settings as settings
def removeArnoldShelf():
   if cmds.shelfLayout('Arnold', exists=True):
      cmds.deleteUI('Arnold')

#def createPhysicalSky():
#   core.createOptions()
#   if cmds.objExists('defaultArnoldRenderOptions'):
#      settings.createBackground('aiPhysicalSky', None, False)
#   else:
#      cmds.confirmDialog(message='The Arnold Render Options node does not exists!')
#def createSky():
#   core.createOptions()
#   if cmds.objExists('defaultArnoldRenderOptions'):
#      settings.createBackground('aiSky', None, False)
#   else:
#      cmds.confirmDialog(message='The Arnold Render Options node does not exists!')

def createArnoldShelf():
   mtoaBuildID = cmds.arnoldPlugins(getBuildID=True)
   try:
      if cmds.optionVar(exists='mtoaBuildID'):
         buildID = cmds.optionVar(query='mtoaBuildID')
         if mtoaBuildID == buildID:
            return
         else:
            cmds.optionVar(sv=('mtoaBuildID', mtoaBuildID))
      else:
         cmds.optionVar(sv=('mtoaBuildID', mtoaBuildID))
   except:
      pass
   removeArnoldShelf()
   shelfTab = maya.mel.eval('global string $gShelfTopLevel;')
   maya.mel.eval('global string $arnoldShelf;')
   maya_version = cmds.about(version=True)
   if int(float(maya_version)) < 2017:
      maya.mel.eval('$arnoldShelf = `shelfLayout -cellWidth 32 -cellHeight 32 -p $gShelfTopLevel Arnold`;')   
   else:
      maya.mel.eval('$arnoldShelf = `shelfLayout -cellWidth 32 -cellHeight 32 -p $gShelfTopLevel -version \"2017\" Arnold`;')

   shelfStyle = ('shelf' if int(float(maya_version)) >= 2016 else 'simple')

   cmds.shelfButton(label='Create Area Light', command='import mtoa.utils as mutils;mutils.createLocator("aiAreaLight", asLight=True)', sourceType='python', annotation='Create Area Light', image='AreaLightShelf.png', style='iconOnly')
   cmds.shelfButton(label='Create Mesh Light', command='import mtoa.utils as mutils; mutils.createMeshLight()', sourceType='python', annotation='Create Mesh Light', image='MeshLightShelf.png', style='iconOnly')
   cmds.shelfButton(label='Create Photometric Light', command='import mtoa.utils as mutils; mutils.createLocator("aiPhotometricLight", asLight=True)', sourceType='python', annotation='Create Photometric Light', image='PhotometricLightShelf.png', style='iconOnly') 
   cmds.shelfButton(label='Create SkyDome Light', command='import mtoa.utils as mutils; mutils.createLocator("aiSkyDomeLight", asLight=True)', sourceType='python', annotation='Create SkyDome Light', image='SkydomeLightShelf.png', style='iconOnly')
   cmds.shelfButton(label='Create Light Portal', command='import mtoa.ui.arnoldmenu as arnoldmenu; arnoldmenu.doCreateLightPortal()', sourceType='python', annotation='Create Light Portal', image='LightPortalShelf.png', style='iconOnly')   
   #cmds.shelfButton(label='Create Sky', command='import mtoa.cmds.arnoldShelf as arnoldShelf; arnoldShelf.createSky()', sourceType='python', annotation='Create Sky', image='SkyShelf.png', style='iconOnly')
   cmds.shelfButton(label='Create Physical Sky', command='import mtoa.ui.arnoldmenu as arnoldmenu; arnoldmenu.doCreatePhysicalSky()', sourceType='python', annotation='Create Physical Sky', image='PhysicalSkyShelf.png', style='iconOnly')
   cmds.separator(width=12,height=35, style=shelfStyle, hr=False)
   cmds.shelfButton(label='Create StandIn', command='import mtoa.ui.arnoldmenu as arnoldmenu; arnoldmenu.createStandIn()', sourceType='python', annotation='Create StandIn', image='StandinShelf.png', style='iconOnly')      
   cmds.shelfButton(label='Export StandIn', command='import mtoa.ui.arnoldmenu as arnoldmenu; arnoldmenu.doExportStandIn()', sourceType='python', annotation='Export StandIn', image='ExportStandinShelf.png', style='iconOnly')      
   cmds.shelfButton(label='Create Curve Collector', command='import mtoa.ui.arnoldmenu as arnoldmenu; arnoldmenu.doCreateCurveCollector()', sourceType='python', annotation='Create Curve Collector', image='CurveCollectorShelf.png', style='iconOnly')      
   cmds.shelfButton(label='Create Volume', command='import mtoa.ui.arnoldmenu as arnoldmenu; arnoldmenu.createVolume()', sourceType='python', annotation='Create Volume', image='VolumeShelf.png', style='iconOnly')      
   cmds.separator(width=12,height=35, style=shelfStyle, hr=False)
   cmds.shelfButton(label='TX Manager', command='import mtoa.ui.arnoldmenu as arnoldmenu; arnoldmenu.arnoldTxManager()', sourceType='python', annotation='TX Manager', image='TXManagerShelf.png', style='iconOnly')
   cmds.shelfButton(label='Update TX files', command='import mtoa.ui.arnoldmenu as arnoldmenu; arnoldmenu.arnoldUpdateTx()', sourceType='python', annotation='Update TX files', image='UpdateTxShelf.png', style='iconOnly')
   cmds.shelfButton(label='Flush Texture Cache', command='import maya.cmds as cmds; cmds.arnoldFlushCache(textures=True)', sourceType='python', annotation='Flush Texture Cache', image='FlushTextureShelf.png', style='iconOnly')
   #cmds.shelfButton(label='Flush Background Cache', command='import maya.cmds as cmds; cmds.arnoldFlushCache(skydome=True)', sourceType='python', annotation='Flush Background Cache', image='FlushBackgroundShelf.png', style='iconOnly')
   #cmds.shelfButton(label='Flush Quad Caches', command='import maya.cmds as cmds; cmds.arnoldFlushCache(quads=True)', sourceType='python', annotation='Flush Quad Caches', image='FlushQuadLightShelf.png', style='iconOnly')
   #cmds.shelfButton(label='Flush All Caches', command='import maya.cmds as cmds;cmds.arnoldFlushCache(flushall=True)', sourceType='python', annotation='Flush All Caches', image='FlushAllCachesShelf.png', style='iconOnly')
   cmds.separator(width=12,height=35, style=shelfStyle, hr=False)
   #cmds.shelfButton(label='Light Manager', command='import mtoa.ui.arnoldmenu as arnoldmenu; arnoldmenu.arnoldLightManager()', sourceType='python', annotation='Light Manager', image='LightManagerShelf.png', style='iconOnly')
   cmds.shelfButton(label='Bake Selected Geometry', command='import mtoa.ui.arnoldmenu as arnoldmenu; arnoldmenu.arnoldBakeGeo()', sourceType='python', annotation='Bake Selected Geometry', image='BakeGeometryShelf.png', style='iconOnly')
   cmds.shelfButton(label='Render Selection to Texture', command='import mtoa.ui.arnoldmenu as arnoldmenu; arnoldmenu.arnoldRenderToTexture()', sourceType='python', annotation='Render Selection to Texture', image='RenderToTextureShelf.png', style='iconOnly')
   cmds.separator(width=12,height=35, style=shelfStyle, hr=False)
   cmds.shelfButton(label='Arnold RenderView', command='import mtoa.ui.arnoldmenu as arnoldmenu; arnoldmenu.arnoldOpenMtoARenderView()', sourceType='python', annotation='Arnold RenderView', image='RenderViewShelf.png', style='iconOnly')
   cmds.shelfButton(label='Render', command='import mtoa.ui.arnoldmenu as arnoldmenu; arnoldmenu.arnoldMtoARenderView()', sourceType='python', annotation='Render', image='RenderShelf.png', style='iconOnly')    



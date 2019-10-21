import mtoa.core as core
from mtoa.core import createStandIn, createVolume
from mtoa.ui.ae.aiStandInTemplate import LoadStandInButtonPush
import mtoa.utils as mutils
import mtoa.melUtils as melUtils
import maya.cmds as cmds
import maya.mel
import mtoa.txManager
import mtoa.lightManager
import mtoa.renderToTexture
import mtoa.materialx
import mtoa.operators
import mtoa.denoise
import mtoa.licensing
import arnold as ai
import mtoa.convertShaders
from maya.api import OpenMaya


from uuid import getnode as get_mac
import os
import sys

defaultFolder = ""
defaultOperatorsFolder = ""

_maya_version = mutils.getMayaVersion()
clmVersion = int(cmds.arnoldPlugins(getClmVersion=True))
if clmVersion > 1:
    import arnold.ai_license_clm

def doCreateStandInFile():
    node = createStandIn()
    LoadStandInButtonPush('{}.dso'.format(node))


def doExportStandIn():
    #Save the defaultType
    core.createOptions()
    default = cmds.optionVar(q='defaultFileExportActiveType')
    defaultBounds = cmds.getAttr('defaultArnoldRenderOptions.outputAssBoundingBox')
    cmds.setAttr('defaultArnoldRenderOptions.outputAssBoundingBox', 1)
    try:
        #Change it to ASS
        cmds.optionVar(sv=('defaultFileExportActiveType', "ASS Export"))
        maya.mel.eval('ExportSelection')
    finally:
        cmds.optionVar(sv=('defaultFileExportActiveType', default))

    cmds.setAttr('defaultArnoldRenderOptions.outputAssBoundingBox', defaultBounds)

def doExportOptionsStandIn():
    core.createOptions()
    defaultBounds = cmds.getAttr('defaultArnoldRenderOptions.outputAssBoundingBox')
    cmds.setAttr('defaultArnoldRenderOptions.outputAssBoundingBox', 1)

    maya.mel.eval('ExportSelectionOptions')
    maya.mel.eval('setCurrentFileTypeOption ExportActive "" "ASS Export"')

    cmds.setAttr('defaultArnoldRenderOptions.outputAssBoundingBox', defaultBounds)

def doCreateCurveCollector():
    # Get selection and group the curves ?
    sls = cmds.ls(sl=True, et='transform')
    curveNode = mutils.createLocator('aiCurveCollector')

    if len(sls) > 0:
        for slsElem in sls:
            shs = cmds.listRelatives(slsElem, fullPath=True, type='nurbsCurve', allDescendents=True)
            if shs is None:
                continue
            if len(shs):
                for shsElem in shs:
                    sts = cmds.listRelatives(shsElem, fullPath=True, parent=True)
                    if len(sts) > 0:
                        cmds.parent(sts, curveNode[1])
                
    cmds.select(curveNode, replace=True)

def doCreateCustomShape(shapeName):
    shapeNode = mutils.createLocator(shapeName)
    cmds.select(shapeNode, replace=True)

def doCreateOperator(opName):
    opNode = cmds.createNode(opName)
    cmds.select(opNode, replace=True)

def doCreateOldMeshLight():
    sls = cmds.ls(sl=True, fullPath=True, et='transform')
    if len(sls) == 0:
        cmds.confirmDialog(title='Error', message='No transform is selected!', button='Ok')
        return
    shs = cmds.listRelatives(sls[0], fullPath=True, type='mesh')
    if shs is None:
        cmds.confirmDialog(title='Error', message='The selected transform has no meshes', button='Ok')
        return
    elif len(shs) == 0:
        cmds.confirmDialog(title='Error', message='The selected transform has no meshes', button='Ok')
        return
    cmds.setAttr('%s.aiTranslator' % shs[0], 'mesh_light', type='string')

def doCreatePhysicalSky(): 
    skydome = mutils.createLocator('aiSkyDomeLight', asLight=True)
    physicalSkyShader = cmds.createNode('aiPhysicalSky')
    cmds.connectAttr(physicalSkyShader+".outColor", skydome[0] + ".color")

def doCreateLightPortal():

    sls = cmds.ls(sl=False, et='aiSkyDomeLight')
    if len(sls) == 0:
        msg = "Light Portals need a "
        msg += "SkyDome light in the scene"

        result = cmds.confirmDialog(
            title='No SkyDome Light found',
            message=msg
            #button=['OK', 'Cancel'],
            #defaultButton='OK',
            #cancelButton='Cancel',
            #dismissString='Cancel'
            )

        return

    mutils.createLocator('aiLightPortal', asLight=True)



def arnoldAboutDialog():
    legaltext = ""
    
    pluginPath = os.path.dirname(os.path.dirname(cmds.pluginInfo( 'mtoa', query=True, path=True)))
    pluginPath = os.path.join(pluginPath,"scripts","mtoa","ui","AboutArnold.txt")

    with open(pluginPath, "r") as fileHandle:
        legaltext = fileHandle.read()

    arnoldAboutText =  u"Arnold for Maya\n\n"
    arnoldAboutText += "MtoA " + cmds.pluginInfo( 'mtoa', query=True, version=True)
    arnoldBuildID = cmds.arnoldPlugins(getBuildID=True)
    mtoaBuildDate = cmds.arnoldPlugins(getBuildDate=True)
    if not '(Master)' in arnoldBuildID:
        arnoldAboutText += " - " + arnoldBuildID + " - " + mtoaBuildDate
    arnoldAboutText += "\nArnold Core "+".".join(ai.AiGetVersion())
    arnoldAboutText += "\nCLM V"+ cmds.arnoldPlugins(getClmVersion=True) + "\n\n"
    arnoldAboutText += u"Developed by: Ángel Jimenez, Olivier Renouard, Yannick Puech,\nBorja Morales, Nicolas Dumay, Pedro Fernando Gomez,\nPál Mezei, Sebastien Blaineau-Ortega, Ashley Handscomb Retallack,\nKrishnan Ramachandran\n\n"
    arnoldAboutText += u"Acknowledgements: Javier González, Miguel González, Lee Griggs,\nChad Dombrova, Gaetan Guidet, Gaël Honorez, Diego Garcés,\nKevin Tureski, Frédéric Servant, Darin Grant"

    if (cmds.window("AboutArnold", ex=True)):
        cmds.deleteUI("AboutArnold")
    w = cmds.window("AboutArnold", title="About")
    cmds.window("AboutArnold", edit=True, width=520, height=550)
    cmds.rowColumnLayout( numberOfColumns=4, columnWidth=[(1,20), (2, 52), (3, 50), (4, 380)] )

    cmds.text(label="");cmds.text(label="");cmds.text(label="");cmds.text(label="")

    cmds.text(label="")
    cmds.image(image="arnold_small.png")
    cmds.text(label="")
    cmds.text(align="left",label=arnoldAboutText)

    cmds.text(label="");cmds.text(label="");cmds.text(label="");cmds.text(label="")
    
    cmds.text(label="");cmds.text(label="");cmds.text(label="");
    
    cmds.scrollField(editable=False, wordWrap=True, font="plainLabelFont", height=200, text=legaltext)
    
    cmds.text(label="");cmds.text(label="");cmds.text(label="");cmds.text(label="")

    cmds.text(label="");cmds.text(label="\n");cmds.text(label="");cmds.text(label="")

    cmds.text(label="")
    cmds.text(label="")
    cmds.button( width=150,label='OK', command=('import maya.cmds as cmds;cmds.deleteUI(\"' + w + '\", window=True)') )
    cmds.setParent( '..' )
    
    cmds.showWindow(w)

def arnoldLicenseManager():
    mtoa.licensing.licenseManager()

def arnoldLicensingSignIn():
    ai.ai_license_clm.AiLicenseClmSignIn()

def arnoldLicensingSignOut():
    ai.ai_license_clm.AiLicenseClmSignOut()

def arnoldLicensingLicenseManager():
    ai.ai_license_clm.AiLicenseClmLicenseManager()  
        
def arnoldLicensingGetMacAddress():
    if (cmds.window("ArnoldLicenseGetMacAddress", ex=True)):
        cmds.deleteUI("ArnoldLicenseGetMacAddress")
    w = cmds.window("ArnoldLicenseGetMacAddress", sizeable=False, title="Get MAC Address")
    cmds.window("ArnoldLicenseGetMacAddress", edit=True, width=240, height=60)
    
    cmds.columnLayout()

    cmds.rowColumnLayout( numberOfColumns=3, columnWidth=[(1,10),(2,90),(3,140)] )
    cmds.text(align="left", label="")
    cmds.text(align="left", label="MAC Address")
    name = cmds.textField()
    mac = get_mac()
    mactext = ("%012X" % mac)
    cmds.textField(name,  edit=True, text=mactext, editable=False )
    cmds.setParent( '..' )
    cmds.rowColumnLayout( numberOfColumns=5, columnWidth=[(1,10),(2,70),(3,80), (4, 70), (5,10)])
    cmds.text(align="left",label="")
    commandStr = 'import maya.cmds as cmds;cmds.arnoldLicense(copyToClipboard=\"' + mactext+'\")'
    cmds.button( align="left", label='Copy', command=(commandStr))
    cmds.text(label="")
    cmds.button(align="right", label='Close', command=('import maya.cmds as cmds;cmds.deleteUI(\"' + w + '\", window=True)'))
    cmds.text(align="right", label="")
    cmds.setParent( '..' )
    cmds.showWindow(w)

def arnoldLicensingConnectLicenseServer():
    win = mtoa.licensing.ConnectToLicenseServer()
    win.create()
def arnoldLicensingGetDiagnostics():
    win = mtoa.licensing.GetDiagnostics()
    win.create()

def arnoldLicensingNodeLocked():
    win = mtoa.licensing.NodeLocked()
    win.create()
    
def arnoldTxManager():
    core.createOptions()
    win = mtoa.txManager.MtoATxManager()
    win.create()
    win.refreshList()

def arnoldUpdateTx():
    core.createOptions()
    cmds.arnoldUpdateTx()
    
def arnoldConvertDeprecated():
    mtoa.convertShaders.convertArnoldShaders()

    
def arnoldLightManager():
    win = mtoa.lightManager.MtoALightManager()
    win.create()

def arnoldBakeGeo():
    objFilter = "Obj File (*.obj)"
    global defaultFolder
    if defaultFolder == "":
        defaultFolder = cmds.workspace(q=True,rd=True, fn=True)

    ret = cmds.fileDialog2(cap='Bake Selection as OBJ', fm=0, ff=objFilter, dir=defaultFolder)
    if ret is not None and len(ret):
        defaultFolder = ret[0]
        cmds.arnoldBakeGeo(f=defaultFolder)

def arnoldDenoise():
    win = mtoa.denoise.MtoANoice()
    win.create()    


def arnoldExportOperators():
    win = mtoa.operators.MtoAExportOperatorGraph()
    win.create()


def arnoldImportOperators():
    global defaultOperatorsFolder
    if defaultOperatorsFolder == "":
        defaultOperatorsFolder = cmds.workspace(q=True,rd=True, fn=True)

    objFilter = "ASS File (*.ass)"
    ret = cmds.fileDialog2(cap='Import Operator Graph',okc='Select',fm=1,ff=objFilter,dir=defaultOperatorsFolder) or []
    if len(ret):
        defaultOperatorsFolder = ret[0]
        cmds.arnoldImportAss(f=ret[0],  mask=ai.AI_NODE_OPERATOR + ai.AI_NODE_SHADER)

def arnoldExportShaders():
    selection = cmds.ls(selection=True)
    global defaultFolder
    if defaultFolder == "":
        defaultFolder = cmds.workspace(q=True,rd=True, fn=True)
    ret = cmds.fileDialog2(cap='Select File',okc='Select',ff="Arnold Shaders File (*.ass)",fm=0,dir=defaultFolder) or []
    if len(ret) > 0:
        cmds.arnoldExportAss(filename = ret[0], selected=True, exportAllShadingGroups=True, mask = ai.AI_NODE_SHADER)

def arnoldImportShaders():
    global defaultOperatorsFolder
    if defaultOperatorsFolder == "":
        defaultOperatorsFolder = cmds.workspace(q=True,rd=True, fn=True)

    objFilter = "ASS File (*.ass);; AXF File (*.axf);; MaterialX File (*.mtlx)"
    ret = cmds.fileDialog2(cap='Import Arnold Shaders',okc='Select',fm=1,ff=objFilter,dir=defaultOperatorsFolder) or []
    if len(ret):
        defaultOperatorsFolder = ret[0]
        cmds.arnoldImportAss(f=ret[0], mask=ai.AI_NODE_SHADER)

def arnoldExportMaterialx(selected=False):
    selList = cmds.ls(sl=1)
    if (len(selList) == 0):
        cmds.confirmDialog( title='Export to MaterialX', message='No Geometry Selected', button=['Ok'], defaultButton='Ok', cancelButton='Ok', dismissString='Ok' )
        return False
    win = mtoa.materialx.MtoAExportToMaterialX()
    win.create()

def arnoldRenderToTexture():
    selList = cmds.ls(sl=1)
    if (len(selList) == 0):
        cmds.confirmDialog( title='Render To Texture', message='No Geometry Selected', button=['Ok'], defaultButton='Ok', cancelButton='Ok', dismissString='Ok' )
        return False

    win = mtoa.renderToTexture.MtoARenderToTexture()
    win.create()

def arnoldOpenMtoARenderView():
    cmds.arnoldRenderView(mode ="open")

def arnoldMtoARenderView():
    # core.ACTIVE_CAMERA is not set, anything we could do here ?
    #if core.ACTIVE_CAMERA != None:
    #    cmds.arnoldRenderView(cam=core.ACTIVE_CAMERA)
    # so instead we're calling it without any argument
    cmds.arnoldRenderView()

def addRuntimeMenuItem(name, parent, command, label = '', rtcLabel = '', tearOff=False, optionBox=False, image='', annotation='', category='', keywords ='', tags=''):
    
    tags = 'Render;Arnold' if (tags == '') else 'Render;Arnold;{}'.format(tags)
    category = 'Menu items.Mainframe.Arnold' if (category == '') else 'Menu items.Mainframe.Arnold.{}'.format(category)
    rtcLabel = 'Arnold: {}'.format(label) if (rtcLabel == '') else rtcLabel

    if _maya_version < 2019:
        cmds.menuItem(name, label=label, parent=parent, c=command, tearOff=tearOff, optionBox=optionBox, image=image)
    else:
        cmds.runTimeCommand('cmd{}'.format(name), d=True, label=rtcLabel, annotation=annotation, category=category, keywords=keywords, tags=tags, command=command, image=image )
        cmds.menuItem(name, parent=parent, rtc='cmd{}'.format(name), label = label, sourceType= 'mel', optionBox=optionBox, tearOff=tearOff)

def GPUCacheStart():
    mutils.populate_GPUCache()

def GPUCacheStop():
    mutils.terminate_GPUCache();

def createArnoldMenu():
    # Add an Arnold menu in Maya main window
    if not cmds.about(b=1):
        
        if _maya_version < 2017:
            cmds.menu('ArnoldMenu', label='Arnold', parent='MayaWindow', tearOff=True )
        else:
            cmds.menu('ArnoldMenu', label='Arnold', parent='MayaWindow', tearOff=True, version="2017" )

        addRuntimeMenuItem('ArnoldRender', label='Render', parent='ArnoldMenu', rtcLabel = 'Arnold: Render', image='RenderShelf.png', 
                    command='import mtoa.ui.arnoldmenu;mtoa.ui.arnoldmenu.arnoldMtoARenderView()', keywords='arv;ipr', annotation ='Render with the Arnold RenderView')
        
        addRuntimeMenuItem('ArnoldMtoARenderView', label='Open Arnold RenderView', rtcLabel = 'Arnold: Open RenderView', parent='ArnoldMenu',  image='RenderViewShelf.png',
                    command='import mtoa.ui.arnoldmenu;mtoa.ui.arnoldmenu.arnoldOpenMtoARenderView()', keywords='arv;ipr', annotation='Open the Arnold RenderView window')
        cmds.menuItem(parent='ArnoldMenu', divider=True)

        cmds.menuItem('ArnoldStandIn', label='StandIn', parent='ArnoldMenu', subMenu=True, tearOff=True)

        addRuntimeMenuItem('ArnoldCreateStandIn', parent='ArnoldStandIn', label="Create StandIn", image='StandinShelf.png',
                    command='import mtoa.ui.arnoldmenu;mtoa.ui.arnoldmenu.createStandIn()', category='StandIn', keywords='procedural;proxy;archive;import', annotation='Create a StandIn to load a .ass file')
        addRuntimeMenuItem('ArnoldCreateStandInFile', parent='ArnoldStandIn', optionBox=True,  
                    command='import mtoa.ui.arnoldmenu;mtoa.ui.arnoldmenu.doCreateStandInFile()', rtcLabel='Arnold: Create StandIn from File', category='StandIn', keywords='procedural;proxy;archive;import', annotation='Create a StandIn to load a .ass file')
        addRuntimeMenuItem('ArnoldExportStandIn', parent='ArnoldStandIn', label='Export StandIn', image='ExportStandinShelf.png',
                    command='import mtoa.ui.arnoldmenu;mtoa.ui.arnoldmenu.doExportStandIn()', category='StandIn', keywords='procedural;proxy;archive', annotation='Export the selection as a Standin .ass file')
        cmds.menuItem('ArnoldExportOptionsStandIn', parent='ArnoldStandIn', optionBox=True,
                    command='import mtoa.ui.arnoldmenu;mtoa.ui.arnoldmenu.doExportOptionsStandIn()')

        cmds.menuItem('ArnoldLights', label='Lights', parent='ArnoldMenu', subMenu=True, tearOff=True)
        
        addRuntimeMenuItem('ArnoldAreaLights', parent='ArnoldLights', label="Area Light",rtcLabel = 'Arnold: Create Area Light', image='AreaLightShelf.png',
                    command='import mtoa.utils;mtoa.utils.createLocator("aiAreaLight", asLight=True)', category='Lights', keywords='quad;softbox', annotation='Create an Arnold Area Light (Quad/Cylinder/Disk)')
        addRuntimeMenuItem('SkydomeLight', parent='ArnoldLights', label="Skydome Light", rtcLabel='Arnold: Create Skydome Light', image='SkydomeLightShelf.png',
                    command='import mtoa.utils;mtoa.utils.createLocator("aiSkyDomeLight", asLight=True)', category='Lights', keywords='ibl;hdr;dome;env', annotation='Create an Arnold Skydome Light for Environment/IBL lighting')
        addRuntimeMenuItem('ArnoldMeshLight', parent='ArnoldLights', label='Mesh Light', rtcLabel='Arnold: Create Mesh Light', image='MeshLightShelf.png',
                    command='import mtoa.utils;mtoa.utils.createMeshLight()', category='Lights', keywords='geo light;object light', annotation='Convert the selected Mesh to an Arnold Mesh Light')
        addRuntimeMenuItem('PhotometricLights', parent='ArnoldLights', label="Photometric Light", rtcLabel="Arnold: Create Photometric Light", image='PhotometricLightShelf.png', keywords='ies;physical light;light map;practical light',
                    command='import mtoa.utils;mtoa.utils.createLocator("aiPhotometricLight", asLight=True)', category='Lights', annotation='Create an Arnold Photometric Light to load IES files')
        addRuntimeMenuItem('LightPortal', parent='ArnoldLights', label="Light Portal", rtcLabel="Arnold: Create Light Portal", image='LightPortalShelf.png',
                    command='import mtoa.ui.arnoldmenu;mtoa.ui.arnoldmenu.doCreateLightPortal()', keywords='hdr;ibl;env;dome', annotation='Portals can optimize Skydomes interior lighting', category='Lights')
        addRuntimeMenuItem('PhysicalSky', parent='ArnoldLights', label="Physical Sky", rtcLabel="Arnold: Create Physical Sky", image='PhysicalSkyShelf.png', keywords='sun;sky;env;dome;ibl',
                    command='import mtoa.ui.arnoldmenu;mtoa.ui.arnoldmenu.doCreatePhysicalSky()', category='Lights', annotation='Create an Arnold Skydome w/ Physical Sky illumination')
        
        customShapes = cmds.arnoldPlugins(listCustomShapes=True)
        if customShapes and len(customShapes) > 0:
            cmds.menuItem('ArnoldCustomShapes', label='Custom Shapes', parent='ArnoldMenu', subMenu=True, tearOff=True)
            for customShape in customShapes:
                addRuntimeMenuItem(customShape, parent='ArnoldCustomShapes', label=customShape, rtcLabel='Arnold: Create {} Shape'.format(customShape),
                    command='import mtoa.ui.arnoldmenu;mtoa.ui.arnoldmenu.doCreateCustomShape("{}")'.format(customShape), category='Custom Shapes')

        operators = cmds.arnoldPlugins(listOperators=True)
        if operators and len(operators) > 0:
            cmds.menuItem('ArnoldOperators', label='Operators', parent='ArnoldMenu', subMenu=True, tearOff=True)
            for operator in operators:
                addRuntimeMenuItem(operator, parent='ArnoldOperators', label=operator, rtcLabel='Arnold: Create {} Operator'.format(operator),
                    command='import mtoa.ui.arnoldmenu;mtoa.ui.arnoldmenu.doCreateOperator("{}")'.format(operator), category = 'Operators')

        addRuntimeMenuItem('CurveCollector', label='Curve Collector', rtcLabel='Arnold: Create Curve Collector', parent='ArnoldMenu', image='CurveCollectorShelf.png',
                    command='import mtoa.ui.arnoldmenu;mtoa.ui.arnoldmenu.doCreateCurveCollector()', annotation="Group the selected curves in an Arnold Curve Collector")
        addRuntimeMenuItem('ArnoldVolume', label='Volume', rtcLabel='Arnold: Create Volume', parent='ArnoldMenu', image='VolumeShelf.png', 
                    command='import mtoa.ui.arnoldmenu;mtoa.ui.arnoldmenu.createVolume()', keywords='vdb;openvdb', annotation="Create an Arnold Volume node to load a VDB file")
                    
        cmds.menuItem('ArnoldFlush', label='Flush Caches', parent='ArnoldMenu', subMenu=True, tearOff=True)
        addRuntimeMenuItem('ArnoldFlushTexture', parent='ArnoldFlush', label="Textures", rtcLabel='Arnold: Flush Texture Cache', image='FlushTextureShelf.png', 
                    command='import maya.cmds;maya.cmds.arnoldFlushCache(textures=True)', category="Flush Caches")
        addRuntimeMenuItem('ArnoldFlushSelectedTextures', parent='ArnoldFlush', label="Selected Textures", rtcLabel = 'Arnold: Flush selected textures', image='FlushTextureShelf.png', 
                    command='import maya.cmds;maya.cmds.arnoldFlushCache(selected_textures=True)', category="Flush Caches")
        addRuntimeMenuItem('ArnoldFlushBackground', parent='ArnoldFlush', label="Skydome Lights", rtcLabel = 'Arnold: Flush Skydome Cache', image='FlushBackgroundShelf.png',
                    command='import maya.cmds;maya.cmds.arnoldFlushCache(skydome=True)', category="Flush Caches")
        addRuntimeMenuItem('ArnoldFlushQuads', parent='ArnoldFlush', label="Quad Lights", rtcLabel='Arnold: Flush Area Lights Cache', image='FlushQuadLightShelf.png',
                    command='import maya.cmds; maya.cmds.arnoldFlushCache(quads=True)', category="Flush Caches")
        addRuntimeMenuItem('ArnoldFlushAll', parent='ArnoldFlush', label="All", rtcLabel='Arnold: Flush All Caches', image='FlushAllCachesShelf.png',
                    command='import maya.cmds; maya.cmds.arnoldFlushCache(flushall=True)', category="Flush Caches")
                    
        cmds.menuItem('ArnoldUtilities', label='Utilities', parent='ArnoldMenu', subMenu=True, tearOff=True)
        addRuntimeMenuItem('ArnoldDenoise', label='Arnold Denoiser (noice)', parent='ArnoldUtilities', 
                    command='import mtoa.ui.arnoldmenu;mtoa.ui.arnoldmenu.arnoldDenoise()', category="Utilities", keywords='noice;denoising', annotation="Denoise an image or a sequence using the Arnold denoise \"noice\"")
        addRuntimeMenuItem('ArnoldBakeGeo', label='Bake Selected Geometry', parent='ArnoldUtilities', image='BakeGeometryShelf.png', 
                    command='import mtoa.ui.arnoldmenu;mtoa.ui.arnoldmenu.arnoldBakeGeo()', category="Utilities", keywords='baking', annotation='Bake the selected geometry to OBJ (w/ subdivision and displacement)')
        addRuntimeMenuItem('ArnoldRenderToTexture', label='Render Selection To Texture', parent='ArnoldUtilities', image='RenderToTextureShelf.png', 
                    command='import mtoa.ui.arnoldmenu;mtoa.ui.arnoldmenu.arnoldRenderToTexture()', category="Utilities", keywords='bake;baking', annotation="Shade the selected shape and bake it on a UV texture")
        addRuntimeMenuItem('ArnoldTxManager', label='TX Manager', parent='ArnoldUtilities', image='TXManagerShelf.png', keywords='textures;convert;optimize',
                    command='import mtoa.ui.arnoldmenu;mtoa.ui.arnoldmenu.arnoldTxManager()', category="Utilities", annotation='Open the Arnold TX Manager')
        addRuntimeMenuItem('ArnoldUpdateTx', label='Update TX Files', parent='ArnoldUtilities',  image='UpdateTxShelf.png', keywords='textures',
                    command='import mtoa.ui.arnoldmenu;mtoa.ui.arnoldmenu.arnoldUpdateTx()', category="Utilities", annotation="Convert / Updates all textures to TX for Arnold rendering")
        cmds.menuItem('ArnoldLightManager', label='Light Manager', parent='ArnoldUtilities', image='LightManagerShelf.png', 
                    command='import mtoa.ui.arnoldmenu;mtoa.ui.arnoldmenu.arnoldLightManager()')
        cmds.menuItem('ArnoldConvertShaders', label='Convert Shaders to Arnold', parent='ArnoldUtilities',
                    command='import mtoa.ui.arnoldmenu;mtoa.ui.arnoldmenu.arnoldConvertDeprecated()')
        addRuntimeMenuItem('ArnoldExportOperators', label='Export Operator Graph', parent='ArnoldUtilities', keywords='operator',
                    command='import mtoa.ui.arnoldmenu;mtoa.ui.arnoldmenu.arnoldExportOperators()', category="Utilities", annotation='Export an operator graph to a .ass file')
        addRuntimeMenuItem('ArnoldImportOperators', label='Import Operator Graph', parent='ArnoldUtilities', keywords='operator',
                    command='import mtoa.ui.arnoldmenu;mtoa.ui.arnoldmenu.arnoldImportOperators()', category="Utilities", annotation='Import an operator graph from a .ass file')
        addRuntimeMenuItem('ArnoldExportShaders', label='Export Selected Shaders', parent='ArnoldUtilities', keywords='operator',
                    command='import mtoa.ui.arnoldmenu;mtoa.ui.arnoldmenu.arnoldExportShaders()', category="Utilities", annotation='Export the selected shaders to a .ass file')
        addRuntimeMenuItem('ArnoldImportShaders', label='Import Arnold Shaders', parent='ArnoldUtilities', keywords='operator',
                    command='import mtoa.ui.arnoldmenu;mtoa.ui.arnoldmenu.arnoldImportShaders()', category="Utilities", annotation='Import a shader library from a .ass file')
        addRuntimeMenuItem('ArnoldExportSelectedToMaterialx', label='Export Selection to MaterialX', parent='ArnoldUtilities', keywords='materialx',
                    command='import mtoa.ui.arnoldmenu;mtoa.ui.arnoldmenu.arnoldExportMaterialx(selected=True)', category="Utilities", annotation='Export the selected shading trees to .mtlx files')
        addRuntimeMenuItem('GPUCache', label='Pre-populate GPU Cache', parent='ArnoldUtilities', keywords='GPU',
                    command='import mtoa.ui.arnoldmenu;mtoa.ui.arnoldmenu.GPUCacheStart()', category="Utilities", annotation='Pre-Populate the Optix GPU Cache')
        addRuntimeMenuItem('AbortGPUCache', label='Abort GPU Cache pre-population', parent='ArnoldUtilities', keywords='GPU',
                    command='import mtoa.ui.arnoldmenu;mtoa.ui.arnoldmenu.GPUCacheStop()', category="Utilities", annotation='Terminate the Optix GPU cache creation')
        cmds.menuItem('ArnoldLicensingMenu', label='Licensing', parent='ArnoldMenu',
                    subMenu=True, tearOff=True)
        cmds.menuItem('ArnoldLicenseManager', label='License Manager', parent='ArnoldLicensingMenu',
                    c=lambda *args: arnoldLicenseManager())
        cmds.menuItem('ArnoldLicensingHelp',  label='Licensing Help', parent='ArnoldLicensingMenu', 
                    c=lambda *args: cmds.launch(webPage='https://docs.arnoldrenderer.com/display/A5AILIC/Licensing+Home'))
        cmds.menuItem('ArnoldSuscribe',  label='Purchase Subscription', parent='ArnoldLicensingMenu', 
                    c=lambda *args: cmds.launch(webPage='https://www.solidangle.com/arnold/buy/'))
        
        '''
        cmds.menuItem('ArnoldConnectLicenseServer', label='Connect to License Server', parent='ArnoldLicensingMenu',
                    c=lambda *args: arnoldLicensingConnectLicenseServer())
        cmds.menuItem('ArnoldGetDiagnostics', label='Diagnostics', parent='ArnoldLicensingMenu',
                    c=lambda *args: arnoldLicensingGetDiagnostics())
        cmds.menuItem('ArnoldTroubleshootWatermarks', label='Troubleshoot Watermarks', parent='ArnoldLicensingMenu', 
                    c=lambda *args: cmds.launch(webPage='https://support.solidangle.com/x/LAAzAg'))
        
        cmds.menuItem('ArnoldGetMacAddress', label='Get MAC Address', parent='ArnoldLicensingMenu',
                    c=lambda *args: arnoldLicensingGetMacAddress())

        setupServerLink = 'https://support.solidangle.com/display/A5AILIC/Setting+up+a+License+Server'
        platformName = sys.platform

        if platformName.startswith('win'):
            setupServerLink = 'https://support.solidangle.com/x/IQAzAg'
        elif platformName.startswith('linux'):
            setupServerLink = 'https://support.solidangle.com/x/JwAzAg'
        elif platformName.startswith('darwin'):
            setupServerLink = 'https://support.solidangle.com/x/EQAzAg'

        cmds.menuItem('ArnoldSetupLicenseServer', label='Setup License Server', parent='ArnoldLicensingMenu', 
                    c=lambda *args: cmds.launch(webPage=setupServerLink))

        cmds.menuItem('ArnoldInstallTrialLicense', label='Install Trial License', parent='ArnoldLicensingMenu',
                    c=lambda *args: arnoldLicensingNodeLocked())
        '''

        cmds.menuItem('ArnoldHelpMenu', label='Help', parent='ArnoldMenu', 
                    subMenu=True, tearOff=True)

        cmds.menuItem('ArnoldUserGuide', label='User Guide', parent='ArnoldHelpMenu',
                    c=lambda *args: cmds.launch(webPage='https://support.solidangle.com/display/a5AFMUG/Arnold+for+Maya+User+Guide'))

        cmds.menuItem('ArnoldTutorials', label='Tutorials', parent='ArnoldHelpMenu',
                    c=lambda *args: cmds.launch(webPage='https://support.solidangle.com/display/A5AFMUG/Tutorials'))

#        cmds.menuItem('ArnoldVideos', label='Videos', parent='ArnoldHelpMenu',
#                    c=lambda *args: cmds.launch(webPage='https://support.solidangle.com/display/AFMUG/Video+Tutorials'))

        cmds.menuItem('ArnoldAnswers', label='Arnold Answers', parent='ArnoldHelpMenu',
                    c=lambda *args: cmds.launch(webPage='https://answers.arnoldrenderer.com/index.html'))

        cmds.menuItem('ArnoldLearningScenes', label='Learning Scenes', parent='ArnoldHelpMenu',
                    c=lambda *args: cmds.launch(webPage='https://support.solidangle.com/display/A5AFMUG/Learning+Scenes'))

        cmds.menuItem(divider=1, parent='ArnoldHelpMenu')

        cmds.menuItem('ArnoldSolidAngle', label='Solid Angle', parent='ArnoldHelpMenu',
                    c=lambda *args: cmds.launch(webPage='https://www.solidangle.com'))

        cmds.menuItem('ArnoldMailingLists', label='Mailing Lists', parent='ArnoldHelpMenu',
                    c=lambda *args: cmds.launch(webPage='https://subscribe.solidangle.com'))
        
#        cmds.menuItem('ArnoldAsk', label='Knowledge Base', parent='ArnoldHelpMenu',
#                    c=lambda *args: cmds.launch(webPage='https://ask.solidangle.com'))

        cmds.menuItem('ArnoldSupportBlog', label='Support Blog', parent='ArnoldHelpMenu',
                    c=lambda *args: cmds.launch(webPage='https://support.solidangle.com/blog/arnsupp'))

#        cmds.menuItem('ArnoldLicensing', label='Licensing', parent='ArnoldHelpMenu',
#                    c=lambda *args: arnoldLicenseDialog())

        cmds.menuItem(divider=1, parent='ArnoldHelpMenu')

        cmds.menuItem('ArnoldDeveloperGuide', label='Developer Guide', parent='ArnoldHelpMenu',
                    c=lambda *args: cmds.launch(webPage='https://support.solidangle.com/display/A5ARP/Arnoldpedia'))
        
        cmds.menuItem('MtoAReleaseNotes', label='Release Notes', parent='ArnoldHelpMenu',
        c=lambda *args: cmds.launch(webPage= 'https://docs.arnoldrenderer.com/display/A5AFMUG/' + str(cmds.pluginInfo( 'mtoa', query=True, version=True))))
                    
        cmds.menuItem('ArnoldAbout', label='About', parent='ArnoldMenu', image ='menuIconHelp.png',
                    c=lambda *args: arnoldAboutDialog())
        cmds.menuItem(divider=1, parent='ArnoldMenu')

        clmVersion = cmds.arnoldPlugins(getClmVersion=True)
        if int(clmVersion) > 1:
            cmds.menuItem('ArnoldLicensingLicenseManager', label='Manage License...', parent='ArnoldMenu',
                        c=lambda *args: arnoldLicensingLicenseManager())

            darkSite = int(os.getenv('ADLSDK_DARK_SITE') or 0)
            if not darkSite:
                cmds.menuItem('ArnoldLicensingSignIn', label='Sign-In', parent='ArnoldMenu',
                            c=lambda *args: arnoldLicensingSignIn())
                cmds.menuItem('ArnoldLicensingSignOut', label='Sign-Out', parent='ArnoldMenu',
                            c=lambda *args: arnoldLicensingSignOut())
            
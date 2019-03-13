
from mtoa.ui.ae.templates import createTranslatorMenu
from mtoa.callbacks import *
import mtoa.core as core
import arnold as ai
import maya.cmds as cmds
import maya.mel
import mtoa.utils as utils
import sys

def updateRenderSettings(*args):
    flag = cmds.getAttr('defaultArnoldRenderOptions.threads_autodetect') == False
    cmds.attrControlGrp('os_threads', edit=True, enable=flag)
    flag = cmds.getAttr('defaultArnoldRenderOptions.renderUnit') == 1
    cmds.attrControlGrp('os_scene_scale', edit=True, enable=flag)
    
def updateAutotileSettings(*args):
    flag = cmds.getAttr('defaultArnoldRenderOptions.autotile')
    cmds.attrControlGrp('ts_texture_autotile', edit=True, enable=flag)

def updateAutoTxSettings(*args):
    flag = cmds.getAttr('defaultArnoldRenderOptions.autotx') == 0
    cmds.attrControlGrp('use_existing_tiled_textures', edit=True, enable=flag)


def updateSamplingSettings(*args):
    flag = (cmds.getAttr('defaultArnoldRenderOptions.use_sample_clamp') == True) 
    cmds.attrControlGrp('ss_max_value', edit=True, enable=flag)
    cmds.attrControlGrp('ss_clamp_sample_values_AOVs', edit=True, enable=flag)

def calculateRayCounts(AASamples, rayTypeSamples, rayTypeDepth):
    computed = 0
    computedDepth = 0

    if rayTypeDepth > 1:
        computed = AASamples * rayTypeSamples * rayTypeSamples
        computedDepth = (rayTypeSamples * rayTypeSamples + rayTypeDepth - 1) * AASamples
    elif rayTypeDepth == 1:
        computed = AASamples * rayTypeSamples * rayTypeSamples
        computedDepth = computed

    return (computed, computedDepth)

def updateComputeSamples(*args):
    
    isGpu = cmds.getAttr('defaultArnoldRenderOptions.renderDevice')

    if (isGpu):
        cmds.attrControlGrp('ss_hemi_samples', edit=True, enable=False)
        cmds.attrControlGrp('ss_specular_samples', edit=True, enable=False)
        cmds.attrControlGrp('ss_transmission_samples', edit=True, enable=False)
        cmds.attrControlGrp('ss_sss_samples', edit=True, enable=False)
        cmds.attrControlGrp('ss_volume_samples', edit=True, enable=False)
        if cmds.control('gpu_max_texturemax_texture_resolution', exists = True):
            cmds.attrControlGrp('gpu_max_texturemax_texture_resolution', edit = True , enable = True)
    else :
        cmds.attrControlGrp('ss_hemi_samples', edit=True, enable=True)
        cmds.attrControlGrp('ss_specular_samples', edit=True, enable=True)
        cmds.attrControlGrp('ss_transmission_samples', edit=True, enable=True)
        cmds.attrControlGrp('ss_sss_samples', edit=True, enable=True)
        cmds.attrControlGrp('ss_volume_samples', edit=True, enable=True)
        if cmds.control('gpu_max_texturemax_texture_resolution', exists = True):
            cmds.attrControlGrp('gpu_max_texturemax_texture_resolution', edit = True , enable = False)

    
    updateAdaptiveSettings()

    AASamples = cmds.getAttr('defaultArnoldRenderOptions.AASamples')
    GISamples = cmds.getAttr('defaultArnoldRenderOptions.GIDiffuseSamples')
    specularSamples = cmds.getAttr('defaultArnoldRenderOptions.GISpecularSamples')
    transmissionSamples = cmds.getAttr('defaultArnoldRenderOptions.GITransmissionSamples')
    
    diffuseDepth = cmds.getAttr('defaultArnoldRenderOptions.GIDiffuseDepth')
    specularDepth = cmds.getAttr('defaultArnoldRenderOptions.GISpecularDepth')
    transmissionDepth = cmds.getAttr('defaultArnoldRenderOptions.GITransmissionDepth')
    
    if AASamples <= 0:
        AASamples = 1
    AASamplesComputed = AASamples * AASamples

    GISamplesComputed, GISamplesComputedDepth = calculateRayCounts(AASamplesComputed, GISamples, diffuseDepth)
    specularSamplesComputed, specularSamplesComputedDepth = calculateRayCounts(AASamplesComputed, specularSamples, specularDepth)
    transmissionSamplesComputed, transmissionSamplesComputedDepth = calculateRayCounts(AASamplesComputed, transmissionSamples, transmissionDepth)
    
    totalSamples = AASamplesComputed + GISamplesComputed + specularSamplesComputed + transmissionSamplesComputed
    totalSamplesDepth = AASamplesComputed + GISamplesComputedDepth + specularSamplesComputedDepth + transmissionSamplesComputedDepth

    adaptive = cmds.getAttr('defaultArnoldRenderOptions.enable_adaptive_sampling') and (cmds.getAttr('defaultArnoldRenderOptions.AA_samples_max') > cmds.getAttr('defaultArnoldRenderOptions.AASamples'))

    if adaptive:

        AASamplesComputedMax = cmds.getAttr('defaultArnoldRenderOptions.AA_samples_max')
        if AASamplesComputedMax <= 0:
            AASamplesComputedMax = 1
        AASamplesComputedMax = AASamplesComputedMax * AASamplesComputedMax

        GISamplesComputedMax, GISamplesComputedDepthMax = calculateRayCounts(AASamplesComputedMax, GISamples, diffuseDepth)
        specularSamplesComputedMax, specularSamplesComputedDepthMax = calculateRayCounts(AASamplesComputedMax, specularSamples, specularDepth)
        transmissionSamplesComputedMax, transmissionSamplesComputedDepthMax = calculateRayCounts(AASamplesComputedMax, transmissionSamples, transmissionDepth)
        
        totalSamplesMax = AASamplesComputedMax + GISamplesComputedMax + specularSamplesComputedMax + transmissionSamplesComputedMax
        totalSamplesDepthMax = AASamplesComputedMax + GISamplesComputedDepthMax + specularSamplesComputedDepthMax + transmissionSamplesComputedDepthMax

        cmds.text("textAASamples",
                edit=True, 
                label='Camera (AA) Samples : %i to %i' % (AASamplesComputed, AASamplesComputedMax))

        if not isGpu :
            cmds.text("textGISamples",
                    edit=True, 
                    label='Diffuse Samples : %i to %i (max : %i to %i)' % (GISamplesComputed, GISamplesComputedMax, GISamplesComputedDepth, GISamplesComputedDepthMax),
                    en = True
                    )
            cmds.text("textSpecularSamples",
                    edit=True, 
                    label='Specular Samples : %i to %i (max : %i to %i)' % (specularSamplesComputed, specularSamplesComputedMax, specularSamplesComputedDepth, specularSamplesComputedDepthMax),
                    en = True
                    )
            cmds.text("textTransmissionSamples",
                    edit=True, 
                    label='Transmission Samples : %i to %i (max : %i to %i)' % (transmissionSamplesComputed, transmissionSamplesComputedMax, transmissionSamplesComputedDepth, transmissionSamplesComputedDepthMax),
                    en = True
                    )
            cmds.text("textTotalSamples",
                    edit=True, 
                    label='Total (no lights) : %i to %i (max : %i to %i)' % (totalSamples, totalSamplesMax, totalSamplesDepth, totalSamplesDepthMax),
                    en = True
                    )
        else : 
            cmds.text("textGISamples",
                    edit=True, 
                    label='Diffuse Samples : %i (max : %i)' % (GISamplesComputed, GISamplesComputedDepth),
                    en = False
                    )
            cmds.text("textSpecularSamples",
                    edit=True, 
                    label='Specular Samples : %i (max : %i)' % (specularSamplesComputed, specularSamplesComputedDepth),
                    en = False
                    )
            cmds.text("textTransmissionSamples",
                    edit=True, 
                    label='Transmission Samples : %i (max : %i)' % (transmissionSamplesComputed, transmissionSamplesComputedDepth),
                    en = False
                    )
            cmds.text("textTotalSamples",
                    edit=True, 
                    label='Total (no lights) : %i (max : %i)' % (totalSamples, totalSamplesDepth),
                    en = False
                    )
    else:
        cmds.text("textAASamples",
                edit=True, 
                label='Camera (AA) Samples : %i' % AASamplesComputed)

        if not isGpu :
            cmds.text("textGISamples",
                    edit=True, 
                    label='Diffuse Samples : %i (max : %i)' % (GISamplesComputed, GISamplesComputedDepth),
                    en = True
                    )
            cmds.text("textSpecularSamples",
                    edit=True, 
                    label='Specular Samples : %i (max : %i)' % (specularSamplesComputed, specularSamplesComputedDepth),
                    en = True
                    )
            cmds.text("textTransmissionSamples",
                    edit=True, 
                    label='Transmission Samples : %i (max : %i)' % (transmissionSamplesComputed, transmissionSamplesComputedDepth),
                    en = True
                    )
            cmds.text("textTotalSamples",
                    edit=True, 
                    label='Total (no lights) : %i (max : %i)' % (totalSamples, totalSamplesDepth),
                    en = True
                    )
        else:
            cmds.text("textGISamples",
                    edit=True, 
                    label='Diffuse Samples : %i (max : %i)' % (GISamplesComputed, GISamplesComputedDepth),
                    en = False
                    )
            cmds.text("textSpecularSamples",
                    edit=True, 
                    label='Specular Samples : %i (max : %i)' % (specularSamplesComputed, specularSamplesComputedDepth),
                    en = False
                    )
            cmds.text("textTransmissionSamples",
                    edit=True, 
                    label='Transmission Samples : %i (max : %i)' % (transmissionSamplesComputed, transmissionSamplesComputedDepth),
                    en = False
                    )
            cmds.text("textTotalSamples",
                    edit=True, 
                    label='Total (no lights) : %i (max : %i)' % (totalSamples, totalSamplesDepth),
                    en = False
                    )

def updateMotionBlurSettings(*args):
    flag = cmds.getAttr('defaultArnoldRenderOptions.motion_blur_enable') == True
    cmds.attrControlGrp('ignore_motion_blur', edit=True, enable=flag)
    cmds.attrControlGrp('mb_object_deform_enable', edit=True, enable=flag)
    cmds.attrControlGrp('mb_camera_enable', edit=True, enable=flag)
    cmds.attrControlGrp('mb_shader_enable', edit=True, enable=flag)
    cmds.attrControlGrp('mb_motion_steps', edit=True, enable=flag)
    cmds.attrControlGrp('mb_motion_frames', edit=True, enable=flag)
    cmds.attrControlGrp('textArnoldMBAngle', edit=True, enable=flag)
    cmds.attrControlGrp('mb_position', edit=True, enable=flag)
    if flag:
        arnoldMotionBlurPositionChanged()
    else:
        cmds.attrControlGrp('mb_motion_frames', edit=True, enable=False)
        cmds.attrControlGrp('mb_motion_range_start', edit=True, enable=False)
        cmds.attrControlGrp('mb_motion_range_end', edit=True, enable=False)


def updateAdaptiveSettings(*args):
    flag = cmds.getAttr('defaultArnoldRenderOptions.enable_adaptive_sampling')
    cmds.attrControlGrp('ss_aa_samples_max', edit=True, enable=flag)

    flag = flag and (cmds.getAttr('defaultArnoldRenderOptions.AA_samples_max') > cmds.getAttr('defaultArnoldRenderOptions.AASamples'))
    cmds.attrControlGrp('ss_adaptive_threshold', edit=True, enable=flag)
    

def updateLogSettings(*args):
    name = cmds.getAttr('defaultArnoldRenderOptions.log_filename')
    logToFile = cmds.getAttr('defaultArnoldRenderOptions.log_to_file')

def getBackgroundShader(*args):
    if cmds.objExists('defaultArnoldRenderOptions.background'):
        conns = cmds.listConnections('defaultArnoldRenderOptions.background', s=True, d=False, p=True)
        if conns:
            return conns[0].split('.')[0]
    return ""

def selectBackground(*args):
    node = getBackgroundShader()
    if node:
        cmds.select(node, r=True)

## Operator
def getOperator(*args):
    if cmds.objExists('defaultArnoldRenderOptions.operator'):
        conns = cmds.listConnections('defaultArnoldRenderOptions.operator', s=True, d=False, p=True)
        if conns:
            return conns[0].split('.')[0]
    return ""

def changeOperator(node, field, select):
    connection = cmds.listConnections('defaultArnoldRenderOptions.operator')
    if connection:
        if str(connection[0]) == str(node):
            selectOperator()
            return 0
    cmds.connectAttr("%s.message"%node,'defaultArnoldRenderOptions.operator', force=True)
    if field is not None:
        cmds.textField(field, edit=True, text=node)
        cmds.symbolButton(select, edit=True, enable=True)
    selectOperator()

def removeOperator(field, doDelete, select):
    node = getOperator()
    if node:
        cmds.disconnectAttr("%s.message"%node, 'defaultArnoldRenderOptions.operator')
        cmds.textField(field, edit=True, text="")
        cmds.symbolButton(select, edit=True, enable=False)
        if doDelete:
            cmds.delete(node)

def buildOperatorMenu(popup, field, select):
    cmds.popupMenu(popup, edit=True, deleteAllItems=True)
    operators = cmds.arnoldPlugins(listOperators=True) or []

    for operator in operators:
        opNodes = cmds.ls(type=operator) or []
        for opNode in opNodes:
            cmds.menuItem(parent=popup, label=opNode, command=Callback(changeOperator, opNode, field, select))

    cmds.menuItem(parent=popup, divider=True)
    for operator in operators:
        cmdsLbl = 'Create {}'.format(operator)
        cmds.menuItem(parent=popup, label=cmdsLbl, command=Callback(createOperator, operator, field, select))

    currentOperator = getOperator()
    if currentOperator:
        cmds.menuItem(parent=popup, divider=True)
        cmds.menuItem(parent=popup, label="Disconnect", command=Callback(removeOperator, field, False, select))
        cmds.menuItem(parent=popup, label="Delete", command=Callback(removeOperator, field, True, select))


def selectOperator(*args):
    node = getOperator()
    if node:
        cmds.select(node, r=True)

def createOperator(type, field, select):
    bg = getOperator()
    opNode = cmds.createNode(type)
    changeOperator(opNode, field, select)

def removeOperator(field, doDelete, select):
    node = getOperator()
    if node:
        cmds.disconnectAttr("%s.message"%node, 'defaultArnoldRenderOptions.operator')
        cmds.textField(field, edit=True, text="")
        cmds.symbolButton(select, edit=True, enable=False)
        if doDelete:
            cmds.delete(node)


def changeBackground(node, field, select):
    connection = cmds.listConnections('defaultArnoldRenderOptions.background')
    if connection:
        if cmds.nodeType(connection[0]) == 'transform':
            connection = cmds.listRelatives(connection[0], s=True)
        if connection and len(connection) and (str(connection[0]) == str(node)):
            selectBackground()
            return 0
    cmds.connectAttr("%s.message"%node,'defaultArnoldRenderOptions.background', force=True)
    if field is not None:
        cmds.textField(field, edit=True, text=node)
        cmds.symbolButton(select, edit=True, enable=True)
    selectBackground()

def createBackground(type, field, select):
    bg = getBackgroundShader()
    #if bg:
        #cmds.delete(bg)
    node = cmds.shadingNode(type, asShader=True, name=type)
    changeBackground(node, field, select)

def removeBackground(field, doDelete, select):
    node = getBackgroundShader()
    if node:
        cmds.disconnectAttr("%s.message"%node, 'defaultArnoldRenderOptions.background')
        cmds.textField(field, edit=True, text="")
        cmds.symbolButton(select, edit=True, enable=False)
        if doDelete:
            cmds.delete(node)

def buildBackgroundMenu(popup, field, select):

    switches = cmds.ls(type='aiRaySwitch')
    skies = cmds.ls(type='aiSky')
    pSkies = cmds.ls(type='aiPhysicalSky')

    cmds.popupMenu(popup, edit=True, deleteAllItems=True)
    for item in skies:
        cmds.menuItem(parent=popup, label=item, command=Callback(changeBackground, item, field, select))

    cmds.menuItem(parent=popup, divider=True)
    
    for item in pSkies:
        cmds.menuItem(parent=popup, label=item, command=Callback(changeBackground, item, field, select))

    cmds.menuItem(parent=popup, divider=True)

    for item in switches:
        cmds.menuItem(parent=popup, label=item, command=Callback(changeBackground, item, field, select))

    cmds.menuItem(parent=popup, divider=True)
    

    cmds.menuItem(parent=popup, label="Create Sky Shader", command=Callback(createBackground, "aiSky", field, select))
    cmds.menuItem(parent=popup, label="Create Physical Sky Shader", command=Callback(createBackground, "aiPhysicalSky", field, select))
    cmds.menuItem(parent=popup, label="Create RaySwitch Shader", command=Callback(createBackground, "aiRaySwitch", field, select))

    background = getBackgroundShader()

    if background:
        cmds.menuItem(parent=popup, divider=True)
        cmds.menuItem(parent=popup, label="Disconnect", command=Callback(removeBackground, field, False, select))
        cmds.menuItem(parent=popup, label="Delete", command=Callback(removeBackground, field, True, select))

    
def getAtmosphereShader(*args):
    conns = cmds.listConnections('defaultArnoldRenderOptions.atmosphere', s=True, d=False, p=True)
    if conns:
        return conns[0].split('.')[0]
    return ""

def selectAtmosphere(*args):
    node = getAtmosphereShader()
    if node:
        cmds.select(node, r=True)
        
def changeAtmosphere(node, field, select):
    connection = cmds.listConnections('defaultArnoldRenderOptions.atmosphere')
    if connection:
        if cmds.nodeType(connection[0]) == 'transform':
            connection = cmds.listRelatives(connection[0], s=True)
        if connection and len(connection) and (str(connection[0]) == str(node)):
            selectAtmosphere()
            return 0
    cmds.connectAttr("%s.message"%node,'defaultArnoldRenderOptions.atmosphere', force=True)
    if field is not None:
        cmds.textField(field, edit=True, text=node)
        cmds.symbolButton(select, edit=True, enable=True)
    selectAtmosphere()

def createAtmosphere(type, field, select):
    bg = getAtmosphereShader()
    node = cmds.shadingNode(type, asShader=True, name=type)
    changeAtmosphere(node, field, select)

def removeAtmosphere(field, doDelete, select):
    node = getAtmosphereShader()
    if node:
        cmds.disconnectAttr("%s.message"%node, 'defaultArnoldRenderOptions.atmosphere')
        cmds.textField(field, edit=True, text="")
        cmds.symbolButton(select, edit=True, enable=False)
        if doDelete:
            cmds.delete(node)
    
def buildAtmosphereMenu(popup, field, select):

    cmds.popupMenu(popup, edit=True, deleteAllItems=True)

    for typ in cmds.listNodeTypes(['rendernode/arnold/light/volume']) or []:
        shaders = cmds.ls(type=typ)
        for item in shaders:
            cmds.menuItem(parent=popup, label=item, command=Callback(changeAtmosphere, item, field, select))
    
    cmds.menuItem(parent=popup, divider=True)
    
    for typ in cmds.listNodeTypes(['rendernode/arnold/light/volume']) or []:
        menuLabel = "Create "+typ
        cmds.menuItem(parent=popup, label=menuLabel, command=Callback(createAtmosphere, typ, field, select))

    atmosphere = getAtmosphereShader()
    if atmosphere:        
        cmds.menuItem(parent=popup, divider=True)

        cmds.menuItem(parent=popup, label="Disconnect", command=Callback(removeAtmosphere, field, False, select))
        cmds.menuItem(parent=popup, label="Delete", command=Callback(removeAtmosphere, field, True, select))
        
def getSubdivDicingCameraShader(*args):
    conns = cmds.listConnections('defaultArnoldRenderOptions.subdivDicingCamera', s=True, d=False, p=True)
    if conns:
        return conns[0].split('.')[0]
    return ""

def selectSubdivDicingCamera(*args):
    node = getSubdivDicingCameraShader()
    if node:
        cmds.select(node, r=True)

def changeSubdivDicingCamera(node, field, select):
    connection = cmds.listConnections('defaultArnoldRenderOptions.subdivDicingCamera')
    if connection:
        if cmds.nodeType(connection[0]) == 'transform':
            connection = cmds.listRelatives(connection[0], s=True)
        if connection and len(connection) and (str(connection[0]) == str(node)):
            selectSubdivDicingCamera()
            return 0
    cmds.connectAttr("%s.message"%node,'defaultArnoldRenderOptions.subdivDicingCamera', force=True)
    if field is not None:
        cmds.textField(field, edit=True, text=node)
        cmds.symbolButton(select, edit=True, enable=True)
    selectSubdivDicingCamera()

def createSubdivDicingCamera(type, field, select):
    bg = getSubdivDicingCameraShader()
    node = cmds.camera()
    changeSubdivDicingCamera(node[1], field, select)

def removeSubdivDicingCamera(field, doDelete, select):
    node = getSubdivDicingCameraShader()
    if node:
        cmds.disconnectAttr("%s.message"%node, 'defaultArnoldRenderOptions.subdivDicingCamera')
        cmds.textField(field, edit=True, text="")
        cmds.symbolButton(select, edit=True, enable=False)
        if doDelete:
            parent = cmds.listRelatives( 'cameraShape1', fullPath=True, parent=True )
            if parent and len(parent):
                cmds.delete(parent)
    
def buildSubdivDicingCameraMenu(popup, field, select):

    cmds.popupMenu(popup, edit=True, deleteAllItems=True)

    cameras = cmds.ls(type="camera")
    for item in cameras:
        cmds.menuItem(parent=popup, label=item, command=Callback(changeSubdivDicingCamera, item, field, select))
    
    cmds.menuItem(parent=popup, divider=True)
    
    menuLabel = "Create camera"
    cmds.menuItem(parent=popup, label=menuLabel, command=Callback(createSubdivDicingCamera, "camera", field, select))
        
    cmds.menuItem(parent=popup, divider=True)

    cmds.menuItem(parent=popup, label="Disconnect", command=Callback(removeSubdivDicingCamera, field, False, select))
    cmds.menuItem(parent=popup, label="Delete", command=Callback(removeSubdivDicingCamera, field, True, select))

def changeRenderType():
    try:
        enabled = cmds.getAttr('defaultArnoldRenderOptions.renderType') == 2
        cmds.attrControlGrp('os_kickRenderFlags',
                            edit=True,
                            enable=enabled)
    except:
        pass
def changeGpu():
    try:
        devicesEnabled = cmds.getAttr('defaultArnoldRenderOptions.manual_gpu_devices')
        cmds.textScrollList('os_render_devices', edit=True, enable=devicesEnabled)
    except:
        pass


def changeFrustumCulling(*args):
    frustumCulling = cmds.getAttr('defaultArnoldRenderOptions.subdiv_frustum_culling')
    cmds.attrControlGrp('sub_frustum_padding', edit=True, enable=frustumCulling)

def setupOriginText():
    sel = cmds.listConnections('defaultArnoldRenderOptions.origin', d=0, s=1)
    if (sel != None) and (len(sel) > 0):
        tr = sel[0]
        cmds.textField('defaultArnoldRenderOptionsOriginTextField', e=1, text=tr)

def selectOrigin(*args, **kwargs):
    sel = cmds.ls(sl=1, transforms=1, long=1)

    if (sel != None) and (len(sel) > 0):
        tr = sel[0]
        if cmds.objExists('%s.message' % tr):
            cmds.connectAttr('%s.message' % tr, 'defaultArnoldRenderOptions.origin', force=1)
    setupOriginText()

def renderDevicesListEdit(*args):
    gpuDeviceIdsArray = ai.AiDeviceGetIds(ai.AI_DEVICE_TYPE_GPU)
    gpuDeviceIds = []
    gpuDeviceCount = ai.AiArrayGetNumElements(gpuDeviceIdsArray)
    for i in range(gpuDeviceCount):
        gpuDeviceIds.append(ai.AiArrayGetUInt(gpuDeviceIdsArray, i))

    selList = cmds.textScrollList('os_render_devices', query=True, sii=True)
    idsList = []

    for i in range(len(selList)):
        if i <= len(gpuDeviceIds):
            idsList.append(selList[i])

    
    prevSize = cmds.getAttr('defaultArnoldRenderOptions.render_devices', s=True)
    selCount = len(idsList)
    for i in range(selCount):
        cmds.setAttr('defaultArnoldRenderOptions.render_devices[{}]'.format(i), idsList[i])

    if selCount < prevSize:
        for i in range(selCount, prevSize):
            cmds.removeMultiInstance('defaultArnoldRenderOptions.render_devices[{}]'.format(i))


def createGpuSettings():
    cmds.setUITemplate('attributeEditorTemplate', pushTemplate=True)
    cmds.columnLayout(adjustableColumn=True)

    isGPU = False
    universeCreated = False
    if not ai.AiUniverseIsActive():
        ai.AiBegin()
        universeCreated = True

    if ai.AiNodeEntryLookUpParameter(ai.AiNodeGetNodeEntry(ai.AiUniverseGetOptions()), "render_device"):
        isGPU = True
        
    if universeCreated:
        ai.AiEnd()

    if isGPU:
        cmds.attrControlGrp('renderDevice', 
                    label="Render Device ", 
                    attribute='defaultArnoldRenderOptions.renderDevice',
                    changeCommand = updateComputeSamples
                    )

    cmds.frameLayout(label='Automatic Device Selection', collapse=False)
    cmds.attrControlGrp('gpu_default_names', 
                        label="GPU Names", 
                        attribute='defaultArnoldRenderOptions.gpu_default_names')

    cmds.attrControlGrp('gpu_default_min_memory_MB', 
                        label="Min. Memory (MB)", 
                        attribute='defaultArnoldRenderOptions.gpu_default_min_memory_MB')

    cmds.setParent('..')
    cmds.frameLayout(label='Manual Device Selection (Local Render)', collapse=False)
    
    cmds.attrControlGrp('manual_gpu_devices', 
                        label="Enable Manual Device Selection", 
                        changeCommand=changeGpu,
                        attribute='defaultArnoldRenderOptions.manual_gpu_devices')

    deviceListEnabled = cmds.getAttr('defaultArnoldRenderOptions.manual_gpu_devices')
    cmds.textScrollList('os_render_devices', height=50,allowMultiSelection=True, enable=deviceListEnabled, selectCommand=lambda *args: renderDevicesListEdit(*args))
    # fill attribute
    
    gpuDeviceIdsArray = ai.AiDeviceGetIds(ai.AI_DEVICE_TYPE_GPU)
    gpuDeviceCount = ai.AiArrayGetNumElements(gpuDeviceIdsArray)
    gpuDeviceIds = []

    for i in range(gpuDeviceCount):
        gpuDeviceIds.append(ai.AiArrayGetUInt(gpuDeviceIdsArray, i))
        
    cmds.textScrollList('os_render_devices', edit=True, removeAll=True)

    for gpuDevice in gpuDeviceIds:
        deviceName = ai.AiDeviceGetName(ai.AI_DEVICE_TYPE_GPU, gpuDevice)
        cmds.textScrollList('os_render_devices', edit=True, append=str(deviceName))

    attrIds = cmds.getAttr('defaultArnoldRenderOptions.render_devices', mi=True) or []

    if len(attrIds) == 0:
        attrIds.append(0)

    for i in attrIds:
        attrVal = cmds.getAttr('defaultArnoldRenderOptions.render_devices[{}]'.format(i))
        if attrVal <= 0:
            continue

        cmds.textScrollList('os_render_devices', edit=True, selectIndexedItem=attrVal)

    changeGpu()
    cmds.setParent('..')
    if isGPU:
        cmds.attrControlGrp('gpu_max_texturemax_texture_resolution', 
                        label="Max Texture Resolution", 
                        attribute='defaultArnoldRenderOptions.gpu_max_texture_resolution', 
                        en = False)
    cmds.setParent('..')
        
def createArnoldRenderSettings():

    cmds.setUITemplate('attributeEditorTemplate', pushTemplate=True)
    cmds.columnLayout('arnoldRenderLayout', adjustableColumn=True)

    cmds.attrControlGrp('os_renderType',
                        label="Render Type",
                        changeCommand=changeRenderType,
                        attribute='defaultArnoldRenderOptions.renderType')

    

    cmds.separator()

    cmds.attrControlGrp('os_bucket_scanning',
                        label="Bucket Scanning",
                        attribute='defaultArnoldRenderOptions.bucketScanning')

    cmds.attrControlGrp('os_bucket_size',
                        label="Bucket Size",
                        attribute='defaultArnoldRenderOptions.bucketSize')

    
    cmds.attrControlGrp('os_output_overscan',
                        label='Overscan',
                        attribute='defaultArnoldRenderOptions.outputOverscan')
    
    cmds.separator()

    cmds.checkBoxGrp('os_threads_autodetect',
                    cc=updateRenderSettings,
                    label='',
                    label1='Autodetect Threads')

    cmds.connectControl('os_threads_autodetect', 'defaultArnoldRenderOptions.threads_autodetect', index=2)

    cmds.attrControlGrp('os_threads',
                        label="Threads",
                        attribute='defaultArnoldRenderOptions.threads')

    cmds.separator()

    cmds.attrControlGrp('os_binary_ass',
                        label='Binary-encode ASS Files',
                        attribute='defaultArnoldRenderOptions.binaryAss')
    
                    
    cmds.attrControlGrp('os_outputAssBoundingBox',
                        label="Export Bounding Box (.asstoc)",
                        attribute='defaultArnoldRenderOptions.outputAssBoundingBox')                   
                   
    cmds.attrControlGrp('os_expandProcedurals',
                        label='Expand Procedurals',
                        attribute='defaultArnoldRenderOptions.expandProcedurals')

    cmds.attrControlGrp('os_exportAllShadingGroups',
                        label='Export All Shading Groups',
                        attribute='defaultArnoldRenderOptions.exportAllShadingGroups')

    cmds.attrControlGrp('os_exportFullPaths',
                        label='Export Full Paths',
                        attribute='defaultArnoldRenderOptions.exportFullPaths')
    cmds.attrControlGrp('os_exportShadingEngine',
                        label='Export Shading Engine (Legacy)',
                        attribute='defaultArnoldRenderOptions.exportShadingEngine')

    cmds.separator()

    enabled = cmds.getAttr('defaultArnoldRenderOptions.renderType') == 2

    cmds.attrControlGrp('os_kickRenderFlags',
                        label='Kick Render Flags',
                        enable=enabled,
                        attribute='defaultArnoldRenderOptions.kickRenderFlags')

    cmds.separator()

    cmds.attrControlGrp('os_render_unit',
                        label='Render Unit',
                        cc=updateRenderSettings,
                        attribute='defaultArnoldRenderOptions.renderUnit')

    enabled = cmds.getAttr('defaultArnoldRenderOptions.renderUnit') == 1

    cmds.attrControlGrp('os_scene_scale',
                        label='Scene Scale',
                        enable=enabled,
                        attribute='defaultArnoldRenderOptions.sceneScale')

    cmds.separator()

    cmds.attrControlGrp('os_offset_origin',
                        label='Offset Origin',
                        attribute='defaultArnoldRenderOptions.offsetOrigin')

    cmds.rowLayout(numberOfColumns=2, adjustableColumn=1, columnWidth2=(200, 80))

    cmds.textField('defaultArnoldRenderOptionsOriginTextField', editable=False)
    cmds.button(label='Select Origin', command=selectOrigin)

    setupOriginText()

    cmds.setParent('..')

    cmds.frameLayout(label='Callbacks', collapse=True)
    
    cmds.attrControlGrp(
            'os_postTranslationCallback',
            label='Post Translation',
            attribute='defaultArnoldRenderOptions.PostTranslation')
    cmds.separator()

    cmds.attrControlGrp(
            'os_iprRefinementStartedCallback',
            label='IPR Refinement Started',
            attribute='defaultArnoldRenderOptions.IPRRefinementStarted')

    cmds.attrControlGrp(
            'os_iprRefinementFinishedCallback',
            label='IPR Refinement Finished',
            attribute='defaultArnoldRenderOptions.IPRRefinementFinished')
    
    cmds.attrControlGrp(
            'os_iprStepStartedCallback',
            label='IPR Step Started',
            attribute='defaultArnoldRenderOptions.IPRStepStarted')

    cmds.attrControlGrp(
            'os_iprStepFinishedCallback',
            label='IPR Step Finished',
            attribute='defaultArnoldRenderOptions.IPRStepFinished')

    cmds.setParent('..')

    cmds.setParent('..')

    cmds.setUITemplate(popTemplate=True)

def updateArnoldFilterOptions(*args):
    pass

def createArnoldSamplingSettings():

    cmds.setUITemplate('attributeEditorTemplate', pushTemplate=True)
    cmds.columnLayout(adjustableColumn=True)

    cmds.text( "textAASamples", 
               font = "smallBoldLabelFont",
               align='left',
               label = 'Camera (AA) Samples'
               )
    
    cmds.text( "textGISamples", 
               font = "smallBoldLabelFont",
               align='left',
               label = 'Diffuse Samples'
               )
    
    cmds.text( "textSpecularSamples", 
               font = "smallBoldLabelFont",
               align='left',
               label = 'Specular Samples'
               )

    cmds.text( "textTransmissionSamples", 
               font = "smallBoldLabelFont",
               align='left',
               label = 'Transmission Samples'
               )

    cmds.text( "textTotalSamples", 
               font = "smallBoldLabelFont",
               align='left',
               label = 'Total (no lights)'
               )
                        
    cmds.separator()

    cmds.intSliderGrp('ss_AA_samples',
                        label="Camera (AA)",
                        minValue = 1,
                        maxValue = 10,
                        fieldMinValue=-10,
                        fieldMaxValue=100,
                        cc=lambda *args: cmds.evalDeferred(updateComputeSamples)
                        )

    cmds.connectControl('ss_AA_samples', 'defaultArnoldRenderOptions.AASamples', index=1)
    cmds.connectControl('ss_AA_samples', 'defaultArnoldRenderOptions.AASamples', index=2)
    cmds.connectControl('ss_AA_samples', 'defaultArnoldRenderOptions.AASamples', index=3)

    cmds.intSliderGrp('ss_hemi_samples',
                        label="Diffuse",
                        maxValue = 10,
                        fieldMaxValue=100,
                        cc=lambda *args: cmds.evalDeferred(updateComputeSamples))
    
    cmds.connectControl('ss_hemi_samples', 'defaultArnoldRenderOptions.GIDiffuseSamples', index=1)
    cmds.connectControl('ss_hemi_samples', 'defaultArnoldRenderOptions.GIDiffuseSamples', index=2)
    cmds.connectControl('ss_hemi_samples', 'defaultArnoldRenderOptions.GIDiffuseSamples', index=3)
    
    cmds.intSliderGrp('ss_specular_samples',
                        label="Specular",
                        maxValue = 10,
                        fieldMaxValue=100,
                        cc=lambda *args: cmds.evalDeferred(updateComputeSamples))
    
    cmds.connectControl('ss_specular_samples', 'defaultArnoldRenderOptions.GISpecularSamples', index=1)
    cmds.connectControl('ss_specular_samples', 'defaultArnoldRenderOptions.GISpecularSamples', index=2)
    cmds.connectControl('ss_specular_samples', 'defaultArnoldRenderOptions.GISpecularSamples', index=3)    
    
    cmds.intSliderGrp('ss_transmission_samples',
                        label='Transmission',
                        maxValue = 10,
                        fieldMaxValue=100,
                        cc=lambda *args: cmds.evalDeferred(updateComputeSamples))
    
    cmds.connectControl('ss_transmission_samples', 'defaultArnoldRenderOptions.GITransmissionSamples', index=1)
    cmds.connectControl('ss_transmission_samples', 'defaultArnoldRenderOptions.GITransmissionSamples', index=2)
    cmds.connectControl('ss_transmission_samples', 'defaultArnoldRenderOptions.GITransmissionSamples', index=3)    

    cmds.attrControlGrp('ss_sss_samples',
                        label='SSS',
                        attribute='defaultArnoldRenderOptions.GI_sss_samples')
    
    cmds.attrControlGrp('ss_volume_samples',
                        label='Volume Indirect',
                        attribute='defaultArnoldRenderOptions.GI_volume_samples')
    
    cmds.separator()
    cmds.attrControlGrp('ss_progressive_render',
                        label="Progressive Render",
                        attribute='defaultArnoldRenderOptions.enable_progressive_render')

    cmds.frameLayout(label='Adaptive Sampling', collapse=True)
    cmds.columnLayout(adjustableColumn=True)
    
    cmds.attrControlGrp('ss_enable_adaptive_sampling',
                        label="Enable",
                        cc=lambda *args: cmds.evalDeferred(updateComputeSamples),
                        attribute='defaultArnoldRenderOptions.enable_adaptive_sampling')

    cmds.attrControlGrp('ss_aa_samples_max',
                        label="Max. Camera (AA)",
                        cc=lambda *args: cmds.evalDeferred(updateComputeSamples),
                        attribute='defaultArnoldRenderOptions.AA_samples_max')

    cmds.attrControlGrp('ss_adaptive_threshold',
                        label="Adaptive Threshold",
                        attribute='defaultArnoldRenderOptions.AA_adaptive_threshold')
    cmds.setParent('..')
    cmds.setParent('..')
    
    cmds.frameLayout(label='Clamping', collapse=True)
    cmds.columnLayout(adjustableColumn=True)
    
    cmds.checkBoxGrp('ss_clamp_sample_values',
                    cc=updateSamplingSettings,
                    label='Clamp AA Samples')

    cmds.connectControl('ss_clamp_sample_values', 'defaultArnoldRenderOptions.use_sample_clamp', index=1)
    cmds.connectControl('ss_clamp_sample_values', 'defaultArnoldRenderOptions.use_sample_clamp', index=2)

    cmds.checkBoxGrp('ss_clamp_sample_values_AOVs',
                    cc=updateSamplingSettings,
                    label='Affect AOVs')

    cmds.connectControl('ss_clamp_sample_values_AOVs', 'defaultArnoldRenderOptions.use_sample_clamp_AOVs', index=1)
    cmds.connectControl('ss_clamp_sample_values_AOVs', 'defaultArnoldRenderOptions.use_sample_clamp_AOVs', index=2)

    cmds.attrControlGrp('ss_max_value',
                        label="AA Clamp Value",
                        attribute='defaultArnoldRenderOptions.AASampleClamp')

    cmds.attrControlGrp('ss_indirect_max_value',
                        label="Indirect Clamp Value",
                        attribute='defaultArnoldRenderOptions.indirectSampleClamp')
                        
    cmds.setParent('..')
    cmds.setParent('..')
    
    cmds.frameLayout(label="Filter", collapse=True)
    cmds.columnLayout(adjustableColumn=True)
    
    createTranslatorMenu('defaultArnoldFilter',
                            label='Type',
                            nodeType='aiAOVFilter',
                            default='gaussian')

    cmds.setParent('..')
    cmds.setParent('..')
    
    cmds.frameLayout(label='Advanced', collapse=True)
    cmds.columnLayout(adjustableColumn=True)

    cmds.attrControlGrp('ss_lock_sampling_noise',
                        label="Lock Sampling Pattern",
                        attribute='defaultArnoldRenderOptions.lock_sampling_noise')

    cmds.attrControlGrp('ss_use_autobump',
                        label='Use Autobump in SSS',
                        attribute='defaultArnoldRenderOptions.sssUseAutobump',
                        annotation='WARNING : Enabling this checkbox triples shader evaluations in SSS.')
    
    cmds.separator()
    cmds.attrControlGrp('ss_indirect_specular_blur',
                        label="Indirect Specular Blur",
                        attribute='defaultArnoldRenderOptions.indirectSpecularBlur')
    cmds.setParent('..')
    
    cmds.setParent('..')
    cmds.setParent('..') # column layout
    
    cmds.setUITemplate(popTemplate=True)
    updateArnoldFilterOptions()


def createArnoldGammaSettings():

    cmds.setUITemplate('attributeEditorTemplate', pushTemplate=True)
    cmds.columnLayout(adjustableColumn=True)
    
    cmds.attrControlGrp('ss_driver_gamma',
                        label="Display Driver gamma",
                        attribute='defaultArnoldRenderOptions.display_gamma')

    cmds.setParent('..')

    cmds.setUITemplate(popTemplate=True)


def createArnoldRayDepthSettings():

    cmds.setUITemplate('attributeEditorTemplate', pushTemplate=True)
    cmds.columnLayout(adjustableColumn=True)

    cmds.attrControlGrp('rs_total_depth',
                        label="Total",
                        attribute='defaultArnoldRenderOptions.GITotalDepth')

    cmds.separator(style="none")

    
    cmds.intSliderGrp('rs_diffuse_depth',
                        label="Diffuse",
                        maxValue = 16,
                        fieldMaxValue=100,
                        cc=lambda *args: cmds.evalDeferred(updateComputeSamples))
    
    cmds.connectControl('rs_diffuse_depth', 'defaultArnoldRenderOptions.GIDiffuseDepth', index=1)
    cmds.connectControl('rs_diffuse_depth', 'defaultArnoldRenderOptions.GIDiffuseDepth', index=2)
    cmds.connectControl('rs_diffuse_depth', 'defaultArnoldRenderOptions.GIDiffuseDepth', index=3)
    
    '''
    cmds.attrControlGrp('rs_diffuse_depth',
                        label="Diffuse depth",
                        attribute='defaultArnoldRenderOptions.GIDiffuseDepth')
    '''
    
    cmds.intSliderGrp('rs_specular_depth',
                        label="Specular",
                        maxValue = 16,
                        fieldMaxValue=100,
                        cc=lambda *args: cmds.evalDeferred(updateComputeSamples))
    
    cmds.connectControl('rs_specular_depth', 'defaultArnoldRenderOptions.GISpecularDepth', index=1)
    cmds.connectControl('rs_specular_depth', 'defaultArnoldRenderOptions.GISpecularDepth', index=2)
    cmds.connectControl('rs_specular_depth', 'defaultArnoldRenderOptions.GISpecularDepth', index=3)
    
    '''
    cmds.attrControlGrp('rs_specular_depth',
                        label="Specular depth",
                        attribute='defaultArnoldRenderOptions.GISpecularDepth')
    '''

    cmds.intSliderGrp('rs_transmission_depth',
                        label="Transmission ",
                        maxValue = 16,
                        fieldMaxValue=100,
                        cc=lambda *args: cmds.evalDeferred(updateComputeSamples))
    
    cmds.connectControl('rs_transmission_depth', 'defaultArnoldRenderOptions.GITransmissionDepth', index=1)
    cmds.connectControl('rs_transmission_depth', 'defaultArnoldRenderOptions.GITransmissionDepth', index=2)
    cmds.connectControl('rs_transmission_depth', 'defaultArnoldRenderOptions.GITransmissionDepth', index=3)

    '''
    cmds.attrControlGrp('rs_transmission_depth',
                        label="Refraction depth",
                        attribute='defaultArnoldRenderOptions.GITransmissionDepth')
    '''
    
    cmds.attrControlGrp('rs_volume_depth',
                        label="Volume",
                        attribute='defaultArnoldRenderOptions.GIVolumeDepth')
    
    cmds.separator(style="none")

    cmds.attrControlGrp('rs_auto_transparency_depth',
                        label="Transparency Depth",
                        attribute='defaultArnoldRenderOptions.autoTransparencyDepth')


    cmds.setParent('..')

    cmds.setUITemplate(popTemplate=True)

def createArnoldEnvironmentSettings():

    cmds.setUITemplate('attributeEditorTemplate', pushTemplate=True)
    cmds.columnLayout(adjustableColumn=True)

    
    cmds.rowLayout(adjustableColumn=2, numberOfColumns=4)
    cmds.text('es_atmosphere_text', label="Atmosphere")
    cmds.connectControl('es_atmosphere_text', 'defaultArnoldRenderOptions.atmosphere')
    atmosphereTextField = cmds.textField("defaultArnoldRenderOptionsAtmosphereTextField",editable=False)
    atmosphereButton = cmds.symbolButton(image="navButtonUnconnected.png")
    atmosphereSelectButton = cmds.symbolButton("defaultArnoldRenderOptionsAtmosphereSelectButton", image="navButtonConnected.png", command=selectAtmosphere, enable=False)
    atpopup = cmds.popupMenu(parent=atmosphereButton, button=1)
    cmds.popupMenu(atpopup, edit=True, postMenuCommand=Callback(buildAtmosphereMenu, atpopup, atmosphereTextField, atmosphereSelectButton))
    
    cmds.setParent('..')

    conns = cmds.listConnections('defaultArnoldRenderOptions.atmosphere', s=True, d=False)
    if conns:
        cmds.textField(atmosphereTextField, edit=True, text=conns[0])
        cmds.symbolButton(atmosphereSelectButton, edit=True, enable=True)

    cmds.rowLayout(adjustableColumn=2, numberOfColumns=4)
    cmds.text('es_background_text', label="Background (Legacy)")
    cmds.connectControl('es_background_text', 'defaultArnoldRenderOptions.background')
    backgroundTextField = cmds.textField("defaultArnoldRenderOptionsBackgroundTextField",editable=False)
    backgroundButton = cmds.symbolButton(image="navButtonUnconnected.png")
    backgroundSelectButton = cmds.symbolButton("defaultArnoldRenderOptionsBackgroundSelectButton", image="navButtonConnected.png", command=selectBackground, enable=False)
    bgpopup = cmds.popupMenu(parent=backgroundButton, button=1)
    cmds.popupMenu(bgpopup, edit=True, postMenuCommand=Callback(buildBackgroundMenu, bgpopup, backgroundTextField, backgroundSelectButton))

    cmds.setParent('..')

    conns = cmds.listConnections('defaultArnoldRenderOptions.background', s=True, d=False)
    if conns:
        cmds.textField(backgroundTextField, edit=True, text=conns[0])
        cmds.symbolButton(backgroundSelectButton, edit=True, enable=True)

    cmds.separator(style="none")
    
    cmds.setParent('..')

    cmds.setUITemplate(popTemplate=True)

def createArnoldOperatorSettings():

    cmds.setUITemplate('attributeEditorTemplate', pushTemplate=True)
    cmds.columnLayout(adjustableColumn=True)
    
    cmds.rowLayout(adjustableColumn=2, numberOfColumns=4)
    cmds.text('es_operators_text', label="Target Operator")
    cmds.connectControl('es_operators_text', 'defaultArnoldRenderOptions.operator')
    operatorTextField = cmds.textField("defaultArnoldRenderOptionsOperatorTextField",editable=False)
    operatorButton = cmds.symbolButton(image="navButtonUnconnected.png")
    operatorSelectButton = cmds.symbolButton("defaultArnoldRenderOptionsOperatorSelectButton", image="navButtonConnected.png", command=selectOperator, enable=False)
    oppopup = cmds.popupMenu(parent=operatorButton, button=1)
    cmds.popupMenu(oppopup, edit=True, postMenuCommand=Callback(buildOperatorMenu, oppopup, operatorTextField, operatorSelectButton))
    
    cmds.setParent('..')

    conns = cmds.listConnections('defaultArnoldRenderOptions.operator', s=True, d=False)
    if conns:
        cmds.textField(operatorTextField, edit=True, text=conns[0])
        cmds.symbolButton(operatorSelectButton, edit=True, enable=True)

    cmds.separator(style="none")
    
    cmds.setParent('..')

    cmds.setUITemplate(popTemplate=True)

def arnoldMotionBlurPositionChanged(*args):
    sel = cmds.optionMenuGrp('mb_position', q=True, select=True) - 1
    if (sel is 3):
        cmds.attrControlGrp('mb_motion_frames', edit=True, enable=False)
        cmds.attrControlGrp('mb_motion_range_start', edit=True, enable=True)
        cmds.attrControlGrp('mb_motion_range_end', edit=True, enable=True)
        arnoldMotionCustomChanged()
    else:
        cmds.attrControlGrp('mb_motion_frames', edit=True, enable=True)
        cmds.attrControlGrp('mb_motion_range_start', edit=True, enable=False)
        cmds.attrControlGrp('mb_motion_range_end', edit=True, enable=False)
        arnoldMotionFramesChanged()

def arnoldMotionFramesChanged(*args):
    length = cmds.getAttr('defaultArnoldRenderOptions.motion_frames')
    angle = length * 360
    cmds.text("textArnoldMBAngle",
                edit=True, 
                label=u'  Shutter Angle : %i°' % angle)
    
def arnoldMotionCustomChanged(*args):
    start = cmds.getAttr('defaultArnoldRenderOptions.motion_start')
    end = cmds.getAttr('defaultArnoldRenderOptions.motion_end')
    angle = abs(end-start) * 360
    cmds.text("textArnoldMBAngle",
               edit=True, 
               label=u'  Shutter Angle : %i°' % angle)
    
def createArnoldMotionBlurRange(*args):

    cmds.text("textArnoldMBAngle", 
                font = "smallBoldLabelFont",
                align='left',
                enable=False
            )
    
    cmds.text( "textArnoldMBAngle",
                edit=True, 
                label=u'  Shutter Angle : %i°' % 180)

               
    cmds.optionMenuGrp('mb_position', label='Position', changeCommand=arnoldMotionBlurPositionChanged)
    cmds.menuItem( label='Start On Frame', data=0)
    cmds.menuItem( label='Center On Frame', data=1)
    cmds.menuItem( label='End On Frame', data=2)
    cmds.menuItem( label='Custom', data=3)
    
    cmds.connectControl('mb_position', 'defaultArnoldRenderOptions.range_type', index=1)
    cmds.connectControl('mb_position', 'defaultArnoldRenderOptions.range_type', index=2)

    
    
    cmds.attrFieldSliderGrp('mb_motion_frames',
                            label="Length",
                            ann='Motion Range in Frames',
                            attribute='defaultArnoldRenderOptions.motion_frames',
                            cc=arnoldMotionFramesChanged)
                        
    cmds.attrFieldSliderGrp('mb_motion_range_start',
                            label="Start",
                            ann='Motion Range Start in Frames',
                            attribute='defaultArnoldRenderOptions.motion_start',
                            cc=arnoldMotionCustomChanged)
                        
    cmds.attrFieldSliderGrp('mb_motion_range_end',
                            label="End",
                            ann='Motion Range End in Frames',
                            attribute='defaultArnoldRenderOptions.motion_end',
                            cc=arnoldMotionCustomChanged)
    
def createArnoldMotionBlurSettings():

    cmds.setUITemplate('attributeEditorTemplate', pushTemplate=True)
    cmds.columnLayout(adjustableColumn=True)

    
                   
    cmds.checkBoxGrp('mb_enable',
                    cc=updateMotionBlurSettings,
                    label='Enable')

    cmds.connectControl('mb_enable', 'defaultArnoldRenderOptions.motion_blur_enable', index=1)
    cmds.connectControl('mb_enable', 'defaultArnoldRenderOptions.motion_blur_enable', index=2)

    cmds.checkBoxGrp('ignore_motion_blur',
                    label='Instantaneous Shutter')
    cmds.connectControl('ignore_motion_blur', 'defaultArnoldRenderOptions.ignore_motion_blur', index=1)
    cmds.connectControl('ignore_motion_blur', 'defaultArnoldRenderOptions.ignore_motion_blur', index=2)

    cmds.checkBoxGrp('mb_object_deform_enable',
                    label='Deformation')                     
    cmds.connectControl('mb_object_deform_enable', 'defaultArnoldRenderOptions.mb_object_deform_enable', index=1)
    cmds.connectControl('mb_object_deform_enable', 'defaultArnoldRenderOptions.mb_object_deform_enable', index=2)
    
    cmds.checkBoxGrp('mb_camera_enable',
                    label='Camera')
                     
    cmds.connectControl('mb_camera_enable', 'defaultArnoldRenderOptions.mb_camera_enable', index=1)
    cmds.connectControl('mb_camera_enable', 'defaultArnoldRenderOptions.mb_camera_enable', index=2)
    
    cmds.checkBoxGrp('mb_shader_enable',
                    label='Shaders')
                     
    cmds.connectControl('mb_shader_enable', 'defaultArnoldRenderOptions.mb_shader_enable', index=1)
    cmds.connectControl('mb_shader_enable', 'defaultArnoldRenderOptions.mb_shader_enable', index=2)
    
    cmds.attrControlGrp('mb_motion_steps',
                        label="Keys",
                        attribute='defaultArnoldRenderOptions.motion_steps')                   
                        
    cmds.separator()
    
    createArnoldMotionBlurRange()

    cmds.setParent('..')

    cmds.setUITemplate(popTemplate=True)

def createArnoldLightSettings():

    cmds.setUITemplate('attributeEditorTemplate', pushTemplate=True)
    cmds.columnLayout(adjustableColumn=True)

    cmds.attrControlGrp('lightThreshold',
                        label="Low Light Threshold",
                        attribute='defaultArnoldRenderOptions.lowLightThreshold')

    cmds.separator()

    cmds.attrControlGrp('lightLinking',
                        label="Light Linking",
                        attribute='defaultArnoldRenderOptions.lightLinking')

    cmds.attrControlGrp('shadowLinking',
                        label="Shadow Linking",
                        attribute='defaultArnoldRenderOptions.shadowLinking')


    cmds.setParent('..')

    cmds.setUITemplate(popTemplate=True)

def createArnoldSubdivSettings():

    cmds.setUITemplate('attributeEditorTemplate', pushTemplate=True)
    cmds.columnLayout(adjustableColumn=True)

    cmds.attrControlGrp('sub_max_subdivisions',
                        label="Max. Subdivisions",
                        attribute='defaultArnoldRenderOptions.maxSubdivisions')

    cmds.attrControlGrp('sub_frustum_culling',
                        label="Frustum Culling",
                        changeCommand=changeFrustumCulling,
                        attribute='defaultArnoldRenderOptions.subdivFrustumCulling')

    cmds.attrControlGrp('sub_frustum_padding',
                        label="Frustum Padding",
                        attribute='defaultArnoldRenderOptions.subdivFrustumPadding')
    frustumCulling = cmds.getAttr('defaultArnoldRenderOptions.subdiv_frustum_culling')
    cmds.attrControlGrp('sub_frustum_padding', edit=True, enable=frustumCulling)

    cmds.rowLayout(adjustableColumn=2, numberOfColumns=4)
    cmds.text('es_subdiv_dicing_camera_text', label="Dicing Camera")
    cmds.connectControl('es_subdiv_dicing_camera_text', 'defaultArnoldRenderOptions.subdivDicingCamera')
    subdivDicingCameraTextField = cmds.textField("defaultArnoldRenderOptionsSubdivDicingCameraTextField",editable=False)
    subdivDicingCameraButton = cmds.symbolButton(image="navButtonUnconnected.png")
    subdivDicingCameraSelectButton = cmds.symbolButton("defaultArnoldRenderOptionsSubdivDicingCameraSelectButton", image="navButtonConnected.png", command=selectSubdivDicingCamera, enable=False)
    sdcpopup = cmds.popupMenu(parent=subdivDicingCameraButton, button=1)
    cmds.popupMenu(sdcpopup, edit=True, postMenuCommand=Callback(buildSubdivDicingCameraMenu, sdcpopup, subdivDicingCameraTextField, subdivDicingCameraSelectButton))
    
    cmds.setParent('..')

    conns = cmds.listConnections('defaultArnoldRenderOptions.subdivDicingCamera', s=True, d=False)
    if conns:
        cmds.textField(subdivDicingCameraTextField, edit=True, text=conns[0])
        cmds.symbolButton(subdivDicingCameraSelectButton, edit=True, enable=True)

    cmds.setParent('..')

    cmds.setUITemplate(popTemplate=True)


def createArnoldTextureSettings():

    cmds.setUITemplate('attributeEditorTemplate', pushTemplate=True)
    cmds.columnLayout(adjustableColumn=True)

   
    cmds.attrControlGrp('autotx', 
                        cc=updateAutoTxSettings,
                        label="Auto-convert Textures to TX ", 
                        attribute='defaultArnoldRenderOptions.autotx')

    cmds.attrControlGrp('use_existing_tiled_textures', 
                        label="Use Existing TX Textures", 
                        attribute='defaultArnoldRenderOptions.use_existing_tiled_textures')
    
    updateAutoTxSettings()
    cmds.separator()
    
    # don't create texture_automip for 2016 as autoTx is ON by default
    maya_version = utils.getMayaVersion()
    if maya_version < 2016:
        cmds.attrControlGrp('texture_automip',
                            label="Auto-mipmap",
                            attribute='defaultArnoldRenderOptions.textureAutomip')
                            
    cmds.attrControlGrp('texture_accept_unmipped',
                        label="Accept Unmipped",
                        attribute='defaultArnoldRenderOptions.textureAcceptUnmipped')
                        
    cmds.separator()
    
    
    cmds.checkBoxGrp('ts_autotile',
                    cc=updateAutotileSettings,
                    label='',
                    label1='Auto-tile')
                     
    cmds.connectControl('ts_autotile', 'defaultArnoldRenderOptions.autotile', index=2)
    
    cmds.intSliderGrp('ts_texture_autotile',
                        label="Tile Size",
                        minValue = 16,
                        maxValue = 64,
                        fieldMinValue=16,
                        fieldMaxValue=1024
                    )

    cmds.connectControl('ts_texture_autotile', 'defaultArnoldRenderOptions.textureAutotile', index=1)
    cmds.connectControl('ts_texture_autotile', 'defaultArnoldRenderOptions.textureAutotile', index=2)
    cmds.connectControl('ts_texture_autotile', 'defaultArnoldRenderOptions.textureAutotile', index=3)
    
    '''cmds.attrControlGrp('texture_autotile',
                        label="Auto-tile Size",
                        attribute='defaultArnoldRenderOptions.textureAutotile')'''

    cmds.attrControlGrp('texture_accept_untiled',
                        label="Accept Untiled",
                        attribute='defaultArnoldRenderOptions.textureAcceptUntiled')
    

    cmds.attrControlGrp('texture_max_memory_MB',
                        label="Max Cache Size (MB)",
                        attribute='defaultArnoldRenderOptions.textureMaxMemoryMB')

    cmds.attrControlGrp('texture_max_open_files',
                        label="Max Open Files",
                        attribute='defaultArnoldRenderOptions.textureMaxOpenFiles')

    cmds.setParent('..')

    cmds.setUITemplate(popTemplate=True)

def createArnoldOverrideSettings():

    cmds.setUITemplate('attributeEditorTemplate', pushTemplate=True)
    cmds.columnLayout(adjustableColumn=True)

    cmds.attrControlGrp('ignore_textures',
                        attribute='defaultArnoldRenderOptions.ignore_textures')

    cmds.attrControlGrp('ignore_shaders',
                        attribute='defaultArnoldRenderOptions.ignore_shaders')

    cmds.attrControlGrp('ignore_atmosphere',
                        attribute='defaultArnoldRenderOptions.ignore_atmosphere')

    cmds.attrControlGrp('ignore_lights',
                        attribute='defaultArnoldRenderOptions.ignore_lights')

    cmds.attrControlGrp('ignore_shadows',
                        attribute='defaultArnoldRenderOptions.ignore_shadows')
                        
    cmds.attrControlGrp('ignore_subdivision',
                        attribute='defaultArnoldRenderOptions.ignore_subdivision')

    cmds.attrControlGrp('ignore_displacement',
                        attribute='defaultArnoldRenderOptions.ignore_displacement')

    cmds.attrControlGrp('ignore_bump',
                        attribute='defaultArnoldRenderOptions.ignore_bump')

    cmds.attrControlGrp('ignore_smoothing',
                        attribute='defaultArnoldRenderOptions.ignore_smoothing', label='Ignore Normal Smoothing')
                        
    cmds.attrControlGrp('ignore_motion',
                        attribute='defaultArnoldRenderOptions.ignore_motion')

    cmds.attrControlGrp('ignore_dof',
                        attribute='defaultArnoldRenderOptions.ignore_dof', label='Ignore Depth of Field')
                        
    cmds.attrControlGrp('ignore_sss',
                        attribute='defaultArnoldRenderOptions.ignore_sss', label='Ignore Sub-Surface Scattering')

    cmds.attrControlGrp('ignore_operators',
                        attribute='defaultArnoldRenderOptions.ignore_operators')

    cmds.attrControlGrp('force_translate_shading_engines',
                       attribute='defaultArnoldRenderOptions.forceTranslateShadingEngines', label='Force Shader Assignments')

    cmds.attrControlGrp('ignore_list',
                       attribute='defaultArnoldRenderOptions.ignore_list', label='Ignore List')

    cmds.setParent('..')

    cmds.setUITemplate(popTemplate=True)

def createArnoldPathSettings():

    cmds.setUITemplate('attributeEditorTemplate', pushTemplate=True)
    cmds.columnLayout(adjustableColumn=True)
    
    cmds.attrControlGrp('texture_absolute_paths',
                        label='Absolute Texture Paths',
                        attribute='defaultArnoldRenderOptions.absoluteTexturePaths')

    cmds.attrControlGrp('os_absoluteProceduralPaths',
                        label='Absolute Procedural Paths',
                        attribute='defaultArnoldRenderOptions.absoluteProceduralPaths')

    cmds.separator()


    cmds.attrControlGrp('os_procedural_searchpath',
                        label="Procedural Search Path",
                        attribute='defaultArnoldRenderOptions.procedural_searchpath')

    cmds.attrControlGrp('os_plugin_searchpath',
                        label="Plugin Search Path",
                        attribute='defaultArnoldRenderOptions.plugin_searchpath')

    cmds.attrControlGrp('os_texture_searchpath',
                        label="Texture Search Path",
                        attribute='defaultArnoldRenderOptions.texture_searchpath')

    cmds.setParent('..')

    cmds.setUITemplate(popTemplate=True)

def createArnoldMayaintegrationSettings():

    cmds.setUITemplate('attributeEditorTemplate', pushTemplate=True)
    cmds.columnLayout(adjustableColumn=True)
                  
    cmds.attrControlGrp('os_enable_swatch_render',
                        label="Enable Swatch Render",
                        attribute='defaultArnoldRenderOptions.enable_swatch_render')

    cmds.attrControlGrp('os_standin_draw_override',
                        label="StandIn Viewport Display",
                        attribute='defaultArnoldRenderOptions.standin_draw_override')

    cmds.separator()
    cmds.frameLayout('arnoldMayaIntegrationSettings', label="Maya Render View", cll=True, cl=1)
    cmds.attrControlGrp('os_progressive_rendering',
                        label='Progressive Refinement',
                        attribute='defaultArnoldRenderOptions.progressive_rendering')

    cmds.attrControlGrp('os_progressive_initial_level',
                        label="Initial Sampling Level",
                        attribute='defaultArnoldRenderOptions.progressive_initial_level')
                    
    cmds.separator()
                    
    cmds.attrControlGrp('os_clear_before_render',
                        label="Clear Before Render",
                        attribute='defaultArnoldRenderOptions.clear_before_render')
                   
    cmds.attrControlGrp('os_force_scene_update_before_IPR_refresh',
                        label='Force Scene Update On IPR Refresh',
                        attribute='defaultArnoldRenderOptions.force_scene_update_before_IPR_refresh')
    
    cmds.attrControlGrp('os_force_texture_cache_flush_after_render',
                        label='Force Texture Cache Flush After Render',
                        attribute='defaultArnoldRenderOptions.force_texture_cache_flush_after_render')
                   
    

    cmds.setParent('..')
    cmds.setParent('..')
    

    cmds.setUITemplate(popTemplate=True)
    
def createArnoldLicensingSettings():
    cmds.setUITemplate('attributeEditorTemplate', pushTemplate=True)
    cmds.columnLayout(adjustableColumn=True)
    
    cmds.attrControlGrp('os_abort_on_license_fail',
                        label="Abort On License Fail",
                        attribute='defaultArnoldRenderOptions.abortOnLicenseFail')

    cmds.attrControlGrp('os_skip_license_check',
                        label="Render with Watermarks (Skip License Check)",
                        attribute='defaultArnoldRenderOptions.skip_license_check')

    cmds.setParent('..')

    cmds.setUITemplate(popTemplate=True)

def LoadFilenameButtonPush(*args):
    import os
    basicFilter = 'All Files (*.*)'
    initFolder = cmds.textFieldGrp("ls_log_filename", query=True, text=True)
    if "$MTOA_LOG_PATH" in initFolder:
        logPath = maya.mel.eval('getenv "MTOA_LOG_PATH"')
        if not logPath:
            logPath = cmds.workspace(query=True, rootDirectory=True)
        resolvedFolder = initFolder.replace("$MTOA_LOG_PATH",logPath)
    else:
        resolvedFolder = initFolder
    resolvedFolder = os.path.split(resolvedFolder)
    ret = cmds.fileDialog2(fileFilter=basicFilter, cap='Select Log File',okc='Select',fm=0,startingDirectory=resolvedFolder[0])
    if ret is not None and len(ret):
        cmds.textFieldGrp("ls_log_filename", edit=True, text=ret[0])
        cmds.setAttr("defaultArnoldRenderOptions.log_filename", ret[0], type="string")

def ChangeLogToConsole(*args):
    logToConsole = cmds.getAttr('defaultArnoldRenderOptions.log_to_console')
    logToFile = cmds.getAttr('defaultArnoldRenderOptions.log_to_file')
    cmds.attrControlGrp('log_max_warnings', edit=True, enable=logToConsole or logToFile)

def ChangeLogToFile(*args):
    logToFile = cmds.getAttr('defaultArnoldRenderOptions.log_to_file')
    logToConsole = cmds.getAttr('defaultArnoldRenderOptions.log_to_console')
    cmds.textFieldGrp('ls_log_filename', edit=True, enable=logToFile)
    cmds.symbolButton("ls_log_filename_button", edit=True, enable=logToFile)
    cmds.attrControlGrp('log_max_warnings', edit=True, enable=logToConsole or logToFile)

def LoadStatsFileButtonPush(*args):
    import os
    basicFilter = 'JSON Files (*.json)'
    initFolder = cmds.textFieldGrp("ls_stats_file", query=True, text=True)
    if "$MTOA_LOG_PATH" in initFolder:
        logPath = maya.mel.eval('getenv "MTOA_LOG_PATH"')
        if not logPath:
            logPath = cmds.workspace(query=True, rootDirectory=True)
        resolvedFolder = initFolder.replace("$MTOA_LOG_PATH",logPath)
    else:
        resolvedFolder = initFolder
    resolvedFolder = os.path.split(resolvedFolder)
    ret = cmds.fileDialog2(fileFilter=basicFilter, cap='Select JSON File',okc='Select',fm=0,startingDirectory=resolvedFolder[0])
    if ret is not None and len(ret):
        cmds.textFieldGrp("ls_stats_file", edit=True, text=ret[0])
        cmds.setAttr("defaultArnoldRenderOptions.stats_file", ret[0], type="string")

def LoadProfileFileButtonPush(*args):
    import os
    basicFilter = 'JSON Files (*.json)'
    initFolder = cmds.textFieldGrp("ls_profile_file", query=True, text=True)
    if "$MTOA_LOG_PATH" in initFolder:
        logPath = maya.mel.eval('getenv "MTOA_LOG_PATH"')
        if not logPath:
            logPath = cmds.workspace(query=True, rootDirectory=True)
        resolvedFolder = initFolder.replace("$MTOA_LOG_PATH",logPath)
    else:
        resolvedFolder = initFolder
    resolvedFolder = os.path.split(resolvedFolder)
    ret = cmds.fileDialog2(fileFilter=basicFilter, cap='Select JSON File',okc='Select',fm=0,startingDirectory=resolvedFolder[0])
    if ret is not None and len(ret):
        cmds.textFieldGrp("ls_profile_file", edit=True, text=ret[0])
        cmds.setAttr("defaultArnoldRenderOptions.profile_file", ret[0], type="string")

def UpdateRenderStats(*args):
    renderStats = cmds.getAttr('defaultArnoldRenderOptions.stats_enable')
    cmds.textFieldGrp('ls_stats_file', edit=True, enable=renderStats)
    cmds.symbolButton("ls_stats_file_button", edit=True, enable=renderStats)
    cmds.attrControlGrp("ls_stats_mode", edit=True, enable=renderStats)

def UpdateProfile(*args):
    profile = cmds.getAttr('defaultArnoldRenderOptions.profile_enable')
    cmds.textFieldGrp('ls_profile_file', edit=True, enable=profile)
    cmds.symbolButton("ls_profile_file_button", edit=True, enable=profile)
    

def createArnoldLogSettings():

    cmds.setUITemplate('attributeEditorTemplate', pushTemplate=True)
    cmds.columnLayout(adjustableColumn=True)

    logToFile = cmds.getAttr('defaultArnoldRenderOptions.log_to_file')
    logToConsole = cmds.getAttr('defaultArnoldRenderOptions.log_to_console')

    
    cmds.attrControlGrp('log_verbosity',
                        label="Verbosity Level",
                        enable=logToConsole,
                        attribute='defaultArnoldRenderOptions.log_verbosity')                
                        
    
    cmds.checkBoxGrp('log_to_console',
                    label='Console',
                    changeCommand=ChangeLogToConsole)

    cmds.connectControl('log_to_console', 'defaultArnoldRenderOptions.log_to_console', index=1)
    cmds.connectControl('log_to_console', 'defaultArnoldRenderOptions.log_to_console', index=2)
    
    cmds.checkBoxGrp('log_to_file',
                    label='File',
                    changeCommand=ChangeLogToFile)

    cmds.connectControl('log_to_file', 'defaultArnoldRenderOptions.log_to_file', index=1)
    cmds.connectControl('log_to_file', 'defaultArnoldRenderOptions.log_to_file', index=2)
    
    cmds.rowLayout(numberOfColumns=2, columnWidth2=(80,220), adjustableColumn=2, columnAttach=[(1, 'left', 0), (2, 'left', -10)])
    path = cmds.textFieldGrp('ls_log_filename',
                                label='Filename',
                                enable=logToFile,
                                cc=updateLogSettings,
                                width=325)
    cmds.symbolButton('ls_log_filename_button', image='navButtonBrowse.png', command=LoadFilenameButtonPush, enable=logToFile)
    cmds.connectControl('ls_log_filename', 'defaultArnoldRenderOptions.log_filename', index=1)
    cmds.connectControl('ls_log_filename', 'defaultArnoldRenderOptions.log_filename', index=2)
    cmds.setParent('..')
    
    '''
    cmds.attrControlGrp('log_filename',
                        label="Filename",
                        attribute='defaultArnoldRenderOptions.log_filename',
                        cc=updateLogSettings)
    '''

    cmds.attrControlGrp('log_max_warnings',
                        label='Max. Warnings',
                        enable=logToConsole or logToFile,
                        attribute='defaultArnoldRenderOptions.log_max_warnings')

    
    cmds.separator()
    cmds.checkBoxGrp('ls_stats_enable',
                    label='Render Statistics',
                    changeCommand=UpdateRenderStats)
    cmds.connectControl('ls_stats_enable', 'defaultArnoldRenderOptions.stats_enable', index=1)
    cmds.connectControl('ls_stats_enable', 'defaultArnoldRenderOptions.stats_enable', index=2)
    

    cmds.rowLayout(numberOfColumns=2, columnWidth2=(80,220), adjustableColumn=2, columnAttach=[(1, 'left', 0), (2, 'left', -10)])
    path = cmds.textFieldGrp('ls_stats_file',
                                label='Stats File Path',
                                width=325)
    cmds.symbolButton('ls_stats_file_button', image='navButtonBrowse.png', command=LoadStatsFileButtonPush)
    cmds.connectControl('ls_stats_file', 'defaultArnoldRenderOptions.stats_file', index=1)
    cmds.connectControl('ls_stats_file', 'defaultArnoldRenderOptions.stats_file', index=2)
    cmds.setParent('..')

    cmds.attrControlGrp('ls_stats_mode',
                        label="Stats Mode",
                        attribute='defaultArnoldRenderOptions.stats_mode')
    cmds.connectControl('ls_stats_mode', 'defaultArnoldRenderOptions.stats_mode', index=1)
    cmds.connectControl('ls_stats_mode', 'defaultArnoldRenderOptions.stats_mode', index=2)

    UpdateRenderStats()
    cmds.separator()
    cmds.checkBoxGrp('ls_profile_enable',
        label='Profile',
        changeCommand=UpdateProfile)
    cmds.connectControl('ls_profile_enable', 'defaultArnoldRenderOptions.profile_enable', index=1)
    cmds.connectControl('ls_profile_enable', 'defaultArnoldRenderOptions.profile_enable', index=2)

    cmds.rowLayout(numberOfColumns=2, columnWidth2=(80,220), adjustableColumn=2, columnAttach=[(1, 'left', 0), (2, 'left', -10)])
    path = cmds.textFieldGrp('ls_profile_file',
                                label='Profile File Path',
                                width=325)
    cmds.symbolButton('ls_profile_file_button', image='navButtonBrowse.png', command=LoadProfileFileButtonPush)
    cmds.connectControl('ls_profile_file', 'defaultArnoldRenderOptions.profile_file', index=1)
    cmds.connectControl('ls_profile_file', 'defaultArnoldRenderOptions.profile_file', index=2)
    cmds.setParent('..')
    UpdateProfile()

    cmds.separator()

    cmds.checkBoxGrp('mtoa_translation_info',
                    label='MtoA Translation Info')
    cmds.connectControl('mtoa_translation_info', 'defaultArnoldRenderOptions.mtoa_translation_info', index=1)
    cmds.connectControl('mtoa_translation_info', 'defaultArnoldRenderOptions.mtoa_translation_info', index=2)

    cmds.setParent('..')    
    cmds.setUITemplate(popTemplate=True)
    
def createArnoldErrorHandlingSettings():
    cmds.setUITemplate('attributeEditorTemplate', pushTemplate=True)
    cmds.columnLayout(adjustableColumn=True)

    cmds.attrControlGrp('os_abort_on_error',
                        label="Abort On Error",
                        attribute='defaultArnoldRenderOptions.abortOnError')
                   
    cmds.separator()
    
    cmds.attrControlGrp('os_error_color_bad_texture',
                        label="Texture Error Color",
                        attribute='defaultArnoldRenderOptions.errorColorBadTexture')
                   
    cmds.attrControlGrp('os_error_color_bad_pixel',
                        label="NaN Error Color",
                        attribute='defaultArnoldRenderOptions.errorColorBadPixel')

    cmds.setParent('..')

    cmds.setUITemplate(popTemplate=True)

def createArnoldUserOptionsSettings():
    cmds.setUITemplate('attributeEditorTemplate', pushTemplate=True)
    cmds.columnLayout(adjustableColumn=True)
    
    cmds.attrControlGrp('os_user_options',
                        label="Options",
                        attribute='defaultArnoldRenderOptions.aiUserOptions')
    cmds.setParent('..')
    
    cmds.setUITemplate(popTemplate=True)

    
def createArnoldRendererDiagnosticsTab():

    # Make sure the aiOptions node exists
    #core.createOptions()

    parentForm = cmds.setParent(query=True)
    
    cmds.setUITemplate('attributeEditorTemplate', pushTemplate=True)
    cmds.scrollLayout('arnoldDiagnosticsScrollLayout', horizontalScrollBarThickness=0)
    cmds.columnLayout('arnoldDiagnosticsColumn', adjustableColumn=True)

    # Log
    #
    cmds.frameLayout('arnoldLogSettings', label="Log", cll=True, cl=0)
    createArnoldLogSettings()
    cmds.setParent('..')
    
    # Error handling
    #
    cmds.frameLayout('arnoldErrorHandlingSettings', label="Error Handling", cll=True, cl=0)
    createArnoldErrorHandlingSettings()
    cmds.setParent('..')
    
    # User Options
    #
    cmds.frameLayout('arnoldUserOptionsSettings', label="User Options", cll=True,  cl=0)
    createArnoldUserOptionsSettings()
    cmds.setParent('..')
    
    # Overrides
    #
    cmds.frameLayout('arnoldOverrideSettings', label="Feature Overrides", cll=True,  cl=0)
    createArnoldOverrideSettings()
    cmds.setParent('..')


    cmds.formLayout(parentForm,
                    edit=True,
                    af=[('arnoldDiagnosticsScrollLayout', "top", 0),
                        ('arnoldDiagnosticsScrollLayout', "bottom", 0),
                        ('arnoldDiagnosticsScrollLayout', "left", 0),
                        ('arnoldDiagnosticsScrollLayout', "right", 0)])

    cmds.setParent(parentForm)
    
def updateArnoldRendererDiagnosticsTab(*args):
    updateLogSettings()
    
def createArnoldRendererSystemTab():

    # Make sure the aiOptions node exists
    #core.createOptions()

    parentForm = cmds.setParent(query=True)
    
    cmds.setUITemplate('attributeEditorTemplate', pushTemplate=True)
    cmds.scrollLayout('arnoldSystemScrollLayout', horizontalScrollBarThickness=0)
    cmds.columnLayout('arnoldSystemColumn', adjustableColumn=True)

    platformName = sys.platform

    if not platformName.startswith('darwin'):
        cmds.frameLayout('arnoldGpuSettings', label="GPU Settings", cll=True, cl=0)
        createGpuSettings()
        cmds.setParent('..')

    # Render
    #
    cmds.frameLayout('arnoldRenderSettings', label="Render Settings", cll= True, cl=0)
    createArnoldRenderSettings()
    cmds.setParent('..')
    
    # Maya Integration
    #
    cmds.frameLayout('arnoldMayaIntegrationSettings', label="Maya Integration", cll=True, cl=0)
    createArnoldMayaintegrationSettings()
    cmds.setParent('..')

    # Search paths
    #
    cmds.frameLayout('arnoldPathSettings', label="Search Paths", cll=True, cl=0)
    createArnoldPathSettings()
    cmds.setParent('..')
    
    # Licensing
    #
    cmds.frameLayout('arnoldLicensingSettings', label="Licensing", cll=True, cl=0)
    createArnoldLicensingSettings()
    cmds.setParent('..')
    
    

    cmds.formLayout(parentForm,
                    edit=True,
                    af=[('arnoldSystemScrollLayout', "top", 0),
                        ('arnoldSystemScrollLayout', "bottom", 0),
                        ('arnoldSystemScrollLayout', "left", 0),
                        ('arnoldSystemScrollLayout', "right", 0)])

    cmds.setParent(parentForm)
    
    updateRenderSettings()
    
def updateArnoldRendererSystemTab(*args):
    updateRenderSettings()

def createArnoldRendererGlobalsTab():

    # Make sure the aiOptions node exists
    core.createOptions()

    parentForm = cmds.setParent(query=True)

    cmds.setUITemplate('attributeEditorTemplate', pushTemplate=True)
    cmds.scrollLayout('arnoldGlobalsScrollLayout', horizontalScrollBarThickness=0)
    cmds.columnLayout('arnoldTabColumn', adjustableColumn=True)

    # Sampling
    #
    cmds.frameLayout('arnoldSamplingSettings', label='Sampling', cll=True, cl=0)
    createArnoldSamplingSettings()
    cmds.setParent('..')

    # Ray depth
    #
    cmds.frameLayout('arnoldRayDepthSettings', label="Ray Depth", cll= True, cl=1)
    createArnoldRayDepthSettings()
    cmds.setParent('..')

    # Environment
    #
    cmds.frameLayout('arnoldEnvironmentSettings', label="Environment", cll= True, cl=1)
    createArnoldEnvironmentSettings()
    cmds.setParent('..')

    # Motion Blur
    #
    cmds.frameLayout('arnoldMotionBlurSettings', label="Motion Blur", cll= True, cl=1)
    createArnoldMotionBlurSettings()
    cmds.setParent('..')

    # Operators
    #
    cmds.frameLayout('arnoldOperatorSettings', label="Operators", cll= True, cl=1)
    createArnoldOperatorSettings()
    cmds.setParent('..')


    # Light Linking
    #
    cmds.frameLayout('arnoldLightSettings', label="Lights", cll= True, cl=1)
    createArnoldLightSettings()
    cmds.setParent('..')


    # Gamma correction
    #
    
    maya_version = utils.getMayaVersion()
    if maya_version < 2016:
        cmds.frameLayout('arnoldGammaSettings', label="Gamma Correction", cll=True, cl=1)
        createArnoldGammaSettings()
        cmds.setParent('..')

    # Gamma correction
    #
    cmds.frameLayout('arnoldTextureSettings', label="Textures", cll=True, cl=1)
    createArnoldTextureSettings()
    cmds.setParent('..')

    # Subdivision Surfaces
    #
    cmds.frameLayout('arnoldSubdivSettings', label="Subdivision", cll= True, cl=1)
    createArnoldSubdivSettings()
    cmds.setParent('..')
    
    cmds.formLayout(parentForm,
                    edit=True,
                    af=[('arnoldGlobalsScrollLayout', "top", 0),
                        ('arnoldGlobalsScrollLayout', "bottom", 0),
                        ('arnoldGlobalsScrollLayout', "left", 0),
                        ('arnoldGlobalsScrollLayout', "right", 0)])

    cmds.setParent(parentForm)

    updateArnoldRendererGlobalsTab()

def updateBackgroundSettings(*args):
    background = getBackgroundShader()
    if cmds.textField( 'defaultArnoldRenderOptionsBackgroundTextField', query=True, exists=True):
        cmds.textField('defaultArnoldRenderOptionsBackgroundTextField', edit=True, text=background)
    if cmds.symbolButton( 'defaultArnoldRenderOptionsBackgroundSelectButton', query=True, exists=True):
        if not background:
            cmds.symbolButton('defaultArnoldRenderOptionsBackgroundSelectButton', edit=True, enable=False)
        else:
            cmds.symbolButton('defaultArnoldRenderOptionsBackgroundSelectButton', edit=True, enable=True)
            
def updateAtmosphereSettings(*args):
    atmosphere = getAtmosphereShader()
    if cmds.textField( 'defaultArnoldRenderOptionsAtmosphereTextField', query=True, exists=True):
        cmds.textField('defaultArnoldRenderOptionsAtmosphereTextField', edit=True, text=atmosphere)
    if cmds.symbolButton( 'defaultArnoldRenderOptionsAtmosphereSelectButton', query=True, exists=True):
        if not atmosphere:
            cmds.symbolButton('defaultArnoldRenderOptionsAtmosphereSelectButton', edit=True, enable=False)
        else:
            cmds.symbolButton('defaultArnoldRenderOptionsAtmosphereSelectButton', edit=True, enable=True)

def updateArnoldRendererGlobalsTab(*args):
    updateComputeSamples()
    updateSamplingSettings()
    updateMotionBlurSettings()
    updateAdaptiveSettings()
    updateAutotileSettings()
    

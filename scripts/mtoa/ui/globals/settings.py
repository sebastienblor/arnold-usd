import pymel.core as pm
from mtoa.ui.ae.templates import createTranslatorMenu
from mtoa.callbacks import *
import mtoa.core as core

def updateRenderSettings(*args):
    flag = pm.getAttr('defaultArnoldRenderOptions.threads_autodetect') == False
    pm.attrControlGrp('os_threads', edit=True, enable=flag)

def updateSamplingSettings(*args):
    flag = (pm.getAttr('defaultArnoldRenderOptions.use_sample_clamp') == True) 
    pm.attrControlGrp('ss_max_value', edit=True, enable=flag)
    pm.attrControlGrp('ss_clamp_sample_values_AOVs', edit=True, enable=flag)

def updateComputeSamples(*args):
    AASamples = pm.getAttr('defaultArnoldRenderOptions.AASamples')
    GISamples = pm.getAttr('defaultArnoldRenderOptions.GIDiffuseSamples')
    glossySamples = pm.getAttr('defaultArnoldRenderOptions.GIGlossySamples')
    refractionSamples = pm.getAttr('defaultArnoldRenderOptions.GIRefractionSamples')
    
    diffuseDepth = pm.getAttr('defaultArnoldRenderOptions.GIDiffuseDepth')
    glossyDepth = pm.getAttr('defaultArnoldRenderOptions.GIGlossyDepth')
    refractionDepth = pm.getAttr('defaultArnoldRenderOptions.GIRefractionDepth')
    
    if AASamples <= 0:
        AASamples = 1
    AASamplesComputed = AASamples * AASamples
    
    GISamplesComputed = GISamples * GISamples * AASamplesComputed
    GISamplesComputedDepth = GISamplesComputed*diffuseDepth
    
    glossySamplesComputed = glossySamples * glossySamples * AASamplesComputed
    glossySamplesComputedDepth = glossySamplesComputed*glossyDepth
    
    refractionSamplesComputed = refractionSamples * refractionSamples * AASamplesComputed
    refractionSamplesComputedDepth = refractionSamplesComputed*refractionDepth
    
    totalSamples = AASamplesComputed + GISamplesComputed + glossySamplesComputed + refractionSamplesComputed
    totalSamplesDepth = AASamplesComputed + GISamplesComputedDepth + glossySamplesComputedDepth + refractionSamplesComputedDepth

    pm.text( "textAASamples",
               edit=True, 
               label='AA Samples : %i' % AASamplesComputed)

    pm.text( "textGISamples",
               edit=True, 
               label='GI Samples (with Max Depth) : %i (%i)' % (GISamplesComputed, GISamplesComputedDepth))
    
    pm.text( "textGlossySamples",
               edit=True, 
               label='Glossy Samples (with Max Depth) : %i (%i)' % (glossySamplesComputed, glossySamplesComputedDepth))
        
    pm.text( "textRefractionSamples",
               edit=True, 
               label='Refraction Samples (with Max Depth) : %i (%i)' % (refractionSamplesComputed, refractionSamplesComputedDepth))
        
    pm.text( "textTotalSamples",
               edit=True, 
               label='Total Samples without lights (with Max Depth) : %i (%i)' % (totalSamples, totalSamplesDepth))


def updateMotionBlurSettings(*args):
    flag = pm.getAttr('defaultArnoldRenderOptions.motion_blur_enable') == True
    pm.attrControlGrp('mb_camera_enable', edit=True, enable=flag)
    pm.attrControlGrp('mb_objects_enable', edit=True, enable=flag)
    pm.attrControlGrp('mb_object_deform_enable', edit=True, enable=flag)
    pm.attrControlGrp('mb_lights_enable', edit=True, enable=flag)
    pm.attrControlGrp('mb_shutter_size', edit=True, enable=flag)
    pm.attrControlGrp('mb_shutter_offset', edit=True, enable=flag)
    pm.attrControlGrp('mb_shutter_type', edit=True, enable=flag)
    pm.attrControlGrp('mb_motion_steps', edit=True, enable=flag)
    pm.attrControlGrp('mb_motion_frames', edit=True, enable=flag)
    pm.attrControlGrp('mb_shader_enable', edit=True, enable=flag)


def updateLogSettings(*args):
    name = pm.getAttr('defaultArnoldRenderOptions.log_filename')
    pm.attrControlGrp('log_file_verbosity', edit=True, enable= name != "")

def getBackgroundShader(*args):
    conns = pm.listConnections('defaultArnoldRenderOptions.background', s=True, d=False, p=True)
    if conns:
        return conns[0].split('.')[0]
    return ""

def selectBackground(*args):
    node = getBackgroundShader()
    if node:
        pm.select(node, r=True)

def changeBackground(node, field):
    connection = pm.listConnections('defaultArnoldRenderOptions.background')
    if connection:
        if pm.nodeType(connection[0]) == 'transform':
            connection = pm.listRelatives(connection[0], s=True)
        if str(connection[0]) == str(node):
            selectBackground()
            return 0
    pm.connectAttr("%s.message"%node,'defaultArnoldRenderOptions.background', force=True)
    pm.textField(field, edit=True, text=node)
    selectBackground()

def createBackground(type, field):
    bg = getBackgroundShader()
    #if bg:
        #pm.delete(bg)
    node = pm.shadingNode(type, asShader=True, name=type+"Shape")
    changeBackground(node, field)


def deleteBackground(field):
    node = getBackgroundShader();
    if node:
        pm.disconnectAttr("%s.message"%node, 'defaultArnoldRenderOptions.background')
        pm.delete(node)
        pm.textField(field, edit=True, text="")

def buildBackgroundMenu(popup, field):

    switches = pm.ls(type='aiRaySwitch')
    skies = pm.ls(type='aiSky')

    pm.popupMenu(popup, edit=True, deleteAllItems=True)
    for item in skies:
        pm.menuItem(parent=popup, label=item, command=Callback(changeBackground, item, field))

    pm.menuItem(parent=popup, divider=True)

    for item in switches:
        pm.menuItem(parent=popup, label=item, command=Callback(changeBackground, item, field))

    pm.menuItem(parent=popup, divider=True)

    pm.menuItem(parent=popup, label="Create Sky Shader", command=Callback(createBackground, "aiSky", field))
    pm.menuItem(parent=popup, label="Create RaySwitch Shader", command=Callback(createBackground, "aiRaySwitch", field))

    pm.menuItem(parent=popup, divider=True)

    pm.menuItem(parent=popup, label="Delete", command=Callback(deleteBackground, field))

def selectAtmosphere(*args):
    bkg = pm.getAttr('defaultArnoldRenderOptions.atmosphere')

    if bkg == 1:
        pm.createNode('aiFog', shared=True, name='defaultFog')
    elif bkg == 2:
        pm.createNode('aiVolumeScattering', shared=True, name='defaultVolumeScattering')



def createArnoldRenderSettings():

    pm.setUITemplate('attributeEditorTemplate', pushTemplate=True)
    pm.columnLayout('arnoldRenderLayout', adjustableColumn=True)

    pm.attrControlGrp('os_renderType',
                   label="Render Type",
                   attribute='defaultArnoldRenderOptions.renderType')

    pm.attrControlGrp('os_progressive_rendering',
                   label='Progressive Rendering',
                   attribute='defaultArnoldRenderOptions.progressive_rendering')

    pm.attrControlGrp('os_progressive_initial_level',
                    label="Progressive Initial Level",
                    attribute='defaultArnoldRenderOptions.progressive_initial_level')

    pm.separator()

    pm.attrControlGrp('os_bucket_scanning',
                   label="Bucket Scanning",
                   attribute='defaultArnoldRenderOptions.bucketScanning')

    pm.attrControlGrp('os_bucket_size',
                   label="Bucket Size",
                   attribute='defaultArnoldRenderOptions.bucketSize')

    pm.attrControlGrp('os_clear_before_render',
                   label="Clear Before Render",
                   attribute='defaultArnoldRenderOptions.clear_before_render')
                   
    pm.attrControlGrp('os_force_scene_update_before_IPR_refresh',
                   label='Force Scene Update On IPR Refresh',
                   attribute='defaultArnoldRenderOptions.force_scene_update_before_IPR_refresh')

    pm.separator()

    pm.checkBoxGrp('os_threads_autodetect',
                     cc=updateRenderSettings,
                     label='',
                     label1='Autodetect Threads')

    pm.connectControl('os_threads_autodetect', 'defaultArnoldRenderOptions.threads_autodetect', index=2)

    pm.attrControlGrp('os_threads',
                   label="Threads",
                   attribute='defaultArnoldRenderOptions.threads')

    pm.separator()

    pm.attrControlGrp('os_binary_ass',
                   label='Binary Ass Export',
                   attribute='defaultArnoldRenderOptions.binaryAss')
    
    pm.separator()
                    
    pm.attrControlGrp('os_outputAssBoundingBox',
                   label="Export Bounding Box",
                   attribute='defaultArnoldRenderOptions.outputAssBoundingBox')                   

    pm.attrControlGrp('os_preserve_scene_data',
                   label='Preserve Scene Data',
                   attribute='defaultArnoldRenderOptions.preserveSceneData')

    pm.separator()

    pm.attrControlGrp('os_abort_on_error',
                   label="Abort On Error",
                   attribute='defaultArnoldRenderOptions.abortOnError')

    pm.attrControlGrp('os_abort_on_license_fail',
                   label="Abort On License Fail",
                   attribute='defaultArnoldRenderOptions.abortOnLicenseFail')

    pm.attrControlGrp('os_skip_license_check',
                   label="Skip License Check",
                   attribute='defaultArnoldRenderOptions.skip_license_check')
                   
    pm.attrControlGrp('os_enable_swatch_render',
                   label="Enable Swatch Render",
                   attribute='defaultArnoldRenderOptions.enable_swatch_render')
                   
    pm.separator()
    
    pm.attrControlGrp('os_user_options',
                   label="User Options",
                   attribute='defaultArnoldRenderOptions.aiUserOptions')
             
    pm.setParent('..')

    pm.setUITemplate(popTemplate=True)


def updateArnoldFilterOptions(*args):
    pass

def raytracedSSSChanged(someArg=None):
    enableRaytracedSSS = pm.getAttr('defaultArnoldRenderOptions.enable_raytraced_SSS')
    pm.attrControlGrp('ss_sss_bssrdf_samples', edit=True, enable=enableRaytracedSSS)
    pm.attrControlGrp('ss_sss_sample_factor', edit=True, enable=not enableRaytracedSSS)

def createArnoldSamplingSettings():

    pm.setUITemplate('attributeEditorTemplate', pushTemplate=True)
    pm.columnLayout(adjustableColumn=True)

    pm.text( "textAASamples", 
               font = "smallBoldLabelFont",
               align='left',
               )
    
    pm.text( "textGISamples", 
               font = "smallBoldLabelFont",
               align='left',
               )
    
    pm.text( "textGlossySamples", 
               font = "smallBoldLabelFont",
               align='left',
               )

    pm.text( "textRefractionSamples", 
               font = "smallBoldLabelFont",
               align='left',
               )

    pm.text( "textTotalSamples", 
               font = "smallBoldLabelFont",
               align='left',
               )

    pm.separator()
    
    pm.attrControlGrp('ss_lock_sampling_noise',
                        label="Lock Sampling Pattern",
                        attribute='defaultArnoldRenderOptions.lock_sampling_noise')
                        
    pm.separator()

    pm.intSliderGrp('ss_AA_samples',
                        label="AA Samples",
                        minValue = 1,
                        maxValue = 10,
                        fieldMinValue=-10,
                        fieldMaxValue=100,
                        cc=lambda *args: pm.evalDeferred(updateComputeSamples)
                        )

    pm.connectControl('ss_AA_samples', 'defaultArnoldRenderOptions.AASamples', index=1)
    pm.connectControl('ss_AA_samples', 'defaultArnoldRenderOptions.AASamples', index=2)
    pm.connectControl('ss_AA_samples', 'defaultArnoldRenderOptions.AASamples', index=3)

    '''
    pm.attrControlGrp('ss_AA_samples',
                        label="AA Samples",
                        attribute='defaultArnoldRenderOptions.AASamples',
                        cc=updateComputeSamples
                        
                        )
    '''
    pm.intSliderGrp('ss_hemi_samples',
                        label="Diffuse Samples",
                        maxValue = 10,
                        fieldMaxValue=100,
                        cc=lambda *args: pm.evalDeferred(updateComputeSamples))
    
    pm.connectControl('ss_hemi_samples', 'defaultArnoldRenderOptions.GIDiffuseSamples', index=1)
    pm.connectControl('ss_hemi_samples', 'defaultArnoldRenderOptions.GIDiffuseSamples', index=2)
    pm.connectControl('ss_hemi_samples', 'defaultArnoldRenderOptions.GIDiffuseSamples', index=3)
    '''
    pm.attrControlGrp('ss_hemi_samples',
                        label="Hemi Samples",
                        attribute='defaultArnoldRenderOptions.GIDiffuseSamples')
    '''

    
    pm.intSliderGrp('ss_glossy_samples',
                        label="Glossy Samples",
                        maxValue = 10,
                        fieldMaxValue=100,
                        cc=lambda *args: pm.evalDeferred(updateComputeSamples))
    
    pm.connectControl('ss_glossy_samples', 'defaultArnoldRenderOptions.GIGlossySamples', index=1)
    pm.connectControl('ss_glossy_samples', 'defaultArnoldRenderOptions.GIGlossySamples', index=2)
    pm.connectControl('ss_glossy_samples', 'defaultArnoldRenderOptions.GIGlossySamples', index=3)    
    
    pm.intSliderGrp('ss_refraction_samples',
                        label="Refraction Samples",
                        maxValue = 10,
                        fieldMaxValue=100,
                        cc=lambda *args: pm.evalDeferred(updateComputeSamples))
    
    pm.connectControl('ss_refraction_samples', 'defaultArnoldRenderOptions.GIRefractionSamples', index=1)
    pm.connectControl('ss_refraction_samples', 'defaultArnoldRenderOptions.GIRefractionSamples', index=2)
    pm.connectControl('ss_refraction_samples', 'defaultArnoldRenderOptions.GIRefractionSamples', index=3)    

    '''
    pm.attrControlGrp('ss_glossy_samples',
                        label="Glossy Samples",
                        attribute='defaultArnoldRenderOptions.giGlossySamples')
    '''
    
    pm.frameLayout(label="Diffusion SSS", collapse=False)
    
    pm.checkBoxGrp('ss_enable_raytraced_SSS',
                   label="Raytraced")
                   
    pm.connectControl('ss_enable_raytraced_SSS', 'defaultArnoldRenderOptions.enable_raytraced_SSS', index=1)
    pm.connectControl('ss_enable_raytraced_SSS', 'defaultArnoldRenderOptions.enable_raytraced_SSS', index=2)
                   
    enableRaytracedSSS = pm.getAttr('defaultArnoldRenderOptions.enable_raytraced_SSS')
                        
    pm.attrControlGrp('ss_sss_bssrdf_samples',
                   label="BSSRDF Samples",
                   enable=enableRaytracedSSS,
                   attribute='defaultArnoldRenderOptions.sss_bssrdf_samples')
    
    pm.attrControlGrp('ss_sss_sample_factor',
                   label="PointCloud Sample Factor",
                   enable=not enableRaytracedSSS,
                   attribute='defaultArnoldRenderOptions.sss_sample_factor')
                   
    pm.scriptJob(ac=['defaultArnoldRenderOptions.enable_raytraced_SSS', raytracedSSSChanged])
    
    pm.setParent('..')
    
    pm.frameLayout(label="Volumes", collapse=False)
    
    pm.attrControlGrp('ss_volume_sampling_mode',
                      label='Sampling Mode',
                      attribute='defaultArnoldRenderOptions.volume_sampling_mode')
                      
    pm.attrControlGrp('ss_volume_scattering_sampling_rate',
                      label='Scattering Sampling Rate',
                      attribute='defaultArnoldRenderOptions.volume_scattering_sampling_rate')
                      
    pm.attrControlGrp('ss_volume_diffuse_sampling_rate',
                      label='Diffuse Sampling Rate',
                      attribute='defaultArnoldRenderOptions.volume_diffuse_sampling_rate')
                      
    pm.attrControlGrp('ss_volume_shadow_step_size_mult',
                       label='Shadow Step Size Multiplier',
                       attribute='defaultArnoldRenderOptions.volume_shadow_step_size_mult')
                      
    pm.setParent('..')
    
    pm.frameLayout(label="Clamping", collapse=True)

    pm.checkBoxGrp('ss_clamp_sample_values',
                     cc=updateSamplingSettings,
                     label='Clamp Sample Values')

    pm.connectControl('ss_clamp_sample_values', 'defaultArnoldRenderOptions.use_sample_clamp', index=1)
    pm.connectControl('ss_clamp_sample_values', 'defaultArnoldRenderOptions.use_sample_clamp', index=2)

    pm.checkBoxGrp('ss_clamp_sample_values_AOVs',
                     cc=updateSamplingSettings,
                     label='Affect AOVs')

    pm.connectControl('ss_clamp_sample_values_AOVs', 'defaultArnoldRenderOptions.use_sample_clamp_AOVs', index=1)
    pm.connectControl('ss_clamp_sample_values_AOVs', 'defaultArnoldRenderOptions.use_sample_clamp_AOVs', index=2)
    
    '''
    pm.attrControlGrp('ss_clamp_sample_values',
                        label="Clamp Sample Values",
                        attribute='defaultArnoldRenderOptions.use_sample_clamp',
                        cc=updateSamplingSettings)
    '''

    pm.attrControlGrp('ss_max_value',
                        label="Max Value",
                        attribute='defaultArnoldRenderOptions.AASampleClamp')
                        
    pm.setParent('..')
    
    pm.frameLayout(label="Sample Filtering", collapse=True)
    
    createTranslatorMenu('defaultArnoldFilter',
                         label='Filter Type',
                         nodeType='aiAOVFilter',
                         default='gaussian')
     
    pm.setParent('..')

#    pm.rowLayout(numberOfColumns=2, columnWidth=(1, 80))
#    pm.separator(style='none')

    # TODO: connect node to options
    

#    pm.attrEnumOptionMenu('os_filter_type',
#                               cc=updateArnoldFilterOptions,
#                               attribute='defaultArnoldRenderOptions.filter_type',
#                               label='Filter Type')
#    pm.setParent('..')
#    '''
#    pm.attrControlGrp('os_filter_type',
#                        label="Filter Type",
#                        attribute='defaultArnoldRenderOptions.filter_type',
#                        cc=updateArnoldFilterOptions)
#    '''
#
#    pm.columnLayout('cl_filter_width', vis=0)
#    pm.attrControlGrp('os_filter_width',
#                        label="Filter Width",
#                        attribute='defaultArnoldRenderOptions.filter_width')
#
#    pm.setParent('..')
#
#    pm.columnLayout('cl_filter_domain', vis=0)
#    pm.attrControlGrp('os_filter_domain',
#                         label="Filter Domain",
#                         attribute='defaultArnoldRenderOptions.filter_domain')
#
#    pm.setParent('..')
#
#    pm.columnLayout('cl_filter_scalar_mode', vis=0)
#    pm.attrControlGrp('os_filter_scalar_mode',
#                        label="Filter Scalar Mode",
#                        attribute='defaultArnoldRenderOptions.filter_scalar_mode')
#
#    pm.setParent('..')
#
#    pm.columnLayout('cl_filter_minmax', vis=0)
#    pm.attrControlGrp('os_filter_minimum',
#                        label="Filter Minimum",
#                        attribute='defaultArnoldRenderOptions.filter_minimum')
#
#    pm.attrControlGrp('os_filter_maximum',
#                        label="Filter Maximum",
#                        attribute='defaultArnoldRenderOptions.filter_maximum')
#
#    pm.setParent('..')

    pm.setParent('..') # column layout

    pm.setUITemplate(popTemplate=True)
    updateArnoldFilterOptions()


def createArnoldGammaSettings():

    pm.setUITemplate('attributeEditorTemplate', pushTemplate=True)
    pm.columnLayout(adjustableColumn=True)

    pm.attrControlGrp('ss_driver_gamma',
                        label="Display Driver gamma",
                        attribute='defaultArnoldRenderOptions.display_gamma')

    pm.separator()

    pm.attrControlGrp('ss_light_gamma',
                        label="Lights",
                        attribute='defaultArnoldRenderOptions.light_gamma')

    pm.attrControlGrp('ss_shader_gamma',
                   label="Shaders",
                   attribute='defaultArnoldRenderOptions.shader_gamma')

    pm.attrControlGrp('ss_texture_gamma',
                        label="Textures",
                        attribute='defaultArnoldRenderOptions.texture_gamma')

    pm.setParent('..')

    pm.setUITemplate(popTemplate=True)


def createArnoldRayDepthSettings():

    pm.setUITemplate('attributeEditorTemplate', pushTemplate=True)
    pm.columnLayout(adjustableColumn=True)

    pm.attrControlGrp('rs_total_depth',
                        label="Total",
                        attribute='defaultArnoldRenderOptions.GITotalDepth')

    pm.separator(style="none")

    
    pm.intSliderGrp('rs_diffuse_depth',
                        label="Diffuse",
                        maxValue = 16,
                        fieldMaxValue=100,
                        cc=lambda *args: pm.evalDeferred(updateComputeSamples))
    
    pm.connectControl('rs_diffuse_depth', 'defaultArnoldRenderOptions.GIDiffuseDepth', index=1)
    pm.connectControl('rs_diffuse_depth', 'defaultArnoldRenderOptions.GIDiffuseDepth', index=2)
    pm.connectControl('rs_diffuse_depth', 'defaultArnoldRenderOptions.GIDiffuseDepth', index=3)
    
    '''
    pm.attrControlGrp('rs_diffuse_depth',
                        label="Diffuse depth",
                        attribute='defaultArnoldRenderOptions.GIDiffuseDepth')
    '''
    
    pm.intSliderGrp('rs_glossy_depth',
                        label="Glossy",
                        maxValue = 16,
                        fieldMaxValue=100,
                        cc=lambda *args: pm.evalDeferred(updateComputeSamples))
    
    pm.connectControl('rs_glossy_depth', 'defaultArnoldRenderOptions.GIGlossyDepth', index=1)
    pm.connectControl('rs_glossy_depth', 'defaultArnoldRenderOptions.GIGlossyDepth', index=2)
    pm.connectControl('rs_glossy_depth', 'defaultArnoldRenderOptions.GIGlossyDepth', index=3)
    
    '''
    pm.attrControlGrp('rs_glossy_depth',
                        label="Glossy depth",
                        attribute='defaultArnoldRenderOptions.GIGlossyDepth')
    '''

    pm.attrControlGrp('rs_reflection_depth',
                        label="Reflection",
                        attribute='defaultArnoldRenderOptions.GIReflectionDepth')

    pm.intSliderGrp('rs_refraction_depth',
                        label="Refraction ",
                        maxValue = 16,
                        fieldMaxValue=100,
                        cc=lambda *args: pm.evalDeferred(updateComputeSamples))
    
    pm.connectControl('rs_refraction_depth', 'defaultArnoldRenderOptions.GIRefractionDepth', index=1)
    pm.connectControl('rs_refraction_depth', 'defaultArnoldRenderOptions.GIRefractionDepth', index=2)
    pm.connectControl('rs_refraction_depth', 'defaultArnoldRenderOptions.GIRefractionDepth', index=3)

    '''
    pm.attrControlGrp('rs_refraction_depth',
                        label="Refraction depth",
                        attribute='defaultArnoldRenderOptions.GIRefractionDepth')
    '''
    pm.separator(style="none")

    pm.attrControlGrp('rs_auto_transparency_depth',
                        label="Transparency Depth",
                        attribute='defaultArnoldRenderOptions.autoTransparencyDepth')

    pm.attrControlGrp('rs_auto_transparency_threshold',
                        label="Transparency Threshold",
                        attribute='defaultArnoldRenderOptions.autoTransparencyThreshold')

    pm.setParent('..')

    pm.setUITemplate(popTemplate=True)

def createArnoldEnvironmentSettings():

    pm.setUITemplate('attributeEditorTemplate', pushTemplate=True)
    pm.columnLayout(adjustableColumn=True)

    pm.rowLayout(adjustableColumn=2, numberOfColumns=3)
    pm.text(label="Background")
    bgfield = pm.textField("defaultArnoldRenderOptionsBackgroundTextField",editable=False)
    bgpopup = pm.popupMenu(parent=bgfield)
    pm.popupMenu(bgpopup, edit=True, postMenuCommand=Callback(buildBackgroundMenu, bgpopup, bgfield))
    pm.button(label="Select", height=22, width=50, command=selectBackground)
    pm.setParent('..')

    conns = cmds.listConnections('defaultArnoldRenderOptions.background', s=True, d=False)
    if conns:
        pm.textField(bgfield, edit=True, text=conns[0])

    pm.separator(style="none")

    pm.rowLayout(numberOfColumns=2, columnWidth=(1, 80))
    pm.separator(style="none")
    pm.attrEnumOptionMenu('es_atmosphere',
                            label = 'Atmosphere',
                            attribute='defaultArnoldRenderOptions.atmosphere',
                            cc=selectAtmosphere)
    pm.connectControl('es_atmosphere', 'defaultArnoldRenderOptions.atmosphere', index=1)
    pm.connectControl('es_atmosphere', 'defaultArnoldRenderOptions.atmosphere', index=2)
    
    pm.setParent('..')
    pm.setParent('..')

    pm.setUITemplate(popTemplate=True)

def createArnoldMotionBlurSettings():

    pm.setUITemplate('attributeEditorTemplate', pushTemplate=True)
    pm.columnLayout(adjustableColumn=True)

    pm.attrControlGrp('reference_time',
                   label='Reference Time',
                   attribute='defaultArnoldRenderOptions.reference_time')
                   
    pm.checkBoxGrp('mb_enable',
                     cc=updateMotionBlurSettings,
                     label='Enable')

    pm.connectControl('mb_enable', 'defaultArnoldRenderOptions.motion_blur_enable', index=1)
    pm.connectControl('mb_enable', 'defaultArnoldRenderOptions.motion_blur_enable', index=2)

    '''
    pm.attrControlGrp('mb_enable',
                        label="Enable",
                        attribute='defaultArnoldRenderOptions.motion_blur_enable',
                        cc=updateMotionBlurSettings)
    '''

    pm.attrControlGrp('mb_lights_enable',
                        label="Lights",
                        attribute='defaultArnoldRenderOptions.mb_lights_enable')

    pm.attrControlGrp('mb_camera_enable',
                        label="Camera",
                        attribute='defaultArnoldRenderOptions.mb_camera_enable')

    pm.attrControlGrp('mb_objects_enable',
                        label="Objects",
                        attribute='defaultArnoldRenderOptions.mb_objects_enable')

    pm.attrControlGrp('mb_object_deform_enable',
                        label="Object deformation",
                        attribute='defaultArnoldRenderOptions.mb_object_deform_enable')

    pm.attrControlGrp('mb_shader_enable',
                        label="Shaders",
                        attribute='defaultArnoldRenderOptions.mb_shader_enable')                        
                        
    pm.separator()

    pm.floatSliderGrp('mb_shutter_size',
                      label="Shutter Size"
                      )
    pm.connectControl('mb_shutter_size', 'defaultArnoldRenderOptions.shutter_size', index=1)
    pm.connectControl('mb_shutter_size', 'defaultArnoldRenderOptions.shutter_size', index=2)
    pm.connectControl('mb_shutter_size', 'defaultArnoldRenderOptions.shutter_size', index=3)

    
    pm.attrControlGrp('mb_motion_frames',
                        label="Sample Range (Frames)",
                        attribute='defaultArnoldRenderOptions.motion_frames')

    pm.separator(style='none')

    pm.attrControlGrp('mb_shutter_offset',
                        label="Shutter Offset (Frames)",
                        attribute='defaultArnoldRenderOptions.shutter_offset')

    pm.attrControlGrp('mb_shutter_type',
                        label="Shutter Type",
                        attribute='defaultArnoldRenderOptions.shutter_type')

    pm.attrControlGrp('mb_motion_steps',
                        label="Motion Steps",
                        attribute='defaultArnoldRenderOptions.motion_steps')

    pm.setParent('..')

    pm.setUITemplate(popTemplate=True)

def createArnoldSSSSettings():

    pm.setUITemplate('attributeEditorTemplate', pushTemplate=True)
    pm.columnLayout(adjustableColumn=True)

    pm.attrControlGrp('mb_show_samples',
                        label="Show samples",
                        attribute='defaultArnoldRenderOptions.showSamples')   

    pm.setParent('..')

    pm.setUITemplate(popTemplate=True)

def createArnoldLightSettings():

    pm.setUITemplate('attributeEditorTemplate', pushTemplate=True)
    pm.columnLayout(adjustableColumn=True)

    pm.attrControlGrp('lightThreshold',
                      label="Low Light Threshold",
                      attribute='defaultArnoldRenderOptions.lowLightThreshold')

    pm.separator()

    pm.attrControlGrp('lightLinking',
                        label="Light Linking",
                        attribute='defaultArnoldRenderOptions.lightLinking')

    pm.attrControlGrp('shadowLinking',
                        label="Shadow Linking",
                        attribute='defaultArnoldRenderOptions.shadowLinking')

    pm.setParent('..')

    pm.setUITemplate(popTemplate=True)

def createArnoldSubdivSettings():

    pm.setUITemplate('attributeEditorTemplate', pushTemplate=True)
    pm.columnLayout(adjustableColumn=True)

    pm.attrControlGrp('sub_max_subdivisions',
                        label="Max. Subdivisions",
                        attribute='defaultArnoldRenderOptions.maxSubdivisions')

    pm.setParent('..')

    pm.setUITemplate(popTemplate=True)


def createArnoldTextureSettings():

    pm.setUITemplate('attributeEditorTemplate', pushTemplate=True)
    pm.columnLayout(adjustableColumn=True)

    pm.attrControlGrp('texture_automip',
                        label="Auto Mipmap",
                        attribute='defaultArnoldRenderOptions.textureAutomip')

    pm.attrControlGrp('texture_accept_untiled',
                        label="Accept Untiled",
                        attribute='defaultArnoldRenderOptions.textureAcceptUntiled')

    pm.attrControlGrp('use_existing_tiled_textures', 
                        label="Use Existing Tiled Textures", 
                        attribute='defaultArnoldRenderOptions.use_existing_tiled_textures')
    
    pm.attrControlGrp('texture_accept_unmipped',
                        label="Accept Unmipped",
                        attribute='defaultArnoldRenderOptions.textureAcceptUnmipped')
    
    pm.attrControlGrp('texture_autotile',
                        label="Auto Tile Size",
                        attribute='defaultArnoldRenderOptions.textureAutotile')

    pm.attrControlGrp('texture_max_memory_MB',
                        label="Max Cache Size (MB)",
                        attribute='defaultArnoldRenderOptions.textureMaxMemoryMB')

    pm.attrControlGrp('texture_max_open_files',
                        label="Max Open Files",
                        attribute='defaultArnoldRenderOptions.textureMaxOpenFiles')

    pm.separator()

    pm.attrControlGrp('texture_per_file_stats',
                        label="Per File Stats",
                        attribute='defaultArnoldRenderOptions.texturePerFileStats')

    cmds.separator() 

    cmds.attrControlGrp('texture_diffuse_blur', 
                        label="Diffuse Blur", 
                        attribute='defaultArnoldRenderOptions.textureDiffuseBlur') 

    cmds.attrControlGrp('texture_glossy_blur', 
                        label="Glossy Blur", 
                        attribute='defaultArnoldRenderOptions.textureGlossyBlur') 

    pm.setParent('..')

    pm.setUITemplate(popTemplate=True)

def createArnoldOverrideSettings():

    pm.setUITemplate('attributeEditorTemplate', pushTemplate=True)
    pm.columnLayout(adjustableColumn=True)

    pm.attrControlGrp('ignore_textures',
                        attribute='defaultArnoldRenderOptions.ignore_textures')

    pm.attrControlGrp('ignore_shaders',
                        attribute='defaultArnoldRenderOptions.ignore_shaders')

    pm.attrControlGrp('ignore_atmosphere',
                        attribute='defaultArnoldRenderOptions.ignore_atmosphere')

    pm.attrControlGrp('ignore_lights',
                        attribute='defaultArnoldRenderOptions.ignore_lights')

    pm.attrControlGrp('ignore_shadows',
                        attribute='defaultArnoldRenderOptions.ignore_shadows')
                        
    pm.attrControlGrp('ignore_subdivision',
                        attribute='defaultArnoldRenderOptions.ignore_subdivision')

    pm.attrControlGrp('ignore_displacement',
                        attribute='defaultArnoldRenderOptions.ignore_displacement')

    pm.attrControlGrp('ignore_bump',
                        attribute='defaultArnoldRenderOptions.ignore_bump')

    pm.attrControlGrp('ignore_smoothing',
                        attribute='defaultArnoldRenderOptions.ignore_smoothing')
                        
    pm.attrControlGrp('ignore_motion_blur',
                        attribute='defaultArnoldRenderOptions.ignore_motion_blur')

    pm.attrControlGrp('ignore_sss',
                        attribute='defaultArnoldRenderOptions.ignore_sss')
    					
    pm.attrControlGrp('ignore_mis',
                        attribute='defaultArnoldRenderOptions.ignore_mis')
                        
    pm.attrControlGrp('ignore_dof',
                        attribute='defaultArnoldRenderOptions.ignore_dof')

    pm.setParent('..')

    pm.setUITemplate(popTemplate=True)

def createArnoldPathSettings():

    pm.setUITemplate('attributeEditorTemplate', pushTemplate=True)
    pm.columnLayout(adjustableColumn=True)
    
    pm.attrControlGrp('os_plugins_path',
                   label="Plug-ins Path",
                   attribute='defaultArnoldRenderOptions.plugins_path')

    pm.separator()

    pm.attrControlGrp('os_procedural_searchpath',
                   label="Procedural Search Path",
                   attribute='defaultArnoldRenderOptions.procedural_searchpath')

    pm.attrControlGrp('os_shader_searchpath',
                   label="Shader Search Path",
                   attribute='defaultArnoldRenderOptions.shader_searchpath')

    pm.attrControlGrp('os_texture_searchpath',
                   label="Texture Search Path",
                   attribute='defaultArnoldRenderOptions.texture_searchpath')

    pm.setParent('..')

    pm.setUITemplate(popTemplate=True)

def createArnoldLogSettings():

    pm.setUITemplate('attributeEditorTemplate', pushTemplate=True)
    pm.columnLayout(adjustableColumn=True)

    pm.textFieldGrp('log_filename',
                      label='Filename',
                      cc=updateLogSettings)
    pm.connectControl('log_filename', 'defaultArnoldRenderOptions.log_filename', index=1)
    pm.connectControl('log_filename', 'defaultArnoldRenderOptions.log_filename', index=2)

    '''
    pm.attrControlGrp('log_filename',
                        label="Filename",
                        attribute='defaultArnoldRenderOptions.log_filename',
                        cc=updateLogSettings)
    '''

    pm.attrControlGrp('log_max_warnings',
                        label="Max. Warnings",
                        attribute='defaultArnoldRenderOptions.log_max_warnings')

    pm.attrControlGrp('log_console_verbosity',
                        label="Console Verbosity Level",
                        attribute='defaultArnoldRenderOptions.log_console_verbosity')

    pm.attrControlGrp('log_file_verbosity',
                        label="File Verbosity Level",
                        attribute='defaultArnoldRenderOptions.log_file_verbosity')

    pm.setParent('..')

    pm.setUITemplate(popTemplate=True)


def createArnoldRendererGlobalsTab():

    # Make sure the aiOptions node exists
    core.createOptions()

    parentForm = pm.setParent(query=True)

    pm.setUITemplate('attributeEditorTemplate', pushTemplate=True)
    pm.scrollLayout('arnoldGlobalsScrollLayout', horizontalScrollBarThickness=0)
    pm.columnLayout('arnoldTabColumn', adjustableColumn=True)

    # Sampling
    #
    pm.frameLayout('arnoldSamplingSettings', label='Sampling', cll=True, cl=0)
    createArnoldSamplingSettings()
    pm.setParent('..')

    # Ray depth
    #
    pm.frameLayout('arnoldRayDepthSettings', label="Ray Depth", cll= True, cl=1)
    createArnoldRayDepthSettings()
    pm.setParent('..')

    # Environment
    #
    pm.frameLayout('arnoldEnvironmentSettings', label="Environment", cll= True, cl=1)
    createArnoldEnvironmentSettings()
    pm.setParent('..')

    # Motion Blur
    #
    pm.frameLayout('arnoldMotionBlurSettings', label="Motion Blur", cll= True, cl=1)
    createArnoldMotionBlurSettings()
    pm.setParent('..')

    # Sub-Surface Scattering
    #
    pm.frameLayout('arnoldSSSSettings', label="Sub-Surface Scattering", cll= True, cl=1)
    createArnoldSSSSettings()
    pm.setParent('..')

    # Light Linking
    #
    pm.frameLayout('arnoldLightSettings', label="Lights", cll= True, cl=1)
    createArnoldLightSettings()
    pm.setParent('..')

    # Subdivision Surfaces
    #
    pm.frameLayout('arnoldSubdivSettings', label="Subdivision", cll= True, cl=1)
    createArnoldSubdivSettings()
    pm.setParent('..')

    # Render
    #
    pm.frameLayout('arnoldRenderSettings', label="Render Settings", cll= True, cl=1)
    createArnoldRenderSettings()
    pm.setParent('..')

    # Gamma correction
    #
    pm.frameLayout('arnoldGammaSettings', label="Gamma Correction", cll=True, cl=1)
    createArnoldGammaSettings()
    pm.setParent('..')

    # Gamma correction
    #
    pm.frameLayout('arnoldTextureSettings', label="Textures", cll=True, cl=1)
    createArnoldTextureSettings()
    pm.setParent('..')

    # Overrides
    #
    pm.frameLayout('arnoldOverrideSettings', label="Feature Overrides", cll=True, cl=1)
    createArnoldOverrideSettings()
    pm.setParent('..')

    # Search path
    #
    pm.frameLayout('arnoldPathSettings', label="Search Path", cll=True, cl=1)
    createArnoldPathSettings()
    pm.setParent('..')

    # Log
    #
    pm.frameLayout('arnoldLogSettings', label="Log", cll=True, cl=1)
    createArnoldLogSettings()
    pm.setParent('..')

    pm.setParent('..')

    pm.formLayout(parentForm,
               edit=True,
               af=[('arnoldGlobalsScrollLayout', "top", 0),
                   ('arnoldGlobalsScrollLayout', "bottom", 0),
                   ('arnoldGlobalsScrollLayout', "left", 0),
                   ('arnoldGlobalsScrollLayout', "right", 0)])

    pm.setParent(parentForm)

    updateArnoldRendererGlobalsTab()

def updateBackgroundSettings(*args):
    background = getBackgroundShader()
    pm.textField('defaultArnoldRenderOptionsBackgroundTextField', edit=True, text=background)

def updateArnoldRendererGlobalsTab(*args):
    updateComputeSamples()
    updateRenderSettings()
    updateSamplingSettings()
    updateMotionBlurSettings()
    updateLogSettings()

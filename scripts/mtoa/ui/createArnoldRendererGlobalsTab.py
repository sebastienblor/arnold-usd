import maya.cmds as cmds
from mtoa.ui.callback import *
import mtoa.ui.arnoldAOVEditor as AOVEditor

def updateRenderSettings(*args):
    flag = cmds.getAttr('defaultArnoldRenderOptions.threads_autodetect') == False
    cmds.attrControlGrp('os_threads', edit=True, enable=flag)


def updateSamplingSettings(*args):
    flag = cmds.getAttr('defaultArnoldRenderOptions.use_sample_clamp') == True
    cmds.attrControlGrp('ss_max_value', edit=True, enable=flag)


def updateMotionBlurSettings(*args):

    flag = cmds.getAttr('defaultArnoldRenderOptions.motion_blur_enable') == True
    cmds.attrControlGrp('mb_camera_enable', edit=True, enable=flag)
    cmds.attrControlGrp('mb_objects_enable', edit=True, enable=flag)
    cmds.attrControlGrp('mb_object_deform_enable', edit=True, enable=flag)
    cmds.attrControlGrp('mb_lights_enable', edit=True, enable=flag)
    cmds.attrControlGrp('mb_shutter_size', edit=True, enable=flag)
    cmds.attrControlGrp('mb_shutter_offset', edit=True, enable=flag)
    cmds.attrControlGrp('mb_shutter_type', edit=True, enable=flag)
    cmds.attrControlGrp('mb_motion_steps', edit=True, enable=flag)
    cmds.attrControlGrp('mb_motion_frames', edit=True, enable=flag)


def updateLogSettings(*args):
    name = cmds.getAttr('defaultArnoldRenderOptions.log_filename')
    cmds.attrControlGrp('log_file_verbosity', edit=True, enable= name != "")

def getBackgroundShader(*args):
    conns = cmds.listConnections('defaultArnoldRenderOptions.background', s=True, d=False, p=True)
    if conns:
       return conns[0].split('.')[0]
     
    return ""

def selectBackground(*args):
    node = getBackgroundShader()
    if node:
        cmds.select(node, r=True)

def changeBackground(node, field):
    connection = cmds.listConnections('defaultArnoldRenderOptions.background')
    if connection:
        if cmds.nodeType(connection) == 'transform':
            connection = cmds.listRelatives(connection, s=True)[0]
        if connection == node:
            selectBackground()
            return 0
    cmds.connectAttr("%s.message"%node,'defaultArnoldRenderOptions.background', force=True)
    cmds.textField(field, edit=True, text=node)
    selectBackground()

def createBackground(type, field):
    bg = getBackgroundShader()
    #if bg:
        #cmds.delete(bg)
    node = cmds.shadingNode(type, asShader=True)
    changeBackground(node, field)


def deleteBackground(field):
   node = getBackgroundShader();
   if node:
      cmds.disconnectAttr("%s.message"%node, 'defaultArnoldRenderOptions.background')
      cmds.delete(node)
      cmds.textField(field, edit=True, text="")

def buildBackgroundMenu(popup, field):

    switches = cmds.ls(type='ArnoldRaySwitchShader')
    skies = cmds.ls(type='ArnoldSkyShader')

    cmds.popupMenu(popup, edit=True, deleteAllItems=True)

    for item in skies:
        cmds.menuItem(parent=popup, label=item, command=Callback(changeBackground, item, field))

    cmds.menuItem(parent=popup, divider=True)

    for item in switches:
       cmds.menuItem(parent=popup, label=item, command=Callback(changeBackground, item, field))

    cmds.menuItem(parent=popup, divider=True)

    cmds.menuItem(parent=popup, label="Create Sky Shader", command=Callback(createBackground, "ArnoldSkyShader", field))
    cmds.menuItem(parent=popup, label="Create RaySwitch Shader", command=Callback(createBackground, "ArnoldRaySwitchShader", field))

    cmds.menuItem(parent=popup, divider=True)

    cmds.menuItem(parent=popup, label="Delete", command=Callback(deleteBackground, field))

def selectAtmosphere(*args):
    bkg = cmds.getAttr('defaultArnoldRenderOptions.atmosphere')

    cases = { 1: "cmds.createNode('ArnoldFogShader', shared=True, name='defaultFogShader')",
              2: "cmds.createNode('ArnoldVolumeScatteringShader', shared=True, name='defaultVolumeScatteringShader')"
            }
   
    if bkg in cases.keys():
        eval(cases[bkg])


def createArnoldRenderSettings():

    cmds.setUITemplate('attributeEditorTemplate', pushTemplate=True)
    cmds.columnLayout('arnoldRenderLayout', adjustableColumn=True)

    cmds.checkBoxGrp('os_progressive_rendering',
                     cc=updateRenderSettings,
                     label='Progressive Rendering')

    cmds.connectControl('os_progressive_rendering', 'defaultArnoldRenderOptions.progressive_rendering', index=2)

    '''
    cmds.attrControlGrp('os_progressive_rendering',
                        label="Progressive Rendering",
                        attribute='defaultArnoldRenderOptions.progressive_rendering',
                        cc=updateRenderSettings)
    '''

    cmds.checkBoxGrp('os_threads_autodetect',
                     cc=updateRenderSettings,
                     label='Autodetect Threads')

    cmds.connectControl('os_threads_autodetect', 'defaultArnoldRenderOptions.threads_autodetect', index=2)

    '''
    cmds.attrControlGrp('os_threads_autodetect',
                   label="Autodetect Threads",
                   attribute='defaultArnoldRenderOptions.threads_autodetect',
                   cc=updateRenderSettings)
    '''

    cmds.attrControlGrp('os_threads',
                   label="Threads",
                   attribute='defaultArnoldRenderOptions.threads')

    cmds.attrControlGrp('os_bucket_scanning',
                   label="Bucket Scanning",
                   attribute='defaultArnoldRenderOptions.bucket_scanning')

    cmds.attrControlGrp('os_bucket_size',
                   label="Bucket Size",
                   attribute='defaultArnoldRenderOptions.bucket_size')


    cmds.attrControlGrp('os_clear_before_render',
                   label="Clear Before Render",
                   attribute='defaultArnoldRenderOptions.clear_before_render')


    cmds.attrControlGrp('os_abort_on_error',
                   label="Abort On Error",
                   attribute='defaultArnoldRenderOptions.abort_on_error')

    cmds.attrControlGrp('os_abort_on_license_fail',
                   label="Abort On License Fail",
                   attribute='defaultArnoldRenderOptions.abort_on_license_fail')

    cmds.separator()

    cmds.attrControlGrp('os_plugins_path',
                   label="Plug-ins Path",
                   attribute='defaultArnoldRenderOptions.plugins_path')

    cmds.separator()

    cmds.button(label="Setup AOVs", command=AOVEditor.arnoldAOVEditor)

    cmds.setParent('..')

    cmds.setUITemplate(popTemplate=True)


def updateArnoldFilterOptions(*args):

    selected_filter_type = cmds.getAttr('defaultArnoldRenderOptions.filter_type', asString=True)

    filtersA = ['box_filter',
                'catrom2d_filter',
                'catrom_filter',
                'closest_filter',
                'cubic_filter',
                'mitnet_filter',
                'video_filter']

    filtersB = ['cone_filter',
                'cook_filter',
                'disk_filter',
                'gaussian_filter',
                'sinc_filter',
                'triangle_filter']

    filtersC = ['farthest_filter']
    filtersD = ['heatmap_filter']
    filtersE = ['variance_filter']

    visSwitch = (0, 0, 0, 0)

    if selected_filter_type in filtersA:
        pass
    elif selected_filter_type in filtersB:
        visSwitch = (1, 0, 0, 0)
    elif selected_filter_type in filtersC:
        visSwitch = (0, 1, 0, 0)
    elif selected_filter_type in filtersD:
        visSwitch = (0, 0, 0, 1)
    elif selected_filter_type in filtersE:
        visSwitch = (1, 0, 1, 0)

    cmds.columnLayout('cl_filter_width', e=True,       vis=visSwitch[0])
    cmds.columnLayout('cl_filter_domain', e=True,      vis=visSwitch[1])
    cmds.columnLayout('cl_filter_scalar_mode', e=True, vis=visSwitch[2])
    cmds.columnLayout('cl_filter_minmax', e=True,      vis=visSwitch[3])

def createArnoldSamplingSettings():

    cmds.setUITemplate('attributeEditorTemplate', pushTemplate=True)
    cmds.columnLayout(adjustableColumn=True)

    cmds.attrControlGrp('ss_AA_samples',
                        label="AA Samples",
                        attribute='defaultArnoldRenderOptions.AA_samples')


    cmds.attrControlGrp('ss_hemi_samples',
                        label="Hemi Samples",
                        attribute='defaultArnoldRenderOptions.GI_diffuse_samples')

    cmds.attrControlGrp('ss_glossy_samples',
                        label="Glossy Samples",
                        attribute='defaultArnoldRenderOptions.GI_glossy_samples')

    cmds.attrControlGrp('ss_sss_hemi_samples',
                   label="SSS Samples",
                   attribute='defaultArnoldRenderOptions.GI_sss_hemi_samples')

    cmds.checkBoxGrp('ss_clamp_sample_values',
                     cc=updateSamplingSettings,
                     label='Clamp Sample Values')

    cmds.connectControl('ss_clamp_sample_values', 'defaultArnoldRenderOptions.use_sample_clamp', index=2)

    '''
    cmds.attrControlGrp('ss_clamp_sample_values',
                        label="Clamp Sample Values",
                        attribute='defaultArnoldRenderOptions.use_sample_clamp',
                        cc=updateSamplingSettings)
    '''

    cmds.attrControlGrp('ss_max_value',
                        label="Max. Value",
                        attribute='defaultArnoldRenderOptions.AA_sample_clamp')

    cmds.separator()

    cmds.attrControlGrp('ss_lock_sampling_noise',
                        label="Lock sample noise",
                        attribute='defaultArnoldRenderOptions.lock_sampling_noise')

    cmds.separator()

    cmds.rowLayout(numberOfColumns=2, columnWidth=(1, 80))
    cmds.separator(style='none')
    cmds.attrEnumOptionMenu('os_filter_type',
                               cc=updateArnoldFilterOptions,
                               attribute='defaultArnoldRenderOptions.filter_type',
                               label='Filter Type')
    cmds.setParent('..')
    '''
    cmds.attrControlGrp('os_filter_type',
                        label="Filter Type",
                        attribute='defaultArnoldRenderOptions.filter_type',
                        cc=updateArnoldFilterOptions)
    '''

    cmds.columnLayout('cl_filter_width', vis=0)
    cmds.attrControlGrp('os_filter_width',
                        label="Filter Width",
                        attribute='defaultArnoldRenderOptions.filter_width')

    cmds.setParent('..')

    cmds.columnLayout('cl_filter_domain', vis=0)
    cmds.attrControlGrp('os_filter_domain',
                         label="Filter Domain",
                         attribute='defaultArnoldRenderOptions.filter_domain')

    cmds.setParent('..')

    cmds.columnLayout('cl_filter_scalar_mode', vis=0)
    cmds.attrControlGrp('os_filter_scalar_mode',
                        label="Filter Scalar Mode",
                        attribute='defaultArnoldRenderOptions.filter_scalar_mode')

    cmds.setParent('..')

    cmds.columnLayout('cl_filter_minmax', vis=0)
    cmds.attrControlGrp('os_filter_minimum',
                        label="Filter Minimum",
                        attribute='defaultArnoldRenderOptions.filter_minimum')

    cmds.attrControlGrp('os_filter_maximum',
                        label="Filter Maximum",
                        attribute='defaultArnoldRenderOptions.filter_maximum')

    cmds.setParent('..')

    cmds.setParent('..')

    cmds.setUITemplate(popTemplate=True)
    updateArnoldFilterOptions()


def createArnoldGammaSettings():

    cmds.setUITemplate('attributeEditorTemplate', pushTemplate=True)
    cmds.columnLayout(adjustableColumn=True)

    cmds.attrControlGrp('ss_driver_gamma',
                        label="Display Driver gamma",
                        attribute='defaultArnoldRenderOptions.driver_gamma')

    cmds.separator()

    cmds.attrControlGrp('ss_light_gamma',
                        label="Lights",
                        attribute='defaultArnoldRenderOptions.light_gamma')

    cmds.attrControlGrp('ss_shader_gamma',
                   label="Shaders",
                   attribute='defaultArnoldRenderOptions.shader_gamma')

    cmds.attrControlGrp('ss_texture_gamma',
                        label="Textures",
                        attribute='defaultArnoldRenderOptions.texture_gamma')

    cmds.setParent('..')

    cmds.setUITemplate(popTemplate=True)


def createArnoldRayDepthSettings():

    cmds.setUITemplate('attributeEditorTemplate', pushTemplate=True)
    cmds.columnLayout(adjustableColumn=True)

    cmds.attrControlGrp('rs_total_depth',
                        label="Total depth",
                        attribute='defaultArnoldRenderOptions.GI_total_depth')

    cmds.separator(style="none")

    cmds.attrControlGrp('rs_diffuse_depth',
                        label="Diffuse depth",
                        attribute='defaultArnoldRenderOptions.GI_diffuse_depth')

    cmds.attrControlGrp('rs_glossy_depth',
                        label="Glossy depth",
                        attribute='defaultArnoldRenderOptions.GI_glossy_depth')

    cmds.attrControlGrp('rs_reflection_depth',
                        label="Reflection depth",
                        attribute='defaultArnoldRenderOptions.GI_reflection_depth')

    cmds.attrControlGrp('rs_refraction_depth',
                        label="Refraction depth",
                        attribute='defaultArnoldRenderOptions.GI_refraction_depth')

    cmds.separator(style="none")

    cmds.attrControlGrp('rs_auto_transparency_depth',
                        label="Auto transp. depth",
                        attribute='defaultArnoldRenderOptions.auto_transparency_depth')

    cmds.attrControlGrp('rs_auto_transparency_threshold',
                        label="Auto transp. threshold",
                        attribute='defaultArnoldRenderOptions.auto_transparency_threshold')

    cmds.setParent('..')

    cmds.setUITemplate(popTemplate=True)

def createArnoldEnvironmentSettings():

    cmds.setUITemplate('attributeEditorTemplate', pushTemplate=True)
    cmds.columnLayout(adjustableColumn=True)
    
    cmds.rowLayout(adjustableColumn=2, numberOfColumns=3)
    cmds.text(label="Background")
    bgfield = cmds.textField(editable=False)
    bgpopup = cmds.popupMenu(parent=bgfield)
    cmds.popupMenu(bgpopup, edit=True, postMenuCommand=Callback(buildBackgroundMenu, bgpopup, bgfield))
    cmds.button(label="Select", height=22, width=50, command=selectBackground)
    cmds.setParent('..')

    conns = cmds.listConnections('defaultArnoldRenderOptions.background', s=True, d=False)
    if conns:
       cmds.textField(bgfield, edit=True, text=conns[0])

    cmds.separator(style="none")

    cmds.rowLayout(numberOfColumns=2, columnWidth=(1, 80))
    cmds.separator(style="none")
    cmds.attrEnumOptionMenu('es_atmosphere',
                            label = 'Atmosphere',
                            attribute='defaultArnoldRenderOptions.atmosphere',
                            cc=selectAtmosphere)
    cmds.setParent('..')
    cmds.setParent('..')

    cmds.setUITemplate(popTemplate=True)

def createArnoldMotionBlurSettings():

    cmds.setUITemplate('attributeEditorTemplate', pushTemplate=True)
    cmds.columnLayout(adjustableColumn=True)

    cmds.checkBoxGrp('mb_enable',
                     cc=updateMotionBlurSettings,
                     label='Enable')

    cmds.connectControl('mb_enable', 'defaultArnoldRenderOptions.motion_blur_enable', index=2)

    '''
    cmds.attrControlGrp('mb_enable',
                        label="Enable",
                        attribute='defaultArnoldRenderOptions.motion_blur_enable',
                        cc=updateMotionBlurSettings)
    '''

    cmds.attrControlGrp('mb_lights_enable',
                        label="Lights",
                        attribute='defaultArnoldRenderOptions.mb_lights_enable')

    cmds.attrControlGrp('mb_camera_enable',
                        label="Camera",
                        attribute='defaultArnoldRenderOptions.mb_camera_enable')

    cmds.attrControlGrp('mb_objects_enable',
                        label="Objects",
                        attribute='defaultArnoldRenderOptions.mb_objects_enable')

    cmds.attrControlGrp('mb_object_deform_enable',
                        label="Object deformation",
                        attribute='defaultArnoldRenderOptions.mb_object_deform_enable')

    cmds.separator()

    cmds.attrControlGrp('mb_shutter_size',
                        label="Shutter Size",
                        attribute='defaultArnoldRenderOptions.shutter_size')

    cmds.attrControlGrp('mb_shutter_offset',
                        label="Shutter Offset",
                        attribute='defaultArnoldRenderOptions.shutter_offset')

    cmds.attrControlGrp('mb_shutter_type',
                        label="Shutter Type",
                        attribute='defaultArnoldRenderOptions.shutter_type')

    cmds.attrControlGrp('mb_motion_steps',
                        label="Motion Steps",
                        attribute='defaultArnoldRenderOptions.motion_steps')

    cmds.attrControlGrp('mb_motion_frames',
                        label="Blur By (Frames)",
                        attribute='defaultArnoldRenderOptions.motion_frames')

    cmds.setParent('..')

    cmds.setUITemplate(popTemplate=True)


def createArnoldSSSSettings():

    cmds.setUITemplate('attributeEditorTemplate', pushTemplate=True)
    cmds.columnLayout(adjustableColumn=True)

    '''
    cmds.attrControlGrp('mb_sss_lazy_evaluation',
                        label="SSS lazy evaluation",
                        attribute='defaultArnoldRenderOptions.sss_lazy_evaluation')
    '''

    cmds.attrControlGrp('mb_sss_subpixel_cache',
                        label="SSS subpixel cache",
                        attribute='defaultArnoldRenderOptions.sss_subpixel_cache')

    cmds.attrControlGrp('mb_show_samples',
                        label="Show samples",
                        attribute='defaultArnoldRenderOptions.show_samples')

    cmds.setParent('..')

    cmds.setUITemplate(popTemplate=True)


def createArnoldSubdivSettings():

    cmds.setUITemplate('attributeEditorTemplate', pushTemplate=True)
    cmds.columnLayout(adjustableColumn=True)

    cmds.attrControlGrp('sub_max_subdivisions',
                        label="Max. Subdivisions",
                        attribute='defaultArnoldRenderOptions.max_subdivisions')

    cmds.setParent('..')

    cmds.setUITemplate(popTemplate=True)


def createArnoldTextureSettings():

    cmds.setUITemplate('attributeEditorTemplate', pushTemplate=True)
    cmds.columnLayout(adjustableColumn=True)

    cmds.attrControlGrp('texture_automip',
                        label="Auto Mipmap",
                        attribute='defaultArnoldRenderOptions.texture_automip')

    cmds.attrControlGrp('texture_autotile',
                        label="Auto Tile Size",
                        attribute='defaultArnoldRenderOptions.texture_autotile')

    cmds.attrControlGrp('texture_max_memory_MB',
                        label="Max Cache Size (MB)",
                        attribute='defaultArnoldRenderOptions.texture_max_memory_MB')

    cmds.setParent('..')

    cmds.setUITemplate(popTemplate=True)

def createArnoldLogSettings():

    cmds.setUITemplate('attributeEditorTemplate', pushTemplate=True)
    cmds.columnLayout(adjustableColumn=True)

    cmds.textFieldGrp('log_filename',
                      label='Filename',
                      cc=updateLogSettings)
    cmds.connectControl('log_filename', 'defaultArnoldRenderOptions.log_filename', index=2)

    '''
    cmds.attrControlGrp('log_filename',
                        label="Filename",
                        attribute='defaultArnoldRenderOptions.log_filename',
                        cc=updateLogSettings)
    '''

    cmds.attrControlGrp('log_max_warnings',
                        label="Max. Warnings",
                        attribute='defaultArnoldRenderOptions.log_max_warnings')

    cmds.attrControlGrp('log_console_verbosity',
                        label="Console Verbosity Level",
                        attribute='defaultArnoldRenderOptions.log_console_verbosity')

    cmds.attrControlGrp('log_file_verbosity',
                        label="File Verbosity Level",
                        attribute='defaultArnoldRenderOptions.log_file_verbosity')

    cmds.setParent('..')

    cmds.setUITemplate(popTemplate=True)


def createArnoldRendererGlobalsTab():

    # Make sure the ArnoldRenderOptions node exists
    cmds.createNode('ArnoldRenderOptions', skipSelect=True, shared=True, name='defaultArnoldRenderOptions')

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
    cmds.frameLayout('arnoldRayDepthSettings', label="Ray Depth", cll= True, cl=0)
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

    # Subsurface scattering
    #
    cmds.frameLayout('arnoldSSSSettings', label="Subsurface scattering", cll= True, cl=1)
    createArnoldSSSSettings()
    cmds.setParent('..')

    # Subdivision Surfaces
    #
    cmds.frameLayout('arnoldSubdivSettings', label="Subdivision", cll= True, cl=1)
    createArnoldSubdivSettings()
    cmds.setParent('..')

    # Render
    #
    cmds.frameLayout('arnoldRenderSettings', label="Render Settings", cll= True, cl=1)
    createArnoldRenderSettings()
    cmds.setParent('..')

    # Gamma correction
    #
    cmds.frameLayout('arnoldGammaSettings', label="Gamma Correction", cll=True, cl=1)
    createArnoldGammaSettings()
    cmds.setParent('..')

    # Gamma correction
    #
    cmds.frameLayout('arnoldTextureSettings', label="Textures", cll=True, cl=1)
    createArnoldTextureSettings()
    cmds.setParent('..')

    # Log
    #
    cmds.frameLayout('arnoldLogSettings', label="Log", cll=True, cl=1)
    createArnoldLogSettings()
    cmds.setParent('..')

    cmds.setParent('..')

    cmds.formLayout(parentForm,
               edit=True,
               af=[('arnoldGlobalsScrollLayout', "top", 0),
                   ('arnoldGlobalsScrollLayout', "bottom", 0),
                   ('arnoldGlobalsScrollLayout', "left", 0),
                   ('arnoldGlobalsScrollLayout', "right", 0)])

    cmds.setParent(parentForm)

    updateArnoldRendererGlobalsTab()


def updateArnoldRendererGlobalsTab(*args):
    updateRenderSettings()
    updateSamplingSettings()
    updateMotionBlurSettings()
    updateLogSettings()


import tempfile
import datetime
from functools import partial
import maya.cmds as cmds         
import maya.mel as mel         
import maya.utils as utils     

import mtoa.aovs as aovs
import mtoa.core as core

TMP_DIR = tempfile.gettempdir()
CROP_CTX = 'rmanViewportContext1'
SHOW_GEO_HELP = 'Show VP2 geometry in snapshot'
SMOOTH_GEO_HELP = 'Anti-alias VP2 geometry in snapshot'
PROGRESS_BAR_HELP = 'Display the progress bar'
SHOW_PROGRESS_BAR_HELP = 'Display the progress bar in snapshots'

g_debug_mode = {}

def playblast(arnold_panel, show_ui=True):
    pass

def do_playblast(arnold_panel, options, stop_when_done):
    prog = cmds.rman('progress')
    if prog < 0:
        utils.executeDeferred(partial(do_playblast, arnold_panel, options,
                                       stop_when_done))
        return

    # force a refresh on the viewport to make sure the playblast will appear
    # in that panel.
    cmds.setFocus(arnold_panel)
    cmds.refresh(currentView=True, force=True)

    # launch the playblast
    kwargs = options.options()
    # for kw, val in kwargs.iteritems():
    #     print '%20s: %s' % (kw, val)
    imgs = cmds.playblast(**kwargs)

    if stop_when_done:
        stop(arnold_panel)

    # restore progress bar state
    if cmds.optionVar(q='rfmPrePlayblastProgressBarState'):
        set_enable_progress_bar(1)
    # restore in-view messages
    if cmds.optionVar(q='rfmPrePlayblastInviewMsgState'):
        cmds.optionVar(iv=('inViewMessageEnable', True))

    options.view(imgs)
    del options

    # rfm_log().info('RenderMan playblast: %s', imgs)

def snapshot(arnold_panel):
    """Take a snapshot of the current viewport and send it to It.

    Arguments:
        arnold_panel {str} -- Neded to make sure it has focus before saving.
    """
    if not arnold_panel or not cmds.modelEditor(arnold_panel, ex=True):
        return

    cmds.setFocus(arnold_panel)

    multi_sample_enable = cmds.getAttr('hardwareRenderingGlobals.multiSampleEnable')
    multi_sample_count = cmds.getAttr('hardwareRenderingGlobals.multiSampleCount')
    line_aa_enable = cmds.getAttr('hardwareRenderingGlobals.lineAAEnable')
    renderer = cmds.modelEditor(arnold_panel, q=True, rom=True)
    re_enable_pbar = False
    # if prefs.get_pref_by_name('rfmVpProgressBar') and not get_show_progress_bar():
    #     prefs.set_pref_by_name('rfmVpProgressBar', 0)
    #     re_enable_pbar = True
    if renderer == 'RenderManViewport' and not get_show_geometry():
        cmds.modelEditor(arnold_panel, e=True, allObjects=False)
    if get_smooth_geometry():
        cmds.setAttr('hardwareRenderingGlobals.multiSampleCount', 8)
        cmds.setAttr('hardwareRenderingGlobals.multiSampleEnable', 1)
        cmds.setAttr('hardwareRenderingGlobals.lineAAEnable', 1)

    # build the file path
    # this_scene = maya_scene_name(short_name=True).split('.')[0]
    this_scene = "foo"
    # this_frame = cmds.currentTime(q=True)
    t_stamp = datetime.datetime.now().strftime('%Y-%m-%d__%Hh%Mm%Ss')
    tmp_file = TMP_DIR.join('%s__snap_%s.iff' % (this_scene, t_stamp))

    # take a snapshot
    cmds.refresh(cv=True, fileExtension='iff', filename=tmp_file)

    # send image to it
    # rfm2.render.show_it(tmp_file)

    # rfm_log().info('Snapshot: %s', tmp_file)

    # if re_enable_pbar:
    #     prefs.set_pref_by_name('rfmVpProgressBar', 1)
    # if renderer == 'RenderManViewport' and not get_show_geometry():
    #     cmds.modelEditor(arnold_panel, e=True, allObjects=True)
    # if get_smooth_geometry():
    #     cmds.setAttr('hardwareRenderingGlobals.multiSampleCount', multi_sample_count)
    #     cmds.setAttr('hardwareRenderingGlobals.lineAAEnable', line_aa_enable)
    #     cmds.setAttr('hardwareRenderingGlobals.multiSampleEnable', multi_sample_enable)



def toggle_render(arnold_panel, state, update_buttons=True):
    """Start rendering in a given panel

    Arguments:
        arnold_panel {str} -- the panel we should start/stop rendering in.
        state {bool} -- The toggle state
    """
    mel.eval('if (`currentRenderer` != "arnold") '
             'setCurrentRenderer("arnold");')
    print("viewport.toggle : {}".format("on" if state else "off"))
    toggle_ctl = 'avp_toggle_%s' % arnold_panel
    if not cmds.iconTextCheckBox(toggle_ctl, ex=True):
        return
    if not arnold_panel or not cmds.modelEditor(arnold_panel, ex=True):
        return
    # start / stop the renderer
    # ipr_mode = "1" if state else "0"
    # cmds.arnoldViewOverrideOptionBox(opt=("Run IPR", ipr_mode))
    if state:
        # check if the current panel is the same as the given panel
        # if not stop rendering on the current panel before starting on this panel
        current_arnold_panel = get_arnold_panel()
        if current_arnold_panel != arnold_panel:
            update_button_state("avp_crop", current_arnold_panel, False)
            update_button_state("avp_toggle", current_arnold_panel, False)
            update_icon_bar_enable_state(current_arnold_panel, False)

        # check if crop is enabled, if so stop any renders and disable crop
        if get_option_state("crop", arnold_panel):
            toggle_crop(arnold_panel,False)

        cmds.modelEditor(arnold_panel, e=True,
                       rendererOverrideName='arnoldViewOverride')
        cmds.evalDeferred("cmds.arnoldViewOverrideOptionBox(opt=(\"Run IPR\", \"1\"))", lp=True)
    else:
        stop(arnold_panel)


    if cmds.iconTextCheckBox(toggle_ctl, ex=True):
        bstate = cmds.iconTextCheckBox(toggle_ctl, q=True, v=True)
        if bstate != state:
            cmds.iconTextCheckBox(toggle_ctl, e=True, v=state)

    if update_buttons:
        update_icon_bar_enable_state(arnold_panel, state)
    cmds.refresh(cv=True, force=True)


def stop(arnold_panel):
    """Stop viewport rendering.

    Arguments:
        arnold_panel {str} -- The panel that should be stopped.
    """
    if cmds.arnoldViewOverrideOptionBox(get="Run IPR") == "1":
        # the priority for the IPR command is very important, any calls to stop IPR must be ran with the lowest priority
        cmds.evalDeferred("cmds.arnoldViewOverrideOptionBox(opt=(\"Run IPR\", \"0\"))", lp=True)

    if arnold_panel:
        cmds.setFocus(arnold_panel)
        cmds.ActivateViewport20()


def restart(arnold_panel):
    cmds.arnoldViewOverrideOptionBox(opt=("Refresh Render", "1"))    

def toggle_crop(arnold_panel, state):
    """Enable/Disable the viewport's crop controls.

    Arguments:
        arnold_panel {str} -- The current panel
        state {bool} -- active or not
    """
    # get the current IPR state

    crop_mode = "1" if state else "0"

    if not state:
        cmds.evalDeferred("cmds.arnoldViewOverrideOptionBox(opt=(\"Run IPR\", \"0\"))", low=True)
        cmds.evalDeferred("cmds.arnoldViewOverrideOptionBox(opt=(\"Crop Region\", \"0\"))", low=True)
        cmds.setToolTo("selectSuperContext")
    else:
        # check if the current panel is the same as the given panel
        # if not stop rendering on the current panel before starting on this panel
        current_arnold_panel = get_arnold_panel()
        if current_arnold_panel != arnold_panel:
            update_button_state("avp_crop", current_arnold_panel, False)
            update_button_state("avp_toggle", current_arnold_panel, False)
            update_icon_bar_enable_state(current_arnold_panel, False)

        # check if the current view is enabled for full frame, and disable the icon if it is 
        update_button_state("avp_toggle", arnold_panel, False)

        cmds.modelEditor(arnold_panel, e=True,
                       rendererOverrideName='arnoldViewOverride')
        cmds.arnoldViewportRegionToolContext()
        cmds.setToolTo("arnoldViewportRegionToolContext1")
        cmds.evalDeferred("cmds.arnoldViewOverrideOptionBox(opt=(\"Crop Region\", \"1\"))", low=True)
        if get_crop_region() != (0,0,0,0):
            print(get_crop_region())
            cmds.evalDeferred("cmds.arnoldViewOverrideOptionBox(opt=(\"Run IPR\", \"1\"))", lp=True)


    update_icon_bar_enable_state(arnold_panel, state)

    # make sure the icon state reflects the actual tool state.
    update_button_state("avp_crop", arnold_panel, state)

def get_crop_region():
    defaultOptionsNode = 'defaultArnoldRenderOptions'
    if not cmds.objExists(defaultOptionsNode):
        core.createOptions()
    left = cmds.getAttr("{}.avpRegionLeft".format(defaultOptionsNode))
    right = cmds.getAttr("{}.avpRegionRight".format(defaultOptionsNode))
    bottom = cmds.getAttr("{}.avpRegionBottom".format(defaultOptionsNode))
    top = cmds.getAttr("{}.avpRegionTop".format(defaultOptionsNode))

    return left,right,bottom,top

def get_option_state(option, arnold_panel):
    option_ctl = 'avp_{}_{}'.format(option,arnold_panel)
    print("[get_option_state] Option {} exists {}".format(option_ctl, cmds.iconTextCheckBox(option_ctl, ex=True)))
    if cmds.iconTextCheckBox(option_ctl, ex=True):
        print("[get_option_state] Option {} is {}".format(option_ctl, cmds.iconTextCheckBox(option_ctl, q=True, value=True)))
        return cmds.iconTextCheckBox(option_ctl, q=True, value=True)
    return False

def update_button_state(control_name, arnold_panel, state):
    icn_name = '{}_{}'.format(control_name, arnold_panel)
    print(icn_name)
    if cmds.iconTextCheckBox(icn_name, ex=True):
        print("{} : {} {}".format(icn_name, cmds.iconTextCheckBox(icn_name, q=True, v=True), state))
    if cmds.iconTextCheckBox(icn_name, ex=True) and cmds.iconTextCheckBox(icn_name, q=True, v=True) != state:
            cmds.iconTextCheckBox(icn_name, e=True, v=state)

def update_icon_bar_enable_state(arnold_panel, state):
    """Some buttons should be disabled until a render takes place, i.e.
    crop and restart.

    Arguments:
        arnold_panel {str} -- The current panel
        state {boo} -- Enabled or not
    """

    restart_ctl = 'avp_restart_%s' % arnold_panel
    if cmds.iconTextButton(restart_ctl, ex=True):
        cmds.iconTextButton(restart_ctl, e=True, en=state)

    debug_ctl = 'avp_debug_%s' % arnold_panel
    if cmds.iconTextCheckBox(debug_ctl, ex=True):
        cmds.iconTextCheckBox(debug_ctl, e=True, en=state)

    aov_ctl = 'avp_aov_%s' % arnold_panel
    if cmds.iconTextButton(aov_ctl, ex=True):
        cmds.iconTextButton(aov_ctl, e=True, en=state)

    res_ctl = 'avp_resolution_%s' % arnold_panel
    if cmds.iconTextButton(res_ctl, ex=True):
        cmds.iconTextButton(res_ctl, e=True, en=state)

def get_arnold_panel():
    """Return the name of panel containg a rendering viewport.
    None if there is no panel currently rendering.
    """
    # find renderman panel
    panel_list = cmds.getPanel(type='modelPanel')
    arnold_panel = None
    for panel in panel_list:
        if cmds.modelEditor(
                panel,
                q=True,
                rendererOverrideName=True) == 'arnoldViewOverride':
            arnold_panel = panel
            break
    print('viewport.get_arnold_panel = {}'.format(arnold_panel))
    return arnold_panel


def get_arnold_panel_iconbar(panel=None):
    """Returns the icon bar layout.

    Keyword Arguments:
        panel {str} -- the panel of interest. If not defined we call
        get_arnold_panel() to get the currently rendering panel (default: {None})

    Returns:
        None -- If we could not find the layout
        (arnold_panel, iconbar_layout) -- If successful.
    """
    arnold_panel = panel if panel else get_arnold_panel()
    if not arnold_panel:
        return None
    # print ' + arnold_panel = %r' % arnold_panel
    ctl = cmds.modelEditor(arnold_panel, q=True, ctl=True)
    # print ' + ctl = %r' % ctl
    form = cmds.control(ctl, q=True, parent=True)
    # print ' + form = %r' % form
    kids = cmds.layout(form, q=True, ca=True)
    # print ' + kids = %r' % kids
    icb = '%s|modelEditorIconBar' % form
    if not cmds.control(icb, ex=True):
        # 2019 has a slightly different location of its icon bars,
        # so make another attempt
        form2 = cmds.layout(form, q=True, parent=True)
        icb = '%s|modelEditorIconBar' % form2
        if not cmds.control(icb, ex=True):
            return None
    kids = cmds.layout(icb, q=True, ca=True)
    if kids and 'flowLayout' in kids[0]:
        return arnold_panel, kids[0]
    return None


def setup_icon_bar(iconbar, panel, uip):
    """Insert our controls in the icon bar of a given panel.

    Arguments:
        iconbar {str} -- the layout where we will add our controls
        panel {str} -- the panel this icon bar belongs to.
        uip {str} -- the original ui parent we must restore when we are done.
        active {bool} -- Is this panel active or not.
    """
    # check if the controls are already here
    kids = cmds.layout(iconbar, q=True, ca=True)
    btn_lyt = 'arnold_vp_ctls_%s' % panel
    if not kids or btn_lyt in kids:
        return

    # Add separator icon
    melcmd = '''modelPanelBarToggleButtonCallback("%s", %d)''' % (iconbar, 0)
    cmds.iconTextButton(width=9, sourceType='mel', command=melcmd,
                      i1='ShortOpenBar.png', p=iconbar)
    # Add form with our buttons
    ctlform = cmds.formLayout(btn_lyt, p=iconbar)
    # start/stop
    toggle_render_btn = cmds.iconTextCheckBox('avp_toggle_%s' % panel, w=18, h=18,
                            #   hlc=(0.9, 0.9, 0.9),
                              i='avp_toggle.svg',
                              ann='Start/Stop Arnold in the viewport',
                              v=False, p=ctlform)
    # cmds.popupMenu('avp_toggle_menu_%s' % panel, parent=toggle_render_btn, button=3,
    #              pmc=partial(build_display_options_menu))
    cmds.iconTextCheckBox(toggle_render_btn, e=True,
                        cc=partial(toggle_render, panel))
    # # playblast
    # bt2 = cmds.iconTextButton('arnold_playblast_%s' % panel, w=18, h=18, en=True,
    #                         i=':/rm_vp_playblast.svg',
    #                         ann='Render a playblast of this panel',
    #                         c=partial(playblast, panel))

    # crop window
    crop_btn = cmds.iconTextCheckBox('avp_crop_%s' % panel, w=18, h=18, en=True,
                            #   hlc=(0.8, 0.2, 0.0),
                              i='avp_crop.svg',
                              ann='Define a crop window',
                              v=False, p=ctlform)
    cmds.iconTextCheckBox(crop_btn, e=True,
                        cc=partial(toggle_crop, panel))

    # resolution
    res_btn = cmds.iconTextButton('avp_resolution_%s' % panel, w=18, h=18, en=False,
                            i='avp_resolution.svg',
                            ann="Set the viewport's render resolution",
                            p=ctlform)
    cmds.popupMenu('avp_resolution_menu_%s' % panel, parent=res_btn, button=1,
                 pmc=partial(build_resolution_menu, panel))

    # debug shading
    debug_btn = cmds.iconTextCheckBox('avp_debug_%s' % panel, w=18, h=18, en=False,
                              i='avp_debug.svg',
                              ann='Set shading to debug mode',
                              v=False, p=ctlform,
                              cc=partial(toggle_debug, panel))
    cmds.popupMenu('avp_debug_menu_%s' % panel, parent=debug_btn, button=3,
                 pmc=partial(build_debug_menu, panel))

    # restart
    # res_btn = cmds.iconTextButton('avp_restart_%s' % panel, w=18, h=18, en=False,
    #                         i='avp_refresh.svg',
    #                         c=partial(restart, panel),
    #                         ann='Restart the viewport render', p=ctlform)

    # AOVs
    aov_btn = cmds.iconTextButton('avp_aov_%s' % panel, w=18, h=18, en=False,
                            i='avp_aovs.svg',
                            ann="Select display channels",
                            p=ctlform)
    cmds.popupMenu('avp_aov_menu_%s' % panel, parent=aov_btn, button=1, pmc=update_aov_menu)

    # # snapshot
    # bt8 = cmds.iconTextButton('arnold_vp_snapshot_%s' % panel, w=18, h=18,
    #                         i='rm_vp_snapshot.svg',
    #                         c=partial(snapshot, panel),
    #                         ann='Save a snapshot to It', p=ctlform)
    # cmds.popupMenu('arnold_vp_snapshot_menu_%s' % panel, parent=bt8, button=3,
    #              pmc=partial(build_snapshot_menu))

    cmds.formLayout(ctlform, e=True,
                  af=[(toggle_render_btn, 'top', 0)],
                  ac=[(crop_btn, 'left', 1, toggle_render_btn),
                      (res_btn, 'left', 1, crop_btn),
                      (debug_btn, 'left', 1, res_btn),
                      (aov_btn, 'left', 1, debug_btn)])
    # repack the layout
    tmp = cmds.formLayout(p=iconbar, m=False)
    for k in kids:
        cmds.control(k, e=True, p=tmp)
    cmdstr = '''modelPanelBarToggleButtonCallback("%s", %d)'''
    for i, k in enumerate(kids):
        ctl = cmds.control(k, e=True, p=iconbar)
        if cmds.iconTextButton(ctl, ex=True):
            cmds.iconTextButton(ctl, e=True, sourceType='mel',
                              c=cmdstr % (iconbar, i + 2))
    cmds.deleteUI(tmp)
    # restore original parent
    cmds.setParent(uip)


def build_resolution_menu(*args):
    """Build the resolution menu when the user clicks the button.

    Arguments:
        args[0] {str}: the current panel.
        args[1] {str}: the parent control.
    """
    panel = args[0]
    parent = args[1]

    testres = cmds.arnoldViewOverrideOptionBox(get="Test Resolution") or "100%"

    cmds.popupMenu(parent, e=True, deleteAllItems=True)
    cmds.radioMenuItemCollection(p=parent)
    for res in ["100%", "75%", "50%", "25%", "10%"]:
        cmds.menuItem(l=res, p=parent, rb=(testres == res),
                    c=partial(change_resolution, res, panel))


def change_resolution(testres, arnold_panel, *args):
    """Store the new resolution multiplier and restart the render if it was
    already running.

    Arguments:
        args[0] {float}: the new resolution multiplier
        args[1] {str}: the current panel
    """
    current_arnold_panel = get_arnold_panel()
    # if resolution is set from another viewport, ignore that and warn.
    if current_arnold_panel and current_arnold_panel != arnold_panel:
        cmds.warning('Ignored new resolution: there is already one active'
                          ' Arnold viewport (%s).', current_arnold_panel)
        return
    cmds.arnoldViewOverrideOptionBox(option=("Test Resolution", testres))

def set_show_geometry(*args):
    # cmds.optionVar(iv=('rfmVpShowGeo', args[0]))
    pass


def set_smooth_geometry(*args):
    # cmds.optionVar(iv=('rfmVpSmoothGeo', args[0]))
    pass


def set_show_progress_bar(*args):
    # cmds.optionVar(iv=('rfmVpShowProgressBar', args[0]))
    pass


def get_show_geometry():
    # if cmds.optionVar(ex='rfmVpShowGeo'):
    #     return cmds.optionVar(q='rfmVpShowGeo')
    return 0


def get_smooth_geometry():
    # if cmds.optionVar(ex='rfmVpSmoothGeo'):
    #     return cmds.optionVar(q='rfmVpSmoothGeo')
    return 1


def get_show_progress_bar():
    # if cmds.optionVar(ex='rfmVpShowProgressBar'):
    #     return cmds.optionVar(q='rfmVpShowProgressBar')
    return 1


def get_enable_progress_bar():
    # if cmds.optionVar(ex='rfmVpProgressBar'):
    #     return cmds.optionVar(q='rfmVpProgressBar')
    return 1


def set_enable_progress_bar(*args):
    # cmds.optionVar(iv=('rfmVpProgressBar', args[0]))
    pass


def build_snapshot_menu(*args):
    parent = args[0]
    show_geo = get_show_geometry()
    smooth = get_smooth_geometry()
    progress = get_show_progress_bar()
    cmds.popupMenu(parent, e=True, deleteAllItems=True)
    cmds.menuItem(l="Show geometry", p=parent, cb=(show_geo == 1),
                c=partial(set_show_geometry), ann=SHOW_GEO_HELP)
    cmds.menuItem(l="Smooth geometry", p=parent, cb=(smooth == 1),
                c=partial(set_smooth_geometry), ann=SMOOTH_GEO_HELP)
    cmds.menuItem(l="Show Progress bar", p=parent, cb=(progress == 1),
                c=partial(set_show_progress_bar), ann=SHOW_PROGRESS_BAR_HELP)


def build_display_options_menu(*args):
    parent = args[0]
    progress_bar = get_enable_progress_bar()
    cmds.popupMenu(parent, e=True, deleteAllItems=True)
    cmds.menuItem(l="Progress bar", p=parent, cb=(progress_bar == 1),
                c=partial(set_enable_progress_bar), ann=PROGRESS_BAR_HELP)


def build_debug_menu(*args):
    def _set_style(*args):
        cmds.setAttr('PxrVisualizer.style', args[0], type='string')

    panel = args[0]
    parent = args[1]

    global g_debug_mode
   
    if panel not in g_debug_mode:
        g_debug_mode[panel] = cmds.arnoldViewOverrideOptionBox(get="Debug Shading") or "Basic"

    modes = ["Basic","Occlusion",
             "Wireframe","Normal",
             "UV","Primitive ID",
             "Object",
             "Barycentric",
             "Lighting"]

    cmds.popupMenu(parent, e=True, deleteAllItems=True)
    k_collection_name = parent.split('|')[-1] + '_collection'
    cmds.radioMenuItemCollection(k_collection_name, p=parent)
    for mode in modes:
        cmds.menuItem(l=mode, p=parent, rb=(g_debug_mode[panel] == mode),
                    collection=k_collection_name, c=partial(change_debug_mode, mode, panel))
        # if mode == modes[0]:
        #     cmds.menuItem(divider=True, p=parent, collection=k_collection_name)

def change_debug_mode(mode, arnold_panel, *args):
    """Store the new resolution multiplier and restart the render if it was
    already running.

    Arguments:
        args[0] {float}: the new resolution multiplier
        args[1] {str}: the current panel
    """
    global g_debug_mode
    g_debug_mode[arnold_panel] = mode
    # cmds.arnoldViewOverrideOptionBox(option=("Debug Shading", mode))
    toggle_debug(arnold_panel, True)

def toggle_debug(arnold_panel, state):
    global g_debug_mode

    if arnold_panel not in g_debug_mode:
        g_debug_mode[arnold_panel] = "Basic"
    
    if not state:
        cmds.arnoldViewOverrideOptionBox(option=("Debug Shading", "Disabled"))
    else:
        cmds.arnoldViewOverrideOptionBox(option=("Debug Shading", g_debug_mode[arnold_panel]))

    update_button_state("avp_debug", arnold_panel, state)


def build_aov_menu(*args):

    def set_avp_aov(name, index, *_args):
        cmds.arnoldViewOverrideOptionBox(opt=("AOVs", name))

    current_aov_name = cmds.arnoldViewOverrideOptionBox(get="AOVs")
    parent = args[0]

    # delete menu items and setup the collection
    cmds.popupMenu(parent, e=True, deleteAllItems=True)
    k_collection_name = parent.split('|')[-1] + '_collection'
    cmds.radioMenuItemCollection(k_collection_name, p=parent)

    cmds.menuItem(l="Beauty", p=parent, c=partial(set_avp_aov, "Beauty", 0),
                collection=k_collection_name, rb=("Beauty" == current_aov_name))
    cmds.menuItem(divider=True, p=parent, collection=k_collection_name)
    # Get list of AOVs
    aov_inter  = aovs.AOVInterface()
    for i,aov in enumerate(aov_inter.getAOVs(group=False, sort=True, enabled=None, include=None, exclude=None)):
        cmds.menuItem(l=aov.name, p=parent, c=partial(set_avp_aov, aov.name, i+1),
                    collection=k_collection_name, rb=(aov.name == current_aov_name))

def update_aov_menu(*args):
    """Called every time the viewport's AOV menu needs to be displayed."""
    rendering_panel = get_arnold_panel()
    if rendering_panel:
        build_aov_menu(*args)

def add_ctls_to_all_panels():
    """List all panels and setup their icon bar.
    """
    uip = cmds.setParent(q=True)
    arnold_panel = get_arnold_panel()
    panel_list = cmds.getPanel(type='modelPanel')
    if not panel_list:
        return
    for panel in panel_list:
        try:
            _, iconbar = get_arnold_panel_iconbar(panel=panel)
        except TypeError:
            pass
        else:
            setup_icon_bar(iconbar, panel, uip)


def add_controls():
    """This is called at startup. Any prefs change requires restarting Maya.
    """
    if cmds.about(batch=True):
        return

    # if prefs.get_pref_by_name('rfmEnableVpControls'):
    add_ctls_to_all_panels()


# def info_string():
#     apistrings.set_var('integrator', apinodes.get_integrator())
#     return apistrings.expand_string(prefs.get_pref_by_name('rfmVpInfoString'))


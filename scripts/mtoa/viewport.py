# import tempfile
# import datetime
import os
from functools import partial
import maya.cmds as cmds         
import maya.mel as mel         
import maya.utils as utils     

import mtoa.aovs as aovs
import mtoa.core as core
import mtoa.callbacks as callbacks

CROP_CTX = 'arnoldViewportRegionToolContext1'
VIEW_OVERRIDE = 'arnoldViewOverride'

global gArnoldViewportRenderContol 
gArnoldViewportRenderContol = None

def getOption(option, **kwargs):
    return cmds.arnoldViewport(get=option)

def setOption(option, value, **kwargs):
    if len(kwargs):
        cmds.evalDeferred("cmds.arnoldViewport(opt=(\"{}\", \"{}\"))".format(option, value), **kwargs)
    else:
        cmds.arnoldViewport(opt=(option, value))

ON = True
OFF = False

# mel.eval("""

# global proc buildArnoldViewportRenderMenu()
# {
#     //
#     // Description:
#     //	This procedure builds the Presets menu in the Hardware Renderer 2.0 Settings window.
#     //
#     setParent arnoldRenderOverrideOptionsWindow;
#     menu -edit -deleteAllItems arnoldViewportOoptionsRenderMenu;
#     setParent -menu arnoldViewportOoptionsRenderMenu;

#     menuItem -label "Update Full Scene" -command "arnoldViewport -opt \\\"Update Full Scene\\\" \\\"1\\\"";

#     setParent -menu ..; 
# }

# gloabl proc arnoldViewportUpdateOptionsUI()
# {

# }

# global proc arnoldViewOverrideOptionBox()
# {
#     string $windowName = "arnoldRenderOverrideOptionsWindow";
#     // If the window exists already, just show it.
#     //
#     if (`window -exists $windowName`) {
#         showWindow $windowName;
#         arnoldViewportUpdateOptionsUI;
#         return;
#     }
    
#     string $prefDir = `internalVar -userPrefDir`;
#     string $totalString = $prefDir + "windowPrefs.mel";

#     string $windowTitle = "Arnold Renderer Viewport Settings";
#     window -menuBar true -title $windowTitle -retain $windowName;

#     menu -label "Render"
#             -postMenuCommand "buildArnoldViewportRenderMenu"
#             arnoldViewportOoptionsRenderMenu;

#     string $topLayout = `formLayout`;


#     checkBox -label "Show Status in HUD" arnoldViewportHUDCB;


#     if (!`exists $totalString`) {
#             window -edit -widthHeight 456 670 -topLeftCorner 220 220
#                     $windowName;
#     } 
    
# }
# """)

def buildArnoldViewportRenderMenu(*args):
    """Build the Redner Menu in the Viewport options UI
    """
    cmds.setParent("arnoldRenderOverrideOptionsWindow")
    cmds.menu( "arnoldViewportOptionsRenderMenu",  edit=True, deleteAllItems=True)
    cmds.setParent( "arnoldViewportOptionsRenderMenu", menu=True )

    cmds.menuItem(label="Update Full Scene", command="cmds.arnoldViewport(opt=(\"Update Full Scene\",\"1\"))")

    cmds.setParent("..", menu=True) 

def getViewportOptions():
    """Retrieve the viewport options from user prefs

    Returns:
        dict: dictionary of option->value pairs
    """
    options = {}
    options['viewportShowHUD'] = True

    for option, default in options.items():
        if not cmds.optionVar(exists="arnold_{}".format(option)):
            if type(default) in [bool, int]:
                cmds.optionVar(intValue=("arnold_{}".format(option), default))
            elif isinstance(default, utils.string_types):
                cmds.optionVar(stringValue=("arnold_{}".format(option), default))
        else:
            options[option] = cmds.optionVar(query="arnold_{}".format(option))

    return options

def setViewportOptions(ui, option):
    """Set the viewpoert options based on UI values

    Args:
        ui (str): UI to get value from
        option (str): option to set
    """
    options = {}
    options[option] = cmds.checkBoxGrp(ui, query=True, value1=True)

    for option, value in options.items():
        if type(value) in [bool, int]:
            cmds.optionVar(intValue=("arnold_{}".format(option), value))
        elif isinstance(value, utils.string_types):
            cmds.optionVar(stringValue=("arnold_{}".format(option), value))

    applyOptions()

def applyOptions():
    """Apply the current saved options to the current viewport
    """
    options = getViewportOptions()
    for option, value in options.items():
        if option == 'viewportShowHUD':
            cmds.arnoldViewport(hud=value)
            cmds.refresh(cv=True, force=True)    

def arnoldViewportUpdateOptionsUI():
    """Update the arnold viewport options UI, set the values of the controls
    """
    options = getViewportOptions()

    cmds.checkBoxGrp("arnoldViewportHUDCB", edit=True, value1=options['viewportShowHUD'])

def arnoldViewOverrideOptionBox():
    """Main call to generate Arnold Viewport options UI
    Called by then option box in the viewport renderer menu
    """

    windowName = "arnoldRenderOverrideOptionsWindow"
    if cmds.window(windowName, exists=True):
        cmds.showWindow(windowName)
        arnoldViewportUpdateOptionsUI()
        return
    
    prefDir = cmds.internalVar(userPrefDir=True);
    totalString = os.path.join(prefDir,"windowPrefs.mel")

    windowTitle = "Arnold Renderer Viewport Settings";
    cmds.window(windowName, menuBar=True, title=windowTitle, retain=True)

    cmds.menu("arnoldViewportOptionsRenderMenu",
              label="Render",
              postMenuCommand=buildArnoldViewportRenderMenu
             )

    # cmds.setParent(windowName)
    optionLayout = cmds.scrollLayout(childResizable=True)
    cmds.columnLayout(adjustableColumn=True)

    cmds.frameLayout("arnoldViewportOptionsFrame", label="Arnold Viewport Options", collapse=False, marginHeight=5)
    cmds.columnLayout(adjustableColumn=True)

    arnoldViewportHUDCB = cmds.checkBoxGrp("arnoldViewportHUDCB",
                     numberOfCheckBoxes=1,
                     label="Show Status in HUD")

    cmds.setParent("..")
    cmds.setParent("..")
    cmds.setParent("..")

    arnoldViewportUpdateOptionsUI()

    cmds.checkBoxGrp("arnoldViewportHUDCB",
                     edit=True,
                     cc=lambda arg=None, ui=arnoldViewportHUDCB, option='viewportShowHUD':setViewportOptions(ui, option))

    if os.path.exists(totalString):
            cmds.window(windowName, edit=True, widthHeight=[456,400], topLeftCorner=[220,220])
    cmds.showWindow(windowName)

class ArnoldViewportRenderControl():

    def __init__(self):
        self.ipr_state = OFF
        self.crop_state = OFF
        self.crop_region = (0,0,0,0)
        self.debug_mode = {} 
        self.script_jobs = []

        self.setup_callbacks()

    def reset(self):
        """Rest the viewport options on scene Open or New
        """
        self.ipr_state = OFF
        self.crop_state = OFF
        self.crop_region = (0,0,0,0)
        self.debug_mode = {} 

        self.update_panels(True)
        self.setup_callbacks()
        cmds.evalDeferred("cmds.ActivateViewport20()")

    def update(self, panel):
        """Update the current state
        if panel is set to "arnoldRenderView" rest all the icons

        Args:
            panel (str): panel name or arnoldRenderView
        """
        current_panel = self.get_arnold_panel()
        options = getViewportOptions()
        showHUD = options['viewportShowHUD']
        if panel == "arnoldRenderView":
            # if the given panel is arnoldRenderView, reset all icons
            self.ipr_state = OFF
            self.crop_state = OFF
            self.update_panels()
            return
        
        if current_panel:
            self.ipr_state = getOption("Run IPR") == "1"
            self.crop_state = getOption("Crop Region") == "1"

        if self.crop_state:
            reg = []
            for at in ['avpRegionLeft', 'avpRegionRight', 'avpRegionTop', 'avpRegionBottom']:
                reg.append(cmds.getAttr("defaultArnoldRenderOptions."+at))
           
            if reg == [0,0,0,0]:
                showHUD = False
        cmds.arnoldViewport(hud=showHUD)

    def setup_callbacks(self):

        for job in self.script_jobs:
            if not cmds.scriptJob(exists=job):
                self.script_jobs.pop(self.script_jobs.index(job))

        self.script_jobs.append(cmds.scriptJob(event=["NewSceneOpened", self.reset]))
        self.script_jobs.append(cmds.scriptJob(event=["PostSceneRead", self.reset]))

    def toggle_render(self, arnold_panel, state, update_buttons=True):
        """Start rendering in a given panel

        Arguments:
            arnold_panel {str} -- the panel we should start/stop rendering in.
            state {bool} -- The toggle state
        """
        mel.eval('if (`currentRenderer` != "arnold") '
                'setCurrentRenderer("arnold");')
        toggle_ctl = 'avp_toggle_%s' % arnold_panel
        if not cmds.iconTextCheckBox(toggle_ctl, ex=True):
            return
        if not arnold_panel or not cmds.modelEditor(arnold_panel, ex=True):
            return
            
        if state:
            # check if the current panel is the same as the given panel
            # if not stop rendering on the current panel before starting on this panel
            current_arnold_panel = self.get_arnold_panel()
            if current_arnold_panel != arnold_panel:
                self.update_button_state("avp_crop", current_arnold_panel, False)
                self.update_button_state("avp_toggle", current_arnold_panel, False)
                self.update_icon_bar_enable_state(current_arnold_panel, False)

            if cmds.modelEditor(
                    arnold_panel,
                    q=True,
                    rendererOverrideName=True) != VIEW_OVERRIDE:
                cmds.modelEditor(arnold_panel, e=True,
                        rendererOverrideName=VIEW_OVERRIDE)

            self.toggle_crop(arnold_panel, False, False)
            # constantly switching IPr on and off can cause crashes
            # for now we only enable it if it was disabled
            if not self.ipr_state:
                self.set_ipr(ON)
        else:
            self.stop_render(arnold_panel)

        if cmds.iconTextCheckBox(toggle_ctl, ex=True):
            bstate = cmds.iconTextCheckBox(toggle_ctl, q=True, v=True)
            if bstate != state:
                cmds.iconTextCheckBox(toggle_ctl, e=True, v=state)

        if update_buttons:
            self.update_icon_bar_enable_state(arnold_panel, state)
        cmds.refresh(cv=True, force=True)

    def stop_render(self, arnold_panel):
        """Stop viewport rendering.

        Arguments:
            arnold_panel {str} -- The panel that should be stopped.
        """
        # self.set_ipr(OFF)

        if arnold_panel:
            cmds.setFocus(arnold_panel)
            cmds.ActivateViewport20()

    def set_ipr(self, state):
        """Toggle IPR state

        Args:
            state (bool): enable state for IPR
        """
        if state != self.ipr_state:
            ipr_value = "1" if state else "0"
            self.ipr_state = state
            setOption("Run IPR", ipr_value, lp=True)

    def restart(self, arnold_panel):
        cmds.arnoldViewport(opt=("Refresh Render", "1"))    

    def toggle_crop(self, arnold_panel, state, stop_ipr=True):
        """Enable/Disable the viewport's crop controls.

        Arguments:
            arnold_panel {str} -- The current panel
            state {bool} -- active or not
        """

        if not state:
            # if we're turning off the crop we 
            if stop_ipr:
                self.set_ipr(OFF)
                self.stop_render(arnold_panel)
            self.set_crop(OFF)
            cmds.setToolTo("selectSuperContext")
        else:
            # check if the current panel is the same as the given panel
            # if not stop rendering on the current panel before starting on this panel
            current_arnold_panel = self.get_arnold_panel()
            if current_arnold_panel != arnold_panel:
                self.update_button_state("avp_crop", current_arnold_panel, False)
                self.update_button_state("avp_toggle", current_arnold_panel, False)
                self.update_icon_bar_enable_state(current_arnold_panel, False)

            # check if the current view is enabled for full frame, and disable the icon if it is 
            self.update_button_state("avp_toggle", arnold_panel, False)

            self.set_crop(ON)
            if cmds.modelEditor(
                    arnold_panel,
                    q=True,
                    rendererOverrideName=True) != VIEW_OVERRIDE:
                cmds.modelEditor(arnold_panel, e=True,
                        rendererOverrideName=VIEW_OVERRIDE)
            cmds.arnoldViewportRegionToolContext()
            cmds.setToolTo(CROP_CTX)
            if not self.ipr_state:
                self.set_ipr(ON)

        cmds.refresh(cv=True, force=True)

        self.update_icon_bar_enable_state(arnold_panel, state)

        # make sure the icon state reflects the actual tool state.
        self.update_button_state("avp_crop", arnold_panel, state)
    
    def set_crop(self, state):
        """Toggle Crop state

        Args:
            state (bool): enable state for crop
        """
        if state != self.crop_state:
            crop_value = "1" if state else "0"
            self.crop_state = state
            setOption("Crop Region", crop_value)

    def get_crop_region(self):
        """Retrieve the dimensions of the crop region

        Returns:
            tupple: int values relating to left,right,bottom,top
        """
        defaultOptionsNode = 'defaultArnoldRenderOptions'
        if not cmds.objExists(defaultOptionsNode):
            core.createOptions()
        left = cmds.getAttr("{}.avpRegionLeft".format(defaultOptionsNode))
        right = cmds.getAttr("{}.avpRegionRight".format(defaultOptionsNode))
        bottom = cmds.getAttr("{}.avpRegionBottom".format(defaultOptionsNode))
        top = cmds.getAttr("{}.avpRegionTop".format(defaultOptionsNode))

        return left,right,bottom,top

    def get_option_state(self, option, arnold_panel):
        option_ctl = 'avp_{}_{}'.format(option,arnold_panel)
        if cmds.iconTextCheckBox(option_ctl, ex=True):
            return cmds.iconTextCheckBox(option_ctl, q=True, value=True)
        return False

    def update_button_state(self, control_name, arnold_panel, state):
        icn_name = '{}_{}'.format(control_name, arnold_panel)
        if cmds.iconTextCheckBox(icn_name, ex=True) and cmds.iconTextCheckBox(icn_name, q=True, v=True) != state:
                cmds.iconTextCheckBox(icn_name, e=True, v=state)

    def update_icon_bar_enable_state(self, arnold_panel, state, reset=False):
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
            if reset:
                self.toggle_debug(arnold_panel, False)

        aov_ctl = 'avp_aov_%s' % arnold_panel
        if cmds.iconTextButton(aov_ctl, ex=True):
            cmds.iconTextButton(aov_ctl, e=True, en=state)

        res_ctl = 'avp_resolution_%s' % arnold_panel
        if cmds.iconTextButton(res_ctl, ex=True):
            cmds.iconTextButton(res_ctl, e=True, en=state)

    def get_arnold_panel(self):
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
                    rendererOverrideName=True) == VIEW_OVERRIDE:
                arnold_panel = panel
                break
        return arnold_panel


    def get_arnold_panel_iconbar(self, panel=None):
        """Returns the icon bar layout.

        Keyword Arguments:
            panel {str} -- the panel of interest. If not defined we call
            get_arnold_panel() to get the currently rendering panel (default: {None})

        Returns:
            None -- If we could not find the layout
            (arnold_panel, iconbar_layout) -- If successful.
        """
        arnold_panel = panel if panel else self.get_arnold_panel()
        if not arnold_panel:
            return None
        ctl = cmds.modelEditor(arnold_panel, q=True, ctl=True)
        form = cmds.control(ctl, q=True, parent=True)
        kids = cmds.layout(form, q=True, ca=True)
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


    def setup_icon_bar(self, iconbar, panel, uip):
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
        # start/stop full view render
        toggle_render_btn = cmds.iconTextCheckBox('avp_toggle_%s' % panel, w=18, h=18,
                                i='avp_toggle.png',
                                ann='Start/Stop Arnold in the viewport',
                                v=False, p=ctlform)
        cmds.iconTextCheckBox(toggle_render_btn, e=True,
                            cc=partial(self.toggle_render, panel))

        # Start crop render tool
        crop_btn = cmds.iconTextCheckBox('avp_crop_%s' % panel, w=18, h=18, en=True,
                                i='avp_crop.png',
                                ann='Define a crop window',
                                v=False, p=ctlform)
        cmds.iconTextCheckBox(crop_btn, e=True,
                            cc=partial(self.toggle_crop, panel))

        # resolution
        res_btn = cmds.iconTextButton('avp_resolution_%s' % panel, w=18, h=18, en=False,
                                i='avp_resolution.png',
                                ann="Set the viewport's render resolution",
                                p=ctlform)
        cmds.popupMenu('avp_resolution_menu_%s' % panel, parent=res_btn, button=1,
                    pmc=partial(self.build_resolution_menu, panel))

        # debug shading
        debug_btn = cmds.iconTextCheckBox('avp_debug_%s' % panel, w=18, h=18, en=False,
                                i='avp_debug.png',
                                ann='Set shading to debug mode',
                                v=False, p=ctlform,
                                cc=partial(self.toggle_debug, panel))
        cmds.popupMenu('avp_debug_menu_%s' % panel, parent=debug_btn, button=1,
                    pmc=partial(self.build_debug_menu, panel))

        # AOVs
        aov_btn = cmds.iconTextButton('avp_aov_%s' % panel, w=18, h=18, en=False,
                                i='avp_aovs.png',
                                ann="Select display channels",
                                p=ctlform)
        cmds.popupMenu('avp_aov_menu_%s' % panel, parent=aov_btn, button=1, pmc=self.update_aov_menu)

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

    def build_resolution_menu(self, *args):
        """Build the resolution menu when the user clicks the button.

        Arguments:
            args[0] {str}: the current panel.
            args[1] {str}: the parent control.
        """
        panel = args[0]
        parent = args[1]

        testres = cmds.arnoldViewport(get="Test Resolution") or "100%"

        cmds.popupMenu(parent, e=True, deleteAllItems=True)
        cmds.radioMenuItemCollection(p=parent)
        for res in ["100%", "75%", "50%", "25%", "10%"]:
            cmds.menuItem(l=res, p=parent, rb=(testres == res),
                        c=partial(self.change_resolution, res, panel))


    def change_resolution(self, testres, arnold_panel, *args):
        """Store the new resolution multiplier and restart the render if it was
        already running.

        Arguments:
            args[0] {float}: the new resolution multiplier
            args[1] {str}: the current panel
        """
        current_arnold_panel = self.get_arnold_panel()
        # if resolution is set from another viewport, ignore that and warn.
        if current_arnold_panel and current_arnold_panel != arnold_panel:
            cmds.warning('Ignored new resolution: there is already one active'
                            ' Arnold viewport (%s).', current_arnold_panel)
            return
        setOption("Test Resolution", testres)

    def build_debug_menu(self, *args):

        panel = args[0]
        parent = args[1]

        # current_mode = getOption("Debug Shading") or "Basic"
        current_state = self.get_option_state("debug", panel)
    
        if panel not in self.debug_mode:
            self.debug_mode[panel] = "Basic"

        modes = ["Basic","Occlusion",
                "Wireframe","Normal",
                "UV","Primitive ID",
                "Object",
                "Barycentric",
                "Lighting"]

        # clear the menu
        cmds.popupMenu(parent, e=True, deleteAllItems=True)

        # start adding new items
        cmds.menuItem(l='Enable debug shading',                  
                    p=parent,
                    cb=current_state,
                    c=partial(self.toggle_debug, panel))
        cmds.menuItem(divider=True, p=parent)
        k_collection_name = parent.split('|')[-1] + '_collection'
        cmds.radioMenuItemCollection(k_collection_name, p=parent)
        for mode in modes:
            cmds.menuItem(l=mode, p=parent, rb=(self.debug_mode[panel] == mode),
                        collection=k_collection_name, c=partial(self.change_debug_mode, mode, panel))


    def change_debug_mode(self, mode, arnold_panel, *args):
        """Change the debug mode if it is different to the current debug mdoe

        Arguments:
            args[0] {str}: the new debug mode
            args[1] {str}: the current panel
        """
        if mode != self.debug_mode[arnold_panel] or getOption("Debug Shading") == 'Disabled':
            self.debug_mode[arnold_panel] = mode
            self.toggle_debug(arnold_panel, True)

    def toggle_debug(self, arnold_panel, state):

        if arnold_panel not in self.debug_mode:
            self.debug_mode[arnold_panel] = "Basic"
        
        mode = self.debug_mode[arnold_panel]

        if not state:
            mode = "Disabled"

        setOption("Debug Shading", mode)
        self.update_button_state("avp_debug", arnold_panel, state)


    def build_aov_menu(self, *args):

        def set_avp_aov(name, *_args):
            setOption("AOVs", name)

            cmds.arnoldViewport(opt=("AOVs", name))

        current_aov_name = cmds.arnoldViewport(get="AOVs")
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

    def update_aov_menu(self, *args):
        """Called every time the viewport's AOV menu needs to be displayed."""
        rendering_panel = self.get_arnold_panel()
        if rendering_panel:
            self.build_aov_menu(*args)

    def add_ctls_to_all_panels(self):
        """List all panels and setup their icon bar.
        """
        uip = cmds.setParent(q=True)
        arnold_panel = self.get_arnold_panel()
        panel_list = cmds.getPanel(type='modelPanel')
        if not panel_list:
            return
        for panel in panel_list:
            try:
                _, iconbar = self.get_arnold_panel_iconbar(panel=panel)
            except TypeError:
                pass
            else:
                self.setup_icon_bar(iconbar, panel, uip)
    
    def update_panels(self, reset=False):
        panel_list = cmds.getPanel(type='modelPanel')
        if not panel_list:
            return
        for panel in panel_list:
            self.update_button_state("avp_crop", panel, self.crop_state)
            self.update_button_state("avp_toggle", panel, self.ipr_state)
            self.update_icon_bar_enable_state(panel, self.ipr_state or self.crop_state, reset)


def add_controls():
    """the main function called at atsrtup to create the viewport controls
    """
    if cmds.about(batch=True):
        return

    global gArnoldViewportRenderContol

    if not gArnoldViewportRenderContol:
        gArnoldViewportRenderContol = ArnoldViewportRenderControl()

    gArnoldViewportRenderContol.add_ctls_to_all_panels()

def update_controls(panel):
    """Update the controls, called when a scene is opened or new scene created

    Args:
        panel (string): panel to update
    """
    global gArnoldViewportRenderContol

    if gArnoldViewportRenderContol:
        gArnoldViewportRenderContol.update(panel)

def run_override(panel):
    """Update the controls, called when a scene is opened or new scene created

    Args:
        panel (string): panel to update
    """
    global gArnoldViewportRenderContol

    if gArnoldViewportRenderContol and not gArnoldViewportRenderContol.crop_state:
        gArnoldViewportRenderContol.toggle_render(panel, True)
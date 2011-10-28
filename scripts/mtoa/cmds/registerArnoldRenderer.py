import glob
import os
import sys
import mtoa.utils as utils

if 'pymel' not in globals():
    import pymel
    import pymel.versions as versions
    maya_version = versions.shortName()
    print "Maya %s importing module pymel %s" % (maya_version, pymel.__version__)
else :
    print "Maya %s had already imported module pymel %s" % (maya_version, pymel.__version__)
    
# fix for pymel versions less than 1.0.3 (pre-2012)
if 'pymel' in globals():
    if pymel.__version__ < '1.0.3' and not hasattr(pymel, '_mtoaPatch'):
        maya_version = versions.shortName()
        print "Maya %s reloading patched pymel" % maya_version
        root = utils.mtoaPackageRoot()
        path = os.path.join(root, maya_version)
        sys.path.insert(0, path)
        # clear so pymel reloads
        for key in sys.modules.keys() :
            if key.startswith('pymel') :
                del sys.modules[key]
        import pymel

import pymel.core as pm
import pymel.versions as versions

if hasattr(pymel, '_mtoaPatch') :
    print "Patched pymel version %s loaded" % pymel.__version__
else :
    print "Standard pymel version %s loaded" % pymel.__version__

import mtoa.ui.exportass as exportass
import mtoa.ui.nodeTreeLister as nodeTreeLister
import mtoa.ui.globals.common
from mtoa.ui.globals.common import createArnoldRendererCommonGlobalsTab, updateArnoldRendererCommonGlobalsTab
from mtoa.ui.globals.arnold import createArnoldRendererGlobalsTab, updateArnoldRendererGlobalsTab
from mtoa.ui.aoveditor import createArnoldAOVTab, updateArnoldAOVTab
import mtoa.ui.ae.utils as aeUtils
from mtoa.ui.ae.aiStandInTemplate import ArnoldExportRenderObjectWindow

import mtoa.cmds.arnoldRender as arnoldRender

def _overrideMelScripts():
    root = utils.mtoaPackageRoot()
    maya_version = versions.shortName()
    for f in glob.glob(os.path.join(root, maya_version, 'mel', '*.mel')):
        print "Maya %s sourcing MEL override %s" % (maya_version, f)
        pm.mel.source(pm.mel.encodeString(f))

def _overridePythonScripts():
    root = utils.mtoaPackageRoot()
    maya_version = versions.shortName()
    path = os.path.join(root, maya_version)
    sys.path.insert(0, path)
    # for root, dirnames, filenames in os.walk('path'): 
    for f in os.listdir(path):
        if f.endswith('.py'):
            print "Maya %s importing * from Python override %s from %s" % (maya_version, f, path)
            import_string = "from %s import *" % os.path.splitext(f)[0]
            exec import_string
            # module = __import__(os.path.splitext(f)[0])


# We need to override this two proc to avoid
# errors because of the hardcoded code.
def updateMayaImageFormatControl():
    #pm.mel.source("createMayaSoftwareCommonGlobalsTab.mel")
    currentRenderer = utils.currentRenderer()
    if currentRenderer == 'mentalRay':
        pm.mel.updateMentalRayImageFormatControl()
    elif currentRenderer == 'arnold':
        mtoa.ui.globals.common.updateArnoldImageFormatControl()
    else:
        pm.mel.updateMayaSoftwareImageFormatControl();

    if currentRenderer != 'arnold' and pm.mel.getApplicationVersionAsFloat() >= 2009:
        pm.mel.updateMultiCameraBufferNamingMenu();

def renderSettingsTabLabel_melToUI(smel):

    # The arguments passed inside this procedure should not
    # be localized. This procedure uses the first string
    # argument that is passed with the "-addGlobalsTab"
    # flag in the "renderer" command.
    result = smel

    if smel == 'Common':
        result = pm.mel.uiRes("m_unifiedRenderGlobalsWindow.kCommon")

    elif smel == 'Passes':
        result = pm.mel.uiRes("m_unifiedRenderGlobalsWindow.kPassesTab")

    elif smel == 'Maya Software':
        result = pm.mel.uiRes("m_unifiedRenderGlobalsWindow.kMayaSoftware")

    elif smel == 'Maya Hardware':
        result = pm.mel.uiRes("m_unifiedRenderGlobalsWindow.kMayaHardware")

    elif smel == 'Maya Vector':
        result = pm.mel.uiRes("m_unifiedRenderGlobalsWindow.kMayaVector")

    elif smel == 'Features':
        result = pm.mel.uiRes("m_unifiedRenderGlobalsWindow.kFeatures")

    elif smel == 'Quality':
        result = pm.mel.uiRes("m_unifiedRenderGlobalsWindow.kQuality")

    elif smel == 'Indirect Lighting':
        result = pm.mel.uiRes("m_unifiedRenderGlobalsWindow.kIndirectLighting")

    elif smel == 'Options':
        result = pm.mel.uiRes("m_unifiedRenderGlobalsWindow.kOptions")

    else:
        pm.mel.uiToMelMsg("renderSettingsTabLabel_melToUI", smel, 0)

    return result

def addOneTabToGlobalsWindow(renderer, tabLabel, createProc):
    # Check to see if the unified render globals window existed.
    # If it does not exist, then we don't need to add any tab yet.
    if not pm.window('unifiedRenderGlobalsWindow', exists=True):
        try:
            pm.error(pm.mel.uiRes("m_unifiedRenderGlobalsWindow.kCannotAddTabs"))
        except:
            pass
        return
    
    displayAllTabs = pm.mel.isDisplayingAllRendererTabs()

    # If the current renderer the renderer is not this
    # renderer, then don't add the tab yet.
    if not displayAllTabs and utils.currentRenderer() != renderer:
        return

    pm.setParent('unifiedRenderGlobalsWindow')

    # Hide the tabForm while updating.
    tabFormManagedStatus = pm.formLayout('tabForm', q=True, manage=True)
    pm.formLayout('tabForm', edit=True, manage=False)
    pm.setParent('tabForm')

    # Set the correct tabLayout parent.
    if displayAllTabs:
        tabLayoutName = pm.mel.getRendererTabLayout(pm.melGlobals['gMasterLayerRendererName'])
    else:
        tabLayoutName = pm.mel.getRendererTabLayout(renderer)

    pm.setParent(tabLayoutName)

    # The tabName is the tabLabel with the white space removed
    # and the word "Tab" added to the end.
    # "masterLayer" will act as the renderer name if the tab
    # is in the master layer.
    tabName = pm.mel.rendererTabName(renderer, tabLabel)

    # if the tab-control does not exist, define it and add it
    # to the tabLayout
    if not pm.layout(tabName, exists=True):
        pm.setUITemplate('renderGlobalsTemplate', pushTemplate=True)
        pm.setUITemplate('attributeEditorTemplate', pushTemplate=True)

        # Define the tab
        pm.formLayout(tabName)

        # get the content of the tab from the createTabProc

        # Most create procs are now deferred till the tab is selected
        # These two are the default tabs, so we don't defer them
        createProcs = ['createMayaSoftwareCommonGlobalsTab',
                          'createMayaSoftwareGlobalsTab',
                          'createArnoldRendererCommonGlobalsTab',
                          'createArnoldRendererGlobalsTab']

        if createProc in createProcs:
            pm.mel.eval(createProc)

        # These end off the layouts of the information in the Tab
        pm.setParent('..')

        pm.setUITemplate(popTemplate=True)
        pm.setUITemplate(popTemplate=True)

        # Add the tab to the tabLayout
        pm.tabLayout(tabLayoutName,
                       edit=True,
                       tabLabel=(tabName, renderSettingsTabLabel_melToUI(tabLabel)))

    # Restore the old manage status for the tabForm.
    pm.formLayout('tabForm', edit=True, manage=tabFormManagedStatus)

def arnoldAddGlobalsTabs():
    pm.renderer('arnold', edit=True, addGlobalsTab=('Common',
                                                      utils.pyToMelProc(createArnoldRendererCommonGlobalsTab, useName=True),
                                                      utils.pyToMelProc(updateArnoldRendererCommonGlobalsTab, useName=True)))
    pm.renderer('arnold', edit=True, addGlobalsTab=('Arnold Renderer',
                                                      utils.pyToMelProc(createArnoldRendererGlobalsTab, useName=True),
                                                      utils.pyToMelProc(updateArnoldRendererGlobalsTab, useName=True)))
    pm.renderer('arnold', edit=True, addGlobalsTab=('AOVs', 
                                                      utils.pyToMelProc(createArnoldAOVTab, useName=True), 
                                                      utils.pyToMelProc(updateArnoldAOVTab, useName=True)))


def registerArnoldRenderer():
    pm.createNode('aiOptions', skipSelect=True, shared=True, name='defaultArnoldRenderOptions')

    alreadyRegistered = pm.renderer('arnold', exists=True)
    if not alreadyRegistered:
        pm.renderer('arnold',
                      rendererUIName='Arnold Renderer',
                      renderProcedure = utils.pyToMelProc(arnoldRender.arnoldRender,
                                                          [('int', 'width'), ('int', 'height'),
                                                           ('int', 'doShadows'), ('int', 'doGlowPass'),
                                                           ('string', 'camera'), ('string', 'options')]),
                      renderRegionProcedure = 'mayaRenderRegion',
                      commandRenderProcedure    = utils.pyToMelProc(arnoldRender.arnoldBatchRender,
                                                                    [('string', 'option')]),
                      batchRenderProcedure      = utils.pyToMelProc(arnoldRender.arnoldBatchRender,
                                                                    [('string', 'option')]),
                      iprRenderProcedure        = utils.pyToMelProc(arnoldRender.arnoldIprRender,
                                                                    [('int', 'width'), ('int', 'height'),
                                                                     ('int', 'doShadows'), ('int', 'doGlowPass'),
                                                                     ('string', 'camera')]),
                      isRunningIprProcedure     = utils.pyToMelProc(arnoldRender.arnoldIprIsRunning, returnType='int'),
                      startIprRenderProcedure   = utils.pyToMelProc(arnoldRender.arnoldIprStart,
                                                                    [('string', 'editor'), ('int', 'resolutionX'),
                                                                     ('int', 'resolutionY'), ('string', 'camera')]),
                      stopIprRenderProcedure    = utils.pyToMelProc(arnoldRender.arnoldIprStop),
                      refreshIprRenderProcedure = utils.pyToMelProc(arnoldRender.arnoldIprRefresh),
                      pauseIprRenderProcedure   = utils.pyToMelProc(arnoldRender.arnoldIprPause,
                                                                    [('string', 'editor'), ('int', 'pause')]),
                      changeIprRegionProcedure  = utils.pyToMelProc(arnoldRender.arnoldIprChangeRegion,
                                                                    [('string', 'renderPanel')]))


        pm.evalDeferred(arnoldAddGlobalsTabs)
        
        #We have to source this file otherwise maya will override
        #our mel proc overrides below.
        #
        pm.mel.source('createMayaSoftwareCommonGlobalsTab.mel')
        
        utils.pyToMelProc(addOneTabToGlobalsWindow,
                          [('string', 'renderer'), ('string', 'tabLabel'), ('string', 'createProc')],
                          useName=True)
        utils.pyToMelProc(renderSettingsTabLabel_melToUI,
                          [('string', 'mel')],
                          useName=True)
        utils.pyToMelProc(updateMayaImageFormatControl,
                          useName=True)

        aeUtils.loadAETemplates()
        import mtoa.ui.ae.customShapeAttributes
        
        # version specific overrides or additions
        _overridePythonScripts()
        _overrideMelScripts()

        pm.renderer('arnold', edit=True, addGlobalsNode='defaultArnoldRenderOptions')

        # Add an Arnold menu in Maya main window
        if not pm.about(b=1):
            pm.menu('ArnoldMenu', label='Arnold', parent='MayaWindow', tearOff=True )
            pm.menuItem('ArnoldStandIn', label='StandIn', parent='ArnoldMenu', subMenu=True)
            pm.menuItem('ArnoldCreateStandIn', parent='ArnoldStandIn', label="Create",
                        c=lambda *args: pm.createNode('aiStandIn', n='ArnoldStandInShape'))
            pm.menuItem('ArnoldExportStandIn', parent='ArnoldStandIn', label='Export', c=ArnoldExportRenderObjectWindow)

            #cmds.menuItem(parent='ArnoldMenu', divider=True)aiSkyDomeLight1
            # Add option box for file translator
            utils.pyToMelProc(exportass.arnoldAssOpts,
                              [('string', 'parent'), ('string', 'action'),
                               ('string', 'initialSettings'), ('string', 'resultCallback')],
                               useName=True)

            pm.menuItem('ArnoldLights', label='Lights', parent='ArnoldMenu', subMenu=True)
            pm.menuItem('ArnoldAreaLights', parent='ArnoldLights', label="Area Light",
                        c=lambda *args: pm.createNode('aiAreaLight', n='aiAreaLight'))
            pm.menuItem('SkydomeLight', parent='ArnoldLights', label="Skydome Light",
                        c=lambda *args: pm.createNode('aiSkyDomeLight', n='aiSkyDomeLight'))

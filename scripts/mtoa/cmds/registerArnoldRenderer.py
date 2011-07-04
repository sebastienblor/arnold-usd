import maya.cmds as cmds
import maya.mel as mel

import mtoa.utils as utils
import mtoa.ui.exportass as exportass
import mtoa.ui.nodeTreeLister as nodeTreeLister
import mtoa.ui.globals.common
from mtoa.ui.globals.common import createArnoldRendererCommonGlobalsTab, updateArnoldRendererCommonGlobalsTab
from mtoa.ui.globals.arnold import createArnoldRendererGlobalsTab, updateArnoldRendererGlobalsTab
import mtoa.ui.ae.utils as aeUtils
from mtoa.ui.ae.aiStandInTemplate import ArnoldExportRenderObjectWindow

import mtoa.cmds.arnoldRender as arnoldRender


# We need to override this two proc to avoid
# errors because of the hardcoded code.
def updateMayaImageFormatControl():
    # mel.eval('source "createMayaSoftwareCommonGlobalsTab.mel";')
    currentRenderer = utils.currentRenderer()
    if currentRenderer == 'mentalRay':
        mel.eval('updateMentalRayImageFormatControl();')
    elif currentRenderer == 'arnold':
        mtoa.ui.globals.common.updateArnoldImageFormatControl()
    else:
        mel.eval('updateMayaSoftwareImageFormatControl();')

    if currentRenderer != 'arnold' and mel.eval('getApplicationVersionAsFloat') >= 2009:
        mel.eval('updateMultiCameraBufferNamingMenu();')

def renderSettingsTabLabel_melToUI(smel):

    # The arguments passed inside this procedure should not
    # be localized. This procedure uses the first string
    # argument that is passed with the "-addGlobalsTab"
    # flag in the "renderer" command.
    result = smel

    if smel == 'Common':
        result = mel.eval('uiRes("m_unifiedRenderGlobalsWindow.kCommon")')

    elif smel == 'Passes':
        result = mel.eval('uiRes("m_unifiedRenderGlobalsWindow.kPassesTab")')

    elif smel == 'Maya Software':
        result = mel.eval('uiRes("m_unifiedRenderGlobalsWindow.kMayaSoftware")')

    elif smel == 'Maya Hardware':
        result = mel.eval('uiRes("m_unifiedRenderGlobalsWindow.kMayaHardware")')

    elif smel == 'Maya Vector':
        result = mel.eval('uiRes("m_unifiedRenderGlobalsWindow.kMayaVector")')

    elif smel == 'Features':
        result = mel.eval('uiRes("m_unifiedRenderGlobalsWindow.kFeatures")')

    elif smel == 'Quality':
        result = mel.eval('uiRes("m_unifiedRenderGlobalsWindow.kQuality")')

    elif smel == 'Indirect Lighting':
        result = mel.eval('uiRes("m_unifiedRenderGlobalsWindow.kIndirectLighting")')

    elif smel == 'Options':
        result = mel.eval('uiRes("m_unifiedRenderGlobalsWindow.kOptions")')

    else:
        mel.eval('uiToMelMsg( "renderSettingsTabLabel_melToUI", "%s", 0)'%smel)

    return result

def addOneTabToGlobalsWindow(renderer, tabLabel, createProc):
    # Check to see if the unified render globals window existed.
    # If it does not exist, then we don't need to add any tab yet.
    if not cmds.window('unifiedRenderGlobalsWindow', exists=True):
        try:
            cmds.error(mel.eval('uiRes("m_unifiedRenderGlobalsWindow.kCannotAddTabs")'))
        except:
            pass
        return

    displayAllTabs = mel.eval('isDisplayingAllRendererTabs')

    # If the current renderer the renderer is not this
    # renderer, then don't add the tab yet.
    if not displayAllTabs and utils.currentRenderer() != renderer:
        return

    cmds.setParent('unifiedRenderGlobalsWindow')

    # Hide the tabForm while updating.
    tabFormManagedStatus = cmds.formLayout('tabForm', q=True, manage=True)
    cmds.formLayout('tabForm', edit=True, manage=False)
    cmds.setParent('tabForm')

    # Set the correct tabLayout parent.
    tabLayoutName = ''
    if displayAllTabs:
        gMasterLayerRendererName = mel.eval('global string $gMasterLayerRendererName; $a_temps = $gMasterLayerRendererName')
        tabLayoutName = mel.eval('getRendererTabLayout("%s")'%gMasterLayerRendererName)
    else:
        tabLayoutName = mel.eval('getRendererTabLayout("%s")'%(renderer))

    cmds.setParent(tabLayoutName)

    # The tabName is the tabLabel with the white space removed
    # and the word "Tab" added to the end.
    # "masterLayer" will act as the renderer name if the tab
    # is in the master layer.
    tabName = mel.eval('rendererTabName("%s", "%s")'%(renderer, tabLabel))

    # if the tab-control does not exist, define it and add it
    # to the tabLayout
    if not cmds.layout(tabName, exists=True):
        cmds.setUITemplate('renderGlobalsTemplate', pushTemplate=True)
        cmds.setUITemplate('attributeEditorTemplate', pushTemplate=True)

        # Define the tab
        cmds.formLayout(tabName)

        # get the content of the tab from the createTabProc

        # Most create procs are now deferred till the tab is selected
        # These two are the default tabs, so we don't defer them
        createProcs = ['createMayaSoftwareCommonGlobalsTab',
                          'createMayaSoftwareGlobalsTab',
                          'createArnoldRendererCommonGlobalsTab',
                          'createArnoldRendererGlobalsTab']

        if createProc in createProcs:
            mel.eval(createProc)

        # These end off the layouts of the information in the Tab
        cmds.setParent('..')

        cmds.setUITemplate(popTemplate=True)
        cmds.setUITemplate(popTemplate=True)

        # Add the tab to the tabLayout
        cmds.tabLayout(tabLayoutName,
                       edit=True,
                       tabLabel=(tabName, renderSettingsTabLabel_melToUI(tabLabel)))

    # Restore the old manage status for the tabForm.
    cmds.formLayout('tabForm', edit=True, manage=tabFormManagedStatus)

def arnoldAddGlobalsTabs():
    cmds.renderer('arnold', edit=True, addGlobalsTab=('Common',
                                                      utils.pyToMelProc(createArnoldRendererCommonGlobalsTab, useName=True),
                                                      utils.pyToMelProc(updateArnoldRendererCommonGlobalsTab, useName=True)))
    cmds.renderer('arnold', edit=True, addGlobalsTab=('Arnold Renderer',
                                                      utils.pyToMelProc(createArnoldRendererGlobalsTab, useName=True),
                                                      utils.pyToMelProc(updateArnoldRendererGlobalsTab, useName=True)))


def registerArnoldRenderer():
    cmds.createNode('aiOptions', skipSelect=True, shared=True, name='defaultArnoldRenderOptions')

    alreadyRegistered = cmds.renderer('arnold', exists=True)
    if not alreadyRegistered:
        cmds.renderer('arnold',
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


        cmds.evalDeferred(arnoldAddGlobalsTabs)

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

        cmds.renderer('arnold', edit=True, addGlobalsNode='defaultArnoldRenderOptions')

        # Add an Arnold menu in Maya main window
        if not cmds.about(b=1):
            cmds.menu('ArnoldMenu', label='Arnold', parent='MayaWindow', tearOff=True )
            cmds.menuItem('ArnoldStandIn', label='StandIn', parent='ArnoldMenu', subMenu=True)
            cmds.menuItem('ArnoldCreateStandIn', parent='ArnoldStandIn', label="Create",
                        c=lambda *args: cmds.createNode('aiStandIn', n='ArnoldStandInShape'))
            cmds.menuItem('ArnoldExportStandIn', parent='ArnoldStandIn', label='Export', c=ArnoldExportRenderObjectWindow)

            #cmds.menuItem(parent='ArnoldMenu', divider=True)
            # Add option box for file translator
            utils.pyToMelProc(exportass.arnoldAssOpts,
                              [('string', 'parent'), ('string', 'action'),
                               ('string', 'initialSettings'), ('string', 'resultCallback')],
                               useName=True)
            # setup hypershade node tree listing
            nodeTreeLister.setup()

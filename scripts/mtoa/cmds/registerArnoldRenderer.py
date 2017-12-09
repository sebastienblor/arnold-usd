import glob
import os
import sys
import inspect
import mtoa.utils
import arnoldShelf
import maya.cmds as cmds
import maya.mel as mel
import mtoa.melUtils as mu

def mtoaPackageRoot():
    '''return the path to the mtoa python package directory'''
    return os.path.dirname(os.path.dirname(inspect.getfile(inspect.currentframe())))

try:
    import mtoa.utils as utils
    import mtoa.ui.exportass as exportass
    import mtoa.ui.nodeTreeLister as nodeTreeLister
    import mtoa.ui.globals.common
    from mtoa.ui.globals.settings import updateBackgroundSettings, updateAtmosphereSettings
    import mtoa.ui.ae.utils as aeUtils
    from mtoa.ui.arnoldmenu import createArnoldMenu
    import mtoa.cmds.arnoldRender as arnoldRender
    from mtoa.cmds.rendererCallbacks import aiRenderSettingsBuiltCallback
except:
    import traceback
    traceback.print_exc(file=sys.__stderr__) # goes to the console
    raise

# These few lines should be removed (see #1913)
#if not cmds.about(batch=True):
#    for nodeType in pluginInfo('mtoa', q=1, dependNode=1):
#        _factories.addMayaType(nodeType, 'kPluginDependNode')

'''
def _overrideMelScripts():
    # for those procedures that we could not simply define overrides interactively, we keep edited files
    # per version of maya
    root = mtoaPackageRoot()
    maya_version = cmds.about(version=True)
    meldir = os.path.join(root, maya_version, 'mel')
    meldir = mtoa.utils.convertToUnicode(meldir)
    pathsep = mtoa.utils.convertToUnicode(os.pathsep)
    maya_script_path = mtoa.utils.convertToUnicode(mtoa.utils.getEnvironmentVariable(u'MAYA_SCRIPT_PATH'))
    mtoa.utils.setEnvironmentVariable(u'MAYA_SCRIPT_PATH', meldir + pathsep + maya_script_path)
    for f in glob.glob(os.path.join(meldir, '*.mel')):
        print>>sys.__stdout__, "Maya %s sourcing MEL override %s" % (maya_version, f)
        print "Maya %s sourcing MEL override %s" % (maya_version, f)
        fStr = mel.eval("encodeString \"+f+"\"")
        mel.eval("source \""+fStr+"\"")
        test = mel.("eval "+ (os.path.split(f)[1]).split(': ', 1)))
        if len(test) == 2 and test[1].replace('\\', '/') != f.replace('\\', '/'):
            cmds.warning("Overriding failed: Maya is still using %s" % test[1])

def _overridePythonScripts():
    root = mtoaPackageRoot()
    maya_version = cmds.about(version=True)
    path = os.path.join(root, maya_version)
    if not os.path.isdir(path):
        return
    sys.path.insert(0, path)
    # for root, dirnames, filenames in os.walk('path'): 
    for f in os.listdir(path):
        if f.endswith('.py'):
            print>>sys.__stdout__, "Maya %s importing * from Python override %s from %s" % (maya_version, f, path)
            print "Maya %s importing * from Python override %s from %s" % (maya_version, f, path)
            import_string = "from %s import *" % os.path.splitext(f)[0]
            exec import_string
            # module = __import__(os.path.splitext(f)[0])
'''

def addSpreadSheetHooks():

    arnoldAttrList = "{\"aiOpaque\", \"aiMatte\", \"aiVisibleInDiffuseReflection\", \"aiVisibleInSpecularReflection\", \"aiVisibleInSpecularTransmission\", "
    arnoldAttrList += "\"aiVisibleInDiffuseTransmission\", \"aiVisibleInVolume\", \"aiSelfShadows\", \"aiExportTangents\", \"aiExportColors\", "
    arnoldAttrList += "\"aiExportRefPoints\", \"aiExportRefNormals\", \"aiExportRefTangents\", \"aiMotionVectorScale\", \"aiSubdivType\", \"aiSubdivIterations\", "
    arnoldAttrList += "\"aiSubdivAdaptiveMetric\", \"aiSubdivPixelError\", \"aiSubdivAdaptiveSpace\", \"aiSubdivUvSmoothing\", \"aiSubdivSmoothDerivs\", "
    arnoldAttrList += "\"aiDispHeight\", \"aiDispPadding\", \"aiDispZeroValue\", \"aiDispAutobump\", \"aiStepSize\", \"aiVolumePadding\", "
    arnoldAttrList += "\"aiSampleRate\", \"aiCurveWidth\", \"aiCurveShader\", \"aiExportRefPoints\", \"aiMinPixelWidth\", \"aiMode\", "
    arnoldAttrList += "\"standInDrawOverride\", \"overrideNodes\", "
    arnoldAttrList += "\"overrideLightLinking\", \"overrideShaders\", \"overrideReceiveShadows\", \"overrideSelfShadows\", \"overrideOpaque\", \"overrideMatte\", "
    arnoldAttrList += "\"aiColorTemperature\", \"aiExposure\", \"aiAngle\", \"aiSamples\", \"aiNormalize\", \"aiCastShadows\", \"aiShadowDensity\", "
    arnoldAttrList += "\"aiCastVolumetricShadows\", \"aiVolumeSamples\", \"aiDiffuse\", \"aiSpecular\", \"aiSss\", \"aiIndirect\", \"aiVolume\", "
    arnoldAttrList += "\"colorR\", \"colorG\", \"colorB\", \"intensity\", \"aiExposure\", \"aiSpread\", \"aiResolution\", \"aiSoftEdge\", \"aiSamples\", \"aiNormalize\", "
    arnoldAttrList += "\"aiFormat\", \"aiPortalMode\"}"
        
    if cmds.optionVar(exists='SSEitem1'):
        previousSSEitem1 = cmds.optionVar(q='SSEitem1')
        previousSSEitem2 = cmds.optionVar(q='SSEitem2')

        ind = 0
        for item in previousSSEitem1:
            if item != "Arnold":
                ind = ind+1
                continue

            attrList = previousSSEitem2[ind]
            if attrList == arnoldAttrList:
                # same attributes, we can get out of here
                return

            # the attribute list is different, let's remove the items
            cmds.optionVar(removeFromArray=("SSEitem1", ind))
            cmds.optionVar(removeFromArray=("SSEitem2", ind))
            cmds.optionVar(removeFromArray=("SSEitem3", ind))
            break
        
    cmds.optionVar(stringValueAppend=("SSEitem1", "Arnold"))
    cmds.optionVar(stringValueAppend=("SSEitem2", arnoldAttrList))
    cmds.optionVar(intValueAppend=("SSEitem3", 1))
    

# We need to override this two proc to avoid
# errors because of the hardcoded code.
def updateMayaImageFormatControl():
    #mel.eval("source createMayaSoftwareCommonGlobalsTab.mel")
    currentRenderer = utils.currentRenderer()
    if currentRenderer == 'mentalRay':
        mel.eval("updateMentalRayImageFormatControl()")
    elif currentRenderer == 'arnold':
        mtoa.ui.globals.common.updateArnoldImageFormatControl()
    else:
        mel.eval("updateMayaSoftwareImageFormatControl()")

    if currentRenderer != 'arnold':
        mel.eval("updateMultiCameraBufferNamingMenu()")

def renderSettingsTabLabel_melToUI(smel):

    # The arguments passed inside this procedure should not
    # be localized. This procedure uses the first string
    # argument that is passed with the "-addGlobalsTab"
    # flag in the "renderer" command.
    try:

        tabUI = {
            'Common'             : "m_unifiedRenderGlobalsWindow.kCommon",
            'Passes'             : "m_unifiedRenderGlobalsWindow.kPassesTab",
            'Maya Software'      : "m_unifiedRenderGlobalsWindow.kMayaSoftware",
            'Maya Hardware'      : "m_unifiedRenderGlobalsWindow.kMayaHardware",
            'Maya Vector'        : "m_unifiedRenderGlobalsWindow.kMayaVector",
            'Features'           : "m_unifiedRenderGlobalsWindow.kFeatures",
            'Quality'            : "m_unifiedRenderGlobalsWindow.kQuality",
            'Indirect Lighting'  : "m_unifiedRenderGlobalsWindow.kIndirectLighting",
            'Options'            : "m_unifiedRenderGlobalsWindow.kOptions"
            }[smel]


        result = mel.eval("uiRes "+tabUI)
    except:
        result = smel
        mel.eval('uiToMelMsg renderSettingsTabLabel_melToUI "{}" 0'.format(smel))
        
    return result

def addOneTabToGlobalsWindow(renderer, tabLabel, createProc):
    # Check to see if the unified render globals window existed.
    # If it does not exist, then we don't need to add any tab yet.
    if not cmds.window('unifiedRenderGlobalsWindow', exists=True):
        try:
            cmds.error(mel.eval("uiRes m_unifiedRenderGlobalsWindow.kCannotAddTabs"))
        except:
            pass
        return
    
    displayAllTabs = mel.eval("isDisplayingAllRendererTabs()")

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
    if displayAllTabs:
        renderVal = mu.getVar('gMasterLayerRendererName')
        tabLayoutName = mel.eval('getRendererTabLayout "{}"'.format(renderVal))
    else:
        tabLayoutName = mel.eval('getRendererTabLayout "{}"'.format(renderer))

    cmds.setParent(tabLayoutName)

    # The tabName is the tabLabel with the white space removed
    # and the word "Tab" added to the end.
    # "masterLayer" will act as the renderer name if the tab
    # is in the master layer.
    tabName = mel.eval('rendererTabName "{}" "{}"'.format(renderer, tabLabel))

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
                          'createArnoldRendererGlobalsTab',
                          'createArnoldRendererSystemTab',
                          'createArnoldRendererDiagnosticsTab']

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

def _register():
    args = {}
    args['renderProcedure'] = utils.pyToMelProc(arnoldRender.arnoldRender,
                                          [('int', 'width'), ('int', 'height'),
                                           ('int', 'doShadows'), ('int', 'doGlowPass'),
                                           ('string', 'camera'), ('string', 'options')])
    args['renderRegionProcedure'] = 'mayaRenderRegion'

    maya_version = cmds.about(version=True)
    if int(float(maya_version)) >= 2017:
        args['renderSequenceProcedure'] = utils.pyToMelProc(arnoldRender.arnoldSequenceRender,
                                          [('int', 'width'), ('int', 'height'),
                                           ('string', 'camera'), ('string', 'saveToRenderView')])

    args['commandRenderProcedure']    = utils.pyToMelProc(arnoldRender.arnoldBatchRender,
                                                    [('string', 'option')])
    args['batchRenderProcedure']        = utils.pyToMelProc(arnoldRender.arnoldBatchRender,
                                                    [('string', 'option')])
    args['batchRenderOptionsStringProcedure'] = utils.pyToMelProc(arnoldRender.arnoldBatchRenderOptionsString, returnType='string')
    args['cancelBatchRenderProcedure']  = utils.pyToMelProc(arnoldRender.arnoldBatchStop)
    args['iprRenderProcedure']          = utils.pyToMelProc(arnoldRender.arnoldIprRender,
                                                    [('int', 'width'), ('int', 'height'),
                                                     ('int', 'doShadows'), ('int', 'doGlowPass'),
                                                     ('string', 'camera')])
    args['isRunningIprProcedure']       = utils.pyToMelProc(arnoldRender.arnoldIprIsRunning, returnType='int')
    args['startIprRenderProcedure']     = utils.pyToMelProc(arnoldRender.arnoldIprStart,
                                                    [('string', 'editor'), ('int', 'resolutionX'),
                                                     ('int', 'resolutionY'), ('string', 'camera')])
    args['stopIprRenderProcedure']      = utils.pyToMelProc(arnoldRender.arnoldIprStop)
    args['refreshIprRenderProcedure']   = utils.pyToMelProc(arnoldRender.arnoldIprRefresh)
    args['pauseIprRenderProcedure']     =   utils.pyToMelProc(arnoldRender.arnoldIprPause,
                                                    [('string', 'editor'), ('int', 'pause')])
    args['changeIprRegionProcedure']    = utils.pyToMelProc(arnoldRender.arnoldIprChangeRegion,
                                                    [('string', 'renderPanel')])
    cmds.renderer('arnold', rendererUIName='Arnold Renderer', **args)

    if int(float(maya_version)) > 2018:
        cmds.renderer('arnold', edit=True, supportColorManagement=True)

    aiRenderSettingsBuiltCallback("arnold")
    cmds.renderer('arnold', edit=True, addGlobalsNode='defaultArnoldRenderOptions')
    cmds.renderer('arnold', edit=True, addGlobalsNode='defaultArnoldDriver')
    cmds.renderer('arnold', edit=True, addGlobalsNode='defaultArnoldFilter')
    utils.pyToMelProc(updateBackgroundSettings, useName=True)
    utils.pyToMelProc(updateAtmosphereSettings, useName=True)
    #We have to source this file otherwise maya will override
    #our mel proc overrides below.
    #
    mel.eval('source "createMayaSoftwareCommonGlobalsTab.mel"')
    
    if int(float(maya_version)) < 2018:
        utils.pyToMelProc(addOneTabToGlobalsWindow,
                          [('string', 'renderer'), ('string', 'tabLabel'), ('string', 'createProc')],
                          useName=True)
    utils.pyToMelProc(renderSettingsTabLabel_melToUI,
                      [('string', 'mel')],
                      useName=True)
    utils.pyToMelProc(updateMayaImageFormatControl,
                      useName=True)

def registerArnoldRenderer():
    try:
        alreadyRegistered = cmds.renderer('arnold', exists=True)
        if not alreadyRegistered:

            cmds.evalDeferred(_register)

            # AE Templates
            # the following must occur even in batch mode because they contain calls to registerDefaultTranslator
            cmds.evalDeferred(aeUtils.loadAETemplates)
            import rendererCallbacks
            rendererCallbacks.registerCallbacks()
            import mtoa.ui.ae.customShapeAttributes
            import mtoa.ui.ae.customShaderTemplates
            if not cmds.about(batch=True):
                # Reload the AE Window if it has already been opened
                cmds.evalDeferred(aeUtils.rebuildAE)
                # create the Arnold menu
                createArnoldMenu()

            # FIXME: This doesn't seem to be used anymore
            # version specific overrides or additions
            #_overridePythonScripts()
            #_overrideMelScripts()

            # Add option box for file translator
            utils.pyToMelProc(exportass.arnoldAssOpts,
                              [('string', 'parent'), ('string', 'action'),
                               ('string', 'initialSettings'), ('string', 'resultCallback')],
                               useName=True)
            
            # callbacks
            import mtoa.core as core
            core.installCallbacks()
            core.MTOA_GLOBALS['COMMAND_PORT'] = None

            if not cmds.about(batch=True):
                commandPortBase = 4700
                try:
                    commandPortBase = int(os.environ['MTOA_COMMAND_PORT'])
                except:
                    commandPortBase = 4700
                # opening a command port for different tools and maya batch progress messages
                for port in range(commandPortBase, commandPortBase + 100):
                    commandPortName = ':%i' % port
                    try:
                        cmds.commandPort(name=commandPortName)
                        core.MTOA_GLOBALS['COMMAND_PORT'] = port
                        break
                    except:
                        pass
            if not cmds.about(batch=True):
                cmds.evalDeferred(arnoldShelf.createArnoldShelf)

            addSpreadSheetHooks()


    except:
        import traceback
        traceback.print_exc(file=sys.__stderr__)
        raise


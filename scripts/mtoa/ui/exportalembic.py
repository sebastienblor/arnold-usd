"""
BulletBaking - Python module containing bullet baking commands
    for use with MayaBullet.

"""
# Python
import os
import re
import types
# Maya
import maya.cmds as cmds
import maya.mel as mel
import maya.api.OpenMaya as OpenMaya


############################### Global Variables ###########################
gAbcArnoldExportLastDirectory=""
gAbcArnoldExportLastWorkspace=""

############################### BulletExport ###############################

MTOA_ATTR_PREFIX = 'mtoa_'

def containsWhiteSpace(str):
    pattern = re.compile(r'\s+')
    return pattern.match(str)


def formValidObjectName(string):
    return re.sub(r'[^a-zA-Z0-9_]', '_', string)


def setOptionVars(forceFactorySettings=False):

    if forceFactorySettings or not cmds.optionVar(exists="Alembic_exportCacheTimeRanges") :
        cmds.optionVar( clearArray="Alembic_exportCacheTimeRanges" )
        cmds.optionVar( intValueAppend=["Alembic_exportCacheTimeRanges", 2])

    if forceFactorySettings or not cmds.optionVar(exists="Alembic_exportStarts") :
        cmds.optionVar( clearArray="Alembic_exportStarts" )
        cmds.optionVar( floatValueAppend=["Alembic_exportStarts", 1] )

    if forceFactorySettings or not cmds.optionVar(exists="Alembic_exportEnds") :
        cmds.optionVar( clearArray="Alembic_exportEnds" )
        cmds.optionVar( floatValueAppend=["Alembic_exportEnds", 10])

    if forceFactorySettings or not cmds.optionVar(exists="Alembic_exportEvaluateEverys") :
        cmds.optionVar( clearArray="Alembic_exportEvaluateEverys" )
        cmds.optionVar( floatValueAppend=["Alembic_exportEvaluateEverys", 1])

    if forceFactorySettings or not cmds.optionVar(exists="Alembic_exportEnableFrameRelativeSamples") :
        cmds.optionVar( clearArray="Alembic_exportEnableFrameRelativeSamples" )
        cmds.optionVar( intValueAppend=["Alembic_exportEnableFrameRelativeSamples", 0])

    if forceFactorySettings or not cmds.optionVar(exists="Alembic_exportLowFrameRelativeSamples") :
        cmds.optionVar( clearArray="Alembic_exportLowFrameRelativeSamples" )
        cmds.optionVar( floatValueAppend=["Alembic_exportLowFrameRelativeSamples", -0.2])

    if forceFactorySettings or not cmds.optionVar(exists="Alembic_exportHighFrameRelativeSamples") :
        cmds.optionVar( clearArray="Alembic_exportHighFrameRelativeSamples" )
        cmds.optionVar( floatValueAppend=["Alembic_exportHighFrameRelativeSamples", 0.2])

    if forceFactorySettings or not cmds.optionVar(exists="Alembic_exportEnablePreRoll") :
        cmds.optionVar( intValue=["Alembic_exportEnablePreRoll", 0])

    if forceFactorySettings or not cmds.optionVar(exists="Alembic_exportPreRollStartFrame") :
        cmds.optionVar( floatValue=["Alembic_exportPreRollStartFrame", 0])

    if forceFactorySettings or not cmds.optionVar(exists="Alembic_exportAttr") :
        cmds.optionVar( stringValue=["Alembic_exportAttr", ""])

    if forceFactorySettings or not cmds.optionVar(exists="Alembic_exportAttrPrefix") :
        cmds.optionVar( stringValue=["Alembic_exportAttrPrefix", ""])

    if forceFactorySettings or not cmds.optionVar(exists="Alembic_exportVerbose") :
        cmds.optionVar( intValue=["Alembic_exportVerbose", 0])

    if forceFactorySettings or not cmds.optionVar(exists="Alembic_exportNoNormals") :
        cmds.optionVar( intValue=["Alembic_exportNoNormals", 0])

    if forceFactorySettings or not cmds.optionVar(exists="Alembic_exportRenderableOnly") :
        cmds.optionVar( intValue=["Alembic_exportRenderableOnly", 0])

    if forceFactorySettings or not cmds.optionVar(exists="Alembic_exportStripNamespaces") :
        cmds.optionVar( intValue=["Alembic_exportStripNamespaces", 0])

    if forceFactorySettings or not cmds.optionVar(exists="Alembic_exportUVWrite") :
        cmds.optionVar( intValue=["Alembic_exportUVWrite", 0])

    if forceFactorySettings or not cmds.optionVar(exists="Alembic_exportWholeFrameGeo") :
        cmds.optionVar( intValue=["Alembic_exportWholeFrameGeo", 0])

    if forceFactorySettings or not cmds.optionVar(exists="Alembic_exportWorldSpace") :
        cmds.optionVar( intValue=["Alembic_exportWorldSpace", 0])

    if forceFactorySettings or not cmds.optionVar(exists="Alembic_exportWriteVisibility") :
        cmds.optionVar( intValue=["Alembic_exportWriteVisibility", 0])

    if forceFactorySettings or not cmds.optionVar(exists="Alembic_exportPerFrameCallbackMel") :
        cmds.optionVar( stringValue=["Alembic_exportPerFrameCallbackMel", ""])

    if forceFactorySettings or not cmds.optionVar(exists="Alembic_exportPostJobCallbackMel") :
        cmds.optionVar( stringValue=["Alembic_exportPostJobCallbackMel", ""])

    if forceFactorySettings or not cmds.optionVar(exists="Alembic_exportPerFrameCallbackPython") :
        cmds.optionVar( stringValue=["Alembic_exportPerFrameCallbackPython", ""])

    if forceFactorySettings or not cmds.optionVar(exists="Alembic_exportPostJobCallbackPython") :
        cmds.optionVar( stringValue=["Alembic_exportPostJobCallbackPython", ""])

    # version 2
    #
    if forceFactorySettings or not cmds.optionVar(exists="Alembic_exportFilterEulerRotations") :
        cmds.optionVar( intValue=["Alembic_exportFilterEulerRotations", 0])

    # version 3
    #
    if forceFactorySettings or not cmds.optionVar(exists="Alembic_exportWriteColorSets") :
        cmds.optionVar( intValue=["Alembic_exportWriteColorSets", 0])

    if forceFactorySettings or not cmds.optionVar(exists="Alembic_exportWriteFaceSets") :
        cmds.optionVar( intValue=["Alembic_exportWriteFaceSets", 0])

    # version 4
    #
    if (forceFactorySettings or not bool(cmds.optionVar(exists="Alembic_exportDataFormat"))) :
        cmds.optionVar( intValue=["Alembic_exportDataFormat", 2])
    

    # version 5
    #
    if (forceFactorySettings or not bool(cmds.optionVar(exists="Alembic_exportPreRollStep"))) :
        cmds.optionVar( floatValue=["Alembic_exportPreRollStep", 1])
    
    # version 6
    #
    if (forceFactorySettings or not bool(cmds.optionVar(exists="Alembic_exportWriteCreases"))) :
        cmds.optionVar( intValue=["Alembic_exportWriteCreases", 0])
    

def captureAlembicExportOptionVars(*args, **kw):
    version = args[0]

    setOptionVars(0)

    cacheTimeRanges         = cmds.optionVar(q="Alembic_exportCacheTimeRanges")
    startFrames             = cmds.optionVar(q="Alembic_exportStarts")
    endFrames               = cmds.optionVar(q="Alembic_exportEnds")
    evaluateEvery           = cmds.optionVar(q="Alembic_exportEvaluateEverys")
    enableSample            = cmds.optionVar(q="Alembic_exportEnableFrameRelativeSamples")
    lowFrameRelativeSamples = cmds.optionVar(q="Alembic_exportLowFrameRelativeSamples")
    highFrameRelativeSamples= cmds.optionVar(q="Alembic_exportHighFrameRelativeSamples")
    enablePreRoll           = cmds.optionVar(q="Alembic_exportEnablePreRoll")
    preRollStartFrame       = cmds.optionVar(q="Alembic_exportPreRollStartFrame")
    attr                    = cmds.optionVar(q="Alembic_exportAttr")
    attrPrefix              = cmds.optionVar(q="Alembic_exportAttrPrefix")
    verbose                 = cmds.optionVar(q="Alembic_exportVerbose")
    noNormals               = cmds.optionVar(q="Alembic_exportNoNormals")
    renderableOnly          = cmds.optionVar(q="Alembic_exportRenderableOnly")
    stripNamespaces         = cmds.optionVar(q="Alembic_exportStripNamespaces")
    uvWrite                 = cmds.optionVar(q="Alembic_exportUVWrite")
    wholeFrameGeo           = cmds.optionVar(q="Alembic_exportWholeFrameGeo")
    worldSpace              = cmds.optionVar(q="Alembic_exportWorldSpace")
    writeVisibility         = cmds.optionVar(q="Alembic_exportWriteVisibility")
    perFrameCallbackMel     = cmds.optionVar(q="Alembic_exportPerFrameCallbackMel")
    postJobCallbackMel      = cmds.optionVar(q="Alembic_exportPostJobCallbackMel")
    perFrameCallbackPython  = cmds.optionVar(q="Alembic_exportPerFrameCallbackPython")
    postJobCallbackPython   = cmds.optionVar(q="Alembic_exportPostJobCallbackPython")

    optionArgs = [
        exportAll,
        cacheTimeRanges,
        startFrames,
        endFrames,
        evaluateEvery,
        enableSample,
        lowFrameRelativeSamples,
        highFrameRelativeSamples,
        enablePreRoll,
        preRollStartFrame,
        attr,
        attrPrefix,
        verbose,
        noNormals,
        renderableOnly,
        stripNamespaces,
        uvWrite,
        wholeFrameGeo,
        worldSpace,
        writeVisibility,
        perFrameCallbackMel,
        postJobCallbackMel,
        perFrameCallbackPython,
        postJobCallbackPython
        ]

    if (version >= 2) :
        filterEulerRotations    = cmds.optionVar(q="Alembic_exportFilterEulerRotations")

        optionArgs.append(str(filterEulerRotations))

    if (version >= 3) :
        writeColorSets    = cmds.optionVar(q="Alembic_exportWriteColorSets")
        writeFaceSets      = cmds.optionVar(q="Alembic_exportWriteFaceSets")

        optionArgs.append(writeColorSets)
        optionArgs.append(writeFaceSets)

    if (version >= 4) :
        dataFormat = cmds.optionVar(q="Alembic_exportDataFormat")

        optionArgs.append(dataFormat)

    if (version >= 5) :
        preRollStep = cmds.optionVar(q="Alembic_exportPreRollStep")

        optionArgs.append(preRollStep)

    if (version >= 6) :
        writeCreases = cmds.optionVar(q="Alembic_exportWriteCreases")

        optionArgs.append(writeCreases)

    return optionArgs


def syncOptionVars(versionNum, args):

    cacheTimeRanges         = args[1] if len(args)>1 and args[1] else []
    startFrames             = args[2] if len(args)>2 and args[2] else []
    endFrames               = args[3] if len(args)>3 and args[3] else []
    evaluateEvery           = args[4] if len(args)>4 and args[4] else []
    enableSample            = args[5] if len(args)>5 and args[5] else []
    lowFrameRelativeSamples = args[6] if len(args)>6 and args[6] else []
    highFrameRelativeSamples= args[7] if len(args)>7 and args[7] else []

    enablePreRoll           = args[8] if len(args)>8 else None
    preRollStartFrame       = args[9] if len(args)>9 else None
    attr                    = args[10] if len(args)>10 else None
    attrPrefix              = args[11] if len(args)>11 else None
    verbose                 = args[12] if len(args)>12 else None
    noNormals               = args[13] if len(args)>13 else None
    renderableOnly          = args[14] if len(args)>14 else None
    stripNamespaces         = args[15] if len(args)>15 else None
    uvWrite                 = args[16] if len(args)>16 else None
    wholeFrameGeo           = args[17] if len(args)>17 else None
    worldSpace              = args[18] if len(args)>18 else None
    writeVisibility         = args[19] if len(args)>19 else None
    perFrameCallbackMel     = args[20] if len(args)>20 else None
    postJobCallbackMel      = args[21] if len(args)>21 else None
    perFrameCallbackPython  = args[22] if len(args)>22 else None
    postJobCallbackPython   = args[23] if len(args)>23 else None

    for i, (tmpRangeMode, tmpStart, tmpEnd, tmpEvaluateEvery,tmpEnableSample,tmpLowSample, tmpHighSample) \
        in enumerate(zip(cacheTimeRanges, startFrames, endFrames, evaluateEvery, enableSample, lowFrameRelativeSamples, highFrameRelativeSamples)):

        if i==0:
            cmds.optionVar( clearArray="Alembic_exportCacheTimeRanges" )
            cmds.optionVar( clearArray="Alembic_exportStarts" )
            cmds.optionVar( clearArray="Alembic_exportEnds" )
            cmds.optionVar( clearArray="Alembic_exportEvaluateEverys" )
            cmds.optionVar( clearArray="Alembic_exportEnableFrameRelativeSamples" )
            cmds.optionVar( clearArray="Alembic_exportLowFrameRelativeSamples" )
            cmds.optionVar( clearArray="Alembic_exportHighFrameRelativeSamples" )

        cmds.optionVar( intValueAppend=    ["Alembic_exportCacheTimeRanges",            int(tmpRangeMode)])
        cmds.optionVar( floatValueAppend=  ["Alembic_exportStarts",                     float(tmpStart)])
        cmds.optionVar( floatValueAppend=  ["Alembic_exportEnds",                       float(tmpEnd)])
        cmds.optionVar( floatValueAppend=  ["Alembic_exportEvaluateEverys",             float(tmpEvaluateEvery)])
        cmds.optionVar( intValueAppend=    ["Alembic_exportEnableFrameRelativeSamples", int(tmpEnableSample)])
        cmds.optionVar( floatValueAppend=  ["Alembic_exportLowFrameRelativeSamples",    float(tmpLowSample)])
        cmds.optionVar( floatValueAppend=  ["Alembic_exportHighFrameRelativeSamples",   float(tmpHighSample)])

    if enablePreRoll:
        cmds.optionVar( intValue=      ["Alembic_exportEnablePreRoll",         int(enablePreRoll)])
    if preRollStartFrame:
        cmds.optionVar( floatValue=    ["Alembic_exportPreRollStartFrame",     float(preRollStartFrame)])
    if attr:
        cmds.optionVar( stringValue=   ["Alembic_exportAttr",                  attr])
    if attrPrefix:
        cmds.optionVar( stringValue=   ["Alembic_exportAttrPrefix",            attrPrefix])
    if verbose:
        cmds.optionVar( intValue=      ["Alembic_exportVerbose",               int(verbose)])
    if noNormals:
        cmds.optionVar( intValue=      ["Alembic_exportNoNormals",             int(noNormals)])
    if renderableOnly:
        cmds.optionVar( intValue=      ["Alembic_exportRenderableOnly",        int(renderableOnly)])
    if stripNamespaces:
        cmds.optionVar( intValue=      ["Alembic_exportStripNamespaces",       int(stripNamespaces)])
    if uvWrite:
        cmds.optionVar( intValue=      ["Alembic_exportUVWrite",               int(uvWrite)])
    if wholeFrameGeo:
        cmds.optionVar( intValue=      ["Alembic_exportWholeFrameGeo",         int(wholeFrameGeo)])
    if worldSpace:
        cmds.optionVar( intValue=      ["Alembic_exportWorldSpace",            int(worldSpace)])
    if writeVisibility:
        cmds.optionVar( intValue=      ["Alembic_exportWriteVisibility",       int(writeVisibility)])
    if perFrameCallbackMel:
        cmds.optionVar( stringValue=   ["Alembic_exportPerFrameCallbackMel",   perFrameCallbackMel])
    if postJobCallbackMel:
        cmds.optionVar( stringValue=   ["Alembic_exportPostJobCallbackMel",    postJobCallbackMel])
    if perFrameCallbackPython:
        cmds.optionVar( stringValue=   ["Alembic_exportPerFrameCallbackPython",perFrameCallbackPython])
    if postJobCallbackPython:
        cmds.optionVar( stringValue=   ["Alembic_exportPostJobCallbackPython", postJobCallbackPython])

    if (versionNum >= 2) :
        filterEulerRotations    = args[24] if len(args)>24 else None
        if filterEulerRotations:
            cmds.optionVar( intValue=  ["Alembic_exportFilterEulerRotations",  filterEulerRotations])

    if (versionNum >= 3) :
        writeColorSets      = args[25] if len(args)>25 else None
        writeFaceSets       = args[26] if len(args)>26 else None
        if writeColorSets:
            cmds.optionVar( intValue=  ["Alembic_exportWriteColorSets",    writeColorSets])
        if writeFaceSets:
            cmds.optionVar( intValue=  ["Alembic_exportWriteFaceSets",     writeFaceSets])

    if (versionNum >= 4):
        dataFormat = args[27] if len(args)>25 else None
        if dataFormat:
            cmds.optionVar( intValue=  ["Alembic_exportDataFormat",        dataFormat])

    if (versionNum >= 5):
        preRollStep = args[28]
        if preRollStep:
            cmds.optionVar( floatValue=    ["Alembic_exportPreRollStep",       preRollStep])

    if (versionNum >= 6):
        writeCreases = args[29]
        if writeCreases:
            cmds.optionVar( intValue=  ["Alembic_exportWriteCreases",      writeCreases])

    if (versionNum >= 7):
        writeUVSets = args[30]
        if writeCreases:
            cmds.optionVar( intValue=  ["Alembic_exportWriteUVSets",      writeUVSets])


def getObjectsToExport(*args, **kw):
    result = set([])

    sl = kw['sl'] if kw.has_key('sl') else False
    slist = cmds.ls( sl=sl, long=True )

    result = result.union(set(slist))

    return result


def doExportArgList(*args, **kw):
    # back up the current option values so that we can restore
    # them later if the dialog is cancelled
    exportAll = kw['exportAll'] if kw.has_key('exportAll') else False
    version = kw['version'] if kw.has_key('version') else 1

    optionVarsBackup = captureAlembicExportOptionVars(version, exportAll)

    # synchronize the option values with the argument list
    syncOptionVars(version, args)

    # prepare filter and starting dir for file dialog
    filter = "Alembic (*.abc);; All Files (*.*)"
    if (len(cmds.workspace(fileRuleEntry='alembicCache')) == 0) :
        cmds.workspace( fileRule=["alembicCache","cache/alembic"] )
        cmds.workspace( saveWorkspace=True )

    workspace = cmds.workspace( fileRuleEntry='alembicCache')
    workspace = cmds.workspace( expandName=workspace )
    cmds.sysFile( workspace, makeDir=True )

    global gAbcArnoldExportLastDirectory
    global gAbcArnoldExportLastWorkspace
    startingDir = gAbcArnoldExportLastDirectory
    if (len(startingDir) == 0 or gAbcArnoldExportLastWorkspace != cmds.workspace(q=True, rootDirectory=True)) :
        startingDir = workspace

    # choose a file to export
    result=[]
    if (exportAll) :
        result = cmds.fileDialog2(
                    returnFilter=1,
                    fileFilter=filter,
                    caption="Export All",
                    startingDirectory=startingDir,
                    fileMode=0,
                    okCaption="Export All",
                    optionsUICreate="Alembic_exportFileOptionsUICreate",
                    optionsUIInit="Alembic_exportFileOptionsUIInit",
                    optionsUICommit="Alembic_exportAllFileOptionsUICommit"
                    )
    else:
        result = cmds.fileDialog2(
                    returnFilter=1,
                    fileFilter=filter,
                    caption="Export Selection",
                    startingDirectory=startingDir,
                    fileMode=0,
                    okCaption="Export Selection",
                    optionsUICreate="Alembic_exportFileOptionsUICreate",
                    optionsUIInit="Alembic_exportFileOptionsUIInit",
                    optionsUICommit="Alembic_exportSelectionFileOptionsUICommit",
                    )

    if (not result or len(result) == 0 or len(result[0]) == 0) :
        # cancelled
        # Restore optionVars to the state before this procedure is called
        #
        syncOptionVars(version, optionVarsBackup)
        return


    # Save the last directory
    gAbcArnoldExportLastDirectory = os.path.dirname(result[0])
    gAbcArnoldExportLastWorkspace = cmds.workspace(q=True, rootDirectory=True)

    # parameters
    cacheTimeRanges             = cmds.optionVar(q='Alembic_exportCacheTimeRanges')
    startFrames                 = cmds.optionVar(q='Alembic_exportStarts')
    endFrames                   = cmds.optionVar(q='Alembic_exportEnds')
    evaluateEverys              = cmds.optionVar(q='Alembic_exportEvaluateEverys')
    enableSamples               = cmds.optionVar(q='Alembic_exportEnableFrameRelativeSamples')
    lowFrameRelativeSamples     = cmds.optionVar(q='Alembic_exportLowFrameRelativeSamples')
    highFrameRelativeSamples        = cmds.optionVar(q='Alembic_exportHighFrameRelativeSamples')

    enablePreRoll               = bool(cmds.optionVar(q='Alembic_exportEnablePreRoll'))
    preRollStartFrame           = float(cmds.optionVar(q='Alembic_exportPreRollStartFrame'))
    preRollStep                 = float(cmds.optionVar(q='Alembic_exportPreRollStep'))
    attr                        = cmds.optionVar(q='Alembic_exportAttr')
    attrPrefix                  = cmds.optionVar(q='Alembic_exportAttrPrefix')
    verbose                     = bool(cmds.optionVar(q='Alembic_exportVerbose'))
    noNormals                   = bool(cmds.optionVar(q='Alembic_exportNoNormals'))
    renderableOnly              = bool(cmds.optionVar(q='Alembic_exportRenderableOnly'))
    stripNamespaces             = bool(cmds.optionVar(q='Alembic_exportStripNamespaces'))
    uvWrite                     = bool(cmds.optionVar(q='Alembic_exportUVWrite'))
    writeColorSets              = bool(cmds.optionVar(q='Alembic_exportWriteColorSets'))
    writeFaceSets               = bool(cmds.optionVar(q='Alembic_exportWriteFaceSets'))
    wholeFrameGeo               = bool(cmds.optionVar(q='Alembic_exportWholeFrameGeo'))
    worldSpace                  = bool(cmds.optionVar(q='Alembic_exportWorldSpace'))
    writeVisibility             = bool(cmds.optionVar(q='Alembic_exportWriteVisibility'))
    filterEulerRotations        = bool(cmds.optionVar(q='Alembic_exportFilterEulerRotations'))
    writeCreases                = bool(cmds.optionVar(q='Alembic_exportWriteCreases'))
    writeUVSets                 = bool(cmds.optionVar(q='Alembic_exportWriteUVSets'))
    dataFormat                  = int(cmds.optionVar(q='Alembic_exportDataFormat'))
    perFrameCallbackMel         = cmds.optionVar(q='Alembic_exportPerFrameCallbackMel')
    postJobCallbackMel              = cmds.optionVar(q='Alembic_exportPostJobCallbackMel')
    perFrameCallbackPython      = cmds.optionVar(q='Alembic_exportPerFrameCallbackPython')
    postJobCallbackPython       = cmds.optionVar(q='Alembic_exportPostJobCallbackPython')

    # build AbcExport command
    command = "arnoldExportAlembic "
    job = ""

    firstCacheFrame = None
    startEnd = [firstCacheFrame,firstCacheFrame]

    for cacheTimeRange, startFrame, endFrame, evaluateEvery, enableSample, lowSample, highSample \
        in zip(cacheTimeRanges, startFrames, endFrames, evaluateEverys, enableSamples, lowFrameRelativeSamples, highFrameRelativeSamples):

        if (cacheTimeRange != 3):
            startEnd = mel.eval('Alembic_getStartEndFrames({0});'.format(cacheTimeRange))
        else :
            startEnd[0] = startFrame
            startEnd[1] = endFrame

        if (firstCacheFrame == None):
            firstCacheFrame = startEnd[0]

        job += "-frameRange {0} {1} ".format(startEnd[0],startEnd[1])

        if (evaluateEvery != 1) :
            if (evaluateEvery <= 0) :
                OpenMaya.MGlobal.displayError( "Evaluate every should be greater than 0" )
                return

            job += "-step {0} ".format(evaluateEvery)

        if (enableSample) :
            if (lowSample > 0 or lowSample < -evaluateEvery) :
                OpenMaya.MGlobal.displayError( "Invalid low frame relative sample" )
                return

            if (highSample < 0 or highSample > evaluateEvery) :
                OpenMaya.MGlobal.displayError(  "Invalid high frame relative sample" )
                return

            job += "-frameRelativeSample {0} ".format(lowSample)
            job += "-frameRelativeSample 0 -frameRelativeSample {0} ".format(highSample)

    if (enablePreRoll) :
        # Check arguments.
        if (preRollStep <= 0) :
            OpenMaya.MGlobal.displayError( "Pre-roll step should be greater than 0" )
            return

        if (preRollStartFrame >= firstCacheFrame) :
            OpenMaya.MGlobal.displayError( "Pre-roll start frame should be less than start frame" )
            return

        # Compute the pre-roll end frame.
        preRollEndFrame = mel.eval('Alembic_getPrerollEndFrame({preRollStartFrame}, {firstCacheFrame}, {preRollStep});'
                                  .format(preRollStartFrame=preRollStartFrame,firstCacheFrame=firstCacheFrame,preRollStep=preRollStep))

        preRollFlags = "-frameRange {0} {1} -step {2} -preRoll ".format(preRollStartFrame, preRollEndFrame, preRollStep)
        job = preRollFlags + job

    attrArray = attr.split(',')
    attrPrefixArray = attrPrefix.split(',')

    for i in attrArray:
        if (len(i) > 0):
            job += "-attr "
            job += formValidObjectName(i)
            job += " "

    for i in attrPrefixArray:
        if (len(i) > 0):
            job += "-attrPrefix "
            job += formValidObjectName(i)
            job += " "

    if (verbose) :
        command += "-verbose "

    if (noNormals) :
        job += "-noNormals "

    if (renderableOnly) :
        job += "-ro "

    if (stripNamespaces) :
        job += "-stripNamespaces "

    if (uvWrite) :
        job += "-uvWrite "

    if (writeColorSets) :
        job += "-writeColorSets "

    if (writeFaceSets) :
        job += "-writeFaceSets "

    if (wholeFrameGeo) :
        job += "-wholeFrameGeo "

    if (worldSpace) :
        job += "-worldSpace "

    if (writeVisibility) :
        job += "-writeVisibility "

    if (filterEulerRotations) :
        job += "-eulerFilter "

    if (writeCreases) :
        job += "-autoSubd "

    if (writeUVSets) :
        job += "-writeUVSets "

    if (dataFormat == 1):
        job += "-dataFormat hdf "
    elif (dataFormat == 2):
        job += "-dataFormat ogawa "

    if (len(perFrameCallbackMel) > 0) :
        if (containsWhiteSpace(perFrameCallbackMel)) :
            perFrameCallbackMel = "\"" + cmds.encodeString(perFrameCallbackMel) + "\"" 

        job += ("-melPerFrameCallback " + cmds.encodeString(perFrameCallbackMel) + " ")

    if (len(postJobCallbackMel) > 0) :
        if (containsWhiteSpace(postJobCallbackMel)) :
            postJobCallbackMel = "\"" + cmds.encodeString(postJobCallbackMel) + "\"" 

        job += ("-melPostJobCallback " + cmds.encodeString(postJobCallbackMel) + " ")

    if (len(perFrameCallbackPython) > 0) :
        if (containsWhiteSpace(perFrameCallbackPython)) :
            perFrameCallbackPython = "\"" + cmds.encodeString(perFrameCallbackPython) + "\"" 

        job += ("-pythonPerFrameCallback " + cmds.encodeString(perFrameCallbackPython) + " ")

    if (len(postJobCallbackPython) > 0) :
        if (containsWhiteSpace(postJobCallbackPython)) :
            postJobCallbackPython = "\"" + cmds.encodeString(postJobCallbackPython) + "\"" 

        job += ("-pythonPostJobCallback " + cmds.encodeString(postJobCallbackPython) + " ")

    # get objects to export
    if not exportAll:
        roots = getObjectsToExport(sl=True)
        for root in roots:
            job += "-root {0} ".format(root)

    file = result[0]
    if (containsWhiteSpace(file)) :
        file = "\"" + file + "\""

    command += ("-j \"{} -file {} \"".format(job, cmds.encodeString(file)))

    # execute command
    print command
    result = mel.eval(command)

    return


def getOptionVar(name, default):
    if cmds.optionVar(exists=name):
        return cmds.optionVar(q=name)
    else:
        return default


def setupExport():

    # main options
    cmds.optionVar(iv=['Alembic_exportUVWrite', 1])
    cmds.optionVar(iv=['Alembic_exportWriteVisibility', 1])
    cmds.optionVar(iv=['Alembic_exportWriteCreases', 1])
    cmds.optionVar(iv=['Alembic_exportWriteUVSets', 1])
    cmds.optionVar(iv=['Alembic_exportWriteColorSets', 1])
    cmds.optionVar(iv=['Alembic_exportWriteFaceSets', 1])

    # callbacks
    cbs = []
    pref_val = getOptionVar('Alembic_exportPerFrameCallbackPython', None)
    if pref_val:
        cbs.append(pref_val)
    pref_val = getOptionVar('Alembic_exportPostJobCallbackPython', None)
    if pref_val:
        cbs.append(pref_val)

    # known attributes
    all_prefixes = []
    pref_val = getOptionVar('Alembic_exportAttrPrefix', '')
    if pref_val:
        all_prefixes = pref_val.split(',')
    if MTOA_ATTR_PREFIX not in all_prefixes:
        all_prefixes.append(MTOA_ATTR_PREFIX)
    cmds.optionVar(sv=['Alembic_exportAttrPrefix', ','.join(all_prefixes)])

    return cbs

def exportSelected( *args, **kw ):
    kw['exportAll']=False
    setupExport()
    doExportArgList( *args, **kw )


def exportAll( *args, **kw ):
    kw['exportAll']=True
    setupExport()
    doExportArgList( *args, **kw )


def exportOptions( export_all ):
    commandName = ""
    label = ""
    command = None
    if (export_all):
        commandName = "ArnoldAlembicExportAll"
        label       = "Export All"
        command = exportSelected
    else:
        commandName = "ArnoldAlembicExportSelection"
        label       = "Export Selection"
        command = exportAll

    layout = mel.eval('getOptionBox')

    mel.eval('setOptionBoxCommandName("{}");'.format(commandName))

    ##  STEP 3:  Create option box contents.
    ##  ====================================
    ##
    parent = exportOptionsUI(layout)

    ## 'Export' button
    ##
    exportBtn = mel.eval('getOptionBoxApplyBtn;')
    cmds.button(exportBtn, e=True, label=label, command=command)

    ## 'Save' button
    ##
    saveBtn = mel.eval('getOptionBoxSaveBtn;')
    cmds.button(saveBtn, e=True, command=command)

    ## 'Reset' button
    ##
    resetBtn = mel.eval('getOptionBoxResetBtn;')
    cmds.button(resetBtn, e=True, command="Alembic_exportSetup {} 0".format(parent))

    ##  STEP 5:  Set the option box title.
    ##  ==================================
    ##
    mel.eval('setOptionBoxTitle("Arnold Alembic Export");')

    ##  STEP 6:  Customize the 'Help' menu item text.
    ##  =============================================
    ##
    # mel.eval('setOptionBoxHelpTag($commandName);

    ##  Set the current values of the option box.
    ##  =========================================
    ##
    mel.eval("Alembic_exportSetup {} 0".format(parent))

    ##  Show the option box.
    ##  ====================
    ##
    mel.eval('showOptionBox')

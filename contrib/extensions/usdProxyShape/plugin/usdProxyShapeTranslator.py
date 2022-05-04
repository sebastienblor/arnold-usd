
## we want to add this directory to the path so we can use the extra xgenArnoldUI  files
from pxr import Usd, UsdUtils, Tf, Vt
import maya.cmds as cmds
import mayaUsd.lib as mayaUsdLib
import maya.api.OpenMaya as OpenMaya
import mtoa.core
import arnold as ai
import mtoa.utils as mtoaUtils

global _stageCacheDict
_stageCacheDict = {}

global _stageNames
_stageNames = {}

global _sessionName
_sessionName = ''

def UsdArnoldUnregisterListener(cacheId):
    global _stageCacheDict
    stageCallback = _stageCacheDict.get(cacheId)
    if stageCallback:
        del stageCallback
        _stageCacheDict.pop(cacheId)
    stageName = _stageNames.get(cacheId)
    if stageName:
        _stageNames.pop(cacheId)

def UsdArnoldListener(cacheId, shapeName, sessionName = 'arnoldRenderView'):
    global _stageCacheDict
    global _stageNames
    global _sessionName

    stageCache = UsdUtils.StageCache.Get()
    id = Usd.StageCache.Id.FromLongInt(cacheId);
    stage = stageCache.Find(id)
    
    stageCallback = _stageCacheDict.get(cacheId)
    if stageCallback:
        return

    _stageCacheDict[cacheId] = Tf.Notice.Register(Usd.Notice.StageContentsChanged, OnStageContentsChanged, stage)
    _stageNames[cacheId] = shapeName
    _sessionName = sessionName

def OnStageContentsChanged(notice, stage):
    # the USD stage has changed, we need to tell MtoA to update the render if an interactive render is going on
    stageCache = UsdUtils.StageCache.Get()
    if not stageCache:
        return
    id = stageCache.GetId(stage)
    cacheId = id.ToLongInt()
    cmds.arnoldScene(_stageNames.get(cacheId), mode='update_selected', session=_sessionName)

### Python scripts for mayaUsd exports
### TODO move this to a separate extension
global _registerTranslators
_registerTranslators = False

global _optionsNode
_optionsNode = None

global _tmpOptions
_tmpOptions = None

global _nodesQueue
_nodesQueue = []

# Create a temporary options node needed for export
def InitOptionsNode():
    global _optionsNode
    if _optionsNode:
        # the options node already exists, nothing to do
        return

    # store a list of maya Objects to be exported
    global _nodesQueue
    _nodesQueue = []

    try:
        # Here we want to create a new "options" node, inheriting from the default one,
        # so that we can override its attributes. It's a bit hacky, but for now it's
        # a simple way to get this to work : We're creating a new aiOptions node, and while 
        # looping over all of its attributes we're connecting them to the corresponding ones
        # in defaultArnoldRenderOptions. This way we can provide this new options node to 
        # "arnoldScene" and the export will be done with this new node. Once the export is
        # finished, we can delete this temporary node.
        mtoa.core.createOptions()
        _optionsNode = cmds.createNode('aiOptions', name='tmpArnoldMayaUsdOptions')

        attrs = cmds.listAttr("defaultArnoldRenderOptions", hd=True,iu=True, c=True)

        for attr in attrs:
            if attr in ['exportMayaUsd','exportFullPaths','exportSeparator', 'exportDagName', 'exportPrefix']:
                continue
            try:
                cmds.connectAttr('defaultArnoldRenderOptions.{}'.format(attr), '{}.{}'.format(_optionsNode, attr))
            except RuntimeError as err:
                pass
        cmds.setAttr('{}.exportMayaUsd'.format(_optionsNode), 1)
    except RuntimeError as err:
        print(err)

# Exported the input node to Arnold, calling the arnoldScene command
def ExportNodesToArnold(nodeName, parentScope, mergeTransformAndShape):
    global _optionsNode
    try:
        if _optionsNode and len(_optionsNode) > 0:
            exportDag = 1 if mergeTransformAndShape else 0
            # Ensure we set properly the options attributes exportDagName and exportPrefix,
            # to be consistent with the mayaUsd settings
            cmds.setAttr('{}.exportDagName'.format(_optionsNode), exportDag)
            if parentScope and len(parentScope) > 0:
                cmds.setAttr('{}.exportPrefix'.format(_optionsNode), parentScope,type='string')

            # call arnoldScene with a specific options node, which will be used for export
            return cmds.arnoldScene([nodeName], mode='convert_selected', options = _optionsNode, list='newNodes')

        return cmds.arnoldScene([nodeName], mode='convert_selected', list='newNodes')
    except RuntimeError as err:
        print(err)
    
# Translator for Arnold DAG nodes (lights, procedurals, etc...)
class ArnoldDagWriter(mayaUsdLib.PrimWriter):
    
    def __init__(self, *args, **kwargs):
        try:
            super(ArnoldDagWriter, self).__init__(*args, **kwargs)
            if not self.GetDagPath().isValid():
                print("Invalid DagPath '{}'\n".format(self.GetDagPath()))
                return
            
            # Call Write with the default time
            self.Write(Usd.TimeCode.Default())
            
        except RuntimeError as err:
            print(err)
        
    # Write the given frame to USD
    def Write(self, usdTime):
        try:            
            if not usdTime:
                usdTime = Usd.TimeCode.Default()

            depNodeFn = OpenMaya.MFnDependencyNode(self.GetMayaObject())
            nodeName = depNodeFn.name()

            global _nodesQueue
            # Add this node to the queue
            _nodesQueue.append([nodeName, None])
            # call parent function so that mayaUsd can author the transform attributes
            super(ArnoldDagWriter, self).Write(usdTime)
            
        except RuntimeError as err:
            print(err)

# API Adaptors are custom translators called on top of builtin translators.
# This way, we can add arnold attributes on top of a primitive that was 
# exported by MayaUsd. We need this for meshes, maya lights, etc..
class ArnoldSchemaAdaptor(mayaUsdLib.SchemaApiAdaptor):
    def __init__(self, *args, **kwargs):
        super(ArnoldSchemaAdaptor, self).__init__(*args, **kwargs)

    def CanAdapt(self):
        return True

    def CanAdaptForExport(self, jobArgs):
        return True
 
    def CopyToPrim(self, prim, usdTime, valueWriter):
        try:
            global _nodesQueue
            depNodeFn = OpenMaya.MFnDependencyNode(self.mayaObject)
            nodeName = depNodeFn.name()
            # Append this node to the queue. Here, this node was already exported 
            # to Usd by MayaUsd, so we need to ensure the arnold node names have 
            # the exact same name as the one authored by MayaUsd. In this case
            # we store the output primitive path as a second element in the list
            _nodesQueue.append([nodeName, prim.GetPath().pathString])
        except RuntimeError as err:
            print(err)   
        return True

# During MayaUsd Exports, the Chaser will be invoked after all other nodes were translated, for each frame.
# Here we will process the export of all the nodes that were queued
class ArnoldUsdChaser(mayaUsdLib.ExportChaser):
    def __init__(self, *args, **kwargs):
        try:
            # chaser context
            ctx = args[0]
            # Store the UsdStage that mayaUsd is authoring
            self.stage = ctx.GetStage()
            jobArgs = ctx.GetJobArgs()
            # Store the parent scope used by mayaUsd
            self.parentScope = jobArgs.parentScope or ''
            if len(self.parentScope) > 0:
                self.parentScope += '/'
            # Should transform and shape nodes be merged
            self.mergeTransformAndShape = jobArgs.mergeTransformAndShape

            # Store this usdStage in a cache that will be used in our C++ 
            # code that calls ArnoldUsdWriter to convert the Arnold 
            # scene to USD
            stageCache = UsdUtils.StageCache.Get()
            if stageCache:
                stageCache.Insert(self.stage)
                # store the cache ID that will be used to retrieve this usdStage
                self.stageId = stageCache.GetId(self.stage)
            
            # Ensure the options node is initialized
            InitOptionsNode()
            
        except RuntimeError as err:
            print(err)
            
    def ExportDefault(self):
        # Export the default time first
        return self.ExportFrame(Usd.TimeCode.Default())

    def ExportFrame(self, frame):
        # Export a given frame
        global _nodesQueue
        try:
            # we want to add the arnold render options to the list of nodes to export.
            _nodesQueue.append(['defaultArnoldRenderOptions', None])
            # loop through all nodes that were added to the queue and convert each of them 
            # to Arnold. Note that we could call arnoldScene only once with all the nodes at once
            # but for now we're doing this to ensure we rename properly each node if needed
            for node in _nodesQueue:
                # export the queued maya node to arnold
                cmdRes = ExportNodesToArnold(node[0], self.parentScope, self.mergeTransformAndShape) or []
                if node[1] and len(cmdRes) > 0:
                    # If an output name is specified, we rename the arnold node that was just created.                    
                    # This is needed for API adaptors, when we export arnold data on top of mayaUsd translators.
                    # In this case we need to ensure the names are identical between maya and arnold exports
                    aNode = ai.AiNodeLookUpByName(None, cmdRes[0])
                    if (aNode):
                        ai.AiNodeSetStr(aNode, 'name', node[1])

            # Call arnoldScene with a usd_cache argument, that will convert the current arnold universe to USD.
            # If needed, add an argument for the frame to be authored, so that it shows as time samples in USD
            if frame.IsDefault():
                cmds.arnoldScene('', usd_cache=self.stageId.ToLongInt(), options = _optionsNode)
            else:
                cmds.arnoldScene('', usd_cache=self.stageId.ToLongInt(), usd_frame = frame.GetValue(), options = _optionsNode)

            # Clear the nodes from the queue
            _nodesQueue = []
            # Destroy the Arnold universe, we might create a fresh new one for another frame
            cmds.arnoldScene(mode='destroy')

        except RuntimeError as err:
            print(err)

        return True

    # Function called after all frames were exported.
    # Here we want to clear all the data that was stored for export
    def PostExport(self):
        global _optionsNode
        try:
            if _optionsNode:
                # Delete the temporary options node that we created
                cmds.delete(_optionsNode)
                _optionsNode = None

            if self.stageId:
                # Remove the usdStage from the stageCache
                stageCache = UsdUtils.StageCache.Get()
                if stageCache:
                    stageCache.Erase(self.stageId)
                self.stageId = None
        
        except RuntimeError as err:
            print(err)            

        return True
    

# Job Contexts allow to customize several settings during a mayaUsd export
# This will add an "Arnold" option in the "PlugIn configuration" drop down in mayaUsd exports
def ArnoldExportJobContext():
    
    extraArgs = {}
    # Store the list of API schemas that need to be enabled during export.
    # We want meshes, curves, cameras, and lights
    extraArgs['apiSchema'] = ['ArnoldPolymeshAPI', 
                  'ArnoldCurvesAPI', 
                  'ArnoldPerspCameraAPI', 
                  'ArnoldLightAPI', 
                  'ArnoldAreaLightAPI']
    
    # The Arnold chaser needs to be enabled in this job context, as it will 
    # centralize the export of Arnold data
    extraArgs['chaser']  = ['ArnoldUsdChaser']
    return extraArgs

# This code should only be called once when the mayaUsd plugin is loaded.
# We register all the custom translators, the chaser, and the job context
if _registerTranslators == False:
    _registerTranslators = True

    maya_version = mtoaUtils.getMayaVersion()
    # For now, we only use the mayaUSD python bindings for maya 2023 and up
    if maya_version >= 2023:
        # Register the Prim Writers for Arnold Dag nodes
        mayaUsdLib.PrimWriter.Register(ArnoldDagWriter, "aiSkyDomeLight")
        mayaUsdLib.PrimWriter.Register(ArnoldDagWriter, "aiAreaLight")
        mayaUsdLib.PrimWriter.Register(ArnoldDagWriter, "aiPhotometricLight")
        mayaUsdLib.PrimWriter.Register(ArnoldDagWriter, "aiLightPortal")
        mayaUsdLib.PrimWriter.Register(ArnoldDagWriter, "aiMeshLight")
        mayaUsdLib.PrimWriter.Register(ArnoldDagWriter, "aiStandIn")
        mayaUsdLib.PrimWriter.Register(ArnoldDagWriter, "aiVolume")
        mayaUsdLib.PrimWriter.Register(ArnoldDagWriter, "aiCurveCollector")

        # Register the API adaptors for maya builtin nodes that require additional arnold attributes
        mayaUsdLib.SchemaApiAdaptor.Register(ArnoldSchemaAdaptor, "mesh", "ArnoldPolymeshAPI")
        mayaUsdLib.SchemaApiAdaptor.Register(ArnoldSchemaAdaptor, "curves", "ArnoldCurvesAPI")
        mayaUsdLib.SchemaApiAdaptor.Register(ArnoldSchemaAdaptor, "camera", "ArnoldPerspCameraAPI")
        mayaUsdLib.SchemaApiAdaptor.Register(ArnoldSchemaAdaptor, "directionalLight", "ArnoldLightAPI")
        mayaUsdLib.SchemaApiAdaptor.Register(ArnoldSchemaAdaptor, "pointLight", "ArnoldLightAPI")
        mayaUsdLib.SchemaApiAdaptor.Register(ArnoldSchemaAdaptor, "areaLight", "ArnoldLightAPI")
        mayaUsdLib.SchemaApiAdaptor.Register(ArnoldSchemaAdaptor, "spotLight", "ArnoldLightAPI")
        
        # Register the Chaser, it will be invoked for each frame after all the translators were processed
        mayaUsdLib.ExportChaser.Register(ArnoldUsdChaser, "ArnoldUsdChaser")
        # Register the Job context, it will add an option "Arnold" to the "Plugin Configuration" 
        # drop down, in the MayaUsd export UI
        mayaUsdLib.JobContextRegistry.RegisterExportJobContext("Arnold", "Arnold", "Arnold", ArnoldExportJobContext)

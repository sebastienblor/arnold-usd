
## we want to add this directory to the path so we can use the extra xgenArnoldUI  files
from pxr import Usd, UsdUtils, Tf
import maya.cmds as cmds

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
    
    
    



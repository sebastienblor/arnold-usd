"""
To override a function in this module, do the following:

    import mtoa.hooks
    def setupFilter(filter, aovName=None):
        if aovName == "Occlusion":
            filter.aiTranslator.set("guassian")
    mtoa.hooks.setupFilter = setupFilter

There are copies of each function in this module that start with an underscore. 
This makes it easy to extend the built-in functionality within your override.  For example:

    import mtoa.hooks
    def getDefaultAOVs():
        return _getDefaultAOVs() + [("Occlusion", "float")]
    mtoa.hooks.getDefaultAOVs = getDefaultAOVs
"""
import os

def setupFilter(filter, aovName=None):
    """
    Setup a filter that was created by mtoa.
    
    filter : pymel.PyNode
        the newly created filter node
    
    aovName : string or None
        the name of the AOV that the filter will be connected to
    
    This function can be used to set the default filter type or set attributes like
    filter width.
    
    Note that if you only want to change the default filter, you can use `registerDefaultTranslator`
    also found in this module. For example, to set the default to gaussian:
    
        registerDefaultTranslator('aiAOVFilter', 'gaussian')
    """
    pass

def setupDriver(driver, aovName=None):
    """
    Setup a driver that was created by mtoa.
    
    filter : pymel.PyNode
        the newly created driver node
    
    aovName : string or None
        the name of the AOV that the driver will be connected to
    
    This function can be used to set the default driver type or set attributes like
    compression.
    
    Note that if you only want to change the default driver, you can use `registerDefaultTranslator`
    also found in this module. For example, to set the default to jpg:
    
        registerDefaultTranslator('aiAOVDriver', 'jpg')
    """
    pass

def setupOptions(options):
    """
    Setup the 'defaultArnoldRenderOptions' node.
    
    options : pymel.PyNode
        the newly created options node

    Override this function to change defaults on the options node.
    """
    pass

def setupDefaultAOVs(opts):
    """
    opts: aovs.AOVInterface
        used to call addAOV, etc
    
    By default, this function calls getDefaultAOVs to get the names and types of AOVs to create.
    
    Override this function if getDefaultAOVs does not provide enough control.
    """
    for args in getDefaultAOVs():
        opts.addAOV(*args)

# save a copy
_setupDefaultAOVs = setupDefaultAOVs

def getDefaultAOVs():
    """
    Returns a list of aov (name, type) pairs for setting up the default AOVs in a scene.
    type can either be a string or integer from `mtoa.aovs.TYPES`
    """
    return []

# save a copy
_getDefaultAOVs = getDefaultAOVs

def fileTokenScene(path, tokens, **kwargs):
    import pymel.core as pm
    if '<Scene>' in path and 'Scene' not in tokens:
        sceneName = pm.sceneName().namebase
        if sceneName == '':
            sceneName = 'untitled'
        tokens['Scene'] = sceneName
_fileTokenScene = fileTokenScene

def fileTokenRenderPass(path, tokens, **kwargs):
    import pymel.core as pm
    if not kwargs.get('strictAOVs', False) and '<RenderPass>' not in path and 'RenderPass' in tokens:
        if not os.path.isabs(path):
            path = '<RenderPass>/' + path
        else:
            pm.cmds.warning('[mtoa] Multiple render passes (AOVs) exist, but output path is absolute and without <RenderPass> token: "%s"' % path)
    return path
_fileTokenRenderPass = fileTokenRenderPass

def fileTokenCamera(path, tokens, **kwargs):
    import pymel.core as pm
    if '<Camera>' in path:
        if 'Camera' not in tokens:
            raise ValueError("You must provide a value for Camera token")
    elif len([c for c in pm.ls(type='camera') if c.renderable.get()]) > 1:
        if not os.path.isabs(path):
            path = '<Camera>/' + path
            if 'Camera' not in tokens:
                raise ValueError("You must provide a value for Camera token")
        else:
            pm.cmds.warning('[mtoa] Multiple renderable cameras exist, but output path is absolute and without <Camera> token: "%s"' % path)
    return path
_fileTokenCamera = fileTokenCamera

def fileTokenRenderLayer(path, tokens, **kwargs):
    import pymel.core as pm
    if '<RenderLayer>' in path:
        if 'RenderLayer' not in tokens:
            tokens['RenderLayer'] = pm.cmds.editRenderLayerGlobals(q=True, currentRenderLayer=True)
    elif len(pm.cmds.listConnections('renderLayerManager.renderLayerId', source=False, destination=True)) > 1:
        if not os.path.isabs(path):
            path = '<RenderLayer>/' + path
            if 'RenderLayer' not in tokens:
                tokens['RenderLayer'] = pm.cmds.editRenderLayerGlobals(q=True, currentRenderLayer=True)
        else:
            pm.cmds.warning('[mtoa] Multiple renderable render layers exist, but output path is absolute and without <RenderLayer> token: "%s"' % path)

    if tokens.get('RenderLayer', None) == 'defaultRenderLayer':
        tokens['RenderLayer'] = 'masterLayer'
    return path
_fileTokenRenderLayer = fileTokenRenderLayer

def fileTokenVersion(path, tokens, **kwargs):
    import pymel.core as pm
    if '<Version>' in path and 'Version' not in tokens:
        tokens['Version'] = pm.getAttr('defaultRenderGlobals.renderVersion')

import maya.cmds as cmds
import maya.mel as mel
import inspect
import types
import re
import os
import shlex
import sys
import ctypes
import string
import locale
import maya.OpenMayaRender
from maya.api import OpenMaya
import arnold as ai
import ctypes
from ctypes import *

from multiprocessing import cpu_count
from ui.qt.Qt import *
from maya import OpenMayaUI as omui 
import time


from hooks import fileTokenScene, fileTokenRenderPass, fileTokenCamera, fileTokenRenderLayer, fileTokenVersion

def even(num):
    return bool(num % 2)

def odd(num):
    return not bool(num % 2)

def capitalize(s):
    return s[0].upper() + s[1:] if s else s

def prettify(s):
    "convert from '_fooBar_Spangle22poop1' to 'Foo Bar Spangle22 Poop1'"
    return ' '.join([capitalize(x) for x in re.findall('[a-zA-Z][a-z]*[0-9]*',s)])

def toMayaStyle(s):
    "convert from this_style to thisStyle"
    parts = s.split('_')
    return ''.join([parts[0]] + [capitalize(x) for x in parts[1:]])

def groupn(iterable, n):
    '''
    group a flat list into tuples of length n
    '''
    return zip(*[iter(iterable)]*n)

def createColor(node, name):
    node = str(node)
    cmds.addAttr(node, longName=name, at='float3', usedAsColor=True)
    cmds.addAttr(node, longName=name + 'R', at='float', parent=name)
    cmds.addAttr(node, longName=name + 'G', at='float', parent=name)
    cmds.addAttr(node, longName=name + 'B', at='float', parent=name)

_objectStore = {}

def pyToMelProc(pyobj, args=(), returnType=None, procName=None, useName=False, procPrefix='pyToMel_'):
    '''
    create a MEL procedure from a python callable
    
    :pyobj:
        any python callable

    :args: 
        a list of (type, name) pairs defining the arguments of the MEL procedure to create. should be
        compatible with the arguments of the passed python object. 

    :procName:
        name to use for the MEL procedure, if None, useName and procPrefix are used to control the name

    :useName:
        use the name of the python object as the MEL procedure

    :procPrefix:
        if neither procName or useName are provided, the id() function is used to generate an unique
        integer for the python object to use as the MEL procedure name. the procPrefix is prepended to this name. 
    '''
    melParams = []
    pyParams = []
    melReturn = returnType if returnType else '' 

    for type, name in args:
        melParams.append('%s $%s' % (type, name))
        #if the arguement is a string we add \\" before and after
        if type == 'string':
            pyParams.append(r"""'"+$%s+"'""" % name)
        else:
            pyParams.append(r'"+$%s+"' % name)

    # get a unique id for this object
    objId = id(pyobj)

    # fill out a dictionary for formatting the procedure definition
    d = {}

    if procName:
        d['procname'] = procName
    elif useName:
        d['procname'] = pyobj.__name__
    else:
        # prefix
        # try to add some extra info to the name for easier debugging
        if isinstance(pyobj, types.LambdaType):
            procPrefix += '_lambda'
        elif isinstance(pyobj, (types.FunctionType, types.BuiltinFunctionType)):
            try:
                procPrefix += '_' + pyobj.__name__
            except (AttributeError, TypeError):
                pass
        elif isinstance(pyobj, types.MethodType):
            try:
                procPrefix += '_' + pyobj.im_class.__name__ + '_' + pyobj.__name__
            except (AttributeError, TypeError):
                pass
        d['procname'] = '%s%s' % (procPrefix, objId)

    # ensure we remove forbidden characters. This appears because of '<built-in>' token in procname #3263
    d['procname'] = d['procname'].replace('<', '_').replace('>', '_').replace('-', '_')
    d['melParams'] = ', '.join(melParams)
    d['pyParams'] = ', '.join(pyParams)
    d['melReturn'] = melReturn
    d['thisModule'] = __name__
    d['id'] = objId

    contents = '''global proc %(melReturn)s %(procname)s(%(melParams)s){'''
    if melReturn:
        contents += 'return '
    contents += '''python("import %(thisModule)s;%(thisModule)s._objectStore[%(id)s](%(pyParams)s)");}'''

    mel.eval(contents % d)
    # TODO: check for error and don't add the python object if we failed
    _objectStore[objId] = pyobj
    return d['procname']

# from supportRenderers.mel
def currentRenderer():
    return cmds.getAttr('defaultRenderGlobals.currentRenderer')

def findMelScript(name):
    path = mel.eval('whatIs("%s")'%name)
    path = path.split('in:')[-1][1:]
    proc = []
    grab = 0
    go = False
    with open(path) as f:
        for line in f:
            if name in line and 'proc' in line:
                proc.append(line)
                go = True
            
            if go and ('{' in line):
                grab += 1
              
            if grab > 0 and go:
                proc.append(line)
            
            if '}' in line and go:
                grab -= 1

            if grab == 0 and len(proc) != 1 and go:
                go=False
                break
    return proc

def safeDelete(node):
    '''delete a node, or disconnect it, if it is read-only'''
    node_str = str(node)
    if cmds.lockNode( node_str, q=True, lock=True )[0]:

        node_connections = cmds.listConnections('{}.message'.format(node_str),
                                                  source=False, destination=True,
                                                  connections=True,
                                                  plugs=True) or []
        for src, dest in zip(node_connections[::2], node_connections[1::2]):
            cmds.disconnectAttr(src, dest)
    else:
        cmds.delete(node_str)

def _substitute(parts, tokens, allOrNothing=False, leaveUnmatchedTokens=False):
    result = []
    for i, tok in enumerate(parts):
        if even(i):
            try:
                tokn = tokens[tok]
                if tokn is None:
                    result.append('<%s>' % tok)
                else:
                    result.append(tokn.replace(':', '_'))
            except KeyError:
                if allOrNothing:
                    if leaveUnmatchedTokens:
                        return '<%s>' % tok
                    else:
                        return ''
                elif leaveUnmatchedTokens:
                    result.append('<%s>' % tok)
                else:
                    result.append('')
        else:
            result.append(tok)
    return ''.join(result)

_tokenCallbacks = []
_tokenNames = []
def registerFileToken(func, newTokens=None):
    """
    Register a function for modifying the file path generated by getImageName.
    
    func : callable
        The callback function. It is expected to take the form:
    
        func(path, tokens, **kwargs)
        
        - path (str) : the file path with unexpanded tokens
        - tokens (dict) : a dictionary of token names to token values.
        - kwargs (dict) : additional arguments passed to getFileName()

        If the function modifies the path string, it should return the modified path as a result.
        The dictionary should be modified in place.

    newTokens : string, list of strings, or None
        The names of any new tokens defined by the function, if any
    """
    global _tokenCallbacks
    global _tokenNames
    assert callable(func), "first argument to registerFileToken must be a python callable"
    _tokenCallbacks.append(func)
    if newTokens:
        if isinstance(newTokens, str):
            _tokenNames.append(newTokens)
        else:
            try:
                _tokenNames.extend(newTokens)
            except:
                print "second argument to registerFileToken expects a string or a list of strings"

def registeredTokens():
    global _tokenNames
    return _tokenNames[:]

def expandFileTokens(path, tokens, leaveUnmatchedTokens=False):
    """
    path : str
        unexpanded path, containing tokens of the form <MyToken>
    
    tokens : dict or str
        dictionary of the form {'MyToken' : value} or space separated string of form 'MyToken=value'

    This is a token expansion system based on Maya's, but with several improvements.
    In addition to standard tokens of the form <MyToken>, it also supports
    conditional groups using brackets, which will only be expanded if all the
    tokens within it exist.

    for example, in the following case, the group's contents (the underscore) are
    included because the RenderPass token is filled:
    
        >>> expandFileTokens('filename[_<RenderPass>].jpg', {'RenderPass' : 'Diffuse'})
        'filename_Diffuse.jpg'

    but in this case the contents enclosed in brackets is dropped:

        >>> expandFileTokens('filename[_<RenderPass>].jpg', {})
        'filename.jpg'
    """
    if isinstance(tokens, basestring):
        tokens = dict([pair.split('=') for pair in shlex.split(tokens)])

    grp_reg = re.compile('\[([^\]]+)\]')
    tok_reg = re.compile('<([a-zA-Z]+)>')
    result = []
    for i, grp in enumerate(grp_reg.split(path)):
        parts = tok_reg.split(grp)
        if even(i):
            result.append(_substitute(parts, tokens, allOrNothing=True, leaveUnmatchedTokens=leaveUnmatchedTokens))
        else:
            result.append(_substitute(parts, tokens, allOrNothing=False, leaveUnmatchedTokens=leaveUnmatchedTokens))
    return ''.join(result)

def translatorToExtension(translatorName):
    if (translatorName == "deepexr") :
        return "exr"
    elif (translatorName == "jpeg") :
        return "jpg"
    else :
        return translatorName
    
def getFileName(pathType, tokens, path='<Scene>', frame=None, fileType='images',
                 createDirectory=False, isSequence=None, leaveUnmatchedTokens=False,
                 catchErrors=True, **kwargs):
    """
    A more generic replacement for MCommonRenderSettingsData.getImageName() that also works for types other
    than images.
    
    The naming scheme defined by the `path` argument is error-checked and corrected where necessary.
    For example, if there are multiple renderable cameras in the scene but a <Camera> token does not appear
    in the passed `path` naming scheme, then a <Camera> sub-directory will be added to `path`.
    A similar check is performed for render layers and AOVs.
    
    This function largely reproduces the behavior of MCommonRenderSettingsData.getFileName() with
    several important exceptions:
    
        - If 'RenderPass' is in the passed tokens map but not in the naming scheme, a <RenderPass>
          sub-directory will be automatically added.  By default, MCommonRenderSettingsData.getImageName()
          would only perform this operation if a Maya render pass node was setup in the scene (which
          MtoA does not use)
        - Whether or not the generated path is a sequence can be overridden by the `isSequence` argument,
          a setting which MCommonRenderSettingsData.getImageName() always pulled from the globals
        - MCommonRenderSettingsData.getImageName() only works for images, adding them to the workspace directory
          set for the 'images' type.  This function can work for any registered file rule (see the MEL
          workspace command), including 'ASS'.

    pathType : 
            - MCommonRenderSettingsData.kFullPathImage or 'full'
            - MCommonRenderSettingsData.kRelativePath or 'relative'
            - MCommonRenderSettingsData.kFullPathTmp or 'temp'
    
    path : str
            unexpanded path, containing tokens surrounded by square brackets: <MyToken>
    
    tokens : dict or str
            dictionary of the form {'MyToken' : value} or space separated string of form 
            'MyToken=value Other=foo'
    
    frame : float, int, or None
            frame number. If None, current frame is used
    
    fileType : str
            a valid type to pass to workspace -fileRuleEntry
    
    createDirectory : bool
            whether or not to create the directory (ignored when pathType is 'temp')
    
    isSequence : bool or None
            specify whether the path generated should include a frame number. If None, use the render globals
    
    leaveUnmatchedTokens : bool
            whether unmatched tokens should be left unexpanded or removed
    
    catchErrors : bool
            if False, errors raised by a token will not be caught and will abort the entire function

    """
    # convert tokens to dictionary
    if isinstance(tokens, basestring):
        tokens = dict([pair.split('=') for pair in shlex.split(tokens)])

    kwargs.update(dict(frame=frame,
                       fileType=fileType,
                       createDirectory=createDirectory,
                       isSequence=isSequence,
                       leaveUnmatchedTokens=leaveUnmatchedTokens))


    if 'Eye' in tokens:
        # adding automatically the Eye token for stereo rendering
        # do we want to test if it's already present in 'path'
        # and only add the token if it's not there ?
        if (tokens['Eye'] != ''):
            path += '.<Eye>'

    # get info from globals
    # NOTE: there is a bug in the wrapper of this class that prevents us from retrieving the
    # 'namePattern' property, so that must be properly passed in via the 'path' argument
    settings = maya.OpenMayaRender.MCommonRenderSettingsData()
    
    maya.OpenMayaRender.MRenderUtil.getCommonRenderSettings(settings)
    if isSequence is None:
        isSequence = settings.isAnimated()
    if isSequence:
        schemes = ('',
                   '.<Frame>.<Extension>',
                   '.<Frame>.<Extension>',
                   '.<Extension>.<Frame>',
                   '<Frame>',
                   '<Frame>.<Extension>',
                   '_<Frame>.<Extension>')
    else:
        schemes = ('',
                   '.<Extension>',
                   '.<Extension>',
                   '.<Extension>',
                   '',
                   '.<Extension>',
                   '.<Extension>')

    path += schemes[settings.namingScheme]

    if '<Extension>' in path and 'Extension' not in tokens:
        tokens['Extension'] = translatorToExtension(cmds.getAttr('defaultArnoldDriver.aiTranslator'))
    if '<Frame>' in path and 'Frame' not in tokens:
        # TODO: add handling of sub-frames
        if frame is None:
            frame = cmds.currentTime(query=True)
        else:
            frame = float(frame)
        if settings.renumberFrames:
            byFrame = settings.renumberBy/settings.frameBy
            frame = frame * byFrame - (settings.frameStart.value()-settings.renumberStart) - (byFrame-1.0)
        tokens['Frame'] = frame
    if 'Frame' in tokens and isinstance(tokens['Frame'], (float, int)):
        frame = tokens['Frame']
        frame = str(int(round(frame)))
        # add padding
        frame = ((settings.framePadding -len(frame)) * '0') + frame
        tokens['Frame'] = frame

    global _tokenCallbacks
    for cb in _tokenCallbacks:
        try:
            res = cb(path, tokens, **kwargs)
        except Exception, err:
            if catchErrors:
                print "Callback %s.%s failed: %s" % (cb.__module__, cb.__name__, err)
            else:
                raise
        else:
            if res is not None:
                path = res

    # Use display name for render layer
    layerName = tokens.get('RenderLayer', None)
    if layerName and mel.eval('exists renderLayerDisplayName'):
        tokens['RenderLayer'] = mel.eval('renderLayerDisplayName ' + layerName)

    #print path, tokens
    partialPath = expandFileTokens(path, tokens, leaveUnmatchedTokens=leaveUnmatchedTokens)
    if pathType in [maya.OpenMayaRender.MCommonRenderSettingsData.kRelativePath, 'relative']:
        return partialPath

    if cmds.optionVar(exists="OverrideFileOutputDirectory"):
        imageDir = cmds.optionVar(query="OverrideFileOutputDirectory")
    else:
        imageDir = cmds.workspace(fileRuleEntry=fileType)
        imageDir = imageDir if imageDir else 'data'
        imageDir = cmds.workspace(expandName=imageDir);

    codecs = ['utf-8', 'latin-1']
    for i in codecs:
        try:
            partialPath = partialPath.decode(i)
            break
        except UnicodeDecodeError:
            pass
    for i in codecs:
        try:
            imageDir = imageDir.decode(i)
            break
        except UnicodeDecodeError:
            pass   

    if cmds.optionVar(exists="OverrideFileOutputDirectory"):
        result = os.path.join(imageDir, partialPath)
    elif pathType in [maya.OpenMayaRender.MCommonRenderSettingsData.kFullPathTmp, 'temp']:
        result = os.path.join(imageDir, 'tmp', partialPath)
    elif pathType in [maya.OpenMayaRender.MCommonRenderSettingsData.kFullPathImage, 'full']:
        result = os.path.join(imageDir, partialPath)
    else:
        raise TypeError("Invalid pathType")

    result = result.replace("\\", "/")
    result = convertToUnicode(result)
    if createDirectory:
        dir =  os.path.dirname(result)
        try:
            os.makedirs(dir)
        except OSError as exc:
            import errno
            # if directory already exists we ignore the exception
            # NOTE that we do not use os.path.exists to avoid potential race conditions
            # on render farms
            if exc.errno != errno.EEXIST:
                raise
    return result

registerFileToken(fileTokenScene, 'Scene')
registerFileToken(fileTokenRenderPass, 'RenderPass')
registerFileToken(fileTokenCamera, 'Camera')
registerFileToken(fileTokenRenderLayer, 'RenderLayer')
registerFileToken(fileTokenVersion, 'Version')
registerFileToken(fileTokenVersion, 'Eye')

def convertToUnicode(s):
    try:
        s = s.encode('utf-8')
    except UnicodeDecodeError:
        pass
    return s

def getEnvironmentVariable(name):
    '''
    This function is meant to support unicode environment variables in python 2.*
    '''
    if sys.platform == 'win32':
        n= ctypes.windll.kernel32.GetEnvironmentVariableW(name, None, 0)
        if n==0:
            return None
        buf= ctypes.create_unicode_buffer(u'\0'*n)
        ctypes.windll.kernel32.GetEnvironmentVariableW(name, buf, n)
        return buf.value
    else:
        return os.environ[name]

def setEnvironmentVariable(name, value):
    '''
    This function is meant to support unicode environment variables in python 2.*
    '''
    if sys.platform == 'win32':    
        buf= ctypes.create_unicode_buffer(value)
        ctypes.windll.kernel32.SetEnvironmentVariableW(name, buf)
    else:
        os.environ[name] = value    
        
def createLocator(locatorType, asLight=False):
    if asLight:
        lName = cmds.shadingNode(locatorType, name=('%sShape1' % locatorType), asLight=True)
        shapeNames = cmds.listRelatives(lName, fullPath=True, shapes=True)
        cmds.select(lName, r = True)
        return (shapeNames[0], lName)

    lNode = cmds.createNode('transform', name='%s1' % locatorType)
    lId = lNode[len(locatorType):]
    shapeName = '%sShape%s' % (locatorType, lId)
    cmds.createNode(locatorType, name=shapeName, parent=lNode, skipSelect = True)
    return (shapeName, lNode)

# in theory we could merge this function with the one above easily,
# but we need to make sure first it's not breaking other calls
def createLocatorWithName(locatorType, nodeName, asLight=False):
    if asLight:
        lName = cmds.shadingNode(locatorType, name=('%sShape' % nodeName), asLight=True)
        shapeNames = cmds.listRelatives(lName, fullPath=True, shapes=True)
        return (shapeNames[0], lName)

    lNode = cmds.createNode('transform', name='%s' % nodeName)
    lId = lNode[len(locatorType):]
    shapeName = '%sShape' % nodeName
    cmds.createNode(locatorType, name=shapeName, parent=lNode)       
    return (shapeName, lNode)


def createMeshLight(legacy=False, centerPivot=True):
    sls = cmds.ls(sl=True, et='transform')
    if len(sls) == 0:
        cmds.confirmDialog(title='Error', message='No transform is selected!', button='Ok')
        return
    meshTransform = sls[0]
    shs = cmds.listRelatives(meshTransform, fullPath=True, type='mesh')
    if shs is None or len(shs) == 0:
        cmds.confirmDialog(title='Error', message='The selected transform has no meshes', button='Ok')
        return
    meshShape = shs[0]
    if legacy:
        cmds.setAttr('%s.aiTranslator' % meshShape, 'mesh_light', type='string')
    else:
        # Make sure the shape has not been converted already
        existing = cmds.listConnections('%s.outMesh' % meshShape, shapes=True, type='aiMeshLight')
        if existing and len(existing) > 0:
            cmds.confirmDialog(title='Error', message='Mesh light already created!', button='Ok')
            return

        # Make sure the shape has only a single parent
        # Multiple light instances are not supported
        allPaths = cmds.listRelatives(meshShape, allParents=True, fullPath=True) or []
        if len(allPaths) != 1:
            cmds.confirmDialog(title='Error', message='The mesh has multiple instances. Light instances are not supported!', button='Ok')
            return

        lightName = 'light_%s' % meshTransform
        (lightShape,lightTransform) = createLocatorWithName('aiMeshLight', nodeName=lightName, asLight=True)

        cmds.connectAttr('%s.outMesh' % meshShape, '%s.inMesh' % lightShape)

        p = cmds.parent(lightTransform, meshTransform, relative=True)
        lightShape = cmds.listRelatives(p[0], shapes=True, fullPath=True)[0]
        # Hide the original mesh using the visibility attribute
        # We previously used lodVisibility to keep the dirtiness propagation enabled,
        # but I can't manage to find a situation that fails. So we're now using visibility

        #cmds.connectAttr('%s.showOriginalMesh' % lightShape, '%s.lodVisibility' % meshShape)
        cmds.connectAttr('%s.showOriginalMesh' % lightShape, '%s.visibility' % meshShape)

        # FIXME : we shouldn't have to do this, but otherwise it takes a couple of tweaks on
        # showOriginalMesh before seeing its effect
        cmds.setAttr('%s.showOriginalMesh' % lightShape, 1)
        cmds.setAttr('%s.showOriginalMesh' % lightShape, 0)


        cmds.select(lightTransform)

def getSourceImagesDir():
    sourceImagesRule = cmds.workspace(fileRuleEntry='sourceImages')
    if sourceImagesRule != None:
        sourceImagesRule = sourceImagesRule.split(';')
        ret = []
        for rule in sourceImagesRule:
            ret.append(cmds.workspace(expandName=rule))
        return ret
    else:
        return [cmds.workspace(expandName='sourceimages')]

def getActiveRenderLayerName():
    if not cmds.objExists('renderLayerManager.renderLayerId'):
        return ''
        
    renderLayers = cmds.listConnections('renderLayerManager.renderLayerId') or []
    if (len(renderLayers) > 1):
        layer = cmds.editRenderLayerGlobals(query=True, currentRenderLayer=True)
        if (cmds.getAttr(layer+'.identification') == 0):
            return 'masterLayer'
        return layer
    return ''

def getMayaVersion():
    version = cmds.about(f=True)
    return int(float(version[:4]))



cb_id = None
percent_done = 0.0
dialog = None
start_time = 0

def terminate_GPUCache():

    OpenMaya.MGlobal.displayWarning("GPU cache creation terminated")
    ai.AiGPUCachePopulateTerminate()
    dialog.cancel()
    OpenMaya.MMessage.removeCallback(cb_id)
    
def GPU_optixCacheCallBack(*args):

    global cb_id, start_time , time_remaining
    (e, f) = ai.AiGPUCachePopulateStatus()
    percent = f*100

    elapsed = time.time() - start_time
    elapsed_string = time.strftime("%H:%M:%S", time.gmtime(elapsed))

    if (percent <= 0):
        time_remaining_string = " ~10 - 20 mins"
    else:
        time_remaining = (elapsed / percent ) * ( 100 - percent)
        time_remaining_string = time.strftime("%H:%M:%S", time.gmtime(time_remaining))

    text = str( " Time Elapsed : " + elapsed_string + "     Time Remaining : " + time_remaining_string )

    dialog.setValue(int(percent))
    dialog.setLabelText(text)
    if e == ai.AI_RENDER_STATUS_FINISHED.value :
        dialog.cancel()
        OpenMaya.MMessage.removeCallback(cb_id)

def cache_populate_callback(cUserdata, status, fraction_done, msg):

   if fraction_done==0.0:
      print '[AiGPUCachePopulate] Running ..'
      return
   if status != ai.AtRenderErrorCode:
      global percent_done
      if ( fraction_done*100.0 > percent_done ):
        print '[AiGPUCachePopulate] (%.1f%% done)' % (100.0*fraction_done)
        percent_done = 100.0 * fraction_done
   else:
      print '[AiGPUCachePopulate] Error: %s'
   if fraction_done==1.0:
      print '[AiGPUCachePopulate] Finished.'

def populate_GPUCache():

    global dialog , cb_id, start_time ,percent_done
    start_time = 0
    percent_done = 0
    mayaMainWindowPtr = omui.MQtUtil.mainWindow() 
    mayaMainWindow= shModule.wrapInstance(long(mayaMainWindowPtr), QtWidgets.QWidget) 
    start_time = time.time()
    text = str( " Time Elapsed : 00:00:00 " + "     Time Remaining : ~10 - 20 mins" )
    dialog = QtWidgets.QProgressDialog( parent = mayaMainWindow , minimum = 0 , maximum = 100, labelText = text , flags = QtCore.Qt.Window )
    dialog.setWindowTitle(' Pre Populating GPU Cache ')
    dialog.setModal(True)
    dialog.setMinimumWidth(500)
    dialog.canceled.connect(terminate_GPUCache)
    cb_id =  OpenMaya.MTimerMessage.addTimerCallback(0.1, GPU_optixCacheCallBack)
    use_threads = int(cpu_count()) - 2 # Total threads - 2 for other processes. 
    ai.AiGPUCachePopulate(ai.AI_GPU_CACHE_POPULATE_NON_BLOCKING, use_threads , cache_populate_callback)
    dialog.show()


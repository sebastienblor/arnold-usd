# NOTE: this module should not import PyMEL
import maya.cmds as cmds
import maya.mel as mel
import pymel.core as pm
import inspect
import types
import re
import os
import shlex
import sys
import ctypes

def even(num):
    return bool(num % 2)

def odd(num):
    return not bool(num % 2)

def capitalize(s):
    return s[0].upper() + s[1:] if s else s

def prettify(s):
    "convert from '_fooBar_Spangle22poop1' to 'Foo Bar Spangle22 Poop1'"
    return ' '.join([capitalize(x) for x in re.findall('[a-zA-Z][a-z]*[0-9]*',s)])

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
    if node.isReadOnly():
        node.message.disconnect()
    else:
        cmds.delete(str(node))

def _substitute(parts, tokens, allOrNothing=False):
    result = []
    for i, tok in enumerate(parts):
        if even(i):
            try:
                result.append(tokens[tok])
            except KeyError:
                if allOrNothing:
                    return ''
                else:
                    result.append('')
        else:
            result.append(tok)
    return ''.join(result)

def expandFileTokens(path, tokens, customOnly=False):
    """
    path : unexpanded path, containing tokens of the form <MyToken>
    tokens: dictionary of the form {'MyToken' : value} or space separated string of form 'MyToken=value'
    customOnly: if True, standard tokens filled by Maya are left unexpanded so that
    the result can be passed to MCommonRenderSettingsData.getFileName()

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
    if customOnly:
        newTokens = tokens.copy()
        for tok in ['Scene', 'RenderLayer', 'Camera', 'Extension', 'Version']:
            newTokens[tok] = '<%s>' % tok
        tokens = newTokens
    grp_reg = re.compile('\[([^\]]+)\]')
    tok_reg = re.compile('<([a-zA-Z]+)>')
    result = []
    for i, grp in enumerate(grp_reg.split(path)):
        parts = tok_reg.split(grp)
        if even(i):
            result.append(_substitute(parts, tokens, allOrNothing=True))
        else:
            result.append(_substitute(parts, tokens, allOrNothing=False))
    return ''.join(result)

def getFileName(pathType, tokens, path='<Scene>', frame=None, fileType='images',
                 createDirectory=True, isSequence=None):
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
            one of MCommonRenderSettingsData.kFullPathImage, MCommonRenderSettingsData.kRelativePath,
            or MCommonRenderSettingsData.kFullPathTmp
    path :
            unexpanded path, containing tokens of the form <MyToken>
    tokens :
            dictionary of the form {'MyToken' : value} or space separated string of form 
            'MyToken=value Other=foo'
    frame :
            float or int. If None, current frame is used
    fileType :
            a valid type to pass to workspace -fileRuleEntry
    createDirectory:
            whether or not to create the directory
    """
    # convert tokens to dictionary
    if isinstance(tokens, basestring):
        tokens = dict([pair.split('=') for pair in shlex.split(tokens)])

    if '<Scene>' in path and 'Scene' not in tokens:
        sceneName = pm.sceneName().namebase
        if sceneName == '':
            sceneName = 'untitled'
        tokens['Scene'] = sceneName

    isRelPath = not os.path.isabs(path)

    if '<RenderPass>' not in path and 'RenderPass' in tokens:
        path = '<RenderPass>/' + path

    if '<Camera>' in path:
        if 'Camera' not in tokens:
            raise ValueError("You must provide a value for Camera token")
    elif len([c for c in pm.ls(type='camera') if c.renderable.get()]) > 1:
        if isRelPath:
            path = '<Camera>/' + path
            if 'Camera' not in tokens:
                raise ValueError("You must provide a value for Camera token")
        else:
            cmds.warning('[mtoa] Multiple renderable cameras exist, but output path is absolute and without <Camera> token: "%s"' % path)

    if '<RenderLayer>' in path:
        if 'RenderLayer' not in tokens:
            tokens['RenderLayer'] = cmds.editRenderLayerGlobals(q=True, currentRenderLayer=True)
    elif len(pm.ls('*', type='renderLayer')) > 1: # the '*' ensures that we only find layers in the empty namespace
        if isRelPath:
            path = '<RenderLayer>/' + path
            if 'RenderLayer' not in tokens:
                tokens['RenderLayer'] = cmds.editRenderLayerGlobals(q=True, currentRenderLayer=True)
        else:
            cmds.warning('[mtoa] Multiple renderable render layers exist, but output path is absolute and without <RenderLayer> token: "%s"' % path)

    if tokens.get('RenderLayer', None) == 'defaultRenderLayer':
        tokens['RenderLayer'] = 'masterLayer'

    if '<Version>' in path and 'Version' not in tokens:
        tokens['Version'] = pm.getAttr('defaultRenderGlobals.renderVersion')

    # get info from globals
    # NOTE: there is a bug in the wrapper of this class that prevents us from retrieving the
    # 'namePattern' property, so that must be properly passed in via the 'path' argument
    settings = pm.api.MCommonRenderSettingsData()
    pm.api.MRenderUtil.getCommonRenderSettings(settings)
    if isSequence is None:
        isSequence = settings.isAnimated()
    if isSequence:
        schemes = ('',
                   '.<Extension>',
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
        tokens['Extension'] = pm.getAttr('defaultArnoldDriver.aiTranslator')
    if '<Frame>' in path and 'Frame' not in tokens:
        # TODO: add handling of sub-frames
        if frame is None:
            frame = pm.currentTime()
        else:
            frame = float(frame)
        if settings.renumberFrames:
            byFrame = settings.renumberBy/settings.frameBy
            frame = frame * byFrame - (settings.frameStart.value()-settings.renumberStart) - (byFrame-1.0)
        frame = str(int(round(frame)))
        # add padding
        frame = ((settings.framePadding -len(frame)) * '0') + frame
        tokens['Frame'] = frame
    #print path, tokens
    partialPath = expandFileTokens(path, tokens)
    if pathType == pm.api.MCommonRenderSettingsData.kRelativePath:
        return partialPath

    rootPath = pm.workspace(q=True, rd=True)
    imageDir = pm.workspace(fileType, q=True, fileRuleEntry=True)
    imageDir = imageDir if imageDir else 'data'

    if not os.path.isabs(imageDir):
        imageDir = os.path.join(rootPath, imageDir)

    if pathType == pm.api.MCommonRenderSettingsData.kFullPathTmp:
        result = os.path.join(imageDir, 'tmp', partialPath)
    elif pathType == pm.api.MCommonRenderSettingsData.kFullPathImage:
        result = os.path.join(imageDir, partialPath)
    else:
        raise TypeError("Invalid pathType")

    result = result.replace("\\", "/")
    if createDirectory:
        dir =  os.path.dirname(result)
        if not os.path.exists(dir):
            os.makedirs(dir)
    return result

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
        buf= ctypes.create_unicode_buffer(unicode(value))
        ctypes.windll.kernel32.SetEnvironmentVariableW(name, buf)
    else:
        os.environ[name] = value    
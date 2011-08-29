# NOTE: this module should not import PyMEL
import maya.cmds as cmds
import maya.mel as mel
import inspect
import re
import os

def even(num):
    return bool(num % 2)

def odd(num):
    return not bool(num % 2)

def capitalize(s):
    return s[0].upper() + s[1:] if s else s

def prettify(s):
    "convert from '_fooBar_Spangle22poop1' to 'Foo Bar Spangle22 Poop1'"
    return ' '.join([capitalize(x) for x in re.findall('[a-zA-Z][a-z]*[0-9]*',s)])

_objectStore = {}

def mtoaPackageRoot():
    '''return the path to the mtoa python package directory'''
    return os.path.dirname(inspect.getfile(inspect.currentframe()))

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

def arnoldFixLightFilters():

    lights = cmds.ls(type='light')
    for light in lights:
        if cmds.attributeQuery("aiLightFilters", exists=True, node=light):
                attr = '%s.aiLightFilters'%light
                if cmds.getAttr(attr, type=True) == 'string':
                    filters = cmds.getAttr(attr)
                    tmp = filters.split(':')

                    cmds.deleteAttr(attr)
                    cmds.addAttr(ln='aiLightFilters', sn='light_filters', nn='Light Filters', at='message', multi=light)

                    for filter in filters:
                        if cmds.objExists(filter):
                            cmds.connectAttr('%s.message'%filter, '%s.[%d]'%(attr, i))

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

    This is a more advanced token expansion system based on Maya's.
    In addition to standard tokens of the form <MyToken>, it also supports
    conditional groups using brackets, which will only be expanded if all the
    tokens within it exist

    for example, in the following case, the group's contents (the underscore) are
    included because the RenderPass token is filled:
    
        >>> expandFileTokens('filename[_<RenderPass>].jpg', {'RenderPass' : 'Diffuse'})
        'filename_Diffuse.jpg'

    but in this case the contents enclosed in brackets is dropped:

        >>> expandFileTokens('filename[_<RenderPass>].jpg', {})
        'filename.jpg'
    """
    if isinstance(tokens, basestring):
        tokens = dict([pair.split('=') for pair in tokens.split()])
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


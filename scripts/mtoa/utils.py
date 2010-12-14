import maya.cmds as cmds
import maya.mel as mel
import inspect

#hacked together to wrap a python function inside a mel function.
def pyToMelProc(classname, *args, **kwargs):
    #build the arguements
    melParams = ''
    pyParams = ''
    melReturn = ''

    if args:
        for arg in args:
            #if user has a tuple arguement with return then the mel proc has a return type
            if arg[0] == 'return':
                melReturn = arg[1]
                continue
            melParams += '%s $%s,'%(arg[0], arg[1])
            #if the arguement is a string we add \\" before and after
            if arg[0] == 'string':
                pyParams += '\\"\"+$%s+\"\\",'%arg[1]
            else:
                pyParams += '\"+$%s+\",'%arg[1]

    #strip the last , off the end
    melParams = melParams[:-1]
    pyParams = pyParams[:-1]

    d = {}
    #if the function was imported into the calling module then we use __module__
    #to find where it's from.  Otherwise we can inspect the stack to see what
    #module is calling this procedure.
    if '__module__' in dir(classname):
        d['modname'] = classname.__module__
    else:
        frm = inspect.stack()[1]
        mod = inspect.getmodule(frm[0])
        d['modname'] = mod.__name__

    #If user specified shortName=True then we won't append the module in front
    #to avoid clashing.  Should not specify unless overriding maya procs.
    if 'shortName' in kwargs.keys() and kwargs['shortName']:
        d['procname'] = classname.__name__
    else:
        d['procname'] = '%s_%s'%(d['modname'].replace('.', '_'), classname.__name__)

    #if user specified a string to prepend to the proc name we do it here
    if 'prepend' in kwargs.keys():
        d['procname']= '%s_%s'%(kwargs['prepend'], d['procname'])

    d['classname'] = classname.__name__
    d['melParams'] = melParams
    d['pyParams'] = pyParams
    d['melReturn'] = melReturn

    contents = '''global proc %(melReturn)s %(procname)s(%(melParams)s){'''

    if melReturn:
        contents += '''return '''

    contents += '''python("import %(modname)s;%(modname)s.%(classname)s(%(pyParams)s)");}'''

    mel.eval(contents % d)
    return d['procname']

def arnoldFixLightFilters():

    lights = cmds.ls(type='light')
    for light in lights:
        if cmds.attributeQuery("light_filters", exists=True, node=light):
                attr = '%s.light_filters'%light
                if cmds.getAttr(attr, type=True) == 'string':
                    filters = cmds.getAttr(attr)
                    tmp = filters.split(':')

                    cmds.deleteAttr(attr)
                    cmds.addAttr(ln='light_filters', at='message', multi=light)

                    for filter in filters:
                        if cmds.objExists(filter):
                            cmds.connectAttr('%s.message'%filter, '%s.[%d]'%(attr, i))

# from supportRenderers.mel
def currentRenderer():
    return cmds.getAttr('defaultRenderGlobals.currentRenderer')

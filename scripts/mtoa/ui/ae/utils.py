import maya.cmds as cmds
import maya.mel as mel
import mtoa.utils as utils
import mtoa.ui.ae

import os
import pkgutil

def arnoldGetDimValue(node, attr):

    fullAttr = '%s.%s'%(node, attr)
    value = cmds.getAttr(fullAttr)
    return value

# Dims target control if source attribute is true.
def arnoldDimControlIfTrue(node, target, source):
    dim = arnoldGetDimValue(node, source)
    cmds.editorTemplate(dimControl=(node, target, dim))

# Dims target control if source attribute is false.
def arnoldDimControlIfFalse(node, target, source):

    dim = not arnoldGetDimValue(node, source)
    cmds.editorTemplate(dimControl=(node, target, dim))

def getNodeType(name):
    nodeType = cmds.nodeType(name)
    lights = ["ambientLight",
                "directionalLight",
                "pointLight",
                "spotLight",
                "areaLight"]

    if nodeType in lights:
        nodeType = 'light'

    return nodeType

def attributeExists(attribute, nodeName):
    return cmds.attributeQuery(attribute, node=nodeName, exists=True)

def getAETemplates(package):
    templates = []
    for importer, modname, ispkg in pkgutil.iter_modules(package.__path__):
        if 'Template' in modname and modname not in templates:
            templates.append(modname)

    return templates

def loadAETemplates():
    templates = getAETemplates(mtoa.ui.ae)
    for template in templates:
        node_name = 'AE%s' % template
        _makeAEProc(template, template, node_name )

    loadAEshapesTemplate()

def loadAEshapesTemplate():
    template = 'shapeTemplate'
    _makeAEProc(template, template, 'AE%s' % template)

def aeCallback(classname):
    return utils.pyToMelProc(classname, ('string', 'nodeName'), prepend='AEArnoldCallback')

def _makeAEProc(modname, classname, procname):
    contents = '''global proc %(procname)s( string $nodeName ){
    python("import %(__name__)s;%(__name__)s._aeLoader('%(modname)s','%(classname)s','" + $nodeName + "')");}'''
    d = locals().copy()
    d['__name__'] = __name__
    mel.eval( contents % d )

def _aeLoader(modname, classname, nodename):
    mod = __import__(modname, globals(), locals(), [classname], -1)
    try:
        f = getattr(mod,classname)
        f(nodename)
    except Exception:
        print "failed to load python attribute editor template '%s.%s'" % (modname, classname)
        import traceback
        traceback.print_exc()

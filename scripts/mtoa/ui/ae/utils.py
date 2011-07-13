import maya.cmds as cmds
import maya.mel as mel
import mtoa.utils as utils
import mtoa.ui.ae

import os
import pkgutil
import re
import sys

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


def loadAETemplates():
    templates = []
    for importer, modname, ispkg in pkgutil.iter_modules(mtoa.ui.ae.__path__):
        if modname.endswith('Template') and modname not in templates:
            templates.append(modname)
            procName = 'AE%s' % modname
            _makeAEProc(modname, modname, procName)

    loadAEshapesTemplate()

def loadAEshapesTemplate():
    template = 'shapeTemplate'
    _makeAEProc(template, template, 'AE%s' % template)

def aeCallback(func):
    return utils.pyToMelProc(func, [('string', 'nodeName')], procPrefix='AEArnoldCallback')

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

def interToUI(label):
    label = re.sub('([a-z])([A-Z])', r'\1 \2', label.replace('_', ' '))
    label = re.sub('(\s[a-z])|(^[a-z])', lambda m: m.group().upper(), label)
    return label

def attrType(attr):
    type = cmds.getAttr(attr, type=True)
    if type == 'float3':
        if cmds.addAttr(attr, q=True, usedAsColor=True):
            type = 'color'
    return type

class AttrControlGrp(object):
    UI_TYPES = {
        'float':cmds.attrFieldSliderGrp,
        'float2':cmds.attrFieldGrp,
        'float3':cmds.attrFieldGrp,
        'color':cmds.attrColorSliderGrp,
        'bool':cmds.attrControlGrp,
        'long':cmds.attrFieldSliderGrp,
        'long2':cmds.attrFieldGrp,
        'long3':cmds.attrFieldGrp,
        'short':cmds.attrFieldSliderGrp,
        'short2':cmds.attrFieldGrp,
        'short3':cmds.attrFieldGrp,
        'enum':cmds.attrEnumOptionMenuGrp,
        'double':cmds.attrFieldSliderGrp,
        'double2':cmds.attrFieldGrp,
        'double3':cmds.attrFieldGrp,
        'string':cmds.attrControlGrp,
        'message':cmds.attrNavigationControlGrp
    }
    def __init__(self, attribute, *args, **kwargs):
        self.attribute = attribute
        self.type = kwargs.pop('type', kwargs.pop('typ', None))
        if not self.type:
            self.type = attrType(self.attribute)
        kwargs['attribute'] = self.attribute
        self.control = self.UI_TYPES[self.type](*args, **kwargs)

    def edit(self, **kwargs):
        kwargs['edit'] = True
        self.UI_TYPES[self.type](self.control, **kwargs)

    def setAttribute(self, attribute):
        self.attribute = attribute
        self.UI_TYPES[self.type](self.control, edit=True, attribute=self.attribute)

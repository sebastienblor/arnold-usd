import maya.cmds as cmds
import maya.mel as mel
import mtoa.utils as utils
import mtoa.ui.ae

import os
import pkgutil
import re

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

def interToUI(label):
    label = re.sub('([a-z])([A-Z])', r'\1 \2', label.replace('_', ' '))
    label = re.sub('(\s[a-z])|(^[a-z])', lambda m: m.group().upper(), label)
    return label

class AttrControlGrp(object):
    AttrUITypes = {
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
        'string':cmds.attrFieldGrp,
        'message':cmds.attrNavigationControlGrp
    }
    def __init__(self, *args, **kwargs):
        self.type = kwargs.pop('type', kwargs.pop('typ', None))
        self.attribute = kwargs.pop('attribute', kwargs.pop('a', None))
        
        if self.attribute:
            kwargs['attribute'] = self.attribute
        else:
            return None
        
        label = kwargs.pop('label', kwargs.pop('l', None))
        if label:
            kwargs['label'] = interToUI(label)
        
        self.control = self.AttrUITypes[self.type](*args, **kwargs)

    def edit(self, *args, **kwargs):
        type = kwargs.pop('type', kwargs.pop('typ', None))
        if not args:
            args=[self.control]
        if type:
            self.type = type
        kwargs['edit'] = True
        self.AttrUITypes[self.type](*args, **kwargs)

    def updateAttribute(self, *args, **kwargs):
        self.edit(attribute=self.attribute)

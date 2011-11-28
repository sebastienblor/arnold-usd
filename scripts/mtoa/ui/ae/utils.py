import pymel.core as pm
import mtoa.utils as utils
import mtoa.ui.ae

import os
import pkgutil
import re
import sys
import inspect

def arnoldGetDimValue(node, attr):

    fullAttr = '%s.%s'%(node, attr)
    value = pm.getAttr(fullAttr)
    return value

# Dims target control if source attribute is true.
def arnoldDimControlIfTrue(node, target, source):
    dim = arnoldGetDimValue(node, source)
    pm.editorTemplate(dimControl=(node, target, dim))

# Dims target control if source attribute is false.
def arnoldDimControlIfFalse(node, target, source):
    dim = not arnoldGetDimValue(node, source)
    pm.editorTemplate(dimControl=(node, target, dim))

def getNodeType(name):
    nodeType = pm.nodeType(name)
    lights = ["ambientLight",
                "directionalLight",
                "pointLight",
                "spotLight",
                "areaLight"]

    if nodeType in lights:
        nodeType = 'light'

    return nodeType

def attributeExists(attribute, nodeName):
    return pm.attributeQuery(attribute, node=nodeName, exists=True)


def loadAETemplates():
    templates = []
    for importer, modname, ispkg in pkgutil.iter_modules(mtoa.ui.ae.__path__):
        # module name must end in "Template"
        if modname.endswith('Template') and modname not in templates:
            # TODO: use importer?
            mod = __import__(modname, globals(), locals(), [], -1)
            procName = 'AE%s' % modname
            if hasattr(mod, modname):
                # a function named after the module
                templates.append(modname)
                _makeAEProc(modname, modname, procName)
            elif hasattr(mod, procName):
                # a class named AEmodname
                templates.append(modname)
                _makeAEProc(modname, procName, procName)
    loadAEshapesTemplate()

def loadAEshapesTemplate():
    template = 'shapeTemplate'
    _makeAEProc(template, template, 'AE%s' % template)

def aeCallback(func):
    return utils.pyToMelProc(func, [('string', 'nodeName')], procPrefix='AEArnoldCallback')

def _makeAEProc(modname, objname, procname):
    contents = '''global proc %(procname)s( string $nodeName ){
    python("import %(__name__)s;%(__name__)s._aeLoader('%(modname)s','%(objname)s','" + $nodeName + "')");}'''
    d = locals().copy()
    d['__name__'] = __name__
    pm.mel.eval( contents % d )

def _aeLoader(modname, objname, nodename):
    mod = __import__(modname, globals(), locals(), [objname], -1)
    try:
        f = getattr(mod, objname)
        if inspect.isfunction(f):
            f(nodename)
        elif inspect.isclass(f):
            f(nodename, doSetup=True)
        else:
            print "AE object %s has invalid type %s" % (f, type(f))
    except Exception:
        print "failed to load python attribute editor template '%s.%s'" % (modname, objname)
        import traceback
        traceback.print_exc()

def interToUI(label):
    label = re.sub('([a-z])([A-Z])', r'\1 \2', label.replace('_', ' '))
    label = re.sub('(\s[a-z])|(^[a-z])', lambda m: m.group().upper(), label)
    return label

def attrType(attr):
    type = pm.getAttr(attr, type=True)
    if type == 'float3':
        node, at = attr.split('.', 1)
        if pm.attributeQuery(at, node=node, usedAsColor=1):
            type = 'color'
    return type

def rebuildAE():
    "completely rebuild the attribute editor"
    edForm = pm.melGlobals['gAttributeEditorForm']
    if pm.layout(edForm, q=True, exists=True):
        children = pm.layout(edForm, q=True, childArray=True)
        if children:
            pm.deleteUI(children[0])
            pm.mel.attributeEditorVisibilityStateChange(1, "")

class AttrControlGrp(object):
    UI_TYPES = {
        'float':pm.cmds.attrFieldSliderGrp,
        'float2':pm.cmds.attrFieldGrp,
        'float3':pm.cmds.attrFieldGrp,
        'color':pm.cmds.attrColorSliderGrp,
        'bool':pm.cmds.attrControlGrp,
        'long':pm.cmds.attrFieldSliderGrp,
        'long2':pm.cmds.attrFieldGrp,
        'long3':pm.cmds.attrFieldGrp,
        'short':pm.cmds.attrFieldSliderGrp,
        'short2':pm.cmds.attrFieldGrp,
        'short3':pm.cmds.attrFieldGrp,
        'enum':pm.cmds.attrEnumOptionMenuGrp,
        'double':pm.cmds.attrFieldSliderGrp,
        'double2':pm.cmds.attrFieldGrp,
        'double3':pm.cmds.attrFieldGrp,
        'string':pm.cmds.attrControlGrp,
        'message':pm.cmds.attrNavigationControlGrp
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

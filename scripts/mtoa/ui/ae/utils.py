import pymel.core as pm
import mtoa.utils as utils
import maya.mel as mel
import mtoa.ui.ae
import maya.cmds
import os
import pkgutil
import re
import sys
import inspect
import maya.cmds as cmds

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
    lights = ["directionalLight",
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
    customTemplatePaths = []
    
    if (os.getenv('MTOA_TEMPLATES_PATH')):
        import sys
        customTemplatePaths = os.getenv('MTOA_TEMPLATES_PATH').split(os.pathsep)
        sys.path += customTemplatePaths
        
    pathsList = mtoa.ui.ae.__path__ + customTemplatePaths
    
    for importer, modname, ispkg in pkgutil.iter_modules(pathsList):
        # module name must end in "Template"
        if modname.endswith('Template') and modname not in templates:
            # TODO: use importer?
            try:
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
            except:
                print '[MtoA] Error parsing AETemplate file %s' % str(modname)
                import traceback
                print traceback.format_exc()

def aeCallback(func):
    return utils.pyToMelProc(func, [('string', 'nodeName')], procPrefix='AEArnoldCallback')

def _makeAEProc(modname, objname, procname):
    contents = '''global proc %(procname)s( string $nodeName ){
    python("import %(__name__)s;%(__name__)s._aeLoader('%(modname)s','%(objname)s','" + $nodeName + "')");}'''
    d = locals().copy()
    d['__name__'] = __name__
    mel.eval( contents % d )

def _aeLoader(modname, objname, nodename):
    mod = __import__(modname, globals(), locals(), [objname], -1)
    try:
        f = getattr(mod, objname)
        if inspect.isfunction(f):
            f(nodename)
        elif inspect.isclass(f):
            inst = f(cmds.nodeType(nodename))
            inst._doSetup(nodename)
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
    type = cmds.getAttr(attr, type=True)
    if type == 'float3':
        node, at = attr.split('.', 1)
        if cmds.attributeQuery(at, node=node, usedAsColor=1):
            type = 'color'
    return type

def rebuildAE():
    "completely rebuild the attribute editor"
    edForm = pm.melGlobals['gAttributeEditorForm']
    if cmds.layout(edForm, q=True, exists=True):
        children = cmds.layout(edForm, q=True, childArray=True)
        if children:
            cmds.deleteUI(children[0])
            pm.mel.attributeEditorVisibilityStateChange(1, "")

def attrTextFieldGrp(*args, **kwargs):
    """
    There is a bug with attrControlGrp and string attributes where it ignores
    any attempt to edit the current attribute.  So, we have to write our own
    replacement
    """
    attribute = kwargs.pop('attribute', kwargs.pop('a', None))
    assert attribute is not None, "You must passed an attribute"
    changeCommand = kwargs.pop('changeCommand', kwargs.pop('cc', None))
    if changeCommand:
        def cc(newVal):
            cmds.setAttr(attribute, newVal, type="string")
            changeCommand(newVal)
    else:
        cc = lambda newVal: cmds.setAttr(attribute, newVal, type="string")

    if kwargs.pop('edit', kwargs.pop('e', False)):
        ctrl = args[0]
        cmds.textFieldGrp(ctrl, edit=True,
                    text=cmds.getAttr(attribute),
                    changeCommand=cc)
        cmds.scriptJob(parent=ctrl,
                     replacePrevious=True,
                     attributeChange=[attribute,
                                      lambda: cmds.textFieldGrp(ctrl, edit=True,
                                                              text=cmds.getAttr(attribute))])
    elif kwargs.pop('query', kwargs.pop('q', False)):
        # query
        pass
    else:
        # create
        labelText = kwargs.pop('label', None)
        if not labelText:
            labelText = pm.mel.interToUI(attribute.split('.')[-1])
        ctrl = None
        if len(args) > 0:
            ctrl = args[0]
            cmds.textFieldGrp(ctrl,
                            label=labelText,
                            text=cmds.getAttr(attribute),
                            changeCommand=cc)
        else:
            ctrl = cmds.textFieldGrp(label=labelText,
                                   text=cmds.getAttr(attribute),
                                   changeCommand=cc)
        cmds.scriptJob(parent=ctrl,
                     attributeChange=[attribute,
                                      lambda: cmds.textFieldGrp(ctrl, edit=True,
                                                              text=cmds.getAttr(attribute))])
        return ctrl

def attrBoolControlGrp(*args, **kwargs):
    attribute = kwargs.pop('attribute', kwargs.pop('a', None))
    assert attribute is not None, "You must passed an attribute"
    changeCommand = kwargs.pop('changeCommand', kwargs.pop('cc', None))
    if changeCommand:
        def cc(newVal):
            cmds.setAttr(attribute, newVal)
            changeCommand(newVal)
    else:
        cc = lambda newVal: cmds.setAttr(attribute, newVal)

    if kwargs.pop('edit', kwargs.pop('e', False)):
        ctrl = args[0]
        cmds.checkBox(ctrl, edit=True,
                    value=cmds.getAttr(attribute),
                    changeCommand=cc)
        cmds.scriptJob(parent=ctrl,
                     replacePrevious=True,
                     attributeChange=[attribute,
                                      lambda: cmds.checkBox(ctrl, edit=True, value=cmds.getAttr(attribute))])
    elif kwargs.pop('query', kwargs.pop('q', False)):
        # query
        pass
    else:
        # create
        labelText = kwargs.pop('label', None)
        if not labelText:
            labelText = pm.mel.interToUI(attribute.split('.')[-1])
        ctrl = args[0]
        cmds.rowLayout(numberOfColumns=1, columnWidth1=285, columnAttach1='right')
        cmds.checkBox(ctrl, label=labelText,
                    value=cmds.getAttr(attribute),
                    changeCommand=cc)
        cmds.setParent('..')
        cmds.scriptJob(parent=ctrl,
                     attributeChange=[attribute,
                     lambda: cmds.checkBox(ctrl, edit=True, value=cmds.getAttr(attribute))])
        return ctrl

class AttrControlGrp(object):
    UI_TYPES = {
        'float':  cmds.attrFieldSliderGrp,
        'float2': cmds.attrFieldGrp,
        'float3': cmds.attrFieldGrp,
        'color':  cmds.attrColorSliderGrp,
        'bool':   cmds.attrControlGrp,
        'long':   cmds.attrFieldSliderGrp,
        'byte':   cmds.attrFieldSliderGrp,
        'long2':  cmds.attrFieldGrp,
        'long3':  cmds.attrFieldGrp,
        'short':  cmds.attrFieldSliderGrp,
        'short2': cmds.attrFieldGrp,
        'short3': cmds.attrFieldGrp,
        'enum':   cmds.attrEnumOptionMenuGrp,
        'double': cmds.attrFieldSliderGrp,
        'double2':cmds.attrFieldGrp,
        'double3':cmds.attrFieldGrp,
        'string': attrTextFieldGrp,
        'message':cmds.attrNavigationControlGrp
    }
    def __init__(self, attribute, *args, **kwargs):
        self.attribute = attribute
        self.type = kwargs.pop('type', kwargs.pop('typ', None))
        if not self.type:
            self.type = attrType(self.attribute)

        if self.type in ['color', 'enum', 'message']:
            self.callback = kwargs.pop('changeCommand', None)
        else:
            self.callback = None
        kwargs['attribute'] = self.attribute
        if self.type not in self.UI_TYPES:
            return
        cmd = self.UI_TYPES[self.type]
        try:
            self.control = cmd(*args, **kwargs)
        except RuntimeError:
            print "Error creating %s:" % cmd.__name__
            raise
        if self.callback:
            cmds.scriptJob(attributeChange=[self.attribute, self.callback],
                         replacePrevious=True, parent=self.control)

    def edit(self, **kwargs):
        kwargs['edit'] = True
        if self.type not in self.UI_TYPES:
            return
        self.UI_TYPES[self.type](self.control, **kwargs)

    def setAttribute(self, attribute):
        self.attribute = attribute
        if self.type not in self.UI_TYPES:
            return
        self.UI_TYPES[self.type](self.control, edit=True, attribute=self.attribute)
        if self.callback:
            cmds.scriptJob(attributeChange=[self.attribute, self.callback],
                         replacePrevious=True, parent=self.control)

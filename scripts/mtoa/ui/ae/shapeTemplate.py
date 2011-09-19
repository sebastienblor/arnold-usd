import maya.cmds as cmds
import maya.mel as mel
import pymel.core as pm
from maya.utils import executeDeferred
from mtoa.ui.ae.utils import aeCallback, AttrControlGrp
from mtoa.utils import prettify
import mtoa.callbacks as callbacks
import mtoa.core as core
import arnold

from collections import defaultdict
import inspect

global _translatorTemplates
_translatorTemplates = defaultdict(dict)

global _templates
_templates = {}

def getTranslators(nodeType):
    return [x[0] for x in core.listTranslators(nodeType)]

#-------------------------------------------------
# translator defaults
#-------------------------------------------------

def registerDefaultTranslator(nodeType, stringOrFunc):
    """
    Register the default translator for a node type. The second argument identifies the name of the
    translator.  Pass a string if the default is always the same,
    or a function that takes the current node and returns a string.

    The default will automatically be set whenever a node of the given type is added to the scene.
    """

    global _templates
    try:
        inst = _templates[nodeType]
    except KeyError:
        # create a new subclass
        inst = TranslatorControl(nodeType)
        _templates[nodeType] = inst

    inst.setDefaultTranslator(stringOrFunc)

    # set defaults for existing nodes
    for node in cmds.ls(exactType=nodeType):
        # this will set aiTranslator if it is not set
        inst.getCurrentTranslator(node)

    callbacks.addNodeAddedCallback(inst._doSetDefaultTranslator, nodeType)


#-------------------------------------------------
# AE templates
#-------------------------------------------------

def getTranslatorTemplates(nodeType):
    """
    Return a dictionary of {translatorName : [template instance]} for the given nodeType
    """
    # return a copy so it doesn't get messed with
    global _translatorTemplates
    return dict(_translatorTemplates[nodeType])

def getTranslatorTemplate(nodeType, translatorName):
    """
    return a template instance for the given nodeType, or None if one has not been registered
    """
    try:
        return getTranslatorTemplates(nodeType)[translatorName]
    except KeyError:
        pass

class BaseTemplate(object):
    """
    This class provides a simple framework for creating UIs.
    """
    def __init__(self, nodeType):
        self._nodeType = nodeType
        self._nodeName = None

    def __repr__(self):
        return '%s(%r)' % (self.__class__.__name__, self.nodeType())

    def _doSetup(self, nodeAttr, *args):
        '''
        build the UI from the list of added attributes
        '''
        self._setActiveNode(nodeAttr.split('.')[0])
        self.setup()

    def _doUpdate(self, nodeAttr, *args):
        self._setActiveNode(nodeAttr.split('.')[0])
        self.update()

    def _setActiveNode(self, nodeName):
        "set the active node"
        self._nodeName = nodeName

    # queries
    @property
    def nodeName(self):
        "get the active node"
        # assert self._nodeName, "%r: nodeName should be set by now" % self
        return self._nodeName

    def nodeType(self):
        if self._nodeType is None:
            self._nodeType = cmds.objectType(self.nodeName)
        return self._nodeType

    def nodeAttr(self, attr):
        return self.nodeName + '.' + attr

    def nodeAttrExists(self, attr):
        return cmds.addAttr(self.nodeAttr(attr), q=True, ex=True)


class AttributeTemplate(BaseTemplate):
    """
    This class provides a framework for creating and managing AE-like templates and is largely compatible with pymel's
    AETemplate class.
    """
    def __init__(self, nodeType):
        super(AttributeTemplate, self).__init__(nodeType)
        self._controls = []
        self._builders = []
        self._layoutStack = []

    def _doSetup(self, attr):
        '''
        build the UI from the list of added attributes
        '''
        self._setActiveNode(attr.split('.')[0])
        cmds.setUITemplate('attributeEditorTemplate', pushTemplate=True)
        self._layoutStack.append(cmds.setParent(query=True))
        self.setup()
        cmds.setUITemplate(popTemplate=True)

    def _doUpdate(self, attr):
        self._setActiveNode(attr.split('.')[0])
        self.update()

    def addChildTemplate(self, attr, template):
        if isinstance(template, pm.uitypes.AETemplate):
            print "this is a pm.uitypes.AETemplate subclass. this will probably break"
        self.addCustom(attr, template._doSetup, template._doUpdate)

    def setup(self):
        """
        this method should be overridden. it is called when the class is initialized. it is kept as a
        separate method to avoid the user coming into conflict with variables managed by this class
        """
        pass

    def update(self):
        cmds.setUITemplate('attributeEditorTemplate', pushTemplate=True)
        for attr, updateFunc, parent in self._controls:
            cmds.setParent(parent)
            updateFunc(self.nodeAttr(attr))
        cmds.setUITemplate(popTemplate=True)

    # building
    def _manageControl(self, attr, updateFunc, parent):
        self._controls.append((attr, updateFunc, parent))

    def addControl(self, attr, label=None, annotation=None):
        # TODO: lookup label and descr from metadata
        if not label:
            label = prettify(attr)
            if label.startswith('Ai '):
                label = label[3:]
        kwargs = {}
        kwargs['label'] = label
        kwargs['attribute'] = self.nodeAttr(attr)
        if annotation:
            kwargs['annotation'] = annotation
        parent = self._layoutStack[-1]
        cmds.setParent(parent)
        control = AttrControlGrp(**kwargs)
        self._manageControl(attr, control.setAttribute, parent)

    def addSeparator(self):
        cmds.separator()

    def addCustom(self, attr, createFunc, updateFunc):
        parent = self._layoutStack[-1]
        cmds.setParent(parent)
        createFunc(self.nodeAttr(attr))
        self._manageControl(attr, updateFunc, parent)

    def beginLayout(self, label, **kwargs):
        '''
        begin a frameLayout.
        accepts any keyword args valid for creating a frameLayout
        '''
        kwargs['label'] = label
        cmds.setParent(self._layoutStack[-1])
        cmds.frameLayout(**kwargs)
        self._layoutStack.append(cmds.columnLayout(adjustableColumn=True))

    def endLayout(self):
        '''
        end the current frameLayout
        '''
        self._layoutStack.pop()
        cmds.setParent(self._layoutStack[-1])

    # for compatibility with pymel.core.uitypes.AETemplate
    def beginNoOptimize(self):
        pass

    # for compatibility with pymel.core.uitypes.AETemplate
    def endNoOptimize(self):
        pass

    def attachToAE(self):
        "add the appropriate callbacks to the editor template"
        #super(AttributeTemplate, self).attachToAE()
#        for attr in self.getAttributes():
#            cmds.editorTemplate(suppress=attr)

class AttributeEditorTemplate(pm.uitypes.AETemplate):
    """
    A sub-class of pymel.uitypes.AETemplate.  A properly defined AETemplate class will automatically
    register itself as the AE Template for the specified node type.  (See the documentation for pymel.uitypes.AETemplate for more
    on how to specify the desired node type).

    This sub-class adds a compatibility layer to make it behave more like mtoa's AttributeTemplate class,
    which is used for translator UIs. 

    One tricky consideration is that pymel.uitypes.AETemplate triggers building immediately upon init, but for our uses, it is
    preferable to store an instantiated class and build it later, which is how AttributeTemplate works.  The 
    AttributeTemplate.autoBuild boolean exists for this purpose.
    It allows for auto-building to be globally disabled, while avoiding adding an additional argument to __init__, which
    would make it incompatible with AttributeTemplate

    AttributeEditorTemplates are used for:
        - Full AE Node Templates
        - Partial AE Templates that are used inline (cannot be used with callCustom)

    AttributeTemplates are used for:
        - Partial AE Templates that are used with callCustom
    """
    autoBuild = True
    def __init__(self, arg):
        if self.autoBuild:
            # arg is a node name
            super(AttributeEditorTemplate, self).__init__(arg)
            self._doSetup(arg)
        else:
            # argument is a node type
            self._nodeName = None
            self._nodeType = arg

    def _doSetup(self, node, *args):
        '''
        build the UI from the list of added attributes
        '''
        self._setActiveNode(node)
        self.setup()

    def _doUpdate(self, node, *args):
        self._setActiveNode(node)
        self.update()

    def _setActiveNode(self, nodeName):
        "set the active node"
        self._nodeName = nodeName

    def setup(self):
        pass

    def update(self):
        pass

    def addCustom(self, attr, newFunc, replaceFunc):
        self.callCustom(newFunc, replaceFunc, attr)

    def addChildTemplate(self, attr, template):
        if isinstance(template, pm.uitypes.AETemplate):
            template._doSetup(attr)
        else:
            cmds.editorTemplate(aeCallback(template._doSetup),
                                aeCallback(template._doUpdate),
                                attr,
                                callCustom=True)

class ShapeMixin(object):
    def renderStatsAttributes(self):
        self.addControl("castsShadows")
        self.addControl("receiveShadows")
        self.addControl("primaryVisibility")
        self.addControl("visibleInReflections")
        self.addControl("visibleInRefractions")

    def commonShapeAttributes(self):
        self.addControl("aiSelfShadows")
        self.addControl("aiOpaque")
        self.addControl("aiVisibleInDiffuse")
        self.addControl("aiVisibleInGlossy")

class ShapeAETemplate(AttributeEditorTemplate, ShapeMixin):
    pass

class ShapeTranslatorTemplate(AttributeTemplate, ShapeMixin):
    pass


#class AttributeTemplate(AttributeTemplate):
#    """
#    To implement an AE template for a custom translator, create a subclass of this class and
#    register it using registerTranslatorUI
#    """
#    def __init__(self, nodeType):
#        super(AttributeTemplate, self).__init__(nodeType)
#        self._keyableDefaults = {}
#
#    def showInChannelBox(self, enabled):
#        for attr in self.getAttributes():
#            type = self.nodeAttrType(attr)
#            keyable = enabled and cmds.attributeQuery(attr, node=self.nodeName, keyable=True)
#            if cmds.attributeQuery(attr, node=self.nodeName, numberOfChildren=True):
#                children = cmds.attributeQuery(attr, node=self.nodeName, listChildren=True)
#                for c in children:
#                    # some sort of a bug forces a call like this in order to set keyable and channelbox correctly...
#                    cmds.setAttr(self.nodeAttr(c), channelBox=enabled, keyable=keyable)
#                    cmds.setAttr(self.nodeAttr(c), keyable=keyable)
#            else:
#                # some sort of a bug forces a call like this in order to set keyable and channelbox correctly...
#                cmds.setAttr(self.nodeAttr(attr), channelBox=enabled, keyable=keyable)
#                cmds.setAttr(self.nodeAttr(attr), keyable=keyable)
#
#    @staticmethod
#    def syncChannelBox(nodeName, nodeType, default):
#        """
#        make only the attributes for the active translator visible in the channel box
#        """
#        templates = getTranslatorTemplates(nodeType)
#        # Do the actual syncing with the ChannelBox
#        for name, template in templates.items():
#            if name == default:
#                continue
#            template._setActiveNode(nodeName)
#            template.showInChannelBox(False)
#        # We need to run this last for cases where templates share attributes
#        if default in templates:
#            templates[default]._setActiveNode(nodeName)
#            templates[default].showInChannelBox(True)

class AutoTranslatorTemplate(AttributeTemplate):
    _arnoldNodeType = None

    def setup(self):
        """
        default setup automatically builds a UI based on metadata
        """
        for paramName, attrName, label, annotation in core.getAttributeData(self._arnoldNodeType):
            self.addControl(attrName,
                            label if label else prettify(paramName),
                            annotation)

class DisableLoader(pm.uitypes.AELoader):
    """
    Metaclass which disables the automatic loading behavior of AETemplate
    """
    def __new__(cls, classname, bases, classdict):
        return type.__new__(cls, classname, bases, classdict)

class TranslatorControl(AttributeEditorTemplate):
    '''
    Allows multiple AttributeTemplates, each representing an arnold translator, to be controlled via
    one optionMenu, such that only the active template is visible.  A default controller is automatically created for
    each node that has registered arnold translator UIs. Manually creating a TranslatorControl is only necessary if you
    need to customize the default controller behavior.
    '''
    __metaclass__ = DisableLoader
    def __init__(self, nodeType, label='Arnold Translator', controlAttr='aiTranslator', default=None, optionMenuName=None):
        self.autoBuild = False
        super(TranslatorControl, self).__init__(nodeType)
        self.autoBuild = True
        self._optionMenu = optionMenuName if optionMenuName is not None else controlAttr + "OMG"
        self._translators = None
        self._label = label

        # class attributes
        self._attr = controlAttr
        if not (default is None or isinstance(default, basestring) or callable(default)):
            cmds.warning("[mtoa] default translator must be a string or a function")
            return
        self._default = default

    #---- translator methods

    def nodeType(self):
        return self._nodeType

    def _doSetDefaultTranslator(self, node):
        try:
            cmds.setAttr(node + '.' + self._attr, self.getDefaultTranslator(node), type='string')
        except RuntimeError:
            cmds.warning("failed to set default translator for %s" % node.name())

    def setDefaultTranslator(self, default):
        self._default = default

    def getDefaultTranslator(self, node):
        if isinstance(self._default, basestring):
            return self._default
        elif callable(self._default):
            return self._default(node)

    def getCurrentTranslator(self, nodeName):
        """
        get the current translator for this node, querying and setting the default if not yet set
        """
        try :
            transName = cmds.getAttr(nodeName + "." + self._attr)
        except :
            transName = None
        translators = self.getTranslators()
        if not transName or transName not in translators:
            # set default
            transName = self.getDefaultTranslator(nodeName)
            if transName is None:
                if not translators:
                    cmds.warning("cannot find default translator for %s" % nodeName)
                    return
                transName = translators[0]
            try :
                cmds.setAttr(nodeName + "." + self._attr, transName, type='string')
            except :
                cmds.warning("cannot set default translator for %s" % nodeName)
        return transName

    def updateChildrenCallback(self, attr):
        """
        this function is assigned to an AE refresh callback. this allows us to call updateChildren
        to set things up.
        """
        # attr should be aiTranslator. do we need to split?
        nodeName = attr.split('.')[0]
        self.updateChildren(nodeName, cmds.getAttr(nodeName + "." + self._attr))

    def updateChildren(self, nodeName, currentTranslator):
        """
        update the translator UI, which consists of an optionMenuGrp and a frameLayout per translator,
        so that only the frameLayout corresponding to the currently selected translator is visible
        """
        if not cmds.layout(self._optionMenu, exists=True):
            # not built yet
            return
        fullpath = cmds.layout(self._optionMenu, query=True, fullPathName=True)
        # get the grand-parent columnLayout
        gparent = fullpath.rsplit('|', 2)[0]
        # get the great-grand parent frame layout
        frame = fullpath.rsplit('|', 3)[0]
        try:
            cmds.frameLayout(frame, edit=True, collapsable=False, labelVisible=False)
        except RuntimeError:
            # this is a little dirty: it will only succeed when attaching to AE
            pass
        children = cmds.layout(gparent, query=True, childArray=True)
        # hide all frameLayouts but ours
        assert currentTranslator, "we should have a translator set by now"

        for child in children[:-1]:
            # is it a frame layout?
            objType = cmds.objectTypeUI(child)
            if objType == 'frameLayout':
                label = cmds.frameLayout(child, query=True, label=True)
                # turn collapsable and label off
                cmds.frameLayout(child, edit=True, collapsable=False, labelVisible=False,
                                 visible=(label == currentTranslator))
        # FIXME: this needs a check for read-only nodes from referenced files. also, not sure
        # changing attribute properties is the best approach
        #AttributeTemplate.syncChannelBox(nodeName, nodeType, currentTranslator)
        # last child is the 'hide_me' control that always needs to be hidden
        cmds.layout(children[-1], edit=True, visible=False)

    def menuChanged(self, nodeName, currentTranslator):
        """
        called when the translator optionMenuGrp (aiTranslatorOMG) changes
        """
        cmds.setAttr(nodeName + "." + self._attr, currentTranslator, type='string')
        self.updateChildren(nodeName, currentTranslator)

    def createMenu(self, nodeName):
        """
        called to create an optionMenuGrp for choosing between multiple translator options for a given node
        """
        self._optionMenu = cmds.optionMenuGrp(self._optionMenu, label=self._label,
                                             cc=lambda *args: self.menuChanged(nodeName, args[0]))
        # create menu items
        for tran in self.getTranslators():
            cmds.menuItem(label=tran)
        cmds.setParent(menu=True)

        transName = self.getCurrentTranslator(nodeName)
        cmds.optionMenuGrp(self._optionMenu, edit=True, value=transName)

    def updateMenu(self, nodeName):
        """
        called to update an optionMenuGrp for choosing between multiple translator options for a given node
        """
        # delete current options
        translators = cmds.optionMenuGrp(self._optionMenu, q=True, itemListLong=True)
        for tran in translators:
            cmds.deleteUI(tran, menuItem=True)

        # populate with a fresh list
        parent = cmds.setParent(self._optionMenu)
        for tran in self._translators:
            cmds.menuItem(label=tran, parent=parent + '|OptionMenu')

        transName = self.getCurrentTranslator(nodeName)
        cmds.optionMenuGrp(self._optionMenu, edit=True, value=transName,
                           cc=lambda *args: self.menuChanged(nodeName, args[0]))
        self.updateChildren(nodeName, transName)

    def getTranslators(self):
        if self._translators is None:
            self._translators = getTranslators(self.nodeType())
        return self._translators

    def getTranslatorTemplates(self):
        return filter(lambda x: bool(x[1]),
                      [(translator, getTranslatorTemplate(self.nodeType(), translator)) \
                        for translator in self.getTranslators()])

    def setup(self):
        translatorTemplates = self.getTranslatorTemplates()
        if translatorTemplates:
            if len(translatorTemplates) > 1:
                cmds.editorTemplate(beginLayout='hide', collapse=False)
                # if there is more than one translator, we group each in its own layout
                # create the menu for selecting the translator
                cmds.editorTemplate(aeCallback(lambda attr: self.createMenu(attr.split('.')[0])),
                                    aeCallback(lambda attr: self.updateMenu(attr.split('.')[0])),
                                    self._attr, callCustom=True)
                for translator, template in translatorTemplates:
                    # we always create a layout, even if it's empty
                    cmds.editorTemplate(beginLayout=translator, collapse=False)
                    self.addChildTemplate('message', template)
                    cmds.editorTemplate(endLayout=True)
                # timing on AE's is difficult: the frameLayouts are not created at this point even though
                # the `editorTemplate -beginLayout` calls have been made. this is a little hack
                # to ensure we get a callback after the AE ui elements have been built: normal controls can get
                # an update callback, but we don't have any normal controls around, so we'll have to make one and
                # hide it
                cmds.editorTemplate('message',
                                    aeCallback(self.updateChildrenCallback),
                                    addDynamicControl=True, label='hide_me')
                cmds.editorTemplate(endLayout=True)
            else:
                translator, template = translatorTemplates[0]
                self.addChildTemplate('message', template)

class TranslatorControlUI(TranslatorControl):

    def addChildTemplate(self, attr, template, parent=None):
        "add the appropriate callbacks to the current UI"
        currParent = cmds.setParent(query=True)
        if parent is not None:
            cmds.setParent(parent)
        template._doSetup(self.nodeName)
        if currParent is not None and currParent != '' :
            cmds.setParent(currParent)

    def setup(self):
        translatorTemplates = self.getTranslatorTemplates()
        if translatorTemplates:
            mainCol = cmds.columnLayout(
                                        adjustableColumn=True
                                        )
            if len(translatorTemplates) > 1:
                # if there is more than one translator, we group each in its own layout
                # FIXME: reduce this to one call:
                cmds.columnLayout()
                # create the menu for selecting the translator
                self.createMenu(self.nodeName)
                cmds.setParent(mainCol)
                for translator, template in translatorTemplates:

                    # we always create a layout, even if it's empty
                    cmds.frameLayout(label=translator, collapse=False)
                    cmds.columnLayout(
                                      adjustableColumn=True,
                                      #columnAttach=("both", 0)
                                      )
                    self.addChildTemplate('message', template)
                    cmds.setParent(mainCol)
                # for compatibility with AE templates
                cmds.columnLayout()
                cmds.text(label='hide_me')
                cmds.setParent('..')
                self.updateMenu(self.nodeName)
            else:
                translator, template = translatorTemplates[0]
                self.addChildTemplate('message', template)
            cmds.setParent(mainCol)
            cmds.setParent('..')

def registerAETemplate(templateClass, nodeType):
    assert inspect.isclass(templateClass) and issubclass(templateClass, (AttributeTemplate, AttributeEditorTemplate)), \
        "you must pass a subclass of AttributeTemplate or AttributeEditorTemplate"
    print "registering attribute template for %s" % nodeType
    global _templates
    if nodeType not in _templates:
        AttributeEditorTemplate.autoBuild = False
        try:
            _templates[nodeType] = templateClass(nodeType)
        except:
            print "Failed to instantiate AE Template", templateClass
        finally:
            AttributeEditorTemplate.autoBuild = True

def aeTemplate(nodeType, baseClass=AttributeTemplate):
    """
    decorator
    """
    def registerUIDecorator(func):
        cls = type(nodeType + "Template", (baseClass,), dict(setup=func))
        registerAETemplate(cls, nodeType)
        # return function unchanged
        return func
    return registerUIDecorator

def registerTranslatorUI(templateClass, nodeType, translatorName='<built-in>'):
    """
    A translator UI is a specialized attribute template based on the AttributeTemplate class. 
    
    Every node type can have multiple mtoa translators. Each translator UI class
    is responsible for creating the UI for a single translator.  The TranslatorControl class creates a parent
    UI which switches between the available translator UIs. It is automatically created when the first translator
    UI is registered.
    """
    global _translatorTemplates
#    assert inspect.isclass(templateClass) and issubclass(templateClass, AttributeTemplate),\
#        "you must pass a subclass of AttributeTemplate"
    _translatorTemplates[nodeType][translatorName] = templateClass(nodeType)

    registerAETemplate(TranslatorControl, nodeType)

# FIXME: should we just get rid of this?
def translatorUI(nodeType, translatorName='<built-in>', baseClass=AttributeTemplate):
    """
    Decorator for registering a function for creating a simple translator UI. 
    
    Normally an AttributeTemplate sub-class
    would be created manually, but for simple UIs that require only one function, this decorator can reduce the boiler-plate
    class code.  The function that it is applied to should be written to receive an AttributeTemplate instance, which it should
    use to make calls to addControl, addSeparator, addCustom, etc.
    """
    def registerUIDecorator(func):
        cls = type(nodeType + "_TransTemplate", (baseClass,), dict(setup=func))
        registerTranslatorUI(cls, nodeType, translatorName)
        # return function unchanged
        return func
    return registerUIDecorator

def createTranslatorMenu(nodeAttr, label=None, nodeType=None, default=None, optionMenuName=None):
    '''
    convenience function for creating a TranslatorControl and attaching it to a UI
    '''
    node, controlAttr = nodeAttr.split('.', 1)
    if nodeType is None:
        nodeType = cmds.nodeType(node)
    kwargs = {'controlAttr' : controlAttr}
    if label:
        kwargs['label'] = label
    if optionMenuName:
        kwargs['optionMenuName'] = optionMenuName
    if default:
        kwargs['default'] = default
    trans = TranslatorControlUI(nodeType, **kwargs)
    trans._doSetup(node)
    return trans

def shapeTemplate(nodeName):
    """
    override for the builtin maya shapeTemplate procedure
    """
    global _templates
    nodeType = cmds.objectType(nodeName)

    try:
        # has one been explicitly registered?
        template = _templates[nodeType]
    except KeyError:
        pass
    else:
        cmds.editorTemplate(beginLayout='Arnold', collapse=True)
        template._doSetup(nodeName)
        cmds.editorTemplate(endLayout=True)

    cmds.editorTemplate(beginLayout=mel.eval('uiRes("m_AEshapeTemplate.kObjectDisplay")'))

    # include/call base class/node attributes
    mel.eval('AEdagNodeCommon "%s"'%nodeName)
    cmds.editorTemplate(endLayout=True)

    # include/call base class/node attributes
    mel.eval('AEdagNodeInclude "%s"'%nodeName)


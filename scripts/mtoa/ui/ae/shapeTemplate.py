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
    Register the default translator for a node. the second argument identifies the name of the
    translator.  pass a string if the default is always the same,
    or a function which returns a string, if the default depends on context.

    The default will automatically be set whenever a node of the given type is added to the scene.
    """

    # to register a default translator, we need to create a TranslatorControl instance for this
    # node type.

    try:
        inst = _templates[nodeType]
        inst.setDefaultTranslator(stringOrFunc)
    except KeyError:
        inst = TranslatorControl(nodeType, default=stringOrFunc)
        _templates[nodeType] = inst

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
    return a dictionary of {translatorName : [template class]} for the given nodeType
    """
    # return a copy so it doesn't get messed with
    global _translatorTemplates
    return dict(_translatorTemplates[nodeType])

def getTranslatorTemplate(nodeType, translatorName):
    """
    return a setup template *instance* for the given nodeType, or None if one has not been registered
    """
    try:
        return getTranslatorTemplates(nodeType)[translatorName]
    except KeyError:
        pass

class BaseTemplate(object):
    """
    This class provides a framework for creating UIs that are compatible with the Attribute Editor.
    Once instantiated, the UI can be attached to an AE template via AttributeTemplate.attachToAE(),
    or simply built as a normal UI via AttributeTemplate.attachToUI()
    """
    def __init__(self, nodeType=None):
        self._nodeType = nodeType
        self._nodeName = None

    def __repr__(self):
        return '%s(%r)' % (self.__class__.__name__, self.nodeType())

    def _doBuild(self, node, *args):
        '''
        build the UI from the list of added attributes
        '''
        self._setActiveNode(node)
        self.build()

    def _doUpdate(self, node, *args):
        self._setActiveNode(node)
        self.update()

    def _setActiveNode(self, nodeName):
        "set the active node"
        assert nodeName, "%r: trying to set nodeName to None" % self
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

    # creation
    def attachToUI(self, nodeName, parent=None):
        "add the appropriate callbacks to the current UI"
        currParent = cmds.setParent(query=True)
        if parent is not None:
            cmds.setParent(parent)
        self._doBuild(nodeName)
        if currParent is not None and currParent != '' :
            cmds.setParent(currParent)

    def attachToAE(self, controlAttr='aiTranslator'):
        "add the appropriate callbacks to the editor template"
        cmds.editorTemplate(aeCallback(lambda attr: self._doBuild(attr.split('.')[0])),
                            aeCallback(lambda attr: self._doUpdate(attr.split('.')[0])),
                            controlAttr,
                            callCustom=True)

def controlBuilder(func):
    """
    decorator to make an AttributeTemplate method capable of building a UI control. the functionality
    that the wrapped function implements is delayed until the AttributeTemplate.build() is called
    """
    def wrapped(self, *args, **kwargs):
        print "running wrapped", func.__name__, self, args, kwargs
        self._builders.append((func, args, kwargs))
    wrapped.__name__ = func.__name__
    wrapped.__doc__ = func.__doc__
    return wrapped

class AttributeTemplate(BaseTemplate):
    """
    This class provides a framework for creating and managing AE-like templates with multiple UI elements.
    """
    def __init__(self, nodeType=None):
        super(AttributeTemplate, self).__init__(nodeType)
        self._controls = []
        self._builders = []
        self._layoutStack = []
        self.setup()

    def setup(self):
        """
        this method should be overridden. it is called when the class is initialized. it is kept as a
        separate method to avoid the user coming into conflict with variables managed by this class
        """
        pass

    def build(self):
        '''
        build the UI from the list of added attributes
        '''
        cmds.setUITemplate('attributeEditorTemplate', pushTemplate=True)
        self._layoutStack.append(cmds.setParent(query=True))
        for func, args, kwargs in self._builders:
            func(self, *args, **kwargs)
        cmds.setUITemplate(popTemplate=True)

    def update(self):
        cmds.setUITemplate('attributeEditorTemplate', pushTemplate=True)
        for attr, updateFunc, parent in self._controls:
            cmds.setParent(parent)
            updateFunc(self.nodeAttr(attr))
        cmds.setUITemplate(popTemplate=True)

    # building
    def _addControl(self, attr, updateFunc, parent):
        self._controls.append((attr, updateFunc, parent))

    @controlBuilder
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
        control = AttrControlGrp(**kwargs)
        self._addControl(attr, control.setAttribute, parent)

    @controlBuilder
    def addSeparator(self):
        cmds.separator()

    @controlBuilder
    def addCustom(self, attr, createFunc, updateFunc):
        parent = self._layoutStack[-1]
        createFunc(self.nodeAttr(attr))
        self._addControl(attr, updateFunc, parent)

    @controlBuilder
    def beginLayout(self, label, **kwargs):
        '''
        begin a frameLayout.
        accepts any keyword args valid for creating a frameLayout
        '''
        kwargs['label'] = label
        cmds.setParent(self._layoutStack[-1])
        cmds.frameLayout(**kwargs)
        self._layoutStack.append(cmds.columnLayout(adjustableColumn=True))

    @controlBuilder
    def endLayout(self):
        '''
        end the current frameLayout
        '''
        self._layoutStack.pop()
        cmds.setParent(self._layoutStack[-1])

    def attachToAE(self, controlAttr='aiTranslator'):
        "add the appropriate callbacks to the editor template"
        super(AttributeTemplate, self).attachToAE(controlAttr)
#        for attr in self.getAttributes():
#            cmds.editorTemplate(suppress=attr)

class AttributeEditorTemplate(pm.uitypes.AETemplate):
    def __init__(self, nodeName):
        super(AttributeEditorTemplate, self).__init__(nodeName)
        self.setup()

    def addCustom(self, attr, newFunc, replaceFunc):
        self.callCustom(newFunc, replaceFunc, attr)

#
#class AttributeEditorTemplate(AttributeTemplate):
#    def addSwatch(self):
#        self.addCustom("message", aiSwatchDisplay.aiSwatchDisplayNew, aiSwatchDisplay.aiSwatchDisplayReplace)
#
#    @controlBuilder
#    def beginScrollLayout(self):
#        cmds.setParent(self._layoutStack[-1])
#        self._layoutStack.append(cmds.scrollLayout())
#
#    @controlBuilder
#    def endScrollLayout(self):
#        '''
#        end the current scrollLayout
#        '''
#        self._layoutStack.pop()
#        cmds.setParent(self._layoutStack[-1])
#
#    @controlBuilder
#    def addExtraControls(self):
#        '''
#        end the current scrollLayout
#        '''
#        cmds.editorTemplate(addExtraControls=True)

class ArnoldTranslatorTemplate(AttributeEditorTemplate):
    pass

#class ArnoldTranslatorTemplate(AttributeTemplate):
#    """
#    To implement an AE template for a custom translator, create a subclass of this class and
#    register it using registerTranslatorUI
#    """
#    def __init__(self, nodeType):
#        super(ArnoldTranslatorTemplate, self).__init__(nodeType)
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

class TranslatorControl(BaseTemplate):
    '''
    Allows multiple AttributeTemplates, each representing an arnold translator, to be controlled via
    one optionMenu, such that only the active template is visible.  A default controller is automatically created for
    each node that has registered arnold translator UIs. Manually creating a TranslatorControl is only necessary if you
    need to customize the default controller behavior.
    '''
    def __init__(self, nodeType, label='Arnold Translator', controlAttr='aiTranslator', default=None, optionMenuName=None):
        super(TranslatorControl, self).__init__(nodeType)
        self._attr = controlAttr
        self._optionMenu = optionMenuName if optionMenuName is not None else controlAttr + "OMG"
        self._translators = None
        self._nodeName = None
        self._nodeType = nodeType
        self._label = label
        if not (default is None or isinstance(default, basestring) or callable(default)):
            cmds.warning("[mtoa] default translator must be a string or a function")
            return
        self._default = default

    #---- translator methods

    def _doSetDefaultTranslator(self, node):
        try:
            node.attr(self._attr).set(self.getDefaultTranslator(node))
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
        #ArnoldTranslatorTemplate.syncChannelBox(nodeName, nodeType, currentTranslator)
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


    def attachToUI(self, nodeName, parent=None):
        currParent = cmds.setParent(query=True)
        if parent is not None:
            cmds.setParent(parent)

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
                self.createMenu(nodeName)
                cmds.setParent(mainCol)
                for translator, template in translatorTemplates:

                    # we always create a layout, even if it's empty
                    cmds.frameLayout(label=translator, collapse=False)
                    cmds.columnLayout(
                                      adjustableColumn=True,
                                      #columnAttach=("both", 0)
                                      )
                    template.attachToUI(nodeName)
                    cmds.setParent(mainCol)
                # for compatibility with AE templates
                cmds.columnLayout()
                cmds.text(label='hide_me')
                cmds.setParent('..')
                self.updateMenu(nodeName)
            else:
                translator, template = translatorTemplates[0]
                template.attachToUI(nodeName)
            cmds.setParent(mainCol)
            cmds.setParent('..')

    def attachToAE(self):
        translatorTemplates = self.getTranslatorTemplates()
        if translatorTemplates:
            cmds.editorTemplate(beginLayout='hide', collapse=False)
            if len(translatorTemplates) > 1:
                # if there is more than one translator, we group each in its own layout
                # create the menu for selecting the translator
                cmds.editorTemplate(aeCallback(lambda attr: self.createMenu(attr.split('.')[0])),
                                    aeCallback(lambda attr: self.updateMenu(attr.split('.')[0])),
                                    self._attr, callCustom=True)
                for translator, template in translatorTemplates:
                    # we always create a layout, even if it's empty
                    cmds.editorTemplate(beginLayout=translator, collapse=False)
                    template.attachToAE(self._attr)
                    cmds.editorTemplate(endLayout=True)
                # timing on AE's is difficult: the frameLayouts are not created at this point even though
                # the `editorTemplate -beginLayout` calls have been made. this is a little hack
                # to ensure we get a callback after the AE ui elements have been built: normal controls can get
                # an update callback, but we don't have any normal controls around, so we'll have to make one and
                # hide it
                cmds.editorTemplate(self._attr,
                                    aeCallback(self.updateChildrenCallback),
                                    addDynamicControl=True, label='hide_me')
            else:
                translator, template = translatorTemplates[0]
                template.attachToAE(translator)
            cmds.editorTemplate(endLayout=True)

#class DriverTranslatorControl(TranslatorControl):
#    def __init__(self, controlAttr='imageType', optionMenuName=None):
#        TranslatorControl.__init__(self, controlAttr, optionMenuName)


def registerAETemplate(templateClass, nodeType):
    assert inspect.isclass(templateClass) and issubclass(templateClass, ArnoldTranslatorTemplate), \
        "you must pass a subclass of ArnoldTranslatorTemplate"
    inst = templateClass(nodeType)
    print "registering attribute template for %s" % nodeType
    global _templates
    _templates[nodeType] = inst

def aeTemplate(nodeType, baseClass=ArnoldTranslatorTemplate):
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
    a translator UI is a specialized attribute template based on the
    ArnoldTranslatorTemplate class.
    """
    global _translatorTemplates
    assert inspect.isclass(templateClass) and issubclass(templateClass, ArnoldTranslatorTemplate),\
        "you must pass a subclass of ArnoldTranslatorTemplate"
    print "registering translator template for %s on %s" % (translatorName, nodeType)

    template = templateClass(nodeType)
    _translatorTemplates[nodeType][translatorName] = template

    if nodeType not in _templates:
        _templates[nodeType] = TranslatorControl(nodeType)

def translatorUI(nodeType, translatorName=None, baseClass=ArnoldTranslatorTemplate):
    """
    decorator for registering a function for creating a simple translator UI. The function receives an ArnoldTranslatorTemplate
    instance, which it uses to make calls to addControl, addSeparator, addCustom, etc. 
    """
    def registerUIDecorator(func):
        transName = translatorName
        if transName is None:
            module = inspect.getmodule(func)
            if hasattr(module, '__name__'):
                transName = module.__name__.split('.')[0]
            else:
                transName = '<built-in>'
        cls = type(nodeType + "Template", (baseClass,), dict(setup=func))
        registerTranslatorUI(cls, nodeType, transName)
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
    trans = TranslatorControl(nodeType, **kwargs)
    trans.attachToUI(node)
    return trans

def shapeTemplate(nodeName):
    """
    override for the builtin maya shapeTemplate procedure
    """
    global _templates
    nodeType = cmds.objectType(nodeName)

    template = None
    try:
        # has one been explicitly registered?
        template = _templates[nodeType]
    except KeyError:
        if getTranslators(nodeType):
            # if not and this node has translators, create a default TranslatorControl
            template = TranslatorControl(nodeType)
    if template:
        cmds.editorTemplate(beginLayout='Arnold', collapse=True)
        template.attachToAE()
        cmds.editorTemplate(endLayout=True)

    cmds.editorTemplate(beginLayout=mel.eval('uiRes("m_AEshapeTemplate.kObjectDisplay")'))

    # include/call base class/node attributes
    mel.eval('AEdagNodeCommon "%s"'%nodeName)
    cmds.editorTemplate(endLayout=True)

    # include/call base class/node attributes
    mel.eval('AEdagNodeInclude "%s"'%nodeName)


import maya.cmds as cmds
import maya.mel as mel
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

global _translatorControllers
_translatorControllers = {}

#-------------------------------------------------
# translator defaults
#-------------------------------------------------

def registerDefaultTranslator(nodeType, stringOrFunc):
    """
    register the default translator for a node. pass a string if the default is static,
    or a function which returns a string, if the default depends on context.

    the default will automatically be set whenever a node of the given type is added to the scene.
    """
    # to register a default translator, we need to create a TranslatorControl instance for this
    # node type.
    if isinstance(stringOrFunc, basestring):
        classdict = {}
        kwargs = dict(default=stringOrFunc)
    elif callable(stringOrFunc):
        classdict = {'getDefaultTranslator' : staticmethod(stringOrFunc)}
        kwargs = {}
    else:
        cmds.warning("[mtoa] you must pass a string or a function to registerDefaultTranslator")
        return

    cls = type('%s_TransControl', (TranslatorControl,), classdict)
    inst = cls(nodeType, **kwargs)
    _translatorControllers[nodeType] = inst

    # set defaults for existing nodes
    for node in cmds.ls(exactType=nodeType):
        # this will set aiTranslator if it is not set
        # TOOD: replace this with a call to TranslatorControl.getCurrentTranslator
        inst.getCurrentTranslator(node)

    callbacks.addNodeAddedCallback(inst.setDefaultTranslator, nodeType)


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
        cls = getTranslatorTemplates(nodeType)[translatorName]
        template = cls(nodeType)
        template.setup()
        return template
    except KeyError:
        pass

def registerTranslatorUI(cls, nodeType, translatorName='<built-in>'):
    """
    a translator UI is a specialize custom attr template based on the
    ArnoldTranslatorTemplate class.
    """
    global _customAttrTemplates
    assert inspect.isclass(cls) and issubclass(cls, ArnoldTranslatorTemplate), "you must pass a subclass of ArnoldTranslatorTemplate"
    print "registering translator template for %s on %s" % (translatorName, nodeType)
    _translatorTemplates[nodeType][translatorName] = cls

class BaseTemplate(object):
    """
    This class provides a framework for managing AE-like templates. Once instantiated,
    the UI can be registered as an AE tempalte via AttributeTemplate.attachToAE(),
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
    def nodeName(self):
        "get the active node"
        # assert self._nodeName, "%r: nodeName should be set by now" % self
        return self._nodeName

    def nodeType(self):
        if self._nodeType is None:
            self._nodeType = cmds.objectType(self.nodeName())
        return self._nodeType

    def nodeAttr(self, attr):
        return self.nodeName() + '.' + attr

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

class AttributeTemplate(BaseTemplate):
    """
    This class provides a framework for managing AE-like templates. Once instantiated,
    the UI can be registered as an AE template via AttributeTemplate.attachToAE(),
    or simply built as a normal UI via AttributeTemplate.attachToUI()
    """
    SEPARATOR = '-'
    ATTRIBUTE = 'attr'
    CUSTOM = 'custom'
    BEGIN_LAYOUT = 'beginLayout'
    END_LAYOUT = 'endLayout'
    def __init__(self, nodeType=None):
        super(AttributeTemplate, self).__init__(nodeType)
        self._controls = []
        self._attributes = []
        self._layoutStack = []

    def build(self):
        '''
        build the UI from the list of added attributes
        '''
        self._layoutStack.append(cmds.setParent(query=True))
        for mode, data in self._attributes:
            if mode == self.SEPARATOR:
                cmds.separator()
            elif mode == self.ATTRIBUTE:
                attr, label, annotation = data
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
                control = AttrControlGrp(**kwargs)
                self._controls.append((attr, control.setAttribute))
            elif mode == self.CUSTOM:
                attr, createFunc, updateFunc = data
                createFunc(self.nodeAttr(attr))
                self._controls.append((attr, updateFunc))
            elif mode == self.BEGIN_LAYOUT:
                cmds.setParent(self._layoutStack[-1])
                cmds.frameLayout(**data)
                self._layoutStack.append(cmds.columnLayout())
            elif mode == self.END_LAYOUT:
                self._layoutStack.pop()
                cmds.setParent(self._layoutStack[-1])

    def update(self):
        for attr, updateFunc in self._controls:
            updateFunc(self.nodeAttr(attr))

    # building
    def addAttribute(self, attr, label=None, annotation=None):
        self._attributes.append((self.ATTRIBUTE,
                                 (attr, label, annotation)))

    def addSeparator(self):
        self._attributes.append((self.SEPARATOR,
                                 None))

    def addCustom(self, attr, createFunc, updateFunc):
        self._attributes.append((self.CUSTOM,
                                 (attr, createFunc, updateFunc)))

    def beginLayout(self, **kwargs):
        '''
        begin a frameLayout.
        accepts any keyword args valid for creating a frameLayout
        '''
        self._attributes.append((self.BEGIN_LAYOUT, kwargs))

    def endLayout(self,):
        '''
        end the current frameLayout
        '''
        self._attributes.append((self.END_LAYOUT, None))

    def getAttributes(self):
        "return attributes controlled by this template"
        return [x[0] for x in self._attributes if x[0] != self.SEPARATOR]

    def attachToAE(self, controlAttr='aiTranslator'):
        "add the appropriate callbacks to the editor template"
        super(AttributeTemplate, self).attachToAE(controlAttr)
        for attr in self.getAttributes():
            cmds.editorTemplate(suppress=attr)

class ArnoldTranslatorTemplate(AttributeTemplate):
    """
    To implement an AE template for a custom translator, create a subclass of this class and
    register it using registerTranslatorUI
    """
    def __init__(self, nodeType):
        super(ArnoldTranslatorTemplate, self).__init__(nodeType)
        self._keyableDefaults = {}

    def showInChannelBox(self, enabled):
        for attr in self.getAttributes():
            type = self.nodeAttrType(attr)
            keyable = enabled and cmds.attributeQuery(attr, node=self.nodeName(), keyable=True)
            if cmds.attributeQuery(attr, node=self.nodeName(), numberOfChildren=True):
                children = cmds.attributeQuery(attr, node=self.nodeName(), listChildren=True)
                for c in children:
                    # some sort of a bug forces a call like this in order to set keyable and channelbox correctly...
                    cmds.setAttr(self.nodeAttr(c), channelBox=enabled, keyable=keyable)
                    cmds.setAttr(self.nodeAttr(c), keyable=keyable)
            else:
                # some sort of a bug forces a call like this in order to set keyable and channelbox correctly...
                cmds.setAttr(self.nodeAttr(attr), channelBox=enabled, keyable=keyable)
                cmds.setAttr(self.nodeAttr(attr), keyable=keyable)

    @staticmethod
    def syncChannelBox(nodeName, nodeType, default):
        """
        make only the attributes for the active translator visible in the channel box
        """
        templates = getTranslatorTemplates(nodeType)
        # Do the actual syncing with the ChannelBox
        for name, template in templates.items():
            if name == default:
                continue
            template._setActiveNode(nodeName)
            template.showInChannelBox(False)
        # We need to run this last for cases where templates share attributes
        if default in templates:
            templates[default]._setActiveNode(nodeName)
            templates[default].showInChannelBox(True)

class AutoTranslatorTemplate(ArnoldTranslatorTemplate):
    _arnoldNodeType = None

    def setup(self):
        """
        default setup automatically builds a UI based on metadata
        """
        for paramName, attrName, label, annotation in core.getAttributeData(self._arnoldNodeType):
            self.addAttribute(attrName,
                              label if label else prettify(paramName),
                              annotation)

def registerUI(nodeType, translatorName=None, baseClass=ArnoldTranslatorTemplate):
    "decorator for easily registering a simple UI function"
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

class TranslatorControl(BaseTemplate):
    '''
    Allows multiple AttributeTemplates, each representing an arnold translator, to be controlled via
    one optionMenu, such that only the active template is visible.
    '''
    def __init__(self, nodeType, label='Arnold Translator', controlAttr='aiTranslator', default=None, optionMenuName=None):
        super(TranslatorControl, self).__init__(nodeType)
        self._attr = controlAttr
        self._optionMenu = optionMenuName if optionMenuName is not None else controlAttr + "OMG"
        self._translators = None
        self._nodeName = None
        self._nodeType = nodeType
        self._label = label
        self._default = default

    #---- translator methods

    def setDefaultTranslator(self, nodeName):
        try:
            cmds.setAttr(nodeName + "." + self._attr, self.getDefaultTranslator(nodeName), type='string')
        except RuntimeError:
            cmds.warning("failed to set default translator for %s" % nodeName)

    def getDefaultTranslator(self, nodeName):
        return self._default

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
            cmds.setAttr(nodeName + "." + self._attr, transName, type='string')
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
            self._translators = [x[0] for x in core.listTranslators(self.nodeType())]
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

    def attachToAE(self, layoutName='Arnold', collapse=True):
        translatorTemplates = self.getTranslatorTemplates()
        if translatorTemplates:
            cmds.editorTemplate(beginLayout=layoutName, collapse=collapse)
            if len(translatorTemplates) > 1:
                # if there is more than one translator, we group each in its own layout
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

def createTranslatorUI(nodeAttr, label=None, nodeType=None, default=None, optionMenuName=None):
    '''
    convenience function for creating a TranslatorControl
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
    nodeType = cmds.objectType(nodeName)
    try:
        transCtrl = _translatorControllers[nodeType]
    except KeyError:
        transCtrl = TranslatorControl(nodeType)
    transCtrl.attachToAE()

    cmds.editorTemplate(beginLayout=mel.eval('uiRes("m_AEshapeTemplate.kObjectDisplay")'))

    # include/call base class/node attributes
    mel.eval('AEdagNodeCommon "%s"'%nodeName)
    cmds.editorTemplate(endLayout=True)

    # include/call base class/node attributes
    mel.eval('AEdagNodeInclude "%s"'%nodeName)


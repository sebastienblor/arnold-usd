import maya.cmds as cmds
import maya.mel as mel
from maya.utils import executeDeferred
from mtoa.ui.ae.utils import aeCallback, AttrControlGrp
import mtoa.callbacks as callbacks

from collections import defaultdict
import inspect

global _customAttrTemplates
_customAttrTemplates = {}

global _translatorTemplates
_translatorTemplates = defaultdict(dict)

global _translatorDefaults
_translatorDefaults = {}

#-------------------------------------------------
# standard templates
#-------------------------------------------------

def registerCustomAttrTemplate(nodeType, func):
    global _customAttrTemplates
    assert callable(func), "you must pass a callable object"
    print "[mtoa] registering custom attr template for %s" % nodeType
    _customAttrTemplates[nodeType] = func

def getCustomAttrTemplate(nodeType):
    """
    return a function for building an arnold AE template for this nodeType
    or None if one has not been registered
    """ 
    global _customAttrTemplates
    return _customAttrTemplates.get(nodeType, None)

def registerUI(nodeType):
    "decorator for easily registering a UI function"
    def registerUIDecorator(func):
        registerCustomAttrTemplate(nodeType, func)
        return func
    return registerUIDecorator

#-------------------------------------------------
# translator defaults
#-------------------------------------------------

def registerDefaultTranslator(nodeType, stringOrFunc):
    """
    register the default translator for a node. pass a string if the default is static,
    or a function which returns a string, if the default depends on context.

    the default will automatically be set whenever a node of the given type is added to the scene.
    """
    if isinstance(stringOrFunc, basestring):
        # for the sake of consistency, make it a function
        getFunc = lambda nodeType: stringOrFunc
        def setFunc(nodeName):
            try:
                cmds.setAttr(nodeName + ".aiTranslator", stringOrFunc, type='string')
            except RuntimeError:
                cmds.warning("failed to set default translator for %s" % nodeName)
    elif callable(stringOrFunc):
        getFunc = stringOrFunc
        def setFunc(nodeName):
            try:
                cmds.setAttr(nodeName + ".aiTranslator", getFunc(nodeName), type='string')
            except RuntimeError:
                cmds.warning("failed to set default translator for %s" % nodeName)
    else:
        cmds.warning("[mtoa] you must pass a string or a function to registerDefaultTranslator")
        return
    _translatorDefaults[nodeType] = getFunc
 
    # set defaults for existing nodes
    for node in cmds.ls(type=nodeType):
        # this will set aiTranslator if it is not set
        getCurrentTranslator(node)

    callbacks.addNodeAddedCallback(setFunc, nodeType)

def getDefaultTranslator(nodeName, default=None):
    """
    return the default translator for the given node instance
    """
    nodeType = cmds.objectType(nodeName)
    try:
        func = _translatorDefaults[nodeType]
    except KeyError:
        return default
    else:
        res = func(nodeName)
        assert isinstance(res, basestring), "default translator callback must return a string"
        return res

def getCurrentTranslator(nodeName):
    """
    get the current translator for this node, querying and setting the default if not yet set
    """
    transName = cmds.getAttr(nodeName + ".aiTranslator")
    if not transName:
        # set default
        transName = getDefaultTranslator(nodeName)
        if transName is None:
            translators = cmds.arnoldPlugins(listTranslators=nodeName)
            if not translators:
                cmds.warning("cannot find default translator for %s" % nodeName)
                return
            transName = translators[0]
        cmds.setAttr(nodeName + ".aiTranslator", transName, type='string')
    return transName

#-------------------------------------------------
# translator templates
#-------------------------------------------------

def getTranslatorTemplates(nodeType):
    """
    return a dictionary of {translatorName : [templateInstance]} for the given nodeType
    """
    # return a copy so it doesn't get messed with
    global _translatorTemplates
    return dict(_translatorTemplates[nodeType])

def registerTranslatorUI(nodeType, translatorName, cls):
    """
    a translator UI is a specialize custom attr template based on the
    ArnoldTranslatorTemplate class. 
    """
    global _customAttrTemplates
    assert inspect.isclass(cls) and issubclass(cls, ArnoldTranslatorTemplate), "you must pass a subclass of ArnoldTranslatorTemplate"
    print "registering translator template for %s on %s" % (translatorName, nodeType)
    _translatorTemplates[nodeType][translatorName] = cls()

class CustomAETemplate(object):
    """
    This class provides a framework for managing custom AE templates registered
    via cmds.editorTemplate(callCustom=True) 
    """
    SEPARATOR = '-'
    def __init__(self):
        self._nodeName = None
        self._controls = []
        self._attributes = []

    def _doBuild(self, attr, *args):
        self._setActiveNode(attr.split('.')[0])
        self.build()

    def _doUpdate(self, attr, *args):
        self._setActiveNode(attr.split('.')[0])
        self.update()

    def _setActiveNode(self, nodeName):
        "set the active node"
        self._nodeName = nodeName

    def activeNode(self):
        "get the active node"
        return self._nodeName

    def nodeType(self):
        return cmds.objectType(self._nodeName)
        
    def nodeAttr(self, attr):
        return self.activeNode() + '.' + attr

    def nodeAttrExists(self, attr):
        return cmds.addAttr(self.nodeAttr(attr), q=True, ex=True)

    def nodeAttrType(self, attr):
        type = cmds.getAttr(self.nodeAttr(attr), type=True)
        if type == 'float3':
            if cmds.addAttr(self.nodeAttr(attr), q=True, usedAsColor=True):
                type = 'color'
        return type

    def addAttribute(self, attr):
        self._attributes.append(attr)
        
    def addSeparator(self):
        self._attributes.append(self.SEPARATOR)

    def getAttributes(self):
        "return attributes controlled by this template"
        return [x for x in self._attributes if x != self.SEPARATOR]

    def update(self):
        for control in self._controls:
            control.updateAttribute()

    def build(self):
        for attr in self._attributes:
            if attr == self.SEPARATOR:
                cmds.separator()
            else:
                control = AttrControlGrp(type=self.nodeAttrType(attr), label=attr, attribute=self.nodeAttr(attr))
                self._controls.append(control)

class ArnoldTranslatorTemplate(CustomAETemplate):
    """
    To implement an AE template for a custom translator, create a subclass of this class and
    register it using registerTranslatorUI
    """
    def __init__(self):
        CustomAETemplate.__init__(self)
        self._keyableDefaults = {}

    def showInChannelBox(self, enabled):
        for attr in self.getAttributes():
            type = self.nodeAttrType(attr)
            keyable = enabled and cmds.attributeQuery(attr, node=self.activeNode(), keyable=True)
            if cmds.attributeQuery(attr, node=self.activeNode(), numberOfChildren=True):
                children = cmds.attributeQuery(attr, node=self.activeNode(), listChildren=True)
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

    @classmethod
    def register(cls, nodeType, translatorName):
        registerTranslatorUI(nodeType, translatorName, cls)

def updateTranslatorUICallback(attr):
    """
    this function is assigned to an AE refresh callback. this allows us to call updateTranslatorUI
    to set things up.
    """
    nodeName = attr.split('.')[0]
    updateTranslatorUI(attr, cmds.getAttr(nodeName + '.aiTranslator'))

def updateTranslatorUI(attr, currentTranslator):
    """
    update the translator UI, which consists of an optionMenuGrp and a frameLayout per translator,
    so that only the frameLayout corresponding to the currently selected translator is visible
    """
    if not cmds.layout('aiTranslatorOMG', exists=True):
        # not built yet
        return
    nodeName = attr.split('.')[0]
    nodeType = cmds.objectType(nodeName)
    fullpath = cmds.layout('aiTranslatorOMG', query=True, fullPathName=True)
    # get the grand-parent
    gparent = fullpath.rsplit('|', 2)[0]
    children = cmds.layout(gparent, query=True, childArray=True)
    # hide all frameLayouts but ours
    assert currentTranslator, "we should have a translator set by now"

    for child in children[:-1]:
        # is it a frame layout?
        objType = cmds.objectTypeUI(child)
        if objType == 'frameLayout':
            label = cmds.frameLayout(child, query=True, label=True)
            cmds.frameLayout(child, edit=True, collapsable=False, labelVisible=False, 
                             visible=(label == currentTranslator))
    # FIXME: this needs a check for read-only nodes from referenced files. also, not sure 
    # changing attribute properties is the best approach
    #ArnoldTranslatorTemplate.syncChannelBox(nodeName, nodeType, currentTranslator)
    # last child is the 'hide_me' control that always needs to be hidden
    cmds.layout(children[-1], edit=True, visible=False)

def translatorListCB(attr, currentTranslator):
    """
    called with the translator optionMenuGrp (aiTranslatorOMG) changes
    """
    cmds.setAttr(attr, currentTranslator, type='string')
    updateTranslatorUI(attr, currentTranslator)

def translatorListNew(attr):
    """
    called to create an optionMenuGrp for choosing between multiple translator options for a given node
    """
    nodeName = attr.split('.')[0]
    cmds.optionMenuGrp('aiTranslatorOMG', label='Arnold Translator', cc=lambda *args: translatorListCB(attr, args[0]))
    # create menu items
    translators = cmds.arnoldPlugins(listTranslators=nodeName)
    for tran in translators:
        cmds.menuItem(label=tran)
    cmds.setParent(menu=True)

    transName = getCurrentTranslator(nodeName)
    cmds.optionMenuGrp('aiTranslatorOMG', edit=True, value=transName)

def translatorListReplace(attr):
    """
    called to update an optionMenuGrp for choosing between multiple translator options for a given node
    """
    nodeName = attr.split('.')[0]
    # delete current options
    translators = cmds.optionMenuGrp('aiTranslatorOMG', q=True, itemListLong=True)
    for tran in translators:
        cmds.deleteUI(tran, menuItem=True)

    # populate with a fresh list
    parent = cmds.setParent('aiTranslatorOMG')
    translators = cmds.arnoldPlugins(listTranslators=nodeName)
    for tran in translators:
        cmds.menuItem(label=tran, parent=parent + '|OptionMenu')

    transName = getCurrentTranslator(nodeName)
    cmds.optionMenuGrp('aiTranslatorOMG', edit=True, value=transName, cc=lambda *args: translatorListCB(attr, args[0]))
    updateTranslatorUI(attr, transName)

def _makeBuildCallback(nodeType, translatorName):
    "workaround to create a dynamic python callback with a mel wrapper"
    def doBuildCB(attr):
        getTranslatorTemplates(nodeType)[translatorName]._doBuild(attr)
    name = '%s_%s_doBuildCB' % (nodeType, translatorName)
    # can't be unicode
    doBuildCB.__name__ = str(name)
    # must be in the globals for for aeCallback to work
    globals()[name] = doBuildCB
    return aeCallback(doBuildCB)

def _makeUpdateCallback(nodeType, translatorName):
    "workaround to create a dynamic python callback with a mel wrapper"
    def doUpdateCB(attr):
        getTranslatorTemplates(nodeType)[translatorName]._doUpdate(attr)
    name = '%s_%s_doUpdateCB' % (nodeType, translatorName)
    # can't be unicode
    doUpdateCB.__name__ = str(name)
    # must be in the globals for for aeCallback to work
    globals()[name] = doUpdateCB
    return aeCallback(doUpdateCB)

def shapeTemplate(nodeName):
    """
    override for the builtin maya shapeTemplate procedure
    """
    nodeType = cmds.objectType(nodeName)
    basicTemplate = getCustomAttrTemplate(nodeType)
    translatorTemplates = getTranslatorTemplates(nodeType)
    allTranslators = cmds.arnoldPlugins(listTranslators=nodeName)
    if basicTemplate or translatorTemplates or len(allTranslators) > 1:
        cmds.editorTemplate(beginLayout="Arnold")
        if len(allTranslators) > 1:
            # if there is more than one translator, we group each in its own layout
            cmds.editorTemplate(aeCallback(translatorListNew), aeCallback(translatorListReplace), "aiTranslator", callCustom=True)
            for translator in allTranslators:
                # we always create a layout, even if it's empty
                cmds.editorTemplate(beginLayout=translator, collapse=False)
                if translator in translatorTemplates:
                    builder = translatorTemplates[translator]
                    cmds.editorTemplate(_makeBuildCallback(nodeType, translator), 
                                        _makeUpdateCallback(nodeType, translator),
                                        "aiTranslator",
                                        callCustom=True)
                    for attr in builder.getAttributes():
                        cmds.editorTemplate(suppress=attr)
                cmds.editorTemplate(endLayout=True)
            # timing on AE's is difficult: the frameLayouts are not created at this point even though
            # the `editorTemplate -beginLayout` calls have been made. this is a little hack
            # to ensure we get a callback after the AE ui elements have been built: normal controls can get
            # an update callback, but we don't have any normal controls around, so we'll have to make one and
            # hide it
            cmds.editorTemplate("aiTranslator", aeCallback(updateTranslatorUICallback), addDynamicControl=True, label='hide_me')
        else:
            translator = allTranslators[0]
            if basicTemplate:
                # simple function
                basicTemplate(nodeName)
            elif translator in translatorTemplates:
                translatorTemplates[translator](nodeName)
        cmds.editorTemplate(endLayout=True)

    cmds.editorTemplate(beginLayout=mel.eval('uiRes("m_AEshapeTemplate.kObjectDisplay")'))

    cmds.editorTemplate(beginLayout=mel.eval('uiRes("m_AEshapeTemplate.kObjectDisplay")'))

    # include/call base class/node attributes
    mel.eval('AEdagNodeCommon "%s"'%nodeName)
    cmds.editorTemplate(endLayout=True)

    # include/call base class/node attributes
    mel.eval('AEdagNodeInclude "%s"'%nodeName)


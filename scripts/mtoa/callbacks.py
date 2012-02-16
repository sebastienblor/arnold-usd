"""
a module for managing mtoa's callbacks
"""
 
import maya.cmds as cmds
import pymel.core as pm
import maya.OpenMaya as om
from collections import defaultdict

global _callbackIds
_callbackIds = om.MCallbackIdArray()

global _nodeAddedCallbacks
_nodeAddedCallbacks = defaultdict(list)

global _attrChangedCallbacks
_attrChangedCallbacks = {}

CONTEXTS = [om.MNodeMessage.kConnectionMade,
            om.MNodeMessage.kConnectionBroken,
            om.MNodeMessage.kAttributeEval,
            om.MNodeMessage.kAttributeSet,
            om.MNodeMessage.kAttributeLocked,
            om.MNodeMessage.kAttributeUnlocked,
            om.MNodeMessage.kAttributeAdded,
            om.MNodeMessage.kAttributeRemoved,
            om.MNodeMessage.kAttributeRenamed,
            om.MNodeMessage.kAttributeKeyable,
            om.MNodeMessage.kAttributeUnkeyable,
            om.MNodeMessage.kIncomingDirection,
            om.MNodeMessage.kAttributeArrayAdded,
            om.MNodeMessage.kAttributeArrayRemoved,
            om.MNodeMessage.kOtherPlugSet]

ANY_CHANGE = 0
for _msg in CONTEXTS:
    ANY_CHANGE |= _msg


def _removeCallbacks(*args):
    if args[0][0] != 'mtoa':
        return
    global _callbackIds
    om.MDGMessage.removeCallbacks(_callbackIds)

def manageCallback(callbackId):
    "track a callback id so that it can be automatically removed when mtoa is unloaded"
    global _callbackIds
    _callbackIds.append(callbackId)

def _makeNodeAddedCB(nodeType):
    def nodeAddedCB(obj, *args):
        node = pm.nt.DependNode(obj)
        # nodeAdded callback includes sub-types, but we want exact type only
        if node.type() != nodeType:
            return
        global _nodeAddedCallbacks
        for func, apiArgs in _nodeAddedCallbacks[nodeType]:
            if apiArgs:
                func(obj)
            else:
                func(node)
    # no unicode allowed
    nodeAddedCB.__name__ = "nodeAddedCB_" + str(nodeType) 
    return nodeAddedCB

def addNodeAddedCallback(func, nodeType, applyToExisting=True, apiArgs=False):
    """
    creates and manages a node added callback

    Parameters
    ----------
    func : callback function
        should take a single argument. the type of the argument is controlled by the apiArgs flag
    nodeType : string
        type of node to install callbacks for
    applyToExisting : boolean
        whether to apply the function to existing nodes
    apiArgs : boolean
        if True, api objects (MObjects, MPlugs, etc) are left as is. If False, they're converted to string names
    """
    if nodeType not in _nodeAddedCallbacks:
        manageCallback(om.MDGMessage.addNodeAddedCallback(_makeNodeAddedCB(nodeType), nodeType))
    _nodeAddedCallbacks[nodeType].append((func, apiArgs))
    
    if applyToExisting and apiArgs:
        _updateExistingNodes(nodeType, func)

def _getHandle(obj):
    handle = om.MObjectHandle(obj)
    handle.__hash__ = handle.hashCode
    return handle

def _makeInstallAttributeChangedCallback(nodeType):
    """
    make a function to be used with a nodeAdded callback which
    installs attributeChanged callbacks for the passed attribute
    """
    def installAttrChangeCallback(obj):
        fnNode = om.MFnDependencyNode(obj)
        # nodeAdded callback includes sub-types, but we want exact type only
        if fnNode.typeName() != nodeType:
            return

        # scriptJob does not receive an arg, but we want ours to
        def attrChanged(msg, plug, otherPlug, *args):
            global _attrChangedCallbacks
            try:
                funcMap = _attrChangedCallbacks[nodeType]
            except KeyError:
                pass
            else:
                plugName = plug.partialName(False, False, True, False, True, True)
                try:
                    func, context = funcMap[plugName]
                except KeyError:
                    pass
                else:
                    if context & msg:
                        func(plug, otherPlug, *args)
                    else:
                        print "skipping", plugName, func, "based on context", msg, context
#        _attrChangedCallbacks[_getHandle(node)]
        om.MNodeMessage.addAttributeChangedCallback(obj, attrChanged)
    return installAttrChangeCallback

def _updateExistingNodes(nodeType, func):
    fnNode = om.MFnDependencyNode()
    nodeIt = om.MItDependencyNodes()
    while 1:
        if nodeIt.isDone():
            break
        node = nodeIt.item()
        if node.isNull():
            continue
        fnNode.setObject(node)
        if fnNode.typeName() == nodeType:
            func(node)
        nodeIt.next()

def addAttributeChangedCallback(func, nodeType, attribute, context=ANY_CHANGE, applyToExisting=True):
    """
    add an attribute changed callback for all current and future nodes of the given type

    Parameters
    ----------
    func : function
        should take a single string arg for the node of the attribute that changed
    nodeType : string
        type of node to install attribute changed callbacks for 
    attribute : string
        name of attribute without leading period ('.')
    context : int mask
        an AttributeMessage enum from maya.OpenMaya.MNodeMessage describing what type of attribute
        change triggers the callback. defaults to any
    applyToExisting : boolean
        whether to apply the function to existing nodes
    """
    global _attrChangedCallbacks
    nodeAddedCallback = _makeInstallAttributeChangedCallback(nodeType)
    if nodeType not in _attrChangedCallbacks:
        # add a callback which creates the scriptJob that calls our function
        addNodeAddedCallback(nodeAddedCallback, nodeType, applyToExisting=False, apiArgs=True)
        _attrChangedCallbacks[nodeType] = {}
    _attrChangedCallbacks[nodeType][attribute] = (func, context)

    # setup callback for existing nodes
    if applyToExisting and not om.MFileIO.isOpeningFile():
        _updateExistingNodes(nodeType, nodeAddedCallback)

def addAttributeChangedCallbacks(nodeType, attrFuncs, context=ANY_CHANGE):
    """
    add multiple attribute changed callbacks for all current and future nodes of the given type.
    
    this is more efficient at installing multiple attribute changed callbacks than repeatedly
    calling `addAttributeChangedCallback`
    
    Parameters
    ----------
    nodeType : string
        type of node to install attribute changed callbacks for 
    attrFuncs : list of (attribute name, functions) pairs
        function should take a single string arg for the node of the attribute that changed
        attributes should be names of attribute without leading period ('.')
    context : int mask
        an AttributeMessage enum from maya.OpenMaya.MNodeMessage describing what type of attribute
        change triggers the callback. defaults to any
    """
    global _attrChangedCallbacks
    nodeAddedCallback = _makeInstallAttributeChangedCallback(nodeType)
    if nodeType not in _attrChangedCallbacks:
        # add a callback which creates the scriptJob that calls our function
        addNodeAddedCallback(nodeAddedCallback, nodeType, apiArgs=True)
        _attrChangedCallbacks[nodeType] = {}

    for attr, func in attrFuncs:
        # TODO: support more than one callback per nodeType/attribute
        _attrChangedCallbacks[nodeType][attr] = (func, context)

    # setup callback for existing nodes
    if not om.MFileIO.isOpeningFile():
        _updateExistingNodes(nodeType, nodeAddedCallback)

def removeAttributeChangedCallback(nodeType, attribute):
    _attrChangedCallbacks[nodeType].pop(attribute)

manageCallback(om.MSceneMessage.addStringArrayCallback(om.MSceneMessage.kAfterPluginUnload, _removeCallbacks, None))


class CallbackError(Exception): pass

class Callback(object):
    """
    Enables deferred function evaluation with 'baked' arguments.
    Useful where lambdas won't work...

    It also ensures that the entire callback will be be represented by one
    undo entry.

    Example:

    .. python::

        import pymel as pm
        def addRigger(rigger, **kwargs):
            print "adding rigger", rigger

        for rigger in riggers:
            pm.menuItem(
                label = "Add " + str(rigger),
                c = Callback(addRigger,rigger,p=1))   # will run: addRigger(rigger,p=1)
    """

    def __init__(self,func,*args,**kwargs):
        self.func = func
        self.args = args
        self.kwargs = kwargs

    def __call__(self,*args):
        cmds.undoInfo(openChunk=1)
        try:
            try:
                return self.func(*self.args, **self.kwargs)
            except Exception, e:
                raise CallbackError('Error during callback: %s' % e)
        finally:
            cmds.undoInfo(closeChunk=1)

class CallbackWithArgs(Callback):
    def __call__(self,*args,**kwargs):
        # not sure when kwargs would get passed to __call__,
        # but best not to remove support now
        kwargsFinal = self.kwargs.copy()
        kwargsFinal.update(kwargs)
        cmds.undoInfo(openChunk=1)
        try:
            return self.func(*self.args + args, **kwargsFinal)
        finally:
            cmds.undoInfo(closeChunk=1)


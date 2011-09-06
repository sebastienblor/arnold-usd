"""
a module for managing mtoa's callbacks
"""
 
import maya.cmds as cmds
import maya.OpenMaya as om
from collections import defaultdict

global _callbackIds
_callbackIds = om.MCallbackIdArray()

global _nodeAddedCallbacks
_nodeAddedCallbacks = defaultdict(list)

global _attrChangedCallbacks
_attrChangedCallbacks = {}

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
        fnNode = om.MFnDependencyNode(obj)
        # nodeAdded callback includes sub-types, but we want exact type only
        if fnNode.typeName() != nodeType:
            return
        name = fnNode.name()
        global _nodeAddedCallbacks
        for func, apiArgs in _nodeAddedCallbacks[nodeType]:
            if apiArgs:
                func(obj)
            else:
                func(name)
    # no unicode allowed
    nodeAddedCB.__name__ = "nodeAddedCB_" + str(nodeType) 
    return nodeAddedCB

def addNodeAddedCallback(func, nodeType, apiArgs=False):
    """
    creates and manages a node added callback

    Parameters
    ----------
    func : function
        should take a single string arg for the node that was created
    nodeType : string
        type of node to install callbacks for
    apiArgs : boolean
        if True, api objects (MObjects, MPlugs, etc) are left as is. If False, they're converted to string names 
    """
    if nodeType not in _nodeAddedCallbacks:
        manageCallback(om.MDGMessage.addNodeAddedCallback(_makeNodeAddedCB(nodeType), nodeType))
    _nodeAddedCallbacks[nodeType].append((func, apiArgs))

def _getHandle(obj):
    handle = om.MObjectHandle(obj)
    handle.__hash__ = handle.hashCode
    return handle

def _makeAttributeChangedCallback(nodeType):
    """
    make a function to be used with a nodeAdded callback which
    installs attributeChanged scriptjobs for the passed attribute
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
                    func = funcMap[plugName]
                except KeyError:
                    pass
                else:
                    func(plug, otherPlug, *args)

#        _attrChangedCallbacks[_getHandle(node)]
        om.MNodeMessage.addAttributeChangedCallback(obj, attrChanged)
    return installAttrChangeCallback

def _updateExistingNodes(nodeType, func):
    fnNode = om.MFnDependencyNode()
    nodeIt = om.MItDependencyNodes()
    while 1:
        node = nodeIt.item()
        if node.isNull():
            continue
        fnNode.setObject(node)
        if fnNode.typeName() == nodeType:
            print "updating", fnNode.name()
            func(node)
        nodeIt.next()
        if nodeIt.isDone():
            break

def addAttributeChangedCallback(func, nodeType, attribute, applyToExisting=True):
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
    applyToExisting : boolean
        whether to apply the function to existing nodes
    """
    global _attrChangedCallbacks
    nodeAddedCallback = _makeAttributeChangedCallback(nodeType)
    if nodeType not in _attrChangedCallbacks:
        # add a callback which creates the scriptJob that calls our function
        addNodeAddedCallback(nodeAddedCallback, nodeType, apiArgs=True)
        _attrChangedCallbacks[nodeType] = {}
    _attrChangedCallbacks[nodeType][attribute] = func

    # setup callback for existing nodes
    if applyToExisting and not om.MFileIO.isOpeningFile():
        _updateExistingNodes(nodeType, nodeAddedCallback)

def addAttributeChangedCallbacks(nodeType, attrFuncs):
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
    """
    global _attrChangedCallbacks
    nodeAddedCallback = _makeAttributeChangedCallback(nodeType)
    if nodeType not in _attrChangedCallbacks:
        # add a callback which creates the scriptJob that calls our function
        addNodeAddedCallback(nodeAddedCallback, nodeType, apiArgs=True)
        _attrChangedCallbacks[nodeType] = {}

    for attr, func in attrFuncs:
        _attrChangedCallbacks[nodeType][attr] = func

    # setup callback for existing nodes
    if not om.MFileIO.isOpeningFile():
        _updateExistingNodes(nodeType, nodeAddedCallback)

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


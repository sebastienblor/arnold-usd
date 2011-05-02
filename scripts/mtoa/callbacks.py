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
    def nodeAddedCB(node, *args):
        fnNode = om.MFnDependencyNode(node)
        name = fnNode.name()
        global _nodeAddedCallbacks
        for func in _nodeAddedCallbacks[nodeType]:
            func(name)
    return nodeAddedCB

def addNodeAddedCallback(func, nodeType):
    """
    creates and manages a node added callback

    Parameters
    ----------
    func : function
        should take a single string arg for the node that was created
    nodeType : string
        type of node to install attribute changed callbacks for 
    """
    if nodeType not in _nodeAddedCallbacks:
        manageCallback(om.MDGMessage.addNodeAddedCallback(_makeNodeAddedCB(nodeType), nodeType))
    _nodeAddedCallbacks[nodeType].append(func)

def _makeAttributeChangedCallback(func, attribute):
    """
    make a function to be used with a nodeAdded callback which
    installs attributeChanged scriptjobs for the passed attribute
    """
    def nodeAddedCallback(node):
        nodeAttr = node + '.' + attribute
        # scriptJob does not receive an arg, but we want ours to
        cmds.scriptJob(attributeChange=[nodeAttr, lambda: func(nodeAttr)])
    return nodeAddedCallback

def addAttributeChangedCallback(func, nodeType, attribute):
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
    """
    nodeAddedCallback = _makeAttributeChangedCallback(func, attribute)
    # add a callback which creates the scriptJob that calls our function
    addNodeAddedCallback(nodeAddedCallback, nodeType)

    # setup callback for existing nodes
    for node in cmds.ls(type=nodeType):
        nodeAddedCallback(node)

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
    callbacks = []
    for attr, func in attrFuncs:
        nodeAddedCallback = _makeAttributeChangedCallback(func, attr)
        # add a callback which creates the scriptJob that calls our function
        addNodeAddedCallback(nodeAddedCallback, nodeType)
        callbacks.append(nodeAddedCallback)

    # setup callback for existing nodes
    for node in cmds.ls(type=nodeType):
        for callback in callbacks:
            callback(node)

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


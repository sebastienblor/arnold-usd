import pymel.core as pm
import mtoa.utils as utils
import mtoa.callbacks as callbacks
from collections import namedtuple
from itertools import groupby

BUILTIN_AOVS = (
                ('P',       'point'),
                ('Z',       'float'),
                ('N',       'vector'),
                ('opacity', 'rgb'),
                ('motionvector', 'rgb'),
                ('Pref',    'rgb'),
                ('raycount','float'),
                ('cputime', 'float'),
#                ('RGB',     'rgb'),
#                ('RGBA',    'rgba'),
#                ('A',       'float'),
#                ('ID',      'int'),
#                ('OBJECT',  'node'),
#                ('SHADER',  'node'),
                )

# TODO: use types from arnold python module?
TYPES = (
    "int",
    "bool",
    "float",
    "rgb",
    "rgba",
    "vector",
    "point",
    "point2",
    "pointer")


GlobalAOVData = namedtuple('GlobalAOVData', ['name', 'attribute', 'type'])

SceneAOVData = namedtuple('SceneAOVData', ['name', 'type', 'index', 'node'])

class SceneAOV(object):
    def __init__(self, node, index):
        self._node = node
        self._index = index
    def __repr__(self):
        return '%s(%r, %d)' % (self.__class__.__name__, self._node, self._index)
    def __eq__(self, other):
        return other == self.name
    @property
    def index(self):
        return self._index
    @property
    def name(self):
        return self._node.attr('name').get()
    @property
    def type(self):
        return self._node.attr('type').get()
    @property
    def node(self):
        return self._node

def safeDelete(node):
    '''delete a node, or disconnect it, if it is read-only'''
    if node.isReadOnly():
        node.message.disconnect()
    else:
        pm.delete(node)

#------------------------------------------------------------
# scene queries
#------------------------------------------------------------

class AOVNode(object):
    def __init__(self, node):
        self._node = node
        self._aovAttr = node.aovs

    def __repr__(self):
        return '%s(%r)' % (self.__class__.__name__, self._node)

    @property
    def node(self):
        if not self._node.exists():
            raise TypeError("node doesn't exist")
        return self._node
    
    def getActiveAOVs(self, group=False):
        '''
        return a list of (aovName, SceneAOV) tuples for all active AOVs in the scene
        if group is True, the SceneAOVs are grouped by name: (aovName, [SceneAOV1, SceneAOV2, ...])
        '''
        result = [SceneAOV(fromAttr.node(), toAttr.index()) for toAttr, fromAttr in self._aovAttr.inputs(plugs=True, connections=True)]
        result = [(x.name, x) for x in result]
        result = sorted(result, key = lambda x: x[0])
        if group:
            result = [(aovName, [x[1] for x in aovs]) for aovName, aovs in groupby(result, lambda x: x[0])]

        return result

    def getActiveAOVNodes(self, names=False):
        '''
        sorted by aovName
        @param names: if True, returns pairs of (aovName, aovNode). if False, returns a list of aovNodes
        '''
        if names:
            result = [(x.attr('name').get(), x) for x in self._aovAttr.inputs()]
            return sorted(result, key = lambda x: x[0])
        else:
            result = self._aovAttr.inputs()
            return sorted(result, key = lambda x: x.attr('name').get())

    def getAOVNode(self, aovName):
        '''
        given the name of an AOV, return the corresponding aov node
        
        raises an error if there is more than one match.
        returns None if there are no matches.
        '''
        matches = [aov for name, aov in self.getActiveAOVs() if name == aovName]
        if len(matches) > 1:
            raise ValueError("More than one AOV matches name %r" % aovName)
        elif matches:
            return matches[0].node

    def getAOVNodes(self, aovName):
        '''
        given the name of an AOV, return the corresponding aov nodes
        '''
        return [aov.node for name, aov in self.getActiveAOVs() if name == aovName]

    def getAOVMap(self, removeDupes=True):
        '''return a mapping from aovName to aovNode
        
        @param removeDupes: if False, nodes will be stored in the map within a list'''
        if removeDupes:
            return dict(self.getActiveAOVNodes(names=True))
        else:
            aovMap = {}
            for aovName, aov in self.getActiveAOVs():
                if aovName in aovMap:
                    val = aovMap[aov.name]
                    if not isinstance(val, list):
                        val = [val]
                    val.append(aov.node)
                    aovMap[aovName] = val
                else:
                    aovMap[aovName] = aov.node
            return aovMap

    def addAOV(self, aovName, aovType='rgba'):
        '''
        add an AOV to the active list for this AOV node

        returns the created AOV node
        '''
        aovNode = pm.createNode('aiAOV', name='aiAOV_' + aovName, skipSelect=True)
        aovNode.attr('name').set(aovName)
        aovNode.attr('type').set(aovType)
        aovNode.message.connect(self._aovAttr, nextAvailable=True)
        return aovNode

    def removeAOV(self, aovName):
        '''
        remove an AOV from the active list for this AOV node

        raises an error if there is more than one match
        returns True if the node was found and removed, False otherwise
        '''
        aovNode = self.getAOVNode(aovName)
        if aovNode:
            safeDelete(aovNode)
            return True
        return False

    def removeAOVs(self, aovName):
        '''
        remove AOVs matching aovName from the active list for this AOV node

        returns True if the node was found and removed
        '''
        matches = self.getAOVNodes(aovName)
        for aovNode in matches:
            safeDelete(aovNode)
        return bool(matches)

    def removeAOVNode(self, aovNode):
        safeDelete(aovNode)

    def renameAOVs(self, oldName, newName):
        '''
        rename an AOV in the active list for this AOV node
        '''
        matches = self.getAOVNodes(oldName)
        if matches:
            for aovNode in matches:
                aovNode.attr('name').set(newName)
        else:
            raise NameError('Scene does not contain any AOVs with name %r' % oldName)

def getAOVNode():
    return AOVNode(pm.PyNode('defaultArnoldRenderOptions'))

def getActiveAOVs(group=False):
    try:
        return getAOVNode().getActiveAOVs(group)
    except pm.MayaNodeError:
        return []

def getActiveAOVNodes(names=False):
    try:
        return getAOVNode().getActiveAOVNodes(names)
    except pm.MayaNodeError:
        return []

def getAOVMap():
    '''return a mapping from aovName to aovNode'''
    try:
        return getAOVNode().getAOVMap()
    except pm.MayaNodeError:
        return {}
#------------------------------------------------------------
# global queries
#------------------------------------------------------------

#def updateShadingGroups():
#    aovs = list(enumerate(getActiveAOVs()))
#    print aovs
#    for sg in pm.ls(type='shadingEngine'):
#        for i, aov in aovs:
#            print "setting ", sg.aiCustomAOVs[i].aovName, aov
#            sg.aiCustomAOVs[i].aovName.set(aov)
#            # TODO: check type
#            pm.aliasAttr(sg.attr(aov), remove=True)
#            pm.aliasAttr(aov, sg.aiCustomAOVs[i].aovColorInput)

def getRegisteredAOVs(builtin=False, nodeType=None):
    '''
    returns a list of all registered aov names.

    @param builtin: set to True to include built-in AOVs
    @param nodeType: a node name or list of node names to restrict result to AOVs for only those nodes
    '''
    if nodeType:
        if isinstance(nodeType, (list, tuple)):
            result = [x[0] for x in getNodeGlobalAOVData(nt) for nt in nodeType]
        else:
            result = [x[0] for x in getNodeGlobalAOVData(nodeType)]
    else:
        result = pm.cmds.arnoldPlugins(listAOVs=True)
    if builtin:
        result = getBuiltinAOVs() + result
    return result

def getBuiltinAOVs():
    return [x[0] for x in BUILTIN_AOVS]

def getNodeGlobalAOVData(nodeType):
    "returns a list of registered (name, attribute, data type) pairs for the given node type"
    # convert to a 2d array
    result = [GlobalAOVData(*x) for x in utils.groupn(pm.cmds.arnoldPlugins(listAOVs=True, nodeType=nodeType), 3)]
    return sorted(result, key=lambda x: x.name)

def getNodeTypesWithAOVs():
    return sorted(pm.cmds.arnoldPlugins(listAOVNodeTypes=True))

#- groups

def getAOVGroups():
    return ['<builtin>']

def getGroupAOVs(groupName):
    if groupName == '<builtin>':
        return getBuiltinAOVs()
    raise


#------------------------------------------------------------
# callbacks
#------------------------------------------------------------

_aovOptionsChangedCallbacks = callbacks.DeferredCallbackQueue()

# a public function for adding AOV callbacks
def addAOVChangedCallback(func, key=None):
    _aovOptionsChangedCallbacks.addCallback(func, key)

if not pm.about(batch=True):
    callbacks.addAttributeChangedCallback(_aovOptionsChangedCallbacks.entryCallback, 'aiOptions', 'aovList',
                                          context=pm.api.MNodeMessage.kConnectionMade | pm.api.MNodeMessage.kConnectionBroken,
                                          applyToExisting=True)
    
    #callbacks.addAttributeChangedCallback(_aovOptionsChangedCallbacks.entryCallback, 'aiAOV', None, applyToExisting=True)


#def getAllShaderAOVs():
#    '''
#    collect a set of AOVs as present on shader attributes
#    '''
#    # TODO: rewrite in c++ if this is too slow
#    aovs = set([])
#    for nodeType in pm.cmds.arnoldPlugins(listAOVNodeTypes=True):
#        attrs = dict(getNodeGlobalAOVData(nodeType)).values()
#        for node in pm.ls(type=nodeType):
#            for attr in attrs:
#                aovs.add(pm.getAttr(node + '.' + attr))
#    return aovs

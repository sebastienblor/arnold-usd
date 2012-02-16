import pymel.core as pm
import mtoa.utils as utils
from collections import namedtuple

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

    def getActiveAOVs(self, indices=False):
        '''
        @param indicies: if True, returns pairs of (index, aovName)
        '''
        result = []
        for fromAttr, toAttr in self._aovAttr.inputs(connections=True, plugs=True, sourceFirst=True):
            aovNode = fromAttr.node()
            name = aovNode.attr('name').get()
            if name == '':
                pm.removeMultiInstance(toAttr, b=True)
            else:
                if indices:
                    result.append((toAttr.index(), name))
                else:
                    result.append(name)
        return result

    def getActiveAOVNodes(self, names=False):
        '''
        @param sort: if True, sort by aovName
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
        '''
        for aovNode in self.getActiveAOVNodes():
            if aovNode.attr('name').get() == aovName:
                return aovNode

    def getAOVMap(self):
        '''return a mapping from aovName to aovNode'''
        return dict(self.getActiveAOVNodes(names=True))

    def addAOV(self, aovName, aovType='rgba'):
        '''
        add an AOV to the active list for this AOV node if it does not already exist

        returns the created AOV node
        '''
        if aovName not in self.getActiveAOVs():
            aovNode = pm.createNode('aiAOV', name='aiAOV_' + aovName, skipSelect=True)
            aovNode.attr('name').set(aovName)
            aovNode.attr('type').set(aovType)
            aovNode.message.connect(self._aovAttr, nextAvailable=True)
            return aovNode

    def removeAOV(self, aovName):
        '''
        remove an AOV from the active list for this AOV node

        returns True if the node was found and removed
        '''
        aovNode = self.getAOVNode(aovName)
        if aovNode:
            safeDelete(aovNode)
            return True
        return False

    def removeAOVNode(self, aovNode):
        safeDelete(aovNode)

    def renameAOV(self, oldName, newName):
        '''
        rename an AOV in the active list for this AOV node

        returns the index of the AOV renamed or None, if the named aov does not exist on the node
        '''
        aovNode = self.getAOVNode(oldName)
        if aovNode:
            aovNode.attr('name').set(newName)

def getAOVNode():
    return AOVNode(pm.PyNode('defaultArnoldRenderOptions'))

def getActiveAOVs():
    return getAOVNode().getActiveAOVs()

def getActiveAOVNodes(names=False):
    return getAOVNode().getActiveAOVNodes(names)

def getAOVMap():
    '''return a mapping from aovName to aovNode'''
    return getAOVNode().getAOVMap()

#------------------------------------------------------------
# global queries
#------------------------------------------------------------

AOVData = namedtuple('AOVData', ['name', 'attribute', 'type'])

def getRegisteredAOVs(builtin=False, nodeType=None):
    '''
    returns a list of all registered aov names.

    @param builtin: set to True to include built-in AOVs
    @param nodeType: a node name or list of node names to restrict result to AOVs for only those nodes
    '''
    if nodeType:
        if isinstance(nodeType, (list, tuple)):
            result = [x[0] for x in getNodeAOVData(nt) for nt in nodeType]
        else:
            result = [x[0] for x in getNodeAOVData(nodeType)]
    else:
        result = pm.cmds.arnoldPlugins(listAOVs=True)
    if builtin:
        result = getBuiltinAOVs() + result
    return result

def getBuiltinAOVs():
    return [x[0] for x in BUILTIN_AOVS]

def getNodeAOVData(nodeType):
    "returns a list of registered (name, attribute, data type) pairs for the given node type"
    # convert to a 2d array
    result = [AOVData(*x) for x in utils.groupn(pm.cmds.arnoldPlugins(listAOVs=True, nodeType=nodeType), 3)]
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

#def getAllShaderAOVs():
#    '''
#    collect a set of AOVs as present on shader attributes
#    '''
#    # TODO: rewrite in c++ if this is too slow
#    aovs = set([])
#    for nodeType in pm.cmds.arnoldPlugins(listAOVNodeTypes=True):
#        attrs = dict(getNodeAOVData(nodeType)).values()
#        for node in pm.ls(type=nodeType):
#            for attr in attrs:
#                aovs.add(pm.getAttr(node + '.' + attr))
#    return aovs

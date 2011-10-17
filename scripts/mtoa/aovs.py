import pymel.core as pm
import sys

BUILTIN_AOVS = (
                ('P',       'POINT'),
                ('Z',       'FLOAT'),
                ('N',       'VECTOR'),
                ('OPACITY', 'RGB'),
                ('motionvector', 'RGB'),
                ('Pref',    'RGB'),
                ('raycount','FLOAT'),
                ('cputime', 'FLOAT'),
#                ('RGB',     'RGB'),
#                ('RGBA',    'RGBA'),
#                ('A',       'FLOAT'),
#                ('ID',      'INT'),
#                ('OBJECT',  'NODE'),
#                ('SHADER',  'NODE'),
                )

def getAOVNode():
    return AOVNode(pm.PyNode('defaultArnoldRenderOptions'))

def safeDelete(node):
    if node.isReadOnly():
        node.message.disconnect()
    else:
        pm.delete(node)

class AOVNode(object):
    def __init__(self, nodeName):
        self._node = nodeName
        self._aovAttr = nodeName.aovs

    def __repr__(self):
        return '%s(%r)' % (self.__class__.__name__, self._node)

    @property
    def node(self):
        return self._node

    def getIndices(self):
        indices = self._aovAttr.getArrayIndices()
        # don't know why the -1 index shows up sometimes: api bug coming through in pymel.
        if indices and indices[-1] == -1:
            indices.pop()
        return indices

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

    def addAOV(self, aovName):
        '''
        add an AOV to the active list for this AOV node if it does not already exist

        returns the created AOV node
        '''
        if aovName not in self.getActiveAOVs():
            aovNode = pm.createNode('aiAOV', name='aiAOV_' + aovName, skipSelect=True)
            aovNode.attr('name').set(aovName)
            aovNode.message.connect(self._aovAttr, nextAvailable=True)
            return aovNode

#    def removeAOV(self, aov):
#        '''
#        remove an AOV from the active list for this AOV node
#
#        returns the index of the AOV removed or None, if the named aov does not exist on the node
#        '''
#        if isinstance(aov, int):
#            at = self._aovAttr[aov]
#            inputs = at.inputs()
#            if inputs:
#                name = inputs[0].attr('name').get()
#                safeDelete(inputs[0])
#            else:
#                name = None
#            pm.removeMultiInstance(at, b=True)
#            return name
#        elif isinstance(aov, basestring):
#            for i in self.getIndices():
#                at = self._aovAttr[i]
#                inputs = at.inputs()
#                if inputs:
#                    name = inputs[0].attr('name').get()
#                    if name == aov:
#                        safeDelete(inputs[0])
#                        # the following line is crashing when called from AOV browser
#                        #pm.removeMultiInstance(at, b=True)
#                        return i

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

def getActiveAOVs():
    return getAOVNode().getActiveAOVs()

def getActiveAOVNodes(names=False):
    return getAOVNode().getActiveAOVNodes(names)

def getAOVMap():
    '''return a mapping from aovName to aovNode'''
    return dict(getActiveAOVNodes(names=True))


def getRegisteredAOVs(builtin=False, nodeType=None):
    '''
    returns a list of all registered aov names.

    @param builtin: set to True to include built-in AOVs
    @param nodeType: provide a node name or list of node names, returns AOVs for only those nodes
    '''
    if nodeType:
        if isinstance(nodeType, (list, tuple)):
            result = [x[0] for x in getNodeAOVAttrs(nt) for nt in nodeType]
        else:
            result = [x[0] for x in getNodeAOVAttrs(nodeType)]
    else:
        result = pm.arnoldPlugins(listAOVs=True)
    if builtin:
        result = getBuiltinAOVs() + result
    return result

def getBuiltinAOVs():
    return [x[0] for x in BUILTIN_AOVS]

def getNodeAOVAttrs(nodeType):
    "returns a list of registered (aov name, aov attribute) pairs for the given node type"
    # convert to a 2d array
    tmp = pm.arnoldPlugins(listAOVs=True, nodeType=nodeType)
    result = []
    for i in range(0, len(tmp), 2):
        result.append((tmp[i], tmp[i+1]))
    # sort alphabetically by aov name
    return sorted(result, key=lambda x: x[0])

def getNodeAOVs(nodeType):
    "returns a list of registered aov names for the given node type"
    return [x[0] for x in getNodeAOVAttrs(nodeType)]

def getNodesWithAOVs():
    return sorted(pm.arnoldPlugins(listAOVNodeTypes=True))

def getAOVGroups():
    return ['<builtin>']

def getGroupAOVs(groupName):
    if groupName == '<builtin>':
        return getBuiltinAOVs()
    raise

def getAllShaderAOVs():
    '''
    collect a set of AOVs as present on shader attributes
    '''
    # TODO: rewrite in c++ if this is too slow
    aovs = set([])
    for nodeType in pm.arnoldPlugins(listAOVNodeTypes=True):
        attrs = dict(getNodeAOVAttrs(nodeType)).values()
        for node in pm.ls(type=nodeType):
            for attr in attrs:
                aovs.add(pm.getAttr(node + '.' + attr))
    return aovs

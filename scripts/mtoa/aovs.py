import pymel.core as pm

BUILTIN_AOVS = (
#                ('RGB',     'RGB'),
#                ('RGBA',    'RGBA'),
#                ('A',       'FLOAT'),
                ('Z',       'FLOAT'),
                ('OPACITY', 'RGB'),
                ('ID',      'INT'),
                ('OBJECT',  'NODE'),
                ('SHADER',  'NODE'),
                ('P',       'POINT'),
                ('N',       'VECTOR'))

def getAOVNode():
    conns = pm.listConnections('defaultArnoldRenderOptions.aovs', s=1, d=0)
    if not conns:
        aovnode = pm.createNode('ArnoldAOV')
        pm.connectAttr('%s.message' % aovnode, 'defaultArnoldRenderOptions.aovs')
    else:
        aovnode = conns[0]
    return AOVNode(aovnode)

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

    def getAOVs(self, indices=False):
        """
        @param indicies: if True, returns pairs of (index, aovName)
        """
        result = []
        for i in self.getIndices():
            name = self._aovAttr[i].attr('name').get()
            if name == '':
                pm.removeMultiInstance(self._aovAttr[i], b=True)
            else:
                if indices:
                    result.append((i, name))
                else:
                    result.append(name)
        return result

    def addAOV(self, aovName):
        """
        add an AOV to the active list for this AOV node if it does not already exist
        
        returns the index of the newly created AOV
        """
        indices = self.getIndices()
        if indices:
            for i in indices:
                name = self._aovAttr[i].attr('name').get()
                if name == aovName:
                    # already exists
                    return
                elif len(name) == 0:
                    break
            i+=1
        else:
            i = 0

        # create first
        self._aovAttr[i].get()
        # now set
        self._aovAttr[i].attr('name').set(aovName)
        return i

    def makeContiguous(self):
        # sparse array
        indices = self.getIndices()
        for i, index in enumerate(indices):
            if i != index:
                aovFrom = self._aovAttr[index]
                aovTo = self._aovAttr[i]
                aovTo.get()
                aovTo.attr('name').set(aovFrom.attr('name').get())
                aovTo.attr('prefix').set(aovFrom.attr('prefix').get())
        n = len(indices) - 1
        # remove remaining
        for i in indices:
            if i > n:
                pm.removeMultiInstance(self._aovAttr[i], b=True)

    def removeAOV(self, aov):
        """
        remove an AOV from the active list for this AOV node
        
        returns the index of the AOV removed or None, if the named aov does not exist on the node
        """
        if isinstance(aov, int):
            at = self._aovAttr[aov]
            name = at.attr('name').get()
            pm.removeMultiInstance(at, b=True)
            self.makeContiguous()
            return name
        else:
            for i in self.getIndices():
                at = self._aovAttr[i]
                name = at.attr('name').get()
                if name == aov:
                    pm.removeMultiInstance(self._aovAttr[i], b=True)
                    self.makeContiguous()
                    return i

    def renameAOV(self, oldName, newName):
        """
        rename an AOV in the active list for this AOV node
        
        returns the index of the AOV renamed or None, if the named aov does not exist on the node
        """
        n = pm.getAttr('%s.aovs' % self._node, size=True)
        for i in range(n):
            name = pm.getAttr('%s.aovs[%d].name' % (self._node, i))
            if name == oldName:
                # set last to null so we don't proceed beyond it (will be cleared by maya after save and reopen)
                pm.setAttr('%s.aovs[%d].name' % (self._node, i), newName, type='string')
                return i

def getActiveAOVs():
    return getAOVNode().getAOVs()

def getAOVs(builtin=False, nodeType=None):
    """
    returns a list of all registered aov names.
    
    @param builtin: set to True to include built-in AOVs
    @param nodeType: provide a node name or list of node names
    """
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
    return result

def getNodeAOVs(nodeType):
    "returns a list of register aov names fro the given node type"
    return [x[0] for x in getNodeAOVAttrs(nodeType)]

def getNodesWithAOVs():
    return pm.arnoldPlugins(listAOVNodeTypes=True)


def getAOVGroups():
    return ['<builtin>']

def getGroupAOVs(groupName):
    if groupName == '<builtin>':
        return getBuiltinAOVs()
    raise

def getAllShaderAOVs():
    """
    collect a set of AOVs as present on shader attributes
    """
    # TODO: rewrite in c++ if this is too slow
    aovs = set([])
    for nodeType in pm.arnoldPlugins(listAOVNodeTypes=True):
        attrs = dict(getNodeAOVAttrs(nodeType)).values()
        for node in pm.ls(type=nodeType):
            for attr in attrs:
                aovs.add(pm.getAttr(node + '.' + attr))
    return aovs

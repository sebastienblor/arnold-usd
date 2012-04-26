'''
functions for dealing with mtoa node types and classifications
'''

import pymel.core as pm
from . import utils
from . import aovs
from . import callbacks

CATEGORY_TO_RUNTIME_CLASS = {
                ('shader',):            'asShader',
                ('texture',):           'asTexture',
                ('light',):             'asLight',
                ('light', 'filter'):    'asUtility',
                ('utility',):           'asUtility',
                }

def _processClass(nodeType):
    '''
    convert the passed node type's classification string to a tuple containing a formatted path string
    compatible with the node lister and the runtime classification.
    
    e.g. from 'aiStandard' to ('rendernode/arnold/shader/surface', 'asShader', 'Arnold/Shader/Surface')
    '''
    for klass in pm.getClassification(nodeType):
        if klass.startswith('rendernode/arnold'):
            parts = klass.split('/')
            if len(parts) < 3:
                return (klass, 'asUtility', 'Arnold')
            else :
                # remove the rendernode first token
                parts.pop(0)
                label = '/'.join([utils.prettify(x) for x in parts])
                cat = 'asUtility'
                # find a runtime classification. try matching from most specific to most generic
                # first token is always 'arnold':
                parts.pop(0)
                while parts:
                    try:
                        cat = CATEGORY_TO_RUNTIME_CLASS[tuple(parts)]
                    except KeyError:
                        parts.pop(-1)
                    else:
                        break
                return (klass, cat, label)
    return (None, None, None)

def isSubClassification(testClass, otherClass):
    '''
    returns True if the first classification is contained within the second
    
    for example 'rendernode/arnold/shader/displacement' is a sub-filter of 'rendernode/arnold/shader'
    '''
    otherParts = otherClass.split('/')
    testParts = testClass.split('/')
    if len(testParts) < len(otherParts):
        return False
    return testParts[:len(otherParts)] == otherParts

def getRuntimeClass(nodeType):
    '''
    get the runtime classification of an arnold node
    '''
    return _processClass(nodeType)[1]

def createArnoldNode(nodeType, name=None, skipSelect=False, runtimeClassification=None):
    '''
    create an arnold node with the proper runtime classification
    '''
    kwargs = {}
    kwargs['skipSelect'] = skipSelect
    if name:
        kwargs['name'] = name
    if runtimeClassification is None:
        runtimeClassification = getRuntimeClass(nodeType)
    if runtimeClassification:
        kwargs[runtimeClassification] = True
        node = pm.shadingNode(nodeType, **kwargs)
    else:
        pm.warning("[mtoa] Could not determine runtime classification of %s: set maya.classification metadata" % nodeType)
        node = pm.createNode(nodeType, **kwargs)

    createOptions()

    return node

def getAttributeData(nodeType):
    import maya.cmds as cmds
    data = cmds.arnoldPlugins(getAttrData=nodeType) or []
    # convert empty strings to None
    data = [x or None for x in data]
    return utils.groupn(data, 4)

def arnoldIsCurrentRenderer():
    "return whether arnold is the current renderer"
    return pm.getAttr('defaultRenderGlobals.currentRenderer') == 'arnold'

def listTranslators(nodeType):
    '''
    return a list of (translator, arnoldNode) pairs
    '''
    import maya.cmds as cmds
    data = cmds.arnoldPlugins(listTranslators=nodeType) or []
    # convert empty strings to None
    data = [x or None for x in data]
    return utils.groupn(data, 2)

def createStandIn(path=None):
    if not pm.objExists('ArnoldStandInDefaultLightSet'):
        pm.createNode("objectSet", name="ArnoldStandInDefaultLightSet", shared=True)
        pm.lightlink(object='ArnoldStandInDefaultLightSet', light='defaultLightSet')

    standIn = pm.createNode('aiStandIn', n='ArnoldStandInShape')
    # temp fix until we can correct in c++ plugin
    standIn.visibleInReflections.set(True)
    standIn.visibleInRefractions.set(True)
    pm.sets('ArnoldStandInDefaultLightSet', add=standIn)
    if path:
        standIn.dso.set(path)
    return standIn

def upgradeAOVOutput(options, defaultFilter=None, defaultDriver=None):
    """
    Upgrades scenes to use new node-base filter and drivers

    Unfortunately, in Maya 2012 old driver/filter attributes are extension attributes, so no longer
    exist on the aiAOV node.  As a result AOVs that overrode the global value will lose 
    driver/filter specific settings like compression and quality.
    """
    print "[mtoa] upgrading to new AOV driver/filter setup"
    aovNodes = pm.ls(type='aiAOV')
    if defaultDriver is None:
        defaultDriver = pm.nt.DependNode('defaultArnoldDriver')
        
    if defaultFilter is None:
        defaultFilter = pm.nt.DependNode('defaultArnoldFilter')

    driver = options.imageFormat.get()
    defaultDriver.aiTranslator.set(driver)

    filter = options.filterType.get()
    defaultFilter.aiTranslator.set(filter)

    data = [(aovNodes, 'aiAOVDriver', '.outputs[0].driver', 'imageFormat', defaultDriver),
            (aovNodes, 'aiAOVFilter', '.outputs[0].filter', 'filterType', defaultFilter)]

    for nodes, mayaNodeType, inputAttr, controlAttr, defaultNode in data:
#        attrSet = set([])
#        for transName, arnoldNode in listTranslators(mayaNodeType):
#            print "\t", arnoldNode
#            for paramName, attrName, label, annotation in getAttributeData(arnoldNode):
#                print "\t\t", paramName, attrName
#                attrSet.add(attrName)
        for node in nodes:
            at = node.name() + inputAttr
            inputs = pm.listConnections(at, source=True, destination=False)
            if not inputs:
                translator = node.attr(controlAttr).get()
                if translator in ['', '<Use Globals>']:
                    defaultNode.message.connect(at)
                    print "[mtoa] upgrading %s: connected to default node %s" % (node, defaultNode)
                else:
                    outputNode = pm.createNode(mayaNodeType, skipSelect=True)
                    print "[mtoa] upgrading %s: created new node %s and set translator to %r" % (node, outputNode, translator)
                    outputNode.message.connect(at)
                    outputNode.aiTranslator.set(translator)

#                for attr in attrSet:
#                    oldName = outputType + attr[0].upper() + attr[1:]
#                    try:
#                        value = aovNode.attr(oldName).get()
#                        print aovNode, oldName, value
#                    except AttributeError:
#                        pass
#                    outputNode.attr(attr)


def createOptions():
    """
    override this with your own function to set defaults
    """
    # the shared option ensures that it is only created if it does not exist
    options = pm.createNode('aiOptions', skipSelect=True, shared=True, name='defaultArnoldRenderOptions')
    filterNode = pm.createNode('aiAOVFilter', name='defaultArnoldFilter', skipSelect=True, shared=True)
    driverNode = pm.createNode('aiAOVDriver', name='defaultArnoldDriver', skipSelect=True, shared=True)

    if (filterNode or driverNode) and not options:
        options = pm.nt.DependNode('defaultArnoldRenderOptions')
        # options previously existed, so we need to upgrade
        upgradeAOVOutput(options, filterNode, driverNode)

    # if we're just creating the options node, then be sure to connect up the driver and filter
    if not filterNode:
        filterNode = pm.nt.DependNode('defaultArnoldFilter')
    if not driverNode:
        driverNode = pm.nt.DependNode('defaultArnoldDriver')
    if not options:
        options = pm.nt.DependNode('defaultArnoldRenderOptions')

    filterNode.message.connect(options.filter, force=True)
    driverNode.message.connect(options.driver, force=True)



#-------------------------------------------------
# translator defaults
#-------------------------------------------------

_defaultTranslators = {}

def _doSetDefaultTranslator(node):
    if not arnoldIsCurrentRenderer(): return
    try:
        node.attr('aiTranslator').set(getDefaultTranslator(node))
    except RuntimeError:
        pm.warning("failed to set default translator for %s" % node.name())

def registerDefaultTranslator(nodeType, default):
    """
    Register the default translator for a node type. The second argument identifies the name of the
    translator.  Pass the translator name (as a string) if the default is always the same,
    or a function that takes the current node as a pymel PyNode and returns the translator name as a string.

    The default will automatically be set whenever a node of the given type is added to the scene.
    """

    global _defaultTranslators
    _defaultTranslators[nodeType] = default

    if arnoldIsCurrentRenderer():
        # set defaults for existing nodes of this type
        for node in pm.ls(exactType=nodeType):
            if callable(default):
                val = default(node)
            else:
                val = default
            node.attr('aiTranslator').set(val)

    callbacks.addNodeAddedCallback(_doSetDefaultTranslator, nodeType)

def getDefaultTranslator(node):
    if isinstance(node, basestring):
        node = pm.PyNode(node)
    global _defaultTranslators
    try:
        default = _defaultTranslators[node.type()]
        if callable(default):
            return default(node)
        else:
            return default
    except KeyError:
        pass

def _rendererChanged(plug, *args):
    if plug.asString() == 'arnold':
        global _defaultTranslators
        for nodeType, default in _defaultTranslators.iteritems():
            if default:
                # set defaults for existing nodes
                for node in pm.ls(exactType=nodeType):
                    # only set the default if it has not already been set
                    at = node.attr('aiTranslator')
                    if not at.get():
                        if callable(default):
                            val = default(node)
                        else:
                            val = default
                        at.set(val)


callbacks.addAttributeChangedCallback(_rendererChanged, 'renderGlobals', 'currentRenderer')

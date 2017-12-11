'''
functions for dealing with mtoa node types and classifications
'''

import mtoa.utils as utils
import mtoa.callbacks as callbacks
import maya.cmds as cmds
import maya.mel as mel
import maya.OpenMaya as om
import os

CATEGORY_TO_RUNTIME_CLASS = {
                ('shader',):            'asShader',
                ('texture',):           'asTexture',
                ('light',):             'asLight',
                ('light', 'filter'):    'asUtility',
                ('utility',):           'asUtility',
                }

MTOA_GLOBALS = {}
ACTIVE_CAMERA = None

def _processClass(nodeType):
    '''
    convert the passed node type's classification string to a tuple containing a formatted path string
    compatible with the node lister and the runtime classification.
    
    e.g. from 'aiStandard' to ('rendernode/arnold/shader/surface', 'asShader', 'Arnold/Shader/Surface')
    '''
    nodeClassifications = cmds.getClassification(nodeType)[0].split(':') or []

    for klass in nodeClassifications:
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
        node = cmds.shadingNode(nodeType, **kwargs)
    else:
        cmds.warning("[mtoa] Could not determine runtime classification of %s: set maya.classification metadata" % nodeType)
        node = cmds.createNode(nodeType, **kwargs)

    createShadingGroupIfNeeded(nodeType, node)
    createOptions()

    return node

_mtoaNodes = None
def isMtoaNode(nodeType):
    """
    return whether the passed node type was created by mtoa
    """
    global _mtoaNodes
    if _mtoaNodes is None:
        _mtoaNodes = cmds.pluginInfo('mtoa', query=True, dependNode=True)
    return nodeType in _mtoaNodes

def getAttributeData(nodeType):
    import maya.cmds as cmds
    data = cmds.arnoldPlugins(getAttrData=nodeType) or []
    # convert empty strings to None
    data = [x or None for x in data]
    return utils.groupn(data, 4)

def arnoldIsCurrentRenderer():
    "return whether arnold is the current renderer"
    return cmds.getAttr('defaultRenderGlobals.currentRenderer') == 'arnold'

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
    if not cmds.objExists('ArnoldStandInDefaultLightSet'):
        cmds.createNode("objectSet", name=":ArnoldStandInDefaultLightSet", shared=True)
        cmds.lightlink(object='ArnoldStandInDefaultLightSet', light='defaultLightSet')

    standIn = cmds.createNode('aiStandIn', n='aiStandInShape')
    # temp fix until we can correct in c++ plugin
#    cmds.setAttr('%s.visibleInReflections' % standIn.name(), True)
#    cmds.setAttr('%s.visibleInRefractions' % standIn.name(), True)
    cmds.sets(standIn, add='ArnoldStandInDefaultLightSet')
    if path:
        standIn.dso.set(path)
    return standIn
    
def createVolume():
    cmds.createNode('aiVolume', n='aiVolumeShape')


# FIXME can we get rid of this function now ?
def upgradeAOVOutput(options, defaultFilter=None, defaultDriver=None):
    """
    Upgrades scenes to use new node-base filter and drivers

    Unfortunately, in Maya 2012 old driver/filter attributes are extension attributes, so no longer
    exist on the aiAOV node.  As a result AOVs that override the global value will lose 
    driver/filter specific settings like compression and quality.
    """
    print "[mtoa] upgrading to new AOV driver/filter setup"
    aovNodes = cmds.ls(type='aiAOV')
    if defaultDriver is None:
        defaultDriver = 'defaultArnoldDriver'
        
    if defaultFilter is None:
        defaultFilter = 'defaultArnoldFilter'

    driver = cmds.getAttr('{}.imageFormat'.format(options), asString=True)
    if driver:
        cmds.setAttr('{}.aiTranslator'.format(defaultDriver), driver, type="string")
        

    filter = cmds.getAttr('{}.filterType'.format(options), asString=True)
    if filter:
        cmds.setAttr('{}.aiTranslator'.format(defaultFilter), filter, type="string")

    data = [(aovNodes, 'aiAOVDriver', '.outputs[0].driver', 'imageFormat', defaultDriver),
            (aovNodes, 'aiAOVFilter', '.outputs[0].filter', 'filterType', defaultFilter)]

    for nodes, mayaNodeType, inputAttr, controlAttr, defaultNode in data:
        for node in nodes:
            at = node + inputAttr
            inputs = cmds.listConnections(at, source=True, destination=False)
            if not inputs:
                translator = cmds.getAttr('{}.{}'.format(node, controlAttr), asString=True)
                if translator in ['', '<Use Globals>']:
                    cmds.connectAttr('{}.message'.format(defaultNode), at)
                    print "[mtoa] upgrading %s: connected to default node %s" % (node, defaultNode)
                else:
                    outputNode = cmds.createNode(mayaNodeType, skipSelect=True)
                    print "[mtoa] upgrading %s: created new node %s and set translator to %r" % (node, outputNode, translator)
                    cmds.connectAttr('{}.message'.format(outputNode), at)
                    cmds.setAttr('{}.aiTranslator'.format(outputNode), translator, type="string")




def createOptions():
    mtoaLoaded = cmds.pluginInfo('mtoa', query=True,loaded=True)
    
    if not mtoaLoaded:
        cmds.error("MtoA plugin isn't loaded")
        return
        
    """
    override this with your own function to set defaults
    """
    import mtoa.aovs as aovs
    import mtoa.hooks as hooks

    # the shared option ensures that it is only created if it does not exist
    # testing for obj existence before creating because createNode with shared and forcing a namespace
    # will switch the namespace if the object already exists (it's bugged).
    options = cmds.createNode('aiOptions', skipSelect=True, shared=True, name=':defaultArnoldRenderOptions')\
        if not cmds.objExists('defaultArnoldRenderOptions') else None
    filterNode = cmds.createNode('aiAOVFilter', name=':defaultArnoldFilter', skipSelect=True, shared=True)\
        if not cmds.objExists('defaultArnoldFilter') else None
    driverNode = cmds.createNode('aiAOVDriver', name=':defaultArnoldDriver', skipSelect=True, shared=True)\
        if not cmds.objExists('defaultArnoldDriver') else None
    displayDriverNode = cmds.createNode('aiAOVDriver', name=':defaultArnoldDisplayDriver', skipSelect=True, shared=True)\
        if not cmds.objExists('defaultArnoldDisplayDriver') else None

    if (filterNode or driverNode) and not options:
        options = 'defaultArnoldRenderOptions'
        # options previously existed, so we need to upgrade
        upgradeAOVOutput(options, filterNode, driverNode)

    # if we're just creating the options node, then be sure to connect up the driver and filter
    if filterNode:
        # newly created default filter
        hooks.setupFilter(filterNode)
    else:
        filterNode = 'defaultArnoldFilter'

    if driverNode:
        # newly created default driver
        hooks.setupDriver(driverNode)
    else:
        driverNode = 'defaultArnoldDriver'

    if options:
        # newly created options
        hooks.setupDefaultAOVs(aovs.AOVInterface(options))
        hooks.setupOptions(options)
        cmds.setAttr('defaultArnoldRenderOptions.version', str(cmds.pluginInfo( 'mtoa', query=True, version=True)), type="string")
    else:
        options = 'defaultArnoldRenderOptions'
        if displayDriverNode:
            # options exist, but not display driver: upgrade from older version of mtoa
            hooks.setupDefaultAOVs(aovs.AOVInterface(options))

    if displayDriverNode:
        # newly created default driver
        # FIXME why is there a 'maya' translator ??
        cmds.setAttr('{}.aiTranslator'.format(displayDriverNode), 'maya', type="string")
        # GUI only
        cmds.setAttr('{}.outputMode'.format(displayDriverNode), 0)        
        hooks.setupDriver(displayDriverNode)
        cmds.connectAttr('{}.message'.format(displayDriverNode), '{}.drivers'.format(options), nextAvailable=True)
    elif not cmds.listConnections('{}.drivers'.format(options), source=True, destination=False):
        cmds.connectAttr('defaultArnoldDisplayDriver.message', '{}.drivers'.format(options), nextAvailable=True)
    try:
        # we first verify if option's attribute filter is already connected to the right node.
        # In Maya 2018, connecting to the same attribute sends a dirtiness signal for this attribute,
        # and therefore restarts the render (#3178)
        filterInputs = cmds.listConnections('{}.filter'.format(options), source=True, destination=False)
        if (filterInputs is None) or (len(filterInputs) == 0) or (filterInputs[0] != filterNode):
            cmds.connectAttr('%s.message' % filterNode, '%s.filter' % options, force=True)
        
    except:
        pass
    try:
        # we first verify if option's attribute driver is already connected to the right node.
        # Same reason as with filter above (#3178)
        driverInputs = cmds.listConnections('{}.driver'.format(options), source=True, destination=False)
        if driverInputs is None or len(driverInputs) == 0 or (driverInputs[0] != driverNode):
            cmds.connectAttr('%s.message' % driverNode, '%s.driver' % options, force=True)
    except:
        pass
    

# If the current node is a surface shader then create the shading group for it and connect it.
def createShadingGroupIfNeeded(nodeType, node):
    if cmds.getClassification(nodeType, sat="shader/surface") and cmds.getClassification(nodeType, sat="rendernode/arnold/shader/surface"):
        shadingNodeGroupName = node + "SG"
        group = cmds.sets(renderable=True, noSurfaceShader=True, empty=True, name=shadingNodeGroupName)
        outAttr = node

        if cmds.attributeQuery('outColor', node=node, exists=True):
            outAttr += ".outColor"
        else:
            outAttr += ".message"
        cmds.connectAttr(outAttr, group + ".surfaceShader")


#-------------------------------------------------
# translator defaults
#-------------------------------------------------

_defaultTranslators = {}

def _doSetDefaultTranslator(obj):
    if not arnoldIsCurrentRenderer():
        return
    try:
        default = getDefaultTranslator(obj)
        plug = om.MFnDependencyNode(obj).findPlug('aiTranslator')

        # we're also being called when an object is duplicated. In that case the attribute 
        # translator is already set to a given value. For newly created objects this value is empty
        if not plug.isNull() and plug.asString() == "":
            plug.setString(default)

    except RuntimeError:
        cmds.warning("failed to set default translator for %s" % om.MFnDependencyNode(obj).name())

def registerDefaultTranslator(nodeType, default):
    """
    Register the default translator for a node type. The second argument identifies the name of the
    translator.  Pass the translator name (as a string) if the default is always the same,
    or a function that takes the current node (as a string) and returns the translator name as a string.

    The default will automatically be set whenever a node of the given type is added to the scene.
    """

    global _defaultTranslators
    _defaultTranslators[nodeType] = default

    isFunc = callable(default)
    if not isFunc:
      cmds.arnoldPlugins(setDefaultTranslator=(nodeType, default))    
    if arnoldIsCurrentRenderer():
        it = om.MItDependencyNodes()
        while not it.isDone():
            obj = it.item()
            if not obj.isNull():
                mfn = om.MFnDependencyNode(obj)
                if mfn.typeName() == nodeType:
                    plug = mfn.findPlug("aiTranslator")
                    if not plug.isNull() and plug.asString() == "":
                        if isFunc:
                            val = default(obj)
                        else:
                            val = default
                        plug.setString(val)
            it.next()

    callbacks.addNodeAddedCallback(_doSetDefaultTranslator, nodeType,
                                   applyToExisting=False, apiArgs=True)

def getDefaultTranslator(obj):
    if isinstance(obj, basestring):
        selList = om.MSelectionList()
        selList.add(obj)
        obj = om.MObject()
        selList.getDependNode(0, obj)
        
    mfn = om.MFnDependencyNode(obj)
    global _defaultTranslators
    try:
        default = _defaultTranslators[mfn.typeName()]
        if callable(default):
            return default(obj)
        else:
            return default
    except KeyError:
        pass

def _rendererChanged(*args):
    if cmds.getAttr('defaultRenderGlobals.currentRenderer') == 'arnold':
        global _defaultTranslators

        it = om.MItDependencyNodes()
        while not it.isDone():
            obj = it.item()
            if not obj.isNull():
                mfn = om.MFnDependencyNode(obj)
                nodeType = mfn.typeName()
                if nodeType in _defaultTranslators:
                    default = _defaultTranslators[nodeType]
                    assert default is not None
                    plug = mfn.findPlug("aiTranslator")
                    if not plug.isNull() and plug.asString() == "":
                        if callable(default):
                            val = default(obj)
                        else:
                            val = default
                        plug.setString(val)
            it.next()
            
def installCallbacks():
    """
    install all callbacks
    """
    # certain scenes fail to execute this callback:
    #callbacks.addAttributeChangedCallback(_rendererChanged, 'renderGlobals', 'currentRenderer')
    if cmds.about(batch=True):
        callbacks.addAttributeChangedCallback(_rendererChanged, 'renderGlobals', 'currentRenderer')
    else:
        cmds.scriptJob(attributeChange=['defaultRenderGlobals.currentRenderer', _rendererChanged] )
        cmds.scriptJob(event =['SceneOpened', _rendererChanged] )

    import mtoa.aovs as aovs
    aovs.installCallbacks()

def uninstallCallbacks():
    #TODO: write uninstall code
    pass

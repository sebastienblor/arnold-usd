'''
functions for dealing with mtoa node types and classifications
'''

import pymel.core as pm
import mtoa.utils as utils
import mtoa.aovs as aovs

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
    
    e.g. from 'aiStandard' to ('arnold/shader/surface', 'asShader', 'Arnold/Shader/Surface')
    '''
    for klass in pm.getClassification(nodeType):
        if klass.startswith('arnold'):
            parts = klass.split('/')
            if len(parts) < 2:
                return (klass, 'asUtility', 'Arnold')
            else :
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
    
    for example 'arnold/shader/displacement' is a sub-filter of 'arnold/shader'
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

    # connect any shader aovs to global aov nodes
    if not pm.objExists('defaultArnoldRenderOptions'):
        pm.createNode('aiOptions', skipSelect=True, shared=True, name='defaultArnoldRenderOptions')
    activeAOVMap = aovs.getAOVMap()
    if activeAOVMap:
        for aovName, aovAttr, aovType in aovs.getNodeAOVData(nodeType):
            aovNodeAttr = node.attr(aovAttr)
            try:
                aovNode = activeAOVMap[aovNodeAttr.get()]
            except KeyError:
                pass
            else:
                aovNode.attr('name').connect(aovNodeAttr)
    return node

def getAttributeData(nodeType):
    import maya.cmds as cmds
    data = cmds.arnoldPlugins(getAttrData=nodeType) or []
    # convert empty strings to None
    data = [x or None for x in data]
    return utils.groupn(data, 4)

def listTranslators(nodeType):
    '''
    return a list of (translator, arnoldNode) pairs
    '''
    import maya.cmds as cmds
    data = cmds.arnoldPlugins(listTranslators=nodeType) or []
    # convert empty strings to None
    data = [x or None for x in data]
    return utils.groupn(data, 2)
    
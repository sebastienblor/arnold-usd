import maya.cmds as cmds
import mtoa.utils as utils

CATEGORY_TO_RUNTIME_CLASS = {
                'shader':       'asShader',
                'texture':      'asTexture',
                'light':        'asLight',
                'utility':      'asUtility',
                }

def _processClass(nodeType):
    '''
    convert the passed node type's classification string to a tuple containing a formatted path string
    compatible with the node lister and the runtime classification.
    
    e.g. from 'aiStandard' to ('arnold/shader/surface', 'asShader', 'Arnold/Shader/Surface')
    '''
    for klass in cmds.getClassification(nodeType)[0].split(':'):
        if klass.startswith('arnold'):
            parts = klass.split('/')
            if len(parts) < 2:
                return (klass, 'asUtility', 'Arnold')
            else :
                #labelName = node.replace('/', '|')
                return (klass,
                        CATEGORY_TO_RUNTIME_CLASS.get(parts[1], 'asUtility'),
                        '/'.join([utils.prettify(x) for x in parts]))
    return (None, None, None)

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
    return node

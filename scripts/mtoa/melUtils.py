import maya.cmds as cmds
import maya.mel as mel
import maya.api.OpenMaya as om


def getAttr(objName, attrName, asString=False, mi=False):
    cmds.getAttr('{}.{}'.format(objName, attrName), asString=asString, mi=mi)

def setAttr(objName, attrName, value, type=None):
    if type is None:
        cmds.setAttr('{}.{}'.format(objName, attrName), value)
    else:
        cmds.setAttr('{}.{}'.format(objName, attrName), value, type)

def getAttrInputs(objName, attrName, plugs=False, connections=False):
    return cmds.listConnections('{}.{}'.format(objName, attrName),
                                source=True, destination=False,
                                connections=connections, plugs=plugs) or []

def hasAttrInputs(objName, attrName):
    return cmds.listConnections('{}.{}'.format(objName, attrName),
                                source=True, destination=False)

def getAttrOutputs(objName, attrName, plugs=False, connections=False):
    return cmds.listConnections('{}.{}'.format(objName, attrName),
                                source=False, destination=True,
                                connections=connections, plugs=plugs) or []

def hasAttrOutputs(objName, attrName):
    return cmds.listConnections('{}.{}'.format(objName, attrName),
                                source=False, destination=True)

def getAttrNumElements(objName, attrName):
    sel = om.MSelectionList()
    sel.add(objName)
    obj = sel.getDependNode(0)
    plug = None
    ret = None
    if obj:
        depNodeFn = om.MFnDependencyNode(obj)
        attr = depNodeFn.attribute(attrName)
        plug = om.MPlug(obj, attr)

        ret = plug.evaluateNumElements()
    return ret

def getChildNumberElements(objName, attrName, index, childPlug):
    sel = om.MSelectionList()
    sel.add(objName)
    obj = sel.getDependNode(0)
    plug = None
    ret = None
    if obj:
        depNodeFn = om.MFnDependencyNode(obj)
        attr = depNodeFn.attribute(attrName)
        plug = om.MPlug(obj, attr)
        child = plug.elementByPhysicalIndex(index)
        partObj = child.attribute(childPlug)
        part = child.child(partObj)
        if part.isArray():
            ret = part.evaluateNumElements()
    return ret

def initVar(varName, type='string'):
    if varName.startswith('$'):
        varName = varName[1:]
    if varName.endswith('[]'):
        varName = varName[:-2]

    if type.endswith('[]'):
        type = type[:-2]
        varName += '[]'

    mel.eval('global {} ${};'.format(type, varName))
    
def getVar(varName, type='string', init=False):

    if init:
        initVar(varName, type)

    arrayToken = ''
    if varName.startswith('$'):
        varName = varName[1:]
    if varName.endswith('[]'):
        varName = varName[:-2]
    funcName = '_mtoa_global_var_%s' % varName
    if type.endswith('[]'):
        type = type[:-2]
        arrayToken = '[]'
        funcName += 'Array'


    cmd = 'global proc %s%s %s() { global %s $%s%s; return $%s; } %s()'%(type, arrayToken, funcName, type, varName, arrayToken, varName, funcName)
    return mel.eval(cmd)

def setVar(varName, value, type='string'):
    if varName.startswith('$'):
        varName = varName[1:]
    if varName.endswith('[]'):
        varName = varName[:-2]
    if type.endswith('[]'):
        type = type[:-2]
        value = str(value).replace('[','{').replace(']','}')

    if type == 'string':
        cmd = '${} =\"{}\";'.format(varName, value)
    else:
        cmd = '${} ={};'.format(varName, value)
    mel.eval(cmd)



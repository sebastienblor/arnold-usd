import maya.cmds as cmds
import maya.mel as mel


def getAttr(objName, attrName, asString=False, mi=False):
    cmds.getAttr('{}.{}'.format(objName, attrName), asString=asString, mi=mi)

def setAttr(objName, attrName, value, type=None):
    if type is None:
        cmds.setAttr('{}.{}'.format(objName, attrName), value)
    else:
        cmds.setAttr('{}.{}'.format(objName, attrName), value, type)

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



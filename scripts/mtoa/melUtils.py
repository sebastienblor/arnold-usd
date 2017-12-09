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
    mel.eval('global {} ${};'.format(type, varName))
    
def getVar(varName, type='string'):
    return mel.eval('global proc {} _mtoa_global_var_{}() { global {} ${}; return ${}; } _mtoa_global_var_{}()'.format(type, varName, type, varName, varName, varName))



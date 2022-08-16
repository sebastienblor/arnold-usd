import maya.mel
import maya.cmds as cmds
from mtoa.ui.ae.shaderTemplate import ShaderAETemplate
from mtoa.ui.ae.utils import AttrControlGrp
import re
import os

from arnold import *
from maya import OpenMaya as om

defaultFolder = ""


def to_camelcase(s):
    return re.sub(r'(?!^)_([a-zA-Z])', lambda m: m.group(1).upper(), s)


def createMayaAttributeForArnoldAttributes(mayaNodeName, mayaAttributeName, arnoldAttributeType, defaultValue, isOutputAttr):
    '''
    Create maya attributes for all Arnold-OSL attributes with their default values.
    We dont support AI_TYPE_ARRAY AI_TYPE_POINTER AI_TYPE_NODE AI_TYPE_MATRIX attribute types yet.
    '''

    ## Default values are for an input attribute
    write = True
    read = False

    if (isOutputAttr):
        write = False
        read = True

    if ('param_' in mayaAttributeName):
        mayaAttributeLongName = mayaAttributeName.replace('param_','')
    mayaAttributeShortName = to_camelcase(mayaAttributeName)

    if arnoldAttributeType == AI_TYPE_BYTE:
        if not cmds.attributeQuery(mayaAttributeLongName, node=mayaNodeName, ex=True):
            cmds.addAttr(mayaNodeName, sn=mayaAttributeShortName, longName=mayaAttributeLongName, at='byte', w=write, r=read, ct="oslAttribute")
    elif arnoldAttributeType == AI_TYPE_INT:
        if not cmds.attributeQuery(mayaAttributeLongName, node=mayaNodeName, ex=True):
            attrDefaultValue = defaultValue.contents.INT
            cmds.addAttr(mayaNodeName, sn=mayaAttributeShortName, longName=mayaAttributeLongName, at='long', dv=attrDefaultValue, w=write, r=read, ct="oslAttribute")
    elif arnoldAttributeType == AI_TYPE_UINT:
        if not cmds.attributeQuery(mayaAttributeLongName, node=mayaNodeName, ex=True):
            attrDefaultValue = defaultValue.contents.INT
            cmds.addAttr(mayaNodeName, sn=mayaAttributeShortName, longName=mayaAttributeLongName, at='long', dv=attrDefaultValue, w=write, r=read, ct="oslAttribute")
    elif arnoldAttributeType == AI_TYPE_BOOLEAN:
        if not cmds.attributeQuery(mayaAttributeLongName, node=mayaNodeName, ex=True):
            attrDefaultValue = defaultValue.contents.BOOL
            cmds.addAttr(mayaNodeName, sn=mayaAttributeShortName, longName=mayaAttributeLongName, at='bool', dv=attrDefaultValue, w=write, r=read, ct="oslAttribute")
    elif arnoldAttributeType == AI_TYPE_FLOAT:
        if not cmds.attributeQuery(mayaAttributeLongName, node=mayaNodeName, ex=True):
            attrDefaultValue = defaultValue.contents.FLT
            cmds.addAttr(mayaNodeName, sn=mayaAttributeShortName, longName=mayaAttributeLongName, at="float", dv=attrDefaultValue, w=write, r=read, ct="oslAttribute")
    elif arnoldAttributeType == AI_TYPE_RGB or arnoldAttributeType == AI_TYPE_RGBA or arnoldAttributeType == AI_TYPE_CLOSURE:
        if not cmds.attributeQuery(mayaAttributeLongName, node=mayaNodeName, ex=True):
            dv_x, dv_y, dv_z = (defaultValue.contents.RGB.r, defaultValue.contents.RGB.g, defaultValue.contents.RGB.b)
            cmds.addAttr(mayaNodeName, sn=mayaAttributeShortName, longName=mayaAttributeLongName, at="float3", uac=True, w=write, r=read, ct="oslAttribute")
            cmds.addAttr(mayaNodeName, sn=mayaAttributeShortName + 'R', longName=mayaAttributeLongName + 'R', at="float", parent=mayaAttributeLongName, dv=dv_x, w=write, r=read)
            cmds.addAttr(mayaNodeName, sn=mayaAttributeShortName + 'G', longName=mayaAttributeLongName + 'G', at="float", parent=mayaAttributeLongName, dv=dv_y, w=write, r=read)
            cmds.addAttr(mayaNodeName, sn=mayaAttributeShortName + 'B', longName=mayaAttributeLongName + 'B', at="float", parent=mayaAttributeLongName, dv=dv_z, w=write, r=read)
    elif arnoldAttributeType == AI_TYPE_VECTOR:
        if not cmds.attributeQuery(mayaAttributeLongName, node=mayaNodeName, ex=True):
            dv_x, dv_y, dv_z = (defaultValue.contents.VEC.x, defaultValue.contents.VEC.y, defaultValue.contents.VEC.z)
            cmds.addAttr(mayaNodeName, sn=mayaAttributeShortName, longName=mayaAttributeLongName, at="float3", w=write, r=read, ct="oslAttribute")
            cmds.addAttr(mayaNodeName, sn=mayaAttributeShortName + 'X', longName=mayaAttributeLongName + 'X', at="float", parent=mayaAttributeLongName, dv=dv_x, w=write, r=read)
            cmds.addAttr(mayaNodeName, sn=mayaAttributeShortName + 'Y', longName=mayaAttributeLongName + 'Y', at="float", parent=mayaAttributeLongName, dv=dv_y, w=write, r=read)
            cmds.addAttr(mayaNodeName, sn=mayaAttributeShortName + 'Z', longName=mayaAttributeLongName + 'Z', at="float", parent=mayaAttributeLongName, dv=dv_z, w=write, r=read)
    elif arnoldAttributeType == AI_TYPE_VECTOR2:
        if not cmds.attributeQuery(mayaAttributeLongName, node=mayaNodeName, ex=True):
            dv_x, dv_y, dv_z = (defaultValue.contents.VEC.x, defaultValue.contents.VEC.y, defaultValue.contents.VEC.z)
            cmds.addAttr(mayaNodeName, sn=mayaAttributeShortName, longName=mayaAttributeLongName, at="float2", w=write, r=read, ct="oslAttribute")
            cmds.addAttr(mayaNodeName, sn=mayaAttributeShortName + 'U', longName=mayaAttributeLongName + 'U', at="float", parent=mayaAttributeLongName, w=write, r=read)
            cmds.addAttr(mayaNodeName, sn=mayaAttributeShortName + 'V', longName=mayaAttributeLongName + 'V', at="float", parent=mayaAttributeLongName, w=write, r=read)
    elif arnoldAttributeType == AI_TYPE_STRING:
        if not cmds.attributeQuery(mayaAttributeLongName, node=mayaNodeName, ex=True):
            attrDefaultValue = defaultValue.contents.STR
            cmds.addAttr(mayaNodeName, sn=mayaAttributeShortName, longName=mayaAttributeLongName, dt="string", w=write, r=read, ct="oslAttribute")
            cmds.setAttr("%s.%s" % (mayaNodeName, mayaAttributeShortName), "%s" % attrDefaultValue, typ="string")
    elif arnoldAttributeType == AI_TYPE_ARRAY or arnoldAttributeType == AI_TYPE_POINTER or arnoldAttributeType == AI_TYPE_NODE or arnoldAttributeType == AI_TYPE_MATRIX:
        pass
    else:
        # Node is undefined
        pass


class OSLSceneModel():

    '''
        Creating a model, that contains the code , compiles it and creates a list of input and output attributes
        that can then be passed on to creating / modifying them.
    '''

    def __init__(self, shaderCode, mayaNodeName):
        self._shaderCode = shaderCode
        self._mayaNodeName = mayaNodeName
        self.compileState = False
        self.compilationErrors = []
        self.compilationWarnings = []
        self.inputAttributes = {}
        self.outputAttributes = {}
        self.__compileOSLCode__()
        if self.compileState:
            self.__modifyScene__()

    def __compileOSLCode__(self):

        '''
        Compile the OSL code in a separate Arnold Universe and create a list of input / output attribute
        We're not using the output attribute yet, as we dont have a way to get named output attributes from Arnold yet.
        '''

        isActive = AiUniverseIsActive()
        if not isActive:
            AiBegin()

        # create a universe dedicated to OSL node compilation
        # for parameter/output type introspection and error checking
        compilation_universe = AiUniverse()

        # create an osl node
        osl_node = AiNode(compilation_universe, "osl", "test_node")
        # set the osl code as the "code" node parameter value
        # this triggers the compilation and the morphing of the node
        # into a node with the correct node paramters and output
        AiNodeSetStr(osl_node, "code", self._shaderCode)

        # check for compilation errors
        # compilation errors are stored into a "compilation_errors" user parameter
        if AiNodeLookUpUserParameter(osl_node, "compilation_errors"):
            compilation_errors = AiNodeGetArray(osl_node, "compilation_errors")
            for i in range(AiArrayGetNumElements(compilation_errors)):
                comp_error = AiArrayGetStr(compilation_errors, i)
                if re.match(r'[<>\w]+:\d+: warning:', comp_error):
                    self.compilationWarnings.append(comp_error)
                else:
                    self.compilationErrors.append(comp_error)

        # if osl_node is not reused it is possible to only check if the parameter exists
        # otherwise if the node is reused to check multiple code snippets
        # the parameter will be reset
        if len(self.compilationErrors) == 0:
            self.compileState = True
            # get the morphed node entry
            osl_node_entry = AiNodeGetNodeEntry(osl_node)
            num_outputs = AiNodeEntryGetNumOutputs(osl_node_entry)
            if num_outputs > 0 :
                for i in range(num_outputs):
                    param = AiNodeEntryGetOutput(osl_node_entry,i)
                    paramName = AiParamGetName(param)
                    paramType = AiParamGetType(param)
                    paramDefault = AiParamGetDefault(param)
                    self.outputAttributes[paramName] = {}
                    self.outputAttributes[paramName]['paramName'] = paramName
                    self.outputAttributes[paramName]['paramType'] = paramType
                    self.outputAttributes[paramName]['paramDefaultValue'] = paramDefault
            else:
                self.outputAttributes['output'] = {}
                self.outputAttributes['output']['outputType'] = AiNodeEntryGetOutputType(osl_node_entry)
            it = AiNodeEntryGetParamIterator(osl_node_entry)
            while not AiParamIteratorFinished(it):
                pentry = AiParamIteratorGetNext(it)
                # OSL parameters start with "param_"
                if str(AiParamGetName(pentry)).startswith("param_"):
                    paramName = AiParamGetName(pentry)
                    self.inputAttributes[paramName] = {}
                    self.inputAttributes[paramName]['paramName'] = paramName
                    self.inputAttributes[paramName]['paramType'] = AiParamGetType(pentry)
                    self.inputAttributes[paramName]['paramDefaultValue'] = AiParamGetDefault(pentry)
        else:
            self.compileState = False
        # cleanup the node
        AiUniverseDestroy(compilation_universe)

    def __modifyScene__(self):

        '''
        Clean up old / unnecessary attributes and create new ones in Maya
        '''

        if self.compileState:
            nodeAttrs = cmds.listAttr(self._mayaNodeName, userDefined=True, write=True, ct="oslAttribute", sn=True)
            if nodeAttrs:
                for attr in nodeAttrs:
                    if attr in self.inputAttributes.keys() or attr in self.outputAttributes.keys():
                        pass
                    else:
                        cmds.deleteAttr(self._mayaNodeName + '.' + attr)

            for key in self.inputAttributes.keys():
                createMayaAttributeForArnoldAttributes(self._mayaNodeName,  # The Maya node to which attributes need to be added
                                                       self.inputAttributes[key]['paramName'],  # The name of attribute
                                                       self.inputAttributes[key]['paramType'],  # The type of the attribute
                                                       self.inputAttributes[key]['paramDefaultValue'],  # The default value
                                                       False ## Is not an output attribute
                                                       )

            for key in self.outputAttributes.keys():
                createMayaAttributeForArnoldAttributes(self._mayaNodeName,  # The Maya node to which attributes need to be added
                                                       self.outputAttributes[key]['paramName'],  # The name of attribute
                                                       self.outputAttributes[key]['paramType'],  # The type of the attribute
                                                       self.outputAttributes[key]['paramDefaultValue'],  # The default value
                                                       True ## Is an output attribute
                                                       )

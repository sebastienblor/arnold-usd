import os
import re
import maya.cmds as cmds
import mtoa.melUtils as mu
from mtoa.ui.qt import BaseTransverser
from alembic import Abc, AbcGeom
from arnold import *

(ABC_PATH,
 ABC_NAME,
 ABC_PARENT,
 ABC_VISIBILITY,
 ABC_INSTANCEPATH,
 ABC_ENTIY_TYPE,
 ABC_IOBJECT) = range(7)

VISIBILITY = ['differed', 'hidden', 'visible']

SELECTION_REGEX = re.compile(r"(/[/\w]*\w+)/*\**")

EXP_REGEX = re.compile(r"""(?P<param>\w+)\s* # parameter
                         (?P<op>=|\+=|-=|\*=)\s* # operation
                         (?P<value>.*) # value
                         """, re.X)


def ArnoldUniverseOnlyBegin():
    if not AiUniverseIsActive():
        AiBegin()
        AiMsgSetConsoleFlags(AI_LOG_NONE)
        return True
    return False


def ArnoldUniverseEnd():
    if AiUniverseIsActive():
        if AiRendering():
            AiRenderInterrupt()
        if AiRendering():
            AiRenderAbort()
        AiEnd()


def abcToArnType(iObj):
    if not iObj:
        return

    md = iObj.getMetaData()
    if AbcGeom.IPolyMesh.matches(md) or AbcGeom.ISubD.matches(md):
        return 'polymesh'
    elif AbcGeom.IPoints.matches(md):
        return 'points'
    elif AbcGeom.ICurves.matches(md):
        return 'curves'
    elif AbcGeom.IXform.matches(md):
        return 'xform'
    else:
        return None


class AlembicTransverser(BaseTransverser):
    """ Alembic Transverser class """

    __instance = None

    def __new__(cls, *args, **kwargs):

        if not cls.__instance:
            instance = super(AlembicTransverser, cls).__new__(cls, *args, **kwargs)

            instance.impl = AlembicTransverserImpl()

            cls.__instance = instance

        return cls.__instance

    def __init__(self):
        super(AlembicTransverser, self).__init__(self)

    def getArchive(self, abc_file):

        return self.impl.getArchive(abc_file)

    def visitObject(self, iObj, parent="", visibility="visible"):

        return self.impl.visitObject(iObj)

    def getObjectInfo(self, iObj):

        return self.impl.getObjectInfo(iObj)

    def getParams(self, node_types):

        return self.impl.getParams(node_types)

    def getNodeTypes(self, iObj):

        return self.impl.getNodeTypes(iObj)

    def dir(self, *args):

        return self.impl.dir(*args)

    # def properties(self, node, path):

    #     return self.impl.properties(node, path)

    def createOperator(self, node, iobject):

        return self.impl.createOperator(node, iobject)

    def getOperator(self, node, path):

        return self.impl.getOperator(node, path)

    def getOverrides(self, node, path):

        return self.impl.getOverrides(node, path)

    def setOverride(self, node, path, param, op, value, param_type, is_array=False, index=-1):

        return self.impl.setOverride(node, path, param, op, value, param_type, is_array, index)

    def deleteOverride(self, node, path, index):

        return self.impl.deleteOverride(node, path, index)


class AlembicTransverserImpl(object):

    def getArchivePath(self, node):

        return os.path.abspath(cmds.getAttr("{}.cacheFileName".format(node)))

    def getArchive(self, node):

        abc_file = self.getArchivePath(node)
        iarch = Abc.IArchive(str(abc_file))
        return iarch

    def visitObject(self, iObj, parent="", visibility="visible"):

        abc_items = []
        obj_data = self.getObjectInfo(iObj)

        abc_items.append(obj_data)

        for child in iObj.children:
            abc_items += self.visitObject(child, obj_data[ABC_PATH], obj_data[ABC_VISIBILITY])

        return abc_items

    def getObjectInfo(self, iobject):
        path = iobject.getFullName()
        name = iobject.getName()
        parent = iobject.getParent().getFullName()
        instancedPath = iobject.instanceSourcePath()

        entity_type = abcToArnType(iobject)

        visibility = VISIBILITY[int(AbcGeom.GetVisibility(iobject))+1]

        return [path, name, parent, visibility, instancedPath, entity_type, iobject]

    def _getDefaultValue(self, param, param_type):

        AiUniverseCreated = ArnoldUniverseOnlyBegin()
        AiMsgSetConsoleFlags(AI_LOG_NONE)

        value = None
        param_default = AiParamGetDefault(param)
        if param_type is AI_TYPE_INT:
            value = param_default.contents.INT
        elif param_type is AI_TYPE_UINT:
            value = param_default.contents.UINT
        elif param_type is AI_TYPE_BYTE:
            value = param_default.contents.BYTE
        elif param_type is AI_TYPE_FLOAT:
            value = param_default.contents.FLT
        elif param_type is AI_TYPE_BOOLEAN:
            value = param_default.contents.BOOL
        elif param_type is AI_TYPE_STRING:
            value = str(param_default.contents.STR)
        elif param_type is AI_TYPE_ENUM:
            idx = param_default.contents.INT
            value = AiEnumGetString(AiParamGetEnum(param), idx)

        if AiUniverseCreated:
            ArnoldUniverseEnd()

        return value

    def getParams(self, node_types):

        paramDict = {}

        AiUniverseCreated = ArnoldUniverseOnlyBegin()
        AiMsgSetConsoleFlags(AI_LOG_NONE)

        for nodeType in node_types:
            if nodeType:
                nodeEntry = AiNodeEntryLookUp(nodeType)
                paramIter = AiNodeEntryGetParamIterator(nodeEntry)
                while not AiParamIteratorFinished(paramIter):
                    param = AiParamIteratorGetNext(paramIter)
                    param_type = AiParamGetType(param)
                    if param_type == AI_TYPE_ARRAY:
                        array_default = AiParamGetDefault(param).contents.ARRAY
                        param_type = AiArrayGetType(array_default)

                    enum_values = []

                    if param_type == AI_TYPE_ENUM:
                        enumParam = AiParamGetEnum(param)
                        i = 0
                        t = True
                        while t is not None:
                            t = AiEnumGetString(enumParam, i)
                            if t:
                                enum_values.append(t)
                            i += 1

                    default_value = self._getDefaultValue(param, param_type)

                    paramName = AiParamGetName(param)
                    if paramName not in paramDict:
                        paramDict[paramName] = (nodeType, param_type, default_value, param_type == AI_TYPE_ARRAY, enum_values)
                    else:
                        paramDict[paramName] = ("common", param_type, default_value, param_type == AI_TYPE_ARRAY, enum_values)

                AiParamIteratorDestroy(paramIter)

        if AiUniverseCreated:
            ArnoldUniverseEnd()

        return paramDict

    def getNodeTypes(self, iObj):

        node_types = []
        children = self.visitObject(iObj)

        for child in children:
            if child[ABC_ENTIY_TYPE] not in node_types:
                node_types.append(child[ABC_ENTIY_TYPE])

        return node_types

    def dir(self, iobject):

        children = []
        for ich in iobject.children:
            children.append(self.getObjectInfo(ich))

        return children

    def properties(self, node, path):

        pass

    def createOperator(self, node, iobject):
        num_ops = mu.getAttrNumElements(node, 'operators')
        op_name = '{}_setParam{}'.format(node, num_ops)
        op = cmds.createNode("aiSetParameter", name=op_name, ss=True)
        path = iobject[ABC_PATH]
        if iobject[ABC_ENTIY_TYPE] == "xform":
            path += "/*"
        elif iobject[ABC_ENTIY_TYPE] == None:
            path += "*"
        cmds.setAttr(op + ".selection",
                     path,
                     type="string")

        newItem = '{}.operators[{}]'.format(node, num_ops)
        cmds.connectAttr(op + ".out", newItem)
        return op

    def getOperator(self, node, path):

        def walkInputs(op, path, plug):

            selection_exp = '{}*'.format(path)

            r_ipt = r_plug = None
            if cmds.attributeQuery('selection', node=op, exists=True):
                sel_exp = cmds.getAttr('{}.selection'.format(op))
                tokens = sel_exp.rsplit()
                for tok in tokens:
                    mat = SELECTION_REGEX.match(tok)
                    if mat and mat.group(1) == path or \
                       (path == "/" and tok == "/*"):
                        return op, plug

            if cmds.attributeQuery('inputs', node=op, exists=True):
                inputs_raw = cmds.listConnections("{}.inputs".format(op), c=True) or []
                it = iter(inputs_raw)
                inputs = zip(it, it)
                for plug, ipt in inputs:
                    r_ipt, r_plug = walkInputs(ipt, path, plug)

            return r_ipt, r_plug

        operator = None
        operators = cmds.listConnections('{}.operators'.format(node)) or []
        for idx, op in enumerate(operators):
            out_op, plug = walkInputs(op, path, '{}.operators[{}]'.format(node, idx))
            if out_op:
                operator = out_op
                break

        return operator

    def deleteOperator(self, op):
        pass

    def getOverrides(self, node, path):

        op = self.getOperator(node, path)
        print "getOverrides", op, node, path

        overrides = []
        if op:
            for c in cmds.getAttr('{}.assignment'.format(op), multiIndices=True) or []:
                ass_str = cmds.getAttr("{}.assignment[{}]".format(op, c))
                mat = EXP_REGEX.match(ass_str)
                data = list(mat.groups())
                data.append(c)
                overrides.append(data)  # split to param, op, value

        return overrides

    def setOverride(self, node, path, param, op, value, param_type, array=False, index=-1):

        op = self.getOperator(node, path)
        if not op:
            return False

        if index == -1:
            n_conn = mu.getAttrNumElements(op, "assignment")
            index = n_conn
            if param != 'NEWOVERRIDE':
                for c in cmds.getAttr('{}.assignment'.format(op), multiIndices=True) or []:
                    ass_str = cmds.getAttr("{}.assignment[{}]".format(op, c))
                    if ass_str.startswith(param):
                        index = c
                        break

        if param_type in [AI_TYPE_ENUM, AI_TYPE_STRING, AI_TYPE_POINTER, AI_TYPE_NODE]:
            value = "'{}'".format(value)

        param_exp = "{}={}".format(param, value)
        cmds.setAttr("{}.assignment[{}]".format(op, index),
                     param_exp,
                     type="string")
        return True

        return False

    def _indexInAssignment(self, index, op):
        indices = cmds.getAttr('{}.assignment'.format(op), multiIndices=True) or []
        if index in indices:
            return True
        return False

    def deleteOverride(self, node, path, index):
        op = self.getOperator(node, path)
        print "AlembicTransverserImpl.deleteOverride", node, path, index
        if index != -1 and op:
            indices = cmds.getAttr('{}.assignment'.format(op), multiIndices=True) or []
            if self._indexInAssignment(index, op):
                cmds.removeMultiInstance('{}.assignment[{}]'.format(op, index))
            else:
                return False
            # Final check the the index was removed
            if not self._indexInAssignment(index, op):
                return True
        return False

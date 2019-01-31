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

(PARM, OP, VALUE, INDEX) = range(4)

VISIBILITY = ['differed', 'hidden', 'visible']

SELECTION_REGEX = re.compile(r"(/[/\w]*\w+)/*\**")

EXP_REGEX = re.compile(r"""(?P<param>\w+)\s* # parameter
                         (?P<op>=|\+=|-=|\*=)\s* # operation
                         (?P<value>.*) # value
                         """, re.X)

OVERRIDE_OP = "aiSetParameter"
DISABLE_OP = "aiDisable"


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


# This part belongs in the ABC code
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

    def deleteOperator(self, node, path, operator_type):

        return self.impl.deleteOperator(node, path, operator_type)

    def createOperator(self, node, item, operator_type):

        return self.impl.createOperator(node, item, operator_type)

    def getOperators(self, node, path, operator_type=None):

        return self.impl.getOperators(node, path, operator_type)

    def getOverrides(self, node, path, override_type=None):

        return self.impl.getOverrides(node, path, override_type)

    def setOverride(self, node, path, param, operation, value, param_type, is_array=False, index=-1):

        return self.impl.setOverride(node, path, param, operation, value, param_type, is_array, index)

    def deleteOverride(self, node, path, index):

        return self.impl.deleteOverride(node, path, index)
    def showOperatorItems(self):
        return self.impl.showOperatorItems()


class AlembicTransverserImpl(object):

    # We must extract the alembic-specific part from the common one that will be used for all procedural
    # So far it looks like getArcivePath, getArchive, visitObject, getObjectInfo, are alembic-specific,
    # while the rest is more generic

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
        """
        Get all the parameters that can be overidden for the given node types
        """
        # FIXME cache this so we don't call it everytime the widget is created
        # FIXME blacklist parameters that we don't want to expose
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

    def getOperatorIndices(self, node):
        return cmds.getAttr('{}.operators'.format(node), multiIndices=True) or []

    def getConnectedOperator(self, node, index):
        return cmds.connectionInfo('{}.operators[{}]'.format(node, index), sourceFromDestination=True)

    def getOperatorIndex(self, node, operator):

        index = -1

        for idx in self.getOperatorIndices(node):
            src = self.getConnectedOperator(node, idx)
            if src and src.split('.')[0] == operator:
                index = idx

        return index

    def createOperator(self, node, item, operator_type):
        num_ops = mu.getAttrNumElements(node, 'operators')
        # get the index that this operator should come in the list
        data = item.data
        # get parent index
        index = 0
        parent_index = self.getOperatorIndex(node, item.getOverridesOp(True))
        if parent_index > -1:
            index = parent_index + 1
        print "AlembicTransverserImpl.createOperator", node, parent_index, index

        op_name = '{}_setParam{}'.format(node, num_ops)
        op = cmds.createNode(operator_type, name=op_name, ss=True)
        if op:
            # if this operator has a selection attribute set it to the
            # given object path
            if cmds.attributeQuery('selection', node=op, exists=True):
                path = data[ABC_PATH]
                if data[ABC_ENTIY_TYPE] == "xform":
                    path += "/*"
                elif data[ABC_ENTIY_TYPE] == None:
                    path += "*"
                cmds.setAttr(op + ".selection",
                             path,
                             type="string")

            self.insertOperator(node, op, index)
            item.addOverrideOp(op)
            return op
        return None

    def insertOperator(self, node, op, index):

        # check if index has connection, if not do a straight connection at given index
        dest = '{}.operators[{}]'.format(node, index)
        src = '{}.out'.format(op)
        if not cmds.connectionInfo(dest, isDestination=True):
            cmds.connectAttr(src, dest)
        else:
            # move other connections along starting at the end
            for idx in reversed(self.getOperatorIndices(node)):
                if idx >= index:
                    idx_src = self.getConnectedOperator(node, idx)
                    cmds.disconnectAttr(idx_src, '{}.operators[{}]'.format(node, idx))
                    cmds.connectAttr(idx_src, '{}.operators[{}]'.format(node, idx+1))
            # connect at the index given
            cmds.connectAttr(src, '{}.operators[{}]'.format(node, index))

    def getOperators(self, node, path, operator_type=None):

        def walkInputs(op, path, plug):
            """
            walk the inputs of the given plug and
            return list of operators matching the path
            """

            selection_exp = '{}*'.format(path)
            ops = []
            r_ipt = r_plug = None
            if cmds.attributeQuery('selection', node=op, exists=True) and \
               (operator_type is None or cmds.nodeType(op) == operator_type):

                sel_exp = cmds.getAttr('{}.selection'.format(op))
                tokens = sel_exp.rsplit()
                for tok in tokens:
                    mat = SELECTION_REGEX.match(tok)
                    if mat and mat.group(1) == path or \
                       (path == "/" and tok == "/*"):
                        ops.append(op)

            if cmds.attributeQuery('inputs', node=op, exists=True):
                inputs_raw = cmds.listConnections("{}.inputs".format(op), c=True) or []
                it = iter(inputs_raw)
                inputs = zip(it, it)
                for plug, ipt in inputs:
                    ops += walkInputs(ipt, path, plug)

            return ops

        # Start the query
        operators = []
        con_operators = cmds.listConnections('{}.operators'.format(node)) or []
        for idx, op in enumerate(con_operators):
            out_op = walkInputs(op, path, '{}.operators[{}]'.format(node, idx))
            if out_op:
                operators += out_op

        return operators

    def deleteOperator(self, node, path, operator_type):

        op = self.getOperators(node, path, OVERRIDE_OP)
        if len(op) == 1:
            # for now only delete if there is only one op that has this path
            cmds.delete(op[0])

    def getOverrides(self, node, path, override_type=None):

        ops = self.getOperators(node, path, OVERRIDE_OP)

        overrides = []
        if len(ops):
            for op in ops:
                for c in cmds.getAttr('{}.assignment'.format(op), multiIndices=True) or []:
                    ass_str = cmds.getAttr("{}.assignment[{}]".format(op, c))
                    mat = EXP_REGEX.match(ass_str)
                    data = list(mat.groups())
                    if override_type is None or \
                       (override_type and data[0] == override_type):
                        data.append(c)
                    overrides.append(data)  # split to param, op, value

        return overrides

    def setOverride(self, node, path, param, operation, value, param_type, array=False, index=-1):

        ops = self.getOperators(node, path, OVERRIDE_OP)
        if not len(ops):
            return False
        # for now only add the assignment to the first matching op
        op = ops[0]
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

        param_exp = "{}{}{}".format(param, operation, value)
        cmds.setAttr("{}.assignment[{}]".format(op, index),
                     param_exp,
                     type="string")
        return True

    def _getOverrideIndices(self, op):
        if cmds.nodeType(op) == OVERRIDE_OP:
            return cmds.getAttr('{}.assignment'.format(op), multiIndices=True) or []
        else:
            raise NodeTypeError("Given operator is not of type {}".format(OVERRIDE_OP))

    def _indexInAssignment(self, index, op):
        indices = self._getOverrideIndices(op)
        if index in indices:
            return True
        return False

    def deleteOverride(self, node, path, index):
        ops = self.getOperators(node, path, OVERRIDE_OP)
        if index != -1 and len(ops):
            op = ops[0]
            if self._indexInAssignment(index, op):
                cmds.removeMultiInstance('{}.assignment[{}]'.format(op, index))
            else:
                return False
            # Final check the the index was removed
            if not self._indexInAssignment(index, op):
                return True
        return False

    # Do we want to see a line for each operator in the hierarchy. For now it's disabled
    def showOperatorItems(self):
        return False

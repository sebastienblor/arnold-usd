import os
import re
import maya.cmds as cmds
import mtoa.melUtils as mu
from mtoa.ui.qt import BaseTransverser
from alembic import Abc, AbcGeom
from arnold import *

(PROC_PATH,
 PROC_NAME,
 PROC_PARENT,
 PROC_VISIBILITY,
 PROC_INSTANCEPATH,
 PROC_ENTRY_TYPE,
 PROC_IOBJECT) = range(7)

(PARM, OP, VALUE, INDEX) = range(4)


SELECTION_REGEX = re.compile(r"(/[/\w]*\w+)/*\**")

EXP_REGEX = re.compile(r"""(?P<param>\w+)\s* # parameter
                         (?P<op>=|\+=|-=|\*=)\s* # operation
                         (?P<value>.*) # value
                         """, re.X)

OVERRIDE_OP = "aiSetParameter"
DISABLE_OP = "aiDisable"

NODE_TYPES = ['polymesh', 'curves', 'nurbs', 'points']

PARAM_BLACKLIST = ['id', 'visibility', 'name', 'matrix',
                   'motion_start', 'motion_end', 'shader', 'disp_map',
                   'vidxs', 'vlist', 'nsides', 'uvidxs', 'shidxs',
                   'nlist', 'uvlist', 'crease_idxs', 'crease_sharpness',
                   'use_shadow_group', 'use_light_group', 'degree_u',
                   'degree_v', 'transform_type',
                   'num_points', 'points', 'orientations',
                   'uvs', 'cvs', 'knots_u', 'knots_v', 'degree_u', 'degree_v']


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


class ProceduralTransverser(BaseTransverser):
    """ Procedural Transverser class """

    def __init__(self):
        super(ProceduralTransverser, self).__init__()
        self.selectionAttr = None  # eventually a string attribute to be updated when the selection changes
        self.paramDict = {}
        self.selectionStr = ''

    def getParams(self, node_types):
        """
        Get all the parameters that can be overidden for the given node types
        """
        if not node_types:
            return {}

        self.paramDict['hidden'] = {'shader': (AI_TYPE_NODE, None, False, []),
                                    'disp_map': (AI_TYPE_NODE, None, False, []),}

        self.paramDict['common'] = {"visibility": (AI_TYPE_BYTE, 255, False, []),
                                    "sidedness": (AI_TYPE_BYTE, 255, False, []),
                                    "recieve_shadows": (AI_TYPE_BOOLEAN, True, False, []),
                                    "opaque": (AI_TYPE_BOOLEAN, True, False, []),
                                    "matte": (AI_TYPE_BOOLEAN, True, False, []),
                                    "self_shadows": (AI_TYPE_BOOLEAN, True, False, [])}

        for nodeType in node_types:
            if nodeType and nodeType not in self.paramDict:
                self.paramDict[nodeType] = {}
                AiUniverseCreated = ArnoldUniverseOnlyBegin()
                AiMsgSetConsoleFlags(AI_LOG_NONE)

                nodeEntry = AiNodeEntryLookUp(nodeType)
                paramIter = AiNodeEntryGetParamIterator(nodeEntry)
                while not AiParamIteratorFinished(paramIter):
                    param = AiParamIteratorGetNext(paramIter)
                    param_type = AiParamGetType(param)
                    paramName = AiParamGetName(param)

                    # skip parameters in the blacklist
                    if paramName in PARAM_BLACKLIST:
                        continue

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

                    if paramName not in self.paramDict[nodeType].keys() + self.paramDict['common'].keys():
                        is_array = param_type == AI_TYPE_ARRAY
                        self.paramDict[nodeType][paramName] = (param_type,
                                                               default_value,
                                                               is_array,
                                                               enum_values)

                AiParamIteratorDestroy(paramIter)

                if AiUniverseCreated:
                    ArnoldUniverseEnd()

        return self.paramDict

    def getNodeTypes(self, iObj):
        if not iObj:
            return None

        objectInfo = self.getObjectInfo(iObj)
        if not objectInfo:
            return None

        return [objectInfo[PROC_ENTRY_TYPE]]

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

        op_name = '{}_{}'.format(node, operator_type)
        op = cmds.createNode(operator_type, name=op_name, ss=True)
        if op:
            # if this operator has a selection attribute set it to the
            # given object path
            if cmds.attributeQuery('selection', node=op, exists=True):
                path = data[PROC_PATH]
                if data[PROC_ENTRY_TYPE] == "xform":
                    path += "/*"
                elif data[PROC_ENTRY_TYPE] == None:
                    path += "*"
                cmds.setAttr(op + ".selection",
                             path,
                             type="string")

            self.insertOperator(node, op, index)
            item.addOverrideOp(op)
            return op
        return None

    def selectNode(self, obj):

        if not obj or not cmds.objExists(obj):
            # It happens when the object is in the Alembic file, not in the
            # scene.
            cmds.warning(
                "Can't select {}. This object is not in the Maya scene.".format(obj))
            return

        cmds.select(obj)

    def toggleOperator(self, operator):

        if not operator or not cmds.objExists(operator):
            # It happens when the object is in the Alembic file, not in the
            # scene.
            cmds.warning(
                "Can't select {}. This object is not in the Maya scene.".format(obj))
            return

        attrname = operator+".enable"
        cmds.setAttr(attrname, not cmds.getAttr(attrname))

    # this function is invoked when we select an item in the tree view
    def selectionChanged(self, node, selection):
        if not self.selectionAttr:
            return
        selectionStr = ''
        for sel in selection:
            if len(selectionStr):
                selectionStr += ','

            if sel[PROC_PARENT] == None and len(selection) == 1:
                selectionStr = '' 
                break
            selectionStr += sel[PROC_PATH]
            if sel[PROC_ENTRY_TYPE] =='xform':
                selectionStr += '/*'
        if selectionStr == self.selectionStr:
            return
        self.selectionStr = selectionStr
        cmds.setAttr('{}.{}'.format(node, self.selectionAttr), selectionStr, type='string')

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

    @classmethod
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

    @classmethod
    def setOverride(cls, node, path, param, operation, value, param_type, array=False, index=-1):

        ops = cls.getOperators(node, path, OVERRIDE_OP)
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

        if value is None:
            value = ''

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

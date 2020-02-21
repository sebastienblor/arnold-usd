import os
import re
import fnmatch
import maya.cmds as cmds
import mtoa.melUtils as mu
from mtoa.ui.qt import BaseTransverser, valueIsExpression
from mtoa.ui.qt.widgets import TYPES_DICT_STRINGS
from arnold import *

(PROC_PATH,
 PROC_NAME,
 PROC_PARENT,
 PROC_VISIBILITY,
 PROC_INSTANCEPATH,
 PROC_ENTRY,
 PROC_IOBJECT,
 PROC_ENTRY_TYPE,
 PROC_NUM_CHILDREN) = range(9)

(PARAM_TYPE, PARAM, OP, VALUE, INDEX, OPERATOR, ENABLED) = range(7)
(DATA_PARAM_TYPE, DATA_DEFAULT_VALUE, DATA_IS_ARRAY, DATA_ENUM_VALUES) = range(4)

SELECTION_REGEX = re.compile(r'.*(?=/\*)')

EXP_REGEX = re.compile(r"""(?:(?P<type>bool|byte|int|uint|float|rgb|rgba|vector|vector2|string|matrix|node)\s+)? # parameter
                         (?P<param>[\w.:\[\]]+)\s* # parameter
                         (?P<op>=|\+=|-=|\*=)\s* # operation
                         (?P<value>.*) # value
                         """, re.X)

OVERRIDE_OP = "aiSetParameter"
DISABLE_OP = "aiDisable"
COLLECTION_OP = "aiCollection"
MERGE_OP = "aiMerge"
SWITCH_OP = "aiSwitchOperator"
INCLUDEGRAPH_OP = "aiIncludeGraph"
MATERIALX_OP = "aiMaterialx"
LOOKSWITCH_OP = "aiLookSwitch"

PROCEDURAL_NODES = ["procedural", "alembic", "usd"]

SELECTION_OPS = [OVERRIDE_OP, DISABLE_OP, COLLECTION_OP]

NODE_TYPES = ['polymesh', 'curves', 'nurbs', 'points']

PARAM_BLACKLIST = ['id', 'visibility', 'name', 'matrix',
                   'motion_start', 'motion_end', 'shader', 'disp_map',
                   'vidxs', 'vlist', 'nsides', 'uvidxs', 'shidxs',
                   'nlist', 'nidxs', 'uvlist', 'crease_idxs',
                   'crease_sharpness', 'use_shadow_group', 'use_light_group',
                   'degree_u', 'degree_v', 'transform_type',
                   'num_points', 'points', 'orientations',
                   'uvs', 'cvs', 'knots_u', 'knots_v', 'degree_u', 'degree_v']

DISP_MAP = 'disp_map'
SHADER = 'shader'

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
                        while t:
                            try:
                                t = AiEnumGetString(enumParam, i)
                                if t:
                                    enum_values.append(t)
                            except UnicodeDecodeError as e:
                                print("{}.{}[{}] '{}'".format(nodeType, paramName, i, t))
                                raise e
                            i += 1

                    default_value = self._getDefaultValue(param, param_type)

                    if paramName not in list(self.paramDict[nodeType].keys()) + list(self.paramDict['common'].keys()):
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

        return [objectInfo[PROC_ENTRY]]

    def properties(self, node, path):
        pass

    def getOperatorIndices(self, node):
        con_attr = 'operators'
        if cmds.nodeType(node) == MERGE_OP:
            con_attr = 'inputs'
        elif cmds.nodeType(node) == LOOKSWITCH_OP:
            var_index = cmds.getAttr('{}.index'.format(node))
            con_attr = 'looks[{}].inputs'.format(var_index)
        return cmds.getAttr('{}.{}'.format(node, con_attr), multiIndices=True) or []

    def getConnectedOperator(self, node, index):
        con_attr = 'operators'
        if cmds.nodeType(node) == MERGE_OP:
            con_attr = 'inputs'
        elif cmds.nodeType(node) == LOOKSWITCH_OP:
            var_index = cmds.getAttr('{}.index'.format(node))
            con_attr = 'looks[{}].inputs'.format(var_index)
        return cmds.connectionInfo('{}.{}[{}]'.format(node, con_attr, index), sourceFromDestination=True)

    def getInputs(self, operator, transverse=True):

        def walkInputs(op, transverse=True):
            """
            return list of operators connected to the inputs
            """

            ops = []
            ops.append(op)

            if cmds.attributeQuery('inputs', node=op, exists=True):
                node_type = cmds.nodeType(op)
                if node_type == SWITCH_OP:
                    switch_index = cmds.getAttr("{}.index".format(op))
                    inputs_raw = cmds.listConnections("{}.inputs[{}]".format(op, switch_index), c=True) or []
                elif node_type == LOOKSWITCH_OP:
                    switch_index = cmds.getAttr("{}.index".format(op))
                    inputs_raw = cmds.listConnections("{}.looks[{}].inputs".format(op, switch_index), c=True) or []
                else:
                    inputs_raw = cmds.listConnections("{}.inputs".format(op), c=True) or []
                it = iter(inputs_raw)
                inputs = zip(it, it)
                if transverse:
                    for plug, ipt in inputs:
                        ops += walkInputs(ipt)

            return ops

        if not cmds.objExists(operator) or not operator:
            return []

        operators = []
        out_op = walkInputs(operator, transverse)
        for op in out_op:
            if op not in operators:
                operators.append(op)

        return operators

    def getOperatorIndex(self, node, operator):

        index = -1

        for idx in self.getOperatorIndices(node):
            src = self.getConnectedOperator(node, idx)
            op_list = self.getInputs(src.split('.')[0])
            if operator in op_list:
                index = idx

        return index

    def getVariantNode(self, node):
        look_node = False
        ops = cmds.listConnections('{}.operators'.format(node), plugs=True)
        for op in ops or []:
            op_node, plug = op.split('.')
            if cmds.nodeType(op_node) == LOOKSWITCH_OP:
                look_node = op_node

        return look_node

    def createOperator(self, node, item, operator_type):

        # get if we have a look merge connected
        look_node = self.getVariantNode(node)
        if look_node:
            # get current index
            node = look_node

        # get the index that this operator should come in the list
        data = item.data
        # get parent index
        index = 0
        op_idxs = self.getOperatorIndices(node)
        for idx in op_idxs:
            op = self.getConnectedOperator(node, idx)
            if op and cmds.nodeType(self.getConnectedOperator(node, idx)) != OVERRIDE_OP:
                index = idx+1

        parent_op = item.getOverridesOp(True)
        if not type(parent_op) == list:
            parent_op = [parent_op]

        parent_index = -1
        for p_op in parent_op:
            p_idx = self.getOperatorIndex(node, p_op)
            if p_idx > parent_index:
                parent_index = p_idx

        if parent_index > -1:
            index = parent_index + 1

        op_name = '{}_{}#'.format(data[PROC_NAME], operator_type)
        op = cmds.createNode(operator_type, name=op_name, ss=True)
        if op:
            # if this operator has a selection attribute set it to the
            # given object path
            if cmds.attributeQuery('selection', node=op, exists=True):
                path = data[PROC_PATH]
                if data[PROC_ENTRY] == "xform":
                    path += "/*"
                elif data[PROC_ENTRY] in PROCEDURAL_NODES:
                    path = data[PROC_PARENT] + "*"
                cmds.setAttr(op + ".selection",
                             path,
                             type="string")

            self.insertOperator(node, op, index)
            if operator_type == OVERRIDE_OP:
                item.setOverrideOp(op)
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
            if sel[PROC_ENTRY] =='xform':
                selectionStr += '/*'
        if selectionStr == self.selectionStr:
            return
        self.selectionStr = selectionStr
        cmds.setAttr('{}.{}'.format(node, self.selectionAttr), selectionStr, type='string')

    def insertOperator(self, node, op, index):

        con_attr = 'operators'
        if cmds.nodeType(node) == MERGE_OP:
            con_attr = 'inputs'
        elif cmds.nodeType(node) == LOOKSWITCH_OP:
            var_index = cmds.getAttr('{}.index'.format(node))
            con_attr = 'looks[{}].inputs'.format(var_index)
        # check if index has connection, if not do a straight connection at given index
        dest = '{}.{}[{}]'.format(node, con_attr, index)
        src = '{}.out'.format(op)
        if not cmds.connectionInfo(dest, isDestination=True):
            cmds.connectAttr(src, dest)
        else:
            # move other connections along starting at the end
            for idx in reversed(self.getOperatorIndices(node)):
                if idx >= index:
                    idx_src = self.getConnectedOperator(node, idx)
                    cmds.disconnectAttr(idx_src, '{}.{}[{}]'.format(node, con_attr, idx))
                    cmds.connectAttr(idx_src, '{}.{}[{}]'.format(node, con_attr, idx+1))
            # connect at the index given
            cmds.connectAttr(src, '{}.{}[{}]'.format(node, con_attr, index))

    def getCustomParamName(self, operator):
        if cmds.nodeType(operator) == OVERRIDE_OP:
            customParam = "myParam"
            c = 0
            for a in cmds.getAttr('{}.assignment'.format(operator), multiIndices=True) or []:
                attr = cmds.getAttr('{}.assignment[{}]'.format(operator, a))
                mat = EXP_REGEX.match(attr)
                if mat:
                    param_name = mat.group('param')
                    if param_name.startswith(customParam):
                        c += 1
            if c > 0:
                customParam = '{}{}'.format(customParam, c)
            return customParam
        return

    @classmethod
    def operatorAffectsPath(self, path, operator, operator_type=None, collections=[]):

        sel_mat = False
        exact_match = False
        if cmds.attributeQuery('selection', node=operator, exists=True) and \
           (operator_type is None or cmds.nodeType(operator) == operator_type):
            sel_exp = cmds.getAttr('{}.selection'.format(operator))
            tokens = sel_exp.rsplit()
            for tok in tokens:
                mat = SELECTION_REGEX.match(tok)
                if mat and mat.group() == path or tok == path:
                    exact_match = True
                    sel_mat = True
                    break

                if tok[1:] in collections:
                    sel_mat = True
                    break
                if (tok == "*" and path == '/') or \
                   (tok == path):
                    exact_match = True
                    sel_mat = True
                    break

                pat = fnmatch.translate(tok)
                if re.match(pat, path):
                    sel_mat = True
                    break
        elif (operator_type is None or cmds.nodeType(operator) == operator_type):
            sel_mat = True
            exact_match = False

        return sel_mat, exact_match

    @classmethod
    def getCollections(self, node, path):

        col_ops = self.getOperators(node, path, COLLECTION_OP)
        collections = []
        local_collections = []
        for op, match in col_ops:

            if match:
                local_collections.append(cmds.getAttr('{}.collection'.format(op)))
            else:
                collections.append(cmds.getAttr('{}.collection'.format(op)))
        return collections, local_collections

    @classmethod
    def getOperators(self, node, path='', operator_type=None, collections=[], index=-1, gather_parents=False):

        def walkInputs(op, path, plug, collections, gather_parents=False, parent_ops=[]):
            """
            walk the inputs of the given plug and
            return list of operators matching the path
            """
            ops = []
            op_type = cmds.nodeType(op)
            sel_mat, exact_match = self.operatorAffectsPath(path, op, operator_type, collections)
            if sel_mat and op:
                for p_op, p_exact_match in parent_ops:
                    if p_op not in ops and \
                       (operator_type is None or cmds.nodeType(p_op) == operator_type):
                        ops.append((p_op, p_exact_match))
                ops.append((op, exact_match))
            if gather_parents:
                parent_ops.append((op, exact_match))
            if cmds.attributeQuery('inputs', node=op, exists=True):
                node_type = cmds.nodeType(op)

                if node_type == SWITCH_OP:
                    switch_index = cmds.getAttr("{}.index".format(op))
                    inputs_raw = cmds.listConnections("{}.inputs[{}]".format(op, switch_index), c=True) or []
                elif node_type == LOOKSWITCH_OP:
                    switch_index = cmds.getAttr("{}.index".format(op))
                    inputs_raw = cmds.listConnections("{}.looks[{}].inputs".format(op, switch_index), c=True) or []
                else:
                    inputs_raw = cmds.listConnections("{}.inputs".format(op), c=True) or []
                it = iter(inputs_raw)
                inputs = zip(it, it)
                for plug, ipt in inputs:
                    ops += walkInputs(ipt, path, plug, collections, gather_parents, parent_ops)

            return ops

        if not cmds.objExists(node):
            return []

        # Start the query
        operators = []
        if cmds.attributeQuery('operators', node=node, exists=True):
            con_operators = cmds.listConnections('{}.operators'.format(node)) or []
            for idx, op in enumerate(con_operators):
                out_op = walkInputs(op, path, '{}.operators[{}]'.format(node, idx), collections, gather_parents, [])
                for op, match in out_op:
                    if op not in operators:
                        operators.append((op, match))

        return operators

    def deleteOperator(self, operator):
        return cmds.delete(operator)

    def getOverrides(self, node, path):

        def getParmInList(param, param_list):
            for i, p in enumerate(param_list):
                if p[PARAM] == param:
                    return i
            return -1

        collections = self.getCollections(node, path)
        ops = self.getOperators(node, path, OVERRIDE_OP, collections)

        overrides = []
        parent_overrides = []
        if len(ops):
            for op, match in ops:
                for c in cmds.getAttr('{}.assignment'.format(op), multiIndices=True) or []:
                    ass_str = cmds.getAttr("{}.assignment[{}]".format(op, c))
                    enabled = cmds.getAttr("{}.enableAssignment[{}]".format(op, c))
                    mat = EXP_REGEX.match(ass_str)
                    if mat:
                        param = mat.group('param')
                        data = list(mat.groups())
                        data.append(c)
                        data.append(op)
                        data.append(enabled)
                        if match:  # exact match
                            # get if this parameter is already in the list, if so replace it with this one
                            idx = getParmInList(param, overrides)
                            if idx != -1:
                                overrides[idx] = data
                            else:
                                overrides.append(data)  # split to param, op, VALUE
                        else:
                            # get if this parameter is already in the list, if so replace it with this one
                            idx = getParmInList(param, parent_overrides)
                            if idx != -1:
                                parent_overrides[idx] = data
                            else:
                                parent_overrides.append(data)  # split to param, op, VALUE

        return overrides, parent_overrides

    @classmethod
    def setOverride(cls, node, path, operator, param, operation, value, param_type, custom=False, enable=True, index=-1):

        op = operator
        if index == -1:
            n_conn = mu.getAttrNumElements(op, "assignment")
            index = n_conn
            for c in cmds.getAttr('{}.assignment'.format(op), multiIndices=True) or []:
                ass_str = cmds.getAttr("{}.assignment[{}]".format(op, c))
                if ass_str.startswith(param):
                    index = c
                    break
                index = c+1

        if value is None:
            value = ''

        value = str(value)

        if param_type in [AI_TYPE_ENUM, AI_TYPE_STRING, AI_TYPE_POINTER, AI_TYPE_NODE] and not valueIsExpression(value):
            value = "'{}'".format(value)

        # get if this is a custom param
        type_str = ''
        if custom:
            type_str = TYPES_DICT_STRINGS[param_type] + ' '

        param_exp = "{}{}{}{}".format(type_str, param, operation, value)
        cmds.setAttr("{}.assignment[{}]".format(op, index),
                     param_exp,
                     type="string")
        cmds.setAttr("{}.enableAssignment[{}]".format(op, index),
                     enable)
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

    def deleteOverride(self, operator, index):
        if not cmds.objExists(operator):
            return False
        if index != -1:
            if self._indexInAssignment(index, operator):
                cmds.removeMultiInstance('{}.assignment[{}]'.format(operator, index))
                cmds.removeMultiInstance('{}.enableAssignment[{}]'.format(operator, index))
            else:
                return False
            # Final check the the index was removed
            if not self._indexInAssignment(index, operator):
                return True
        return False

    def showGraphEditor(self, node):

        if node:

            window = cmds.window("Operators - {}".format(node), widthHeight=(800, 450))
            form = cmds.formLayout()
            panel = cmds.scriptedPanel(type="nodeEditorPanel", label="Node Editor")
            cmds.formLayout(form, e=True, af=[(panel,s,0) for s in ("top","bottom","left","right")])
            cmds.showWindow(window)

            nodeeditor = panel+'NodeEditorEd'

            ops = cmds.listConnections(node+'.operators')

            for op in ops:
                cmds.nodeEditor(nodeeditor, edit=True, addNode=op)

            cmds.nodeEditor(nodeeditor, edit=True, rootNode=node, fa=True, ann=False)

            return window, nodeeditor
        return None, None

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
            value = param_default.contents.STR
        elif param_type is AI_TYPE_RGB:
            rgb = param_default.contents.RGB
            value = '[{} {} {}]'.format(rgb.r, rgb.g, rgb.b)
        elif param_type is AI_TYPE_RGBA:
            rgb = param_default.contents.RGBA
            value = '[{} {} {} {}]'.format(rgb.r, rgb.g, rgb.b, rgb.a)
        elif param_type is AI_TYPE_VECTOR:
            vec = param_default.contents.VEC
            value = '[{} {} {}]'.format(vec.x, vec.y, vec.z)
        elif param_type is AI_TYPE_VECTOR2:
            vec = param_default.contents.VEC2
            value = '[{} {}]'.format(vec.x, vec.y)
        elif param_type is AI_TYPE_ENUM:
            idx = param_default.contents.INT
            value = AiEnumGetString(AiParamGetEnum(param), idx)

        if AiUniverseCreated:
            ArnoldUniverseEnd()

        return value

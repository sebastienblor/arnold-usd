import mtoa.ui.ae.templates as templates
from mtoa.ui.ae.templates import AttributeTemplate, registerTranslatorUI
from mtoa.ui.ae import aiStandInTemplate
import mtoa.melUtils as mu
import mtoa.utils as utils
from mtoa.callbacks import *
import maya.cmds as cmds
import maya.mel as mel
import re
import os
import os.path
from ast import literal_eval

from alembic.AbcCoreAbstract import *
from alembic.Abc import *
from alembic.Util import *
from alembic.AbcGeom import *

from arnold import *

NODE_TYPES = ['polymesh', 'curves', 'nurbs', 'points']

BLACK_LIST_PARAMS = ['id', 'name', 'sidedness', 'matrix', 'motion_start',
                     'motion_end', 'shader', 'disp_map', 'vidxs', 'vlist',
                     'nsides', 'uvidxs', 'shidxs', 'nlist', 'uvlist',
                     'crease_idxs', 'crease_sharpness', 'use_shadow_group',
                     'use_light_group', 'degree_u', 'degree_v',
                     'transform_type']

CACHE_ATTR = 'ai_abccache'

VISIBILITY = ['differed', 'hidden', 'visible']


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


def ArnoldGetArrayType(nodeType, paramName):
    AiUniverseCreated = ArnoldUniverseOnlyBegin()
    node_entry = AiNodeEntryLookUp(nodeType)
    param_entry = AiNodeEntryLookUpParameter(node_entry, paramName)
    param_type = AiParamGetType(param_entry)
    if param_type == AI_TYPE_ARRAY:
        array_default = AiParamGetDefault(param).contents.ARRAY
        param_type = AiArrayGetType(array_default)
    if AiUniverseCreated:
        ArnoldUniverseEnd()

    return param_type


def abcToArnType(iObj):
    if not iObj:
        return

    md = iObj.getMetaData()
    if IPolyMesh.matches(md) or ISubD.matches(md):
        return 'polymesh'
    elif IPoints.matches(md):
        return 'points'
    elif ICurves.matches(md):
        return 'curves'
    else:
        return None


def getVisibilityValue(vis_flags=[True]*8):

    vis = AI_RAY_ALL

    if not vis_flags[0]:
        vis &= ~AI_RAY_CAMERA
    if not vis_flags[1]:
        vis &= ~AI_RAY_SHADOW
    if not vis_flags[2]:
        vis &= ~AI_RAY_DIFFUSE_TRANSMIT
    if not vis_flags[3]:
        vis &= ~AI_RAY_SPECULAR_TRANSMIT
    if not vis_flags[4]:
        vis &= ~AI_RAY_VOLUME
    if not vis_flags[5]:
        vis &= ~AI_RAY_DIFFUSE_REFLECT
    if not vis_flags[6]:
        vis &= ~AI_RAY_SPECULAR_REFLECT
    if not vis_flags[7]:
        vis &= ~AI_RAY_SUBSURFACE

    return vis


class gpuCacheDescriptionTemplate(templates.ShapeTranslatorTemplate):

    def createCacheAttr(self, node):
        if not cmds.attributeQuery(CACHE_ATTR, node=node, exists=True):
            # make the attr
            # cmds.addAttr(node, longName=CACHE_ATTR, dt="stringArray" )
            cmds.addAttr(node, longName=CACHE_ATTR, numberOfChildren=6, multi=True, at="compound", hidden=True )
            cmds.addAttr(node, longName="ai_abcPath", dt="string", parent=CACHE_ATTR)
            cmds.addAttr(node, longName="ai_abcLabel", dt="string", parent=CACHE_ATTR)
            cmds.addAttr(node, longName="ai_abcParent", dt="string", parent=CACHE_ATTR)
            cmds.addAttr(node, longName="ai_abcVisibility", dt="string", parent=CACHE_ATTR)
            cmds.addAttr(node, longName="ai_abcInstancePath", dt="string", parent=CACHE_ATTR)
            cmds.addAttr(node, longName="ai_entity_type", dt="string", parent=CACHE_ATTR)

    def getPathProperties(self, path):

        num_overrides = mu.getAttrNumElements(self.nodeName, "aiOverrides")
        path_props = {'path': path, 'shader': None, 'disp': None, 'overrides': [], 'index': num_overrides}

        for i, p in enumerate(self.abcItems):
            if p[0] == path:
                path_props['path'] = p[0]
                path_props['shader'] = cmds.listConnections('{}.aiOverrides[{}].abcShader'.format(self.nodeName, i)) or None
                path_props['disp'] = cmds.listConnections('{}.aiOverrides[{}].abcDisplacement'.format(self.nodeName, i)) or None
                indices = cmds.getAttr('{}.aiOverrides[{}].abcOverrides'.format(self.nodeName, i), multiIndices=True) or []
                for c in indices:
                    path_props['overrides'].append(cmds.getAttr('{}.aiOverrides[{}].abcOverrides[{}]'.format(self.nodeName, i, c)))
                path_props['index'] = i
                break
        return path_props

    def selectGeomPath(self, itemName, itemLabel):
        # if itemName in self.abcItems:
        for item in self.abcItems:
            if itemName == item[0] and item[3] != VISIBILITY[1]:
                cmds.setAttr('{}.cacheGeomPath'.format(self.nodeName), itemName.replace('/', '|'), type='string')

    def getOperator(self, path_props, createOp=True):
        # return the operator for this path,
        # or create a new one

        def walkInputs(op, path, plug):

            selection_exp = '{}*'.format(path)

            r_ipt = r_plug = None
            if cmds.attributeQuery('selection', node=op, exists=True):
                _this_sel_exp = cmds.getAttr('{}.selection'.format(op))
                if selection_exp == _this_sel_exp:
                    return op, plug

            if cmds.attributeQuery('inputs', node=op, exists=True):
                inputs_raw = cmds.listConnections("{}.inputs".format(op), c=True) or []
                it = iter(inputs_raw)
                inputs = zip(it, it)
                for plug, ipt in inputs:
                    if cmds.attributeQuery('selection', node=ipt, exists=True):
                        _this_sel_exp = cmds.getAttr('{}.selection'.format(ipt))
                        if selection_exp == _this_sel_exp:
                            return ipt, plug
                    r_ipt, r_plug = walkInputs(ipt, path, plug)

            return r_ipt, r_plug

        sel_path = path_props['path']

        selection_exp = '{}*'.format(sel_path)

        out_op = None
        plug = None
        op_list = []

        operators = cmds.listConnections('{}.operators'.format(self.nodeName)) or []
        for idx, op in enumerate(operators):
            out_op, plug = walkInputs(op, path_props['path'], '{}.operators[{}]'.format(self.nodeName, idx))
            if out_op:
                break

        # if no operator matches make one
        if not out_op and createOp:
            num_ops = mu.getAttrNumElements(self.nodeName, "operators")
            op_name = '{}_setParam{}'.format(self.nodeName,path_props['index'])
            out_op = cmds.createNode('aiSetParameter',
                                     n=op_name,
                                     ss=True)
            cmds.setAttr("{}.selection".format(out_op), selection_exp, type="string")

            cmds.connectAttr("{}.message".format(out_op), "{}.operators[{}]".format(self.nodeName, num_ops), force=True)

        return out_op

    def deleteOverride(self, op, index, layout):
        status = cmds.removeMultiInstance('{}.assignment[{}]'.format(op, index))
        path_props = self.getPathProperties(self.currentItem)
        path_attr = "{}.aiOverrides[{}]".format(self.nodeName, path_props['index'])
        status = cmds.removeMultiInstance("{}.abcOverrides[{}]".format(path_attr, index))

        childUIs = cmds.layout(layout, q=True, childArray=True) or []
        for cu in childUIs:
            cmds.deleteUI(cu)
        cmds.deleteUI(layout)
        # update the label in the tree view
        for path, label, parent, visibility, instancedPath, entity_type in self.abcItems:
            if path == path_props['path']:
                self.updateTreeItem(path, label, parent, visibility, instancedPath, entity_type)

        return status

    def addOverride(self):
        if self.currentItem:
            path_props = self.getPathProperties(self.currentItem)
            path_nodeAttr = "{}.aiOverrides[{}]".format(self.nodeName, path_props['index'])
            # get the operator for this path
            op = self.getOperator(path_props)

            self.setParamValueOverride(op, "newOverride", "newValue")
            self.showAbcItemProperties(force=True)
            # update the label in the tree view
            for path, label, parent, visibility, instancedPath, entity_type in self.abcItems:
                if path == path_props['path']:
                    self.updateTreeItem(path, label, parent, visibility, instancedPath, entity_type)

    def addOverrideGUI(self, item, layout, attr, value, index):
        path_props = self.getPathProperties(item)
        path_nodeAttr = "{}.aiOverrides[{}]".format(self.nodeName, path_props['index'])
        # get the operator for this path
        op = self.getOperator(path_props)

        _layout = cmds.formLayout(p=layout)

        deleteBtn = cmds.iconTextButton(image="trash.png", style="iconOnly",
                                        height=20,
                                        width=20,
                                        flat=True,
                                        command=lambda *a: self.deleteOverride(op, index, _layout))
        paramBox = cmds.optionMenu()

        cmds.menuItem(label="Choose Override ..")
        for param in sorted(self.paramDict.keys()):
            if param not in BLACK_LIST_PARAMS:
                cmds.menuItem(label=param)

        # now make the control that is appropriate for the current parameter

        paramValueLayout = cmds.formLayout(p=_layout)

        cmds.formLayout(_layout, edit=True,
                        attachForm=[(deleteBtn, 'left', 2),(paramValueLayout, 'right', 2),
                                    (deleteBtn, 'top', 2), (paramBox, 'top', 4), (paramValueLayout, 'top', 4)],
                        attachControl=[(paramBox, 'left', 2, deleteBtn), (paramValueLayout, 'left', 2, paramBox)]
                        )

        if attr != "newOverride":
            cmds.optionMenu(paramBox,  e=True, value=attr)

        cmds.optionMenu(paramBox,  e=True,
                        changeCommand=lambda v: self.setParamAttrOverride(op, v, paramValueLayout, index))

        paramValueCtrl = None
        if value != "newValue":
            cmds.setParent(paramValueLayout)
            paramValueCtrl = self.makeValueControl(op, attr, value, paramValueLayout, paramBox)
            cmds.formLayout(paramValueLayout, edit=True,
                            attachForm=[(paramValueCtrl, 'left', 0),
                                        (paramValueCtrl, 'bottom', 0),
                                        (paramValueCtrl, 'top', 0)])
            cmds.setParent("..")

    def _convertValue(self, value, param_type):
        o_value = value
        if param_type in [AI_TYPE_INT, AI_TYPE_UINT, AI_TYPE_BYTE]:
            o_value = int(value)
        elif param_type == AI_TYPE_FLOAT:
            o_value = float(value)
        elif param_type == AI_TYPE_BOOLEAN:
            if type(value) in [str, unicode]:
                o_value = value.lower() == "true"
            elif type(value) is bool:
                o_value = value
        elif param_type in [AI_TYPE_STRING, AI_TYPE_ENUM, AI_TYPE_POINTER]:
            o_value = str(value.replace("'", ""))
        return o_value

    def makeValueControl(self, op, param_name, value, parent, paramBox=None):
        ctrl = None
        node_type, param_type = self.paramDict.get(param_name, (None, None))

        if not node_type and paramBox:
            param_name = cmds.optionMenu(paramBox, q=True, value=True)
            node_type, param_type = self.paramDict.get(param_name, (None, None))

        AiUniverseCreated = ArnoldUniverseOnlyBegin()
        node_entry = AiNodeEntryLookUp(node_type)

        if node_entry:
            param_entry = AiNodeEntryLookUpParameter(node_entry, param_name)
            if param_entry:
                o_value = None
                if value == "newValue":
                    o_value = self._getDefaultValue(param_entry, param_type)
                else:
                    o_value = self._convertValue(value, param_type)

                ctrl = self._createControl(node_type, param_name, param_type,
                                           param_entry, "ARNOp{}_{}".format(node_type, param_name), parent=parent,
                                           label="", value=o_value, changeCommand=lambda v: self.setParamValueOverride(op, param_name, v))
        if AiUniverseCreated:
            ArnoldUniverseEnd()
        return ctrl

    def setParamAttrOverride(self, op, newParam, layout, index):
        AiUniverseCreated = ArnoldUniverseOnlyBegin()
        node_type, param_type = self.paramDict.get(newParam, (None, None))
        node_entry = AiNodeEntryLookUp(node_type)
        if node_entry:
            param_entry = AiNodeEntryLookUpParameter(node_entry, newParam)
            if param_entry:
                value = self._getDefaultValue(param_entry, param_type)
                if param_type in [AI_TYPE_ENUM, AI_TYPE_STRING, AI_TYPE_POINTER]:
                    value = "'{}'".format(value)
                param_exp = "{}={}".format(newParam, value)
                cmds.setAttr("{}.assignment[{}]".format(op, index),
                             param_exp,
                             type="string")
                path_props = self.getPathProperties(self.currentItem)
                path_attr = "{}.aiOverrides[{}]".format(self.nodeName, path_props['index'])
                cmds.setAttr("{}.abcOverrides[{}]".format(path_attr, index), param_exp, type="string")

                # recreate the value control
                childUIs = cmds.layout(layout, q=True, childArray=True) or []
                for cu in childUIs:
                    cmds.deleteUI(cu)
                cmds.setParent(layout)
                ctrl = self.makeValueControl(op, newParam, value, layout)
                cmds.formLayout(layout, edit=True,
                                attachForm=[(ctrl, 'left', 0),
                                            (ctrl, 'bottom', 0),
                                            (ctrl, 'top', 0)])

                # update the label in the tree view
                for path, label, parent, visibility, instancedPath, entity_type in self.abcItems:
                    if path == path_props['path']:
                        self.updateTreeItem(path, label, parent, visibility, instancedPath, entity_type)

        if AiUniverseCreated:
            ArnoldUniverseEnd()

    def setParamValueOverride(self, op, param, value):
        if param == 'visibility':
            vis_state = []
            for ctrl in self.visibility_ctrls:
                vis_state.append(cmds.checkBox(ctrl, q=True, value=True))
            if len(vis_state) == 8:
                value = getVisibilityValue(vis_state)
            else:
                return

        node_type, param_type = self.paramDict.get(param, (None, None))
        if param_type == AI_TYPE_ARRAY:
            param_type = ArnoldGetArrayType(node_type, param)
        # get the assignment index for this parameter
        n_conn = mu.getAttrNumElements(op, "assignment")
        c_idx = n_conn
        if param != 'newOverride':
            for c in cmds.getAttr('{}.assignment'.format(op), multiIndices=True) or []:
                ass_str = cmds.getAttr("{}.assignment[{}]".format(op, c))
                if ass_str.startswith(param):
                    c_idx = c
                    break

        path_props = self.getPathProperties(self.currentItem)
        if value == "" and c_idx != n_conn:
            status = cmds.removeMultiInstance('{}.assignment[{}]'.format(op, c_idx))
            path_attr = "{}.aiOverrides[{}]".format(self.nodeName, path_props['index'])
            status = cmds.removeMultiInstance("{}.abcOverrides[{}]".format(path_attr, c_idx))
        else:
            if param_type in [AI_TYPE_ENUM, AI_TYPE_STRING, AI_TYPE_POINTER, AI_TYPE_NODE]:
                value = "'{}'".format(value)
            param_exp = "{}={}".format(param, value)
            cmds.setAttr("{}.assignment[{}]".format(op, c_idx),
                         param_exp,
                         type="string")
            path_attr = "{}.aiOverrides[{}]".format(self.nodeName, path_props['index'])
            cmds.setAttr("{}.abcOverrides[{}]".format(path_attr, c_idx), param_exp, type="string")

        # update the label in the tree view
        for path, label, parent, visibility, instancedPath, entity_type in self.abcItems:
            if path == path_props['path']:
                self.updateTreeItem(path, label, parent, visibility, instancedPath, entity_type)

    def shaderChanged(self):
        self.setShading('abcShader', 'shader')

    def displacementChanged(self):
        self.setShading('abcDisplacement', 'disp_map')

    def setShading(self, attrName, paramName):
        if self.currentItem:
            path_props = self.getPathProperties(self.currentItem)
            path_attr = "{}.aiOverrides[{}]".format(self.nodeName, path_props['index'])
            shaderAttrName = "{}.{}".format(path_attr, attrName)

            shader = cmds.listConnections(shaderAttrName) or []

            # get an operator for the selection

            op = self.getOperator(path_props)
            if len(shader):
                shader = shader[0]
                self.setParamValueOverride(op, paramName, shader)

                # update the label in the tree view
                for path, label, parent, visibility, instancedPath, entity_type in self.abcItems:
                    if path == path_props['path']:
                        self.updateTreeItem(path, label, parent, visibility, instancedPath, entity_type)
            else:
                self.setParamValueOverride(op, paramName, "")

    def selectShader(self, item, state):
        self.selectNode('abcShader', item)

    def selectDisplacment(self, item, state):
        self.selectNode('abcDisplacement', item)

    def selectNode(self, attr, item):

        path_props = self.getPathProperties(item)
        path_attr = "{}.aiOverrides[{}]".format(self.nodeName, path_props['index'])
        connAttrName = "{}.{}".format(path_attr, attr)

        node_conn = cmds.listConnections(connAttrName) or []

        if len(node_conn):
            cmds.select(node_conn, r=True)

    def selectOperator(self, item, state):
        path_props = self.getPathProperties(item)
        path_attr = "{}.operators[{}]".format(self.nodeName, path_props['index'])

        node_conn = cmds.listConnections(path_attr) or []

        if len(node_conn):
            cmds.select(node_conn, r=True)

    def showAbcItemProperties(self, force=False):
        '''Display the shaders and overrides for the selected Item'''

        item = cmds.treeView(self.abcInfoPath, q=True, selectItem=True) or []
        if len(item):
            item = item[0]
        else:
            return False
        # remove if deselected
        childUIs = cmds.layout(self.overridesLayout, q=True, childArray=True) or []
        for cu in childUIs:
            cmds.deleteUI(cu)

        # if item is not self.currentItem and (len(item) or force):

        path_props = self.getPathProperties(item)
        path_attr = "{}.aiOverrides[{}]".format(self.nodeName, path_props['index'])
        cmds.layout(self.shaderAssignerLayout, e=True, visible=True)
        cmds.text(self.overrideEditorLabel, e=True, label=item)
        cmds.setAttr("{}.abcPath".format(path_attr), item, type="string")
        shaderAttrName = "{}.abcShader".format(path_attr)
        shaderConnection = cmds.listConnections("{}.abcShader".format(path_attr), s=True, d=False) or []

        cmds.attrNavigationControlGrp(self.shaderAssignerField,
                                      edit=True, at=shaderAttrName,
                                      label="Surface Shader",
                                      cn="createRenderNode -allWithShadersUp \"defaultNavigation -force true -connectToExisting -source %node -destination "+shaderAttrName+"\" \"\"")

        cmds.layout(self.displacementShaderAssignerLayout, e=True, visible=True)
        dispAttrName = "{}.abcDisplacement".format(path_attr)
        displacementConnection = cmds.listConnections("{}.abcDisplacement".format(path_attr), s=True, d=False) or []
        cmds.attrNavigationControlGrp(self.displacementShaderAssignerField,
                                      edit=True,
                                      at=dispAttrName,
                                      label="Displacement Shader",
                                      cn="createRenderNode -allWithTexturesUp \"defaultNavigation -force true -connectToExisting -source %node -destination "+dispAttrName+"\" \"\"")

        overrides_collayout = cmds.columnLayout(p=self.overridesLayout, adjustableColumn=True)

        self.visibility_ctrls = []
        self.currentItem = item
        self.paramDict = {}
        node_types = []
        # get the node_types from the current Item
        for p in self.abcItems:
            if item in p[0]:
                node_types.append(p[5])

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

                    paramName = AiParamGetName(param)
                    if paramName not in self.paramDict:
                        self.paramDict[paramName] = (nodeType, param_type)

                AiParamIteratorDestroy(paramIter)

        op = self.getOperator(path_props, False)
        if op:
            for o in cmds.getAttr('{}.assignment'.format(op), multiIndices=True) or []:
                exp = cmds.getAttr('{}.assignment[{}]'.format(op, o))
                if '=' in exp:
                    attr, value = exp.split('=')
                    if attr not in ['shader', 'disp_map']:
                        cmds.setParent(overrides_collayout)
                        self.addOverrideGUI(item, overrides_collayout, attr.strip('\"'), value.strip('\"'), o)

        # populate the overrides for this node
        self.addOverrideBtn = cmds.iconTextButton(parent=overrides_collayout, label='Add Override', image="gear.png",
                                                  style="iconAndTextHorizontal", command=self.addOverride)

        if AiUniverseCreated:
            ArnoldUniverseEnd()

        cmds.scriptJob(attributeChange=[shaderAttrName, self.shaderChanged],
                       replacePrevious=True, parent=self.shaderAssignerField)
        cmds.scriptJob(attributeChange=[dispAttrName, self.displacementChanged],
                       replacePrevious=True, parent=self.displacementShaderAssignerField)

        return True

    def abcInfoNew(self, nodeAttr):
        self.currentItem = None
        cmds.rowLayout(nc=2)
        cmds.text(label='')
        self.inspectAlembicPath = cmds.button(align="center", label='Inspect Alembic File', command=lambda *args: self.inspectAlembic())
        cmds.setParent('..')  # rowLayout
        self.abcInfoPath = cmds.treeView(height=300, numberOfButtons=3,
                                         allowReparenting=False,
                                         attachButtonRight=True,
                                         selectionChangedCommand=self.showAbcItemProperties,
                                         itemDblClickCommand2=self.selectGeomPath,
                                         pressCommand=[(1, self.selectOperator),
                                                       (2, self.selectShader),
                                                       (3, self.selectDisplacment)]
                                         )
        # editor panel
        self.overrideEditorLabel = cmds.text(label="")
        self.shaderAssignerLayout = cmds.rowLayout(nc=2, columnWidth=[[2, 30]], adjustableColumn=1, visible=False)
        self.shaderAssignerField = cmds.attrNavigationControlGrp(
                                          label="Surface Shader",
                                          parent=self.shaderAssignerLayout)

        cmds.setParent('..')  # rowLayout
        self.displacementShaderAssignerLayout = cmds.rowLayout(nc=2, columnWidth=[[2, 30]], adjustableColumn=1, visible=False)
        self.displacementShaderAssignerField = cmds.attrNavigationControlGrp(
                                                label="Displacement Shader",
                                                parent=self.displacementShaderAssignerLayout)
        cmds.setParent('..')  # rowLayout
        cmds.separator()
        self.overridesLayout = cmds.columnLayout(adjustableColumn=True)

        cmds.setParent('..')  # rowLayout

        self.abcInfoReplace(nodeAttr)

        fileAttr = self.nodeName + ".cacheFileName"
        cmds.scriptJob(attributeChange=[fileAttr, self.updateAlembicFile],
                       replacePrevious=True, parent=self.inspectAlembicPath)

    def updateAlembicFile(self):
        # clear the cache
        self.abcItems = []
        self.abcPath = cmds.getattr(self.nodeName + ".cacheFileName", type="string")
        cmds.treeView(self.abcInfoPath, edit=True, visible=False)
        cmds.button(self.inspectAlembicPath, edit=True, visible=True)
        cmds.treeView(self.abcInfoPath, edit=True, removeAll=True)

    def visitObject(self, iObj, parent="", visibility="visible"):

        path = iObj.getFullName()
        name = iObj.getName()
        instancedPath = iObj.instanceSourcePath()

        entity_type = abcToArnType(iObj)

        if visibility != VISIBILITY[1]:
            visibility = VISIBILITY[int(GetVisibility(iObj))+1]

        geomPath = path.replace('/', '|')
        self.abcItems.append([path, name, parent, visibility, instancedPath, entity_type])

        for child in iObj.children:
            self.visitObject(child, path, visibility)

    def updateTreeItem(self, path, label, parent, visibility, instancedPath, entity_type):

        assigned_color = (0.8, 0.8, 1.0)

        path_props = self.getPathProperties(path)

        overrides_length = len(path_props['overrides'])

        if visibility == VISIBILITY[1]:
            label += " (hidden)"
            cmds.treeView(self.abcInfoPath, edit=True, textColor=(path, 0.5, 0.5, 0.5))
        if instancedPath != '':
            label += " (instanced)"
            cmds.treeView(self.abcInfoPath, edit=True, textColor=(path, 0.5, 0.9, 0.5))
        if path_props['shader']:
            overrides_length -= 1
            label += " surf: {}".format(','.join(path_props['shader']))
            cmds.treeView(self.abcInfoPath, e=True,
                          enableButton=[(path, 2, 1)],
                          textColor=tuple([path])+assigned_color)
        if path_props['disp']:
            overrides_length -= 1
            label += " disp: {}".format(','.join(path_props['disp']))
            cmds.treeView(self.abcInfoPath, e=True,
                          enableButton=[(path, 3, 1)],
                          textColor=tuple([path])+assigned_color)
        if overrides_length >= 1:
            label += " overrides:{}".format(len(path_props['overrides']))
            cmds.treeView(self.abcInfoPath, e=True,
                          enableButton=[(path, 1, 1)],
                          textColor=tuple([path])+assigned_color)

        cmds.treeView(self.abcInfoPath, edit=True, displayLabel=(path, label))

    def displayTree(self):
        cmds.treeView(self.abcInfoPath, edit=True, removeAll=True)
        cmds.treeView(self.abcInfoPath, edit=True, visible=True)
        cmds.button(self.inspectAlembicPath, edit=True, visible=False)

        assigned_color = (0.2, 0.2, 0.9)

        for _path, _label, _parent, _visibility, _instancedPath, _entity_type in self.abcItems:

            cmds.treeView(self.abcInfoPath, edit=True, addItem=(_path, _parent))
            cmds.treeView(self.abcInfoPath, e=True,
                          image=[(_path, 1, "gear.png"),
                                 (_path, 2, "shaderGlow.svg"),
                                 (_path, 3, "displacementShader.svg")],
                          enableButton=[(_path, 1, 0),
                                        (_path, 2, 0),
                                        (_path, 3, 0)],
                          buttonTooltip=[(_path, 1, "Select override"),
                                         (_path, 2,  "Select Surface Shader"),
                                         (_path, 3,  "Select Displacement Shader")])

            self.updateTreeItem(_path, _label, _parent, _visibility, _instancedPath, _entity_type)

        geomPathAttr = self.nodeName + '.cacheGeomPath'
        geomPath = cmds.getAttr(geomPathAttr).replace('|', '/')

        if self.currentItem and cmds.treeView(self.abcInfoPath, q=True,
                                              itemExists=self.currentItem):
            cmds.treeView(self.abcInfoPath, edit=True,
                              selectItem=[self.currentItem , True])
        else:
            # if geomPath in self.abcItems:
            for item in self.abcItems:
                if geomPath == item[0]:
                    cmds.treeView(self.abcInfoPath, edit=True,
                                  selectItem=[geomPath, True])

        self.showAbcItemProperties()

    def inspectAlembic(self):
        filenameAttr = self.nodeName + '.cacheFileName'
        filename = cmds.getAttr(filenameAttr)

        if not os.path.exists(filename):
            return

        iArchive = IArchive(str(filename))
        self.visitObject( iArchive.getTop() )

        for i, v in enumerate(self.abcItems):
            cmds.setAttr('{}.{}[{}].ai_abcPath'.format(self.nodeName, CACHE_ATTR, i), v[0], type="string")
            cmds.setAttr('{}.{}[{}].ai_abcLabel'.format(self.nodeName, CACHE_ATTR, i), v[1], type="string")
            cmds.setAttr('{}.{}[{}].ai_abcParent'.format(self.nodeName, CACHE_ATTR, i), v[2], type="string")
            cmds.setAttr('{}.{}[{}].ai_abcVisibility'.format(self.nodeName, CACHE_ATTR, i), v[3], type="string")
            cmds.setAttr('{}.{}[{}].ai_abcInstancePath'.format(self.nodeName, CACHE_ATTR, i), v[4], type="string")
            cmds.setAttr('{}.{}[{}].ai_entity_type'.format(self.nodeName, CACHE_ATTR, i), str(v[5]), type="string")

        self.displayTree()

    def populateItems(self):
        # get the items in the cache
        for i in range(mu.getAttrNumElements(self.nodeName, CACHE_ATTR)):
            _path = cmds.getAttr('{}.{}[{}].ai_abcPath'.format(self.nodeName, CACHE_ATTR, i))
            _label = cmds.getAttr('{}.{}[{}].ai_abcLabel'.format(self.nodeName, CACHE_ATTR, i))
            _parent = cmds.getAttr('{}.{}[{}].ai_abcParent'.format(self.nodeName, CACHE_ATTR, i))
            _visibility = cmds.getAttr('{}.{}[{}].ai_abcVisibility'.format(self.nodeName, CACHE_ATTR, i))
            _instanced = cmds.getAttr('{}.{}[{}].ai_abcInstancePath'.format(self.nodeName, CACHE_ATTR, i))
            _entity_type= cmds.getAttr('{}.{}[{}].ai_entity_type'.format(self.nodeName, CACHE_ATTR, i))

            self.abcItems.append((_path, _label, _parent, _visibility, _instanced, _entity_type))

    def abcInfoReplace(self, nodeAttr):
        self.abcItems = []
        cache_attr_exists = cmds.attributeQuery(CACHE_ATTR, node=self.nodeName, exists=True)
        if not cache_attr_exists:
            self.createCacheAttr(self.nodeName)
        else:
            self.populateItems()
            if len(self.abcItems):
                self.displayTree()
            else:
                cmds.treeView(self.abcInfoPath, edit=True, visible=False)
                cmds.button(self.inspectAlembicPath, edit=True, visible=True)
                cmds.treeView(self.abcInfoPath, edit=True, removeAll=True)

    def populateParams(self, node_type):
        # iterate over params
        node_entry = AiNodeEntryLookUp(node_type)
        parmaIter = AiNodeEntryGetParamIterator(node_entry)
        while not AiParamIteratorFinished(parmaIter):
            this_param = AiParamIteratorGetNext(parmaIter)
            param_name = AiParamGetName(this_param)
            param_type = AiParamGetType(this_param)

            # check this param is not in the black list
            if param_name not in BLACK_LIST_PARAMS:
                if param_type not in [AI_TYPE_ARRAY]:
                    self.user_attrs[node_type][param_name] = self._createControl(node_type, param_name, param_type, this_param)

    def _createVisibilityCtrl(self, control_name, value=255, changeCommand=None, parent=None):
        _layout = cmds.formLayout(parent=parent)

        self.visibility_ctrls = []

        s=0
        for l in ['Camera', 'Shadow', 'Diffuse Transmission', 'Specualr Transmission',
                  'Volume', 'Diffuse Reflections', 'Specular Reflections', 'Sub-Surface']:

            vis_ctrl = cmds.checkBox(control_name+l.replace(" ", ""),
                                     label=l,
                                     changeCommand=changeCommand)

            cmds.formLayout(_layout, edit=True,
                            attachForm=[(vis_ctrl, 'left', 0)])
            if s == 0:
                cmds.formLayout(_layout, edit=True,
                                attachForm=[(vis_ctrl, 'top', 0)])
            else:
                cmds.formLayout(_layout, edit=True,
                                attachControl=[(vis_ctrl, 'top', 0, self.visibility_ctrls[-1])])
            self.visibility_ctrls.append(vis_ctrl)
            s += 1

        # set the checkbox state based on the byte value

        state_list = [True]*8
        compViz = AI_RAY_ALL
        if value < compViz:
            compViz &= ~AI_RAY_SUBSURFACE
            if value <= compViz:
                state_list[7] = False
            else:
                compViz += AI_RAY_SUBSURFACE
            compViz &= ~AI_RAY_SPECULAR_REFLECT
            if value <= compViz:
                state_list[6] = False
            else:
                compViz += AI_RAY_SPECULAR_REFLECT
            compViz &= ~AI_RAY_DIFFUSE_REFLECT
            if value <= compViz:
                state_list[5] = False
            else:
                compViz += AI_RAY_DIFFUSE_REFLECT
            compViz &= ~AI_RAY_VOLUME
            if value <= compViz:
                state_list[4] = False
            else:
                compViz += AI_RAY_VOLUME
            compViz &= ~AI_RAY_SPECULAR_TRANSMIT
            if value <= compViz:
                state_list[3] = False
            else:
                compViz += AI_RAY_SPECULAR_TRANSMIT
            compViz &= ~AI_RAY_DIFFUSE_TRANSMIT
            if value <= compViz:
                state_list[2] = False
            else:
                compViz += AI_RAY_DIFFUSE_TRANSMIT
            compViz &= ~AI_RAY_SHADOW
            if value <= compViz:
                state_list[1] = False
            else:
                compViz += AI_RAY_SHADOW
            compViz &= ~AI_RAY_CAMERA
            if value <= compViz:
                state_list[0] = False

        c=0
        for ctrl in self.visibility_ctrls:
            cmds.checkBox(ctrl, edit=True, value=state_list[c])
            c+=1

        cmds.setParent('..')

        return _layout

    def _createControl(self, node_type, param_name, param_type, param_entry, control_name="", parent=None, label=None, value=None, changeCommand=None):
        if not control_name:
            control_name = '{}_{}_ctrl'.format(node_type, param_name)
        control = None
        args = {}
        if label == None:
            label = param_name.replace('_', ' ').title()
        # if label:
        #     args['label'] = label
        if parent:
            args['parent'] = parent
        if param_type in [AI_TYPE_BYTE] and param_name == 'visibility':
            control = self._createVisibilityCtrl(control_name, value, changeCommand, parent)
        elif param_type in [AI_TYPE_INT, AI_TYPE_BYTE, AI_TYPE_UINT]:
            control = cmds.intField(control_name, **args)
            if value:
                cmds.intField(control, edit=True, value=value)
            if changeCommand:
                cmds.intField(control, edit=True, changeCommand=changeCommand)
        elif param_type is AI_TYPE_FLOAT:
            control = cmds.floatField(control_name, **args)
            if value:
                cmds.floatField(control, edit=True, value=value)
            if changeCommand:
                cmds.floatField(control, edit=True, changeCommand=changeCommand)
        elif param_type is AI_TYPE_BOOLEAN:
            control = cmds.checkBox(control_name, **args)
            if value:
                cmds.checkBox(control, edit=True, value=str(value).lower() == 'true' )
            if changeCommand:
                cmds.checkBox(control, edit=True, changeCommand=changeCommand)
        elif param_type in [AI_TYPE_STRING, AI_TYPE_NODE, AI_TYPE_POINTER]:
            control = cmds.textField(control_name, **args)
            if value:
                cmds.textField(control, edit=True, value=value)
            if changeCommand:
                cmds.textField(control, edit=True, changeCommand=changeCommand)
        elif param_type is AI_TYPE_ENUM:
            enum_ctrl = cmds.optionMenu(control_name, **args)
            # populate the options
            i = 0
            t = True
            while t is not None:
                t = AiEnumGetString(AiParamGetEnum(param_entry), i)
                if t:
                    cmds.menuItem(label=t)
                i += 1
            control = enum_ctrl
            if value:
                cmds.optionMenu(enum_ctrl, edit=True, value=value)
            if changeCommand:
                cmds.optionMenu(enum_ctrl, edit=True, changeCommand=changeCommand)
        return control

    def _getDefaultValue(self, param, param_type):
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

        return value

    def _getCtrlValue(self, control, param_type):
        value = None
        if param_type in [AI_TYPE_INT, AI_TYPE_BYTE, AI_TYPE_UINT]:
            value = cmds.intFieldGrp(control, query=True, value1=True)
        elif param_type is AI_TYPE_FLOAT:
            value = cmds.floatFieldGrp(control, query=True, value1=True)
        elif param_type is AI_TYPE_BOOLEAN:
            value = cmds.checkBoxGrp(control, query=True, value1=True)
        elif param_type is AI_TYPE_STRING:
            value = cmds.textFieldGrp(control, query=True, value1=True)
        elif param_type is AI_TYPE_ENUM:
            value = cmds.optionMenuGrp(control, query=True, value=True)

        return value

    def createSubSections(self, node_type):
        section = {}
        section['common'] = cmds.frameLayout(label='Common', collapse=False)
        cmds.setParent('..')
        if node_type == 'polymesh':
            section['subdiv'] = cmds.frameLayout(label='Subdivision', collapse=True)
            cmds.setParent('..')
            section['disp'] = cmds.frameLayout(label='Displacement', collapse=True)
            cmds.setParent('..')
            section['volume'] = cmds.frameLayout(label='Volume', collapse=True)
            cmds.setParent('..')
        if node_type == 'nurbs':
            section['disp'] = cmds.frameLayout(label='Displacement', collapse=True)
            cmds.setParent('..')
            section['volume'] = cmds.frameLayout(label='Volume', collapse=True)
            cmds.setParent('..')
        if node_type == 'points':
            section['volume'] = cmds.frameLayout(label='Volume', collapse=True)
            cmds.setParent('..')
        return section

    def userAttrsNew(self, nodeAttrName):
        AiUniverseCreated = ArnoldUniverseOnlyBegin()
        subsections = {}
        for node_type in NODE_TYPES:
            layout = cmds.frameLayout(label=node_type.title(), collapse=True)
            self.attr_ctrls[node_type] = {}
            # add specific sub-sections
            subsections[node_type] = self.createSubSections(node_type)
            cmds.setParent('..')
            node_entry = AiNodeEntryLookUp(node_type)
            parmaIter = AiNodeEntryGetParamIterator(node_entry)
            param_default = None
            while not AiParamIteratorFinished(parmaIter):
                this_param = AiParamIteratorGetNext(parmaIter)
                param_name = AiParamGetName(this_param)
                param_type = AiParamGetType(this_param)
                sub_param = 'common'
                if param_name.startswith('subdiv'):
                    sub_param = 'subdiv'
                if param_name.startswith('disp'):
                    sub_param = 'disp'
                if param_name in ['step_size', 'volume_padding']:
                    sub_param = 'volume'
                # special catch for polymesh:smoothing
                if node_type == "polymesh" and param_name == "smoothing":
                    param_default = True
                else:
                    param_default = self._getDefaultValue(this_param, param_type)

                # check this param is not in the black list
                if param_name not in BLACK_LIST_PARAMS:
                    if param_type not in [AI_TYPE_ARRAY]:
                        cmds.setParent(subsections[node_type][sub_param])
                        self.attr_ctrls[node_type][param_name] = {'control':self._createControl(node_type, param_name, param_type, this_param),
                                                                  'type': AiParamGetTypeName(param_type),
                                                                  'default': param_default}
                        cmds.setParent('..')
            cmds.setParent('..')
        if AiUniverseCreated: ArnoldUniverseEnd()

        self.userAttrsReplace(nodeAttrName)

    def userAttrsReplace(self, nodeAttrName):
        self.user_attrs = {}
        # populate the current attributes list
        idxs = cmds.getAttr(nodeAttrName, mi=True) or []
        for idx in idxs:
            param_name = cmds.getAttr(nodeAttrName+'[{}].attrName'.format(idx))
            attrValue = cmds.getAttr(nodeAttrName+'[{}].attrValue'.format(idx))
            attrNodeType = cmds.getAttr(nodeAttrName+'[{}].attrNodeType'.format(idx))
            nodeType_str = NODE_TYPES[attrNodeType]

            nodeParam_str = '{}:{}'.format(nodeType_str, param_name)
            self.user_attrs[nodeParam_str] = {'idx':idx, 'value': attrValue}

        for node_type, params in self.attr_ctrls.items():
            for param_name, control in params.items():
                self.setUserAttrCtrl(nodeAttrName, node_type, param_name)

    def setUserAttrCtrl(self, nodeAttrName, node_type, param_name):
        # check if attribute allready has been set in the current node
        value = None
        ctrl = None
        nodeParamName = ':'.join([node_type, param_name])
        details = self.attr_ctrls[node_type][param_name]
        if nodeParamName in self.user_attrs:
            value = self.user_attrs[nodeParamName].get('value', None)
        else:
            value = self.attr_ctrls[node_type][param_name].get('default')
        if value is not None:
            # set the control to the attribute value
            if details['type'] in ['INT', 'BYTE', 'UINT']:
                ctrl = cmds.intFieldGrp(details['control'], edit=True, value1=int(value), changeCommand=lambda *args:self.setUserAttr(nodeAttrName, nodeParamName, *args))
            elif details['type'] == 'FLOAT':
                ctrl = cmds.floatFieldGrp(details['control'], edit=True, value1=float(value), changeCommand=lambda *args:self.setUserAttr(nodeAttrName, nodeParamName, *args))
            elif details['type'] == 'BOOL':
                if type(value) is not bool:
                    value = literal_eval(value.title())
                ctrl = cmds.checkBoxGrp(details['control'], edit=True, value1=value, changeCommand=lambda *args:self.setUserAttr(nodeAttrName, nodeParamName, *args))
            elif details['type'] == 'STRING':
                ctrl = cmds.textFieldGrp(details['control'], edit=True, value1=str(value), changeCommand=lambda *args:self.setUserAttr(nodeAttrName, nodeParamName, *args))
            elif details['type'] == 'ENUM':
                ctrl = cmds.optionMenuGrp(details['control'], edit=True, value=str(value), changeCommand=lambda *args:self.setUserAttr(nodeAttrName, nodeParamName, *args))
            else:
                cmds.warning('[gpuCache][mtoa] Could not make control for param {}.{}'.format(node_type, param_name))
        return ctrl

    def setUserAttr(self, nodeAttrName, nodeParamName, value=None):
        node_type, param_name = nodeParamName.split(':')[:2]

        control = self.attr_ctrls[node_type][param_name].get('control')
        param_type = self.attr_ctrls[node_type][param_name].get('type')
        default = self.attr_ctrls[node_type][param_name].get('default')

        if value is None:
            value = self._getCtrlValue(control, param_type)

        if nodeParamName in self.user_attrs:
            idx = self.user_attrs[nodeParamName].get('idx')
            if value == default:
                # pop this attribute
                status = cmds.removeMultiInstance( '{}[{}]'.format(nodeAttrName, idx) )
        else:
            idxs = cmds.getAttr(nodeAttrName, mi=True) or []
            idx = idxs[-1]+1 if len(idxs) else 0

            cmds.setAttr(nodeAttrName+'[{}].attrName'.format(idx), nodeParamName.split(':')[-1], type="string" )
            cmds.setAttr(nodeAttrName+'[{}].attrNodeType'.format(idx), NODE_TYPES.index(node_type))

        # get the value from the control

        cmds.setAttr(nodeAttrName+'[{}].attrValue'.format(idx), str(value).lower(), type="string")
        if nodeParamName not in self.user_attrs:
            self.user_attrs[nodeParamName] = {'idx':idx, 'value': value}
        else:
            self.user_attrs[nodeParamName]['value'] = value

    def overridesChanged(self, nodeAttr, control, *args):

        enabled = bool(cmds.getAttr(nodeAttr))
        cmds.attrControlGrp(control, edit=True, enable=enabled)
        if enabled:
            # check if aiFrame is connected, if not connect to time1
            if not len(cmds.listConnections(".aiFrame".format(self.nodeName)) or []):
                cmds.connectAttr("time1.outTime", ".aiFrame".format(self.nodeName))

    def overrideFrameNew(self, nodeAttr):

        cmds.setUITemplate('attributeEditorTemplate', pst=True)

        self.aiOverrideFrameCtrl = cmds.attrControlGrp('aiOverrideFrameCtrl', label="Override Frame",
                                                       attribute='.'.join([self.nodeName, 'aiOverrideFrame']))
        self.aiFrameCtrl = cmds.attrControlGrp("aiFrameCtrl", label=" Arnold Frame",
                                               annotation="Set the frame number that is rendered, affects Arnold only, not viewport",
                                               attribute='.'.join([self.nodeName, 'aiFrame']))

        cmds.setUITemplate(ppt=True)

        self.overrideFrameReplace(nodeAttr)

    def overrideFrameReplace(self, nodeAttr):

        cmds.attrControlGrp(self.aiOverrideFrameCtrl, edit=True,
                            changeCommand=lambda *args: self.overridesChanged('.'.join([self.nodeName, 'aiOverrideFrame']), self.aiFrameCtrl, *args),
                            attribute='.'.join([self.nodeName, 'aiOverrideFrame']))
        cmds.attrControlGrp(self.aiFrameCtrl, edit=True, attribute='.'.join([self.nodeName, 'aiFrame']),
                            enable=bool(cmds.getAttr('.'.join([self.nodeName, 'aiOverrideFrame']))))

    def operatorsReplace(self, nodeAttr):
        self._setActiveNodeAttr(nodeAttr)

        for ctrl in self._msgCtrls:
            cmds.deleteUI(ctrl)
        self._msgCtrls = []

        cmds.setUITemplate('attributeEditorTemplate', pushTemplate=True)
        cmds.setParent(self.otherCol)
        attrSize = mu.getAttrNumElements(*nodeAttr.split('.', 1))

        for i in range(attrSize):
            attrName = '{}[{}]'.format(nodeAttr, i)
            attrLabel = 'Operators[{}]'.format(i)
            ctrl = cmds.attrNavigationControlGrp(at=attrName,
                                                 label=attrLabel, cn="createRenderNode -allWithShadersUp \"defaultNavigation -force true -connectToExisting -source %node -destination "+attrName+"\" \"\"")
            self._msgCtrls.append(ctrl) 

        cmds.setUITemplate('attributeEditorTemplate', popTemplate=True)

    def operatorsNew(self, nodeAttr):
        # TODO: move this into AttributeEditorTemplate
        self._setActiveNodeAttr(nodeAttr)

        self._msgCtrls = []
        cmds.setUITemplate('attributeEditorTemplate', pushTemplate=True)

        cmds.frameLayout(label='Operators', collapse=True)
        cmds.columnLayout(adjustableColumn=True)

        cmds.rowLayout(nc=2)
        cmds.text(label='')
        #cmds.text(label='')
        addInputButton = cmds.button(label='Add Operator')

        self.oppopup = cmds.popupMenu(parent=addInputButton, button=1) 
        cmds.popupMenu(self.oppopup, edit=True, postMenuCommand=Callback(aiStandInTemplate.buildOperatorMenu, self.oppopup, nodeAttr))
        cmds.setParent('..') # rowLayout

        cmds.frameLayout(labelVisible=False, collapsable=False)
        self.otherCol = cmds.columnLayout(adjustableColumn=True)
        attrSize = mu.getAttrNumElements(*nodeAttr.split('.', 1))

        for i in range(attrSize):
            attrName = '{}[{}]'.format(nodeAttr, i)
            attrLabel = 'Inputs[{}]'.format(i)
            ctrl = cmds.attrNavigationControlGrp(at=attrName,
                                                 label=attrLabel, cn="createRenderNode -allWithShadersUp \"defaultNavigation -force true -connectToExisting -source %node -destination "+attrName+"\" \"\"")
            self._msgCtrls.append(ctrl)

        cmds.setParent('..') # columnLayout
        cmds.setParent('..') # frameLayout

        cmds.setParent('..') # columnLayout
        cmds.setParent('..') # frameLayout

        cmds.setParent('..') # columnLayout
        cmds.setParent('..') # frameLayout
        cmds.setUITemplate('attributeEditorTemplate', popTemplate=True)

    def overridesChanged(self, nodeAttr, control, *args):

        enabled = bool(cmds.getAttr(nodeAttr))
        cmds.attrControlGrp(control, edit=True, enable=enabled)

    def overrideFrameNew(self, nodeAttr):

        cmds.setUITemplate('attributeEditorTemplate', pst=True)

        self.aiOverrideFrameCtrl = cmds.attrControlGrp('aiOverrideFrameCtrl', label="Override Frame",
                                                       attribute='.'.join([self.nodeName, 'aiOverrideFrame']))
        self.aiFrameCtrl = cmds.attrControlGrp("aiFrameCtrl", label=" Arnold Frame",
                                               annotation="Set the frame number that is rendered, affects Arnold only, not viewport",
                                               attribute='.'.join([self.nodeName, 'aiFrame']))

        cmds.setUITemplate(ppt=True)

        self.overrideFrameReplace(nodeAttr)

    def overrideFrameReplace(self, nodeAttr):

        cmds.attrControlGrp(self.aiOverrideFrameCtrl, edit=True,
                            changeCommand=lambda *args: self.overridesChanged('.'.join([self.nodeName, 'aiOverrideFrame']), self.aiFrameCtrl, *args),
                            attribute='.'.join([self.nodeName, 'aiOverrideFrame']))
        cmds.attrControlGrp(self.aiFrameCtrl, edit=True, attribute='.'.join([self.nodeName, 'aiFrame']),
                            enable=bool(cmds.getAttr('.'.join([self.nodeName, 'aiOverrideFrame']))))

    def setup(self):
        self.abcInfoPath = ''
        self.inspectAlembicPath = ''
        self.abcItems = []
        self.user_attrs = {}
        self.attr_ctrls = {}
        self.currentItem = None

        self.commonShapeAttributes()
        self.beginLayout("Translator Options", collapse=False)
        self.addControl("aiMakeInstance", label="Make Instance", annotation='Create instances where possible, to save geometry duplication')
        self.addControl("aiExcludeXform", label="Exclude Xform", annotation='Exclude transform information from cache')
        self.addControl("aiFlipV", label="Flip V", annotation='Flip UVs in V direction (when using caches with inverted UVs)')
        self.addControl("aiVisibilityIgnore", label="Visibility Ignore", annotation='Ignore Visibility channels on geo in the archive')
        self.addControl("aiExpandHidden", label="Expand Hidden", annotation='Force creation of hidden geometry')
        self.addSeparator()

        self.addControl("aiVelocityIgnore", label="Velocity Ignore", annotation='Ignore Velocity attributes on geometry')
        self.addControl("aiVelocityScale", label="Velocity Scale", annotation='Scale the velocity')
        self.addSeparator()
        self.addCustom("aiOverrideFrame", self.overrideFrameNew, self.overrideFrameReplace)
        self.addSeparator()

        self.addControl("aiRadiusAttribute", label="Radius Attribute", annotation='Set the attribute to read for getting the width/radius of points/curves')
        self.addControl("aiRadiusDefault", label="Radius Default",  annotation='Default radius if not set on geometry')
        self.addControl("aiRadiusScale", label="Radius Scale", annotation='Scale the radius')
        self.addSeparator()
        self.addControl("aiNamespace", label="Namespace", annotation='Create nodes in this namespace if present.')
        self.addControl("aiNameprefix", label="Name Prefix", annotation='Optional prefix to add to all nodes created by the procedural.')
        self.addSeparator()
        self.addControl("aiUserOptions", label="User Options")
        self.endLayout()

        self.beginLayout("Alembic Contents", collapse=False)
        self.addCustom('aiInfo', self.abcInfoNew, self.abcInfoReplace)
        self.endLayout()
        self.addCustom("operators", self.operatorsNew, self.operatorsReplace)
        self.addSeparator()


templates.registerTranslatorUI(gpuCacheDescriptionTemplate, "gpuCache", "alembic")

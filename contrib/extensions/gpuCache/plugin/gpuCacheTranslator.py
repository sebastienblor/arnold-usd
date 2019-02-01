import mtoa.ui.ae.templates as templates
from mtoa.ui.ae.templates import AttributeTemplate, registerTranslatorUI
from mtoa.ui.ae import aiStandInTemplate

from mtoa.ui.qt.Qt import QtCore
from mtoa.ui.qt.Qt import QtGui
from mtoa.ui.qt.Qt import QtWidgets
from mtoa.ui.qt.Qt import shiboken
from mtoa.ui.qt import toQtObject

import mtoa.melUtils as mu
import mtoa.utils as utils
from mtoa.callbacks import *
import maya.OpenMayaUI as OpenMayaUI
import maya.cmds as cmds
import maya.mel as mel
import re
import os
import os.path
from ast import literal_eval

from mtoa.ui.procview.ProceduralTreeView import ProceduralTreeView, ProceduralTreeModel, ProceduralItem
from mtoa.ui.procview.ProceduralWidgets import ProceduralPropertiesPanel
from mtoa.ui.procview.ProceduralTransverser import ProceduralTransverser, \
                           PROC_PATH, PROC_NAME, PROC_PARENT, PROC_VISIBILITY, \
                           PROC_INSTANCEPATH, PROC_ENTRY_TYPE, PROC_IOBJECT, \
                           OVERRIDE_OP, DISABLE_OP


from alembic import Abc, AbcGeom

from alembic.AbcCoreAbstract import *
from alembic.Abc import *
from alembic.Util import *
from alembic.AbcGeom import *

from arnold import *

NODE_TYPES = ['polymesh', 'curves', 'nurbs', 'points']
TYPE_MATCH = {IPolyMesh: "polymesh", ISubD: "polymesh", IXform: "xform",
              INuPatch: "nurbs", ICurves: "curves", IPoints: "points",
              ICamera: "persp_camera"}

BLACK_LIST_PARAMS = ['id', 'visibility', 'name', 'sidedness', 'matrix',
                     'motion_start', 'motion_end', 'shader', 'disp_map',
                     'vidxs', 'vlist', 'nsides', 'uvidxs', 'shidxs',
                     'nlist', 'uvlist', 'crease_idxs', 'crease_sharpness',
                     'use_shadow_group', 'use_light_group', 'degree_u',
                     'degree_v', 'transform_type']

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


VISIBILITY = ['differed', 'hidden', 'visible']

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


class AlembicTransverser(ProceduralTransverser):
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

    def getRootObjectInfo(self, node):
        return self.impl.getRootObjectInfo(node)
        
    def dir(self, *args):
        return self.impl.dir(*args)

    def getNodeTypes(self, iObj):
        node_types = []
        children = self.visitObject(iObj)

        for child in children:
            if child[PROC_ENTRY_TYPE] not in node_types:
                node_types.append(child[PROC_ENTRY_TYPE])

        return node_types


class AlembicTransverserImpl(object):

    # We must extract the alembic-specific part from the common one that will be used for all procedural
    # So far it looks like getArcivePath, getArchive, visitObject, getObjectInfo, getRootObjectInfo, are alembic-specific,
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
            abc_items += self.visitObject(child, obj_data[PROC_PATH], obj_data[PROC_VISIBILITY])

        return abc_items

    def getObjectInfo(self, iobject):
        path = iobject.getFullName()
        name = iobject.getName()
        parent = iobject.getParent().getFullName()
        instancedPath = iobject.instanceSourcePath()

        entity_type = abcToArnType(iobject)

        visibility = VISIBILITY[int(AbcGeom.GetVisibility(iobject))+1]

        return [path, name, parent, visibility, instancedPath, entity_type, iobject]

    def getRootObjectInfo(self, node):
        abc_file = self.getArchive(node) 
        return self.getObjectInfo(abc_file.getTop())
        
    def dir(self, iobject):
        children = []
        for ich in iobject.children:
            children.append(self.getObjectInfo(ich))

        return children

class gpuCacheDescriptionTemplate(templates.ShapeTranslatorTemplate):

    def __currentWidget(self, pySideType=QtWidgets.QWidget):
        """Cast and return the current widget."""
        # Get the current widget Maya name.
        currentWidgetName = cmds.setParent(query=True)
        return toQtObject(currentWidgetName, pySideType)

    def abcInfoNew(self, nodeAttr):
        self.currentItem = None
        currentWidget = self.__currentWidget()

        abcTransverser = AlembicTransverser()

        self.tree = ProceduralTreeView(abcTransverser, currentWidget)
        self.tree.setObjectName("abcTreeWidget")
        currentWidget.layout().addWidget(self.tree)

        # now add the preperties panel
        self.properties_panel = ProceduralPropertiesPanel(abcTransverser, currentWidget)
        currentWidget.layout().addWidget(self.properties_panel)

        self.tree.itemSelected.connect(self.showItemProperties)
        self.abcInfoReplace(nodeAttr)

        fileAttr = self.nodeName + ".cacheFileName"
        cmds.scriptJob(attributeChange=[fileAttr, self.updateAlembicFile],
                       replacePrevious=True, parent=self.inspectAlembicPath)

    def abcInfoReplace(self, nodeAttr):
        self.tree.setCurrentNode(self.nodeName)
        # self.properties_panel.setItem(self.nodeName, None)

    @QtCore.Slot(str, object)
    def showItemProperties(self, node, items):
        for item in items:
            self.properties_panel.setItem(node, item)

    def updateAlembicFile(self):
        # clear the cache
        self.abcItems = []
        self.abcPath = cmds.getattr(self.nodeName + ".cacheFileName", type="string")
        self.abcInfoReplace(self.nodeName + ".cacheFileName")

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

    def _createControl(self, node_type, param_name, param_type, param_entry):
        control_name = '{}_{}_ctrl'.format(node_type,param_name)
        control = None
        label = param_name.replace('_', ' ').title()
        if param_type in [AI_TYPE_INT, AI_TYPE_BYTE, AI_TYPE_UINT]:
            control = cmds.intFieldGrp(control_name, label=label)
        elif param_type is AI_TYPE_FLOAT:
            control = cmds.floatFieldGrp(control_name, label=label)
        elif param_type is AI_TYPE_BOOLEAN:
            control = cmds.checkBoxGrp(control_name, label=label)
        elif param_type is AI_TYPE_STRING:
            control = cmds.textFieldGrp(control_name, label=label)
        elif param_type is AI_TYPE_ENUM:
            enum_ctrl = cmds.optionMenuGrp(control_name, label=label)
            # populate the options
            i = 0
            t = True
            while t is not None:
                t = AiEnumGetString(AiParamGetEnum(param_entry), i)
                if t:
                    cmds.menuItem( label=t )
                i += 1
            control = enum_ctrl
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

        cmds.setParent('..')  # columnLayout
        cmds.setParent('..')  # frameLayout

        cmds.setParent('..')  # columnLayout
        cmds.setParent('..')  # frameLayout

        cmds.setParent('..')  # columnLayout
        cmds.setParent('..')  # frameLayout
        cmds.setUITemplate('attributeEditorTemplate', popTemplate=True)

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
        self.addCustom("operators", self.operatorsNew, self.operatorsReplace)
        self.endLayout()
        self.addSeparator()
        self.beginLayout("Alembic Overrides", collapse=False)
        self.addControl("aiPullUserParams", label="Enable Overrides", annotation='Enable to override the attributes found in the archive')
        self.addCustom('aiNodeAttrs', self.userAttrsNew, self.userAttrsReplace)
        self.endLayout()


templates.registerTranslatorUI(gpuCacheDescriptionTemplate, "gpuCache", "alembic")

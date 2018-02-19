import mtoa.ui.ae.templates as templates
from mtoa.ui.ae.templates import AttributeTemplate, registerTranslatorUI
import maya.cmds as cmds
import os
import os.path
from ast import literal_eval

from alembic.AbcCoreAbstract import *
from alembic.Abc import *
from alembic.Util import *

from arnold import *

NODE_TYPES = ['polymesh', 'curves', 'nurbs', 'points']

def ArnoldUniverseOnlyBegin():
   if not AiUniverseIsActive():
      AiBegin()
      return True

   return False


def ArnoldUniverseEnd():
   if AiUniverseIsActive():
      if AiRendering():
         AiRenderInterrupt()
      if AiRendering():
         AiRenderAbort()
      AiEnd()

class gpuCacheDescriptionTemplate(templates.ShapeTranslatorTemplate):

    def selectGeomPath(self):
        selectedGeomPath = cmds.textScrollList(self.abcInfoPath, q=True, si=True)
        if selectedGeomPath:
            selectedGeomPath = selectedGeomPath[0]
        for key in self.abcItems:
            if self.abcItems[key] == selectedGeomPath:
                cmds.setAttr('{}.cacheGeomPath'.format(self.nodeName), key, type='string')

    def abcInfoNew(self, nodeName) :
        cmds.rowLayout(nc=2)
        cmds.text(label='')
        self.inspectAlembicPath = cmds.button(align="center", label='Inspect Alembic File', command=lambda *args: self.inspectAlembic())
        cmds.setParent('..') # rowLayout
        self.abcInfoPath = cmds.textScrollList(height=300,allowMultiSelection=False, doubleClickCommand=self.selectGeomPath)
        self.abcInfoReplace(nodeName)

        fileAttr = self.nodeName + ".cacheFileName"
        cmds.scriptJob(attributeChange=[fileAttr,self.updateAlembicFile],
                     replacePrevious=True, parent=self.inspectAlembicPath)

    def updateAlembicFile(self):
        cmds.textScrollList(self.abcInfoPath, edit=True, visible=False)
        cmds.button(self.inspectAlembicPath, edit=True, visible=True)
        cmds.textScrollList(self.abcInfoPath, edit=True, removeAll=True)

    def visitObject(self, iObj, iIndent = 0 ):

        path = str(iObj)
        indent = "-" * iIndent

        strLine = "%s%s" % ( indent,  path )
        cmds.textScrollList(self.abcInfoPath, edit=True, append=str(strLine))
        geomPath = path.replace('/', '|')
        self.abcItems[geomPath] = strLine

        iIndent += 2
        for child in iObj.children:
            self.visitObject( child, iIndent )

    def inspectAlembic(self):
        filenameAttr = self.nodeName + '.cacheFileName'
        filename = cmds.getAttr(filenameAttr)
        cmds.textScrollList(self.abcInfoPath, edit=True, removeAll=True)
        cmds.textScrollList(self.abcInfoPath, edit=True, visible=True)
        cmds.button(self.inspectAlembicPath, edit=True, visible=False)
        if not os.path.exists(filename):
            return

        iArchive = IArchive(str(filename))
        self.visitObject( iArchive.getTop() )

        allItems = cmds.textScrollList(self.abcInfoPath, query=True, allItems=True) or []
        
        geomPathAttr = self.nodeName + '.cacheGeomPath'
        geomPath = cmds.getAttr(geomPathAttr)
        
        if geomPath in self.abcItems:
            cmds.textScrollList(self.abcInfoPath, edit=True, selectItem=self.abcItems[geomPath])

    def abcInfoReplace(self, nodeName) :
        self.abcItems = {}
        cmds.textScrollList(self.abcInfoPath, edit=True, visible=False)
        cmds.button(self.inspectAlembicPath, edit=True, visible=True)
        cmds.textScrollList(self.abcInfoPath, edit=True, removeAll=True)

    def populateParams(self, node_type):
        # iterate over params
        node_entry = AiNodeEntryLookUp(node_type)
        parmaIter = AiNodeEntryGetParamIterator(node_entry)
        while not AiParamIteratorFinished(parmaIter):
            this_param = AiParamIteratorGetNext(parmaIter)
            param_name = AiParamGetName(this_param)
            param_type = AiParamGetType(this_param)

            # check this param is not in the black list
            if param_name not in ['name', 'visibility', 'sidedness', 'matrix', 'motion_start', 'motion_end']:
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
        if param_type in [AI_TYPE_INT, AI_TYPE_BYTE, AI_TYPE_UINT]:
            value = param_default.contents.INT
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

    def userAttrsNew(self, nodeAttrName):
        AiUniverseCreated = ArnoldUniverseOnlyBegin()
        for node_type in NODE_TYPES:
            layout = cmds.frameLayout(label=node_type.title(), collapse=True)
            self.attr_ctrls[node_type] = {}
            # self.populateParams(node)
            node_entry = AiNodeEntryLookUp(node_type)
            parmaIter = AiNodeEntryGetParamIterator(node_entry)
            param_default = None
            while not AiParamIteratorFinished(parmaIter):
                this_param = AiParamIteratorGetNext(parmaIter)
                param_name = AiParamGetName(this_param)
                param_type = AiParamGetType(this_param)
                # special catch for polymesh:smoothing
                if node_type == "polymesh" and param_name == "smoothing":
                    param_default = True
                else:
                    param_default = self._getDefaultValue(this_param, param_type)

                # check this param is not in the black list
                if param_name not in ['name', 'visibility', 'sidedness', 'matrix', 'motion_start', 'motion_end']:
                    if param_type not in [AI_TYPE_ARRAY]:
                        self.attr_ctrls[node_type][param_name] = {'control':self._createControl(node_type, param_name, param_type, this_param),
                                                                  'type': AiParamGetTypeName(param_type),
                                                                  'default': param_default}
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

    def setup(self):
        self.abcInfoPath = ''
        self.inspectAlembicPath = ''
        self.abcItems = {}
        self.user_attrs = {}
        self.attr_ctrls = {}

        self.commonShapeAttributes()
        self.addSeparator()
        self.addControl("aiUserOptions", label="User Options")
        self.beginLayout("Alembic Contents", collapse=False)
        self.addCustom('aiInfo', self.abcInfoNew, self.abcInfoReplace)
        self.endLayout()
        self.addSeparator()
        self.beginLayout("Alembic Overrides", collapse=False)
        self.addControl("aiPullUserParams", label="Pull User Params")
        self.addCustom('aiNodeAttrs', self.userAttrsNew, self.userAttrsReplace)
        # self.addControl('aiNodeAttrs', label="Set User Params")
        self.endLayout()

templates.registerTranslatorUI(gpuCacheDescriptionTemplate, "gpuCache", "gpuCacheTranslator")

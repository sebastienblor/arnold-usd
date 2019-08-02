
from mtoa.ui.qt.Qt import QtWidgets
from mtoa.ui.qt.Qt import QtGui
from mtoa.ui.qt.Qt import QtCore
from mtoa.ui.qt import MtoAStyle, setStaticSize, clearWidget
from mtoa.ui.qt.widgets import *
import mtoa.ui.ae.utils as utils
import time

import maya.cmds as cmds

from mtoa.ui.procview.ProceduralTransverser import EXP_REGEX, PROC_PATH, PROC_NAME, PROC_PARENT, PROC_VISIBILITY, \
                            PROC_INSTANCEPATH, PROC_ENTRY, PROC_ENTRY_TYPE, PROC_IOBJECT, \
                            OVERRIDE_OP, DISABLE_OP, COLLECTION_OP, MERGE_OP, \
                            SWITCH_OP, INCLUDEGRAPH_OP, MATERIALX_OP, \
                            NODE_TYPES, PARAM_TYPE, PARAM, OP, VALUE, INDEX, OPERATOR, \
                            DATA_PARAM_TYPE, DATA_DEFAULT_VALUE, DATA_IS_ARRAY, DATA_ENUM_VALUES,\
                            DISP_MAP, SHADER


class SetParametersPanel(QtWidgets.QFrame):

    def __init__(self, transverser, parent=None):
        super(SetParametersPanel, self).__init__(parent)

        self.node = None
        self.transverser = transverser
        self.paramDict = {}
        self.layout = QtWidgets.QVBoxLayout(self)

        # AddOverride Button
        self.toolBar = QtWidgets.QFrame()
        self.toolBar.setLayout(QtWidgets.QHBoxLayout())
        self.layout.addWidget(self.toolBar)

        self.addOverrideButton = QtWidgets.QPushButton("Add Assignment")
        self.overrideMenu = QtWidgets.QMenu()
        self.addOverrideButton.setMenu(self.overrideMenu)
        self.rootMenus = []

        self.toolBar.layout().addWidget(self.addOverrideButton)
        self.overrideMenu.triggered.connect(self.setOverrideFromMenu)

        self.overridesPanel = QtWidgets.QFrame()
        self.overridesPanel.setLayout(QtWidgets.QVBoxLayout())
        self.layout.addWidget(self.overridesPanel)

        self.getParams()
        self.addOverrideMenu()

    def setOverrideFromMenu(self, action):
        param = action.text()
        self.setNewOverride(param)

    def setNewOverride(self, param):
        op = self.node
        if param == "custom":
            self.addCustomOverride()
        else:
            if param == "displacement":
                param = "disp_map"
            value = self.getDefaultValue(param)
            self.setOverride(param, "=", value, operator=op)
            self.refresh()

    def getDefaultValue(self, param):
        default_value = ""
        param_data = self.getParamData(param)
        if param_data is not None:
            default_value = param_data[DEFAULT_VALUE]
        return default_value

    def addCustomOverride(self):
        operator = self.node
        param_name = self.getCustomParamName()
        self.setOverride(param_name, "=", "1", TYPES_DICT['int'], True, operator=operator)
        self.refresh()

    def getCustomParamName(self):
        customParam = self.transverser.getCustomParamName(self.node) or "myParam"
        return customParam

    def addOverrideMenu(self):

        if not self.overrideMenu.isEmpty():
            self.overrideMenu.clear()
            for menu in self.rootMenus:
                menu.deleteLater()
            self.rootMenus = []

        self.overrideMenu.addAction("shader")
        self.overrideMenu.addAction("displacement")
        self.overrideMenu.addSeparator()

        for node_type, params in sorted(self.paramDict.items()):
            if node_type == 'hidden':
                continue

            parent_menu = self.overrideMenu
            for sub_menu in self.rootMenus:
                if sub_menu.title() == node_type:
                    parent_menu = sub_menu

            for param, data in sorted(params.items()):
                # check if a root item exists
                if parent_menu == self.overrideMenu:
                    parent_menu = self.overrideMenu.addMenu(node_type)
                self.rootMenus.append(parent_menu)
                if parent_menu:
                    parent_menu.addAction(param)

        self.overrideMenu.addAction("custom")

    def setNode(self, node):
        changed = False
        if self.node != node and node and cmds.objExists(node):
            self.node = node
            changed = True
        self.refresh()
        return changed

    def refresh(self):
        clearWidget(self.overridesPanel)

        if self.node:
            for override in self.getOverrides():
                widget = self.addOverrideGUI(*override)
                widget.overrideTriggered.connect(self.setNewOverride)

    def getParams(self):
        if self.transverser:
            self.paramDict = self.transverser.getParams(NODE_TYPES)

    def getOverrides(self):

        def getParmInList(param, param_list):
            for i, p in enumerate(param_list):
                if p[PARAM] == param:
                    return i
            return -1

        op = self.node
        overrides = []
        if op:
            for c in cmds.getAttr('{}.assignment'.format(self.node), multiIndices=True) or []:
                ass_str = cmds.getAttr("{}.assignment[{}]".format(op, c))
                enabled = cmds.getAttr("{}.enableAssignment[{}]".format(op, c))
                mat = EXP_REGEX.match(ass_str)
                if mat:
                    param = mat.group('param')
                    data = list(mat.groups())
                    data.append(c)
                    data.append(op)
                    data.append(enabled)
                    # get if this parameter is already in the list, if so replace it with this one
                    idx = getParmInList(param, overrides)
                    if idx != -1:
                        overrides[idx] = data
                    else:
                        overrides.append(data)  # split to param, op, VALUE

        return overrides

    def addOverrideGUI(self, param_type, param, op, value, index, operator, enabled):

        new_widget = MtoAOperatorOverrideWidget(param_type, param, op, value, self.paramDict, self, enabled, True)

        new_widget.index = index
        new_widget.operator = operator
        new_widget.deleteMe.connect(self.removeOverrideWidget)

        if param_type == AI_TYPE_NODE or param in [SHADER, DISP_MAP]:
            new_widget.controlWidget.control.connectionButtonClicked.connect(lambda: self.newShadingNode(param))

        # param, operation, value, param_type, custom, index, operator
        new_widget.valueChanged[str, str, str, int, bool, int, str, bool].connect(self.setOverride)
        new_widget.valueChanged[str, str, int, int, bool, int, str, bool].connect(self.setOverride)
        new_widget.valueChanged[str, str, bool, int, bool, int, str, bool].connect(self.setOverride)
        new_widget.valueChanged[str, str, float, int, bool, int, str, bool].connect(self.setOverride)
        # add widget
        self.overridesPanel.layout().addWidget(new_widget)
        return new_widget

    def newShadingNode(self, param):
        callback = "from mtoa.ui.procview.ProceduralTransverser import ProceduralTransverser;"
        callback += "ProceduralTransverser.setOverride(None, None, '{operator}', '{param}', '=', '%node', {param_type})"

        param_data = self.getParamData(param)
        callback = callback.format(operator=self.node, param_type=AI_TYPE_NODE,
                                   param=param)
        mel.eval("createRenderNode -all \"python(\\\"" + callback + "\\\")\" \"\"")

    def removeOverrideWidget(self, widget):
        index = widget.index
        operator = self.node
        self.removeOverride(operator, index)

    def removeOverride(self, operator, index):
        removed = self.transverser.deleteOverride(operator, index)
        if removed:
            if len(self.getOverrides()) == 0:
                self.transverser.deleteOperator(operator)
            self.refresh()

    def getParamData(self, param):
        for node_type, params in self.paramDict.items():
            if param in params:
                return params[param]

        return None

    def setOverride(self, param, op, value, param_type=None, custom=False, index=-1, operator=None, enabled=True):
        if not operator:
            return

        param_data = self.getParamData(param)
        if param_data:
            if not param_type:
                param_type = param_data[DATA_PARAM_TYPE]

        return self.transverser.setOverride(None, None, operator, param, op, value, param_type, custom, enabled, index)

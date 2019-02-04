
from mtoa.ui.qt.Qt import QtWidgets
from mtoa.ui.qt.Qt import QtGui
from mtoa.ui.qt.Qt import QtCore
from mtoa.ui.qt import setStaticSize, clearWidget
from mtoa.ui.qt.widgets import *

import maya.cmds as cmds

from mtoa.ui.procview.ProceduralTransverser import PROC_PATH, PROC_NAME, PROC_PARENT, PROC_VISIBILITY, \
                            PROC_INSTANCEPATH, PROC_ENTRY_TYPE, PROC_IOBJECT, \
                            OVERRIDE_OP, DISABLE_OP,\
                            PARM, OP, VALUE, INDEX

OPERATORS = cmds.arnoldPlugins(listOperators=True) or []


class ProceduralPropertiesPanel(QtWidgets.QFrame):
    """SubClass properties panel for displying properties of an object"""

    propertyChanged = QtCore.Signal(str, object)
    GEAR_ICON = QtGui.QPixmap(":/gear.png")
    SHADER_ICON = QtGui.QPixmap(":/out_blinn.png")
    DISP_ICON = QtGui.QPixmap(":/out_displacementShader.png")

    def __init__(self, transverser, parent=None):
        super(ProceduralPropertiesPanel, self).__init__(parent)

        self.node = None
        self.item = None
        self.object = None
        self.paramDict = {}
        self.transverser = transverser

        self.layout = QtWidgets.QVBoxLayout(self)
        self.setLayout(self.layout)
        self.object_label = QtWidgets.QLabel("Select Item")
        self.layout.addWidget(self.object_label)

        # AddOverride Button
        self.toolBar = QtWidgets.QFrame()
        self.toolBar.setLayout(QtWidgets.QHBoxLayout())
        self.layout.addWidget(self.toolBar)

        self.addOverrideButton = QtWidgets.QPushButton("Add Parameter")
        self.addOverrideButton.setIcon(self.GEAR_ICON)
        self.overrideMenu = QtWidgets.QMenu()
        self.addOverrideButton.setMenu(self.overrideMenu)
        self.rootMenus = []

        self.toolBar.layout().addWidget(self.addOverrideButton)

        self.overrideMenu.triggered.connect(self.setOverrideFromMenu)

        self.addShaderButton = QtWidgets.QPushButton("Add Shader")
        self.addShaderButton.setIcon(self.SHADER_ICON)
        self.toolBar.layout().addWidget(self.addShaderButton)

        self.addShaderButton.clicked.connect(self.addShader)

        self.addDisplacementButton = QtWidgets.QPushButton("Add Displacement")
        self.addDisplacementButton.setIcon(self.DISP_ICON)
        self.toolBar.layout().addWidget(self.addDisplacementButton)

        self.addDisplacementButton.clicked.connect(self.addDisplacement)

        self.addOperatorButton = QtWidgets.QPushButton("Add Operator")
        self.addOperatorButton.setIcon(self.GEAR_ICON)
        self.toolBar.layout().addWidget(self.addOperatorButton)
        self.operatorMenu = QtWidgets.QMenu()
        self.addOperatorButton.setMenu(self.operatorMenu)

        self.operatorMenu.triggered.connect(self.addOperator)

        self.toolBar.layout().insertStretch(-1)

        self.shadingPanel = QtWidgets.QFrame()
        self.shadingPanel.setLayout(QtWidgets.QVBoxLayout())
        self.shadingPanel.layout().setContentsMargins(0, 0, 0, 0)

        # shader override - HIDDEN BY DEFAULT
        self.shadingWidgets = {}
        self.shaderOverrideWidget = MtoANodeConnectionWidget("Shader")
        self.shaderOverrideWidget.valueChanged.connect(self.setShader)
        self.shaderOverrideWidget.setVisible(False)
        self.shaderOverrideWidget.nodeDisconnected.connect(self.disconnectShader)
        self.shadingPanel.layout().addWidget(self.shaderOverrideWidget)
        self.shadingWidgets['shader'] = self.shaderOverrideWidget

        # displacement override - HIDDEN BY DEFAULT
        self.dispOverrideWidget = MtoANodeConnectionWidget("Displacement")
        self.dispOverrideWidget.valueChanged.connect(self.setDisplacement)
        self.dispOverrideWidget.setVisible(False)
        self.shadingPanel.layout().addWidget(self.dispOverrideWidget)
        self.shadingWidgets['disp_map'] = self.dispOverrideWidget

        self.layout.addWidget(self.shadingPanel)

        self.overridesPanel = QtWidgets.QFrame()
        self.overridesPanel.setLayout(QtWidgets.QVBoxLayout())
        self.layout.addWidget(self.overridesPanel)

    def addOperatorMenu(self):
        self.operatorMenu.clear()

        # get list of operators in scene
        # for operator in OPERATORS:
        #     opNodes = cmds.ls(type=operator) or []
        #     for opNode in opNodes:
        #         if opNode == nodeName:
        #             continue
        #         self.operatorMenu.addAction(opNode)

        for op in OPERATORS:
            self.operatorMenu.addAction(op)

    def addOperator(self, action):
        op_name = action.text()
        # check if op exists, otherwise create it
        if not cmds.objExists(op_name) and op_name in OPERATORS:
            op = self.transverser.createOperator(self.node, self.item, op_name)

    def addOverrideMenu(self):

        if not self.overrideMenu.isEmpty():
            self.overrideMenu.clear()
            for menu in self.rootMenus:
                menu.deleteLater()
            self.rootMenus = []

        for node_type, params in sorted(self.paramDict.items()):
            if node_type != 'hidden':
                parent_menu = self.overrideMenu
                print node_type
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

    def setOverrideFromMenu(self, action):
        op = self.getOverrideOperator()
        param = action.text()
        value = self.getDefaultValue(param)
        self.setOverride(param, "=", value)
        self.refresh()

    def getDefaultValue(self, param):
        default_value = ""
        param_data = self.getParamData(param)
        if param_data:
            default_value = param_data[DEFAULT_VALUE]
        return default_value

    def setTransverser(self, transverser):
        self.transverser = transverser

    def setItem(self, node, item):
        self.node = node
        self.item = item
        if item:
            self.object = item.data
            self.object_label.setText(item.getName())
            self.getParams()
            self.addOverrideMenu()
            self.addOperatorMenu()

        self.refresh()
        # Tell the transverser that the selection has changed.
        # Let's pass the data as an array in case we end up supporting multi-selection
        if self.transverser:
            self.transverser.selectionChanged(node, [item.data]) 

    def setShader(self, shader):
        self.shaderOverrideWidget.setNode(shader, False)
        self.shaderOverrideWidget.setVisible(True)
        self.setNodeParam("shader", shader)

    def disconnectShader(self):
        pass

    def setDisplacement(self, disp):
        self.setNodeParam("disp_map", disp)

    def addShader(self):
        shader = 'lambert1'
        self.setShader(shader)

    def addDisplacement(self):
        pass

    def getOverrideOperator(self, create=True):
        ops = self.transverser.getOperators(self.node, self.item.getName(), OVERRIDE_OP)
        if not len(ops) and create:
            op = self.transverser.createOperator(self.node, self.item, OVERRIDE_OP)
        else:
            op = ops[0]
        return op

    def getInheritedOverrideOperator(self):
        return self.item.getOverridesOp()

    def setNodeParam(self, param, node):
        ops = self.getOverrideOperator()
        self.setOverride(param, "=", node)

    def getParams(self):
        data = self.getData(self.item)
        if not data:
            return

        node_types = self.transverser.getNodeTypes(data[PROC_IOBJECT])
        self.paramDict = self.transverser.getParams(node_types)

    def resetShadingWidgets(self):
        for widget in self.shadingWidgets.values():
            widget.disconnectNode()

    def getData(self, item):
        if item:
            if item.data:
                return item.data
            else:
                return self.getData(item.parent())

    def refresh(self):
        clearWidget(self.overridesPanel)
        self.shaderOverrideWidget.setVisible(False)
        self.dispOverrideWidget.setVisible(False)

        self.resetShadingWidgets()
        if self.item:
            data = self.getData(self.item)
            if data:
                for override in self.transverser.getOverrides(self.node, data[PROC_PATH]):
                    # FIXME what if the user wants to connect a shader from inside the procedural?
                    if override[PARM] in ["shader", "disp_map"]:
                        # set the shader slot
                        node = override[VALUE].replace("'", "").replace('"', "")
                        self.shadingWidgets[override[PARM]].setNode(node, False)
                        self.shadingWidgets[override[PARM]].setVisible(True)
                    else:
                        self.addOverrideGUI(*override)

    def getProperties(self, obj):
        pass

    def getOverrides(self):
        data = self.getData(self.item)
        if not data:
            return
        return self.transverser.getOverrides(self.node, data[PROC_PATH])

    def addOverride(self):
        op = self.getOverrideOperator()
        self.setOverride(None, "=", None)
        self.refresh()

    def addOverrideGUI(self, param, op, value, index=-1):
        new_widget = MtoAOperatorOverrideWidget(self.overridesPanel)
        new_widget.index = index
        new_widget.deleteMe.connect(self.removeOverride)
        new_widget.populateParams(self.paramDict)
        # set the widget
        new_widget.setParam(param, self.paramDict)
        new_widget.setOperation(op)
        new_widget.setValue(value)

        new_widget.valueChanged[str, str, str, int].connect(self.setOverride)
        new_widget.valueChanged[str, str, int, int].connect(self.setOverride)
        new_widget.valueChanged[str, str, bool, int].connect(self.setOverride)
        new_widget.valueChanged[str, str, float, int].connect(self.setOverride)
        # add widget
        self.overridesPanel.layout().addWidget(new_widget)
        return new_widget

    def removeOverride(self, widget):
        index = widget.index
        data = self.getData(self.item)
        if not data:
            return
        removed = self.transverser.deleteOverride(self.node, data[PROC_PATH], index)
        if removed:
            if len(self.getOverrides()) == 0:
                self.transverser.deleteOperator(self.node, data[PROC_PATH], OVERRIDE_OP)
            self.refresh()

    def getParamData(self, param):
        for node_type, params in self.paramDict.items():
            if param in params:
                return params[param]

        return None

    @QtCore.Slot(str, str, str, int)
    @QtCore.Slot(str, str, int, int)
    @QtCore.Slot(str, str, bool, int)
    @QtCore.Slot(str, str, float, int)
    def setOverride(self, param, op, value, index=-1):
        if not param and not value:
            param = "NEWOVERRIDE"
            value = "''"
        param_type = None
        is_array = False
        data = self.getData(self.item)
        if not data:
            return

        param_data = self.getParamData(param)
        if param_data:
            param_type = param_data[PARAM_TYPE]
            is_array = param_data[IS_ARRAY]

        return self.transverser.setOverride(self.node, data[PROC_PATH], param, op, value, param_type, is_array, index)

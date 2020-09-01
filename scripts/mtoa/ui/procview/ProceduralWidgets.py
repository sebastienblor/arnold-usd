
from mtoa.ui.qt.Qt import QtWidgets
from mtoa.ui.qt.Qt import QtGui
from mtoa.ui.qt.Qt import QtCore
from mtoa.ui.qt import MtoAStyle, setStaticSize, clearWidget
from mtoa.ui.qt.widgets import *
from mtoa.ui.qt.treeView import *
import mtoa.ui.ae.utils as utils
import time

import maya.cmds as cmds

from mtoa.ui.procview.ProceduralTransverser import PROC_PATH, PROC_NAME, PROC_PARENT, PROC_VISIBILITY, \
                            PROC_INSTANCEPATH, PROC_ENTRY, PROC_ENTRY_TYPE, PROC_IOBJECT, \
                            OVERRIDE_OP, DISABLE_OP, COLLECTION_OP, MERGE_OP, \
                            SWITCH_OP, INCLUDEGRAPH_OP, MATERIALX_OP, \
                            NODE_TYPES, PARAM_TYPE, PARAM, OP, VALUE, INDEX, OPERATOR, ENABLED,\
                            DATA_PARAM_TYPE, DATA_DEFAULT_VALUE, DATA_IS_ARRAY, DATA_ENUM_VALUES,\
                            DISP_MAP, SHADER, PROCEDURAL_NODES

OPERATORS = cmds.arnoldPlugins(listOperators=True) or []


class OperatorTreeView(BaseTreeView):

    def __init__(self, transverser, parent=None):
        super(OperatorTreeView, self).__init__(parent)
        self.transverser = None

        model = OperatorTreeModel(self)
        self.setModel(model)

        # Custom style
        delegate = OperatorTreeViewDelegate(self)
        self.setItemDelegate(delegate)

        self.setTransverser(transverser)

    def setTransverser(self, transverser):
        self.transverser = transverser
        self.model().setTransverser(transverser)

    def setCurrentNode(self, node):
        """Clear the widget and generate the view of the new node."""
        model = self.model()
        if model.setCurrentNode(node):
            model.refresh()

    def setCurrentItem(self, item):
        """Clear the widget and generate the view of the new node."""
        model = self.model()
        if model.setCurrentItem(item):
            model.refresh()

    def mouseDoubleClickEvent(self, event):
        """Receive mouse double click events for the widget."""
        # super(BaseTreeView, self).mouseDoubleClickEvent(event)
        index = self.indexAt(event.pos())
        item = index.internalPointer()

        item.selectOperator()


class OperatorTreeModel(BaseModel):

    def __init__(self, treeView, parent=None):

        self.transverser = None
        self.currentNode = None
        self.currentItem = None

        super(OperatorTreeModel, self).__init__(treeView, parent)

    def setTransverser(self, transverser):
        self.transverser = transverser
        self.refresh()

    def refresh(self):
        self.beginResetModel()

        self.rootItem = OperatorItem(None, "")
        if self.currentItem:
            path = self.currentItem.data[PROC_PATH]
            collections, local_collections = self.transverser.getCollections(self.currentNode, path)
            operators = self.transverser.getOperators(self.currentNode, path, collections=collections, gather_parents=True)
            for op, match in operators:
                enabled = cmds.getAttr(op+'.enable')
                local = self.transverser.operatorAffectsPath(path, op, collections=local_collections)
                OperatorItem(self.rootItem, op, enabled, local)

        self.endResetModel()

    def setCurrentNode(self, node):
        if self.currentNode != node:
            self.currentNode = node
            return True
        return False

    def setCurrentItem(self, item):
        if self.currentItem != item:
            self.currentItem = item
            return True
        return False

    def executeAction(self, action, index):
        """User pressed by one of the actions."""
        item = index.internalPointer()

        if action == OperatorItem.ACTION_SELECT:
            self.transverser.selectNode(item.name)
        elif action == OperatorItem.ACTION_DISABLE:
            self.transverser.toggleOperator(item.name)
            item.enabled = not item.enabled

    def data(self, index, role=QtCore.Qt.DisplayRole):
        """
        Return the data stored under the given role for the item referred to by
        the index.
        """
        if not index.isValid():
            return
        item = index.internalPointer()

        if (role == QtCore.Qt.DisplayRole or
                role == QtCore.Qt.EditRole):
            return item.getName()
        if role == QtCore.Qt.ToolTipRole:
            return "{} : {}".format(item.getName(), item.getNodeType())
        elif role == QtCore.Qt.SizeHintRole:
            return QtCore.QSize(250, ITEM_HEIGHT)
        elif role == QtCore.Qt.BackgroundRole:
            return item.getBackgroundColor()
        elif role == NODE_BAR_COLOUR:
            return item.getLabelColor()
        elif role == CHILD_COUNT:
            return item.childCount()
        elif role == ACTIONS:
            return item.getActions()
        elif role == ICON:
            return item.getIcon()
        elif role == TEXT_INDENT:
            return item.getIndent()
        elif role == NODE_ENABLED:
            return item.isEnabled()


class OperatorTreeViewDelegate(BaseDelegate):

    def __init__(self, treeView):
        super(OperatorTreeViewDelegate, self).__init__(treeView)


class OperatorItem(BaseItem):

    CONNECTED_ICON = BaseItem.dpiScaledIcon(":/outArrow.png")
    # ENABLED_ICON = BaseItem.dpiScaledIcon(":/hyper_s_ON.png")
    # DISABLED_ICON = BaseItem.dpiScaledIcon(":/hyper_s_OFF.png")
    DISABLED_ICON = BaseItem.dpiScaledIcon(":/RS_disable.png")

    COLOR_SETPARAMETER = QtGui.QColor(18, 82, 18)
    COLOR_COLLECTION = QtGui.QColor(204, 203, 129)
    COLOR_DISABLE = QtGui.QColor(227, 149, 141)
    COLOR_MERGE = QtGui.QColor(129, 140, 204)
    COLOR_SWITCH = QtGui.QColor(129, 192, 204)
    COLOR_INCLUDEGRAPH = QtGui.QColor(129, 204, 166)
    COLOR_MATERIALX = QtGui.QColor(204, 129, 203)

    BACKGROUND_COLOR_LOCAL = QtGui.QColor(82, 82, 82)
    BACKGROUND_COLOR_INHERITED = QtGui.QColor(71, 71, 71)

    (ACTION_EXPAND,  # Always first, even if not used
     ACTION_NONE,
     ACTION_SELECT,
     ACTION_DISABLE) = range(4)

    node_colors = {
        COLLECTION_OP:
            COLOR_COLLECTION,
        OVERRIDE_OP:
            COLOR_SETPARAMETER,
        DISABLE_OP:
            COLOR_DISABLE,
        MERGE_OP:
            COLOR_MERGE,
        SWITCH_OP:
            COLOR_SWITCH,
        INCLUDEGRAPH_OP:
            COLOR_INCLUDEGRAPH,
        MATERIALX_OP:
            COLOR_MATERIALX
    }

    def __init__(self, parentItem, name, enabled=True, local=True, index=-1):
        super(OperatorItem, self).__init__(parentItem, name, index)
        self.enabled = enabled
        self.local = local

    def selectOperator(self):
        cmds.select(self.name, r=True)

    def getNodeType(self):
        return utils.getNodeType(self.name)

    def getLabelColor(self):
        node_type = self.getNodeType()
        if node_type in self.node_colors:
            return self.node_colors[node_type]
        else:
            return QtGui.QColor(0, 0, 0)  # default to black if unknown operator

    def getBackgroundColor(self):
        """
        The background color of current node. It can be different depending on
        the item type.
        """
        if self.local:
            return self.BACKGROUND_COLOR_LOCAL
        else:
            return self.BACKGROUND_COLOR_INHERITED

    def getActions(self):

        actions = []
        actions.append((self.CONNECTED_ICON, 1.0, self.ACTION_SELECT, False, None))

        actions.append((self.DISABLED_ICON, 1.0, self.ACTION_DISABLE, not self.enabled, None))

        return actions

    def isEnabled(self):
        return self.enabled


class ProceduralPropertiesPanel(QtWidgets.QFrame):
    """SubClass properties panel for displying properties of an object"""

    propertyChanged = QtCore.Signal(str, object)
    GEAR_ICON = QtGui.QPixmap(":/gear.png")
    SHADER_ICON = QtGui.QPixmap(":/out_blinn.png")
    DISP_ICON = QtGui.QPixmap(":/out_displacementShader.png")

    def __init__(self, transverser, parent=None, style=None):
        super(ProceduralPropertiesPanel, self).__init__(parent)

        if not style:
            style = MtoAStyle.currentStyle()
        # style.apply(self)

        self.node = None
        self.item = None
        self.object = None
        self.graphview = None
        self.paramDict = {}
        self.transverser = transverser

        self.layout = QtWidgets.QVBoxLayout(self)
        self.setLayout(self.layout)
        self.object_label = QtWidgets.QLineEdit(self)
        self.object_label.setPlaceholderText("Select Item")
        self.object_label.setReadOnly(True)
        self.object_label.setAlignment(QtCore.Qt.AlignHCenter)
        self.layout.addWidget(self.object_label)

        # AddOverride Button
        self.toolBar = QtWidgets.QFrame()
        self.toolBar.setLayout(QtWidgets.QHBoxLayout())
        self.toolBar.setEnabled(False)
        self.layout.addWidget(self.toolBar)

        self.addOverrideButton = QtWidgets.QPushButton("Add Assignment")
        # self.addOverrideButton.setIcon(self.GEAR_ICON)
        self.overrideMenu = QtWidgets.QMenu()
        self.addOverrideButton.setMenu(self.overrideMenu)
        self.rootMenus = []

        self.toolBar.layout().addWidget(self.addOverrideButton)

        self.overrideMenu.triggered.connect(self.setOverrideFromMenu)

        self.addOperatorButton = QtWidgets.QPushButton("Add Operator")
        self.toolBar.layout().addWidget(self.addOperatorButton)
        self.operatorMenu = QtWidgets.QMenu()
        self.addOperatorButton.setMenu(self.operatorMenu)

        self.operatorMenu.triggered.connect(self.addOperator)

        self.showGraphButton = QtWidgets.QPushButton("Show Graph")
        self.toolBar.layout().addWidget(self.showGraphButton)
        self.showGraphButton.clicked.connect(self.showOperatorGraph)

        self.toolBar.layout().insertStretch(0)
        self.toolBar.layout().insertStretch(-1)

        self.overridesPanel = QtWidgets.QFrame()
        self.overridesPanel.setLayout(QtWidgets.QVBoxLayout())

        self.shadingPanel = QtWidgets.QFrame()
        self.shadingPanel.setLayout(QtWidgets.QVBoxLayout())
        self.shadingPanel.layout().setContentsMargins(15, 0, 0, 15)

        # shader override - HIDDEN BY DEFAULT
        self.shadingWidgets = {}
        self.shaderOverrideWidget = MtoANodeConnectionWidget("Shader", override=True, parent=self.shadingPanel)
        deleteShaderAction = self.shaderOverrideWidget.menu.addAction("Delete")
        deleteShaderAction.triggered.connect(self.deleteShaderAssignment)

        self.shaderOverrideWidget.valueChanged.connect(self.setShader)
        self.shaderOverrideWidget.overrideTriggered.connect(self.addShader)
        self.shaderOverrideWidget.connectionButtonClicked.connect(self.newShader)
        self.shaderOverrideWidget.nodeDisconnected.connect(self.disconnectShader)
        self.shaderOverrideWidget.setVisible(False)
        self.shadingPanel.layout().addWidget(self.shaderOverrideWidget)
        self.shadingWidgets[SHADER] = [self.shaderOverrideWidget]

        shaderCbContainerWidget = QtWidgets.QWidget(self)
        shaderCbContainerWidget.setLayout(QtWidgets.QHBoxLayout())
        shaderCbContainerWidget.setContentsMargins(0, 2, 0, 0)
        self.shaderEnabledCheckBox = MtoACheckbox(True, self)
        shaderCbContainerWidget.layout().addWidget(self.shaderEnabledCheckBox)
        self.shaderOverrideWidget.layout().insertWidget(0, shaderCbContainerWidget, alignment=QtCore.Qt.AlignTop)
        self.shaderEnabledCheckBox.toggled.connect(self.toggleShaderEnabled)
        self.shadingWidgets[SHADER].append(self.shaderEnabledCheckBox)

        # displacement override - HIDDEN BY DEFAULT
        self.dispOverrideWidget = MtoANodeConnectionWidget("Displacement", override=True, parent=self.shadingPanel)
        deleteDispAction = self.dispOverrideWidget.menu.addAction("Delete")
        deleteDispAction.triggered.connect(self.deleteDisplacementAssignment)

        self.dispOverrideWidget.valueChanged.connect(self.setDisplacement)
        self.dispOverrideWidget.overrideTriggered.connect(self.addDisplacement)
        self.dispOverrideWidget.connectionButtonClicked.connect(self.newDisplacement)
        self.dispOverrideWidget.setVisible(False)
        self.shadingPanel.layout().addWidget(self.dispOverrideWidget)
        self.shadingWidgets[DISP_MAP] = [self.dispOverrideWidget]

        dispCbContainerWidget = QtWidgets.QWidget(self)
        dispCbContainerWidget.setLayout(QtWidgets.QHBoxLayout())
        dispCbContainerWidget.setContentsMargins(0, 2, 0, 0)
        self.dispEnabledCheckBox = MtoACheckbox(True, self)
        dispCbContainerWidget.layout().addWidget(self.dispEnabledCheckBox)
        self.dispOverrideWidget.layout().insertWidget(0, dispCbContainerWidget, alignment=QtCore.Qt.AlignTop)
        self.dispEnabledCheckBox.toggled.connect(self.toggleDisplacementEnabled)
        self.shadingWidgets[DISP_MAP].append(self.dispEnabledCheckBox)

        self.overridesPanel.layout().addWidget(self.shadingPanel)

        self.localOverridesPanel = QtWidgets.QGroupBox("Local Overrides", self)
        self.localOverridesPanel.setLayout(QtWidgets.QVBoxLayout())
        self.localOverridesPanel.setVisible(False)
        self.overridesPanel.layout().addWidget(self.localOverridesPanel)

        self.inheritedOverridesPanel = QtWidgets.QGroupBox("Inherited Overrides", self)
        self.inheritedOverridesPanel.setLayout(QtWidgets.QVBoxLayout())
        self.inheritedOverridesPanel.setVisible(False)
        self.overridesPanel.layout().addWidget(self.inheritedOverridesPanel)

        self.layout.addWidget(self.overridesPanel)

        # now the operators list

        self.operators_frame = QtWidgets.QGroupBox("Operators", self)
        self.operators_frame.setLayout(QtWidgets.QVBoxLayout())
        self.operators_frame.setMinimumHeight(150)

        self.operators_tree = OperatorTreeView(self.transverser, self.operators_frame)
        self.operators_frame.layout().addWidget(self.operators_tree)
        self.layout.addWidget(self.operators_frame)

    def showOperatorGraph(self):
        window, nodeEditor = self.transverser.showGraphEditor(self.node)

    def addOperatorMenu(self):
        self.operatorMenu.clear()
        for op in OPERATORS:
            self.operatorMenu.addAction(op)

    def addOperator(self, action):
        op_name = action.text()
        # check if op exists, otherwise create it
        if not cmds.objExists(op_name) and op_name in OPERATORS:
            op = self.transverser.createOperator(self.node, self.item, op_name)
            self.refresh()

    def addOverrideMenu(self):

        def _addMenus(node_type, params):
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

        if not self.overrideMenu.isEmpty():
            self.overrideMenu.clear()
            for menu in self.rootMenus:
                menu.deleteLater()
            self.rootMenus = []

        nodeEntry = self.object[PROC_ENTRY]
        nodeEntryType = self.object[PROC_ENTRY_TYPE]

        isShape = (nodeEntryType == 'shape')
        group_nodes = [None, '', 'xform'] + PROCEDURAL_NODES
        isGroup = (nodeEntry in group_nodes)

        if isShape or isGroup:
            self.overrideMenu.addAction("shader")
            self.overrideMenu.addAction("displacement")
            self.overrideMenu.addSeparator()

        common = None

        for node_type, params in sorted(self.paramDict.items()):
            if node_type == 'hidden':
                continue

            showMenu = isGroup or (node_type == self.object[PROC_ENTRY])
            if isShape and node_type == 'common':
                common = params
                showMenu = False

            if not showMenu:
                continue

            _addMenus(node_type, params)

        if isShape and common:
            _addMenus("common", common)

        self.overrideMenu.addAction("custom")

    def setOverrideFromMenu(self, action):
        param = action.text()
        self.setNewOverride(param)

    def setNewOverride(self, param):
        ops = self.getItemOverrideOperator(True)
        if not type(ops) == list:
            ops = [ops]
        op = ops[-1] # always use the last operator
        if param == "shader":
            self.addShader()
        elif param == "displacement":
            self.addDisplacement()
        elif param == "custom":
            self.addCustomOverride()
        else:
            value = self.getDefaultValue(param)
            self.setOverride(param, "=", value, operator=op)
            self.refresh()

    def getDefaultValue(self, param):
        default_value = ""
        param_data = self.getParamData(param)
        if param_data is not None:
            default_value = param_data[DEFAULT_VALUE]
        return default_value

    def setTransverser(self, transverser):
        self.transverser = transverser
        self.operators_tree.setTransverser(transverser)

    def setCurrentItem(self, item):
        itemchanged = False
        if self.item != item:
            self.item = item
            self.operators_tree.setCurrentItem(item)
            itemchanged = True
        return itemchanged

    def setNode(self, node, force=False):
        changed = False
        if self.node != node and node and cmds.objExists(node) or force:
            self.node = node
            changed = True
        self.populateOperatorsList()
        return changed

    def setItem(self, node, item):
        nodechanged = self.setNode(node)

        itemchanged = False
        if self.item != item and (item is not None):
            self.item = item
            self.operators_tree.setCurrentItem(item)
            itemchanged = True
        elif nodechanged and item is None:
            self.item = None
            self.operators_tree.setCurrentItem(None)

        if self.item and itemchanged:
            self.toolBar.setEnabled(True)
            self.object = item.data
            self.object_label.setText(item.data[PROC_PATH])
            self.getParams()
            self.addOverrideMenu()
            self.addOperatorMenu()
        elif self.item is None:
            self.object_label.setText("")
            self.toolBar.setEnabled(False)

        self.populateOperatorsList()

        self.refresh()
        # Tell the transverser that the selection has changed.
        # Let's pass the data as an array in case we end up supporting multi-selection
        if self.transverser and item:
            self.transverser.selectionChanged(node, [item.data])

    def populateOperatorsList(self):
        self.operators_tree.setCurrentNode(self.node)

    def setShader(self, shader, create=False, enabled=True):
        self.setNodeParam("shader", shader, create, enabled)

    def setDisplacement(self, disp, create=False, enabled=True):
        self.setNodeParam("disp_map", disp, create, enabled)

    def setNodeParam(self, param, node, create=False, enabled=True):
        widget, checkbox = self.shadingWidgets[param]
        operator = widget.data.get('operator', None)
        if create:
            operator = self.getOverrideOperator()
        self.setOverride(param, "=", node, operator=operator, enabled=enabled)
        self.refresh()

    def addShader(self):
        if self.item:
            self.setShader(None, True)

    def addDisplacement(self):
        if self.item:
            self.setDisplacement(None, True)

    def newDisplacement(self):
        self.newShadingNode(DISP_MAP)

    def newShader(self):
        self.newShadingNode(SHADER)

    def disconnectShader(self):
        if self.item:
            self.setShader(None)

    def toggleShaderEnabled(self, value):
        shader = self.shaderOverrideWidget.getValue()
        self.setShader(shader, False, value)

    def toggleDisplacementEnabled(self, value):
        disp = self.dispOverrideWidget.getValue()
        self.setDisplacement(disp, False, value)

    def deleteShaderAssignment(self):
        self.deleteNodeAssignment(SHADER)

    def deleteDisplacementAssignment(self):
        self.deleteNodeAssignment(DISP_MAP)

    def deleteNodeAssignment(self, param):
        if self.item:
            widget, checkbox = self.shadingWidgets[param]
            index = widget.data['index']
            operator = widget.data['operator']
            self.removeOverride(operator, index)

    def addCustomOverride(self):
        operator = self.getOverrideOperator()
        param_name = self.transverser.getCustomParamName(operator)
        self.setOverride(param_name, "=", "1", TYPES_DICT['int'], custom=True, operator=operator)
        self.refresh()

    def newShadingNode(self, param):
        # feed the output of the createRedner Node dialog to the setShader method
        widget, checkbox = self.shadingWidgets[param]
        operator = widget.data.get('operator', None)
        callback = "from mtoa.ui.procview.ProceduralTransverser import ProceduralTransverser;"
        callback += "ProceduralTransverser.setOverride('{node}', '{path}', '{operator}', '{param}', '=', '%node', {param_type})"

        param_data = self.getParamData(param)
        callback = callback.format(node=self.node, path=self.item.data[PROC_PATH],
                                   operator=operator, param_type=param_data[PARAM_TYPE],
                                   param=param)
        mel.eval("createRenderNode -all \"python(\\\"" + callback + "\\\")\" \"\"")

    def getOverrideOperator(self, create=True):
        collections = self.transverser.getCollections(self.node, self.item.data[PROC_PATH])
        ops = self.transverser.getOperators(self.node, self.item.data[PROC_PATH], OVERRIDE_OP, collections)
        ops = [o for o, m in ops if m]
        if not len(ops) and create:
            op = self.transverser.createOperator(self.node, self.item, OVERRIDE_OP)
        else:
            op = ops[0]
        return op

    def getItemOverrideOperator(self, create=False):
        op = self.item.getOverridesOp()
        if not op and create:
            op = self.getOverrideOperator()
        return op

    def getParams(self):
        data = self.getData(self.item)
        if not data:
            return

        nodeEntry = data[PROC_ENTRY]
        group_nodes = [None, '', 'xform'] + PROCEDURAL_NODES
        isGroup = (nodeEntry in group_nodes)
        # For groups, let's append the default node types (for alembic shapes)
        if isGroup:
            self.paramDict = self.transverser.getParams([nodeEntry] + NODE_TYPES)
        else:
            # for specific node types, let's just use this node type attributes
            self.paramDict = self.transverser.getParams([data[PROC_ENTRY]])

    def resetShadingWidgets(self):
        for widget, checkbox in self.shadingWidgets.values():
            widget.disconnectNode(False)

    def getData(self, item):
        if item:
            if item.data:
                return item.data
            else:
                return self.getData(item.parent())

    def refresh(self):

        def getParmInList(param, param_list):
            for i, p in enumerate(param_list):
                if p[PARAM] == param:
                    return i
            return -1

        clearWidget(self.localOverridesPanel)
        clearWidget(self.inheritedOverridesPanel)


        for widget, checkbox in self.shadingWidgets.values():
            widget.setVisible(False)
            oldState = checkbox.blockSignals(True)
            checkbox.setChecked(True)
            checkbox.blockSignals(oldState)
            widget.node = None

        self.resetShadingWidgets()
        if self.item:
            path = self.item.data[PROC_PATH]
            overrides, p_overrides = self.getOverrides()
            for ov in overrides:
                idx = getParmInList(ov, p_overrides)
                if idx != -1:
                    p_overrides[idx] = ov
                else:
                    p_overrides.append(ov)  # split to param, op, VALUE
            for override in p_overrides:
                # FIXME what if the user wants to connect a shader from inside the procedural?
                operator = override[OPERATOR]
                index = override[INDEX]
                ops = self.getItemOverrideOperator()
                if not type(ops) == list:
                    ops = [ops]
                inherited = operator not in ops

                if override[PARAM] in [SHADER, DISP_MAP]:
                    # set the shader slot
                    node = override[VALUE].replace("'", "").replace('"', "")
                    widget, checkbox = self.shadingWidgets[override[PARAM]]
                    oldState = checkbox.blockSignals(True)
                    checkbox.setChecked(override[ENABLED])
                    checkbox.blockSignals(oldState)
                    widget.setNode(node, False)
                    widget.setVisible(True)
                    widget.setInherited(inherited)
                    widget.data['operator'] = operator
                    widget.data['index'] = index
                else:
                    widget = self.addOverrideGUI(*override)
                    widget.setInherited(inherited)
                    widget.overrideTriggered.connect(self.setNewOverride)
            # hide the panels if they don't have children
            for pan in [self.inheritedOverridesPanel, self.localOverridesPanel]:
                if pan.layout().count() == 0:
                    pan.setVisible(False)
        else:
            self.object_label.setText("Select Item")
            self.toolBar.setEnabled(False)

        # refresh the operators list
        self.operators_tree.model().refresh()

    def getProperties(self, obj):
        pass

    def getOverrides(self):
        data = self.getData(self.item)
        if not data:
            return []
        return self.transverser.getOverrides(self.node, data[PROC_PATH])

    def addOverrideGUI(self, param_type, param, op, value, index, operator, enabled):

        parentPanel = self.inheritedOverridesPanel
        data = self.getData(self.item)
        path = data[PROC_PATH]
        collections, local_collections = self.transverser.getCollections(self.node,
                                                            path)

        local_collections = [c for c, m in local_collections if m]
        sel_match, exact_match = self.transverser.operatorAffectsPath(path, operator, collections=local_collections)
        if exact_match:
            parentPanel = self.localOverridesPanel

        parentPanel.setVisible(True)

        new_widget = MtoAOperatorOverrideWidget(param_type, param, op, value, self.paramDict, parentPanel, enabled)

        new_widget.index = index
        new_widget.operator = operator
        new_widget.deleteMe.connect(self.removeOverrideWidget)

        # param, operation, value, param_type, custom, index, operator
        new_widget.valueChanged[str, str, str, int, bool, int, str, bool, bool].connect(self.setOverride)
        new_widget.valueChanged[str, str, int, int, bool, int, str, bool, bool].connect(self.setOverride)
        new_widget.valueChanged[str, str, bool, int, bool, int, str, bool, bool].connect(self.setOverride)
        new_widget.valueChanged[str, str, float, int, bool, int, str, bool, bool].connect(self.setOverride)
        # add widget
        parentPanel.layout().addWidget(new_widget)
        return new_widget

    def removeOverrideWidget(self, widget):
        index = widget.index
        operator = widget.operator
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

    def setOverride(self, param, op, value, param_type=None, custom=False,  index=-1, operator=None, enabled=True):
        if not operator:
            ops = self.getItemOverrideOperator()
            if not type(ops) == list:
                ops = [ops]
            if not len(ops):
                return
            operator = ops[-1]

        is_array = False
        data = self.getData(self.item)
        if not data:
            return

        param_data = self.getParamData(param)
        if param_data:
            is_array = param_data[DATA_IS_ARRAY]
            if not param_type:
                param_type = param_data[DATA_PARAM_TYPE]

        if self.transverser.setOverride(self.node, data[PROC_PATH], operator, param, op, value, param_type, custom, enabled, index):
            self.item.setDirty(True)
            return True
        return False

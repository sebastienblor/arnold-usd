
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
                            PROC_INSTANCEPATH, PROC_ENTRY_TYPE, PROC_IOBJECT, \
                            OVERRIDE_OP, DISABLE_OP, COLLECTION_OP, NODE_TYPES,\
                            PARM, OP, VALUE, INDEX, OPERATOR


OPERATORS = cmds.arnoldPlugins(listOperators=True) or []

DISP_MAP = 'disp_map'
SHADER = 'shader'


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
        # if not self.currentNode or \
        #    not cmds.objExists(self.currentNode) or \
        #    not self.transverser:
        #     return

        self.beginResetModel()

        self.rootItem = OperatorItem(None, "")
        if self.currentItem:
            path = self.currentItem.data[PROC_PATH]
            collections = self.transverser.getCollections(self.currentNode, path, False)
            local_collections = self.transverser.getCollections(self.currentNode, path, True)
            operators = self.transverser.getOperators(self.currentNode, path, exact_match=False, collections=collections)
            for op in operators:
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


class OperatorTreeViewDelegate(BaseDelegate):

    def __init__(self, treeView):
        super(OperatorTreeViewDelegate, self).__init__(treeView)


class OperatorItem(BaseItem):

    CONNECTED_ICON = BaseItem.dpiScaledIcon(":/outArrow.png")
    # ENABLED_ICON = BaseItem.dpiScaledIcon(":/hyper_s_ON.png")
    # DISABLED_ICON = BaseItem.dpiScaledIcon(":/hyper_s_OFF.png")
    DISABLED_ICON = BaseItem.dpiScaledIcon(":/RS_disable.png")

    COLOR_OPERATOR = QtGui.QColor(18, 82, 18)
    COLOR_COLLECTION = QtGui.QColor(179, 177, 71)

    (ACTION_EXPAND,  # Always first, even if not used
     ACTION_NONE,
     ACTION_SELECT,
     ACTION_DISABLE) = range(4)

    def __init__(self, parentItem, name, enabled=True, local=True, index=-1):
        super(OperatorItem, self).__init__(parentItem, name, index)
        self.enabled = enabled
        self.local = local

    def selectOperator(self):
        cmds.select(self.name, r=True)

    def getNodeType(self):
        return utils.getNodeType(self.name)

    def getLabelColor(self):
        if self.getNodeType() == COLLECTION_OP:
            return self.COLOR_COLLECTION
        if self.local:
            return self.COLOR_OPERATOR
        else:
            return QtGui.QColor(100, 149, 100)

    def getBackgroundColor(self):
        """
        The background color of current node. It can be different depending on
        the item type.
        """
        if self.local:
            return QtGui.QColor(71, 71, 71)
        else:
            return QtGui.QColor(51, 51, 51)

    def getActions(self):

        actions = []
        actions.append((self.CONNECTED_ICON, 1.0, self.ACTION_SELECT, False))

        # disableIcon = self.ENABLED_ICON if self.enabled else self.DISABLED_ICON
        actions.append((self.DISABLED_ICON, 1.0, self.ACTION_DISABLE, not self.enabled))

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
        self.object_label = QtWidgets.QLabel("Select Item")
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

        # shader override - HIDDEN BY DEFAULT
        self.shadingWidgets = {}
        self.shaderOverrideWidget = MtoANodeConnectionWidget("Shader")
        deleteShaderAction = self.shaderOverrideWidget.menu.addAction("Delete")
        deleteShaderAction.triggered.connect(self.deleteShaderAssignment)

        self.shaderOverrideWidget.valueChanged.connect(self.setShader)
        self.shaderOverrideWidget.overrideTriggered.connect(self.addShader)
        self.shaderOverrideWidget.connectionButtonClicked.connect(self.newShader)
        self.shaderOverrideWidget.nodeDisconnected.connect(self.disconnectShader)
        self.shaderOverrideWidget.setVisible(False)
        self.shadingPanel.layout().addWidget(self.shaderOverrideWidget)
        self.shadingWidgets[SHADER] = self.shaderOverrideWidget

        # displacement override - HIDDEN BY DEFAULT
        self.dispOverrideWidget = MtoANodeConnectionWidget("Displacement")
        deleteDispAction = self.dispOverrideWidget.menu.addAction("Delete")
        deleteDispAction.triggered.connect(self.deleteDisplacementAssignment)

        self.dispOverrideWidget.valueChanged.connect(self.setDisplacement)
        self.dispOverrideWidget.overrideTriggered.connect(self.addDisplacement)
        self.dispOverrideWidget.connectionButtonClicked.connect(self.newDisplacement)
        self.dispOverrideWidget.setVisible(False)
        self.shadingPanel.layout().addWidget(self.dispOverrideWidget)
        self.shadingWidgets[DISP_MAP] = self.dispOverrideWidget

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

        if not self.overrideMenu.isEmpty():
            self.overrideMenu.clear()
            for menu in self.rootMenus:
                menu.deleteLater()
            self.rootMenus = []
        self.overrideMenu.addAction("shader")
        self.overrideMenu.addAction("displacement")
        self.overrideMenu.addSeparator()
        for node_type, params in sorted(self.paramDict.items()):
            if node_type != 'hidden':
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

    def setOverrideFromMenu(self, action):
        param = action.text()
        self.setNewOverride(param)

    def setNewOverride(self, param):
        op = self.getItemOverrideOperator(True)
        if param == "shader":
            self.addShader()
        elif param == "displacement":
            self.addDisplacement()
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

    def setNode(self, node):
        changed = False
        if self.node != node and cmds.objExists(node):
            self.node = node
            changed = True
        self.populateOperatorsList()
        return changed

    def setItem(self, node, item):
        nodechanged = self.setNode(node)
        print "ProceduralPropertiesPanel.setItem", nodechanged, node, item
        # itemchanged = self.setCurrentItem(item)

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
            self.object_label.setText("Select Item")
            self.toolBar.setEnabled(False)

        self.populateOperatorsList()

        self.refresh()
        # Tell the transverser that the selection has changed.
        # Let's pass the data as an array in case we end up supporting multi-selection
        if self.transverser and item:
            self.transverser.selectionChanged(node, [item.data])

    def populateOperatorsList(self):
        self.operators_tree.setCurrentNode(self.node)

    def setShader(self, shader, create=False):
        self.setNodeParam("shader", shader, create)

    def setDisplacement(self, disp, create=False):
        self.setNodeParam("disp_map", disp, create)

    def setNodeParam(self, param, node, create=False):
        operator = self.shadingWidgets[param].data.get('operator', None)
        if create:
            operator = self.getOverrideOperator()
        self.setOverride(param, "=", node, operator=operator)
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

    def deleteShaderAssignment(self):
        self.deleteNodeAssignment(SHADER)

    def deleteDisplacementAssignment(self):
        self.deleteNodeAssignment(DISP_MAP)

    def deleteNodeAssignment(self, param):
        if self.item:
            index = self.shadingWidgets[param].data['index']
            operator = self.shadingWidgets[param].data['operator']
            self.removeOverride(operator, index)

    def newShadingNode(self, param):
        # feed the output of the createRedner Node dialog to the setShader method
        operator = self.shadingWidgets[param].data.get('operator', None)
        callback = "from mtoa.ui.procview.ProceduralTransverser import ProceduralTransverser;"
        callback += "ProceduralTransverser.setOverride('{node}', '{path}', '{operator}', '{param}', '=', '%node', {param_type})"

        param_data = self.getParamData(param)
        callback = callback.format(node=self.node, path=self.item.data[PROC_PATH],
                                   operator=operator, param_type=param_data[PARAM_TYPE],
                                   param=param)
        mel.eval("createRenderNode -all \"python(\\\"" + callback + "\\\")\" \"\"")

    def getOverrideOperator(self, create=True, exact_match=True):
        collections = self.transverser.getCollections(self.node, self.item.data[PROC_PATH], exact_match)
        ops = self.transverser.getOperators(self.node, self.item.data[PROC_PATH], OVERRIDE_OP, exact_match, collections)
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

        # node_types = self.transverser.getNodeTypes(data[PROC_IOBJECT])
        self.paramDict = self.transverser.getParams(NODE_TYPES)

    def resetShadingWidgets(self):
        for widget in self.shadingWidgets.values():
            widget.disconnectNode(False)

    def getData(self, item):
        if item:
            if item.data:
                return item.data
            else:
                return self.getData(item.parent())

    def refresh(self):
        clearWidget(self.localOverridesPanel)
        clearWidget(self.inheritedOverridesPanel)
        for widget in self.shadingWidgets.values():
            widget.setVisible(False)
            widget.node = None

        self.resetShadingWidgets()
        if self.item:
            for override in self.getOverrides():
                # FIXME what if the user wants to connect a shader from inside the procedural?
                operator = override[OPERATOR]
                index = override[INDEX]
                inherited = operator != self.getItemOverrideOperator()

                if override[PARM] in [SHADER, "disp_map"]:
                    # set the shader slot
                    node = override[VALUE].replace("'", "").replace('"', "")
                    self.shadingWidgets[override[PARM]].setNode(node, False)
                    self.shadingWidgets[override[PARM]].setVisible(True)
                    self.shadingWidgets[override[PARM]].setInherited(inherited)
                    self.shadingWidgets[override[PARM]].data['operator'] = operator
                    self.shadingWidgets[override[PARM]].data['index'] = index
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

    def addOverrideGUI(self, param, op, value, index, operator):

        parentPanel = self.inheritedOverridesPanel
        data = self.getData(self.item)
        path = data[PROC_PATH]
        local_collections = self.transverser.getCollections(self.node,
                                                            path, True)
        if self.transverser.operatorAffectsPath(path, operator, collections=local_collections):
            parentPanel = self.localOverridesPanel

        parentPanel.setVisible(True)

        new_widget = MtoAOperatorOverrideWidget(param, op, value, self.paramDict, parentPanel)

        new_widget.index = index
        new_widget.operator = operator
        new_widget.deleteMe.connect(self.removeOverrideWidget)

        new_widget.valueChanged[str, str, str, int, str].connect(self.setOverride)
        new_widget.valueChanged[str, str, int, int, str].connect(self.setOverride)
        new_widget.valueChanged[str, str, bool, int, str].connect(self.setOverride)
        new_widget.valueChanged[str, str, float, int, str].connect(self.setOverride)
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

    @QtCore.Slot(str, str, str, int)
    @QtCore.Slot(str, str, int, int)
    @QtCore.Slot(str, str, bool, int)
    @QtCore.Slot(str, str, float, int)
    def setOverride(self, param, op, value, index=-1, operator=None):
        if not operator:
            operator = self.getItemOverrideOperator()
        param_type = None
        is_array = False
        data = self.getData(self.item)
        if not data:
            return

        param_data = self.getParamData(param)
        if param_data:
            param_type = param_data[PARAM_TYPE]
            is_array = param_data[IS_ARRAY]

        return self.transverser.setOverride(self.node, data[PROC_PATH], operator, param, op, value, param_type, is_array, index)

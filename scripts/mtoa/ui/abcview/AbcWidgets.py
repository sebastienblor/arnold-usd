
from mtoa.ui.qt.Qt import QtWidgets
from mtoa.ui.qt.Qt import QtGui
from mtoa.ui.qt.Qt import QtCore
from mtoa.ui.qt import setStaticSize, clearWidget
from mtoa.ui.qt.widgets import *

from mtoa.ui.abcview.AbcTransverser import ABC_PATH, ABC_NAME, ABC_PARENT, ABC_VISIBILITY, \
                            ABC_INSTANCEPATH, ABC_ENTIY_TYPE, ABC_IOBJECT, \
                            OVERRIDE_OP, DISABLE_OP,\
                            PARM, OP, VALUE, INDEX

class AbcPropertiesPanel(QtWidgets.QFrame):
    """SubClass properties panel for displying properties of an object"""

    propertyChanged = QtCore.Signal(str, object)
    GEAR_ICON = QtGui.QPixmap(":/gear.png")

    def __init__(self, transverser, parent=None):
        super(AbcPropertiesPanel, self).__init__(parent)

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

        self.addOverideButton = QtWidgets.QPushButton("Add Parameter")
        self.addOverideButton.setIcon(self.GEAR_ICON)
        self.toolBar.layout().addWidget(self.addOverideButton)
        self.toolBar.layout().insertStretch(-1)

        self.addOverideButton.clicked.connect(self.addOverride)

        self.shadingPanel = QtWidgets.QFrame()
        self.shadingPanel.setLayout(QtWidgets.QVBoxLayout())
        self.shadingPanel.layout().setContentsMargins(0, 0, 0, 0)

        # shader override
        self.shadingWidgets = {}
        self.shaderOverrideWidget = MtoANodeConnectionWidget("Shader")
        self.shaderOverrideWidget.valueChanged.connect(self.setShader)
        self.shaderOverrideWidget.nodeDisconnected.connect(self.disconnectShader)
        self.shadingPanel.layout().addWidget(self.shaderOverrideWidget)
        self.shadingWidgets['shader'] = self.shaderOverrideWidget

        # displacement override
        self.dispOverrideWidget = MtoANodeConnectionWidget("Displacement")
        self.dispOverrideWidget.valueChanged.connect(self.setDisplacement)
        self.shadingPanel.layout().addWidget(self.dispOverrideWidget)
        self.shadingWidgets['disp_map'] = self.dispOverrideWidget

        self.layout.addWidget(self.shadingPanel)

        self.overridesPanel = QtWidgets.QFrame()
        self.overridesPanel.setLayout(QtWidgets.QVBoxLayout())
        self.layout.addWidget(self.overridesPanel)

    def setItem(self, node, item):
        self.node = node
        self.item = item
        if item:
            self.object = item.data
            self.object_label.setText(item.getName())
            self.getParams()

        self.refresh()

    def setShader(self, shader):
        self.setNodeParam("shader", shader)

    def disconnectShader(self):
        pass

    def setDisplacement(self, disp):
        self.setNodeParam("disp_map", disp)

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

        node_types = self.transverser.getNodeTypes(data[ABC_IOBJECT])
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
        self.resetShadingWidgets()
        if self.item:
            data = self.getData(self.item)
            if data:
                for override in self.transverser.getOverrides(self.node, data[ABC_PATH]):
                    # FIXME what if the user wants to connect a shader from inside the procedural?
                    if override[PARM] in ["shader", "disp_map"]:
                        # set the shader slot
                        node = override[VALUE].replace("'", "").replace('"', "")
                        self.shadingWidgets[override[PARM]].setNode(node, False)
                    else:
                        self.addOverrideGUI(*override)

    def getProperties(self, obj):
        pass

    def getOverrides(self):
        data = self.getData(self.item)
        if not data:
            return
        return self.transverser.getOverrides(self.node, data[ABC_PATH])

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
        param_data = self.paramDict.get(param, (None, None))
        new_widget.setParam(param, param_data[PARAM_TYPE], param_data[NODE_TYPE], self.paramDict)
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
        removed = self.transverser.deleteOverride(self.node, data[ABC_PATH], index)
        if removed:
            if len(self.getOverrides()) == 0:
                self.transverser.deleteOperator(self.node, data[ABC_PATH], OVERRIDE_OP)
            self.refresh()

    @QtCore.Slot(str, str, str, int)
    @QtCore.Slot(str, str, int, int)
    @QtCore.Slot(str, str, bool, int)
    @QtCore.Slot(str, str, float, int)
    def setOverride(self, param, op, value, index=-1):
        if not param and not value:
            param = "new"
            value = "''"
        param_type = None
        is_array = False

        data = self.getData(self.item)
        if not data:
            return

        param_data = self.paramDict.get(param)
        if param_data:
            param_type = param_data[PARAM_TYPE]
            is_array = param_data[IS_ARRAY]

        return self.transverser.setOverride(self.node, data[ABC_PATH], param, op, value, param_type, is_array, index)


from mtoa.ui.qt.Qt import QtWidgets
from mtoa.ui.qt.Qt import QtGui
from mtoa.ui.qt.Qt import QtCore
from mtoa.ui.qt import setStaticSize, clearWidget
from mtoa.ui.qt.widgets import *

from AbcTransverser import ABC_PATH, ABC_NAME, ABC_PARENT, ABC_VISIBILITY, \
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

        self.addOverideButton = QtWidgets.QPushButton("Add Override")
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

    def setObject(self, node, obj):
        self.node = node
        self.object = obj
        self.object_label.setText(obj[ABC_PATH])
        self.getParams()
        self.getOverrides()

    def setShader(self, shader):
        print "AbcPropertiesPanel.setShader", shader
        self.setNodeParam(shader, "shader")

    def disconnectShader(self):
        pass

    def setDisplacement(self, disp):
        self.setNodeParam(disp, "disp_map")

    def getOverrideOperator(self):
        op = self.transverser.getOperator(self.node, self.object[ABC_PATH])
        if not op:
            op = self.transverser.createOperator(self.node, self.object, "aiSetParameter")

    def setNodeParam(self, node, param):
        op = self.getOverrideOperator()
        self.setOverride(param, "=", node)

    def getParams(self):
        node_types = self.transverser.getNodeTypes(self.object[ABC_IOBJECT])
        self.paramDict = self.transverser.getParams(node_types)

    def getOverrides(self):
        clearWidget(self.overridesPanel)
        for override in self.transverser.getOverrides(self.node, self.object[ABC_PATH]):
            # FIXME what if the user wants to connect a shader from inside the procedural?
            if override[PARM] in ["shader", "disp_map"]:
                # set the shader slot
                self.shadingWidgets[override[PARM]].setNode(value)
            else:
                self.addOverrideGUI(*override)

    def getProperties(self, obj):
        pass

    def addOverride(self):
        op = getOverrideOperator()
        self.setOverride(None, "=", None)
        self.getOverrides()

    def addOverrideGUI(self, param, op, value, index=-1):
        print "AbcPropertiesPanel.addOverrideGUI", param, op, value, index
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
        new_widget.valueChanged[str, str, float, int].connect(self.setOverride)
        # add widget
        self.overridesPanel.layout().addWidget(new_widget)
        return new_widget

    def removeOverride(self, widget):
        index = widget.index
        removed = self.transverser.deleteOverride(self.node, self.object[ABC_PATH], index)
        if removed:
            self.getOverrides()

    @QtCore.Slot(str, str, str, int)
    @QtCore.Slot(str, str, int, int)
    @QtCore.Slot(str, str, float, int)
    def setOverride(self, param, op, value, index=-1):
        print "AbcPropertiesPanel.setOverride", param, op, value, index

        if not param and not value:
            param = "NEWOVERRIDE"
            value = "NEWVALUE"
        param_type = None
        is_array = False

        param_data = self.paramDict.get(param)
        if param_data:
            param_type = param_data[PARAM_TYPE]
            is_array = param_data[IS_ARRAY]

        return self.transverser.setOverride(self.node, self.object[ABC_PATH], param, op, value, param_type, is_array, index)

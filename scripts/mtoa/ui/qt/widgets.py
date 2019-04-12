from .Qt import OpenMayaUI
from .Qt import QtCore
from .Qt import QtGui
from .Qt import QtWidgets
from .itemStyle import ItemStyle
from .treeView import BaseItem
from .utils import dpiScale, setStaticSize, clearWidget, valueIsExpression

from .button import MtoAButton, MtoACheckableButton

from maya.app.general.mayaMixin import MayaQWidgetBaseMixin
import maya.cmds as cmds
import maya.mel as mel
import re

from arnold import *

(PARAM_TYPE,
 DEFAULT_VALUE,
 IS_ARRAY,
 ENUM_VALUES) = range(4)

OPERATIONS = ["=", "+=", "-=", "*=", "/=", "^=", "%="]

TYPES = ["string", "int", "uint", "byte", "float", "bool", "node"]
TYPES_DICT = {"string": AI_TYPE_STRING,
              "int":    AI_TYPE_INT,
              "uint":   AI_TYPE_UINT,
              "byte":   AI_TYPE_BYTE,
              "float":  AI_TYPE_FLOAT,
              "bool":   AI_TYPE_BOOLEAN,
              "node":   AI_TYPE_NODE}

TYPES_DICT_STRINGS = {v:k for k,v in TYPES_DICT.items()}

TYPES_DEFAULT = {AI_TYPE_STRING: "",
                 AI_TYPE_INT: 0,
                 AI_TYPE_UINT: 0,
                 AI_TYPE_BYTE: 0,
                 AI_TYPE_FLOAT: 0.0,
                 AI_TYPE_BOOLEAN: False,
                 AI_TYPE_NODE: ""}

# TYPES = ["string", "int", "byte", "uint", "float", "bool", "node", "rgb", "rgba", "vector", "vector2", "matrix"]

DEFAULT_VISBILITIES = {'AI_RAY_CAMERA': True,
                       'AI_RAY_SHADOW': True,
                       'AI_RAY_DIFFUSE_REFLECT': True,
                       'AI_RAY_SPECULAR_REFLECT': True,
                       'AI_RAY_DIFFUSE_TRANSMIT': True,
                       'AI_RAY_SPECULAR_TRANSMIT': True,
                       'AI_RAY_VOLUME': True,
                       'AI_RAY_SUBSURFACE': True}

RAYS = [("ai_ray_camera", AI_RAY_CAMERA),
        ("ai_ray_shadow", AI_RAY_SHADOW),
        ("ai_ray_diffuse_transmit", AI_RAY_DIFFUSE_TRANSMIT),
        ("ai_ray_specular_transmit", AI_RAY_SPECULAR_TRANSMIT),
        ("ai_ray_volume", AI_RAY_VOLUME),
        ("ai_ray_diffuse_reflect", AI_RAY_DIFFUSE_REFLECT),
        ("ai_ray_specular_reflect", AI_RAY_SPECULAR_REFLECT),
        ("ai_ray_subsurface", AI_RAY_SUBSURFACE)]


STRING_EXP = re.compile(r'["\']*(\w+)["\']*')


def getVisibilityValue(ai_ray_camera=True,
                       ai_ray_shadow=True,
                       ai_ray_diffuse_transmit=True,
                       ai_ray_specular_transmit=True,
                       ai_ray_volume=True,
                       ai_ray_diffuse_reflect=True,
                       ai_ray_specular_reflect=True,
                       ai_ray_subsurface=True):

    vis = AI_RAY_ALL

    if not ai_ray_camera:
        vis &= ~AI_RAY_CAMERA
    if not ai_ray_shadow:
        vis &= ~AI_RAY_SHADOW
    if not ai_ray_diffuse_transmit:
        vis &= ~AI_RAY_DIFFUSE_TRANSMIT
    if not ai_ray_specular_transmit:
        vis &= ~AI_RAY_SPECULAR_TRANSMIT
    if not ai_ray_volume:
        vis &= ~AI_RAY_VOLUME
    if not ai_ray_diffuse_reflect:
        vis &= ~AI_RAY_DIFFUSE_REFLECT
    if not ai_ray_specular_reflect:
        vis &= ~AI_RAY_SPECULAR_REFLECT
    if not ai_ray_subsurface:
        vis &= ~AI_RAY_SUBSURFACE
    return vis


def getVisibilityDict(value):
    vis_dict = {}
    for n, v in RAYS:
        vis_dict[v] = True
    compViz = AI_RAY_ALL
    if value < compViz:
        for v in reversed([v for n, v in RAYS]):
            compViz &= ~v
            if value <= compViz:
                vis_dict[v] = False
            else:
                compViz += v
    return vis_dict


class MtoAVisibilityCheckBox(QtWidgets.QCheckBox):
    """docstring for MtoAVisibilityCheckBox"""
    def __init__(self, label, parent=None):
        super(MtoAVisibilityCheckBox, self).__init__(label, parent)
        self.VIS_RAY = None


class MtoAVisibilityWidget(QtWidgets.QFrame):
    """docstring for MtoAVisibilityWidget"""

    valueChanged = QtCore.Signal(int)
    deleteAttr = QtCore.Signal(str, str, str)

    def __init__(self, value=255, title='test', parent=None):
        super(MtoAVisibilityWidget, self).__init__(parent)
        self.setLayout(QtWidgets.QVBoxLayout())
        # self.layout().setContentsMargins(0, 0, 0, 0)
        self.node = None
        self.pattern = None
        self.attr = None

        self.vis_boxes = []
        # make a box per visibility option
        for name, ray in RAYS:
            if ray != AI_RAY_SUBSURFACE:
                _visBox = MtoAVisibilityCheckBox(' '.join(name.split('_')[2:]).title())
                _visBox.setChecked(True)
                _visBox.VIS_RAY = ray
                self.layout().addWidget(_visBox)
                self.vis_boxes.append(_visBox)
                _visBox.stateChanged.connect(self.emitValue)

        self.setValue(value)

    def getValue(self):

        vis = AI_RAY_ALL
        for cb in self.vis_boxes:
            if not cb.isChecked():
                vis &= ~cb.VIS_RAY
        return vis

    def setValue(self, value):
        vis_dict = getVisibilityDict(int(value))
        for vis_box in self.vis_boxes:
            vis_box.setChecked(vis_dict[vis_box.VIS_RAY])

    def emitValue(self, *args):
        self.valueChanged.emit(self.getValue())


class MtoARGBControl(QtWidgets.QFrame):

    valueChanged = QtCore.Signal(float, float, float)

    def __init__(self, value=True, parent=None):
        super(MtoARGBControl, self).__init__(parent)

        # color rect
        
        # RGB float boxes


class MtoACheckbox(QtWidgets.QCheckBox):

    valueChanged = QtCore.Signal(bool)

    def __init__(self, value=True, parent=None):
        super(MtoACheckbox, self).__init__(parent)
        self.setValue(value)
        self.stateChanged.connect(self.emitValueChanged)

    def emitValueChanged(self, value):
        self.valueChanged.emit(bool(value))

    def getValue(self):
        return self.isChecked()

    def setValue(self, value):
        s_value = str(value).lower()
        b_value = s_value not in ['false', 'off', '0']
        self.setChecked(b_value)


class MtoAComboBox(QtWidgets.QComboBox):

    valueChanged = QtCore.Signal(str)

    def __init__(self, parent=None):
        super(MtoAComboBox, self).__init__(parent)
        self.currentTextChanged .connect(self.emitValueChanged)

    def emitValueChanged(self, text):
        self.valueChanged.emit(text)

    def getValue(self):
        return self.currentText()

    def setValue(self, value):
        m = STRING_EXP.match(str(value))
        if m:
            self.setCurrentText(m.group(1))


class MtoAIntControl(QtWidgets.QSpinBox):

    def __init__(self, parent=None):
        super(MtoAIntControl, self).__init__(parent)

    def setValue(self, value):
        super(MtoAIntControl, self).setValue(int(value))


class MtoAFltControl(QtWidgets.QDoubleSpinBox):
    def __init__(self, parent=None):
        super(MtoAFltControl, self).__init__(parent)

    def setValue(self, value):
        super(MtoAFltControl, self).setValue(float(value))


class MtoAStrControl(QtWidgets.QLineEdit):

    valueChanged = QtCore.Signal(str)

    def __init__(self, parent=None):
        super(MtoAStrControl, self).__init__(parent)

        self.textEdited.connect(self.emitValueChanged)

    def emitValueChanged(self, text):
        self.valueChanged.emit(text)

    def getValue(self):
        return self.text()

    def setValue(self, value):
        m = STRING_EXP.match(value)
        if m:
            self.setText(m.group(1))


class MtoALabelLineEdit(MayaQWidgetBaseMixin, QtWidgets.QFrame):

    valueChanged = QtCore.Signal(str)

    def __init__(self, label='label', parent=None):
        super(MtoALabelLineEdit, self).__init__(parent)

        self.setLayout(QtWidgets.QHBoxLayout())
        self.layout().setContentsMargins(0, 0, 0, 0)

        self.titleLabel = QtWidgets.QLabel(label)
        self.layout().addWidget(self.titleLabel)
        self.lineEdit = QtWidgets.QLineEdit()
        self.layout().addWidget(self.lineEdit)

    def getText(self):
        return self.lineEdit.text()

    def setText(self, text):
        return self.lineEdit.setText(text)


class MtoANodeConnectionWidget(MtoALabelLineEdit):

    CONNECTED_ICON = BaseItem.dpiScaledIcon(":/navButtonConnected.png")
    UNCONNECTED_ICON = BaseItem.dpiScaledIcon(":/navButtonUnconnected.png")

    INHERITED_ICON = BaseItem.dpiScaledIcon(":/expandInfluenceList.png")

    nodeDisconnected = QtCore.Signal()
    connectionButtonClicked = QtCore.Signal()
    menuTriggered = QtCore.Signal(object)
    overrideTriggered = QtCore.Signal()

    def __init__(self, label='label', nodeType="shader", parent=None):
        super(MtoANodeConnectionWidget, self).__init__(label, parent)

        self.node = None

        # attribute for adding custom user data
        self.data = {}

        self.overrideButton = MtoAButton(self, self.INHERITED_ICON, dpiScale(15))
        self.overrideButton.setEnabled(False)
        self.overrideButton.setToolTip("Override Assignment")
        self.layout().insertWidget(0, self.overrideButton)
        self.overrideButton.clicked.connect(self.emitOverrideTriggered)

        self.conButton = MtoAButton(self, self.UNCONNECTED_ICON, dpiScale(15))
        self.layout().addWidget(self.conButton)

        self._orginalStyle = self.frameStyle()
        self._orginalWidth = self.frameWidth()
        self.conButton.clicked.connect(self.connectionButtonClicked)

        self.menu = QtWidgets.QMenu()
        disconnectAction = self.menu.addAction("Disconnect")
        disconnectAction.triggered.connect(self.disconnectNode)
        selectAction = self.menu.addAction("Select")
        selectAction.triggered.connect(self.selectNode)
        self.setMenu(self.menu)

        self.setAcceptDrops(True)

        self.lineEdit.editingFinished.connect(self.manualSet)

    def emitOverrideTriggered(self):
        self.overrideTriggered.emit()

    def setMenu(self, menu):
        self.menu = menu
        self.menu.triggered.connect(self.menuTriggered.emit)

    def menu(self):
        return self.menu

    def contextMenuEvent(self, event):
        if self.menu:
            self.menu.exec_(event.globalPos())

    def manualSet(self):
        self.setNode(self.getText())

    def setNode(self, node, emit=True):
        if node and node != '':
            self.node = node
            self.setText(node)
            self.conButton.setIcon(self.CONNECTED_ICON)
            self.conButton.clicked.disconnect()
            self.conButton.clicked.connect(self.selectNode)
        if emit:
            self.valueChanged.emit(node)

    def setInherited(self, inherited):
        self.overrideButton.setEnabled(inherited)

    def disconnectNode(self, emit=True):
        self.node = None
        self.setText("")
        self.conButton.setIcon(self.UNCONNECTED_ICON)
        self.conButton.clicked.disconnect()
        self.conButton.clicked.connect(self.connectionButtonClicked)
        if emit:
            self.nodeDisconnected.emit()

    def selectNode(self):
        if self.node:
            cmds.select(self.node, r=True)

    def dragEnterEvent(self, event):
        """ Reimplementing event to accept plain text, """
        self.setFrameStyle(QtWidgets.QFrame.Box)
        self.setLineWidth(1)
        if event.mimeData().hasFormat('text/plain'):
            event.accept()
        else:
            event.ignore()

    def resetStyle(self):
        self.setFrameStyle(self._orginalStyle)
        self.setLineWidth(self._orginalWidth)

    def dragLeaveEvent(self, event):
        """ Reimplementing event to accept plain text, """
        self.resetStyle()

    def dragMoveEvent(self, event):
        """ Reimplementing event to accept plain text, """
        if event.mimeData().hasFormat('text/plain'):
            event.accept()
        else:
            event.ignore()

    def dropEvent(self, event):
        node = event.mimeData().text()
        if cmds.objExists(node):
            self.setNode(node)
        self.resetStyle()


class MtoAMutiControlWidget(MayaQWidgetBaseMixin, QtWidgets.QFrame):
    """docstring for MtoAMutiControlWidget"""
    valueChanged = QtCore.Signal((str,),
                                 (int,),
                                 (bool,),
                                 (float,),)

    def __init__(self, parent=None):
        super(MtoAMutiControlWidget, self).__init__(parent)
        self.setLayout(QtWidgets.QVBoxLayout())
        self.layout().setContentsMargins(0, 0, 0, 0)
        self.control = None

    def setWidget(self, widget):
        self.control = widget
        self.layout().addWidget(widget)
        # self.control.valueChanged.connect(self.emitValueChanged)

    def emitValueChanged(self):
        value = self.getValue
        self.valueChanged.emit(value)

    def getValue(self):
        if self.control:
            return self.control.getValue()

    def setValue(self, value):
        if self.control and not valueIsExpression(value):
            self.control.setValue(value)


class MtoAOperatorOverrideWidget(MayaQWidgetBaseMixin, QtWidgets.QFrame):

    EXPRESSION_ICON = QtGui.QIcon(":/expression.svg")
    BIN_ICON = QtGui.QIcon(":/RS_delete.png")
    INHERITED_ICON = BaseItem.dpiScaledIcon(":/expandInfluenceList.png")

    deleteMe = QtCore.Signal(object)
    paramChanged = QtCore.Signal(str)
    #                             param, operation, value, param_type, custom, index, operator
    valueChanged = QtCore.Signal((str, str, str, int, bool, int, str),
                                 (str, str, int, int, bool, int, str),
                                 (str, str, bool, int, bool, int, str),
                                 (str, str, float, int, bool, int, str))
    overrideTriggered = QtCore.Signal(str)

    def __init__(self, paramType=None, param=None, op=None, value=None, paramDict={}, parent=None):
        super(MtoAOperatorOverrideWidget, self).__init__(parent)

        self.setObjectName("MtoAOperatorOverrideWidget")
        # set the paramater type so we know what type of widget to create.
        self.param_name = None
        self.param_type = None
        self.user_param = False
        self.param_dict = paramDict
        self.index = -1
        self.operator = ""
        self.setLayout(QtWidgets.QHBoxLayout())
        self.layout().setContentsMargins(0, 0, 0, 0)

        self.overrideButton = MtoAButton(self, self.INHERITED_ICON, dpiScale(15))
        self.overrideButton.setEnabled(False)
        self.overrideButton.setToolTip("Override Assignment")
        self.layout().insertWidget(0, self.overrideButton, alignment=QtCore.Qt.AlignTop)
        self.overrideButton.clicked.connect(self.emitOverrideTriggered)

        self.paramWidget = MtoAParamBox(self)
        self.layout().addWidget(self.paramWidget, alignment=QtCore.Qt.AlignTop)
        # self.paramWidget.setVisible(False)
        self.populateParams(self.param_dict)

        self.op_menu = QtWidgets.QComboBox()
        self.op_menu.setObjectName("MtoAOperatorChoice")
        self.op_menu.setContentsMargins(0, 10, 0, 0)
        self.op_menu.addItems(OPERATIONS)
        self.layout().addWidget(self.op_menu, alignment=QtCore.Qt.AlignTop)

        self.valueWidget = QtWidgets.QStackedWidget()  # set the widget for this control
        self.valueWidget.setContentsMargins(0, 0, 0, 0)
        self.layout().addWidget(self.valueWidget, alignment=QtCore.Qt.AlignTop)

        self.controlWidget = MtoAMutiControlWidget()
        self.valueWidget.addWidget(self.controlWidget)

        self.exp_panel = QtWidgets.QFrame()
        self.exp_panel.setLayout(QtWidgets.QVBoxLayout())
        self.expressionEditor = QtWidgets.QLineEdit()
        self.exp_panel.layout().addWidget(self.expressionEditor)
        self.exp_panel.layout().setContentsMargins(0, 0, 0, 0)
        self.exp_panel.layout().insertStretch(-1)
        self.valueWidget.addWidget(self.exp_panel)

        # self.buttonsPanel = QtWidgets.QToolBar()

        self.expBtn = MtoACheckableButton(self, self.EXPRESSION_ICON, dpiScale(20))  # self.buttonsPanel.addAction(self.EXPRESSION_ICON, "Toggle Expression")
        self.expBtn.setCheckable(True)
        self.layout().addWidget(self.expBtn, alignment=QtCore.Qt.AlignTop)

        self.delBtn = MtoAButton(self, self.BIN_ICON, dpiScale(20))  # self.buttonsPanel.addAction(self.BIN_ICON, "Delete")

        self.layout().addWidget(self.delBtn, alignment=QtCore.Qt.AlignTop)

        if paramType:
            self.setParamType(paramType)
            self.user_param = True
        if param:
            self.setParam(param)
            self.setControlWidget(param)
        if op:
            self.setOperation(op)
        if value:
            oldState = self.controlWidget.control.blockSignals(True)
            self.setValue(value)
            self.controlWidget.control.blockSignals(oldState)

        self.setup()

    def setup(self):
        self.delBtn.clicked.connect(self.callDeleteMe)
        self.paramWidget.paramChanged.connect(self.emitParamChanged)
        self.paramWidget.paramTypeChanged.connect(self.emitParamTypeChanged)
        self.expBtn.toggled.connect(self.valueWidget.setCurrentIndex)
        self.expressionEditor.textEdited.connect(self.emitExpressionValueChanged)
        self.op_menu.currentTextChanged.connect(self.emitOperationChanged)

    def setInherited(self, inherited):
        self.overrideButton.setEnabled(inherited)

    def callDeleteMe(self):
        self.deleteMe.emit(self)

    def emitParamChanged(self, param):
        current_param = self.getParam()
        if current_param == param:
            return

        default_value = ""
        param_data = self.getParamData(param)
        if param_data:
            default_value = param_data[DEFAULT_VALUE]
            self.user_param = False
        self.param_name = param
        self.setControlWidget(param)
        self.setValue(default_value)
        self.emitValueChanged(default_value)

    def emitParamTypeChanged(self, paramType):
        current_paramType = self.getParamType()
        param = self.getParam()
        arn_type = TYPES_DICT[paramType]
        if current_paramType == arn_type:
            return

        self.param_type = arn_type
        self.setControlWidget(param)
        self.setValue(TYPES_DEFAULT[arn_type])
        self.emitValueChanged(TYPES_DEFAULT[arn_type])

    def emitOverrideTriggered(self):
        param = self.getParam()
        self.overrideTriggered.emit(param)

    def emitValueChanged(self, value):

        param = self.getParam()
        self.valueChanged[str, str, type(value), int, bool, int, str].emit(
                                   param,
                                   self.getOperation(),
                                   value,
                                   self.param_type,
                                   self.user_param,
                                   self.index,
                                   self.operator)
        self.expressionEditor.setText(str(value))

    def emitExpressionValueChanged(self, value):
        param = self.getParam()
        self.valueChanged[str, str, type(value), int, bool, int, str].emit(
                                   param,
                                   self.getOperation(),
                                   value,
                                   self.param_type,
                                   self.user_param,
                                   self.index,
                                   self.operator)

    def emitOperationChanged(self, operation):
        param = self.getParam()
        value = self.getValue()
        self.valueChanged[str, str, type(value), int, bool, int, str].emit(
                                   param,
                                   operation,
                                   value,
                                   self.param_type,
                                   self.user_param,
                                   self.index,
                                   self.operator)

    def populateParams(self, paramDict):

        for node_type, params in sorted(paramDict.items()):
            if node_type != 'hidden':
                for param, data in sorted(params.items()):
                    self.paramWidget.addItem(param, node_type)

    def getParamData(self, param):
        for node_type, params in self.param_dict.items():
            if param in params:
                return params[param]

        return None

    def getParam(self):
        return self.param_name

    def setParam(self, param):
        oldState = self.paramWidget.editBox.blockSignals(True)
        self.param_name = param
        self.paramWidget.setText(param)
        self.setControlWidget(param)
        self.paramWidget.editBox.blockSignals(oldState)

    def getParamType(self):
        return self.param_type

    def setParamType(self, param_type):
        self.param_type = TYPES_DICT[param_type]
        self.paramWidget.setParamType(param_type)

    def setParamTypeFromParam(self, param):
        param_data = self.getParamData(param)
        if param_data:
            self.param_type = param_data[PARAM_TYPE]

    def getOperation(self):
        return self.op_menu.currentText()

    def setOperation(self, operation):
        return self.op_menu.setCurrentText(operation)

    def getValue(self):
        if self.controlWidget and self.valueWidget.currentIndex() == 0:
            return self.controlWidget.value()
        else:
            return self.expressionEditor.text()

    def setValue(self, value):
        self.controlWidget.setValue(value)
        self.expressionEditor.setText(str(value))
        if valueIsExpression(value):
            # show the expression editor
            self.expBtn.setChecked(True)
            self.valueWidget.setCurrentIndex(1)

    def setControlWidget(self, param):
        # make sure the param type is updated
        if not self.user_param:
            self.setParamTypeFromParam(param)
        # delete old widgets
        clearWidget(self.controlWidget)
        control = None
        if self.param_type in [AI_TYPE_BYTE] and param in ['visibility', 'sidedness']:
            control = MtoAVisibilityWidget()
        elif self.param_type in [AI_TYPE_INT, AI_TYPE_BYTE, AI_TYPE_UINT]:
            control = MtoAIntControl()
        elif self.param_type is AI_TYPE_FLOAT:
            control = MtoAFltControl()
        elif self.param_type is AI_TYPE_BOOLEAN:
            control = MtoACheckbox()
        elif self.param_type is AI_TYPE_ENUM:
            control = MtoAComboBox()
            # populate the options
            param_data = self.getParamData(param)
            for i in param_data[ENUM_VALUES]:
                    control.addItem(i)
        else:
            control = MtoAStrControl()

        control.valueChanged.connect(self.emitValueChanged)
        self.controlWidget.setWidget(control)

        # hide the ops menu if the param is not a digit
        if self.param_type not in [AI_TYPE_INT, AI_TYPE_BYTE, AI_TYPE_UINT, AI_TYPE_FLOAT]:
            self.setOperation("=")
            self.op_menu.setVisible(False)
        else:
            self.op_menu.setVisible(True)


class MtoAParamBox(QtWidgets.QFrame):
    """docstring for MtoAMenuBox"""
    PARAM_ICON = QtGui.QPixmap(":/list.svg")

    paramChanged = QtCore.Signal(str)
    paramTypeChanged = QtCore.Signal(str)

    def __init__(self, parent):
        super(MtoAParamBox, self).__init__(parent)

        self.setLayout(QtWidgets.QHBoxLayout())
        self.layout().setContentsMargins(0, 0, 0, 0)

        self.menuButton = QtWidgets.QPushButton()
        self.menuButton.setIcon(self.PARAM_ICON)
        setStaticSize(self.menuButton, 20, 16)
        self.menu = QtWidgets.QMenu()
        self.menuButton.setMenu(self.menu)
        self.layout().addWidget(self.menuButton)

        self.rootMenus = []

        self.paramTypeBox = QtWidgets.QComboBox()
        self.paramTypeBox.addItems(TYPES)
        self.paramTypeBox.setVisible(False)
        self.layout().addWidget(self.paramTypeBox)

        self.editBox = QtWidgets.QLineEdit()
        self.editBox.setReadOnly(True)
        self.layout().addWidget(self.editBox)

        self.setup()

    def setup(self):
        self.menu.triggered.connect(self.setTextFromAction)
        self.paramTypeBox.currentTextChanged.connect(self.emitParamTypeChanged)
        self.editBox.editingFinished.connect(self.emitChange)

    def setParamType(self, paramType):
        self.paramTypeBox.setVisible(True)
        self.paramTypeBox.setCurrentText(paramType)

    def emitParamTypeChanged(self, paramType):
        self.paramTypeChanged.emit(paramType)

    def emitChange(self):
        self.paramChanged.emit(self.getText())

    def setTextFromAction(self, action):
        new_text = action.text()
        self.setText(new_text)
        if new_text != "custom":
            self.paramTypeBox.setVisible(False)
        self.paramChanged.emit(new_text)

    def setText(self, text):
        self.editBox.setText(text)

    def getText(self):
        return self.editBox.text()

    def addItem(self, item, parent):
        parent_menu = self.menu
        if parent:
            # check if a root item exists
            for sub_menu in self.rootMenus:
                    if sub_menu.title() == parent:
                        parent_menu = sub_menu
            if parent_menu == self.menu:
                parent_menu = self.menu.addMenu(parent)
                self.rootMenus.append(parent_menu)
        if parent_menu:
            parent_menu.addAction(item)

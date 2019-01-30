from .Qt import OpenMayaUI
from .Qt import QtCore
from .Qt import QtGui
from .Qt import QtWidgets
from .itemStyle import ItemStyle
from .treeView import BaseItem
from .utils import dpiScale, setStaticSize, clearWidget

from maya.app.general.mayaMixin import MayaQWidgetBaseMixin
import maya.cmds as cmds
import re

from arnold import *

(NODE_TYPE,
 PARAM_TYPE,
 DEFAULT_VALUE,
 NODE_ENTRY,
 IS_ARRAY,
 ENUM_VALUES) = range(6)

OPERATIONS = ["=", "+=", "-=", "*=", "/=", "^=", "%="]

TYPES = ["string", "int", "byte", "uint", "float", "bool", "node"]
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
        vis_dict = getVisibilityDict(value)
        for vis_box in self.vis_boxes:
            vis_box.setChecked(vis_dict[vis_box.VIS_RAY])

    def emitValue(self, *args):
        self.valueChanged.emit(self.getValue())


class MtoARGBControl(QtWidgets.QFrame):

    valueChanged = QtCore.Signal(float, float, float)

    def __init__(self, value=True, parent=None):
        super(MtoARGBControl, self).__init__(parent)


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
        self.setChecked(value)


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
        m = STRING_EXP.match(value)
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

    valueChanged = QtCore.Signal(str)
    nodeDisconnected = QtCore.Signal()
    connectionButtonClicked = QtCore.Signal()
    menuTriggered = QtCore.Signal(object)

    def __init__(self, label='label', nodeType="shader", parent=None):
        super(MtoANodeConnectionWidget, self).__init__(label, parent)

        self.node = None

        self.conButton = QtWidgets.QPushButton()
        self.conButton.setFlat(True)
        self.conButton.setIcon(self.UNCONNECTED_ICON)
        setStaticSize(self.conButton, 16, 16)
        self.layout().addWidget(self.conButton)

        self._orginalStyle = self.frameStyle()
        self._orginalWidth = self.frameWidth()
        self.conButton.clicked.connect(self.connectionButtonClicked)

        self.menu = None

        menu = QtWidgets.QMenu()
        menu.addAction("Disconnect")
        self.setMenu(menu)

        self.setAcceptDrops(True)

        self.lineEdit.textEdited.connect(self.valueChanged)

    def setMenu(self, menu):
        self.menu = menu
        self.menu.triggered.connect(self.menuTriggered.emit)

    def contextMenuEvent(self, event):
        if self.menu:
            self.menu.exec_(event.globalPos())

    def setNode(self, node, emit=True):
        self.node = node
        self.setText(node)
        self.conButton.setIcon(self.CONNECTED_ICON)
        self.conButton.clicked.disconnect()
        self.conButton.clicked.connect(self.selectNode)
        if emit:
            self.valueChanged.emit(node)

    def disconnectNode(self):
        self.node = None
        self.setText("")
        self.conButton.setIcon(self.UNCONNECTED_ICON)
        self.conButton.clicked.disconnect()
        self.conButton.clicked.connect(self.connectionButtonClicked)
        self.nodeDisconnected.emit()

    def selectNode(self):
        if self.node:
            cmds.select(node, r=True)

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
        if self.control:
            self.control.setValue(value)


class MtoAOperatorOverrideWidget(MayaQWidgetBaseMixin, QtWidgets.QToolBar):

    EXPRESSION_ICON = QtGui.QIcon(":/expression.svg")
    BIN_ICON = QtGui.QIcon(":/deleteActive.png")

    deleteMe = QtCore.Signal(object)
    paramChanged = QtCore.Signal(str)
    valueChanged = QtCore.Signal((str, str, str, int,),
                                 (str, str, int, int,),
                                 (str, str, bool, int,),
                                 (str, str, float, int,))

    def __init__(self, parent=None):
        super(MtoAOperatorOverrideWidget, self).__init__(parent)

        # set the paramater type so we know what type of widget to create.
        self.param_name = None
        self.param_type = None
        self.param_dict = {}
        self.index = -1

        self.paramWidget = MtoAParamBox(self)
        self.addWidget(self.paramWidget)

        self.op_menu = QtWidgets.QComboBox()
        self.op_menu.addItems(OPERATIONS)
        self.addWidget(self.op_menu)

        self.valueWidget = QtWidgets.QStackedWidget()  # set the widget for this control
        self.addWidget(self.valueWidget)

        self.controlWidget = MtoAMutiControlWidget()
        self.valueWidget.addWidget(self.controlWidget)

        self.expressionEditor = QtWidgets.QLineEdit()
        self.valueWidget.addWidget(self.expressionEditor)

        self.expBtn = self.addAction(self.EXPRESSION_ICON, "Toggle Expression")
        self.expBtn.setCheckable(True)

        self.delBtn = self.addAction(self.BIN_ICON, "Delete")

        self.addAction(self.delBtn)

        self.setup()

    def setup(self):
        self.delBtn.triggered.connect(self.callDeleteMe)
        self.paramWidget.paramChanged.connect(self.emitParamChanged)
        self.expBtn.toggled.connect(self.valueWidget.setCurrentIndex)

    def callDeleteMe(self):
        self.deleteMe.emit(self)

    def emitParamChanged(self, param):
        default_value = ""
        param_data = self.param_dict.get(param, None)
        if param_data:
            default_value = param_data[2]

        self.setControlWidget(param)
        self.setValue(default_value)
        self.emitValueChanged(default_value)

    def emitValueChanged(self, value):

        param = self.getParam()
        self.valueChanged[str, str, type(value), int].emit(
                                   param,
                                   self.getOperation(),
                                   value,
                                   self.index)

    def populateParams(self, paramDict):
        for param, data in sorted(paramDict.items()):
            self.paramWidget.addItem(param, data[0])

    def getParam(self):
        return self.paramWidget.getText()

    def setParam(self, param, param_type, node_entry, param_dict):
        self.paramWidget.setText(param)
        self.param_name = param
        self.param_type = param_type
        self.node_entry = node_entry
        self.param_dict = param_dict
        self.setControlWidget(param)

    def setParamType(self, param):
        param_data = self.param_dict.get(param, None)
        if param_data:
            self.param_type = param_data[PARAM_TYPE]

    def getOperation(self):
        return self.op_menu.currentText()

    def setOperation(self, operation):
        return self.op_menu.setCurrentText(operation)

    def getValue(self):
        if self.controlWidget:
            return self.controlWidget.value()
        else:
            return self.expressionEditor.text()

    def setValue(self, value):
        self.controlWidget.setValue(value)
        self.expressionEditor.setText(str(value))

    def setControlWidget(self, param):
        # make sure the param type is updated
        self.setParamType(param)
        # delete old widgets
        clearWidget(self.controlWidget)
        control = None
        if self.param_type in [AI_TYPE_BYTE] and param == 'visibility':
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
            for i in self.param_dict[param][-1]:
                    control.addItem(i)
        else:
            control = MtoAStrControl()

        control.valueChanged.connect(self.emitValueChanged)
        self.controlWidget.setWidget(control)


class MtoAParamBox(QtWidgets.QToolBar):
    """docstring for MtoAMenuBox"""
    PARAM_ICON = QtGui.QPixmap(":/list.svg")

    paramChanged = QtCore.Signal(str)

    def __init__(self, parent):
        super(MtoAParamBox, self).__init__(parent)

        # self.setLayout(QtWidgets.QHBoxLayout())
        # self.layout().setContentsMargins(0, 0, 0, 0)

        self.menuButton = QtWidgets.QPushButton()
        self.menuButton.setIcon(self.PARAM_ICON)
        self.menu = QtWidgets.QMenu()
        self.menuButton.setMenu(self.menu)
        self.menuAction = self.addWidget(self.menuButton)

        self.paramTypeBox = QtWidgets.QComboBox()
        self.paramTypeBox.addItems(TYPES)
        self.paramTypeAction = self.addWidget(self.paramTypeBox)
        self.paramTypeAction.setVisible(False)

        self.editBox = QtWidgets.QLineEdit()
        self.editBox.setReadOnly(True)
        self.addWidget(self.editBox)

        self.setup()

    def setup(self):
        self.menu.triggered.connect(self.setTextFromAction)
        self.editBox.textEdited.connect(self.emitChange)
        self.paramTypeBox.hide()

    def emitChange(self):
        self.paramChanged.emit(self.getText())

    def setTextFromAction(self, action):
        new_text = action.text()
        self.editBox.setText(new_text)
        self.paramChanged.emit(new_text)

    def setText(self, text):
        self.editBox.setText(text)

    def getText(self):
        return self.editBox.text()

    def addItem(self, item, parent):
        parent_menu = self.menu
        if parent:
            # check if a root item exists
            for action in self.menu.actions():
                if action.menu():
                    sub_menu = action.menu()
                    if sub_menu.title() == parent:
                        parent_menu = sub_menu
            if parent_menu == self.menu:
                parent_menu = self.menu.addMenu(parent)
        if parent_menu:
            parent_menu.addAction(item)

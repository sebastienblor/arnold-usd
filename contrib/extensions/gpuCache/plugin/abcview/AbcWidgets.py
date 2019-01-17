from mtoa.ui.qt.Qt import QtWidgets
from mtoa.ui.qt.Qt import QtGui
from mtoa.ui.qt.Qt import QtCore
from mtoa.ui.qt import setStaticSize
from mtoa.ui.qt.widgets import *

from AbcTransverser import ABC_PATH, ABC_NAME, ABC_PARENT, ABC_VISIBILITY, \
                            ABC_INSTANCEPATH, ABC_ENTIY_TYPE, ABC_IOBJECT


class AbcPropertiesPanel(QtWidgets.QFrame):
    """SubClass properties panel for displying properties of an object"""

    propertyChanged = QtCore.Signal(str, object)

    def __init__(self, parent=None):
        super(AbcPropertiesPanel, self).__init__(parent)

        self.node = None
        self.object = None

        self.layout = QtWidgets.QVBoxLayout(self)
        self.setLayout(self.layout)
        self.object_label = QtWidgets.QLabel("Select Item")
        self.layout.addWidget(self.object_label)

        # AddOverride Button
        self.toolBar = QtWidgets.QFrame()
        self.toolBar.setLayout(QtWidgets.QHBoxLayout())
        self.layout.addWidget(self.toolBar)
        self.addOverideButton = QtWidgets.QPushButton("Add Override")
        self.toolBar.layout().addWidget(self.addOverideButton)
        self.toolBar.layout().insertStretch(-1)

        self.shadingPanel = QtWidgets.QFrame()
        self.shadingPanel.setLayout(QtWidgets.QVBoxLayout())

        # shader override
        self.shaderOverrideWidget = MtoALabelLineEdit("Shader")
        self.shadingPanel.layout().addWidget(self.shaderOverrideWidget)
        # displacement override
        self.dispOverrideWidget = MtoALabelLineEdit("Displacement")
        self.shadingPanel.layout().addWidget(self.dispOverrideWidget)

        self.layout.addWidget(self.shadingPanel)

        self.overridesPanel = QtWidgets.QFrame()
        self.overridesPanel.setLayout(QtWidgets.QVBoxLayout())
        self.layout.addWidget(self.overridesPanel)

        tmp_widget = MtoAOperatorOverrideWidget()
        self.overridesPanel.layout().addWidget(tmp_widget)

    def setObject(self, node, obj):
        self.node = node
        self.object = obj
        self.object_label.setText(obj[ABC_PATH])

    def getProperties(self, obj):
        pass

    def addOverride(self, param, value):
        pass

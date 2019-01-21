
from mtoa.ui.qt.Qt import QtWidgets
from mtoa.ui.qt.Qt import QtGui
from mtoa.ui.qt.Qt import QtCore
from mtoa.ui.qt import setStaticSize, clearWidget
from mtoa.ui.qt.widgets import *

from AbcTransverser import ABC_PATH, ABC_NAME, ABC_PARENT, ABC_VISIBILITY, \
                            ABC_INSTANCEPATH, ABC_ENTIY_TYPE, ABC_IOBJECT


class AbcPropertiesPanel(QtWidgets.QFrame):
    """SubClass properties panel for displying properties of an object"""

    propertyChanged = QtCore.Signal(str, object)

    def __init__(self, transverser, parent=None):
        super(AbcPropertiesPanel, self).__init__(parent)

        self.node = None
        self.object = None
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
        self.toolBar.layout().addWidget(self.addOverideButton)
        self.toolBar.layout().insertStretch(-1)

        self.addOverideButton.clicked.connect(self.addOverride)

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

    def setObject(self, node, obj):
        print "setObject", node, obj
        self.node = node
        self.object = obj
        self.object_label.setText(obj[ABC_PATH])
        self.getOverrides()

    def getOverrides(self):
        clearWidget(self.overridesPanel)
        for override in self.transverser.getOverrides(self.node, self.object[ABC_PATH]):
            self.addOverride()

    def getProperties(self, obj):
        pass

    def addOverride(self):
        op = self.transverser.getOperator(self.node, self.object[ABC_PATH])
        if not op:
            print "addOverride", self.node, self.object[ABC_PATH]
            op = self.transverser.createOperator(self.node, self.object)
        self.setOverride(None, None)
        new_widget = MtoAOperatorOverrideWidget(self.overridesPanel)
        new_widget.deleteMe.connect(self.removeOverride)
        self.overridesPanel.layout().addWidget(new_widget)
        return new_widget

    def removeOverride(self, widget):
        index = self.overridesPanel.layout().indexOf(widget)
        if self.transverser.deleteOverride(self.node, self.object[ABC_PATH], index):
            self.overridesPanel.layout().removeWidget(widget)
            widget.deleteLater()

    def setOverride(self, param, value):
        if not param and not value:
            param = "NEWOVERRIDE"
            value = "NEWVALUE"

        return self.transverser.setOverride(self.node, self.object[ABC_PATH], param, value)

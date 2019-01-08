"""A set of useful utils."""

from .Qt import __binding__ as qtBinding
from .Qt import OpenMayaUI
from .Qt import QtCore
from .Qt import QtGui
from .Qt import QtWidgets
from .Qt import shiboken
from .Qt import cmds

_DPI_SCALE = \
    1.0 \
    if not hasattr(cmds, "mayaDpiSetting") else \
    cmds.mayaDpiSetting(query=True, realScaleValue=True)


def dpiScale(value):
    """Scale the value according to the current DPI of the current monitor."""
    return _DPI_SCALE * value


def toPyObject(obj):
    """Call QVariant.toPyObject on the case we are in PyQt."""
    # QAbstractItemModel.data() returns QVariant in PyQt and a python type in
    # PySide. The result of this function should be the same for all the Qts.
    if obj and qtBinding == "PyQt4":
        return obj.toPyObject()

    return obj


def toQtObject(mayaUIName):
    '''
    Given the name of a Maya UI element of any type,
    return the corresponding QWidget or QAction.
    If the object does not exist, returns None
    '''
    ptr = OpenMayaUI.MQtUtil.findControl(mayaUIName)
    if ptr is None:
        ptr = OpenMayaUI.MQtUtil.findLayout(mayaUIName)
        if ptr is None:
            ptr = OpenMayaUI.MQtUtil.findMenuItem(mayaUIName)

    if ptr is not None:
        obj = shiboken.wrapInstance(long(ptr), QObject)
        return obj


def clearWidget(widget):
    """clear children from given widget"""
    while widget.layout().count():
        item = widget.layout().takeAt(0)
        if isinstance(item, QWidgetItem):
            item.widget().close()
        elif isinstance(item, QSpacerItem):
            widget.layout().removeItem(item)


def setStaticSize(widget, width=0, height=0, posx=0, posy=0):
    if width:
        widget.setMinimumWidth(width)
        widget.setMaximumWidth(width)
    if height:
        widget.setMinimumHeight(height)
        widget.setMaximumHeight(height)

    if width and height and posx and posy:
        widget.setGeometry(width, height, width, height)


def getMayaWindow():
    """
    Get the main Maya window as a QMainWindow instance
    @return: QMainWindow instance of the top level Maya windows
    """
    ptr = OpenMayaUI.MQtUtil.mainWindow()
    if ptr is not None:
        return shiboken.wrapInstance(long(ptr), QtWidgets.QWidget)

"""A set of useful utils."""

from .Qt import __binding__ as qtBinding
from .Qt import OpenMayaUI
from .Qt import QtCore
from .Qt import QtGui
from .Qt import QtWidgets
from .Qt import shiboken
from .Qt import cmds

import re
import time

_DPI_SCALE = \
    1.0 \
    if not hasattr(cmds, "mayaDpiSetting") else \
    cmds.mayaDpiSetting(query=True, realScaleValue=True)


STRING_EXP = re.compile(r'[\'\"](.*)[\'\"]')

def busy_cursor(func):
    """ Decorator to change the mouse cursor to 'busy' while a function is processed.

    Args:
        func: Decorated function.
    """

    def busy(*args, **kwargs):
        # noinspection PyTypeChecker, PyArgumentList, PyCallByClass
        QtWidgets.QApplication.setOverrideCursor(QtGui.QCursor(QtCore.Qt.BusyCursor))
        try:
            return func(*args, **kwargs)
        finally:
            QtWidgets.QApplication.restoreOverrideCursor()

    return busy

def dpiScale(value):
    """Scale the value according to the current DPI of the current monitor."""
    if type(value) in [QtCore.QPoint, QtCore.QPointF]:
        result = tuple([_DPI_SCALE * x for x in value.toTuple()])
        return type(value)(*result)
    return _DPI_SCALE * value


def dpiScaledIcon(path):
    """Creates QPixmap and scales it for hi-dpi mode"""
    icon = QtGui.QPixmap(path)

    scale = dpiScale(1.0)
    if scale > 1.0:
        icon = icon.scaledToHeight(
            icon.height() * scale,
            QtCore.Qt.SmoothTransformation)

    return icon


def toPyObject(obj):
    """Call QVariant.toPyObject on the case we are in PyQt."""
    # QAbstractItemModel.data() returns QVariant in PyQt and a python type in
    # PySide. The result of this function should be the same for all the Qts.
    if obj and qtBinding == "PyQt4":
        return obj.toPyObject()

    return obj


def toQtObject(mayaUIName, pySideType=QtCore.QObject):
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
        obj = shiboken.wrapInstance(long(ptr), pySideType)
        return obj


def toMayaName(qtObject):
    '''
    Given a QtWidget get the maya name
    '''
    return OpenMayaUI.MQtUtil.fullName(long(shiboken.getCppPointer(qtObject)[0]))


def clearWidget(widget):
    """clear children from given widget"""
    item = widget.layout().takeAt(0)
    while item:
        del item
        item = widget.layout().takeAt(0)


def setStaticSize(widget, width=0, height=0, posx=0, posy=0):
    if width:
        widget.setMinimumWidth(dpiScale(width))
        widget.setMaximumWidth(dpiScale(width))

    if height:
        widget.setMinimumHeight(dpiScale(height))
        widget.setMaximumHeight(dpiScale(height))

    if width and height and posx and posy:
        widget.setGeometry(dpiScale(width),
                           dpiScale(height),
                           dpiScale(width),
                           dpiScale(height))


def valueIsExpression(value):
    """Check if the given value is a expression"""

    def isDigit(value):
        try:
            float(value)
            return True
        except (ValueError, TypeError):
            return False

    def isBoolean(value):
        if value.lower() in ['true', 'false', 'on', 'off', '0', '1']:
            return True
        return False

    if STRING_EXP.match(value):
        return False
    if isDigit(value):
        return False
    if isBoolean(value):
        return False
    if re.match(r'\[(\.?\d+(?:\.\d+)?\s?)*\]$', value):
        return False
    if re.match(r'\[(\.?\d+(?:\.\d+)?\s?)*\]\s+[\+\-\*/\^\%]?', value):
        return True
    if re.search(r'[@#][a-zA-Z0-9]*', value):
        return True
    if re.search(r'[@#][a-zA-Z0-9]*', value):
        return True
    if re.match(r'^(\.?\d+(?:\.\d+)?|(?:[@#][a-zA-Z0-9]*))\s*[\+\-\*/\^\%]?', value):
        return True

    return False


def getMayaWindow():
    """
    Get the main Maya window as a QMainWindow instance
    @return: QMainWindow instance of the top level Maya windows
    """
    ptr = OpenMayaUI.MQtUtil.mainWindow()
    if ptr is not None:
        return shiboken.wrapInstance(long(ptr), QtWidgets.QWidget)


class Timer:

    def __init__(self):
        self.start = time.time()

    def restart(self):
        self.start = time.time()

    def get_time_hhmmss(self):
        end = time.time()
        m, s = divmod(end - self.start, 60)
        h, m = divmod(m, 60)
        time_str = "%02d:%02d:%02d" % (h, m, s)
        return time_str

from .Qt import OpenMayaUI
from .Qt import QtCore
from .Qt import QtGui
from .Qt import QtWidgets
from .itemStyle import ItemStyle
from .utils import dpiScale
from .utils import getMayaWindow
from .treeView import BaseTreeView


class BaseWindow(QtWidgets.QMainWindow):

    def __init__(self, parent=None):
        super(BaseWindow, self).__init__(parent)

        self.applyStyle(ItemStyle(self.style()))


class TreeWindow(BaseWindow):
    """docstring for TreeWindow"""
    def __init__(self, arg, parent=None):
        super(TreeWindow, self).__init__(parent)
        self.arg = arg

        self.treeView = BaseTreeView(self)


def test():
    mayawindow = getMayaWindow()
    testwindow = TreeWindow(mayawindow)
    testwindow.show()

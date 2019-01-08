from .Qt import OpenMayaUI
from .Qt import QtCore
from .Qt import QtGui
from .Qt import QtWidgets
from .itemStyle import ItemStyle
from .utils import dpiScale
from .utils import getMayaWindow
from .treeView import BaseTreeView
from .codeEditor import BaseCodeEditor


class BaseWindow(QtWidgets.QMainWindow):

    def __init__(self, parent=None):
        super(BaseWindow, self).__init__(parent)

        self.setStyle(ItemStyle(self.style()))

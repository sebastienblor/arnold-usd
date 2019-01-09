from .Qt import OpenMayaUI
from .Qt import QtCore
from .Qt import QtGui
from .Qt import QtWidgets
from .itemStyle import ItemStyle
from .utils import dpiScale


class MtoADialog(QtWidgets.QDialog):

    def __init__(self, title="", parent=None, style=None):
        QDialog.__init__(self, parent)

        self.setWindowTitle(title)

        self.setObjectName("MtoADialog")
        self.setStyleSheet(self.styleSheet() + "MtoADialog{padding-left:0px}")

        self.setLayout(QVBoxLayout())


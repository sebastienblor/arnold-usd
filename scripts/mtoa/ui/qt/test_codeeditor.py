import sys
import os

__path = os.path.abspath(os.path.dirname(__file__)+'/..')

sys.path.append(__path)


from qt.Qt import QtWidgets, QtCore
from qt import dpiScale, getMayaWindow
from qt.window import CodeWindow


if __name__ == '__main__':

    import sys

    app = QtWidgets.QApplication(sys.argv)
    window = CodeWindow()
    window.resize(640, 512)
    window.show()
    sys.exit(app.exec_())

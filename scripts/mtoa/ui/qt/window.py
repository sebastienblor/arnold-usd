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


class TreeWindow(BaseWindow):
    """docstring for TreeWindow"""
    def __init__(self, parent=None):
        super(TreeWindow, self).__init__(parent)

        self.treeView = BaseTreeView(self)


EXAMPLE_CPP = """
#include <iostream>

int main()
{
  /* code */
  std::cout << "Hello World!" << "\\n";
  return 0;
}

"""


class CodeWindow(BaseWindow):
    """docstring for TreeWindow"""
    def __init__(self, parent=None):
        super(CodeWindow, self).__init__(parent)

        self.setUpEditor()
        self.setCentralWidget(self.editor)
        self.setWindowTitle("Example code editor window")

    def setUpEditor(self):
        self.editor = BaseCodeEditor(self)
        self.editor.setPlainText(EXAMPLE_CPP)

    def newFile(self):
        self.editor.clear()

    def openFile(self, path=None):
        if not path:
            path = QtWidgets.QFileDialog.getOpenFileName(self, "Open File",
                    '', "C++ Files (*.cpp *.h)")

        if path:
            inFile = QtCore.QFile(path[0])
            if inFile.open(QtCore.QFile.ReadOnly | QtCore.QFile.Text):
                text = inFile.readAll()

                try:
                    # Python v3.
                    text = str(text, encoding='ascii')
                except TypeError:
                    # Python v2.
                    text = str(text)

                self.editor.setPlainText(text)

    def setupFileMenu(self):
        fileMenu = QtWidgets.QMenu("&File", self)
        self.menuBar().addMenu(fileMenu)

        fileMenu.addAction("&New...", self.newFile, "Ctrl+N")
        fileMenu.addAction("&Open...", self.openFile, "Ctrl+O")
        fileMenu.addAction("E&xit", QtWidgets.qApp.quit, "Ctrl+Q")

    def setupHelpMenu(self):
        helpMenu = QtWidgets.QMenu("&Help", self)
        self.menuBar().addMenu(helpMenu)

        helpMenu.addAction("&About", self.about)
        helpMenu.addAction("About &Qt", QtWidgets.qApp.aboutQt)


def test():
    mayawindow = getMayaWindow()
    testwindow = TreeWindow(mayawindow)
    testwindow.show()


if __name__ == '__main__':

    import sys

    app = QtWidgets.QApplication(sys.argv)
    window = CodeWindow()
    window.resize(640, 512)
    window.show()
    sys.exit(app.exec_())

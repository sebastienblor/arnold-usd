import sys
import os

__path = os.path.abspath(os.path.dirname(__file__)+'/../..')
sys.path.append(__path)


from qt.Qt import QtWidgets, QtCore
from qt import dpiScale, getMayaWindow
from qt.window import BaseWindow
from qt.codeEditor import BaseCodeEditor
from qt.syntaxhighlighter import CppHighlighter


EXAMPLE_CPP = """
#include <iostream>

int main()
{
  /* code */
  std::cout << "Hello World!" << "\\n";
  return 0;
}

"""


class TestCodeWindow(BaseWindow):
    """docstring for TreeWindow"""
    def __init__(self, parent=None):
        super(TestCodeWindow, self).__init__(parent)

        self.setUpEditor()
        self.setWindowTitle("Example code editor window")

    def setUpEditor(self):
        self.editor = BaseCodeEditor(self, CppHighlighter)
        self.editor.setPlainText(EXAMPLE_CPP)
        self.setCentralWidget(self.editor)

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


if __name__ == '__main__':

    import sys

    app = QtWidgets.QApplication(sys.argv)
    window = TestCodeWindow()
    window.resize(640, 512)
    window.show()
    sys.exit(app.exec_())

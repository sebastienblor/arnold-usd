import sys
import os

__path = os.path.abspath(os.path.dirname(__file__)+'/../..')
sys.path.append(__path)

from qt.Qt import QtWidgets, QtCore
from qt import dpiScale, getMayaWindow
from qt.window import BaseWindow
from qt.treeView import BaseTreeView, BaseModel, BaseItem

TreeData = {"/":
            {"child1": {
             "sub-child1": {"sub1": {}, "sub2": {}, "sub3": {}, "sub4": {}}, "sub-child2": {}},
             "child2": {
             "sub-child1": {}, "sub-child2": {}}
             }
            }


class TestModel(BaseModel):
    """docstring for TestModel"""
    def __init__(self, parent):
        super(TestModel, self).__init__(parent)

    def refresh(self):
        def __walkPath(data, parent):
            for path in sorted(data.keys()):
                item = BaseItem(parent, path)
                __walkPath(data[path], item)

        self.beginResetModel()

        self.rootItem = BaseItem(None, "")

        __walkPath(TreeData, self.rootItem)

        self.endResetModel()


class TestTreeViewWindow(BaseWindow):

    def __init__(self, parent=None):
        super(TestTreeViewWindow, self).__init__(parent)

        self.tree = BaseTreeView()
        self.treeModel = TestModel(self.tree)
        self.tree.setModel(self.treeModel)

        self.setCentralWidget(self.tree)


if __name__ == '__main__':

    import sys
    app = QtWidgets.QApplication(sys.argv)
    window = TestTreeViewWindow()
    window.resize(640, 512)
    window.show()
    sys.exit(app.exec_())

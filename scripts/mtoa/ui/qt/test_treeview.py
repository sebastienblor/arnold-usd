import sys
import os

__path = os.path.abspath(os.path.dirname(__file__)+'/..')

sys.path.append(__path)


from qt.Qt import QtWidgets, QtCore
from qt import dpiScale, getMayaWindow
from qt.window import BaseWindow
from qt.treeView import BaseTreeView, BaseModel, BaseItem


TreeData = {"root":
            {"child1": {
             "sub-child1": {}, "sub-child2": {}},
             "child2": {
             "sub-child1": {}, "sub-child2": {}}
             }
            }


def walkPath(data, parent):
    for path in data.keys():
        item = BaseItem(parent, path)
        walkPath(data[path], item)


class TestModel(BaseModel):
    """docstring for TestModel"""
    def __init__(self, parent):
        super(TestModel, self).__init__(parent)

    def refresh(self):
        self.beginResetModel()

        self.rootItem = BaseItem(None, None)

        walkPath(TreeData, self.rootItem)

        self.endResetModel()


if __name__ == '__main__':

    import sys

    app = QtWidgets.QApplication(sys.argv)
    window = BaseWindow()

    # creat a new tree and assign our ovvrided model to it
    tree = BaseTreeView()
    model = TestModel(tree)
    tree.setModel(model)

    window.setCentralWidget(tree)
    window.resize(640, 512)
    window.show()
    sys.exit(app.exec_())

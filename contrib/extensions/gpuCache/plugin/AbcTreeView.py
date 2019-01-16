#
#
#

import maya.cmds as cmds
from mtoa.ui.qt.Qt import QtWidgets, QtCore

from mtoa.ui.qt import BaseTreeView, BaseModel, BaseDelegate, BaseItem, BaseWindow
from AbcTransverser import AlembicTransverser, ABC_PATH, \
                            ABC_NAME, ABC_PARENT, ABC_VISIBILITY, \
                            ABC_INSTANCEPATH, ABC_ENTIY_TYPE, ABC_IOBJECT
from alembic import Abc, AbcGeom


class AbcTreeView(BaseTreeView):
    """docstring for AbcTree"""
    def __init__(self, transverser, parent=None):
        super(AbcTreeView, self).__init__(parent)
        self.transverser = transverser

        model = AbcTreeModel(self, self.transverser)
        self.setModel(model)

        # Custom style
        delegate = AbcTreeViewDelegate(self)
        self.setItemDelegate(delegate)

        self.expanded.connect(self.onExpanded)

    def setCurrentNode(self, node):
        """Clear the widget and generate the view of the new node."""
        model = self.model()
        model.setCurrentNode(node)
        model.refresh()

    # TODO: put it to TreeModel
    def onExpanded(self, index):
        """It is called when the item specified by index is expanded."""
        if not index.isValid():
            return

        # Load the objects of the children items
        item = index.internalPointer()
        for i in range(item.childCount()):
            child = item.child(i)
            child.obtainChildren()


class AbcTreeModel(BaseModel):
    """docstring for TestModel"""
    def __init__(self, treeView, transverser, parent=None):

        self.transverser = transverser
        self.currentNode = None
        self.iarch = None
        self.abc_items = []

        # call the base class init and refresh the data
        super(AbcTreeModel, self).__init__(treeView, parent)

    def refresh(self):
        if not self.currentNode or not cmds.objExists(self.currentNode):
            return

        self.beginResetModel()

        self.rootItem = AbcItem(None, self.transverser, self.currentNode)
        self.rootItem.obtainChildren()

        self.endResetModel()

    def setCurrentNode(self, node):
        self.currentNode = node


class AbcTreeViewDelegate(BaseDelegate):

    def __init__(self, treeView):
        super(AbcTreeViewDelegate, self).__init__(treeView)


class AbcItem(BaseItem):

    def __init__(self, parentItem, transverser, node=None, iobject=None):
        self.childItems = []
        self.childrenObtained = False

        self.transverser = transverser

        self.origin = False
        self.node = node
        self.iarch = None

        self.iobject = iobject

        name = ""
        if self.iobject:
            name = iobject.getName()

        if not parentItem:
            self.origin = True

        super(AbcItem, self).__init__(parentItem, name)

    def getNode(self):
        """Recursively search the the origin parent TreeItem in parents."""
        if self.node:
            return self.node

        return self.parent().getNode()

    def obtainChildren(self):
        if self.childrenObtained:
            return
        self.iarch = self.transverser.getArchive(self.getNode())
        if not self.iobject:
            item = AbcItem(self, self.transverser, iobject=self.iarch.getTop())
            item.obtainChildren()
        else:
            children = self.transverser.dir(self.iobject)
            if children:
                for child in children:
                    AbcItem(self, self.transverser, iobject=child[ABC_IOBJECT])

        self.childrenObtained = True

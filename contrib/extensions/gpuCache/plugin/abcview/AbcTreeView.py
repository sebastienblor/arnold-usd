#
#
#

import maya.cmds as cmds
from mtoa.ui.qt.Qt import QtWidgets, QtCore, QtGui

from mtoa.ui.qt import BaseTreeView, BaseModel, BaseDelegate, BaseItem, BaseWindow
from AbcTransverser import AlembicTransverser, ABC_PATH, \
                           ABC_NAME, ABC_PARENT, ABC_VISIBILITY, \
                           ABC_INSTANCEPATH, ABC_ENTIY_TYPE, ABC_IOBJECT
from alembic import Abc, AbcGeom


class AbcTreeView(BaseTreeView):
    """docstring for AbcTree"""

    itemSelected = QtCore.Signal(str, object)

    def __init__(self, transverser, parent=None):
        super(AbcTreeView, self).__init__(parent)
        self.transverser = transverser
        self.currentItems = []

        model = AbcTreeModel(self, self.transverser)
        self.setModel(model)

        # Custom style
        delegate = AbcTreeViewDelegate(self)
        self.setItemDelegate(delegate)

        self.expanded.connect(self.onExpanded)

    def setCurrentNode(self, node):
        """Clear the widget and generate the view of the new node."""
        model = self.model()
        if model.setCurrentNode(node):
            model.refresh()
            self.expandToDepth(0)

    def onExpanded(self, index):
        """It is called when the item specified by index is expanded."""
        if not index.isValid():
            return

        # Load the objects of the children items
        item = index.internalPointer()
        for i in range(item.childCount()):
            child = item.child(i)
            child.obtainChildren()

    def selectionChanged(self, selected, deselected):
        """
        Called when the selection is changed. The previous selection (which
        may be empty), is specified by deselected, and the new selection by
        selected.
        """
        # This will redraw selected items in the tree view.
        super(AbcTreeView, self).selectionChanged(selected, deselected)
        indices = self.selectedIndexes()
        if indices:
            objects = []

            for i in indices:
                if not i.isValid():
                    continue

                item = i.internalPointer()

                subObject = item.data
                if subObject:
                    objects.append(subObject)

            self.itemSelected.emit(self.model().currentNode, objects)


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
        if self.currentNode != node:
            self.currentNode = node
            return True
        return False


class AbcTreeViewDelegate(BaseDelegate):

    def __init__(self, treeView):
        super(AbcTreeViewDelegate, self).__init__(treeView)


class AbcItem(BaseItem):

    ALEMBIC_ICON = QtGui.QPixmap(":/out_objectSet.png")
    GROUP_ICON = QtGui.QPixmap(":/out_transform.png")
    MESH_ICON = QtGui.QPixmap(":/out_mesh.png")
    CURVES_ICON = QtGui.QPixmap(":/out_curves.png")

    def __init__(self, parentItem, transverser, node=None, data=None):
        self.childItems = []
        self.childrenObtained = False

        self.transverser = transverser

        self.origin = False
        self.node = node
        self.iarch = None

        self.data = data

        name = ""
        if self.data:
            name = data[ABC_NAME]
            if data[ABC_PATH] == '/':
                name = '/'

        if not parentItem:
            self.origin = True

        super(AbcItem, self).__init__(parentItem, name)

    def getNode(self):
        """Recursively search the the origin parent TreeItem in parents."""
        if self.node:
            return self.node

        return self.parent().getNode()

    # def getIcon(self):
    #     if self.childItems:
    #         return self.GROUP_ICON
    #     return self.MESH_ICON

    def obtainChildren(self):
        if self.childrenObtained:
            return
        self.iarch = self.transverser.getArchive(self.getNode())
        if not self.data:
            item = AbcItem(self, self.transverser, self.node, data=self.transverser.getObjectInfo(self.iarch.getTop()))
            item.obtainChildren()
        else:
            children = self.transverser.dir(self.data[ABC_IOBJECT])
            if children:
                for child in children:
                    AbcItem(self, self.transverser, self.node, data=child)

        self.childrenObtained = True

    def getOverrides(self):
        overrides = []
        op_node = self.transverser.getOperator(self.getNode(), self.data)
        return overrides

    def addOverride(self, param, value):
        pass


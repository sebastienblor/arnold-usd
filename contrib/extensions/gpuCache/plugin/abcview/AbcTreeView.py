#
#
#

import maya.cmds as cmds
from mtoa.ui.qt.Qt import QtWidgets, QtCore, QtGui

from mtoa.ui.qt import BaseTreeView, BaseModel, BaseDelegate, \
                       BaseItem, BaseWindow, dpiScale
from AbcTransverser import AlembicTransverser, ABC_PATH, \
                           ABC_NAME, ABC_PARENT, ABC_VISIBILITY, \
                           ABC_INSTANCEPATH, ABC_ENTIY_TYPE, ABC_IOBJECT, \
                           OVERRIDE_OP, DISABLE_OP
from alembic import Abc, AbcGeom

SHADER = "shader"
DISPLACEMENT = "disp_map"
PARAMETER = "other"


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

                if item:
                    objects.append(item)

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

        self.rootItem = AbcItem(None, self.transverser, self.currentNode, self)
        self.rootItem.obtainChildren()

        self.endResetModel()

    def setCurrentNode(self, node):
        if self.currentNode != node:
            self.currentNode = node
            return True
        return False

    def setData(self, index, value, role=QtCore.Qt.EditRole):
        """Set the role data for the item at index to value."""
        if not index.isValid():
            return False

        if role == QtCore.Qt.EditRole and value:
            item = index.internalPointer()

            # TODO: Don't assign directly
            item.name = value
            self.dataChanged.emit(index, index)

            return True

        elif role == OVERRIDE:
            item = index.internalPointer()
            # item.setOverride(
            #     value, self.treeView().getLayer(), OVERRIDE_ATTRIBUTE)
            self.dataChanged.emit(index, QtCore.QModelIndex())
            return True

        return False

    def executeAction(self, action, index):
        """User pressed by one of the actions."""
        item = index.internalPointer()

        if action == AbcItem.ACTION_SHADER:
            self.transverser.action(item.getOverride(SHADER))
        elif action == AbcItem.ACTION_EXPAND:
            self.treeView().setExpanded(
                index, not self.treeView().isExpanded(index))


class AbcTreeViewDelegate(BaseDelegate):

    def __init__(self, treeView):
        super(AbcTreeViewDelegate, self).__init__(treeView)


class AbcItem(BaseItem):

    ALEMBIC_ICON = QtGui.QPixmap(":/out_objectSet.png")
    GROUP_ICON = QtGui.QPixmap(":/out_transform.png")
    MESH_ICON = QtGui.QPixmap(":/out_mesh.png")
    CURVES_ICON = QtGui.QPixmap(":/out_curves.png")

    COLOR_OBJECT = QtGui.QColor(113, 142, 164)
    COLOR_OPERATOR = QtGui.QColor(18, 54, 82)
    COLOR_UNDIFINE = QtGui.QColor(0, 0, 0)

    (ACTION_EXPAND,  # Always first
     ACTION_NONE,
     ACTION_OPERATOR,
     ACTION_SHADER,
     ACTION_DISPACEMENT,
     ACTION_DISABLE) = range(6)

    (UNKNWON_TYPE,
     OBJECT_TYPE,
     OPERATOR_TYPE,
     ASSIGNMENT_TYPE) = range(4)

    def __init__(self, parentItem, transverser,
                 node=None, model=None, data=None, operator=None, index=-1):

        self.ATTRIBUTE_ICON = ":/out_list.png"
        self.SHADER_ICON = ":/out_blinn.png"
        self.DISPMAP_ICON = ":/out_displacementShader.png"

        self.childItems = []
        self.childrenObtained = False

        self.transverser = transverser

        self.origin = False
        self.node = node
        self.iarch = None

        self.data = data
        self.operator = operator

        self.overrides_op = None
        self.disable_op = None

        name = ""
        if self.data:
            name = data[ABC_NAME]
            if data[ABC_PATH] == '/':
                name = '/'
            self.itemType = self.OBJECT_TYPE

        elif self.operator:
            name = self.operator
            self.itemType = self.OPERATOR_TYPE
            if cmds.nodeType(self.operator) == OVERRIDE_OP:
                self.overrides_op = self.operator
        else:
            name = "unknown"
            self.itemType = self.UNKNWON_TYPE

        if not parentItem:
            self.origin = True

        super(AbcItem, self).__init__(parentItem, name, model, index)

    def getNode(self):
        """Recursively search the the origin parent TreeItem in parents."""
        if self.node:
            return self.node

        return self.parent().getNode()

    def getModel(self):
        """Recursively search the the origin parent TreeItem in parents."""
        if self.model:
            return self.model

        return self.parent().getModel()

    def getOverridesOp(self, transverse=False):
        if self.overrides_op:
            return self.overrides_op

        if self.parent() and transverse:
            return self.parent().getOverridesOp(True)
        return None

    def getIcon(self):
        if self.childItems:
            return self.GROUP_ICON
        return self.MESH_ICON

    def getBackgroundColor(self):
        """
        The background color of current node. It can be different depending on
        the item type.
        """
        if self.itemType == self.OBJECT_TYPE:
            return QtGui.QColor(71, 71, 71)
        elif self.itemType == self.OPERATOR_TYPE:
            return QtGui.QColor(71, 80, 71)
        else:
            return QtGui.QColor(71, 71, 71)

    def getLabelColor(self):
        """
        Return the color of the  of the current item. It can be different
        depending on the item type.
        """
        if self.itemType == self.OBJECT_TYPE:
            return self.COLOR_OBJECT
        elif self.itemType == self.OPERATOR_TYPE:
            return self.COLOR_OPERATOR
        return self.COLOR_UNDEFINED

    def getActions(self):
        actions = []

        my_overrides = self.getOverrides()
        params = [p[0] for p in my_overrides]
        attr_params = [x for x in params if x not in [SHADER, DISPLACEMENT]]

        parent_overrides = self.getOverrides(True)
        parent_params = [p[0] for p in parent_overrides]
        parent_attr_params = [x for x in parent_params if x not in [SHADER, DISPLACEMENT]]

        OVERRIDES = {SHADER: False,
                     DISPLACEMENT: False,
                     PARAMETER: False}

        if len(my_overrides):
            OVERRIDES[SHADER] = SHADER in params
            OVERRIDES[DISPLACEMENT] = DISPLACEMENT in params
            OVERRIDES[PARAMETER] = len(attr_params) > 0
        if len(parent_overrides):
            OVERRIDES[SHADER] = SHADER in parent_params
            OVERRIDES[DISPLACEMENT] = DISPLACEMENT in parent_params
            OVERRIDES[PARAMETER] = len(parent_attr_params) > 0

        for override, enabled in OVERRIDES.items():
            icon = None
            action = None

            if override in params or (override == PARAMETER and len(attr_params) > 0):
                opacity = 1.0
            elif override in parent_params or (override == PARAMETER and len(parent_attr_params) > 0):
                opacity = 0.25
            else:
                opacity = None

            if opacity:

                if enabled and override == SHADER:
                    action = self.ACTION_SHADER
                    icon = self.SHADER_ICON
                elif enabled and override == DISPLACEMENT:
                    action = self.ACTION_DISPACEMENT
                    icon = self.DISPMAP_ICON
                elif enabled and override == PARAMETER:
                    action = self.ACTION_OPERATOR
                    icon = self.ATTRIBUTE_ICON

                if icon:
                    icon = BaseItem.dpiScaledIcon(icon)

            actions.append((icon, opacity, action))

        return actions

    def getIndent(self):
        """The text indent. offset for the icon"""
        return dpiScale(40)

    def getOverrides(self, tranverse=False, override_type=None):
        if not tranverse:
            if self.data:
                return self.transverser.getOverrides(self.node, self.data[ABC_PATH], override_type)
            return []
        else:
            overrides = []
            currentOverrides = self.getOverrides(False, override_type)
            if currentOverrides:
                overrides += currentOverrides

            parent = self.parent()
            if parent:
                overrides += parent.getOverrides(True, override_type)

            return overrides

    def obtainChildren(self):
        if self.childrenObtained:
            return
        self.iarch = self.transverser.getArchive(self.getNode())
        # root node
        print "AbcItem.obtainChildren", self.data, self.itemType
        if not self.data and not self.parent():
            item = AbcItem(self, self.transverser, self.node, data=self.transverser.getObjectInfo(self.iarch.getTop()))
            item.obtainChildren()
        elif self.itemType == self.OBJECT_TYPE:
            # get operators with this path
            operators = self.transverser.getOperators(self.node, self.data[ABC_PATH])
            if operators:
                print operators
                for op in operators:
                    AbcItem(self, self.transverser, self.node, operator=op)
            children = self.transverser.dir(self.data[ABC_IOBJECT])
            if children:
                for child in children:
                    AbcItem(self, self.transverser, self.node, data=child)

        self.childrenObtained = True

    def addOverrideOp(self, operator):
        self.overrides_op = operator
        op_item = AbcItem(self, self.transverser, self.node, operator=operator, index=0)
        # model = self.getModel()
        # if model:
        #     index = model.indexFromItem(self)
        #     model.executeAction(AbcItem.ACTION_EXPAND, index)
        # model = self.getModel()
        # index = model.indexFromItem(self)
        # model.setData(index, operator, OVERRIDE)

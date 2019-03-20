#
#
#

import maya.cmds as cmds
from mtoa.ui.qt.Qt import QtWidgets, QtCore, QtGui

from mtoa.ui.qt import BaseTreeView, BaseModel, BaseDelegate, \
                       BaseItem, BaseWindow, dpiScale
from mtoa.ui.procview.ProceduralTransverser import PROC_PATH, \
                           PROC_NAME, PROC_PARENT, PROC_VISIBILITY, \
                           PROC_INSTANCEPATH, PROC_ENTRY_TYPE, PROC_IOBJECT, \
                           OVERRIDE_OP, DISABLE_OP

SHADER = "shader"
DISPLACEMENT = "disp_map"
PARAMETER = "other"


class ProceduralTreeView(BaseTreeView):
    """docstring for ProceduralTree"""

    itemSelected = QtCore.Signal(str, object)

    def __init__(self, transverser, parent=None):
        super(ProceduralTreeView, self).__init__(parent)
        self.transverser = transverser

        model = ProceduralTreeModel(self, self.transverser)
        self.setModel(model)

        # Custom style
        delegate = ProceduralTreeViewDelegate(self)
        self.setItemDelegate(delegate)

        self.pressed.connect(self.onPressed)
        self.expanded.connect(self.onExpanded)

    def setTransverser(self, transverser, refresh=True):
        self.transverser = transverser
        self.model().setTransverser(transverser, refresh)

    def setCurrentNode(self, node, expand=True, force=False):
        """Clear the widget and generate the view of the new node."""
        model = self.model()
        if model.setCurrentNode(node, force):
            # refresh the model, pass if we need to delay the update
            # of the child node to the actual expand
            model.refresh(not expand)
            if expand:
                self.expandToDepth(0)
            self.clearSelection()

    def onPressed(self, index):
        if not index.isValid():
            return
        item = index.internalPointer()
        # refresh the children of this item if needed
        item.obtainChildren()

    def onExpanded(self, index):
        """It is called when the item specified by index is expanded."""
        if not index.isValid():
            return

        item = index.internalPointer()
        # Load the objects of the children items
        for i in range(item.childCount()):
            child = item.child(i)
            child.obtainChildren()

    def select(self, path):
        root = self.model().rootItem
        item = root.find(path)
        if not item:
            return

        if self.transverser:
            if path == self.transverser.selectionStr:
                return # nothing changed, we can leave
            self.transverser.selectionStr = path

        parent = item.parent()
        while parent:
            index = self.model().indexFromItem(parent)
            self.onExpanded(index)
            parent = parent.parent()

        self.clearSelection()

        # We are ready to manipulate with the selection.
        # Get the index of the item.
        index = self.model().indexFromItem(item)
        # Select it.
        self.selectionModel().setCurrentIndex(
            index, QtCore.QItemSelectionModel.Select)

    def selectionChanged(self, selected, deselected):
        """
        Called when the selection is changed. The previous selection (which
        may be empty), is specified by deselected, and the new selection by
        selected.
        """
        # This will redraw selected items in the tree view.
        super(ProceduralTreeView, self).selectionChanged(selected, deselected)
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


class ProceduralTreeModel(BaseModel):
    """docstring for TestModel"""

    def __init__(self, treeView, transverser, parent=None):

        self.transverser = transverser
        self.currentNode = None
        self.iarch = None

        # call the base class init and refresh the data
        super(ProceduralTreeModel, self).__init__(treeView, parent)

    def setTransverser(self, transverser, refresh=True):
        self.transverser = transverser
        self.iarch = None
        if refresh:
            self.refresh()

    def refresh(self, delayUpdate=False):
        if not self.currentNode or not cmds.objExists(self.currentNode) or not self.transverser:
            return

        self.beginResetModel()
        self.rootItem = ProceduralItem(None, self.transverser, self.currentNode, self)
        self.rootItem.obtainChildren(delayUpdate)

        self.endResetModel()

    def setCurrentNode(self, node, force=False):
        if self.currentNode != node or force:
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

        if action == ProceduralItem.ACTION_SHADER:
            ov = item.getOverrides() or []
            if len(ov) == 0:
                return
            ov = ov[0]
            if ov and len(ov) >= 3 and ov[0] == 'shader':
                ov = str(ov[2]).replace("'", "")
                self.transverser.selectNode(ov)
        elif action == ProceduralItem.ACTION_EXPAND:
            self.treeView().setExpanded(
                index, not self.treeView().isExpanded(index))


class ProceduralTreeViewDelegate(BaseDelegate):

    def __init__(self, treeView):
        super(ProceduralTreeViewDelegate, self).__init__(treeView)


class ProceduralItem(BaseItem):

    ALEMBIC_ICON = QtGui.QPixmap(":/out_objectSet.png")
    GROUP_ICON = QtGui.QPixmap(":/out_transform.png")
    MESH_ICON = QtGui.QPixmap(":/out_mesh.png")
    POINTS_ICON = QtGui.QPixmap(":/out_particle.png")
    CURVES_ICON = QtGui.QPixmap(":/nurbsCurve.svg")
    INSTANCE_ICON = QtGui.QPixmap(":/out_instancer.png")
    UNKNOWN_ICON = QtGui.QPixmap(":/question.png")

    COLOR_OBJECT = QtGui.QColor(113, 142, 164)
    COLOR_OPERATOR = QtGui.QColor(18, 54, 82)
    COLOR_UNDEFINED = QtGui.QColor(0, 0, 0)

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

        self.childrenObtained = False

        self.transverser = transverser

        self.origin = False
        self.node = node
        self.iarch = None

        self.model = model

        self.data = data
        self.operator = operator

        self.overrides_op = None
        self.disable_op = None

        name = ""
        if self.data:
            name = data[PROC_NAME]
            if data[PROC_PATH] == '/':
                name = '/'
            self.itemType = self.OBJECT_TYPE
            self.setOverridesOp()

        # this item is an operator, TODO make it's own Item class
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

        super(ProceduralItem, self).__init__(parentItem, name, index)

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

    def setOverridesOp(self):
        ops = []
        if self.data:
            collections = self.transverser.getCollections(self.node, self.data[PROC_PATH], True)
            ops = self.transverser.getOperators(self.node, self.data[PROC_PATH], OVERRIDE_OP, True, collections)
        if len(ops):
            self.overrides_op = ops
        else:
            self.overrides_op = None
        return self.overrides_op

    def getOverridesOp(self, transverse=False):
        self.setOverridesOp()
        if self.overrides_op:
            return self.overrides_op

        if self.parent() and transverse:
            return self.parent().getOverridesOp(True)
        return None

    def getIcon(self):
        if self.data[PROC_ENTRY_TYPE] == 'polymesh':
            return self.MESH_ICON
        elif self.data[PROC_ENTRY_TYPE] == 'points':
            return self.POINTS_ICON
        elif self.data[PROC_ENTRY_TYPE] == 'curves':
            return self.CURVES_ICON
        elif self.data[PROC_ENTRY_TYPE] == 'ginstance':
            return self.INSTANCE_ICON
        elif self.childItems:
            return self.GROUP_ICON
        return self.UNKNOWN_ICON

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

            actions.append((icon, opacity, action, False))

        return actions

    def getIndent(self):
        """The text indent. offset for the icon"""
        return dpiScale(40)

    def getOverrides(self, tranverse=False):
        if not tranverse:
            # get the overrides just for this node
            if self.data:
                return self.transverser.getOverrides(self.getNode(), self.data[PROC_PATH], exact_match=True)
            return []
        else:
            overrides = []
            currentOverrides = self.getOverrides(False)
            if currentOverrides:
                overrides += currentOverrides

            # get the overides for parent nodes as well
            parent = self.parent()
            if parent:
                overrides += self.transverser.getOverrides(parent.node, self.data[PROC_PATH])

            return overrides

    def obtainChildren(self, delayUpdate=False):
        if self.childrenObtained:
            return

        # delete any nodes under this one
        for ch in range(self.childCount()):
            self.removeChild(self.child(ch))

        if not self.data and not self.parent():
            rootData = self.transverser.getRootObjectInfo(self.node)
            if rootData:
                item = ProceduralItem(self, self.transverser, self.node, data=rootData)
                item.obtainChildren(delayUpdate)
        elif delayUpdate:
            # delay the update by creating a tempory node that will be deleted on the expand
            ProceduralItem(self, self.transverser, self.node, data=['foo', 'foo', 'foo', 'visible', 'foo', 'foo', "NULL"])
            return
        elif self.itemType == self.OBJECT_TYPE:
            # get operators with this path

            # For now we don't show the operators in the hierarchy, we need to make it an option
            if self.transverser.showOperatorItems():
                collections = self.transverser.getCollections(self.getNode(), self.data[PROC_PATH])
                operators = self.transverser.getOperators(self.getNode(), self.data[PROC_PATH], collections=collections)
                if operators:
                    for op in operators:
                        ProceduralItem(self, self.transverser, operator=op)

            children = self.transverser.dir(self.data[PROC_IOBJECT])
            if children:
                for child in children:
                    ProceduralItem(self, self.transverser, self.node, data=child)

        self.childrenObtained = True

    def find(self, path):
        self.obtainChildren()
        for child in self.childItems:
            if child.data and path == child.data[PROC_PATH]:
                return child
            if child.data:
                isRoot = child.data[PROC_PATH] == '/' or child.data[PROC_PATH] == ''
                childPath = child.data[PROC_PATH] + ('' if isRoot else '/')
                if isRoot or path == childPath or path.startswith(childPath):
                    return child.find(path)

    def setOverrideOp(self, operator):
        self.overrides_op = operator
        # FIXME for now we're not showing the operator in the hierarchy, we need to make it an option
        if self.transverser.showOperatorItems():
            op_item = ProceduralItem(self, self.transverser, self.node, operator=operator, index=0)

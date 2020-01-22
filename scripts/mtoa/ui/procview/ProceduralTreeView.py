#
#
#

import os
import weakref
import maya.cmds as cmds
from mtoa.ui.qt.Qt import QtWidgets, QtCore, QtGui

from mtoa.ui.qt import BaseTreeView, BaseModel, BaseDelegate, \
                       BaseItem, BaseWindow, dpiScale, Timer
from mtoa.ui.procview.ProceduralTransverser import PROC_PATH, \
                           PROC_NAME, PROC_PARENT, PROC_VISIBILITY, \
                           PROC_INSTANCEPATH, PROC_ENTRY, PROC_ENTRY_TYPE, \
                           PROC_IOBJECT, PROC_NUM_CHILDREN, \
                           OVERRIDE_OP, DISABLE_OP, \
                           PARAM

from mtoa.ui.qt.utils import busy_cursor

import traceback

SHADER = "shader"
DISPLACEMENT = "disp_map"
PARAMETER = "other"


class ProceduralTreeView(BaseTreeView):
    """docstring for ProceduralTree"""

    itemSelected = QtCore.Signal(str, object)

    MIN_VISIBLE_ENTRIES = 4
    MAX_VISIBLE_ENTRIES = 10

    def __init__(self, transverser, parent=None):
        super(ProceduralTreeView, self).__init__(parent)
        self.transverser = transverser

        self.baseModel = ProceduralTreeModel(self, self.transverser)
        self.filterModel = ProceduralTreeFilterModel(self)
        self.filterModel.setSourceModel(self.baseModel)
        self.setModel(self.filterModel)

        # Custom style
        delegate = ProceduralTreeViewDelegate(self)
        self.setItemDelegate(delegate)

        self.expanded.connect(self.onExpanded)
        self.collapsed.connect(self.onCollapse)

        self.INITIAL_HEIGHT = self.sizeHintForRow(0) * self.MIN_VISIBLE_ENTRIES + 2 * self.frameWidth()
        self.MAX_HEIGHT = self.sizeHintForRow(0) * self.MAX_VISIBLE_ENTRIES + 2 * self.frameWidth()

    def setTransverser(self, transverser, refresh=True):
        self.transverser = transverser
        self.model().setTransverser(transverser, refresh)

    def setCurrentNode(self, node, expand=True, force=False):
        """Clear the widget and generate the view of the new node."""
        model = self.model()
        if model.setCurrentNode(node, force):
            # refresh the model, pass if we need to delay the update
            # of the child node to the actual expand
            model.refresh()
            if expand:
                self.expandToDepth(0)
            self.clearSelection()

    def _calculateHeight(self):

        model = self.model()
        contentSizeHeight = self.sizeHintForRow(0) * model.expandedCount() + 2 * self.frameWidth()
        contentSizeHeight = self.INITIAL_HEIGHT if contentSizeHeight < self.INITIAL_HEIGHT else self.MAX_HEIGHT if contentSizeHeight > self.MAX_HEIGHT else contentSizeHeight
        return contentSizeHeight

    def onExpanded(self, index):
        """It is called when the item specified by index is expanded."""
        # scale the treeView based on number of expanded rows
        self.setFixedHeight(self._calculateHeight())

    def onCollapse(self, index):
        # scale the treeView based on number of expanded rows
        self.setFixedHeight(self._calculateHeight())

    def select(self, path):
        root = self.model().rootItem()
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
            # self.onExpanded(index)
            parent = parent.parent()

        self.clearSelection()

        # We are ready to manipulate with the selection.
        # Get the index of the item.
        index = self.model().indexFromItem(item)
        proxyIndex = self.model().mapFromSource(index)
        # Select it.
        self.selectionModel().setCurrentIndex(
            proxyIndex, QtCore.QItemSelectionModel.Select)

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
                modIndex = self.model().mapToSource(i)
                if not modIndex.isValid():
                    continue

                item = modIndex.internalPointer()

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
        self.rootItem = ProceduralItem(None, self.transverser, node=self.currentNode, model=self)
        self.rootItem.obtainChildren(delayUpdate)

        self.endResetModel()

    def canFetchMore(self, parent):
        if not parent.isValid():
            return False
        item = parent.internalPointer()
        return item.canFetchMore()

    # @busy_cursor
    def fetchMore(self, parent):
        if not parent.isValid():
            return False
        item = parent.internalPointer()
        if item:
            s = parent.row() + 1
            e = parent.row() + item.childCount()
            self.beginInsertRows(parent, s, e)
            # self.beginInsertRows(parent, s, s + item.childCount())
            # Load the objects of the children items
            for i in range(item.childCount()):
                child = item.child(i)
                child.obtainChildren()
            self.endInsertRows()
            return True
        return False

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
            self.dataChanged.emit(index, QtCore.QModelIndex())
            return True

        return False

    def expandedCount(self):
        def count_expanded(item):
            c = 0
            index = self.indexFromItem(item)
            is_expanded = self.treeView().isExpanded(index)
            if is_expanded or item.itemType == item.UNKNWON_TYPE:
                c += item.childCount()
                for i in item.childItems:
                    c += count_expanded(i)
            return c

        c = 1
        if self.rootItem:
            return c + count_expanded(self.rootItem)
        return c

    def executeAction(self, action, index):
        """User pressed by one of the actions."""
        item = index.internalPointer()
        treeView = self.treeView()

        if action == ProceduralItem.ACTION_SHADER:
            ov, pov = item.getOverrides() or [], []
            if len(ov) == 0:
                return
            ov = ov[0]
            if ov and len(ov) >= 3 and ov[0] == 'shader':
                ov = str(ov[2]).replace("'", "")
                self.transverser.selectNode(ov)
        elif action == ProceduralItem.ACTION_EXPAND:
            treeView.setExpanded(
                index, not treeView.isExpanded(index))


class ProceduralTreeViewDelegate(BaseDelegate):

    def __init__(self, treeView):
        super(ProceduralTreeViewDelegate, self).__init__(treeView)


class ProceduralItem(BaseItem):
    # __metaclass__ = LogMethodCalls

    ALEMBIC_ICON = QtGui.QPixmap(os.path.join(cmds.getModulePath(moduleName='mtoa'), 'icons', "alembic.png"))
    GROUP_ICON = QtGui.QPixmap(":/out_transform.png")
    MESH_ICON = QtGui.QPixmap(":/out_mesh.png")
    POINTS_ICON = QtGui.QPixmap(":/out_particle.png")
    CURVES_ICON = QtGui.QPixmap(":/nurbsCurve.svg")
    INSTANCE_ICON = QtGui.QPixmap(":/out_instancer.png")
    UNKNOWN_ICON = QtGui.QPixmap(":/question.png")
    MATERIAL_ICON = QtGui.QPixmap(":/out_blinn.png")
    LIGHT_ICON = QtGui.QPixmap(":/out_ambientLight.png")
    CAMERA_ICON = QtGui.QPixmap(":/out_camera.png")
    VOLUME_ICON = QtGui.QPixmap(":/VolumeShelf.png")
    PROCEDURAL_ICON = QtGui.QPixmap(":/openScript.png") # or StandinShelf.png ?
    OPERATOR_ICON = QtGui.QPixmap(":/gear.png")
    UNKNOWN_SHAPE_ICON = QtGui.QPixmap(":/cube.png")
    TEXTURE_ICON = QtGui.QPixmap(":/menuIconImages")

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
                 node=None, model=None, data=[], operator=None, index=-1):

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

        self.actions = []
        self.dirty = True

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

    def __repr__(self):
        return "<{} {}>".format(self.__class__.__name__, self.getName())

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

    def canFetchMore(self):
        return self.numChildren() >= 1 and not self.childrenObtained

    def numChildren(self):
        if PROC_NUM_CHILDREN > len(self.data) - 1:
            return 0
        return self.data[PROC_NUM_CHILDREN]

    def setOverridesOp(self):
        ops = []
        if self.data:
            # collections = self.transverser.getCollections(self.node, self.data[PROC_PATH], True)
            ops = self.transverser.getOperators(self.node, self.data[PROC_PATH], OVERRIDE_OP, [])
        if len(ops):
            self.overrides_op = [o for o, m in ops if m]
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
        nodeEntry = self.data[PROC_ENTRY]
        nodeEntryType = self.data[PROC_ENTRY_TYPE]

        if nodeEntry == 'polymesh':
            return self.MESH_ICON
        elif nodeEntry == 'points':
            return self.POINTS_ICON
        elif nodeEntry == 'curves':
            return self.CURVES_ICON
        elif nodeEntry == 'ginstance':
            return self.INSTANCE_ICON
        elif nodeEntry == 'xform':
            return self.GROUP_ICON
        elif nodeEntry == 'alembic':
            return self.ALEMBIC_ICON
        elif nodeEntryType == 'procedural': # we hack the node entry type for procedurals
            return self.PROCEDURAL_ICON
        elif nodeEntryType == 'volume': # we hack the node entry type for volumes
            return self.VOLUME_ICON
        elif nodeEntryType == 'shape':
            return self.UNKNOWN_SHAPE_ICON
        elif nodeEntry == 'image':
            return self.TEXTURE_ICON
        elif nodeEntryType == 'shader':
            return self.MATERIAL_ICON
        elif nodeEntryType == 'light':
            return self.LIGHT_ICON
        elif nodeEntryType == 'camera':
            return self.CAMERA_ICON
        elif nodeEntryType == 'operator':
            return self.OPERATOR_ICON
        return None

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
        if self.dirty:
            self.actions = []
            my_overrides, parent_overrides = self.getOverrides()
            params = [p[PARAM] for p in my_overrides]
            attr_params = [x for x in params if x not in [SHADER, DISPLACEMENT]]

            parent_params = [p[PARAM] for p in parent_overrides]
            parent_attr_params = [x for x in parent_params if x not in [SHADER, DISPLACEMENT]]

            OVERRIDES = {SHADER: False,
                         DISPLACEMENT: False,
                         PARAMETER: False}

            OVERRIDES[SHADER] = SHADER in params+parent_params
            OVERRIDES[DISPLACEMENT] = DISPLACEMENT in params+parent_params
            OVERRIDES[PARAMETER] = len(attr_params) > 0 or len(parent_attr_params) > 0

            inherited_params = list(set(parent_params) - set(params))
            inherited_attr_params = [x for x in inherited_params if x not in [SHADER, DISPLACEMENT]]

            for override, enabled in OVERRIDES.items():
                icon = None
                action = None
                overlay = None

                is_inherited = override in inherited_params or (override == PARAMETER and len(inherited_attr_params) > 0)

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

                # for now we arn't indicating with an additional icon
                # if is_inherited:
                #     inherited_icon = os.path.join(cmds.getModulePath(moduleName='mtoa'), 'icons', "inherit_100.png")
                #     overlay = BaseItem.dpiScaledIcon(inherited_icon)

                self.actions.append((icon, opacity, action, False, overlay))
                self.dirty = False

        return self.actions

    def getIndent(self):
        """The text indent. offset for the icon"""
        return dpiScale(40)

    def getOverrides(self, tranverse=False):
        return self.transverser.getOverrides(self.getNode(), self.data[PROC_PATH])

    def obtainChildren(self, delayUpdate=False):
        if self.childrenObtained:
            return

        if not self.data and not self.parent():
            rootData = self.transverser.getRootObjectInfo(self.node)
            if rootData:
                item = ProceduralItem(self, self.transverser, self.node, data=rootData)
                item.obtainChildren(delayUpdate)
        elif delayUpdate:
            # delay the update by creating a tempory node that will be deleted on the expand
            ProceduralItem(self, self.transverser, self.node, data=['foo', 'foo', 'foo', 'visible', 'foo', 'foo', "NULL", 0])
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
                    childItem = ProceduralItem(self, self.transverser, self.node, data=child)

            self.childrenObtained = True

        return self.childrenObtained

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

    def setDirty(self, dirty):
        self.dirty = dirty


class ProceduralTreeFilterModel(QtCore.QSortFilterProxyModel):
    """docstring for ClassName"""
    def __init__(self, treeView, parent=None):
        super(ProceduralTreeFilterModel, self).__init__(parent)

        self.treeView = weakref.ref(treeView)
        # self.setRecursiveFilteringEnabled(True) # qt 5.10 +

    @property
    def currentNode(self):
        return self.sourceModel().currentNode

    def filterAcceptsRow(self, sourceRow, sourceParent):
        if self.filterAcceptsRowItself(sourceRow, sourceParent):
            return True
        parent = sourceParent
        while parent.isValid():
            if self.filterAcceptsRowItself(parent.row(), parent.parent()):
                return True
            parent = parent.parent()

        if self.hasAcceptedChildren(sourceRow, sourceParent):
            return True

        return False

    def filterAcceptsRowItself(self, sourceRow, sourceParent):
        index = self.sourceModel().index(sourceRow, 0, sourceParent)
        item = index.internalPointer()
        if item and not item.childrenObtained:
            item.obtainChildren()
        accepted = super(ProceduralTreeFilterModel, self).filterAcceptsRow(sourceRow, sourceParent)
        # TODO get if this should be expanded
        return accepted

    def hasAcceptedChildren(self, sourceRow, sourceParent):
        index = self.sourceModel().index(sourceRow, 0, sourceParent)
        if not index.isValid():
            return False

        item = index.internalPointer()
        if not item.childrenObtained:
            item.obtainChildren()

        childCount = item.numChildren()
        if childCount == 0:
            return False

        for nChild in range(childCount):
            if self.filterAcceptsRowItself(nChild, index):
                return True
            if self.hasAcceptedChildren(nChild, index):
                return True

        return False

    def setTransverser(self, *args, **kwargs):
        return self.sourceModel().setTransverser(*args, **kwargs)

    def indexFromItem(self, *args, **kwargs):
        return self.sourceModel().indexFromItem(*args, **kwargs)

    def refresh(self, *args, **kwargs):
        return self.sourceModel().refresh(*args, **kwargs)

    def rootItem(self, *args, **kwargs):
        return self.sourceModel().rootItem

    def canFetchMore(self, parent):
        if not parent.isValid():
            return True
        sourceIndex = self.mapToSource(parent)
        return self.sourceModel().canFetchMore(sourceIndex)

    def fetchMore(self, parent):
        if not parent.isValid():
            return False
        else:
            row = parent.row()
            startRow = row + 1
            sourceIndex = self.mapToSource(parent)
            item = sourceIndex.internalPointer()
            self.beginInsertRows(parent, startRow, startRow + item.numChildren())
            item.obtainChildren()
            self.endInsertRows()
            return True
        return False

    def setCurrentNode(self, *args, **kwargs):
        return self.sourceModel().setCurrentNode(*args, **kwargs)

    def setData(self, *args, **kwargs):
        return self.sourceModel().setData(*args, **kwargs)

    def expandedCount(self):
        def count_expanded(item):
            c = 0
            mod_index = self.indexFromItem(item)
            index = self.mapFromSource(mod_index)
            is_expanded = self.treeView().isExpanded(index)
            if is_expanded or item.itemType == item.UNKNWON_TYPE:
                c += item.childCount()
                for i in item.childItems:
                    c += count_expanded(i)
            return c

        c = 1
        if self.rootItem():
            return c + count_expanded(self.rootItem())
        return c

    def executeAction(self, action, index):
        modelIndex = self.mapToSource(index)
        return self.sourceModel().executeAction(action, modelIndex)

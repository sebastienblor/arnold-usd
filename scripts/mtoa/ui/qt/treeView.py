from . import toPyObject
from .Qt import OpenMayaUI
from .Qt import QtCore
from .Qt import QtGui
from .Qt import QtWidgets
from .itemStyle import ItemStyle
from .utils import dpiScale, dpiScaledIcon
from .color import Color
from .style import MtoAStyle
import weakref
from copy import deepcopy


# Global look variables.
ITEM_HEIGHT = dpiScale(24)
ITEM_INDENT = dpiScale(8)
EXPAND_SENSITIVITY = dpiScale(4)

NODE_BAR_COLOUR = QtCore.Qt.UserRole + 1
ACTIONS = QtCore.Qt.UserRole + 2
ICON = QtCore.Qt.UserRole + 3
TEXT_INDENT = QtCore.Qt.UserRole + 4
NODE_ENABLED = QtCore.Qt.UserRole + 5

CHILD_COUNT = QtCore.Qt.UserRole + 64


class BaseTreeView(QtWidgets.QTreeView):
    """The main view, used to show the heirarchy."""

    def __init__(self, parent=None, style=None):
        """Called after the instance has been created."""
        super(BaseTreeView, self).__init__(parent)

        # FIXME: currently completely overriding maya style
        if not style:
            style = MtoAStyle.currentStyle()
        style.apply(self)

        self.setObjectName("BaseTreeView")
        # Set the custom tree model
        model = BaseModel(self)
        self.setModel(model)

        # Hide the header
        self.header().hide()

        # Custom style
        delegate = BaseDelegate(self)
        self.setItemDelegate(delegate)
        self.setIndentation(ITEM_INDENT)

        self.setRootIsDecorated(False)

        self.setAnimated(True)

        self.setExpandsOnDoubleClick(False)

        # for mouseMoveEvent to redraw on hover
        self.setMouseTracking(True)

        # We need this custom flag because if setDropIndicatorShown is set to
        # false, dropIndicatorPosition returns wrong data. We use this flag to
        # skip drawing of the indicator.
        self.customIndicatorShown = False

        # do connections
        self.clicked.connect(self.executeAction)

    def mousePressEvent(self, event):
        """Receive mouse press events for the widget."""

        index = self.indexAt(event.pos())

        super(BaseTreeView, self).mousePressEvent(event)

        # Redraw the item
        self.redraw(index)

    def mouseReleaseEvent(self, event):
        """Trigger actions based on mouse presses."""
        super(BaseTreeView, self).mouseReleaseEvent(event)
        index = self.indexAt(event.pos())

        if not index.isValid():
            return

        # Redraw the item
        self.redraw(index)

    def mouseMoveEvent(self, event):

        super(BaseTreeView, self).mouseMoveEvent(event)

        index = self.indexAt(event.pos())

        if not index.isValid():
            return

        # Redraw the item
        self.itemDelegate().clearLastAction()
        self.redraw(index)

    def mouseDoubleClickEvent(self, event):
        """Receive mouse double click events for the widget."""
        super(BaseTreeView, self).mouseDoubleClickEvent(event)
        index = self.indexAt(event.pos())

        self.setExpandedChildren(index, not self.isExpanded(index))

    def setExpandedChildren(self, index, expanded):
        """
        Set the item referred to by index and all the children to either
        collapse or expanded, depending on the value of expanded.
        """
        if not index.isValid():
            return

        # Expand this
        self.setExpanded(index, expanded)

        # Expand children
        childCount = index.model().rowCount(index)
        for i in range(childCount):
            child = index.child(i, 0)
            self.setExpandedChildren(child, expanded)

    def executeAction(self, index):
        action = self.itemDelegate(index).getLastAction()
        self.model().executeAction(action, index)

    def leaveEvent(self, *args, **kwargs):
        """Clear the action tracking."""
        self.itemDelegate().clearLastAction()

    def redraw(self, index):
        """Regraw given index."""
        rect = self.visualRect(index)
        self.setDirtyRegion(QtGui.QRegion(rect))

    def showProperties(self, event):
        """Show the properties of the item at index `index`."""
        raise NotImplemenedError("{}.showProperties not implemented yet".format(str(self.__class__.__name__)))


class BaseModel(QtCore.QAbstractItemModel):
    """Data model for our QTreeView."""

    def __init__(self, treeView, parent=None):
        """Called after the instance has been created."""
        super(BaseModel, self).__init__(parent)
        self.treeView = weakref.ref(treeView)

        self.rootItem = None
        self.refresh()

    def flags(self, index):
        """The item's flags for the given index."""
        defaultFlags = super(BaseModel, self).flags(index)

        if index.isValid():
            item = index.internalPointer()
            return defaultFlags | item.flags()

        return defaultFlags | QtCore.Qt.ItemIsDropEnabled

    def refresh(self):
        """Read the attributes and recreate the layers."""
        pass

    def rowCount(self, parent=QtCore.QModelIndex()):
        """The number of rows under the given parent."""
        if parent.column() > 0:
            return 0
        if not parent.isValid():
            item = self.rootItem
        else:
            item = parent.internalPointer()

        if not item:
            return 0

        return item.childCount()

    def columnCount(self, parent=None):
        """Return the number of columns for the children of the given parent."""
        return 1

    def data(self, index, role=QtCore.Qt.DisplayRole):
        """
        Return the data stored under the given role for the item referred to by
        the index.
        """
        if not index.isValid():
            return
        item = index.internalPointer()
        if (role == QtCore.Qt.DisplayRole or
                role == QtCore.Qt.EditRole or role == QtCore.Qt.ToolTipRole):
            return item.getName()
        elif role == QtCore.Qt.SizeHintRole:
            return QtCore.QSize(250, ITEM_HEIGHT)
        elif role == QtCore.Qt.BackgroundRole:
            return item.getBackgroundColor()
        elif role == NODE_BAR_COLOUR:
            return item.getLabelColor()
        elif role == CHILD_COUNT:
            return item.childCount()
        elif role == ACTIONS:
            return item.getActions()
        elif role == ICON:
            return item.getIcon()
        elif role == TEXT_INDENT:
            return item.getIndent()
        elif role == NODE_ENABLED:
            return item.isEnabled()

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

        return False

    def indexFromItem(self, node):
        """Create the index that represents the given item in the model."""
        if not node or not node.parent():
            return QtCore.QModelIndex()

        for i in range(node.parent().childCount()):
            if node.parent().child(i) is node:
                return self.createIndex(i, 0, node)

        return QtCore.QModelIndex()

    def index(self, row, column, parent):
        """
        The index of the item in the model specified by the given row, column
        and parent index.
        """
        if not self.hasIndex(row, column, parent):
            return QtCore.QModelIndex()

        if not parent.isValid():
            parentItem = self.rootItem
        else:
            parentItem = parent.internalPointer()

        childItem = parentItem.child(row)
        if childItem:
            return self.createIndex(row, column, childItem)
        else:
            return QtCore.QModelIndex()

    def parent(self, index):
        """The parent of the model item with the given index."""
        if not index.isValid():
            return QtCore.QModelIndex()

        childItem = index.internalPointer()
        if not childItem or not isinstance(childItem, BaseItem):
            return QtCore.QModelIndex()

        parentItem = childItem.parent()

        if not parentItem or parentItem == self.rootItem:
            return QtCore.QModelIndex()

        return self.createIndex(parentItem.row(), 0, parentItem)

    def executeAction(self, action, index):
        """User pressed by one of the actions."""
        if action == BaseItem.ACTION_EXPAND:
            self.treeView().setExpanded(
                index, not self.treeView().isExpanded(index))

    def executeContextAction(self, action, index, point):
        """User pressed with right button by one of the actions."""
        pass


class BaseDelegate(QtWidgets.QStyledItemDelegate):
    """Custom display delegate for the tree items."""

    ICON_PADDING = dpiScale(10)
    COLOR_BAR_WIDTH = dpiScale(6)
    ICON_WIDTH = dpiScale(20)
    ACTION_BORDER = dpiScale(2)
    ACTION_WIDTH = dpiScale(24)
    ICON_TOP_OFFSET = dpiScale(2)

    BACKGROUND_RECT_LENGTH = dpiScale(28)
    BACKGROUND_RECT_LEFT_OFFSET = dpiScale(4)

    DISABLED_BACKGROUND_IMAGE = dpiScaledIcon(":/RS_disabled_tile.png")
    DISABLED_HIGHLIGHT_IMAGE = dpiScaledIcon(":/RS_disabled_tile_highlight.png")

    ICON_HIGHLIGHT = QtGui.QColor(113, 142, 184)

    EXPANDED_ARROW = (
        dpiScale(QtCore.QPointF(12.0, -2.5)),
        dpiScale(QtCore.QPointF(22.0, -2.5)),
        dpiScale(QtCore.QPointF(17.0, 2.5)))
    COLLAPSED_ARROW = (
        dpiScale(QtCore.QPointF(15.0, -5.0)),
        dpiScale(QtCore.QPointF(20.0, 0.0)),
        dpiScale(QtCore.QPointF(15.0, 5.0)))
    ARROW_COLOR = QtGui.QColor(189, 189, 189)

    def __init__(self, treeView):
        """Called after the instance has been created."""
        super(BaseDelegate, self).__init__()
        self.treeView = weakref.ref(treeView)
        self.lastHitAction = None
        self.textColor = self.treeView().palette().text().color()

    def paint(self, painter, option, index):
        """Main entry point of drawing the cell."""
        if not index.isValid():
            return

        # sizeHint = toPyObject(index.data(QtCore.Qt.SizeHintRole))
        # if sizeHint.height() <= 1:
        #     return

        rect = deepcopy(option.rect)
        highlightedColor = option.palette.color(QtGui.QPalette.Highlight)
        isHighlighted = \
            option.showDecorationSelected and \
            option.state & QtWidgets.QStyle.State_Selected

        self.drawBackground(painter, rect, index, isHighlighted, highlightedColor)
        self.drawColorBar(painter, rect, index)
        self.drawFill(painter, rect)
        actionIconRect = self.drawActionIcons(rect, painter, option, index)
        textRect = self.drawText(rect, actionIconRect, painter, index)
        self.drawIcon(textRect, painter, option, index)
        rect = deepcopy(option.rect)
        self.drawArrowDragLock(painter, rect, index)

    def drawArrowDragLock(self, painter, rect, index):
        """Draw the expansion arrow on the nodes that want it."""
        painter.save()

        arrow = None
        if index.data(CHILD_COUNT):
            center = index.data(QtCore.Qt.SizeHintRole).height() / 2
            painter.translate(rect.left(), rect.top() + center)
            # Draw the arrow
            if self.treeView().isExpanded(index):
                arrow = self.EXPANDED_ARROW
            else:
                arrow = self.COLLAPSED_ARROW

            painter.setBrush(self.ARROW_COLOR)
            painter.setPen(QtCore.Qt.NoPen)
            painter.drawPolygon(arrow)

            cursorPosition = self.treeView().mapFromGlobal(QtGui.QCursor.pos())
            if rect.contains(cursorPosition):
                x = cursorPosition.x()
                arrowPoints = [p.x() for p in arrow]
                minX = min(arrowPoints) + rect.left() - EXPAND_SENSITIVITY
                maxX = max(arrowPoints) + rect.left() + EXPAND_SENSITIVITY
                if x >= minX and x <= maxX:
                    # Save the action to expand
                    self.lastHitAction = BaseItem.ACTION_EXPAND

        painter.restore()

    def drawToolbarFrame(self, painter, rect, toolbarCount):
        # draw the darkened toolbar background
        if toolbarCount > 0:
            top = rect.top() + self.ICON_TOP_OFFSET
            backgroundColor = QtGui.QColor(55, 55, 55)
            toolbarLength = toolbarCount*self.ACTION_WIDTH + (2*self.ACTION_BORDER)
            left = rect.right() - (toolbarLength + self.ACTION_BORDER)
            left = left if left > 0 else 0
            painter.setOpacity(0.8)
            painter.fillRect(left, top-self.ACTION_BORDER, toolbarLength, self.ACTION_WIDTH, backgroundColor)
            painter.setOpacity(1.0)

    def drawBackground(
            self, painter, rect, index, isHighlighted, highlightColor):
        painter.save()
        rect2 = deepcopy(rect)

        """Draw the cell bacground color / image."""
        if isHighlighted:
            # Draw the highlight colorif not item.data(renderSetupRoles.NODE_ENABLED):
            if not index.data(NODE_ENABLED):
                painter.drawTiledPixmap(rect2, self.DISABLED_HIGHLIGHT_IMAGE)
            else:
                painter.fillRect(rect2, highlightColor)
        else:
            # Otherwise draw our background color
            background = toPyObject(index.data(QtCore.Qt.BackgroundRole))
            painter.fillRect(rect, background)
            if not index.data(NODE_ENABLED):
                painter.drawTiledPixmap(rect2, self.DISABLED_BACKGROUND_IMAGE)
        painter.restore()

    def drawColorBar(self, painter, rect, index):
        """Draw the label color bar."""
        color = toPyObject(index.data(NODE_BAR_COLOUR))
        rect2 = deepcopy(rect)
        rect2.setRight(rect2.left() + self.COLOR_BAR_WIDTH)
        painter.fillRect(rect2, color)

    def drawFill(self, painter, rect):
        """Draw the border of the cell."""
        painter.save()

        painter.setRenderHint(QtGui.QPainter.Antialiasing)
        painter.setRenderHint(QtGui.QPainter.HighQualityAntialiasing)

        # Draw a 2 pixel border around the box
        painter.setPen(QtGui.QPen(QtGui.QColor(43, 43, 43), 2))
        # painter.drawRoundedRect(rect, 3, 3)
        painter.drawRect(rect)

        painter.restore()

    def drawIcon(self, rect, painter, option, index):
        """Draw the item's icon."""
        painter.save()

        if (index.column() == 0):
            icon = toPyObject(index.data(ICON))
            if icon:
                newRect = QtCore.QRect()
                padding = dpiScale(4)

                center = \
                    toPyObject(index.data(QtCore.Qt.SizeHintRole)).height() / 2
                newRect.setRight(rect.left() - padding)
                newRect.setLeft(newRect.right() - self.ICON_WIDTH + dpiScale(1))
                newRect.setTop(rect.top() + center - self.ICON_WIDTH / 2)
                newRect.setBottom(newRect.top() + self.ICON_WIDTH - dpiScale(1))
                painter.drawPixmap(newRect, icon)

        painter.restore()

    def drawText(self, rect, actionIconRect, painter, index):
        """Draw the node name."""
        # Text color
        painter.setPen(
            QtGui.QPen(QtGui.QColor(182, 182, 182), 1))

        # Text font
        painter.setFont(QtWidgets.QApplication.font())

        # Draw the text for the node
        textRect = deepcopy(rect)
        textRect.setBottom(textRect.bottom() + dpiScale(2))
        textRect.setLeft(
            textRect.left() +
            toPyObject(index.data(TEXT_INDENT)) +
            self.ICON_PADDING)

        right = textRect.right() - dpiScale(11)
        if actionIconRect:
            right = actionIconRect.left() - dpiScale(11)
        textRect.setRight(right)

        painter.drawText(
            textRect,
            QtCore.Qt.AlignLeft | QtCore.Qt.AlignVCenter,
            str(toPyObject(index.data(QtCore.Qt.DisplayRole))))

        return textRect

    def drawPixmap(self, painter, pixmap, left, top):
        painter.drawPixmap(QtCore.QRect(left, top, self.ICON_WIDTH, self.ICON_WIDTH), pixmap)

    def drawActionIcons(self, rect, painter, option, index):
        """Draw the icons and buttons on the right side of the item."""
        painter.save()

        actions = toPyObject(index.data(ACTIONS))

        buttonPressed = \
            QtWidgets.QApplication.mouseButtons() == QtCore.Qt.LeftButton or \
            QtWidgets.QApplication.mouseButtons() == QtCore.Qt.RightButton

        # Position
        center = toPyObject(index.data(QtCore.Qt.SizeHintRole)).height() / 2
        start = self.ACTION_BORDER

        self.drawToolbarFrame(painter, rect, len([a for a in actions if a[0]]))

        iconRectCumul = None
        for pixmap, opacity, action, checked in actions:

            if not pixmap or not opacity:
                continue

            cursorPosition = self.treeView().mapFromGlobal(QtGui.QCursor.pos())

            # Draw icon
            start += self.ICON_WIDTH + self.ACTION_BORDER

            width = min(self.ICON_WIDTH, pixmap.rect().width())
            height = min(self.ICON_WIDTH, pixmap.rect().height())

            left = rect.right() - start + (self.ICON_WIDTH - width) / 2
            top = rect.top() + center - height / 2

            painter.setOpacity(1.0)

            iconRect = QtCore.QRect(
                left,
                top,
                width,
                height)
            iconRectCumul = iconRect

            backgroundRect = QtCore.QRect(left - self.BACKGROUND_RECT_LEFT_OFFSET, top - self.ACTION_BORDER, self.BACKGROUND_RECT_LENGTH, self.ACTION_WIDTH)

            if checked:
                painter.fillRect(iconRect, self.ICON_HIGHLIGHT)

            painter.setRenderHint(QtGui.QPainter.SmoothPixmapTransform)

            if not iconRect.contains(cursorPosition):
                painter.setOpacity(opacity * 0.7)
            else:
                painter.setOpacity(opacity)
                self.lastHitAction = action

            painter.drawPixmap(iconRect, pixmap)

            # Highlight the icon depending on the mouse over.
            if buttonPressed and iconRect.contains(cursorPosition):
                painter.setOpacity(0.75)
                painter.setRenderHint(QtGui.QPainter.Antialiasing)
                painter.setRenderHint(QtGui.QPainter.HighQualityAntialiasing)

                # Set the clip region to the given rectangle
                painter.setClipping(True)
                painter.setClipRect(iconRect)

                # Form a new rectangle to draw a circle
                centerRect = iconRect.center()
                tick = 1.0

                iconRect.setCoords(
                    centerRect.x() - tick * self.ICON_WIDTH,
                    centerRect.y() - tick * self.ICON_WIDTH,
                    centerRect.x() + tick * self.ICON_WIDTH,
                    centerRect.y() + tick * self.ICON_WIDTH)

                painter.setBrush(QtGui.QBrush(self.ICON_HIGHLIGHT))
                painter.setPen(QtCore.Qt.NoPen)

                painter.drawEllipse(iconRect)

                # Save the action to know

                painter.setClipping(False)

        painter.restore()
        return iconRectCumul

    def getLastAction(self):
        """Track action to process clicks on the icons."""
        action = self.lastHitAction
        self.clearLastAction()
        return action

    def clearLastAction(self):
        """Clear the action tracking."""
        self.lastHitAction = None

    def updateEditorGeometry(self, editor, option, index):
        """Update the editor's rectangle for the item specified by index."""
        rect = deepcopy(option.rect)

        rigthCumul = 0
        start = self.ACTION_BORDER
        actions = toPyObject(index.data(ACTIONS))
        for pixmap, opacity, action in actions:
            if not pixmap or not opacity:
                continue

            start += self.ICON_WIDTH + self.ACTION_BORDER
            width = min(self.ICON_WIDTH, pixmap.rect().width())
            rigthCumul = rect.right() - start + (self.ICON_WIDTH - width) / 2

        if rigthCumul:
            right = rigthCumul - dpiScale(11)
        else:
            right = rect.right() - dpiScale(11)

        # Offset the rectangle because we need to see all the decorations.
        leftOffset = \
            toPyObject(index.data(TEXT_INDENT)) + \
            self.ICON_PADDING - \
            2 * self.ACTION_BORDER
        topOffset = self.ACTION_BORDER
        bottomOffset = self.ACTION_BORDER

        rect.setCoords(
            rect.left() + leftOffset,
            rect.top() + topOffset,
            right,
            rect.bottom() - bottomOffset)

        editor.setGeometry(rect)


class BaseItem(object):
    """
    A Python object used to store an item's data, and keep note of it's parents
    and/or children. It's a tree data structure with parent and children.
    """

    ACTION_EXPAND = 0

    def __init__(self, parentItem, name, index=-1):
        """Called after the instance has been created."""
        self.name = name
        self.childItems = []
        if index >= 0:
            self.setParent(parentItem, index)
        else:
            self.setParent(parentItem)

    def getName(self):
        """The label of the item."""
        return self.name

    def appendChild(self, child):
        """Add a child to the data structure."""
        child.parentItem = self
        self.childItems.append(child)

    def insertChild(self, child, rowNumber):
        """Insert a child item."""
        child.parentItem = self
        self.childItems.insert(rowNumber, child)

    def removeChild(self, child):
        for child_ in child.childItems:
            child.removeChild(child_)
        child.parentItem = None
        self.childItems.remove(child)

    def setParent(self, parent, position=-1):
        """Set the parent item."""
        if parent:
            if position >= 0 and position < len(parent.childItems):
                parent.insertChild(self, position)
            else:
                parent.appendChild(self)
        else:
            self.parentItem = None

    def child(self, rowNumber):
        """Return the child if exists."""
        if rowNumber < len(self.childItems):
            return self.childItems[rowNumber]
        else:
            return None

    def childCount(self):
        """The number of the children."""
        return len(self.childItems)

    def rowCount(self, parentIndex=QtCore.QModelIndex()):
        """Return the number of rows under the given parent."""
        parent = self.parentItem
        if not parent:
            return 0
        return parent.childCount()

    def parent(self):
        """Return the parent TreeItem of the current item."""
        return self.parentItem

    def row(self):
        """The index of current node in the list of children of the parent."""
        if self.parent():
            return self.parent().childItems.index(self)
        return 0

    def getActions(self):
        """
        The list of actions to draw on the right side of the item in the tree
        view. It should return the data in the following format:
        [(icon1, opacity1, action1), (icon2, opacity2, action2), ...]
        """
        return []

    def flags(self):
        """The item's flags."""
        return QtCore.Qt.ItemIsDropEnabled

    def getIcon(self):
        """
        The icon of current node. It can be different depending on the item
        type.
        """
        pass

    def getBackgroundColor(self):
        """
        The background color of current node. It can be different depending on
        the item type.
        """
        return QtGui.QColor(71, 71, 71)

    def getLabelColor(self):
        """
        The background color of current node. It can be different depending on
        the item type.
        """
        return QtGui.QColor(113, 142, 164)

    def getIndent(self):
        """The text indent."""
        return dpiScale(20)

    def isEnabled(self):
        return True

    @staticmethod
    def dpiScaledIcon(path):
        """Creates QPixmap and scales it for hi-dpi mode"""
        return dpiScaledIcon(path)

    @staticmethod
    def coloredIcon(fileName, color=QtGui.QColor(255, 255, 255, 255)):
        """Load an icon and multiply it by specified color"""
        rgba = color.rgba()
        image = BaseItem.dpiScaledIcon(fileName).toImage()
        for x in range(image.width()):
            for y in range(image.height()):
                currentColor = image.pixel(x, y)
                image.setPixel(x, y, currentColor & rgba)

        return QtGui.QPixmap.fromImage(image)

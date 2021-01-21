
import os
import re
import arnold as ai
from mtoa.ui.ae.templates import createTranslatorMenu
from mtoa.callbacks import *
import mtoa.core as core
import mtoa.utils as mu
import maya.cmds as cmds
import mtoa.ui.ae.utils as aeUtils
from mtoa.ui.ae.aiImagersBaseTemplate import getImagerTemplate

from mtoa.ui.qt import toQtObject, toMayaName, clearWidget
from mtoa.ui.qt.Qt import QtWidgets, QtCore, QtGui, shiboken

from mtoa.ui.qt import BaseTreeView, BaseModel, BaseDelegate, \
                       BaseItem, BaseWindow, dpiScale, Timer
from mtoa.ui.qt import MtoAStyle

from mtoa.ui.qt.treeView import *


IMAGER_MIME_TYPE = "application/arnold/imager"
MAX_WIDTH = 16777215
IMAGERS_ATTR = "defaultArnoldRenderOptions.imagers"


class ImagerStackView(BaseTreeView):
    """docstring for ProceduralTree"""

    itemSelected = QtCore.Signal(str, object)

    MIN_VISIBLE_ENTRIES = 4
    MAX_VISIBLE_ENTRIES = 10

    itemSelected = QtCore.Signal(str)
    itemRemoved = QtCore.Signal(str)
    itemDropped = QtCore.Signal()

    def __init__(self, transverser = None, parent=None, showPlaceholder=True):
        super(ImagerStackView, self).__init__(parent)
        model = ImagerStackModel(self)
        self.baseModel = model
        self.transverser = None
        self.dragStartPosition = None
        self.showPlaceholder = showPlaceholder
        self.setModel(model)

        self.setSelectionBehavior(QtWidgets.QAbstractItemView.SelectRows)

        self.setDragEnabled(True)

        self.setDropIndicatorShown(True)
        self.setDragDropMode(QtWidgets.QAbstractItemView.InternalMove)
        self.setDefaultDropAction(QtCore.Qt.MoveAction)

        self.setContextMenuPolicy(QtCore.Qt.CustomContextMenu)
        self.customContextMenuRequested.connect(self.rightClickMenu)

        self.actionDisable = None
        self.actionDelete = None
        self.contextMenu = None
        self.actionMoveUp = None
        self.actionMoveDown = None

        delegate = ImagerStackDelegate(self)
        self.setItemDelegate(delegate)
        self.setTransverser(transverser)

    def rightClickMenu(self, pos):
        if not self.contextMenu:
            self.contextMenu = QtWidgets.QMenu(self)
        self.contextMenu.clear()
        index = self.indexAt(pos)
        item = index.internalPointer()
        if not item:
            return

        self.actionDisable = self.contextMenu.addAction("Disable Imager" if item.enabled else "Enable Imager")
        self.actionDisable.triggered.connect(lambda arg=None: self.disableImager(pos))
        self.actionDelete = self.contextMenu.addAction("Remove Imager")
        self.actionDelete.triggered.connect(lambda arg=None: self.removeImagerAtPos(pos))
        self.actionMoveUp = None
        self.actionMoveDown = None

        imagers = self.model().imagers
        if imagers and len(imagers) > 1:
            itemIndex = imagers.index(item.name)
            if itemIndex > 0:
                self.actionMoveUp = self.contextMenu.addAction("Move Up")
                self.actionMoveUp.triggered.connect(lambda arg=None: self.moveImagerUp(itemIndex))
            if itemIndex < len(imagers) - 1:
                self.actionMoveDown = self.contextMenu.addAction("Move Down")
                self.actionMoveDown.triggered.connect(lambda arg=None: self.moveImagerDown(itemIndex))

        self.contextMenu.popup(self.mapToGlobal(pos))

    def disableImager(self, pos):
        index = self.indexAt(pos)
        self.model().executeAction(ImagerItem.ACTION_DISABLE, index)

    def removeImagerAtPos(self, pos):
        index = self.indexAt(pos)
        self.removeImager(index)

    def removeSelectedImager(self):
        index = self.currentIndex()
        self.removeImager(index)

    def removeImager(self, index):
        item = index.internalPointer()
        self.model().removeImager(item.name)
        self.itemRemoved.emit(item.name)

    def moveImagerUp(self, itemIndex):
        self.model().moveImagerUp(itemIndex)

    def moveImagerDown(self, itemIndex):
        self.model().moveImagerDown(itemIndex)

    def setTransverser(self, transverser):
        self.transverser = transverser
        self.model().setTransverser(transverser)

    def dropEvent(self, event):
        super(ImagerStackView, self).dropEvent(event)
        self.dragStartPosition = None
        self.itemDropped.emit()

    def startDrag(self, actions):
        if self.dragStartPosition:
            index = self.indexAt(self.dragStartPosition)
            item = index.internalPointer()
            # onlly allow drag if we have an item at the drag start position
            if item:
                super(ImagerStackView, self).startDrag(actions)

    def mousePressEvent(self, event):
        super(ImagerStackView, self).mousePressEvent(event)
        if event.button() == QtCore.Qt.LeftButton:
            self.dragStartPosition = event.pos()

    # # Every time we select an imager element, we want to
    # # select it in Maya
        index = self.indexAt(event.pos())
        if index:
            item = index.internalPointer()
            if item:
                item.selectImager()
                self.itemSelected.emit(item.getNodeName())

    def paintEvent(self, event):
        super(ImagerStackView, self).paintEvent(event)
        if self.model().rowCount() == 0 and self.showPlaceholder:
            painter = QtGui.QPainter(self.viewport())
            font = painter.font()
            bigFontSize = 8
            smallFontSize = 8
            font.setPointSize(bigFontSize)
            font.setBold(True)
            painter.setFont(font)
            textCol = QtGui.QColor(150, 150, 150)
            painter.setPen(textCol)
            pixMap = QtGui.QPixmap(os.path.join(mu.rootdir(), 'icons', "empty_box_dark-grey.svg"))

            iconSize = pixMap.size()
            iconRect = QtCore.QRect(event.rect().center().x() - (iconSize.width() / 2),
                                    event.rect().center().y() - (iconSize.height() / 2) - dpiScale(16),
                                    iconSize.width(), iconSize.height())

            painter.drawPixmap(iconRect, pixMap)

            metrics = QtGui.QFontMetrics(font)
            listEmpty = "Imagers List is empty."
            addImagers_1 = "Click Add to create Color Correct,"
            addImagers_2 = "Light Mix or Effects Imagers."

            # Draw the first line of text
            textSize = metrics.size(QtCore.Qt.TextSingleLine, listEmpty)
            p = event.rect().center()
            p.setX(p.x() - textSize.width() / 2)
            p.setY(p.y() + textSize.height() / 2 + (iconSize.height() / 2))
            painter.drawText(p, listEmpty)

            # Draw the second line of text
            font.setPointSize(smallFontSize)
            font.setBold(False)
            m = QtGui.QFontMetrics(font)
            painter.setFont(font)
            textSize = m.size(QtCore.Qt.TextSingleLine, addImagers_1)
            p.setX(event.rect().center().x() - textSize.width() / 2)
            p.setY(p.y() + metrics.lineSpacing() - dpiScale(2))
            painter.drawText(p, addImagers_1)

            # Draw the third line of text
            textSize = m.size(QtCore.Qt.TextSingleLine, addImagers_2)
            p.setX(event.rect().center().x() - textSize.width() / 2)
            p.setY(p.y() + metrics.lineSpacing() - dpiScale(2))
            painter.drawText(p, addImagers_2)

    def remapImagersAttr(self):
        if shiboken.isValid(self) and self.model():
            self.model().remapImagersAttr()


class ImagerStackModel(BaseModel):
    def __init__(self, treeView, parent=None):

        self.transverser = None
        self.imagers = []
        self.scriptJobList = []
        self.dropMimeDataFailure = False
        super(ImagerStackModel, self).__init__(treeView, parent)

    def setTransverser(self, transverser):
        self.transverser = transverser
        self.refresh()

    def refresh(self):
        self.beginResetModel()
        self.imagers = []

        self.rootItem = ImagerItem(None, "")
        # get all the imagers 
        imagers = cmds.listConnections(IMAGERS_ATTR) or []

        for imager in imagers:
            if cmds.objExists(imager) and cmds.attributeQuery('enable', node=imager, exists=True):
                enableAttr = '{}.enable'.format(imager)
                enabled = cmds.getAttr(enableAttr)
                ImagerItem(self.rootItem, imager, enabled)
                self.imagers.append(imager)
                if imager not in self.scriptJobList:
                    self.scriptJobList.append(imager)
                    cmds.scriptJob(attributeChange=[enableAttr, lambda *args: self.refresh()])
                    cmds.scriptJob(nodeDeleted=[imager, lambda *args: self.remapImagersAttr(True)])

        self.endResetModel()

    def remapImagersAttr(self, refresh=False):
        imagers = cmds.listConnections(IMAGERS_ATTR, p=True, d=False,s=True) or []

        multiIndices = cmds.getAttr(IMAGERS_ATTR, mi=True) or []
        if not len(multiIndices) == len(imagers):
            for mi in multiIndices:
                _attr = IMAGERS_ATTR+"[{}]".format(int(mi))
                cmds.removeMultiInstance(_attr , b=True)

            i = 0
            for imager in imagers:
                cmds.connectAttr(imager, IMAGERS_ATTR+"[{}]".format(i))
                i += 1

        if refresh:
            self.refresh()

    def removeImager(self, imager):
        if not self.imagers or len(self.imagers) == 0:
            return
        imagersLength = len(self.imagers)
        index = self.imagers.index(imager)

        if index < 0 or index >= imagersLength:
            return 
        for i in range(index, imagersLength):
            # first disconnect the existing imagers connected at this index
            imagerElemAttr = IMAGERS_ATTR+'[%d]' % i
            elemConnection = cmds.listConnections(imagerElemAttr,p=True, d=False,s=True)
            if (not elemConnection is None):
                for elem in elemConnection:
                    cmds.disconnectAttr(elem, imagerElemAttr)

            # then, connect the following element to the current one
            if i < imagersLength - 1:
                imagerNextElemAttr = IMAGERS_ATTR+'[%d]' % (i+1)
                nextElemConnection = cmds.listConnections(imagerNextElemAttr,p=True, d=False,s=True)
                if (not nextElemConnection is None) and len(nextElemConnection) > 0:
                    cmds.connectAttr(nextElemConnection[0], imagerElemAttr)

        imagerElemAttr = IMAGERS_ATTR+'[%d]' % (imagersLength - 1)
        cmds.removeMultiInstance(imagerElemAttr , b=True)
        self.remapImagersAttr()

    def moveImagerUp(self, itemIndex):
        if not self.imagers:
            return
        
        if itemIndex <= 0:
            return
        imagerElemAttr = IMAGERS_ATTR+'[%d]' % (itemIndex)
        elemConnection = cmds.listConnections(imagerElemAttr,p=True, d=False,s=True)
        if elemConnection is None or len(elemConnection) == 0:
            return
        currentImager = elemConnection[0]
        cmds.disconnectAttr(currentImager, imagerElemAttr)

        otherImagerElemAttr = IMAGERS_ATTR+'[%d]' % (itemIndex-1)
        elemConnection = cmds.listConnections(otherImagerElemAttr,p=True, d=False,s=True)
        if elemConnection is None or len(elemConnection) == 0:
            return
        otherImager = elemConnection[0]
        cmds.disconnectAttr(otherImager, otherImagerElemAttr)

        cmds.connectAttr(otherImager, imagerElemAttr)
        cmds.connectAttr(currentImager, otherImagerElemAttr)
        self.refresh()

    def moveImagerDown(self, itemIndex):
        if not self.imagers:
            return

        if itemIndex >= len(self.imagers) -1:
            return

        imagerElemAttr = IMAGERS_ATTR+'[{}]'.format(itemIndex)
        elemConnection = cmds.listConnections(imagerElemAttr,p=True, d=False,s=True)
        if elemConnection is None or len(elemConnection) == 0:
            return
        currentImager = elemConnection[0]
        cmds.disconnectAttr(currentImager, imagerElemAttr)

        otherImagerElemAttr = IMAGERS_ATTR+'[%d]' % (itemIndex+1)
        elemConnection = cmds.listConnections(otherImagerElemAttr,p=True, d=False,s=True)
        if elemConnection is None or len(elemConnection) == 0:
            return
        otherImager = elemConnection[0]
        cmds.disconnectAttr(otherImager, otherImagerElemAttr)

        cmds.connectAttr(otherImager, imagerElemAttr)
        cmds.connectAttr(currentImager, otherImagerElemAttr)
        self.refresh()

    def executeAction(self, action, index):
        """User pressed by one of the actions."""
        item = index.internalPointer()

        if action == ImagerItem.ACTION_DISABLE:
            enableAttr = '{}.enable'.format(item.name)
            wasEnabled = cmds.getAttr(enableAttr)
            item.enabled = not item.enabled
            cmds.setAttr(enableAttr, item.enabled)

    def data(self, index, role=QtCore.Qt.DisplayRole):
        """
        Return the data stored under the given role for the item referred to by
        the index.
        """
        if not index.isValid():
            return
        item = index.internalPointer()

        if (role == QtCore.Qt.DisplayRole or
                role == QtCore.Qt.EditRole):
            return item.getName()
        if role == QtCore.Qt.ToolTipRole:
            return "{} : {}".format(item.getName(), item.getNodeType())
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

    def supportedDropActions(self):
        return QtCore.Qt.MoveAction

    def mimeTypes(self):
        return [ IMAGER_MIME_TYPE ]

    def mimeData(self, indices):
        ''' This method builds the mimeData if the selection is correct '''

        # On drag start, prepare to pass the names of the dragged items to the drop mime data handler
        self.dropMimeDataFailure = False

        # Prepare the entries to move
        mimeData = QtCore.QMimeData()
        if not self.dropMimeDataFailure:
            encodedData = QtCore.QByteArray()
            stream = QtCore.QDataStream(encodedData, QtCore.QIODevice.WriteOnly)
            for index in indices:
                stream.writeString(self.data(index, QtCore.Qt.EditRole))
            mimeData.setData(IMAGER_MIME_TYPE, encodedData)

        return mimeData

    def dropMimeData(self, mimeData, action, row, column, parentIndex):

        if self.dropMimeDataFailure:
            # The mimeData parsing faced a type mismatch
            return False

        self.dropMimeDataFailure = False

        if action == QtCore.Qt.IgnoreAction:
            return False

        if not mimeData.hasFormat(IMAGER_MIME_TYPE) or column > 0:
            self.dropMimeDataFailure = True
            return False

        # row is -1 when dropped on a parent item and not between rows.
        #   In that case we want to do nothing
        if row == -1:
            return False

        # Parse the mime data that was passed to us (a list of item string names)
        encodedData = mimeData.data(IMAGER_MIME_TYPE)
        stream = QtCore.QDataStream(encodedData, QtCore.QIODevice.ReadOnly)

        # Is the drop allowed ?
        items = []
        while not stream.atEnd():
            name = stream.readString()
            item = self.findProxyItem(name)
            items.append(item)

        # Now do the drop
        i = 0
        for item in items:
            destinationPosition = row + i
            # if item._model.parent() == destinationModel and row > item.row():
            #     destinationPosition -= 1
            self.moveItem(item, destinationPosition)
            i += 1
        return not self.dropMimeDataFailure

    def findProxyItem(self, name):
        count = self.rowCount()
        for i in range(0, count):
            index = self.index(i, 0)
            item = index.internalPointer()
            if name == item.name:
                return item
        return None

    def moveItem(self, item, position):
        """
        * Move item to position.
        * Reorder the current item in position
          to one index below position
        """
        self.remapImagersAttr()

        # get index for given item
        itemIndex = self.imagers.index(item.getNodeName())
        oldElemAttr = IMAGERS_ATTR+'[{}]'.format(itemIndex)
        oldElemConnection = cmds.listConnections(oldElemAttr, p=True, d=False, s=True)
        movedImager = oldElemConnection[0]

        # nothing to do the position is the same as before
        if position == itemIndex:
            return

        # disconnect the current item at the given index
        # cmds.disconnectAttr(currentImager, imagerElemAttr)
        # disconnect the moved item from it's current index
        cmds.disconnectAttr(movedImager, oldElemAttr)

        # are we moveing down or up?
        up = position < itemIndex
        if not up:
            position -= 1

        # get the item currently in the given position
        imagerElemAttr = IMAGERS_ATTR+'[{}]'.format(position)
        elemConnection = cmds.listConnections(imagerElemAttr, p=True, d=False, s=True)
        if elemConnection is None or len(elemConnection) == 0:
            return

        # move the nodes below the given position up/down by one index starting
        # at the index before the moved one
        if up:
            idx = itemIndex-1
            while idx >= position:
                imager = self.imagers[idx]
                elemAttr = IMAGERS_ATTR+'[{}]'.format(idx)
                oldElemConnection = cmds.listConnections(elemAttr, p=True, d=False, s=True)
                if oldElemConnection:
                    cmds.disconnectAttr(oldElemConnection[0], elemAttr)
                    targetAttr = IMAGERS_ATTR+'[{}]'.format(idx+1)
                    cmds.connectAttr('{}.message'.format(imager), targetAttr)
                idx -= 1
        else:
            idx = itemIndex+1
            while idx <= position:
                imager = self.imagers[idx]
                elemAttr = IMAGERS_ATTR+'[{}]'.format(idx)
                oldElemConnection = cmds.listConnections(elemAttr, p=True, d=False, s=True)
                if oldElemConnection:
                    cmds.disconnectAttr(oldElemConnection[0], elemAttr)
                    targetAttr = IMAGERS_ATTR+'[{}]'.format(idx-1)
                    cmds.connectAttr('{}.message'.format(imager), targetAttr)
                idx += 1

        # reconnect the node in the new position
        cmds.connectAttr(movedImager, imagerElemAttr)
        self.refresh()


class ImagerStackDelegate(BaseDelegate):

    def __init__(self, treeView):
        super(ImagerStackDelegate, self).__init__(treeView)


class ImagerItem(BaseItem):
    DISABLED_ICON = BaseItem.dpiScaledIcon(":/RS_disable.png")

    BACKGROUND_COLOR = QtGui.QColor(82, 82, 82)
    BAR_COLOR = QtGui.QColor(144, 196, 222)

    (ACTION_EXPAND,  # Always first, even if not used
     ACTION_NONE,
     ACTION_SELECT,
     ACTION_DISABLE) = range(4)

    def __init__(self, parentItem, name, enabled=True, index=-1):
        super(ImagerItem, self).__init__(parentItem, name, index)
        self.enabled = enabled

    def selectImager(self):
        cmds.select(self.name, r=True)

    def getNodeName(self):
        return self.name

    def getNodeType(self):
        return aeUtils.getNodeType(self.name)

    def getLabelColor(self):
        return self.BAR_COLOR

    def getBackgroundColor(self):
        return self.BACKGROUND_COLOR

    def getActions(self):
        actions = []
        actions.append((self.DISABLED_ICON, 1.0, self.ACTION_DISABLE, not self.enabled, None))
        return actions

    def isEnabled(self):
        return self.enabled

    def flags(self):
        """The item's flags."""
        return QtCore.Qt.ItemIsDropEnabled | QtCore.Qt.ItemIsDragEnabled


class ImagersUI(QtWidgets.QFrame):

    def __init__(self, parent=None,listOnly = False):
        super(ImagersUI, self).__init__(parent )

        style = MtoAStyle.currentStyle()
        self.parent = parent
        self.parentMayaName = toMayaName(parent)
        self.listOnly = listOnly
        self.scriptJobs = []
        self.nodes = []

        self.layout = QtWidgets.QVBoxLayout(self)
        self.setLayout(self.layout)

        self.toolBar = QtWidgets.QFrame()
        self.toolBar.setLayout(QtWidgets.QHBoxLayout())
        self.layout.addWidget(self.toolBar)

        self.addImagerButton = QtWidgets.QPushButton("Add Imager")
        self.toolBar.layout().addWidget(self.addImagerButton)
        self.imagerMenu = QtWidgets.QMenu()
        self.addImagerButton.setMenu(self.imagerMenu)

        self.imagerMenu.triggered.connect(self.addImagerAction)

        self.removeImagerButton = QtWidgets.QPushButton("Remove Imager")
        self.removeImagerButton.setDisabled(True)
        self.toolBar.layout().addWidget(self.removeImagerButton)

        self.removeImagerButton.pressed.connect(self.removeImagerAction)

        self.toolBar.layout().addItem(QtWidgets.QSpacerItem(0, 0, QtWidgets.QSizePolicy.Expanding, QtWidgets.QSizePolicy.Minimum))

        self.splitter = QtWidgets.QSplitter(self)
        self.splitter.setOrientation(QtCore.Qt.Vertical)
        self.splitter.setObjectName("splitter")

        self.imagerStack = ImagerStackView(None, self.splitter, showPlaceholder=not self.listOnly)
        self.imagerStack.setObjectName("ImagerStackWidget")
        self.imagerStack.setMinimumHeight(dpiScale(100))
        self.layout.addWidget(self.splitter)
        self.attributeScrollArea = None
        self.scrollAreaWidgetContents = None
        self.scrollAreaLayout = None

        if not self.listOnly:
            self.attributeScrollArea = QtWidgets.QScrollArea(self.splitter)
            self.attributeScrollArea.setObjectName("AttributeScrollArea")
            self.attributeScrollArea.setWidgetResizable(True)
            self.attributeScrollArea.setMinimumHeight(dpiScale(200))

            self.scrollAreaWidgetContents = QtWidgets.QWidget()
            self.scrollAreaWidgetContents.setObjectName("scrollAreaWidgetContents")
            self.scrollAreaLayout = QtWidgets.QVBoxLayout(self.scrollAreaWidgetContents)

            self.attributeScrollArea.setWidget(self.scrollAreaWidgetContents)

            self.imagerAttributesFrame = None

            self.imagerStack.itemSelected.connect(self.showItemProperties)
            self.imagerStack.itemRemoved.connect(self.updateImagers)

        self.splitter.setSizes([MAX_WIDTH, MAX_WIDTH])

        # every time the attribute imagers in the options node is modified, we want to update the widget
        self.scriptJobs.append(cmds.scriptJob(attributeChange=[IMAGERS_ATTR, self.updateImagers], dri=True, alc=True))
        self.scriptJobs.append(cmds.scriptJob(connectionChange=[IMAGERS_ATTR, self.connectionUpdate]))
        self.scriptJobs.append(cmds.scriptJob(event=["NewSceneOpened", self.newSceneCallback]))
        self.scriptJobs.append(cmds.scriptJob(event=["PostSceneRead", self.newSceneCallback]))
        self.scriptJobs.append(cmds.scriptJob(event=["SelectionChanged", self.updateSelection]))

        self.imagerStack.itemDropped.connect(self.updateSelection)
        self.imagerStack.selectionModel().selectionChanged.connect(self.selectionChanged)

        self.updateImagers()
        cmds.setParent('..')

    @QtCore.Slot(str)
    def showItemProperties(self, node):

        if self.scrollAreaWidgetContents:
            # clear the scrollAreaWidgetContents
            clearWidget(self.scrollAreaWidgetContents)
        else:
            return

        if not node or node not in self.imagerStack.model().imagers or \
           len(self.imagerStack.model().imagers) == 0 or self.listOnly:
            return

        cmds.setParent(self.parentMayaName)
        imagerAttributesFrame = cmds.rowColumnLayout('ImagersAttributeFrame',
                                                     numberOfColumns=1,
                                                     columnAttach=[1, "both", 5])

        frameQTWidget = toQtObject(imagerAttributesFrame, QtWidgets.QWidget)
        frameQTWidget.setLayout(QtWidgets.QVBoxLayout())

        imagersUITemplate = getImagerTemplate(cmds.nodeType(node))

        if imagersUITemplate:
            imagersUITemplate(parent=imagerAttributesFrame, nodeName=node)

        frameQTWidget.setSizePolicy(QtWidgets.QSizePolicy.Expanding, QtWidgets.QSizePolicy.Expanding)
        frameQTWidget.setMaximumWidth(MAX_WIDTH)
        children = frameQTWidget.children()
        for c in children:
            if isinstance(c, QtWidgets.QWidget):
                frameQTWidget.layout().removeWidget(c)
                self.scrollAreaLayout.addWidget(c)
                c.setSizePolicy(QtWidgets.QSizePolicy.Expanding, QtWidgets.QSizePolicy.Minimum)
                c.setMaximumWidth(MAX_WIDTH)
                c.updateGeometry()

        cmds.deleteUI(imagerAttributesFrame)
        self.scrollAreaLayout.addItem(QtWidgets.QSpacerItem(0, 0, QtWidgets.QSizePolicy.Minimum, QtWidgets.QSizePolicy.Expanding))

        cmds.setParent('..')

    def selectionChanged(self, selected, deselected):
        self.removeImagerButton.setDisabled(selected.isEmpty())

    def newSceneCallback(self):
        for job in self.scriptJobs:
            if not cmds.scriptJob(exists=job):
                self.scriptJobs.pop(self.scriptJobs.index(job))

        # the options node might not exist anymore
        if cmds.objExists('defaultArnoldRenderOptions'):
            self.scriptJobs.append(cmds.scriptJob(attributeChange=[IMAGERS_ATTR, self.updateImagers], dri=True, alc=True))
            self.scriptJobs.append(cmds.scriptJob(connectionChange=[IMAGERS_ATTR, self.connectionUpdate]))
            self.updateImagers()

    def enterEvent(self, event):
        super(ImagersUI, self).enterEvent(event)
        # ensure the list and selection is up-to-date    
        self.connectionUpdate()
        self.updateSelection()
    
    def remapImagersAttr(self):
        self.imagerStack.remapImagersAttr()
        self.updateImagers()

    def connectionUpdate(self):
        # get if we have an empty connection
        dorefresh = False

        _nodes = cmds.listConnections(IMAGERS_ATTR) or []
        mi = cmds.getAttr(IMAGERS_ATTR, mi=True) or []
        for i in mi:
            _attr = IMAGERS_ATTR+"[{}]".format(int(i))
            connection = cmds.listConnections(_attr)
            if not connection:
                dorefresh = True
                break

        if dorefresh or not self.nodes == _nodes:
            self.remapImagersAttr()

    def updateImagers(self, selectLast=False):
        if shiboken.isValid(self.imagerStack):

            # get current selection
            selection = self.imagerStack.selectedIndexes()
            idxs = []
            for s in selection:
                idxs.append(s.row())

            sceneSelection = cmds.ls(sl=True)

            self.imagerStack.model().refresh()
            self.nodes = self.imagerStack.model().imagers
            # update the add imagers menu
            self.buildImagerMenu()
            # if there are no imagers selected clear the items panel
            if not self.imagerStack.model().rowCount():
                self.showItemProperties(None)

            # select the imager that matches the current scene selection
            if any(i in self.nodes for i in sceneSelection):
                self.updateSelection()
            else:
                # otherwise select was was selected previously
                for idx in idxs:
                    self.imagerStack.setCurrentIndex(self.imagerStack.model().index(idx, 0))
                    n = self.nodes[idx] if len(self.nodes) > idx else None
                    self.showItemProperties(n)

            self.removeImagerButton.setDisabled(not len(self.imagerStack.selectedIndexes()))
            #self.scriptJobs.append(cmds.scriptJob(connectionChange=[IMAGERS_ATTR, self.connectionUpdate]))

    def updateSelection(self, node=None):
        # we only want to update the selection if the UI is visible.
        # Otherwise, this will be updated in the next mouse enter
        if not shiboken.isValid(self) or not self.isVisible():
            return
        nodes = []
        if shiboken.isValid(self.imagerStack) and self.imagerStack.isVisible():
            nodes = self.imagerStack.model().imagers or []
            noSelection = len(self.imagerStack.selectedIndexes()) == 0
            if len(nodes) > 0:
                sceneSelection = cmds.ls(sl=True)
                # select the imager that matches the current scene selection
                for s in sceneSelection:
                    if s in nodes:
                        idx = nodes.index(s)
                        self.imagerStack.setCurrentIndex(self.imagerStack.model().index(idx, 0))
                        self.showItemProperties(s)
                        noSelection = False

            if noSelection:
                self.showItemProperties(None)

    def createImager(self, nodeType):
        imager = cmds.createNode(nodeType)
        self.addImager(imager)

    def addImager(self, node):
        imagersSize = cmds.getAttr(IMAGERS_ATTR, s=True)
        attrName = IMAGERS_ATTR+'[%d]' % imagersSize

        cmds.connectAttr("%s.message"%node, attrName, force=True)
        self.updateImagers(True)

    def addImagerAction(self, action):
        imager_name = action.text()
        if not imager_name.startswith("aiImager"):
            imager_name = "aiImager"+imager_name.replace(" ", "")
        # check if op exists, otherwise create it
        creatable_imagers = cmds.arnoldPlugins(listImagers=True) or []
        if not cmds.objExists(imager_name) and imager_name in creatable_imagers:
            self.createImager(imager_name)
        else:
            self.addImager(imager_name)

    def removeImagerAction(self):
        self.imagerStack.removeSelectedImager()
        self.showItemProperties(None)

    def buildImagerMenu(self):
        if not self.imagerStack:
            return

        self.imagerMenu.clear()

        imagers = cmds.arnoldPlugins(listImagers=True) or []
        imagers.sort()
        for imager in imagers:
            imagerNodes = cmds.ls(type=imager) or []
            for imagerNode in imagerNodes:
                # don't display the nodes that are already in the stack
                if imagerNode not in self.imagerStack.model().imagers:
                    self.imagerMenu.addAction(imagerNode)

        self.imagerMenu.addSeparator()
        for imager in imagers:
            cmdsLbl = '{}'.format(imager)
            cmdsLbl = re.sub(r'((?<=[a-z])[A-Z]|(?<!\A)[A-Z](?=[a-z]))', r' \1', imager.replace("aiImager", ""))
            self.imagerMenu.addAction(cmdsLbl)


def createImagersWidgetForARV():
    if (cmds.window("ImagersForARV", exists=True)):
        return

    window = cmds.window("ImagersForARV")
    imagerShadersFrame = cmds.frameLayout('arnoldImagersFrame', label='Imagers', borderVisible=False, labelVisible=False)
    currentWidget = toQtObject(imagerShadersFrame, QtWidgets.QWidget)
    imagersUI = ImagersUI(currentWidget, False)
    currentWidget.layout().addWidget(imagersUI)
    return imagerShadersFrame

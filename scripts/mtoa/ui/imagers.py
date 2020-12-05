
from mtoa.ui.ae.templates import createTranslatorMenu
from mtoa.callbacks import *
import mtoa.core as core
import arnold as ai
import maya.cmds as cmds
import mtoa.ui.ae.utils as aeUtils
from mtoa.ui.ae.aiImagersBaseTemplate import getImagerTemplate
# from mtoa.ui.ae.aiImagerLensEffectsTemplate import ImagerLensEffectUI
# from mtoa.ui.ae.aiImagerWhiteBalanceTemplate import ImagerWhiteBalanceUI
# from mtoa.ui.ae.aiImagerColorCorrectTemplate import ImagerColorCorrectUI
# from mtoa.ui.ae.aiImagerTonemapTemplate import ImagerTonemapUI

from mtoa.ui.qt import toQtObject
from mtoa.ui.qt import toMayaName
from mtoa.ui.qt.Qt import QtWidgets, QtCore, QtGui
from mtoa.ui.qt import BaseTreeView, BaseModel, BaseDelegate, \
                       BaseItem, BaseWindow, dpiScale, Timer
from mtoa.ui.qt import MtoAStyle

from mtoa.ui.qt.treeView import *


IMAGER_MIME_TYPE = "application/arnold/imager"


class ImagerStackView(BaseTreeView):
    """docstring for ProceduralTree"""

    itemSelected = QtCore.Signal(str, object)

    MIN_VISIBLE_ENTRIES = 4
    MAX_VISIBLE_ENTRIES = 10
    
    itemSelected = QtCore.Signal(str)

    def __init__(self, transverser = None, parent=None):
        super(ImagerStackView, self).__init__(parent)
        model = ImagerStackModel(self)
        self.baseModel = model
        self.transverser = None
        self.dragStartPosition = None
        self.setModel(model)

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
        self.actionDelete.triggered.connect(lambda arg=None: self.removeImager(pos))
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

    def removeImager(self, pos):
        index = self.indexAt(pos)
        item = index.internalPointer()
        self.model().removeImager(item.name)
        
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
        imagersSize = cmds.getAttr('defaultArnoldRenderOptions.imagers', s=True)
        for i in range(imagersSize):
            imagerShaderElem = 'defaultArnoldRenderOptions.imagers[%d]' % (i)
            elemConnection = cmds.listConnections(imagerShaderElem,p=True, d=False,s=True)

            if elemConnection and len(elemConnection) > 0:
                imager = elemConnection[0].split('.')[0]
                if cmds.objExists(imager) and cmds.attributeQuery('enable', node=imager, exists=True):
                    enableAttr = '{}.enable'.format(imager)
                    enabled = cmds.getAttr(enableAttr)
                    ImagerItem(self.rootItem, imager, enabled)
                    self.imagers.append(imager)
                    if imager not in self.scriptJobList:
                        self.scriptJobList.append(imager)
                        cmds.scriptJob(attributeChange=[enableAttr, lambda *args: self.refresh()])

        self.endResetModel()

    def removeImager(self, imager):
        if not self.imagers or len(self.imagers) == 0:
            return
        imagersLength = len(self.imagers)
        index = self.imagers.index(imager)

        if index < 0 or index >= imagersLength:
            return 
        for i in range(index, imagersLength):
            # first disconnect the existing imagers connected at this index
            imagerElemAttr = 'defaultArnoldRenderOptions.imagers[%d]' % i
            elemConnection = cmds.listConnections(imagerElemAttr,p=True, d=False,s=True)
            if (not elemConnection is None):
                for elem in elemConnection:
                    cmds.disconnectAttr(elem, imagerElemAttr)

            # then, connect the following element to the current one
            if i < imagersLength - 1:
                imagerNextElemAttr = 'defaultArnoldRenderOptions.imagers[%d]' % (i+1)
                nextElemConnection = cmds.listConnections(imagerNextElemAttr,p=True, d=False,s=True)
                if (not nextElemConnection is None) and len(nextElemConnection) > 0:
                    cmds.connectAttr(nextElemConnection[0], imagerElemAttr)

        imagerElemAttr = 'defaultArnoldRenderOptions.imagers[%d]' % (imagersLength - 1)
        cmds.removeMultiInstance(imagerElemAttr , b=True)
        self.refresh()

    def moveImagerUp(self, itemIndex):
        if not self.imagers:
            return
        
        if itemIndex <= 0:
            return
        imagerElemAttr = 'defaultArnoldRenderOptions.imagers[%d]' % (itemIndex)
        elemConnection = cmds.listConnections(imagerElemAttr,p=True, d=False,s=True)
        if elemConnection is None or len(elemConnection) == 0:
            return
        currentImager = elemConnection[0]
        cmds.disconnectAttr(currentImager, imagerElemAttr)

        otherImagerElemAttr = 'defaultArnoldRenderOptions.imagers[%d]' % (itemIndex-1)
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

        imagerElemAttr = 'defaultArnoldRenderOptions.imagers[{}]'.format(itemIndex)
        elemConnection = cmds.listConnections(imagerElemAttr,p=True, d=False,s=True)
        if elemConnection is None or len(elemConnection) == 0:
            return
        currentImager = elemConnection[0]
        cmds.disconnectAttr(currentImager, imagerElemAttr)

        otherImagerElemAttr = 'defaultArnoldRenderOptions.imagers[%d]' % (itemIndex+1)
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

        # get index for given item
        itemIndex = self.imagers.index(item.getNodeName())
        oldElemAttr = 'defaultArnoldRenderOptions.imagers[{}]'.format(itemIndex)
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
        imagerElemAttr = 'defaultArnoldRenderOptions.imagers[{}]'.format(position)
        elemConnection = cmds.listConnections(imagerElemAttr, p=True, d=False, s=True)
        if elemConnection is None or len(elemConnection) == 0:
            return

        # move the nodes below the given position up/down by one index starting
        # at the index before the moved one
        if up:
            idx = itemIndex-1
            while idx >= position:
                imager = self.imagers[idx]
                elemAttr = 'defaultArnoldRenderOptions.imagers[{}]'.format(idx)
                oldElemConnection = cmds.listConnections(elemAttr, p=True, d=False, s=True)
                if oldElemConnection:
                    cmds.disconnectAttr(oldElemConnection[0], elemAttr)
                    targetAttr = 'defaultArnoldRenderOptions.imagers[{}]'.format(idx+1)
                    cmds.connectAttr('{}.message'.format(imager), targetAttr)
                idx -= 1
        else:
            idx = itemIndex+1
            while idx <= position:
                imager = self.imagers[idx]
                elemAttr = 'defaultArnoldRenderOptions.imagers[{}]'.format(idx)
                oldElemConnection = cmds.listConnections(elemAttr, p=True, d=False, s=True)
                if oldElemConnection:
                    cmds.disconnectAttr(oldElemConnection[0], elemAttr)
                    targetAttr = 'defaultArnoldRenderOptions.imagers[{}]'.format(idx-1)
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
        return QtGui.QColor(0, 0, 0)

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

    def __init__(self, parent=None):
        super(ImagersUI, self).__init__(parent)

        style = MtoAStyle.currentStyle()
        self.parent = parent
        self.parentMayaName = toMayaName(parent)

        self.layout = QtWidgets.QVBoxLayout(self)
        self.setLayout(self.layout)

        cmds.setParent(self.parentMayaName)
        rowLayout = cmds.rowLayout('arnoldImagerShaderButtonRow', nc=3, columnWidth3=[140, 100, 100], columnAttach3=['right', 'both', 'both'])
        cmds.text(label='')
        addButton = cmds.button(label='Add Imager')
        impopup = cmds.popupMenu(parent=addButton, button=1)
        cmds.popupMenu(impopup, edit=True, postMenuCommand=Callback(self.buildImagerMenu, impopup))
        cmds.setParent('..') # rowLayout

        cmds.setParent(self.parentMayaName)

        self.rowLayoutQt = toQtObject(rowLayout, QtWidgets.QWidget)
        # self.frame = QtWidgets.QFrame(self.currentWidget)

        self.splitter = QtWidgets.QSplitter(self)
        self.splitter.setOrientation(QtCore.Qt.Vertical)
        self.splitter.setObjectName("splitter")

        self.imagerStack = ImagerStackView(None, self.splitter)
        self.imagerStack.setObjectName("ImagerStackWidget")
        # self.imagerStack.setMinimumHeight(dpiScale(300))
        # self.frame.layout().addWidget(self.imagerStack)

        # self.attributesFrame = QtWidgets.QFrame(self.splitter)
        # self.attributesFrame.setLayout(QtWidgets.QVBoxLayout(self.attributesFrame))
        self.attributeScrollArea = QtWidgets.QScrollArea(self.splitter)
        self.attributeScrollArea.setObjectName("AttributeScrollArea")
        self.attributeScrollArea.setWidgetResizable(True)
        self.attributeScrollArea.setMinimumHeight(dpiScale(200))

        self.scrollAreaWidgetContents = QtWidgets.QWidget()
        # self.scrollAreaWidgetContents.setGeometry(QtCore.QRect(0, 0, 816, 424))
        self.scrollAreaWidgetContents.setObjectName("scrollAreaWidgetContents")
        self.scrollAreaLayout = QtWidgets.QVBoxLayout(self.scrollAreaWidgetContents)

        self.attributeScrollArea.setWidget(self.scrollAreaWidgetContents)

        # self.frame.layout().addWidget(self.attributesFrame)
        self.layout.addWidget(self.splitter)

        self.imagerAttributesFrame = None

        self.imagerStack.itemSelected.connect(self.showItemProperties)

        # every time the attribute imagers in the options node is modified, we want to update the widget
        cmds.scriptJob(parent=self.parentMayaName, attributeChange=['defaultArnoldRenderOptions.imagers', self.updateImagers], dri=True, alc=True, per=True )
        self.updateImagers()
        cmds.setParent('..')

    @QtCore.Slot(str)
    def showItemProperties(self, node):

        if (self.imagerAttributesFrame):
            cmds.deleteUI(self.imagerAttributesFrame)
            self.imagerAttributesFrame = None

        cmds.setParent(self.parentMayaName)
        self.imagerAttributesFrame = cmds.rowColumnLayout('ImagersAttributeFrame', numberOfColumns=1)

        imagersUITemplate = getImagerTemplate(cmds.nodeType(node))

        if imagersUITemplate:
            imagersUITemplate(parent=self.imagerAttributesFrame, nodeName=node)

        self.scrollAreaLayout.addWidget(toQtObject(self.imagerAttributesFrame, QtWidgets.QWidget))

        cmds.setParent('..')

    def updateImagers(self):
        self.imagerStack.model().refresh()

    def createImager(self, nodeType):
        imager = cmds.createNode(nodeType)
        self.addImager(imager)

    def addImager(self, node):
        imagersSize = cmds.getAttr('defaultArnoldRenderOptions.imagers', s=True)
        attrName = 'defaultArnoldRenderOptions.imagers[%d]' % imagersSize
        cmds.connectAttr("%s.message"%node, attrName, force=True)
        self.updateImagers()
        cmds.select(node, r=True)

    def buildImagerMenu(self, popup):
        if not self.imagerStack:
            return

        cmds.popupMenu(popup, edit=True, deleteAllItems=True)
        imagers = cmds.arnoldPlugins(listImagers=True) or []

        for imager in imagers:
            imagerNodes = cmds.ls(type=imager) or []
            for imagerNode in imagerNodes:
                # don't display the nodes that are already in the stack
                if not imagerNode in self.imagerStack.model().imagers:
                    cmds.menuItem(parent=popup, label=imagerNode, command=Callback(self.addImager, imagerNode))

        cmds.menuItem(parent=popup, divider=True)
        for imager in imagers:
            cmdsLbl = 'Create {}'.format(imager)
            cmds.menuItem(parent=popup, label=cmdsLbl,  command=Callback(self.createImager, imager))


def createImagersWidgetForARV():
    if (cmds.window("ImagersForARV", exists=True)):
        return

    window = cmds.window("ImagersForARV")
    imagerShadersFrame = cmds.frameLayout('arnoldImagersFrame', label='Imagers', borderVisible=False, labelVisible=False)
    currentWidget = toQtObject(imagerShadersFrame, QtWidgets.QWidget)

    imagersUI = ImagersUI(currentWidget)

    currentWidget.layout().addWidget(imagersUI)

    # imagersUI_maya = toMayaName(imagersUI)

    return imagerShadersFrame

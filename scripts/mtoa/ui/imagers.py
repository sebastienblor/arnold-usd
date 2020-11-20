
from mtoa.ui.ae.templates import createTranslatorMenu
from mtoa.callbacks import *
import mtoa.core as core
import arnold as ai
import maya.cmds as cmds
import mtoa.ui.ae.utils as aeUtils
from mtoa.ui.ae.aiImagerExposureTemplate import ImagerExposureUI
from mtoa.ui.ae.aiImagerLensEffectsTemplate import ImagerLensEffectUI
from mtoa.ui.ae.aiImagerWhiteBalanceTemplate import ImagerWhiteBalanceUI
from mtoa.ui.ae.aiImagerColorCorrectTemplate import ImagerColorCorrectUI
from mtoa.ui.ae.aiImagerTonemapTemplate import ImagerTonemapUI

from mtoa.ui.qt import toQtObject
from mtoa.ui.qt import toMayaName
from mtoa.ui.qt.Qt import QtWidgets, QtCore, QtGui
from mtoa.ui.qt import BaseTreeView, BaseModel, BaseDelegate, \
                       BaseItem, BaseWindow, dpiScale, Timer
from mtoa.ui.qt import MtoAStyle

from mtoa.ui.qt.treeView import *

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
        self.setModel(model)
        self.setDragEnabled(True)
        self.setDropIndicatorShown(True)
        self.setDragDropMode(QtWidgets.QAbstractItemView.InternalMove)
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

    # # Every time we select an imager element, we want to 
    # # select it in Maya
    def mousePressEvent(self, event):
        super(ImagerStackView, self).mousePressEvent(event)
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
       


class ImagersUI(object):

    def __init__(self, parent=None):
        
        style = MtoAStyle.currentStyle()
        self.parent = parent
        # every time the attribute imagers in the options node is modified, we want to update the widget
        cmds.scriptJob(parent=parent, attributeChange=['defaultArnoldRenderOptions.imagers', self.updateImagers], dri=True, alc=True, per=True )
        self.imagerStack = None
        cmds.rowLayout('arnoldImagerShaderButtonRow', nc=3, columnWidth3=[140, 100, 100], columnAttach3=['right', 'both', 'left'])
        cmds.text(label='')
        addButton =  cmds.button(label='Add Imager')
        impopup = cmds.popupMenu(parent=addButton, button=1)
        cmds.popupMenu(impopup, edit=True, postMenuCommand=Callback(self.buildImagerMenu, impopup))
        removeButton =  cmds.button(label='Remove Imager')
        cmds.setParent('..') # rowLayout

        cmds.setParent(parent)

        self.currentWidget = toQtObject(cmds.setParent(query=True), QtWidgets.QWidget)
        self.frame = QtWidgets.QFrame(self.currentWidget)
        self.frame.setLayout(QtWidgets.QVBoxLayout(self.frame))
        self.currentWidget.layout().addWidget(self.frame)
        self.imagerStack = ImagerStackView(None, self.currentWidget)
        self.imagerStack.setObjectName("ImagerStackWidget")
        self.frame.layout().addWidget(self.imagerStack)
        self.imagerStack.itemSelected.connect(self.showItemProperties)
        self.attributesFrame = QtWidgets.QFrame(self.currentWidget)
        self.attributesFrame.setLayout(QtWidgets.QVBoxLayout(self.attributesFrame))
        self.currentWidget.layout().addWidget(self.attributesFrame)
        self.imagerAttributesFrame = None
        self.imagerAttributesParentFrame = None
        
        self.updateImagers()
        cmds.setParent('..')

    @QtCore.Slot(str)
    def showItemProperties(self, node):
        
        if (self.imagerAttributesParentFrame):
            cmds.deleteUI(self.imagerAttributesParentFrame)
            self.imagerAttributesParentFrame = None

        cmds.setParent(self.parent)
        self.imagerAttributesParentFrame = cmds.frameLayout(label = "Attributes", collapsable=False, labelVisible=True, borderVisible=False)
        self.imagerAttributesFrame = cmds.rowColumnLayout('ImagersAttributeFrame', numberOfColumns=1)

        if cmds.nodeType(node) == "aiImagerExposure":
            exposure = ImagerExposureUI(parent = self.imagerAttributesFrame,nodeName = node)
        elif cmds.nodeType(node) == "aiImagerLensEffects":
            lensEffect = ImagerLensEffectUI(parent = self.imagerAttributesFrame,nodeName = node)
        elif cmds.nodeType(node) == "aiImagerWhiteBalance":
            whiteBalance = ImagerWhiteBalanceUI(parent = self.imagerAttributesFrame,nodeName = node)
        elif cmds.nodeType(node) == "aiImagerColorCorrect":
            colorCorrect = ImagerColorCorrectUI(parent = self.imagerAttributesFrame,nodeName = node)
        elif cmds.nodeType(node) == "aiImagerTonemap":
            toneMap = ImagerTonemapUI(parent = self.imagerAttributesFrame,nodeName = node)
        
        cmds.setParent('..')
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



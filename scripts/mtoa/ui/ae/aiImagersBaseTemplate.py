import maya.mel
from mtoa.ui.qt import toQtObject
from mtoa.ui.qt import toMayaName
import maya.cmds as cmds
from mtoa.ui.qt.Qt import QtWidgets, QtCore, QtGui
from mtoa.ui.ae.templates import AEChildMode
from mtoa.ui.ae.utils import AttrControlGrp
from mtoa.utils import prettify


class ImagerBaseUI(object):
    def __init__(self, parent=None, nodeName = None):
        self.parentLayout = cmds.rowColumnLayout( numberOfColumns=3, columnWidth=[(1, 100), (2, 100) , (3, 100) ])
        self._controls = []
        self._layoutStack = [parent]
        self._nodeName = nodeName
        self._attr = None

        cmds.text(label = "")
        cmds.text(label = "Enable")
        checkbox = cmds.checkBox( label='', parent = self.parentLayout )
        cmds.connectControl( checkbox, '%s.enable' % nodeName )
        # self._controls.append((checkbox))
        cmds.text(label = "")
        cmds.text(label = "Layer Selection")
        textField = cmds.textField()
        cmds.connectControl( textField, '%s.layerSelection' % nodeName )
        # self._controls.append(textField)
        cmds.setParent('..')

    # queries
    @property
    def nodeName(self):
        "get the active node"
        # assert self._nodeName, "%r: nodeName should be set by now" % self
        return self._nodeName

    @property
    def attr(self):
        return self._attr

    def nodeAttr(self, attr):
        return self.nodeName + '.' + attr

    def getParentLayout(self):
        return self.parentLayout

    def addControl(self, attr, label=None, changeCommand=None, annotation=None,
                   preventOverride=False, dynamic=False, enumeratedItem=None):

        if not label:
            label = prettify(attr)
            if label.startswith('Ai '):
                label = label[3:]
        kwargs = {}
        kwargs['label'] = label
        kwargs['attribute'] = self.nodeAttr(attr)
        if annotation:
            kwargs['annotation'] = annotation
        if changeCommand:
            kwargs['changeCommand'] = changeCommand
        if enumeratedItem:
            kwargs['enumeratedItem'] = enumeratedItem
        parent = self._layoutStack[-1]
        cmds.setParent(parent)
        control = AttrControlGrp(**kwargs)
        self._controls.append((attr, control.setAttribute, parent))

    def addCustom(self, attr, createFunc, updateFunc):
        parent = self._layoutStack[-1]
        cmds.setParent(parent)
        col = cmds.columnLayout(adj=True)
        createFunc(self.nodeAttr(attr))
        cmds.setParent(parent)
        self._controls.append((attr, updateFunc, col))

    def addSeparator(self):
        cmds.separator()

    def beginLayout(self, label, **kwargs):
        '''
        begin a frameLayout.
        accepts any keyword args valid for creating a frameLayout
        '''
        kwargs['label'] = label
        cmds.setParent(self._layoutStack[-1])
        cmds.frameLayout(**kwargs)
        self._layoutStack.append(cmds.columnLayout(adjustableColumn=True))

    def endLayout(self):
        '''
        end the current frameLayout
        '''
        self._layoutStack.pop()
        cmds.setParent(self._layoutStack[-1])

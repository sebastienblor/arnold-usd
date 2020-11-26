import maya.mel
from mtoa.ui.qt import toQtObject
from mtoa.ui.qt import toMayaName
import maya.cmds as cmds
from mtoa.ui.qt.Qt import QtWidgets, QtCore, QtGui
from mtoa.ui.ae.templates import AEChildMode
from mtoa.ui.ae.utils import AttrControlGrp, attrType
from mtoa.utils import prettify


class ImagerBoolCtl(object):
    """docstring for ImagerBoolCtl"""
    def __init__(self, attribute, parent=None, label=None, changeCommand=None, annotation=None,
                 preventOverride=False, dynamic=False, enumeratedItem=None):
        super(ImagerBoolCtl, self).__init__()
        self._attr = None
        self.checkbox = None
        self.parent = parent

        cmds.setParent(parent)
        self.parentLayout = cmds.rowColumnLayout( numberOfColumns=3, columnWidth=[(1, 100), (2, 100) , (3, 100) ])
        cmds.text(label="")
        cmds.text(label=label if label else "Enable")
        cmds.setParent('..')
        self.setAttribute(attribute)

    def setAttribute(self, nodeAttr):
        if nodeAttr == self._attr:
            return
        self._attr = nodeAttr
        if self.checkbox and cmds.control(self.checkbox, q=True, exists=True):
            cmds.deleteUI(self.checkbox)
        cmds.setParent(self.parentLayout)
        self.checkbox = cmds.checkBox(label='', parent=self.parentLayout)
        cmds.setParent('..')
        cmds.connectControl(self.checkbox, self._attr)

    def edit(self, **kwargs):
        cmds.checkBox(self.checkbox, edit=True, **kwargs)


class ImagerBaseUI(object):
    def __init__(self, parent=None, nodeName=None, template=None):
        self._controls = []
        self._layoutStack = [parent]
        self._nodeName = nodeName
        self._attr = None
        self._template = template

        self.setup()

    # queries
    @property
    def nodeName(self):
        "get the active node"
        # assert self._nodeName, "%r: nodeName should be set by now" % self
        return self._nodeName

    @property
    def attr(self):
        return self._attr

    def setup(self):

        # cmds.text(label = "")
        # cmds.text(label = "Enable")
        # checkbox = cmds.checkBox( label='', parent = self.parentLayout )
        # cmds.connectControl( checkbox, '%s.enable' % nodeName )
        # # self._controls.append((checkbox))
        # cmds.text(label = "")
        # cmds.text(label = "Layer Selection")
        # textField = cmds.textField()
        # cmds.connectControl( textField, '%s.layerSelection' % nodeName )
        # # self._controls.append(textField)
        # cmds.setParent('..')
        self.addControl("enable", label="Enable")
        if cmds.attributeQuery('layerSelection', n=self._nodeName, exists=True):
            self.addControl("layerSelection", label="Layer Selection")

    def nodeAttr(self, attr):
        return self.nodeName + '.' + attr

    def getParentLayout(self):
        return self.parentLayout

    def addControl(self, attr, label=None, changeCommand=None, annotation=None,
                   preventOverride=False, dynamic=False, enumeratedItem=None):

        control = None

        if not label:
            label = prettify(attr)
            if label.startswith('Ai '):
                label = label[3:]
        kwargs = {}
        kwargs['label'] = label
        if annotation:
            kwargs['annotation'] = annotation
        if changeCommand:
            kwargs['changeCommand'] = changeCommand
        if enumeratedItem:
            kwargs['enumeratedItem'] = enumeratedItem
        if preventOverride:
            kwargs['preventOverride'] = preventOverride
        if dynamic:
            kwargs['dynamic'] = dynamic

        if self._template:
            self._template.addControl(attr, **kwargs)
        else:
            kwargs['attribute'] = self.nodeAttr(attr)
            parent = self._layoutStack[-1]
            cmds.setParent(parent)
            control = None
            if attrType(self.nodeAttr(attr)) == 'bool':
                kwargs['parent'] = parent
                control = ImagerBoolCtl(**kwargs)
            else:
                control = AttrControlGrp(**kwargs)
            if control:
                self._controls.append((attr, control, control.setAttribute, parent))

        return control

    def addCustom(self, attr, createFunc, updateFunc):
        if self._template:
            self._template.addCustom(attr, createFunc, updateFunc)
        else:
            parent = self._layoutStack[-1]
            cmds.setParent(parent)
            col = cmds.columnLayout(adj=True)
            control = createFunc(self.nodeAttr(attr))
            cmds.setParent(parent)
            self._controls.append((attr, control, updateFunc, col))

    def addSeparator(self):

        if self._template:
            self._template.addSeparator()
        else:
            cmds.separator()

    def beginLayout(self, label, **kwargs):
        '''
        begin a frameLayout.
        accepts any keyword args valid for creating a frameLayout
        '''
        if self._template:
            self._template.beginLayout(label, **kwargs)
        else:
            kwargs['label'] = label
            cmds.setParent(self._layoutStack[-1])
            cmds.frameLayout(**kwargs)
            self._layoutStack.append(cmds.columnLayout(adjustableColumn=True))

    def endLayout(self):
        '''
        end the current frameLayout
        '''
        if self._template:
            self._template.endLayout()
        else:
            self._layoutStack.pop()
            cmds.setParent(self._layoutStack[-1])

    def dimControl(self, attr, state=True):

        if self._template:
            self._template.dimControl(attr, state)
        else:
            control = self.getControl(attr)
            if control:
                control.edit(enable=not state)

    def getControl(self, attr):
        for ctrl in self._controls:
            if ctrl[0] == attr:
                return ctrl[1]

        return None

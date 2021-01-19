import maya.mel
import inspect
from mtoa.ui.qt import toQtObject
from mtoa.ui.qt import toMayaName
import maya.cmds as cmds
from mtoa.ui.qt.Qt import QtWidgets, QtCore, QtGui
from mtoa.ui.ae.templates import AEChildMode
from mtoa.ui.ae.utils import AttrControlGrp, attrType
from mtoa.utils import prettify
import mtoa.aovs as aovs

global _imagerTemplates
_imagerTemplates = {}


class ImagerBoolCtl(object):
    """docstring for ImagerBoolCtl"""
    def __init__(self, attribute, parent=None, label=None, changeCommand=None, annotation=None,
                 preventOverride=False, dynamic=False, enumeratedItem=None):
        super(ImagerBoolCtl, self).__init__()
        self._attr = None
        self.checkbox = None
        self.parent = parent
        self.label = label

        cmds.setParent(parent)
        self.parentLayout = cmds.rowLayout("BoolLayout#",
                                           numberOfColumns=2,
                                           adjustableColumn=2,
                                           columnWidth=[1, 140])
        cmds.text("")
        cmds.setParent('..')
        self.setAttribute(attribute)

    def setAttribute(self, nodeAttr):
        if nodeAttr == self._attr:
            return
        self._attr = nodeAttr
        if self.checkbox and cmds.control(self.checkbox, q=True, exists=True):
            cmds.deleteUI(self.checkbox)
        cmds.setParent(self.parentLayout)
        self.checkbox = cmds.checkBox(label=self.label, parent=self.parentLayout)
        cmds.setParent('..')
        cmds.connectControl(self.checkbox, self._attr)

    def edit(self, **kwargs):
        cmds.checkBox(self.checkbox, edit=True, **kwargs)

class LayersList(QtWidgets.QDialog):
    def __init__(self, parent=None, displayList = None  ):
        super(LayersList, self).__init__(toQtObject(parent))
        buttons = QtWidgets.QDialogButtonBox.Ok | QtWidgets.QDialogButtonBox.Cancel
        self.buttonBox = QtWidgets.QDialogButtonBox(buttons)
        self.buttonBox.accepted.connect(self.accept)
        self.buttonBox.rejected.connect(self.reject)

        self.mainLayout = QtWidgets.QVBoxLayout(self)
        self.setWindowTitle("Light Group Layer(s)")
        self.list = QtWidgets.QListWidget(self)
        self.list.setSelectionMode(QtWidgets.QAbstractItemView.ExtendedSelection)
        for item in displayList:
            self.list.addItem(item)
        self.setLayout(self.mainLayout)
        self.mainLayout.addWidget(self.list)
        self.mainLayout.addWidget(self.buttonBox)

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

    def layerSelectionChanged(self, nodeName):
        full_path_control = cmds.control('layerSel',query = True, fullPathName = True)
        layer_selection_text = cmds.textFieldButtonGrp(full_path_control, query = True , text = True)
        if layer_selection_text != cmds.getAttr(nodeName):
            cmds.setAttr(nodeName, layer_selection_text,type="string")
        
    
    def layerSelectionButtonClicked(self, nodeName):
        selected_items = []
        scene_aovs = aovs.getAOVs()
        aov_list = []
        for item in scene_aovs:
            aov_list.append(item.name)
        aov_list += ['RGBA','Custom']
        popup = LayersList(self, aov_list)
        if popup.exec_():
            for item in popup.list.selectedItems():
                selected_items.append(item.text())
        else:
            return
        if 'Custom' in selected_items:
            layer_selection_string = " "
        else:
            layer_selection_string = ' or '.join(selected_items)
        full_path_control = cmds.control('layerSel',query = True, fullPathName = True)
        cmds.textFieldButtonGrp(full_path_control, edit = True , text = layer_selection_string)


    def addLayerSelection(self, nodeName):
        cmds.columnLayout()
        layer_selection_text = cmds.getAttr(nodeName)
        self.layerSelectionControl = cmds.textFieldButtonGrp( 'layerSel',label='Layer Selection',
                                                              text=layer_selection_text,
                                                              buttonLabel='Select AOVs',
                                                              buttonCommand = lambda *args: self.layerSelectionButtonClicked(nodeName, *args),
                                                              textChangedCommand = lambda *args: self.layerSelectionChanged(nodeName),
                                                              columnWidth3=[150, 75, 100],
                                                              ann = "The Imager will apply to the AOV's in this list. The field accepts an Arnold Selection Expression"
                                                            )
        cmds.setParent('..')

    def updateLayerSelection(self, nodeName):

        full_path_control = cmds.control('layerSel',query = True, fullPathName = True)
        cmds.textFieldButtonGrp(full_path_control, edit = True,
                                buttonCommand = lambda *args: self.layerSelectionButtonClicked(nodeName, *args),
                                textChangedCommand = lambda *args: self.layerSelectionChanged(nodeName)
                                )
        layer_selection_text = cmds.getAttr(nodeName)
        if layer_selection_text != cmds.textFieldButtonGrp(full_path_control, query = True , text = True):
            cmds.textFieldButtonGrp(full_path_control, edit = True , text = layer_selection_text)

    def setup(self):
        self.beginLayout("Common", collapse=False)

        self.addControl("enable", label="Enable")
        if cmds.attributeQuery('layerSelection', n=self._nodeName, exists=True):
            self.addCustom('layerSelection', self.addLayerSelection, self.updateLayerSelection)

        self.endLayout()

    def nodeAttr(self, attr):
        return self.nodeName + '.' + attr

    def getParentLayout(self):
        return self.parentLayout

    def addControl(self, attr, label=None, changeCommand=None, annotation=None,
                   preventOverride=False, dynamic=False, enumeratedItem=None, hideMapButton=None):

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
        if hideMapButton:
            kwargs['hideMapButton'] = hideMapButton

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
            kwargs['collapsable'] = True
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


def registerImagerTemplate(nodeType, templateClass, *args, **kwargs):
    """
    Register an `ImagerBaseUI` class to be used with the given nodeType.
    """
    assert inspect.isclass(templateClass) and issubclass(templateClass, ImagerBaseUI), \
        "you must pass a subclass of ImagerBaseUI"
    global _imagerTemplates
    if nodeType not in _imagerTemplates:
        try:
            _imagerTemplates[nodeType] = templateClass
        except:
            arnold.AiMsgError("Failed to instantiate Imager UI Template %s" % templateClass)
            import traceback
            traceback.print_exc()


def getImagerTemplate(nodeType):
    """
    Return an `AttributeTemplate` instance for the given nodeType or None if one has not been registered.
    
    This is the root template for the node type. Unlike translator UIs, there can be only one template per node type.
    """
    global _imagerTemplates
    try:
        # has one been explicitly registered?
        templateClass = _imagerTemplates[nodeType]
    except KeyError:
        return
    else:
        return templateClass
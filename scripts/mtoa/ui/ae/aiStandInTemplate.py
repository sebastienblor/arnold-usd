import re
import maya.cmds as cmds
import maya.mel as mel
import os.path
import mtoa.melUtils as mu
from mtoa.ui.ae.utils import aeCallback
from mtoa.ui.ae.utils import resolveFilePathSequence
from mtoa.ui.ae.utils import expandEnvVars
import mtoa.core as core
from mtoa.ui.ae.shaderTemplate import ShaderAETemplate
import arnold as ai

from mtoa.ui.qt.Qt import QtCore
from mtoa.ui.qt.Qt import QtGui
from mtoa.ui.qt.Qt import QtWidgets
from mtoa.ui.qt.Qt import shiboken
from mtoa.ui.qt import toQtObject

CACHE_ATTR = 'ai_asscache'

from mtoa.ui.procview.ProceduralTreeView import ProceduralTreeView, ProceduralTreeModel, ProceduralItem
from mtoa.ui.procview.ProceduralWidgets import ProceduralPropertiesPanel
from mtoa.ui.procview.StandInTransverser import StandInTransverser
from mtoa.ui.procview.AlembicTransverser import AlembicTransverser
from mtoa.ui.procview.CustomProceduralTransverser import CustomProceduralTransverser
from mtoa.ui.procview.ProceduralTransverser import VARIANTSWITCH_OP, SWITCH_OP, MERGE_OP

from mtoa.callbacks import *

from functools import partial

################################################


def LoadStandInButtonPush(attrName):
    basicFilter = 'Arnold Archive (*.ass *.ass.gz *.obj *.ply *.abc *.usd)'
    defaultDir = cmds.workspace(query=True, directory=True)
    currentDir = cmds.getAttr(attrName) or ''
    currentDir = os.path.dirname(currentDir)
    if os.path.exists(currentDir):
        defaultDir = currentDir

    ret = cmds.fileDialog2(fileFilter=basicFilter, cap='Load StandIn',okc='Load',fm=1, startingDirectory=defaultDir)
    if ret is not None and len(ret):
        cmds.setAttr(attrName.replace('.dso', '.useFrameExtension'), False) # I picked one file, no file sequence
        ArnoldStandInDsoEdit(attrName, ret[0])


def ArnoldStandInDsoEdit(attrName, mPath) :
    cmds.setAttr(attrName, mPath, type='string')


def ArnoldStandInTemplateDsoNew(attrName) :
    cmds.rowColumnLayout( numberOfColumns=3, columnAlign=[(1, 'right'),(2, 'right'),(3, 'left')], columnAttach=[(1, 'right', 0), (2, 'both', 0), (3, 'left', 5)], columnWidth=[(1,145),(2,220),(3,30)] )
    cmds.text(label='Path ')
    path = cmds.textField('standInDsoPath',changeCommand=lambda *args: ArnoldStandInDsoEdit(attrName, *args))
    cmds.textField( path, edit=True, text=cmds.getAttr(attrName) )
    cmds.symbolButton('standInDsoPathButton', image='navButtonBrowse.png', command=lambda arg=None, x=attrName: LoadStandInButtonPush(x))


def ArnoldStandInTemplateDsoReplace(attrName) :
    cmds.textField( 'standInDsoPath', edit=True, changeCommand=lambda *args: ArnoldStandInDsoEdit(attrName, *args))
    cmds.textField( 'standInDsoPath', edit=True, text=cmds.getAttr(attrName) )
    cmds.symbolButton('standInDsoPathButton', edit=True, image='navButtonBrowse.png' , command=lambda arg=None, x=attrName: LoadStandInButtonPush(x))


def ArnoldStandInUpdateUI(attrName) :
    overrideVisAttrs = ['overridePrimaryVisibility', 
                     'overrideCastsShadows', 
                     'overrideVisibleInDiffuseReflection',
                     'overrideVisibleInSpecularReflection',
                     'overrideVisibleInDiffuseTransmission', 
                     'overrideVisibleInSpecularTransmission',
                     'overrideVisibleInVolume']
    visAttrs =      ['primaryVisibility', 
                     'castsShadows', 
                     'aiVisibleInDiffuseReflection',
                     'aiVisibleInSpecularReflection',
                     'aiVisibleInDiffuseTransmission', 
                     'aiVisibleInSpecularTransmission',
                     'aiVisibleInVolume']

    for i in range(len(overrideVisAttrs)):
        if cmds.getAttr(attrName + overrideVisAttrs[i]) == 0:
            cmds.setAttr(attrName + overrideVisAttrs[i], 1)
            if cmds.getAttr(attrName + visAttrs[i]) == 0:
                cmds.setAttr(attrName + visAttrs[i], 1)


def changeOperator(node, nodeAttr):
    attrSize = mu.getAttrNumElements(*nodeAttr.split('.', 1))
    newItem = '{}[{}]'.format(nodeAttr, attrSize)
    cmds.connectAttr("%s.message"%node, newItem, force=True)


def createOperator(opType, nodeAttr):
    opNode = cmds.createNode(opType)
    changeOperator(opNode, nodeAttr)


def buildOperatorMenu(popup, attrName):
    nodeName = attrName.split('.')[0]
    cmds.popupMenu(popup, edit=True, deleteAllItems=True)
    operators = cmds.arnoldPlugins(listOperators=True) or []
    for operator in operators:
        opNodes = cmds.ls(type=operator) or []
        for opNode in opNodes:
            if opNode == nodeName:
                continue
            cmds.menuItem(parent=popup, label=opNode, command=Callback(changeOperator, opNode, attrName))

    cmds.menuItem(parent=popup, divider=True)
    for operator in operators:
        cmdsLbl = 'Create {}'.format(operator)
        cmds.menuItem(parent=popup, label=cmdsLbl, command=Callback(createOperator, operator, attrName))


def editLabelCmd(str1, str2):
    return ''


class VariantDialog(object):

    def __init__(self, title="New Variant", edit=False, variantName=None):

        self.title = title
        self.edit = edit
        self.variant = "newPass"
        if variantName:
            self.variant = variantName
        self.duplicateCurrent = False
        self._nameWidget = None
        self._duplicateCurrentWidget = None

    def show(self):

        return cmds.layoutDialog(ui=self.buildUI, title=self.title)

    def buildUI(self):

        form = cmds.setParent(q=True)

        # layoutDialog's are not resizable, so hard code a size here,
        # to make sure all UI elements are visible.
        #
        cmds.formLayout(form, e=True, width=300)

        self._nameWidget = cmds.textFieldGrp(
            ad2=2, label="Variant Name",
            text=self.variant)

        self._duplicateCurrentWidget = cmds.checkBox(label='Duplicate Current Variant')
        if self.edit:
            cmds.checkBox(self._duplicateCurrentWidget, e=True, visible=False)

        confirmBtn = cmds.button(l='OK', c=partial(self.onDismiss, msg='ok'))
        cancelBtn = cmds.button(l='Cancel', c=partial(self.onDismiss, msg='cancel'))

        spacer = 5
        top = 5
        edge = 5

        cmds.formLayout(form, edit=True,
                        attachForm=[(self._nameWidget, 'top', top),
                                    (self._nameWidget, 'left', 0),
                                    (self._duplicateCurrentWidget, 'left', 0),
                                    (self._duplicateCurrentWidget, 'right', edge),
                                    (confirmBtn, 'bottom', spacer),
                                    (cancelBtn, 'bottom', spacer),
                                    (cancelBtn, 'right', edge)],
                        attachControl=[(self._duplicateCurrentWidget, 'top',   spacer, self._nameWidget),
                                       (confirmBtn,                   'top',   spacer, self._duplicateCurrentWidget),
                                       (cancelBtn,                    'top',   spacer, self._duplicateCurrentWidget),
                                       (confirmBtn,                   'right', spacer, cancelBtn)])

    def onDismiss(self, data, msg):

        self.variant = cmds.textFieldGrp(self._nameWidget, query=True, text=True)
        self.duplicateCurrent = cmds.checkBox(self._duplicateCurrentWidget, query=True, value=True)

        cmds.layoutDialog(dismiss=msg)


class AEaiStandInTemplate(ShaderAETemplate):

    def __currentWidget(self, pySideType=QtWidgets.QWidget):
        """Cast and return the current widget."""
        # Get the current widget Maya name.
        currentWidgetName = cmds.setParent(query=True)
        return toQtObject(currentWidgetName, pySideType)

    def updateSelectedItems(self):
        if not self.tree or not self.tree.transverser:
            return
        selection = cmds.getAttr('{}.{}'.format(self.nodeName, 'selected_items'))
        
        if selection == self.tree.transverser.selectionStr:
            return
        #self.tree.transverser.selectionStr = selection
        selectionSplit = selection.split(',')
        for selected in selectionSplit:
            if selected:
                # Prevent firing signals in Qt to avoid infinite loop.
                #oldState = self.tree.blockSignals(True)
                self.tree.select(selected)
                #self.tree.blockSignals(oldState)
                return
                
    def updateAssFile(self):
        self.assItems = []
        self.fileInfoReplace(self.nodeName + ".dso")
        # FIXME for now let's not rely on this cache attribute
        #cmds.setAttr('{}.{}'.format(self.nodeName, CACHE_ATTR), 0, type="stringArray")


    '''
    def selectElement(self, itemName, itemValue):
        if itemValue == 0:
            cmds.button(self.overrideSelectionButton, edit=True, enable=False)
            cmds.button(self.selectOperatorButton, edit=True, enable=False)
            if itemName in self.selectedItems:
                self.selectedItems.remove(itemName)
        else:
            cmds.button(self.overrideSelectionButton, edit=True, enable=True)
            cmds.button(self.selectOperatorButton, edit=True, enable=False) # FIXME need to select the corresponding operator
            self.selectedItems.append(itemName)

        attrVal = ''
        for sel in self.selectedItems:
            if len(attrVal) > 0:
                attrVal = ',' + attrVal
            attrVal = attrVal + sel

        selAttr = self.nodeName + ".selectedItems"
        cmds.setAttr(selAttr, attrVal, type='string')
        return True
    '''
    def refreshAssignmentsUI(self):
        fileAttr = '{}.dso'.format(self.nodeName)
        filename = cmds.getAttr(fileAttr)
        filename = expandEnvVars(filename)

        ext_str = ".ass"
        if filename:
            ext_str = os.path.splitext(filename)[1].lower()

        expand = False
        if ext_str == '.abc':
            expand = True

        self.tree.setCurrentNode(self.nodeName, expand, force=True)
        self.properties_panel.setNode(self.nodeName)

    def variantReplace(self, nodeAttr):
        old_variant = self.variant_node
        old_node = self.current_node
        # check if attribute need updating
        if not cmds.attributeQuery("variant", node=self.nodeName, exists=True):
            cmds.addAttr(self.nodeName, longName="variant", at="enum", enumName="default" )

        self.variant_node = self.getVariantSwitchNode()
        self.current_node = self.nodeName

        replace_variant = old_variant != self.variant_node
        replace_node = old_node != self.current_node
        update = self.updateVariantAttr()
        if update or replace_variant or replace_node or self._update_var_ui:

            if self.variantCtrl:
                cmds.deleteUI(self.variantCtrl)

            self.variantCtrl = cmds.attrEnumOptionMenu(label="Look Variant",
                                                       attribute="{}.variant".format(self.nodeName),
                                                       parent=self.variantRowLayout,
                                                       changeCommand=self.setVariant)

            self._update_var_ui = False
            if self.variant_node:
                variant_idx = cmds.getAttr("{}.index".format(self.variant_node))
                cmds.setAttr("{}.variant".format(self.nodeName), variant_idx)

            self.refreshAssignmentsUI()

    def variantNew(self, nodeAttr):
        self.current_node = self.nodeName
        # get if theres an enum attribute
        if not cmds.attributeQuery("variant", node=self.nodeName, exists=True):
            cmds.addAttr(self.nodeName, longName="variant", at="enum", enumName="default" )

        self.variant_node = self.getVariantSwitchNode()

        self.variantRowLayout = cmds.rowColumnLayout(numberOfColumns=5, adjustableColumn=5,
                                         columnAlign=[(1, 'left'), (2, 'left'), (3, 'left'), (4, 'left')],
                                         columnAttach=[(1, 'left', 10), (2, 'left', 1), (3, 'left', 1), (4, 'left', 1)])

        self.newVariantCtrl = cmds.symbolButton('standInNewVarientButton', image='newRenderPass.png', command=self.newVariant )
        self.editVariantCtrl = cmds.symbolButton('standInEditVarientButton', image='editRenderPass.png', command=self.editVariant )
        self.removeVariantCtrl = cmds.symbolButton('standInRemoveVarientButton', image='deleteRenderPass.png', command=self.removeVariant )

        self.variantCtrl = cmds.attrEnumOptionMenu(label="Look Variant",
                                                   attribute="{}.variant".format(self.nodeName),
                                                   changeCommand=self.setVariant)

        cmds.text("")
        cmds.setParent('..')

    def fileInfoReplace(self, nodeAttr):
        nodeName = nodeAttr.split('.')[0]
        fileAttr = '{}.dso'.format(nodeName)
        filename = cmds.getAttr(fileAttr)
        filename = expandEnvVars(filename)
        if nodeName == self.currentNode and filename == self.currentFilename:
            self.properties_panel.setItem(self.nodeName, None)
            return  # nothing to do here...

        filename_changed = False
        if nodeName == self.currentNode and filename != self.currentFilename:
            filename_changed = True

        self.currentNode = nodeName
        self.currentFilename = filename

        ext_str = ".ass"
        if filename:
            ext_str = os.path.splitext(filename)[1].lower()

        expand = False
        if ext_str == '.abc':
            transverser = AlembicTransverser()
            transverser.filenameAttr = 'dso'
            expand = True
        elif ext_str == '.usd' or ext_str == '.usda' or ext_str == '.usdc':
            # need to find out which procedural to use with it
            procName = 'usd'
            transverser = CustomProceduralTransverser(procName, 'filename', filename)
        else:
            transverser = StandInTransverser()

        transverser.selectionAttr = 'selected_items' # attribute to be updated when the selection changes
        # setting refresh to False forces the tranverser not to refresh the tree
        self.tree.setTransverser(transverser, refresh=False)
        self.properties_panel.setTransverser(transverser)
        # setting node triggers the refresh
        self.tree.setCurrentNode(self.nodeName, expand, filename_changed)
        self.properties_panel.setNode(self.nodeName)

        scriptAttr = self.nodeName + ".dso"
        cmds.scriptJob(attributeChange=[scriptAttr, self.updateAssFile])

        scriptAttr = self.nodeName + ".selected_items"
        cmds.scriptJob(attributeChange=[scriptAttr, self.updateSelectedItems])

    def fileInfoNew(self, nodeAttr):

        currentWidget = self.__currentWidget()
        self.currentNode = ''
        self.currentFilename = ''

        # Here we first create the ProceduralTreeView with a 'None' ProceduralTranverser, because we'll set it later or 
        # in fileInfoReplace
        self.tree = ProceduralTreeView(None, currentWidget)
        self.tree.setObjectName("standinTreeWidget")
        currentWidget.layout().addWidget(self.tree)

        # now add the preperties panel
        self.properties_panel = ProceduralPropertiesPanel(None, currentWidget)
        currentWidget.layout().addWidget(self.properties_panel)

        self.tree.itemSelected.connect(self.showItemProperties)

        cmds.scriptJob(event=["NewSceneOpened", self.newSceneCallback])
        cmds.scriptJob(event=["PostSceneRead", self.newSceneCallback])

        self.fileInfoReplace(nodeAttr)

    def newVariantUI(self, defaultname):

        variantDialog = VariantDialog(variantName=defaultname)

        val = variantDialog.show()
        if val == "cancel":
            return False, False

        return variantDialog.variant, variantDialog.duplicateCurrent

    def editVariant(self, *args):
        if self.variant_node:
            variant_idx = cmds.getAttr("{}.index".format(self.variant_node))
            current_variant_name = cmds.getAttr("{}.variants[{}].name".format(self.variant_node, variant_idx))
            # show pop up to rename the variant
            variant_dialog = VariantDialog(edit=True, variantName=current_variant_name)
            val = variant_dialog.show()
            if val == "cancel":
                return False
            cmds.setAttr("{}.variants[{}].name".format(self.variant_node, variant_idx), variant_dialog.variant, type="string")
            self.variantReplace("{}.variant".format(self.nodeName))

    def removeVariant(self, *args):
        if self.variant_node:
            # get the index
            variant_idxs = cmds.getAttr("{}.variants".format(self.variant_node), multiIndices=True) or []
            variant_idx = cmds.getAttr("{}.index".format(self.variant_node))
            # is the index above 0
            if variant_idx == 0:
                cmds.error("Cannot remove the default look variant on node {}".format(self.variant_node))
                return

            conns = cmds.listConnections("{}.variants[{}].inputs".format(self.variant_node, variant_idx), plugs=True) or []
            for op in conns:
                op_node, plug = op.split('.')
                cmds.delete(op_node, inputConnectionsAndNodes=True)
            cmds.removeMultiInstance("{}.variants[{}]".format(self.variant_node, variant_idx), b=True)
            # shuffle the remaing merge nodes to the previous index
            prev_idx = variant_idx
            for idx in cmds.getAttr("{}.variants".format(self.variant_node), multiIndices=True) or []:
                if idx > prev_idx:
                    this_name = cmds.getAttr("{}.variants[{}].name".format(self.variant_node, idx))
                    cmds.setAttr("{}.variants[{}].name".format(self.variant_node, prev_idx), this_name, type="string")
                    i=0
                    for op in cmds.listConnections("{}.variants[{}].inputs".format(self.variant_node, idx), plugs=True) or []:
                        op_node, plug = op.split('.')
                        cmds.connectAttr("{}.{}".format(op_node, plug),
                                         "{}.variants[{}].inputs[{}]".format(self.variant_node, prev_idx, i),
                                         force=True)
                        cmds.removeMultiInstance("{}.variants[{}].inputs[{}]".format(self.variant_node, idx, i), b=True)
                        i += 1
                    prev_idx = idx

            if prev_idx > variant_idx:
                cmds.removeMultiInstance("{}.variants[{}]".format(self.variant_node, prev_idx))

            cmds.setAttr("{}.index".format(self.variant_node), variant_idx-1)

            # get the current Enums and remove the given variant from it
            self.variantReplace("{}.variant".format(self.nodeName))

    def setVariant(self, *args):
        variant = cmds.getAttr("{}.variant".format(self.nodeName))
        variant_str = cmds.getAttr("{}.variant".format(self.nodeName), asString=True)
        if self.variant_node:
            cmds.setAttr("{}.index".format(self.variant_node), variant)
        self.refreshAssignmentsUI()

    def copyVariant(self, fromIndex, toIndex):

        if self.variant_node:
            conns = cmds.listConnections("{}.variants[{}].inputs".format(self.variant_node, fromIndex), plugs=True) or []
            i = 0
            for op in conns:
                op_node, plug = op.split('.')
                new_op_node = cmds.duplicate(op_node, upstreamNodes=True, returnRootsOnly=False)
                if len(new_op_node):
                    cmds.connectAttr("{}.{}".format(new_op_node[0], plug),
                                     "{}.variants[{}].inputs[{}]".format(self.variant_node, toIndex, i),
                                     force=True)
                i += 1

    def updateVariantAttr(self):

        if self.variant_node:
            enumList = []
            for a in cmds.getAttr('{}.variants'.format(self.variant_node), multiIndices=True) or []:
                variantName = cmds.getAttr("{}.variants[{}].name".format(self.variant_node, a))
                if variantName:
                    enumList.append("{}".format(cmds.getAttr("{}.variants[{}].name".format(self.variant_node, a))))

            enumNames = ':'.join(enumList)
            # compare to current attr
            current_enums = cmds.addAttr("{}.variant".format(self.nodeName), q=True, enumName=True)
            if current_enums != enumNames:
                cmds.deleteAttr("{}.variant".format(self.nodeName))
                cmds.addAttr(self.nodeName, longName="variant", at="enum", enumName=enumNames)
                return True

        return False

    def newVariant(self, *args):
        # check switch node exists, if not make one
        self.variant_node = self.getVariantSwitchNode(True)

        if self.variant_node:
            # make a new merge Node and connect that to the
            # next available input for the variant switch node
            current_index = cmds.getAttr('{}.index'.format(self.variant_node))
            next_index = cmds.getAttr('{}.variants'.format(self.variant_node), size=True)

            new_variant_name, duplicate = self.newVariantUI("pass{}".format(next_index))
            if new_variant_name:
                cmds.setAttr('{}.variants[{}].name'.format(self.variant_node, next_index), new_variant_name, type="string")

                if duplicate:
                    self.copyVariant(current_index, next_index)

                self._update_var_ui = self.updateVariantAttr()

                cmds.setAttr("{}.variant".format(self.nodeName), next_index)
                cmds.setAttr("{}.index".format(self.variant_node), next_index)

                cmds.attrEnumOptionMenu(self.variantCtrl, e=True, attribute="{}.variant".format(self.nodeName))

                self.fileInfoReplace('{}.aiInfo'.format(self.nodeName))

    def getVariantSwitchNode(self, create=False):
        variant_switch = None
        ops = cmds.listConnections('{}.operators'.format(self.nodeName), plugs=True) or []
        for op in ops:
            op_node, plug = op.split('.')
            if cmds.nodeType(op_node) == VARIANTSWITCH_OP:
                # check for variant attribute
                variant_switch = op_node
                break

        if not variant_switch and create:
            # create variant switch node
            variant_switch = cmds.createNode(VARIANTSWITCH_OP, name="variantSwitch#", ss=True)
            cmds.setAttr('{}.variants[0].name'.format(variant_switch), "default", type="string")

            # now move all the connections from the standIn to the merge node
            c=0
            for op in ops:
                cmds.disconnectAttr(op, '{}.operators[{}]'.format(self.nodeName, c))
                cmds.connectAttr(op, '{}.variants[0].inputs[{}]'.format(variant_switch, c))
                c+=1

            for i in cmds.getAttr('{}.operators'.format(self.nodeName), multiIndices=True) or []:
                cmds.removeMultiInstance('{}.operators[{}]'.format(self.nodeName, i))

            cmds.connectAttr('{}.out'.format(variant_switch), '{}.operators[0]'.format(self.nodeName))
            cmds.connectAttr('{}.variant'.format(self.nodeName), '{}.index'.format(variant_switch))

        return variant_switch

    def newSceneCallback(self):
        self.tree.setCurrentNode(None)
        self.tree.clearSelection()
        self.properties_panel.setItem(None, None)
        self.current_node = None
        self.variant_node = None

    @QtCore.Slot(str, object)
    def showItemProperties(self, node, items):
        for item in items:
            self.properties_panel.setItem(node, item)

    def displayTree(self):
        cmds.treeView(self.assInfoPath, edit=True, removeAll=True)
        cmds.treeView(self.assInfoPath, edit=True, visible=True)
        cmds.button(self.inspectAssPath, edit=True, visible=False)
        cmds.button(self.overrideSelectionButton, edit=True, visible=True, enable=False)
        cmds.button(self.selectOperatorButton, edit=True, visible=False, enable=False)        
        # First the Entry type names
        for i in self.assItems:
            nodeType = ' '+i[1]
            '''
            entryType = i[2]
            if not cmds.treeView(self.assInfoPath, query=True, itemExists=entryType):
                cmds.treeView(self.assInfoPath, edit=True, addItem=(entryType, ''))
            '''
            if not cmds.treeView(self.assInfoPath, query=True, itemExists=nodeType):
                cmds.treeView(self.assInfoPath, edit=True, addItem=(nodeType, ''))

            cmds.treeView(self.assInfoPath, edit=True, addItem=(i[0],nodeType))

    def overrideSelection(self):
        selectedItems = cmds.treeView(self.assInfoPath, query=True, selectItem=True) or []
        selectedParents = []
        for selItem in selectedItems:
            nodeType = cmds.treeView(self.assInfoPath, query=True, itemParent=selItem)
            if len(nodeType) > 1:
                selectedType = nodeType[1:]
            else:
                selectedType = selItem[1:]

            if not (selectedType in selectedParents):
                selectedParents.append(selectedType)

        win = MtoAProceduralOperator()
        win.create(self.nodeName, selectedItems, selectedParents)
        return

    def selectOperators(self):
        return

    def populateItems(self):
        self.assItems = []
        self.selectedItems = []
        # get the items in the cache
        '''
        cache_str_list = cmds.getAttr('{}.{}'.format(self.nodeName, CACHE_ATTR)) or []
        for s in cache_str_list:
            self.assItems.append(s.split(','))
        '''

    def useSequenceChange(self, nodeName):

        resolveFilePathSequence(nodeName,
                                'useFrameExtension',
                                'dso',
                                'standInDsoPath',
                                'frameNumber',
                                'useSubFrame'
                                )

    def operatorsReplace(self, nodeAttr):
        self._setActiveNodeAttr(nodeAttr)

        for ctrl in self._msgCtrls:
            cmds.deleteUI(ctrl)
        self._msgCtrls = []

        cmds.setUITemplate('attributeEditorTemplate', pushTemplate=True)
        cmds.setParent(self.otherCol)
        attrSize = mu.getAttrNumElements(*nodeAttr.split('.', 1))

        for i in range(attrSize):
            attrName = '{}[{}]'.format(nodeAttr, i)
            attrLabel = 'Operators[{}]'.format(i)
            ctrl = cmds.attrNavigationControlGrp(at=attrName,
                                                    label=attrLabel, cn="createRenderNode -allWithShadersUp \"defaultNavigation -force true -connectToExisting -source %node -destination "+attrName+"\" \"\"")

            self._msgCtrls.append(ctrl) 

        cmds.setUITemplate('attributeEditorTemplate', popTemplate=True)

    def operatorsNew(self, nodeAttr):

        # TODO: move this into AttributeEditorTemplate
        self._setActiveNodeAttr(nodeAttr)

        self._msgCtrls = []
        cmds.setUITemplate('attributeEditorTemplate', pushTemplate=True)

        cmds.frameLayout(label='Operators', collapse=True)
        cmds.columnLayout(adjustableColumn=True)

        cmds.rowLayout(nc=2)
        cmds.text(label='')
        #cmds.text(label='')
        addInputButton = cmds.button(label='Add Operator')

        self.oppopup = cmds.popupMenu(parent=addInputButton, button=1) 
        cmds.popupMenu(self.oppopup, edit=True, postMenuCommand=Callback(buildOperatorMenu, self.oppopup, nodeAttr))
        cmds.setParent('..') # rowLayout

        cmds.frameLayout(labelVisible=False, collapsable=False)
        self.otherCol = cmds.columnLayout(adjustableColumn=True)
        
        attrSize = mu.getAttrNumElements(*nodeAttr.split('.', 1))

        for i in range(attrSize):
            attrName = '{}[{}]'.format(nodeAttr, i)
            attrLabel = 'Inputs[{}]'.format(i)
            ctrl = cmds.attrNavigationControlGrp(at=attrName,
                                                    label=attrLabel, cn="createRenderNode -allWithShadersUp \"defaultNavigation -force true -connectToExisting -source %node -destination "+attrName+"\" \"\"")
   
            self._msgCtrls.append(ctrl) 

        cmds.setParent('..') # columnLayout
        cmds.setParent('..') # frameLayout

        cmds.setParent('..') # columnLayout
        cmds.setParent('..') # frameLayout
        
        cmds.setParent('..') # columnLayout
        cmds.setParent('..') # frameLayout
        cmds.setUITemplate('attributeEditorTemplate', popTemplate=True)

    def alembicAddLayer(self, nodeAttr):
        basicFilter = 'Alembic Cache(*.abc)'
        projectDir = cmds.workspace(query=True, directory=True)
        projectRootDir = cmds.workspace(query=True, rootDirectory=True)
        ret = cmds.fileDialog2(fileFilter=basicFilter, cap='Add Alembic Layer', okc='Load', fm=1, startingDirectory=projectDir)
        if ret is not None and len(ret):
            # get number of layers
            currentLayers = cmds.getAttr(nodeAttr) or []
            if len(currentLayers):
                currentLayers = currentLayers.split(';')
            else:
                currentLayers = []
            rel_filepath = ret[0].replace(projectRootDir, "")
            currentLayers.append(rel_filepath)
            cmds.setAttr(nodeAttr, ';'.join(currentLayers), type="string")
            self.alembicLayersReplace(nodeAttr)

    def alembicRemoveLayer(self, nodeAttr):
        # get the selected layers
        selectedLayers = cmds.textScrollList(self.abcLayersList, q=True, selectIndexedItem=True)
        currentLayers = cmds.getAttr(nodeAttr)
        if len(currentLayers):
            currentLayers = currentLayers.split(';')
            for i in reversed(selectedLayers):
                currentLayers.pop(i-1)
            cmds.setAttr(nodeAttr, ';'.join(currentLayers), type='string')
            self.alembicLayersReplace(nodeAttr)

    def alembicLayersNew(self, nodeAttr):

        # 2 Columns (Left with label+line edit, Right with list)
        cmds.rowColumnLayout(numberOfColumns=3,
                             rowAttach=[(1, "top", 0)],
                             columnWidth=[(1, 145),(2, 190), (3, 100)],
                             columnAlign=[(1, 'right'),(2, 'right'),(3, 'left')],
                             columnAttach=[(1, 'right', 2), (2, 'left', 2)])

        cmds.text(label="Layers")
        self.abcLayersList = cmds.textScrollList(height=100, width=180, allowMultiSelection=True)

        # add remove buttons
        cmds.rowColumnLayout(numberOfRows=3, rowHeight=[(1,20),(2,20)], rowSpacing=[(2,2)])
        self.addLayerBtn = cmds.button(label='Add Layer', height=20, command=lambda *args: self.alembicAddLayer(nodeAttr))
        self.removeLayerBtn = cmds.button(label='Remove Layer', height=20, command=lambda *args: self.alembicRemoveLayer(nodeAttr))

        cmds.setParent('..') # rowLayout
        cmds.setParent('..') # columnLayout
        self.alembicLayersReplace(nodeAttr)

    def alembicLayersReplace(self, nodeAttr):

        cmds.textScrollList(self.abcLayersList, e=True, removeAll=True)

        # add the layers from the new node
        # numlayers = mu.getAttrNumElements(*nodeAttr.split('.'))
        layer_str = cmds.getAttr(nodeAttr)
        if not layer_str:
            return

        for l in layer_str.split(';'):
            cmds.textScrollList(self.abcLayersList, e=True, append=os.path.basename(l))

        cmds.button(self.addLayerBtn, e=True, command=lambda *args: self.alembicAddLayer(nodeAttr))
        cmds.button(self.removeLayerBtn, e=True, command=lambda *args: self.alembicRemoveLayer(nodeAttr))

    def addAlembicParams(self):

        self.beginLayout('Alembic Settings', collapse=True)
        # fps
        self.addControl('abc_fps', label='FPS')
        # layers
        self.addCustom('abc_layers', self.alembicLayersNew, self.alembicLayersReplace)

        self.beginNoOptimize()
        # name_prefix
        self.addControl('abc_nameprefix', label='Name Prefix')
        # make_instance
        self.addControl('abc_make_instance', label='Make Instance')
        # pull_user_params
        self.addControl('abc_pull_user_params', label='Pull User Params')
        # visibility_ignore
        self.addControl('abc_visibility_ignore', label='Ignore Visibility Attributes')
        # radius_attribute
        self.addControl('abc_radius_attribute', label='Radius Attrbute Name')
        # radius_default
        self.addControl('abc_radius_default', label='Radius Default')
        # radius_scale
        self.addControl('abc_radius_scale', label='Radius Scale')
        # exclude_xform
        self.addControl('abc_exclude_xform', label='Exclude Xform')
        # velocity_ignore
        self.addControl('abc_velocity_ignore', label='Velocity Ignore')
        # velocity_scale
        self.addControl('abc_velocity_scale', label='Velocity Scale')
        self.endNoOptimize()
        self.endLayout()

    def setUserOptions(self, nodeAttr):

        text = cmds.scrollField(self.userOptionsBox, q=True, text=True)
        cmds.setAttr(nodeAttr, text, type='string')

    def userOptionsNew(self, nodeAttr):

        cmds.rowColumnLayout(numberOfColumns=2,
                             rowAttach=[(1, "top", 0)],
                             columnWidth=[(1, 148)],
                             adjustableColumn=2,
                             columnAlign=[(2, 'right')],
                             columnAttach=[(1, 'right', 2)])

        cmds.text(label="User Options")
        self.userOptionsBox = cmds.scrollField(height=80)

        self.userOptionsReplace(nodeAttr)

    def userOptionsReplace(self, nodeAttr):

        cmds.scrollField(self.userOptionsBox, e=True, text=cmds.getAttr(nodeAttr), changeCommand=lambda *args: self.setUserOptions(nodeAttr))

    def setup(self):
        self.assInfoPath = ''
        self.inspectAssPath = ''
        self.variant_node = None
        self._update_var_ui = False
        self.assItems = {}

        self.beginScrollLayout()
        
        self.beginLayout('File/Frame', collapse=False)        
        self.addCustom('dso', ArnoldStandInTemplateDsoNew, ArnoldStandInTemplateDsoReplace)
        self.addControl('standInDrawOverride', label='Viewport Override')
        self.addControl('mode', label='Viewport Draw Mode')
        self.addSeparator()
        self.addControl('useFrameExtension', label='Use File Sequence', changeCommand=self.useSequenceChange)
        self.addControl('frameNumber', label='Frame')
        self.addControl('frameOffset')
        self.addSeparator()
        self.addControl('overrideNodes')
        self.addControl('aiNamespace', label='Namespace')

        self.addSeparator()
        self.addCustom('aiUserOptions', self.userOptionsNew, self.userOptionsReplace)

        self.addSeparator()
        self.addControl('ignoreGroupNodes', label='Ignore Group Nodes')
        # usd and alembic
        # object_path
        self.addControl('objectpath', label='Object Path')
        self.endLayout()

        # alembic options
        self.addAlembicParams()
        
        self.beginNoOptimize()

        self.beginLayout("File Contents", collapse=False)
        self.addCustom('variant', self.variantNew, self.variantReplace)
        self.addCustom('aiInfo', self.fileInfoNew, self.fileInfoReplace)
        self.endLayout()
        # self.addCustom("operators", self.operatorsNew, self.operatorsReplace)
        self.addSeparator()
        

        self.beginLayout('Render Stats', collapse=True)

        # FIXME : the line below is just to create a callback that will be invoked when a StandIn UI
        # appears in the attribute editor. We need to hack the deprecated "override" attribute
        # (see #2515), and making sure they appear with the right value when the scene is 
        # inspected will avoid confusion. We can remove this once we no longer care about
        # pre-2.0.2 compatibility
        self.addCustom('', ArnoldStandInUpdateUI, ArnoldStandInUpdateUI)
        self.addControl('castsShadows', label='Casts Shadows')
        self.addControl('motionBlur', label='Motion Blur')
        self.addControl('primaryVisibility', label='Primary Visibility')
        self.addControl('aiVisibleInDiffuseReflection', label='Visible in Diffuse Reflection')
        self.addControl('aiVisibleInSpecularReflection', label='Visible in Specular Reflection')
        self.addControl('aiVisibleInDiffuseTransmission', label='Visible in Diffuse Transmission')
        self.addControl('aiVisibleInSpecularTransmission', label='Visible in Specular Transmission')
        self.addControl('aiVisibleInVolume', label='Visible in Volume')
        self.endLayout()

        self.beginLayout('StandIn Overrides', collapse=True)
        self.addControl('overrideLightLinking', label='Override StandIn Light Linking')
        self.addControl('overrideShaders', label='Override StandIn Shaders')
        self.addSeparator()
        self.addControl('overrideReceiveShadows', changeCommand=self.updateOverridesVisibility, label='Override Receive Shadows')
        self.addControl('receiveShadows', label='   Receive Shadows')
        self.addControl('overrideSelfShadows', changeCommand=self.updateOverridesVisibility, label='Override Self Shadows')
        self.addControl('aiSelfShadows', label='   Self Shadows')
        self.addControl('overrideOpaque', changeCommand=self.updateOverridesVisibility, label='Override Opaque')
        self.addControl('aiOpaque', label='   Opaque')
        self.addControl('overrideDoubleSided', changeCommand=self.updateOverridesVisibility, label='Override Double-Sided')
        self.addControl('doubleSided', label='   Double-Sided')
        self.addControl('overrideMatte', changeCommand=self.updateOverridesVisibility, label='Override Matte')
        self.addControl('aiMatte', label='   Matte')
        self.endLayout()

        self.endNoOptimize();
        
        self.beginLayout('Object Display', collapse=True)
        self.addControl('visibility')
        self.addControl('template')
        self.addControl('ghosting')
        self.addControl('intermediateObject')
        self.endLayout()
        
        self.beginLayout('Draw Override', collapse=True)
        self.addControl('overrideDisplayType')
        self.addControl('overrideLevelOfDetail')
        self.addControl('overrideShading')
        self.addControl('overrideTexturing')
        self.addControl('overridePlayback')
        self.addControl('overrideEnabled')
        self.addControl('useObjectColor')
        self.addControl('objectColor')
        self.endLayout()
    

        # include/call base class/node attributes
        mel.eval('AEdependNodeTemplate '+self.nodeName)
        
        self.suppress('blackBox')
        self.suppress('containerType')
        self.suppress('templateName')
        self.suppress('viewName')
        self.suppress('iconName')
        self.suppress('templateVersion')
        self.suppress('uiTreatment')
        self.suppress('customTreatment')
        self.suppress('creator')
        self.suppress('creationDate')
        self.suppress('rmbCommand')
        self.suppress('templatePath')
        self.suppress('viewMode')
        self.suppress('ignoreHwShader')
        self.suppress('boundingBoxScale')
        self.suppress('featureDisplacement')
        self.suppress('boundingBoxScale')
        self.suppress('initialSampleRate')
        self.suppress('extraSampleRate')
        self.suppress('textureThreshold')
        self.suppress('normalThreshold')
        self.suppress('lodVisibility')
        self.suppress('ghostingControl')
        self.suppress('ghostPreSteps')
        self.suppress('ghostPostSteps')
        self.suppress('ghostStepSize')
        self.suppress('ghostRangeStart')
        self.suppress('ghostRangeEnd')
        self.suppress('ghostDriver')
        self.suppress('ghostFrames')
        self.suppress('ghosting')
        self.suppress('ghostCustomSteps')
        self.suppress('ghostColorPreA')
        self.suppress('ghostColorPre')
        self.suppress('ghostColorPostA')
        self.suppress('ghostColorPost')
        self.suppress('tweak')
        self.suppress('relativeTweak')
        self.suppress('currentUVSet')
        self.suppress('displayImmediate')
        self.suppress('displayColors')
        self.suppress('displayColorChannel')
        self.suppress('currentColorSet')
        self.suppress('smoothShading')
        self.suppress('drawOverride')
        self.suppress('shadingSamples')
        self.suppress('maxVisibilitySamplesOverride')
        self.suppress('maxVisibilitySamples')
        self.suppress('antialiasingLevel')
        self.suppress('maxShadingSamples')
        self.suppress('shadingSamplesOverride')
        self.suppress('geometryAntialiasingOverride')
        self.suppress('antialiasingLevel')
        self.suppress('volumeSamplesOverride')
        self.suppress('volumeSamples')
        self.suppress('depthJitter')
        self.suppress('ignoreSelfShadowing')
        self.suppress('controlPoints')
        self.suppress('colorSet')
        self.suppress('uvSet')
        self.suppress('weights')
        self.suppress('renderInfo')
        self.suppress('renderLayerInfo')
        self.suppress('compInstObjGroups')
        self.suppress('instObjGroups')
        self.suppress('collisionOffsetVelocityIncrement')
        self.suppress('collisionOffsetVelocityMultiplier')
        self.suppress('collisionDepthVelocityMultiplier')
        self.suppress('collisionDepthVelocityIncrement')

        self.suppress('objectPath')
        self.suppress('abcLayers')
        self.suppress('abcFPS')
        self.suppress('abcNamePrefix')
        self.suppress('abcRadiusAttribute')
        self.suppress('abcRadiusDefault')
        self.suppress('abcRadiusScale')
        self.suppress('abcVelocityIgnore')
        self.suppress('abcVelocityScale')
        self.suppress('abcVisibilityIgnore')
        self.suppress('abcMakeInstance')
        self.suppress('abcPullUserParams')
    
        self.addExtraControls()
        self.endScrollLayout()

    def updateOverridesVisibility(self, nodeName):
                
        cmds.editorTemplate(dimControl=(nodeName, 'receiveShadows', not cmds.getAttr('%s.%s' % (nodeName, 'overrideReceiveShadows'))))  
        cmds.editorTemplate(dimControl=(nodeName, 'aiSelfShadows', not cmds.getAttr('%s.%s' % (nodeName, 'overrideSelfShadows'))))  
        cmds.editorTemplate(dimControl=(nodeName, 'aiOpaque', not cmds.getAttr('%s.%s' % (nodeName, 'overrideOpaque'))))  
        cmds.editorTemplate(dimControl=(nodeName, 'doubleSided', not cmds.getAttr('%s.%s' % (nodeName, 'overrideDoubleSided'))))  
        cmds.editorTemplate(dimControl=(nodeName, 'aiMatte', not cmds.getAttr('%s.%s' % (nodeName, 'overrideMatte'))))  
    

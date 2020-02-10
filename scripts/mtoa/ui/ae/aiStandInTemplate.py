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
from mtoa.ui import exportlook


from mtoa.ui.procview.ProceduralTreeView import ProceduralTreeView, ProceduralTreeModel, ProceduralItem
from mtoa.ui.procview.ProceduralWidgets import ProceduralPropertiesPanel
from mtoa.ui.procview.StandInTransverser import StandInTransverser
from mtoa.ui.procview.AlembicTransverser import AlembicTransverser
from mtoa.ui.procview.CustomProceduralTransverser import CustomProceduralTransverser
from mtoa.ui.procview.UsdTransverser import UsdTransverser
from mtoa.ui.procview.ProceduralTransverser import LOOKSWITCH_OP, SWITCH_OP, \
                                                   MERGE_OP, OVERRIDE_OP, \
                                                   INCLUDEGRAPH_OP, MATERIALX_OP, \
                                                   EXP_REGEX

from mtoa.callbacks import *

from functools import partial

defaultFolder = ""
DEFAULT_LOOK = 'default'
CACHE_ATTR = 'ai_asscache'

################################################



def LoadStandInButtonPush(attrName):
    basicFilter = 'Arnold Archive (*.ass *.ass.gz *.obj *.ply *.abc *.usd *.usda *.usdc)'
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


class LookDialog(object):

    def __init__(self, title="New Look", edit=False, lookName=None):

        self.title = title
        self.edit = edit
        self.look = "newPass"
        if lookName:
            self.look = lookName
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
            ad2=2, label="Look Name",
            text=self.look)

        self._duplicateCurrentWidget = cmds.checkBox(label='Duplicate Current Look')
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

        self.look = cmds.textFieldGrp(self._nameWidget, query=True, text=True)
        self.duplicateCurrent = cmds.checkBox(self._duplicateCurrentWidget, query=True, value=True)

        cmds.layoutDialog(dismiss=msg)


class AEaiStandInTemplate(ShaderAETemplate):

    def __currentWidget(self, pySideType=QtWidgets.QWidget):
        """Cast and return the current widget."""
        # Get the current widget Maya name.
        currentWidgetName = cmds.setParent(query=True)
        return toQtObject(currentWidgetName, pySideType)

    def updateSelectedItems(self, force=False):
        if not self.tree or not self.tree.transverser:
            return
        selection = cmds.getAttr('{}.{}'.format(self.nodeName, 'selected_items')) or ""
        
        if selection == self.tree.transverser.selectionStr and not force:
            return
        #self.tree.transverser.selectionStr = selection
        selectionSplit = selection.split(',')
        for selected in selectionSplit:
            if selected:
                # Prevent firing signals in Qt to avoid infinite loop.
                #oldState = self.tree.blockSignals(True)
                self.tree.select(selected, force)
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

    def getExpandFileType(self):
        '''
        Get if this file type should start off expanded
        '''
        ext_str = os.path.splitext(self.current_filename.split(';')[0])[1].lower()
        if ext_str in ['.abc']:
            return True
        return False

    def refreshAssignmentsUI(self):
        if self.refreshTransverser():
            self.tree.setCurrentNode(self.nodeName, self.getExpandFileType(), force=True)
            self.properties_panel.setNode(self.nodeName)

    def lookReplace(self, nodeAttr):
        old_look = self.look_node
        old_node = self.current_node

        self.look_node = self.getLookSwitchNode()
        self.current_node = self.nodeName

        replace_look = old_look != self.look_node
        replace_node = old_node != self.current_node
        if replace_look or replace_node:

            # clear the look menu
            for m in cmds.optionMenu(self.lookCtrl, q=True, itemListLong=True) or []:
                cmds.deleteUI(m)

            looks = self.getLooks()

            for idx, look in looks.items():
                cmds.menuItem(label=look, data=idx, parent=self.lookCtrl)

            if self.look_node:
                look_idx = cmds.getAttr("{}.index".format(self.look_node))
                cmds.optionMenu(self.lookCtrl, edit=True, value=looks[look_idx])

            # if self.tree:
            #     self.refreshAssignmentsUI()

    def lookNew(self, nodeAttr):

        self.lookRowLayout = cmds.rowColumnLayout(numberOfColumns=6, adjustableColumn=6,
                                         columnAlign=[(1, 'left'), (2, 'left'), (3, 'left'), (4, 'left'), (5, 'left')],
                                         columnAttach=[(1, 'left', 10), (2, 'left', 3), (3, 'left', 1), (4, 'left', 1), (5, 'left', 1)])

        cmds.rowLayout(numberOfColumns=1, rowAttach=[1, 'top', 4], columnAttach=[1, 'left', 0])
        self.lookCtrl = cmds.optionMenu(label="look",changeCommand=self.setLook, height=20,
                                        annotation="Set current look")
        cmds.setParent('..')
        self.newLookCtrl = cmds.symbolButton('standInNewLookButton', image='newRenderPass.png', command=self.newLook,
                                             annotation="Create new look")
        self.editLookCtrl = cmds.symbolButton('standInEditLookButton', image='editRenderPass.png', command=self.editLook,
                                              annotation="Edit current look")
        self.removeLookCtrl = cmds.symbolButton('standInRemoveLookButton', image='deleteRenderPass.png', command=self.removeLook,
                                                annotation="Remove current look")

        self.lookExportCtrl = cmds.symbolButton('standInExportLookButton', image='save.png', command=self.exportLook,
                                                annotation="Export looks to .ass or MaterialX file" )

        cmds.text("")
        cmds.setParent('..')
        self.lookReplace(nodeAttr)

    def refreshTransverser(self):
        projectRootDir = cmds.workspace(query=True, rootDirectory=True)
        fileAttr = '{}.dso'.format(self.nodeName)
        filename = cmds.getAttr(fileAttr)
        filename = expandEnvVars(filename)
        layersAttr = '{}.abc_layers'.format(self.nodeName)
        layers = cmds.getAttr(layersAttr) or ''
        if len(layers):
            for lay in layers.split(';'):
                if not os.path.isfile(lay):
                    lay = os.path.join(projectRootDir, lay)
                filename += ';' + lay

        if self.nodeName == self.transverser_node and filename == self.current_filename:
            self.properties_panel.setItem(self.nodeName, None)
            return False  # nothing to do here...

        filename_changed = False
        if self.nodeName == self.transverser_node and filename != self.current_filename:
            filename_changed = True

        self.transverser_node = self.nodeName
        self.current_filename = filename
        ext_str = ".ass"
        if filename:
            ext_str = os.path.splitext(filename.split(';')[0])[1].lower()

        expand = False
        if ext_str == '.abc':
            transverser = AlembicTransverser()
            transverser.filenameAttr = 'dso'
            expand = True
        elif ext_str == '.usd' or ext_str == '.usda' or ext_str == '.usdc':
            # need to find out which procedural to use with it
            procName = 'usd'
            transverser = UsdTransverser('filename', filename)
        else:
            transverser = StandInTransverser()

        transverser.selectionAttr = 'selected_items' # attribute to be updated when the selection changes
        # setting refresh to False forces the tranverser not to refresh the tree
        self.tree.setTransverser(transverser, refresh=False)
        self.properties_panel.setTransverser(transverser)

        return True

    def fileInfoReplace(self, nodeAttr):
        if self.refreshTransverser():
            # setting node triggers the refresh
            self.tree.setCurrentNode(self.nodeName, self.getExpandFileType(), force=True)
            self.properties_panel.setNode(self.nodeName)

            scriptAttr = self.nodeName + ".dso"
            cmds.scriptJob(attributeChange=[scriptAttr, self.updateAssFile])

            scriptAttr = self.nodeName + ".selected_items"
            cmds.scriptJob(attributeChange=[scriptAttr, self.updateSelectedItems])

        # now get the selection and set it if the selction is not empty
        selection = cmds.getAttr(self.nodeName + ".selected_items")
        if selection:
            self.updateSelectedItems(True)
        else:
            self.tree.clearSelection()

    def fileInfoNew(self, nodeAttr):

        currentWidget = self.__currentWidget()

        # Here we first create the ProceduralTreeView with a 'None' ProceduralTranverser,
        # because we'll set it later or in fileInfoReplace

        self.fileContents = QtWidgets.QFrame(currentWidget)
        self.fileContents.setLayout(QtWidgets.QVBoxLayout(self.fileContents))
        currentWidget.layout().addWidget(self.fileContents)

        self.filter_box = QtWidgets.QLineEdit(self.fileContents)
        self.filter_box.setPlaceholderText("filter ..")
        self.fileContents.layout().addWidget(self.filter_box)
        self.tree = ProceduralTreeView(None, currentWidget)
        self.tree.setObjectName("standinTreeWidget")
        self.fileContents.layout().addWidget(self.tree)

        # now add the preperties panel
        self.properties_panel = ProceduralPropertiesPanel(None, currentWidget)
        self.fileContents.layout().addWidget(self.properties_panel)
        self.tree.itemSelected.connect(self.showItemProperties)
        self.filter_box.textChanged.connect(self.tree.model().setFilterWildcard)

        cmds.scriptJob(event=["NewSceneOpened", self.newSceneCallback])
        cmds.scriptJob(event=["PostSceneRead", self.newSceneCallback])

        self.fileInfoReplace(nodeAttr)

    def newLookUI(self, defaultname):

        lookDialog = LookDialog(lookName=defaultname)

        val = lookDialog.show()
        if val == "cancel":
            return False, False

        return lookDialog.look, lookDialog.duplicateCurrent

    def editLook(self, *args):
        if self.look_node:
            look_idx = cmds.getAttr("{}.index".format(self.look_node))
            current_look_name = cmds.getAttr("{}.looks[{}].name".format(self.look_node, look_idx))
            # show pop up to rename the look
            look_dialog = LookDialog(edit=True, lookName=current_look_name)
            val = look_dialog.show()
            if val == "cancel":
                return False
            cmds.setAttr("{}.looks[{}].name".format(self.look_node, look_idx), look_dialog.look, type="string")
            self.lookReplace("{}.look".format(self.nodeName))

    def removeLook(self, *args):
        if self.look_node:
            # get the index
            look_idxs = cmds.getAttr("{}.looks".format(self.look_node), multiIndices=True) or []
            look_idx = cmds.getAttr("{}.index".format(self.look_node))
            # is the index above 0
            if look_idx == 0:
                cmds.error("Cannot remove the default look look on node {}".format(self.look_node))
                return

            conns = cmds.listConnections("{}.looks[{}].inputs".format(self.look_node, look_idx), plugs=True) or []
            for op in conns:
                op_node, plug = op.split('.')
                cmds.delete(op_node, inputConnectionsAndNodes=True)
            cmds.removeMultiInstance("{}.looks[{}]".format(self.look_node, look_idx), b=True)
            # shuffle the remaing merge nodes to the previous index
            prev_idx = look_idx
            for idx in cmds.getAttr("{}.looks".format(self.look_node), multiIndices=True) or []:
                if idx > prev_idx:
                    this_name = cmds.getAttr("{}.looks[{}].name".format(self.look_node, idx))
                    cmds.setAttr("{}.looks[{}].name".format(self.look_node, prev_idx), this_name, type="string")
                    i=0
                    for op in cmds.listConnections("{}.looks[{}].inputs".format(self.look_node, idx), plugs=True) or []:
                        op_node, plug = op.split('.')
                        cmds.connectAttr("{}.{}".format(op_node, plug),
                                         "{}.looks[{}].inputs[{}]".format(self.look_node, prev_idx, i),
                                         force=True)
                        cmds.removeMultiInstance("{}.looks[{}].inputs[{}]".format(self.look_node, idx, i), b=True)
                        i += 1
                    prev_idx = idx

            if prev_idx > look_idx:
                cmds.removeMultiInstance("{}.looks[{}]".format(self.look_node, prev_idx))

            cmds.setAttr("{}.index".format(self.look_node), look_idx-1)

            # get the current Enums and remove the given look from it
            self.lookReplace("{}.look".format(self.nodeName))

    def setLook(self, *args):
        look_name = args[0]
        if self.look_node:
            looks = {y:x for x,y in self.getLooks().iteritems()}
            cmds.setAttr("{}.index".format(self.look_node), looks[look_name])
        self.refreshAssignmentsUI()

    def copyLook(self, fromIndex, toIndex):

        if self.look_node:
            conns = cmds.listConnections("{}.looks[{}].inputs".format(self.look_node, fromIndex), plugs=True) or []
            i = 0
            for op in conns:
                op_node, plug = op.split('.')
                new_op_node = cmds.duplicate(op_node, upstreamNodes=True, returnRootsOnly=False)
                if len(new_op_node):
                    cmds.connectAttr("{}.{}".format(new_op_node[0], plug),
                                     "{}.looks[{}].inputs[{}]".format(self.look_node, toIndex, i),
                                     force=True)
                i += 1

    def getLooks(self):
        looks = {}

        self.look_node = self.getLookSwitchNode()

        if self.look_node:
            for a in cmds.getAttr('{}.looks'.format(self.look_node), multiIndices=True) or []:
                look_name = cmds.getAttr('{}.looks[{}].name'.format(self.look_node, a))
                looks[a] = look_name
        else:
            looks[0] = DEFAULT_LOOK

        return looks

    def newLook(self, *args):
        # check switch node exists, if not make one
        self.look_node = self.getLookSwitchNode(True)

        if self.look_node:
            # make a new merge Node and connect that to the
            # next available input for the look switch node
            current_index = cmds.getAttr('{}.index'.format(self.look_node))
            next_index = cmds.getAttr('{}.looks'.format(self.look_node), size=True)

            new_look_name, duplicate = self.newLookUI("pass{}".format(next_index))
            if new_look_name:
                cmds.setAttr('{}.looks[{}].name'.format(self.look_node, next_index), new_look_name, type="string")

                if duplicate:
                    self.copyLook(current_index, next_index)

                cmds.setAttr("{}.index".format(self.look_node), next_index)
                cmds.menuItem(label=new_look_name, data=next_index, parent=self.lookCtrl)
                cmds.optionMenu(self.lookCtrl, e=True, value=new_look_name)

                self.fileInfoReplace('{}.aiInfo'.format(self.nodeName))

    def browseObjFilename(self, fileFormat="Arnold Operator Graph (*.ass)", options={}):
        global defaultFolder
        if defaultFolder == "":
            defaultFolder = cmds.workspace(q=True, rd=True, fn=True)
        ret = cmds.fileDialog2(cap='Export Look',
                               okc='Export Look',
                               ff=fileFormat,
                               fm=0,
                               dir=defaultFolder,
                               **options
                               ) or []
        if ret is not None and len(ret):
            defaultFolder = ret[0]
            return ret[0]

    def exportLook(self, mode="ass"):
        # check switch node exists
        self.look_node = self.getLookSwitchNode()

        # get the current export options
        d_options = exportlook.getOperatorOptions()
        # first check for and export looks from aiLookSwicth
        if self.look_node:
            indices = cmds.getAttr('{}.looks'.format(self.look_node), multiIndices=True) or []
            if not len(indices):
                cmds.error("{} has no looks to export".format(self.look_node))
            # include_graph
            filename = self.browseObjFilename(
                "{0[0]} (*{0[1]});; {1[0]} (*{1[1]});;".format(*[(exportlook.OPERATOR_FILETYPES[k], k) for k in sorted(exportlook.OPERATOR_FILETYPES.keys())]),
                options={"selectFileFilter": exportlook.OPERATOR_FILETYPES[d_options['defaultLookExt']],
                         "optionsUICreate": "arnoldOpExportUI_Create",
                         "optionsUIInit": "arnoldOpExportUI_Init",
                         "optionsUICommit": "arnoldOpExportUI_Commit",
                         "fileTypeChanged": "arnoldOpExportUI_Change"}
            )

            if filename:
                options = exportlook.getOperatorOptions()
                ext = os.path.splitext(filename)[-1]
                if ext == ".ass":
                    cmds.arnoldExportOperators(self.look_node, selection=True,
                                               filename=filename,
                                               shaders=options['exportShaders'])
                    # check if we want to replace the graph
                    if options['replaceNetwork']:
                        self.replaceNetwork(filename)
                elif ext == ".mtlx":
                    # get the current index
                    _base_index = cmds.getAttr("{}.index".format(self.look_node))
                    # loop over the looks
                    for idx in indices:
                        if len(cmds.listConnections("{}.looks[{}].inputs".format(self.look_node, idx), plugs=True) or []):
                            look_name = cmds.getAttr("{}.looks[{}].name".format(self.look_node, idx))
                            cmds.setAttr("{}.index".format(self.look_node), idx)

                            properties = self.getLookParams(idx)

                            cmds.arnoldExportToMaterialX(self.nodeName, filename=filename, look=look_name,
                                                         properties=' '.join(properties),
                                                         fullPath=options['exportFullPath'],
                                                         separator=options['exportSeparator'],
                                                         relative=options['relativeAssignments'])

                    cmds.setAttr("{}.index".format(self.look_node), _base_index)
                    if options['replaceNetwork']:
                        self.replaceNetwork(filename)
        else:                    # check if we want to replace the graph
            # if no look node export the current standin
            filename = self.browseObjFilename(
                "{0[0]} (*{0[1]});; {1[0]} (*{1[1]});;".format(*[(exportlook.OPERATOR_FILETYPES[k], k) for k in sorted(exportlook.OPERATOR_FILETYPES.keys())]),
                options={"selectFileFilter": exportlook.OPERATOR_FILETYPES[d_options['defaultLookExt']],
                         "optionsUICreate": "arnoldOpExportUI_Create",
                         "optionsUIInit": "arnoldOpExportUI_Init",
                         "optionsUICommit": "arnoldOpExportUI_Commit",
                         "fileTypeChanged": "arnoldOpExportUI_Change"}
            )
            if filename:
                options = exportlook.getOperatorOptions()
                ext = os.path.splitext(filename)[-1]
                if ext == ".ass":
                    cmds.arnoldExportOperators(self.nodeName, selection=True,
                                               filename=filename,
                                               shaders=options['exportShaders'])
                    if options['replaceNetwork']:
                        self.replaceNetwork(filename)
                elif ext == ".mtlx":

                    properties = []
                    inputs = cmds.listConnections("{}.operators".format(self.nodeName)) or []
                    for o in inputs:
                        # walk the inputs of this node record the parms if it's a set_parameter operator
                        properties += self.getParams(o)

                    cmds.arnoldExportToMaterialX(self.nodeName, filename=filename, look="default",
                                                 properties=' '.join(properties),
                                                 fullPath=options['exportFullPath'],
                                                 separator=options['exportSeparator'],
                                                 relative=options['relativeAssignments'])
                    if options['replaceNetwork']:
                        self.replaceNetwork(filename)

    def getParams(self, op):
        params = []
        # is this an aiSetPArameter node?
        if cmds.nodeType(op) == OVERRIDE_OP:
            a_indices = cmds.getAttr("{}.assignment".format(op), mi=True) or []
            for a in a_indices:
                exp = cmds.getAttr("{}.assignment[{}]".format(op, a))
                m = EXP_REGEX.match(exp)
                param = m.group('param')
                if len(param) and param not in params:
                    params.append(param)

        op_inputs = cmds.listConnections("{}.inputs".format(op)) or []
        for o in op_inputs:
            parmas += _walkParams(o)

        return params

    def getLookParams(self, index):

        self.look_node = self.getLookSwitchNode(True)

        params = []
        # first check fr and export loks from aiLookSwicth
        if self.look_node:
            inputs = cmds.listConnections("{}.looks[{}].inputs".format(self.look_node, index)) or []
            for i in inputs:
                # walk the inputs of this node record the parms if it's a set_parameter operator
                params += self.getParams(i)
        return params

    def getLookSwitchNode(self, create=False):
        look_switch = None
        ops = cmds.listConnections('{}.operators'.format(self.nodeName), plugs=True, type=LOOKSWITCH_OP) or []
        for op in ops:
            op_node, plug = op.split('.')
            if cmds.nodeType(op_node) == LOOKSWITCH_OP:
                # check for look attribute
                look_switch = op_node
                break

        if not look_switch and create:
            # create look switch node
            look_switch = cmds.createNode(LOOKSWITCH_OP, name="lookSwitch#", ss=True)
            cmds.setAttr('{}.looks[0].name'.format(look_switch), "default", type="string")

            # now move all the connections from the standIn to the merge node
            c=0
            for op in cmds.listConnections('{}.operators'.format(self.nodeName), plugs=True) or []:
                cmds.disconnectAttr(op, '{}.operators[{}]'.format(self.nodeName, c))
                cmds.connectAttr(op, '{}.looks[0].inputs[{}]'.format(look_switch, c))
                c+=1

            for i in cmds.getAttr('{}.operators'.format(self.nodeName), multiIndices=True) or []:
                cmds.removeMultiInstance('{}.operators[{}]'.format(self.nodeName, i))

            cmds.connectAttr('{}.out'.format(look_switch), '{}.operators[0]'.format(self.nodeName))

        return look_switch

    def replaceNetwork(self, filename):

        def _getUniqueConnections(op, ops):
            all_ops = []
            all_ops += ops
            op_inputs = cmds.listConnections("{}.inputs".format(op)) or []
            for o in op_inputs:
                skip = False
                # get if this node has connections to nodes outside of the network
                out_connections = cmds.listConnections("{}.out".format(op), source=False) or []
                for oc in out_connections:
                    if oc not in all_ops:
                        skip = True
                if not skip:
                    all_ops += _getUniqueConnections(o, all_ops)

            return all_ops

        fn, ext = os.path.splitext(filename)

        target_connection = ''
        current_look = ''
        # check switch node exists
        self.look_node = self.getLookSwitchNode()

        # first check for and export looks from aiLookSwicth
        if self.look_node:

            look_out_conns = cmds.listConnections("{}.out".format(self.look_node), plugs=True) or []
            target_connection = look_out_conns[0]
            current_look = cmds.getAttr('{}.looks[{}].name'.format(self.look_node, cmds.getAttr('{}.index'.format(self.look_node))))

            indices = cmds.getAttr('{}.looks'.format(self.look_node), multiIndices=True) or []

            op_list = [self.look_node]
            for i in indices:
                connections = cmds.listConnections("{}.looks[{}].inputs".format(self.look_node, i)) or []
                for op in connections:
                    if op not in op_list:
                        op_list.append(op)
                    op_list += _getUniqueConnections(op, op_list)

            for op_node in op_list:
                if cmds.objExists(op_node):
                    cmds.delete(op_node)

        else:
            connections = cmds.listConnections("{}.operators".format(self.nodeName)) or []
            op_list = []
            for op in connections:
                if op not in op_list:
                    op_list.append(op)
                op_list += _getUniqueConnections(op, op_list)

            target_connection = "{}.operators[0]".format(self.nodeName)

        # delete the operator nodes attached to the current standIn
        for op_node in op_list:
            if cmds.objExists(op_node):
                cmds.delete(op_node)

        if target_connection:
            node = None
            if ext == '.ass':
                # make a new ai_includeGraph node
                node = cmds.createNode(INCLUDEGRAPH_OP)
                if current_look:
                    cmds.setAttr("{}.target".format(node), current_look, type="string")
            elif ext == '.mtlx':
                # make new aiMaterialX operator
                node = cmds.createNode(MATERIALX_OP)
                cmds.setAttr("{}.selection".format(node), "*", type="string")
                if current_look:
                    cmds.setAttr("{}.look".format(node), current_look, type="string")
            if node:
                cmds.setAttr("{}.filename".format(node), filename, type="string")
                cmds.connectAttr("{}.out".format(node), target_connection)

    def newSceneCallback(self):
        self.tree.setCurrentNode(None)
        self.tree.clearSelection()
        self.properties_panel.setItem(None, None)
        self.current_node = None
        self.transverser_node = None
        self.current_filename = ''
        self.look_node = None

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
            self.updateAssFile()

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
            self.updateAssFile()

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
        self.current_node = None
        self.transverser_node = None
        self.look_node = None
        self.current_filename = ''
        self.tree = None
        self.properties_panel = None
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


        self.beginLayout('Arnold Procedural Settings', collapse=True)
        self.addControl('useAutoInstancing', label='Auto Instancing', annotation="Disable to prevent automatic instancing of the same .ass file")
        self.endLayout()

        # alembic options
        self.addAlembicParams()
        
        self.beginNoOptimize()

        self.beginLayout("File Contents", collapse=False)
        self.addCustom('look', self.lookNew, self.lookReplace)
        self.addCustom('aiInfo', self.fileInfoNew, self.fileInfoReplace)
        self.endLayout()
        # self.addCustom("operators", self.operatorsNew, self.operatorsReplace)
        self.addSeparator()
        

        self.beginLayout('Render Stats', collapse=True)

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

        self.endNoOptimize()

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
    

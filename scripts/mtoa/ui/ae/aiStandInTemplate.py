import re
import maya.cmds as cmds
import maya.mel as mel
import os.path
import mtoa.melUtils as mu
from mtoa.ui.ae.utils import aeCallback
from mtoa.ui.ae.utils import resolveFilePathSequence
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

from mtoa.callbacks import *
    
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

class AEaiStandInTemplate(ShaderAETemplate):

    def __currentWidget(self, pySideType=QtWidgets.QWidget):
        """Cast and return the current widget."""
        # Get the current widget Maya name.
        currentWidgetName = cmds.setParent(query=True)
        return toQtObject(currentWidgetName, pySideType)

    
    def updateAssFile(self):
        self.assItems = []
        self.fileInfoReplace(self.nodeName + ".dso")
        cmds.setAttr('{}.{}'.format(self.nodeName, CACHE_ATTR), 0, type="stringArray")


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
    
    def fileInfoReplace(self, nodeAttr) :
        fileAttr = '{}.dso'.format(nodeAttr.split('.')[0])
        filename = cmds.getAttr(fileAttr)
        ext_str = os.path.splitext(filename)[1].lower()

        if ext_str == '.abc':
            transverser = AlembicTransverser()
            transverser.filenameAttr = 'dso'
        elif ext_str == '.ass' or ext_str == '.ass.gz' or ext_str == '.gz':
            transverser = StandInTransverser()
        else:
            # need to find out which procedural to use with it
            procName = 'procedural'
            if ext_str == '.usd' or ext_str == '.usda' or ext_str == '.usdc':
                procName = 'usd'
            transverser = CustomProceduralTransverser(procName, 'filename', filename)

        transverser.selectionAttr = 'selected_items' # attribute to be updated when the selection changes
        self.tree.setTransverser(transverser)
        self.properties_panel.setTransverser(transverser)
        self.tree.setCurrentNode(self.nodeName)
        

    def fileInfoNew(self, nodeAttr):
        self.currentItem = None
        currentWidget = self.__currentWidget()

        # Here we first create the ProceduralTreeView with a 'None' ProceduralTranverser, because we'll set it later or 
        # in fileInfoReplace
        self.tree = ProceduralTreeView(None, currentWidget)
        self.tree.setObjectName("standinTreeWidget")
        currentWidget.layout().addWidget(self.tree)

        # now add the preperties panel
        self.properties_panel = ProceduralPropertiesPanel(None, currentWidget)
        currentWidget.layout().addWidget(self.properties_panel)

        self.tree.itemSelected.connect(self.showItemProperties)
        self.fileInfoReplace(nodeAttr)

        fileAttr = self.nodeName + ".dso"
        cmds.scriptJob(attributeChange=[fileAttr, self.updateAssFile],
                       replacePrevious=True, parent=self.inspectAssPath)


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
        cache_str_list = cmds.getAttr('{}.{}'.format(self.nodeName, CACHE_ATTR)) or []
        for s in cache_str_list:
            self.assItems.append(s.split(','))

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

    

    def setup(self):
        self.assInfoPath = ''
        self.inspectAssPath = ''
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

        self.endLayout()
        
        self.beginNoOptimize()

        self.beginLayout("File Contents", collapse=False)
        self.addCustom('aiInfo', self.fileInfoNew, self.fileInfoReplace)
        self.endLayout()
        self.addCustom("operators", self.operatorsNew, self.operatorsReplace)
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
    
        self.addExtraControls()
        self.endScrollLayout()

    def updateOverridesVisibility(self, nodeName):
                
        cmds.editorTemplate(dimControl=(nodeName, 'receiveShadows', not cmds.getAttr('%s.%s' % (nodeName, 'overrideReceiveShadows'))))  
        cmds.editorTemplate(dimControl=(nodeName, 'aiSelfShadows', not cmds.getAttr('%s.%s' % (nodeName, 'overrideSelfShadows'))))  
        cmds.editorTemplate(dimControl=(nodeName, 'aiOpaque', not cmds.getAttr('%s.%s' % (nodeName, 'overrideOpaque'))))  
        cmds.editorTemplate(dimControl=(nodeName, 'doubleSided', not cmds.getAttr('%s.%s' % (nodeName, 'overrideDoubleSided'))))  
        cmds.editorTemplate(dimControl=(nodeName, 'aiMatte', not cmds.getAttr('%s.%s' % (nodeName, 'overrideMatte'))))  
    

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

CACHE_ATTR = 'ai_asscache'

from mtoa.callbacks import *

###### UI to create a procedural operator

class MtoAProceduralOperator(object):
    window = None
    def __new__(cls, *args, **kwargs):
        if not '_instance' in vars(cls):
            cls._instance = super(MtoAProceduralOperator, cls).__new__(cls, *args, **kwargs)
        return cls._instance

    def __init__(self):
        if self.window is None:
            self.window = 'MtoAProceduralOperator'
            self.listElements = []
            self.column = ''
            self.selection = ''
            self.selectedItems = []
            self.selectedTypes = []

    def doCreateOperator(self):
        opType = cmds.optionMenuGrp(self.operatorType, q=True, v=True)
        opNode = cmds.createNode(opType, name = self.standinName + '_op')
        if opNode and len(opNode) > 0:
            selString = cmds.textFieldGrp(self.selection, q=True, tx=True)
            if selString[:1] == ' ':
                selString  = selString[1:]
                selString = '*.(@node==\'{}\')'.format(selString)
            cmds.setAttr('{}.selection'.format(opNode), selString, type='string')
            if opType == 'aiSetParameter':
                paramName = cmds.optionMenuGrp(self.paramBox, q=True, v=True)
                paramVal = cmds.textFieldGrp(self.paramValue, q=True, tx=True)
                if paramName == 'shader':
                    paramVal = '"{}"'.format(paramVal)
                cmds.setAttr('{}.assignment[0]'.format(opNode), '{}={}'.format(paramName, paramVal), type='string')

            attrSize = mu.getAttrNumElements(self.standinName, 'operators')
            newItem = '{}.operators[{}]'.format(self.standinName, attrSize)
            cmds.connectAttr("%s.message"%opNode, newItem, force=True)

        
        cmds.deleteUI(self.window)
        return True

    def doCancel(self):
        cmds.deleteUI(self.window)
        return True

    def create(self, standinName, selectedItems, selectedTypes):
        if cmds.window(self.window, exists=True):
            cmds.deleteUI(self.window)

        winTitle = "Create Operator on " + standinName
        self.selectedItems = selectedItems
        self.selectedTypes = selectedTypes
        self.standinName = standinName
        self.selection = ''

        for sel in selectedItems:
            if len(self.selection) > 0:
                self.selection += ' or '
            self.selection += sel
        
        self.window = cmds.window(self.window, widthHeight=(460, 200), title=winTitle)
        self.createUI()

        cmds.setParent(menu=True)
        cmds.showWindow(self.window)

        try:
            initPos = cmds.windowPref( self.window, query=True, topLeftCorner=True )
            if initPos[0] < 0:
                initPos[0] = 0
            if initPos[1] < 0:
                initPos[1] = 0
            cmds.windowPref( self.window, edit=True, topLeftCorner=initPos )
        except :
            pass

    def updateLayout(self, val):
        setParam = (val == 'aiSetParameter')
        cmds.textFieldGrp(self.paramValue, edit=True, visible=setParam)
        cmds.optionMenuGrp(self.paramBox, edit=True, visible=setParam)


    def createUI(self):
        cmds.scrollLayout(childResizable=True,)
        cmds.columnLayout(adjustableColumn=True)
        #cmds.setParent("..")
        cmds.rowLayout(numberOfColumns=1, columnAlign1='both')
        self.selection = cmds.textFieldGrp('selection', label='Selection', ct2=('left', 'left'), cw2=(90,310), text=self.selection, w=450)
        cmds.setParent("..")
        cmds.rowLayout(numberOfColumns=1, columnAlign1='both')
        self.operatorType = cmds.optionMenuGrp('operatorType', label='Operator Type')
 
        operators = cmds.arnoldPlugins(listOperators=True) or []
        for operator in operators:
            cmds.menuItem(operator)

        # get list of operators
        cmds.optionMenuGrp(self.operatorType, e=True, w=230, ct2=('left', 'left'), cw2=(90,110), v='aiSetParameter', cc=self.updateLayout)
        cmds.setParent("..")

        cmds.rowLayout(numberOfColumns=2, columnAlign1='both')
        self.paramBox = cmds.optionMenuGrp('param', label='Parameter')        
        
        paramList = []
        universeCreated = False

        if not ai.AiUniverseIsActive():
            universeCreated = True
            ai.AiBegin()

        defaultParam = ''

        for nodeType in self.selectedTypes:
            nodeEntry = ai.AiNodeEntryLookUp(nodeType)
            paramIter = ai.AiNodeEntryGetParamIterator(nodeEntry)
            while not ai.AiParamIteratorFinished(paramIter):
                param = ai.AiParamIteratorGetNext(paramIter)
                paramName = ai.AiParamGetName(param)
                if not (paramName in paramList):
                    paramList.append(paramName)

            ai.AiParamIteratorDestroy(paramIter)

        for param in paramList:
            if (len(defaultParam) == 0) or (param == 'shader'):
                defaultParam = param
            cmds.menuItem( label=param)


        cmds.optionMenuGrp(self.paramBox,  e=True, w=200, ct2=('left', 'left'), v=defaultParam, cw2=(90,110))

        self.paramValue = cmds.textFieldGrp('paramValue', label='Value', ct2=('left', 'left'), cw2=(90,150), text='', w=350)
        cmds.setParent("..")
        
        cmds.rowLayout(numberOfColumns=4, columnAlign4=('left', 'left', 'left', 'right'))
        cmds.text( '                                             ')

        cmds.button(label='Ok', al='right', w=85, h=25, command=lambda *args: self.doCreateOperator())
        cmds.text( '              ')
        cmds.button(label='Cancel', al='right', w=85, h=25, command=lambda *args: self.doCancel())
        cmds.setParent("..")

        if universeCreated:
            ai.AiEnd()

################################################

def LoadStandInButtonPush(attrName):
    basicFilter = 'Arnold Archive (*.ass *.ass.gz *.obj *.ply *.abc)'
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

    def updateAssFile(self):
        # clear the cache
        self.assItems = []
        cmds.setAttr('{}.{}'.format(self.nodeName, CACHE_ATTR), 0, type="stringArray")

        cmds.treeView(self.assInfoPath, edit=True, visible=False)
        cmds.button(self.inspectAssPath, edit=True, visible=True)
        cmds.treeView(self.assInfoPath, edit=True, removeAll=True)
        cmds.button(self.overrideSelectionButton, edit=True, visible=False)
        cmds.button(self.selectOperatorButton, edit=True, visible=False)

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

    def fileInfoNew(self, nodeAttr) :
        cmds.rowLayout(nc=2)
        cmds.text(label='')
        self.inspectAssPath = cmds.button(align="center", label='Inspect Arnold File', command=lambda *args: self.inspectFile())
        cmds.setParent('..') # rowLayout
        self.assInfoPath = cmds.treeView(height=300, numberOfButtons=0, allowReparenting=False, editLabelCommand=editLabelCmd, selectCommand=self.selectElement)
        
        cmds.rowLayout(nc=3)
        self.overrideSelectionButton = cmds.button(align="left", label='Override Selection', command=lambda *args: self.overrideSelection())
        cmds.text(label='')
        self.selectOperatorButton = cmds.button(align="right", label='Select Operators', command=lambda *args: self.selectOperators())
        cmds.setParent('..') # rowLayout
        self.fileInfoReplace(nodeAttr)
        
        fileAttr = self.nodeName + ".dso"
        cmds.scriptJob(attributeChange=[fileAttr,self.updateAssFile],
                     replacePrevious=True, parent=self.inspectAssPath)

    def fileInfoReplace(self, nodeAttr) :
        if not cmds.attributeQuery(CACHE_ATTR, node=self.nodeName, exists=True):
            # make the attr
            cmds.addAttr(self.nodeName, longName=CACHE_ATTR, dt="stringArray" )
        self.populateItems()
        if len(self.assItems):
            self.displayTree()
        else:
            cmds.treeView(self.assInfoPath, edit=True, visible=False)
            cmds.button(self.inspectAssPath, edit=True, visible=True)
            cmds.treeView(self.assInfoPath, edit=True, removeAll=True)
            cmds.button(self.overrideSelectionButton, edit=True, visible=False)
            cmds.button(self.selectOperatorButton, edit=True, visible=False)

    def fileInfoReplace(self, nodeAttr) :
        if not cmds.attributeQuery(CACHE_ATTR, node=self.nodeName, exists=True):
            # make the attr
            cmds.addAttr(self.nodeName, longName=CACHE_ATTR, dt="stringArray" )
        self.populateItems()
        if len(self.assItems):
            self.displayTree()
        else:
            cmds.treeView(self.assInfoPath, edit=True, visible=False)
            cmds.button(self.inspectAssPath, edit=True, visible=True)
            cmds.treeView(self.assInfoPath, edit=True, removeAll=True)
            cmds.button(self.overrideSelectionButton, edit=True, visible=False)
            cmds.button(self.selectOperatorButton, edit=True, visible=False)

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

    def inspectFile(self):
        filenameAttr = self.nodeName + '.dso'
        filename = cmds.getAttr(filenameAttr)

        if not os.path.exists(filename):
            return
        
        ext_str = os.path.splitext(filename)[1].lower()
        # do not try to load anything else than .ass files....
        if ext_str != '.ass' and ext_str != '.ass.gz':
            return
       
        self.assItems = []

        universeCreated = False
        if not ai.AiUniverseIsActive():
            universeCreated = True
            ai.AiBegin()

        universe = ai.AiUniverse()
        ai.AiASSLoad(universe, filename, ai.AI_NODE_ALL)

        iter = ai.AiUniverseGetNodeIterator(universe, ai.AI_NODE_ALL);
        
        while not ai.AiNodeIteratorFinished(iter):
            node = ai.AiNodeIteratorGetNext(iter)
            nodeName = ai.AiNodeGetName(node)
            if nodeName == 'root' or nodeName == 'ai_default_reflection_shader' or nodeName == 'options':
                continue
            
            nodeEntry = ai.AiNodeGetNodeEntry(node)
            entryName = ai.AiNodeEntryGetName(nodeEntry)
            #entryType = ai.AiNodeEntryGetTypeName(nodeEntry)
            
            self.assItems.append([nodeName, entryName])

        ai.AiNodeIteratorDestroy(iter)
        ai.AiUniverseDestroy(universe)

        if universeCreated:
            ai.AiEnd()

        cmds.setAttr('{}.{}'.format(self.nodeName, CACHE_ATTR), len(self.assItems), *[','.join(a) for a in self.assItems], type="stringArray")
        self.displayTree()

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
    

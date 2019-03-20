
import maya.mel
from mtoa.ui.ae.operatorTemplate import OperatorAETemplate
import maya.cmds as cmds
import arnold as ai
import os.path

class AEaiIncludeGraphTemplate(OperatorAETemplate):

    def filenameEdit(self, mPath) :
        attr = self.nodeAttr('filename')
        prevPath = cmds.getAttr(attr)
        if prevPath == mPath:
            return
        cmds.setAttr(attr,mPath,type="string")
        cmds.textScrollList(self.targetListPath, edit=True, removeAll=True)

        if not os.path.isfile(mPath): 
            return

        attrName = self.nodeAttr('target')
        self.targetParamReplace(attrName)

        
    def LoadFilenameButtonPush(self, *args):
        basicFilter = 'Arnold Scenes (*.ass);;All Files (*.*)'
        projectDir = cmds.workspace(query=True, directory=True)
        ret = cmds.fileDialog2(fileFilter=basicFilter,
                                cap='Load Operator Graph',okc='Load',fm=4, startingDirectory=projectDir)
        if ret is not None and len(ret):
            self.filenameEdit(ret[0])
            cmds.textFieldGrp("filenameGrp", edit=True, text=ret[0])
            
    def filenameNew(self, nodeName):
        cmds.rowLayout(nc=2, cw2=(360,30), cl2=('left', 'left'), adjustableColumn=1, columnAttach=[(1, 'left', -4), (2, 'left', 0)])
        path = cmds.textFieldGrp("filenameGrp", label="Filename",
                                        changeCommand=self.filenameEdit)
        cmds.textFieldGrp(path, edit=True, text=cmds.getAttr(nodeName))
        cmds.symbolButton( image='navButtonBrowse.png', command=self.LoadFilenameButtonPush)
     
    def filenameReplace(self, nodeName):
        cmds.textFieldGrp("filenameGrp", edit=True,
                                    text=cmds.getAttr(nodeName) )
            
    def targetEdit(self, nodeName, mPath) :
        self.updateList(mPath)
        

    def targetListEdit(self, nodeName) :
        
        targetListField = self.targetListPath
        targetTextField = self.targetPath
        
        selectedList = cmds.textScrollList(targetListField, query=True, si=True);
        targetValue = ''
        addSpace = False

        for item in selectedList:
            if addSpace:
                targetValue += ' '
            
            addSpace = True
            targetValue += item

        cmds.setAttr(nodeName, targetValue, type='string')
        cmds.textField(targetTextField, edit=True, text=targetValue)

    def updateList(self, targetValue):

        targetListField = self.targetListPath
        targetTextField = self.targetPath

        cmds.textScrollList(targetListField, edit=True, deselectAll=True)
        if not targetValue:
            return

        targetList = targetValue.split(' ')
        for target in targetList:
            cmds.textScrollList(targetListField, edit=True, selectItem=target)
            

    def targetParamNew(self, nodeName) :

        textLabel = 'Target Operator'
        labelWidth = 92
    
        # 2 Columns (Left with label+line edit, Right with list)
        cmds.rowColumnLayout( numberOfColumns=2, columnWidth=[(1,320),(2,100)], columnAlign=[(1, 'right'),(2, 'left')], columnAttach=[(1, 'right', 0), (2, 'left', 5)]) 
        # 2 Rows (to get an empty space below the label)
        cmds.rowColumnLayout( numberOfRows=2, rowHeight=[(1,20),(2,20)] )
        # 2 Columns : label and line edit
        cmds.rowColumnLayout( numberOfColumns=2, columnWidth=[(1,labelWidth),(2,175)] )
        
        cmds.text(label=textLabel)
        self.targetPath = cmds.textField( 'arnoldTargetOp', height=20)
        cmds.setParent('..')
        cmds.setParent('..')
        self.targetListPath = cmds.textScrollList(height=50, width=120, allowMultiSelection=False)
        cmds.setParent('..')
        self.targetParamReplace(nodeName)
        
        
    def targetParamReplace(self, plugName) :

        attrName = plugName.replace('.target', '.filename')
                
        cmds.textField(self.targetPath, edit=True, changeCommand=lambda *args: self.targetEdit(plugName, *args))
        cmds.textScrollList(self.targetListPath, edit=True, removeAll=True,selectCommand=lambda *args: self.targetListEdit(plugName, *args))
        targetParam = cmds.getAttr(plugName)

        filename = cmds.getAttr(attrName)

        print filename
        target_ops = []

        universeCreated = False
        if not ai.AiUniverseIsActive():
            universeCreated = True
            ai.AiBegin()

        universe = ai.AiUniverse()
        ai.AiASSLoad(universe, filename)

        operators = []
        inputOperators = []

        defaultOperatorNode = ai.AiNodeGetPtr(ai.AiUniverseGetOptions(universe), "operator")
        defaultOperator = ''
        if defaultOperatorNode:
            defaultOperator = ai.AiNodeGetName(defaultOperatorNode)

        iter = ai.AiUniverseGetNodeIterator(universe, ai.AI_NODE_OPERATOR);
        while not ai.AiNodeIteratorFinished(iter):
            node = ai.AiNodeIteratorGetNext(iter)
            nodeName = ai.AiNodeGetName(node)
            operators.append(nodeName)
            
            inputsArray = ai.AiNodeGetArray(node, "inputs")
            numInputs = ai.AiArrayGetNumElements(inputsArray)
            for i in range(numInputs):
                inputNode = ai.AiArrayGetPtr(inputsArray, i)
                if inputNode:
                    inputOperators.append(ai.AiNodeGetName(inputNode))
        ai.AiNodeIteratorDestroy(iter)

        if defaultOperator != '' and defaultOperator in inputOperators:
            cmds.textScrollList(self.targetListPath, edit=True, append=str(defaultOperator))

        for operator in operators:
            if operator in inputOperators:
                continue
            cmds.textScrollList(self.targetListPath, edit=True, append=str(operator))

        ai.AiUniverseDestroy(universe)

        if universeCreated:
            ai.AiEnd()
        
        targetVal = cmds.getAttr(plugName) or ''
        
        if targetVal == '' and defaultOperator != '':
            cmds.textScrollList(self.targetListPath, edit=True, selectItem=defaultOperator)

        cmds.textField(self.targetPath, edit=True, text=targetVal)
    
    def setup(self):
        self.beginScrollLayout()
        self.beginLayout('Operator', collapse=False)
        self.addControl("enable")
        self.addControl("selection")
        self.endLayout()
        self.addCustom('filename', self.filenameNew, self.filenameReplace)

        self.addCustom('target', self.targetParamNew, self.targetParamReplace)

        self.addOperatorInputs()
        maya.mel.eval('AEdependNodeTemplate '+self.nodeName)
        self.addExtraControls()
        self.endScrollLayout()


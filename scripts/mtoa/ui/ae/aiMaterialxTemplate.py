import maya.mel
from mtoa.ui.ae.operatorTemplate import OperatorAETemplate
import maya.cmds as cmds
import mtoa.ui.ae.utils as aeUtils
import os
import os.path

def GetMtlxLooks(filename):
    looks = []

    if os.path.exists(filename):
        with open(filename, 'r') as content_file:
            fileLine = content_file.read()
            ind = fileLine.find('<look name')

            while ind >= 0:
                ind = ind + 12
                fileLine = fileLine[ind:]

                ind = fileLine.find('>')
                if ind > 1:
                    lookName = fileLine[:ind - 1]
                    firstChar = lookName[:1]
                    while firstChar == ' ' or firstChar == '=' or firstChar == '\'' or firstChar == '\"':
                        lookName = lookName[1:]
                        firstChar = lookName[:1]

                    lastChar = lookName[-1:]
                    while lastChar == ' ' or lastChar == '\'' or lastChar == '\"':
                        lookName = lookName[:-1]
                        firstChar = lookName[-1:]

                    looks.append(lookName)

                ind = fileLine.find('<look name')

    return looks

class AEaiMaterialxTemplate(OperatorAETemplate):

    '''
    def selectionReplace(self, attrName):
        aeUtils.attrTextFieldGrp('aiMaterialxSelection', edit=True, attribute=attrName)
        

    def selectionNew(self, attrName):
        
        aeUtils.attrTextFieldGrp('aiMaterialxSelection', label='Selection', attribute=attrName.format(self.nodeName))
        #cmds.button('aiMaterialxAddSelection',  label='Add Selected Nodes', command=lambda arg=None: self.addSelection())
    ''' 

    def updateList(self, lookValue):

        cmds.textScrollList(self.looksListField, edit=True, deselectAll=True)
        if not lookValue:
            return

        cmds.textScrollList(self.looksListField, edit=True, selectItem=lookValue)            
        

    def lookEdit(self, attrName, mPath) :
        self.updateList(mPath)
        
    def looksListEdit(self, attrName) :
        lookValue = ''        
        selectedList = cmds.textScrollList(self.looksListField, query=True, si=True);
        if (selectedList and len(selectedList) > 0):
            lookValue = selectedList[0]

        cmds.setAttr(attrName, lookValue, type='string')
        cmds.textField(self.lookTextField, edit=True, text=lookValue)

    def filenameEdit(self, attrName, mPath) :
        prevFilename = cmds.getAttr(attrName)
        if prevFilename == mPath:
            return
        
        cmds.setAttr(attrName,mPath,type='string')
        cmds.textScrollList(self.looksListField, edit=True, removeAll=True)

        if not os.path.isfile(mPath):
            return
        
        lookAttrName = attrName.replace('.filename', '.look')
        self.lookParamReplace(lookAttrName)
        

    def filenameButtonPush(self, attrName):
        basicFilter = 'MaterialX File(*.mtlx)'
        projectDir = cmds.workspace(query=True, directory=True)     
        ret = cmds.fileDialog2(fileFilter=basicFilter, cap='Load MaterialX File',okc='Load',fm=1, startingDirectory=projectDir)
        if ret is not None and len(ret):
            self.filenameEdit(attrName, ret[0])
            cmds.textField('arnoldMaterialxFilenamePath', edit=True, text=ret[0])

    def filenameNew(self, attrName) :
        cmds.rowColumnLayout( numberOfColumns=3, columnAlign=[(1, 'right'),(2, 'right'),(3, 'left')], columnAttach=[(1, 'right', 0), (2, 'both', 0), (3, 'left', 5)], columnWidth=[(1,145),(2,220),(3,30)] )
        cmds.text(label='Mtlx Filename ')
        path = cmds.textField('arnoldMaterialxFilenamePath',changeCommand=lambda *args: self.filenameEdit(attrName, *args))
        cmds.textField( path, edit=True, text=cmds.getAttr(attrName) )
        cmds.symbolButton('arnoldMaterialxFilenamePathButton', height=20, image='navButtonBrowse.png', command=lambda *args: self.filenameButtonPush(attrName))
        
    def filenameReplace(self, plugName) :
        cmds.textField( 'arnoldMaterialxFilenamePath', edit=True, changeCommand=lambda *args: self.filenameEdit(plugName, *args))
        cmds.textField( 'arnoldMaterialxFilenamePath', edit=True, text=cmds.getAttr(plugName) )
        cmds.symbolButton('arnoldMaterialxFilenamePathButton', edit=True, image='navButtonBrowse.png' , command=lambda *args: self.filenameButtonPush(plugName))

    def lookParamNew(self, attrName) :

        textLabel = 'Look'
        labelWidth = 42
        
        # 2 Columns (Left with label+line edit, Right with list)
        cmds.rowColumnLayout( numberOfColumns=2, columnWidth=[(1,320),(2,100)], columnAlign=[(1, 'right'),(2, 'left')], columnAttach=[(1, 'right', 0), (2, 'left', 5)]) 
        # 2 Rows (to get an empty space below the label)
        cmds.rowColumnLayout( numberOfRows=2, rowHeight=[(1,20),(2,20)] )
        # 2 Columns : label and line edit
        cmds.rowColumnLayout( numberOfColumns=2, columnWidth=[(1,labelWidth),(2,175)] )

        
        cmds.text(label=textLabel)
        self.lookTextField = cmds.textField( 'arnoldMaterialxLook', height=20)

        cmds.setParent('..')
        cmds.setParent('..')

        self.looksListField = cmds.textScrollList(height=50,allowMultiSelection=False)
        cmds.setParent('..')
        self.lookParamReplace(attrName)
        
        
    def lookParamReplace(self, attrName) :
        mtlxAttrName = attrName.replace('.look', '.filename')        
        cmds.textField(self.lookTextField, edit=True, changeCommand=lambda *args: self.lookEdit(attrName, *args))
        cmds.textScrollList(self.looksListField, edit=True, removeAll=True,selectCommand=lambda *args: self.looksListEdit(attrName, *args))
        lookParam = cmds.getAttr(attrName)
        filename = cmds.getAttr(mtlxAttrName)

        if filename is not None:
            filename = os.path.expandvars(filename)
            
            if os.path.isfile(filename):
                looksList = GetMtlxLooks(filename)
                for look in looksList:
                    cmds.textScrollList(self.looksListField, edit=True, append=str(look))

        cmds.textField(self.lookTextField, edit=True, text=cmds.getAttr(attrName))
        

    def setup(self):
    	self.beginScrollLayout()
        
        self.beginLayout('Operator', collapse=False)
        self.addControl("enable")
        #self.addCustom('selection', self.selectionNew, self.selectionReplace)
        self.addControl("selection")

        self.endLayout()
        self.beginLayout('MaterialX', collapse=False)
        self.addCustom('filename', self.filenameNew, self.filenameReplace)
        
        self.looksListPath = ''
        self.lookPath = ''
        
        self.addCustom('look', self.lookParamNew, self.lookParamReplace)

        self.addSeparator()
        self.addControl("assign_materials")
        self.addControl("assign_properties")
        self.addControl("assign_visibilities")
        self.endLayout()
        self.addOperatorInputs()

        maya.mel.eval('AEdependNodeTemplate '+self.nodeName)

        self.addExtraControls()

        self.endScrollLayout()


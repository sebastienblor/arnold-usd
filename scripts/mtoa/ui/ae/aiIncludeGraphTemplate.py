
import maya.mel
from mtoa.ui.ae.operatorTemplate import OperatorAETemplate
import maya.cmds as cmds
    
class AEaiIncludeGraphTemplate(OperatorAETemplate):

    def filenameEdit(self, mData) :
        attr = self.nodeAttr('filename')
        cmds.setAttr(attr,mData,type="string")
        
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
            
    
    def setup(self):
        self.beginScrollLayout()
        self.beginLayout('Operator', collapse=False)
        self.addControl("enable")
        self.addControl("selection")
        self.endLayout()
        self.addCustom('filename', self.filenameNew, self.filenameReplace)
        self.addOperatorInputs()
        maya.mel.eval('AEdependNodeTemplate '+self.nodeName)
        self.addExtraControls()
        self.endScrollLayout()


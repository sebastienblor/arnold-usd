import maya.mel
import maya.cmds as cmds
from mtoa.ui.ae.shaderTemplate import ShaderAETemplate
import os
import arnold as ai
from mtoa.callbacks import *




class AEaiMaterialXShaderTemplate(ShaderAETemplate):
    
    def setup(self):
        self.beginScrollLayout()

        self.addCustom('materialXFilePath', self.mtlxFilePathNew, self.mtlxFilePathReplace)
        self.addCustom('materialName', self.listMaterialsNew, self.listMaterialsReplace)
        maya.mel.eval('AEdependNodeTemplate '+self.nodeName)
        self.addExtraControls()
        self.endScrollLayout()

    def filenameEdit(self, mPath) :
            attr = self.nodeAttr('materialXFilePath')
            prevPath = cmds.getAttr(attr)
            if prevPath == mPath:
                return
            cmds.setAttr(attr,mPath,type="string")
            self.populateMaterials(mPath)

    def LoadFilenameButtonPush(self, *args):
        basicFilter = 'Axf Files (*.mtlx);;All Files (*.*)'
        projectDir = cmds.workspace(query=True, directory=True)
        ret = cmds.fileDialog2(fileFilter=basicFilter,
                                cap='Load MaterialX File',okc='Load',fm=4, startingDirectory=projectDir)
        if ret is not None and len(ret):
            self.filenameEdit(ret[0])
            cmds.textFieldGrp("filenameGrp", edit=True, text=ret[0])
    
    def mtlxFilePathNew(self, nodeName):
        cmds.rowLayout(nc=2, cw2=(360,30), cl2=('left', 'left'), adjustableColumn=1, columnAttach=[(1, 'left', -4), (2, 'left', 0)])
        info = cmds.getAttr(nodeName)
        self.axfFilePathText = cmds.textFieldGrp("filenameGrp", label="MaterialX Filename", changeCommand = self.filenameEdit, text = info)
        cmds.symbolButton( image='navButtonBrowse.png', command=self.LoadFilenameButtonPush)

    def mtlxFilePathReplace(self, nodeName):
        if cmds.getAttr(nodeName):
            cmds.textFieldGrp("filenameGrp", edit=True, text=cmds.getAttr(nodeName) )
        else:
            cmds.textFieldGrp("filenameGrp", edit=True, text= "" )

    # def texturePathEdit(self, mPath) :
    #         attr = self.nodeAttr('texturePath')
    #         prevPath = cmds.getAttr(attr)
    #         if prevPath == mPath:
    #             return
    #         cmds.setAttr(attr,mPath,type="string")

    # def SelectTexturePathButtonPush(self, *args):
    #     projectDir = cmds.workspace(query=True, directory=True)
    #     ret = cmds.fileDialog2(cap='Select Texture Path',okc='Ok',fm=3, startingDirectory=projectDir)
    #     if ret is not None and len(ret):
    #         self.texturePathEdit(ret[0])
    #         cmds.textFieldGrp("texnameGrp", edit=True, text=ret[0])
    
    
    def populateMaterials(self, mPath):
        looksArray = ai.AiMaterialxGetMaterialNames(mPath)
        numLooks = ai.AiArrayGetNumElements(looksArray)
        for i in range(numLooks):
            look = ai.AiArrayGetStr(looksArray, i)
            cmds.menuItem(parent = self.materials, label=str(look))

    def setMaterialName(self, nodeName):
        cmds.setAttr(nodeName, cmds.optionMenu(self.materials, q = True , value = True), type = "string")
    
    
    def listMaterialsNew(self, nodeName):
        cmds.rowLayout(nc=2, cw2=(360,30), cl2=('left', 'left'), adjustableColumn=1, columnAttach=[(1, 'left', -4), (2, 'left', 0)])
        self.materials = cmds.optionMenu( label='Materials', changeCommand = Callback(self.setMaterialName, nodeName))

    def listMaterialsReplace(self, nodeName):
        print "Goyyale"
        # if cmds.getAttr(nodeName):
        #     cmds.textFieldGrp("texnameGrp", edit=True,text=cmds.getAttr(nodeName) )
        # else:
        #     cmds.textFieldGrp("texnameGrp", edit=True, text="")


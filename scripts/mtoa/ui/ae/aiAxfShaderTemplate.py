import maya.mel
import maya.cmds as cmds
from mtoa.ui.ae.shaderTemplate import ShaderAETemplate
import os

defaultFolder = ""


class AEaiAxfShaderTemplate(ShaderAETemplate):
    
    def setup(self):
        self.beginScrollLayout()
        self.axfDirectory = os.path.join(cmds.workspace( q=True, directory=True), "sourceimages", "axf")
        if (not os.path.exists(self.axfDirectory)) :
            os.mkdir(self.axfDirectory)

        self.addCustom('axfFilePath', self.axfFilePathNew, self.axfFilePathReplace)
        self.addCustom('texturePath', self.texturePathNew, self.texturePathReplace)
        self.addControl('uvScale', label='UV Scale')
        maya.mel.eval('AEdependNodeTemplate '+self.nodeName)
        self.addExtraControls()
        self.endScrollLayout()

    def filenameEdit(self, mPath) :
            attr = self.nodeAttr('axfFilePath')
            prevPath = cmds.getAttr(attr)
            if prevPath == mPath:
                return
            cmds.setAttr(attr,mPath,type="string")

    def LoadFilenameButtonPush(self, *args):
        basicFilter = 'Axf Files (*.axf);;All Files (*.*)'
        global defaultFolder
        if defaultFolder == "":
            defaultFolder = "{}/{}".format(cmds.workspace(q=True, rd=True),
                                           cmds.workspace(fre="AXF")
                                           )
        ret = cmds.fileDialog2(fileFilter=basicFilter,
                                cap='Load Axf File',okc='Load',fm=4, startingDirectory=defaultFolder)
        if ret is not None and len(ret):
            defaultFolder = ret[0]
            self.filenameEdit(ret[0])
            cmds.textFieldGrp("filenameGrp", edit=True, text=ret[0])
    
    def axfFilePathNew(self, nodeName):
        cmds.rowLayout(nc=2, cw2=(360,30), cl2=('left', 'left'), adjustableColumn=1, columnAttach=[(1, 'left', -4), (2, 'left', 0)])
        info = cmds.getAttr(nodeName)
        self.axfFilePathText = cmds.textFieldGrp("filenameGrp", label="Axf Filename", changeCommand = self.filenameEdit, text = info)
        cmds.symbolButton( image='navButtonBrowse.png', command=self.LoadFilenameButtonPush)

    def axfFilePathReplace(self, nodeName):
        if cmds.getAttr(nodeName):
            cmds.textFieldGrp("filenameGrp", edit=True, text=cmds.getAttr(nodeName) )
        else:
            cmds.textFieldGrp("filenameGrp", edit=True, text= "" )

    def texturePathEdit(self, mPath) :
            attr = self.nodeAttr('texturePath')
            prevPath = cmds.getAttr(attr)
            if prevPath == mPath:
                return
            cmds.setAttr(attr,mPath,type="string")

    def SelectTexturePathButtonPush(self, *args):
        projectDir = cmds.workspace(query=True, directory=True)
        ret = cmds.fileDialog2(cap='Select Texture Path',okc='Ok',fm=3, startingDirectory=projectDir)
        if ret is not None and len(ret):
            self.texturePathEdit(ret[0])
            cmds.textFieldGrp("texnameGrp", edit=True, text=ret[0])
    
    def texturePathNew(self, nodeName):
        cmds.rowLayout(nc=2, cw2=(360,30), cl2=('left', 'left'), adjustableColumn=1, columnAttach=[(1, 'left', -4), (2, 'left', 0)])
        info = cmds.getAttr(nodeName)
        if (not info):
            info = "sourceimages/axf"
            cmds.setAttr(self.nodeAttr('texturePath'), info , type="string")
        
        self.texturePathText = cmds.textFieldGrp("texnameGrp", label="Axf Texture Path", changeCommand = self.texturePathEdit, text = info)
        cmds.symbolButton( image='navButtonBrowse.png', command=self.SelectTexturePathButtonPush)
     
    def texturePathReplace(self, nodeName):
        if cmds.getAttr(nodeName):
            cmds.textFieldGrp("texnameGrp", edit=True,text=cmds.getAttr(nodeName) )
        else:
            cmds.textFieldGrp("texnameGrp", edit=True, text="")


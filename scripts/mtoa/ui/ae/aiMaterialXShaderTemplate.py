import maya.mel
import maya.cmds as cmds
from mtoa.ui.ae.shaderTemplate import ShaderAETemplate
import os
import arnold as ai
from mtoa.callbacks import *

defaultFolder = ""

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
            self.listMaterialsReplace(self.nodeAttr('materialName'))

    def LoadFilenameButtonPush(self, *args):
        basicFilter = 'MaterialX Files (*.mtlx);;All Files (*.*)'
        global defaultFolder
        if defaultFolder == "":
            defaultFolder = "{}/{}".format(cmds.workspace(q=True, rd=True),
                                           cmds.workspace(fre="materialx")
                                           )
        ret = cmds.fileDialog2(fileFilter=basicFilter,
                                cap='Load MaterialX File',okc='Load',fm=4, startingDirectory=defaultFolder)
        if ret is not None and len(ret):
            defaultFolder = ret[0]
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

    def setMaterialName(self, nodeName):
        cmds.setAttr(nodeName, cmds.optionMenu(self.materials, q = True , value = True), type = "string")

    def listMaterialsNew(self, nodeName):
        cmds.rowLayout(nc=2, cw2=(300,30), cl2=('center', 'center'), adjustableColumn=1, columnAttach=[(1, 'left', 0), (2, 'left', 0)])
        self.materials = cmds.optionMenu( label='Materials', changeCommand = Callback(self.setMaterialName, nodeName))

    def listMaterialsReplace(self, nodeName):
        material_value = cmds.getAttr(nodeName)
        for m in cmds.optionMenu(self.materials, q=True, itemListLong=True) or []:
            cmds.deleteUI(m)

        node = nodeName.split('.')[0]
        mPath = cmds.getAttr(node+'.materialXFilePath')

        if (mPath):
            looksArray = ai.AiMaterialxGetMaterialNames(mPath)
            numLooks = ai.AiArrayGetNumElements(looksArray)
            found = False
            for i in range(numLooks):
                look = ai.AiArrayGetStr(looksArray, i)
                cmds.menuItem(parent=self.materials, label=str(look))
                if (str(look) == str(material_value)):
                    cmds.optionMenu(self.materials, e=True, sl=i+1)
                    found = True

            # set the attribute to the first item in the option menu
            if not found and numLooks >= 1:
                cmds.optionMenu(self.materials, e=True, sl=1)
                cmds.setAttr(nodeName, cmds.optionMenu(self.materials, q=True, value=True), type="string")

            # set materialName blank if no materials are found
            elif not numLooks:
                cmds.setAttr(nodeName, "", type="string")
        cmds.optionMenu(self.materials, e = True , changeCommand = Callback(self.setMaterialName, nodeName))

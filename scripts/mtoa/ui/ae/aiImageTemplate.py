import mtoa.utils as utils
import mtoa.ui.ae.utils as aeUtils
import maya.cmds as cmds
from mtoa.ui.ae.shaderTemplate import ShaderAETemplate
import maya.mel as mel
from mtoa.ui.ae.utils import resolveFilePathSequence

defaultFolder = ""


class AEaiImageTemplate(ShaderAETemplate):
    def filenameEdit(self, newFilename) :
        attr = self.nodeAttr('filename')
        cmds.setAttr(attr, newFilename, type="string")
        attr = self.nodeAttr('colorSpace')
        ignoreFileRules =  cmds.getAttr(self.nodeAttr('ignoreColorSpaceFileRules'))

        # if ignoreFileRules is enabled, we don't want to update the color space (#2843)
        if ignoreFileRules == 0:
            cmds.setAttr(attr, cmds.colorManagementFileRules(evaluate=newFilename), type="string")

    def LoadFilenameButtonPush(self, *args):
        global defaultFolder
        if defaultFolder == "":
            defaultFolder = "{}/{}".format(cmds.workspace(q=True, rd=True),
                                           cmds.workspace(fre="sourceImages")
                                           )
        basicFilter = 'All Files (*.*)'
        ret = cmds.fileDialog2(fileFilter=basicFilter,
                               cap='Load Image File',
                               okc='Load', fm=4,
                               dir=defaultFolder)
        if ret is not None and len(ret):
            defaultFolder = ret[0]
            self.filenameEdit(ret[0])
            cmds.textFieldGrp("filenameImageGrp", edit=True, text=ret[0])

    def filenameNew(self, nodeName):
        #cmds.rowLayout(nc=3)
        cmds.rowLayout(nc=2, cw2=(360,30), cl2=('left', 'left'), adjustableColumn=1, columnAttach=[(1, 'left', -4), (2, 'left', 0)])
        path = cmds.textFieldGrp("filenameImageGrp", label="Image Name", changeCommand=self.filenameEdit)
        cmds.textFieldGrp(path, edit=True, text=cmds.getAttr(nodeName))
        cmds.symbolButton( image='navButtonBrowse.png', command=self.LoadFilenameButtonPush)

    def filenameReplace(self, nodeName):
        cmds.textFieldGrp( "filenameImageGrp", edit=True, text=cmds.getAttr(nodeName) )

    @staticmethod
    def editUVSet(newValue):
        try:
            if len(newValue) > 0:
                cmds.attrFieldGrp('aiImageUVCoords', edit=True, enable=False)
            else:
                cmds.attrFieldGrp('aiImageUVCoords', edit=True, enable=True)
            mel.eval('refreshAE')
        except:
            import traceback, sys
            traceback.print_exc(file=sys.__stderr__)

    def uvsetNew(self, attrName):
        cmds.setUITemplate('attributeEditorPresetsTemplate', pushTemplate=True)
        aeUtils.attrTextFieldGrp('aiImageUVSet', label='UV Set', attribute=attrName, changeCommand=AEaiImageTemplate.editUVSet)
        cmds.setUITemplate(popTemplate=True)

    def uvsetReplace(self, attrName):
        try:
            aeUtils.attrTextFieldGrp('aiImageUVSet', edit=True, attribute=attrName, changeCommand=AEaiImageTemplate.editUVSet)
        except:
            pass

    def uvcoordsNew(self, attrName):
        cmds.setUITemplate('attributeEditorPresetsTemplate', pushTemplate=True)
        enabled = True
        if len(cmds.getAttr('%s.uvset' % attrName.split('.')[0])) > 0:
            enabled = False
        cmds.attrFieldGrp('aiImageUVCoords', label='UV Coords', enable=enabled, attribute=attrName)
        cmds.setUITemplate(popTemplate=True)

    def uvcoordsReplace(self, attrName):
        try:
            enabled = True
            if len(cmds.getAttr('%s.uvset' % attrName.split('.')[0])) > 0:
                enabled = False
            cmds.attrFieldGrp('aiImageUVCoords', edit=True, enable=enabled, attribute=attrName)
        except:
            pass

##   In case we need an attribute "imageName" so that colorSpace is updated correctly
#    def imageNameConnect(self, attrName):
#        if not cmds.isConnected(self.nodeAttr('filename'), self.nodeAttr('imageName')):
#            cmds.connectAttr(self.nodeAttr('filename'), self.nodeAttr('imageName'))


    def setup(self):
        self.addSwatch()
        self.beginScrollLayout()
        
        self.beginLayout("Image Attributes", collapse=False)
        self.addCustom('filename', self.filenameNew, self.filenameReplace)
        self.addControl('useFrameExtension', label='Use File Sequence', changeCommand=self.useSequenceChange)
        self.addControl('frame')
        self.addControl("filter", label="Filter")
        
        self.addControl("mipmap_bias", label="Mipmap Bias *") # Not supported in GPU
        self.addControl("multiply", label="Multiply")
        self.addControl("offset", label="Offset")
        # self.addCustom('imageName', self.imageNameConnect, self.imageNameConnect)
    

        cmds.editorTemplate('AEcolorSpaceNew', 'AEcolorSpaceReplace', 'colorSpace', callCustom=True)
        self.addControl("ignoreColorSpaceFileRules", label="Ignore Color Space File Rules")
        self.addControl("autoTx", label="Auto-generate TX Textures")
        self.addSeparator()
        
        self.addControl("ignoreMissingTextures", label="Ignore Missing Textures")
        self.addControl("missingTextureColor", label="Missing Texture Color")
        self.endLayout()
        
        self.beginLayout("UV Coordinates", collapse=True)
        self.beginNoOptimize()
        self.addCustom('uvset', self.uvsetNew, self.uvsetReplace)
        self.addSeparator()
        self.addCustom('uvcoords', self.uvcoordsNew, self.uvcoordsReplace)
        
        self.addControl("soffset", label="Offset U")
        self.addControl("toffset", label="Offset V")
        
        self.addControl("swrap", label="Wrap U")
        self.addControl("twrap", label="Wrap V")
        
        self.addControl("sscale", label="Scale U")
        self.addControl("tscale", label="Scale V")
        
        self.addControl("sflip", label="Flip U")
        self.addControl("tflip", label="Flip V")
        
        self.addControl("swap_st", label="Swap UV")

        self.endNoOptimize()
        self.endLayout()
        
        mel.eval('AEdependNodeTemplate '+self.nodeName)
        self.addExtraControls()
        self.endScrollLayout()

    def useSequenceChange(self, nodeName):

        resolveFilePathSequence( nodeName,
                                'useFrameExtension',
                                'filename',
                                'filenameImageGrp',
                                'frame'
                                )
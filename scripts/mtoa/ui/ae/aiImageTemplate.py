import pymel.core as pm
import mtoa.utils as utils
import mtoa.ui.ae.utils as aeUtils
import maya.cmds as cmds
from mtoa.ui.ae.shaderTemplate import ShaderAETemplate
import maya.mel as mel

class AEaiImageTemplate(ShaderAETemplate):
    def filenameEdit(self, mData) :
        attr = self.nodeAttr('filename')
        cmds.setAttr(attr,mData,type="string")

    def LoadFilenameButtonPush(self, *args):
        basicFilter = 'All Files (*.*)'
        ret = cmds.fileDialog2(fileFilter=basicFilter, dialogStyle=2,cap='Load Image File',okc='Load',fm=4)
        if ret is not None and len(ret):
            self.filenameEdit(ret[0])
            cmds.textFieldButtonGrp("filenameGrp", edit=True, text=ret[0])

    def filenameNew(self, nodeName):
        path = cmds.textFieldButtonGrp("filenameGrp", label="Image Name", changeCommand=self.filenameEdit, width=300)
        cmds.textFieldButtonGrp(path, edit=True, text=cmds.getAttr(nodeName))
        cmds.textFieldButtonGrp(path, edit=True, buttonLabel="...", buttonCommand=self.LoadFilenameButtonPush)

    def filenameReplace(self, nodeName):
        cmds.textFieldButtonGrp( "filenameGrp", edit=True, text=cmds.getAttr(nodeName) )

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
            pass

    def uvsetNew(self, attrName):
        cmds.setUITemplate('attributeEditorPresetsTemplate', pushTemplate=True)
        aeUtils.attrTextFieldGrp('aiImageUVSet', label='UV Set', attribute=attrName, changeCommand=AEaiImageTemplate.editUVSet)
        cmds.setUITemplate(popTemplate=True)

    def uvsetReplace(self, attrName):
        try:
            aeUtils.attrTextFieldGrp('aiImageUVSet', edit=True, attribute=attrName)
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

    def setup(self):
        self.addSwatch()
        self.beginScrollLayout()
        
        self.beginLayout("Image Attributes", collapse=False)
        self.addCustom('filename', self.filenameNew, self.filenameReplace)
        self.addControl("filter", label="Filter")
        
        self.addControl("mipmap_bias", label="Mipmap Bias")
        self.addControl("multiply", label="Multiply")
        self.addControl("offset", label="Offset")      
        
        self.addControl("cache_texture_handles", label="Cache Texture Handles")
        self.endLayout()
        
        self.beginLayout("UV Coordinates", collapse=True)
        self.beginNoOptimize()
        self.addCustom('uvset', self.uvsetNew, self.uvsetReplace)
        self.addSeparator();        
        self.addCustom('uvcoords', self.uvcoordsNew, self.uvcoordsReplace)
        
        self.addControl("swrap", label="Wrap U")
        self.addControl("twrap", label="Wrap V")
        
        self.addControl("sscale", label="Scale U")
        self.addControl("tscale", label="Scale V")
        
        self.addControl("sflip", label="Flip U")
        self.addControl("tflip", label="Flip V")
        
        self.addControl("swap_st", label="Swap UV")

        self.endNoOptimize()
        self.endLayout()
        
        pm.mel.AEdependNodeTemplate(self.nodeName)
        self.addExtraControls()
        self.endScrollLayout()


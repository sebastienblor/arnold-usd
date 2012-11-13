import pymel.core as pm
import mtoa.utils as utils
import mtoa.ui.ae.utils as aeUtils
import maya.cmds as cmds
from mtoa.ui.ae.shaderTemplate import ShaderAETemplate

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

    def setup(self):
        self.addSwatch()
        self.beginScrollLayout()
        
        self.beginLayout("Image Attributes", collapse=False)
        self.addCustom('filename', self.filenameNew, self.filenameReplace)
        self.addControl("filter", label="Filter")
        
        self.addControl("single_channel", label="Only First Channel")
        
        self.addControl("mipmap_bias", label="Mipmap Bias")
        self.addControl("multiply", label="Multiply")
        self.addControl("offset", label="Offset")
        
        self.addControl("cache_texture_handles", label="Cache Texture Handles")
        self.endLayout()
        
        self.beginLayout("UV Coordinates", collapse=True)
        self.addControl("uvcoords", label="UV Coord")
        
        self.addControl("swrap", label="Wrap U")
        self.addControl("twrap", label="Wrap V")
        
        self.addControl("sscale", label="Scale U")
        self.addControl("tscale", label="Scale V")
        
        self.addControl("sflip", label="Flip U")
        self.addControl("tflip", label="Flip V")
        
        self.addControl("swap_st", label="Swap UV")
        
        self.endLayout()
        
        pm.mel.AEdependNodeTemplate(self.nodeName)
        self.addExtraControls()
        self.endScrollLayout()


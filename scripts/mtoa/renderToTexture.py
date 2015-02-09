import maya.cmds as cmds
import maya.utils as utils
import os.path
import glob
import re
import sys, os
import subprocess
import threading
import mtoa.callbacks as callbacks
import maya.OpenMaya as om
import mtoa.utils as mutils

class MtoARenderToTexture(object):
    window = None
    def __new__(cls, *args, **kwargs):
        if not '_instance' in vars(cls):
            cls._instance = super(MtoARenderToTexture, cls).__new__(cls, *args, **kwargs)
        return cls._instance

    def __init__(self):
        if self.window is None:
            self.window = 'MtoARenderToTexture'
            self.listElements = []
            self.column = ''
            
        
        
    def doExport(self):
        outFolder = cmds.textFieldButtonGrp('outputFolder', q=True, tx=True)

        if (outFolder == ''):
            cmds.confirmDialog( title='Render To Texture', message='An Output folder must be selected', button=['Ok'], defaultButton='Ok', cancelButton='Ok', dismissString='Ok' )
            return False


        resolution = cmds.intFieldGrp('resolution', q=True, v1=True)
        aa_sampling = cmds.intFieldGrp('aa_samples', q=True, v1=True)
        
        filter_type = cmds.optionMenuGrp('filter', q=True, v=True)
        filter_width = cmds.floatFieldGrp('filterWidth', q=True, v1=True)
        shader = cmds.textFieldGrp('shader', q=True, tx=True)

        cmds.arnoldRenderToTexture(folder=outFolder, shader=shader, resolution=resolution, aa_samples=aa_sampling, filter=filter_type, filter_width=filter_width )

        cmds.deleteUI(self.window)
        return True

    def doCancel(self):
        cmds.deleteUI(self.window)        
        return True

    def browseObjFilename(self):
        objFilter = "*.obj"
        ret = cmds.fileDialog2(cap='Select Folder',okc='Select',fm=3)
        if ret is not None and len(ret):
            cmds.textFieldButtonGrp('outputFolder', e=True, text=ret[0])

        return True


    def create(self):

        if cmds.window(self.window, exists=True):
            cmds.deleteUI(self.window)
        
        selList = cmds.ls(sl=1)
        winTitle = "Render To Texture :      "
        if (len(selList) == 1):
            winTitle += selList[0]
        else:
            winTitle += str(len(selList))
            winTitle += " Objects"

        self.window = cmds.window(self.window, widthHeight=(300, 150), title=winTitle)
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
        
            
    
    def createUI(self):
        cmds.scrollLayout(childResizable=True,)
        cmds.columnLayout(adjustableColumn=True)
        #cmds.setParent("..")
        cmds.rowLayout(numberOfColumns=1, columnAlign1='left')        
        cmds.textFieldButtonGrp('outputFolder', label='Output Folder', text="", buttonLabel='...', buttonCommand=lambda *args: self.browseObjFilename())

        cmds.setParent("..")
        cmds.rowLayout(numberOfColumns=2, columnAlign2=('left', 'right'))
        cmds.intFieldGrp('resolution', label='Resolution', value1=512, w=200)  
        cmds.intFieldGrp('aa_samples', label='Camera Samples (AA)', value1=3, w=200)
        cmds.setParent("..")

        cmds.rowLayout(numberOfColumns=2, columnAlign2=('left', 'right')) 
        cmds.optionMenuGrp('filter', label='Filter ')
        cmds.menuItem( label='blackman_harris' )
        cmds.menuItem( label='box' )
        cmds.menuItem( label='catrom' )
        cmds.menuItem( label='catrom2d' )
        cmds.menuItem( label='closest' )
        cmds.menuItem( label='cone' )
        cmds.menuItem( label='cook' )
        cmds.menuItem( label='cubic' )
        cmds.menuItem( label='disk' )
        cmds.menuItem( label='farthest' )
        cmds.menuItem( label='gaussian' )
        cmds.menuItem( label='heatmap' )
        cmds.menuItem( label='mitnet' )
        cmds.menuItem( label='sync' )
        cmds.menuItem( label='triangle' )
        cmds.menuItem( label='variance' )
        cmds.menuItem( label='video' )

        cmds.optionMenuGrp('filter', e=True, w=200, v='gaussian')

        cmds.floatFieldGrp('filterWidth', label='Filter Width', w=200, value1=2.0)
        cmds.setParent("..")
        cmds.rowLayout(numberOfColumns=1, columnAlign1='both')        
        cmds.textFieldGrp('shader', label='Shader', text="", w=400)
        cmds.setParent("..")

        cmds.rowLayout(numberOfColumns=4, columnAlign4=('left', 'left', 'left', 'right'))     
        cmds.text( '                                                ')
        
        cmds.button(label='Ok', al='right', w=85, h=25, command=lambda *args: self.doExport())  
        cmds.text( '                  ')
        cmds.button(label='Cancel', al='right', w=85, h=25, command=lambda *args: self.doCancel())              
        cmds.setParent("..")

    
        
        
        

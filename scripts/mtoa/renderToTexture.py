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

defaultFolder = ""

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
        all_udims = cmds.checkBox('all_udims', q=True, v=True)
        filter_width = cmds.floatFieldGrp('filterWidth', q=True, v1=True)
        shader = cmds.textFieldGrp('shader', q=True, tx=True)
        uv_set = cmds.textFieldGrp('uv_set', q=True, tx=True)
        udims = cmds.textFieldGrp('udims', q=True, tx=True)
        normalOffset = cmds.floatFieldGrp('normalOffset', q=True, v1=True)
        enableAovs = cmds.checkBox('enableAovs', q=True, v=True)
        uStart = cmds.floatFieldGrp('uStart', q=True, v1=True)
        uScale = cmds.floatFieldGrp('uScale', q=True, v1=True)
        vStart = cmds.floatFieldGrp('vStart', q=True, v1=True)
        vScale = cmds.floatFieldGrp('vScale', q=True, v1=True)
        extendEdges = cmds.checkBox('extendEdges', q=True, v=True)
        useSequence = cmds.checkBox('sequence', q=True, v=True)
        frameStart = cmds.intFieldGrp('frameStart', q=True, v1=True)
        frameEnd = cmds.intFieldGrp('frameEnd', q=True, v1=True)
        frameStep = cmds.intFieldGrp('frameStep', q=True, v1=True)
        framePadding = cmds.intFieldGrp('framePadding', q=True, v1=True)
        
        selList = cmds.ls(sl=1)

        if (len(selList) == 0):
            cmds.confirmDialog( title='Render To Texture', message='No Geometry Selected', button=['Ok'], defaultButton='Ok', cancelButton='Ok', dismissString='Ok' )
            return False

        cmds.arnoldRenderToTexture(folder=outFolder, shader=shader, resolution=resolution, aa_samples=aa_sampling, filter=filter_type, filter_width=filter_width, all_udims=all_udims, udims=udims, uv_set=uv_set, normal_offset=normalOffset, enable_aovs=enableAovs, extend_edges=extendEdges,u_start=uStart, u_scale=uScale, v_start=vStart, v_scale=vScale, sequence= useSequence, frame_start=frameStart, frame_end=frameEnd, frame_step=frameStep, frame_padding=framePadding)

        cmds.deleteUI(self.window)
        return True

    def doCancel(self):
        cmds.deleteUI(self.window)
        return True

    def browseObjFilename(self):

        global defaultFolder
        if defaultFolder == "":
            defaultFolder = cmds.workspace(q=True,rd=True, fn=True)
        ret = cmds.fileDialog2(cap='Select Folder',okc='Select',fm=3,dir=defaultFolder)
        if ret is not None and len(ret):
            defaultFolder = ret[0]
            cmds.textFieldButtonGrp('outputFolder', e=True, text=defaultFolder)


        return True


    def create(self):

        if cmds.window(self.window, exists=True):
            cmds.deleteUI(self.window)

        winTitle = "Render To Texture"

        self.window = cmds.window(self.window, widthHeight=(460, 320), title=winTitle)
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
        global defaultFolder
        cmds.textFieldButtonGrp('outputFolder', label='Output Folder', cw3=(90,320, 50), text=defaultFolder, buttonLabel='...', buttonCommand=lambda *args: self.browseObjFilename())

        cmds.setParent("..")
        cmds.rowLayout(numberOfColumns=2, columnAlign2=('left', 'right'))
        cmds.intFieldGrp('resolution', label='Resolution', value1=512, ct2=('left', 'left'),  cw2=(90,110), w=230)
        cmds.intFieldGrp('aa_samples', label='Camera Samples (AA)', cw2=(150,60), value1=3, w=200)
        cmds.setParent("..")

        cmds.rowLayout(numberOfColumns=2, columnAlign2=('left', 'right'))
        cmds.optionMenuGrp('filter', label='Filter ')
        cmds.menuItem( label='blackman_harris' )
        cmds.menuItem( label='box' )
        cmds.menuItem( label='catrom' )
        cmds.menuItem( label='closest' )
        cmds.menuItem( label='farthest' )
        cmds.menuItem( label='gaussian' )
        cmds.menuItem( label='heatmap' )
        cmds.menuItem( label='mitnet' )
        cmds.menuItem( label='sinc' )
        cmds.menuItem( label='triangle' )
        cmds.menuItem( label='variance' )

        cmds.optionMenuGrp('filter', e=True, w=230, ct2=('left', 'left'), cw2=(90,110), v='gaussian')

        cmds.floatFieldGrp('filterWidth', label='Filter Width', w=200, ct2=('left', 'left'), cw2=(150,60), value1=2.0)
        cmds.setParent("..")

        cmds.rowLayout(numberOfColumns=1, columnAlign1='both')
        cmds.textFieldGrp('shader', label='Shader Override', ct2=('left', 'left'), cw2=(90,110), text="", w=380)
        cmds.setParent("..")
        cmds.textFieldGrp('uv_set', label='UV Set', ct2=('left', 'left'), cw2=(90,110), text="", w=280)
        cmds.setParent("..")
        cmds.checkBox( 'extendEdges',label='Extend Edges', value=True )
        cmds.setParent("..")

        cmds.rowLayout(numberOfColumns=2, columnAlign2=('left', 'right'))
        cmds.textFieldGrp('udims', label='Udims', ct2=('left', 'left'), cw2=(90,110), text="", w=280)
        cmds.checkBox( 'all_udims',label='All Udims', value=False )
        cmds.setParent("..")

        cmds.rowLayout(numberOfColumns=2, columnAlign2=('left', 'right'))
        cmds.floatFieldGrp('uStart', label='U Start', value1= 0, ct2=('left', 'left'),  cw2=(90,110), w=230)
        cmds.floatFieldGrp('uScale', label='U Scale', value1= 1.0, cw2=(150,60), w=200)
        cmds.setParent("..")
        cmds.rowLayout(numberOfColumns=2, columnAlign2=('left', 'right'))
        cmds.floatFieldGrp('vStart', label='V Start', value1= 0, ct2=('left', 'left'),  cw2=(90,110), w=230)
        cmds.floatFieldGrp('vScale', label='V Scale', value1= 1.0, cw2=(150,60), w=200)
        cmds.setParent("..")

        cmds.checkBox( 'enableAovs',label='Enable AOVs', value=False )
        
        cmds.floatFieldGrp('normalOffset', label='Normal Offset', w=380, ct2=('left', 'left'), cw2=(90,110), value1=0.1)
        cmds.setParent("..")

        cmds.rowLayout(numberOfColumns=5, columnAlign5=('left', 'left', 'left', 'left', 'left'))
        cmds.checkBox( 'sequence',label='Sequence', value=False )
        cmds.intFieldGrp('frameStart', label='Start', value1= 0, ct2=('left', 'left'),  cw2=(30,40), w=70)
        cmds.intFieldGrp('frameEnd', label='End', value1= 10, cw2=(30,40), w=70)
        cmds.intFieldGrp('frameStep', label='Step', value1= 1, cw2=(30,40), w=70)
        cmds.intFieldGrp('framePadding', label='Frame Padding', value1= 0, cw2=(80,40), w=120)
        cmds.setParent("..")

        cmds.rowLayout(numberOfColumns=4, columnAlign4=('left', 'left', 'left', 'right'))
        cmds.text( '                                             ')

        cmds.button(label='Render', al='right', w=85, h=25, command=lambda *args: self.doExport())
        cmds.text( '              ')
        cmds.button(label='Cancel', al='right', w=85, h=25, command=lambda *args: self.doCancel())
        cmds.setParent("..")
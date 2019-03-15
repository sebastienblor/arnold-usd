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


class MtoAExportOperatorGraph(object):
    window = None
    def __new__(cls, *args, **kwargs):
        if not '_instance' in vars(cls):
            cls._instance = super(MtoAExportOperatorGraph, cls).__new__(cls, *args, **kwargs)
        return cls._instance

    def __init__(self):
        if self.window is None:
            self.window = 'MtoAExportOperatorGraph'
            self.listElements = []
            self.column = ''

    def doCancel(self):
        cmds.deleteUI(self.window)
        return True

    def browseObjFilename(self):
        global defaultFolder
        if defaultFolder == "":
            defaultFolder = cmds.workspace(q=True,rd=True, fn=True)
        ret = cmds.fileDialog2(cap='Select File',okc='Select',ff="Arnold Operator Graph (*.ass)",fm=0,dir=defaultFolder) or []
        if ret is not None and len(ret):
            defaultFolder = ret[0]
            cmds.textFieldButtonGrp('Filename', e=True, text=defaultFolder)            
        return True

    def doExport(self):
        filename = cmds.textFieldButtonGrp('Filename', q=True, tx=True)

        if (filename == ''):
            cmds.confirmDialog( title='Export Operator Graph', message='An output .ass file must be selected', button=['Ok'], defaultButton='Ok', cancelButton='Ok', dismissString='Ok' )
            return False

        exportMode = cmds.optionMenuGrp('export', q=True, v=True)
        selection = False

        if exportMode == 'Selection':
            selList = cmds.ls(sl=1) or []
            if len(selList) == 0:
                cmds.confirmDialog( title='Export Operator Graph', message='No selection found', button=['Ok'], defaultButton='Ok', cancelButton='Ok', dismissString='Ok' )
                return False
            selection = True

        exportShaders = cmds.checkBox('exportShaders', q=True, v=True)
        cmds.arnoldExportOperators(filename=filename, selection=selection, shaders=exportShaders)
        cmds.deleteUI(self.window)
        return True
    

    def create(self):

        if cmds.window(self.window, exists=True):
            cmds.deleteUI(self.window)

        winTitle = "Export Operator Graph"

        self.window = cmds.window(self.window, widthHeight=(460, 120), title=winTitle)
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
        cmds.textFieldButtonGrp('Filename', label='Filename', cw3=(90,320, 50), text=defaultFolder, buttonLabel='...', buttonCommand=lambda *args: self.browseObjFilename())
        cmds.setParent("..")    
        cmds.rowLayout(numberOfColumns=1, columnAlign1='left')
        cmds.optionMenuGrp('export', label='Export ')
        cmds.menuItem( label='Selection' )
        cmds.menuItem( label='Scene Graph' )

        selList = cmds.ls(sl=1) or []
        if len(selList) == 0:
            cmds.optionMenuGrp('export', e=True, w=230, ct2=('left', 'left'), cw2=(90,110), v='Scene Graph')
        else:
            cmds.optionMenuGrp('export', e=True, w=230, ct2=('left', 'left'), cw2=(90,110), v='Selection')
        cmds.setParent("..")    
        cmds.rowLayout(numberOfColumns=2, columnAlign2=('left','left'))
        cmds.text(label='                ')
        cmds.checkBox( 'exportShaders',label='Export Assigned Shaders', value=False )
        cmds.setParent("..")

        cmds.rowLayout(numberOfColumns=4, columnAlign4=('left', 'left', 'left', 'right'))
        cmds.text( '                                             ')

        cmds.button(label='Export', al='right', w=85, h=25, command=lambda *args: self.doExport())
        cmds.text( '              ')
        cmds.button(label='Cancel', al='right', w=85, h=25, command=lambda *args: self.doCancel())
        cmds.setParent("..")
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


class MtoAExportToMaterialX(object):
    window = None
    def __new__(cls, *args, **kwargs):
        if not '_instance' in vars(cls):
            cls._instance = super(MtoAExportToMaterialX, cls).__new__(cls, *args, **kwargs)
        return cls._instance

    def __init__(self):
        if self.window is None:
            self.window = 'MtoAExportToMaterialX'
            self.listElements = []
            self.column = ''

    def doCancel(self):
        cmds.deleteUI(self.window)
        return True

    def browseObjFilename(self):
        global defaultFolder
        if defaultFolder == "":
            defaultFolder = cmds.workspace(q=True,rd=True, fn=True)
        ret = cmds.fileDialog2(cap='Select File',okc='Select',ff="MaterialX File (*.mtlx)",fm=0,dir=defaultFolder) or []
        if ret is not None and len(ret):
            defaultFolder = ret[0]
            cmds.textFieldButtonGrp('Filename', e=True, text=defaultFolder)            
        return True

    def doExport(self):
        filename = cmds.textFieldButtonGrp('Filename', q=True, tx=True)

        if (filename == ''):
            cmds.confirmDialog( title='Export to MaterialX', message='An output .mtlx file must be selected', button=['Ok'], defaultButton='Ok', cancelButton='Ok', dismissString='Ok' )
            return False

        lookName = cmds.textFieldGrp('LookName', q=True, tx=True)
        properties = cmds.textFieldGrp('Properties', q=True, tx=True)
        selList = cmds.ls(sl=1)
        if (len(selList) == 0):
            cmds.confirmDialog( title='Export to MaterialX', message='No Geometry Selected', button=['Ok'], defaultButton='Ok', cancelButton='Ok', dismissString='Ok' )
            return False

        cmds.arnoldExportToMaterialX(filename=filename, look=lookName, properties=properties)
        cmds.deleteUI(self.window)
        return True
    

    def create(self):

        if cmds.window(self.window, exists=True):
            cmds.deleteUI(self.window)

        winTitle = "Export to MaterialX"

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
        cmds.textFieldButtonGrp('Filename', label='Filename', cw3=(90,320, 50), text=defaultFolder, buttonLabel='...', buttonCommand=lambda *args: self.browseObjFilename())

        cmds.setParent("..")
        cmds.textFieldGrp('LookName', label='Look Name', ct2=('left', 'left'), cw2=(90,320), text="", w=380)
        cmds.setParent("..")
        cmds.textFieldGrp('Properties', label='Properties', ct2=('left', 'left'), cw2=(90,320), text="", w=380)
        cmds.setParent("..")

        cmds.rowLayout(numberOfColumns=4, columnAlign4=('left', 'left', 'left', 'right'))
        cmds.text( '                                             ')

        cmds.button(label='Export', al='right', w=85, h=25, command=lambda *args: self.doExport())
        cmds.text( '              ')
        cmds.button(label='Cancel', al='right', w=85, h=25, command=lambda *args: self.doCancel())
        cmds.setParent("..")
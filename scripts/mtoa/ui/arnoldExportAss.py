import maya.cmds as cmds
import maya.mel as mel
from mtoa.ui.callback import *
from mtoa.ui.arnoldAssOpts import arnoldAssOpts
import os

def doExportAssOpts():
    if (cmds.radioButtonGrp('arnoldExportAssSelected', query=True, select=True) == 1):
       mel.eval('ExportOptions')
       # mel.eval('fileOptions "ExportAll" "projectViewer ExportAll"')
       # mel.eval('setCurrentFileTypeOption "ExportAll" "actionOptionsForm" "ArnoldSceneSource"')
    else :
       mel.eval('ExportSelectionOptions')
       # mel.eval('fileOptions "ExportActive" "projectViewer ExportActive"')
       # mel.eval('setCurrentFileTypeOption "ExportActive" "actionOptionsForm" "ArnoldSceneSource"')    

def doExportAss():
    if (cmds.radioButtonGrp('arnoldExportAssSelected', query=True, select=True) == 1):
       mel.eval('Export')
       # mel.eval('fileOptions "ExportAll" "projectViewer ExportAll"')
       mel.eval('setCurrentFileTypeOption "ExportAll" "actionOptionsForm" "ArnoldSceneSource"')
    else :
       mel.eval('ExportSelection')
       # mel.eval('fileOptions "ExportActive" "projectViewer ExportActive"')
       mel.eval('setCurrentFileTypeOption "ExportActive" "actionOptionsForm" "ArnoldSceneSource"')    


    
def doExportAssAndClose(window):
    doExportAssOpts()
    cmds.deleteUI(window, window=True)

def arnoldExportAss(*args):

    # Make sure the ArnoldRenderOptions node exists
    if not cmds.ls('defaultArnoldRenderOptions'):
        cmds.createNode('ArnoldRenderOptions', skipSelect=True, shared=True, name='defaultArnoldRenderOptions')
        
    window = cmds.window(title='Export to .ass Options', resizeToFitChildren=True)

    cmds.columnLayout(adjustableColumn=True,
                      columnOffset=("both", 20),
                      rowSpacing=10)
    cmds.radioButtonGrp('arnoldExportAssSelected',
         numberOfRadioButtons=2,
         select=1,
                        labelArray2=['All', 'Selected'])

    cmds.separator(style='none')

    cmds.rowLayout(numberOfColumns=2)
    cmds.button(width=80, label='Export', command=Callback(doExportAssAndClose, window))
    cmds.button(width=80, label='Close', command=Callback(cmds.deleteUI, window, window=True))
    cmds.setParent('..')
    
    cmds.showWindow(window)

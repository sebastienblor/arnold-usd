import maya.cmds as cmds
import maya.mel as mel
from mtoa.ui.callback import *


def storeMaskValues():
    mask = 0

    if cmds.checkBox('oa_export_options', q=True, value=True):
        mask += 1
    if cmds.checkBox('oa_export_cameras', q=True, value=True):
        mask += 2
    if cmds.checkBox('oa_export_lights', q=True, value=True):
        mask += 4
    if cmds.checkBox('oa_export_shapes', q=True, value=True):
        mask += 8
    if cmds.checkBox('oa_export_shaders', q=True, value=True):
        mask += 16
    if cmds.checkBox('oa_export_override', q=True, value=True):
        mask += 32
    if cmds.checkBox('oa_export_drivers', q=True, value=True):
        mask += 64
    if cmds.checkBox('oa_export_filters', q=True, value=True):
        mask += 128

    cmds.setAttr('defaultArnoldRenderOptions.output_ass_mask', mask)

def readMaskValues():
    mask = cmds.getAttr('defaultArnoldRenderOptions.output_ass_mask')

    cmds.checkBox('oa_export_filters', edit=True, value=(mask / 128))
    mask = mask % 128
    cmds.checkBox('oa_export_drivers', edit=True, value=(mask / 64))
    mask = mask % 64
    cmds.checkBox('oa_export_override', edit=True, value=(mask / 32))
    mask = mask % 32
    cmds.checkBox('oa_export_shaders', edit=True, value=(mask / 16))
    mask = mask % 16
    cmds.checkBox('oa_export_shapes', edit=True, value=(mask / 8))
    mask = mask % 8
    cmds.checkBox('oa_export_lights', edit=True, value=(mask / 4))
    mask = mask % 4
    cmds.checkBox('oa_export_cameras', edit=True, value=(mask / 2))
    mask = mask % 2
    cmds.checkBox('oa_export_options', edit=True, value=mask)


def arnoldAssOpts(parent = '', action = '', initialSettings = '', resultCallback = ''):
    #print 'parent: %(p)s, action: %(a)s, settings: %(s)s, callback: %(c)s\n' % \
    #  {"p": parent, "a": action, "s": initialSettings, "c": resultCallback}

    retval = 0
    currentOptions = ''

    # Make sure the aiOptions node exists
    if not cmds.ls('defaultArnoldRenderOptions'):
        cmds.createNode('aiOptions', skipSelect=True, shared=True, name='defaultArnoldRenderOptions')

    if action == 'post':
        cmds.setParent(parent)

        #cmds.connectControl('oa_filename', 'defaultArnoldRenderOptions.output_ass_filename', fileName=True)

        cmds.checkBox('oa_compressed', label='Use Compression')
        cmds.connectControl('oa_compressed', 'defaultArnoldRenderOptions.output_ass_compressed')

        cmds.checkBox('oa_write_bbox', label='Write Bounding Box')

        cmds.separator(style='none')

        cmds.text('Export Node Types')
        cmds.frameLayout(label='')
        cmds.columnLayout(adjustableColumn=True,
                       columnOffset=('both', 10),
                       rowSpacing=10)

        cmds.checkBox('oa_export_options', align='left', label='Export Options Node', value=True)
        cmds.checkBox('oa_export_cameras', align='left', label='Export Cameras', value=True)
        cmds.checkBox('oa_export_lights', align='left', label='Export Lights', value=True)
        cmds.checkBox('oa_export_shapes', align='left', label='Export Shapes', value=True)
        cmds.checkBox('oa_export_shaders', align='left', label='Export Shaders', value=True)
        cmds.checkBox('oa_export_override', align='left', label='Export Override Nodes', value=True)
        cmds.checkBox('oa_export_drivers', align='left', label='Export Drivers', value=True)
        cmds.checkBox('oa_export_filters', align='left', label='Export Filters', value=True)

        readMaskValues()
        retval = 1

    elif action == 'query':
        # TODO: make this more optionVar compliant ?
        storeMaskValues()

        # If more options are used, the callback expects them to be ";" separated
        if cmds.checkBox('oa_write_bbox', q=True, value=True):
            currentOptions = '-bb'

        #print 'callback: %(c)s, options: %(o)s\n' % {"c": resultCallback, "o": currentOptions}
        mel.eval(resultCallback+'("'+currentOptions+'")')
        retval = 1

    else:
        retval = 0

    return retval

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

    # Make sure the aiOptions node exists
    if not cmds.ls('defaultArnoldRenderOptions'):
        cmds.createNode('aiOptions', skipSelect=True, shared=True, name='defaultArnoldRenderOptions')

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

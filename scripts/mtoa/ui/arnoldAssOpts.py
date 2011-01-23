import maya.cmds as cmds
import maya.mel as mel
from mtoa.ui.callback import *
import os

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
                            
    print 'parent: %(p)s, action: %(a)s, settings: %(s)s, callback: %(c)s\n' % \
        {"p": parent, "a": action, "s": initialSettings, "c": resultCallback}                            
                            
    retval = 0
    currentOptions = ''
    
    # Make sure the ArnoldRenderOptions node exists
    if not cmds.ls('defaultArnoldRenderOptions'):
        cmds.createNode('ArnoldRenderOptions', skipSelect=True, shared=True, name='defaultArnoldRenderOptions')
    
    if (action == 'post') :
        cmds.setParent(parent);

        cmds.columnLayout('assTypeCol', adjustableColumn=True)
        cmds.radioButtonGrp('lepTypeGrp',
                            label='Place holder, does nothing',
                            numberOfRadioButtons=2,
                            labelArray2=['True', 'False'])
        cmds.setParent(parent)
        cmds.columnLayout(  'assExportOptions',
                            adjustableColumn=True,
                              columnOffset=("both", 20),
                              rowSpacing=10)
    
        cmds.checkBox('oa_compressed', label='Use Compression')
        cmds.connectControl('oa_compressed', 'defaultArnoldRenderOptions.output_ass_compressed')
    
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
                    
        # Now set to current settings.
        
        # make this more optionVar compliant
        readMaskValues()
        
        currentOptions = initialSettings;
        optionList = currentOptions.split(';')
        for option in optionList :
            keyName, sep, keyValue = option.partition('=')            
            if (keyName == 'showPositions') :
                cmds.radioButtonGrp('lepTypeGrp', edit=True, select=(1 if (keyValue!='0') else 2))

        retval = 1;   
    elif (action == 'query') :
    
        # make this more optionVar compliant
        storeMaskValues()
    
        currentOptions += 'showPositions='
        currentOptions += '1' if (cmds.radioButtonGrp('lepTypeGrp', query=True, select=True) == 1) else '0'

        print ('cmd: '+resultCallback+' "'+currentOptions+'"\n')    

        mel.eval(resultCallback+' "'+currentOptions+'"')
        
        retval = 1      
    else :
        retval = 0
    
     
    return retval

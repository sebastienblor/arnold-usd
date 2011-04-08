import maya.cmds as cmds
import maya.mel as mel
from mtoa.ui.callback import *
import os

from mtoa.ui.createArnoldRendererCommonGlobalsTab import createArnoldRendererCommonGlobalsTab
from mtoa.ui.createArnoldRendererGlobalsTab import createArnoldRendererGlobalsTab


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

    if action == 'post':

        # Build the UI
        cmds.setParent(parent)

        form = cmds.formLayout('assExportForm')

        tabLayoutName = 'assExportTab'

        # tabLayoutName = cmds.tabLayout(tabLayoutName, tabsVisible=True, width=1)
        tabs = cmds.tabLayout('assExportTab')
        cmds.formLayout(form , edit=True, attachForm=((tabs, 'top', 0), (tabs, 'left', 0), (tabs, 'bottom', 0), (tabs, 'right', 0)) )

        # -preSelectCommand "fillSelectedTabForCurrentRenderer"

        cmds.setParent(tabs)

        commonForm = cmds.formLayout('assExportCommon')
        createArnoldRendererCommonGlobalsTab()

        cmds.setParent(tabs)

        arnoldForm = cmds.formLayout('assExportArnold')
        createArnoldRendererGlobalsTab()

        cmds.setParent(tabs)
        cmds.tabLayout( tabs, edit=True, tabLabel=((commonForm, 'Common'), (arnoldForm, 'Arnold Renderer')) )

        retval = 1

    elif action == 'query':

        # make this more optionVar compliant
        # storeMaskValues()

        #currentOptions += 'showPositions='
        #currentOptions += '1' if (cmds.radioButtonGrp('lepTypeGrp', query=True, select=True) == 1) else '0'

        mel.eval(resultCallback+' "'+currentOptions+'"')
        retval = 1

    else:
        retval = 0

    return retval

'''
 Copyright (C) 1997-2010 Autodesk, Inc., and/or its licensors.
 All rights reserved.

 The coded instructions, statements, computer programs, and/or related
 material (collectively the "Data") in these files contain unpublished
 information proprietary to Autodesk, Inc. ("Autodesk") and/or its licensors,
 which is protected by U.S. and Canadian federal copyright law and by
 international treaties.

 The Data is provided for use exclusively by You. You have the right to use,
 modify, and incorporate this Data into other products for purposes authorized
 by the Autodesk software license agreement, without fee.
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND. AUTODESK
 DOES NOT MAKE AND HEREBY DISCLAIMS ANY EXPRESS OR IMPLIED WARRANTIES
 INCLUDING, BUT NOT LIMITED TO, THE WARRANTIES OF NON-INFRINGEMENT,
 MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE, OR ARISING FROM A COURSE
 OF DEALING, USAGE, OR TRADE PRACTICE. IN NO EVENT WILL AUTODESK AND/OR ITS
 LICENSORS BE LIABLE FOR ANY LOST REVENUES, DATA, OR PROFITS, OR SPECIAL,
 DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES, EVEN IF AUTODESK AND/OR ITS
 LICENSORS HAS BEEN ADVISED OF THE POSSIBILITY OR PROBABILITY OF SUCH DAMAGES.
'''

import os
import math
import re

import maya.cmds as cmds
import mtoa.utils as utils
from mtoa.ui.ae.templates import createTranslatorMenu
from mtoa.callbacks import *
import mtoa.core as core
import mtoa.aovs as aovs
import maya.OpenMayaRender
import maya.mel as mel
import mtoa.melUtils as mu

from maya.app.stereo import stereoCameraRig

MENU_SEPARATOR = ('-', None)

PLE_MAX_Y =  768
PLE_MAX_X = 1024

# Use global variables because MEL do not have a concept of enum or
# constants. These values are used to define the action to take in
# the renderable camera menus.
CAM_MENU_CAMERA     = 1
CAM_MENU_STEREOPAIR = 2
CAM_MENU_ADD        = 3
CAM_MENU_IGNORE     = 4




def _listStereoRigs():
    return stereoCameraRig.listRigs(True) or []
def _isMono(camera):
    return not stereoCameraRig.rigRoot(camera)

def getStereoLeftCamera(camera):
    result = cmds.listConnections('{}.leftCam'.format(camera), d=False, s=True)                
    if result and len(result):
        return result[0]
    else:
        return None
def getStereoRightCamera(camera):
    result = cmds.listConnections('{}.leftCam'.format(camera), d=False, s=True)                
    if result and len(result):
        return result[0]
    else:
        return None

def getMultiCameraChildren(camera):
    cameras = []
    if cmds.pluginInfo("stereoCamera", query=True, loaded=True):
        import maya.app.stereo.stereoCameraRig as stereoCameraRig
        if stereoCameraRig.isRigRoot(str(camera)):
            try:
                lCam = getStereoLeftCamera(camera)
                if lCam:
                    cameras.append(lCam)
                    rCam = getStereoRightCamera(camera)
                    if rCam:
                        cameras.append(rCam)
            except IndexError:
                cmds.warning("Stereo camera %s is missing required connections" % camera)
    return cameras

def fileTypeToExtension(fileType):
   if (fileType == "jpeg") :
       return "jpg"
   elif (fileType == "tiff") :
       return "tif"
   else :
       return fileType


# ----------------------------------------------------------------------------
# Utility procedures used by other procedures in this file.
# Must be used to account for multiple instances of the same tab.
#
def setParentToArnoldCommonTab():
    # First set the parent to the correct tab layout.
    # Account for the special "all renderers" master layer layout
    # when we are using render layers
    if mel.eval("isDisplayingAllRendererTabs"):
        renderer = mu.getVar('gMasterLayerRendererName', init=True)
    else:
        renderer = utils.currentRenderer()

    if renderer == 'arnold':
        tabLayout = mel.eval('rendererTabLayoutName "arnold"')
        cmds.setParent(tabLayout)
    
        # Now set the parent to the correct column layout
        cmds.setParent('commonTabColumn')

def createArnoldTargetFilePreview():

    oldParent = cmds.setParent(query=True)

    cmds.columnLayout('targetFilePreview', adjustableColumn=True)

    cmds.text('exampleText0',
              align="left",
              font="smallBoldLabelFont",
              label=mel.eval("uiRes m_createMayaSoftwareCommonGlobalsTab.kPath"))

    cmds.text('exampleText1',
              align="left",
              font="smallBoldLabelFont",
              label=mel.eval("uiRes m_createMayaSoftwareCommonGlobalsTab.kFileName"))

    cmds.text('exampleText2',
              align="left",
              font="smallBoldLabelFont",
              label=mel.eval("uiRes m_createMayaSoftwareCommonGlobalsTab.kTo"))

    cmds.text('exampleText3',
              align="left",
              font="smallBoldLabelFont",
              label=mel.eval("uiRes m_createMayaSoftwareCommonGlobalsTab.kImageSize"))

    cmds.setParent(oldParent)

    # This target file preview is affected by a number of attributes.
    # If any of those attributes change, this preview needs to be updated.
    #
    # Here we fill an array with the names of all of the current renderer's
    # attributes which affect the naming of the target file.
    #
    attrArray = ["defaultRenderGlobals.imageFilePrefix",
                 "defaultRenderGlobals.outFormatControl",
                 "defaultRenderGlobals.imfPluginKey",
                 "defaultRenderGlobals.outFormatExt",
                 "defaultRenderGlobals.animation",
                 "defaultRenderGlobals.byFrameStep",
                 "defaultRenderGlobals.extensionPadding",
                 "defaultRenderGlobals.startFrame",
                 "defaultRenderGlobals.endFrame",
                 "defaultRenderGlobals.modifyExtension",
                 "defaultRenderGlobals.startExtension",
                 "defaultRenderGlobals.byExtension",
                 "defaultRenderGlobals.periodInExt",
                 "defaultResolution.fields",
                 "defaultRenderGlobals.fieldExtControl",
                 "defaultRenderGlobals.oddFieldExt",
                 "defaultRenderGlobals.evenFieldExt",
                 "defaultRenderGlobals.putFrameBeforeExt",
                 "defaultResolution.width",
                 "defaultResolution.height",
                 "defaultResolution.dotsPerInch",
                 "defaultResolution.imageSizeUnits",
                 "defaultResolution.pixelDensityUnits",
                 "defaultRenderGlobals.renderVersion",
                 "defaultArnoldRenderOptions.aovMode",
                 "defaultArnoldDriver.mergeAOVs"]

    # Now we establish scriptJobs to invoke the procedure which updates the
    # target file preview when any of the above attributes change.
    for attr in attrArray:
        cmds.scriptJob(attributeChange = (attr,updateArnoldTargetFilePreview),
                        parent='targetFilePreview')


    cmds.scriptJob(event = ('workspaceChanged',
                            updateArnoldTargetFilePreview),
                            parent='targetFilePreview')

    updateArnoldTargetFilePreview()

def updateArnoldTargetFilePreview(*args):
    '''
    Description:
    This procedure is called any time an attribute change occurs which
       would affect the name(s) of the file(s) that would be created when the
       user performs a render.
    This procedure updates the lines of text in the General tab that allow
    the user to see what files are going to be created when they render.
    '''

    oldParent = cmds.setParent(query=True)

    if mel.eval("isDisplayingAllRendererTabs"):
        renderer = mu.getVar('gMasterLayerRendererName', init=True)
    else:
        renderer = utils.currentRenderer()

    tabLayout = mel.eval("rendererTabLayoutName \""+renderer+"\"")
    if cmds.tabLayout(tabLayout, exists=True):
        cmds.setParent(tabLayout)

    #
    # Update the File Name portion of the preview.
    #

    title1 = mel.eval("uiRes m_createMayaSoftwareCommonGlobalsTab.kNewFileName")
    title2 = mel.eval("uiRes m_createMayaSoftwareCommonGlobalsTab.kNewTo")

    kwargs = {}
    tokens = {}
    try:
        prefix = cmds.getAttr('defaultRenderGlobals.imageFilePrefix')
    except:
        pass
    else:
        if prefix:
            kwargs['path'] = prefix

    kwargs['createDirectory'] = False
    kwargs['leaveUnmatchedTokens'] = True

    if not cmds.objExists('defaultArnoldRenderOptions'):
        return

    aovsEnabled = cmds.getAttr('defaultArnoldRenderOptions.aovMode') and aovs.getAOVs(enabled=True, exclude=['beauty', 'RGBA', 'RGB'])

    if aovsEnabled:
        tokens['RenderPass'] = '<RenderPass>'
    kwargs['strictAOVs'] = not (aovsEnabled and not cmds.getAttr('defaultArnoldDriver.mergeAOVs'))
    tokens['Frame'] = cmds.getAttr('defaultRenderGlobals.startFrame')
    first = utils.getFileName('relative', tokens, **kwargs)

    if os.path.isabs(first):
        # the entered path is absolute so there is no prepended path
        cmds.text('exampleText0', edit=True, label='')
    else:
        #
        # Update the Path portion of the preview.
        #
    
        # get the project's image directory
        #
        imgDir = cmds.workspace(fileRuleEntry="images")
        fullPath = cmds.workspace(expandName=imgDir)
        pathLabel = mel.eval("uiRes m_createMayaSoftwareCommonGlobalsTab.kNewPath")
        path = cmds.format(pathLabel, s=fullPath)
        cmds.text('exampleText0', edit=True, label=path)

    cmds.text('exampleText1', edit=True, label=cmds.format(title1, s=first))
    settings = maya.OpenMayaRender.MCommonRenderSettingsData()
    maya.OpenMayaRender.MRenderUtil.getCommonRenderSettings(settings)
    if settings.isAnimated():
        tokens['Frame'] = cmds.getAttr('defaultRenderGlobals.endFrame')
        last = utils.getFileName('relative', tokens, **kwargs)
        cmds.text('exampleText2', edit=True, label=cmds.format(title2, s=last))
    else:
        cmds.text('exampleText2', edit=True, label="")

    #
    # Update the Image Size portion of the preview.
    #

    # Get attributes
    #
    width = cmds.getAttr('defaultResolution.width')
    height = cmds.getAttr('defaultResolution.height')
    dpi = cmds.getAttr('defaultResolution.dotsPerInch')
    sizeUnits = cmds.getAttr('defaultResolution.imageSizeUnits')
    resUnits = cmds.getAttr('defaultResolution.pixelDensityUnits')

    # Default measurement units to inches if pixels selected
    if sizeUnits == 0:
        sizeUnits = 1

    gMeasurementUnitsNames = mu.getVar('gMeasurementUnitsNames', type='string[]', init=True)
    gResolutionUnitsNames = mu.getVar('gResolutionUnitsNames', type='string[]', init=True)
    
    if not gResolutionUnitsNames:
        mel.eval("source \"resolutionFormats.mel\"")
        gMeasurementUnitsNames =  mu.getVar('gMeasurementUnitsNames', type= 'string[]')
        gResolutionUnitsNames = mu.getVar('gResolutionUnitsNames', type='string[]')

    # Convert from pixels to the correct measurement units
    inchWidth = mel.eval('convertPixelsToInches({}, {})'.format(width, dpi))
    inchHeight = mel.eval('convertPixelsToInches({}, {})'.format(height, dpi))
    docWidth = mel.eval('convertMeasurement({}, \"inches\", \"{}\")'.format(inchWidth, gMeasurementUnitsNames[sizeUnits]))
    docHeight = mel.eval('convertMeasurement({}, \"inches\",\"{}\")'.format(inchHeight, gMeasurementUnitsNames[sizeUnits]))

    # Convert from DPI to the correct resolution units
    res = mel.eval('convertResolutionMeasurement({}, \"pixels/inch\", \"{}\")'.format(dpi,gResolutionUnitsNames[resUnits]))

    # Convert to strings, rounding applicable floats to 1 decimal place
    imW = width
    imH = height

    docW = mel.eval('setDecimalPrecision({}, 1.0)'.format(docWidth))
    docH = mel.eval('setDecimalPrecision({}, 1.0)'.format(docHeight))
    units = mel.eval('resolutionFormats_melToUI(\"{}\")'.format(gMeasurementUnitsNames[sizeUnits]))
    resVal = mel.eval('setDecimalPrecision({}, 1.0)'.format(res))

    resUnitsStr = mel.eval('resolutionFormats_melToUI(\"{}\")'.format(gResolutionUnitsNames[resUnits]))

    imageLabel = mel.eval("uiRes m_createMayaSoftwareCommonGlobalsTab.kNewImageSize")
    imageSizeString = cmds.format(imageLabel, s=(imW, imH, docW, docH, units, resVal, resUnitsStr))

    cmds.text('exampleText3', edit=True, label=imageSizeString)

    cmds.setParent(oldParent)


def insertArnoldKeywordMenuCallback(token):

    setParentToArnoldCommonTab()

    # if not yet set, then replace name with token
    prefix = cmds.textFieldGrp('mayaSoftwareFileName', query=True, text=True)
    if prefix == mel.eval('uiRes("m_createMayaSoftwareCommonGlobalsTab.kNotSetUsingFilename")'):
        cmds.textFieldGrp('mayaSoftwareFileName', e=True, text=token, forceChangeCommand=True)
    else:
        cmds.textFieldGrp('mayaSoftwareFileName', e=True, insertText=token, forceChangeCommand=True)


def createArnoldInsertKeywordMenu(parent):

    cmds.popupMenu(parent, edit=True, deleteAllItems=True)

    cmds.setParent(parent, menu=True)

    cmds.menuItem(label=mel.eval("uiRes m_createMayaSoftwareCommonGlobalsTab.kFileNameKeywords"), enable=0)
    cmds.menuItem(divider=True)
    cmds.menuItem(label=mel.eval("uiRes m_createMayaSoftwareCommonGlobalsTab.kKeywordScene"),
                  command=lambda *args: insertArnoldKeywordMenuCallback("<Scene>"))
    cmds.menuItem(label=mel.eval("uiRes m_createMayaSoftwareCommonGlobalsTab.kKeywordLayer"),
                  command=lambda *args:insertArnoldKeywordMenuCallback("<RenderLayer>"))
    cmds.menuItem(label=mel.eval("uiRes m_createMayaSoftwareCommonGlobalsTab.kKeywordCamera"),
                  command=lambda *args:insertArnoldKeywordMenuCallback("<Camera>"))
    cmds.menuItem(label=mel.eval("uiRes m_createMayaSoftwareCommonGlobalsTab.kKeywordRPFG"),
                  command=lambda *args:insertArnoldKeywordMenuCallback("<RenderPassFileGroup>"))
    cmds.menuItem(label=mel.eval("uiRes m_createMayaSoftwareCommonGlobalsTab.kKeywordRenderPass"),
                  command=lambda *args:insertArnoldKeywordMenuCallback("<RenderPass>"))
    cmds.menuItem(label=mel.eval("uiRes m_createMayaSoftwareCommonGlobalsTab.kKeywordRenderPassType"),
                  command=lambda *args:insertArnoldKeywordMenuCallback("<RenderPassType>"))
    cmds.menuItem(label=mel.eval("uiRes m_createMayaSoftwareCommonGlobalsTab.kKeywordExtension"),
                  command=lambda *args:insertArnoldKeywordMenuCallback("<Extension>"))
    cmds.menuItem(label=mel.eval("uiRes m_createMayaSoftwareCommonGlobalsTab.kKeywordVersion"),
                  command=lambda *args:insertArnoldKeywordMenuCallback("<Version>"))
    date = cmds.date(format="YY_MM_DD")
    cmds.menuItem(label=(mel.eval("uiRes m_createMayaSoftwareCommonGlobalsTab.kKeywordDate") + date),
                  command=lambda arg=None, x=date:insertArnoldKeywordMenuCallback(x))
    time = cmds.date(format="hh-mm-ss")
    cmds.menuItem(label=(mel.eval("uiRes m_createMayaSoftwareCommonGlobalsTab.kKeywordTime") + time),
                  command=lambda arg=None, x=time:insertArnoldKeywordMenuCallback(x))

# ----------------------------------------------------------------------------
# Code to create and update the Image File Output frame
#
def createArnoldFileNamePrefixControl():

    # Create the control
    #
    cmds.textFieldGrp('mayaSoftwareFileName',
                     label=mel.eval("uiRes m_createMayaSoftwareCommonGlobalsTab.kFileNamePrefix"),
                     annotation=mel.eval("uiRes m_createMayaSoftwareCommonGlobalsTab.kFileNamePrefixAnn"))

    popup = cmds.popupMenu(parent='mayaSoftwareFileName|field')
    cmds.popupMenu(popup, edit=True, postMenuCommand=Callback(createArnoldInsertKeywordMenu, popup))

    # connect the label, so we can change its color
    cmds.connectControl('mayaSoftwareFileName', 'defaultRenderGlobals.imageFilePrefix', index=1)
    cmds.connectControl('mayaSoftwareFileName', 'defaultRenderGlobals.imageFilePrefix', index=2)

    # Create a scriptJob which will update the control when the value of the
    # attribute it represents is changed.
    #
    cmds.scriptJob(parent='mayaSoftwareFileName',
                 attributeChange=("defaultRenderGlobals.imageFilePrefix", updateArnoldFileNamePrefixControl))


def changeArnoldFileNamePrefix(*args):

    oldParent = cmds.setParent(query=True)
    setParentToArnoldCommonTab()

    prefix = cmds.textFieldGrp('mayaSoftwareFileName', query=True, text=True)
    prefixAttr = "defaultRenderGlobals.imageFilePrefix"

    if prefix != mel.eval("uiRes m_createMayaSoftwareCommonGlobalsTab.kNotSetUsingFilename") and mel.eval("isValidFileNamePrefix \""+prefix+"\""):
        # The user has set the prefix to something, and it is a valid name, so
        # we will set the value of the corresponding attribute.
        #
        cmds.setAttr(prefixAttr, prefix, type="string")
    else:
        # The user has set the prefix to an invalid value. We will refresh the
        # UI to show the current value, which has not been changed.
        #
        updateArnoldFileNamePrefixControl()

    cmds.setParent(oldParent)

def updateArnoldFileNamePrefixControl(*args):
    #
    #  Procedure Name:
    #      changeFileName
    #
    #  Description:
    #    This procedure is called when the user changes the file
    #    prefix.  It sets the internal representation of the prefix
    #    and then updates the example to show the changes.
    #
    oldParent = cmds.setParent(query=True)
    setParentToArnoldCommonTab()

    prefix = cmds.getAttr("defaultRenderGlobals.imageFilePrefix")

    if prefix:
        cmds.textFieldGrp('mayaSoftwareFileName', edit=True, text=prefix)
    else:
        cmds.textFieldGrp('mayaSoftwareFileName',
                    edit=True,
                    text=mel.eval("uiRes m_createMayaSoftwareCommonGlobalsTab.kNotSetUsingFilename"))

    cmds.setParent(oldParent)


def createArnoldFileNameFormatControl():

    cmds.optionMenuGrp('extMenu',
                    label= mel.eval("uiRes m_createMayaSoftwareCommonGlobalsTab.kFrameAnimationExt"),
                    changeCommand=changeArnoldFileNameFormat)


    cmds.menuItem(label=mel.eval("uiRes m_createMayaSoftwareCommonGlobalsTab.kExt1"))
    cmds.menuItem(label=mel.eval("uiRes m_createMayaSoftwareCommonGlobalsTab.kExt2"))
    cmds.menuItem('mayaSoftwareNameDotFrameDotExtension', label=mel.eval("uiRes m_createMayaSoftwareCommonGlobalsTab.kExt3"))
    cmds.menuItem('mayaSoftwareNameDotExtensionDotFrame', label=mel.eval("uiRes m_createMayaSoftwareCommonGlobalsTab.kExt4"))
    cmds.menuItem('mayaSoftwareNameDotFrame', label=mel.eval("uiRes m_createMayaSoftwareCommonGlobalsTab.kExt5"))
    cmds.menuItem('mayaSoftwareFrameDotExtension', label=mel.eval("uiRes m_createMayaSoftwareCommonGlobalsTab.kExt6"))
    cmds.menuItem('mayaSoftwareNameUnderFrameDotExtension', label=mel.eval("uiRes m_createMayaSoftwareCommonGlobalsTab.kExt7"))
    cmds.menuItem('mayaSoftwareMultiFrame', label=mel.eval("uiRes m_createMayaSoftwareCommonGlobalsTab.kExt8"))
    cmds.menuItem('mayaSoftwareMultiFrameDotExtension', label=mel.eval("uiRes m_createMayaSoftwareCommonGlobalsTab.kExt9"))

    attrArray=[]
    attrArray.append("defaultRenderGlobals.outFormatControl")
    attrArray.append("defaultRenderGlobals.animation")
    attrArray.append("defaultRenderGlobals.periodInExt")
    attrArray.append("defaultRenderGlobals.putFrameBeforeExt")
    attrArray.append("defaultRenderGlobals.imageFormat")
    attrArray.append("defaultRenderGlobals.imfPluginKey")

    # Now we establish scriptJobs to invoke the procedure which updates the
    # file name format control when any of the above attributes change.
    #
    for attr in attrArray:
        cmds.scriptJob(attributeChange=(attr, updateArnoldFileNameFormatControl),
                     parent='extMenu')


def changeArnoldFileNameFormat(*args):
    '''
    Procedure Name:
        changeExtension
    
    Description:
      This procedure is called when the user changes the format
      of the file extension.  It sets the internal representation
      and then updates the example to show the changes.
    
    Note:
      Although the user sees only one control to change the
      extension, it actually affects more than one value.
    '''
    oldParent = cmds.setParent(query=True)
    setParentToArnoldCommonTab()

    item = cmds.optionMenuGrp('extMenu', q=True, sl=True)

    mel.eval('setMayaSoftwareFrameExt(\"{}\", 0)'.format(fileTypeToExtension(item)))

    # Update the batch render window if it exists
    #
    if mel.eval("exists \"updateBatchRenderWindowTitle\""):
        mel.eval("updateBatchRenderWindowTitle")

    cmds.setParent(oldParent)

def updateArnoldFileNameFormatControl(*args):

    oldParent = cmds.setParent(query=True)

    setParentToArnoldCommonTab()

    frameBeforeExt  = cmds.getAttr("defaultRenderGlobals.putFrameBeforeExt")
    useAnim         = cmds.getAttr("defaultRenderGlobals.animation")
    imageUse        = cmds.getAttr("defaultRenderGlobals.outFormatControl")
    period          = cmds.getAttr("defaultRenderGlobals.periodInExt")

    if cmds.getAttr('defaultRenderGlobals.imageFormat') == 31: # Check if PSD format
        multiframe = 0
        psdFormat = 1
    else:
        imfType = mel.eval('getImfImageType()')
        multiframe = mel.eval('multiframeFormat(\"{}\")'.format(imfType))
        psdFormat = 0
    activeMenuItem = 0

    # Update Frame/Animation Ext menuItems and enable only the relevant ones.
    #
    notMultiFrameOrPsd = not multiframe or psdFormat

    cmds.menuItem('mayaSoftwareNameDotFrameDotExtension', edit=True, enable=notMultiFrameOrPsd)
    cmds.menuItem('mayaSoftwareNameDotExtensionDotFrame', edit=True, enable=notMultiFrameOrPsd)
    cmds.menuItem('mayaSoftwareNameDotFrame', edit=True, enable=notMultiFrameOrPsd)
    cmds.menuItem('mayaSoftwareFrameDotExtension', edit=True, enable=notMultiFrameOrPsd)
    cmds.menuItem('mayaSoftwareNameUnderFrameDotExtension', edit=True, enable=notMultiFrameOrPsd)
    cmds.menuItem('mayaSoftwareMultiFrame',edit=True, enable=multiframe)
    cmds.menuItem('mayaSoftwareMultiFrameDotExtension',edit=True, enable=multiframe)

    if multiframe:
        if useAnim:
            if imageUse == 1:     # no extension
                activeMenuItem = 8
            else:
                activeMenuItem = 9
        else:
            if imageUse == 1:     # no extension
                activeMenuItem = 1
            else:
                activeMenuItem = 2
    else:
        if useAnim:
            if imageUse == 1:
                activeMenuItem = 5
            else:
                if frameBeforeExt == 0:
                    activeMenuItem = 4
                else:
                    if period == 1: # period in extension
                        activeMenuItem = 3
                    elif period == 2: # underscore in extension
                        activeMenuItem = 7
                    else: # $period == 0
                        activeMenuItem = 6
        else:
            if imageUse == 1:
                activeMenuItem = 1
            else:
                activeMenuItem = 2

    cmds.optionMenuGrp('extMenu', edit=True, sl=activeMenuItem)

    # Also update the frame number controls to enable/disable them according
    # to whether or not they are being used.
    #
    updateArnoldFrameNumberControls()

    cmds.setParent(oldParent)

def createArnoldUseCustomExtensionControl():

    cmds.checkBoxGrp('useCustomExtensionCtrl',
                   numberOfCheckBoxes=1,
                   label='',
                   label1=mel.eval("uiRes m_createMayaSoftwareCommonGlobalsTab.kUseCustomExtension"),
                   cc=changeArnoldUseCustomExtension)

    cmds.scriptJob(parent='useCustomExtensionCtrl',
                 attributeChange=("defaultRenderGlobals.outFormatControl", updateArnoldUseCustomExtensionControl))

def updateArnoldUseCustomExtensionControl():
    
    oldParent = cmds.setParent(query=True)
    setParentToArnoldCommonTab();
    useImage = cmds.getAttr('defaultRenderGlobals.outFormatControl') !=  1

    cmds.checkBoxGrp('useCustomExtensionCtrl',
                   e=True,
                   value1=cmds.getAttr('defaultRenderGlobals.outFormatControl') == 2,
                   enable=useImage)

    cmds.setParent(oldParent)

def changeArnoldUseCustomExtension(*args, **kwargs):
    #  Procedure Name:
    #      changeCustomExtensionCheck
    #
    #  Description:
    #		This procedure is called when the user turns the custom
    #		extension on or off.  It sets the internal representation
    #		and then updates the example to show the changes.
    #
    oldParent = cmds.setParent(query=True)
    setParentToArnoldCommonTab();
    isOn = cmds.checkBoxGrp('useCustomExtensionCtrl', query=True, value1=True)
    if isOn:
        cmds.setAttr('defaultRenderGlobals.outFormatControl', 2)
    else:
        # We have to figure out if there should be an extension
        # at all or not.
        #
        item = cmds.optionMenuGrp('extMenu', query=True, select=True)
        
        if item == 1 or item == 5:
            cmds.setAttr('defaultRenderGlobals.outFormatControl', 1)
        else:
            cmds.setAttr('defaultRenderGlobals.outFormatControl', 0)
        
        cmds.setParent(oldParent)

def createArnoldCustomExtensionControl():

    cmds.attrControlGrp('userExt',
                      label=mel.eval("uiRes m_createMayaSoftwareCommonGlobalsTab.kExtension"),
                      attribute='defaultRenderGlobals.outFormatExt')
    
    cmds.connectControl('userExt', 'defaultRenderGlobals.outFormatExt', index=1)

    cmds.scriptJob(parent='userExt',
                 attributeChange=('defaultRenderGlobals.outFormatControl', updateArnoldCustomExtensionControl))

def updateArnoldCustomExtensionControl():
    oldParent = cmds.setParent(query=True)
    
    setParentToArnoldCommonTab();
    
    useImage = cmds.getAttr('defaultRenderGlobals.outFormatControl') != 1
    value1 = cmds.getAttr('defaultRenderGlobals.outFormatControl') == 2
    useExt = useImage and value1
    
    cmds.attrControlGrp('userExt', edit=True, enable=useExt)
    
    cmds.setParent(oldParent)

def createArnoldImageFormatControl():

    cRenderer = utils.currentRenderer()
    if cRenderer == "mentalRay":
        return mel.eval("createMRImageFormatControl")
    if cRenderer == "mayaSoftware":
        return mel.eval("createMayaImageFormatControl")

    parent = cmds.setParent(query=True)

    # Delete the control if it already exists
    #
    fullPath = "%s|imageMenuMayaSW" % parent
    if cmds.layout(fullPath, exists=True):
        cmds.deleteUI(fullPath)

    # TODO: connect node to options
    createTranslatorMenu('defaultArnoldDriver', 
                         label=mel.eval("uiRes m_createMayaSoftwareCommonGlobalsTab.kImageFormatMenu"),
                         nodeType='aiAOVDriver',
                         default='exr',
                         optionMenuName='imageMenuMayaSW')


    maya_version = cmds.about(version=True)
    if int(float(maya_version)) >= 2016:
        cmds.attrEnumOptionMenuGrp( l='Color Space',
                            at='defaultArnoldDriver.colorManagement' )
    
    # We need to create controls that we don't need to avoid
    # Maya errors because of the harcoded code. keep them hidden
    cmds.columnLayout('cl_output_compression', vis=0, rowSpacing=0)
    cmds.button('renderGlobalsCompression', label="", enable=False)
    cmds.attrEnumOptionMenuGrp('multiCamNamingMenu', label="")
    cmds.textFieldGrp('multiCamCustomToken', label="")
    cmds.setParent('..')

    cmds.scriptJob(
        parent=parent,
        attributeChange=("defaultArnoldDriver.aiTranslator",
                         updateArnoldImageFormatControl))


    if int(float(maya_version)) >= 2016:

        cmds.scriptJob(
          parent=parent,
          attributeChange=("defaultArnoldDriver.aiTranslator",
                           updateArnoldColorSpace))

        cmds.scriptJob(
            parent=parent,
            attributeChange=("defaultArnoldDriver.tiffFormat",
                             updateArnoldColorSpace))

#    changeArnoldImageFormat()
    return "imageMenuMayaSW"


def updateArnoldColorSpace(*args):
    maya_version = cmds.about(version=True)
    if int(float(maya_version)) < 2016:
        return

    curr = cmds.getAttr('defaultArnoldDriver.aiTranslator')
    if curr == "jpeg" or curr == "png":
        cmds.setAttr('defaultArnoldDriver.colorManagement', 1)
        return
    
    if curr == "exr":
        cmds.setAttr('defaultArnoldDriver.colorManagement', 2)
        return

    if curr == "deepexr":
        cmds.setAttr('defaultArnoldDriver.colorManagement', 0)
        return

    if curr == "tif":
        tiffFormat = cmds.getAttr('defaultArnoldDriver.tiffFormat')
        if tiffFormat == 0:
            cmds.setAttr('defaultArnoldDriver.colorManagement', 1)
        else:
            cmds.setAttr('defaultArnoldDriver.colorManagement', 2)



def updateArnoldImageFormatControl(*args):

    core.createOptions()
    curr = cmds.getAttr('defaultArnoldDriver.aiTranslator')
    cmds.setAttr('defaultRenderGlobals.imageFormat', 51)
    cmds.setAttr('defaultRenderGlobals.imfkey', str(curr), type="string")
    

def extendToShape(cam):
    if cam is None:
        return None

    try:
      camShapes = cmds.listRelatives(cam, shapes=True)
      if camShapes and len(camShapes):
          return camShapes[0]
      else:
          return None
    except AttributeError:
        return None

def getCameras():
    '''
    Return a tuple of (ortho, mono, stereo) camera lists, converting camera
    shapes to transforms
    '''
    ortho = cmds.listCameras(orthographic=True) or []
    mono = []
    stereo = []
    # List all mono perspective cameras first
    for camera in cmds.listCameras(perspective=True) or []:
        if _isMono(camera):
            # Ensure to use its shape node
            mono.append(camera)
        else:
            stereo.append(camera)
    return ortho, mono, stereo

def arnoldCameraMaskChange(ui, camera, mask_name):
    val = cmds.checkBoxGrp(ui, q=True, value1=True)
    if _isMono(camera):
        cmds.setAttr('{}.{}'.format(camera, mask_name), val)
    else:

        lCam = getStereoLeftCamera(camera)
        if lCam:
            cmds.setAttr('{}.{}'.format(lCam, mask_name), val)
        rCam = getStereoRightCamera(camera)
        if rCam:
            cmds.setAttr('{}.{}'.format(rCam, mask_name), val)
        

def arnoldChangedCamera(camera, cameraMode, menu):
    '''
    callback used when the user changed a renderable camera.
    camera is the previous camera
    cameraMode was its type
    menu is the menu item used for this change
    '''

    data = CAM_MENU_CAMERA

    if menu:
        sel = cmds.optionMenuGrp(menu, q=True, select=True) - 1
        items = cmds.optionMenuGrp(menu, q=True, itemListShort=True)
        data = cmds.menuItem(items[sel], query=True, data=True)


    if data == CAM_MENU_IGNORE:
        # Make sure to reselect the first entry, in case user clicked
        # on the separator.
        cmds.optionMenuGrp(menu, edit=True, select=1)
        return

    newCamNeedLayerAdj = False
    currentLayer = cmds.editRenderLayerGlobals(q=True, currentRenderLayer=True)
    isBaseLayer = not cmds.getAttr(currentLayer + '.identification')

    # If replacing a camera, start by making the selected camera
    # non-renderable.
    if data in [CAM_MENU_CAMERA, CAM_MENU_STEREOPAIR]:
        if cameraMode == CAM_MENU_IGNORE:
            # There was no previous renderable camera,
            # always create adjustments if not on the master layer.
            if not isBaseLayer:
                newCamNeedLayerAdj = True
        else:
            if cameraMode == CAM_MENU_CAMERA:
                cameras = [camera]
            else:
                cameras = getMultiCameraChildren(camera)

            for cam in cameras:
                camShape = extendToShape(cam)
                if camShape:
                    # Create adjustments if we are not on the master layer
                    if not isBaseLayer and not newCamNeedLayerAdj:
                        # If the source had an adjustment, create one on the
                        # new camera as well.
                        if cmds.listConnections('{}.renderable'.format(camShape), d=False, s=True):
                            newCamNeedLayerAdj = True;
                    cmds.setAttr('{}.renderable'.format(camShape), False)

    elif data == CAM_MENU_ADD:
        # Create adjustments if not on the master layer.
        if not isBaseLayer:
            newCamNeedLayerAdj = True

    # Now process the new value
    if menu != "":
        new = cmds.optionMenuGrp(menu, query=True, value=True)
        cameras = []
        if data == CAM_MENU_CAMERA:
            cameras.append(new)
        elif data == CAM_MENU_STEREOPAIR:
            pairStr = mel.eval("uiRes m_createMayaSoftwareCommonGlobalsTab.kStereoPair")
            stereoCam = new[:-len(pairStr)]
            cameras = getMultiCameraChildren(stereoCam)
        elif data == CAM_MENU_ADD:
            # Mark renderable the first non renderable camera we can find
            allCameraShapes = cmds.ls(cameras=True)
            for cameraShape in allCameraShapes:
                if not cmds.getAttr('{}.renderable'.format(cameraShape)):
                    cameras.append(cameraShape)
                    break

        # Now make the new cameras renderable
        for cam in cameras:
            if newCamNeedLayerAdj:
                cmds.editRenderLayerAdjustment('{}.renderable'.format(cam))
            cmds.setAttr('{}.renderable'.format(cam), True)
            
    # Finally force recomputing the UI
    cmds.evalDeferred(updateArnoldCameraControl)

def setArnoldCheckboxFromAttr(camera, chkbox, attr):
    if cmds.attributeQuery('stereoRigType', node=camera, exists=True):
        try:
            camera = getStereoLeftCamera(camera)
            if camera is None:
                return
        except IndexError:
            return
    val = cmds.getAttr('{}.{}'.format(camera, attr))
    cmds.checkBoxGrp(chkbox, e=True, value1=val)


def updateArnoldCameraControl(*args):

    mel.eval('global string $gRenderableCameraListMenu')
    
    oldParent = cmds.setParent(query=True)

    setParentToArnoldCommonTab()
    cameraLayout = cmds.setParent('mayaSoftwareCameraLayout')

    # Unmanage the layout while we edit it, it will update faster
    cmds.columnLayout(cameraLayout, edit=True, visible=0)
    cmds.setUITemplate('attributeEditorTemplate', pushTemplate=True)

    # Empty the layout first
    for child in cmds.columnLayout(cameraLayout, query=True, childArray=True) or []:
        cmds.deleteUI(child)

    # Populate all stereo cameras and mono camera
    orthoCams, monoCams, stereoCams = getCameras()

    renderableCameras = []
    nonRenderableCameras = [MENU_SEPARATOR]

    # List all mono perspective cameras first
    for camera in monoCams:
        if cmds.getAttr('{}.renderable'.format(camera)):
            renderableCameras.append((camera, False))
        else:
            nonRenderableCameras.append((camera, False))

    # Remove the separator if nothing was added.
    if nonRenderableCameras and nonRenderableCameras[-1] == MENU_SEPARATOR:
        nonRenderableCameras.pop()

    # list all stereo cameras rigs, and the mono cameras in the rig.
    rigs = _listStereoRigs()
    if rigs:
        for rig in rigs:
            nonRenderableCameras.append(MENU_SEPARATOR)
            # rig.leftCam.get() does not work in Maya2011
            try:
                lCam = extendToShape(getStereoLeftCamera(rig))
                rCam = extendToShape(getStereoRightCamera(rig))
            except IndexError:
                cmds.warning("Stereo camera %s is missing required connections" % rig)
                continue
            cameras = cmds.listRelatives(rig, type="camera", allDescendents=True) or []
            # Add an entry for the rig pair if at least one cam is not
            # renderable. Use the + character to mark it.
            skipLR = False
            if cmds.getAttr('{}.renderable'.format(lCam)) and cmds.getAttr('{}.renderable'.format(rCam)):
                renderableCameras.append((rig, True))
                skipLR = True
            else:
                nonRenderableCameras.append((rig, True))

            for camShape in cameras:
                camParents = cmds.listRelatives(camShape, parent=True) or []
                if len(camParents) and cmds.getAttr('{}.renderable'.format(camShape)):
                    camera = camParents[0]
                    if (camShape == lCam or camShape == rCam):
                        if not skipLR:
                            renderableCameras.append((camera, False))
                        else:
                            nonRenderableCameras.append((camera, False))
                    else:
                        renderableCameras.append((camera, False))
                else:
                    nonRenderableCameras.append((camera, False))
                
        # Remove the separator if nothing was added.
        if nonRenderableCameras and nonRenderableCameras[-1] == MENU_SEPARATOR:
            nonRenderableCameras.pop()

    # List all the ortho cameras
    nonRenderableCameras.append(MENU_SEPARATOR)

    for camera in orthoCams:
        # Ensure to use its shape node
        if cmds.getAttr('{}.renderable'.format(camera)):
            renderableCameras.append((camera, False))
        else:
            nonRenderableCameras.append((camera, False))

    # Remove the separator if nothing was added
    if nonRenderableCameras and nonRenderableCameras[-1] == MENU_SEPARATOR:
        nonRenderableCameras.pop()

    # If there is no renderable camera on this layer, add a fake
    # entry so that users can switch to an existing one.
    if not renderableCameras:
        isFakeCam = True
        renderableCameras.append((mel.eval("uiRes m_createMayaSoftwareCommonGlobalsTab.kNoRenderableCamSelect"), False))
    else:
        isFakeCam = False

    cmds.columnLayout()
    for s, (camera, isStereo) in enumerate(renderableCameras):
        if s % 10 == 9:
            cmds.setParent('..')
            cmds.columnLayout()

        cmds.columnLayout()

        if s > 0:
            cmds.separator()

        if isFakeCam:
            cameraMode = CAM_MENU_IGNORE
        elif isStereo:
            cameraMode = CAM_MENU_STEREOPAIR
        else:
            cameraMode = CAM_MENU_CAMERA

        cmds.rowLayout(nc=2, cw2=(340, 30), cl2=("left", "right"))
        optMenu = cmds.optionMenuGrp(cw=(1, 141), label="Renderable Camera")

        cmds.optionMenuGrp(optMenu,
                         edit=True,
                         changeCommand=lambda arg=None, x=camera, y=cameraMode, z=optMenu: arnoldChangedCamera(x, y, z))

        # The first item is the current renderable camera
        if cameraMode == CAM_MENU_STEREOPAIR:
            thisCamLabel = '%s%s'%(camera, mel.eval("uiRes m_createMayaSoftwareCommonGlobalsTab.kStereoPair"))
        else:
            thisCamLabel = str(camera)

        cmds.menuItem(label=thisCamLabel, data=CAM_MENU_IGNORE)
        # Save this as a global variable for others to access
        mu.setVar('gRenderableCameraListMenu', str(optMenu))
        
        # Insert cameras
        for nonRenderableCamera, isStereo2 in nonRenderableCameras:
            if (nonRenderableCamera, isStereo2) == MENU_SEPARATOR:
                cmds.menuItem(divider=1, data=CAM_MENU_IGNORE)
            elif isStereo2:
                # Stereo rig
                lab = '%s%s'%(nonRenderableCamera, mel.eval("uiRes m_createMayaSoftwareCommonGlobalsTab.kStereoPair"))
                cmds.menuItem(label=lab, data=CAM_MENU_STEREOPAIR)
            else:
                # Mono camera.
                cmds.menuItem(label=str(nonRenderableCamera), data=CAM_MENU_CAMERA)

        # Insert add menuItem
        if not isFakeCam and nonRenderableCamera:
            cmds.menuItem(divider=1, data=CAM_MENU_IGNORE)
            cmds.menuItem(label=mel.eval("uiRes m_createMayaSoftwareCommonGlobalsTab.kAddRenderCam"), data=CAM_MENU_ADD)

        if not isFakeCam:
            # connect the label, so we can change its color
            camShape = extendToShape(camera)
            if camShape:
                cmds.connectControl(optMenu, "%s.renderable"%camShape, index=1)

            if len(renderableCameras) > 1:
                cmds.iconTextButton(style="iconOnly",
                                    image="removeRenderable.png",
                                    annotation=mel.eval("uiRes m_createMayaSoftwareCommonGlobalsTab.kNonRendCam"),
                                    width=20,
                                    height=20,
                                    command=lambda arg=None, x=camera, y=cameraMode: arnoldChangedCamera(x, y, ''))

        cmds.setParent('..')

        if not isFakeCam:
            cmds.columnLayout()
            chkbox = cmds.checkBoxGrp(numberOfCheckBoxes=1,
                                    label=mel.eval("uiRes m_createMayaSoftwareCommonGlobalsTab.kAlphaChannel"))
            cmds.checkBoxGrp(chkbox, e=True, cc=lambda arg=None, x=chkbox, y=camera:arnoldCameraMaskChange(x, y, 'mask'))
            setArnoldCheckboxFromAttr(camera, chkbox, "mask")
            cmds.connectControl(chkbox, "%s.mask"%camShape, index=1)
            cmds.connectControl(chkbox, "%s.mask"%camShape, index=2)
            chkbox = cmds.checkBoxGrp(numberOfCheckBoxes=1,
                                    label=mel.eval("uiRes m_createMayaSoftwareCommonGlobalsTab.kDepthChannel"))
            cmds.checkBoxGrp(chkbox, e=True, cc=lambda arg=None, x=chkbox, y=camera:arnoldCameraMaskChange(x, y, 'depth'))
            setArnoldCheckboxFromAttr(camera, chkbox, "depth")
            cmds.connectControl(chkbox, "%s.depth"%camShape, index=1)
            cmds.connectControl(chkbox, "%s.depth"%camShape, index=2)

            cmds.setParent('..')
        cmds.setParent('..')
    cmds.setParent('..')

    #
    #  Invoke any user supplied code. This callback is published and
    #  needs to remain consistent in future versions of Maya.
    #
    if mel.eval("exists \"renderableCameraListUserCallback\""):
        # Use catchQuiet in case no callback is supplied, we don't
        # want that to show an error.
        mel.eval('catchQuiet( eval("source \"renderableCameraListUserCallback\"")')

    cmds.setParent('..')

    cmds.setUITemplate('attributeEditorTemplate', popTemplate=True)
    cmds.columnLayout(cameraLayout, edit=True, visible=1)

    cmds.setParent(oldParent)

    updateArnoldTargetFilePreview()

def updateArnoldFrameNumberControls(*args):

    oldParent = cmds.setParent(query=True)
    setParentToArnoldCommonTab()

    useAnim      = cmds.getAttr("defaultRenderGlobals.animation")
    useCustomExt = cmds.getAttr("defaultRenderGlobals.modifyExtension")
    imfType = mel.eval('getImfImageType()')
    multiframe = mel.eval('multiframeFormat(\"{}\")'.format(imfType))

    cmds.attrControlGrp('startFrameCtrl',
                        edit=True,
                        enable=useAnim)
    cmds.attrControlGrp('endFrameCtrl',
                        edit=True,
                        enable=useAnim)
    cmds.attrControlGrp('byFrameStepCtrl',
                        edit=True,
                        enable=useAnim)
    cmds.attrControlGrp('skipExistingFramesCtrl',
                        edit=True,
                        enable=useAnim)
    cmds.attrControlGrp('extensionPaddingCtrl',
                        edit=True,
                        enable=(useAnim and not multiframe))
    cmds.attrControlGrp('modifyExtensionCtrl',
                        edit=True,
                        enable=(useAnim and not multiframe))
    cmds.attrControlGrp('startExtensionCtrl',
                        edit=True,
                        enable=(useAnim and useCustomExt and not multiframe))
    cmds.attrControlGrp('byExtensionCtrl',
                        edit=True,
                        enable=(useAnim and useCustomExt and not multiframe))

    cmds.setParent(oldParent)

def createArnoldRenderVersionKeywordMenu(parent):

    cmds.popupMenu(parent, edit=True, deleteAllItems=True)
    cmds.setParent(parent, menu=True)
    
    cmds.menuItem(label=mel.eval("uiRes m_createMayaSoftwareCommonGlobalsTab.kVersionTitle"), enable=0)
    cmds.menuItem(divider=True)
    
    val = cmds.textFieldGrp('renderVersionCtrl', q=True, text=True)
    ival, ival2 = ('1', '2')
    match = re.search('^(\d+)|(\d+)$', val)
    if match:
        i = int(match.group())
        if i > 0:
            ival2 = val.replace(str(i), str(i + 1))
            ival = val.replace(str(i), str(i - 1))
        else:
            ival = '1'

    formatString = mel.eval("uiRes m_createMayaSoftwareCommonGlobalsTab.kVersionNumber")
    cmds.menuItem(label=formatString.replace('^1s', ival),
                command=Callback(cmds.textFieldGrp, 'renderVersionCtrl', e=True, text=ival, forceChangeCommand=True))

    cmds.menuItem(label=formatString.replace('^1s', ival2),
                command=Callback(cmds.textFieldGrp, 'renderVersionCtrl', e=True, text=ival2, forceChangeCommand=True))

    date = cmds.date(format="YY_MM_DD")
    cmds.menuItem(label=mel.eval("uiRes m_createMayaSoftwareCommonGlobalsTab.kVersionDate").replace('^1s', date),
                command=Callback(cmds.textFieldGrp, 'renderVersionCtrl', e=True, text=date, forceChangeCommand=True))
    
    time = cmds.date(format="hh-mm-ss")
    cmds.menuItem(label=mel.eval("uiRes m_createMayaSoftwareCommonGlobalsTab.kVersionTime").replace('^1s', time),
                command=Callback(cmds.textFieldGrp, 'renderVersionCtrl', e=True, text=time, forceChangeCommand=True))

def updateArnoldRenderVersionControl():
    oldParent = cmds.setParent(query=True)
    setParentToArnoldCommonTab();
    
    version = cmds.getAttr('defaultRenderGlobals.renderVersion')
    version = '' if not version else version
    cmds.textFieldGrp('renderVersionCtrl', edit=True, text=version)
    
    cmds.setParent(oldParent)

def changeArnoldRenderVersion(*args, **kwargs):
    oldParent = cmds.setParent(query=True)
    setParentToArnoldCommonTab();
    
    version = cmds.textFieldGrp('renderVersionCtrl', query=True, text=True)
    cmds.setAttr('defaultRenderGlobals.renderVersion', version, type="string")
    
    cmds.setParent(oldParent)

def createArnoldCommonImageFile():
    '''
    Procedure Name:
        createArnoldCommonImageFile
    
    Description:
        Creates the UI in the "Image File Output" expand/collapse section.
      This section is always created so is treated differently
      then the sections created when the tab is expanded.
    '''

    parent = cmds.setParent(query=True)

    # Delete the control if it already exists
    #
    fullPath = "%s|imageFileOutputSW"%parent
    if cmds.layout(fullPath, exists=True):
        cmds.deleteUI(fullPath)

    cmds.setUITemplate('attributeEditorTemplate', pushTemplate=True)

    cmds.columnLayout('imageFileOutputSW', adjustableColumn=True)

    createArnoldFileNamePrefixControl()

    createArnoldImageFormatControl()

    createArnoldFileNameFormatControl()


    cmds.attrControlGrp('extensionPaddingCtrl',
                        attribute='defaultRenderGlobals.extensionPadding',
                        label=mel.eval("uiRes m_createMayaSoftwareCommonGlobalsTab.kFramePadding"),
                        hideMapButton=True)
    
    cmds.separator()
     
    createArnoldUseCustomExtensionControl()
    createArnoldCustomExtensionControl()
    
    cmds.textFieldGrp('renderVersionCtrl',
                    label=mel.eval("uiRes m_createMayaSoftwareCommonGlobalsTab.kVersionLabel"),
                    annotation=mel.eval("uiRes m_createMayaSoftwareCommonGlobalsTab.kVersionLabelAnn"),
                    cc=changeArnoldRenderVersion)
    popup = cmds.popupMenu(parent='renderVersionCtrl|field')
        
    cmds.popupMenu(popup, edit=True, postMenuCommand=Callback(createArnoldRenderVersionKeywordMenu, popup))
    cmds.connectControl('renderVersionCtrl', 'defaultRenderGlobals.renderVersion', index=1)
    cmds.scriptJob(parent='renderVersionCtrl',
                 attributeChange=('defaultRenderGlobals.renderVersion', updateArnoldRenderVersionControl))
    
    updateArnoldRenderVersionControl()
    updateArnoldUseCustomExtensionControl()
    updateArnoldCustomExtensionControl()

    cmds.setParent(parent)
    cmds.setUITemplate(popTemplate=True)

    # Perform an initial update of the UI created above, so that controls
    # which are not directly connected to attributes are properly initialized.
    #
    updateArnoldFileNamePrefixControl()
    updateArnoldImageFormatControl()


def createArnoldCommonFrameRange():
    '''
      Procedure Name:
          createArnoldCommonFrameRange
    
      Description:
          Creates the UI in the "Frame Range" expand/collapse section.
        This section is always created so is treated differently
        then the sections created when the tab is expanded.
    '''

    parent = cmds.setParent(query=True)

    # Delete the control if it already exists
    #
    fullPath = "%s|frameRangeSW"%parent
    if cmds.layout(fullPath, exists=True):
        cmds.deleteUI(fullPath)

    cmds.setUITemplate('attributeEditorTemplate', pushTemplate=True)

    cmds.columnLayout('frameRangeSW', adjustableColumn=True)

    cmds.attrControlGrp('startFrameCtrl',
                        attribute='defaultRenderGlobals.startFrame',
                        label=mel.eval("uiRes m_createMayaSoftwareCommonGlobalsTab.kStartFrame"),
                        hideMapButton=True)

    cmds.attrControlGrp('endFrameCtrl',
                        attribute='defaultRenderGlobals.endFrame',
                        label=mel.eval("uiRes m_createMayaSoftwareCommonGlobalsTab.kEndFrame"),
                        hideMapButton=True)

    cmds.attrControlGrp('byFrameStepCtrl',
                        attribute='defaultRenderGlobals.byFrameStep',
                        label=mel.eval("uiRes m_createMayaSoftwareCommonGlobalsTab.kByFrame"),
                        hideMapButton=True)

    cmds.attrControlGrp('skipExistingFramesCtrl',
                        attribute='defaultRenderGlobals.skipExistingFrames',
                        label=mel.eval("uiRes m_createMayaSoftwareCommonGlobalsTab.kSkipExistingFrames"),
                        hideMapButton=True)

    cmds.separator()

    cmds.checkBoxGrp('modifyExtensionCtrl',
                     cc=updateArnoldFrameNumberControls,
                     label=mel.eval("uiRes m_createMayaSoftwareCommonGlobalsTab.kRenumberFramesUsing"))

    cmds.connectControl('modifyExtensionCtrl', 'defaultRenderGlobals.modifyExtension', index=1)
    cmds.connectControl('modifyExtensionCtrl', 'defaultRenderGlobals.modifyExtension', index=2)

    cmds.attrControlGrp('startExtensionCtrl',
                        attribute='defaultRenderGlobals.startExtension',
                        enable=cmds.getAttr('defaultRenderGlobals.modifyExtension'),
                        hideMapButton=True,
                        label=mel.eval("uiRes m_createMayaSoftwareCommonGlobalsTab.kStartNumber"))

    cmds.attrControlGrp('byExtensionCtrl',
                        attribute='defaultRenderGlobals.byExtension',
                        enable=cmds.getAttr('defaultRenderGlobals.modifyExtension'),
                        hideMapButton=True,
                        label=mel.eval("uiRes m_createMayaSoftwareCommonGlobalsTab.kRenumberByFrame"))

    cmds.setParent(parent)
    cmds.setUITemplate(popTemplate=True)

    # Perform an initial update of the UI created above, so that controls
    # which are not directly connected to attributes are properly initialized.
    #
    updateArnoldFileNameFormatControl()


def createArnoldCommonRenderCameras():
    '''
    Procedure Name:
        createArnoldCommonRenderCameras
    
    Description:
        Creates the UI in the "Renderable Cameras" expand/collapse section.
      This section is always created so is treated differently
      then the sections created when the tab is expanded.
    '''

    parent = cmds.setParent(query=True)

    # Delete the control if it already exists
    #
    fullPath = "%s|renderableCamerasSW"%parent
    if cmds.layout(fullPath, exists=True):
        cmds.deleteUI(fullPath)


    cmds.setUITemplate('attributeEditorTemplate', pushTemplate=True)

    cmds.columnLayout('renderableCamerasSW', adjustableColumn=True)

    # Cameras ------------------------------------------------
    cmds.columnLayout('mayaSoftwareCameraLayout')
    updateArnoldCameraControl()
    cmds.setParent('..')

    cmds.setParent(parent)
    cmds.setUITemplate(popTemplate=True)

# ----------------------------------------------------------------------------
# Code to create and update the Resolution frame
#

def createArnoldCommonResolution():
    '''
    Procedure Name:
        createArnoldCommonResolution
    
    Description:
        Creates the UI in the "Resolution" expand/collapse section.
    '''

    #
    # Make sure the list of predefined resolutions has been read in.
    #
    gImageFormatData = mu.getVar('gImageFormatData', type='string[]', init=True)
    gUserImageFormatData = mu.getVar('gUserImageFormatData', type='string[]', init=True)

    if not gImageFormatData:
        mel.eval("source imageFormats.mel")
        gImageFormatData = mu.getVar('gImageFormatData', type='string[]')


    if not mel.eval('exists imageFormats_melToUI'):
        mel.eval('source imageFormats.mel')
        gUserImageFormatData = mu.getVar('gUserImageFormatData', type='string[]')


    gResolutionUnitsNames = mu.getVar('gResolutionUnitsNames', type='string[]', init=True)
    gMeasurementUnitsNames = mu.getVar('gMeasurementUnitsNames', type='string[]', init=True)

    if not gResolutionUnitsNames:
        mel.eval("source resolutionFormats.mel")
        gResolutionUnitsNames = mu.getVar('gResolutionUnitsNames', type='string[]')

    isMayaEvalVersion = cmds.about(ev=True)
    gPLEImageFormatData = []
    if isMayaEvalVersion:
        gImageFormatData = gPLEImageFormatData

    if mel.eval("exists userImageFormats.mel") and len(gUserImageFormatData) == 0:
        # Yes, we need the eval here, to avoid doing the source
        # until we know whether the file actually exists
        mel.eval('catchQuiet( eval("userImageFormats.mel"))')
        gUserImageFormatData = mu.getVar('gUserImageFormatData', type='string[]')

    cmds.setUITemplate('attributeEditorTemplate', pushTemplate=True)

    parent = cmds.setParent(q=True)
    # If the UI is created already then just update the attribute values.
    if cmds.columnLayout("%s|rgResolutionLayout"%parent, exists=True):
        updateArnoldResolution()
        return

    cmds.columnLayout('rgResolutionLayout', adjustableColumn=True)
    resItem = 1
    numResolutionPresets = len(gImageFormatData)
    allResNodes = cmds.ls(type='resolution')
    numResolutionNodePresets = len(allResNodes) - 1
    gImageFormatDividerPosition = mu.getVar('gImageFormatDividerPosition', type='int', init=True)
    cmds.optionMenuGrp('resolutionMenu',
                     label=mel.eval("uiRes m_createMayaSoftwareCommonGlobalsTab.kPresets"),
                     changeCommand=changeArnoldResolution)

    cmds.menuItem(label=mel.eval("uiRes m_createMayaSoftwareCommonGlobalsTab.kCustom"))
    for resItem in range(0, numResolutionPresets):

        if resItem == gImageFormatDividerPosition:
            cmds.menuItem(label="---------------------", enable=False)
        else:
            item = gImageFormatData[resItem]
            tokens = item.split(' ')
            numTokens = len(tokens)

            # Change any underscore into a space;
            # some names may have up to 2 underscores in them,
            # so we do this twice.
            #
            niceName = tokens[0].replace(' ', '_').replace('"', '\\"')
            uiName = mel.eval("imageFormats_melToUI \""+niceName+"\"")
            cmds.menuItem(label=uiName)

    for item in gUserImageFormatData:
        tokens = item.split(' ')

        # Change any underscore into a space;
        # some names may have up to 2 underscores in them,
        # so we do this twice.
        #
        niceName = tokens[0].replace(' ', '_').replace('"', '\"')
        cmds.menuItem(label=niceName)

    for resItem in range(0, numResolutionNodePresets):
        cmds.menuItem(label=allResNodes[resItem + 1])

    cmds.separator()

    cmds.checkBoxGrp('aspectLockCheck',
                     numberOfCheckBoxes=1,
                     label="",
                     label1=mel.eval("uiRes m_createMayaSoftwareCommonGlobalsTab.kMaintainWidthHeightRatio"))

    cmds.connectControl('aspectLockCheck', 'defaultResolution.aspectLock', index=2)

    cmds.radioButtonGrp('ratioLockRadio',
                      numberOfRadioButtons=2,
                      vertical=True,
                      label=mel.eval("uiRes m_createMayaSoftwareCommonGlobalsTab.kMaintainRatio"),
                      label1=mel.eval("uiRes m_createMayaSoftwareCommonGlobalsTab.kPixelAspect"),
                      label2=mel.eval("uiRes m_createMayaSoftwareCommonGlobalsTab.kDeviceAspect"),
                      on1=lambda *args:cmds.setAttr("defaultResolution.lockDeviceAspectRatio", 0),
                      on2=lambda *args:cmds.setAttr("defaultResolution.lockDeviceAspectRatio", 1),
                      data1=0,
                      data2=1)


    cmds.connectControl('ratioLockRadio', 'defaultResolution.lockDeviceAspectRatio', index=1)

    cmds.floatFieldGrp('mayaSoftwareResWidth',
                        label=mel.eval("uiRes m_createMayaSoftwareCommonGlobalsTab.kWidth"),
                        changeCommand=changeArnoldAspectLockWidth)

    cmds.connectControl('mayaSoftwareResWidth', 'defaultResolution.width', index=1)

    cmds.floatFieldGrp('mayaSoftwareResHeight', label=mel.eval("uiRes m_createMayaSoftwareCommonGlobalsTab.kHeight"),
                        changeCommand=changeArnoldAspectLockHeight)

    cmds.connectControl('mayaSoftwareResHeight', 'defaultResolution.height', index=1)

    cmds.optionMenuGrp('sizeUnitsMenu',
                        label=mel.eval("uiRes m_createMayaSoftwareCommonGlobalsTab.kSizeUnits"),
                        changeCommand=updateArnoldResolution)

    # Construct all menu items
    for i, melUnit in enumerate(gMeasurementUnitsNames):
        cmds.menuItem(label=mel.eval("resolutionFormats_melToUI \""+melUnit+"\""), data=i)

    # connect the label, so we can change its color
    cmds.connectControl('sizeUnitsMenu', 'defaultResolution.imageSizeUnits', index=1)
    # connect the menu, so it will always match the attribute
    cmds.connectControl('sizeUnitsMenu', 'defaultResolution.imageSizeUnits', index=2)

    cmds.separator(style='none', h=5)

    cmds.floatFieldGrp('mayaSoftwareRes',
                        label=mel.eval("uiRes m_createMayaSoftwareCommonGlobalsTab.kResolution"),
                        changeCommand=changeArnoldRes)

    cmds.connectControl('mayaSoftwareRes', 'defaultResolution.dotsPerInch', index=1)

    cmds.optionMenuGrp('resUnitsMenu',
                        label=mel.eval("uiRes m_createMayaSoftwareCommonGlobalsTab.kResolutionUnits"),
                        changeCommand=updateArnoldResolution)

    # Construct all menu items
    for i, melUnit in enumerate(gResolutionUnitsNames):
        cmds.menuItem(label=mel.eval("resolutionFormats_melToUI \""+melUnit+"\""), data=i)

    # connect the label, so we can change its color
    cmds.connectControl('resUnitsMenu', 'defaultResolution.pixelDensityUnits', index=1)
    # connect the menu, so it will always match the attribute
    cmds.connectControl('resUnitsMenu', 'defaultResolution.pixelDensityUnits', index=2)

    cmds.separator()

    cmds.floatFieldGrp('resRatio',
                        label=mel.eval("uiRes m_createMayaSoftwareCommonGlobalsTab.kDeviceAspectRatio"),
                        changeCommand=updateArnoldDeviceAspectRatio)

    # connect the label, so we can change its color
    cmds.connectControl('resRatio', 'defaultResolution.deviceAspectRatio', index=1)
    # connect the menu, so it will always match the attribute
    cmds.connectControl('resRatio', 'defaultResolution.deviceAspectRatio', index=2)

    cmds.floatFieldGrp('pixRatio',
                        label=mel.eval("uiRes m_createMayaSoftwareCommonGlobalsTab.kPixelAspectRatio"),
                        changeCommand=updateArnoldPixelAspectRatio)

    # connect the label, so we can change its color
    cmds.connectControl('pixRatio', 'defaultResolution.pixelAspect', index=1)
    # connect the menu, so it will always match the attribute
    cmds.connectControl('pixRatio', 'defaultResolution.pixelAspect', index=2)

    cmds.setParent('..')
    cmds.setUITemplate(popTemplate=True)

    # Make sure the values are right
    updateArnoldResolution()

    # Set up script jobs for those attributes which require updating of
    # multiple controls.
    # This is especially important when a user changes render layers.
    #
    attrArray = []
    attrArray.append("defaultResolution.width")
    attrArray.append("defaultResolution.height")
    attrArray.append("defaultResolution.dotsPerInch")
    attrArray.append("defaultResolution.imageSizeUnits")
    attrArray.append("defaultResolution.pixelDensityUnits")

    for attr in attrArray:
        cmds.scriptJob(attributeChange=(attr, updateArnoldResolution),
                        parent=cmds.setParent(query=True))



def changeArnoldRes(*args):
    '''
     Description:
        Called when the resolution field is changed.
        Updates the corresponding attribute, converting to DPI.
    '''

    oldParent = cmds.setParent(query=True)
    setParentToArnoldCommonTab()

    gResolutionUnitsNames = mu.getVar('gResolutionUnitsNames', type='string[]', init=True)
    oldDPI = cmds.getAttr('defaultResolution.dotsPerInch')
    value = cmds.floatFieldGrp('mayaSoftwareRes', q=True, v1=True)

    # Convert from the current resolution units to DPI
    resUnits = cmds.getAttr('defaultResolution.pixelDensityUnits')
    newDPI = mel.eval('convertResolutionMeasurement({}, \"{}\", \"pixels/inch\")'.format(value, gResolutionUnitsNames[resUnits]))

    # Check that value is within value range
    if newDPI < 1.0:
        cmds.warning(mel.eval("uiRes m_createMayaSoftwareCommonGlobalsTab.kResolutionDPIWarn"))
        newDPI = 1.0

    oldWidth = cmds.getAttr('defaultResolution.width')
    newWidth = oldWidth
    oldHeight = cmds.getAttr('defaultResolution.height')
    newHeight = oldHeight

    # Change pixel width/height only if the image size units are not
    # currently set as pixels
    #
    sizeUnits = cmds.getAttr('defaultResolution.imageSizeUnits')
    if sizeUnits != 0: # 0 corresponds to pixels
        newWidth = math.floor( oldWidth * newDPI/oldDPI + 0.5 )
        newHeight = math.floor( oldHeight * newDPI/oldDPI + 0.5 )

    # Account for version restrictions and bounds
    #
    isMayaEvalVersion = cmds.about(ev=True)
    PLE_MAX_X = 1024
    PLE_MAX_Y =  768

    if isMayaEvalVersion:
        warnMsg = mel.eval("uiRes m_createMayaSoftwareCommonGlobalsTab.kDPICannotBeAchieved")
        warnDisp = ''
        # Check width
        if newWidth > PLE_MAX_X:
            warnDisp = cmds.format(warnMsg, s=(PLE_MAX_X, PLE_MAX_Y))
            cmds.warning(warnDisp)
            newWidth = PLE_MAX_X
            # Adjust DPI to maintain constant document size
            newDPI = oldDPI * newWidth/oldWidth
            # Adjust height to maintain correct ratio
            newHeight = oldHeight * newWidth/oldWidth

        # Check height
        if newHeight > PLE_MAX_Y:
            warnDisp = cmds.format(warnMsg, s=(PLE_MAX_X, PLE_MAX_Y))
            cmds.warning(warnDisp)
            newHeight = PLE_MAX_Y
            # Adjust DPI to maintain constant document size
            newDPI = oldDPI * newHeight/oldHeight
            # Adjust width to maintain correct ratio
            newWidth = oldWidth * newHeight/oldHeight


    if newWidth < 2:
        cmds.warning(mel.eval("uiRes m_createMayaSoftwareCommonGlobalsTab.kWidthWarning"))
        newWidth = 2

    if newHeight < 2:
        cmds.warning(mel.eval("uiRes m_createMayaSoftwareCommonGlobalsTab.kHeightWarning"))
        newHeight = 2


    # All attributes should now be correct
    cmds.setAttr('defaultResolution.dotsPerInch', newDPI)
    cmds.setAttr('defaultResolution.width', newWidth)
    cmds.setAttr('defaultResolution.height', newHeight)

    # Update the values, will correct any invalid entries
    updateArnoldResolution()

    cmds.setParent(oldParent)


def updateArnoldResolution(*args):
    '''
    Procedure Name:
        updateArnoldResolution
    
    Description:
        Gets the real values from the nodes and sets the UI based
      on these values.  This procedure updates all of the resolution
      values.
    '''

    oldParent = cmds.setParent(query=True)
    setParentToArnoldCommonTab()

    width = cmds.getAttr('defaultResolution.width')
    height = cmds.getAttr('defaultResolution.height')
    aspect = cmds.getAttr('defaultResolution.deviceAspectRatio')
    dpi = cmds.getAttr('defaultResolution.dotsPerInch')
    resItem = 0
    whichRes = 1 # use "Custom" if no match is found
    allResNodes = cmds.ls(type='resolution')

    gImageFormatData = mu.getVar('gImageFormatData', type='string[]', init=True)
    gUserImageFormatData = mu.getVar('gUserImageFormatData', type='string[]', init=True)
    gDefaultDpi = mu.getVar('gDefaultDpi', type='float', init=True)

    numResolutionPresets = len(gImageFormatData)
    numUserResolutionPresets = len(gUserImageFormatData)
    numResolutionNodePresets = len(allResNodes) - 1
    resWidth = 0
    resHeight = 0
    resAspect = 0
    resDpi = 0

    for resItem in range(0, numResolutionPresets):

        item = gImageFormatData[resItem]
        tokens = item.split()
        numTokens = len(tokens)

        if numTokens == 5:

            resWidth = float(tokens[1])
            resHeight = float(tokens[2])
            resAspect = float(tokens[3])
            resDpi = float(tokens[4])

            # Check all values, including DPI. If the DPI in the array is 0
            # (i.e. unspecified), then any dpi is considered a match.
            if width == resWidth and height == resHeight \
                  and math.fabs(aspect - resAspect) < 0.001 \
                  and (resDpi==0 or math.fabs(dpi - resDpi)) < 0.001:

                # We add _2_ to $resItem below: 1 because we're
                # skipping the first item (Custom) in the list, and 1
                # because the optionMenu items are numbered starting at 1,
                # but our list in $gImageFormatData is indexed starting at 0.
                whichRes = resItem + 2
                break
        else:
            invalidImageFormat = mel.eval("uiRes m_createMayaSoftwareCommonGlobalsTab.kInvalidImageFormat")
            warnMsg = cmds.format(invalidImageFormat, s=item)
            cmds.warning(warnMsg)

    # If no match was found in the built-in resolutions,
    # check out the user-defined ones
    #
    if whichRes == 1:
        for resItem in range(0, numUserResolutionPresets):
            item = gUserImageFormatData[resItem]
            tokens = item.split()
            numTokens = len(tokens)

            # User may or may not have specified a resolution.
            # Ensure compatibility.
            #
            if numTokens == 4 or numTokens == 5:
                resWidth = float(tokens[1])
                resHeight = float(tokens[2])
                resAspect = float(tokens[3])
                if numTokens == 5:
                    resDpi = float(tokens[4])
                else:
                    resDpi = gDefaultDpi

                if width == resWidth and height == resHeight \
                     and math.fabs(aspect - resAspect) < 0.001 \
                     and (resDpi==0 or math.fabs(dpi - resDpi)) < 0.001:

                    whichRes = numResolutionPresets + resItem + 2
                    break
            else:
                invalidImageFormat = mel.eval("uiRes m_createMayaSoftwareCommonGlobalsTab.kInvalidUserImageFormat")
                warnMsg = cmds.format(invalidImageFormat, s=item)
                cmds.warning(warnMsg)

    # If no match was found in the user-defined resolutions,
    # see if there are any 'extra' resolution nodes in the scene.
    #
    if whichRes == 1:
        for resItem in range(0, numResolutionNodePresets):

            # We assume the 0th item in the list of resolution nodes is
            # the default one, which is created implicitly...
            #
            resNodeName = allResNodes[resItem + 1]

            resWidth = cmds.getAttr(resNodeName + ".width")
            resHeight = cmds.getAttr(resNodeName + ".height")
            resAspect = cmds.getAttr(resNodeName + ".deviceAspectRatio")

            if width == resWidth and height == resHeight \
                  and math.fabs(aspect - resAspect) < 0.001:

                # We add _2_ to $resItem below: 1 because we're
                # skipping the first item (Custom) in the list, and 1
                # because the optionMenu items are numbered starting at 1,
                # but our list in $gImageFormatData is indexed starting at 0.
                #
                whichRes = numResolutionPresets + numUserResolutionPresets + resItem + 2
                break

    cmds.optionMenuGrp('resolutionMenu', edit=True, sl=whichRes)

    cmds.checkBoxGrp('aspectLockCheck', edit=True, v1=cmds.getAttr('defaultResolution.aspectLock'))
    resNode = 'defaultResolution'
    cmds.floatFieldGrp('resRatio', edit=True, v1=aspect)
    adjustArnoldPixelAspect(resNode)
    cmds.setAttr('{}.pixelAspect'.format(resNode), cmds.floatFieldGrp('pixRatio', q=True, v1=True))
    cmds.radioButtonGrp('ratioLockRadio',
                        edit=True,
                        select=(cmds.getAttr('{}.lockDeviceAspectRatio'.format(resNode))+1))
    #
    # Update the UI controls for image size and resolution
    #
    gMeasurementUnitsNames = mu.getVar('gMeasurementUnitsNames', type='string[]')
    gResolutionUnitsNames = mu.getVar('gResolutionUnitsNames', type='string[]')

    sizeUnits = cmds.getAttr('{}.imageSizeUnits'.format(resNode))
    resUnits = cmds.getAttr('{}.pixelDensityUnits'.format(resNode))

    # Update width and height fields
    docWidth = float(width)
    docHeight = float(height)

    precision = 0 # To ensure pixel values are displayed without decimals
    if sizeUnits != 0:
        # Convert from pixels to the correct measurement units
        inchWidth = mel.eval('convertPixelsToInches({}, {})'.format(width, dpi))
        inchHeight = mel.eval('convertPixelsToInches({}, {})'.format(heght, dpi))
        docWidth = mel.eval('convertMeasurement({}, \"inches\", \"{}\")'.format(inchWidth, gMeasurementUnitsNames[sizeUnits]))
        docHeight = mel.eval('convertMeasurement({}, \"inches\", \"{}\")'.format(inchHeight, gMeasurementUnitsNames[sizeUnits]))
        precision = 3

    cmds.floatFieldGrp('mayaSoftwareResWidth', edit=True, precision=precision, v1=docWidth)
    cmds.floatFieldGrp('mayaSoftwareResHeight', edit=True, precision=precision, v1=docHeight)

    # Update resolution field
    # Convert from DPI to the correct resolution units
    res = mel.eval('convertResolutionMeasurement({}, \"pixels/inch\", \"{}\")'.format(dpi, gResolutionUnitsNames[resUnits]))
    cmds.floatFieldGrp('mayaSoftwareRes', edit=True, precision=3, v1=res)

    # "Size Units" and "Resolution Units" fields automatically update
    # because they are attached to a harness

    cmds.setParent(oldParent)


def changeArnoldResolution(*args):
    '''
    Procedure Name:
        changeResolution
    
    Description:
      This procedure is called when the user selects a different
      resolution.  It sets the internal representation
      and then updates the example to show the changes.
    '''

    oldParent = cmds.setParent(query=True)
    setParentToArnoldCommonTab()

    gImageFormatData = mu.getVar('gImageFormatData', type='string[]')
    gUserImageFormatData = mu.getVar('gUserImageFormatData', type='string[]')
    gDefaultDpi = mu.getVar('gDefaultDpi')

    # We are suppose to get proper image formats for PLE.
    isMayaEvalVersion = cmds.about(ev=True)
    if isMayaEvalVersion:
        gPLEImageFormatData = mu.getVar('gPLEImageFormatData', type='string[]')
        gImageFormatData = gPLEImageFormatData

    numResolutionPresets = len(gImageFormatData)
    numUserResolutionPresets = len(gUserImageFormatData)
    allResNodes = cmds.ls(type='resolution')
    numResolutionNodePresets = len(allResNodes) - 1
    tokens = []
    resItem = cmds.optionMenuGrp('resolutionMenu', q=True, sl=True)
    resWidth = 0
    resHeight = 0
    resAspect = 0
    resDpi = 0 # signals preset doesn't contain dpi info
    item = ''

    # Item #1 is Custom, which doesn't change the fields
    # We subtract _2_ from $resItem below: 1 because we're
    # skipping the first item (Custom) in the list, and 1
    # because the optionMenu items are numbered starting at 1,
    # but our list in $gImageFormatData is indexed starting at 0.
    #
    if resItem > 1:
        if resItem > (numResolutionPresets + 1):
            if resItem > (numResolutionPresets + numUserResolutionPresets + 1):
                # It's one of the user-defined resolution nodes' presets
                resNode = allResNodes[resItem - numResolutionPresets - numUserResolutionPresets - 1]
                resWidth = cmds.getAttr('{}.width'.format(resNode))
                resHeight = cmds.getAttr('{}.height'.format(resNode))
                resAspect = cmds.getAttr('{}.deviceAspectRatio'.format(resNode))
            else:
                # It's one of the user-defined resolution presets
                item = gUserImageFormatData[resItem - numResolutionPresets - 2]
                tokens = item.split()
                resWidth = float(tokens[1])
                resHeight = float(tokens[2])
                resAspect = float(tokens[3])
                if len(tokens) == 5:  # user has included the dpi field
                    resDpi = float(tokens[4])
                else:
                    resDpi = gDefaultDpi # default dpi
        else:
            # It's one of the built-in resolution presets
            item = gImageFormatData[resItem - 2]
            tokens = item.split()
            numTokens = len(tokens)
            resWidth = float(tokens[1])
            resHeight = float(tokens[2])
            resAspect = float(tokens[3])
            resDpi = float(tokens[4])

        cmds.setAttr("defaultResolution.width", resWidth)
        cmds.setAttr("defaultResolution.height", resHeight)
        cmds.setAttr("defaultResolution.deviceAspectRatio", resAspect)
        cmds.setAttr("defaultResolution.lockDeviceAspectRatio", 0)
        pixelAspect = float(resHeight)/float(resWidth)*resAspect
        cmds.setAttr("defaultResolution.pixelAspect", pixelAspect)

        # Set the dpi if it's non-zero
        if resDpi != 0:
            cmds.setAttr("defaultResolution.dotsPerInch", resDpi)


        # Set the proper field ordering if PAL or NTSC.
        if cmds.getAttr('defaultResolution.height') == 576: # PAL
            cmds.setAttr("defaultResolution.oddFieldFirst", 0)
            if cmds.columnLayout('rgFieldLayout', exists=True) and mel.eval("exists updateFieldOptions"):
                mel.eval("updateFieldOptions")
        elif cmds.getAttr('defaultResolution.height') == 486: # NTSC
            cmds.setAttr("defaultResolution.oddFieldFirst", 1)
            if cmds.columnLayout('rgFieldLayout', exists=True) and mel.eval("exists updateFieldOptions"):
                mel.eval("updateFieldOptions")

    updateArnoldResolution()

    cmds.setParent(oldParent)



def updateArnoldPixelDeviceRatios(node):
    '''
    This is called when the resolution changes. Update the pixel or the
    device aspect ration as necessary.
    '''
    aspect = float(cmds.getAttr('{}.width'.format(node))) / float(cmds.getAttr('{}.width'.format(node)))

    if cmds.getAttr('{}.lockDeviceAspectRatio'.format(node)) == 0:
        aspect = aspect * float(cmds.getAttr('{}.pixelAspect'.format(node)))
        cmds.setAttr('{}.deviceAspectRatio'.format(node), aspect)
    else:
        aspect = float(cmds.getAttr('{}.deviceAspectRatio'.format(node))) / aspect
        cmds.setAttr('{}.pixelAspect'.format(node), aspect)

def checkArnoldAspectLockWidth(node):
    if cmds.getAttr('{}.aspectLock'.format(node)):
        value = cmds.getAttr('{}.width'.format(node))
        aspect = cmds.getAttr('{}.pixelAspect'.format(node))
        aspect /= float(cmds.getAttr('{}.deviceAspectRatio'.format(node)))

        #fix for bug#269698, plus 0.5 to give round value
        rez = (aspect * value) + 0.5

        if cmds.about(ev=True):
            if rez > PLE_MAX_Y:
                warnMsg = mel.eval("uiRes m_createMayaSoftwareCommonGlobalsTab.kImageResolutionLimited")
                dispMsg = cmds.format(warnMsg, s=(PLE_MAX_X,PLE_MAX_Y))
                cmds.warning(dispMsg)
                rez = PLE_MAX_Y

        cmds.setAttr('{}.height'.format(node), rez)

    updateArnoldPixelDeviceRatios(node)

def checkArnoldAspectLockHeight(node):
    if cmds.getAttr('{}.aspectLock'.format(node)):
        value = cmds.getAttr('{}.height'.format(node))
        aspect = cmds.getAttr('{}.pixelAspect'.format(node))
        aspect /= cmds.getAttr('{}.deviceAspectRatio'.format(node))

        #fix for bug#269698, plus 0.5 to give round value
        rez = (value/aspect) + 0.5

        if cmds.about(ev=True):
            if rez > PLE_MAX_X:
                warnMsg = mel.eval("uiRes m_createMayaSoftwareCommonGlobalsTab.kImageResolutionLimited")
                dispMsg = cmds.format(warnMsg, s=(PLE_MAX_X, PLE_MAX_Y))
                cmds.warning(dispMsg)
                rez = PLE_MAX_X

        cmds.setAttr('{}.width'.format(node), rez)
        
    updateArnoldPixelDeviceRatios(node)


def changeArnoldAspectLockWidth(*args):
    '''
    Procedure Name:
        changeArnoldAspectLockWidth
    
    Description:
      This procedure is called when the user changes the
      resolution width.  It sets the internal representation
      then looks at the ratio lock etc and changes any other
      values that rely on it.
    '''

    oldParent = cmds.setParent(query=True)
    setParentToArnoldCommonTab()

    widthValue = cmds.floatFieldGrp('mayaSoftwareResWidth', q=True, v1=True)

    gMeasurementUnitsNames = mu.getVar('gMeasurementUnitsNames', type='string[]')

    resNode = 'defaultResolution'
    dpi = cmds.getAttr('{}.dotsPerInch'.format(resNode))
    sizeUnits = cmds.getAttr('{}.imageSizeUnits'.format(resNode))

    if sizeUnits != 0:
        # Convert the obtained value to inches, then to pixels
        inchWidth = mel.eval('convertMeasurement({}, \"{}\", \"inches\")'.format(widthValue, gMeasurementUnitsNames[sizeUnits]))
        requestedWidth = mel.eval('convertInchesToPixels({}, {})'.format(inchWidth, dpi))
    else: # the width value is in pixels, so no need to convert
        requestedWidth = widthValue

    if cmds.about(ev=True):
        if requestedWidth > PLE_MAX_X:
            warnMsg = mel.eval("uiRes m_createMayaSoftwareCommonGlobalsTab.kImageResolutionLimited")
            dispMsg = cmds.format(warnMsg, s=(PLE_MAX_X, PLE_MAX_Y))
            cmds.warning(dispMsg)
            requestedWidth = PLE_MAX_X

    if requestedWidth < 2:
        cmds.warning(mel.eval("uiRes m_createMayaSoftwareCommonGlobalsTab.kWidthWarning"))
        requestedWidth = 2

    cmds.setAttr('{}.width'.format(resNode), requestedWidth)
    cmds.optionMenuGrp('resolutionMenu', edit=True, sl=1)
    checkArnoldAspectLockWidth(resNode)

    # Update the values
    updateArnoldResolution()

    cmds.setParent(oldParent)


def changeArnoldAspectLockHeight(*args):
    '''
    Procedure Name:
        changeArnoldAspectLockHeight
    
    Description:
      This procedure is called when the user changes the
      resolution width.  It sets the internal representation
      then looks at the ratio lock etc and changes any other
      values that rely on it.
    '''
    oldParent = cmds.setParent(query=True)
    setParentToArnoldCommonTab()

    heightValue = cmds.floatFieldGrp('mayaSoftwareResHeight', q=True, v1=True)

    gMeasurementUnitsNames = mu.getVar('gMeasurementUnitsNames', type='string[]')

    resNode = 'defaultResolution'
    dpi = cmds.getAttr('{}.dotsPerInch'.format(resNode))
    sizeUnits = cmds.getAttr('{}.imageSizeUnits'.format(resNode))

    if sizeUnits != 0:
        inchHeight = mel.eval('convertMeasurement({}, \"{}\", \"inches\")'.format(heightValue, gMeasurementUnitsNames[sizeUnits]))
        # Convert the obtained value to inches, then to pixels
        requestedHeight = mel.eval('convertInchesToPixels({},{})'.format(inchHeight, dpi))
    else:
        # the width value is in pixels, so no need to convert
        requestedHeight = heightValue

    if cmds.about(ev=True):
        if requestedHeight > PLE_MAX_Y:
            warnMsg = mel.eval("uiRes m_createMayaSoftwareCommonGlobalsTab.kImageResolutionLimited")
            dispMsg = cmds.format(warnMsg, s=(PLE_MAX_X, PLE_MAX_Y))
            cmds.warning(dispMsg)
            requestedHeight = PLE_MAX_Y

    if requestedHeight < 2:
        cmds.warning(mel.eval("uiRes m_createMayaSoftwareCommonGlobalsTab.kHeightWarning"))
        requestedHeight = 2

    cmds.setAttr('{}.height'.format(resNode), requestedHeight)
    cmds.optionMenuGrp('resolutionMenu', edit=True, sl=1)
    checkArnoldAspectLockHeight(resNode)

    # Set the proper field ordering if PAL or NTSC.
    if requestedHeight == 576: # PAL
        cmds.setAttr('{}.oddFieldFirst'.format(resNode), 0)
        if cmds.columnLayout('rgFieldLayout', exists=True):
            if mel.eval("exists updateFieldOptions"):
                mel.eval("updateFieldOptions")


    elif requestedHeight == 486: # NTSC
        cmds.setAttr('{}.oddFieldFirst'.format(resNode), 1)
        if cmds.columnLayout('rgFieldLayout', exists=True):
            if mel.eval("exists updateFieldOptions"):
                mel.eval("updateFieldOptions")

    # Update the values
    updateArnoldResolution()

    cmds.setParent(oldParent)


def adjustArnoldPixelAspect(node):

    oldParent = cmds.setParent(query=True)
    setParentToArnoldCommonTab()

    aspect = cmds.getAttr('{}.deviceAspectRatio'.format(node))
    width = cmds.getAttr('{}.width'.format(node))
    height = cmds.getAttr('{}.height'.format(node))
    pixelAspect = float(width) / float(height)
    pixelAspect = aspect / pixelAspect
    cmds.floatFieldGrp('pixRatio', e=True, v1=pixelAspect)

    cmds.setParent(oldParent)


def adjustArnoldDeviceAspect(node):

    oldParent = cmds.setParent(query=True)
    setParentToArnoldCommonTab()

    width = cmds.getAttr('{}.width'.format(node))
    height = cmds.getAttr('{}.height'.format(node))

    pixelAspect = cmds.floatFieldGrp('pixRatio', q=True, v1=True)
    aspect = float(width) / float(height)
    aspect = pixelAspect * aspect
    cmds.setAttr('{}.deviceAspectRatio'.format(node), aspect)
    cmds.floatFieldGrp('resRatio', edit=True, v1=aspect)

    cmds.setParent(oldParent)


def updateArnoldPixelAspectRatio(*args):

    oldParent = cmds.setParent(query=True)
    setParentToArnoldCommonTab()
    resNode = 'defaultResolution'
    resNode.pixelAspect.set(cmds.floatFieldGrp('pixRatio', q=True, v1=True))
    adjustArnoldDeviceAspect(resNode)
    updateArnoldResolution()

    cmds.setParent(oldParent)


def updateArnoldDeviceAspectRatio(*args):

    oldParent = cmds.setParent(query=True)
    setParentToArnoldCommonTab()

    cmds.setAttr('defaultResolution.deviceAspectRatio', cmds.floatFieldGrp('resRatio', q=True, v1=True))
    adjustArnoldPixelAspect('defaultResolution')
    updateArnoldResolution()

    cmds.setParent(oldParent)


# ----------------------------------------------------------------------------
# Code to update pre/post layer/frame callbacks
#

def changeArnoldMelCallbacks(control, attr):

    oldParent = cmds.setParent(query=True)
    setParentToArnoldCommonTab()

    val = cmds.scrollField(control, query=True, text=True)
    cmds.setAttr(attr, val, type="string")

    cmds.setParent(oldParent)


def updateArnoldMelCallbacks(*args):
    for attr in ['preMel','postMel','preRenderLayerMel', 'postRenderLayerMel', 'preRenderMel', 'postRenderMel']:
        control = attr + 'SwGrp'
        plug = 'defaultRenderGlobals.' + attr
        updateArnoldMelCallback(control, plug)


def updateArnoldMelCallback(control, attr):
  oldParent = cmds.setParent(query=True)
  setParentToArnoldCommonTab()

  value = cmds.getAttr(attr)
  if value is None:
      value = ''
  
  cmds.scrollField(control, edit=True, text=value)
  cmds.setParent(oldParent)


# ----------------------------------------------------------------------------
# Code to create and update the Render Options frame
#

def createArnoldCommonRenderOptions():

    parent = cmds.setParent(query=True)

    cmds.setUITemplate('attributeEditorTemplate', pushTemplate=True)

    cmds.columnLayout(adjustableColumn=True)
    cmds.separator(style='none', height=10)

    def makeMelRenderCB(attr, ui, scriptJob=False):
        plug = 'defaultRenderGlobals.' + attr
        cmds.text(label=mel.eval("uiRes m_createMayaSoftwareCommonGlobalsTab." + ui))
        control = attr + 'SwGrp'
        cmds.scrollField(control,
                       preventOverride=True,
                       height=50,
                       changeCommand=lambda arg=None, x=control, y=plug: changeArnoldMelCallbacks(x, y))
        updateArnoldMelCallback(control, plug)
  
        cmds.scriptJob(parent=parent,
                     attributeChange=(plug, lambda arg=None, x=control, y=plug: updateArnoldMelCallback(x, y)))

    # Set up script jobs for those attributes which require updating of
    # multiple controls.
    # This is especially important when a user changes render layers.
    for attr, ui, scriptJob in [ ('preMel', 'kPreRenderMEL', False),
                                 ('postMel', 'kPostRenderMEL', False),
                                 ('preRenderLayerMel', 'kPreRenderLayerMEL', True),
                                 ('postRenderLayerMel', 'kPostRenderLayerMEL', True),
                                 ('preRenderMel', 'kPreRenderFrameMEL', True),
                                 ('postRenderMel', 'kPostRenderFrameMEL', True)]:
        makeMelRenderCB(attr, ui, scriptJob)

    cmds.setParent(parent)
    cmds.setUITemplate(popTemplate=True)



#==================================================================
# Common Tab
#==================================================================

def updateArnoldRendererCommonGlobalsTab(*args):

    '''
     Description:
     This procedure is called when the current renderer changes to be the
     Arnold Renderer.
     This procedure updates controls in the Common tab of the Maya Software
     renderer to reflect values which may have been copied from the previous
     current renderer.
    '''
    # Re check for aiOptions node to exists
    core.createOptions()

    updateArnoldFileNamePrefixControl()
    updateArnoldFileNameFormatControl()

    # Must recreate the Image Format option menu because it is renderer specific.
    # This is only required for the master layer layout.
    #
    '''
    if( isDisplayingAllRendererTabs()){

        // Set the correct parent
        setParentToArnoldCommonTab();
        setParent rgImageFileFrame;

        // Recreate the tab
        createArnoldCommonImageFile();
    }
    '''
    updateArnoldImageFormatControl()
    updateArnoldCameraControl()
    updateArnoldResolution()
    updateArnoldMelCallbacks()
    updateArnoldTargetFilePreview()


def createArnoldRendererCommonGlobalsTab():
    '''
     Description:
     This procedure is called when building the render globals tabs for the
     Maya Software renderer.
     This procedure builds the "General" tab for the Maya Software renderer.
    '''

    # Make sure the aiOptions node exists
    core.createOptions()

    parentForm = cmds.setParent(query=True)

    createArnoldTargetFilePreview()

    cmds.setParent(parentForm)

    cmds.scrollLayout('scrollLayout',horizontalScrollBarThickness=0)

    commonTabColumn = cmds.columnLayout('commonTabColumn', adjustableColumn=True)

    # Image File Name
    #
    cmds.frameLayout('rgImageFileFrame',
                     label=mel.eval("uiRes m_createMayaSoftwareCommonGlobalsTab.kImageFileOutput"),
                     collapsable=True,
                     collapse=False)

    createArnoldCommonImageFile()

    cmds.setParent(commonTabColumn)

    # Frame Range Output
    #
    cmds.frameLayout('rgFrameRangeFrame',
                     label="Frame Range",
                     collapsable=True,
                     collapse=False)

    createArnoldCommonFrameRange()

    cmds.setParent(commonTabColumn)

    # Renderable Cameras
    #
    cmds.frameLayout('rgRenderableCamerasFrame',
                     label=mel.eval("uiRes m_createMayaSoftwareCommonGlobalsTab.kRenderableCameras"),
                     collapsable=True,
                     collapse=False)


    createArnoldCommonRenderCameras()

    cmds.setParent(commonTabColumn)

    # Resolution ("Image Size") Section
    #
    cmds.frameLayout('rgResolutionFrame',
                     label=mel.eval("uiRes m_createMayaSoftwareCommonGlobalsTab.kLayoutImageSize"),
                     collapsable=True,
                     collapse=False)


    createArnoldCommonResolution()

    cmds.setParent(commonTabColumn)
    
    # Scene Assembly Section
    #
    maya_version = cmds.about(version=True)
    if int(float(maya_version)) >= 2017:
        cmds.frameLayout('sceneAssemblyFrame',
                        label=mel.eval("uiRes m_createMayaSoftwareCommonGlobalsTab.kSceneAssembly"),
                        collapsable=True,
                        collapse=True)

        mel.eval('createCommonSceneAssembly()')
    
        cmds.setParent(commonTabColumn)

    # Render Options
    #
    cmds.frameLayout('mayaSoftwareOptionFrame',
                     label=mel.eval("uiRes m_createMayaSoftwareCommonGlobalsTab.kRenderOptions"),
                     collapsable=True,
                     collapse=True)

    createArnoldCommonRenderOptions()

    cmds.setParent(commonTabColumn)

    cmds.setParent(parentForm)

    cmds.formLayout(parentForm,
                    edit=True,
                    af=[('targetFilePreview',"top", 5),
                        ('targetFilePreview', "left", 0),
                        ('targetFilePreview', "right", 0),
                        ('scrollLayout', "bottom", 0),
                        ('scrollLayout', "left", 0),
                        ('scrollLayout', "right", 0)],
                    an=[('targetFilePreview', "bottom")],
                    ac=[('scrollLayout', "top", 5, 'targetFilePreview')])

    # Update the target file preview.
    #
    updateArnoldTargetFilePreview()

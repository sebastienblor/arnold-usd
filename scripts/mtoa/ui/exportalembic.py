import maya.cmds as cmds
import maya.mel as mel
from mtoa.callbacks import *
import mtoa.core as core

import re
from functools import partial

#
#  Description:
#      This script performs the AlembicExportAll and
#      AlembicExportSelection commands.
#

#
# Global Variables
#
# Modes of each cache time range in General Options UI
global gTmpCacheTimeRangeModes
# Low sample frames of each cache time range in General Options UI
global gTmpSampleLowFrames
# High sample frames of each cache time range in General Options UI
global gTmpSampleHighFrames
# Flag that if we can edit the time range in scriptTable. When we are updating
# the time range in General Option UI, it should be allowed.
global gAllowEditTimeRangeInTable
# Flag that if the overlapping warning dialog will pop up.
# Three states:
#    -1 - the dialog should pop up;
#     0 - the dialog should not pop up; and abort the operation (behavior 
#         of "Review Ranges" button)
#     1 - the dialog should not pop up; and auto-resolve the overlapping (behavior 
#         of "Continue" button)
global gAlembicShowRangeAdjustDialog


def stringToStringArray(in_str, sep):
    return [x for x in in_str.split(sep) if len(x)]

#
#  Procedure Name:
#      setOptionVars
#
#  Description:
#      Initialize the option values.
#
#  Input Arguments:
#      Whether to set the options to default values.
#
#  Return Value:
#      None.
def setOptionVars(forceFactorySettings=False):

    if forceFactorySettings or not cmds.optionVar(exists="MTOA_Alembic_exportCacheTimeRanges"):
        cmds.optionVar(clearArray='MTOA_Alembic_exportCacheTimeRanges')
        cmds.optionVar(intValueAppend='MTOA_Alembic_exportCacheTimeRanges', 2)

    if forceFactorySettings or not cmds.optionVar(exists='MTOA_Alembic_exportStarts'):
        cmds.optionVar(clearArray='MTOA_Alembic_exportStarts')
        cmds.optionVar(floatValueAppend='MTOA_Alembic_exportStarts', 1)

    if forceFactorySettings or not cmds.optionVar(exists='MTOA_Alembic_exportEnds'):
        cmds.optionVar(clearArray='MTOA_Alembic_exportEnds')
        cmds.optionVar(floatValueAppend='MTOA_Alembic_exportEnds', 24)

    if forceFactorySettings or not cmds.optionVar(exists='MTOA_Alembic_exportEvaluateEverys'):
        cmds.optionVar(clearArray='MTOA_Alembic_exportEvaluateEverys')
        cmds.optionVar(floatValueAppend='MTOA_Alembic_exportEvaluateEverys', 1)

    if forceFactorySettings or not cmds.optionVar(exists='MTOA_Alembic_exportEnableFrameRelativeSamples'):
        cmds.optionVar(clearArray='MTOA_Alembic_exportEnableFrameRelativeSamples')
        cmds.optionVar(intValueAppend='MTOA_Alembic_exportEnableFrameRelativeSamples', 0)

    if forceFactorySettings or not cmds.optionVar(exists='MTOA_Alembic_exportLowFrameRelativeSamples'):
        cmds.optionVar(clearArray='MTOA_Alembic_exportLowFrameRelativeSamples')
        cmds.optionVar(floatValueAppend='MTOA_Alembic_exportLowFrameRelativeSamples', -0.2)

    if forceFactorySettings or not cmds.optionVar(exists='MTOA_Alembic_exportHighFrameRelativeSamples'):
        cmds.optionVar(clearArray='MTOA_Alembic_exportHighFrameRelativeSamples')
        cmds.optionVar(floatValueAppend='MTOA_Alembic_exportHighFrameRelativeSamples', 0.2)

    if forceFactorySettings or not cmds.optionVar(exists='MTOA_Alembic_exportEnablePreRoll'):
        cmds.optionVar(intValue='MTOA_Alembic_exportEnablePreRoll', 0)

    if forceFactorySettings or not cmds.optionVar(exists='MTOA_Alembic_exportPreRollStartFrame'):
        cmds.optionVar(floatValue='MTOA_Alembic_exportPreRollStartFrame', 0)

    if forceFactorySettings or not cmds.optionVar(exists='MTOA_Alembic_exportAttr'):
        cmds.optionVar(stringValue='MTOA_Alembic_exportAttr', "")

    if forceFactorySettings or not cmds.optionVar(exists='MTOA_Alembic_exportAttrPrefix'):
        cmds.optionVar(stringValue='MTOA_Alembic_exportAttrPrefix', "")

    if forceFactorySettings or not cmds.optionVar(exists='MTOA_Alembic_exportVerbose'):
        cmds.optionVar(intValue='MTOA_Alembic_exportVerbose', 0)

    if forceFactorySettings or not cmds.optionVar(exists='MTOA_Alembic_exportNoNormals'):
        cmds.optionVar(intValue='MTOA_Alembic_exportNoNormals', 0)

    if forceFactorySettings or not cmds.optionVar(exists='MTOA_Alembic_exportRenderableOnly'):
        cmds.optionVar(intValue='MTOA_Alembic_exportRenderableOnly', 0)

    if forceFactorySettings or not cmds.optionVar(exists='MTOA_Alembic_exportStripNamespaces'):
        cmds.optionVar(intValue='MTOA_Alembic_exportStripNamespaces', 0)

    if forceFactorySettings or not cmds.optionVar(exists='MTOA_Alembic_exportUVWrite'):
        cmds.optionVar(intValue='MTOA_Alembic_exportUVWrite', 0)

    if forceFactorySettings or not cmds.optionVar(exists='MTOA_Alembic_exportWholeFrameGeo'):
        cmds.optionVar(intValue='MTOA_Alembic_exportWholeFrameGeo', 0)

    if forceFactorySettings or not cmds.optionVar(exists='MTOA_Alembic_exportWorldSpace'):
        cmds.optionVar(intValue='MTOA_Alembic_exportWorldSpace', 0)

    if forceFactorySettings or not cmds.optionVar(exists='MTOA_Alembic_exportWriteVisibility'):
        cmds.optionVar(intValue='MTOA_Alembic_exportWriteVisibility', 0)

    if forceFactorySettings or not cmds.optionVar(exists='MTOA_Alembic_exportPerFrameCallbackMel'):
        cmds.optionVar(stringValue='MTOA_Alembic_exportPerFrameCallbackMel', "")

    if forceFactorySettings or not cmds.optionVar(exists='MTOA_Alembic_exportPostJobCallbackMel'):
        cmds.optionVar(stringValue='MTOA_Alembic_exportPostJobCallbackMel', "")

    if forceFactorySettings or not cmds.optionVar(exists='MTOA_Alembic_exportPerFrameCallbackPython'):
        cmds.optionVar(stringValue='MTOA_Alembic_exportPerFrameCallbackPython', "")

    if forceFactorySettings or not cmds.optionVar(exists='MTOA_Alembic_exportPostJobCallbackPython'):
        cmds.optionVar(stringValue='MTOA_Alembic_exportPostJobCallbackPython', "")

    if forceFactorySettings or not cmds.optionVar(exists='MTOA_Alembic_exportFilterEulerRotations'):
        cmds.optionVar(intValue='MTOA_Alembic_exportFilterEulerRotations', 0)

    if forceFactorySettings or not cmds.optionVar(exists='MTOA_Alembic_exportWriteColorSets'):
        cmds.optionVar(intValue='MTOA_Alembic_exportWriteColorSets', 0)

    if forceFactorySettings or not cmds.optionVar(exists='MTOA_Alembic_exportWriteFaceSets'):
        cmds.optionVar(intValue='MTOA_Alembic_exportWriteFaceSets', 0)

    if forceFactorySettings or not cmds.optionVar(exists='MTOA_Alembic_exportDataFormat'):
        cmds.optionVar(intValue='MTOA_Alembic_exportDataFormat', 2)

    if forceFactorySettings or not cmds.optionVar(exists='MTOA_Alembic_exportPreRollStep'):
        cmds.optionVar(floatValue='MTOA_Alembic_exportPreRollStep', 1)

    if forceFactorySettings or not cmds.optionVar(exists='MTOA_Alembic_exportWriteCreases'):
        cmds.optionVar(intValue='MTOA_Alembic_exportWriteCreases', 0)

    if forceFactorySettings or not cmds.optionVar(exists='MTOA_Alembic_exportWriteUVSets'):
        cmds.optionVar(intValue='MTOA_Alembic_exportWriteUVSets', 0)


class AlembicOptions(object):
    """docstring for AlembicOptions"""
    def __init__(self, arg):
        super(AlembicOptions, self).__init__()
        self.arg = arg

    #
    #  Procedure Name:
    #      Alembic_getActiveTimeRange
    #
    #  Description:
    #      Get active time range according to the selected row of time ranges table
    #
    #  Return Value:
    #      Row number of active time range in the table.
    #
    def getActiveTimeRange(self):

        rows = cmds.scriptTable(self.cacheTimeTable, query=True, selectedRows=True)

        # no row selected, this should be an error
        if len(rows) == 0:
            return False

        # there should be only one row selected
        if rows[0] == 1:
            if cmds.checkBoxGrp(self.enablePreRoll, q=True, value1=True):
                rows[0] = 2

        return rows[0]

    #
    #  Procedure Name:
    #      Alembic_getActiveTimeRangeIndex
    #
    #  Description:
    #      Get the index of active time range according to the selected row of time ranges table
    #
    #  Return Value:
    #      Index of active time range in the table.
    #
    def getActiveTimeRangeIndex(self):
        rows = cmds.scriptTable(self.cacheTimeTable, query=True, selectedRows=True)

        # no row selected, this should be an error
        if len(rows) == 0:
            return False

        index = rows[0] - 1
        if index > 0 and cmds.checkBoxGrp(self.enablePreRoll, q=True, value1=True):
            index -= 1

        return index

    #
    #  Procedure Name:
    #      Alembic_getStartEndFramesOfRange
    #
    #  Description:
    #      Get the start and end frame of the specified range by parsing
    #      the string in scriptTable.
    #
    #  Input Arguments:
    #      row    - Row number in the cache time range table.
    #
    #  Return Value:
    #      The start and end frame of the specified range.
    #
    def getStartEndFramesOfRange(self, row):

        cacheTime = cmds.scriptTable(self. cacheTimeTable, q=True, cellIndex=[row, 1], cellValue=True)
        frames = stringToStringArray(cacheTime[0], '-')
        result = []
        result[0] = float(frames[0])
        result[1] = float(frames[1])
        # frames may be negative, for example, the string may be:
        # "-20--5", "-20-5", "5-20"
        if cacheTime[0].startswith("-"):
            result[0] = -result[0]

            if re.search(r"\-\-", cacheTime[0]):
                result[1] = -result[1];

        return result

    #
    #  Procedure Name:
    #      Alembic_getEvaluateEveryOfRange
    #
    #  Description:
    #      Get evaluate step of the the specified range by parsing
    #      the string in scriptTable.
    #
    #  Input Arguments:
    #      row    - Row number in the cache time range table.
    #
    #  Return Value:
    #      The evaluate step of the the specified range.
    #
    def getEvaluateEveryOfRange(self, row):

        step = cmds.scriptTable(self.cacheTimeTable, q=True, cellIndex=[row, 3], cellValue=True)
        return step[0]

    #
    #  Procedure Name:
    #      Alembic_getSampleEnabledOfRange
    #
    #  Description:
    #      Get sample enable state of the the specified range by parsing
    #      the string in scriptTable.
    #
    #  Input Arguments:
    #      row    - Row number in the cache time range table.
    #
    #  Return Value:
    #      The sample enable state of the the specified range.
    #
    def getSampleEnabledOfRange(self, row):
        subSamples = cmds.scriptTable(self. cacheTimeTable, q=True, cellIndex=[row, 4], cellValue=True)
        return subSamples[0] != "0"

    def getYesLabel(self):
        return "         Yes"

    #
    #  Procedure Name:
    #      Alembic_cacheTimeCellChanged
    #
    #  Description:
    #      Callback when the value of a cell is changed.
    #
    #  Return Value:
    #      An integer value which indicates whether that value 
    #      should be accepted (return 1 if yes, and 0 if no).
    #
    def cacheTimeCellChanged(self, row, column, value):

        global gTmpCacheTimeRangeModes
        global gTmpSampleLowFrames
        global gTmpSampleHighFrames
        global gAllowEditTimeRangeInTable

        rows = cmds.scriptTable(self. cacheTimeTable, query=True, selectedRows=True)
        # no row selected, this should be in initial state
        # so allow edit and don't update UI in "General Options"
        if len(rows) == 0:
            return True

        prerollEnabled = cmds.checkBoxGrp(self.enablePreRoll, q=True, value1=True)

        if column == 1:
            # "Cache Time" column
            totalRows = cmds.scriptTable(self. cacheTimeTable, q=True, rows=True)
            if totalRows - 1 == row: # last row is "Add Range+"
                return True

            if row != 1 or not prerollEnabled:
                rangeIndex = self.getActiveTimeRangeIndex()
                if gTmpCacheTimeRangeModes[rangeIndex] != 3 or gAllowEditTimeRangeInTable == 0:
                    cmds.warning("Cannot edit the time range because it is specified by Maya settings")
                    return False

            value = value.strip()
            res = re.match("^[\\-]*[0-9\\.]+\\-[-]*[0-9\\.]+", value)
            if len(res) == 0:
                cmds.warning("Illegal format")
                return False

            frames = stringToStringArray(value, "-")
            start = float(frames[0])
            end = float(frames[1])
            if value.startswith("-"):
                start = -start

            if re.search(r"\-\-", value):
                end = -end

            if row == 1 and prerollEnabled:
                cmds.floatFieldGrp(sel.preRollStartFrame, e=True, value1=start)
            else:
                cmds.floatFieldGrp(self.startEnd, -e=True, value1=start, value2=end)

            return True
        elif column == 2:
            # "Pre Roll" column cann't be editable
            if row == 1 and prerollEnabled:
                if value == self.getYesLabel():
                    return True

            cmds.warning("Cannot edit PreRoll column")
            return False

        elif column == 3:   # "Step" column
            value = value.strip();
            if value.isdigit():
                step = float(value)
                if step <= 0:
                    cmds.warning("Input invalid: Step cannot be 0 or negative.")
                    return False

                if row == 1 and prerollEnabled:
                    cmds.floatFieldGrp(self.preRollStep, e=True, value1=step)
                else:
                    cmds.floatFieldGrp(self.evaluateEvery, e=True, value1=step)

                return True

            return False
        elif column == 4:   # "Subsamples" column
            if row == 1 and prerollEnabled:
                cmds.warning("Cannot set subsample for PreRoll")
                return False

            samples = stringToStringArray(value, ";")
            if len(samples) == 1 and samples[0] == "0":
                cmds.checkBoxGrp(self.enableFrameRelativeSample, e=True, value1=0)
                cmds.floatFieldGrp(self.lowFrameRelativeSample, e=True, enable=False)
                cmds.floatFieldGrp(self.highFrameRelativeSample, e=True, enable=False)
                return True

            if len(samples) != 2 and len(samples) != 3:
                cmds.warning("Incorrect subsample number")
                return False

            if not samples[0].isdigit() or not samples[len(samples)-1].isdigit():
                return False

            if len(samples) == 3 and samples[1].strip(0) != "0":
                cmds.warning("The middle value of subsample should be 0")
                return False

            rangeIndex = self._getActiveTimeRangeIndex()
            gTmpSampleLowFrames[rangeIndex] = samples[0]
            gTmpSampleHighFrames[rangeIndex] = samples[len(samples)-1]
            cmds.floatFieldGrp(self.lowFrameRelativeSample, e=True, enable=True, value1=gTmpSampleLowFrames[rangeIndex])
            cmds.floatFieldGrp(self.highFrameRelativeSample, e=True, enable=True, value1=gTmpSampleHighFrames[rangeIndex])
            cmds.checkBoxGrp(self.enableFrameRelativeSample, e=True, value1=True)

            return True

        return False

        #
        #  Procedure Name:
        #      Alembic_getPrerollEndFrame
        #
        #  Description:
        #      Calculate the end frame of preroll.
        #
        #  Input Arguments:
        #      startFrame      - Start frame of preroll.
        #      cacheStartFrame - Start frame of first cache time range.
        #      prerollStep     - Step of preroll.
        #
        #  Return Value:
        #      The end frame of preroll.
        #
        def getPrerollEndFrame(self, startFrame, cacheStartFrame, prerollStep):

            preRollEndFrame = float(startFrame)
            while (preRollEndFrame + prerollStep) < cacheStartFrame:
                preRollEndFrame += prerollStep

            return preRollEndFrame

        #
        #  Procedure Name:
        #      Alembic_updateGlobalVariables
        #
        #  Description:
        #      Update global arrays when some rows are removed.
        #      After update, the array size is not shriked. Some elements in the end are invalid now.
        #
        #  Input Arguments:
        #      startIndex     - Index of the first cache time range that is removed.
        #      removedCount   - Count of cache time range that is removed.
        #
        #  Return Value:
        #      None.
        #
        def updateGlobalVariables(self, startIndex, removedCount):

            global gTmpCacheTimeRangeModes
            global gTmpSampleLowFrames
            global gTmpSampleHighFrames

            for i in range(startIndex, len(gTmpCacheTimeRangeModes)-removedCount):
                gTmpCacheTimeRangeModes[i] = gTmpCacheTimeRangeModes[i+removedCount]
                gTmpSampleLowFrames[i] = gTmpSampleLowFrames[i+removedCount]
                gTmpSampleHighFrames[i] = gTmpSampleHighFrames[i+removedCount]

        #
        #  Procedure Name:
        #      Alembic_adjustFrameRanges
        #
        #  Description:
        #      All time ranges are sorted according to their start/end frames.
        #      When start or end frame of one time range is changed, check and 
        #      change other time ranges that overlap with it.
        #
        #  Input Arguments:
        #      rangeIndex   - Index of cache time range changed.
        #      startFrame   - Start frame of the cache time range.
        #      endFrame     - End frame of the cache time range.
        #      dryRun       - if True, don't update the overlapping time ranges.
        #
        #  Return Value:
        #      If dryRun is True, return 1 if there is overlapping, otherwise return 0.
        #      If dryRun is false, return the new index for the cache time range.
        #
        def adjustFrameRanges(self, rangeIndex, startFrame, endFrame, dryRun=False):

            global gTmpCacheTimeRangeModes
            global gAllowEditTimeRangeInTable

            gAllowEditTimeRangeInTable = 1

            isOverlapping = False
            lastRowToRemoved = -1
            prerollEnabled = cmds.checkBoxGrp(self.enablePreRoll, q=True, value1=True)

            ri = rangeIndex + int(prerollEnabled) + 2
            totalRows = cmds.scriptTable(self.cacheTimeTable, q=True, rows=True)
            # check time ranges that are after the range
            #
            for i in range(totalRows-1):
                frames = self.getStartEndFramesOfRange(ri)
                if frames[1] <= endFrame:
                    if dryRun:
                        return True
                    lastRowToRemoved = ri
                else :
                    if frames[0] <= endFrame:
                        step = self.getEvaluateEveryOfRange(ri)
                        newStartFrame = frames[1]
                        while newStartFrame - step > endFrame:
                            newStartFrame -= step

                        if not dryRun:
                            isOverlapping = True
                            newTimeRange = '{}-{}'.format(newStartFrame, frames[1])
                            cmds.scriptTable(self.cacheTimeTable, edit=True, cellIndex=[ri, 1], cellValue=newTimeRange)
                            # change the range mode to start/end
                            gTmpCacheTimeRangeModes[ri-int(prerollEnabled)-1] = 3
                        else:
                            return True

                ri+=1

            removedRowCount = 0
            if not dryRun and lastRowToRemoved > 0:  # remove overlapping rows
                isOverlapping = True
                ri = lastRowToRemoved
                while ri > rangeIndex + int(prerollEnabled) + 1:
                    cmds.scriptTable(self.cacheTimeTable, edit=True, deleteRow=ri)
                    ri -= 1

                removedRowCount = lastRowToRemoved - (rangeIndex + int(prerollEnabled) + 1)
                self.updateGlobalVariables(rangeIndex+1, removedRowCount)

            # check time ranges that are before the range
            #
            if rangeIndex > 0:
                removedRowCount = 0
                ri = rangeIndex + int(prerollEnabled)
                while ri > int(prerollEnabled):
                    frames = self.getStartEndFramesOfRange(ri)
                    if frames[0] >= startFrame:
                        if not dryRun:
                            isOverlapping = True
                            # remove overlapping row
                            cmds.scriptTable(self.cacheTimeTable, edit=True, deleteRow=ri)
                            removedRowCount += 1
                        else:
                            return True

                    else:
                        if frames[1] >= startFrame:
                            step = self.getEvaluateEveryOfRange(ri)
                            newEndFrame = frames[0]
                            while newEndFrame + step < startFrame:
                                newEndFrame += step
                            if not dryRun:
                                isOverlapping = True
                                newTimeRange = "{}-{}".format(frames[0], newEndFrame)
                                cmds.scriptTable(self.cacheTimeTable, edit=True, cellIndex=[ri, 1], cellValue=newTimeRange)
                                # change the range mode to start/end
                                gTmpCacheTimeRangeModes[ri - int(prerollEnabled) -1] = 3
                            else:
                                return True
                    ri -= 1

                if removedRowCount > 0:
                    rangeIndex -= removedRowCount
                    self.updateGlobalVariables(rangeIndex, removedRowCount)

            gAllowEditTimeRangeInTable = 0

            if dryRun: return False
            else:
                if isOverlapping:
                    newRow = rangeIndex + int(prerollEnabled) + 1
                    cmds.scriptTable(self.cacheTimeTable, edit=True, selectedRows=[newRow])
                return rangeIndex

        #
        #  Procedure Name:
        #      Alembic_cacheTimeAfterCellChanged
        #
        #  Description:
        #      Callback after the value of a cell is changed.
        #
        #  Return Value:
        #      None.
        #
        def cacheTimeAfterCellChanged(self, row, column, value):
            # skip AddRange row
            totalRows = cmds.scriptTable(self.cacheTimeTable, query=True, rows=True)
            if totalRows-1 == row:
                return

            # adjust overlapping time ranges
            #
            if column == 1:
                # skip preRoll row
                preRollChecked = cmds.checkBoxGrp(self.enablePreRoll, qury=True, value1=True)
                if not preRollChecked or row != 1:
                    startEnd = self.getStartEndFramesOfRange(row)
                    newRangeIndex = self.adjustFrameRanges(row-1-preRollChecked, startEnd[0], startEnd[1], 0)

                    # if this row is the first time range, adjust preroll
                    if preRollChecked and newRangeIndex == 0:
                        preStartFrame = cmds.floatFieldGrp(self.preRollStartFrame, query=True, value1=True)
                        step = cmds.floatFieldGrp(self.preRollStep, query=True, value1=True)
                        preRollEndFrame = self.getPrerollEndFrame(preStartFrame, startEnd[0], step)

                        preRollTimeRangeVal = "{}-{}".format(preStartFrame, preRollEndFrame)
                        cmds.scriptTable(self.cacheTimeTable, edit=True, cellIndex=[1, 1], cellValue=preRollTimeRangeVal)

                    # select the new row if it is not the same with old row
                    newRow = newRangeIndex + 1 + int(preRollChecked)
                    if newRow != row:
                        row = newRow


            # convert digital number format, for example, from ".1" to "0.1"
            #
            value = value.strip()
            if column == 1:
                frames = stringToStringArray(value, "-")
                if frames[0].startswith(".") or frames[1].startswith("."):
                    newStr = ""
                    if frames[0].startswith("-"):
                        newStr = "-"

                    if frames[0].startswith("."):
                        newStr += "0"

                    newStr += frames[0]
                    newStr += "-"
                    if re.search(r"\-\-", frames[0]):
                        newStr += "-"
                    if frames[1].startswith("."):
                        newStr += "0"

                    newStr += frames[1]
                    cmds.scriptTable(self.cacheTimeTable, edit=True, cellIndex=[row, column], cellValue=newStr)

            elif column == 3:
                if value.startswith("."):
                    newStr = "0" + value
                    cmds.scriptTable(self.cacheTimeTable, edit=True, cellIndex=[row, column], cellValue=newStr)

            elif column == 4:
                samples = stringToStringArray(value, ";")
                needToConvert = 0
                for sam in samples:
                    if sam.strip().startsWith("."):
                        needToConvert = True

                if needToConvert:
                    newStr = ""
                    for sam in samples:
                        if sam.strip().startsWith("."):
                            newStr += "0"

                        newStr += sam.strip()
                        if sam == samples[-1]:
                            newStr += "; "

                    cmds.scriptTable(self.cacheTimeTable, edit=True, cellIndex=[row, column], cellValue=newStr)

        def createRangeDialogDismiss(self, checkBox, status):
            global gAlembicShowRangeAdjustDialog

            noMore = cmds.checkBox(checkBox, query=True, value=True)
            if noMore:
                gAlembicShowRangeAdjustDialog = status

            if status:
                cmds.layoutDialog(dismiss="continue")
            else:
                cmds.layoutDialog(dismiss="dismiss")

        def createRangeAdjustedDialog(self):
            form = cmds.setParent(query=True)

            cmds.formLayout(form, e=True)

            text = "Warning: One or more Alembic export time ranges are affected."
            contText = "Continue"
            reviewText = "Review Ranges"
            dontShowAgainText = "Do not show this message again."

            dontShowAgainCheckBox = cmds.checkBox(l=dontShowAgainText, v=0)
            textCtrl = cmds.text(l=text)
            callbackCmd = partial(self.createRangeDialogDismiss, checkBox=dontShowAgainCheckBox, status=1)
            contButton = cmds.button(l=contText, c=callbackCmd)
            callbackCmd = partial(self.createRangeDialogDismiss, checkBox=dontShowAgainCheckBox, status=0)
            reviewButton = cmds.button(l=reviewText, c=callbackCmd)

            cmds.checkBox(dontShowAgainCheckBox, e=True)

            spacer = 5
            top = 10
            edge = 10
            bottom = 10

            cmds.formLayout(form, edit=True,
                            attachForm=[        textCtrl,               "top",       10],
                            attachForm=[        textCtrl,               "left",      10],
                            attachControl=[     textCtrl,                "bottom",    5,     contButton],
                            attachNone=[        textCtrl,               "right"],
                            attachNone=[        contButton,             "top"],
                            attachForm=[        contButton,             "left",      10],
                            attachForm=[        contButton,             "bottom",    10],
                            attachPosition=[    contButton,             "right",     5,     20],
                            attachNone=[        reviewButton,           "top"],
                            attachControl=[     reviewButton,           "left",      5,     contButton],
                            attachForm=[        reviewButton,           "bottom",    10],
                            attachNone=[        reviewButton,           "right"],
                            attachNone=[        dontShowAgainCheckBox,  "top"],
                            attachPosition=[    dontShowAgainCheckBox,  "left",      5,     60],
                            attachForm=[        dontShowAgainCheckBox,  "bottom",    10],
                            attachPosition=[    dontShowAgainCheckBox,  "right",     5,     80]
                           )

        #
        #  Procedure Name:
        #      Alembic_updateCacheTimeRangeRow
        #
        #  Description:
        #      Update the row of time range in table.
        #      In the callback of afterCellChangedCmd, if the new start/end frame is
        #      overlapping with other ranges, rows of other ranges will be updated or
        #      removed.
        #
        #  Input Arguments:
        #      row         - Row number of the active time range.
        #
        #  Return Value:
        #      None.
        #
        def updateCacheTimeRangeRow(self, row):

            global gAllowEditTimeRangeInTable

            startFrame = cmds.floatFieldGrp(self.startEnd, q=True, value1=True)
            endFrame = cmds.floatFieldGrp(self.startEnd, q=True, value2=True)

            cacheTimeRangeVal = "{}-{}".format(startFrame, endFrame)
            gAllowEditTimeRangeInTable = 1
            cmds.scriptTable(self.cacheTimeTable, edit=True, cellIndex=[row, 1], cellValue=cacheTimeRangeVal)
            gAllowEditTimeRangeInTable = 0

        #
        #  Procedure Name:
        #      Alembic_exportCacheTimeFramesChanged
        #
        #  Description:
        #      Callback procedure when the start/end frame has been changed.
        #
        #  Input Arguments:
        #      parent    - Top level parent layout of the option box UI. Required so
        #                  that UI object names can be successfully resolved.
        #
        #  Return Value:
        #      None.
        #
        def exportCacheTimeFramesChanged(self, parent):
            cmds.setParent(parent)

            row = self.getActiveTimeRange()
            newRangeIndex = self.updateCacheTimeRangeRow(row)

        #
        #  Procedure Name:
        #      Alembic_addNewCacheTimeRange
        #
        #  Description:
        #      Add a new time range after the last time range.
        #
        #  Return Value:
        #      None.
        #
        def addNewCacheTimeRange(self):

            global gAllowEditTimeRangeInTable

            global gTmpCacheTimeRangeModes
            global gTmpSampleLowFrames
            global gTmpSampleHighFrames

            rowNum = cmds.scriptTable(self.cacheTimeTable, query=True, rows=True)
            lastRow = rowNum - 2
            rowNum -= 1
            cmds.scriptTable(self.cacheTimeTable, edit=True, insertRow=rowNum)

            preTimeRangeVal = cmds.scriptTable(self.cacheTimeTable, query=True, cellValue=True, cellIndex=[lastRow, 1])
            preTimes = stringToStringArray(preTimeRangeVal[0], "-")
            startTime = float(preTimes[1]) + 1
            endTime = startTime + 1
            newTimeRange = "{}-{}".formt(startTime, endTime)
            gAllowEditTimeRangeInTable = 1
            cmds.scriptTable(self.cacheTimeTable, edit=True, cellIndex=[rowNum, 1], cellValue=newTimeRange)
            gAllowEditTimeRangeInTable = 0
            cmds.scriptTable(self.cacheTimeTable, edit=True, cellIndex[rowNum, 3], cellValue=1)
            cmds.scriptTable(self.cacheTimeTable, edit=True, cellIndex[rowNum, 4], cellValue="0")

            rangeIndex = rowNum-1
            if cmds.checkBoxGrp(self.enablePreRoll, q=True, value1=True):
                rangeIndex -= 1

            gTmpCacheTimeRangeModes[rangeIndex] = 3
            gTmpSampleLowFrames[rangeIndex] = 0
            gTmpSampleHighFrames[rangeIndex] = 0

            cmds.scriptTable(self.cacheTimeTable, edit=True, selectedRows=rowNum)

        def cacheTimeRangeRemoved(self, rows):
            global gTmpCacheTimeRangeModes
            global gTmpSampleLowFrames
            global gTmpSampleHighFrames

            # remove the data for this time range row
            rangeIndex = rows[0]-1
            if cmds.checkBoxGrp(self.enablePreRoll, q=True, value1=True):
                rangeIndex -= 1
                if rangeIndex < 0:
                    # the row of preroll is removed
                    cmds.checkBoxGrp(self.enablePreRoll, e=True, value1=0)
                    cmds.floatFieldGrp(self.preRollStartFrame, e=True, enable=0)
                    cmds.floatFieldGrp(self.preRollStep, e=True, enable=0)

                    return

                if rangeIndex == 0:
                    # update end frame of preroll
                    firstFrameRange = self.getStartEndFramesOfRange(2)
                    startFrame = cmds.floatFieldGrp(self.preRollStartFrame, q=True, value1=True)
                    step = cmds.floatFieldGrp(self.preRollStep, query=True, value1=True)
                    preRollEndFrame = self.getPrerollEndFrame(startFrame, firstFrameRange[0], step)

                    preRollTimeRangeVal = "{}-{}".format(startFrame, preRollEndFrame)
                    cmds.scriptTable(self.cacheTimeTable, edit=True, cellIndex=[1, 1], cellValue=preRollTimeRangeVal)

            self.updateGlobalVariables(rangeIndex, 1)

            totalRows = cmds.scriptTable(self.cacheTimeTable, query=True, rows=True)
            if rows[0] == totalRows-1:
                # last time range is removed, select the first time range
                cmds.scriptTable(self.cacheTimeTable, edit=True, selectedRows=1)
            else:
                cmds.scriptTable(self.cacheTimeTable, edit=True, selectedRows=rows[0])

        def cacheTimeRangeToBeRemoved(self, rows):

            # only one row can be selected
            # the last row ("Add Range+) cannot be selected staticly, so
            # don't need to care it here.
            if rows[0] <= 2:
                preRollEnabled = cmds.checkBoxGrp(self.enablePreRoll, query=True, value1=True)
                totalRows = cmds.scriptTable(self.cacheTimeTable, query=True, rows=True)
                if totalRows-int(preRollEnabled) == 3 and rows[0]-int(preRollEnabled) == 1:
                    # only one time range in the table, which cannot be deleted
                    cmds.warning("Only one time range is defined, which cannot be deleted")
                    return False
            return True

        def setRangeModeUI(self, mode):
            if mode == 4:
                cmds.radioButtonGrp(self.rangeCurrentFrame, e=True, select=1)
                return
            elif mode == 1:
                cmds.radioButtonGrp(self.rangeRenderSettings, e=True, select=1)
                return
            elif mode == 2:
                cmds.radioButtonGrp(self.rangeTimeSlider, e=True, select=1)
                return
            elif mode ==  3:
                cmds.radioButtonGrp(self.rangeStartEnd, e=True, select=1)
                return

        #
        #  Procedure Name:
        #      Alembic_cacheTimeSelectionChanged
        #
        #  Description:
        #      Callback when selected row of time range table is changed.
        #      When the last row is selected (clicked), add a new time range.
        #
        #  Return Value:
        #      None.
        #
        def cacheTimeSelectionChanged(self):
            global gTmpCacheTimeRangeModes
            global gTmpSampleLowFrames
            global gTmpSampleHighFrames

            selectedRow = self.getActiveTimeRange()
            # if last row is selected, add a new range
            rowNum = cmds.scriptTable(self.cacheTimeTable, query=True, row=True)
            if rowNum-1 == selectedRow:
                self.addNewCacheTimeRange()
                return

            # update Generic Options to current selected row
            #
            rangeIndex = self.getActiveTimeRangeIndex()
            self.setRangeModeUI(gTmpCacheTimeRangeModes[rangeIndex])

            # setup start/end float fields
            #
            startEndEnabled = gTmpCacheTimeRangeModes[rangeIndex] == 3
            cmds.floatFieldGrp(self.startEnd, e=True, enable=startEndEnabled)
            startEnd = self.getStartEndFramesOfRange(selectedRow)
            cmds.floatFieldGrp(self.startEnd, e=True, value1=startEnd[0], value2=startEnd[1])

            step = self.getEvaluateEveryOfRange(selectedRow)
            cmds.floatFieldGrp(self.evaluateEvery, e=True, value1=step)

            sampleEnabled = self.getSampleEnabledOfRange(selectedRow)
            cmds.checkBoxGrp(self.enableFrameRelativeSample, e=True, value1=sampleEnabled)
            cmds.floatFieldGrp(self.lowFrameRelativeSample, e=True, enable=sampleEnabled, value1=gTmpSampleLowFrames[rangeIndex])
            cmds.floatFieldGrp(self.highFrameRelativeSample, e=True, enable=sampleEnabled, value1=gTmpSampleHighFrames[rangeIndex])

            if rangeIndex == 0:
                cmds.checkBoxGrp(self.enablePreRoll, e=True, enable=True)
                if cmds.checkBoxGrp(self.enablePreRoll, q=True, value1=True):
                    cmds.floatFieldGrp(self.preRollStartFrame, e=True, enable=1)
                    cmds.floatFieldGrp(self.preRollStep, e=True, enable=1)
            else:
                cmds.checkBoxGrp(self.enablePreRoll, e=True, enable=0)  # disable checkbox of preroll
                cmds.floatFieldGrp(self.preRollStartFrame, e=True, enable=0)
                cmds.floatFieldGrp(self.preRollStep, e=True, enable=0)

        #
        #  Procedure Name:
        #      Alembic_setupRowForPreRoll
        #
        #  Description:
        #      Set up the row for preRoll. The row is always the first row of the table.
        #
        #  Input Arguments:
        #      parent               - Top level parent layout of the option box UI.
        #                             Required so that UI object names can be
        #                             successfully resolved.
        #
        #  Return Value:
        #      None.
        #
        def setupRowForPreRoll(self, parent):
            cmds.setParent(parent)

            # get the first frame range from the second row
            firstFrameRange = self.getStartEndFramesOfRange(2)
            startFrame = cmds.floatFieldGrp(self.preRollStartFrame, q=True, value1=True)
            step = cmds.floatFieldGrp(self.preRollStep, query=True, value1=True)
            if step <= 0:
                cmds.warning("Step of preRoll cannot be 0 or negative.")
                step = self.getEvaluateEveryOfRange(1)
                cmds.floatFieldGrp(self.preRollStep, e=True, value1=step)
            else:
                cmds.scriptTable(self.cacheTimeTable, edit=True, cellIndex=[1, 3], cellValue=step)

            preRollEndFrame = self.getPrerollEndFrame(startFrame, firstFrameRange[0], step)

            preRollTimeRangeVal = "{}-{}".format(startFrame, preRollEndFrame)
            cmds.scriptTable(self.cacheTimeTable, edit=True, cellIndex=[1, 1], cellValue=preRollTimeRangeVal)
            yesLabel = self.getYesLabel()
            cmds.scriptTable(self.cacheTimeTable, edit=True, cellIndex=[1, 2], cellValue=yesLabel)

        #
        #  Procedure Name:
        #      Alembic_getStartEndFrames
        #
        #  Description:
        #      Get start and end frame according the range mode.
        #      If the mode is 3 (frames are specified directly), do not call this method.
        #
        #  Input Arguments:
        #      mode   - Mode of cache time range.
        #
        #  Return Value:
        #      The start and end frame.
        #
        def getStartEndFrames(self, mode):

            startEnd = 0.0
            if mode == 4:
                startEnd[0] = cmds.currentTime(q=True)
                startEnd[1] = startEnd[0]
            elif mode == 1:
                startEnd[0] = cmds.getAttr("defaultRenderGlobals.startFrame")
                startEnd[1] = cmds.getAttr("defaultRenderGlobals.endFrame")
            elif mode == 2:
                startEnd[0] = cmds.playbackOptions(q=Tue, min=True)
                startEnd[1] = cmds.playbackOptions(q=Tue, max=True)
            return startEnd

        #
        #  Procedure Name:
        #      Alembic_getOverlappingSolution
        #
        #  Description:
        #      Determine the resolve method for time ranges overlapping.
        #
        #  Return Value:
        #      1 if "Continue" is selected.
        #      0 if "Review Ranges" is selected.
        #
        def getOverlappingSolution(self):
            global gAlembicShowRangeAdjustDialog

            res = ""
            if gAlembicShowRangeAdjustDialog == -1:
                title = "Export Ranges Adjusted"
                res = cmds.layoutDialog(title=title, ui=self.createRangeAdjustedDialog)

            if gAlembicShowRangeAdjustDialog == 1 or res == "continue":
                return True
            else:
                return False

        #
        #  Procedure Name:
        #      Alembic_setupCacheTimeTable
        #
        #  Description:
        #      Create the cache time ranges table when initialize the UI.
        #      If there is time ranges specified by system settings, detect
        #      if the new setting values are overlapped with other time ranges
        #      and resolve it.
        #
        #  Input Arguments:
        #      parent               - Top level parent layout of the option box UI.
        #                             Required so that UI object names can be
        #                             successfully resolved.
        #
        #  Return Value:
        #      None.
        #
        def setupCacheTimeTable(self, parent):

            global gTmpCacheTimeRangeModes
            global gTmpSampleLowFrames
            global gTmpSampleHighFrames
            global gAllowEditTimeRangeInTable

            cmds.setParent(parent)

            # make sure there is only one row when reset settings
            # the title is counted as one row (seems a bug of scriptTable)
            totalRows = cmds.scriptTable(self.cacheTimeTable, query=True, rows=True)
            if totalRows > 2:
                i = 2
                while i < totalRows:
                    cmds.scriptTable(self.cacheTimeTable, e=True, deleteRow=1)
                    i += 1

            rowNum = 1
            preRollEnabled = cmds.optionVar( q="MTOA_Alembic_exportEnablePreRoll")
            if preRollEnabled:
                cmds.scriptTable(self.cacheTimeTable, edit=True, insertRow=1)
                rowNum += 1

            gTmpCacheTimeRangeModes = []
            gTmpSampleLowFrames = []
            gTmpSampleHighFrames = []

            gTmpCacheTimeRangeModes = cmds.optionVar(q="MTOA_Alembic_exportCacheTimeRanges")
            startFrames = cmds.optionVar(q="MTOA_Alembic_exportStarts")
            endFrames = cmds.optionVar(q="MTOA_Alembic_exportEnds")
            sampleEnabled = cmds.optionVar(q="MTOA_Alembic_exportEnableFrameRelativeSamples")
            gTmpSampleLowFrames = cmds.optionVar(q="MTOA_Alembic_exportLowFrameRelativeSamples")
            gTmpSampleHighFrames = cmds.optionVar(q="MTOA_Alembic_exportHighFrameRelativeSamples")
            evaluateEvery = cmds.optionVar(q="MTOA_Alembic_exportEvaluateEverys")

            # check time ranges and resolve overlapping
            #
            overlappingResolve = -1
            rangeRemovedNum = 0
            i = 0
            while i < len(gTmpCacheTimeRangeModes)-rangeRemovedNum:
                if gTmpCacheTimeRangeModes[i] != 3:
                    # if the mode of time range is not start/end, get current time ranges
                    # according system settings
                    #
                    startEnd = self.getStartEndFrames(gTmpCacheTimeRangeModes[i])
                    if startEnd[0] != startFrames[i] or startEnd[1] != endFrames[i]:
                        # if current time ranges is not equal to the old one
                        # check if the new time ranges are overlapping
                        isOverlapping = False
                        if i > 0 and startEnd[0] <= endFrames[i-1]:
                            isOverlapping = True

                        if i < size(gTmpCacheTimeRangeModes)-1 and startEnd[1] >= startFrames[i+1]:
                            isOverlapping = True

                        if isOverlapping:
                            if overlappingResolve == -1:
                                # get the overlapping resolve method
                                # the resolve method will be used for other time ranges conflict
                                overlappingResolve = self.getOverlappingSolution()

                            if overlappingResolve == 1:
                                startFrames[i] = startEnd[0]
                                endFrames[i] = startEnd[1]

                                # if user select "Continue", resolve conflict
                                ri = i+1
                                while ri < len(gTmpCacheTimeRangeModes):
                                    if startEnd[1] >= endFrames[ri]:
                                        rangeRemovedNum +=1
                                    else:
                                        if startEnd[1] >= startFrames[ri]:
                                            startFrames[ri] = endFrames[ri];
                                            while startFrames[ri] - evaluateEvery[ri] > startEnd[1]:
                                                startFrames[ri] -= evaluateEvery[ri]
                                            gTmpCacheTimeRangeModes[ri] = 3
                                        break
                                    ri+=1

                                if rangeRemovedNum > 0:
                                    self.updateGlobalVariables(i+1, rangeRemovedNum)
                                    j = i+1
                                    while j < size(gTmpCacheTimeRangeModes)-rangeRemovedNum:
                                        startFrames[j] = startFrames[j+rangeRemovedNum]
                                        endFrames[j] = endFrames[j+rangeRemovedNum]
                                        sampleEnabled[j] = sampleEnabled[j+rangeRemovedNum]
                                        evaluateEvery[j] = evaluateEvery[j+rangeRemovedNum]
                                        j += 1

                                preRangeRemovedNum = 0
                                ri = i-1
                                while ri >= 0:
                                    if startEnd[0] <= startFrames[ri]:
                                        rangeRemovedNum += 1
                                        preRangeRemovedNum += 1
                                    else:
                                        if startEnd[0] <= endFrames[ri]:
                                            endFrames[ri] = startFrames[ri]
                                            while endFrames[ri] + evaluateEvery[ri] < startEnd[0]:
                                                endFrames[ri] += evaluateEvery[ri]
                                            gTmpCacheTimeRangeModes[ri] = 3
                                        break
                                    ri -= 1

                                if ri != i-1:
                                    num = i-1-ri;
                                    self.updateGlobalVariables(ri+1, num)
                                    j = ri+1
                                    while j < size(gTmpCacheTimeRangeModes)-num:
                                        startFrames[j] = startFrames[j+num]
                                        endFrames[j] = endFrames[j+num]
                                        sampleEnabled[j] = sampleEnabled[j+num]
                                        evaluateEvery[j] = evaluateEvery[j+num]
                                        j += 1

                                # we will check from next new range
                                i = i - preRangeRemovedNum
                            else:
                                # if user select "Review Ranges", change the mode to start/end
                                gTmpCacheTimeRangeModes[i] = 3
                        else:
                            startFrames[i] = startEnd[0]
                            endFrames[i] = startEnd[1]
                i += 1

            firstRowNum = rowNum

            i = 0
            while i < len(gTmpCacheTimeRangeModes)-rangeRemovedNum:
                cmds.scriptTable(self.cacheTimeTable, edit=True, insertRow=rowNum)

                frameSample = "0"
                if sampleEnabled[i] > 0:
                    frameSample = "{}; 0; {}".format(gTmpSampleLowFrames[i], gTmpSampleHighFrames[i])

                cacheTimeRangeVal = "{}-{}".format(startFrames[i], endFrames[i])

                gAllowEditTimeRangeInTable = 1
                cmds.scriptTable(self.cacheTimeTable, edit=True, cellIndex=[rowNum, 1], cellValue=cacheTimeRangeVal)
                gAllowEditTimeRangeInTable = 0
                cmds.scriptTable(self.cacheTimeTable, edit=True, cellIndex=[rowNum, 3], cellValue=evaluateEvery[i])
                cmds.scriptTable(self.cacheTimeTable, edit=True, cellIndex=[rowNum, 4], cellValue=frameSample)

                rowNum += 1

            if preRollEnabled:
                self.setupRowForPreRoll(parent)

            gAllowEditTimeRangeInTable = 1
            cmds.scriptTable(self.cacheTimeTable, edit=True, cellIndex=[rowNum, 1], cellValue="Add Range+")
            gAllowEditTimeRangeInTable = 0

            cmds.scriptTable(self.cacheTimeTable, edit=True, selectedRows=firstRowNum)

        #
        #  Procedure Name:
        #      Alembic_exportSetup
        #
        #  Description:
        #      Update the state of the option box UI to reflect the option values.
        #
        #  Input Arguments:
        #      parent               - Top level parent layout of the option box UI.
        #                             Required so that UI object names can be
        #                             successfully resolved.
        #
        #      forceFactorySettings - Whether the option values should be set to
        #                             default values.
        #
        #  Return Value:
        #      None.
        #
        def exportSetup(self, parent, forceFactorySettings):
            #  Retrieve the option settings
            #
            cmds.setOptionVars(forceFactorySettings)

            cmds.setParent(parent)

            #  Query the preroll optionVar's and set the values into the controls.
            #
            preRollEnabled = cmds.optionVar(q="MTOA_Alembic_exportEnablePreRoll")
            cmds.checkBoxGrp(self.enablePreRoll, e=True, value1=preRollEnabled)
            cmds.floatFieldGrp(self.preRollStartFrame, e=True, value1=cmds.optionVar(q="MTOA_Alembic_exportPreRollStartFrame"), enable=preRollEnabled)
            cmds.floatFieldGrp(self.preRollStep, e=True, value1=cmds.optionVar(q="MTOA_Alembic_exportPreRollStep"), enable=preRollEnabled)

            # General Options UI will be updated when setup the table
            self.setupCacheTimeTable(parent)

            attrArray = attrPrefixArray = []
            attr = cmds.optionVar(q="MTOA_Alembic_exportAttr")
            attrPrefix = cmds.optionVar(q="MTOA_Alembic_exportAttrPrefix")
            attrArray = attr.split(",")
            attrPrefixArray = attrPrefix.split(",") 
            cmds.textScrollList(self.attrList, e=True, removeAll=True)
            cmds.textScrollList(self.attrPrefixList, e=True, removeAll=True)

            for a in attrArray:
                    cmds.textScrollList(self.attrList, e=True, append=a)

            for ap in attrPrefixArray:
                    cmds.textScrollList(self.attrPrefixList, e=True, append=ap)

            cmds.checkBoxGrp(self.verbose, e=True, value1=cmds.optionVar(q="MTOA_Alembic_exportVerbose"))
            cmds.checkBoxGrp(self.noNormals, e=True, value1=cmds.optionVar(q="MTOA_Alembic_exportNoNormals"))
            cmds.checkBoxGrp(self.renderableOnly, e=True, value1=cmds.optionVar(q="MTOA_Alembic_exportRenderableOnly"))
            cmds.checkBoxGrp(self.stripNamespaces, e=True, value1=cmds.optionVar(q="MTOA_Alembic_exportStripNamespaces"))
            cmds.checkBoxGrp(self.uvWrite, e=True, value1=cmds.optionVar(q="MTOA_Alembic_exportUVWrite"))
            cmds.checkBoxGrp(self.writeColorSets, e=True, value1=cmds.optionVar(q="MTOA_Alembic_exportWriteColorSets"))
            cmds.checkBoxGrp(self.writeFaceSets, e=True, value1=cmds.optionVar(q="MTOA_Alembic_exportWriteFaceSets"))
            cmds.checkBoxGrp(self.wholeFrameGeo, e=True, value1=cmds.optionVar(q="MTOA_Alembic_exportWholeFrameGeo"))
            cmds.checkBoxGrp(self.worldSpace, e=True, value1=cmds.optionVar(q="MTOA_Alembic_exportWorldSpace"))
            cmds.checkBoxGrp(self.writeVisibility, e=True, value1=cmds.optionVar(q="MTOA_Alembic_exportWriteVisibility"))
            cmds.checkBoxGrp(self.filterEulerRotations, e=True, value1=cmds.optionVar(q="MTOA_Alembic_exportFilterEulerRotations"))
            cmds.checkBoxGrp(self.writeCreases, e=True, value1=cmds.optionVar(q="MTOA_Alembic_exportWriteCreases"))
            cmds.checkBoxGrp(self.writeUVSets, e=True, value1=cmds.optionVar(q="MTOA_Alembic_exportWriteUVSets"))

            dataFormat = cmds.optionVar(q="MTOA_Alembic_exportDataFormat")
            if dataFormat == 1:
                cmds.radioButtonGrp(self.dataFormatHDF5, e=True, select=1)
            elif dataFormat == 2:
                cmds.radioButtonGrp(self.dataFormatOgawa, e=True, select=1)

            cmds.textFieldGrp(self.perFrameCallbackMel, e=True, text=cmds.optionVar(q="MTOA_Alembic_exportPerFrameCallbackMel"))
            cmds.textFieldGrp(self.postJobCallbackMel, e=True, text=cmds.optionVar(q="MTOA_Alembic_exportPostJobCallbackMel"))
            cmds.textFieldGrp(self.perFrameCallbackPython, e=True, text=cmds.optionVar(q="MTOA_Alembic_exportPerFrameCallbackPython"))
            cmds.textFieldGrp(self.postJobCallbackPython, e=True, text=cmds.optionVar(q="MTOA_Alembic_exportPostJobCallbackPython"))

        #
        #  Procedure Name:
        #      Alembic_exportCallback
        #
        #  Description:
        #      Update the option values with the current state of the option box UI.
        #
        #  Input Arguments:
        #      parent    - Top level parent layout of the option box UI. Required so
        #                  that UI object names can be successfully resolved.
        #
        #      exportAll - Whether to export the entire scene or only selected objects.
        #
        #      doIt      - Whether the command should execute.
        #
        #  Return Value:
        #      None.
        #
        def exportCallback(self, parent, exportAll, doIt):
            global gTmpCacheTimeRangeModes
            global gTmpSampleLowFrames
            global gTmpSampleHighFrames

            cmds.setParent(parent)

            #  Set the optionVar's from the time range table
            cmds.optionVar(clearArray="MTOA_Alembic_exportCacheTimeRanges")
            cmds.optionVar(clearArray="MTOA_Alembic_exportStarts")
            cmds.optionVar(clearArray="MTOA_Alembic_exportEnds")
            cmds.optionVar(clearArray="MTOA_Alembic_exportEvaluateEverys")
            cmds.optionVar(clearArray="MTOA_Alembic_exportEnableFrameRelativeSamples")
            cmds.optionVar(clearArray="MTOA_Alembic_exportLowFrameRelativeSamples")
            cmds.optionVar(clearArray="MTOA_Alembic_exportHighFrameRelativeSamples")

            totalRows = cmds.scriptTable(self.cacheTimeTable, query=True, rows=True)
            proRollEnabled = cmds.checkBoxGrp(self.enablePreRoll, q=True, value1=True)
            i = 0
            curRow = int(proRollEnabled)+1
            while i < totalRows-2-int(proRollEnabled):
                startEndFrames = self.getStartEndFramesOfRange(curRow)
                cmds.optionVar(floatValueAppend=["MTOA_Alembic_exportStarts", startEndFrames[0]])
                cmds.optionVar(floatValueAppend=["MTOA_Alembic_exportEnds", startEndFrames[1]])

                step = self.getEvaluateEveryOfRange(curRow)
                cmds.optionVar(floatValueAppend=["MTOA_Alembic_exportEvaluateEverys", step])

                sampleEnabled = self.getSampleEnabledOfRange(curRow)
                cmds.optionVar(intValueAppend=["MTOA_Alembic_exportEnableFrameRelativeSamples", sampleEnabled])
                cmds.optionVar(intValueAppend=["MTOA_Alembic_exportCacheTimeRanges", gTmpCacheTimeRangeModes[i]])
                cmds.optionVar(floatValueAppend=["MTOA_Alembic_exportLowFrameRelativeSamples", gTmpSampleLowFrames[i]])
                cmds.optionVar(floatValueAppend=["MTOA_Alembic_exportHighFrameRelativeSamples", gTmpSampleHighFrames[i]])
                i += 1
                curRow += 1

            #  Set the optionVar's from the control values, and then
            #  perform the command.
            cmds.optionVar(intValue=["MTOA_Alembic_exportEnablePreRoll", proRollEnabled])
            cmds.optionVar(floatValue=["MTOA_Alembic_exportPreRollStartFrame", cmds.floatFieldGrp(self.preRollStartFrame, q=True, value1=True)])
            cmds.optionVar(floatValue=["MTOA_Alembic_exportPreRollStep", cmds.floatFieldGrp(self.preRollStep, q=True, value1=True)])

            attrArray = cmds.textScrollList(self.attrList, q=True, allItems=True)
            attrPrefixArray = cmds.textScrollList(self.attrPrefixList, q=True, allItems=True)

            attr = ','.join(attrArray)
            attrPrefix = ','.join(attrPrefixArray)

            cmds.optionVar(stringValue=["MTOA_Alembic_exportAttr",       attr])
            cmds.optionVar(stringValue=["MTOA_Alembic_exportAttrPrefix", attrPrefix])

            cmds.optionVar(intValue=["MTOA_Alembic_exportVerbose", cmds.checkBoxGrp(self.verbose, q=True, value1=True)])
            cmds.optionVar(intValue=["MTOA_Alembic_exportNoNormals", cmds.checkBoxGrp(self.noNormals, q=True, value1=True)])
            cmds.optionVar(intValue=["MTOA_Alembic_exportRenderableOnly", cmds.checkBoxGrp(self.renderableOnly, q=True, value1=True)])
            cmds.optionVar(intValue=["MTOA_Alembic_exportStripNamespaces", cmds.checkBoxGrp(self.stripNamespaces, q=True, value1=True)])
            cmds.optionVar(intValue=["MTOA_Alembic_exportUVWrite", cmds.checkBoxGrp(self.uvWrite, q=True, value1=True)])
            cmds.optionVar(intValue=["MTOA_Alembic_exportWriteColorSets", cmds.checkBoxGrp(self.writeColorSets, q=True, value1=True)])
            cmds.optionVar(intValue=["MTOA_Alembic_exportWriteFaceSets", cmds.checkBoxGrp(self.writeFaceSets, q=True, value1=True)])
            cmds.optionVar(intValue=["MTOA_Alembic_exportWholeFrameGeo", cmds.checkBoxGrp(self.wholeFrameGeo, q=True, value1=True)])
            cmds.optionVar(intValue=["MTOA_Alembic_exportWorldSpace", cmds.checkBoxGrp(self.worldSpace, q=True, value1=True)])
            cmds.optionVar(intValue=["MTOA_Alembic_exportWriteVisibility", cmds.checkBoxGrp(self.writeVisibility, q=True, value1=True)])
            cmds.optionVar(intValue=["MTOA_Alembic_exportFilterEulerRotations", cmds.checkBoxGrp(self.filterEulerRotations, q=True, value1=True)])
            cmds.optionVar(intValue=["MTOA_Alembic_exportWriteCreases", cmds.checkBoxGrp(self.writeCreases, q=True, value1=True)])
            cmds.optionVar(intValue=["MTOA_Alembic_exportWriteUVSets", cmds.checkBoxGrp(self.writeUVSets, q=True, value1=True)])


            dataFormat = 1
            if cmds.radioButtonGrp(self.dataFormatHDF5, q=True, select=True):
                dataFormat = 1
            elif cmds.radioButtonGrp(self.dataFormatOgawa, q=True, select=True):
                dataFormat = 2

            cmds.optionVar(intValue=["MTOA_Alembic_exportDataFormat", dataFormat])

            cmds.optionVar(stringValue=["MTOA_Alembic_exportPerFrameCallbackMel", cmds.textFieldGrp(self.perFrameCallbackMel, q=True, text=True)])
            cmds.optionVar(stringValue=["MTOA_Alembic_exportPostJobCallbackMel", cmds.textFieldGrp(self.postJobCallbackMel, q=True, text=True)])
            cmds.optionVar(stringValue=["MTOA_Alembic_exportPerFrameCallbackPython", cmds.textFieldGrp(self.perFrameCallbackPython, q=True, text=True)])
            cmds.optionVar(stringValue=["MTOA_Alembic_exportPostJobCallbackPython", cmds.textFieldGrp(self.postJobCallbackPython, q=True, text=True)])

            if doIt:
                performAlembicExport(ABC_ACTION_EXEC, exportAll)

        #
        #  Procedure Name:
        #      exportOptionsUI
        #
        #  Description:
        #      Construct the UI for option box or file options.
        #
        #  Input Arguments:
        #      parent    - Top level parent layout of the option box or file options UI.
        #                  Required so that UI object names can be successfully resolved.
        #
        #  Return Value:
        #      Top level layout of the options UI.
        #
        def exportOptionsUI(self, parent):
            #  The argument is used as the parent for the options UI
            #
            cmds.setParent(parent)

            #  Activate the default UI template so that the layout of this
            #  option box is consistent with the layout of the rest of the
            #  application.
            #
            cmds.setUITemplate("MtoAAlembicDefaultTemplate", pushTemplate=True)

            #  Turn on the wait cursor.
            #
            cmds.waitCursor(state=True)

            # Create option box or file options contents.
            #
            optionLayout = cmds.scrollLayout(childResizable=True)
            cmds.columnLayout(adjustableColumn=True)

            secondLevelIndent = 190

            cmds.frameLayout(label="General Options", collapse=False)
            cmds.columnLayout()
            self.rangeCurrentFrame = cmds.radioButtonGrp(
                    numberOfRadioButtons=1,
                    label="Cache time range:",
                    label1="Current frame",
                    onCommand1=partial(self.exportCacheTimeRangeChanged, optionLayout, 0))
            self.rangeRenderSettings = cmds.radioButtonGrp(
                    numberOfRadioButtons=1,
                    label1="Render settings",
                    shareCollection=rangeCurrentFrame,
                    onCommand1=partial(self.exportCacheTimeRangeChanged, optionLayout, 0))
            self.rangeTimeSlider = cmds.radioButtonGrp(
                    numberOfRadioButtons=1,
                    label1="Time Slider",
                    shareCollection=rangeCurrentFrame,
                    onCommand1=partial(self.exportCacheTimeRangeChanged, optionLayout, 0))
            self.rangeStartEnd = cmds.radioButtonGrp(
                    numberOfRadioButtons=1,
                    label1=_L10N(kStartEnd,"Start/End"),
                    shareCollection=rangeCurrentFrame,
                    onCommand1=partial(self.exportCacheTimeRangeChanged, optionLayout, 0))
            self.startEnd = cmds.floatFieldGrp(
                    numberOfFields=2,
                    label="Start/End",
                    changeCommand=partial(self.exportCacheTimeFramesChanged, optionLayout))
            self.evaluateEvery = cmds.floatFieldGrp(
                    numberOfFields=1,
                    label="Step:",
                    changeCommand=partial(self.exportEvaluateEveryChanged, optionLayout))

            cmds.separator(height=10, style="none")

            self.enableFrameRelativeSample = cmds.checkBoxGrp(
                    numberOfCheckBoxes=1,
                    label="Frame relative sample:",
                    changeCommand1=partial(self.exportEnableFrameRelativeSampleChanged, optionLayout))
            self.lowFrameRelativeSample = cmds.floatFieldGrp(
                    numberOfFields=1,
                    label="Low",
                    columnWidth=[1, secondLevelIndent],
                    changeCommand=partial(self.frameRelativeSampleChanged, optionLayout))
            self.highFrameRelativeSample = cmds.floatFieldGrp(
                    numberOfFields=1,
                    label="High",
                    columnWidth=[1, secondLevelIndent],
                    changeCommand=partial(self.frameRelativeSampleChanged, optionLayout))

            cmds.separator(height=10, style="none")

            self.enablePreRoll = cmds.checkBoxGrp(
                    numberOfCheckBoxes=1,
                    label="Pre-Roll:",
                    changeCommand1=partial(self.exportEnablePreRollChanged, optionLayout))
            self.preRollStartFrame = cmds.floatFieldGrp(
                    numberOfFields=1,
                    label="Start:",
                    columnWidth=[1, secondLevelIndent],
                    changeCommand=partial(self.setupRowForPreRoll, optionLayout))
            self.preRollStep = cmds.floatFieldGrp(
                    numberOfFields=1,
                    label="Step:",
                    columnWidth=[1, secondLevelIndent],
                    changeCommand=partial(self.setupRowForPreRoll, optionLayout))
            cmds.setParent("..")
            cmds.setParent("..")
            
            cmds.frameLayout(label="Cache Time Ranges", collapse=False)
            self.tableForm = cmds.formLayout()
            self.cacheTimeTable = cmds.scriptTable(
                columns=4,
                rows=1,
                height=140,
                editable=True,
                useDoubleClickEdit=True,
                selectionBehavior=1,
                selectionMode=1,
                selectionChangedCmd=self.cacheTimeSelectionChanged,
                rowsToBeRemovedCmd=self.cacheTimeRangeToBeRemoved,
                rowsRemovedCmd=self.cacheTimeRangeRemoved,
                label=[1,"Cache Time"],
                label=[2,"Pre Roll"],
                label=[3,"Step"],
                label=[4,"Subsamples"],
                cellChangedCmd=self.cacheTimeCellChanged,
                afterCellChangedCmd=self.cacheTimeAfterCellChanged,
                multiEditEnabled=false)
            
            cmds.formLayout(self.tableForm, edit=True,
                attachForm= [self.cacheTimeTable, "top", 0],
                attachForm= [self.cacheTimeTable, "left", 0],
                attachForm= [self.cacheTimeTable, "right", 0],
                attachForm= [self.cacheTimeTable, "bottom", 0])

            cmds.setParent("..")    
            cmds.setParent("..")
            
            cmds.frameLayout(label="Attributes", collapse=True, marginHeight=3, marginWidth=3)
            cmds.columnLayout(adjustableColumn=1, rowSpacing=3)
            cmds.rowLayout(
                numberOfColumns=4,
                columnWidth4=[90, 120, 95, 95],
                columnAttach4=["left", "left", "right", "right"],
                columnAlign=[1, "left"])
            cmds.text(
                label="Attribute:")
            self.attrField = cmds.textField(
                changeCommand=partial(self.exportAttrAddRemove, 0, optionLayout))
            cmds.button(
                label="Add",
                width=90,
                command=partial(self.exportAttrAddRemove, 0, optionLayout))
            cmds.button(
                label="Remove",
                width=90,
                command=partial(self.exportAttrAddRemove, 1, optionLayout))

            cmds.setParent("..")

            self.attrList = cmds.textScrollList(
                    numberOfRows=8,
                    allowMultiSelection=1,
                    height=200)

            cmds.rowLayout(
                numberOfColumns=3,
                columnWidth3=[115, 115, 115],
                columnAttach3=["left", "right", "right"])
            cmds.button(
                label="From Channel Box",
                width=130,
                command=partial(self.exportAttrGetChannels, optionLayout))
            cmds.button(
                label="Remove Selected",
                width=130,
                command=partial(self.exportAttrRemove, 0, optionLayout))
            cmds.button(
                label="Remove All",
                width=130,
                command=partial(self.exportAttrRemove, 1, optionLayout))
            cmds.setParent("..")
            cmds.setParent("..")
            cmds.setParent("..")

            cmds.frameLayout(label="Attribute Prefix", collapse=0, marginHeight=3, marginWidth=3)
            cmds.columnLayout(adjustableColumn=1, rowSpacing=3)
            cmds.rowLayout(
                numberOfColumns=4,
                columnWidth4=[90, 120, 95, 95],
                columnAttach4=["left", "left", "right", "right"],
                columnAlign=[1, "left"])
            cmds.text(
                label="Attribute Prefix:")
            self.attrPrefixField = cmds.textField(
                changeCommand=partial(self.exportAttrPrefixAddRemove, 0, optionLayout))
            cmds.button(
                label="Add",
                width=90,
                command=partial(self.exportAttrPrefixAddRemove, 0, optionLayout))
            cmds.button(
                label="Remove",
                width=90,
                command=partial(self.exportAttrPrefixAddRemove, 1, optionLayout))
            cmds.setParent("..")

            self.attrPrefixList = cmds.textScrollList(
                    numberOfRows=8,
                    allowMultiSelection=1,
                    height=200)

            cmds.rowLayout(
                numberOfColumns=2,
                columnWidth2=[115, 115],
                columnAttach2=["left", "left"])
            cmds.button(
                label="Remove Selected",
                width=110,
                command=partial(self.exportAttrPrefixRemove, 0, optionLayout))
            cmds.button(
                label="Remove All",
                width=110,
                command=partial(self.exportAttrPrefixRemove, 1, optionLayout))
            cmds.setParent("..")
            cmds.setParent("..")
            cmds.setParent("..")

            cmds.frameLayout(label="Advanced Options", collapse=False)
            cmds.columnLayout()
            self.verbose = cmds.checkBoxGrp(
                    numberOfCheckBoxes=1,
                    label1="Verbose")
            self.noNormals = cmds.checkBoxGrp(
                    numberOfCheckBoxes=1,
                    label1="No Normals")
            self.renderableOnly = cmds.checkBoxGrp(
                    numberOfCheckBoxes=1,
                    label1="Renderable Onl")
            self.stripNamespaces = cmds.checkBoxGrp(
                    numberOfCheckBoxes=1,
                    label1="Strip Namespaces")
            self.uvWrite = cmds.checkBoxGrp(
                    numberOfCheckBoxes=1,
                    label1="UV Write")
            self.writeColorSets = cmds.checkBoxGrp(
                    numberOfCheckBoxes=1,
                    label1="Write Color Sets")
            self.writeFaceSets = cmds.checkBoxGrp(
                    numberOfCheckBoxes=1,
                    label1="Write Face Sets")
            self.wholeFrameGeo = cmds.checkBoxGrp(
                    numberOfCheckBoxes=1,
                    label1="Whole Frame Geo")
            self.worldSpace = cmds.checkBoxGrp(
                    numberOfCheckBoxes=1,
                    label1="World Space")
            self.writeVisibility = cmds.checkBoxGrp(
                    numberOfCheckBoxes=1,
                    label1="Write Visibility")
            self.filterEulerRotations = cmds.checkBoxGrp(
                    numberOfCheckBoxes=1,
                    label1="Filter Euler Rotations")
            self.writeCreases = cmds.checkBoxGrp(
                    numberOfCheckBoxes=1,
                    label1="Write Creases")
            self.writeUVSets = cmds.checkBoxGrp(
                    numberOfCheckBoxes=1,
                    label1="Write UV Sets")

            self.dataFormatHDF5 = cmds.radioButtonGrp(
                    numberOfRadioButtons=1,
                    label="File Format:",
                    label1="HDF5 - Maya 2014 Extension 1 and Legacy",
                    annotation="Creates an Alembic file in a legacy format readable by any Maya version using the Alembic.")
            self.dataFormatOgawa = cmds.radioButtonGrp(
                    numberOfRadioButtons=1,
                    label1="Ogawa - Maya 2014 Extension 1",
                    annotation="Creates an Alembic 1.5 file usable only by Maya 2014 Extension 1 and future versions of Maya.",
                    shareCollection=self.dataFormatHDF5)
            cmds.setParent("..")
            cmds.setParent("..")

            cmds.frameLayout(label="Callbacks", collapse=False)
            cmds.columnLayout()
            self.perFrameCallbackMel = cmds.textFieldGrp(
                    label="Per Frame Callback MEL:")
            self.postJobCallbackMel = cmds.textFieldGrp(
                    label="Post Job Callback MEL:")
            self.perFrameCallbackPython = cmds.textFieldGrp(
                    label="Per Frame Callback Python:")
            self.postJobCallbackPython = cmds.textFieldGrp(
                    label="Post Job Callback Python:")
            cmds.setParent("..")
            cmds.setParent("..")

            # Attach callbacks to update Start/End fields.
            #
            cmds.scriptJob(parent=optionLayout,
                event=["timeChanged", partial(self.activeCacheTimeRangeChanged, optionLayout, 4)])
            cmds.scriptJob(parent=optionLayout,
                event=["playbackRangeChanged", partial(self.activeCacheTimeRangeChanged, optionLayout, 2)])
            cmds.scriptJob(parent=optionLayout,
                attributeChange=["defaultRenderGlobals.startFrame", partial(self.activeCacheTimeRangeChanged, optionLayout, 1)])
            cmds.scriptJob(parent=optionLayout,
                attributeChange=["defaultRenderGlobals.endFrame", partial(self.activeCacheTimeRangeChanged, optionLayout, 1)])

            #  Turn off the wait cursor.
            #
            cmds.waitCursor(state=0)

            # Deactivate the default UI template
            #
            cmds.setUITemplate(popTemplate=True)

            return optionLayout

        #
        #  Procedure Name:
        #      Alembic_activeCacheTimeRangeChanged
        #
        #  Description:
        #      Callback procedure when Cache Time Range specified by system settings 
        #      is changed.
        #
        #  Input Arguments:
        #      parent    - Top level parent layout of the option box or file options UI.
        #                  Required so that UI object names can be successfully resolved.
        #
        #  Return Value:
        #      None.
        #
        def activeCacheTimeRangeChanged(self, parent, mode):
            global gTmpCacheTimeRangeModes

            cmds.setParent(parent)

            preRollEnabled = cmds.checkBoxGrp(self.enablePreRoll, query=True, value1=True)
            totalRows = cmds.scriptTable(self.cacheTimeTable, query=True, rows=True)
            rangeCount = totalRows - int(prerollEnabled) - 2
            for i in range(rangeCount):
                if gTmpCacheTimeRangeModes[i] == mode:
                    row = i + int(prerollEnabled) + 1
                    # make the row active
                    cmds.scriptTable(self.cacheTimeTable, edit=True, selectedRows=row)
                    self.exportCacheTimeRangeChanged(parent, 1)
                    break

        #
        #  Procedure Name:
        #      Alembic_exportCacheTimeRangeChanged
        #
        #  Description:
        #      Callback procedure when the state of Cache Time Range radio
        #      cmds.button group has been changed.
        #
        #  Input Arguments:
        #      parent    - Top level parent layout of the option box or file options UI.
        #                  Required so that UI object names can be successfully resolved.
        #
        #  Return Value:
        #      None.
        #
        def exportCacheTimeRangeChanged(self, parent, checkOveralpping):
        {
            global int   gTmpCacheTimeRangeModes[];

            setParent parent;

            int mode;
            if (`radioButtonGrp -q -select rangeRenderSettings`:
                mode = 1;
            }
            else if (`radioButtonGrp -q -select rangeTimeSlider`:
                mode = 2;
            }
            else if (`radioButtonGrp -q -select rangeCurrentFrame`:
                mode = 4;
            }
            else {
                mode = 3;
            }

            # Update start/end float fields.
            #
            int rangeIndex = self.getActiveTimeRangeIndex();
            if (mode != 3:
                # check duplicated mode
                #
                preRollEnabled = cmds.checkBoxGrp(self.enablePreRoll, query=True, value1=True)
                int totalRows = `cmds.scriptTable(self.cacheTimeTable, query=True -rows cacheTimeTable`;
                int rangeCount = totalRows - prerollEnabled - 2;
                for (i = 0; i < rangeCount; i++:
                    if (gTmpCacheTimeRangeModes[i] == mode and
                        i != rangeIndex:
                        warning _L10N(kDuplicatedMode, "There is a time range with this mode already.");
                        Alembic_setRangeModeUI (gTmpCacheTimeRangeModes[rangeIndex]);
                        return;
                    }
                }

                float startEnd[] = self.getStartEndFrames(mode);
                if (checkOveralpping:
                    if (Alembic_adjustFrameRanges(rangeIndex, startEnd[0], startEnd[1], 1))
                    {
                        if (Alembic_getOverlappingSolution() == 0:
                            # abort the user operation and show the dialog
                            #
                            float start = `floatFieldGrp -q -value1 startEnd`;
                            float end = `floatFieldGrp -q -value2 startEnd`;
                            switch (mode:
                                case 1:
                                        setAttr defaultRenderGlobals.startFrame start;
                                        setAttr defaultRenderGlobals.endFrame end;
                                        break;
                                case 2:
                                        playbackOptions -e -minTime start -maxTime end;
                                        break;
                                case 4:
                                        currentTime -e start;
                                        break;
                            }
                            setFocus parent;
                            return;
                        }
                    }
                }

                floatFieldGrp -e -value1 startEnd[0]
                                -value2 startEnd[1]
                                -enable 0
                                startEnd;
                
                int row = self.getActiveTimeRange();
                gTmpCacheTimeRangeModes[rangeIndex] = mode;
                Alembic_updateCacheTimeRangeRow(row);
            }
            else {
                floatFieldGrp -e -enable 1 startEnd;
                gTmpCacheTimeRangeModes[rangeIndex] = mode;
            }
        }

        #
        #  Procedure Name:
        #      Alembic_exportEnableFrameRelativeSampleChanged
        #
        #  Description:
        #      Callback procedure when the state of Enable frame relative sample checkbox has been changed.
        #
        #  Input Arguments:
        #      parent    - Top level parent layout of the option box or file options UI.
        #                  Required so that UI object names can be successfully resolved.
        #
        #  Return Value:
        #      None.
        #
        def exportEnableFrameRelativeSampleChanged(string parent)
        {
            setParent parent;

            # Enable or disable Low/High fields
            int frameRelativeSampleEnabled = `checkBoxGrp -q -value1 enableFrameRelativeSample`;
            floatFieldGrp -e -enable frameRelativeSampleEnabled lowFrameRelativeSample;
            floatFieldGrp -e -enable frameRelativeSampleEnabled highFrameRelativeSample;
            
            int activeRow = self.getActiveTimeRange();

            if( frameRelativeSampleEnabled:
                string frameSampleVal = ""+`floatFieldGrp -q -value1 lowFrameRelativeSample` + "; 0; " + `floatFieldGrp -q -value1 highFrameRelativeSample`;
                cmds.scriptTable(self.cacheTimeTable, edit=True -cellIndex activeRow 4 -cellValue frameSampleVal cacheTimeTable;
            }
            else {
                cmds.scriptTable(self.cacheTimeTable, edit=True -cellIndex activeRow 4 -cellValue "0" cacheTimeTable;
            }
        }

        #
        #  Procedure Name:
        #      Alembic_exportEnablePreRollChanged
        #
        #  Description:
        #      Callback procedure when the state of Enable Pre Roll checkbox has been changed.
        #
        #  Input Arguments:
        #      parent    - Top level parent layout of the option box or file options UI.
        #                  Required so that UI object names can be successfully resolved.
        #
        #  Return Value:
        #      None.
        #
        def exportEnablePreRollChanged(string parent)
        {
            setParent parent;

            # enable/disable preRollStartFrame
            #
            preRollEnabled = cmds.checkBoxGrp(self.enablePreRoll, query=True, value1=True)
            floatFieldGrp -e -enable preRollEnabled preRollStartFrame;
            floatFieldGrp -e -enable preRollEnabled preRollStep;
            
            if (preRollEnabled:
                # insert row for preRoll in the beginning of scriptTable
                cmds.scriptTable(self.cacheTimeTable, edit=True -insertRow 1 cacheTimeTable;
                Alembic_setupRowForPreRoll(parent);
            }
            else {
                # delete row 1 for preRoll in scriptTable
                cmds.scriptTable(self.cacheTimeTable, edit=True -deleteRow 1 cacheTimeTable;
                # select the first time range again. The default behavior
                # will select the second row, which is not expected.
                cmds.scriptTable(self.cacheTimeTable, edit=True -selectedRows 1 cacheTimeTable;
            }
        }

        #
        #  Procedure Name:
        #      Alembic_frameRelativeSampleChanged
        #
        #  Description:
        #      Callback procedure when the frame sample has been changed.
        #
        #  Input Arguments:
        #      parent    - Top level parent layout of the option box or file options UI.
        #                  Required so that UI object names can be successfully resolved.
        #
        #  Return Value:
        #      None.
        #
        def frameRelativeSampleChanged(string parent)
        {
            global float gTmpSampleLowFrames[];
            global float gTmpSampleHighFrames[];

            setParent parent;
            
            int activeRow = self.getActiveTimeRange();
            int rangeIndex = self.getActiveTimeRangeIndex();
            
            gTmpSampleLowFrames[rangeIndex] = `floatFieldGrp -q -value1 lowFrameRelativeSample`;
            gTmpSampleHighFrames[rangeIndex] = `floatFieldGrp -q -value1 highFrameRelativeSample`;

            string frameRelativeSampleVal = "" + gTmpSampleLowFrames[rangeIndex] + "; 0; " + gTmpSampleHighFrames[rangeIndex];
            cmds.scriptTable(self.cacheTimeTable, edit=True -cellIndex activeRow 4 -cellValue frameRelativeSampleVal cacheTimeTable;
        }

        #
        #  Procedure Name:
        #      Alembic_exportEvaluateEveryChanged
        #
        #  Description:
        #      Callback procedure when step has been changed.
        #
        #  Input Arguments:
        #      parent    - Top level parent layout of the option box or file options UI.
        #                  Required so that UI object names can be successfully resolved.
        #
        #  Return Value:
        #      None.
        #
        def exportEvaluateEveryChanged(string parent)
        {
            setParent parent;
            
            float stepVal = `floatFieldGrp -q -value1 evaluateEvery`;
            int activeRow = self.getActiveTimeRange();

            if (stepVal <= 0:
                warning _L10N(kStepInvalid, "Step cannot be 0 or negative.");
                float oldVal = self.getEvaluateEveryOfRange(activeRow);
                floatFieldGrp -e -value1 oldVal evaluateEvery;
                return;
            }

            cmds.scriptTable(self.cacheTimeTable, edit=True -cellIndex activeRow 3 -cellValue stepVal cacheTimeTable;
        }

        #
        #  Procedure Name:
        #      isUnique
        #
        #  Description:
        #      Determines whether the string array contains the specified string or not.
        #
        #  Input Arguments:
        #      item          - The string item to check in the array.
        #
        #      stringArray   - The array which may contains the string item.
        #
        #  Return Value
        #      True if the stringArray contains the specified item.
        #
        proc int isUnique(string item, string stringArray[])
        {
            int i, result = 1;
            for (i = 0; i < size(stringArray); i++:
                if (item == stringArray[i]:
                    result = 0;
                    break;
                }
            }

            return result;
        }

        #
        #  Procedure Name:
        #      Alembic_exportAttrAddRemove
        #
        #  Description:
        #      Callback procedure when Add/Remove button in Attributes tab is pushed.
        #
        #  Input Arguments:
        #      remove    - 0: Add
        #                  1: Remove
        #
        #      parent    - Top level parent layout of the option box or file options UI.
        #                  Required so that UI object names can be successfully resolved.
        #
        #  Return Value:
        #      None.
        #
        def exportAttrAddRemove(int remove, string parent)
        {
            setParent parent;

            # split the attribute names in the text field
            string attr = `textField -q -text attrField`;
            string attrList[];
            tokenize attr " " attrList;

            string attrArray[] = `textScrollList -q -allItems attrList`;
            int i;
            if (remove:
                # remove the attribute
                for (i = 0; i < size(attrList); i++:
                    if (!isUnique(attrList[i], attrArray):
                        textScrollList -e -removeItem attrList[i] attrList;
                    }
                }
            }
            else {
                # add the attribute
                for (i = 0; i < size(attrList); i++:
                    if (size(attrList[i]) > 0 and isUnique(attrList[i], attrArray):
                        textScrollList -e -append attrList[i] attrList;
                    }
                }
            }
        }

        #
        #  Procedure Name:
        #      Alembic_exportAttrGetChannels
        #
        #  Description:
        #      Callback procedure when From Channel Box button in Attributes tab is pushed.
        #
        #  Input Arguments:
        #      parent    - Top level parent layout of the option box or file options UI.
        #                  Required so that UI object names can be successfully resolved.
        #
        #  Return Value:
        #      None.
        #
        def exportAttrGetChannels(string parent)
        {
            setParent parent;

            if (`channelBox -q -exists mainChannelBox`:
                # current list of attributes
                string attrArray[] = `textScrollList -q -allItems attrList`;

                # the list of new attributes to append
                string newAttrArray[];

                # selected main attributes
                string mainAttrList[]  = `channelBox -q -selectedMainAttributes mainChannelBox`;

                # current selected nodes
                string selection[] = `ls -sl`;

                # add selected attributes from channel box to the attribute list
                string attr, node;
                for (attr in mainAttrList:
                    for (node in selection:
                        if (`attributeExists attr node`:
                            string longName = `attributeName -long (node + "." + attr)`;
                            if (isUnique(longName, attrArray) and isUnique(longName, newAttrArray):
                                newAttrArray[size(newAttrArray)] = longName;
                            }
                            break;
                        }
                    }
                }

                # selected shape attributes
                string shapeAttrList[] = `channelBox -q -selectedShapeAttributes mainChannelBox`;

                # shapes
                string shapes[] = `listRelatives -shapes selection`;

                # add selected shape attributes from channel box to the attribute list
                for (attr in shapeAttrList:
                    for (node in shapes:
                        if (`attributeExists attr node`:
                            string longName = `attributeName -long (node + "." + attr)`;
                            if (isUnique(longName, attrArray) and isUnique(longName, newAttrArray):
                                newAttrArray[size(newAttrArray)] = longName;
                            }
                            break;
                        }
                    }
                }

                # append the attribute list to scroll list
                for (attr in newAttrArray:
                    textScrollList -e -append attr attrList;
                }
            }
        }


        #
        #  Procedure Name:
        #      Alembic_exportAttrRemove
        #
        #  Description:
        #      Callback procedure when Remove Selected/All button in Attributes tab is pushed.
        #
        #  Input Arguments:
        #      removeAll - 0: Remove Selected
        #                  1: Remove All
        #
        #      parent    - Top level parent layout of the option box or file options UI.
        #                  Required so that UI object names can be successfully resolved.
        #
        #  Return Value:
        #      None.
        #
        def exportAttrRemove(int removeAll, string parent)
        {
            setParent parent;

            if (removeAll:
                textScrollList -e -removeAll attrList;
            }
            else {
                # remove the selected items
                string attrList[] = `textScrollList -q -selectItem attrList`;
                string attrItem;
                for (attrItem in attrList:
                    textScrollList -e -removeItem attrItem attrList;
                }
            }
        }

        #
        #  Procedure Name:
        #      Alembic_exportAttrPrefixAddRemove
        #
        #  Description:
        #      Callback procedure when Add/Remove button in Attribute Prefix tab is pushed.
        #
        #  Input Arguments:
        #      remove    - 0: Add
        #                  1: Remove
        #
        #      parent    - Top level parent layout of the option box or file options UI.
        #                  Required so that UI object names can be successfully resolved.
        #
        #  Return Value:
        #      None.
        #
        def exportAttrPrefixAddRemove(int remove, string parent)
        {
            setParent parent;

            # split the attribute prefixes in the text field
            string attrPrefix = `textField -q -text attrPrefixField`;
            string attrPrefixList[];
            tokenize attrPrefix " " attrPrefixList;

            string attrPrefixArray[] = `textScrollList -q -allItems attrPrefixList`;
            int i;
            if (remove:
                # remove the attribute
                for (i = 0; i < size(attrPrefixList); i++:
                    if (!isUnique(attrPrefixList[i], attrPrefixArray):
                        textScrollList -e -removeItem attrPrefixList[i] attrPrefixList;
                    }
                }
            }
            else {
                # add the attribute
                for (i = 0; i < size(attrPrefixList); i++:
                    if (size(attrPrefixList[i]) > 0 and isUnique(attrPrefixList[i], attrPrefixArray):
                        textScrollList -e -append attrPrefixList[i] attrPrefixList;
                    }
                }
            }
        }

        #
        #  Procedure Name:
        #      Alembic_exportAttrPrefixRemove
        #
        #  Description:
        #      Callback procedure when Remove Selected/All button in Attribute Prefix tab is pushed.
        #
        #  Input Arguments:
        #      removeAll - 0: Remove Selected
        #                  1: Remove All
        #
        #      parent    - Top level parent layout of the option box or file options UI.
        #                  Required so that UI object names can be successfully resolved.
        #
        #  Return Value:
        #      None.
        #
        def exportAttrPrefixRemove(int removeAll, string parent)
        {
            setParent parent;

            if (removeAll:
                # remove all items
                textScrollList -e -removeAll attrPrefixList;
            }
            else {
                # remove the selected items
                string attrPrefixList[] = `textScrollList -q -selectItem attrPrefixList`;
                string attrPrefixItem;
                for (attrPrefixItem in attrPrefixList:
                    textScrollList -e -removeItem attrPrefixItem attrPrefixList;
                }
            }
        }

        #
        #  Procedure Name:
        #      exportOptions
        #
        #  Description:
        #      Construct the option box UI. Involves accessing the standard option
        #      box and customizing the UI accordingly.
        #
        #  Input Arguments:
        #      exportAll - Whether to export the entire scene or only selected objects.
        #
        #  Return Value:
        #      None.
        #
        proc exportOptions(int exportAll)
        {
            #  Name of the command for this option box.
            #
            string commandName;
            string label;
            if (exportAll:
                commandName = "AlembicExportAll";
                label       = _L10N(kExportAll,"Export All");
            }
            else {
                commandName = "AlembicExportSelection";
                label       = _L10N(kExportSelection,"Export Selection");
            }

            #  Build the option box actions.
            #
            string callback = "Alembic_exportCallback";
            string setup    = "Alembic_exportSetup";

            #  STEP 1:  Get the option box.
            #  ============================
            #
            #  The value returned is the name of the layout to be used as
            #  the parent for the option box UI.
            #
            string layout = getOptionBox();

            #  STEP 2:  Pass the command name to the option box.
            #  =================================================
            #
            #  Any default option box behaviour based on the command name is set
            #  up with this call. For example, updating the 'Help' menu item with
            #  the name of the command.
            #
            setOptionBoxCommandName(commandName);

            #  STEP 3:  Create option box contents.
            #  ====================================
            #
            string parent = exportOptionsUI(layout);

            #  STEP 4:  Customize the buttons.
            #  ===============================
            #
            #  Provide more descriptive labels for the buttons.
            #  Disable those buttons that are not applicable to the option box.
            #  Attach actions to those buttons that are applicable to the option box.

            # 'Export' button
            #
            string exportBtn = getOptionBoxApplyBtn();
            button -e -label label
                -command (callback + " " + parent + " " + exportAll + " 1")
                exportBtn;

            # 'Save' button
            #
            string saveBtn = getOptionBoxSaveBtn();
            button -e
                -command (callback + " " + parent + " " + exportAll + " 0; hideOptionBox")
                saveBtn;

            # 'Reset' button
            #
            string resetBtn = getOptionBoxResetBtn();
            button -e
                -command (setup + " " + parent + " 1")
                resetBtn;

            #  STEP 5:  Set the option box title.
            #  ==================================
            #
            setOptionBoxTitle(_L10N(kAlembicExport,"Alembic Export"));

            #  STEP 6:  Customize the 'Help' menu item text.
            #  =============================================
            #
            setOptionBoxHelpTag(commandName);

            #  Set the current values of the option box.
            #  =========================================
            #
            eval (setup + " " + parent + " 0");

            #  Show the option box.
            #  ====================
            #
            showOptionBox();
        }

        #
        #  Procedure Name:
        #      captureAlembicExportOptionVars
        #
        #  Description:
        #      Capture the current state of option values to an argument list.
        #
        #  Input Arguments:
        #      exportAll - Whether to export the entire scene or only selected objects.
        #
        #  Return Value:
        #      A list of arguments to invoke AbcExport.
        #
        def string[] captureAlembicExportOptionVars(int version, int exportAll)
        {
            setOptionVars(0);
            int    cacheTimeRanges[]         = `optionVar -q Alembic_exportCacheTimeRanges`;
            float  startFrames[]             = `optionVar -q Alembic_exportStarts`;
            float  endFrames[]               = `optionVar -q Alembic_exportEnds`;
            float  evaluateEvery[]           = `optionVar -q Alembic_exportEvaluateEverys`;
            int    enableSample[]            = `optionVar -q Alembic_exportEnableFrameRelativeSamples`;
            float  lowFrameRelativeSamples[] = `optionVar -q Alembic_exportLowFrameRelativeSamples`;
            float  highFrameRelativeSamples[]= `optionVar -q Alembic_exportHighFrameRelativeSamples`;
            int    enablePreRoll             = `optionVar -q Alembic_exportEnablePreRoll`;
            float  preRollStartFrame         = `optionVar -q Alembic_exportPreRollStartFrame`;
            string attr                      = `optionVar -q Alembic_exportAttr`;
            string attrPrefix                = `optionVar -q Alembic_exportAttrPrefix`;
            int    verbose                   = `optionVar -q Alembic_exportVerbose`;
            int    noNormals                 = `optionVar -q Alembic_exportNoNormals`;
            int    renderableOnly            = `optionVar -q Alembic_exportRenderableOnly`;
            int    stripNamespaces           = `optionVar -q Alembic_exportStripNamespaces`;
            int    uvWrite                   = `optionVar -q Alembic_exportUVWrite`;
            int    wholeFrameGeo             = `optionVar -q Alembic_exportWholeFrameGeo`;
            int    worldSpace                = `optionVar -q Alembic_exportWorldSpace`;
            int    writeVisibility           = `optionVar -q Alembic_exportWriteVisibility`;
            string perFrameCallbackMel       = `optionVar -q Alembic_exportPerFrameCallbackMel`;
            string postJobCallbackMel        = `optionVar -q Alembic_exportPostJobCallbackMel`;
            string perFrameCallbackPython    = `optionVar -q Alembic_exportPerFrameCallbackPython`;
            string postJobCallbackPython     = `optionVar -q Alembic_exportPostJobCallbackPython`;
            
            string cacheTimeRangeStr;
            string startFrameStr;
            string endFrameStr;
            string evaluateEveryStr;
            string enableSampleStr;
            string lowSampleStr;
            string highSampleStr;
            int i;
            for (i = 0; i < size(cacheTimeRanges); i++:
                if (i != 0:
                    cacheTimeRangeStr += ":";
                    startFrameStr += ":";
                    endFrameStr += ":";
                    evaluateEveryStr += ":";
                    enableSampleStr += ":";
                    lowSampleStr += ":";
                    highSampleStr += ":";
                }
                cacheTimeRangeStr += cacheTimeRanges[i];
                startFrameStr += startFrames[i];
                endFrameStr += endFrames[i];
                evaluateEveryStr += evaluateEvery[i];
                enableSampleStr += enableSample[i];
                lowSampleStr += lowFrameRelativeSamples[i];
                highSampleStr += highFrameRelativeSamples[i];
            }

            string args[] = {
                "" + exportAll,
                cacheTimeRangeStr,
                startFrameStr,
                endFrameStr,
                evaluateEveryStr,
                enableSampleStr,
                lowSampleStr,
                highSampleStr,
                enablePreRoll,
                preRollStartFrame,
                attr,
                attrPrefix,
                verbose,
                noNormals,
                renderableOnly,
                stripNamespaces,
                uvWrite,
                wholeFrameGeo,
                worldSpace,
                writeVisibility,
                perFrameCallbackMel,
                postJobCallbackMel,
                perFrameCallbackPython,
                postJobCallbackPython
            };

            if (version >= 2:
                int    filterEulerRotations    = `optionVar -q Alembic_exportFilterEulerRotations`;

                string argsVer2[] = {
                    "" + filterEulerRotations
                };

                appendStringArray(args, argsVer2, 1);
            }

            if (version >= 3:
                int    writeColorSets      = `optionVar -q Alembic_exportWriteColorSets`;
                int    writeFaceSets       = `optionVar -q Alembic_exportWriteFaceSets`;

                string argsVer3[] = {
                    "" + writeColorSets,
                    writeFaceSets
                };

                appendStringArray(args, argsVer3, 2);
            }

            if (version >= 4:
                int dataFormat = `optionVar -q Alembic_exportDataFormat`;

                string argsVer4[] = {
                    "" + dataFormat
                };

                appendStringArray(args, argsVer4, 1);
            }

            if (version >= 5:
                float preRollStep = `optionVar -q Alembic_exportPreRollStep`;

                string argsVer5[] = {
                    "" + preRollStep
                };

                appendStringArray(args, argsVer5, 1);
            }
                
            if (version >= 6:
                int    writeCreases = `optionVar -q Alembic_exportWriteCreases`;
                string argsVer6[] = {
                    "" + writeCreases
                };

                appendStringArray(args, argsVer6, 1);
            }

            if (version >= 7:
                int    writeUVSets = `optionVar -q Alembic_exportWriteUVSets`;
                string argsVer7[] = {
                    "" + writeUVSets
                };

                appendStringArray(args, argsVer7, 1);
            }
            return args;
        }

        #
        #  Procedure Name:
        #      assembleCmd
        #
        #  Description:
        #      Construct the command that will apply the option box values.
        #
        #  Input Arguments:
        #      exportAll - Whether to export the entire scene or only selected objects.
        #
        proc string assembleCmd(int exportAll)
        {
            int version = 7;
            string args[] = captureAlembicExportOptionVars(version, exportAll);
            string cmd = "doAlembicExportArgList " + version + " {";
            int i;
            for (i = 0; i < size(args); i++:
                if (i > 0) cmd += ",";
                cmd += ("\"" + encodeString(args[i]) + "\"");
            }
            cmd += "};";
            return cmd;
        }

        #
        #  Procedure Name:
        #      Alembic_exportFileOptionsUICreate
        #
        #  Description:
        #      Construct the file options UI.
        #
        #  Input Arguments:
        #      parent    - Top level parent layout of the file options UI.
        #                  Required so that UI object names can be successfully resolved.
        #
        #  Return Value:
        #      None.
        #      
        def exportFileOptionsUICreate(string parent)
        {
            exportOptionsUI(parent);
        }

        #
        #  Procedure Name:
        #      Alembic_exportFileOptionsUIInit
        #
        #  Description:
        #      Update the state of the file options UI to reflect the option values.
        #
        #  Input Arguments:
        #      parent    - Top level parent layout of the file options UI.
        #                  Required so that UI object names can be successfully resolved.
        #
        #      filter    - The file type filter.
        #
        #  Return Value:
        #      None.
        #
        def exportFileOptionsUIInit(string parent, string filter)
        {
            Alembic_exportSetup parent 0;
        }

        #
        #  Procedure Name:
        #      Alembic_exportAllFileOptionsUICommit
        #
        #  Description:
        #      Update the option values with the current state of the file options UI.
        #
        #  Input Arguments:
        #      parent    - Top level parent layout of the file options UI.
        #                  Required so that UI object names can be successfully resolved.
        #
        #  Return Value:
        #      None.
        #
        def exportAllFileOptionsUICommit(string parent)
        {
            Alembic_exportCallback parent 1 0;
        }


        #
        #  Procedure Name:
        #      Alembic_exportSelectionFileOptionsUICommit
        #
        #  Description:
        #      Update the option values with the current state of the file options UI.
        #
        #  Input Arguments:
        #      parent    - Top level parent layout of the file options UI.
        #                  Required so that UI object names can be successfully resolved.
        #
        #  Return Value:
        #      None.
        #
        def exportSelectionFileOptionsUICommit(parent):
            Alembic_exportCallback(parent, 0, 0)


def containsWhiteSpace(string):
    if len(re.match(r'\s', string)) > 0:
        return True
    else:
        return False


(ABC_ACTION_EXEC,
 ABC_ACTION_OPTION,
 ABC_ACTION_CMD) = range(3)


#
#  Procedure Name:
#      performAlembicExport
#
#  Description:
#      Perform the AlembicExportAll or AlembicExportSelection command using the
#      corresponding option values. This procedure will also show the option box
#      window if necessary as well as construct the command string
#      that will invoke the sphere command with the current
#      option box values.
#
#  Input Arguments:
#      0 - Execute the command.
#      1 - Show the option box dialog.
#      2 - Return the command.
#
#      exportAll - Whether to export the entire scene or only selected objects.
#
#  Return Value:
#      None.
#
def performAlembicExport(action=ABC_ACTION_EXEC, exportAll=True):

    cmd = ""

    #  Execute the command.
    #
    if action == ABC_ACTION_EXEC:
        #  Get the command.
        #
        cmd = __assembleCmd(exportAll)

        # Execute the command with the option settings.
        #
        eval(cmd)

    # Show the option box.
    #
    elif action == ABC_ACTION_OPTION:
        # Show the option box.
        #
        __exportOptions(exportAll)

    # Return the command string.
    #
    elif action == ABC_ACTION_CMD:
        # Get the command.
        #
        cmd = __assembleCmd(exportAll)

    return cmd;




from mtoa.callbacks import *
import mtoa.aovs as aovs
import mtoa.utils as utils
import mtoa.ui.ae.shaderTemplate as shaderTemplate
import mtoa.ui.ae.templates as templates
import mtoa.core as core
import mtoa.callbacks as callbacks
import mtoa.hooks as hooks
import maya.mel as mel

from collections import defaultdict
import sys

import maya.OpenMaya as om

UI_NAME = 'ArnoldAOVUI'
AOV_ATTRS = ('name', 'type', 'prefix')
WIDTH = 402

AOV_ROW_WIDTHS = [18, 110, 74]
OUTPUT_ROW_WIDTHS = [60, 90]
    
def _uiName(tag):
    return '%s_%s' % (UI_NAME, tag)

global _updating
_updating = False

global _tabEditor
_tabEditor = None

AOV_CALLBACK_ATTRS = ('type', 'defaultValue')

class AOVBrowser(object):
    '''
    A UI for browsing node types and their registered AOVs
    '''
    def __init__(self, renderOptions=None, nodeTypes=None):
        '''
        renderOptions : an aovs.AOVInterface instance, or None to use the default
        
        nodeTypes : a list of node types to display in the available nodes column, 
            or None to display the complete list of nodes with AOVs
        '''

        self.allAOVs = set([])
        self.renderOptions = aovs.AOVInterface() if renderOptions is None else renderOptions
        self.allNodeTypes = set(aovs.getNodeTypesWithAOVs())
        
        if nodeTypes:
            self.setNodeTypes(nodeTypes)
        else:
            self.nodeTypes = sorted(self.allNodeTypes)

        
        self.form = cmds.formLayout()

        availableLbl = cmds.text(_uiName('availableLbl'), align='center', label='Available AOVs')
        activeLbl = cmds.text(_uiName('activeLbl'), align='center', label='Active AOVs')
        detailsLbl = cmds.text(_uiName('detailsLbl'), align='center', label='')
        detailsFieldLbl = cmds.text(_uiName('detailsFieldLbl'), align='center', label='')

        self.availableLst = cmds.textScrollList(_uiName('availableLst'), numberOfRows=10, allowMultiSelection=True,
                          doubleClickCommand=self.addAOVs)
        self.activeLst = cmds.textScrollList(_uiName('activeLst'), numberOfRows=10, allowMultiSelection=True,
                          selectCommand=self.selectAOV)

        addBtn = cmds.button(_uiName('addBtn'), label='>>', command=self.addAOVs)
        addCustomBtn = cmds.button(_uiName('addCustomBtn'), label='Add Custom', command=self.addCustomAOV)
        remBtn = cmds.button(_uiName('remBtn'), label='<<', command=self.removeAOVs)

        cmds.formLayout(self.form, edit=True, attachForm=[
                                    (availableLbl, 'top', 1),
                                    (activeLbl, 'top', 1),
                                    (detailsLbl, 'top', 1),
                                    
                                    (activeLbl, 'right', 1),
                                    (self.activeLst, 'right', 1),
                                    (detailsFieldLbl, 'right', 1),

                                    (addCustomBtn, 'right', 1),
                                    (remBtn, 'right', 1),
                                    (addBtn, 'bottom', 1),
                                    (addCustomBtn, 'bottom', 1),
                                    (remBtn, 'bottom', 1),

                               ])
        cmds.formLayout(self.form, edit=True, attachControl=[
                                    (self.activeLst, 'top', 1, activeLbl),
                                    (self.availableLst, 'top', 1, availableLbl),
                                    (detailsFieldLbl, 'top', 1, detailsLbl),
                                    
                                    (self.activeLst, 'bottom', 1, addBtn),
                                    (self.availableLst, 'bottom', 1, addCustomBtn),
                                    
                                    (addBtn, 'right', 1, addCustomBtn),
                                    (addCustomBtn, 'right', 1, remBtn),
                                ])

        cmds.formLayout(self.form, edit=True, attachPosition=[
                                    (availableLbl, 'left', 1, 1),
                                    (availableLbl, 'right', 1, 50),
                                    (activeLbl, 'left', 1, 50),
                                    (activeLbl, 'right', 1, 100),
                                    (detailsLbl, 'left', 1, 100),

                                    
                                    (self.availableLst, 'left', 1, 1),
                                    (self.availableLst, 'right', 1, 50),
                                    (self.activeLst, 'left', 1, 50),
                                    (self.activeLst, 'right', 1, 100),
                                    (detailsFieldLbl, 'left', 1, 100),
                                    
                                    (addBtn, 'left', 1, 1),
                                    (addBtn, 'right', 1, 50),
                                    (addCustomBtn, 'left', 1, 50),
                                    (addCustomBtn, 'right', 1, 75),
                                    (remBtn, 'left', 1, 75),
                                    (remBtn, 'right', 1, 100),                                    
                                    

                                ])
 


    def setNodeTypes(self, nodeTypes):
        '''set the node types listed in the AOV Groups column'''
        self.nodeTypes = sorted(self.allNodeTypes.intersection(nodeTypes))
 
    def populate(self):
        # populate available and active based on aovs provided by groups and nodes
        self.updateActiveAOVs()

    def addAOVs(self, *args):
        '''
        create the selected AOVs, and connect the new AOV nodes to their corresponding
        AOV attributes for any nodes in the scene.
        '''
        sel = cmds.textScrollList(self.availableLst, query=True, selectItem=True)
        aovShaderName = None

        # clear the current selection
        cmds.textScrollList(self.activeLst, edit=True,deselectAll=True)

        if sel:
            global _updating
            _updating = True
            try:
                for aovFullName in sel:
                    aovList = aovFullName.split()
                    aovName = aovList[0] # splits on whitespace
                    aovShaderName = None
                    if len(aovList) > 1:
                        shaderName = aovList[1]
                        if shaderName[:1] == "(" and shaderName[-1:] == ")":
                            shaderName = shaderName[1:-1]
                            aovShadersList = aovs.getAOVShaders()
                            if shaderName in aovShadersList:
                                aovShaderName = shaderName

                    aov = self.renderOptions.addAOV(aovName, aovShader=aovShaderName)
            finally:
                _updating = False
            self.updateActiveAOVs()

            cmds.textScrollList(self.activeLst, edit=True,selectItem=sel[0].split()[0])

            if not aovShaderName:
                self.selectAOV()

    def removeAOVs(self, *args):
        '''
        delete the selected AOVs
        '''
        sel = cmds.textScrollList(self.activeLst, query=True, selectItem=True)
        if sel:
            global _updating
            _updating = True
            try:
                self.renderOptions.removeAOVs(sel)
                for aov in sel:

                    # need to check if the AOV we're removing is part of the allAOVs list
                    # but since this list contain the eventual shader owner , eg (aiShadowMatte)
                    # we need to take this into account
                    found = False
                    for allAOVs in self.allAOVs:
                        aovName = allAOVs.split()[0]
                        if aovName == aov:
                            found = True
                            break

                    if found:
                        cmds.textScrollList(self.availableLst, edit=True, append=aov)
                    cmds.textScrollList(self.activeLst, edit=True, removeItem=aov)
            finally:
                _updating = False
            self.updateActiveAOVs()

    def selectAOV(self, *args):
        sel = cmds.textScrollList(self.activeLst, query=True, selectItem=True)
        cmds.select(clear=True)
        if sel:
            for aov in sel:
                for aovName, aovList in self.renderOptions.getAOVs(group=True):
                    if aovName == aov and aovList and len(aovList):
                        cmds.select(aovList[0].node, add=True)
                        break
                    


    def addCustomAOV(self, *args):
        global _updating
        
        aovName = ""
        _updating = True
        try:
            aovName, aovNode = shaderTemplate.newAOVPrompt()    
        finally:
            _updating = False
        
        self.updateActiveAOVs()
        # clear the current selection
        cmds.textScrollList(self.activeLst, edit=True,deselectAll=True)

        cmds.textScrollList(self.activeLst, edit=True,selectItem=aovName)
        self.selectAOV()


    def updateActiveAOVs(self):
        '''
        fill the active and inactive columns based on the nodeType/group selected
        '''
        if _updating:
            return

        # first, find out what's selected, so we can reselect any persistent items
        availableSel = cmds.textScrollList(self.availableLst, query=True, selectItem=True) or []
        activeSel = cmds.textScrollList(self.activeLst, query=True, selectItem=True) or []
        availableList = []
        activeList = []

        # update the available list
        cmds.textScrollList(self.availableLst, edit=True, removeAll=True)
        cmds.textScrollList(self.activeLst, edit=True, removeAll=True)
        # try:
        activeAOVs = self.renderOptions.getAOVs()
        # except:
            # activeAOVs = []
        self.allAOVs = set([])

        aovList = aovs.getBuiltinAOVs()
        aovList.sort()
        for group in self.nodeTypes:
            for x in aovs.getRegisteredAOVs(nodeType=group) :
                if x:
                    aovLabel = x + ' ('
                    aovLabel +=group
                    aovLabel += ')'
                    aovList.append(aovLabel)

        self.allAOVs.update(aovList)

        for aovFullName in aovList:
            aovName = aovFullName.split()[0]
            if aovName not in activeAOVs:
                if aovName not in availableList:
                    availableList.append(aovFullName)

        for activeAOV in activeAOVs:
            activeList.append(activeAOV.name)

        for aovName in availableList:
            cmds.textScrollList(self.availableLst, edit=True, append=aovName)
            if aovName in availableSel:
                cmds.textScrollList(self.availableLst, edit=True, selectItem=aovName)
        # update sorted and not duplicated active AOVs
        activeList.sort()
        for aovName in activeList:
            cmds.textScrollList(self.activeLst, edit=True, append=aovName)
            if aovName in activeSel:
                cmds.textScrollList(self.activeLst, edit=True, selectItem=aovName)

class AOVItem(object):
    '''
    Builds the UI for a single AOV control, which may contain multiple outputs (Driver + Filter combo)
    '''

    def __init__(self, parent, aovObject, lockName=False):
        self.outputsChanged = True
        self.parent = parent
        self.aov = aovObject
        DARK_BLUE = [.16, .17, .2]
        aovNode = self.aov.node

        self.baseWidget = cmds.columnLayout(adj=True)

        cmds.rowLayout(nc=3,
                     rowAttach=([1, 'top', 2],
                                [3, 'top', 2]),
                     columnWidth3=[sum(AOV_ROW_WIDTHS)+8, sum(OUTPUT_ROW_WIDTHS)+8, 20],
                     columnAttach3=['right', 'both', 'both'])

        # AOV UI --------
        cmds.rowLayout(nc=3,
                     columnWidth3=AOV_ROW_WIDTHS,
                     columnAttach3=['right', 'both', 'both'])

        self.enabledCtrl = cmds.checkBox(label='')
        cmds.connectControl(self.enabledCtrl, '{}.enabled'.format(aovNode))

        nameAttr = '{}.name'.format(aovNode)
        # use evalDeferred to ensure that the update happens after the aov node attribute is set
        self.nameCtrl = cmds.textField(editable=not lockName,
                                # we save out the current aov name for the replaceShadingGroupDummyAttrs callback
                                changeCommand=lambda new, old=cmds.getAttr(nameAttr): self.aov.rename(new, old)
                                )
        cmds.connectControl(self.nameCtrl, nameAttr)
        # must set editability after connecting control
        cmds.textField(self.nameCtrl, edit=True, editable=not lockName)
        if cmds.referenceQuery(aovNode, isNodeReferenced=True):
            # orange
            cmds.textField(self.nameCtrl, edit=True, backgroundColor=DARK_BLUE)

        #cmds.text(label='name')
        # attrEnumOptionMenu does not work with multi-attrs and optionMenu does not work with connectControl,
        # so, unfortunately, our best option is attrEnumOptionMenuGrp
        self.channelsMenu = cmds.attrEnumOptionMenuGrp(attribute='{}.type'.format(aovNode), columnWidth2=[1, 50])

        cmds.setParent('..')

        cmds.frameLayout(labelVisible=False)
        self.outputColumn = cmds.columnLayout(adj=True, rowSpacing=2)

        # cache the list of outputs
        self.outputs = []
        outputsAttrStr = '{}.outputs'.format(aovNode)
        outputsAttrVal = cmds.getAttr(outputsAttrStr, mi=True) or []
        for i in outputsAttrVal:
            outputAttr = '{}[{}]'.format(outputsAttrStr, i)
            try:
                outputRow = AOVOutputItem(self.outputColumn, outputAttr, self)
            except IndexError as e:
                continue
            else:
                self.outputs.append(outputRow)
                cmds.setParent('..')

        cmds.setParent('..')
        cmds.setParent('..')


        aovMenuButton = cmds.symbolButton(image="arrowDown.png")

        self.popupMenu = cmds.popupMenu(parent=aovMenuButton, button=1, postMenuCommand=self.buildPopupMenu)

        cmds.setParent('..')
        cmds.setParent('..')

    def aovName(self):
        return cmds.getAttr('{}.name'.format(self.aov.node))

    def getMenus(self):
        '''
        Get a list of all of the menus in this AOVItem
        '''
        # flatten the menus
        return [row.filterMenu for row in self.outputs]

    def fixOptionMenus(self):
        for menu in self.getMenus():
            try:
                cmds.optionMenu(menu, edit=True, visible=False)
                cmds.optionMenu(menu, edit=True, visible=True)
            except:
                pass

    def delete(self):
        '''
        Delete the control and all of its children
        '''
        cmds.deleteUI(self.baseWidget)

    def addOutput(self):
        '''
        Connect the defaultArnoldDriver and defaultArnoldFilter to the next available
        output on the aiAOV node for this AOVItem, then build a sub-UI for it
        '''
        # all new output starts with the default driver/filter nodes
        driverNode = 'defaultArnoldDriver'
        filterNode = 'defaultArnoldFilter'
        outputAttr = '{}.outputs'.format(self.aov)
        outputAttr = '{}[{}]'.format(outputAttr, cmds.getAttr(outputAttr, size=True))

        cmds.connectAttr('{}.message'.format(driverNode), '{}.driver'.fornat(outputAttr))
        cmds.connectAttr('{}.message'.format(filterNode), '{}.filter'.fornat(outputAttr))
        outputRow = AOVOutputItem(self.outputColumn, outputAttr, self)
        self.outputs.append(outputRow)
        self.outputsChanged = True

    def removeOutput(self, index):
        '''
        Disconnect the driver and filter nodes at the given index from the aiAOV node for this AOVItem.
        Delete the driver and filter nodes if they are no longer used by any AOVs. Delete the UI
        for this output.
        '''
        outputRow = self.outputs.pop(index)
        outputRow.delete()
        self.outputsChanged = True
        
    def buildPopupMenu(self, menu, parent):
        if self.outputsChanged:
            cmds.popupMenu(self.popupMenu, edit=True, deleteAllItems=True)
            cmds.menuItem(parent=menu, label='Select AOV Node', c=lambda *args: cmds.select(self.aov.node))
            cmds.menuItem(parent=menu, label='Add New Output Driver', c=lambda *args: self.addOutput())
            cmds.menuItem(parent=menu, label='Remove AOV', c=lambda *args: self.parent.removeAOV(self.aov))

            cmds.menuItem(parent=menu, divider=True)
            if len(self.outputs) > 1:
                for i, outputRow in enumerate(self.outputs):
                    subMenu = cmds.menuItem(parent=menu, label='Output Driver %d' % (i+1), subMenu=True)
                    cmds.menuItem(parent=subMenu, label='Select Driver',
                                     c=lambda *args: cmds.select(outputRow.driverNode))
                    cmds.menuItem(parent=subMenu, label='Select Filter',
                                     c=lambda *args: cmds.select(outputRow.filterNode))
                    cmds.menuItem(parent=subMenu, divider=True)
                    cmds.menuItem(parent=subMenu, label='Remove',
                                     c=lambda *args: self.removeOutput(i))
            elif len(self.outputs) == 1:
                outputRow = self.outputs[0]
                cmds.menuItem(parent=menu, label='Select Driver',
                                 c=lambda *args: cmds.select(outputRow.driverNode))
                cmds.menuItem(parent=menu, label='Select Filter',
                                 c=lambda *args: cmds.select(outputRow.filterNode))
            self.outputsChanged = False
        return menu

class AOVOutputItem(object):
    '''
    Builds the UI for an output row belonging to an AOVItem
    '''
    def __init__(self, parent, outputAttr, aovItem):

        self.parent = parent
        self.aovItem = aovItem # required to set .outputsChanged
        self.outputAttr = outputAttr
        self.row = None
        self.driverMenu = None
        self.filterMenu = None
        driver_inputs = cmds.listConnections('{}.driver'.format(outputAttr), source=True, destination=False) or []
        filter_inputs = cmds.listConnections('{}.filter'.format(outputAttr), source=True, destination=False) or []
        if len(driver_inputs):
            self.driverNode = driver_inputs[0]
        else:
            self.driverNode = None

        if len(filter_inputs):
            self.filterNode = filter_inputs[0]
        else:
            self.filterNode = None

        self.buildOutputRow()

    def buildOutputRow(self):
        '''
        Add a new Driver and Filter row within the AOVItem row
        '''

        cmds.setParent(self.parent)

        # DRIVER UI ----------
        self.row = cmds.rowLayout(nc=2,
                     columnWidth2=OUTPUT_ROW_WIDTHS,
                     columnAttach2=['both', 'both'])

        self.driverMenu = cmds.optionMenu(label='', w=50,
                                             changeCommand=lambda newDriver, at='{}.driver'.format(self.outputAttr): \
                                             self.driverMenuChanged(at, newDriver))

        defaultDriver = '<%s>' % cmds.getAttr('defaultArnoldDriver.aiTranslator')
        cmds.menuItem(label=defaultDriver)
        for tran in templates.getTranslators('aiAOVDriver'):
            cmds.menuItem(label=tran)
        if self.driverNode == 'defaultArnoldDriver':
            driver = defaultDriver
            isDefaultDriver=True
        else:
            driver = cmds.getAttr('{}.aiTranslator'.format(self.driverNode))
            isDefaultDriver=False
        try:
            cmds.optionMenu(self.driverMenu, e=True, value=driver)
            #driverMenu.setValue(driver)
        except RuntimeError:
            cmds.warning("[mtoa] %s: Unknown driver %r" % (self.driverNode, driver))
        else:
            if not isDefaultDriver:
                drivTransAttr = '{}.aiTranslator'.format(self.driverNode)
                self.driverJobId = cmds.scriptJob(attributeChange=[drivTransAttr,
                                              lambda: self.translatorChanged(drivTransAttr, self.driverMenu)],
                             parent=self.driverMenu)
            # rebuild the menu when the default driver changes
            cmds.scriptJob(attributeChange=['defaultArnoldDriver.aiTranslator',
                                          lambda: self.defaultTranslatorChanged('defaultArnoldDriver', self.driverMenu, 'aiAOVDriver')],
                                          parent=self.parent)
            
        # FILTER UI ----------
        self.filterMenu = cmds.optionMenu(label='', w=60,
                                             changeCommand=lambda newFilter, at='{}.filter'.format(self.outputAttr): \
                                             self.filterMenuChanged(at, newFilter))
        
        defaultFilter = '<%s>' % cmds.getAttr('defaultArnoldFilter.aiTranslator')
        cmds.menuItem(label=defaultFilter)
        for tran in templates.getTranslators('aiAOVFilter'):
            cmds.menuItem(label=tran)
        if self.filterNode == 'defaultArnoldFilter':
            filter = defaultFilter
            isDefaultFilter=True
        else:
            filter = cmds.getAttr('{}.aiTranslator'.format(self.filterNode))
            isDefaultFilter=False
        try:
            cmds.optionMenu(self.filterMenu, e=True, value=filter)
            #filterMenu.setValue(filter)
        except RuntimeError:
            cmds.warning("[mtoa] %s: Unknown filter %r" % (self.filterNode, filter))
        else:
            if not isDefaultFilter:
                filtTransAttr = '{}.aiTranslator'.format(self.filterNode)
                self.filterJobId = cmds.scriptJob(attributeChange=[filtTransAttr,
                                              lambda: self.translatorChanged(filtTransAttr, self.filterMenu, isDefaultFilter, 'aiAOVFilter')],
                             parent=self.filterMenu)
            # rebuild the menu when the default filter changes
            cmds.scriptJob(attributeChange=['defaultArnoldFilter.aiTranslator',
                                          lambda: self.defaultTranslatorChanged('defaultArnoldFilter', self.filterMenu, 'aiAOVFilter')],
                                          parent=self.parent)

        callbacks.DelayedIdleCallbackQueue(self.fixOptionMenus)

    def delete(self):
        driver_outputs = cmds.listConnections('{}.message'.format(self.driverNode), source=False, destination=True) or []
        if len(driver_outputs) > 1:
            driver_connections = cmds.listConnections('{}.driver'.format(self.outputAttr),
                                                      source=False, destination=True,
                                                      connections=True,
                                                      plugs=True) or []
            for src, dest in zip(driver_connections[::2], driver_connections[1::2]):
                cmds.disconnectAttr(src, dest)
        else:
            utils.safeDelete(self.driverNode)

        filter_outputs = cmds.listConnections('{}.message'.format(self.filterNode), source=False, destination=True) or []
        if self.filterNode.message.outputs() > 1:
            filter_connections = cmds.listConnections('{}.filter'.format(self.outputAttr),
                                                      source=False, destination=True,
                                                      connections=True,
                                                      plugs=True) or []
            for src, dest in zip(filter_connections[::2], filter_connections[1::2]):
                cmds.disconnectAttr(src, dest)
        else:
            utils.safeDelete(self.filterNode)
        self.outputAttr.remove()
        cmds.deleteUI(self.row)

    def translatorChanged(self, translatorAttr, menu):
        '''
        called when the aiTranslator attribute of a driver/filter node changes
        so that we can update the corresponding menu
        '''
        value = cmds.getAttr(translatorAttr)
        cmds.optionMenu(menu, e=True, value=value)

    def defaultTranslatorChanged(self, defaultNode, menu, outputType):
        '''
        rebuilds the menu, updating the value for the default driver/filter and
        restoring the selected item to the proper value
        '''
        # clear menu
        value = cmds.optionMenu(menu, query=True, value=True)
        for item in cmds.optionMenu(menu, query=True, itemListLong=True) or []:
            cmds.deleteUI(item)
        default = '<%s>' % cmds.getAttr(defaultNode + '.aiTranslator')
        cmds.menuItem(parent=menu, label=default)
        for tran in templates.getTranslators(outputType):
            cmds.menuItem(parent=menu, label=tran)
        callbacks.DelayedIdleCallbackQueue(self.fixOptionMenus)
        if value.startswith('<'):
            value = default
        cmds.optionMenu(menu, e=True, value=value)

    def dummy(self, *args):
        pass

    def driverMenuChanged(self, aovOutputAttr, newValue):
        self.outputChangedCallback(aovOutputAttr, newValue, 'aiAOVDriver', 'defaultArnoldDriver')

    def filterMenuChanged(self, aovOutputAttr, newValue):
        self.outputChangedCallback(aovOutputAttr, newValue, 'aiAOVFilter', 'defaultArnoldFilter')

    def outputChangedCallback(self, aovOutputAttr, newValue, outputType, defaultNode):
        """
        change callback for both filter and driver menus
        
        outputType: either 'aiAOVDriver' or 'aiAOVFilter'
        """
        conn = cmds.listConnections(aovOutputAttr, source=True, destination=False) or []
        if newValue.startswith('<'):
            isDefault=True
            cmds.connectAttr(defaultNode + '.message', aovOutputAttr, force=True)
            cmds.select(defaultNode)
            if conn and not cmds.listConnections(conn[0], source=False, destination=True):
                utils.safeDelete(conn[0])
        else:
            isDefault=False
            if conn and cmds.listConnections(conn[0], source=False, destination=True):
                # other AOVs are dependent on existing filter/driver. create and connect a new one
                outputNode = cmds.createNode(outputType)
                cmds.connectAttr('{}.message'.format(outputNode), aovOutputAttr, force=True)
            else:
                outputNode = conn[0]
            newValue = newValue.strip('<>')
            cmds.setAttr('{}.aiTranslator'.format(outputNode), newValue, type="string")

        if outputType == 'aiAOVFilter':
            self.filterNode = outputNode
            hooks.setupFilter(outputNode, self.aovItem.aovName())
            menu = self.filterMenu
        else:
            self.driverNode = outputNode
            hooks.setupDriver(outputNode, self.aovItem.aovName())
            menu = self.driverMenu

        transAttr = '{}.aiTranslator'.format(outputNode)
        if not isDefault:
            # change the selected menu item when the translator attribute changes for our driver/filter
            cmds.scriptJob(attributeChange=[transAttr, lambda: self.translatorChanged(transAttr, menu)],
                         replacePrevious=True,
                         parent=menu)
        else:
            # delete pre-existing scriptJob
            cmds.scriptJob(attributeChange=[transAttr, lambda: self.dummy()],
                         replacePrevious=True,
                         parent=menu)

        self.aovItem.outputsChanged = True

    def fixOptionMenus(self):
        try:
            cmds.optionMenu(self.filterMenu, edit=True, visible=False)
            cmds.optionMenu(self.filterMenu, edit=True, visible=True)
        except:
            pass

class ArnoldAOVEditor(object):

    def __init__(self, aovNode=None):
        self.waitingToRefresh = False
        self.aovControls = []
        self.optionMenus = []
        self.aovShaders = []
        self.aovRows = {}
        self.renderOptions = aovs.AOVInterface() if aovNode is None else aovNode

        self.mainCol = cmds.columnLayout('arnoldAOVMainColumn')

        cmds.setParent(self.mainCol)

        cmds.frameLayout('arnoldAOVBrowserFrame', label='AOV Browser', width=WIDTH,
                            collapsable=True, collapse=False, height=200)
 
        self.browser = AOVBrowser(self.renderOptions)
        cmds.setParent(self.mainCol)

        cmds.frameLayout('arnoldAOVPrimaryFrame', label='AOVs', width=WIDTH,
                            collapsable=True, collapse=False)
        self.aovCol = cmds.columnLayout('arnoldAOVListColumn', adj=True)

        cmds.rowLayout('arnoldAOVButtonRow', nc=3, columnWidth3=[140, 100, 100], columnAttach3=['right', 'both', 'both'])
        cmds.text(label='')
        #cmds.button(label='Add Custom', c=lambda *args: shaderTemplate.newAOVPrompt())
        cmds.button(label='Delete All', c=lambda *args: (self.renderOptions.removeAOVs(self.aovRows.keys()), \
                                                            hooks.setupDefaultAOVs(self.renderOptions)))
        cmds.setParent('..') # rowLayout

        cmds.separator(style='in')
        cmds.rowLayout(nc=4,
                     columnWidth4=[130, 66, 80, 40],
                     columnAttach4=['both', 'both', 'both', 'both'])
        cmds.text(label='name')
        cmds.text(label='data')
        cmds.text(label='driver')
        cmds.text(label='filter')

        cmds.setParent('..') # rowLayout
        
        cmds.separator(style='in')

    #    cmds.text(_uiName('prefixLbl'), align='center', label='Prefix', parent=form)
    #    cmds.textField(_uiName('prefixFld'), enable=False, text='', parent=form, changeCommand=Callback(setAOVPrefix, aovnode))

        self.browser.populate()

        # add all control rows
        self.addRows()

        aovs.addAOVChangedCallback(self.refresh, 'aoveditor')
        
        # update AOV imageFormat of all rows when the default imageFormat changes.  a scriptJob will suffice here 
        cmds.scriptJob(parent=self.aovCol,
                     attributeChange=['{}.imageFormat'.format(self.renderOptions.node),
                                      lambda *args: cmds.evalDeferred(self.refresh)])

    def removeAOVCallbacks(self, *args):
        for attr in AOV_CALLBACK_ATTRS:
            try:
                callbacks.removeAttributeChangedCallbacks('aiAOV', attr)
            except KeyError:
                print "AOV callback no longer exists", attr

    def addRows(self):
        for aovName, aovList in self.renderOptions.getAOVs(group=True):
            frame = cmds.frameLayout(collapsable=False, labelVisible=False)
            col = cmds.columnLayout(adj=True)
            rows = []
            for aov in aovList:
                row = AOVItem(self, aov)
                rows.append(row)
                self.optionMenus.extend(row.getMenus())
            self.aovRows[aov.name] = rows
            self.aovControls.append(frame)
            cmds.setParent('..')
            cmds.setParent('..')

    def refresh(self):
        '''
        Delete and rebuild the AOV control rows
        '''
        self.waitingToRefresh = False
        try:
            cmds.setParent(self.aovCol)
        except:
            return
        cmds.columnLayout(self.aovCol, edit=True, visible=False)
        numDeleted = len(self.optionMenus)
        for ctrl in self.aovControls:
            cmds.deleteUI(ctrl)
        self.aovControls = []
        self.optionMenus = []
        self.aovRows = {}

        # add all control rows
        try:
            if cmds.objExists(self.renderOptions.node):
                self.addRows()
        except:
            return

        self.browser.updateActiveAOVs()

        cmds.columnLayout(self.aovCol, edit=True, visible=True)

        # a maya bug causes menus to ignore their specified width
        #print "refresh", numDeleted, len(self.optionMenus)
        if numDeleted != len(self.optionMenus):
            #print "creating script job"
            callbacks.DelayedIdleCallbackQueue(self.fixOptionMenus)
            #cmds.scriptJob(runOnce=True, idleEvent=self.fixOptionMenus)
            #cmds.evalDeferred(self.fixOptionMenus)

    def removeAOV(self, aov):
        self.renderOptions.removeAOV(aov)

    def setEnabledState(self):
        mode = cmds.getAttr('{}.aovMode'.format(self.renderOptions.node))
        state = mode > 0
        cmds.columnLayout(self.mainCol, edit=True, enable=state)


    def fixOptionMenus(self):
        '''
        Callback to fix an annoying bug where option menus do not respect their set width.
        ''' 
        # The only thing I've found that will make the option menu's return to their proper width
        # is hiding and then unhiding them.  However, this must be delayed until after the window they're in
        # is shown. Even an idle event scriptJob (set to run-once) is not 100% successful as sometimes it
        # is trigger too soon.  This technique relies on an idle callback where we skip the first coule fires
        # before calling fixOptionMenus() callback and removing the idle callback. -CHAD
        #print self.idle_id, self.idle_ticker
        for menu in self.optionMenus:
            #print "fixing", menu
            try:
                cmds.optionMenu(menu, edit=True, visible=False)
                cmds.optionMenu(menu, edit=True, visible=True)
            except:
                pass


        
def arnoldAOVEditor(*args):
    if cmds.window(UI_NAME, exists=True):
        cmds.deleteUI(UI_NAME)
    win = cmds.window(UI_NAME, title='AOV setup', width=640, height=300)
    import time
    s = time.time()
    ed = ArnoldAOVEditor()
    print time.time() - s
    win.show()
    cmds.evalDeferred(ed.fixOptionMenus)
    return ed

def arnoldAOVBrowser(**kwargs):
    core.createOptions()
    win = cmds.window(title='AOV Browser', width=640, height=300)
    browser = AOVBrowser(**kwargs)
    browser.populate()
    win.show()
    return browser


global _aovDisplayCtrl
_aovDisplayCtrl= None
global _aovShadersFrame
_aovShadersFrame = None
global _aovShaders
_aovShaders = []

def updateAovShaders(*args):

    global _aovShaders
    global _aovShadersFrame

    for row in _aovShaders:
        row.delete()

    _aovShaders = []

    cmds.setParent(_aovShadersFrame)
    shadersSize = cmds.getAttr('defaultArnoldRenderOptions.aov_shaders', s=True)

    for i in range(shadersSize):

        frame = cmds.frameLayout(collapsable=False, labelVisible=False)
        _aovShaders.append(frame)

        rowName = 'arnoldAOVShadersRow%d' % i
        cmds.rowLayout(rowName, nc=2, columnWidth2=[350, 50], columnAttach2=['both', 'right'])
        aovCtrlName = 'aov_shaders%d' % i
        aovShaderName = 'defaultArnoldRenderOptions.aov_shaders[%d]' % i

        cmds.attrNavigationControlGrp(aovCtrlName,
                                label='',
                                at=aovShaderName, cn="createRenderNode -allWithShadersUp \"defaultNavigation -force true -connectToExisting -source %node -destination "+aovShaderName+"\" \"\"")

        aovShaderDelete = 'arnoldAOVShaderDelete%d' % i
        cmds.symbolButton(aovShaderDelete, image="SP_TrashIcon.png", command=lambda *args: deleteAovShader(i))
        cmds.setParent(_aovShadersFrame)

    cmds.setParent('..')
    

def deleteAovShader(index):
    global _aovShaders
    global _aovShadersFrame

    if _aovShaders is None:
        return

    shadersLength = len(_aovShaders)

    if index < 0 or index >= shadersLength:
        return 

    if index <= shadersLength - 2:
        for i in range(index, shadersLength - 1):
            aovShaderElem = 'defaultArnoldRenderOptions.aov_shaders[%d]' % i
            elemConnection = cmds.listConnections(aovShaderElem,p=True, d=False,s=True)
            if (not elemConnection is None):
                for elem in elemConnection:
                    cmds.disconnectAttr(elem, aovShaderElem)


            aovShaderNextElem = 'defaultArnoldRenderOptions.aov_shaders[%d]' % (i+1)
            nextElemConnection = cmds.listConnections(aovShaderNextElem,p=True, d=False,s=True)
            if (not nextElemConnection is None) and len(nextElemConnection) > 0:
                cmds.connectAttr(nextElemConnection[0], aovShaderElem)

    aovShaderElem = 'defaultArnoldRenderOptions.aov_shaders[%d]' % (shadersLength - 1)
    cmds.removeMultiInstance(aovShaderElem , b=True)

    updateAovShaders()

def addAovShader(*args):

    shadersSize = cmds.getAttr('defaultArnoldRenderOptions.aov_shaders', s=True)
    attrName = 'defaultArnoldRenderOptions.aov_shaders[%d]' % shadersSize

    melCmd = "createRenderNode -all \"defaultNavigation -force true -connectToExisting -destination "+attrName+" -source %node\" \"\""
    mel.eval(melCmd);
    updateAovShaders()


def createArnoldAOVTab():
    parentForm = cmds.setParent(query=True)

    aovNode = aovs.AOVInterface()
    cmds.columnLayout('enableAOVs', adjustableColumn=True)

    cmds.setUITemplate('attributeEditorTemplate', pushTemplate=True)

    legacyFrame = cmds.frameLayout('legacyFrame', label='Legacy', width=WIDTH,
                            collapsable=True, collapse=True)

    mayaRenderViewFrame = cmds.frameLayout('mayaRenderViewFrame', label='Maya Render View', width=WIDTH,
                            collapsable=True, collapse=True)

    cmds.attrControlGrp(attribute='{}.aovMode'.format(aovNode.node), label='Mode')

    # the tab gets recreated from scratch each time rather than updated and each
    # time the AOVOptionMenuGrp adds itself to the AOVChanged callback list. 
    # we must remove it or we'll leave behind invalid copies
    global _aovDisplayCtrl
    if _aovDisplayCtrl is not None:
        aovs.removeAOVChangedCallback(_aovDisplayCtrl.safeUpdate)

    _aovDisplayCtrl = shaderTemplate.AOVOptionMenuGrp('aiOptions', 'displayAOV', label='Render View AOV',
                                           allowCreation=False,
                                           includeBeauty=True,
                                           allowEmpty=False,
                                           allowDisable=False)
    _aovDisplayCtrl._setToChildMode()
    _aovDisplayCtrl._doSetup('{}.displayAOV'.format(aovNode.node))

    cmds.setParent('..')

    # aov shader
    global _aovShadersFrame
    _aovShadersFrame = cmds.frameLayout('arnoldAOVShadersFrame', label='AOV Shaders', width=WIDTH,
                        collapsable=True, collapse=True)

    cmds.scriptJob(parent=_aovShadersFrame, attributeChange=['defaultArnoldRenderOptions.aov_shaders', updateAovShaders], dri=True, alc=True, per=True )

    cmds.rowLayout('arnoldAOVShaderButtonRow', nc=3, columnWidth3=[140, 100, 100], columnAttach3=['right', 'both', 'both'])
    cmds.text(label='')
    cmds.button(label='Add', c=lambda *args: addAovShader())
    cmds.setParent('..') # rowLayout

    cmds.setParent(_aovShadersFrame)
    updateAovShaders()

    cmds.setParent('..')
    cmds.setParent(parentForm)

    cmds.scrollLayout('arnoldAOVsScrollLayout', horizontalScrollBarThickness=0)

    cmds.columnLayout('arnoldTabColumn', adjustableColumn=True)

    global _tabEditor
    _tabEditor = ArnoldAOVEditor(aovNode)

    cmds.formLayout(parentForm,
               edit=True,
                    af=[('enableAOVs',"top", 5),
                        ('enableAOVs', "left", 0),
                        ('enableAOVs', "right", 0),
                        ('arnoldAOVsScrollLayout', "bottom", 0),
                        ('arnoldAOVsScrollLayout', "left", 0),
                        ('arnoldAOVsScrollLayout', "right", 0)],
                    an=[('enableAOVs', "bottom")],
                    ac=[('arnoldAOVsScrollLayout', "top", 5, 'enableAOVs')])

    cmds.setUITemplate('attributeEditorTemplate', popTemplate=True)

    cmds.setParent(parentForm)
    cmds.evalDeferred(_tabEditor.fixOptionMenus)
    _tabEditor.setEnabledState()
    cmds.scriptJob(attributeChange = ('{}.aovMode'.format(aovNode.node), _tabEditor.setEnabledState), parent=_tabEditor.mainCol)

    #cmds.setUITemplate(popTemplate=True)

def updateArnoldAOVTab():
    pass

def refreshArnoldAOVTab():
    global _tabEditor
    if _tabEditor:
        _tabEditor.refresh()

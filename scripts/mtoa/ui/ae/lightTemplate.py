import mtoa.ui.ae.utils as aeUtils
import mtoa.core as core
import mtoa.melUtils as melUtils
from mtoa.callbacks import *
from mtoa.ui.ae.templates import AttributeTemplate
from mtoa.utils import prettify
from mtoa.lightFilters import getLightFilterClassification
import mtoa.callbacks as callbacks
import arnold as ai
import maya.cmds as cmds
import maya.mel

def getSourcePlug(plugname, index):
    conns = []
    if index < 0:
        conns = cmds.listConnections(plugname, p=1, s=1, d=0)
    else:
        conns = cmds.listConnections('%s[%s]'%(plugname, index), p=1, s=1, d=0)
    if conns:
        if len(conns) == 1:
            return conns[0]
        else:
            return ""

def getNodeName(plugname):
    if not plugname:
        return 0
    nodeAttrs = plugname.split('.')
    if len(nodeAttrs) >= 1:
        return nodeAttrs[0]
    return "";

def getConnectedCount(plugname):
    srcplug = ''
    n = melUtils.getAttrNumElements(*plugname.split('.', 1))
    c = 0

    for i in range(0, n):
        srcplug = getSourcePlug(plugname, i)
        if srcplug:
            c+=1

    return c

def swapConnections(plugname, i_zero, i_one):

    dstPlug_zero = '%s[%s]'%(plugname, i_zero)
    dstPlug_one = '%s[%s]'%(plugname, i_one)
    srcPlug_zero = getSourcePlug(dstPlug_zero, -1)
    srcPlug_one = getSourcePlug(dstPlug_one, -1)

    if srcPlug_zero != "":
        cmds.disconnectAttr(srcPlug_zero, dstPlug_zero)

    if srcPlug_one != "":
        cmds.disconnectAttr(srcPlug_one, dstPlug_one)

    if srcPlug_zero != "":
        cmds.connectAttr(srcPlug_zero, dstPlug_one)

    if srcPlug_one != "":
        cmds.connectAttr(srcPlug_one, dstPlug_zero)

class LightFilterWindow(object):
    
    def __init__(self, template):
        self.template = template
        self.win = "arnold_filter_list_win"
        if cmds.window(self.win, exists=True):
            cmds.deleteUI(self.win)
    
        cmds.window(self.win, title="Add Light Filter",
                    sizeable=False,
                    resizeToFitChildren=True)
        #cmds.windowPref(removeAll=True)
        cmds.columnLayout(adjustableColumn=True,
                          columnOffset=("both", 10),
                          #columnAttach=('both',1),
                          rowSpacing=10)
    
        self.scrollList = cmds.textScrollList('alf_filter_list', nr=4, ams=False)
        cmds.textScrollList(self.scrollList,
                            e=True,
                            doubleClickCommand=Callback(self.addFilterAndHide))

        for label, nodeType in self.filters():
            cmds.textScrollList(self.scrollList, edit=True, append=label)

        cmds.rowLayout(numberOfColumns=2, columnAlign2=("center", "center"))
        cmds.button(width=100, label="Add", command=Callback(self.addFilterAndHide))
        cmds.button(width=100, label="Cancel", command=Callback(cmds.deleteUI, self.win, window=True))
    
        cmds.setParent('..')
        cmds.setParent('..')

        cmds.showWindow(self.win)

    def filters(self):
        result = []
        for filter in self.template.validFilters():
            result.append((prettify(filter).strip('Ai '), filter))
        return result

    def addFilterAndHide(self):
        cmds.window(self.win, edit=True, visible=False)
        filterLabels = cmds.textScrollList(self.scrollList, q=True, si=True)
        if filterLabels:
            filterLabels = filterLabels[0]
        else:
            return
        filterNodeType = dict(self.filters())[filterLabels]
        self.template.addLightFilter(filterNodeType)

from functools import partial

class ColorTemperatureTemplate:
    def updateUseColorTemperature(self, *args):
        try:
            cmds.attrFieldSliderGrp(self.sliderCtrl, edit=True, enable=cmds.getAttr(self.nodeAttr('aiUseColorTemperature')))
        except:
            pass

    def updateColorTemperature(self, *args, **kwargs):
        try:
            temperature = cmds.getAttr(self.nodeAttr('aiColorTemperature'))
            colorTemp = cmds.arnoldTemperatureToColor(temperature)
            displayColor = colorTemp
            if maya.mel.eval("exists \"colorManagementConvert\""):
                displayColor = cmds.colorManagementConvert(toDisplaySpace=colorTemp)

            displayColor[0] = min(max(displayColor[0], 0.0), 1.0)
            displayColor[1] = min(max(displayColor[1], 0.0), 1.0)
            displayColor[2] = min(max(displayColor[2], 0.0), 1.0)
            cmds.canvas(self.canvasName, edit=True, rgbValue=displayColor)
        except:
            pass

    def createLightColorTemperatureUI(self, attrName):
        cmds.setUITemplate('attributeEditorPresetsTemplate', pushTemplate=True)
        isEnabled = True
        isEnabled = cmds.getAttr(self.nodeAttr('aiUseColorTemperature'))
        aeUtils.attrBoolControlGrp(self.checkBoxName, attribute=self.nodeAttr('aiUseColorTemperature'),
                                   label='Use Color Temperature', changeCommand=lambda *args: self.updateUseColorTemperature(*args))
        cmds.setParent('..')        
        cmds.rowLayout(numberOfColumns=2, columnWidth2=(80,220), adjustableColumn=2, columnAttach=[(1, 'left', 0), (2, 'left', -10)])
        cmds.canvas(self.canvasName, width=65, height=12)
        self.sliderCtrl = cmds.attrFieldSliderGrp(self.sliderName, label='Temperature', width=220, 
                                attribute=self.nodeAttr('aiColorTemperature'),
                                enable=isEnabled,
                                precision=0, columnWidth=[(1, 70), (2, 70), (3, 80)], changeCommand=self.updateColorTemperature)
        cmds.setParent('..')
        colorTemp = cmds.arnoldTemperatureToColor(cmds.getAttr(self.nodeAttr('aiColorTemperature')))
        displayColor = colorTemp
        if maya.mel.eval("exists \"colorManagementConvert\""):
            displayColor = cmds.colorManagementConvert(toDisplaySpace=displayColor)

        displayColor[0] = min(max(displayColor[0], 0.0), 1.0)
        displayColor[1] = min(max(displayColor[1], 0.0), 1.0)
        displayColor[2] = min(max(displayColor[2], 0.0), 1.0)

        cmds.canvas(self.canvasName, edit=True, rgbValue=displayColor)
        cmds.setUITemplate(popTemplate=True)

    def updateLightColorTemperatureUI(self, attrName):
        isEnabled = True
        isEnabled = cmds.getAttr(self.nodeAttr('aiUseColorTemperature'))
        aeUtils.attrBoolControlGrp(self.checkBoxName, edit=True, attribute=self.nodeAttr('aiUseColorTemperature'), 
                                   changeCommand=self.updateUseColorTemperature)
        cmds.attrFieldSliderGrp(self.sliderCtrl, edit=True, 
                                attribute=self.nodeAttr('aiColorTemperature'), enable=isEnabled,
                                changeCommand=self.updateColorTemperature)
        colorTemp = cmds.arnoldTemperatureToColor(cmds.getAttr(self.nodeAttr('aiColorTemperature')))
        displayColor = colorTemp
        if maya.mel.eval("exists \"colorManagementConvert\""):
            displayColor = cmds.colorManagementConvert(toDisplaySpace=colorTemp)

        displayColor[0] = min(max(displayColor[0], 0.0), 1.0)
        displayColor[1] = min(max(displayColor[1], 0.0), 1.0)
        displayColor[2] = min(max(displayColor[2], 0.0), 1.0)
        cmds.canvas(self.canvasName, edit=True, rgbValue=displayColor)
            
    def setupColorTemperature(self, lightType=""):
        self.sliderCtrl = ""
        self.sliderName = '%s_LightColorTemperature' % lightType
        self.checkBoxName = '%s_UseLightColorTemperature' % lightType
        self.canvasName = '%s_LightColorCanvas' % lightType
        self.addCustom('aiUseColorTemperature', self.createLightColorTemperatureUI, self.updateLightColorTemperatureUI)
        self.channelBox = False;
        self.addSeparator()

class LightTemplate(AttributeTemplate, ColorTemperatureTemplate):
    MENU_NODE_TYPE = 0
    MENU_NODE_INSTANCE = 1
    _callbacks = []
    def __init__(self, nodeType):
        super(LightTemplate, self).__init__(nodeType)

    def validFilters(self):
        return getLightFilterClassification(self.nodeType())

    def commonLightAttributes(self, addUserOptions = True, addVisibility = False):        
        self.addControl("aiCastVolumetricShadows", label="Cast Volumetric Shadows")
        
        self.addControl("aiVolumeSamples", label="Volume Samples")
        
        self.addSeparator()
    
        self.beginLayout("Visibility" , collapse = False)
        # for now only Skydome supports it, but area lights will also have it soon
        if addVisibility:
            self.addControl("camera", label="Camera")
            self.addControl("transmission", label="Transmission")    
        self.addControl("aiDiffuse", label="Diffuse")
        self.addControl("aiSpecular", label="Specular")
        self.addControl("aiSss", label="SSS")
        self.addControl("aiIndirect", label="Indirect")
        self.addControl("aiVolume", label="Volume")
        self.addControl("aiMaxBounces", label="Max Bounces")
        self.endLayout()

        self.addSeparator()
        self.addControl("aiAov", label="AOV Light Group")
        
        self.lightFiltersLayout()
        
        if addUserOptions:
            self.addControl("aiUserOptions", "User Options")

    def lightFiltersLayout(self):
        self.beginLayout("Light Filters", collapse=False)
        self.addCustom("aiFilters", self.customLightFiltersNew, self.customLightFiltersReplace)
        self.endLayout()

    def moveLightFilterUp(self):
        items = cmds.textScrollList(self.scrollList, q=True, sii=True)
        if not items:
            return 0
    
        current = items[0] - 1
        if current > 0:
            swapConnections(self.nodeAttr("aiFilters"), current-1, current)
            self.lightFiltersUpdateList()

    def moveLightFilterDown(self):
        attr = self.nodeAttr('aiFilters')
        items = cmds.textScrollList(self.scrollList, q=True, sii=True)
        if not items:
            return 0

        current = items[0]
        if current < getConnectedCount(attr):
            swapConnections(attr, current-1, current)
            self.lightFiltersUpdateList()

    def lightFilterListChanged(self):
        '''
        update the buttons in response to a change to the list of connected filters
        '''
        items = cmds.textScrollList(self.scrollList, q=True, si=True)

        selection = 1 if items else 0

        cmds.button('lf_remove_button', edit=True, enable=selection)
        cmds.button('lf_move_up_button', edit=True, enable=selection)
        cmds.button('lf_move_down_button', edit=True, enable=selection)

    def addLightFilterWin(self):
        LightFilterWindow(self)

    def addLightFilterCB(self, name):
        """
        This callback is triggered when the filter menu changes.  The filter menu contains a list of 
        filter types, and of existing filter nodes.
        """
        if not name or name == '<Add Filter>':
            # selected a menu divider. reset
            self.updateAddMenu()
            return
        items = cmds.optionMenuGrp(self.addOptionMenuGrp, query=True, itemListLong=True)
        index = cmds.optionMenuGrp(self.addOptionMenuGrp, query=True, select=True)
        # get the mode of the callback
        mode = cmds.menuItem(items[index-1], query=True, data=True)
        if mode == self.MENU_NODE_TYPE:
            # name is a type
            newFilter = self.addLightFilter(name)
            # not sure whether selecting is the right thing to do. it's a bit of jolt.
            #cmds.mel.updateAE(newFilter)
        else: # MENU_NODE_INSTANCE
            # name is an existing node
            self.connectLightFilter(name)

    def addLightFilter(self, filterNodeType):
        '''
        create and connect a filter of the passed type
        '''
        newFilter = core.createArnoldNode(filterNodeType, skipSelect=True)
        self.connectLightFilter(newFilter)
        return newFilter

    def connectLightFilter(self, newFilter):
        attr =  self.nodeAttr('aiFilters')
        nfilters = getConnectedCount(attr)
        cmds.connectAttr('%s.message' % newFilter, '%s[%s]'%(attr, nfilters), force=True)
        self.lightFiltersUpdateList()
        self.updateAddMenu()

    def removeLightFilter(self):
        attr = self.nodeAttr('aiFilters')

        selection = cmds.textScrollList(self.scrollList, q=True, si=True)
        selected = selection[0] if selection else ''

        nfilters = getConnectedCount(attr)

        for j in range(0, nfilters):
            srcplug = getSourcePlug(attr, j)
            filter = getNodeName(srcplug)

            if filter == selected:
                for k in range(j+1, nfilters):
                    swapConnections(attr, j, k)
                    j+=1
                cmds.disconnectAttr(srcplug, '%s[%s]'%(attr, nfilters-1))
                # node might be used elsewhere, so we can't just delete it
                # Note: with proper 'existsWithoutConnections' settings Maya would do it if it isn't
                # connected to anything anymore
                #cmds.delete(filter)

        self.lightFiltersUpdateList()
        self.updateAddMenu()

    def getConnectedLightFilters(self):
        nfilters = getConnectedCount(self.nodeAttr('aiFilters'))
        result = []
        for j in range(0, nfilters):
            filter = getNodeName(getSourcePlug(self.nodeAttr('aiFilters'), j))
            if not filter:
                continue
            result.append(filter)
        return result

    def lightFiltersUpdateList(self):
        '''
        refresh the list of connected light filters
        '''
        selection = cmds.textScrollList(self.scrollList, q=True, si=True)
        selected = selection[0] if selection else ''
    
        cmds.textScrollList(self.scrollList, edit=True, removeAll=True)
        for filter in self.getConnectedLightFilters():
            cmds.textScrollList(self.scrollList, edit=True, append=filter)
            if filter == selected:
                cmds.textScrollList(self.scrollList, edit=True, si=selected)
    
        self.lightFilterListChanged()

    def customLightFiltersChanged(self, userChangeCB=None):
        if userChangeCB:
            cmds.evalDeferred(userChangeCB)

    def updateCustomLightFiltersNew(self):
        val = cmds.textScrollList(self.scrollList, q=True, si=True)
        if val:
            maya.mel.eval("updateAE "+val[0])

    def updateAddMenu(self):
        # clear
        for item in cmds.optionMenuGrp(self.addOptionMenuGrp, query=True, itemListLong=True) or []:
            cmds.deleteUI(item)
        # rebuild
        cmds.menuItem(label='<Add Filter>', parent=self.addOptionMenu)
        if self.validFilters():
            for filterType in self.validFilters():
                cmds.menuItem(label=filterType, data=self.MENU_NODE_TYPE, parent=self.addOptionMenu)
            connected = self.getConnectedLightFilters()
            existing = [node for node in cmds.ls(type=self.validFilters()) or [] if node not in connected]
            if existing:
                #cmds.menuItem(label='<Existing Filters...>', parent=self.addOptionMenu)
                cmds.menuItem(divider=True, parent=self.addOptionMenu)
                for filter in existing:
                    cmds.menuItem(label=filter, data=self.MENU_NODE_INSTANCE, parent=self.addOptionMenu)

    def customLightFiltersNew(self, attr):
        cmds.rowLayout(numberOfColumns=3,
                       adjustableColumn=2,
                       rowAttach=(1, "top", 0),
                       columnWidth=[(1, 140), (2, 180)])
        cmds.text(label="")
        cmds.columnLayout(adjustableColumn=True,
                          columnAttach=("both", 0),
                          rowSpacing=5)
        uiName = '%s_aiFilter'%(self.nodeType())
        self.scrollList = cmds.textScrollList(uiName, height=150, ams=False,
                                              sc=self.lightFilterListChanged,
                                              dcc=self.updateCustomLightFiltersNew)

        cmds.rowLayout(numberOfColumns=2,
                       columnWidth2=(80,80),
                       columnAttach2=("both", "both"),
                       columnAlign2=("center", "center"),
                       columnOffset2=(2, 2))

        cmds.button('lf_add_button', label="Add", c=Callback(self.addLightFilterWin))
        cmds.button('lf_remove_button', label="Disconnect", c=Callback(self.removeLightFilter))
        # implicit end of row layout
        cmds.setParent('..') # back to column layout
        cmds.setParent('..') # back to row layout
        cmds.columnLayout(adjustableColumn=True,
                          columnAttach=("both", 0),
                          rowSpacing=5)
        cmds.symbolButton('lf_move_up_button', image='arrowUp.xpm', c=Callback(self.moveLightFilterUp))
        cmds.symbolButton('lf_move_down_button', image='arrowDown.xpm', c=Callback(self.moveLightFilterDown))
        cmds.setParent('..')
        cmds.setParent('..')
                
        self.addOptionMenuGrp = cmds.optionMenuGrp('lf_add_menu', label='Add',
                                                 changeCommand=self.addLightFilterCB)
        self.addOptionMenu = self.addOptionMenuGrp + '|OptionMenu'
    
        self.lightFiltersUpdateList()
        self.updateAddMenu()

    def customLightFiltersReplace(self, attr):
        self.lightFiltersUpdateList()
        self.updateAddMenu()

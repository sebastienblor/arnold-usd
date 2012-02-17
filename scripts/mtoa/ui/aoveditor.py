import pymel.core as pm
from mtoa.callbacks import *
import mtoa.aovs as aovs
import mtoa.ui.ae.shaderTemplate as shaderTemplate
import mtoa.core as core
from collections import defaultdict
import sys

UI_NAME = 'ArnoldAOVUI'
AOV_ATTRS = ('name', 'type', 'prefix')
WIDTH = 400

def _uiName(tag):
    return '%s_%s' % (UI_NAME, tag)

global _updating
_updating = False

class AOVBrowser(object):
    '''
    A UI for browsing node types and their registered AOVs
    '''
    def __init__(self, renderOptions=None, nodeTypes=None, listAOVGroups=True, showGroupsColumn=True):
        '''
        renderOptions : an aov.AOVNode instance, or None to use the default
        
        nodeTypes : a list of node types to display in the available nodes column, 
            or None to display the complete list of nodes with AOVs
        '''
        print "INIT"
        self.allAOVs = set([])
        self.renderOptions = aovs.getAOVNode() if renderOptions is None else renderOptions
        self.allNodeTypes = set(aovs.getNodeTypesWithAOVs())
        if nodeTypes:
            self.setNodeTypes(nodeTypes)
        else:
            self.nodeTypes = sorted(self.allNodeTypes)

        self.doAOVGroups = listAOVGroups
        self.doGroups = showGroupsColumn
        print self.nodeTypes, self.doGroups

        self.form = pm.formLayout()
        if self.doGroups:
            groupsLbl = pm.text(_uiName('groupsLbl'), align='center', label='AOV Groups')
        availableLbl = pm.text(_uiName('availableLbl'), align='center', label='Available AOVs')
        activeLbl = pm.text(_uiName('activeLbl'), align='center', label='Active AOVs')

        if self.doGroups:
            self.groupLst = pm.textScrollList(_uiName('groupLst'), numberOfRows=10, allowMultiSelection=True,
                          selectCommand=self.updateActiveAOVs)
        self.availableLst = pm.textScrollList(_uiName('availableLst'), numberOfRows=10, allowMultiSelection=True,
                          doubleClickCommand=self.addAOVs)
        self.activeLst = pm.textScrollList(_uiName('activeLst'), numberOfRows=10, allowMultiSelection=True,
                          doubleClickCommand=self.removeAOVs)

        addBtn = pm.button(_uiName('addBtn'), label='>>', command=self.addAOVs)
        remBtn = pm.button(_uiName('remBtn'), label='<<', command=self.removeAOVs)

        pm.formLayout(self.form, edit=True, attachForm=[
                                    (groupsLbl, 'top', 1),
                                    (availableLbl, 'top', 1),
                                    (activeLbl, 'top', 1),
                                    
                                    (groupsLbl, 'left', 1),
                                    (self.groupLst, 'left', 1),
                                    
                                    (activeLbl, 'right', 1),
                                    (self.activeLst, 'right', 1),
                                    
                                    (remBtn, 'right', 1),
                                    (addBtn, 'bottom', 1),
                                    (remBtn, 'bottom', 1),
                               ])
        pm.formLayout(self.form, edit=True, attachControl=[
                                    #(availableLbl, 'left', 1, groupsLbl),
                                    
                                    (self.groupLst, 'top', 1, groupsLbl),
                                    (self.activeLst, 'top', 1, activeLbl),
                                    (self.availableLst, 'top', 1, availableLbl),
                                    
                                    (self.groupLst, 'bottom', 1, addBtn),
                                    (self.activeLst, 'bottom', 1, addBtn),
                                    (self.availableLst, 'bottom', 1, remBtn),
                                    
                                    (addBtn, 'right', 1, remBtn),
#                                    (addBtn, 'top', 1, self.activeLst),
#                                    (remBtn, 'top', 1, self.availableLst)
                                ])

        pm.formLayout(self.form, edit=True, attachPosition=[
                                    (groupsLbl, 'right', 1, 33),
                                    (availableLbl, 'left', 1, 33),
                                    (availableLbl, 'right', 1, 66),
                                    (activeLbl, 'left', 1, 66),
                                    
                                    (self.groupLst, 'right', 1, 33),
                                    (self.availableLst, 'left', 1, 33),
                                    (self.availableLst, 'right', 1, 66),
                                    (self.activeLst, 'left', 1, 66),
                                    
                                    (addBtn, 'left', 1, 33),
                                    (addBtn, 'right', 1, 66),
                                    (remBtn, 'left', 1, 66),
                                ])
 


    def setNodeTypes(self, nodeTypes):
        '''set the node types listed in the AOV Groups column'''
        self.nodeTypes = sorted(self.allNodeTypes.intersection(nodeTypes))
 
    def populate(self):
        '''
        update the contents of all scroll lists
        '''
        if self.doGroups:
            pm.textScrollList(self.groupLst, edit=True, removeAll=True)
            if self.doAOVGroups:
                for nodeType in aovs.getAOVGroups():
                    pm.textScrollList(self.groupLst, edit=True, append=nodeType)
            for nodeType in self.nodeTypes:
                # make sure we have at least one named aov
                # FIXME: what does an empty AOV mean? why are these nodes returned by getNodeTypesWithAOVs()?  
                #if any([x for x in aovs.getRegisteredAOVs(nodeType=nodeType) if x]):
                pm.textScrollList(self.groupLst, edit=True, append=nodeType)
        # populate available and active based on aovs provided by groups and nodes
        self.updateActiveAOVs()

    def addAOVs(self, *args):
        '''
        create the selected AOVs, and connect the new AOV nodes to their corresponding
        AOV attributes for any nodes in the scene.
        '''
        map = defaultdict(list)
        typeMap = {}
        for nodeType in self.nodeTypes:
            for aovName, attr, type in aovs.getNodeGlobalAOVData(nodeType):
                map[aovName].append((nodeType, attr))
                typeMap[aovName] = type
        typeMap.update(dict(aovs.BUILTIN_AOVS))
        
        sel = pm.textScrollList(self.availableLst, query=True, selectItem=True)
        if sel:
            global _updating
            _updating = True
            try:
                for aov in sel:
                    aovNode = self.renderOptions.addAOV(aov, typeMap[aov])
                    aovNameAttr = aovNode.attr('name')
                    # connect this aov node to all existing aov attributes
                    for nodeType, aovAttr in map[aov]:
                        for node in pm.ls(type=nodeType):
                            try:
                                # do not force, in case it already has an incoming connection
                                aovNameAttr.connect(node.attr(aovAttr))
                            except:
                                pass
            finally:
                _updating = False
            self.updateActiveAOVs()

    def removeAOVs(self, *args):
        '''
        delete the selected AOVs
        '''
        sel = pm.textScrollList(self.activeLst, query=True, selectItem=True)
        if sel:
            global _updating
            _updating = True
            try:
                for aov in sel:
                    if self.renderOptions.removeAOV(aov) is not None:
                        pm.textScrollList(self.availableLst, edit=True, append=aov)
                        pm.textScrollList(self.activeLst, edit=True, removeItem=aov)
            finally:
                _updating = False
            self.updateActiveAOVs()

    def updateActiveAOVs(self):
        '''
        fill the active and inactive columns based on the nodeType/group selected
        '''
        if _updating:
            return
        
        if not self.doGroups or (not self.doAOVGroups and len(self.nodeTypes) == 1):
            groups = self.nodeTypes
        else:
            groups = pm.textScrollList(self.groupLst, query=True, selectItem=True)
            

        # first, find out what's selected, so we can reselect any persistent items
        sel = pm.textScrollList(self.availableLst, query=True, selectItem=True)

        # update the available list
        pm.textScrollList(self.availableLst, edit=True, removeAll=True)
        pm.textScrollList(self.activeLst, edit=True, removeAll=True)
        activeAOVs = self.renderOptions.getActiveAOVs()
        self.allAOVs = set([])
        for group in groups:
            if group.startswith('<'):
                # it's an AOV group
                aovList = aovs.getGroupAOVs(group)
            else:
                aovList = [x for x in aovs.getRegisteredAOVs(nodeType=group) if x]
            self.allAOVs.update(aovList)
            for aovName in aovList:
                if aovName not in activeAOVs:
                    pm.textScrollList(self.availableLst, edit=True, append=aovName)
                    if aovName in sel:
                        pm.textScrollList(self.availableLst, edit=True, selectItem=aovName)
                else:
                    pm.textScrollList(self.activeLst, edit=True, append=aovName)

class ArnoldAOVEditor(object):

    def __init__(self, aovNode=None):
        self.waitingToRefresh = False
        self.aovControls = {}
        self.renderOptions = aovs.getAOVNode() if aovNode is None else aovNode

        pm.columnLayout()

        pm.frameLayout(label='AOV Browser', width=WIDTH, collapsable=True, collapse=False)

        self.browser = AOVBrowser(self.renderOptions)
        pm.setParent('..') # frame

        pm.frameLayout(label='Primary AOVs', width=WIDTH, collapsable=True, collapse=False)
        self.aovCol = pm.columnLayout()
        pm.rowLayout(nc=3, columnWidth3=[140, 100, 100], columnAttach3=['right', 'both', 'both'])
        pm.text(label='')
        pm.button(label='Add Custom', c=lambda *args: shaderTemplate.newAOVPrompt())
        pm.button(label='Delete All', c=lambda *args: pm.delete(pm.ls(type='aiAOV')))
        pm.setParent('..') # rowLayout

        pm.separator()

    #    pm.text(_uiName('prefixLbl'), align='center', label='Prefix', parent=form)
    #    pm.textField(_uiName('prefixFld'), enable=False, text='', parent=form, changeCommand=Callback(setAOVPrefix, aovnode))

        self.browser.populate()

        # add all control rows
        for aovNode in self.renderOptions.getActiveAOVNodes():
            self.addControlRow(aovNode)

        aovs.addAOVChangedCallback(self.refresh)
#        self.id = pm.api.MNodeMessage.addAttributeChangedCallback(self.renderOptions.node.__apimobject__(),
#                                                                  self.attrAddedCallback)
#        pm.scriptJob(uiDeleted=[str(self.aovCol), lambda *args: pm.api.MNodeMessage.removeCallback(self.id)])

    def refresh(self):
        '''
        rebuild the control rows
        '''
        self.waitingToRefresh = False
        pm.setParent(self.aovCol)
        for ctrl in self.aovControls.values():
            ctrl.delete()
        self.aovControls = {}
        # add all control rows
        for aovNode in self.renderOptions.getActiveAOVNodes():
            self.addControlRow(aovNode)
        self.browser.updateActiveAOVs()

#    def attrAddedCallback(self, msg, plug, otherPlug, *args):
#        #print "attr changed", msg, pm.Attribute(plug)
#        #if msg & pm.api.MNodeMessage.kAttributeArrayAdded:
#        if self.waitingToRefresh or om.MFileIO.isOpeningFile() or om.MFileIO.isNewingFile():
#            return
#        if msg & pm.api.MNodeMessage.kConnectionMade or msg & pm.api.MNodeMessage.kConnectionBroken:
##            attr = pm.Attribute(plug)
##            otherAttr = pm.Attribute(otherPlug)
##            print "plug connected", attr, otherAttr
#            self.waitingToRefresh = True
#            pm.evalDeferred(self.refresh)

#            index = attr.index()
#            #print "index", index, "numControls", len(self.aovControls)
#            if index == len(self.aovControls):
#                #print "adding row"
#                self.addControlRow(attr, index)
#        elif msg & pm.api.MNodeMessage.kAttributeArrayRemoved:
#            attr = pm.Attribute(plug)
#            print "array plug removed", attr
#            index = attr.index()
#            #print "index", index, "numControls", len(self.aovControls)
#            if index < len(self.aovControls):
#                #print "removing row"
#                self.removeControlRow()


    def addControlRow(self, aovNode, lockName=False):
        row = pm.rowLayout(numberOfColumns=5,
                                columnWidth5=[130, 150, 70, 20, 20],
                                columnAttach5=['right', 'both', 'both', 'both', 'both'])

        enabledCtrl = pm.checkBox(label='')
        pm.connectControl(enabledCtrl, aovNode.attr('enabled'))

        nameAttr = aovNode.attr('name')
        # use evalDeferred to ensure that the update happens after the aov node attribute is set
        nameCtrl = pm.textField(editable=not lockName,
                                # we save out the current aov name for the replaceShadingGroupDummyAttrs callback
                                changeCommand=lambda new, old=nameAttr.get(): pm.evalDeferred(self.refresh))
        pm.connectControl(nameCtrl, nameAttr)
        # must set editability after connecting control
        nameCtrl.setEditable(not lockName)

        # attrEnumOptionMenu does not work with multi-attrs and optionMenu does not work with connectControl,
        # so, unfortunately, our best option is attrEnumOptionMenuGrp
        pm.attrEnumOptionMenuGrp(attribute=str(aovNode.attr('type')), columnWidth2=[1, 60])
        pm.symbolButton(image="navButtonConnected.png",
                        command=lambda *args: pm.select(aovNode))

        pm.symbolButton(image="smallTrash.png",
                        command=lambda *args: self.removeAOV(aovNode))
        cmds.setParent('..')
        self.aovControls[aovNode] = row
        return row

    def removeControlRow(self, aovName):
        self.aovControls.pop(aovName).delete()

    def removeAOV(self, aovNode):
        self.renderOptions.removeAOVNode(aovNode)

def arnoldAOVEditor(*args):
    if pm.window(UI_NAME, exists=True):
        pm.deleteUI(UI_NAME)
    win = pm.window(UI_NAME, title='AOV setup', width=640, height=300)
    import time
    s = time.time()
    ed = ArnoldAOVEditor()
    print time.time() - s
    win.show()
    return ed

def arnoldAOVBrowser(**kwargs):
    core.createOptions()
    win = pm.window(title='AOV Browser', width=640, height=300)
    browser = AOVBrowser(**kwargs)
    browser.populate()
    win.show()
    return browser

def createArnoldAOVTab():
    parentForm = cmds.setParent(query=True)

    aovNode = aovs.getAOVNode()
    pm.columnLayout('enableAOVs', adjustableColumn=True)
    
    with pm.uitypes.UITemplate('attributeEditorTemplate'):
        pm.attrControlGrp(attribute=aovNode.node.aovMode, label='Mode')
    
        ctrl = shaderTemplate.AOVOptionMenuGrp('aiOptions', label='Render View AOV',
                                               allowCreation=False,
                                               includeBeauty=True,
                                               allowEmpty=False,
                                               allowDisable=False)
        ctrl._doSetup(aovNode.node.name() + '.displayAOV')
        pm.attrControlGrp('enable_aov_composition',
                          attribute = 'defaultArnoldRenderOptions.enable_aov_composition',
                          label = 'Enable AOV Composition')
    
    pm.setParent(parentForm)

    cmds.scrollLayout('arnoldAOVsScrollLayout', horizontalScrollBarThickness=0)

    cmds.columnLayout('arnoldTabColumn', adjustableColumn=True)

    ArnoldAOVEditor(aovNode)

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

    cmds.setParent(parentForm)
    #cmds.setUITemplate(popTemplate=True)

def updateArnoldAOVTab():
    pass

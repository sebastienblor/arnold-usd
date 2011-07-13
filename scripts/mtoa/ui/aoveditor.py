import pymel.core as pm
from mtoa.callbacks import *
import mtoa.aovs as aovs
import mtoa.ui.ae.shaderTemplate as shaderTemplate
from collections import defaultdict
import sys

UI_NAME = 'ArnoldAOVUI'
AOV_ATTRS = ('name', 'type', 'prefix')
WIDTH = 400
TYPES = ("int",
   "bool",
   "float",
   "rgb",
   "rgba",
   "vector",
   "point",
   "point2",
   "pointer")

def _uiName(tag):
    return '%s_%s' % (UI_NAME, tag)

global _updating
_updating = False

class AOVBrowser(object):
    '''
    A UI for browsing node types and their registered AOVs
    '''
    def __init__(self, renderOptions=None):
        self.renderOptions = aovs.getAOVNode() if renderOptions is None else renderOptions

        opts = {'columnWidth3' : [112, 142, 142], 'columnAttach3' : ['both']*3, 'columnOffset3' : [0, 0, 6]}

        pm.columnLayout()

        pm.rowLayout(nc=3, **opts)
        pm.text(_uiName('groupsLbl'), align='center', label='AOV Groups')
        pm.text(_uiName('availableLbl'), align='center', label='Available AOVs')
        pm.text(_uiName('activeLbl'), align='center', label='Active AOVs')
        pm.setParent('..') # row

        pm.rowLayout(nc=3, **opts)
        self.groupLst = pm.textScrollList(_uiName('groupLst'), numberOfRows=10, allowMultiSelection=True,
                          selectCommand=self.updateActiveAOVs)
        self.availableLst = pm.textScrollList(_uiName('availableLst'), numberOfRows=10, allowMultiSelection=True,
                          doubleClickCommand=self.addAOVs)
        self.activeLst = pm.textScrollList(_uiName('activeLst'), numberOfRows=10, allowMultiSelection=True,
                          doubleClickCommand=self.removeAOVs)
        pm.setParent('..') # row

        pm.rowLayout(nc=3, **opts)
        pm.text(_uiName('empty'), label='')
        pm.button(_uiName('addBtn'), label='>>', command=self.addAOVs)
        pm.button(_uiName('remBtn'), label='<<', command=self.removeAOVs)
        pm.setParent('..') # row

        pm.setParent('..') # column

    def populate(self):
        '''
        update the contents of all scroll lists
        '''
        for nodeType in aovs.getAOVGroups():
            pm.textScrollList(self.groupLst, edit=True, append=nodeType, selectItem=nodeType)
        for nodeType in aovs.getNodesWithAOVs():
            # make sure we have at least one named aov
            if any([x for x in aovs.getNodeAOVs(nodeType) if x]):
                pm.textScrollList(self.groupLst, edit=True, append=nodeType, selectItem=nodeType)
        # populate available and active based on aovs provided by groups and nodes
        self.updateActiveAOVs()

    def addAOVs(self, *args):
        '''
        create the selected AOVs, and connect the new AOV nodes to their corresponding
        AOV attributes for any nodes in the scene.
        '''
        map = defaultdict(list)
        for nodeType in aovs.getNodesWithAOVs():
            for aovName, attr in aovs.getNodeAOVAttrs(nodeType):
                map[aovName].append((nodeType, attr))

        sel = pm.textScrollList(self.availableLst, query=True, selectItem=True)
        if sel:
            global _updating
            _updating = True
            try:
                for aov in sel:
                    aovNode = self.renderOptions.addAOV(aov)
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
        groups = pm.textScrollList(self.groupLst, query=True, selectItem=True)

        # first, find out what's selected, so we can reselect any persistent items
        sel = pm.textScrollList(self.availableLst, query=True, selectItem=True)

        # update the available list
        pm.textScrollList(self.availableLst, edit=True, removeAll=True)
        pm.textScrollList(self.activeLst, edit=True, removeAll=True)
        activeAOVs = self.renderOptions.getActiveAOVs()
        for group in groups:
            if group.startswith('<'):
                # it's an AOV group
                aovList = aovs.getGroupAOVs(group)
            else:
                aovList = [x for x in aovs.getNodeAOVs(group) if x]
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
        pm.rowLayout(nc=2, columnWidth2=[150, 150], columnAttach2=['right', 'both'])
        pm.text(label='')
        pm.button(label='Add Custom AOV', c=lambda *args: self.renderOptions.addAOV('custom'))
        pm.setParent('..') # rowLayout

        pm.separator()

    #    pm.text(_uiName('prefixLbl'), align='center', label='Prefix', parent=form)
    #    pm.textField(_uiName('prefixFld'), enable=False, text='', parent=form, changeCommand=Callback(setAOVPrefix, aovnode))

        self.browser.populate()

        # add all control rows
        for aovNode in self.renderOptions.getActiveAOVNodes():
            self.addControlRow(aovNode)

        self.id = pm.api.MNodeMessage.addAttributeChangedCallback(self.renderOptions.node.__apimobject__(), self.attrAddedCallback)
        pm.scriptJob(uiDeleted=[str(self.aovCol), lambda *args: pm.api.MNodeMessage.removeCallback(self.id)])

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
        shaderTemplate.AOVOptionMenuGrp.globalAOVListChanged()

    def attrAddedCallback(self, msg, plug, otherPlug, *args):
        #print "attr changed", msg, pm.Attribute(plug)
        #if msg & pm.api.MNodeMessage.kAttributeArrayAdded:
        if msg & pm.api.MNodeMessage.kConnectionMade or msg & pm.api.MNodeMessage.kConnectionBroken:
#            attr = pm.Attribute(plug)
#            otherAttr = pm.Attribute(otherPlug)
#            print "plug connected", attr, otherAttr
            self.waitingToRefresh = True
            pm.evalDeferred(self.refresh)

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

        # use evalDeferred to ensure that the update happens after the aov node attribute is set
        nameCtrl = pm.textField(editable=not lockName,
                                changeCommand=lambda *args: pm.evalDeferred(self.refresh))
        pm.connectControl(nameCtrl, aovNode.attr('name'))
        # must set editability after connecting control
        nameCtrl.setEditable(not lockName)

        # attrEnumOptionMenu does not work with multi-attrs and optionMenu does not work with connectControl,
        # so, unfortunately, our best option is attrEnumOptionMenuGrp
        pm.attrEnumOptionMenuGrp(attribute=str(aovNode.attr('type')), columnWidth2=[1, 60])
        pm.symbolButton(image="navButtonConnected.png",
                        command=lambda *args: pm.select(aovNode))
        # we need a reliable way to get the plug name
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
    pm.showWindow(win)
    return ed

def createArnoldAOVTab():
    parentForm = cmds.setParent(query=True)

    aovNode = aovs.getAOVNode()
    pm.columnLayout('enableAOVs', adjustableColumn=True)
    pm.attrControlGrp(attribute=aovNode.node.aovMode, label='Mode')

    ctrl = shaderTemplate.AOVOptionMenuGrp('aiOptions', 'displayAOV', label='Renderview AOV', allowCreation=False, includeBeauty=True)
    ctrl.attachToUI('defaultArnoldRenderOptions')

    pm.setParent(parentForm)

    #cmds.setUITemplate('attributeEditorTemplate', pushTemplate=True)
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

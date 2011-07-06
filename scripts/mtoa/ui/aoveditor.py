import pymel.core as pm
from mtoa.callbacks import *
import mtoa.aovs as aovs

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

class AOVBrowser(object):
    def __init__(self, aovNode=None):
        self.aovNode = aovs.getAOVNode() if aovNode is None else aovNode

        opts = {'columnWidth3' : [132]*3, 'columnAttach3' : ['both']*3, 'columnOffset3' : [0, 0, 6]}

        pm.columnLayout()

        pm.rowLayout(nc=3, **opts)
        pm.text(_uiName('groupsLbl'), align='center', label='AOV Groups')
        pm.text(_uiName('availableLbl'), align='center', label='Available AOVs')
        pm.text(_uiName('activeLbl'), align='center', label='Active AOVs')
        pm.setParent('..') # row

        pm.rowLayout(nc=3, **opts)
        self.groupLst = pm.textScrollList(_uiName('groupLst'), numberOfRows=10, allowMultiSelection=True,
                          selectCommand=self.groupFilterChanged)
        self.availableLst = pm.textScrollList(_uiName('availableLst'), numberOfRows=10, allowMultiSelection=True,
                          doubleClickCommand=self.addAOVs)
        self.activeLst = pm.textScrollList(_uiName('activeLst'), numberOfRows=10, allowMultiSelection=True,
                          doubleClickCommand=self.removeAOVs)
                            #,selectCommand=self.selectAOVs)
        pm.setParent('..') # row

        pm.rowLayout(nc=3, **opts)
        pm.text(_uiName('empty'), label='')
        pm.button(_uiName('addBtn'), label='>>', command=self.addAOVs)
        pm.button(_uiName('remBtn'), label='<<', command=self.removeAOVs)
        pm.setParent('..') # row
    
        pm.setParent('..') # column

    def update(self):
        for nodeType in aovs.getAOVGroups() + aovs.getNodesWithAOVs():
            pm.textScrollList(self.groupLst, edit=True, append=nodeType, selectItem=nodeType)
        # populate available and active based on aovs provided by groups and nodes
        self.groupFilterChanged()

    def addAOVs(self, *args):
        sel = pm.textScrollList(self.availableLst, query=True, selectItem=True)
        for aov in sel:
            index = self.aovNode.addAOV(aov)
            pm.textScrollList(self.activeLst, edit=True, append=aov)
            pm.textScrollList(self.availableLst, edit=True, removeItem=aov)
            # handled by attrAdded callback
            #self.addControlRow(self.aovListPlug[index], aov, lockName=True)

    def removeAOVs(self, *args):
        sel = pm.textScrollList(self.activeLst, query=True, selectItem=True)
        for aov in sel:
            self.removeAOV(aov)

    def groupFilterChanged(self):
        sel = pm.textScrollList(self.groupLst, query=True, selectItem=True)

        # update the available list
        pm.textScrollList(self.availableLst, edit=True, removeAll=True)
        pm.textScrollList(self.activeLst, edit=True, removeAll=True)
        activeAOVs = self.aovNode.getAOVs()

        for group in sel:
            if group.startswith('<'):
                # it's an AOV group
                aovList = aovs.getGroupAOVs(group)
            else:
                aovList = aovs.getNodeAOVs(group)
            for aovName in aovList:
                if aovName not in activeAOVs:
                    pm.textScrollList(self.availableLst, edit=True, append=aovName)
                else:
                    pm.textScrollList(self.activeLst, edit=True, append=aovName)

class ArnoldAOVEditor(object):

    def __init__(self, aovNode=None):
        self.aovControls = []
        self.aovNode = aovs.getAOVNode() if aovNode is None else aovNode

        self.aovListPlug = self.aovNode.node.aovs

        pm.columnLayout()

        pm.frameLayout(label='AOV Browser', width=WIDTH, collapsable=True, collapse=False)

        self.browser = AOVBrowser(self.aovNode)
        pm.setParent('..') # frame

        pm.frameLayout(label='Primary AOVs', width=WIDTH, collapsable=True, collapse=False)
        self.aovCol = pm.columnLayout()
        pm.rowLayout(nc=2, columnWidth2=[150, 150], columnAttach2=['right', 'both'])
        pm.text(label='')
        pm.button(label='Add Custom AOV', c=lambda *args: self.aovNode.addAOV('custom'))
        pm.setParent('..') # rowLayout

        pm.separator()

    #    pm.text(_uiName('prefixLbl'), align='center', label='Prefix', parent=form)
    #    pm.textField(_uiName('prefixFld'), enable=False, text='', parent=form, changeCommand=Callback(setAOVPrefix, aovnode))

        self.aovNode.makeContiguous()

        self.browser.update()

        # add all control rows 
        for index, aovName in self.aovNode.getAOVs(indices=True):
            self.addControlRow(self.aovListPlug[index], index)

        self.id = pm.api.MNodeMessage.addAttributeChangedCallback(self.aovNode.node.__apimobject__(), self.attrAddedCallback)
        pm.scriptJob(uiDeleted=[str(self.aovCol), lambda *args: pm.api.MNodeMessage.removeCallback(self.id)])

    #    pm.formLayout(form, edit=True,
    #
    #              attachForm     = [(_uiName('bmoLbl'),"top",2),
    #                               (_uiName('bmoLbl'),"left",2),
    #                               (_uiName('bmoChk'),"top",2),
    #                               (_uiName('bmoChk'),"right",2),
    #                               (_uiName('availableLbl'),"left",2),
    #                               (_uiName('activeLbl'),"right",2),
    #                               (self.availableLst,"left",2),
    #                               (self.activeLst,"right",2),
    #                               (_uiName('prefixLbl'),"left",2),
    #                               (_uiName('prefixLbl'),"bottom",2),
    #                               (_uiName('prefixFld'),"right",2),
    #                               (_uiName('prefixFld'),"bottom",2)],
    #
    #              attachPosition = [(_uiName('bmoLbl'),"right",2,50),
    #                               (_uiName('bmoChk'),"left",2,50),
    #                               (_uiName('availableLbl'),"right",10,50),
    #                               (_uiName('activeLbl'),"left",10,50),
    #                               (_uiName('addBtn'),"bottom",2,50),
    #                               (_uiName('addBtn'),"left",2,45),
    #                               (_uiName('addBtn'),"right",2,55),
    #                               (_uiName('remBtn'),"top",2,50),
    #                               (_uiName('remBtn'),"left",2,45),
    #                               (_uiName('remBtn'),"right",2,55),
    #                               (_uiName('prefixLbl'),"right",2,25),
    #                               (_uiName('prefixFld'),"left",2,25)],
    #
    #              attachNone     = [(_uiName('bmoLbl'),"bottom"),
    #                               (_uiName('availableLbl'),"bottom"),
    #                               (_uiName('bmoChk'),"bottom"),
    #                               (_uiName('activeLbl'),"bottom"),
    #                               (_uiName('addBtn'),"top"),
    #                               (_uiName('remBtn'),"bottom"),
    #                               (_uiName('prefixLbl'),"top"),
    #                               (_uiName('prefixFld'),"top")],
    #
    #              attachControl  = [(_uiName('availableLbl'),"top",2,_uiName('bmoLbl')),
    #                               (_uiName('activeLbl'),"top",2, _uiName('bmoChk')),
    #                               (self.availableLst,"top",2,_uiName('availableLbl')),
    #                               (self.availableLst,"right",2,_uiName('addBtn')),
    #                               (self.availableLst,"bottom",2,_uiName('prefixFld')),
    #                               (self.activeLst,"top",2,_uiName('activeLbl')),
    #                               (self.activeLst,"left",2,_uiName('addBtn')),
    #                               (self.activeLst,"bottom",2,_uiName('prefixFld'))])
    

    def attrAddedCallback(self, msg, plug, *args):
        #print "attr changed", msg, pm.Attribute(plug)
        if msg & pm.api.MNodeMessage.kAttributeArrayAdded:
            attr = pm.Attribute(plug)
            #print "array plug added", attr
            index = attr.index()
            #print "index", index, "numControls", len(self.aovControls)
            if index == len(self.aovControls):
                #print "adding row"
                self.addControlRow(attr, index)
        elif msg & pm.api.MNodeMessage.kAttributeArrayRemoved:
            attr = pm.Attribute(plug)
            #print "array plug removed", attr
            index = attr.index()
            #print "index", index, "numControls", len(self.aovControls)
            if index < len(self.aovControls):
                #print "removing row"
                self.removeControlRow()

    def addControlRow(self, plug, index=None, lockName=False):
        pm.setParent(self.aovCol)
        if index is None:
            index = plug.index()
        #print "add control", plug, index
        row = pm.rowLayout(numberOfColumns=4,
                                columnWidth4=[150, 150, 70, 20],
                                columnAttach4=['right', 'both', 'both', 'both'])

        enabledCtrl = pm.checkBox(label='')
        pm.connectControl(enabledCtrl, plug.attr('enabled'))

        nameCtrl = pm.textField(editable=not lockName)
        pm.connectControl(nameCtrl, plug.attr('name'))
        # must set editability after connecting control
        nameCtrl.setEditable(not lockName)

        # attrEnumOptionMenu does not work with multi-attrs and optionMenu does not work with connectControl,
        # so, unfortunately, our best option is attrEnumOptionMenuGrp
        pm.attrEnumOptionMenuGrp(attribute=str(plug.attr('type')), columnWidth2=[1, 60])
        # we need a reliable way to get the plug name
        pm.symbolButton(image="smallTrash.png",
                        command=lambda *args: self.removeAOV(index))
        cmds.setParent('..')
        self.aovControls.append(row)
        return row

    def removeControlRow(self):
        self.aovControls.pop(-1).delete()

    def removeAOV(self, aov):
        #print "removeAOV", `aov`
        if isinstance(aov, int):
            index = aov
            aov = self.aovNode.removeAOV(aov)
        else:
            index = self.aovNode.removeAOV(aov)
        if index is not None:
            pm.textScrollList(self.availableLst, edit=True, append=aov)
            pm.textScrollList(self.activeLst, edit=True, removeItem=aov)

#        pm.textField(_uiName('prefixFld'), edit=True, enable=False)

#    def setAOVPrefix(self):
#        sel = pm.textScrollList(self.activeLst, query=True, selectItem=True)
#        if sel is None:
#            sel = []
#    
#        if len(sel) != 1:
#            return
#    
#        aov = sel[0]
#    
#        n = pm.getAttr("%s.aovs"%node, size=True)
#        for i in range(0, n):
#            name = pm.getAttr('%s.aovs[%d].name'%(node, i))
#            if name == aov:
#                pm.setAttr("%s.aovs[%d].prefix"%(node, i), pm.textField(_uiName('prefixFld'), query=True, text=True), type='string')
#                break

    def selectAOVs(self):
        sel = pm.textScrollList(self.activeLst, query=True, selectItem=True)
        if not sel:
            return

        if len(sel) != 1:
            pm.textField(_uiName('prefixFld'), edit=True, enable=False)
            return
        else:
            pm.textField(_uiName('prefixFld'), edit=True, enable=True)
    
        aov = sel[0]
    
        n = pm.getAttr('%s.aovs' % self.aovNode.node, size=True,)
        for i in range(0, n):
            name = pm.getAttr('%s.aovs[%d].name'%(self.aovNode.node, i))
            if name == aov:
                prefix = pm.getAttr('%s.aovs[%d].prefix'%(self.aovNode.node, i))
                pm.textField(_uiName('prefixFld'), edit=True, text=prefix)
                break

def arnoldAOVEditor(*args):
    if pm.window(UI_NAME, exists=True):
        pm.deleteUI(UI_NAME)
    win = pm.window(UI_NAME, title='AOV setup', width=640, height=300)
    import time
    s = time.time()
    ArnoldAOVEditor()
    print time.time() - s
    pm.showWindow(win)

def createArnoldAOVTab():
    parentForm = cmds.setParent(query=True)

    aovNode = aovs.getAOVNode()
    pm.columnLayout('enableAOVs', adjustableColumn=True)
    pm.attrControlGrp(attribute=aovNode.node.mode, label='AOV Mode')
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

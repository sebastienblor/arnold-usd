import maya.OpenMaya as om
import mtoa.ui.ae.templates as templates
from mtoa.callbacks import Callback
import maya.cmds as cmds
import re
from mtoa.ui.ae.utils import AttrControlGrp

class AttributeListWindow(object):
    
    _attributes = {}
    
    def __init__(self, template, mode='add'):
        self.template = template
        self.win = "arnold_attribute_win"
        if cmds.window(self.win, exists=True):
            cmds.deleteUI(self.win)
    
        modeLabel = mode.capitalize()
        wintitle = "%s Override Attribute" % modeLabel
        cmds.window(self.win, title=wintitle,
                    sizeable=True,
                    resizeToFitChildren=False)
        form = cmds.formLayout('form')
        filterText = cmds.textField('alf_filter_text', height=20)
        self.filterText = filterText
        cmds.textField(self.filterText, edit=True, changeCommand=Callback(self.filterAttributes))
        txList = cmds.textScrollList('alf_attribute_list', ams=True)
        self.scrollList = txList
        if mode == 'add':
            cmd = self.addAttrAndHide
        else:
            cmd = self.removeAttrAndHide
        cmds.textScrollList(self.scrollList,
                            edit=True,
                            doubleClickCommand=Callback(cmd))

        self.handleAttributes(mode)
        
        if self._attributes :
            labels = self._attributes.keys()
            labels.sort()
            for attr in labels:
                cmds.textScrollList(self.scrollList, edit=True, append=attr)

        row = cmds.rowLayout(numberOfColumns=2, columnAlign2=("center", "center"))
        # cmds.button(width=100, label=modeLabel, c=lambda *args: self.addAttrAndHide())      

        cmds.button(width=100, label=modeLabel, command=Callback(cmd))
        cmds.button(width=100, label="Cancel", c=lambda *args: cmds.deleteUI(self.win, window=True))  
        cmds.setParent('..')
        cmds.setParent('..')
        
        cmds.formLayout(form, edit=True,
                attachForm=[(filterText, 'top', 5), (filterText, 'left', 5), (filterText, 'right', 5), (txList, 'left', 5), (txList, 'right', 5), (row, 'bottom', 5), (row, 'left', 5), (row, 'right', 5)],
                attachControl=[(txList, 'bottom', 5, row), (txList, 'top', 5, filterText)])

        cmds.showWindow(self.win)

    def handleAttributes(self, mode='add'):
        if mode == 'add':
            self._attributes = self.template.getCandidateAttributes()
            for attr in self.template.getExistingAttributes():
                self._attributes.pop(attr, None)
        else:
            self._attributes = self.template.getExistingAttributes()

        
    def filterAttributes(self):
        cmds.textScrollList(self.scrollList, edit=True, removeAll=True)        
        if self._attributes is None:
            return
        filterText = cmds.textField(self.filterText, query=True, text=True)
        labels = self._attributes.keys()
        labels.sort()
        if filterText == "":
            for attr in labels:
                cmds.textScrollList(self.scrollList, edit=True, append=attr)       
        else:
            for attr in labels:
                if re.search(filterText, attr) is not None:
                    cmds.textScrollList(self.scrollList, edit=True, append=attr)

    def addAttrAndHide(self):
        
        attrLabels = cmds.textScrollList(self.scrollList, q=True, si=True)
        
        if attrLabels:
            self.template.addAttr([self._attributes[x] for x in attrLabels])
        self.handleAttributes('add')
        self.filterAttributes()

    def removeAttrAndHide(self):
        attrLabels = cmds.textScrollList(self.scrollList, q=True, si=True)
        if attrLabels:
            self.template.removeAttr([self._attributes[x] for x in attrLabels])
        self.handleAttributes('remove')
        self.filterAttributes()


class ObjectSetTemplate(templates.AttributeTemplate):
        
    def setup(self):
        self.baseAttrs = cmds.listAttr(self.nodeName)
        self._controls = []
        self.addControl("aiOverride")
        self.addCustom("aiOverride", self.createAttributesButtons, self.updateAttributesButtons)
        self.addSeparator()
        # FIXME: need a proper listing of override attributes
        self.beginLayout('Override Attributes', collapse=False)
        self.addCustom('addlnAttribute', self.addExtraAttributes, self.updateExtraAttributes)
        self.endLayout()

                
    def update(self):
        pass
        pass
            
    def createAttributesButtons(self, attr):
        # cmds.setUITemplate('attributeEditorTemplate', pushTemplate=True)
        cmds.rowLayout(numberOfColumns=3,
                       columnWidth3=(140, 80, 80),
                       columnAttach3=("both", "both", "both"),
                       columnAlign3=("center", "center", "center"),
                       columnOffset3=(2, 2, 2))
        cmds.text(label="")
        cmds.button('attr_add_button', label="Add", c=lambda *args: AttributeListWindow(self, mode='add'))
        cmds.button('attr_remove_button', label="Remove", c=lambda *args: AttributeListWindow(self, mode='remove'))
        # cmds.button('attr_remove_button', label="Remove", c=Callback(self.removeAttrWin))
        # cmds.setUITemplate('attributeEditorTemplate', popTemplate=True)
        
    def updateAttributesButtons(self, attr):
        pass
                
    def addExtraAttributes(self, attr):
        self.overrideAttributeLayout = cmds.columnLayout(adjustableColumn=True)

    def updateExtraAttributes(self, attr):

        for ctrl in self._controls:
            cmds.deleteUI(ctrl)
        del self._controls[:]
        
        arnoldAttrs = cmds.listAttr(self.nodeName, userDefined=True, write=True, ct="arnold")
        if not arnoldAttrs:
            return 

        for attr in arnoldAttrs:
            if attr not in self.baseAttrs:
                cmds.setParent(self.overrideAttributeLayout)
                shortName = cmds.attributeQuery(attr, node=self.nodeName, sn=True)
                kwargs = {}
                kwargs['label'] = attr
                kwargs['attribute'] = self.nodeName + '.' + shortName
                control = AttrControlGrp(**kwargs)
                self._controls.append(control.control)
                cmds.setParent('..')
    
    def getCandidateAttributes(self):
        attributeList = cmds.arnoldListAttributes(self.nodeName)
        candidates = {}
        if attributeList:
            for attrName in attributeList:
                try:
                    attrShortName = attrName[attrName.find('.')+1:]
                    candidates[attrShortName] = attrName
                except:
                    pass
        return candidates

    def getExistingAttributes(self):
        existing = {}
        
        attrs = cmds.listAttr(self.nodeName, userDefined=True) or []
        for attr in attrs:
            if not attr in existing:
                existing[attr] = '{}.{}'.format(self.nodeName, attr)

        return existing
        
    def addAttr(self, attrs):    
        for attr in attrs:
            # must add from top parent
            attrSplit = attr.split('.')
            if len(attrSplit) > 1:
                node = attrSplit[0]
                self._doAdd(node, attrSplit[1], None)
                
       
    def _doAdd(self, srcNode, attrName, parentName):
        srcNode = str(srcNode)
        args                     = {}
        if parentName:
            args['parent']              = parentName  
        args['longName']         = cmds.attributeQuery(attrName, node=srcNode, longName=True)
        args['shortName']        = cmds.attributeQuery(attrName, node=srcNode, shortName=True)
        try:
            args['niceName']         = cmds.attributeQuery(attrName, node=srcNode, niceName=True)
        except:
            pass
        children                 = cmds.attributeQuery(attrName, node=srcNode, listChildren=True)
        if children:
            args['numberOfChildren']    = len(children)
        else:
            children             = []
            try:
                defaultValue            = cmds.attributeQuery(attrName, node=srcNode, listDefault=True)
                args['defaultValue']    = defaultValue[0]
            except:
                pass           
        
        attributeType = cmds.getAttr("%s.%s" % (srcNode, attrName), type=True)
        # silly 2012 bug, returns float3 as type on surfaceShader attribute
        if attributeType == 'string' or attributeType == 'float2':
            #args['attributeType'] = None  #shouldn't set this (#3511)
            args['dataType'] = attributeType            
        else:
            args['attributeType'] = attributeType
            
        # args['dataType']       = None
        try:
            args['category']         = cmds.attributeQuery(attrName, node=srcNode, categories=True)
        except:
            pass         
        isEnum                   = cmds.attributeQuery(attrName, node=srcNode, enum=True)
        if isEnum:
            try:
                listEnum                = cmds.attributeQuery(attrName, node=srcNode, listEnum=True)
                args['enumName']        = listEnum[0]
            except:
                pass
        isMulti                  = cmds.attributeQuery(attrName, node=srcNode, multi=True)
        if isMulti:
            args['multi']               = True
            args['indexMatters']        = cmds.attributeQuery(attrName, node=srcNode, indexMatters=True)
        hasMin                   = cmds.attributeQuery(attrName, node=srcNode, minExists=True)
        if hasMin:
            try:
                minValue                = cmds.attributeQuery(attrName, node=srcNode, minimum=True)
                args['minValue']        = minValue[0]
            except:
                pass        
        hasMax                   = cmds.attributeQuery(attrName, node=srcNode, maxExists=True)
        if hasMax:    
            try:
                maxValue               = cmds.attributeQuery(attrName, node=srcNode, maximum=True)
                args['maxValue']       = maxValue[0]
            except:
                pass
        hasSoftMin                   = cmds.attributeQuery(attrName, node=srcNode, softMinExists=True)
        if hasSoftMin:
            try:
                softMinValue           = cmds.attributeQuery(attrName, node=srcNode, softMin=True)
                args['softMinValue']   = softMinValue[0]
            except:
                pass
        hasSoftMax                   = cmds.attributeQuery(attrName, node=srcNode, softMaxExists=True)
        if hasSoftMax:
            try:
                softMaxValue           = cmds.attributeQuery(attrName, node=srcNode, softMax=True)
                args['softMaxValue']   = softMaxValue[0]
            except:
                pass
        try:                                              
            args['usedAsColor']      = cmds.attributeQuery(attrName, node=srcNode, usedAsColor=True)
        except:
            pass            
        try:
            args['usedAsFilename']   = cmds.attributeQuery(attrName, node=srcNode, usedAsFilename=True)
        except:
            pass           
        args['keyable']          = cmds.attributeQuery(attrName, node=srcNode, keyable=True) 
        
        
        cmds.addAttr(self.nodeName, **args)
        for child in children:
            self._doAdd(srcNode, child, args['longName'])       

              
    def removeAttr(self, attrs):
        for attr in attrs:
            attrSplit = attr.split('.')
            if len(attrSplit) > 1:
                cmds.deleteAttr('{}.{}'.format(attrSplit[0], attrSplit[1]))
            
            
templates.registerAETemplate(ObjectSetTemplate, "objectSet")


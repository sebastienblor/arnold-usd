import pymel.core as pm
import maya.OpenMaya as om
import mtoa.ui.ae.templates as templates
from mtoa.callbacks import Callback

class AttributeListWindow(object):
    
    _attributes = {}
    
    def __init__(self, template, mode='add'):
        self.template = template
        self.win = "arnold_attribute_win"
        if pm.window(self.win, exists=True):
            pm.deleteUI(self.win)
    
        modeLabel = mode.capitalize()
        wintitle = "%s Override Attribute" % modeLabel
        pm.window(self.win, title=wintitle,
                    sizeable=True,
                    resizeToFitChildren=False)
        #pm.windowPref(removeAll=True)
        form = pm.formLayout('form')    
        if pm.mel.getApplicationVersionAsFloat() < 2013:
            list = pm.textScrollList('alf_attribute_list', nr=10, ams=True)
        else:
            list = pm.textScrollList('alf_attribute_list', ams=True)
        self.scrollList = list
        if mode == 'add':
            cmd = self.addAttrAndHide
        else:
            cmd = self.removeAttrAndHide
        pm.textScrollList(self.scrollList,
                            edit=True,
                            doubleClickCommand=Callback(cmd))

        if mode == 'add':
            self._attributes = self.template.getCandidateAttributes()
            for attr in self.template.getExistingAttributes():
                self._attributes.pop(attr, None)
        else:
            self._attributes = self.template.getExistingAttributes()
            
        if self._attributes :
            labels = self._attributes.keys()
            labels.sort()
            for attr in labels:
                pm.textScrollList(self.scrollList, edit=True, append=attr)

        row = pm.rowLayout(numberOfColumns=2, columnAlign2=("center", "center"))
        # pm.button(width=100, label=modeLabel, c=lambda *args: self.addAttrAndHide())        
        pm.button(width=100, label=modeLabel, command=Callback(cmd))
        pm.button(width=100, label="Cancel", c=lambda *args: pm.deleteUI(self.win, window=True))  
        pm.setParent('..')
        pm.setParent('..')
        
        pm.formLayout(form, edit=True,
                attachForm=[(list, 'top', 5), (list, 'left', 5), (list, 'right', 5), (row, 'bottom', 5), (row, 'left', 5), (row, 'right', 5)],
                attachControl=[(list, 'bottom', 5, row)])

        pm.showWindow(self.win)

    def addAttrAndHide(self):
        pm.window(self.win, edit=True, visible=False)
        attrLabels = pm.textScrollList(self.scrollList, q=True, si=True)
        if attrLabels:
            self.template.addAttr([self._attributes[x] for x in attrLabels])

    def removeAttrAndHide(self):
        pm.window(self.win, edit=True, visible=False)
        attrLabels = pm.textScrollList(self.scrollList, q=True, si=True)
        if attrLabels:
            self.template.removeAttr([self._attributes[x] for x in attrLabels])


class ObjectSetTemplate(templates.AttributeEditorTemplate):
        
    def setup(self):
        self.addControl("aiOverride")
        # print "ObjectSetTemplate setup %s" % self.nodeName
        # print self.attributeCandidates()
        self.addCustom("aiOverride", self.createAttributesButtons, self.updateAttributesButtons)
        self.addSeparator()
        # FIXME: need a proper listing of override attributes
        self.addExtraControls()
                
    def update(self):
        pass
        # print "ObjectSetTemplate update %s" % self.nodeName
        # FIXME seems never to get called
        pass
            
    def createAttributesButtons(self, attr):
        # print "ObjectSetTemplate Create Buttons %r for %r" % (self.nodeName, attr)
        self._doUpdate(attr)
        # print "ObjectSetTemplate Created Buttons %r for %r" % (self.nodeName, attr)
        pm.setUITemplate('attributeEditorTemplate', pushTemplate=True)
        pm.rowLayout(numberOfColumns=3,
                       columnWidth3=(140, 80, 80),
                       columnAttach3=("both", "both", "both"),
                       columnAlign3=("center", "center", "center"),
                       columnOffset3=(2, 2, 2))
        pm.text(label="")
        pm.button('attr_add_button', label="Add", c=lambda *args: AttributeListWindow(self, mode='add'))
        pm.button('attr_remove_button', label="Remove", c=lambda *args: AttributeListWindow(self, mode='remove'))
        # pm.button('attr_remove_button', label="Remove", c=Callback(self.removeAttrWin))
        pm.setUITemplate('attributeEditorTemplate', popTemplate=True)
        
    def updateAttributesButtons(self, attr):
        # print "ObjectSetTemplate Update Buttons %r for %r" % (self.nodeName, attr)
        self._doUpdate(attr)
        # print "ObjectSetTemplate Updated Buttons %r for %r" % (self.nodeName, attr)
   
    def getCandidateAttributes(self):
        candidates = {}
        elts = pm.sets(self.nodeName, query=True)
        for elt in elts :
            dag = pm.listRelatives(elt, allDescendents=True)
            dag.append(elt)
            for node in dag:                
                attrs = node.listAttr(write=True, visible=True)
                for attr in attrs :
                    name = attr.longName(fullPath=True)
                    # Too restrictive, missing useful attributes like "primary visibility" on shapes
                    # or "Out Matte Opacity" on surface shader for instance.
                    # Why it's not marked as render source or affects appearance beats me but point
                    # is these distinctions don't seem reliable enough in Maya yet to filter useful attrs
                    # fnAttr = om.MFnAttribute(attr.__apimobject__())
                    # if (fnAttr.isRenderSource() or fnAttr.affectsAppearance() or fnAttr.hasCategory('arnold')) :                       
                    if not name in candidates :
                        candidates[name] = attr
        return candidates

    def getExistingAttributes(self):
        existing = {}
        attrs = pm.PyNode(self.nodeName).listAttr(userDefined=True)
        for attr in attrs :
            name = attr.longName(fullPath=True)
            if not name in existing :
                existing[name] = attr
        return existing   
        
    def addAttr(self, attrs):    
        # print "addAttr %r" % attrs
        for attr in attrs:
            # must add from top parent
            parent = attr.getParent(-1, True)
            self._doAdd(parent.node(), parent.attrName(longName=True), None)
       
    def _doAdd(self, srcNode, attrName, parentName):
        dstNode = pm.PyNode(self.nodeName)
        # print "Create %s.%s by copying from %s.%s" % (dstNode, attrName, srcNode, attrName)
        # print "Get %s.%s info" % (srcNode, attrName)
        args                     = {}
        if parentName:
            args['parent']              = parentName  
        args['longName']         = pm.attributeQuery(attrName, node=srcNode, longName=True)
        args['shortName']        = pm.attributeQuery(attrName, node=srcNode, shortName=True)
        try:
            args['niceName']         = pm.attributeQuery(attrName, node=srcNode, niceName=True)
        except:
            pass
        children                 = pm.attributeQuery(attrName, node=srcNode, listChildren=True)
        if children:
            args['numberOfChildren']    = len(children)
        else:
            children             = []
            try:
                defaultValue            = pm.attributeQuery(attrName, node=srcNode, listDefault=True)
                args['defaultValue']    = defaultValue[0]
            except:
                pass           
        if pm.mel.getApplicationVersionAsFloat() < 2013:
            args['attributeType']    = pm.getAttr("%s.%s" % (srcNode, attrName), type=True)
            # silly 2012 bug, returns float3 as type on surfaceShader attribute
            if (args['attributeType'] == 'float3' and not children):
                args['attributeType'] = 'typed'
        else:
            args['attributeType']    = pm.attributeQuery(attrName, node=srcNode, attributeType=True)
        # args['dataType']       = None
        try:
            args['category']         = pm.attributeQuery(attrName, node=srcNode, categories=True)
        except:
            pass         
        isEnum                   = pm.attributeQuery(attrName, node=srcNode, enum=True)
        if isEnum:
            try:
                listEnum                = pm.attributeQuery(attrName, node=srcNode, listEnum=True)
                args['enumName']        = listEnum[0]
            except:
                pass
        isMulti                  = pm.attributeQuery(attrName, node=srcNode, multi=True)
        if isMulti:
            args['multi']               = True
            args['indexMatters']        = pm.attributeQuery(attrName, node=srcNode, indexMatters=True)
        hasMin                   = pm.attributeQuery(attrName, node=srcNode, minExists=True)
        if hasMin:
            try:
                minValue                = pm.attributeQuery(attrName, node=srcNode, minimum=True)
                args['minValue']        = minValue[0]
            except:
                pass        
        hasMax                   = pm.attributeQuery(attrName, node=srcNode, maxExists=True)
        if hasMax:    
            try:
                maxValue               = pm.attributeQuery(attrName, node=srcNode, maximum=True)
                args['maxValue']       = maxValue[0]
            except:
                pass
        hasSoftMin                   = pm.attributeQuery(attrName, node=srcNode, softMinExists=True)
        if hasSoftMin:
            try:
                softMinValue           = pm.attributeQuery(attrName, node=srcNode, softMin=True)
                args['softMinValue']   = softMinValue[0]
            except:
                pass
        hasSoftMax                   = pm.attributeQuery(attrName, node=srcNode, softMaxExists=True)
        if hasSoftMax:
            try:
                softMaxValue           = pm.attributeQuery(attrName, node=srcNode, softMax=True)
                args['softMaxValue']   = softMaxValue[0]
            except:
                pass
        try:                                              
            args['usedAsColor']      = pm.attributeQuery(attrName, node=srcNode, usedAsColor=True)
        except:
            pass            
        try:
            args['usedAsFilename']   = pm.attributeQuery(attrName, node=srcNode, usedAsFilename=True)
        except:
            pass           
        args['keyable']          = pm.attributeQuery(attrName, node=srcNode, keyable=True) 
        # connectable            = pm.attributeQuery(attrName, node=srcNode, connectable=True)     
        
        # print "Add %s.%s with options: %s" % (dstNode, attrName, args)
        pm.addAttr(dstNode, **args)
        for child in children:
            self._doAdd(srcNode, child, args['longName'])       

              
    def removeAttr(self, attrs):
        # print "removeAttr %r" % attrs
        for attr in attrs:
            # Can only delete top parent of compound / multi attributes
            parent = attr.getParent(-1, True)
            # print "remove %r will need to remove %r" % (attr, parent)
            parent.delete()
            
            
templates.registerAETemplate(ObjectSetTemplate, "objectSet")


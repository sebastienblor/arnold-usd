import pymel.core as pm
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
        pm.columnLayout(adjustableColumn=True,
                          columnOffset=("both", 10),
                          #columnAttach=('both',1),
                          rowSpacing=10)
    
        self.scrollList = pm.textScrollList('alf_attribute_list', nr=30, ams=True)
        if mode == 'add':
            cmd = self.addAttrAndHide
        else:
            cmd = self.removeAttrAndHide
        pm.textScrollList(self.scrollList,
                            edit=True,
                            doubleClickCommand=Callback(cmd))

        if mode == 'add':
            self._attributes = self.getCandidateAttributes()
            for attr in self.getExistingAttributes():
                self._attributes.pop(attr, None)
        else:
            self._attributes = self.getExistingAttributes()
            
        if self._attributes :
            labels = self._attributes.keys()
            labels.sort()
            for attr in labels:
                pm.textScrollList(self.scrollList, edit=True, append=attr)

        pm.rowLayout(numberOfColumns=2, columnAlign2=("center", "center"))
        # pm.button(width=100, label=modeLabel, c=lambda *args: self.addAttrAndHide())        
        pm.button(width=100, label=modeLabel, command=Callback(cmd))
        pm.button(width=100, label="Cancel", c=lambda *args: pm.deleteUI(self.win, window=True))
    
        pm.setParent('..')
        pm.setParent('..')

        pm.showWindow(self.win)

    def getCandidateAttributes(self):
        candidates = {}
        elts = pm.sets(self.template.nodeName, query=True)
        for elt in elts :
            attrs = elt.listAttr(write=True, visible=True)
            for attr in attrs :
                name = attr.longName(fullPath=True)
                if not name in candidates :
                    candidates[name] = attr  
        return candidates

    def getExistingAttributes(self):
        existing = {}
        attrs = pm.PyNode(self.template.nodeName).listAttr(userDefined=True)
        for attr in attrs :
            name = attr.longName(fullPath=True)
            if not name in existing :
                existing[name] = attr
        return existing

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
    
    _overrides = {}
        
    def setup(self):
        self.addControl("aiOverride")
        print "setup %s" % self.nodeName
        # print self.attributeCandidates()
        self.addCustom("aiOverride", self.createAttributesButtons, self.updateAttributesButtons)
        self.addSeparator()
        #self.beginScrollLayout()
        # FIXME: need a proper listing of override attributes
        self.addExtraControls()
        #self.endScrollLayout()
                
    def update(self):
        print "update %s" % self.nodeName
        #if self.nodeName is None or not pm.objExists(self.nodeName):
        
    def createAttributesButtons(self, attr):
        print "Create Buttons %s" % attr
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
        print "Update Buttons %s" % attr
        pass
        
    def addAttr(self, attrs):    
        print "addAttr %r" % attrs
        destNode = pm.PyNode(self.nodeName)
        for attr in attrs:
            name = attr.name(includeNode=False, longName=True, fullAttrPath=True, fullDagPath=False, placeHolderIndices=True)
            print "add %s.%s by copying from %r" % (self.nodeName, name, attr)
            if attr.isCompound():
                print "Is compound"
            if attr.isArray():
                print "Is array"    
            if attr.isElement():
                print "Is element" 
            if attr.isMulti():
                print "Is multi"  
            sourceNode = attr.node()
            attrQueryName = attr.attrName(longName=True)
            ln = pm.attributeQuery(attrQueryName, node=sourceNode, longName=True)
            sn = pm.attributeQuery(attrQueryName, node=sourceNode, shortName=True)
            at = pm.attributeQuery(attrQueryName, node=sourceNode, attributeType=True)
            print "%s(%s) attribute type %s" % (ln, sn, at)  
            pm.addAttr(destNode, longName=ln, shortName=sn, attributeType=at)
            
    def removeAttr(self, attrs):
        print "removeAttr %r" % attrs
        for attr in attrs:
            print "remove %r" % attr
            attr.delete()
        
templates.registerAETemplate(ObjectSetTemplate, "objectSet")


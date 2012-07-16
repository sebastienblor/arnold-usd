import pymel.core as pm
import mtoa.aovs as aovs
import mtoa.ui.ae.aiSwatchDisplay as aiSwatchDisplay
from mtoa.ui.ae.utils import interToUI
import mtoa.ui.ae.templates as templates
import mtoa.core as core

def newAOVPrompt(default=''):
    result = pm.cmds.promptDialog(button=['Create', 'Cancel'],
                                  defaultButton='Create',
                                  cancelButton='Cancel',
                                  message='AOV Name',
                                  title='New AOV',
                                  text=default)
    if result == 'Create':
        core.createOptions()
        newAOV = pm.promptDialog(query=True, text=True)
        if str(newAOV).replace("_","").isalnum():
            return newAOV, aovs.AOVInterface().addAOV(newAOV)
        else:
            print "Invalid AOV Name"
            return None, None
    else:
        print "AOV creation canceled"
        return None, None

class AOVOptionMenuGrp(templates.AttributeTemplate):
    EMPTY_AOV_ITEM = "<None>"
    NEW_AOV_ITEM = "<Create New...>"
    UNKNOWN_AOV_ITEM = "%s (Inactive)"
    BEAUTY_ITEM = "RGBA"
    _instances = []
    
    def __init__(self, nodeType, label=None, allowCreation=True, includeBeauty=False, allowEmpty=True, allowDisable=False):
        super(AOVOptionMenuGrp, self).__init__(nodeType)
        aovs.addAOVChangedCallback(self.update)
        self.allowCreation = allowCreation
        self.includeBeauty = includeBeauty
        self.allowDisable = allowDisable
        self.allowEmpty = allowEmpty
        self._label = label
        self._defaultLabel = ""
        self._prevLabel = ""
    # TODO: convert to propertycache
    @property
    def menuName(self):
        return self.nodeType() + self.attr + '_aovOptionMenu'
    # TODO: convert to propertycache
    @property
    def label(self):
        return self._label if self._label else interToUI(self.attr)

    def changeCallback(self, nodeAttr, newAOV):
        """
        nodeAttr is the string plug that stores the name of the AOV
        """
        if newAOV == self.NEW_AOV_ITEM:
            # the the current value is inactive, fill this in as the starting text
            currVal = pm.getAttr(nodeAttr)
            if currVal not in self.activeNames:
                if currVal != self.EMPTY_AOV_ITEM and currVal != self.NEW_AOV_ITEM and currVal != self.BEAUTY_ITEM:
                    default = str(currVal).replace(" (Inactive)", "")
                else:
                    default = ''
            else:
                # TODO: reset menu to previous value?
                default = ''

            aovName, aovNode = newAOVPrompt(default)
            if aovNode is None:
                pm.setAttr(nodeAttr, self.NEW_AOV_ITEM)
                return
        elif newAOV == self.BEAUTY_ITEM:
            aovName = self.BEAUTY_ITEM
        elif newAOV == self.EMPTY_AOV_ITEM:
            aovName = ""
        else:
            aovName = newAOV
        pm.setAttr(nodeAttr, aovName)

    def updateMenu(self, nodeAttr):
        self.clear()
        currVal = str(pm.getAttr(nodeAttr))
        prevVal = str(self._prevLabel)
        defval = str(self._defaultLabel)
        currVal = currVal.replace(" (Inactive)", "")
        prevVal = prevVal.replace(" (Inactive)", "")
        if defval == "":
            self._defaultLabel = currVal
            defval = str(self._defaultLabel)
        
        if currVal == self.NEW_AOV_ITEM:
            currVal = prevVal
        activeNodes = dict(aovs.getAOVNodes(names=True))

        self.activeNames = sorted(activeNodes.keys())
        
        # If default value is not used, add it to the menu list
        if defval != currVal:
            if defval not in self.activeNames and defval != self.BEAUTY_ITEM:
                defval = self.UNKNOWN_AOV_ITEM % defval
                pm.menuItem(label=defval, parent=(self.menuName))
        
        
        if not currVal or currVal == self.EMPTY_AOV_ITEM:
            currVal = self.EMPTY_AOV_ITEM
        elif currVal not in self.activeNames and currVal != self.BEAUTY_ITEM:
            currVal = self.UNKNOWN_AOV_ITEM % currVal
            pm.menuItem(label=currVal, parent=(self.menuName))
            

        if self.includeBeauty:
            pm.menuItem(label=self.BEAUTY_ITEM, parent=(self.menuName))

        # add items
        for aov in self.activeNames:
            pm.menuItem(label=aov, parent=(self.menuName))

        if self.allowEmpty:
            pm.menuItem(label=self.EMPTY_AOV_ITEM, parent=(self.menuName))

        if self.allowCreation:
            pm.menuItem(label=self.NEW_AOV_ITEM, parent=(self.menuName))
        # set active
        pm.optionMenu(self.menuName, edit=True, value=currVal)
        self._prevLabel = currVal

    def clear(self):
        for item in pm.optionMenu(self.menuName, query=True, itemListLong=True) or []:
            pm.deleteUI(item)

    def setup(self):
        nodeAttr = self.nodeAttr(self.attr)

        pm.setUITemplate(popTemplate=1)
        
        if self.allowDisable:
            kwargs = dict(nc=3,
                          columnWidth3=[145, 20, 200],
                          columnAttach3=['right', 'both', 'both'])
        else:
            kwargs = dict(nc=2,
                          columnWidth2=[145, 220],
                          columnAttach2=['right', 'both'])
        pm.rowLayout(**kwargs)
        pm.text(label=self.label)
        if self.allowDisable:
            pm.checkBox(label='')
        pm.optionMenu(self.menuName)
        pm.setParent('..')

        self.updateMenu(nodeAttr)
        menu = pm.optionMenu(self.menuName, edit=True,
                             changeCommand=lambda *args: self.changeCallback(nodeAttr, *args))

        # make sure the UI gets updated if the attribute changes while we have the AE open
        pm.scriptJob(parent=menu,
                     attributeChange=(nodeAttr, lambda: self.updateMenu(nodeAttr)))

    def update(self):
        if self.nodeName is None or not pm.objExists(self.nodeName) or not pm.control(self.menuName, exists=True):
            return
        nodeAttr = self.nodeAttr(self.attr)
        self.updateMenu(nodeAttr)
        menu = pm.optionMenu(self.menuName, edit=True,
                             changeCommand=lambda *args: self.changeCallback(nodeAttr, *args))
        pm.scriptJob(parent=menu, replacePrevious=True,
                     attributeChange=(nodeAttr, lambda: self.updateMenu(nodeAttr)))

class ShaderMixin(object):
    def bumpNew(self, attrName):
        pm.setUITemplate('attributeEditorTemplate', pst=True)
        pm.attrNavigationControlGrp('bumpControl', label="Bump Mapping", at=attrName)
        pm.setUITemplate(ppt=True)

    def bumpReplace(self, attrName):
        pm.attrNavigationControlGrp('bumpControl', edit=True, at=attrName)

    def addBumpLayout(self):
        self.beginLayout("Bump Mapping", collapse=True)
        self.addCustom("normalCamera", self.bumpNew, self.bumpReplace)
        self.endLayout() # End Bump Layout

    def addSwatch(self):
        self.addCustom("message", aiSwatchDisplay.aiSwatchDisplayNew, aiSwatchDisplay.aiSwatchDisplayReplace)

    def addAOVControl(self, attr):
        menu = AOVOptionMenuGrp(self.nodeType())
        self.addChildTemplate(attr, menu)

    def addAOVLayout(self):
        '''Add an aov control for each aov registered for this node type'''
        aovAttrs = aovs.getNodeGlobalAOVData(nodeType=self.nodeType())
        if aovAttrs:
            self.beginLayout("AOVs", collapse=True)
#            self.beginNoOptimize()
#            self.addControl('enableAOVs', label='Enable AOVs')
#            self.addControl('overrideAOVs', label='Override AOV Names')
#            self.endNoOptimize()
            dynamic = self.nodeType() not in set(pm.pluginInfo("mtoa", q=True, dependNode=True))
            for name, attr, type in aovAttrs:
                if dynamic:
                    attr = 'ai_' + attr
                self.addAOVControl(attr)
            self.endLayout()


class ShaderAETemplate(templates.AttributeTemplate, ShaderMixin):
    pass


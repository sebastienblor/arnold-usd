import maya.mel
from mtoa.ui.ae.operatorTemplate import OperatorAETemplate
import maya.cmds as cmds
import mtoa.ui.ae.utils as aeUtils

class AEaiMaterialxTemplate(OperatorAETemplate):

    def selectionReplace(self, attrName):
        aeUtils.attrTextFieldGrp('aiMaterialxSelection', edit=True, attribute=attrName)
        

    def selectionNew(self, attrName):
        
        aeUtils.attrTextFieldGrp('aiMaterialxSelection', label='Selection', attribute=attrName.format(self.nodeName))
        #cmds.button('aiMaterialxAddSelection',  label='Add Selected Nodes', command=lambda arg=None: self.addSelection())
                

    def addSelection(self):
        pass

    def setup(self):
    	self.beginScrollLayout()
        
        self.beginLayout('Operator', collapse=False)
        self.addControl("enable")
        self.addCustom('selection', self.selectionNew, self.selectionReplace)

#        self.addControl("selection")

        self.endLayout()
        self.beginLayout('MaterialX', collapse=False)
        self.addControl("mtlx")
        self.addControl("look")
        self.addSeparator()
        self.addControl("assign_materials")
        self.addControl("assign_properties")
        self.addControl("assign_visibilities")
        self.endLayout()
        self.addOperatorInputs()

        maya.mel.eval('AEdependNodeTemplate '+self.nodeName)

        self.addExtraControls()

        self.endScrollLayout()


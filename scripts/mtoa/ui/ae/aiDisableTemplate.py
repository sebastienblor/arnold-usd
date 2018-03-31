import maya.mel
from mtoa.ui.ae.operatorTemplate import OperatorAETemplate
import maya.cmds as cmds
    
class AEaiDisableTemplate(OperatorAETemplate):

    
    def setup(self):
    	self.beginScrollLayout()
        
        self.beginLayout('Operator', collapse=False)
        self.addControl("enable")
        self.addControl("selection")
        self.endLayout()

        self.beginLayout('Enable/Disable', collapse=False)
        self.addControl("mode")
        self.addControl("shapes")
        self.addControl("lights")
        self.addControl("shaders")
        self.addControl("operators")
        self.endLayout()
        self.addOperatorInputs()
                
        maya.mel.eval('AEdependNodeTemplate '+self.nodeName)

        self.addExtraControls()

        self.endScrollLayout()


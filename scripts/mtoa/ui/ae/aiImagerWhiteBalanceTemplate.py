import maya.mel
from mtoa.ui.ae.templates import TranslatorControl
from mtoa.ui.ae.shaderTemplate import ShaderAETemplate
import maya.cmds as cmds
from mtoa.ui.ae.aiImagersBaseTemplate import ImagerBaseUI


class AEaiImagerWhiteBalanceTemplate(ShaderAETemplate):

    def createNewWhiteBalanceUI(self,nodeAttr):
        currentWidget = cmds.setParent(query=True)
        self.ui = ImagerWhiteBalanceUI(parent = currentWidget,nodeName = self.nodeName)
   

    def replaceWhiteBalanceUI(self,nodeAttr):
        currentWidget = cmds.setParent(query=True)
        if self.ui:
            for item in cmds.layout( currentWidget, query=True, childArray=True) :
                cmds.deleteUI(item)
        self.ui = ImagerWhiteBalanceUI(currentWidget, self.nodeName)

    def setup(self):
        self.beginScrollLayout()
        self.ui = None
        self.baseLayout = self.beginLayout("Main", collapse=False)
        self.addCustom('dummy', self.createNewWhiteBalanceUI, self.replaceWhiteBalanceUI)
        self.endLayout()
        maya.mel.eval('AEdependNodeTemplate '+self.nodeName)
        self.addExtraControls()
        self.endScrollLayout()

class ImagerWhiteBalanceUI(ImagerBaseUI):
    
    def updateParamsVisibility(self, nodeName):
        modeAttr = '%s.%s' % (nodeName, 'mode')
        modeValue = cmds.getAttr(modeAttr)
        cmds.control(self.illuminant, enable = (modeValue == 0 ), edit = True)
        cmds.control(self.temperature, enable = (modeValue == 1), edit = True)
        cmds.control(self.custom, enable = (modeValue == 2), edit = True)

    def __init__(self, parent=None, nodeName = None):
        super(ImagerWhiteBalanceUI, self).__init__(parent,nodeName)
        cmds.attrControlGrp(attribute=nodeName+'.mode', changeCommand=lambda *args:self.updateParamsVisibility(nodeName, *args), annotation='EWhite balance mode. (illuminant, temperature, custom)')
        self.illuminant = cmds.attrControlGrp(attribute = nodeName+'.illuminant', annotation='Temperature for black body mode.')
        self.temperature = cmds.attrControlGrp(attribute = nodeName+'.temperature', annotation='Standard illuminant name for illuminante mode.')
        self.custom = cmds.attrControlGrp(attribute = nodeName+'.custom', annotation='Custom white balance color.')
        self.updateParamsVisibility(nodeName)

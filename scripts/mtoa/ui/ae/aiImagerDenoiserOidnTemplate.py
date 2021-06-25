import maya.mel
from mtoa.ui.ae.templates import TranslatorControl
from mtoa.ui.ae.shaderTemplate import ShaderAETemplate
from mtoa.ui.ae.aiImagersBaseTemplate import ImagerBaseUI, registerImagerTemplate
import maya.cmds as cmds


class AEaiImagerDenoiserOidnTemplate(ShaderAETemplate):

    def setup(self):

        self.beginScrollLayout()
        currentWidget = cmds.setParent(query=True)
        self.ui = ImagerDenoiserOIDNUI(parent=currentWidget, nodeName=self.nodeName, template=self)

        maya.mel.eval('AEdependNodeTemplate '+self.nodeName)

        self.addExtraControls()
        self.endScrollLayout()


class ImagerDenoiserOIDNUI(ImagerBaseUI):
    def __init__(self, parent=None, nodeName = None, template=None):
        super(ImagerDenoiserOIDNUI, self).__init__(parent, nodeName,template)
    
    def doChangeFilter(self):
        if not cmds.objExists('defaultArnoldFilter'):
            return
        cmds.setAttr('defaultArnoldFilter.aiTranslator', 'box', type="string")
        self.filterMsgReplace('message')
    
    def filterMsgNew(self, nodeAttr):

        cmds.setUITemplate('attributeEditorTemplate', pst=True)
        cmds.rowColumnLayout( numberOfColumns=2, columnAlign=[(1, 'left'),(2, 'right')], columnAttach=[(1, 'left', 0), (2, 'both', 0)], columnWidth=[(1,145),(2,80)] )
        self.filterTextCtrl = cmds.text(label='   A box filter is required for   \n    better denoising results  ')
        self.filterButtonCtrl = cmds.button(label='Set Box Filter', al='right', w=85, h=25, command=lambda *args: self.doChangeFilter())
        cmds.setUITemplate(ppt=True)
        self.filterMsgReplace(nodeAttr)
        
    def filterMsgReplace(self, nodeAttr):
        filter = cmds.getAttr('defaultArnoldFilter.aiTranslator', asString=True)
        enabled = False if filter == 'box' else True
        cmds.text(self.filterTextCtrl, edit=True, enable = enabled)
        cmds.button(self.filterButtonCtrl, edit=True, enable = enabled)

    def setup(self):
        super(ImagerDenoiserOIDNUI, self).setup()
        self.beginLayout("Main", collapse=False)
        self.addControl('outputSuffix', annotation='')
        self.addControl('useFeatureAovs', annotation='')
        self.addSeparator()
        self.addCustom("message", self.filterMsgNew, self.filterMsgReplace)
        self.endLayout()


registerImagerTemplate("aiImagerDenoiserOidn", ImagerDenoiserOIDNUI)

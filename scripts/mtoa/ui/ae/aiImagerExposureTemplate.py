import maya.mel
from mtoa.ui.ae.templates import TranslatorControl
from mtoa.ui.ae.shaderTemplate import ShaderAETemplate
from mtoa.ui.qt import toQtObject
from mtoa.ui.qt import toMayaName
import maya.cmds as cmds
from mtoa.ui.qt.Qt import QtWidgets, QtCore, QtGui
from mtoa.ui.ae.aiImagersBaseTemplate import ImagerBaseUI

class AEaiImagerExposureTemplate(ShaderAETemplate):
    
    
    def createNewExposureUI(self,nodeAttr):
        currentWidget = cmds.setParent(query=True)
        self.ui = ImagerExposureUI(parent = currentWidget,nodeName = self.nodeName)
   

    def replaceExposureUI(self,nodeAttr):
        currentWidget = cmds.setParent(query=True)
        if self.ui:
            for item in cmds.layout( currentWidget, query=True, childArray=True) :
                cmds.deleteUI(item)
        self.ui = ImagerExposureUI(currentWidget, self.nodeName)

    
    def setup(self):
        self.beginScrollLayout()
        self.ui = None
        self.baseLayout = self.beginLayout("Main", collapse=False)
        self.addCustom('dummy', self.createNewExposureUI, self.replaceExposureUI)
        self.endLayout()
        maya.mel.eval('AEdependNodeTemplate '+self.nodeName)
        self.addExtraControls()
        self.endScrollLayout()

class ImagerExposureUI(ImagerBaseUI):
    def __init__(self, parent=None, nodeName = None):
        super(ImagerExposureUI, self).__init__(parent,nodeName)
        cmds.attrControlGrp( attribute=nodeName+'.exposure')



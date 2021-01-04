import maya.mel
from mtoa.ui.ae.templates import TranslatorControl
from mtoa.ui.ae.shaderTemplate import ShaderAETemplate
from mtoa.ui.qt import toQtObject
from mtoa.ui.qt import toMayaName
import maya.cmds as cmds
from mtoa.ui.qt.Qt import QtWidgets, QtCore, QtGui
from mtoa.ui.ae.aiImagersBaseTemplate import ImagerBaseUI, registerImagerTemplate


class AEaiImagerExposureTemplate(ShaderAETemplate):

    def setup(self):
        self.beginScrollLayout()
        self.baseLayout = self.beginLayout("Main", collapse=False)
        currentWidget = cmds.setParent(query=True)
        self.ui = ImagerExposureUI(parent=currentWidget, nodeName=self.nodeName, template=self)
        self.endLayout()
        maya.mel.eval('AEdependNodeTemplate '+self.nodeName)
        self.addExtraControls()
        self.endScrollLayout()


class ImagerExposureUI(ImagerBaseUI):
    def __init__(self, parent=None, nodeName=None, template=None):
        super(ImagerExposureUI, self).__init__(parent, nodeName, template)

    def setup(self):
        super(ImagerExposureUI, self).setup()
        self.addSeparator()
        self.addControl('exposure', label = "Exposure", annotation = "Exposure compensation amount in f-stops.")


registerImagerTemplate("aiImagerExposure", ImagerExposureUI)

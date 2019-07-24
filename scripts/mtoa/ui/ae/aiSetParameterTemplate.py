import maya.mel
import maya.cmds as cmds
from mtoa.ui.ae.operatorTemplate import OperatorAETemplate

import mtoa.melUtils as mu
import mtoa.core as core
from mtoa.ui.ae.utils import aeCallback
from mtoa.ui.ae.shaderTemplate import ShaderAETemplate
from mtoa.ui.procview.SetParameterWidgets import SetParametersPanel
from mtoa.ui.procview.ProceduralTransverser import ProceduralTransverser

from mtoa.ui.qt.Qt import QtCore
from mtoa.ui.qt.Qt import QtGui
from mtoa.ui.qt.Qt import QtWidgets
from mtoa.ui.qt.Qt import shiboken
from mtoa.ui.qt import toQtObject

import arnold as ai

CACHE_ATTR = 'ai_asscache'


class AEaiSetParameterTemplate(OperatorAETemplate):

    def __currentWidget(self, pySideType=QtWidgets.QWidget):
        """Cast and return the current widget."""
        # Get the current widget Maya name.
        currentWidgetName = cmds.setParent(query=True)
        return toQtObject(currentWidgetName, pySideType)

    def assignmentReplace(self, nodeAttr) :
        nodeName = nodeAttr.split('.')[0]
        self.currentNode = nodeName
        self.properties_panel.setNode(self.nodeName)

    def assignmentNew(self, nodeAttr):

        currentWidget = self.__currentWidget()
        self.currentNode = ''

        transverser = ProceduralTransverser()

        # now add the preperties panel
        self.properties_panel = SetParametersPanel(transverser, currentWidget)
        currentWidget.layout().addWidget(self.properties_panel)

        self.assignmentReplace(nodeAttr)

    def setup(self):
        self.beginScrollLayout()

        self.beginLayout('Operator', collapse=False)
        self.addControl("enable")
        self.addControl("selection")
        self.endLayout()
        self.beginLayout('Set Parameter', collapse=False)
        self.addCustom('assignment', self.assignmentNew, self.assignmentReplace)
        self.endLayout()
        self.addOperatorInputs()

        maya.mel.eval('AEdependNodeTemplate '+self.nodeName)

        self.addExtraControls()

        self.endScrollLayout()


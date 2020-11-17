import maya.mel
from mtoa.ui.qt import toQtObject
from mtoa.ui.qt import toMayaName
import maya.cmds as cmds
from mtoa.ui.qt.Qt import QtWidgets, QtCore, QtGui


class ImagerBaseUI(object):
    def __init__(self, parent=None, nodeName = None):
        self.parentLayout = cmds.rowColumnLayout( numberOfColumns=3, columnWidth=[(1, 100), (2, 100) , (3, 100) ])
        cmds.text(label = "")
        cmds.text(label = "Enable")
        checkbox = cmds.checkBox( label='', parent = self.parentLayout )
        cmds.connectControl( checkbox, '%s.enable' % nodeName )
        cmds.text(label = "")
        cmds.text(label = "Layer Selection")
        textField = cmds.textField()
        cmds.connectControl( textField, '%s.layerSelection' % nodeName )
        cmds.setParent('..')
    
    def getParentLayout(self):
        return self.parentLayout

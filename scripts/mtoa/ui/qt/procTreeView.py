from .Qt import OpenMayaUI
from .Qt import QtCore
from .Qt import QtGui
from .Qt import QtWidgets
from .itemStyle import ItemStyle
from .utils import dpiScale
from qt.treeView import BaseTreeView, BaseModel, BaseItem


class ProceduralTreeView(BaseTreeView):

    def __init__(self, parent=None):

        super(ProceduralTreeView, self).__init__(parent)

        model = ProceduralModel(self)
        self.setModel(model)

    def setCurrentNode(self, node):
        """Clear the widget and generate the view of the new node.
        Called when node is replaced in AE"""
        model = self.model()
        model.setCurrentNode(node)
        model.refresh()


class ProceduralModel(object):
    """docstring for ProceduralModel"""
    def __init__(self, treeView, parent=None):
        self.currentNode = None

        super(ProceduralModel, self).__init__(treeView, parent)

    def refresh(self):
        if not self.currentNode or not cmds.objExists(self.currentNode):
            return

    def setCurrentNode(self, node):
        """Save the node to use in future."""
        self.currentNode = node

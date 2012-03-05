import maya.cmds as cmds
from mtoa.ui.ae.customShapeAttributes import commonShapeAttributes, renderStatsAttributes
from mtoa.ui.ae.templates import registerUI

@registerUI("apiMesh")
def builtin_apiShape(ui):
    renderStatsAttributes(ui)
    commonShapeAttributes(ui)


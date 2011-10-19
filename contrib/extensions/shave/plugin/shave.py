import maya.cmds as cmds
from mtoa.ui.ae.shapeTemplate import translatorUI

@translatorUI("shaveHair", "shave")
def shaveHair(ui):
    ui.renderStatsAttributes()
    ui.commonShapeAttributes()
    ui.addSeparator()

    ui.addControl("aiOverrideHair")
    ui.addControl("aiHairShader")

    ui.addSeparator()

    ui.addControl("aiMinPixelWidth")
    ui.addControl("aiMode")

from __future__ import print_function
import maya.cmds as cmds
import mtoa
defaultFolder = ""


class MtoABatchRenderOptions(object):
    window = None

    def __new__(cls, *args, **kwargs):
        if '_instance' not in vars(cls):
            cls._instance = super(MtoABatchRenderOptions, cls).__new__(cls, *args, **kwargs)
        return cls._instance

    def __init__(self):
        if self.window is None:
            self.window = 'MtoABatchRenderOptions'

    def doCancel(self):
        cmds.deleteUI(self.window)
        return True

    def doBatchRender(self):
        cmds.BatchRender()
        cmds.deleteUI(self.window)
        return True

    def create(self):

        if cmds.window(self.window, exists=True):
            cmds.deleteUI(self.window)

        winTitle = "Arnold Batch Render Options"

        self.window = cmds.window(self.window, widthHeight=(460, 160), title=winTitle)
        self.createUI()

        cmds.setParent(menu=True)
        cmds.showWindow(self.window)

        try:
            initPos = cmds.windowPref( self.window, query=True, topLeftCorner=True)
            if initPos[0] < 0:
                initPos[0] = 0
            if initPos[1] < 0:
                initPos[1] = 0
            cmds.windowPref(self.window, edit=True, topLeftCorner=initPos)
        except Exception as e:
            pass

    def createUI(self):
        self.mainLayout = cmds.formLayout()
        text = cmds.text(label='Arnold does not have any specific batch render options \nUse the render settings to change options for batch rendering')
        renderBtn = cmds.button(label='Batch Render', al='right', w=85, h=25, command=lambda *args: self.doBatchRender())
        cancelBtn = cmds.button(label='Cancel', al='right', w=85, h=25, command=lambda *args: self.doCancel())

        cmds.formLayout(self.mainLayout, edit=True,
                        attachForm=[
                            (text, 'top', 5),
                            (text, 'left', 5),
                            (text, 'right', 5),
                            (renderBtn, 'left', 5),
                            (cancelBtn, 'right', 5),
                            (renderBtn, 'bottom', 5),
                            (cancelBtn, 'bottom', 5),],
                        attachPosition=[
                            (renderBtn, 'right', 5, 50),
                            (cancelBtn, 'left', 5, 50)],
                        attachNone=[
                            (renderBtn, 'top'),
                            (cancelBtn, 'top')]
                        )

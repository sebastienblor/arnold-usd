import maya.cmds as cmds

def unregisterArnoldRenderer():
    cmds.deleteUI('ArnoldMenu', menu=True)
    cmds.renderer('arnold', unregisterRenderer=True)



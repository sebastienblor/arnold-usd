import maya.cmds as cmds

def unregisterArnoldRenderer():
    cmds.renderer('arnold', unregisterRenderer=True)



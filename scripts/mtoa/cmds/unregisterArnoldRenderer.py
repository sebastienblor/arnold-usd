import maya.cmds as cmds
import pymel.versions as versions

def unregisterArnoldRenderer():
    maya_version = versions.shortName()
    if (maya_version >= '2013') :
        cmds.callbacks(clearCallbacks=True, owner='arnold')
    cmds.deleteUI('ArnoldMenu', menu=True)
    cmds.renderer('arnold', unregisterRenderer=True)



import maya.cmds as cmds
import maya.mel as mel

def arnoldRender(width, height, doShadows, doGlowPass, camera, options):
    # Make sure the ArnoldRenderOptions node exists
    cmds.createNode('ArnoldRenderOptions', skipSelect=True, shared=True, name='defaultArnoldRenderOptions')
    cmds.arnoldRender(cam=camera, w=width, h=height)

def arnoldBatchRender(option):
    cmds.arnoldRender(batch=option)
    #cmd = 'arnoldRender -batch %s'%option
    #mel.eval(cmd)

def arnoldIprStart(editor, resolutionX, resolutionY, camera):
    # Make sure the ArnoldRenderOptions node exists
    cmds.createNode('ArnoldRenderOptions', skipSelect=True, shared=True, name='defaultArnoldRenderOptions')
    cmds.arnoldIpr(cam=camera, w=resolutionX, h=resolutionY, mode='start')

def arnoldIprStop():
    cmds.arnoldIpr(mode='stop')

def arnoldIprIsRunning():
    return cmds.arnoldIpr()

def arnoldIprRender(width, height, doShadows, doGlowPass, camera):
    cmds.arnoldIpr(cam=camera, w=width, h=height, mode=render)

def arnoldIprRefresh():
    cmds.arnoldIpr(mode='refresh')

def arnoldIprPause(editor, pause):
    if pause:
        cmds.arnoldIpr(mode='pause')
    else:
        cmds.arnoldIpr(mode='unpause')

def arnoldIprChangeRegion(renderPanel):
    cmds.arnoldIpr(mode='region')

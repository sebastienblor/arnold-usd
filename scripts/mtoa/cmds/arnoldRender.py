import maya.cmds as cmds
import maya.mel as mel
import mtoa.core as core
import platform, os
import mtoa.utils as mutils
_maya_version = mutils.getMayaVersion()

def arnoldRender(width, height, doShadows, doGlowPass, camera, options):
    # Make sure the aiOptions node exists
    core.createOptions()
    cmds.arnoldRender(cam=camera, w=width, h=height) 

def arnoldSequenceRender(width, height, camera, saveToRenderView):
    # Make sure the aiOptions node exists
    core.createOptions()
    if len(camera) > 0:
        cmds.arnoldRender(seq="", w=width, h=height, cam=camera, srv=saveToRenderView)
    else:
        cmds.arnoldRender(seq="", w=width, h=height, srv=saveToRenderView)

def arnoldBatchRenderOptionsString():    
    origFileName = cmds.file(q=True, sn=True)
    try:
        port = core.MTOA_GLOBALS['COMMAND_PORT']
        return ' -r arnold -ai:ofn \\"' + origFileName + '\\" -ai:port %i ' % port
    except:
        return ' -r arnold -ai:ofn \\"' + origFileName + '\\" '

def arnoldBatchRender(option):
    # Make sure the aiOptions node exists
    core.createOptions()
    # Parse option string
    kwargs = {}
    options = option.split(" ")
    i, n = 0, len(options)
    if cmds.objExists('arnoldBatchNode.mtoaCommandPort'):
        kwargs['port'] = cmds.getAttr('arnoldBatchNode.mtoaCommandPort')
    if cmds.objExists('arnoldBatchNode.mtoaOrigFileName'):
        kwargs['ofn'] = cmds.getAttr('arnoldBatchNode.mtoaOrigFileName')
    while i < n:
        if options[i] in ["-w", "-width"]:
            i += 1
            if i >= n:
                break
            kwargs["width"] = int(options[i])
        elif options[i] in ["-h", "-height"]:
            i += 1
            if i >= n:
                break
            kwargs["height"] = int(options[i])
        elif options[i] in ["-cam", "-camera"]:
            i += 1
            if i >= n:
                break
            kwargs["camera"] = options[i]
        elif options[i] in ["-seq"]:
            i += 1
            seqoption = options[i]
            if seqoption and seqoption[0] in["\"", "'"]:
                while options[i][-1] not in ["\"", "'"]:
                    i += 1
                    seqoption += " {}".format(options[i])
            if i >= n:
                break
            kwargs["seq"] = seqoption.strip("\"'")
        i += 1
    try:
        cmds.arnoldRender(batch=True, **kwargs)
    except RuntimeError, err:
        print err

        # need to raise the error otherwise it's not caught by maya (#3405)
        if _maya_version >= 2019: # doing it only on mainline for testing
            raise
        
def arnoldBatchStop():
    mel.eval('batchRender')

def arnoldIprStart(editor, resolutionX, resolutionY, camera):
    # Make sure the aiOptions node exists
    core.createOptions()
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

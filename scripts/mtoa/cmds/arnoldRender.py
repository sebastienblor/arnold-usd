from __future__ import print_function
import maya.cmds as cmds
import maya.mel as mel
import mtoa.core as core
import platform, os
import mtoa.utils as mutils
import mtoa.batchRenderOptions
_maya_version = mutils.getMayaVersion()

def arnoldRender(width, height, doShadows, doGlowPass, camera, options):
    # Close the Maya RenderView
    if cmds.control("renderViewWindow", exists=True):
        mel.eval('deleteUI "renderViewWindow"')
    
    # Make sure the aiOptions node exists
    core.createOptions()

    # We need to give time for Maya to close the maya RenderView, so we're calling 
    # evalDeferred
    cmd = 'cmds.arnoldRenderView(cam="{}", w = {}, h={});'.format(camera, width, height)
    cmd += 'cmds.arnoldRenderView(option=("Run IPR","0"));'
    cmd += 'cmds.arnoldRenderView(option=("Scene Updates", "0"));'
    cmd += 'cmds.arnoldRenderView(option=("Refresh Render", "1"));'
    cmds.evalDeferred(cmd)
    

def arnoldSequenceRender(width, height, camera, saveToRenderView):

    useARV = False
    # For now we're still using the Maya RenderView to render a sequence.
    # In order to change this, we should set useArv to True
    
    if 'MTOA_ARV_SEQUENCE' in os.environ and os.environ['MTOA_ARV_SEQUENCE'] != '0':
        useARV = True
        
    if useARV:

        # Close the Maya RenderView
        if cmds.control("renderViewWindow", exists=True):
            mel.eval('deleteUI "renderViewWindow"')

        # Make sure the aiOptions node exists
        core.createOptions()
        if saveToRenderView:
            cmds.optionVar(iv=('ArnoldSequenceSnapshot', 1))

        cmd = 'cmds.arnoldRenderView(mode="sequence", w = {}, h={}'.format(width, height)

        if len(camera) > 0:
            cmd += ', cam="{}")'.format(camera)
        else:
            cmd += ')'

        # We need to give time for Maya to close the maya RenderView, so we're calling 
        # evalDeferred
        cmds.evalDeferred(cmd)
    else:
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
    except RuntimeError as err:
        print(err)

        # need to raise the error otherwise it's not caught by maya (#3405)
        if _maya_version >= 2019: # doing it only on mainline for testing
            raise
        
def arnoldBatchStop():
    mel.eval('batchRender')

def arnoldBatchRenderOptions():
    win = mtoa.batchRenderOptions.MtoABatchRenderOptions()
    win.create()

def arnoldIprStart(editor, width, height, camera):
    # Make sure the aiOptions node exists
    core.createOptions()
    if cmds.control("renderViewWindow", exists=True):
        mel.eval('deleteUI "renderViewWindow"')
    
    # We need to give time for Maya to close the maya RenderView, so we're calling 
    # evalDeferred
    cmd = 'cmds.arnoldRenderView(mode="open", cam="{}", w = {}, h={});'.format(camera, width, height)
    cmd += 'cmds.arnoldRenderView(option=("Progressive Refinement", "1"));'
    cmd += 'cmds.arnoldRenderView(option=("Run IPR", "1"));'
    cmds.evalDeferred(cmd)
    
def arnoldIprStop():
    cmds.arnoldRenderView('mode="stop"')

def arnoldIprIsRunning():
    res = cmds.arnoldRenderView(getoption=("Run IPR")) or 0
    return int(res) > 0;
    
def arnoldIprRender(width, height, doShadows, doGlowPass, camera):
    core.createOptions()
    if cmds.control("renderViewWindow", exists=True):
        mel.eval('deleteUI "renderViewWindow"')

    # We need to give time for Maya to close the maya RenderView, so we're calling 
    # evalDeferred
    cmd = 'cmds.arnoldRenderView(option=("Run IPR", "1"));'
    cmds.evalDeferred(cmd)

def arnoldIprRefresh():
    cmds.arnoldRenderView('option=("Refresh Render", "1"))')

def arnoldIprPause(editor, pause):
    if pause:
        cmds.arnoldRenderView('option=("Abort Render", "1"))')
        cmds.arnoldRenderView('option=("Run IPR", "0"))')
    else:
        cmds.arnoldRenderView('option=("Run IPR", "1"))')

def arnoldIprChangeRegion(renderPanel):
    cmds.arnoldRenderView('option=("Crop Region", "1"))')

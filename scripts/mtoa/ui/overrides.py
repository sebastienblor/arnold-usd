import maya.cmds as cmds
import maya.mel as mel
import mtoa.utils as mu
import mtoa.ui.arnoldmenu as arnoldmenu

# Override the command ran by the renderViewButton
def RenderViewWindow(*args):
    renderer = mu.currentRenderer()
    if renderer == 'arnold':
        arnoldmenu.arnoldOpenMtoARenderView()
    else:
        localizedName = mel.eval("localizedPanelLabel(\"Render View\")")
        mel.eval("tearOffRestorePanel \"" + localizedName + "\" \"renderWindowPanel\" true;")

mu.pyToMelProc(RenderViewWindow, useName=True)

# Override the command ran by the renderButton
def RenderIntoNewWindow(*args):
    renderer = mu.currentRenderer()
    if renderer == 'arnold':
        if not cmds.workspaceControl("ArnoldRenderView", exists=True):
            arnoldmenu.arnoldOpenMtoARenderView()
        else:
            cmds.workspaceControl("ArnoldRenderView", e=True, visible=True)
            cmds.arnoldRenderView(mode="refresh")
        cmds.arnoldRenderView(option=("Run IPR","0"))
        cmds.arnoldRenderView(option=("Scene Updates", "0"))
        cmds.arnoldRenderView(option=("Refresh Render", "1"))
    else:
        mel.eval("renderIntoNewWindow render")

mu.pyToMelProc(RenderIntoNewWindow, useName=True)

# Override the command ran by the iprRenderButton
def IPRRenderIntoNewWindow():
    renderer = mu.currentRenderer()
    if renderer == 'arnold':
        arnoldmenu.arnoldMtoARenderView()
        cmds.arnoldRenderView(option=("Run IPR", "1"))
    else:
        mel.eval("renderIntoNewWindow iprRender")

mu.pyToMelProc(IPRRenderIntoNewWindow, useName=True)

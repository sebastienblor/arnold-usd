import maya.cmds as cmds
from mtoa.callbacks import *


UI_NAME = 'ArnoldAOVUI'

g_ArnoldStdAOVs = {"direct_diffuse":1,
                   "direct_specular":1,
                   "indirect_diffuse":1,
                   "indirect_specular":1,
                   "emission":1,
                   "reflection":1,
                   "refraction":1,
                   "sss":1,
                   "Z":0,
                   "P":4}

def listIndex(item, list):
    for i in range(0, len(list)):
        if list[i] == item:
            return i

    return -1;

def arnoldAddAOV(node):
    sel = cmds.textScrollList('%s_availableLst'%UI_NAME, query=True, selectItem=True)
    if sel is None:
        sel = []

    for aov in sel:
        n = cmds.getAttr('%s.aovs'%node, size=True)
        i = 0
        while i < n:
            name = cmds.getAttr('%s.aovs[%d].aov_name'%(node, i))
            if len(name) == 0:
                break
            i += 1

        cmds.setAttr('%s.aovs[%d].aov_name'%(node, i), aov, type="string")
        cmds.setAttr('%s.aovs[%d].aov_type'%(node, i), g_ArnoldStdAOVs[aov])
        cmds.setAttr('%s.aovs[%d].aov_prefix'%(node, i), '', type="string")

        cmds.textScrollList('%s_activeLst'%UI_NAME, edit=True, append=aov)
        cmds.textScrollList('%s_availableLst'%UI_NAME, edit=True, removeItem=aov)

def arnoldRemAOV(node):
    sel = cmds.textScrollList('%s_activeLst'%UI_NAME, query=True, selectItem=True)
    if sel is None:
        sel = []

    for aov in sel:
        n = cmds.getAttr('%s.aovs'%node, size=True)
        for i in range(0, n):
            name = cmds.getAttr('%s.aovs[%d].aov_name'%(node, i))
            if name == aov:

                for j in range(i+1, n):
                    cmds.setAttr('%s.aovs[%d].aov_name'%(node, j-1), cmds.getAttr('%s.aovs[%d].aov_name'%(node, j)), type='string')
                    cmds.setAttr('%s.aovs[%d].aov_prefix'%(node, j-1), cmds.getAttr('%s.aovs[%d].aov_prefix'%(node, j)), type='string')

                cmds.setAttr('%s.aovs[%d].aov_name'%(node, n-1), '', type='string')
                cmds.setAttr('%s.aovs[%d].aov_prefix'%(node, n-1), '', type='string')

                cmds.textScrollList('%s_availableLst'%UI_NAME, edit=True, append=aov)
                cmds.textScrollList('%s_activeLst'%UI_NAME, edit=True, removeItem=aov)

                break

    cmds.textField('%s_prefixFld'%UI_NAME, edit=True, enable=False)


def arnoldSetAOVPrefix(node):
    sel = cmds.textScrollList('%s_activeLst'%UI_NAME, query=True, selectItem=True)
    if sel is None:
        sel = []

    if len(sel) != 1:
        return

    aov = sel[0]

    n = cmds.getAttr("%s.aovs"%node, size=True)
    for i in range(0, n):
        name = cmds.getAttr('%s.aovs[%d].aov_name'%(node, i))
        if name == aov:
            cmds.setAttr("%s.aovs[%d].aov_prefix"%(node, i), cmds.textField('%s_prefixFld'%UI_NAME, query=True, text=True), type='string')
            break


def arnoldSelectAOV(node):

    sel = cmds.textScrollList('%s_activeLst'%UI_NAME, query=True, selectItem=True)
    if sel is None:
        sel = []

    if len(sel) != 1:
        cmds.textField('%s_prefixFld'%UI_NAME, edit=True, enable=False)
        return
    else:
        cmds.textField('%s_prefixFld'%UI_NAME, edit=True, enable=True)

    aov = sel[0]

    n = cmds.getAttr('%s.aovs'%node, size=True,)
    for i in range(0, n):
        name = cmds.getAttr('%s.aovs[%d].aov_name'%(node, i))
        if name == aov:
            prefix = cmds.getAttr('%s.aovs[%d].aov_prefix'%(node, i))
            cmds.textField('%s_prefixFld'%UI_NAME, edit=True, text=prefix)
            break


def arnoldToggleAOVBatchModeOnly(node):
    cmds.setAttr('%s.aov_batch_mode_only'%node, cmds.checkBox('%s_bmoChk'%UI_NAME, query=True, value=True))



def arnoldAOVEditor(*args):

    if cmds.window(UI_NAME, exists=True):
        cmds.deleteUI(UI_NAME)

    win = cmds.window(UI_NAME, title='AOV setup', width=640, height=300)

    aovnode = ""
    conns = cmds.listConnections('defaultArnoldRenderOptions.aovs', s=1, d=0)
    if not conns:
        aovnode = cmds.createNode('ArnoldAOV')
        cmds.connectAttr('%s.message'%aovnode, 'defaultArnoldRenderOptions.aovs')
    else:
        aovnode = conns[0]

    aovList = []
    naovs = cmds.getAttr('%s.aovs'%aovnode, size=True)
    for caov in range(0, naovs):
        aov = cmds.getAttr('%s.aovs[%d].aov_name'%(aovnode, caov))
        if aov:
            aovList.append(aov)



    form = cmds.formLayout('ArnoldAOVUI_topFrm', numberOfDivisions=100)

    cmds.text('%s_bmoLbl'%UI_NAME, align='center', label='Only output AOVs in batch mode', parent=form)
    cmds.checkBox('%s_bmoChk'%UI_NAME, label='', value=cmds.getAttr('%s.aov_batch_mode_only'%aovnode), parent=form, changeCommand=Callback(arnoldToggleAOVBatchModeOnly, aovnode))
    cmds.text('%s_availableLbl'%UI_NAME, align='center', label='Available AOVs', parent=form)
    cmds.text('%s_activeLbl'%UI_NAME, align='center', label='Active AOVs', parent=form)
    cmds.textScrollList('%s_availableLst'%UI_NAME, numberOfRows=10, allowMultiSelection=True, parent=form, doubleClickCommand=Callback(arnoldAddAOV, aovnode))
    cmds.textScrollList('%s_activeLst'%UI_NAME, numberOfRows=10, allowMultiSelection=True, parent=form, doubleClickCommand=Callback(arnoldRemAOV, aovnode), selectCommand=Callback(arnoldSelectAOV, aovnode))
    cmds.button('%s_addBtn'%UI_NAME, label='>>', parent=form, command=Callback(arnoldAddAOV, aovnode))
    cmds.button('%s_remBtn'%UI_NAME, label='<<', parent=form, command=Callback(arnoldRemAOV, aovnode))
    cmds.text('%s_prefixLbl'%UI_NAME, align='center', label='Prefix', parent=form)
    cmds.textField('%s_prefixFld'%UI_NAME, enable=False, text='', parent=form, changeCommand=Callback(arnoldSetAOVPrefix, aovnode))

    for i in sorted(g_ArnoldStdAOVs):
        if i not in aovList:
            cmds.textScrollList('%s_availableLst'%UI_NAME, edit=True, append=i)
        else:
            cmds.textScrollList('%s_activeLst'%UI_NAME, edit=True, append=i)


    cmds.formLayout(form, edit=True,

              attachForm     = [('%s_bmoLbl'%UI_NAME,"top",2),
                               ('%s_bmoLbl'%UI_NAME,"left",2),
                               ('%s_bmoChk'%UI_NAME,"top",2),
                               ('%s_bmoChk'%UI_NAME,"right",2),
                               ('%s_availableLbl'%UI_NAME,"left",2),
                               ('%s_activeLbl'%UI_NAME,"right",2),
                               ('%s_availableLst'%UI_NAME,"left",2),
                               ('%s_activeLst'%UI_NAME,"right",2),
                               ('%s_prefixLbl'%UI_NAME,"left",2),
                               ('%s_prefixLbl'%UI_NAME,"bottom",2),
                               ('%s_prefixFld'%UI_NAME,"right",2),
                               ('%s_prefixFld'%UI_NAME,"bottom",2)],

              attachPosition = [('%s_bmoLbl'%UI_NAME,"right",2,50),
                               ('%s_bmoChk'%UI_NAME,"left",2,50),
                               ('%s_availableLbl'%UI_NAME,"right",10,50),
                               ('%s_activeLbl'%UI_NAME,"left",10,50),
                               ('%s_addBtn'%UI_NAME,"bottom",2,50),
                               ('%s_addBtn'%UI_NAME,"left",2,45),
                               ('%s_addBtn'%UI_NAME,"right",2,55),
                               ('%s_remBtn'%UI_NAME,"top",2,50),
                               ('%s_remBtn'%UI_NAME,"left",2,45),
                               ('%s_remBtn'%UI_NAME,"right",2,55),
                               ('%s_prefixLbl'%UI_NAME,"right",2,25),
                               ('%s_prefixFld'%UI_NAME,"left",2,25)],

              attachNone     = [('%s_bmoLbl'%UI_NAME,"bottom"),
                               ('%s_availableLbl'%UI_NAME,"bottom"),
                               ('%s_bmoChk'%UI_NAME,"bottom"),
                               ('%s_activeLbl'%UI_NAME,"bottom"),
                               ('%s_addBtn'%UI_NAME,"top"),
                               ('%s_remBtn'%UI_NAME,"bottom"),
                               ('%s_prefixLbl'%UI_NAME,"top"),
                               ('%s_prefixFld'%UI_NAME,"top")],

              attachControl  = [('%s_availableLbl'%UI_NAME,"top",2,'%s_bmoLbl'%UI_NAME),
                               ('%s_activeLbl'%UI_NAME,"top",2, '%s_bmoChk'%UI_NAME),
                               ('%s_availableLst'%UI_NAME,"top",2,'%s_availableLbl'%UI_NAME),
                               ('%s_availableLst'%UI_NAME,"right",2,'%s_addBtn'%UI_NAME),
                               ('%s_availableLst'%UI_NAME,"bottom",2,'%s_prefixFld'%UI_NAME),
                               ('%s_activeLst'%UI_NAME,"top",2,'%s_activeLbl'%UI_NAME),
                               ('%s_activeLst'%UI_NAME,"left",2,'%s_addBtn'%UI_NAME),
                               ('%s_activeLst'%UI_NAME,"bottom",2,'%s_prefixFld'%UI_NAME)])

    cmds.showWindow(win)

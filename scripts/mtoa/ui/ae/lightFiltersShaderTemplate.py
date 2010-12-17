import maya.cmds as cmds
import maya.mel as mel
import mtoa.ui.ae.utils as aeUtils
from mtoa.ui.callback import *

def getSourcePlug(plugname, index):
    conns = []
    if index < 0:
       conns = cmds.listConnections(plugname, p=1, s=1, d=0)
    else:
       conns = cmds.listConnections('%s[%s]'%(plugname, index), p=1, s=1, d=0)
    if conns:
        if len(conns) == 1:
           return conns[0]
        else:
           return ""


def getNodeName(plugname):
    if not plugname:
        return 0
    nodeAttrs = plugname.split('.')

    if len(nodeAttrs) >= 1:
       return nodeAttrs[0]

    return "";


def getConnectedCount(plugname):
    srcplug = ''
    n= cmds.getAttr(plugname, size=True)
    c = 0

    for i in range(0, n):
        srcplug = getSourcePlug(plugname, i)
        if srcplug:
            c+=1

    return c

def swapConnections(plugname, i_zero, i_one):

    dstPlug_zero = '%s[%s]'%(plugname, i_zero)
    dstPlug_one = '%s[%s]'%(plugname, i_one)
    srcPlug_zero = getSourcePlug(dstPlug_zero, -1)
    srcPlug_one = getSourcePlug(dstPlug_one, -1)

    if srcPlug_zero != "":
        cmds.disconnectAttr(srcPlug_zero, dstPlug_zero)

    if srcPlug_one != "":
        cmds.disconnectAttr(srcPlug_one, dstPlug_one)

    if srcPlug_zero != "":
        cmds.connectAttr(srcPlug_zero, dstPlug_one)

    if srcPlug_one != "":
        cmds.connectAttr(srcPlug_one, dstPlug_zero)


def moveLightFilterUp(node, list):

    attr = "%s.light_filters"%node
    items = cmds.textScrollList(list, q=True, sii=True)
    if not items:
        return 0

    current = items[0] - 1
    if current > 0:
        swapConnections(attr, current-1, current)
        lightFiltersGetList(node, list)



def moveLightFilterDown(node, list):

    attr = "%s.light_filters"%node
    items = cmds.textScrollList(list, q=True, sii=True)
    if not items:
        return 0

    current = items[0]

    if current < getConnectedCount(attr):
        swapConnections(attr, current-1, current)
        lightFiltersGetList(node, list)


def lightFilterListChanged(list):

    items = cmds.textScrollList(list, q=True, si=True)

    selection = 0
    if items:
        selection = 1

    cmds.button('lf_remove_button', edit=True, enable=selection)
    cmds.button('lf_move_up_button', edit=True, enable=selection)
    cmds.button('lf_move_down_button', edit=True, enable=selection)


def addLightFilterCB(lightNode, list):

    attr =  "%s.light_filters"%lightNode
    filterTypes = cmds.textScrollList(list, q=True, si=True)
    filterType = ''

    if filterTypes:
        filterType = filterTypes[0]

    new_filter = ""

    if filterType == "Barndoor":
        new_filter = cmds.createNode('ArnoldBarndoorShader', skipSelect=True)

    elif filterType == "Gobo":
        new_filter = cmds.createNode('ArnoldGoboShader', skipSelect=True)

    elif filterType == "Light Blocker":
        new_filter = cmds.createNode('ArnoldLightBlockerShader', skipSelect=True)

    elif filterType == "Light Decay":
        new_filter = cmds.createNode('ArnoldLightDecayShader', skipSelect=True)


    if new_filter:
        nfilters = getConnectedCount(attr)
        cmds.connectAttr("%s.message"%new_filter, '%s[%s]'%(attr, nfilters))
        lightFiltersGetList(lightNode, list)
        mel.eval('updateAE "%s"'%new_filter)

def addFilterAndHide(lightNode, list, win):
    addLightFilterCB(lightNode, list)
    cmds.window(win, edit=True, visible=False)

def addLightFilter(lightNode, list):

    win = "arnold_filter_list_win"
    if cmds.window(win, exists=True):
        cmds.deleteUI(win)

    cmds.window(win, title="Add Light Filter", resizeToFitChildren=True)

    cmds.windowPref(removeAll=True)

    cmds.columnLayout(adjustableColumn=True,
                      columnOffset=("both", 10),
                      rowSpacing=10)

    list = cmds.textScrollList('alf_filter_list', nr=4, ams=False)
    cmds.textScrollList(list, e=True, dcc=Callback(addFilterAndHide, lightNode, list, win))

    nodeType = cmds.objectType(lightNode)

    if nodeType == "spotLight":
        cmds.textScrollList(list, edit=True, append="Barndoor")
        cmds.textScrollList(list, edit=True, append="Gobo")

    cmds.textScrollList(list, edit=True, append="Light Blocker")
    cmds.textScrollList(list, edit=True, append="Light Decay")


    cmds.rowLayout(numberOfColumns=2, columnAlign2=("center", "center"))
    cmds.button(width=80, label="Add", command=Callback(addFilterAndHide, lightNode, list, win))
    cmds.button(width=80, label="Cancel", command=Callback(cmds.deleteUI, win, window=True))

    cmds.setParent('..')
    cmds.setParent('..')

    cmds.showWindow(win)


def removeLightFilter(lightNode, list):

    attr = "%s.light_filters"%lightNode

    selection = cmds.textScrollList(list, q=True, si=True)
    selected = ''
    if selection:
        selected = selection[0]

    nfilters = getConnectedCount(attr)

    for j in range(0, nfilters):

        srcplug = getSourcePlug(attr, j)
        filter = getNodeName(srcplug)

        if filter == selected:
            for k in range(j+1, nfilters):
                swapConnections(attr, j, k)
                j+=1

            cmds.disconnectAttr(srcplug, '%s[%s]'%(attr, nfilters-1))
            cmds.delete(filter)

    lightFiltersGetList(lightNode, list)


def lightFiltersGetList(node, list):

    nfilters = getConnectedCount("%s.light_filters"%node)

    selection = cmds.textScrollList(list, q=True, si=True)
    selected = ''
    if selection:
        selected = selection[0]

    cmds.textScrollList(list, edit=True, removeAll=True)
    for j in range(0, nfilters):
        filter = getNodeName(getSourcePlug("%s.light_filters"%node, j))
        if filter == "":
            continue

        cmds.textScrollList(list, edit=True, append=filter)
        if filter == selected:
            cmds.textScrollList(list, edit=True, si=selected)

    lightFilterListChanged(list)


def customLightFiltersChanged(attr, userChangeCB=None):

    if not attr:
        return 0

    buffer = attr.split('.')

    nodeName   = buffer[0]
    attribName = buffer[1]
    nodeType   = aeUtils.getNodeType(nodeName)

    if userChangeCB:
        cmds.evalDeferred(userChangeCB)


def updateCustomLightFiltersNew(uiName):
    val = cmds.textScrollList(uiName, q=True, si=True)
    if val:
        mel.eval('updateAE "%s"'%val[0])

def customLightFiltersNew(attr):
    if not attr:
        return 0

    buffer = attr.split('.')

    nodeName   = buffer[0]
    attribName = buffer[1]
    nodeType   = aeUtils.getNodeType(nodeName)

    if not aeUtils.attributeExists(attribName, nodeName):
        cmds.addAttr(nodeName, ln=attribName, at="message", multi=True)

    uiName = '%s_%s'%(nodeName, attribName)

    cmds.rowLayout(nc=3, rat=(1, "top", 0), cw=[(1, 30), (2, 330)])
    cmds.text(label="")
    cmds.columnLayout(adj=True, cat=("both", 0), rs=5)
    cmds.textScrollList(uiName, h=150, ams=False, sc=Callback(lightFilterListChanged, uiName), dcc=Callback(updateCustomLightFiltersNew, uiName))
    cmds.rowLayout(nc=2, cw2=(80,80), columnAttach2=("both", "both"), columnAlign2=("center", "center"), co2=(2, 2))
    cmds.button('lf_add_button', label="Add", c=Callback(addLightFilter, nodeName, uiName))
    cmds.button('lf_remove_button', label="Remove", c=Callback(removeLightFilter, nodeName, uiName))
    cmds.setParent('..')
    cmds.setParent('..')
    cmds.columnLayout(adj=True, cat=("both", 0), rs=5)
    cmds.symbolButton('lf_move_up_button', image='arrowUp.xpm', c=Callback(moveLightFilterUp, nodeName, uiName))
    cmds.symbolButton('lf_move_down_button', image='arrowDown.xpm', c=Callback(moveLightFilterDown, nodeName, uiName))
    cmds.setParent('..')
    cmds.setParent('..')

    lightFiltersGetList(nodeName, uiName)


def customLightFiltersReplace(attr):
    if not attr:
        return 0

    buffer = attr.split('.')

    nodeName   = buffer[0]
    attribName = buffer[1]
    nodeType   = aeUtils.getNodeType(nodeName)

    if not aeUtils.attributeExists(attribName, nodeName):
        cmds.addAttr(nodeName, ln=attribName, at="message", multi=True)


    uiName = '%s_%s'%(nodeName, attribName)

    cmds.textScrollList(uiName, edit=True, dcc=Callback(updateCustomLightFiltersNew, uiName))
    cmds.symbolButton('lf_move_up_button', edit=True, c=Callback(moveLightFilterUp, nodeName, uiName))
    cmds.symbolButton('lf_move_down_button', edit=True, c=Callback(moveLightFilterDown, nodeName, uiName))

    cmds.button('lf_add_button', edit=True, c=Callback(addLightFilter, nodeName, uiName))
    cmds.button('lf_remove_button', edit=True, c=Callback(removeLightFilter,nodeName, uiName))

    lightFiltersGetList(nodeName, uiName)


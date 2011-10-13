import maya.cmds as cmds
import maya.mel as mel
import mtoa.utils as utils
from mtoa.ui.nodeTreeLister import aiHyperShadeCreateMenu_BuildMenu, createArnoldNodesTreeLister_Content

def aiHyperShadePanelBuildCreateMenu() :
    print "callback : aiHyperShadePanelBuildCreateMenu"
    if cmds.pluginInfo('mtoa', query=True, loaded=True) :
        aiHyperShadeCreateMenu_BuildMenu()
        cmds.menuItem(divider=True)

def aiHyperShadePanelBuildCreateSubMenu() :
    print "callback : aiHyperShadePanelBuildCreateSubMenu"
    parent = cmds.setParent(query=True, menu=True)
    label = cmds.menuItem(parent, query=True, label=True)
    annotation = cmds.menuItem(parent, query=True, annotation=True)
    # print "parent   : %s" % parent
    # print "label    : %s" % label
    # print "annotation   : %s" % annotation
    # how can we deduce what category to return to (uf i understood well)
    # prevent arnold nodes from also appearing in the create submenus?
    # return ("arnold/shader",)
    return "rendernode/arnold"

def aiRenderNodeClassificationCallback() :
    print "callback : aiRenderNodeClassificationCallback"
    return "rendernode/arnold"

def aiCreateRenderNodeSelectNodeCategoriesCallback(nodeTypesFlag, renderNodeTreeLister) :
    print "callback : aiCreateRenderNodeSelectNodeCategoriesCallback"
    if (nodeTypesFlag == "allWithArnoldUp") :
        cmds.treeLister(renderNodeTreeLister, edit=True, selectPath="arnold")

def aiBuildRenderNodeTreeListerContentCallback(renderNodeTreeLister, postCommand, filterString) :
    print "callback : aiBuildRenderNodeTreeListerContentCallback"
    # seems a list is passed after all ?
    # filterClassArray = filterString.split(" ")
    filterClassArray = filterString
    createArnoldNodesTreeLister_Content(renderNodeTreeLister, postCommand, filterClassArray)

    
# Add the callbacks

# we probably don't need this if we correctly import in global namespace

utils.pyToMelProc(aiHyperShadePanelBuildCreateMenu, [], useName=True)

utils.pyToMelProc(aiHyperShadePanelBuildCreateSubMenu, [], useName=True)    

mel.eval("callbacks -addCallback \"aiHyperShadePanelBuildCreateMenu\" -hook \"hyperShadePanelBuildCreateMenu\" -owner \"arnold\"")

mel.eval("callbacks -addCallback \"aiHyperShadePanelBuildCreateSubMenu\" -hook \"hyperShadePanelBuildCreateSubMenu\" -owner \"arnold\"")


#cmds.callbacks(addCallback="aiHyperShadePanelBuildCreateMenu()", hook="hyperShadePanelBuildCreateMenu", owner="arnold")

#cmds.callbacks(addCallback="aiHyperShadePanelBuildCreateSubMenu()", hook="hyperShadePanelBuildCreateSubMenu", owner="arnold")

# however until callbacks support python functions with arguments, we do need these hacks

utils.pyToMelProc(aiCreateRenderNodeSelectNodeCategoriesCallback,
                        [('string', 'nodeTypesFlag'),
                         ('string', 'renderNodeTreeLister')], useName=True)

utils.pyToMelProc(aiBuildRenderNodeTreeListerContentCallback,
                        [('string', 'renderNodeTreeLister'),
                         ('string', 'postCommand'),
                         ('string', 'filterString')], useName=True)

mel.eval("callbacks -addCallback \"aiCreateRenderNodeSelectNodeCategoriesCallback\" -hook \"createRenderNodeSelectNodeCategories\" -owner \"arnold\"")

mel.eval("callbacks -addCallback \"aiBuildRenderNodeTreeListerContentCallback\" -hook \"buildRenderNodeTreeListerContent\" -owner \"arnold\"")

#cmds.callbacks(addCallback="aiCreateRenderNodeSelectNodeCategoriesCallback", hook="createRenderNodeSelectNodeCategories", owner="arnold")

#cmds.callbacks(addCallback="aiBuildRenderNodeTreeListerContentCallback", hook="buildRenderNodeTreeListerContent", owner="arnold")


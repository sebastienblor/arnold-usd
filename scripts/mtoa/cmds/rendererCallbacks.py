import maya.cmds as cmds
import maya.mel as mel
import mtoa.utils as utils
from mtoa.ui.nodeTreeLister import aiHyperShadeCreateMenu_BuildMenu, createArnoldNodesTreeLister_Content
import mtoa.ui.ae.templates as templates

def aiHyperShadePanelBuildCreateMenuCallback() :
    print "aiHyperShadePanelBuildCreateMenuCallback"
    if cmds.pluginInfo('mtoa', query=True, loaded=True) :
        aiHyperShadeCreateMenu_BuildMenu()
        cmds.menuItem(divider=True)

def aiHyperShadePanelBuildCreateSubMenuCallback() :
    print "aiHyperShadePanelBuildCreateSubMenuCallback"
    return "rendernode/arnold"

def aiRenderNodeClassificationCallback() :
    print "aiRenderNodeClassificationCallback"
    return "rendernode/arnold"

def aiCreateRenderNodePluginChangeCallback(classification) :
    print "aiCreateRenderNodePluginChangeCallback"
    return classification.startswith("rendernode/arnold")

def aiCreateRenderNodeSelectNodeCategoriesCallback(nodeTypesFlag, renderNodeTreeLister) :
    print "aiCreateRenderNodeSelectNodeCategoriesCallback"
    if (nodeTypesFlag == "allWithArnoldUp") :
        cmds.treeLister(renderNodeTreeLister, edit=True, selectPath="arnold")

def aiBuildRenderNodeTreeListerContentCallback(renderNodeTreeLister, postCommand, filterString) :
    print "aiBuildRenderNodeTreeListerContentCallback"
    # seems a list is passed after all ?
    # filterClassArray = filterString.split(" ")
    print "filterString   : %s" % filterString
    filterClassArray = filterString
    createArnoldNodesTreeLister_Content(renderNodeTreeLister, postCommand, filterClassArray)

    
# Add the callbacks

cmds.callbacks(addCallback=aiHyperShadePanelBuildCreateMenuCallback,
               hook="hyperShadePanelBuildCreateMenu",
               owner="arnold")

cmds.callbacks(addCallback=aiHyperShadePanelBuildCreateSubMenuCallback,
               hook="hyperShadePanelBuildCreateSubMenu",
               owner="arnold")

cmds.callbacks(addCallback=aiCreateRenderNodeSelectNodeCategoriesCallback,
               hook="createRenderNodeSelectNodeCategories",
               owner="arnold")

cmds.callbacks(addCallback=aiRenderNodeClassificationCallback,
               hook="addToRenderNodeTreeLister",
               owner="arnold")

cmds.callbacks(addCallback=aiBuildRenderNodeTreeListerContentCallback,
               hook="buildRenderNodeTreeListerContent",
               owner="arnold")

cmds.callbacks(addCallback=aiCreateRenderNodePluginChangeCallback,
               hook="createRenderNodePluginChange",
               owner="arnold")

cmds.callbacks(addCallback=templates.loadArnoldTemplate,
               hook="AETemplateCustomContent",
               owner="arnold")

import maya.cmds as cmds
import maya.mel as mel
import mtoa.utils as utils
from mtoa.ui.nodeTreeLister import aiHyperShadeCreateMenu_BuildMenu, createArnoldNodesTreeLister_Content
import mtoa.ui.ae.templates as templates

def aiHyperShadePanelBuildCreateMenuCallback() :
    if cmds.pluginInfo('mtoa', query=True, loaded=True) :
        aiHyperShadeCreateMenu_BuildMenu()
        cmds.menuItem(divider=True)

def aiHyperShadePanelBuildCreateSubMenuCallback() :  
    return "rendernode/arnold"

def aiRenderNodeClassificationCallback() :
    return "rendernode/arnold"

def aiCreateRenderNodePluginChangeCallback(classification) :
    return classification.startswith("rendernode/arnold")

def aiCreateRenderNodeSelectNodeCategoriesCallback(nodeTypesFlag, renderNodeTreeLister) :
    if (nodeTypesFlag == "allWithArnoldUp") :
        cmds.treeLister(renderNodeTreeLister, edit=True, selectPath="arnold")

def aiBuildRenderNodeTreeListerContentCallback(renderNodeTreeLister, postCommand, filterString) :
    filterClassArray = filterString
    createArnoldNodesTreeLister_Content(renderNodeTreeLister, postCommand, filterClassArray)

def aiProvideClassificationStringsForFilteredTreeListerCallback(classification) :
    return "rendernode/arnold/shader/surface"


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

# FIXME: Maya doc is wrong
#cmds.callbacks(addCallback=aiRenderNodeClassificationCallback,
#               hook="addToRenderNodeTreeLister",
#              owner="arnold")
# Should be this instead

cmds.callbacks(addCallback=aiRenderNodeClassificationCallback,
               hook="renderNodeClassification",
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

cmds.callbacks(addCallback=aiProvideClassificationStringsForFilteredTreeListerCallback,
               hook="provideClassificationStringsForFilteredTreeLister",
               owner="arnold")

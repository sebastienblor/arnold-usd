"""
add arnold nodes to maya's node tree lister

there are several options for getting callbacks for adding our own nodes, but because autodesk gives Mayatomr specail
treatment it is very hard for 3rd party renderers to properly integrate.
 
options:
  - use the renderNodeTreeInitializeUserProc optionVar: could be displaced at any time by a custom script or another renderer.
  - override buildRenderNodeTreeListerContent: difficult to maintain as it changes between versions
  - override the functions that call buildRenderNodeTreeListerContent: best option

"""

import mtoa.utils as utils
import maya.mel as mel
import maya.cmds as cmds
from mtoa.ui.callback import *
from collections import namedtuple
import re

NodeClassInfo = namedtuple('NodeClassInfo', ['staticClassification', 'runtimeClassification', 'nodePath', 'nodeTypes'])

global _aiCreateCustomNodeProc
_aiCreateCustomNodeProc = ''

# known categories: used for ordering in the UI
CATEGORIES = ('surface', 'volume', 'displacement', 'texture', 'light', 'utility')
CATEGORY_TO_RUNTIME_CLASS = {
                'surface':      'asShader',
                'volume':       'asShader',
                'displacement': 'asShader',                
                'texture':      'asTexture',
                'light':        'asLight',
                'utility':      'asUtility',
                }

def capitalize(s):
    return s[0].upper() + s[1:] if s else s

def prettify(s):
    "convert from '_fooBar_Spangle22poop1' to 'Foo Bar Spangle22 Poop1'"
    return ' '.join([capitalize(x) for x in re.findall('[a-zA-Z][a-z]*[0-9]*',s)])

def processClass(nodeType):
    '''
    convert the passed node type's classification string to a tuple containing a formatted path string compatible with
    the node lister and the runtime classification.
    
    e.g. from 'aiStandard' to 'arnold/shader/surface' to ('Arnold/Surface', 'asShader')
    '''
    classification = cmds.getClassification(nodeType)[0]
    for klass in classification.split(':'):
        if klass.startswith('arnold/shader'):
            parts = klass.split('/')
            if len(parts) < 3:
                return (klass, 'asUtility', 'Arnold')
            # remove "shader"
            parts.pop(1)
            #labelName = node.replace('/', '|')
            return (klass,
                    CATEGORY_TO_RUNTIME_CLASS.get(parts[1], 'asUtility'),
                    '/'.join([prettify(x) for x in parts]))

global _typeInfoMap
_typeInfoMap = None
def getTypeInfo():
    '''
    return a tuple of NodeClassInfo namedtuples containing 
    (staticClassification, runtimeClassifciation, nodePath, nodeTypes)
    '''
    global _typeInfoMap
    if _typeInfoMap is None:
        # use a dictionary to get groupings
        tmpmap = {}
        nodeTypes = cmds.listNodeTypes('arnold/shader')
        if nodeTypes:
            for nodeType in nodeTypes:
                (staticClass, runtimeClass, nodePath) = processClass(nodeType)
                if staticClass not in tmpmap:
                    print nodeType, staticClass
                    tmpmap[staticClass] = [runtimeClass, nodePath, [nodeType]]
                else:
                    tmpmap[staticClass][2].append(nodeType)
        # consistent order is important for UIs. build a reliably ordered list. 
        tmplist = []
        # known types first.
        for cat in CATEGORIES:
            cat = 'arnold/shader/' + cat
            if cat in tmpmap:
                values = tmpmap.pop(cat)
                tmplist.append(NodeClassInfo(*([cat] + values)))
        # custom types in alphabetical order
        for custom in sorted(tmpmap.keys()):
            tmplist.append(NodeClassInfo(*([custom] + tmpmap[custom])))
        _typeInfoMap = tuple(tmplist)
    return _typeInfoMap

def createTreeListerContent(renderNodeTreeLister, postCommand, filterString):
    if not filterString:
        for (staticClass, runtimeClass, nodePath, nodeTypes) in getTypeInfo():
            print nodePath, runtimeClass
            for nodeType in nodeTypes:
                command = Callback(aiCreateCustomNode, runtimeClass, postCommand, nodeType)
                cmds.nodeTreeLister(renderNodeTreeLister, e=True, add=[nodePath + '/' + nodeType, "render_%s.png" % nodeType, command])
    else:
        #TODO: setup filtering ??I think this is unnecessary with the new QT UI.
        pass

def aiHyperShadeCreateMenu_BuildMenu():
    """
    Function:   aiHyperShadeCreateMenu_BuildMenu()
    Purpose:    Builds menu items for creating mental ray nodes, organized
                into submenus by category.

    Notes:  When this function is invoked, it is inside of the Create menu.
            This function mimics the buildCreateSubmenu() function in 
            hyperShadePanel.mel, and in fact calls that function with a slightly
            different set of arguments than the other Maya node types.  For 
            arnold nodes, the menu items are set up to call back to the
            aiCreateCustomNode() function for node creation.
    """

    # build a submenu for each node category
    #
    global _aiCreateCustomNodeProc
    for (staticClass, runtimeClass, nodePath, nodeTypes) in getTypeInfo():
        # skip unclassified
        if staticClass == 'arnold/shader':
            continue
        cmds.menuItem(label = nodePath.replace('/', ' '), 
                      tearOff = True, subMenu = True)
        
        # call buildCreateSubMenu() to create the menu entries.  The specified 
        # creation command is aiCreateCustomNode runtimeClassification.  The
        # buildCreateSubMenu will append to that argument list the name of the
        # node type, thereby completing the correct argument list for the 
        # creation routine.
        #
        mel.eval('''buildCreateSubMenu("%s", "%s %s \\"\\"");''' % (staticClass,
                                                                    _aiCreateCustomNodeProc,
                                                                    runtimeClass))
        cmds.setParent('..', menu=True)

def aiCreateCustomNode(runtimeClassification, postCommand , nodeType):
    flag = {runtimeClassification:True}
    node = cmds.shadingNode(nodeType, **flag)
    return node

def isClassified(node, eval):
    nodeType = cmds.nodeType(node)
    classification = cmds.getClassification(nodeType)
    for c in classification:
        if eval in c:
            return True
    return False

def setup():
    
    global _aiCreateCustomNodeProc
    _aiCreateCustomNodeProc = utils.pyToMelProc(aiCreateCustomNode, 
                                                ('string', 'runtimeClassification'),
                                                ('string', 'postCommand'),
                                                ('string', 'nodeType'),
                                                ('return', 'string'))
    
    treeListerMelProc = utils.pyToMelProc(createTreeListerContent,
                                          ('string', 'renderNodeTreeLister'),
                                          ('string', 'postCommand'),
                                          ('string', 'filterString'), shortName=True)
    overrides = """
    global proc string createRenderNodeTreeLister(string $postCommand, string $filterString)
    //
    // Description:  This procedure is to create a new treeLister for render nodes.
    //
    {
        string $renderNodeTreeLister = `nodeTreeLister`;
        %s($renderNodeTreeLister, $postCommand, $filterString);
        buildRenderNodeTreeListerContent($renderNodeTreeLister, $postCommand, $filterString);
        return $renderNodeTreeLister;
    }
    
    global proc refreshRenderNodeTreeLister(string $renderNodeTreeLister, string $postCommand, string $filterString)
    //
    // Description:  This procedure is to refresh a render node treeLister
    //               by clearing its contents and repopulating it.
    //
    {
        treeLister -e -clearContents $renderNodeTreeLister;
        %s($renderNodeTreeLister, $postCommand, $filterString);
        buildRenderNodeTreeListerContent($renderNodeTreeLister, $postCommand, $filterString);
    }
    """ % (treeListerMelProc, treeListerMelProc)
    mel.eval(overrides)

    #HACKY AS ALL FUCK SORRY!!!  This will pull out the hyperShadePanelBuildCreateMenu procedure
    #and insert into it custom code that will call our python function to add our arnold nodes
    #in.
    buildMenuScript = utils.findMelScript('hyperShadePanelBuildCreateMenu')
    index = next((i for i in xrange(len(buildMenuScript)) if 'if (`pluginInfo -query -loaded Mayatomr`)' in buildMenuScript[i]), None)
    if index:
        createMenuMelProc = utils.pyToMelProc(aiHyperShadeCreateMenu_BuildMenu)
        aiBuildScript = '''
        if (`pluginInfo -query -loaded mtoa`)
        {
            %s();
            menuItem -divider true;
        }
        ''' % createMenuMelProc
        buildMenuScript.insert(index, aiBuildScript)
        buildMenuScript = ''.join(buildMenuScript)
        mel.eval(buildMenuScript)
        
        #Maya UI scripts are so poorly written that we have to actually take this procedure and redeclare it global
        #otherwise the hyperShadePanelBuildCreateMenu fails...
        overrides = """       
        global proc buildCreateNodesSubMenu(string $types[], string $callback)
        {
            //
            // Description:
            //    This procedure is called from buildMainMenu().
            //    This procedure builds menu items to create nodes of the nodeTypes
            //    specified in $types[].
            //    The specified callback script is the one which is called to do the
            //    creation of a node of a particular type.
            //
    
            string $annotMsg = (uiRes("m_hyperShadePanel.kAnnotMsg"));
            for($type in $types) 
            {
                    string $typeString = `nodeTypeNiceName $type`;
                    menuItem 
                            -label $typeString
                            -annotation `format -s $typeString $annotMsg` 
                            -command ($callback + \"(\\"\" + $type + \"\\")\");
            }
        }
        """
        mel.eval(overrides)

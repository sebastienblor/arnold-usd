import pymel.core as pm
import mtoa.aovs as aovs
import mtoa.ui.ae.shapeTemplate as templates
import mtoa.ui.ae.shaderTemplate as shaderTemplate
import mtoa.ui.aoveditor as aoveditor
import mtoa.callbacks as callbacks
import mtoa.utils as utils
from collections import defaultdict

#------------------------------------------
# Shading Group
#------------------------------------------

def getAOVsInNetwork(rootNode):
    '''
    returns a map from PyNode to aovs
    
    the aov list contains all registered aovs for that node type, regardless of whether
    they are currently enabled in the globals 
    '''
    results = {}
    for node in pm.listHistory(rootNode, pruneDagObjects=True):
        # TOOD: cache this result
        results[node] = [node.attr(at).get() for (aov, at, type) in aovs.getNodeGlobalAOVData(node.type())]
    return results

class ShadingEngineTemplate(templates.AttributeEditorTemplate):
    def __init__(self, nodeType):
        self._msgCtrls = []
        aovs.addAOVChangedCallback(self.update, 'ShadingEngineTemplate')
        self.networkCol = None
        self.otherCol = None

        # populated by updateNetworkData()
        self.networkData = None # mapping from node -> aov list
        self.networkAOVs = None # set of all possible aovs in network, regardless of whether they are active
        self.networkNodeTypes =None # set of node types in the shading network 
        self.aovNodes = None # reverse lookup to networkData:  aovName -> node list

        super(ShadingEngineTemplate, self).__init__(nodeType)

    def setup(self):
        self.addCustom("aiCustomAOVs", self.buildNetworkAOVBrowser, self.updateNetworkAOVBrowser)


    def update(self):
        if self.nodeName is None or not pm.objExists(self.nodeName) \
            or self.networkCol is None or not pm.layout(self.networkCol, exists=True):
            return

        nodeAttr = pm.Attribute(self.nodeAttr('aiCustomAOVs'))
        for aov in aovs.getAOVs():
            # FIXME- delay setting the aov name attribute until something is connected
            #cb = utils.pyToMelProc(pm.Callback(nodeAttr[aov.index].aovName.set, aov.name))
            at = nodeAttr[aov.index]
            at.aovName.set(aov.name)

    def updateNetworkData(self):
        self.networkData = getAOVsInNetwork(self.nodeAttr('surfaceShader'))
        self.networkAOVs = set(aovs.getBuiltinAOVs()) # builtins are always in network
        self.networkNodeTypes = set([])
        self.aovNodes = defaultdict(list)
        for node, aovList in self.networkData.iteritems():
            self.networkAOVs.update(aovList)
            self.networkNodeTypes.add(node.type())
            for aov in aovList:
                self.aovNodes[aov].append(node)

    def buildNetworkAOVBrowser(self, nodeAttr):
        # TODO: move this into AttributeEditorTemplate
        self._setActiveNodeAttr(nodeAttr)
        self.updateNetworkData()

        aovList = aovs.getAOVs()

        pm.setUITemplate('attributeEditorTemplate', pushTemplate=True)
    
        pm.cmds.frameLayout(label='AOVs in Shading Network', collapse=False)
        pm.cmds.columnLayout(adjustableColumn=True)
        
        pm.cmds.rowLayout(nc=2)
        pm.cmds.text(label='')
        pm.cmds.button(label='AOV Browser',
                  c=lambda *args: aoveditor.arnoldAOVBrowser(listAOVGroups=False,
                                                             nodeTypes=self.networkNodeTypes))
        pm.setParent('..') # rowLayout

        pm.cmds.frameLayout(labelVisible=False, collapsable=False)
        self.networkCol = pm.cmds.columnLayout(adjustableColumn=True)
        self.buildNetworkAOVs(nodeAttr, aovList)
        pm.setParent('..') # columnLayout
        pm.setParent('..') # frameLayout

        pm.setParent('..') # columnLayout
        pm.setParent('..') # frameLayout

        pm.cmds.frameLayout(label='Other AOVs', collapse=False)
        pm.cmds.columnLayout(adjustableColumn=True)

        pm.cmds.rowLayout(nc=2)
        pm.cmds.text(label='')
        pm.cmds.button(label='Add Custom', c=lambda *args: shaderTemplate.newAOVPrompt())
        pm.setParent('..') # rowLayout

        pm.cmds.frameLayout(labelVisible=False, collapsable=False)
        self.otherCol = pm.cmds.columnLayout(adjustableColumn=True)
        self.buildOtherAOVs(nodeAttr, aovList)
        pm.setParent('..') # columnLayout
        pm.setParent('..') # frameLayout

        pm.setParent('..') # columnLayout
        pm.setParent('..') # frameLayout
        pm.setUITemplate('attributeEditorTemplate', popTemplate=True)

    def updateNetworkAOVBrowser(self, nodeAttr):
        # TODO: move this into AttributeEditorTemplate
        self._setActiveNodeAttr(nodeAttr)
        self.updateNetworkData()
        for ctrl in self._msgCtrls:
            pm.deleteUI(ctrl)
        self._msgCtrls = []

        pm.setUITemplate('attributeEditorTemplate', pushTemplate=True)

        aovList = aovs.getAOVs()
        pm.setParent(self.networkCol)
        self.buildNetworkAOVs(nodeAttr, aovList)

        pm.setParent(self.otherCol)
        self.buildOtherAOVs(nodeAttr, aovList)

        pm.setUITemplate('attributeEditorTemplate', popTemplate=True)

    def buildNetworkAOVs(self, nodeAttr, aovList):
        '''
        Populate the UI with an attrNavigationControlGrp for each AOV in the network
        '''
        nodeAttr = pm.Attribute(nodeAttr)
        for aov in aovList:
            if aov.name in self.networkAOVs:
                at = nodeAttr[aov.index]
                at.aovName.set(aov.name)
                ctrl = pm.attrNavigationControlGrp(at=at.aovInput,
                                                   label=aov.name)
                self._msgCtrls.append(ctrl)
                pm.popupMenu(parent=ctrl);
                pm.menuItem(subMenu=True, label="Goto Node")
                for node in self.aovNodes[aov.name]:
                    pm.cmds.menuItem(label=node.name(), command=lambda arg, node=node: pm.select(node))

    def buildOtherAOVs(self, nodeAttr, aovList):
        '''
        Populate the UI with an attrNavigationControlGrp for each AOV not in the network
        '''
        nodeAttr = pm.Attribute(nodeAttr)
        for aov in aovList:
            if aov.name not in self.networkAOVs:
                at = nodeAttr[aov.index]
                at.aovName.set(aov.name)
                self._msgCtrls.append(pm.cmds.attrNavigationControlGrp(at=at.aovInput.name(),
                                                                       label=aov.name))


templates.registerAETemplate(ShadingEngineTemplate, "shadingEngine")


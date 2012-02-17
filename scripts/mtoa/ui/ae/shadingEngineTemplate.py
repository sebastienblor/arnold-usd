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

def createCallback(attr, aovName):
    print "running"

def getAOVsInNetwork(rootNode):
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
        super(ShadingEngineTemplate, self).__init__(nodeType)

    def setup(self):
        self.addCustom("aiCustomAOVs", self.buildNetworkAOVBrowser, self.updateNetworkAOVBrowser)


    def update(self):
        print "ShadingEngineTemplate.update"
        if self.nodeName is None or not pm.objExists(self.nodeName) \
            or self.networkCol is None or not pm.layout(self.networkCol, exists=True):
            return
        #self.updateCustomAOVArray()
        nodeAttr = pm.Attribute(self.nodeAttr('aiCustomAOVs'))
        for aov in aovs.getAOVs():
            # FIXME- delay setting the aov name attribute until something is connected
            #cb = utils.pyToMelProc(pm.Callback(nodeAttr[aov.index].aovName.set, aov.name))
            at = nodeAttr[aov.index]
            at.aovName.set(aov.name)

    def updateNetworkData(self):
        self.networkData = getAOVsInNetwork(self.nodeAttr('surfaceShader'))
        self.networkAOVs = set([])
        self.networkNodeTypes = set([])
        self.aovNodes = defaultdict(list)
        for node, aovs in self.networkData.iteritems():
            self.networkAOVs.update(aovs)
            self.networkNodeTypes.add(node.type())
            for aov in aovs:
                self.aovNodes[aov].append(node)

    def buildNetworkAOVBrowser(self, nodeAttr):
        # TODO: move this into AttributeEditorTemplate
        self._setActiveNodeAttr(nodeAttr)
        self.updateNetworkData()
        with pm.uitypes.UITemplate('attributeEditorTemplate'):
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
            self.buildNetworkAOVs(nodeAttr)
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
            self.buildOtherAOVs(nodeAttr)
            pm.setParent('..') # columnLayout
            pm.setParent('..') # frameLayout

            pm.setParent('..') # columnLayout
            pm.setParent('..') # frameLayout

    def updateNetworkAOVBrowser(self, nodeAttr):
        # TODO: move this into AttributeEditorTemplate
        self._setActiveNodeAttr(nodeAttr)
        self.updateNetworkData()
        for ctrl in self._msgCtrls:
            pm.deleteUI(ctrl)
        self._msgCtrls = []
        with pm.uitypes.UITemplate('attributeEditorTemplate'):
            pm.setParent(self.networkCol)
            self.buildNetworkAOVs(nodeAttr)
            pm.setParent(self.otherCol)
            self.buildOtherAOVs(nodeAttr)

    def buildNetworkAOVs(self, nodeAttr):
        nodeAttr = pm.Attribute(nodeAttr)
        for aovName, aovList in aovs.getAOVs(group=True):
            if aovName in self.networkAOVs:
                aov = aovList[0]
                at = nodeAttr[aov.index]
                at.aovName.set(aov.name)
                ctrl = pm.attrNavigationControlGrp(at=at.aovInput,
                                                   label=aov.name)
                self._msgCtrls.append(ctrl)
                pm.popupMenu(parent=ctrl);
                pm.menuItem(subMenu=True, label="Goto Node")
                for node in self.aovNodes[aov.name]:
                    pm.cmds.menuItem(label=node.name(), command=lambda arg, node=node: pm.select(node))

    def buildOtherAOVs(self, nodeAttr):
        nodeAttr = pm.Attribute(nodeAttr)
        for aov in aovs.getAOVs():
            if aov.name not in self.networkAOVs:
                at = nodeAttr[aov.index]
                at.aovName.set(aov.name)
                self._msgCtrls.append(pm.cmds.attrNavigationControlGrp(at=at.aovInput.name(),
                                                                       label=aov.name))

    def buildCustomAOVArray(self, nodeAttr):
        nodeAttr = pm.Attribute(nodeAttr)

        conn = pm.listConnections(self.nodeAttr('surfaceShader'), s=True, d=False, p=True)
        if conn: 
            usedAOVs = set(pm.cmds.arnoldPlugins(listAOVs=True, nodePlug=conn[0].name()))
        else:
            usedAOVs = set([])
        for aov in aovs.getAOVs():
            # FIXME- delay setting the aov name attribute until something is connected
            #cb = utils.pyToMelProc(pm.Callback(nodeAttr[aov.index].aovName.set, aov.name))
            at = nodeAttr[aov.index]
            at.aovName.set(aov.name)
            self._msgCtrls.append(pm.cmds.attrNavigationControlGrp(at=at.aovInput.name(),
                                                                   label=aov.name,
                                                                   enable=aov.name not in usedAOVs))

    def updateCustomAOVArray(self, nodeAttr):
        print "updateCustomAOVArray", nodeAttr

        for ctrl in self._msgCtrls:
            pm.deleteUI(ctrl)
        self._msgCtrls = []
        self.buildCustomAOVArray(nodeAttr)

templates.registerAETemplate(ShadingEngineTemplate, "shadingEngine")


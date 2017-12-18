import maya.cmds as cmds
import mtoa.aovs as aovs
import mtoa.ui.ae.templates as templates
import mtoa.ui.ae.shaderTemplate as shaderTemplate
import mtoa.ui.aoveditor as aoveditor
from collections import defaultdict

#------------------------------------------
# Shading Group
#------------------------------------------

def getAOVsInNetwork(rootNode):
    '''
    returns a map from nodes to aovs
    
    the aov list contains all registered aovs for that node type, regardless of whether
    they are currently enabled in the globals 
    '''
    results = {}
    for node in cmds.listHistory(rootNode, pruneDagObjects=True):

        nodeType = cmds.nodeType(node)
        # TODO: Create a general method to assign AOVs of hidden nodes
        # lambert shader has only translator registered so this is a Lambert Maya node and does not have an AOV tab
        if nodeType == "lambert":
            results[node] = [u'direct_diffuse', u'indirect_diffuse']
        else:
            # TODO: cache this result
            results[node] = [cmds.getAttr('{}.{}'.format(node, at)) for (aov, at, type) in aovs.getNodeGlobalAOVData(nodeType)]
    return results

class ShadingEngineTemplate(templates.AttributeTemplate):
    def __init__(self, nodeType):
        self._msgCtrls = []
        aovs.addAOVChangedCallback(self.update, 'ShadingEngineTemplate')
        self.networkCol = None
        self.otherCol = None

        # populated by updateNetworkData()
        self.networkData = {} # mapping from node -> aov list
        self.networkAOVs = set([]) # set of all possible aovs in network, regardless of whether they are active
        self.networkNodeTypes = set([]) # set of node types in the shading network 
        self.aovNodes = {} # reverse lookup to networkData:  aovName -> node list
        
        # populated by updateCustomArrayData()
        self.nameToAttr = {} # mapping from aov name to element plug on aiCustomAOVs 
        #self.arrayIndices = set([])  # set of all indices used by aiCustomAOVs
        #self.orphanedAOVs = set([]) # set of aov names that appear in aiCustomAOVs that are not in the globals
        self.nextIndex = 0

        super(ShadingEngineTemplate, self).__init__(nodeType)
        
    def surfaceShaderCreate(self, attrName):
        cmds.columnLayout()
        cmds.attrNavigationControlGrp("ShadingEngineSurfaceShader", label = "Surface Shader",
                            attribute=attrName)
        cmds.setParent('..')
        
    def surfaceShaderUpdate(self, attrName):
        cmds.attrNavigationControlGrp("ShadingEngineSurfaceShader", edit=True, attribute=attrName)

    def volumeShaderCreate(self, attrName):
        cmds.columnLayout()
        cmds.attrNavigationControlGrp('ShadingEngineVolumeShader', label = 'Volume Shader',
                                     attribute=attrName)
        cmds.setParent('..')

    def volumeShaderUpdate(self, attrName):
        cmds.attrNavigationControlGrp('ShadingEngineVolumeShader', edit=True, attribute=attrName)

    def setup(self):
        self.addCustom("aiSurfaceShader", self.surfaceShaderCreate, self.surfaceShaderUpdate)
        self.addCustom("aiVolumeShader", self.volumeShaderCreate, self.volumeShaderUpdate)
        self.addCustom("aiCustomAOVs", self.buildAOVFrame, self.updateAOVFrame)

    def update(self):
        if self.nodeName is None or not cmds.objExists(self.nodeName) \
            or self.networkCol is None or not cmds.layout(self.networkCol, exists=True):
            return

        nodeAttr = self.nodeAttr('aiCustomAOVs')
        self.updateAOVFrame(nodeAttr)

    def getAOVAttr(self, nodeAttr, aovName):
        '''
        given an aov name, return the corresponding attribute in the aiCustomAOVs array,
        or make a new one if it does not yet exist
        '''

        try:
            return self.nameToAttr[aovName]
        except KeyError:
            values = cmds.getAttr(nodeAttr, mi=True) or []
            values.append(aovName)
            cmds.setAttr(nodeAttr, values, type=stringArray)
            aovList = aovs.getAOVs()
            self.updateCustomArrayData(nodeAttr, aovList)
            return at

    def updateCustomArrayData(self, nodeAttr, aovList):
        '''
        set three data structures regarding the shadingEngine aiCustomAOVs attribute:
            - mapping from aov name to element plug on aiCustomAOVs 
            - set of all indices used by aiCustomAOVs
            - set of aov names that appear in aiCustomAOVs that are not in the globals
        '''
        self.nameToAttr, self.nextIndex = aovs.getShadingGroupAOVMap(str(nodeAttr))
        #self.arrayIndices = set([at.index() for at in self.nameToAttr.values()])
        #self.orphanedAOVs = set(self.nameToAttr.keys()).difference([aov.name for aov in aovList])

    def updateNetworkData(self):
        self.networkData = getAOVsInNetwork(self.nodeAttr('surfaceShader'))
        self.networkAOVs = set(aovs.getBuiltinAOVs()) # builtins are always in network
        self.networkNodeTypes = set([])
        self.aovNodes = defaultdict(list)
        for node, aovList in self.networkData.iteritems():
            self.networkAOVs.update(aovList)
            self.networkNodeTypes.add(cmds.nodeType(node))
            for aov in aovList:
                self.aovNodes[aov].append(node)

    def buildAOVFrame(self, nodeAttr):
        # TODO: move this into AttributeEditorTemplate
        self._setActiveNodeAttr(nodeAttr)
        
        aovList = aovs.getAOVs()
        self.updateNetworkData()
        self.updateCustomArrayData(nodeAttr, aovList)

        cmds.setUITemplate('attributeEditorTemplate', pushTemplate=True)

        cmds.frameLayout(label='AOVs', collapse=False)
        cmds.columnLayout(adjustableColumn=True)

        cmds.rowLayout(nc=2)
        cmds.text(label='')
        cmds.button(label='Add Custom AOV', c=lambda *args: shaderTemplate.newAOVPrompt())
        cmds.setParent('..') # rowLayout

        cmds.frameLayout(labelVisible=False, collapsable=False)
        self.otherCol = cmds.columnLayout(adjustableColumn=True)
        self.buildOtherAOVs(nodeAttr, aovList)
        cmds.setParent('..') # columnLayout
        cmds.setParent('..') # frameLayout

        cmds.setParent('..') # columnLayout
        cmds.setParent('..') # frameLayout
        
        cmds.setParent('..') # columnLayout
        cmds.setParent('..') # frameLayout
        cmds.setUITemplate('attributeEditorTemplate', popTemplate=True)

    def updateAOVFrame(self, nodeAttr):
        # TODO: move this into AttributeEditorTemplate
        self._setActiveNodeAttr(nodeAttr)
        
        self.updateNetworkData()
        for ctrl in self._msgCtrls:
            cmds.deleteUI(ctrl)
        self._msgCtrls = []

        cmds.setUITemplate('attributeEditorTemplate', pushTemplate=True)

        aovList = aovs.getAOVs()
        self.updateCustomArrayData(nodeAttr, aovList)

        cmds.setParent(self.networkCol)
        self.buildNetworkAOVs(nodeAttr, aovList)

        cmds.setParent(self.otherCol)
        self.buildOtherAOVs(nodeAttr, aovList)

        cmds.setUITemplate('attributeEditorTemplate', popTemplate=True)

    def buildNetworkAOVs(self, nodeAttr, aovList):
        '''
        Populate the UI with an attrNavigationControlGrp for each AOV in the network
        '''
        for aov in aovList:
            if aov.name in self.networkAOVs:
                at = self.getAOVAttr(nodeAttr, aov.name)
                #at = nodeAttr[aov.index]
                #at.aovName.set(aov.name)

                attrName = '{}.aovInput'.format(at)
                ctrl = cmds.attrNavigationControlGrp(at=attrName,
                                                   label=aov.name,
                                     cn="createRenderNode -allWithShadersUp \"defaultNavigation -force true -connectToExisting -source %node -destination "+attrName+"\" \"\"")

                self._msgCtrls.append(ctrl)
                cmds.popupMenu(parent=ctrl);
                cmds.menuItem(subMenu=True, label="Goto Node")
                for node in self.aovNodes[aov.name]:
                    cmds.menuItem(label=node, command=lambda arg, node=node: cmds.select(node))

    def buildOtherAOVs(self, nodeAttr, aovList):
        '''
        Populate the UI with an attrNavigationControlGrp for each AOV not in the network
        '''
        for aov in aovList:
            if aov.name not in self.networkAOVs:
                at = self.getAOVAttr(nodeAttr, aov.name)
                attrName = '{}.aovInput'.format(at)
                ctrl = cmds.attrNavigationControlGrp(at=attrName,
                                                        label=aov.name, cn="createRenderNode -allWithShadersUp \"defaultNavigation -force true -connectToExisting -source %node -destination "+attrName+"\" \"\"")
                self._msgCtrls.append(ctrl)


templates.registerAETemplate(ShadingEngineTemplate, "shadingEngine")


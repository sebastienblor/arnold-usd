import pymel.core as pm
import mtoa.aovs as aovs
from mtoa.ui.ae.shaderTemplate import ShaderAETemplate

class AEaiAOVTemplate(ShaderAETemplate):

    def defaultValueNew(self, nodeAttr):
        pm.attrNavigationControlGrp('aiAOVDefaultValue',
                                    label='Shader Output',
                                    at=nodeAttr, cn="createRenderNode -allWithShadersUp \"defaultNavigation -force true -connectToExisting -source %node -destination "+nodeAttr+"\" \"\"")

    def defaultValueReplace(self, nodeAttr):
        pm.attrNavigationControlGrp('aiAOVDefaultValue', edit=True, at=nodeAttr, cn="createRenderNode -allWithShadersUp \"defaultNavigation -force true -connectToExisting -source %node -destination "+nodeAttr+"\" \"\"")

    def outputsNew(self, attr):
        node, plug = attr.split('.', 1)
        attr = pm.Attribute(attr)
        self.frame = pm.mel.AEnewNonNumericMulti(node,
                                                 plug,
                                                 "AOV Outputs",
                                                 "", "AEnewCompound",
                                                 attr.getArrayIndices())

    def outputsReplace(self, attr):
        node, plug = attr.split('.', 1)
        attr = pm.Attribute(attr)
        #pm.setParent(self.frame)
        pm.mel.AEreplaceNonNumericMulti(self.frame,
                                        node,
                                        plug,
                                        "", "AEreplaceCompound",
                                        attr.getArrayIndices())

    def updateLightGroupsVisibility(self, nodeName):
        nameAttr = '%s.%s' % (nodeName, 'name')
        nameValue = pm.getAttr(nameAttr)

        aovLightingList = aovs.getLightingAOVs()
        lightGroupsAttr = (nameValue in aovLightingList)

        pm.editorTemplate(dimControl=(nodeName, 'globalAov', not lightGroupsAttr))
        pm.editorTemplate(dimControl=(nodeName, 'lightGroups', not lightGroupsAttr))
        pm.editorTemplate(dimControl=(nodeName, 'lightGroupsList', not lightGroupsAttr))

        builtinAOVs = aovs.getBuiltinAOVs()
        customAOV = not (nameValue in builtinAOVs)

        pm.editorTemplate(dimControl=(nodeName, 'defaultValue', not customAOV))
        pm.editorTemplate(dimControl=(nodeName, 'lightPathExpression', not customAOV))
        
         

    def setup(self):
        #mel.eval('AEswatchDisplay "%s"' % nodeName)

        self.beginScrollLayout()
        self.beginLayout("AOV Attributes", collapse=False)

        #self.beginLayout("Primary Controls", collapse=False)
        self.addControl('enabled')
        self.addControl('name', changeCommand=self.updateLightGroupsVisibility)
        self.addControl('type', label='Data Type')

        self.beginLayout("Light Groups", collapse=False)
        self.beginNoOptimize()

        self.addControl('globalAov', label='Global AOV')
        self.addControl('lightGroups', label='All Light Groups')
        self.addControl('lightGroupsList', label='Light Groups List')
        self.endNoOptimize()
        self.endLayout()
        self.beginLayout("Custom AOV", collapse=False)
        self.addCustom('defaultValue', self.defaultValueNew, self.defaultValueReplace)
        self.addSeparator()
        self.addControl('lightPathExpression', label='Light Path Expression')
        self.endLayout()
        self.endLayout()

        
        self.addCustom('outputs', self.outputsNew, self.outputsReplace)

        # include/call base class/node attributes
        pm.mel.AEdependNodeTemplate(self.nodeName)
        self.addExtraControls()

        self.endScrollLayout()
        
        # attributes left to allow easier upgrading. will be removed
        self.suppress('imageFormat')
        self.suppress('filterType')
        self.suppress('prefix')

        self.updateLightGroupsVisibility(self.nodeName)

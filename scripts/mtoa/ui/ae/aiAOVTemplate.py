import pymel.core as pm
import mtoa.aovs as aovs
import maya.cmds as cmds
from mtoa.ui.ae.shaderTemplate import ShaderAETemplate
import mtoa.ui.ae.utils as aeUtils

class AEaiAOVTemplate(ShaderAETemplate):

    def defaultValueNew(self, nodeAttr):
        pm.attrNavigationControlGrp('aiAOVDefaultValue',
                                    label='Shader',
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

    def addTokenLPE(self, value):
        attr = self.nodeName + '.lightPathExpression'
        previousLPE = pm.getAttr(attr)
        newLPE = previousLPE + value
        pm.setAttr(attr, newLPE)

    def createLPE(self, attr):
        tokens = attr.split('.')
        nodeName = tokens[0]        

        controlName = 'CtrlLightPathExpression'
        cmds.setUITemplate('attributeEditorPresetsTemplate', pushTemplate=True)
        aeUtils.attrTextFieldGrp(controlName, attribute=attr, label='Light Path Expression <span>&#8801;</span>')
        
        attrChildren = pm.layout(controlName, query=True, childArray=True)
        pm.popupMenu(button=1, parent=attrChildren[0])

        lpeTokens=[ 
            ["Camera                                                                        C" , "C"],
            ["Light Emission                                                            L" , "L"],
            ["Light Emission from 'default' LightGroup               <L.'default'>" , "<L.'default'>"],
            ["Surface & Volume Emission                                         O" , "O"],
            ["Background Emission                                                B" , "B"],
            ["Albedo                                                                         A" , "A"],
            ["Diffuse Reflection & Transmission or Subsurface      D" , "D"],
            ["Diffuse Reflection                                                      RD" , "RD"],
            ["Diffuse Transmission or Subsurface                         TD" , "TD"],
            ["Specular Reflection or Refraction                            S" , "S"],
            ["Specular Reflection                                                   <RS>" , "<RS>"],
            ["Specular Refraction                                                   <TS>" , "<TS>"],
            ["Volume Scattering                                                      V" , "V"],
            ["Or                                                                                 |" , "|"],
            ["Not                                                                               ^" , "^"],
            ["0 or more events                                                         *" , "*"],
            ["1 or more events                                                         +" , "+"],
            ["Any event                                                                     ." , "."],
            ["Custom event                                                              myevent" , "myevent"],
            ["<builtin> RGBA                                                          C.*" , "C.*"],
            ["<builtin> direct                                                         C[DSV]L" , "C[DSV]L"],
            ["<builtin> indirect                                                      C[DSV][DSVOB].*" , "C[DSV][DSVOB].*"],
            ["<builtin> emission                                                    C[LO]" , "C[LO]"],
            ["<builtin> background                                              CB" , "CB"],
            ["<builtin> diffuse                                                       C<RD>.*" , "C<RD>.*"],
            ["<builtin> specular                                                     C<RS[^'coat']>.*" , "C<RS[^'coat']>.*"],
            ["<builtin> coat                                                            C<RS'coat'>.*" , "C[DSV]L"],
            ["<builtin> transmission                                             C<TS>.*" , "C<TS>.*"],
            ["<builtin> sss                                                              C<TD>.*" , "C<TD>.*"],
            ["<builtin> volume                                                      CV.*" , "CV.*"],
            ["<builtin> albedo                                                      C[DSV]A" , "C[DSV]A"],
            ["<builtin> diffuse_direct                                           C<RD>L", "C<RD>L"],
            ["<builtin> diffuse_indirect                                        C<RD>[DSVOB].*", "C<RD>[DSVOB].*"],
            ["<builtin> diffuse_albedo                                         C<RD>A", "C<RD>A"],
            ["<builtin> specular_direct                                         C<RS[^'coat']>L" , "C<RS[^'coat']>L"],
            ["<builtin> specular_indirect                                      C<RS[^'coat']>[DSVOB].*" , "C<RS[^'coat']>[DSVOB].*"],
            ["<builtin> specular_albedo                                       C<RS[^'coat']>A" , "C<RS[^'coat']>A"],
            ["<builtin> coat_direct                                                C<RS'coat'>L" , "C<RS'coat'>L"],
            ["<builtin> coat_indirect                                             C<RS'coat'>[DSVOB].*" , "C<RS'coat'>[DSVOB].*"],
            ["<builtin> coat_albedo                                              C<RS'coat'>A" , "C<RS'coat'>A"],
            ["<builtin> transmission_direct                                  C<TS>L" , "C<TS>L"],
            ["<builtin> transmission_indirect                               C<TS>[DSVOB].*" , "C<TS>[DSVOB].*"],
            ["<builtin> transmission_albedo                                C<TS>A" , "C<TS>A"],
            ["<builtin> sss_direct                                                   C<TD>L" , "C<TD>L"],
            ["<builtin> sss_indirect                                               C<TD>[DSVOB].*" , "C<TD>[DSVOB].*"],
            ["<builtin> sss_albedo                                                C<TD>A" , "C<TD>A"],
            ["<builtin> volume_direct                                           CVL" , "CVL"],
            ["<builtin> volume_indirect                                        CV[DSVOB].*" , "CV[DSVOB].*"],
            ["<builtin> volume_albedo                                         CVA" , "CVA"],
        ]
        
        for lpeToken in lpeTokens:
            pm.menuItem(label=lpeToken[0], annotation=lpeToken[1], command=pm.Callback(self.addTokenLPE, lpeToken[1]))

            if lpeTokens[1] == "V" or lpeTokens[1] == "A":
                pm.menuItem(divider=True)
        
        cmds.setUITemplate(popTemplate=True)        

    def updateLPE(self, attr):
        tokens = attr.split('.')
        nodeName = tokens[0]       
        controlName = 'CtrlLightPathExpression'
        aeUtils.attrTextFieldGrp(controlName, edit=True, attribute=attr)


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
        
        self.addCustom("lightPathExpression", self.createLPE, self.updateLPE)
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

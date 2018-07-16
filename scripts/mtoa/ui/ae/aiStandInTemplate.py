import re
import maya.cmds as cmds
import maya.mel as mel
import os.path
import mtoa.melUtils as mu
from mtoa.ui.ae.utils import aeCallback
import mtoa.core as core
from mtoa.ui.ae.shaderTemplate import ShaderAETemplate
import arnold as ai

CACHE_ATTR = 'ai_asscache'


def LoadStandInButtonPush(attrName):
    basicFilter = 'Arnold Archive (*.ass *.ass.gz *.obj *.ply);;Arnold Procedural (*.so *.dll *.dylib)'
    defaultDir = cmds.workspace(query=True, directory=True)
    currentDir = cmds.getAttr(attrName) or ''
    currentDir = os.path.dirname(currentDir)
    if os.path.exists(currentDir):
        defaultDir = currentDir

    ret = cmds.fileDialog2(fileFilter=basicFilter, cap='Load StandIn',okc='Load',fm=1, startingDirectory=defaultDir)
    if ret is not None and len(ret):
        cmds.setAttr(attrName.replace('.dso', '.useFrameExtension'), False) # I picked one file, no file sequence
        ArnoldStandInDsoEdit(attrName, ret[0])

def ArnoldStandInDsoEdit(attrName, mPath) :
    cmds.setAttr(attrName, mPath, type='string')

def ArnoldStandInTemplateDsoNew(attrName) :
    cmds.rowColumnLayout( numberOfColumns=3, columnAlign=[(1, 'right'),(2, 'right'),(3, 'left')], columnAttach=[(1, 'right', 0), (2, 'both', 0), (3, 'left', 5)], columnWidth=[(1,145),(2,220),(3,30)] )
    cmds.text(label='Path ')
    path = cmds.textField('standInDsoPath',changeCommand=lambda *args: ArnoldStandInDsoEdit(attrName, *args))
    cmds.textField( path, edit=True, text=cmds.getAttr(attrName) )
    cmds.symbolButton('standInDsoPathButton', image='navButtonBrowse.png', command=lambda arg=None, x=attrName: LoadStandInButtonPush(x))

def ArnoldStandInTemplateDsoReplace(attrName) :
    cmds.textField( 'standInDsoPath', edit=True, changeCommand=lambda *args: ArnoldStandInDsoEdit(attrName, *args))
    cmds.textField( 'standInDsoPath', edit=True, text=cmds.getAttr(attrName) )
    cmds.symbolButton('standInDsoPathButton', edit=True, image='navButtonBrowse.png' , command=lambda arg=None, x=attrName: LoadStandInButtonPush(x))


def ArnoldStandInUpdateUI(attrName) :
    overrideVisAttrs = ['overridePrimaryVisibility', 
                     'overrideCastsShadows', 
                     'overrideVisibleInDiffuseReflection',
                     'overrideVisibleInSpecularReflection',
                     'overrideVisibleInDiffuseTransmission', 
                     'overrideVisibleInSpecularTransmission',
                     'overrideVisibleInVolume']
    visAttrs =      ['primaryVisibility', 
                     'castsShadows', 
                     'aiVisibleInDiffuseReflection',
                     'aiVisibleInSpecularReflection',
                     'aiVisibleInDiffuseTransmission', 
                     'aiVisibleInSpecularTransmission',
                     'aiVisibleInVolume']

    for i in range(len(overrideVisAttrs)):
        if cmds.getAttr(attrName + overrideVisAttrs[i]) == 0:
            cmds.setAttr(attrName + overrideVisAttrs[i], 1)
            if cmds.getAttr(attrName + visAttrs[i]) == 0:
                cmds.setAttr(attrName + visAttrs[i], 1)
    
        
class AEaiStandInTemplate(ShaderAETemplate):

    def updateAssFile(self):
        # clear the cache
        self.assItems = []
        cmds.setAttr('{}.{}'.format(self.nodeName, CACHE_ATTR), 0, type="stringArray")

        cmds.treeView(self.assInfoPath, edit=True, visible=False)
        cmds.button(self.inspectAssPath, edit=True, visible=True)
        cmds.treeView(self.assInfoPath, edit=True, removeAll=True)

    def selectElement(self, itemName, itemValue):
        if itemValue == 0:
            if itemName in self.selectedItems:
                self.selectedItems.remove(itemName)
        else:
            self.selectedItems.append(itemName)

        attrVal = ''
        for sel in self.selectedItems:
            if len(attrVal) > 0:
                attrVal = ',' + attrVal
            attrVal = attrVal + sel

        selAttr = self.nodeName + ".selectedItems"
        cmds.setAttr(selAttr, attrVal, type='string')

        return True

    def fileInfoNew(self, nodeAttr) :
        cmds.rowLayout(nc=2)
        cmds.text(label='')
        self.inspectAssPath = cmds.button(align="center", label='Inspect Arnold File', command=lambda *args: self.inspectFile())
        cmds.setParent('..') # rowLayout
        self.assInfoPath = cmds.treeView(height=300, numberOfButtons=0, allowReparenting=False, selectCommand=self.selectElement)
        self.fileInfoReplace(nodeAttr)

        fileAttr = self.nodeName + ".dso"
        cmds.scriptJob(attributeChange=[fileAttr,self.updateAssFile],
                     replacePrevious=True, parent=self.inspectAssPath)

    def fileInfoReplace(self, nodeAttr) :
        if not cmds.attributeQuery(CACHE_ATTR, node=self.nodeName, exists=True):
            # make the attr
            cmds.addAttr(self.nodeName, longName=CACHE_ATTR, dt="stringArray" )
        self.populateItems()
        if len(self.assItems):
            self.displayTree()
        else:
            cmds.treeView(self.assInfoPath, edit=True, visible=False)
            cmds.button(self.inspectAssPath, edit=True, visible=True)
            cmds.treeView(self.assInfoPath, edit=True, removeAll=True)

    def fileInfoReplace(self, nodeAttr) :
        if not cmds.attributeQuery(CACHE_ATTR, node=self.nodeName, exists=True):
            # make the attr
            cmds.addAttr(self.nodeName, longName=CACHE_ATTR, dt="stringArray" )
        self.populateItems()
        if len(self.assItems):
            self.displayTree()
        else:
            cmds.treeView(self.assInfoPath, edit=True, visible=False)
            cmds.button(self.inspectAssPath, edit=True, visible=True)
            cmds.treeView(self.assInfoPath, edit=True, removeAll=True)

    def displayTree(self):
        cmds.treeView(self.assInfoPath, edit=True, removeAll=True)
        cmds.treeView(self.assInfoPath, edit=True, visible=True)
        cmds.button(self.inspectAssPath, edit=True, visible=False)
        
        # First the Entry type names
        for i in self.assItems:
            nodeType = i[1]
            entryType = i[2]
            if not cmds.treeView(self.assInfoPath, query=True, itemExists=entryType):
                cmds.treeView(self.assInfoPath, edit=True, addItem=(entryType, ''))
            if not cmds.treeView(self.assInfoPath, query=True, itemExists=nodeType):
                cmds.treeView(self.assInfoPath, edit=True, addItem=(nodeType, entryType))

            cmds.treeView(self.assInfoPath, edit=True, addItem=(i[0],nodeType))
        
    def inspectFile(self):
        filenameAttr = self.nodeName + '.dso'
        filename = cmds.getAttr(filenameAttr)


        if not os.path.exists(filename):
            return

        '''
        iArchive = IArchive(str(filename))
        self.visitObject( iArchive.getTop() )

        self.assItems.append([path, name, parent, visibility])
        '''
        self.assItems = []

        universeCreated = False
        if not ai.AiUniverseIsActive():
            universeCreated = True
            ai.AiBegin()

        universe = ai.AiUniverse()
        ai.AiASSLoad(filename, ai.AI_NODE_ALL, universe)

        iter = ai.AiUniverseGetNodeIterator(ai.AI_NODE_ALL, universe);         
        
        while not ai.AiNodeIteratorFinished(iter):
            node = ai.AiNodeIteratorGetNext(iter)
            nodeName = ai.AiNodeGetName(node)
            if nodeName == 'root' or nodeName == 'ai_default_reflection_shader' or nodeName == 'options':
                continue
            
            nodeEntry = ai.AiNodeGetNodeEntry(node)
            entryName = ai.AiNodeEntryGetName(nodeEntry)
            entryType = ai.AiNodeEntryGetTypeName(nodeEntry)
            entryType = entryType.upper() + 'S'

            self.assItems.append([nodeName, entryName, entryType])

        ai.AiNodeIteratorDestroy(iter)
        ai.AiUniverseDestroy(universe)

        if universeCreated:
            ai.AiEnd()

        cmds.setAttr('{}.{}'.format(self.nodeName, CACHE_ATTR), len(self.assItems), *[','.join(a) for a in self.assItems], type="stringArray")
        self.displayTree()

    def populateItems(self):
        self.assItems = []
        self.selectedItems = []
        # get the items in the cache
        cache_str_list = cmds.getAttr('{}.{}'.format(self.nodeName, CACHE_ATTR)) or []
        for s in cache_str_list:
            self.assItems.append(s.split(','))

    # when "Use File Sequence" is toggled we need to change the Filename
    # -> enabling it should replace the numeric part by ###
    # -> disabling it should replace ### by the frameNumber value
    def useSequenceChange(self, nodeName):
        useSequence = cmds.getAttr(nodeName + '.useFrameExtension')
        mOrigPath = cmds.getAttr(nodeName + '.dso') or r''
        mPath = mOrigPath

        if useSequence:
            # check if something is connected to FrameNumber
            if not mu.hasAttrInputs(nodeName, 'frameNumber'):
                # Nothing connected yet, make an expression and connect it to frameNumber
                cmds.expression(s= nodeName +'.frameNumber=frame' )

            # We're supposed to find a file sequence, let's see if there is one
            expression = r'(.*?)([\._])([0-9#]*)([\.]?)([0-9#]*)(\.ass\.gz|\.ass|\.obj|\.ply)$'
            if re.search(expression,mPath) != None: # check if format is recognized
                m_groups = re.search(expression,mPath).groups()
                # Single file
                if not m_groups[2]:
                    cmds.setAttr(nodeName + '.useFrameExtension', False)
                    cmds.error(("[mtoa] StandIn Sequence not recognized with filename %s" % mPath))
                # Sequence without subframes    
                elif not m_groups[3]:
                #    cmds.setAttr(nodeName+'.useFrameExtension',True)
                    mPath = m_groups[0]+m_groups[1]+'#'*len(m_groups[2])+m_groups[5]
                    
                    if cmds.getAttr(nodeName+'.useSubFrame'):
                        cmds.setAttr(nodeName+'.useSubFrame',False)
                    
                # Sequence with subframes
                else:
                    mPath = m_groups[0]+m_groups[1]+'#'*len(m_groups[2])+m_groups[3]+'#'*len(m_groups[4])+m_groups[5]

                    if not cmds.getAttr(nodeName+'.useSubFrame'):
                        cmds.setAttr(nodeName+'.useSubFrame',True)       

        else:
            # replace #### by the current frame
            frameNumber = float(cmds.getAttr(nodeName + '.frameNumber')) + float(cmds.getAttr(nodeName + '.frameOffset'))
            if not cmds.getAttr(nodeName + '.useSubFrame'):
                frameNumber = int(frameNumber)
            startIndex = mPath.find('#')
            if startIndex >= 0 :
                hashCount = 0
                for i in range(startIndex, len(mPath)):
                    if mPath[i] == '#':
                        hashCount=hashCount+1
                    else:
                        break            
                frameStr = str(frameNumber)

                # apply the padding
                while len(frameStr) < hashCount:
                    frameStr = '0' + frameStr

                mPath = mPath[:startIndex] + frameStr + mPath[startIndex+hashCount:]

        if mPath != mOrigPath:
            cmds.setAttr(nodeName+'.dso', mPath, type='string')
        cmds.textField('standInDsoPath', edit=True, text=mPath)

    def setup(self):
        self.assInfoPath = ''
        self.inspectAssPath = ''
        self.assItems = {}

        self.beginScrollLayout()
        
        self.beginLayout('File/Frame', collapse=False)        
        self.addCustom('dso', ArnoldStandInTemplateDsoNew, ArnoldStandInTemplateDsoReplace)
        self.addControl('standInDrawOverride', label='Viewport Override')
        self.addControl('mode', label='Viewport Draw Mode')
        self.addSeparator()
        self.addControl('useFrameExtension', label='Use File Sequence', changeCommand=self.useSequenceChange)
        self.addControl('frameNumber', label='Frame')
        self.addControl('frameOffset')
        self.addSeparator()
        self.addControl('overrideNodes')
        self.addControl('aiNamespace', label='Namespace')
        self.endLayout()
        self.beginNoOptimize();

        self.beginLayout("File Contents", collapse=False)
        self.addCustom('aiInfo', self.fileInfoNew, self.fileInfoReplace)
        self.endLayout()

        self.beginLayout('Render Stats', collapse=True)

        # FIXME : the line below is just to create a callback that will be invoked when a StandIn UI
        # appears in the attribute editor. We need to hack the deprecated "override" attribute
        # (see #2515), and making sure they appear with the right value when the scene is 
        # inspected will avoid confusion. We can remove this once we no longer care about
        # pre-2.0.2 compatibility
        self.addCustom('', ArnoldStandInUpdateUI, ArnoldStandInUpdateUI)
        self.addControl('castsShadows', label='Casts Shadows')
        self.addControl('motionBlur', label='Motion Blur')
        self.addControl('primaryVisibility', label='Primary Visibility')
        self.addControl('aiVisibleInDiffuseReflection', label='Visible in Diffuse Reflection')
        self.addControl('aiVisibleInSpecularReflection', label='Visible in Specular Reflection')
        self.addControl('aiVisibleInDiffuseTransmission', label='Visible in Diffuse Transmission')
        self.addControl('aiVisibleInSpecularTransmission', label='Visible in Specular Transmission')
        self.addControl('aiVisibleInVolume', label='Visible in Volume')
        self.endLayout()

        self.beginLayout('StandIn Overrides', collapse=True)
        self.addControl('overrideLightLinking', label='Override StandIn Light Linking')
        self.addControl('overrideShaders', label='Override StandIn Shaders')
        self.addSeparator()
        self.addControl('overrideReceiveShadows', changeCommand=self.updateOverridesVisibility, label='Override Receive Shadows')
        self.addControl('receiveShadows', label='   Receive Shadows')
        self.addControl('overrideSelfShadows', changeCommand=self.updateOverridesVisibility, label='Override Self Shadows')
        self.addControl('aiSelfShadows', label='   Self Shadows')
        self.addControl('overrideOpaque', changeCommand=self.updateOverridesVisibility, label='Override Opaque')
        self.addControl('aiOpaque', label='   Opaque')
        self.addControl('overrideDoubleSided', changeCommand=self.updateOverridesVisibility, label='Override Double-Sided')
        self.addControl('doubleSided', label='   Double-Sided')
        self.addControl('overrideMatte', changeCommand=self.updateOverridesVisibility, label='Override Matte')
        self.addControl('aiMatte', label='   Matte')
        self.endLayout()

        self.endNoOptimize();
        
        self.beginLayout('Object Display', collapse=True)
        self.addControl('visibility')
        self.addControl('template')
        self.addControl('ghosting')
        self.addControl('intermediateObject')
        self.endLayout()
        
        self.beginLayout('Draw Override', collapse=True)
        self.addControl('overrideDisplayType')
        self.addControl('overrideLevelOfDetail')
        self.addControl('overrideShading')
        self.addControl('overrideTexturing')
        self.addControl('overridePlayback')
        self.addControl('overrideEnabled')
        self.addControl('useObjectColor')
        self.addControl('objectColor')
        self.endLayout()
    

        # include/call base class/node attributes
        mel.eval('AEdependNodeTemplate '+self.nodeName)
        
        self.suppress('blackBox')
        self.suppress('containerType')
        self.suppress('templateName')
        self.suppress('viewName')
        self.suppress('iconName')
        self.suppress('templateVersion')
        self.suppress('uiTreatment')
        self.suppress('customTreatment')
        self.suppress('creator')
        self.suppress('creationDate')
        self.suppress('rmbCommand')
        self.suppress('templatePath')
        self.suppress('viewMode')
        self.suppress('ignoreHwShader')
        self.suppress('boundingBoxScale')
        self.suppress('featureDisplacement')
        self.suppress('boundingBoxScale')
        self.suppress('initialSampleRate')
        self.suppress('extraSampleRate')
        self.suppress('textureThreshold')
        self.suppress('normalThreshold')
        self.suppress('lodVisibility')
        self.suppress('ghostingControl')
        self.suppress('ghostPreSteps')
        self.suppress('ghostPostSteps')
        self.suppress('ghostStepSize')
        self.suppress('ghostRangeStart')
        self.suppress('ghostRangeEnd')
        self.suppress('ghostDriver')
        self.suppress('ghostFrames')
        self.suppress('ghosting')
        self.suppress('ghostCustomSteps')
        self.suppress('ghostColorPreA')
        self.suppress('ghostColorPre')
        self.suppress('ghostColorPostA')
        self.suppress('ghostColorPost')
        self.suppress('tweak')
        self.suppress('relativeTweak')
        self.suppress('currentUVSet')
        self.suppress('displayImmediate')
        self.suppress('displayColors')
        self.suppress('displayColorChannel')
        self.suppress('currentColorSet')
        self.suppress('smoothShading')
        self.suppress('drawOverride')
        self.suppress('shadingSamples')
        self.suppress('maxVisibilitySamplesOverride')
        self.suppress('maxVisibilitySamples')
        self.suppress('antialiasingLevel')
        self.suppress('maxShadingSamples')
        self.suppress('shadingSamplesOverride')
        self.suppress('geometryAntialiasingOverride')
        self.suppress('antialiasingLevel')
        self.suppress('volumeSamplesOverride')
        self.suppress('volumeSamples')
        self.suppress('depthJitter')
        self.suppress('ignoreSelfShadowing')
        self.suppress('controlPoints')
        self.suppress('colorSet')
        self.suppress('uvSet')
        self.suppress('weights')
        self.suppress('renderInfo')
        self.suppress('renderLayerInfo')
        self.suppress('compInstObjGroups')
        self.suppress('instObjGroups')
        self.suppress('collisionOffsetVelocityIncrement')
        self.suppress('collisionOffsetVelocityMultiplier')
        self.suppress('collisionDepthVelocityMultiplier')
        self.suppress('collisionDepthVelocityIncrement')
    
        self.addExtraControls()
        self.endScrollLayout()

    def updateOverridesVisibility(self, nodeName):
                
        cmds.editorTemplate(dimControl=(nodeName, 'receiveShadows', not cmds.getAttr('%s.%s' % (nodeName, 'overrideReceiveShadows'))))  
        cmds.editorTemplate(dimControl=(nodeName, 'aiSelfShadows', not cmds.getAttr('%s.%s' % (nodeName, 'overrideSelfShadows'))))  
        cmds.editorTemplate(dimControl=(nodeName, 'aiOpaque', not cmds.getAttr('%s.%s' % (nodeName, 'overrideOpaque'))))  
        cmds.editorTemplate(dimControl=(nodeName, 'doubleSided', not cmds.getAttr('%s.%s' % (nodeName, 'overrideDoubleSided'))))  
        cmds.editorTemplate(dimControl=(nodeName, 'aiMatte', not cmds.getAttr('%s.%s' % (nodeName, 'overrideMatte'))))  
    

import re
import maya.cmds as cmds
import arnold as ai
import maya.mel as mel
from mtoa.ui.ae.utils import aeCallback
import mtoa.core as core
import os
import os.path
from mtoa.ui.ae.shaderTemplate import ShaderAETemplate

def ArnoldVolumeAutoStepChange(nodeName):
    autoStep = cmds.getAttr(nodeName+'.autoStepSize')
    dimStepSize = autoStep
    dimStepScale = not autoStep

    cmds.editorTemplate(dimControl=(nodeName, "stepSize",  dimStepSize))
    cmds.editorTemplate(dimControl=(nodeName, "stepScale", dimStepScale))

def ArnoldVolumeTypeChange(nodeName):
    volumeType = cmds.getAttr(nodeName + '.type')
    dimImplicitAttrs = True
    if (volumeType == 1):
        dimImplicitAttrs = False

    cmds.editorTemplate(dimControl=(nodeName, "field",  dimImplicitAttrs))
    cmds.editorTemplate(dimControl=(nodeName, "solver",  dimImplicitAttrs))
    cmds.editorTemplate(dimControl=(nodeName, "fieldChannel",  dimImplicitAttrs))
    cmds.editorTemplate(dimControl=(nodeName, "samples",  dimImplicitAttrs))
    cmds.editorTemplate(dimControl=(nodeName, "threshold",  dimImplicitAttrs))

def aiVolumeFieldReplace(plugName):
    nodeAndAttrs = plugName.split(".")
    node = nodeAndAttrs[0]
    ctrlName = "aiVolumeImplicit"
    ctrlName += nodeAndAttrs[1]
    cmds.attrNavigationControlGrp(ctrlName, edit=True, attribute=(plugName),  cn="createRenderNode -allWithShadersUp \"defaultNavigation -force true -connectToExisting -source %node -destination "+plugName+"\" \"\"")

def aiVolumeFieldNew(plugName):
    cmds.setUITemplate('attributeEditorTemplate', pst=True)

    nodeAndAttrs = plugName.split(".")
    ctrlName = "aiVolumeImplicit"
    ctrlName += nodeAndAttrs[1]

    cmds.attrNavigationControlGrp(ctrlName, label=nodeAndAttrs[1], cn="createRenderNode -allWithShadersUp \"defaultNavigation -force true -connectToExisting -source %node -destination "+plugName+"\" \"\"")
    cmds.setUITemplate(ppt=True)
    aiVolumeFieldReplace(plugName)

class AEaiVolumeTemplate(ShaderAETemplate):

    def filenameEdit(self, nodeName, mPath) :
        prevFilename = cmds.getAttr(nodeName)
        if prevFilename == mPath:
            return
        cmds.setAttr(nodeName,mPath,type='string')
        cmds.textScrollList(self.gridsListPath, edit=True, removeAll=True)

        if not os.path.isfile(mPath):
            return

        attrName = nodeName.replace('.filename', '.grids')
        self.genericGridsReplace(attrName, False)
        attrName = nodeName.replace('.filename', '.velocityGrids')
        self.genericGridsReplace(attrName, True)

    def filenameButtonPush(self, nodeName):
        basicFilter = 'OpenVDB File(*.vdb)'
        projectDir = cmds.workspace(query=True, directory=True)     
        ret = cmds.fileDialog2(fileFilter=basicFilter, cap='Load OpenVDB File',okc='Load',fm=1, startingDirectory=projectDir)
        if ret is not None and len(ret):
            self.filenameEdit(nodeName, ret[0])
            cmds.textField('arnoldVolumeFilenamePath', edit=True, text=ret[0])

    def filenameNew(self, nodeName) :
        cmds.rowColumnLayout( numberOfColumns=3, columnAlign=[(1, 'right'),(2, 'right'),(3, 'left')], columnAttach=[(1, 'right', 0), (2, 'both', 0), (3, 'left', 5)], columnWidth=[(1,145),(2,220),(3,30)] )
        cmds.text(label='Filename ')
        path = cmds.textField('arnoldVolumeFilenamePath',changeCommand=lambda *args: self.filenameEdit(nodeName, *args))
        cmds.textField( path, edit=True, text=cmds.getAttr(nodeName) )
        cmds.symbolButton('arnoldVolumeFilenamePathButton', height=20, image='navButtonBrowse.png', command=lambda *args: self.filenameButtonPush(nodeName))
        
    def filenameReplace(self, plugName) :
        cmds.textField( 'arnoldVolumeFilenamePath', edit=True, changeCommand=lambda *args: self.filenameEdit(plugName, *args))
        cmds.textField( 'arnoldVolumeFilenamePath', edit=True, text=cmds.getAttr(plugName) )
        cmds.symbolButton('arnoldVolumeFilenamePathButton', edit=True, image='navButtonBrowse.png' , command=lambda *args: self.filenameButtonPush(plugName))

    def gridsEdit(self, nodeName, isVelocity, mPath) :
        self.updateList(mPath, isVelocity)
        

    def gridsListEdit(self, nodeName, isVelocity) :
        
        if isVelocity:
            gridListField = self.velocityGridsListPath
            gridTextField = self.velocityGridsPath
        else:
            gridListField = self.gridsListPath
            gridTextField = self.gridsPath

        selectedList = cmds.textScrollList(gridListField, query=True, si=True);
        gridsValue = ''
        addSpace = False

        for item in selectedList:
            if addSpace:
                gridsValue += ' '
            
            addSpace = True
            gridsValue += item

        cmds.setAttr(nodeName, gridsValue, type='string')
        cmds.textField(gridTextField, edit=True, text=gridsValue)

    def updateList(self, gridsValue, isVelocity):

        if isVelocity:
            gridListField = self.velocityGridsListPath
            gridTextField = self.velocityGridsPath
        else:
            gridListField = self.gridsListPath
            gridTextField = self.gridsPath

        cmds.textScrollList(gridListField, edit=True, deselectAll=True)
        if not gridsValue:
            return

        gridsList = gridsValue.split(' ')
        for grid in gridsList:
            cmds.textScrollList(gridListField, edit=True, selectItem=grid)
            

    def gridsParamNew(self, nodeName) :
        self.genericGridsNew(nodeName, False)
    def gridsParamReplace(self, nodeName) :
        self.genericGridsReplace(nodeName, False)
    def velocityGridsParamNew(self, nodeName) :
        self.genericGridsNew(nodeName, True)
    def velocityGridsParamReplace(self, nodeName) :
        self.genericGridsReplace(nodeName, True)


    def genericGridsNew(self, nodeName, isVelocity) :

        if isVelocity:
            textLabel = 'Velocity Grids '
            labelWidth = 72
        else:
            textLabel = 'Grids '
            labelWidth = 32

        # 2 Columns (Left with label+line edit, Right with list)
        cmds.rowColumnLayout( numberOfColumns=2, columnWidth=[(1,320),(2,100)], columnAlign=[(1, 'right'),(2, 'left')], columnAttach=[(1, 'right', 0), (2, 'left', 5)]) 
        # 2 Rows (to get an empty space below the label)
        cmds.rowColumnLayout( numberOfRows=2, rowHeight=[(1,20),(2,20)] )
        # 2 Columns : label and line edit
        cmds.rowColumnLayout( numberOfColumns=2, columnWidth=[(1,labelWidth),(2,175)] )

        
        cmds.text(label=textLabel)
        gridTextField = cmds.textField( 'arnoldVolumeGrids', height=20)

        cmds.setParent('..')
        cmds.setParent('..')

        gridListField = cmds.textScrollList(height=50,allowMultiSelection=True)
        cmds.setParent('..')
        

        if isVelocity:
            self.velocityGridsPath = gridTextField
            self.velocityGridsListPath = gridListField
        else:
            self.gridsPath = gridTextField
            self.gridsListPath = gridListField

        self.genericGridsReplace(nodeName, isVelocity)
        
        
    def genericGridsReplace(self, nodeName, isVelocity) :

        if isVelocity:
            gridTextField = self.velocityGridsPath
            gridListField = self.velocityGridsListPath
            attrName = nodeName.replace('.velocityGrids', '.filename')
        else:
            gridTextField = self.gridsPath
            gridListField = self.gridsListPath
            attrName = nodeName.replace('.grids', '.filename')
        
        
        cmds.textField(gridTextField, edit=True, changeCommand=lambda *args: self.gridsEdit(nodeName, isVelocity, *args))
        cmds.textScrollList(gridListField, edit=True, removeAll=True,selectCommand=lambda *args: self.gridsListEdit(nodeName, isVelocity, *args))
        gridsParam = cmds.getAttr(nodeName)

        filename = cmds.getAttr(attrName)

        if filename is not None:
            filename = os.path.expandvars(filename)
            if os.path.isfile(filename):
                gridsList = ai.AiVolumeFileGetChannels(filename);
                
                numGrids = ai.AiArrayGetNumElements(gridsList)
                for i in range(0, numGrids):
                    cmds.textScrollList(gridListField, edit=True, append=str(ai.AiArrayGetStr(gridsList, i)))

                # if parameter 'grids' wasn't previously set, choose the first in the file list
                # FIXME do we really want to do that, or do we want to have a hardcoded default ?
                # note that nothing will happen until the node is shown in AE
                if not gridsParam:
                    if numGrids > 0:
                        if not isVelocity:
                            cmds.textScrollList(gridListField, edit=True, selectIndexedItem=1)
                            cmds.setAttr(nodeName, str(ai.AiArrayGetStr(gridsList,0)), type='string')
                else:
                    self.updateList(gridsParam, isVelocity)

        cmds.textField(gridTextField, edit=True, text=cmds.getAttr(nodeName))
        


    def setup(self):
        self.beginScrollLayout()
        
        self.beginLayout('Volume Attributes', collapse=False)        
        
        self.addSeparator()
        
        self.addCustom('filename', self.filenameNew, self.filenameReplace)

        self.gridsListPath = ''
        self.gridsPath = ''
        self.velocityGridsPath = ''
        self.velocityGridsListPath = ''

        self.addCustom('grids', self.gridsParamNew, self.gridsParamReplace)

        self.addControl('frame')
        
        self.addControl('padding')
        self.addControl('MinBoundingBox')
        self.addControl('MaxBoundingBox')
        self.addControl('autoStepSize', label = "Automatic Step Size", changeCommand=ArnoldVolumeAutoStepChange)
        self.addControl('stepSize')
        self.addControl('stepScale')
#        self.addControl('loadAtInit')
        self.addSeparator()
        self.addControl('disableRayExtents')
        self.addControl('volumePadding')
        self.addControl('compress')
        
        self.addSeparator()
        
        self.addCustom('velocityGrids', self.velocityGridsParamNew, self.velocityGridsParamReplace)
        self.addControl('velocityScale')
        self.addControl('velocityFps')
        self.addControl('motionStart')
        self.addControl('motionEnd')
        self.addControl('velocityThreshold')
        self.addSeparator()

        self.addControl('type', changeCommand=ArnoldVolumeTypeChange)
        self.addControl('solver')
        self.addControl('threshold')
        self.addControl('samples')
        self.addControl('fieldChannel')
        self.addCustom("field", aiVolumeFieldNew, aiVolumeFieldReplace)
        #self.addControl('field')

        self.endLayout()
                
        self.beginLayout('Render Stats', collapse=True)
        self.beginNoOptimize()
        self.addControl("castsShadows")
        self.addControl("receiveShadows")
        self.addControl("primaryVisibility")
        self.addControl("aiVisibleInDiffuseReflection", label="Visible In Diffuse Reflection")
        self.addControl("aiVisibleInSpecularReflection", label="Visible In Specular Reflection")
        self.addControl("aiVisibleInDiffuseTransmission", label="Visible In Diffuse Transmission")
        self.addControl("aiVisibleInSpecularTransmission", label="Visible In Specular Transmission")
        
        self.addControl("aiSelfShadows", label="Self Shadows")
        
        self.addControl("aiMatte", label="Matte")
        self.addControl("aiTraceSets", label="Trace Sets")
        
        self.endNoOptimize()
        
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




  

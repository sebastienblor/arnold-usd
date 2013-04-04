import re
import maya.cmds as cmds
import maya.mel as mel
from mtoa.ui.ae.utils import aeCallback
import mtoa.core as core
import pymel.core as pm
from mtoa.ui.ae.shaderTemplate import ShaderAETemplate

def LoadStandInButtonPush(nodeName):
    basicFilter = 'Arnold Archive (*.ass *.ass.gz *.obj *.ply);;Arnold Procedural (*.so *.dll *.dylib)'
    projectDir = cmds.workspace(query=True, directory=True)     
    ret = cmds.fileDialog2(fileFilter=basicFilter, dialogStyle=2,cap='Load StandIn',okc='Load',fm=4, startingDirectory=projectDir)
    if ret is not None and len(ret):
        ArnoldStandInDsoEdit(nodeName, ret[0], True)

def ArnoldStandInDsoEdit(nodeName, mPath, replace=False) :
    mArchivePath = ''
    nodeName = nodeName.replace('.dso','')
    
    expression = r''
    if replace:
        # Expression to replace frame numbers by #
        expression = r'(.*?)([\.\_]?)([0-9#]*)([\.]?)([0-9#]*)(.ass.gz|.ass|.obj|.ply)'
    else:
        expression = r'(.*?)([\.\_]?)([#]*)([\.]?)([#]*)(.ass.gz|.ass|.obj|.ply)'

    # If it is a recogniced format
    if re.search(expression,mPath) != None:
        m_groups = re.search(expression,mPath).groups()
        # Single file
        if not m_groups[2]:
            mArchivePath = mPath
            cmds.setAttr(nodeName+'.useFrameExtension',False)
        # Sequence without subframes    
        elif not m_groups[3]:
            cmds.setAttr(nodeName+'.useFrameExtension',True)
            mArchivePath = m_groups[0]+m_groups[1]+'#'*len(m_groups[2])+m_groups[5]
            cmds.setAttr(nodeName+'.useSubFrame',False)
        # Sequence with subframes
        else:
            cmds.setAttr(nodeName+'.useFrameExtension',True)
            mArchivePath = m_groups[0]+m_groups[1]+'#'*len(m_groups[2])+m_groups[3]+'#'*len(m_groups[4])+m_groups[5]
            cmds.setAttr(nodeName+'.useSubFrame',True)
    # Other
    else:
        mArchivePath = mPath
        
    cmds.setAttr(nodeName+'.dso',mArchivePath,type='string')
    if '.so' in mPath or '.dll' in mPath or '.dylib' in mPath:
        cmds.text('standInDataLabel', edit=True, enable=True)
        cmds.textField('standInData', edit=True, enable=True)
    else:
        cmds.text('standInDataLabel', edit=True, enable=False)
        cmds.textField('standInData', edit=True, enable=False)
    cmds.textField('standInDsoPath', edit=True, text=mArchivePath)

def ArnoldStandInBBoxScaleEdit(mScale) :
    # Get AE tab name
    node = mel.eval('$tempNode = $gAECurrentTab')
    
    # Update value
    cmds.setAttr(node+'.bboxScale',mScale)
    
def ArnoldStandInDataEdit(mData) :
    # Get AE tab name
    nodeName = mel.eval('$tempNode = $gAECurrentTab')
    
    # Set data
    cmds.setAttr(nodeName+'.data',mData,type='string')

def ArnoldStandInTemplateDsoNew(nodeName) :
    cmds.rowColumnLayout( numberOfColumns=3, columnAlign=(1, 'right'), columnAttach=[(1, 'right', 0), (2, 'both', 0), (3, 'right', 0)], columnWidth=[(1,145),(3,30)] )
    cmds.text(label='Path ')
    path = cmds.textField('standInDsoPath',changeCommand=lambda *args: ArnoldStandInDsoEdit(nodeName, *args))
    cmds.textField( path, edit=True, text=cmds.getAttr(nodeName) )
    cmds.button('standInDsoPathButton', label='...', command=lambda *args: LoadStandInButtonPush(nodeName))
    
def ArnoldStandInTemplateDataNew(nodeName) :
    print 'ArnoldStandInTemplateDataNew',nodeName
    cmds.rowColumnLayout( numberOfColumns=2, columnAlign=(1, 'right'), columnAttach=[(1, 'right', 0), (2, 'right', 0)], columnWidth=(1,145) )
    cmds.text('standInDataLabel', label='Data ')
    path = cmds.textField('standInData',changeCommand=ArnoldStandInDataEdit)
    cmds.textField( path, edit=True, text=cmds.getAttr(nodeName))
    filePath=cmds.getAttr(nodeName.replace('.data','.dso'))
    if filePath:
        if '.so' in filePath or '.dll' in filePath or '.dylib' in filePath:
            cmds.text('standInDataLabel', edit=True, enable=True)
            cmds.textField('standInData', edit=True, enable=True)
    else:
        cmds.text('standInDataLabel', edit=True, enable=False)
        cmds.textField('standInData', edit=True, enable=False)

def ArnoldStandInTemplateDsoReplace(plugName) :
    cmds.textField( 'standInDsoPath', edit=True, changeCommand=lambda *args: ArnoldStandInDsoEdit(plugName, *args))
    cmds.textField( 'standInDsoPath', edit=True, text=cmds.getAttr(plugName) )
    cmds.button('standInDsoPathButton', edit=True, label='...', command=lambda *args: LoadStandInButtonPush(plugName))

def ArnoldStandInTemplateDataReplace(plugName) :
    print 'ArnoldStandInTemplateDataReplace',plugName
    cmds.textField( 'standInData', edit=True, text=cmds.getAttr(plugName) )
    filePath=cmds.getAttr(plugName.replace('.data','.dso'))
    if filePath:
        if '.so' in filePath or '.dll' in filePath or '.dylib' in filePath:
            cmds.text('standInDataLabel', edit=True, enable=True)
            cmds.textField('standInData', edit=True, enable=True)
    else:
        cmds.text('standInDataLabel', edit=True, enable=False)
        cmds.textField('standInData', edit=True, enable=False)

def deferStandinLoadChange(nodeName):
    status = cmds.getAttr(nodeName+'.deferStandinLoad')
    if status == False:
        cmds.floatField('standInBBoxScale', edit=True, enable=False)
        cmds.text('standInBBoxScaleLabel', edit=True, enable=False)
    else:
        cmds.floatField('standInBBoxScale', edit=True, enable=True)
        cmds.text('standInBBoxScaleLabel', edit=True, enable=True)

def ArnoldStandInTemplateBBoxScaleNew(nodeName) :
    cmds.rowColumnLayout( numberOfColumns=2, columnAlign=(1, 'right'), columnAttach=[(1, 'right', 0), (2, 'left', 0)], columnWidth=[(1,145),(2,70)] )
    cmds.text('standInBBoxScaleLabel', label='Bounding Box Scale ', enable=False)
    path = cmds.floatField('standInBBoxScale', changeCommand=ArnoldStandInBBoxScaleEdit)
    cmds.floatField(path, edit=True, value=cmds.getAttr(nodeName), enable=False)
        
def ArnoldStandInTemplateBBoxScaleReplace(plugName) :
    cmds.floatField('standInBBoxScale', edit=True, value=cmds.getAttr(plugName) )
        
class AEaiStandInTemplate(ShaderAETemplate):
    def setup(self):
        self.beginScrollLayout()
        
        self.beginLayout('File/Frame', collapse=False)        
        self.addCustom('dso', ArnoldStandInTemplateDsoNew, ArnoldStandInTemplateDsoReplace)
        self.addCustom('data', ArnoldStandInTemplateDataNew, ArnoldStandInTemplateDataReplace)
        self.addControl('mode')
        self.addSeparator()
        self.addControl('useFrameExtension')
        self.addControl('frameNumber', label='Frame')
        self.addControl('frameOffset')
        self.addSeparator()
        self.addControl('overrideLightLinking', label='Override StandIn Light Linking')
        self.addControl('overrideShaders', label='Override StandIn Shaders')
        self.addSeparator()
        self.addControl('deferStandinLoad', label='Defer StandIn Load', changeCommand=deferStandinLoadChange)
        self.addCustom('bboxScale', ArnoldStandInTemplateBBoxScaleNew, ArnoldStandInTemplateBBoxScaleReplace)
        self.addControl('MaxBoundingBox')
        self.endLayout()
        
        self.beginLayout('Render Stats', collapse=True)
        self.addControl('castsShadows')
        self.addControl('receiveShadows')
        self.addControl('motionBlur')
        self.addControl('primaryVisibility')
        self.addControl('visibleInReflections')
        self.addControl('visibleInRefractions')
        self.addControl('doubleSided')
        self.addControl('opposite')
        self.endLayout()
        
        self.beginLayout('Arnold', collapse=True)
        self.addControl('aiSelfShadows', label='Self Shadows')
        self.addControl('aiOpaque', label='Opaque')
        self.addControl('aiVisibleInDiffuse', label='Visible in Diffuse')
        self.addControl('aiVisibleInGlossy', label='Visible in Glossy')
        self.addControl('aiSssSampleDistribution', label='SSS Samples Distribution')
        self.addControl('aiSssSampleSpacing', label='SSS Sample Spacing')
        self.endLayout()
        
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
        pm.mel.AEdependNodeTemplate(self.nodeName)
        
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




  

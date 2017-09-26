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
    ret = cmds.fileDialog2(fileFilter=basicFilter, cap='Load StandIn',okc='Load',fm=1, startingDirectory=projectDir)
    if ret is not None and len(ret):
        ArnoldStandInDsoEdit(nodeName, ret[0], True)

def ArnoldStandInDsoEdit(nodeName, mPath, replace=False) :
    mArchivePath = ''
    nodeName = nodeName.replace('.dso','')
    
    expression = r''
    if replace:
        # Expression to replace frame numbers by #
        expression = r'(.*?)([\._])([0-9#]*)([\.]?)([0-9#]*)(\.ass\.gz|\.ass|\.obj|\.ply)$'
    else:
        expression = r'(.*?)([\._])([#]*)([\.]?)([#]*)(\.ass\.gz|\.ass|\.obj|\.ply)$'

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


#def ArnoldStandInBBoxScaleEdit(mScale) :
    # Get AE tab name
#    node = mel.eval('$tempNode = $gAECurrentTab')
    
    # Update value
#    cmds.setAttr(node+'.bboxScale',mScale)
    
def ArnoldStandInDataEdit(mData) :
    # Get AE tab name
    nodeName = mel.eval('$tempNode = $gAECurrentTab')
    
    # Set data
    cmds.setAttr(nodeName+'.data',mData,type='string')

def ArnoldStandInTemplateDsoNew(nodeName) :
    cmds.rowColumnLayout( numberOfColumns=3, columnAlign=[(1, 'right'),(2, 'right'),(3, 'left')], columnAttach=[(1, 'right', 0), (2, 'both', 0), (3, 'left', 5)], columnWidth=[(1,145),(2,220),(3,30)] )
    cmds.text(label='Path ')
    path = cmds.textField('standInDsoPath',changeCommand=lambda *args: ArnoldStandInDsoEdit(nodeName, *args))
    cmds.textField( path, edit=True, text=cmds.getAttr(nodeName) )
    cmds.symbolButton('standInDsoPathButton', image='navButtonBrowse.png', command=lambda *args: LoadStandInButtonPush(nodeName))
    
def ArnoldStandInTemplateDataNew(nodeName) :
    cmds.rowColumnLayout( numberOfColumns=2, columnAlign=(1, 'right'), columnAttach=[(1, 'right', 0), (2, 'both', 0)], columnWidth=[(1,145),(2,220)] )
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
    cmds.symbolButton('standInDsoPathButton', edit=True, image='navButtonBrowse.png' , command=lambda *args: LoadStandInButtonPush(plugName))

def ArnoldStandInTemplateDataReplace(plugName) :
    cmds.textField( 'standInData', edit=True, text=cmds.getAttr(plugName) )
    filePath=cmds.getAttr(plugName.replace('.data','.dso'))
    if filePath:
        if '.so' in filePath or '.dll' in filePath or '.dylib' in filePath:
            cmds.text('standInDataLabel', edit=True, enable=True)
            cmds.textField('standInData', edit=True, enable=True)
    else:
        cmds.text('standInDataLabel', edit=True, enable=False)
        cmds.textField('standInData', edit=True, enable=False)



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
        if pm.getAttr(attrName + overrideVisAttrs[i]) == 0:
            pm.setAttr(attrName + overrideVisAttrs[i], 1)
            if pm.getAttr(attrName + visAttrs[i]) == 0:
                pm.setAttr(attrName + visAttrs[i], 1)

    
#def deferStandinLoadChange(nodeName):
#    status = cmds.getAttr(nodeName+'.deferStandinLoad')
#    if status == False:
#        cmds.floatField('standInBBoxScale', edit=True, enable=False)
#        cmds.text('standInBBoxScaleLabel', edit=True, enable=False)
#    else:
#        cmds.floatField('standInBBoxScale', edit=True, enable=True)
#        cmds.text('standInBBoxScaleLabel', edit=True, enable=True)

#def ArnoldStandInTemplateBBoxScaleNew(nodeName) :
#    cmds.rowColumnLayout( numberOfColumns=2, columnAlign=(1, 'right'), columnAttach=[(1, 'right', 0), (2, 'left', 0)], columnWidth=[(1,145),(2,70)] )
#    cmds.text('standInBBoxScaleLabel', label='Bounding Box Scale ', enable=False)
#    path = cmds.floatField('standInBBoxScale', changeCommand=ArnoldStandInBBoxScaleEdit)
#    cmds.floatField(path, edit=True, value=cmds.getAttr(nodeName), enable=False)
        
#def ArnoldStandInTemplateBBoxScaleReplace(plugName) :
#    cmds.floatField('standInBBoxScale', edit=True, value=cmds.getAttr(plugName) )


        
class AEaiStandInTemplate(ShaderAETemplate):
    def setup(self):
        self.beginScrollLayout()
        
        self.beginLayout('File/Frame', collapse=False)        
        self.addCustom('dso', ArnoldStandInTemplateDsoNew, ArnoldStandInTemplateDsoReplace)
        self.addCustom('data', ArnoldStandInTemplateDataNew, ArnoldStandInTemplateDataReplace)
        self.addControl('standInDrawOverride', label='Viewport Override')
        self.addControl('mode', label='Viewport Draw Mode')
        self.addSeparator()
        self.addControl('frameNumber', label='Frame')
        self.addControl('frameOffset')
        self.addSeparator()
        self.addControl('overrideNodes')
        self.addControl('aiNamespace', label='Namespace')
        self.endLayout()
        self.beginNoOptimize();

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
                
        pm.editorTemplate(dimControl=(nodeName, 'receiveShadows', not pm.getAttr('%s.%s' % (nodeName, 'overrideReceiveShadows'))))  
        pm.editorTemplate(dimControl=(nodeName, 'aiSelfShadows', not pm.getAttr('%s.%s' % (nodeName, 'overrideSelfShadows'))))  
        pm.editorTemplate(dimControl=(nodeName, 'aiOpaque', not pm.getAttr('%s.%s' % (nodeName, 'overrideOpaque'))))  
        pm.editorTemplate(dimControl=(nodeName, 'doubleSided', not pm.getAttr('%s.%s' % (nodeName, 'overrideDoubleSided'))))  
        pm.editorTemplate(dimControl=(nodeName, 'aiMatte', not pm.getAttr('%s.%s' % (nodeName, 'overrideMatte'))))  
    

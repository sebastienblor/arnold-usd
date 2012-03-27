import re
import maya.cmds as cmds
import maya.mel as mel
from mtoa.ui.ae.utils import aeCallback
import mtoa.core as core

def LoadStandInButtonPush(nodeName):
    basicFilter = 'Arnold Archive (*.ass *.ass.gz *.obj);;Arnold Procedural (*.so *.dll)'
    ret = cmds.fileDialog2(fileFilter=basicFilter, dialogStyle=2,cap='Load StandIn',okc='Load',fm=4)
    if ret is not None and len(ret):
        ArnoldStandInDsoEdit(nodeName, ret[0])

def ArnoldStandInDsoEdit(nodeName, mPath) :
    mArchivePath = ''
    nodeName = nodeName.replace(".dso","")
    
    # Sequence of .ass.gz
    if re.search(r'([-_/a-zA-Z0-9.]*[-/a-zA-Z])([_.])([0-9.]+)(.ass.gz)',mPath) != None:
        m_groups = re.search(r'([-_/a-zA-Z0-9.]*[-/a-zA-Z])([_.])([0-9.]+)(.ass.gz)',mPath).groups()
        mArchivePath = m_groups[0]+m_groups[1]+'#'+m_groups[3]
        if '.' in m_groups[2]:
            cmds.setAttr(nodeName+".useSubFrame",True)
        else:
            cmds.setAttr(nodeName+".useSubFrame",False)
        cmds.setAttr(nodeName+".useFrameExtension",True)
    # Single .ass.gz
    elif re.search(r'([-_/a-zA-Z0-9.]+)(\.ass.gz)',mPath) != None:
        mArchivePath = mPath
        cmds.setAttr(nodeName+".useFrameExtension",False)
    # Sequence of .ass
    elif re.search(r'([-_/a-zA-Z0-9.]*[-/a-zA-Z])([_.])([0-9.]+?)(.ass)',mPath) != None:
        m_groups = re.search(r'([-_/a-zA-Z0-9.]*[-/a-zA-Z])([_.])([0-9.]+)(.ass)',mPath).groups()
        mArchivePath = m_groups[0]+m_groups[1]+'#'+m_groups[3]
        if '.' in m_groups[2]:
            cmds.setAttr(nodeName+".useSubFrame",True)
        else:
            cmds.setAttr(nodeName+".useSubFrame",False)
        cmds.setAttr(nodeName+".useFrameExtension",True)
    # Single .ass
    elif re.search(r'([-_/a-zA-Z0-9.]+)(\.ass)',mPath) != None:
        mArchivePath = mPath
        cmds.setAttr(nodeName+".useFrameExtension",False)
    # Sequence of .obj
    elif re.search(r'([-_/a-zA-Z0-9.]*[-/a-zA-Z])[_.]([0-9.]+)(.obj)',mPath) != None:
        m_groups = re.search(r'([-_/a-zA-Z0-9.]*[-/a-zA-Z])[_.]([0-9.]+)(.obj)',mPath).groups()
        mArchivePath = m_groups[0]+'.#'+m_groups[2]
        if '.' in m_groups[1]:
            cmds.setAttr(nodeName+".useSubFrame",True)
        else:
            cmds.setAttr(nodeName+".useSubFrame",False)
        cmds.setAttr(nodeName+".useFrameExtension",True)
    # Single .obj
    elif re.search(r'([-_/a-zA-Z0-9.]+)(\.obj)',mPath) != None:
        mArchivePath = mPath
        cmds.setAttr(nodeName+".useFrameExtension",False)
    # Other
    else:
        mArchivePath = mPath
        
    cmds.setAttr(nodeName+".dso",mArchivePath,type="string")
    if ".so" in mPath or ".dll" in mPath or ".dylib" in mPath:
        cmds.text("standInDataLabel", edit=True, enable=True)
        cmds.textField("standInData", edit=True, enable=True)
    else:
        cmds.text("standInDataLabel", edit=True, enable=False)
        cmds.textField("standInData", edit=True, enable=False)
    cmds.textField("standInDsoPath", edit=True, text=mArchivePath)

def ArnoldStandInBBoxScaleEdit(mScale) :
    # Get AE tab name
    node = mel.eval('$tempNode = $gAECurrentTab')
    
    # Update value
    cmds.setAttr(node+".bboxScale",mScale)
    
def ArnoldStandInDataEdit(mData) :
    # Get AE tab name
    nodeName = mel.eval('$tempNode = $gAECurrentTab')
    
    # Set data
    cmds.setAttr(nodeName+".data",mData,type="string")

def ArnoldStandInTemplateDsoNew(nodeName) :
    cmds.rowColumnLayout( numberOfColumns=3, columnAlign=(1, "right"), columnAttach=[(1, "right", 0), (2, "both", 0), (3, "right", 0)], columnWidth=[(1,145),(3,30)] )
    cmds.text(label="Path ")
    path = cmds.textField("standInDsoPath",changeCommand=lambda *args: ArnoldStandInDsoEdit(nodeName, *args))
    cmds.textField( path, edit=True, text=cmds.getAttr(nodeName) )
    cmds.button("standInDsoPathButton", label="...", command=lambda *args: LoadStandInButtonPush(nodeName))
    
def ArnoldStandInTemplateDataNew(nodeName) :
    print 'ArnoldStandInTemplateDataNew',nodeName
    cmds.rowColumnLayout( numberOfColumns=2, columnAlign=(1, "right"), columnAttach=[(1, "right", 0), (2, "right", 0)], columnWidth=(1,145) )
    cmds.text("standInDataLabel", label="Data ", enable=False)
    path = cmds.textField("standInData",changeCommand=ArnoldStandInDataEdit)
    cmds.textField( path, edit=True, text=cmds.getAttr(nodeName), enable=False)

def ArnoldStandInTemplateDsoReplace(plugName) :
    cmds.textField( "standInDsoPath", edit=True, changeCommand=lambda *args: ArnoldStandInDsoEdit(plugName, *args))
    cmds.textField( "standInDsoPath", edit=True, text=cmds.getAttr(plugName) )
    cmds.button("standInDsoPathButton", edit=True, label="...", command=lambda *args: LoadStandInButtonPush(plugName))

def ArnoldStandInTemplateDataReplace(plugName) :
    print 'ArnoldStandInTemplateDataReplace',plugName
    cmds.textField( "standInData", edit=True, text=cmds.getAttr(plugName) )

def deferStandinLoadChange(nodeName):
    status = cmds.getAttr(nodeName+".deferStandinLoad")
    if status == False:
        cmds.floatField("standInBBoxScale", edit=True, enable=False)
        cmds.text("standInBBoxScaleLabel", edit=True, enable=False)
    else:
        cmds.floatField("standInBBoxScale", edit=True, enable=True)
        cmds.text("standInBBoxScaleLabel", edit=True, enable=True)

def ArnoldStandInTemplateBBoxScaleNew(nodeName) :
    cmds.rowColumnLayout( numberOfColumns=2, columnAlign=(1, "right"), columnAttach=[(1, "right", 0), (2, "left", 0)], columnWidth=[(1,145),(2,70)] )
    cmds.text("standInBBoxScaleLabel", label="Bounding Box Scale ", enable=False)
    path = cmds.floatField("standInBBoxScale", changeCommand=ArnoldStandInBBoxScaleEdit)
    cmds.floatField(path, edit=True, value=cmds.getAttr(nodeName), enable=False)
        
def ArnoldStandInTemplateBBoxScaleReplace(plugName) :
    cmds.floatField("standInBBoxScale", edit=True, value=cmds.getAttr(plugName) )
        
def aiStandInTemplate(nodeName):

    cmds.editorTemplate(beginScrollLayout=True)

    cmds.editorTemplate(beginLayout="File/Frame", collapse=False)
    cmds.editorTemplate(aeCallback(ArnoldStandInTemplateDsoNew), aeCallback(ArnoldStandInTemplateDsoReplace), "dso", callCustom=True)
    cmds.editorTemplate(aeCallback(ArnoldStandInTemplateDataNew), aeCallback(ArnoldStandInTemplateDataReplace), "data", callCustom=True)
    cmds.editorTemplate("mode", addControl=True)
    cmds.editorTemplate(addSeparator=True)
    cmds.editorTemplate("useFrameExtension", addControl=True)
    cmds.editorTemplate("frameNumber", label="Frame", addControl=True)
    cmds.editorTemplate("frameOffset", addControl=True)
    cmds.editorTemplate(addSeparator=True)
    cmds.editorTemplate("overrideLightLinking", label="Override StandIn Light Linking", addControl=True)
    cmds.editorTemplate(interruptOptimize=True)
    cmds.editorTemplate("overrideShaders", label="Override StandIn Shaders", addControl=True)
    cmds.editorTemplate(addSeparator=True)
    cmds.editorTemplate("deferStandinLoad", aeCallback(deferStandinLoadChange), label="Defer StandIn Load", addControl=True)
    cmds.editorTemplate(aeCallback(ArnoldStandInTemplateBBoxScaleNew), aeCallback(ArnoldStandInTemplateBBoxScaleReplace), "bboxScale" , callCustom=True)
    #cmds.editorTemplate("MaxBoundingBox", addControl=True)
    cmds.editorTemplate(endLayout=True) 
        
    cmds.editorTemplate(beginLayout="Render Stats", collapse=True)
    cmds.editorTemplate("castsShadows", addControl=True)
    cmds.editorTemplate("receiveShadows", addControl=True)
    cmds.editorTemplate("motionBlur", addControl=True)
    cmds.editorTemplate("primaryVisibility", addControl=True)
    cmds.editorTemplate("visibleInReflections", addControl=True)
    cmds.editorTemplate("visibleInRefractions", addControl=True)
    cmds.editorTemplate("doubleSided", addControl=True)
    cmds.editorTemplate("opposite", addControl=True)
    cmds.editorTemplate(endLayout=True)

    cmds.editorTemplate(beginLayout="Arnold", collapse=True)
    cmds.editorTemplate("aiSelfShadows", addControl=True)
    cmds.editorTemplate("aiOpaque", addControl=True)
    cmds.editorTemplate("aiVisibleInDiffuse", addControl=True)
    cmds.editorTemplate("aiVisibleInGlossy", addControl=True)
    cmds.editorTemplate("aiSssSampleDistribution", label="SSS Samples Distribution", addControl=True)
    cmds.editorTemplate("aiSssSampleSpacing", label="SSS Sample Spacing", addControl=True)
    cmds.editorTemplate(endLayout=True)

    #cmds.editorTemplate(endLayout=True)

    cmds.editorTemplate(beginLayout="Object Display", collapse=True)
    cmds.editorTemplate("visibility", addControl=True)
    cmds.editorTemplate("template", addControl=True)
    cmds.editorTemplate("ghosting", addControl=True)
    cmds.editorTemplate("intermediateObject", addControl=True)
    cmds.editorTemplate(endLayout=True)
    
    cmds.editorTemplate(beginLayout="Draw Override", collapse=True)
    cmds.editorTemplate("overrideDisplayType", addControl=True)
    cmds.editorTemplate("overrideLevelOfDetail", addControl=True)
    cmds.editorTemplate("overrideShading", addControl=True)
    cmds.editorTemplate("overrideTexturing", addControl=True)
    cmds.editorTemplate("overridePlayback", addControl=True)
    cmds.editorTemplate("overrideEnabled", addControl=True)
    cmds.editorTemplate("useObjectColor", addControl=True)
    cmds.editorTemplate("objectColor", addControl=True)
    cmds.editorTemplate(endLayout=True)    

    # include/call base class/node attributes
    mel.eval('AEdependNodeTemplate "%s"' % nodeName)
    cmds.editorTemplate(suppress="blackBox")
    cmds.editorTemplate(suppress="containerType")
    cmds.editorTemplate(suppress="templateName")
    cmds.editorTemplate(suppress="viewName")
    cmds.editorTemplate(suppress="iconName")
    cmds.editorTemplate(suppress="templateVersion")
    cmds.editorTemplate(suppress="uiTreatment")
    cmds.editorTemplate(suppress="customTreatment")
    cmds.editorTemplate(suppress="creator")
    cmds.editorTemplate(suppress="creationDate")
    cmds.editorTemplate(suppress="rmbCommand")
    cmds.editorTemplate(suppress="templatePath")
    cmds.editorTemplate(suppress="viewMode")
    cmds.editorTemplate(suppress="ignoreHwShader")
    cmds.editorTemplate(suppress="boundingBoxScale")
    cmds.editorTemplate(suppress="featureDisplacement")
    cmds.editorTemplate(suppress="boundingBoxScale")
    cmds.editorTemplate(suppress="initialSampleRate")
    cmds.editorTemplate(suppress="extraSampleRate")
    cmds.editorTemplate(suppress="textureThreshold")
    cmds.editorTemplate(suppress="normalThreshold")
    cmds.editorTemplate(suppress="lodVisibility")
    cmds.editorTemplate(suppress="ghostingControl")
    cmds.editorTemplate(suppress="ghostPreSteps")
    cmds.editorTemplate(suppress="ghostPostSteps")
    cmds.editorTemplate(suppress="ghostStepSize")
    cmds.editorTemplate(suppress="ghostRangeStart")
    cmds.editorTemplate(suppress="ghostRangeEnd")
    cmds.editorTemplate(suppress="ghostDriver")
    cmds.editorTemplate(suppress="ghostFrames")
    cmds.editorTemplate(suppress="ghosting")
    cmds.editorTemplate(suppress="ghostCustomSteps")
    cmds.editorTemplate(suppress="tweak")
    cmds.editorTemplate(suppress="relativeTweak")
    cmds.editorTemplate(suppress="currentUVSet")
    cmds.editorTemplate(suppress="displayImmediate")
    cmds.editorTemplate(suppress="displayColors")
    cmds.editorTemplate(suppress="displayColorChannel")
    cmds.editorTemplate(suppress="currentColorSet")
    cmds.editorTemplate(suppress="smoothShading")
    cmds.editorTemplate(suppress="drawOverride")
    cmds.editorTemplate(suppress="shadingSamples")
    cmds.editorTemplate(suppress="maxVisibilitySamplesOverride")
    cmds.editorTemplate(suppress="maxVisibilitySamples")
    cmds.editorTemplate(suppress="antialiasingLevel")
    cmds.editorTemplate(suppress="maxShadingSamples")
    cmds.editorTemplate(suppress="shadingSamplesOverride")
    cmds.editorTemplate(suppress="geometryAntialiasingOverride")
    cmds.editorTemplate(suppress="antialiasingLevel")
    cmds.editorTemplate(suppress="volumeSamplesOverride")
    cmds.editorTemplate(suppress="volumeSamples")
    cmds.editorTemplate(suppress="depthJitter")
    cmds.editorTemplate(suppress="ignoreSelfShadowing")
    cmds.editorTemplate(suppress="controlPoints")
    cmds.editorTemplate(suppress="colorSet")
    cmds.editorTemplate(suppress="uvSet")
    cmds.editorTemplate(suppress="weights")
    cmds.editorTemplate(suppress="renderInfo")
    cmds.editorTemplate(suppress="renderLayerInfo")
    cmds.editorTemplate(suppress="compInstObjGroups")
    cmds.editorTemplate(suppress="instObjGroups")
    cmds.editorTemplate(suppress="collisionOffsetVelocityIncrement")
    cmds.editorTemplate(suppress="collisionOffsetVelocityMultiplier")
    cmds.editorTemplate(suppress="collisionDepthVelocityMultiplier")
    cmds.editorTemplate(suppress="collisionDepthVelocityIncrement")
    
    cmds.editorTemplate(addExtraControls=True)
    cmds.editorTemplate(endScrollLayout=True)




  

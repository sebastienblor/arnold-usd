import maya.cmds as cmds
import maya.mel as mel
from mtoa.ui.ae.utils import aeCallback
import mtoa.callbacks as callbacks

def LoadStandInButtonPush(*arg):
    basicFilter = "Arnold Source Scene (*.ass);;Arnold Procedural (*.so)"
    ret = cmds.fileDialog2(fileFilter=basicFilter, dialogStyle=2,cap="Load StandIn",okc="Load",fm=4)
    if len(ret):
        m_tmpSelected = cmds.ls(sl=1)[0]
        if cmds.listRelatives(m_tmpSelected) != None:
            nodeName = cmds.listRelatives(m_tmpSelected)[0]
        else:
            nodeName = m_tmpSelected
        cmds.setAttr(nodeName+".dso",ret[0],type="string")
        if ".so" in ret[0]:
            cmds.text("standInDataLabel", edit=True, enable=True)
            cmds.textField("standInData", edit=True, enable=True)
        else:
            cmds.text("standInDataLabel", edit=True, enable=False)
            cmds.textField("standInData", edit=True, enable=False)
        cmds.textField("standInDsoPath", edit=True, text=ret[0])

def ArnoldStandInTemplateDsoNew(nodeName) :
    cmds.rowColumnLayout( numberOfColumns=3, columnAlign=(1, "right"), columnAttach=[(1, "left", 0), (2, "both", 0), (3, "right", 0)], columnWidth=[(1,145),(3,30)] )
    cmds.text(label="Path ")
    path = cmds.textField("standInDsoPath")
    cmds.textField( path, edit=True, text=cmds.getAttr(nodeName) )
    cmds.button( label="...", command=LoadStandInButtonPush)
    
def ArnoldStandInTemplateDataNew(nodeName) :
    cmds.rowColumnLayout( numberOfColumns=2, columnAlign=(1, "right"), columnAttach=[(1, "left", 0), (2, "right", 0)], columnWidth=(1,145) )
    cmds.text("standInDataLabel", label="Data ", enable=False)
    path = cmds.textField("standInData")
    cmds.textField( path, edit=True, text=cmds.getAttr(nodeName), enable=False)

def ArnoldStandInTemplateDsoReplace(plugName) :
    cmds.textField( "standInDsoPath", edit=True, text=cmds.getAttr(plugName) )

def ArnoldStandInTemplateDataReplace(plugName) :
    cmds.textField( "standInData", edit=True, text=cmds.getAttr(plugName) )

def ArnoldStandInTemplate(nodeName):

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
    cmds.editorTemplate("loadAtInit", label="Deferred Loading", addControl=True)
    cmds.editorTemplate("MinBoundingBox", addControl=True)
    cmds.editorTemplate("MaxBoundingBox", addControl=True)
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
    cmds.editorTemplate("selfShadows", addControl=True)
    cmds.editorTemplate("opaque", addControl=True)
    cmds.editorTemplate("aiVisibleInDiffuse", addControl=True)
    cmds.editorTemplate("aiVisibleInGlossy", addControl=True)
    cmds.editorTemplate(endLayout=True)


    cmds.editorTemplate(endLayout=True)

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
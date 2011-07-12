import re
import maya.cmds as cmds
import maya.mel as mel
from mtoa.ui.ae.utils import aeCallback
import mtoa.callbacks as callbacks
from customShapeAttributes import commonShapeAttributes

def LoadStandInButtonPush(*arg):
    basicFilter = 'Arnold Archive (*.ass *.obj);;Arnold Procedural (*.so *.dll)'
    ret = cmds.fileDialog2(fileFilter=basicFilter, dialogStyle=2,cap='Load StandIn',okc='Load',fm=4)
    if len(ret):
        ArnoldStandInDsoEdit(ret[0])
        
def ArnoldStandInDsoEdit(mPath) :
    mArchivePath = ''
    # Select StandIn shape
    m_tmpSelected = cmds.ls(sl=1)[0]
    if cmds.listRelatives(m_tmpSelected) != None:
        nodeName = cmds.listRelatives(m_tmpSelected)[0]
    else:
        nodeName = m_tmpSelected
    # Single .ass
    if   re.search(r'([-_/a-zA-Z0-9.]+)(\.ass)',mPath) != None:
        mArchivePath = mPath
        cmds.setAttr(nodeName+".useFrameExtension",False)
    # Sequence of .ass
    elif re.search(r'([-_/a-zA-Z0-9.]+)([0-9.]+)(.ass)',mPath) != None:
        m_groups = re.search(r'([-_/a-zA-Z0-9.]+)([0-9.]+)(.ass)',mPath).groups()
        mArchivePath = m_groups[0]+'#'+m_groups[2]
        if '.' in m_groups[1]:
            cmds.setAttr(nodeName+".useSubFrame",True)
        else:
            cmds.setAttr(nodeName+".useSubFrame",False)
        cmds.setAttr(nodeName+".useFrameExtension",True)
    # Single .obj
    elif re.search(r'([-_/a-zA-Z0-9.]+)(\.obj)',mPath) != None:
        mArchivePath = mPath
        cmds.setAttr(nodeName+".useFrameExtension",False)
    # Sequence of .obj
    elif re.search(r'([-_/a-zA-Z0-9.]+)([0-9.]+)(.obj)',mPath) != None:
        m_groups = re.search(r'([-_/a-zA-Z0-9.]+)([0-9.]+)(.obj)',mPath).groups()
        mArchivePath = m_groups[0]+'#'+m_groups[2]
        if '.' in m_groups[1]:
            cmds.setAttr(nodeName+".useSubFrame",True)
        else:
            cmds.setAttr(nodeName+".useSubFrame",False)
        cmds.setAttr(nodeName+".useFrameExtension",True)
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
   
def ArnoldStandInTemplateDsoNew(nodeName) :
    cmds.rowColumnLayout( numberOfColumns=3, columnAlign=(1, "right"), columnAttach=[(1, "left", 0), (2, "both", 0), (3, "right", 0)], columnWidth=[(1,145),(3,30)] )
    cmds.text(label="Path ")
    path = cmds.textField("standInDsoPath",changeCommand=ArnoldStandInDsoEdit)
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
    cmds.editorTemplate("loadAtInit", label="Deferred Loading", addControl=True)
    cmds.editorTemplate("bboxScale", addControl=True)
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
    commonShapeAttributes(nodeName)
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

def SaveStandInButtonPush(*arg):
    basicFilter = "Arnold Source Scene (*.ass)"
    ret = cmds.fileDialog2(fileFilter=basicFilter, dialogStyle=2,cap="Save StandIn",okc="Save",fm=0)
    if len(ret):
        cmds.textField("aiExportFilename", edit=True, text=ret[0].replace(".ass",""))

def SequenceToggleOn(*arg):
   ToggleSequenceLine(True)
   
def SequenceToggleOff(*arg):
   ToggleSequenceLine(False)

def ToggleSequenceLine(flag):
   cmds.text("aiExportStartLabel",edit=True,enable=flag)
   cmds.floatField("aiExportStart",edit=True,enable=flag)
   cmds.text("aiExportEndLabel",edit=True,enable=flag)
   cmds.floatField("aiExportEnd",edit=True,enable=flag)
   cmds.text("aiExportStepLabel",edit=True,enable=flag)
   cmds.floatField("aiExportStep",edit=True,enable=flag)

def DoExportStandInArchive(*arg):
   name = cmds.textField("aiExportFilename", query=True, text=True)
   if len(cmds.ls(sl=True)):
      if (name != None) and (name != ""):
         # Output mask for shape and shader only
         if not cmds.ls('defaultArnoldRenderOptions'):
            cmds.createNode('aiOptions', skipSelect=True, shared=True, name='defaultArnoldRenderOptions')
         oldOutputMask = cmds.getAttr("defaultArnoldRenderOptions.output_ass_mask")
         cmds.setAttr("defaultArnoldRenderOptions.output_ass_mask", 24)

         if cmds.checkBox("aiExportSequence", query=True, value=True):
            start = cmds.floatField("aiExportStart", query=True, value=True)
            end   = cmds.floatField("aiExportEnd", query=True, value=True)
            step  = cmds.floatField("aiExportStep", query=True, value=True)
            cmds.arnoldExportAss(f=name, s=True, bb=True, sf=start, ef=end, fs=step)
         else:
            cmds.arnoldExportAss(f=name, s=True, bb=True)
         # Restore old output mask
         cmds.setAttr("defaultArnoldRenderOptions.output_ass_mask", oldOutputMask)
         cmds.deleteUI("arnold_export_render_object_win")
      else:
         cmds.confirmDialog( title="Error", message="Enter a name", button="Ok", dismissString="Ok", icon="warning" )
         cmds.warning("Enter a name")
   else:
      cmds.confirmDialog( title="Error", message="Select an object", button="Ok", dismissString="Ok", icon="warning" )
      cmds.warning("Select an object")
  
def ArnoldExportRenderObjectWindow(*arg):
   win = "arnold_export_render_object_win"
   if cmds.window(win, exists=True):
      cmds.deleteUI(win)
    
   cmds.window(win, title="Export StandIn Archive", resizeToFitChildren=True)
    
   cmds.frameLayout( label="File", borderStyle='out' )
    
   cmds.rowColumnLayout( numberOfColumns=3, columnAlign=(1, "right"), columnAttach=[(1, "left", 0), (2, "both", 0), (3, "right", 0)], columnWidth=[(1,145),(3,30)] )
   cmds.text(label="Filename ")
   cmds.textField("aiExportFilename")
   cmds.button( label="...", command=SaveStandInButtonPush)
   cmds.setParent( '..' )
    
   cmds.setParent( '..' )
    
   cmds.frameLayout( label="Sequence", borderStyle='out' )
    
   cmds.rowColumnLayout( numberOfColumns=2, columnAlign=(1, "right"), columnAttach=[(1, "left", 0), (2, "right", 0)] )
   cmds.text(label="Sequence ")
   cmds.checkBox("aiExportSequence",label="",onCommand=SequenceToggleOn,offCommand=SequenceToggleOff)
   cmds.setParent( '..' )
    
   cmds.rowColumnLayout( numberOfColumns=6,  columnAttach=[(1, "left", 0), (2, "both", 0), (3, "both", 0),(4, "both", 0), (5, "both", 0), (6, "right", 0)])
   cmds.text("aiExportStartLabel",label="Start ")
   cmds.floatField("aiExportStart")
   cmds.floatField("aiExportStart", edit=True, value=cmds.playbackOptions(query=True, animationStartTime=True), enable=False)
   cmds.text("aiExportEndLabel",label="End   ")
   cmds.floatField("aiExportEnd")
   cmds.floatField("aiExportEnd", edit=True, value=cmds.playbackOptions(query=True, animationEndTime=True), enable=False)
   cmds.text("aiExportStepLabel",label="Step  ")
   cmds.floatField("aiExportStep")
   cmds.floatField("aiExportStep", edit=True, value=1, enable=False)
   cmds.setParent( '..' )
    
   cmds.setParent( '..' )
    
   cmds.rowColumnLayout(numberOfColumns=2, columnAlign=(1, "right"), columnAttach=[(1, "left", 0), (2, "right", 0)])
   cmds.button(label='Ok', command=DoExportStandInArchive)
   cmds.button(label='Cancel', command=('import maya.cmds as cmds;cmds.deleteUI(\"' + win + '\", window=True)'))
   cmds.setParent('..')
    
   cmds.showWindow(win)

#ArnoldExportRenderObjectWindow()
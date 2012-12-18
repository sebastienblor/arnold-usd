import pymel.core as pm
import maya.cmds as cmds
import maya.OpenMaya as om
import mtoa.ui.ae.lightTemplate as lightTemplate
from mtoa.ui.ae.utils import aeCallback
import mtoa.ui.ae.templates as templates
import mtoa.callbacks as callbacks
import mtoa.core as core

class ParticleTemplate(templates.ShapeTranslatorTemplate):
    def setup(self):
        self.commonShapeAttributes()
        self.addSeparator()
        self.addControl("aiSssSampleDistribution", label="SSS Samples Distribution")
        self.addControl("aiSssSampleSpacing", label="SSS Sample Spacing")
        self.addSeparator()
        self.addControl("aiRenderPointsAs", label="Render Points As")
        self.addControl("aiMinParticleRadius", label="Min Particle Radius")
        self.addControl("aiRadiusMultiplier", label="Radius Multiplier")
        self.addControl("aiMaxParticleRadius", label="Max Particle Radius")
        self.addControl("aiMinPixelWidth", label="Min Pixel Width")
        self.addSeparator()   
        self.addControl("aiExportParticleIDs", label="Export Particle Id")
        self.addControl("aiExportAttributes", label="Export Attributes")
        self.addSeparator()
        self.addControl("aiDeleteDeadParticles", label="Delete Dead Particles")
        self.addControl("aiInterpolateBlur", label="Interpolate Blur Steps")
        self.addSeparator()
        self.addControl("aiUserOptions", label="User Options")
        
templates.registerTranslatorUI(ParticleTemplate, "particle", "<built-in>")

class NParticleTemplate(ParticleTemplate):
        pass
    
templates.registerTranslatorUI(NParticleTemplate, "nParticle", "<built-in>")

class MeshTemplate(templates.ShapeTranslatorTemplate):
    def subdivDicingCameraNew(self, attrName):
        pm.setUITemplate('attributeEditorTemplate', pst=True)
        pm.attrNavigationControlGrp('aiSubdivDicingCameraCtrl',
                                    at=attrName,
                                    label="Subdivision Dicing Camera" )
        pm.setUITemplate(ppt=True)
    
    def subdivDicingCameraReplace(self, attrName):
        pm.attrNavigationControlGrp('aiSubdivDicingCameraCtrl', edit=True,
                                    at=attrName )
        # pm.editorTemplate("aiSubdivDicingCamera", label="Subdivision Dicing Camera", addDynamicControl=True)
        #pm.editorTemplate(aeCallback(self.subdivDicingCameraNew), aeCallback(self.subdivDicingCameraReplace), "aiSubdivDicingCamera", callCustom=True)

    def setup(self):
        self.commonShapeAttributes()
        
        self.addSeparator()
        self.addControl("aiExportTangents", label="Export Tangents")
        self.addControl("aiExportColors", label="Export Vertex Colors")
        self.addControl("aiExportRefPoints", label="Export Reference Positions")
        self.addControl("aiExportRefNormals", label="Export Reference Normals")
        self.addControl("aiExportRefTangents", label="Export Reference Tangents")
        
        self.addSeparator()
        self.addControl("aiSssSampleDistribution", label="SSS Samples Distribution")
        self.addControl("aiSssSampleSpacing", label="SSS Sample Spacing")
        
        self.beginLayout('Subdivision', collapse=False)
        self.addControl("aiSubdivType", label="Type")
        self.addControl("aiSubdivIterations", label="Iterations")
        self.addControl("aiSubdivAdaptiveMetric", label="Adaptive Metric")
        self.addControl("aiSubdivPixelError", label="Pixel Error")
        # TODO: add dicing camera UI
        self.addControl("aiSubdivDicingCamera", label="Dicing Camera")
        self.addControl("aiSubdivUvSmoothing", label="UV Smoothing")
        self.addControl("aiSubdivSmoothDerivs", label="Smooth Tangents")
        self.endLayout()
        
        self.beginLayout('Displacement Attributes', collapse=False)
        self.addControl("aiDispHeight", label="Height")
        self.addControl("aiDispPadding", label="Bounds Padding")
        self.addControl("aiDispZeroValue", label="Scalar Zero Value")
        self.addControl("aiDispAutobump", label="Auto Bump")
        self.endLayout()
        self.addControl("aiUserOptions", label="User Options")
        #pm.editorTemplate("aiExportHairIDs", label="Export Hair IDs", addDynamicControl=True)
        # FIXME: these are not on the shape node!
#       ui.addSeparator()
#       ui.addControl("enableProcedural")
#       ui.addControl("dso")
templates.registerTranslatorUI(MeshTemplate, "mesh", "<built-in>")
templates.registerTranslatorUI(MeshTemplate, "nurbsSurface", "<built-in>")

class HairSystemTemplate(templates.ShapeTranslatorTemplate):
    def shaderCreate(self, attrName):
        cmds.setUITemplate('attributeEditorPresetsTemplate', pushTemplate=True)
        cmds.attrNavigationControlGrp("HairSystemTemplateShader", attribute=attrName, label="Hair Shader")
        cmds.setUITemplate(popTemplate=True)

    def shaderUpdate(self, attrName):
        cmds.attrNavigationControlGrp("HairSystemTemplateShader", edit=True, attribute=attrName)

    def minPixelCreate(self, attrName):
        cmds.setUITemplate('attributeEditorPresetsTemplate', pushTemplate=True)
        isEnabled = not (cmds.getAttr("%s.aiMode" % (attrName.split(".")[0])) is 1)
        cmds.attrFieldSliderGrp("HairTemplateMinPixelWidth", label="Min Pixel Width",
                            attribute=attrName, enable=isEnabled)
        cmds.setUITemplate(popTemplate=True)
    
    def minPixelUpdate(self, attrName):
        isEnabled = not (cmds.getAttr("%s.aiMode" % (attrName.split(".")[0])) is 1)
        cmds.attrFieldSliderGrp("HairTemplateMinPixelWidth", edit=True,
                            attribute=attrName, enable=isEnabled)

    def modeChanged(self, *args):
        try:
            if cmds.getAttr(self.nodeAttr('aiMode')) == 1:
                cmds.attrFieldSliderGrp("HairTemplateMinPixelWidth", edit=True, enable=False)
            else:
                cmds.attrFieldSliderGrp("HairTemplateMinPixelWidth", edit=True, enable=True)
        except RuntimeError:
            # this callback runs immediately, before HairTemplateMinPixelWidth exists
            pass

    def setup(self):
        self.addControl("primaryVisibility")
        self.addControl("castsShadows")
        self.addSeparator()
        self.commonShapeAttributes()
        self.addSeparator()
        self.addControl("aiExportHairIDs", label="Export Hair IDs")
        self.addControl("aiExportHairUVs", label="Export Hair UVs")
        self.addControl("aiExportHairColors", label="Export Hair Colors")
        self.addControl("aiOverrideHair", label="Override Hair")
        self.addCustom("aiHairShader", self.shaderCreate, self.shaderUpdate)
        self.addSeparator()
        self.addCustom("aiMinPixelWidth", self.minPixelCreate, self.minPixelUpdate)
        self.addControl("aiMode", label="Mode", changeCommand=self.modeChanged)
        self.addSeparator()
        self.addControl("aiUserOptions", label="User Options")
templates.registerAETemplate(HairSystemTemplate, "hairSystem")

class FLuidShapeTemplate(templates.ShapeTranslatorTemplate):
    def setup(self):
        self.addControl("aiStepSize", label="Step Size")
        self.addControl("aiStepSizeShadowMult", label="Step Size Shadow Multiplier")
        self.addControl("aiStepSizeBlurryMult", label="Step Size Blurry Multiplier")
        self.addControl("aiPhaseFunc", label="Phase Function")
        self.addControl("aiShadowDensity", label="Shadow Density")
        self.addSeparator()
        self.addControl("aiUserOptions", label="User Options")
templates.registerAETemplate(FLuidShapeTemplate, "fluidShape")

class NurbsCurveTemplate(templates.ShapeTranslatorTemplate):
    def minPixelCreate(self, attrName):
        cmds.setUITemplate('attributeEditorPresetsTemplate', pushTemplate=True)
        isEnabled = not (cmds.getAttr("%s.aiMode" % (attrName.split(".")[0])) is 1)
        cmds.attrFieldSliderGrp("NurbsCurveTemplateMinPixelWidth", label="Min Pixel Width",
                            attribute=attrName, enable=isEnabled)
        cmds.setUITemplate(popTemplate=True)
    
    def minPixelUpdate(self, attrName):
        isEnabled = not (cmds.getAttr("%s.aiMode" % (attrName.split(".")[0])) is 1)
        cmds.attrFieldSliderGrp("NurbsCurveTemplateMinPixelWidth", edit=True,
                            attribute=attrName, enable=isEnabled)

    def modeChanged(self, *args):
        try:
            if cmds.getAttr(self.nodeAttr('aiMode')) == 1:
                cmds.attrFieldSliderGrp("NurbsCurveTemplateMinPixelWidth", edit=True, enable=False)
            else:
                cmds.attrFieldSliderGrp("NurbsCurveTemplateMinPixelWidth", edit=True, enable=True)
        except RuntimeError:
            # this callback runs immediately, before NurbsCurveTemplateMinPixelWidth exists
            pass
            
    def setup(self):
        #pm.mel.eval('AEaddRampControl("widthProfile")')
        #pm.mel.eval('AEaddRampControl("colorTable")')
        self.addControl("aiRenderCurve")
        self.addControl("aiCurveWidth")
        self.addControl("aiSampleRate")
        self.addControl("aiCurveShader")
        self.addSeparator()
        self.addControl("primaryVisibility")
        self.addControl("castsShadows")
        self.commonShapeAttributes()
        self.addSeparator()
        self.addCustom("aiMinPixelWidth", self.minPixelCreate, self.minPixelUpdate)
        self.addControl("aiMode", label="Mode", changeCommand=self.modeChanged)
        self.addSeparator()
        self.addControl("aiUserOptions", label="User Options")
templates.registerTranslatorUI(NurbsCurveTemplate, "nurbsCurve", "<built-in>")


class AmbientLightTemplate(lightTemplate.LightTemplate):
    def setup(self):
        self.setupColorTemperature("Ambient")
        self.addControl("aiNormalize")  

        self.addSeparator()
        
        self.addControl("aiCastShadows")
        self.addControl("aiShadowDensity")

        self.addSeparator()
        self.commonLightAttributes()
templates.registerTranslatorUI(AmbientLightTemplate, "ambientLight")

class DirectionalLightTemplate(lightTemplate.LightTemplate):
    def setup(self):
        self.setupColorTemperature("Directional")
        self.addControl("aiExposure")
        self.addControl("aiAngle")
        
        self.addSeparator()
        
        self.addControl("aiSamples")
        self.addControl("aiNormalize")
        
        self.addSeparator()
        
        self.addControl("aiCastShadows")
        self.addControl("aiShadowDensity")

        self.addSeparator()
        
        self.addControl("aiAffectVolumetrics")
        self.addControl("aiCastVolumetricShadows")
        
        self.addSeparator()                
        self.commonLightAttributes()

templates.registerTranslatorUI(DirectionalLightTemplate, "directionalLight")

class PointLightTemplate(lightTemplate.LightTemplate):
    def setup(self):
        self.setupColorTemperature("Point")
        self.addControl("aiDecayType")
        self.addControl("aiExposure")
        
        self.addSeparator()
        
        self.addControl("aiSamples")
        self.addControl("aiRadius")
        self.addControl("aiNormalize")

        self.addSeparator()

        self.addControl("aiCastShadows")
        self.addControl("aiShadowDensity")

        self.addSeparator()

        self.addControl("aiAffectVolumetrics")
        self.addControl("aiCastVolumetricShadows")
        
        self.addSeparator()
        self.commonLightAttributes()

templates.registerTranslatorUI(PointLightTemplate, "pointLight")

class SpotLightTemplate(lightTemplate.LightTemplate):
    def setup(self):
        self.setupColorTemperature("Spot")
        self.addControl("aiDecayType")
        self.addControl("aiExposure")
        
        self.addSeparator()
                        
        self.addControl("aiSamples")
        self.addControl("aiRadius")
        self.addControl("aiNormalize")

        self.addSeparator()

        self.addControl("aiCastShadows")
        self.addControl("aiShadowDensity")

        self.addSeparator()

        self.addControl("aiAffectVolumetrics")
        self.addControl("aiCastVolumetricShadows")

        self.addSeparator()

        self.addControl("aiAspectRatio")
        self.addControl("aiLensRadius")

        self.addSeparator()

        self.commonLightAttributes()

templates.registerTranslatorUI(SpotLightTemplate, "spotLight")

class AreaLightTemplate(lightTemplate.LightTemplate):
    def setup(self):
        self.setupColorTemperature("Area")
        self.addControl("aiDecayType")
        self.addControl("aiExposure")
        
        self.addSeparator()
        
        self.addControl("aiSamples")
        self.addControl("aiNormalize")

        self.addSeparator()

        self.addControl("aiCastShadows")
        self.addControl("aiShadowDensity")

        self.addSeparator()

        self.addControl("aiResolution")
        
        self.addSeparator()

        self.addControl("aiAffectVolumetrics")
        self.addControl("aiCastVolumetricShadows")

        self.addSeparator()

        self.commonLightAttributes()

templates.registerTranslatorUI(AreaLightTemplate, "areaLight")

# Actually currently connecting the other way round, filter's decayRate
# to light's decay type which might be the best idea
"""
def lightDecayChanged(decayPlug, *args):
    "called to sync first found lightDecay filter when decayRate changes"
    # fnCam = om.MFnCamera(transPlug.node())
    # currTrans = transPlug.asString()
    #orthoPlug = fnCam.findPlug('orthographic')
    # isOrtho = orthoPlug.asBool()
    print "lightDecayChanged", decayPlug.name(), decayPlug.asInt()
    print "filters", lightTemplate.LightTemplate.getConnectedLightFilters()
    # aiLightDecay

print "Adding attribute changed callback for lights"
callbacks.addAttributeChangedCallback(lightDecayChanged, 'pointLight', 'decayRate')
callbacks.addAttributeChangedCallback(lightDecayChanged, 'spotLight', 'decayRate')
callbacks.addAttributeChangedCallback(lightDecayChanged, 'areaLight', 'decayRate')
callbacks.addAttributeChangedCallback(lightDecayChanged, 'aiAreaLight', 'decayRate')
"""

templates.registerAETemplate(templates.TranslatorControl, "camera", label="Camera Type")

class CameraTemplate(templates.AttributeTemplate):

    def addCommonAttributes(self):
        self.addControl("aiExposure")
        
    def addDOFAttributes(self):
        self.addSeparator()
        self.addControl("aiEnableDOF", label="Enable DOF")
        self.addControl("aiFocusDistance")
        self.addControl("aiApertureSize")
        self.addControl("aiApertureBlades")
        self.addControl("aiApertureBladeCurvature")
        self.addControl("aiApertureRotation")

class PerspCameraTemplate(CameraTemplate):
    def setup(self):
        self.addCommonAttributes()
        self.addDOFAttributes()
        self.addSeparator()
        self.addControl('aiUvRemap', label="UV Remap")
        self.addSeparator()
        self.addControl("aiUserOptions", label="User Options")

templates.registerTranslatorUI(PerspCameraTemplate, "camera", "perspective")
templates.registerTranslatorUI(PerspCameraTemplate, "stereoRigCamera", "perspective")


class OrthographicTemplate(CameraTemplate):
    def setup(self):
        self.addCommonAttributes()
        self.addSeparator()
        self.addControl("aiUserOptions", label="User Options")

templates.registerTranslatorUI(OrthographicTemplate, "camera", "orthographic")
templates.registerTranslatorUI(OrthographicTemplate, "stereoRigCamera", "orthographic")

class FisheyeCameraTemplate(CameraTemplate):
    def setup(self):
        self.addCommonAttributes()
        self.addDOFAttributes()
        self.addSeparator()
        self.addControl('aiFov')
        self.addControl('aiAutocrop')
        self.addSeparator()
        self.addControl("aiUserOptions", label="User Options")

templates.registerTranslatorUI(FisheyeCameraTemplate, "camera", "fisheye")
templates.registerTranslatorUI(FisheyeCameraTemplate, "stereoRigCamera", "fisheye")

class CylCameraTemplate(CameraTemplate):
    def setup(self):
        self.addCommonAttributes()
        self.addControl('aiHorizontalFov')
        self.addControl('aiVerticalFov')
        self.addControl('aiProjective')
        self.addSeparator()
        self.addControl("aiUserOptions", label="User Options")

templates.registerTranslatorUI(CylCameraTemplate, "camera", "cylindrical")
templates.registerTranslatorUI(CylCameraTemplate, "stereoRigCamera", "cylindrical")

class SphericalCameraTemplate(CameraTemplate):
    def setup(self):
        self.addCommonAttributes()
        self.addSeparator()
        self.addControl("aiUserOptions", label="User Options")

templates.registerTranslatorUI(SphericalCameraTemplate, "camera", "spherical")
templates.registerTranslatorUI(SphericalCameraTemplate, "stereoRigCamera", "spherical")

def cameraOrthographicChanged(orthoPlug, *args):
    "called to sync .aiTranslator when .orthographic changes"
    if not core.arnoldIsCurrentRenderer(): return
    fnCam = om.MFnCamera(orthoPlug.node())
    transPlug = fnCam.findPlug('aiTranslator')
    if not transPlug.isNull():
        isOrtho = orthoPlug.asBool()
        
        currTrans = transPlug.asString()
        #print "cameraOrthographicChanged", fnCam.name(), currTrans, isOrtho
        newTrans = None
        if isOrtho and currTrans != 'orthographic':
            newTrans = 'orthographic'
        elif not isOrtho and currTrans == 'orthographic':
            newTrans = 'perspective'
        #print "newTrans", newTrans
        if newTrans:
            transPlug.setString(newTrans)

def cameraTranslatorChanged(transPlug, *args):
    "called to sync .orthographic when .aiTranslator changes"
    if not core.arnoldIsCurrentRenderer(): return
    fnCam = om.MFnCamera(transPlug.node())
    currTrans = transPlug.asString()
    orthoPlug = fnCam.findPlug('orthographic')
    isOrtho = orthoPlug.asBool()
    #print "cameraTranslatorChanged", fnCam.name(), currTrans, isOrtho
    # when a file is opening, we need to choose one attribute to lead, because
    # the order that attributes are set is unpredictable. This fixes a case
    # where translators may have gotten out of sync
    if om.MFileIO.isOpeningFile():
        if isOrtho and currTrans != 'orthographic':
            orthoPlug.setBool(True)
    else:
        if not isOrtho and currTrans == 'orthographic':
            orthoPlug.setBool(True)
        elif isOrtho and currTrans != 'orthographic':
            orthoPlug.setBool(False)

def getCameraDefault(obj):
    isOrtho = pm.api.MFnDependencyNode(obj).findPlug("orthographic").asBool()
    default = 'orthographic' if isOrtho else 'perspective'
    return default

templates.registerDefaultTranslator('camera', getCameraDefault)
templates.registerDefaultTranslator('stereoRigCamera', getCameraDefault)

callbacks.addAttributeChangedCallbacks('camera',
                                       [('aiTranslator', cameraTranslatorChanged),
                                        ('orthographic', cameraOrthographicChanged)])

callbacks.addAttributeChangedCallbacks('stereoRigCamera',
                                       [('aiTranslator', cameraTranslatorChanged),
                                        ('orthographic', cameraOrthographicChanged)])

def registerDriverTemplates():
    # register driver templates
    for transName, arnoldNode in core.listTranslators("aiAOVDriver"):
        templates.registerAutoTranslatorUI(arnoldNode, "aiAOVDriver", transName, skipEmpty=True)

    templates.registerDefaultTranslator('aiAOVDriver', 'exr')

def registerFilterTemplates():
    # register driver templates
    for transName, arnoldNode in core.listTranslators("aiAOVFilter"):
        templates.registerAutoTranslatorUI(arnoldNode, "aiAOVFilter", transName, skipEmpty=True)

    templates.registerDefaultTranslator('aiAOVFilter', 'gaussian')

registerDriverTemplates()
registerFilterTemplates()

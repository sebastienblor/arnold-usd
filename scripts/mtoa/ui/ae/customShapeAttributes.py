import pymel.core as pm
import maya.OpenMaya as om
import mtoa.ui.ae.lightTemplate as lightTemplate
from mtoa.ui.ae.utils import aeCallback
import mtoa.ui.ae.shapeTemplate as templates
import mtoa.callbacks as callbacks
import mtoa.core as core

def renderStatsAttributes(ui):
    ui.addAttribute("castsShadows")
    ui.addAttribute("receiveShadows")
    ui.addAttribute("primaryVisibility")
    ui.addAttribute("visibleInReflections")
    ui.addAttribute("visibleInRefractions")

def commonShapeAttributes(ui):
    ui.addAttribute("aiSelfShadows")
    ui.addAttribute("aiOpaque")
    ui.addAttribute("aiVisibleInDiffuse")
    ui.addAttribute("aiVisibleInGlossy")

def subdivDicingCameraNew(attrName):
    pm.setUITemplate('attributeEditorTemplate', pst=True)
    pm.attrNavigationControlGrp( 'aiSubdivDicingCameraCtrl',
                                    at=attrName,
                                    label="Subdivision Dicing Camera" )
    pm.setUITemplate(ppt=True)

def subdivDicingCameraReplace(attrName):
    pm.attrNavigationControlGrp(  'aiSubdivDicingCameraCtrl', edit=True,
                                    at=attrName )
    # pm.editorTemplate("aiSubdivDicingCamera", label="Subdivision Dicing Camera", addDynamicControl=True)
    pm.editorTemplate(aeCallback(subdivDicingCameraNew), aeCallback(subdivDicingCameraReplace), "aiSubdivDicingCamera", callCustom=True)


@templates.translatorUI("mesh", "<built-in>")
def builtin_mesh(ui):
    commonShapeAttributes(ui)
    ui.addSeparator()
    ui.addAttribute("aiSubdivType", label="Subdivision Type")
    ui.addAttribute("aiSubdivIterations", label="Subdivision Iterations")
    ui.addAttribute("aiSubdivAdaptiveMetric", label="Subdivision Adaptive Metric")
    ui.addAttribute("aiSubdivPixelError", label="Subdivision Pixel Error")
    # TODO: add dicing camera UI
    ui.addAttribute("aiSubdivDicingCamera", label="Subdivision Dicing Camera")
    ui.addAttribute("aiSubdivUvSmoothing", label="Subdivision UVs Smoothing")
    ui.addSeparator()
    ui.addAttribute("aiSssSampleDistribution", label="SSS Samples Distribution")
    ui.addAttribute("aiSssSampleSpacing", label="SSS Sample Spacing")
    ui.addSeparator()
    ui.addAttribute("aiExportTangents")
    ui.addAttribute("aiExportColors")
    #pm.editorTemplate("aiExportHairIDs", label="Export Hair IDs", addDynamicControl=True)
    # FIXME: these are not on the shape node!
#    ui.addSeparator()
#
#    ui.addAttribute("enableProcedural")
#    ui.addAttribute("dso")

@templates.translatorUI("hairSystem", "<built-in>")
def builtin_hairSystem(ui):
    commonShapeAttributes(ui)
    ui.addSeparator()
    ui.addAttribute("aiOverrideHair")
    ui.addAttribute("aiHairShader")
    ui.addSeparator()
    ui.addAttribute("aiMinPixelWidth")
    ui.addAttribute("aiMode")

class AmbientLightTemplate(lightTemplate.LightTemplate):
    # TODO: handle filter association via metadata
    def validFilters(self):
        return ['aiLightBlocker', 'aiLightDecay']

    def setup(self):
        self.commonLightAttributes()
templates.registerTranslatorUI(AmbientLightTemplate, "ambientLight")

class DirectionalLightTemplate(lightTemplate.LightTemplate):
    # TODO: handle filter association via metadata
    def validFilters(self):
        return ['aiLightBlocker']
    def setup(self):
        self.addAttribute("aiCastShadows")
        self.addAttribute("aiExposure")
        self.addAttribute("aiAngle")
        self.addAttribute("aiSamples")
        self.addAttribute("aiMis", label="Multiple Importance Sampling")
        self.addSeparator()
        self.commonLightAttributes()
templates.registerTranslatorUI(DirectionalLightTemplate, "directionalLight")

class PointLightTemplate(lightTemplate.LightTemplate):
    # TODO: handle filter association via metadata
    def validFilters(self):
        return ['aiLightBlocker', 'aiLightDecay']
    def setup(self):
        self.addAttribute("aiCastShadows")
        self.addAttribute("aiExposure")
        self.addAttribute("aiRadius")
        self.addAttribute("aiSamples")
        self.addAttribute("aiMis", label="Multiple Importance Sampling")
        self.addSeparator()
        self.addAttribute("aiAffectVolumetrics")
        self.addAttribute("aiCastVolumetricShadows")
        self.addSeparator()
        self.commonLightAttributes()
templates.registerTranslatorUI(PointLightTemplate, "pointLight")

class SpotLightTemplate(lightTemplate.LightTemplate):
    # TODO: handle filter association via metadata
    def validFilters(self):
        return ['aiLightBlocker', 'aiLightDecay', 'aiBarndoor', 'aiGobo']
    def setup(self):
        self.addAttribute("aiCastShadows")
        self.addAttribute("aiExposure")
        self.addAttribute("aiRadius")
        self.addAttribute("aiSamples")
        self.addAttribute("aiMis", label="Multiple Importance Sampling")

        self.addSeparator()

        self.addAttribute("aiAffectVolumetrics")
        self.addAttribute("aiCastVolumetricShadows")

        self.addSeparator()

        self.addAttribute("aiAspectRatio")
        self.addAttribute("aiLensRadius")

        self.addSeparator()

        self.commonLightAttributes()
templates.registerTranslatorUI(SpotLightTemplate, "spotLight")

class AreaLightTemplate(lightTemplate.LightTemplate):
    # TODO: handle filter association via metadata
    def validFilters(self):
        return ['aiLightBlocker', 'aiLightDecay']

    def setup(self):
        self.addAttribute("aiCastShadows")
        self.addAttribute("aiExposure")
        self.addAttribute("aiSamples")
        self.addAttribute("aiMis", label="Multiple Importance Sampling")

        self.addSeparator()

        self.addAttribute("aiResolution")
        self.addAttribute("aiAffectVolumetrics")
        self.addAttribute("aiCastVolumetricShadows")

        self.addSeparator()

        self.commonLightAttributes()

templates.registerTranslatorUI(AreaLightTemplate, "areaLight")

class CameraTemplate(templates.ArnoldTranslatorTemplate):
    def addDOFAttributes(self):
        self.addAttribute("aiEnableDOF")
        self.addSeparator()
        self.addAttribute("aiFocusDistance")
        self.addAttribute("aiApertureSize")
        self.addAttribute("aiApertureBlades")
        self.addAttribute("aiApertureBladeCurvature")
        self.addAttribute("aiApertureRotation")

class PerspCameraTemplate(CameraTemplate):
    def setup(self):
        self.addDOFAttributes()
        self.addSeparator()
        self.addAttribute('aiUvRemap')

templates.registerTranslatorUI(PerspCameraTemplate, "camera", "perspective")
templates.registerTranslatorUI(PerspCameraTemplate, "stereoRigCamera", "perspective")


class OrthographicTemplate(CameraTemplate):
    def setup(self):
        pass

templates.registerTranslatorUI(OrthographicTemplate, "camera", "orthographic")
templates.registerTranslatorUI(OrthographicTemplate, "stereoRigCamera", "orthographic")

class FisheyeCameraTemplate(CameraTemplate):
    def setup(self):
        self.addDOFAttributes()
        self.addSeparator()
        self.addAttribute('aiFov')
        self.addAttribute('aiAutocrop')

templates.registerTranslatorUI(FisheyeCameraTemplate, "camera", "fisheye")
templates.registerTranslatorUI(FisheyeCameraTemplate, "stereoRigCamera", "fisheye")

class CylCameraTemplate(CameraTemplate):
    def setup(self):
        self.addAttribute('aiHorizontalFov')
        self.addAttribute('aiVerticalFov')
        self.addAttribute('aiProjective')

templates.registerTranslatorUI(CylCameraTemplate, "camera", "cylindrical")
templates.registerTranslatorUI(CylCameraTemplate, "stereoRigCamera", "cylindrical")

def cameraOrthographicChanged(orthoPlug, *args):
    "called to sync .aiTranslator when .orthographic changes"
    fnCam = om.MFnCamera(orthoPlug.node())
    isOrtho = orthoPlug.asBool()
    transPlug = fnCam.findPlug('aiTranslator')
    currTrans = transPlug.asString()
    #print "cameraOrthographicChanged", fnCam.name(), currTrans, isOrtho
    newTrans = None
    if isOrtho and currTrans != 'orthographic':
        newTrans = 'orthographic'
    elif not isOrtho and currTrans == 'orthographic':
        newTrans = 'perspective'
    #print "newTrans", newTrans
    if newTrans:
        if pm.optionMenuGrp('aiTranslatorOMG', exists=True):
            pm.optionMenuGrp('aiTranslatorOMG', edit=True, value=newTrans)
        transPlug.setString(newTrans)

def cameraTranslatorChanged(transPlug, *args):
    "called to sync .orthographic when .aiTranslator changes"
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

def getCameraDefault(cam):
    default = 'orthographic' if pm.getAttr(cam + '.orthographic') else 'perspective'
    return default

templates.registerDefaultTranslator('camera', getCameraDefault)
templates.registerDefaultTranslator('stereoRigCamera', getCameraDefault)

print "Adding attribute changed callback for camera"
callbacks.addAttributeChangedCallbacks('camera',
                                       [('aiTranslator', cameraTranslatorChanged),
                                        ('orthographic', cameraOrthographicChanged)])

callbacks.addAttributeChangedCallbacks('stereoRigCamera',
                                       [('aiTranslator', cameraTranslatorChanged),
                                        ('orthographic', cameraOrthographicChanged)])

def registerDriverTemplates():
    # register driver templates
    for transName, arnoldNode in core.listTranslators("<driver>"):
        transName = str(transName) # doesn't like unicode
        cls = type('Driver_%sTemplate' % transName, (templates.AutoTranslatorTemplate,), dict(_arnoldNodeType=arnoldNode))
        templates.registerTranslatorUI(cls, "<driver>", transName)
    
    #templates.registerDefaultTranslator('<driver>', 'exr')

def registerFilterTemplates():
    # register driver templates
    for transName, arnoldNode in core.listTranslators("<filter>"):
        transName = str(transName) # doesn't like unicode
        cls = type('Filter_%sTemplate' % transName, (templates.AutoTranslatorTemplate,), dict(_arnoldNodeType=arnoldNode))
        templates.registerTranslatorUI(cls, "<filter>", transName)

    #templates.registerDefaultTranslator('<filter>', 'gaussian')

registerDriverTemplates()
registerFilterTemplates()

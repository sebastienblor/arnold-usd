import maya.cmds as cmds
import maya.OpenMaya as om
import mtoa.ui.ae.lightTemplate as lightTemplate
from mtoa.ui.ae.utils import aeCallback
from mtoa.ui.ae.shapeTemplate import registerUI, registerDefaultTranslator, ArnoldTranslatorTemplate, AutoTranslatorTemplate, registerTranslatorUI
import mtoa.callbacks as callbacks

def commonLightAttributes(ui):
    ui.addAttribute("aiNormalize")
    ui.addAttribute("aiBounceFactor")
    ui.addAttribute("aiBounces")
    ui.addSeparator()
    ui.addSeparator()
    ui.addAttribute("aiSssSampleDistribution", label="SSS Samples Distribution")
#    cmds.editorTemplate(beginLayout="Light Filters")
#
#    cmds.editorTemplate(aeCallback(lightFiltersTemplate.customLightFiltersNew), aeCallback(lightFiltersTemplate.customLightFiltersReplace), "aiLightFilters", callCustom=True)
#
#    cmds.editorTemplate(endLayout=True)

def renderStatsAttributes(ui):
    ui.addAttribute("castsShadows")
    ui.addAttribute("primaryVisibility")
    ui.addAttribute("visibleInReflections")
    ui.addAttribute("visibleInRefractions")

def commonShapeAttributes(ui):
    ui.addAttribute("aiSelfShadows")
    ui.addAttribute("aiOpaque")
    ui.addAttribute("aiVisibleInDiffuse")
    ui.addAttribute("aiVisibleInGlossy")


def subdivDicingCameraNew(attrName):
    cmds.setUITemplate('attributeEditorTemplate', pst=True)
    cmds.attrNavigationControlGrp( 'aiSubdivDicingCameraCtrl',
                                    at=attrName,
                                    label="Subdivision Dicing Camera" )
    cmds.setUITemplate(ppt=True)

def subdivDicingCameraReplace(attrName):
    cmds.attrNavigationControlGrp(  'aiSubdivDicingCameraCtrl', edit=True,
                                    at=attrName )
    # cmds.editorTemplate("aiSubdivDicingCamera", label="Subdivision Dicing Camera", addDynamicControl=True)
    cmds.editorTemplate(aeCallback(subdivDicingCameraNew), aeCallback(subdivDicingCameraReplace), "aiSubdivDicingCamera", callCustom=True)


@registerUI("mesh", "<built-in>")
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
    ui.addSeparator()
    ui.addAttribute("aiExportTangents")
    ui.addAttribute("aiExportColors")
    #cmds.editorTemplate("aiExportHairIDs", label="Export Hair IDs", addDynamicControl=True)
    # FIXME: these are not on the shape node!
#    ui.addSeparator()
#
#    ui.addAttribute("enableProcedural")
#    ui.addAttribute("dso")

@registerUI("hairSystem", "<built-in>")
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
registerTranslatorUI(AmbientLightTemplate, "ambientLight")

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
registerTranslatorUI(DirectionalLightTemplate, "directionalLight")

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
registerTranslatorUI(PointLightTemplate, "pointLight")

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
registerTranslatorUI(SpotLightTemplate, "spotLight")

class CameraTemplate(ArnoldTranslatorTemplate):
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

registerTranslatorUI(PerspCameraTemplate, "camera", "perspective")

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

registerTranslatorUI(AreaLightTemplate, "areaLight")

class OrthographicTemplate(CameraTemplate):
    def setup(self):
        pass

registerTranslatorUI(OrthographicTemplate, "camera", "orthographic")

class FisheyeCameraTemplate(CameraTemplate):
    def setup(self):
        self.addDOFAttributes()
        self.addSeparator()
        self.addAttribute('aiFov')
        self.addAttribute('aiAutocrop')

registerTranslatorUI(FisheyeCameraTemplate, "camera", "fisheye")

class CylCameraTemplate(CameraTemplate):
    def setup(self):
        self.addAttribute('aiHorizontalFov')
        self.addAttribute('aiVerticalFov')
        self.addAttribute('aiProjective')

registerTranslatorUI(CylCameraTemplate, "camera", "cylindrical")

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
        if cmds.optionMenuGrp('aiTranslatorOMG', exists=True):
            cmds.optionMenuGrp('aiTranslatorOMG', edit=True, value=newTrans)
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
    default = 'orthographic' if cmds.getAttr(cam + '.orthographic') else 'perspective'
    return default

registerDefaultTranslator('camera', getCameraDefault)

print "Adding attribute changed callback for camera"
callbacks.addAttributeChangedCallbacks('camera',
                                       [('aiTranslator', cameraTranslatorChanged),
                                        ('orthographic', cameraOrthographicChanged)])

def registerDriverTemplates():
    # register driver templates
    for transName in cmds.arnoldPlugins(listTranslators="<driver>"):
        transName = str(transName) # doesn't like unicode
        # FIXME: create a real relationship between the arnold node and the translator name
        arnoldNode = 'driver_%s' % transName
        cls = type('Driver_%sTemplate' % transName, (AutoTranslatorTemplate,), dict(_arnoldNodeType=arnoldNode))
        registerTranslatorUI(cls, "<driver>", transName)
    
    #registerDefaultTranslator('<driver>', 'exr')

def registerFilterTemplates():
    # register driver templates
    for transName in cmds.arnoldPlugins(listTranslators="<filter>"):
        transName = str(transName) # doesn't like unicode
        # FIXME: create a real relationship between the arnold node and the translator name
        arnoldNode = '%s_filter' % transName
        cls = type('Filter_%sTemplate' % transName, (AutoTranslatorTemplate,), dict(_arnoldNodeType=arnoldNode))
        registerTranslatorUI(cls, "<filter>", transName)

    #registerDefaultTranslator('<filter>', 'gaussian')

registerDriverTemplates()
registerFilterTemplates()

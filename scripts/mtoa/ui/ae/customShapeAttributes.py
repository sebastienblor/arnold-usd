import maya.cmds as cmds
import maya.OpenMaya as om
import mtoa.ui.ae.lightFiltersTemplate as lightFiltersTemplate
from mtoa.ui.ae.utils import aeCallback
from mtoa.ui.ae.shapeTemplate import registerUI, registerDefaultTranslator, ArnoldTranslatorTemplate
import mtoa.callbacks as callbacks

def overrideSssToggle(attrName):
    if cmds.getAttr(attrName+".aiOverrideSssSamples"):
        cmds.editorTemplate(dimControl=(attrName, "aiSssSamples", False))
    else:
        cmds.editorTemplate(dimControl=(attrName, "aiSssSamples", True))

def commonLightAttributes(nodeName):
    cmds.editorTemplate("aiNormalize", label="Normalize", addDynamicControl=True)
    cmds.editorTemplate("aiBounceFactor", label="Bounce Factor", addDynamicControl=True)
    cmds.editorTemplate("aiBounces", label="Bounces", addDynamicControl=True)

    cmds.editorTemplate(addSeparator=True)

    cmds.editorTemplate("aiOverrideSssSamples", aeCallback(overrideSssToggle), label="Override SSS Samples", addDynamicControl=True)
    cmds.editorTemplate("aiSssSamples", label="SSS Samples", addDynamicControl=True)

    cmds.editorTemplate(beginLayout="Light Filters")

    cmds.editorTemplate(aeCallback(lightFiltersTemplate.customLightFiltersNew), aeCallback(lightFiltersTemplate.customLightFiltersReplace), "aiLightFilters", callCustom=True)

    cmds.editorTemplate(endLayout=True)


def commonShapeAttributes(nodeName):
    cmds.editorTemplate("aiSelfShadows", label="Self Shadows", addDynamicControl=True)
    cmds.editorTemplate("aiOpaque", label="Opaque", addDynamicControl=True)
    cmds.editorTemplate("aiVisibleInDiffuse", label="Visible In Diffuse", addDynamicControl=True)
    cmds.editorTemplate("aiVisibleInGlossy", label="Visible In Glossy", addDynamicControl=True)    

    cmds.editorTemplate(addSeparator=True)

    cmds.editorTemplate("aiSssUseGi", label="SSS Use Gi", addDynamicControl=True)
    cmds.editorTemplate("aiSssMaxSamples", label="SSS Max Samples", addDynamicControl=True)
    cmds.editorTemplate("aiSssSampleSpacing", label="SSS Sample Spacing", addDynamicControl=True)

@registerUI("mesh")
def builtin_mesh(nodeName):
    commonShapeAttributes(nodeName)
    cmds.editorTemplate(addSeparator=True)

    cmds.editorTemplate("aiSubdivType", label="Subdivision Type", addDynamicControl=True)
    cmds.editorTemplate("aiSubdivIterations", label="Subdivision Iterations", addDynamicControl=True)
    cmds.editorTemplate("aiSubdivAdaptiveMetric", label="Subdivision Adaptive Metric", addDynamicControl=True)
    cmds.editorTemplate("aiSubdivPixelError", label="Subdivision Pixel Error", addDynamicControl=True)
    cmds.editorTemplate("aiSubdivDicingCamera", label="Subdivision Dicing Camera", addDynamicControl=True)
    cmds.editorTemplate("aiSubdivUvSmoothing", label="Subdivision UVs Smoothing", addDynamicControl=True)

    cmds.editorTemplate(addSeparator=True)

    cmds.editorTemplate("aiExportTangents", label="Export Tangents", addDynamicControl=True)
    cmds.editorTemplate("aiExportColors", label="Export Colors", addDynamicControl=True)

    cmds.editorTemplate(addSeparator=True)
 
    cmds.editorTemplate("enableProcedural", addDynamicControl=True)
    cmds.editorTemplate("dso", addDynamicControl=True)

@registerUI("hairSystem")
def builtin_hairSystem(nodeName):
    commonShapeAttributes(nodeName)
    cmds.editorTemplate(addSeparator=True)

    cmds.editorTemplate("aiOverrideHair", label="Override Hair", addDynamicControl=True)
    cmds.editorTemplate("aiHairShader", label="Hair Shader", addDynamicControl=True)

    cmds.editorTemplate(addSeparator=True)

    cmds.editorTemplate("aiMinPixelWidth", label="Min Pixel Width", addDynamicControl=True)
    cmds.editorTemplate("aiMode", label="Mode", addDynamicControl=True)

@registerUI("ambientLight")
def builtin_ambientLight(nodeName):
    commonLightAttributes(nodeName);

@registerUI("directionalLight")
def builtin_directionalLight(nodeName):
    cmds.editorTemplate("aiCastShadows", label="Cast Shadows", addDynamicControl=True)
    cmds.editorTemplate("aiExposure", label="Exposure", addDynamicControl=True)
    cmds.editorTemplate("aiAngle", label="Angle", addDynamicControl=True)
    cmds.editorTemplate("aiSamples", label="Samples", addDynamicControl=True)
    cmds.editorTemplate("aiMis", label="Multiple Importance Sampling", addDynamicControl=True)

    cmds.editorTemplate(addSeparator=True)

    commonLightAttributes(nodeName);

@registerUI("pointLight")
def builtin_pointLight(nodeName):
    cmds.editorTemplate("aiCastShadows", label="Cast Shadows", addDynamicControl=True)
    cmds.editorTemplate("aiExposure", label="Exposure", addDynamicControl=True)
    cmds.editorTemplate("aiRadius", label="Radius", addDynamicControl=True)
    cmds.editorTemplate("aiSamples", label="Samples", addDynamicControl=True)
    cmds.editorTemplate("aiMis", label="Multiple Importance Sampling", addDynamicControl=True)

    cmds.editorTemplate(addSeparator=True)

    cmds.editorTemplate("aiAffectVolumetrics", label="Affect Volumetrics", addControl=True)
    cmds.editorTemplate("aiCastVolumetricShadows", label="Cast Volumetric Shadows", addControl=True)

    cmds.editorTemplate(addSeparator=True)

    commonLightAttributes(nodeName);

@registerUI("spotLight")
def builtin_spotLight(nodeName):
    cmds.editorTemplate("aiCastShadows", label="Cast Shadows", addDynamicControl=True)
    cmds.editorTemplate("aiExposure", label="Exposure", addDynamicControl=True)
    cmds.editorTemplate("aiRadius", label="Radius", addDynamicControl=True)
    cmds.editorTemplate("aiSamples", label="Samples", addDynamicControl=True)
    cmds.editorTemplate("aiMis", label="Multiple Importance Sampling", addDynamicControl=True)

    cmds.editorTemplate(addSeparator=True)

    cmds.editorTemplate("aiAffectVolumetrics", label="Affect Volumetrics", addControl=True)
    cmds.editorTemplate("aiCastVolumetricShadows", label="Cast Volumetric Shadows", addControl=True)

    cmds.editorTemplate(addSeparator=True)

    cmds.editorTemplate("aiAspectRatio", label="Aspect Ratio", addControl=True)
    cmds.editorTemplate("aiLensRadius", label="Lens Radius", addControl=True)

    cmds.editorTemplate(addSeparator=True)

    commonLightAttributes(nodeName);

@registerUI("areaLight")
def builtin_areaLight(nodeName):
    cmds.editorTemplate("aiCastShadows", label="Cast Shadows", addDynamicControl=True)
    cmds.editorTemplate("aiExposure", label="Exposure", addDynamicControl=True)
    cmds.editorTemplate("aiSamples", label="Samples", addDynamicControl=True)
    cmds.editorTemplate("aiMis", label="Multiple Importance Sampling", addDynamicControl=True)

    cmds.editorTemplate(addSeparator=True)

    cmds.editorTemplate("aiResolution", label="Importance Map Resolution",addControl=True)
    cmds.editorTemplate("aiAffectVolumetrics", label="Affect Volumetrics", addControl=True)
    cmds.editorTemplate("aiCastVolumetricShadows", label="Cast Volumetric Shadows", addControl=True)

    cmds.editorTemplate(addSeparator=True)

    commonLightAttributes(nodeName);


class CameraTemplate(ArnoldTranslatorTemplate):
    def __init__(self):
        ArnoldTranslatorTemplate.__init__(self)
    def addDOFAttributes(self):
        self.addAttribute("aiEnableDOF")
        self.addSeparator()
        self.addAttribute("aiFocusDistance")
        self.addAttribute("aiApertureSize")
        self.addAttribute("aiApertureBlades")
        self.addAttribute("aiApertureBladeCurvature")
        self.addAttribute("aiApertureRotation")

class PerspCameraTemplate(CameraTemplate):
    def __init__(self):
        CameraTemplate.__init__(self)
        self.addDOFAttributes()
        self.addSeparator()
        self.addAttribute('aiUvRemap')

PerspCameraTemplate.register("camera", "perspective")

class OrthographicTemplate(CameraTemplate):
    def __init__(self):
        CameraTemplate.__init__(self)

OrthographicTemplate.register("camera", "orthographic")

class FisheyeCameraTemplate(CameraTemplate):
    def __init__(self):
        CameraTemplate.__init__(self)
        self.addDOFAttributes()
        self.addSeparator()
        self.addAttribute('aiFov')
        self.addAttribute('aiAutocrop')

FisheyeCameraTemplate.register("camera", "fisheye")

class CylCameraTemplate(CameraTemplate):
    def __init__(self):
        CameraTemplate.__init__(self)
        self.addAttribute('aiHorizontalFov')
        self.addAttribute('aiVerticalFov')
        self.addAttribute('aiProjective')

CylCameraTemplate.register("camera", "cylindrical")

def cameraOrthographicChanged(orthoPlug):
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

def cameraTranslatorChanged(transPlug):
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




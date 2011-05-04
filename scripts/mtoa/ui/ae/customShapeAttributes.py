import maya.cmds as cmds
import maya.OpenMaya as om
import mtoa.ui.ae.lightFiltersTemplate as lightFiltersTemplate
from mtoa.ui.ae.utils import aeCallback
from mtoa.ui.ae.shapeTemplate import registerUI, registerDefaultTranslator, ArnoldTranslatorTemplate
import mtoa.callbacks as callbacks

def commonLightAttributes(nodeName):
    cmds.editorTemplate("normalize", addDynamicControl=True)
    cmds.editorTemplate("bounceFactor", addDynamicControl=True)
    cmds.editorTemplate("bounces", addDynamicControl=True)

    cmds.editorTemplate(addSeparator=True)

    cmds.editorTemplate("sssSamples", addDynamicControl=True)

    cmds.editorTemplate(beginLayout="Light Filters")

    cmds.editorTemplate(aeCallback(lightFiltersTemplate.customLightFiltersNew), aeCallback(lightFiltersTemplate.customLightFiltersReplace), "light_filters", callCustom=True)

    cmds.editorTemplate(endLayout=True)


def commonShapeAttributes(nodeName):
    cmds.editorTemplate("selfShadows", addDynamicControl=True)
    cmds.editorTemplate("opaque", addDynamicControl=True)
    cmds.editorTemplate("visibleInDiffuse", addDynamicControl=True)
    cmds.editorTemplate("visibleInGlossy", addDynamicControl=True)    

    cmds.editorTemplate(addSeparator=True)

    cmds.editorTemplate("sssUseGi", addDynamicControl=True)
    cmds.editorTemplate("sssMaxSamples", addDynamicControl=True)
    cmds.editorTemplate("sssSampleSpacing", addDynamicControl=True)

@registerUI("mesh")
def builtin_mesh(nodeName):
    commonShapeAttributes(nodeName)
    cmds.editorTemplate(addSeparator=True)

    cmds.editorTemplate("subdivType", addDynamicControl=True)
    cmds.editorTemplate("subdivIterations", addDynamicControl=True)
    cmds.editorTemplate("subdivAdaptiveMetric", addDynamicControl=True)
    cmds.editorTemplate("subdivPixelError", addDynamicControl=True)
    cmds.editorTemplate("subdivDicingCamera", addDynamicControl=True)
    cmds.editorTemplate("subdivUvSmoothing", label="Subdiv UV Smoothing", addDynamicControl=True)

    cmds.editorTemplate(addSeparator=True)

    cmds.editorTemplate("exportTangents", addDynamicControl=True)
    cmds.editorTemplate("exportColors", addDynamicControl=True)

@registerUI("hairSystem")
def builtin_hairSystem(nodeName):
    commonShapeAttributes(nodeName)
    cmds.editorTemplate(addSeparator=True)

    cmds.editorTemplate("overrideHair", addDynamicControl=True)
    cmds.editorTemplate("hairShader", addDynamicControl=True)

    cmds.editorTemplate(addSeparator=True)

    cmds.editorTemplate("minPixelWidth", addDynamicControl=True)
    cmds.editorTemplate("mode", addDynamicControl=True)

@registerUI("ambientLight")
def builtin_ambientLight(nodeName):
    commonLightAttributes(nodeName);

@registerUI("directionalLight")
def builtin_directionalLight(nodeName):
    cmds.editorTemplate("cast_shadows", addDynamicControl=True)
    cmds.editorTemplate("angle", addDynamicControl=True)
    cmds.editorTemplate("samples", addDynamicControl=True)

    cmds.editorTemplate(addSeparator=True)

    commonLightAttributes(nodeName);

@registerUI("pointLight")
def builtin_pointLight(nodeName):
    cmds.editorTemplate("cast_shadows", addDynamicControl=True)
    cmds.editorTemplate("radius", addDynamicControl=True)
    cmds.editorTemplate("samples", addDynamicControl=True)

    cmds.editorTemplate(addSeparator=True)

    cmds.editorTemplate("affect_volumetrics", addControl=True)
    cmds.editorTemplate("cast_volumetric_shadows", addControl=True)

    cmds.editorTemplate(addSeparator=True)

    commonLightAttributes(nodeName);

@registerUI("spotLight")
def builtin_spotLight(nodeName):
    cmds.editorTemplate("cast_shadows", addDynamicControl=True)
    cmds.editorTemplate("radius", addDynamicControl=True)
    cmds.editorTemplate("samples", addDynamicControl=True)

    cmds.editorTemplate(addSeparator=True)

    cmds.editorTemplate("affect_volumetrics", addControl=True)
    cmds.editorTemplate("cast_volumetric_shadows", addControl=True)

    cmds.editorTemplate(addSeparator=True)

    cmds.editorTemplate("aspect_ratio", addControl=True)
    cmds.editorTemplate( "lens_radius", addControl=True)

    cmds.editorTemplate(addSeparator=True)

    commonLightAttributes(nodeName);

@registerUI("areaLight")
def builtin_areaLight(nodeName):
    cmds.editorTemplate("cast_shadows", addDynamicControl=True)
    cmds.editorTemplate("samples", addDynamicControl=True)

    cmds.editorTemplate(addSeparator=True)

    cmds.editorTemplate("resolution", addControl=True)
    cmds.editorTemplate("affect_volumetrics", addControl=True)
    cmds.editorTemplate("cast_volumetric_shadows", addControl=True)

    cmds.editorTemplate(addSeparator=True)

    cmds.editorTemplate("sidedness", addControl=True)
    cmds.editorTemplate("solid_angle", addControl=True)

    cmds.editorTemplate(addSeparator=True)

    commonLightAttributes(nodeName);


class CameraTemplate(ArnoldTranslatorTemplate):
    def __init__(self):
        ArnoldTranslatorTemplate.__init__(self)
    def addDOFAttributes(self):
        self.addAttribute("enableDOF")
        self.addSeparator()
        self.addAttribute("focalDistance")
        self.addAttribute("apertureSize")
        self.addAttribute("apertureBlades")
        self.addAttribute("apertureBladeCurvature")
        self.addAttribute("apertureRotation")

class PerspCameraTemplate(CameraTemplate):
    def __init__(self):
        CameraTemplate.__init__(self)
        self.addDOFAttributes()
        self.addSeparator()
        self.addAttribute('uvRemap')

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
        self.addAttribute('fov')
        self.addAttribute('autocrop')

FisheyeCameraTemplate.register("camera", "fisheye")

class CylCameraTemplate(CameraTemplate):
    def __init__(self):
        CameraTemplate.__init__(self)
        self.addAttribute('horizontalFov')
        self.addAttribute('verticalFov')
        self.addAttribute('projective')

CylCameraTemplate.register("camera", "cylindrical")

def cameraOrthographicChanged(orthoPlug):
    "called to sync .arnoldTranslator when .orthographic changes"
    fnCam = om.MFnCamera(orthoPlug.node())
    isOrtho = orthoPlug.asBool()
    transPlug = fnCam.findPlug('arnoldTranslator')
    currTrans = transPlug.asString()
    #print "cameraOrthographicChanged", fnCam.name(), currTrans, isOrtho
    newTrans = None
    if isOrtho and currTrans != 'orthographic':
        newTrans = 'orthographic'
    elif not isOrtho and currTrans == 'orthographic':
        newTrans = 'perspective'
    #print "newTrans", newTrans
    if newTrans:
        if cmds.optionMenuGrp('arnoldTranslatorOMG', exists=True):
            cmds.optionMenuGrp('arnoldTranslatorOMG', edit=True, value=newTrans)
        transPlug.setString(newTrans)

def cameraTranslatorChanged(transPlug):
    "called to sync .orthographic when .arnoldTranslator changes"
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
                                       [('arnoldTranslator', cameraTranslatorChanged),
                                        ('orthographic', cameraOrthographicChanged)])




import pymel.core as pm
import maya.cmds as cmds
import maya.OpenMaya as om
import mtoa.ui.ae.lightTemplate as lightTemplate
from mtoa.ui.ae.utils import aeCallback
import mtoa.ui.ae.shapeTemplate as templates
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
        self.addControl("aiInheritCacheTransform", label="Inherit Cache Transform")
        self.addControl("aiInterpolateBlur", label="Interpolate Blur Steps")
        
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
        self.addControl("aiSubdivType", label="Subdivision Type")
        self.addControl("aiSubdivIterations", label="Subdivision Iterations")
        self.addControl("aiSubdivAdaptiveMetric", label="Subdivision Adaptive Metric")
        self.addControl("aiSubdivPixelError", label="Subdivision Pixel Error")
        # TODO: add dicing camera UI
        self.addControl("aiSubdivDicingCamera", label="Subdivision Dicing Camera")
        self.addControl("aiSubdivUvSmoothing", label="Subdivision UVs Smoothing")
        self.addControl("aiSubdivSmoothDerivs", label="Smooth Subdivision Tangents")
        self.addSeparator()
        self.addControl("aiSssSampleDistribution", label="SSS Samples Distribution")
        self.addControl("aiSssSampleSpacing", label="SSS Sample Spacing")
        self.addSeparator()
        self.addControl("aiExportTangents", label="Export Tangents")
        self.addControl("aiExportColors", label="Export Vertex Colors")
        self.addControl("aiExportRefNormals", label="Export Reference Normals")
        self.addControl("aiExportRefTangents", label="Export Reference Tangents")
        #pm.editorTemplate("aiExportHairIDs", label="Export Hair IDs", addDynamicControl=True)
        # FIXME: these are not on the shape node!
#       ui.addSeparator()
#       ui.addControl("enableProcedural")
#       ui.addControl("dso")
templates.registerTranslatorUI(MeshTemplate, "mesh", "<built-in>")
templates.registerTranslatorUI(MeshTemplate, "nurbsSurface", "<built-in>")

class HairSystemTemplate(templates.ShapeTranslatorTemplate):
    def setup(self):
        self.commonShapeAttributes()
        self.addSeparator()
        self.addControl("aiExportHairIDs")
        self.addControl("aiExportHairUVs")
        self.addControl("aiOverrideHair")
        self.addControl("aiHairShader")
        self.addSeparator()
        self.addControl("aiMinPixelWidth")
        self.addControl("aiMode")
templates.registerTranslatorUI(HairSystemTemplate, "hairSystem", "<built-in>")

class AmbientLightTemplate(lightTemplate.LightTemplate):
    # TODO: handle filter association via metadata
    def validFilters(self):
        return ['aiLightBlocker']

    def setup(self):
        self.addControl("aiNormalize")  

        self.addSeparator()
        
        self.addControl("aiCastShadows")

        self.addSeparator()
        self.commonLightAttributes()
templates.registerTranslatorUI(AmbientLightTemplate, "ambientLight")

class DirectionalLightTemplate(lightTemplate.LightTemplate):
    # TODO: handle filter association via metadata
    def validFilters(self):
        return ['aiLightBlocker']
    def setup(self):
        self.addControl("aiExposure")
        self.addControl("aiAngle")
        
        self.addSeparator()
        
        self.addControl("aiSamples")
        self.addControl("aiNormalize")
        
        self.addSeparator()
        
        self.addControl("aiCastShadows")

        self.addSeparator()

        self.commonLightAttributes()
templates.registerTranslatorUI(DirectionalLightTemplate, "directionalLight")

class PointLightTemplate(lightTemplate.LightTemplate):
    # TODO: handle filter association via metadata
    def validFilters(self):
        return ['aiLightBlocker', 'aiLightDecay']
    def setup(self):
        cmds.editorTemplate("aiDecayType", aeCallback(self.customDecayTypeAttach), label="Decay Type", addControl=True)
        self.addControl("aiExposure")
        
        self.addSeparator()
        
        self.addControl("aiSamples")
        self.addControl("aiRadius")
        self.addControl("aiNormalize")

        self.addSeparator()

        self.addControl("aiCastShadows")

        self.addSeparator()

        self.addControl("aiAffectVolumetrics")
        self.addControl("aiCastVolumetricShadows")
        self.addSeparator()
        self.commonLightAttributes()
    # attach Maya Decay Rate to Arnold Decay Type
    def customDecayTypeAttach(self, attr):
        nodeName = ""+attr
        if(cmds.attributeQuery("decayRate", node=nodeName, exists=True)):
            origAttrName = nodeName + ".decayRate"
            destAttrName = nodeName + ".aiDecayType"
            cmds.setAttr(destAttrName, lock=False)
            cmds.setAttr(origAttrName, lock=False)
            value = cmds.getAttr(origAttrName)
            cmds.setAttr(destAttrName, lock=False)
            cmds.setAttr(destAttrName, value/2)
            cmds.setAttr(destAttrName, lock=True)
templates.registerTranslatorUI(PointLightTemplate, "pointLight")

class SpotLightTemplate(lightTemplate.LightTemplate):
    # TODO: handle filter association via metadata
    def validFilters(self):
        return ['aiLightBlocker', 'aiLightDecay', 'aiBarndoor', 'aiGobo']
    def setup(self):
        cmds.editorTemplate("aiDecayType", aeCallback(self.customDecayTypeAttach), label="Decay Type", addControl=True)
        self.addControl("aiExposure")
        
        self.addSeparator()
                        
        self.addControl("aiSamples")
        self.addControl("aiRadius")
        self.addControl("aiNormalize")

        self.addSeparator()

        self.addControl("aiCastShadows")

        self.addSeparator()

        self.addControl("aiAffectVolumetrics")
        self.addControl("aiCastVolumetricShadows")

        self.addSeparator()

        self.addControl("aiAspectRatio")
        self.addControl("aiLensRadius")

        self.addSeparator()

        self.commonLightAttributes()
    # attach Maya Decay Rate to Arnold Decay Type
    def customDecayTypeAttach(self, attr):
        nodeName = ""+attr
        if(cmds.attributeQuery("decayRate", node=nodeName, exists=True)):
            origAttrName = nodeName + ".decayRate"
            destAttrName = nodeName + ".aiDecayType"
            cmds.setAttr(destAttrName, lock=False)
            cmds.setAttr(origAttrName, lock=False)
            value = cmds.getAttr(origAttrName)
            cmds.setAttr(destAttrName, lock=False)
            cmds.setAttr(destAttrName, value/2)
            cmds.setAttr(destAttrName, lock=True)
templates.registerTranslatorUI(SpotLightTemplate, "spotLight")

class AreaLightTemplate(lightTemplate.LightTemplate):
    # TODO: handle filter association via metadata
    def validFilters(self):
        return ['aiLightBlocker', 'aiLightDecay']

    def setup(self):
        cmds.editorTemplate("aiDecayType", aeCallback(self.customDecayTypeAttach), label="Decay Type", addControl=True)
        self.addControl("aiExposure")
        
        self.addSeparator()
        
        self.addControl("aiSamples")
        self.addControl("aiNormalize")

        self.addSeparator()

        self.addControl("aiCastShadows")

        self.addSeparator()

        self.addControl("aiResolution")
        
        self.addSeparator()

        self.addControl("aiAffectVolumetrics")
        self.addControl("aiCastVolumetricShadows")

        self.addSeparator()

        self.commonLightAttributes()
    # attach Maya Decay Rate to Arnold Decay Type
    def customDecayTypeAttach(self, attr):
        nodeName = ""+attr
        if(cmds.attributeQuery("decayRate", node=nodeName, exists=True)):
            origAttrName = nodeName + ".decayRate"
            destAttrName = nodeName + ".aiDecayType"
            cmds.setAttr(destAttrName, lock=False)
            cmds.setAttr(origAttrName, lock=False)
            value = cmds.getAttr(origAttrName)
            cmds.setAttr(destAttrName, lock=False)
            cmds.setAttr(destAttrName, value/2)
            cmds.setAttr(destAttrName, lock=True)
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
    def addDOFAttributes(self):
        self.addControl("aiEnableDOF")
        self.addSeparator()
        self.addControl("aiFocusDistance")
        self.addControl("aiApertureSize")
        self.addControl("aiApertureBlades")
        self.addControl("aiApertureBladeCurvature")
        self.addControl("aiApertureRotation")

class PerspCameraTemplate(CameraTemplate):
    def setup(self):
        self.addDOFAttributes()
        self.addSeparator()
        self.addControl('aiUvRemap')

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
        self.addControl('aiFov')
        self.addControl('aiAutocrop')

templates.registerTranslatorUI(FisheyeCameraTemplate, "camera", "fisheye")
templates.registerTranslatorUI(FisheyeCameraTemplate, "stereoRigCamera", "fisheye")

class CylCameraTemplate(CameraTemplate):
    def setup(self):
        self.addControl('aiHorizontalFov')
        self.addControl('aiVerticalFov')
        self.addControl('aiProjective')

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
    default = 'orthographic' if cam.orthographic.get() else 'perspective'
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
    for transName, arnoldNode in core.listTranslators("aiAOVDriver"):
        templates.registerAutoTranslatorUI(arnoldNode, "aiAOVDriver", transName)

    templates.registerDefaultTranslator('aiAOVDriver', 'exr')

def registerFilterTemplates():
    # register driver templates
    for transName, arnoldNode in core.listTranslators("aiAOVFilter"):
        templates.registerAutoTranslatorUI(arnoldNode, "aiAOVFilter", transName)

    templates.registerDefaultTranslator('aiAOVFilter', 'gaussian')

registerDriverTemplates()
registerFilterTemplates()

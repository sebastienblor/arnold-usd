import maya.mel
from mtoa.ui.ae.shaderTemplate import ShaderAETemplate

class AEaiSkyTemplate(ShaderAETemplate):
    def setup(self):
        self.addSwatch()
        self.beginScrollLayout()
        
        self.beginLayout('Sky Attributes', collapse=False)
        self.addControl('format', label='Format')
        self.addControl('color', label='Color')
        self.addControl('intensity', label='Intensity')
        self.endLayout()
        
        self.beginLayout('Render Stats', collapse=True)
        self.beginNoOptimize()
        self.addControl('castsShadows', label='Casts Shadows')
        self.addControl('primaryVisibility', label='Primary Visibility')
        self.addControl('aiVisibleInDiffuseReflection', label='Visible in Diffuse Reflection')
        self.addControl('aiVisibleInSpecularReflection', label='Visible in Specular Reflection')
        self.addControl('aiVisibleInDiffuseTransmission', label='Visible in Diffuse Transmission')
        self.addControl('aiVisibleInSpecularTransmission', label='Visible in Specular Transmission')
        self.addControl('aiVisibleInVolume', label='Visible in Volume')

        self.endNoOptimize()
        self.endLayout()
        
        self.beginLayout('Hardware Texturing', collapse=True)
        self.addControl('sampling', label='Texture Resolution')
        self.addControl('hwtexalpha', label='Opacity')
        self.endLayout()
        
        self.beginLayout('Viewport', collapse=True)
        self.addControl('skyRadius', label='Sky Radius')
        self.addControl('skyFacing', label='Facing')
        self.endLayout()

        # Do not show extra attributes
        extras = ["visibility",
                  "intermediateObject",
                  "template",
                  "ghosting",
                  "instObjGroups",
                  "useObjectColor",
                  "objectColor",
                  "drawOverride",
                  "lodVisibility",
                  "renderInfo",
                  "renderLayerInfo",
                  "ghostingControl",
                  "ghostCustomSteps",
                  "ghostFrames",
                  "ghostRangeStart",
                  "ghostRangeEnd",
                  "ghostDriver",
                  "ghostColorPreA",
                  "ghostColorPre",
                  "ghostColorPostA",
                  "ghostColorPost",
                  "motionBlur",
                  "visibleInReflections",
                  "visibleInRefractions",
                  "castsShadows",
                  "receiveShadows",
                  "maxVisibilitySamplesOverride",
                  "maxVisibilitySamples",
                  "geometryAntialiasingOverride",
                  "antialiasingLevel",
                  "shadingSamplesOverride",
                  "shadingSamples",
                  "maxShadingSamples",
                  "volumeSamplesOverride",
                  "volumeSamples",
                  "depthJitter",
                  "ignoreSelfShadowing",
                  "primaryVisibility",
                  "compInstObjGroups",
                  "localPosition",
                  "localScale"]

        for extra in extras:
            self.suppress(extra)
            
        maya.mel.eval('AEdependNodeTemplate '+self.nodeName)

        self.addExtraControls()
        self.endScrollLayout()


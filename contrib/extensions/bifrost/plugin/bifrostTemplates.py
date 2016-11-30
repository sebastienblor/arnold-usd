import mtoa.ui.ae.templates as templates
from mtoa.ui.ae.templates import AttributeTemplate

#class BifrostShapeTemplate(AttributeTemplate):
#    def setup(self):
#        self.suppress("aiDebug");
#        self.suppress("aiUserOptions");
#templates.registerAETemplate(BifrostShapeTemplate, 'bifrostShape')

class BifrostAeroMaterialTemplate(AttributeTemplate):
    def setup(self):
        self.addControl("aiStepSize", label="Step Size")
        self.addControl("aiMaxSteps", label="Max Steps")
        self.addControl("aiShadowing", label="Shadowing")
        self.addControl("aiShadowingStepSize", label="Shadowing Step Size")
        self.addControl("aiShadowingMaxSteps", label="Shadowing Max Steps")

templates.registerAETemplate(BifrostAeroMaterialTemplate, 'bifrostAeroMaterial')

class BifrostHydroTemplate(AttributeTemplate):
    def setup(self):
        self.beginLayout("Matte", collapse=False)
        self.addControl("aiEnableMatte", label = "Enable Matte")
        self.addControl("aiMatteColor", label = "Matte Color")
        self.addControl("aiMatteColorA", label = "Matte Opacity")
        self.endLayout()

        self.beginLayout("AOV", collapse=False)
        self.addControl("aiAovDiffuseColor", label = "Diffuse Color")
        self.addControl("aiAovDirectDiffuse", label = "Direct Diffuse")
        self.addControl("aiAovDirectDiffuseRaw", label = "Direct Diffuse Raw")
        self.addControl("aiAovIndirectDiffuse", label = "Indirect Diffuse")
        self.addControl("aiAovIndirectDiffuseRaw", label = "Indirect Diffuse Raw")
        self.addControl("aiAovDirectSpecular", label = "Direct Specular")
        self.addControl("aiAovIndirectSpecular", label = "Indirect Specular")
        self.addControl("aiAovSingleScatter", label = "Single Scatter")
        self.addControl("aiAovRefraction", label = "Refraction")
        self.addControl("aiAovEmission", label = "Emission")
        self.addControl("aiAovUV", label = "UV")
        self.addControl("aiAovDepth", label = "Depth")
        self.addControl("aiStandardCompatibleAOVs", label = "Standard AOVs")
        self.addControl("aiTransmitAovs", label = "Transmit AOVs")
        self.endLayout()

        self.beginLayout("Sampling", collapse=True)
        self.addControl("aiDiffuseExtraSamples", label = "Extra Diffuse Samples")
        self.addControl("aiSpecularExtraSamples", label = "Extra Specular Samples")
        self.addControl("aiTransmissionExtraSamples", label = "Extra Transmission Samples")
        self.addControl("aiSpecularDistribution", label = "Specular Distribution")
        self.addControl("aiRussianRoulette", label = "Use Russian Roulette")
        self.addControl("aiRrTransmissionDepth", label = "Transmission RR Depth")
        self.endLayout()

        self.beginLayout("Trace Optimizations", collapse=True)
        self.addControl("aiRr", label = "Russian Roulette")
        self.addControl("aiTraceSetAll", label = "Global Trace Set")
        self.addControl("aiTraceSetDiffuse", label = "Diffuse Trace Set")
        self.addControl("aiTraceSetSpecular", label = "Specular Trace Set")
        self.addControl("aiTraceSetTransmission", label = "Transmission Trace Set")
        self.addControl("aiTraceSetShadows", label = "Shadow Trace Set")
        self.endLayout()

        self.addControl("aiUserOptions", label = "User Options")
        self.suppress("aiDebug");

templates.registerAETemplate(BifrostHydroTemplate, 'bifrostHydro')

class BifrostPointTemplate(AttributeTemplate):
    def setup(self):
        self.beginLayout("Matte", collapse=False)
        self.addControl("aiEnableMatte", label = "Enable Matte")
        self.addControl("aiMatteColor", label = "Matte Color")
        self.addControl("aiMatteColorA", label = "Matte Opacity")
        self.endLayout()

        self.beginLayout("AOV", collapse=False)
        self.addControl("aiAovDensityNormal", label = "Density Normal")
        self.addControl("aiAovDiffuseColor", label = "Diffuse Color")
        self.addControl("aiAovDirectDiffuse", label = "Direct Diffuse")
        self.addControl("aiAovDirectDiffuseRaw", label = "Direct Diffuse Raw")
        self.addControl("aiAovIndirectDiffuse", label = "Indirect Diffuse")
        self.addControl("aiAovIndirectDiffuseRaw", label = "Indirect Diffuse  Raw")
        self.addControl("aiAovDirectBacklight", label = "Direct Backlight")
        self.addControl("aiAovDirectBacklightRaw", label = "Direct Backlight Raw")
        self.addControl("aiAovIndirectBacklight", label = "Indirect Backlight")
        self.addControl("aiAovIndirectBacklightRaw", label = "Indirect Backlight  Raw")
        self.addControl("aiAovEmission", label = "Emission")
        self.endLayout()

        self.beginLayout("Trace Optimizations", collapse=True)
        self.addControl("aiTraceSetAll", label = "Global Trace Set")
        self.addControl("aiTraceSetDiffuse", label = "Diffuse Trace Set")
        self.addControl("aiTraceSetBacklight", label = "Backlight Trace Set")
        self.addControl("aiTraceSetShadows", label = "Shadow Trace Set")
        self.endLayout()

        self.addControl("aiUserOptions", label = "User Options")

        self.suppress("aiDebug");
        self.suppress("aiAovLightGroup1");
        self.suppress("aiAovLightGroup2");
        self.suppress("aiAovLightGroup3");
        self.suppress("aiAovLightGroup4");
        self.suppress("aiAovLightGroup5");
        self.suppress("aiAovLightGroup6");
        self.suppress("aiAovLightGroup7");
        self.suppress("aiAovLightGroup8");

templates.registerAETemplate(BifrostPointTemplate, 'bifrostPoint')

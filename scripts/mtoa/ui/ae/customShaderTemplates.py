import mtoa.aovs as aovs
import mtoa.ui.ae.templates as templates
import mtoa.ui.ae.shaderTemplate as shaderTemplate
import mtoa.ui.aoveditor as aoveditor
from collections import defaultdict

import maya.mel as mel

class DisplacementShaderTemplate(templates.AttributeTemplate):
    def setup(self):
        self.addControl('aiDisplacementPadding', label='Bounds Padding')
        self.addControl('aiDisplacementZeroValue', label='Scalar Zero Value')
        self.addControl('aiDisplacementAutoBump', label='Auto Bump')

templates.registerAETemplate(DisplacementShaderTemplate, 'displacementShader')

class FileTemplate(templates.AttributeTemplate):
    def setup(self):
        self.addControl('aiFilter', label='Filter Type')
        self.addControl('aiMipBias', label='Mip-map Bias')
        self.addControl('aiUseDefaultColor', label='Use Default Color')
        self.addControl('aiAutoTx', label='Auto-generate TX Textures')
        
templates.registerAETemplate(FileTemplate, 'file')

class Bump2dTemplate(templates.AttributeTemplate):
    def setup(self):
        self.addControl('aiFlipR', label='Flip R Channel')
        self.addControl('aiFlipG', label='Flip G Channel')
        self.addControl('aiSwapTangents', label='Swap Tangents')
        self.addControl('aiUseDerivatives', label='Use Derivatives')

templates.registerAETemplate(Bump2dTemplate, 'bump2d')

class ProjectionTemplate(templates.AttributeTemplate):
    def setup(self):
        self.addControl('aiUseReferenceObject', label='Use Texture Reference Object')
        self.addSeparator()
        self.addControl("aiUserOptions", label="User Options")

templates.registerAETemplate(ProjectionTemplate, 'projection')

class ImagePlaneTemplate(templates.AttributeTemplate):
    def setup(self):
        self.addControl('aiAutoTx', label='Auto-generate TX Textures')
        self.addControl('aiOffscreenColor', label='Offscreen Color')


templates.registerAETemplate(ImagePlaneTemplate, 'imagePlane')

class StandardSurfaceTemplate(templates.AttributeTemplate):
   
    def setup(self):

        self.beginLayout("Matte", collapse=True)
        self.addControl("aiEnableMatte", label="Enable Matte")
        self.addControl("aiMatteColor", label="Matte Color")
        self.addControl("aiMatteColorA", label="Matte Opacity")
        self.endLayout()
        self.beginLayout("AOVs", collapse=True)
        self.addControl("aov_id1", label="ID 1 AOV")
        self.addControl("id1", label="ID 1")
        self.addSeparator()
        self.addControl("aov_id2", label="ID 2 AOV")
        self.addControl("id2", label="ID 2")
        self.addSeparator()
        self.addControl("aov_id3", label="ID 3 AOV")
        self.addControl("id3", label="ID 3")
        self.addSeparator()
        self.addControl("aov_id4", label="ID 4 AOV")
        self.addControl("id4", label="ID 4")
        self.addSeparator()
        self.addControl("aov_id5", label="ID 5 AOV")
        self.addControl("id5", label="ID 5")
        self.addSeparator()
        self.addControl("aov_id6", label="ID 6 AOV")
        self.addControl("id6", label="ID 6")
        self.addSeparator()
        self.addControl("aov_id7", label="ID 7 AOV")
        self.addControl("id7", label="ID 7")
        self.addSeparator()
        self.addControl("aov_id8", label="ID 8 AOV")
        self.addControl("id8", label="ID 8")
        self.endLayout()

        self.beginLayout("Advanced", collapse=True)
        self.addControl("caustics", label="Caustics", annotation="Enable Caustics")
        self.addControl("internalReflections", label="Internal Reflections", annotation="Enable Internal Reflections")
        self.addControl("exitToBackground", label="Exit To Background *", annotation="When bounce depth is exceeded, use background color instead")
        self.addSeparator()
        self.addControl("indirectDiffuse", label="Indirect Diffuse", annotation="Indirect Diffuse")
        self.addControl("indirectSpecular", label="Indirect Specular", annotation="Indirect Specular")
        self.endLayout()

templates.registerAETemplate(StandardSurfaceTemplate, "standardSurface")


def appendToSSTemplate():
    mel.eval("source AEstandardSurfaceTemplate;")
    mel.eval("$notUsed = $gAEstandardSurfaceTemplateCallbacks")
    mel.eval("""
    source AEstandardSurfaceTemplate;
    $notUsed = $gAEstandardSurfaceTemplateCallbacks;
    global proc myCallback(int $layoutID) {
        global int $gAEstandardSurfaceLayout_Subsurface;
        if ($layoutID == $gAEstandardSurfaceLayout_Subsurface) {
            editorTemplate -addSeparator;
            editorTemplate -addControl "subsurface_type";
            }
    }
    stringArrayInsertAtIndex(0, $gAEstandardSurfaceTemplateCallbacks, "myCallback");
    evalDeferred("refreshAE; refreshEditorTemplates;");
    """)


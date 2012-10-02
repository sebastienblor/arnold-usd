import pymel.core as pm
#from mtoa.ui.ae.aiSwatchDisplay import aiSwatchDisplay
from mtoa.ui.ae.shaderTemplate import ShaderAETemplate

class AEaiUtilityTemplate(ShaderAETemplate):

    def setup(self):
        #aiSwatchDisplay(nodeName)
        self.addSwatch()
        self.beginScrollLayout()
        
        self.addCustom('message', 'AEshaderTypeNew', 'AEshaderTypeReplace')

        #cmds.editorTemplate('AEshaderTypeNew', 'AEshaderTypeReplace', "message", callCustom=True)

        self.beginLayout('Utility Attributes', collapse=False)
        self.addControl('shade_mode', label='Shade Mode')
        self.addControl('color_mode', label='Color Mode', enumeratedItem=[(0, 'Color'), (1, 'Geometric Normal'), (2, 'Un-bumped Normal'), (3, 'Normal'),
                                                                          (4, 'Barycentric Coords'), (5, 'UV Coords'), (6, 'U Coords'), (7, 'V Coords'),
                                                                          (8, 'U Surface Derivative (dPdu)'), (9, 'V Surface Derivative (dPdv)'),
                                                                          (10, 'Shading Point (Relative to BBox)'), (11, 'Primitive ID'),
                                                                          (12, 'Triangle Wireframe'), (13, 'Polygon Wireframe'), (15, 'Object'),
                                                                          (16, 'Subdivision Edge Length'), (17, 'Floatgrid'), (18, 'Reflection Lines'),
                                                                          (19, 'Bad UVs'), (20, 'Number of Lights'), (21, 'Object ID'),
                                                                          (22, 'Bump Difference')])
        self.addControl('color', label='Color')
        self.addControl('opacity', label='Opacity')
        self.endLayout()

        # include/call base class/node attributes
        pm.mel.AEdependNodeTemplate(self.nodeName)
        self.addExtraControls()
        
        self.endScrollLayout()        


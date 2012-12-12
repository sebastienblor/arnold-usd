import pymel.core as pm
import maya.cmds as cmds
from mtoa.ui.ae.shaderTemplate import ShaderAETemplate

def aiUtilityCreateColorMode(attr):
    cmds.setUITemplate('attributeEditorPresetsTemplate', pushTemplate=True)
    cmds.attrEnumOptionMenuGrp('AIUtilityColorMode', attribute=attr, label="Color Mode", 
                               enumeratedItem=[(0, 'Color'), (3, 'Normal'), (1, 'Geometric Normal'), (2, 'Un-bumped Normal'), (21, 'Bump Difference'),
                                               (4, 'Barycentric Coords'), (5, 'UV Coords'), (6, 'U Coords'), (7, 'V Coords'),
                                               (8, 'U Surface Derivative (dPdu)'), (9, 'V Surface Derivative (dPdv)'),
                                               (10, 'Shading Point (Relative to BBox)'), (11, 'Primitive ID'),
                                               (12, 'Triangle Wireframe'), (13, 'Polygon Wireframe'), (14, 'Object'),
                                               (15, 'Subdivision Edge Length'), (16, 'Floatgrid'), (17, 'Reflection Lines'),
                                               (18, 'Bad UVs'), (29, 'Number of Lights'), (20, 'Object ID')])
    cmds.setUITemplate(popTemplate=True)

def aiUtilitySetColorMode(attr):
    cmds.attrEnumOptionMenuGrp('AIUtilityColorMode', edit=True, attribute=attr)

class AEaiUtilityTemplate(ShaderAETemplate):

    def setup(self):
        self.addSwatch()
        self.beginScrollLayout()
        
        self.addCustom('message', 'AEshaderTypeNew', 'AEshaderTypeReplace')

        self.beginLayout('Utility Attributes', collapse=False)
        self.addControl('shade_mode', label='Shade Mode')
        self.addCustom('color_mode', aiUtilityCreateColorMode, aiUtilitySetColorMode)
        self.addControl('color', label='Color')
        self.addControl('opacity', label='Opacity')
        self.endLayout()

        # include/call base class/node attributes
        pm.mel.AEdependNodeTemplate(self.nodeName)
        self.addExtraControls()
        
        self.endScrollLayout()        


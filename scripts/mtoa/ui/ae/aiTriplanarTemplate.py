import maya.mel
import maya.cmds as cmds
from mtoa.ui.ae.shaderTemplate import ShaderAETemplate

class AEaiTriplanarTemplate(ShaderAETemplate):
    def updateCell(self, nodeName):
        cellAttr = '%s.%s' % (nodeName, 'cell')
        cellValue = cmds.getAttr(cellAttr)
        
        cmds.editorTemplate(dimControl=(nodeName, 'cellRotate', not cellValue))
        cmds.editorTemplate(dimControl=(nodeName, 'cellBlend', not cellValue))
            

    def setup(self):
        self.addSwatch()
        self.beginScrollLayout()
        
        self.beginLayout("Input", collapse=False)
        self.addControl("input", label="Input")
        self.addControl('inputPerAxis', label = "Input Per Axis", changeCommand=lambda *args: self.updateParamsVisibility(self.nodeName), annotation = " This enables a texture input per each axis")
        self.addControl("inputY", label="InputY")
        self.addControl("inputZ", label="InputZ")
        
        self.endLayout()

        self.beginLayout("Transform", collapse=False)
        self.addControl("scale", label="Scale")
        self.addControl("rotate", label="Rotate")
        self.addControl("offset", label="Offset")
        self.addControl("coordSpace")
        self.addControl("prefName")
        self.addControl("blend")
        self.addControl("flipOnOppositeDirection", annotation = "Flip the uv's on the opposite direction to match with Maya's triplanar projection")

        self.endLayout()

        self.beginLayout("Cell", collapse=False)
        self.addControl("cell", changeCommand=self.updateCell, label="Enable Cell")
        self.addControl("cellRotate", label="Rotate")
        self.addControl("cellBlend", label="Blend")
        self.endLayout()
                
        maya.mel.eval('AEdependNodeTemplate '+self.nodeName)

        self.addExtraControls()
        self.endScrollLayout()
        self.updateParamsVisibility(self.nodeName)
    

    def updateParamsVisibility(self, nodeName):
        modeAttr = '%s.%s' % (nodeName, 'inputPerAxis')
        modeValue = cmds.getAttr(modeAttr)
        self.dimControl('inputY', state=modeValue != 1)
        self.dimControl('inputZ', state=modeValue != 1)

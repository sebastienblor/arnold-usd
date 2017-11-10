import pymel.core as pm
from mtoa.ui.ae.shaderTemplate import ShaderAETemplate

class AEaiTriplanarTemplate(ShaderAETemplate):
    def updateCell(self, nodeName):
        cellAttr = '%s.%s' % (nodeName, 'cell')
        cellValue = pm.getAttr(cellAttr)
        
        pm.editorTemplate(dimControl=(nodeName, 'cellRotate', not cellValue))
        pm.editorTemplate(dimControl=(nodeName, 'cellBlend', not cellValue))
            

    def setup(self):
        self.addSwatch()
        self.beginScrollLayout()
        
        self.addControl("input", label="Input")
        self.addSeparator()
        self.addControl("coordSpace")
        self.addControl("P")
        self.addControl("blend")

        self.beginLayout("Transform", collapse=False)
        self.addControl("scale", label="Scale")
        self.addControl("rotate", label="Rotate")
        self.addControl("offset", label="Offset")
        self.endLayout()

        self.beginLayout("Cell", collapse=False)
        self.addControl("cell", changeCommand=self.updateCell, label="Enable Cell")
        self.addControl("cellRotate", label="Rotate")
        self.addControl("cellBlend", label="Blend")
        self.endLayout()
                
        pm.mel.AEdependNodeTemplate(self.nodeName)

        self.addExtraControls()
        self.endScrollLayout()
    

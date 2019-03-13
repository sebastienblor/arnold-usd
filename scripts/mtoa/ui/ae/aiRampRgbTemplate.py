import maya.mel
from mtoa.ui.ae.shaderTemplate import ShaderAETemplate
import maya.cmds as cmds
class AEaiRampRgbTemplate(ShaderAETemplate):

    def rampUpdate(self, attr):
        node, plug = attr.split('.', 1)
        attrSize = cmds.getAttr(self.nodeName + '.ramp', size=True)
        if attrSize == 0:
            cmds.setAttr('{}.ramp[0].ramp_Color'.format(self.nodeName), 0, 0, 0, type='double3')
            cmds.setAttr('{}.ramp[0].ramp_Position'.format(self.nodeName), 0)
            cmds.setAttr('{}.ramp[0].ramp_Interp'.format(self.nodeName), 1)
            cmds.setAttr('{}.ramp[1].ramp_Color'.format(self.nodeName), 1, 1, 1, type='double3')
            cmds.setAttr('{}.ramp[1].ramp_Position'.format(self.nodeName), 1)
            cmds.setAttr('{}.ramp[1].ramp_Interp'.format(self.nodeName), 1)
    
    def setup(self):
        self.addSwatch()
        self.beginScrollLayout()

        self.addControl('type', label='Type')
        self.addControl('input', label='Input')
        
        self.addCustom('ramp', self.rampUpdate, self.rampUpdate)
        maya.mel.eval('AEaddRampControl '+self.nodeName + '.ramp')
        
        self.addControl('uvset', label='Uv Set')
        self.addControl('use_implicit_uvs', label='Use Implicit UVs')
        
        maya.mel.eval('AEdependNodeTemplate '+self.nodeName)

        self.addExtraControls()
        self.endScrollLayout()



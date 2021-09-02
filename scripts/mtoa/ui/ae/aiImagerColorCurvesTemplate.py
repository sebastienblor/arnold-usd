import maya.cmds as cmds
import maya.mel as mel
from maya import stringTable
from mtoa.ui.ae.templates import TranslatorControl
from mtoa.ui.ae.shaderTemplate import ShaderAETemplate
from mtoa.ui.ae.aiImagersBaseTemplate import ImagerBaseUI, registerImagerTemplate
import maya.internal.common.ae.ramp as aeramp

TOP =  "top"
BOTTOM = "bottom"
RIGHT = "right"
LEFT = "left"

class AEaiImagerColorCurvesTemplate(ShaderAETemplate):

    def setup(self):

        self.beginScrollLayout()
        currentWidget = cmds.setParent(query=True)
        self.ui = ImagerColorCurvesAEUI(parent=currentWidget, nodeName=self.nodeName, template=self)
        mel.eval('AEdependNodeTemplate '+self.nodeName)
        self.addExtraControls()
        self.endScrollLayout()


class ImagerColorCurvesAEUI(ImagerBaseUI):
    def __init__(self, parent=None, nodeName=None, template=None):
        super(ImagerColorCurvesAEUI, self).__init__(parent, nodeName, template)


    def setup(self):
        super(ImagerColorCurvesAEUI, self).setup()
        self.addSeparator()
        self.beginLayout("Color Curves", collapse=False)
        self.addCustom('rampRGB', rampUpdateRGB, rampUpdateRGB)
        mel.eval('AEaddRampControl '+self.nodeName + '.rampRGB')
        self.addCustom('rampR', rampUpdateRGB, rampUpdateRGB)
        mel.eval('AEaddRampControl '+self.nodeName + '.rampR')
        self.addCustom('rampG', rampUpdateRGB, rampUpdateRGB)
        mel.eval('AEaddRampControl '+self.nodeName + '.rampG')
        self.addCustom('rampB', rampUpdateRGB, rampUpdateRGB)
        mel.eval('AEaddRampControl '+self.nodeName + '.rampB')
        self.endLayout()
        self.beginLayout("Color Space", collapse=True)
        self.addCustom('workingColorSpace', self.colorSpaceNew, self.colorSpaceReplace)
        self.endLayout()

    def colorSpaceEdit(self, attrName) :
        lookValue = cmds.optionMenu(self.colorSpaces, q = True , value = True)
        cmds.setAttr(attrName, lookValue, type='string')

    def colorSpaceNew(self, attrName) :
        cmds.rowColumnLayout( numberOfColumns=1, columnWidth=[(1,220)], columnAlign=[(1, 'left')], columnAttach=[(1, 'left', 0)]) 
        self.colorSpaces = cmds.optionMenu( label='Working Color Space', changeCommand = lambda *args: self.colorSpaceEdit(attrName))
        cmds.setParent('..')
        self.colorSpaceReplace(attrName)

    def colorSpaceReplace(self, attrName) :
        cmds.optionMenu(self.colorSpaces , edit=True, changeCommand = lambda *args: self.colorSpaceEdit(attrName))
        for m in cmds.optionMenu(self.colorSpaces, q=True, itemListLong=True) or []:
            cmds.deleteUI(m)
        color_space_value = cmds.getAttr(attrName)
        color_spaces = cmds.colorManagementPrefs(q=True, inputSpaceNames=True)
        index = 1
        count = 1
        for color in color_spaces:
            if (not color_space_value):
                # TODO need to get current working colorspace
                if (str(color) == str("ACES2065-1")):
                    index = count
            elif (str(color) == str(color_space_value)):
                index = count
            cmds.menuItem(parent=self.colorSpaces, label=str(color))
            count +=1
        cmds.optionMenu(self.colorSpaces, edit=True, sl = index)


class ArnoldGradientControl(object):
    """
    interp_choices = ["constant", "linear", "catrom", "cubic"]
    """


    def __init__(self, attr, parent):
        self.attribute = attr
        self.nodeName, self.plug = self.attribute.split('.')
        self.rampCtrl = None
        self.posCtrl = None
        self.valCtrl = None
        self.interp = None
        rampUpdateRGB(self.attribute)
        self.setup()
    
    def setup(self):
        rampForm = cmds.formLayout()
        self.posCtrl =  cmds.attrFieldSliderGrp("posCtrl#",label=stringTable[u'y_maya_internal_common_ae_ramp.kPos' ], cw=(1,123), parent=rampForm, cc=self.updateGradient)
        self.valCtrl =  cmds.attrFieldSliderGrp("valCtrl#",label=stringTable[u'y_maya_internal_common_ae_ramp.kVal' ], cw=(1,123), parent=rampForm, cc=self.updateGradient)
        self.interp = cmds.attrEnumOptionMenuGrp("interp#",label=stringTable[ u'y_maya_internal_common_ae_ramp.kInterp' ],cw=(1,123), parent=rampForm)
        self.posCtrl = self.posCtrl.split('|')[-1]
        self.valCtrl = self.valCtrl.split('|')[-1]
        self.interp = self.interp.split('|')[-1]
        self.editButton = cmds.button(label=stringTable[u'y_maya_internal_common_ae_ramp.kLabelEditRamp' ], width=23, parent=rampForm, command="import maya.mel;maya.mel.eval('editRampAttribute " + self.attribute + "')")
        self.rampframe = cmds.frameLayout(lv=0, cll=0, parent=rampForm)

        self.rampCtrl = cmds.gradientControlNoAttr(w=135, h=74)

        cmds.formLayout(rampForm, edit=True,
                    attachForm=[
                    (self.posCtrl, LEFT,  -10),
                    (self.posCtrl, TOP, 2),

                    (self.valCtrl, LEFT, -10),

                    (self.rampframe, TOP, 3),

                    (self.interp, LEFT, -10 ),

                    (self.editButton, TOP, 0),
                    (self.editButton, BOTTOM, 0)],

                    attachControl=[
                    (self.valCtrl, TOP, -3, self.posCtrl),
                    (self.interp, TOP, -3, self.valCtrl),
                    (self.rampframe, LEFT, 5, self.posCtrl),
                    (self.editButton, LEFT, 5, self.rampframe)],

                    attachNone=[
                    (self.posCtrl, RIGHT),
                    (self.posCtrl, BOTTOM),

                    (self.valCtrl, RIGHT),
                    (self.valCtrl, BOTTOM),

                    (self.rampframe, RIGHT),
                    (self.rampframe, BOTTOM),

                    (self.interp, RIGHT),
                    (self.interp, BOTTOM),

                    (self.editButton, RIGHT)]
                )


        # get the current value of this attribute        
        val_s = self.getValueString()

        cmds.gradientControlNoAttr(self.rampCtrl,
                                   edit=True,
                                   asString=val_s,
                                   cc=lambda *args:self.gradientChanged(*args),
        )

        cmds.gradientControlNoAttr(self.rampCtrl, edit=True, ckc=lambda *args:self.keySelected(*args))

        self.keySelected()

        cmds.scriptJob(attributeChange=[self.attribute,
                                          self.updateGradient],
                                          parent=self.rampCtrl)
    def getValueString(self):
        values = []
        for i in range(cmds.getAttr(self.attribute, s=True)):
            k_vals = cmds.getAttr('{}[{}]'.format(self.attribute, i))[0]
            # need to reorder the values for feeding the input of the gradient
            # as it takes in val, pos, interp
            values += [k_vals[1], k_vals[0], k_vals[2]] 
        
        val_s = ",".join([str(v) for v in values])
        return val_s

    def getValues(self):

        vals =  cmds.gradientControlNoAttr(self.rampCtrl, q=True, asString=True)
        it = iter([float(v) for v in vals.split(',')])
        v_list = list(zip(it,it,it))
        
        return v_list

    def keySelected(self, *args):
        # get the current key
        key = cmds.gradientControlNoAttr(self.rampCtrl, q=True, ck=True)
        # v_list = self.getValues()
        attr_prefix = "{}[{}].{}".format(self.attribute, key, self.plug)
        cmds.attrFieldSliderGrp(self.valCtrl, e=True, attribute="{}_FloatValue".format(attr_prefix))
        cmds.attrFieldSliderGrp(self.posCtrl, e=True, attribute="{}_Position".format(attr_prefix))
        cmds.attrEnumOptionMenuGrp(self.interp, e=True, attribute="{}_Interp".format(attr_prefix))

    def gradientChanged(self, *args):
        # get the values
        v_list = self.getValues()
        # set the attribute
        for i,v in enumerate(v_list):
            attr_prefix = "{}[{}].{}".format(self.attribute, i, self.plug)
            cmds.setAttr('{}_FloatValue'.format(attr_prefix), v[0])
            cmds.setAttr('{}_Position'.format(attr_prefix), v[1])
            cmds.setAttr('{}_Interp'.format(attr_prefix), v[2])

    def updateGradient(self):
        # get the current value of this attribute        
        val_s = self.getValueString()

        cmds.gradientControlNoAttr(self.rampCtrl,
                                   edit=True,
                                   asString=val_s)


class ImagerColorCurvesARVUI(ImagerBaseUI):
    def __init__(self, parent=None, nodeName=None, template=None):
        super(ImagerColorCurvesARVUI, self).__init__(parent, nodeName, template)

    def setup(self):
        super(ImagerColorCurvesARVUI, self).setup()
        self.addSeparator()
        self.beginLayout("Color Curves", collapse=False)
        self.buildUI('rampRGB')
        self.buildUI('rampR')
        self.buildUI('rampG')
        self.buildUI('rampB')
        self.endLayout()
    
    def buildUI(self, attributeName):

        attrFullName = self.nodeName + '.' + attributeName

        # Prepare
        parent = cmds.setParent(q=True)
        cmds.setUITemplate('attributeEditorTemplate', pushTemplate=True)
        niceName = cmds.attributeName(self.nodeName + '.' + attributeName, nice=True)
        frm = cmds.frameLayout( label=niceName, collapsable=True, collapse=False, parent=parent )
        # Widgets
        colLayout = cmds.columnLayout(rowSpacing=2)
        ctrl = ArnoldGradientControl(attrFullName, colLayout)
        cmds.setUITemplate(popTemplate=True)
        cmds.setParent(parent)


def rampUpdateRGB(attr):
    node, plug = attr.split('.', 1)
    attrSize = cmds.getAttr(node + '.'+plug, size=True)
    if attrSize == 0:
        cmds.setAttr('{}.{}[0].{}_FloatValue'.format(node,plug,plug), 0)
        cmds.setAttr('{}.{}[0].{}_Position'.format(node,plug,plug), 0)
        cmds.setAttr('{}.{}[0].{}_Interp'.format(node,plug,plug), 1)
        cmds.setAttr('{}.{}[1].{}_FloatValue'.format(node,plug,plug), 1)
        cmds.setAttr('{}.{}[1].{}_Position'.format(node,plug,plug), 1)
        cmds.setAttr('{}.{}[1].{}_Interp'.format(node,plug,plug), 1)

registerImagerTemplate("aiImagerColorCurves", ImagerColorCurvesARVUI)

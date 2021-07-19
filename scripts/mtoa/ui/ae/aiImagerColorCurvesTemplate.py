import maya.cmds as cmds
import maya.mel
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
        maya.mel.eval('AEdependNodeTemplate '+self.nodeName)
        self.addExtraControls()
        self.endScrollLayout()


class ImagerColorCurvesAEUI(ImagerBaseUI):
    def __init__(self, parent=None, nodeName=None, template=None):
        super(ImagerColorCurvesAEUI, self).__init__(parent, nodeName, template)

    def rampUpdateRGB(self, attr):
        node, plug = attr.split('.', 1)
        attrSize = cmds.getAttr(self.nodeName + '.'+plug, size=True)
        if attrSize == 0:
            cmds.setAttr('{}.{}[0].{}_FloatValue'.format(self.nodeName,plug,plug), 0)
            cmds.setAttr('{}.{}[0].{}_Position'.format(self.nodeName,plug,plug), 0)
            cmds.setAttr('{}.{}[0].{}_Interp'.format(self.nodeName,plug,plug), 1)
            cmds.setAttr('{}.{}[1].{}_FloatValue'.format(self.nodeName,plug,plug), 1)
            cmds.setAttr('{}.{}[1].{}_Position'.format(self.nodeName,plug,plug), 1)
            cmds.setAttr('{}.{}[1].{}_Interp'.format(self.nodeName,plug,plug), 1)

    def setup(self):
        super(ImagerColorCurvesAEUI, self).setup()
        self.addSeparator()
        self.beginLayout("Color Curves", collapse=True)
        self.addCustom('rampRGB', self.rampUpdateRGB, self.rampUpdateRGB)
        maya.mel.eval('AEaddRampControl '+self.nodeName + '.rampRGB')
        self.addCustom('rampR', self.rampUpdateRGB, self.rampUpdateRGB)
        maya.mel.eval('AEaddRampControl '+self.nodeName + '.rampR')
        self.addCustom('rampG', self.rampUpdateRGB, self.rampUpdateRGB)
        maya.mel.eval('AEaddRampControl '+self.nodeName + '.rampG')
        self.addCustom('rampB', self.rampUpdateRGB, self.rampUpdateRGB)
        maya.mel.eval('AEaddRampControl '+self.nodeName + '.rampB')
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
                if (str(color) == str("ACES2065-1")):
                    index = count
            elif (str(color) == str(color_space_value)):
                index = count
            cmds.menuItem(parent=self.colorSpaces, label=str(color))
            count +=1
        cmds.optionMenu(self.colorSpaces, edit=True, sl = index)

class ImagerColorCurvesARVUI(ImagerBaseUI):
    def __init__(self, parent=None, nodeName=None, template=None):
        super(ImagerColorCurvesARVUI, self).__init__(parent, nodeName, template)

    def setup(self):
        super(ImagerColorCurvesARVUI, self).setup()
        self.addSeparator()
        self.beginLayout("Color Curves", collapse=True)
        self.buildUI()
        self.endLayout()

    def buildUI(self):
        # Prepare
        print ("***********")
        parent = maya.cmds.setParent(q=True)
        maya.cmds.setUITemplate('attributeEditorTemplate', pushTemplate=True)
        niceName = maya.cmds.attributeName(self.nodeName + '.rampRGB', nice=True)
        frm = maya.cmds.frameLayout( label=niceName, collapsable=True, collapse=False, parent=parent )
        # Widgets
        maya.cmds.columnLayout(rowSpacing=2)
        rampForm = maya.cmds.formLayout()
        print("Ramp Form", rampForm)
        spc =  maya.cmds.attrFieldSliderGrp("spc#",label=maya.stringTable[u'y_maya_internal_common_ae_ramp.kPos' ], cw=(1,123), parent = rampForm)
        print ("SPC", spc)
        scc =  maya.cmds.attrFieldSliderGrp("scc#",label=maya.stringTable[u'y_maya_internal_common_ae_ramp.kVal' ], cw=(1,123), parent = rampForm)
        print ("SCC", scc)
        interp = maya.cmds.attrEnumOptionMenuGrp("interp#",label=maya.stringTable[ u'y_maya_internal_common_ae_ramp.kInterp' ], cw=(1,123), parent = rampForm)
        print ("Interp", interp)
        editButton = maya.cmds.button(label=maya.stringTable[u'y_maya_internal_common_ae_ramp.kLabelEditRamp' ], width=23, parent = rampForm)
        print ("Edit Button", editButton)

        rframe = maya.cmds.frameLayout(lv=0, cll=0, parent=rampForm)
        # spc = spc.split("|")[-1]
        # print ("SPC", spc)
        # scc = scc.split("|")[-1]
        # print ("SCC", scc)
        # interp = interp.split("|")[-1]
        # print ("Interp", interp)

        # spc = cmds.control(spc, q = True, fpn = True)
        # print ("SPC Again", spc)
        # scc = cmds.control(scc, q = True, fpn = True)
        # print ("SCC Again", scc)
        # interp = cmds.control(interp, q = True, fpn = True)
        # print ("Interp Again", interp)

        self.rampGrad = maya.cmds.gradientControl(snc=0, sp=0, w=135, h=74, at=self.nodeName + '.rampRGB')
        currentWidget = toQtObject(imagerShadersFrame, QtWidgets.QWidget)
        maya.cmds.formLayout(rampForm, edit=True,
                    attachForm=[
                    (spc, LEFT,  0),
                    (spc, TOP, 0),
                    (scc, LEFT, 0 ),
                    (interp, LEFT, 0 ),
                    (rframe, TOP, 0),
                    (editButton, TOP, 0),
                    (editButton, BOTTOM, 0)] ,
                    attachControl=[
                    (scc, TOP, 0, spc),
                    (interp, TOP, 0, scc),
                    (rframe, LEFT, 2, interp),
                    (editButton, LEFT, 5, rframe)],
                    attachNone=[
                    (spc, RIGHT),
                    (spc, BOTTOM),
                    (scc, RIGHT),
                    (scc, BOTTOM),
                    (interp, RIGHT),
                    (interp, BOTTOM),
                    (rframe, RIGHT),
                    (rframe, BOTTOM),
                    (editButton, RIGHT)]
                )

        # maya.cmds.gradientControl(self.rampGrad, edit=True, scc=scc)
        # print(" Setting SCC of gradient Control ")
        # maya.cmds.gradientControl(self.rampGrad, edit=True, spc=spc)
        # print(" Setting SPC of gradient Control ")
        # maya.cmds.gradientControl(self.rampGrad, edit=True, sic=interp)
        # print(" Setting SIC of gradient Control ")
        # # Finish
        maya.cmds.setUITemplate(popTemplate=True)
        maya.cmds.setParent(parent)
        print ("***********")
        print ()


registerImagerTemplate("aiImagerColorCurves", ImagerColorCurvesARVUI)


## self.addCustom('rampRGB', self.rampUpdateRGB, self.rampUpdateRGB)
#print("Parent Before is ",cmds.setParent(query=True))
## cmds.setParent(cmds.setParent(query=True))
## cmds.columnLayout( adjustableColumn=True )
#print("Parent After is ",cmds.setParent(query=True))
## self.rampWidget = aeramp.RampWidget(attribute=self.nodeName + '.rampRGB')
## self.rampWidget.buildUI()
# cmds.setParent('..')


    
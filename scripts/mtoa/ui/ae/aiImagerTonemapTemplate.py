import maya.cmds as cmds
import maya.mel
from mtoa.ui.ae.templates import TranslatorControl
from mtoa.ui.ae.shaderTemplate import ShaderAETemplate
from mtoa.ui.ae.aiImagersBaseTemplate import ImagerBaseUI, registerImagerTemplate
import os


class AEaiImagerTonemapTemplate(ShaderAETemplate):

    def setup(self):

        self.beginScrollLayout()

        currentWidget = cmds.setParent(query=True)
        self.ui = ImagerTonemapUI(parent=currentWidget, nodeName=self.nodeName, template=self)

        maya.mel.eval('AEdependNodeTemplate '+self.nodeName)

        self.addExtraControls()
        self.endScrollLayout()


class ImagerTonemapUI(ImagerBaseUI):
    def __init__(self, parent=None, nodeName=None, template=None):
        super(ImagerTonemapUI, self).__init__(parent, nodeName, template)

    def setup(self):
        super(ImagerTonemapUI, self).setup()

        self.beginLayout("Main", collapse=False)
        self.addControl('mode', label='Mode', changeCommand=self.updateParamsVisibility, annotation='The mode used to perform tonemapping (filmic, reinhard,lut')
        self.addControl('preserveSaturation', label='Preserve Saturation', annotation='Preserves color saturation for extreme bright values.')
        self.addControl('gamma', label='Gamma', annotation='Gamma curve exponent for midtones value control.')
        self.endLayout()

        self.beginLayout("Filmic", collapse=False)
        self.addControl('filmicToeStrength', label='Toe Strength', annotation='Amount of curvature for the tonemap curve in the darker values.', hideMapButton = True)
        self.addControl('filmicToeLength', label='Toe Length', annotation='Amount of darker values affected by the toe.', hideMapButton = True)
        self.addControl('filmicShoulderStrength', label='Shoulder Strength', annotation='Amount of curvature for the tonemap curve in the brighter values.', hideMapButton = True)
        self.addControl('filmicShoulderLength', label='Shoulder Length', annotation='Amount in f-stops of brighter values affected by the shoulder.', hideMapButton = True)
        self.addControl('filmicShoulderAngle', label='Shoulder Angle', annotation='Curve slope when white is reached.', hideMapButton = True)
        self.endLayout()

        self.beginLayout("Reinhard", collapse=False)
        self.addControl('reinhardHighlights', label='Highlights', annotation='Reinhard photographic tonemap operator strength.', hideMapButton = True)
        self.addControl('reinhardShadows', label='Shadows', annotation='Additional tonemapping control for darker values.', hideMapButton = True)
        self.endLayout()

        self.beginLayout("LUT", collapse=False)
        self.addCustom('lutFilename', self.filenameNew, self.filenameReplace)
        self.addCustom('lutWorkingColorSpace', self.colorSpaceNew, self.colorSpaceReplace)
        self.endLayout()

        self.updateParamsVisibility(self.nodeName)

    def updateParamsVisibility(self, *args):
        modeAttr = '%s.%s' % (self.nodeName, 'mode')
        modeValue = cmds.getAttr(modeAttr)
        self.dimControl('filmicToeStrength', state=modeValue != 0)
        self.dimControl('filmicToeLength', state=modeValue != 0)
        self.dimControl('filmicShoulderStrength', state=modeValue != 0)
        self.dimControl('filmicShoulderLength', state=modeValue != 0)
        self.dimControl('filmicShoulderAngle', state=modeValue != 0)
        self.dimControl('reinhardHighlights', state=modeValue != 1)
        self.dimControl('reinhardShadows', state=modeValue != 1)
        self.dimControl('lutFilename', state=modeValue != 2)
        self.dimControl('lutWorkingColorSpace', state=modeValue != 2)

    def filenameEdit(self, newFilename) :
        attr = self.nodeAttr('lutFilename')
        cmds.setAttr(attr, newFilename, type="string")
    
    def LoadFilenameButtonPush(self, *args):
        defaultFolder = cmds.workspace(q=True, rd=True)

        basicFilter = 'All Files (*.*)'
        ret = cmds.fileDialog2(fileFilter=basicFilter,
                               cap='Load LUT File',
                               okc='Load', fm=4,
                               dir=defaultFolder)
        if ret is not None and len(ret):
            defaultFolder = ret[0]
            self.filenameEdit(ret[0])
            cmds.textFieldGrp("filenameImageGrp", edit=True, text=ret[0])

    def filenameNew(self, nodeName):
        cmds.rowLayout(nc=2, cw2=(360,30), cl2=('left', 'left'), adjustableColumn=1, columnAttach=[(1, 'left', -4), (2, 'left', 0)])
        path = cmds.textFieldGrp("filenameImageGrp", label="LUT Filename", changeCommand=self.filenameEdit)
        cmds.textFieldGrp(path, edit=True, text=cmds.getAttr(nodeName))
        cmds.symbolButton( image='navButtonBrowse.png', command=self.LoadFilenameButtonPush)

    def filenameReplace(self, nodeName):
        cmds.textFieldGrp( "filenameImageGrp", edit=True, text=cmds.getAttr(nodeName) )

    def lookEdit(self, attrName, mPath) :
        cmds.setAttr(attrName, mPath, type='string')

    def looksListEdit(self, attrName) :

        lookValue = cmds.optionMenu(self.colorSpaces, q = True , value = True)
        cmds.setAttr(attrName, lookValue, type='string')
        # cmds.textField(self.lookTextField, edit=True, text=lookValue)

    def colorSpaceNew(self, attrName) :
        cmds.rowColumnLayout( numberOfColumns=1, columnWidth=[(1,220)], columnAlign=[(1, 'left')], columnAttach=[(1, 'left', 0)]) 
        self.colorSpaces = cmds.optionMenu( label='Working Color Space', changeCommand = lambda *args: self.looksListEdit(attrName))
        # self.lookTextField = cmds.textField( 'lutWorkingColorSpace', changeCommand = lambda *args: self.lookEdit(attrName), width = 200)
        cmds.setParent('..')
        self.colorSpaceReplace(attrName)
        
        
    def colorSpaceReplace(self, attrName) :
        lutAttrName = attrName.replace('.lutWorkingColorSpace', '.lutFilename')
        # cmds.textField(self.lookTextField, edit=True, changeCommand=lambda *args: self.lookEdit(attrName, *args))
        cmds.optionMenu(self.colorSpaces , edit=True, changeCommand = lambda *args: self.looksListEdit(attrName))
        for m in cmds.optionMenu(self.colorSpaces, q=True, itemListLong=True) or []:
            cmds.deleteUI(m)
        filename = cmds.getAttr(lutAttrName)

        color_space_value = cmds.getAttr(attrName)
        color_spaces = cmds.colorManagementPrefs(q=True, inputSpaceNames=True)
        index = 1
        count = 1
        for color in color_spaces:
            if (str(color) == str(color_space_value)):
                index = count
            cmds.menuItem(parent=self.colorSpaces, label=str(color))
            count +=1
        cmds.optionMenu(self.colorSpaces, edit=True, sl = index)
        # cmds.textField(self.lookTextField, edit=True, text=cmds.getAttr(attrName))


registerImagerTemplate("aiImagerTonemap", ImagerTonemapUI)

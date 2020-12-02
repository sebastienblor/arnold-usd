import maya.mel
from mtoa.ui.ae.templates import TranslatorControl
from mtoa.ui.ae.shaderTemplate import ShaderAETemplate
from mtoa.ui.qt import toQtObject
from mtoa.ui.qt import toMayaName
import maya.cmds as cmds
from mtoa.ui.qt.Qt import QtWidgets, QtCore, QtGui
from mtoa.ui.ae.aiImagersBaseTemplate import ImagerBaseUI, registerImagerTemplate

s_pushStyleButton = "QPushButton:pressed {background-color: rgb(150, 150, 150) ;} ";

def getLightGroups():
    # loop over all light groups in the scene
    lights =cmds.ls(exactType=['pointLight', 'directionalLight', 'spotLight', 'areaLight', 'aiAreaLight', 'aiSkyDomeLight', 'aiMeshLight', 'aiPhotometricLight'])
    existingLightGroups = []
    for light in lights:
        lightGroup = cmds.getAttr(light+".aiAov")
        if lightGroup != "" and not lightGroup in existingLightGroups:
            existingLightGroups.append(lightGroup)
    return existingLightGroups

class TintButton(QtWidgets.QPushButton):
    def __init__(self, attribute, parent = None):
        super(TintButton, self).__init__(parent)
        self.attribute = attribute
        self.bgc = cmds.getAttr(self.attribute)
        self.setStyleSheet("background-color:rgb(%d,%d,%d)"%(self.bgc[0][0]*255,self.bgc[0][1]*255,self.bgc[0][2]*255 ))
        self.clicked.connect(self.set_color)

    def set_color(self):
        color = cmds.colorEditor()
        parsedcolor = [float(i) for i in color.split()]
        self.setStyleSheet("background-color:rgb(%d,%d,%d)" %(parsedcolor[0]*255, parsedcolor[1]*255, parsedcolor[2]*255))
        cmds.setAttr(self.attribute, parsedcolor[0] , parsedcolor[1], parsedcolor[2], type = 'float3')


class LightGroupItem(QtWidgets.QFrame):
    
    def __init__(self, parent = None, nodeName = None, index = -1, name = ""):
        super(LightGroupItem, self).__init__(parent=parent)
        self.parent = parent
        self.nodeName = nodeName
        self.itemIndex = index

        self.solo_button = QtWidgets.QPushButton("S")
        self.solo_button.setStyleSheet(s_pushStyleButton)
        self.solo_button.setCheckable(True)
        self.solo_button.setChecked(cmds.getAttr(self.nodeName+'.layer_solo[%d]'%(self.itemIndex)))
        self.solo_button.clicked.connect(self.soloButtonClicked)


        self.enable_button = QtWidgets.QPushButton("E")
        self.enable_button.setStyleSheet(s_pushStyleButton)
        self.enable_button.setCheckable(True)
        self.enable_button.setChecked(cmds.getAttr(self.nodeName+'.layer_enable[%d]'%(self.itemIndex)))
        self.enable_button.clicked.connect(self.enableButtonClicked)

        self.label = QtWidgets.QLabel(name)
        self.mix_slider = QtWidgets.QSlider(QtCore.Qt.Horizontal)
        self.mix_slider.setRange(0,100)
        self.mix_value = QtWidgets.QDoubleSpinBox()
        self.mix_slider.valueChanged.connect(self.sliderValueChanged)
        self.mix_slider.setValue(cmds.getAttr(self.nodeName+'.mix[%d]'%(self.itemIndex))*100)
        
        self.tint_button = TintButton(attribute = self.nodeName + '.tint[%d]'%(self.itemIndex))


        self.addWidget(self.solo_button)
        self.addWidget(self.enable_button)
        self.addWidget(self.label)
        self.addWidget(self.mix_slider)
        self.addWidget(self.mix_value)
        self.addWidget(self.tint_button)

    def soloButtonClicked(self):
        button_state = self.solo_button.isChecked()
        attribute = self.nodeName + '.layerSolo[%d]' %(self.itemIndex)
        cmds.setAttr(attribute, button_state)

    def enableButtonClicked(self):
        button_state = self.enable_button.isChecked()
        attribute = self.nodeName +'.layerEnable[%d]' %(self.itemIndex)
        cmds.setAttr(attribute, button_state)

    def sliderValueChanged(self):
        value =float(self.mix_slider.value())/100.0
        self.mix_value.setValue(value)
        cmds.setAttr(self.nodeName+'.mix[%d]' %(self.itemIndex), value)

class LightMixer(QtWidgets.QWidget):

    def __init__(self, parent = None, nodeName = None):
        super(LightMixer, self).__init__(parent=parent)
        self.mainLayout = QtWidgets.QGridLayout(parent)
        self.nodeName = nodeName
        lightGroups = getLightGroups()
        for i in range(0,len(getLightGroups())):
            if not cmds.attributeQuery( 'layerName[%d]'%(i), node=self.nodeName, exists=True ):
                self.setDefaults(index = i, layerName = lightGroups[i])
            self.item = LightGroupItem(parent = self.mainLayout, name = lightGroups[i], index = i , nodeName = self.nodeName)
            self.mainLayout.addItem(self.item)

    def setDefaults(self, index , layerName):
        cmds.setAttr(self.nodeName+'.layerName[%d]' %(index), "RGBA_"+layerName, type = "string")
        cmds.setAttr(self.nodeName+'.layerEnable[%d]' %(index), True)
        cmds.setAttr(self.nodeName+'.layerSolo[%d]' %(index), False)
        cmds.setAttr(self.nodeName+'.tint[%d]' %(index), 1, 1, 1, type = "float3")
        cmds.setAttr(self.nodeName+'.mix[%d]' %(index), 1)


class AEaiImagerLightMixerTemplate(ShaderAETemplate):

    def setup(self):
        self.beginScrollLayout()
        self.baseLayout = self.beginLayout("Main", collapse=False)
        currentWidget = cmds.setParent(query=True)
        self.ui = ImagerLightMixerUI(parent=currentWidget, nodeName=self.nodeName, template=self)
        self.endLayout()
        maya.mel.eval('AEdependNodeTemplate '+self.nodeName)
        self.addExtraControls()
        self.endScrollLayout()

class ImagerLightMixerUI(ImagerBaseUI):

    def __currentWidget(self, pySideType=QtWidgets.QWidget):
        """Cast and return the current widget."""
        # Get the current widget Maya name.
        currentWidgetName = cmds.setParent(query=True)
        return toQtObject(currentWidgetName, pySideType)
    
    def __init__(self, parent=None, nodeName=None, template=None):
        super(ImagerLightMixerUI, self).__init__(parent, nodeName, template)

    def setup(self):
        super(ImagerLightMixerUI, self).setup()
        self.addSeparator()
        self.beginLayout("Light Mixer", collapse=True)
        currentWidget = self.__currentWidget()
        self.light_mixer = LightMixer(currentWidget,self.nodeName)
        currentWidget.layout().addWidget(self.light_mixer)
        self.endLayout()

registerImagerTemplate("aiImagerLightMixer", ImagerLightMixerUI)

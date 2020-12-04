import maya.mel
from mtoa.ui.ae.templates import TranslatorControl
from mtoa.ui.ae.shaderTemplate import ShaderAETemplate
from mtoa.ui.qt import toQtObject
from mtoa.ui.qt import toMayaName , dpiScale
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

    def update(self, nodeName , index):
        
        if index == -1:
            attribute = nodeName + '.residualTint'
        else:
            attribute = nodeName + '.tint[%d]'%(index)
        
        bgc = cmds.getAttr(attribute)
        self.setStyleSheet("background-color:rgb(%d,%d,%d)"%(bgc[0][0]*255,bgc[0][1]*255,bgc[0][2]*255 ))

class LightGroupItem(QtWidgets.QWidget):

    def __init__(self, parent = None, nodeName = None, index = -1, name = ""):
        super(LightGroupItem, self).__init__()

        self.parent = parent
        self.nodeName = nodeName
        self.itemIndex = index
        self.mainLayout = QtWidgets.QHBoxLayout(self)
        self.setLayout(self.mainLayout)
        self.layerName = name

        self.select_check_box = QtWidgets.QCheckBox()

        self.solo_button = QtWidgets.QPushButton("S")
        self.solo_button.setStyleSheet(s_pushStyleButton)
        self.solo_button.setCheckable(True)
        

        self.solo_button.clicked.connect(self.soloButtonClicked)
        
        self.enable_button = QtWidgets.QPushButton("E")
        self.enable_button.setStyleSheet(s_pushStyleButton)
        self.enable_button.setCheckable(True)
        
        self.enable_button.clicked.connect(self.enableButtonClicked)

        self.label = QtWidgets.QLabel(name,self)
        self.mix_slider = QtWidgets.QSlider(QtCore.Qt.Horizontal)
        self.mix_slider.setRange(0,100)
        self.mix_value = QtWidgets.QDoubleSpinBox()
        self.mix_value.setRange(0.0,1.0)
        self.mix_slider.valueChanged.connect(self.sliderValueChanged)
        

        if self.itemIndex == -1 :
            self.solo_button.setChecked(cmds.getAttr(self.nodeName+'.residualSolo'))
            self.enable_button.setChecked(cmds.getAttr(self.nodeName+'.residualEnable'))
            self.mix_slider.setValue(cmds.getAttr(self.nodeName+'.residualMix')*100)
            self.tint_button = TintButton(attribute = self.nodeName + '.residualTint')
            self.select_check_box.setVisible(False)
        else:
            self.solo_button.setChecked(cmds.getAttr(self.nodeName+'.layerSolo[%d]'%(self.itemIndex)))
            self.enable_button.setChecked(cmds.getAttr(self.nodeName+'.layerEnable[%d]'%(self.itemIndex)))
            self.mix_slider.setValue(cmds.getAttr(self.nodeName+'.mix[%d]'%(self.itemIndex))*100)
            self.tint_button = TintButton(attribute = self.nodeName + '.tint[%d]'%(self.itemIndex))

        self.mainLayout.addWidget(self.select_check_box)
        self.mainLayout.addWidget(self.solo_button)
        self.mainLayout.addWidget(self.enable_button)
        self.mainLayout.addWidget(self.label)
        self.mainLayout.addWidget(self.mix_slider)
        self.mainLayout.addWidget(self.mix_value)
        self.mainLayout.addWidget(self.tint_button)

    def isLayerSelected(self):
        return self.select_check_box.isChecked()

    def update(self, nodeName, index):
        self.nodeName = nodeName
        self.itemIndex = index
        if self.itemIndex == -1:
            self.enable_button.setChecked(cmds.getAttr(nodeName+'.residualEnable'))
            self.solo_button.setChecked(cmds.getAttr(nodeName+'.residualSolo'))
            self.mix_slider.setValue(cmds.getAttr(nodeName+'.residualMix')*100)
            self.tint_button.update(nodeName, -1)
        else:
            self.enable_button.setChecked(cmds.getAttr(nodeName+'.layerEnable[%d]'%(index)))
            self.solo_button.setChecked(cmds.getAttr(nodeName+'.layerSolo[%d]'%(index)))
            self.mix_slider.setValue(cmds.getAttr(nodeName+'.mix[%d]'%(index))*100)
            self.tint_button.update(nodeName, index)

    def soloButtonClicked(self):
        button_state = self.solo_button.isChecked()
        if self.itemIndex == -1 :
            attribute = self.nodeName + '.residualSolo' 
        else:
            attribute = self.nodeName + '.layerSolo[%d]' %(self.itemIndex)
        cmds.setAttr(attribute, button_state)

    def enableButtonClicked(self):
        button_state = self.enable_button.isChecked()
        attribute = None
        if self.itemIndex == -1:
            attribute = self.nodeName +'.residualEnable' 
        else:
            attribute = self.nodeName +'.layerEnable[%d]' %(self.itemIndex)
        cmds.setAttr(attribute, button_state)

    def sliderValueChanged(self):
        value =float(self.mix_slider.value())/100.0
        self.mix_value.setValue(value)
        attribute = None
        
        if self.itemIndex ==-1 :
            attribute = self.nodeName+'.residualMix'
        else:
            attribute = self.nodeName+'.mix[%d]' %(self.itemIndex)
        cmds.setAttr(attribute, value)

class LightMixer(QtWidgets.QFrame):

    def __init__(self, parent = None, nodeName = None):
        super(LightMixer, self).__init__(parent=parent)
        self.nodeName = nodeName
        self.mainLayout = QtWidgets.QVBoxLayout(self)

        self.actionsFrame = QtWidgets.QFrame(self)
        self.actionsLayout = QtWidgets.QHBoxLayout(self.actionsFrame)
        self.actionsFrame.setLayout(self.actionsLayout)

        ## Commenting out the add remove actions until syncing between layers is sorted

        # self.addLayerButton = QtWidgets.QPushButton("Add Layer(s)", self.actionsFrame)
        # self.removeLayerButton = QtWidgets.QPushButton("Remove Layer(s)", self.actionsFrame)
        # self.refreshLayerButton = QtWidgets.QPushButton("Refresh Layer", self.actionsFrame)

        self.actionsLayout.addWidget(self.addLayerButton)
        self.actionsLayout.addWidget(self.removeLayerButton)
        self.actionsLayout.addWidget(self.refreshLayerButton)
        self.addLayerButton.clicked.connect(self.addLayerAction)
        self.removeLayerButton.clicked.connect(self.removeLayerAction)
        self.refreshLayerButton.clicked.connect(self.refreshLayerAction)

        self.layerFrame = QtWidgets.QFrame(self)
        self.layerLayout = QtWidgets.QVBoxLayout(parent)
        self.layerFrame.setLayout(self.layerLayout)

        self.residualLayerFrame = QtWidgets.QFrame(self)
        self.residualLayerLayout = QtWidgets.QVBoxLayout(parent)
        self.residualLayerFrame.setLayout(self.residualLayerLayout)

        lightGroups = getLightGroups()
        self.lightGroupWidgets = []
        if not cmds.getAttr(self.nodeName+'.layerName', size = True) > 0:
            self.setDefaults( lightGroups = getLightGroups() )
        for i in range(0,len(lightGroups)):
            self.item = LightGroupItem(parent = self.mainLayout, name = lightGroups[i], index = i , nodeName = self.nodeName)
            self.lightGroupWidgets.append(self.item)
            self.layerLayout.addWidget(self.item)

        self.residualLayerLayout.addWidget(LightGroupItem(parent = self.mainLayout, name = "Residual", index = -1 , nodeName = self.nodeName))
        self.residualLayerFrame.setFrameStyle(QtWidgets.QFrame.Panel | QtWidgets.QFrame.Sunken)
        self.layerFrame.setFrameStyle(QtWidgets.QFrame.Panel | QtWidgets.QFrame.Sunken)
        self.mainLayout.addWidget(self.actionsFrame)
        self.mainLayout.addWidget(self.layerFrame)
        self.mainLayout.addWidget(self.residualLayerFrame)
        self.setLayout(self.mainLayout)

    def setDefaults(self, lightGroups):
        for index in range(0,len(lightGroups)):
            cmds.setAttr(self.nodeName+'.layerName[%d]' %(index), "RGBA_"+lightGroups[index], type = "string")
            cmds.setAttr(self.nodeName+'.layerEnable[%d]' %(index), True)
            cmds.setAttr(self.nodeName+'.layerSolo[%d]' %(index), False)
            cmds.setAttr(self.nodeName+'.tint[%d]' %(index), 1, 1, 1, type = "float3")
            cmds.setAttr(self.nodeName+'.mix[%d]' %(index), 1)

    def update(self, nodeName):
        
        for i in range(0,len(self.lightGroupWidgets)):
            if self.lightGroupWidgets[i]:
                self.lightGroupWidgets[i].update(nodeName,i)

    def getWidgetLayers(self):
        layers_in_widget = []
        for item in self.lightGroupWidgets:
            layers_in_widget.append(item.layerName)

    def getSelectedLayers(self):
        selectedIndices = []
        for i in range(0,len(self.lightGroupWidgets)):
            if self.lightGroupWidgets[i].isLayerSelected():
                selectedIndices.append(i)
        return selectedIndices


    def addLayerAction(self):
        light_groups_in_scene = getLightGroups()
        items_to_add = []
        light_groups_in_widget = self.getWidgetLayers()
        for item in self.light_groups_in_scene:
            if item not in light_groups_in_widget:
                items_to_add.append(item)
        
        print items_to_add
        
        # if not cmds.getAttr(self.nodeName+'.layerName', size = True) > 0:
        #     self.setDefaults( lightGroups = getLightGroups() )
        # for i in range(0,len(lightGroups)):
        #     self.item = LightGroupItem(parent = self.mainLayout, name = lightGroups[i], index = i , nodeName = self.nodeName)
        #     self.lightGroupWidgets[lightGroups[i]] = self.item
        #     self.layerLayout.addWidget(self.item)

    
    def removeLayerAction(self):
        selectedLayers = self.getSelectedLayers()

        print "Going to remove from node" , self.nodeName

        ## Remove the UI element
        for index in selectedLayers:
            item = self.layerLayout.itemAt(index)
            item.widget().close()
            self.layerLayout.removeItem(item)
            self.lightGroupWidgets[index] = None
        
        for index in selectedLayers:
            cmds.removeMultiInstance( self.nodeName+'.layerName[%d]'%(index))
            cmds.removeMultiInstance( self.nodeName+'.layerEnable[%d]'%(index))
            cmds.removeMultiInstance( self.nodeName+'.layerSolo[%d]'%(index))
            cmds.removeMultiInstance( self.nodeName+'.mix[%d]'%(index))
            cmds.removeMultiInstance( self.nodeName+'.tint[%d]'%(index))
        

    def refreshLayerAction(self):
        print "Refresh Layer Action"


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
        self.addControl("outputName")
        self.addSeparator()
        self.addSeparator()
        self.addCustom("mixerWidget", self.createLightMixerWidget , self.updateLightMixerWidget)
        

    
    def createLightMixerWidget(self, nodeName):
        currentWidget = self.__currentWidget()
        node = nodeName.split('.')[0]
        self.mixer = LightMixer(currentWidget, node)
        currentWidget.layout().addWidget(self.mixer)

    def updateLightMixerWidget(self, nodeName):
        node = nodeName.split('.')[0]
        self.mixer.update(node)

registerImagerTemplate("aiImagerLightMixer", ImagerLightMixerUI)

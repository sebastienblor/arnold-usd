import maya.mel
from mtoa.ui.ae.templates import TranslatorControl
from mtoa.ui.ae.shaderTemplate import ShaderAETemplate
from mtoa.ui.qt import toQtObject, clearWidget
from mtoa.ui.qt import toMayaName , dpiScale
import maya.cmds as cmds
from mtoa.ui.qt.Qt import QtWidgets, QtCore, QtGui
from mtoa.ui.ae.aiImagersBaseTemplate import ImagerBaseUI, registerImagerTemplate, LayersList
from mtoa.ui.qt import BaseItem

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
        attribute = nodeName + '.layerTint[%d]'%(index)
        
        bgc = cmds.getAttr(attribute)
        self.setStyleSheet("background-color:rgb(%d,%d,%d)"%(bgc[0][0]*255,bgc[0][1]*255,bgc[0][2]*255 ))

class CustomDelete(QtCore.QObject):
    sendDelete = QtCore.Signal(int)
    def __init__(self):
        super(CustomDelete, self).__init__()  


class LightGroupItem(QtWidgets.QWidget):

    def __init__(self, parent = None, nodeName = None, index = -1, name = ""):
        super(LightGroupItem, self).__init__()
        self.parent = parent
        self.nodeName = nodeName
        self.itemIndex = index
        self.mainLayout = QtWidgets.QHBoxLayout(self)
        self.setLayout(self.mainLayout)
        self.layerName = name
        self.itemDeleted = CustomDelete()
        self.setAttribute(QtCore.Qt.WA_DeleteOnClose) 
        if self.itemIndex >=0 :
            objectParent = 'LightGroupItem'+ str(self.itemIndex)
        else:
            objectParent = 'LightGroupItem' 
        
        self.setObjectName(objectParent)
        
        DISABLED_ICON = BaseItem.dpiScaledIcon(":/RS_disable.png")
        SOLO_ICON = BaseItem.dpiScaledIcon(":/RS_isolate.png")
        BIN_ICON = BaseItem.dpiScaledIcon(":/RS_delete.png")

        self.delete_button = QtWidgets.QPushButton()
        self.delete_button.setIcon(BIN_ICON)
        self.delete_button.clicked.connect(self.deleteAction)
        self.delete_button.setToolTip("Delete Layer")
        self.solo_button = QtWidgets.QPushButton()
        self.solo_button.setStyleSheet(s_pushStyleButton)
        self.solo_button.setCheckable(True)
        self.solo_button.setIcon(SOLO_ICON)
        self.solo_button.setToolTip("Solo Layer")

        self.solo_button.clicked.connect(self.soloButtonClicked)
        
        self.enable_button = QtWidgets.QPushButton()
        self.enable_button.setStyleSheet(s_pushStyleButton)
        self.enable_button.setCheckable(True)
        self.enable_button.setIcon(DISABLED_ICON)
        self.enable_button.toggled.connect(self.enableButtonClicked)
        self.enable_button.setToolTip("Enable/Disable Layer")

        self.label = QtWidgets.QLabel(name,self)
        self.mix_slider = QtWidgets.QSlider(QtCore.Qt.Horizontal)
        self.mix_slider.setRange(0,1000)
        self.mix_value = QtWidgets.QDoubleSpinBox()
        self.mix_value.setRange(0.0,10.0)
        self.mix_value.setToolTip("Set the Intensity Value")
        self.mix_slider.setToolTip("Set the Intensity Value")
        self.mix_slider.valueChanged.connect(self.intensitySliderValueChanged)
        self.mix_value.valueChanged.connect(self.intensityValueChanged)
        
        self.exposure_slider = QtWidgets.QSlider(QtCore.Qt.Horizontal)
        self.exposure_slider.setRange(-500,500)
        self.exposure_value = QtWidgets.QDoubleSpinBox()
        self.exposure_value.setRange(-5.0,5.0)
        self.exposure_slider.setToolTip("Set the Exposure Value")
        self.exposure_value.setToolTip("Set the Exposure Value")
        self.exposure_slider.valueChanged.connect(self.exposureSliderValueChanged)
        self.exposure_value.valueChanged.connect(self.exposureValueChanged)

        self.scriptJobList = []

        self.solo_button.setChecked(cmds.getAttr(self.nodeName+'.layerSolo[%d]'%(self.itemIndex)))
        self.enable_button.setChecked(not cmds.getAttr(self.nodeName+'.layerEnable[%d]'%(self.itemIndex)))
        self.mix_slider.setValue(cmds.getAttr(self.nodeName+'.layerIntensity[%d]'%(self.itemIndex)) * 100)
        self.exposure_slider.setValue(cmds.getAttr(self.nodeName+'.layerExposure[%d]'%(self.itemIndex)) * 100)
        self.tint_button = TintButton(attribute = self.nodeName + '.layerTint[%d]'%(self.itemIndex))
        self.scriptJobList.append(cmds.scriptJob(p = objectParent, attributeChange=[self.nodeName+'.layerSolo', lambda *args: self.update(self.nodeName, self.itemIndex)]))
        self.scriptJobList.append(cmds.scriptJob(p = objectParent, attributeChange=[self.nodeName+'.layerEnable', lambda *args: self.update(self.nodeName, self.itemIndex)]))
        self.scriptJobList.append(cmds.scriptJob(p = objectParent, attributeChange=[self.nodeName+'.layerIntensity', lambda *args: self.update(self.nodeName, self.itemIndex)]))
        self.scriptJobList.append(cmds.scriptJob(p = objectParent, attributeChange=[self.nodeName+'.layerExposure', lambda *args: self.update(self.nodeName, self.itemIndex)]))
        self.scriptJobList.append(cmds.scriptJob(p = objectParent, attributeChange=[self.nodeName+'.layerTint', lambda *args: self.update(self.nodeName, self.itemIndex)]))
        self.scriptJobList.append(cmds.scriptJob(p = objectParent, attributeDeleted=[self.nodeName+'.layerName[%d]'%(self.itemIndex), lambda *args: self.destroy()]))

        self.mainLayout.addWidget(self.delete_button)
        self.mainLayout.addWidget(self.solo_button)
        self.mainLayout.addWidget(self.enable_button)
        self.mainLayout.addWidget(self.label)
        self.mainLayout.addWidget(self.mix_slider)
        self.mainLayout.addWidget(self.mix_value)
        self.mainLayout.addWidget(self.exposure_slider)
        self.mainLayout.addWidget(self.exposure_value)
        self.mainLayout.addWidget(self.tint_button)

    def deleteAction(self):
        self.itemDeleted.sendDelete.emit(self.itemIndex)

    def destroy(self):
        self.close()
    
    def update(self, nodeName, index):
        
        self.nodeName = nodeName
        self.itemIndex = index
        multiIndices = cmds.getAttr(self.nodeName+'.layerName', mi=True) or []
        if index in multiIndices:
            enabled = cmds.getAttr(nodeName+'.layerEnable[%d]'%(index))
            self.enable_button.setChecked(not enabled)
            self.solo_button.setChecked(cmds.getAttr(nodeName+'.layerSolo[%d]'%(index)))
            self.mix_slider.setValue(cmds.getAttr(nodeName+'.layerIntensity[%d]'%(index)) * 100)
            self.exposure_slider.setValue(cmds.getAttr(nodeName+'.layerExposure[%d]'%(index)) * 100)
            self.tint_button.update(nodeName, index)
        else:
            self.destroy()

    def soloButtonClicked(self):
        button_state = self.solo_button.isChecked()
        attribute = self.nodeName + '.layerSolo[%d]' %(self.itemIndex)
        cmds.setAttr(attribute, button_state)

    def enableButtonClicked(self, disabled):
        enabled = not disabled
        self.mix_slider.setEnabled(enabled)
        self.mix_value.setEnabled(enabled)
        self.exposure_slider.setEnabled(enabled)
        self.exposure_value.setEnabled(enabled)
        self.tint_button.setEnabled(enabled)
        self.label.setEnabled(enabled)

        attribute = None
        attribute = self.nodeName +'.layerEnable[%d]' %(self.itemIndex)
        cmds.setAttr(attribute, enabled)

    def intensityValueChanged(self, value):
        self.mix_slider.setValue(value * 100)
        self.setIntensityAttr(value)

    def intensitySliderValueChanged(self, value):
        value = value/100.0
        self.mix_value.setValue(value)
        self.setIntensityAttr(value)

    def setIntensityAttr(self, value):
        attribute = None
        attribute = '{}.layerIntensity[{}]'.format(self.nodeName, self.itemIndex)
        cmds.setAttr(attribute, value)

    def exposureValueChanged(self, value):
        self.exposure_slider.setValue(value * 100)
        self.setExposureAttr(value)

    def exposureSliderValueChanged(self, value):
        value = value/100.0
        self.exposure_value.setValue(value)
        self.setExposureAttr(value)

    def setExposureAttr(self, value):
        attribute = None
        attribute = '{}.layerExposure[{}]'.format(self.nodeName, self.itemIndex)
        cmds.setAttr(attribute, value)


# class LightGroupLayers(QtWidgets.QDialog):
#     def __init__(self, parent=None, displayList = None  ):
#         super(LightGroupLayers, self).__init__(parent)
#         buttons = QtWidgets.QDialogButtonBox.Ok | QtWidgets.QDialogButtonBox.Cancel
#         self.buttonBox = QtWidgets.QDialogButtonBox(buttons)
#         self.buttonBox.accepted.connect(self.accept)
#         self.buttonBox.rejected.connect(self.reject)

#         self.mainLayout = QtWidgets.QVBoxLayout(self)
#         self.setWindowTitle("Light Group Layer(s)")
#         self.list = QtWidgets.QListWidget(self)
#         self.list.setSelectionMode(QtWidgets.QAbstractItemView.ExtendedSelection)
#         for item in displayList:
#             self.list.addItem(item)
#         self.setLayout(self.mainLayout)
#         self.mainLayout.addWidget(self.list)
#         self.mainLayout.addWidget(self.buttonBox)


class LightMixer(QtWidgets.QFrame):

    def __init__(self, parent = None, nodeName = None):
        super(LightMixer, self).__init__(parent=parent)
        self.nodeName = nodeName
        sizePolicy = QtWidgets.QSizePolicy(QtWidgets.QSizePolicy.Expanding, QtWidgets.QSizePolicy.Expanding)
        self.mainLayout = QtWidgets.QVBoxLayout(self)
        self.setSizePolicy(sizePolicy)

        self.actionsFrame = QtWidgets.QFrame(self)
        self.actionsLayout = QtWidgets.QHBoxLayout(self.actionsFrame)
        self.actionsFrame.setLayout(self.actionsLayout)

        ADD_LAYER_ICON =  BaseItem.dpiScaledIcon(":/RS_create_layer.png")
        self.addLayerButton = QtWidgets.QPushButton( self.actionsFrame)
        self.addLayerButton.setIcon(ADD_LAYER_ICON)

        self.actionsLayout.addWidget(self.addLayerButton)
        self.actionsLayout.addWidget(QtWidgets.QLabel())
        self.actionsLayout.addWidget(QtWidgets.QLabel())
        self.actionsLayout.addWidget(QtWidgets.QLabel())
        self.actionsLayout.addWidget(QtWidgets.QLabel())
        self.addLayerButton.clicked.connect(self.addLayerAction)

        self.layerFrame = QtWidgets.QFrame(self)
        self.layerFrame.setSizePolicy(sizePolicy)
        self.layerLayout = QtWidgets.QVBoxLayout(self.layerFrame)
        self.layerFrame.setLayout(self.layerLayout)
        self.lightGroupWidgets = []

        self.layerFrame.setFrameStyle(QtWidgets.QFrame.Panel | QtWidgets.QFrame.Sunken)
        self.mainLayout.addWidget(self.actionsFrame)
        self.mainLayout.addWidget(self.layerFrame)
        self.setLayout(self.mainLayout)

        self.update(self.nodeName)

    def setDefaults(self, lightGroups):
        for index in range(0,len(lightGroups)):
            cmds.setAttr(self.nodeName+'.layerName[%d]' %(index), "RGBA_"+lightGroups[index], type = "string")
            cmds.setAttr(self.nodeName+'.layerEnable[%d]' %(index), True)
            cmds.setAttr(self.nodeName+'.layerSolo[%d]' %(index), False)
            cmds.setAttr(self.nodeName+'.layerTint[%d]' %(index), 1, 1, 1, type = "float3")
            cmds.setAttr(self.nodeName+'.layerIntensity[%d]' %(index), 1)
            cmds.setAttr(self.nodeName+'.layerExposure[%d]' %(index), 0)

    def update(self, nodeName):
        self.nodeName = nodeName

        # clear the previous widgets
        clearWidget(self.layerFrame)
        self.lightGroupWidgets = []

        lightGroups = getLightGroups()
        if not cmds.getAttr(self.nodeName+'.layerName', size=True) > 0:
            self.setDefaults(lightGroups=lightGroups)

        multiIndices = cmds.getAttr(self.nodeName+'.layerName', mi=True) or []

        for i in multiIndices:
            layerName = cmds.getAttr('{}.layerName[{}]'.format(self.nodeName, i))
            item = LightGroupItem(parent=self.mainLayout, name=layerName, index=i, nodeName=self.nodeName)
            self.lightGroupWidgets.append(item)
            self.layerLayout.addWidget(item)
            item.itemDeleted.sendDelete.connect(self.removeLayerAction)

    def getWidgetLayers(self):
        layers_in_widget = []
        for item in self.lightGroupWidgets:
            if item:
                layers_in_widget.append(item.layerName)
        return layers_in_widget

    def addLayerAction(self):
        
        light_groups_in_scene = getLightGroups()
        items_to_add = []
        light_groups_in_widget = self.getWidgetLayers()
        for item in light_groups_in_scene:
            if 'RGBA_'+item not in light_groups_in_widget:
                items_to_add.append(item)

        if '<residual_lights>' not in light_groups_in_widget:
            items_to_add.append('<residual_lights>')
        
        selected_items = []
        popup = LayersList(self, items_to_add)
        if popup.exec_():
            for item in popup.list.selectedItems():
                selected_items.append(item.text())
        else:
            return

        for newItem in selected_items:
            if cmds.getAttr(self.nodeName+'.layerName', mi = True):
                index = cmds.getAttr(self.nodeName+'.layerName', mi = True)[-1]+1
            else:
                index = 0

            if '<residual_lights>' not in newItem:
                cmds.setAttr(self.nodeName+'.layerName[%d]' %(index), "RGBA_"+newItem, type = "string")
            else:
                cmds.setAttr(self.nodeName+'.layerName[%d]' %(index), newItem, type = "string")
            cmds.setAttr(self.nodeName+'.layerEnable[%d]' %(index), True)
            cmds.setAttr(self.nodeName+'.layerSolo[%d]' %(index), False)
            cmds.setAttr(self.nodeName+'.layerTint[%d]' %(index), 1, 1, 1, type = "float3")
            cmds.setAttr(self.nodeName+'.layerIntensity[%d]' %(index), 1)
            cmds.setAttr(self.nodeName+'.layerExposure[%d]' %(index), 1)

            item = LightGroupItem(parent = self.mainLayout, name = 'RGBA_'+newItem, index = index , nodeName = self.nodeName)
            self.lightGroupWidgets.append(item)
            self.layerLayout.addWidget(item)
            item.itemDeleted.sendDelete.connect(self.removeLayerAction)

    def removeLayerAction(self,index):
        cmds.removeMultiInstance( self.nodeName+'.layerName[%d]'%(index))
        cmds.removeMultiInstance( self.nodeName+'.layerEnable[%d]'%(index))
        cmds.removeMultiInstance( self.nodeName+'.layerSolo[%d]'%(index))
        cmds.removeMultiInstance( self.nodeName+'.layerIntensity[%d]'%(index))
        cmds.removeMultiInstance( self.nodeName+'.layerExposure[%d]'%(index))
        cmds.removeMultiInstance( self.nodeName+'.layerTint[%d]'%(index))
        self.lightGroupWidgets[index].close()
        self.lightGroupWidgets[index] = None

class AEaiImagerLightMixerTemplate(ShaderAETemplate):

    def setup(self):
        self.beginScrollLayout()
        self.baseLayout = self.beginLayout("Main", collapse=False)
        currentWidget = cmds.setParent(query=True)
        # self.ui = ImagerLightMixerUI(parent=currentWidget, nodeName=self.nodeName, template=self)
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
        self.addCustom("mixerWidget", self.createLightMixerWidget , self.updateLightMixerWidget)
    
    def createLightMixerWidget(self, nodeName):
        currentWidget = self.__currentWidget()
        node = nodeName.split('.')[0]
        self.mixer = LightMixer(currentWidget, node)
        currentWidget.layout().addWidget(self.mixer)

    def updateLightMixerWidget(self, nodeName):
        if self.mixer:
            node = nodeName.split('.')[0]
            self.mixer.update(node)

registerImagerTemplate("aiImagerLightMixer", ImagerLightMixerUI)

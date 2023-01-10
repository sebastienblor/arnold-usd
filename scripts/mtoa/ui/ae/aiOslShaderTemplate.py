import maya.mel
import maya.cmds as cmds
from mtoa.ui.ae.shaderTemplate import ShaderAETemplate
from mtoa.ui.ae.utils import AttrControlGrp
from mtoa.ui.qt import toQtObject, dpiScale
from mtoa.ui.qt.Qt import QtWidgets
from mtoa.ui.qt.codeEditor import BaseCodeEditor
from mtoa.ui.qt.syntaxhighlighter import OSLHighlighter
import re
import os
import mtoa.osl

from arnold import *
from maya import OpenMaya as om

defaultFolder = ""


class AEaiOslShaderTemplate(ShaderAETemplate):

    def __currentWidget(self, pySideType=QtWidgets.QWidget):
        """Cast and return the current widget."""
        # Get the current widget Maya name.
        currentWidgetName = cmds.setParent(query=True)
        return toQtObject(currentWidgetName, pySideType)

    def setup(self):
        self.codeAttr = ''
        self.compileEnum = ['Needs (Re)Compile', 'Compile Success', 'Compile Warnings', 'Compile Failure']
        self._controls = []
        self._attrs = {}
        self.code_widget = None
        self.addSwatch()
        self.beginScrollLayout()
        self.beginLayout(' OSL Code ', collapse=False)
        self.addSeparator()
        self.addSeparator()
        self.addCustom('compileStatus', self.codeStatusCreate, self.codeStatusUpdate)
        self.addSeparator()
        self.addSeparator()
        self.addCustom('code', self.codeWidgetCreate, self.codeWidgetUpdate)
        self.addSeparator()
        self.addSeparator()
        self.addCustom('compile', self.compileButtonCreate, self.compileButonUpdate)
        self.addSeparator()
        self.addSeparator()
        self.addCustom('importExport', self.importExportCreate, self.importExportUpdate)
        self.addSeparator()
        self.addSeparator()

        self.endLayout()
        self.oslAttributeLayout = None
        self.beginLayout('OSL Attributes', collapse=False)
        self.addCustom('oslAttribute', self.addOslAttributes, self.updateOslAttributes)
        self.endLayout()
        maya.mel.eval('AEdependNodeTemplate ' + self.nodeName)
        self.suppress('shadername')

        self.addExtraControls()
        self.endScrollLayout()

    def codeStatusCreate(self, attrName):
        if not cmds.attributeQuery("compileStatus", node=self.nodeName, ex=True):
            cmds.addAttr(self.nodeName, ln="compileStatus", sn="cmpSts", at="enum", enumName=':'.join(self.compileEnum), hidden=True)
        cmds.rowLayout(numberOfColumns=2, columnWidth2=(100, 100), adjustableColumn=1, columnAlign=(1, 'right'), columnAttach=[(1, 'both', 0), (2, 'both', 0)])
        cmds.text(" Compile Status", al="left", fn="smallBoldLabelFont")
        compileStatusText = self.compileEnum[cmds.getAttr(attrName)]
        self.compile_control = cmds.text(compileStatusText, al="right", fn="boldLabelFont")
        cmds.setParent("..")

    def codeStatusUpdate(self, attrName):
        if not cmds.attributeQuery("compileStatus", node=self.nodeName, ex=True):
            cmds.addAttr(self.nodeName, ln="compileStatus", sn="cmpSts", at="enum", enumName=':'.join(self.compileEnum), hidden=True)

        compileStatus = cmds.getAttr(attrName)
        bg_color = [0.0, 0.0, 0.0]
        if compileStatus == 0:
            bg_color = [0.0, 1.0, 1.0]
        elif compileStatus == 1:
            bg_color = [0.0, 1.0, 0.0]
        elif compileStatus == 2:
            bg_color = [1.0, 1.0, 0.0]
        elif compileStatus == 3:
            bg_color = [1.0, 0.0, 0.0]
        cmds.text(self.compile_control, e=True, l=self.compileEnum[compileStatus], bgc=bg_color)

    def addOslAttributes(self, attrName):
        self.updateOslAttributes(attrName)

    def updateOslAttributes(self, attrName):
        if not self.oslAttributeLayout:
            self.oslAttributeLayout = cmds.columnLayout(adjustableColumn=True)

        for ctrl in self._controls:
            cmds.deleteUI(ctrl)
        del self._controls[:]
        nodeName = attrName.split('.')[0]
        nodeAttrs = cmds.listAttr(nodeName, userDefined=True, write=True, ct="oslAttribute")
        if nodeAttrs:
            for attr in nodeAttrs:
                shortName = cmds.attributeQuery(attr, node=nodeName, sn=True)
                kwargs = {}
                kwargs['label'] = attr
                kwargs['attribute'] = nodeName + '.' + shortName
                kwargs['parent'] = self.oslAttributeLayout
                control = AttrControlGrp(**kwargs)
                self._controls.append(control.control)
            self._controls.sort()
        cmds.setParent("..")

    def codeWidgetCreate(self, attrName):
        currentWidget = self.__currentWidget()

        self.code_widget = BaseCodeEditor(currentWidget, OSLHighlighter)
        currentWidget.layout().addWidget(self.code_widget)
        self.code_widget.setFixedHeight(dpiScale(400))

        self.code_widget.textChanged.connect(self.setShaderCode)
        self.codeWidgetUpdate(attrName)

    def codeWidgetUpdate(self, attrName):
        self.codeAttr = attrName
        osl_code = cmds.getAttr(attrName + 'Cache')
        if osl_code == '':
            osl_code = cmds.getAttr(attrName)
        if self.code_widget:
            self.code_widget.setPlainText(osl_code)
        else:
            self.codeStatusCreate(attrName)

    def compileButtonCreate(self, attrName):
        cmds.button('compileButtonPath', label='Compile OSL Code', command=lambda *args: self.compiler(attrName))

    def compileButonUpdate(self, attrName):
        cmds.button('compileButtonPath', e=True, label='Compile OSL Code', command=lambda *args: self.compiler(attrName))

    def compiler(self, attrName):
        nodeName = attrName.split('.')[0]
        compileText = cmds.getAttr(nodeName + '.codeCache');
        # avoid compilation for empty shader code
        if compileText == "":
            return
        # reset attrs
        self._attrs[nodeName] = {}
        # store attribute values
        nodeAttrs = cmds.listAttr(nodeName, userDefined=True, write=True, ct="oslAttribute")
        if nodeAttrs:
            for attr in nodeAttrs:
                shortName = cmds.attributeQuery(attr, node=nodeName, sn=True)
                attrValue = cmds.getAttr("%s.%s" % (nodeName, attr))
                attrType = cmds.getAttr("%s.%s" % (nodeName, attr), type=True)
                self._attrs[nodeName][attr] = (attrType, attrValue)
        fromSlots = []
        toSlots = []
        incoming = cmds.listConnections(nodeName, d=False, s=True, p=True)
        if incoming != None:
            for inConnection in incoming:
                outgoing = cmds.listConnections(inConnection, s=False, d=True, p=True)
                if outgoing != None:
                    for outConnection in outgoing:
                        outNodeName = outConnection.split('.')[0]
                        if outNodeName == nodeName:
                            fromSlots.append(inConnection)
                            toSlots.append(outConnection)
        cmds.setAttr(nodeName + '.code', compileText, type="string")
        oslMayaScene = mtoa.osl.OSLSceneModel(compileText, nodeName)
        if oslMayaScene.compileState:
            if len(oslMayaScene.compilationWarnings):
                om.MGlobal.displayInfo(" Code compiled with Warnings ")
                for warn in oslMayaScene.compilationWarnings:
                    cmds.warning(warn)
                self.setCodeStatus(attrName, 2)
            else:
                om.MGlobal.displayInfo(" Code compiled successfully ")
                self.setCodeStatus(attrName, 1)
        else:
            self.setCodeStatus(attrName, 3)
            om.MGlobal.displayError(" Compilation Failed")
            for error in oslMayaScene.compilationErrors:
                om.MGlobal.displayError(error)
            return
        om.MGlobal.displayInfo(" Code successfully compiled, restoring attribute values and connections ")
        # restore attribute values
        for attr in self._attrs[nodeName].keys():
            attrType, attrValue = self._attrs[nodeName][attr]
            if cmds.attributeQuery(attr, node=nodeName, ex=True):
                if attrType == "long": # OSL: int
                    cmds.setAttr("%s.%s" % (nodeName, attr), attrValue)
                elif attrType == "bool": # OSL: not sure this even exists
                    cmds.setAttr("%s.%s" % (nodeName, attr), attrValue)
                elif attrType == "float": # OSL: float
                    cmds.setAttr("%s.%s" % (nodeName, attr), attrValue)
                elif attrType == "float2": # OSL: not sure this even exists
                    cmds.setAttr("%s.%s" % (nodeName, attr), attrValue[0][0], attrValue[0][1], type="float2")
                elif attrType == "float3": # OSL: point, vector, or normal, color
                    cmds.setAttr("%s.%s" % (nodeName, attr), attrValue[0][0], attrValue[0][1], attrValue[0][2], type="float3")
                elif attrType == "string": # OSL: string
                    cmds.setAttr("%s.%s" % (nodeName, attr), "%s" % attrValue, type="string")
                else:
                    # TODO: matrix (and void returning functions?)
                    om.MGlobal.displayInfo(" TODO: attrType = %s " % attrType)
            # else:
            #     om.MGlobal.displayInfo(" WARNING: %s.%s does not exist " % (nodeName, attr))
        # reset attrs
        self._attrs[nodeName] = {}
        # connect slots
        for index in range(len(fromSlots)):
            fromSlot = fromSlots[index]
            toSlot = toSlots[index]
            cmds.connectAttr(fromSlot, toSlot, force=True)

    def setShaderCode(self):
        nodeName = self.codeAttr.split('.')[0]
        compileText = self.code_widget.toPlainText()
        if compileText == cmds.getAttr(self.codeAttr):
            return
        cmds.setAttr(self.codeAttr + 'Cache', compileText, type="string")
        self.setCodeStatus(self.codeAttr, 0)

    def setCodeStatus(self, attrName, status):
        nodeName = attrName.split('.')[0]
        attrName = nodeName + '.compileStatus'
        cmds.setAttr(attrName, status)
        self.codeStatusUpdate(attrName)

    def importExportCreate(self, attrName):
        cmds.rowLayout(numberOfColumns=3, columnWidth3=(100, 100, 100), adjustableColumn=2, columnAlign=(1, 'right'), columnAttach=[(1, 'both', 0), (2, 'both', 0), (3, 'both', 0)])
        self.importButton = cmds.button("Import", command=lambda *args: self.importOSL(attrName))
        cmds.text(" ")
        self.exportButton = cmds.button(" Export ", command=lambda *args: self.exportOSL(attrName))
        cmds.setParent("..")

    def importExportUpdate(self, attrName):
        cmds.button(self.importButton, edit=True, command=lambda *args: self.importOSL(attrName))
        cmds.button(self.exportButton, edit=True, command=lambda *args: self.exportOSL(attrName))

    def importOSL(self, attrName):
        nodeName = attrName.split('.')[0]
        basicFilter = 'OSL Files (*.osl)'
        global defaultFolder
        if defaultFolder == "":
            defaultFolder = "{}/{}".format(cmds.workspace(q=True, rd=True),
                                           cmds.workspace(fre="OSL")
                                           )
        ret = cmds.fileDialog2(fileFilter=basicFilter, cap='Import OSL', okc='Load', fm=1, startingDirectory=defaultFolder)
        if ret is not None and len(ret):
            defaultFolder = ret[0]
            code_string = ''
            with open(ret[0], 'r') as myfile:
                code_string = myfile.read()
            codeAttr = attrName.split('.')[0] + '.code'
            if code_string != cmds.getAttr(codeAttr):
                cmds.setAttr(codeAttr + 'Cache', code_string, type="string")
                self.code_widget.setPlainText(code_string)
                self.setCodeStatus(attrName, 1)
                self.compiler(attrName)

    def exportOSL(self, attrName):
        basicFilter = 'OSL Files (*.osl)'
        defaultDir = cmds.workspace(query=True, directory=True)
        ret = cmds.fileDialog2(fileFilter=basicFilter, cap='Export OSL', dialogStyle=2, startingDirectory=defaultDir)
        if ret is not None and len(ret):
            codeAttr = attrName.split('.')[0] + '.code'
            code_string = cmds.getAttr(codeAttr)
            with open(ret[0], 'w') as myfile:
                myfile.write(code_string)

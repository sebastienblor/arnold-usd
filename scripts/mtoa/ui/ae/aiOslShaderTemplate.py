import maya.mel
import maya.cmds as cmds
from mtoa.ui.ae.shaderTemplate import ShaderAETemplate
from mtoa.ui.ae.utils import AttrControlGrp
import re
import os

from arnold import *
from maya import OpenMaya as om


def to_camelcase(s):
    return re.sub(r'(?!^)_([a-zA-Z])', lambda m: m.group(1).upper(), s)


def createMayaAttributeForArnoldAttributes(mayaNodeName, mayaAttributeName, arnoldAttributeType, defaultValue):
    '''
    Create maya attributes for all Arnold-OSL attributes with their default values.
    We dont support AI_TYPE_ARRAY AI_TYPE_POINTER AI_TYPE_NODE AI_TYPE_MATRIX attribute types yet.
    '''

    mayaAttributeLongName = mayaAttributeName.split('param_')[-1]
    mayaAttributeName = to_camelcase(mayaAttributeName)

    if arnoldAttributeType == AI_TYPE_BYTE:
        if not cmds.attributeQuery(mayaAttributeLongName, node=mayaNodeName, ex=True):
            cmds.addAttr(mayaNodeName, sn=mayaAttributeName, longName=mayaAttributeLongName, at='byte', w=True, r=False, ct="oslAttribute")
    elif arnoldAttributeType == AI_TYPE_INT:
        if not cmds.attributeQuery(mayaAttributeLongName, node=mayaNodeName, ex=True):
            attrDefaultValue = defaultValue.contents.INT
            cmds.addAttr(mayaNodeName, sn=mayaAttributeName, longName=mayaAttributeLongName, at='long', dv=attrDefaultValue, w=True, r=False, ct="oslAttribute")
    elif arnoldAttributeType == AI_TYPE_UINT:
        if not cmds.attributeQuery(mayaAttributeLongName, node=mayaNodeName, ex=True):
            attrDefaultValue = defaultValue.contents.INT
            cmds.addAttr(mayaNodeName, sn=mayaAttributeName, longName=mayaAttributeLongName, at='long', dv=attrDefaultValue, w=True, r=False, ct="oslAttribute")
    elif arnoldAttributeType == AI_TYPE_BOOLEAN:
        if not cmds.attributeQuery(mayaAttributeLongName, node=mayaNodeName, ex=True):
            attrDefaultValue = defaultValue.contents.BOOL
            cmds.addAttr(mayaNodeName, sn=mayaAttributeName, longName=mayaAttributeLongName, at='bool', dv=attrDefaultValue, w=True, r=False, ct="oslAttribute")
    elif arnoldAttributeType == AI_TYPE_FLOAT:
        if not cmds.attributeQuery(mayaAttributeLongName, node=mayaNodeName, ex=True):
            attrDefaultValue = defaultValue.contents.FLT
            cmds.addAttr(mayaNodeName, sn=mayaAttributeName, longName=mayaAttributeLongName, at="float", dv=attrDefaultValue, w=True, r=False, ct="oslAttribute")
    elif arnoldAttributeType == AI_TYPE_RGB or arnoldAttributeType == AI_TYPE_RGBA or arnoldAttributeType == AI_TYPE_CLOSURE:
        if not cmds.attributeQuery(mayaAttributeLongName, node=mayaNodeName, ex=True):
            dv_x, dv_y, dv_z = (defaultValue.contents.RGB.r, defaultValue.contents.RGB.g, defaultValue.contents.RGB.b)
            cmds.addAttr(mayaNodeName, sn=mayaAttributeName, longName=mayaAttributeLongName, at="float3", uac=True, w=True, r=False, ct="oslAttribute")
            cmds.addAttr(mayaNodeName, sn=mayaAttributeName + 'R', longName=mayaAttributeLongName + 'R', at="float", parent=mayaAttributeLongName, dv=dv_x, w=True, r=False)
            cmds.addAttr(mayaNodeName, sn=mayaAttributeName + 'G', longName=mayaAttributeLongName + 'G', at="float", parent=mayaAttributeLongName, dv=dv_y, w=True, r=False)
            cmds.addAttr(mayaNodeName, sn=mayaAttributeName + 'B', longName=mayaAttributeLongName + 'B', at="float", parent=mayaAttributeLongName, dv=dv_z, w=True, r=False)
    elif arnoldAttributeType == AI_TYPE_VECTOR:
        if not cmds.attributeQuery(mayaAttributeLongName, node=mayaNodeName, ex=True):
            dv_x, dv_y, dv_z = (defaultValue.contents.VEC.x, defaultValue.contents.VEC.y, defaultValue.contents.VEC.z)
            cmds.addAttr(mayaNodeName, sn=mayaAttributeName, longName=mayaAttributeLongName, at="float3", w=True, r=False, ct="oslAttribute")
            cmds.addAttr(mayaNodeName, sn=mayaAttributeName + 'X', longName=mayaAttributeLongName + 'X', at="float", parent=mayaAttributeLongName, dv=dv_x, w=True, r=False)
            cmds.addAttr(mayaNodeName, sn=mayaAttributeName + 'Y', longName=mayaAttributeLongName + 'Y', at="float", parent=mayaAttributeLongName, dv=dv_y, w=True, r=False)
            cmds.addAttr(mayaNodeName, sn=mayaAttributeName + 'Z', longName=mayaAttributeLongName + 'Z', at="float", parent=mayaAttributeLongName, dv=dv_z, w=True, r=False)
    elif arnoldAttributeType == AI_TYPE_VECTOR2:
        if not cmds.attributeQuery(mayaAttributeLongName, node=mayaNodeName, ex=True):
            dv_x, dv_y, dv_z = (defaultValue.contents.VEC.x, defaultValue.contents.VEC.y, defaultValue.contents.VEC.z)
            cmds.addAttr(mayaNodeName, sn=mayaAttributeName, longName=mayaAttributeLongName, at="float2", w=True, r=False, ct="oslAttribute")
            cmds.addAttr(mayaNodeName, sn=mayaAttributeName + 'U', longName=mayaAttributeLongName + 'U', at="float", parent=mayaAttributeLongName, w=True, r=False)
            cmds.addAttr(mayaNodeName, sn=mayaAttributeName + 'V', longName=mayaAttributeLongName + 'V', at="float", parent=mayaAttributeLongName, w=True, r=False)
    elif arnoldAttributeType == AI_TYPE_STRING:
        if not cmds.attributeQuery(mayaAttributeLongName, node=mayaNodeName, ex=True):
            cmds.addAttr(mayaNodeName, sn=mayaAttributeName, longName=mayaAttributeLongName, dt="string", w=True, r=False, ct="oslAttribute")
    elif arnoldAttributeType == AI_TYPE_ARRAY or arnoldAttributeType == AI_TYPE_POINTER or arnoldAttributeType == AI_TYPE_NODE or arnoldAttributeType == AI_TYPE_MATRIX:
        pass
    else:
        # Node is undefined
        pass


class OSLSceneModel():

    '''
        Creating a model, that contains the code , compiles it and creates a list of input and output attributes
        that can then be passed on to creating / modifying them.
    '''

    def __init__(self, shaderCode, mayaNodeName):
        self._shaderCode = shaderCode
        self._mayaNodeName = mayaNodeName
        self.compileState = False
        self.compilationErrors = ''
        self.inputAttributes = {}
        self.outputAttributes = {}
        self.__compileOSLCode__()
        if self.compileState:
            self.__modifyScene__()

    def __compileOSLCode__(self):

        '''
        Compile the OSL code in a separate Arnold Universe and create a list of input / output attribute
        We're not using the output attribute yet, as we dont have a way to get named output attributes from Arnold yet.
        '''

        isActive = AiUniverseIsActive()
        if not isActive:
            AiBegin()

        # create a universe dedicated to OSL node compilation
        # for parameter/output type introspection and error checking
        compilation_universe = AiUniverse()

        # create an osl node
        osl_node = AiNode(compilation_universe, "osl", "test_node")
        # set the osl code as the "code" node parameter value
        # this triggers the compilation and the morphing of the node
        # into a node with the correct node paramters and output
        AiNodeSetStr(osl_node, "code", self._shaderCode)

        # check for compilation errors
        # compilation errors are stored into a "compilation_errors" user parameter
        if AiNodeLookUpUserParameter(osl_node, "compilation_errors"):
            compilation_errors = AiNodeGetArray(osl_node, "compilation_errors")
        else:
            compilation_errors = None

        # if osl_node is not reused it is possible to only check if the parameter exists
        # otherwise if the node is reused to check multiple code snippets
        # the parameter will be reset
        if compilation_errors is None or AiArrayGetNumElements(compilation_errors) == 0:
            self.compileState = True
            # get the morphed node entry
            osl_node_entry = AiNodeGetNodeEntry(osl_node)
            self.outputAttributes['output'] = {}
            self.outputAttributes['output']['outputType'] = AiNodeEntryGetOutputType(osl_node_entry)
            it = AiNodeEntryGetParamIterator(osl_node_entry)
            while not AiParamIteratorFinished(it):
                pentry = AiParamIteratorGetNext(it)
                # OSL parameters start with "param_"
                if str(AiParamGetName(pentry)).startswith("param_"):
                    paramName = AiParamGetName(pentry)
                    self.inputAttributes[paramName] = {}
                    self.inputAttributes[paramName]['paramName'] = paramName
                    self.inputAttributes[paramName]['paramType'] = AiParamGetType(pentry)
                    self.inputAttributes[paramName]['paramDefaultValue'] = AiParamGetDefault(pentry)
        else:
            self.compileState = False
            for i in range(AiArrayGetNumElements(compilation_errors)):
                self.compilationErrors += AiArrayGetStr(compilation_errors, i) + "\n"
        # cleanup the node
        AiUniverseDestroy(compilation_universe)

        if not isActive:
            AiEnd()

    def __modifyScene__(self):

        '''
        Clean up old / unnecessary attributes and craete new ones in Maya
        '''

        if self.compileState:
            nodeAttrs = cmds.listAttr(self._mayaNodeName, userDefined=True, write=True, ct="oslAttribute", sn=True)
            if nodeAttrs:
                for attr in nodeAttrs:
                    if attr in self.inputAttributes.keys():
                        pass
                    else:
                        cmds.deleteAttr(self._mayaNodeName + '.' + attr)

            for key in self.inputAttributes.keys():
                createMayaAttributeForArnoldAttributes(self._mayaNodeName,  # The Maya node to which attributes need to be added
                                                       self.inputAttributes[key]['paramName'],  # The name of attribute
                                                       self.inputAttributes[key]['paramType'],  # The type of the attribute
                                                       self.inputAttributes[key]['paramDefaultValue']  # The default value
                                                       )


class AEaiOslShaderTemplate(ShaderAETemplate):

    def setup(self):
        self.compileEnum = {0: 'Needs (Re)Compile', 1: 'Compile Success', 2: 'Compile Failure'}
        self._controls = []
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
            cmds.addAttr(self.nodeName, ln="compileStatus", sn="cmpSts", at="enum", enumName="Needs Compile:Compile Success:Compile Failure", hidden=True)
        cmds.rowLayout(numberOfColumns=2, columnWidth2=(100, 100), adjustableColumn=1, columnAlign=(1, 'right'), columnAttach=[(1, 'both', 0), (2, 'both', 0)])
        cmds.text(" Compile Status", al="left", fn="smallBoldLabelFont")
        compileStatusText = self.compileEnum[cmds.getAttr(attrName)]
        self.compile_control = cmds.text(compileStatusText, al="right", fn="boldLabelFont")
        cmds.setParent("..")

    def codeStatusUpdate(self, attrName):
        if not cmds.attributeQuery("compileStatus", node=self.nodeName, ex=True):
            cmds.addAttr(self.nodeName, ln="compileStatus", sn="cmpSts", at="enum", enumName="Needs Compile:Compile Success:Compile Failure", hidden=True)

        compileStatus = cmds.getAttr(attrName)
        bg_color = [0.0, 0.0, 0.0]
        if compileStatus == 0:
            bg_color = [0.0, 1.0, 1.0]
        elif compileStatus == 1:
            bg_color = [0.0, 1.0, 0.0]
        else:
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
        self.code_widget_path = cmds.scrollField(editable=True, wordWrap=True,
                                                 font="fixedWidthFont",
                                                 w=200, h=400,
                                                 cc=lambda *args: self.setShaderCode(attrName))
        self.codeWidgetUpdate(attrName)

    def codeWidgetUpdate(self, attrName):
        cmds.scrollField(self.code_widget_path, edit=True, tx=cmds.getAttr(attrName), cc=lambda *args: self.setShaderCode(attrName))

    def compileButtonCreate(self, attrName):
        cmds.button('compileButtonPath', label='Compile OSL Code', command=lambda *args: self.compiler(attrName, self.code_widget_path))

    def compileButonUpdate(self, attrName):
        cmds.button('compileButtonPath', e=True, label='Compile OSL Code', command=lambda *args: self.compiler(attrName, self.code_widget_path))

    def compiler(self, attrName, path):
        nodeName = attrName.split('.')[0]
        compileText = cmds.scrollField(path, q=True, tx=True)

        if compileText != cmds.getAttr(nodeName + '.code'):
            self.setShaderCode(nodeName + '.code')

        oslMayaScene = OSLSceneModel(compileText, nodeName)
        if oslMayaScene.compileState:
            self.setCodeStatus(attrName, 1)
            om.MGlobal.displayInfo(" Code compiled successfully ")
        else:
            self.setCodeStatus(attrName, 2)
            om.MGlobal.displayError(" Compilation Failed")
            cmds.error(oslMayaScene.compilationErrors)

    def setShaderCode(self, attrName):
        nodeName = attrName.split('.')[0]
        compileText = cmds.scrollField(self.code_widget_path, q=True, tx=True)
        if compileText == cmds.getAttr(attrName):
            return
        cmds.setAttr(attrName, compileText, type="string")
        self.setCodeStatus(attrName, 0)

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
        defaultDir = cmds.workspace(query=True, directory=True)
        ret = cmds.fileDialog2(fileFilter=basicFilter, cap='Impot OSL', okc='Load', fm=1, startingDirectory=defaultDir)
        if ret is not None and len(ret):
            code_string = ''
            with open(ret[0], 'r') as myfile:
                code_string = myfile.read()
            codeAttr = attrName.split('.')[0] + '.code'
            if code_string != cmds.getAttr(codeAttr):
                cmds.setAttr(codeAttr, code_string, type="string")
                cmds.scrollField(self.code_widget_path, edit=True, tx=code_string)
                self.setCodeStatus(attrName, 1)

    def exportOSL(self, attrName):
        basicFilter = 'OSL Files (*.osl)'
        defaultDir = cmds.workspace(query=True, directory=True)
        ret = cmds.fileDialog2(fileFilter=basicFilter, cap='Export OSL', dialogStyle=2, startingDirectory=defaultDir)
        if ret is not None and len(ret):
            codeAttr = attrName.split('.')[0] + '.code'
            code_string = cmds.getAttr(codeAttr)
            with open(ret[0], 'w') as myfile:
                myfile.write(code_string)

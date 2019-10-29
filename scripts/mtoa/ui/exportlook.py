
import maya.cmds as cmds
import mtoa.utils as utils

OPERATOR_FILETYPES = {".ass": "Arnold Operator Graph",
                      ".mtlx": "MaterialX Look file"}

def setOperatorOptions(options):

    for option, value in options.items():
        if type(value) in [bool, int]:
            cmds.optionVar(intValue=("arnold_{}".format(option), value))
        elif type(value) in [str, unicode]:
            cmds.optionVar(stringValue=("arnold_{}".format(option), value))

def getOperatorOptions(mergeOptions=False):
    options = {}
    options['defaultLookExt'] = '.ass'
    options['exportShaders'] = True

    options['relativeAssignments'] = True
    options['exportFullPath'] = True
    options['exportSeparator'] = '/'

    options['assReplaceNetwork'] = False
    options['mtlxReplaceNetwork'] = False

    for option, default in options.items():
        if not cmds.optionVar(exists="arnold_{}".format(option)):
            if type(default) in [bool, int]:
                cmds.optionVar(intValue=("arnold_{}".format(option), default))
            elif type(default) in [str, unicode]:
                cmds.optionVar(stringValue=("arnold_{}".format(option), default))
        else:
            options[option] = cmds.optionVar(query="arnold_{}".format(option))

    if mergeOptions:
        # get default settings from options node
        optionsNode = 'defaultArnoldRenderOptions'
        if cmds.ls(optionsNode):
            options.setdefault('exportFullPath', cmds.getAttr('%s.exportFullPaths' % optionsNode))
            optionsSeperator = '/' if cmds.getAttr('%s.exportFullPaths' % optionsNode) == 1 else '|'
            options.setdefault('exportSeparator', optionsSeperator)

    return options

# create th UI controls
def arnoldOpExportUI_Create(parent):

    cmds.setParent(parent)

    cmds.setUITemplate("DefaultTemplate", pushTemplate=True)

    #  Turn on the wait cursor
    cmds.waitCursor(state=1)

    # Create option box or file options contents.

    optionLayout = cmds.scrollLayout(childResizable=True)
    cmds.columnLayout(adjustableColumn=True)

    cmds.frameLayout("arn_operatorOptions", label="Operator Options", collapse=False, visible=False)
    cmds.columnLayout()

    cmds.checkBoxGrp("expShadersCB",
                     numberOfCheckBoxes=1,
                     label1="Export Shaders")
    cmds.checkBoxGrp("assReplaceNetworkCB",
                     numberOfCheckBoxes=1,
                     label1="Replace Network")

    cmds.setParent("..")
    cmds.setParent("..")

    cmds.frameLayout("arn_materialXOptions", label="MaterialX Options", collapse=False, visible=False)
    cmds.columnLayout()

    cmds.checkBoxGrp("relativeAssignmentsCB",
                     numberOfCheckBoxes=1,
                     label1="Relative Assignmnets")
    cmds.checkBoxGrp("exportFullPathCB",
                     numberOfCheckBoxes=1,
                     label1="Export Full Path")

    cmds.optionMenuGrp("exportSeparatorCB",
                       label="Export Seperator")
    cmds.menuItem("/")
    cmds.menuItem("|")
    cmds.setParent("..")

    cmds.checkBoxGrp("mtlxReplaceNetworkCB",
                     numberOfCheckBoxes=1,
                     label1="Replace Network")

    cmds.setParent("..")
    cmds.setParent("..")

    #  Turn off the wait cursor.
    #
    cmds.waitCursor(state=0)

    # Deactivate the default UI template
    #
    cmds.setUITemplate(popTemplate=True)

    return optionLayout

# get the option vars and set the controls
def arnoldOpExportUI_Init(parent, fileFilter):
    # get option vars
    options = getOperatorOptions(True)
    # set the controls to the option var values
    cmds.setParent(parent)

    cmds.checkBoxGrp("expShadersCB", edit=True, value1=options['exportShaders'])

    cmds.checkBoxGrp("relativeAssignmentsCB", edit=True, value1=options['relativeAssignments'])
    cmds.checkBoxGrp("exportFullPathCB", edit=True, value1=options['exportFullPath'])

    cmds.optionMenuGrp("exportSeparatorCB", edit=True, value=options['exportSeparator'])

    cmds.checkBoxGrp("assReplaceNetworkCB", edit=True, value1=options['assReplaceNetwork'])
    cmds.checkBoxGrp("mtlxReplaceNetworkCB", edit=True, value1=options['mtlxReplaceNetwork'])

    arnoldOpExportUI_Change(parent, OPERATOR_FILETYPES[options['defaultLookExt']])

# set the correct UI for the current file type
def arnoldOpExportUI_Change(parent, newType):
    cmds.setParent(parent)

    if newType == OPERATOR_FILETYPES['.ass']:
        # show operator export
        cmds.frameLayout("arn_operatorOptions", edit=True, visible=True)
        cmds.frameLayout("arn_materialXOptions", edit=True, visible=False)
    elif newType == OPERATOR_FILETYPES['.mtlx']:
        # show materialx export
        cmds.frameLayout("arn_operatorOptions", edit=True, visible=False)
        cmds.frameLayout("arn_materialXOptions", edit=True, visible=True)


# set the option vars for the command
def arnoldOpExportUI_Commit(parent):
    # get option vars
    cmds.setParent(parent)

    options = {}

    if cmds.frameLayout("arn_operatorOptions", query=True, visible=True):
        options['exportShaders'] = cmds.checkBoxGrp("expShadersCB", query=True, value1=True)
        options['assReplaceNetwork'] = cmds.checkBoxGrp("assReplaceNetworkCB", query=True, value1=True)
        options['defaultLookExt'] = '.ass'
    elif cmds.frameLayout("arn_materialXOptions", query=True, visible=True):
        options['relativeAssignments'] = cmds.checkBoxGrp("relativeAssignmentsCB", query=True, value1=True)
        options['exportFullPath'] = cmds.checkBoxGrp("exportFullPathCB", query=True, value1=True)
        options['exportSeparator'] = cmds.optionMenuGrp("exportSeparatorCB", query=True, value=True)
        options['mtlxReplaceNetwork'] = cmds.checkBoxGrp("mtlxReplaceNetworkCB", query=True, value1=True)
        options['defaultLookExt'] = '.mtlx'

    setOperatorOptions(options)


utils.pyToMelProc(arnoldOpExportUI_Create,
                  [('string', 'parent')],
                   useName=True)

utils.pyToMelProc(arnoldOpExportUI_Init,
                  [('string', 'parent'), ('string', 'fileFilter')],
                   useName=True)

utils.pyToMelProc(arnoldOpExportUI_Change,
                  [('string', 'parent'), ('string', 'newType')],
                   useName=True)

utils.pyToMelProc(arnoldOpExportUI_Commit,
                  [('string', 'parent')],
                   useName=True)

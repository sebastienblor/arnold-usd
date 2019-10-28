
import maya.cmds as cmds


def getOperatorAssOptions():
    options = {}

    # exportShaders
    # replaceNetwork

def getOperatorMtlXOptions():
    options = {}

    # relativeAssignments
    # exportFullPath
    # exportSeparator
    # replaceNetwork

# create th UI controls
def arnoldOpExportUI_Create(parent):

    cmds.setParent(parent)

    cmds.setUITemplate(pushTemplate="DefaultTemplate")

    #  Turn on the wait cursor
    cmds.waitCursor(state=1)

    # Create option box or file options contents.

    optionLayout = cmds.scrollLayout(childResizable=True)
    cmds.columnLayout(adjustableColumn=True)

    #  Turn off the wait cursor.
    #
    cmds.waitCursor(state=0)

    # Deactivate the default UI template
    #
    cmds.setUITemplate(popTemplate=True)

    return optionLayout

# get the option vars and set the controls
def arnoldOpExportUI_Init(parent, fileFilter):
    pass
    # get option vars

    # set the controls to the option var values

# set the option vars for the command
def arnoldOpExportUI_Commit(parent, fileFilter):
    pass

import maya.cmds as cmds
import mtoa.utils
import os.path
import glob
import re
import sys, os
import platform
import subprocess
import threading
import mtoa.callbacks as callbacks
import arnold.ai_license
import time

global _waitingForLicenseStatus
_waitingForLicenseStatus = False

global _waitingForDiagnosticsStatus
_waitingForDiagnosticsStatus = False

def getDiagnosticsResult():

    diagnosticResults = 'MtoA License Diagnostics '
    diagnosticResults += cmds.date()
    diagnosticResults += "\n"

    if platform.system().lower() == 'windows':
        _no_window = subprocess.STARTUPINFO()
        _no_window.dwFlags |= subprocess.STARTF_USESHOWWINDOW
    else:
        _no_window = None

    if arnold.AiUniverseIsActive() or cmds.arnoldScene(query=True):
        diagnosticsResult += "COULDN'T START A TEST RENDER AS ANOTHER ARNOLD SESSION IS IN PROGRESS.\n "
    else:
        logPath = cmds.workspace(directory=True, query=True)
        logPath = os.path.join(logPath, "arnoldDiagnostics.log")

        cmds.arnoldScene(mode="create")
        arnold.AiMsgSetLogFileName(logPath)
        arnold.AiMsgSetLogFileFlags(arnold.AI_LOG_ERRORS | arnold.AI_LOG_WARNINGS | arnold.AI_LOG_INFO)
        arnold.AiMsgResetCallback()
        arnold.AiRender(arnold.AI_RENDER_MODE_FREE)
        cmds.arnoldScene(mode="destroy")

        if os.path.exists(logPath):
            with open(logPath, 'r') as content_file:
                diagnosticResults += content_file.read()
        else:
            diagnosticResults += "RENDER LOG FILE NOT FOUND\n"

    global _waitingForDiagnosticsStatus
    _waitingForDiagnosticsStatus = True
    cmds.arnoldLicense(runServerStatus=True)
    diagnosticsCmds = ["kick -licensecheck", "lmutil lmstat -S adskflex -i", "rlmutil rlmhostid -q ether"]
    i = 0
    for diagnosticCmd in diagnosticsCmds:

        diagnosticResults += "-------------------------------------------------\n"

        if i == 0:
            mtoa.utils.setEnvironmentVariable(u'RLM_DEBUG', u'arnold')

        cmdBinary = os.path.join(os.path.dirname(os.path.dirname(os.path.dirname(__file__))), 'bin', diagnosticCmd)
        timerStart = time.clock()
        cmdRes = subprocess.Popen(cmdBinary, stdin=subprocess.PIPE, stdout=subprocess.PIPE, stderr=subprocess.PIPE, startupinfo=_no_window).communicate()[0]
        
        timerLength = time.clock() - timerStart


        diagnosticResults += "%s (%d sec) \n" % (diagnosticCmd, timerLength)
        diagnosticResults += "==============================\n"
        diagnosticResults += cmdRes


        if i == 0:
            if sys.platform == 'win32':    
                mtoa.utils.setEnvironmentVariable(u'RLM_DEBUG', u'')
            else:
                del os.environ['RLM_DEBUG']

        i = i +1

    return diagnosticResults

def updateStatus(win, textRLM, rlmStatusValue, textNLM, nlmStatusValue):

        if rlmStatusValue == arnold.AI_LIC_SUCCESS or rlmStatusValue == arnold.AI_LIC_ERROR_NOTAVAILABLE:
            cmds.text(win.rlmStatus, edit=True, label=textRLM, backgroundColor=[0,1,0])
        elif rlmStatusValue == arnold.AI_LIC_ERROR_CANTCONNECT or rlmStatusValue == arnold.AI_LIC_ERROR_INIT:
            cmds.text(win.rlmStatus, edit=True, label=textRLM, backgroundColor=[1,0,0])
        else:
            cmds.text(win.rlmStatus, edit=True, label=textRLM, enableBackground=False)

        if nlmStatusValue == arnold.AI_LIC_SUCCESS or nlmStatusValue == arnold.AI_LIC_ERROR_NOTAVAILABLE:
            cmds.text(win.nlmStatus, edit=True, label=textNLM, backgroundColor=[0,1,0])
        elif nlmStatusValue == arnold.AI_LIC_ERROR_CANTCONNECT or nlmStatusValue == arnold.AI_LIC_ERROR_INIT:
            cmds.text(win.nlmStatus, edit=True, label=textNLM, backgroundColor=[1,0,0])
        else:
            cmds.text(win.nlmStatus, edit=True, label=textNLM, enableBackground=False)

#class GetDiagnosticsThread (threading.Thread):
#    def __init__(self,win):
#        self.diagnosticsWindow = win
#        threading.Thread.__init__(self)
#    def run (self):
#        diagnosticsResult = getDiagnosticsResult()
#        cmds.scrollField(self.diagnosticsWindow.diagnosticsEdit, edit=True, text=diagnosticsResult)

class ConnectToLicenseServer(object):
    window = None
    def __new__(cls, *args, **kwargs):
        if not '_instance' in vars(cls):
            cls._instance = super(ConnectToLicenseServer, cls).__new__(cls, *args, **kwargs)
        return cls._instance

    def __init__(self):
        if self.window is None:
            self.window = 'MtoAConnectToLicenseServer'
            self.rlmServerEdit = ''
            self.nlmServerEdit = ''

    def doCancel(self):
        cmds.deleteUI(self.window)
        return True

    
    def create(self):
        if cmds.window(self.window, exists=True):
            cmds.deleteUI(self.window)

        winTitle = "Connect To License Server"

        self.window = cmds.window(self.window, sizeable=False,widthHeight=(380, 120), title=winTitle)
        self.createUI()

        cmds.setParent(menu=True)
        cmds.showWindow(self.window)

        try:
            initPos = cmds.windowPref( self.window, query=True, topLeftCorner=True )
            if initPos[0] < 0:
                initPos[0] = 0
            if initPos[1] < 0:
                initPos[1] = 0
            cmds.windowPref( self.window, edit=True, topLeftCorner=initPos )
        except :
            pass

    def doConnectRLM(self):        
        rlmServer = cmds.textField(self.rlmServerEdit,  query=True, text=True)
        cmds.arnoldLicense(setRlmServer=rlmServer)

        cmds.text(self.rlmStatus, edit=True, label="Checking Status...", backgroundColor=[1,1,0])
        cmds.text(self.nlmStatus, edit=True, label="", enableBackground=False)
        cmds.arnoldLicense(runServerStatus=True)

    def doConnectNLM(self):
        nlmServer = cmds.textField(self.nlmServerEdit,  query=True, text=True)
        cmds.arnoldLicense(setNlmServer=nlmServer)
        cmds.text(self.rlmStatus, edit=True, label="Checking Status...", backgroundColor=[1,1,0])
        cmds.text(self.nlmStatus, edit=True, label="", enableBackground=False)
        cmds.arnoldLicense(runServerStatus=True)
        
    def createUI(self):
        cmds.scrollLayout(childResizable=True)
        cmds.columnLayout()

        cmds.rowColumnLayout( numberOfColumns=3, columnWidth=[(1,100),(2,190),(3,80)] )
        cmds.text(align="left", label="Solid Angle (RLM)")
        self.rlmServerEdit = cmds.textField()

        rlmServerLocalEnv = os.getenv('solidangle_LICENSE')
        if rlmServerLocalEnv is None:
            rlmServerLocalEnv = ""
        
        rlmServerEnv = cmds.arnoldLicense(getRlmServer=True)
        if rlmServerLocalEnv != rlmServerEnv:
            cmds.warning("The RLM local and global environment does not match")

        nlmServerLocalEnv = os.getenv('ADSKFLEX_LICENSE_FILE')
        if nlmServerLocalEnv is None:
            nlmServerLocalEnv = ""
        
        nlmServerEnv = cmds.arnoldLicense(getNlmServer=True)
        if nlmServerLocalEnv != nlmServerEnv:
            cmds.warning("The NLM local and global environment does not match")

        if rlmServerEnv == "" and nlmServerEnv == "":
            rlmServerEnv = 'localhost'
            cmds.arnoldLicense(setRlmServer=rlmServerEnv)

        cmds.textField(self.rlmServerEdit,  edit=True, text=rlmServerEnv, editable=True)

        cmds.button(align="right", label='Connect', command=lambda *args: self.doConnectRLM())
        cmds.setParent( '..' )
        cmds.rowColumnLayout( numberOfColumns=3, columnWidth=[(1,100),(2,190),(3,80)] )
        cmds.text(align="left", label="Autodesk (NLM)")
        self.nlmServerEdit = cmds.textField()

        cmds.textField(self.nlmServerEdit,  edit=True, text=nlmServerEnv, editable=True)
        cmds.button(align="right", label='Connect', command=lambda *args: self.doConnectNLM())
        cmds.setParent( '..' )

        self.rlmStatus = cmds.text(align="left", label="Checking Status...",  backgroundColor=[1,1,0])
        self.nlmStatus = cmds.text(align="left", label="")
        cmds.separator()
        cmds.rowColumnLayout( numberOfColumns=3, columnWidth=[(1,120),(2,170),(3,80)] )
        cmds.button( align="left", label='Get Diagnostics...', command=('import mtoa.licensing;win = mtoa.licensing.GetDiagnostics();win.create()'))
        #cmds.text(label="")
        cmds.text(label="")
        cmds.button(align="right", label='Close', command=('import maya.cmds as cmds;cmds.deleteUI(\"' + self.window + '\", window=True)'))
        cmds.setParent( '..' )
        
        cmds.waitCursor(state=True)
        global _waitingForLicenseStatus
        _waitingForLicenseStatus = True
        cmds.arnoldLicense(runServerStatus=True)
        


def nlmStatus():
    if platform.system().lower() == 'windows':
        _no_window = subprocess.STARTUPINFO()
        _no_window.dwFlags |= subprocess.STARTF_USESHOWWINDOW
    else:
        _no_window = None

    lmutil_binary = os.path.join(os.path.dirname(os.path.dirname(os.path.dirname(__file__))), 'bin', 'lmutil')
    cmd = lmutil_binary + ' lmstat -S adskflex -a'
    res = subprocess.Popen(cmd, stdin=subprocess.PIPE, stdout=subprocess.PIPE, stderr=subprocess.PIPE, startupinfo=_no_window).communicate()[0]
    return res


def getStatusMessage(licenseType, status, licenseCount, licenseInUse):

    statusMsg = ''

    if status == arnold.AI_LIC_SUCCESS or status == arnold.AI_LIC_ERROR_NOTAVAILABLE:
        statusMsg = "Found %d %s license%s (%d in use)" % (licenseCount, licenseType, "s" if licenseCount > 1 else "", licenseInUse)
        
    elif status == arnold.AI_LIC_ERROR_CANTCONNECT:
        statusMsg = "Cannot connect to the %s license server" % licenseType
    elif status == arnold.AI_LIC_ERROR_INIT:
        statusMsg = "Error initializing the %s license system" % licenseType
    elif status == arnold.AI_LIC_ERROR_NOTFOUND:
        statusMsg = "No %s license found" % licenseType
    else:
        statusMsg = "Unkown %s license error" % licenseType

    return statusMsg


###########################################################
### GET DIAGNOSTICS
##########################################################

class GetDiagnostics(object):
    window = None
    def __new__(cls, *args, **kwargs):
        if not '_instance' in vars(cls):
            cls._instance = super(GetDiagnostics, cls).__new__(cls, *args, **kwargs)
        return cls._instance

    def __init__(self):
        if self.window is None:
            self.window = 'MtoAGetDiagnostics'


    def doCancel(self):
        cmds.deleteUI(self.window)
        return True

    
    def create(self):
        if cmds.window(self.window, exists=True):
            cmds.deleteUI(self.window)

        winTitle = "Get Diagnostics"

        self.window = cmds.window(self.window, sizeable=False, widthHeight=(600, 600), title=winTitle)
        self.createUI()

        cmds.setParent(menu=True)
        cmds.showWindow(self.window)

        try:
            initPos = cmds.windowPref( self.window, query=True, topLeftCorner=True )
            if initPos[0] < 0:
                initPos[0] = 0
            if initPos[1] < 0:
                initPos[1] = 0
            cmds.windowPref( self.window, edit=True, topLeftCorner=initPos )
        except :
            pass

    global _defaultDiagnosticsFolder
    _defaultDiagnosticsFolder = ''

    def saveDiagnosticsToFile(self):
        diagnosticsText = cmds.scrollField(self.diagnosticsEdit, query=True, text=True)

        global _defaultDiagnosticsFolder
        if _defaultDiagnosticsFolder == "":
            _defaultDiagnosticsFolder = cmds.workspace(q=True,rd=True, fn=True)

        filename = cmds.fileDialog2(cap='Save Diagnostics to File', fm=0, dir=_defaultDiagnosticsFolder)

        if filename is not None and len(filename):
            with open(filename[0], "w") as text_file:
                text_file.write(diagnosticsText)

    def copyDiagnosticsToClipboard(self):
        diagnosticsText = cmds.scrollField(self.diagnosticsEdit, query=True, text=True)
        cmds.arnoldLicense(copyToClipboard=diagnosticsText)


    def createUI(self):
        cmds.scrollLayout(childResizable=True)
        cmds.columnLayout()

        self.rlmStatus = cmds.text(align="left", label="RLM Status")
        self.nlmStatus = cmds.text(align="left", label="RLM Status")

        self.diagnosticsEdit = cmds.scrollField(width=520, height=500)
        
        cmds.separator()
        cmds.rowColumnLayout( numberOfColumns=6, columnWidth=[(1,80),(2,10),(3,80),(4,260),(5,80)] )
        cmds.button( align="left", label='Copy', command=lambda *args: self.copyDiagnosticsToClipboard())
        cmds.text(label="")
        cmds.button( align="left", label='Save to File', command=lambda *args: self.saveDiagnosticsToFile())
        cmds.text(label="")
        cmds.button(align="right", label='Close', command=('import maya.cmds as cmds;cmds.deleteUI(\"' + self.window + '\", window=True)'))
        cmds.setParent( '..' )
    
        cmds.waitCursor(state=True)
        
        cmds.scrollField(self.diagnosticsEdit, edit=True, text=getDiagnosticsResult())
        cmds.waitCursor(state=False)


def returnServerStatus(rlmStatus, rlmLicensesCount, rlmLicensesInUse, nlmStatus, nlmLicensesCount, nlmLicensesInUse):
    textRLM = getStatusMessage("RLM", rlmStatus, rlmLicensesCount, rlmLicensesInUse)
    textNLM = getStatusMessage("NLM", nlmStatus, nlmLicensesCount, nlmLicensesInUse)
    global _waitingForDiagnosticsStatus
    global _waitingForLicenseStatus

    if _waitingForDiagnosticsStatus:
        updateStatus(GetDiagnostics(), textRLM, rlmStatus, textNLM, nlmStatus)
        _waitingForDiagnosticsStatus = False
        
    if _waitingForLicenseStatus:
        updateStatus(ConnectToLicenseServer(), textRLM, rlmStatus, textNLM, nlmStatus)
        _waitingForLicenseStatus = False
        cmds.waitCursor(state=False)
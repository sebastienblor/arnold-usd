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
from uuid import getnode as get_mac
import shutil

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

    ps_env = os.environ.copy()
    ps_env['FLEXLM_BATCH'] = '1'
    ps_env['FLEXLM_DIAGNOSTICS'] = '3'
    ps_env['ADCLMHUB_LOG_LEVEL'] = 'T'
    ps_env['RLM_DEBUG'] = 'arnold'

    logPath = cmds.workspace(directory=True, query=True)
    logPath = os.path.join(logPath, "arnoldDiagnostics.log")

    null_path = 'NUL' if sys.platform == 'win32' else '/dev/null'
    cmd = "kick -nostdin -i {}".format(null_path)
    cmdBinary = os.path.join(os.path.dirname(os.path.dirname(os.path.dirname(__file__))), 'bin', cmd)
    timerStart = time.clock()
    cmdRes = subprocess.Popen(cmdBinary.split(), stdin=subprocess.PIPE, stdout=subprocess.PIPE, stderr=subprocess.PIPE, startupinfo=_no_window, env=ps_env).communicate()[0]
    timerLength = time.clock() - timerStart

    diagnosticResults += "%s (%d sec) \n" % (cmd, timerLength)
    diagnosticResults += "==============================\n"
    diagnosticResults += cmdRes

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
        cmdRes = subprocess.Popen(cmdBinary.split(), stdin=subprocess.PIPE, stdout=subprocess.PIPE, stderr=subprocess.PIPE, startupinfo=_no_window).communicate()[0]
        
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

        ret = cmds.confirmDialog(title='Connect to License Server', message='This will set an environment variable in your system', button=['Ok', 'Cancel'], defaultButton='Ok', cancelButton='Cancel' )
        if ret != 'Ok':
            return

        rlmServer = cmds.textField(self.rlmServerEdit,  query=True, text=True)
        if rlmServer == "":
            rlmServer = 'localhost'
            cmds.textField(self.rlmServerEdit,  edit=True, text=rlmServer, editable=True)
            
        
        cmds.arnoldLicense(setRlmServer=rlmServer)
        cmds.text(self.rlmStatus, edit=True, label="Checking Status...", backgroundColor=[1,1,0])
        cmds.text(self.nlmStatus, edit=True, label="", enableBackground=False)
        cmds.arnoldLicense(runServerStatus=True)

    def doConnectNLM(self):
        ret = cmds.confirmDialog(title='Connect to License Server', message='This will set an environment variable in your system', button=['Ok', 'Cancel'], defaultButton='Ok', cancelButton='Cancel' )
        if ret != 'Ok':
            return
        
        nlmServer = cmds.textField(self.nlmServerEdit,  query=True, text=True)
        if nlmServer == "":
            nlmServer = 'localhost'
            cmds.textField(self.nlmServerEdit,  edit=True, text=nlmServer, editable=True)

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

        '''
        we prefer not to set the env variables under the hood
        if rlmServerEnv == "" and nlmServerEnv == "":
            rlmServerEnv = 'localhost'
            cmds.arnoldLicense(setRlmServer=rlmServerEnv)
        '''

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
    res = subprocess.Popen(cmd.split(), stdin=subprocess.PIPE, stdout=subprocess.PIPE, stderr=subprocess.PIPE, startupinfo=_no_window).communicate()[0]
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

        self.window = cmds.window(self.window, sizeable=False, widthHeight=(540, 600), title=winTitle)
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


###########################################
### INSTALL NODE-LOCKED
###########################################

class NodeLocked(object):
    window = None
    def __new__(cls, *args, **kwargs):
        if not '_instance' in vars(cls):
            cls._instance = super(NodeLocked, cls).__new__(cls, *args, **kwargs)
        return cls._instance

    def __init__(self):
        if self.window is None:
            self.window = 'MtoANodeLocked'


    def doCancel(self):
        cmds.deleteUI(self.window)
        return True

    def create(self):
        if cmds.window(self.window, exists=True):
            cmds.deleteUI(self.window)

        winTitle = "Arnold Node-locked License"

        self.window = cmds.window(self.window, sizeable=False, widthHeight=(630, 280), title=winTitle)
        self.createUI()

        cmds.setParent(menu=True)
        cmds.showWindow(self.window)


    def dotDotDotButtonPush(self, file):
        licenseFilter = 'License Files (*.lic)'
        ret = cmds.fileDialog2(fileFilter=licenseFilter, 
                                cap='Load License File',okc='Load',fm=1)
        if ret is not None and len(ret):
            cmds.textField(file, edit=True, text=ret[0])

    def installButtonPush(self, file):
        licenseFile = cmds.textField(file, query=True, text=True)
        import maya.mel as mel
        mPath = mel.eval('getenv "MTOA_PATH"')
        destination = os.path.join(mPath,'bin')
        try:
            shutil.copy(licenseFile,destination)
            cmds.confirmDialog(title='Success', message='License Successfully Installed', button=['Ok'], defaultButton='Ok' )
        except:
            cmds.arnoldCopyAsAdmin(f=licenseFile,o=destination)


    def createUI(self):
        cmds.scrollLayout(childResizable=True)

        cmds.columnLayout()
        #cmds.rowColumnLayout( numberOfColumns=2, columnWidth=[(1,10), (2, 412)] )

        #cmds.text(label="");cmds.text(label="");

        arnoldAboutText =  u"A node-locked license allows you to render with Arnold on one computer only.\n"

        cmds.text(label="")
        cmds.text(align="center",label=arnoldAboutText)
        arnoldAboutText =  u"Note that monthly and annual subscription licenses are floating licenses, not node-locked. They require a license server\n"
        cmds.text(align="left",label=arnoldAboutText)
        cmds.text(label="")
        cmds.separator()

        cmds.setParent( '..' )
        cmds.separator()

        cmds.rowColumnLayout( numberOfColumns=2, columnWidth=[(1,10), (2, 412)] )
        macText =  u"To issue a node-locked license, we need the MAC address of your computer.\n"
        cmds.text(label="")
        cmds.text(align="left",label=macText)
        cmds.setParent( '..' )
        cmds.separator()

        cmds.rowColumnLayout( numberOfColumns=6, columnWidth=[(1,10),(2,90), (3, 190),(4,40),(5,80),(6,12)] )
        cmds.text(label="")
        cmds.text(align="left",label="MAC Address")
        name = cmds.textField()
        mac = get_mac()
        mactext = ("%012X" % mac)
        cmds.textField(name,  edit=True, text=mactext, editable=False )
        cmds.text(label="")
        cmds.text(label="")
        cmds.text(label="")

        cmds.text(label="")
        cmds.text(label="")
        cmds.text(label="")
        cmds.text(label="")
        cmds.text(label="")
        cmds.text(label="")


        cmds.setParent( '..' )

        cmds.separator()
        
        cmds.rowColumnLayout( numberOfColumns=2, columnWidth=[(1,10), (2, 412)] )
        macText =  u"To install your node-locked license, locate the license file (.lic) and click Install.\n"
        cmds.text(label="")
        cmds.text(align="left",label=macText)
        cmds.setParent( '..' )

        cmds.rowColumnLayout( numberOfColumns=8, columnWidth=[(1,10),(2,90),(3,390),(4,7),(5,26),(6,7),(7,80),(8,12)] )
        cmds.text(label="")
        cmds.text(align="left",label="License file (.lic)")
        file = cmds.textField()
        cmds.text(label="")
        cmds.button( label='...', command=lambda arg=None,x=file: self.dotDotDotButtonPush(x) )
        cmds.text(label="")
        cmds.button( label='Install', command=lambda arg=None,x=file: self.installButtonPush(x) )
        cmds.text(label="")

        cmds.text(label="")
        cmds.text(label="")
        cmds.text(label="")
        cmds.text(label="")
        cmds.text(label="")
        cmds.text(label="")
        cmds.text(label="")
        cmds.text(label="")

        cmds.text(label="")
        cmds.text(label="")
        cmds.text(label="")
        cmds.text(label="")
        cmds.text(label="")
        cmds.text(label="")
        cmds.text(label="")
        cmds.text(label="")

        cmds.setParent( '..' )

        cmds.rowColumnLayout( numberOfColumns=5, columnWidth=[(1,80),(2,360), (3, 80),(4,20),(5,80)] )
        cmds.text(label="")
        cmds.text(label="")
        cmds.button( label='Close', command=('import maya.cmds as cmds;cmds.deleteUI(\"' + self.window + '\", window=True)'))
        cmds.text(label="")
        cmds.button( label='Help', c=lambda *args: cmds.launch(webPage='https://www.solidangle.com/support/licensing/'))

        cmds.setParent( '..' )

        
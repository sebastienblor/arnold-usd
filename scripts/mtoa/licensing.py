import maya.cmds as cmds
import maya.utils as utils
import os.path
import glob
import re
import sys, os
import platform
import subprocess
import threading
import mtoa.callbacks as callbacks
import arnold.ai_license

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

        self.window = cmds.window(self.window, widthHeight=(380, 120), title=winTitle)
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
        
    def updateStatus(self, textRLM, rlmStatusValue, textNLM, nlmStatusValue):

        if rlmStatusValue == arnold.ai_license.AI_LIC_SUCCESS or rlmStatusValue == arnold.ai_license.AI_LIC_ERROR_NOTAVAILABLE:
            cmds.text(self.rlmStatus, edit=True, label=textRLM, backgroundColor=[0,1,0])
        elif rlmStatusValue == arnold.ai_license.AI_LIC_ERROR_CANTCONNECT or rlmStatusValue == arnold.ai_license.AI_LIC_ERROR_INIT:
            cmds.text(self.rlmStatus, edit=True, label=textRLM, backgroundColor=[1,0,0])
        else:
            cmds.text(self.rlmStatus, edit=True, label=textRLM, enableBackground=False)

        if nlmStatusValue == arnold.ai_license.AI_LIC_SUCCESS or nlmStatusValue == arnold.ai_license.AI_LIC_ERROR_NOTAVAILABLE:
            cmds.text(self.nlmStatus, edit=True, label=textNLM, backgroundColor=[0,1,0])
        elif nlmStatusValue == arnold.ai_license.AI_LIC_ERROR_CANTCONNECT or nlmStatusValue == arnold.ai_license.AI_LIC_ERROR_INIT:
            cmds.text(self.nlmStatus, edit=True, label=textNLM, backgroundColor=[1,0,0])
        else:
            cmds.text(self.nlmStatus, edit=True, label=textNLM, enableBackground=False)



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
        #cmds.button( align="left", label='Get Diagnostics...', command=(''))
        cmds.text(label="")
        cmds.text(label="")
        cmds.button(align="right", label='Close', command=('import maya.cmds as cmds;cmds.deleteUI(\"' + self.window + '\", window=True)'))
        cmds.setParent( '..' )
        
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

    if status == arnold.ai_license.AI_LIC_SUCCESS or status == arnold.ai_license.AI_LIC_ERROR_NOTAVAILABLE:
        statusMsg = "Found %d %s license%s (%d in use)" % (licenseCount, licenseType, "s" if licenseCount > 1 else "", licenseInUse)
        
    elif status == arnold.ai_license.AI_LIC_ERROR_CANTCONNECT:
        statusMsg = "Cannot connect to the %s license server" % licenseType
    elif status == arnold.ai_license.AI_LIC_ERROR_INIT:
        statusMsg = "Error initializing the %s license system" % licenseType
    elif status == arnold.ai_license.AI_LIC_ERROR_NOTFOUND:
        statusMsg = "No %s license found" % licenseType
    else:
        statusMsg = "Unkown %s license error" % licenseType

    return statusMsg


def returnServerStatus(rlmStatus, rlmLicensesCount, rlmLicensesInUse, nlmStatus, nlmLicensesCount, nlmLicensesInUse):
    
    textRLM = getStatusMessage("RLM", rlmStatus, rlmLicensesCount, rlmLicensesInUse)
    textNLM = getStatusMessage("NLM", nlmStatus, nlmLicensesCount, nlmLicensesInUse)
    
    licServer = ConnectToLicenseServer()
    licServer.updateStatus(textRLM, rlmStatus, textNLM, nlmStatus)


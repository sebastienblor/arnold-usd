import maya.cmds as cmds
import mtoa.utils
import os.path
import glob
import re
import sys, os
import platform
import time
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

def licenseManager():
    cmdBinary = os.path.join(os.path.dirname(os.path.dirname(os.path.dirname(__file__))), 'bin', 'ArnoldLicenseManager')
    platformName = sys.platform
    if platformName.startswith('darwin'):
        cmdBinary = os.path.join(os.path.dirname(os.path.dirname(os.path.dirname(__file__))), 'bin', 'ArnoldLicenseManager.app', 'Contents', 'MacOS', 'ArnoldLicenseManager' )

    ret = subprocess.Popen([cmdBinary])


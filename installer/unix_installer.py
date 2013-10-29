import subprocess, zipfile, shutil
import os, sys

subprocess.call(['less', '-e', os.path.abspath('MtoAEULA.txt')])

inp = raw_input('Please type in Accept to accept the EULA, or press enter to cancel the installer : ')

if inp.replace(' ', '').lower() != 'accept':
    sys.exit(0)

def InstallerHeader():
    os.system('clear')
    print '   --== MtoA Installer == --    '

InstallerHeader()
print ''' 
    Select installation mode:
        1) Default (Setting up Maya Module and Env variables)
        2) Extracting the Zip archive
      '''
inp = raw_input('Please select the mode [1] : ')
inp = inp.replace(' ', '')

installMode = 1

if inp == '2':
    installMode = 2

def EnsureDir(d):
    try:        
        if not os.path.exists(d):
            os.makedirs(d)
        return True
    except:
        return False

installDir = ''

while True:
    InstallerHeader()
    inp = raw_input('Select the installation directory [/opt/solidangle/mtoa] : ').lstrip()

    installDir = '/opt/solidangle/mtoa'
    if inp != '':
        installDir = inp
    if not EnsureDir(installDir):
        InstallerHeader()
        print '''
    Cannot create target Directory.
    Do you want to retry [yes / no]?
              '''
        inp = raw_input('    ').replace(' ', '').lower()
        if inp == 'no':
            sys.exit(0)
    else:
        break

try:
    zipfile.ZipFile(os.path.abspath('package.zip'), 'r').extractall(installDir)
except:
    print 'Error extracting the contents of the package.'

if installMode == '1': # do the proper installation
    pass

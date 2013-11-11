import subprocess, zipfile, shutil
import os, sys

subprocess.call(['less', '-e', os.path.abspath('MtoAEULA.txt')])

def InstallerHeader():
    os.system('clear')
    print '   --== MtoA Installer == --    '

InstallerHeader()
print '''
    Please Type in Accept to confirm the EULA,
    or press enter to exit.
      '''
inp = raw_input('    ').replace(' ', '').lower()

if inp != 'accept':
    sys.exit(0)

InstallerHeader()
print ''' 
    Installation modes:
        1) Default (Setting up Maya Module and Env variables)
        2) Extracting the package
      '''
inp = raw_input('    Please select mode [1] : ')
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

mayaVersion = sys.argv[1] if sys.argv[1] != '20135' else '2013.5'
enableEnvInstall = mayaVersion == '2012'

while True:
    homeDir = os.path.expanduser('~')
    InstallerHeader()
    installDir = os.path.join(homeDir, 'solidangle', 'mtoa', mayaVersion)
    print '''
    Select the installation directory.
    [%s]
          ''' % installDir
    inp = raw_input('    ').lstrip()   
    if inp != '':
        installDir = inp
    if not EnsureDir(installDir):
        InstallerHeader()
        print '''
    Cannot create target Directory.
    Do you want to a different Directory?
    [yes / no]
              '''
        inp = raw_input('    ').replace(' ', '').lower()
        if inp != 'yes':
            sys.exit(0)
    else:
        break

try:
    zipfile.ZipFile(os.path.abspath('package.zip'), 'r').extractall(installDir)
except:
    print 'Error extracting the contents of the package.'
    sys.exit(0)

# regenerating the module file
mtoaModPath = os.path.join(installDir, 'mtoa.mod')
mtoaMod = open(mtoaModPath, 'w')
mtoaMod.write('+ mtoa any %s\n' % installDir)
if not enableEnvInstall:
    mtoaMod.write('PATH +:= bin\n')
    mtoaMod.write('MAYA_RENDER_DESC_PATH +:= \n')
mtoaMod.close()

# setting up executables properly
exList = [os.path.join('bin', 'kick'), os.path.join('bin', 'maketx')]
for ex in exList:
    try:
        subprocess.call(['chmod', '+x', os.path.join(installDir, ex)])
    except:
        print 'Error adding +x to executable %s' % ex
        sys.exit(0)

if installMode == 1: # do the proper installation
    homeDir = os.path.expanduser('~')
    mayaBaseDir = ''
    if sys.platform == 'darwin':
        mayaBaseDir = os.path.join(homeDir, 'Library', 'Preferences', 'Autodesk', 'maya', '%s-x64' % mayaVersion)
    else:
        mayaBaseDir = os.path.join(homeDir, 'maya', '%s-x64' % mayaVersion)
    if not os.path.exists(mayaBaseDir):
        os.system('clear')
        print 'Home directory for Maya %s does not exists.' % mayaVersion
        sys.exit(1)
    modulesDir = os.path.join(mayaBaseDir, 'modules')
    if not EnsureDir(modulesDir):
        os.system('clear')
        print 'Modules directory for the current Maya Version cannot be created.'
        sys.exit(1)
    shutil.copy(mtoaModPath, os.path.join(modulesDir, 'mtoa.mod'))
    if enableEnvInstall:
        mayaEnvPath = os.path.join(mayaBaseDir, 'Maya.env')
        mayaEnvContents = []
        additionToEnv = ['PATH=$PATH:%s\n' % os.path.join(installDir, 'bin'), 
                     'MAYA_RENDER_DESC_PATH=$MAYA_RENDER_DESC_PATH:%s\n' % installDir]    
        if os.path.exists(mayaEnvPath):
            for line in open(mayaEnvPath, 'r').readlines():
                if line in additionToEnv:
                    continue
                if line[-1:] != '\n':
                    line += '\n'
                mayaEnvContents.append(line)
        mayaEnv = open(mayaEnvPath, 'w')
        for line in mayaEnvContents:
            mayaEnv.write(line)
        for line in additionToEnv:
            mayaEnv.write(line)
        mayaEnv.close()

os.system('clear')
print 'Installation successful!'

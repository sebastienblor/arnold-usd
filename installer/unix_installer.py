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
        1) Default (Setting up Maya Module and arnoldRenderer.xml)
        2) Extracting the package
      '''
inp = raw_input('    Please select mode [1] : ')
inp = inp.replace(' ', '')

installMode = 1

if inp == '2':
    installMode = 2
else:
    # Check for being the root user
    try:
        p = subprocess.Popen('whoami', stdout=subprocess.PIPE)
        whoami, err = p.communicate()
        if whoami[0:4] != 'root':
            print 'Root privileges are required to configure MtoA for Maya.'
            sys.exit(0)
    except:
        print 'Root privileges are required to configure MtoA for Maya.'
        sys.exit(0)

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
    sudoUser = ''
    try:
        sudoUser = os.environ['SUDO_USER']
    except:
        print 'Error accessing the sudo user.'
        sys.exit(0)
    homeDir = os.path.join('/home', sudoUser)
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
        additionToEnv = ['PATH=$PATH:%s\n' % os.path.join(installDir, 'bin')]
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
    try:
        subprocess.call(['chown', '-R', sudoUser, installDir])
    except:
        print 'Error setting the user as the owner of the Installation Directory.'
    # install the renderer description file in the maya dir
    mayaInstallDir = os.path.join('/usr', 'autodesk', 'maya%s-x64' % mayaVersion)
    if not os.path.exists(mayaInstallDir):
        print '''
            Please specify maya installation directory
            for version %s :
        ''' % mayaVersion
        mayaInstallDir = raw_input('    ')
    renderDescFolder = os.path.join(mayaInstallDir, 'bin', 'rendererDesc')
    shutil.copy(os.path.join(installDir, 'arnoldRenderer.xml'), os.path.join(renderDescFolder, 'arnoldRenderer.xml'))

os.system('clear')
print 'Installation successful!'

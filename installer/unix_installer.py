import subprocess, zipfile, shutil
import os, sys, platform

silent = False;

installMode = 1
isRoot = False
inp = ''

def InstallerHeader():
        os.system('clear')
        print('   --== Maya to Arnold Installer ==--    ')

        
if len(sys.argv) > 3 and "silent" == sys.argv[3]:
    silent = True
    
if not silent:

    if platform.system().lower() != sys.argv[2]:
        print('''
        Installer incompatible with your operating system.
        Your system is %s , while the installer has been built
        for %s .
            ''' % (platform.system().lower(), sys.argv[2]))
        sys.exit(0)

    try: input = raw_input
    except NameError: pass

    subprocess.call(['less', '-e', os.path.abspath('MtoAEULA.txt')])

    InstallerHeader()
    print('''
        Please type accept and press enter to agree to the terms and conditions,
        or press enter to exit.
          ''')
    inp = input('    ').replace(' ', '').lower()

    if inp != 'accept':
        sys.exit(0)

    InstallerHeader()
    print(''' 
        Installation modes:
            1) Automatic (set up Maya module and arnoldRenderer.xml)
            2) Extract the package
          ''')
    inp = input('    Please select mode [1] : ')
    inp = inp.replace(' ', '')



if inp == '2':
    installMode = 2
else:
    # Check for being the root user
    try:
        p = subprocess.Popen('whoami', stdout=subprocess.PIPE)
        whoami, err = p.communicate()
        if whoami.find(b'root') != 0:
            if not silent:
                print('Root privileges are required to configure MtoA for Maya.')
            sys.exit(0)
        isRoot = True
    except:
        sys.exit(0)

installDir = ''

mayaVersionDir = ''

if sys.argv[1] != '2016' and sys.argv[1] != '2017' and sys.argv[1] != '2018':
    mayaVersionDir = '%s-x64' % sys.argv[1]
else:
    mayaVersionDir = sys.argv[1]
       
mayaVersion = ''
if sys.argv[1] != '20135':
    mayaVersion = sys.argv[1]
else:
    mayaVersion = '2013.5'

userString = '~'
sudoUser = ''

if isRoot:
    try:
        sudoUser = os.environ['SUDO_USER']
        userString = '~%s' % sudoUser
    except:
        userString = '~'
        sudoUser = 'root'

def EnsureDir(d):
    try:
        dirlist = d.split('/')
        dr = '/'
        for dd in dirlist:
            dr = os.path.join(dr, dd)
            if not os.path.exists(dr):
                os.makedirs(dr)
                try:
                    subprocess.call(['chown', sudoUser, dr])
                except:
                    pass
        return True
    except:
        return False

if silent:
    installDir = os.path.join('/opt', 'solidangle', 'mtoa', mayaVersion)
    if not EnsureDir(installDir):
        sys.exit(0)
else:
    while True:
        InstallerHeader()
        installDir = os.path.join('/opt', 'solidangle', 'mtoa', mayaVersion)
        print('''
        Select the installation directory.
        [%s]
              ''' % installDir)
        inp = input('    ').lstrip()   
        if inp != '':
            installDir = inp
        if not EnsureDir(installDir):
            InstallerHeader()
            print('''
        Cannot create target directory.
        Do you want to install to a different directory?
        [yes / no]
                  ''')
            inp = input('    ').replace(' ', '').lower()
            if inp != 'yes':
                sys.exit(0)
        else:
            break

# http://stackoverflow.com/questions/7806563/how-to-unzip-a-file-with-python-2-4

def unzip(zipFilePath, destDir):
    zfile = zipfile.ZipFile(zipFilePath)
    for name in zfile.namelist():
        (dirName, fileName) = os.path.split(name)
        # Check if the directory exisits
        if dirName == '.':
            dirName = ''
        newDir = os.path.join(destDir, dirName)
        if not EnsureDir(newDir):
            continue
        if not fileName == '':
            # file
            fd = open(destDir + '/' + name, 'wb')
            fd.write(zfile.read(name))
            fd.close()
    zfile.close()

try:
    #zipfile.ZipFile(os.path.abspath('package.zip'), 'r').extractall(installDir)
    unzip(os.path.abspath('package.zip'), installDir)
except:
    if not silent:
        print('Error extracting the contents of the package.')
    sys.exit(0)

# regenerating the module file
mtoaModPath = os.path.join(installDir, 'mtoa.mod')
mtoaMod = open(mtoaModPath, 'w')
mtoaMod.write('+ mtoa any %s\n' % installDir)
mtoaMod.write('PATH +:= bin\n')
mtoaMod.write('MAYA_CUSTOM_TEMPLATE_PATH +:= scripts/mtoa/ui/templates\n')
mtoaMod.close()

# setting up executables properly
exList = [os.path.join('bin', 'kick'), os.path.join('bin', 'maketx')]
for ex in exList:
    try:
        subprocess.call(['chmod', '+x', os.path.join(installDir, ex)])
    except:
        if not silent:
            print('Error adding +x to executable %s' % ex)
        sys.exit(0)

if installMode == 1: # do the proper installation
    homeDir = os.path.expanduser(userString)
    mayaBaseDir = ''
    if sys.platform == 'darwin':
        mayaBaseDir = os.path.join(homeDir, 'Library', 'Preferences', 'Autodesk', 'maya%s' % mayaVersionDir)
    else:
        mayaBaseDir = os.path.join('/usr', 'autodesk', 'modules', 'maya', sys.argv[1])
    if not EnsureDir(mayaBaseDir):
        if not silent:
            os.system('clear')
            print('Home directory for Maya %s does not exists.' % mayaVersion)
        sys.exit(1)
    if sys.platform == 'darwin':
        modulesDir = os.path.join(mayaBaseDir, 'modules')
    else:
        modulesDir = mayaBaseDir
    if not EnsureDir(modulesDir):
        if not silent:
            os.system('clear')
            print('Modules directory for the current Maya version cannot be created.')
        sys.exit(1)
    shutil.copy(mtoaModPath, os.path.join(modulesDir, 'mtoa.mod'))
    try:
        subprocess.call(['chown', sudoUser, os.path.join(modulesDir, 'mtoa.mod')])
    except:
        pass
    # install the renderer description file in the maya dir
    mayaInstallDir = ''
    if sys.platform == 'darwin':
        mayaInstallDir = os.path.join('/Applications', 'autodesk', 'maya%s' % mayaVersionDir)
    else:
        mayaInstallDir = os.path.join('/usr', 'autodesk', 'maya%s' % mayaVersionDir)
    if not os.path.exists(mayaInstallDir):
        if not silent:
            print('''
        Please specify maya installation directory
        for version %s :
            ''' % mayaVersion)
            mayaInstallDir = input('    ')
    if sys.platform == 'darwin':
        renderDescFolder = os.path.join(mayaInstallDir, 'Maya.app', 'Contents', 'bin', 'rendererDesc')
    else:
        renderDescFolder = os.path.join(mayaInstallDir, 'bin', 'rendererDesc')
    shutil.copy(os.path.join(installDir, 'arnoldRenderer.xml'), os.path.join(renderDescFolder, 'arnoldRenderer.xml'))
    
    if sys.argv[1] == '2017' or sys.argv[1] == '2018':
        homeDir = os.path.expanduser(userString)
        templatesDir = os.path.join(homeDir, 'maya', 'RSTemplates')
        if EnsureDir(templatesDir):
            shutil.copy(os.path.join(installDir, 'RSTemplates', 'MatteOverride-Arnold.json'), os.path.join(homeDir, 'maya', 'RSTemplates', 'MatteOverride-Arnold.json'))
            shutil.copy(os.path.join(installDir, 'RSTemplates', 'RenderLayerExample-Arnold.json'), os.path.join(homeDir, 'maya', 'RSTemplates', 'RenderLayerExample-Arnold.json'))

if not silent:
    os.system('clear')
    print('Installation successful!')

# vim: filetype=python

## first we extend the module path to load our own modules
import subprocess
import sys, os, re
sys.path = ["tools/python"]  + sys.path

import utils.system as system
import glob
from utils.build_tools import *
from utils.mtoa_build_tools import *
#from solidangle_tools import *

from multiprocessing import cpu_count

import SCons
import shutil

from colorama import init
init()
from colorama import Fore, Back, Style

MTOA_VERSION = get_mtoa_version(4)

################################################################################
#   Operating System detection
################################################################################

EXTERNAL_PATH = os.path.abspath('external')

if system.os == 'darwin':
    ALLOWED_COMPILERS = ('gcc',)   # Do not remove this comma, it's magic
    arnold_default_api_lib = os.path.join('$ARNOLD', 'bin')
    glew_default_lib = os.path.join(EXTERNAL_PATH, 'glew-1.10.0', 'lib', 'libGLEW.a')
    glew_default_include = os.path.join(EXTERNAL_PATH, 'glew-1.10.0', 'include')
elif system.os == 'linux':
    ALLOWED_COMPILERS = ('gcc',)   # Do not remove this comma, it's magic
    # linux conventions would be to actually use lib for dynamic libraries!
    arnold_default_api_lib = os.path.join('$ARNOLD', 'bin')
    glew_default_lib = os.path.join(EXTERNAL_PATH, 'glew-1.10.0', 'lib', 'libGLEW.a')
    glew_default_include = '/usr/include'
elif system.os == 'windows':
    ALLOWED_COMPILERS = ('msvc', 'icc')
    arnold_default_api_lib = os.path.join('$ARNOLD', 'lib')
    glew_default_lib = os.path.join(EXTERNAL_PATH, 'glew-1.10.0', 'lib', 'glew32s.lib')
    glew_default_include = os.path.join(EXTERNAL_PATH, 'glew-1.10.0', 'include')
else:
    print "Unknown operating system: %s" % system.os
    Exit(1)

# Scons doesn't provide a string variable
def StringVariable(key, help, default):
    # We always get string values, so it's always valid and trivial to convert
    return (key, help, default, lambda k, v, e: True, lambda s: s)

################################################################################
#   Build system options
################################################################################

vars = Variables('custom.py')
vars.AddVariables(
    ## basic options
    StringVariable('TARGET_ARCH', 'Target Architecture', 'x86_64'),
    EnumVariable('MODE'       , 'Set compiler configuration', 'opt'             , allowed_values=('opt', 'debug', 'profile')),
    EnumVariable('WARN_LEVEL' , 'Set warning level'         , 'strict'            , allowed_values=('strict', 'warn-only', 'none')),
    EnumVariable('COMPILER'   , 'Set compiler to use'       , ALLOWED_COMPILERS[0], allowed_values=ALLOWED_COMPILERS),
    ('COMPILER_VERSION'       , 'Version of compiler to use', ''),
    BoolVariable('MULTIPROCESS','Enable multiprocessing in the testsuite', True),
    BoolVariable('SHOW_CMDS'  , 'Display the actual command lines used for building', False),
    PathVariable('LINK', 'Linker to use', None),
    PathVariable('SHCC', 'Path to C++ (gcc) compiler used', None),
    PathVariable('SHCXX', 'Path to C++ (gcc) compiler used for generating shared-library objects', None),
    ('FTP'            , 'Path of the FTP to upload the package'        , ''),
    ('FTP_SUBDIR'     , 'Subdirectory on the FTP to place the package' , ''),
    ('FTP_USER'       , 'Username for the FTP'                         , ''),
    ('FTP_PASS'       , 'Password for the FTP'                         , ''),
    ('PACKAGE_SUFFIX' , 'Suffix for the package names'                 , ''),
                  
    BoolVariable('COLOR_CMDS' , 'Display colored output messages when building', True),
    EnumVariable('SHOW_TEST_OUTPUT', 'Display the test log as it is being run', 'single', allowed_values=('always', 'never', 'single')),
    EnumVariable('TEST_ORDER', 'Set the execution order of tests to be run', 'reverse', allowed_values=('normal', 'reverse')),
    EnumVariable('USE_VALGRIND', 'Enable Valgrinding', 'False', allowed_values=('False', 'True', 'Full')),
    BoolVariable('UPDATE_REFERENCE', 'Update the reference log/image for the specified targets', False),
    BoolVariable('USE_GPU', 'Update the reference log/image for the specified targets', False),
    BoolVariable('SHOW_PLOTS', 'Display timing plots for the testsuite. gnuplot has to be found in the environment path.', False),
    BoolVariable('CLEAN_TESTSUITE_RESULTS', 'Remove all the test files from the testsuite output and only keep the reports/images', False),
    ('TEST_THREADS' , 'Number of simultaneous tests to run', 4),
    ('TEST_PATTERN' , 'Glob pattern of tests to be run', 'test_*'),
    ('GCC_OPT_FLAGS', 'Optimization flags for gcc', '-O3 -funroll-loops'),
    BoolVariable('DISABLE_COMMON', 'Disable shaders found in the common repository', False),
    BoolVariable('SWATCHES_SKIP_LICENSE', 'Skip License check for render swatches', True),
    PathVariable('BUILD_DIR',
                 'Directory where temporary build files are placed by scons', 
                 'build', PathVariable.PathIsDirCreate),

    PathVariable('MAYA_ROOT',
                 'Directory where Maya is installed (defaults to $MAYA_LOCATION)', 
                 get_default_path('MAYA_LOCATION', '.')),
    PathVariable('MAYA_INCLUDE_PATH',
                 'Directory where Maya SDK headers are installed',
                 '.'),
    PathVariable('ARNOLD', 
                 'Where to find Arnold installation', 
                 get_default_path('ARNOLD_HOME', 'Arnold')),                   
    PathVariable('ARNOLD_API_INCLUDES', 
                 'Where to find Arnold API includes', 
                 os.path.join('$ARNOLD', 'include'), PathVariable.PathIsDir),
    PathVariable('ARNOLD_API_LIB', 
                 'Where to find Arnold API static libraries', 
                 arnold_default_api_lib, PathVariable.PathIsDir),
    PathVariable('ARNOLD_BINARIES', 
                 'Where to find Arnold API dynamic libraries and executables', 
                 os.path.join('$ARNOLD', 'bin'), PathVariable.PathIsDir),
    PathVariable('ARNOLD_PYTHON', 
                 'Where to find Arnold python bindings', 
                 os.path.join('$ARNOLD', 'python'), PathVariable.PathIsDir),  
    PathVariable('GLEW_INCLUDES', 
                 'Where to find GLEW includes', 
                 glew_default_include, PathVariable.PathIsDir),
    PathVariable('GLEW_LIB', 
                 'Where to find GLEW static library', 
                 glew_default_lib, PathVariable.PathIsFile),
    PathVariable('TARGET_MODULE_PATH', 
                 'Path used for installation of the mtoa module', 
                 '.', PathVariable.PathIsDirCreate),
    PathVariable('TARGET_PLUGIN_PATH', 
                 'Path used for installation of the mtoa plugin', 
                 os.path.join('$TARGET_MODULE_PATH', 'plug-ins'), PathVariable.PathIsDirCreate),
    PathVariable('TARGET_SCRIPTS_PATH', 
                 'Path used for installation of scripts', 
                 os.path.join('$TARGET_MODULE_PATH', 'scripts'), PathVariable.PathIsDirCreate),
    PathVariable('TARGET_PYTHON_PATH', 
                 'Path used for installation of Python scripts', 
                 os.path.join('$TARGET_MODULE_PATH', 'scripts'), PathVariable.PathIsDirCreate),
    PathVariable('TARGET_INCLUDE_PATH', 
                 'Path used for installation of API headers', 
                 os.path.join('$TARGET_MODULE_PATH', 'include'), PathVariable.PathIsDirCreate),                   
    PathVariable('TARGET_ICONS_PATH', 
                 'Path used for installation of icons', 
                 os.path.join('$TARGET_MODULE_PATH', 'icons'), PathVariable.PathIsDirCreate),
    PathVariable('TARGET_DESCR_PATH', 
                 'Path for renderer description xml file', 
                 '$TARGET_MODULE_PATH', PathVariable.PathIsDirCreate),
    PathVariable('TARGET_SHADER_PATH', 
                 'Path used for installation of arnold shaders', 
                 os.path.join('$TARGET_MODULE_PATH', 'shaders'), PathVariable.PathIsDirCreate),
    PathVariable('TARGET_PROCEDURAL_PATH', 
                 'Path used for installation of arnold procedurals', 
                 os.path.join('$TARGET_MODULE_PATH', 'procedurals'), PathVariable.PathIsDirCreate),
    PathVariable('TARGET_PLUGINS_PATH', 
                 'Path used for installation of arnold/mtoa plugins', 
                 os.path.join('$TARGET_MODULE_PATH', 'plugins'), PathVariable.PathIsDirCreate),    
    PathVariable('TARGET_EXTENSION_PATH', 
                 'Path used for installation of mtoa translator extensions', 
                 os.path.join('$TARGET_MODULE_PATH', 'extensions'), PathVariable.PathIsDirCreate),
    PathVariable('TARGET_LIB_PATH', 
                 'Path for libraries', 
                 os.path.join('$TARGET_MODULE_PATH', 'lib'), PathVariable.PathIsDirCreate),
    PathVariable('TARGET_DOC_PATH', 
                 'Path for documentation', 
                 os.path.join('$TARGET_MODULE_PATH', 'docs','mtoa'), PathVariable.PathIsDirCreate),                  
    PathVariable('TARGET_BINARIES', 
                 'Path for libraries', 
                 os.path.join('$TARGET_MODULE_PATH', 'bin'), PathVariable.PathIsDirCreate),
    PathVariable('TARGET_VP2_PATH',
                    'Path for VP2 shader files.',
                    os.path.join('$TARGET_MODULE_PATH', 'vp2'), PathVariable.PathIsDirCreate),
    PathVariable('TARGET_PRESETS_PATH',
                 'Path for presets.',
                 os.path.join('$TARGET_MODULE_PATH', 'presets'), PathVariable.PathIsDirCreate),
    PathVariable('TARGET_USD_PATH', 
                 'Path used for installation of arnold USD modules', 
                 os.path.join('$TARGET_MODULE_PATH', 'usd'), PathVariable.PathIsDirCreate),
    PathVariable('SHAVE_API', 
                 'Where to find Shave API', 
                 '.', PathVariable.PathIsDir),
    PathVariable('TOOLS_PATH',
                 'Where to find external tools required for sh',
                 '.', PathVariable.PathIsDir),
    PathVariable('NSIS_PATH', 'Where to find NSIS installed. Required for generating the Windows installers.',
                 '.', PathVariable.PathIsDir),
    PathVariable('REFERENCE_API_LIB', 'Path to the reference mtoa_api lib', None),
    ('REFERENCE_API_VERSION', 'Version of the reference mtoa_api lib', ''),
    StringVariable('USD_PATH', 'Path to the USD root folder, to build the render delegate', None),
    StringVariable('MAYAUSD_PATH', 'Maya-USD installation root', None),
    StringVariable('USD_PATH_PYTHON2', 'Path to the USD root folder, to build the render delegate for python2', None),
    StringVariable('MAYAUSD_PATH_PYTHON2', 'Maya-USD installation root for python2', None),
    StringVariable('MOD_SUFFIX', 'Install another mod file with a given suffix', None),
    StringVariable('TIMELIMIT', 'Time limit for each test (in seconds)', '300'),
    BoolVariable('MTOA_DISABLE_RV', 'Disable Arnold RenderView in MtoA', False),
    BoolVariable('MAYA_MAINLINE', 'Set correct MtoA version for Maya mainline/master builds', False),
    BoolVariable('BUILD_EXT_TARGET_INCLUDES', 'Build MtoA extensions against the target API includes', False),
    BoolVariable('PREBUILT_MTOA', 'Use already built MtoA targets, instead of triggering a rebuild', False),
    BoolVariable('ENABLE_AXFTOA' , 'Enable the build of the AXF shader that links against AxftoA', True),
    BoolVariable('UPDATE_SUBMODULES', 'Init and update the submodules prior to the build', True),
    ('SIGN_COMMAND', 'Script to be executed in each of the packaged files', '')
)

if system.os == 'darwin':
    vars.Add(StringVariable('SDK_VERSION', 'Version of the Mac OSX SDK to use', None))
    vars.Add(PathVariable('SDK_PATH', 'Root path to installed OSX SDKs', '/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs'))
    vars.Add(StringVariable('MACOS_VERSION_MIN', 'Target MacOS version', '10.11'))
    
if system.os == 'windows':
    vars.Add(BoolVariable('USE_VISUAL_STUDIO_EXPRESS', 'Use the express version of visual studio. (UNSUPPORTED!)', False))
    # Ugly hack. Create a temporary environment, without loading any tool, so we can set the MSVC_ARCH
    # variable from the contents of the TARGET_ARCH variable. Then we can load tools.
    tmp_env = Environment(variables = vars, tools=[])
    TARGET_ARCH = tmp_env['TARGET_ARCH']
    tmp_env.Append(MSVC_ARCH = 'amd64')
    MAYA_ROOT = tmp_env.subst(tmp_env['MAYA_ROOT'])
    MAYA_INCLUDE_PATH = tmp_env.subst(tmp_env['MAYA_INCLUDE_PATH'])
    if MAYA_INCLUDE_PATH == '.':
        MAYA_INCLUDE_PATH = os.path.join(MAYA_ROOT, 'include')
    maya_version = get_maya_version(os.path.join(MAYA_INCLUDE_PATH, 'maya', 'MTypes.h'))

    maya_version_base = maya_version[0:4]

    msvc_version = '14.0'

    # Visual Studio 2019
    if int(maya_version_base) >= 2022 or tmp_env['MAYA_MAINLINE']:
        msvc_version = '14.2'
    
    if tmp_env['USE_VISUAL_STUDIO_EXPRESS']:
        msvc_version += 'Exp'
    tmp_env['MSVC_VERSION'] = msvc_version
    #print tmp_env['MSVC_VERSION']
    env = tmp_env.Clone(tools=['default'])
    # restore as the Clone overrides it
    env['TARGET_ARCH'] = TARGET_ARCH
else:
    env = Environment(variables = vars)

if env['TARGET_MODULE_PATH'] == '.':
    print "Please define TARGET_MODULE_PATH (Path used for installation of the mtoa plugin)"
    Exit(1)

env.Append(BUILDERS = {'MakeModule' : make_module})

env.AppendENVPath('PATH', env.subst(env['TOOLS_PATH']))

env['MTOA_VERSION'] = MTOA_VERSION

# Set default amount of threads set to the cpu counts in this machine.
# This can be overridden through command line by setting e.g. "abuild -j 1"
SetOption('num_jobs', int(cpu_count()))

# Configure colored output
color_green   = ''
color_red     = ''
color_bright  = ''
color_bgreen  = ''
color_bred    = ''
color_reset   = ''
if env['COLOR_CMDS']:
    color_green   = Fore.GREEN
    color_red     = Fore.RED
    color_bright  = Style.BRIGHT
    color_bgreen  = color_green + color_bright
    color_bred    = color_red   + color_bright
    color_reset   = Fore.RESET + Back.RESET + Style.RESET_ALL

#define shortcuts for the above paths, with substitution of environment variables
MAYA_ROOT = env.subst(env['MAYA_ROOT'])
MAYA_INCLUDE_PATH = env.subst(env['MAYA_INCLUDE_PATH'])
if env['MAYA_INCLUDE_PATH'] == '.':
    if system.os == 'darwin':
        MAYA_INCLUDE_PATH = os.path.join(MAYA_ROOT, '../../devkit/include')
    else:
        MAYA_INCLUDE_PATH = os.path.join(MAYA_ROOT, 'include')
    env['MAYA_INCLUDE_PATH'] = MAYA_INCLUDE_PATH

env['EXTERNAL_PATH'] = EXTERNAL_PATH
ARNOLD = env.subst(env['ARNOLD'])
ARNOLD_API_INCLUDES = env.subst(env['ARNOLD_API_INCLUDES'])

ARNOLD_AXF_LIB = os.path.join(EXTERNAL_PATH, 'axf/lib/', system.os )

ARNOLD_API_LIB = env.subst(env['ARNOLD_API_LIB'])
ARNOLD_BINARIES = env.subst(env['ARNOLD_BINARIES'])
ARNOLD_PYTHON = env.subst(env['ARNOLD_PYTHON']) 
TARGET_MODULE_PATH = env.subst(env['TARGET_MODULE_PATH']) 
TARGET_PLUGIN_PATH = env.subst(env['TARGET_PLUGIN_PATH'])  
TARGET_SCRIPTS_PATH = env.subst(env['TARGET_SCRIPTS_PATH']) 
TARGET_PYTHON_PATH = env.subst(env['TARGET_PYTHON_PATH']) 
TARGET_INCLUDE_PATH = env.subst(env['TARGET_INCLUDE_PATH']) 
TARGET_ICONS_PATH = env.subst(env['TARGET_ICONS_PATH'])  
TARGET_DESCR_PATH = env.subst(env['TARGET_DESCR_PATH'])  
TARGET_SHADER_PATH = env.subst(env['TARGET_SHADER_PATH']) 
TARGET_PROCEDURAL_PATH = env.subst(env['TARGET_PROCEDURAL_PATH'])
TARGET_USD_PATH = env.subst(env['TARGET_USD_PATH'])
TARGET_PLUGINS_PATH = env.subst(env['TARGET_PLUGINS_PATH'])
TARGET_EXTENSION_PATH = env.subst(env['TARGET_EXTENSION_PATH']) 
TARGET_LIB_PATH = env.subst(env['TARGET_LIB_PATH'])  
TARGET_DOC_PATH = env.subst(env['TARGET_DOC_PATH'])  
TARGET_BINARIES = env.subst(env['TARGET_BINARIES']) 
TARGET_VP2_PATH = env.subst(env['TARGET_VP2_PATH'])
TARGET_PRESETS_PATH = env.subst(env['TARGET_PRESETS_PATH'])
SHAVE_API = env.subst(env['SHAVE_API'])
PACKAGE_SUFFIX = env.subst(env['PACKAGE_SUFFIX'])
MOD_SUFFIX = env.get('MOD_SUFFIX')
env['ENABLE_BIFROST'] = 0
env['ENABLE_BIFROST_GRAPH'] = 0
env['ENABLE_GPU_CACHE'] = 1
env['ENABLE_ALEMBIC'] = 0


# Get arnold and maya versions used for this build


arnold_version    = get_arnold_version(ARNOLD_API_INCLUDES)
env['ARNOLD_VERSION'] = arnold_version
clm_version = 1

p = subprocess.Popen(os.path.join(ARNOLD_BINARIES, 'kick%s' % get_executable_extension()), shell=True, stdout = subprocess.PIPE)
retcode = p.wait()
for line in p.stdout:
    if 'clm-' in line:
        clmIndex = line.find('clm-')
        if line[clmIndex + 4:clmIndex + 5] == '2':
            clm_version = 2

if clm_version == 2:
    env.Append(CPPDEFINES = Split('CLIC_V2')) 
else:
    env.Append(CPPDEFINES = Split('CLIC_V1')) 

if env['ENABLE_AXFTOA']:
    env.Append(CPPDEFINES = Split('ENABLE_AXFTOA')) 


if env['SWATCHES_SKIP_LICENSE']:
    env.Append(CPPDEFINES = Split('SWATCHES_SKIP_LICENSE')) 
# Always read Maya VERSION from MType.h
maya_version = get_maya_version(os.path.join(MAYA_INCLUDE_PATH, 'maya', 'MTypes.h'))

if env['MAYA_MAINLINE']:
    env.Append(CPPDEFINES = Split('MAYA_MAINLINE')) 

maya_version_base = maya_version[0:4]

env['MAYA_VERSION'] = maya_version
env['MAYA_VERSION_BASE'] = maya_version_base

if system.os == 'linux' or env['MODE'] != 'debug' :    
    env['ENABLE_BIFROST'] = 1

bifrost_ext = 'bifrost'
# TODO add detection of Bifrost board plugin SDk and bifrost sdk
env["ENABLE_BIFROST_GRAPH"] = 1
env["ENABLE_ALEMBIC"] = 1


build_id = ""

try:
    p = subprocess.Popen(['git', 'rev-parse','--short=8','HEAD'], stdout=subprocess.PIPE)
    build_id, err = p.communicate()
    p = subprocess.Popen(['git', 'rev-parse','--abbrev-ref','HEAD'], stdout=subprocess.PIPE)
    build_branch, err = p.communicate()
    build_id = build_id.rstrip('\n') + " (" + build_branch.rstrip('\n') + ")"
except:
    pass #git is not in the path
build_id_file_contents = '#pragma once\n#define BUILD_ID "%s"\n\n' % build_id
build_id_file_read = ''
try:
    build_id_file = open(os.path.join('plugins', 'mtoa', 'utils', 'BuildID.h'), 'r')
    build_id_file_read = open.read()
    build_id_file.close()
except:
    pass # the file doesn't exists yet
if build_id_file_read != build_id_file_contents:
    build_id_file = open(os.path.join('plugins', 'mtoa', 'utils', 'BuildID.h'), 'w')
    build_id_file.write(build_id_file_contents)
    build_id_file.flush()
    build_id_file.close()
# print build info
print ''
print 'Building       : ' + 'MtoA %s' % (MTOA_VERSION)
print 'Arnold version : %s' % arnold_version
print 'Maya version   : %s' % maya_version
print 'CLM version    : %s' % clm_version
print 'Mode           : %s' % (env['MODE'])
print 'Host OS        : %s' % (system.os)
print 'Threads        : %s' % GetOption('num_jobs')
if system.os == 'linux':
    try:
        if env['SHCC'] != '' and env['SHCC'] != '$CC':
            print 'Compiler       : %s'  % (env['SHCC'])
        else:
            p = subprocess.Popen([env['COMPILER'] + env['COMPILER_VERSION'], '-dumpversion'], stdout=subprocess.PIPE)
            compiler_version, err = p.communicate()
            print 'Compiler       : %s' % (env['COMPILER'] + compiler_version[:-1])
    except:
        pass
elif system.os == 'windows':
    print 'MSVC version   : %s' % (env['MSVC_VERSION'])
print 'Build ID       : %s' % build_id
print 'SCons          : %s' % (SCons.__version__)
print ''

try:
   import json
except ImportError:
   import simplejson as json

try:
    json_data = open('dependencies.json')
    data = json.load(json_data)
    if data['arnold'] != arnold_version:
        print '''
        You are building with arnold %s instead
        of the officially supported version %s. 
        You might encounter bugs, build errors 
        or undefined behavior.
        ''' % (arnold_version, data['arnold'])
except:
    pass

################################
## COMPILER OPTIONS
################################

## Generic Windows stuff476
if system.os == 'windows':
    # Embed manifest in executables and dynamic libraries
    env['LINKCOM'] = [env['LINKCOM'], 'mt.exe -nologo -manifest ${TARGET}.manifest -outputresource:$TARGET;1']
    env['SHLINKCOM'] = [env['SHLINKCOM'], 'mt.exe -nologo -manifest ${TARGET}.manifest -outputresource:$TARGET;2']

export_symbols = env['MODE'] in ['debug', 'profile']

# FIXME : Bifrost library "bifrostrendercore" is returning warnings. Until we solve this I'm forcing warn_only here :-/
env['WARN_LEVEL'] = 'warn_only'

if env['COMPILER'] == 'gcc':
    if system.os == 'linux' and env['SHCC'] != '' and env['SHCC'] != '$CC':
        env['CC'] = env['SHCC']
        env['CXX'] = env['SHCXX']
        #env.Append(CXXFLAGS = Split('-std=c++11 -Wno-reorder'))
        #env.Append(CCFLAGS = Split('-std=c++11 -Wno-reorder'))

        # FIXME : To be removed and used through scons variables
        #env['CC']  = '/solidangle/toolchain/3.0/bin/clang'
        #env['CXX'] = '/solidangle/toolchain/3.0/bin/clang++'

    else:
        compiler_version = env['COMPILER_VERSION']
        if compiler_version != '':
            env['CC']  = 'gcc' + compiler_version
            env['CXX'] = 'g++' + compiler_version
        #else:
            # FIXME to be removed and used through scons variables
        #    env['CC']  = '/opt/local/bin/clang'# + compiler_version
        #    env['CXX'] = '/opt/local/bin/clang++'# + compiler_version

	#env.Append(CCFLAGS = Split('-msse4.2')) # matches Arnold and ensures same math symbols

    # env.Append(CXXFLAGS = Split('-fno-rtti'))

    if env['MODE'] == 'opt': 
        env.Append(CPPDEFINES = Split('NDEBUG')) 

    ## Hide all internal symbols (the ones without AI_API decoration)
    # On linux, for Maya IO we need to add more build flags so that the 
    # CLM symbols aren't loaded (see #3786). We should remove this 
    # once the problem is solved on the CLM side 
    if system.os == 'linux':
        env.Append(CCFLAGS = Split('-fvisibility=hidden -Wl,-Bsymbolic'))
        env.Append(CXXFLAGS = Split('-fvisibility=hidden -Wl,-Bsymbolic'))
        env.Append(LINKFLAGS = Split('-fvisibility=hidden -Wl,-Bsymbolic'))
    else:
        env.Append(CCFLAGS = Split('-fvisibility=hidden'))
        env.Append(CXXFLAGS = Split('-fvisibility=hidden'))
        env.Append(LINKFLAGS = Split('-fvisibility=hidden'))
        # need to add extra padding for the rpath #4488
        env.Append(LINKFLAGS = Split('-headerpad_max_install_names'))

    env.Append(CXXFLAGS = Split('-Wno-reorder'))
    env.Append(CCFLAGS = Split('-Wno-reorder'))


    ## Hardcode '.' directory in RPATH in linux
    if system.os == 'linux':
        env.Append(LINKFLAGS = Split('-z origin') )
        #env.Append(RPATH = env.Literal(os.path.join('\\$$ORIGIN', '..', 'bin')))
    
    if int(maya_version_base) >= 2021:
        print('------ Setting C++14 for version {}'.format(maya_version_base))
        env.Append(CXXFLAGS = Split('-std=c++14'))
        env.Append(CCFLAGS = Split('-std=c++14'))
    else:
        env.Append(CXXFLAGS = Split('-std=c++11'))
        
    if system.os == 'darwin':
        env.Append(CXXFLAGS = Split('-stdlib=libc++'))
        env.Append(LINKFLAGS = Split('-stdlib=libc++'))
    
        
    ## warning level
    if env['WARN_LEVEL'] == 'none':
        env.Append(CCFLAGS = Split('-w'))
    else:
        env.Append(CCFLAGS = Split('-Wall -Wsign-compare'))
        if env['WARN_LEVEL'] == 'strict':
            env.Append(CCFLAGS = Split('-Werror'))

    ## optimization flags
    if env['MODE'] == 'opt' or env['MODE'] == 'profile':
        env.Append(CCFLAGS = Split(env['GCC_OPT_FLAGS']))
    if env['MODE'] == 'debug' or env['MODE'] == 'profile':

        if system.os == 'darwin': 
            env.Append(CCFLAGS = Split('-g3')) 
            env.Append(LINKFLAGS = Split('-g3')) 
        else: 
            env.Append(CCFLAGS = Split('-g -fno-omit-frame-pointer')) 
            env.Append(LINKFLAGS = Split('-g')) 

    if system.os == 'darwin':
        ## tell gcc to compile a 64 bit binary
        if env['TARGET_ARCH'] == 'x86_64':
            env.Append(CCFLAGS   = ['-arch', 'x86_64'])
            env.Append(LINKFLAGS = ['-arch', 'x86_64'])
        if env['TARGET_ARCH'] == 'arm64':
            env.Append(CCFLAGS   = ['-arch', 'arm64'])
            env.Append(LINKFLAGS = ['-arch', 'arm64'])
        if env['TARGET_ARCH'].find('arm64') >= 0 and env['TARGET_ARCH'].find('x86_64') >= 0:
            env.Append(CCFLAGS   = ['-arch', 'arm64'])
            env.Append(CCFLAGS   = ['-arch', 'x86_64'])
            env.Append(LINKFLAGS = ['-arch', 'arm64'])
            env.Append(LINKFLAGS = ['-arch', 'x86_64'])

        env.Append(CCFLAGS = env.Split('-mmacosx-version-min=' + env['MACOS_VERSION_MIN']))
        env.Append(LINKFLAGS = env.Split('-mmacosx-version-min='+ env['MACOS_VERSION_MIN']))


        if env['SDK_VERSION']:
            env.Append(CCFLAGS = env.Split('-isysroot %s/MacOSX%s.sdk/' % (env['SDK_PATH'], env['SDK_VERSION'])))
            env.Append(LINKFLAGS = env.Split('-isysroot %s/MacOSX%s.sdk/' % (env['SDK_PATH'], env['SDK_VERSION'])))

        env.Append(LINKFLAGS = env.Split(['-framework', 'Security']))

elif env['COMPILER'] == 'msvc':
    MSVC_FLAGS  = " /W3"         # Warning level : 3
    MSVC_FLAGS += " /wd 4005"
    MSVC_FLAGS += " /EHsc"       # enable synchronous C++ exception handling model & 
                                # assume extern "C" functions do not throw exceptions
    MSVC_FLAGS += " /Gd"         # makes __cdecl the default calling convention 
    MSVC_FLAGS += " /fp:precise" # precise floating point model: results are predictable

    if env['WARN_LEVEL'] == 'strict':
        MSVC_FLAGS += " /WX"  # treats warnings as errors

    if export_symbols:
        MSVC_FLAGS += " /Z7"  # generates complete debug information

    LINK_FLAGS  = " /MANIFEST"

    # Needed because of the compilation and/or runtime problem 
    # related to the list iterator optimization from MSVC. 
    if env['MODE'] in ['debug']:
        MSVC_FLAGS += " -D_ITERATOR_DEBUG_LEVEL=0"

    if env['MODE'] in ['opt', 'profile']:
        MSVC_FLAGS += " /Ob2"    # enables inlining of ANY function (compiler discretion)
        MSVC_FLAGS += " /GL"     # enables whole program optimization
        MSVC_FLAGS += " /MD"     # uses multithreaded DLL runtime library
        MSVC_FLAGS += " /Ox"     # selects maximum optimization
        LINK_FLAGS += " /DEBUG"
        LINK_FLAGS += " /LTCG"   # enables link time code generation (needed by /GL)
    else:  ## Debug mode
        MSVC_FLAGS += " /Od"   # disables all optimizations
        MSVC_FLAGS += " /MDd"   # uses *NON-DEBUG* multithreaded DLL runtime library

        LINK_FLAGS += " /DEBUG"

    env.Append(CCFLAGS = Split(MSVC_FLAGS))
    env.Append(LINKFLAGS = Split(LINK_FLAGS))
   
    if env['MODE'] == 'opt':
        env.Append(CPPDEFINES = Split('NDEBUG'))
    # We cannot enable this define, as it will try to use symbols from the debug runtime library  
    # Re-acivated to allow memory tracking in MSVC
    if env['MODE'] == 'debug':
        env.Append(CPPDEFINES = Split('_DEBUG'))
        # for MSVC memory tracking 
        #env.Append(CPPDEFINES = Split('_CRTDBG_MAP_ALLOC'))

    env.Append(CPPDEFINES = Split('_CRT_SECURE_NO_WARNINGS'))
elif env['COMPILER'] == 'icc':
    env.Tool('intelc', abi = 'intel64')

    ICC_FLAGS  = " /W3"            # displays remarks, warnings, and errors
    ICC_FLAGS += " /Qstd:c99"      # conforms to The ISO/IEC 9899:1999 International Standard
    ICC_FLAGS += " /EHsc"          # enable synchronous C++ exception handling model & 
                                  # assume extern "C" functions do not throw exceptions
    ICC_FLAGS += " /GS"            # generates code that detects some buffer overruns 
    ICC_FLAGS += " /Qprec"         # improves floating-point precision and consistency 
    ICC_FLAGS += " /Qvec-report0"  # disables diagnostic information reported by the vectorizer 

    if env['WARN_LEVEL'] == 'strict':
        ICC_FLAGS += " /WX"  # treats warnings as errors

    # Disables the following warnings:
    #
    #  424 : 
    #  537 : 
    #  991 : 
    # 1478 : 
    # 1572 : 
    # 1786 : 
    ICC_FLAGS += " /Qdiag-disable:424,537,991,1478,1572,1786" 

    XILINK_FLAGS  = " /LARGEADDRESSAWARE"

    if export_symbols:
        ICC_FLAGS    += " /debug:full"  # generates complete debug information
        ICC_FLAGS    += " /Zi"          # 
        XILINK_FLAGS += " /DEBUG"

    if env['MODE'] in ['opt', 'profile']:
        ICC_FLAGS += " /Ob2"    # enables inlining of ANY function (compiler discretion)
        ICC_FLAGS += " -Qipo"   # enables interprocedural optimization between files
        ICC_FLAGS += " /G7"     # optimize for latest Intel processors (deprecated)
        ICC_FLAGS += " /QaxW"   # optimize for Intel processors with SSE2 (deprecated)
        ICC_FLAGS += " /MD"     # uses multithreaded DLL runtime library
        ICC_FLAGS += " -O3"     # max optimization level

        if env['MODE'] == 'profile':
            ICC_FLAGS += " /FR"     # Enable browse information

            XILINK_FLAGS += " /FIXED:NO"

        XILINK_FLAGS += " /INCREMENTAL:NO"
    else:
        ICC_FLAGS += " /Od"   # disables all optimizations
        ICC_FLAGS += " /MDd"   # uses *NON-DEBUG* multithreaded DLL runtime library

        XILINK_FLAGS += " /INCREMENTAL"

    env.Append(CCFLAGS = Split(ICC_FLAGS))
    env.Append(LINKFLAGS = Split(XILINK_FLAGS))
   
    if env['MODE'] == 'opt':
        env.Append(CPPDEFINES = Split('NDEBUG'))
# We cannot enable this define, as it will try to use symbols from the debug runtime library  
#   if env['MODE'] == 'debug':
#      env.Append(CPPDEFINES = Split('_DEBUG'))
          
if env['MODE'] == 'debug':
    env.Append(CPPDEFINES = Split('ARNOLD_DEBUG'))

if env['ENABLE_BIFROST'] == 1:
    env.Append(CPPDEFINES=Split('ENABLE_BIFROST'))
if env['ENABLE_ALEMBIC'] == 1:
    env.Append(CPPDEFINES=Split('ENABLE_ALEMBIC'))

if MOD_SUFFIX:
    env.Append(CPPDEFINES={'MOD_SUFFIX':'\\"{}\\"'.format(MOD_SUFFIX.lower())})
## platform related defines
if system.os == 'windows':
    env.Append(CPPDEFINES = Split('_WINDOWS _WIN32 WIN32'))
    env.Append(CPPDEFINES = Split('_WIN64'))
elif system.os == 'darwin':
    env.Append(CPPDEFINES = Split('_DARWIN OSMac_'))
elif system.os == 'linux':
    env.Append(CPPDEFINES = Split('_LINUX'))

## Add path to Arnold API by default
env.Append(CPPPATH = [ARNOLD_API_INCLUDES,])
env.Append(LIBPATH = [ARNOLD_API_LIB, ARNOLD_BINARIES])

## Add Qt includes

mayaQtFolder = ""
mayaQtTarGz = ""

for x in (x for x in os.listdir(MAYA_INCLUDE_PATH) if x.startswith('qt') and  not ( x.endswith('zip') or x.endswith('.gz')) ):
    mayaQtFolder = os.path.join(MAYA_INCLUDE_PATH,x)
    break

for x in (x for x in os.listdir(MAYA_INCLUDE_PATH) if x.startswith('qt') and  ( x.endswith('zip') or x.endswith('.gz'))):
    mayaQtTarGz = os.path.join(MAYA_INCLUDE_PATH,x)
    break

if not mayaQtFolder or not mayaQtTarGz :

    if mayaQtTarGz and os.path.exists(mayaQtTarGz):
        mayaQtFolder = os.path.join(EXTERNAL_PATH, os.path.basename(mayaQtTarGz).replace(".tar.gz", "").replace(".zip", "")) 
        print("found " + mayaQtTarGz + " extracting to " + mayaQtFolder)
    else:
        if (int(maya_version_base) < 2020):
            mayaQtFolder = os.path.join(EXTERNAL_PATH, 'qt-5.6.1-include') 
            mayaQtTarGz = os.path.join(MAYA_INCLUDE_PATH, 'qt-5.6.1-include.tar.gz')
        elif (int(maya_version_base) == 2020):
            mayaQtFolder = os.path.join(EXTERNAL_PATH, 'qt-5.12.5-include') 
            if system.os == 'windows':
                mayaQtTarGz = os.path.join(MAYA_INCLUDE_PATH, 'qt_5.12.5_vc14-include.zip')
            else:
                mayaQtTarGz = os.path.join(MAYA_INCLUDE_PATH, 'qt_5.12.5-include.tar.gz')
        else:
            mayaQtFolder = os.path.join(EXTERNAL_PATH, 'qt-5.15.2-include') 
            if system.os == 'windows':
                mayaQtTarGz = os.path.join(MAYA_INCLUDE_PATH, 'qt_5.15.2_vc14-include.zip')
            else:
                mayaQtTarGz = os.path.join(MAYA_INCLUDE_PATH, 'qt_5.15.2-include.tar.gz')

if not os.path.isdir(mayaQtFolder):
    if os.path.exists(mayaQtTarGz):
        print "Extracting Qt Files..."
        tmpFile, tmpExt = os.path.splitext(mayaQtTarGz)
        if tmpExt == '.zip':
            import zipfile
            with zipfile.ZipFile(mayaQtTarGz, 'r') as zip_ref:
                zip_ref.extractall(mayaQtFolder)
        else:
            import tarfile
            tfile = tarfile.open(mayaQtTarGz, 'r:gz')
            tfile.extractall(mayaQtFolder)
    else:
        print "Error : Qt Files not Found"


env['QT_ROOT_DIR'] = mayaQtFolder
env.Append(CPPPATH = [mayaQtFolder])

## configure base directory for temp files
BUILD_BASE_DIR = os.path.join(env['BUILD_DIR'], '%s_%s' % (system.os, env['TARGET_ARCH']), maya_version, '%s_%s' % (env['COMPILER'], env['MODE']))
env['BUILD_BASE_DIR'] = BUILD_BASE_DIR

if not env['SHOW_CMDS']:
    ## hide long compile lines from the user
    env['CCCOMSTR']     = color_bgreen + 'Compiling $SOURCE ...' + color_reset
    env['SHCCCOMSTR']   = color_bgreen + 'Compiling $SOURCE ...' + color_reset
    env['CXXCOMSTR']    = color_bgreen + 'Compiling $SOURCE ...' + color_reset
    env['SHCXXCOMSTR']  = color_bgreen + 'Compiling $SOURCE ...' + color_reset
    env['LINKCOMSTR']   = color_bred   + 'Linking $TARGET ...'   + color_reset
    env['SHLINKCOMSTR'] = color_bred   + 'Linking $TARGET ...'   + color_reset

if env['MTOA_DISABLE_RV']:
    print 'Disabling Arnold RenderView as MTOA_DISABLE_RV is defined'
    env.Append(CPPDEFINES = Split('MTOA_DISABLE_RV')) 

################################
## BUILD TARGETS
################################

env['BUILDERS']['MakePackage'] = Builder(action = Action(make_package, "Preparing release package: '$TARGET'"))
env['ROOT_DIR'] = os.getcwd()


(USD_CUT_PATH,
 USD_CUT_VERSION,
 USD_CUT_FULL_VERSION,
 USD_CUT_PYTHON,
 USD_CUT_DELEGATE,
 USD_CUT_PROCEDURAL,
 USD_CUT_MAYAUSD_PATH,
 USD_CUT_REGISTRY,
 USD_CUT_LIST_COUNT) = range(9)

USD_VERSIONS = []

USD_DELEGATES = []
USD_DELEGATES_PYTHON2 = []
USD_PROCEDURALS = []
USD_PROCEDURALS_PYTHON2 = []
MAYAUSD_REGISTRIES = []
MAYAUSD_REGISTRIES_PYTHON2 = []
USD_PATHS = env.get('USD_PATH')
USD_PATHS_PYTHON2 = env.get('USD_PATH_PYTHON2')
MAYAUSD_PATHS = env.get('MAYAUSD_PATH')
MAYAUSD_PATHS_PYTHON2 = env.get('MAYAUSD_PATH_PYTHON2')
PATH_SEPARATOR = ';' if system.os == 'windows' else ':'
ENABLE_USD = False

usd_path_list = []
usd_path_python2_count = 0
if int(maya_version_base) >= 2021:
    if USD_PATHS_PYTHON2 and len(USD_PATHS_PYTHON2) > 0:
        USD_PATHS_PYTHON2 = env.subst(USD_PATHS_PYTHON2)
        usd_path_list += USD_PATHS_PYTHON2.split(PATH_SEPARATOR)
        usd_path_python2_count = len(usd_path_list)
    
    if USD_PATHS and len(USD_PATHS) > 0:
        USD_PATHS = env.subst(USD_PATHS)
        usd_path_list += USD_PATHS.split(PATH_SEPARATOR)

env['MTOA_USD_PYTHON'] = '3'

ind = 0
for usd_path in usd_path_list:
    usd_major_version = '0'
    usd_minor_version = '00'
    usd_patch_version = '00'
    usd_version = None
    python_version = '3' if ind >= usd_path_python2_count else '2'
    ind = ind + 1

    # get usd header info
    pxr_h = open(os.path.join(usd_path, 'include', 'pxr', 'pxr.h'), 'r').read()
    r = re.search('PXR_VERSION ([0-9]+)', pxr_h)
    if r:
        usd_version = r.group(1)
    r = re.search('PXR_MAJOR_VERSION ([0-9]+)', pxr_h)
    if r:
        usd_major_version = r.group(1)
    r = re.search('PXR_MINOR_VERSION ([0-9]+)', pxr_h)
    if r:
        usd_minor_version = r.group(1)
    r = re.search('PXR_PATCH_VERSION ([0-9]+)', pxr_h)
    if r:
        usd_patch_version = r.group(1)

    if usd_version:
        # for each path pointing to a usd cut, we add it to the list, as well as its usd version
        found_existing = False
        for usd_cut in USD_VERSIONS:
            if usd_cut[USD_CUT_VERSION] == usd_version and usd_cut[USD_CUT_PYTHON] == python_version:
                found_existing = True
                break

        if not found_existing:
            usd_cut = []
            for i in range(USD_CUT_LIST_COUNT):
                usd_cut.append(None)
            usd_cut[USD_CUT_PATH] = usd_path
            usd_cut[USD_CUT_VERSION] = usd_version
            usd_cut[USD_CUT_FULL_VERSION] = '{}.{}.{}'.format(usd_major_version, usd_minor_version, usd_patch_version)
            usd_cut[USD_CUT_PYTHON] = python_version

            USD_VERSIONS.append(usd_cut)
            
            # used by the proxy shape extension
            if not ENABLE_USD:
                env.Append(CPPDEFINES = Split('ENABLE_USD'))
                ENABLE_USD = True
                if not os.path.exists(os.path.join(BUILD_BASE_DIR, 'usd')):
                    os.makedirs(os.path.join(BUILD_BASE_DIR, 'usd'))


mayausd_path_list = []
mayausd_path_python2_count = 0
if int(maya_version_base) >= 2021:
    if MAYAUSD_PATHS_PYTHON2 and len(MAYAUSD_PATHS_PYTHON2) > 0:
        MAYAUSD_PATHS_PYTHON2 = env.subst(MAYAUSD_PATHS_PYTHON2)
        mayausd_path_list += MAYAUSD_PATHS_PYTHON2.split(PATH_SEPARATOR)
        mayausd_path_python2_count = len(mayausd_path_list)
    
    if MAYAUSD_PATHS and len(MAYAUSD_PATHS) > 0:
        MAYAUSD_PATHS = env.subst(MAYAUSD_PATHS)
        mayausd_path_list += MAYAUSD_PATHS.split(PATH_SEPARATOR)
    if len(mayausd_path_list) == len(usd_path_list) and mayausd_path_python2_count == usd_path_python2_count:
        ind = 0
        for usd_version in USD_VERSIONS:
            usd_version[USD_CUT_MAYAUSD_PATH] = mayausd_path_list[ind]
            ind = ind + 1

mayapy_bin = os.path.join(env['MAYA_ROOT'], 'bin', 'mayapy')

if ENABLE_USD:
    if env['UPDATE_SUBMODULES']:
        print ('updating usd submodule...')
        system.execute('git submodule sync')
        system.execute('git submodule update --init --recursive')
     
        # We need to ensure that jinja2 will be installed through mayapy   
        mayapy_cmd = mayapy_bin + " -m pip install jinja2"
        system.execute(mayapy_cmd)

        # if we're also building for python2 usd modules, then we need to 
        # install jijna2 in the mayapy2 environment
        if usd_path_python2_count > 0:
            mayapy2_bin = os.path.join(env['MAYA_ROOT'], 'bin', 'mayapy2')
            mayapy2_cmd = mayapy2_bin + " -m pip install jinja2"
            system.execute(mayapy2_cmd)

        print ('done')     

if system.os == 'windows':
    maya_env = env.Clone()
    maya_env.Append(CPPPATH = ['.'])
    maya_env.Append(CPPPATH = [MAYA_INCLUDE_PATH])
    maya_env.Append(CPPDEFINES = Split('NT_PLUGIN REQUIRE_IOSTREAM'))
    maya_env.Append(LIBPATH = [os.path.join(MAYA_ROOT, 'lib'),])
    maya_env.Append(LIBS=Split('ai.lib OpenGl32.lib Foundation.lib OpenMaya.lib OpenMayaRender.lib OpenMayaUI.lib OpenMayaAnim.lib OpenMayaFX.lib shell32.lib'))
    maya_env.Append(LIBS = ['Qt5Core.lib', 'Qt5Gui.lib', 'Qt5OpenGL.lib', 'Qt5Widgets.lib'])

    if env['PREBUILT_MTOA']:       
        MTOA_API = [os.path.join(BUILD_BASE_DIR, 'api', 'mtoa_api.dll'), os.path.join(BUILD_BASE_DIR, 'api', 'mtoa_api.lib')]
        MTOA = [os.path.join(BUILD_BASE_DIR, 'mtoa', 'mtoa.dll'), os.path.join(BUILD_BASE_DIR, 'mtoa', 'mtoa.lib')]
        MTOA_SHADERS = [os.path.join(BUILD_BASE_DIR, 'shaders', 'mtoa_shaders.dll')]
        for usd_version in USD_VERSIONS:
            usd_folder = usd_version[USD_CUT_VERSION]
            if usd_version[USD_CUT_PYTHON] == '2':
                usd_folder += '_python2'
            usd_version[USD_CUT_DELEGATE] = os.path.join(BUILD_BASE_DIR, 'usd', 'hydra', usd_folder)
            usd_version[USD_CUT_PROCEDURAL] = os.path.join(BUILD_BASE_DIR, 'usd', 'proc', usd_folder, 'usd_cache_proc.dll')
            if usd_version[USD_CUT_MAYAUSD_PATH]:
                usd_version[USD_CUT_REGISTRY] = os.path.join(BUILD_BASE_DIR, 'usd', 'mayaUsdRegistry', usd_folder, 'mayaUsdRegistry.dll')

    else:
        MTOA_API = env.SConscript(os.path.join('plugins', 'mtoa', 'SConscriptAPI'),
                                            variant_dir = os.path.join(BUILD_BASE_DIR, 'api'),
                                            duplicate = 0,
                                            exports   = 'maya_env')

        MTOA = env.SConscript(os.path.join('plugins', 'mtoa', 'SConscript'),
                                        variant_dir = os.path.join(BUILD_BASE_DIR, 'mtoa'),
                                        duplicate   = 0,
                                        exports     = 'maya_env')
        
        MTOA_SHADERS = env.SConscript(os.path.join('shaders', 'src', 'SConscript'),
                                                    variant_dir = os.path.join(BUILD_BASE_DIR, 'shaders'),
                                                    duplicate   = 0,
                                                    exports     = 'env')

        for usd_version in USD_VERSIONS:
            maya_env['USD_PATH'] = usd_version[USD_CUT_PATH]
            maya_env['MTOA_USD_PYTHON'] = usd_version[USD_CUT_PYTHON]
            maya_env['USD_VERSION'] = usd_version[USD_CUT_VERSION]
            usd_folder = usd_version[USD_CUT_VERSION]
            if usd_version[USD_CUT_PYTHON] == '2':
                usd_folder += '_python2'
            usd_version[USD_CUT_DELEGATE] = env.SConscript(os.path.join('usd', 'hydra', 'SConscript'),
                              variant_dir = os.path.join(BUILD_BASE_DIR, 'usd', 'hydra', usd_folder),
                              duplicate   = 0,
                              exports     = 'maya_env')
            usd_version[USD_CUT_PROCEDURAL] = env.SConscript(os.path.join('usd', 'procedural', 'SConscript'),
                              variant_dir = os.path.join(BUILD_BASE_DIR, 'usd', 'proc', usd_folder),
                              duplicate   = 0,
                              exports     = 'maya_env')
            
            if usd_version[USD_CUT_MAYAUSD_PATH]:
                maya_env['MAYAUSD_PATH'] = usd_version[USD_CUT_MAYAUSD_PATH]
                usd_version[USD_CUT_REGISTRY] = env.SConscript(os.path.join('usd', 'mayaUsdRegistry', 'SConscript'),
                                      variant_dir = os.path.join(BUILD_BASE_DIR, 'usd', 'mayaUsdRegistry', usd_folder),
                                      duplicate   = 0,
                                      exports     = 'maya_env')

    MTOA_PROCS = env.SConscript(os.path.join('procedurals', 'SConscript'),
                                                variant_dir = os.path.join(BUILD_BASE_DIR, 'procedurals'),
                                                duplicate   = 0,
                                                exports     = 'env')
    
else:
    maya_env = env.Clone()
    maya_env.Append(CPPPATH = ['.'])
    maya_env.Append(CPPDEFINES = Split('_BOOL REQUIRE_IOSTREAM'))

    if system.os == 'linux':
        maya_env.Append(CPPPATH = [MAYA_INCLUDE_PATH])
        maya_env.Append(LIBS=Split('GL'))
        maya_env.Append(CPPDEFINES = Split('LINUX'))
        maya_env.Append(LIBPATH = [os.path.join(MAYA_ROOT, 'lib')])
        maya_env.Append(LIBS = ['Qt5Core', 'Qt5Gui', 'Qt5OpenGL', 'Qt5Widgets'])

    elif system.os == 'darwin':
        # MAYA_LOCATION on osx includes Maya.app/Contents
        maya_env.Append(CPPPATH = [MAYA_INCLUDE_PATH])
        maya_env.Append(LIBPATH = [os.path.join(MAYA_ROOT, 'MacOS')])
        maya_env.Append(LIBS = ['Qt5Core.5', 'Qt5Gui.5', 'Qt5OpenGL.5', 'Qt5Widgets.5'])
        
    maya_env.Append(LIBS=Split('ai pthread Foundation OpenMaya OpenMayaRender OpenMayaUI OpenMayaAnim OpenMayaFX'))

    if env['PREBUILT_MTOA']:       
        MTOA_API = [os.path.join(BUILD_BASE_DIR, 'api', 'libmtoa_api' + get_library_extension())]
        if system.os == 'darwin':
            MTOA = [os.path.join(BUILD_BASE_DIR, 'mtoa', 'mtoa.bundle')]
        else:
            MTOA = [os.path.join(BUILD_BASE_DIR, 'mtoa', 'mtoa.so')]
        MTOA_SHADERS = [os.path.join(BUILD_BASE_DIR, 'shaders', 'mtoa_shaders' + get_library_extension())]
        for usd_version in USD_VERSIONS:
            usd_folder = usd_version[USD_CUT_VERSION]
            if usd_version[USD_CUT_PYTHON] == '2':
                usd_folder += '_python2'
            usd_version[USD_CUT_DELEGATE] = os.path.join(BUILD_BASE_DIR, 'usd', 'hydra', usd_folder)
            usd_version[USD_CUT_PROCEDURAL] = os.path.join(BUILD_BASE_DIR, 'usd', 'proc', usd_folder, 'usd_cache_proc' + get_library_extension())
            if usd_version[USD_CUT_MAYAUSD_PATH]:
                usd_version[USD_CUT_REGISTRY] = os.path.join(BUILD_BASE_DIR, 'usd', 'mayaUsdRegistry', usd_folder, 'mayaUsdRegistry'+ get_library_extension())
            
    else:
        MTOA_API = env.SConscript(os.path.join('plugins', 'mtoa', 'SConscriptAPI'),
                                  variant_dir = os.path.join(BUILD_BASE_DIR, 'api'),
                                  duplicate   = 0,
                                  exports     = 'maya_env')

        MTOA = env.SConscript(os.path.join('plugins', 'mtoa', 'SConscript'),
                              variant_dir = os.path.join(BUILD_BASE_DIR, 'mtoa'),
                              duplicate   = 0,
                              exports     = 'maya_env')

        MTOA_SHADERS = env.SConscript(os.path.join('shaders', 'src', 'SConscript'),
                                      variant_dir = os.path.join(BUILD_BASE_DIR, 'shaders'),
                                      duplicate   = 0,
                                      exports     = 'env')

        for usd_version in USD_VERSIONS:
            maya_env['USD_PATH'] = usd_version[USD_CUT_PATH]
            maya_env['MTOA_USD_PYTHON'] = usd_version[USD_CUT_PYTHON]
            maya_env['USD_VERSION'] = usd_version[USD_CUT_VERSION]
            usd_folder = usd_version[USD_CUT_VERSION]
            if usd_version[USD_CUT_PYTHON] == '2':
                usd_folder += '_python2'
            usd_version[USD_CUT_DELEGATE]  = env.SConscript(os.path.join('usd', 'hydra', 'SConscript'),
                              variant_dir = os.path.join(BUILD_BASE_DIR, 'usd', 'hydra', usd_folder),
                              duplicate   = 0,
                              exports     = 'maya_env')
            usd_version[USD_CUT_PROCEDURAL] = env.SConscript(os.path.join('usd', 'procedural', 'SConscript'),
                              variant_dir = os.path.join(BUILD_BASE_DIR, 'usd', 'proc', usd_folder),
                              duplicate   = 0,
                              exports     = 'maya_env')
            if usd_version[USD_CUT_MAYAUSD_PATH]:
                maya_env['MAYAUSD_PATH'] = usd_version[USD_CUT_MAYAUSD_PATH]
                usd_version[USD_CUT_REGISTRY] = env.SConscript(os.path.join('usd', 'mayaUsdRegistry', 'SConscript'),
                              variant_dir = os.path.join(BUILD_BASE_DIR, 'usd', 'mayaUsdRegistry', usd_folder),
                              duplicate   = 0,
                              exports     = 'maya_env')

    MTOA_PROCS = env.SConscript(os.path.abspath(os.path.join('procedurals', 'SConscript')),
                                variant_dir = os.path.join(BUILD_BASE_DIR, 'procedurals'),
                                duplicate   = 0,
                                exports     = 'env')
                                 
    def osx_hardcode_path(target, source, env):
        cmd = None


        if target[0] == MTOA_API[0]:
            cmd = "install_name_tool -id @rpath/libmtoa_api.dylib"
        elif target[0] == MTOA[0]:
            cmd = " install_name_tool -add_rpath @loader_path/../bin/"
            print cmd
            

        if cmd:
            p = subprocess.Popen(cmd + " " + str(target[0]), shell=True)
            retcode = p.wait()

        return 0

    if system.os == 'darwin':
        env.AddPostAction(MTOA_API[0],  Action(osx_hardcode_path, 'Adjusting paths in mtoa_api.dylib ...'))
        env.AddPostAction(MTOA, Action(osx_hardcode_path, 'Adjusting paths in mtoa.boundle ...'))
        #env.AddPostAction(MTOA_SHADERS, Action(osx_hardcode_path, 'Adjusting paths in mtoa_shaders ...'))
        #env.AddPostAction(MTOA_PROCS, Action(osx_hardcode_path, 'Adjusting paths in mtoa_procs ...'))

# Install all USD modules (render delegate and shader registry)
if ENABLE_USD:
    # create the file mayaUsdPlugInfo.json
    jsonFileText = '{\n\t"MayaUsdIncludes":[\n'
    modulesFound = False  
    if not os.path.exists(os.path.join(TARGET_USD_PATH, 'hydra')):
        os.makedirs(os.path.join(TARGET_USD_PATH, 'hydra'))

    for usd_version in USD_VERSIONS:
        if usd_version[USD_CUT_DELEGATE]:
            if modulesFound:
               jsonFileText += ',\n'
            modulesFound = True
            if isinstance(usd_version[USD_CUT_DELEGATE], SCons.Node.NodeList):
                usd_version[USD_CUT_DELEGATE] = usd_version[USD_CUT_DELEGATE][0]
            usd_delegate = usd_version[USD_CUT_DELEGATE]
            jsonFileText += "\t\t{\n\t\t\t\"PlugPath\":\"hydra/"
            jsonFileText += usd_version[USD_CUT_VERSION]
            if usd_version[USD_CUT_PYTHON] == '2':
                jsonFileText += '_python2'
            jsonFileText += "\",\n"
            jsonFileText += '\t\t\t"VersionCheck":{\n'
            jsonFileText += '\t\t\t\t"Python":"'
            jsonFileText += usd_version[USD_CUT_PYTHON]
            jsonFileText += '",\n'
            jsonFileText += '\t\t\t\t"USD":"{}"\n'.format(usd_version[USD_CUT_FULL_VERSION])
            jsonFileText += '\t\t\t}\n\t\t}'
            env.Install(os.path.join(TARGET_USD_PATH, 'hydra'), usd_delegate)

        if usd_version[USD_CUT_PROCEDURAL]:
            if isinstance(usd_version[USD_CUT_PROCEDURAL], SCons.Node.NodeList):
                usd_version[USD_CUT_PROCEDURAL] = usd_version[USD_CUT_PROCEDURAL][0]
            usd_procedural = usd_version[USD_CUT_PROCEDURAL]
            usd_folder = usd_version[USD_CUT_VERSION]
            if usd_version[USD_CUT_PYTHON] == '2':
                usd_folder += '_python2'

            proc_folder = os.path.join(TARGET_EXTENSION_PATH, 'usd', usd_folder)
            if not os.path.exists(proc_folder):
                os.makedirs(proc_folder)
            env.Install(proc_folder, usd_procedural)

        if usd_version[USD_CUT_REGISTRY]:
            if isinstance(usd_version[USD_CUT_REGISTRY], SCons.Node.NodeList):
                usd_version[USD_CUT_REGISTRY] = usd_version[USD_CUT_REGISTRY][0]
            usd_registry = usd_version[USD_CUT_REGISTRY]
            usd_folder = usd_version[USD_CUT_VERSION]
            if usd_version[USD_CUT_PYTHON] == '2':
                usd_folder += '_python2'
            if modulesFound:
                jsonFileText += ',\n'
            modulesFound = True
            jsonFileText += "\t\t{\n\t\t\t\"PlugPath\":\"mayaUsdRegistry/"
            jsonFileText += usd_folder            
            jsonFileText += "\",\n"
            jsonFileText += '\t\t\t"VersionCheck":{\n'
            jsonFileText += '\t\t\t\t"Python":"'
            jsonFileText += usd_version[USD_CUT_PYTHON]
            jsonFileText += '",\n'
            jsonFileText += '\t\t\t\t"USD":"{}"\n'.format(usd_version[USD_CUT_FULL_VERSION])
            jsonFileText += '\t\t\t}\n\t\t}'

            env.Install(os.path.join(TARGET_USD_PATH, 'mayaUsdRegistry', usd_folder), usd_registry)
            env.Install(os.path.join(TARGET_USD_PATH, 'mayaUsdRegistry', usd_folder), os.path.join(BUILD_BASE_DIR, 'usd', 'mayaUsdRegistry', usd_folder, 'plugInfo.json'))
            env.Install(os.path.join(TARGET_USD_PATH, 'mayaUsdRegistry', usd_folder), os.path.join(BUILD_BASE_DIR, 'usd', 'mayaUsdRegistry', usd_folder, 'mayaUsdRegistry'))

    jsonFileText += '\n\t]\n}'
    jsonFilePath = os.path.join(BUILD_BASE_DIR, 'usd', 'mayaUsdPlugInfo.json')
    jsonFile = open(jsonFilePath, 'w')
    jsonFile.write(jsonFileText)
    jsonFile.close()
    env.Install(os.path.join(TARGET_USD_PATH), os.path.join(BUILD_BASE_DIR, 'usd', 'mayaUsdPlugInfo.json'))


Depends(MTOA, MTOA_API[0])
Depends(MTOA, ARNOLD_API_LIB)


SConscriptChdir(0)
TESTSUITE = env.SConscript(os.path.join('testsuite', 'SConscript'),
                           variant_dir = os.path.join(BUILD_BASE_DIR, 'testsuite'),
                           duplicate   = 0,
                           exports     = 'env BUILD_BASE_DIR MTOA MTOA_SHADERS ')

MTOA_API_DOCS = env.SConscript('docs/doxygen_api/SConscript',
                     variant_dir = os.path.join(BUILD_BASE_DIR, 'docs', 'mtoa'),
                     duplicate   = 0,
                     exports     = 'env BUILD_BASE_DIR')
SConscriptChdir(1)

env.Install(TARGET_PLUGIN_PATH, os.path.join('plugins', 'mtoa', 'mtoa.mtd'))
if not env['DISABLE_COMMON']:
    env.Install(TARGET_SHADER_PATH, os.path.join('shaders', 'mtoa_shaders.mtd'))


if system.os == 'windows':
    # Rename plugins as .mll and install them in the target path
    mtoa_new = os.path.splitext(str(MTOA[0]))[0] + '.mll'
    env.Command(mtoa_new, str(MTOA[0]), Copy("$TARGET", "$SOURCE"))
    env.Install(TARGET_PLUGIN_PATH, [mtoa_new])
    env.Install(TARGET_SHADER_PATH, MTOA_SHADERS[0])
    nprocs = []
    for proc in MTOA_PROCS:
        if str(proc)[-3:] == 'dll':
            nprocs.append(proc)
    MTOA_PROCS = nprocs
    env.Install(env['TARGET_PROCEDURAL_PATH'], MTOA_PROCS)
    
    libs = MTOA_API[1]
    env.Install(env['TARGET_LIB_PATH'], libs)

    # on windows, also install the ai.lib file
    env.Install(os.path.join(TARGET_LIB_PATH), glob.glob(os.path.join(ARNOLD_API_LIB, '*')))
else:
    env.Install(TARGET_PLUGIN_PATH, MTOA)
    env.Install(TARGET_SHADER_PATH, MTOA_SHADERS)
    env.Install(env['TARGET_PROCEDURAL_PATH'], MTOA_PROCS)
    if system.os == 'linux':
        libs = glob.glob(os.path.join(ARNOLD_API_LIB, '*.so'))
    else:
        libs = glob.glob(os.path.join(ARNOLD_API_LIB, '*.dylib'))

dylibs = glob.glob(os.path.join(ARNOLD_BINARIES, '*%s' % get_library_extension()))
dylibs += glob.glob(os.path.join(ARNOLD_BINARIES, '*%s' % get_executable_extension()))
dylibs += glob.glob(os.path.join(ARNOLD_BINARIES, '*%s.*' % get_library_extension()))
dylibs += glob.glob(os.path.join(ARNOLD_BINARIES, '*%s.*' % get_executable_extension()))


if int(maya_version_base) < 2024:
    # install syncolor packages until Maya 2023
    syncolor_library_path = os.path.join(env['ROOT_DIR'], 'external', 'synColor', 'lib', system.os)
    if (system.os == 'linux'):
        # on linux the version number is after ".so."
        env.Install(env['TARGET_BINARIES'], glob.glob(syncolor_library_path + "/"+ get_library_prefix() + "synColor"+get_library_extension()+".*"))
    else:
        env.Install(env['TARGET_BINARIES'], glob.glob(syncolor_library_path + "/"+ get_library_prefix() + "synColor*"+get_library_extension()))

if env['ENABLE_AXFTOA']:
    if (system.os == 'linux'):
        env.Install(env['TARGET_BINARIES'], glob.glob(os.path.join(ARNOLD_AXF_LIB, "*")))
    else:
        env.Install(env['TARGET_BINARIES'], glob.glob(os.path.join(ARNOLD_AXF_LIB, "*%s" % get_library_extension())))

env.Install(os.path.join(env['TARGET_MODULE_PATH'], 'license'), glob.glob(os.path.join(ARNOLD, 'license', '*')))

env.Install(env['TARGET_BINARIES'], dylibs)

env.Install(os.path.join(env['TARGET_MODULE_PATH'], 'osl'), glob.glob(os.path.join(ARNOLD, 'osl', '*')))
env.Install(os.path.join(env['TARGET_MODULE_PATH'], 'materialx'), glob.glob(os.path.join(ARNOLD, 'materialx', '*')))
env.Install(os.path.join(env['TARGET_MODULE_PATH'], 'ocio'), glob.glob(os.path.join(ARNOLD, 'ocio', '*')))
# install all arnold sdk headers
env.Install(os.path.join(TARGET_INCLUDE_PATH, 'arnold'), glob.glob(os.path.join(ARNOLD, 'include', '*')))


env.Install(TARGET_PLUGINS_PATH, glob.glob(os.path.join(ARNOLD, 'plugins', "*")))
if os.path.exists(os.path.join(os.path.join(ARNOLD, 'plugins', 'usd'))):
    env.Install(os.path.join(env['TARGET_MODULE_PATH'], 'plugins','usd'), glob.glob(os.path.join(ARNOLD, 'plugins', 'usd', '*')))

# if env['ENABLE_BIFROST'] and int(maya_version) >= 201800 :
#     env.Install(os.path.join(TARGET_EXTENSION_PATH, 'bifrost', '1.5.0'), glob.glob(os.path.join(env['ROOT_DIR'], 'external', 'bifrost', '1.5.0', system.os, '*')))

if not env['MTOA_DISABLE_RV']:
    RENDERVIEW_DYLIB = get_library_prefix() + 'ai_renderview'+ get_library_extension()
    if int(maya_version_base) >= 2021:
        RENDERVIEW_DYLIBPATH = os.path.join(EXTERNAL_PATH, 'renderview', 'lib', '2022', RENDERVIEW_DYLIB)
    else:
        RENDERVIEW_DYLIBPATH = os.path.join(EXTERNAL_PATH, 'renderview', 'lib', RENDERVIEW_DYLIB)
    
    env.Install(env['TARGET_BINARIES'], glob.glob(RENDERVIEW_DYLIBPATH))


env.Install(env['TARGET_BINARIES'], MTOA_API[0])

# install mtoa common scritps
scriptfiles = find_files_recursive(os.path.join('scripts', 'mtoa'), ['.py', '.mel', '.ui', '.xml', '.qss', '.txt', '.json', 'README'])
env.InstallAs([os.path.join(TARGET_PYTHON_PATH, 'mtoa', x) for x in scriptfiles],
              [os.path.join('scripts', 'mtoa', x) for x in scriptfiles])

# install mtoa version specific scripts (must be done after to allow overwriting)
versionfiles = find_files_recursive(os.path.join('scripts', maya_version_base), ['.py', '.mel'])
env.InstallAs([os.path.join(TARGET_PYTHON_PATH, 'mtoa', maya_version_base, x) for x in versionfiles],
              [os.path.join('scripts', maya_version_base, x) for x in versionfiles])

# install Arnold python bindings
arpybds = find_files_recursive(ARNOLD_PYTHON, ['.py'])
env.InstallAs([os.path.join(TARGET_PYTHON_PATH, x) for x in arpybds],
              [os.path.join(ARNOLD_PYTHON, x) for x in arpybds])


def GetViewportShaders(maya_version):
    vp2ShadersList = []
    vp2ShaderExtensions = ['.xml', '.cgfx', '.fx', '.ogsfx']
    
    if system.os == 'windows':
        vp2ShaderExtensions.append('.hlsl')
    vp2shaders = find_files_recursive(os.path.join('plugins', 'mtoa', 'viewport2'), vp2ShaderExtensions)

    if  int(maya_version_base) < 2020:
        indices = [i for i, s in enumerate (vp2shaders) if '2020' in s ]
        for index in indices:
            del vp2shaders[index]

    for vp2shader in vp2shaders:
        # If the version is >= 2020, replace aiRectangleAreaLight.xml from the usual vp2 directory
        # with the one that's under vp2/2020. 
        # In this case , we're adding both via find_files_recursive and removing the older file. 

        if vp2shader.find('2020') >= 0:
            vp2ShadersList.remove("aiRectangleAreaLight.xml")
        vp2ShadersList.append(vp2shader)

    return vp2ShadersList

vp2Shaders = GetViewportShaders(maya_version)

for vp2Shader in vp2Shaders:
    vpTargetShader = vp2Shader.replace('2020/', '')
    vpTargetShader = vpTargetShader.replace('2020\\', '')
    env.InstallAs([os.path.join(TARGET_VP2_PATH, vpTargetShader)], [os.path.join('plugins', 'mtoa', 'viewport2', vp2Shader)])

# install include files
apibasepath = os.path.join('plugins', 'mtoa')
apiheaders = [
                os.path.join('attributes', 'AttrHelper.h'),
                #os.path.join('attributes', 'Components.h'),
                #os.path.join('common', 'MObjectCompare.h'),
                os.path.join('common', 'UtilityFunctions.h'),
                os.path.join('common', 'UnorderedContainer.h'),
                os.path.join('extension', 'Extension.h'),
                #os.path.join('extension', 'ExtensionsManager.h'),
                #os.path.join('extension', 'AbMayaNode.h'),
                os.path.join('extension', 'AbTranslator.h'),
                #os.path.join('extension', 'PxUtils.h'),
                #os.path.join('extension', 'PxMayaNode.h'),
                #os.path.join('extension', 'PxArnoldNode.h'),
                #os.path.join('extension', 'PxTranslator.h'),
                #os.path.join('extension', 'PathUtils.h'),
                os.path.join('platform', 'Platform.h'),
                os.path.join('platform', 'darwin', 'Event.h'),
                os.path.join('platform', 'linux', 'Event.h'),
                os.path.join('platform', 'win32', 'Event.h'),
                os.path.join('platform', 'win32', 'dirent.h'),
                os.path.join('platform', 'win32', 'Debug.h'),
                #os.path.join('render', 'AOV.h'),
                #os.path.join('render', 'RenderSession.h'),
                #os.path.join('render', 'RenderOptions.h'),
                #os.path.join('scene', 'MayaScene.h'),
                #os.path.join('session', 'ArnoldSession.h'),
                os.path.join('session', 'SessionOptions.h'),
                #os.path.join('session', 'ArnoldLightLinks.h'),
                os.path.join('translators', 'NodeTranslator.h'),
                os.path.join('translators', 'AutoDagTranslator.h'),
                os.path.join('translators', 'DagTranslator.h'),
                #os.path.join('translators', 'ObjectSetTranslator.h'),
                os.path.join('translators', 'camera', 'CameraTranslator.h'),
                os.path.join('translators', 'camera', 'AutoCameraTranslator.h'),
                os.path.join('translators', 'driver', 'DriverTranslator.h'),
                os.path.join('translators', 'filter', 'FilterTranslator.h'),
                os.path.join('translators', 'light', 'LightTranslator.h'),
                #os.path.join('translators', 'options', 'OptionsTranslator.h'),
                os.path.join('translators', 'shader', 'ShaderTranslator.h'),
                os.path.join('translators', 'shape', 'ShapeTranslator.h'),
                os.path.join('translators', 'shape', 'ProceduralTranslator.h'),
                os.path.join('utils', 'Version.h'),
                os.path.join('utils', 'Universe.h'),
                os.path.join('utils', 'MtoaLog.h'),
                os.path.join('utils', 'time.h'),
                os.path.join('utils', 'HashUtils.h'),
                os.path.join('viewport2', 'ArnoldProceduralSubSceneOverride.h')

]

env.InstallAs([os.path.join(TARGET_INCLUDE_PATH, 'mtoa', x) for x in apiheaders],
              [os.path.join(apibasepath, x) for x in apiheaders])
             
              
# install icons
env.Install(TARGET_ICONS_PATH, glob.glob(os.path.join('icons', '*.xpm')))
env.Install(TARGET_ICONS_PATH, glob.glob(os.path.join('icons', '*.png')))
env.Install(TARGET_ICONS_PATH, glob.glob(os.path.join('icons', '*.svg')))
# install MtoA docs
docfiles = find_files_recursive(os.path.join(BUILD_BASE_DIR, 'docs', 'mtoa', 'html'), None)
env.InstallAs([os.path.join(TARGET_DOC_PATH, x) for x in docfiles],
              [os.path.join(BUILD_BASE_DIR, 'docs', 'mtoa', 'html', x) for x in docfiles])

# install Arnold docs
docfiles = find_files_recursive(os.path.join(ARNOLD, 'doc', 'html'), None)
env.InstallAs([os.path.join(TARGET_MODULE_PATH, 'docs', 'arnold', x) for x in docfiles],
              [os.path.join(ARNOLD, 'doc', 'html', x) for x in docfiles])

env.Install(TARGET_MODULE_PATH, glob.glob(os.path.join('docs', 'readme.txt')))
# install presets
presetfiles = find_files_recursive(os.path.join('presets'), ['.mel'])
env.InstallAs([os.path.join(TARGET_PRESETS_PATH, x) for x in presetfiles],
              [os.path.join('presets', x) for x in presetfiles])
# install renderer description
env.Install(TARGET_DESCR_PATH, glob.glob(os.path.join('scripts', 'arnoldRenderer.xml')))
env.Install(TARGET_MODULE_PATH, glob.glob(os.path.join('scripts', 'arnoldRenderer.xml')))

env.MakeModule(TARGET_MODULE_PATH, os.path.join(BUILD_BASE_DIR, 'mtoa.mod'))
env.Install(TARGET_MODULE_PATH, os.path.join(BUILD_BASE_DIR, 'mtoa.mod'))

maya_base_version = maya_version[:4]

## Sets release package name based on MtoA version, architecture and compiler used.
##
package_name = "MtoA-" + MTOA_VERSION + "-" + system.os + "-" + maya_base_version + PACKAGE_SUFFIX

if env['MODE'] in ['debug', 'profile']:
    package_name += '-' + env['MODE']

package_name_inst = package_name



PACKAGE = env.MakePackage(package_name, MTOA + MTOA_API + MTOA_SHADERS + MTOA_PROCS + MTOA_API_DOCS)
#PACKAGE = env.MakePackage(package_name, MTOA + MTOA_API + MTOA_SHADERS)

import ftplib

def deploy(target, source, env):

    def ftp_send_binary_cb(block):
        print "\b#",

    local_package_name = str(source[0])
    
    server = env['FTP']

    ftp = ftplib.FTP(server)

    ftp.login(env['FTP_USER'], env['FTP_PASS'])

    directory = env['FTP_SUBDIR']

    directory_split = directory.split('/')

    for d in directory_split:
        try:
            ftp.cwd(d)
        except:
            ftp.mkd(d)
            ftp.cwd(d)    

    f = open(os.path.abspath(local_package_name), 'rb')
    print 'Sending "%s" to %s/%s...' % (source[0], server, directory)
    command = "STOR %s" % local_package_name
    try:
        ftp.storbinary(command, f, 81920, ftp_send_binary_cb)
    except:
        # Old python versions have no ftp callback
        ftp.storbinary(command, f, 81920)
    print

    f.close()
    ftp.close()

env['BUILDERS']['PackageDeploy']  = Builder(action = Action(deploy,  "Deploying release package: '$SOURCE'"))

################################
## EXTENSIONS
################################

ext_env = maya_env.Clone()

# In theory we should always build our extensions against the resulting "target" includes folder. 
# However, when there are changes in the API files they aren't always updated before the build starts. 
# We might have been doing something wrong here,
# but for now we will only build extensions against target API include folder if this variable is defined.
# It is important to try that regularly, in order to make sure our extensions aren't cheating by including more
# files than what they're supposed to
if env['BUILD_EXT_TARGET_INCLUDES'] == 1:
    ext_env.Append(CPPPATH = [env['ARNOLD_API_INCLUDES']])
    # Instead of including our whole MtoA folder, we should just include what's provided in the public API
    ext_env.Append(CPPPATH = [os.path.join(TARGET_INCLUDE_PATH, 'mtoa')])
else:
    ext_env.Append(CPPPATH = ['plugin', os.path.join(maya_env['ROOT_DIR'], 'plugins', 'mtoa'), env['ARNOLD_API_INCLUDES']])
    

ext_env.Append(LIBPATH = ['.', ARNOLD_API_LIB, ARNOLD_BINARIES])
ext_env.Append(LIBPATH = [ os.path.join(maya_env['ROOT_DIR'], os.path.split(str(MTOA[0]))[0]),
                           os.path.join(maya_env['ROOT_DIR'], os.path.split(str(MTOA_API[0]))[0])])
ext_env.Append(LIBS = ['mtoa_api',])

ext_base_dir = os.path.join('contrib', 'extensions')

for ext in os.listdir(ext_base_dir):
    #Only build extensions if they are requested by user
    if not ((ext in COMMAND_LINE_TARGETS) or ('%spack' % ext in COMMAND_LINE_TARGETS) or ('%sdeploy' % ext in COMMAND_LINE_TARGETS) or
            (ext == 'xgen') or
            (ext == 'xgenSpline') or
            (ext == 'hairPhysicalShader') or
            (env['ENABLE_BIFROST'] == 1 and ext == bifrost_ext) or
            (ext == 'lookdevkit') or
            (ext == 'renderSetup') or 
            (ext == 'synColor') or
            (ext == 'usdProxyShape') or
            (ext == 'usdPreviewSurface') or
            (env['ENABLE_GPU_CACHE'] == 1 and ext == 'gpuCache') or
            (env['ENABLE_BIFROST_GRAPH'] == 1 and ext == 'bifrostGraph')):
        continue
    ext_dir = os.path.join(ext_base_dir, ext)

    if os.path.isdir(ext_dir):

        EXT = env.SConscript(os.path.join(ext_dir, 'SConscript'),
                             variant_dir = os.path.join(BUILD_BASE_DIR, ext),
                             duplicate   = 0,
                             exports     = ['ext_env', 'env'])

        if len(EXT) >= 2:
            EXT_SHADERS = EXT[1] 
        if len(EXT) == 3:
            EXT_PROCS = EXT[2]

        # EXT may contain a shader result
        ext_arnold = None
        target_type = 'shader'
        ext_files = []
        if len(EXT) > 1:
            ext_arnold = str(EXT[1][0])
            if hasattr(EXT[1], 'target_type'):
                target_type = EXT[1].target_type
            plugin = str(EXT[0][0])
        else:
            plugin = str(EXT[0])
        ext_files.append(plugin)
        
        pluginDir = os.path.join(ext_dir, 'plugin')
        pyfiles = glob.glob(pluginDir+"/*.py")
        for pyfile  in pyfiles:
            if os.path.exists(pyfile):
                ext_files.append(pyfile)
                env.Install(TARGET_EXTENSION_PATH, pyfile)

        mtdfiles = glob.glob(pluginDir+"/*.mtd")
        for mtdfile  in mtdfiles:
            if os.path.exists(mtdfile):
                ext_files.append(mtdfile)
                env.Install(TARGET_EXTENSION_PATH, mtdfile)
        
        pymodules = glob.glob(pluginDir+"/*/__init__.py")
        for pymodule in pymodules:
            if os.path.exists(pymodule):
                moddir = os.path.dirname(pymodule)
                modirname = os.path.basename(moddir)
                for modpy in glob.glob(moddir+"/*.py"):
                    ext_files.append(modpy)
                    env.Install(os.path.join(TARGET_EXTENSION_PATH, modirname), modpy)

#TODO XGEN: figure out the proper place these can go so that they always override the maya scripts

        # install extension override scripts
        scriptsDir = os.path.join(ext_dir, 'scripts')
        extensionFiles = find_files_recursive(scriptsDir, ['.py', '.mel'])
        for extFile  in extensionFiles:
            overrideScriptsDir = os.path.join(TARGET_PYTHON_PATH, 'mtoa', maya_version_base)
            env.Install(overrideScriptsDir, os.path.join(ext_dir, 'scripts',extFile))

        if ext_arnold and (target_type == 'shader'):
            mtdfile = os.path.splitext(os.path.basename(ext_arnold))[0] + '.mtd'
            mtdfile = os.path.join(ext_dir, 'shaders', mtdfile)
            if os.path.exists(mtdfile):
                ext_files.append(mtdfile)
                env.Install(TARGET_SHADER_PATH, mtdfile)
        env.Install(TARGET_EXTENSION_PATH, plugin)
        package_files = []
        if ext_arnold:
            target_path = "shaders"
            if target_type == 'procedural':
                env.Install(TARGET_PROCEDURAL_PATH, ext_arnold)
                target_path = "shaders"
            elif ext == 'synColor':
                # Syncolor is an exception and should be installed in plugins/
                env.Install(os.path.join(TARGET_MODULE_PATH, 'plugins'), ext_arnold)
            else:
                env.Install(TARGET_SHADER_PATH, ext_arnold)
            package_files += [[ext_arnold, target_path]]
        if ext_arnold:
            if len(EXT) == 3:
                procedural = EXT[2]
                ext_files.append(procedural)
                env.Install(TARGET_PROCEDURAL_PATH, procedural)
                package_files += [[str(EXT[2][0]), 'shaders']]    
        for p in ext_files:
            package_files += [[p, 'extensions']]
        local_env = env.Clone()
        local_env['PACKAGE_FILES'] = package_files
        extension_package_name = '%s-MtoA-%s-%s-%s%s' % (ext, MTOA_VERSION, system.os, maya_base_version, PACKAGE_SUFFIX)
        EXT_PACKAGE = local_env.MakePackage(extension_package_name, EXT)        
        top_level_alias(local_env, '%spack' % ext, EXT_PACKAGE)        
        local_env.AlwaysBuild(EXT_PACKAGE)
        top_level_alias(env, ext, EXT)
        EXT_PACKAGE_DEPLOY = local_env.PackageDeploy('%sdeploy' % ext, extension_package_name)
        top_level_alias(env, '%sdeploy' % ext, EXT_PACKAGE_DEPLOY)
        Depends(EXT, MTOA_API[0])
        Depends(EXT_PACKAGE, EXT)
        Depends(EXT_PACKAGE_DEPLOY, EXT_PACKAGE)

## Specifies the files that will be included in the release package.
## List items have 2 or 3 elements, with 3 possible formats:
##
## (source_file, destination_path [, new_file_name])    Copies file to destination path, optionally renaming it
## (source_dir, destination_dir)                        Recursively copies the source directory as destination_dir
## (file_spec, destination_path)                        Copies a group of files specified by a glob expression
##
PACKAGE_FILES = [
[os.path.join(BUILD_BASE_DIR, 'mtoa.mod'), '.'],
[os.path.join('icons', '*.xpm'), 'icons'],
[os.path.join('icons', '*.png'), 'icons'],
[os.path.join('icons', '*.svg'), 'icons'],
[os.path.join('scripts', '*.xml'), '.'],
[MTOA_API[0], 'bin'],
[os.path.join(ARNOLD_BINARIES, 'kick%s' % get_executable_extension()), 'bin'],
[os.path.join(ARNOLD_BINARIES, '*%s' % get_library_extension()), 'bin'],
[os.path.join(ARNOLD_BINARIES, '*.png'), 'bin'],
[os.path.join(ARNOLD_BINARIES, '*.lic'), 'bin'],
[os.path.join(ARNOLD_BINARIES, '*.pit'), 'bin'],
[os.path.join(ARNOLD_BINARIES, '*.txt'), 'bin'],
[os.path.join(ARNOLD_BINARIES, 'oslc%s' % get_executable_extension()), 'bin'],
[os.path.join(ARNOLD_BINARIES, 'oslinfo%s' % get_executable_extension()), 'bin'],
[os.path.join(ARNOLD_BINARIES, 'noice%s' % get_executable_extension()), 'bin'],
[os.path.join(ARNOLD_BINARIES, 'oiiotool%s' % get_executable_extension()), 'bin'],
[os.path.join('plugins', 'mtoa', 'mtoa.mtd'), 'plug-ins'],
[MTOA_SHADERS[0], 'shaders'],
[os.path.splitext(str(MTOA_API[0]))[0] + '.lib', 'lib'],
[os.path.join('docs', 'readme.txt'), '.'],
# [os.path.join(ARNOLD, 'osl'), os.path.join('osl', 'include')],
[os.path.join(ARNOLD, 'include'), os.path.join('include', 'arnold')],
[os.path.join(ARNOLD, 'plugins', '*'), os.path.join('plugins')],
]

materialx_files = find_files_recursive(os.path.join(ARNOLD, 'materialx'), None)
for p in materialx_files:
    (d, f) = os.path.split(p)
    PACKAGE_FILES += [
        [os.path.join(ARNOLD, 'materialx', p), os.path.join('materialx', d)]
    ]

ocio_files = find_files_recursive(os.path.join(ARNOLD, 'ocio'), None)
for p in ocio_files:
    (d, f) = os.path.split(p)
    PACKAGE_FILES += [
        [os.path.join(ARNOLD, 'ocio', p), os.path.join('ocio', d)]
    ]
osl_files = find_files_recursive(os.path.join(ARNOLD, 'osl'), None)
for p in osl_files:
    (d, f) = os.path.split(p)
    PACKAGE_FILES += [
        [os.path.join(ARNOLD, 'osl', p), os.path.join('osl', d)]
    ]

if os.path.exists(os.path.join(ARNOLD, 'plugins', 'usd')):
    usd_resources = find_files_recursive(os.path.join(ARNOLD, 'plugins', 'usd'), None)
    for p in usd_resources:
        (d, f) = os.path.split(p)
        PACKAGE_FILES += [
            [os.path.join(ARNOLD, 'plugins', 'usd', p), os.path.join('plugins', 'usd', d)]
        ]
if system.os == 'darwin':
    license_manager_app = find_files_recursive(os.path.join(ARNOLD_BINARIES, 'ArnoldLicenseManager.app'), None)
    for p in license_manager_app:
        (d, f) = os.path.split(p)
        PACKAGE_FILES += [
            [os.path.join(ARNOLD_BINARIES, 'ArnoldLicenseManager.app', p), os.path.join('bin', 'ArnoldLicenseManager.app', d)]
        ]
else:
    PACKAGE_FILES += [
        [os.path.join(ARNOLD_BINARIES, 'ArnoldLicenseManager%s' % get_executable_extension()), 'bin']
    ]


for p in presetfiles:
    (d, f) = os.path.split(p)
    PACKAGE_FILES += [
        [os.path.join('presets', p), os.path.join('presets', d)]
    ]

PACKAGE_FILES.append([os.path.join(ARNOLD_BINARIES, 'maketx%s' % get_executable_extension()), 'bin'])


if int(maya_version_base) < 2024:
    # we also need to copy the syncolor dylib, for syncolor extension
    # FIXME couldn't this be done in the extension script ?
    syncolor_library_path = os.path.join(EXTERNAL_PATH, 'synColor', 'lib', system.os)
    if (system.os == 'linux'):
        # on linux the syncolor version number is after ".so."
        syncolor_files = glob.glob(syncolor_library_path + "/"+ get_library_prefix() + "synColor"+get_library_extension()+".*")
    else:
        syncolor_files = glob.glob(syncolor_library_path + "/"+ get_library_prefix() + "synColor*"+get_library_extension())

    for syncolor_file in syncolor_files:
        PACKAGE_FILES.append([syncolor_file, 'bin'])
        

PACKAGE_FILES.append([os.path.join('installer', 'RSTemplates', '*.json'), 'RSTemplates'])

PACKAGE_FILES.append([os.path.join(ARNOLD, '*.txt'), 'bin'])

if env['ENABLE_AXFTOA']:
    if (system.os == 'linux'):
        PACKAGE_FILES.append([os.path.join(ARNOLD_AXF_LIB, '*' ), 'bin'])
    else:
        PACKAGE_FILES.append([os.path.join(ARNOLD_AXF_LIB, '*%s' % get_library_extension()), 'bin'])

PACKAGE_FILES.append([os.path.join(ARNOLD, 'license', 'pit', '*'), 'license'])

if system.os == 'windows':
    PACKAGE_FILES.append([os.path.join(ARNOLD_API_LIB, '*'), 'lib'])
    PACKAGE_FILES.append([os.path.join(EXTERNAL_PATH, 'vcredist', '*'), ''])

license_files = find_files_recursive(os.path.join(ARNOLD, 'license'), None)
for p in license_files:
    (d, f) = os.path.split(p)
    PACKAGE_FILES += [
        [os.path.join(ARNOLD, 'license', p), os.path.join('license', d)]
    ]

vp2shaders = GetViewportShaders(maya_version)
installedVp2Shaders = []
for vp2shader in vp2shaders:
    installedVp2Shaders.append( os.path.join('plugins', 'mtoa', 'viewport2', vp2shader ))

for vp2shader in installedVp2Shaders:
    PACKAGE_FILES.append([vp2shader, 'vp2'])

# Adding MtoA docs
docfiles = find_files_recursive(os.path.join(BUILD_BASE_DIR, 'docs', 'mtoa', 'html'), None)
for p in docfiles:
    (d, f) = os.path.split(p)
    PACKAGE_FILES += [
        [os.path.join(BUILD_BASE_DIR, 'docs', 'mtoa', 'html', p), os.path.join('docs', 'mtoa', d)]
    ]
# Adding Arnold docs
docfiles = find_files_recursive(os.path.join(ARNOLD, 'doc', 'html'), None)
for p in docfiles:
    (d, f) = os.path.split(p)
    PACKAGE_FILES += [
        [os.path.join(ARNOLD, 'doc', 'html', p), os.path.join('docs', 'arnold', d)]
    ]

if ENABLE_USD:
    PACKAGE_FILES += [[os.path.join(BUILD_BASE_DIR, 'usd', 'mayaUsdPlugInfo.json'), 'usd']]
    for usd_version in USD_VERSIONS:
        usd_folder = usd_version[USD_CUT_VERSION]
        if usd_version[USD_CUT_PYTHON] == '2':
            usd_folder += '_python2'

        usd_delegate = usd_version[USD_CUT_DELEGATE]
        if usd_delegate:
            hydrafolder = str(usd_delegate)
            hydrafiles = find_files_recursive(hydrafolder, ['.dll', '.so', '.dylib', '.json', '.usda'])
            for p in hydrafiles:
                (d, f) = os.path.split(p)
                PACKAGE_FILES += [[os.path.join(hydrafolder, p), os.path.join('usd', 'hydra', usd_folder, d)]]

        usd_procedural = usd_version[USD_CUT_PROCEDURAL]
        if usd_procedural:
            PACKAGE_FILES += [[usd_procedural, os.path.join('extensions', 'usd', usd_folder)]]

        usd_registry = usd_version[USD_CUT_REGISTRY]
        if usd_registry:
            registryfolder = os.path.join(BUILD_BASE_DIR, 'usd', 'mayaUsdRegistry', usd_folder)
            registryfiles = find_files_recursive(registryfolder, ['.dll', '.so', '.dylib', '.json'])
            for p in registryfiles:
                (d, f) = os.path.split(p)
                PACKAGE_FILES += [[os.path.join(registryfolder, p), os.path.join('usd', 'mayaUsdRegistry', usd_folder, d)]]


PACKAGE_FILES.append([os.path.join(BUILD_BASE_DIR, 'xgen', 'xgen_procedural%s' % get_library_extension()), 'procedurals'])
PACKAGE_FILES.append([os.path.join(BUILD_BASE_DIR, 'xgen', 'xgenTranslator%s' % get_library_extension()), 'extensions'])
PACKAGE_FILES.append([os.path.join('contrib', 'extensions', 'xgen', 'plugin', '*.py'), 'extensions'])
PACKAGE_FILES.append([os.path.join('contrib', 'extensions', 'xgen', 'plugin', '*.mtd'), 'extensions'])
  
PACKAGE_FILES.append([os.path.join(BUILD_BASE_DIR, 'xgenSpline', 'xgenSpline_procedural%s' % get_library_extension()), 'procedurals'])
PACKAGE_FILES.append([os.path.join(BUILD_BASE_DIR, 'xgenSpline', 'xgenSplineTranslator%s' % get_library_extension()), 'extensions'])
PACKAGE_FILES.append([os.path.join(BUILD_BASE_DIR, 'xgenSpline', 'xgenSpline_shaders%s' % get_library_extension()), 'shaders'])
PACKAGE_FILES.append([os.path.join('contrib', 'extensions', 'xgenSpline', 'plugin', '*.py'), 'extensions'])
PACKAGE_FILES.append([os.path.join('contrib', 'extensions', 'xgenSpline', 'plugin', '*.mtd'), 'extensions'])
    
PACKAGE_FILES.append([os.path.join(BUILD_BASE_DIR, 'hairPhysicalShader', 'hairPhysicalShaderTranslator%s' % get_library_extension()), 'extensions'])
PACKAGE_FILES.append([os.path.join(BUILD_BASE_DIR, 'hairPhysicalShader', 'hairPhysicalShader_shaders%s' % get_library_extension()), 'shaders'])
PACKAGE_FILES.append([os.path.join('contrib', 'extensions', 'hairPhysicalShader', 'plugin', '*.py'), 'extensions'])

PACKAGE_FILES.append([os.path.join(BUILD_BASE_DIR, 'usdProxyShape', 'usdProxyShapeTranslator%s' % get_library_extension()), 'extensions'])
PACKAGE_FILES.append([os.path.join('contrib', 'extensions', 'usdProxyShape', 'plugin', '*.py'), 'extensions'])
PACKAGE_FILES.append([os.path.join('contrib', 'extensions', 'usdProxyShape', 'plugin', '*.mtd'), 'extensions'])

PACKAGE_FILES.append([os.path.join(BUILD_BASE_DIR, 'usdPreviewSurface', 'usdPreviewSurfaceTranslator%s' % get_library_extension()), 'extensions'])
if env['ENABLE_BIFROST'] == 1:
    PACKAGE_FILES.append([os.path.join(BUILD_BASE_DIR, bifrost_ext, 'bifrostTranslator%s' % get_library_extension()), 'extensions'])
    PACKAGE_FILES.append([os.path.join('contrib', 'extensions', bifrost_ext, 'plugin', '*.py'), 'extensions'])

    if bifrost_ext == 'bifrost':
        PACKAGE_FILES.append([os.path.join(EXTERNAL_PATH, 'bifrost', '1.5.0', system.os), os.path.join('extensions', 'bifrost', '1.5.0')])
    else:
         #PACKAGE_FILES.append([os.path.join(EXTERNAL_PATH, 'bifrost', 'bifrost_procedural_0_1%s' % get_library_extension()), 'procedurals'])
    #else:
        PACKAGE_FILES.append([os.path.join(BUILD_BASE_DIR, bifrost_ext, 'bifrost_procedurals%s' % get_library_extension()), 'procedurals'])
        PACKAGE_FILES.append([os.path.join(BUILD_BASE_DIR,  bifrost_ext, 'bifrost_shaders%s' % get_library_extension()), 'shaders'])


if env['ENABLE_BIFROST_GRAPH'] == 1:
    PACKAGE_FILES.append([os.path.join(BUILD_BASE_DIR, 'bifrostGraph', 'bifShapeTranslator%s' % get_library_extension()), 'extensions'])
    PACKAGE_FILES.append([os.path.join('contrib', 'extensions', 'bifrostGraph', 'plugin', '*.py'), 'extensions'])

PACKAGE_FILES.append([os.path.join(BUILD_BASE_DIR, 'lookdevkit', 'lookdevkit%s' % get_library_extension()), 'extensions'])
PACKAGE_FILES.append([os.path.join(BUILD_BASE_DIR, 'lookdevkit', 'lookdevkit_shaders%s' % get_library_extension()), 'shaders'])

PACKAGE_FILES.append([os.path.join(BUILD_BASE_DIR, 'renderSetup', 'renderSetup%s' % get_library_extension()), 'extensions'])
PACKAGE_FILES.append([os.path.join(BUILD_BASE_DIR, 'renderSetup', 'renderSetup_shaders%s' % get_library_extension()), 'shaders'])

PACKAGE_FILES.append([os.path.join(BUILD_BASE_DIR, 'synColor', 'synColorTranslator%s' % get_library_extension()), 'extensions'])
if int(maya_version_base) < 2024:
    PACKAGE_FILES.append([os.path.join(BUILD_BASE_DIR, 'synColor', 'synColor_shaders%s' % get_library_extension()), 'plugins'])

if env['ENABLE_GPU_CACHE'] == 1:
    PACKAGE_FILES.append([os.path.join(BUILD_BASE_DIR, 'gpuCache', 'gpuCacheTranslator%s' % get_library_extension()), 'extensions'])
    PACKAGE_FILES.append([os.path.join('contrib', 'extensions', 'gpuCache', 'plugin', '*.py'), 'extensions'])


for p in MTOA_PROCS:
    PACKAGE_FILES += [[p, 'procedurals']]

if not env['DISABLE_COMMON']:
    PACKAGE_FILES.append([os.path.join('shaders', 'mtoa_shaders.mtd'), 'shaders'])

for p in scriptfiles:
    (d, f) = os.path.split(p)
    PACKAGE_FILES += [
        [os.path.join('scripts', 'mtoa', p), os.path.join('scripts', 'mtoa', d)]
    ]

for p in versionfiles:
    (d, f) = os.path.split(p)
    PACKAGE_FILES += [
        [os.path.join('scripts', maya_version_base, p), os.path.join('scripts', 'mtoa', maya_version_base, d)]
    ]

for p in arpybds:
    (d, f) = os.path.split(p)
    PACKAGE_FILES += [
        [os.path.join(ARNOLD_PYTHON, p), os.path.join('scripts', d)]
    ]
   
for p in apiheaders:
    (d, f) = os.path.split(p)
    PACKAGE_FILES += [
        [os.path.join('plugins', 'mtoa', p), os.path.join('include', 'mtoa', d)]
    ]

if system.os == 'windows':
    PACKAGE_FILES += [
        [MTOA[0], 'plug-ins', 'mtoa.mll'],
    ]
elif system.os == 'linux':
    PACKAGE_FILES += [
        [MTOA[0], 'plug-ins'],
        [os.path.join(ARNOLD_BINARIES, '*%s.*' % get_library_extension()), 'bin'],
    ]
elif system.os == 'darwin':
    PACKAGE_FILES += [
       [MTOA[0], 'plug-ins'],
    ]


if not env['MTOA_DISABLE_RV']:
    PACKAGE_FILES.append([RENDERVIEW_DYLIBPATH, 'bin'])

env['PACKAGE_FILES'] = PACKAGE_FILES
installer_name = ''

mode_token = '' if env['MODE'] == 'opt' else '-%s' % env['MODE']

if system.os == "windows":
    installer_name = 'MtoA-%s-%s%s%s.exe' % (MTOA_VERSION, maya_base_version, PACKAGE_SUFFIX, mode_token)
elif system.os == "darwin":
    installer_name = 'MtoA-%s-%s-%s%s%s.zip' % (MTOA_VERSION, system.os, maya_base_version, PACKAGE_SUFFIX, mode_token)
else:
    installer_name = 'MtoA-%s-%s-%s%s%s.run' % (MTOA_VERSION, system.os, maya_base_version, PACKAGE_SUFFIX, mode_token)

def create_installer(target, source, env):

    import tempfile
    import shutil
    tempdir = tempfile.mkdtemp() # creating a temporary directory for the makeself.run to work
    shutil.copyfile(os.path.abspath('installer/MtoAEULA.txt'), os.path.join(tempdir, 'MtoAEULA.txt'))
    shutil.copyfile(os.path.join(ARNOLD, 'EULA.txt'), os.path.join(tempdir, 'EULA.txt'))

    if system.os == "windows":
        import zipfile
        shutil.copyfile(os.path.abspath('installer/arnold.ico'), os.path.join(tempdir, 'arnold.ico'))
        shutil.copyfile(os.path.abspath('installer/left.bmp'), os.path.join(tempdir, 'left.bmp'))
        shutil.copyfile(os.path.abspath('installer/top.bmp'), os.path.join(tempdir, 'top.bmp'))
        shutil.copyfile(os.path.abspath('installer/MtoAEULA.txt'), os.path.join(tempdir, 'MtoAEULA.txt'))
        copiedNsi = os.path.join(tempdir, 'MtoA.nsi')
        shutil.copyfile(os.path.abspath('installer/MtoA.nsi'), copiedNsi)

        if MOD_SUFFIX:
            # edit the Mtoa.nsi installer to add an additional mod installation suffix
            nsiFile = open(copiedNsi, "rt")
            nsiFileData = nsiFile.read()
            modBegin = nsiFileData.find('; mod begin')
            modEnd = nsiFileData.find('; mod end')

            editedNsiData = nsiFileData[:modEnd]
            modSection = nsiFileData[modBegin:modEnd]
            editedNsiData += modSection.replace('AYA_VERSION%', 'AYA_VERSION%{}'.format(MOD_SUFFIX))
            editedNsiData += nsiFileData[modEnd:]
            nsiFileData = editedNsiData

            uninstallBegin = nsiFileData.find('; uninstall begin')
            uninstallEnd = nsiFileData.find('; uninstall end')
            editedNsiData = nsiFileData[:uninstallEnd]
            uninstallSection = nsiFileData[uninstallBegin:uninstallEnd]
            editedNsiData += uninstallSection.replace('AYA_VERSION%', 'AYA_VERSION%{}'.format(MOD_SUFFIX))
            editedNsiData += nsiFileData[uninstallEnd:]

            nsiFileData = editedNsiData
            nsiFile.close()
            nsiFile = open(copiedNsi, "wt")
            nsiFile.write(nsiFileData)
            nsiFile.close()
            
        zipfile.ZipFile(os.path.abspath('%s.zip' % package_name), 'r').extractall(tempdir)
        NSIS_PATH = env.subst(env['NSIS_PATH'])
        os.environ['NSISDIR'] = NSIS_PATH
        os.environ['NSISCONFDIR'] = NSIS_PATH
        mtoaVersionString = MTOA_VERSION
        mtoaVersionString = mtoaVersionString.replace('.dev', ' Dev')
        mtoaVersionString = mtoaVersionString.replace('.RC', ' RC')
        mayaVersionString = maya_base_version
        
        os.environ['MTOA_VERSION_NAME'] = mtoaVersionString
        os.environ['MAYA_VERSION'] = mayaVersionString

        # run script on each of the packaged files
        signed_extensions = ['.exe', '.dll', '.lib', '.mll']
        excluded_files = [] #['OpenColorIO.dll']
        sign_packaged_file(env['SIGN_COMMAND'], tempdir, signed_extensions, excluded_files)

        subprocess.call([os.path.join(NSIS_PATH, 'makensis.exe'), '/V3', copiedNsi])
        sign_packaged_file(env['SIGN_COMMAND'], os.path.join(tempdir, 'MtoA.exe'), signed_extensions)

        shutil.copyfile(os.path.join(tempdir, 'MtoA.exe'), installer_name)
    elif system.os == "darwin":
        import zipfile
        maya_version = maya_version_base
        shutil.copyfile(os.path.abspath('installer/MtoA_'+maya_version+'_Installer.pkgproj'), os.path.join(tempdir, 'MtoA_Installer.pkgproj'))
        shutil.copyfile(os.path.abspath('installer/top.jpg'), os.path.join(tempdir, 'top.jpg'))
        zipfile.ZipFile(os.path.abspath('%s.zip' % package_name), 'r').extractall(os.path.join(tempdir, maya_version))
        subprocess.call(['chmod', 'a+x', os.path.join(tempdir, maya_version, 'bin', 'kick')])
        subprocess.call(['chmod', 'a+x', os.path.join(tempdir, maya_version, 'bin', 'maketx')])
        subprocess.call(['chmod', 'a+x', os.path.join(tempdir, maya_version, 'bin', 'oslc')])
        subprocess.call(['chmod', 'a+x', os.path.join(tempdir, maya_version, 'bin', 'oslinfo')])
        subprocess.call(['chmod', 'a+x', os.path.join(tempdir, maya_version, 'license', 'lmutil')])
        subprocess.call(['chmod', 'a+x', os.path.join(tempdir, maya_version, 'license', 'rlmutil')])
        subprocess.call(['chmod', 'a+x', os.path.join(tempdir, maya_version, 'bin', 'noice')])

        subprocess.call(['chmod', 'a+x', os.path.join(tempdir, maya_version, 'bin', 'ArnoldLicenseManager.app', 'Contents', 'MacOS', 'ArnoldLicenseManager')])
                
        mtoaMod = open(os.path.join(tempdir, maya_version, 'mtoa.mod'), 'w')
        
        installerFiles = glob.glob(os.path.join(tempdir, maya_version, 'license', 'installer', '*'))
        for installerFile in installerFiles:
            subprocess.call(['chmod', 'a+x', installerFile])

        installPath = '/Applications/Autodesk/Arnold/mtoa/' + maya_version
        mtoaMod.write('+ mtoa any %s\n' % installPath)
        mtoaMod.write('PATH +:= bin\n')
        mtoaMod.write('MAYA_CUSTOM_TEMPLATE_PATH +:= scripts/mtoa/ui/templates\n')
        mtoaMod.write('MAYA_SCRIPT_PATH +:= scripts/mtoa/mel\n')
        mtoaMod.write('MAYA_RENDER_DESC_PATH += %s\n' % installPath)
        mtoaMod.write('MAYA_PXR_PLUGINPATH_NAME += %s/usd\n' % installPath)
        mtoaMod.close()

        postScript = open(os.path.join(tempdir, 'post_install.sh'), 'w')
        postScript.write('#!/usr/bin/env bash\n')
        

        postCommand = "POST_FILE=$2/Applications/Autodesk/Arnold/mtoa/%s/license/ArnoldLicensing-8.1.0.1084_RC6-darwin.dmg\n" % maya_version
        postScript.write(postCommand)
        postScript.write('if [ -e $POST_FILE ]; then\n')
        postCommand = "  hdiutil attach $2/Applications/Autodesk/Arnold/mtoa/%s/license/ArnoldLicensing-8.1.0.1084_RC6-darwin.dmg\n" % maya_version
        postScript.write(postCommand)
        postScript.write('else\n')
        postCommand = "  hdiutil attach $3/Applications/Autodesk/Arnold/mtoa/%s/license/ArnoldLicensing-8.1.0.1084_RC6-darwin.dmg\n" % maya_version
        postScript.write(postCommand)
        postScript.write('fi\n')
        postCommand = "/Volumes/ArnoldLicensing/ArnoldLicensing-8.1.0.951_RC6-darwin.app/Contents/MacOS/ArnoldLicensing-8.1.0.1084_RC6-darwin --silent\n"
        postScript.write(postCommand)
        postCommand = "hdiutil detach /Volumes/ArnoldLicensing"
        postScript.write(postCommand)        
       
        if MOD_SUFFIX:
            mod_suffix_folder = maya_version + MOD_SUFFIX
            postScript.write('\n')
            postCommand = "SUFFIX_FOLDER=/Users/Shared/Autodesk/modules/maya/%s\n" % mod_suffix_folder
            postScript.write(postCommand)
            postCommand = "if [ ! -e $SUFFIX_FOLDER ]; then\n"
            postScript.write(postCommand)
            postScript.write("mkdir $SUFFIX_FOLDER\n")
            postScript.write('fi\n')
            postCommand = "cp /Users/Shared/Autodesk/modules/maya/%s/mtoa.mod /Users/Shared/Autodesk/modules/maya/%s/mtoa.mod" % (maya_version, mod_suffix_folder)
            postScript.write(postCommand)

        ### Add the LicenseUpdater 
        postScript.write('\n')
        postCommand = "  chmod +x $2/Applications/Autodesk/Arnold/mtoa/%s/license/LicensingUpdater\n" % maya_version
        postScript.write(postCommand)
        postCommand = "$2/Applications/Autodesk/Arnold/mtoa/%s/license/LicensingUpdater\n" % maya_version
        postScript.write(postCommand)
        postCommand = 'mkdir -p -m 777 "/Library/Application Support/Reprise"'
        postScript.write(postCommand)
        postScript.close()

        postScript = open(os.path.join(tempdir, 'empty_script.sh'), 'w')
        postScript.write('#!/usr/bin/env bash\n')
        postScript.close()


        signed_extensions = ['.dylib', '.pkg', '.exe', '.bundle']
        excluded_files = [] #['libOpenColorIO.1.dylib']
        sign_packaged_file(env['SIGN_COMMAND'], tempdir, signed_extensions, excluded_files)
        subprocess.call(['packagesbuild', os.path.join(tempdir, 'MtoA_Installer.pkgproj')])
        sign_packaged_file(env['SIGN_COMMAND'], os.path.join(tempdir, 'MtoA_Installer.pkgproj'), signed_extensions)

        shutil.copyfile(os.path.join(tempdir, 'MtoA_Setup.pkg'), installer_name[:-4]+'.pkg')
        

    else:
        shutil.copyfile(os.path.abspath('%s.zip' % package_name), os.path.join(tempdir, "package.zip"))
        shutil.copyfile(os.path.abspath('installer/unix_installer.py'), os.path.join(tempdir, 'unix_installer.py'))
        commandFilePath = os.path.join(tempdir, 'unix_installer.sh')
        commandFile = open(commandFilePath, 'w')
        commandFile.write("#!/bin/bash")
        absPathCD = """
ABSPATH=$(readlink -f "$0")
ABSDIR=$(dirname "$ABSPATH")
cd $ABSDIR
"""
        commandFile.write(absPathCD)
        # we don't need the line below, but it allows to ensure this script is modified for
        # each new release and allows ODIS to understand this is a new version #MTOA-663
        commandFile.write('# MtoA %s\n' % (MTOA_VERSION)) 
        python3Check = """
cmd="python3";
if ! command -v $cmd &> /dev/null;then
  cmd="python";
fi
"""
        commandFile.write(python3Check)
        commandFile.write('$cmd $ABSDIR/unix_installer.py %s %s $*' % (maya_base_version, platform.system().lower()))
        
        commandFile.write("exit 0")
        commandFile.close()
        subprocess.call(['chmod', '+x', commandFilePath])
        shutil.copyfile(commandFilePath, './unix_installer.sh')
        installerPath = os.path.abspath('./%s' % (installer_name))
        subprocess.call(['installer/makeself.sh', tempdir, installerPath,
                         'MtoA for Linux Installer', './unix_installer.sh'])
        subprocess.call(['chmod', '+x', installerPath])

    print "Clearing temporary folder %s" % tempdir
    shutil.rmtree(tempdir)

env['BUILDERS']['PackageInstaller'] = Builder(action = Action(create_installer,  "Creating installer for package: '$SOURCE'"))

if system.os == 'linux':
    def check_compliance(target, source, env):
        REFERENCE_API_LIB = env['REFERENCE_API_LIB']
        REFERENCE_API_VERSION = env['REFERENCE_API_VERSION']
        CURRENT_API_LIB = os.path.abspath(str(source[0]))
        print REFERENCE_API_LIB
        subprocess.call(['abi-dumper', REFERENCE_API_LIB, '-lver', REFERENCE_API_VERSION, '-o', 'old.dump'])
        subprocess.call(['abi-dumper', CURRENT_API_LIB, '-lver', MTOA_VERSION, '-o', 'new.dump'])
        subprocess.call(['abi-compliance-checker', '-l', 'libmtoa_api', '-old', 'old.dump', '-new', 'new.dump'])

    env['BUILDERS']['ComplianceChecker'] = Builder(action = Action(check_compliance, "Checking compliance for file: '$SOURCE'"))
    COMPLIANCECHECKER = env.ComplianceChecker('check_compliance', MTOA_API[0])
    top_level_alias(env, 'check_compliance', COMPLIANCECHECKER)

INSTALLER = env.PackageInstaller('create_installer', package_name)
DEPLOY = env.PackageDeploy('deploy', installer_name)

################################
## TARGETS ALIASES AND DEPENDENCIES
################################

aliases = []
aliases.append(env.Alias('install-module',  env['TARGET_MODULE_PATH']))
aliases.append(env.Alias('install-python',  env['TARGET_PYTHON_PATH']))
aliases.append(env.Alias('install-include',  env['TARGET_INCLUDE_PATH']))
aliases.append(env.Alias('install-icons',   env['TARGET_ICONS_PATH']))
aliases.append(env.Alias('install-descr',   env['TARGET_DESCR_PATH']))
aliases.append(env.Alias('install-lib',     env['TARGET_LIB_PATH']))
aliases.append(env.Alias('install-bin',     env['TARGET_BINARIES']))
aliases.append(env.Alias('install-plugins', env['TARGET_PLUGIN_PATH']))
aliases.append(env.Alias('install-shaders', env['TARGET_SHADER_PATH']))
aliases.append(env.Alias('install-ext',     env['TARGET_EXTENSION_PATH']))
aliases.append(env.Alias('install-usd_delegate', env['TARGET_USD_PATH']))

top_level_alias(env, 'mtoa', MTOA)
top_level_alias(env, 'docs', MTOA_API_DOCS)
top_level_alias(env, 'shaders', MTOA_SHADERS)
top_level_alias(env, 'testsuite', TESTSUITE)
top_level_alias(env, 'install', aliases)
top_level_alias(env, 'pack', PACKAGE)
top_level_alias(env, 'deploy', DEPLOY)
top_level_alias(env, 'installer', INSTALLER)

env.Depends(INSTALLER, PACKAGE)
env.Depends(DEPLOY, INSTALLER)

env.AlwaysBuild(PACKAGE)

#env.AlwaysBuild('install')
Default(['mtoa', 'shaders'])

## Process top level aliases into the help message
Help('''%s

Top level targets:
    %s

Individual tests can be run using the 'test_nnnn' target.
Note that this folder must fall within the TEST_PATTERN glob.
''' % (vars.GenerateHelpText(env), get_all_aliases()))

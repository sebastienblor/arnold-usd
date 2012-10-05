# vim: filetype=python

## first we extend the module path to load our own modules
import sys, os
sys.path = ["tools/python"] + sys.path

import system, glob
from build_tools import *
from solidangle_tools import *

import SCons

from colorama import init
init()
from colorama import Fore, Back, Style

MTOA_VERSION = get_mtoa_version(3)

################################################################################
#   Operating System detection
################################################################################

if system.os() == 'darwin':
   ALLOWED_COMPILERS = ('gcc',)   # Do not remove this comma, it's magic
   arnold_default_api_lib = os.path.join('$ARNOLD', 'bin')
elif system.os() == 'linux':
   ALLOWED_COMPILERS = ('gcc',)   # Do not remove this comma, it's magic
   # linux conventions would be to actually use lib for dynamic libraries!
   arnold_default_api_lib = os.path.join('$ARNOLD', 'bin')
elif system.os() == 'windows':
   ALLOWED_COMPILERS = ('msvc', 'icc')
   arnold_default_api_lib = os.path.join('$ARNOLD', 'lib')
else:
   print "Unknown operating system: %s" % system.os()
   Exit(1)

################################################################################
#   Build system options
################################################################################

vars = Variables('custom.py')
vars.AddVariables(
      ## basic options
      EnumVariable('MODE'       , 'Set compiler configuration', 'debug'             , allowed_values=('opt', 'debug', 'profile')),
      EnumVariable('WARN_LEVEL' , 'Set warning level'         , 'strict'            , allowed_values=('strict', 'warn-only', 'none')),
      EnumVariable('COMPILER'   , 'Set compiler to use'       , ALLOWED_COMPILERS[0], allowed_values=ALLOWED_COMPILERS),
      BoolVariable('MULTIPROCESS','Enable multiprocessing in the testsuite', True),
      BoolVariable('SHOW_CMDS'  , 'Display the actual command lines used for building', False),
      PathVariable('LINK', 'Linker to use', None),
      PathVariable('SHCC', 'Path to C++ (gcc) compiler used', None),
      PathVariable('SHCXX', 'Path to C++ (gcc) compiler used for generating shared-library objects', None),
                  
      BoolVariable('COLOR_CMDS' , 'Display colored output messages when building', True),
      EnumVariable('SHOW_TEST_OUTPUT', 'Display the test log as it is being run', 'single', allowed_values=('always', 'never', 'single')),
      BoolVariable('UPDATE_REFERENCE', 'Update the reference log/image for the specified targets', False),
      ('TEST_THREADS' , 'Number of simultaneous tests to run', 4),
      ('TEST_PATTERN' , 'Glob pattern of tests to be run', 'test_*'),
      ('GCC_OPT_FLAGS', 'Optimization flags for gcc', '-O3 -funroll-loops'),

      PathVariable('MAYA_ROOT',
                   'Directory where Maya is installed (defaults to $MAYA_LOCATION)', 
                   get_default_path('MAYA_LOCATION', '.')),
      PathVariable('EXTERNAL_PATH',
                   'External dependencies are found here', 
                   '.', PathVariable.PathIsDir),
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
      PathVariable('TARGET_EXTENSION_PATH', 
                   'Path used for installation of mtoa translator extensions', 
                   os.path.join('$TARGET_MODULE_PATH', 'extensions'), PathVariable.PathIsDirCreate),
      PathVariable('TARGET_LIB_PATH', 
                   'Path for libraries', 
                   os.path.join('$TARGET_MODULE_PATH', 'lib'), PathVariable.PathIsDirCreate),
      PathVariable('TARGET_DOC_PATH', 
                   'Path for documentation', 
                   os.path.join('$TARGET_MODULE_PATH', 'docs'), PathVariable.PathIsDirCreate),                  
      PathVariable('TARGET_BINARIES', 
                   'Path for libraries', 
                   os.path.join('$TARGET_MODULE_PATH', 'bin'), PathVariable.PathIsDirCreate),
      PathVariable('SHAVE_API', 
                   'Where to find Shave API', 
                   '.', PathVariable.PathIsDir)
)

if system.os() == 'windows':
   vars.Add(EnumVariable('MSVC_VERSION', 'Version of MS Visual Studio to use', '9.0', allowed_values=('8.0', '8.0Exp', '9.0', '9.0Exp', '10.0')))

if system.os() == 'windows':
   # Ugly hack. Create a temporary environment, without loading any tool, so we can set the MSVC_ARCH
   # variable from the contents of the TARGET_ARCH variable. Then we can load tools.
   tmp_env = Environment(variables = vars, tools=[])
   tmp_env.Append(MSVC_ARCH = 'amd64')
   env = tmp_env.Clone(tools=['default'])
   # restore as the Clone overrides it
   env['TARGET_ARCH'] = 'x86_64'
else:
   env = Environment(variables = vars)

if env['TARGET_MODULE_PATH'] == '.':
   print "Please define TARGET_MODULE_PATH (Path used for installation of the mtoa plugin)"
   Exit(1)

env.Append(BUILDERS = {'MakeModule' : make_module})

system.set_target_arch('x86_64')

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
if system.os() == 'darwin':
    MAYA_INCLUDE_PATH = os.path.join(MAYA_ROOT, '../../devkit/include')
else:
    MAYA_INCLUDE_PATH = os.path.join(MAYA_ROOT, 'include')
EXTERNAL_PATH = env.subst(env['EXTERNAL_PATH'])
ARNOLD = env.subst(env['ARNOLD'])
ARNOLD_API_INCLUDES = env.subst(env['ARNOLD_API_INCLUDES'])
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
TARGET_EXTENSION_PATH = env.subst(env['TARGET_EXTENSION_PATH']) 
TARGET_LIB_PATH = env.subst(env['TARGET_LIB_PATH'])  
TARGET_DOC_PATH = env.subst(env['TARGET_DOC_PATH'])  
TARGET_BINARIES = env.subst(env['TARGET_BINARIES']) 
SHAVE_API = env.subst(env['SHAVE_API'])

# Get arnold and maya versions used for this build
arnold_version    = get_arnold_version(os.path.join(ARNOLD_API_INCLUDES, 'ai_version.h'))
maya_version      = get_maya_version(os.path.join(MAYA_INCLUDE_PATH, 'maya', 'MTypes.h'))
maya_version_base = maya_version[0:4]

# print build info
print ''
print 'Building       : ' + 'MtoA %s' % (MTOA_VERSION)
print 'Arnold version : %s' % arnold_version
print 'Maya version   : %s' % maya_version
print 'Mode           : %s' % (env['MODE'])
print 'Host OS        : %s' % (system.os())
print 'Host arch.     : %s' % (system.host_arch())
print 'Target arch.   : %s' % (system.target_arch())
print 'SCons          : %s' % (SCons.__version__)
print ''


################################
## COMPILER OPTIONS
################################

## Generic Windows stuff476
if system.os() == 'windows':
   # Embed manifest in executables and dynamic libraries
   env['LINKCOM'] = [env['LINKCOM'], 'mt.exe -nologo -manifest ${TARGET}.manifest -outputresource:$TARGET;1']
   env['SHLINKCOM'] = [env['SHLINKCOM'], 'mt.exe -nologo -manifest ${TARGET}.manifest -outputresource:$TARGET;2']

export_symbols = env['MODE'] in ['debug', 'profile']

if env['COMPILER'] == 'gcc':
   # env.Append(CXXFLAGS = Split('-fno-rtti'))

   if env['MODE'] == 'opt': 
      env.Append(CPPDEFINES = Split('NDEBUG')) 

   ## Hide all internal symbols (the ones without AI_API decoration)
   env.Append(CCFLAGS = Split('-fvisibility=hidden'))
   env.Append(CXXFLAGS = Split('-fvisibility=hidden'))
   env.Append(LINKFLAGS = Split('-fvisibility=hidden'))

   ## Hardcode '.' directory in RPATH in linux
   if system.os() == 'linux':
      env.Append(LINKFLAGS = Split('-z origin') )
      env.Append(RPATH = env.Literal(os.path.join('\\$$ORIGIN', '..', 'bin')))

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
      if system.os() == 'darwin': 
         env.Append(CCFLAGS = Split('-gstabs')) 
         env.Append(LINKFLAGS = Split('-gstabs')) 
      else: 
         env.Append(CCFLAGS = Split('-g -fno-omit-frame-pointer')) 
         env.Append(LINKFLAGS = Split('-g')) 
   if system.os() == 'linux' and env['MODE'] == 'profile':
      env.Append(CCFLAGS = Split('-pg'))
      env.Append(LINKFLAGS = Split('-pg'))

   if system.os() == 'darwin':
      ## tell gcc to compile a 64 bit binary
      env.Append(CCFLAGS = Split('-arch x86_64'))
      env.Append(LINKFLAGS = Split('-arch x86_64'))

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
      MSVC_FLAGS += " /Zi"  # generates complete debug information

   LINK_FLAGS  = " /MANIFEST"

   if env['MODE'] in ['opt', 'profile']:
      MSVC_FLAGS += " /Ob2"    # enables inlining of ANY function (compiler discretion)
      MSVC_FLAGS += " /GL"     # enables whole program optimization
      MSVC_FLAGS += " /MD"     # uses multithreaded DLL runtime library
      MSVC_FLAGS += " /Ox"     # selects maximum optimization
      
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
      env.Append(CPPDEFINES = Split('_CRTDBG_MAP_ALLOC'))

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

## platform related defines
if system.os() == 'windows':
   env.Append(CPPDEFINES = Split('_WINDOWS _WIN32 WIN32'))
   env.Append(CPPDEFINES = Split('_WIN64'))
elif system.os() == 'darwin':
   env.Append(CPPDEFINES = Split('_DARWIN OSMac_'))
elif system.os() == 'linux':
   env.Append(CPPDEFINES = Split('_LINUX'))

## Add path to Arnold API by default
env.Append(CPPPATH = [ARNOLD_API_INCLUDES,])
env.Append(LIBPATH = [ARNOLD_API_LIB, ARNOLD_BINARIES])
   
## configure base directory for temp files
BUILD_BASE_DIR = os.path.join('build', '%s_%s' % (system.os(), system.target_arch()), maya_version, '%s_%s' % (env['COMPILER'], env['MODE']))

if not env['SHOW_CMDS']:
   ## hide long compile lines from the user
   env['CCCOMSTR']     = color_bgreen + 'Compiling $SOURCE ...' + color_reset
   env['SHCCCOMSTR']   = color_bgreen + 'Compiling $SOURCE ...' + color_reset
   env['CXXCOMSTR']    = color_bgreen + 'Compiling $SOURCE ...' + color_reset
   env['SHCXXCOMSTR']  = color_bgreen + 'Compiling $SOURCE ...' + color_reset
   env['LINKCOMSTR']   = color_bred   + 'Linking $TARGET ...'   + color_reset
   env['SHLINKCOMSTR'] = color_bred   + 'Linking $TARGET ...'   + color_reset

################################
## BUILD TARGETS
################################

env['BUILDERS']['MakePackage'] = Builder(action = Action(make_package, "Preparing release package: '$TARGET'"))
env['ROOT_DIR'] = os.getcwd()

if system.os() == 'windows':
   maya_env = env.Clone()
   maya_env.Append(CPPPATH = ['.'])
   maya_env.Append(CPPPATH = [MAYA_INCLUDE_PATH])
   maya_env.Append(CPPDEFINES = Split('NT_PLUGIN REQUIRE_IOSTREAM'))
   maya_env.Append(LIBPATH = [os.path.join(MAYA_ROOT, 'lib'),])
   
   maya_env.Append(LIBS=Split('ai.lib OpenGl32.lib glu32.lib Foundation.lib OpenMaya.lib OpenMayaRender.lib OpenMayaUI.lib OpenMayaAnim.lib OpenMayaFX.lib'))
   
   [MTOA_API, MTOA_API_PRJ] = env.SConscript(os.path.join('plugins', 'mtoa', 'SConscriptAPI'),
                                             variant_dir = os.path.join(BUILD_BASE_DIR, 'api'),
                                             duplicate = 0,
                                             exports   = 'maya_env')
   
   [MTOA, MTOA_PRJ] = env.SConscript(os.path.join('plugins', 'mtoa', 'SConscript'),
                                     variant_dir = os.path.join(BUILD_BASE_DIR, 'mtoa'),
                                     duplicate = 0,
                                     exports   = 'maya_env')

   [MTOA_SHADERS, MTOA_SHADERS_PRJ] = env.SConscript(os.path.join('shaders', 'src', 'SConscript'),
                                                     variant_dir = os.path.join(BUILD_BASE_DIR, 'shaders'),
                                                     duplicate = 0,
                                                     exports   = 'env')

   [MTOA_PROCS, MTOA_PROCS_PRJ] = env.SConscript(os.path.join('procedurals', 'SConscript'),
                                                     variant_dir = os.path.join(BUILD_BASE_DIR, 'procedurals'),
                                                     duplicate = 0,
                                                     exports   = 'env')

   INSTALL_PRJ = env.MSVSProject(target = 'install' + env['MSVS']['PROJECTSUFFIX'],
                                 srcs = [],
                                 incs = [],
                                 buildtarget = 'install',
                                 cmdargs = ['-Q -s COMPILER=msvc MODE=debug TARGET_ARCH=x86_64',
                                            '-Q -s COMPILER=icc MODE=debug TARGET_ARCH=x86_64',
                                            '-Q -s COMPILER=msvc MODE=opt TARGET_ARCH=x86_64',
                                            '-Q -s COMPILER=icc MODE=opt TARGET_ARCH=x86_64'],
                                 variant = ['Debug_MSVC|Win64',
                                            'Debug_ICC|Win64',
                                            'Opt_MSVC|Win64',
                                            'Opt_ICC|Win64'],
                                 auto_build_solution = 0,
                                 nokeep = 1)
   
   SOLUTION = env.MSVSSolution(target = 'mtoa' + env['MSVS']['SOLUTIONSUFFIX'],
                               projects = [os.path.join('plugins', 'mtoa', 'mtoa') + env['MSVS']['PROJECTSUFFIX'],
                                           os.path.join('plugins', 'mtoa', 'mtoa_api') + env['MSVS']['PROJECTSUFFIX'],
                                           os.path.join('shaders', 'src', 'mtoa_shaders') + env['MSVS']['PROJECTSUFFIX'],
                                           'install' + env['MSVS']['PROJECTSUFFIX']],  ## TODO: Find a clean way of getting these project paths
                               dependencies = [[], [], [], ['mtoa', 'mtoa_api', 'mtoa_shaders']],
                               variant = ['Debug_MSVC|Win64',
                                          'Debug_ICC|Win64',
                                          'Opt_MSVC|Win64',
                                          'Opt_ICC|Win64'])
else:
   maya_env = env.Clone()
   maya_env.Append(CPPPATH = ['.'])
   maya_env.Append(CPPDEFINES = Split('_BOOL REQUIRE_IOSTREAM'))

   if system.os() == 'linux':
      maya_env.Append(CPPPATH = [MAYA_INCLUDE_PATH])
      maya_env.Append(LIBS=Split('GL GLU'))
      maya_env.Append(CPPDEFINES = Split('LINUX'))
      maya_env.Append(LIBPATH = [os.path.join(MAYA_ROOT, 'lib')])
   elif system.os() == 'darwin':
      # MAYA_LOCATION on osx includes Maya.app/Contents
      maya_env.Append(CPPPATH = [MAYA_INCLUDE_PATH])
      maya_env.Append(LIBPATH = [os.path.join(MAYA_ROOT, 'MacOS')])

   maya_env.Append(LIBS=Split('ai pthread Foundation OpenMaya OpenMayaRender OpenMayaUI OpenMayaAnim OpenMayaFX'))

   MTOA_API = env.SConscript(os.path.join('plugins', 'mtoa', 'SConscriptAPI'),
                         variant_dir = os.path.join(BUILD_BASE_DIR, 'api'),
                         duplicate = 0,
                         exports   = 'maya_env')

   MTOA = env.SConscript(os.path.join('plugins', 'mtoa', 'SConscript'),
                         variant_dir = os.path.join(BUILD_BASE_DIR, 'mtoa'),
                         duplicate = 0,
                         exports   = 'maya_env')

   MTOA_SHADERS = env.SConscript(os.path.join('shaders', 'src', 'SConscript'),
                                 variant_dir = os.path.join(BUILD_BASE_DIR, 'shaders'),
                                 duplicate = 0,
                                 exports   = 'env')

   MTOA_PROCS = env.SConscript(os.path.join('procedurals', 'SConscript'),
                                 variant_dir = os.path.join(BUILD_BASE_DIR, 'procedurals'),
                                 duplicate = 0,
                                 exports   = 'env')
                                 
   def osx_hardcode_path(target, source, env):
      cmd = ""

      if target[0] == MTOA_API[0]:
         cmd = "install_name_tool -id @loader_path/../bin/libmtoa_api.dylib"
      elif target[0] == MTOA[0]:
         cmd = " install_name_tool -add_rpath @loader_path/../bin/"
      else:
	     cmd = "install_name_tool -id " + str(target[0]).split('/')[-1]
         
      if cmd :
         p = subprocess.Popen(cmd + " " + str(target[0]), shell=True)
         retcode = p.wait()

      return 0

   if system.os() == 'darwin':
      env.AddPostAction(MTOA_API[0],  Action(osx_hardcode_path, 'Adjusting paths in mtoa_api.dylib ...'))
      env.AddPostAction(MTOA, Action(osx_hardcode_path, 'Adjusting paths in mtoa.boundle ...'))
      env.AddPostAction(MTOA_SHADERS, Action(osx_hardcode_path, 'Adjusting paths in mtoa_shaders ...'))
      env.AddPostAction(MTOA_PROCS, Action(osx_hardcode_path, 'Adjusting paths in mtoa_procs ...'))

Depends(MTOA, MTOA_API[0])

DIFFTIFF = env.SConscript(os.path.join('tools', 'difftiff', 'SConscript'),
                          variant_dir = os.path.join(BUILD_BASE_DIR, 'difftiff'),
                          duplicate = 0,
                          exports   = 'env')

TIFF2JPEG = env.SConscript(os.path.join('tools', 'tiff2jpeg', 'SConscript'),
                          variant_dir = os.path.join(BUILD_BASE_DIR, 'tiff2jpeg'),
                          duplicate = 0,
                          exports   = 'env')

SConscriptChdir(0)
TESTSUITE = env.SConscript(os.path.join('testsuite', 'SConscript'),
                           variant_dir = os.path.join(BUILD_BASE_DIR, 'testsuite'),
                           duplicate = 0,
                           exports   = 'env BUILD_BASE_DIR MTOA MTOA_SHADERS DIFFTIFF TIFF2JPEG')

MTOA_API_DOCS = env.SConscript('docs/doxygen_api/SConscript',
                     variant_dir = os.path.join(BUILD_BASE_DIR, 'docs', 'api'),
                     duplicate   = 0,
                     exports     = 'env BUILD_BASE_DIR')
SConscriptChdir(1)

env.Install(TARGET_PLUGIN_PATH, os.path.join('plugins', 'mtoa', 'mtoa.mtd'))
env.Install(TARGET_SHADER_PATH, os.path.join('shaders', 'mtoa_shaders.mtd'))

if system.os() == 'windows':
   # Rename plugins as .mll and install them in the target path
   mtoa_new = os.path.splitext(str(MTOA[0]))[0] + '.mll'
   env.Command(mtoa_new, str(MTOA[0]), Copy("$TARGET", "$SOURCE"))
   env.Install(TARGET_PLUGIN_PATH, [mtoa_new])
   env.Install(TARGET_SHADER_PATH, MTOA_SHADERS[0])
   env.Install(env['TARGET_PROCEDURAL_PATH'], MTOA_PROCS[0])
   libs = glob.glob(os.path.join(env.subst(env['ARNOLD_API_LIB']), '*.lib'))
else:
   env.Install(TARGET_PLUGIN_PATH, MTOA)
   env.Install(TARGET_SHADER_PATH, MTOA_SHADERS)
   env.Install(env['TARGET_PROCEDURAL_PATH'], MTOA_PROCS)
   if system.os() == 'linux':
       libs = glob.glob(os.path.join(ARNOLD_API_LIB, '*.so'))
   else:
       libs = glob.glob(os.path.join(ARNOLD_API_LIB, '*.dylib'))

env.Install(env['TARGET_LIB_PATH'], libs)

dylibs = glob.glob(os.path.join(ARNOLD_BINARIES, '*%s' % get_library_extension()))
dylibs += glob.glob(os.path.join(ARNOLD_BINARIES, '*%s.*' % get_library_extension()))
env.Install(env['TARGET_BINARIES'], dylibs)

env.Install(env['TARGET_BINARIES'], MTOA_API[0])

# install mtoa common scritps
scriptfiles = find_files_recursive(os.path.join('scripts', 'mtoa'), ['.py', '.mel'])
env.InstallAs([os.path.join(TARGET_PYTHON_PATH, 'mtoa', x) for x in scriptfiles],
              [os.path.join('scripts', 'mtoa', x) for x in scriptfiles])

# install mtoa version specific scritps (myst be done after to allow overwriting)
versionfiles = find_files_recursive(os.path.join('scripts', maya_version_base), ['.py', '.mel'])
env.InstallAs([os.path.join(TARGET_PYTHON_PATH, 'mtoa', maya_version_base, x) for x in versionfiles],
              [os.path.join('scripts', maya_version_base, x) for x in versionfiles])

# install Arnold python bindings
arpybds = find_files_recursive(ARNOLD_PYTHON, ['.py'])
env.InstallAs([os.path.join(TARGET_PYTHON_PATH, x) for x in arpybds],
              [os.path.join(ARNOLD_PYTHON, x) for x in arpybds])
# install include files
apibasepath = os.path.join('plugins', 'mtoa')
apiheaders = [os.path.join('platform', 'Platform.h'),
              os.path.join('platform', 'darwin', 'Event.h'),
              os.path.join('platform', 'linux', 'Event.h'),
              os.path.join('platform', 'win32', 'Event.h'),
              os.path.join('platform', 'win32', 'dirent.h'),
              os.path.join('platform', 'win32', 'Debug.h'),
              os.path.join('common', 'MObjectCompare.h'),
              os.path.join('attributes', 'AttrHelper.h'),
              os.path.join('attributes', 'MNodeClass.h'),
              os.path.join('extension', 'Extension.h'),
              os.path.join('extension', 'AbMayaNode.h'),
              os.path.join('extension', 'AbTranslator.h'),
              os.path.join('extension', 'PxUtils.h'),
              os.path.join('extension', 'PxMayaNode.h'),
              os.path.join('extension', 'PxArnoldNode.h'),  
              os.path.join('extension', 'PxTranslator.h'),                                        
              os.path.join('extension', 'PathUtils.h'),
              os.path.join('session', 'ArnoldSession.h'),
              os.path.join('session', 'SessionOptions.h'),
              os.path.join('session', 'ArnoldLightLinks.h'),
              os.path.join('render', 'AOV.h'),
              os.path.join('translators', 'NodeTranslator.h'),
              os.path.join('translators', 'shape', 'ShapeTranslator.h')]

env.InstallAs([os.path.join(TARGET_INCLUDE_PATH, x) for x in apiheaders],
              [os.path.join(apibasepath, x) for x in apiheaders])
# install icons
env.Install(TARGET_ICONS_PATH, glob.glob(os.path.join('icons', '*.xpm')))
env.Install(TARGET_ICONS_PATH, glob.glob(os.path.join('icons', '*.png')))
# install renderer description
env.Install(TARGET_DESCR_PATH, glob.glob(os.path.join('scripts', 'arnoldRenderer.xml')))

env.MakeModule(TARGET_MODULE_PATH, os.path.join(BUILD_BASE_DIR, 'mtoa.mod'))
env.Install(TARGET_MODULE_PATH, os.path.join(BUILD_BASE_DIR, 'mtoa.mod'))

## Sets release package name based on MtoA version, architecture and compiler used.
##
package_name = "MtoA-" + MTOA_VERSION + "-" + system.get_arch_label(system.os(), system.target_arch()) + "-" + maya_version

if env['MODE'] in ['debug', 'profile']:
   package_name += '-' + env['MODE']

package_name_inst = package_name

if system.os() == 'windows':
   package_name += ".rar"
else:
   package_name += ".tgz"

PACKAGE = env.MakePackage(package_name, MTOA + MTOA_API + MTOA_SHADERS + MTOA_PROCS + MTOA_API_DOCS)
#PACKAGE = env.MakePackage(package_name, MTOA + MTOA_API + MTOA_SHADERS)

################################
## EXTENSIONS
################################

ext_env = maya_env.Clone()
ext_env.Append(CPPPATH = ['plugin', os.path.join(maya_env['ROOT_DIR'], 'plugins', 'mtoa'), env['ARNOLD_API_INCLUDES']])
ext_env.Append(LIBPATH = ['.', ARNOLD_API_LIB, ARNOLD_BINARIES])
ext_env.Append(LIBPATH = [ os.path.join(maya_env['ROOT_DIR'], os.path.split(str(MTOA[0]))[0]),
                           os.path.join(maya_env['ROOT_DIR'], os.path.split(str(MTOA_API[0]))[0])])
ext_env.Append(LIBS = ['mtoa_api',])

ext_base_dir = os.path.join('contrib', 'extensions')
ext_files = []
ext_shaders = []
for ext in os.listdir(ext_base_dir):
    #Only build extensions if they are requested by user
    if ext not in COMMAND_LINE_TARGETS:
       continue
    ext_dir = os.path.join(ext_base_dir, ext)
    if os.path.isdir(ext_dir):
        if system.os() == 'windows':
           EXT = env.SConscript(os.path.join(ext_dir, 'SConscript'),
                                variant_dir = os.path.join(BUILD_BASE_DIR, ext),
                                duplicate = 0,
                                exports   = ['ext_env', 'env'])
           
           if len(EXT) == 4:
              EXT_SHADERS = EXT[1]
              EXT_PRJ = EXT[2]
              EXT_SHADERS_PRJ = EXT[3]
              env.Depends(SOLUTION, EXT_SHADERS_PRJ)
           else:
              EXT_PRJ = EXT[1]
           
           env.Depends(SOLUTION, EXT_PRJ)
        else:
           EXT = env.SConscript(os.path.join(ext_dir, 'SConscript'),
                                variant_dir = os.path.join(BUILD_BASE_DIR, ext),
                                duplicate = 0,
                                exports   = ['ext_env', 'env'])
        
        top_level_alias(env, ext, EXT)
        Depends(EXT, MTOA_API[0])
        Depends(PACKAGE, EXT)
        # only install if the target has been specified
        if ext in COMMAND_LINE_TARGETS:
            # EXT may contain a shader result
            if system.os() == 'windows':
               if len(EXT) > 2:
                  ext_shaders.append(str(EXT[1][0]))
               plugin = str(EXT[0][0])
            else:
               if len(EXT) > 1:
                  ext_shaders.append(str(EXT[1][0]))
                  plugin = str(EXT[0][0])
               else:
                  plugin = str(EXT[0])
            pyfile = os.path.splitext(os.path.basename(plugin))[0] + '.py'
            pyfile = os.path.join(ext_dir, 'plugin', pyfile)
            ext_files.append(plugin)
            if os.path.exists(pyfile):
                ext_files.append(pyfile)

if ext_files:
   env.Install(TARGET_EXTENSION_PATH, ext_files)
if ext_shaders:
   env.Install(TARGET_SHADER_PATH, ext_shaders)

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
[os.path.join('scripts', '*.xml'), '.'],
[MTOA_API[0], 'bin'],
[os.path.join(ARNOLD_BINARIES, 'kick%s' % get_executable_extension()), 'bin'],
[os.path.join(ARNOLD_BINARIES, 'maketx%s' % get_executable_extension()), 'bin'],
[os.path.join(ARNOLD_BINARIES, '*%s' % get_library_extension()), 'bin'],
[os.path.join('plugins', 'mtoa', 'mtoa.mtd'), 'plug-ins'],
[os.path.join('shaders', 'mtoa_shaders.mtd'), 'shaders'],
[MTOA_SHADERS[0], 'shaders'],
[os.path.join(BUILD_BASE_DIR, 'docs', 'api', 'html'), os.path.join('doc', 'api')],
[os.path.splitext(str(MTOA_API[0]))[0] + '.lib', 'lib'],
[os.path.join('docs', 'HOW_TO_INSTALL.txt'), 'doc'],
]


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
      [os.path.join('plugins', 'mtoa', p), os.path.join('include', d)]
   ]
   
for e in ext_files:
   PACKAGE_FILES += [
      [e, 'extensions']
   ]

for e in ext_shaders:
   PACKAGE_FILES += [
      [e, 'shaders']
   ]

if system.os() == 'windows':
   PACKAGE_FILES += [
      [MTOA[0], 'plug-ins', 'mtoa.mll'],
   ]
elif system.os() == 'linux':
   PACKAGE_FILES += [
      [MTOA[0], 'plug-ins'],
      [os.path.join(ARNOLD_BINARIES, '*%s.*' % get_library_extension()), 'bin'],
   ]
elif system.os() == 'darwin':
   PACKAGE_FILES += [
      [MTOA[0], 'plug-ins'],
   ]

env['PACKAGE_FILES'] = PACKAGE_FILES

################################
## TARGETS ALIASES AND DEPENDENCIES
################################

if system.os() == 'windows':
   env.Depends(SOLUTION, MTOA_PRJ)
   env.Depends(SOLUTION, MTOA_API_PRJ)
   env.Depends(SOLUTION, MTOA_SHADERS_PRJ)
   env.Depends(SOLUTION, MTOA_PROCS_PRJ)
   env.Depends(SOLUTION, INSTALL_PRJ)
   env.AlwaysBuild(INSTALL_PRJ)
   top_level_alias(env, 'solution', SOLUTION)

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

top_level_alias(env, 'mtoa', MTOA)
top_level_alias(env, 'docs', MTOA_API_DOCS)
top_level_alias(env, 'shaders', MTOA_SHADERS)
top_level_alias(env, 'testsuite', TESTSUITE)
top_level_alias(env, 'install', aliases)
top_level_alias(env, 'pack', PACKAGE)

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

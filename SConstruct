# vim: filetype=python

## first we extend the module path to load our own modules
import sys, os
sys.path = ["tools/python"] + sys.path

import system
from build_tools import *

import glob, shutil

def SymLink(target, source, env):
   print "making link %s pointing to target %s" % (target[0], source[0])
   os.symlink(os.path.split(str(source[0]))[1], str(target[0]))

def MakeModule(env, target, source):
   # TODO: get mtoa version from somewhere...
   f = open(source[0], 'w' )
   f.write('+ mtoa 0.4 %s\n' % target[0])
   f.close()

if system.os() == 'darwin':
   ALLOWED_COMPILERS = ('gcc',)   # Do not remove this comma, it's magic
elif system.os() == 'linux':
   ALLOWED_COMPILERS = ('gcc',)   # Do not remove this comma, it's magic
elif system.os() == 'windows':
   ALLOWED_COMPILERS = ('msvc', 'icc')
else:
   print "Unknown operating system: %s" % system.os()
   Exit(1)

vars = Variables('custom.py')
vars.AddVariables(
      ## basic options
      EnumVariable('MODE',        'Set compiler configuration', 'debug', allowed_values=('opt', 'debug', 'profile')),
      EnumVariable('WARN_LEVEL',  'Set warning level',          'strict', allowed_values=('strict', 'warn-only', 'none')),
      EnumVariable('COMPILER',    'Set compiler to use',        ALLOWED_COMPILERS[0], allowed_values=ALLOWED_COMPILERS),
      EnumVariable('TARGET_ARCH', 'Allows compiling for a different architecture', system.host_arch(), allowed_values=system.get_valid_target_archs()),
      BoolVariable('SHOW_CMDS',   'Display the actual command lines used for building', False),

      EnumVariable('SHOW_TEST_OUTPUT', 'Display the test log as it is being run', 'single', allowed_values=('always', 'never', 'single') ),
      BoolVariable('UPDATE_REFERENCE', 'Update the reference log/image for the specified targets', False),
      ('TEST_PATTERN' , 'Glob pattern of tests to be run', 'test_*'),
      ('GCC_OPT_FLAGS', 'Optimization flags for gcc', '-O3 -funroll-loops'),

      PathVariable('MAYA_ROOT', 'Directory where Maya is installed', get_default_path('MAYA_ROOT', '.')),
      PathVariable('EXTERNAL_PATH', 'External dependencies are found here', '.'),
      PathVariable('ARNOLD_API_INCLUDES', 'Where to find Arnold API includes', '.'),
      PathVariable('ARNOLD_API_LIB', 'Where to find Arnold API libraries', '.'),
      PathVariable('TARGET_MODULE_PATH', 'Path used for installation of the mtoa plugin', '.', PathVariable.PathIsDirCreate),
      PathVariable('TARGET_PLUGIN_PATH', 'Path used for installation of the mtoa plugin', '$TARGET_MODULE_PATH/plug-ins', PathVariable.PathIsDirCreate),
      PathVariable('TARGET_SCRIPTS_PATH', 'Path used for installation of scripts', '$TARGET_MODULE_PATH/scripts', PathVariable.PathIsDirCreate),
      PathVariable('TARGET_PYTHON_PATH', 'Path used for installation of Python scripts', '$TARGET_SCRIPTS_PATH', PathVariable.PathIsDirCreate),
      PathVariable('TARGET_ICONS_PATH', 'Path used for installation of icons', '$TARGET_MODULE_PATH/icons', PathVariable.PathIsDirCreate),
      PathVariable('TARGET_DESCR_PATH', 'Path for renderer description file', '.'),
      PathVariable('TARGET_SHADER_PATH', 'Path used for installation of arnold shaders', '$TARGET_MODULE_PATH/shaders', PathVariable.PathIsDirCreate),
      PathVariable('TARGET_EXTENSION_PATH', 'Path used for installation of mtoa translator extensions', '$TARGET_MODULE_PATH/extensions', PathVariable.PathIsDirCreate),
      PathVariable('TARGET_LIB_PATH', 'Path for libraries', '$TARGET_MODULE_PATH/lib', PathVariable.PathIsDirCreate)
)

if system.os() == 'windows':
   vars.Add(EnumVariable('MSVS_VERSION', 'Version of MS Visual Studio to use', '8.0', allowed_values=('8.0', '8.0Exp', '9.0', '9.0Exp')))

if system.os() == 'windows':
   # Ugly hack. Create a temporary environment, without loading any tool, so we can set the MSVS_ARCH
   # variable from the contents of the TARGET_ARCH variable. Then we can load tools.
   tmp_env = Environment(variables = vars, tools=[])
   tmp_env.Append(MSVS_ARCH = ('amd64' if tmp_env['TARGET_ARCH'] == 'x86_64' else 'x86'))
   env = tmp_env.Clone(tools=['default'])
else:
   env = Environment(variables = vars)

env.Append(BUILDERS = {'MakeModule' : MakeModule})

system.set_target_arch(env['TARGET_ARCH'])

if len(BUILD_TARGETS) > 0:
   ## This message is not too clear. We should make it easier to read.
   print "Building mtoa version '%s'. Mode = '%s'" % ('<not available>', env['MODE'])
   print "Host OS: '%s', host architecture: '%s', target architecture: '%s'" % (system.os(), system.host_arch(), system.target_arch())
   print "Arnold version '%s'" % get_arnold_version(os.path.join(env['ARNOLD_API_INCLUDES'], 'ai_version.h'))

################################
## COMPILER OPTIONS
################################

if system.os() == 'darwin':
   env.Append(CPPDEFINES = Split('_DARWIN'))
elif system.os() == 'linux':
   env.Append(CPPDEFINES = Split('_LINUX'))
elif system.os() == 'windows':
   # Embed manifest in executables and dynamic libraries
   env['LINKCOM'] = [env['LINKCOM'], 'mt.exe -nologo -manifest ${TARGET}.manifest -outputresource:$TARGET;1']
   env['SHLINKCOM'] = [env['SHLINKCOM'], 'mt.exe -nologo -manifest ${TARGET}.manifest -outputresource:$TARGET;2']
   env.Append(CPPDEFINES = Split('_WINDOWS _WIN32 WIN32'))
   if system.target_arch() == 'x86_64':
      env.Append(CPPDEFINES = Split('_WIN64'))

if env['COMPILER'] == 'gcc':
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
         env.Append(CCFLAGS = Split('-g')) 
         env.Append(LINKFLAGS = Split('-g')) 
   if system.os() == 'linux' and env['MODE'] == 'profile':
      env.Append(CCFLAGS = Split('-pg'))
      env.Append(LINKFLAGS = Split('-pg'))

   if system.os() == 'darwin':
      if system.target_arch() == 'x86_64':
         ## tell gcc to compile a 64 bit binary
         env.Append(CCFLAGS = Split('-arch x86_64'))
         env.Append(LINKFLAGS = Split('-arch x86_64'))
      else:
         ## tell gcc to compile a 32 bit binary
         env.Append(CCFLAGS = Split('-arch i386'))
         env.Append(LINKFLAGS = Split('-arch i386'))

elif env['COMPILER'] == 'msvc':
   env.Append(CCFLAGS = Split('/W3 -D_CRT_SECURE_NO_WARNINGS /EHsc /Gd /fp:precise'))
   env.Append(LINKFLAGS = Split('/LARGEADDRESSAWARE'))
   
   if env['MODE'] == 'opt':
      env.Append(CCFLAGS = Split('/Ob2 /MD'))
      env.Append(CPPDEFINES = Split('NDEBUG'))
   elif env['MODE'] == 'profile':
      env.Append(CCFLAGS = Split('/Ob2 /MD /Zi'))
   else:  ## Debug mode
      env.Append(CCFLAGS = Split('/Od /Zi /MDd'))
      env.Append(LINKFLAGS = Split('/DEBUG'))
      env.Append(CPPDEFINES = Split('_DEBUG'))
elif env['COMPILER'] == 'icc':
   if system.target_arch() == 'x86_64':
      env.Tool('intelc', abi = 'intel64')
   else:
      env.Tool('intelc', abi = 'x86')
   
   env.Append(CCFLAGS = Split('/W3 /Qstd=c99 /EHsc /GS /GR /Qprec /Qvec-report0 /Qwd1478 /Qwd1786 /Qwd537 /Qwd1572 /Qwd991 /Qwd424'))
   env.Append(LINKFLAGS = Split('/LARGEADDRESSAWARE'))

   if system.target_arch() == 'x86_64':
   	pass
      # Enable this for 64 bit portability warnings
      ##env.Append(CCFLAGS = Split('/Wp64'))  
   else:
      env.Append(CCFLAGS = Split('/Gd'))
   
   if env['MODE'] == 'opt':
      env.Append(CCFLAGS = Split('/Ob2 /MD -G7 -O3 -QaxW -Qipo'))
      env.Append(LINKFLAGS = Split('/INCREMENTAL:NO'))
      env.Append(CPPDEFINES = Split('NDEBUG'))
   elif env['MODE'] == 'profile':
      env.Append(CCFLAGS = Split('/Ob2 /MD -G7 -O3 -QaxW -Qipo /Zi /FR /debug:all'))
      env.Append(LINKFLAGS = Split('/FIXED:no /DEBUG /INCREMENTAL:NO'))
   else:  ## Debug mode
      env.Append(CCFLAGS = Split('/Od /Zi /MDd'))
      env.Append(LINKFLAGS = Split('/DEBUG /INCREMENTAL'))
      env.Append(CPPDEFINES = Split('_DEBUG'))
else:
   print "Compiler %s is not supported yet" % (env['COMPILER'])
   Exit(1)

## Add path to Arnold API by default
env.Append(CPPPATH = [env['ARNOLD_API_INCLUDES']])
env.Append(LIBPATH = [env['ARNOLD_API_LIB']])
   
## configure base directory for temp files
BUILD_BASE_DIR = os.path.join('build', '%s_%s' % (system.os(), system.target_arch()), '%s_%s' % (env['COMPILER'], env['MODE']))

if not env['SHOW_CMDS']:
   ## hide long compile lines from the user
   env['CCCOMSTR']     = 'Compiling $SOURCE ...'
   env['SHCCCOMSTR']   = 'Compiling $SOURCE ...'
   env['CXXCOMSTR']    = 'Compiling $SOURCE ...'
   env['SHCXXCOMSTR']  = 'Compiling $SOURCE ...'
   env['LINKCOMSTR']   = 'Linking $TARGET ...'
   env['SHLINKCOMSTR'] = 'Linking $TARGET ...'

################################
## BUILD TARGETS
################################

env['ROOT_DIR'] = os.getcwd()

if system.os() == 'windows':
   maya_env = env.Clone()
   maya_env.Append(CPPPATH = ['.'])
   maya_env.Append(CPPPATH = [os.path.join(env['MAYA_ROOT'], 'include')])
   maya_env.Append(CPPDEFINES = Split('NT_PLUGIN REQUIRE_IOSTREAM'))
   maya_env.Append(LIBPATH = [os.path.join(env['MAYA_ROOT'], 'lib')])
   maya_env.Append(LIBS=Split('ai.lib OpenGl32.lib glu32.lib Foundation.lib OpenMaya.lib OpenMayaRender.lib OpenMayaUI.lib OpenMayaAnim.lib OpenMayaFX.lib'))

   [MTOA, MTOA_PRJ] = env.SConscript(os.path.join('plugins', 'mtoa', 'SConscript'),
                                     build_dir = os.path.join(BUILD_BASE_DIR, 'mtoa'),
                                     duplicate = 0,
                                     exports   = 'maya_env')

   [MTOA_SHADERS, MTOA_SHADERS_PRJ] = env.SConscript(os.path.join('shaders', 'src', 'SConscript'),
                                                     build_dir = os.path.join(BUILD_BASE_DIR, 'shaders'),
                                                     duplicate = 0,
                                                     exports   = 'env')

   INSTALL_PRJ = env.MSVSProject(target = 'install' + env['MSVS']['PROJECTSUFFIX'],
                                 srcs = [],
                                 incs = [],
                                 buildtarget = 'install',
                                 cmdargs = ['-Q -s COMPILER=msvc MODE=debug TARGET_ARCH=x86',
                                            '-Q -s COMPILER=icc MODE=debug TARGET_ARCH=x86',
                                            '-Q -s COMPILER=msvc MODE=opt TARGET_ARCH=x86',
                                            '-Q -s COMPILER=icc MODE=opt TARGET_ARCH=x86'],
                                 variant = ['Debug_MSVC|Win32',
                                            'Debug_ICC|Win32',
                                            'Opt_MSVC|Win32',
                                            'Opt_ICC|Win32'],
                                 auto_build_solution = 0,
                                 nokeep = 1)

   SOLUTION = env.MSVSSolution(target = 'mtoa' + env['MSVS']['SOLUTIONSUFFIX'],
                               projects = [os.path.join('plugins', 'mtoa', 'mtoa') + env['MSVS']['PROJECTSUFFIX'],
                                           os.path.join('shaders', 'src', 'mtoa_shaders') + env['MSVS']['PROJECTSUFFIX'],
                                           'install' + env['MSVS']['PROJECTSUFFIX']],  ## TODO: Find a clean way of getting these project paths
                               dependencies = [[], [], ['mtoa', 'mtoa_shaders']],
                               variant = ['Debug_MSVC|Win32',
                                          'Debug_ICC|Win32',
                                          'Opt_MSVC|Win32',
                                          'Opt_ICC|Win32'])
else:
   maya_env = env.Clone()
   maya_env.Append(CPPPATH = ['.'])
   maya_env.Append(CPPPATH = [os.path.join(env['MAYA_ROOT'], 'include')])
   maya_env.Append(CPPDEFINES = Split('LINUX _BOOL REQUIRE_IOSTREAM'))
   maya_env.Append(LIBPATH = [os.path.join(env['MAYA_ROOT'], 'lib')])
   maya_env.Append(LIBS=Split('ai GL GLU pthread Foundation OpenMaya OpenMayaRender OpenMayaUI OpenMayaAnim OpenMayaFX'))

   MTOA = env.SConscript(os.path.join('plugins', 'mtoa', 'SConscript'),
                         build_dir = os.path.join(BUILD_BASE_DIR, 'mtoa'),
                         duplicate = 0,
                         exports   = 'maya_env')

   MTOA_SHADERS = env.SConscript(os.path.join('shaders', 'src', 'SConscript'),
                                 build_dir = os.path.join(BUILD_BASE_DIR, 'shaders'),
                                 duplicate = 0,
                                 exports   = 'env')

DIFFTIFF = env.SConscript(os.path.join('tools', 'difftiff', 'SConscript'),
                          build_dir = os.path.join(BUILD_BASE_DIR, 'difftiff'),
                          duplicate = 0,
                          exports   = 'env')

TIFF2JPEG = env.SConscript(os.path.join('tools', 'tiff2jpeg', 'SConscript'),
                          build_dir = os.path.join(BUILD_BASE_DIR, 'tiff2jpeg'),
                          duplicate = 0,
                          exports   = 'env')

SConscriptChdir(0)
TESTSUITE = env.SConscript(os.path.join('testsuite', 'SConscript'),
                           build_dir = os.path.join(BUILD_BASE_DIR, 'testsuite'),
                           duplicate = 0,
                           exports   = 'env BUILD_BASE_DIR MTOA MTOA_SHADERS DIFFTIFF TIFF2JPEG')
SConscriptChdir(1)

if system.os() == 'windows':
   # Rename plugins as .mll and install them in the target path
   mtoa_new = os.path.splitext(str(MTOA[0]))[0] + '.mll'
   env.Command(mtoa_new, str(MTOA[0]), Copy("$TARGET", "$SOURCE"))
   env.Install(env['TARGET_PLUGIN_PATH'], [mtoa_new] + glob.glob(os.path.join(env['ARNOLD_API_LIB'], '*.dll')))
   env.Install(env['TARGET_SHADER_PATH'], MTOA_SHADERS[0])
else:
   env.Install(env['TARGET_PLUGIN_PATH'], MTOA + glob.glob(os.path.join(env['ARNOLD_API_LIB'], '*.so')))
   env.Install(env['TARGET_SHADER_PATH'], MTOA_SHADERS)

env.Install(env['TARGET_SCRIPTS_PATH'], glob.glob(os.path.join('scripts', '*.mel')))
pyfiles = find_files_recursive('scripts', ['.py'])
env.InstallAs([os.path.join(env['TARGET_PYTHON_PATH'], x) for x in pyfiles],
              [os.path.join('scripts', x) for x in  pyfiles])
env.Install(env['TARGET_ICONS_PATH'], glob.glob(os.path.join('icons', '*.xpm')))
env.Install(env['TARGET_DESCR_PATH'], glob.glob(os.path.join('scripts', '*.xml')))

env.MakeModule(env['TARGET_MODULE_PATH'], os.path.join(BUILD_BASE_DIR, 'mtoa.mod'))
env.Install(env['TARGET_MODULE_PATH'], os.path.join(BUILD_BASE_DIR, 'mtoa.mod'))

################################
## TARGETS ALIASES AND DEPENDENCIES
################################

if system.os() == 'windows':
   env.Depends(SOLUTION, MTOA_PRJ)
   env.Depends(SOLUTION, MTOA_SHADERS_PRJ)
   env.Depends(SOLUTION, INSTALL_PRJ)
   env.AlwaysBuild(INSTALL_PRJ)
   top_level_alias(env, 'solution', SOLUTION)

aliases = []
aliases.append(env.Alias('install-scripts', env['TARGET_SCRIPTS_PATH']))
aliases.append(env.Alias('install-python', env['TARGET_PYTHON_PATH']))
aliases.append(env.Alias('install-icons', env['TARGET_ICONS_PATH']))
aliases.append(env.Alias('install-descr', env['TARGET_DESCR_PATH']))
aliases.append(env.Alias('install-lib', env['TARGET_LIB_PATH']))
aliases.append(env.Alias('install-plugins', env['TARGET_PLUGIN_PATH']))
aliases.append(env.Alias('install-shaders', env['TARGET_SHADER_PATH']))
aliases.append(env.Alias('install-module', env['TARGET_MODULE_PATH']))

top_level_alias(env, 'mtoa', MTOA)
top_level_alias(env, 'shaders', MTOA_SHADERS)
top_level_alias(env, 'testsuite', TESTSUITE)
top_level_alias(env, 'install', aliases)

#env.AlwaysBuild('install')
Default('mtoa')

## Process top level aliases into the help message
Help('''%s

Top level targets:
    %s

Individual tests can be run using the 'test_nnnn' target.
Note that this folder must fall within the TEST_PATTERN glob.
''' % (vars.GenerateHelpText(env), get_all_aliases()))

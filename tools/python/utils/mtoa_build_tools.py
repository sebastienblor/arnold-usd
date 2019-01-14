import os
import system
import platform
from os import environ
import zipfile
import glob

HOST_ARCH   = ''
TARGET_ARCH = ''

def find_files(path, valid_extensions):
   '''
   Returns a list of all files with an extension from the 'valid_extensions' list
   '''
   path = os.path.normpath(path)
   return [f for f in os.listdir(path) if any([f.endswith(ext) for ext in valid_extensions])]

def get_mtoa_version(components = 4):
   '''
   Obtains MtoA version by parsing 'Version.cpp'
   '''
   ARCH_VERSION = ''
   MAJOR_VERSION = ''
   MINOR_VERSION = ''
   FIX_VERSION = ''

   # TODO: define in the plug-in the symbols necessary to get MtoA version 
   f = open(os.path.join('plugins', 'mtoa', 'utils', 'Version.h'), 'r')
   while True:
      line = f.readline().lstrip(' \t')
      if line == "":
         # We have reached the end of file.
         break
      if line.startswith('#define'):
         tokens = line.split()
         if tokens[1] == 'MTOA_ARCH_VERSION_NUM':
            ARCH_VERSION = tokens[2]
         elif tokens[1] == 'MTOA_MAJOR_VERSION_NUM':
            MAJOR_VERSION = tokens[2]
         elif tokens[1] == 'MTOA_MINOR_VERSION_NUM':
            MINOR_VERSION = tokens[2]
         elif tokens[1] == 'MTOA_FIX_VERSION':
            FIX_VERSION = tokens[2][1:].strip('"')
   f.close()
   
   version = ''
   if (components > 0):
      version += ARCH_VERSION
   if (components > 1):
      version += '.' + MAJOR_VERSION
   if (components > 2):
      version += '.' + MINOR_VERSION
   if (components > 3 and FIX_VERSION != ''):
      version += '.' + FIX_VERSION
   return version 

def get_maya_version(path):
   try:
      f = open(path, 'r')
   except:
      return "000000"
   while True:
      line = f.readline().lstrip(' \t')
      if line == "":
         # We have reached the end of file.
         break
      if line.startswith('#define'):
         tokens = line.split()
         if tokens[1] == 'MAYA_API_VERSION':
            return tokens[2][:]
   f.close()

def make_module(env, target, source):
   # When symbols defined in the plug-in
   if not os.path.exists(os.path.dirname(source[0])) and os.path.dirname(source[0]):
      os.makedirs(os.path.dirname(source[0]))
   f = open(source[0], 'w' )
   # Maya got problems with double digit versions
   # f.write('+ mtoa %s %s\n' % (get_mtoa_version(3), target[0]))
   f.write('+ mtoa %s %s\n' % ('any', target[0]))
   f.write('PATH +:= bin\n')
   f.write('MAYA_CUSTOM_TEMPLATE_PATH +:= scripts/mtoa/ui/templates\n')
   f.close()

def set_target_arch(arch):
   global TARGET_ARCH, TARGET_ARCH_LABEL
   if not arch in VALID_ARCHS:
      print "ERROR: Target architecture '%s' is not valid" % arch
   else:
      TARGET_ARCH = arch
      TARGET_ARCH_LABEL = get_arch_label(system.os, arch)


## Returns a simplified label for the system architecture ('linux_x86', 'linux_x86_64', 'darwin32', 'win32')
def get_arch_label(os, arch):
   if system.os == 'windows':
      if arch == 'x86':
         return 'win32'
      elif arch == 'x86_64':
         return 'win64'
   elif system.os == 'darwin':
      if arch == 'x86':
         return 'darwin32'
      elif arch == 'x86_64':
         return 'darwin64'
   elif system.os == 'linux':
      if arch == 'x86':
         return 'linux32'
      elif arch == 'x86_64':
         return 'linux64'
      elif arch == 'sparc64':
         return 'linux_sparc_64'

   return 'Unknown'

def get_executable_extension():
   if system.os == 'windows':
      return ".exe"
   else:
      return ""

if system.os == 'windows':
   VALID_ARCHS = ('x86', 'x86_64')
   if (environ.has_key('PROCESSOR_ARCHITECTURE') and environ['PROCESSOR_ARCHITECTURE'] == 'AMD64') or (environ.has_key('PROCESSOR_ARCHITEW6432') and environ['PROCESSOR_ARCHITEW6432'] == 'AMD64'):
      HOST_ARCH = 'x86_64'
   else:
      HOST_ARCH = 'x86'
elif system.os == 'darwin':
   VALID_ARCHS = ('x86', 'x86_64')
   if platform.architecture()[0] == '64bit':
      HOST_ARCH = 'x86_64'
   else:
      HOST_ARCH = 'x86'
elif system.os == 'linux':
   VALID_ARCHS = ('x86', 'x86_64', 'sparc_64')
   
   if platform.machine() == 'sparc64':
      HOST_ARCH = 'sparc_64'
   elif platform.machine() == 'x86_64':
      HOST_ARCH = 'x86_64'
   else:
      HOST_ARCH = 'x86'

TARGET_ARCH = HOST_ARCH

def target_arch():
   return TARGET_ARCH

def saferemove(path):
   '''
   handy function to remove files only if they exist
   '''
   for p in glob.glob(path):
      if os.path.isfile(p):
          try:
             os.remove(p)
          except OSError as e:
             print "Couldn't remove %s"  % p

## Creates a release package for Arnold
def make_package(target, source, env):
    package_name = str(target[0])
    saferemove(package_name)
    zp = zipfile.ZipFile('%s.zip' % package_name, 'w', zipfile.ZIP_DEFLATED)

    for t in env['PACKAGE_FILES']:
        target_dir = str(t[1])
        file_spec = str(t[0])
        if os.path.isdir(file_spec):
            for path, subdirs, files in os.walk(file_spec):
                for f in files:
                    _target_dir = target_dir
                    for subdir in subdirs:
                        _target_dir = os.path.join(_target_dir, subdir)
                    zp.write(os.path.join(str(path), f), os.path.join(_target_dir, f))
        else:
            file_list = glob.glob(file_spec)
            if (len(file_list) == 1 and len(t) == 3):
                zp.write(file_list[0], os.path.join(target_dir, str(t[2])))
            else:
                for f in file_list:
                    zp.write(f, os.path.join(target_dir, os.path.basename(f)))
         
    return None

def sign_packaged_file(sign_script, target, allowed_extensions, excluded_files = []):
    if sign_script == '':
        return
    if os.path.isdir(target):
        for root, subdirs, files in os.walk(target):
            for file in files:
                installedFile = os.path.join(root, file)
                if not os.path.exists(installedFile):
                    # can this happen ?
                    continue

                filename, file_ext = os.path.splitext(installedFile)
                if file_ext not in allowed_extensions:
                    continue
                basename = os.path.basename(installedFile)
                if basename in excluded_files:
                    continue

                os.system(sign_script + ' ' + installedFile)
    else:
        os.system(sign_script + ' ' + target)

def get_library_extension():
   if system.os == 'windows':
      return ".dll"
   elif system.os == 'linux':
      return ".so"
   elif system.os == 'darwin':
      return ".dylib"
   else:
      return ""

def get_library_prefix():
   if system.os == 'windows':
      return ""
   elif system.os == 'linux':
      return "lib"
   elif system.os == 'darwin':
      return "lib"
   else:
      return ""

ALIASES = ''
def top_level_alias(env, name, targets):
   '''create a top level alias so that the help system will know about it'''
   global ALIASES
   ALIASES = '%s %s' % (ALIASES, name)
   env.Alias(name, targets)

def get_all_aliases():
   return ALIASES
import os.path

from build_tools import *

import os, sys, glob, shutil, subprocess

TRAC_SERVER = "https://trac.solidangle.com/mtoa"

## Creates a release package for Arnold
def make_package(target, source, env):
   package_name = str(target[0])
   base_pkg_dir = 'package_temp'
   
   # First we make sure the temp directory doesn't exist
   if os.path.exists(base_pkg_dir):
      shutil.rmtree(base_pkg_dir)

   # Copy all package files into the temporary directory   
   for t in env['PACKAGE_FILES']:
      target_dir = os.path.join(base_pkg_dir, str(t[1]))
      file_spec = str(t[0])
      if os.path.isdir(file_spec):
         # This is needed for Python 2.4, as copytree didn't create intermediate directories
         os.makedirs(os.path.dirname(target_dir))
         # If the first element is a directory, use copytree to make a recursive copy
         shutil.copytree(file_spec, target_dir, True)
      else:
         # If the first element is a file or wildcard, create the target dir and copy files separately
         if not os.path.exists(target_dir):
            os.makedirs(target_dir)
         file_list = glob.glob(file_spec)
      
         # Optionally rename the destination file (only for single files)
         if (len(file_list) == 1) and (len(t) == 3):
            copy_file_or_link(file_list[0], os.path.join(target_dir, str(t[2])))
         else:
            for f in file_list:
               copy_file_or_link(f, target_dir)
      
   saferemove(package_name)

   # For now, we default to .rar for windows and .tar.gz for Linux/Mac
   command_string = ''   
   if system.os() == 'windows':
      if find_in_path('rar.exe') != []:
         command_string = "rar a -ep1 -inul -r %s %s" % (package_name, os.path.join(base_pkg_dir, '*'))
   else:
      # this gives us an explicit list of all the files/directories in 'package_temp/'
      file_list = " ".join(os.listdir('package_temp'))
      command_string = "tar -czf %s -C ./package_temp %s" % (package_name, file_list)

   if command_string != '':
      p = subprocess.Popen(command_string, shell=True, stdout = None)
      retcode = p.wait()
      if retcode != 0:
         print "ERROR: Could not create package '%s'" % package_name

   # Clean temporary directory
   shutil.rmtree(base_pkg_dir)
         
   return None

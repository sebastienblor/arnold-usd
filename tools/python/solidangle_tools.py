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

   import zipfile

   zp = zipfile.ZipFile('%s.zip' % package_name, 'w', zipfile.ZIP_DEFLATED)
   for dirpath, dirnames, filenames in os.walk(base_pkg_dir):
      prefix_path = dirpath[len(base_pkg_dir):]
      if (len(prefix_path) != 0) and (prefix_path[0] == '/'):
         prefix_path = prefix_path[1:]
      for f in filenames:
         zp.write(os.path.join(dirpath, f), '%s/%s' % (prefix_path, f))

   # Clean temporary directory
   shutil.rmtree(base_pkg_dir)
         
   return None

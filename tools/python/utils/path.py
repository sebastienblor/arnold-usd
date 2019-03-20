# vim: filetype=python

import os, shutil

def list(path, extensions=None):
   '''
   Returns a list of all files with an extension from the 'extensions' list
   '''
   path = os.path.normpath(path)
   result = []
   for root, dirs, files in os.walk(path):
      if '.git' in dirs:
         dirs.remove('.git')
      for f in files:
         if not extensions or os.path.splitext(f)[1] in extensions:
            # Build the absolute path and then remove the root path, to get the relative path from root
            result.append(os.path.join(root, f)[len(path) + 1:])
   result.sort(key=os.path.split) ## keep the files in a predictable order
   return result

def remove(path):
   '''
   Handy function to remove files only if they exist
   '''
   if os.path.exists(path):
      if os.path.isdir(path):
         shutil.rmtree(path)
      else:
         os.remove(path)

def copy(src, target):
   '''
   Copies a file or a symbolic link (creating a new link in the target dir)
   '''
   if os.path.isdir(target):
      target = os.path.join(target, os.path.basename(src))

   if os.path.islink(src):
      linked_path = os.readlink(src)
      os.symlink(linked_path, target)
   else:
      shutil.copy(src, target)

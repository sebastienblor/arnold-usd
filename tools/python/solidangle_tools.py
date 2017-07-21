import os.path

from build_tools import *

import os, glob, zipfile

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


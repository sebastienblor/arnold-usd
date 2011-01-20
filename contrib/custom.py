from __future__ import with_statement
import os, platform
from os.path import join

mayaver = os.environ['MAYA_VERSION_NUM']

basedir = '/usr/local/solidAngle'
mtoadir = join(basedir, 'mtoa-0.4', mayaver)
arnoldver = '3.3.3.0'
arnolddir = join(basedir, 'arnold-%s' % arnoldver)

TARGET_ARCH         = os.environ['ARCH']
MAYA_ROOT           = os.environ['MAYA_LOCATION']
EXTERNAL_PATH       = join(basedir, 'external')
ARNOLD_API_INCLUDES = join(arnolddir, 'include')
ARNOLD_API_LIB      = join(arnolddir, 'bin')
TARGET_MODULE_PATH   = mtoadir
#MODE                = 'opt'
#WARN_LEVEL          = 'warn-only'
#SHOW_CMDS = True

# create library symlinks
libdir = join(mtoadir, 'lib')
if not os.path.exists(libdir):
    os.makedirs(libdir)

for lib in ['libai.so',
            'libboost_filesystem.so.1.42.0', 'libboost_program_options.so.1.42.0', 
            'libboost_regex.so.1.42.0', 'libboost_system.so.1.42.0',
            'libboost_thread.so.1.42.0',
            'libOpenImageIO.so']:
    source = join(arnolddir, 'bin', lib)
    link = join(libdir, lib)
    if os.path.islink(link):
        os.remove(link)
    os.symlink(source, link)


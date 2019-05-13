import os
import sys
import platform

import subprocess

OS = platform.system().lower()

# Execute the script
maya_root_path    = sys.argv[1]
mtoa_root_path    = sys.argv[2]
xgen_root_path    = os.path.join(maya_root_path, "plug-ins", "xgen")

shaders_path = os.path.join(mtoa_root_path, "shaders")
procedurals_path = os.path.join(mtoa_root_path, "procedurals")

env = {}
if OS == "linux":
    maya_lib_path = os.path.join(maya_root_path, "lib")
    xgen_lib_path = os.path.join(xgen_root_path, "lib")
    env["LD_LIBRARY_PATH"] = ":".join([maya_lib_path, xgen_lib_path])
elif OS == "darwin":
    maya_lib_path = os.path.join(maya_root_path, "Maya.app", "Contents", "MacOS")
    xgen_lib_path = os.path.join(xgen_root_path, "lib")
    env["DYLD_LIBRARY_PATH"] = ":".join([maya_lib_path, xgen_lib_path])
elif OS == "windows":
    maya_lib_path = os.path.join(maya_root_path, "bin")
    xgen_lib_path = os.path.join(xgen_root_path, "bin")
    env["PATH"] = ";"+";".join([maya_lib_path, xgen_lib_path])

env["XGEN_LOCATION"] = xgen_root_path
env["MAYA_PROJECT"] = os.path.dirname(__file__)

kick = os.path.join(mtoa_root_path, 'bin', 'kick')
cmd = '%s -v 2 -dp -dw scene.ass -l %s -l %s -r 160 120 -o testrender.tif' \
 % (kick, shaders_path, procedurals_path)

p = subprocess.Popen(cmd.split(), env=env)
p.communicate()

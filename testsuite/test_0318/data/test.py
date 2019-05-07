import os
import sys
import platform

OS = platform.system().lower()

# Execute the script
maya_root_path    = sys.argv[1]
mtoa_root_path    = sys.argv[2]

shaders_path = os.path.join(mtoa_root_path, "shaders")
procedurals_path = os.path.join(mtoa_root_path, "procedurals")

xgen_lib_path = os.path.join(maya_root_path, "plug-ins", "xgen", "lib")
maya_lib_path = os.path.join(maya_root_path, "lib")

if OS == "linux":
    os.environ["LD_LIBRARY_PATH"] = ":".join([maya_lib_path, xgen_lib_path])
elif OS == "darwin":
    os.environ["DYLD_LIBRARY_PATH"] = ":".join([maya_lib_path, xgen_lib_path])
elif OS == "windows":
    os.environ["PATH"] = ";"+";".join([maya_lib_path, xgen_lib_path])
os.environ["MAYA_PROJECT"] = os.environ["PWD"]

kick = os.path.join(mtoa_root_path, 'bin', 'kick')
cmd = '%s -v 2 -dp -dw scene.ass -l "%s" -l "%s" -r 160 120 -o testrender.tif' \
 % (kick, shaders_path, procedurals_path)

print cmd
os.system(cmd)

import os
import sys

# The test validates that the color conversion mode is working as the driver color space is 
# an input color space instead of the expected output color space.

# Execute the script

maya_root_path    = sys.argv[1]
mtoa_root_path    = sys.argv[2]

shaders_path = os.path.join(mtoa_root_path, "shaders")
synColor_path = os.path.join(maya_root_path, "synColor")

kick = os.path.join(mtoa_root_path, 'bin', 'kick')
cmd = '%s -v 0 -dp -dw ref.ass -l "%s" -r 160 120 -set defaultColorMgtGlobals.native_catalog_path "%s" -t 64 -o testrender.tif' \
  % (kick, shaders_path, synColor_path)

print cmd

os.system(cmd)

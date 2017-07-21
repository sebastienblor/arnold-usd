import os
import sys

# The test validates the Arnold Core color space linearity check for the exr driver. 
# It uses the post_script to extract the rgba part from the exr to create a tiff file as only tiff files 
# are used for the color difference computation.

maya_root_path    = sys.argv[1]
mtoa_root_path    = sys.argv[2]

shaders_path = os.path.join(mtoa_root_path, "shaders")
synColor_path = os.path.join(maya_root_path, "synColor")

kick = os.path.join(mtoa_root_path, 'bin', 'kick')
cmd = '%s -v 0 -dp -dw ref.ass -l "%s" -r 160 120 -set defaultColorMgtGlobals.native_catalog_path "%s" -t 64 -o testrender.exr' \
  % (kick, shaders_path, synColor_path)

print cmd

os.system(cmd)

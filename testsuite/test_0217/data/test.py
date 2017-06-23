import os
import sys

# This test validates kick with output transform. I've also added some not color managed textures 
# to stress a little bit more the synColor plugin (i.e. output & input color transformations).

# Also I've artificially increase the number of threads to stress the multi-thread mechanism used 
# by the synColor plugin cache.

# Execute the script

maya_root_path    = sys.argv[1]
mtoa_root_path    = sys.argv[2]

shaders_path = os.path.join(mtoa_root_path, "shaders")
synColor_path = os.path.join(maya_root_path, "synColor")

cmd = 'kick -v 0 -dp -dw ref.ass -l "%s" -r 160 120 -set defaultColorMgtGlobals.native_catalog_path "%s" -t 64 -o testrender.tif' \
 % (shaders_path, synColor_path)

print cmd

os.system(cmd)

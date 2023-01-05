import os
import sys

# This test validates kick with output transform. I've also added some not color managed textures 
# to stress a little bit more the synColor plugin (i.e. output & input color transformations).

# Also I've artificially increase the number of threads to stress the multi-thread mechanism used 
# by the synColor plugin cache.

# Execute the script

maya_root_path    = sys.argv[1]
mtoa_root_path    = sys.argv[2]

ocio_path = os.path.join(maya_root_path, 'resources', 'OCIO-configs', 'Maya2022-default', 'config.ocio')
kick = os.path.join(mtoa_root_path, 'bin', 'kick')
cmd = '%s -v 0 -dp -dw ref.ass -r 160 120 -set defaultColorMgtGlobals.config "%s" -o testrender.tif' \
  % (kick, ocio_path)

print(cmd)

os.system(cmd)

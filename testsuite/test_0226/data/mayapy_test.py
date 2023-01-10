import os
import sys

# Execute the script

maya_root_path    = sys.argv[1]
mtoa_root_path    = sys.argv[2]

kick = os.path.join(mtoa_root_path, 'bin', 'kick')
cmd = '{} -v 0 -dp -dw ref.ass -r 160 120 -set defaultColorMgtGlobals.config "ocio/config.ocio" -o testrender.tif'.format(kick)

print(cmd)

os.system(cmd)

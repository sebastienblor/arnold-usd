import sys
import os
import shutil

test_root_path = sys.argv[1]
oiiotool_path  = sys.argv[2]
mtoa_path = sys.argv[3]

cmd = '{} -dp -dw -frame 2 {}'.format(os.path.join(mtoa_path, 'bin', 'kick'), os.path.join(test_root_path, 'out.usda'))
os.system(cmd)
shutil.copyfile(os.path.join(test_root_path, 'data', 'testrender.tif'), os.path.join(test_root_path, 'testrender.tif') )



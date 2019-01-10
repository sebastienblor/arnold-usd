import sys
import os
test_root_path = sys.argv[1]
oiiotool_path  = sys.argv[2]
mtoa_path = sys.argv[3]

cmd = '{} -i {} -o {} -v 1'.format(os.path.join(mtoa_path, 'bin', 'noice'), os.path.join(test_root_path, 'testrender.exr'), os.path.join(test_root_path, 'denoised.exr'))
os.system(cmd)


cmd = ('%s %s -ch "R,G,B,A" -o %s') % (oiiotool_path, os.path.join(test_root_path, "denoised.exr"), os.path.join(test_root_path, "testrender.tif"))
os.system(cmd)

import sys
import os
test_root_path = sys.argv[1]
oiiotool_path  = sys.argv[2]

cmd = ('%s %s -ch "R,G,B,A" -o %s') % (oiiotool_path, os.path.join(test_root_path, "pSphereShape1.1.exr"), os.path.join(test_root_path, "testrender1.tif"))
os.system(cmd)

cmd = ('%s %s -ch "R,G,B,A" -o %s') % (oiiotool_path, os.path.join(test_root_path, "pSphereShape1.3.exr"), os.path.join(test_root_path, "testrender2.tif"))
os.system(cmd)

cmd = ('%s %s -ch "R,G,B" -o %s') % (oiiotool_path, os.path.join(test_root_path, "pSphereShape1.diffuse_direct.1.exr"), os.path.join(test_root_path, "testrender3.tif"))
os.system(cmd)

cmd = ('%s %s -ch "R,G,B" -o %s') % (oiiotool_path, os.path.join(test_root_path, "pSphereShape1.diffuse_direct.3.exr"), os.path.join(test_root_path, "testrender4.tif"))
os.system(cmd)

cmd = ('%s %s -ch "R,G,B,A" -o %s') % (oiiotool_path, os.path.join(test_root_path, "aiStandardSurface1_pSphereShape2_1001.02.exr"), os.path.join(test_root_path, "testrender5.tif"))
os.system(cmd)

cmd = ('%s %s -ch "R,G,B,A" -o %s') % (oiiotool_path, os.path.join(test_root_path, "aiStandardSurface1_pSphereShape2_1001.03.exr"), os.path.join(test_root_path, "testrender6.tif"))
os.system(cmd)

cmd = ('%s %s -ch "R,G,B" -o %s') % (oiiotool_path, os.path.join(test_root_path, "aiStandardSurface1_pSphereShape2_1001.diffuse_direct.02.exr"), os.path.join(test_root_path, "testrender7.tif"))
os.system(cmd)

cmd = ('%s %s -ch "R,G,B" -o %s') % (oiiotool_path, os.path.join(test_root_path, "aiStandardSurface1_pSphereShape2_1001.diffuse_direct.03.exr"), os.path.join(test_root_path, "testrender8.tif"))

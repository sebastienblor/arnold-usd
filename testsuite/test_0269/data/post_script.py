import sys

test_root_path = sys.argv[1]
oiiotool_path  = sys.argv[2]

cmd = ('%s %s -ch "R,G,B,A" -o %s') % (oiiotool_path, os.path.join(test_root_path, "testrender.exr"), os.path.join(test_root_path, "testrender.tif"))
os.system(cmd)




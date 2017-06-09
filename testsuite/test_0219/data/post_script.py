import sys

test_root_path = sys.argv[1]
oiiotool_path  = sys.argv[2]

src_file_path = os.path.join(test_root_path, 'testrender.exr')
dst_file_path = os.path.join(test_root_path, 'testrender.tif')

cmd = ('%s %s -ch "R,G,B,A" -o %s') % (oiiotool_path, src_file_path, dst_file_path)

os.system(cmd)

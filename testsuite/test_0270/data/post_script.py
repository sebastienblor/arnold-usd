import os
test_root_path = sys.argv[1]
oiiotool_path  = sys.argv[2]
mtoa_path = sys.argv[3]

cmd = '{} -dp -dw {}'.format(os.path.join(mtoa_path, 'bin', 'kick'), os.path.join(test_root_path, 'test_camera1.ass'))
os.system(cmd)

cmd = '{} -dp -dw {}'.format(os.path.join(mtoa_path, 'bin', 'kick'), os.path.join(test_root_path, 'test_camera2.ass'))
os.system(cmd)






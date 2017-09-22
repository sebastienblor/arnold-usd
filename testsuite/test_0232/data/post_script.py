import sys

test_root_path = sys.argv[1]
oiiotool_path  = sys.argv[2]

cmd = ('%s %s -ch "R,G,B,A" -o %s') % (oiiotool_path, os.path.join(test_root_path, "testrenderTorusShape.exr"), os.path.join(test_root_path, "testrender1.tif"))
os.system(cmd)

cmd = ('%s %s -ch "R,G,B,A" -o %s') % (oiiotool_path, os.path.join(test_root_path, "testrenderTorus1Shape.exr"), os.path.join(test_root_path, "testrender2.tif"))
os.system(cmd)

cmd = ('%s %s -ch "R,G,B,A" -o %s') % (oiiotool_path, os.path.join(test_root_path, "testrenderSphereShape.exr"), os.path.join(test_root_path, "testrender3.tif"))
os.system(cmd)

cmd = ('%s %s -ch "R,G,B,A" -o %s') % (oiiotool_path, os.path.join(test_root_path, "testrenderSphere1Shape.exr"), os.path.join(test_root_path, "testrender4.tif"))
os.system(cmd)

cmd = ('%s %s -ch "R,G,B,A" -o %s') % (oiiotool_path, os.path.join(test_root_path, "testrenderSphere2Shape.exr"), os.path.join(test_root_path, "testrender5.tif"))
os.system(cmd)

cmd = ('%s %s -ch "R,G,B,A" -o %s') % (oiiotool_path, os.path.join(test_root_path, "testrenderAovShape.exr"), os.path.join(test_root_path, "testrender6.tif"))
os.system(cmd)

cmd = ('%s %s -ch "R,G,B" -o %s') % (oiiotool_path, os.path.join(test_root_path, "testrenderAovShape.diffuse_direct.exr"), os.path.join(test_root_path, "testrender7.tif"))
os.system(cmd)

cmd = ('%s %s -ch "R,G,B" -o %s') % (oiiotool_path, os.path.join(test_root_path, "testrenderAovShape.emission.exr"), os.path.join(test_root_path, "testrender8.tif"))
os.system(cmd)

cmd = ('%s %s -ch "R,G,B" -o %s') % (oiiotool_path, os.path.join(test_root_path, "testrenderAovShape.specular_direct.exr"), os.path.join(test_root_path, "testrender9.tif"))
os.system(cmd)

cmd = ('%s %s -ch "R,G,B,A" -o %s') % (oiiotool_path, os.path.join(test_root_path, "aiStandardSurface1_testrenderTorus2Shape_1001.exr"), os.path.join(test_root_path, "testrender10.tif"))
os.system(cmd)

cmd = ('%s %s -ch "R,G,B,A" -o %s') % (oiiotool_path, os.path.join(test_root_path, "aiStandardSurface1_testrenderTorus3Shape_1001.exr"), os.path.join(test_root_path, "testrender11.tif"))
os.system(cmd)

cmd = ('%s %s -ch "R,G,B,A" -o %s') % (oiiotool_path, os.path.join(test_root_path, "aiStandardSurface1_testrenderTorus3Shape_1012.exr"), os.path.join(test_root_path, "testrender12.tif"))
os.system(cmd)

cmd = ('%s %s -ch "R,G,B,A" -o %s') % (oiiotool_path, os.path.join(test_root_path, "testrenderSphere3Shape.exr"), os.path.join(test_root_path, "testrender13.tif"))
os.system(cmd)


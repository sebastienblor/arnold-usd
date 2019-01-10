import sys
import os
import shutil

test_root_path = sys.argv[1]
oiiotool_path  = sys.argv[2]
mtoa_path = sys.argv[3]


os.environ['PATH']= '{}:{}'.format(os.environ['PATH'], os.path.join(mtoa_path, 'bin'))
sys.path.append(os.path.join(mtoa_path, 'scripts'))

from arnold import *

print 'Testing original scene'
AiBegin()
AiASSLoad('test.ass')
success = True

outputsArray = AiNodeGetArray(AiUniverseGetOptions(), "outputs")

numElems = AiArrayGetNumElements(outputsArray)
if numElems != 7:
	success = False
else:
	success = success and AiArrayGetStr(outputsArray, 0) == 'RGBA RGBA defaultArnoldFilter@gaussian_filter defaultArnoldDriver@driver_exr.RGBA'
	success = success and AiArrayGetStr(outputsArray, 1) == 'RGBA_denoise RGBA defaultArnoldRenderOptions@RGBA_denoise defaultArnoldDriver@driver_exr.RGBA_denoise'
	success = success and AiArrayGetStr(outputsArray, 2) == 'albedo RGB defaultArnoldFilter@gaussian_filter defaultArnoldDriver@driver_exr.albedo'
	success = success and AiArrayGetStr(outputsArray, 3) == 'diffuse RGB defaultArnoldFilter@gaussian_filter defaultArnoldDriver@driver_exr.diffuse'
	success = success and AiArrayGetStr(outputsArray, 4) == 'diffuse_denoise RGB defaultArnoldRenderOptions@diffuse_denoise defaultArnoldDriver@driver_exr.diffuse_denoise'
	success = success and AiArrayGetStr(outputsArray, 5) == 'specular RGB defaultArnoldFilter@gaussian_filter defaultArnoldDriver@driver_exr.specular'
	success = success and AiArrayGetStr(outputsArray, 6) == 'specular_denoise RGB defaultArnoldRenderOptions@specular_denoise defaultArnoldDriver@driver_exr.specular_denoise'

if success:
	print 'OK'
	shutil.copy('ok.tif', 'testrender.tif')
else:
	print 'FAIL'
	shutil.copy('fail.tif', 'testrender.tif')






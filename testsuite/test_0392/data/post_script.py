import sys
import os
import shutil

test_root_path = sys.argv[1]
oiiotool_path  = sys.argv[2]
mtoa_path = sys.argv[3]


os.environ['PATH']= '{}:{}'.format(os.environ['PATH'], os.path.join(mtoa_path, 'bin'))
sys.path.append(os.path.join(mtoa_path, 'scripts'))

from arnold import *

AiBegin()
AiASSLoad('test.ass')
success = True

beautyDriver = AiNodeLookUpByName('defaultArnoldDriver/driver_exr.RGBA')
denoisedDriver = AiNodeLookUpByName('defaultArnoldDriver/driver_exr.RGBA_denoise')
albedoDriver = AiNodeLookUpByName('defaultArnoldDriver/driver_exr.albedo')
imager = AiNodeLookUpByName('aiImagerColorCorrect1')

imagerLayer = AiNodeGetStr(imager, 'layer_selection')
if imagerLayer != 'RGBA or RGBA_denoise':
    success = False
    print 'Imager has not set layer_selection to "RGBA or RGBA_denoise"'

beautyImager = AiNodeGetPtr(beautyDriver, 'input')
if beautyImager is None or AiNodeGetName(beautyImager) != AiNodeGetName(imager):
    success = False
    print 'Beauty does not have any imager applied'	

denoisedImager = AiNodeGetPtr(denoisedDriver, 'input')
if denoisedImager is None or AiNodeGetName(denoisedImager) != AiNodeGetName(imager):
    success = False
    print 'Denoised beauty does not have any imager applied'

albedoImager = AiNodeGetPtr(albedoDriver, 'input')
if albedoImager != None:
    success = False
    print 'Albedo should not have an imager applied'

if success:
	print 'OK'
	shutil.copy('ok.tif', 'testrender.tif')
else:
	print 'FAIL'
	shutil.copy('fail.tif', 'testrender.tif')






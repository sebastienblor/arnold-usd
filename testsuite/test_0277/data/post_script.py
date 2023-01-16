import sys
import os
import shutil

test_root_path = sys.argv[1]
oiiotool_path  = sys.argv[2]
mtoa_path = sys.argv[3]


os.environ['PATH']= '{}:{}'.format(os.environ['PATH'], os.path.join(mtoa_path, 'bin'))
sys.path.append(os.path.join(mtoa_path, 'scripts'))

from arnold import *

initArnold = False
if not AiArnoldIsActive():
	initArnold = True
	AiBegin(AI_SESSION_INTERACTIVE)
AiSceneLoad(None, 'scene.ass', None)
success = True

success = success and AiNodeLookUpByName(None, 'lambert1')
for i in range(1,10):
	shaderName = 'aiShader{}'.format(i)
	success = success and AiNodeLookUpByName(None, shaderName)

success = success and AiNodeLookUpByName(None, 'aiStandardSurface1SG')
success = success and AiNodeLookUpByName(None, 'aiStandardSurface1SG/aov1')
success = success and AiNodeLookUpByName(None, 'aiImage1')
success = success and AiNodeLookUpByName(None, 'aiNoise1')

print('Success is %s' % success)

if success:
	shutil.copy('ok.tif', 'testrender.tif')
else:
	shutil.copy('fail.tif', 'testrender.tif')

if initArnold:
	AiEnd()





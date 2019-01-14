import sys
import os
import shutil

test_root_path = sys.argv[1]
oiiotool_path  = sys.argv[2]
mtoa_path = sys.argv[3]


os.environ['PATH']= '{}:{}'.format(os.environ['PATH'], os.path.join(mtoa_path, 'bin'))
sys.path.append(os.path.join(mtoa_path, 'scripts'))

from arnold import *

AiBegin(AI_SESSION_INTERACTIVE)
AiASSLoad('scene.ass')
success = True

success = success and AiNodeLookUpByName('lambert1')
for i in range(1,10):
	shaderName = 'aiShader{}'.format(i)
	success = success and AiNodeLookUpByName(shaderName)

success = success and AiNodeLookUpByName('aiStandardSurface1SG')
success = success and AiNodeLookUpByName('aiStandardSurface1SG@aov1')
success = success and AiNodeLookUpByName('aiImage1')
success = success and AiNodeLookUpByName('aiNoise1')

print 'Success is %s' % success

if success:
	shutil.copy('ok.tif', 'testrender.tif')
else:
	shutil.copy('fail.tif', 'testrender.tif')

AiEnd()





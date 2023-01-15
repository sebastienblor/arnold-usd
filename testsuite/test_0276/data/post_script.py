import sys
import os
import shutil

test_root_path = sys.argv[1]
oiiotool_path  = sys.argv[2]
mtoa_path = sys.argv[3]



previousPath = os.environ['PATH']

if previousPath and len(previousPath) > 0:
	os.environ['PATH']= previousPath + ':' + os.path.join(mtoa_path, 'bin')
else:
	os.environ['PATH']= os.path.join(mtoa_path, 'bin')

previousSysPath = sys.path

sys.path.append(os.path.join(mtoa_path, 'scripts'))

success = True

try:
	from arnold import *

	print('Testing original scene')
	if AiArnoldIsActive():
		AiEnd()
	AiBegin()
	
	AiSceneLoad(None, 'test.ass', None)
	

	success = success and AiNodeLookUpByName(None, 'perspShape')
	success = success and AiNodeLookUpByName(None, 'directionalLightShape1')
	success = success and AiNodeLookUpByName(None, 'pSphereShape1')
	success = success and AiNodeLookUpByName(None, 'pCube1|pCubeShape1')
	success = success and AiNodeLookUpByName(None, 'pCube2|pCubeShape1')
	success = success and AiNodeLookUpByName(None,'lambert1')
	AiEnd()

	print('Testing Full Path Export')
	AiBegin()
	AiSceneLoad(None, 'test_fp.ass', None)
	success = True
	success = success and AiNodeLookUpByName(None, '|persp|perspShape')
	success = success and AiNodeLookUpByName(None, '|directionalLight1|directionalLightShape1')
	success = success and AiNodeLookUpByName(None, '|pSphere1|pSphereShape1')
	success = success and AiNodeLookUpByName(None, '|pCube1|pCubeShape1')
	success = success and AiNodeLookUpByName(None, '|pCube2|pCubeShape1')
	success = success and AiNodeLookUpByName(None, 'lambert1')
	AiEnd()

	print('Testing Name Prefix')
	AiBegin()
	AiSceneLoad(None, 'test_pr.ass', None)
	success = True
	success = success and AiNodeLookUpByName(None, 'pref_perspShape')
	success = success and AiNodeLookUpByName(None, 'pref_directionalLightShape1')
	success = success and AiNodeLookUpByName(None, 'pref_pSphereShape1')
	success = success and AiNodeLookUpByName(None, 'pref_pCube1|pCubeShape1')
	success = success and AiNodeLookUpByName(None, 'pref_pCube2|pCubeShape1')
	success = success and AiNodeLookUpByName(None, 'pref_lambert1')
	AiEnd()

	print('Testing Full path AND Name Prefix')
	AiBegin()
	AiSceneLoad(None, 'test_fp_pr.ass', None)
	success = True
	success = success and AiNodeLookUpByName(None, 'pref_|persp|perspShape')
	success = success and AiNodeLookUpByName(None, 'pref_|directionalLight1|directionalLightShape1')
	success = success and AiNodeLookUpByName(None, 'pref_|pSphere1|pSphereShape1')
	success = success and AiNodeLookUpByName(None, 'pref_|pCube1|pCubeShape1')
	success = success and AiNodeLookUpByName(None, 'pref_|pCube2|pCubeShape1')
	success = success and AiNodeLookUpByName(None, 'pref_lambert1')
	AiEnd()

	if success:
		print('OK')
		shutil.copy('ok.tif', 'testrender.tif')
	else:
		print('FAIL')
		shutil.copy('fail.tif', 'testrender.tif')

except RuntimeError as err:
	print(err)

sys.path = previousSysPath
os.environ['PATH'] = previousPath
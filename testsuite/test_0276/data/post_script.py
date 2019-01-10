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

	print 'Testing original scene'
	AiBegin()
	AiASSLoad('test.ass')
	

	success = success and AiNodeLookUpByName('perspShape')
	success = success and AiNodeLookUpByName('directionalLightShape1')
	success = success and AiNodeLookUpByName('pSphereShape1')
	success = success and AiNodeLookUpByName('pCube1|pCubeShape1')
	success = success and AiNodeLookUpByName('pCube2|pCubeShape1')
	success = success and AiNodeLookUpByName('lambert1')
	AiEnd()

	print 'Testing Full Path Export'
	AiBegin()
	AiASSLoad('test_fp.ass')
	success = True
	success = success and AiNodeLookUpByName('|persp|perspShape')
	success = success and AiNodeLookUpByName('|directionalLight1|directionalLightShape1')
	success = success and AiNodeLookUpByName('|pSphere1|pSphereShape1')
	success = success and AiNodeLookUpByName('|pCube1|pCubeShape1')
	success = success and AiNodeLookUpByName('|pCube2|pCubeShape1')
	success = success and AiNodeLookUpByName('lambert1')
	AiEnd()

	print 'Testing Name Prefix'
	AiBegin()
	AiASSLoad('test_pr.ass')
	success = True
	success = success and AiNodeLookUpByName('pref_perspShape')
	success = success and AiNodeLookUpByName('pref_directionalLightShape1')
	success = success and AiNodeLookUpByName('pref_pSphereShape1')
	success = success and AiNodeLookUpByName('pref_pCube1|pCubeShape1')
	success = success and AiNodeLookUpByName('pref_pCube2|pCubeShape1')
	success = success and AiNodeLookUpByName('pref_lambert1')
	AiEnd()

	print 'Testing Full path AND Name Prefix'
	AiBegin()
	AiASSLoad('test_fp_pr.ass')
	success = True
	success = success and AiNodeLookUpByName('pref_|persp|perspShape')
	success = success and AiNodeLookUpByName('pref_|directionalLight1|directionalLightShape1')
	success = success and AiNodeLookUpByName('pref_|pSphere1|pSphereShape1')
	success = success and AiNodeLookUpByName('pref_|pCube1|pCubeShape1')
	success = success and AiNodeLookUpByName('pref_|pCube2|pCubeShape1')
	success = success and AiNodeLookUpByName('pref_lambert1')
	AiEnd()

	if success:
		print 'OK'
		shutil.copy('ok.tif', 'testrender.tif')
	else:
		print 'FAIL'
		shutil.copy('fail.tif', 'testrender.tif')

except RuntimeError, err:
	print err

sys.path = previousSysPath
os.environ['PATH'] = previousPath
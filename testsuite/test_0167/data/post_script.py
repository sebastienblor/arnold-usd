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

textMesh = AiNodeLookUpByName('/typeMesh1/typeMeshShape1')
if textMesh is None:
    success = False
    print('No mesh has been exported with the expected name')

elif not AiNodeIs(textMesh, 'polymesh'):
    success = False
    print('Exported text node is not a polymesh')
else:
    holesArray = AiNodeGetArray(textMesh, 'polygon_holes')

    if holesArray is None:
        success = False
        print('No holes array has been exported')
    elif AiArrayGetNumElements(holesArray) != 12:
        success = False
        print('Not the right amount of holes in the text node. Found {} instead of 12'.format(AiArrayGetNumElements(holesArray)))

if success:
    print('OK')
    shutil.copy('ok.tif', 'testrender.tif')
else:
    print('FAIL')
    shutil.copy('fail.tif', 'testrender.tif')






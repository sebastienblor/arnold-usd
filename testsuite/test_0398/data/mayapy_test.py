import maya.standalone
maya.standalone.initialize()

import os
import sys

import maya.cmds as cmds
import maya.mel as mel
import arnold as ai
# Execute the script

maya_root_path    = sys.argv[1]
mtoa_root_path    = sys.argv[2]

shaders_path = os.path.join(mtoa_root_path, "shaders")
synColor_path = os.path.join(maya_root_path, "synColor")

cmds.loadPlugin('mtoa', quiet=True)
cmds.arnoldScene(mode="create")
tmp_scene = os.path.join(os.path.dirname(os.path.realpath(__file__)), "test1.ma")

cmds.file(tmp_scene, o=True, f=True)
resultColor = '0,1,0'

cmds.arnoldScene(['perspShape', 'pSphere1', 'camera1', 'camera2', 'directionalLight1', 'spotLight1', 'pointLightShape1', 'areaLightShape1'], mode="convert_selected")
nodeList = []
nodeReferenceList = ['/camera1/cameraShape1', '/camera2/cameraShape2', '/directionalLight1/directionalLightShape1', '/spotLight1/spotLightShape1', '/pointLight1/pointLightShape1', '/areaLight1/areaLightShape1', '/pSphere1/pSphereShape1', 'lambert1']
iter = ai.AiUniverseGetNodeIterator(None, ai.AI_NODE_SHAPE + ai.AI_NODE_CAMERA + ai.AI_NODE_LIGHT + ai.AI_NODE_SHADER)
while not ai.AiNodeIteratorFinished(iter):
    node = ai.AiNodeIteratorGetNext(iter)
    nodeName = ai.AiNodeGetName(node)
    if nodeName == 'root' or nodeName == '' or nodeName == 'ai_default_reflection_shader':
        continue
    nodeList.append(nodeName)

ai.AiNodeIteratorDestroy(iter)
success = True
print(nodeList)
for node in nodeList:
    if not node in nodeReferenceList:
        resultColor = '1,0,0'
        success = False
for node in nodeReferenceList:
    if not node in nodeList:
        resultColor = '1,0,0'
        success = False

if not success:
    print('Reference is {}'.format(nodeReferenceList))

cmds.arnoldScene(mode="destroy")
cmds.arnoldScene(mode="create")

post_script = 'import sys\nimport os\ntest_root_path = sys.argv[1]\noiiotool_path  = sys.argv[2]\n'
post_script += 'cmd = oiiotool_path\n'
post_script += 'cmd += " --create 160x120 3 -d uint8 --fill:color={} 160x120 -o "\n'.format(resultColor)
post_script += 'cmd += os.path.join(test_root_path, "testrender.tif")\n'
post_script += 'print(cmd)\n'
post_script += '\nos.system(cmd)'

print('post_script')
with open('post_script.py','w') as f:
    f.write(post_script)

maya.standalone.uninitialize()

import maya.standalone
maya.standalone.initialize()

import os
import sys

import maya.cmds as cmds
import maya.mel as mel
# Execute the script

whiteList = ['directionalLight.aiExposure',
			'directionalLight.aiAngle',
			'spotLight.aiExposure',
			'spotLight.aiRadius',
			'pointLight.aiExposure',
			'pointLight.aiRadius',
			'areaLight.aiExposure',
			'areaLight.aiSpread',
			'areaLight.aiSoftEdge',
			'areaLight.aiRoundness',
			'volumeLight.aiExposure',
			'volumeLight.aiRadius'
			]

def testMayaNode(nodeName):
	success = True
	keyableAttrs = cmds.listAttr(nodeName, fp=True, k=True) or []
	channelBoxAttrs = cmds.listAttr(nodeName, fp=True, cb=True) or []
	attrs = keyableAttrs + channelBoxAttrs
	for attr in attrs:
		if attr[:2] != 'ai':
			continue

		if '.' in attr:
			continue

		attrType = cmds.attributeQuery(attr, node=nodeName, at=True )
		# ignore parameters of type message as they will always be set as keyable (not sure why...)
		if attrType == 'message':
			continue

		objType = cmds.objectType(nodeName)
		fullAttr = '{}.{}'.format(objType, attr)
		if fullAttr in whiteList:
			continue

		print 'WARNING: Parameter {} might appear in the channel Box'.format(fullAttr)
		success = False
		
	return success

maya_root_path    = sys.argv[1]
mtoa_root_path    = sys.argv[2]

shaders_path = os.path.join(mtoa_root_path, "shaders")
synColor_path = os.path.join(maya_root_path, "synColor")

cmds.loadPlugin('mtoa', quiet=True)
resultColor = '0,1,0'
success = True


sphereObjects = cmds.polySphere(r=True)
sphereShapes = cmds.listRelatives(sphereObjects[0],allDescendents=True)


for sphereShape in sphereShapes:
	success = (testMayaNode(sphereShape) and success)

createdNodes = ['mesh',
				'camera',
				'stereoRigCamera',
				'nurbsSurface',
				'nurbsCurve',
				'transform',
				'directionalLight',
				'spotLight',
				'pointLight',
				'areaLight',
				'volumeLight',
				'ambientLight',
				'pfxHair',
				'particle',
				'nParticle',
				'instancer',
				'objectSet',
				'fluidShape',
				'file',
				'bump2d',
				'projection',
				'ramp',
				'imagePlane',
				'shadingEngine',
				'displacementShader',
				'hairSystem']


for nodeType in createdNodes:
	newNode = cmds.createNode(nodeType)
	success = (testMayaNode(newNode) and success)


if success == False:
	resultColor = '1,0,0'


post_script = 'import sys\nimport os\ntest_root_path = sys.argv[1]\noiiotool_path  = sys.argv[2]\n'
post_script += 'cmd = oiiotool_path\n'
post_script += 'cmd += " --create 160x120 3 -d uint8 --fill:color={} 160x120 -o "\n'.format(resultColor)
post_script += 'cmd += os.path.join(test_root_path, "testrender.tif")\n'
post_script += 'print cmd\n'
post_script += '\nos.system(cmd)'

print 'post_script'
with open('post_script.py','w') as f:
    f.write(post_script)


maya.standalone.uninitialize()




import maya.standalone
maya.standalone.initialize()

import os
import sys

import maya.cmds as cmds
import maya.mel as mel
# Execute the script

maya_root_path    = sys.argv[1]
mtoa_root_path    = sys.argv[2]

shaders_path = os.path.join(mtoa_root_path, "shaders")
synColor_path = os.path.join(maya_root_path, "synColor")

cmds.loadPlugin('mtoa', quiet=True)
mel.eval('CreatePolygonSphere')
mel.eval('CreateNURBSCircle')
mel.eval('CreateNURBSCircle')

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
				'shadingEngine',
				'displacementShader',
				'hairSystem']

for node in createdNodes:
	cmds.createNode(node)

tmp_scene = os.path.join(os.path.dirname(os.path.realpath(__file__)), "test1.ma")

cmds.file(rename=tmp_scene)
tmp_scene = cmds.file(force=True, save=True, type='mayaAscii')

cmds.unloadPlugin('mtoa', force=True)

resultColor = '0,1,0'
if cmds.pluginInfo( 'mtoa', query=True, loaded=True ):
	print "MTOA DIDN'T UNLOAD"
	resultColor='1,1,0'
else:
	cmds.file(tmp_scene, o=True, f=True)
	if cmds.pluginInfo( 'mtoa', query=True, loaded=True ):
		print "MTOA WAS LOADED DESPITE NOT BEING USED"
		resultColor='1,0,0'
	

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




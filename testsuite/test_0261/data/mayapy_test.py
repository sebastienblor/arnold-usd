import maya.standalone
maya.standalone.initialize()

import os
import sys

import maya.cmds as cmds
# Execute the script

maya_root_path    = sys.argv[1]
mtoa_root_path    = sys.argv[2]

shaders_path = os.path.join(mtoa_root_path, "shaders")
synColor_path = os.path.join(maya_root_path, "synColor")

scene = os.path.join(os.path.dirname(os.path.realpath(__file__)), "test1.ma")
cmds.file(scene, o=True, f=True)
cmds.loadPlugin('mtoa', quiet=True)

cmds.createNode('mesh')
cmds.createNode('camera')
cmds.createNode('stereoRigCamera')
cmds.createNode('nurbsSurface')
cmds.createNode('nurbsCurve')
cmds.createNode('transform')
cmds.createNode('directionalLight')
cmds.createNode('spotLight')
cmds.createNode('pointLight')
cmds.createNode('areaLight')
cmds.createNode('volumeLight')
cmds.createNode('ambientLight')
cmds.createNode('pfxHair')
cmds.createNode('particle')
cmds.createNode('nParticle')
cmds.createNode('instancer')
cmds.createNode('objectSet')
cmds.createNode('fluidShape')
cmds.createNode('file')
cmds.createNode('bump2d')
cmds.createNode('projection')
cmds.createNode('ramp')
cmds.createNode('shadingEngine')
cmds.createNode('displacementShader')
cmds.createNode('hairSystem')

arnoldNodes = ['aiStandIn',
				'aiAreaLight',
				#'aiSkyDomeLight',  #CRASHING
				'aiCurveCollector',
				#'aiVolume',  #CRASHING
				'aiStandardSurface',
				'aiImage',
				'aiPhotometricLight',
				'aiLightPortal',
				'aiPhysicalSky',
				'aiAOV',
				'aiAOVFilter',
				'aiAOVDriver']



for arnoldNode in arnoldNodes:
	cmds.createNode(arnoldNode)


cmds.unloadPlugin('mtoa', force=True)

tmp_scene = os.path.join(os.path.dirname(os.path.realpath(__file__)), "test2.ma")
cmds.file(rename=tmp_scene)

tmp_scene = cmds.file(force=True, save=True, type='mayaAscii')

resultColor = '0,1,0'
if cmds.pluginInfo( 'mtoa', query=True, loaded=True ):
	print "MTOA DIDN'T UNLOAD"
	resultColor='1,1,0'
else:
	cmds.file(tmp_scene, o=True, f=True)
	if cmds.pluginInfo( 'mtoa', query=True, loaded=True ):
		print "REQUIRES MTOA PRESENT"
		resultColor='1,0,0'
	else:

		objects = cmds.ls()

		for obj in objects:
			nodeType = cmds.nodeType(obj)
			if nodeType in arnoldNodes:
				print "ERROR ARNOLD NODE REMAINING %s "%nodeType
				resultColor = '1,0,1'
				continue

			if cmds.attributeQuery('aiUserOptions', node=obj, exists=True):
				resultColor = '0,1,1'
				print "ERROR MAYA NODE REMAINING ATTRs"

			if cmds.attributeQuery('aiTranslator', node=obj, exists=True):
				resultColor = '0,1,1'
				print "ERROR MAYA NODE REMAINING ATTRs"



post_script = 'import sys\ntest_root_path = sys.argv[1]\noiiotool_path  = sys.argv[2]\n'
post_script += 'cmd = oiiotool_path\n'
post_script += 'cmd += " --create 160x120 3 -d uint8 --fill:color={} 160x120 -o "\n'.format(resultColor)
post_script += 'cmd += os.path.join(test_root_path, "testrender.tif")\n'
post_script += 'print cmd\n'
post_script += '\nos.system(cmd)'

print 'post_script'
with open('post_script.py','w') as f:
    f.write(post_script)

maya.standalone.uninitialize()




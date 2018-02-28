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

print 'loading MtoA'
cmds.loadPlugin('mtoa', quiet=True)
cmds.polySphere(r=1,sx=20,sy=20,cuv=2,ch= 1, n='sphere')
print '---setting aiOpaque attribute'
cmds.setAttr('sphereShape.aiOpaque', 0)

tmp_scene = os.path.join(os.path.dirname(os.path.realpath(__file__)), "test1.ma")

print '---saving test1.ma'
cmds.file(rename=tmp_scene)
tmp_scene = cmds.file(force=True, save=True, type='mayaAscii')

print '---unloading mtoa'
cmds.unloadPlugin('mtoa', force=True)

resultColor = '0,1,0'
if cmds.pluginInfo( 'mtoa', query=True, loaded=True ):
	print "MTOA DIDN'T UNLOAD"
	resultColor='1,1,0'
else:
	print '---mtoa unloaded properly'
	print '---opening test1.ma'
	cmds.file(tmp_scene, o=True, f=True)
	if not cmds.pluginInfo( 'mtoa', query=True, loaded=True ):
		print "REQUIRES MTOA NOT PRESENT"
		resultColor='1,0,0'
	

post_script = 'import sys\ntest_root_path = sys.argv[1]\noiiotool_path  = sys.argv[2]\n'
post_script += 'cmd = oiiotool_path\n'
post_script += 'cmd += " --create 160x120 3 -d uint8 --fill:color={} 160x120 -o "\n'.format(resultColor)
post_script += 'cmd += os.path.join(test_root_path, "testrender.tif")\n'
post_script += 'print cmd\n'
post_script += '\nos.system(cmd)'

print '---running post script'
with open('post_script.py','w') as f:
    f.write(post_script)

maya.standalone.uninitialize()




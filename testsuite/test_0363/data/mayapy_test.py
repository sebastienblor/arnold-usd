import maya.standalone
maya.standalone.initialize()

import os
import sys

import maya.cmds as cmds
import maya.mel as mel
# Execute the script
import mtoa.utils
import mtoa.core


maya_root_path    = sys.argv[1]
mtoa_root_path    = sys.argv[2]

shaders_path = os.path.join(mtoa_root_path, "shaders")
synColor_path = os.path.join(maya_root_path, "synColor")

cmds.loadPlugin('mtoa', quiet=True)


curdir = os.path.abspath(os.path.curdir)

cmds.file(rename=os.path.join(curdir,"ref.ma"))

# create a sube and add attributes

trans, cubeObj = cmds.polyCube()

cubeShape = cmds.ls(trans, dag=True, ap=True)[1]

cmds.addAttr( cubeShape, longName='mtoa_constant_int8', attributeType='byte' )
cmds.addAttr( cubeShape, longName='mtoa_constant_int16', attributeType='short' )
cmds.addAttr( cubeShape, longName='mtoa_constant_int32', attributeType='long' )
cmds.addAttr( cubeShape, longName='mtoa_constant_float', attributeType='float' )
cmds.addAttr( cubeShape, longName='mtoa_constant_bool', attributeType='bool' )

cmds.addAttr( cubeShape, longName='mtoa_constant_string', dataType='string' )

cmds.addAttr( cubeShape, longName='mtoa_constant_vec2', attributeType='float2' )
cmds.addAttr( cubeShape,  longName='vec2_a', attributeType='float', parent='mtoa_constant_vec2' )
cmds.addAttr( cubeShape,  longName='vec2_b', attributeType='float', parent='mtoa_constant_vec2' )
cmds.setAttr('{}.mtoa_constant_vec2'.format(cubeShape), 0.1, 0.2, type='float2')

cmds.addAttr( cubeShape, longName='mtoa_constant_vec', attributeType='float3' )
cmds.addAttr( cubeShape,  longName='vec_x', attributeType='float', parent='mtoa_constant_vec' )
cmds.addAttr( cubeShape,  longName='vec_y', attributeType='float', parent='mtoa_constant_vec' )
cmds.addAttr( cubeShape,  longName='vec_z', attributeType='float', parent='mtoa_constant_vec' )
cmds.setAttr('{}.mtoa_constant_vec'.format(cubeShape), 0.0, 1.0, 0.0, type='float3')

cmds.addAttr( cubeShape, longName='mtoa_constant_Cd', usedAsColor=True, attributeType='float3' )
cmds.addAttr( cubeShape,  longName='red', attributeType='float', parent='mtoa_constant_Cd' )
cmds.addAttr( cubeShape,  longName='green', attributeType='float', parent='mtoa_constant_Cd' )
cmds.addAttr( cubeShape,  longName='blue', attributeType='float', parent='mtoa_constant_Cd' )
cmds.setAttr('{}.mtoa_constant_Cd'.format(cubeShape), 0.0, 1.0, 1.0, type='float3')

cmds.addAttr( cubeShape, longName='mtoa_uniform_faceAttr', dataType='doubleArray' )
cmds.setAttr('{}.mtoa_uniform_faceAttr'.format(cubeShape), [0.1, 0.2, 0.3, 0.4, 0.5, 0.6], type='doubleArray')

cmds.addAttr( cubeShape, longName='mtoa_varying_vertexAttr', dataType='doubleArray' )
cmds.setAttr('{}.mtoa_varying_vertexAttr'.format(cubeShape), [0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8], type='doubleArray')

cmds.setAttr('{}.aiSubdivType'.format(cubeShape), 1)
cmds.setAttr('{}.aiSubdivIterations'.format(cubeShape), 4)


# EXPORT alembic
cube_out = os.path.join(curdir, "test_cube.abc")

cmds.arnoldExportAlembic(v=True, j="-root |pCube1 -attrPrefix mtoa_ -writeColorSets -uvwrite -file {}".format(cube_out))

# load it back in t a standIn node
standIn = cmds.createNode("aiStandIn")
cmds.setAttr("{}.dso".format(standIn), cube_out, type="string")

s_trans = cmds.listRelatives(standIn, p=True)[0]
cmds.xform(s_trans, a=True, t=[1.5,0,0] )

# set up render
mtoa.core.createOptions()

cam, camshape = cmds.camera(name="renderCam")
cmds.xform(cam, a=True, t=[.8,0,3] )

sky, skyShape = mtoa.utils.createLocator("aiSkyDomeLight", asLight=True)
cmds.setAttr("{}.camera".format(sky), 0)

# create shader network

testSG = cmds.sets( r=True, em=True )
testMat = cmds.shadingNode( "aiFlat", asShader=True )
cmds.connectAttr( "{}.outColor".format(testMat), "{}.surfaceShader".format(testSG), f=True )

userData = cmds.shadingNode("aiUserDataColor", asUtility=True)
cmds.setAttr( "{}.attribute".format(userData), "Cd", type='string' )
cmds.connectAttr( "{}.outColor".format(userData), "{}.color".format(testMat), f=True )

cmds.sets( cubeShape, e=True, fe=testSG )
cmds.sets( standIn, e=True, fe=testSG )

cmds.setAttr("{}.renderable".format(camshape), 1)
cmds.setAttr("perspShape.renderable".format(camshape), 0)

cmds.setAttr("defaultArnoldDriver.aiTranslator", "tif", type="string")
cmds.setAttr("defaultRenderGlobals.imageFilePrefix", os.path.join(curdir,"testrender"), type="string" )


cmds.setAttr("defaultResolution.width", 160 )
cmds.setAttr("defaultResolution.height", 120 )
cmds.setAttr("defaultResolution.pixelAspect", 1 )
cmds.setAttr("defaultArnoldRenderOptions.log_verbosity", 3)

cmds.file(force=True, type="mayaAscii", save=True)

cmds.arnoldRender(batch=True, camera=camshape)

maya.standalone.initialize()

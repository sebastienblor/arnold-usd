import maya.standalone
maya.standalone.initialize()

import os
import sys
import traceback

import maya.cmds as cmds
import maya.mel as mel
# Execute the script

maya_root_path    = sys.argv[1]
mtoa_root_path    = sys.argv[2]

cmds.loadPlugin('mtoa', quiet=True)

successColor = '0,1,0'
failColor = '1,0,0'
resultColor = successColor

try:
    import mtoa
    import mtoa.api
    import mtoa.api.extensions

    
    # Commands scripts
    import mtoa.cmds
    import mtoa.cmds.arnoldRender
    import mtoa.cmds.arnoldShelf
    import mtoa.cmds.registerArnoldRenderer
    import mtoa.cmds.rendererCallbacks
    import mtoa.cmds.unregisterArnoldRenderer

    # base modules
    import mtoa.aovs
    import mtoa.batchRenderOptions
    import mtoa.callbacks
    import mtoa.convertShaders
    import mtoa.core
    import mtoa.denoise
    import mtoa.hooks
    import mtoa.licensing
    import mtoa.lightFilters
    import mtoa.lightManager
    import mtoa.makeTx
    import mtoa.materialx
    import mtoa.melUtils
    import mtoa.operators
    import mtoa.osl
    import mtoa.renderToTexture
    import mtoa.utils
    import mtoa.viewport

    # UI scripts
    import mtoa.ui
    import mtoa.ui.ae
    import mtoa.ui.ae.utils
    import mtoa.ui.ae.templates
    import mtoa.ui.ae.shadingEngineTemplate
    import mtoa.ui.ae.cryptomatteTemplate
    import mtoa.ui.ae.customShaderTemplates
    import mtoa.ui.ae.customShapeAttributes
    import mtoa.ui.ae.objectSetTemplate
    import mtoa.ui.ae.shaderTemplate
    import mtoa.ui.ae.lightTemplate
    import mtoa.ui.ae.operatorTemplate

    import mtoa.ui.ae.aiAbsTemplate
    import mtoa.ui.ae.aiAddTemplate
    import mtoa.ui.ae.aiAmbientOcclusionTemplate
    import mtoa.ui.ae.aiAOVDriverTemplate
    import mtoa.ui.ae.aiAOVFilterTemplate
    import mtoa.ui.ae.aiAOVTemplate
    import mtoa.ui.ae.aiAreaLightTemplate
    import mtoa.ui.ae.aiAtanTemplate
    import mtoa.ui.ae.aiAtmosphereVolumeTemplate
    import mtoa.ui.ae.aiAxfShaderTemplate
    import mtoa.ui.ae.aiBarndoorTemplate
    import mtoa.ui.ae.aiBlackbodyTemplate
    import mtoa.ui.ae.aiBump2dTemplate
    import mtoa.ui.ae.aiBump3dTemplate
    import mtoa.ui.ae.aiCacheTemplate
    import mtoa.ui.ae.aiCameraProjectionTemplate
    import mtoa.ui.ae.aiCarPaintTemplate
    import mtoa.ui.ae.aiCellNoiseTemplate
    import mtoa.ui.ae.aiCheckerboardTemplate
    import mtoa.ui.ae.aiClampTemplate
    import mtoa.ui.ae.aiClipGeoTemplate
    import mtoa.ui.ae.aiCollectionTemplate
    import mtoa.ui.ae.aiColorConvertTemplate
    import mtoa.ui.ae.aiColorCorrectTemplate
    import mtoa.ui.ae.aiColorJitterTemplate
    import mtoa.ui.ae.aiColorToFloatTemplate
    import mtoa.ui.ae.aiCompareTemplate
    import mtoa.ui.ae.aiComplementTemplate
    import mtoa.ui.ae.aiComplexIorTemplate
    import mtoa.ui.ae.aiCompositeTemplate
    import mtoa.ui.ae.aiCrossTemplate
    import mtoa.ui.ae.aiCurvatureTemplate
    import mtoa.ui.ae.aiCurveCollectorTemplate
    import mtoa.ui.ae.aiCustomIorTemplate
    import mtoa.ui.ae.aiDisableTemplate
    import mtoa.ui.ae.aiDivideTemplate
    import mtoa.ui.ae.aiDotTemplate
    import mtoa.ui.ae.aiExpTemplate
    import mtoa.ui.ae.aiFacingRatioTemplate
    import mtoa.ui.ae.aiFlakesTemplate
    import mtoa.ui.ae.aiFlatTemplate
    import mtoa.ui.ae.aiFloatToIntTemplate
    import mtoa.ui.ae.aiFloatToMatrixTemplate
    import mtoa.ui.ae.aiFloatToRgbaTemplate
    import mtoa.ui.ae.aiFogTemplate
    import mtoa.ui.ae.aiFractionTemplate
    import mtoa.ui.ae.aiGoboTemplate
    import mtoa.ui.ae.aiHairTemplate
    import mtoa.ui.ae.aiImagerColorCorrectTemplate
    import mtoa.ui.ae.aiImagerColorCurvesTemplate
    import mtoa.ui.ae.aiImagerDenoiserNoiceTemplate
    import mtoa.ui.ae.aiImagerDenoiserOidnTemplate
    import mtoa.ui.ae.aiImagerDenoiserOptixTemplate
    import mtoa.ui.ae.aiImagerExposureTemplate
    import mtoa.ui.ae.aiImagerLensEffectsTemplate
    import mtoa.ui.ae.aiImagerLightMixerTemplate
    import mtoa.ui.ae.aiImagersBaseTemplate
    import mtoa.ui.ae.aiImagerTonemapTemplate
    import mtoa.ui.ae.aiImagerWhiteBalanceTemplate
    import mtoa.ui.ae.aiImageTemplate
    import mtoa.ui.ae.aiIncludeGraphTemplate
    import mtoa.ui.ae.aiIsFiniteTemplate
    import mtoa.ui.ae.aiLambertTemplate
    import mtoa.ui.ae.aiLayerFloatTemplate
    import mtoa.ui.ae.aiLayerRgbaTemplate
    import mtoa.ui.ae.aiLayerShaderTemplate
    import mtoa.ui.ae.aiLengthTemplate
    import mtoa.ui.ae.aiLightBlockerTemplate
    import mtoa.ui.ae.aiLightDecayTemplate
    import mtoa.ui.ae.aiLogTemplate
    import mtoa.ui.ae.aiLookSwitchTemplate
    import mtoa.ui.ae.aiMaterialXShaderTemplate
    import mtoa.ui.ae.aiMaterialxTemplate
    import mtoa.ui.ae.aiMatteTemplate
    import mtoa.ui.ae.aiMaxTemplate
    import mtoa.ui.ae.aiMergeTemplate
    import mtoa.ui.ae.aiMeshLightTemplate
    import mtoa.ui.ae.aiMinTemplate
    import mtoa.ui.ae.aiMixShaderTemplate
    import mtoa.ui.ae.aiModuloTemplate
    import mtoa.ui.ae.aiMotionVectorTemplate
    import mtoa.ui.ae.aiMultiplyTemplate
    import mtoa.ui.ae.aiNegateTemplate
    import mtoa.ui.ae.aiNoiseTemplate
    import mtoa.ui.ae.aiNormalizeTemplate
    import mtoa.ui.ae.aiNormalMapTemplate
    import mtoa.ui.ae.aiOslShaderTemplate
    import mtoa.ui.ae.aiPassthroughTemplate
    import mtoa.ui.ae.aiPhotometricLightTemplate
    import mtoa.ui.ae.aiPhysicalSkyTemplate
    import mtoa.ui.ae.aiPowTemplate
    import mtoa.ui.ae.aiRampFloatTemplate
    import mtoa.ui.ae.aiRampRgbTemplate
    import mtoa.ui.ae.aiRandomTemplate
    import mtoa.ui.ae.aiRangeTemplate
    import mtoa.ui.ae.aiRaySwitchTemplate
    import mtoa.ui.ae.aiReciprocalTemplate
    import mtoa.ui.ae.aiRgbToVectorTemplate
    import mtoa.ui.ae.aiRoundCornersTemplate
    import mtoa.ui.ae.aiSetParameterTemplate
    import mtoa.ui.ae.aiSetTransformTemplate
    import mtoa.ui.ae.aiShadowMatteTemplate
    import mtoa.ui.ae.aiShuffleTemplate
    import mtoa.ui.ae.aiSignTemplate
    import mtoa.ui.ae.aiSkinTemplate
    import mtoa.ui.ae.aiSkyDomeLightTemplate
    import mtoa.ui.ae.aiSkyTemplate
    import mtoa.ui.ae.aiSpaceTransformTemplate
    import mtoa.ui.ae.aiSqrtTemplate
    import mtoa.ui.ae.aiStandardHairTemplate
    import mtoa.ui.ae.aiStandardSurfaceTemplate
    import mtoa.ui.ae.aiStandardTemplate
    import mtoa.ui.ae.aiStandardVolumeTemplate
    import mtoa.ui.ae.aiStandInTemplate
    import mtoa.ui.ae.aiStateFloatTemplate
    import mtoa.ui.ae.aiStateIntTemplate
    import mtoa.ui.ae.aiStateVectorTemplate
    import mtoa.ui.ae.aiStringReplaceTemplate
    import mtoa.ui.ae.aiSubstractTemplate
    import mtoa.ui.ae.aiSwatchDisplay
    import mtoa.ui.ae.aiSwitchOperatorTemplate
    import mtoa.ui.ae.aiSwitchTemplate
    import mtoa.ui.ae.aiThinFilmTemplate
    import mtoa.ui.ae.aiToonTemplate
    import mtoa.ui.ae.aiTraceSetTemplate
    import mtoa.ui.ae.aiTrigoTemplate
    import mtoa.ui.ae.aiTriplanarTemplate
    import mtoa.ui.ae.aiTwoSidedTemplate
    import mtoa.ui.ae.aiUserDataBoolTemplate
    import mtoa.ui.ae.aiUserDataColorTemplate
    import mtoa.ui.ae.aiUserDataFloatTemplate
    import mtoa.ui.ae.aiUserDataIntTemplate
    import mtoa.ui.ae.aiUserDataStringTemplate
    import mtoa.ui.ae.aiUserDataVec2Template
    import mtoa.ui.ae.aiUserDataVectorTemplate
    import mtoa.ui.ae.aiUtilityTemplate
    import mtoa.ui.ae.aiUvProjectionTemplate
    import mtoa.ui.ae.aiUvTransformTemplate
    import mtoa.ui.ae.aiVectorMapTemplate
    import mtoa.ui.ae.aiVectorToRgbTemplate
    import mtoa.ui.ae.aiVolumeCollectorTemplate
    import mtoa.ui.ae.aiVolumeSampleFloatTemplate
    import mtoa.ui.ae.aiVolumeSampleRgbTemplate
    import mtoa.ui.ae.aiVolumeTemplate
    import mtoa.ui.ae.aiWireframeTemplate
    import mtoa.ui.ae.aiWriteColorTemplate
    import mtoa.ui.ae.aiWriteFloatTemplate
    import mtoa.ui.ae.aiWriteIntTemplate
    import mtoa.ui.ae.aiWriteVectorTemplate

    import mtoa.ui.globals
    import mtoa.ui.globals.settings
    import mtoa.ui.globals.common

    import mtoa.ui.nodeTreeLister
    import mtoa.ui.procview
    import mtoa.ui.procview.SetParameterWidgets
    import mtoa.ui.procview.CustomProceduralTransverser
    import mtoa.ui.procview.UsdTransverser
    import mtoa.ui.procview.AlembicTransverser
    import mtoa.ui.procview.StandInTransverser
    import mtoa.ui.procview.ProceduralTransverser
    import mtoa.ui.procview.ProceduralTreeView
    import mtoa.ui.procview.ProceduralWidgets

    # Qt Tests
    import mtoa.ui.qt

    import mtoa.ui.aoveditor
    import mtoa.ui.exportalembic
    import mtoa.ui.exportlook
    import mtoa.ui.exportass
    import mtoa.ui.imagers
    import mtoa.ui.overrides
    import mtoa.ui.arnoldmenu


    # TX Manager
    import mtoa.txManager
    import mtoa.txManager.ui
    import mtoa.txManager.lib

except ImportError as E:
    resultColor = failColor
    print("!"*80)
    print("There was an error importing some modules from mtoa, see traceback below ...")
    traceback.print_exc()
    print("!"*80)
else:
    print("="*80)
    print("mtoa python modules successfuly imported")
    print("="*80)

post_script = 'import sys\nimport os\ntest_root_path = sys.argv[1]\noiiotool_path  = sys.argv[2]\n'
post_script += 'cmd = oiiotool_path\n'
post_script += 'cmd += " --create 160x120 3 -d uint8 --fill:color={} 160x120 -o "\n'.format(resultColor)
post_script += 'cmd += os.path.join(test_root_path, "testrender.tif")\n'
post_script += 'print(cmd)\n'
post_script += '\nos.system(cmd)'

print('Writing post_script')
with open('post_script.py','w') as f:
    f.write(post_script)

maya.standalone.uninitialize()

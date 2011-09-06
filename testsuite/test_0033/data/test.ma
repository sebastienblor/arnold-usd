//Maya ASCII 2011 scene
//Name: test.ma
//Last modified: Fri, Sep 02, 2011 04:32:21 PM
//Codeset: 1252
requires maya "2011";
requires "mtoa" "0.10.0";
currentUnit -linear centimeter -angle degree -time film;
fileInfo "application" "maya";
fileInfo "product" "Maya 2011";
fileInfo "version" "2011 x64";
fileInfo "cutIdentifier" "201009060330-781623";
fileInfo "osv" "Microsoft Windows 7 Business Edition, 64-bit Windows 7  (Build 7600)\n";
createNode transform -shared -name "persp";
	setAttr ".visibility" no;
	setAttr ".translate" -type "double3" 1.4235782869176601 1.0063204384796565 1.4004041871751336 ;
	setAttr ".rotate" -type "double3" -29.738352729602607 44.600000000000257 0 ;
createNode camera -shared -name "perspShape" -parent "persp";
	addAttr -cachedInternally true -keyable true -shortName "ai_filtermap" -longName "aiFiltermap" 
		-attributeType "message";
	addAttr -cachedInternally true -keyable true -shortName "ai_horizontal_fov" -longName "aiHorizontalFov" 
		-defaultValue 60 -minValue 0 -maxValue 1000000000 -softMaxValue 360 -attributeType "float";
	addAttr -cachedInternally true -keyable true -shortName "ai_vertical_fov" -longName "aiVerticalFov" 
		-defaultValue 90 -minValue 0 -maxValue 1000000000 -softMaxValue 180 -attributeType "float";
	addAttr -cachedInternally true -keyable true -shortName "ai_projective" -longName "aiProjective" 
		-defaultValue 1 -minValue 0 -maxValue 1 -attributeType "bool";
	addAttr -cachedInternally true -keyable true -shortName "ai_focus_distance" -longName "aiFocusDistance" 
		-defaultValue 1 -minValue 0 -maxValue 1000000000 -softMinValue 0 -softMaxValue 200 
		-attributeType "float";
	addAttr -cachedInternally true -keyable true -shortName "ai_aperture_size" -longName "aiApertureSize" 
		-minValue 0 -maxValue 20 -softMinValue 0 -softMaxValue 1 -attributeType "float";
	addAttr -cachedInternally true -keyable true -shortName "ai_aperture_blades" -longName "aiApertureBlades" 
		-minValue 3 -maxValue 40 -attributeType "long";
	addAttr -cachedInternally true -keyable true -shortName "ai_aperture_blade_curvature" 
		-longName "aiApertureBladeCurvature" -minValue -20 -maxValue 20 -softMinValue -5 
		-softMaxValue 5 -attributeType "float";
	addAttr -cachedInternally true -keyable true -shortName "ai_aperture_rotation" -longName "aiApertureRotation" 
		-minValue 0 -maxValue 360 -softMinValue 0 -softMaxValue 50 -attributeType "float";
	addAttr -cachedInternally true -keyable true -shortName "ai_edof" -longName "aiEnableDOF" 
		-minValue 0 -maxValue 1 -attributeType "bool";
	addAttr -cachedInternally true -keyable true -shortName "ai_fov" -longName "aiFov" 
		-defaultValue 90 -minValue 0 -maxValue 1000000000 -softMaxValue 360 -attributeType "float";
	addAttr -cachedInternally true -keyable true -shortName "ai_autocrop" -longName "aiAutocrop" 
		-minValue 0 -maxValue 1 -attributeType "bool";
	addAttr -cachedInternally true -usedAsColor -keyable true -shortName "ai_uv_remap" 
		-longName "aiUvRemap" -attributeType "float3" -numberOfChildren 3;
	addAttr -cachedInternally true -keyable true -shortName "ai_uv_remapr" -longName "aiUvRemapR" 
		-attributeType "float" -parent "aiUvRemap";
	addAttr -cachedInternally true -keyable true -shortName "ai_uv_remapg" -longName "aiUvRemapG" 
		-attributeType "float" -parent "aiUvRemap";
	addAttr -cachedInternally true -keyable true -shortName "ai_uv_remapb" -longName "aiUvRemapB" 
		-attributeType "float" -parent "aiUvRemap";
	addAttr -cachedInternally true -hidden true -keyable true -shortName "ai_uv_remapa" 
		-longName "aiUvRemapA" -attributeType "float";
	addAttr -cachedInternally true -keyable true -shortName "ai_translator" -longName "aiTranslator" 
		-dataType "string";
	setAttr -keyable off ".visibility" no;
	setAttr ".overscan" 1.3;
	setAttr ".focalLength" 34.999999999999986;
	setAttr ".centerOfInterest" 2.2357839209826662;
	setAttr ".imageName" -type "string" "persp";
	setAttr ".depthName" -type "string" "persp_depth";
	setAttr ".maskName" -type "string" "persp_mask";
	setAttr ".homeCommand" -type "string" "viewSet -p %camera";
	setAttr ".displayResolution" yes;
	setAttr -keyable on ".aiTranslator" -type "string" "perspective";
createNode transform -shared -name "top";
	setAttr ".visibility" no;
	setAttr ".translate" -type "double3" 0 1000.1 0 ;
	setAttr ".rotate" -type "double3" -89.999999999999986 0 0 ;
createNode camera -shared -name "topShape" -parent "top";
	addAttr -cachedInternally true -keyable true -shortName "ai_filtermap" -longName "aiFiltermap" 
		-attributeType "message";
	addAttr -cachedInternally true -keyable true -shortName "ai_horizontal_fov" -longName "aiHorizontalFov" 
		-defaultValue 60 -minValue 0 -maxValue 1000000000 -softMaxValue 360 -attributeType "float";
	addAttr -cachedInternally true -keyable true -shortName "ai_vertical_fov" -longName "aiVerticalFov" 
		-defaultValue 90 -minValue 0 -maxValue 1000000000 -softMaxValue 180 -attributeType "float";
	addAttr -cachedInternally true -keyable true -shortName "ai_projective" -longName "aiProjective" 
		-defaultValue 1 -minValue 0 -maxValue 1 -attributeType "bool";
	addAttr -cachedInternally true -keyable true -shortName "ai_focus_distance" -longName "aiFocusDistance" 
		-defaultValue 1 -minValue 0 -maxValue 1000000000 -softMinValue 0 -softMaxValue 200 
		-attributeType "float";
	addAttr -cachedInternally true -keyable true -shortName "ai_aperture_size" -longName "aiApertureSize" 
		-minValue 0 -maxValue 20 -softMinValue 0 -softMaxValue 1 -attributeType "float";
	addAttr -cachedInternally true -keyable true -shortName "ai_aperture_blades" -longName "aiApertureBlades" 
		-minValue 3 -maxValue 40 -attributeType "long";
	addAttr -cachedInternally true -keyable true -shortName "ai_aperture_blade_curvature" 
		-longName "aiApertureBladeCurvature" -minValue -20 -maxValue 20 -softMinValue -5 
		-softMaxValue 5 -attributeType "float";
	addAttr -cachedInternally true -keyable true -shortName "ai_aperture_rotation" -longName "aiApertureRotation" 
		-minValue 0 -maxValue 360 -softMinValue 0 -softMaxValue 50 -attributeType "float";
	addAttr -cachedInternally true -keyable true -shortName "ai_edof" -longName "aiEnableDOF" 
		-minValue 0 -maxValue 1 -attributeType "bool";
	addAttr -cachedInternally true -keyable true -shortName "ai_fov" -longName "aiFov" 
		-defaultValue 90 -minValue 0 -maxValue 1000000000 -softMaxValue 360 -attributeType "float";
	addAttr -cachedInternally true -keyable true -shortName "ai_autocrop" -longName "aiAutocrop" 
		-minValue 0 -maxValue 1 -attributeType "bool";
	addAttr -cachedInternally true -usedAsColor -keyable true -shortName "ai_uv_remap" 
		-longName "aiUvRemap" -attributeType "float3" -numberOfChildren 3;
	addAttr -cachedInternally true -keyable true -shortName "ai_uv_remapr" -longName "aiUvRemapR" 
		-attributeType "float" -parent "aiUvRemap";
	addAttr -cachedInternally true -keyable true -shortName "ai_uv_remapg" -longName "aiUvRemapG" 
		-attributeType "float" -parent "aiUvRemap";
	addAttr -cachedInternally true -keyable true -shortName "ai_uv_remapb" -longName "aiUvRemapB" 
		-attributeType "float" -parent "aiUvRemap";
	addAttr -cachedInternally true -hidden true -keyable true -shortName "ai_uv_remapa" 
		-longName "aiUvRemapA" -attributeType "float";
	addAttr -cachedInternally true -keyable true -shortName "ai_translator" -longName "aiTranslator" 
		-dataType "string";
	setAttr -keyable off ".visibility" no;
	setAttr ".renderable" no;
	setAttr ".centerOfInterest" 1000.1;
	setAttr ".orthographicWidth" 30;
	setAttr ".imageName" -type "string" "top";
	setAttr ".depthName" -type "string" "top_depth";
	setAttr ".maskName" -type "string" "top_mask";
	setAttr ".homeCommand" -type "string" "viewSet -t %camera";
	setAttr ".orthographic" yes;
	setAttr -keyable on ".aiTranslator" -type "string" "orthographic";
createNode transform -shared -name "front";
	setAttr ".visibility" no;
	setAttr ".translate" -type "double3" 0 0 1000.1 ;
createNode camera -shared -name "frontShape" -parent "front";
	addAttr -cachedInternally true -keyable true -shortName "ai_filtermap" -longName "aiFiltermap" 
		-attributeType "message";
	addAttr -cachedInternally true -keyable true -shortName "ai_horizontal_fov" -longName "aiHorizontalFov" 
		-defaultValue 60 -minValue 0 -maxValue 1000000000 -softMaxValue 360 -attributeType "float";
	addAttr -cachedInternally true -keyable true -shortName "ai_vertical_fov" -longName "aiVerticalFov" 
		-defaultValue 90 -minValue 0 -maxValue 1000000000 -softMaxValue 180 -attributeType "float";
	addAttr -cachedInternally true -keyable true -shortName "ai_projective" -longName "aiProjective" 
		-defaultValue 1 -minValue 0 -maxValue 1 -attributeType "bool";
	addAttr -cachedInternally true -keyable true -shortName "ai_focus_distance" -longName "aiFocusDistance" 
		-defaultValue 1 -minValue 0 -maxValue 1000000000 -softMinValue 0 -softMaxValue 200 
		-attributeType "float";
	addAttr -cachedInternally true -keyable true -shortName "ai_aperture_size" -longName "aiApertureSize" 
		-minValue 0 -maxValue 20 -softMinValue 0 -softMaxValue 1 -attributeType "float";
	addAttr -cachedInternally true -keyable true -shortName "ai_aperture_blades" -longName "aiApertureBlades" 
		-minValue 3 -maxValue 40 -attributeType "long";
	addAttr -cachedInternally true -keyable true -shortName "ai_aperture_blade_curvature" 
		-longName "aiApertureBladeCurvature" -minValue -20 -maxValue 20 -softMinValue -5 
		-softMaxValue 5 -attributeType "float";
	addAttr -cachedInternally true -keyable true -shortName "ai_aperture_rotation" -longName "aiApertureRotation" 
		-minValue 0 -maxValue 360 -softMinValue 0 -softMaxValue 50 -attributeType "float";
	addAttr -cachedInternally true -keyable true -shortName "ai_edof" -longName "aiEnableDOF" 
		-minValue 0 -maxValue 1 -attributeType "bool";
	addAttr -cachedInternally true -keyable true -shortName "ai_fov" -longName "aiFov" 
		-defaultValue 90 -minValue 0 -maxValue 1000000000 -softMaxValue 360 -attributeType "float";
	addAttr -cachedInternally true -keyable true -shortName "ai_autocrop" -longName "aiAutocrop" 
		-minValue 0 -maxValue 1 -attributeType "bool";
	addAttr -cachedInternally true -usedAsColor -keyable true -shortName "ai_uv_remap" 
		-longName "aiUvRemap" -attributeType "float3" -numberOfChildren 3;
	addAttr -cachedInternally true -keyable true -shortName "ai_uv_remapr" -longName "aiUvRemapR" 
		-attributeType "float" -parent "aiUvRemap";
	addAttr -cachedInternally true -keyable true -shortName "ai_uv_remapg" -longName "aiUvRemapG" 
		-attributeType "float" -parent "aiUvRemap";
	addAttr -cachedInternally true -keyable true -shortName "ai_uv_remapb" -longName "aiUvRemapB" 
		-attributeType "float" -parent "aiUvRemap";
	addAttr -cachedInternally true -hidden true -keyable true -shortName "ai_uv_remapa" 
		-longName "aiUvRemapA" -attributeType "float";
	addAttr -cachedInternally true -keyable true -shortName "ai_translator" -longName "aiTranslator" 
		-dataType "string";
	setAttr -keyable off ".visibility" no;
	setAttr ".renderable" no;
	setAttr ".centerOfInterest" 1000.1;
	setAttr ".orthographicWidth" 30;
	setAttr ".imageName" -type "string" "front";
	setAttr ".depthName" -type "string" "front_depth";
	setAttr ".maskName" -type "string" "front_mask";
	setAttr ".homeCommand" -type "string" "viewSet -f %camera";
	setAttr ".orthographic" yes;
	setAttr -keyable on ".aiTranslator" -type "string" "orthographic";
createNode transform -shared -name "side";
	setAttr ".visibility" no;
	setAttr ".translate" -type "double3" 1000.1 0 0 ;
	setAttr ".rotate" -type "double3" 0 89.999999999999986 0 ;
createNode camera -shared -name "sideShape" -parent "side";
	addAttr -cachedInternally true -keyable true -shortName "ai_filtermap" -longName "aiFiltermap" 
		-attributeType "message";
	addAttr -cachedInternally true -keyable true -shortName "ai_horizontal_fov" -longName "aiHorizontalFov" 
		-defaultValue 60 -minValue 0 -maxValue 1000000000 -softMaxValue 360 -attributeType "float";
	addAttr -cachedInternally true -keyable true -shortName "ai_vertical_fov" -longName "aiVerticalFov" 
		-defaultValue 90 -minValue 0 -maxValue 1000000000 -softMaxValue 180 -attributeType "float";
	addAttr -cachedInternally true -keyable true -shortName "ai_projective" -longName "aiProjective" 
		-defaultValue 1 -minValue 0 -maxValue 1 -attributeType "bool";
	addAttr -cachedInternally true -keyable true -shortName "ai_focus_distance" -longName "aiFocusDistance" 
		-defaultValue 1 -minValue 0 -maxValue 1000000000 -softMinValue 0 -softMaxValue 200 
		-attributeType "float";
	addAttr -cachedInternally true -keyable true -shortName "ai_aperture_size" -longName "aiApertureSize" 
		-minValue 0 -maxValue 20 -softMinValue 0 -softMaxValue 1 -attributeType "float";
	addAttr -cachedInternally true -keyable true -shortName "ai_aperture_blades" -longName "aiApertureBlades" 
		-minValue 3 -maxValue 40 -attributeType "long";
	addAttr -cachedInternally true -keyable true -shortName "ai_aperture_blade_curvature" 
		-longName "aiApertureBladeCurvature" -minValue -20 -maxValue 20 -softMinValue -5 
		-softMaxValue 5 -attributeType "float";
	addAttr -cachedInternally true -keyable true -shortName "ai_aperture_rotation" -longName "aiApertureRotation" 
		-minValue 0 -maxValue 360 -softMinValue 0 -softMaxValue 50 -attributeType "float";
	addAttr -cachedInternally true -keyable true -shortName "ai_edof" -longName "aiEnableDOF" 
		-minValue 0 -maxValue 1 -attributeType "bool";
	addAttr -cachedInternally true -keyable true -shortName "ai_fov" -longName "aiFov" 
		-defaultValue 90 -minValue 0 -maxValue 1000000000 -softMaxValue 360 -attributeType "float";
	addAttr -cachedInternally true -keyable true -shortName "ai_autocrop" -longName "aiAutocrop" 
		-minValue 0 -maxValue 1 -attributeType "bool";
	addAttr -cachedInternally true -usedAsColor -keyable true -shortName "ai_uv_remap" 
		-longName "aiUvRemap" -attributeType "float3" -numberOfChildren 3;
	addAttr -cachedInternally true -keyable true -shortName "ai_uv_remapr" -longName "aiUvRemapR" 
		-attributeType "float" -parent "aiUvRemap";
	addAttr -cachedInternally true -keyable true -shortName "ai_uv_remapg" -longName "aiUvRemapG" 
		-attributeType "float" -parent "aiUvRemap";
	addAttr -cachedInternally true -keyable true -shortName "ai_uv_remapb" -longName "aiUvRemapB" 
		-attributeType "float" -parent "aiUvRemap";
	addAttr -cachedInternally true -hidden true -keyable true -shortName "ai_uv_remapa" 
		-longName "aiUvRemapA" -attributeType "float";
	addAttr -cachedInternally true -keyable true -shortName "ai_translator" -longName "aiTranslator" 
		-dataType "string";
	setAttr -keyable off ".visibility" no;
	setAttr ".renderable" no;
	setAttr ".centerOfInterest" 1000.1;
	setAttr ".orthographicWidth" 30;
	setAttr ".imageName" -type "string" "side";
	setAttr ".depthName" -type "string" "side_depth";
	setAttr ".maskName" -type "string" "side_mask";
	setAttr ".homeCommand" -type "string" "viewSet -s %camera";
	setAttr ".orthographic" yes;
	setAttr -keyable on ".aiTranslator" -type "string" "orthographic";
createNode transform -name "pCube1";
createNode mesh -name "pCubeShape1" -parent "pCube1";
	addAttr -cachedInternally true -shortName "ai_sss_sample_distribution" -longName "aiSssSampleDistribution" 
		-minValue 0 -maxValue 3 -enumName "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-attributeType "enum";
	addAttr -cachedInternally true -keyable true -shortName "ai_sss_sample_spacing" 
		-longName "aiSssSampleSpacing" -defaultValue 0.10000000149011612 -minValue 0 -softMaxValue 
		1 -attributeType "float";
	addAttr -cachedInternally true -keyable true -shortName "ai_self_shadows" -longName "aiSelfShadows" 
		-defaultValue 1 -minValue 0 -maxValue 1 -attributeType "bool";
	addAttr -cachedInternally true -keyable true -shortName "ai_opaque" -longName "aiOpaque" 
		-defaultValue 1 -minValue 0 -maxValue 1 -attributeType "bool";
	addAttr -cachedInternally true -keyable true -shortName "ai_vid" -longName "aiVisibleInDiffuse" 
		-defaultValue 1 -minValue 0 -maxValue 1 -attributeType "bool";
	addAttr -cachedInternally true -keyable true -shortName "ai_vig" -longName "aiVisibleInGlossy" 
		-defaultValue 1 -minValue 0 -maxValue 1 -attributeType "bool";
	addAttr -cachedInternally true -shortName "ai_subdiv_type" -longName "aiSubdivType" 
		-minValue 0 -maxValue 2 -enumName "none:catclark:linear" -attributeType "enum";
	addAttr -cachedInternally true -keyable true -shortName "ai_subdiv_iterations" -longName "aiSubdivIterations" 
		-defaultValue 1 -minValue 0 -maxValue 100 -softMinValue 0 -softMaxValue 10 -attributeType "long";
	addAttr -cachedInternally true -shortName "ai_subdiv_adaptive_metric" -longName "aiSubdivAdaptiveMetric" 
		-minValue 0 -maxValue 2 -enumName "auto:edge_length:flatness" -attributeType "enum";
	addAttr -cachedInternally true -keyable true -shortName "ai_subdiv_pixel_error" 
		-longName "aiSubdivPixelError" -minValue 0 -softMaxValue 10 -attributeType "float";
	addAttr -cachedInternally true -keyable true -shortName "ai_subdiv_dicing_camera" 
		-longName "aiSubdivDicingCamera" -attributeType "message";
	addAttr -cachedInternally true -shortName "ai_subdiv_uv_smoothing" -longName "aiSubdivUvSmoothing" 
		-minValue 0 -maxValue 3 -enumName "pin_corners:pin_borders:linear:smooth" -attributeType "enum";
	addAttr -cachedInternally true -keyable true -shortName "ai_exptan" -longName "aiExportTangents" 
		-minValue 0 -maxValue 1 -attributeType "bool";
	addAttr -cachedInternally true -keyable true -shortName "ai_expcol" -longName "aiExportColors" 
		-minValue 0 -maxValue 1 -attributeType "bool";
	addAttr -cachedInternally true -shortName "mso" -longName "miShadingSamplesOverride" 
		-minValue 0 -maxValue 1 -attributeType "bool";
	addAttr -cachedInternally true -shortName "msh" -longName "miShadingSamples" -minValue 
		0 -softMaxValue 8 -attributeType "float";
	addAttr -cachedInternally true -shortName "mdo" -longName "miMaxDisplaceOverride" 
		-minValue 0 -maxValue 1 -attributeType "bool";
	addAttr -cachedInternally true -shortName "mmd" -longName "miMaxDisplace" -minValue 
		0 -softMaxValue 1 -attributeType "float";
	setAttr -keyable off ".visibility";
	setAttr ".visibleInReflections" yes;
	setAttr ".visibleInRefractions" yes;
	setAttr ".uvSet[0].uvSetName" -type "string" "map1";
	setAttr ".currentUVSet" -type "string" "map1";
	setAttr ".displayColorChannel" -type "string" "Ambient+Diffuse";
	setAttr ".collisionOffsetVelocityMultiplier[0]"  0 1 1;
	setAttr ".collisionDepthVelocityMultiplier[0]"  0 1 1;
createNode place3dTexture -name "place3dTexture1";
createNode aiOptions -shared -name "defaultArnoldRenderOptions";
	addAttr -cachedInternally true -shortName "driver_exr_compression" -longName "driverExrCompression" 
		-defaultValue 2 -minValue 0 -maxValue 4 -enumName "none:rle:zip:piz:pxr24" -attributeType "enum";
	addAttr -cachedInternally true -keyable true -shortName "driver_half_precision" 
		-longName "driverHalfPrecision" -minValue 0 -maxValue 1 -attributeType "bool";
	addAttr -cachedInternally true -keyable true -shortName "driver_tiled" -longName "driverTiled" 
		-defaultValue 1 -minValue 0 -maxValue 1 -attributeType "bool";
	addAttr -cachedInternally true -keyable true -shortName "driver_preserve_layer_name" 
		-longName "driverPreserveLayerName" -minValue 0 -maxValue 1 -attributeType "bool";
	addAttr -cachedInternally true -keyable true -shortName "driver_quality" -longName "driverQuality" 
		-defaultValue 100 -minValue 0 -maxValue 100 -attributeType "long";
	addAttr -cachedInternally true -keyable true -shortName "driver_output_padded" -longName "driverOutputPadded" 
		-minValue 0 -maxValue 1 -attributeType "bool";
	addAttr -cachedInternally true -keyable true -shortName "driver_gamma" -longName "driverGamma" 
		-defaultValue 2.2000000476837158 -minValue 9.9999997473787516e-005 -softMaxValue 
		5 -attributeType "float";
	addAttr -cachedInternally true -keyable true -shortName "driver_dither_amplitude" 
		-longName "driverDitherAmplitude" -defaultValue 1 -attributeType "float";
	addAttr -cachedInternally true -shortName "driver_png_format" -longName "driverPngFormat" 
		-minValue 0 -maxValue 1 -enumName "int8:int16" -attributeType "enum";
	addAttr -cachedInternally true -shortName "driver_tiff_compression" -longName "driverTiffCompression" 
		-minValue 0 -maxValue 4 -enumName "none:lzw:ccittrle:zip:packbits" -attributeType "enum";
	addAttr -cachedInternally true -shortName "driver_tiff_format" -longName "driverTiffFormat" 
		-minValue 0 -maxValue 3 -enumName "int8:int16:int32:float32" -attributeType "enum";
	addAttr -cachedInternally true -keyable true -shortName "driver_unpremult_alpha" 
		-longName "driverUnpremultAlpha" -minValue 0 -maxValue 1 -attributeType "bool";
	addAttr -cachedInternally true -keyable true -shortName "filter_width" -longName "filterWidth" 
		-defaultValue 3 -attributeType "float";
	addAttr -cachedInternally true -shortName "filter_domain" -longName "filterDomain" 
		-minValue 0 -maxValue 1 -enumName "first_hit:all_hits" -attributeType "enum";
	addAttr -cachedInternally true -keyable true -shortName "filter_minimum" -longName "filterMinimum" 
		-attributeType "float";
	addAttr -cachedInternally true -keyable true -shortName "filter_maximum" -longName "filterMaximum" 
		-defaultValue 1 -attributeType "float";
	addAttr -cachedInternally true -keyable true -shortName "filter_scalar_mode" -longName "filterScalarMode" 
		-minValue 0 -maxValue 1 -attributeType "bool";
	setAttr ".imageFormat" -type "string" "tif";
	setAttr ".filterType" -type "string" "gaussian";
	setAttr -keyable on ".driverTiled" no;
createNode lightLinker -shared -name "lightLinker1";
	setAttr -size 3 ".link";
	setAttr -size 3 ".shadowLink";
createNode displayLayerManager -name "layerManager";
createNode displayLayer -name "defaultLayer";
createNode renderLayerManager -name "renderLayerManager";
createNode renderLayer -name "defaultRenderLayer";
	setAttr ".global" yes;
createNode polyCube -name "polyCube1";
	setAttr ".createUVs" 4;
createNode deleteComponent -name "deleteComponent1";
	setAttr ".deleteComponents" -type "componentList" 2 "f[0:1]" "f[4]";
createNode projection -name "projection1";
	addAttr -cachedInternally true -shortName "resolution" -longName "resolution" -defaultValue 
		32 -attributeType "long";
	setAttr ".uAngle" 180.00000500895632;
	setAttr ".vAngle" 90.000002504478161;
	setAttr ".projType" 6;
	setAttr ".vertexUvOne" -type "float2" 0.5 0.5 ;
	setAttr ".vertexUvTwo" -type "float2" 0.5 0.5 ;
	setAttr ".vertexUvThree" -type "float2" 0.5 0.5 ;
	setAttr ".resolution" 256;
createNode file -name "file1";
	setAttr ".fileTextureName" -type "string" "grid_opacity.tif";
createNode place2dTexture -name "place2dTexture1";
createNode surfaceShader -name "surfaceShader1";
createNode shadingEngine -name "surfaceShader1SG";
	setAttr ".isHistoricallyInteresting" 0;
	setAttr ".renderableOnlySet" yes;
createNode materialInfo -name "materialInfo1";
createNode script -name "sceneConfigurationScriptNode";
	setAttr ".before" -type "string" "playbackOptions -min 1 -max 24 -ast 1 -aet 48 ";
	setAttr ".scriptType" 6;
select -noExpand :time1;
	setAttr ".outTime" 1;
	setAttr ".unwarpedTime" 1;
select -noExpand :renderPartition;
	setAttr -size 3 ".sets";
select -noExpand :initialShadingGroup;
	setAttr ".renderableOnlySet" yes;
select -noExpand :initialParticleSE;
	setAttr ".renderableOnlySet" yes;
select -noExpand :defaultShaderList1;
	setAttr -size 3 ".shaders";
select -noExpand :defaultTextureList1;
select -noExpand :postProcessList1;
	setAttr -size 2 ".postProcesses";
select -noExpand :defaultRenderUtilityList1;
	setAttr -size 2 ".utilities";
select -noExpand :renderGlobalsList1;
select -noExpand :defaultRenderGlobals;
	setAttr ".currentRenderer" -type "string" "arnold";
	setAttr ".imageFormat" 51;
	setAttr ".imfPluginKey" -type "string" "tif";
	setAttr ".imageFilePrefix" -type "string" "testrender";
select -noExpand :defaultResolution;
	setAttr ".width" 160;
	setAttr ".height" 120;
	setAttr ".pixelAspect" 1;
	setAttr ".deviceAspectRatio" 1.3333333730697632;
select -noExpand :hardwareRenderGlobals;
	setAttr ".colorTextureResolution" 256;
	setAttr ".bumpTextureResolution" 512;
select -noExpand :defaultHardwareRenderGlobals;
	setAttr ".filename" -type "string" "im";
	setAttr ".resolution" -type "string" "ntsc_4d 646 485 1.333";
connectAttr "deleteComponent1.outputGeometry" "pCubeShape1.inMesh";
relationship "link" ":lightLinker1" ":initialShadingGroup.message" ":defaultLightSet.message";
relationship "link" ":lightLinker1" ":initialParticleSE.message" ":defaultLightSet.message";
relationship "link" ":lightLinker1" "surfaceShader1SG.message" ":defaultLightSet.message";
relationship "shadowLink" ":lightLinker1" ":initialShadingGroup.message" ":defaultLightSet.message";
relationship "shadowLink" ":lightLinker1" ":initialParticleSE.message" ":defaultLightSet.message";
relationship "shadowLink" ":lightLinker1" "surfaceShader1SG.message" ":defaultLightSet.message";
connectAttr "layerManager.displayLayerId[0]" "defaultLayer.identification";
connectAttr "renderLayerManager.renderLayerId[0]" "defaultRenderLayer.identification"
		;
connectAttr "polyCube1.output" "deleteComponent1.inputGeometry";
connectAttr "file1.outColor" "projection1.image";
connectAttr "place3dTexture1.worldInverseMatrix" "projection1.placementMatrix";
connectAttr "place2dTexture1.coverage" "file1.coverage";
connectAttr "place2dTexture1.translateFrame" "file1.translateFrame";
connectAttr "place2dTexture1.rotateFrame" "file1.rotateFrame";
connectAttr "place2dTexture1.mirrorU" "file1.mirrorU";
connectAttr "place2dTexture1.mirrorV" "file1.mirrorV";
connectAttr "place2dTexture1.stagger" "file1.stagger";
connectAttr "place2dTexture1.wrapU" "file1.wrapU";
connectAttr "place2dTexture1.wrapV" "file1.wrapV";
connectAttr "place2dTexture1.repeatUV" "file1.repeatUV";
connectAttr "place2dTexture1.offset" "file1.offset";
connectAttr "place2dTexture1.rotateUV" "file1.rotateUV";
connectAttr "place2dTexture1.noiseUV" "file1.noiseUV";
connectAttr "place2dTexture1.vertexUvOne" "file1.vertexUvOne";
connectAttr "place2dTexture1.vertexUvTwo" "file1.vertexUvTwo";
connectAttr "place2dTexture1.vertexUvThree" "file1.vertexUvThree";
connectAttr "place2dTexture1.vertexCameraOne" "file1.vertexCameraOne";
connectAttr "place2dTexture1.outUV" "file1.uvCoord";
connectAttr "place2dTexture1.outUvFilterSize" "file1.uvFilterSize";
connectAttr "projection1.outColor" "surfaceShader1.outColor";
connectAttr "surfaceShader1.outColor" "surfaceShader1SG.surfaceShader";
connectAttr "pCubeShape1.instObjGroups" "surfaceShader1SG.dagSetMembers" -nextAvailable
		;
connectAttr "surfaceShader1SG.message" "materialInfo1.shadingGroup";
connectAttr "surfaceShader1.message" "materialInfo1.material";
connectAttr "projection1.message" "materialInfo1.texture" -nextAvailable;
connectAttr "projection1.outColor" "materialInfo1.textureChannel";
connectAttr "surfaceShader1SG.partition" ":renderPartition.sets" -nextAvailable;
connectAttr "surfaceShader1.message" ":defaultShaderList1.shaders" -nextAvailable
		;
connectAttr "file1.message" ":defaultTextureList1.textures" -nextAvailable;
connectAttr "projection1.message" ":defaultRenderUtilityList1.utilities" -nextAvailable
		;
connectAttr "place2dTexture1.message" ":defaultRenderUtilityList1.utilities" -nextAvailable
		;
// End of test.ma

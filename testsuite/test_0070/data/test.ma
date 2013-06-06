//Maya ASCII 2011 scene
//Name: test.ma
//Last modified: Thu, Apr 18, 2013 06:53:32 PM
//Codeset: UTF-8
requires maya "2011";
requires "mtoa" "0.23.0.dev";
requires "stereoCamera" "10.0";
currentUnit -l centimeter -a degree -t film;
fileInfo "application" "maya";
fileInfo "product" "Maya 2011";
fileInfo "version" "2011 x64";
fileInfo "cutIdentifier" "201009060248-781623";
fileInfo "osv" "Linux 2.6.40.3-0.fc15.x86_64 #1 SMP Tue Aug 16 04:10:59 UTC 2011 x86_64";
createNode transform -s -n "persp";
	setAttr ".v" no;
	setAttr ".t" -type "double3" 13.511425177544998 36.14081972808065 7.0588861436796373 ;
	setAttr ".r" -type "double3" -91.53835272960464 0.2000000000000258 -1.2424117416441811e-17 ;
createNode camera -s -n "perspShape" -p "persp";
	addAttr -ci true -k true -sn "ai_user_options" -ln "aiUserOptions" -dt "string";
	addAttr -ci true -sn "ai_exposure" -ln "aiExposure" -min -100 -max 100 -smn -5 -smx 
		5 -at "float";
	addAttr -ci true -k true -sn "ai_filtermap" -ln "aiFiltermap" -at "message";
	addAttr -ci true -sn "ai_horizontal_fov" -ln "aiHorizontalFov" -dv 60 -min 0 -max 
		1000000000 -smx 360 -at "float";
	addAttr -ci true -sn "ai_vertical_fov" -ln "aiVerticalFov" -dv 90 -min 0 -max 1000000000 
		-smx 180 -at "float";
	addAttr -ci true -sn "ai_projective" -ln "aiProjective" -dv 1 -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "ai_focus_distance" -ln "aiFocusDistance" -dv 1 -min 0 -max 
		1000000000 -smn 0 -smx 200 -at "float";
	addAttr -ci true -sn "ai_aperture_size" -ln "aiApertureSize" -min 0 -max 20 -smn 
		0 -smx 1 -at "float";
	addAttr -ci true -sn "ai_aperture_blades" -ln "aiApertureBlades" -min 3 -max 40 
		-at "long";
	addAttr -ci true -sn "ai_aperture_blade_curvature" -ln "aiApertureBladeCurvature" 
		-min -20 -max 20 -smn 0 -smx 1 -at "float";
	addAttr -ci true -sn "ai_aperture_rotation" -ln "aiApertureRotation" -min 0 -max 
		360 -smn 0 -smx 50 -at "float";
	addAttr -ci true -k true -sn "ai_edof" -ln "aiEnableDOF" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "ai_fov" -ln "aiFov" -dv 90 -min 0 -max 1000000000 -smx 360 
		-at "float";
	addAttr -ci true -sn "ai_autocrop" -ln "aiAutocrop" -min 0 -max 1 -at "bool";
	addAttr -ci true -h true -sn "ai_uv_remapa" -ln "aiUvRemapA" -min 0 -max 1 -at "float";
	addAttr -ci true -uac -sn "ai_uv_remap" -ln "aiUvRemap" -at "float3" -nc 3;
	addAttr -ci true -sn "ai_uv_remapr" -ln "aiUvRemapR" -at "float" -p "aiUvRemap";
	addAttr -ci true -sn "ai_uv_remapg" -ln "aiUvRemapG" -at "float" -p "aiUvRemap";
	addAttr -ci true -sn "ai_uv_remapb" -ln "aiUvRemapB" -at "float" -p "aiUvRemap";
	addAttr -ci true -k true -sn "ai_translator" -ln "aiTranslator" -dt "string";
	setAttr -k off ".v" no;
	setAttr ".fl" 34.999999999999986;
	setAttr ".coi" 36.587757769529887;
	setAttr ".imn" -type "string" "persp";
	setAttr ".den" -type "string" "persp_depth";
	setAttr ".man" -type "string" "persp_mask";
	setAttr ".tp" -type "double3" 8.5728802034224074 0 2.3337462221316123 ;
	setAttr ".hc" -type "string" "viewSet -p %camera";
	setAttr -k on ".ai_translator" -type "string" "perspective";
createNode transform -s -n "top";
	setAttr ".v" no;
	setAttr ".t" -type "double3" 0 100.1 0 ;
	setAttr ".r" -type "double3" -89.999999999999986 0 0 ;
createNode camera -s -n "topShape" -p "top";
	addAttr -ci true -k true -sn "ai_user_options" -ln "aiUserOptions" -dt "string";
	addAttr -ci true -sn "ai_exposure" -ln "aiExposure" -min -100 -max 100 -smn -5 -smx 
		5 -at "float";
	addAttr -ci true -k true -sn "ai_filtermap" -ln "aiFiltermap" -at "message";
	addAttr -ci true -sn "ai_horizontal_fov" -ln "aiHorizontalFov" -dv 60 -min 0 -max 
		1000000000 -smx 360 -at "float";
	addAttr -ci true -sn "ai_vertical_fov" -ln "aiVerticalFov" -dv 90 -min 0 -max 1000000000 
		-smx 180 -at "float";
	addAttr -ci true -sn "ai_projective" -ln "aiProjective" -dv 1 -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "ai_focus_distance" -ln "aiFocusDistance" -dv 1 -min 0 -max 
		1000000000 -smn 0 -smx 200 -at "float";
	addAttr -ci true -sn "ai_aperture_size" -ln "aiApertureSize" -min 0 -max 20 -smn 
		0 -smx 1 -at "float";
	addAttr -ci true -sn "ai_aperture_blades" -ln "aiApertureBlades" -min 3 -max 40 
		-at "long";
	addAttr -ci true -sn "ai_aperture_blade_curvature" -ln "aiApertureBladeCurvature" 
		-min -20 -max 20 -smn 0 -smx 1 -at "float";
	addAttr -ci true -sn "ai_aperture_rotation" -ln "aiApertureRotation" -min 0 -max 
		360 -smn 0 -smx 50 -at "float";
	addAttr -ci true -k true -sn "ai_edof" -ln "aiEnableDOF" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "ai_fov" -ln "aiFov" -dv 90 -min 0 -max 1000000000 -smx 360 
		-at "float";
	addAttr -ci true -sn "ai_autocrop" -ln "aiAutocrop" -min 0 -max 1 -at "bool";
	addAttr -ci true -h true -sn "ai_uv_remapa" -ln "aiUvRemapA" -min 0 -max 1 -at "float";
	addAttr -ci true -uac -sn "ai_uv_remap" -ln "aiUvRemap" -at "float3" -nc 3;
	addAttr -ci true -sn "ai_uv_remapr" -ln "aiUvRemapR" -at "float" -p "aiUvRemap";
	addAttr -ci true -sn "ai_uv_remapg" -ln "aiUvRemapG" -at "float" -p "aiUvRemap";
	addAttr -ci true -sn "ai_uv_remapb" -ln "aiUvRemapB" -at "float" -p "aiUvRemap";
	addAttr -ci true -k true -sn "ai_translator" -ln "aiTranslator" -dt "string";
	setAttr -k off ".v" no;
	setAttr ".rnd" no;
	setAttr ".coi" 100.1;
	setAttr ".ow" 30;
	setAttr ".imn" -type "string" "top";
	setAttr ".den" -type "string" "top_depth";
	setAttr ".man" -type "string" "top_mask";
	setAttr ".hc" -type "string" "viewSet -t %camera";
	setAttr ".o" yes;
	setAttr -k on ".ai_translator" -type "string" "orthographic";
createNode transform -s -n "front";
	setAttr ".v" no;
	setAttr ".t" -type "double3" 0 0 100.1 ;
createNode camera -s -n "frontShape" -p "front";
	addAttr -ci true -k true -sn "ai_user_options" -ln "aiUserOptions" -dt "string";
	addAttr -ci true -sn "ai_exposure" -ln "aiExposure" -min -100 -max 100 -smn -5 -smx 
		5 -at "float";
	addAttr -ci true -k true -sn "ai_filtermap" -ln "aiFiltermap" -at "message";
	addAttr -ci true -sn "ai_horizontal_fov" -ln "aiHorizontalFov" -dv 60 -min 0 -max 
		1000000000 -smx 360 -at "float";
	addAttr -ci true -sn "ai_vertical_fov" -ln "aiVerticalFov" -dv 90 -min 0 -max 1000000000 
		-smx 180 -at "float";
	addAttr -ci true -sn "ai_projective" -ln "aiProjective" -dv 1 -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "ai_focus_distance" -ln "aiFocusDistance" -dv 1 -min 0 -max 
		1000000000 -smn 0 -smx 200 -at "float";
	addAttr -ci true -sn "ai_aperture_size" -ln "aiApertureSize" -min 0 -max 20 -smn 
		0 -smx 1 -at "float";
	addAttr -ci true -sn "ai_aperture_blades" -ln "aiApertureBlades" -min 3 -max 40 
		-at "long";
	addAttr -ci true -sn "ai_aperture_blade_curvature" -ln "aiApertureBladeCurvature" 
		-min -20 -max 20 -smn 0 -smx 1 -at "float";
	addAttr -ci true -sn "ai_aperture_rotation" -ln "aiApertureRotation" -min 0 -max 
		360 -smn 0 -smx 50 -at "float";
	addAttr -ci true -k true -sn "ai_edof" -ln "aiEnableDOF" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "ai_fov" -ln "aiFov" -dv 90 -min 0 -max 1000000000 -smx 360 
		-at "float";
	addAttr -ci true -sn "ai_autocrop" -ln "aiAutocrop" -min 0 -max 1 -at "bool";
	addAttr -ci true -h true -sn "ai_uv_remapa" -ln "aiUvRemapA" -min 0 -max 1 -at "float";
	addAttr -ci true -uac -sn "ai_uv_remap" -ln "aiUvRemap" -at "float3" -nc 3;
	addAttr -ci true -sn "ai_uv_remapr" -ln "aiUvRemapR" -at "float" -p "aiUvRemap";
	addAttr -ci true -sn "ai_uv_remapg" -ln "aiUvRemapG" -at "float" -p "aiUvRemap";
	addAttr -ci true -sn "ai_uv_remapb" -ln "aiUvRemapB" -at "float" -p "aiUvRemap";
	addAttr -ci true -k true -sn "ai_translator" -ln "aiTranslator" -dt "string";
	setAttr -k off ".v" no;
	setAttr ".rnd" no;
	setAttr ".coi" 100.1;
	setAttr ".ow" 30;
	setAttr ".imn" -type "string" "front";
	setAttr ".den" -type "string" "front_depth";
	setAttr ".man" -type "string" "front_mask";
	setAttr ".hc" -type "string" "viewSet -f %camera";
	setAttr ".o" yes;
	setAttr -k on ".ai_translator" -type "string" "orthographic";
createNode transform -s -n "side";
	setAttr ".v" no;
	setAttr ".t" -type "double3" 100.1 0 0 ;
	setAttr ".r" -type "double3" 0 89.999999999999986 0 ;
createNode camera -s -n "sideShape" -p "side";
	addAttr -ci true -k true -sn "ai_user_options" -ln "aiUserOptions" -dt "string";
	addAttr -ci true -sn "ai_exposure" -ln "aiExposure" -min -100 -max 100 -smn -5 -smx 
		5 -at "float";
	addAttr -ci true -k true -sn "ai_filtermap" -ln "aiFiltermap" -at "message";
	addAttr -ci true -sn "ai_horizontal_fov" -ln "aiHorizontalFov" -dv 60 -min 0 -max 
		1000000000 -smx 360 -at "float";
	addAttr -ci true -sn "ai_vertical_fov" -ln "aiVerticalFov" -dv 90 -min 0 -max 1000000000 
		-smx 180 -at "float";
	addAttr -ci true -sn "ai_projective" -ln "aiProjective" -dv 1 -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "ai_focus_distance" -ln "aiFocusDistance" -dv 1 -min 0 -max 
		1000000000 -smn 0 -smx 200 -at "float";
	addAttr -ci true -sn "ai_aperture_size" -ln "aiApertureSize" -min 0 -max 20 -smn 
		0 -smx 1 -at "float";
	addAttr -ci true -sn "ai_aperture_blades" -ln "aiApertureBlades" -min 3 -max 40 
		-at "long";
	addAttr -ci true -sn "ai_aperture_blade_curvature" -ln "aiApertureBladeCurvature" 
		-min -20 -max 20 -smn 0 -smx 1 -at "float";
	addAttr -ci true -sn "ai_aperture_rotation" -ln "aiApertureRotation" -min 0 -max 
		360 -smn 0 -smx 50 -at "float";
	addAttr -ci true -k true -sn "ai_edof" -ln "aiEnableDOF" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "ai_fov" -ln "aiFov" -dv 90 -min 0 -max 1000000000 -smx 360 
		-at "float";
	addAttr -ci true -sn "ai_autocrop" -ln "aiAutocrop" -min 0 -max 1 -at "bool";
	addAttr -ci true -h true -sn "ai_uv_remapa" -ln "aiUvRemapA" -min 0 -max 1 -at "float";
	addAttr -ci true -uac -sn "ai_uv_remap" -ln "aiUvRemap" -at "float3" -nc 3;
	addAttr -ci true -sn "ai_uv_remapr" -ln "aiUvRemapR" -at "float" -p "aiUvRemap";
	addAttr -ci true -sn "ai_uv_remapg" -ln "aiUvRemapG" -at "float" -p "aiUvRemap";
	addAttr -ci true -sn "ai_uv_remapb" -ln "aiUvRemapB" -at "float" -p "aiUvRemap";
	addAttr -ci true -k true -sn "ai_translator" -ln "aiTranslator" -dt "string";
	setAttr -k off ".v" no;
	setAttr ".rnd" no;
	setAttr ".coi" 100.1;
	setAttr ".ow" 30;
	setAttr ".imn" -type "string" "side";
	setAttr ".den" -type "string" "side_depth";
	setAttr ".man" -type "string" "side_mask";
	setAttr ".hc" -type "string" "viewSet -s %camera";
	setAttr ".o" yes;
	setAttr -k on ".ai_translator" -type "string" "orthographic";
createNode transform -n "pPlane1";
	setAttr ".t" -type "double3" 1.8991104903154916 0 2.3337462221316119 ;
createNode mesh -n "pPlaneShape1" -p "pPlane1";
	addAttr -ci true -k true -sn "ai_user_options" -ln "aiUserOptions" -dt "string";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-min 0 -smx 1 -at "float";
	addAttr -ci true -sn "ai_self_shadows" -ln "aiSelfShadows" -dv 1 -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "ai_opaque" -ln "aiOpaque" -dv 1 -min 0 -max 1 -at "bool";
	addAttr -ci true -k true -sn "trace_sets" -ln "aiTraceSets" -dt "string";
	addAttr -ci true -k true -sn "ai_sss_setname" -ln "aiSssSetname" -dt "string";
	addAttr -ci true -k true -sn "ai_vid" -ln "aiVisibleInDiffuse" -dv 1 -min 0 -max 
		1 -at "bool";
	addAttr -ci true -k true -sn "ai_vig" -ln "aiVisibleInGlossy" -dv 1 -min 0 -max 
		1 -at "bool";
	addAttr -ci true -sn "ai_subdiv_type" -ln "aiSubdivType" -min 0 -max 2 -en "none:catclark:linear" 
		-at "enum";
	addAttr -ci true -sn "ai_subdiv_iterations" -ln "aiSubdivIterations" -dv 1 -min 
		0 -max 100 -smn 0 -smx 10 -at "long";
	addAttr -ci true -sn "ai_subdiv_adaptive_metric" -ln "aiSubdivAdaptiveMetric" -min 
		0 -max 2 -en "auto:edge_length:flatness" -at "enum";
	addAttr -ci true -sn "ai_subdiv_pixel_error" -ln "aiSubdivPixelError" -min 0 -smx 
		10 -at "float";
	addAttr -ci true -k true -sn "ai_subdiv_dicing_camera" -ln "aiSubdivDicingCamera" 
		-at "message";
	addAttr -ci true -sn "ai_subdiv_uv_smoothing" -ln "aiSubdivUvSmoothing" -min 0 -max 
		3 -en "pin_corners:pin_borders:linear:smooth" -at "enum";
	addAttr -ci true -sn "ai_subdiv_smooth_derivs" -ln "aiSubdivSmoothDerivs" -min 0 
		-max 1 -at "bool";
	addAttr -ci true -sn "ai_disp_height" -ln "aiDispHeight" -dv 1 -at "float";
	addAttr -ci true -sn "ai_disp_padding" -ln "aiDispPadding" -at "float";
	addAttr -ci true -sn "ai_disp_zero_value" -ln "aiDispZeroValue" -at "float";
	addAttr -ci true -sn "ai_disp_autobump" -ln "aiDispAutobump" -min 0 -max 1 -at "bool";
	addAttr -ci true -k true -sn "ai_exptan" -ln "aiExportTangents" -min 0 -max 1 -at "bool";
	addAttr -ci true -k true -sn "ai_expcol" -ln "aiExportColors" -min 0 -max 1 -at "bool";
	addAttr -ci true -k true -sn "ai_exprpt" -ln "aiExportRefPoints" -dv 1 -min 0 -max 
		1 -at "bool";
	addAttr -ci true -k true -sn "ai_exprnrm" -ln "aiExportRefNormals" -min 0 -max 1 
		-at "bool";
	addAttr -ci true -k true -sn "ai_exprtan" -ln "aiExportRefTangents" -min 0 -max 
		1 -at "bool";
	addAttr -ci true -k true -sn "ai_volume_container_mode" -ln "aiVolumeContainerMode" 
		-min 0 -max 1 -en "Mesh:Bounding Box" -at "enum";
	addAttr -ci true -k true -sn "ai_step_size" -ln "aiStepSize" -min 0 -smx 1 -at "float";
	addAttr -ci true -sn "mso" -ln "miShadingSamplesOverride" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "msh" -ln "miShadingSamples" -min 0 -smx 8 -at "float";
	addAttr -ci true -sn "mdo" -ln "miMaxDisplaceOverride" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "mmd" -ln "miMaxDisplace" -min 0 -smx 1 -at "float";
	setAttr -k off ".v";
	setAttr ".vir" yes;
	setAttr ".vif" yes;
	setAttr ".pv" -type "double2" 3.3944030600191279 0.51535434068418007 ;
	setAttr ".uvst[0].uvsn" -type "string" "map1";
	setAttr ".cuvs" -type "string" "map1";
	setAttr ".dcc" -type "string" "Ambient+Diffuse";
	setAttr ".covm[0]"  0 1 1;
	setAttr ".cdvm[0]"  0 1 1;
	setAttr -s 9 ".pt[0:8]" -type "float3"  0 -2.3841858e-07 0 0 -2.3841858e-07 0 0 
		-2.3841858e-07 0 0 2.646978e-23 6.6174449e-24 0 2.646978e-23 6.6174449e-24 0 2.646978e-23 
		6.6174449e-24 0 2.3841858e-07 0 0 2.3841858e-07 0 0 2.3841858e-07 0;
createNode transform -n "directionalLight1";
	setAttr ".t" -type "double3" 0 5.1628475931277249 3.1280933277221914 ;
	setAttr ".r" -type "double3" -38.968848577268517 0 0 ;
createNode directionalLight -n "directionalLightShape1" -p "directionalLight1";
	addAttr -ci true -k true -sn "ai_user_options" -ln "aiUserOptions" -dt "string";
	addAttr -ci true -sn "ai_cast_shadows" -ln "aiCastShadows" -dv 1 -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "ai_shadow_density" -ln "aiShadowDensity" -dv 1 -min 0 -max 
		1 -smn 0 -smx 1 -at "float";
	addAttr -ci true -sn "ai_exposure" -ln "aiExposure" -smn 0 -smx 10 -at "float";
	addAttr -ci true -sn "ai_samples" -ln "aiSamples" -dv 1 -min 1 -max 100 -smx 10 
		-at "long";
	addAttr -ci true -sn "ai_normalize" -ln "aiNormalize" -dv 1 -min 0 -max 1 -at "bool";
	addAttr -ci true -k true -m -sn "ai_filters" -ln "aiFilters" -at "message";
	addAttr -ci true -sn "ai_diffuse" -ln "aiDiffuse" -dv 1 -smn 0 -smx 1 -at "float";
	addAttr -ci true -sn "ai_specular" -ln "aiSpecular" -dv 1 -smn 0 -smx 1 -at "float";
	addAttr -ci true -sn "ai_sss" -ln "aiSss" -dv 1 -smn 0 -smx 1 -at "float";
	addAttr -ci true -sn "ai_indirect" -ln "aiIndirect" -dv 1 -smn 0 -smx 1 -at "float";
	addAttr -ci true -sn "ai_max_bounces" -ln "aiMaxBounces" -dv 999 -at "long";
	addAttr -ci true -sn "ai_volume_samples" -ln "aiVolumeSamples" -dv 2 -min 0 -max 
		100 -smx 10 -at "long";
	addAttr -ci true -k true -sn "ai_use_color_temperature" -ln "aiUseColorTemperature" 
		-min 0 -max 1 -at "bool";
	addAttr -ci true -k true -sn "ai_color_temperature" -ln "aiColorTemperature" -dv 
		6500 -min 0 -smn 1000 -smx 15000 -at "float";
	addAttr -ci true -sn "ai_angle" -ln "aiAngle" -min 0 -smx 10 -at "float";
	addAttr -ci true -sn "ai_affect_volumetrics" -ln "aiAffectVolumetrics" -dv 1 -min 
		0 -max 1 -at "bool";
	addAttr -ci true -sn "ai_cast_volumetric_shadows" -ln "aiCastVolumetricShadows" 
		-dv 1 -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "ai_bounce_factor" -ln "aiBounceFactor" -dv 1 -min 0 -smx 20 
		-at "float";
	addAttr -ci true -sn "ai_bounces" -ln "aiBounces" -dv 999 -min 0 -max 10000 -at "long";
	setAttr -k off ".v";
createNode transform -n "pPlane2";
	setAttr ".t" -type "double3" 13.680371051832623 0 2.3337462221316119 ;
createNode mesh -n "pPlaneShape2" -p "pPlane2";
	addAttr -ci true -k true -sn "ai_user_options" -ln "aiUserOptions" -dt "string";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-min 0 -smx 1 -at "float";
	addAttr -ci true -sn "ai_self_shadows" -ln "aiSelfShadows" -dv 1 -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "ai_opaque" -ln "aiOpaque" -dv 1 -min 0 -max 1 -at "bool";
	addAttr -ci true -k true -sn "trace_sets" -ln "aiTraceSets" -dt "string";
	addAttr -ci true -k true -sn "ai_sss_setname" -ln "aiSssSetname" -dt "string";
	addAttr -ci true -k true -sn "ai_vid" -ln "aiVisibleInDiffuse" -dv 1 -min 0 -max 
		1 -at "bool";
	addAttr -ci true -k true -sn "ai_vig" -ln "aiVisibleInGlossy" -dv 1 -min 0 -max 
		1 -at "bool";
	addAttr -ci true -sn "ai_subdiv_type" -ln "aiSubdivType" -min 0 -max 2 -en "none:catclark:linear" 
		-at "enum";
	addAttr -ci true -sn "ai_subdiv_iterations" -ln "aiSubdivIterations" -dv 1 -min 
		0 -max 100 -smn 0 -smx 10 -at "long";
	addAttr -ci true -sn "ai_subdiv_adaptive_metric" -ln "aiSubdivAdaptiveMetric" -min 
		0 -max 2 -en "auto:edge_length:flatness" -at "enum";
	addAttr -ci true -sn "ai_subdiv_pixel_error" -ln "aiSubdivPixelError" -min 0 -smx 
		10 -at "float";
	addAttr -ci true -k true -sn "ai_subdiv_dicing_camera" -ln "aiSubdivDicingCamera" 
		-at "message";
	addAttr -ci true -sn "ai_subdiv_uv_smoothing" -ln "aiSubdivUvSmoothing" -min 0 -max 
		3 -en "pin_corners:pin_borders:linear:smooth" -at "enum";
	addAttr -ci true -sn "ai_subdiv_smooth_derivs" -ln "aiSubdivSmoothDerivs" -min 0 
		-max 1 -at "bool";
	addAttr -ci true -sn "ai_disp_height" -ln "aiDispHeight" -dv 1 -at "float";
	addAttr -ci true -sn "ai_disp_padding" -ln "aiDispPadding" -at "float";
	addAttr -ci true -sn "ai_disp_zero_value" -ln "aiDispZeroValue" -at "float";
	addAttr -ci true -sn "ai_disp_autobump" -ln "aiDispAutobump" -min 0 -max 1 -at "bool";
	addAttr -ci true -k true -sn "ai_exptan" -ln "aiExportTangents" -min 0 -max 1 -at "bool";
	addAttr -ci true -k true -sn "ai_expcol" -ln "aiExportColors" -min 0 -max 1 -at "bool";
	addAttr -ci true -k true -sn "ai_exprpt" -ln "aiExportRefPoints" -dv 1 -min 0 -max 
		1 -at "bool";
	addAttr -ci true -k true -sn "ai_exprnrm" -ln "aiExportRefNormals" -min 0 -max 1 
		-at "bool";
	addAttr -ci true -k true -sn "ai_exprtan" -ln "aiExportRefTangents" -min 0 -max 
		1 -at "bool";
	addAttr -ci true -k true -sn "ai_volume_container_mode" -ln "aiVolumeContainerMode" 
		-min 0 -max 1 -en "Mesh:Bounding Box" -at "enum";
	addAttr -ci true -k true -sn "ai_step_size" -ln "aiStepSize" -min 0 -smx 1 -at "float";
	addAttr -ci true -sn "mso" -ln "miShadingSamplesOverride" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "msh" -ln "miShadingSamples" -min 0 -smx 8 -at "float";
	addAttr -ci true -sn "mdo" -ln "miMaxDisplaceOverride" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "mmd" -ln "miMaxDisplace" -min 0 -smx 1 -at "float";
	setAttr -k off ".v";
	setAttr ".vir" yes;
	setAttr ".vif" yes;
	setAttr ".pv" -type "double2" 1.0315449237823486 1.5064982054507841 ;
	setAttr ".uvst[0].uvsn" -type "string" "map1";
	setAttr -s 16 ".uvst[0].uvsp[0:15]" -type "float2" 1.2438611 1.3030871 1.7438611 
		1.3030871 0.81922877 1.2787113 0.29912809 0.29390275 1.2378938 0.30627072 0.81922877 
		1.7099094 0.29912809 0.72510093 1.2378938 0.73746872 1.7378938 0.73746872 0.79912812 
		0.72510093 0.31922877 1.7099094 1.7438611 1.734285 0.79912812 0.29390275 1.7378938 
		0.30627072 1.2438611 1.734285 0.31922877 1.2787113;
	setAttr ".cuvs" -type "string" "map1";
	setAttr ".dcc" -type "string" "Ambient+Diffuse";
	setAttr ".covm[0]"  0 1 1;
	setAttr ".cdvm[0]"  0 1 1;
	setAttr -s 9 ".pt[0:8]" -type "float3"  0 -2.3841858e-07 0 0 -2.3841858e-07 0 0 
		-2.3841858e-07 0 0 2.646978e-23 6.6174449e-24 0 2.646978e-23 6.6174449e-24 0 2.646978e-23 
		6.6174449e-24 0 2.3841858e-07 0 0 2.3841858e-07 0 0 2.3841858e-07 0;
	setAttr -s 9 ".vt[0:8]"  -5.781405 -2.3841858e-07 4.9858618 0 -2.3841858e-07 4.9858618 
		5.781405 -2.3841858e-07 4.9858618 -5.781405 2.646978e-23 6.6174449e-24 0 2.646978e-23 
		6.6174449e-24 5.781405 2.646978e-23 6.6174449e-24 -5.781405 2.3841858e-07 -4.9858618 
		0 2.3841858e-07 -4.9858618 5.781405 2.3841858e-07 -4.9858618;
	setAttr -s 12 ".ed[0:11]"  0 1 0 0 3 0 1 2 0 1 4 1 2 5 0 3 4 1 3 6 0 4 5 1 4 7 1 
		5 8 0 6 7 0 7 8 0;
	setAttr -s 4 ".fc[0:3]" -type "polyFaces" 
		f 4 0 3 -6 -2 
		mu 0 4 0 1 11 14 
		f 4 2 4 -8 -4 
		mu 0 4 15 2 5 10 
		f 4 5 8 -11 -7 
		mu 0 4 3 12 9 6 
		f 4 7 9 -12 -9 
		mu 0 4 4 13 8 7 ;
	setAttr ".cd" -type "dataPolyComponent" Index_Data Edge 0 ;
	setAttr ".cvd" -type "dataPolyComponent" Index_Data Vertex 0 ;
	setAttr ".hfd" -type "dataPolyComponent" Index_Data Face 0 ;
createNode transform -n "group";
	setAttr ".t" -type "double3" 0 0 10.530631381358173 ;
	setAttr ".rp" -type "double3" 1.8991104903154916 0 2.3337462221316123 ;
	setAttr ".sp" -type "double3" 1.8991104903154916 0 2.3337462221316123 ;
createNode transform -n "pPlane3";
	setAttr ".t" -type "double3" -1.2499089037731976 0 10.804117128822247 ;
	setAttr ".s" -type "double3" 1.1583769136445605 1.1583769136445605 1.1583769136445605 ;
createNode mesh -n "pPlaneShape3" -p "pPlane3";
	addAttr -ci true -k true -sn "ai_user_options" -ln "aiUserOptions" -dt "string";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-min 0 -smx 1 -at "float";
	addAttr -ci true -sn "ai_self_shadows" -ln "aiSelfShadows" -dv 1 -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "ai_opaque" -ln "aiOpaque" -dv 1 -min 0 -max 1 -at "bool";
	addAttr -ci true -k true -sn "trace_sets" -ln "aiTraceSets" -dt "string";
	addAttr -ci true -k true -sn "ai_sss_setname" -ln "aiSssSetname" -dt "string";
	addAttr -ci true -k true -sn "ai_vid" -ln "aiVisibleInDiffuse" -dv 1 -min 0 -max 
		1 -at "bool";
	addAttr -ci true -k true -sn "ai_vig" -ln "aiVisibleInGlossy" -dv 1 -min 0 -max 
		1 -at "bool";
	addAttr -ci true -sn "ai_subdiv_type" -ln "aiSubdivType" -min 0 -max 2 -en "none:catclark:linear" 
		-at "enum";
	addAttr -ci true -sn "ai_subdiv_iterations" -ln "aiSubdivIterations" -dv 1 -min 
		0 -max 100 -smn 0 -smx 10 -at "long";
	addAttr -ci true -sn "ai_subdiv_adaptive_metric" -ln "aiSubdivAdaptiveMetric" -min 
		0 -max 2 -en "auto:edge_length:flatness" -at "enum";
	addAttr -ci true -sn "ai_subdiv_pixel_error" -ln "aiSubdivPixelError" -min 0 -smx 
		10 -at "float";
	addAttr -ci true -k true -sn "ai_subdiv_dicing_camera" -ln "aiSubdivDicingCamera" 
		-at "message";
	addAttr -ci true -sn "ai_subdiv_uv_smoothing" -ln "aiSubdivUvSmoothing" -min 0 -max 
		3 -en "pin_corners:pin_borders:linear:smooth" -at "enum";
	addAttr -ci true -sn "ai_subdiv_smooth_derivs" -ln "aiSubdivSmoothDerivs" -min 0 
		-max 1 -at "bool";
	addAttr -ci true -sn "ai_disp_height" -ln "aiDispHeight" -dv 1 -at "float";
	addAttr -ci true -sn "ai_disp_padding" -ln "aiDispPadding" -at "float";
	addAttr -ci true -sn "ai_disp_zero_value" -ln "aiDispZeroValue" -at "float";
	addAttr -ci true -sn "ai_disp_autobump" -ln "aiDispAutobump" -min 0 -max 1 -at "bool";
	addAttr -ci true -k true -sn "ai_exptan" -ln "aiExportTangents" -min 0 -max 1 -at "bool";
	addAttr -ci true -k true -sn "ai_expcol" -ln "aiExportColors" -min 0 -max 1 -at "bool";
	addAttr -ci true -k true -sn "ai_exprpt" -ln "aiExportRefPoints" -dv 1 -min 0 -max 
		1 -at "bool";
	addAttr -ci true -k true -sn "ai_exprnrm" -ln "aiExportRefNormals" -min 0 -max 1 
		-at "bool";
	addAttr -ci true -k true -sn "ai_exprtan" -ln "aiExportRefTangents" -min 0 -max 
		1 -at "bool";
	addAttr -ci true -k true -sn "ai_volume_container_mode" -ln "aiVolumeContainerMode" 
		-min 0 -max 1 -en "Mesh:Bounding Box" -at "enum";
	addAttr -ci true -k true -sn "ai_step_size" -ln "aiStepSize" -min 0 -smx 1 -at "float";
	addAttr -ci true -sn "mtoa_constant_myString" -ln "mtoa_constant_myString" -dt "string";
	setAttr -k off ".v";
	setAttr ".vir" yes;
	setAttr ".vif" yes;
	setAttr ".uvst[0].uvsn" -type "string" "map1";
	setAttr ".cuvs" -type "string" "map1";
	setAttr ".dcc" -type "string" "Ambient+Diffuse";
	setAttr ".covm[0]"  0 1 1;
	setAttr ".cdvm[0]"  0 1 1;
	setAttr -k on ".mtoa_constant_myString" -type "string" "1001";
createNode transform -n "pPlane4";
	setAttr ".t" -type "double3" 4.8807076606846103 0 10.804117128822247 ;
	setAttr ".s" -type "double3" 1.1583769136445605 1.1583769136445605 1.1583769136445605 ;
createNode mesh -n "pPlaneShape4" -p "pPlane4";
	addAttr -ci true -k true -sn "ai_user_options" -ln "aiUserOptions" -dt "string";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-min 0 -smx 1 -at "float";
	addAttr -ci true -sn "ai_self_shadows" -ln "aiSelfShadows" -dv 1 -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "ai_opaque" -ln "aiOpaque" -dv 1 -min 0 -max 1 -at "bool";
	addAttr -ci true -k true -sn "trace_sets" -ln "aiTraceSets" -dt "string";
	addAttr -ci true -k true -sn "ai_sss_setname" -ln "aiSssSetname" -dt "string";
	addAttr -ci true -k true -sn "ai_vid" -ln "aiVisibleInDiffuse" -dv 1 -min 0 -max 
		1 -at "bool";
	addAttr -ci true -k true -sn "ai_vig" -ln "aiVisibleInGlossy" -dv 1 -min 0 -max 
		1 -at "bool";
	addAttr -ci true -sn "ai_subdiv_type" -ln "aiSubdivType" -min 0 -max 2 -en "none:catclark:linear" 
		-at "enum";
	addAttr -ci true -sn "ai_subdiv_iterations" -ln "aiSubdivIterations" -dv 1 -min 
		0 -max 100 -smn 0 -smx 10 -at "long";
	addAttr -ci true -sn "ai_subdiv_adaptive_metric" -ln "aiSubdivAdaptiveMetric" -min 
		0 -max 2 -en "auto:edge_length:flatness" -at "enum";
	addAttr -ci true -sn "ai_subdiv_pixel_error" -ln "aiSubdivPixelError" -min 0 -smx 
		10 -at "float";
	addAttr -ci true -k true -sn "ai_subdiv_dicing_camera" -ln "aiSubdivDicingCamera" 
		-at "message";
	addAttr -ci true -sn "ai_subdiv_uv_smoothing" -ln "aiSubdivUvSmoothing" -min 0 -max 
		3 -en "pin_corners:pin_borders:linear:smooth" -at "enum";
	addAttr -ci true -sn "ai_subdiv_smooth_derivs" -ln "aiSubdivSmoothDerivs" -min 0 
		-max 1 -at "bool";
	addAttr -ci true -sn "ai_disp_height" -ln "aiDispHeight" -dv 1 -at "float";
	addAttr -ci true -sn "ai_disp_padding" -ln "aiDispPadding" -at "float";
	addAttr -ci true -sn "ai_disp_zero_value" -ln "aiDispZeroValue" -at "float";
	addAttr -ci true -sn "ai_disp_autobump" -ln "aiDispAutobump" -min 0 -max 1 -at "bool";
	addAttr -ci true -k true -sn "ai_exptan" -ln "aiExportTangents" -min 0 -max 1 -at "bool";
	addAttr -ci true -k true -sn "ai_expcol" -ln "aiExportColors" -min 0 -max 1 -at "bool";
	addAttr -ci true -k true -sn "ai_exprpt" -ln "aiExportRefPoints" -dv 1 -min 0 -max 
		1 -at "bool";
	addAttr -ci true -k true -sn "ai_exprnrm" -ln "aiExportRefNormals" -min 0 -max 1 
		-at "bool";
	addAttr -ci true -k true -sn "ai_exprtan" -ln "aiExportRefTangents" -min 0 -max 
		1 -at "bool";
	addAttr -ci true -k true -sn "ai_volume_container_mode" -ln "aiVolumeContainerMode" 
		-min 0 -max 1 -en "Mesh:Bounding Box" -at "enum";
	addAttr -ci true -k true -sn "ai_step_size" -ln "aiStepSize" -min 0 -smx 1 -at "float";
	addAttr -ci true -sn "mtoa_constant_myString" -ln "mtoa_constant_myString" -dt "string";
	setAttr -k off ".v";
	setAttr ".vir" yes;
	setAttr ".vif" yes;
	setAttr ".uvst[0].uvsn" -type "string" "map1";
	setAttr -s 4 ".uvst[0].uvsp[0:3]" -type "float2" 0 0 1 0 0 0.96074474 1 0.96074474;
	setAttr ".cuvs" -type "string" "map1";
	setAttr ".dcc" -type "string" "Ambient+Diffuse";
	setAttr ".covm[0]"  0 1 1;
	setAttr ".cdvm[0]"  0 1 1;
	setAttr -s 4 ".vt[0:3]"  -2.4955454 -5.3237016e-16 2.3975821 2.4955454 -5.3237016e-16 
		2.3975821 -2.4955454 5.3237016e-16 -2.3975821 2.4955454 5.3237016e-16 -2.3975821;
	setAttr -s 4 ".ed[0:3]"  0 1 0 0 2 0 1 3 0 2 3 0;
	setAttr ".fc[0]" -type "polyFaces" 
		f 4 0 2 -4 -2 
		mu 0 4 0 1 3 2 ;
	setAttr ".cd" -type "dataPolyComponent" Index_Data Edge 0 ;
	setAttr ".cvd" -type "dataPolyComponent" Index_Data Vertex 0 ;
	setAttr ".hfd" -type "dataPolyComponent" Index_Data Face 0 ;
	setAttr -k on ".mtoa_constant_myString" -type "string" "1002";
createNode transform -n "pPlane5";
	setAttr ".t" -type "double3" -1.2726613752561029 0 16.628541820402706 ;
	setAttr ".s" -type "double3" 1.1583769136445605 1.1583769136445605 1.1583769136445605 ;
createNode mesh -n "pPlaneShape5" -p "pPlane5";
	addAttr -ci true -k true -sn "ai_user_options" -ln "aiUserOptions" -dt "string";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-min 0 -smx 1 -at "float";
	addAttr -ci true -sn "ai_self_shadows" -ln "aiSelfShadows" -dv 1 -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "ai_opaque" -ln "aiOpaque" -dv 1 -min 0 -max 1 -at "bool";
	addAttr -ci true -k true -sn "trace_sets" -ln "aiTraceSets" -dt "string";
	addAttr -ci true -k true -sn "ai_sss_setname" -ln "aiSssSetname" -dt "string";
	addAttr -ci true -k true -sn "ai_vid" -ln "aiVisibleInDiffuse" -dv 1 -min 0 -max 
		1 -at "bool";
	addAttr -ci true -k true -sn "ai_vig" -ln "aiVisibleInGlossy" -dv 1 -min 0 -max 
		1 -at "bool";
	addAttr -ci true -sn "ai_subdiv_type" -ln "aiSubdivType" -min 0 -max 2 -en "none:catclark:linear" 
		-at "enum";
	addAttr -ci true -sn "ai_subdiv_iterations" -ln "aiSubdivIterations" -dv 1 -min 
		0 -max 100 -smn 0 -smx 10 -at "long";
	addAttr -ci true -sn "ai_subdiv_adaptive_metric" -ln "aiSubdivAdaptiveMetric" -min 
		0 -max 2 -en "auto:edge_length:flatness" -at "enum";
	addAttr -ci true -sn "ai_subdiv_pixel_error" -ln "aiSubdivPixelError" -min 0 -smx 
		10 -at "float";
	addAttr -ci true -k true -sn "ai_subdiv_dicing_camera" -ln "aiSubdivDicingCamera" 
		-at "message";
	addAttr -ci true -sn "ai_subdiv_uv_smoothing" -ln "aiSubdivUvSmoothing" -min 0 -max 
		3 -en "pin_corners:pin_borders:linear:smooth" -at "enum";
	addAttr -ci true -sn "ai_subdiv_smooth_derivs" -ln "aiSubdivSmoothDerivs" -min 0 
		-max 1 -at "bool";
	addAttr -ci true -sn "ai_disp_height" -ln "aiDispHeight" -dv 1 -at "float";
	addAttr -ci true -sn "ai_disp_padding" -ln "aiDispPadding" -at "float";
	addAttr -ci true -sn "ai_disp_zero_value" -ln "aiDispZeroValue" -at "float";
	addAttr -ci true -sn "ai_disp_autobump" -ln "aiDispAutobump" -min 0 -max 1 -at "bool";
	addAttr -ci true -k true -sn "ai_exptan" -ln "aiExportTangents" -min 0 -max 1 -at "bool";
	addAttr -ci true -k true -sn "ai_expcol" -ln "aiExportColors" -min 0 -max 1 -at "bool";
	addAttr -ci true -k true -sn "ai_exprpt" -ln "aiExportRefPoints" -dv 1 -min 0 -max 
		1 -at "bool";
	addAttr -ci true -k true -sn "ai_exprnrm" -ln "aiExportRefNormals" -min 0 -max 1 
		-at "bool";
	addAttr -ci true -k true -sn "ai_exprtan" -ln "aiExportRefTangents" -min 0 -max 
		1 -at "bool";
	addAttr -ci true -k true -sn "ai_volume_container_mode" -ln "aiVolumeContainerMode" 
		-min 0 -max 1 -en "Mesh:Bounding Box" -at "enum";
	addAttr -ci true -k true -sn "ai_step_size" -ln "aiStepSize" -min 0 -smx 1 -at "float";
	addAttr -ci true -sn "mtoa_constant_myString" -ln "mtoa_constant_myString" -dt "string";
	setAttr -k off ".v";
	setAttr ".vir" yes;
	setAttr ".vif" yes;
	setAttr ".uvst[0].uvsn" -type "string" "map1";
	setAttr -s 4 ".uvst[0].uvsp[0:3]" -type "float2" 0 0 1 0 0 0.96074474 1 0.96074474;
	setAttr ".cuvs" -type "string" "map1";
	setAttr ".dcc" -type "string" "Ambient+Diffuse";
	setAttr ".covm[0]"  0 1 1;
	setAttr ".cdvm[0]"  0 1 1;
	setAttr -s 4 ".vt[0:3]"  -2.4955454 -5.3237016e-16 2.3975821 2.4955454 -5.3237016e-16 
		2.3975821 -2.4955454 5.3237016e-16 -2.3975821 2.4955454 5.3237016e-16 -2.3975821;
	setAttr -s 4 ".ed[0:3]"  0 1 0 0 2 0 1 3 0 2 3 0;
	setAttr ".fc[0]" -type "polyFaces" 
		f 4 0 2 -4 -2 
		mu 0 4 0 1 3 2 ;
	setAttr ".cd" -type "dataPolyComponent" Index_Data Edge 0 ;
	setAttr ".cvd" -type "dataPolyComponent" Index_Data Vertex 0 ;
	setAttr ".hfd" -type "dataPolyComponent" Index_Data Face 0 ;
	setAttr -k on ".mtoa_constant_myString" -type "string" "1003";
createNode transform -n "pPlane6";
	setAttr ".t" -type "double3" 4.9255348223961999 0 16.675014370506702 ;
	setAttr ".s" -type "double3" 1.1583769136445605 1.1583769136445605 1.1583769136445605 ;
createNode mesh -n "pPlaneShape6" -p "pPlane6";
	addAttr -ci true -k true -sn "ai_user_options" -ln "aiUserOptions" -dt "string";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-min 0 -smx 1 -at "float";
	addAttr -ci true -sn "ai_self_shadows" -ln "aiSelfShadows" -dv 1 -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "ai_opaque" -ln "aiOpaque" -dv 1 -min 0 -max 1 -at "bool";
	addAttr -ci true -k true -sn "trace_sets" -ln "aiTraceSets" -dt "string";
	addAttr -ci true -k true -sn "ai_sss_setname" -ln "aiSssSetname" -dt "string";
	addAttr -ci true -k true -sn "ai_vid" -ln "aiVisibleInDiffuse" -dv 1 -min 0 -max 
		1 -at "bool";
	addAttr -ci true -k true -sn "ai_vig" -ln "aiVisibleInGlossy" -dv 1 -min 0 -max 
		1 -at "bool";
	addAttr -ci true -sn "ai_subdiv_type" -ln "aiSubdivType" -min 0 -max 2 -en "none:catclark:linear" 
		-at "enum";
	addAttr -ci true -sn "ai_subdiv_iterations" -ln "aiSubdivIterations" -dv 1 -min 
		0 -max 100 -smn 0 -smx 10 -at "long";
	addAttr -ci true -sn "ai_subdiv_adaptive_metric" -ln "aiSubdivAdaptiveMetric" -min 
		0 -max 2 -en "auto:edge_length:flatness" -at "enum";
	addAttr -ci true -sn "ai_subdiv_pixel_error" -ln "aiSubdivPixelError" -min 0 -smx 
		10 -at "float";
	addAttr -ci true -k true -sn "ai_subdiv_dicing_camera" -ln "aiSubdivDicingCamera" 
		-at "message";
	addAttr -ci true -sn "ai_subdiv_uv_smoothing" -ln "aiSubdivUvSmoothing" -min 0 -max 
		3 -en "pin_corners:pin_borders:linear:smooth" -at "enum";
	addAttr -ci true -sn "ai_subdiv_smooth_derivs" -ln "aiSubdivSmoothDerivs" -min 0 
		-max 1 -at "bool";
	addAttr -ci true -sn "ai_disp_height" -ln "aiDispHeight" -dv 1 -at "float";
	addAttr -ci true -sn "ai_disp_padding" -ln "aiDispPadding" -at "float";
	addAttr -ci true -sn "ai_disp_zero_value" -ln "aiDispZeroValue" -at "float";
	addAttr -ci true -sn "ai_disp_autobump" -ln "aiDispAutobump" -min 0 -max 1 -at "bool";
	addAttr -ci true -k true -sn "ai_exptan" -ln "aiExportTangents" -min 0 -max 1 -at "bool";
	addAttr -ci true -k true -sn "ai_expcol" -ln "aiExportColors" -min 0 -max 1 -at "bool";
	addAttr -ci true -k true -sn "ai_exprpt" -ln "aiExportRefPoints" -dv 1 -min 0 -max 
		1 -at "bool";
	addAttr -ci true -k true -sn "ai_exprnrm" -ln "aiExportRefNormals" -min 0 -max 1 
		-at "bool";
	addAttr -ci true -k true -sn "ai_exprtan" -ln "aiExportRefTangents" -min 0 -max 
		1 -at "bool";
	addAttr -ci true -k true -sn "ai_volume_container_mode" -ln "aiVolumeContainerMode" 
		-min 0 -max 1 -en "Mesh:Bounding Box" -at "enum";
	addAttr -ci true -k true -sn "ai_step_size" -ln "aiStepSize" -min 0 -smx 1 -at "float";
	addAttr -ci true -sn "mtoa_constant_myString" -ln "mtoa_constant_myString" -dt "string";
	setAttr -k off ".v";
	setAttr ".vir" yes;
	setAttr ".vif" yes;
	setAttr ".uvst[0].uvsn" -type "string" "map1";
	setAttr -s 4 ".uvst[0].uvsp[0:3]" -type "float2" 0 0 1 0 0 0.96074474 1 0.96074474;
	setAttr ".cuvs" -type "string" "map1";
	setAttr ".dcc" -type "string" "Ambient+Diffuse";
	setAttr ".covm[0]"  0 1 1;
	setAttr ".cdvm[0]"  0 1 1;
	setAttr -s 4 ".vt[0:3]"  -2.4955454 -5.3237016e-16 2.3975821 2.4955454 -5.3237016e-16 
		2.3975821 -2.4955454 5.3237016e-16 -2.3975821 2.4955454 5.3237016e-16 -2.3975821;
	setAttr -s 4 ".ed[0:3]"  0 1 0 0 2 0 1 3 0 2 3 0;
	setAttr ".fc[0]" -type "polyFaces" 
		f 4 0 2 -4 -2 
		mu 0 4 0 1 3 2 ;
	setAttr ".cd" -type "dataPolyComponent" Index_Data Edge 0 ;
	setAttr ".cvd" -type "dataPolyComponent" Index_Data Vertex 0 ;
	setAttr ".hfd" -type "dataPolyComponent" Index_Data Face 0 ;
	setAttr -k on ".mtoa_constant_myString" -type "string" "1004";
createNode transform -n "pPlane7";
	setAttr ".t" -type "double3" 10.880730227047321 0 10.804117128822247 ;
	setAttr ".s" -type "double3" 1.1583769136445605 1.1583769136445605 1.1583769136445605 ;
createNode mesh -n "pPlaneShape1001" -p "|pPlane7";
	addAttr -ci true -k true -sn "ai_user_options" -ln "aiUserOptions" -dt "string";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-min 0 -smx 1 -at "float";
	addAttr -ci true -sn "ai_self_shadows" -ln "aiSelfShadows" -dv 1 -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "ai_opaque" -ln "aiOpaque" -dv 1 -min 0 -max 1 -at "bool";
	addAttr -ci true -k true -sn "trace_sets" -ln "aiTraceSets" -dt "string";
	addAttr -ci true -k true -sn "ai_sss_setname" -ln "aiSssSetname" -dt "string";
	addAttr -ci true -k true -sn "ai_vid" -ln "aiVisibleInDiffuse" -dv 1 -min 0 -max 
		1 -at "bool";
	addAttr -ci true -k true -sn "ai_vig" -ln "aiVisibleInGlossy" -dv 1 -min 0 -max 
		1 -at "bool";
	addAttr -ci true -sn "ai_subdiv_type" -ln "aiSubdivType" -min 0 -max 2 -en "none:catclark:linear" 
		-at "enum";
	addAttr -ci true -sn "ai_subdiv_iterations" -ln "aiSubdivIterations" -dv 1 -min 
		0 -max 100 -smn 0 -smx 10 -at "long";
	addAttr -ci true -sn "ai_subdiv_adaptive_metric" -ln "aiSubdivAdaptiveMetric" -min 
		0 -max 2 -en "auto:edge_length:flatness" -at "enum";
	addAttr -ci true -sn "ai_subdiv_pixel_error" -ln "aiSubdivPixelError" -min 0 -smx 
		10 -at "float";
	addAttr -ci true -k true -sn "ai_subdiv_dicing_camera" -ln "aiSubdivDicingCamera" 
		-at "message";
	addAttr -ci true -sn "ai_subdiv_uv_smoothing" -ln "aiSubdivUvSmoothing" -min 0 -max 
		3 -en "pin_corners:pin_borders:linear:smooth" -at "enum";
	addAttr -ci true -sn "ai_subdiv_smooth_derivs" -ln "aiSubdivSmoothDerivs" -min 0 
		-max 1 -at "bool";
	addAttr -ci true -sn "ai_disp_height" -ln "aiDispHeight" -dv 1 -at "float";
	addAttr -ci true -sn "ai_disp_padding" -ln "aiDispPadding" -at "float";
	addAttr -ci true -sn "ai_disp_zero_value" -ln "aiDispZeroValue" -at "float";
	addAttr -ci true -sn "ai_disp_autobump" -ln "aiDispAutobump" -min 0 -max 1 -at "bool";
	addAttr -ci true -k true -sn "ai_exptan" -ln "aiExportTangents" -min 0 -max 1 -at "bool";
	addAttr -ci true -k true -sn "ai_expcol" -ln "aiExportColors" -min 0 -max 1 -at "bool";
	addAttr -ci true -k true -sn "ai_exprpt" -ln "aiExportRefPoints" -dv 1 -min 0 -max 
		1 -at "bool";
	addAttr -ci true -k true -sn "ai_exprnrm" -ln "aiExportRefNormals" -min 0 -max 1 
		-at "bool";
	addAttr -ci true -k true -sn "ai_exprtan" -ln "aiExportRefTangents" -min 0 -max 
		1 -at "bool";
	addAttr -ci true -k true -sn "ai_volume_container_mode" -ln "aiVolumeContainerMode" 
		-min 0 -max 1 -en "Mesh:Bounding Box" -at "enum";
	addAttr -ci true -k true -sn "ai_step_size" -ln "aiStepSize" -min 0 -smx 1 -at "float";
	addAttr -ci true -sn "mtoa_constant_myString" -ln "mtoa_constant_myString" -dt "string";
	setAttr -k off ".v";
	setAttr ".vir" yes;
	setAttr ".vif" yes;
	setAttr ".uvst[0].uvsn" -type "string" "map1";
	setAttr -s 4 ".uvst[0].uvsp[0:3]" -type "float2" 0 0 1 0 0 0.96074474 1 0.96074474;
	setAttr ".cuvs" -type "string" "map1";
	setAttr ".dcc" -type "string" "Ambient+Diffuse";
	setAttr ".covm[0]"  0 1 1;
	setAttr ".cdvm[0]"  0 1 1;
	setAttr -s 4 ".vt[0:3]"  -2.4955454 -5.3237016e-16 2.3975821 2.4955454 -5.3237016e-16 
		2.3975821 -2.4955454 5.3237016e-16 -2.3975821 2.4955454 5.3237016e-16 -2.3975821;
	setAttr -s 4 ".ed[0:3]"  0 1 0 0 2 0 1 3 0 2 3 0;
	setAttr ".fc[0]" -type "polyFaces" 
		f 4 0 2 -4 -2 
		mu 0 4 0 1 3 2 ;
	setAttr ".cd" -type "dataPolyComponent" Index_Data Edge 0 ;
	setAttr ".cvd" -type "dataPolyComponent" Index_Data Vertex 0 ;
	setAttr ".hfd" -type "dataPolyComponent" Index_Data Face 0 ;
	setAttr -k on ".mtoa_constant_myString" -type "string" "1001";
createNode transform -n "group1";
	setAttr ".t" -type "double3" 6.0639464973609503 0 0 ;
	setAttr ".rp" -type "double3" 11.41947298636258 0 10.804117128822247 ;
	setAttr ".sp" -type "double3" 11.41947298636258 0 10.804117128822247 ;
createNode transform -n "pasted__pPlane7" -p "group1";
	setAttr ".t" -type "double3" 10.880730227047321 0 10.804117128822247 ;
	setAttr ".s" -type "double3" 1.1583769136445605 1.1583769136445605 1.1583769136445605 ;
createNode mesh -n "pPlaneShape1002" -p "pasted__pPlane7";
	addAttr -ci true -k true -sn "ai_user_options" -ln "aiUserOptions" -dt "string";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-min 0 -smx 1 -at "float";
	addAttr -ci true -sn "ai_self_shadows" -ln "aiSelfShadows" -dv 1 -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "ai_opaque" -ln "aiOpaque" -dv 1 -min 0 -max 1 -at "bool";
	addAttr -ci true -k true -sn "trace_sets" -ln "aiTraceSets" -dt "string";
	addAttr -ci true -k true -sn "ai_sss_setname" -ln "aiSssSetname" -dt "string";
	addAttr -ci true -k true -sn "ai_vid" -ln "aiVisibleInDiffuse" -dv 1 -min 0 -max 
		1 -at "bool";
	addAttr -ci true -k true -sn "ai_vig" -ln "aiVisibleInGlossy" -dv 1 -min 0 -max 
		1 -at "bool";
	addAttr -ci true -sn "ai_subdiv_type" -ln "aiSubdivType" -min 0 -max 2 -en "none:catclark:linear" 
		-at "enum";
	addAttr -ci true -sn "ai_subdiv_iterations" -ln "aiSubdivIterations" -dv 1 -min 
		0 -max 100 -smn 0 -smx 10 -at "long";
	addAttr -ci true -sn "ai_subdiv_adaptive_metric" -ln "aiSubdivAdaptiveMetric" -min 
		0 -max 2 -en "auto:edge_length:flatness" -at "enum";
	addAttr -ci true -sn "ai_subdiv_pixel_error" -ln "aiSubdivPixelError" -min 0 -smx 
		10 -at "float";
	addAttr -ci true -k true -sn "ai_subdiv_dicing_camera" -ln "aiSubdivDicingCamera" 
		-at "message";
	addAttr -ci true -sn "ai_subdiv_uv_smoothing" -ln "aiSubdivUvSmoothing" -min 0 -max 
		3 -en "pin_corners:pin_borders:linear:smooth" -at "enum";
	addAttr -ci true -sn "ai_subdiv_smooth_derivs" -ln "aiSubdivSmoothDerivs" -min 0 
		-max 1 -at "bool";
	addAttr -ci true -sn "ai_disp_height" -ln "aiDispHeight" -dv 1 -at "float";
	addAttr -ci true -sn "ai_disp_padding" -ln "aiDispPadding" -at "float";
	addAttr -ci true -sn "ai_disp_zero_value" -ln "aiDispZeroValue" -at "float";
	addAttr -ci true -sn "ai_disp_autobump" -ln "aiDispAutobump" -min 0 -max 1 -at "bool";
	addAttr -ci true -k true -sn "ai_exptan" -ln "aiExportTangents" -min 0 -max 1 -at "bool";
	addAttr -ci true -k true -sn "ai_expcol" -ln "aiExportColors" -min 0 -max 1 -at "bool";
	addAttr -ci true -k true -sn "ai_exprpt" -ln "aiExportRefPoints" -dv 1 -min 0 -max 
		1 -at "bool";
	addAttr -ci true -k true -sn "ai_exprnrm" -ln "aiExportRefNormals" -min 0 -max 1 
		-at "bool";
	addAttr -ci true -k true -sn "ai_exprtan" -ln "aiExportRefTangents" -min 0 -max 
		1 -at "bool";
	addAttr -ci true -k true -sn "ai_volume_container_mode" -ln "aiVolumeContainerMode" 
		-min 0 -max 1 -en "Mesh:Bounding Box" -at "enum";
	addAttr -ci true -k true -sn "ai_step_size" -ln "aiStepSize" -min 0 -smx 1 -at "float";
	addAttr -ci true -sn "mtoa_constant_myString" -ln "mtoa_constant_myString" -dt "string";
	addAttr -ci true -sn "mso" -ln "miShadingSamplesOverride" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "msh" -ln "miShadingSamples" -min 0 -smx 8 -at "float";
	addAttr -ci true -sn "mdo" -ln "miMaxDisplaceOverride" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "mmd" -ln "miMaxDisplace" -min 0 -smx 1 -at "float";
	setAttr -k off ".v";
	setAttr ".vir" yes;
	setAttr ".vif" yes;
	setAttr ".uvst[0].uvsn" -type "string" "map1";
	setAttr -s 4 ".uvst[0].uvsp[0:3]" -type "float2" 0 0 1 0 0 0.96074474 1 0.96074474;
	setAttr ".cuvs" -type "string" "map1";
	setAttr ".dcc" -type "string" "Ambient+Diffuse";
	setAttr ".covm[0]"  0 1 1;
	setAttr ".cdvm[0]"  0 1 1;
	setAttr -s 4 ".vt[0:3]"  -2.4955454 -5.3237016e-16 2.3975821 2.4955454 -5.3237016e-16 
		2.3975821 -2.4955454 5.3237016e-16 -2.3975821 2.4955454 5.3237016e-16 -2.3975821;
	setAttr -s 4 ".ed[0:3]"  0 1 0 0 2 0 1 3 0 2 3 0;
	setAttr ".fc[0]" -type "polyFaces" 
		f 4 0 2 -4 -2 
		mu 0 4 0 1 3 2 ;
	setAttr ".cd" -type "dataPolyComponent" Index_Data Edge 0 ;
	setAttr ".cvd" -type "dataPolyComponent" Index_Data Vertex 0 ;
	setAttr ".hfd" -type "dataPolyComponent" Index_Data Face 0 ;
	setAttr -k on ".mtoa_constant_myString" -type "string" "1001";
createNode transform -n "group2";
	setAttr ".t" -type "double3" 0 0 5.8014485910920097 ;
	setAttr ".rp" -type "double3" 11.41947298636258 0 10.804117128822247 ;
	setAttr ".sp" -type "double3" 11.41947298636258 0 10.804117128822247 ;
createNode transform -n "pPlane7" -p "group2";
	setAttr ".t" -type "double3" 10.880730227047321 0 10.804117128822247 ;
	setAttr ".s" -type "double3" 1.1583769136445605 1.1583769136445605 1.1583769136445605 ;
createNode mesh -n "pPlaneShape1003" -p "|group2|pPlane7";
	addAttr -ci true -k true -sn "ai_user_options" -ln "aiUserOptions" -dt "string";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-min 0 -smx 1 -at "float";
	addAttr -ci true -sn "ai_self_shadows" -ln "aiSelfShadows" -dv 1 -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "ai_opaque" -ln "aiOpaque" -dv 1 -min 0 -max 1 -at "bool";
	addAttr -ci true -k true -sn "trace_sets" -ln "aiTraceSets" -dt "string";
	addAttr -ci true -k true -sn "ai_sss_setname" -ln "aiSssSetname" -dt "string";
	addAttr -ci true -k true -sn "ai_vid" -ln "aiVisibleInDiffuse" -dv 1 -min 0 -max 
		1 -at "bool";
	addAttr -ci true -k true -sn "ai_vig" -ln "aiVisibleInGlossy" -dv 1 -min 0 -max 
		1 -at "bool";
	addAttr -ci true -sn "ai_subdiv_type" -ln "aiSubdivType" -min 0 -max 2 -en "none:catclark:linear" 
		-at "enum";
	addAttr -ci true -sn "ai_subdiv_iterations" -ln "aiSubdivIterations" -dv 1 -min 
		0 -max 100 -smn 0 -smx 10 -at "long";
	addAttr -ci true -sn "ai_subdiv_adaptive_metric" -ln "aiSubdivAdaptiveMetric" -min 
		0 -max 2 -en "auto:edge_length:flatness" -at "enum";
	addAttr -ci true -sn "ai_subdiv_pixel_error" -ln "aiSubdivPixelError" -min 0 -smx 
		10 -at "float";
	addAttr -ci true -k true -sn "ai_subdiv_dicing_camera" -ln "aiSubdivDicingCamera" 
		-at "message";
	addAttr -ci true -sn "ai_subdiv_uv_smoothing" -ln "aiSubdivUvSmoothing" -min 0 -max 
		3 -en "pin_corners:pin_borders:linear:smooth" -at "enum";
	addAttr -ci true -sn "ai_subdiv_smooth_derivs" -ln "aiSubdivSmoothDerivs" -min 0 
		-max 1 -at "bool";
	addAttr -ci true -sn "ai_disp_height" -ln "aiDispHeight" -dv 1 -at "float";
	addAttr -ci true -sn "ai_disp_padding" -ln "aiDispPadding" -at "float";
	addAttr -ci true -sn "ai_disp_zero_value" -ln "aiDispZeroValue" -at "float";
	addAttr -ci true -sn "ai_disp_autobump" -ln "aiDispAutobump" -min 0 -max 1 -at "bool";
	addAttr -ci true -k true -sn "ai_exptan" -ln "aiExportTangents" -min 0 -max 1 -at "bool";
	addAttr -ci true -k true -sn "ai_expcol" -ln "aiExportColors" -min 0 -max 1 -at "bool";
	addAttr -ci true -k true -sn "ai_exprpt" -ln "aiExportRefPoints" -dv 1 -min 0 -max 
		1 -at "bool";
	addAttr -ci true -k true -sn "ai_exprnrm" -ln "aiExportRefNormals" -min 0 -max 1 
		-at "bool";
	addAttr -ci true -k true -sn "ai_exprtan" -ln "aiExportRefTangents" -min 0 -max 
		1 -at "bool";
	addAttr -ci true -k true -sn "ai_volume_container_mode" -ln "aiVolumeContainerMode" 
		-min 0 -max 1 -en "Mesh:Bounding Box" -at "enum";
	addAttr -ci true -k true -sn "ai_step_size" -ln "aiStepSize" -min 0 -smx 1 -at "float";
	addAttr -ci true -sn "mtoa_constant_myString" -ln "mtoa_constant_myString" -dt "string";
	setAttr -k off ".v";
	setAttr ".vir" yes;
	setAttr ".vif" yes;
	setAttr ".uvst[0].uvsn" -type "string" "map1";
	setAttr -s 4 ".uvst[0].uvsp[0:3]" -type "float2" 0 0 1 0 0 0.96074474 1 0.96074474;
	setAttr ".cuvs" -type "string" "map1";
	setAttr ".dcc" -type "string" "Ambient+Diffuse";
	setAttr ".covm[0]"  0 1 1;
	setAttr ".cdvm[0]"  0 1 1;
	setAttr -s 4 ".vt[0:3]"  -2.4955454 -5.3237016e-16 2.3975821 2.4955454 -5.3237016e-16 
		2.3975821 -2.4955454 5.3237016e-16 -2.3975821 2.4955454 5.3237016e-16 -2.3975821;
	setAttr -s 4 ".ed[0:3]"  0 1 0 0 2 0 1 3 0 2 3 0;
	setAttr ".fc[0]" -type "polyFaces" 
		f 4 0 2 -4 -2 
		mu 0 4 0 1 3 2 ;
	setAttr ".cd" -type "dataPolyComponent" Index_Data Edge 0 ;
	setAttr ".cvd" -type "dataPolyComponent" Index_Data Vertex 0 ;
	setAttr ".hfd" -type "dataPolyComponent" Index_Data Face 0 ;
	setAttr -k on ".mtoa_constant_myString" -type "string" "1001";
createNode transform -n "group3";
	setAttr ".t" -type "double3" 5.9381250254779854 0 5.8367052810465854 ;
	setAttr ".rp" -type "double3" 11.41947298636258 0 10.804117128822247 ;
	setAttr ".sp" -type "double3" 11.41947298636258 0 10.804117128822247 ;
createNode transform -n "pPlane7" -p "group3";
	setAttr ".t" -type "double3" 10.880730227047321 0 10.804117128822247 ;
	setAttr ".s" -type "double3" 1.1583769136445605 1.1583769136445605 1.1583769136445605 ;
createNode mesh -n "pPlaneShape1003" -p "|group3|pPlane7";
	addAttr -ci true -k true -sn "ai_user_options" -ln "aiUserOptions" -dt "string";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-min 0 -smx 1 -at "float";
	addAttr -ci true -sn "ai_self_shadows" -ln "aiSelfShadows" -dv 1 -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "ai_opaque" -ln "aiOpaque" -dv 1 -min 0 -max 1 -at "bool";
	addAttr -ci true -k true -sn "trace_sets" -ln "aiTraceSets" -dt "string";
	addAttr -ci true -k true -sn "ai_sss_setname" -ln "aiSssSetname" -dt "string";
	addAttr -ci true -k true -sn "ai_vid" -ln "aiVisibleInDiffuse" -dv 1 -min 0 -max 
		1 -at "bool";
	addAttr -ci true -k true -sn "ai_vig" -ln "aiVisibleInGlossy" -dv 1 -min 0 -max 
		1 -at "bool";
	addAttr -ci true -sn "ai_subdiv_type" -ln "aiSubdivType" -min 0 -max 2 -en "none:catclark:linear" 
		-at "enum";
	addAttr -ci true -sn "ai_subdiv_iterations" -ln "aiSubdivIterations" -dv 1 -min 
		0 -max 100 -smn 0 -smx 10 -at "long";
	addAttr -ci true -sn "ai_subdiv_adaptive_metric" -ln "aiSubdivAdaptiveMetric" -min 
		0 -max 2 -en "auto:edge_length:flatness" -at "enum";
	addAttr -ci true -sn "ai_subdiv_pixel_error" -ln "aiSubdivPixelError" -min 0 -smx 
		10 -at "float";
	addAttr -ci true -k true -sn "ai_subdiv_dicing_camera" -ln "aiSubdivDicingCamera" 
		-at "message";
	addAttr -ci true -sn "ai_subdiv_uv_smoothing" -ln "aiSubdivUvSmoothing" -min 0 -max 
		3 -en "pin_corners:pin_borders:linear:smooth" -at "enum";
	addAttr -ci true -sn "ai_subdiv_smooth_derivs" -ln "aiSubdivSmoothDerivs" -min 0 
		-max 1 -at "bool";
	addAttr -ci true -sn "ai_disp_height" -ln "aiDispHeight" -dv 1 -at "float";
	addAttr -ci true -sn "ai_disp_padding" -ln "aiDispPadding" -at "float";
	addAttr -ci true -sn "ai_disp_zero_value" -ln "aiDispZeroValue" -at "float";
	addAttr -ci true -sn "ai_disp_autobump" -ln "aiDispAutobump" -min 0 -max 1 -at "bool";
	addAttr -ci true -k true -sn "ai_exptan" -ln "aiExportTangents" -min 0 -max 1 -at "bool";
	addAttr -ci true -k true -sn "ai_expcol" -ln "aiExportColors" -min 0 -max 1 -at "bool";
	addAttr -ci true -k true -sn "ai_exprpt" -ln "aiExportRefPoints" -dv 1 -min 0 -max 
		1 -at "bool";
	addAttr -ci true -k true -sn "ai_exprnrm" -ln "aiExportRefNormals" -min 0 -max 1 
		-at "bool";
	addAttr -ci true -k true -sn "ai_exprtan" -ln "aiExportRefTangents" -min 0 -max 
		1 -at "bool";
	addAttr -ci true -k true -sn "ai_volume_container_mode" -ln "aiVolumeContainerMode" 
		-min 0 -max 1 -en "Mesh:Bounding Box" -at "enum";
	addAttr -ci true -k true -sn "ai_step_size" -ln "aiStepSize" -min 0 -smx 1 -at "float";
	addAttr -ci true -sn "mtoa_constant_myString" -ln "mtoa_constant_myString" -dt "string";
	addAttr -ci true -sn "mso" -ln "miShadingSamplesOverride" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "msh" -ln "miShadingSamples" -min 0 -smx 8 -at "float";
	addAttr -ci true -sn "mdo" -ln "miMaxDisplaceOverride" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "mmd" -ln "miMaxDisplace" -min 0 -smx 1 -at "float";
	setAttr -k off ".v";
	setAttr ".vir" yes;
	setAttr ".vif" yes;
	setAttr ".uvst[0].uvsn" -type "string" "map1";
	setAttr -s 4 ".uvst[0].uvsp[0:3]" -type "float2" 0 0 1 0 0 0.96074474 1 0.96074474;
	setAttr ".cuvs" -type "string" "map1";
	setAttr ".dcc" -type "string" "Ambient+Diffuse";
	setAttr ".covm[0]"  0 1 1;
	setAttr ".cdvm[0]"  0 1 1;
	setAttr -s 4 ".vt[0:3]"  -2.4955454 -5.3237016e-16 2.3975821 2.4955454 -5.3237016e-16 
		2.3975821 -2.4955454 5.3237016e-16 -2.3975821 2.4955454 5.3237016e-16 -2.3975821;
	setAttr -s 4 ".ed[0:3]"  0 1 0 0 2 0 1 3 0 2 3 0;
	setAttr ".fc[0]" -type "polyFaces" 
		f 4 0 2 -4 -2 
		mu 0 4 0 1 3 2 ;
	setAttr ".cd" -type "dataPolyComponent" Index_Data Edge 0 ;
	setAttr ".cvd" -type "dataPolyComponent" Index_Data Vertex 0 ;
	setAttr ".hfd" -type "dataPolyComponent" Index_Data Face 0 ;
	setAttr -k on ".mtoa_constant_myString" -type "string" "1001";
createNode transform -n "pPlane8";
	setAttr ".t" -type "double3" 25.498345327022669 0 2.3337462221316119 ;
createNode mesh -n "pPlaneShape8" -p "pPlane8";
	addAttr -ci true -k true -sn "ai_user_options" -ln "aiUserOptions" -dt "string";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-min 0 -smx 1 -at "float";
	addAttr -ci true -sn "ai_self_shadows" -ln "aiSelfShadows" -dv 1 -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "ai_opaque" -ln "aiOpaque" -dv 1 -min 0 -max 1 -at "bool";
	addAttr -ci true -k true -sn "trace_sets" -ln "aiTraceSets" -dt "string";
	addAttr -ci true -k true -sn "ai_sss_setname" -ln "aiSssSetname" -dt "string";
	addAttr -ci true -k true -sn "ai_vid" -ln "aiVisibleInDiffuse" -dv 1 -min 0 -max 
		1 -at "bool";
	addAttr -ci true -k true -sn "ai_vig" -ln "aiVisibleInGlossy" -dv 1 -min 0 -max 
		1 -at "bool";
	addAttr -ci true -sn "ai_subdiv_type" -ln "aiSubdivType" -min 0 -max 2 -en "none:catclark:linear" 
		-at "enum";
	addAttr -ci true -sn "ai_subdiv_iterations" -ln "aiSubdivIterations" -dv 1 -min 
		0 -max 100 -smn 0 -smx 10 -at "long";
	addAttr -ci true -sn "ai_subdiv_adaptive_metric" -ln "aiSubdivAdaptiveMetric" -min 
		0 -max 2 -en "auto:edge_length:flatness" -at "enum";
	addAttr -ci true -sn "ai_subdiv_pixel_error" -ln "aiSubdivPixelError" -min 0 -smx 
		10 -at "float";
	addAttr -ci true -k true -sn "ai_subdiv_dicing_camera" -ln "aiSubdivDicingCamera" 
		-at "message";
	addAttr -ci true -sn "ai_subdiv_uv_smoothing" -ln "aiSubdivUvSmoothing" -min 0 -max 
		3 -en "pin_corners:pin_borders:linear:smooth" -at "enum";
	addAttr -ci true -sn "ai_subdiv_smooth_derivs" -ln "aiSubdivSmoothDerivs" -min 0 
		-max 1 -at "bool";
	addAttr -ci true -sn "ai_disp_height" -ln "aiDispHeight" -dv 1 -at "float";
	addAttr -ci true -sn "ai_disp_padding" -ln "aiDispPadding" -at "float";
	addAttr -ci true -sn "ai_disp_zero_value" -ln "aiDispZeroValue" -at "float";
	addAttr -ci true -sn "ai_disp_autobump" -ln "aiDispAutobump" -min 0 -max 1 -at "bool";
	addAttr -ci true -k true -sn "ai_exptan" -ln "aiExportTangents" -min 0 -max 1 -at "bool";
	addAttr -ci true -k true -sn "ai_expcol" -ln "aiExportColors" -min 0 -max 1 -at "bool";
	addAttr -ci true -k true -sn "ai_exprpt" -ln "aiExportRefPoints" -dv 1 -min 0 -max 
		1 -at "bool";
	addAttr -ci true -k true -sn "ai_exprnrm" -ln "aiExportRefNormals" -min 0 -max 1 
		-at "bool";
	addAttr -ci true -k true -sn "ai_exprtan" -ln "aiExportRefTangents" -min 0 -max 
		1 -at "bool";
	addAttr -ci true -k true -sn "ai_volume_container_mode" -ln "aiVolumeContainerMode" 
		-min 0 -max 1 -en "Mesh:Bounding Box" -at "enum";
	addAttr -ci true -k true -sn "ai_step_size" -ln "aiStepSize" -min 0 -smx 1 -at "float";
	addAttr -ci true -sn "mso" -ln "miShadingSamplesOverride" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "msh" -ln "miShadingSamples" -min 0 -smx 8 -at "float";
	addAttr -ci true -sn "mdo" -ln "miMaxDisplaceOverride" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "mmd" -ln "miMaxDisplace" -min 0 -smx 1 -at "float";
	setAttr -k off ".v";
	setAttr ".vir" yes;
	setAttr ".vif" yes;
	setAttr ".pv" -type "double2" 1.0315449237823486 1.5064982054507841 ;
	setAttr ".uvst[0].uvsn" -type "string" "map1";
	setAttr -s 16 ".uvst[0].uvsp[0:15]" -type "float2" 1.2438611 1.3030871 1.7438611 
		1.3030871 0.81922877 1.2787113 0.29912809 0.29390275 1.2378938 0.30627072 0.81922877 
		1.7099094 0.29912809 0.72510093 1.2378938 0.73746872 1.7378938 0.73746872 0.79912812 
		0.72510093 0.31922877 1.7099094 1.7438611 1.734285 0.79912812 0.29390275 1.7378938 
		0.30627072 1.2438611 1.734285 0.31922877 1.2787113;
	setAttr ".cuvs" -type "string" "map1";
	setAttr ".dcc" -type "string" "Ambient+Diffuse";
	setAttr ".covm[0]"  0 1 1;
	setAttr ".cdvm[0]"  0 1 1;
	setAttr -s 9 ".pt[0:8]" -type "float3"  0 -2.3841858e-07 0 0 -2.3841858e-07 0 0 
		-2.3841858e-07 0 0 2.646978e-23 6.6174449e-24 0 2.646978e-23 6.6174449e-24 0 2.646978e-23 
		6.6174449e-24 0 2.3841858e-07 0 0 2.3841858e-07 0 0 2.3841858e-07 0;
	setAttr -s 9 ".vt[0:8]"  -5.781405 -2.3841858e-07 4.9858618 0 -2.3841858e-07 4.9858618 
		5.781405 -2.3841858e-07 4.9858618 -5.781405 2.646978e-23 6.6174449e-24 0 2.646978e-23 
		6.6174449e-24 5.781405 2.646978e-23 6.6174449e-24 -5.781405 2.3841858e-07 -4.9858618 
		0 2.3841858e-07 -4.9858618 5.781405 2.3841858e-07 -4.9858618;
	setAttr -s 12 ".ed[0:11]"  0 1 0 0 3 0 1 2 0 1 4 1 2 5 0 3 4 1 3 6 0 4 5 1 4 7 1 
		5 8 0 6 7 0 7 8 0;
	setAttr -s 4 ".fc[0:3]" -type "polyFaces" 
		f 4 0 3 -6 -2 
		mu 0 4 0 1 11 14 
		f 4 2 4 -8 -4 
		mu 0 4 15 2 5 10 
		f 4 5 8 -11 -7 
		mu 0 4 3 12 9 6 
		f 4 7 9 -12 -9 
		mu 0 4 4 13 8 7 ;
	setAttr ".cd" -type "dataPolyComponent" Index_Data Edge 0 ;
	setAttr ".cvd" -type "dataPolyComponent" Index_Data Vertex 0 ;
	setAttr ".hfd" -type "dataPolyComponent" Index_Data Face 0 ;
createNode lightLinker -s -n "lightLinker1";
	setAttr -s 11 ".lnk";
	setAttr -s 11 ".slnk";
createNode displayLayerManager -n "layerManager";
createNode displayLayer -n "defaultLayer";
createNode renderLayerManager -n "renderLayerManager";
createNode renderLayer -n "defaultRenderLayer";
	setAttr ".g" yes;
createNode polyPlane -n "polyPlane1";
	setAttr ".w" 11.562810012906045;
	setAttr ".h" 9.9717231649193288;
	setAttr ".sw" 2;
	setAttr ".sh" 2;
	setAttr ".cuv" 2;
createNode aiOptions -s -n "defaultArnoldRenderOptions";
createNode aiAOVFilter -s -n "defaultArnoldFilter";
	addAttr -ci true -k true -sn "ai_user_options" -ln "aiUserOptions" -dt "string";
	addAttr -ci true -sn "width" -ln "width" -dv 2 -at "float";
	addAttr -ci true -sn "domain" -ln "domain" -min 0 -max 1 -en "first_hit:all_hits" 
		-at "enum";
	addAttr -ci true -sn "minimum" -ln "minimum" -at "float";
	addAttr -ci true -sn "maximum" -ln "maximum" -dv 1 -at "float";
	addAttr -ci true -sn "scalar_mode" -ln "scalarMode" -min 0 -max 1 -at "bool";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVDriver -s -n "defaultArnoldDriver";
	addAttr -ci true -k true -sn "ai_user_options" -ln "aiUserOptions" -dt "string";
	addAttr -ci true -sn "exr_compression" -ln "exrCompression" -dv 3 -min 0 -max 5 
		-en "none:rle:zips:zip:piz:pxr24" -at "enum";
	addAttr -ci true -sn "half_precision" -ln "halfPrecision" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "tiled" -ln "tiled" -dv 1 -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "preserve_layer_name" -ln "preserveLayerName" -min 0 -max 1 
		-at "bool";
	addAttr -ci true -sn "autocrop" -ln "autocrop" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "append" -ln "append" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "quality" -ln "quality" -dv 100 -min 0 -max 100 -at "long";
	addAttr -ci true -sn "output_padded" -ln "outputPadded" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "gamma" -ln "gamma" -dv 2.2000000476837158 -min 9.9999997473787516e-05 
		-smx 5 -at "float";
	addAttr -ci true -sn "dither_amplitude" -ln "ditherAmplitude" -dv 1 -at "float";
	addAttr -ci true -sn "png_format" -ln "pngFormat" -min 0 -max 1 -en "int8:int16" 
		-at "enum";
	addAttr -ci true -sn "tiff_compression" -ln "tiffCompression" -dv 1 -min 0 -max 
		4 -en "none:lzw:ccittrle:zip:packbits" -at "enum";
	addAttr -ci true -sn "tiff_format" -ln "tiffFormat" -min 0 -max 3 -en "int8:int16:int32:float32" 
		-at "enum";
	addAttr -ci true -sn "unpremult_alpha" -ln "unpremultAlpha" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "host_name" -ln "hostName" -dt "string";
	setAttr ".ai_translator" -type "string" "tif";
	setAttr ".tiled" no;
createNode aiStandard -n "aiStandard1";
createNode shadingEngine -n "aiStandard1SG";
	addAttr -ci true -m -sn "aovs" -ln "aiCustomAOVs" -at "compound" -nc 2;
	addAttr -ci true -k true -sn "aov_name" -ln "aovName" -dt "string" -p "aiCustomAOVs";
	addAttr -ci true -k true -sn "aov_input" -ln "aovInput" -at "message" -p "aiCustomAOVs";
	addAttr -ci true -k true -sn "ai_surface_shader" -ln "aiSurfaceShader" -at "message";
	addAttr -ci true -k true -sn "ai_volume_shader" -ln "aiVolumeShader" -at "message";
	addAttr -ci true -k true -sn "ai_user_options" -ln "aiUserOptions" -dt "string";
	setAttr ".ihi" 0;
	setAttr ".ro" yes;
createNode materialInfo -n "materialInfo1";
createNode file -n "file_udim";
	addAttr -ci true -k true -sn "ai_user_options" -ln "aiUserOptions" -dt "string";
	addAttr -ci true -sn "ai_mipBias" -ln "aiMipBias" -at "long";
	addAttr -ci true -sn "ai_filter" -ln "aiFilter" -dv 3 -min 0 -max 3 -en "closest:bilinear:bicubic:smart_bicubic" 
		-at "enum";
	addAttr -ci true -sn "ai_useDefaultColor" -ln "aiUseDefaultColor" -dv 1 -min 0 -max 
		1 -at "bool";
	setAttr ".ftn" -type "string" "./test/mari.<udim>.jpg";
createNode place2dTexture -n "place2dTexture1";
createNode script -n "sceneConfigurationScriptNode";
	setAttr ".b" -type "string" "playbackOptions -min 1 -max 24 -ast 1 -aet 48 ";
	setAttr ".st" 6;
createNode polyTweakUV -n "polyTweakUV1";
	setAttr ".uopa" yes;
	setAttr -s 9 ".uvtk[0:8]" -type "float2" 0.033340443 0.038897183 0.033340458 0.038897183 
		0.033340398 0.038897183 0.033340443 0.03889719 0.033340458 0.03889719 0.033340398 
		0.03889719 0.033340443 0.03889716 0.033340458 0.03889716 0.033340398 0.03889716;
createNode polyMapCut -n "polyMapCut1";
	setAttr ".uopa" yes;
	setAttr ".ics" -type "componentList" 2 "e[8]" "e[10:11]";
createNode polyMapCut -n "polyMapCut2";
	setAttr ".uopa" yes;
	setAttr ".ics" -type "componentList" 2 "e[8]" "e[10:11]";
createNode polyMapCut -n "polyMapCut3";
	setAttr ".uopa" yes;
	setAttr ".ics" -type "componentList" 3 "e[3]" "e[5]" "e[7:8]";
createNode polyTweakUV -n "polyTweakUV2";
	setAttr ".uopa" yes;
	setAttr -s 16 ".uvtk[0:15]" -type "float2" 3.1110625 0.26085815 3.1110625 0.26085815 
		1.6381931 0.26542473 0.26578763 -0.17619254 0.70455343 -0.16382457 1.6381931 0.26542473 
		0.26578763 -0.17619248 0.70455343 -0.16382466 0.70455343 -0.16382466 0.26578766 -0.17619248 
		1.6381931 0.26542473 3.1110625 0.260858 0.26578766 -0.17619254 0.70455343 -0.16382457 
		3.1110625 0.260858 1.6381931 0.26542473;
createNode aiStandard -n "aiStandard2";
createNode shadingEngine -n "aiStandard2SG";
	addAttr -ci true -m -sn "aovs" -ln "aiCustomAOVs" -at "compound" -nc 2;
	addAttr -ci true -k true -sn "aov_name" -ln "aovName" -dt "string" -p "aiCustomAOVs";
	addAttr -ci true -k true -sn "aov_input" -ln "aovInput" -at "message" -p "aiCustomAOVs";
	addAttr -ci true -k true -sn "ai_surface_shader" -ln "aiSurfaceShader" -at "message";
	addAttr -ci true -k true -sn "ai_volume_shader" -ln "aiVolumeShader" -at "message";
	addAttr -ci true -k true -sn "ai_user_options" -ln "aiUserOptions" -dt "string";
	setAttr ".ihi" 0;
	setAttr ".ro" yes;
createNode materialInfo -n "materialInfo2";
createNode file -n "file_tile";
	addAttr -ci true -k true -sn "ai_user_options" -ln "aiUserOptions" -dt "string";
	addAttr -ci true -sn "ai_mipBias" -ln "aiMipBias" -at "long";
	addAttr -ci true -sn "ai_filter" -ln "aiFilter" -dv 3 -min 0 -max 3 -en "closest:bilinear:bicubic:smart_bicubic" 
		-at "enum";
	addAttr -ci true -sn "ai_useDefaultColor" -ln "aiUseDefaultColor" -dv 1 -min 0 -max 
		1 -at "bool";
	setAttr ".ftn" -type "string" "./test/mudbox.<tile>.jpg";
createNode place2dTexture -n "place2dTexture2";
createNode materialInfo -n "pasted__materialInfo1";
createNode shadingEngine -n "pasted__aiStandard1SG";
	addAttr -ci true -m -sn "aovs" -ln "aiCustomAOVs" -at "compound" -nc 2;
	addAttr -ci true -k true -sn "aov_name" -ln "aovName" -dt "string" -p "aiCustomAOVs";
	addAttr -ci true -k true -sn "aov_input" -ln "aovInput" -at "message" -p "aiCustomAOVs";
	addAttr -ci true -k true -sn "ai_surface_shader" -ln "aiSurfaceShader" -at "message";
	addAttr -ci true -k true -sn "ai_volume_shader" -ln "aiVolumeShader" -at "message";
	addAttr -ci true -k true -sn "ai_user_options" -ln "aiUserOptions" -dt "string";
	setAttr ".ihi" 0;
	setAttr ".ro" yes;
createNode aiStandard -n "pasted__aiStandard1";
createNode file -n "pasted__file1";
	addAttr -ci true -k true -sn "ai_user_options" -ln "aiUserOptions" -dt "string";
	addAttr -ci true -sn "ai_mipBias" -ln "aiMipBias" -at "long";
	addAttr -ci true -sn "ai_filter" -ln "aiFilter" -dv 3 -min 0 -max 3 -en "closest:bilinear:bicubic:smart_bicubic" 
		-at "enum";
	addAttr -ci true -sn "ai_useDefaultColor" -ln "aiUseDefaultColor" -dv 1 -min 0 -max 
		1 -at "bool";
	setAttr ".ftn" -type "string" "./test/mari.<udim>.jpg";
createNode place2dTexture -n "pasted__place2dTexture1";
createNode polyPlane -n "polyPlane2";
	setAttr ".w" 4.9910908449713203;
	setAttr ".h" 4.7951639057995674;
	setAttr ".sw" 1;
	setAttr ".sh" 1;
	setAttr ".cuv" 2;
createNode aiStandard -n "aiStandard3";
createNode shadingEngine -n "aiStandard3SG";
	addAttr -ci true -m -sn "aovs" -ln "aiCustomAOVs" -at "compound" -nc 2;
	addAttr -ci true -k true -sn "aov_name" -ln "aovName" -dt "string" -p "aiCustomAOVs";
	addAttr -ci true -k true -sn "aov_input" -ln "aovInput" -at "message" -p "aiCustomAOVs";
	addAttr -ci true -k true -sn "ai_surface_shader" -ln "aiSurfaceShader" -at "message";
	addAttr -ci true -k true -sn "ai_volume_shader" -ln "aiVolumeShader" -at "message";
	addAttr -ci true -k true -sn "ai_user_options" -ln "aiUserOptions" -dt "string";
	setAttr ".ihi" 0;
	setAttr -s 4 ".dsm";
	setAttr ".ro" yes;
createNode materialInfo -n "materialInfo3";
createNode file -n "file_attr";
	addAttr -ci true -k true -sn "ai_user_options" -ln "aiUserOptions" -dt "string";
	addAttr -ci true -sn "ai_mipBias" -ln "aiMipBias" -at "long";
	addAttr -ci true -sn "ai_filter" -ln "aiFilter" -dv 3 -min 0 -max 3 -en "closest:bilinear:bicubic:smart_bicubic" 
		-at "enum";
	addAttr -ci true -sn "ai_useDefaultColor" -ln "aiUseDefaultColor" -dv 1 -min 0 -max 
		1 -at "bool";
	setAttr ".ftn" -type "string" "./test/mari.<attr:myString>.jpg";
createNode place2dTexture -n "place2dTexture3";
createNode aiStandard -n "aiStandard4";
createNode shadingEngine -n "aiStandard4SG";
	addAttr -ci true -m -sn "aovs" -ln "aiCustomAOVs" -at "compound" -nc 2;
	addAttr -ci true -k true -sn "aov_name" -ln "aovName" -dt "string" -p "aiCustomAOVs";
	addAttr -ci true -k true -sn "aov_input" -ln "aovInput" -at "message" -p "aiCustomAOVs";
	addAttr -ci true -k true -sn "ai_surface_shader" -ln "aiSurfaceShader" -at "message";
	addAttr -ci true -k true -sn "ai_volume_shader" -ln "aiVolumeShader" -at "message";
	addAttr -ci true -k true -sn "ai_user_options" -ln "aiUserOptions" -dt "string";
	setAttr ".ihi" 0;
	setAttr ".ro" yes;
createNode materialInfo -n "materialInfo4";
createNode file -n "file_utile_vtile";
	addAttr -ci true -k true -sn "ai_user_options" -ln "aiUserOptions" -dt "string";
	addAttr -ci true -sn "ai_mipBias" -ln "aiMipBias" -at "long";
	addAttr -ci true -sn "ai_filter" -ln "aiFilter" -dv 3 -min 0 -max 3 -en "closest:bilinear:bicubic:smart_bicubic" 
		-at "enum";
	addAttr -ci true -sn "ai_useDefaultColor" -ln "aiUseDefaultColor" -dv 1 -min 0 -max 
		1 -at "bool";
	setAttr ".ftn" -type "string" "./test/mudbox._u<utile>_v<vtile>.jpg";
createNode place2dTexture -n "place2dTexture4";
createNode materialInfo -n "pasted__materialInfo4";
createNode shadingEngine -n "pasted__aiStandard4SG";
	addAttr -ci true -m -sn "aovs" -ln "aiCustomAOVs" -at "compound" -nc 2;
	addAttr -ci true -k true -sn "aov_name" -ln "aovName" -dt "string" -p "aiCustomAOVs";
	addAttr -ci true -k true -sn "aov_input" -ln "aovInput" -at "message" -p "aiCustomAOVs";
	addAttr -ci true -k true -sn "ai_surface_shader" -ln "aiSurfaceShader" -at "message";
	addAttr -ci true -k true -sn "ai_volume_shader" -ln "aiVolumeShader" -at "message";
	addAttr -ci true -k true -sn "ai_user_options" -ln "aiUserOptions" -dt "string";
	setAttr ".ihi" 0;
	setAttr -s 2 ".dsm";
	setAttr ".ro" yes;
createNode aiStandard -n "pasted__aiStandard4";
createNode file -n "file_shapeName";
	addAttr -ci true -k true -sn "ai_user_options" -ln "aiUserOptions" -dt "string";
	addAttr -ci true -sn "ai_mipBias" -ln "aiMipBias" -at "long";
	addAttr -ci true -sn "ai_filter" -ln "aiFilter" -dv 3 -min 0 -max 3 -en "closest:bilinear:bicubic:smart_bicubic" 
		-at "enum";
	addAttr -ci true -sn "ai_useDefaultColor" -ln "aiUseDefaultColor" -dv 1 -min 0 -max 
		1 -at "bool";
	setAttr ".ftn" -type "string" "./test/mari.<shapeName>.jpg";
createNode place2dTexture -n "pasted__place2dTexture4";
createNode materialInfo -n "pasted__materialInfo5";
createNode shadingEngine -n "pasted__aiStandard4SG1";
	addAttr -ci true -m -sn "aovs" -ln "aiCustomAOVs" -at "compound" -nc 2;
	addAttr -ci true -k true -sn "aov_name" -ln "aovName" -dt "string" -p "aiCustomAOVs";
	addAttr -ci true -k true -sn "aov_input" -ln "aovInput" -at "message" -p "aiCustomAOVs";
	addAttr -ci true -k true -sn "ai_surface_shader" -ln "aiSurfaceShader" -at "message";
	addAttr -ci true -k true -sn "ai_volume_shader" -ln "aiVolumeShader" -at "message";
	addAttr -ci true -k true -sn "ai_user_options" -ln "aiUserOptions" -dt "string";
	setAttr ".ihi" 0;
	setAttr ".ro" yes;
createNode aiStandard -n "pasted__aiStandard5";
createNode file -n "pasted__file5";
	addAttr -ci true -k true -sn "ai_user_options" -ln "aiUserOptions" -dt "string";
	addAttr -ci true -sn "ai_mipBias" -ln "aiMipBias" -at "long";
	addAttr -ci true -sn "ai_filter" -ln "aiFilter" -dv 3 -min 0 -max 3 -en "closest:bilinear:bicubic:smart_bicubic" 
		-at "enum";
	addAttr -ci true -sn "ai_useDefaultColor" -ln "aiUseDefaultColor" -dv 1 -min 0 -max 
		1 -at "bool";
	setAttr ".ftn" -type "string" "./test/mari.<shapeName>.jpg";
createNode place2dTexture -n "pasted__place2dTexture5";
createNode materialInfo -n "pasted__materialInfo6";
createNode shadingEngine -n "pasted__aiStandard4SG2";
	addAttr -ci true -m -sn "aovs" -ln "aiCustomAOVs" -at "compound" -nc 2;
	addAttr -ci true -k true -sn "aov_name" -ln "aovName" -dt "string" -p "aiCustomAOVs";
	addAttr -ci true -k true -sn "aov_input" -ln "aovInput" -at "message" -p "aiCustomAOVs";
	addAttr -ci true -k true -sn "ai_surface_shader" -ln "aiSurfaceShader" -at "message";
	addAttr -ci true -k true -sn "ai_volume_shader" -ln "aiVolumeShader" -at "message";
	addAttr -ci true -k true -sn "ai_user_options" -ln "aiUserOptions" -dt "string";
	setAttr ".ihi" 0;
	setAttr ".ro" yes;
createNode aiStandard -n "pasted__aiStandard6";
createNode file -n "pasted__file6";
	addAttr -ci true -k true -sn "ai_user_options" -ln "aiUserOptions" -dt "string";
	addAttr -ci true -sn "ai_mipBias" -ln "aiMipBias" -at "long";
	addAttr -ci true -sn "ai_filter" -ln "aiFilter" -dv 3 -min 0 -max 3 -en "closest:bilinear:bicubic:smart_bicubic" 
		-at "enum";
	addAttr -ci true -sn "ai_useDefaultColor" -ln "aiUseDefaultColor" -dv 1 -min 0 -max 
		1 -at "bool";
	setAttr ".ftn" -type "string" "./test/mari.<shapeName>.jpg";
createNode place2dTexture -n "pasted__place2dTexture6";
createNode aiStandard -n "aiStandard5";
createNode shadingEngine -n "aiStandard5SG";
	addAttr -ci true -m -sn "aovs" -ln "aiCustomAOVs" -at "compound" -nc 2;
	addAttr -ci true -k true -sn "aov_name" -ln "aovName" -dt "string" -p "aiCustomAOVs";
	addAttr -ci true -k true -sn "aov_input" -ln "aovInput" -at "message" -p "aiCustomAOVs";
	addAttr -ci true -k true -sn "ai_surface_shader" -ln "aiSurfaceShader" -at "message";
	addAttr -ci true -k true -sn "ai_volume_shader" -ln "aiVolumeShader" -at "message";
	addAttr -ci true -k true -sn "ai_user_options" -ln "aiUserOptions" -dt "string";
	setAttr ".ihi" 0;
	setAttr -s 2 ".dsm";
	setAttr ".ro" yes;
createNode materialInfo -n "materialInfo5";
createNode file -n "file_shapePath";
	addAttr -ci true -k true -sn "ai_user_options" -ln "aiUserOptions" -dt "string";
	addAttr -ci true -sn "ai_mipBias" -ln "aiMipBias" -at "long";
	addAttr -ci true -sn "ai_filter" -ln "aiFilter" -dv 3 -min 0 -max 3 -en "closest:bilinear:bicubic:smart_bicubic" 
		-at "enum";
	addAttr -ci true -sn "ai_useDefaultColor" -ln "aiUseDefaultColor" -dv 1 -min 0 -max 
		1 -at "bool";
	setAttr ".ftn" -type "string" "./test/mari.<shapePath>.jpg";
createNode place2dTexture -n "place2dTexture5";
select -ne :time1;
	setAttr -av -k on ".cch";
	setAttr -cb on ".ihi";
	setAttr -k on ".nds";
	setAttr -cb on ".bnm";
	setAttr -k on ".o" 1;
	setAttr ".unw" 1;
select -ne :renderPartition;
	setAttr -k on ".cch";
	setAttr -cb on ".ihi";
	setAttr -k on ".nds";
	setAttr -cb on ".bnm";
	setAttr -s 11 ".st";
	setAttr -cb on ".an";
	setAttr -cb on ".pt";
select -ne :initialShadingGroup;
	addAttr -ci true -m -sn "aovs" -ln "aiCustomAOVs" -at "compound" -nc 2;
	addAttr -ci true -k true -sn "aov_name" -ln "aovName" -dt "string" -p "aiCustomAOVs";
	addAttr -ci true -k true -sn "aov_input" -ln "aovInput" -at "message" -p "aiCustomAOVs";
	addAttr -ci true -k true -sn "ai_surface_shader" -ln "aiSurfaceShader" -at "message";
	addAttr -ci true -k true -sn "ai_volume_shader" -ln "aiVolumeShader" -at "message";
	addAttr -ci true -k true -sn "ai_user_options" -ln "aiUserOptions" -dt "string";
	setAttr -k on ".cch";
	setAttr -cb on ".ihi";
	setAttr -av -k on ".nds";
	setAttr -cb on ".bnm";
	setAttr -k on ".mwc";
	setAttr -cb on ".an";
	setAttr -cb on ".il";
	setAttr -cb on ".vo";
	setAttr -cb on ".eo";
	setAttr -cb on ".fo";
	setAttr -cb on ".epo";
	setAttr ".ro" yes;
	setAttr -cb on ".mimt";
	setAttr -cb on ".miop";
	setAttr -cb on ".mise";
	setAttr -cb on ".mism";
	setAttr -cb on ".mice";
	setAttr -av -cb on ".micc";
	setAttr -cb on ".mica";
	setAttr -cb on ".micw";
	setAttr -cb on ".mirw";
select -ne :initialParticleSE;
	addAttr -ci true -m -sn "aovs" -ln "aiCustomAOVs" -at "compound" -nc 2;
	addAttr -ci true -k true -sn "aov_name" -ln "aovName" -dt "string" -p "aiCustomAOVs";
	addAttr -ci true -k true -sn "aov_input" -ln "aovInput" -at "message" -p "aiCustomAOVs";
	addAttr -ci true -k true -sn "ai_surface_shader" -ln "aiSurfaceShader" -at "message";
	addAttr -ci true -k true -sn "ai_volume_shader" -ln "aiVolumeShader" -at "message";
	addAttr -ci true -k true -sn "ai_user_options" -ln "aiUserOptions" -dt "string";
	setAttr -k on ".cch";
	setAttr -cb on ".ihi";
	setAttr -k on ".nds";
	setAttr -cb on ".bnm";
	setAttr -k on ".mwc";
	setAttr -cb on ".an";
	setAttr -cb on ".il";
	setAttr -cb on ".vo";
	setAttr -cb on ".eo";
	setAttr -cb on ".fo";
	setAttr -cb on ".epo";
	setAttr ".ro" yes;
	setAttr -cb on ".mimt";
	setAttr -cb on ".miop";
	setAttr -cb on ".mise";
	setAttr -cb on ".mism";
	setAttr -cb on ".mice";
	setAttr -cb on ".micc";
	setAttr -cb on ".mica";
	setAttr -cb on ".micw";
	setAttr -cb on ".mirw";
select -ne :defaultShaderList1;
	setAttr -k on ".cch";
	setAttr -cb on ".ihi";
	setAttr -k on ".nds";
	setAttr -cb on ".bnm";
	setAttr -s 11 ".s";
select -ne :defaultTextureList1;
	setAttr -s 9 ".tx";
select -ne :lightList1;
select -ne :postProcessList1;
	setAttr -k on ".cch";
	setAttr -cb on ".ihi";
	setAttr -k on ".nds";
	setAttr -cb on ".bnm";
	setAttr -s 2 ".p";
select -ne :defaultRenderUtilityList1;
	setAttr -s 9 ".u";
select -ne :renderGlobalsList1;
	setAttr -k on ".cch";
	setAttr -cb on ".ihi";
	setAttr -k on ".nds";
	setAttr -cb on ".bnm";
select -ne :defaultRenderGlobals;
	setAttr -k on ".cch";
	setAttr -cb on ".ihi";
	setAttr -k on ".nds";
	setAttr -cb on ".bnm";
	setAttr -k on ".macc";
	setAttr -k on ".macd";
	setAttr -k on ".macq";
	setAttr -k on ".mcfr";
	setAttr -cb on ".ifg";
	setAttr -k on ".clip";
	setAttr -k on ".edm";
	setAttr -k on ".edl";
	setAttr -cb on ".ren" -type "string" "arnold";
	setAttr -av -k on ".esr";
	setAttr -k on ".ors";
	setAttr -cb on ".sdf";
	setAttr -av -k on ".outf" 51;
	setAttr -cb on ".imfkey" -type "string" "tif";
	setAttr -k on ".gama";
	setAttr -k on ".ar";
	setAttr -k on ".fs";
	setAttr -k on ".ef";
	setAttr -av -k on ".bfs";
	setAttr -k on ".me";
	setAttr -k on ".se";
	setAttr -k on ".be";
	setAttr -k on ".fec";
	setAttr -k on ".ofc";
	setAttr -cb on ".ofe";
	setAttr -cb on ".efe";
	setAttr -k on ".oft";
	setAttr -cb on ".umfn";
	setAttr -cb on ".ufe";
	setAttr -cb on ".peie";
	setAttr -cb on ".ifp";
	setAttr -k on ".comp";
	setAttr -k on ".cth";
	setAttr -k on ".soll";
	setAttr -k on ".rd";
	setAttr -k on ".lp";
	setAttr -av -k on ".sp";
	setAttr -k on ".shs";
	setAttr -k on ".lpr";
	setAttr -cb on ".gv";
	setAttr -cb on ".sv";
	setAttr -k on ".mm";
	setAttr -k on ".npu";
	setAttr -k on ".itf";
	setAttr -k on ".shp";
	setAttr -cb on ".isp";
	setAttr -k on ".uf";
	setAttr -k on ".oi";
	setAttr -k on ".rut";
	setAttr -k on ".mb";
	setAttr -av -k on ".mbf";
	setAttr -k on ".afp";
	setAttr -k on ".pfb";
	setAttr -k on ".pram";
	setAttr -k on ".poam";
	setAttr -k on ".prlm";
	setAttr -k on ".polm";
	setAttr -cb on ".prm";
	setAttr -cb on ".pom";
	setAttr -cb on ".pfrm";
	setAttr -cb on ".pfom";
	setAttr -av -k on ".bll";
	setAttr -k on ".bls";
	setAttr -av -k on ".smv";
	setAttr -k on ".ubc";
	setAttr -k on ".mbc";
	setAttr -cb on ".mbt";
	setAttr -k on ".udbx";
	setAttr -k on ".smc";
	setAttr -k on ".kmv";
	setAttr -cb on ".isl";
	setAttr -cb on ".ism";
	setAttr -cb on ".imb";
	setAttr -k on ".rlen";
	setAttr -av -k on ".frts";
	setAttr -k on ".tlwd";
	setAttr -k on ".tlht";
	setAttr -k on ".jfc";
	setAttr -cb on ".rsb";
	setAttr -k on ".ope";
	setAttr -k on ".oppf";
	setAttr -cb on ".hbl";
select -ne :defaultResolution;
	setAttr -k on ".cch";
	setAttr -k on ".ihi";
	setAttr -k on ".nds";
	setAttr -k on ".bnm";
	setAttr -av ".w" 160;
	setAttr -av ".h" 120;
	setAttr -av -k on ".pa" 1;
	setAttr -k on ".al";
	setAttr -av ".dar" 1.3333333730697632;
	setAttr -k on ".ldar";
	setAttr -k on ".dpi";
	setAttr -k on ".off";
	setAttr -k on ".fld";
	setAttr -k on ".zsl";
	setAttr -k on ".isu";
	setAttr -k on ".pdu";
select -ne :defaultLightSet;
	addAttr -ci true -k true -sn "ai_override" -ln "aiOverride" -dv 1 -min 0 -max 1 
		-at "bool";
	addAttr -ci true -k true -sn "ai_user_options" -ln "aiUserOptions" -dt "string";
	setAttr -k on ".cch";
	setAttr -k on ".ihi";
	setAttr -k on ".nds";
	setAttr -k on ".bnm";
	setAttr -k on ".mwc";
	setAttr -k on ".an";
	setAttr -k on ".il";
	setAttr -k on ".vo";
	setAttr -k on ".eo";
	setAttr -k on ".fo";
	setAttr -k on ".epo";
	setAttr ".ro" yes;
select -ne :defaultObjectSet;
	addAttr -ci true -k true -sn "ai_override" -ln "aiOverride" -dv 1 -min 0 -max 1 
		-at "bool";
	addAttr -ci true -k true -sn "ai_user_options" -ln "aiUserOptions" -dt "string";
	setAttr ".ro" yes;
select -ne :hardwareRenderGlobals;
	setAttr ".ctrs" 256;
	setAttr ".btrs" 512;
select -ne :defaultHardwareRenderGlobals;
	setAttr -k on ".cch";
	setAttr -cb on ".ihi";
	setAttr -k on ".nds";
	setAttr -cb on ".bnm";
	setAttr -k on ".rp";
	setAttr -k on ".cai";
	setAttr -k on ".coi";
	setAttr -cb on ".bc";
	setAttr -av -k on ".bcb";
	setAttr -av -k on ".bcg";
	setAttr -av -k on ".bcr";
	setAttr -k on ".ei";
	setAttr -k on ".ex";
	setAttr -av -k on ".es";
	setAttr -av -k on ".ef";
	setAttr -k on ".bf";
	setAttr -k on ".fii";
	setAttr -av -k on ".sf";
	setAttr -k on ".gr";
	setAttr -k on ".li";
	setAttr -k on ".ls";
	setAttr -k on ".mb";
	setAttr -k on ".ti";
	setAttr -k on ".txt";
	setAttr -k on ".mpr";
	setAttr -k on ".wzd";
	setAttr ".fn" -type "string" "im";
	setAttr -k on ".if";
	setAttr ".res" -type "string" "ntsc_4d 646 485 1.333";
	setAttr -k on ".as";
	setAttr -k on ".ds";
	setAttr -k on ".lm";
	setAttr -k on ".fir";
	setAttr -k on ".aap";
	setAttr -k on ".gh";
	setAttr -cb on ".sd";
connectAttr "polyTweakUV2.out" "pPlaneShape1.i";
connectAttr "polyTweakUV2.uvtk[0]" "pPlaneShape1.uvst[0].uvtw";
connectAttr "polyPlane2.out" "pPlaneShape3.i";
relationship "link" ":lightLinker1" ":initialShadingGroup.message" ":defaultLightSet.message";
relationship "link" ":lightLinker1" ":initialParticleSE.message" ":defaultLightSet.message";
relationship "link" ":lightLinker1" "aiStandard1SG.message" ":defaultLightSet.message";
relationship "link" ":lightLinker1" "aiStandard2SG.message" ":defaultLightSet.message";
relationship "link" ":lightLinker1" "pasted__aiStandard1SG.message" ":defaultLightSet.message";
relationship "link" ":lightLinker1" "aiStandard3SG.message" ":defaultLightSet.message";
relationship "link" ":lightLinker1" "aiStandard4SG.message" ":defaultLightSet.message";
relationship "link" ":lightLinker1" "pasted__aiStandard4SG.message" ":defaultLightSet.message";
relationship "link" ":lightLinker1" "pasted__aiStandard4SG1.message" ":defaultLightSet.message";
relationship "link" ":lightLinker1" "pasted__aiStandard4SG2.message" ":defaultLightSet.message";
relationship "link" ":lightLinker1" "aiStandard5SG.message" ":defaultLightSet.message";
relationship "shadowLink" ":lightLinker1" ":initialShadingGroup.message" ":defaultLightSet.message";
relationship "shadowLink" ":lightLinker1" ":initialParticleSE.message" ":defaultLightSet.message";
relationship "shadowLink" ":lightLinker1" "aiStandard1SG.message" ":defaultLightSet.message";
relationship "shadowLink" ":lightLinker1" "aiStandard2SG.message" ":defaultLightSet.message";
relationship "shadowLink" ":lightLinker1" "pasted__aiStandard1SG.message" ":defaultLightSet.message";
relationship "shadowLink" ":lightLinker1" "aiStandard3SG.message" ":defaultLightSet.message";
relationship "shadowLink" ":lightLinker1" "aiStandard4SG.message" ":defaultLightSet.message";
relationship "shadowLink" ":lightLinker1" "pasted__aiStandard4SG.message" ":defaultLightSet.message";
relationship "shadowLink" ":lightLinker1" "pasted__aiStandard4SG1.message" ":defaultLightSet.message";
relationship "shadowLink" ":lightLinker1" "pasted__aiStandard4SG2.message" ":defaultLightSet.message";
relationship "shadowLink" ":lightLinker1" "aiStandard5SG.message" ":defaultLightSet.message";
connectAttr "layerManager.dli[0]" "defaultLayer.id";
connectAttr "renderLayerManager.rlmi[0]" "defaultRenderLayer.rlid";
connectAttr ":defaultArnoldFilter.msg" ":defaultArnoldRenderOptions.filt";
connectAttr ":defaultArnoldDriver.msg" ":defaultArnoldRenderOptions.drvr";
connectAttr "file_udim.oc" "aiStandard1.Kd_color";
connectAttr "aiStandard1.out" "aiStandard1SG.ss";
connectAttr "pPlaneShape1.iog" "aiStandard1SG.dsm" -na;
connectAttr "aiStandard1SG.msg" "materialInfo1.sg";
connectAttr "aiStandard1.msg" "materialInfo1.m";
connectAttr "aiStandard1.msg" "materialInfo1.t" -na;
connectAttr "place2dTexture1.c" "file_udim.c";
connectAttr "place2dTexture1.tf" "file_udim.tf";
connectAttr "place2dTexture1.rf" "file_udim.rf";
connectAttr "place2dTexture1.mu" "file_udim.mu";
connectAttr "place2dTexture1.mv" "file_udim.mv";
connectAttr "place2dTexture1.s" "file_udim.s";
connectAttr "place2dTexture1.wu" "file_udim.wu";
connectAttr "place2dTexture1.wv" "file_udim.wv";
connectAttr "place2dTexture1.re" "file_udim.re";
connectAttr "place2dTexture1.of" "file_udim.of";
connectAttr "place2dTexture1.r" "file_udim.ro";
connectAttr "place2dTexture1.n" "file_udim.n";
connectAttr "place2dTexture1.vt1" "file_udim.vt1";
connectAttr "place2dTexture1.vt2" "file_udim.vt2";
connectAttr "place2dTexture1.vt3" "file_udim.vt3";
connectAttr "place2dTexture1.vc1" "file_udim.vc1";
connectAttr "place2dTexture1.o" "file_udim.uv";
connectAttr "place2dTexture1.ofs" "file_udim.fs";
connectAttr "polyPlane1.out" "polyTweakUV1.ip";
connectAttr "polyTweakUV1.out" "polyMapCut1.ip";
connectAttr "polyMapCut1.out" "polyMapCut2.ip";
connectAttr "polyMapCut2.out" "polyMapCut3.ip";
connectAttr "polyMapCut3.out" "polyTweakUV2.ip";
connectAttr "file_tile.oc" "aiStandard2.Kd_color";
connectAttr "aiStandard2.out" "aiStandard2SG.ss";
connectAttr "pPlaneShape2.iog" "aiStandard2SG.dsm" -na;
connectAttr "aiStandard2SG.msg" "materialInfo2.sg";
connectAttr "aiStandard2.msg" "materialInfo2.m";
connectAttr "aiStandard2.msg" "materialInfo2.t" -na;
connectAttr "place2dTexture2.c" "file_tile.c";
connectAttr "place2dTexture2.tf" "file_tile.tf";
connectAttr "place2dTexture2.rf" "file_tile.rf";
connectAttr "place2dTexture2.mu" "file_tile.mu";
connectAttr "place2dTexture2.mv" "file_tile.mv";
connectAttr "place2dTexture2.s" "file_tile.s";
connectAttr "place2dTexture2.wu" "file_tile.wu";
connectAttr "place2dTexture2.wv" "file_tile.wv";
connectAttr "place2dTexture2.re" "file_tile.re";
connectAttr "place2dTexture2.of" "file_tile.of";
connectAttr "place2dTexture2.r" "file_tile.ro";
connectAttr "place2dTexture2.n" "file_tile.n";
connectAttr "place2dTexture2.vt1" "file_tile.vt1";
connectAttr "place2dTexture2.vt2" "file_tile.vt2";
connectAttr "place2dTexture2.vt3" "file_tile.vt3";
connectAttr "place2dTexture2.vc1" "file_tile.vc1";
connectAttr "place2dTexture2.o" "file_tile.uv";
connectAttr "place2dTexture2.ofs" "file_tile.fs";
connectAttr "pasted__aiStandard1SG.msg" "pasted__materialInfo1.sg";
connectAttr "pasted__aiStandard1.msg" "pasted__materialInfo1.m";
connectAttr "pasted__aiStandard1.msg" "pasted__materialInfo1.t" -na;
connectAttr "pasted__aiStandard1.out" "pasted__aiStandard1SG.ss";
connectAttr "pasted__file1.oc" "pasted__aiStandard1.Kd_color";
connectAttr "pasted__place2dTexture1.c" "pasted__file1.c";
connectAttr "pasted__place2dTexture1.tf" "pasted__file1.tf";
connectAttr "pasted__place2dTexture1.rf" "pasted__file1.rf";
connectAttr "pasted__place2dTexture1.mu" "pasted__file1.mu";
connectAttr "pasted__place2dTexture1.mv" "pasted__file1.mv";
connectAttr "pasted__place2dTexture1.s" "pasted__file1.s";
connectAttr "pasted__place2dTexture1.wu" "pasted__file1.wu";
connectAttr "pasted__place2dTexture1.wv" "pasted__file1.wv";
connectAttr "pasted__place2dTexture1.re" "pasted__file1.re";
connectAttr "pasted__place2dTexture1.of" "pasted__file1.of";
connectAttr "pasted__place2dTexture1.r" "pasted__file1.ro";
connectAttr "pasted__place2dTexture1.n" "pasted__file1.n";
connectAttr "pasted__place2dTexture1.vt1" "pasted__file1.vt1";
connectAttr "pasted__place2dTexture1.vt2" "pasted__file1.vt2";
connectAttr "pasted__place2dTexture1.vt3" "pasted__file1.vt3";
connectAttr "pasted__place2dTexture1.vc1" "pasted__file1.vc1";
connectAttr "pasted__place2dTexture1.o" "pasted__file1.uv";
connectAttr "pasted__place2dTexture1.ofs" "pasted__file1.fs";
connectAttr "file_attr.oc" "aiStandard3.Kd_color";
connectAttr "aiStandard3.out" "aiStandard3SG.ss";
connectAttr "pPlaneShape3.iog" "aiStandard3SG.dsm" -na;
connectAttr "pPlaneShape4.iog" "aiStandard3SG.dsm" -na;
connectAttr "pPlaneShape5.iog" "aiStandard3SG.dsm" -na;
connectAttr "pPlaneShape6.iog" "aiStandard3SG.dsm" -na;
connectAttr "aiStandard3SG.msg" "materialInfo3.sg";
connectAttr "aiStandard3.msg" "materialInfo3.m";
connectAttr "aiStandard3.msg" "materialInfo3.t" -na;
connectAttr "place2dTexture3.c" "file_attr.c";
connectAttr "place2dTexture3.tf" "file_attr.tf";
connectAttr "place2dTexture3.rf" "file_attr.rf";
connectAttr "place2dTexture3.mu" "file_attr.mu";
connectAttr "place2dTexture3.mv" "file_attr.mv";
connectAttr "place2dTexture3.s" "file_attr.s";
connectAttr "place2dTexture3.wu" "file_attr.wu";
connectAttr "place2dTexture3.wv" "file_attr.wv";
connectAttr "place2dTexture3.re" "file_attr.re";
connectAttr "place2dTexture3.of" "file_attr.of";
connectAttr "place2dTexture3.r" "file_attr.ro";
connectAttr "place2dTexture3.n" "file_attr.n";
connectAttr "place2dTexture3.vt1" "file_attr.vt1";
connectAttr "place2dTexture3.vt2" "file_attr.vt2";
connectAttr "place2dTexture3.vt3" "file_attr.vt3";
connectAttr "place2dTexture3.vc1" "file_attr.vc1";
connectAttr "place2dTexture3.o" "file_attr.uv";
connectAttr "place2dTexture3.ofs" "file_attr.fs";
connectAttr "file_utile_vtile.oc" "aiStandard4.Kd_color";
connectAttr "aiStandard4.out" "aiStandard4SG.ss";
connectAttr "pPlaneShape8.iog" "aiStandard4SG.dsm" -na;
connectAttr "aiStandard4SG.msg" "materialInfo4.sg";
connectAttr "aiStandard4.msg" "materialInfo4.m";
connectAttr "aiStandard4.msg" "materialInfo4.t" -na;
connectAttr "place2dTexture4.c" "file_utile_vtile.c";
connectAttr "place2dTexture4.tf" "file_utile_vtile.tf";
connectAttr "place2dTexture4.rf" "file_utile_vtile.rf";
connectAttr "place2dTexture4.mu" "file_utile_vtile.mu";
connectAttr "place2dTexture4.mv" "file_utile_vtile.mv";
connectAttr "place2dTexture4.s" "file_utile_vtile.s";
connectAttr "place2dTexture4.wu" "file_utile_vtile.wu";
connectAttr "place2dTexture4.wv" "file_utile_vtile.wv";
connectAttr "place2dTexture4.re" "file_utile_vtile.re";
connectAttr "place2dTexture4.of" "file_utile_vtile.of";
connectAttr "place2dTexture4.r" "file_utile_vtile.ro";
connectAttr "place2dTexture4.n" "file_utile_vtile.n";
connectAttr "place2dTexture4.vt1" "file_utile_vtile.vt1";
connectAttr "place2dTexture4.vt2" "file_utile_vtile.vt2";
connectAttr "place2dTexture4.vt3" "file_utile_vtile.vt3";
connectAttr "place2dTexture4.vc1" "file_utile_vtile.vc1";
connectAttr "place2dTexture4.o" "file_utile_vtile.uv";
connectAttr "place2dTexture4.ofs" "file_utile_vtile.fs";
connectAttr "pasted__aiStandard4SG.msg" "pasted__materialInfo4.sg";
connectAttr "pasted__aiStandard4.msg" "pasted__materialInfo4.m";
connectAttr "pasted__aiStandard4.msg" "pasted__materialInfo4.t" -na;
connectAttr "pasted__aiStandard4.out" "pasted__aiStandard4SG.ss";
connectAttr "pPlaneShape1002.iog" "pasted__aiStandard4SG.dsm" -na;
connectAttr "pPlaneShape1001.iog" "pasted__aiStandard4SG.dsm" -na;
connectAttr "file_shapeName.oc" "pasted__aiStandard4.Kd_color";
connectAttr "pasted__place2dTexture4.c" "file_shapeName.c";
connectAttr "pasted__place2dTexture4.tf" "file_shapeName.tf";
connectAttr "pasted__place2dTexture4.rf" "file_shapeName.rf";
connectAttr "pasted__place2dTexture4.mu" "file_shapeName.mu";
connectAttr "pasted__place2dTexture4.mv" "file_shapeName.mv";
connectAttr "pasted__place2dTexture4.s" "file_shapeName.s";
connectAttr "pasted__place2dTexture4.wu" "file_shapeName.wu";
connectAttr "pasted__place2dTexture4.wv" "file_shapeName.wv";
connectAttr "pasted__place2dTexture4.re" "file_shapeName.re";
connectAttr "pasted__place2dTexture4.of" "file_shapeName.of";
connectAttr "pasted__place2dTexture4.r" "file_shapeName.ro";
connectAttr "pasted__place2dTexture4.n" "file_shapeName.n";
connectAttr "pasted__place2dTexture4.vt1" "file_shapeName.vt1";
connectAttr "pasted__place2dTexture4.vt2" "file_shapeName.vt2";
connectAttr "pasted__place2dTexture4.vt3" "file_shapeName.vt3";
connectAttr "pasted__place2dTexture4.vc1" "file_shapeName.vc1";
connectAttr "pasted__place2dTexture4.o" "file_shapeName.uv";
connectAttr "pasted__place2dTexture4.ofs" "file_shapeName.fs";
connectAttr "pasted__aiStandard4SG1.msg" "pasted__materialInfo5.sg";
connectAttr "pasted__aiStandard5.msg" "pasted__materialInfo5.m";
connectAttr "pasted__aiStandard5.msg" "pasted__materialInfo5.t" -na;
connectAttr "pasted__aiStandard5.out" "pasted__aiStandard4SG1.ss";
connectAttr "pasted__file5.oc" "pasted__aiStandard5.Kd_color";
connectAttr "pasted__place2dTexture5.c" "pasted__file5.c";
connectAttr "pasted__place2dTexture5.tf" "pasted__file5.tf";
connectAttr "pasted__place2dTexture5.rf" "pasted__file5.rf";
connectAttr "pasted__place2dTexture5.mu" "pasted__file5.mu";
connectAttr "pasted__place2dTexture5.mv" "pasted__file5.mv";
connectAttr "pasted__place2dTexture5.s" "pasted__file5.s";
connectAttr "pasted__place2dTexture5.wu" "pasted__file5.wu";
connectAttr "pasted__place2dTexture5.wv" "pasted__file5.wv";
connectAttr "pasted__place2dTexture5.re" "pasted__file5.re";
connectAttr "pasted__place2dTexture5.of" "pasted__file5.of";
connectAttr "pasted__place2dTexture5.r" "pasted__file5.ro";
connectAttr "pasted__place2dTexture5.n" "pasted__file5.n";
connectAttr "pasted__place2dTexture5.vt1" "pasted__file5.vt1";
connectAttr "pasted__place2dTexture5.vt2" "pasted__file5.vt2";
connectAttr "pasted__place2dTexture5.vt3" "pasted__file5.vt3";
connectAttr "pasted__place2dTexture5.vc1" "pasted__file5.vc1";
connectAttr "pasted__place2dTexture5.o" "pasted__file5.uv";
connectAttr "pasted__place2dTexture5.ofs" "pasted__file5.fs";
connectAttr "pasted__aiStandard4SG2.msg" "pasted__materialInfo6.sg";
connectAttr "pasted__aiStandard6.msg" "pasted__materialInfo6.m";
connectAttr "pasted__aiStandard6.msg" "pasted__materialInfo6.t" -na;
connectAttr "pasted__aiStandard6.out" "pasted__aiStandard4SG2.ss";
connectAttr "pasted__file6.oc" "pasted__aiStandard6.Kd_color";
connectAttr "pasted__place2dTexture6.c" "pasted__file6.c";
connectAttr "pasted__place2dTexture6.tf" "pasted__file6.tf";
connectAttr "pasted__place2dTexture6.rf" "pasted__file6.rf";
connectAttr "pasted__place2dTexture6.mu" "pasted__file6.mu";
connectAttr "pasted__place2dTexture6.mv" "pasted__file6.mv";
connectAttr "pasted__place2dTexture6.s" "pasted__file6.s";
connectAttr "pasted__place2dTexture6.wu" "pasted__file6.wu";
connectAttr "pasted__place2dTexture6.wv" "pasted__file6.wv";
connectAttr "pasted__place2dTexture6.re" "pasted__file6.re";
connectAttr "pasted__place2dTexture6.of" "pasted__file6.of";
connectAttr "pasted__place2dTexture6.r" "pasted__file6.ro";
connectAttr "pasted__place2dTexture6.n" "pasted__file6.n";
connectAttr "pasted__place2dTexture6.vt1" "pasted__file6.vt1";
connectAttr "pasted__place2dTexture6.vt2" "pasted__file6.vt2";
connectAttr "pasted__place2dTexture6.vt3" "pasted__file6.vt3";
connectAttr "pasted__place2dTexture6.vc1" "pasted__file6.vc1";
connectAttr "pasted__place2dTexture6.o" "pasted__file6.uv";
connectAttr "pasted__place2dTexture6.ofs" "pasted__file6.fs";
connectAttr "file_shapePath.oc" "aiStandard5.Kd_color";
connectAttr "aiStandard5.out" "aiStandard5SG.ss";
connectAttr "|group3|pPlane7|pPlaneShape1003.iog" "aiStandard5SG.dsm" -na;
connectAttr "|group2|pPlane7|pPlaneShape1003.iog" "aiStandard5SG.dsm" -na;
connectAttr "aiStandard5SG.msg" "materialInfo5.sg";
connectAttr "aiStandard5.msg" "materialInfo5.m";
connectAttr "aiStandard5.msg" "materialInfo5.t" -na;
connectAttr "place2dTexture5.c" "file_shapePath.c";
connectAttr "place2dTexture5.tf" "file_shapePath.tf";
connectAttr "place2dTexture5.rf" "file_shapePath.rf";
connectAttr "place2dTexture5.mu" "file_shapePath.mu";
connectAttr "place2dTexture5.mv" "file_shapePath.mv";
connectAttr "place2dTexture5.s" "file_shapePath.s";
connectAttr "place2dTexture5.wu" "file_shapePath.wu";
connectAttr "place2dTexture5.wv" "file_shapePath.wv";
connectAttr "place2dTexture5.re" "file_shapePath.re";
connectAttr "place2dTexture5.of" "file_shapePath.of";
connectAttr "place2dTexture5.r" "file_shapePath.ro";
connectAttr "place2dTexture5.n" "file_shapePath.n";
connectAttr "place2dTexture5.vt1" "file_shapePath.vt1";
connectAttr "place2dTexture5.vt2" "file_shapePath.vt2";
connectAttr "place2dTexture5.vt3" "file_shapePath.vt3";
connectAttr "place2dTexture5.vc1" "file_shapePath.vc1";
connectAttr "place2dTexture5.o" "file_shapePath.uv";
connectAttr "place2dTexture5.ofs" "file_shapePath.fs";
connectAttr "aiStandard1SG.pa" ":renderPartition.st" -na;
connectAttr "aiStandard2SG.pa" ":renderPartition.st" -na;
connectAttr "pasted__aiStandard1SG.pa" ":renderPartition.st" -na;
connectAttr "aiStandard3SG.pa" ":renderPartition.st" -na;
connectAttr "aiStandard4SG.pa" ":renderPartition.st" -na;
connectAttr "pasted__aiStandard4SG.pa" ":renderPartition.st" -na;
connectAttr "pasted__aiStandard4SG1.pa" ":renderPartition.st" -na;
connectAttr "pasted__aiStandard4SG2.pa" ":renderPartition.st" -na;
connectAttr "aiStandard5SG.pa" ":renderPartition.st" -na;
connectAttr "aiStandard1.msg" ":defaultShaderList1.s" -na;
connectAttr "aiStandard2.msg" ":defaultShaderList1.s" -na;
connectAttr "pasted__aiStandard1.msg" ":defaultShaderList1.s" -na;
connectAttr "aiStandard3.msg" ":defaultShaderList1.s" -na;
connectAttr "aiStandard4.msg" ":defaultShaderList1.s" -na;
connectAttr "pasted__aiStandard4.msg" ":defaultShaderList1.s" -na;
connectAttr "pasted__aiStandard5.msg" ":defaultShaderList1.s" -na;
connectAttr "pasted__aiStandard6.msg" ":defaultShaderList1.s" -na;
connectAttr "aiStandard5.msg" ":defaultShaderList1.s" -na;
connectAttr "file_udim.msg" ":defaultTextureList1.tx" -na;
connectAttr "file_tile.msg" ":defaultTextureList1.tx" -na;
connectAttr "pasted__file1.msg" ":defaultTextureList1.tx" -na;
connectAttr "file_attr.msg" ":defaultTextureList1.tx" -na;
connectAttr "file_utile_vtile.msg" ":defaultTextureList1.tx" -na;
connectAttr "file_shapeName.msg" ":defaultTextureList1.tx" -na;
connectAttr "pasted__file5.msg" ":defaultTextureList1.tx" -na;
connectAttr "pasted__file6.msg" ":defaultTextureList1.tx" -na;
connectAttr "file_shapePath.msg" ":defaultTextureList1.tx" -na;
connectAttr "directionalLightShape1.ltd" ":lightList1.l" -na;
connectAttr "place2dTexture1.msg" ":defaultRenderUtilityList1.u" -na;
connectAttr "place2dTexture2.msg" ":defaultRenderUtilityList1.u" -na;
connectAttr "pasted__place2dTexture1.msg" ":defaultRenderUtilityList1.u" -na;
connectAttr "place2dTexture3.msg" ":defaultRenderUtilityList1.u" -na;
connectAttr "place2dTexture4.msg" ":defaultRenderUtilityList1.u" -na;
connectAttr "pasted__place2dTexture4.msg" ":defaultRenderUtilityList1.u" -na;
connectAttr "pasted__place2dTexture5.msg" ":defaultRenderUtilityList1.u" -na;
connectAttr "pasted__place2dTexture6.msg" ":defaultRenderUtilityList1.u" -na;
connectAttr "place2dTexture5.msg" ":defaultRenderUtilityList1.u" -na;
connectAttr "directionalLight1.iog" ":defaultLightSet.dsm" -na;
// End of test.ma

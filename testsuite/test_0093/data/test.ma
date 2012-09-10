//Maya ASCII 2011 scene
//Name: test.ma
//Last modified: Mon, Aug 27, 2012 05:30:29 PM
//Codeset: 1252
requires maya "2011";
requires "mtoa" "0.19.0.dev";
requires "Mayatomr" "2011.0m - 3.8.1.33 ";
requires "stereoCamera" "10.0";
currentUnit -l centimeter -a degree -t film;
fileInfo "application" "maya";
fileInfo "product" "Maya 2011";
fileInfo "version" "2011 x64";
fileInfo "cutIdentifier" "201009060330-781623";
fileInfo "osv" "Microsoft Windows 7 Business Edition, 64-bit Windows 7 Service Pack 1 (Build 7601)\n";
createNode transform -s -n "persp";
	setAttr ".v" no;
	setAttr ".t" -type "double3" 1.8504574514214569 -1.0770697050290727 7.5921441709163897 ;
	setAttr ".r" -type "double3" -11.138352729602609 2.6000000000002843 -4.9747378351975742e-017 ;
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
	addAttr -ci true -h true -sn "ai_uv_remapa" -ln "aiUvRemapA" -at "float";
	addAttr -ci true -uac -sn "ai_uv_remap" -ln "aiUvRemap" -at "float3" -nc 3;
	addAttr -ci true -sn "ai_uv_remapr" -ln "aiUvRemapR" -at "float" -p "aiUvRemap";
	addAttr -ci true -sn "ai_uv_remapg" -ln "aiUvRemapG" -at "float" -p "aiUvRemap";
	addAttr -ci true -sn "ai_uv_remapb" -ln "aiUvRemapB" -at "float" -p "aiUvRemap";
	addAttr -ci true -k true -sn "ai_translator" -ln "aiTranslator" -dt "string";
	setAttr -k off ".v" no;
	setAttr ".ovr" 1.3;
	setAttr ".fl" 34.999999999999986;
	setAttr ".coi" 7.3353301727623323;
	setAttr ".imn" -type "string" "persp";
	setAttr ".den" -type "string" "persp_depth";
	setAttr ".man" -type "string" "persp_mask";
	setAttr ".tp" -type "double3" -2.86102294921875e-006 0 -2.86102294921875e-006 ;
	setAttr ".hc" -type "string" "viewSet -p %camera";
	setAttr ".dr" yes;
	setAttr -k on ".ai_translator" -type "string" "perspective";
createNode transform -s -n "top";
	setAttr ".v" no;
	setAttr ".t" -type "double3" 0 1000.1 0 ;
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
	addAttr -ci true -h true -sn "ai_uv_remapa" -ln "aiUvRemapA" -at "float";
	addAttr -ci true -uac -sn "ai_uv_remap" -ln "aiUvRemap" -at "float3" -nc 3;
	addAttr -ci true -sn "ai_uv_remapr" -ln "aiUvRemapR" -at "float" -p "aiUvRemap";
	addAttr -ci true -sn "ai_uv_remapg" -ln "aiUvRemapG" -at "float" -p "aiUvRemap";
	addAttr -ci true -sn "ai_uv_remapb" -ln "aiUvRemapB" -at "float" -p "aiUvRemap";
	addAttr -ci true -k true -sn "ai_translator" -ln "aiTranslator" -dt "string";
	setAttr -k off ".v" no;
	setAttr ".rnd" no;
	setAttr ".coi" 1000.1;
	setAttr ".ow" 30;
	setAttr ".imn" -type "string" "top";
	setAttr ".den" -type "string" "top_depth";
	setAttr ".man" -type "string" "top_mask";
	setAttr ".hc" -type "string" "viewSet -t %camera";
	setAttr ".o" yes;
	setAttr -k on ".ai_translator" -type "string" "orthographic";
createNode transform -s -n "front";
	setAttr ".v" no;
	setAttr ".t" -type "double3" 0 0 1000.1 ;
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
	addAttr -ci true -h true -sn "ai_uv_remapa" -ln "aiUvRemapA" -at "float";
	addAttr -ci true -uac -sn "ai_uv_remap" -ln "aiUvRemap" -at "float3" -nc 3;
	addAttr -ci true -sn "ai_uv_remapr" -ln "aiUvRemapR" -at "float" -p "aiUvRemap";
	addAttr -ci true -sn "ai_uv_remapg" -ln "aiUvRemapG" -at "float" -p "aiUvRemap";
	addAttr -ci true -sn "ai_uv_remapb" -ln "aiUvRemapB" -at "float" -p "aiUvRemap";
	addAttr -ci true -k true -sn "ai_translator" -ln "aiTranslator" -dt "string";
	setAttr -k off ".v" no;
	setAttr ".rnd" no;
	setAttr ".coi" 1000.1;
	setAttr ".ow" 30;
	setAttr ".imn" -type "string" "front";
	setAttr ".den" -type "string" "front_depth";
	setAttr ".man" -type "string" "front_mask";
	setAttr ".hc" -type "string" "viewSet -f %camera";
	setAttr ".o" yes;
	setAttr -k on ".ai_translator" -type "string" "orthographic";
createNode transform -s -n "side";
	setAttr ".v" no;
	setAttr ".t" -type "double3" 1000.1 0 0 ;
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
	addAttr -ci true -h true -sn "ai_uv_remapa" -ln "aiUvRemapA" -at "float";
	addAttr -ci true -uac -sn "ai_uv_remap" -ln "aiUvRemap" -at "float3" -nc 3;
	addAttr -ci true -sn "ai_uv_remapr" -ln "aiUvRemapR" -at "float" -p "aiUvRemap";
	addAttr -ci true -sn "ai_uv_remapg" -ln "aiUvRemapG" -at "float" -p "aiUvRemap";
	addAttr -ci true -sn "ai_uv_remapb" -ln "aiUvRemapB" -at "float" -p "aiUvRemap";
	addAttr -ci true -k true -sn "ai_translator" -ln "aiTranslator" -dt "string";
	setAttr -k off ".v" no;
	setAttr ".rnd" no;
	setAttr ".coi" 1000.1;
	setAttr ".ow" 30;
	setAttr ".imn" -type "string" "side";
	setAttr ".den" -type "string" "side_depth";
	setAttr ".man" -type "string" "side_mask";
	setAttr ".hc" -type "string" "viewSet -s %camera";
	setAttr ".o" yes;
	setAttr -k on ".ai_translator" -type "string" "orthographic";
createNode transform -n "reference:pCylinder1";
createNode mesh -n "reference:pCylinderShape1" -p "reference:pCylinder1";
	addAttr -ci true -k true -sn "ai_user_options" -ln "aiUserOptions" -dt "string";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-min 0 -smx 1 -at "float";
	addAttr -ci true -sn "ai_self_shadows" -ln "aiSelfShadows" -dv 1 -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "ai_opaque" -ln "aiOpaque" -dv 1 -min 0 -max 1 -at "bool";
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
	setAttr -k off ".v";
	setAttr -s 4 ".iog[0].og";
	setAttr ".vir" yes;
	setAttr ".vif" yes;
	setAttr ".uvst[0].uvsn" -type "string" "map1";
	setAttr ".cuvs" -type "string" "map1";
	setAttr ".dcc" -type "string" "Ambient+Diffuse";
	setAttr ".covm[0]"  0 1 1;
	setAttr ".cdvm[0]"  0 1 1;
createNode mesh -n "reference:pCylinderShape1Orig" -p "reference:pCylinder1";
	addAttr -ci true -k true -sn "ai_user_options" -ln "aiUserOptions" -dt "string";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-min 0 -smx 1 -at "float";
	addAttr -ci true -sn "ai_self_shadows" -ln "aiSelfShadows" -dv 1 -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "ai_opaque" -ln "aiOpaque" -dv 1 -min 0 -max 1 -at "bool";
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
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".vir" yes;
	setAttr ".vif" yes;
	setAttr ".uvst[0].uvsn" -type "string" "map1";
	setAttr ".cuvs" -type "string" "map1";
	setAttr ".dcc" -type "string" "Ambient+Diffuse";
	setAttr ".covm[0]"  0 1 1;
	setAttr ".cdvm[0]"  0 1 1;
createNode transform -n "reference:bend1Handle";
	setAttr ".t" -type "double3" -1.1920928955078125e-007 -5 -1.7881393432617188e-007 ;
	setAttr ".s" -type "double3" 5 10 5 ;
	setAttr ".smd" 7;
createNode deformBend -n "reference:bend1HandleShape" -p "reference:bend1Handle";
	setAttr -k off ".v";
	setAttr ".dd" -type "doubleArray" 3 0 1 3.0843026920107346 ;
	setAttr ".hw" 1.1000003278255464;
createNode transform -n "reference:pCylinder1_reference";
	setAttr ".tmp" yes;
createNode mesh -n "reference:pCylinderShape1_reference" -p "reference:pCylinder1_reference";
	addAttr -ci true -k true -sn "ai_user_options" -ln "aiUserOptions" -dt "string";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-min 0 -smx 1 -at "float";
	addAttr -ci true -sn "ai_self_shadows" -ln "aiSelfShadows" -dv 1 -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "ai_opaque" -ln "aiOpaque" -dv 1 -min 0 -max 1 -at "bool";
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
	setAttr -k off ".v";
	setAttr ".vir" yes;
	setAttr ".vif" yes;
	setAttr ".uvst[0].uvsn" -type "string" "map1";
	setAttr -s 273 ".uvst[0].uvsp";
	setAttr ".uvst[0].uvsp[0:249]" -type "float2" 0.64860266 0.10796607 0.62640899 
		0.064408496 0.59184152 0.029841021 0.54828393 0.0076473355 0.5 -7.4505806e-008 0.45171607 
		0.0076473504 0.40815851 0.029841051 0.37359107 0.064408526 0.3513974 0.10796608 0.34374997 
		0.15625 0.3513974 0.20453392 0.37359107 0.24809146 0.40815854 0.28265893 0.4517161 
		0.3048526 0.5 0.3125 0.54828387 0.3048526 0.59184146 0.28265893 0.62640893 0.24809146 
		0.6486026 0.2045339 0.65625 0.15625 0.375 0.3125 0.38749999 0.3125 0.39999998 0.3125 
		0.41249996 0.3125 0.42499995 0.3125 0.43749994 0.3125 0.44999993 0.3125 0.46249992 
		0.3125 0.4749999 0.3125 0.48749989 0.3125 0.49999988 0.3125 0.51249987 0.3125 0.52499986 
		0.3125 0.53749985 0.3125 0.54999983 0.3125 0.56249982 0.3125 0.57499981 0.3125 0.5874998 
		0.3125 0.59999979 0.3125 0.61249977 0.3125 0.62499976 0.3125 0.375 0.35009399 0.38749999 
		0.35009399 0.39999998 0.35009399 0.41249996 0.35009399 0.42499995 0.35009399 0.43749994 
		0.35009399 0.44999993 0.35009399 0.46249992 0.35009399 0.4749999 0.35009399 0.48749989 
		0.35009399 0.49999988 0.35009399 0.51249987 0.35009399 0.52499986 0.35009399 0.53749985 
		0.35009399 0.54999983 0.35009399 0.56249982 0.35009399 0.57499981 0.35009399 0.5874998 
		0.35009399 0.59999979 0.35009399 0.61249977 0.35009399 0.62499976 0.35009399 0.375 
		0.38768798 0.38749999 0.38768798 0.39999998 0.38768798 0.41249996 0.38768798 0.42499995 
		0.38768798 0.43749994 0.38768798 0.44999993 0.38768798 0.46249992 0.38768798 0.4749999 
		0.38768798 0.48749989 0.38768798 0.49999988 0.38768798 0.51249987 0.38768798 0.52499986 
		0.38768798 0.53749985 0.38768798 0.54999983 0.38768798 0.56249982 0.38768798 0.57499981 
		0.38768798 0.5874998 0.38768798 0.59999979 0.38768798 0.61249977 0.38768798 0.62499976 
		0.38768798 0.375 0.42528197 0.38749999 0.42528197 0.39999998 0.42528197 0.41249996 
		0.42528197 0.42499995 0.42528197 0.43749994 0.42528197 0.44999993 0.42528197 0.46249992 
		0.42528197 0.4749999 0.42528197 0.48749989 0.42528197 0.49999988 0.42528197 0.51249987 
		0.42528197 0.52499986 0.42528197 0.53749985 0.42528197 0.54999983 0.42528197 0.56249982 
		0.42528197 0.57499981 0.42528197 0.5874998 0.42528197 0.59999979 0.42528197 0.61249977 
		0.42528197 0.62499976 0.42528197 0.375 0.46287596 0.38749999 0.46287596 0.39999998 
		0.46287596 0.41249996 0.46287596 0.42499995 0.46287596 0.43749994 0.46287596 0.44999993 
		0.46287596 0.46249992 0.46287596 0.4749999 0.46287596 0.48749989 0.46287596 0.49999988 
		0.46287596 0.51249987 0.46287596 0.52499986 0.46287596 0.53749985 0.46287596 0.54999983 
		0.46287596 0.56249982 0.46287596 0.57499981 0.46287596 0.5874998 0.46287596 0.59999979 
		0.46287596 0.61249977 0.46287596 0.62499976 0.46287596 0.375 0.50046992 0.38749999 
		0.50046992 0.39999998 0.50046992 0.41249996 0.50046992 0.42499995 0.50046992 0.43749994 
		0.50046992 0.44999993 0.50046992 0.46249992 0.50046992 0.4749999 0.50046992 0.48749989 
		0.50046992 0.49999988 0.50046992 0.51249987 0.50046992 0.52499986 0.50046992 0.53749985 
		0.50046992 0.54999983 0.50046992 0.56249982 0.50046992 0.57499981 0.50046992 0.5874998 
		0.50046992 0.59999979 0.50046992 0.61249977 0.50046992 0.62499976 0.50046992 0.375 
		0.53806388 0.38749999 0.53806388 0.39999998 0.53806388 0.41249996 0.53806388 0.42499995 
		0.53806388 0.43749994 0.53806388 0.44999993 0.53806388 0.46249992 0.53806388 0.4749999 
		0.53806388 0.48749989 0.53806388 0.49999988 0.53806388 0.51249987 0.53806388 0.52499986 
		0.53806388 0.53749985 0.53806388 0.54999983 0.53806388 0.56249982 0.53806388 0.57499981 
		0.53806388 0.5874998 0.53806388 0.59999979 0.53806388 0.61249977 0.53806388 0.62499976 
		0.53806388 0.375 0.57565784 0.38749999 0.57565784 0.39999998 0.57565784 0.41249996 
		0.57565784 0.42499995 0.57565784 0.43749994 0.57565784 0.44999993 0.57565784 0.46249992 
		0.57565784 0.4749999 0.57565784 0.48749989 0.57565784 0.49999988 0.57565784 0.51249987 
		0.57565784 0.52499986 0.57565784 0.53749985 0.57565784 0.54999983 0.57565784 0.56249982 
		0.57565784 0.57499981 0.57565784 0.5874998 0.57565784 0.59999979 0.57565784 0.61249977 
		0.57565784 0.62499976 0.57565784 0.375 0.61325181 0.38749999 0.61325181 0.39999998 
		0.61325181 0.41249996 0.61325181 0.42499995 0.61325181 0.43749994 0.61325181 0.44999993 
		0.61325181 0.46249992 0.61325181 0.4749999 0.61325181 0.48749989 0.61325181 0.49999988 
		0.61325181 0.51249987 0.61325181 0.52499986 0.61325181 0.53749985 0.61325181 0.54999983 
		0.61325181 0.56249982 0.61325181 0.57499981 0.61325181 0.5874998 0.61325181 0.59999979 
		0.61325181 0.61249977 0.61325181 0.62499976 0.61325181 0.375 0.65084577 0.38749999 
		0.65084577 0.39999998 0.65084577 0.41249996 0.65084577 0.42499995 0.65084577 0.43749994 
		0.65084577 0.44999993 0.65084577 0.46249992 0.65084577 0.4749999 0.65084577 0.48749989 
		0.65084577 0.49999988 0.65084577 0.51249987 0.65084577 0.52499986 0.65084577 0.53749985 
		0.65084577 0.54999983 0.65084577 0.56249982 0.65084577 0.57499981 0.65084577 0.5874998 
		0.65084577 0.59999979 0.65084577 0.61249977 0.65084577 0.62499976 0.65084577 0.375 
		0.68843973 0.38749999 0.68843973 0.39999998 0.68843973 0.41249996 0.68843973 0.42499995 
		0.68843973 0.43749994 0.68843973 0.44999993 0.68843973 0.46249992 0.68843973 0.4749999 
		0.68843973 0.48749989 0.68843973 0.49999988 0.68843973 0.51249987 0.68843973 0.52499986 
		0.68843973 0.53749985 0.68843973 0.54999983 0.68843973 0.56249982 0.68843973 0.57499981 
		0.68843973 0.5874998 0.68843973 0.59999979 0.68843973 0.61249977 0.68843973;
	setAttr ".uvst[0].uvsp[250:272]" 0.62499976 0.68843973 0.64860266 0.79546607 
		0.62640899 0.75190848 0.59184152 0.71734101 0.54828393 0.69514734 0.5 0.68749994 
		0.45171607 0.69514734 0.40815851 0.71734107 0.37359107 0.75190854 0.3513974 0.79546607 
		0.34374997 0.84375 0.3513974 0.89203393 0.37359107 0.93559146 0.40815854 0.97015893 
		0.4517161 0.9923526 0.5 1 0.54828387 0.9923526 0.59184146 0.97015893 0.62640893 0.93559146 
		0.6486026 0.89203393 0.65625 0.84375 0.5 0.15000001 0.5 0.83749998;
	setAttr ".cuvs" -type "string" "map1";
	setAttr ".dcc" -type "string" "Ambient+Diffuse";
	setAttr ".covm[0]"  0 1 1;
	setAttr ".cdvm[0]"  0 1 1;
	setAttr -s 222 ".vt";
	setAttr ".vt[0:165]"  0.95105714 -5 -0.30901718 0.80901754 -5 -0.5877856 
		0.5877856 -5 -0.80901748 0.30901715 -5 -0.95105702 0 -5 -1.0000005 -0.30901715 -5 
		-0.95105696 -0.58778548 -5 -0.8090173 -0.80901724 -5 -0.58778542 -0.95105678 -5 -0.30901706 
		-1.0000002 -5 2.646978e-023 -0.95105678 -5 0.30901706 -0.80901718 -5 0.58778536 -0.58778536 
		-5 0.80901712 -0.30901706 -5 0.95105666 -2.9802322e-008 -5 1.0000001 0.30901697 -5 
		0.9510566 0.58778524 -5 0.80901706 0.809017 -5 0.5877853 0.95105654 -5 0.309017 1 
		-5 2.646978e-023 0.95105714 -4 -0.30901718 0.80901754 -4 -0.5877856 0.5877856 -4 
		-0.80901748 0.30901715 -4 -0.95105702 0 -4 -1.0000005 -0.30901715 -4 -0.95105696 
		-0.58778548 -4 -0.8090173 -0.80901724 -4 -0.58778542 -0.95105678 -4 -0.30901706 -1.0000002 
		-4 2.646978e-023 -0.95105678 -4 0.30901706 -0.80901718 -4 0.58778536 -0.58778536 
		-4 0.80901712 -0.30901706 -4 0.95105666 -2.9802322e-008 -4 1.0000001 0.30901697 -4 
		0.9510566 0.58778524 -4 0.80901706 0.809017 -4 0.5877853 0.95105654 -4 0.309017 1 
		-4 2.646978e-023 0.95105714 -3 -0.30901718 0.80901754 -3 -0.5877856 0.5877856 -3 
		-0.80901748 0.30901715 -3 -0.95105702 0 -3 -1.0000005 -0.30901715 -3 -0.95105696 
		-0.58778548 -3 -0.8090173 -0.80901724 -3 -0.58778542 -0.95105678 -3 -0.30901706 -1.0000002 
		-3 2.646978e-023 -0.95105678 -3 0.30901706 -0.80901718 -3 0.58778536 -0.58778536 
		-3 0.80901712 -0.30901706 -3 0.95105666 -2.9802322e-008 -3 1.0000001 0.30901697 -3 
		0.9510566 0.58778524 -3 0.80901706 0.809017 -3 0.5877853 0.95105654 -3 0.309017 1 
		-3 2.646978e-023 0.95105714 -2 -0.30901718 0.80901754 -2 -0.5877856 0.5877856 -2 
		-0.80901748 0.30901715 -2 -0.95105702 0 -2 -1.0000005 -0.30901715 -2 -0.95105696 
		-0.58778548 -2 -0.8090173 -0.80901724 -2 -0.58778542 -0.95105678 -2 -0.30901706 -1.0000002 
		-2 2.646978e-023 -0.95105678 -2 0.30901706 -0.80901718 -2 0.58778536 -0.58778536 
		-2 0.80901712 -0.30901706 -2 0.95105666 -2.9802322e-008 -2 1.0000001 0.30901697 -2 
		0.9510566 0.58778524 -2 0.80901706 0.809017 -2 0.5877853 0.95105654 -2 0.309017 1 
		-2 2.646978e-023 0.95105714 -1 -0.30901718 0.80901754 -1 -0.5877856 0.5877856 -1 
		-0.80901748 0.30901715 -1 -0.95105702 0 -1 -1.0000005 -0.30901715 -1 -0.95105696 
		-0.58778548 -1 -0.8090173 -0.80901724 -1 -0.58778542 -0.95105678 -1 -0.30901706 -1.0000002 
		-1 2.646978e-023 -0.95105678 -1 0.30901706 -0.80901718 -1 0.58778536 -0.58778536 
		-1 0.80901712 -0.30901706 -1 0.95105666 -2.9802322e-008 -1 1.0000001 0.30901697 -1 
		0.9510566 0.58778524 -1 0.80901706 0.809017 -1 0.5877853 0.95105654 -1 0.309017 1 
		-1 2.646978e-023 0.95105714 0 -0.30901718 0.80901754 0 -0.5877856 0.5877856 0 -0.80901748 
		0.30901715 0 -0.95105702 0 0 -1.0000005 -0.30901715 0 -0.95105696 -0.58778548 0 -0.8090173 
		-0.80901724 0 -0.58778542 -0.95105678 0 -0.30901706 -1.0000002 0 2.646978e-023 -0.95105678 
		0 0.30901706 -0.80901718 0 0.58778536 -0.58778536 0 0.80901712 -0.30901706 0 0.95105666 
		-2.9802322e-008 0 1.0000001 0.30901697 0 0.9510566 0.58778524 0 0.80901706 0.809017 
		0 0.5877853 0.95105654 0 0.309017 1 0 2.646978e-023 0.95105714 1 -0.30901718 0.80901754 
		1 -0.5877856 0.5877856 1 -0.80901748 0.30901715 1 -0.95105702 0 1 -1.0000005 -0.30901715 
		1 -0.95105696 -0.58778548 1 -0.8090173 -0.80901724 1 -0.58778542 -0.95105678 1 -0.30901706 
		-1.0000002 1 2.646978e-023 -0.95105678 1 0.30901706 -0.80901718 1 0.58778536 -0.58778536 
		1 0.80901712 -0.30901706 1 0.95105666 -2.9802322e-008 1 1.0000001 0.30901697 1 0.9510566 
		0.58778524 1 0.80901706 0.809017 1 0.5877853 0.95105654 1 0.309017 1 1 2.646978e-023 
		0.95105714 2 -0.30901718 0.80901754 2 -0.5877856 0.5877856 2 -0.80901748 0.30901715 
		2 -0.95105702 0 2 -1.0000005 -0.30901715 2 -0.95105696 -0.58778548 2 -0.8090173 -0.80901724 
		2 -0.58778542 -0.95105678 2 -0.30901706 -1.0000002 2 2.646978e-023 -0.95105678 2 
		0.30901706 -0.80901718 2 0.58778536 -0.58778536 2 0.80901712 -0.30901706 2 0.95105666 
		-2.9802322e-008 2 1.0000001 0.30901697 2 0.9510566 0.58778524 2 0.80901706 0.809017 
		2 0.5877853 0.95105654 2 0.309017 1 2 2.646978e-023 0.95105714 3 -0.30901718 0.80901754 
		3 -0.5877856 0.5877856 3 -0.80901748 0.30901715 3 -0.95105702 0 3 -1.0000005 -0.30901715 
		3 -0.95105696;
	setAttr ".vt[166:221]" -0.58778548 3 -0.8090173 -0.80901724 3 -0.58778542 
		-0.95105678 3 -0.30901706 -1.0000002 3 2.646978e-023 -0.95105678 3 0.30901706 -0.80901718 
		3 0.58778536 -0.58778536 3 0.80901712 -0.30901706 3 0.95105666 -2.9802322e-008 3 
		1.0000001 0.30901697 3 0.9510566 0.58778524 3 0.80901706 0.809017 3 0.5877853 0.95105654 
		3 0.309017 1 3 2.646978e-023 0.95105714 4 -0.30901718 0.80901754 4 -0.5877856 0.5877856 
		4 -0.80901748 0.30901715 4 -0.95105702 0 4 -1.0000005 -0.30901715 4 -0.95105696 -0.58778548 
		4 -0.8090173 -0.80901724 4 -0.58778542 -0.95105678 4 -0.30901706 -1.0000002 4 2.646978e-023 
		-0.95105678 4 0.30901706 -0.80901718 4 0.58778536 -0.58778536 4 0.80901712 -0.30901706 
		4 0.95105666 -2.9802322e-008 4 1.0000001 0.30901697 4 0.9510566 0.58778524 4 0.80901706 
		0.809017 4 0.5877853 0.95105654 4 0.309017 1 4 2.646978e-023 0.95105714 5 -0.30901718 
		0.80901754 5 -0.5877856 0.5877856 5 -0.80901748 0.30901715 5 -0.95105702 0 5 -1.0000005 
		-0.30901715 5 -0.95105696 -0.58778548 5 -0.8090173 -0.80901724 5 -0.58778542 -0.95105678 
		5 -0.30901706 -1.0000002 5 2.646978e-023 -0.95105678 5 0.30901706 -0.80901718 5 0.58778536 
		-0.58778536 5 0.80901712 -0.30901706 5 0.95105666 -2.9802322e-008 5 1.0000001 0.30901697 
		5 0.9510566 0.58778524 5 0.80901706 0.809017 5 0.5877853 0.95105654 5 0.309017 1 
		5 2.646978e-023 0 -5 2.646978e-023 0 5 2.646978e-023;
	setAttr -s 460 ".ed";
	setAttr ".ed[0:165]"  0 1 0 1 2 0 2 3 
		0 3 4 0 4 5 0 5 6 0 6 7 
		0 7 8 0 8 9 0 9 10 0 10 11 
		0 11 12 0 12 13 0 13 14 0 14 15 
		0 15 16 0 16 17 0 17 18 0 18 19 
		0 19 0 0 20 21 1 21 22 1 22 23 
		1 23 24 1 24 25 1 25 26 1 26 27 
		1 27 28 1 28 29 1 29 30 1 30 31 
		1 31 32 1 32 33 1 33 34 1 34 35 
		1 35 36 1 36 37 1 37 38 1 38 39 
		1 39 20 1 40 41 1 41 42 1 42 43 
		1 43 44 1 44 45 1 45 46 1 46 47 
		1 47 48 1 48 49 1 49 50 1 50 51 
		1 51 52 1 52 53 1 53 54 1 54 55 
		1 55 56 1 56 57 1 57 58 1 58 59 
		1 59 40 1 60 61 1 61 62 1 62 63 
		1 63 64 1 64 65 1 65 66 1 66 67 
		1 67 68 1 68 69 1 69 70 1 70 71 
		1 71 72 1 72 73 1 73 74 1 74 75 
		1 75 76 1 76 77 1 77 78 1 78 79 
		1 79 60 1 80 81 1 81 82 1 82 83 
		1 83 84 1 84 85 1 85 86 1 86 87 
		1 87 88 1 88 89 1 89 90 1 90 91 
		1 91 92 1 92 93 1 93 94 1 94 95 
		1 95 96 1 96 97 1 97 98 1 98 99 
		1 99 80 1 100 101 1 101 102 1 102 103 
		1 103 104 1 104 105 1 105 106 1 106 107 
		1 107 108 1 108 109 1 109 110 1 110 111 
		1 111 112 1 112 113 1 113 114 1 114 115 
		1 115 116 1 116 117 1 117 118 1 118 119 
		1 119 100 1 120 121 1 121 122 1 122 123 
		1 123 124 1 124 125 1 125 126 1 126 127 
		1 127 128 1 128 129 1 129 130 1 130 131 
		1 131 132 1 132 133 1 133 134 1 134 135 
		1 135 136 1 136 137 1 137 138 1 138 139 
		1 139 120 1 140 141 1 141 142 1 142 143 
		1 143 144 1 144 145 1 145 146 1 146 147 
		1 147 148 1 148 149 1 149 150 1 150 151 
		1 151 152 1 152 153 1 153 154 1 154 155 
		1 155 156 1 156 157 1 157 158 1 158 159 
		1 159 140 1 160 161 1 161 162 1 162 163 
		1 163 164 1 164 165 1 165 166 1;
	setAttr ".ed[166:331]" 166 167 1 167 168 1 168 169 
		1 169 170 1 170 171 1 171 172 1 172 173 
		1 173 174 1 174 175 1 175 176 1 176 177 
		1 177 178 1 178 179 1 179 160 1 180 181 
		1 181 182 1 182 183 1 183 184 1 184 185 
		1 185 186 1 186 187 1 187 188 1 188 189 
		1 189 190 1 190 191 1 191 192 1 192 193 
		1 193 194 1 194 195 1 195 196 1 196 197 
		1 197 198 1 198 199 1 199 180 1 200 201 
		0 201 202 0 202 203 0 203 204 0 204 205 
		0 205 206 0 206 207 0 207 208 0 208 209 
		0 209 210 0 210 211 0 211 212 0 212 213 
		0 213 214 0 214 215 0 215 216 0 216 217 
		0 217 218 0 218 219 0 219 200 0 0 20 
		1 1 21 1 2 22 1 3 23 1 4 24 
		1 5 25 1 6 26 1 7 27 1 8 28 
		1 9 29 1 10 30 1 11 31 1 12 32 
		1 13 33 1 14 34 1 15 35 1 16 36 
		1 17 37 1 18 38 1 19 39 1 20 40 
		1 21 41 1 22 42 1 23 43 1 24 44 
		1 25 45 1 26 46 1 27 47 1 28 48 
		1 29 49 1 30 50 1 31 51 1 32 52 
		1 33 53 1 34 54 1 35 55 1 36 56 
		1 37 57 1 38 58 1 39 59 1 40 60 
		1 41 61 1 42 62 1 43 63 1 44 64 
		1 45 65 1 46 66 1 47 67 1 48 68 
		1 49 69 1 50 70 1 51 71 1 52 72 
		1 53 73 1 54 74 1 55 75 1 56 76 
		1 57 77 1 58 78 1 59 79 1 60 80 
		1 61 81 1 62 82 1 63 83 1 64 84 
		1 65 85 1 66 86 1 67 87 1 68 88 
		1 69 89 1 70 90 1 71 91 1 72 92 
		1 73 93 1 74 94 1 75 95 1 76 96 
		1 77 97 1 78 98 1 79 99 1 80 100 
		1 81 101 1 82 102 1 83 103 1 84 104 
		1 85 105 1 86 106 1 87 107 1 88 108 
		1 89 109 1 90 110 1 91 111 1 92 112 
		1 93 113 1 94 114 1 95 115 1 96 116 
		1 97 117 1 98 118 1 99 119 1 100 120 
		1 101 121 1 102 122 1 103 123 1 104 124 
		1 105 125 1 106 126 1 107 127 1 108 128 
		1 109 129 1 110 130 1 111 131 1;
	setAttr ".ed[332:459]" 112 132 1 113 133 1 114 134 
		1 115 135 1 116 136 1 117 137 1 118 138 
		1 119 139 1 120 140 1 121 141 1 122 142 
		1 123 143 1 124 144 1 125 145 1 126 146 
		1 127 147 1 128 148 1 129 149 1 130 150 
		1 131 151 1 132 152 1 133 153 1 134 154 
		1 135 155 1 136 156 1 137 157 1 138 158 
		1 139 159 1 140 160 1 141 161 1 142 162 
		1 143 163 1 144 164 1 145 165 1 146 166 
		1 147 167 1 148 168 1 149 169 1 150 170 
		1 151 171 1 152 172 1 153 173 1 154 174 
		1 155 175 1 156 176 1 157 177 1 158 178 
		1 159 179 1 160 180 1 161 181 1 162 182 
		1 163 183 1 164 184 1 165 185 1 166 186 
		1 167 187 1 168 188 1 169 189 1 170 190 
		1 171 191 1 172 192 1 173 193 1 174 194 
		1 175 195 1 176 196 1 177 197 1 178 198 
		1 179 199 1 180 200 1 181 201 1 182 202 
		1 183 203 1 184 204 1 185 205 1 186 206 
		1 187 207 1 188 208 1 189 209 1 190 210 
		1 191 211 1 192 212 1 193 213 1 194 214 
		1 195 215 1 196 216 1 197 217 1 198 218 
		1 199 219 1 220 0 1 220 1 1 220 2 
		1 220 3 1 220 4 1 220 5 1 220 6 
		1 220 7 1 220 8 1 220 9 1 220 10 
		1 220 11 1 220 12 1 220 13 1 220 14 
		1 220 15 1 220 16 1 220 17 1 220 18 
		1 220 19 1 200 221 1 201 221 1 202 221 
		1 203 221 1 204 221 1 205 221 1 206 221 
		1 207 221 1 208 221 1 209 221 1 210 221 
		1 211 221 1 212 221 1 213 221 1 214 221 
		1 215 221 1 216 221 1 217 221 1 218 221 
		1 219 221 1;
	setAttr -s 240 ".fc[0:239]" -type "polyFaces" 
		f 4 0 221 -21 -221 
		mu 0 4 20 21 42 41 
		f 4 1 222 -22 -222 
		mu 0 4 21 22 43 42 
		f 4 2 223 -23 -223 
		mu 0 4 22 23 44 43 
		f 4 3 224 -24 -224 
		mu 0 4 23 24 45 44 
		f 4 4 225 -25 -225 
		mu 0 4 24 25 46 45 
		f 4 5 226 -26 -226 
		mu 0 4 25 26 47 46 
		f 4 6 227 -27 -227 
		mu 0 4 26 27 48 47 
		f 4 7 228 -28 -228 
		mu 0 4 27 28 49 48 
		f 4 8 229 -29 -229 
		mu 0 4 28 29 50 49 
		f 4 9 230 -30 -230 
		mu 0 4 29 30 51 50 
		f 4 10 231 -31 -231 
		mu 0 4 30 31 52 51 
		f 4 11 232 -32 -232 
		mu 0 4 31 32 53 52 
		f 4 12 233 -33 -233 
		mu 0 4 32 33 54 53 
		f 4 13 234 -34 -234 
		mu 0 4 33 34 55 54 
		f 4 14 235 -35 -235 
		mu 0 4 34 35 56 55 
		f 4 15 236 -36 -236 
		mu 0 4 35 36 57 56 
		f 4 16 237 -37 -237 
		mu 0 4 36 37 58 57 
		f 4 17 238 -38 -238 
		mu 0 4 37 38 59 58 
		f 4 18 239 -39 -239 
		mu 0 4 38 39 60 59 
		f 4 19 220 -40 -240 
		mu 0 4 39 40 61 60 
		f 4 20 241 -41 -241 
		mu 0 4 41 42 63 62 
		f 4 21 242 -42 -242 
		mu 0 4 42 43 64 63 
		f 4 22 243 -43 -243 
		mu 0 4 43 44 65 64 
		f 4 23 244 -44 -244 
		mu 0 4 44 45 66 65 
		f 4 24 245 -45 -245 
		mu 0 4 45 46 67 66 
		f 4 25 246 -46 -246 
		mu 0 4 46 47 68 67 
		f 4 26 247 -47 -247 
		mu 0 4 47 48 69 68 
		f 4 27 248 -48 -248 
		mu 0 4 48 49 70 69 
		f 4 28 249 -49 -249 
		mu 0 4 49 50 71 70 
		f 4 29 250 -50 -250 
		mu 0 4 50 51 72 71 
		f 4 30 251 -51 -251 
		mu 0 4 51 52 73 72 
		f 4 31 252 -52 -252 
		mu 0 4 52 53 74 73 
		f 4 32 253 -53 -253 
		mu 0 4 53 54 75 74 
		f 4 33 254 -54 -254 
		mu 0 4 54 55 76 75 
		f 4 34 255 -55 -255 
		mu 0 4 55 56 77 76 
		f 4 35 256 -56 -256 
		mu 0 4 56 57 78 77 
		f 4 36 257 -57 -257 
		mu 0 4 57 58 79 78 
		f 4 37 258 -58 -258 
		mu 0 4 58 59 80 79 
		f 4 38 259 -59 -259 
		mu 0 4 59 60 81 80 
		f 4 39 240 -60 -260 
		mu 0 4 60 61 82 81 
		f 4 40 261 -61 -261 
		mu 0 4 62 63 84 83 
		f 4 41 262 -62 -262 
		mu 0 4 63 64 85 84 
		f 4 42 263 -63 -263 
		mu 0 4 64 65 86 85 
		f 4 43 264 -64 -264 
		mu 0 4 65 66 87 86 
		f 4 44 265 -65 -265 
		mu 0 4 66 67 88 87 
		f 4 45 266 -66 -266 
		mu 0 4 67 68 89 88 
		f 4 46 267 -67 -267 
		mu 0 4 68 69 90 89 
		f 4 47 268 -68 -268 
		mu 0 4 69 70 91 90 
		f 4 48 269 -69 -269 
		mu 0 4 70 71 92 91 
		f 4 49 270 -70 -270 
		mu 0 4 71 72 93 92 
		f 4 50 271 -71 -271 
		mu 0 4 72 73 94 93 
		f 4 51 272 -72 -272 
		mu 0 4 73 74 95 94 
		f 4 52 273 -73 -273 
		mu 0 4 74 75 96 95 
		f 4 53 274 -74 -274 
		mu 0 4 75 76 97 96 
		f 4 54 275 -75 -275 
		mu 0 4 76 77 98 97 
		f 4 55 276 -76 -276 
		mu 0 4 77 78 99 98 
		f 4 56 277 -77 -277 
		mu 0 4 78 79 100 99 
		f 4 57 278 -78 -278 
		mu 0 4 79 80 101 100 
		f 4 58 279 -79 -279 
		mu 0 4 80 81 102 101 
		f 4 59 260 -80 -280 
		mu 0 4 81 82 103 102 
		f 4 60 281 -81 -281 
		mu 0 4 83 84 105 104 
		f 4 61 282 -82 -282 
		mu 0 4 84 85 106 105 
		f 4 62 283 -83 -283 
		mu 0 4 85 86 107 106 
		f 4 63 284 -84 -284 
		mu 0 4 86 87 108 107 
		f 4 64 285 -85 -285 
		mu 0 4 87 88 109 108 
		f 4 65 286 -86 -286 
		mu 0 4 88 89 110 109 
		f 4 66 287 -87 -287 
		mu 0 4 89 90 111 110 
		f 4 67 288 -88 -288 
		mu 0 4 90 91 112 111 
		f 4 68 289 -89 -289 
		mu 0 4 91 92 113 112 
		f 4 69 290 -90 -290 
		mu 0 4 92 93 114 113 
		f 4 70 291 -91 -291 
		mu 0 4 93 94 115 114 
		f 4 71 292 -92 -292 
		mu 0 4 94 95 116 115 
		f 4 72 293 -93 -293 
		mu 0 4 95 96 117 116 
		f 4 73 294 -94 -294 
		mu 0 4 96 97 118 117 
		f 4 74 295 -95 -295 
		mu 0 4 97 98 119 118 
		f 4 75 296 -96 -296 
		mu 0 4 98 99 120 119 
		f 4 76 297 -97 -297 
		mu 0 4 99 100 121 120 
		f 4 77 298 -98 -298 
		mu 0 4 100 101 122 121 
		f 4 78 299 -99 -299 
		mu 0 4 101 102 123 122 
		f 4 79 280 -100 -300 
		mu 0 4 102 103 124 123 
		f 4 80 301 -101 -301 
		mu 0 4 104 105 126 125 
		f 4 81 302 -102 -302 
		mu 0 4 105 106 127 126 
		f 4 82 303 -103 -303 
		mu 0 4 106 107 128 127 
		f 4 83 304 -104 -304 
		mu 0 4 107 108 129 128 
		f 4 84 305 -105 -305 
		mu 0 4 108 109 130 129 
		f 4 85 306 -106 -306 
		mu 0 4 109 110 131 130 
		f 4 86 307 -107 -307 
		mu 0 4 110 111 132 131 
		f 4 87 308 -108 -308 
		mu 0 4 111 112 133 132 
		f 4 88 309 -109 -309 
		mu 0 4 112 113 134 133 
		f 4 89 310 -110 -310 
		mu 0 4 113 114 135 134 
		f 4 90 311 -111 -311 
		mu 0 4 114 115 136 135 
		f 4 91 312 -112 -312 
		mu 0 4 115 116 137 136 
		f 4 92 313 -113 -313 
		mu 0 4 116 117 138 137 
		f 4 93 314 -114 -314 
		mu 0 4 117 118 139 138 
		f 4 94 315 -115 -315 
		mu 0 4 118 119 140 139 
		f 4 95 316 -116 -316 
		mu 0 4 119 120 141 140 
		f 4 96 317 -117 -317 
		mu 0 4 120 121 142 141 
		f 4 97 318 -118 -318 
		mu 0 4 121 122 143 142 
		f 4 98 319 -119 -319 
		mu 0 4 122 123 144 143 
		f 4 99 300 -120 -320 
		mu 0 4 123 124 145 144 
		f 4 100 321 -121 -321 
		mu 0 4 125 126 147 146 
		f 4 101 322 -122 -322 
		mu 0 4 126 127 148 147 
		f 4 102 323 -123 -323 
		mu 0 4 127 128 149 148 
		f 4 103 324 -124 -324 
		mu 0 4 128 129 150 149 
		f 4 104 325 -125 -325 
		mu 0 4 129 130 151 150 
		f 4 105 326 -126 -326 
		mu 0 4 130 131 152 151 
		f 4 106 327 -127 -327 
		mu 0 4 131 132 153 152 
		f 4 107 328 -128 -328 
		mu 0 4 132 133 154 153 
		f 4 108 329 -129 -329 
		mu 0 4 133 134 155 154 
		f 4 109 330 -130 -330 
		mu 0 4 134 135 156 155 
		f 4 110 331 -131 -331 
		mu 0 4 135 136 157 156 
		f 4 111 332 -132 -332 
		mu 0 4 136 137 158 157 
		f 4 112 333 -133 -333 
		mu 0 4 137 138 159 158 
		f 4 113 334 -134 -334 
		mu 0 4 138 139 160 159 
		f 4 114 335 -135 -335 
		mu 0 4 139 140 161 160 
		f 4 115 336 -136 -336 
		mu 0 4 140 141 162 161 
		f 4 116 337 -137 -337 
		mu 0 4 141 142 163 162 
		f 4 117 338 -138 -338 
		mu 0 4 142 143 164 163 
		f 4 118 339 -139 -339 
		mu 0 4 143 144 165 164 
		f 4 119 320 -140 -340 
		mu 0 4 144 145 166 165 
		f 4 120 341 -141 -341 
		mu 0 4 146 147 168 167 
		f 4 121 342 -142 -342 
		mu 0 4 147 148 169 168 
		f 4 122 343 -143 -343 
		mu 0 4 148 149 170 169 
		f 4 123 344 -144 -344 
		mu 0 4 149 150 171 170 
		f 4 124 345 -145 -345 
		mu 0 4 150 151 172 171 
		f 4 125 346 -146 -346 
		mu 0 4 151 152 173 172 
		f 4 126 347 -147 -347 
		mu 0 4 152 153 174 173 
		f 4 127 348 -148 -348 
		mu 0 4 153 154 175 174 
		f 4 128 349 -149 -349 
		mu 0 4 154 155 176 175 
		f 4 129 350 -150 -350 
		mu 0 4 155 156 177 176 
		f 4 130 351 -151 -351 
		mu 0 4 156 157 178 177 
		f 4 131 352 -152 -352 
		mu 0 4 157 158 179 178 
		f 4 132 353 -153 -353 
		mu 0 4 158 159 180 179 
		f 4 133 354 -154 -354 
		mu 0 4 159 160 181 180 
		f 4 134 355 -155 -355 
		mu 0 4 160 161 182 181 
		f 4 135 356 -156 -356 
		mu 0 4 161 162 183 182 
		f 4 136 357 -157 -357 
		mu 0 4 162 163 184 183 
		f 4 137 358 -158 -358 
		mu 0 4 163 164 185 184 
		f 4 138 359 -159 -359 
		mu 0 4 164 165 186 185 
		f 4 139 340 -160 -360 
		mu 0 4 165 166 187 186 
		f 4 140 361 -161 -361 
		mu 0 4 167 168 189 188 
		f 4 141 362 -162 -362 
		mu 0 4 168 169 190 189 
		f 4 142 363 -163 -363 
		mu 0 4 169 170 191 190 
		f 4 143 364 -164 -364 
		mu 0 4 170 171 192 191 
		f 4 144 365 -165 -365 
		mu 0 4 171 172 193 192 
		f 4 145 366 -166 -366 
		mu 0 4 172 173 194 193 
		f 4 146 367 -167 -367 
		mu 0 4 173 174 195 194 
		f 4 147 368 -168 -368 
		mu 0 4 174 175 196 195 
		f 4 148 369 -169 -369 
		mu 0 4 175 176 197 196 
		f 4 149 370 -170 -370 
		mu 0 4 176 177 198 197 
		f 4 150 371 -171 -371 
		mu 0 4 177 178 199 198 
		f 4 151 372 -172 -372 
		mu 0 4 178 179 200 199 
		f 4 152 373 -173 -373 
		mu 0 4 179 180 201 200 
		f 4 153 374 -174 -374 
		mu 0 4 180 181 202 201 
		f 4 154 375 -175 -375 
		mu 0 4 181 182 203 202 
		f 4 155 376 -176 -376 
		mu 0 4 182 183 204 203 
		f 4 156 377 -177 -377 
		mu 0 4 183 184 205 204 
		f 4 157 378 -178 -378 
		mu 0 4 184 185 206 205 
		f 4 158 379 -179 -379 
		mu 0 4 185 186 207 206 
		f 4 159 360 -180 -380 
		mu 0 4 186 187 208 207 
		f 4 160 381 -181 -381 
		mu 0 4 188 189 210 209 
		f 4 161 382 -182 -382 
		mu 0 4 189 190 211 210 
		f 4 162 383 -183 -383 
		mu 0 4 190 191 212 211 
		f 4 163 384 -184 -384 
		mu 0 4 191 192 213 212 
		f 4 164 385 -185 -385 
		mu 0 4 192 193 214 213 
		f 4 165 386 -186 -386 
		mu 0 4 193 194 215 214 
		f 4 166 387 -187 -387 
		mu 0 4 194 195 216 215 
		f 4 167 388 -188 -388 
		mu 0 4 195 196 217 216 
		f 4 168 389 -189 -389 
		mu 0 4 196 197 218 217 
		f 4 169 390 -190 -390 
		mu 0 4 197 198 219 218 
		f 4 170 391 -191 -391 
		mu 0 4 198 199 220 219 
		f 4 171 392 -192 -392 
		mu 0 4 199 200 221 220 
		f 4 172 393 -193 -393 
		mu 0 4 200 201 222 221 
		f 4 173 394 -194 -394 
		mu 0 4 201 202 223 222 
		f 4 174 395 -195 -395 
		mu 0 4 202 203 224 223 
		f 4 175 396 -196 -396 
		mu 0 4 203 204 225 224 
		f 4 176 397 -197 -397 
		mu 0 4 204 205 226 225 
		f 4 177 398 -198 -398 
		mu 0 4 205 206 227 226 
		f 4 178 399 -199 -399 
		mu 0 4 206 207 228 227 
		f 4 179 380 -200 -400 
		mu 0 4 207 208 229 228 
		f 4 180 401 -201 -401 
		mu 0 4 209 210 231 230 
		f 4 181 402 -202 -402 
		mu 0 4 210 211 232 231 
		f 4 182 403 -203 -403 
		mu 0 4 211 212 233 232 
		f 4 183 404 -204 -404 
		mu 0 4 212 213 234 233 
		f 4 184 405 -205 -405 
		mu 0 4 213 214 235 234 
		f 4 185 406 -206 -406 
		mu 0 4 214 215 236 235 
		f 4 186 407 -207 -407 
		mu 0 4 215 216 237 236 
		f 4 187 408 -208 -408 
		mu 0 4 216 217 238 237 
		f 4 188 409 -209 -409 
		mu 0 4 217 218 239 238 
		f 4 189 410 -210 -410 
		mu 0 4 218 219 240 239 
		f 4 190 411 -211 -411 
		mu 0 4 219 220 241 240 
		f 4 191 412 -212 -412 
		mu 0 4 220 221 242 241 
		f 4 192 413 -213 -413 
		mu 0 4 221 222 243 242 
		f 4 193 414 -214 -414 
		mu 0 4 222 223 244 243 
		f 4 194 415 -215 -415 
		mu 0 4 223 224 245 244 
		f 4 195 416 -216 -416 
		mu 0 4 224 225 246 245 
		f 4 196 417 -217 -417 
		mu 0 4 225 226 247 246 
		f 4 197 418 -218 -418 
		mu 0 4 226 227 248 247 
		f 4 198 419 -219 -419 
		mu 0 4 227 228 249 248 
		f 4 199 400 -220 -420 
		mu 0 4 228 229 250 249 
		f 3 -1 -421 421 
		mu 0 3 1 0 271 
		f 3 -2 -422 422 
		mu 0 3 2 1 271 
		f 3 -3 -423 423 
		mu 0 3 3 2 271 
		f 3 -4 -424 424 
		mu 0 3 4 3 271 
		f 3 -5 -425 425 
		mu 0 3 5 4 271 
		f 3 -6 -426 426 
		mu 0 3 6 5 271 
		f 3 -7 -427 427 
		mu 0 3 7 6 271 
		f 3 -8 -428 428 
		mu 0 3 8 7 271 
		f 3 -9 -429 429 
		mu 0 3 9 8 271 
		f 3 -10 -430 430 
		mu 0 3 10 9 271 
		f 3 -11 -431 431 
		mu 0 3 11 10 271 
		f 3 -12 -432 432 
		mu 0 3 12 11 271 
		f 3 -13 -433 433 
		mu 0 3 13 12 271 
		f 3 -14 -434 434 
		mu 0 3 14 13 271 
		f 3 -15 -435 435 
		mu 0 3 15 14 271 
		f 3 -16 -436 436 
		mu 0 3 16 15 271 
		f 3 -17 -437 437 
		mu 0 3 17 16 271 
		f 3 -18 -438 438 
		mu 0 3 18 17 271 
		f 3 -19 -439 439 
		mu 0 3 19 18 271 
		f 3 -20 -440 420 
		mu 0 3 0 19 271 
		f 3 200 441 -441 
		mu 0 3 269 268 272 
		f 3 201 442 -442 
		mu 0 3 268 267 272 
		f 3 202 443 -443 
		mu 0 3 267 266 272 
		f 3 203 444 -444 
		mu 0 3 266 265 272 
		f 3 204 445 -445 
		mu 0 3 265 264 272 
		f 3 205 446 -446 
		mu 0 3 264 263 272 
		f 3 206 447 -447 
		mu 0 3 263 262 272 
		f 3 207 448 -448 
		mu 0 3 262 261 272 
		f 3 208 449 -449 
		mu 0 3 261 260 272 
		f 3 209 450 -450 
		mu 0 3 260 259 272 
		f 3 210 451 -451 
		mu 0 3 259 258 272 
		f 3 211 452 -452 
		mu 0 3 258 257 272 
		f 3 212 453 -453 
		mu 0 3 257 256 272 
		f 3 213 454 -454 
		mu 0 3 256 255 272 
		f 3 214 455 -455 
		mu 0 3 255 254 272 
		f 3 215 456 -456 
		mu 0 3 254 253 272 
		f 3 216 457 -457 
		mu 0 3 253 252 272 
		f 3 217 458 -458 
		mu 0 3 252 251 272 
		f 3 218 459 -459 
		mu 0 3 251 270 272 
		f 3 219 440 -460 
		mu 0 3 270 269 272 ;
	setAttr ".cd" -type "dataPolyComponent" Index_Data Edge 0 ;
	setAttr ".cvd" -type "dataPolyComponent" Index_Data Vertex 0 ;
	setAttr ".hfd" -type "dataPolyComponent" Index_Data Face 0 ;
createNode mesh -n "reference:pCylinder1_referenceShape1Orig" -p "reference:pCylinder1_reference";
	addAttr -ci true -k true -sn "ai_user_options" -ln "aiUserOptions" -dt "string";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-min 0 -smx 1 -at "float";
	addAttr -ci true -sn "ai_self_shadows" -ln "aiSelfShadows" -dv 1 -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "ai_opaque" -ln "aiOpaque" -dv 1 -min 0 -max 1 -at "bool";
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
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".vir" yes;
	setAttr ".vif" yes;
	setAttr ".uvst[0].uvsn" -type "string" "map1";
	setAttr -s 273 ".uvst[0].uvsp";
	setAttr ".uvst[0].uvsp[0:249]" -type "float2" 0.64860266 0.10796607 0.62640899 
		0.064408496 0.59184152 0.029841021 0.54828393 0.0076473355 0.5 -7.4505806e-008 0.45171607 
		0.0076473504 0.40815851 0.029841051 0.37359107 0.064408526 0.3513974 0.10796608 0.34374997 
		0.15625 0.3513974 0.20453392 0.37359107 0.24809146 0.40815854 0.28265893 0.4517161 
		0.3048526 0.5 0.3125 0.54828387 0.3048526 0.59184146 0.28265893 0.62640893 0.24809146 
		0.6486026 0.2045339 0.65625 0.15625 0.375 0.3125 0.38749999 0.3125 0.39999998 0.3125 
		0.41249996 0.3125 0.42499995 0.3125 0.43749994 0.3125 0.44999993 0.3125 0.46249992 
		0.3125 0.4749999 0.3125 0.48749989 0.3125 0.49999988 0.3125 0.51249987 0.3125 0.52499986 
		0.3125 0.53749985 0.3125 0.54999983 0.3125 0.56249982 0.3125 0.57499981 0.3125 0.5874998 
		0.3125 0.59999979 0.3125 0.61249977 0.3125 0.62499976 0.3125 0.375 0.35009399 0.38749999 
		0.35009399 0.39999998 0.35009399 0.41249996 0.35009399 0.42499995 0.35009399 0.43749994 
		0.35009399 0.44999993 0.35009399 0.46249992 0.35009399 0.4749999 0.35009399 0.48749989 
		0.35009399 0.49999988 0.35009399 0.51249987 0.35009399 0.52499986 0.35009399 0.53749985 
		0.35009399 0.54999983 0.35009399 0.56249982 0.35009399 0.57499981 0.35009399 0.5874998 
		0.35009399 0.59999979 0.35009399 0.61249977 0.35009399 0.62499976 0.35009399 0.375 
		0.38768798 0.38749999 0.38768798 0.39999998 0.38768798 0.41249996 0.38768798 0.42499995 
		0.38768798 0.43749994 0.38768798 0.44999993 0.38768798 0.46249992 0.38768798 0.4749999 
		0.38768798 0.48749989 0.38768798 0.49999988 0.38768798 0.51249987 0.38768798 0.52499986 
		0.38768798 0.53749985 0.38768798 0.54999983 0.38768798 0.56249982 0.38768798 0.57499981 
		0.38768798 0.5874998 0.38768798 0.59999979 0.38768798 0.61249977 0.38768798 0.62499976 
		0.38768798 0.375 0.42528197 0.38749999 0.42528197 0.39999998 0.42528197 0.41249996 
		0.42528197 0.42499995 0.42528197 0.43749994 0.42528197 0.44999993 0.42528197 0.46249992 
		0.42528197 0.4749999 0.42528197 0.48749989 0.42528197 0.49999988 0.42528197 0.51249987 
		0.42528197 0.52499986 0.42528197 0.53749985 0.42528197 0.54999983 0.42528197 0.56249982 
		0.42528197 0.57499981 0.42528197 0.5874998 0.42528197 0.59999979 0.42528197 0.61249977 
		0.42528197 0.62499976 0.42528197 0.375 0.46287596 0.38749999 0.46287596 0.39999998 
		0.46287596 0.41249996 0.46287596 0.42499995 0.46287596 0.43749994 0.46287596 0.44999993 
		0.46287596 0.46249992 0.46287596 0.4749999 0.46287596 0.48749989 0.46287596 0.49999988 
		0.46287596 0.51249987 0.46287596 0.52499986 0.46287596 0.53749985 0.46287596 0.54999983 
		0.46287596 0.56249982 0.46287596 0.57499981 0.46287596 0.5874998 0.46287596 0.59999979 
		0.46287596 0.61249977 0.46287596 0.62499976 0.46287596 0.375 0.50046992 0.38749999 
		0.50046992 0.39999998 0.50046992 0.41249996 0.50046992 0.42499995 0.50046992 0.43749994 
		0.50046992 0.44999993 0.50046992 0.46249992 0.50046992 0.4749999 0.50046992 0.48749989 
		0.50046992 0.49999988 0.50046992 0.51249987 0.50046992 0.52499986 0.50046992 0.53749985 
		0.50046992 0.54999983 0.50046992 0.56249982 0.50046992 0.57499981 0.50046992 0.5874998 
		0.50046992 0.59999979 0.50046992 0.61249977 0.50046992 0.62499976 0.50046992 0.375 
		0.53806388 0.38749999 0.53806388 0.39999998 0.53806388 0.41249996 0.53806388 0.42499995 
		0.53806388 0.43749994 0.53806388 0.44999993 0.53806388 0.46249992 0.53806388 0.4749999 
		0.53806388 0.48749989 0.53806388 0.49999988 0.53806388 0.51249987 0.53806388 0.52499986 
		0.53806388 0.53749985 0.53806388 0.54999983 0.53806388 0.56249982 0.53806388 0.57499981 
		0.53806388 0.5874998 0.53806388 0.59999979 0.53806388 0.61249977 0.53806388 0.62499976 
		0.53806388 0.375 0.57565784 0.38749999 0.57565784 0.39999998 0.57565784 0.41249996 
		0.57565784 0.42499995 0.57565784 0.43749994 0.57565784 0.44999993 0.57565784 0.46249992 
		0.57565784 0.4749999 0.57565784 0.48749989 0.57565784 0.49999988 0.57565784 0.51249987 
		0.57565784 0.52499986 0.57565784 0.53749985 0.57565784 0.54999983 0.57565784 0.56249982 
		0.57565784 0.57499981 0.57565784 0.5874998 0.57565784 0.59999979 0.57565784 0.61249977 
		0.57565784 0.62499976 0.57565784 0.375 0.61325181 0.38749999 0.61325181 0.39999998 
		0.61325181 0.41249996 0.61325181 0.42499995 0.61325181 0.43749994 0.61325181 0.44999993 
		0.61325181 0.46249992 0.61325181 0.4749999 0.61325181 0.48749989 0.61325181 0.49999988 
		0.61325181 0.51249987 0.61325181 0.52499986 0.61325181 0.53749985 0.61325181 0.54999983 
		0.61325181 0.56249982 0.61325181 0.57499981 0.61325181 0.5874998 0.61325181 0.59999979 
		0.61325181 0.61249977 0.61325181 0.62499976 0.61325181 0.375 0.65084577 0.38749999 
		0.65084577 0.39999998 0.65084577 0.41249996 0.65084577 0.42499995 0.65084577 0.43749994 
		0.65084577 0.44999993 0.65084577 0.46249992 0.65084577 0.4749999 0.65084577 0.48749989 
		0.65084577 0.49999988 0.65084577 0.51249987 0.65084577 0.52499986 0.65084577 0.53749985 
		0.65084577 0.54999983 0.65084577 0.56249982 0.65084577 0.57499981 0.65084577 0.5874998 
		0.65084577 0.59999979 0.65084577 0.61249977 0.65084577 0.62499976 0.65084577 0.375 
		0.68843973 0.38749999 0.68843973 0.39999998 0.68843973 0.41249996 0.68843973 0.42499995 
		0.68843973 0.43749994 0.68843973 0.44999993 0.68843973 0.46249992 0.68843973 0.4749999 
		0.68843973 0.48749989 0.68843973 0.49999988 0.68843973 0.51249987 0.68843973 0.52499986 
		0.68843973 0.53749985 0.68843973 0.54999983 0.68843973 0.56249982 0.68843973 0.57499981 
		0.68843973 0.5874998 0.68843973 0.59999979 0.68843973 0.61249977 0.68843973;
	setAttr ".uvst[0].uvsp[250:272]" 0.62499976 0.68843973 0.64860266 0.79546607 
		0.62640899 0.75190848 0.59184152 0.71734101 0.54828393 0.69514734 0.5 0.68749994 
		0.45171607 0.69514734 0.40815851 0.71734107 0.37359107 0.75190854 0.3513974 0.79546607 
		0.34374997 0.84375 0.3513974 0.89203393 0.37359107 0.93559146 0.40815854 0.97015893 
		0.4517161 0.9923526 0.5 1 0.54828387 0.9923526 0.59184146 0.97015893 0.62640893 0.93559146 
		0.6486026 0.89203393 0.65625 0.84375 0.5 0.15000001 0.5 0.83749998;
	setAttr ".cuvs" -type "string" "map1";
	setAttr ".dcc" -type "string" "Ambient+Diffuse";
	setAttr ".covm[0]"  0 1 1;
	setAttr ".cdvm[0]"  0 1 1;
	setAttr -s 222 ".vt";
	setAttr ".vt[0:165]"  0.95105714 -5 -0.30901718 0.80901754 -5 -0.5877856 
		0.5877856 -5 -0.80901748 0.30901715 -5 -0.95105702 0 -5 -1.0000005 -0.30901715 -5 
		-0.95105696 -0.58778548 -5 -0.8090173 -0.80901724 -5 -0.58778542 -0.95105678 -5 -0.30901706 
		-1.0000002 -5 0 -0.95105678 -5 0.30901706 -0.80901718 -5 0.58778536 -0.58778536 -5 
		0.80901712 -0.30901706 -5 0.95105666 -2.9802322e-008 -5 1.0000001 0.30901697 -5 0.9510566 
		0.58778524 -5 0.80901706 0.809017 -5 0.5877853 0.95105654 -5 0.309017 1 -5 0 0.95105714 
		-4 -0.30901718 0.80901754 -4 -0.5877856 0.5877856 -4 -0.80901748 0.30901715 -4 -0.95105702 
		0 -4 -1.0000005 -0.30901715 -4 -0.95105696 -0.58778548 -4 -0.8090173 -0.80901724 
		-4 -0.58778542 -0.95105678 -4 -0.30901706 -1.0000002 -4 0 -0.95105678 -4 0.30901706 
		-0.80901718 -4 0.58778536 -0.58778536 -4 0.80901712 -0.30901706 -4 0.95105666 -2.9802322e-008 
		-4 1.0000001 0.30901697 -4 0.9510566 0.58778524 -4 0.80901706 0.809017 -4 0.5877853 
		0.95105654 -4 0.309017 1 -4 0 0.95105714 -3 -0.30901718 0.80901754 -3 -0.5877856 
		0.5877856 -3 -0.80901748 0.30901715 -3 -0.95105702 0 -3 -1.0000005 -0.30901715 -3 
		-0.95105696 -0.58778548 -3 -0.8090173 -0.80901724 -3 -0.58778542 -0.95105678 -3 -0.30901706 
		-1.0000002 -3 0 -0.95105678 -3 0.30901706 -0.80901718 -3 0.58778536 -0.58778536 -3 
		0.80901712 -0.30901706 -3 0.95105666 -2.9802322e-008 -3 1.0000001 0.30901697 -3 0.9510566 
		0.58778524 -3 0.80901706 0.809017 -3 0.5877853 0.95105654 -3 0.309017 1 -3 0 0.95105714 
		-2 -0.30901718 0.80901754 -2 -0.5877856 0.5877856 -2 -0.80901748 0.30901715 -2 -0.95105702 
		0 -2 -1.0000005 -0.30901715 -2 -0.95105696 -0.58778548 -2 -0.8090173 -0.80901724 
		-2 -0.58778542 -0.95105678 -2 -0.30901706 -1.0000002 -2 0 -0.95105678 -2 0.30901706 
		-0.80901718 -2 0.58778536 -0.58778536 -2 0.80901712 -0.30901706 -2 0.95105666 -2.9802322e-008 
		-2 1.0000001 0.30901697 -2 0.9510566 0.58778524 -2 0.80901706 0.809017 -2 0.5877853 
		0.95105654 -2 0.309017 1 -2 0 0.95105714 -1 -0.30901718 0.80901754 -1 -0.5877856 
		0.5877856 -1 -0.80901748 0.30901715 -1 -0.95105702 0 -1 -1.0000005 -0.30901715 -1 
		-0.95105696 -0.58778548 -1 -0.8090173 -0.80901724 -1 -0.58778542 -0.95105678 -1 -0.30901706 
		-1.0000002 -1 0 -0.95105678 -1 0.30901706 -0.80901718 -1 0.58778536 -0.58778536 -1 
		0.80901712 -0.30901706 -1 0.95105666 -2.9802322e-008 -1 1.0000001 0.30901697 -1 0.9510566 
		0.58778524 -1 0.80901706 0.809017 -1 0.5877853 0.95105654 -1 0.309017 1 -1 0 0.95105714 
		0 -0.30901718 0.80901754 0 -0.5877856 0.5877856 0 -0.80901748 0.30901715 0 -0.95105702 
		0 0 -1.0000005 -0.30901715 0 -0.95105696 -0.58778548 0 -0.8090173 -0.80901724 0 -0.58778542 
		-0.95105678 0 -0.30901706 -1.0000002 0 0 -0.95105678 0 0.30901706 -0.80901718 0 0.58778536 
		-0.58778536 0 0.80901712 -0.30901706 0 0.95105666 -2.9802322e-008 0 1.0000001 0.30901697 
		0 0.9510566 0.58778524 0 0.80901706 0.809017 0 0.5877853 0.95105654 0 0.309017 1 
		0 0 0.95105714 1 -0.30901718 0.80901754 1 -0.5877856 0.5877856 1 -0.80901748 0.30901715 
		1 -0.95105702 0 1 -1.0000005 -0.30901715 1 -0.95105696 -0.58778548 1 -0.8090173 -0.80901724 
		1 -0.58778542 -0.95105678 1 -0.30901706 -1.0000002 1 0 -0.95105678 1 0.30901706 -0.80901718 
		1 0.58778536 -0.58778536 1 0.80901712 -0.30901706 1 0.95105666 -2.9802322e-008 1 
		1.0000001 0.30901697 1 0.9510566 0.58778524 1 0.80901706 0.809017 1 0.5877853 0.95105654 
		1 0.309017 1 1 0 0.95105714 2 -0.30901718 0.80901754 2 -0.5877856 0.5877856 2 -0.80901748 
		0.30901715 2 -0.95105702 0 2 -1.0000005 -0.30901715 2 -0.95105696 -0.58778548 2 -0.8090173 
		-0.80901724 2 -0.58778542 -0.95105678 2 -0.30901706 -1.0000002 2 0 -0.95105678 2 
		0.30901706 -0.80901718 2 0.58778536 -0.58778536 2 0.80901712 -0.30901706 2 0.95105666 
		-2.9802322e-008 2 1.0000001 0.30901697 2 0.9510566 0.58778524 2 0.80901706 0.809017 
		2 0.5877853 0.95105654 2 0.309017 1 2 0 0.95105714 3 -0.30901718 0.80901754 3 -0.5877856 
		0.5877856 3 -0.80901748 0.30901715 3 -0.95105702 0 3 -1.0000005 -0.30901715 3 -0.95105696;
	setAttr ".vt[166:221]" -0.58778548 3 -0.8090173 -0.80901724 3 -0.58778542 
		-0.95105678 3 -0.30901706 -1.0000002 3 0 -0.95105678 3 0.30901706 -0.80901718 3 0.58778536 
		-0.58778536 3 0.80901712 -0.30901706 3 0.95105666 -2.9802322e-008 3 1.0000001 0.30901697 
		3 0.9510566 0.58778524 3 0.80901706 0.809017 3 0.5877853 0.95105654 3 0.309017 1 
		3 0 0.95105714 4 -0.30901718 0.80901754 4 -0.5877856 0.5877856 4 -0.80901748 0.30901715 
		4 -0.95105702 0 4 -1.0000005 -0.30901715 4 -0.95105696 -0.58778548 4 -0.8090173 -0.80901724 
		4 -0.58778542 -0.95105678 4 -0.30901706 -1.0000002 4 0 -0.95105678 4 0.30901706 -0.80901718 
		4 0.58778536 -0.58778536 4 0.80901712 -0.30901706 4 0.95105666 -2.9802322e-008 4 
		1.0000001 0.30901697 4 0.9510566 0.58778524 4 0.80901706 0.809017 4 0.5877853 0.95105654 
		4 0.309017 1 4 0 0.95105714 5 -0.30901718 0.80901754 5 -0.5877856 0.5877856 5 -0.80901748 
		0.30901715 5 -0.95105702 0 5 -1.0000005 -0.30901715 5 -0.95105696 -0.58778548 5 -0.8090173 
		-0.80901724 5 -0.58778542 -0.95105678 5 -0.30901706 -1.0000002 5 0 -0.95105678 5 
		0.30901706 -0.80901718 5 0.58778536 -0.58778536 5 0.80901712 -0.30901706 5 0.95105666 
		-2.9802322e-008 5 1.0000001 0.30901697 5 0.9510566 0.58778524 5 0.80901706 0.809017 
		5 0.5877853 0.95105654 5 0.309017 1 5 0 0 -5 0 0 5 0;
	setAttr -s 460 ".ed";
	setAttr ".ed[0:165]"  0 1 0 1 2 0 2 3 
		0 3 4 0 4 5 0 5 6 0 6 7 
		0 7 8 0 8 9 0 9 10 0 10 11 
		0 11 12 0 12 13 0 13 14 0 14 15 
		0 15 16 0 16 17 0 17 18 0 18 19 
		0 19 0 0 20 21 1 21 22 1 22 23 
		1 23 24 1 24 25 1 25 26 1 26 27 
		1 27 28 1 28 29 1 29 30 1 30 31 
		1 31 32 1 32 33 1 33 34 1 34 35 
		1 35 36 1 36 37 1 37 38 1 38 39 
		1 39 20 1 40 41 1 41 42 1 42 43 
		1 43 44 1 44 45 1 45 46 1 46 47 
		1 47 48 1 48 49 1 49 50 1 50 51 
		1 51 52 1 52 53 1 53 54 1 54 55 
		1 55 56 1 56 57 1 57 58 1 58 59 
		1 59 40 1 60 61 1 61 62 1 62 63 
		1 63 64 1 64 65 1 65 66 1 66 67 
		1 67 68 1 68 69 1 69 70 1 70 71 
		1 71 72 1 72 73 1 73 74 1 74 75 
		1 75 76 1 76 77 1 77 78 1 78 79 
		1 79 60 1 80 81 1 81 82 1 82 83 
		1 83 84 1 84 85 1 85 86 1 86 87 
		1 87 88 1 88 89 1 89 90 1 90 91 
		1 91 92 1 92 93 1 93 94 1 94 95 
		1 95 96 1 96 97 1 97 98 1 98 99 
		1 99 80 1 100 101 1 101 102 1 102 103 
		1 103 104 1 104 105 1 105 106 1 106 107 
		1 107 108 1 108 109 1 109 110 1 110 111 
		1 111 112 1 112 113 1 113 114 1 114 115 
		1 115 116 1 116 117 1 117 118 1 118 119 
		1 119 100 1 120 121 1 121 122 1 122 123 
		1 123 124 1 124 125 1 125 126 1 126 127 
		1 127 128 1 128 129 1 129 130 1 130 131 
		1 131 132 1 132 133 1 133 134 1 134 135 
		1 135 136 1 136 137 1 137 138 1 138 139 
		1 139 120 1 140 141 1 141 142 1 142 143 
		1 143 144 1 144 145 1 145 146 1 146 147 
		1 147 148 1 148 149 1 149 150 1 150 151 
		1 151 152 1 152 153 1 153 154 1 154 155 
		1 155 156 1 156 157 1 157 158 1 158 159 
		1 159 140 1 160 161 1 161 162 1 162 163 
		1 163 164 1 164 165 1 165 166 1;
	setAttr ".ed[166:331]" 166 167 1 167 168 1 168 169 
		1 169 170 1 170 171 1 171 172 1 172 173 
		1 173 174 1 174 175 1 175 176 1 176 177 
		1 177 178 1 178 179 1 179 160 1 180 181 
		1 181 182 1 182 183 1 183 184 1 184 185 
		1 185 186 1 186 187 1 187 188 1 188 189 
		1 189 190 1 190 191 1 191 192 1 192 193 
		1 193 194 1 194 195 1 195 196 1 196 197 
		1 197 198 1 198 199 1 199 180 1 200 201 
		0 201 202 0 202 203 0 203 204 0 204 205 
		0 205 206 0 206 207 0 207 208 0 208 209 
		0 209 210 0 210 211 0 211 212 0 212 213 
		0 213 214 0 214 215 0 215 216 0 216 217 
		0 217 218 0 218 219 0 219 200 0 0 20 
		1 1 21 1 2 22 1 3 23 1 4 24 
		1 5 25 1 6 26 1 7 27 1 8 28 
		1 9 29 1 10 30 1 11 31 1 12 32 
		1 13 33 1 14 34 1 15 35 1 16 36 
		1 17 37 1 18 38 1 19 39 1 20 40 
		1 21 41 1 22 42 1 23 43 1 24 44 
		1 25 45 1 26 46 1 27 47 1 28 48 
		1 29 49 1 30 50 1 31 51 1 32 52 
		1 33 53 1 34 54 1 35 55 1 36 56 
		1 37 57 1 38 58 1 39 59 1 40 60 
		1 41 61 1 42 62 1 43 63 1 44 64 
		1 45 65 1 46 66 1 47 67 1 48 68 
		1 49 69 1 50 70 1 51 71 1 52 72 
		1 53 73 1 54 74 1 55 75 1 56 76 
		1 57 77 1 58 78 1 59 79 1 60 80 
		1 61 81 1 62 82 1 63 83 1 64 84 
		1 65 85 1 66 86 1 67 87 1 68 88 
		1 69 89 1 70 90 1 71 91 1 72 92 
		1 73 93 1 74 94 1 75 95 1 76 96 
		1 77 97 1 78 98 1 79 99 1 80 100 
		1 81 101 1 82 102 1 83 103 1 84 104 
		1 85 105 1 86 106 1 87 107 1 88 108 
		1 89 109 1 90 110 1 91 111 1 92 112 
		1 93 113 1 94 114 1 95 115 1 96 116 
		1 97 117 1 98 118 1 99 119 1 100 120 
		1 101 121 1 102 122 1 103 123 1 104 124 
		1 105 125 1 106 126 1 107 127 1 108 128 
		1 109 129 1 110 130 1 111 131 1;
	setAttr ".ed[332:459]" 112 132 1 113 133 1 114 134 
		1 115 135 1 116 136 1 117 137 1 118 138 
		1 119 139 1 120 140 1 121 141 1 122 142 
		1 123 143 1 124 144 1 125 145 1 126 146 
		1 127 147 1 128 148 1 129 149 1 130 150 
		1 131 151 1 132 152 1 133 153 1 134 154 
		1 135 155 1 136 156 1 137 157 1 138 158 
		1 139 159 1 140 160 1 141 161 1 142 162 
		1 143 163 1 144 164 1 145 165 1 146 166 
		1 147 167 1 148 168 1 149 169 1 150 170 
		1 151 171 1 152 172 1 153 173 1 154 174 
		1 155 175 1 156 176 1 157 177 1 158 178 
		1 159 179 1 160 180 1 161 181 1 162 182 
		1 163 183 1 164 184 1 165 185 1 166 186 
		1 167 187 1 168 188 1 169 189 1 170 190 
		1 171 191 1 172 192 1 173 193 1 174 194 
		1 175 195 1 176 196 1 177 197 1 178 198 
		1 179 199 1 180 200 1 181 201 1 182 202 
		1 183 203 1 184 204 1 185 205 1 186 206 
		1 187 207 1 188 208 1 189 209 1 190 210 
		1 191 211 1 192 212 1 193 213 1 194 214 
		1 195 215 1 196 216 1 197 217 1 198 218 
		1 199 219 1 220 0 1 220 1 1 220 2 
		1 220 3 1 220 4 1 220 5 1 220 6 
		1 220 7 1 220 8 1 220 9 1 220 10 
		1 220 11 1 220 12 1 220 13 1 220 14 
		1 220 15 1 220 16 1 220 17 1 220 18 
		1 220 19 1 200 221 1 201 221 1 202 221 
		1 203 221 1 204 221 1 205 221 1 206 221 
		1 207 221 1 208 221 1 209 221 1 210 221 
		1 211 221 1 212 221 1 213 221 1 214 221 
		1 215 221 1 216 221 1 217 221 1 218 221 
		1 219 221 1;
	setAttr -s 240 ".fc[0:239]" -type "polyFaces" 
		f 4 0 221 -21 -221 
		mu 0 4 20 21 42 41 
		f 4 1 222 -22 -222 
		mu 0 4 21 22 43 42 
		f 4 2 223 -23 -223 
		mu 0 4 22 23 44 43 
		f 4 3 224 -24 -224 
		mu 0 4 23 24 45 44 
		f 4 4 225 -25 -225 
		mu 0 4 24 25 46 45 
		f 4 5 226 -26 -226 
		mu 0 4 25 26 47 46 
		f 4 6 227 -27 -227 
		mu 0 4 26 27 48 47 
		f 4 7 228 -28 -228 
		mu 0 4 27 28 49 48 
		f 4 8 229 -29 -229 
		mu 0 4 28 29 50 49 
		f 4 9 230 -30 -230 
		mu 0 4 29 30 51 50 
		f 4 10 231 -31 -231 
		mu 0 4 30 31 52 51 
		f 4 11 232 -32 -232 
		mu 0 4 31 32 53 52 
		f 4 12 233 -33 -233 
		mu 0 4 32 33 54 53 
		f 4 13 234 -34 -234 
		mu 0 4 33 34 55 54 
		f 4 14 235 -35 -235 
		mu 0 4 34 35 56 55 
		f 4 15 236 -36 -236 
		mu 0 4 35 36 57 56 
		f 4 16 237 -37 -237 
		mu 0 4 36 37 58 57 
		f 4 17 238 -38 -238 
		mu 0 4 37 38 59 58 
		f 4 18 239 -39 -239 
		mu 0 4 38 39 60 59 
		f 4 19 220 -40 -240 
		mu 0 4 39 40 61 60 
		f 4 20 241 -41 -241 
		mu 0 4 41 42 63 62 
		f 4 21 242 -42 -242 
		mu 0 4 42 43 64 63 
		f 4 22 243 -43 -243 
		mu 0 4 43 44 65 64 
		f 4 23 244 -44 -244 
		mu 0 4 44 45 66 65 
		f 4 24 245 -45 -245 
		mu 0 4 45 46 67 66 
		f 4 25 246 -46 -246 
		mu 0 4 46 47 68 67 
		f 4 26 247 -47 -247 
		mu 0 4 47 48 69 68 
		f 4 27 248 -48 -248 
		mu 0 4 48 49 70 69 
		f 4 28 249 -49 -249 
		mu 0 4 49 50 71 70 
		f 4 29 250 -50 -250 
		mu 0 4 50 51 72 71 
		f 4 30 251 -51 -251 
		mu 0 4 51 52 73 72 
		f 4 31 252 -52 -252 
		mu 0 4 52 53 74 73 
		f 4 32 253 -53 -253 
		mu 0 4 53 54 75 74 
		f 4 33 254 -54 -254 
		mu 0 4 54 55 76 75 
		f 4 34 255 -55 -255 
		mu 0 4 55 56 77 76 
		f 4 35 256 -56 -256 
		mu 0 4 56 57 78 77 
		f 4 36 257 -57 -257 
		mu 0 4 57 58 79 78 
		f 4 37 258 -58 -258 
		mu 0 4 58 59 80 79 
		f 4 38 259 -59 -259 
		mu 0 4 59 60 81 80 
		f 4 39 240 -60 -260 
		mu 0 4 60 61 82 81 
		f 4 40 261 -61 -261 
		mu 0 4 62 63 84 83 
		f 4 41 262 -62 -262 
		mu 0 4 63 64 85 84 
		f 4 42 263 -63 -263 
		mu 0 4 64 65 86 85 
		f 4 43 264 -64 -264 
		mu 0 4 65 66 87 86 
		f 4 44 265 -65 -265 
		mu 0 4 66 67 88 87 
		f 4 45 266 -66 -266 
		mu 0 4 67 68 89 88 
		f 4 46 267 -67 -267 
		mu 0 4 68 69 90 89 
		f 4 47 268 -68 -268 
		mu 0 4 69 70 91 90 
		f 4 48 269 -69 -269 
		mu 0 4 70 71 92 91 
		f 4 49 270 -70 -270 
		mu 0 4 71 72 93 92 
		f 4 50 271 -71 -271 
		mu 0 4 72 73 94 93 
		f 4 51 272 -72 -272 
		mu 0 4 73 74 95 94 
		f 4 52 273 -73 -273 
		mu 0 4 74 75 96 95 
		f 4 53 274 -74 -274 
		mu 0 4 75 76 97 96 
		f 4 54 275 -75 -275 
		mu 0 4 76 77 98 97 
		f 4 55 276 -76 -276 
		mu 0 4 77 78 99 98 
		f 4 56 277 -77 -277 
		mu 0 4 78 79 100 99 
		f 4 57 278 -78 -278 
		mu 0 4 79 80 101 100 
		f 4 58 279 -79 -279 
		mu 0 4 80 81 102 101 
		f 4 59 260 -80 -280 
		mu 0 4 81 82 103 102 
		f 4 60 281 -81 -281 
		mu 0 4 83 84 105 104 
		f 4 61 282 -82 -282 
		mu 0 4 84 85 106 105 
		f 4 62 283 -83 -283 
		mu 0 4 85 86 107 106 
		f 4 63 284 -84 -284 
		mu 0 4 86 87 108 107 
		f 4 64 285 -85 -285 
		mu 0 4 87 88 109 108 
		f 4 65 286 -86 -286 
		mu 0 4 88 89 110 109 
		f 4 66 287 -87 -287 
		mu 0 4 89 90 111 110 
		f 4 67 288 -88 -288 
		mu 0 4 90 91 112 111 
		f 4 68 289 -89 -289 
		mu 0 4 91 92 113 112 
		f 4 69 290 -90 -290 
		mu 0 4 92 93 114 113 
		f 4 70 291 -91 -291 
		mu 0 4 93 94 115 114 
		f 4 71 292 -92 -292 
		mu 0 4 94 95 116 115 
		f 4 72 293 -93 -293 
		mu 0 4 95 96 117 116 
		f 4 73 294 -94 -294 
		mu 0 4 96 97 118 117 
		f 4 74 295 -95 -295 
		mu 0 4 97 98 119 118 
		f 4 75 296 -96 -296 
		mu 0 4 98 99 120 119 
		f 4 76 297 -97 -297 
		mu 0 4 99 100 121 120 
		f 4 77 298 -98 -298 
		mu 0 4 100 101 122 121 
		f 4 78 299 -99 -299 
		mu 0 4 101 102 123 122 
		f 4 79 280 -100 -300 
		mu 0 4 102 103 124 123 
		f 4 80 301 -101 -301 
		mu 0 4 104 105 126 125 
		f 4 81 302 -102 -302 
		mu 0 4 105 106 127 126 
		f 4 82 303 -103 -303 
		mu 0 4 106 107 128 127 
		f 4 83 304 -104 -304 
		mu 0 4 107 108 129 128 
		f 4 84 305 -105 -305 
		mu 0 4 108 109 130 129 
		f 4 85 306 -106 -306 
		mu 0 4 109 110 131 130 
		f 4 86 307 -107 -307 
		mu 0 4 110 111 132 131 
		f 4 87 308 -108 -308 
		mu 0 4 111 112 133 132 
		f 4 88 309 -109 -309 
		mu 0 4 112 113 134 133 
		f 4 89 310 -110 -310 
		mu 0 4 113 114 135 134 
		f 4 90 311 -111 -311 
		mu 0 4 114 115 136 135 
		f 4 91 312 -112 -312 
		mu 0 4 115 116 137 136 
		f 4 92 313 -113 -313 
		mu 0 4 116 117 138 137 
		f 4 93 314 -114 -314 
		mu 0 4 117 118 139 138 
		f 4 94 315 -115 -315 
		mu 0 4 118 119 140 139 
		f 4 95 316 -116 -316 
		mu 0 4 119 120 141 140 
		f 4 96 317 -117 -317 
		mu 0 4 120 121 142 141 
		f 4 97 318 -118 -318 
		mu 0 4 121 122 143 142 
		f 4 98 319 -119 -319 
		mu 0 4 122 123 144 143 
		f 4 99 300 -120 -320 
		mu 0 4 123 124 145 144 
		f 4 100 321 -121 -321 
		mu 0 4 125 126 147 146 
		f 4 101 322 -122 -322 
		mu 0 4 126 127 148 147 
		f 4 102 323 -123 -323 
		mu 0 4 127 128 149 148 
		f 4 103 324 -124 -324 
		mu 0 4 128 129 150 149 
		f 4 104 325 -125 -325 
		mu 0 4 129 130 151 150 
		f 4 105 326 -126 -326 
		mu 0 4 130 131 152 151 
		f 4 106 327 -127 -327 
		mu 0 4 131 132 153 152 
		f 4 107 328 -128 -328 
		mu 0 4 132 133 154 153 
		f 4 108 329 -129 -329 
		mu 0 4 133 134 155 154 
		f 4 109 330 -130 -330 
		mu 0 4 134 135 156 155 
		f 4 110 331 -131 -331 
		mu 0 4 135 136 157 156 
		f 4 111 332 -132 -332 
		mu 0 4 136 137 158 157 
		f 4 112 333 -133 -333 
		mu 0 4 137 138 159 158 
		f 4 113 334 -134 -334 
		mu 0 4 138 139 160 159 
		f 4 114 335 -135 -335 
		mu 0 4 139 140 161 160 
		f 4 115 336 -136 -336 
		mu 0 4 140 141 162 161 
		f 4 116 337 -137 -337 
		mu 0 4 141 142 163 162 
		f 4 117 338 -138 -338 
		mu 0 4 142 143 164 163 
		f 4 118 339 -139 -339 
		mu 0 4 143 144 165 164 
		f 4 119 320 -140 -340 
		mu 0 4 144 145 166 165 
		f 4 120 341 -141 -341 
		mu 0 4 146 147 168 167 
		f 4 121 342 -142 -342 
		mu 0 4 147 148 169 168 
		f 4 122 343 -143 -343 
		mu 0 4 148 149 170 169 
		f 4 123 344 -144 -344 
		mu 0 4 149 150 171 170 
		f 4 124 345 -145 -345 
		mu 0 4 150 151 172 171 
		f 4 125 346 -146 -346 
		mu 0 4 151 152 173 172 
		f 4 126 347 -147 -347 
		mu 0 4 152 153 174 173 
		f 4 127 348 -148 -348 
		mu 0 4 153 154 175 174 
		f 4 128 349 -149 -349 
		mu 0 4 154 155 176 175 
		f 4 129 350 -150 -350 
		mu 0 4 155 156 177 176 
		f 4 130 351 -151 -351 
		mu 0 4 156 157 178 177 
		f 4 131 352 -152 -352 
		mu 0 4 157 158 179 178 
		f 4 132 353 -153 -353 
		mu 0 4 158 159 180 179 
		f 4 133 354 -154 -354 
		mu 0 4 159 160 181 180 
		f 4 134 355 -155 -355 
		mu 0 4 160 161 182 181 
		f 4 135 356 -156 -356 
		mu 0 4 161 162 183 182 
		f 4 136 357 -157 -357 
		mu 0 4 162 163 184 183 
		f 4 137 358 -158 -358 
		mu 0 4 163 164 185 184 
		f 4 138 359 -159 -359 
		mu 0 4 164 165 186 185 
		f 4 139 340 -160 -360 
		mu 0 4 165 166 187 186 
		f 4 140 361 -161 -361 
		mu 0 4 167 168 189 188 
		f 4 141 362 -162 -362 
		mu 0 4 168 169 190 189 
		f 4 142 363 -163 -363 
		mu 0 4 169 170 191 190 
		f 4 143 364 -164 -364 
		mu 0 4 170 171 192 191 
		f 4 144 365 -165 -365 
		mu 0 4 171 172 193 192 
		f 4 145 366 -166 -366 
		mu 0 4 172 173 194 193 
		f 4 146 367 -167 -367 
		mu 0 4 173 174 195 194 
		f 4 147 368 -168 -368 
		mu 0 4 174 175 196 195 
		f 4 148 369 -169 -369 
		mu 0 4 175 176 197 196 
		f 4 149 370 -170 -370 
		mu 0 4 176 177 198 197 
		f 4 150 371 -171 -371 
		mu 0 4 177 178 199 198 
		f 4 151 372 -172 -372 
		mu 0 4 178 179 200 199 
		f 4 152 373 -173 -373 
		mu 0 4 179 180 201 200 
		f 4 153 374 -174 -374 
		mu 0 4 180 181 202 201 
		f 4 154 375 -175 -375 
		mu 0 4 181 182 203 202 
		f 4 155 376 -176 -376 
		mu 0 4 182 183 204 203 
		f 4 156 377 -177 -377 
		mu 0 4 183 184 205 204 
		f 4 157 378 -178 -378 
		mu 0 4 184 185 206 205 
		f 4 158 379 -179 -379 
		mu 0 4 185 186 207 206 
		f 4 159 360 -180 -380 
		mu 0 4 186 187 208 207 
		f 4 160 381 -181 -381 
		mu 0 4 188 189 210 209 
		f 4 161 382 -182 -382 
		mu 0 4 189 190 211 210 
		f 4 162 383 -183 -383 
		mu 0 4 190 191 212 211 
		f 4 163 384 -184 -384 
		mu 0 4 191 192 213 212 
		f 4 164 385 -185 -385 
		mu 0 4 192 193 214 213 
		f 4 165 386 -186 -386 
		mu 0 4 193 194 215 214 
		f 4 166 387 -187 -387 
		mu 0 4 194 195 216 215 
		f 4 167 388 -188 -388 
		mu 0 4 195 196 217 216 
		f 4 168 389 -189 -389 
		mu 0 4 196 197 218 217 
		f 4 169 390 -190 -390 
		mu 0 4 197 198 219 218 
		f 4 170 391 -191 -391 
		mu 0 4 198 199 220 219 
		f 4 171 392 -192 -392 
		mu 0 4 199 200 221 220 
		f 4 172 393 -193 -393 
		mu 0 4 200 201 222 221 
		f 4 173 394 -194 -394 
		mu 0 4 201 202 223 222 
		f 4 174 395 -195 -395 
		mu 0 4 202 203 224 223 
		f 4 175 396 -196 -396 
		mu 0 4 203 204 225 224 
		f 4 176 397 -197 -397 
		mu 0 4 204 205 226 225 
		f 4 177 398 -198 -398 
		mu 0 4 205 206 227 226 
		f 4 178 399 -199 -399 
		mu 0 4 206 207 228 227 
		f 4 179 380 -200 -400 
		mu 0 4 207 208 229 228 
		f 4 180 401 -201 -401 
		mu 0 4 209 210 231 230 
		f 4 181 402 -202 -402 
		mu 0 4 210 211 232 231 
		f 4 182 403 -203 -403 
		mu 0 4 211 212 233 232 
		f 4 183 404 -204 -404 
		mu 0 4 212 213 234 233 
		f 4 184 405 -205 -405 
		mu 0 4 213 214 235 234 
		f 4 185 406 -206 -406 
		mu 0 4 214 215 236 235 
		f 4 186 407 -207 -407 
		mu 0 4 215 216 237 236 
		f 4 187 408 -208 -408 
		mu 0 4 216 217 238 237 
		f 4 188 409 -209 -409 
		mu 0 4 217 218 239 238 
		f 4 189 410 -210 -410 
		mu 0 4 218 219 240 239 
		f 4 190 411 -211 -411 
		mu 0 4 219 220 241 240 
		f 4 191 412 -212 -412 
		mu 0 4 220 221 242 241 
		f 4 192 413 -213 -413 
		mu 0 4 221 222 243 242 
		f 4 193 414 -214 -414 
		mu 0 4 222 223 244 243 
		f 4 194 415 -215 -415 
		mu 0 4 223 224 245 244 
		f 4 195 416 -216 -416 
		mu 0 4 224 225 246 245 
		f 4 196 417 -217 -417 
		mu 0 4 225 226 247 246 
		f 4 197 418 -218 -418 
		mu 0 4 226 227 248 247 
		f 4 198 419 -219 -419 
		mu 0 4 227 228 249 248 
		f 4 199 400 -220 -420 
		mu 0 4 228 229 250 249 
		f 3 -1 -421 421 
		mu 0 3 1 0 271 
		f 3 -2 -422 422 
		mu 0 3 2 1 271 
		f 3 -3 -423 423 
		mu 0 3 3 2 271 
		f 3 -4 -424 424 
		mu 0 3 4 3 271 
		f 3 -5 -425 425 
		mu 0 3 5 4 271 
		f 3 -6 -426 426 
		mu 0 3 6 5 271 
		f 3 -7 -427 427 
		mu 0 3 7 6 271 
		f 3 -8 -428 428 
		mu 0 3 8 7 271 
		f 3 -9 -429 429 
		mu 0 3 9 8 271 
		f 3 -10 -430 430 
		mu 0 3 10 9 271 
		f 3 -11 -431 431 
		mu 0 3 11 10 271 
		f 3 -12 -432 432 
		mu 0 3 12 11 271 
		f 3 -13 -433 433 
		mu 0 3 13 12 271 
		f 3 -14 -434 434 
		mu 0 3 14 13 271 
		f 3 -15 -435 435 
		mu 0 3 15 14 271 
		f 3 -16 -436 436 
		mu 0 3 16 15 271 
		f 3 -17 -437 437 
		mu 0 3 17 16 271 
		f 3 -18 -438 438 
		mu 0 3 18 17 271 
		f 3 -19 -439 439 
		mu 0 3 19 18 271 
		f 3 -20 -440 420 
		mu 0 3 0 19 271 
		f 3 200 441 -441 
		mu 0 3 269 268 272 
		f 3 201 442 -442 
		mu 0 3 268 267 272 
		f 3 202 443 -443 
		mu 0 3 267 266 272 
		f 3 203 444 -444 
		mu 0 3 266 265 272 
		f 3 204 445 -445 
		mu 0 3 265 264 272 
		f 3 205 446 -446 
		mu 0 3 264 263 272 
		f 3 206 447 -447 
		mu 0 3 263 262 272 
		f 3 207 448 -448 
		mu 0 3 262 261 272 
		f 3 208 449 -449 
		mu 0 3 261 260 272 
		f 3 209 450 -450 
		mu 0 3 260 259 272 
		f 3 210 451 -451 
		mu 0 3 259 258 272 
		f 3 211 452 -452 
		mu 0 3 258 257 272 
		f 3 212 453 -453 
		mu 0 3 257 256 272 
		f 3 213 454 -454 
		mu 0 3 256 255 272 
		f 3 214 455 -455 
		mu 0 3 255 254 272 
		f 3 215 456 -456 
		mu 0 3 254 253 272 
		f 3 216 457 -457 
		mu 0 3 253 252 272 
		f 3 217 458 -458 
		mu 0 3 252 251 272 
		f 3 218 459 -459 
		mu 0 3 251 270 272 
		f 3 219 440 -460 
		mu 0 3 270 269 272 ;
	setAttr ".cd" -type "dataPolyComponent" Index_Data Edge 0 ;
	setAttr ".cvd" -type "dataPolyComponent" Index_Data Vertex 0 ;
	setAttr ".hfd" -type "dataPolyComponent" Index_Data Face 0 ;
createNode place3dTexture -n "reference:place3dTexture1";
createNode transform -n "directionalLight1";
	setAttr ".t" -type "double3" 0 -2.6704400917380697 9.3887730745344875 ;
	setAttr ".r" -type "double3" 0 2.2484173379269747 0 ;
createNode directionalLight -n "directionalLightShape1" -p "directionalLight1";
	addAttr -ci true -k true -sn "ai_user_options" -ln "aiUserOptions" -dt "string";
	addAttr -ci true -sn "ai_cast_shadows" -ln "aiCastShadows" -dv 1 -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "ai_shadow_density" -ln "aiShadowDensity" -dv 1 -min 0 -max 
		1 -smn 0 -smx 1 -at "float";
	addAttr -ci true -sn "ai_exposure" -ln "aiExposure" -min 0 -smx 10 -at "float";
	addAttr -ci true -sn "ai_samples" -ln "aiSamples" -dv 1 -min 1 -max 100 -at "long";
	addAttr -ci true -sn "ai_normalize" -ln "aiNormalize" -dv 1 -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "ai_bounce_factor" -ln "aiBounceFactor" -dv 1 -min 0 -smx 20 
		-at "float";
	addAttr -ci true -sn "ai_bounces" -ln "aiBounces" -dv 999 -min 0 -max 10000 -at "long";
	addAttr -ci true -k true -m -sn "ai_filters" -ln "aiFilters" -at "message";
	addAttr -ci true -sn "ai_angle" -ln "aiAngle" -min 0 -smx 10 -at "float";
	setAttr -k off ".v";
	setAttr ".in" 5;
createNode aiOptions -s -n "defaultArnoldRenderOptions";
	addAttr -ci true -sn "driver_exr_compression" -ln "driverExrCompression" -dv 2 
		-min 0 -max 4 -en "none:rle:zip:piz:pxr24" -at "enum";
	addAttr -ci true -k true -sn "driver_half_precision" -ln "driverHalfPrecision" -min 
		0 -max 1 -at "bool";
	addAttr -ci true -k true -sn "driver_tiled" -ln "driverTiled" -dv 1 -min 0 -max 
		1 -at "bool";
	addAttr -ci true -k true -sn "driver_preserve_layer_name" -ln "driverPreserveLayerName" 
		-min 0 -max 1 -at "bool";
	addAttr -ci true -k true -sn "driver_quality" -ln "driverQuality" -dv 100 -min 0 
		-max 100 -at "long";
	addAttr -ci true -k true -sn "driver_output_padded" -ln "driverOutputPadded" -min 
		0 -max 1 -at "bool";
	addAttr -ci true -k true -sn "driver_gamma" -ln "driverGamma" -dv 2.2000000476837158 
		-min 9.9999997473787516e-005 -smx 5 -at "float";
	addAttr -ci true -k true -sn "driver_dither_amplitude" -ln "driverDitherAmplitude" 
		-dv 1 -at "float";
	addAttr -ci true -sn "driver_png_format" -ln "driverPngFormat" -min 0 -max 1 -en 
		"int8:int16" -at "enum";
	addAttr -ci true -sn "driver_tiff_compression" -ln "driverTiffCompression" -min 
		0 -max 4 -en "none:lzw:ccittrle:zip:packbits" -at "enum";
	addAttr -ci true -sn "driver_tiff_format" -ln "driverTiffFormat" -min 0 -max 3 -en 
		"int8:int16:int32:float32" -at "enum";
	addAttr -ci true -k true -sn "driver_unpremult_alpha" -ln "driverUnpremultAlpha" 
		-min 0 -max 1 -at "bool";
	addAttr -ci true -k true -sn "filter_width" -ln "filterWidth" -dv 3 -at "float";
	addAttr -ci true -sn "filter_domain" -ln "filterDomain" -min 0 -max 1 -en "first_hit:all_hits" 
		-at "enum";
	addAttr -ci true -k true -sn "filter_minimum" -ln "filterMinimum" -at "float";
	addAttr -ci true -k true -sn "filter_maximum" -ln "filterMaximum" -dv 1 -at "float";
	addAttr -ci true -k true -sn "filter_scalar_mode" -ln "filterScalarMode" -min 0 
		-max 1 -at "bool";
	setAttr ".img" -type "string" "tif";
	setAttr ".fltr" -type "string" "gaussian";
	setAttr ".dgamma" 1;
	setAttr ".lgamma" 1;
	setAttr ".sgamma" 1;
	setAttr ".tgamma" 1;
	setAttr -k on ".driver_tiled" no;
	setAttr -k on ".driver_gamma" 1;
	setAttr -k on ".filter_width" 2;
createNode lightLinker -s -n "lightLinker1";
	setAttr -s 5 ".lnk";
	setAttr -s 5 ".slnk";
createNode displayLayerManager -n "layerManager";
createNode displayLayer -n "defaultLayer";
createNode renderLayerManager -n "renderLayerManager";
createNode renderLayer -n "defaultRenderLayer";
	setAttr ".g" yes;
createNode aiStandard -n "ArnoldStandardShader1";
	setAttr ".Kd_color" -type "float3" 1 0 0 ;
	setAttr ".emission" 1;
	setAttr ".emission_color" -type "float3" 1 0 0 ;
createNode shadingEngine -n "ArnoldStandardShader1SG";
	addAttr -ci true -k true -sn "ai_user_options" -ln "aiUserOptions" -dt "string";
	addAttr -ci true -m -sn "aovs" -ln "aiCustomAOVs" -at "compound" -nc 2;
	addAttr -ci true -k true -sn "aov_name" -ln "aovName" -dt "string" -p "aiCustomAOVs";
	addAttr -ci true -k true -sn "aov_input" -ln "aovInput" -at "message" -p "aiCustomAOVs";
	setAttr ".ihi" 0;
	setAttr ".ro" yes;
createNode materialInfo -n "materialInfo1";
createNode shadingEngine -n "ArnoldStandardShader1SG1";
	addAttr -ci true -k true -sn "ai_user_options" -ln "aiUserOptions" -dt "string";
	addAttr -ci true -m -sn "aovs" -ln "aiCustomAOVs" -at "compound" -nc 2;
	addAttr -ci true -k true -sn "aov_name" -ln "aovName" -dt "string" -p "aiCustomAOVs";
	addAttr -ci true -k true -sn "aov_input" -ln "aovInput" -at "message" -p "aiCustomAOVs";
	setAttr ".ihi" 0;
	setAttr ".ro" yes;
createNode materialInfo -n "materialInfo2";
createNode script -n "sceneConfigurationScriptNode";
	setAttr ".b" -type "string" "playbackOptions -min 1 -max 24 -ast 1 -aet 48 ";
	setAttr ".st" 6;
createNode script -n "uiConfigurationScriptNode";
	setAttr ".b" -type "string" (
		"// Maya Mel UI Configuration File.\n//\n//  This script is machine generated.  Edit at your own risk.\n//\n//\n\nglobal string $gMainPane;\nif (`paneLayout -exists $gMainPane`) {\n\n\tglobal int $gUseScenePanelConfig;\n\tint    $useSceneConfig = $gUseScenePanelConfig;\n\tint    $menusOkayInPanels = `optionVar -q allowMenusInPanels`;\tint    $nVisPanes = `paneLayout -q -nvp $gMainPane`;\n\tint    $nPanes = 0;\n\tstring $editorName;\n\tstring $panelName;\n\tstring $itemFilterName;\n\tstring $panelConfig;\n\n\t//\n\t//  get current state of the UI\n\t//\n\tsceneUIReplacement -update $gMainPane;\n\n\t$panelName = `sceneUIReplacement -getNextPanel \"modelPanel\" (localizedPanelLabel(\"Top View\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `modelPanel -unParent -l (localizedPanelLabel(\"Top View\")) -mbv $menusOkayInPanels `;\n\t\t\t$editorName = $panelName;\n            modelEditor -e \n                -camera \"top\" \n                -useInteractiveMode 0\n                -displayLights \"default\" \n                -displayAppearance \"wireframe\" \n"
		+ "                -activeOnly 0\n                -ignorePanZoom 0\n                -wireframeOnShaded 0\n                -headsUpDisplay 1\n                -selectionHiliteDisplay 1\n                -useDefaultMaterial 0\n                -bufferMode \"double\" \n                -twoSidedLighting 1\n                -backfaceCulling 0\n                -xray 0\n                -jointXray 0\n                -activeComponentsXray 0\n                -displayTextures 0\n                -smoothWireframe 0\n                -lineWidth 1\n                -textureAnisotropic 0\n                -textureHilight 1\n                -textureSampling 2\n                -textureDisplay \"modulate\" \n                -textureMaxSize 16384\n                -fogging 0\n                -fogSource \"fragment\" \n                -fogMode \"linear\" \n                -fogStart 0\n                -fogEnd 100\n                -fogDensity 0.1\n                -fogColor 0.5 0.5 0.5 1 \n                -maxConstantTransparency 1\n                -rendererName \"base_OpenGL_Renderer\" \n"
		+ "                -colorResolution 256 256 \n                -bumpResolution 512 512 \n                -textureCompression 0\n                -transparencyAlgorithm \"frontAndBackCull\" \n                -transpInShadows 0\n                -cullingOverride \"none\" \n                -lowQualityLighting 0\n                -maximumNumHardwareLights 1\n                -occlusionCulling 0\n                -shadingModel 0\n                -useBaseRenderer 0\n                -useReducedRenderer 0\n                -smallObjectCulling 0\n                -smallObjectThreshold -1 \n                -interactiveDisableShadows 0\n                -interactiveBackFaceCull 0\n                -sortTransparent 1\n                -nurbsCurves 1\n                -nurbsSurfaces 1\n                -polymeshes 1\n                -subdivSurfaces 1\n                -planes 1\n                -lights 1\n                -cameras 1\n                -controlVertices 1\n                -hulls 1\n                -grid 1\n                -joints 1\n                -ikHandles 1\n"
		+ "                -deformers 1\n                -dynamics 1\n                -fluids 1\n                -hairSystems 1\n                -follicles 1\n                -nCloths 1\n                -nParticles 1\n                -nRigids 1\n                -dynamicConstraints 1\n                -locators 1\n                -manipulators 1\n                -dimensions 1\n                -handles 1\n                -pivots 1\n                -textures 1\n                -strokes 1\n                -shadows 0\n                $editorName;\nmodelEditor -e -viewSelected 0 $editorName;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tmodelPanel -edit -l (localizedPanelLabel(\"Top View\")) -mbv $menusOkayInPanels  $panelName;\n\t\t$editorName = $panelName;\n        modelEditor -e \n            -camera \"top\" \n            -useInteractiveMode 0\n            -displayLights \"default\" \n            -displayAppearance \"wireframe\" \n            -activeOnly 0\n            -ignorePanZoom 0\n            -wireframeOnShaded 0\n            -headsUpDisplay 1\n            -selectionHiliteDisplay 1\n"
		+ "            -useDefaultMaterial 0\n            -bufferMode \"double\" \n            -twoSidedLighting 1\n            -backfaceCulling 0\n            -xray 0\n            -jointXray 0\n            -activeComponentsXray 0\n            -displayTextures 0\n            -smoothWireframe 0\n            -lineWidth 1\n            -textureAnisotropic 0\n            -textureHilight 1\n            -textureSampling 2\n            -textureDisplay \"modulate\" \n            -textureMaxSize 16384\n            -fogging 0\n            -fogSource \"fragment\" \n            -fogMode \"linear\" \n            -fogStart 0\n            -fogEnd 100\n            -fogDensity 0.1\n            -fogColor 0.5 0.5 0.5 1 \n            -maxConstantTransparency 1\n            -rendererName \"base_OpenGL_Renderer\" \n            -colorResolution 256 256 \n            -bumpResolution 512 512 \n            -textureCompression 0\n            -transparencyAlgorithm \"frontAndBackCull\" \n            -transpInShadows 0\n            -cullingOverride \"none\" \n            -lowQualityLighting 0\n"
		+ "            -maximumNumHardwareLights 1\n            -occlusionCulling 0\n            -shadingModel 0\n            -useBaseRenderer 0\n            -useReducedRenderer 0\n            -smallObjectCulling 0\n            -smallObjectThreshold -1 \n            -interactiveDisableShadows 0\n            -interactiveBackFaceCull 0\n            -sortTransparent 1\n            -nurbsCurves 1\n            -nurbsSurfaces 1\n            -polymeshes 1\n            -subdivSurfaces 1\n            -planes 1\n            -lights 1\n            -cameras 1\n            -controlVertices 1\n            -hulls 1\n            -grid 1\n            -joints 1\n            -ikHandles 1\n            -deformers 1\n            -dynamics 1\n            -fluids 1\n            -hairSystems 1\n            -follicles 1\n            -nCloths 1\n            -nParticles 1\n            -nRigids 1\n            -dynamicConstraints 1\n            -locators 1\n            -manipulators 1\n            -dimensions 1\n            -handles 1\n            -pivots 1\n            -textures 1\n            -strokes 1\n"
		+ "            -shadows 0\n            $editorName;\nmodelEditor -e -viewSelected 0 $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextPanel \"modelPanel\" (localizedPanelLabel(\"Side View\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `modelPanel -unParent -l (localizedPanelLabel(\"Side View\")) -mbv $menusOkayInPanels `;\n\t\t\t$editorName = $panelName;\n            modelEditor -e \n                -camera \"side\" \n                -useInteractiveMode 0\n                -displayLights \"default\" \n                -displayAppearance \"wireframe\" \n                -activeOnly 0\n                -ignorePanZoom 0\n                -wireframeOnShaded 0\n                -headsUpDisplay 1\n                -selectionHiliteDisplay 1\n                -useDefaultMaterial 0\n                -bufferMode \"double\" \n                -twoSidedLighting 1\n                -backfaceCulling 0\n                -xray 0\n                -jointXray 0\n                -activeComponentsXray 0\n"
		+ "                -displayTextures 0\n                -smoothWireframe 0\n                -lineWidth 1\n                -textureAnisotropic 0\n                -textureHilight 1\n                -textureSampling 2\n                -textureDisplay \"modulate\" \n                -textureMaxSize 16384\n                -fogging 0\n                -fogSource \"fragment\" \n                -fogMode \"linear\" \n                -fogStart 0\n                -fogEnd 100\n                -fogDensity 0.1\n                -fogColor 0.5 0.5 0.5 1 \n                -maxConstantTransparency 1\n                -rendererName \"base_OpenGL_Renderer\" \n                -colorResolution 256 256 \n                -bumpResolution 512 512 \n                -textureCompression 0\n                -transparencyAlgorithm \"frontAndBackCull\" \n                -transpInShadows 0\n                -cullingOverride \"none\" \n                -lowQualityLighting 0\n                -maximumNumHardwareLights 1\n                -occlusionCulling 0\n                -shadingModel 0\n                -useBaseRenderer 0\n"
		+ "                -useReducedRenderer 0\n                -smallObjectCulling 0\n                -smallObjectThreshold -1 \n                -interactiveDisableShadows 0\n                -interactiveBackFaceCull 0\n                -sortTransparent 1\n                -nurbsCurves 1\n                -nurbsSurfaces 1\n                -polymeshes 1\n                -subdivSurfaces 1\n                -planes 1\n                -lights 1\n                -cameras 1\n                -controlVertices 1\n                -hulls 1\n                -grid 1\n                -joints 1\n                -ikHandles 1\n                -deformers 1\n                -dynamics 1\n                -fluids 1\n                -hairSystems 1\n                -follicles 1\n                -nCloths 1\n                -nParticles 1\n                -nRigids 1\n                -dynamicConstraints 1\n                -locators 1\n                -manipulators 1\n                -dimensions 1\n                -handles 1\n                -pivots 1\n                -textures 1\n                -strokes 1\n"
		+ "                -shadows 0\n                $editorName;\nmodelEditor -e -viewSelected 0 $editorName;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tmodelPanel -edit -l (localizedPanelLabel(\"Side View\")) -mbv $menusOkayInPanels  $panelName;\n\t\t$editorName = $panelName;\n        modelEditor -e \n            -camera \"side\" \n            -useInteractiveMode 0\n            -displayLights \"default\" \n            -displayAppearance \"wireframe\" \n            -activeOnly 0\n            -ignorePanZoom 0\n            -wireframeOnShaded 0\n            -headsUpDisplay 1\n            -selectionHiliteDisplay 1\n            -useDefaultMaterial 0\n            -bufferMode \"double\" \n            -twoSidedLighting 1\n            -backfaceCulling 0\n            -xray 0\n            -jointXray 0\n            -activeComponentsXray 0\n            -displayTextures 0\n            -smoothWireframe 0\n            -lineWidth 1\n            -textureAnisotropic 0\n            -textureHilight 1\n            -textureSampling 2\n            -textureDisplay \"modulate\" \n"
		+ "            -textureMaxSize 16384\n            -fogging 0\n            -fogSource \"fragment\" \n            -fogMode \"linear\" \n            -fogStart 0\n            -fogEnd 100\n            -fogDensity 0.1\n            -fogColor 0.5 0.5 0.5 1 \n            -maxConstantTransparency 1\n            -rendererName \"base_OpenGL_Renderer\" \n            -colorResolution 256 256 \n            -bumpResolution 512 512 \n            -textureCompression 0\n            -transparencyAlgorithm \"frontAndBackCull\" \n            -transpInShadows 0\n            -cullingOverride \"none\" \n            -lowQualityLighting 0\n            -maximumNumHardwareLights 1\n            -occlusionCulling 0\n            -shadingModel 0\n            -useBaseRenderer 0\n            -useReducedRenderer 0\n            -smallObjectCulling 0\n            -smallObjectThreshold -1 \n            -interactiveDisableShadows 0\n            -interactiveBackFaceCull 0\n            -sortTransparent 1\n            -nurbsCurves 1\n            -nurbsSurfaces 1\n            -polymeshes 1\n            -subdivSurfaces 1\n"
		+ "            -planes 1\n            -lights 1\n            -cameras 1\n            -controlVertices 1\n            -hulls 1\n            -grid 1\n            -joints 1\n            -ikHandles 1\n            -deformers 1\n            -dynamics 1\n            -fluids 1\n            -hairSystems 1\n            -follicles 1\n            -nCloths 1\n            -nParticles 1\n            -nRigids 1\n            -dynamicConstraints 1\n            -locators 1\n            -manipulators 1\n            -dimensions 1\n            -handles 1\n            -pivots 1\n            -textures 1\n            -strokes 1\n            -shadows 0\n            $editorName;\nmodelEditor -e -viewSelected 0 $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextPanel \"modelPanel\" (localizedPanelLabel(\"Front View\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `modelPanel -unParent -l (localizedPanelLabel(\"Front View\")) -mbv $menusOkayInPanels `;\n\t\t\t$editorName = $panelName;\n"
		+ "            modelEditor -e \n                -camera \"front\" \n                -useInteractiveMode 0\n                -displayLights \"default\" \n                -displayAppearance \"wireframe\" \n                -activeOnly 0\n                -ignorePanZoom 0\n                -wireframeOnShaded 0\n                -headsUpDisplay 1\n                -selectionHiliteDisplay 1\n                -useDefaultMaterial 0\n                -bufferMode \"double\" \n                -twoSidedLighting 1\n                -backfaceCulling 0\n                -xray 0\n                -jointXray 0\n                -activeComponentsXray 0\n                -displayTextures 0\n                -smoothWireframe 0\n                -lineWidth 1\n                -textureAnisotropic 0\n                -textureHilight 1\n                -textureSampling 2\n                -textureDisplay \"modulate\" \n                -textureMaxSize 16384\n                -fogging 0\n                -fogSource \"fragment\" \n                -fogMode \"linear\" \n                -fogStart 0\n                -fogEnd 100\n"
		+ "                -fogDensity 0.1\n                -fogColor 0.5 0.5 0.5 1 \n                -maxConstantTransparency 1\n                -rendererName \"base_OpenGL_Renderer\" \n                -colorResolution 256 256 \n                -bumpResolution 512 512 \n                -textureCompression 0\n                -transparencyAlgorithm \"frontAndBackCull\" \n                -transpInShadows 0\n                -cullingOverride \"none\" \n                -lowQualityLighting 0\n                -maximumNumHardwareLights 1\n                -occlusionCulling 0\n                -shadingModel 0\n                -useBaseRenderer 0\n                -useReducedRenderer 0\n                -smallObjectCulling 0\n                -smallObjectThreshold -1 \n                -interactiveDisableShadows 0\n                -interactiveBackFaceCull 0\n                -sortTransparent 1\n                -nurbsCurves 1\n                -nurbsSurfaces 1\n                -polymeshes 1\n                -subdivSurfaces 1\n                -planes 1\n                -lights 1\n"
		+ "                -cameras 1\n                -controlVertices 1\n                -hulls 1\n                -grid 1\n                -joints 1\n                -ikHandles 1\n                -deformers 1\n                -dynamics 1\n                -fluids 1\n                -hairSystems 1\n                -follicles 1\n                -nCloths 1\n                -nParticles 1\n                -nRigids 1\n                -dynamicConstraints 1\n                -locators 1\n                -manipulators 1\n                -dimensions 1\n                -handles 1\n                -pivots 1\n                -textures 1\n                -strokes 1\n                -shadows 0\n                $editorName;\nmodelEditor -e -viewSelected 0 $editorName;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tmodelPanel -edit -l (localizedPanelLabel(\"Front View\")) -mbv $menusOkayInPanels  $panelName;\n\t\t$editorName = $panelName;\n        modelEditor -e \n            -camera \"front\" \n            -useInteractiveMode 0\n            -displayLights \"default\" \n"
		+ "            -displayAppearance \"wireframe\" \n            -activeOnly 0\n            -ignorePanZoom 0\n            -wireframeOnShaded 0\n            -headsUpDisplay 1\n            -selectionHiliteDisplay 1\n            -useDefaultMaterial 0\n            -bufferMode \"double\" \n            -twoSidedLighting 1\n            -backfaceCulling 0\n            -xray 0\n            -jointXray 0\n            -activeComponentsXray 0\n            -displayTextures 0\n            -smoothWireframe 0\n            -lineWidth 1\n            -textureAnisotropic 0\n            -textureHilight 1\n            -textureSampling 2\n            -textureDisplay \"modulate\" \n            -textureMaxSize 16384\n            -fogging 0\n            -fogSource \"fragment\" \n            -fogMode \"linear\" \n            -fogStart 0\n            -fogEnd 100\n            -fogDensity 0.1\n            -fogColor 0.5 0.5 0.5 1 \n            -maxConstantTransparency 1\n            -rendererName \"base_OpenGL_Renderer\" \n            -colorResolution 256 256 \n            -bumpResolution 512 512 \n"
		+ "            -textureCompression 0\n            -transparencyAlgorithm \"frontAndBackCull\" \n            -transpInShadows 0\n            -cullingOverride \"none\" \n            -lowQualityLighting 0\n            -maximumNumHardwareLights 1\n            -occlusionCulling 0\n            -shadingModel 0\n            -useBaseRenderer 0\n            -useReducedRenderer 0\n            -smallObjectCulling 0\n            -smallObjectThreshold -1 \n            -interactiveDisableShadows 0\n            -interactiveBackFaceCull 0\n            -sortTransparent 1\n            -nurbsCurves 1\n            -nurbsSurfaces 1\n            -polymeshes 1\n            -subdivSurfaces 1\n            -planes 1\n            -lights 1\n            -cameras 1\n            -controlVertices 1\n            -hulls 1\n            -grid 1\n            -joints 1\n            -ikHandles 1\n            -deformers 1\n            -dynamics 1\n            -fluids 1\n            -hairSystems 1\n            -follicles 1\n            -nCloths 1\n            -nParticles 1\n            -nRigids 1\n"
		+ "            -dynamicConstraints 1\n            -locators 1\n            -manipulators 1\n            -dimensions 1\n            -handles 1\n            -pivots 1\n            -textures 1\n            -strokes 1\n            -shadows 0\n            $editorName;\nmodelEditor -e -viewSelected 0 $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextPanel \"modelPanel\" (localizedPanelLabel(\"Persp View\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `modelPanel -unParent -l (localizedPanelLabel(\"Persp View\")) -mbv $menusOkayInPanels `;\n\t\t\t$editorName = $panelName;\n            modelEditor -e \n                -camera \"persp\" \n                -useInteractiveMode 0\n                -displayLights \"default\" \n                -displayAppearance \"wireframe\" \n                -activeOnly 0\n                -ignorePanZoom 0\n                -wireframeOnShaded 0\n                -headsUpDisplay 1\n                -selectionHiliteDisplay 1\n                -useDefaultMaterial 0\n"
		+ "                -bufferMode \"double\" \n                -twoSidedLighting 1\n                -backfaceCulling 0\n                -xray 0\n                -jointXray 0\n                -activeComponentsXray 0\n                -displayTextures 0\n                -smoothWireframe 0\n                -lineWidth 1\n                -textureAnisotropic 0\n                -textureHilight 1\n                -textureSampling 2\n                -textureDisplay \"modulate\" \n                -textureMaxSize 16384\n                -fogging 0\n                -fogSource \"fragment\" \n                -fogMode \"linear\" \n                -fogStart 0\n                -fogEnd 100\n                -fogDensity 0.1\n                -fogColor 0.5 0.5 0.5 1 \n                -maxConstantTransparency 1\n                -rendererName \"base_OpenGL_Renderer\" \n                -colorResolution 256 256 \n                -bumpResolution 512 512 \n                -textureCompression 0\n                -transparencyAlgorithm \"frontAndBackCull\" \n                -transpInShadows 0\n"
		+ "                -cullingOverride \"none\" \n                -lowQualityLighting 0\n                -maximumNumHardwareLights 1\n                -occlusionCulling 0\n                -shadingModel 0\n                -useBaseRenderer 0\n                -useReducedRenderer 0\n                -smallObjectCulling 0\n                -smallObjectThreshold -1 \n                -interactiveDisableShadows 0\n                -interactiveBackFaceCull 0\n                -sortTransparent 1\n                -nurbsCurves 1\n                -nurbsSurfaces 1\n                -polymeshes 1\n                -subdivSurfaces 1\n                -planes 1\n                -lights 1\n                -cameras 1\n                -controlVertices 1\n                -hulls 1\n                -grid 1\n                -joints 1\n                -ikHandles 1\n                -deformers 1\n                -dynamics 1\n                -fluids 1\n                -hairSystems 1\n                -follicles 1\n                -nCloths 1\n                -nParticles 1\n                -nRigids 1\n"
		+ "                -dynamicConstraints 1\n                -locators 1\n                -manipulators 1\n                -dimensions 1\n                -handles 1\n                -pivots 1\n                -textures 1\n                -strokes 1\n                -shadows 0\n                $editorName;\nmodelEditor -e -viewSelected 0 $editorName;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tmodelPanel -edit -l (localizedPanelLabel(\"Persp View\")) -mbv $menusOkayInPanels  $panelName;\n\t\t$editorName = $panelName;\n        modelEditor -e \n            -camera \"persp\" \n            -useInteractiveMode 0\n            -displayLights \"default\" \n            -displayAppearance \"wireframe\" \n            -activeOnly 0\n            -ignorePanZoom 0\n            -wireframeOnShaded 0\n            -headsUpDisplay 1\n            -selectionHiliteDisplay 1\n            -useDefaultMaterial 0\n            -bufferMode \"double\" \n            -twoSidedLighting 1\n            -backfaceCulling 0\n            -xray 0\n            -jointXray 0\n            -activeComponentsXray 0\n"
		+ "            -displayTextures 0\n            -smoothWireframe 0\n            -lineWidth 1\n            -textureAnisotropic 0\n            -textureHilight 1\n            -textureSampling 2\n            -textureDisplay \"modulate\" \n            -textureMaxSize 16384\n            -fogging 0\n            -fogSource \"fragment\" \n            -fogMode \"linear\" \n            -fogStart 0\n            -fogEnd 100\n            -fogDensity 0.1\n            -fogColor 0.5 0.5 0.5 1 \n            -maxConstantTransparency 1\n            -rendererName \"base_OpenGL_Renderer\" \n            -colorResolution 256 256 \n            -bumpResolution 512 512 \n            -textureCompression 0\n            -transparencyAlgorithm \"frontAndBackCull\" \n            -transpInShadows 0\n            -cullingOverride \"none\" \n            -lowQualityLighting 0\n            -maximumNumHardwareLights 1\n            -occlusionCulling 0\n            -shadingModel 0\n            -useBaseRenderer 0\n            -useReducedRenderer 0\n            -smallObjectCulling 0\n            -smallObjectThreshold -1 \n"
		+ "            -interactiveDisableShadows 0\n            -interactiveBackFaceCull 0\n            -sortTransparent 1\n            -nurbsCurves 1\n            -nurbsSurfaces 1\n            -polymeshes 1\n            -subdivSurfaces 1\n            -planes 1\n            -lights 1\n            -cameras 1\n            -controlVertices 1\n            -hulls 1\n            -grid 1\n            -joints 1\n            -ikHandles 1\n            -deformers 1\n            -dynamics 1\n            -fluids 1\n            -hairSystems 1\n            -follicles 1\n            -nCloths 1\n            -nParticles 1\n            -nRigids 1\n            -dynamicConstraints 1\n            -locators 1\n            -manipulators 1\n            -dimensions 1\n            -handles 1\n            -pivots 1\n            -textures 1\n            -strokes 1\n            -shadows 0\n            $editorName;\nmodelEditor -e -viewSelected 0 $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextPanel \"outlinerPanel\" (localizedPanelLabel(\"Outliner\")) `;\n"
		+ "\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `outlinerPanel -unParent -l (localizedPanelLabel(\"Outliner\")) -mbv $menusOkayInPanels `;\n\t\t\t$editorName = $panelName;\n            outlinerEditor -e \n                -showShapes 1\n                -showAttributes 0\n                -showConnected 0\n                -showAnimCurvesOnly 0\n                -showMuteInfo 0\n                -organizeByLayer 1\n                -showAnimLayerWeight 1\n                -autoExpandLayers 1\n                -autoExpand 0\n                -showDagOnly 1\n                -showAssets 1\n                -showContainedOnly 1\n                -showPublishedAsConnected 0\n                -showContainerContents 1\n                -ignoreDagHierarchy 0\n                -expandConnections 0\n                -showUpstreamCurves 1\n                -showUnitlessCurves 1\n                -showCompounds 1\n                -showLeafs 1\n                -showNumericAttrsOnly 0\n                -highlightActive 1\n                -autoSelectNewObjects 0\n"
		+ "                -doNotSelectNewObjects 0\n                -dropIsParent 1\n                -transmitFilters 0\n                -setFilter \"defaultSetFilter\" \n                -showSetMembers 1\n                -allowMultiSelection 1\n                -alwaysToggleSelect 0\n                -directSelect 0\n                -displayMode \"DAG\" \n                -expandObjects 0\n                -setsIgnoreFilters 1\n                -containersIgnoreFilters 0\n                -editAttrName 0\n                -showAttrValues 0\n                -highlightSecondary 0\n                -showUVAttrsOnly 0\n                -showTextureNodesOnly 0\n                -attrAlphaOrder \"default\" \n                -animLayerFilterOptions \"allAffecting\" \n                -sortOrder \"none\" \n                -longNames 0\n                -niceNames 1\n                -showNamespace 1\n                -showPinIcons 0\n                $editorName;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\toutlinerPanel -edit -l (localizedPanelLabel(\"Outliner\")) -mbv $menusOkayInPanels  $panelName;\n"
		+ "\t\t$editorName = $panelName;\n        outlinerEditor -e \n            -showShapes 1\n            -showAttributes 0\n            -showConnected 0\n            -showAnimCurvesOnly 0\n            -showMuteInfo 0\n            -organizeByLayer 1\n            -showAnimLayerWeight 1\n            -autoExpandLayers 1\n            -autoExpand 0\n            -showDagOnly 1\n            -showAssets 1\n            -showContainedOnly 1\n            -showPublishedAsConnected 0\n            -showContainerContents 1\n            -ignoreDagHierarchy 0\n            -expandConnections 0\n            -showUpstreamCurves 1\n            -showUnitlessCurves 1\n            -showCompounds 1\n            -showLeafs 1\n            -showNumericAttrsOnly 0\n            -highlightActive 1\n            -autoSelectNewObjects 0\n            -doNotSelectNewObjects 0\n            -dropIsParent 1\n            -transmitFilters 0\n            -setFilter \"defaultSetFilter\" \n            -showSetMembers 1\n            -allowMultiSelection 1\n            -alwaysToggleSelect 0\n            -directSelect 0\n"
		+ "            -displayMode \"DAG\" \n            -expandObjects 0\n            -setsIgnoreFilters 1\n            -containersIgnoreFilters 0\n            -editAttrName 0\n            -showAttrValues 0\n            -highlightSecondary 0\n            -showUVAttrsOnly 0\n            -showTextureNodesOnly 0\n            -attrAlphaOrder \"default\" \n            -animLayerFilterOptions \"allAffecting\" \n            -sortOrder \"none\" \n            -longNames 0\n            -niceNames 1\n            -showNamespace 1\n            -showPinIcons 0\n            $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"graphEditor\" (localizedPanelLabel(\"Graph Editor\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"graphEditor\" -l (localizedPanelLabel(\"Graph Editor\")) -mbv $menusOkayInPanels `;\n\n\t\t\t$editorName = ($panelName+\"OutlineEd\");\n            outlinerEditor -e \n                -showShapes 1\n                -showAttributes 1\n"
		+ "                -showConnected 1\n                -showAnimCurvesOnly 1\n                -showMuteInfo 0\n                -organizeByLayer 1\n                -showAnimLayerWeight 1\n                -autoExpandLayers 1\n                -autoExpand 1\n                -showDagOnly 0\n                -showAssets 1\n                -showContainedOnly 0\n                -showPublishedAsConnected 0\n                -showContainerContents 0\n                -ignoreDagHierarchy 0\n                -expandConnections 1\n                -showUpstreamCurves 1\n                -showUnitlessCurves 1\n                -showCompounds 0\n                -showLeafs 1\n                -showNumericAttrsOnly 1\n                -highlightActive 0\n                -autoSelectNewObjects 1\n                -doNotSelectNewObjects 0\n                -dropIsParent 1\n                -transmitFilters 1\n                -setFilter \"0\" \n                -showSetMembers 0\n                -allowMultiSelection 1\n                -alwaysToggleSelect 0\n                -directSelect 0\n"
		+ "                -displayMode \"DAG\" \n                -expandObjects 0\n                -setsIgnoreFilters 1\n                -containersIgnoreFilters 0\n                -editAttrName 0\n                -showAttrValues 0\n                -highlightSecondary 0\n                -showUVAttrsOnly 0\n                -showTextureNodesOnly 0\n                -attrAlphaOrder \"default\" \n                -animLayerFilterOptions \"allAffecting\" \n                -sortOrder \"none\" \n                -longNames 0\n                -niceNames 1\n                -showNamespace 1\n                -showPinIcons 1\n                $editorName;\n\n\t\t\t$editorName = ($panelName+\"GraphEd\");\n            animCurveEditor -e \n                -displayKeys 1\n                -displayTangents 0\n                -displayActiveKeys 0\n                -displayActiveKeyTangents 1\n                -displayInfinities 0\n                -autoFit 0\n                -snapTime \"integer\" \n                -snapValue \"none\" \n                -showResults \"off\" \n                -showBufferCurves \"off\" \n"
		+ "                -smoothness \"fine\" \n                -resultSamples 1\n                -resultScreenSamples 0\n                -resultUpdate \"delayed\" \n                -showUpstreamCurves 1\n                -stackedCurves 0\n                -stackedCurvesMin -1\n                -stackedCurvesMax 1\n                -stackedCurvesSpace 0.2\n                -displayNormalized 0\n                -preSelectionHighlight 0\n                -constrainDrag 0\n                $editorName;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Graph Editor\")) -mbv $menusOkayInPanels  $panelName;\n\n\t\t\t$editorName = ($panelName+\"OutlineEd\");\n            outlinerEditor -e \n                -showShapes 1\n                -showAttributes 1\n                -showConnected 1\n                -showAnimCurvesOnly 1\n                -showMuteInfo 0\n                -organizeByLayer 1\n                -showAnimLayerWeight 1\n                -autoExpandLayers 1\n                -autoExpand 1\n                -showDagOnly 0\n"
		+ "                -showAssets 1\n                -showContainedOnly 0\n                -showPublishedAsConnected 0\n                -showContainerContents 0\n                -ignoreDagHierarchy 0\n                -expandConnections 1\n                -showUpstreamCurves 1\n                -showUnitlessCurves 1\n                -showCompounds 0\n                -showLeafs 1\n                -showNumericAttrsOnly 1\n                -highlightActive 0\n                -autoSelectNewObjects 1\n                -doNotSelectNewObjects 0\n                -dropIsParent 1\n                -transmitFilters 1\n                -setFilter \"0\" \n                -showSetMembers 0\n                -allowMultiSelection 1\n                -alwaysToggleSelect 0\n                -directSelect 0\n                -displayMode \"DAG\" \n                -expandObjects 0\n                -setsIgnoreFilters 1\n                -containersIgnoreFilters 0\n                -editAttrName 0\n                -showAttrValues 0\n                -highlightSecondary 0\n                -showUVAttrsOnly 0\n"
		+ "                -showTextureNodesOnly 0\n                -attrAlphaOrder \"default\" \n                -animLayerFilterOptions \"allAffecting\" \n                -sortOrder \"none\" \n                -longNames 0\n                -niceNames 1\n                -showNamespace 1\n                -showPinIcons 1\n                $editorName;\n\n\t\t\t$editorName = ($panelName+\"GraphEd\");\n            animCurveEditor -e \n                -displayKeys 1\n                -displayTangents 0\n                -displayActiveKeys 0\n                -displayActiveKeyTangents 1\n                -displayInfinities 0\n                -autoFit 0\n                -snapTime \"integer\" \n                -snapValue \"none\" \n                -showResults \"off\" \n                -showBufferCurves \"off\" \n                -smoothness \"fine\" \n                -resultSamples 1\n                -resultScreenSamples 0\n                -resultUpdate \"delayed\" \n                -showUpstreamCurves 1\n                -stackedCurves 0\n                -stackedCurvesMin -1\n                -stackedCurvesMax 1\n"
		+ "                -stackedCurvesSpace 0.2\n                -displayNormalized 0\n                -preSelectionHighlight 0\n                -constrainDrag 0\n                $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"dopeSheetPanel\" (localizedPanelLabel(\"Dope Sheet\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"dopeSheetPanel\" -l (localizedPanelLabel(\"Dope Sheet\")) -mbv $menusOkayInPanels `;\n\n\t\t\t$editorName = ($panelName+\"OutlineEd\");\n            outlinerEditor -e \n                -showShapes 1\n                -showAttributes 1\n                -showConnected 1\n                -showAnimCurvesOnly 1\n                -showMuteInfo 0\n                -organizeByLayer 1\n                -showAnimLayerWeight 1\n                -autoExpandLayers 1\n                -autoExpand 0\n                -showDagOnly 0\n                -showAssets 1\n                -showContainedOnly 0\n                -showPublishedAsConnected 0\n"
		+ "                -showContainerContents 0\n                -ignoreDagHierarchy 0\n                -expandConnections 1\n                -showUpstreamCurves 1\n                -showUnitlessCurves 0\n                -showCompounds 1\n                -showLeafs 1\n                -showNumericAttrsOnly 1\n                -highlightActive 0\n                -autoSelectNewObjects 0\n                -doNotSelectNewObjects 1\n                -dropIsParent 1\n                -transmitFilters 0\n                -setFilter \"0\" \n                -showSetMembers 0\n                -allowMultiSelection 1\n                -alwaysToggleSelect 0\n                -directSelect 0\n                -displayMode \"DAG\" \n                -expandObjects 0\n                -setsIgnoreFilters 1\n                -containersIgnoreFilters 0\n                -editAttrName 0\n                -showAttrValues 0\n                -highlightSecondary 0\n                -showUVAttrsOnly 0\n                -showTextureNodesOnly 0\n                -attrAlphaOrder \"default\" \n                -animLayerFilterOptions \"allAffecting\" \n"
		+ "                -sortOrder \"none\" \n                -longNames 0\n                -niceNames 1\n                -showNamespace 1\n                -showPinIcons 0\n                $editorName;\n\n\t\t\t$editorName = ($panelName+\"DopeSheetEd\");\n            dopeSheetEditor -e \n                -displayKeys 1\n                -displayTangents 0\n                -displayActiveKeys 0\n                -displayActiveKeyTangents 0\n                -displayInfinities 0\n                -autoFit 0\n                -snapTime \"integer\" \n                -snapValue \"none\" \n                -outliner \"dopeSheetPanel1OutlineEd\" \n                -showSummary 1\n                -showScene 0\n                -hierarchyBelow 0\n                -showTicks 1\n                -selectionWindow 0 0 0 0 \n                $editorName;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Dope Sheet\")) -mbv $menusOkayInPanels  $panelName;\n\n\t\t\t$editorName = ($panelName+\"OutlineEd\");\n            outlinerEditor -e \n                -showShapes 1\n"
		+ "                -showAttributes 1\n                -showConnected 1\n                -showAnimCurvesOnly 1\n                -showMuteInfo 0\n                -organizeByLayer 1\n                -showAnimLayerWeight 1\n                -autoExpandLayers 1\n                -autoExpand 0\n                -showDagOnly 0\n                -showAssets 1\n                -showContainedOnly 0\n                -showPublishedAsConnected 0\n                -showContainerContents 0\n                -ignoreDagHierarchy 0\n                -expandConnections 1\n                -showUpstreamCurves 1\n                -showUnitlessCurves 0\n                -showCompounds 1\n                -showLeafs 1\n                -showNumericAttrsOnly 1\n                -highlightActive 0\n                -autoSelectNewObjects 0\n                -doNotSelectNewObjects 1\n                -dropIsParent 1\n                -transmitFilters 0\n                -setFilter \"0\" \n                -showSetMembers 0\n                -allowMultiSelection 1\n                -alwaysToggleSelect 0\n"
		+ "                -directSelect 0\n                -displayMode \"DAG\" \n                -expandObjects 0\n                -setsIgnoreFilters 1\n                -containersIgnoreFilters 0\n                -editAttrName 0\n                -showAttrValues 0\n                -highlightSecondary 0\n                -showUVAttrsOnly 0\n                -showTextureNodesOnly 0\n                -attrAlphaOrder \"default\" \n                -animLayerFilterOptions \"allAffecting\" \n                -sortOrder \"none\" \n                -longNames 0\n                -niceNames 1\n                -showNamespace 1\n                -showPinIcons 0\n                $editorName;\n\n\t\t\t$editorName = ($panelName+\"DopeSheetEd\");\n            dopeSheetEditor -e \n                -displayKeys 1\n                -displayTangents 0\n                -displayActiveKeys 0\n                -displayActiveKeyTangents 0\n                -displayInfinities 0\n                -autoFit 0\n                -snapTime \"integer\" \n                -snapValue \"none\" \n                -outliner \"dopeSheetPanel1OutlineEd\" \n"
		+ "                -showSummary 1\n                -showScene 0\n                -hierarchyBelow 0\n                -showTicks 1\n                -selectionWindow 0 0 0 0 \n                $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"clipEditorPanel\" (localizedPanelLabel(\"Trax Editor\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"clipEditorPanel\" -l (localizedPanelLabel(\"Trax Editor\")) -mbv $menusOkayInPanels `;\n\n\t\t\t$editorName = clipEditorNameFromPanel($panelName);\n            clipEditor -e \n                -displayKeys 0\n                -displayTangents 0\n                -displayActiveKeys 0\n                -displayActiveKeyTangents 0\n                -displayInfinities 0\n                -autoFit 0\n                -snapTime \"none\" \n                -snapValue \"none\" \n                -manageSequencer 0 \n                $editorName;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n"
		+ "\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Trax Editor\")) -mbv $menusOkayInPanels  $panelName;\n\n\t\t\t$editorName = clipEditorNameFromPanel($panelName);\n            clipEditor -e \n                -displayKeys 0\n                -displayTangents 0\n                -displayActiveKeys 0\n                -displayActiveKeyTangents 0\n                -displayInfinities 0\n                -autoFit 0\n                -snapTime \"none\" \n                -snapValue \"none\" \n                -manageSequencer 0 \n                $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"sequenceEditorPanel\" (localizedPanelLabel(\"Camera Sequencer\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"sequenceEditorPanel\" -l (localizedPanelLabel(\"Camera Sequencer\")) -mbv $menusOkayInPanels `;\n\n\t\t\t$editorName = sequenceEditorNameFromPanel($panelName);\n            clipEditor -e \n                -displayKeys 0\n"
		+ "                -displayTangents 0\n                -displayActiveKeys 0\n                -displayActiveKeyTangents 0\n                -displayInfinities 0\n                -autoFit 0\n                -snapTime \"none\" \n                -snapValue \"none\" \n                -manageSequencer 1 \n                $editorName;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Camera Sequencer\")) -mbv $menusOkayInPanels  $panelName;\n\n\t\t\t$editorName = sequenceEditorNameFromPanel($panelName);\n            clipEditor -e \n                -displayKeys 0\n                -displayTangents 0\n                -displayActiveKeys 0\n                -displayActiveKeyTangents 0\n                -displayInfinities 0\n                -autoFit 0\n                -snapTime \"none\" \n                -snapValue \"none\" \n                -manageSequencer 1 \n                $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"hyperGraphPanel\" (localizedPanelLabel(\"Hypergraph Hierarchy\")) `;\n"
		+ "\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"hyperGraphPanel\" -l (localizedPanelLabel(\"Hypergraph Hierarchy\")) -mbv $menusOkayInPanels `;\n\n\t\t\t$editorName = ($panelName+\"HyperGraphEd\");\n            hyperGraph -e \n                -graphLayoutStyle \"hierarchicalLayout\" \n                -orientation \"horiz\" \n                -mergeConnections 0\n                -zoom 1\n                -animateTransition 0\n                -showRelationships 1\n                -showShapes 0\n                -showDeformers 0\n                -showExpressions 0\n                -showConstraints 0\n                -showUnderworld 0\n                -showInvisible 0\n                -transitionFrames 1\n                -opaqueContainers 0\n                -freeform 0\n                -imagePosition 0 0 \n                -imageScale 1\n                -imageEnabled 0\n                -graphType \"DAG\" \n                -heatMapDisplay 0\n                -updateSelection 1\n                -updateNodeAdded 1\n"
		+ "                -useDrawOverrideColor 0\n                -limitGraphTraversal -1\n                -range 0 0 \n                -iconSize \"smallIcons\" \n                -showCachedConnections 0\n                $editorName;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Hypergraph Hierarchy\")) -mbv $menusOkayInPanels  $panelName;\n\n\t\t\t$editorName = ($panelName+\"HyperGraphEd\");\n            hyperGraph -e \n                -graphLayoutStyle \"hierarchicalLayout\" \n                -orientation \"horiz\" \n                -mergeConnections 0\n                -zoom 1\n                -animateTransition 0\n                -showRelationships 1\n                -showShapes 0\n                -showDeformers 0\n                -showExpressions 0\n                -showConstraints 0\n                -showUnderworld 0\n                -showInvisible 0\n                -transitionFrames 1\n                -opaqueContainers 0\n                -freeform 0\n                -imagePosition 0 0 \n                -imageScale 1\n"
		+ "                -imageEnabled 0\n                -graphType \"DAG\" \n                -heatMapDisplay 0\n                -updateSelection 1\n                -updateNodeAdded 1\n                -useDrawOverrideColor 0\n                -limitGraphTraversal -1\n                -range 0 0 \n                -iconSize \"smallIcons\" \n                -showCachedConnections 0\n                $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"hyperShadePanel\" (localizedPanelLabel(\"Hypershade\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"hyperShadePanel\" -l (localizedPanelLabel(\"Hypershade\")) -mbv $menusOkayInPanels `;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Hypershade\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"visorPanel\" (localizedPanelLabel(\"Visor\")) `;\n"
		+ "\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"visorPanel\" -l (localizedPanelLabel(\"Visor\")) -mbv $menusOkayInPanels `;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Visor\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"polyTexturePlacementPanel\" (localizedPanelLabel(\"UV Texture Editor\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"polyTexturePlacementPanel\" -l (localizedPanelLabel(\"UV Texture Editor\")) -mbv $menusOkayInPanels `;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"UV Texture Editor\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"renderWindowPanel\" (localizedPanelLabel(\"Render View\")) `;\n"
		+ "\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"renderWindowPanel\" -l (localizedPanelLabel(\"Render View\")) -mbv $menusOkayInPanels `;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Render View\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextPanel \"blendShapePanel\" (localizedPanelLabel(\"Blend Shape\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\tblendShapePanel -unParent -l (localizedPanelLabel(\"Blend Shape\")) -mbv $menusOkayInPanels ;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tblendShapePanel -edit -l (localizedPanelLabel(\"Blend Shape\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"dynRelEdPanel\" (localizedPanelLabel(\"Dynamic Relationships\")) `;\n\tif (\"\" == $panelName) {\n"
		+ "\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"dynRelEdPanel\" -l (localizedPanelLabel(\"Dynamic Relationships\")) -mbv $menusOkayInPanels `;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Dynamic Relationships\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"relationshipPanel\" (localizedPanelLabel(\"Relationship Editor\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"relationshipPanel\" -l (localizedPanelLabel(\"Relationship Editor\")) -mbv $menusOkayInPanels `;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Relationship Editor\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"referenceEditorPanel\" (localizedPanelLabel(\"Reference Editor\")) `;\n"
		+ "\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"referenceEditorPanel\" -l (localizedPanelLabel(\"Reference Editor\")) -mbv $menusOkayInPanels `;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Reference Editor\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"componentEditorPanel\" (localizedPanelLabel(\"Component Editor\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"componentEditorPanel\" -l (localizedPanelLabel(\"Component Editor\")) -mbv $menusOkayInPanels `;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Component Editor\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"dynPaintScriptedPanelType\" (localizedPanelLabel(\"Paint Effects\")) `;\n"
		+ "\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"dynPaintScriptedPanelType\" -l (localizedPanelLabel(\"Paint Effects\")) -mbv $menusOkayInPanels `;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Paint Effects\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"scriptEditorPanel\" (localizedPanelLabel(\"Script Editor\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"scriptEditorPanel\" -l (localizedPanelLabel(\"Script Editor\")) -mbv $menusOkayInPanels `;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Script Editor\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"Stereo\" (localizedPanelLabel(\"Stereo\")) `;\n"
		+ "\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"Stereo\" -l (localizedPanelLabel(\"Stereo\")) -mbv $menusOkayInPanels `;\nstring $editorName = ($panelName+\"Editor\");\n            stereoCameraView -e \n                -camera \"persp\" \n                -useInteractiveMode 0\n                -displayLights \"default\" \n                -displayAppearance \"wireframe\" \n                -activeOnly 0\n                -ignorePanZoom 0\n                -wireframeOnShaded 0\n                -headsUpDisplay 1\n                -selectionHiliteDisplay 1\n                -useDefaultMaterial 0\n                -bufferMode \"double\" \n                -twoSidedLighting 1\n                -backfaceCulling 0\n                -xray 0\n                -jointXray 0\n                -activeComponentsXray 0\n                -displayTextures 0\n                -smoothWireframe 0\n                -lineWidth 1\n                -textureAnisotropic 0\n                -textureHilight 1\n                -textureSampling 2\n"
		+ "                -textureDisplay \"modulate\" \n                -textureMaxSize 16384\n                -fogging 0\n                -fogSource \"fragment\" \n                -fogMode \"linear\" \n                -fogStart 0\n                -fogEnd 100\n                -fogDensity 0.1\n                -fogColor 0.5 0.5 0.5 1 \n                -maxConstantTransparency 1\n                -colorResolution 4 4 \n                -bumpResolution 4 4 \n                -textureCompression 0\n                -transparencyAlgorithm \"frontAndBackCull\" \n                -transpInShadows 0\n                -cullingOverride \"none\" \n                -lowQualityLighting 0\n                -maximumNumHardwareLights 0\n                -occlusionCulling 0\n                -shadingModel 0\n                -useBaseRenderer 0\n                -useReducedRenderer 0\n                -smallObjectCulling 0\n                -smallObjectThreshold -1 \n                -interactiveDisableShadows 0\n                -interactiveBackFaceCull 0\n                -sortTransparent 1\n"
		+ "                -nurbsCurves 1\n                -nurbsSurfaces 1\n                -polymeshes 1\n                -subdivSurfaces 1\n                -planes 1\n                -lights 1\n                -cameras 1\n                -controlVertices 1\n                -hulls 1\n                -grid 1\n                -joints 1\n                -ikHandles 1\n                -deformers 1\n                -dynamics 1\n                -fluids 1\n                -hairSystems 1\n                -follicles 1\n                -nCloths 1\n                -nParticles 1\n                -nRigids 1\n                -dynamicConstraints 1\n                -locators 1\n                -manipulators 1\n                -dimensions 1\n                -handles 1\n                -pivots 1\n                -textures 1\n                -strokes 1\n                -shadows 0\n                -displayMode \"centerEye\" \n                -viewColor 0 0 0 1 \n                $editorName;\nstereoCameraView -e -viewSelected 0 $editorName;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n"
		+ "\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Stereo\")) -mbv $menusOkayInPanels  $panelName;\nstring $editorName = ($panelName+\"Editor\");\n            stereoCameraView -e \n                -camera \"persp\" \n                -useInteractiveMode 0\n                -displayLights \"default\" \n                -displayAppearance \"wireframe\" \n                -activeOnly 0\n                -ignorePanZoom 0\n                -wireframeOnShaded 0\n                -headsUpDisplay 1\n                -selectionHiliteDisplay 1\n                -useDefaultMaterial 0\n                -bufferMode \"double\" \n                -twoSidedLighting 1\n                -backfaceCulling 0\n                -xray 0\n                -jointXray 0\n                -activeComponentsXray 0\n                -displayTextures 0\n                -smoothWireframe 0\n                -lineWidth 1\n                -textureAnisotropic 0\n                -textureHilight 1\n                -textureSampling 2\n                -textureDisplay \"modulate\" \n                -textureMaxSize 16384\n"
		+ "                -fogging 0\n                -fogSource \"fragment\" \n                -fogMode \"linear\" \n                -fogStart 0\n                -fogEnd 100\n                -fogDensity 0.1\n                -fogColor 0.5 0.5 0.5 1 \n                -maxConstantTransparency 1\n                -colorResolution 4 4 \n                -bumpResolution 4 4 \n                -textureCompression 0\n                -transparencyAlgorithm \"frontAndBackCull\" \n                -transpInShadows 0\n                -cullingOverride \"none\" \n                -lowQualityLighting 0\n                -maximumNumHardwareLights 0\n                -occlusionCulling 0\n                -shadingModel 0\n                -useBaseRenderer 0\n                -useReducedRenderer 0\n                -smallObjectCulling 0\n                -smallObjectThreshold -1 \n                -interactiveDisableShadows 0\n                -interactiveBackFaceCull 0\n                -sortTransparent 1\n                -nurbsCurves 1\n                -nurbsSurfaces 1\n                -polymeshes 1\n"
		+ "                -subdivSurfaces 1\n                -planes 1\n                -lights 1\n                -cameras 1\n                -controlVertices 1\n                -hulls 1\n                -grid 1\n                -joints 1\n                -ikHandles 1\n                -deformers 1\n                -dynamics 1\n                -fluids 1\n                -hairSystems 1\n                -follicles 1\n                -nCloths 1\n                -nParticles 1\n                -nRigids 1\n                -dynamicConstraints 1\n                -locators 1\n                -manipulators 1\n                -dimensions 1\n                -handles 1\n                -pivots 1\n                -textures 1\n                -strokes 1\n                -shadows 0\n                -displayMode \"centerEye\" \n                -viewColor 0 0 0 1 \n                $editorName;\nstereoCameraView -e -viewSelected 0 $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\tif ($useSceneConfig) {\n        string $configName = `getPanel -cwl (localizedPanelLabel(\"Current Layout\"))`;\n"
		+ "        if (\"\" != $configName) {\n\t\t\tpanelConfiguration -edit -label (localizedPanelLabel(\"Current Layout\")) \n\t\t\t\t-defaultImage \"\"\n\t\t\t\t-image \"\"\n\t\t\t\t-sc false\n\t\t\t\t-configString \"global string $gMainPane; paneLayout -e -cn \\\"single\\\" -ps 1 100 100 $gMainPane;\"\n\t\t\t\t-removeAllPanels\n\t\t\t\t-ap false\n\t\t\t\t\t(localizedPanelLabel(\"Persp View\")) \n\t\t\t\t\t\"modelPanel\"\n"
		+ "\t\t\t\t\t\"$panelName = `modelPanel -unParent -l (localizedPanelLabel(\\\"Persp View\\\")) -mbv $menusOkayInPanels `;\\n$editorName = $panelName;\\nmodelEditor -e \\n    -cam `findStartUpCamera persp` \\n    -useInteractiveMode 0\\n    -displayLights \\\"default\\\" \\n    -displayAppearance \\\"wireframe\\\" \\n    -activeOnly 0\\n    -ignorePanZoom 0\\n    -wireframeOnShaded 0\\n    -headsUpDisplay 1\\n    -selectionHiliteDisplay 1\\n    -useDefaultMaterial 0\\n    -bufferMode \\\"double\\\" \\n    -twoSidedLighting 1\\n    -backfaceCulling 0\\n    -xray 0\\n    -jointXray 0\\n    -activeComponentsXray 0\\n    -displayTextures 0\\n    -smoothWireframe 0\\n    -lineWidth 1\\n    -textureAnisotropic 0\\n    -textureHilight 1\\n    -textureSampling 2\\n    -textureDisplay \\\"modulate\\\" \\n    -textureMaxSize 16384\\n    -fogging 0\\n    -fogSource \\\"fragment\\\" \\n    -fogMode \\\"linear\\\" \\n    -fogStart 0\\n    -fogEnd 100\\n    -fogDensity 0.1\\n    -fogColor 0.5 0.5 0.5 1 \\n    -maxConstantTransparency 1\\n    -rendererName \\\"base_OpenGL_Renderer\\\" \\n    -colorResolution 256 256 \\n    -bumpResolution 512 512 \\n    -textureCompression 0\\n    -transparencyAlgorithm \\\"frontAndBackCull\\\" \\n    -transpInShadows 0\\n    -cullingOverride \\\"none\\\" \\n    -lowQualityLighting 0\\n    -maximumNumHardwareLights 1\\n    -occlusionCulling 0\\n    -shadingModel 0\\n    -useBaseRenderer 0\\n    -useReducedRenderer 0\\n    -smallObjectCulling 0\\n    -smallObjectThreshold -1 \\n    -interactiveDisableShadows 0\\n    -interactiveBackFaceCull 0\\n    -sortTransparent 1\\n    -nurbsCurves 1\\n    -nurbsSurfaces 1\\n    -polymeshes 1\\n    -subdivSurfaces 1\\n    -planes 1\\n    -lights 1\\n    -cameras 1\\n    -controlVertices 1\\n    -hulls 1\\n    -grid 1\\n    -joints 1\\n    -ikHandles 1\\n    -deformers 1\\n    -dynamics 1\\n    -fluids 1\\n    -hairSystems 1\\n    -follicles 1\\n    -nCloths 1\\n    -nParticles 1\\n    -nRigids 1\\n    -dynamicConstraints 1\\n    -locators 1\\n    -manipulators 1\\n    -dimensions 1\\n    -handles 1\\n    -pivots 1\\n    -textures 1\\n    -strokes 1\\n    -shadows 0\\n    $editorName;\\nmodelEditor -e -viewSelected 0 $editorName\"\n"
		+ "\t\t\t\t\t\"modelPanel -edit -l (localizedPanelLabel(\\\"Persp View\\\")) -mbv $menusOkayInPanels  $panelName;\\n$editorName = $panelName;\\nmodelEditor -e \\n    -cam `findStartUpCamera persp` \\n    -useInteractiveMode 0\\n    -displayLights \\\"default\\\" \\n    -displayAppearance \\\"wireframe\\\" \\n    -activeOnly 0\\n    -ignorePanZoom 0\\n    -wireframeOnShaded 0\\n    -headsUpDisplay 1\\n    -selectionHiliteDisplay 1\\n    -useDefaultMaterial 0\\n    -bufferMode \\\"double\\\" \\n    -twoSidedLighting 1\\n    -backfaceCulling 0\\n    -xray 0\\n    -jointXray 0\\n    -activeComponentsXray 0\\n    -displayTextures 0\\n    -smoothWireframe 0\\n    -lineWidth 1\\n    -textureAnisotropic 0\\n    -textureHilight 1\\n    -textureSampling 2\\n    -textureDisplay \\\"modulate\\\" \\n    -textureMaxSize 16384\\n    -fogging 0\\n    -fogSource \\\"fragment\\\" \\n    -fogMode \\\"linear\\\" \\n    -fogStart 0\\n    -fogEnd 100\\n    -fogDensity 0.1\\n    -fogColor 0.5 0.5 0.5 1 \\n    -maxConstantTransparency 1\\n    -rendererName \\\"base_OpenGL_Renderer\\\" \\n    -colorResolution 256 256 \\n    -bumpResolution 512 512 \\n    -textureCompression 0\\n    -transparencyAlgorithm \\\"frontAndBackCull\\\" \\n    -transpInShadows 0\\n    -cullingOverride \\\"none\\\" \\n    -lowQualityLighting 0\\n    -maximumNumHardwareLights 1\\n    -occlusionCulling 0\\n    -shadingModel 0\\n    -useBaseRenderer 0\\n    -useReducedRenderer 0\\n    -smallObjectCulling 0\\n    -smallObjectThreshold -1 \\n    -interactiveDisableShadows 0\\n    -interactiveBackFaceCull 0\\n    -sortTransparent 1\\n    -nurbsCurves 1\\n    -nurbsSurfaces 1\\n    -polymeshes 1\\n    -subdivSurfaces 1\\n    -planes 1\\n    -lights 1\\n    -cameras 1\\n    -controlVertices 1\\n    -hulls 1\\n    -grid 1\\n    -joints 1\\n    -ikHandles 1\\n    -deformers 1\\n    -dynamics 1\\n    -fluids 1\\n    -hairSystems 1\\n    -follicles 1\\n    -nCloths 1\\n    -nParticles 1\\n    -nRigids 1\\n    -dynamicConstraints 1\\n    -locators 1\\n    -manipulators 1\\n    -dimensions 1\\n    -handles 1\\n    -pivots 1\\n    -textures 1\\n    -strokes 1\\n    -shadows 0\\n    $editorName;\\nmodelEditor -e -viewSelected 0 $editorName\"\n"
		+ "\t\t\t\t$configName;\n\n            setNamedPanelLayout (localizedPanelLabel(\"Current Layout\"));\n        }\n\n        panelHistory -e -clear mainPanelHistory;\n        setFocus `paneLayout -q -p1 $gMainPane`;\n        sceneUIReplacement -deleteRemaining;\n        sceneUIReplacement -clear;\n\t}\n\n\ngrid -spacing 5 -size 12 -divisions 5 -displayAxes yes -displayGridLines yes -displayDivisionLines yes -displayPerspectiveLabels no -displayOrthographicLabels no -displayAxesBold yes -perspectiveLabelPosition axis -orthographicLabelPosition edge;\nviewManip -drawCompass 0 -compassAngle 0 -frontParameters \"\" -homeParameters \"\" -selectionLockParameters \"\";\n}\n");
	setAttr ".st" 3;
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
	addAttr -ci true -sn "gamma" -ln "gamma" -dv 2.2000000476837158 -min 9.9999997473787516e-005 
		-smx 5 -at "float";
	addAttr -ci true -sn "dither_amplitude" -ln "ditherAmplitude" -dv 1 -at "float";
	addAttr -ci true -sn "png_format" -ln "pngFormat" -min 0 -max 1 -en "int8:int16" 
		-at "enum";
	addAttr -ci true -sn "tiff_compression" -ln "tiffCompression" -dv 1 -min 0 -max 
		4 -en "none:lzw:ccittrle:zip:packbits" -at "enum";
	addAttr -ci true -sn "tiff_format" -ln "tiffFormat" -min 0 -max 3 -en "int8:int16:int32:float32" 
		-at "enum";
	addAttr -ci true -sn "unpremult_alpha" -ln "unpremultAlpha" -min 0 -max 1 -at "bool";
	setAttr ".ai_translator" -type "string" "tif";
	setAttr ".tiled" no;
createNode polyCylinder -n "reference:polyCylinder1";
	setAttr ".h" 10;
	setAttr ".sh" 10;
	setAttr ".sc" 1;
	setAttr ".cuv" 3;
createNode nonLinear -n "reference:bend1";
	addAttr -is true -ci true -k true -sn "cur" -ln "curvature" -smn -4 -smx 4 -at "double";
	addAttr -is true -ci true -k true -sn "lb" -ln "lowBound" -dv -1 -max 0 -smn -10 
		-smx 0 -at "double";
	addAttr -is true -ci true -k true -sn "hb" -ln "highBound" -dv 1 -min 0 -smn 0 -smx 
		10 -at "double";
	setAttr -k on ".cur";
	setAttr -k on ".lb" 0;
	setAttr -k on ".hb";
createNode tweak -n "reference:tweak1";
createNode objectSet -n "reference:bend1Set";
	addAttr -ci true -k true -sn "ai_user_options" -ln "aiUserOptions" -dt "string";
	addAttr -ci true -k true -sn "ai_override" -ln "aiOverride" -min 0 -max 1 -at "bool";
	setAttr ".ihi" 0;
	setAttr ".vo" yes;
createNode groupId -n "reference:bend1GroupId";
	setAttr ".ihi" 0;
createNode groupParts -n "reference:bend1GroupParts";
	setAttr ".ihi" 0;
	setAttr ".ic" -type "componentList" 1 "vtx[*]";
createNode objectSet -n "reference:tweakSet1";
	addAttr -ci true -k true -sn "ai_user_options" -ln "aiUserOptions" -dt "string";
	addAttr -ci true -k true -sn "ai_override" -ln "aiOverride" -min 0 -max 1 -at "bool";
	setAttr ".ihi" 0;
	setAttr ".vo" yes;
createNode groupId -n "reference:groupId2";
	setAttr ".ihi" 0;
createNode groupParts -n "reference:groupParts2";
	setAttr ".ihi" 0;
	setAttr ".ic" -type "componentList" 1 "vtx[*]";
createNode mentalrayItemsList -s -n "mentalrayItemsList";
createNode mentalrayGlobals -s -n "mentalrayGlobals";
	setAttr ".rvb" 3;
	setAttr ".ivb" no;
createNode mentalrayOptions -s -n "miDefaultOptions";
	addAttr -ci true -m -sn "stringOptions" -ln "stringOptions" -at "compound" -nc 
		3;
	addAttr -ci true -sn "name" -ln "name" -dt "string" -p "stringOptions";
	addAttr -ci true -sn "value" -ln "value" -dt "string" -p "stringOptions";
	addAttr -ci true -sn "type" -ln "type" -dt "string" -p "stringOptions";
	setAttr ".scan" 3;
	setAttr ".maxr" 200;
	setAttr ".shrd" 100;
	setAttr ".shmth" 3;
	setAttr -s 28 ".stringOptions";
	setAttr ".stringOptions[0].name" -type "string" "rast motion factor";
	setAttr ".stringOptions[0].value" -type "string" "1.0";
	setAttr ".stringOptions[0].type" -type "string" "scalar";
	setAttr ".stringOptions[1].name" -type "string" "rast transparency depth";
	setAttr ".stringOptions[1].value" -type "string" "40";
	setAttr ".stringOptions[1].type" -type "string" "integer";
	setAttr ".stringOptions[2].name" -type "string" "rast useopacity";
	setAttr ".stringOptions[2].value" -type "string" "true";
	setAttr ".stringOptions[2].type" -type "string" "boolean";
	setAttr ".stringOptions[3].name" -type "string" "importon";
	setAttr ".stringOptions[3].value" -type "string" "false";
	setAttr ".stringOptions[3].type" -type "string" "boolean";
	setAttr ".stringOptions[4].name" -type "string" "importon density";
	setAttr ".stringOptions[4].value" -type "string" "1.0";
	setAttr ".stringOptions[4].type" -type "string" "scalar";
	setAttr ".stringOptions[5].name" -type "string" "importon merge";
	setAttr ".stringOptions[5].value" -type "string" "0.0";
	setAttr ".stringOptions[5].type" -type "string" "scalar";
	setAttr ".stringOptions[6].name" -type "string" "importon trace depth";
	setAttr ".stringOptions[6].value" -type "string" "0";
	setAttr ".stringOptions[6].type" -type "string" "integer";
	setAttr ".stringOptions[7].name" -type "string" "importon traverse";
	setAttr ".stringOptions[7].value" -type "string" "true";
	setAttr ".stringOptions[7].type" -type "string" "boolean";
	setAttr ".stringOptions[8].name" -type "string" "shadowmap pixel samples";
	setAttr ".stringOptions[8].value" -type "string" "3";
	setAttr ".stringOptions[8].type" -type "string" "integer";
	setAttr ".stringOptions[9].name" -type "string" "ambient occlusion";
	setAttr ".stringOptions[9].value" -type "string" "false";
	setAttr ".stringOptions[9].type" -type "string" "boolean";
	setAttr ".stringOptions[10].name" -type "string" "ambient occlusion rays";
	setAttr ".stringOptions[10].value" -type "string" "256";
	setAttr ".stringOptions[10].type" -type "string" "integer";
	setAttr ".stringOptions[11].name" -type "string" "ambient occlusion cache";
	setAttr ".stringOptions[11].value" -type "string" "false";
	setAttr ".stringOptions[11].type" -type "string" "boolean";
	setAttr ".stringOptions[12].name" -type "string" "ambient occlusion cache density";
	setAttr ".stringOptions[12].value" -type "string" "1.0";
	setAttr ".stringOptions[12].type" -type "string" "scalar";
	setAttr ".stringOptions[13].name" -type "string" "ambient occlusion cache points";
	setAttr ".stringOptions[13].value" -type "string" "64";
	setAttr ".stringOptions[13].type" -type "string" "integer";
	setAttr ".stringOptions[14].name" -type "string" "irradiance particles";
	setAttr ".stringOptions[14].value" -type "string" "false";
	setAttr ".stringOptions[14].type" -type "string" "boolean";
	setAttr ".stringOptions[15].name" -type "string" "irradiance particles rays";
	setAttr ".stringOptions[15].value" -type "string" "256";
	setAttr ".stringOptions[15].type" -type "string" "integer";
	setAttr ".stringOptions[16].name" -type "string" "irradiance particles interpolate";
	setAttr ".stringOptions[16].value" -type "string" "1";
	setAttr ".stringOptions[16].type" -type "string" "integer";
	setAttr ".stringOptions[17].name" -type "string" "irradiance particles interppoints";
	setAttr ".stringOptions[17].value" -type "string" "64";
	setAttr ".stringOptions[17].type" -type "string" "integer";
	setAttr ".stringOptions[18].name" -type "string" "irradiance particles indirect passes";
	setAttr ".stringOptions[18].value" -type "string" "0";
	setAttr ".stringOptions[18].type" -type "string" "integer";
	setAttr ".stringOptions[19].name" -type "string" "irradiance particles scale";
	setAttr ".stringOptions[19].value" -type "string" "1.0";
	setAttr ".stringOptions[19].type" -type "string" "scalar";
	setAttr ".stringOptions[20].name" -type "string" "irradiance particles env";
	setAttr ".stringOptions[20].value" -type "string" "true";
	setAttr ".stringOptions[20].type" -type "string" "boolean";
	setAttr ".stringOptions[21].name" -type "string" "irradiance particles env rays";
	setAttr ".stringOptions[21].value" -type "string" "256";
	setAttr ".stringOptions[21].type" -type "string" "integer";
	setAttr ".stringOptions[22].name" -type "string" "irradiance particles env scale";
	setAttr ".stringOptions[22].value" -type "string" "1";
	setAttr ".stringOptions[22].type" -type "string" "integer";
	setAttr ".stringOptions[23].name" -type "string" "irradiance particles rebuild";
	setAttr ".stringOptions[23].value" -type "string" "true";
	setAttr ".stringOptions[23].type" -type "string" "boolean";
	setAttr ".stringOptions[24].name" -type "string" "irradiance particles file";
	setAttr ".stringOptions[24].value" -type "string" "";
	setAttr ".stringOptions[24].type" -type "string" "string";
	setAttr ".stringOptions[25].name" -type "string" "geom displace motion factor";
	setAttr ".stringOptions[25].value" -type "string" "1.0";
	setAttr ".stringOptions[25].type" -type "string" "scalar";
	setAttr ".stringOptions[26].name" -type "string" "contrast all buffers";
	setAttr ".stringOptions[26].value" -type "string" "true";
	setAttr ".stringOptions[26].type" -type "string" "boolean";
	setAttr ".stringOptions[27].name" -type "string" "finalgather normal tolerance";
	setAttr ".stringOptions[27].value" -type "string" "25.842";
	setAttr ".stringOptions[27].type" -type "string" "scalar";
createNode mentalrayFramebuffer -s -n "miDefaultFramebuffer";
createNode animCurveTU -n "reference:bend1_curvature";
	setAttr ".tan" 3;
	setAttr ".wgt" no;
	setAttr -s 3 ".ktv[0:2]"  0.96 0 9.6 3.1 19.2 0;
createNode shadingEngine -n "reference:blinn1SG";
	addAttr -ci true -k true -sn "ai_user_options" -ln "aiUserOptions" -dt "string";
	addAttr -ci true -m -sn "aovs" -ln "aiCustomAOVs" -at "compound" -nc 2;
	addAttr -ci true -k true -sn "aov_name" -ln "aovName" -dt "string" -p "aiCustomAOVs";
	addAttr -ci true -k true -sn "aov_input" -ln "aovInput" -at "message" -p "aiCustomAOVs";
	setAttr ".ihi" 0;
	setAttr -s 2 ".dsm";
	setAttr ".ro" yes;
createNode materialInfo -n "reference:materialInfo2";
createNode solidFractal -n "reference:solidFractal1";
	setAttr ".ail" yes;
	setAttr ".ra" 1;
createNode lambert -n "lambert2";
select -ne :time1;
	setAttr ".o" 10;
	setAttr ".unw" 10;
select -ne :renderPartition;
	setAttr -s 5 ".st";
select -ne :initialShadingGroup;
	addAttr -ci true -k true -sn "ai_user_options" -ln "aiUserOptions" -dt "string";
	addAttr -ci true -m -sn "aovs" -ln "aiCustomAOVs" -at "compound" -nc 2;
	addAttr -ci true -k true -sn "aov_name" -ln "aovName" -dt "string" -p "aiCustomAOVs";
	addAttr -ci true -k true -sn "aov_input" -ln "aovInput" -at "message" -p "aiCustomAOVs";
	setAttr ".ro" yes;
select -ne :initialParticleSE;
	addAttr -ci true -k true -sn "ai_user_options" -ln "aiUserOptions" -dt "string";
	addAttr -ci true -m -sn "aovs" -ln "aiCustomAOVs" -at "compound" -nc 2;
	addAttr -ci true -k true -sn "aov_name" -ln "aovName" -dt "string" -p "aiCustomAOVs";
	addAttr -ci true -k true -sn "aov_input" -ln "aovInput" -at "message" -p "aiCustomAOVs";
	setAttr ".ro" yes;
select -ne :defaultShaderList1;
	setAttr -s 4 ".s";
select -ne :defaultTextureList1;
select -ne :lightList1;
select -ne :postProcessList1;
	setAttr -s 2 ".p";
select -ne :defaultRenderUtilityList1;
select -ne :renderGlobalsList1;
select -ne :defaultRenderGlobals;
	setAttr ".ren" -type "string" "arnold";
	setAttr ".outf" 51;
	setAttr ".imfkey" -type "string" "tif";
	setAttr ".ifp" -type "string" "testrender";
select -ne :defaultResolution;
	setAttr ".w" 160;
	setAttr ".h" 120;
	setAttr ".pa" 1;
	setAttr ".dar" 1.3333333730697632;
select -ne :defaultLightSet;
	addAttr -ci true -k true -sn "ai_user_options" -ln "aiUserOptions" -dt "string";
	addAttr -ci true -k true -sn "ai_override" -ln "aiOverride" -min 0 -max 1 -at "bool";
select -ne :hardwareRenderGlobals;
	setAttr ".ctrs" 256;
	setAttr ".btrs" 512;
select -ne :defaultHardwareRenderGlobals;
	setAttr ".fn" -type "string" "im";
	setAttr ".res" -type "string" "ntsc_4d 646 485 1.333";
connectAttr "reference:bend1GroupId.id" "reference:pCylinderShape1.iog.og[0].gid"
		;
connectAttr "reference:bend1Set.mwc" "reference:pCylinderShape1.iog.og[0].gco";
connectAttr "reference:groupId2.id" "reference:pCylinderShape1.iog.og[1].gid";
connectAttr "reference:tweakSet1.mwc" "reference:pCylinderShape1.iog.og[1].gco";
connectAttr "reference:bend1.og[0]" "reference:pCylinderShape1.i";
connectAttr "reference:tweak1.vl[0].vt[0]" "reference:pCylinderShape1.twl";
connectAttr "reference:pCylinderShape1_reference.msg" "reference:pCylinderShape1.rob"
		;
connectAttr "reference:polyCylinder1.out" "reference:pCylinderShape1Orig.i";
connectAttr "reference:bend1.msg" "reference:bend1Handle.sml";
connectAttr "reference:bend1.cur" "reference:bend1HandleShape.cur";
connectAttr "reference:bend1.lb" "reference:bend1HandleShape.lb";
connectAttr "reference:bend1.hb" "reference:bend1HandleShape.hb";
connectAttr ":time1.o" ":defaultArnoldRenderOptions.aaseed";
connectAttr ":defaultArnoldFilter.msg" ":defaultArnoldRenderOptions.filt";
connectAttr ":defaultArnoldDriver.msg" ":defaultArnoldRenderOptions.drvr";
relationship "link" ":lightLinker1" ":initialShadingGroup.message" ":defaultLightSet.message";
relationship "link" ":lightLinker1" ":initialParticleSE.message" ":defaultLightSet.message";
relationship "link" ":lightLinker1" "ArnoldStandardShader1SG.message" ":defaultLightSet.message";
relationship "link" ":lightLinker1" "ArnoldStandardShader1SG1.message" ":defaultLightSet.message";
relationship "link" ":lightLinker1" "reference:blinn1SG.message" ":defaultLightSet.message";
relationship "shadowLink" ":lightLinker1" ":initialShadingGroup.message" ":defaultLightSet.message";
relationship "shadowLink" ":lightLinker1" ":initialParticleSE.message" ":defaultLightSet.message";
relationship "shadowLink" ":lightLinker1" "ArnoldStandardShader1SG.message" ":defaultLightSet.message";
relationship "shadowLink" ":lightLinker1" "ArnoldStandardShader1SG1.message" ":defaultLightSet.message";
relationship "shadowLink" ":lightLinker1" "reference:blinn1SG.message" ":defaultLightSet.message";
connectAttr "layerManager.dli[0]" "defaultLayer.id";
connectAttr "renderLayerManager.rlmi[0]" "defaultRenderLayer.rlid";
connectAttr "ArnoldStandardShader1SG.msg" "materialInfo1.sg";
connectAttr "ArnoldStandardShader1.msg" "materialInfo1.m";
connectAttr "ArnoldStandardShader1.msg" "materialInfo1.t" -na;
connectAttr "ArnoldStandardShader1.out" "ArnoldStandardShader1SG1.ss";
connectAttr "ArnoldStandardShader1SG1.msg" "materialInfo2.sg";
connectAttr "ArnoldStandardShader1.msg" "materialInfo2.m";
connectAttr "ArnoldStandardShader1.msg" "materialInfo2.t" -na;
connectAttr "reference:bend1_curvature.o" "reference:bend1.cur";
connectAttr "reference:bend1GroupParts.og" "reference:bend1.ip[0].ig";
connectAttr "reference:bend1GroupId.id" "reference:bend1.ip[0].gi";
connectAttr "reference:bend1HandleShape.dd" "reference:bend1.dd";
connectAttr "reference:bend1Handle.wm" "reference:bend1.ma";
connectAttr "reference:groupParts2.og" "reference:tweak1.ip[0].ig";
connectAttr "reference:groupId2.id" "reference:tweak1.ip[0].gi";
connectAttr "reference:bend1GroupId.msg" "reference:bend1Set.gn" -na;
connectAttr "reference:pCylinderShape1.iog.og[0]" "reference:bend1Set.dsm" -na;
connectAttr "reference:bend1.msg" "reference:bend1Set.ub[0]";
connectAttr "reference:tweak1.og[0]" "reference:bend1GroupParts.ig";
connectAttr "reference:bend1GroupId.id" "reference:bend1GroupParts.gi";
connectAttr "reference:groupId2.msg" "reference:tweakSet1.gn" -na;
connectAttr "reference:pCylinderShape1.iog.og[1]" "reference:tweakSet1.dsm" -na;
connectAttr "reference:tweak1.msg" "reference:tweakSet1.ub[0]";
connectAttr "reference:pCylinderShape1Orig.w" "reference:groupParts2.ig";
connectAttr "reference:groupId2.id" "reference:groupParts2.gi";
connectAttr ":mentalrayGlobals.msg" ":mentalrayItemsList.glb";
connectAttr ":miDefaultOptions.msg" ":mentalrayItemsList.opt" -na;
connectAttr ":miDefaultFramebuffer.msg" ":mentalrayItemsList.fb" -na;
connectAttr ":miDefaultOptions.msg" ":mentalrayGlobals.opt";
connectAttr ":miDefaultFramebuffer.msg" ":mentalrayGlobals.fb";
connectAttr "lambert2.oc" "reference:blinn1SG.ss";
connectAttr "reference:pCylinderShape1.iog" "reference:blinn1SG.dsm" -na;
connectAttr "reference:pCylinderShape1_reference.iog" "reference:blinn1SG.dsm" -na
		;
connectAttr "reference:blinn1SG.msg" "reference:materialInfo2.sg";
connectAttr "lambert2.msg" "reference:materialInfo2.m";
connectAttr "reference:solidFractal1.msg" "reference:materialInfo2.t" -na;
connectAttr "reference:place3dTexture1.wim" "reference:solidFractal1.pm";
connectAttr "reference:solidFractal1.oc" "lambert2.c";
connectAttr "ArnoldStandardShader1SG.pa" ":renderPartition.st" -na;
connectAttr "ArnoldStandardShader1SG1.pa" ":renderPartition.st" -na;
connectAttr "reference:blinn1SG.pa" ":renderPartition.st" -na;
connectAttr "ArnoldStandardShader1.msg" ":defaultShaderList1.s" -na;
connectAttr "lambert2.msg" ":defaultShaderList1.s" -na;
connectAttr "reference:solidFractal1.msg" ":defaultTextureList1.tx" -na;
connectAttr "directionalLightShape1.ltd" ":lightList1.l" -na;
connectAttr "reference:place3dTexture1.msg" ":defaultRenderUtilityList1.u" -na;
connectAttr "directionalLight1.iog" ":defaultLightSet.dsm" -na;
// End of test.ma

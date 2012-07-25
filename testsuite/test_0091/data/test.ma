//Maya ASCII 2011 scene
//Name: test.ma
//Last modified: Wed, Jul 25, 2012 03:55:53 PM
//Codeset: UTF-8
requires maya "2011";
requires "mtoa" "0.18.0.dev";
requires "stereoCamera" "10.0";
currentUnit -l centimeter -a degree -t film;
fileInfo "application" "maya";
fileInfo "product" "Maya 2011";
fileInfo "version" "2011 x64";
fileInfo "cutIdentifier" "201009060248-781623";
fileInfo "osv" "Linux 3.4.6-2.fc17.x86_64 #1 SMP Thu Jul 19 22:54:16 UTC 2012 x86_64";
createNode transform -s -n "persp";
	addAttr -ci true -sn "lumaID" -ln "lumaID" -dt "string";
	setAttr ".v" no;
	setAttr ".t" -type "double3" -5.4357364090923888 4.0945451682370564 0.85817276042129309 ;
	setAttr ".r" -type "double3" -39.938352729856746 -90.999999999994543 0 ;
	setAttr ".rp" -type "double3" -3.5804692544161298e-15 -7.1054273576010019e-15 -1.4210854715202004e-14 ;
	setAttr ".rpt" -type "double3" -8.7783779362275568e-15 1.1498476670238778e-14 9.4020555375448207e-16 ;
	setAttr ".lumaID" -type "string" "a:1338571498.23:0.320735484449";
createNode camera -s -n "perspShape" -p "persp";
	addAttr -ci true -sn "ai_exposure" -ln "aiExposure" -min -100 -max 100 -smn -5 
		-smx 5 -at "float";
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
	addAttr -ci true -sn "lumaID" -ln "lumaID" -dt "string";
	setAttr -k off ".v" no;
	setAttr ".fl" 34.999999999999979;
	setAttr ".coi" 6.9317017759382997;
	setAttr ".imn" -type "string" "persp";
	setAttr ".den" -type "string" "persp_depth";
	setAttr ".man" -type "string" "persp_mask";
	setAttr ".hc" -type "string" "viewSet -p %camera";
	setAttr -k on ".ai_translator" -type "string" "perspective";
	setAttr ".lumaID" -type "string" "a:1338571498.23:0.72459433658";
createNode transform -s -n "top";
	addAttr -ci true -sn "lumaID" -ln "lumaID" -dt "string";
	setAttr ".v" no;
	setAttr ".t" -type "double3" 0 100.1 0 ;
	setAttr ".r" -type "double3" -89.999999999999986 0 0 ;
	setAttr ".lumaID" -type "string" "a:1338571498.23:0.853973786518";
createNode camera -s -n "topShape" -p "top";
	addAttr -ci true -sn "ai_exposure" -ln "aiExposure" -min -100 -max 100 -smn -5 
		-smx 5 -at "float";
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
	addAttr -ci true -sn "lumaID" -ln "lumaID" -dt "string";
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
	setAttr ".lumaID" -type "string" "a:1338571498.23:0.388952734123";
createNode transform -s -n "front";
	addAttr -ci true -sn "lumaID" -ln "lumaID" -dt "string";
	setAttr ".v" no;
	setAttr ".t" -type "double3" 0 0 100.1 ;
	setAttr ".lumaID" -type "string" "a:1338571498.23:0.020474217924";
createNode camera -s -n "frontShape" -p "front";
	addAttr -ci true -sn "ai_exposure" -ln "aiExposure" -min -100 -max 100 -smn -5 
		-smx 5 -at "float";
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
	addAttr -ci true -sn "lumaID" -ln "lumaID" -dt "string";
	setAttr -k off ".v" no;
	setAttr ".rnd" no;
	setAttr ".coi" 100.1;
	setAttr ".ow" 144.46730469819329;
	setAttr ".imn" -type "string" "front";
	setAttr ".den" -type "string" "front_depth";
	setAttr ".man" -type "string" "front_mask";
	setAttr ".hc" -type "string" "viewSet -f %camera";
	setAttr ".o" yes;
	setAttr -k on ".ai_translator" -type "string" "orthographic";
	setAttr ".lumaID" -type "string" "a:1338571498.23:0.406400237346";
createNode transform -s -n "side";
	addAttr -ci true -sn "lumaID" -ln "lumaID" -dt "string";
	setAttr ".v" no;
	setAttr ".t" -type "double3" 100.1 1.6871751536864319 12.910557697774426 ;
	setAttr ".r" -type "double3" 0 89.999999999999986 0 ;
	setAttr ".lumaID" -type "string" "a:1338571498.23:0.926548889195";
createNode camera -s -n "sideShape" -p "side";
	addAttr -ci true -sn "ai_exposure" -ln "aiExposure" -min -100 -max 100 -smn -5 
		-smx 5 -at "float";
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
	addAttr -ci true -sn "lumaID" -ln "lumaID" -dt "string";
	setAttr -k off ".v" no;
	setAttr ".rnd" no;
	setAttr ".coi" 100.1;
	setAttr ".ow" 51.715586232562316;
	setAttr ".imn" -type "string" "side";
	setAttr ".den" -type "string" "side_depth";
	setAttr ".man" -type "string" "side_mask";
	setAttr ".hc" -type "string" "viewSet -s %camera";
	setAttr ".o" yes;
	setAttr -k on ".ai_translator" -type "string" "orthographic";
	setAttr ".lumaID" -type "string" "a:1338571498.23:0.771390131536";
createNode transform -n "pPlane1";
	setAttr ".t" -type "double3" -0.78665086554556929 0 0.9243226907026969 ;
	setAttr ".s" -type "double3" 2.061064491222937 2.061064491222937 2.061064491222937 ;
createNode mesh -n "pPlaneShape1" -p "pPlane1";
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
	setAttr ".cuvs" -type "string" "map1";
	setAttr ".dcc" -type "string" "Ambient+Diffuse";
	setAttr ".covm[0]"  0 1 1;
	setAttr ".cdvm[0]"  0 1 1;
createNode transform -n "pCylinder1";
	setAttr ".t" -type "double3" 0.48884127763690621 0.56287865063488252 -0.63205810580458688 ;
createNode mesh -n "pCylinderShape1" -p "pCylinder1";
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
	setAttr ".cuvs" -type "string" "map1";
	setAttr ".dcc" -type "string" "Ambient+Diffuse";
	setAttr ".covm[0]"  0 1 1;
	setAttr ".cdvm[0]"  0 1 1;
createNode transform -n "pCube1";
	setAttr ".t" -type "double3" -1.1940193459091279 0.28760028298830953 1.4979388730369196 ;
createNode mesh -n "pCubeShape1" -p "pCube1";
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
	setAttr ".cuvs" -type "string" "map1";
	setAttr ".dcc" -type "string" "Ambient+Diffuse";
	setAttr ".covm[0]"  0 1 1;
	setAttr ".cdvm[0]"  0 1 1;
createNode transform -n "aiAreaLight1";
	setAttr ".t" -type "double3" 0.70662219238883528 0.4768235872272984 -0.66196126788463716 ;
	setAttr ".s" -type "double3" 0.65248695684312075 0.65248695684312075 0.65248695684312075 ;
createNode aiAreaLight -n "aiAreaLightShape1" -p "aiAreaLight1";
	addAttr -ci true -sn "ai_cast_shadows" -ln "aiCastShadows" -dv 1 -min 0 -max 1 
		-at "bool";
	addAttr -ci true -sn "ai_shadow_density" -ln "aiShadowDensity" -dv 1 -min 0 -max 
		1 -smn 0 -smx 1 -at "float";
	addAttr -ci true -sn "ai_exposure" -ln "aiExposure" -min 0 -smx 10 -at "float";
	addAttr -ci true -sn "ai_samples" -ln "aiSamples" -dv 1 -min 0 -max 100 -at "long";
	addAttr -ci true -sn "ai_normalize" -ln "aiNormalize" -dv 1 -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "ai_bounce_factor" -ln "aiBounceFactor" -dv 1 -at "float";
	addAttr -ci true -sn "ai_bounces" -ln "aiBounces" -dv 999 -at "long";
	addAttr -ci true -k true -m -sn "ai_filters" -ln "aiFilters" -at "message";
	addAttr -ci true -uac -sn "ai_shadow_color" -ln "aiShadowColor" -at "float3" -nc 
		3;
	addAttr -ci true -sn "ai_shadow_colorr" -ln "aiShadowColorR" -at "float" -p "aiShadowColor";
	addAttr -ci true -sn "ai_shadow_colorg" -ln "aiShadowColorG" -at "float" -p "aiShadowColor";
	addAttr -ci true -sn "ai_shadow_colorb" -ln "aiShadowColorB" -at "float" -p "aiShadowColor";
	addAttr -ci true -sn "ai_decay_type" -ln "aiDecayType" -dv 1 -min 0 -max 1 -en "constant:quadratic" 
		-at "enum";
	addAttr -ci true -sn "ai_affect_volumetrics" -ln "aiAffectVolumetrics" -dv 1 -min 
		0 -max 1 -at "bool";
	addAttr -ci true -sn "ai_cast_volumetric_shadows" -ln "aiCastVolumetricShadows" 
		-dv 1 -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "ai_resolution" -ln "aiResolution" -dv 512 -at "long";
	addAttr -ci true -k true -sn "ai_translator" -ln "aiTranslator" -dt "string";
	setAttr -k off ".v";
	setAttr ".intensity" 15;
	setAttr ".input_mesh" -type "mesh" 


		"v"	400
		0.81921768	0	-0.26617995
		0.69686818	0	-0.50630432
		0.50630432	0	-0.69686812
		0.26617992	0	-0.81921756
		0	0	-0.86137629
		-0.26617992	0	-0.8192175
		-0.5063042	0	-0.696868
		-0.69686794	0	-0.5063042
		-0.81921738	0	-0.26617983
		-0.86137605	0	0
		-0.81921738	0	0.26617983
		-0.69686788	0	0.50630414
		-0.50630414	0	0.69686782
		-0.26617983	0	0.81921726
		-2.5671001e-08	0	0.86137599
		0.26617977	0	0.81921721
		0.50630403	0	0.69686776
		0.6968677	0	0.50630409
		0.81921715	0	0.2661798
		0.86137587	0	0
		0.82852727	0.061803401	-0.26920483
		0.70478743	0.061803401	-0.51205796
		0.51205796	0.061803401	-0.70478737
		0.2692048	0.061803401	-0.82852715
		0	0.061803401	-0.87116498
		-0.2692048	0.061803401	-0.82852709
		-0.5120579	0.061803401	-0.70478719
		-0.70478714	0.061803401	-0.51205784
		-0.82852697	0.061803401	-0.26920471
		-0.87116474	0.061803401	0
		-0.82852697	0.061803401	0.26920471
		-0.70478708	0.061803401	0.51205778
		-0.51205778	0.061803401	0.70478702
		-0.26920471	0.061803401	0.82852685
		-2.5962727e-08	0.061803401	0.87116468
		0.26920465	0.061803401	0.82852679
		0.51205766	0.061803401	0.70478702
		0.70478696	0.061803401	0.51205772
		0.82852674	0.061803401	0.26920465
		0.87116456	0.061803401	0
		0.85554481	0.11755706	-0.27798334
		0.72776985	0.11755706	-0.52875572
		0.52875572	0.11755706	-0.72776979
		0.27798331	0.11755706	-0.85554469
		0	0.11755706	-0.89957285
		-0.27798331	0.11755706	-0.85554463
		-0.52875561	0.11755706	-0.72776967
		-0.72776961	0.11755706	-0.52875555
		-0.85554445	0.11755706	-0.27798322
		-0.89957267	0.11755706	0
		-0.85554445	0.11755706	0.27798322
		-0.72776955	0.11755706	0.52875549
		-0.52875549	0.11755706	0.72776949
		-0.27798322	0.11755706	0.85554433
		-2.6809348e-08	0.11755706	0.89957255
		0.27798316	0.11755706	0.85554427
		0.52875543	0.11755706	0.72776943
		0.72776937	0.11755706	0.52875543
		0.85554427	0.11755706	0.27798319
		0.89957243	0.11755706	0
		0.89762563	0.16180341	-0.29165623
		0.76356596	0.16180341	-0.55476308
		0.55476308	0.16180341	-0.7635659
		0.2916562	0.16180341	-0.89762551
		0	0.16180341	-0.94381928
		-0.2916562	0.16180341	-0.89762545
		-0.55476302	0.16180341	-0.76356572
		-0.76356566	0.16180341	-0.55476296
		-0.89762527	0.16180341	-0.29165611
		-0.94381905	0.16180341	0
		-0.89762527	0.16180341	0.29165611
		-0.76356566	0.16180341	0.5547629
		-0.5547629	0.16180341	0.7635656
		-0.29165611	0.16180341	0.89762515
		-2.8127992e-08	0.16180341	0.94381893
		0.29165602	0.16180341	0.89762509
		0.55476278	0.16180341	0.76356554
		0.76356548	0.16180341	0.55476284
		0.89762503	0.16180341	0.29165605
		0.94381881	0.16180341	0
		0.95065051	0.19021133	-0.30888507
		0.80867165	0.19021133	-0.58753431
		0.58753431	0.19021133	-0.80867159
		0.30888504	0.19021133	-0.95065039
		0	0.19021133	-0.99957293
		-0.30888504	0.19021133	-0.95065033
		-0.58753419	0.19021133	-0.80867141
		-0.80867136	0.19021133	-0.58753413
		-0.95065016	0.19021133	-0.30888495
		-0.99957269	0.19021133	0
		-0.95065016	0.19021133	0.30888495
		-0.8086713	0.19021133	0.58753407
		-0.58753407	0.19021133	0.80867124
		-0.30888495	0.19021133	0.95065004
		-2.9789581e-08	0.19021133	0.99957258
		0.30888486	0.19021133	0.95064998
		0.58753395	0.19021133	0.80867118
		0.80867112	0.19021133	0.58753401
		0.95064992	0.19021133	0.30888489
		0.99957246	0.19021133	0
		1.0094291	0.20000003	-0.32798338
		0.85867167	0.20000003	-0.62386143
		0.62386143	0.20000003	-0.85867161
		0.32798335	0.20000003	-1.009429
		0	0.20000003	-1.0613763
		-0.32798335	0.20000003	-1.0094289
		-0.62386131	0.20000003	-0.85867143
		-0.85867137	0.20000003	-0.62386125
		-1.0094287	0.20000003	-0.32798326
		-1.0613761	0.20000003	0
		-1.0094287	0.20000003	0.32798326
		-0.85867131	0.20000003	0.62386119
		-0.62386119	0.20000003	0.85867125
		-0.32798326	0.20000003	1.0094286
		-3.1631465e-08	0.20000003	1.061376
		0.32798314	0.20000003	1.0094285
		0.62386107	0.20000003	0.85867119
		0.85867113	0.20000003	0.62386113
		1.0094285	0.20000003	0.3279832
		1.0613759	0.20000003	0
		1.0682077	0.19021134	-0.34708172
		0.9086718	0.19021134	-0.66018862
		0.66018862	0.19021134	-0.90867168
		0.34708166	0.19021134	-1.0682076
		0	0.19021134	-1.1231798
		-0.34708166	0.19021134	-1.0682075
		-0.6601885	0.19021134	-0.9086715
		-0.90867144	0.19021134	-0.66018844
		-1.0682073	0.19021134	-0.34708157
		-1.1231796	0.19021134	0
		-1.0682073	0.19021134	0.34708157
		-0.90867138	0.19021134	0.66018838
		-0.66018838	0.19021134	0.90867132
		-0.34708157	0.19021134	1.0682071
		-3.3473352e-08	0.19021134	1.1231794
		0.34708148	0.19021134	1.0682071
		0.66018826	0.19021134	0.90867126
		0.90867114	0.19021134	0.66018832
		1.068207	0.19021134	0.34708151
		1.1231793	0.19021134	0
		1.1212325	0.16180342	-0.36431053
		0.95377737	0.16180342	-0.69295979
		0.69295979	0.16180342	-0.95377731
		0.3643105	0.16180342	-1.1212324
		0	0.16180342	-1.1789335
		-0.3643105	0.16180342	-1.1212324
		-0.69295967	0.16180342	-0.95377713
		-0.95377707	0.16180342	-0.69295955
		-1.1212322	0.16180342	-0.36431038
		-1.1789331	0.16180342	0
		-1.1212322	0.16180342	0.36431038
		-0.95377696	0.16180342	0.69295949
		-0.69295949	0.16180342	0.9537769
		-0.36431038	0.16180342	1.121232
		-3.5134939e-08	0.16180342	1.178933
		0.36431026	0.16180342	1.1212319
		0.69295937	0.16180342	0.95377684
		0.95377678	0.16180342	0.69295943
		1.1212318	0.16180342	0.36431032
		1.1789329	0.16180342	0
		1.1633134	0.11755707	-0.37798342
		0.98957354	0.11755707	-0.7189672
		0.7189672	0.11755707	-0.98957348
		0.37798339	0.11755707	-1.1633133
		0	0.11755707	-1.2231799
		-0.37798339	0.11755707	-1.1633133
		-0.71896708	0.11755707	-0.98957324
		-0.98957318	0.11755707	-0.71896696
		-1.163313	0.11755707	-0.37798327
		-1.2231796	0.11755707	0
		-1.163313	0.11755707	0.37798327
		-0.98957312	0.11755707	0.7189669
		-0.7189669	0.11755707	0.989573
		-0.37798327	0.11755707	1.1633129
		-3.6453585e-08	0.11755707	1.2231795
		0.37798318	0.11755707	1.1633128
		0.71896678	0.11755707	0.98957294
		0.98957288	0.11755707	0.71896684
		1.1633127	0.11755707	0.37798321
		1.2231793	0.11755707	0
		1.1903309	0.061803412	-0.38676193
		1.012556	0.061803412	-0.7356649
		0.7356649	0.061803412	-1.0125558
		0.3867619	0.061803412	-1.1903307
		0	0.061803412	-1.2515877
		-0.3867619	0.061803412	-1.1903306
		-0.73566478	0.061803412	-1.0125557
		-1.0125556	0.061803412	-0.73566467
		-1.1903304	0.061803412	-0.38676178
		-1.2515875	0.061803412	0
		-1.1903304	0.061803412	0.38676178
		-1.0125555	0.061803412	0.73566461
		-0.73566461	0.061803412	1.0125555
		-0.38676178	0.061803412	1.1903303
		-3.7300204e-08	0.061803412	1.2515873
		0.38676167	0.061803412	1.1903303
		0.73566449	0.061803412	1.0125554
		1.0125552	0.061803412	0.73566455
		1.1903301	0.061803412	0.38676172
		1.2515872	0.061803412	0
		1.1996405	0	-0.38978684
		1.0204753	0	-0.7414186
		0.7414186	0	-1.0204751
		0.38978678	0	-1.1996404
		0	0	-1.2613765
		-0.38978678	0	-1.1996403
		-0.74141842	0	-1.0204749
		-1.0204749	0	-0.74141836
		-1.1996402	0	-0.38978669
		-1.2613763	0	0
		-1.1996402	0	0.38978669
		-1.0204748	0	0.7414183
		-0.7414183	0	1.0204747
		-0.38978669	0	1.1996399
		-3.7591931e-08	0	1.261376
		0.38978657	0	1.1996399
		0.74141812	0	1.0204747
		1.0204746	0	0.74141824
		1.1996398	0	0.3897866
		1.2613759	0	0
		1.1903309	-0.061803412	-0.38676193
		1.012556	-0.061803412	-0.7356649
		0.7356649	-0.061803412	-1.0125558
		0.3867619	-0.061803412	-1.1903307
		0	-0.061803412	-1.2515877
		-0.3867619	-0.061803412	-1.1903306
		-0.73566478	-0.061803412	-1.0125557
		-1.0125556	-0.061803412	-0.73566467
		-1.1903304	-0.061803412	-0.38676178
		-1.2515875	-0.061803412	0
		-1.1903304	-0.061803412	0.38676178
		-1.0125555	-0.061803412	0.73566461
		-0.73566461	-0.061803412	1.0125555
		-0.38676178	-0.061803412	1.1903303
		-3.7300204e-08	-0.061803412	1.2515873
		0.38676167	-0.061803412	1.1903303
		0.73566449	-0.061803412	1.0125554
		1.0125552	-0.061803412	0.73566455
		1.1903301	-0.061803412	0.38676172
		1.2515872	-0.061803412	0
		1.1633134	-0.11755709	-0.37798342
		0.98957354	-0.11755709	-0.7189672
		0.7189672	-0.11755709	-0.98957348
		0.37798339	-0.11755709	-1.1633133
		0	-0.11755709	-1.2231799
		-0.37798339	-0.11755709	-1.1633133
		-0.71896708	-0.11755709	-0.98957324
		-0.98957318	-0.11755709	-0.71896696
		-1.163313	-0.11755709	-0.37798327
		-1.2231796	-0.11755709	0
		-1.163313	-0.11755709	0.37798327
		-0.98957312	-0.11755709	0.7189669
		-0.7189669	-0.11755709	0.989573
		-0.37798327	-0.11755709	1.1633129
		-3.6453585e-08	-0.11755709	1.2231795
		0.37798318	-0.11755709	1.1633128
		0.71896678	-0.11755709	0.98957294
		0.98957288	-0.11755709	0.71896684
		1.1633127	-0.11755709	0.37798321
		1.2231793	-0.11755709	0
		1.1212325	-0.16180347	-0.36431053
		0.95377737	-0.16180347	-0.69295979
		0.69295979	-0.16180347	-0.95377731
		0.3643105	-0.16180347	-1.1212324
		0	-0.16180347	-1.1789335
		-0.3643105	-0.16180347	-1.1212324
		-0.69295967	-0.16180347	-0.95377713
		-0.95377707	-0.16180347	-0.69295955
		-1.1212322	-0.16180347	-0.36431038
		-1.1789331	-0.16180347	0
		-1.1212322	-0.16180347	0.36431038
		-0.95377696	-0.16180347	0.69295949
		-0.69295949	-0.16180347	0.9537769
		-0.36431038	-0.16180347	1.121232
		-3.5134939e-08	-0.16180347	1.178933
		0.36431026	-0.16180347	1.1212319
		0.69295937	-0.16180347	0.95377684
		0.95377678	-0.16180347	0.69295943
		1.1212318	-0.16180347	0.36431032
		1.1789329	-0.16180347	0
		1.0682077	-0.1902114	-0.34708172
		0.9086718	-0.1902114	-0.66018862
		0.66018862	-0.1902114	-0.90867168
		0.34708166	-0.1902114	-1.0682076
		0	-0.1902114	-1.1231798
		-0.34708166	-0.1902114	-1.0682075
		-0.6601885	-0.1902114	-0.9086715
		-0.90867144	-0.1902114	-0.66018844
		-1.0682073	-0.1902114	-0.34708157
		-1.1231796	-0.1902114	0
		-1.0682073	-0.1902114	0.34708157
		-0.90867138	-0.1902114	0.66018838
		-0.66018838	-0.1902114	0.90867132
		-0.34708157	-0.1902114	1.0682071
		-3.3473352e-08	-0.1902114	1.1231794
		0.34708148	-0.1902114	1.0682071
		0.66018826	-0.1902114	0.90867126
		0.90867114	-0.1902114	0.66018832
		1.068207	-0.1902114	0.34708151
		1.1231793	-0.1902114	0
		1.0094291	-0.20000009	-0.32798338
		0.85867167	-0.20000009	-0.62386143
		0.62386143	-0.20000009	-0.85867161
		0.32798335	-0.20000009	-1.009429
		0	-0.20000009	-1.0613763
		-0.32798335	-0.20000009	-1.0094289
		-0.62386131	-0.20000009	-0.85867143
		-0.85867137	-0.20000009	-0.62386125
		-1.0094287	-0.20000009	-0.32798326
		-1.0613761	-0.20000009	0
		-1.0094287	-0.20000009	0.32798326
		-0.85867131	-0.20000009	0.62386119
		-0.62386119	-0.20000009	0.85867125
		-0.32798326	-0.20000009	1.0094286
		-3.1631465e-08	-0.20000009	1.061376
		0.32798314	-0.20000009	1.0094285
		0.62386107	-0.20000009	0.85867119
		0.85867113	-0.20000009	0.62386113
		1.0094285	-0.20000009	0.3279832
		1.0613759	-0.20000009	0
		0.95065045	-0.1902114	-0.30888504
		0.80867159	-0.1902114	-0.58753425
		0.58753425	-0.1902114	-0.80867153
		0.30888501	-0.1902114	-0.95065033
		0	-0.1902114	-0.99957287
		-0.30888501	-0.1902114	-0.95065027
		-0.58753413	-0.1902114	-0.80867136
		-0.8086713	-0.1902114	-0.58753407
		-0.9506501	-0.1902114	-0.30888492
		-0.99957263	-0.1902114	0
		-0.9506501	-0.1902114	0.30888492
		-0.80867124	-0.1902114	0.58753401
		-0.58753401	-0.1902114	0.80867118
		-0.30888492	-0.1902114	0.95064998
		-2.9789579e-08	-0.1902114	0.99957252
		0.30888483	-0.1902114	0.95064992
		0.58753389	-0.1902114	0.80867112
		0.80867106	-0.1902114	0.58753395
		0.95064986	-0.1902114	0.30888486
		0.9995724	-0.1902114	0
		0.89762557	-0.1618035	-0.2916562
		0.7635659	-0.1618035	-0.55476308
		0.55476308	-0.1618035	-0.76356584
		0.29165617	-0.1618035	-0.89762545
		0	-0.1618035	-0.94381922
		-0.29165617	-0.1618035	-0.89762539
		-0.55476296	-0.1618035	-0.76356572
		-0.76356566	-0.1618035	-0.5547629
		-0.89762521	-0.1618035	-0.29165611
		-0.94381899	-0.1618035	0
		-0.89762521	-0.1618035	0.29165611
		-0.7635656	-0.1618035	0.55476284
		-0.55476284	-0.1618035	0.76356554
		-0.29165611	-0.1618035	0.89762509
		-2.8127991e-08	-0.1618035	0.94381887
		0.29165602	-0.1618035	0.89762503
		0.55476272	-0.1618035	0.76356548
		0.76356542	-0.1618035	0.55476278
		0.89762497	-0.1618035	0.29165605
		0.94381875	-0.1618035	0
		0.85554475	-0.11755712	-0.27798331
		0.72776985	-0.11755712	-0.52875566
		0.52875566	-0.11755712	-0.72776979
		0.27798328	-0.11755712	-0.85554463
		0	-0.11755712	-0.89957279
		-0.27798328	-0.11755712	-0.85554457
		-0.52875561	-0.11755712	-0.72776961
		-0.72776955	-0.11755712	-0.52875555
		-0.85554439	-0.11755712	-0.27798322
		-0.89957261	-0.11755712	0
		-0.85554439	-0.11755712	0.27798322
		-0.72776949	-0.11755712	0.52875549
		-0.52875549	-0.11755712	0.72776943
		-0.27798322	-0.11755712	0.85554427
		-2.6809346e-08	-0.11755712	0.89957249
		0.27798313	-0.11755712	0.85554427
		0.52875537	-0.11755712	0.72776937
		0.72776932	-0.11755712	0.52875543
		0.85554421	-0.11755712	0.27798316
		0.89957237	-0.11755712	0
		0.82852715	-0.061803438	-0.26920477
		0.70478731	-0.061803438	-0.5120579
		0.5120579	-0.061803438	-0.70478725
		0.26920477	-0.061803438	-0.82852703
		0	-0.061803438	-0.87116486
		-0.26920477	-0.061803438	-0.82852697
		-0.51205784	-0.061803438	-0.70478708
		-0.70478708	-0.061803438	-0.51205778
		-0.82852685	-0.061803438	-0.26920468
		-0.87116462	-0.061803438	0
		-0.82852685	-0.061803438	0.26920468
		-0.70478702	-0.061803438	0.51205772
		-0.51205772	-0.061803438	0.70478696
		-0.26920468	-0.061803438	0.82852674
		-2.5962724e-08	-0.061803438	0.87116456
		0.26920459	-0.061803438	0.82852668
		0.5120576	-0.061803438	0.7047869
		0.70478684	-0.061803438	0.51205766
		0.82852662	-0.061803438	0.26920462
		0.87116444	-0.061803438	0

		"vt"	441
		0	1
		0.050000001	1
		0.1	1
		0.15000001	1
		0.2	1
		0.25	1
		0.30000001	1
		0.35000002	1
		0.40000004	1
		0.45000005	1
		0.50000006	1
		0.55000007	1
		0.60000008	1
		0.6500001	1
		0.70000011	1
		0.75000012	1
		0.80000013	1
		0.85000014	1
		0.90000015	1
		0.95000017	1
		1.0000001	1
		0	0.94999999
		0.050000001	0.94999999
		0.1	0.94999999
		0.15000001	0.94999999
		0.2	0.94999999
		0.25	0.94999999
		0.30000001	0.94999999
		0.35000002	0.94999999
		0.40000004	0.94999999
		0.45000005	0.94999999
		0.50000006	0.94999999
		0.55000007	0.94999999
		0.60000008	0.94999999
		0.6500001	0.94999999
		0.70000011	0.94999999
		0.75000012	0.94999999
		0.80000013	0.94999999
		0.85000014	0.94999999
		0.90000015	0.94999999
		0.95000017	0.94999999
		1.0000001	0.94999999
		0	0.89999998
		0.050000001	0.89999998
		0.1	0.89999998
		0.15000001	0.89999998
		0.2	0.89999998
		0.25	0.89999998
		0.30000001	0.89999998
		0.35000002	0.89999998
		0.40000004	0.89999998
		0.45000005	0.89999998
		0.50000006	0.89999998
		0.55000007	0.89999998
		0.60000008	0.89999998
		0.6500001	0.89999998
		0.70000011	0.89999998
		0.75000012	0.89999998
		0.80000013	0.89999998
		0.85000014	0.89999998
		0.90000015	0.89999998
		0.95000017	0.89999998
		1.0000001	0.89999998
		0	0.84999996
		0.050000001	0.84999996
		0.1	0.84999996
		0.15000001	0.84999996
		0.2	0.84999996
		0.25	0.84999996
		0.30000001	0.84999996
		0.35000002	0.84999996
		0.40000004	0.84999996
		0.45000005	0.84999996
		0.50000006	0.84999996
		0.55000007	0.84999996
		0.60000008	0.84999996
		0.6500001	0.84999996
		0.70000011	0.84999996
		0.75000012	0.84999996
		0.80000013	0.84999996
		0.85000014	0.84999996
		0.90000015	0.84999996
		0.95000017	0.84999996
		1.0000001	0.84999996
		0	0.79999995
		0.050000001	0.79999995
		0.1	0.79999995
		0.15000001	0.79999995
		0.2	0.79999995
		0.25	0.79999995
		0.30000001	0.79999995
		0.35000002	0.79999995
		0.40000004	0.79999995
		0.45000005	0.79999995
		0.50000006	0.79999995
		0.55000007	0.79999995
		0.60000008	0.79999995
		0.6500001	0.79999995
		0.70000011	0.79999995
		0.75000012	0.79999995
		0.80000013	0.79999995
		0.85000014	0.79999995
		0.90000015	0.79999995
		0.95000017	0.79999995
		1.0000001	0.79999995
		0	0.74999994
		0.050000001	0.74999994
		0.1	0.74999994
		0.15000001	0.74999994
		0.2	0.74999994
		0.25	0.74999994
		0.30000001	0.74999994
		0.35000002	0.74999994
		0.40000004	0.74999994
		0.45000005	0.74999994
		0.50000006	0.74999994
		0.55000007	0.74999994
		0.60000008	0.74999994
		0.6500001	0.74999994
		0.70000011	0.74999994
		0.75000012	0.74999994
		0.80000013	0.74999994
		0.85000014	0.74999994
		0.90000015	0.74999994
		0.95000017	0.74999994
		1.0000001	0.74999994
		0	0.69999993
		0.050000001	0.69999993
		0.1	0.69999993
		0.15000001	0.69999993
		0.2	0.69999993
		0.25	0.69999993
		0.30000001	0.69999993
		0.35000002	0.69999993
		0.40000004	0.69999993
		0.45000005	0.69999993
		0.50000006	0.69999993
		0.55000007	0.69999993
		0.60000008	0.69999993
		0.6500001	0.69999993
		0.70000011	0.69999993
		0.75000012	0.69999993
		0.80000013	0.69999993
		0.85000014	0.69999993
		0.90000015	0.69999993
		0.95000017	0.69999993
		1.0000001	0.69999993
		0	0.64999992
		0.050000001	0.64999992
		0.1	0.64999992
		0.15000001	0.64999992
		0.2	0.64999992
		0.25	0.64999992
		0.30000001	0.64999992
		0.35000002	0.64999992
		0.40000004	0.64999992
		0.45000005	0.64999992
		0.50000006	0.64999992
		0.55000007	0.64999992
		0.60000008	0.64999992
		0.6500001	0.64999992
		0.70000011	0.64999992
		0.75000012	0.64999992
		0.80000013	0.64999992
		0.85000014	0.64999992
		0.90000015	0.64999992
		0.95000017	0.64999992
		1.0000001	0.64999992
		0	0.5999999
		0.050000001	0.5999999
		0.1	0.5999999
		0.15000001	0.5999999
		0.2	0.5999999
		0.25	0.5999999
		0.30000001	0.5999999
		0.35000002	0.5999999
		0.40000004	0.5999999
		0.45000005	0.5999999
		0.50000006	0.5999999
		0.55000007	0.5999999
		0.60000008	0.5999999
		0.6500001	0.5999999
		0.70000011	0.5999999
		0.75000012	0.5999999
		0.80000013	0.5999999
		0.85000014	0.5999999
		0.90000015	0.5999999
		0.95000017	0.5999999
		1.0000001	0.5999999
		0	0.54999989
		0.050000001	0.54999989
		0.1	0.54999989
		0.15000001	0.54999989
		0.2	0.54999989
		0.25	0.54999989
		0.30000001	0.54999989
		0.35000002	0.54999989
		0.40000004	0.54999989
		0.45000005	0.54999989
		0.50000006	0.54999989
		0.55000007	0.54999989
		0.60000008	0.54999989
		0.6500001	0.54999989
		0.70000011	0.54999989
		0.75000012	0.54999989
		0.80000013	0.54999989
		0.85000014	0.54999989
		0.90000015	0.54999989
		0.95000017	0.54999989
		1.0000001	0.54999989
		0	0.49999988
		0.050000001	0.49999988
		0.1	0.49999988
		0.15000001	0.49999988
		0.2	0.49999988
		0.25	0.49999988
		0.30000001	0.49999988
		0.35000002	0.49999988
		0.40000004	0.49999988
		0.45000005	0.49999988
		0.50000006	0.49999988
		0.55000007	0.49999988
		0.60000008	0.49999988
		0.6500001	0.49999988
		0.70000011	0.49999988
		0.75000012	0.49999988
		0.80000013	0.49999988
		0.85000014	0.49999988
		0.90000015	0.49999988
		0.95000017	0.49999988
		1.0000001	0.49999988
		0	0.44999987
		0.050000001	0.44999987
		0.1	0.44999987
		0.15000001	0.44999987
		0.2	0.44999987
		0.25	0.44999987
		0.30000001	0.44999987
		0.35000002	0.44999987
		0.40000004	0.44999987
		0.45000005	0.44999987
		0.50000006	0.44999987
		0.55000007	0.44999987
		0.60000008	0.44999987
		0.6500001	0.44999987
		0.70000011	0.44999987
		0.75000012	0.44999987
		0.80000013	0.44999987
		0.85000014	0.44999987
		0.90000015	0.44999987
		0.95000017	0.44999987
		1.0000001	0.44999987
		0	0.39999986
		0.050000001	0.39999986
		0.1	0.39999986
		0.15000001	0.39999986
		0.2	0.39999986
		0.25	0.39999986
		0.30000001	0.39999986
		0.35000002	0.39999986
		0.40000004	0.39999986
		0.45000005	0.39999986
		0.50000006	0.39999986
		0.55000007	0.39999986
		0.60000008	0.39999986
		0.6500001	0.39999986
		0.70000011	0.39999986
		0.75000012	0.39999986
		0.80000013	0.39999986
		0.85000014	0.39999986
		0.90000015	0.39999986
		0.95000017	0.39999986
		1.0000001	0.39999986
		0	0.34999985
		0.050000001	0.34999985
		0.1	0.34999985
		0.15000001	0.34999985
		0.2	0.34999985
		0.25	0.34999985
		0.30000001	0.34999985
		0.35000002	0.34999985
		0.40000004	0.34999985
		0.45000005	0.34999985
		0.50000006	0.34999985
		0.55000007	0.34999985
		0.60000008	0.34999985
		0.6500001	0.34999985
		0.70000011	0.34999985
		0.75000012	0.34999985
		0.80000013	0.34999985
		0.85000014	0.34999985
		0.90000015	0.34999985
		0.95000017	0.34999985
		1.0000001	0.34999985
		0	0.29999983
		0.050000001	0.29999983
		0.1	0.29999983
		0.15000001	0.29999983
		0.2	0.29999983
		0.25	0.29999983
		0.30000001	0.29999983
		0.35000002	0.29999983
		0.40000004	0.29999983
		0.45000005	0.29999983
		0.50000006	0.29999983
		0.55000007	0.29999983
		0.60000008	0.29999983
		0.6500001	0.29999983
		0.70000011	0.29999983
		0.75000012	0.29999983
		0.80000013	0.29999983
		0.85000014	0.29999983
		0.90000015	0.29999983
		0.95000017	0.29999983
		1.0000001	0.29999983
		0	0.24999984
		0.050000001	0.24999984
		0.1	0.24999984
		0.15000001	0.24999984
		0.2	0.24999984
		0.25	0.24999984
		0.30000001	0.24999984
		0.35000002	0.24999984
		0.40000004	0.24999984
		0.45000005	0.24999984
		0.50000006	0.24999984
		0.55000007	0.24999984
		0.60000008	0.24999984
		0.6500001	0.24999984
		0.70000011	0.24999984
		0.75000012	0.24999984
		0.80000013	0.24999984
		0.85000014	0.24999984
		0.90000015	0.24999984
		0.95000017	0.24999984
		1.0000001	0.24999984
		0	0.19999984
		0.050000001	0.19999984
		0.1	0.19999984
		0.15000001	0.19999984
		0.2	0.19999984
		0.25	0.19999984
		0.30000001	0.19999984
		0.35000002	0.19999984
		0.40000004	0.19999984
		0.45000005	0.19999984
		0.50000006	0.19999984
		0.55000007	0.19999984
		0.60000008	0.19999984
		0.6500001	0.19999984
		0.70000011	0.19999984
		0.75000012	0.19999984
		0.80000013	0.19999984
		0.85000014	0.19999984
		0.90000015	0.19999984
		0.95000017	0.19999984
		1.0000001	0.19999984
		0	0.14999984
		0.050000001	0.14999984
		0.1	0.14999984
		0.15000001	0.14999984
		0.2	0.14999984
		0.25	0.14999984
		0.30000001	0.14999984
		0.35000002	0.14999984
		0.40000004	0.14999984
		0.45000005	0.14999984
		0.50000006	0.14999984
		0.55000007	0.14999984
		0.60000008	0.14999984
		0.6500001	0.14999984
		0.70000011	0.14999984
		0.75000012	0.14999984
		0.80000013	0.14999984
		0.85000014	0.14999984
		0.90000015	0.14999984
		0.95000017	0.14999984
		1.0000001	0.14999984
		0	0.099999845
		0.050000001	0.099999845
		0.1	0.099999845
		0.15000001	0.099999845
		0.2	0.099999845
		0.25	0.099999845
		0.30000001	0.099999845
		0.35000002	0.099999845
		0.40000004	0.099999845
		0.45000005	0.099999845
		0.50000006	0.099999845
		0.55000007	0.099999845
		0.60000008	0.099999845
		0.6500001	0.099999845
		0.70000011	0.099999845
		0.75000012	0.099999845
		0.80000013	0.099999845
		0.85000014	0.099999845
		0.90000015	0.099999845
		0.95000017	0.099999845
		1.0000001	0.099999845
		0	0.049999844
		0.050000001	0.049999844
		0.1	0.049999844
		0.15000001	0.049999844
		0.2	0.049999844
		0.25	0.049999844
		0.30000001	0.049999844
		0.35000002	0.049999844
		0.40000004	0.049999844
		0.45000005	0.049999844
		0.50000006	0.049999844
		0.55000007	0.049999844
		0.60000008	0.049999844
		0.6500001	0.049999844
		0.70000011	0.049999844
		0.75000012	0.049999844
		0.80000013	0.049999844
		0.85000014	0.049999844
		0.90000015	0.049999844
		0.95000017	0.049999844
		1.0000001	0.049999844
		0	-1.5646219e-07
		0.050000001	-1.5646219e-07
		0.1	-1.5646219e-07
		0.15000001	-1.5646219e-07
		0.2	-1.5646219e-07
		0.25	-1.5646219e-07
		0.30000001	-1.5646219e-07
		0.35000002	-1.5646219e-07
		0.40000004	-1.5646219e-07
		0.45000005	-1.5646219e-07
		0.50000006	-1.5646219e-07
		0.55000007	-1.5646219e-07
		0.60000008	-1.5646219e-07
		0.6500001	-1.5646219e-07
		0.70000011	-1.5646219e-07
		0.75000012	-1.5646219e-07
		0.80000013	-1.5646219e-07
		0.85000014	-1.5646219e-07
		0.90000015	-1.5646219e-07
		0.95000017	-1.5646219e-07
		1.0000001	-1.5646219e-07

		"e"	800
		0	1	"smooth"
		1	2	"smooth"
		2	3	"smooth"
		3	4	"smooth"
		4	5	"smooth"
		5	6	"smooth"
		6	7	"smooth"
		7	8	"smooth"
		8	9	"smooth"
		9	10	"smooth"
		10	11	"smooth"
		11	12	"smooth"
		12	13	"smooth"
		13	14	"smooth"
		14	15	"smooth"
		15	16	"smooth"
		16	17	"smooth"
		17	18	"smooth"
		18	19	"smooth"
		19	0	"smooth"
		20	21	"smooth"
		21	22	"smooth"
		22	23	"smooth"
		23	24	"smooth"
		24	25	"smooth"
		25	26	"smooth"
		26	27	"smooth"
		27	28	"smooth"
		28	29	"smooth"
		29	30	"smooth"
		30	31	"smooth"
		31	32	"smooth"
		32	33	"smooth"
		33	34	"smooth"
		34	35	"smooth"
		35	36	"smooth"
		36	37	"smooth"
		37	38	"smooth"
		38	39	"smooth"
		39	20	"smooth"
		40	41	"smooth"
		41	42	"smooth"
		42	43	"smooth"
		43	44	"smooth"
		44	45	"smooth"
		45	46	"smooth"
		46	47	"smooth"
		47	48	"smooth"
		48	49	"smooth"
		49	50	"smooth"
		50	51	"smooth"
		51	52	"smooth"
		52	53	"smooth"
		53	54	"smooth"
		54	55	"smooth"
		55	56	"smooth"
		56	57	"smooth"
		57	58	"smooth"
		58	59	"smooth"
		59	40	"smooth"
		60	61	"smooth"
		61	62	"smooth"
		62	63	"smooth"
		63	64	"smooth"
		64	65	"smooth"
		65	66	"smooth"
		66	67	"smooth"
		67	68	"smooth"
		68	69	"smooth"
		69	70	"smooth"
		70	71	"smooth"
		71	72	"smooth"
		72	73	"smooth"
		73	74	"smooth"
		74	75	"smooth"
		75	76	"smooth"
		76	77	"smooth"
		77	78	"smooth"
		78	79	"smooth"
		79	60	"smooth"
		80	81	"smooth"
		81	82	"smooth"
		82	83	"smooth"
		83	84	"smooth"
		84	85	"smooth"
		85	86	"smooth"
		86	87	"smooth"
		87	88	"smooth"
		88	89	"smooth"
		89	90	"smooth"
		90	91	"smooth"
		91	92	"smooth"
		92	93	"smooth"
		93	94	"smooth"
		94	95	"smooth"
		95	96	"smooth"
		96	97	"smooth"
		97	98	"smooth"
		98	99	"smooth"
		99	80	"smooth"
		100	101	"smooth"
		101	102	"smooth"
		102	103	"smooth"
		103	104	"smooth"
		104	105	"smooth"
		105	106	"smooth"
		106	107	"smooth"
		107	108	"smooth"
		108	109	"smooth"
		109	110	"smooth"
		110	111	"smooth"
		111	112	"smooth"
		112	113	"smooth"
		113	114	"smooth"
		114	115	"smooth"
		115	116	"smooth"
		116	117	"smooth"
		117	118	"smooth"
		118	119	"smooth"
		119	100	"smooth"
		120	121	"smooth"
		121	122	"smooth"
		122	123	"smooth"
		123	124	"smooth"
		124	125	"smooth"
		125	126	"smooth"
		126	127	"smooth"
		127	128	"smooth"
		128	129	"smooth"
		129	130	"smooth"
		130	131	"smooth"
		131	132	"smooth"
		132	133	"smooth"
		133	134	"smooth"
		134	135	"smooth"
		135	136	"smooth"
		136	137	"smooth"
		137	138	"smooth"
		138	139	"smooth"
		139	120	"smooth"
		140	141	"smooth"
		141	142	"smooth"
		142	143	"smooth"
		143	144	"smooth"
		144	145	"smooth"
		145	146	"smooth"
		146	147	"smooth"
		147	148	"smooth"
		148	149	"smooth"
		149	150	"smooth"
		150	151	"smooth"
		151	152	"smooth"
		152	153	"smooth"
		153	154	"smooth"
		154	155	"smooth"
		155	156	"smooth"
		156	157	"smooth"
		157	158	"smooth"
		158	159	"smooth"
		159	140	"smooth"
		160	161	"smooth"
		161	162	"smooth"
		162	163	"smooth"
		163	164	"smooth"
		164	165	"smooth"
		165	166	"smooth"
		166	167	"smooth"
		167	168	"smooth"
		168	169	"smooth"
		169	170	"smooth"
		170	171	"smooth"
		171	172	"smooth"
		172	173	"smooth"
		173	174	"smooth"
		174	175	"smooth"
		175	176	"smooth"
		176	177	"smooth"
		177	178	"smooth"
		178	179	"smooth"
		179	160	"smooth"
		180	181	"smooth"
		181	182	"smooth"
		182	183	"smooth"
		183	184	"smooth"
		184	185	"smooth"
		185	186	"smooth"
		186	187	"smooth"
		187	188	"smooth"
		188	189	"smooth"
		189	190	"smooth"
		190	191	"smooth"
		191	192	"smooth"
		192	193	"smooth"
		193	194	"smooth"
		194	195	"smooth"
		195	196	"smooth"
		196	197	"smooth"
		197	198	"smooth"
		198	199	"smooth"
		199	180	"smooth"
		200	201	"smooth"
		201	202	"smooth"
		202	203	"smooth"
		203	204	"smooth"
		204	205	"smooth"
		205	206	"smooth"
		206	207	"smooth"
		207	208	"smooth"
		208	209	"smooth"
		209	210	"smooth"
		210	211	"smooth"
		211	212	"smooth"
		212	213	"smooth"
		213	214	"smooth"
		214	215	"smooth"
		215	216	"smooth"
		216	217	"smooth"
		217	218	"smooth"
		218	219	"smooth"
		219	200	"smooth"
		220	221	"smooth"
		221	222	"smooth"
		222	223	"smooth"
		223	224	"smooth"
		224	225	"smooth"
		225	226	"smooth"
		226	227	"smooth"
		227	228	"smooth"
		228	229	"smooth"
		229	230	"smooth"
		230	231	"smooth"
		231	232	"smooth"
		232	233	"smooth"
		233	234	"smooth"
		234	235	"smooth"
		235	236	"smooth"
		236	237	"smooth"
		237	238	"smooth"
		238	239	"smooth"
		239	220	"smooth"
		240	241	"smooth"
		241	242	"smooth"
		242	243	"smooth"
		243	244	"smooth"
		244	245	"smooth"
		245	246	"smooth"
		246	247	"smooth"
		247	248	"smooth"
		248	249	"smooth"
		249	250	"smooth"
		250	251	"smooth"
		251	252	"smooth"
		252	253	"smooth"
		253	254	"smooth"
		254	255	"smooth"
		255	256	"smooth"
		256	257	"smooth"
		257	258	"smooth"
		258	259	"smooth"
		259	240	"smooth"
		260	261	"smooth"
		261	262	"smooth"
		262	263	"smooth"
		263	264	"smooth"
		264	265	"smooth"
		265	266	"smooth"
		266	267	"smooth"
		267	268	"smooth"
		268	269	"smooth"
		269	270	"smooth"
		270	271	"smooth"
		271	272	"smooth"
		272	273	"smooth"
		273	274	"smooth"
		274	275	"smooth"
		275	276	"smooth"
		276	277	"smooth"
		277	278	"smooth"
		278	279	"smooth"
		279	260	"smooth"
		280	281	"smooth"
		281	282	"smooth"
		282	283	"smooth"
		283	284	"smooth"
		284	285	"smooth"
		285	286	"smooth"
		286	287	"smooth"
		287	288	"smooth"
		288	289	"smooth"
		289	290	"smooth"
		290	291	"smooth"
		291	292	"smooth"
		292	293	"smooth"
		293	294	"smooth"
		294	295	"smooth"
		295	296	"smooth"
		296	297	"smooth"
		297	298	"smooth"
		298	299	"smooth"
		299	280	"smooth"
		300	301	"smooth"
		301	302	"smooth"
		302	303	"smooth"
		303	304	"smooth"
		304	305	"smooth"
		305	306	"smooth"
		306	307	"smooth"
		307	308	"smooth"
		308	309	"smooth"
		309	310	"smooth"
		310	311	"smooth"
		311	312	"smooth"
		312	313	"smooth"
		313	314	"smooth"
		314	315	"smooth"
		315	316	"smooth"
		316	317	"smooth"
		317	318	"smooth"
		318	319	"smooth"
		319	300	"smooth"
		320	321	"smooth"
		321	322	"smooth"
		322	323	"smooth"
		323	324	"smooth"
		324	325	"smooth"
		325	326	"smooth"
		326	327	"smooth"
		327	328	"smooth"
		328	329	"smooth"
		329	330	"smooth"
		330	331	"smooth"
		331	332	"smooth"
		332	333	"smooth"
		333	334	"smooth"
		334	335	"smooth"
		335	336	"smooth"
		336	337	"smooth"
		337	338	"smooth"
		338	339	"smooth"
		339	320	"smooth"
		340	341	"smooth"
		341	342	"smooth"
		342	343	"smooth"
		343	344	"smooth"
		344	345	"smooth"
		345	346	"smooth"
		346	347	"smooth"
		347	348	"smooth"
		348	349	"smooth"
		349	350	"smooth"
		350	351	"smooth"
		351	352	"smooth"
		352	353	"smooth"
		353	354	"smooth"
		354	355	"smooth"
		355	356	"smooth"
		356	357	"smooth"
		357	358	"smooth"
		358	359	"smooth"
		359	340	"smooth"
		360	361	"smooth"
		361	362	"smooth"
		362	363	"smooth"
		363	364	"smooth"
		364	365	"smooth"
		365	366	"smooth"
		366	367	"smooth"
		367	368	"smooth"
		368	369	"smooth"
		369	370	"smooth"
		370	371	"smooth"
		371	372	"smooth"
		372	373	"smooth"
		373	374	"smooth"
		374	375	"smooth"
		375	376	"smooth"
		376	377	"smooth"
		377	378	"smooth"
		378	379	"smooth"
		379	360	"smooth"
		380	381	"smooth"
		381	382	"smooth"
		382	383	"smooth"
		383	384	"smooth"
		384	385	"smooth"
		385	386	"smooth"
		386	387	"smooth"
		387	388	"smooth"
		388	389	"smooth"
		389	390	"smooth"
		390	391	"smooth"
		391	392	"smooth"
		392	393	"smooth"
		393	394	"smooth"
		394	395	"smooth"
		395	396	"smooth"
		396	397	"smooth"
		397	398	"smooth"
		398	399	"smooth"
		399	380	"smooth"
		0	20	"smooth"
		1	21	"smooth"
		2	22	"smooth"
		3	23	"smooth"
		4	24	"smooth"
		5	25	"smooth"
		6	26	"smooth"
		7	27	"smooth"
		8	28	"smooth"
		9	29	"smooth"
		10	30	"smooth"
		11	31	"smooth"
		12	32	"smooth"
		13	33	"smooth"
		14	34	"smooth"
		15	35	"smooth"
		16	36	"smooth"
		17	37	"smooth"
		18	38	"smooth"
		19	39	"smooth"
		20	40	"smooth"
		21	41	"smooth"
		22	42	"smooth"
		23	43	"smooth"
		24	44	"smooth"
		25	45	"smooth"
		26	46	"smooth"
		27	47	"smooth"
		28	48	"smooth"
		29	49	"smooth"
		30	50	"smooth"
		31	51	"smooth"
		32	52	"smooth"
		33	53	"smooth"
		34	54	"smooth"
		35	55	"smooth"
		36	56	"smooth"
		37	57	"smooth"
		38	58	"smooth"
		39	59	"smooth"
		40	60	"smooth"
		41	61	"smooth"
		42	62	"smooth"
		43	63	"smooth"
		44	64	"smooth"
		45	65	"smooth"
		46	66	"smooth"
		47	67	"smooth"
		48	68	"smooth"
		49	69	"smooth"
		50	70	"smooth"
		51	71	"smooth"
		52	72	"smooth"
		53	73	"smooth"
		54	74	"smooth"
		55	75	"smooth"
		56	76	"smooth"
		57	77	"smooth"
		58	78	"smooth"
		59	79	"smooth"
		60	80	"smooth"
		61	81	"smooth"
		62	82	"smooth"
		63	83	"smooth"
		64	84	"smooth"
		65	85	"smooth"
		66	86	"smooth"
		67	87	"smooth"
		68	88	"smooth"
		69	89	"smooth"
		70	90	"smooth"
		71	91	"smooth"
		72	92	"smooth"
		73	93	"smooth"
		74	94	"smooth"
		75	95	"smooth"
		76	96	"smooth"
		77	97	"smooth"
		78	98	"smooth"
		79	99	"smooth"
		80	100	"smooth"
		81	101	"smooth"
		82	102	"smooth"
		83	103	"smooth"
		84	104	"smooth"
		85	105	"smooth"
		86	106	"smooth"
		87	107	"smooth"
		88	108	"smooth"
		89	109	"smooth"
		90	110	"smooth"
		91	111	"smooth"
		92	112	"smooth"
		93	113	"smooth"
		94	114	"smooth"
		95	115	"smooth"
		96	116	"smooth"
		97	117	"smooth"
		98	118	"smooth"
		99	119	"smooth"
		100	120	"smooth"
		101	121	"smooth"
		102	122	"smooth"
		103	123	"smooth"
		104	124	"smooth"
		105	125	"smooth"
		106	126	"smooth"
		107	127	"smooth"
		108	128	"smooth"
		109	129	"smooth"
		110	130	"smooth"
		111	131	"smooth"
		112	132	"smooth"
		113	133	"smooth"
		114	134	"smooth"
		115	135	"smooth"
		116	136	"smooth"
		117	137	"smooth"
		118	138	"smooth"
		119	139	"smooth"
		120	140	"smooth"
		121	141	"smooth"
		122	142	"smooth"
		123	143	"smooth"
		124	144	"smooth"
		125	145	"smooth"
		126	146	"smooth"
		127	147	"smooth"
		128	148	"smooth"
		129	149	"smooth"
		130	150	"smooth"
		131	151	"smooth"
		132	152	"smooth"
		133	153	"smooth"
		134	154	"smooth"
		135	155	"smooth"
		136	156	"smooth"
		137	157	"smooth"
		138	158	"smooth"
		139	159	"smooth"
		140	160	"smooth"
		141	161	"smooth"
		142	162	"smooth"
		143	163	"smooth"
		144	164	"smooth"
		145	165	"smooth"
		146	166	"smooth"
		147	167	"smooth"
		148	168	"smooth"
		149	169	"smooth"
		150	170	"smooth"
		151	171	"smooth"
		152	172	"smooth"
		153	173	"smooth"
		154	174	"smooth"
		155	175	"smooth"
		156	176	"smooth"
		157	177	"smooth"
		158	178	"smooth"
		159	179	"smooth"
		160	180	"smooth"
		161	181	"smooth"
		162	182	"smooth"
		163	183	"smooth"
		164	184	"smooth"
		165	185	"smooth"
		166	186	"smooth"
		167	187	"smooth"
		168	188	"smooth"
		169	189	"smooth"
		170	190	"smooth"
		171	191	"smooth"
		172	192	"smooth"
		173	193	"smooth"
		174	194	"smooth"
		175	195	"smooth"
		176	196	"smooth"
		177	197	"smooth"
		178	198	"smooth"
		179	199	"smooth"
		180	200	"smooth"
		181	201	"smooth"
		182	202	"smooth"
		183	203	"smooth"
		184	204	"smooth"
		185	205	"smooth"
		186	206	"smooth"
		187	207	"smooth"
		188	208	"smooth"
		189	209	"smooth"
		190	210	"smooth"
		191	211	"smooth"
		192	212	"smooth"
		193	213	"smooth"
		194	214	"smooth"
		195	215	"smooth"
		196	216	"smooth"
		197	217	"smooth"
		198	218	"smooth"
		199	219	"smooth"
		200	220	"smooth"
		201	221	"smooth"
		202	222	"smooth"
		203	223	"smooth"
		204	224	"smooth"
		205	225	"smooth"
		206	226	"smooth"
		207	227	"smooth"
		208	228	"smooth"
		209	229	"smooth"
		210	230	"smooth"
		211	231	"smooth"
		212	232	"smooth"
		213	233	"smooth"
		214	234	"smooth"
		215	235	"smooth"
		216	236	"smooth"
		217	237	"smooth"
		218	238	"smooth"
		219	239	"smooth"
		220	240	"smooth"
		221	241	"smooth"
		222	242	"smooth"
		223	243	"smooth"
		224	244	"smooth"
		225	245	"smooth"
		226	246	"smooth"
		227	247	"smooth"
		228	248	"smooth"
		229	249	"smooth"
		230	250	"smooth"
		231	251	"smooth"
		232	252	"smooth"
		233	253	"smooth"
		234	254	"smooth"
		235	255	"smooth"
		236	256	"smooth"
		237	257	"smooth"
		238	258	"smooth"
		239	259	"smooth"
		240	260	"smooth"
		241	261	"smooth"
		242	262	"smooth"
		243	263	"smooth"
		244	264	"smooth"
		245	265	"smooth"
		246	266	"smooth"
		247	267	"smooth"
		248	268	"smooth"
		249	269	"smooth"
		250	270	"smooth"
		251	271	"smooth"
		252	272	"smooth"
		253	273	"smooth"
		254	274	"smooth"
		255	275	"smooth"
		256	276	"smooth"
		257	277	"smooth"
		258	278	"smooth"
		259	279	"smooth"
		260	280	"smooth"
		261	281	"smooth"
		262	282	"smooth"
		263	283	"smooth"
		264	284	"smooth"
		265	285	"smooth"
		266	286	"smooth"
		267	287	"smooth"
		268	288	"smooth"
		269	289	"smooth"
		270	290	"smooth"
		271	291	"smooth"
		272	292	"smooth"
		273	293	"smooth"
		274	294	"smooth"
		275	295	"smooth"
		276	296	"smooth"
		277	297	"smooth"
		278	298	"smooth"
		279	299	"smooth"
		280	300	"smooth"
		281	301	"smooth"
		282	302	"smooth"
		283	303	"smooth"
		284	304	"smooth"
		285	305	"smooth"
		286	306	"smooth"
		287	307	"smooth"
		288	308	"smooth"
		289	309	"smooth"
		290	310	"smooth"
		291	311	"smooth"
		292	312	"smooth"
		293	313	"smooth"
		294	314	"smooth"
		295	315	"smooth"
		296	316	"smooth"
		297	317	"smooth"
		298	318	"smooth"
		299	319	"smooth"
		300	320	"smooth"
		301	321	"smooth"
		302	322	"smooth"
		303	323	"smooth"
		304	324	"smooth"
		305	325	"smooth"
		306	326	"smooth"
		307	327	"smooth"
		308	328	"smooth"
		309	329	"smooth"
		310	330	"smooth"
		311	331	"smooth"
		312	332	"smooth"
		313	333	"smooth"
		314	334	"smooth"
		315	335	"smooth"
		316	336	"smooth"
		317	337	"smooth"
		318	338	"smooth"
		319	339	"smooth"
		320	340	"smooth"
		321	341	"smooth"
		322	342	"smooth"
		323	343	"smooth"
		324	344	"smooth"
		325	345	"smooth"
		326	346	"smooth"
		327	347	"smooth"
		328	348	"smooth"
		329	349	"smooth"
		330	350	"smooth"
		331	351	"smooth"
		332	352	"smooth"
		333	353	"smooth"
		334	354	"smooth"
		335	355	"smooth"
		336	356	"smooth"
		337	357	"smooth"
		338	358	"smooth"
		339	359	"smooth"
		340	360	"smooth"
		341	361	"smooth"
		342	362	"smooth"
		343	363	"smooth"
		344	364	"smooth"
		345	365	"smooth"
		346	366	"smooth"
		347	367	"smooth"
		348	368	"smooth"
		349	369	"smooth"
		350	370	"smooth"
		351	371	"smooth"
		352	372	"smooth"
		353	373	"smooth"
		354	374	"smooth"
		355	375	"smooth"
		356	376	"smooth"
		357	377	"smooth"
		358	378	"smooth"
		359	379	"smooth"
		360	380	"smooth"
		361	381	"smooth"
		362	382	"smooth"
		363	383	"smooth"
		364	384	"smooth"
		365	385	"smooth"
		366	386	"smooth"
		367	387	"smooth"
		368	388	"smooth"
		369	389	"smooth"
		370	390	"smooth"
		371	391	"smooth"
		372	392	"smooth"
		373	393	"smooth"
		374	394	"smooth"
		375	395	"smooth"
		376	396	"smooth"
		377	397	"smooth"
		378	398	"smooth"
		379	399	"smooth"
		380	0	"smooth"
		381	1	"smooth"
		382	2	"smooth"
		383	3	"smooth"
		384	4	"smooth"
		385	5	"smooth"
		386	6	"smooth"
		387	7	"smooth"
		388	8	"smooth"
		389	9	"smooth"
		390	10	"smooth"
		391	11	"smooth"
		392	12	"smooth"
		393	13	"smooth"
		394	14	"smooth"
		395	15	"smooth"
		396	16	"smooth"
		397	17	"smooth"
		398	18	"smooth"
		399	19	"smooth"

		"face"	
		"l"	4	-1	400	20	-402	
		"lt"	4	1	0	21	22	

		"face"	
		"l"	4	-2	401	21	-403	
		"lt"	4	2	1	22	23	

		"face"	
		"l"	4	-3	402	22	-404	
		"lt"	4	3	2	23	24	

		"face"	
		"l"	4	-4	403	23	-405	
		"lt"	4	4	3	24	25	

		"face"	
		"l"	4	-5	404	24	-406	
		"lt"	4	5	4	25	26	

		"face"	
		"l"	4	-6	405	25	-407	
		"lt"	4	6	5	26	27	

		"face"	
		"l"	4	-7	406	26	-408	
		"lt"	4	7	6	27	28	

		"face"	
		"l"	4	-8	407	27	-409	
		"lt"	4	8	7	28	29	

		"face"	
		"l"	4	-9	408	28	-410	
		"lt"	4	9	8	29	30	

		"face"	
		"l"	4	-10	409	29	-411	
		"lt"	4	10	9	30	31	

		"face"	
		"l"	4	-11	410	30	-412	
		"lt"	4	11	10	31	32	

		"face"	
		"l"	4	-12	411	31	-413	
		"lt"	4	12	11	32	33	

		"face"	
		"l"	4	-13	412	32	-414	
		"lt"	4	13	12	33	34	

		"face"	
		"l"	4	-14	413	33	-415	
		"lt"	4	14	13	34	35	

		"face"	
		"l"	4	-15	414	34	-416	
		"lt"	4	15	14	35	36	

		"face"	
		"l"	4	-16	415	35	-417	
		"lt"	4	16	15	36	37	

		"face"	
		"l"	4	-17	416	36	-418	
		"lt"	4	17	16	37	38	

		"face"	
		"l"	4	-18	417	37	-419	
		"lt"	4	18	17	38	39	

		"face"	
		"l"	4	-19	418	38	-420	
		"lt"	4	19	18	39	40	

		"face"	
		"l"	4	-20	419	39	-401	
		"lt"	4	20	19	40	41	

		"face"	
		"l"	4	-21	420	40	-422	
		"lt"	4	22	21	42	43	

		"face"	
		"l"	4	-22	421	41	-423	
		"lt"	4	23	22	43	44	

		"face"	
		"l"	4	-23	422	42	-424	
		"lt"	4	24	23	44	45	

		"face"	
		"l"	4	-24	423	43	-425	
		"lt"	4	25	24	45	46	

		"face"	
		"l"	4	-25	424	44	-426	
		"lt"	4	26	25	46	47	

		"face"	
		"l"	4	-26	425	45	-427	
		"lt"	4	27	26	47	48	

		"face"	
		"l"	4	-27	426	46	-428	
		"lt"	4	28	27	48	49	

		"face"	
		"l"	4	-28	427	47	-429	
		"lt"	4	29	28	49	50	

		"face"	
		"l"	4	-29	428	48	-430	
		"lt"	4	30	29	50	51	

		"face"	
		"l"	4	-30	429	49	-431	
		"lt"	4	31	30	51	52	

		"face"	
		"l"	4	-31	430	50	-432	
		"lt"	4	32	31	52	53	

		"face"	
		"l"	4	-32	431	51	-433	
		"lt"	4	33	32	53	54	

		"face"	
		"l"	4	-33	432	52	-434	
		"lt"	4	34	33	54	55	

		"face"	
		"l"	4	-34	433	53	-435	
		"lt"	4	35	34	55	56	

		"face"	
		"l"	4	-35	434	54	-436	
		"lt"	4	36	35	56	57	

		"face"	
		"l"	4	-36	435	55	-437	
		"lt"	4	37	36	57	58	

		"face"	
		"l"	4	-37	436	56	-438	
		"lt"	4	38	37	58	59	

		"face"	
		"l"	4	-38	437	57	-439	
		"lt"	4	39	38	59	60	

		"face"	
		"l"	4	-39	438	58	-440	
		"lt"	4	40	39	60	61	

		"face"	
		"l"	4	-40	439	59	-421	
		"lt"	4	41	40	61	62	

		"face"	
		"l"	4	-41	440	60	-442	
		"lt"	4	43	42	63	64	

		"face"	
		"l"	4	-42	441	61	-443	
		"lt"	4	44	43	64	65	

		"face"	
		"l"	4	-43	442	62	-444	
		"lt"	4	45	44	65	66	

		"face"	
		"l"	4	-44	443	63	-445	
		"lt"	4	46	45	66	67	

		"face"	
		"l"	4	-45	444	64	-446	
		"lt"	4	47	46	67	68	

		"face"	
		"l"	4	-46	445	65	-447	
		"lt"	4	48	47	68	69	

		"face"	
		"l"	4	-47	446	66	-448	
		"lt"	4	49	48	69	70	

		"face"	
		"l"	4	-48	447	67	-449	
		"lt"	4	50	49	70	71	

		"face"	
		"l"	4	-49	448	68	-450	
		"lt"	4	51	50	71	72	

		"face"	
		"l"	4	-50	449	69	-451	
		"lt"	4	52	51	72	73	

		"face"	
		"l"	4	-51	450	70	-452	
		"lt"	4	53	52	73	74	

		"face"	
		"l"	4	-52	451	71	-453	
		"lt"	4	54	53	74	75	

		"face"	
		"l"	4	-53	452	72	-454	
		"lt"	4	55	54	75	76	

		"face"	
		"l"	4	-54	453	73	-455	
		"lt"	4	56	55	76	77	

		"face"	
		"l"	4	-55	454	74	-456	
		"lt"	4	57	56	77	78	

		"face"	
		"l"	4	-56	455	75	-457	
		"lt"	4	58	57	78	79	

		"face"	
		"l"	4	-57	456	76	-458	
		"lt"	4	59	58	79	80	

		"face"	
		"l"	4	-58	457	77	-459	
		"lt"	4	60	59	80	81	

		"face"	
		"l"	4	-59	458	78	-460	
		"lt"	4	61	60	81	82	

		"face"	
		"l"	4	-60	459	79	-441	
		"lt"	4	62	61	82	83	

		"face"	
		"l"	4	-61	460	80	-462	
		"lt"	4	64	63	84	85	

		"face"	
		"l"	4	-62	461	81	-463	
		"lt"	4	65	64	85	86	

		"face"	
		"l"	4	-63	462	82	-464	
		"lt"	4	66	65	86	87	

		"face"	
		"l"	4	-64	463	83	-465	
		"lt"	4	67	66	87	88	

		"face"	
		"l"	4	-65	464	84	-466	
		"lt"	4	68	67	88	89	

		"face"	
		"l"	4	-66	465	85	-467	
		"lt"	4	69	68	89	90	

		"face"	
		"l"	4	-67	466	86	-468	
		"lt"	4	70	69	90	91	

		"face"	
		"l"	4	-68	467	87	-469	
		"lt"	4	71	70	91	92	

		"face"	
		"l"	4	-69	468	88	-470	
		"lt"	4	72	71	92	93	

		"face"	
		"l"	4	-70	469	89	-471	
		"lt"	4	73	72	93	94	

		"face"	
		"l"	4	-71	470	90	-472	
		"lt"	4	74	73	94	95	

		"face"	
		"l"	4	-72	471	91	-473	
		"lt"	4	75	74	95	96	

		"face"	
		"l"	4	-73	472	92	-474	
		"lt"	4	76	75	96	97	

		"face"	
		"l"	4	-74	473	93	-475	
		"lt"	4	77	76	97	98	

		"face"	
		"l"	4	-75	474	94	-476	
		"lt"	4	78	77	98	99	

		"face"	
		"l"	4	-76	475	95	-477	
		"lt"	4	79	78	99	100	

		"face"	
		"l"	4	-77	476	96	-478	
		"lt"	4	80	79	100	101	

		"face"	
		"l"	4	-78	477	97	-479	
		"lt"	4	81	80	101	102	

		"face"	
		"l"	4	-79	478	98	-480	
		"lt"	4	82	81	102	103	

		"face"	
		"l"	4	-80	479	99	-461	
		"lt"	4	83	82	103	104	

		"face"	
		"l"	4	-81	480	100	-482	
		"lt"	4	85	84	105	106	

		"face"	
		"l"	4	-82	481	101	-483	
		"lt"	4	86	85	106	107	

		"face"	
		"l"	4	-83	482	102	-484	
		"lt"	4	87	86	107	108	

		"face"	
		"l"	4	-84	483	103	-485	
		"lt"	4	88	87	108	109	

		"face"	
		"l"	4	-85	484	104	-486	
		"lt"	4	89	88	109	110	

		"face"	
		"l"	4	-86	485	105	-487	
		"lt"	4	90	89	110	111	

		"face"	
		"l"	4	-87	486	106	-488	
		"lt"	4	91	90	111	112	

		"face"	
		"l"	4	-88	487	107	-489	
		"lt"	4	92	91	112	113	

		"face"	
		"l"	4	-89	488	108	-490	
		"lt"	4	93	92	113	114	

		"face"	
		"l"	4	-90	489	109	-491	
		"lt"	4	94	93	114	115	

		"face"	
		"l"	4	-91	490	110	-492	
		"lt"	4	95	94	115	116	

		"face"	
		"l"	4	-92	491	111	-493	
		"lt"	4	96	95	116	117	

		"face"	
		"l"	4	-93	492	112	-494	
		"lt"	4	97	96	117	118	

		"face"	
		"l"	4	-94	493	113	-495	
		"lt"	4	98	97	118	119	

		"face"	
		"l"	4	-95	494	114	-496	
		"lt"	4	99	98	119	120	

		"face"	
		"l"	4	-96	495	115	-497	
		"lt"	4	100	99	120	121	

		"face"	
		"l"	4	-97	496	116	-498	
		"lt"	4	101	100	121	122	

		"face"	
		"l"	4	-98	497	117	-499	
		"lt"	4	102	101	122	123	

		"face"	
		"l"	4	-99	498	118	-500	
		"lt"	4	103	102	123	124	

		"face"	
		"l"	4	-100	499	119	-481	
		"lt"	4	104	103	124	125	

		"face"	
		"l"	4	-101	500	120	-502	
		"lt"	4	106	105	126	127	

		"face"	
		"l"	4	-102	501	121	-503	
		"lt"	4	107	106	127	128	

		"face"	
		"l"	4	-103	502	122	-504	
		"lt"	4	108	107	128	129	

		"face"	
		"l"	4	-104	503	123	-505	
		"lt"	4	109	108	129	130	

		"face"	
		"l"	4	-105	504	124	-506	
		"lt"	4	110	109	130	131	

		"face"	
		"l"	4	-106	505	125	-507	
		"lt"	4	111	110	131	132	

		"face"	
		"l"	4	-107	506	126	-508	
		"lt"	4	112	111	132	133	

		"face"	
		"l"	4	-108	507	127	-509	
		"lt"	4	113	112	133	134	

		"face"	
		"l"	4	-109	508	128	-510	
		"lt"	4	114	113	134	135	

		"face"	
		"l"	4	-110	509	129	-511	
		"lt"	4	115	114	135	136	

		"face"	
		"l"	4	-111	510	130	-512	
		"lt"	4	116	115	136	137	

		"face"	
		"l"	4	-112	511	131	-513	
		"lt"	4	117	116	137	138	

		"face"	
		"l"	4	-113	512	132	-514	
		"lt"	4	118	117	138	139	

		"face"	
		"l"	4	-114	513	133	-515	
		"lt"	4	119	118	139	140	

		"face"	
		"l"	4	-115	514	134	-516	
		"lt"	4	120	119	140	141	

		"face"	
		"l"	4	-116	515	135	-517	
		"lt"	4	121	120	141	142	

		"face"	
		"l"	4	-117	516	136	-518	
		"lt"	4	122	121	142	143	

		"face"	
		"l"	4	-118	517	137	-519	
		"lt"	4	123	122	143	144	

		"face"	
		"l"	4	-119	518	138	-520	
		"lt"	4	124	123	144	145	

		"face"	
		"l"	4	-120	519	139	-501	
		"lt"	4	125	124	145	146	

		"face"	
		"l"	4	-121	520	140	-522	
		"lt"	4	127	126	147	148	

		"face"	
		"l"	4	-122	521	141	-523	
		"lt"	4	128	127	148	149	

		"face"	
		"l"	4	-123	522	142	-524	
		"lt"	4	129	128	149	150	

		"face"	
		"l"	4	-124	523	143	-525	
		"lt"	4	130	129	150	151	

		"face"	
		"l"	4	-125	524	144	-526	
		"lt"	4	131	130	151	152	

		"face"	
		"l"	4	-126	525	145	-527	
		"lt"	4	132	131	152	153	

		"face"	
		"l"	4	-127	526	146	-528	
		"lt"	4	133	132	153	154	

		"face"	
		"l"	4	-128	527	147	-529	
		"lt"	4	134	133	154	155	

		"face"	
		"l"	4	-129	528	148	-530	
		"lt"	4	135	134	155	156	

		"face"	
		"l"	4	-130	529	149	-531	
		"lt"	4	136	135	156	157	

		"face"	
		"l"	4	-131	530	150	-532	
		"lt"	4	137	136	157	158	

		"face"	
		"l"	4	-132	531	151	-533	
		"lt"	4	138	137	158	159	

		"face"	
		"l"	4	-133	532	152	-534	
		"lt"	4	139	138	159	160	

		"face"	
		"l"	4	-134	533	153	-535	
		"lt"	4	140	139	160	161	

		"face"	
		"l"	4	-135	534	154	-536	
		"lt"	4	141	140	161	162	

		"face"	
		"l"	4	-136	535	155	-537	
		"lt"	4	142	141	162	163	

		"face"	
		"l"	4	-137	536	156	-538	
		"lt"	4	143	142	163	164	

		"face"	
		"l"	4	-138	537	157	-539	
		"lt"	4	144	143	164	165	

		"face"	
		"l"	4	-139	538	158	-540	
		"lt"	4	145	144	165	166	

		"face"	
		"l"	4	-140	539	159	-521	
		"lt"	4	146	145	166	167	

		"face"	
		"l"	4	-141	540	160	-542	
		"lt"	4	148	147	168	169	

		"face"	
		"l"	4	-142	541	161	-543	
		"lt"	4	149	148	169	170	

		"face"	
		"l"	4	-143	542	162	-544	
		"lt"	4	150	149	170	171	

		"face"	
		"l"	4	-144	543	163	-545	
		"lt"	4	151	150	171	172	

		"face"	
		"l"	4	-145	544	164	-546	
		"lt"	4	152	151	172	173	

		"face"	
		"l"	4	-146	545	165	-547	
		"lt"	4	153	152	173	174	

		"face"	
		"l"	4	-147	546	166	-548	
		"lt"	4	154	153	174	175	

		"face"	
		"l"	4	-148	547	167	-549	
		"lt"	4	155	154	175	176	

		"face"	
		"l"	4	-149	548	168	-550	
		"lt"	4	156	155	176	177	

		"face"	
		"l"	4	-150	549	169	-551	
		"lt"	4	157	156	177	178	

		"face"	
		"l"	4	-151	550	170	-552	
		"lt"	4	158	157	178	179	

		"face"	
		"l"	4	-152	551	171	-553	
		"lt"	4	159	158	179	180	

		"face"	
		"l"	4	-153	552	172	-554	
		"lt"	4	160	159	180	181	

		"face"	
		"l"	4	-154	553	173	-555	
		"lt"	4	161	160	181	182	

		"face"	
		"l"	4	-155	554	174	-556	
		"lt"	4	162	161	182	183	

		"face"	
		"l"	4	-156	555	175	-557	
		"lt"	4	163	162	183	184	

		"face"	
		"l"	4	-157	556	176	-558	
		"lt"	4	164	163	184	185	

		"face"	
		"l"	4	-158	557	177	-559	
		"lt"	4	165	164	185	186	

		"face"	
		"l"	4	-159	558	178	-560	
		"lt"	4	166	165	186	187	

		"face"	
		"l"	4	-160	559	179	-541	
		"lt"	4	167	166	187	188	

		"face"	
		"l"	4	-161	560	180	-562	
		"lt"	4	169	168	189	190	

		"face"	
		"l"	4	-162	561	181	-563	
		"lt"	4	170	169	190	191	

		"face"	
		"l"	4	-163	562	182	-564	
		"lt"	4	171	170	191	192	

		"face"	
		"l"	4	-164	563	183	-565	
		"lt"	4	172	171	192	193	

		"face"	
		"l"	4	-165	564	184	-566	
		"lt"	4	173	172	193	194	

		"face"	
		"l"	4	-166	565	185	-567	
		"lt"	4	174	173	194	195	

		"face"	
		"l"	4	-167	566	186	-568	
		"lt"	4	175	174	195	196	

		"face"	
		"l"	4	-168	567	187	-569	
		"lt"	4	176	175	196	197	

		"face"	
		"l"	4	-169	568	188	-570	
		"lt"	4	177	176	197	198	

		"face"	
		"l"	4	-170	569	189	-571	
		"lt"	4	178	177	198	199	

		"face"	
		"l"	4	-171	570	190	-572	
		"lt"	4	179	178	199	200	

		"face"	
		"l"	4	-172	571	191	-573	
		"lt"	4	180	179	200	201	

		"face"	
		"l"	4	-173	572	192	-574	
		"lt"	4	181	180	201	202	

		"face"	
		"l"	4	-174	573	193	-575	
		"lt"	4	182	181	202	203	

		"face"	
		"l"	4	-175	574	194	-576	
		"lt"	4	183	182	203	204	

		"face"	
		"l"	4	-176	575	195	-577	
		"lt"	4	184	183	204	205	

		"face"	
		"l"	4	-177	576	196	-578	
		"lt"	4	185	184	205	206	

		"face"	
		"l"	4	-178	577	197	-579	
		"lt"	4	186	185	206	207	

		"face"	
		"l"	4	-179	578	198	-580	
		"lt"	4	187	186	207	208	

		"face"	
		"l"	4	-180	579	199	-561	
		"lt"	4	188	187	208	209	

		"face"	
		"l"	4	-181	580	200	-582	
		"lt"	4	190	189	210	211	

		"face"	
		"l"	4	-182	581	201	-583	
		"lt"	4	191	190	211	212	

		"face"	
		"l"	4	-183	582	202	-584	
		"lt"	4	192	191	212	213	

		"face"	
		"l"	4	-184	583	203	-585	
		"lt"	4	193	192	213	214	

		"face"	
		"l"	4	-185	584	204	-586	
		"lt"	4	194	193	214	215	

		"face"	
		"l"	4	-186	585	205	-587	
		"lt"	4	195	194	215	216	

		"face"	
		"l"	4	-187	586	206	-588	
		"lt"	4	196	195	216	217	

		"face"	
		"l"	4	-188	587	207	-589	
		"lt"	4	197	196	217	218	

		"face"	
		"l"	4	-189	588	208	-590	
		"lt"	4	198	197	218	219	

		"face"	
		"l"	4	-190	589	209	-591	
		"lt"	4	199	198	219	220	

		"face"	
		"l"	4	-191	590	210	-592	
		"lt"	4	200	199	220	221	

		"face"	
		"l"	4	-192	591	211	-593	
		"lt"	4	201	200	221	222	

		"face"	
		"l"	4	-193	592	212	-594	
		"lt"	4	202	201	222	223	

		"face"	
		"l"	4	-194	593	213	-595	
		"lt"	4	203	202	223	224	

		"face"	
		"l"	4	-195	594	214	-596	
		"lt"	4	204	203	224	225	

		"face"	
		"l"	4	-196	595	215	-597	
		"lt"	4	205	204	225	226	

		"face"	
		"l"	4	-197	596	216	-598	
		"lt"	4	206	205	226	227	

		"face"	
		"l"	4	-198	597	217	-599	
		"lt"	4	207	206	227	228	

		"face"	
		"l"	4	-199	598	218	-600	
		"lt"	4	208	207	228	229	

		"face"	
		"l"	4	-200	599	219	-581	
		"lt"	4	209	208	229	230	

		"face"	
		"l"	4	-201	600	220	-602	
		"lt"	4	211	210	231	232	

		"face"	
		"l"	4	-202	601	221	-603	
		"lt"	4	212	211	232	233	

		"face"	
		"l"	4	-203	602	222	-604	
		"lt"	4	213	212	233	234	

		"face"	
		"l"	4	-204	603	223	-605	
		"lt"	4	214	213	234	235	

		"face"	
		"l"	4	-205	604	224	-606	
		"lt"	4	215	214	235	236	

		"face"	
		"l"	4	-206	605	225	-607	
		"lt"	4	216	215	236	237	

		"face"	
		"l"	4	-207	606	226	-608	
		"lt"	4	217	216	237	238	

		"face"	
		"l"	4	-208	607	227	-609	
		"lt"	4	218	217	238	239	

		"face"	
		"l"	4	-209	608	228	-610	
		"lt"	4	219	218	239	240	

		"face"	
		"l"	4	-210	609	229	-611	
		"lt"	4	220	219	240	241	

		"face"	
		"l"	4	-211	610	230	-612	
		"lt"	4	221	220	241	242	

		"face"	
		"l"	4	-212	611	231	-613	
		"lt"	4	222	221	242	243	

		"face"	
		"l"	4	-213	612	232	-614	
		"lt"	4	223	222	243	244	

		"face"	
		"l"	4	-214	613	233	-615	
		"lt"	4	224	223	244	245	

		"face"	
		"l"	4	-215	614	234	-616	
		"lt"	4	225	224	245	246	

		"face"	
		"l"	4	-216	615	235	-617	
		"lt"	4	226	225	246	247	

		"face"	
		"l"	4	-217	616	236	-618	
		"lt"	4	227	226	247	248	

		"face"	
		"l"	4	-218	617	237	-619	
		"lt"	4	228	227	248	249	

		"face"	
		"l"	4	-219	618	238	-620	
		"lt"	4	229	228	249	250	

		"face"	
		"l"	4	-220	619	239	-601	
		"lt"	4	230	229	250	251	

		"face"	
		"l"	4	-221	620	240	-622	
		"lt"	4	232	231	252	253	

		"face"	
		"l"	4	-222	621	241	-623	
		"lt"	4	233	232	253	254	

		"face"	
		"l"	4	-223	622	242	-624	
		"lt"	4	234	233	254	255	

		"face"	
		"l"	4	-224	623	243	-625	
		"lt"	4	235	234	255	256	

		"face"	
		"l"	4	-225	624	244	-626	
		"lt"	4	236	235	256	257	

		"face"	
		"l"	4	-226	625	245	-627	
		"lt"	4	237	236	257	258	

		"face"	
		"l"	4	-227	626	246	-628	
		"lt"	4	238	237	258	259	

		"face"	
		"l"	4	-228	627	247	-629	
		"lt"	4	239	238	259	260	

		"face"	
		"l"	4	-229	628	248	-630	
		"lt"	4	240	239	260	261	

		"face"	
		"l"	4	-230	629	249	-631	
		"lt"	4	241	240	261	262	

		"face"	
		"l"	4	-231	630	250	-632	
		"lt"	4	242	241	262	263	

		"face"	
		"l"	4	-232	631	251	-633	
		"lt"	4	243	242	263	264	

		"face"	
		"l"	4	-233	632	252	-634	
		"lt"	4	244	243	264	265	

		"face"	
		"l"	4	-234	633	253	-635	
		"lt"	4	245	244	265	266	

		"face"	
		"l"	4	-235	634	254	-636	
		"lt"	4	246	245	266	267	

		"face"	
		"l"	4	-236	635	255	-637	
		"lt"	4	247	246	267	268	

		"face"	
		"l"	4	-237	636	256	-638	
		"lt"	4	248	247	268	269	

		"face"	
		"l"	4	-238	637	257	-639	
		"lt"	4	249	248	269	270	

		"face"	
		"l"	4	-239	638	258	-640	
		"lt"	4	250	249	270	271	

		"face"	
		"l"	4	-240	639	259	-621	
		"lt"	4	251	250	271	272	

		"face"	
		"l"	4	-241	640	260	-642	
		"lt"	4	253	252	273	274	

		"face"	
		"l"	4	-242	641	261	-643	
		"lt"	4	254	253	274	275	

		"face"	
		"l"	4	-243	642	262	-644	
		"lt"	4	255	254	275	276	

		"face"	
		"l"	4	-244	643	263	-645	
		"lt"	4	256	255	276	277	

		"face"	
		"l"	4	-245	644	264	-646	
		"lt"	4	257	256	277	278	

		"face"	
		"l"	4	-246	645	265	-647	
		"lt"	4	258	257	278	279	

		"face"	
		"l"	4	-247	646	266	-648	
		"lt"	4	259	258	279	280	

		"face"	
		"l"	4	-248	647	267	-649	
		"lt"	4	260	259	280	281	

		"face"	
		"l"	4	-249	648	268	-650	
		"lt"	4	261	260	281	282	

		"face"	
		"l"	4	-250	649	269	-651	
		"lt"	4	262	261	282	283	

		"face"	
		"l"	4	-251	650	270	-652	
		"lt"	4	263	262	283	284	

		"face"	
		"l"	4	-252	651	271	-653	
		"lt"	4	264	263	284	285	

		"face"	
		"l"	4	-253	652	272	-654	
		"lt"	4	265	264	285	286	

		"face"	
		"l"	4	-254	653	273	-655	
		"lt"	4	266	265	286	287	

		"face"	
		"l"	4	-255	654	274	-656	
		"lt"	4	267	266	287	288	

		"face"	
		"l"	4	-256	655	275	-657	
		"lt"	4	268	267	288	289	

		"face"	
		"l"	4	-257	656	276	-658	
		"lt"	4	269	268	289	290	

		"face"	
		"l"	4	-258	657	277	-659	
		"lt"	4	270	269	290	291	

		"face"	
		"l"	4	-259	658	278	-660	
		"lt"	4	271	270	291	292	

		"face"	
		"l"	4	-260	659	279	-641	
		"lt"	4	272	271	292	293	

		"face"	
		"l"	4	-261	660	280	-662	
		"lt"	4	274	273	294	295	

		"face"	
		"l"	4	-262	661	281	-663	
		"lt"	4	275	274	295	296	

		"face"	
		"l"	4	-263	662	282	-664	
		"lt"	4	276	275	296	297	

		"face"	
		"l"	4	-264	663	283	-665	
		"lt"	4	277	276	297	298	

		"face"	
		"l"	4	-265	664	284	-666	
		"lt"	4	278	277	298	299	

		"face"	
		"l"	4	-266	665	285	-667	
		"lt"	4	279	278	299	300	

		"face"	
		"l"	4	-267	666	286	-668	
		"lt"	4	280	279	300	301	

		"face"	
		"l"	4	-268	667	287	-669	
		"lt"	4	281	280	301	302	

		"face"	
		"l"	4	-269	668	288	-670	
		"lt"	4	282	281	302	303	

		"face"	
		"l"	4	-270	669	289	-671	
		"lt"	4	283	282	303	304	

		"face"	
		"l"	4	-271	670	290	-672	
		"lt"	4	284	283	304	305	

		"face"	
		"l"	4	-272	671	291	-673	
		"lt"	4	285	284	305	306	

		"face"	
		"l"	4	-273	672	292	-674	
		"lt"	4	286	285	306	307	

		"face"	
		"l"	4	-274	673	293	-675	
		"lt"	4	287	286	307	308	

		"face"	
		"l"	4	-275	674	294	-676	
		"lt"	4	288	287	308	309	

		"face"	
		"l"	4	-276	675	295	-677	
		"lt"	4	289	288	309	310	

		"face"	
		"l"	4	-277	676	296	-678	
		"lt"	4	290	289	310	311	

		"face"	
		"l"	4	-278	677	297	-679	
		"lt"	4	291	290	311	312	

		"face"	
		"l"	4	-279	678	298	-680	
		"lt"	4	292	291	312	313	

		"face"	
		"l"	4	-280	679	299	-661	
		"lt"	4	293	292	313	314	

		"face"	
		"l"	4	-281	680	300	-682	
		"lt"	4	295	294	315	316	

		"face"	
		"l"	4	-282	681	301	-683	
		"lt"	4	296	295	316	317	

		"face"	
		"l"	4	-283	682	302	-684	
		"lt"	4	297	296	317	318	

		"face"	
		"l"	4	-284	683	303	-685	
		"lt"	4	298	297	318	319	

		"face"	
		"l"	4	-285	684	304	-686	
		"lt"	4	299	298	319	320	

		"face"	
		"l"	4	-286	685	305	-687	
		"lt"	4	300	299	320	321	

		"face"	
		"l"	4	-287	686	306	-688	
		"lt"	4	301	300	321	322	

		"face"	
		"l"	4	-288	687	307	-689	
		"lt"	4	302	301	322	323	

		"face"	
		"l"	4	-289	688	308	-690	
		"lt"	4	303	302	323	324	

		"face"	
		"l"	4	-290	689	309	-691	
		"lt"	4	304	303	324	325	

		"face"	
		"l"	4	-291	690	310	-692	
		"lt"	4	305	304	325	326	

		"face"	
		"l"	4	-292	691	311	-693	
		"lt"	4	306	305	326	327	

		"face"	
		"l"	4	-293	692	312	-694	
		"lt"	4	307	306	327	328	

		"face"	
		"l"	4	-294	693	313	-695	
		"lt"	4	308	307	328	329	

		"face"	
		"l"	4	-295	694	314	-696	
		"lt"	4	309	308	329	330	

		"face"	
		"l"	4	-296	695	315	-697	
		"lt"	4	310	309	330	331	

		"face"	
		"l"	4	-297	696	316	-698	
		"lt"	4	311	310	331	332	

		"face"	
		"l"	4	-298	697	317	-699	
		"lt"	4	312	311	332	333	

		"face"	
		"l"	4	-299	698	318	-700	
		"lt"	4	313	312	333	334	

		"face"	
		"l"	4	-300	699	319	-681	
		"lt"	4	314	313	334	335	

		"face"	
		"l"	4	-301	700	320	-702	
		"lt"	4	316	315	336	337	

		"face"	
		"l"	4	-302	701	321	-703	
		"lt"	4	317	316	337	338	

		"face"	
		"l"	4	-303	702	322	-704	
		"lt"	4	318	317	338	339	

		"face"	
		"l"	4	-304	703	323	-705	
		"lt"	4	319	318	339	340	

		"face"	
		"l"	4	-305	704	324	-706	
		"lt"	4	320	319	340	341	

		"face"	
		"l"	4	-306	705	325	-707	
		"lt"	4	321	320	341	342	

		"face"	
		"l"	4	-307	706	326	-708	
		"lt"	4	322	321	342	343	

		"face"	
		"l"	4	-308	707	327	-709	
		"lt"	4	323	322	343	344	

		"face"	
		"l"	4	-309	708	328	-710	
		"lt"	4	324	323	344	345	

		"face"	
		"l"	4	-310	709	329	-711	
		"lt"	4	325	324	345	346	

		"face"	
		"l"	4	-311	710	330	-712	
		"lt"	4	326	325	346	347	

		"face"	
		"l"	4	-312	711	331	-713	
		"lt"	4	327	326	347	348	

		"face"	
		"l"	4	-313	712	332	-714	
		"lt"	4	328	327	348	349	

		"face"	
		"l"	4	-314	713	333	-715	
		"lt"	4	329	328	349	350	

		"face"	
		"l"	4	-315	714	334	-716	
		"lt"	4	330	329	350	351	

		"face"	
		"l"	4	-316	715	335	-717	
		"lt"	4	331	330	351	352	

		"face"	
		"l"	4	-317	716	336	-718	
		"lt"	4	332	331	352	353	

		"face"	
		"l"	4	-318	717	337	-719	
		"lt"	4	333	332	353	354	

		"face"	
		"l"	4	-319	718	338	-720	
		"lt"	4	334	333	354	355	

		"face"	
		"l"	4	-320	719	339	-701	
		"lt"	4	335	334	355	356	

		"face"	
		"l"	4	-321	720	340	-722	
		"lt"	4	337	336	357	358	

		"face"	
		"l"	4	-322	721	341	-723	
		"lt"	4	338	337	358	359	

		"face"	
		"l"	4	-323	722	342	-724	
		"lt"	4	339	338	359	360	

		"face"	
		"l"	4	-324	723	343	-725	
		"lt"	4	340	339	360	361	

		"face"	
		"l"	4	-325	724	344	-726	
		"lt"	4	341	340	361	362	

		"face"	
		"l"	4	-326	725	345	-727	
		"lt"	4	342	341	362	363	

		"face"	
		"l"	4	-327	726	346	-728	
		"lt"	4	343	342	363	364	

		"face"	
		"l"	4	-328	727	347	-729	
		"lt"	4	344	343	364	365	

		"face"	
		"l"	4	-329	728	348	-730	
		"lt"	4	345	344	365	366	

		"face"	
		"l"	4	-330	729	349	-731	
		"lt"	4	346	345	366	367	

		"face"	
		"l"	4	-331	730	350	-732	
		"lt"	4	347	346	367	368	

		"face"	
		"l"	4	-332	731	351	-733	
		"lt"	4	348	347	368	369	

		"face"	
		"l"	4	-333	732	352	-734	
		"lt"	4	349	348	369	370	

		"face"	
		"l"	4	-334	733	353	-735	
		"lt"	4	350	349	370	371	

		"face"	
		"l"	4	-335	734	354	-736	
		"lt"	4	351	350	371	372	

		"face"	
		"l"	4	-336	735	355	-737	
		"lt"	4	352	351	372	373	

		"face"	
		"l"	4	-337	736	356	-738	
		"lt"	4	353	352	373	374	

		"face"	
		"l"	4	-338	737	357	-739	
		"lt"	4	354	353	374	375	

		"face"	
		"l"	4	-339	738	358	-740	
		"lt"	4	355	354	375	376	

		"face"	
		"l"	4	-340	739	359	-721	
		"lt"	4	356	355	376	377	

		"face"	
		"l"	4	-341	740	360	-742	
		"lt"	4	358	357	378	379	

		"face"	
		"l"	4	-342	741	361	-743	
		"lt"	4	359	358	379	380	

		"face"	
		"l"	4	-343	742	362	-744	
		"lt"	4	360	359	380	381	

		"face"	
		"l"	4	-344	743	363	-745	
		"lt"	4	361	360	381	382	

		"face"	
		"l"	4	-345	744	364	-746	
		"lt"	4	362	361	382	383	

		"face"	
		"l"	4	-346	745	365	-747	
		"lt"	4	363	362	383	384	

		"face"	
		"l"	4	-347	746	366	-748	
		"lt"	4	364	363	384	385	

		"face"	
		"l"	4	-348	747	367	-749	
		"lt"	4	365	364	385	386	

		"face"	
		"l"	4	-349	748	368	-750	
		"lt"	4	366	365	386	387	

		"face"	
		"l"	4	-350	749	369	-751	
		"lt"	4	367	366	387	388	

		"face"	
		"l"	4	-351	750	370	-752	
		"lt"	4	368	367	388	389	

		"face"	
		"l"	4	-352	751	371	-753	
		"lt"	4	369	368	389	390	

		"face"	
		"l"	4	-353	752	372	-754	
		"lt"	4	370	369	390	391	

		"face"	
		"l"	4	-354	753	373	-755	
		"lt"	4	371	370	391	392	

		"face"	
		"l"	4	-355	754	374	-756	
		"lt"	4	372	371	392	393	

		"face"	
		"l"	4	-356	755	375	-757	
		"lt"	4	373	372	393	394	

		"face"	
		"l"	4	-357	756	376	-758	
		"lt"	4	374	373	394	395	

		"face"	
		"l"	4	-358	757	377	-759	
		"lt"	4	375	374	395	396	

		"face"	
		"l"	4	-359	758	378	-760	
		"lt"	4	376	375	396	397	

		"face"	
		"l"	4	-360	759	379	-741	
		"lt"	4	377	376	397	398	

		"face"	
		"l"	4	-361	760	380	-762	
		"lt"	4	379	378	399	400	

		"face"	
		"l"	4	-362	761	381	-763	
		"lt"	4	380	379	400	401	

		"face"	
		"l"	4	-363	762	382	-764	
		"lt"	4	381	380	401	402	

		"face"	
		"l"	4	-364	763	383	-765	
		"lt"	4	382	381	402	403	

		"face"	
		"l"	4	-365	764	384	-766	
		"lt"	4	383	382	403	404	

		"face"	
		"l"	4	-366	765	385	-767	
		"lt"	4	384	383	404	405	

		"face"	
		"l"	4	-367	766	386	-768	
		"lt"	4	385	384	405	406	

		"face"	
		"l"	4	-368	767	387	-769	
		"lt"	4	386	385	406	407	

		"face"	
		"l"	4	-369	768	388	-770	
		"lt"	4	387	386	407	408	

		"face"	
		"l"	4	-370	769	389	-771	
		"lt"	4	388	387	408	409	

		"face"	
		"l"	4	-371	770	390	-772	
		"lt"	4	389	388	409	410	

		"face"	
		"l"	4	-372	771	391	-773	
		"lt"	4	390	389	410	411	

		"face"	
		"l"	4	-373	772	392	-774	
		"lt"	4	391	390	411	412	

		"face"	
		"l"	4	-374	773	393	-775	
		"lt"	4	392	391	412	413	

		"face"	
		"l"	4	-375	774	394	-776	
		"lt"	4	393	392	413	414	

		"face"	
		"l"	4	-376	775	395	-777	
		"lt"	4	394	393	414	415	

		"face"	
		"l"	4	-377	776	396	-778	
		"lt"	4	395	394	415	416	

		"face"	
		"l"	4	-378	777	397	-779	
		"lt"	4	396	395	416	417	

		"face"	
		"l"	4	-379	778	398	-780	
		"lt"	4	397	396	417	418	

		"face"	
		"l"	4	-380	779	399	-761	
		"lt"	4	398	397	418	419	

		"face"	
		"l"	4	-381	780	0	-782	
		"lt"	4	400	399	420	421	

		"face"	
		"l"	4	-382	781	1	-783	
		"lt"	4	401	400	421	422	

		"face"	
		"l"	4	-383	782	2	-784	
		"lt"	4	402	401	422	423	

		"face"	
		"l"	4	-384	783	3	-785	
		"lt"	4	403	402	423	424	

		"face"	
		"l"	4	-385	784	4	-786	
		"lt"	4	404	403	424	425	

		"face"	
		"l"	4	-386	785	5	-787	
		"lt"	4	405	404	425	426	

		"face"	
		"l"	4	-387	786	6	-788	
		"lt"	4	406	405	426	427	

		"face"	
		"l"	4	-388	787	7	-789	
		"lt"	4	407	406	427	428	

		"face"	
		"l"	4	-389	788	8	-790	
		"lt"	4	408	407	428	429	

		"face"	
		"l"	4	-390	789	9	-791	
		"lt"	4	409	408	429	430	

		"face"	
		"l"	4	-391	790	10	-792	
		"lt"	4	410	409	430	431	

		"face"	
		"l"	4	-392	791	11	-793	
		"lt"	4	411	410	431	432	

		"face"	
		"l"	4	-393	792	12	-794	
		"lt"	4	412	411	432	433	

		"face"	
		"l"	4	-394	793	13	-795	
		"lt"	4	413	412	433	434	

		"face"	
		"l"	4	-395	794	14	-796	
		"lt"	4	414	413	434	435	

		"face"	
		"l"	4	-396	795	15	-797	
		"lt"	4	415	414	435	436	

		"face"	
		"l"	4	-397	796	16	-798	
		"lt"	4	416	415	436	437	

		"face"	
		"l"	4	-398	797	17	-799	
		"lt"	4	417	416	437	438	

		"face"	
		"l"	4	-399	798	18	-800	
		"lt"	4	418	417	438	439	

		"face"	
		"l"	4	-400	799	19	-781	
		"lt"	4	419	418	439	440	;
	setAttr -k on ".ai_translator" -type "string" "mesh";
createNode transform -n "aiAreaLight2";
	setAttr ".t" -type "double3" -2.2239539486673392 0.18899808861519968 1.793519969370466 ;
	setAttr ".r" -type "double3" 170.40667171453561 -36.996350506505756 19.143380978332281 ;
	setAttr ".s" -type "double3" 1 4.189613730723849 1 ;
createNode aiAreaLight -n "aiAreaLightShape2" -p "aiAreaLight2";
	addAttr -ci true -sn "ai_cast_shadows" -ln "aiCastShadows" -dv 1 -min 0 -max 1 
		-at "bool";
	addAttr -ci true -sn "ai_shadow_density" -ln "aiShadowDensity" -dv 1 -min 0 -max 
		1 -smn 0 -smx 1 -at "float";
	addAttr -ci true -sn "ai_exposure" -ln "aiExposure" -min 0 -smx 10 -at "float";
	addAttr -ci true -sn "ai_samples" -ln "aiSamples" -dv 1 -min 0 -max 100 -at "long";
	addAttr -ci true -sn "ai_normalize" -ln "aiNormalize" -dv 1 -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "ai_bounce_factor" -ln "aiBounceFactor" -dv 1 -at "float";
	addAttr -ci true -sn "ai_bounces" -ln "aiBounces" -dv 999 -at "long";
	addAttr -ci true -k true -m -sn "ai_filters" -ln "aiFilters" -at "message";
	addAttr -ci true -uac -sn "ai_shadow_color" -ln "aiShadowColor" -at "float3" -nc 
		3;
	addAttr -ci true -sn "ai_shadow_colorr" -ln "aiShadowColorR" -at "float" -p "aiShadowColor";
	addAttr -ci true -sn "ai_shadow_colorg" -ln "aiShadowColorG" -at "float" -p "aiShadowColor";
	addAttr -ci true -sn "ai_shadow_colorb" -ln "aiShadowColorB" -at "float" -p "aiShadowColor";
	addAttr -ci true -sn "ai_decay_type" -ln "aiDecayType" -dv 1 -min 0 -max 1 -en "constant:quadratic" 
		-at "enum";
	addAttr -ci true -sn "ai_affect_volumetrics" -ln "aiAffectVolumetrics" -dv 1 -min 
		0 -max 1 -at "bool";
	addAttr -ci true -sn "ai_cast_volumetric_shadows" -ln "aiCastVolumetricShadows" 
		-dv 1 -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "ai_resolution" -ln "aiResolution" -dv 512 -at "long";
	addAttr -ci true -k true -sn "ai_translator" -ln "aiTranslator" -dt "string";
	setAttr -k off ".v";
	setAttr ".intensity" 3;
	setAttr ".input_mesh" -type "mesh" 


		"v"	5
		2.7892353e-08	-0.10635045	-0.2127009
		-0.2127009	-0.10635045	-1.8594903e-08
		-9.2974517e-09	-0.10635045	0.2127009
		0.2127009	-0.10635045	0
		0	0.10635045	0

		"vt"	10
		0.50000006	0
		0.25	0.24999999
		0.5	0.5
		0.75	0.25
		0.25	0.5
		0.375	0.5
		0.5	0.5
		0.625	0.5
		0.75	0.5
		0.5	1

		"e"	8
		0	1	"hard"
		1	2	"hard"
		2	3	"hard"
		3	0	"hard"
		0	4	"hard"
		1	4	"hard"
		2	4	"hard"
		3	4	"hard"

		"face"	
		"l"	4	-4	-3	-2	-1	
		"lt"	4	0	3	2	1	

		"face"	
		"l"	3	0	5	-5	
		"lt"	3	4	5	9	

		"face"	
		"l"	3	1	6	-6	
		"lt"	3	5	6	9	

		"face"	
		"l"	3	2	7	-7	
		"lt"	3	6	7	9	

		"face"	
		"l"	3	3	4	-8	
		"lt"	3	7	8	9	;
	setAttr ".ai_decay_type" 0;
	setAttr -k on ".ai_translator" -type "string" "mesh";
createNode lightLinker -s -n "lightLinker1";
	addAttr -ci true -sn "lumaID" -ln "lumaID" -dt "string";
	setAttr -s 5 ".lnk";
	setAttr -s 6 ".ign";
	setAttr -s 5 ".slnk";
	setAttr ".lumaID" -type "string" "a:1338571498.23:0.072050097977";
createNode displayLayerManager -n "layerManager";
	addAttr -ci true -sn "lumaID" -ln "lumaID" -dt "string";
	setAttr ".lumaID" -type "string" "a:1338571498.23:0.475844066019";
createNode displayLayer -n "defaultLayer";
	addAttr -ci true -sn "lumaID" -ln "lumaID" -dt "string";
	setAttr ".lumaID" -type "string" "a:1338571498.23:0.720118323218";
createNode renderLayerManager -n "renderLayerManager";
	addAttr -ci true -sn "lumaID" -ln "lumaID" -dt "string";
	setAttr ".lumaID" -type "string" "a:1338571498.23:0.253386281794";
createNode renderLayer -n "defaultRenderLayer";
	addAttr -ci true -sn "lumaID" -ln "lumaID" -dt "string";
	setAttr ".g" yes;
	setAttr ".lumaID" -type "string" "a:1338571498.23:0.123169092147";
createNode aiOptions -s -n "defaultArnoldRenderOptions";
	addAttr -ci true -sn "lumaID" -ln "lumaID" -dt "string";
	setAttr ".slnk" 2;
	setAttr ".lumaID" -type "string" "a:1338571498.24:0.365850055938";
createNode aiAOVFilter -s -n "defaultArnoldFilter";
	addAttr -ci true -sn "width" -ln "width" -dv 2 -at "float";
	addAttr -ci true -sn "domain" -ln "domain" -min 0 -max 1 -en "first_hit:all_hits" 
		-at "enum";
	addAttr -ci true -sn "minimum" -ln "minimum" -at "float";
	addAttr -ci true -sn "maximum" -ln "maximum" -dv 1 -at "float";
	addAttr -ci true -sn "scalar_mode" -ln "scalarMode" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "lumaID" -ln "lumaID" -dt "string";
	setAttr ".ai_translator" -type "string" "gaussian";
	setAttr ".lumaID" -type "string" "a:1338571498.24:0.00289691703387";
createNode aiAOVDriver -s -n "defaultArnoldDriver";
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
	addAttr -ci true -sn "lumaID" -ln "lumaID" -dt "string";
	setAttr ".ai_translator" -type "string" "tif";
	setAttr ".tiled" no;
	setAttr ".lumaID" -type "string" "a:1338571498.24:0.727093876018";
createNode script -n "uiConfigurationScriptNode";
	addAttr -ci true -sn "lumaID" -ln "lumaID" -dt "string";
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
		+ "\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `outlinerPanel -unParent -l (localizedPanelLabel(\"Outliner\")) -mbv $menusOkayInPanels `;\n\t\t\t$editorName = $panelName;\n            outlinerEditor -e \n                -docTag \"isolOutln_fromSeln\" \n                -showShapes 1\n                -showAttributes 0\n                -showConnected 0\n                -showAnimCurvesOnly 0\n                -showMuteInfo 0\n                -organizeByLayer 1\n                -showAnimLayerWeight 1\n                -autoExpandLayers 1\n                -autoExpand 0\n                -showDagOnly 1\n                -showAssets 1\n                -showContainedOnly 1\n                -showPublishedAsConnected 0\n                -showContainerContents 1\n                -ignoreDagHierarchy 0\n                -expandConnections 0\n                -showUpstreamCurves 1\n                -showUnitlessCurves 1\n                -showCompounds 1\n                -showLeafs 1\n                -showNumericAttrsOnly 0\n                -highlightActive 1\n"
		+ "                -autoSelectNewObjects 0\n                -doNotSelectNewObjects 0\n                -dropIsParent 1\n                -transmitFilters 0\n                -setFilter \"defaultSetFilter\" \n                -showSetMembers 1\n                -allowMultiSelection 1\n                -alwaysToggleSelect 0\n                -directSelect 0\n                -displayMode \"DAG\" \n                -expandObjects 0\n                -setsIgnoreFilters 1\n                -containersIgnoreFilters 0\n                -editAttrName 0\n                -showAttrValues 0\n                -highlightSecondary 0\n                -showUVAttrsOnly 0\n                -showTextureNodesOnly 0\n                -attrAlphaOrder \"default\" \n                -animLayerFilterOptions \"allAffecting\" \n                -sortOrder \"none\" \n                -longNames 0\n                -niceNames 1\n                -showNamespace 1\n                -showPinIcons 0\n                $editorName;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\toutlinerPanel -edit -l (localizedPanelLabel(\"Outliner\")) -mbv $menusOkayInPanels  $panelName;\n"
		+ "\t\t$editorName = $panelName;\n        outlinerEditor -e \n            -docTag \"isolOutln_fromSeln\" \n            -showShapes 1\n            -showAttributes 0\n            -showConnected 0\n            -showAnimCurvesOnly 0\n            -showMuteInfo 0\n            -organizeByLayer 1\n            -showAnimLayerWeight 1\n            -autoExpandLayers 1\n            -autoExpand 0\n            -showDagOnly 1\n            -showAssets 1\n            -showContainedOnly 1\n            -showPublishedAsConnected 0\n            -showContainerContents 1\n            -ignoreDagHierarchy 0\n            -expandConnections 0\n            -showUpstreamCurves 1\n            -showUnitlessCurves 1\n            -showCompounds 1\n            -showLeafs 1\n            -showNumericAttrsOnly 0\n            -highlightActive 1\n            -autoSelectNewObjects 0\n            -doNotSelectNewObjects 0\n            -dropIsParent 1\n            -transmitFilters 0\n            -setFilter \"defaultSetFilter\" \n            -showSetMembers 1\n            -allowMultiSelection 1\n"
		+ "            -alwaysToggleSelect 0\n            -directSelect 0\n            -displayMode \"DAG\" \n            -expandObjects 0\n            -setsIgnoreFilters 1\n            -containersIgnoreFilters 0\n            -editAttrName 0\n            -showAttrValues 0\n            -highlightSecondary 0\n            -showUVAttrsOnly 0\n            -showTextureNodesOnly 0\n            -attrAlphaOrder \"default\" \n            -animLayerFilterOptions \"allAffecting\" \n            -sortOrder \"none\" \n            -longNames 0\n            -niceNames 1\n            -showNamespace 1\n            -showPinIcons 0\n            $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"graphEditor\" (localizedPanelLabel(\"Graph Editor\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"graphEditor\" -l (localizedPanelLabel(\"Graph Editor\")) -mbv $menusOkayInPanels `;\n\n\t\t\t$editorName = ($panelName+\"OutlineEd\");\n            outlinerEditor -e \n"
		+ "                -showShapes 1\n                -showAttributes 1\n                -showConnected 1\n                -showAnimCurvesOnly 1\n                -showMuteInfo 0\n                -organizeByLayer 1\n                -showAnimLayerWeight 1\n                -autoExpandLayers 1\n                -autoExpand 1\n                -showDagOnly 0\n                -showAssets 1\n                -showContainedOnly 0\n                -showPublishedAsConnected 0\n                -showContainerContents 0\n                -ignoreDagHierarchy 0\n                -expandConnections 1\n                -showUpstreamCurves 1\n                -showUnitlessCurves 1\n                -showCompounds 0\n                -showLeafs 1\n                -showNumericAttrsOnly 1\n                -highlightActive 0\n                -autoSelectNewObjects 1\n                -doNotSelectNewObjects 0\n                -dropIsParent 1\n                -transmitFilters 1\n                -setFilter \"0\" \n                -showSetMembers 0\n                -allowMultiSelection 1\n"
		+ "                -alwaysToggleSelect 0\n                -directSelect 0\n                -displayMode \"DAG\" \n                -expandObjects 0\n                -setsIgnoreFilters 1\n                -containersIgnoreFilters 0\n                -editAttrName 0\n                -showAttrValues 0\n                -highlightSecondary 0\n                -showUVAttrsOnly 0\n                -showTextureNodesOnly 0\n                -attrAlphaOrder \"default\" \n                -animLayerFilterOptions \"allAffecting\" \n                -sortOrder \"none\" \n                -longNames 0\n                -niceNames 1\n                -showNamespace 1\n                -showPinIcons 1\n                $editorName;\n\n\t\t\t$editorName = ($panelName+\"GraphEd\");\n            animCurveEditor -e \n                -displayKeys 1\n                -displayTangents 0\n                -displayActiveKeys 0\n                -displayActiveKeyTangents 1\n                -displayInfinities 0\n                -autoFit 0\n                -snapTime \"integer\" \n                -snapValue \"none\" \n"
		+ "                -showResults \"off\" \n                -showBufferCurves \"off\" \n                -smoothness \"fine\" \n                -resultSamples 1\n                -resultScreenSamples 0\n                -resultUpdate \"delayed\" \n                -showUpstreamCurves 1\n                -stackedCurves 0\n                -stackedCurvesMin -1\n                -stackedCurvesMax 1\n                -stackedCurvesSpace 0.2\n                -displayNormalized 0\n                -preSelectionHighlight 0\n                -constrainDrag 0\n                $editorName;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Graph Editor\")) -mbv $menusOkayInPanels  $panelName;\n\n\t\t\t$editorName = ($panelName+\"OutlineEd\");\n            outlinerEditor -e \n                -showShapes 1\n                -showAttributes 1\n                -showConnected 1\n                -showAnimCurvesOnly 1\n                -showMuteInfo 0\n                -organizeByLayer 1\n                -showAnimLayerWeight 1\n                -autoExpandLayers 1\n"
		+ "                -autoExpand 1\n                -showDagOnly 0\n                -showAssets 1\n                -showContainedOnly 0\n                -showPublishedAsConnected 0\n                -showContainerContents 0\n                -ignoreDagHierarchy 0\n                -expandConnections 1\n                -showUpstreamCurves 1\n                -showUnitlessCurves 1\n                -showCompounds 0\n                -showLeafs 1\n                -showNumericAttrsOnly 1\n                -highlightActive 0\n                -autoSelectNewObjects 1\n                -doNotSelectNewObjects 0\n                -dropIsParent 1\n                -transmitFilters 1\n                -setFilter \"0\" \n                -showSetMembers 0\n                -allowMultiSelection 1\n                -alwaysToggleSelect 0\n                -directSelect 0\n                -displayMode \"DAG\" \n                -expandObjects 0\n                -setsIgnoreFilters 1\n                -containersIgnoreFilters 0\n                -editAttrName 0\n                -showAttrValues 0\n"
		+ "                -highlightSecondary 0\n                -showUVAttrsOnly 0\n                -showTextureNodesOnly 0\n                -attrAlphaOrder \"default\" \n                -animLayerFilterOptions \"allAffecting\" \n                -sortOrder \"none\" \n                -longNames 0\n                -niceNames 1\n                -showNamespace 1\n                -showPinIcons 1\n                $editorName;\n\n\t\t\t$editorName = ($panelName+\"GraphEd\");\n            animCurveEditor -e \n                -displayKeys 1\n                -displayTangents 0\n                -displayActiveKeys 0\n                -displayActiveKeyTangents 1\n                -displayInfinities 0\n                -autoFit 0\n                -snapTime \"integer\" \n                -snapValue \"none\" \n                -showResults \"off\" \n                -showBufferCurves \"off\" \n                -smoothness \"fine\" \n                -resultSamples 1\n                -resultScreenSamples 0\n                -resultUpdate \"delayed\" \n                -showUpstreamCurves 1\n                -stackedCurves 0\n"
		+ "                -stackedCurvesMin -1\n                -stackedCurvesMax 1\n                -stackedCurvesSpace 0.2\n                -displayNormalized 0\n                -preSelectionHighlight 0\n                -constrainDrag 0\n                $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"dopeSheetPanel\" (localizedPanelLabel(\"Dope Sheet\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"dopeSheetPanel\" -l (localizedPanelLabel(\"Dope Sheet\")) -mbv $menusOkayInPanels `;\n\n\t\t\t$editorName = ($panelName+\"OutlineEd\");\n            outlinerEditor -e \n                -showShapes 1\n                -showAttributes 1\n                -showConnected 1\n                -showAnimCurvesOnly 1\n                -showMuteInfo 0\n                -organizeByLayer 1\n                -showAnimLayerWeight 1\n                -autoExpandLayers 1\n                -autoExpand 0\n                -showDagOnly 0\n"
		+ "                -showAssets 1\n                -showContainedOnly 0\n                -showPublishedAsConnected 0\n                -showContainerContents 0\n                -ignoreDagHierarchy 0\n                -expandConnections 1\n                -showUpstreamCurves 1\n                -showUnitlessCurves 0\n                -showCompounds 1\n                -showLeafs 1\n                -showNumericAttrsOnly 1\n                -highlightActive 0\n                -autoSelectNewObjects 0\n                -doNotSelectNewObjects 1\n                -dropIsParent 1\n                -transmitFilters 0\n                -setFilter \"0\" \n                -showSetMembers 0\n                -allowMultiSelection 1\n                -alwaysToggleSelect 0\n                -directSelect 0\n                -displayMode \"DAG\" \n                -expandObjects 0\n                -setsIgnoreFilters 1\n                -containersIgnoreFilters 0\n                -editAttrName 0\n                -showAttrValues 0\n                -highlightSecondary 0\n                -showUVAttrsOnly 0\n"
		+ "                -showTextureNodesOnly 0\n                -attrAlphaOrder \"default\" \n                -animLayerFilterOptions \"allAffecting\" \n                -sortOrder \"none\" \n                -longNames 0\n                -niceNames 1\n                -showNamespace 1\n                -showPinIcons 0\n                $editorName;\n\n\t\t\t$editorName = ($panelName+\"DopeSheetEd\");\n            dopeSheetEditor -e \n                -displayKeys 1\n                -displayTangents 0\n                -displayActiveKeys 0\n                -displayActiveKeyTangents 0\n                -displayInfinities 0\n                -autoFit 0\n                -snapTime \"integer\" \n                -snapValue \"none\" \n                -outliner \"dopeSheetPanel1OutlineEd\" \n                -showSummary 1\n                -showScene 0\n                -hierarchyBelow 0\n                -showTicks 1\n                -selectionWindow 0 0 0 0 \n                $editorName;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Dope Sheet\")) -mbv $menusOkayInPanels  $panelName;\n"
		+ "\t\t\t$editorName = ($panelName+\"OutlineEd\");\n            outlinerEditor -e \n                -showShapes 1\n                -showAttributes 1\n                -showConnected 1\n                -showAnimCurvesOnly 1\n                -showMuteInfo 0\n                -organizeByLayer 1\n                -showAnimLayerWeight 1\n                -autoExpandLayers 1\n                -autoExpand 0\n                -showDagOnly 0\n                -showAssets 1\n                -showContainedOnly 0\n                -showPublishedAsConnected 0\n                -showContainerContents 0\n                -ignoreDagHierarchy 0\n                -expandConnections 1\n                -showUpstreamCurves 1\n                -showUnitlessCurves 0\n                -showCompounds 1\n                -showLeafs 1\n                -showNumericAttrsOnly 1\n                -highlightActive 0\n                -autoSelectNewObjects 0\n                -doNotSelectNewObjects 1\n                -dropIsParent 1\n                -transmitFilters 0\n                -setFilter \"0\" \n"
		+ "                -showSetMembers 0\n                -allowMultiSelection 1\n                -alwaysToggleSelect 0\n                -directSelect 0\n                -displayMode \"DAG\" \n                -expandObjects 0\n                -setsIgnoreFilters 1\n                -containersIgnoreFilters 0\n                -editAttrName 0\n                -showAttrValues 0\n                -highlightSecondary 0\n                -showUVAttrsOnly 0\n                -showTextureNodesOnly 0\n                -attrAlphaOrder \"default\" \n                -animLayerFilterOptions \"allAffecting\" \n                -sortOrder \"none\" \n                -longNames 0\n                -niceNames 1\n                -showNamespace 1\n                -showPinIcons 0\n                $editorName;\n\n\t\t\t$editorName = ($panelName+\"DopeSheetEd\");\n            dopeSheetEditor -e \n                -displayKeys 1\n                -displayTangents 0\n                -displayActiveKeys 0\n                -displayActiveKeyTangents 0\n                -displayInfinities 0\n                -autoFit 0\n"
		+ "                -snapTime \"integer\" \n                -snapValue \"none\" \n                -outliner \"dopeSheetPanel1OutlineEd\" \n                -showSummary 1\n                -showScene 0\n                -hierarchyBelow 0\n                -showTicks 1\n                -selectionWindow 0 0 0 0 \n                $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"clipEditorPanel\" (localizedPanelLabel(\"Trax Editor\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"clipEditorPanel\" -l (localizedPanelLabel(\"Trax Editor\")) -mbv $menusOkayInPanels `;\n\n\t\t\t$editorName = clipEditorNameFromPanel($panelName);\n            clipEditor -e \n                -displayKeys 0\n                -displayTangents 0\n                -displayActiveKeys 0\n                -displayActiveKeyTangents 0\n                -displayInfinities 0\n                -autoFit 0\n                -snapTime \"none\" \n                -snapValue \"none\" \n"
		+ "                -manageSequencer 0 \n                $editorName;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Trax Editor\")) -mbv $menusOkayInPanels  $panelName;\n\n\t\t\t$editorName = clipEditorNameFromPanel($panelName);\n            clipEditor -e \n                -displayKeys 0\n                -displayTangents 0\n                -displayActiveKeys 0\n                -displayActiveKeyTangents 0\n                -displayInfinities 0\n                -autoFit 0\n                -snapTime \"none\" \n                -snapValue \"none\" \n                -manageSequencer 0 \n                $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"sequenceEditorPanel\" (localizedPanelLabel(\"Camera Sequencer\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"sequenceEditorPanel\" -l (localizedPanelLabel(\"Camera Sequencer\")) -mbv $menusOkayInPanels `;\n"
		+ "\t\t\t$editorName = sequenceEditorNameFromPanel($panelName);\n            clipEditor -e \n                -displayKeys 0\n                -displayTangents 0\n                -displayActiveKeys 0\n                -displayActiveKeyTangents 0\n                -displayInfinities 0\n                -autoFit 0\n                -snapTime \"none\" \n                -snapValue \"none\" \n                -manageSequencer 1 \n                $editorName;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Camera Sequencer\")) -mbv $menusOkayInPanels  $panelName;\n\n\t\t\t$editorName = sequenceEditorNameFromPanel($panelName);\n            clipEditor -e \n                -displayKeys 0\n                -displayTangents 0\n                -displayActiveKeys 0\n                -displayActiveKeyTangents 0\n                -displayInfinities 0\n                -autoFit 0\n                -snapTime \"none\" \n                -snapValue \"none\" \n                -manageSequencer 1 \n                $editorName;\n\t\tif (!$useSceneConfig) {\n"
		+ "\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"hyperGraphPanel\" (localizedPanelLabel(\"Hypergraph Hierarchy\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"hyperGraphPanel\" -l (localizedPanelLabel(\"Hypergraph Hierarchy\")) -mbv $menusOkayInPanels `;\n\n\t\t\t$editorName = ($panelName+\"HyperGraphEd\");\n            hyperGraph -e \n                -graphLayoutStyle \"hierarchicalLayout\" \n                -orientation \"horiz\" \n                -mergeConnections 0\n                -zoom 1\n                -animateTransition 0\n                -showRelationships 1\n                -showShapes 0\n                -showDeformers 0\n                -showExpressions 0\n                -showConstraints 0\n                -showUnderworld 0\n                -showInvisible 0\n                -transitionFrames 1\n                -opaqueContainers 0\n                -freeform 0\n                -imagePosition 0 0 \n                -imageScale 1\n"
		+ "                -imageEnabled 0\n                -graphType \"DAG\" \n                -heatMapDisplay 0\n                -updateSelection 1\n                -updateNodeAdded 1\n                -useDrawOverrideColor 0\n                -limitGraphTraversal -1\n                -range -1 -1 \n                -iconSize \"smallIcons\" \n                -showCachedConnections 0\n                $editorName;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Hypergraph Hierarchy\")) -mbv $menusOkayInPanels  $panelName;\n\n\t\t\t$editorName = ($panelName+\"HyperGraphEd\");\n            hyperGraph -e \n                -graphLayoutStyle \"hierarchicalLayout\" \n                -orientation \"horiz\" \n                -mergeConnections 0\n                -zoom 1\n                -animateTransition 0\n                -showRelationships 1\n                -showShapes 0\n                -showDeformers 0\n                -showExpressions 0\n                -showConstraints 0\n                -showUnderworld 0\n                -showInvisible 0\n"
		+ "                -transitionFrames 1\n                -opaqueContainers 0\n                -freeform 0\n                -imagePosition 0 0 \n                -imageScale 1\n                -imageEnabled 0\n                -graphType \"DAG\" \n                -heatMapDisplay 0\n                -updateSelection 1\n                -updateNodeAdded 1\n                -useDrawOverrideColor 0\n                -limitGraphTraversal -1\n                -range -1 -1 \n                -iconSize \"smallIcons\" \n                -showCachedConnections 0\n                $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"hyperShadePanel\" (localizedPanelLabel(\"Hypershade\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"hyperShadePanel\" -l (localizedPanelLabel(\"Hypershade\")) -mbv $menusOkayInPanels `;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Hypershade\")) -mbv $menusOkayInPanels  $panelName;\n"
		+ "\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"visorPanel\" (localizedPanelLabel(\"Visor\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"visorPanel\" -l (localizedPanelLabel(\"Visor\")) -mbv $menusOkayInPanels `;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Visor\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"polyTexturePlacementPanel\" (localizedPanelLabel(\"UV Texture Editor\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"polyTexturePlacementPanel\" -l (localizedPanelLabel(\"UV Texture Editor\")) -mbv $menusOkayInPanels `;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"UV Texture Editor\")) -mbv $menusOkayInPanels  $panelName;\n"
		+ "\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"renderWindowPanel\" (localizedPanelLabel(\"Render View\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"renderWindowPanel\" -l (localizedPanelLabel(\"Render View\")) -mbv $menusOkayInPanels `;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Render View\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\tif ($useSceneConfig) {\n\t\tscriptedPanel -e -to $panelName;\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextPanel \"blendShapePanel\" (localizedPanelLabel(\"Blend Shape\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\tblendShapePanel -unParent -l (localizedPanelLabel(\"Blend Shape\")) -mbv $menusOkayInPanels ;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tblendShapePanel -edit -l (localizedPanelLabel(\"Blend Shape\")) -mbv $menusOkayInPanels  $panelName;\n"
		+ "\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"dynRelEdPanel\" (localizedPanelLabel(\"Dynamic Relationships\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"dynRelEdPanel\" -l (localizedPanelLabel(\"Dynamic Relationships\")) -mbv $menusOkayInPanels `;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Dynamic Relationships\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"relationshipPanel\" (localizedPanelLabel(\"Relationship Editor\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"relationshipPanel\" -l (localizedPanelLabel(\"Relationship Editor\")) -mbv $menusOkayInPanels `;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Relationship Editor\")) -mbv $menusOkayInPanels  $panelName;\n"
		+ "\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"referenceEditorPanel\" (localizedPanelLabel(\"Reference Editor\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"referenceEditorPanel\" -l (localizedPanelLabel(\"Reference Editor\")) -mbv $menusOkayInPanels `;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Reference Editor\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"componentEditorPanel\" (localizedPanelLabel(\"Component Editor\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"componentEditorPanel\" -l (localizedPanelLabel(\"Component Editor\")) -mbv $menusOkayInPanels `;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Component Editor\")) -mbv $menusOkayInPanels  $panelName;\n"
		+ "\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"dynPaintScriptedPanelType\" (localizedPanelLabel(\"Paint Effects\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"dynPaintScriptedPanelType\" -l (localizedPanelLabel(\"Paint Effects\")) -mbv $menusOkayInPanels `;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Paint Effects\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"scriptEditorPanel\" (localizedPanelLabel(\"Script Editor\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"scriptEditorPanel\" -l (localizedPanelLabel(\"Script Editor\")) -mbv $menusOkayInPanels `;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Script Editor\")) -mbv $menusOkayInPanels  $panelName;\n"
		+ "\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"Stereo\" (localizedPanelLabel(\"Stereo\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"Stereo\" -l (localizedPanelLabel(\"Stereo\")) -mbv $menusOkayInPanels `;\nstring $editorName = ($panelName+\"Editor\");\n            stereoCameraView -e \n                -camera \"persp\" \n                -useInteractiveMode 0\n                -displayLights \"default\" \n                -displayAppearance \"wireframe\" \n                -activeOnly 0\n                -ignorePanZoom 0\n                -wireframeOnShaded 0\n                -headsUpDisplay 1\n                -selectionHiliteDisplay 1\n                -useDefaultMaterial 0\n                -bufferMode \"double\" \n                -twoSidedLighting 1\n                -backfaceCulling 0\n                -xray 0\n                -jointXray 0\n                -activeComponentsXray 0\n                -displayTextures 0\n"
		+ "                -smoothWireframe 0\n                -lineWidth 1\n                -textureAnisotropic 0\n                -textureHilight 1\n                -textureSampling 2\n                -textureDisplay \"modulate\" \n                -textureMaxSize 16384\n                -fogging 0\n                -fogSource \"fragment\" \n                -fogMode \"linear\" \n                -fogStart 0\n                -fogEnd 100\n                -fogDensity 0.1\n                -fogColor 0.5 0.5 0.5 1 \n                -maxConstantTransparency 1\n                -colorResolution 4 4 \n                -bumpResolution 4 4 \n                -textureCompression 0\n                -transparencyAlgorithm \"frontAndBackCull\" \n                -transpInShadows 0\n                -cullingOverride \"none\" \n                -lowQualityLighting 0\n                -maximumNumHardwareLights 0\n                -occlusionCulling 0\n                -shadingModel 0\n                -useBaseRenderer 0\n                -useReducedRenderer 0\n                -smallObjectCulling 0\n"
		+ "                -smallObjectThreshold -1 \n                -interactiveDisableShadows 0\n                -interactiveBackFaceCull 0\n                -sortTransparent 1\n                -nurbsCurves 1\n                -nurbsSurfaces 1\n                -polymeshes 1\n                -subdivSurfaces 1\n                -planes 1\n                -lights 1\n                -cameras 1\n                -controlVertices 1\n                -hulls 1\n                -grid 1\n                -joints 1\n                -ikHandles 1\n                -deformers 1\n                -dynamics 1\n                -fluids 1\n                -hairSystems 1\n                -follicles 1\n                -nCloths 1\n                -nParticles 1\n                -nRigids 1\n                -dynamicConstraints 1\n                -locators 1\n                -manipulators 1\n                -dimensions 1\n                -handles 1\n                -pivots 1\n                -textures 1\n                -strokes 1\n                -shadows 0\n                -displayMode \"centerEye\" \n"
		+ "                -viewColor 0 0 0 1 \n                $editorName;\nstereoCameraView -e -viewSelected 0 $editorName;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Stereo\")) -mbv $menusOkayInPanels  $panelName;\nstring $editorName = ($panelName+\"Editor\");\n            stereoCameraView -e \n                -camera \"persp\" \n                -useInteractiveMode 0\n                -displayLights \"default\" \n                -displayAppearance \"wireframe\" \n                -activeOnly 0\n                -ignorePanZoom 0\n                -wireframeOnShaded 0\n                -headsUpDisplay 1\n                -selectionHiliteDisplay 1\n                -useDefaultMaterial 0\n                -bufferMode \"double\" \n                -twoSidedLighting 1\n                -backfaceCulling 0\n                -xray 0\n                -jointXray 0\n                -activeComponentsXray 0\n                -displayTextures 0\n                -smoothWireframe 0\n                -lineWidth 1\n                -textureAnisotropic 0\n"
		+ "                -textureHilight 1\n                -textureSampling 2\n                -textureDisplay \"modulate\" \n                -textureMaxSize 16384\n                -fogging 0\n                -fogSource \"fragment\" \n                -fogMode \"linear\" \n                -fogStart 0\n                -fogEnd 100\n                -fogDensity 0.1\n                -fogColor 0.5 0.5 0.5 1 \n                -maxConstantTransparency 1\n                -colorResolution 4 4 \n                -bumpResolution 4 4 \n                -textureCompression 0\n                -transparencyAlgorithm \"frontAndBackCull\" \n                -transpInShadows 0\n                -cullingOverride \"none\" \n                -lowQualityLighting 0\n                -maximumNumHardwareLights 0\n                -occlusionCulling 0\n                -shadingModel 0\n                -useBaseRenderer 0\n                -useReducedRenderer 0\n                -smallObjectCulling 0\n                -smallObjectThreshold -1 \n                -interactiveDisableShadows 0\n                -interactiveBackFaceCull 0\n"
		+ "                -sortTransparent 1\n                -nurbsCurves 1\n                -nurbsSurfaces 1\n                -polymeshes 1\n                -subdivSurfaces 1\n                -planes 1\n                -lights 1\n                -cameras 1\n                -controlVertices 1\n                -hulls 1\n                -grid 1\n                -joints 1\n                -ikHandles 1\n                -deformers 1\n                -dynamics 1\n                -fluids 1\n                -hairSystems 1\n                -follicles 1\n                -nCloths 1\n                -nParticles 1\n                -nRigids 1\n                -dynamicConstraints 1\n                -locators 1\n                -manipulators 1\n                -dimensions 1\n                -handles 1\n                -pivots 1\n                -textures 1\n                -strokes 1\n                -shadows 0\n                -displayMode \"centerEye\" \n                -viewColor 0 0 0 1 \n                $editorName;\nstereoCameraView -e -viewSelected 0 $editorName;\n"
		+ "\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\tif ($useSceneConfig) {\n        string $configName = `getPanel -cwl (localizedPanelLabel(\"Current Layout\"))`;\n        if (\"\" != $configName) {\n\t\t\tpanelConfiguration -edit -label (localizedPanelLabel(\"Current Layout\")) \n\t\t\t\t-defaultImage \"\"\n\t\t\t\t-image \"\"\n\t\t\t\t-sc false\n\t\t\t\t-configString \"global string $gMainPane; paneLayout -e -cn \\\"single\\\" -ps 1 100 100 $gMainPane;\"\n\t\t\t\t-removeAllPanels\n\t\t\t\t-ap false\n\t\t\t\t\t(localizedPanelLabel(\"Persp View\")) \n\t\t\t\t\t\"modelPanel\"\n"
		+ "\t\t\t\t\t\"$panelName = `modelPanel -unParent -l (localizedPanelLabel(\\\"Persp View\\\")) -mbv $menusOkayInPanels `;\\n$editorName = $panelName;\\nmodelEditor -e \\n    -cam `findStartUpCamera persp` \\n    -useInteractiveMode 0\\n    -displayLights \\\"default\\\" \\n    -displayAppearance \\\"wireframe\\\" \\n    -activeOnly 0\\n    -ignorePanZoom 0\\n    -wireframeOnShaded 0\\n    -headsUpDisplay 1\\n    -selectionHiliteDisplay 1\\n    -useDefaultMaterial 0\\n    -bufferMode \\\"double\\\" \\n    -twoSidedLighting 1\\n    -backfaceCulling 0\\n    -xray 0\\n    -jointXray 0\\n    -activeComponentsXray 0\\n    -displayTextures 0\\n    -smoothWireframe 0\\n    -lineWidth 1\\n    -textureAnisotropic 0\\n    -textureHilight 1\\n    -textureSampling 2\\n    -textureDisplay \\\"modulate\\\" \\n    -textureMaxSize 16384\\n    -fogging 0\\n    -fogSource \\\"fragment\\\" \\n    -fogMode \\\"linear\\\" \\n    -fogStart 0\\n    -fogEnd 100\\n    -fogDensity 0.1\\n    -fogColor 0.5 0.5 0.5 1 \\n    -maxConstantTransparency 1\\n    -rendererName \\\"base_OpenGL_Renderer\\\" \\n    -colorResolution 256 256 \\n    -bumpResolution 512 512 \\n    -textureCompression 0\\n    -transparencyAlgorithm \\\"frontAndBackCull\\\" \\n    -transpInShadows 0\\n    -cullingOverride \\\"none\\\" \\n    -lowQualityLighting 0\\n    -maximumNumHardwareLights 1\\n    -occlusionCulling 0\\n    -shadingModel 0\\n    -useBaseRenderer 0\\n    -useReducedRenderer 0\\n    -smallObjectCulling 0\\n    -smallObjectThreshold -1 \\n    -interactiveDisableShadows 0\\n    -interactiveBackFaceCull 0\\n    -sortTransparent 1\\n    -nurbsCurves 1\\n    -nurbsSurfaces 1\\n    -polymeshes 1\\n    -subdivSurfaces 1\\n    -planes 1\\n    -lights 1\\n    -cameras 1\\n    -controlVertices 1\\n    -hulls 1\\n    -grid 1\\n    -joints 1\\n    -ikHandles 1\\n    -deformers 1\\n    -dynamics 1\\n    -fluids 1\\n    -hairSystems 1\\n    -follicles 1\\n    -nCloths 1\\n    -nParticles 1\\n    -nRigids 1\\n    -dynamicConstraints 1\\n    -locators 1\\n    -manipulators 1\\n    -dimensions 1\\n    -handles 1\\n    -pivots 1\\n    -textures 1\\n    -strokes 1\\n    -shadows 0\\n    $editorName;\\nmodelEditor -e -viewSelected 0 $editorName\"\n"
		+ "\t\t\t\t\t\"modelPanel -edit -l (localizedPanelLabel(\\\"Persp View\\\")) -mbv $menusOkayInPanels  $panelName;\\n$editorName = $panelName;\\nmodelEditor -e \\n    -cam `findStartUpCamera persp` \\n    -useInteractiveMode 0\\n    -displayLights \\\"default\\\" \\n    -displayAppearance \\\"wireframe\\\" \\n    -activeOnly 0\\n    -ignorePanZoom 0\\n    -wireframeOnShaded 0\\n    -headsUpDisplay 1\\n    -selectionHiliteDisplay 1\\n    -useDefaultMaterial 0\\n    -bufferMode \\\"double\\\" \\n    -twoSidedLighting 1\\n    -backfaceCulling 0\\n    -xray 0\\n    -jointXray 0\\n    -activeComponentsXray 0\\n    -displayTextures 0\\n    -smoothWireframe 0\\n    -lineWidth 1\\n    -textureAnisotropic 0\\n    -textureHilight 1\\n    -textureSampling 2\\n    -textureDisplay \\\"modulate\\\" \\n    -textureMaxSize 16384\\n    -fogging 0\\n    -fogSource \\\"fragment\\\" \\n    -fogMode \\\"linear\\\" \\n    -fogStart 0\\n    -fogEnd 100\\n    -fogDensity 0.1\\n    -fogColor 0.5 0.5 0.5 1 \\n    -maxConstantTransparency 1\\n    -rendererName \\\"base_OpenGL_Renderer\\\" \\n    -colorResolution 256 256 \\n    -bumpResolution 512 512 \\n    -textureCompression 0\\n    -transparencyAlgorithm \\\"frontAndBackCull\\\" \\n    -transpInShadows 0\\n    -cullingOverride \\\"none\\\" \\n    -lowQualityLighting 0\\n    -maximumNumHardwareLights 1\\n    -occlusionCulling 0\\n    -shadingModel 0\\n    -useBaseRenderer 0\\n    -useReducedRenderer 0\\n    -smallObjectCulling 0\\n    -smallObjectThreshold -1 \\n    -interactiveDisableShadows 0\\n    -interactiveBackFaceCull 0\\n    -sortTransparent 1\\n    -nurbsCurves 1\\n    -nurbsSurfaces 1\\n    -polymeshes 1\\n    -subdivSurfaces 1\\n    -planes 1\\n    -lights 1\\n    -cameras 1\\n    -controlVertices 1\\n    -hulls 1\\n    -grid 1\\n    -joints 1\\n    -ikHandles 1\\n    -deformers 1\\n    -dynamics 1\\n    -fluids 1\\n    -hairSystems 1\\n    -follicles 1\\n    -nCloths 1\\n    -nParticles 1\\n    -nRigids 1\\n    -dynamicConstraints 1\\n    -locators 1\\n    -manipulators 1\\n    -dimensions 1\\n    -handles 1\\n    -pivots 1\\n    -textures 1\\n    -strokes 1\\n    -shadows 0\\n    $editorName;\\nmodelEditor -e -viewSelected 0 $editorName\"\n"
		+ "\t\t\t\t$configName;\n\n            setNamedPanelLayout (localizedPanelLabel(\"Current Layout\"));\n        }\n\n        panelHistory -e -clear mainPanelHistory;\n        setFocus `paneLayout -q -p1 $gMainPane`;\n        sceneUIReplacement -deleteRemaining;\n        sceneUIReplacement -clear;\n\t}\n\n\ngrid -spacing 5 -size 12 -divisions 5 -displayAxes yes -displayGridLines yes -displayDivisionLines yes -displayPerspectiveLabels no -displayOrthographicLabels no -displayAxesBold yes -perspectiveLabelPosition axis -orthographicLabelPosition edge;\nviewManip -drawCompass 0 -compassAngle 0 -frontParameters \"\" -homeParameters \"\" -selectionLockParameters \"\";\n}\n");
	setAttr ".st" 3;
	setAttr ".lumaID" -type "string" "a:1338571498.24:0.261362618991";
createNode script -n "sceneConfigurationScriptNode";
	addAttr -ci true -sn "lumaID" -ln "lumaID" -dt "string";
	setAttr ".b" -type "string" "playbackOptions -min 1 -max 24 -ast 1 -aet 48 ";
	setAttr ".st" 6;
	setAttr ".lumaID" -type "string" "a:1338571498.24:0.327427577445";
createNode shadingEngine -n "aiStandard1SG";
	addAttr -ci true -m -sn "aovs" -ln "aiCustomAOVs" -at "compound" -nc 2;
	addAttr -ci true -k true -sn "aov_name" -ln "aovName" -dt "string" -p "aiCustomAOVs";
	addAttr -ci true -k true -sn "aov_input" -ln "aovInput" -at "message" -p "aiCustomAOVs";
	addAttr -ci true -sn "lumaID" -ln "lumaID" -dt "string";
	setAttr ".ihi" 0;
	setAttr ".ro" yes;
	setAttr ".lumaID" -type "string" "a:1338571498.24:0.605096344078";
createNode materialInfo -n "materialInfo1";
	addAttr -ci true -sn "lumaID" -ln "lumaID" -dt "string";
	setAttr ".lumaID" -type "string" "a:1338571498.24:0.205191995159";
createNode shadingEngine -n "lambert2SG";
	addAttr -ci true -m -sn "aovs" -ln "aiCustomAOVs" -at "compound" -nc 2;
	addAttr -ci true -k true -sn "aov_name" -ln "aovName" -dt "string" -p "aiCustomAOVs";
	addAttr -ci true -k true -sn "aov_input" -ln "aovInput" -at "message" -p "aiCustomAOVs";
	setAttr ".ihi" 0;
	setAttr ".ro" yes;
createNode materialInfo -n "materialInfo2";
createNode polyPlane -n "polyPlane1";
	setAttr ".w" 11.908113153238778;
	setAttr ".h" 11.908113153238778;
	setAttr ".sw" 1;
	setAttr ".sh" 1;
	setAttr ".cuv" 2;
createNode aiStandard -n "aiStandard2";
createNode shadingEngine -n "aiStandard2SG";
	addAttr -ci true -m -sn "aovs" -ln "aiCustomAOVs" -at "compound" -nc 2;
	addAttr -ci true -k true -sn "aov_name" -ln "aovName" -dt "string" -p "aiCustomAOVs";
	addAttr -ci true -k true -sn "aov_input" -ln "aovInput" -at "message" -p "aiCustomAOVs";
	setAttr ".ihi" 0;
	setAttr ".ro" yes;
createNode materialInfo -n "materialInfo3";
createNode polyCylinder -n "polyCylinder1";
	setAttr ".r" 0.16848355281410488;
	setAttr ".h" 1.125757301269765;
	setAttr ".sc" 1;
	setAttr ".cuv" 3;
createNode polyCube -n "polyCube1";
	setAttr ".w" 0.57520056597661906;
	setAttr ".h" 0.57520056597661906;
	setAttr ".d" 0.57520056597661906;
	setAttr ".cuv" 4;
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
	setAttr -s 5 ".st";
	setAttr -cb on ".an";
	setAttr -cb on ".pt";
select -ne :initialShadingGroup;
	addAttr -ci true -m -sn "aovs" -ln "aiCustomAOVs" -at "compound" -nc 2;
	addAttr -ci true -k true -sn "aov_name" -ln "aovName" -dt "string" -p "aiCustomAOVs";
	addAttr -ci true -k true -sn "aov_input" -ln "aovInput" -at "message" -p "aiCustomAOVs";
	setAttr -k on ".cch";
	setAttr -cb on ".ihi";
	setAttr -av -k on ".nds";
	setAttr -cb on ".bnm";
	setAttr -s 3 ".dsm";
	setAttr -k on ".mwc";
	setAttr -cb on ".an";
	setAttr -cb on ".il";
	setAttr -cb on ".vo";
	setAttr -cb on ".eo";
	setAttr -cb on ".fo";
	setAttr -cb on ".epo";
	setAttr ".ro" yes;
select -ne :initialParticleSE;
	addAttr -ci true -m -sn "aovs" -ln "aiCustomAOVs" -at "compound" -nc 2;
	addAttr -ci true -k true -sn "aov_name" -ln "aovName" -dt "string" -p "aiCustomAOVs";
	addAttr -ci true -k true -sn "aov_input" -ln "aovInput" -at "message" -p "aiCustomAOVs";
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
select -ne :defaultShaderList1;
	setAttr -k on ".cch";
	setAttr -cb on ".ihi";
	setAttr -k on ".nds";
	setAttr -cb on ".bnm";
	setAttr -s 3 ".s";
select -ne :postProcessList1;
	setAttr -k on ".cch";
	setAttr -cb on ".ihi";
	setAttr -k on ".nds";
	setAttr -cb on ".bnm";
	setAttr -s 2 ".p";
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
select -ne :defaultRenderQuality;
	setAttr ".ert" yes;
select -ne :defaultResolution;
	addAttr -ci true -sn "projectResEnabled" -ln "projectResEnabled" -at "double";
	addAttr -ci true -sn "applyOverscan" -ln "applyOverscan" -at "double";
	addAttr -ci true -sn "correctPixelAspect" -ln "correctPixelAspect" -at "double";
	addAttr -ci true -sn "resolutionMultiplier" -ln "resolutionMultiplier" -min 0.01 
		-max 10 -at "double";
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
	setAttr ".applyOverscan" 1;
	setAttr ".correctPixelAspect" 1;
	setAttr ".resolutionMultiplier" 1;
select -ne :defaultLightSet;
	addAttr -ci true -k true -sn "ai_override" -ln "aiOverride" -min 0 -max 1 -at "bool";
	setAttr -k on ".cch";
	setAttr -k on ".ihi";
	setAttr -k on ".nds";
	setAttr -k on ".bnm";
	setAttr -s 2 ".dsm";
	setAttr -k on ".mwc";
	setAttr -k on ".an";
	setAttr -k on ".il";
	setAttr -k on ".vo";
	setAttr -k on ".eo";
	setAttr -k on ".fo";
	setAttr -k on ".epo";
	setAttr ".ro" yes;
select -ne :defaultObjectSet;
	addAttr -ci true -k true -sn "ai_override" -ln "aiOverride" -min 0 -max 1 -at "bool";
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
connectAttr "polyPlane1.out" "pPlaneShape1.i";
connectAttr "polyCylinder1.out" "pCylinderShape1.i";
connectAttr "polyCube1.out" "pCubeShape1.i";
relationship "link" ":lightLinker1" ":initialShadingGroup.message" ":defaultLightSet.message";
relationship "link" ":lightLinker1" ":initialParticleSE.message" ":defaultLightSet.message";
relationship "link" ":lightLinker1" "aiStandard1SG.message" ":defaultLightSet.message";
relationship "link" ":lightLinker1" "lambert2SG.message" ":defaultLightSet.message";
relationship "link" ":lightLinker1" "aiStandard2SG.message" ":defaultLightSet.message";
relationship "shadowLink" ":lightLinker1" ":initialShadingGroup.message" ":defaultLightSet.message";
relationship "shadowLink" ":lightLinker1" ":initialParticleSE.message" ":defaultLightSet.message";
relationship "shadowLink" ":lightLinker1" "aiStandard1SG.message" ":defaultLightSet.message";
relationship "shadowLink" ":lightLinker1" "lambert2SG.message" ":defaultLightSet.message";
relationship "shadowLink" ":lightLinker1" "aiStandard2SG.message" ":defaultLightSet.message";
connectAttr "layerManager.dli[0]" "defaultLayer.id";
connectAttr "renderLayerManager.rlmi[0]" "defaultRenderLayer.rlid";
connectAttr ":defaultArnoldFilter.msg" ":defaultArnoldRenderOptions.filt";
connectAttr ":defaultArnoldDriver.msg" ":defaultArnoldRenderOptions.drvr";
connectAttr "aiStandard1SG.msg" "materialInfo1.sg";
connectAttr "lambert2SG.msg" "materialInfo2.sg";
connectAttr "aiStandard2.out" "aiStandard2SG.ss";
connectAttr "aiStandard2SG.msg" "materialInfo3.sg";
connectAttr "aiStandard2.msg" "materialInfo3.m";
connectAttr "aiStandard2.msg" "materialInfo3.t" -na;
connectAttr "aiStandard1SG.pa" ":renderPartition.st" -na;
connectAttr "lambert2SG.pa" ":renderPartition.st" -na;
connectAttr "aiStandard2SG.pa" ":renderPartition.st" -na;
connectAttr "pPlaneShape1.iog" ":initialShadingGroup.dsm" -na;
connectAttr "pCylinderShape1.iog" ":initialShadingGroup.dsm" -na;
connectAttr "pCubeShape1.iog" ":initialShadingGroup.dsm" -na;
connectAttr "aiStandard2.msg" ":defaultShaderList1.s" -na;
connectAttr "aiAreaLight1.iog" ":defaultLightSet.dsm" -na;
connectAttr "aiAreaLight2.iog" ":defaultLightSet.dsm" -na;
// End of test.ma

//Maya ASCII 2011 scene
//Name: test.ma
//Last modified: Thu, May 03, 2012 05:37:44 PM
//Codeset: 1252
requires maya "2011";
requires "mtoa" "0.16.0.dev";
currentUnit -l centimeter -a degree -t film;
fileInfo "application" "maya";
fileInfo "product" "Maya 2011";
fileInfo "version" "2011 x64";
fileInfo "cutIdentifier" "201009060330-781623";
fileInfo "osv" "Microsoft Windows 7 Business Edition, 64-bit Windows 7 Service Pack 1 (Build 7601)\n";
createNode transform -s -n "persp";
	setAttr ".v" no;
	setAttr ".t" -type "double3" 33.239702804974158 8.5675094119517752 -40.93169838719389 ;
	setAttr ".r" -type "double3" -8.1383527295959368 109.39999999998848 0 ;
createNode camera -s -n "perspShape" -p "persp";
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
	setAttr ".fl" 34.999999999999993;
	setAttr ".coi" 44.821869661955006;
	setAttr ".imn" -type "string" "persp";
	setAttr ".den" -type "string" "persp_depth";
	setAttr ".man" -type "string" "persp_mask";
	setAttr ".hc" -type "string" "viewSet -p %camera";
	setAttr -k on ".ai_translator" -type "string" "spherical";
createNode transform -s -n "top";
	setAttr ".v" no;
	setAttr ".t" -type "double3" 0 100.1 0 ;
	setAttr ".r" -type "double3" -89.999999999999986 0 0 ;
createNode camera -s -n "topShape" -p "top";
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
	setAttr ".coi" 100.1;
	setAttr ".ow" 434.13359162668007;
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
	setAttr ".coi" 100.1;
	setAttr ".ow" 30;
	setAttr ".imn" -type "string" "side";
	setAttr ".den" -type "string" "side_depth";
	setAttr ".man" -type "string" "side_mask";
	setAttr ".hc" -type "string" "viewSet -s %camera";
	setAttr ".o" yes;
	setAttr -k on ".ai_translator" -type "string" "orthographic";
createNode transform -n "pCube1";
	setAttr ".t" -type "double3" -14.220522859000491 3.4222062569788121 -23.061413185612373 ;
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
	addAttr -ci true -sn "mso" -ln "miShadingSamplesOverride" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "msh" -ln "miShadingSamples" -min 0 -smx 8 -at "float";
	addAttr -ci true -sn "mdo" -ln "miMaxDisplaceOverride" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "mmd" -ln "miMaxDisplace" -min 0 -smx 1 -at "float";
	setAttr -k off ".v";
	setAttr ".vir" yes;
	setAttr ".vif" yes;
	setAttr ".uvst[0].uvsn" -type "string" "map1";
	setAttr ".cuvs" -type "string" "map1";
	setAttr ".dcc" -type "string" "Ambient+Diffuse";
	setAttr ".covm[0]"  0 1 1;
	setAttr ".cdvm[0]"  0 1 1;
createNode transform -n "pTorus1";
	setAttr ".t" -type "double3" 18.273008428913116 4.3191544448413755 -28.704543168000114 ;
createNode mesh -n "pTorusShape1" -p "pTorus1";
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
	setAttr ".t" -type "double3" 55.100678733007591 5.7606690800090519 -24.568406418918535 ;
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
createNode transform -n "directionalLight1";
	setAttr ".t" -type "double3" 0 3.5321561850019547 0 ;
	setAttr ".r" -type "double3" -37.928595716690737 32.371643993250771 -22.647544702738745 ;
createNode directionalLight -n "directionalLightShape1" -p "directionalLight1";
	addAttr -ci true -sn "ai_cast_shadows" -ln "aiCastShadows" -dv 1 -min 0 -max 1 
		-at "bool";
	addAttr -ci true -sn "ai_exposure" -ln "aiExposure" -min 0 -smx 10 -at "float";
	addAttr -ci true -sn "ai_samples" -ln "aiSamples" -dv 1 -min 1 -max 100 -at "long";
	addAttr -ci true -sn "ai_normalize" -ln "aiNormalize" -dv 1 -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "ai_bounce_factor" -ln "aiBounceFactor" -dv 1 -min 0 -smx 20 
		-at "float";
	addAttr -ci true -sn "ai_bounces" -ln "aiBounces" -dv 999 -min 0 -max 10000 -at "long";
	addAttr -ci true -k true -m -sn "ai_filters" -ln "aiFilters" -at "message";
	addAttr -ci true -sn "ai_angle" -ln "aiAngle" -min 0 -smx 10 -at "float";
	setAttr -k off ".v";
createNode transform -n "pCube2";
	setAttr ".t" -type "double3" 60.727951150614658 3.4222062569788121 -23.061413185612373 ;
createNode mesh -n "pCubeShape2" -p "pCube2";
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
	addAttr -ci true -sn "mso" -ln "miShadingSamplesOverride" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "msh" -ln "miShadingSamples" -min 0 -smx 8 -at "float";
	addAttr -ci true -sn "mdo" -ln "miMaxDisplaceOverride" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "mmd" -ln "miMaxDisplace" -min 0 -smx 1 -at "float";
	setAttr -k off ".v";
	setAttr ".vir" yes;
	setAttr ".vif" yes;
	setAttr ".uvst[0].uvsn" -type "string" "map1";
	setAttr -s 14 ".uvst[0].uvsp[0:13]" -type "float2" 0.375 0 0.625 0 
		0.375 0.25 0.625 0.25 0.375 0.5 0.625 0.5 0.375 0.75 0.625 0.75 0.375 1 0.625 1 0.875 
		0 0.875 0.25 0.125 0 0.125 0.25;
	setAttr ".cuvs" -type "string" "map1";
	setAttr ".dcc" -type "string" "Ambient+Diffuse";
	setAttr ".covm[0]"  0 1 1;
	setAttr ".cdvm[0]"  0 1 1;
	setAttr -s 8 ".vt[0:7]"  -3.4247265 -3.4222062 37.093811 3.4247265 
		-3.4222062 37.093811 -3.4247265 3.4222062 37.093811 3.4247265 3.4222062 37.093811 
		-3.4247265 3.4222062 -37.093811 3.4247265 3.4222062 -37.093811 -3.4247265 -3.4222062 
		-37.093811 3.4247265 -3.4222062 -37.093811;
	setAttr -s 12 ".ed[0:11]"  0 1 0 2 3 0 
		4 5 0 6 7 0 0 2 0 1 3 0 
		2 4 0 3 5 0 4 6 0 5 7 0 
		6 0 0 7 1 0;
	setAttr -s 6 ".fc[0:5]" -type "polyFaces" 
		f 4 0 5 -2 -5 
		mu 0 4 0 1 3 2 
		f 4 1 7 -3 -7 
		mu 0 4 2 3 5 4 
		f 4 2 9 -4 -9 
		mu 0 4 4 5 7 6 
		f 4 3 11 -1 -11 
		mu 0 4 6 7 9 8 
		f 4 -12 -10 -8 -6 
		mu 0 4 1 10 11 3 
		f 4 10 4 6 8 
		mu 0 4 12 0 2 13 ;
	setAttr ".cd" -type "dataPolyComponent" Index_Data Edge 0 ;
	setAttr ".cvd" -type "dataPolyComponent" Index_Data Vertex 0 ;
	setAttr ".hfd" -type "dataPolyComponent" Index_Data Face 0 ;
createNode transform -n "pCylinder2";
	setAttr ".t" -type "double3" -3.0016660756807667 5.7606690800090519 28.274261002204597 ;
createNode mesh -n "pCylinderShape2" -p "pCylinder2";
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
	setAttr -s 84 ".uvst[0].uvsp[0:83]" -type "float2" 0.64860266 0.10796607 
		0.62640899 0.064408496 0.59184152 0.029841021 0.54828393 0.0076473355 0.5 -7.4505806e-008 
		0.45171607 0.0076473504 0.40815851 0.029841051 0.37359107 0.064408526 0.3513974 0.10796608 
		0.34374997 0.15625 0.3513974 0.20453392 0.37359107 0.24809146 0.40815854 0.28265893 
		0.4517161 0.3048526 0.5 0.3125 0.54828387 0.3048526 0.59184146 0.28265893 0.62640893 
		0.24809146 0.6486026 0.2045339 0.65625 0.15625 0.375 0.3125 0.38749999 0.3125 0.39999998 
		0.3125 0.41249996 0.3125 0.42499995 0.3125 0.43749994 0.3125 0.44999993 0.3125 0.46249992 
		0.3125 0.4749999 0.3125 0.48749989 0.3125 0.49999988 0.3125 0.51249987 0.3125 0.52499986 
		0.3125 0.53749985 0.3125 0.54999983 0.3125 0.56249982 0.3125 0.57499981 0.3125 0.5874998 
		0.3125 0.59999979 0.3125 0.61249977 0.3125 0.62499976 0.3125 0.375 0.68843985 0.38749999 
		0.68843985 0.39999998 0.68843985 0.41249996 0.68843985 0.42499995 0.68843985 0.43749994 
		0.68843985 0.44999993 0.68843985 0.46249992 0.68843985 0.4749999 0.68843985 0.48749989 
		0.68843985 0.49999988 0.68843985 0.51249987 0.68843985 0.52499986 0.68843985 0.53749985 
		0.68843985 0.54999983 0.68843985 0.56249982 0.68843985 0.57499981 0.68843985 0.5874998 
		0.68843985 0.59999979 0.68843985 0.61249977 0.68843985 0.62499976 0.68843985 0.64860266 
		0.79546607 0.62640899 0.75190848 0.59184152 0.71734101 0.54828393 0.69514734 0.5 
		0.68749994 0.45171607 0.69514734 0.40815851 0.71734107 0.37359107 0.75190854 0.3513974 
		0.79546607 0.34374997 0.84375 0.3513974 0.89203393 0.37359107 0.93559146 0.40815854 
		0.97015893 0.4517161 0.9923526 0.5 1 0.54828387 0.9923526 0.59184146 0.97015893 0.62640893 
		0.93559146 0.6486026 0.89203393 0.65625 0.84375 0.5 0.15000001 0.5 0.83749998;
	setAttr ".cuvs" -type "string" "map1";
	setAttr ".dcc" -type "string" "Ambient+Diffuse";
	setAttr ".covm[0]"  0 1 1;
	setAttr ".cdvm[0]"  0 1 1;
	setAttr -s 42 ".vt[0:41]"  3.9444733 -5.7606692 -1.281637 3.3553693 
		-5.7606692 -2.4378183 2.4378183 -5.7606692 -3.3553691 1.2816368 -5.7606692 -3.9444728 
		0 -5.7606692 -4.1474638 -1.2816368 -5.7606692 -3.9444726 -2.4378178 -5.7606692 -3.3553684 
		-3.3553681 -5.7606692 -2.4378176 -3.9444718 -5.7606692 -1.2816365 -4.1474628 -5.7606692 
		0 -3.9444718 -5.7606692 1.2816365 -3.3553679 -5.7606692 2.4378173 -2.4378173 -5.7606692 
		3.3553677 -1.2816365 -5.7606692 3.9444714 -1.23604e-007 -5.7606692 4.1474624 1.2816361 
		-5.7606692 3.9444711 2.4378169 -5.7606692 3.3553674 3.3553672 -5.7606692 2.4378171 
		3.9444706 -5.7606692 1.2816362 4.1474619 -5.7606692 0 3.9444733 5.7606692 -1.281637 
		3.3553693 5.7606692 -2.4378183 2.4378183 5.7606692 -3.3553691 1.2816368 5.7606692 
		-3.9444728 0 5.7606692 -4.1474638 -1.2816368 5.7606692 -3.9444726 -2.4378178 5.7606692 
		-3.3553684 -3.3553681 5.7606692 -2.4378176 -3.9444718 5.7606692 -1.2816365 -4.1474628 
		5.7606692 0 -3.9444718 5.7606692 1.2816365 -3.3553679 5.7606692 2.4378173 -2.4378173 
		5.7606692 3.3553677 -1.2816365 5.7606692 3.9444714 -1.23604e-007 5.7606692 4.1474624 
		1.2816361 5.7606692 3.9444711 2.4378169 5.7606692 3.3553674 3.3553672 5.7606692 2.4378171 
		3.9444706 5.7606692 1.2816362 4.1474619 5.7606692 0 0 -5.7606692 0 0 5.7606692 0;
	setAttr -s 100 ".ed[0:99]"  0 1 0 1 2 0 
		2 3 0 3 4 0 4 5 0 5 6 0 
		6 7 0 7 8 0 8 9 0 9 10 0 
		10 11 0 11 12 0 12 13 0 13 14 0 
		14 15 0 15 16 0 16 17 0 17 18 0 
		18 19 0 19 0 0 20 21 0 21 22 0 
		22 23 0 23 24 0 24 25 0 25 26 0 
		26 27 0 27 28 0 28 29 0 29 30 0 
		30 31 0 31 32 0 32 33 0 33 34 0 
		34 35 0 35 36 0 36 37 0 37 38 0 
		38 39 0 39 20 0 0 20 1 1 21 1 
		2 22 1 3 23 1 4 24 1 5 25 1 
		6 26 1 7 27 1 8 28 1 9 29 1 
		10 30 1 11 31 1 12 32 1 13 33 1 
		14 34 1 15 35 1 16 36 1 17 37 1 
		18 38 1 19 39 1 40 0 1 40 1 1 
		40 2 1 40 3 1 40 4 1 40 5 1 
		40 6 1 40 7 1 40 8 1 40 9 1 
		40 10 1 40 11 1 40 12 1 40 13 1 
		40 14 1 40 15 1 40 16 1 40 17 1 
		40 18 1 40 19 1 20 41 1 21 41 1 
		22 41 1 23 41 1 24 41 1 25 41 1 
		26 41 1 27 41 1 28 41 1 29 41 1 
		30 41 1 31 41 1 32 41 1 33 41 1 
		34 41 1 35 41 1 36 41 1 37 41 1 
		38 41 1 39 41 1;
	setAttr -s 60 ".fc[0:59]" -type "polyFaces" 
		f 4 0 41 -21 -41 
		mu 0 4 20 21 42 41 
		f 4 1 42 -22 -42 
		mu 0 4 21 22 43 42 
		f 4 2 43 -23 -43 
		mu 0 4 22 23 44 43 
		f 4 3 44 -24 -44 
		mu 0 4 23 24 45 44 
		f 4 4 45 -25 -45 
		mu 0 4 24 25 46 45 
		f 4 5 46 -26 -46 
		mu 0 4 25 26 47 46 
		f 4 6 47 -27 -47 
		mu 0 4 26 27 48 47 
		f 4 7 48 -28 -48 
		mu 0 4 27 28 49 48 
		f 4 8 49 -29 -49 
		mu 0 4 28 29 50 49 
		f 4 9 50 -30 -50 
		mu 0 4 29 30 51 50 
		f 4 10 51 -31 -51 
		mu 0 4 30 31 52 51 
		f 4 11 52 -32 -52 
		mu 0 4 31 32 53 52 
		f 4 12 53 -33 -53 
		mu 0 4 32 33 54 53 
		f 4 13 54 -34 -54 
		mu 0 4 33 34 55 54 
		f 4 14 55 -35 -55 
		mu 0 4 34 35 56 55 
		f 4 15 56 -36 -56 
		mu 0 4 35 36 57 56 
		f 4 16 57 -37 -57 
		mu 0 4 36 37 58 57 
		f 4 17 58 -38 -58 
		mu 0 4 37 38 59 58 
		f 4 18 59 -39 -59 
		mu 0 4 38 39 60 59 
		f 4 19 40 -40 -60 
		mu 0 4 39 40 61 60 
		f 3 -1 -61 61 
		mu 0 3 1 0 82 
		f 3 -2 -62 62 
		mu 0 3 2 1 82 
		f 3 -3 -63 63 
		mu 0 3 3 2 82 
		f 3 -4 -64 64 
		mu 0 3 4 3 82 
		f 3 -5 -65 65 
		mu 0 3 5 4 82 
		f 3 -6 -66 66 
		mu 0 3 6 5 82 
		f 3 -7 -67 67 
		mu 0 3 7 6 82 
		f 3 -8 -68 68 
		mu 0 3 8 7 82 
		f 3 -9 -69 69 
		mu 0 3 9 8 82 
		f 3 -10 -70 70 
		mu 0 3 10 9 82 
		f 3 -11 -71 71 
		mu 0 3 11 10 82 
		f 3 -12 -72 72 
		mu 0 3 12 11 82 
		f 3 -13 -73 73 
		mu 0 3 13 12 82 
		f 3 -14 -74 74 
		mu 0 3 14 13 82 
		f 3 -15 -75 75 
		mu 0 3 15 14 82 
		f 3 -16 -76 76 
		mu 0 3 16 15 82 
		f 3 -17 -77 77 
		mu 0 3 17 16 82 
		f 3 -18 -78 78 
		mu 0 3 18 17 82 
		f 3 -19 -79 79 
		mu 0 3 19 18 82 
		f 3 -20 -80 60 
		mu 0 3 0 19 82 
		f 3 20 81 -81 
		mu 0 3 80 79 83 
		f 3 21 82 -82 
		mu 0 3 79 78 83 
		f 3 22 83 -83 
		mu 0 3 78 77 83 
		f 3 23 84 -84 
		mu 0 3 77 76 83 
		f 3 24 85 -85 
		mu 0 3 76 75 83 
		f 3 25 86 -86 
		mu 0 3 75 74 83 
		f 3 26 87 -87 
		mu 0 3 74 73 83 
		f 3 27 88 -88 
		mu 0 3 73 72 83 
		f 3 28 89 -89 
		mu 0 3 72 71 83 
		f 3 29 90 -90 
		mu 0 3 71 70 83 
		f 3 30 91 -91 
		mu 0 3 70 69 83 
		f 3 31 92 -92 
		mu 0 3 69 68 83 
		f 3 32 93 -93 
		mu 0 3 68 67 83 
		f 3 33 94 -94 
		mu 0 3 67 66 83 
		f 3 34 95 -95 
		mu 0 3 66 65 83 
		f 3 35 96 -96 
		mu 0 3 65 64 83 
		f 3 36 97 -97 
		mu 0 3 64 63 83 
		f 3 37 98 -98 
		mu 0 3 63 62 83 
		f 3 38 99 -99 
		mu 0 3 62 81 83 
		f 3 39 80 -100 
		mu 0 3 81 80 83 ;
	setAttr ".cd" -type "dataPolyComponent" Index_Data Edge 0 ;
	setAttr ".cvd" -type "dataPolyComponent" Index_Data Vertex 0 ;
	setAttr ".hfd" -type "dataPolyComponent" Index_Data Face 0 ;
createNode transform -n "pCylinder3";
	setAttr ".t" -type "double3" -3.0016660756807667 5.7606690800090519 -81.271545196355518 ;
createNode mesh -n "pCylinderShape3" -p "pCylinder3";
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
	setAttr -s 84 ".uvst[0].uvsp[0:83]" -type "float2" 0.64860266 0.10796607 
		0.62640899 0.064408496 0.59184152 0.029841021 0.54828393 0.0076473355 0.5 -7.4505806e-008 
		0.45171607 0.0076473504 0.40815851 0.029841051 0.37359107 0.064408526 0.3513974 0.10796608 
		0.34374997 0.15625 0.3513974 0.20453392 0.37359107 0.24809146 0.40815854 0.28265893 
		0.4517161 0.3048526 0.5 0.3125 0.54828387 0.3048526 0.59184146 0.28265893 0.62640893 
		0.24809146 0.6486026 0.2045339 0.65625 0.15625 0.375 0.3125 0.38749999 0.3125 0.39999998 
		0.3125 0.41249996 0.3125 0.42499995 0.3125 0.43749994 0.3125 0.44999993 0.3125 0.46249992 
		0.3125 0.4749999 0.3125 0.48749989 0.3125 0.49999988 0.3125 0.51249987 0.3125 0.52499986 
		0.3125 0.53749985 0.3125 0.54999983 0.3125 0.56249982 0.3125 0.57499981 0.3125 0.5874998 
		0.3125 0.59999979 0.3125 0.61249977 0.3125 0.62499976 0.3125 0.375 0.68843985 0.38749999 
		0.68843985 0.39999998 0.68843985 0.41249996 0.68843985 0.42499995 0.68843985 0.43749994 
		0.68843985 0.44999993 0.68843985 0.46249992 0.68843985 0.4749999 0.68843985 0.48749989 
		0.68843985 0.49999988 0.68843985 0.51249987 0.68843985 0.52499986 0.68843985 0.53749985 
		0.68843985 0.54999983 0.68843985 0.56249982 0.68843985 0.57499981 0.68843985 0.5874998 
		0.68843985 0.59999979 0.68843985 0.61249977 0.68843985 0.62499976 0.68843985 0.64860266 
		0.79546607 0.62640899 0.75190848 0.59184152 0.71734101 0.54828393 0.69514734 0.5 
		0.68749994 0.45171607 0.69514734 0.40815851 0.71734107 0.37359107 0.75190854 0.3513974 
		0.79546607 0.34374997 0.84375 0.3513974 0.89203393 0.37359107 0.93559146 0.40815854 
		0.97015893 0.4517161 0.9923526 0.5 1 0.54828387 0.9923526 0.59184146 0.97015893 0.62640893 
		0.93559146 0.6486026 0.89203393 0.65625 0.84375 0.5 0.15000001 0.5 0.83749998;
	setAttr ".cuvs" -type "string" "map1";
	setAttr ".dcc" -type "string" "Ambient+Diffuse";
	setAttr ".covm[0]"  0 1 1;
	setAttr ".cdvm[0]"  0 1 1;
	setAttr -s 42 ".vt[0:41]"  3.9444733 -5.7606692 -1.281637 3.3553693 
		-5.7606692 -2.4378183 2.4378183 -5.7606692 -3.3553691 1.2816368 -5.7606692 -3.9444728 
		0 -5.7606692 -4.1474638 -1.2816368 -5.7606692 -3.9444726 -2.4378178 -5.7606692 -3.3553684 
		-3.3553681 -5.7606692 -2.4378176 -3.9444718 -5.7606692 -1.2816365 -4.1474628 -5.7606692 
		0 -3.9444718 -5.7606692 1.2816365 -3.3553679 -5.7606692 2.4378173 -2.4378173 -5.7606692 
		3.3553677 -1.2816365 -5.7606692 3.9444714 -1.23604e-007 -5.7606692 4.1474624 1.2816361 
		-5.7606692 3.9444711 2.4378169 -5.7606692 3.3553674 3.3553672 -5.7606692 2.4378171 
		3.9444706 -5.7606692 1.2816362 4.1474619 -5.7606692 0 3.9444733 5.7606692 -1.281637 
		3.3553693 5.7606692 -2.4378183 2.4378183 5.7606692 -3.3553691 1.2816368 5.7606692 
		-3.9444728 0 5.7606692 -4.1474638 -1.2816368 5.7606692 -3.9444726 -2.4378178 5.7606692 
		-3.3553684 -3.3553681 5.7606692 -2.4378176 -3.9444718 5.7606692 -1.2816365 -4.1474628 
		5.7606692 0 -3.9444718 5.7606692 1.2816365 -3.3553679 5.7606692 2.4378173 -2.4378173 
		5.7606692 3.3553677 -1.2816365 5.7606692 3.9444714 -1.23604e-007 5.7606692 4.1474624 
		1.2816361 5.7606692 3.9444711 2.4378169 5.7606692 3.3553674 3.3553672 5.7606692 2.4378171 
		3.9444706 5.7606692 1.2816362 4.1474619 5.7606692 0 0 -5.7606692 0 0 5.7606692 0;
	setAttr -s 100 ".ed[0:99]"  0 1 0 1 2 0 
		2 3 0 3 4 0 4 5 0 5 6 0 
		6 7 0 7 8 0 8 9 0 9 10 0 
		10 11 0 11 12 0 12 13 0 13 14 0 
		14 15 0 15 16 0 16 17 0 17 18 0 
		18 19 0 19 0 0 20 21 0 21 22 0 
		22 23 0 23 24 0 24 25 0 25 26 0 
		26 27 0 27 28 0 28 29 0 29 30 0 
		30 31 0 31 32 0 32 33 0 33 34 0 
		34 35 0 35 36 0 36 37 0 37 38 0 
		38 39 0 39 20 0 0 20 1 1 21 1 
		2 22 1 3 23 1 4 24 1 5 25 1 
		6 26 1 7 27 1 8 28 1 9 29 1 
		10 30 1 11 31 1 12 32 1 13 33 1 
		14 34 1 15 35 1 16 36 1 17 37 1 
		18 38 1 19 39 1 40 0 1 40 1 1 
		40 2 1 40 3 1 40 4 1 40 5 1 
		40 6 1 40 7 1 40 8 1 40 9 1 
		40 10 1 40 11 1 40 12 1 40 13 1 
		40 14 1 40 15 1 40 16 1 40 17 1 
		40 18 1 40 19 1 20 41 1 21 41 1 
		22 41 1 23 41 1 24 41 1 25 41 1 
		26 41 1 27 41 1 28 41 1 29 41 1 
		30 41 1 31 41 1 32 41 1 33 41 1 
		34 41 1 35 41 1 36 41 1 37 41 1 
		38 41 1 39 41 1;
	setAttr -s 60 ".fc[0:59]" -type "polyFaces" 
		f 4 0 41 -21 -41 
		mu 0 4 20 21 42 41 
		f 4 1 42 -22 -42 
		mu 0 4 21 22 43 42 
		f 4 2 43 -23 -43 
		mu 0 4 22 23 44 43 
		f 4 3 44 -24 -44 
		mu 0 4 23 24 45 44 
		f 4 4 45 -25 -45 
		mu 0 4 24 25 46 45 
		f 4 5 46 -26 -46 
		mu 0 4 25 26 47 46 
		f 4 6 47 -27 -47 
		mu 0 4 26 27 48 47 
		f 4 7 48 -28 -48 
		mu 0 4 27 28 49 48 
		f 4 8 49 -29 -49 
		mu 0 4 28 29 50 49 
		f 4 9 50 -30 -50 
		mu 0 4 29 30 51 50 
		f 4 10 51 -31 -51 
		mu 0 4 30 31 52 51 
		f 4 11 52 -32 -52 
		mu 0 4 31 32 53 52 
		f 4 12 53 -33 -53 
		mu 0 4 32 33 54 53 
		f 4 13 54 -34 -54 
		mu 0 4 33 34 55 54 
		f 4 14 55 -35 -55 
		mu 0 4 34 35 56 55 
		f 4 15 56 -36 -56 
		mu 0 4 35 36 57 56 
		f 4 16 57 -37 -57 
		mu 0 4 36 37 58 57 
		f 4 17 58 -38 -58 
		mu 0 4 37 38 59 58 
		f 4 18 59 -39 -59 
		mu 0 4 38 39 60 59 
		f 4 19 40 -40 -60 
		mu 0 4 39 40 61 60 
		f 3 -1 -61 61 
		mu 0 3 1 0 82 
		f 3 -2 -62 62 
		mu 0 3 2 1 82 
		f 3 -3 -63 63 
		mu 0 3 3 2 82 
		f 3 -4 -64 64 
		mu 0 3 4 3 82 
		f 3 -5 -65 65 
		mu 0 3 5 4 82 
		f 3 -6 -66 66 
		mu 0 3 6 5 82 
		f 3 -7 -67 67 
		mu 0 3 7 6 82 
		f 3 -8 -68 68 
		mu 0 3 8 7 82 
		f 3 -9 -69 69 
		mu 0 3 9 8 82 
		f 3 -10 -70 70 
		mu 0 3 10 9 82 
		f 3 -11 -71 71 
		mu 0 3 11 10 82 
		f 3 -12 -72 72 
		mu 0 3 12 11 82 
		f 3 -13 -73 73 
		mu 0 3 13 12 82 
		f 3 -14 -74 74 
		mu 0 3 14 13 82 
		f 3 -15 -75 75 
		mu 0 3 15 14 82 
		f 3 -16 -76 76 
		mu 0 3 16 15 82 
		f 3 -17 -77 77 
		mu 0 3 17 16 82 
		f 3 -18 -78 78 
		mu 0 3 18 17 82 
		f 3 -19 -79 79 
		mu 0 3 19 18 82 
		f 3 -20 -80 60 
		mu 0 3 0 19 82 
		f 3 20 81 -81 
		mu 0 3 80 79 83 
		f 3 21 82 -82 
		mu 0 3 79 78 83 
		f 3 22 83 -83 
		mu 0 3 78 77 83 
		f 3 23 84 -84 
		mu 0 3 77 76 83 
		f 3 24 85 -85 
		mu 0 3 76 75 83 
		f 3 25 86 -86 
		mu 0 3 75 74 83 
		f 3 26 87 -87 
		mu 0 3 74 73 83 
		f 3 27 88 -88 
		mu 0 3 73 72 83 
		f 3 28 89 -89 
		mu 0 3 72 71 83 
		f 3 29 90 -90 
		mu 0 3 71 70 83 
		f 3 30 91 -91 
		mu 0 3 70 69 83 
		f 3 31 92 -92 
		mu 0 3 69 68 83 
		f 3 32 93 -93 
		mu 0 3 68 67 83 
		f 3 33 94 -94 
		mu 0 3 67 66 83 
		f 3 34 95 -95 
		mu 0 3 66 65 83 
		f 3 35 96 -96 
		mu 0 3 65 64 83 
		f 3 36 97 -97 
		mu 0 3 64 63 83 
		f 3 37 98 -98 
		mu 0 3 63 62 83 
		f 3 38 99 -99 
		mu 0 3 62 81 83 
		f 3 39 80 -100 
		mu 0 3 81 80 83 ;
	setAttr ".cd" -type "dataPolyComponent" Index_Data Edge 0 ;
	setAttr ".cvd" -type "dataPolyComponent" Index_Data Vertex 0 ;
	setAttr ".hfd" -type "dataPolyComponent" Index_Data Face 0 ;
createNode lightLinker -s -n "lightLinker1";
	setAttr -s 3 ".lnk";
	setAttr -s 3 ".slnk";
createNode displayLayerManager -n "layerManager";
createNode displayLayer -n "defaultLayer";
createNode renderLayerManager -n "renderLayerManager";
createNode renderLayer -n "defaultRenderLayer";
	setAttr ".g" yes;
createNode aiOptions -s -n "defaultArnoldRenderOptions";
createNode aiAOVFilter -s -n "defaultArnoldFilter";
	addAttr -ci true -sn "width" -ln "width" -dv 2 -at "float";
	addAttr -ci true -sn "domain" -ln "domain" -min 0 -max 1 -en "first_hit:all_hits" 
		-at "enum";
	addAttr -ci true -sn "minimum" -ln "minimum" -at "float";
	addAttr -ci true -sn "maximum" -ln "maximum" -dv 1 -at "float";
	addAttr -ci true -sn "scalar_mode" -ln "scalarMode" -min 0 -max 1 -at "bool";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVDriver -s -n "defaultArnoldDriver";
	addAttr -ci true -sn "exr_compression" -ln "exrCompression" -dv 3 -min 0 -max 5 
		-en "none:rle:zips:zip:piz:pxr24" -at "enum";
	addAttr -ci true -sn "half_precision" -ln "halfPrecision" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "tiled" -ln "tiled" -dv 1 -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "preserve_layer_name" -ln "preserveLayerName" -min 0 -max 1 
		-at "bool";
	addAttr -ci true -sn "autocrop" -ln "autocrop" -min 0 -max 1 -at "bool";
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
createNode polyCube -n "polyCube1";
	setAttr ".w" 6.84945309439372;
	setAttr ".h" 6.8444125139576242;
	setAttr ".d" 74.187618805370036;
	setAttr ".cuv" 4;
createNode polyTorus -n "polyTorus1";
	setAttr ".r" 120;
	setAttr ".sr" 39.527005950663245;
	setAttr ".sa" 50;
createNode polyCylinder -n "polyCylinder1";
	setAttr ".r" 4.1474618021593797;
	setAttr ".h" 11.521338160018104;
	setAttr ".sc" 1;
	setAttr ".cuv" 3;
createNode aiStandard -n "aiStandard1";
	setAttr ".Kd_color" -type "float3" 0.75800002 0.044722013 0.044722013 ;
createNode shadingEngine -n "aiStandard1SG";
	addAttr -ci true -m -sn "aovs" -ln "aiCustomAOVs" -at "compound" -nc 2;
	addAttr -ci true -k true -sn "aov_name" -ln "aovName" -dt "string" -p "aiCustomAOVs";
	addAttr -ci true -k true -sn "aov_input" -ln "aovInput" -at "message" -p "aiCustomAOVs";
	setAttr ".ihi" 0;
	setAttr ".ro" yes;
createNode materialInfo -n "materialInfo1";
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
		+ "\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `outlinerPanel -unParent -l (localizedPanelLabel(\"Outliner\")) -mbv $menusOkayInPanels `;\n\t\t\t$editorName = $panelName;\n            outlinerEditor -e \n                -showShapes 1\n                -showAttributes 0\n                -showConnected 0\n                -showAnimCurvesOnly 0\n                -showMuteInfo 0\n                -organizeByLayer 1\n                -showAnimLayerWeight 1\n                -autoExpandLayers 1\n                -autoExpand 0\n                -showDagOnly 0\n                -showAssets 1\n                -showContainedOnly 1\n                -showPublishedAsConnected 0\n                -showContainerContents 1\n                -ignoreDagHierarchy 0\n                -expandConnections 0\n                -showUpstreamCurves 1\n                -showUnitlessCurves 1\n                -showCompounds 1\n                -showLeafs 1\n                -showNumericAttrsOnly 0\n                -highlightActive 1\n                -autoSelectNewObjects 0\n"
		+ "                -doNotSelectNewObjects 0\n                -dropIsParent 1\n                -transmitFilters 0\n                -setFilter \"defaultSetFilter\" \n                -showSetMembers 1\n                -allowMultiSelection 1\n                -alwaysToggleSelect 0\n                -directSelect 0\n                -displayMode \"DAG\" \n                -expandObjects 0\n                -setsIgnoreFilters 1\n                -containersIgnoreFilters 0\n                -editAttrName 0\n                -showAttrValues 0\n                -highlightSecondary 0\n                -showUVAttrsOnly 0\n                -showTextureNodesOnly 0\n                -attrAlphaOrder \"default\" \n                -animLayerFilterOptions \"allAffecting\" \n                -sortOrder \"none\" \n                -longNames 0\n                -niceNames 1\n                -showNamespace 1\n                -showPinIcons 0\n                $editorName;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\toutlinerPanel -edit -l (localizedPanelLabel(\"Outliner\")) -mbv $menusOkayInPanels  $panelName;\n"
		+ "\t\t$editorName = $panelName;\n        outlinerEditor -e \n            -showShapes 1\n            -showAttributes 0\n            -showConnected 0\n            -showAnimCurvesOnly 0\n            -showMuteInfo 0\n            -organizeByLayer 1\n            -showAnimLayerWeight 1\n            -autoExpandLayers 1\n            -autoExpand 0\n            -showDagOnly 0\n            -showAssets 1\n            -showContainedOnly 1\n            -showPublishedAsConnected 0\n            -showContainerContents 1\n            -ignoreDagHierarchy 0\n            -expandConnections 0\n            -showUpstreamCurves 1\n            -showUnitlessCurves 1\n            -showCompounds 1\n            -showLeafs 1\n            -showNumericAttrsOnly 0\n            -highlightActive 1\n            -autoSelectNewObjects 0\n            -doNotSelectNewObjects 0\n            -dropIsParent 1\n            -transmitFilters 0\n            -setFilter \"defaultSetFilter\" \n            -showSetMembers 1\n            -allowMultiSelection 1\n            -alwaysToggleSelect 0\n            -directSelect 0\n"
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
		+ "\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"dynPaintScriptedPanelType\" -l (localizedPanelLabel(\"Paint Effects\")) -mbv $menusOkayInPanels `;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Paint Effects\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"scriptEditorPanel\" (localizedPanelLabel(\"Script Editor\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"scriptEditorPanel\" -l (localizedPanelLabel(\"Script Editor\")) -mbv $menusOkayInPanels `;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Script Editor\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\tif ($useSceneConfig) {\n        string $configName = `getPanel -cwl (localizedPanelLabel(\"Current Layout\"))`;\n"
		+ "        if (\"\" != $configName) {\n\t\t\tpanelConfiguration -edit -label (localizedPanelLabel(\"Current Layout\")) \n\t\t\t\t-defaultImage \"\"\n\t\t\t\t-image \"\"\n\t\t\t\t-sc false\n\t\t\t\t-configString \"global string $gMainPane; paneLayout -e -cn \\\"single\\\" -ps 1 100 100 $gMainPane;\"\n\t\t\t\t-removeAllPanels\n\t\t\t\t-ap false\n\t\t\t\t\t(localizedPanelLabel(\"Persp View\")) \n\t\t\t\t\t\"modelPanel\"\n"
		+ "\t\t\t\t\t\"$panelName = `modelPanel -unParent -l (localizedPanelLabel(\\\"Persp View\\\")) -mbv $menusOkayInPanels `;\\n$editorName = $panelName;\\nmodelEditor -e \\n    -cam `findStartUpCamera persp` \\n    -useInteractiveMode 0\\n    -displayLights \\\"default\\\" \\n    -displayAppearance \\\"wireframe\\\" \\n    -activeOnly 0\\n    -ignorePanZoom 0\\n    -wireframeOnShaded 0\\n    -headsUpDisplay 1\\n    -selectionHiliteDisplay 1\\n    -useDefaultMaterial 0\\n    -bufferMode \\\"double\\\" \\n    -twoSidedLighting 1\\n    -backfaceCulling 0\\n    -xray 0\\n    -jointXray 0\\n    -activeComponentsXray 0\\n    -displayTextures 0\\n    -smoothWireframe 0\\n    -lineWidth 1\\n    -textureAnisotropic 0\\n    -textureHilight 1\\n    -textureSampling 2\\n    -textureDisplay \\\"modulate\\\" \\n    -textureMaxSize 16384\\n    -fogging 0\\n    -fogSource \\\"fragment\\\" \\n    -fogMode \\\"linear\\\" \\n    -fogStart 0\\n    -fogEnd 100\\n    -fogDensity 0.1\\n    -fogColor 0.5 0.5 0.5 1 \\n    -maxConstantTransparency 1\\n    -rendererName \\\"base_OpenGL_Renderer\\\" \\n    -colorResolution 256 256 \\n    -bumpResolution 512 512 \\n    -textureCompression 0\\n    -transparencyAlgorithm \\\"frontAndBackCull\\\" \\n    -transpInShadows 0\\n    -cullingOverride \\\"none\\\" \\n    -lowQualityLighting 0\\n    -maximumNumHardwareLights 1\\n    -occlusionCulling 0\\n    -shadingModel 0\\n    -useBaseRenderer 0\\n    -useReducedRenderer 0\\n    -smallObjectCulling 0\\n    -smallObjectThreshold -1 \\n    -interactiveDisableShadows 0\\n    -interactiveBackFaceCull 0\\n    -sortTransparent 1\\n    -nurbsCurves 1\\n    -nurbsSurfaces 1\\n    -polymeshes 1\\n    -subdivSurfaces 1\\n    -planes 1\\n    -lights 1\\n    -cameras 1\\n    -controlVertices 1\\n    -hulls 1\\n    -grid 1\\n    -joints 1\\n    -ikHandles 1\\n    -deformers 1\\n    -dynamics 1\\n    -fluids 1\\n    -hairSystems 1\\n    -follicles 1\\n    -nCloths 1\\n    -nParticles 1\\n    -nRigids 1\\n    -dynamicConstraints 1\\n    -locators 1\\n    -manipulators 1\\n    -dimensions 1\\n    -handles 1\\n    -pivots 1\\n    -textures 1\\n    -strokes 1\\n    -shadows 0\\n    $editorName;\\nmodelEditor -e -viewSelected 0 $editorName\"\n"
		+ "\t\t\t\t\t\"modelPanel -edit -l (localizedPanelLabel(\\\"Persp View\\\")) -mbv $menusOkayInPanels  $panelName;\\n$editorName = $panelName;\\nmodelEditor -e \\n    -cam `findStartUpCamera persp` \\n    -useInteractiveMode 0\\n    -displayLights \\\"default\\\" \\n    -displayAppearance \\\"wireframe\\\" \\n    -activeOnly 0\\n    -ignorePanZoom 0\\n    -wireframeOnShaded 0\\n    -headsUpDisplay 1\\n    -selectionHiliteDisplay 1\\n    -useDefaultMaterial 0\\n    -bufferMode \\\"double\\\" \\n    -twoSidedLighting 1\\n    -backfaceCulling 0\\n    -xray 0\\n    -jointXray 0\\n    -activeComponentsXray 0\\n    -displayTextures 0\\n    -smoothWireframe 0\\n    -lineWidth 1\\n    -textureAnisotropic 0\\n    -textureHilight 1\\n    -textureSampling 2\\n    -textureDisplay \\\"modulate\\\" \\n    -textureMaxSize 16384\\n    -fogging 0\\n    -fogSource \\\"fragment\\\" \\n    -fogMode \\\"linear\\\" \\n    -fogStart 0\\n    -fogEnd 100\\n    -fogDensity 0.1\\n    -fogColor 0.5 0.5 0.5 1 \\n    -maxConstantTransparency 1\\n    -rendererName \\\"base_OpenGL_Renderer\\\" \\n    -colorResolution 256 256 \\n    -bumpResolution 512 512 \\n    -textureCompression 0\\n    -transparencyAlgorithm \\\"frontAndBackCull\\\" \\n    -transpInShadows 0\\n    -cullingOverride \\\"none\\\" \\n    -lowQualityLighting 0\\n    -maximumNumHardwareLights 1\\n    -occlusionCulling 0\\n    -shadingModel 0\\n    -useBaseRenderer 0\\n    -useReducedRenderer 0\\n    -smallObjectCulling 0\\n    -smallObjectThreshold -1 \\n    -interactiveDisableShadows 0\\n    -interactiveBackFaceCull 0\\n    -sortTransparent 1\\n    -nurbsCurves 1\\n    -nurbsSurfaces 1\\n    -polymeshes 1\\n    -subdivSurfaces 1\\n    -planes 1\\n    -lights 1\\n    -cameras 1\\n    -controlVertices 1\\n    -hulls 1\\n    -grid 1\\n    -joints 1\\n    -ikHandles 1\\n    -deformers 1\\n    -dynamics 1\\n    -fluids 1\\n    -hairSystems 1\\n    -follicles 1\\n    -nCloths 1\\n    -nParticles 1\\n    -nRigids 1\\n    -dynamicConstraints 1\\n    -locators 1\\n    -manipulators 1\\n    -dimensions 1\\n    -handles 1\\n    -pivots 1\\n    -textures 1\\n    -strokes 1\\n    -shadows 0\\n    $editorName;\\nmodelEditor -e -viewSelected 0 $editorName\"\n"
		+ "\t\t\t\t$configName;\n\n            setNamedPanelLayout (localizedPanelLabel(\"Current Layout\"));\n        }\n\n        panelHistory -e -clear mainPanelHistory;\n        setFocus `paneLayout -q -p1 $gMainPane`;\n        sceneUIReplacement -deleteRemaining;\n        sceneUIReplacement -clear;\n\t}\n\n\ngrid -spacing 5 -size 12 -divisions 5 -displayAxes yes -displayGridLines yes -displayDivisionLines yes -displayPerspectiveLabels no -displayOrthographicLabels no -displayAxesBold yes -perspectiveLabelPosition axis -orthographicLabelPosition edge;\nviewManip -drawCompass 0 -compassAngle 0 -frontParameters \"\" -homeParameters \"\" -selectionLockParameters \"\";\n}\n");
	setAttr ".st" 3;
createNode script -n "sceneConfigurationScriptNode";
	setAttr ".b" -type "string" "playbackOptions -min 1 -max 24 -ast 1 -aet 48 ";
	setAttr ".st" 6;
select -ne :time1;
	setAttr ".o" 1;
	setAttr ".unw" 1;
select -ne :renderPartition;
	setAttr -s 3 ".st";
select -ne :initialShadingGroup;
	addAttr -ci true -m -sn "aovs" -ln "aiCustomAOVs" -at "compound" -nc 2;
	addAttr -ci true -k true -sn "aov_name" -ln "aovName" -dt "string" -p "aiCustomAOVs";
	addAttr -ci true -k true -sn "aov_input" -ln "aovInput" -at "message" -p "aiCustomAOVs";
	setAttr -s 5 ".dsm";
	setAttr ".ro" yes;
select -ne :initialParticleSE;
	addAttr -ci true -m -sn "aovs" -ln "aiCustomAOVs" -at "compound" -nc 2;
	addAttr -ci true -k true -sn "aov_name" -ln "aovName" -dt "string" -p "aiCustomAOVs";
	addAttr -ci true -k true -sn "aov_input" -ln "aovInput" -at "message" -p "aiCustomAOVs";
	setAttr ".ro" yes;
select -ne :defaultShaderList1;
	setAttr -s 3 ".s";
select -ne :lightList1;
select -ne :postProcessList1;
	setAttr -s 2 ".p";
select -ne :renderGlobalsList1;
select -ne :defaultRenderGlobals;
	setAttr ".ren" -type "string" "arnold";
	setAttr ".outf" 51;
	setAttr ".imfkey" -type "string" "tif";
select -ne :defaultResolution;
	setAttr ".w" 160;
	setAttr ".h" 120;
	setAttr ".pa" 1;
	setAttr ".dar" 1.3333333730697632;
select -ne :defaultLightSet;
	addAttr -ci true -k true -sn "ai_override" -ln "aiOverride" -min 0 -max 1 -at "bool";
select -ne :hardwareRenderGlobals;
	setAttr ".ctrs" 256;
	setAttr ".btrs" 512;
select -ne :defaultHardwareRenderGlobals;
	setAttr ".fn" -type "string" "im";
	setAttr ".res" -type "string" "ntsc_4d 646 485 1.333";
connectAttr "polyCube1.out" "pCubeShape1.i";
connectAttr "polyTorus1.out" "pTorusShape1.i";
connectAttr "polyCylinder1.out" "pCylinderShape1.i";
relationship "link" ":lightLinker1" ":initialShadingGroup.message" ":defaultLightSet.message";
relationship "link" ":lightLinker1" ":initialParticleSE.message" ":defaultLightSet.message";
relationship "link" ":lightLinker1" "aiStandard1SG.message" ":defaultLightSet.message";
relationship "shadowLink" ":lightLinker1" ":initialShadingGroup.message" ":defaultLightSet.message";
relationship "shadowLink" ":lightLinker1" ":initialParticleSE.message" ":defaultLightSet.message";
relationship "shadowLink" ":lightLinker1" "aiStandard1SG.message" ":defaultLightSet.message";
connectAttr "layerManager.dli[0]" "defaultLayer.id";
connectAttr "renderLayerManager.rlmi[0]" "defaultRenderLayer.rlid";
connectAttr ":defaultArnoldFilter.msg" ":defaultArnoldRenderOptions.filt";
connectAttr ":defaultArnoldDriver.msg" ":defaultArnoldRenderOptions.drvr";
connectAttr "aiStandard1.out" "aiStandard1SG.ss";
connectAttr "pTorusShape1.iog" "aiStandard1SG.dsm" -na;
connectAttr "aiStandard1SG.msg" "materialInfo1.sg";
connectAttr "aiStandard1.msg" "materialInfo1.m";
connectAttr "aiStandard1.msg" "materialInfo1.t" -na;
connectAttr "aiStandard1SG.pa" ":renderPartition.st" -na;
connectAttr "pCubeShape1.iog" ":initialShadingGroup.dsm" -na;
connectAttr "pCylinderShape1.iog" ":initialShadingGroup.dsm" -na;
connectAttr "pCubeShape2.iog" ":initialShadingGroup.dsm" -na;
connectAttr "pCylinderShape2.iog" ":initialShadingGroup.dsm" -na;
connectAttr "pCylinderShape3.iog" ":initialShadingGroup.dsm" -na;
connectAttr "aiStandard1.msg" ":defaultShaderList1.s" -na;
connectAttr "directionalLightShape1.ltd" ":lightList1.l" -na;
connectAttr "directionalLight1.iog" ":defaultLightSet.dsm" -na;
// End of test.ma

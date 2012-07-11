//Maya ASCII 2011 scene
//Name: test.ma
//Last modified: Fri, Jun 29, 2012 10:30:25 AM
//Codeset: UTF-8
requires maya "2011";
requires "mtoa" "0.16.0";
requires "stereoCamera" "10.0";
currentUnit -l centimeter -a degree -t film;
fileInfo "application" "maya";
fileInfo "product" "Maya 2011";
fileInfo "version" "2011 x64";
fileInfo "cutIdentifier" "201009060248-781623";
fileInfo "osv" "Linux 2.6.32-220.23.1.el6.x86_64 #1 SMP Mon Jun 18 18:58:52 BST 2012 x86_64";
createNode transform -s -n "persp";
	setAttr ".v" no;
	setAttr ".t" -type "double3" 23 12 0 ;
	setAttr ".r" -type "double3" -24.000000000000004 90 0 ;
	setAttr ".rp" -type "double3" -3.5804692544161291e-15 -7.1054273576010019e-15 -1.4210854715202004e-14 ;
	setAttr ".rpt" -type "double3" -8.7783779362275568e-15 1.1498476670238775e-14 9.4020555375448207e-16 ;
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
	setAttr -k off ".v" no;
	setAttr ".fl" 34.999999999999979;
	setAttr ".coi" 21.832126661049116;
	setAttr ".imn" -type "string" "persp";
	setAttr ".den" -type "string" "persp_depth";
	setAttr ".man" -type "string" "persp_mask";
	setAttr ".tp" -type "double3" 3.529134343177768 5.7733093735849303e-16 2.6000673943571693 ;
	setAttr ".hc" -type "string" "viewSet -p %camera";
	setAttr -k on ".ai_translator" -type "string" "perspective";
createNode transform -s -n "top";
	setAttr ".v" no;
	setAttr ".t" -type "double3" 0 100.1 0 ;
	setAttr ".r" -type "double3" -89.999999999999986 0 0 ;
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
	setAttr -k off ".v" no;
	setAttr ".rnd" no;
	setAttr ".coi" 100.1;
	setAttr ".ow" 36.150126532178255;
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
createNode transform -s -n "side";
	setAttr ".v" no;
	setAttr ".t" -type "double3" 100.1 1.6871751536864319 12.910557697774426 ;
	setAttr ".r" -type "double3" 0 89.999999999999986 0 ;
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
	setAttr -k off ".v" no;
	setAttr ".rnd" no;
	setAttr ".coi" 100.1;
	setAttr ".ow" 51.715586232562323;
	setAttr ".imn" -type "string" "side";
	setAttr ".den" -type "string" "side_depth";
	setAttr ".man" -type "string" "side_mask";
	setAttr ".hc" -type "string" "viewSet -s %camera";
	setAttr ".o" yes;
	setAttr -k on ".ai_translator" -type "string" "orthographic";
createNode transform -n "pSphere1";
	setAttr ".t" -type "double3" 0 2 8 ;
createNode mesh -n "pSphereShape1" -p "pSphere1";
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
createNode transform -n "pSphere2";
	setAttr ".t" -type "double3" 0 2 -8 ;
createNode mesh -n "pSphereShape2" -p "pSphere2";
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
createNode transform -n "hairSystem1";
createNode hairSystem -n "hairSystemShape1" -p "hairSystem1";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	addAttr -ci true -sn "ai_self_shadows" -ln "aiSelfShadows" -dv 1 -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "ai_opaque" -ln "aiOpaque" -dv 1 -min 0 -max 1 -at "bool";
	addAttr -ci true -k true -sn "ai_vid" -ln "aiVisibleInDiffuse" -dv 1 -min 0 -max 
		1 -at "bool";
	addAttr -ci true -k true -sn "ai_vig" -ln "aiVisibleInGlossy" -dv 1 -min 0 -max 
		1 -at "bool";
	addAttr -ci true -sn "ai_min_pixel_width" -ln "aiMinPixelWidth" -at "float";
	addAttr -ci true -sn "ai_mode" -ln "aiMode" -min 0 -max 2 -en "ribbon:thick:oriented" 
		-at "enum";
	addAttr -ci true -k true -sn "vis" -ln "primaryVisibility" -dv 1 -min 0 -max 1 -at "bool";
	addAttr -ci true -k true -sn "csh" -ln "castsShadows" -dv 1 -min 0 -max 1 -at "bool";
	addAttr -ci true -k true -sn "ai_export_hair_ids" -ln "aiExportHairIDs" -dv 1 -min 
		0 -max 1 -at "bool";
	addAttr -ci true -k true -sn "ai_export_hair_uvs" -ln "aiExportHairUVs" -min 0 -max 
		1 -at "bool";
	addAttr -ci true -k true -sn "ai_export_hair_colors" -ln "aiExportHairColors" -min 
		0 -max 1 -at "bool";
	addAttr -ci true -k true -sn "ai_override_hair" -ln "aiOverrideHair" -min 0 -max 
		1 -at "bool";
	addAttr -ci true -k true -sn "ai_hair_shader" -ln "aiHairShader" -at "message";
	setAttr -k off ".v";
	setAttr -s 240 ".ih";
	setAttr ".evo" 0;
	setAttr ".sfn" 0.1;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr ".clc[0]"  0 0.5 1;
	setAttr ".cfl[0]"  0 0 1;
	setAttr -s 2 ".hws[0:1]"  0.80000001 1 3 1 0.2 3;
	setAttr -s 3 ".hcs";
	setAttr ".hcs[0].hcsp" 0;
	setAttr ".hcs[0].hcsc" -type "float3" 0.5 0.5 0.5 ;
	setAttr ".hcs[0].hcsi" 3;
	setAttr ".hcs[1].hcsp" 0.30000001192092896;
	setAttr ".hcs[1].hcsc" -type "float3" 0.80000001 0.80000001 0.80000001 ;
	setAttr ".hcs[1].hcsi" 3;
	setAttr ".hcs[2].hcsp" 1;
	setAttr ".hcs[2].hcsc" -type "float3" 1 1 1 ;
	setAttr ".hcs[2].hcsi" 3;
	setAttr ".dsc[0]"  0 1 1;
	setAttr -k on ".ai_override_hair" yes;
createNode transform -n "hairSystem1Follicles";
createNode transform -n "pSphere1Follicle303" -p "hairSystem1Follicles";
createNode follicle -n "pSphere1FollicleShape303" -p "pSphere1Follicle303";
	setAttr -k off ".v";
	setAttr ".pu" 0.03125;
	setAttr ".pv" 0.03125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve1" -p "pSphere1Follicle303";
createNode nurbsCurve -n "curveShape1" -p "curve1";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere1Follicle309" -p "hairSystem1Follicles";
createNode follicle -n "pSphere1FollicleShape309" -p "pSphere1Follicle309";
	setAttr -k off ".v";
	setAttr ".pu" 0.03125;
	setAttr ".pv" 0.09375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve2" -p "pSphere1Follicle309";
createNode nurbsCurve -n "curveShape2" -p "curve2";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere1Follicle315" -p "hairSystem1Follicles";
createNode follicle -n "pSphere1FollicleShape315" -p "pSphere1Follicle315";
	setAttr -k off ".v";
	setAttr ".pu" 0.03125;
	setAttr ".pv" 0.15625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve3" -p "pSphere1Follicle315";
createNode nurbsCurve -n "curveShape3" -p "curve3";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere1Follicle322" -p "hairSystem1Follicles";
createNode follicle -n "pSphere1FollicleShape322" -p "pSphere1Follicle322";
	setAttr -k off ".v";
	setAttr ".pu" 0.03125;
	setAttr ".pv" 0.21875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve4" -p "pSphere1Follicle322";
createNode nurbsCurve -n "curveShape4" -p "curve4";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere1Follicle328" -p "hairSystem1Follicles";
createNode follicle -n "pSphere1FollicleShape328" -p "pSphere1Follicle328";
	setAttr -k off ".v";
	setAttr ".pu" 0.03125;
	setAttr ".pv" 0.28125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve5" -p "pSphere1Follicle328";
createNode nurbsCurve -n "curveShape5" -p "curve5";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere1Follicle334" -p "hairSystem1Follicles";
createNode follicle -n "pSphere1FollicleShape334" -p "pSphere1Follicle334";
	setAttr -k off ".v";
	setAttr ".pu" 0.03125;
	setAttr ".pv" 0.34375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve6" -p "pSphere1Follicle334";
createNode nurbsCurve -n "curveShape6" -p "curve6";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere1Follicle340" -p "hairSystem1Follicles";
createNode follicle -n "pSphere1FollicleShape340" -p "pSphere1Follicle340";
	setAttr -k off ".v";
	setAttr ".pu" 0.03125;
	setAttr ".pv" 0.40625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve7" -p "pSphere1Follicle340";
createNode nurbsCurve -n "curveShape7" -p "curve7";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere1Follicle346" -p "hairSystem1Follicles";
createNode follicle -n "pSphere1FollicleShape346" -p "pSphere1Follicle346";
	setAttr -k off ".v";
	setAttr ".pu" 0.03125;
	setAttr ".pv" 0.46875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve8" -p "pSphere1Follicle346";
createNode nurbsCurve -n "curveShape8" -p "curve8";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere1Follicle353" -p "hairSystem1Follicles";
createNode follicle -n "pSphere1FollicleShape353" -p "pSphere1Follicle353";
	setAttr -k off ".v";
	setAttr ".pu" 0.03125;
	setAttr ".pv" 0.53125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve9" -p "pSphere1Follicle353";
createNode nurbsCurve -n "curveShape9" -p "curve9";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere1Follicle359" -p "hairSystem1Follicles";
createNode follicle -n "pSphere1FollicleShape359" -p "pSphere1Follicle359";
	setAttr -k off ".v";
	setAttr ".pu" 0.03125;
	setAttr ".pv" 0.59375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve10" -p "pSphere1Follicle359";
createNode nurbsCurve -n "curveShape10" -p "curve10";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere1Follicle365" -p "hairSystem1Follicles";
createNode follicle -n "pSphere1FollicleShape365" -p "pSphere1Follicle365";
	setAttr -k off ".v";
	setAttr ".pu" 0.03125;
	setAttr ".pv" 0.65625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve11" -p "pSphere1Follicle365";
createNode nurbsCurve -n "curveShape11" -p "curve11";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere1Follicle371" -p "hairSystem1Follicles";
createNode follicle -n "pSphere1FollicleShape371" -p "pSphere1Follicle371";
	setAttr -k off ".v";
	setAttr ".pu" 0.03125;
	setAttr ".pv" 0.71875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve12" -p "pSphere1Follicle371";
createNode nurbsCurve -n "curveShape12" -p "curve12";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere1Follicle377" -p "hairSystem1Follicles";
createNode follicle -n "pSphere1FollicleShape377" -p "pSphere1Follicle377";
	setAttr -k off ".v";
	setAttr ".pu" 0.03125;
	setAttr ".pv" 0.78125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve13" -p "pSphere1Follicle377";
createNode nurbsCurve -n "curveShape13" -p "curve13";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere1Follicle384" -p "hairSystem1Follicles";
createNode follicle -n "pSphere1FollicleShape384" -p "pSphere1Follicle384";
	setAttr -k off ".v";
	setAttr ".pu" 0.03125;
	setAttr ".pv" 0.84375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve14" -p "pSphere1Follicle384";
createNode nurbsCurve -n "curveShape14" -p "curve14";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere1Follicle390" -p "hairSystem1Follicles";
createNode follicle -n "pSphere1FollicleShape390" -p "pSphere1Follicle390";
	setAttr -k off ".v";
	setAttr ".pu" 0.03125;
	setAttr ".pv" 0.90625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve15" -p "pSphere1Follicle390";
createNode nurbsCurve -n "curveShape15" -p "curve15";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere1Follicle396" -p "hairSystem1Follicles";
createNode follicle -n "pSphere1FollicleShape396" -p "pSphere1Follicle396";
	setAttr -k off ".v";
	setAttr ".pu" 0.03125;
	setAttr ".pv" 0.96875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve16" -p "pSphere1Follicle396";
createNode nurbsCurve -n "curveShape16" -p "curve16";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere1Follicle909" -p "hairSystem1Follicles";
createNode follicle -n "pSphere1FollicleShape909" -p "pSphere1Follicle909";
	setAttr -k off ".v";
	setAttr ".pu" 0.09375;
	setAttr ".pv" 0.09375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve17" -p "pSphere1Follicle909";
createNode nurbsCurve -n "curveShape17" -p "curve17";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere1Follicle915" -p "hairSystem1Follicles";
createNode follicle -n "pSphere1FollicleShape915" -p "pSphere1Follicle915";
	setAttr -k off ".v";
	setAttr ".pu" 0.09375;
	setAttr ".pv" 0.15625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve18" -p "pSphere1Follicle915";
createNode nurbsCurve -n "curveShape18" -p "curve18";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere1Follicle922" -p "hairSystem1Follicles";
createNode follicle -n "pSphere1FollicleShape922" -p "pSphere1Follicle922";
	setAttr -k off ".v";
	setAttr ".pu" 0.09375;
	setAttr ".pv" 0.21875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve19" -p "pSphere1Follicle922";
createNode nurbsCurve -n "curveShape19" -p "curve19";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere1Follicle928" -p "hairSystem1Follicles";
createNode follicle -n "pSphere1FollicleShape928" -p "pSphere1Follicle928";
	setAttr -k off ".v";
	setAttr ".pu" 0.09375;
	setAttr ".pv" 0.28125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve20" -p "pSphere1Follicle928";
createNode nurbsCurve -n "curveShape20" -p "curve20";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere1Follicle934" -p "hairSystem1Follicles";
createNode follicle -n "pSphere1FollicleShape934" -p "pSphere1Follicle934";
	setAttr -k off ".v";
	setAttr ".pu" 0.09375;
	setAttr ".pv" 0.34375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve21" -p "pSphere1Follicle934";
createNode nurbsCurve -n "curveShape21" -p "curve21";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere1Follicle940" -p "hairSystem1Follicles";
createNode follicle -n "pSphere1FollicleShape940" -p "pSphere1Follicle940";
	setAttr -k off ".v";
	setAttr ".pu" 0.09375;
	setAttr ".pv" 0.40625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve22" -p "pSphere1Follicle940";
createNode nurbsCurve -n "curveShape22" -p "curve22";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere1Follicle946" -p "hairSystem1Follicles";
createNode follicle -n "pSphere1FollicleShape946" -p "pSphere1Follicle946";
	setAttr -k off ".v";
	setAttr ".pu" 0.09375;
	setAttr ".pv" 0.46875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve23" -p "pSphere1Follicle946";
createNode nurbsCurve -n "curveShape23" -p "curve23";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere1Follicle953" -p "hairSystem1Follicles";
createNode follicle -n "pSphere1FollicleShape953" -p "pSphere1Follicle953";
	setAttr -k off ".v";
	setAttr ".pu" 0.09375;
	setAttr ".pv" 0.53125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve24" -p "pSphere1Follicle953";
createNode nurbsCurve -n "curveShape24" -p "curve24";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere1Follicle959" -p "hairSystem1Follicles";
createNode follicle -n "pSphere1FollicleShape959" -p "pSphere1Follicle959";
	setAttr -k off ".v";
	setAttr ".pu" 0.09375;
	setAttr ".pv" 0.59375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve25" -p "pSphere1Follicle959";
createNode nurbsCurve -n "curveShape25" -p "curve25";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere1Follicle965" -p "hairSystem1Follicles";
createNode follicle -n "pSphere1FollicleShape965" -p "pSphere1Follicle965";
	setAttr -k off ".v";
	setAttr ".pu" 0.09375;
	setAttr ".pv" 0.65625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve26" -p "pSphere1Follicle965";
createNode nurbsCurve -n "curveShape26" -p "curve26";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere1Follicle971" -p "hairSystem1Follicles";
createNode follicle -n "pSphere1FollicleShape971" -p "pSphere1Follicle971";
	setAttr -k off ".v";
	setAttr ".pu" 0.09375;
	setAttr ".pv" 0.71875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve27" -p "pSphere1Follicle971";
createNode nurbsCurve -n "curveShape27" -p "curve27";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere1Follicle977" -p "hairSystem1Follicles";
createNode follicle -n "pSphere1FollicleShape977" -p "pSphere1Follicle977";
	setAttr -k off ".v";
	setAttr ".pu" 0.09375;
	setAttr ".pv" 0.78125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve28" -p "pSphere1Follicle977";
createNode nurbsCurve -n "curveShape28" -p "curve28";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere1Follicle984" -p "hairSystem1Follicles";
createNode follicle -n "pSphere1FollicleShape984" -p "pSphere1Follicle984";
	setAttr -k off ".v";
	setAttr ".pu" 0.09375;
	setAttr ".pv" 0.84375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve29" -p "pSphere1Follicle984";
createNode nurbsCurve -n "curveShape29" -p "curve29";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere1Follicle990" -p "hairSystem1Follicles";
createNode follicle -n "pSphere1FollicleShape990" -p "pSphere1Follicle990";
	setAttr -k off ".v";
	setAttr ".pu" 0.09375;
	setAttr ".pv" 0.90625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve30" -p "pSphere1Follicle990";
createNode nurbsCurve -n "curveShape30" -p "curve30";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere1Follicle1509" -p "hairSystem1Follicles";
createNode follicle -n "pSphere1FollicleShape1509" -p "pSphere1Follicle1509";
	setAttr -k off ".v";
	setAttr ".pu" 0.15625;
	setAttr ".pv" 0.09375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve31" -p "pSphere1Follicle1509";
createNode nurbsCurve -n "curveShape31" -p "curve31";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere1Follicle1515" -p "hairSystem1Follicles";
createNode follicle -n "pSphere1FollicleShape1515" -p "pSphere1Follicle1515";
	setAttr -k off ".v";
	setAttr ".pu" 0.15625;
	setAttr ".pv" 0.15625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve32" -p "pSphere1Follicle1515";
createNode nurbsCurve -n "curveShape32" -p "curve32";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere1Follicle1522" -p "hairSystem1Follicles";
createNode follicle -n "pSphere1FollicleShape1522" -p "pSphere1Follicle1522";
	setAttr -k off ".v";
	setAttr ".pu" 0.15625;
	setAttr ".pv" 0.21875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve33" -p "pSphere1Follicle1522";
createNode nurbsCurve -n "curveShape33" -p "curve33";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere1Follicle1528" -p "hairSystem1Follicles";
createNode follicle -n "pSphere1FollicleShape1528" -p "pSphere1Follicle1528";
	setAttr -k off ".v";
	setAttr ".pu" 0.15625;
	setAttr ".pv" 0.28125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve34" -p "pSphere1Follicle1528";
createNode nurbsCurve -n "curveShape34" -p "curve34";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere1Follicle1534" -p "hairSystem1Follicles";
createNode follicle -n "pSphere1FollicleShape1534" -p "pSphere1Follicle1534";
	setAttr -k off ".v";
	setAttr ".pu" 0.15625;
	setAttr ".pv" 0.34375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve35" -p "pSphere1Follicle1534";
createNode nurbsCurve -n "curveShape35" -p "curve35";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere1Follicle1540" -p "hairSystem1Follicles";
createNode follicle -n "pSphere1FollicleShape1540" -p "pSphere1Follicle1540";
	setAttr -k off ".v";
	setAttr ".pu" 0.15625;
	setAttr ".pv" 0.40625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve36" -p "pSphere1Follicle1540";
createNode nurbsCurve -n "curveShape36" -p "curve36";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere1Follicle1546" -p "hairSystem1Follicles";
createNode follicle -n "pSphere1FollicleShape1546" -p "pSphere1Follicle1546";
	setAttr -k off ".v";
	setAttr ".pu" 0.15625;
	setAttr ".pv" 0.46875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve37" -p "pSphere1Follicle1546";
createNode nurbsCurve -n "curveShape37" -p "curve37";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere1Follicle1553" -p "hairSystem1Follicles";
createNode follicle -n "pSphere1FollicleShape1553" -p "pSphere1Follicle1553";
	setAttr -k off ".v";
	setAttr ".pu" 0.15625;
	setAttr ".pv" 0.53125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve38" -p "pSphere1Follicle1553";
createNode nurbsCurve -n "curveShape38" -p "curve38";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere1Follicle1559" -p "hairSystem1Follicles";
createNode follicle -n "pSphere1FollicleShape1559" -p "pSphere1Follicle1559";
	setAttr -k off ".v";
	setAttr ".pu" 0.15625;
	setAttr ".pv" 0.59375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve39" -p "pSphere1Follicle1559";
createNode nurbsCurve -n "curveShape39" -p "curve39";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere1Follicle1565" -p "hairSystem1Follicles";
createNode follicle -n "pSphere1FollicleShape1565" -p "pSphere1Follicle1565";
	setAttr -k off ".v";
	setAttr ".pu" 0.15625;
	setAttr ".pv" 0.65625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve40" -p "pSphere1Follicle1565";
createNode nurbsCurve -n "curveShape40" -p "curve40";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere1Follicle1571" -p "hairSystem1Follicles";
createNode follicle -n "pSphere1FollicleShape1571" -p "pSphere1Follicle1571";
	setAttr -k off ".v";
	setAttr ".pu" 0.15625;
	setAttr ".pv" 0.71875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve41" -p "pSphere1Follicle1571";
createNode nurbsCurve -n "curveShape41" -p "curve41";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere1Follicle1577" -p "hairSystem1Follicles";
createNode follicle -n "pSphere1FollicleShape1577" -p "pSphere1Follicle1577";
	setAttr -k off ".v";
	setAttr ".pu" 0.15625;
	setAttr ".pv" 0.78125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve42" -p "pSphere1Follicle1577";
createNode nurbsCurve -n "curveShape42" -p "curve42";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere1Follicle1584" -p "hairSystem1Follicles";
createNode follicle -n "pSphere1FollicleShape1584" -p "pSphere1Follicle1584";
	setAttr -k off ".v";
	setAttr ".pu" 0.15625;
	setAttr ".pv" 0.84375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve43" -p "pSphere1Follicle1584";
createNode nurbsCurve -n "curveShape43" -p "curve43";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere1Follicle1590" -p "hairSystem1Follicles";
createNode follicle -n "pSphere1FollicleShape1590" -p "pSphere1Follicle1590";
	setAttr -k off ".v";
	setAttr ".pu" 0.15625;
	setAttr ".pv" 0.90625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve44" -p "pSphere1Follicle1590";
createNode nurbsCurve -n "curveShape44" -p "curve44";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere1Follicle2203" -p "hairSystem1Follicles";
createNode follicle -n "pSphere1FollicleShape2203" -p "pSphere1Follicle2203";
	setAttr -k off ".v";
	setAttr ".pu" 0.21875;
	setAttr ".pv" 0.03125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve45" -p "pSphere1Follicle2203";
createNode nurbsCurve -n "curveShape45" -p "curve45";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere1Follicle2209" -p "hairSystem1Follicles";
createNode follicle -n "pSphere1FollicleShape2209" -p "pSphere1Follicle2209";
	setAttr -k off ".v";
	setAttr ".pu" 0.21875;
	setAttr ".pv" 0.09375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve46" -p "pSphere1Follicle2209";
createNode nurbsCurve -n "curveShape46" -p "curve46";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere1Follicle2215" -p "hairSystem1Follicles";
createNode follicle -n "pSphere1FollicleShape2215" -p "pSphere1Follicle2215";
	setAttr -k off ".v";
	setAttr ".pu" 0.21875;
	setAttr ".pv" 0.15625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve47" -p "pSphere1Follicle2215";
createNode nurbsCurve -n "curveShape47" -p "curve47";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere1Follicle2222" -p "hairSystem1Follicles";
createNode follicle -n "pSphere1FollicleShape2222" -p "pSphere1Follicle2222";
	setAttr -k off ".v";
	setAttr ".pu" 0.21875;
	setAttr ".pv" 0.21875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve48" -p "pSphere1Follicle2222";
createNode nurbsCurve -n "curveShape48" -p "curve48";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere1Follicle2228" -p "hairSystem1Follicles";
createNode follicle -n "pSphere1FollicleShape2228" -p "pSphere1Follicle2228";
	setAttr -k off ".v";
	setAttr ".pu" 0.21875;
	setAttr ".pv" 0.28125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve49" -p "pSphere1Follicle2228";
createNode nurbsCurve -n "curveShape49" -p "curve49";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere1Follicle2234" -p "hairSystem1Follicles";
createNode follicle -n "pSphere1FollicleShape2234" -p "pSphere1Follicle2234";
	setAttr -k off ".v";
	setAttr ".pu" 0.21875;
	setAttr ".pv" 0.34375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve50" -p "pSphere1Follicle2234";
createNode nurbsCurve -n "curveShape50" -p "curve50";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere1Follicle2240" -p "hairSystem1Follicles";
createNode follicle -n "pSphere1FollicleShape2240" -p "pSphere1Follicle2240";
	setAttr -k off ".v";
	setAttr ".pu" 0.21875;
	setAttr ".pv" 0.40625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve51" -p "pSphere1Follicle2240";
createNode nurbsCurve -n "curveShape51" -p "curve51";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere1Follicle2246" -p "hairSystem1Follicles";
createNode follicle -n "pSphere1FollicleShape2246" -p "pSphere1Follicle2246";
	setAttr -k off ".v";
	setAttr ".pu" 0.21875;
	setAttr ".pv" 0.46875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve52" -p "pSphere1Follicle2246";
createNode nurbsCurve -n "curveShape52" -p "curve52";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere1Follicle2253" -p "hairSystem1Follicles";
createNode follicle -n "pSphere1FollicleShape2253" -p "pSphere1Follicle2253";
	setAttr -k off ".v";
	setAttr ".pu" 0.21875;
	setAttr ".pv" 0.53125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve53" -p "pSphere1Follicle2253";
createNode nurbsCurve -n "curveShape53" -p "curve53";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere1Follicle2259" -p "hairSystem1Follicles";
createNode follicle -n "pSphere1FollicleShape2259" -p "pSphere1Follicle2259";
	setAttr -k off ".v";
	setAttr ".pu" 0.21875;
	setAttr ".pv" 0.59375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve54" -p "pSphere1Follicle2259";
createNode nurbsCurve -n "curveShape54" -p "curve54";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere1Follicle2265" -p "hairSystem1Follicles";
createNode follicle -n "pSphere1FollicleShape2265" -p "pSphere1Follicle2265";
	setAttr -k off ".v";
	setAttr ".pu" 0.21875;
	setAttr ".pv" 0.65625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve55" -p "pSphere1Follicle2265";
createNode nurbsCurve -n "curveShape55" -p "curve55";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere1Follicle2271" -p "hairSystem1Follicles";
createNode follicle -n "pSphere1FollicleShape2271" -p "pSphere1Follicle2271";
	setAttr -k off ".v";
	setAttr ".pu" 0.21875;
	setAttr ".pv" 0.71875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve56" -p "pSphere1Follicle2271";
createNode nurbsCurve -n "curveShape56" -p "curve56";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere1Follicle2277" -p "hairSystem1Follicles";
createNode follicle -n "pSphere1FollicleShape2277" -p "pSphere1Follicle2277";
	setAttr -k off ".v";
	setAttr ".pu" 0.21875;
	setAttr ".pv" 0.78125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve57" -p "pSphere1Follicle2277";
createNode nurbsCurve -n "curveShape57" -p "curve57";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere1Follicle2284" -p "hairSystem1Follicles";
createNode follicle -n "pSphere1FollicleShape2284" -p "pSphere1Follicle2284";
	setAttr -k off ".v";
	setAttr ".pu" 0.21875;
	setAttr ".pv" 0.84375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve58" -p "pSphere1Follicle2284";
createNode nurbsCurve -n "curveShape58" -p "curve58";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere1Follicle2290" -p "hairSystem1Follicles";
createNode follicle -n "pSphere1FollicleShape2290" -p "pSphere1Follicle2290";
	setAttr -k off ".v";
	setAttr ".pu" 0.21875;
	setAttr ".pv" 0.90625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve59" -p "pSphere1Follicle2290";
createNode nurbsCurve -n "curveShape59" -p "curve59";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere1Follicle2296" -p "hairSystem1Follicles";
createNode follicle -n "pSphere1FollicleShape2296" -p "pSphere1Follicle2296";
	setAttr -k off ".v";
	setAttr ".pu" 0.21875;
	setAttr ".pv" 0.96875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve60" -p "pSphere1Follicle2296";
createNode nurbsCurve -n "curveShape60" -p "curve60";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere1Follicle2803" -p "hairSystem1Follicles";
createNode follicle -n "pSphere1FollicleShape2803" -p "pSphere1Follicle2803";
	setAttr -k off ".v";
	setAttr ".pu" 0.28125;
	setAttr ".pv" 0.03125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve61" -p "pSphere1Follicle2803";
createNode nurbsCurve -n "curveShape61" -p "curve61";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere1Follicle2809" -p "hairSystem1Follicles";
createNode follicle -n "pSphere1FollicleShape2809" -p "pSphere1Follicle2809";
	setAttr -k off ".v";
	setAttr ".pu" 0.28125;
	setAttr ".pv" 0.09375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve62" -p "pSphere1Follicle2809";
createNode nurbsCurve -n "curveShape62" -p "curve62";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere1Follicle2815" -p "hairSystem1Follicles";
createNode follicle -n "pSphere1FollicleShape2815" -p "pSphere1Follicle2815";
	setAttr -k off ".v";
	setAttr ".pu" 0.28125;
	setAttr ".pv" 0.15625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve63" -p "pSphere1Follicle2815";
createNode nurbsCurve -n "curveShape63" -p "curve63";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere1Follicle2822" -p "hairSystem1Follicles";
createNode follicle -n "pSphere1FollicleShape2822" -p "pSphere1Follicle2822";
	setAttr -k off ".v";
	setAttr ".pu" 0.28125;
	setAttr ".pv" 0.21875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve64" -p "pSphere1Follicle2822";
createNode nurbsCurve -n "curveShape64" -p "curve64";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere1Follicle2828" -p "hairSystem1Follicles";
createNode follicle -n "pSphere1FollicleShape2828" -p "pSphere1Follicle2828";
	setAttr -k off ".v";
	setAttr ".pu" 0.28125;
	setAttr ".pv" 0.28125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve65" -p "pSphere1Follicle2828";
createNode nurbsCurve -n "curveShape65" -p "curve65";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere1Follicle2834" -p "hairSystem1Follicles";
createNode follicle -n "pSphere1FollicleShape2834" -p "pSphere1Follicle2834";
	setAttr -k off ".v";
	setAttr ".pu" 0.28125;
	setAttr ".pv" 0.34375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve66" -p "pSphere1Follicle2834";
createNode nurbsCurve -n "curveShape66" -p "curve66";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere1Follicle2840" -p "hairSystem1Follicles";
createNode follicle -n "pSphere1FollicleShape2840" -p "pSphere1Follicle2840";
	setAttr -k off ".v";
	setAttr ".pu" 0.28125;
	setAttr ".pv" 0.40625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve67" -p "pSphere1Follicle2840";
createNode nurbsCurve -n "curveShape67" -p "curve67";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere1Follicle2846" -p "hairSystem1Follicles";
createNode follicle -n "pSphere1FollicleShape2846" -p "pSphere1Follicle2846";
	setAttr -k off ".v";
	setAttr ".pu" 0.28125;
	setAttr ".pv" 0.46875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve68" -p "pSphere1Follicle2846";
createNode nurbsCurve -n "curveShape68" -p "curve68";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere1Follicle2853" -p "hairSystem1Follicles";
createNode follicle -n "pSphere1FollicleShape2853" -p "pSphere1Follicle2853";
	setAttr -k off ".v";
	setAttr ".pu" 0.28125;
	setAttr ".pv" 0.53125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve69" -p "pSphere1Follicle2853";
createNode nurbsCurve -n "curveShape69" -p "curve69";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere1Follicle2859" -p "hairSystem1Follicles";
createNode follicle -n "pSphere1FollicleShape2859" -p "pSphere1Follicle2859";
	setAttr -k off ".v";
	setAttr ".pu" 0.28125;
	setAttr ".pv" 0.59375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve70" -p "pSphere1Follicle2859";
createNode nurbsCurve -n "curveShape70" -p "curve70";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere1Follicle2865" -p "hairSystem1Follicles";
createNode follicle -n "pSphere1FollicleShape2865" -p "pSphere1Follicle2865";
	setAttr -k off ".v";
	setAttr ".pu" 0.28125;
	setAttr ".pv" 0.65625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve71" -p "pSphere1Follicle2865";
createNode nurbsCurve -n "curveShape71" -p "curve71";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere1Follicle2871" -p "hairSystem1Follicles";
createNode follicle -n "pSphere1FollicleShape2871" -p "pSphere1Follicle2871";
	setAttr -k off ".v";
	setAttr ".pu" 0.28125;
	setAttr ".pv" 0.71875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve72" -p "pSphere1Follicle2871";
createNode nurbsCurve -n "curveShape72" -p "curve72";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere1Follicle2877" -p "hairSystem1Follicles";
createNode follicle -n "pSphere1FollicleShape2877" -p "pSphere1Follicle2877";
	setAttr -k off ".v";
	setAttr ".pu" 0.28125;
	setAttr ".pv" 0.78125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve73" -p "pSphere1Follicle2877";
createNode nurbsCurve -n "curveShape73" -p "curve73";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere1Follicle2884" -p "hairSystem1Follicles";
createNode follicle -n "pSphere1FollicleShape2884" -p "pSphere1Follicle2884";
	setAttr -k off ".v";
	setAttr ".pu" 0.28125;
	setAttr ".pv" 0.84375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve74" -p "pSphere1Follicle2884";
createNode nurbsCurve -n "curveShape74" -p "curve74";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere1Follicle2890" -p "hairSystem1Follicles";
createNode follicle -n "pSphere1FollicleShape2890" -p "pSphere1Follicle2890";
	setAttr -k off ".v";
	setAttr ".pu" 0.28125;
	setAttr ".pv" 0.90625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve75" -p "pSphere1Follicle2890";
createNode nurbsCurve -n "curveShape75" -p "curve75";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere1Follicle2896" -p "hairSystem1Follicles";
createNode follicle -n "pSphere1FollicleShape2896" -p "pSphere1Follicle2896";
	setAttr -k off ".v";
	setAttr ".pu" 0.28125;
	setAttr ".pv" 0.96875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve76" -p "pSphere1Follicle2896";
createNode nurbsCurve -n "curveShape76" -p "curve76";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere1Follicle3409" -p "hairSystem1Follicles";
createNode follicle -n "pSphere1FollicleShape3409" -p "pSphere1Follicle3409";
	setAttr -k off ".v";
	setAttr ".pu" 0.34375;
	setAttr ".pv" 0.09375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve77" -p "pSphere1Follicle3409";
createNode nurbsCurve -n "curveShape77" -p "curve77";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere1Follicle3415" -p "hairSystem1Follicles";
createNode follicle -n "pSphere1FollicleShape3415" -p "pSphere1Follicle3415";
	setAttr -k off ".v";
	setAttr ".pu" 0.34375;
	setAttr ".pv" 0.15625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve78" -p "pSphere1Follicle3415";
createNode nurbsCurve -n "curveShape78" -p "curve78";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere1Follicle3422" -p "hairSystem1Follicles";
createNode follicle -n "pSphere1FollicleShape3422" -p "pSphere1Follicle3422";
	setAttr -k off ".v";
	setAttr ".pu" 0.34375;
	setAttr ".pv" 0.21875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve79" -p "pSphere1Follicle3422";
createNode nurbsCurve -n "curveShape79" -p "curve79";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere1Follicle3428" -p "hairSystem1Follicles";
createNode follicle -n "pSphere1FollicleShape3428" -p "pSphere1Follicle3428";
	setAttr -k off ".v";
	setAttr ".pu" 0.34375;
	setAttr ".pv" 0.28125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve80" -p "pSphere1Follicle3428";
createNode nurbsCurve -n "curveShape80" -p "curve80";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere1Follicle3434" -p "hairSystem1Follicles";
createNode follicle -n "pSphere1FollicleShape3434" -p "pSphere1Follicle3434";
	setAttr -k off ".v";
	setAttr ".pu" 0.34375;
	setAttr ".pv" 0.34375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve81" -p "pSphere1Follicle3434";
createNode nurbsCurve -n "curveShape81" -p "curve81";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere1Follicle3440" -p "hairSystem1Follicles";
createNode follicle -n "pSphere1FollicleShape3440" -p "pSphere1Follicle3440";
	setAttr -k off ".v";
	setAttr ".pu" 0.34375;
	setAttr ".pv" 0.40625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve82" -p "pSphere1Follicle3440";
createNode nurbsCurve -n "curveShape82" -p "curve82";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere1Follicle3446" -p "hairSystem1Follicles";
createNode follicle -n "pSphere1FollicleShape3446" -p "pSphere1Follicle3446";
	setAttr -k off ".v";
	setAttr ".pu" 0.34375;
	setAttr ".pv" 0.46875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve83" -p "pSphere1Follicle3446";
createNode nurbsCurve -n "curveShape83" -p "curve83";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere1Follicle3453" -p "hairSystem1Follicles";
createNode follicle -n "pSphere1FollicleShape3453" -p "pSphere1Follicle3453";
	setAttr -k off ".v";
	setAttr ".pu" 0.34375;
	setAttr ".pv" 0.53125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve84" -p "pSphere1Follicle3453";
createNode nurbsCurve -n "curveShape84" -p "curve84";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere1Follicle3459" -p "hairSystem1Follicles";
createNode follicle -n "pSphere1FollicleShape3459" -p "pSphere1Follicle3459";
	setAttr -k off ".v";
	setAttr ".pu" 0.34375;
	setAttr ".pv" 0.59375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve85" -p "pSphere1Follicle3459";
createNode nurbsCurve -n "curveShape85" -p "curve85";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere1Follicle3465" -p "hairSystem1Follicles";
createNode follicle -n "pSphere1FollicleShape3465" -p "pSphere1Follicle3465";
	setAttr -k off ".v";
	setAttr ".pu" 0.34375;
	setAttr ".pv" 0.65625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve86" -p "pSphere1Follicle3465";
createNode nurbsCurve -n "curveShape86" -p "curve86";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere1Follicle3471" -p "hairSystem1Follicles";
createNode follicle -n "pSphere1FollicleShape3471" -p "pSphere1Follicle3471";
	setAttr -k off ".v";
	setAttr ".pu" 0.34375;
	setAttr ".pv" 0.71875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve87" -p "pSphere1Follicle3471";
createNode nurbsCurve -n "curveShape87" -p "curve87";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere1Follicle3477" -p "hairSystem1Follicles";
createNode follicle -n "pSphere1FollicleShape3477" -p "pSphere1Follicle3477";
	setAttr -k off ".v";
	setAttr ".pu" 0.34375;
	setAttr ".pv" 0.78125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve88" -p "pSphere1Follicle3477";
createNode nurbsCurve -n "curveShape88" -p "curve88";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere1Follicle3484" -p "hairSystem1Follicles";
createNode follicle -n "pSphere1FollicleShape3484" -p "pSphere1Follicle3484";
	setAttr -k off ".v";
	setAttr ".pu" 0.34375;
	setAttr ".pv" 0.84375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve89" -p "pSphere1Follicle3484";
createNode nurbsCurve -n "curveShape89" -p "curve89";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere1Follicle3490" -p "hairSystem1Follicles";
createNode follicle -n "pSphere1FollicleShape3490" -p "pSphere1Follicle3490";
	setAttr -k off ".v";
	setAttr ".pu" 0.34375;
	setAttr ".pv" 0.90625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve90" -p "pSphere1Follicle3490";
createNode nurbsCurve -n "curveShape90" -p "curve90";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere1Follicle4009" -p "hairSystem1Follicles";
createNode follicle -n "pSphere1FollicleShape4009" -p "pSphere1Follicle4009";
	setAttr -k off ".v";
	setAttr ".pu" 0.40625;
	setAttr ".pv" 0.09375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve91" -p "pSphere1Follicle4009";
createNode nurbsCurve -n "curveShape91" -p "curve91";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere1Follicle4015" -p "hairSystem1Follicles";
createNode follicle -n "pSphere1FollicleShape4015" -p "pSphere1Follicle4015";
	setAttr -k off ".v";
	setAttr ".pu" 0.40625;
	setAttr ".pv" 0.15625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve92" -p "pSphere1Follicle4015";
createNode nurbsCurve -n "curveShape92" -p "curve92";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere1Follicle4022" -p "hairSystem1Follicles";
createNode follicle -n "pSphere1FollicleShape4022" -p "pSphere1Follicle4022";
	setAttr -k off ".v";
	setAttr ".pu" 0.40625;
	setAttr ".pv" 0.21875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve93" -p "pSphere1Follicle4022";
createNode nurbsCurve -n "curveShape93" -p "curve93";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere1Follicle4028" -p "hairSystem1Follicles";
createNode follicle -n "pSphere1FollicleShape4028" -p "pSphere1Follicle4028";
	setAttr -k off ".v";
	setAttr ".pu" 0.40625;
	setAttr ".pv" 0.28125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve94" -p "pSphere1Follicle4028";
createNode nurbsCurve -n "curveShape94" -p "curve94";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere1Follicle4034" -p "hairSystem1Follicles";
createNode follicle -n "pSphere1FollicleShape4034" -p "pSphere1Follicle4034";
	setAttr -k off ".v";
	setAttr ".pu" 0.40625;
	setAttr ".pv" 0.34375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve95" -p "pSphere1Follicle4034";
createNode nurbsCurve -n "curveShape95" -p "curve95";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere1Follicle4040" -p "hairSystem1Follicles";
createNode follicle -n "pSphere1FollicleShape4040" -p "pSphere1Follicle4040";
	setAttr -k off ".v";
	setAttr ".pu" 0.40625;
	setAttr ".pv" 0.40625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve96" -p "pSphere1Follicle4040";
createNode nurbsCurve -n "curveShape96" -p "curve96";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere1Follicle4046" -p "hairSystem1Follicles";
createNode follicle -n "pSphere1FollicleShape4046" -p "pSphere1Follicle4046";
	setAttr -k off ".v";
	setAttr ".pu" 0.40625;
	setAttr ".pv" 0.46875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve97" -p "pSphere1Follicle4046";
createNode nurbsCurve -n "curveShape97" -p "curve97";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere1Follicle4053" -p "hairSystem1Follicles";
createNode follicle -n "pSphere1FollicleShape4053" -p "pSphere1Follicle4053";
	setAttr -k off ".v";
	setAttr ".pu" 0.40625;
	setAttr ".pv" 0.53125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve98" -p "pSphere1Follicle4053";
createNode nurbsCurve -n "curveShape98" -p "curve98";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere1Follicle4059" -p "hairSystem1Follicles";
createNode follicle -n "pSphere1FollicleShape4059" -p "pSphere1Follicle4059";
	setAttr -k off ".v";
	setAttr ".pu" 0.40625;
	setAttr ".pv" 0.59375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve99" -p "pSphere1Follicle4059";
createNode nurbsCurve -n "curveShape99" -p "curve99";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere1Follicle4065" -p "hairSystem1Follicles";
createNode follicle -n "pSphere1FollicleShape4065" -p "pSphere1Follicle4065";
	setAttr -k off ".v";
	setAttr ".pu" 0.40625;
	setAttr ".pv" 0.65625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve100" -p "pSphere1Follicle4065";
createNode nurbsCurve -n "curveShape100" -p "curve100";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere1Follicle4071" -p "hairSystem1Follicles";
createNode follicle -n "pSphere1FollicleShape4071" -p "pSphere1Follicle4071";
	setAttr -k off ".v";
	setAttr ".pu" 0.40625;
	setAttr ".pv" 0.71875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve101" -p "pSphere1Follicle4071";
createNode nurbsCurve -n "curveShape101" -p "curve101";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere1Follicle4077" -p "hairSystem1Follicles";
createNode follicle -n "pSphere1FollicleShape4077" -p "pSphere1Follicle4077";
	setAttr -k off ".v";
	setAttr ".pu" 0.40625;
	setAttr ".pv" 0.78125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve102" -p "pSphere1Follicle4077";
createNode nurbsCurve -n "curveShape102" -p "curve102";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere1Follicle4084" -p "hairSystem1Follicles";
createNode follicle -n "pSphere1FollicleShape4084" -p "pSphere1Follicle4084";
	setAttr -k off ".v";
	setAttr ".pu" 0.40625;
	setAttr ".pv" 0.84375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve103" -p "pSphere1Follicle4084";
createNode nurbsCurve -n "curveShape103" -p "curve103";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere1Follicle4090" -p "hairSystem1Follicles";
createNode follicle -n "pSphere1FollicleShape4090" -p "pSphere1Follicle4090";
	setAttr -k off ".v";
	setAttr ".pu" 0.40625;
	setAttr ".pv" 0.90625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve104" -p "pSphere1Follicle4090";
createNode nurbsCurve -n "curveShape104" -p "curve104";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere1Follicle4603" -p "hairSystem1Follicles";
createNode follicle -n "pSphere1FollicleShape4603" -p "pSphere1Follicle4603";
	setAttr -k off ".v";
	setAttr ".pu" 0.46875;
	setAttr ".pv" 0.03125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve105" -p "pSphere1Follicle4603";
createNode nurbsCurve -n "curveShape105" -p "curve105";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere1Follicle4609" -p "hairSystem1Follicles";
createNode follicle -n "pSphere1FollicleShape4609" -p "pSphere1Follicle4609";
	setAttr -k off ".v";
	setAttr ".pu" 0.46875;
	setAttr ".pv" 0.09375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve106" -p "pSphere1Follicle4609";
createNode nurbsCurve -n "curveShape106" -p "curve106";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere1Follicle4615" -p "hairSystem1Follicles";
createNode follicle -n "pSphere1FollicleShape4615" -p "pSphere1Follicle4615";
	setAttr -k off ".v";
	setAttr ".pu" 0.46875;
	setAttr ".pv" 0.15625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve107" -p "pSphere1Follicle4615";
createNode nurbsCurve -n "curveShape107" -p "curve107";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere1Follicle4622" -p "hairSystem1Follicles";
createNode follicle -n "pSphere1FollicleShape4622" -p "pSphere1Follicle4622";
	setAttr -k off ".v";
	setAttr ".pu" 0.46875;
	setAttr ".pv" 0.21875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve108" -p "pSphere1Follicle4622";
createNode nurbsCurve -n "curveShape108" -p "curve108";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere1Follicle4628" -p "hairSystem1Follicles";
createNode follicle -n "pSphere1FollicleShape4628" -p "pSphere1Follicle4628";
	setAttr -k off ".v";
	setAttr ".pu" 0.46875;
	setAttr ".pv" 0.28125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve109" -p "pSphere1Follicle4628";
createNode nurbsCurve -n "curveShape109" -p "curve109";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere1Follicle4634" -p "hairSystem1Follicles";
createNode follicle -n "pSphere1FollicleShape4634" -p "pSphere1Follicle4634";
	setAttr -k off ".v";
	setAttr ".pu" 0.46875;
	setAttr ".pv" 0.34375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve110" -p "pSphere1Follicle4634";
createNode nurbsCurve -n "curveShape110" -p "curve110";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere1Follicle4640" -p "hairSystem1Follicles";
createNode follicle -n "pSphere1FollicleShape4640" -p "pSphere1Follicle4640";
	setAttr -k off ".v";
	setAttr ".pu" 0.46875;
	setAttr ".pv" 0.40625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve111" -p "pSphere1Follicle4640";
createNode nurbsCurve -n "curveShape111" -p "curve111";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere1Follicle4646" -p "hairSystem1Follicles";
createNode follicle -n "pSphere1FollicleShape4646" -p "pSphere1Follicle4646";
	setAttr -k off ".v";
	setAttr ".pu" 0.46875;
	setAttr ".pv" 0.46875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve112" -p "pSphere1Follicle4646";
createNode nurbsCurve -n "curveShape112" -p "curve112";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere1Follicle4653" -p "hairSystem1Follicles";
createNode follicle -n "pSphere1FollicleShape4653" -p "pSphere1Follicle4653";
	setAttr -k off ".v";
	setAttr ".pu" 0.46875;
	setAttr ".pv" 0.53125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve113" -p "pSphere1Follicle4653";
createNode nurbsCurve -n "curveShape113" -p "curve113";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere1Follicle4659" -p "hairSystem1Follicles";
createNode follicle -n "pSphere1FollicleShape4659" -p "pSphere1Follicle4659";
	setAttr -k off ".v";
	setAttr ".pu" 0.46875;
	setAttr ".pv" 0.59375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve114" -p "pSphere1Follicle4659";
createNode nurbsCurve -n "curveShape114" -p "curve114";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere1Follicle4665" -p "hairSystem1Follicles";
createNode follicle -n "pSphere1FollicleShape4665" -p "pSphere1Follicle4665";
	setAttr -k off ".v";
	setAttr ".pu" 0.46875;
	setAttr ".pv" 0.65625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve115" -p "pSphere1Follicle4665";
createNode nurbsCurve -n "curveShape115" -p "curve115";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere1Follicle4671" -p "hairSystem1Follicles";
createNode follicle -n "pSphere1FollicleShape4671" -p "pSphere1Follicle4671";
	setAttr -k off ".v";
	setAttr ".pu" 0.46875;
	setAttr ".pv" 0.71875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve116" -p "pSphere1Follicle4671";
createNode nurbsCurve -n "curveShape116" -p "curve116";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere1Follicle4677" -p "hairSystem1Follicles";
createNode follicle -n "pSphere1FollicleShape4677" -p "pSphere1Follicle4677";
	setAttr -k off ".v";
	setAttr ".pu" 0.46875;
	setAttr ".pv" 0.78125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve117" -p "pSphere1Follicle4677";
createNode nurbsCurve -n "curveShape117" -p "curve117";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere1Follicle4684" -p "hairSystem1Follicles";
createNode follicle -n "pSphere1FollicleShape4684" -p "pSphere1Follicle4684";
	setAttr -k off ".v";
	setAttr ".pu" 0.46875;
	setAttr ".pv" 0.84375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve118" -p "pSphere1Follicle4684";
createNode nurbsCurve -n "curveShape118" -p "curve118";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere1Follicle4690" -p "hairSystem1Follicles";
createNode follicle -n "pSphere1FollicleShape4690" -p "pSphere1Follicle4690";
	setAttr -k off ".v";
	setAttr ".pu" 0.46875;
	setAttr ".pv" 0.90625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve119" -p "pSphere1Follicle4690";
createNode nurbsCurve -n "curveShape119" -p "curve119";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere1Follicle4696" -p "hairSystem1Follicles";
createNode follicle -n "pSphere1FollicleShape4696" -p "pSphere1Follicle4696";
	setAttr -k off ".v";
	setAttr ".pu" 0.46875;
	setAttr ".pv" 0.96875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve120" -p "pSphere1Follicle4696";
createNode nurbsCurve -n "curveShape120" -p "curve120";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere1Follicle5303" -p "hairSystem1Follicles";
createNode follicle -n "pSphere1FollicleShape5303" -p "pSphere1Follicle5303";
	setAttr -k off ".v";
	setAttr ".pu" 0.53125;
	setAttr ".pv" 0.03125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve121" -p "pSphere1Follicle5303";
createNode nurbsCurve -n "curveShape121" -p "curve121";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere1Follicle5309" -p "hairSystem1Follicles";
createNode follicle -n "pSphere1FollicleShape5309" -p "pSphere1Follicle5309";
	setAttr -k off ".v";
	setAttr ".pu" 0.53125;
	setAttr ".pv" 0.09375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve122" -p "pSphere1Follicle5309";
createNode nurbsCurve -n "curveShape122" -p "curve122";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere1Follicle5315" -p "hairSystem1Follicles";
createNode follicle -n "pSphere1FollicleShape5315" -p "pSphere1Follicle5315";
	setAttr -k off ".v";
	setAttr ".pu" 0.53125;
	setAttr ".pv" 0.15625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve123" -p "pSphere1Follicle5315";
createNode nurbsCurve -n "curveShape123" -p "curve123";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere1Follicle5322" -p "hairSystem1Follicles";
createNode follicle -n "pSphere1FollicleShape5322" -p "pSphere1Follicle5322";
	setAttr -k off ".v";
	setAttr ".pu" 0.53125;
	setAttr ".pv" 0.21875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve124" -p "pSphere1Follicle5322";
createNode nurbsCurve -n "curveShape124" -p "curve124";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere1Follicle5328" -p "hairSystem1Follicles";
createNode follicle -n "pSphere1FollicleShape5328" -p "pSphere1Follicle5328";
	setAttr -k off ".v";
	setAttr ".pu" 0.53125;
	setAttr ".pv" 0.28125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve125" -p "pSphere1Follicle5328";
createNode nurbsCurve -n "curveShape125" -p "curve125";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere1Follicle5334" -p "hairSystem1Follicles";
createNode follicle -n "pSphere1FollicleShape5334" -p "pSphere1Follicle5334";
	setAttr -k off ".v";
	setAttr ".pu" 0.53125;
	setAttr ".pv" 0.34375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve126" -p "pSphere1Follicle5334";
createNode nurbsCurve -n "curveShape126" -p "curve126";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere1Follicle5340" -p "hairSystem1Follicles";
createNode follicle -n "pSphere1FollicleShape5340" -p "pSphere1Follicle5340";
	setAttr -k off ".v";
	setAttr ".pu" 0.53125;
	setAttr ".pv" 0.40625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve127" -p "pSphere1Follicle5340";
createNode nurbsCurve -n "curveShape127" -p "curve127";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere1Follicle5346" -p "hairSystem1Follicles";
createNode follicle -n "pSphere1FollicleShape5346" -p "pSphere1Follicle5346";
	setAttr -k off ".v";
	setAttr ".pu" 0.53125;
	setAttr ".pv" 0.46875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve128" -p "pSphere1Follicle5346";
createNode nurbsCurve -n "curveShape128" -p "curve128";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere1Follicle5353" -p "hairSystem1Follicles";
createNode follicle -n "pSphere1FollicleShape5353" -p "pSphere1Follicle5353";
	setAttr -k off ".v";
	setAttr ".pu" 0.53125;
	setAttr ".pv" 0.53125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve129" -p "pSphere1Follicle5353";
createNode nurbsCurve -n "curveShape129" -p "curve129";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere1Follicle5359" -p "hairSystem1Follicles";
createNode follicle -n "pSphere1FollicleShape5359" -p "pSphere1Follicle5359";
	setAttr -k off ".v";
	setAttr ".pu" 0.53125;
	setAttr ".pv" 0.59375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve130" -p "pSphere1Follicle5359";
createNode nurbsCurve -n "curveShape130" -p "curve130";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere1Follicle5365" -p "hairSystem1Follicles";
createNode follicle -n "pSphere1FollicleShape5365" -p "pSphere1Follicle5365";
	setAttr -k off ".v";
	setAttr ".pu" 0.53125;
	setAttr ".pv" 0.65625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve131" -p "pSphere1Follicle5365";
createNode nurbsCurve -n "curveShape131" -p "curve131";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere1Follicle5371" -p "hairSystem1Follicles";
createNode follicle -n "pSphere1FollicleShape5371" -p "pSphere1Follicle5371";
	setAttr -k off ".v";
	setAttr ".pu" 0.53125;
	setAttr ".pv" 0.71875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve132" -p "pSphere1Follicle5371";
createNode nurbsCurve -n "curveShape132" -p "curve132";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere1Follicle5377" -p "hairSystem1Follicles";
createNode follicle -n "pSphere1FollicleShape5377" -p "pSphere1Follicle5377";
	setAttr -k off ".v";
	setAttr ".pu" 0.53125;
	setAttr ".pv" 0.78125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve133" -p "pSphere1Follicle5377";
createNode nurbsCurve -n "curveShape133" -p "curve133";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere1Follicle5384" -p "hairSystem1Follicles";
createNode follicle -n "pSphere1FollicleShape5384" -p "pSphere1Follicle5384";
	setAttr -k off ".v";
	setAttr ".pu" 0.53125;
	setAttr ".pv" 0.84375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve134" -p "pSphere1Follicle5384";
createNode nurbsCurve -n "curveShape134" -p "curve134";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere1Follicle5390" -p "hairSystem1Follicles";
createNode follicle -n "pSphere1FollicleShape5390" -p "pSphere1Follicle5390";
	setAttr -k off ".v";
	setAttr ".pu" 0.53125;
	setAttr ".pv" 0.90625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve135" -p "pSphere1Follicle5390";
createNode nurbsCurve -n "curveShape135" -p "curve135";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere1Follicle5396" -p "hairSystem1Follicles";
createNode follicle -n "pSphere1FollicleShape5396" -p "pSphere1Follicle5396";
	setAttr -k off ".v";
	setAttr ".pu" 0.53125;
	setAttr ".pv" 0.96875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve136" -p "pSphere1Follicle5396";
createNode nurbsCurve -n "curveShape136" -p "curve136";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere1Follicle5909" -p "hairSystem1Follicles";
createNode follicle -n "pSphere1FollicleShape5909" -p "pSphere1Follicle5909";
	setAttr -k off ".v";
	setAttr ".pu" 0.59375;
	setAttr ".pv" 0.09375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve137" -p "pSphere1Follicle5909";
createNode nurbsCurve -n "curveShape137" -p "curve137";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere1Follicle5915" -p "hairSystem1Follicles";
createNode follicle -n "pSphere1FollicleShape5915" -p "pSphere1Follicle5915";
	setAttr -k off ".v";
	setAttr ".pu" 0.59375;
	setAttr ".pv" 0.15625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve138" -p "pSphere1Follicle5915";
createNode nurbsCurve -n "curveShape138" -p "curve138";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere1Follicle5922" -p "hairSystem1Follicles";
createNode follicle -n "pSphere1FollicleShape5922" -p "pSphere1Follicle5922";
	setAttr -k off ".v";
	setAttr ".pu" 0.59375;
	setAttr ".pv" 0.21875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve139" -p "pSphere1Follicle5922";
createNode nurbsCurve -n "curveShape139" -p "curve139";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere1Follicle5928" -p "hairSystem1Follicles";
createNode follicle -n "pSphere1FollicleShape5928" -p "pSphere1Follicle5928";
	setAttr -k off ".v";
	setAttr ".pu" 0.59375;
	setAttr ".pv" 0.28125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve140" -p "pSphere1Follicle5928";
createNode nurbsCurve -n "curveShape140" -p "curve140";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere1Follicle5934" -p "hairSystem1Follicles";
createNode follicle -n "pSphere1FollicleShape5934" -p "pSphere1Follicle5934";
	setAttr -k off ".v";
	setAttr ".pu" 0.59375;
	setAttr ".pv" 0.34375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve141" -p "pSphere1Follicle5934";
createNode nurbsCurve -n "curveShape141" -p "curve141";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere1Follicle5940" -p "hairSystem1Follicles";
createNode follicle -n "pSphere1FollicleShape5940" -p "pSphere1Follicle5940";
	setAttr -k off ".v";
	setAttr ".pu" 0.59375;
	setAttr ".pv" 0.40625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve142" -p "pSphere1Follicle5940";
createNode nurbsCurve -n "curveShape142" -p "curve142";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere1Follicle5946" -p "hairSystem1Follicles";
createNode follicle -n "pSphere1FollicleShape5946" -p "pSphere1Follicle5946";
	setAttr -k off ".v";
	setAttr ".pu" 0.59375;
	setAttr ".pv" 0.46875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve143" -p "pSphere1Follicle5946";
createNode nurbsCurve -n "curveShape143" -p "curve143";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere1Follicle5953" -p "hairSystem1Follicles";
createNode follicle -n "pSphere1FollicleShape5953" -p "pSphere1Follicle5953";
	setAttr -k off ".v";
	setAttr ".pu" 0.59375;
	setAttr ".pv" 0.53125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve144" -p "pSphere1Follicle5953";
createNode nurbsCurve -n "curveShape144" -p "curve144";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere1Follicle5959" -p "hairSystem1Follicles";
createNode follicle -n "pSphere1FollicleShape5959" -p "pSphere1Follicle5959";
	setAttr -k off ".v";
	setAttr ".pu" 0.59375;
	setAttr ".pv" 0.59375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve145" -p "pSphere1Follicle5959";
createNode nurbsCurve -n "curveShape145" -p "curve145";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere1Follicle5965" -p "hairSystem1Follicles";
createNode follicle -n "pSphere1FollicleShape5965" -p "pSphere1Follicle5965";
	setAttr -k off ".v";
	setAttr ".pu" 0.59375;
	setAttr ".pv" 0.65625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve146" -p "pSphere1Follicle5965";
createNode nurbsCurve -n "curveShape146" -p "curve146";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere1Follicle5971" -p "hairSystem1Follicles";
createNode follicle -n "pSphere1FollicleShape5971" -p "pSphere1Follicle5971";
	setAttr -k off ".v";
	setAttr ".pu" 0.59375;
	setAttr ".pv" 0.71875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve147" -p "pSphere1Follicle5971";
createNode nurbsCurve -n "curveShape147" -p "curve147";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere1Follicle5977" -p "hairSystem1Follicles";
createNode follicle -n "pSphere1FollicleShape5977" -p "pSphere1Follicle5977";
	setAttr -k off ".v";
	setAttr ".pu" 0.59375;
	setAttr ".pv" 0.78125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve148" -p "pSphere1Follicle5977";
createNode nurbsCurve -n "curveShape148" -p "curve148";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere1Follicle5984" -p "hairSystem1Follicles";
createNode follicle -n "pSphere1FollicleShape5984" -p "pSphere1Follicle5984";
	setAttr -k off ".v";
	setAttr ".pu" 0.59375;
	setAttr ".pv" 0.84375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve149" -p "pSphere1Follicle5984";
createNode nurbsCurve -n "curveShape149" -p "curve149";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere1Follicle5990" -p "hairSystem1Follicles";
createNode follicle -n "pSphere1FollicleShape5990" -p "pSphere1Follicle5990";
	setAttr -k off ".v";
	setAttr ".pu" 0.59375;
	setAttr ".pv" 0.90625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve150" -p "pSphere1Follicle5990";
createNode nurbsCurve -n "curveShape150" -p "curve150";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere1Follicle6509" -p "hairSystem1Follicles";
createNode follicle -n "pSphere1FollicleShape6509" -p "pSphere1Follicle6509";
	setAttr -k off ".v";
	setAttr ".pu" 0.65625;
	setAttr ".pv" 0.09375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve151" -p "pSphere1Follicle6509";
createNode nurbsCurve -n "curveShape151" -p "curve151";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere1Follicle6515" -p "hairSystem1Follicles";
createNode follicle -n "pSphere1FollicleShape6515" -p "pSphere1Follicle6515";
	setAttr -k off ".v";
	setAttr ".pu" 0.65625;
	setAttr ".pv" 0.15625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve152" -p "pSphere1Follicle6515";
createNode nurbsCurve -n "curveShape152" -p "curve152";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere1Follicle6522" -p "hairSystem1Follicles";
createNode follicle -n "pSphere1FollicleShape6522" -p "pSphere1Follicle6522";
	setAttr -k off ".v";
	setAttr ".pu" 0.65625;
	setAttr ".pv" 0.21875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve153" -p "pSphere1Follicle6522";
createNode nurbsCurve -n "curveShape153" -p "curve153";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere1Follicle6528" -p "hairSystem1Follicles";
createNode follicle -n "pSphere1FollicleShape6528" -p "pSphere1Follicle6528";
	setAttr -k off ".v";
	setAttr ".pu" 0.65625;
	setAttr ".pv" 0.28125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve154" -p "pSphere1Follicle6528";
createNode nurbsCurve -n "curveShape154" -p "curve154";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere1Follicle6534" -p "hairSystem1Follicles";
createNode follicle -n "pSphere1FollicleShape6534" -p "pSphere1Follicle6534";
	setAttr -k off ".v";
	setAttr ".pu" 0.65625;
	setAttr ".pv" 0.34375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve155" -p "pSphere1Follicle6534";
createNode nurbsCurve -n "curveShape155" -p "curve155";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere1Follicle6540" -p "hairSystem1Follicles";
createNode follicle -n "pSphere1FollicleShape6540" -p "pSphere1Follicle6540";
	setAttr -k off ".v";
	setAttr ".pu" 0.65625;
	setAttr ".pv" 0.40625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve156" -p "pSphere1Follicle6540";
createNode nurbsCurve -n "curveShape156" -p "curve156";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere1Follicle6546" -p "hairSystem1Follicles";
createNode follicle -n "pSphere1FollicleShape6546" -p "pSphere1Follicle6546";
	setAttr -k off ".v";
	setAttr ".pu" 0.65625;
	setAttr ".pv" 0.46875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve157" -p "pSphere1Follicle6546";
createNode nurbsCurve -n "curveShape157" -p "curve157";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere1Follicle6553" -p "hairSystem1Follicles";
createNode follicle -n "pSphere1FollicleShape6553" -p "pSphere1Follicle6553";
	setAttr -k off ".v";
	setAttr ".pu" 0.65625;
	setAttr ".pv" 0.53125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve158" -p "pSphere1Follicle6553";
createNode nurbsCurve -n "curveShape158" -p "curve158";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere1Follicle6559" -p "hairSystem1Follicles";
createNode follicle -n "pSphere1FollicleShape6559" -p "pSphere1Follicle6559";
	setAttr -k off ".v";
	setAttr ".pu" 0.65625;
	setAttr ".pv" 0.59375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve159" -p "pSphere1Follicle6559";
createNode nurbsCurve -n "curveShape159" -p "curve159";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere1Follicle6565" -p "hairSystem1Follicles";
createNode follicle -n "pSphere1FollicleShape6565" -p "pSphere1Follicle6565";
	setAttr -k off ".v";
	setAttr ".pu" 0.65625;
	setAttr ".pv" 0.65625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve160" -p "pSphere1Follicle6565";
createNode nurbsCurve -n "curveShape160" -p "curve160";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere1Follicle6571" -p "hairSystem1Follicles";
createNode follicle -n "pSphere1FollicleShape6571" -p "pSphere1Follicle6571";
	setAttr -k off ".v";
	setAttr ".pu" 0.65625;
	setAttr ".pv" 0.71875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve161" -p "pSphere1Follicle6571";
createNode nurbsCurve -n "curveShape161" -p "curve161";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere1Follicle6577" -p "hairSystem1Follicles";
createNode follicle -n "pSphere1FollicleShape6577" -p "pSphere1Follicle6577";
	setAttr -k off ".v";
	setAttr ".pu" 0.65625;
	setAttr ".pv" 0.78125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve162" -p "pSphere1Follicle6577";
createNode nurbsCurve -n "curveShape162" -p "curve162";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere1Follicle6584" -p "hairSystem1Follicles";
createNode follicle -n "pSphere1FollicleShape6584" -p "pSphere1Follicle6584";
	setAttr -k off ".v";
	setAttr ".pu" 0.65625;
	setAttr ".pv" 0.84375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve163" -p "pSphere1Follicle6584";
createNode nurbsCurve -n "curveShape163" -p "curve163";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere1Follicle6590" -p "hairSystem1Follicles";
createNode follicle -n "pSphere1FollicleShape6590" -p "pSphere1Follicle6590";
	setAttr -k off ".v";
	setAttr ".pu" 0.65625;
	setAttr ".pv" 0.90625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve164" -p "pSphere1Follicle6590";
createNode nurbsCurve -n "curveShape164" -p "curve164";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere1Follicle7103" -p "hairSystem1Follicles";
createNode follicle -n "pSphere1FollicleShape7103" -p "pSphere1Follicle7103";
	setAttr -k off ".v";
	setAttr ".pu" 0.71875;
	setAttr ".pv" 0.03125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve165" -p "pSphere1Follicle7103";
createNode nurbsCurve -n "curveShape165" -p "curve165";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere1Follicle7109" -p "hairSystem1Follicles";
createNode follicle -n "pSphere1FollicleShape7109" -p "pSphere1Follicle7109";
	setAttr -k off ".v";
	setAttr ".pu" 0.71875;
	setAttr ".pv" 0.09375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve166" -p "pSphere1Follicle7109";
createNode nurbsCurve -n "curveShape166" -p "curve166";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere1Follicle7115" -p "hairSystem1Follicles";
createNode follicle -n "pSphere1FollicleShape7115" -p "pSphere1Follicle7115";
	setAttr -k off ".v";
	setAttr ".pu" 0.71875;
	setAttr ".pv" 0.15625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve167" -p "pSphere1Follicle7115";
createNode nurbsCurve -n "curveShape167" -p "curve167";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere1Follicle7122" -p "hairSystem1Follicles";
createNode follicle -n "pSphere1FollicleShape7122" -p "pSphere1Follicle7122";
	setAttr -k off ".v";
	setAttr ".pu" 0.71875;
	setAttr ".pv" 0.21875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve168" -p "pSphere1Follicle7122";
createNode nurbsCurve -n "curveShape168" -p "curve168";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere1Follicle7128" -p "hairSystem1Follicles";
createNode follicle -n "pSphere1FollicleShape7128" -p "pSphere1Follicle7128";
	setAttr -k off ".v";
	setAttr ".pu" 0.71875;
	setAttr ".pv" 0.28125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve169" -p "pSphere1Follicle7128";
createNode nurbsCurve -n "curveShape169" -p "curve169";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere1Follicle7134" -p "hairSystem1Follicles";
createNode follicle -n "pSphere1FollicleShape7134" -p "pSphere1Follicle7134";
	setAttr -k off ".v";
	setAttr ".pu" 0.71875;
	setAttr ".pv" 0.34375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve170" -p "pSphere1Follicle7134";
createNode nurbsCurve -n "curveShape170" -p "curve170";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere1Follicle7140" -p "hairSystem1Follicles";
createNode follicle -n "pSphere1FollicleShape7140" -p "pSphere1Follicle7140";
	setAttr -k off ".v";
	setAttr ".pu" 0.71875;
	setAttr ".pv" 0.40625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve171" -p "pSphere1Follicle7140";
createNode nurbsCurve -n "curveShape171" -p "curve171";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere1Follicle7146" -p "hairSystem1Follicles";
createNode follicle -n "pSphere1FollicleShape7146" -p "pSphere1Follicle7146";
	setAttr -k off ".v";
	setAttr ".pu" 0.71875;
	setAttr ".pv" 0.46875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve172" -p "pSphere1Follicle7146";
createNode nurbsCurve -n "curveShape172" -p "curve172";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere1Follicle7153" -p "hairSystem1Follicles";
createNode follicle -n "pSphere1FollicleShape7153" -p "pSphere1Follicle7153";
	setAttr -k off ".v";
	setAttr ".pu" 0.71875;
	setAttr ".pv" 0.53125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve173" -p "pSphere1Follicle7153";
createNode nurbsCurve -n "curveShape173" -p "curve173";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere1Follicle7159" -p "hairSystem1Follicles";
createNode follicle -n "pSphere1FollicleShape7159" -p "pSphere1Follicle7159";
	setAttr -k off ".v";
	setAttr ".pu" 0.71875;
	setAttr ".pv" 0.59375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve174" -p "pSphere1Follicle7159";
createNode nurbsCurve -n "curveShape174" -p "curve174";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere1Follicle7165" -p "hairSystem1Follicles";
createNode follicle -n "pSphere1FollicleShape7165" -p "pSphere1Follicle7165";
	setAttr -k off ".v";
	setAttr ".pu" 0.71875;
	setAttr ".pv" 0.65625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve175" -p "pSphere1Follicle7165";
createNode nurbsCurve -n "curveShape175" -p "curve175";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere1Follicle7171" -p "hairSystem1Follicles";
createNode follicle -n "pSphere1FollicleShape7171" -p "pSphere1Follicle7171";
	setAttr -k off ".v";
	setAttr ".pu" 0.71875;
	setAttr ".pv" 0.71875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve176" -p "pSphere1Follicle7171";
createNode nurbsCurve -n "curveShape176" -p "curve176";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere1Follicle7177" -p "hairSystem1Follicles";
createNode follicle -n "pSphere1FollicleShape7177" -p "pSphere1Follicle7177";
	setAttr -k off ".v";
	setAttr ".pu" 0.71875;
	setAttr ".pv" 0.78125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve177" -p "pSphere1Follicle7177";
createNode nurbsCurve -n "curveShape177" -p "curve177";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere1Follicle7184" -p "hairSystem1Follicles";
createNode follicle -n "pSphere1FollicleShape7184" -p "pSphere1Follicle7184";
	setAttr -k off ".v";
	setAttr ".pu" 0.71875;
	setAttr ".pv" 0.84375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve178" -p "pSphere1Follicle7184";
createNode nurbsCurve -n "curveShape178" -p "curve178";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere1Follicle7190" -p "hairSystem1Follicles";
createNode follicle -n "pSphere1FollicleShape7190" -p "pSphere1Follicle7190";
	setAttr -k off ".v";
	setAttr ".pu" 0.71875;
	setAttr ".pv" 0.90625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve179" -p "pSphere1Follicle7190";
createNode nurbsCurve -n "curveShape179" -p "curve179";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere1Follicle7196" -p "hairSystem1Follicles";
createNode follicle -n "pSphere1FollicleShape7196" -p "pSphere1Follicle7196";
	setAttr -k off ".v";
	setAttr ".pu" 0.71875;
	setAttr ".pv" 0.96875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve180" -p "pSphere1Follicle7196";
createNode nurbsCurve -n "curveShape180" -p "curve180";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere1Follicle7703" -p "hairSystem1Follicles";
createNode follicle -n "pSphere1FollicleShape7703" -p "pSphere1Follicle7703";
	setAttr -k off ".v";
	setAttr ".pu" 0.78125;
	setAttr ".pv" 0.03125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve181" -p "pSphere1Follicle7703";
createNode nurbsCurve -n "curveShape181" -p "curve181";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere1Follicle7709" -p "hairSystem1Follicles";
createNode follicle -n "pSphere1FollicleShape7709" -p "pSphere1Follicle7709";
	setAttr -k off ".v";
	setAttr ".pu" 0.78125;
	setAttr ".pv" 0.09375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve182" -p "pSphere1Follicle7709";
createNode nurbsCurve -n "curveShape182" -p "curve182";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere1Follicle7715" -p "hairSystem1Follicles";
createNode follicle -n "pSphere1FollicleShape7715" -p "pSphere1Follicle7715";
	setAttr -k off ".v";
	setAttr ".pu" 0.78125;
	setAttr ".pv" 0.15625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve183" -p "pSphere1Follicle7715";
createNode nurbsCurve -n "curveShape183" -p "curve183";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere1Follicle7722" -p "hairSystem1Follicles";
createNode follicle -n "pSphere1FollicleShape7722" -p "pSphere1Follicle7722";
	setAttr -k off ".v";
	setAttr ".pu" 0.78125;
	setAttr ".pv" 0.21875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve184" -p "pSphere1Follicle7722";
createNode nurbsCurve -n "curveShape184" -p "curve184";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere1Follicle7728" -p "hairSystem1Follicles";
createNode follicle -n "pSphere1FollicleShape7728" -p "pSphere1Follicle7728";
	setAttr -k off ".v";
	setAttr ".pu" 0.78125;
	setAttr ".pv" 0.28125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve185" -p "pSphere1Follicle7728";
createNode nurbsCurve -n "curveShape185" -p "curve185";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere1Follicle7734" -p "hairSystem1Follicles";
createNode follicle -n "pSphere1FollicleShape7734" -p "pSphere1Follicle7734";
	setAttr -k off ".v";
	setAttr ".pu" 0.78125;
	setAttr ".pv" 0.34375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve186" -p "pSphere1Follicle7734";
createNode nurbsCurve -n "curveShape186" -p "curve186";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere1Follicle7740" -p "hairSystem1Follicles";
createNode follicle -n "pSphere1FollicleShape7740" -p "pSphere1Follicle7740";
	setAttr -k off ".v";
	setAttr ".pu" 0.78125;
	setAttr ".pv" 0.40625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve187" -p "pSphere1Follicle7740";
createNode nurbsCurve -n "curveShape187" -p "curve187";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere1Follicle7746" -p "hairSystem1Follicles";
createNode follicle -n "pSphere1FollicleShape7746" -p "pSphere1Follicle7746";
	setAttr -k off ".v";
	setAttr ".pu" 0.78125;
	setAttr ".pv" 0.46875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve188" -p "pSphere1Follicle7746";
createNode nurbsCurve -n "curveShape188" -p "curve188";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere1Follicle7753" -p "hairSystem1Follicles";
createNode follicle -n "pSphere1FollicleShape7753" -p "pSphere1Follicle7753";
	setAttr -k off ".v";
	setAttr ".pu" 0.78125;
	setAttr ".pv" 0.53125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve189" -p "pSphere1Follicle7753";
createNode nurbsCurve -n "curveShape189" -p "curve189";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere1Follicle7759" -p "hairSystem1Follicles";
createNode follicle -n "pSphere1FollicleShape7759" -p "pSphere1Follicle7759";
	setAttr -k off ".v";
	setAttr ".pu" 0.78125;
	setAttr ".pv" 0.59375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve190" -p "pSphere1Follicle7759";
createNode nurbsCurve -n "curveShape190" -p "curve190";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere1Follicle7765" -p "hairSystem1Follicles";
createNode follicle -n "pSphere1FollicleShape7765" -p "pSphere1Follicle7765";
	setAttr -k off ".v";
	setAttr ".pu" 0.78125;
	setAttr ".pv" 0.65625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve191" -p "pSphere1Follicle7765";
createNode nurbsCurve -n "curveShape191" -p "curve191";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere1Follicle7771" -p "hairSystem1Follicles";
createNode follicle -n "pSphere1FollicleShape7771" -p "pSphere1Follicle7771";
	setAttr -k off ".v";
	setAttr ".pu" 0.78125;
	setAttr ".pv" 0.71875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve192" -p "pSphere1Follicle7771";
createNode nurbsCurve -n "curveShape192" -p "curve192";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere1Follicle7777" -p "hairSystem1Follicles";
createNode follicle -n "pSphere1FollicleShape7777" -p "pSphere1Follicle7777";
	setAttr -k off ".v";
	setAttr ".pu" 0.78125;
	setAttr ".pv" 0.78125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve193" -p "pSphere1Follicle7777";
createNode nurbsCurve -n "curveShape193" -p "curve193";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere1Follicle7784" -p "hairSystem1Follicles";
createNode follicle -n "pSphere1FollicleShape7784" -p "pSphere1Follicle7784";
	setAttr -k off ".v";
	setAttr ".pu" 0.78125;
	setAttr ".pv" 0.84375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve194" -p "pSphere1Follicle7784";
createNode nurbsCurve -n "curveShape194" -p "curve194";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere1Follicle7790" -p "hairSystem1Follicles";
createNode follicle -n "pSphere1FollicleShape7790" -p "pSphere1Follicle7790";
	setAttr -k off ".v";
	setAttr ".pu" 0.78125;
	setAttr ".pv" 0.90625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve195" -p "pSphere1Follicle7790";
createNode nurbsCurve -n "curveShape195" -p "curve195";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere1Follicle7796" -p "hairSystem1Follicles";
createNode follicle -n "pSphere1FollicleShape7796" -p "pSphere1Follicle7796";
	setAttr -k off ".v";
	setAttr ".pu" 0.78125;
	setAttr ".pv" 0.96875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve196" -p "pSphere1Follicle7796";
createNode nurbsCurve -n "curveShape196" -p "curve196";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere1Follicle8409" -p "hairSystem1Follicles";
createNode follicle -n "pSphere1FollicleShape8409" -p "pSphere1Follicle8409";
	setAttr -k off ".v";
	setAttr ".pu" 0.84375;
	setAttr ".pv" 0.09375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve197" -p "pSphere1Follicle8409";
createNode nurbsCurve -n "curveShape197" -p "curve197";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere1Follicle8415" -p "hairSystem1Follicles";
createNode follicle -n "pSphere1FollicleShape8415" -p "pSphere1Follicle8415";
	setAttr -k off ".v";
	setAttr ".pu" 0.84375;
	setAttr ".pv" 0.15625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve198" -p "pSphere1Follicle8415";
createNode nurbsCurve -n "curveShape198" -p "curve198";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere1Follicle8422" -p "hairSystem1Follicles";
createNode follicle -n "pSphere1FollicleShape8422" -p "pSphere1Follicle8422";
	setAttr -k off ".v";
	setAttr ".pu" 0.84375;
	setAttr ".pv" 0.21875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve199" -p "pSphere1Follicle8422";
createNode nurbsCurve -n "curveShape199" -p "curve199";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere1Follicle8428" -p "hairSystem1Follicles";
createNode follicle -n "pSphere1FollicleShape8428" -p "pSphere1Follicle8428";
	setAttr -k off ".v";
	setAttr ".pu" 0.84375;
	setAttr ".pv" 0.28125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve200" -p "pSphere1Follicle8428";
createNode nurbsCurve -n "curveShape200" -p "curve200";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere1Follicle8434" -p "hairSystem1Follicles";
createNode follicle -n "pSphere1FollicleShape8434" -p "pSphere1Follicle8434";
	setAttr -k off ".v";
	setAttr ".pu" 0.84375;
	setAttr ".pv" 0.34375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve201" -p "pSphere1Follicle8434";
createNode nurbsCurve -n "curveShape201" -p "curve201";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere1Follicle8440" -p "hairSystem1Follicles";
createNode follicle -n "pSphere1FollicleShape8440" -p "pSphere1Follicle8440";
	setAttr -k off ".v";
	setAttr ".pu" 0.84375;
	setAttr ".pv" 0.40625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve202" -p "pSphere1Follicle8440";
createNode nurbsCurve -n "curveShape202" -p "curve202";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere1Follicle8446" -p "hairSystem1Follicles";
createNode follicle -n "pSphere1FollicleShape8446" -p "pSphere1Follicle8446";
	setAttr -k off ".v";
	setAttr ".pu" 0.84375;
	setAttr ".pv" 0.46875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve203" -p "pSphere1Follicle8446";
createNode nurbsCurve -n "curveShape203" -p "curve203";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere1Follicle8453" -p "hairSystem1Follicles";
createNode follicle -n "pSphere1FollicleShape8453" -p "pSphere1Follicle8453";
	setAttr -k off ".v";
	setAttr ".pu" 0.84375;
	setAttr ".pv" 0.53125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve204" -p "pSphere1Follicle8453";
createNode nurbsCurve -n "curveShape204" -p "curve204";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere1Follicle8459" -p "hairSystem1Follicles";
createNode follicle -n "pSphere1FollicleShape8459" -p "pSphere1Follicle8459";
	setAttr -k off ".v";
	setAttr ".pu" 0.84375;
	setAttr ".pv" 0.59375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve205" -p "pSphere1Follicle8459";
createNode nurbsCurve -n "curveShape205" -p "curve205";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere1Follicle8465" -p "hairSystem1Follicles";
createNode follicle -n "pSphere1FollicleShape8465" -p "pSphere1Follicle8465";
	setAttr -k off ".v";
	setAttr ".pu" 0.84375;
	setAttr ".pv" 0.65625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve206" -p "pSphere1Follicle8465";
createNode nurbsCurve -n "curveShape206" -p "curve206";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere1Follicle8471" -p "hairSystem1Follicles";
createNode follicle -n "pSphere1FollicleShape8471" -p "pSphere1Follicle8471";
	setAttr -k off ".v";
	setAttr ".pu" 0.84375;
	setAttr ".pv" 0.71875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve207" -p "pSphere1Follicle8471";
createNode nurbsCurve -n "curveShape207" -p "curve207";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere1Follicle8477" -p "hairSystem1Follicles";
createNode follicle -n "pSphere1FollicleShape8477" -p "pSphere1Follicle8477";
	setAttr -k off ".v";
	setAttr ".pu" 0.84375;
	setAttr ".pv" 0.78125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve208" -p "pSphere1Follicle8477";
createNode nurbsCurve -n "curveShape208" -p "curve208";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere1Follicle8484" -p "hairSystem1Follicles";
createNode follicle -n "pSphere1FollicleShape8484" -p "pSphere1Follicle8484";
	setAttr -k off ".v";
	setAttr ".pu" 0.84375;
	setAttr ".pv" 0.84375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve209" -p "pSphere1Follicle8484";
createNode nurbsCurve -n "curveShape209" -p "curve209";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere1Follicle8490" -p "hairSystem1Follicles";
createNode follicle -n "pSphere1FollicleShape8490" -p "pSphere1Follicle8490";
	setAttr -k off ".v";
	setAttr ".pu" 0.84375;
	setAttr ".pv" 0.90625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve210" -p "pSphere1Follicle8490";
createNode nurbsCurve -n "curveShape210" -p "curve210";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere1Follicle9009" -p "hairSystem1Follicles";
createNode follicle -n "pSphere1FollicleShape9009" -p "pSphere1Follicle9009";
	setAttr -k off ".v";
	setAttr ".pu" 0.90625;
	setAttr ".pv" 0.09375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve211" -p "pSphere1Follicle9009";
createNode nurbsCurve -n "curveShape211" -p "curve211";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere1Follicle9015" -p "hairSystem1Follicles";
createNode follicle -n "pSphere1FollicleShape9015" -p "pSphere1Follicle9015";
	setAttr -k off ".v";
	setAttr ".pu" 0.90625;
	setAttr ".pv" 0.15625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve212" -p "pSphere1Follicle9015";
createNode nurbsCurve -n "curveShape212" -p "curve212";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere1Follicle9022" -p "hairSystem1Follicles";
createNode follicle -n "pSphere1FollicleShape9022" -p "pSphere1Follicle9022";
	setAttr -k off ".v";
	setAttr ".pu" 0.90625;
	setAttr ".pv" 0.21875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve213" -p "pSphere1Follicle9022";
createNode nurbsCurve -n "curveShape213" -p "curve213";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere1Follicle9028" -p "hairSystem1Follicles";
createNode follicle -n "pSphere1FollicleShape9028" -p "pSphere1Follicle9028";
	setAttr -k off ".v";
	setAttr ".pu" 0.90625;
	setAttr ".pv" 0.28125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve214" -p "pSphere1Follicle9028";
createNode nurbsCurve -n "curveShape214" -p "curve214";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere1Follicle9034" -p "hairSystem1Follicles";
createNode follicle -n "pSphere1FollicleShape9034" -p "pSphere1Follicle9034";
	setAttr -k off ".v";
	setAttr ".pu" 0.90625;
	setAttr ".pv" 0.34375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve215" -p "pSphere1Follicle9034";
createNode nurbsCurve -n "curveShape215" -p "curve215";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere1Follicle9040" -p "hairSystem1Follicles";
createNode follicle -n "pSphere1FollicleShape9040" -p "pSphere1Follicle9040";
	setAttr -k off ".v";
	setAttr ".pu" 0.90625;
	setAttr ".pv" 0.40625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve216" -p "pSphere1Follicle9040";
createNode nurbsCurve -n "curveShape216" -p "curve216";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere1Follicle9046" -p "hairSystem1Follicles";
createNode follicle -n "pSphere1FollicleShape9046" -p "pSphere1Follicle9046";
	setAttr -k off ".v";
	setAttr ".pu" 0.90625;
	setAttr ".pv" 0.46875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve217" -p "pSphere1Follicle9046";
createNode nurbsCurve -n "curveShape217" -p "curve217";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere1Follicle9053" -p "hairSystem1Follicles";
createNode follicle -n "pSphere1FollicleShape9053" -p "pSphere1Follicle9053";
	setAttr -k off ".v";
	setAttr ".pu" 0.90625;
	setAttr ".pv" 0.53125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve218" -p "pSphere1Follicle9053";
createNode nurbsCurve -n "curveShape218" -p "curve218";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere1Follicle9059" -p "hairSystem1Follicles";
createNode follicle -n "pSphere1FollicleShape9059" -p "pSphere1Follicle9059";
	setAttr -k off ".v";
	setAttr ".pu" 0.90625;
	setAttr ".pv" 0.59375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve219" -p "pSphere1Follicle9059";
createNode nurbsCurve -n "curveShape219" -p "curve219";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere1Follicle9065" -p "hairSystem1Follicles";
createNode follicle -n "pSphere1FollicleShape9065" -p "pSphere1Follicle9065";
	setAttr -k off ".v";
	setAttr ".pu" 0.90625;
	setAttr ".pv" 0.65625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve220" -p "pSphere1Follicle9065";
createNode nurbsCurve -n "curveShape220" -p "curve220";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere1Follicle9071" -p "hairSystem1Follicles";
createNode follicle -n "pSphere1FollicleShape9071" -p "pSphere1Follicle9071";
	setAttr -k off ".v";
	setAttr ".pu" 0.90625;
	setAttr ".pv" 0.71875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve221" -p "pSphere1Follicle9071";
createNode nurbsCurve -n "curveShape221" -p "curve221";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere1Follicle9077" -p "hairSystem1Follicles";
createNode follicle -n "pSphere1FollicleShape9077" -p "pSphere1Follicle9077";
	setAttr -k off ".v";
	setAttr ".pu" 0.90625;
	setAttr ".pv" 0.78125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve222" -p "pSphere1Follicle9077";
createNode nurbsCurve -n "curveShape222" -p "curve222";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere1Follicle9084" -p "hairSystem1Follicles";
createNode follicle -n "pSphere1FollicleShape9084" -p "pSphere1Follicle9084";
	setAttr -k off ".v";
	setAttr ".pu" 0.90625;
	setAttr ".pv" 0.84375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve223" -p "pSphere1Follicle9084";
createNode nurbsCurve -n "curveShape223" -p "curve223";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere1Follicle9090" -p "hairSystem1Follicles";
createNode follicle -n "pSphere1FollicleShape9090" -p "pSphere1Follicle9090";
	setAttr -k off ".v";
	setAttr ".pu" 0.90625;
	setAttr ".pv" 0.90625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve224" -p "pSphere1Follicle9090";
createNode nurbsCurve -n "curveShape224" -p "curve224";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere1Follicle9603" -p "hairSystem1Follicles";
createNode follicle -n "pSphere1FollicleShape9603" -p "pSphere1Follicle9603";
	setAttr -k off ".v";
	setAttr ".pu" 0.96875;
	setAttr ".pv" 0.03125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve225" -p "pSphere1Follicle9603";
createNode nurbsCurve -n "curveShape225" -p "curve225";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere1Follicle9609" -p "hairSystem1Follicles";
createNode follicle -n "pSphere1FollicleShape9609" -p "pSphere1Follicle9609";
	setAttr -k off ".v";
	setAttr ".pu" 0.96875;
	setAttr ".pv" 0.09375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve226" -p "pSphere1Follicle9609";
createNode nurbsCurve -n "curveShape226" -p "curve226";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere1Follicle9615" -p "hairSystem1Follicles";
createNode follicle -n "pSphere1FollicleShape9615" -p "pSphere1Follicle9615";
	setAttr -k off ".v";
	setAttr ".pu" 0.96875;
	setAttr ".pv" 0.15625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve227" -p "pSphere1Follicle9615";
createNode nurbsCurve -n "curveShape227" -p "curve227";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere1Follicle9622" -p "hairSystem1Follicles";
createNode follicle -n "pSphere1FollicleShape9622" -p "pSphere1Follicle9622";
	setAttr -k off ".v";
	setAttr ".pu" 0.96875;
	setAttr ".pv" 0.21875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve228" -p "pSphere1Follicle9622";
createNode nurbsCurve -n "curveShape228" -p "curve228";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere1Follicle9628" -p "hairSystem1Follicles";
createNode follicle -n "pSphere1FollicleShape9628" -p "pSphere1Follicle9628";
	setAttr -k off ".v";
	setAttr ".pu" 0.96875;
	setAttr ".pv" 0.28125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve229" -p "pSphere1Follicle9628";
createNode nurbsCurve -n "curveShape229" -p "curve229";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere1Follicle9634" -p "hairSystem1Follicles";
createNode follicle -n "pSphere1FollicleShape9634" -p "pSphere1Follicle9634";
	setAttr -k off ".v";
	setAttr ".pu" 0.96875;
	setAttr ".pv" 0.34375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve230" -p "pSphere1Follicle9634";
createNode nurbsCurve -n "curveShape230" -p "curve230";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere1Follicle9640" -p "hairSystem1Follicles";
createNode follicle -n "pSphere1FollicleShape9640" -p "pSphere1Follicle9640";
	setAttr -k off ".v";
	setAttr ".pu" 0.96875;
	setAttr ".pv" 0.40625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve231" -p "pSphere1Follicle9640";
createNode nurbsCurve -n "curveShape231" -p "curve231";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere1Follicle9646" -p "hairSystem1Follicles";
createNode follicle -n "pSphere1FollicleShape9646" -p "pSphere1Follicle9646";
	setAttr -k off ".v";
	setAttr ".pu" 0.96875;
	setAttr ".pv" 0.46875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve232" -p "pSphere1Follicle9646";
createNode nurbsCurve -n "curveShape232" -p "curve232";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere1Follicle9653" -p "hairSystem1Follicles";
createNode follicle -n "pSphere1FollicleShape9653" -p "pSphere1Follicle9653";
	setAttr -k off ".v";
	setAttr ".pu" 0.96875;
	setAttr ".pv" 0.53125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve233" -p "pSphere1Follicle9653";
createNode nurbsCurve -n "curveShape233" -p "curve233";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere1Follicle9659" -p "hairSystem1Follicles";
createNode follicle -n "pSphere1FollicleShape9659" -p "pSphere1Follicle9659";
	setAttr -k off ".v";
	setAttr ".pu" 0.96875;
	setAttr ".pv" 0.59375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve234" -p "pSphere1Follicle9659";
createNode nurbsCurve -n "curveShape234" -p "curve234";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere1Follicle9665" -p "hairSystem1Follicles";
createNode follicle -n "pSphere1FollicleShape9665" -p "pSphere1Follicle9665";
	setAttr -k off ".v";
	setAttr ".pu" 0.96875;
	setAttr ".pv" 0.65625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve235" -p "pSphere1Follicle9665";
createNode nurbsCurve -n "curveShape235" -p "curve235";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere1Follicle9671" -p "hairSystem1Follicles";
createNode follicle -n "pSphere1FollicleShape9671" -p "pSphere1Follicle9671";
	setAttr -k off ".v";
	setAttr ".pu" 0.96875;
	setAttr ".pv" 0.71875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve236" -p "pSphere1Follicle9671";
createNode nurbsCurve -n "curveShape236" -p "curve236";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere1Follicle9677" -p "hairSystem1Follicles";
createNode follicle -n "pSphere1FollicleShape9677" -p "pSphere1Follicle9677";
	setAttr -k off ".v";
	setAttr ".pu" 0.96875;
	setAttr ".pv" 0.78125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve237" -p "pSphere1Follicle9677";
createNode nurbsCurve -n "curveShape237" -p "curve237";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere1Follicle9684" -p "hairSystem1Follicles";
createNode follicle -n "pSphere1FollicleShape9684" -p "pSphere1Follicle9684";
	setAttr -k off ".v";
	setAttr ".pu" 0.96875;
	setAttr ".pv" 0.84375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve238" -p "pSphere1Follicle9684";
createNode nurbsCurve -n "curveShape238" -p "curve238";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere1Follicle9690" -p "hairSystem1Follicles";
createNode follicle -n "pSphere1FollicleShape9690" -p "pSphere1Follicle9690";
	setAttr -k off ".v";
	setAttr ".pu" 0.96875;
	setAttr ".pv" 0.90625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve239" -p "pSphere1Follicle9690";
createNode nurbsCurve -n "curveShape239" -p "curve239";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere1Follicle9696" -p "hairSystem1Follicles";
createNode follicle -n "pSphere1FollicleShape9696" -p "pSphere1Follicle9696";
	setAttr -k off ".v";
	setAttr ".pu" 0.96875;
	setAttr ".pv" 0.96875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve240" -p "pSphere1Follicle9696";
createNode nurbsCurve -n "curveShape240" -p "curve240";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pfxHair1";
createNode pfxHair -n "pfxHairShape1" -p "pfxHair1";
	setAttr -k off ".v";
	setAttr ".dpc" 100;
	setAttr ".dam" no;
createNode transform -n "hairSystem2";
createNode hairSystem -n "hairSystemShape2" -p "hairSystem2";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	addAttr -ci true -sn "ai_self_shadows" -ln "aiSelfShadows" -dv 1 -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "ai_opaque" -ln "aiOpaque" -dv 1 -min 0 -max 1 -at "bool";
	addAttr -ci true -k true -sn "ai_vid" -ln "aiVisibleInDiffuse" -dv 1 -min 0 -max 
		1 -at "bool";
	addAttr -ci true -k true -sn "ai_vig" -ln "aiVisibleInGlossy" -dv 1 -min 0 -max 
		1 -at "bool";
	addAttr -ci true -sn "ai_min_pixel_width" -ln "aiMinPixelWidth" -at "float";
	addAttr -ci true -sn "ai_mode" -ln "aiMode" -min 0 -max 2 -en "ribbon:thick:oriented" 
		-at "enum";
	addAttr -ci true -k true -sn "vis" -ln "primaryVisibility" -dv 1 -min 0 -max 1 -at "bool";
	addAttr -ci true -k true -sn "csh" -ln "castsShadows" -dv 1 -min 0 -max 1 -at "bool";
	addAttr -ci true -k true -sn "ai_export_hair_ids" -ln "aiExportHairIDs" -dv 1 -min 
		0 -max 1 -at "bool";
	addAttr -ci true -k true -sn "ai_export_hair_uvs" -ln "aiExportHairUVs" -min 0 -max 
		1 -at "bool";
	addAttr -ci true -k true -sn "ai_export_hair_colors" -ln "aiExportHairColors" -min 
		0 -max 1 -at "bool";
	addAttr -ci true -k true -sn "ai_override_hair" -ln "aiOverrideHair" -min 0 -max 
		1 -at "bool";
	addAttr -ci true -k true -sn "ai_hair_shader" -ln "aiHairShader" -at "message";
	addAttr -ci true -sn "mso" -ln "miShadingSamplesOverride" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "msh" -ln "miShadingSamples" -min 0 -smx 8 -at "float";
	addAttr -ci true -sn "mdo" -ln "miMaxDisplaceOverride" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "mmd" -ln "miMaxDisplace" -min 0 -smx 1 -at "float";
	setAttr -k off ".v";
	setAttr -s 240 ".ih";
	setAttr ".evo" 0;
	setAttr ".sfn" 0.1;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr ".clc[0]"  0 0.5 1;
	setAttr ".cfl[0]"  0 0 1;
	setAttr -s 2 ".hws[0:1]"  0.80000001 1 3 1 0.2 3;
	setAttr -s 3 ".hcs";
	setAttr ".hcs[0].hcsp" 0;
	setAttr ".hcs[0].hcsc" -type "float3" 0.5 0.5 0.5 ;
	setAttr ".hcs[0].hcsi" 3;
	setAttr ".hcs[1].hcsp" 0.30000001192092896;
	setAttr ".hcs[1].hcsc" -type "float3" 0.80000001 0.80000001 0.80000001 ;
	setAttr ".hcs[1].hcsi" 3;
	setAttr ".hcs[2].hcsp" 1;
	setAttr ".hcs[2].hcsc" -type "float3" 1 1 1 ;
	setAttr ".hcs[2].hcsi" 3;
	setAttr ".dsc[0]"  0 1 1;
	setAttr ".ai_mode" 1;
	setAttr -k on ".ai_override_hair" yes;
createNode transform -n "hairSystem2Follicles";
createNode transform -n "pSphere2Follicle303" -p "hairSystem2Follicles";
createNode follicle -n "pSphere2FollicleShape303" -p "pSphere2Follicle303";
	setAttr -k off ".v";
	setAttr ".pu" 0.03125;
	setAttr ".pv" 0.03125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve241" -p "pSphere2Follicle303";
createNode nurbsCurve -n "curveShape241" -p "curve241";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere2Follicle309" -p "hairSystem2Follicles";
createNode follicle -n "pSphere2FollicleShape309" -p "pSphere2Follicle309";
	setAttr -k off ".v";
	setAttr ".pu" 0.03125;
	setAttr ".pv" 0.09375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve242" -p "pSphere2Follicle309";
createNode nurbsCurve -n "curveShape242" -p "curve242";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere2Follicle315" -p "hairSystem2Follicles";
createNode follicle -n "pSphere2FollicleShape315" -p "pSphere2Follicle315";
	setAttr -k off ".v";
	setAttr ".pu" 0.03125;
	setAttr ".pv" 0.15625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve243" -p "pSphere2Follicle315";
createNode nurbsCurve -n "curveShape243" -p "curve243";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere2Follicle322" -p "hairSystem2Follicles";
createNode follicle -n "pSphere2FollicleShape322" -p "pSphere2Follicle322";
	setAttr -k off ".v";
	setAttr ".pu" 0.03125;
	setAttr ".pv" 0.21875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve244" -p "pSphere2Follicle322";
createNode nurbsCurve -n "curveShape244" -p "curve244";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere2Follicle328" -p "hairSystem2Follicles";
createNode follicle -n "pSphere2FollicleShape328" -p "pSphere2Follicle328";
	setAttr -k off ".v";
	setAttr ".pu" 0.03125;
	setAttr ".pv" 0.28125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve245" -p "pSphere2Follicle328";
createNode nurbsCurve -n "curveShape245" -p "curve245";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere2Follicle334" -p "hairSystem2Follicles";
createNode follicle -n "pSphere2FollicleShape334" -p "pSphere2Follicle334";
	setAttr -k off ".v";
	setAttr ".pu" 0.03125;
	setAttr ".pv" 0.34375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve246" -p "pSphere2Follicle334";
createNode nurbsCurve -n "curveShape246" -p "curve246";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere2Follicle340" -p "hairSystem2Follicles";
createNode follicle -n "pSphere2FollicleShape340" -p "pSphere2Follicle340";
	setAttr -k off ".v";
	setAttr ".pu" 0.03125;
	setAttr ".pv" 0.40625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve247" -p "pSphere2Follicle340";
createNode nurbsCurve -n "curveShape247" -p "curve247";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere2Follicle346" -p "hairSystem2Follicles";
createNode follicle -n "pSphere2FollicleShape346" -p "pSphere2Follicle346";
	setAttr -k off ".v";
	setAttr ".pu" 0.03125;
	setAttr ".pv" 0.46875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve248" -p "pSphere2Follicle346";
createNode nurbsCurve -n "curveShape248" -p "curve248";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere2Follicle353" -p "hairSystem2Follicles";
createNode follicle -n "pSphere2FollicleShape353" -p "pSphere2Follicle353";
	setAttr -k off ".v";
	setAttr ".pu" 0.03125;
	setAttr ".pv" 0.53125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve249" -p "pSphere2Follicle353";
createNode nurbsCurve -n "curveShape249" -p "curve249";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere2Follicle359" -p "hairSystem2Follicles";
createNode follicle -n "pSphere2FollicleShape359" -p "pSphere2Follicle359";
	setAttr -k off ".v";
	setAttr ".pu" 0.03125;
	setAttr ".pv" 0.59375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve250" -p "pSphere2Follicle359";
createNode nurbsCurve -n "curveShape250" -p "curve250";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere2Follicle365" -p "hairSystem2Follicles";
createNode follicle -n "pSphere2FollicleShape365" -p "pSphere2Follicle365";
	setAttr -k off ".v";
	setAttr ".pu" 0.03125;
	setAttr ".pv" 0.65625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve251" -p "pSphere2Follicle365";
createNode nurbsCurve -n "curveShape251" -p "curve251";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere2Follicle371" -p "hairSystem2Follicles";
createNode follicle -n "pSphere2FollicleShape371" -p "pSphere2Follicle371";
	setAttr -k off ".v";
	setAttr ".pu" 0.03125;
	setAttr ".pv" 0.71875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve252" -p "pSphere2Follicle371";
createNode nurbsCurve -n "curveShape252" -p "curve252";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere2Follicle377" -p "hairSystem2Follicles";
createNode follicle -n "pSphere2FollicleShape377" -p "pSphere2Follicle377";
	setAttr -k off ".v";
	setAttr ".pu" 0.03125;
	setAttr ".pv" 0.78125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve253" -p "pSphere2Follicle377";
createNode nurbsCurve -n "curveShape253" -p "curve253";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere2Follicle384" -p "hairSystem2Follicles";
createNode follicle -n "pSphere2FollicleShape384" -p "pSphere2Follicle384";
	setAttr -k off ".v";
	setAttr ".pu" 0.03125;
	setAttr ".pv" 0.84375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve254" -p "pSphere2Follicle384";
createNode nurbsCurve -n "curveShape254" -p "curve254";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere2Follicle390" -p "hairSystem2Follicles";
createNode follicle -n "pSphere2FollicleShape390" -p "pSphere2Follicle390";
	setAttr -k off ".v";
	setAttr ".pu" 0.03125;
	setAttr ".pv" 0.90625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve255" -p "pSphere2Follicle390";
createNode nurbsCurve -n "curveShape255" -p "curve255";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere2Follicle396" -p "hairSystem2Follicles";
createNode follicle -n "pSphere2FollicleShape396" -p "pSphere2Follicle396";
	setAttr -k off ".v";
	setAttr ".pu" 0.03125;
	setAttr ".pv" 0.96875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve256" -p "pSphere2Follicle396";
createNode nurbsCurve -n "curveShape256" -p "curve256";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere2Follicle909" -p "hairSystem2Follicles";
createNode follicle -n "pSphere2FollicleShape909" -p "pSphere2Follicle909";
	setAttr -k off ".v";
	setAttr ".pu" 0.09375;
	setAttr ".pv" 0.09375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve257" -p "pSphere2Follicle909";
createNode nurbsCurve -n "curveShape257" -p "curve257";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere2Follicle915" -p "hairSystem2Follicles";
createNode follicle -n "pSphere2FollicleShape915" -p "pSphere2Follicle915";
	setAttr -k off ".v";
	setAttr ".pu" 0.09375;
	setAttr ".pv" 0.15625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve258" -p "pSphere2Follicle915";
createNode nurbsCurve -n "curveShape258" -p "curve258";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere2Follicle922" -p "hairSystem2Follicles";
createNode follicle -n "pSphere2FollicleShape922" -p "pSphere2Follicle922";
	setAttr -k off ".v";
	setAttr ".pu" 0.09375;
	setAttr ".pv" 0.21875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve259" -p "pSphere2Follicle922";
createNode nurbsCurve -n "curveShape259" -p "curve259";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere2Follicle928" -p "hairSystem2Follicles";
createNode follicle -n "pSphere2FollicleShape928" -p "pSphere2Follicle928";
	setAttr -k off ".v";
	setAttr ".pu" 0.09375;
	setAttr ".pv" 0.28125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve260" -p "pSphere2Follicle928";
createNode nurbsCurve -n "curveShape260" -p "curve260";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere2Follicle934" -p "hairSystem2Follicles";
createNode follicle -n "pSphere2FollicleShape934" -p "pSphere2Follicle934";
	setAttr -k off ".v";
	setAttr ".pu" 0.09375;
	setAttr ".pv" 0.34375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve261" -p "pSphere2Follicle934";
createNode nurbsCurve -n "curveShape261" -p "curve261";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere2Follicle940" -p "hairSystem2Follicles";
createNode follicle -n "pSphere2FollicleShape940" -p "pSphere2Follicle940";
	setAttr -k off ".v";
	setAttr ".pu" 0.09375;
	setAttr ".pv" 0.40625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve262" -p "pSphere2Follicle940";
createNode nurbsCurve -n "curveShape262" -p "curve262";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere2Follicle946" -p "hairSystem2Follicles";
createNode follicle -n "pSphere2FollicleShape946" -p "pSphere2Follicle946";
	setAttr -k off ".v";
	setAttr ".pu" 0.09375;
	setAttr ".pv" 0.46875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve263" -p "pSphere2Follicle946";
createNode nurbsCurve -n "curveShape263" -p "curve263";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere2Follicle953" -p "hairSystem2Follicles";
createNode follicle -n "pSphere2FollicleShape953" -p "pSphere2Follicle953";
	setAttr -k off ".v";
	setAttr ".pu" 0.09375;
	setAttr ".pv" 0.53125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve264" -p "pSphere2Follicle953";
createNode nurbsCurve -n "curveShape264" -p "curve264";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere2Follicle959" -p "hairSystem2Follicles";
createNode follicle -n "pSphere2FollicleShape959" -p "pSphere2Follicle959";
	setAttr -k off ".v";
	setAttr ".pu" 0.09375;
	setAttr ".pv" 0.59375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve265" -p "pSphere2Follicle959";
createNode nurbsCurve -n "curveShape265" -p "curve265";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere2Follicle965" -p "hairSystem2Follicles";
createNode follicle -n "pSphere2FollicleShape965" -p "pSphere2Follicle965";
	setAttr -k off ".v";
	setAttr ".pu" 0.09375;
	setAttr ".pv" 0.65625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve266" -p "pSphere2Follicle965";
createNode nurbsCurve -n "curveShape266" -p "curve266";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere2Follicle971" -p "hairSystem2Follicles";
createNode follicle -n "pSphere2FollicleShape971" -p "pSphere2Follicle971";
	setAttr -k off ".v";
	setAttr ".pu" 0.09375;
	setAttr ".pv" 0.71875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve267" -p "pSphere2Follicle971";
createNode nurbsCurve -n "curveShape267" -p "curve267";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere2Follicle977" -p "hairSystem2Follicles";
createNode follicle -n "pSphere2FollicleShape977" -p "pSphere2Follicle977";
	setAttr -k off ".v";
	setAttr ".pu" 0.09375;
	setAttr ".pv" 0.78125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve268" -p "pSphere2Follicle977";
createNode nurbsCurve -n "curveShape268" -p "curve268";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere2Follicle984" -p "hairSystem2Follicles";
createNode follicle -n "pSphere2FollicleShape984" -p "pSphere2Follicle984";
	setAttr -k off ".v";
	setAttr ".pu" 0.09375;
	setAttr ".pv" 0.84375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve269" -p "pSphere2Follicle984";
createNode nurbsCurve -n "curveShape269" -p "curve269";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere2Follicle990" -p "hairSystem2Follicles";
createNode follicle -n "pSphere2FollicleShape990" -p "pSphere2Follicle990";
	setAttr -k off ".v";
	setAttr ".pu" 0.09375;
	setAttr ".pv" 0.90625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve270" -p "pSphere2Follicle990";
createNode nurbsCurve -n "curveShape270" -p "curve270";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere2Follicle1509" -p "hairSystem2Follicles";
createNode follicle -n "pSphere2FollicleShape1509" -p "pSphere2Follicle1509";
	setAttr -k off ".v";
	setAttr ".pu" 0.15625;
	setAttr ".pv" 0.09375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve271" -p "pSphere2Follicle1509";
createNode nurbsCurve -n "curveShape271" -p "curve271";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere2Follicle1515" -p "hairSystem2Follicles";
createNode follicle -n "pSphere2FollicleShape1515" -p "pSphere2Follicle1515";
	setAttr -k off ".v";
	setAttr ".pu" 0.15625;
	setAttr ".pv" 0.15625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve272" -p "pSphere2Follicle1515";
createNode nurbsCurve -n "curveShape272" -p "curve272";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere2Follicle1522" -p "hairSystem2Follicles";
createNode follicle -n "pSphere2FollicleShape1522" -p "pSphere2Follicle1522";
	setAttr -k off ".v";
	setAttr ".pu" 0.15625;
	setAttr ".pv" 0.21875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve273" -p "pSphere2Follicle1522";
createNode nurbsCurve -n "curveShape273" -p "curve273";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere2Follicle1528" -p "hairSystem2Follicles";
createNode follicle -n "pSphere2FollicleShape1528" -p "pSphere2Follicle1528";
	setAttr -k off ".v";
	setAttr ".pu" 0.15625;
	setAttr ".pv" 0.28125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve274" -p "pSphere2Follicle1528";
createNode nurbsCurve -n "curveShape274" -p "curve274";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere2Follicle1534" -p "hairSystem2Follicles";
createNode follicle -n "pSphere2FollicleShape1534" -p "pSphere2Follicle1534";
	setAttr -k off ".v";
	setAttr ".pu" 0.15625;
	setAttr ".pv" 0.34375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve275" -p "pSphere2Follicle1534";
createNode nurbsCurve -n "curveShape275" -p "curve275";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere2Follicle1540" -p "hairSystem2Follicles";
createNode follicle -n "pSphere2FollicleShape1540" -p "pSphere2Follicle1540";
	setAttr -k off ".v";
	setAttr ".pu" 0.15625;
	setAttr ".pv" 0.40625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve276" -p "pSphere2Follicle1540";
createNode nurbsCurve -n "curveShape276" -p "curve276";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere2Follicle1546" -p "hairSystem2Follicles";
createNode follicle -n "pSphere2FollicleShape1546" -p "pSphere2Follicle1546";
	setAttr -k off ".v";
	setAttr ".pu" 0.15625;
	setAttr ".pv" 0.46875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve277" -p "pSphere2Follicle1546";
createNode nurbsCurve -n "curveShape277" -p "curve277";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere2Follicle1553" -p "hairSystem2Follicles";
createNode follicle -n "pSphere2FollicleShape1553" -p "pSphere2Follicle1553";
	setAttr -k off ".v";
	setAttr ".pu" 0.15625;
	setAttr ".pv" 0.53125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve278" -p "pSphere2Follicle1553";
createNode nurbsCurve -n "curveShape278" -p "curve278";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere2Follicle1559" -p "hairSystem2Follicles";
createNode follicle -n "pSphere2FollicleShape1559" -p "pSphere2Follicle1559";
	setAttr -k off ".v";
	setAttr ".pu" 0.15625;
	setAttr ".pv" 0.59375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve279" -p "pSphere2Follicle1559";
createNode nurbsCurve -n "curveShape279" -p "curve279";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere2Follicle1565" -p "hairSystem2Follicles";
createNode follicle -n "pSphere2FollicleShape1565" -p "pSphere2Follicle1565";
	setAttr -k off ".v";
	setAttr ".pu" 0.15625;
	setAttr ".pv" 0.65625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve280" -p "pSphere2Follicle1565";
createNode nurbsCurve -n "curveShape280" -p "curve280";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere2Follicle1571" -p "hairSystem2Follicles";
createNode follicle -n "pSphere2FollicleShape1571" -p "pSphere2Follicle1571";
	setAttr -k off ".v";
	setAttr ".pu" 0.15625;
	setAttr ".pv" 0.71875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve281" -p "pSphere2Follicle1571";
createNode nurbsCurve -n "curveShape281" -p "curve281";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere2Follicle1577" -p "hairSystem2Follicles";
createNode follicle -n "pSphere2FollicleShape1577" -p "pSphere2Follicle1577";
	setAttr -k off ".v";
	setAttr ".pu" 0.15625;
	setAttr ".pv" 0.78125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve282" -p "pSphere2Follicle1577";
createNode nurbsCurve -n "curveShape282" -p "curve282";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere2Follicle1584" -p "hairSystem2Follicles";
createNode follicle -n "pSphere2FollicleShape1584" -p "pSphere2Follicle1584";
	setAttr -k off ".v";
	setAttr ".pu" 0.15625;
	setAttr ".pv" 0.84375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve283" -p "pSphere2Follicle1584";
createNode nurbsCurve -n "curveShape283" -p "curve283";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere2Follicle1590" -p "hairSystem2Follicles";
createNode follicle -n "pSphere2FollicleShape1590" -p "pSphere2Follicle1590";
	setAttr -k off ".v";
	setAttr ".pu" 0.15625;
	setAttr ".pv" 0.90625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve284" -p "pSphere2Follicle1590";
createNode nurbsCurve -n "curveShape284" -p "curve284";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere2Follicle2203" -p "hairSystem2Follicles";
createNode follicle -n "pSphere2FollicleShape2203" -p "pSphere2Follicle2203";
	setAttr -k off ".v";
	setAttr ".pu" 0.21875;
	setAttr ".pv" 0.03125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve285" -p "pSphere2Follicle2203";
createNode nurbsCurve -n "curveShape285" -p "curve285";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere2Follicle2209" -p "hairSystem2Follicles";
createNode follicle -n "pSphere2FollicleShape2209" -p "pSphere2Follicle2209";
	setAttr -k off ".v";
	setAttr ".pu" 0.21875;
	setAttr ".pv" 0.09375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve286" -p "pSphere2Follicle2209";
createNode nurbsCurve -n "curveShape286" -p "curve286";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere2Follicle2215" -p "hairSystem2Follicles";
createNode follicle -n "pSphere2FollicleShape2215" -p "pSphere2Follicle2215";
	setAttr -k off ".v";
	setAttr ".pu" 0.21875;
	setAttr ".pv" 0.15625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve287" -p "pSphere2Follicle2215";
createNode nurbsCurve -n "curveShape287" -p "curve287";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere2Follicle2222" -p "hairSystem2Follicles";
createNode follicle -n "pSphere2FollicleShape2222" -p "pSphere2Follicle2222";
	setAttr -k off ".v";
	setAttr ".pu" 0.21875;
	setAttr ".pv" 0.21875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve288" -p "pSphere2Follicle2222";
createNode nurbsCurve -n "curveShape288" -p "curve288";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere2Follicle2228" -p "hairSystem2Follicles";
createNode follicle -n "pSphere2FollicleShape2228" -p "pSphere2Follicle2228";
	setAttr -k off ".v";
	setAttr ".pu" 0.21875;
	setAttr ".pv" 0.28125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve289" -p "pSphere2Follicle2228";
createNode nurbsCurve -n "curveShape289" -p "curve289";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere2Follicle2234" -p "hairSystem2Follicles";
createNode follicle -n "pSphere2FollicleShape2234" -p "pSphere2Follicle2234";
	setAttr -k off ".v";
	setAttr ".pu" 0.21875;
	setAttr ".pv" 0.34375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve290" -p "pSphere2Follicle2234";
createNode nurbsCurve -n "curveShape290" -p "curve290";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere2Follicle2240" -p "hairSystem2Follicles";
createNode follicle -n "pSphere2FollicleShape2240" -p "pSphere2Follicle2240";
	setAttr -k off ".v";
	setAttr ".pu" 0.21875;
	setAttr ".pv" 0.40625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve291" -p "pSphere2Follicle2240";
createNode nurbsCurve -n "curveShape291" -p "curve291";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere2Follicle2246" -p "hairSystem2Follicles";
createNode follicle -n "pSphere2FollicleShape2246" -p "pSphere2Follicle2246";
	setAttr -k off ".v";
	setAttr ".pu" 0.21875;
	setAttr ".pv" 0.46875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve292" -p "pSphere2Follicle2246";
createNode nurbsCurve -n "curveShape292" -p "curve292";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere2Follicle2253" -p "hairSystem2Follicles";
createNode follicle -n "pSphere2FollicleShape2253" -p "pSphere2Follicle2253";
	setAttr -k off ".v";
	setAttr ".pu" 0.21875;
	setAttr ".pv" 0.53125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve293" -p "pSphere2Follicle2253";
createNode nurbsCurve -n "curveShape293" -p "curve293";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere2Follicle2259" -p "hairSystem2Follicles";
createNode follicle -n "pSphere2FollicleShape2259" -p "pSphere2Follicle2259";
	setAttr -k off ".v";
	setAttr ".pu" 0.21875;
	setAttr ".pv" 0.59375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve294" -p "pSphere2Follicle2259";
createNode nurbsCurve -n "curveShape294" -p "curve294";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere2Follicle2265" -p "hairSystem2Follicles";
createNode follicle -n "pSphere2FollicleShape2265" -p "pSphere2Follicle2265";
	setAttr -k off ".v";
	setAttr ".pu" 0.21875;
	setAttr ".pv" 0.65625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve295" -p "pSphere2Follicle2265";
createNode nurbsCurve -n "curveShape295" -p "curve295";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere2Follicle2271" -p "hairSystem2Follicles";
createNode follicle -n "pSphere2FollicleShape2271" -p "pSphere2Follicle2271";
	setAttr -k off ".v";
	setAttr ".pu" 0.21875;
	setAttr ".pv" 0.71875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve296" -p "pSphere2Follicle2271";
createNode nurbsCurve -n "curveShape296" -p "curve296";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere2Follicle2277" -p "hairSystem2Follicles";
createNode follicle -n "pSphere2FollicleShape2277" -p "pSphere2Follicle2277";
	setAttr -k off ".v";
	setAttr ".pu" 0.21875;
	setAttr ".pv" 0.78125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve297" -p "pSphere2Follicle2277";
createNode nurbsCurve -n "curveShape297" -p "curve297";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere2Follicle2284" -p "hairSystem2Follicles";
createNode follicle -n "pSphere2FollicleShape2284" -p "pSphere2Follicle2284";
	setAttr -k off ".v";
	setAttr ".pu" 0.21875;
	setAttr ".pv" 0.84375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve298" -p "pSphere2Follicle2284";
createNode nurbsCurve -n "curveShape298" -p "curve298";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere2Follicle2290" -p "hairSystem2Follicles";
createNode follicle -n "pSphere2FollicleShape2290" -p "pSphere2Follicle2290";
	setAttr -k off ".v";
	setAttr ".pu" 0.21875;
	setAttr ".pv" 0.90625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve299" -p "pSphere2Follicle2290";
createNode nurbsCurve -n "curveShape299" -p "curve299";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere2Follicle2296" -p "hairSystem2Follicles";
createNode follicle -n "pSphere2FollicleShape2296" -p "pSphere2Follicle2296";
	setAttr -k off ".v";
	setAttr ".pu" 0.21875;
	setAttr ".pv" 0.96875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve300" -p "pSphere2Follicle2296";
createNode nurbsCurve -n "curveShape300" -p "curve300";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere2Follicle2803" -p "hairSystem2Follicles";
createNode follicle -n "pSphere2FollicleShape2803" -p "pSphere2Follicle2803";
	setAttr -k off ".v";
	setAttr ".pu" 0.28125;
	setAttr ".pv" 0.03125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve301" -p "pSphere2Follicle2803";
createNode nurbsCurve -n "curveShape301" -p "curve301";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere2Follicle2809" -p "hairSystem2Follicles";
createNode follicle -n "pSphere2FollicleShape2809" -p "pSphere2Follicle2809";
	setAttr -k off ".v";
	setAttr ".pu" 0.28125;
	setAttr ".pv" 0.09375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve302" -p "pSphere2Follicle2809";
createNode nurbsCurve -n "curveShape302" -p "curve302";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere2Follicle2815" -p "hairSystem2Follicles";
createNode follicle -n "pSphere2FollicleShape2815" -p "pSphere2Follicle2815";
	setAttr -k off ".v";
	setAttr ".pu" 0.28125;
	setAttr ".pv" 0.15625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve303" -p "pSphere2Follicle2815";
createNode nurbsCurve -n "curveShape303" -p "curve303";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere2Follicle2822" -p "hairSystem2Follicles";
createNode follicle -n "pSphere2FollicleShape2822" -p "pSphere2Follicle2822";
	setAttr -k off ".v";
	setAttr ".pu" 0.28125;
	setAttr ".pv" 0.21875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve304" -p "pSphere2Follicle2822";
createNode nurbsCurve -n "curveShape304" -p "curve304";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere2Follicle2828" -p "hairSystem2Follicles";
createNode follicle -n "pSphere2FollicleShape2828" -p "pSphere2Follicle2828";
	setAttr -k off ".v";
	setAttr ".pu" 0.28125;
	setAttr ".pv" 0.28125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve305" -p "pSphere2Follicle2828";
createNode nurbsCurve -n "curveShape305" -p "curve305";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere2Follicle2834" -p "hairSystem2Follicles";
createNode follicle -n "pSphere2FollicleShape2834" -p "pSphere2Follicle2834";
	setAttr -k off ".v";
	setAttr ".pu" 0.28125;
	setAttr ".pv" 0.34375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve306" -p "pSphere2Follicle2834";
createNode nurbsCurve -n "curveShape306" -p "curve306";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere2Follicle2840" -p "hairSystem2Follicles";
createNode follicle -n "pSphere2FollicleShape2840" -p "pSphere2Follicle2840";
	setAttr -k off ".v";
	setAttr ".pu" 0.28125;
	setAttr ".pv" 0.40625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve307" -p "pSphere2Follicle2840";
createNode nurbsCurve -n "curveShape307" -p "curve307";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere2Follicle2846" -p "hairSystem2Follicles";
createNode follicle -n "pSphere2FollicleShape2846" -p "pSphere2Follicle2846";
	setAttr -k off ".v";
	setAttr ".pu" 0.28125;
	setAttr ".pv" 0.46875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve308" -p "pSphere2Follicle2846";
createNode nurbsCurve -n "curveShape308" -p "curve308";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere2Follicle2853" -p "hairSystem2Follicles";
createNode follicle -n "pSphere2FollicleShape2853" -p "pSphere2Follicle2853";
	setAttr -k off ".v";
	setAttr ".pu" 0.28125;
	setAttr ".pv" 0.53125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve309" -p "pSphere2Follicle2853";
createNode nurbsCurve -n "curveShape309" -p "curve309";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere2Follicle2859" -p "hairSystem2Follicles";
createNode follicle -n "pSphere2FollicleShape2859" -p "pSphere2Follicle2859";
	setAttr -k off ".v";
	setAttr ".pu" 0.28125;
	setAttr ".pv" 0.59375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve310" -p "pSphere2Follicle2859";
createNode nurbsCurve -n "curveShape310" -p "curve310";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere2Follicle2865" -p "hairSystem2Follicles";
createNode follicle -n "pSphere2FollicleShape2865" -p "pSphere2Follicle2865";
	setAttr -k off ".v";
	setAttr ".pu" 0.28125;
	setAttr ".pv" 0.65625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve311" -p "pSphere2Follicle2865";
createNode nurbsCurve -n "curveShape311" -p "curve311";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere2Follicle2871" -p "hairSystem2Follicles";
createNode follicle -n "pSphere2FollicleShape2871" -p "pSphere2Follicle2871";
	setAttr -k off ".v";
	setAttr ".pu" 0.28125;
	setAttr ".pv" 0.71875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve312" -p "pSphere2Follicle2871";
createNode nurbsCurve -n "curveShape312" -p "curve312";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere2Follicle2877" -p "hairSystem2Follicles";
createNode follicle -n "pSphere2FollicleShape2877" -p "pSphere2Follicle2877";
	setAttr -k off ".v";
	setAttr ".pu" 0.28125;
	setAttr ".pv" 0.78125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve313" -p "pSphere2Follicle2877";
createNode nurbsCurve -n "curveShape313" -p "curve313";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere2Follicle2884" -p "hairSystem2Follicles";
createNode follicle -n "pSphere2FollicleShape2884" -p "pSphere2Follicle2884";
	setAttr -k off ".v";
	setAttr ".pu" 0.28125;
	setAttr ".pv" 0.84375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve314" -p "pSphere2Follicle2884";
createNode nurbsCurve -n "curveShape314" -p "curve314";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere2Follicle2890" -p "hairSystem2Follicles";
createNode follicle -n "pSphere2FollicleShape2890" -p "pSphere2Follicle2890";
	setAttr -k off ".v";
	setAttr ".pu" 0.28125;
	setAttr ".pv" 0.90625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve315" -p "pSphere2Follicle2890";
createNode nurbsCurve -n "curveShape315" -p "curve315";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere2Follicle2896" -p "hairSystem2Follicles";
createNode follicle -n "pSphere2FollicleShape2896" -p "pSphere2Follicle2896";
	setAttr -k off ".v";
	setAttr ".pu" 0.28125;
	setAttr ".pv" 0.96875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve316" -p "pSphere2Follicle2896";
createNode nurbsCurve -n "curveShape316" -p "curve316";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere2Follicle3409" -p "hairSystem2Follicles";
createNode follicle -n "pSphere2FollicleShape3409" -p "pSphere2Follicle3409";
	setAttr -k off ".v";
	setAttr ".pu" 0.34375;
	setAttr ".pv" 0.09375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve317" -p "pSphere2Follicle3409";
createNode nurbsCurve -n "curveShape317" -p "curve317";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere2Follicle3415" -p "hairSystem2Follicles";
createNode follicle -n "pSphere2FollicleShape3415" -p "pSphere2Follicle3415";
	setAttr -k off ".v";
	setAttr ".pu" 0.34375;
	setAttr ".pv" 0.15625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve318" -p "pSphere2Follicle3415";
createNode nurbsCurve -n "curveShape318" -p "curve318";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere2Follicle3422" -p "hairSystem2Follicles";
createNode follicle -n "pSphere2FollicleShape3422" -p "pSphere2Follicle3422";
	setAttr -k off ".v";
	setAttr ".pu" 0.34375;
	setAttr ".pv" 0.21875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve319" -p "pSphere2Follicle3422";
createNode nurbsCurve -n "curveShape319" -p "curve319";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere2Follicle3428" -p "hairSystem2Follicles";
createNode follicle -n "pSphere2FollicleShape3428" -p "pSphere2Follicle3428";
	setAttr -k off ".v";
	setAttr ".pu" 0.34375;
	setAttr ".pv" 0.28125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve320" -p "pSphere2Follicle3428";
createNode nurbsCurve -n "curveShape320" -p "curve320";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere2Follicle3434" -p "hairSystem2Follicles";
createNode follicle -n "pSphere2FollicleShape3434" -p "pSphere2Follicle3434";
	setAttr -k off ".v";
	setAttr ".pu" 0.34375;
	setAttr ".pv" 0.34375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve321" -p "pSphere2Follicle3434";
createNode nurbsCurve -n "curveShape321" -p "curve321";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere2Follicle3440" -p "hairSystem2Follicles";
createNode follicle -n "pSphere2FollicleShape3440" -p "pSphere2Follicle3440";
	setAttr -k off ".v";
	setAttr ".pu" 0.34375;
	setAttr ".pv" 0.40625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve322" -p "pSphere2Follicle3440";
createNode nurbsCurve -n "curveShape322" -p "curve322";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere2Follicle3446" -p "hairSystem2Follicles";
createNode follicle -n "pSphere2FollicleShape3446" -p "pSphere2Follicle3446";
	setAttr -k off ".v";
	setAttr ".pu" 0.34375;
	setAttr ".pv" 0.46875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve323" -p "pSphere2Follicle3446";
createNode nurbsCurve -n "curveShape323" -p "curve323";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere2Follicle3453" -p "hairSystem2Follicles";
createNode follicle -n "pSphere2FollicleShape3453" -p "pSphere2Follicle3453";
	setAttr -k off ".v";
	setAttr ".pu" 0.34375;
	setAttr ".pv" 0.53125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve324" -p "pSphere2Follicle3453";
createNode nurbsCurve -n "curveShape324" -p "curve324";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere2Follicle3459" -p "hairSystem2Follicles";
createNode follicle -n "pSphere2FollicleShape3459" -p "pSphere2Follicle3459";
	setAttr -k off ".v";
	setAttr ".pu" 0.34375;
	setAttr ".pv" 0.59375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve325" -p "pSphere2Follicle3459";
createNode nurbsCurve -n "curveShape325" -p "curve325";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere2Follicle3465" -p "hairSystem2Follicles";
createNode follicle -n "pSphere2FollicleShape3465" -p "pSphere2Follicle3465";
	setAttr -k off ".v";
	setAttr ".pu" 0.34375;
	setAttr ".pv" 0.65625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve326" -p "pSphere2Follicle3465";
createNode nurbsCurve -n "curveShape326" -p "curve326";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere2Follicle3471" -p "hairSystem2Follicles";
createNode follicle -n "pSphere2FollicleShape3471" -p "pSphere2Follicle3471";
	setAttr -k off ".v";
	setAttr ".pu" 0.34375;
	setAttr ".pv" 0.71875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve327" -p "pSphere2Follicle3471";
createNode nurbsCurve -n "curveShape327" -p "curve327";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere2Follicle3477" -p "hairSystem2Follicles";
createNode follicle -n "pSphere2FollicleShape3477" -p "pSphere2Follicle3477";
	setAttr -k off ".v";
	setAttr ".pu" 0.34375;
	setAttr ".pv" 0.78125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve328" -p "pSphere2Follicle3477";
createNode nurbsCurve -n "curveShape328" -p "curve328";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere2Follicle3484" -p "hairSystem2Follicles";
createNode follicle -n "pSphere2FollicleShape3484" -p "pSphere2Follicle3484";
	setAttr -k off ".v";
	setAttr ".pu" 0.34375;
	setAttr ".pv" 0.84375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve329" -p "pSphere2Follicle3484";
createNode nurbsCurve -n "curveShape329" -p "curve329";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere2Follicle3490" -p "hairSystem2Follicles";
createNode follicle -n "pSphere2FollicleShape3490" -p "pSphere2Follicle3490";
	setAttr -k off ".v";
	setAttr ".pu" 0.34375;
	setAttr ".pv" 0.90625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve330" -p "pSphere2Follicle3490";
createNode nurbsCurve -n "curveShape330" -p "curve330";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere2Follicle4009" -p "hairSystem2Follicles";
createNode follicle -n "pSphere2FollicleShape4009" -p "pSphere2Follicle4009";
	setAttr -k off ".v";
	setAttr ".pu" 0.40625;
	setAttr ".pv" 0.09375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve331" -p "pSphere2Follicle4009";
createNode nurbsCurve -n "curveShape331" -p "curve331";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere2Follicle4015" -p "hairSystem2Follicles";
createNode follicle -n "pSphere2FollicleShape4015" -p "pSphere2Follicle4015";
	setAttr -k off ".v";
	setAttr ".pu" 0.40625;
	setAttr ".pv" 0.15625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve332" -p "pSphere2Follicle4015";
createNode nurbsCurve -n "curveShape332" -p "curve332";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere2Follicle4022" -p "hairSystem2Follicles";
createNode follicle -n "pSphere2FollicleShape4022" -p "pSphere2Follicle4022";
	setAttr -k off ".v";
	setAttr ".pu" 0.40625;
	setAttr ".pv" 0.21875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve333" -p "pSphere2Follicle4022";
createNode nurbsCurve -n "curveShape333" -p "curve333";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere2Follicle4028" -p "hairSystem2Follicles";
createNode follicle -n "pSphere2FollicleShape4028" -p "pSphere2Follicle4028";
	setAttr -k off ".v";
	setAttr ".pu" 0.40625;
	setAttr ".pv" 0.28125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve334" -p "pSphere2Follicle4028";
createNode nurbsCurve -n "curveShape334" -p "curve334";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere2Follicle4034" -p "hairSystem2Follicles";
createNode follicle -n "pSphere2FollicleShape4034" -p "pSphere2Follicle4034";
	setAttr -k off ".v";
	setAttr ".pu" 0.40625;
	setAttr ".pv" 0.34375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve335" -p "pSphere2Follicle4034";
createNode nurbsCurve -n "curveShape335" -p "curve335";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere2Follicle4040" -p "hairSystem2Follicles";
createNode follicle -n "pSphere2FollicleShape4040" -p "pSphere2Follicle4040";
	setAttr -k off ".v";
	setAttr ".pu" 0.40625;
	setAttr ".pv" 0.40625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve336" -p "pSphere2Follicle4040";
createNode nurbsCurve -n "curveShape336" -p "curve336";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere2Follicle4046" -p "hairSystem2Follicles";
createNode follicle -n "pSphere2FollicleShape4046" -p "pSphere2Follicle4046";
	setAttr -k off ".v";
	setAttr ".pu" 0.40625;
	setAttr ".pv" 0.46875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve337" -p "pSphere2Follicle4046";
createNode nurbsCurve -n "curveShape337" -p "curve337";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere2Follicle4053" -p "hairSystem2Follicles";
createNode follicle -n "pSphere2FollicleShape4053" -p "pSphere2Follicle4053";
	setAttr -k off ".v";
	setAttr ".pu" 0.40625;
	setAttr ".pv" 0.53125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve338" -p "pSphere2Follicle4053";
createNode nurbsCurve -n "curveShape338" -p "curve338";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere2Follicle4059" -p "hairSystem2Follicles";
createNode follicle -n "pSphere2FollicleShape4059" -p "pSphere2Follicle4059";
	setAttr -k off ".v";
	setAttr ".pu" 0.40625;
	setAttr ".pv" 0.59375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve339" -p "pSphere2Follicle4059";
createNode nurbsCurve -n "curveShape339" -p "curve339";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere2Follicle4065" -p "hairSystem2Follicles";
createNode follicle -n "pSphere2FollicleShape4065" -p "pSphere2Follicle4065";
	setAttr -k off ".v";
	setAttr ".pu" 0.40625;
	setAttr ".pv" 0.65625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve340" -p "pSphere2Follicle4065";
createNode nurbsCurve -n "curveShape340" -p "curve340";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere2Follicle4071" -p "hairSystem2Follicles";
createNode follicle -n "pSphere2FollicleShape4071" -p "pSphere2Follicle4071";
	setAttr -k off ".v";
	setAttr ".pu" 0.40625;
	setAttr ".pv" 0.71875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve341" -p "pSphere2Follicle4071";
createNode nurbsCurve -n "curveShape341" -p "curve341";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere2Follicle4077" -p "hairSystem2Follicles";
createNode follicle -n "pSphere2FollicleShape4077" -p "pSphere2Follicle4077";
	setAttr -k off ".v";
	setAttr ".pu" 0.40625;
	setAttr ".pv" 0.78125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve342" -p "pSphere2Follicle4077";
createNode nurbsCurve -n "curveShape342" -p "curve342";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere2Follicle4084" -p "hairSystem2Follicles";
createNode follicle -n "pSphere2FollicleShape4084" -p "pSphere2Follicle4084";
	setAttr -k off ".v";
	setAttr ".pu" 0.40625;
	setAttr ".pv" 0.84375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve343" -p "pSphere2Follicle4084";
createNode nurbsCurve -n "curveShape343" -p "curve343";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere2Follicle4090" -p "hairSystem2Follicles";
createNode follicle -n "pSphere2FollicleShape4090" -p "pSphere2Follicle4090";
	setAttr -k off ".v";
	setAttr ".pu" 0.40625;
	setAttr ".pv" 0.90625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve344" -p "pSphere2Follicle4090";
createNode nurbsCurve -n "curveShape344" -p "curve344";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere2Follicle4603" -p "hairSystem2Follicles";
createNode follicle -n "pSphere2FollicleShape4603" -p "pSphere2Follicle4603";
	setAttr -k off ".v";
	setAttr ".pu" 0.46875;
	setAttr ".pv" 0.03125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve345" -p "pSphere2Follicle4603";
createNode nurbsCurve -n "curveShape345" -p "curve345";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere2Follicle4609" -p "hairSystem2Follicles";
createNode follicle -n "pSphere2FollicleShape4609" -p "pSphere2Follicle4609";
	setAttr -k off ".v";
	setAttr ".pu" 0.46875;
	setAttr ".pv" 0.09375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve346" -p "pSphere2Follicle4609";
createNode nurbsCurve -n "curveShape346" -p "curve346";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere2Follicle4615" -p "hairSystem2Follicles";
createNode follicle -n "pSphere2FollicleShape4615" -p "pSphere2Follicle4615";
	setAttr -k off ".v";
	setAttr ".pu" 0.46875;
	setAttr ".pv" 0.15625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve347" -p "pSphere2Follicle4615";
createNode nurbsCurve -n "curveShape347" -p "curve347";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere2Follicle4622" -p "hairSystem2Follicles";
createNode follicle -n "pSphere2FollicleShape4622" -p "pSphere2Follicle4622";
	setAttr -k off ".v";
	setAttr ".pu" 0.46875;
	setAttr ".pv" 0.21875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve348" -p "pSphere2Follicle4622";
createNode nurbsCurve -n "curveShape348" -p "curve348";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere2Follicle4628" -p "hairSystem2Follicles";
createNode follicle -n "pSphere2FollicleShape4628" -p "pSphere2Follicle4628";
	setAttr -k off ".v";
	setAttr ".pu" 0.46875;
	setAttr ".pv" 0.28125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve349" -p "pSphere2Follicle4628";
createNode nurbsCurve -n "curveShape349" -p "curve349";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere2Follicle4634" -p "hairSystem2Follicles";
createNode follicle -n "pSphere2FollicleShape4634" -p "pSphere2Follicle4634";
	setAttr -k off ".v";
	setAttr ".pu" 0.46875;
	setAttr ".pv" 0.34375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve350" -p "pSphere2Follicle4634";
createNode nurbsCurve -n "curveShape350" -p "curve350";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere2Follicle4640" -p "hairSystem2Follicles";
createNode follicle -n "pSphere2FollicleShape4640" -p "pSphere2Follicle4640";
	setAttr -k off ".v";
	setAttr ".pu" 0.46875;
	setAttr ".pv" 0.40625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve351" -p "pSphere2Follicle4640";
createNode nurbsCurve -n "curveShape351" -p "curve351";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere2Follicle4646" -p "hairSystem2Follicles";
createNode follicle -n "pSphere2FollicleShape4646" -p "pSphere2Follicle4646";
	setAttr -k off ".v";
	setAttr ".pu" 0.46875;
	setAttr ".pv" 0.46875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve352" -p "pSphere2Follicle4646";
createNode nurbsCurve -n "curveShape352" -p "curve352";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere2Follicle4653" -p "hairSystem2Follicles";
createNode follicle -n "pSphere2FollicleShape4653" -p "pSphere2Follicle4653";
	setAttr -k off ".v";
	setAttr ".pu" 0.46875;
	setAttr ".pv" 0.53125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve353" -p "pSphere2Follicle4653";
createNode nurbsCurve -n "curveShape353" -p "curve353";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere2Follicle4659" -p "hairSystem2Follicles";
createNode follicle -n "pSphere2FollicleShape4659" -p "pSphere2Follicle4659";
	setAttr -k off ".v";
	setAttr ".pu" 0.46875;
	setAttr ".pv" 0.59375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve354" -p "pSphere2Follicle4659";
createNode nurbsCurve -n "curveShape354" -p "curve354";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere2Follicle4665" -p "hairSystem2Follicles";
createNode follicle -n "pSphere2FollicleShape4665" -p "pSphere2Follicle4665";
	setAttr -k off ".v";
	setAttr ".pu" 0.46875;
	setAttr ".pv" 0.65625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve355" -p "pSphere2Follicle4665";
createNode nurbsCurve -n "curveShape355" -p "curve355";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere2Follicle4671" -p "hairSystem2Follicles";
createNode follicle -n "pSphere2FollicleShape4671" -p "pSphere2Follicle4671";
	setAttr -k off ".v";
	setAttr ".pu" 0.46875;
	setAttr ".pv" 0.71875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve356" -p "pSphere2Follicle4671";
createNode nurbsCurve -n "curveShape356" -p "curve356";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere2Follicle4677" -p "hairSystem2Follicles";
createNode follicle -n "pSphere2FollicleShape4677" -p "pSphere2Follicle4677";
	setAttr -k off ".v";
	setAttr ".pu" 0.46875;
	setAttr ".pv" 0.78125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve357" -p "pSphere2Follicle4677";
createNode nurbsCurve -n "curveShape357" -p "curve357";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere2Follicle4684" -p "hairSystem2Follicles";
createNode follicle -n "pSphere2FollicleShape4684" -p "pSphere2Follicle4684";
	setAttr -k off ".v";
	setAttr ".pu" 0.46875;
	setAttr ".pv" 0.84375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve358" -p "pSphere2Follicle4684";
createNode nurbsCurve -n "curveShape358" -p "curve358";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere2Follicle4690" -p "hairSystem2Follicles";
createNode follicle -n "pSphere2FollicleShape4690" -p "pSphere2Follicle4690";
	setAttr -k off ".v";
	setAttr ".pu" 0.46875;
	setAttr ".pv" 0.90625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve359" -p "pSphere2Follicle4690";
createNode nurbsCurve -n "curveShape359" -p "curve359";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere2Follicle4696" -p "hairSystem2Follicles";
createNode follicle -n "pSphere2FollicleShape4696" -p "pSphere2Follicle4696";
	setAttr -k off ".v";
	setAttr ".pu" 0.46875;
	setAttr ".pv" 0.96875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve360" -p "pSphere2Follicle4696";
createNode nurbsCurve -n "curveShape360" -p "curve360";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere2Follicle5303" -p "hairSystem2Follicles";
createNode follicle -n "pSphere2FollicleShape5303" -p "pSphere2Follicle5303";
	setAttr -k off ".v";
	setAttr ".pu" 0.53125;
	setAttr ".pv" 0.03125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve361" -p "pSphere2Follicle5303";
createNode nurbsCurve -n "curveShape361" -p "curve361";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere2Follicle5309" -p "hairSystem2Follicles";
createNode follicle -n "pSphere2FollicleShape5309" -p "pSphere2Follicle5309";
	setAttr -k off ".v";
	setAttr ".pu" 0.53125;
	setAttr ".pv" 0.09375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve362" -p "pSphere2Follicle5309";
createNode nurbsCurve -n "curveShape362" -p "curve362";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere2Follicle5315" -p "hairSystem2Follicles";
createNode follicle -n "pSphere2FollicleShape5315" -p "pSphere2Follicle5315";
	setAttr -k off ".v";
	setAttr ".pu" 0.53125;
	setAttr ".pv" 0.15625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve363" -p "pSphere2Follicle5315";
createNode nurbsCurve -n "curveShape363" -p "curve363";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere2Follicle5322" -p "hairSystem2Follicles";
createNode follicle -n "pSphere2FollicleShape5322" -p "pSphere2Follicle5322";
	setAttr -k off ".v";
	setAttr ".pu" 0.53125;
	setAttr ".pv" 0.21875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve364" -p "pSphere2Follicle5322";
createNode nurbsCurve -n "curveShape364" -p "curve364";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere2Follicle5328" -p "hairSystem2Follicles";
createNode follicle -n "pSphere2FollicleShape5328" -p "pSphere2Follicle5328";
	setAttr -k off ".v";
	setAttr ".pu" 0.53125;
	setAttr ".pv" 0.28125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve365" -p "pSphere2Follicle5328";
createNode nurbsCurve -n "curveShape365" -p "curve365";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere2Follicle5334" -p "hairSystem2Follicles";
createNode follicle -n "pSphere2FollicleShape5334" -p "pSphere2Follicle5334";
	setAttr -k off ".v";
	setAttr ".pu" 0.53125;
	setAttr ".pv" 0.34375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve366" -p "pSphere2Follicle5334";
createNode nurbsCurve -n "curveShape366" -p "curve366";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere2Follicle5340" -p "hairSystem2Follicles";
createNode follicle -n "pSphere2FollicleShape5340" -p "pSphere2Follicle5340";
	setAttr -k off ".v";
	setAttr ".pu" 0.53125;
	setAttr ".pv" 0.40625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve367" -p "pSphere2Follicle5340";
createNode nurbsCurve -n "curveShape367" -p "curve367";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere2Follicle5346" -p "hairSystem2Follicles";
createNode follicle -n "pSphere2FollicleShape5346" -p "pSphere2Follicle5346";
	setAttr -k off ".v";
	setAttr ".pu" 0.53125;
	setAttr ".pv" 0.46875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve368" -p "pSphere2Follicle5346";
createNode nurbsCurve -n "curveShape368" -p "curve368";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere2Follicle5353" -p "hairSystem2Follicles";
createNode follicle -n "pSphere2FollicleShape5353" -p "pSphere2Follicle5353";
	setAttr -k off ".v";
	setAttr ".pu" 0.53125;
	setAttr ".pv" 0.53125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve369" -p "pSphere2Follicle5353";
createNode nurbsCurve -n "curveShape369" -p "curve369";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere2Follicle5359" -p "hairSystem2Follicles";
createNode follicle -n "pSphere2FollicleShape5359" -p "pSphere2Follicle5359";
	setAttr -k off ".v";
	setAttr ".pu" 0.53125;
	setAttr ".pv" 0.59375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve370" -p "pSphere2Follicle5359";
createNode nurbsCurve -n "curveShape370" -p "curve370";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere2Follicle5365" -p "hairSystem2Follicles";
createNode follicle -n "pSphere2FollicleShape5365" -p "pSphere2Follicle5365";
	setAttr -k off ".v";
	setAttr ".pu" 0.53125;
	setAttr ".pv" 0.65625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve371" -p "pSphere2Follicle5365";
createNode nurbsCurve -n "curveShape371" -p "curve371";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere2Follicle5371" -p "hairSystem2Follicles";
createNode follicle -n "pSphere2FollicleShape5371" -p "pSphere2Follicle5371";
	setAttr -k off ".v";
	setAttr ".pu" 0.53125;
	setAttr ".pv" 0.71875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve372" -p "pSphere2Follicle5371";
createNode nurbsCurve -n "curveShape372" -p "curve372";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere2Follicle5377" -p "hairSystem2Follicles";
createNode follicle -n "pSphere2FollicleShape5377" -p "pSphere2Follicle5377";
	setAttr -k off ".v";
	setAttr ".pu" 0.53125;
	setAttr ".pv" 0.78125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve373" -p "pSphere2Follicle5377";
createNode nurbsCurve -n "curveShape373" -p "curve373";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere2Follicle5384" -p "hairSystem2Follicles";
createNode follicle -n "pSphere2FollicleShape5384" -p "pSphere2Follicle5384";
	setAttr -k off ".v";
	setAttr ".pu" 0.53125;
	setAttr ".pv" 0.84375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve374" -p "pSphere2Follicle5384";
createNode nurbsCurve -n "curveShape374" -p "curve374";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere2Follicle5390" -p "hairSystem2Follicles";
createNode follicle -n "pSphere2FollicleShape5390" -p "pSphere2Follicle5390";
	setAttr -k off ".v";
	setAttr ".pu" 0.53125;
	setAttr ".pv" 0.90625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve375" -p "pSphere2Follicle5390";
createNode nurbsCurve -n "curveShape375" -p "curve375";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere2Follicle5396" -p "hairSystem2Follicles";
createNode follicle -n "pSphere2FollicleShape5396" -p "pSphere2Follicle5396";
	setAttr -k off ".v";
	setAttr ".pu" 0.53125;
	setAttr ".pv" 0.96875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve376" -p "pSphere2Follicle5396";
createNode nurbsCurve -n "curveShape376" -p "curve376";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere2Follicle5909" -p "hairSystem2Follicles";
createNode follicle -n "pSphere2FollicleShape5909" -p "pSphere2Follicle5909";
	setAttr -k off ".v";
	setAttr ".pu" 0.59375;
	setAttr ".pv" 0.09375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve377" -p "pSphere2Follicle5909";
createNode nurbsCurve -n "curveShape377" -p "curve377";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere2Follicle5915" -p "hairSystem2Follicles";
createNode follicle -n "pSphere2FollicleShape5915" -p "pSphere2Follicle5915";
	setAttr -k off ".v";
	setAttr ".pu" 0.59375;
	setAttr ".pv" 0.15625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve378" -p "pSphere2Follicle5915";
createNode nurbsCurve -n "curveShape378" -p "curve378";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere2Follicle5922" -p "hairSystem2Follicles";
createNode follicle -n "pSphere2FollicleShape5922" -p "pSphere2Follicle5922";
	setAttr -k off ".v";
	setAttr ".pu" 0.59375;
	setAttr ".pv" 0.21875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve379" -p "pSphere2Follicle5922";
createNode nurbsCurve -n "curveShape379" -p "curve379";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere2Follicle5928" -p "hairSystem2Follicles";
createNode follicle -n "pSphere2FollicleShape5928" -p "pSphere2Follicle5928";
	setAttr -k off ".v";
	setAttr ".pu" 0.59375;
	setAttr ".pv" 0.28125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve380" -p "pSphere2Follicle5928";
createNode nurbsCurve -n "curveShape380" -p "curve380";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere2Follicle5934" -p "hairSystem2Follicles";
createNode follicle -n "pSphere2FollicleShape5934" -p "pSphere2Follicle5934";
	setAttr -k off ".v";
	setAttr ".pu" 0.59375;
	setAttr ".pv" 0.34375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve381" -p "pSphere2Follicle5934";
createNode nurbsCurve -n "curveShape381" -p "curve381";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere2Follicle5940" -p "hairSystem2Follicles";
createNode follicle -n "pSphere2FollicleShape5940" -p "pSphere2Follicle5940";
	setAttr -k off ".v";
	setAttr ".pu" 0.59375;
	setAttr ".pv" 0.40625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve382" -p "pSphere2Follicle5940";
createNode nurbsCurve -n "curveShape382" -p "curve382";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere2Follicle5946" -p "hairSystem2Follicles";
createNode follicle -n "pSphere2FollicleShape5946" -p "pSphere2Follicle5946";
	setAttr -k off ".v";
	setAttr ".pu" 0.59375;
	setAttr ".pv" 0.46875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve383" -p "pSphere2Follicle5946";
createNode nurbsCurve -n "curveShape383" -p "curve383";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere2Follicle5953" -p "hairSystem2Follicles";
createNode follicle -n "pSphere2FollicleShape5953" -p "pSphere2Follicle5953";
	setAttr -k off ".v";
	setAttr ".pu" 0.59375;
	setAttr ".pv" 0.53125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve384" -p "pSphere2Follicle5953";
createNode nurbsCurve -n "curveShape384" -p "curve384";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere2Follicle5959" -p "hairSystem2Follicles";
createNode follicle -n "pSphere2FollicleShape5959" -p "pSphere2Follicle5959";
	setAttr -k off ".v";
	setAttr ".pu" 0.59375;
	setAttr ".pv" 0.59375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve385" -p "pSphere2Follicle5959";
createNode nurbsCurve -n "curveShape385" -p "curve385";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere2Follicle5965" -p "hairSystem2Follicles";
createNode follicle -n "pSphere2FollicleShape5965" -p "pSphere2Follicle5965";
	setAttr -k off ".v";
	setAttr ".pu" 0.59375;
	setAttr ".pv" 0.65625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve386" -p "pSphere2Follicle5965";
createNode nurbsCurve -n "curveShape386" -p "curve386";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere2Follicle5971" -p "hairSystem2Follicles";
createNode follicle -n "pSphere2FollicleShape5971" -p "pSphere2Follicle5971";
	setAttr -k off ".v";
	setAttr ".pu" 0.59375;
	setAttr ".pv" 0.71875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve387" -p "pSphere2Follicle5971";
createNode nurbsCurve -n "curveShape387" -p "curve387";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere2Follicle5977" -p "hairSystem2Follicles";
createNode follicle -n "pSphere2FollicleShape5977" -p "pSphere2Follicle5977";
	setAttr -k off ".v";
	setAttr ".pu" 0.59375;
	setAttr ".pv" 0.78125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve388" -p "pSphere2Follicle5977";
createNode nurbsCurve -n "curveShape388" -p "curve388";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere2Follicle5984" -p "hairSystem2Follicles";
createNode follicle -n "pSphere2FollicleShape5984" -p "pSphere2Follicle5984";
	setAttr -k off ".v";
	setAttr ".pu" 0.59375;
	setAttr ".pv" 0.84375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve389" -p "pSphere2Follicle5984";
createNode nurbsCurve -n "curveShape389" -p "curve389";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere2Follicle5990" -p "hairSystem2Follicles";
createNode follicle -n "pSphere2FollicleShape5990" -p "pSphere2Follicle5990";
	setAttr -k off ".v";
	setAttr ".pu" 0.59375;
	setAttr ".pv" 0.90625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve390" -p "pSphere2Follicle5990";
createNode nurbsCurve -n "curveShape390" -p "curve390";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere2Follicle6509" -p "hairSystem2Follicles";
createNode follicle -n "pSphere2FollicleShape6509" -p "pSphere2Follicle6509";
	setAttr -k off ".v";
	setAttr ".pu" 0.65625;
	setAttr ".pv" 0.09375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve391" -p "pSphere2Follicle6509";
createNode nurbsCurve -n "curveShape391" -p "curve391";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere2Follicle6515" -p "hairSystem2Follicles";
createNode follicle -n "pSphere2FollicleShape6515" -p "pSphere2Follicle6515";
	setAttr -k off ".v";
	setAttr ".pu" 0.65625;
	setAttr ".pv" 0.15625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve392" -p "pSphere2Follicle6515";
createNode nurbsCurve -n "curveShape392" -p "curve392";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere2Follicle6522" -p "hairSystem2Follicles";
createNode follicle -n "pSphere2FollicleShape6522" -p "pSphere2Follicle6522";
	setAttr -k off ".v";
	setAttr ".pu" 0.65625;
	setAttr ".pv" 0.21875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve393" -p "pSphere2Follicle6522";
createNode nurbsCurve -n "curveShape393" -p "curve393";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere2Follicle6528" -p "hairSystem2Follicles";
createNode follicle -n "pSphere2FollicleShape6528" -p "pSphere2Follicle6528";
	setAttr -k off ".v";
	setAttr ".pu" 0.65625;
	setAttr ".pv" 0.28125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve394" -p "pSphere2Follicle6528";
createNode nurbsCurve -n "curveShape394" -p "curve394";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere2Follicle6534" -p "hairSystem2Follicles";
createNode follicle -n "pSphere2FollicleShape6534" -p "pSphere2Follicle6534";
	setAttr -k off ".v";
	setAttr ".pu" 0.65625;
	setAttr ".pv" 0.34375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve395" -p "pSphere2Follicle6534";
createNode nurbsCurve -n "curveShape395" -p "curve395";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere2Follicle6540" -p "hairSystem2Follicles";
createNode follicle -n "pSphere2FollicleShape6540" -p "pSphere2Follicle6540";
	setAttr -k off ".v";
	setAttr ".pu" 0.65625;
	setAttr ".pv" 0.40625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve396" -p "pSphere2Follicle6540";
createNode nurbsCurve -n "curveShape396" -p "curve396";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere2Follicle6546" -p "hairSystem2Follicles";
createNode follicle -n "pSphere2FollicleShape6546" -p "pSphere2Follicle6546";
	setAttr -k off ".v";
	setAttr ".pu" 0.65625;
	setAttr ".pv" 0.46875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve397" -p "pSphere2Follicle6546";
createNode nurbsCurve -n "curveShape397" -p "curve397";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere2Follicle6553" -p "hairSystem2Follicles";
createNode follicle -n "pSphere2FollicleShape6553" -p "pSphere2Follicle6553";
	setAttr -k off ".v";
	setAttr ".pu" 0.65625;
	setAttr ".pv" 0.53125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve398" -p "pSphere2Follicle6553";
createNode nurbsCurve -n "curveShape398" -p "curve398";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere2Follicle6559" -p "hairSystem2Follicles";
createNode follicle -n "pSphere2FollicleShape6559" -p "pSphere2Follicle6559";
	setAttr -k off ".v";
	setAttr ".pu" 0.65625;
	setAttr ".pv" 0.59375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve399" -p "pSphere2Follicle6559";
createNode nurbsCurve -n "curveShape399" -p "curve399";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere2Follicle6565" -p "hairSystem2Follicles";
createNode follicle -n "pSphere2FollicleShape6565" -p "pSphere2Follicle6565";
	setAttr -k off ".v";
	setAttr ".pu" 0.65625;
	setAttr ".pv" 0.65625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve400" -p "pSphere2Follicle6565";
createNode nurbsCurve -n "curveShape400" -p "curve400";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere2Follicle6571" -p "hairSystem2Follicles";
createNode follicle -n "pSphere2FollicleShape6571" -p "pSphere2Follicle6571";
	setAttr -k off ".v";
	setAttr ".pu" 0.65625;
	setAttr ".pv" 0.71875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve401" -p "pSphere2Follicle6571";
createNode nurbsCurve -n "curveShape401" -p "curve401";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere2Follicle6577" -p "hairSystem2Follicles";
createNode follicle -n "pSphere2FollicleShape6577" -p "pSphere2Follicle6577";
	setAttr -k off ".v";
	setAttr ".pu" 0.65625;
	setAttr ".pv" 0.78125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve402" -p "pSphere2Follicle6577";
createNode nurbsCurve -n "curveShape402" -p "curve402";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere2Follicle6584" -p "hairSystem2Follicles";
createNode follicle -n "pSphere2FollicleShape6584" -p "pSphere2Follicle6584";
	setAttr -k off ".v";
	setAttr ".pu" 0.65625;
	setAttr ".pv" 0.84375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve403" -p "pSphere2Follicle6584";
createNode nurbsCurve -n "curveShape403" -p "curve403";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere2Follicle6590" -p "hairSystem2Follicles";
createNode follicle -n "pSphere2FollicleShape6590" -p "pSphere2Follicle6590";
	setAttr -k off ".v";
	setAttr ".pu" 0.65625;
	setAttr ".pv" 0.90625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve404" -p "pSphere2Follicle6590";
createNode nurbsCurve -n "curveShape404" -p "curve404";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere2Follicle7103" -p "hairSystem2Follicles";
createNode follicle -n "pSphere2FollicleShape7103" -p "pSphere2Follicle7103";
	setAttr -k off ".v";
	setAttr ".pu" 0.71875;
	setAttr ".pv" 0.03125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve405" -p "pSphere2Follicle7103";
createNode nurbsCurve -n "curveShape405" -p "curve405";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere2Follicle7109" -p "hairSystem2Follicles";
createNode follicle -n "pSphere2FollicleShape7109" -p "pSphere2Follicle7109";
	setAttr -k off ".v";
	setAttr ".pu" 0.71875;
	setAttr ".pv" 0.09375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve406" -p "pSphere2Follicle7109";
createNode nurbsCurve -n "curveShape406" -p "curve406";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere2Follicle7115" -p "hairSystem2Follicles";
createNode follicle -n "pSphere2FollicleShape7115" -p "pSphere2Follicle7115";
	setAttr -k off ".v";
	setAttr ".pu" 0.71875;
	setAttr ".pv" 0.15625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve407" -p "pSphere2Follicle7115";
createNode nurbsCurve -n "curveShape407" -p "curve407";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere2Follicle7122" -p "hairSystem2Follicles";
createNode follicle -n "pSphere2FollicleShape7122" -p "pSphere2Follicle7122";
	setAttr -k off ".v";
	setAttr ".pu" 0.71875;
	setAttr ".pv" 0.21875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve408" -p "pSphere2Follicle7122";
createNode nurbsCurve -n "curveShape408" -p "curve408";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere2Follicle7128" -p "hairSystem2Follicles";
createNode follicle -n "pSphere2FollicleShape7128" -p "pSphere2Follicle7128";
	setAttr -k off ".v";
	setAttr ".pu" 0.71875;
	setAttr ".pv" 0.28125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve409" -p "pSphere2Follicle7128";
createNode nurbsCurve -n "curveShape409" -p "curve409";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere2Follicle7134" -p "hairSystem2Follicles";
createNode follicle -n "pSphere2FollicleShape7134" -p "pSphere2Follicle7134";
	setAttr -k off ".v";
	setAttr ".pu" 0.71875;
	setAttr ".pv" 0.34375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve410" -p "pSphere2Follicle7134";
createNode nurbsCurve -n "curveShape410" -p "curve410";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere2Follicle7140" -p "hairSystem2Follicles";
createNode follicle -n "pSphere2FollicleShape7140" -p "pSphere2Follicle7140";
	setAttr -k off ".v";
	setAttr ".pu" 0.71875;
	setAttr ".pv" 0.40625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve411" -p "pSphere2Follicle7140";
createNode nurbsCurve -n "curveShape411" -p "curve411";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere2Follicle7146" -p "hairSystem2Follicles";
createNode follicle -n "pSphere2FollicleShape7146" -p "pSphere2Follicle7146";
	setAttr -k off ".v";
	setAttr ".pu" 0.71875;
	setAttr ".pv" 0.46875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve412" -p "pSphere2Follicle7146";
createNode nurbsCurve -n "curveShape412" -p "curve412";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere2Follicle7153" -p "hairSystem2Follicles";
createNode follicle -n "pSphere2FollicleShape7153" -p "pSphere2Follicle7153";
	setAttr -k off ".v";
	setAttr ".pu" 0.71875;
	setAttr ".pv" 0.53125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve413" -p "pSphere2Follicle7153";
createNode nurbsCurve -n "curveShape413" -p "curve413";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere2Follicle7159" -p "hairSystem2Follicles";
createNode follicle -n "pSphere2FollicleShape7159" -p "pSphere2Follicle7159";
	setAttr -k off ".v";
	setAttr ".pu" 0.71875;
	setAttr ".pv" 0.59375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve414" -p "pSphere2Follicle7159";
createNode nurbsCurve -n "curveShape414" -p "curve414";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere2Follicle7165" -p "hairSystem2Follicles";
createNode follicle -n "pSphere2FollicleShape7165" -p "pSphere2Follicle7165";
	setAttr -k off ".v";
	setAttr ".pu" 0.71875;
	setAttr ".pv" 0.65625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve415" -p "pSphere2Follicle7165";
createNode nurbsCurve -n "curveShape415" -p "curve415";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere2Follicle7171" -p "hairSystem2Follicles";
createNode follicle -n "pSphere2FollicleShape7171" -p "pSphere2Follicle7171";
	setAttr -k off ".v";
	setAttr ".pu" 0.71875;
	setAttr ".pv" 0.71875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve416" -p "pSphere2Follicle7171";
createNode nurbsCurve -n "curveShape416" -p "curve416";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere2Follicle7177" -p "hairSystem2Follicles";
createNode follicle -n "pSphere2FollicleShape7177" -p "pSphere2Follicle7177";
	setAttr -k off ".v";
	setAttr ".pu" 0.71875;
	setAttr ".pv" 0.78125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve417" -p "pSphere2Follicle7177";
createNode nurbsCurve -n "curveShape417" -p "curve417";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere2Follicle7184" -p "hairSystem2Follicles";
createNode follicle -n "pSphere2FollicleShape7184" -p "pSphere2Follicle7184";
	setAttr -k off ".v";
	setAttr ".pu" 0.71875;
	setAttr ".pv" 0.84375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve418" -p "pSphere2Follicle7184";
createNode nurbsCurve -n "curveShape418" -p "curve418";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere2Follicle7190" -p "hairSystem2Follicles";
createNode follicle -n "pSphere2FollicleShape7190" -p "pSphere2Follicle7190";
	setAttr -k off ".v";
	setAttr ".pu" 0.71875;
	setAttr ".pv" 0.90625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve419" -p "pSphere2Follicle7190";
createNode nurbsCurve -n "curveShape419" -p "curve419";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere2Follicle7196" -p "hairSystem2Follicles";
createNode follicle -n "pSphere2FollicleShape7196" -p "pSphere2Follicle7196";
	setAttr -k off ".v";
	setAttr ".pu" 0.71875;
	setAttr ".pv" 0.96875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve420" -p "pSphere2Follicle7196";
createNode nurbsCurve -n "curveShape420" -p "curve420";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere2Follicle7703" -p "hairSystem2Follicles";
createNode follicle -n "pSphere2FollicleShape7703" -p "pSphere2Follicle7703";
	setAttr -k off ".v";
	setAttr ".pu" 0.78125;
	setAttr ".pv" 0.03125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve421" -p "pSphere2Follicle7703";
createNode nurbsCurve -n "curveShape421" -p "curve421";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere2Follicle7709" -p "hairSystem2Follicles";
createNode follicle -n "pSphere2FollicleShape7709" -p "pSphere2Follicle7709";
	setAttr -k off ".v";
	setAttr ".pu" 0.78125;
	setAttr ".pv" 0.09375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve422" -p "pSphere2Follicle7709";
createNode nurbsCurve -n "curveShape422" -p "curve422";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere2Follicle7715" -p "hairSystem2Follicles";
createNode follicle -n "pSphere2FollicleShape7715" -p "pSphere2Follicle7715";
	setAttr -k off ".v";
	setAttr ".pu" 0.78125;
	setAttr ".pv" 0.15625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve423" -p "pSphere2Follicle7715";
createNode nurbsCurve -n "curveShape423" -p "curve423";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere2Follicle7722" -p "hairSystem2Follicles";
createNode follicle -n "pSphere2FollicleShape7722" -p "pSphere2Follicle7722";
	setAttr -k off ".v";
	setAttr ".pu" 0.78125;
	setAttr ".pv" 0.21875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve424" -p "pSphere2Follicle7722";
createNode nurbsCurve -n "curveShape424" -p "curve424";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere2Follicle7728" -p "hairSystem2Follicles";
createNode follicle -n "pSphere2FollicleShape7728" -p "pSphere2Follicle7728";
	setAttr -k off ".v";
	setAttr ".pu" 0.78125;
	setAttr ".pv" 0.28125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve425" -p "pSphere2Follicle7728";
createNode nurbsCurve -n "curveShape425" -p "curve425";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere2Follicle7734" -p "hairSystem2Follicles";
createNode follicle -n "pSphere2FollicleShape7734" -p "pSphere2Follicle7734";
	setAttr -k off ".v";
	setAttr ".pu" 0.78125;
	setAttr ".pv" 0.34375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve426" -p "pSphere2Follicle7734";
createNode nurbsCurve -n "curveShape426" -p "curve426";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere2Follicle7740" -p "hairSystem2Follicles";
createNode follicle -n "pSphere2FollicleShape7740" -p "pSphere2Follicle7740";
	setAttr -k off ".v";
	setAttr ".pu" 0.78125;
	setAttr ".pv" 0.40625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve427" -p "pSphere2Follicle7740";
createNode nurbsCurve -n "curveShape427" -p "curve427";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere2Follicle7746" -p "hairSystem2Follicles";
createNode follicle -n "pSphere2FollicleShape7746" -p "pSphere2Follicle7746";
	setAttr -k off ".v";
	setAttr ".pu" 0.78125;
	setAttr ".pv" 0.46875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve428" -p "pSphere2Follicle7746";
createNode nurbsCurve -n "curveShape428" -p "curve428";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere2Follicle7753" -p "hairSystem2Follicles";
createNode follicle -n "pSphere2FollicleShape7753" -p "pSphere2Follicle7753";
	setAttr -k off ".v";
	setAttr ".pu" 0.78125;
	setAttr ".pv" 0.53125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve429" -p "pSphere2Follicle7753";
createNode nurbsCurve -n "curveShape429" -p "curve429";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere2Follicle7759" -p "hairSystem2Follicles";
createNode follicle -n "pSphere2FollicleShape7759" -p "pSphere2Follicle7759";
	setAttr -k off ".v";
	setAttr ".pu" 0.78125;
	setAttr ".pv" 0.59375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve430" -p "pSphere2Follicle7759";
createNode nurbsCurve -n "curveShape430" -p "curve430";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere2Follicle7765" -p "hairSystem2Follicles";
createNode follicle -n "pSphere2FollicleShape7765" -p "pSphere2Follicle7765";
	setAttr -k off ".v";
	setAttr ".pu" 0.78125;
	setAttr ".pv" 0.65625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve431" -p "pSphere2Follicle7765";
createNode nurbsCurve -n "curveShape431" -p "curve431";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere2Follicle7771" -p "hairSystem2Follicles";
createNode follicle -n "pSphere2FollicleShape7771" -p "pSphere2Follicle7771";
	setAttr -k off ".v";
	setAttr ".pu" 0.78125;
	setAttr ".pv" 0.71875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve432" -p "pSphere2Follicle7771";
createNode nurbsCurve -n "curveShape432" -p "curve432";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere2Follicle7777" -p "hairSystem2Follicles";
createNode follicle -n "pSphere2FollicleShape7777" -p "pSphere2Follicle7777";
	setAttr -k off ".v";
	setAttr ".pu" 0.78125;
	setAttr ".pv" 0.78125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve433" -p "pSphere2Follicle7777";
createNode nurbsCurve -n "curveShape433" -p "curve433";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere2Follicle7784" -p "hairSystem2Follicles";
createNode follicle -n "pSphere2FollicleShape7784" -p "pSphere2Follicle7784";
	setAttr -k off ".v";
	setAttr ".pu" 0.78125;
	setAttr ".pv" 0.84375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve434" -p "pSphere2Follicle7784";
createNode nurbsCurve -n "curveShape434" -p "curve434";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere2Follicle7790" -p "hairSystem2Follicles";
createNode follicle -n "pSphere2FollicleShape7790" -p "pSphere2Follicle7790";
	setAttr -k off ".v";
	setAttr ".pu" 0.78125;
	setAttr ".pv" 0.90625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve435" -p "pSphere2Follicle7790";
createNode nurbsCurve -n "curveShape435" -p "curve435";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere2Follicle7796" -p "hairSystem2Follicles";
createNode follicle -n "pSphere2FollicleShape7796" -p "pSphere2Follicle7796";
	setAttr -k off ".v";
	setAttr ".pu" 0.78125;
	setAttr ".pv" 0.96875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve436" -p "pSphere2Follicle7796";
createNode nurbsCurve -n "curveShape436" -p "curve436";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere2Follicle8409" -p "hairSystem2Follicles";
createNode follicle -n "pSphere2FollicleShape8409" -p "pSphere2Follicle8409";
	setAttr -k off ".v";
	setAttr ".pu" 0.84375;
	setAttr ".pv" 0.09375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve437" -p "pSphere2Follicle8409";
createNode nurbsCurve -n "curveShape437" -p "curve437";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere2Follicle8415" -p "hairSystem2Follicles";
createNode follicle -n "pSphere2FollicleShape8415" -p "pSphere2Follicle8415";
	setAttr -k off ".v";
	setAttr ".pu" 0.84375;
	setAttr ".pv" 0.15625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve438" -p "pSphere2Follicle8415";
createNode nurbsCurve -n "curveShape438" -p "curve438";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere2Follicle8422" -p "hairSystem2Follicles";
createNode follicle -n "pSphere2FollicleShape8422" -p "pSphere2Follicle8422";
	setAttr -k off ".v";
	setAttr ".pu" 0.84375;
	setAttr ".pv" 0.21875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve439" -p "pSphere2Follicle8422";
createNode nurbsCurve -n "curveShape439" -p "curve439";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere2Follicle8428" -p "hairSystem2Follicles";
createNode follicle -n "pSphere2FollicleShape8428" -p "pSphere2Follicle8428";
	setAttr -k off ".v";
	setAttr ".pu" 0.84375;
	setAttr ".pv" 0.28125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve440" -p "pSphere2Follicle8428";
createNode nurbsCurve -n "curveShape440" -p "curve440";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere2Follicle8434" -p "hairSystem2Follicles";
createNode follicle -n "pSphere2FollicleShape8434" -p "pSphere2Follicle8434";
	setAttr -k off ".v";
	setAttr ".pu" 0.84375;
	setAttr ".pv" 0.34375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve441" -p "pSphere2Follicle8434";
createNode nurbsCurve -n "curveShape441" -p "curve441";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere2Follicle8440" -p "hairSystem2Follicles";
createNode follicle -n "pSphere2FollicleShape8440" -p "pSphere2Follicle8440";
	setAttr -k off ".v";
	setAttr ".pu" 0.84375;
	setAttr ".pv" 0.40625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve442" -p "pSphere2Follicle8440";
createNode nurbsCurve -n "curveShape442" -p "curve442";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere2Follicle8446" -p "hairSystem2Follicles";
createNode follicle -n "pSphere2FollicleShape8446" -p "pSphere2Follicle8446";
	setAttr -k off ".v";
	setAttr ".pu" 0.84375;
	setAttr ".pv" 0.46875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve443" -p "pSphere2Follicle8446";
createNode nurbsCurve -n "curveShape443" -p "curve443";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere2Follicle8453" -p "hairSystem2Follicles";
createNode follicle -n "pSphere2FollicleShape8453" -p "pSphere2Follicle8453";
	setAttr -k off ".v";
	setAttr ".pu" 0.84375;
	setAttr ".pv" 0.53125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve444" -p "pSphere2Follicle8453";
createNode nurbsCurve -n "curveShape444" -p "curve444";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere2Follicle8459" -p "hairSystem2Follicles";
createNode follicle -n "pSphere2FollicleShape8459" -p "pSphere2Follicle8459";
	setAttr -k off ".v";
	setAttr ".pu" 0.84375;
	setAttr ".pv" 0.59375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve445" -p "pSphere2Follicle8459";
createNode nurbsCurve -n "curveShape445" -p "curve445";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere2Follicle8465" -p "hairSystem2Follicles";
createNode follicle -n "pSphere2FollicleShape8465" -p "pSphere2Follicle8465";
	setAttr -k off ".v";
	setAttr ".pu" 0.84375;
	setAttr ".pv" 0.65625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve446" -p "pSphere2Follicle8465";
createNode nurbsCurve -n "curveShape446" -p "curve446";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere2Follicle8471" -p "hairSystem2Follicles";
createNode follicle -n "pSphere2FollicleShape8471" -p "pSphere2Follicle8471";
	setAttr -k off ".v";
	setAttr ".pu" 0.84375;
	setAttr ".pv" 0.71875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve447" -p "pSphere2Follicle8471";
createNode nurbsCurve -n "curveShape447" -p "curve447";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere2Follicle8477" -p "hairSystem2Follicles";
createNode follicle -n "pSphere2FollicleShape8477" -p "pSphere2Follicle8477";
	setAttr -k off ".v";
	setAttr ".pu" 0.84375;
	setAttr ".pv" 0.78125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve448" -p "pSphere2Follicle8477";
createNode nurbsCurve -n "curveShape448" -p "curve448";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere2Follicle8484" -p "hairSystem2Follicles";
createNode follicle -n "pSphere2FollicleShape8484" -p "pSphere2Follicle8484";
	setAttr -k off ".v";
	setAttr ".pu" 0.84375;
	setAttr ".pv" 0.84375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve449" -p "pSphere2Follicle8484";
createNode nurbsCurve -n "curveShape449" -p "curve449";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere2Follicle8490" -p "hairSystem2Follicles";
createNode follicle -n "pSphere2FollicleShape8490" -p "pSphere2Follicle8490";
	setAttr -k off ".v";
	setAttr ".pu" 0.84375;
	setAttr ".pv" 0.90625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve450" -p "pSphere2Follicle8490";
createNode nurbsCurve -n "curveShape450" -p "curve450";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere2Follicle9009" -p "hairSystem2Follicles";
createNode follicle -n "pSphere2FollicleShape9009" -p "pSphere2Follicle9009";
	setAttr -k off ".v";
	setAttr ".pu" 0.90625;
	setAttr ".pv" 0.09375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve451" -p "pSphere2Follicle9009";
createNode nurbsCurve -n "curveShape451" -p "curve451";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere2Follicle9015" -p "hairSystem2Follicles";
createNode follicle -n "pSphere2FollicleShape9015" -p "pSphere2Follicle9015";
	setAttr -k off ".v";
	setAttr ".pu" 0.90625;
	setAttr ".pv" 0.15625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve452" -p "pSphere2Follicle9015";
createNode nurbsCurve -n "curveShape452" -p "curve452";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere2Follicle9022" -p "hairSystem2Follicles";
createNode follicle -n "pSphere2FollicleShape9022" -p "pSphere2Follicle9022";
	setAttr -k off ".v";
	setAttr ".pu" 0.90625;
	setAttr ".pv" 0.21875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve453" -p "pSphere2Follicle9022";
createNode nurbsCurve -n "curveShape453" -p "curve453";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere2Follicle9028" -p "hairSystem2Follicles";
createNode follicle -n "pSphere2FollicleShape9028" -p "pSphere2Follicle9028";
	setAttr -k off ".v";
	setAttr ".pu" 0.90625;
	setAttr ".pv" 0.28125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve454" -p "pSphere2Follicle9028";
createNode nurbsCurve -n "curveShape454" -p "curve454";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere2Follicle9034" -p "hairSystem2Follicles";
createNode follicle -n "pSphere2FollicleShape9034" -p "pSphere2Follicle9034";
	setAttr -k off ".v";
	setAttr ".pu" 0.90625;
	setAttr ".pv" 0.34375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve455" -p "pSphere2Follicle9034";
createNode nurbsCurve -n "curveShape455" -p "curve455";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere2Follicle9040" -p "hairSystem2Follicles";
createNode follicle -n "pSphere2FollicleShape9040" -p "pSphere2Follicle9040";
	setAttr -k off ".v";
	setAttr ".pu" 0.90625;
	setAttr ".pv" 0.40625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve456" -p "pSphere2Follicle9040";
createNode nurbsCurve -n "curveShape456" -p "curve456";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere2Follicle9046" -p "hairSystem2Follicles";
createNode follicle -n "pSphere2FollicleShape9046" -p "pSphere2Follicle9046";
	setAttr -k off ".v";
	setAttr ".pu" 0.90625;
	setAttr ".pv" 0.46875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve457" -p "pSphere2Follicle9046";
createNode nurbsCurve -n "curveShape457" -p "curve457";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere2Follicle9053" -p "hairSystem2Follicles";
createNode follicle -n "pSphere2FollicleShape9053" -p "pSphere2Follicle9053";
	setAttr -k off ".v";
	setAttr ".pu" 0.90625;
	setAttr ".pv" 0.53125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve458" -p "pSphere2Follicle9053";
createNode nurbsCurve -n "curveShape458" -p "curve458";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere2Follicle9059" -p "hairSystem2Follicles";
createNode follicle -n "pSphere2FollicleShape9059" -p "pSphere2Follicle9059";
	setAttr -k off ".v";
	setAttr ".pu" 0.90625;
	setAttr ".pv" 0.59375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve459" -p "pSphere2Follicle9059";
createNode nurbsCurve -n "curveShape459" -p "curve459";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere2Follicle9065" -p "hairSystem2Follicles";
createNode follicle -n "pSphere2FollicleShape9065" -p "pSphere2Follicle9065";
	setAttr -k off ".v";
	setAttr ".pu" 0.90625;
	setAttr ".pv" 0.65625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve460" -p "pSphere2Follicle9065";
createNode nurbsCurve -n "curveShape460" -p "curve460";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere2Follicle9071" -p "hairSystem2Follicles";
createNode follicle -n "pSphere2FollicleShape9071" -p "pSphere2Follicle9071";
	setAttr -k off ".v";
	setAttr ".pu" 0.90625;
	setAttr ".pv" 0.71875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve461" -p "pSphere2Follicle9071";
createNode nurbsCurve -n "curveShape461" -p "curve461";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere2Follicle9077" -p "hairSystem2Follicles";
createNode follicle -n "pSphere2FollicleShape9077" -p "pSphere2Follicle9077";
	setAttr -k off ".v";
	setAttr ".pu" 0.90625;
	setAttr ".pv" 0.78125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve462" -p "pSphere2Follicle9077";
createNode nurbsCurve -n "curveShape462" -p "curve462";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere2Follicle9084" -p "hairSystem2Follicles";
createNode follicle -n "pSphere2FollicleShape9084" -p "pSphere2Follicle9084";
	setAttr -k off ".v";
	setAttr ".pu" 0.90625;
	setAttr ".pv" 0.84375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve463" -p "pSphere2Follicle9084";
createNode nurbsCurve -n "curveShape463" -p "curve463";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere2Follicle9090" -p "hairSystem2Follicles";
createNode follicle -n "pSphere2FollicleShape9090" -p "pSphere2Follicle9090";
	setAttr -k off ".v";
	setAttr ".pu" 0.90625;
	setAttr ".pv" 0.90625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve464" -p "pSphere2Follicle9090";
createNode nurbsCurve -n "curveShape464" -p "curve464";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere2Follicle9603" -p "hairSystem2Follicles";
createNode follicle -n "pSphere2FollicleShape9603" -p "pSphere2Follicle9603";
	setAttr -k off ".v";
	setAttr ".pu" 0.96875;
	setAttr ".pv" 0.03125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve465" -p "pSphere2Follicle9603";
createNode nurbsCurve -n "curveShape465" -p "curve465";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere2Follicle9609" -p "hairSystem2Follicles";
createNode follicle -n "pSphere2FollicleShape9609" -p "pSphere2Follicle9609";
	setAttr -k off ".v";
	setAttr ".pu" 0.96875;
	setAttr ".pv" 0.09375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve466" -p "pSphere2Follicle9609";
createNode nurbsCurve -n "curveShape466" -p "curve466";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere2Follicle9615" -p "hairSystem2Follicles";
createNode follicle -n "pSphere2FollicleShape9615" -p "pSphere2Follicle9615";
	setAttr -k off ".v";
	setAttr ".pu" 0.96875;
	setAttr ".pv" 0.15625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve467" -p "pSphere2Follicle9615";
createNode nurbsCurve -n "curveShape467" -p "curve467";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere2Follicle9622" -p "hairSystem2Follicles";
createNode follicle -n "pSphere2FollicleShape9622" -p "pSphere2Follicle9622";
	setAttr -k off ".v";
	setAttr ".pu" 0.96875;
	setAttr ".pv" 0.21875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve468" -p "pSphere2Follicle9622";
createNode nurbsCurve -n "curveShape468" -p "curve468";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere2Follicle9628" -p "hairSystem2Follicles";
createNode follicle -n "pSphere2FollicleShape9628" -p "pSphere2Follicle9628";
	setAttr -k off ".v";
	setAttr ".pu" 0.96875;
	setAttr ".pv" 0.28125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve469" -p "pSphere2Follicle9628";
createNode nurbsCurve -n "curveShape469" -p "curve469";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere2Follicle9634" -p "hairSystem2Follicles";
createNode follicle -n "pSphere2FollicleShape9634" -p "pSphere2Follicle9634";
	setAttr -k off ".v";
	setAttr ".pu" 0.96875;
	setAttr ".pv" 0.34375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve470" -p "pSphere2Follicle9634";
createNode nurbsCurve -n "curveShape470" -p "curve470";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere2Follicle9640" -p "hairSystem2Follicles";
createNode follicle -n "pSphere2FollicleShape9640" -p "pSphere2Follicle9640";
	setAttr -k off ".v";
	setAttr ".pu" 0.96875;
	setAttr ".pv" 0.40625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve471" -p "pSphere2Follicle9640";
createNode nurbsCurve -n "curveShape471" -p "curve471";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere2Follicle9646" -p "hairSystem2Follicles";
createNode follicle -n "pSphere2FollicleShape9646" -p "pSphere2Follicle9646";
	setAttr -k off ".v";
	setAttr ".pu" 0.96875;
	setAttr ".pv" 0.46875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve472" -p "pSphere2Follicle9646";
createNode nurbsCurve -n "curveShape472" -p "curve472";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere2Follicle9653" -p "hairSystem2Follicles";
createNode follicle -n "pSphere2FollicleShape9653" -p "pSphere2Follicle9653";
	setAttr -k off ".v";
	setAttr ".pu" 0.96875;
	setAttr ".pv" 0.53125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve473" -p "pSphere2Follicle9653";
createNode nurbsCurve -n "curveShape473" -p "curve473";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere2Follicle9659" -p "hairSystem2Follicles";
createNode follicle -n "pSphere2FollicleShape9659" -p "pSphere2Follicle9659";
	setAttr -k off ".v";
	setAttr ".pu" 0.96875;
	setAttr ".pv" 0.59375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve474" -p "pSphere2Follicle9659";
createNode nurbsCurve -n "curveShape474" -p "curve474";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere2Follicle9665" -p "hairSystem2Follicles";
createNode follicle -n "pSphere2FollicleShape9665" -p "pSphere2Follicle9665";
	setAttr -k off ".v";
	setAttr ".pu" 0.96875;
	setAttr ".pv" 0.65625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve475" -p "pSphere2Follicle9665";
createNode nurbsCurve -n "curveShape475" -p "curve475";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere2Follicle9671" -p "hairSystem2Follicles";
createNode follicle -n "pSphere2FollicleShape9671" -p "pSphere2Follicle9671";
	setAttr -k off ".v";
	setAttr ".pu" 0.96875;
	setAttr ".pv" 0.71875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve476" -p "pSphere2Follicle9671";
createNode nurbsCurve -n "curveShape476" -p "curve476";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere2Follicle9677" -p "hairSystem2Follicles";
createNode follicle -n "pSphere2FollicleShape9677" -p "pSphere2Follicle9677";
	setAttr -k off ".v";
	setAttr ".pu" 0.96875;
	setAttr ".pv" 0.78125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve477" -p "pSphere2Follicle9677";
createNode nurbsCurve -n "curveShape477" -p "curve477";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere2Follicle9684" -p "hairSystem2Follicles";
createNode follicle -n "pSphere2FollicleShape9684" -p "pSphere2Follicle9684";
	setAttr -k off ".v";
	setAttr ".pu" 0.96875;
	setAttr ".pv" 0.84375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve478" -p "pSphere2Follicle9684";
createNode nurbsCurve -n "curveShape478" -p "curve478";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere2Follicle9690" -p "hairSystem2Follicles";
createNode follicle -n "pSphere2FollicleShape9690" -p "pSphere2Follicle9690";
	setAttr -k off ".v";
	setAttr ".pu" 0.96875;
	setAttr ".pv" 0.90625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve479" -p "pSphere2Follicle9690";
createNode nurbsCurve -n "curveShape479" -p "curve479";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pSphere2Follicle9696" -p "hairSystem2Follicles";
createNode follicle -n "pSphere2FollicleShape9696" -p "pSphere2Follicle9696";
	setAttr -k off ".v";
	setAttr ".pu" 0.96875;
	setAttr ".pv" 0.96875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve480" -p "pSphere2Follicle9696";
createNode nurbsCurve -n "curveShape480" -p "curve480";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
		0 0 0.44444444440000003
		0 0 0.66666666669999997
		0 0 0.88888888889999995
		0 0 1.111111111
		0 0 1.3333333329999999
		0 0 1.5555555560000001
		0 0 1.7777777779999999
		0 0 2
		;
createNode transform -n "pfxHair2";
createNode pfxHair -n "pfxHairShape2" -p "pfxHair2";
	setAttr -k off ".v";
	setAttr ".dpc" 100;
	setAttr ".dam" no;
createNode transform -n "directionalLight1";
	setAttr ".r" -type "double3" 36.395093627554211 109.71510970921861 84.947632226673704 ;
	setAttr ".s" -type "double3" 5.5144173827393006 5.5144173827393006 5.5144173827393006 ;
createNode directionalLight -n "directionalLightShape1" -p "directionalLight1";
	addAttr -ci true -sn "ai_cast_shadows" -ln "aiCastShadows" -dv 1 -min 0 -max 1 
		-at "bool";
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
createNode lightLinker -s -n "lightLinker1";
	setAttr -s 7 ".lnk";
	setAttr -s 7 ".slnk";
createNode displayLayerManager -n "layerManager";
createNode displayLayer -n "defaultLayer";
createNode renderLayerManager -n "renderLayerManager";
createNode renderLayer -n "defaultRenderLayer";
	setAttr ".g" yes;
createNode aiOptions -s -n "defaultArnoldRenderOptions";
	setAttr ".dgamma" 1;
	setAttr ".lgamma" 1;
	setAttr ".sgamma" 1;
	setAttr ".tgamma" 1;
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
		+ "            -dynamicConstraints 1\n            -locators 1\n            -manipulators 1\n            -dimensions 1\n            -handles 1\n            -pivots 1\n            -textures 1\n            -strokes 1\n            -shadows 0\n            $editorName;\nmodelEditor -e -viewSelected 0 $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextPanel \"modelPanel\" (localizedPanelLabel(\"Persp View\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `modelPanel -unParent -l (localizedPanelLabel(\"Persp View\")) -mbv $menusOkayInPanels `;\n\t\t\t$editorName = $panelName;\n            modelEditor -e \n                -camera \"persp\" \n                -useInteractiveMode 0\n                -displayLights \"default\" \n                -displayAppearance \"smoothShaded\" \n                -activeOnly 0\n                -ignorePanZoom 0\n                -wireframeOnShaded 0\n                -headsUpDisplay 1\n                -selectionHiliteDisplay 1\n                -useDefaultMaterial 0\n"
		+ "                -bufferMode \"double\" \n                -twoSidedLighting 1\n                -backfaceCulling 0\n                -xray 0\n                -jointXray 0\n                -activeComponentsXray 0\n                -displayTextures 0\n                -smoothWireframe 0\n                -lineWidth 1\n                -textureAnisotropic 0\n                -textureHilight 1\n                -textureSampling 2\n                -textureDisplay \"modulate\" \n                -textureMaxSize 16384\n                -fogging 0\n                -fogSource \"fragment\" \n                -fogMode \"linear\" \n                -fogStart 0\n                -fogEnd 100\n                -fogDensity 0.1\n                -fogColor 0.5 0.5 0.5 1 \n                -maxConstantTransparency 1\n                -rendererName \"base_OpenGL_Renderer\" \n                -colorResolution 256 256 \n                -bumpResolution 512 512 \n                -textureCompression 0\n                -transparencyAlgorithm \"frontAndBackCull\" \n                -transpInShadows 0\n"
		+ "                -cullingOverride \"none\" \n                -lowQualityLighting 0\n                -maximumNumHardwareLights 1\n                -occlusionCulling 0\n                -shadingModel 0\n                -useBaseRenderer 0\n                -useReducedRenderer 0\n                -smallObjectCulling 0\n                -smallObjectThreshold -1 \n                -interactiveDisableShadows 0\n                -interactiveBackFaceCull 0\n                -sortTransparent 1\n                -nurbsCurves 1\n                -nurbsSurfaces 1\n                -polymeshes 1\n                -subdivSurfaces 1\n                -planes 1\n                -lights 1\n                -cameras 1\n                -controlVertices 1\n                -hulls 1\n                -grid 1\n                -joints 1\n                -ikHandles 1\n                -deformers 1\n                -dynamics 1\n                -fluids 1\n                -hairSystems 1\n                -follicles 1\n                -nCloths 1\n                -nParticles 1\n                -nRigids 1\n"
		+ "                -dynamicConstraints 1\n                -locators 1\n                -manipulators 1\n                -dimensions 1\n                -handles 1\n                -pivots 1\n                -textures 1\n                -strokes 1\n                -shadows 0\n                $editorName;\nmodelEditor -e -viewSelected 0 $editorName;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tmodelPanel -edit -l (localizedPanelLabel(\"Persp View\")) -mbv $menusOkayInPanels  $panelName;\n\t\t$editorName = $panelName;\n        modelEditor -e \n            -camera \"persp\" \n            -useInteractiveMode 0\n            -displayLights \"default\" \n            -displayAppearance \"smoothShaded\" \n            -activeOnly 0\n            -ignorePanZoom 0\n            -wireframeOnShaded 0\n            -headsUpDisplay 1\n            -selectionHiliteDisplay 1\n            -useDefaultMaterial 0\n            -bufferMode \"double\" \n            -twoSidedLighting 1\n            -backfaceCulling 0\n            -xray 0\n            -jointXray 0\n            -activeComponentsXray 0\n"
		+ "            -displayTextures 0\n            -smoothWireframe 0\n            -lineWidth 1\n            -textureAnisotropic 0\n            -textureHilight 1\n            -textureSampling 2\n            -textureDisplay \"modulate\" \n            -textureMaxSize 16384\n            -fogging 0\n            -fogSource \"fragment\" \n            -fogMode \"linear\" \n            -fogStart 0\n            -fogEnd 100\n            -fogDensity 0.1\n            -fogColor 0.5 0.5 0.5 1 \n            -maxConstantTransparency 1\n            -rendererName \"base_OpenGL_Renderer\" \n            -colorResolution 256 256 \n            -bumpResolution 512 512 \n            -textureCompression 0\n            -transparencyAlgorithm \"frontAndBackCull\" \n            -transpInShadows 0\n            -cullingOverride \"none\" \n            -lowQualityLighting 0\n            -maximumNumHardwareLights 1\n            -occlusionCulling 0\n            -shadingModel 0\n            -useBaseRenderer 0\n            -useReducedRenderer 0\n            -smallObjectCulling 0\n            -smallObjectThreshold -1 \n"
		+ "            -interactiveDisableShadows 0\n            -interactiveBackFaceCull 0\n            -sortTransparent 1\n            -nurbsCurves 1\n            -nurbsSurfaces 1\n            -polymeshes 1\n            -subdivSurfaces 1\n            -planes 1\n            -lights 1\n            -cameras 1\n            -controlVertices 1\n            -hulls 1\n            -grid 1\n            -joints 1\n            -ikHandles 1\n            -deformers 1\n            -dynamics 1\n            -fluids 1\n            -hairSystems 1\n            -follicles 1\n            -nCloths 1\n            -nParticles 1\n            -nRigids 1\n            -dynamicConstraints 1\n            -locators 1\n            -manipulators 1\n            -dimensions 1\n            -handles 1\n            -pivots 1\n            -textures 1\n            -strokes 1\n            -shadows 0\n            $editorName;\nmodelEditor -e -viewSelected 0 $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextPanel \"outlinerPanel\" (localizedPanelLabel(\"Outliner\")) `;\n"
		+ "\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `outlinerPanel -unParent -l (localizedPanelLabel(\"Outliner\")) -mbv $menusOkayInPanels `;\n\t\t\t$editorName = $panelName;\n            outlinerEditor -e \n                -docTag \"isolOutln_fromSeln\" \n                -showShapes 1\n                -showAttributes 0\n                -showConnected 0\n                -showAnimCurvesOnly 0\n                -showMuteInfo 0\n                -organizeByLayer 1\n                -showAnimLayerWeight 1\n                -autoExpandLayers 1\n                -autoExpand 0\n                -showDagOnly 0\n                -showAssets 1\n                -showContainedOnly 1\n                -showPublishedAsConnected 0\n                -showContainerContents 1\n                -ignoreDagHierarchy 0\n                -expandConnections 0\n                -showUpstreamCurves 1\n                -showUnitlessCurves 1\n                -showCompounds 1\n                -showLeafs 1\n                -showNumericAttrsOnly 0\n                -highlightActive 1\n"
		+ "                -autoSelectNewObjects 0\n                -doNotSelectNewObjects 0\n                -dropIsParent 1\n                -transmitFilters 0\n                -setFilter \"defaultSetFilter\" \n                -showSetMembers 1\n                -allowMultiSelection 1\n                -alwaysToggleSelect 0\n                -directSelect 0\n                -displayMode \"DAG\" \n                -expandObjects 0\n                -setsIgnoreFilters 1\n                -containersIgnoreFilters 0\n                -editAttrName 0\n                -showAttrValues 0\n                -highlightSecondary 0\n                -showUVAttrsOnly 0\n                -showTextureNodesOnly 0\n                -attrAlphaOrder \"default\" \n                -animLayerFilterOptions \"allAffecting\" \n                -sortOrder \"none\" \n                -longNames 0\n                -niceNames 1\n                -showNamespace 1\n                -showPinIcons 0\n                $editorName;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\toutlinerPanel -edit -l (localizedPanelLabel(\"Outliner\")) -mbv $menusOkayInPanels  $panelName;\n"
		+ "\t\t$editorName = $panelName;\n        outlinerEditor -e \n            -docTag \"isolOutln_fromSeln\" \n            -showShapes 1\n            -showAttributes 0\n            -showConnected 0\n            -showAnimCurvesOnly 0\n            -showMuteInfo 0\n            -organizeByLayer 1\n            -showAnimLayerWeight 1\n            -autoExpandLayers 1\n            -autoExpand 0\n            -showDagOnly 0\n            -showAssets 1\n            -showContainedOnly 1\n            -showPublishedAsConnected 0\n            -showContainerContents 1\n            -ignoreDagHierarchy 0\n            -expandConnections 0\n            -showUpstreamCurves 1\n            -showUnitlessCurves 1\n            -showCompounds 1\n            -showLeafs 1\n            -showNumericAttrsOnly 0\n            -highlightActive 1\n            -autoSelectNewObjects 0\n            -doNotSelectNewObjects 0\n            -dropIsParent 1\n            -transmitFilters 0\n            -setFilter \"defaultSetFilter\" \n            -showSetMembers 1\n            -allowMultiSelection 1\n"
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
		+ "                -imageEnabled 0\n                -graphType \"DAG\" \n                -heatMapDisplay 0\n                -updateSelection 1\n                -updateNodeAdded 1\n                -useDrawOverrideColor 0\n                -limitGraphTraversal -1\n                -range 0 0 \n                -iconSize \"smallIcons\" \n                -showCachedConnections 0\n                $editorName;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Hypergraph Hierarchy\")) -mbv $menusOkayInPanels  $panelName;\n\n\t\t\t$editorName = ($panelName+\"HyperGraphEd\");\n            hyperGraph -e \n                -graphLayoutStyle \"hierarchicalLayout\" \n                -orientation \"horiz\" \n                -mergeConnections 0\n                -zoom 1\n                -animateTransition 0\n                -showRelationships 1\n                -showShapes 0\n                -showDeformers 0\n                -showExpressions 0\n                -showConstraints 0\n                -showUnderworld 0\n                -showInvisible 0\n"
		+ "                -transitionFrames 1\n                -opaqueContainers 0\n                -freeform 0\n                -imagePosition 0 0 \n                -imageScale 1\n                -imageEnabled 0\n                -graphType \"DAG\" \n                -heatMapDisplay 0\n                -updateSelection 1\n                -updateNodeAdded 1\n                -useDrawOverrideColor 0\n                -limitGraphTraversal -1\n                -range 0 0 \n                -iconSize \"smallIcons\" \n                -showCachedConnections 0\n                $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"hyperShadePanel\" (localizedPanelLabel(\"Hypershade\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"hyperShadePanel\" -l (localizedPanelLabel(\"Hypershade\")) -mbv $menusOkayInPanels `;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Hypershade\")) -mbv $menusOkayInPanels  $panelName;\n"
		+ "\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\tif ($useSceneConfig) {\n\t\tscriptedPanel -e -to $panelName;\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"visorPanel\" (localizedPanelLabel(\"Visor\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"visorPanel\" -l (localizedPanelLabel(\"Visor\")) -mbv $menusOkayInPanels `;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Visor\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"polyTexturePlacementPanel\" (localizedPanelLabel(\"UV Texture Editor\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"polyTexturePlacementPanel\" -l (localizedPanelLabel(\"UV Texture Editor\")) -mbv $menusOkayInPanels `;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"UV Texture Editor\")) -mbv $menusOkayInPanels  $panelName;\n"
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
		+ "\t\t\t\t\t\"$panelName = `modelPanel -unParent -l (localizedPanelLabel(\\\"Persp View\\\")) -mbv $menusOkayInPanels `;\\n$editorName = $panelName;\\nmodelEditor -e \\n    -cam `findStartUpCamera persp` \\n    -useInteractiveMode 0\\n    -displayLights \\\"default\\\" \\n    -displayAppearance \\\"smoothShaded\\\" \\n    -activeOnly 0\\n    -ignorePanZoom 0\\n    -wireframeOnShaded 0\\n    -headsUpDisplay 1\\n    -selectionHiliteDisplay 1\\n    -useDefaultMaterial 0\\n    -bufferMode \\\"double\\\" \\n    -twoSidedLighting 1\\n    -backfaceCulling 0\\n    -xray 0\\n    -jointXray 0\\n    -activeComponentsXray 0\\n    -displayTextures 0\\n    -smoothWireframe 0\\n    -lineWidth 1\\n    -textureAnisotropic 0\\n    -textureHilight 1\\n    -textureSampling 2\\n    -textureDisplay \\\"modulate\\\" \\n    -textureMaxSize 16384\\n    -fogging 0\\n    -fogSource \\\"fragment\\\" \\n    -fogMode \\\"linear\\\" \\n    -fogStart 0\\n    -fogEnd 100\\n    -fogDensity 0.1\\n    -fogColor 0.5 0.5 0.5 1 \\n    -maxConstantTransparency 1\\n    -rendererName \\\"base_OpenGL_Renderer\\\" \\n    -colorResolution 256 256 \\n    -bumpResolution 512 512 \\n    -textureCompression 0\\n    -transparencyAlgorithm \\\"frontAndBackCull\\\" \\n    -transpInShadows 0\\n    -cullingOverride \\\"none\\\" \\n    -lowQualityLighting 0\\n    -maximumNumHardwareLights 1\\n    -occlusionCulling 0\\n    -shadingModel 0\\n    -useBaseRenderer 0\\n    -useReducedRenderer 0\\n    -smallObjectCulling 0\\n    -smallObjectThreshold -1 \\n    -interactiveDisableShadows 0\\n    -interactiveBackFaceCull 0\\n    -sortTransparent 1\\n    -nurbsCurves 1\\n    -nurbsSurfaces 1\\n    -polymeshes 1\\n    -subdivSurfaces 1\\n    -planes 1\\n    -lights 1\\n    -cameras 1\\n    -controlVertices 1\\n    -hulls 1\\n    -grid 1\\n    -joints 1\\n    -ikHandles 1\\n    -deformers 1\\n    -dynamics 1\\n    -fluids 1\\n    -hairSystems 1\\n    -follicles 1\\n    -nCloths 1\\n    -nParticles 1\\n    -nRigids 1\\n    -dynamicConstraints 1\\n    -locators 1\\n    -manipulators 1\\n    -dimensions 1\\n    -handles 1\\n    -pivots 1\\n    -textures 1\\n    -strokes 1\\n    -shadows 0\\n    $editorName;\\nmodelEditor -e -viewSelected 0 $editorName\"\n"
		+ "\t\t\t\t\t\"modelPanel -edit -l (localizedPanelLabel(\\\"Persp View\\\")) -mbv $menusOkayInPanels  $panelName;\\n$editorName = $panelName;\\nmodelEditor -e \\n    -cam `findStartUpCamera persp` \\n    -useInteractiveMode 0\\n    -displayLights \\\"default\\\" \\n    -displayAppearance \\\"smoothShaded\\\" \\n    -activeOnly 0\\n    -ignorePanZoom 0\\n    -wireframeOnShaded 0\\n    -headsUpDisplay 1\\n    -selectionHiliteDisplay 1\\n    -useDefaultMaterial 0\\n    -bufferMode \\\"double\\\" \\n    -twoSidedLighting 1\\n    -backfaceCulling 0\\n    -xray 0\\n    -jointXray 0\\n    -activeComponentsXray 0\\n    -displayTextures 0\\n    -smoothWireframe 0\\n    -lineWidth 1\\n    -textureAnisotropic 0\\n    -textureHilight 1\\n    -textureSampling 2\\n    -textureDisplay \\\"modulate\\\" \\n    -textureMaxSize 16384\\n    -fogging 0\\n    -fogSource \\\"fragment\\\" \\n    -fogMode \\\"linear\\\" \\n    -fogStart 0\\n    -fogEnd 100\\n    -fogDensity 0.1\\n    -fogColor 0.5 0.5 0.5 1 \\n    -maxConstantTransparency 1\\n    -rendererName \\\"base_OpenGL_Renderer\\\" \\n    -colorResolution 256 256 \\n    -bumpResolution 512 512 \\n    -textureCompression 0\\n    -transparencyAlgorithm \\\"frontAndBackCull\\\" \\n    -transpInShadows 0\\n    -cullingOverride \\\"none\\\" \\n    -lowQualityLighting 0\\n    -maximumNumHardwareLights 1\\n    -occlusionCulling 0\\n    -shadingModel 0\\n    -useBaseRenderer 0\\n    -useReducedRenderer 0\\n    -smallObjectCulling 0\\n    -smallObjectThreshold -1 \\n    -interactiveDisableShadows 0\\n    -interactiveBackFaceCull 0\\n    -sortTransparent 1\\n    -nurbsCurves 1\\n    -nurbsSurfaces 1\\n    -polymeshes 1\\n    -subdivSurfaces 1\\n    -planes 1\\n    -lights 1\\n    -cameras 1\\n    -controlVertices 1\\n    -hulls 1\\n    -grid 1\\n    -joints 1\\n    -ikHandles 1\\n    -deformers 1\\n    -dynamics 1\\n    -fluids 1\\n    -hairSystems 1\\n    -follicles 1\\n    -nCloths 1\\n    -nParticles 1\\n    -nRigids 1\\n    -dynamicConstraints 1\\n    -locators 1\\n    -manipulators 1\\n    -dimensions 1\\n    -handles 1\\n    -pivots 1\\n    -textures 1\\n    -strokes 1\\n    -shadows 0\\n    $editorName;\\nmodelEditor -e -viewSelected 0 $editorName\"\n"
		+ "\t\t\t\t$configName;\n\n            setNamedPanelLayout (localizedPanelLabel(\"Current Layout\"));\n        }\n\n        panelHistory -e -clear mainPanelHistory;\n        setFocus `paneLayout -q -p1 $gMainPane`;\n        sceneUIReplacement -deleteRemaining;\n        sceneUIReplacement -clear;\n\t}\n\n\ngrid -spacing 5 -size 12 -divisions 5 -displayAxes yes -displayGridLines yes -displayDivisionLines yes -displayPerspectiveLabels no -displayOrthographicLabels no -displayAxesBold yes -perspectiveLabelPosition axis -orthographicLabelPosition edge;\nviewManip -drawCompass 0 -compassAngle 0 -frontParameters \"\" -homeParameters \"\" -selectionLockParameters \"\";\n}\n");
	setAttr ".st" 3;
createNode script -n "sceneConfigurationScriptNode";
	setAttr ".b" -type "string" "playbackOptions -min 1 -max 24 -ast 1 -aet 48 ";
	setAttr ".st" 6;
createNode shadingEngine -n "aiStandard1SG";
	addAttr -ci true -m -sn "aovs" -ln "aiCustomAOVs" -at "compound" -nc 2;
	addAttr -ci true -k true -sn "aov_name" -ln "aovName" -dt "string" -p "aiCustomAOVs";
	addAttr -ci true -k true -sn "aov_input" -ln "aovInput" -at "message" -p "aiCustomAOVs";
	setAttr ".ihi" 0;
	setAttr ".ro" yes;
createNode materialInfo -n "materialInfo1";
createNode shadingEngine -n "aiUtility1SG";
	addAttr -ci true -m -sn "aovs" -ln "aiCustomAOVs" -at "compound" -nc 2;
	addAttr -ci true -k true -sn "aov_name" -ln "aovName" -dt "string" -p "aiCustomAOVs";
	addAttr -ci true -k true -sn "aov_input" -ln "aovInput" -at "message" -p "aiCustomAOVs";
	setAttr ".ihi" 0;
	setAttr ".ro" yes;
createNode materialInfo -n "materialInfo2";
createNode shadingEngine -n "aiStandard2SG";
	addAttr -ci true -m -sn "aovs" -ln "aiCustomAOVs" -at "compound" -nc 2;
	addAttr -ci true -k true -sn "aov_name" -ln "aovName" -dt "string" -p "aiCustomAOVs";
	addAttr -ci true -k true -sn "aov_input" -ln "aovInput" -at "message" -p "aiCustomAOVs";
	setAttr ".ihi" 0;
	setAttr ".ro" yes;
createNode materialInfo -n "materialInfo3";
createNode polySphere -n "polySphere1";
	setAttr ".r" 2;
createNode polySphere -n "polySphere2";
	setAttr ".r" 2;
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
	setAttr ".ai_translator" -type "string" "tif";
	setAttr ".tiled" no;
createNode aiHair -n "aiHair1";
	setAttr ".rootcolor" -type "float3" 1 1 0 ;
	setAttr ".tipcolor" -type "float3" 0 1 0 ;
	setAttr ".kd_ind" 10;
createNode aiStandard -n "aiStandard3";
	setAttr ".Kd_color" -type "float3" 1 0 0 ;
	setAttr ".emission" 1;
	setAttr ".emission_color" -type "float3" 0 1 1 ;
createNode aiStandard -n "aiStandard4";
createNode shadingEngine -n "aiHair1SG";
	addAttr -ci true -m -sn "aovs" -ln "aiCustomAOVs" -at "compound" -nc 2;
	addAttr -ci true -k true -sn "aov_name" -ln "aovName" -dt "string" -p "aiCustomAOVs";
	addAttr -ci true -k true -sn "aov_input" -ln "aovInput" -at "message" -p "aiCustomAOVs";
	setAttr ".ihi" 0;
	setAttr ".ro" yes;
createNode materialInfo -n "materialInfo4";
createNode shadingEngine -n "aiStandard4SG";
	addAttr -ci true -m -sn "aovs" -ln "aiCustomAOVs" -at "compound" -nc 2;
	addAttr -ci true -k true -sn "aov_name" -ln "aovName" -dt "string" -p "aiCustomAOVs";
	addAttr -ci true -k true -sn "aov_input" -ln "aovInput" -at "message" -p "aiCustomAOVs";
	setAttr ".ihi" 0;
	setAttr -s 2 ".dsm";
	setAttr ".ro" yes;
createNode materialInfo -n "materialInfo5";
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
	setAttr -s 7 ".st";
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
	setAttr -s 5 ".s";
select -ne :lightList1;
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
connectAttr "polySphere1.out" "pSphereShape1.i";
connectAttr "polySphere2.out" "pSphereShape2.i";
connectAttr ":time1.o" "hairSystemShape1.cti";
connectAttr "pSphere1FollicleShape303.oha" "hairSystemShape1.ih[0]";
connectAttr "pSphere1FollicleShape309.oha" "hairSystemShape1.ih[1]";
connectAttr "pSphere1FollicleShape315.oha" "hairSystemShape1.ih[2]";
connectAttr "pSphere1FollicleShape322.oha" "hairSystemShape1.ih[3]";
connectAttr "pSphere1FollicleShape328.oha" "hairSystemShape1.ih[4]";
connectAttr "pSphere1FollicleShape334.oha" "hairSystemShape1.ih[5]";
connectAttr "pSphere1FollicleShape340.oha" "hairSystemShape1.ih[6]";
connectAttr "pSphere1FollicleShape346.oha" "hairSystemShape1.ih[7]";
connectAttr "pSphere1FollicleShape353.oha" "hairSystemShape1.ih[8]";
connectAttr "pSphere1FollicleShape359.oha" "hairSystemShape1.ih[9]";
connectAttr "pSphere1FollicleShape365.oha" "hairSystemShape1.ih[10]";
connectAttr "pSphere1FollicleShape371.oha" "hairSystemShape1.ih[11]";
connectAttr "pSphere1FollicleShape377.oha" "hairSystemShape1.ih[12]";
connectAttr "pSphere1FollicleShape384.oha" "hairSystemShape1.ih[13]";
connectAttr "pSphere1FollicleShape390.oha" "hairSystemShape1.ih[14]";
connectAttr "pSphere1FollicleShape396.oha" "hairSystemShape1.ih[15]";
connectAttr "pSphere1FollicleShape909.oha" "hairSystemShape1.ih[16]";
connectAttr "pSphere1FollicleShape915.oha" "hairSystemShape1.ih[17]";
connectAttr "pSphere1FollicleShape922.oha" "hairSystemShape1.ih[18]";
connectAttr "pSphere1FollicleShape928.oha" "hairSystemShape1.ih[19]";
connectAttr "pSphere1FollicleShape934.oha" "hairSystemShape1.ih[20]";
connectAttr "pSphere1FollicleShape940.oha" "hairSystemShape1.ih[21]";
connectAttr "pSphere1FollicleShape946.oha" "hairSystemShape1.ih[22]";
connectAttr "pSphere1FollicleShape953.oha" "hairSystemShape1.ih[23]";
connectAttr "pSphere1FollicleShape959.oha" "hairSystemShape1.ih[24]";
connectAttr "pSphere1FollicleShape965.oha" "hairSystemShape1.ih[25]";
connectAttr "pSphere1FollicleShape971.oha" "hairSystemShape1.ih[26]";
connectAttr "pSphere1FollicleShape977.oha" "hairSystemShape1.ih[27]";
connectAttr "pSphere1FollicleShape984.oha" "hairSystemShape1.ih[28]";
connectAttr "pSphere1FollicleShape990.oha" "hairSystemShape1.ih[29]";
connectAttr "pSphere1FollicleShape1509.oha" "hairSystemShape1.ih[30]";
connectAttr "pSphere1FollicleShape1515.oha" "hairSystemShape1.ih[31]";
connectAttr "pSphere1FollicleShape1522.oha" "hairSystemShape1.ih[32]";
connectAttr "pSphere1FollicleShape1528.oha" "hairSystemShape1.ih[33]";
connectAttr "pSphere1FollicleShape1534.oha" "hairSystemShape1.ih[34]";
connectAttr "pSphere1FollicleShape1540.oha" "hairSystemShape1.ih[35]";
connectAttr "pSphere1FollicleShape1546.oha" "hairSystemShape1.ih[36]";
connectAttr "pSphere1FollicleShape1553.oha" "hairSystemShape1.ih[37]";
connectAttr "pSphere1FollicleShape1559.oha" "hairSystemShape1.ih[38]";
connectAttr "pSphere1FollicleShape1565.oha" "hairSystemShape1.ih[39]";
connectAttr "pSphere1FollicleShape1571.oha" "hairSystemShape1.ih[40]";
connectAttr "pSphere1FollicleShape1577.oha" "hairSystemShape1.ih[41]";
connectAttr "pSphere1FollicleShape1584.oha" "hairSystemShape1.ih[42]";
connectAttr "pSphere1FollicleShape1590.oha" "hairSystemShape1.ih[43]";
connectAttr "pSphere1FollicleShape2203.oha" "hairSystemShape1.ih[44]";
connectAttr "pSphere1FollicleShape2209.oha" "hairSystemShape1.ih[45]";
connectAttr "pSphere1FollicleShape2215.oha" "hairSystemShape1.ih[46]";
connectAttr "pSphere1FollicleShape2222.oha" "hairSystemShape1.ih[47]";
connectAttr "pSphere1FollicleShape2228.oha" "hairSystemShape1.ih[48]";
connectAttr "pSphere1FollicleShape2234.oha" "hairSystemShape1.ih[49]";
connectAttr "pSphere1FollicleShape2240.oha" "hairSystemShape1.ih[50]";
connectAttr "pSphere1FollicleShape2246.oha" "hairSystemShape1.ih[51]";
connectAttr "pSphere1FollicleShape2253.oha" "hairSystemShape1.ih[52]";
connectAttr "pSphere1FollicleShape2259.oha" "hairSystemShape1.ih[53]";
connectAttr "pSphere1FollicleShape2265.oha" "hairSystemShape1.ih[54]";
connectAttr "pSphere1FollicleShape2271.oha" "hairSystemShape1.ih[55]";
connectAttr "pSphere1FollicleShape2277.oha" "hairSystemShape1.ih[56]";
connectAttr "pSphere1FollicleShape2284.oha" "hairSystemShape1.ih[57]";
connectAttr "pSphere1FollicleShape2290.oha" "hairSystemShape1.ih[58]";
connectAttr "pSphere1FollicleShape2296.oha" "hairSystemShape1.ih[59]";
connectAttr "pSphere1FollicleShape2803.oha" "hairSystemShape1.ih[60]";
connectAttr "pSphere1FollicleShape2809.oha" "hairSystemShape1.ih[61]";
connectAttr "pSphere1FollicleShape2815.oha" "hairSystemShape1.ih[62]";
connectAttr "pSphere1FollicleShape2822.oha" "hairSystemShape1.ih[63]";
connectAttr "pSphere1FollicleShape2828.oha" "hairSystemShape1.ih[64]";
connectAttr "pSphere1FollicleShape2834.oha" "hairSystemShape1.ih[65]";
connectAttr "pSphere1FollicleShape2840.oha" "hairSystemShape1.ih[66]";
connectAttr "pSphere1FollicleShape2846.oha" "hairSystemShape1.ih[67]";
connectAttr "pSphere1FollicleShape2853.oha" "hairSystemShape1.ih[68]";
connectAttr "pSphere1FollicleShape2859.oha" "hairSystemShape1.ih[69]";
connectAttr "pSphere1FollicleShape2865.oha" "hairSystemShape1.ih[70]";
connectAttr "pSphere1FollicleShape2871.oha" "hairSystemShape1.ih[71]";
connectAttr "pSphere1FollicleShape2877.oha" "hairSystemShape1.ih[72]";
connectAttr "pSphere1FollicleShape2884.oha" "hairSystemShape1.ih[73]";
connectAttr "pSphere1FollicleShape2890.oha" "hairSystemShape1.ih[74]";
connectAttr "pSphere1FollicleShape2896.oha" "hairSystemShape1.ih[75]";
connectAttr "pSphere1FollicleShape3409.oha" "hairSystemShape1.ih[76]";
connectAttr "pSphere1FollicleShape3415.oha" "hairSystemShape1.ih[77]";
connectAttr "pSphere1FollicleShape3422.oha" "hairSystemShape1.ih[78]";
connectAttr "pSphere1FollicleShape3428.oha" "hairSystemShape1.ih[79]";
connectAttr "pSphere1FollicleShape3434.oha" "hairSystemShape1.ih[80]";
connectAttr "pSphere1FollicleShape3440.oha" "hairSystemShape1.ih[81]";
connectAttr "pSphere1FollicleShape3446.oha" "hairSystemShape1.ih[82]";
connectAttr "pSphere1FollicleShape3453.oha" "hairSystemShape1.ih[83]";
connectAttr "pSphere1FollicleShape3459.oha" "hairSystemShape1.ih[84]";
connectAttr "pSphere1FollicleShape3465.oha" "hairSystemShape1.ih[85]";
connectAttr "pSphere1FollicleShape3471.oha" "hairSystemShape1.ih[86]";
connectAttr "pSphere1FollicleShape3477.oha" "hairSystemShape1.ih[87]";
connectAttr "pSphere1FollicleShape3484.oha" "hairSystemShape1.ih[88]";
connectAttr "pSphere1FollicleShape3490.oha" "hairSystemShape1.ih[89]";
connectAttr "pSphere1FollicleShape4009.oha" "hairSystemShape1.ih[90]";
connectAttr "pSphere1FollicleShape4015.oha" "hairSystemShape1.ih[91]";
connectAttr "pSphere1FollicleShape4022.oha" "hairSystemShape1.ih[92]";
connectAttr "pSphere1FollicleShape4028.oha" "hairSystemShape1.ih[93]";
connectAttr "pSphere1FollicleShape4034.oha" "hairSystemShape1.ih[94]";
connectAttr "pSphere1FollicleShape4040.oha" "hairSystemShape1.ih[95]";
connectAttr "pSphere1FollicleShape4046.oha" "hairSystemShape1.ih[96]";
connectAttr "pSphere1FollicleShape4053.oha" "hairSystemShape1.ih[97]";
connectAttr "pSphere1FollicleShape4059.oha" "hairSystemShape1.ih[98]";
connectAttr "pSphere1FollicleShape4065.oha" "hairSystemShape1.ih[99]";
connectAttr "pSphere1FollicleShape4071.oha" "hairSystemShape1.ih[100]";
connectAttr "pSphere1FollicleShape4077.oha" "hairSystemShape1.ih[101]";
connectAttr "pSphere1FollicleShape4084.oha" "hairSystemShape1.ih[102]";
connectAttr "pSphere1FollicleShape4090.oha" "hairSystemShape1.ih[103]";
connectAttr "pSphere1FollicleShape4603.oha" "hairSystemShape1.ih[104]";
connectAttr "pSphere1FollicleShape4609.oha" "hairSystemShape1.ih[105]";
connectAttr "pSphere1FollicleShape4615.oha" "hairSystemShape1.ih[106]";
connectAttr "pSphere1FollicleShape4622.oha" "hairSystemShape1.ih[107]";
connectAttr "pSphere1FollicleShape4628.oha" "hairSystemShape1.ih[108]";
connectAttr "pSphere1FollicleShape4634.oha" "hairSystemShape1.ih[109]";
connectAttr "pSphere1FollicleShape4640.oha" "hairSystemShape1.ih[110]";
connectAttr "pSphere1FollicleShape4646.oha" "hairSystemShape1.ih[111]";
connectAttr "pSphere1FollicleShape4653.oha" "hairSystemShape1.ih[112]";
connectAttr "pSphere1FollicleShape4659.oha" "hairSystemShape1.ih[113]";
connectAttr "pSphere1FollicleShape4665.oha" "hairSystemShape1.ih[114]";
connectAttr "pSphere1FollicleShape4671.oha" "hairSystemShape1.ih[115]";
connectAttr "pSphere1FollicleShape4677.oha" "hairSystemShape1.ih[116]";
connectAttr "pSphere1FollicleShape4684.oha" "hairSystemShape1.ih[117]";
connectAttr "pSphere1FollicleShape4690.oha" "hairSystemShape1.ih[118]";
connectAttr "pSphere1FollicleShape4696.oha" "hairSystemShape1.ih[119]";
connectAttr "pSphere1FollicleShape5303.oha" "hairSystemShape1.ih[120]";
connectAttr "pSphere1FollicleShape5309.oha" "hairSystemShape1.ih[121]";
connectAttr "pSphere1FollicleShape5315.oha" "hairSystemShape1.ih[122]";
connectAttr "pSphere1FollicleShape5322.oha" "hairSystemShape1.ih[123]";
connectAttr "pSphere1FollicleShape5328.oha" "hairSystemShape1.ih[124]";
connectAttr "pSphere1FollicleShape5334.oha" "hairSystemShape1.ih[125]";
connectAttr "pSphere1FollicleShape5340.oha" "hairSystemShape1.ih[126]";
connectAttr "pSphere1FollicleShape5346.oha" "hairSystemShape1.ih[127]";
connectAttr "pSphere1FollicleShape5353.oha" "hairSystemShape1.ih[128]";
connectAttr "pSphere1FollicleShape5359.oha" "hairSystemShape1.ih[129]";
connectAttr "pSphere1FollicleShape5365.oha" "hairSystemShape1.ih[130]";
connectAttr "pSphere1FollicleShape5371.oha" "hairSystemShape1.ih[131]";
connectAttr "pSphere1FollicleShape5377.oha" "hairSystemShape1.ih[132]";
connectAttr "pSphere1FollicleShape5384.oha" "hairSystemShape1.ih[133]";
connectAttr "pSphere1FollicleShape5390.oha" "hairSystemShape1.ih[134]";
connectAttr "pSphere1FollicleShape5396.oha" "hairSystemShape1.ih[135]";
connectAttr "pSphere1FollicleShape5909.oha" "hairSystemShape1.ih[136]";
connectAttr "pSphere1FollicleShape5915.oha" "hairSystemShape1.ih[137]";
connectAttr "pSphere1FollicleShape5922.oha" "hairSystemShape1.ih[138]";
connectAttr "pSphere1FollicleShape5928.oha" "hairSystemShape1.ih[139]";
connectAttr "pSphere1FollicleShape5934.oha" "hairSystemShape1.ih[140]";
connectAttr "pSphere1FollicleShape5940.oha" "hairSystemShape1.ih[141]";
connectAttr "pSphere1FollicleShape5946.oha" "hairSystemShape1.ih[142]";
connectAttr "pSphere1FollicleShape5953.oha" "hairSystemShape1.ih[143]";
connectAttr "pSphere1FollicleShape5959.oha" "hairSystemShape1.ih[144]";
connectAttr "pSphere1FollicleShape5965.oha" "hairSystemShape1.ih[145]";
connectAttr "pSphere1FollicleShape5971.oha" "hairSystemShape1.ih[146]";
connectAttr "pSphere1FollicleShape5977.oha" "hairSystemShape1.ih[147]";
connectAttr "pSphere1FollicleShape5984.oha" "hairSystemShape1.ih[148]";
connectAttr "pSphere1FollicleShape5990.oha" "hairSystemShape1.ih[149]";
connectAttr "pSphere1FollicleShape6509.oha" "hairSystemShape1.ih[150]";
connectAttr "pSphere1FollicleShape6515.oha" "hairSystemShape1.ih[151]";
connectAttr "pSphere1FollicleShape6522.oha" "hairSystemShape1.ih[152]";
connectAttr "pSphere1FollicleShape6528.oha" "hairSystemShape1.ih[153]";
connectAttr "pSphere1FollicleShape6534.oha" "hairSystemShape1.ih[154]";
connectAttr "pSphere1FollicleShape6540.oha" "hairSystemShape1.ih[155]";
connectAttr "pSphere1FollicleShape6546.oha" "hairSystemShape1.ih[156]";
connectAttr "pSphere1FollicleShape6553.oha" "hairSystemShape1.ih[157]";
connectAttr "pSphere1FollicleShape6559.oha" "hairSystemShape1.ih[158]";
connectAttr "pSphere1FollicleShape6565.oha" "hairSystemShape1.ih[159]";
connectAttr "pSphere1FollicleShape6571.oha" "hairSystemShape1.ih[160]";
connectAttr "pSphere1FollicleShape6577.oha" "hairSystemShape1.ih[161]";
connectAttr "pSphere1FollicleShape6584.oha" "hairSystemShape1.ih[162]";
connectAttr "pSphere1FollicleShape6590.oha" "hairSystemShape1.ih[163]";
connectAttr "pSphere1FollicleShape7103.oha" "hairSystemShape1.ih[164]";
connectAttr "pSphere1FollicleShape7109.oha" "hairSystemShape1.ih[165]";
connectAttr "pSphere1FollicleShape7115.oha" "hairSystemShape1.ih[166]";
connectAttr "pSphere1FollicleShape7122.oha" "hairSystemShape1.ih[167]";
connectAttr "pSphere1FollicleShape7128.oha" "hairSystemShape1.ih[168]";
connectAttr "pSphere1FollicleShape7134.oha" "hairSystemShape1.ih[169]";
connectAttr "pSphere1FollicleShape7140.oha" "hairSystemShape1.ih[170]";
connectAttr "pSphere1FollicleShape7146.oha" "hairSystemShape1.ih[171]";
connectAttr "pSphere1FollicleShape7153.oha" "hairSystemShape1.ih[172]";
connectAttr "pSphere1FollicleShape7159.oha" "hairSystemShape1.ih[173]";
connectAttr "pSphere1FollicleShape7165.oha" "hairSystemShape1.ih[174]";
connectAttr "pSphere1FollicleShape7171.oha" "hairSystemShape1.ih[175]";
connectAttr "pSphere1FollicleShape7177.oha" "hairSystemShape1.ih[176]";
connectAttr "pSphere1FollicleShape7184.oha" "hairSystemShape1.ih[177]";
connectAttr "pSphere1FollicleShape7190.oha" "hairSystemShape1.ih[178]";
connectAttr "pSphere1FollicleShape7196.oha" "hairSystemShape1.ih[179]";
connectAttr "pSphere1FollicleShape7703.oha" "hairSystemShape1.ih[180]";
connectAttr "pSphere1FollicleShape7709.oha" "hairSystemShape1.ih[181]";
connectAttr "pSphere1FollicleShape7715.oha" "hairSystemShape1.ih[182]";
connectAttr "pSphere1FollicleShape7722.oha" "hairSystemShape1.ih[183]";
connectAttr "pSphere1FollicleShape7728.oha" "hairSystemShape1.ih[184]";
connectAttr "pSphere1FollicleShape7734.oha" "hairSystemShape1.ih[185]";
connectAttr "pSphere1FollicleShape7740.oha" "hairSystemShape1.ih[186]";
connectAttr "pSphere1FollicleShape7746.oha" "hairSystemShape1.ih[187]";
connectAttr "pSphere1FollicleShape7753.oha" "hairSystemShape1.ih[188]";
connectAttr "pSphere1FollicleShape7759.oha" "hairSystemShape1.ih[189]";
connectAttr "pSphere1FollicleShape7765.oha" "hairSystemShape1.ih[190]";
connectAttr "pSphere1FollicleShape7771.oha" "hairSystemShape1.ih[191]";
connectAttr "pSphere1FollicleShape7777.oha" "hairSystemShape1.ih[192]";
connectAttr "pSphere1FollicleShape7784.oha" "hairSystemShape1.ih[193]";
connectAttr "pSphere1FollicleShape7790.oha" "hairSystemShape1.ih[194]";
connectAttr "pSphere1FollicleShape7796.oha" "hairSystemShape1.ih[195]";
connectAttr "pSphere1FollicleShape8409.oha" "hairSystemShape1.ih[196]";
connectAttr "pSphere1FollicleShape8415.oha" "hairSystemShape1.ih[197]";
connectAttr "pSphere1FollicleShape8422.oha" "hairSystemShape1.ih[198]";
connectAttr "pSphere1FollicleShape8428.oha" "hairSystemShape1.ih[199]";
connectAttr "pSphere1FollicleShape8434.oha" "hairSystemShape1.ih[200]";
connectAttr "pSphere1FollicleShape8440.oha" "hairSystemShape1.ih[201]";
connectAttr "pSphere1FollicleShape8446.oha" "hairSystemShape1.ih[202]";
connectAttr "pSphere1FollicleShape8453.oha" "hairSystemShape1.ih[203]";
connectAttr "pSphere1FollicleShape8459.oha" "hairSystemShape1.ih[204]";
connectAttr "pSphere1FollicleShape8465.oha" "hairSystemShape1.ih[205]";
connectAttr "pSphere1FollicleShape8471.oha" "hairSystemShape1.ih[206]";
connectAttr "pSphere1FollicleShape8477.oha" "hairSystemShape1.ih[207]";
connectAttr "pSphere1FollicleShape8484.oha" "hairSystemShape1.ih[208]";
connectAttr "pSphere1FollicleShape8490.oha" "hairSystemShape1.ih[209]";
connectAttr "pSphere1FollicleShape9009.oha" "hairSystemShape1.ih[210]";
connectAttr "pSphere1FollicleShape9015.oha" "hairSystemShape1.ih[211]";
connectAttr "pSphere1FollicleShape9022.oha" "hairSystemShape1.ih[212]";
connectAttr "pSphere1FollicleShape9028.oha" "hairSystemShape1.ih[213]";
connectAttr "pSphere1FollicleShape9034.oha" "hairSystemShape1.ih[214]";
connectAttr "pSphere1FollicleShape9040.oha" "hairSystemShape1.ih[215]";
connectAttr "pSphere1FollicleShape9046.oha" "hairSystemShape1.ih[216]";
connectAttr "pSphere1FollicleShape9053.oha" "hairSystemShape1.ih[217]";
connectAttr "pSphere1FollicleShape9059.oha" "hairSystemShape1.ih[218]";
connectAttr "pSphere1FollicleShape9065.oha" "hairSystemShape1.ih[219]";
connectAttr "pSphere1FollicleShape9071.oha" "hairSystemShape1.ih[220]";
connectAttr "pSphere1FollicleShape9077.oha" "hairSystemShape1.ih[221]";
connectAttr "pSphere1FollicleShape9084.oha" "hairSystemShape1.ih[222]";
connectAttr "pSphere1FollicleShape9090.oha" "hairSystemShape1.ih[223]";
connectAttr "pSphere1FollicleShape9603.oha" "hairSystemShape1.ih[224]";
connectAttr "pSphere1FollicleShape9609.oha" "hairSystemShape1.ih[225]";
connectAttr "pSphere1FollicleShape9615.oha" "hairSystemShape1.ih[226]";
connectAttr "pSphere1FollicleShape9622.oha" "hairSystemShape1.ih[227]";
connectAttr "pSphere1FollicleShape9628.oha" "hairSystemShape1.ih[228]";
connectAttr "pSphere1FollicleShape9634.oha" "hairSystemShape1.ih[229]";
connectAttr "pSphere1FollicleShape9640.oha" "hairSystemShape1.ih[230]";
connectAttr "pSphere1FollicleShape9646.oha" "hairSystemShape1.ih[231]";
connectAttr "pSphere1FollicleShape9653.oha" "hairSystemShape1.ih[232]";
connectAttr "pSphere1FollicleShape9659.oha" "hairSystemShape1.ih[233]";
connectAttr "pSphere1FollicleShape9665.oha" "hairSystemShape1.ih[234]";
connectAttr "pSphere1FollicleShape9671.oha" "hairSystemShape1.ih[235]";
connectAttr "pSphere1FollicleShape9677.oha" "hairSystemShape1.ih[236]";
connectAttr "pSphere1FollicleShape9684.oha" "hairSystemShape1.ih[237]";
connectAttr "pSphere1FollicleShape9690.oha" "hairSystemShape1.ih[238]";
connectAttr "pSphere1FollicleShape9696.oha" "hairSystemShape1.ih[239]";
connectAttr "aiHair1.out" "hairSystemShape1.ai_hair_shader";
connectAttr "pSphere1FollicleShape303.ot" "pSphere1Follicle303.t" -l on;
connectAttr "pSphere1FollicleShape303.or" "pSphere1Follicle303.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape303.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape303.inm";
connectAttr "curveShape1.ws" "pSphere1FollicleShape303.sp";
connectAttr "pSphere1FollicleShape309.ot" "pSphere1Follicle309.t" -l on;
connectAttr "pSphere1FollicleShape309.or" "pSphere1Follicle309.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape309.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape309.inm";
connectAttr "curveShape2.ws" "pSphere1FollicleShape309.sp";
connectAttr "pSphere1FollicleShape315.ot" "pSphere1Follicle315.t" -l on;
connectAttr "pSphere1FollicleShape315.or" "pSphere1Follicle315.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape315.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape315.inm";
connectAttr "curveShape3.ws" "pSphere1FollicleShape315.sp";
connectAttr "pSphere1FollicleShape322.ot" "pSphere1Follicle322.t" -l on;
connectAttr "pSphere1FollicleShape322.or" "pSphere1Follicle322.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape322.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape322.inm";
connectAttr "curveShape4.ws" "pSphere1FollicleShape322.sp";
connectAttr "pSphere1FollicleShape328.ot" "pSphere1Follicle328.t" -l on;
connectAttr "pSphere1FollicleShape328.or" "pSphere1Follicle328.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape328.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape328.inm";
connectAttr "curveShape5.ws" "pSphere1FollicleShape328.sp";
connectAttr "pSphere1FollicleShape334.ot" "pSphere1Follicle334.t" -l on;
connectAttr "pSphere1FollicleShape334.or" "pSphere1Follicle334.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape334.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape334.inm";
connectAttr "curveShape6.ws" "pSphere1FollicleShape334.sp";
connectAttr "pSphere1FollicleShape340.ot" "pSphere1Follicle340.t" -l on;
connectAttr "pSphere1FollicleShape340.or" "pSphere1Follicle340.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape340.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape340.inm";
connectAttr "curveShape7.ws" "pSphere1FollicleShape340.sp";
connectAttr "pSphere1FollicleShape346.ot" "pSphere1Follicle346.t" -l on;
connectAttr "pSphere1FollicleShape346.or" "pSphere1Follicle346.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape346.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape346.inm";
connectAttr "curveShape8.ws" "pSphere1FollicleShape346.sp";
connectAttr "pSphere1FollicleShape353.ot" "pSphere1Follicle353.t" -l on;
connectAttr "pSphere1FollicleShape353.or" "pSphere1Follicle353.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape353.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape353.inm";
connectAttr "curveShape9.ws" "pSphere1FollicleShape353.sp";
connectAttr "pSphere1FollicleShape359.ot" "pSphere1Follicle359.t" -l on;
connectAttr "pSphere1FollicleShape359.or" "pSphere1Follicle359.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape359.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape359.inm";
connectAttr "curveShape10.ws" "pSphere1FollicleShape359.sp";
connectAttr "pSphere1FollicleShape365.ot" "pSphere1Follicle365.t" -l on;
connectAttr "pSphere1FollicleShape365.or" "pSphere1Follicle365.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape365.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape365.inm";
connectAttr "curveShape11.ws" "pSphere1FollicleShape365.sp";
connectAttr "pSphere1FollicleShape371.ot" "pSphere1Follicle371.t" -l on;
connectAttr "pSphere1FollicleShape371.or" "pSphere1Follicle371.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape371.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape371.inm";
connectAttr "curveShape12.ws" "pSphere1FollicleShape371.sp";
connectAttr "pSphere1FollicleShape377.ot" "pSphere1Follicle377.t" -l on;
connectAttr "pSphere1FollicleShape377.or" "pSphere1Follicle377.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape377.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape377.inm";
connectAttr "curveShape13.ws" "pSphere1FollicleShape377.sp";
connectAttr "pSphere1FollicleShape384.ot" "pSphere1Follicle384.t" -l on;
connectAttr "pSphere1FollicleShape384.or" "pSphere1Follicle384.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape384.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape384.inm";
connectAttr "curveShape14.ws" "pSphere1FollicleShape384.sp";
connectAttr "pSphere1FollicleShape390.ot" "pSphere1Follicle390.t" -l on;
connectAttr "pSphere1FollicleShape390.or" "pSphere1Follicle390.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape390.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape390.inm";
connectAttr "curveShape15.ws" "pSphere1FollicleShape390.sp";
connectAttr "pSphere1FollicleShape396.ot" "pSphere1Follicle396.t" -l on;
connectAttr "pSphere1FollicleShape396.or" "pSphere1Follicle396.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape396.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape396.inm";
connectAttr "curveShape16.ws" "pSphere1FollicleShape396.sp";
connectAttr "pSphere1FollicleShape909.ot" "pSphere1Follicle909.t" -l on;
connectAttr "pSphere1FollicleShape909.or" "pSphere1Follicle909.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape909.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape909.inm";
connectAttr "curveShape17.ws" "pSphere1FollicleShape909.sp";
connectAttr "pSphere1FollicleShape915.ot" "pSphere1Follicle915.t" -l on;
connectAttr "pSphere1FollicleShape915.or" "pSphere1Follicle915.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape915.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape915.inm";
connectAttr "curveShape18.ws" "pSphere1FollicleShape915.sp";
connectAttr "pSphere1FollicleShape922.ot" "pSphere1Follicle922.t" -l on;
connectAttr "pSphere1FollicleShape922.or" "pSphere1Follicle922.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape922.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape922.inm";
connectAttr "curveShape19.ws" "pSphere1FollicleShape922.sp";
connectAttr "pSphere1FollicleShape928.ot" "pSphere1Follicle928.t" -l on;
connectAttr "pSphere1FollicleShape928.or" "pSphere1Follicle928.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape928.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape928.inm";
connectAttr "curveShape20.ws" "pSphere1FollicleShape928.sp";
connectAttr "pSphere1FollicleShape934.ot" "pSphere1Follicle934.t" -l on;
connectAttr "pSphere1FollicleShape934.or" "pSphere1Follicle934.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape934.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape934.inm";
connectAttr "curveShape21.ws" "pSphere1FollicleShape934.sp";
connectAttr "pSphere1FollicleShape940.ot" "pSphere1Follicle940.t" -l on;
connectAttr "pSphere1FollicleShape940.or" "pSphere1Follicle940.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape940.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape940.inm";
connectAttr "curveShape22.ws" "pSphere1FollicleShape940.sp";
connectAttr "pSphere1FollicleShape946.ot" "pSphere1Follicle946.t" -l on;
connectAttr "pSphere1FollicleShape946.or" "pSphere1Follicle946.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape946.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape946.inm";
connectAttr "curveShape23.ws" "pSphere1FollicleShape946.sp";
connectAttr "pSphere1FollicleShape953.ot" "pSphere1Follicle953.t" -l on;
connectAttr "pSphere1FollicleShape953.or" "pSphere1Follicle953.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape953.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape953.inm";
connectAttr "curveShape24.ws" "pSphere1FollicleShape953.sp";
connectAttr "pSphere1FollicleShape959.ot" "pSphere1Follicle959.t" -l on;
connectAttr "pSphere1FollicleShape959.or" "pSphere1Follicle959.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape959.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape959.inm";
connectAttr "curveShape25.ws" "pSphere1FollicleShape959.sp";
connectAttr "pSphere1FollicleShape965.ot" "pSphere1Follicle965.t" -l on;
connectAttr "pSphere1FollicleShape965.or" "pSphere1Follicle965.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape965.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape965.inm";
connectAttr "curveShape26.ws" "pSphere1FollicleShape965.sp";
connectAttr "pSphere1FollicleShape971.ot" "pSphere1Follicle971.t" -l on;
connectAttr "pSphere1FollicleShape971.or" "pSphere1Follicle971.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape971.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape971.inm";
connectAttr "curveShape27.ws" "pSphere1FollicleShape971.sp";
connectAttr "pSphere1FollicleShape977.ot" "pSphere1Follicle977.t" -l on;
connectAttr "pSphere1FollicleShape977.or" "pSphere1Follicle977.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape977.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape977.inm";
connectAttr "curveShape28.ws" "pSphere1FollicleShape977.sp";
connectAttr "pSphere1FollicleShape984.ot" "pSphere1Follicle984.t" -l on;
connectAttr "pSphere1FollicleShape984.or" "pSphere1Follicle984.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape984.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape984.inm";
connectAttr "curveShape29.ws" "pSphere1FollicleShape984.sp";
connectAttr "pSphere1FollicleShape990.ot" "pSphere1Follicle990.t" -l on;
connectAttr "pSphere1FollicleShape990.or" "pSphere1Follicle990.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape990.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape990.inm";
connectAttr "curveShape30.ws" "pSphere1FollicleShape990.sp";
connectAttr "pSphere1FollicleShape1509.ot" "pSphere1Follicle1509.t" -l on;
connectAttr "pSphere1FollicleShape1509.or" "pSphere1Follicle1509.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape1509.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape1509.inm";
connectAttr "curveShape31.ws" "pSphere1FollicleShape1509.sp";
connectAttr "pSphere1FollicleShape1515.ot" "pSphere1Follicle1515.t" -l on;
connectAttr "pSphere1FollicleShape1515.or" "pSphere1Follicle1515.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape1515.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape1515.inm";
connectAttr "curveShape32.ws" "pSphere1FollicleShape1515.sp";
connectAttr "pSphere1FollicleShape1522.ot" "pSphere1Follicle1522.t" -l on;
connectAttr "pSphere1FollicleShape1522.or" "pSphere1Follicle1522.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape1522.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape1522.inm";
connectAttr "curveShape33.ws" "pSphere1FollicleShape1522.sp";
connectAttr "pSphere1FollicleShape1528.ot" "pSphere1Follicle1528.t" -l on;
connectAttr "pSphere1FollicleShape1528.or" "pSphere1Follicle1528.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape1528.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape1528.inm";
connectAttr "curveShape34.ws" "pSphere1FollicleShape1528.sp";
connectAttr "pSphere1FollicleShape1534.ot" "pSphere1Follicle1534.t" -l on;
connectAttr "pSphere1FollicleShape1534.or" "pSphere1Follicle1534.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape1534.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape1534.inm";
connectAttr "curveShape35.ws" "pSphere1FollicleShape1534.sp";
connectAttr "pSphere1FollicleShape1540.ot" "pSphere1Follicle1540.t" -l on;
connectAttr "pSphere1FollicleShape1540.or" "pSphere1Follicle1540.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape1540.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape1540.inm";
connectAttr "curveShape36.ws" "pSphere1FollicleShape1540.sp";
connectAttr "pSphere1FollicleShape1546.ot" "pSphere1Follicle1546.t" -l on;
connectAttr "pSphere1FollicleShape1546.or" "pSphere1Follicle1546.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape1546.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape1546.inm";
connectAttr "curveShape37.ws" "pSphere1FollicleShape1546.sp";
connectAttr "pSphere1FollicleShape1553.ot" "pSphere1Follicle1553.t" -l on;
connectAttr "pSphere1FollicleShape1553.or" "pSphere1Follicle1553.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape1553.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape1553.inm";
connectAttr "curveShape38.ws" "pSphere1FollicleShape1553.sp";
connectAttr "pSphere1FollicleShape1559.ot" "pSphere1Follicle1559.t" -l on;
connectAttr "pSphere1FollicleShape1559.or" "pSphere1Follicle1559.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape1559.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape1559.inm";
connectAttr "curveShape39.ws" "pSphere1FollicleShape1559.sp";
connectAttr "pSphere1FollicleShape1565.ot" "pSphere1Follicle1565.t" -l on;
connectAttr "pSphere1FollicleShape1565.or" "pSphere1Follicle1565.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape1565.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape1565.inm";
connectAttr "curveShape40.ws" "pSphere1FollicleShape1565.sp";
connectAttr "pSphere1FollicleShape1571.ot" "pSphere1Follicle1571.t" -l on;
connectAttr "pSphere1FollicleShape1571.or" "pSphere1Follicle1571.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape1571.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape1571.inm";
connectAttr "curveShape41.ws" "pSphere1FollicleShape1571.sp";
connectAttr "pSphere1FollicleShape1577.ot" "pSphere1Follicle1577.t" -l on;
connectAttr "pSphere1FollicleShape1577.or" "pSphere1Follicle1577.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape1577.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape1577.inm";
connectAttr "curveShape42.ws" "pSphere1FollicleShape1577.sp";
connectAttr "pSphere1FollicleShape1584.ot" "pSphere1Follicle1584.t" -l on;
connectAttr "pSphere1FollicleShape1584.or" "pSphere1Follicle1584.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape1584.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape1584.inm";
connectAttr "curveShape43.ws" "pSphere1FollicleShape1584.sp";
connectAttr "pSphere1FollicleShape1590.ot" "pSphere1Follicle1590.t" -l on;
connectAttr "pSphere1FollicleShape1590.or" "pSphere1Follicle1590.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape1590.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape1590.inm";
connectAttr "curveShape44.ws" "pSphere1FollicleShape1590.sp";
connectAttr "pSphere1FollicleShape2203.ot" "pSphere1Follicle2203.t" -l on;
connectAttr "pSphere1FollicleShape2203.or" "pSphere1Follicle2203.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape2203.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape2203.inm";
connectAttr "curveShape45.ws" "pSphere1FollicleShape2203.sp";
connectAttr "pSphere1FollicleShape2209.ot" "pSphere1Follicle2209.t" -l on;
connectAttr "pSphere1FollicleShape2209.or" "pSphere1Follicle2209.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape2209.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape2209.inm";
connectAttr "curveShape46.ws" "pSphere1FollicleShape2209.sp";
connectAttr "pSphere1FollicleShape2215.ot" "pSphere1Follicle2215.t" -l on;
connectAttr "pSphere1FollicleShape2215.or" "pSphere1Follicle2215.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape2215.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape2215.inm";
connectAttr "curveShape47.ws" "pSphere1FollicleShape2215.sp";
connectAttr "pSphere1FollicleShape2222.ot" "pSphere1Follicle2222.t" -l on;
connectAttr "pSphere1FollicleShape2222.or" "pSphere1Follicle2222.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape2222.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape2222.inm";
connectAttr "curveShape48.ws" "pSphere1FollicleShape2222.sp";
connectAttr "pSphere1FollicleShape2228.ot" "pSphere1Follicle2228.t" -l on;
connectAttr "pSphere1FollicleShape2228.or" "pSphere1Follicle2228.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape2228.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape2228.inm";
connectAttr "curveShape49.ws" "pSphere1FollicleShape2228.sp";
connectAttr "pSphere1FollicleShape2234.ot" "pSphere1Follicle2234.t" -l on;
connectAttr "pSphere1FollicleShape2234.or" "pSphere1Follicle2234.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape2234.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape2234.inm";
connectAttr "curveShape50.ws" "pSphere1FollicleShape2234.sp";
connectAttr "pSphere1FollicleShape2240.ot" "pSphere1Follicle2240.t" -l on;
connectAttr "pSphere1FollicleShape2240.or" "pSphere1Follicle2240.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape2240.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape2240.inm";
connectAttr "curveShape51.ws" "pSphere1FollicleShape2240.sp";
connectAttr "pSphere1FollicleShape2246.ot" "pSphere1Follicle2246.t" -l on;
connectAttr "pSphere1FollicleShape2246.or" "pSphere1Follicle2246.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape2246.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape2246.inm";
connectAttr "curveShape52.ws" "pSphere1FollicleShape2246.sp";
connectAttr "pSphere1FollicleShape2253.ot" "pSphere1Follicle2253.t" -l on;
connectAttr "pSphere1FollicleShape2253.or" "pSphere1Follicle2253.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape2253.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape2253.inm";
connectAttr "curveShape53.ws" "pSphere1FollicleShape2253.sp";
connectAttr "pSphere1FollicleShape2259.ot" "pSphere1Follicle2259.t" -l on;
connectAttr "pSphere1FollicleShape2259.or" "pSphere1Follicle2259.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape2259.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape2259.inm";
connectAttr "curveShape54.ws" "pSphere1FollicleShape2259.sp";
connectAttr "pSphere1FollicleShape2265.ot" "pSphere1Follicle2265.t" -l on;
connectAttr "pSphere1FollicleShape2265.or" "pSphere1Follicle2265.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape2265.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape2265.inm";
connectAttr "curveShape55.ws" "pSphere1FollicleShape2265.sp";
connectAttr "pSphere1FollicleShape2271.ot" "pSphere1Follicle2271.t" -l on;
connectAttr "pSphere1FollicleShape2271.or" "pSphere1Follicle2271.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape2271.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape2271.inm";
connectAttr "curveShape56.ws" "pSphere1FollicleShape2271.sp";
connectAttr "pSphere1FollicleShape2277.ot" "pSphere1Follicle2277.t" -l on;
connectAttr "pSphere1FollicleShape2277.or" "pSphere1Follicle2277.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape2277.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape2277.inm";
connectAttr "curveShape57.ws" "pSphere1FollicleShape2277.sp";
connectAttr "pSphere1FollicleShape2284.ot" "pSphere1Follicle2284.t" -l on;
connectAttr "pSphere1FollicleShape2284.or" "pSphere1Follicle2284.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape2284.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape2284.inm";
connectAttr "curveShape58.ws" "pSphere1FollicleShape2284.sp";
connectAttr "pSphere1FollicleShape2290.ot" "pSphere1Follicle2290.t" -l on;
connectAttr "pSphere1FollicleShape2290.or" "pSphere1Follicle2290.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape2290.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape2290.inm";
connectAttr "curveShape59.ws" "pSphere1FollicleShape2290.sp";
connectAttr "pSphere1FollicleShape2296.ot" "pSphere1Follicle2296.t" -l on;
connectAttr "pSphere1FollicleShape2296.or" "pSphere1Follicle2296.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape2296.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape2296.inm";
connectAttr "curveShape60.ws" "pSphere1FollicleShape2296.sp";
connectAttr "pSphere1FollicleShape2803.ot" "pSphere1Follicle2803.t" -l on;
connectAttr "pSphere1FollicleShape2803.or" "pSphere1Follicle2803.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape2803.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape2803.inm";
connectAttr "curveShape61.ws" "pSphere1FollicleShape2803.sp";
connectAttr "pSphere1FollicleShape2809.ot" "pSphere1Follicle2809.t" -l on;
connectAttr "pSphere1FollicleShape2809.or" "pSphere1Follicle2809.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape2809.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape2809.inm";
connectAttr "curveShape62.ws" "pSphere1FollicleShape2809.sp";
connectAttr "pSphere1FollicleShape2815.ot" "pSphere1Follicle2815.t" -l on;
connectAttr "pSphere1FollicleShape2815.or" "pSphere1Follicle2815.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape2815.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape2815.inm";
connectAttr "curveShape63.ws" "pSphere1FollicleShape2815.sp";
connectAttr "pSphere1FollicleShape2822.ot" "pSphere1Follicle2822.t" -l on;
connectAttr "pSphere1FollicleShape2822.or" "pSphere1Follicle2822.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape2822.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape2822.inm";
connectAttr "curveShape64.ws" "pSphere1FollicleShape2822.sp";
connectAttr "pSphere1FollicleShape2828.ot" "pSphere1Follicle2828.t" -l on;
connectAttr "pSphere1FollicleShape2828.or" "pSphere1Follicle2828.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape2828.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape2828.inm";
connectAttr "curveShape65.ws" "pSphere1FollicleShape2828.sp";
connectAttr "pSphere1FollicleShape2834.ot" "pSphere1Follicle2834.t" -l on;
connectAttr "pSphere1FollicleShape2834.or" "pSphere1Follicle2834.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape2834.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape2834.inm";
connectAttr "curveShape66.ws" "pSphere1FollicleShape2834.sp";
connectAttr "pSphere1FollicleShape2840.ot" "pSphere1Follicle2840.t" -l on;
connectAttr "pSphere1FollicleShape2840.or" "pSphere1Follicle2840.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape2840.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape2840.inm";
connectAttr "curveShape67.ws" "pSphere1FollicleShape2840.sp";
connectAttr "pSphere1FollicleShape2846.ot" "pSphere1Follicle2846.t" -l on;
connectAttr "pSphere1FollicleShape2846.or" "pSphere1Follicle2846.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape2846.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape2846.inm";
connectAttr "curveShape68.ws" "pSphere1FollicleShape2846.sp";
connectAttr "pSphere1FollicleShape2853.ot" "pSphere1Follicle2853.t" -l on;
connectAttr "pSphere1FollicleShape2853.or" "pSphere1Follicle2853.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape2853.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape2853.inm";
connectAttr "curveShape69.ws" "pSphere1FollicleShape2853.sp";
connectAttr "pSphere1FollicleShape2859.ot" "pSphere1Follicle2859.t" -l on;
connectAttr "pSphere1FollicleShape2859.or" "pSphere1Follicle2859.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape2859.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape2859.inm";
connectAttr "curveShape70.ws" "pSphere1FollicleShape2859.sp";
connectAttr "pSphere1FollicleShape2865.ot" "pSphere1Follicle2865.t" -l on;
connectAttr "pSphere1FollicleShape2865.or" "pSphere1Follicle2865.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape2865.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape2865.inm";
connectAttr "curveShape71.ws" "pSphere1FollicleShape2865.sp";
connectAttr "pSphere1FollicleShape2871.ot" "pSphere1Follicle2871.t" -l on;
connectAttr "pSphere1FollicleShape2871.or" "pSphere1Follicle2871.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape2871.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape2871.inm";
connectAttr "curveShape72.ws" "pSphere1FollicleShape2871.sp";
connectAttr "pSphere1FollicleShape2877.ot" "pSphere1Follicle2877.t" -l on;
connectAttr "pSphere1FollicleShape2877.or" "pSphere1Follicle2877.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape2877.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape2877.inm";
connectAttr "curveShape73.ws" "pSphere1FollicleShape2877.sp";
connectAttr "pSphere1FollicleShape2884.ot" "pSphere1Follicle2884.t" -l on;
connectAttr "pSphere1FollicleShape2884.or" "pSphere1Follicle2884.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape2884.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape2884.inm";
connectAttr "curveShape74.ws" "pSphere1FollicleShape2884.sp";
connectAttr "pSphere1FollicleShape2890.ot" "pSphere1Follicle2890.t" -l on;
connectAttr "pSphere1FollicleShape2890.or" "pSphere1Follicle2890.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape2890.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape2890.inm";
connectAttr "curveShape75.ws" "pSphere1FollicleShape2890.sp";
connectAttr "pSphere1FollicleShape2896.ot" "pSphere1Follicle2896.t" -l on;
connectAttr "pSphere1FollicleShape2896.or" "pSphere1Follicle2896.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape2896.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape2896.inm";
connectAttr "curveShape76.ws" "pSphere1FollicleShape2896.sp";
connectAttr "pSphere1FollicleShape3409.ot" "pSphere1Follicle3409.t" -l on;
connectAttr "pSphere1FollicleShape3409.or" "pSphere1Follicle3409.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape3409.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape3409.inm";
connectAttr "curveShape77.ws" "pSphere1FollicleShape3409.sp";
connectAttr "pSphere1FollicleShape3415.ot" "pSphere1Follicle3415.t" -l on;
connectAttr "pSphere1FollicleShape3415.or" "pSphere1Follicle3415.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape3415.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape3415.inm";
connectAttr "curveShape78.ws" "pSphere1FollicleShape3415.sp";
connectAttr "pSphere1FollicleShape3422.ot" "pSphere1Follicle3422.t" -l on;
connectAttr "pSphere1FollicleShape3422.or" "pSphere1Follicle3422.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape3422.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape3422.inm";
connectAttr "curveShape79.ws" "pSphere1FollicleShape3422.sp";
connectAttr "pSphere1FollicleShape3428.ot" "pSphere1Follicle3428.t" -l on;
connectAttr "pSphere1FollicleShape3428.or" "pSphere1Follicle3428.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape3428.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape3428.inm";
connectAttr "curveShape80.ws" "pSphere1FollicleShape3428.sp";
connectAttr "pSphere1FollicleShape3434.ot" "pSphere1Follicle3434.t" -l on;
connectAttr "pSphere1FollicleShape3434.or" "pSphere1Follicle3434.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape3434.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape3434.inm";
connectAttr "curveShape81.ws" "pSphere1FollicleShape3434.sp";
connectAttr "pSphere1FollicleShape3440.ot" "pSphere1Follicle3440.t" -l on;
connectAttr "pSphere1FollicleShape3440.or" "pSphere1Follicle3440.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape3440.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape3440.inm";
connectAttr "curveShape82.ws" "pSphere1FollicleShape3440.sp";
connectAttr "pSphere1FollicleShape3446.ot" "pSphere1Follicle3446.t" -l on;
connectAttr "pSphere1FollicleShape3446.or" "pSphere1Follicle3446.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape3446.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape3446.inm";
connectAttr "curveShape83.ws" "pSphere1FollicleShape3446.sp";
connectAttr "pSphere1FollicleShape3453.ot" "pSphere1Follicle3453.t" -l on;
connectAttr "pSphere1FollicleShape3453.or" "pSphere1Follicle3453.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape3453.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape3453.inm";
connectAttr "curveShape84.ws" "pSphere1FollicleShape3453.sp";
connectAttr "pSphere1FollicleShape3459.ot" "pSphere1Follicle3459.t" -l on;
connectAttr "pSphere1FollicleShape3459.or" "pSphere1Follicle3459.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape3459.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape3459.inm";
connectAttr "curveShape85.ws" "pSphere1FollicleShape3459.sp";
connectAttr "pSphere1FollicleShape3465.ot" "pSphere1Follicle3465.t" -l on;
connectAttr "pSphere1FollicleShape3465.or" "pSphere1Follicle3465.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape3465.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape3465.inm";
connectAttr "curveShape86.ws" "pSphere1FollicleShape3465.sp";
connectAttr "pSphere1FollicleShape3471.ot" "pSphere1Follicle3471.t" -l on;
connectAttr "pSphere1FollicleShape3471.or" "pSphere1Follicle3471.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape3471.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape3471.inm";
connectAttr "curveShape87.ws" "pSphere1FollicleShape3471.sp";
connectAttr "pSphere1FollicleShape3477.ot" "pSphere1Follicle3477.t" -l on;
connectAttr "pSphere1FollicleShape3477.or" "pSphere1Follicle3477.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape3477.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape3477.inm";
connectAttr "curveShape88.ws" "pSphere1FollicleShape3477.sp";
connectAttr "pSphere1FollicleShape3484.ot" "pSphere1Follicle3484.t" -l on;
connectAttr "pSphere1FollicleShape3484.or" "pSphere1Follicle3484.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape3484.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape3484.inm";
connectAttr "curveShape89.ws" "pSphere1FollicleShape3484.sp";
connectAttr "pSphere1FollicleShape3490.ot" "pSphere1Follicle3490.t" -l on;
connectAttr "pSphere1FollicleShape3490.or" "pSphere1Follicle3490.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape3490.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape3490.inm";
connectAttr "curveShape90.ws" "pSphere1FollicleShape3490.sp";
connectAttr "pSphere1FollicleShape4009.ot" "pSphere1Follicle4009.t" -l on;
connectAttr "pSphere1FollicleShape4009.or" "pSphere1Follicle4009.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape4009.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape4009.inm";
connectAttr "curveShape91.ws" "pSphere1FollicleShape4009.sp";
connectAttr "pSphere1FollicleShape4015.ot" "pSphere1Follicle4015.t" -l on;
connectAttr "pSphere1FollicleShape4015.or" "pSphere1Follicle4015.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape4015.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape4015.inm";
connectAttr "curveShape92.ws" "pSphere1FollicleShape4015.sp";
connectAttr "pSphere1FollicleShape4022.ot" "pSphere1Follicle4022.t" -l on;
connectAttr "pSphere1FollicleShape4022.or" "pSphere1Follicle4022.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape4022.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape4022.inm";
connectAttr "curveShape93.ws" "pSphere1FollicleShape4022.sp";
connectAttr "pSphere1FollicleShape4028.ot" "pSphere1Follicle4028.t" -l on;
connectAttr "pSphere1FollicleShape4028.or" "pSphere1Follicle4028.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape4028.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape4028.inm";
connectAttr "curveShape94.ws" "pSphere1FollicleShape4028.sp";
connectAttr "pSphere1FollicleShape4034.ot" "pSphere1Follicle4034.t" -l on;
connectAttr "pSphere1FollicleShape4034.or" "pSphere1Follicle4034.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape4034.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape4034.inm";
connectAttr "curveShape95.ws" "pSphere1FollicleShape4034.sp";
connectAttr "pSphere1FollicleShape4040.ot" "pSphere1Follicle4040.t" -l on;
connectAttr "pSphere1FollicleShape4040.or" "pSphere1Follicle4040.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape4040.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape4040.inm";
connectAttr "curveShape96.ws" "pSphere1FollicleShape4040.sp";
connectAttr "pSphere1FollicleShape4046.ot" "pSphere1Follicle4046.t" -l on;
connectAttr "pSphere1FollicleShape4046.or" "pSphere1Follicle4046.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape4046.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape4046.inm";
connectAttr "curveShape97.ws" "pSphere1FollicleShape4046.sp";
connectAttr "pSphere1FollicleShape4053.ot" "pSphere1Follicle4053.t" -l on;
connectAttr "pSphere1FollicleShape4053.or" "pSphere1Follicle4053.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape4053.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape4053.inm";
connectAttr "curveShape98.ws" "pSphere1FollicleShape4053.sp";
connectAttr "pSphere1FollicleShape4059.ot" "pSphere1Follicle4059.t" -l on;
connectAttr "pSphere1FollicleShape4059.or" "pSphere1Follicle4059.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape4059.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape4059.inm";
connectAttr "curveShape99.ws" "pSphere1FollicleShape4059.sp";
connectAttr "pSphere1FollicleShape4065.ot" "pSphere1Follicle4065.t" -l on;
connectAttr "pSphere1FollicleShape4065.or" "pSphere1Follicle4065.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape4065.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape4065.inm";
connectAttr "curveShape100.ws" "pSphere1FollicleShape4065.sp";
connectAttr "pSphere1FollicleShape4071.ot" "pSphere1Follicle4071.t" -l on;
connectAttr "pSphere1FollicleShape4071.or" "pSphere1Follicle4071.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape4071.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape4071.inm";
connectAttr "curveShape101.ws" "pSphere1FollicleShape4071.sp";
connectAttr "pSphere1FollicleShape4077.ot" "pSphere1Follicle4077.t" -l on;
connectAttr "pSphere1FollicleShape4077.or" "pSphere1Follicle4077.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape4077.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape4077.inm";
connectAttr "curveShape102.ws" "pSphere1FollicleShape4077.sp";
connectAttr "pSphere1FollicleShape4084.ot" "pSphere1Follicle4084.t" -l on;
connectAttr "pSphere1FollicleShape4084.or" "pSphere1Follicle4084.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape4084.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape4084.inm";
connectAttr "curveShape103.ws" "pSphere1FollicleShape4084.sp";
connectAttr "pSphere1FollicleShape4090.ot" "pSphere1Follicle4090.t" -l on;
connectAttr "pSphere1FollicleShape4090.or" "pSphere1Follicle4090.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape4090.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape4090.inm";
connectAttr "curveShape104.ws" "pSphere1FollicleShape4090.sp";
connectAttr "pSphere1FollicleShape4603.ot" "pSphere1Follicle4603.t" -l on;
connectAttr "pSphere1FollicleShape4603.or" "pSphere1Follicle4603.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape4603.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape4603.inm";
connectAttr "curveShape105.ws" "pSphere1FollicleShape4603.sp";
connectAttr "pSphere1FollicleShape4609.ot" "pSphere1Follicle4609.t" -l on;
connectAttr "pSphere1FollicleShape4609.or" "pSphere1Follicle4609.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape4609.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape4609.inm";
connectAttr "curveShape106.ws" "pSphere1FollicleShape4609.sp";
connectAttr "pSphere1FollicleShape4615.ot" "pSphere1Follicle4615.t" -l on;
connectAttr "pSphere1FollicleShape4615.or" "pSphere1Follicle4615.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape4615.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape4615.inm";
connectAttr "curveShape107.ws" "pSphere1FollicleShape4615.sp";
connectAttr "pSphere1FollicleShape4622.ot" "pSphere1Follicle4622.t" -l on;
connectAttr "pSphere1FollicleShape4622.or" "pSphere1Follicle4622.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape4622.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape4622.inm";
connectAttr "curveShape108.ws" "pSphere1FollicleShape4622.sp";
connectAttr "pSphere1FollicleShape4628.ot" "pSphere1Follicle4628.t" -l on;
connectAttr "pSphere1FollicleShape4628.or" "pSphere1Follicle4628.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape4628.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape4628.inm";
connectAttr "curveShape109.ws" "pSphere1FollicleShape4628.sp";
connectAttr "pSphere1FollicleShape4634.ot" "pSphere1Follicle4634.t" -l on;
connectAttr "pSphere1FollicleShape4634.or" "pSphere1Follicle4634.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape4634.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape4634.inm";
connectAttr "curveShape110.ws" "pSphere1FollicleShape4634.sp";
connectAttr "pSphere1FollicleShape4640.ot" "pSphere1Follicle4640.t" -l on;
connectAttr "pSphere1FollicleShape4640.or" "pSphere1Follicle4640.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape4640.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape4640.inm";
connectAttr "curveShape111.ws" "pSphere1FollicleShape4640.sp";
connectAttr "pSphere1FollicleShape4646.ot" "pSphere1Follicle4646.t" -l on;
connectAttr "pSphere1FollicleShape4646.or" "pSphere1Follicle4646.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape4646.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape4646.inm";
connectAttr "curveShape112.ws" "pSphere1FollicleShape4646.sp";
connectAttr "pSphere1FollicleShape4653.ot" "pSphere1Follicle4653.t" -l on;
connectAttr "pSphere1FollicleShape4653.or" "pSphere1Follicle4653.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape4653.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape4653.inm";
connectAttr "curveShape113.ws" "pSphere1FollicleShape4653.sp";
connectAttr "pSphere1FollicleShape4659.ot" "pSphere1Follicle4659.t" -l on;
connectAttr "pSphere1FollicleShape4659.or" "pSphere1Follicle4659.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape4659.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape4659.inm";
connectAttr "curveShape114.ws" "pSphere1FollicleShape4659.sp";
connectAttr "pSphere1FollicleShape4665.ot" "pSphere1Follicle4665.t" -l on;
connectAttr "pSphere1FollicleShape4665.or" "pSphere1Follicle4665.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape4665.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape4665.inm";
connectAttr "curveShape115.ws" "pSphere1FollicleShape4665.sp";
connectAttr "pSphere1FollicleShape4671.ot" "pSphere1Follicle4671.t" -l on;
connectAttr "pSphere1FollicleShape4671.or" "pSphere1Follicle4671.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape4671.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape4671.inm";
connectAttr "curveShape116.ws" "pSphere1FollicleShape4671.sp";
connectAttr "pSphere1FollicleShape4677.ot" "pSphere1Follicle4677.t" -l on;
connectAttr "pSphere1FollicleShape4677.or" "pSphere1Follicle4677.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape4677.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape4677.inm";
connectAttr "curveShape117.ws" "pSphere1FollicleShape4677.sp";
connectAttr "pSphere1FollicleShape4684.ot" "pSphere1Follicle4684.t" -l on;
connectAttr "pSphere1FollicleShape4684.or" "pSphere1Follicle4684.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape4684.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape4684.inm";
connectAttr "curveShape118.ws" "pSphere1FollicleShape4684.sp";
connectAttr "pSphere1FollicleShape4690.ot" "pSphere1Follicle4690.t" -l on;
connectAttr "pSphere1FollicleShape4690.or" "pSphere1Follicle4690.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape4690.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape4690.inm";
connectAttr "curveShape119.ws" "pSphere1FollicleShape4690.sp";
connectAttr "pSphere1FollicleShape4696.ot" "pSphere1Follicle4696.t" -l on;
connectAttr "pSphere1FollicleShape4696.or" "pSphere1Follicle4696.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape4696.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape4696.inm";
connectAttr "curveShape120.ws" "pSphere1FollicleShape4696.sp";
connectAttr "pSphere1FollicleShape5303.ot" "pSphere1Follicle5303.t" -l on;
connectAttr "pSphere1FollicleShape5303.or" "pSphere1Follicle5303.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape5303.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape5303.inm";
connectAttr "curveShape121.ws" "pSphere1FollicleShape5303.sp";
connectAttr "pSphere1FollicleShape5309.ot" "pSphere1Follicle5309.t" -l on;
connectAttr "pSphere1FollicleShape5309.or" "pSphere1Follicle5309.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape5309.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape5309.inm";
connectAttr "curveShape122.ws" "pSphere1FollicleShape5309.sp";
connectAttr "pSphere1FollicleShape5315.ot" "pSphere1Follicle5315.t" -l on;
connectAttr "pSphere1FollicleShape5315.or" "pSphere1Follicle5315.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape5315.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape5315.inm";
connectAttr "curveShape123.ws" "pSphere1FollicleShape5315.sp";
connectAttr "pSphere1FollicleShape5322.ot" "pSphere1Follicle5322.t" -l on;
connectAttr "pSphere1FollicleShape5322.or" "pSphere1Follicle5322.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape5322.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape5322.inm";
connectAttr "curveShape124.ws" "pSphere1FollicleShape5322.sp";
connectAttr "pSphere1FollicleShape5328.ot" "pSphere1Follicle5328.t" -l on;
connectAttr "pSphere1FollicleShape5328.or" "pSphere1Follicle5328.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape5328.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape5328.inm";
connectAttr "curveShape125.ws" "pSphere1FollicleShape5328.sp";
connectAttr "pSphere1FollicleShape5334.ot" "pSphere1Follicle5334.t" -l on;
connectAttr "pSphere1FollicleShape5334.or" "pSphere1Follicle5334.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape5334.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape5334.inm";
connectAttr "curveShape126.ws" "pSphere1FollicleShape5334.sp";
connectAttr "pSphere1FollicleShape5340.ot" "pSphere1Follicle5340.t" -l on;
connectAttr "pSphere1FollicleShape5340.or" "pSphere1Follicle5340.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape5340.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape5340.inm";
connectAttr "curveShape127.ws" "pSphere1FollicleShape5340.sp";
connectAttr "pSphere1FollicleShape5346.ot" "pSphere1Follicle5346.t" -l on;
connectAttr "pSphere1FollicleShape5346.or" "pSphere1Follicle5346.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape5346.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape5346.inm";
connectAttr "curveShape128.ws" "pSphere1FollicleShape5346.sp";
connectAttr "pSphere1FollicleShape5353.ot" "pSphere1Follicle5353.t" -l on;
connectAttr "pSphere1FollicleShape5353.or" "pSphere1Follicle5353.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape5353.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape5353.inm";
connectAttr "curveShape129.ws" "pSphere1FollicleShape5353.sp";
connectAttr "pSphere1FollicleShape5359.ot" "pSphere1Follicle5359.t" -l on;
connectAttr "pSphere1FollicleShape5359.or" "pSphere1Follicle5359.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape5359.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape5359.inm";
connectAttr "curveShape130.ws" "pSphere1FollicleShape5359.sp";
connectAttr "pSphere1FollicleShape5365.ot" "pSphere1Follicle5365.t" -l on;
connectAttr "pSphere1FollicleShape5365.or" "pSphere1Follicle5365.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape5365.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape5365.inm";
connectAttr "curveShape131.ws" "pSphere1FollicleShape5365.sp";
connectAttr "pSphere1FollicleShape5371.ot" "pSphere1Follicle5371.t" -l on;
connectAttr "pSphere1FollicleShape5371.or" "pSphere1Follicle5371.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape5371.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape5371.inm";
connectAttr "curveShape132.ws" "pSphere1FollicleShape5371.sp";
connectAttr "pSphere1FollicleShape5377.ot" "pSphere1Follicle5377.t" -l on;
connectAttr "pSphere1FollicleShape5377.or" "pSphere1Follicle5377.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape5377.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape5377.inm";
connectAttr "curveShape133.ws" "pSphere1FollicleShape5377.sp";
connectAttr "pSphere1FollicleShape5384.ot" "pSphere1Follicle5384.t" -l on;
connectAttr "pSphere1FollicleShape5384.or" "pSphere1Follicle5384.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape5384.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape5384.inm";
connectAttr "curveShape134.ws" "pSphere1FollicleShape5384.sp";
connectAttr "pSphere1FollicleShape5390.ot" "pSphere1Follicle5390.t" -l on;
connectAttr "pSphere1FollicleShape5390.or" "pSphere1Follicle5390.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape5390.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape5390.inm";
connectAttr "curveShape135.ws" "pSphere1FollicleShape5390.sp";
connectAttr "pSphere1FollicleShape5396.ot" "pSphere1Follicle5396.t" -l on;
connectAttr "pSphere1FollicleShape5396.or" "pSphere1Follicle5396.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape5396.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape5396.inm";
connectAttr "curveShape136.ws" "pSphere1FollicleShape5396.sp";
connectAttr "pSphere1FollicleShape5909.ot" "pSphere1Follicle5909.t" -l on;
connectAttr "pSphere1FollicleShape5909.or" "pSphere1Follicle5909.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape5909.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape5909.inm";
connectAttr "curveShape137.ws" "pSphere1FollicleShape5909.sp";
connectAttr "pSphere1FollicleShape5915.ot" "pSphere1Follicle5915.t" -l on;
connectAttr "pSphere1FollicleShape5915.or" "pSphere1Follicle5915.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape5915.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape5915.inm";
connectAttr "curveShape138.ws" "pSphere1FollicleShape5915.sp";
connectAttr "pSphere1FollicleShape5922.ot" "pSphere1Follicle5922.t" -l on;
connectAttr "pSphere1FollicleShape5922.or" "pSphere1Follicle5922.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape5922.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape5922.inm";
connectAttr "curveShape139.ws" "pSphere1FollicleShape5922.sp";
connectAttr "pSphere1FollicleShape5928.ot" "pSphere1Follicle5928.t" -l on;
connectAttr "pSphere1FollicleShape5928.or" "pSphere1Follicle5928.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape5928.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape5928.inm";
connectAttr "curveShape140.ws" "pSphere1FollicleShape5928.sp";
connectAttr "pSphere1FollicleShape5934.ot" "pSphere1Follicle5934.t" -l on;
connectAttr "pSphere1FollicleShape5934.or" "pSphere1Follicle5934.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape5934.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape5934.inm";
connectAttr "curveShape141.ws" "pSphere1FollicleShape5934.sp";
connectAttr "pSphere1FollicleShape5940.ot" "pSphere1Follicle5940.t" -l on;
connectAttr "pSphere1FollicleShape5940.or" "pSphere1Follicle5940.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape5940.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape5940.inm";
connectAttr "curveShape142.ws" "pSphere1FollicleShape5940.sp";
connectAttr "pSphere1FollicleShape5946.ot" "pSphere1Follicle5946.t" -l on;
connectAttr "pSphere1FollicleShape5946.or" "pSphere1Follicle5946.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape5946.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape5946.inm";
connectAttr "curveShape143.ws" "pSphere1FollicleShape5946.sp";
connectAttr "pSphere1FollicleShape5953.ot" "pSphere1Follicle5953.t" -l on;
connectAttr "pSphere1FollicleShape5953.or" "pSphere1Follicle5953.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape5953.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape5953.inm";
connectAttr "curveShape144.ws" "pSphere1FollicleShape5953.sp";
connectAttr "pSphere1FollicleShape5959.ot" "pSphere1Follicle5959.t" -l on;
connectAttr "pSphere1FollicleShape5959.or" "pSphere1Follicle5959.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape5959.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape5959.inm";
connectAttr "curveShape145.ws" "pSphere1FollicleShape5959.sp";
connectAttr "pSphere1FollicleShape5965.ot" "pSphere1Follicle5965.t" -l on;
connectAttr "pSphere1FollicleShape5965.or" "pSphere1Follicle5965.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape5965.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape5965.inm";
connectAttr "curveShape146.ws" "pSphere1FollicleShape5965.sp";
connectAttr "pSphere1FollicleShape5971.ot" "pSphere1Follicle5971.t" -l on;
connectAttr "pSphere1FollicleShape5971.or" "pSphere1Follicle5971.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape5971.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape5971.inm";
connectAttr "curveShape147.ws" "pSphere1FollicleShape5971.sp";
connectAttr "pSphere1FollicleShape5977.ot" "pSphere1Follicle5977.t" -l on;
connectAttr "pSphere1FollicleShape5977.or" "pSphere1Follicle5977.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape5977.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape5977.inm";
connectAttr "curveShape148.ws" "pSphere1FollicleShape5977.sp";
connectAttr "pSphere1FollicleShape5984.ot" "pSphere1Follicle5984.t" -l on;
connectAttr "pSphere1FollicleShape5984.or" "pSphere1Follicle5984.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape5984.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape5984.inm";
connectAttr "curveShape149.ws" "pSphere1FollicleShape5984.sp";
connectAttr "pSphere1FollicleShape5990.ot" "pSphere1Follicle5990.t" -l on;
connectAttr "pSphere1FollicleShape5990.or" "pSphere1Follicle5990.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape5990.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape5990.inm";
connectAttr "curveShape150.ws" "pSphere1FollicleShape5990.sp";
connectAttr "pSphere1FollicleShape6509.ot" "pSphere1Follicle6509.t" -l on;
connectAttr "pSphere1FollicleShape6509.or" "pSphere1Follicle6509.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape6509.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape6509.inm";
connectAttr "curveShape151.ws" "pSphere1FollicleShape6509.sp";
connectAttr "pSphere1FollicleShape6515.ot" "pSphere1Follicle6515.t" -l on;
connectAttr "pSphere1FollicleShape6515.or" "pSphere1Follicle6515.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape6515.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape6515.inm";
connectAttr "curveShape152.ws" "pSphere1FollicleShape6515.sp";
connectAttr "pSphere1FollicleShape6522.ot" "pSphere1Follicle6522.t" -l on;
connectAttr "pSphere1FollicleShape6522.or" "pSphere1Follicle6522.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape6522.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape6522.inm";
connectAttr "curveShape153.ws" "pSphere1FollicleShape6522.sp";
connectAttr "pSphere1FollicleShape6528.ot" "pSphere1Follicle6528.t" -l on;
connectAttr "pSphere1FollicleShape6528.or" "pSphere1Follicle6528.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape6528.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape6528.inm";
connectAttr "curveShape154.ws" "pSphere1FollicleShape6528.sp";
connectAttr "pSphere1FollicleShape6534.ot" "pSphere1Follicle6534.t" -l on;
connectAttr "pSphere1FollicleShape6534.or" "pSphere1Follicle6534.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape6534.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape6534.inm";
connectAttr "curveShape155.ws" "pSphere1FollicleShape6534.sp";
connectAttr "pSphere1FollicleShape6540.ot" "pSphere1Follicle6540.t" -l on;
connectAttr "pSphere1FollicleShape6540.or" "pSphere1Follicle6540.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape6540.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape6540.inm";
connectAttr "curveShape156.ws" "pSphere1FollicleShape6540.sp";
connectAttr "pSphere1FollicleShape6546.ot" "pSphere1Follicle6546.t" -l on;
connectAttr "pSphere1FollicleShape6546.or" "pSphere1Follicle6546.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape6546.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape6546.inm";
connectAttr "curveShape157.ws" "pSphere1FollicleShape6546.sp";
connectAttr "pSphere1FollicleShape6553.ot" "pSphere1Follicle6553.t" -l on;
connectAttr "pSphere1FollicleShape6553.or" "pSphere1Follicle6553.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape6553.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape6553.inm";
connectAttr "curveShape158.ws" "pSphere1FollicleShape6553.sp";
connectAttr "pSphere1FollicleShape6559.ot" "pSphere1Follicle6559.t" -l on;
connectAttr "pSphere1FollicleShape6559.or" "pSphere1Follicle6559.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape6559.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape6559.inm";
connectAttr "curveShape159.ws" "pSphere1FollicleShape6559.sp";
connectAttr "pSphere1FollicleShape6565.ot" "pSphere1Follicle6565.t" -l on;
connectAttr "pSphere1FollicleShape6565.or" "pSphere1Follicle6565.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape6565.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape6565.inm";
connectAttr "curveShape160.ws" "pSphere1FollicleShape6565.sp";
connectAttr "pSphere1FollicleShape6571.ot" "pSphere1Follicle6571.t" -l on;
connectAttr "pSphere1FollicleShape6571.or" "pSphere1Follicle6571.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape6571.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape6571.inm";
connectAttr "curveShape161.ws" "pSphere1FollicleShape6571.sp";
connectAttr "pSphere1FollicleShape6577.ot" "pSphere1Follicle6577.t" -l on;
connectAttr "pSphere1FollicleShape6577.or" "pSphere1Follicle6577.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape6577.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape6577.inm";
connectAttr "curveShape162.ws" "pSphere1FollicleShape6577.sp";
connectAttr "pSphere1FollicleShape6584.ot" "pSphere1Follicle6584.t" -l on;
connectAttr "pSphere1FollicleShape6584.or" "pSphere1Follicle6584.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape6584.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape6584.inm";
connectAttr "curveShape163.ws" "pSphere1FollicleShape6584.sp";
connectAttr "pSphere1FollicleShape6590.ot" "pSphere1Follicle6590.t" -l on;
connectAttr "pSphere1FollicleShape6590.or" "pSphere1Follicle6590.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape6590.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape6590.inm";
connectAttr "curveShape164.ws" "pSphere1FollicleShape6590.sp";
connectAttr "pSphere1FollicleShape7103.ot" "pSphere1Follicle7103.t" -l on;
connectAttr "pSphere1FollicleShape7103.or" "pSphere1Follicle7103.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape7103.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape7103.inm";
connectAttr "curveShape165.ws" "pSphere1FollicleShape7103.sp";
connectAttr "pSphere1FollicleShape7109.ot" "pSphere1Follicle7109.t" -l on;
connectAttr "pSphere1FollicleShape7109.or" "pSphere1Follicle7109.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape7109.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape7109.inm";
connectAttr "curveShape166.ws" "pSphere1FollicleShape7109.sp";
connectAttr "pSphere1FollicleShape7115.ot" "pSphere1Follicle7115.t" -l on;
connectAttr "pSphere1FollicleShape7115.or" "pSphere1Follicle7115.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape7115.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape7115.inm";
connectAttr "curveShape167.ws" "pSphere1FollicleShape7115.sp";
connectAttr "pSphere1FollicleShape7122.ot" "pSphere1Follicle7122.t" -l on;
connectAttr "pSphere1FollicleShape7122.or" "pSphere1Follicle7122.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape7122.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape7122.inm";
connectAttr "curveShape168.ws" "pSphere1FollicleShape7122.sp";
connectAttr "pSphere1FollicleShape7128.ot" "pSphere1Follicle7128.t" -l on;
connectAttr "pSphere1FollicleShape7128.or" "pSphere1Follicle7128.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape7128.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape7128.inm";
connectAttr "curveShape169.ws" "pSphere1FollicleShape7128.sp";
connectAttr "pSphere1FollicleShape7134.ot" "pSphere1Follicle7134.t" -l on;
connectAttr "pSphere1FollicleShape7134.or" "pSphere1Follicle7134.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape7134.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape7134.inm";
connectAttr "curveShape170.ws" "pSphere1FollicleShape7134.sp";
connectAttr "pSphere1FollicleShape7140.ot" "pSphere1Follicle7140.t" -l on;
connectAttr "pSphere1FollicleShape7140.or" "pSphere1Follicle7140.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape7140.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape7140.inm";
connectAttr "curveShape171.ws" "pSphere1FollicleShape7140.sp";
connectAttr "pSphere1FollicleShape7146.ot" "pSphere1Follicle7146.t" -l on;
connectAttr "pSphere1FollicleShape7146.or" "pSphere1Follicle7146.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape7146.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape7146.inm";
connectAttr "curveShape172.ws" "pSphere1FollicleShape7146.sp";
connectAttr "pSphere1FollicleShape7153.ot" "pSphere1Follicle7153.t" -l on;
connectAttr "pSphere1FollicleShape7153.or" "pSphere1Follicle7153.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape7153.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape7153.inm";
connectAttr "curveShape173.ws" "pSphere1FollicleShape7153.sp";
connectAttr "pSphere1FollicleShape7159.ot" "pSphere1Follicle7159.t" -l on;
connectAttr "pSphere1FollicleShape7159.or" "pSphere1Follicle7159.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape7159.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape7159.inm";
connectAttr "curveShape174.ws" "pSphere1FollicleShape7159.sp";
connectAttr "pSphere1FollicleShape7165.ot" "pSphere1Follicle7165.t" -l on;
connectAttr "pSphere1FollicleShape7165.or" "pSphere1Follicle7165.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape7165.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape7165.inm";
connectAttr "curveShape175.ws" "pSphere1FollicleShape7165.sp";
connectAttr "pSphere1FollicleShape7171.ot" "pSphere1Follicle7171.t" -l on;
connectAttr "pSphere1FollicleShape7171.or" "pSphere1Follicle7171.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape7171.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape7171.inm";
connectAttr "curveShape176.ws" "pSphere1FollicleShape7171.sp";
connectAttr "pSphere1FollicleShape7177.ot" "pSphere1Follicle7177.t" -l on;
connectAttr "pSphere1FollicleShape7177.or" "pSphere1Follicle7177.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape7177.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape7177.inm";
connectAttr "curveShape177.ws" "pSphere1FollicleShape7177.sp";
connectAttr "pSphere1FollicleShape7184.ot" "pSphere1Follicle7184.t" -l on;
connectAttr "pSphere1FollicleShape7184.or" "pSphere1Follicle7184.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape7184.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape7184.inm";
connectAttr "curveShape178.ws" "pSphere1FollicleShape7184.sp";
connectAttr "pSphere1FollicleShape7190.ot" "pSphere1Follicle7190.t" -l on;
connectAttr "pSphere1FollicleShape7190.or" "pSphere1Follicle7190.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape7190.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape7190.inm";
connectAttr "curveShape179.ws" "pSphere1FollicleShape7190.sp";
connectAttr "pSphere1FollicleShape7196.ot" "pSphere1Follicle7196.t" -l on;
connectAttr "pSphere1FollicleShape7196.or" "pSphere1Follicle7196.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape7196.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape7196.inm";
connectAttr "curveShape180.ws" "pSphere1FollicleShape7196.sp";
connectAttr "pSphere1FollicleShape7703.ot" "pSphere1Follicle7703.t" -l on;
connectAttr "pSphere1FollicleShape7703.or" "pSphere1Follicle7703.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape7703.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape7703.inm";
connectAttr "curveShape181.ws" "pSphere1FollicleShape7703.sp";
connectAttr "pSphere1FollicleShape7709.ot" "pSphere1Follicle7709.t" -l on;
connectAttr "pSphere1FollicleShape7709.or" "pSphere1Follicle7709.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape7709.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape7709.inm";
connectAttr "curveShape182.ws" "pSphere1FollicleShape7709.sp";
connectAttr "pSphere1FollicleShape7715.ot" "pSphere1Follicle7715.t" -l on;
connectAttr "pSphere1FollicleShape7715.or" "pSphere1Follicle7715.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape7715.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape7715.inm";
connectAttr "curveShape183.ws" "pSphere1FollicleShape7715.sp";
connectAttr "pSphere1FollicleShape7722.ot" "pSphere1Follicle7722.t" -l on;
connectAttr "pSphere1FollicleShape7722.or" "pSphere1Follicle7722.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape7722.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape7722.inm";
connectAttr "curveShape184.ws" "pSphere1FollicleShape7722.sp";
connectAttr "pSphere1FollicleShape7728.ot" "pSphere1Follicle7728.t" -l on;
connectAttr "pSphere1FollicleShape7728.or" "pSphere1Follicle7728.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape7728.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape7728.inm";
connectAttr "curveShape185.ws" "pSphere1FollicleShape7728.sp";
connectAttr "pSphere1FollicleShape7734.ot" "pSphere1Follicle7734.t" -l on;
connectAttr "pSphere1FollicleShape7734.or" "pSphere1Follicle7734.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape7734.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape7734.inm";
connectAttr "curveShape186.ws" "pSphere1FollicleShape7734.sp";
connectAttr "pSphere1FollicleShape7740.ot" "pSphere1Follicle7740.t" -l on;
connectAttr "pSphere1FollicleShape7740.or" "pSphere1Follicle7740.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape7740.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape7740.inm";
connectAttr "curveShape187.ws" "pSphere1FollicleShape7740.sp";
connectAttr "pSphere1FollicleShape7746.ot" "pSphere1Follicle7746.t" -l on;
connectAttr "pSphere1FollicleShape7746.or" "pSphere1Follicle7746.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape7746.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape7746.inm";
connectAttr "curveShape188.ws" "pSphere1FollicleShape7746.sp";
connectAttr "pSphere1FollicleShape7753.ot" "pSphere1Follicle7753.t" -l on;
connectAttr "pSphere1FollicleShape7753.or" "pSphere1Follicle7753.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape7753.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape7753.inm";
connectAttr "curveShape189.ws" "pSphere1FollicleShape7753.sp";
connectAttr "pSphere1FollicleShape7759.ot" "pSphere1Follicle7759.t" -l on;
connectAttr "pSphere1FollicleShape7759.or" "pSphere1Follicle7759.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape7759.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape7759.inm";
connectAttr "curveShape190.ws" "pSphere1FollicleShape7759.sp";
connectAttr "pSphere1FollicleShape7765.ot" "pSphere1Follicle7765.t" -l on;
connectAttr "pSphere1FollicleShape7765.or" "pSphere1Follicle7765.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape7765.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape7765.inm";
connectAttr "curveShape191.ws" "pSphere1FollicleShape7765.sp";
connectAttr "pSphere1FollicleShape7771.ot" "pSphere1Follicle7771.t" -l on;
connectAttr "pSphere1FollicleShape7771.or" "pSphere1Follicle7771.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape7771.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape7771.inm";
connectAttr "curveShape192.ws" "pSphere1FollicleShape7771.sp";
connectAttr "pSphere1FollicleShape7777.ot" "pSphere1Follicle7777.t" -l on;
connectAttr "pSphere1FollicleShape7777.or" "pSphere1Follicle7777.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape7777.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape7777.inm";
connectAttr "curveShape193.ws" "pSphere1FollicleShape7777.sp";
connectAttr "pSphere1FollicleShape7784.ot" "pSphere1Follicle7784.t" -l on;
connectAttr "pSphere1FollicleShape7784.or" "pSphere1Follicle7784.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape7784.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape7784.inm";
connectAttr "curveShape194.ws" "pSphere1FollicleShape7784.sp";
connectAttr "pSphere1FollicleShape7790.ot" "pSphere1Follicle7790.t" -l on;
connectAttr "pSphere1FollicleShape7790.or" "pSphere1Follicle7790.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape7790.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape7790.inm";
connectAttr "curveShape195.ws" "pSphere1FollicleShape7790.sp";
connectAttr "pSphere1FollicleShape7796.ot" "pSphere1Follicle7796.t" -l on;
connectAttr "pSphere1FollicleShape7796.or" "pSphere1Follicle7796.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape7796.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape7796.inm";
connectAttr "curveShape196.ws" "pSphere1FollicleShape7796.sp";
connectAttr "pSphere1FollicleShape8409.ot" "pSphere1Follicle8409.t" -l on;
connectAttr "pSphere1FollicleShape8409.or" "pSphere1Follicle8409.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape8409.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape8409.inm";
connectAttr "curveShape197.ws" "pSphere1FollicleShape8409.sp";
connectAttr "pSphere1FollicleShape8415.ot" "pSphere1Follicle8415.t" -l on;
connectAttr "pSphere1FollicleShape8415.or" "pSphere1Follicle8415.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape8415.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape8415.inm";
connectAttr "curveShape198.ws" "pSphere1FollicleShape8415.sp";
connectAttr "pSphere1FollicleShape8422.ot" "pSphere1Follicle8422.t" -l on;
connectAttr "pSphere1FollicleShape8422.or" "pSphere1Follicle8422.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape8422.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape8422.inm";
connectAttr "curveShape199.ws" "pSphere1FollicleShape8422.sp";
connectAttr "pSphere1FollicleShape8428.ot" "pSphere1Follicle8428.t" -l on;
connectAttr "pSphere1FollicleShape8428.or" "pSphere1Follicle8428.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape8428.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape8428.inm";
connectAttr "curveShape200.ws" "pSphere1FollicleShape8428.sp";
connectAttr "pSphere1FollicleShape8434.ot" "pSphere1Follicle8434.t" -l on;
connectAttr "pSphere1FollicleShape8434.or" "pSphere1Follicle8434.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape8434.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape8434.inm";
connectAttr "curveShape201.ws" "pSphere1FollicleShape8434.sp";
connectAttr "pSphere1FollicleShape8440.ot" "pSphere1Follicle8440.t" -l on;
connectAttr "pSphere1FollicleShape8440.or" "pSphere1Follicle8440.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape8440.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape8440.inm";
connectAttr "curveShape202.ws" "pSphere1FollicleShape8440.sp";
connectAttr "pSphere1FollicleShape8446.ot" "pSphere1Follicle8446.t" -l on;
connectAttr "pSphere1FollicleShape8446.or" "pSphere1Follicle8446.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape8446.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape8446.inm";
connectAttr "curveShape203.ws" "pSphere1FollicleShape8446.sp";
connectAttr "pSphere1FollicleShape8453.ot" "pSphere1Follicle8453.t" -l on;
connectAttr "pSphere1FollicleShape8453.or" "pSphere1Follicle8453.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape8453.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape8453.inm";
connectAttr "curveShape204.ws" "pSphere1FollicleShape8453.sp";
connectAttr "pSphere1FollicleShape8459.ot" "pSphere1Follicle8459.t" -l on;
connectAttr "pSphere1FollicleShape8459.or" "pSphere1Follicle8459.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape8459.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape8459.inm";
connectAttr "curveShape205.ws" "pSphere1FollicleShape8459.sp";
connectAttr "pSphere1FollicleShape8465.ot" "pSphere1Follicle8465.t" -l on;
connectAttr "pSphere1FollicleShape8465.or" "pSphere1Follicle8465.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape8465.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape8465.inm";
connectAttr "curveShape206.ws" "pSphere1FollicleShape8465.sp";
connectAttr "pSphere1FollicleShape8471.ot" "pSphere1Follicle8471.t" -l on;
connectAttr "pSphere1FollicleShape8471.or" "pSphere1Follicle8471.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape8471.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape8471.inm";
connectAttr "curveShape207.ws" "pSphere1FollicleShape8471.sp";
connectAttr "pSphere1FollicleShape8477.ot" "pSphere1Follicle8477.t" -l on;
connectAttr "pSphere1FollicleShape8477.or" "pSphere1Follicle8477.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape8477.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape8477.inm";
connectAttr "curveShape208.ws" "pSphere1FollicleShape8477.sp";
connectAttr "pSphere1FollicleShape8484.ot" "pSphere1Follicle8484.t" -l on;
connectAttr "pSphere1FollicleShape8484.or" "pSphere1Follicle8484.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape8484.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape8484.inm";
connectAttr "curveShape209.ws" "pSphere1FollicleShape8484.sp";
connectAttr "pSphere1FollicleShape8490.ot" "pSphere1Follicle8490.t" -l on;
connectAttr "pSphere1FollicleShape8490.or" "pSphere1Follicle8490.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape8490.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape8490.inm";
connectAttr "curveShape210.ws" "pSphere1FollicleShape8490.sp";
connectAttr "pSphere1FollicleShape9009.ot" "pSphere1Follicle9009.t" -l on;
connectAttr "pSphere1FollicleShape9009.or" "pSphere1Follicle9009.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape9009.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape9009.inm";
connectAttr "curveShape211.ws" "pSphere1FollicleShape9009.sp";
connectAttr "pSphere1FollicleShape9015.ot" "pSphere1Follicle9015.t" -l on;
connectAttr "pSphere1FollicleShape9015.or" "pSphere1Follicle9015.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape9015.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape9015.inm";
connectAttr "curveShape212.ws" "pSphere1FollicleShape9015.sp";
connectAttr "pSphere1FollicleShape9022.ot" "pSphere1Follicle9022.t" -l on;
connectAttr "pSphere1FollicleShape9022.or" "pSphere1Follicle9022.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape9022.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape9022.inm";
connectAttr "curveShape213.ws" "pSphere1FollicleShape9022.sp";
connectAttr "pSphere1FollicleShape9028.ot" "pSphere1Follicle9028.t" -l on;
connectAttr "pSphere1FollicleShape9028.or" "pSphere1Follicle9028.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape9028.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape9028.inm";
connectAttr "curveShape214.ws" "pSphere1FollicleShape9028.sp";
connectAttr "pSphere1FollicleShape9034.ot" "pSphere1Follicle9034.t" -l on;
connectAttr "pSphere1FollicleShape9034.or" "pSphere1Follicle9034.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape9034.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape9034.inm";
connectAttr "curveShape215.ws" "pSphere1FollicleShape9034.sp";
connectAttr "pSphere1FollicleShape9040.ot" "pSphere1Follicle9040.t" -l on;
connectAttr "pSphere1FollicleShape9040.or" "pSphere1Follicle9040.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape9040.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape9040.inm";
connectAttr "curveShape216.ws" "pSphere1FollicleShape9040.sp";
connectAttr "pSphere1FollicleShape9046.ot" "pSphere1Follicle9046.t" -l on;
connectAttr "pSphere1FollicleShape9046.or" "pSphere1Follicle9046.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape9046.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape9046.inm";
connectAttr "curveShape217.ws" "pSphere1FollicleShape9046.sp";
connectAttr "pSphere1FollicleShape9053.ot" "pSphere1Follicle9053.t" -l on;
connectAttr "pSphere1FollicleShape9053.or" "pSphere1Follicle9053.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape9053.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape9053.inm";
connectAttr "curveShape218.ws" "pSphere1FollicleShape9053.sp";
connectAttr "pSphere1FollicleShape9059.ot" "pSphere1Follicle9059.t" -l on;
connectAttr "pSphere1FollicleShape9059.or" "pSphere1Follicle9059.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape9059.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape9059.inm";
connectAttr "curveShape219.ws" "pSphere1FollicleShape9059.sp";
connectAttr "pSphere1FollicleShape9065.ot" "pSphere1Follicle9065.t" -l on;
connectAttr "pSphere1FollicleShape9065.or" "pSphere1Follicle9065.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape9065.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape9065.inm";
connectAttr "curveShape220.ws" "pSphere1FollicleShape9065.sp";
connectAttr "pSphere1FollicleShape9071.ot" "pSphere1Follicle9071.t" -l on;
connectAttr "pSphere1FollicleShape9071.or" "pSphere1Follicle9071.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape9071.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape9071.inm";
connectAttr "curveShape221.ws" "pSphere1FollicleShape9071.sp";
connectAttr "pSphere1FollicleShape9077.ot" "pSphere1Follicle9077.t" -l on;
connectAttr "pSphere1FollicleShape9077.or" "pSphere1Follicle9077.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape9077.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape9077.inm";
connectAttr "curveShape222.ws" "pSphere1FollicleShape9077.sp";
connectAttr "pSphere1FollicleShape9084.ot" "pSphere1Follicle9084.t" -l on;
connectAttr "pSphere1FollicleShape9084.or" "pSphere1Follicle9084.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape9084.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape9084.inm";
connectAttr "curveShape223.ws" "pSphere1FollicleShape9084.sp";
connectAttr "pSphere1FollicleShape9090.ot" "pSphere1Follicle9090.t" -l on;
connectAttr "pSphere1FollicleShape9090.or" "pSphere1Follicle9090.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape9090.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape9090.inm";
connectAttr "curveShape224.ws" "pSphere1FollicleShape9090.sp";
connectAttr "pSphere1FollicleShape9603.ot" "pSphere1Follicle9603.t" -l on;
connectAttr "pSphere1FollicleShape9603.or" "pSphere1Follicle9603.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape9603.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape9603.inm";
connectAttr "curveShape225.ws" "pSphere1FollicleShape9603.sp";
connectAttr "pSphere1FollicleShape9609.ot" "pSphere1Follicle9609.t" -l on;
connectAttr "pSphere1FollicleShape9609.or" "pSphere1Follicle9609.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape9609.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape9609.inm";
connectAttr "curveShape226.ws" "pSphere1FollicleShape9609.sp";
connectAttr "pSphere1FollicleShape9615.ot" "pSphere1Follicle9615.t" -l on;
connectAttr "pSphere1FollicleShape9615.or" "pSphere1Follicle9615.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape9615.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape9615.inm";
connectAttr "curveShape227.ws" "pSphere1FollicleShape9615.sp";
connectAttr "pSphere1FollicleShape9622.ot" "pSphere1Follicle9622.t" -l on;
connectAttr "pSphere1FollicleShape9622.or" "pSphere1Follicle9622.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape9622.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape9622.inm";
connectAttr "curveShape228.ws" "pSphere1FollicleShape9622.sp";
connectAttr "pSphere1FollicleShape9628.ot" "pSphere1Follicle9628.t" -l on;
connectAttr "pSphere1FollicleShape9628.or" "pSphere1Follicle9628.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape9628.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape9628.inm";
connectAttr "curveShape229.ws" "pSphere1FollicleShape9628.sp";
connectAttr "pSphere1FollicleShape9634.ot" "pSphere1Follicle9634.t" -l on;
connectAttr "pSphere1FollicleShape9634.or" "pSphere1Follicle9634.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape9634.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape9634.inm";
connectAttr "curveShape230.ws" "pSphere1FollicleShape9634.sp";
connectAttr "pSphere1FollicleShape9640.ot" "pSphere1Follicle9640.t" -l on;
connectAttr "pSphere1FollicleShape9640.or" "pSphere1Follicle9640.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape9640.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape9640.inm";
connectAttr "curveShape231.ws" "pSphere1FollicleShape9640.sp";
connectAttr "pSphere1FollicleShape9646.ot" "pSphere1Follicle9646.t" -l on;
connectAttr "pSphere1FollicleShape9646.or" "pSphere1Follicle9646.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape9646.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape9646.inm";
connectAttr "curveShape232.ws" "pSphere1FollicleShape9646.sp";
connectAttr "pSphere1FollicleShape9653.ot" "pSphere1Follicle9653.t" -l on;
connectAttr "pSphere1FollicleShape9653.or" "pSphere1Follicle9653.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape9653.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape9653.inm";
connectAttr "curveShape233.ws" "pSphere1FollicleShape9653.sp";
connectAttr "pSphere1FollicleShape9659.ot" "pSphere1Follicle9659.t" -l on;
connectAttr "pSphere1FollicleShape9659.or" "pSphere1Follicle9659.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape9659.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape9659.inm";
connectAttr "curveShape234.ws" "pSphere1FollicleShape9659.sp";
connectAttr "pSphere1FollicleShape9665.ot" "pSphere1Follicle9665.t" -l on;
connectAttr "pSphere1FollicleShape9665.or" "pSphere1Follicle9665.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape9665.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape9665.inm";
connectAttr "curveShape235.ws" "pSphere1FollicleShape9665.sp";
connectAttr "pSphere1FollicleShape9671.ot" "pSphere1Follicle9671.t" -l on;
connectAttr "pSphere1FollicleShape9671.or" "pSphere1Follicle9671.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape9671.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape9671.inm";
connectAttr "curveShape236.ws" "pSphere1FollicleShape9671.sp";
connectAttr "pSphere1FollicleShape9677.ot" "pSphere1Follicle9677.t" -l on;
connectAttr "pSphere1FollicleShape9677.or" "pSphere1Follicle9677.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape9677.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape9677.inm";
connectAttr "curveShape237.ws" "pSphere1FollicleShape9677.sp";
connectAttr "pSphere1FollicleShape9684.ot" "pSphere1Follicle9684.t" -l on;
connectAttr "pSphere1FollicleShape9684.or" "pSphere1Follicle9684.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape9684.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape9684.inm";
connectAttr "curveShape238.ws" "pSphere1FollicleShape9684.sp";
connectAttr "pSphere1FollicleShape9690.ot" "pSphere1Follicle9690.t" -l on;
connectAttr "pSphere1FollicleShape9690.or" "pSphere1Follicle9690.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape9690.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape9690.inm";
connectAttr "curveShape239.ws" "pSphere1FollicleShape9690.sp";
connectAttr "pSphere1FollicleShape9696.ot" "pSphere1Follicle9696.t" -l on;
connectAttr "pSphere1FollicleShape9696.or" "pSphere1Follicle9696.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape9696.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape9696.inm";
connectAttr "curveShape240.ws" "pSphere1FollicleShape9696.sp";
connectAttr "hairSystemShape1.orh" "pfxHairShape1.rhs";
connectAttr ":time1.o" "hairSystemShape2.cti";
connectAttr "pSphere2FollicleShape303.oha" "hairSystemShape2.ih[0]";
connectAttr "pSphere2FollicleShape309.oha" "hairSystemShape2.ih[1]";
connectAttr "pSphere2FollicleShape315.oha" "hairSystemShape2.ih[2]";
connectAttr "pSphere2FollicleShape322.oha" "hairSystemShape2.ih[3]";
connectAttr "pSphere2FollicleShape328.oha" "hairSystemShape2.ih[4]";
connectAttr "pSphere2FollicleShape334.oha" "hairSystemShape2.ih[5]";
connectAttr "pSphere2FollicleShape340.oha" "hairSystemShape2.ih[6]";
connectAttr "pSphere2FollicleShape346.oha" "hairSystemShape2.ih[7]";
connectAttr "pSphere2FollicleShape353.oha" "hairSystemShape2.ih[8]";
connectAttr "pSphere2FollicleShape359.oha" "hairSystemShape2.ih[9]";
connectAttr "pSphere2FollicleShape365.oha" "hairSystemShape2.ih[10]";
connectAttr "pSphere2FollicleShape371.oha" "hairSystemShape2.ih[11]";
connectAttr "pSphere2FollicleShape377.oha" "hairSystemShape2.ih[12]";
connectAttr "pSphere2FollicleShape384.oha" "hairSystemShape2.ih[13]";
connectAttr "pSphere2FollicleShape390.oha" "hairSystemShape2.ih[14]";
connectAttr "pSphere2FollicleShape396.oha" "hairSystemShape2.ih[15]";
connectAttr "pSphere2FollicleShape909.oha" "hairSystemShape2.ih[16]";
connectAttr "pSphere2FollicleShape915.oha" "hairSystemShape2.ih[17]";
connectAttr "pSphere2FollicleShape922.oha" "hairSystemShape2.ih[18]";
connectAttr "pSphere2FollicleShape928.oha" "hairSystemShape2.ih[19]";
connectAttr "pSphere2FollicleShape934.oha" "hairSystemShape2.ih[20]";
connectAttr "pSphere2FollicleShape940.oha" "hairSystemShape2.ih[21]";
connectAttr "pSphere2FollicleShape946.oha" "hairSystemShape2.ih[22]";
connectAttr "pSphere2FollicleShape953.oha" "hairSystemShape2.ih[23]";
connectAttr "pSphere2FollicleShape959.oha" "hairSystemShape2.ih[24]";
connectAttr "pSphere2FollicleShape965.oha" "hairSystemShape2.ih[25]";
connectAttr "pSphere2FollicleShape971.oha" "hairSystemShape2.ih[26]";
connectAttr "pSphere2FollicleShape977.oha" "hairSystemShape2.ih[27]";
connectAttr "pSphere2FollicleShape984.oha" "hairSystemShape2.ih[28]";
connectAttr "pSphere2FollicleShape990.oha" "hairSystemShape2.ih[29]";
connectAttr "pSphere2FollicleShape1509.oha" "hairSystemShape2.ih[30]";
connectAttr "pSphere2FollicleShape1515.oha" "hairSystemShape2.ih[31]";
connectAttr "pSphere2FollicleShape1522.oha" "hairSystemShape2.ih[32]";
connectAttr "pSphere2FollicleShape1528.oha" "hairSystemShape2.ih[33]";
connectAttr "pSphere2FollicleShape1534.oha" "hairSystemShape2.ih[34]";
connectAttr "pSphere2FollicleShape1540.oha" "hairSystemShape2.ih[35]";
connectAttr "pSphere2FollicleShape1546.oha" "hairSystemShape2.ih[36]";
connectAttr "pSphere2FollicleShape1553.oha" "hairSystemShape2.ih[37]";
connectAttr "pSphere2FollicleShape1559.oha" "hairSystemShape2.ih[38]";
connectAttr "pSphere2FollicleShape1565.oha" "hairSystemShape2.ih[39]";
connectAttr "pSphere2FollicleShape1571.oha" "hairSystemShape2.ih[40]";
connectAttr "pSphere2FollicleShape1577.oha" "hairSystemShape2.ih[41]";
connectAttr "pSphere2FollicleShape1584.oha" "hairSystemShape2.ih[42]";
connectAttr "pSphere2FollicleShape1590.oha" "hairSystemShape2.ih[43]";
connectAttr "pSphere2FollicleShape2203.oha" "hairSystemShape2.ih[44]";
connectAttr "pSphere2FollicleShape2209.oha" "hairSystemShape2.ih[45]";
connectAttr "pSphere2FollicleShape2215.oha" "hairSystemShape2.ih[46]";
connectAttr "pSphere2FollicleShape2222.oha" "hairSystemShape2.ih[47]";
connectAttr "pSphere2FollicleShape2228.oha" "hairSystemShape2.ih[48]";
connectAttr "pSphere2FollicleShape2234.oha" "hairSystemShape2.ih[49]";
connectAttr "pSphere2FollicleShape2240.oha" "hairSystemShape2.ih[50]";
connectAttr "pSphere2FollicleShape2246.oha" "hairSystemShape2.ih[51]";
connectAttr "pSphere2FollicleShape2253.oha" "hairSystemShape2.ih[52]";
connectAttr "pSphere2FollicleShape2259.oha" "hairSystemShape2.ih[53]";
connectAttr "pSphere2FollicleShape2265.oha" "hairSystemShape2.ih[54]";
connectAttr "pSphere2FollicleShape2271.oha" "hairSystemShape2.ih[55]";
connectAttr "pSphere2FollicleShape2277.oha" "hairSystemShape2.ih[56]";
connectAttr "pSphere2FollicleShape2284.oha" "hairSystemShape2.ih[57]";
connectAttr "pSphere2FollicleShape2290.oha" "hairSystemShape2.ih[58]";
connectAttr "pSphere2FollicleShape2296.oha" "hairSystemShape2.ih[59]";
connectAttr "pSphere2FollicleShape2803.oha" "hairSystemShape2.ih[60]";
connectAttr "pSphere2FollicleShape2809.oha" "hairSystemShape2.ih[61]";
connectAttr "pSphere2FollicleShape2815.oha" "hairSystemShape2.ih[62]";
connectAttr "pSphere2FollicleShape2822.oha" "hairSystemShape2.ih[63]";
connectAttr "pSphere2FollicleShape2828.oha" "hairSystemShape2.ih[64]";
connectAttr "pSphere2FollicleShape2834.oha" "hairSystemShape2.ih[65]";
connectAttr "pSphere2FollicleShape2840.oha" "hairSystemShape2.ih[66]";
connectAttr "pSphere2FollicleShape2846.oha" "hairSystemShape2.ih[67]";
connectAttr "pSphere2FollicleShape2853.oha" "hairSystemShape2.ih[68]";
connectAttr "pSphere2FollicleShape2859.oha" "hairSystemShape2.ih[69]";
connectAttr "pSphere2FollicleShape2865.oha" "hairSystemShape2.ih[70]";
connectAttr "pSphere2FollicleShape2871.oha" "hairSystemShape2.ih[71]";
connectAttr "pSphere2FollicleShape2877.oha" "hairSystemShape2.ih[72]";
connectAttr "pSphere2FollicleShape2884.oha" "hairSystemShape2.ih[73]";
connectAttr "pSphere2FollicleShape2890.oha" "hairSystemShape2.ih[74]";
connectAttr "pSphere2FollicleShape2896.oha" "hairSystemShape2.ih[75]";
connectAttr "pSphere2FollicleShape3409.oha" "hairSystemShape2.ih[76]";
connectAttr "pSphere2FollicleShape3415.oha" "hairSystemShape2.ih[77]";
connectAttr "pSphere2FollicleShape3422.oha" "hairSystemShape2.ih[78]";
connectAttr "pSphere2FollicleShape3428.oha" "hairSystemShape2.ih[79]";
connectAttr "pSphere2FollicleShape3434.oha" "hairSystemShape2.ih[80]";
connectAttr "pSphere2FollicleShape3440.oha" "hairSystemShape2.ih[81]";
connectAttr "pSphere2FollicleShape3446.oha" "hairSystemShape2.ih[82]";
connectAttr "pSphere2FollicleShape3453.oha" "hairSystemShape2.ih[83]";
connectAttr "pSphere2FollicleShape3459.oha" "hairSystemShape2.ih[84]";
connectAttr "pSphere2FollicleShape3465.oha" "hairSystemShape2.ih[85]";
connectAttr "pSphere2FollicleShape3471.oha" "hairSystemShape2.ih[86]";
connectAttr "pSphere2FollicleShape3477.oha" "hairSystemShape2.ih[87]";
connectAttr "pSphere2FollicleShape3484.oha" "hairSystemShape2.ih[88]";
connectAttr "pSphere2FollicleShape3490.oha" "hairSystemShape2.ih[89]";
connectAttr "pSphere2FollicleShape4009.oha" "hairSystemShape2.ih[90]";
connectAttr "pSphere2FollicleShape4015.oha" "hairSystemShape2.ih[91]";
connectAttr "pSphere2FollicleShape4022.oha" "hairSystemShape2.ih[92]";
connectAttr "pSphere2FollicleShape4028.oha" "hairSystemShape2.ih[93]";
connectAttr "pSphere2FollicleShape4034.oha" "hairSystemShape2.ih[94]";
connectAttr "pSphere2FollicleShape4040.oha" "hairSystemShape2.ih[95]";
connectAttr "pSphere2FollicleShape4046.oha" "hairSystemShape2.ih[96]";
connectAttr "pSphere2FollicleShape4053.oha" "hairSystemShape2.ih[97]";
connectAttr "pSphere2FollicleShape4059.oha" "hairSystemShape2.ih[98]";
connectAttr "pSphere2FollicleShape4065.oha" "hairSystemShape2.ih[99]";
connectAttr "pSphere2FollicleShape4071.oha" "hairSystemShape2.ih[100]";
connectAttr "pSphere2FollicleShape4077.oha" "hairSystemShape2.ih[101]";
connectAttr "pSphere2FollicleShape4084.oha" "hairSystemShape2.ih[102]";
connectAttr "pSphere2FollicleShape4090.oha" "hairSystemShape2.ih[103]";
connectAttr "pSphere2FollicleShape4603.oha" "hairSystemShape2.ih[104]";
connectAttr "pSphere2FollicleShape4609.oha" "hairSystemShape2.ih[105]";
connectAttr "pSphere2FollicleShape4615.oha" "hairSystemShape2.ih[106]";
connectAttr "pSphere2FollicleShape4622.oha" "hairSystemShape2.ih[107]";
connectAttr "pSphere2FollicleShape4628.oha" "hairSystemShape2.ih[108]";
connectAttr "pSphere2FollicleShape4634.oha" "hairSystemShape2.ih[109]";
connectAttr "pSphere2FollicleShape4640.oha" "hairSystemShape2.ih[110]";
connectAttr "pSphere2FollicleShape4646.oha" "hairSystemShape2.ih[111]";
connectAttr "pSphere2FollicleShape4653.oha" "hairSystemShape2.ih[112]";
connectAttr "pSphere2FollicleShape4659.oha" "hairSystemShape2.ih[113]";
connectAttr "pSphere2FollicleShape4665.oha" "hairSystemShape2.ih[114]";
connectAttr "pSphere2FollicleShape4671.oha" "hairSystemShape2.ih[115]";
connectAttr "pSphere2FollicleShape4677.oha" "hairSystemShape2.ih[116]";
connectAttr "pSphere2FollicleShape4684.oha" "hairSystemShape2.ih[117]";
connectAttr "pSphere2FollicleShape4690.oha" "hairSystemShape2.ih[118]";
connectAttr "pSphere2FollicleShape4696.oha" "hairSystemShape2.ih[119]";
connectAttr "pSphere2FollicleShape5303.oha" "hairSystemShape2.ih[120]";
connectAttr "pSphere2FollicleShape5309.oha" "hairSystemShape2.ih[121]";
connectAttr "pSphere2FollicleShape5315.oha" "hairSystemShape2.ih[122]";
connectAttr "pSphere2FollicleShape5322.oha" "hairSystemShape2.ih[123]";
connectAttr "pSphere2FollicleShape5328.oha" "hairSystemShape2.ih[124]";
connectAttr "pSphere2FollicleShape5334.oha" "hairSystemShape2.ih[125]";
connectAttr "pSphere2FollicleShape5340.oha" "hairSystemShape2.ih[126]";
connectAttr "pSphere2FollicleShape5346.oha" "hairSystemShape2.ih[127]";
connectAttr "pSphere2FollicleShape5353.oha" "hairSystemShape2.ih[128]";
connectAttr "pSphere2FollicleShape5359.oha" "hairSystemShape2.ih[129]";
connectAttr "pSphere2FollicleShape5365.oha" "hairSystemShape2.ih[130]";
connectAttr "pSphere2FollicleShape5371.oha" "hairSystemShape2.ih[131]";
connectAttr "pSphere2FollicleShape5377.oha" "hairSystemShape2.ih[132]";
connectAttr "pSphere2FollicleShape5384.oha" "hairSystemShape2.ih[133]";
connectAttr "pSphere2FollicleShape5390.oha" "hairSystemShape2.ih[134]";
connectAttr "pSphere2FollicleShape5396.oha" "hairSystemShape2.ih[135]";
connectAttr "pSphere2FollicleShape5909.oha" "hairSystemShape2.ih[136]";
connectAttr "pSphere2FollicleShape5915.oha" "hairSystemShape2.ih[137]";
connectAttr "pSphere2FollicleShape5922.oha" "hairSystemShape2.ih[138]";
connectAttr "pSphere2FollicleShape5928.oha" "hairSystemShape2.ih[139]";
connectAttr "pSphere2FollicleShape5934.oha" "hairSystemShape2.ih[140]";
connectAttr "pSphere2FollicleShape5940.oha" "hairSystemShape2.ih[141]";
connectAttr "pSphere2FollicleShape5946.oha" "hairSystemShape2.ih[142]";
connectAttr "pSphere2FollicleShape5953.oha" "hairSystemShape2.ih[143]";
connectAttr "pSphere2FollicleShape5959.oha" "hairSystemShape2.ih[144]";
connectAttr "pSphere2FollicleShape5965.oha" "hairSystemShape2.ih[145]";
connectAttr "pSphere2FollicleShape5971.oha" "hairSystemShape2.ih[146]";
connectAttr "pSphere2FollicleShape5977.oha" "hairSystemShape2.ih[147]";
connectAttr "pSphere2FollicleShape5984.oha" "hairSystemShape2.ih[148]";
connectAttr "pSphere2FollicleShape5990.oha" "hairSystemShape2.ih[149]";
connectAttr "pSphere2FollicleShape6509.oha" "hairSystemShape2.ih[150]";
connectAttr "pSphere2FollicleShape6515.oha" "hairSystemShape2.ih[151]";
connectAttr "pSphere2FollicleShape6522.oha" "hairSystemShape2.ih[152]";
connectAttr "pSphere2FollicleShape6528.oha" "hairSystemShape2.ih[153]";
connectAttr "pSphere2FollicleShape6534.oha" "hairSystemShape2.ih[154]";
connectAttr "pSphere2FollicleShape6540.oha" "hairSystemShape2.ih[155]";
connectAttr "pSphere2FollicleShape6546.oha" "hairSystemShape2.ih[156]";
connectAttr "pSphere2FollicleShape6553.oha" "hairSystemShape2.ih[157]";
connectAttr "pSphere2FollicleShape6559.oha" "hairSystemShape2.ih[158]";
connectAttr "pSphere2FollicleShape6565.oha" "hairSystemShape2.ih[159]";
connectAttr "pSphere2FollicleShape6571.oha" "hairSystemShape2.ih[160]";
connectAttr "pSphere2FollicleShape6577.oha" "hairSystemShape2.ih[161]";
connectAttr "pSphere2FollicleShape6584.oha" "hairSystemShape2.ih[162]";
connectAttr "pSphere2FollicleShape6590.oha" "hairSystemShape2.ih[163]";
connectAttr "pSphere2FollicleShape7103.oha" "hairSystemShape2.ih[164]";
connectAttr "pSphere2FollicleShape7109.oha" "hairSystemShape2.ih[165]";
connectAttr "pSphere2FollicleShape7115.oha" "hairSystemShape2.ih[166]";
connectAttr "pSphere2FollicleShape7122.oha" "hairSystemShape2.ih[167]";
connectAttr "pSphere2FollicleShape7128.oha" "hairSystemShape2.ih[168]";
connectAttr "pSphere2FollicleShape7134.oha" "hairSystemShape2.ih[169]";
connectAttr "pSphere2FollicleShape7140.oha" "hairSystemShape2.ih[170]";
connectAttr "pSphere2FollicleShape7146.oha" "hairSystemShape2.ih[171]";
connectAttr "pSphere2FollicleShape7153.oha" "hairSystemShape2.ih[172]";
connectAttr "pSphere2FollicleShape7159.oha" "hairSystemShape2.ih[173]";
connectAttr "pSphere2FollicleShape7165.oha" "hairSystemShape2.ih[174]";
connectAttr "pSphere2FollicleShape7171.oha" "hairSystemShape2.ih[175]";
connectAttr "pSphere2FollicleShape7177.oha" "hairSystemShape2.ih[176]";
connectAttr "pSphere2FollicleShape7184.oha" "hairSystemShape2.ih[177]";
connectAttr "pSphere2FollicleShape7190.oha" "hairSystemShape2.ih[178]";
connectAttr "pSphere2FollicleShape7196.oha" "hairSystemShape2.ih[179]";
connectAttr "pSphere2FollicleShape7703.oha" "hairSystemShape2.ih[180]";
connectAttr "pSphere2FollicleShape7709.oha" "hairSystemShape2.ih[181]";
connectAttr "pSphere2FollicleShape7715.oha" "hairSystemShape2.ih[182]";
connectAttr "pSphere2FollicleShape7722.oha" "hairSystemShape2.ih[183]";
connectAttr "pSphere2FollicleShape7728.oha" "hairSystemShape2.ih[184]";
connectAttr "pSphere2FollicleShape7734.oha" "hairSystemShape2.ih[185]";
connectAttr "pSphere2FollicleShape7740.oha" "hairSystemShape2.ih[186]";
connectAttr "pSphere2FollicleShape7746.oha" "hairSystemShape2.ih[187]";
connectAttr "pSphere2FollicleShape7753.oha" "hairSystemShape2.ih[188]";
connectAttr "pSphere2FollicleShape7759.oha" "hairSystemShape2.ih[189]";
connectAttr "pSphere2FollicleShape7765.oha" "hairSystemShape2.ih[190]";
connectAttr "pSphere2FollicleShape7771.oha" "hairSystemShape2.ih[191]";
connectAttr "pSphere2FollicleShape7777.oha" "hairSystemShape2.ih[192]";
connectAttr "pSphere2FollicleShape7784.oha" "hairSystemShape2.ih[193]";
connectAttr "pSphere2FollicleShape7790.oha" "hairSystemShape2.ih[194]";
connectAttr "pSphere2FollicleShape7796.oha" "hairSystemShape2.ih[195]";
connectAttr "pSphere2FollicleShape8409.oha" "hairSystemShape2.ih[196]";
connectAttr "pSphere2FollicleShape8415.oha" "hairSystemShape2.ih[197]";
connectAttr "pSphere2FollicleShape8422.oha" "hairSystemShape2.ih[198]";
connectAttr "pSphere2FollicleShape8428.oha" "hairSystemShape2.ih[199]";
connectAttr "pSphere2FollicleShape8434.oha" "hairSystemShape2.ih[200]";
connectAttr "pSphere2FollicleShape8440.oha" "hairSystemShape2.ih[201]";
connectAttr "pSphere2FollicleShape8446.oha" "hairSystemShape2.ih[202]";
connectAttr "pSphere2FollicleShape8453.oha" "hairSystemShape2.ih[203]";
connectAttr "pSphere2FollicleShape8459.oha" "hairSystemShape2.ih[204]";
connectAttr "pSphere2FollicleShape8465.oha" "hairSystemShape2.ih[205]";
connectAttr "pSphere2FollicleShape8471.oha" "hairSystemShape2.ih[206]";
connectAttr "pSphere2FollicleShape8477.oha" "hairSystemShape2.ih[207]";
connectAttr "pSphere2FollicleShape8484.oha" "hairSystemShape2.ih[208]";
connectAttr "pSphere2FollicleShape8490.oha" "hairSystemShape2.ih[209]";
connectAttr "pSphere2FollicleShape9009.oha" "hairSystemShape2.ih[210]";
connectAttr "pSphere2FollicleShape9015.oha" "hairSystemShape2.ih[211]";
connectAttr "pSphere2FollicleShape9022.oha" "hairSystemShape2.ih[212]";
connectAttr "pSphere2FollicleShape9028.oha" "hairSystemShape2.ih[213]";
connectAttr "pSphere2FollicleShape9034.oha" "hairSystemShape2.ih[214]";
connectAttr "pSphere2FollicleShape9040.oha" "hairSystemShape2.ih[215]";
connectAttr "pSphere2FollicleShape9046.oha" "hairSystemShape2.ih[216]";
connectAttr "pSphere2FollicleShape9053.oha" "hairSystemShape2.ih[217]";
connectAttr "pSphere2FollicleShape9059.oha" "hairSystemShape2.ih[218]";
connectAttr "pSphere2FollicleShape9065.oha" "hairSystemShape2.ih[219]";
connectAttr "pSphere2FollicleShape9071.oha" "hairSystemShape2.ih[220]";
connectAttr "pSphere2FollicleShape9077.oha" "hairSystemShape2.ih[221]";
connectAttr "pSphere2FollicleShape9084.oha" "hairSystemShape2.ih[222]";
connectAttr "pSphere2FollicleShape9090.oha" "hairSystemShape2.ih[223]";
connectAttr "pSphere2FollicleShape9603.oha" "hairSystemShape2.ih[224]";
connectAttr "pSphere2FollicleShape9609.oha" "hairSystemShape2.ih[225]";
connectAttr "pSphere2FollicleShape9615.oha" "hairSystemShape2.ih[226]";
connectAttr "pSphere2FollicleShape9622.oha" "hairSystemShape2.ih[227]";
connectAttr "pSphere2FollicleShape9628.oha" "hairSystemShape2.ih[228]";
connectAttr "pSphere2FollicleShape9634.oha" "hairSystemShape2.ih[229]";
connectAttr "pSphere2FollicleShape9640.oha" "hairSystemShape2.ih[230]";
connectAttr "pSphere2FollicleShape9646.oha" "hairSystemShape2.ih[231]";
connectAttr "pSphere2FollicleShape9653.oha" "hairSystemShape2.ih[232]";
connectAttr "pSphere2FollicleShape9659.oha" "hairSystemShape2.ih[233]";
connectAttr "pSphere2FollicleShape9665.oha" "hairSystemShape2.ih[234]";
connectAttr "pSphere2FollicleShape9671.oha" "hairSystemShape2.ih[235]";
connectAttr "pSphere2FollicleShape9677.oha" "hairSystemShape2.ih[236]";
connectAttr "pSphere2FollicleShape9684.oha" "hairSystemShape2.ih[237]";
connectAttr "pSphere2FollicleShape9690.oha" "hairSystemShape2.ih[238]";
connectAttr "pSphere2FollicleShape9696.oha" "hairSystemShape2.ih[239]";
connectAttr "aiStandard3.out" "hairSystemShape2.ai_hair_shader";
connectAttr "pSphere2FollicleShape303.ot" "pSphere2Follicle303.t" -l on;
connectAttr "pSphere2FollicleShape303.or" "pSphere2Follicle303.r" -l on;
connectAttr "pSphereShape2.wm" "pSphere2FollicleShape303.iwm";
connectAttr "pSphereShape2.o" "pSphere2FollicleShape303.inm";
connectAttr "curveShape241.ws" "pSphere2FollicleShape303.sp";
connectAttr "pSphere2FollicleShape309.ot" "pSphere2Follicle309.t" -l on;
connectAttr "pSphere2FollicleShape309.or" "pSphere2Follicle309.r" -l on;
connectAttr "pSphereShape2.wm" "pSphere2FollicleShape309.iwm";
connectAttr "pSphereShape2.o" "pSphere2FollicleShape309.inm";
connectAttr "curveShape242.ws" "pSphere2FollicleShape309.sp";
connectAttr "pSphere2FollicleShape315.ot" "pSphere2Follicle315.t" -l on;
connectAttr "pSphere2FollicleShape315.or" "pSphere2Follicle315.r" -l on;
connectAttr "pSphereShape2.wm" "pSphere2FollicleShape315.iwm";
connectAttr "pSphereShape2.o" "pSphere2FollicleShape315.inm";
connectAttr "curveShape243.ws" "pSphere2FollicleShape315.sp";
connectAttr "pSphere2FollicleShape322.ot" "pSphere2Follicle322.t" -l on;
connectAttr "pSphere2FollicleShape322.or" "pSphere2Follicle322.r" -l on;
connectAttr "pSphereShape2.wm" "pSphere2FollicleShape322.iwm";
connectAttr "pSphereShape2.o" "pSphere2FollicleShape322.inm";
connectAttr "curveShape244.ws" "pSphere2FollicleShape322.sp";
connectAttr "pSphere2FollicleShape328.ot" "pSphere2Follicle328.t" -l on;
connectAttr "pSphere2FollicleShape328.or" "pSphere2Follicle328.r" -l on;
connectAttr "pSphereShape2.wm" "pSphere2FollicleShape328.iwm";
connectAttr "pSphereShape2.o" "pSphere2FollicleShape328.inm";
connectAttr "curveShape245.ws" "pSphere2FollicleShape328.sp";
connectAttr "pSphere2FollicleShape334.ot" "pSphere2Follicle334.t" -l on;
connectAttr "pSphere2FollicleShape334.or" "pSphere2Follicle334.r" -l on;
connectAttr "pSphereShape2.wm" "pSphere2FollicleShape334.iwm";
connectAttr "pSphereShape2.o" "pSphere2FollicleShape334.inm";
connectAttr "curveShape246.ws" "pSphere2FollicleShape334.sp";
connectAttr "pSphere2FollicleShape340.ot" "pSphere2Follicle340.t" -l on;
connectAttr "pSphere2FollicleShape340.or" "pSphere2Follicle340.r" -l on;
connectAttr "pSphereShape2.wm" "pSphere2FollicleShape340.iwm";
connectAttr "pSphereShape2.o" "pSphere2FollicleShape340.inm";
connectAttr "curveShape247.ws" "pSphere2FollicleShape340.sp";
connectAttr "pSphere2FollicleShape346.ot" "pSphere2Follicle346.t" -l on;
connectAttr "pSphere2FollicleShape346.or" "pSphere2Follicle346.r" -l on;
connectAttr "pSphereShape2.wm" "pSphere2FollicleShape346.iwm";
connectAttr "pSphereShape2.o" "pSphere2FollicleShape346.inm";
connectAttr "curveShape248.ws" "pSphere2FollicleShape346.sp";
connectAttr "pSphere2FollicleShape353.ot" "pSphere2Follicle353.t" -l on;
connectAttr "pSphere2FollicleShape353.or" "pSphere2Follicle353.r" -l on;
connectAttr "pSphereShape2.wm" "pSphere2FollicleShape353.iwm";
connectAttr "pSphereShape2.o" "pSphere2FollicleShape353.inm";
connectAttr "curveShape249.ws" "pSphere2FollicleShape353.sp";
connectAttr "pSphere2FollicleShape359.ot" "pSphere2Follicle359.t" -l on;
connectAttr "pSphere2FollicleShape359.or" "pSphere2Follicle359.r" -l on;
connectAttr "pSphereShape2.wm" "pSphere2FollicleShape359.iwm";
connectAttr "pSphereShape2.o" "pSphere2FollicleShape359.inm";
connectAttr "curveShape250.ws" "pSphere2FollicleShape359.sp";
connectAttr "pSphere2FollicleShape365.ot" "pSphere2Follicle365.t" -l on;
connectAttr "pSphere2FollicleShape365.or" "pSphere2Follicle365.r" -l on;
connectAttr "pSphereShape2.wm" "pSphere2FollicleShape365.iwm";
connectAttr "pSphereShape2.o" "pSphere2FollicleShape365.inm";
connectAttr "curveShape251.ws" "pSphere2FollicleShape365.sp";
connectAttr "pSphere2FollicleShape371.ot" "pSphere2Follicle371.t" -l on;
connectAttr "pSphere2FollicleShape371.or" "pSphere2Follicle371.r" -l on;
connectAttr "pSphereShape2.wm" "pSphere2FollicleShape371.iwm";
connectAttr "pSphereShape2.o" "pSphere2FollicleShape371.inm";
connectAttr "curveShape252.ws" "pSphere2FollicleShape371.sp";
connectAttr "pSphere2FollicleShape377.ot" "pSphere2Follicle377.t" -l on;
connectAttr "pSphere2FollicleShape377.or" "pSphere2Follicle377.r" -l on;
connectAttr "pSphereShape2.wm" "pSphere2FollicleShape377.iwm";
connectAttr "pSphereShape2.o" "pSphere2FollicleShape377.inm";
connectAttr "curveShape253.ws" "pSphere2FollicleShape377.sp";
connectAttr "pSphere2FollicleShape384.ot" "pSphere2Follicle384.t" -l on;
connectAttr "pSphere2FollicleShape384.or" "pSphere2Follicle384.r" -l on;
connectAttr "pSphereShape2.wm" "pSphere2FollicleShape384.iwm";
connectAttr "pSphereShape2.o" "pSphere2FollicleShape384.inm";
connectAttr "curveShape254.ws" "pSphere2FollicleShape384.sp";
connectAttr "pSphere2FollicleShape390.ot" "pSphere2Follicle390.t" -l on;
connectAttr "pSphere2FollicleShape390.or" "pSphere2Follicle390.r" -l on;
connectAttr "pSphereShape2.wm" "pSphere2FollicleShape390.iwm";
connectAttr "pSphereShape2.o" "pSphere2FollicleShape390.inm";
connectAttr "curveShape255.ws" "pSphere2FollicleShape390.sp";
connectAttr "pSphere2FollicleShape396.ot" "pSphere2Follicle396.t" -l on;
connectAttr "pSphere2FollicleShape396.or" "pSphere2Follicle396.r" -l on;
connectAttr "pSphereShape2.wm" "pSphere2FollicleShape396.iwm";
connectAttr "pSphereShape2.o" "pSphere2FollicleShape396.inm";
connectAttr "curveShape256.ws" "pSphere2FollicleShape396.sp";
connectAttr "pSphere2FollicleShape909.ot" "pSphere2Follicle909.t" -l on;
connectAttr "pSphere2FollicleShape909.or" "pSphere2Follicle909.r" -l on;
connectAttr "pSphereShape2.wm" "pSphere2FollicleShape909.iwm";
connectAttr "pSphereShape2.o" "pSphere2FollicleShape909.inm";
connectAttr "curveShape257.ws" "pSphere2FollicleShape909.sp";
connectAttr "pSphere2FollicleShape915.ot" "pSphere2Follicle915.t" -l on;
connectAttr "pSphere2FollicleShape915.or" "pSphere2Follicle915.r" -l on;
connectAttr "pSphereShape2.wm" "pSphere2FollicleShape915.iwm";
connectAttr "pSphereShape2.o" "pSphere2FollicleShape915.inm";
connectAttr "curveShape258.ws" "pSphere2FollicleShape915.sp";
connectAttr "pSphere2FollicleShape922.ot" "pSphere2Follicle922.t" -l on;
connectAttr "pSphere2FollicleShape922.or" "pSphere2Follicle922.r" -l on;
connectAttr "pSphereShape2.wm" "pSphere2FollicleShape922.iwm";
connectAttr "pSphereShape2.o" "pSphere2FollicleShape922.inm";
connectAttr "curveShape259.ws" "pSphere2FollicleShape922.sp";
connectAttr "pSphere2FollicleShape928.ot" "pSphere2Follicle928.t" -l on;
connectAttr "pSphere2FollicleShape928.or" "pSphere2Follicle928.r" -l on;
connectAttr "pSphereShape2.wm" "pSphere2FollicleShape928.iwm";
connectAttr "pSphereShape2.o" "pSphere2FollicleShape928.inm";
connectAttr "curveShape260.ws" "pSphere2FollicleShape928.sp";
connectAttr "pSphere2FollicleShape934.ot" "pSphere2Follicle934.t" -l on;
connectAttr "pSphere2FollicleShape934.or" "pSphere2Follicle934.r" -l on;
connectAttr "pSphereShape2.wm" "pSphere2FollicleShape934.iwm";
connectAttr "pSphereShape2.o" "pSphere2FollicleShape934.inm";
connectAttr "curveShape261.ws" "pSphere2FollicleShape934.sp";
connectAttr "pSphere2FollicleShape940.ot" "pSphere2Follicle940.t" -l on;
connectAttr "pSphere2FollicleShape940.or" "pSphere2Follicle940.r" -l on;
connectAttr "pSphereShape2.wm" "pSphere2FollicleShape940.iwm";
connectAttr "pSphereShape2.o" "pSphere2FollicleShape940.inm";
connectAttr "curveShape262.ws" "pSphere2FollicleShape940.sp";
connectAttr "pSphere2FollicleShape946.ot" "pSphere2Follicle946.t" -l on;
connectAttr "pSphere2FollicleShape946.or" "pSphere2Follicle946.r" -l on;
connectAttr "pSphereShape2.wm" "pSphere2FollicleShape946.iwm";
connectAttr "pSphereShape2.o" "pSphere2FollicleShape946.inm";
connectAttr "curveShape263.ws" "pSphere2FollicleShape946.sp";
connectAttr "pSphere2FollicleShape953.ot" "pSphere2Follicle953.t" -l on;
connectAttr "pSphere2FollicleShape953.or" "pSphere2Follicle953.r" -l on;
connectAttr "pSphereShape2.wm" "pSphere2FollicleShape953.iwm";
connectAttr "pSphereShape2.o" "pSphere2FollicleShape953.inm";
connectAttr "curveShape264.ws" "pSphere2FollicleShape953.sp";
connectAttr "pSphere2FollicleShape959.ot" "pSphere2Follicle959.t" -l on;
connectAttr "pSphere2FollicleShape959.or" "pSphere2Follicle959.r" -l on;
connectAttr "pSphereShape2.wm" "pSphere2FollicleShape959.iwm";
connectAttr "pSphereShape2.o" "pSphere2FollicleShape959.inm";
connectAttr "curveShape265.ws" "pSphere2FollicleShape959.sp";
connectAttr "pSphere2FollicleShape965.ot" "pSphere2Follicle965.t" -l on;
connectAttr "pSphere2FollicleShape965.or" "pSphere2Follicle965.r" -l on;
connectAttr "pSphereShape2.wm" "pSphere2FollicleShape965.iwm";
connectAttr "pSphereShape2.o" "pSphere2FollicleShape965.inm";
connectAttr "curveShape266.ws" "pSphere2FollicleShape965.sp";
connectAttr "pSphere2FollicleShape971.ot" "pSphere2Follicle971.t" -l on;
connectAttr "pSphere2FollicleShape971.or" "pSphere2Follicle971.r" -l on;
connectAttr "pSphereShape2.wm" "pSphere2FollicleShape971.iwm";
connectAttr "pSphereShape2.o" "pSphere2FollicleShape971.inm";
connectAttr "curveShape267.ws" "pSphere2FollicleShape971.sp";
connectAttr "pSphere2FollicleShape977.ot" "pSphere2Follicle977.t" -l on;
connectAttr "pSphere2FollicleShape977.or" "pSphere2Follicle977.r" -l on;
connectAttr "pSphereShape2.wm" "pSphere2FollicleShape977.iwm";
connectAttr "pSphereShape2.o" "pSphere2FollicleShape977.inm";
connectAttr "curveShape268.ws" "pSphere2FollicleShape977.sp";
connectAttr "pSphere2FollicleShape984.ot" "pSphere2Follicle984.t" -l on;
connectAttr "pSphere2FollicleShape984.or" "pSphere2Follicle984.r" -l on;
connectAttr "pSphereShape2.wm" "pSphere2FollicleShape984.iwm";
connectAttr "pSphereShape2.o" "pSphere2FollicleShape984.inm";
connectAttr "curveShape269.ws" "pSphere2FollicleShape984.sp";
connectAttr "pSphere2FollicleShape990.ot" "pSphere2Follicle990.t" -l on;
connectAttr "pSphere2FollicleShape990.or" "pSphere2Follicle990.r" -l on;
connectAttr "pSphereShape2.wm" "pSphere2FollicleShape990.iwm";
connectAttr "pSphereShape2.o" "pSphere2FollicleShape990.inm";
connectAttr "curveShape270.ws" "pSphere2FollicleShape990.sp";
connectAttr "pSphere2FollicleShape1509.ot" "pSphere2Follicle1509.t" -l on;
connectAttr "pSphere2FollicleShape1509.or" "pSphere2Follicle1509.r" -l on;
connectAttr "pSphereShape2.wm" "pSphere2FollicleShape1509.iwm";
connectAttr "pSphereShape2.o" "pSphere2FollicleShape1509.inm";
connectAttr "curveShape271.ws" "pSphere2FollicleShape1509.sp";
connectAttr "pSphere2FollicleShape1515.ot" "pSphere2Follicle1515.t" -l on;
connectAttr "pSphere2FollicleShape1515.or" "pSphere2Follicle1515.r" -l on;
connectAttr "pSphereShape2.wm" "pSphere2FollicleShape1515.iwm";
connectAttr "pSphereShape2.o" "pSphere2FollicleShape1515.inm";
connectAttr "curveShape272.ws" "pSphere2FollicleShape1515.sp";
connectAttr "pSphere2FollicleShape1522.ot" "pSphere2Follicle1522.t" -l on;
connectAttr "pSphere2FollicleShape1522.or" "pSphere2Follicle1522.r" -l on;
connectAttr "pSphereShape2.wm" "pSphere2FollicleShape1522.iwm";
connectAttr "pSphereShape2.o" "pSphere2FollicleShape1522.inm";
connectAttr "curveShape273.ws" "pSphere2FollicleShape1522.sp";
connectAttr "pSphere2FollicleShape1528.ot" "pSphere2Follicle1528.t" -l on;
connectAttr "pSphere2FollicleShape1528.or" "pSphere2Follicle1528.r" -l on;
connectAttr "pSphereShape2.wm" "pSphere2FollicleShape1528.iwm";
connectAttr "pSphereShape2.o" "pSphere2FollicleShape1528.inm";
connectAttr "curveShape274.ws" "pSphere2FollicleShape1528.sp";
connectAttr "pSphere2FollicleShape1534.ot" "pSphere2Follicle1534.t" -l on;
connectAttr "pSphere2FollicleShape1534.or" "pSphere2Follicle1534.r" -l on;
connectAttr "pSphereShape2.wm" "pSphere2FollicleShape1534.iwm";
connectAttr "pSphereShape2.o" "pSphere2FollicleShape1534.inm";
connectAttr "curveShape275.ws" "pSphere2FollicleShape1534.sp";
connectAttr "pSphere2FollicleShape1540.ot" "pSphere2Follicle1540.t" -l on;
connectAttr "pSphere2FollicleShape1540.or" "pSphere2Follicle1540.r" -l on;
connectAttr "pSphereShape2.wm" "pSphere2FollicleShape1540.iwm";
connectAttr "pSphereShape2.o" "pSphere2FollicleShape1540.inm";
connectAttr "curveShape276.ws" "pSphere2FollicleShape1540.sp";
connectAttr "pSphere2FollicleShape1546.ot" "pSphere2Follicle1546.t" -l on;
connectAttr "pSphere2FollicleShape1546.or" "pSphere2Follicle1546.r" -l on;
connectAttr "pSphereShape2.wm" "pSphere2FollicleShape1546.iwm";
connectAttr "pSphereShape2.o" "pSphere2FollicleShape1546.inm";
connectAttr "curveShape277.ws" "pSphere2FollicleShape1546.sp";
connectAttr "pSphere2FollicleShape1553.ot" "pSphere2Follicle1553.t" -l on;
connectAttr "pSphere2FollicleShape1553.or" "pSphere2Follicle1553.r" -l on;
connectAttr "pSphereShape2.wm" "pSphere2FollicleShape1553.iwm";
connectAttr "pSphereShape2.o" "pSphere2FollicleShape1553.inm";
connectAttr "curveShape278.ws" "pSphere2FollicleShape1553.sp";
connectAttr "pSphere2FollicleShape1559.ot" "pSphere2Follicle1559.t" -l on;
connectAttr "pSphere2FollicleShape1559.or" "pSphere2Follicle1559.r" -l on;
connectAttr "pSphereShape2.wm" "pSphere2FollicleShape1559.iwm";
connectAttr "pSphereShape2.o" "pSphere2FollicleShape1559.inm";
connectAttr "curveShape279.ws" "pSphere2FollicleShape1559.sp";
connectAttr "pSphere2FollicleShape1565.ot" "pSphere2Follicle1565.t" -l on;
connectAttr "pSphere2FollicleShape1565.or" "pSphere2Follicle1565.r" -l on;
connectAttr "pSphereShape2.wm" "pSphere2FollicleShape1565.iwm";
connectAttr "pSphereShape2.o" "pSphere2FollicleShape1565.inm";
connectAttr "curveShape280.ws" "pSphere2FollicleShape1565.sp";
connectAttr "pSphere2FollicleShape1571.ot" "pSphere2Follicle1571.t" -l on;
connectAttr "pSphere2FollicleShape1571.or" "pSphere2Follicle1571.r" -l on;
connectAttr "pSphereShape2.wm" "pSphere2FollicleShape1571.iwm";
connectAttr "pSphereShape2.o" "pSphere2FollicleShape1571.inm";
connectAttr "curveShape281.ws" "pSphere2FollicleShape1571.sp";
connectAttr "pSphere2FollicleShape1577.ot" "pSphere2Follicle1577.t" -l on;
connectAttr "pSphere2FollicleShape1577.or" "pSphere2Follicle1577.r" -l on;
connectAttr "pSphereShape2.wm" "pSphere2FollicleShape1577.iwm";
connectAttr "pSphereShape2.o" "pSphere2FollicleShape1577.inm";
connectAttr "curveShape282.ws" "pSphere2FollicleShape1577.sp";
connectAttr "pSphere2FollicleShape1584.ot" "pSphere2Follicle1584.t" -l on;
connectAttr "pSphere2FollicleShape1584.or" "pSphere2Follicle1584.r" -l on;
connectAttr "pSphereShape2.wm" "pSphere2FollicleShape1584.iwm";
connectAttr "pSphereShape2.o" "pSphere2FollicleShape1584.inm";
connectAttr "curveShape283.ws" "pSphere2FollicleShape1584.sp";
connectAttr "pSphere2FollicleShape1590.ot" "pSphere2Follicle1590.t" -l on;
connectAttr "pSphere2FollicleShape1590.or" "pSphere2Follicle1590.r" -l on;
connectAttr "pSphereShape2.wm" "pSphere2FollicleShape1590.iwm";
connectAttr "pSphereShape2.o" "pSphere2FollicleShape1590.inm";
connectAttr "curveShape284.ws" "pSphere2FollicleShape1590.sp";
connectAttr "pSphere2FollicleShape2203.ot" "pSphere2Follicle2203.t" -l on;
connectAttr "pSphere2FollicleShape2203.or" "pSphere2Follicle2203.r" -l on;
connectAttr "pSphereShape2.wm" "pSphere2FollicleShape2203.iwm";
connectAttr "pSphereShape2.o" "pSphere2FollicleShape2203.inm";
connectAttr "curveShape285.ws" "pSphere2FollicleShape2203.sp";
connectAttr "pSphere2FollicleShape2209.ot" "pSphere2Follicle2209.t" -l on;
connectAttr "pSphere2FollicleShape2209.or" "pSphere2Follicle2209.r" -l on;
connectAttr "pSphereShape2.wm" "pSphere2FollicleShape2209.iwm";
connectAttr "pSphereShape2.o" "pSphere2FollicleShape2209.inm";
connectAttr "curveShape286.ws" "pSphere2FollicleShape2209.sp";
connectAttr "pSphere2FollicleShape2215.ot" "pSphere2Follicle2215.t" -l on;
connectAttr "pSphere2FollicleShape2215.or" "pSphere2Follicle2215.r" -l on;
connectAttr "pSphereShape2.wm" "pSphere2FollicleShape2215.iwm";
connectAttr "pSphereShape2.o" "pSphere2FollicleShape2215.inm";
connectAttr "curveShape287.ws" "pSphere2FollicleShape2215.sp";
connectAttr "pSphere2FollicleShape2222.ot" "pSphere2Follicle2222.t" -l on;
connectAttr "pSphere2FollicleShape2222.or" "pSphere2Follicle2222.r" -l on;
connectAttr "pSphereShape2.wm" "pSphere2FollicleShape2222.iwm";
connectAttr "pSphereShape2.o" "pSphere2FollicleShape2222.inm";
connectAttr "curveShape288.ws" "pSphere2FollicleShape2222.sp";
connectAttr "pSphere2FollicleShape2228.ot" "pSphere2Follicle2228.t" -l on;
connectAttr "pSphere2FollicleShape2228.or" "pSphere2Follicle2228.r" -l on;
connectAttr "pSphereShape2.wm" "pSphere2FollicleShape2228.iwm";
connectAttr "pSphereShape2.o" "pSphere2FollicleShape2228.inm";
connectAttr "curveShape289.ws" "pSphere2FollicleShape2228.sp";
connectAttr "pSphere2FollicleShape2234.ot" "pSphere2Follicle2234.t" -l on;
connectAttr "pSphere2FollicleShape2234.or" "pSphere2Follicle2234.r" -l on;
connectAttr "pSphereShape2.wm" "pSphere2FollicleShape2234.iwm";
connectAttr "pSphereShape2.o" "pSphere2FollicleShape2234.inm";
connectAttr "curveShape290.ws" "pSphere2FollicleShape2234.sp";
connectAttr "pSphere2FollicleShape2240.ot" "pSphere2Follicle2240.t" -l on;
connectAttr "pSphere2FollicleShape2240.or" "pSphere2Follicle2240.r" -l on;
connectAttr "pSphereShape2.wm" "pSphere2FollicleShape2240.iwm";
connectAttr "pSphereShape2.o" "pSphere2FollicleShape2240.inm";
connectAttr "curveShape291.ws" "pSphere2FollicleShape2240.sp";
connectAttr "pSphere2FollicleShape2246.ot" "pSphere2Follicle2246.t" -l on;
connectAttr "pSphere2FollicleShape2246.or" "pSphere2Follicle2246.r" -l on;
connectAttr "pSphereShape2.wm" "pSphere2FollicleShape2246.iwm";
connectAttr "pSphereShape2.o" "pSphere2FollicleShape2246.inm";
connectAttr "curveShape292.ws" "pSphere2FollicleShape2246.sp";
connectAttr "pSphere2FollicleShape2253.ot" "pSphere2Follicle2253.t" -l on;
connectAttr "pSphere2FollicleShape2253.or" "pSphere2Follicle2253.r" -l on;
connectAttr "pSphereShape2.wm" "pSphere2FollicleShape2253.iwm";
connectAttr "pSphereShape2.o" "pSphere2FollicleShape2253.inm";
connectAttr "curveShape293.ws" "pSphere2FollicleShape2253.sp";
connectAttr "pSphere2FollicleShape2259.ot" "pSphere2Follicle2259.t" -l on;
connectAttr "pSphere2FollicleShape2259.or" "pSphere2Follicle2259.r" -l on;
connectAttr "pSphereShape2.wm" "pSphere2FollicleShape2259.iwm";
connectAttr "pSphereShape2.o" "pSphere2FollicleShape2259.inm";
connectAttr "curveShape294.ws" "pSphere2FollicleShape2259.sp";
connectAttr "pSphere2FollicleShape2265.ot" "pSphere2Follicle2265.t" -l on;
connectAttr "pSphere2FollicleShape2265.or" "pSphere2Follicle2265.r" -l on;
connectAttr "pSphereShape2.wm" "pSphere2FollicleShape2265.iwm";
connectAttr "pSphereShape2.o" "pSphere2FollicleShape2265.inm";
connectAttr "curveShape295.ws" "pSphere2FollicleShape2265.sp";
connectAttr "pSphere2FollicleShape2271.ot" "pSphere2Follicle2271.t" -l on;
connectAttr "pSphere2FollicleShape2271.or" "pSphere2Follicle2271.r" -l on;
connectAttr "pSphereShape2.wm" "pSphere2FollicleShape2271.iwm";
connectAttr "pSphereShape2.o" "pSphere2FollicleShape2271.inm";
connectAttr "curveShape296.ws" "pSphere2FollicleShape2271.sp";
connectAttr "pSphere2FollicleShape2277.ot" "pSphere2Follicle2277.t" -l on;
connectAttr "pSphere2FollicleShape2277.or" "pSphere2Follicle2277.r" -l on;
connectAttr "pSphereShape2.wm" "pSphere2FollicleShape2277.iwm";
connectAttr "pSphereShape2.o" "pSphere2FollicleShape2277.inm";
connectAttr "curveShape297.ws" "pSphere2FollicleShape2277.sp";
connectAttr "pSphere2FollicleShape2284.ot" "pSphere2Follicle2284.t" -l on;
connectAttr "pSphere2FollicleShape2284.or" "pSphere2Follicle2284.r" -l on;
connectAttr "pSphereShape2.wm" "pSphere2FollicleShape2284.iwm";
connectAttr "pSphereShape2.o" "pSphere2FollicleShape2284.inm";
connectAttr "curveShape298.ws" "pSphere2FollicleShape2284.sp";
connectAttr "pSphere2FollicleShape2290.ot" "pSphere2Follicle2290.t" -l on;
connectAttr "pSphere2FollicleShape2290.or" "pSphere2Follicle2290.r" -l on;
connectAttr "pSphereShape2.wm" "pSphere2FollicleShape2290.iwm";
connectAttr "pSphereShape2.o" "pSphere2FollicleShape2290.inm";
connectAttr "curveShape299.ws" "pSphere2FollicleShape2290.sp";
connectAttr "pSphere2FollicleShape2296.ot" "pSphere2Follicle2296.t" -l on;
connectAttr "pSphere2FollicleShape2296.or" "pSphere2Follicle2296.r" -l on;
connectAttr "pSphereShape2.wm" "pSphere2FollicleShape2296.iwm";
connectAttr "pSphereShape2.o" "pSphere2FollicleShape2296.inm";
connectAttr "curveShape300.ws" "pSphere2FollicleShape2296.sp";
connectAttr "pSphere2FollicleShape2803.ot" "pSphere2Follicle2803.t" -l on;
connectAttr "pSphere2FollicleShape2803.or" "pSphere2Follicle2803.r" -l on;
connectAttr "pSphereShape2.wm" "pSphere2FollicleShape2803.iwm";
connectAttr "pSphereShape2.o" "pSphere2FollicleShape2803.inm";
connectAttr "curveShape301.ws" "pSphere2FollicleShape2803.sp";
connectAttr "pSphere2FollicleShape2809.ot" "pSphere2Follicle2809.t" -l on;
connectAttr "pSphere2FollicleShape2809.or" "pSphere2Follicle2809.r" -l on;
connectAttr "pSphereShape2.wm" "pSphere2FollicleShape2809.iwm";
connectAttr "pSphereShape2.o" "pSphere2FollicleShape2809.inm";
connectAttr "curveShape302.ws" "pSphere2FollicleShape2809.sp";
connectAttr "pSphere2FollicleShape2815.ot" "pSphere2Follicle2815.t" -l on;
connectAttr "pSphere2FollicleShape2815.or" "pSphere2Follicle2815.r" -l on;
connectAttr "pSphereShape2.wm" "pSphere2FollicleShape2815.iwm";
connectAttr "pSphereShape2.o" "pSphere2FollicleShape2815.inm";
connectAttr "curveShape303.ws" "pSphere2FollicleShape2815.sp";
connectAttr "pSphere2FollicleShape2822.ot" "pSphere2Follicle2822.t" -l on;
connectAttr "pSphere2FollicleShape2822.or" "pSphere2Follicle2822.r" -l on;
connectAttr "pSphereShape2.wm" "pSphere2FollicleShape2822.iwm";
connectAttr "pSphereShape2.o" "pSphere2FollicleShape2822.inm";
connectAttr "curveShape304.ws" "pSphere2FollicleShape2822.sp";
connectAttr "pSphere2FollicleShape2828.ot" "pSphere2Follicle2828.t" -l on;
connectAttr "pSphere2FollicleShape2828.or" "pSphere2Follicle2828.r" -l on;
connectAttr "pSphereShape2.wm" "pSphere2FollicleShape2828.iwm";
connectAttr "pSphereShape2.o" "pSphere2FollicleShape2828.inm";
connectAttr "curveShape305.ws" "pSphere2FollicleShape2828.sp";
connectAttr "pSphere2FollicleShape2834.ot" "pSphere2Follicle2834.t" -l on;
connectAttr "pSphere2FollicleShape2834.or" "pSphere2Follicle2834.r" -l on;
connectAttr "pSphereShape2.wm" "pSphere2FollicleShape2834.iwm";
connectAttr "pSphereShape2.o" "pSphere2FollicleShape2834.inm";
connectAttr "curveShape306.ws" "pSphere2FollicleShape2834.sp";
connectAttr "pSphere2FollicleShape2840.ot" "pSphere2Follicle2840.t" -l on;
connectAttr "pSphere2FollicleShape2840.or" "pSphere2Follicle2840.r" -l on;
connectAttr "pSphereShape2.wm" "pSphere2FollicleShape2840.iwm";
connectAttr "pSphereShape2.o" "pSphere2FollicleShape2840.inm";
connectAttr "curveShape307.ws" "pSphere2FollicleShape2840.sp";
connectAttr "pSphere2FollicleShape2846.ot" "pSphere2Follicle2846.t" -l on;
connectAttr "pSphere2FollicleShape2846.or" "pSphere2Follicle2846.r" -l on;
connectAttr "pSphereShape2.wm" "pSphere2FollicleShape2846.iwm";
connectAttr "pSphereShape2.o" "pSphere2FollicleShape2846.inm";
connectAttr "curveShape308.ws" "pSphere2FollicleShape2846.sp";
connectAttr "pSphere2FollicleShape2853.ot" "pSphere2Follicle2853.t" -l on;
connectAttr "pSphere2FollicleShape2853.or" "pSphere2Follicle2853.r" -l on;
connectAttr "pSphereShape2.wm" "pSphere2FollicleShape2853.iwm";
connectAttr "pSphereShape2.o" "pSphere2FollicleShape2853.inm";
connectAttr "curveShape309.ws" "pSphere2FollicleShape2853.sp";
connectAttr "pSphere2FollicleShape2859.ot" "pSphere2Follicle2859.t" -l on;
connectAttr "pSphere2FollicleShape2859.or" "pSphere2Follicle2859.r" -l on;
connectAttr "pSphereShape2.wm" "pSphere2FollicleShape2859.iwm";
connectAttr "pSphereShape2.o" "pSphere2FollicleShape2859.inm";
connectAttr "curveShape310.ws" "pSphere2FollicleShape2859.sp";
connectAttr "pSphere2FollicleShape2865.ot" "pSphere2Follicle2865.t" -l on;
connectAttr "pSphere2FollicleShape2865.or" "pSphere2Follicle2865.r" -l on;
connectAttr "pSphereShape2.wm" "pSphere2FollicleShape2865.iwm";
connectAttr "pSphereShape2.o" "pSphere2FollicleShape2865.inm";
connectAttr "curveShape311.ws" "pSphere2FollicleShape2865.sp";
connectAttr "pSphere2FollicleShape2871.ot" "pSphere2Follicle2871.t" -l on;
connectAttr "pSphere2FollicleShape2871.or" "pSphere2Follicle2871.r" -l on;
connectAttr "pSphereShape2.wm" "pSphere2FollicleShape2871.iwm";
connectAttr "pSphereShape2.o" "pSphere2FollicleShape2871.inm";
connectAttr "curveShape312.ws" "pSphere2FollicleShape2871.sp";
connectAttr "pSphere2FollicleShape2877.ot" "pSphere2Follicle2877.t" -l on;
connectAttr "pSphere2FollicleShape2877.or" "pSphere2Follicle2877.r" -l on;
connectAttr "pSphereShape2.wm" "pSphere2FollicleShape2877.iwm";
connectAttr "pSphereShape2.o" "pSphere2FollicleShape2877.inm";
connectAttr "curveShape313.ws" "pSphere2FollicleShape2877.sp";
connectAttr "pSphere2FollicleShape2884.ot" "pSphere2Follicle2884.t" -l on;
connectAttr "pSphere2FollicleShape2884.or" "pSphere2Follicle2884.r" -l on;
connectAttr "pSphereShape2.wm" "pSphere2FollicleShape2884.iwm";
connectAttr "pSphereShape2.o" "pSphere2FollicleShape2884.inm";
connectAttr "curveShape314.ws" "pSphere2FollicleShape2884.sp";
connectAttr "pSphere2FollicleShape2890.ot" "pSphere2Follicle2890.t" -l on;
connectAttr "pSphere2FollicleShape2890.or" "pSphere2Follicle2890.r" -l on;
connectAttr "pSphereShape2.wm" "pSphere2FollicleShape2890.iwm";
connectAttr "pSphereShape2.o" "pSphere2FollicleShape2890.inm";
connectAttr "curveShape315.ws" "pSphere2FollicleShape2890.sp";
connectAttr "pSphere2FollicleShape2896.ot" "pSphere2Follicle2896.t" -l on;
connectAttr "pSphere2FollicleShape2896.or" "pSphere2Follicle2896.r" -l on;
connectAttr "pSphereShape2.wm" "pSphere2FollicleShape2896.iwm";
connectAttr "pSphereShape2.o" "pSphere2FollicleShape2896.inm";
connectAttr "curveShape316.ws" "pSphere2FollicleShape2896.sp";
connectAttr "pSphere2FollicleShape3409.ot" "pSphere2Follicle3409.t" -l on;
connectAttr "pSphere2FollicleShape3409.or" "pSphere2Follicle3409.r" -l on;
connectAttr "pSphereShape2.wm" "pSphere2FollicleShape3409.iwm";
connectAttr "pSphereShape2.o" "pSphere2FollicleShape3409.inm";
connectAttr "curveShape317.ws" "pSphere2FollicleShape3409.sp";
connectAttr "pSphere2FollicleShape3415.ot" "pSphere2Follicle3415.t" -l on;
connectAttr "pSphere2FollicleShape3415.or" "pSphere2Follicle3415.r" -l on;
connectAttr "pSphereShape2.wm" "pSphere2FollicleShape3415.iwm";
connectAttr "pSphereShape2.o" "pSphere2FollicleShape3415.inm";
connectAttr "curveShape318.ws" "pSphere2FollicleShape3415.sp";
connectAttr "pSphere2FollicleShape3422.ot" "pSphere2Follicle3422.t" -l on;
connectAttr "pSphere2FollicleShape3422.or" "pSphere2Follicle3422.r" -l on;
connectAttr "pSphereShape2.wm" "pSphere2FollicleShape3422.iwm";
connectAttr "pSphereShape2.o" "pSphere2FollicleShape3422.inm";
connectAttr "curveShape319.ws" "pSphere2FollicleShape3422.sp";
connectAttr "pSphere2FollicleShape3428.ot" "pSphere2Follicle3428.t" -l on;
connectAttr "pSphere2FollicleShape3428.or" "pSphere2Follicle3428.r" -l on;
connectAttr "pSphereShape2.wm" "pSphere2FollicleShape3428.iwm";
connectAttr "pSphereShape2.o" "pSphere2FollicleShape3428.inm";
connectAttr "curveShape320.ws" "pSphere2FollicleShape3428.sp";
connectAttr "pSphere2FollicleShape3434.ot" "pSphere2Follicle3434.t" -l on;
connectAttr "pSphere2FollicleShape3434.or" "pSphere2Follicle3434.r" -l on;
connectAttr "pSphereShape2.wm" "pSphere2FollicleShape3434.iwm";
connectAttr "pSphereShape2.o" "pSphere2FollicleShape3434.inm";
connectAttr "curveShape321.ws" "pSphere2FollicleShape3434.sp";
connectAttr "pSphere2FollicleShape3440.ot" "pSphere2Follicle3440.t" -l on;
connectAttr "pSphere2FollicleShape3440.or" "pSphere2Follicle3440.r" -l on;
connectAttr "pSphereShape2.wm" "pSphere2FollicleShape3440.iwm";
connectAttr "pSphereShape2.o" "pSphere2FollicleShape3440.inm";
connectAttr "curveShape322.ws" "pSphere2FollicleShape3440.sp";
connectAttr "pSphere2FollicleShape3446.ot" "pSphere2Follicle3446.t" -l on;
connectAttr "pSphere2FollicleShape3446.or" "pSphere2Follicle3446.r" -l on;
connectAttr "pSphereShape2.wm" "pSphere2FollicleShape3446.iwm";
connectAttr "pSphereShape2.o" "pSphere2FollicleShape3446.inm";
connectAttr "curveShape323.ws" "pSphere2FollicleShape3446.sp";
connectAttr "pSphere2FollicleShape3453.ot" "pSphere2Follicle3453.t" -l on;
connectAttr "pSphere2FollicleShape3453.or" "pSphere2Follicle3453.r" -l on;
connectAttr "pSphereShape2.wm" "pSphere2FollicleShape3453.iwm";
connectAttr "pSphereShape2.o" "pSphere2FollicleShape3453.inm";
connectAttr "curveShape324.ws" "pSphere2FollicleShape3453.sp";
connectAttr "pSphere2FollicleShape3459.ot" "pSphere2Follicle3459.t" -l on;
connectAttr "pSphere2FollicleShape3459.or" "pSphere2Follicle3459.r" -l on;
connectAttr "pSphereShape2.wm" "pSphere2FollicleShape3459.iwm";
connectAttr "pSphereShape2.o" "pSphere2FollicleShape3459.inm";
connectAttr "curveShape325.ws" "pSphere2FollicleShape3459.sp";
connectAttr "pSphere2FollicleShape3465.ot" "pSphere2Follicle3465.t" -l on;
connectAttr "pSphere2FollicleShape3465.or" "pSphere2Follicle3465.r" -l on;
connectAttr "pSphereShape2.wm" "pSphere2FollicleShape3465.iwm";
connectAttr "pSphereShape2.o" "pSphere2FollicleShape3465.inm";
connectAttr "curveShape326.ws" "pSphere2FollicleShape3465.sp";
connectAttr "pSphere2FollicleShape3471.ot" "pSphere2Follicle3471.t" -l on;
connectAttr "pSphere2FollicleShape3471.or" "pSphere2Follicle3471.r" -l on;
connectAttr "pSphereShape2.wm" "pSphere2FollicleShape3471.iwm";
connectAttr "pSphereShape2.o" "pSphere2FollicleShape3471.inm";
connectAttr "curveShape327.ws" "pSphere2FollicleShape3471.sp";
connectAttr "pSphere2FollicleShape3477.ot" "pSphere2Follicle3477.t" -l on;
connectAttr "pSphere2FollicleShape3477.or" "pSphere2Follicle3477.r" -l on;
connectAttr "pSphereShape2.wm" "pSphere2FollicleShape3477.iwm";
connectAttr "pSphereShape2.o" "pSphere2FollicleShape3477.inm";
connectAttr "curveShape328.ws" "pSphere2FollicleShape3477.sp";
connectAttr "pSphere2FollicleShape3484.ot" "pSphere2Follicle3484.t" -l on;
connectAttr "pSphere2FollicleShape3484.or" "pSphere2Follicle3484.r" -l on;
connectAttr "pSphereShape2.wm" "pSphere2FollicleShape3484.iwm";
connectAttr "pSphereShape2.o" "pSphere2FollicleShape3484.inm";
connectAttr "curveShape329.ws" "pSphere2FollicleShape3484.sp";
connectAttr "pSphere2FollicleShape3490.ot" "pSphere2Follicle3490.t" -l on;
connectAttr "pSphere2FollicleShape3490.or" "pSphere2Follicle3490.r" -l on;
connectAttr "pSphereShape2.wm" "pSphere2FollicleShape3490.iwm";
connectAttr "pSphereShape2.o" "pSphere2FollicleShape3490.inm";
connectAttr "curveShape330.ws" "pSphere2FollicleShape3490.sp";
connectAttr "pSphere2FollicleShape4009.ot" "pSphere2Follicle4009.t" -l on;
connectAttr "pSphere2FollicleShape4009.or" "pSphere2Follicle4009.r" -l on;
connectAttr "pSphereShape2.wm" "pSphere2FollicleShape4009.iwm";
connectAttr "pSphereShape2.o" "pSphere2FollicleShape4009.inm";
connectAttr "curveShape331.ws" "pSphere2FollicleShape4009.sp";
connectAttr "pSphere2FollicleShape4015.ot" "pSphere2Follicle4015.t" -l on;
connectAttr "pSphere2FollicleShape4015.or" "pSphere2Follicle4015.r" -l on;
connectAttr "pSphereShape2.wm" "pSphere2FollicleShape4015.iwm";
connectAttr "pSphereShape2.o" "pSphere2FollicleShape4015.inm";
connectAttr "curveShape332.ws" "pSphere2FollicleShape4015.sp";
connectAttr "pSphere2FollicleShape4022.ot" "pSphere2Follicle4022.t" -l on;
connectAttr "pSphere2FollicleShape4022.or" "pSphere2Follicle4022.r" -l on;
connectAttr "pSphereShape2.wm" "pSphere2FollicleShape4022.iwm";
connectAttr "pSphereShape2.o" "pSphere2FollicleShape4022.inm";
connectAttr "curveShape333.ws" "pSphere2FollicleShape4022.sp";
connectAttr "pSphere2FollicleShape4028.ot" "pSphere2Follicle4028.t" -l on;
connectAttr "pSphere2FollicleShape4028.or" "pSphere2Follicle4028.r" -l on;
connectAttr "pSphereShape2.wm" "pSphere2FollicleShape4028.iwm";
connectAttr "pSphereShape2.o" "pSphere2FollicleShape4028.inm";
connectAttr "curveShape334.ws" "pSphere2FollicleShape4028.sp";
connectAttr "pSphere2FollicleShape4034.ot" "pSphere2Follicle4034.t" -l on;
connectAttr "pSphere2FollicleShape4034.or" "pSphere2Follicle4034.r" -l on;
connectAttr "pSphereShape2.wm" "pSphere2FollicleShape4034.iwm";
connectAttr "pSphereShape2.o" "pSphere2FollicleShape4034.inm";
connectAttr "curveShape335.ws" "pSphere2FollicleShape4034.sp";
connectAttr "pSphere2FollicleShape4040.ot" "pSphere2Follicle4040.t" -l on;
connectAttr "pSphere2FollicleShape4040.or" "pSphere2Follicle4040.r" -l on;
connectAttr "pSphereShape2.wm" "pSphere2FollicleShape4040.iwm";
connectAttr "pSphereShape2.o" "pSphere2FollicleShape4040.inm";
connectAttr "curveShape336.ws" "pSphere2FollicleShape4040.sp";
connectAttr "pSphere2FollicleShape4046.ot" "pSphere2Follicle4046.t" -l on;
connectAttr "pSphere2FollicleShape4046.or" "pSphere2Follicle4046.r" -l on;
connectAttr "pSphereShape2.wm" "pSphere2FollicleShape4046.iwm";
connectAttr "pSphereShape2.o" "pSphere2FollicleShape4046.inm";
connectAttr "curveShape337.ws" "pSphere2FollicleShape4046.sp";
connectAttr "pSphere2FollicleShape4053.ot" "pSphere2Follicle4053.t" -l on;
connectAttr "pSphere2FollicleShape4053.or" "pSphere2Follicle4053.r" -l on;
connectAttr "pSphereShape2.wm" "pSphere2FollicleShape4053.iwm";
connectAttr "pSphereShape2.o" "pSphere2FollicleShape4053.inm";
connectAttr "curveShape338.ws" "pSphere2FollicleShape4053.sp";
connectAttr "pSphere2FollicleShape4059.ot" "pSphere2Follicle4059.t" -l on;
connectAttr "pSphere2FollicleShape4059.or" "pSphere2Follicle4059.r" -l on;
connectAttr "pSphereShape2.wm" "pSphere2FollicleShape4059.iwm";
connectAttr "pSphereShape2.o" "pSphere2FollicleShape4059.inm";
connectAttr "curveShape339.ws" "pSphere2FollicleShape4059.sp";
connectAttr "pSphere2FollicleShape4065.ot" "pSphere2Follicle4065.t" -l on;
connectAttr "pSphere2FollicleShape4065.or" "pSphere2Follicle4065.r" -l on;
connectAttr "pSphereShape2.wm" "pSphere2FollicleShape4065.iwm";
connectAttr "pSphereShape2.o" "pSphere2FollicleShape4065.inm";
connectAttr "curveShape340.ws" "pSphere2FollicleShape4065.sp";
connectAttr "pSphere2FollicleShape4071.ot" "pSphere2Follicle4071.t" -l on;
connectAttr "pSphere2FollicleShape4071.or" "pSphere2Follicle4071.r" -l on;
connectAttr "pSphereShape2.wm" "pSphere2FollicleShape4071.iwm";
connectAttr "pSphereShape2.o" "pSphere2FollicleShape4071.inm";
connectAttr "curveShape341.ws" "pSphere2FollicleShape4071.sp";
connectAttr "pSphere2FollicleShape4077.ot" "pSphere2Follicle4077.t" -l on;
connectAttr "pSphere2FollicleShape4077.or" "pSphere2Follicle4077.r" -l on;
connectAttr "pSphereShape2.wm" "pSphere2FollicleShape4077.iwm";
connectAttr "pSphereShape2.o" "pSphere2FollicleShape4077.inm";
connectAttr "curveShape342.ws" "pSphere2FollicleShape4077.sp";
connectAttr "pSphere2FollicleShape4084.ot" "pSphere2Follicle4084.t" -l on;
connectAttr "pSphere2FollicleShape4084.or" "pSphere2Follicle4084.r" -l on;
connectAttr "pSphereShape2.wm" "pSphere2FollicleShape4084.iwm";
connectAttr "pSphereShape2.o" "pSphere2FollicleShape4084.inm";
connectAttr "curveShape343.ws" "pSphere2FollicleShape4084.sp";
connectAttr "pSphere2FollicleShape4090.ot" "pSphere2Follicle4090.t" -l on;
connectAttr "pSphere2FollicleShape4090.or" "pSphere2Follicle4090.r" -l on;
connectAttr "pSphereShape2.wm" "pSphere2FollicleShape4090.iwm";
connectAttr "pSphereShape2.o" "pSphere2FollicleShape4090.inm";
connectAttr "curveShape344.ws" "pSphere2FollicleShape4090.sp";
connectAttr "pSphere2FollicleShape4603.ot" "pSphere2Follicle4603.t" -l on;
connectAttr "pSphere2FollicleShape4603.or" "pSphere2Follicle4603.r" -l on;
connectAttr "pSphereShape2.wm" "pSphere2FollicleShape4603.iwm";
connectAttr "pSphereShape2.o" "pSphere2FollicleShape4603.inm";
connectAttr "curveShape345.ws" "pSphere2FollicleShape4603.sp";
connectAttr "pSphere2FollicleShape4609.ot" "pSphere2Follicle4609.t" -l on;
connectAttr "pSphere2FollicleShape4609.or" "pSphere2Follicle4609.r" -l on;
connectAttr "pSphereShape2.wm" "pSphere2FollicleShape4609.iwm";
connectAttr "pSphereShape2.o" "pSphere2FollicleShape4609.inm";
connectAttr "curveShape346.ws" "pSphere2FollicleShape4609.sp";
connectAttr "pSphere2FollicleShape4615.ot" "pSphere2Follicle4615.t" -l on;
connectAttr "pSphere2FollicleShape4615.or" "pSphere2Follicle4615.r" -l on;
connectAttr "pSphereShape2.wm" "pSphere2FollicleShape4615.iwm";
connectAttr "pSphereShape2.o" "pSphere2FollicleShape4615.inm";
connectAttr "curveShape347.ws" "pSphere2FollicleShape4615.sp";
connectAttr "pSphere2FollicleShape4622.ot" "pSphere2Follicle4622.t" -l on;
connectAttr "pSphere2FollicleShape4622.or" "pSphere2Follicle4622.r" -l on;
connectAttr "pSphereShape2.wm" "pSphere2FollicleShape4622.iwm";
connectAttr "pSphereShape2.o" "pSphere2FollicleShape4622.inm";
connectAttr "curveShape348.ws" "pSphere2FollicleShape4622.sp";
connectAttr "pSphere2FollicleShape4628.ot" "pSphere2Follicle4628.t" -l on;
connectAttr "pSphere2FollicleShape4628.or" "pSphere2Follicle4628.r" -l on;
connectAttr "pSphereShape2.wm" "pSphere2FollicleShape4628.iwm";
connectAttr "pSphereShape2.o" "pSphere2FollicleShape4628.inm";
connectAttr "curveShape349.ws" "pSphere2FollicleShape4628.sp";
connectAttr "pSphere2FollicleShape4634.ot" "pSphere2Follicle4634.t" -l on;
connectAttr "pSphere2FollicleShape4634.or" "pSphere2Follicle4634.r" -l on;
connectAttr "pSphereShape2.wm" "pSphere2FollicleShape4634.iwm";
connectAttr "pSphereShape2.o" "pSphere2FollicleShape4634.inm";
connectAttr "curveShape350.ws" "pSphere2FollicleShape4634.sp";
connectAttr "pSphere2FollicleShape4640.ot" "pSphere2Follicle4640.t" -l on;
connectAttr "pSphere2FollicleShape4640.or" "pSphere2Follicle4640.r" -l on;
connectAttr "pSphereShape2.wm" "pSphere2FollicleShape4640.iwm";
connectAttr "pSphereShape2.o" "pSphere2FollicleShape4640.inm";
connectAttr "curveShape351.ws" "pSphere2FollicleShape4640.sp";
connectAttr "pSphere2FollicleShape4646.ot" "pSphere2Follicle4646.t" -l on;
connectAttr "pSphere2FollicleShape4646.or" "pSphere2Follicle4646.r" -l on;
connectAttr "pSphereShape2.wm" "pSphere2FollicleShape4646.iwm";
connectAttr "pSphereShape2.o" "pSphere2FollicleShape4646.inm";
connectAttr "curveShape352.ws" "pSphere2FollicleShape4646.sp";
connectAttr "pSphere2FollicleShape4653.ot" "pSphere2Follicle4653.t" -l on;
connectAttr "pSphere2FollicleShape4653.or" "pSphere2Follicle4653.r" -l on;
connectAttr "pSphereShape2.wm" "pSphere2FollicleShape4653.iwm";
connectAttr "pSphereShape2.o" "pSphere2FollicleShape4653.inm";
connectAttr "curveShape353.ws" "pSphere2FollicleShape4653.sp";
connectAttr "pSphere2FollicleShape4659.ot" "pSphere2Follicle4659.t" -l on;
connectAttr "pSphere2FollicleShape4659.or" "pSphere2Follicle4659.r" -l on;
connectAttr "pSphereShape2.wm" "pSphere2FollicleShape4659.iwm";
connectAttr "pSphereShape2.o" "pSphere2FollicleShape4659.inm";
connectAttr "curveShape354.ws" "pSphere2FollicleShape4659.sp";
connectAttr "pSphere2FollicleShape4665.ot" "pSphere2Follicle4665.t" -l on;
connectAttr "pSphere2FollicleShape4665.or" "pSphere2Follicle4665.r" -l on;
connectAttr "pSphereShape2.wm" "pSphere2FollicleShape4665.iwm";
connectAttr "pSphereShape2.o" "pSphere2FollicleShape4665.inm";
connectAttr "curveShape355.ws" "pSphere2FollicleShape4665.sp";
connectAttr "pSphere2FollicleShape4671.ot" "pSphere2Follicle4671.t" -l on;
connectAttr "pSphere2FollicleShape4671.or" "pSphere2Follicle4671.r" -l on;
connectAttr "pSphereShape2.wm" "pSphere2FollicleShape4671.iwm";
connectAttr "pSphereShape2.o" "pSphere2FollicleShape4671.inm";
connectAttr "curveShape356.ws" "pSphere2FollicleShape4671.sp";
connectAttr "pSphere2FollicleShape4677.ot" "pSphere2Follicle4677.t" -l on;
connectAttr "pSphere2FollicleShape4677.or" "pSphere2Follicle4677.r" -l on;
connectAttr "pSphereShape2.wm" "pSphere2FollicleShape4677.iwm";
connectAttr "pSphereShape2.o" "pSphere2FollicleShape4677.inm";
connectAttr "curveShape357.ws" "pSphere2FollicleShape4677.sp";
connectAttr "pSphere2FollicleShape4684.ot" "pSphere2Follicle4684.t" -l on;
connectAttr "pSphere2FollicleShape4684.or" "pSphere2Follicle4684.r" -l on;
connectAttr "pSphereShape2.wm" "pSphere2FollicleShape4684.iwm";
connectAttr "pSphereShape2.o" "pSphere2FollicleShape4684.inm";
connectAttr "curveShape358.ws" "pSphere2FollicleShape4684.sp";
connectAttr "pSphere2FollicleShape4690.ot" "pSphere2Follicle4690.t" -l on;
connectAttr "pSphere2FollicleShape4690.or" "pSphere2Follicle4690.r" -l on;
connectAttr "pSphereShape2.wm" "pSphere2FollicleShape4690.iwm";
connectAttr "pSphereShape2.o" "pSphere2FollicleShape4690.inm";
connectAttr "curveShape359.ws" "pSphere2FollicleShape4690.sp";
connectAttr "pSphere2FollicleShape4696.ot" "pSphere2Follicle4696.t" -l on;
connectAttr "pSphere2FollicleShape4696.or" "pSphere2Follicle4696.r" -l on;
connectAttr "pSphereShape2.wm" "pSphere2FollicleShape4696.iwm";
connectAttr "pSphereShape2.o" "pSphere2FollicleShape4696.inm";
connectAttr "curveShape360.ws" "pSphere2FollicleShape4696.sp";
connectAttr "pSphere2FollicleShape5303.ot" "pSphere2Follicle5303.t" -l on;
connectAttr "pSphere2FollicleShape5303.or" "pSphere2Follicle5303.r" -l on;
connectAttr "pSphereShape2.wm" "pSphere2FollicleShape5303.iwm";
connectAttr "pSphereShape2.o" "pSphere2FollicleShape5303.inm";
connectAttr "curveShape361.ws" "pSphere2FollicleShape5303.sp";
connectAttr "pSphere2FollicleShape5309.ot" "pSphere2Follicle5309.t" -l on;
connectAttr "pSphere2FollicleShape5309.or" "pSphere2Follicle5309.r" -l on;
connectAttr "pSphereShape2.wm" "pSphere2FollicleShape5309.iwm";
connectAttr "pSphereShape2.o" "pSphere2FollicleShape5309.inm";
connectAttr "curveShape362.ws" "pSphere2FollicleShape5309.sp";
connectAttr "pSphere2FollicleShape5315.ot" "pSphere2Follicle5315.t" -l on;
connectAttr "pSphere2FollicleShape5315.or" "pSphere2Follicle5315.r" -l on;
connectAttr "pSphereShape2.wm" "pSphere2FollicleShape5315.iwm";
connectAttr "pSphereShape2.o" "pSphere2FollicleShape5315.inm";
connectAttr "curveShape363.ws" "pSphere2FollicleShape5315.sp";
connectAttr "pSphere2FollicleShape5322.ot" "pSphere2Follicle5322.t" -l on;
connectAttr "pSphere2FollicleShape5322.or" "pSphere2Follicle5322.r" -l on;
connectAttr "pSphereShape2.wm" "pSphere2FollicleShape5322.iwm";
connectAttr "pSphereShape2.o" "pSphere2FollicleShape5322.inm";
connectAttr "curveShape364.ws" "pSphere2FollicleShape5322.sp";
connectAttr "pSphere2FollicleShape5328.ot" "pSphere2Follicle5328.t" -l on;
connectAttr "pSphere2FollicleShape5328.or" "pSphere2Follicle5328.r" -l on;
connectAttr "pSphereShape2.wm" "pSphere2FollicleShape5328.iwm";
connectAttr "pSphereShape2.o" "pSphere2FollicleShape5328.inm";
connectAttr "curveShape365.ws" "pSphere2FollicleShape5328.sp";
connectAttr "pSphere2FollicleShape5334.ot" "pSphere2Follicle5334.t" -l on;
connectAttr "pSphere2FollicleShape5334.or" "pSphere2Follicle5334.r" -l on;
connectAttr "pSphereShape2.wm" "pSphere2FollicleShape5334.iwm";
connectAttr "pSphereShape2.o" "pSphere2FollicleShape5334.inm";
connectAttr "curveShape366.ws" "pSphere2FollicleShape5334.sp";
connectAttr "pSphere2FollicleShape5340.ot" "pSphere2Follicle5340.t" -l on;
connectAttr "pSphere2FollicleShape5340.or" "pSphere2Follicle5340.r" -l on;
connectAttr "pSphereShape2.wm" "pSphere2FollicleShape5340.iwm";
connectAttr "pSphereShape2.o" "pSphere2FollicleShape5340.inm";
connectAttr "curveShape367.ws" "pSphere2FollicleShape5340.sp";
connectAttr "pSphere2FollicleShape5346.ot" "pSphere2Follicle5346.t" -l on;
connectAttr "pSphere2FollicleShape5346.or" "pSphere2Follicle5346.r" -l on;
connectAttr "pSphereShape2.wm" "pSphere2FollicleShape5346.iwm";
connectAttr "pSphereShape2.o" "pSphere2FollicleShape5346.inm";
connectAttr "curveShape368.ws" "pSphere2FollicleShape5346.sp";
connectAttr "pSphere2FollicleShape5353.ot" "pSphere2Follicle5353.t" -l on;
connectAttr "pSphere2FollicleShape5353.or" "pSphere2Follicle5353.r" -l on;
connectAttr "pSphereShape2.wm" "pSphere2FollicleShape5353.iwm";
connectAttr "pSphereShape2.o" "pSphere2FollicleShape5353.inm";
connectAttr "curveShape369.ws" "pSphere2FollicleShape5353.sp";
connectAttr "pSphere2FollicleShape5359.ot" "pSphere2Follicle5359.t" -l on;
connectAttr "pSphere2FollicleShape5359.or" "pSphere2Follicle5359.r" -l on;
connectAttr "pSphereShape2.wm" "pSphere2FollicleShape5359.iwm";
connectAttr "pSphereShape2.o" "pSphere2FollicleShape5359.inm";
connectAttr "curveShape370.ws" "pSphere2FollicleShape5359.sp";
connectAttr "pSphere2FollicleShape5365.ot" "pSphere2Follicle5365.t" -l on;
connectAttr "pSphere2FollicleShape5365.or" "pSphere2Follicle5365.r" -l on;
connectAttr "pSphereShape2.wm" "pSphere2FollicleShape5365.iwm";
connectAttr "pSphereShape2.o" "pSphere2FollicleShape5365.inm";
connectAttr "curveShape371.ws" "pSphere2FollicleShape5365.sp";
connectAttr "pSphere2FollicleShape5371.ot" "pSphere2Follicle5371.t" -l on;
connectAttr "pSphere2FollicleShape5371.or" "pSphere2Follicle5371.r" -l on;
connectAttr "pSphereShape2.wm" "pSphere2FollicleShape5371.iwm";
connectAttr "pSphereShape2.o" "pSphere2FollicleShape5371.inm";
connectAttr "curveShape372.ws" "pSphere2FollicleShape5371.sp";
connectAttr "pSphere2FollicleShape5377.ot" "pSphere2Follicle5377.t" -l on;
connectAttr "pSphere2FollicleShape5377.or" "pSphere2Follicle5377.r" -l on;
connectAttr "pSphereShape2.wm" "pSphere2FollicleShape5377.iwm";
connectAttr "pSphereShape2.o" "pSphere2FollicleShape5377.inm";
connectAttr "curveShape373.ws" "pSphere2FollicleShape5377.sp";
connectAttr "pSphere2FollicleShape5384.ot" "pSphere2Follicle5384.t" -l on;
connectAttr "pSphere2FollicleShape5384.or" "pSphere2Follicle5384.r" -l on;
connectAttr "pSphereShape2.wm" "pSphere2FollicleShape5384.iwm";
connectAttr "pSphereShape2.o" "pSphere2FollicleShape5384.inm";
connectAttr "curveShape374.ws" "pSphere2FollicleShape5384.sp";
connectAttr "pSphere2FollicleShape5390.ot" "pSphere2Follicle5390.t" -l on;
connectAttr "pSphere2FollicleShape5390.or" "pSphere2Follicle5390.r" -l on;
connectAttr "pSphereShape2.wm" "pSphere2FollicleShape5390.iwm";
connectAttr "pSphereShape2.o" "pSphere2FollicleShape5390.inm";
connectAttr "curveShape375.ws" "pSphere2FollicleShape5390.sp";
connectAttr "pSphere2FollicleShape5396.ot" "pSphere2Follicle5396.t" -l on;
connectAttr "pSphere2FollicleShape5396.or" "pSphere2Follicle5396.r" -l on;
connectAttr "pSphereShape2.wm" "pSphere2FollicleShape5396.iwm";
connectAttr "pSphereShape2.o" "pSphere2FollicleShape5396.inm";
connectAttr "curveShape376.ws" "pSphere2FollicleShape5396.sp";
connectAttr "pSphere2FollicleShape5909.ot" "pSphere2Follicle5909.t" -l on;
connectAttr "pSphere2FollicleShape5909.or" "pSphere2Follicle5909.r" -l on;
connectAttr "pSphereShape2.wm" "pSphere2FollicleShape5909.iwm";
connectAttr "pSphereShape2.o" "pSphere2FollicleShape5909.inm";
connectAttr "curveShape377.ws" "pSphere2FollicleShape5909.sp";
connectAttr "pSphere2FollicleShape5915.ot" "pSphere2Follicle5915.t" -l on;
connectAttr "pSphere2FollicleShape5915.or" "pSphere2Follicle5915.r" -l on;
connectAttr "pSphereShape2.wm" "pSphere2FollicleShape5915.iwm";
connectAttr "pSphereShape2.o" "pSphere2FollicleShape5915.inm";
connectAttr "curveShape378.ws" "pSphere2FollicleShape5915.sp";
connectAttr "pSphere2FollicleShape5922.ot" "pSphere2Follicle5922.t" -l on;
connectAttr "pSphere2FollicleShape5922.or" "pSphere2Follicle5922.r" -l on;
connectAttr "pSphereShape2.wm" "pSphere2FollicleShape5922.iwm";
connectAttr "pSphereShape2.o" "pSphere2FollicleShape5922.inm";
connectAttr "curveShape379.ws" "pSphere2FollicleShape5922.sp";
connectAttr "pSphere2FollicleShape5928.ot" "pSphere2Follicle5928.t" -l on;
connectAttr "pSphere2FollicleShape5928.or" "pSphere2Follicle5928.r" -l on;
connectAttr "pSphereShape2.wm" "pSphere2FollicleShape5928.iwm";
connectAttr "pSphereShape2.o" "pSphere2FollicleShape5928.inm";
connectAttr "curveShape380.ws" "pSphere2FollicleShape5928.sp";
connectAttr "pSphere2FollicleShape5934.ot" "pSphere2Follicle5934.t" -l on;
connectAttr "pSphere2FollicleShape5934.or" "pSphere2Follicle5934.r" -l on;
connectAttr "pSphereShape2.wm" "pSphere2FollicleShape5934.iwm";
connectAttr "pSphereShape2.o" "pSphere2FollicleShape5934.inm";
connectAttr "curveShape381.ws" "pSphere2FollicleShape5934.sp";
connectAttr "pSphere2FollicleShape5940.ot" "pSphere2Follicle5940.t" -l on;
connectAttr "pSphere2FollicleShape5940.or" "pSphere2Follicle5940.r" -l on;
connectAttr "pSphereShape2.wm" "pSphere2FollicleShape5940.iwm";
connectAttr "pSphereShape2.o" "pSphere2FollicleShape5940.inm";
connectAttr "curveShape382.ws" "pSphere2FollicleShape5940.sp";
connectAttr "pSphere2FollicleShape5946.ot" "pSphere2Follicle5946.t" -l on;
connectAttr "pSphere2FollicleShape5946.or" "pSphere2Follicle5946.r" -l on;
connectAttr "pSphereShape2.wm" "pSphere2FollicleShape5946.iwm";
connectAttr "pSphereShape2.o" "pSphere2FollicleShape5946.inm";
connectAttr "curveShape383.ws" "pSphere2FollicleShape5946.sp";
connectAttr "pSphere2FollicleShape5953.ot" "pSphere2Follicle5953.t" -l on;
connectAttr "pSphere2FollicleShape5953.or" "pSphere2Follicle5953.r" -l on;
connectAttr "pSphereShape2.wm" "pSphere2FollicleShape5953.iwm";
connectAttr "pSphereShape2.o" "pSphere2FollicleShape5953.inm";
connectAttr "curveShape384.ws" "pSphere2FollicleShape5953.sp";
connectAttr "pSphere2FollicleShape5959.ot" "pSphere2Follicle5959.t" -l on;
connectAttr "pSphere2FollicleShape5959.or" "pSphere2Follicle5959.r" -l on;
connectAttr "pSphereShape2.wm" "pSphere2FollicleShape5959.iwm";
connectAttr "pSphereShape2.o" "pSphere2FollicleShape5959.inm";
connectAttr "curveShape385.ws" "pSphere2FollicleShape5959.sp";
connectAttr "pSphere2FollicleShape5965.ot" "pSphere2Follicle5965.t" -l on;
connectAttr "pSphere2FollicleShape5965.or" "pSphere2Follicle5965.r" -l on;
connectAttr "pSphereShape2.wm" "pSphere2FollicleShape5965.iwm";
connectAttr "pSphereShape2.o" "pSphere2FollicleShape5965.inm";
connectAttr "curveShape386.ws" "pSphere2FollicleShape5965.sp";
connectAttr "pSphere2FollicleShape5971.ot" "pSphere2Follicle5971.t" -l on;
connectAttr "pSphere2FollicleShape5971.or" "pSphere2Follicle5971.r" -l on;
connectAttr "pSphereShape2.wm" "pSphere2FollicleShape5971.iwm";
connectAttr "pSphereShape2.o" "pSphere2FollicleShape5971.inm";
connectAttr "curveShape387.ws" "pSphere2FollicleShape5971.sp";
connectAttr "pSphere2FollicleShape5977.ot" "pSphere2Follicle5977.t" -l on;
connectAttr "pSphere2FollicleShape5977.or" "pSphere2Follicle5977.r" -l on;
connectAttr "pSphereShape2.wm" "pSphere2FollicleShape5977.iwm";
connectAttr "pSphereShape2.o" "pSphere2FollicleShape5977.inm";
connectAttr "curveShape388.ws" "pSphere2FollicleShape5977.sp";
connectAttr "pSphere2FollicleShape5984.ot" "pSphere2Follicle5984.t" -l on;
connectAttr "pSphere2FollicleShape5984.or" "pSphere2Follicle5984.r" -l on;
connectAttr "pSphereShape2.wm" "pSphere2FollicleShape5984.iwm";
connectAttr "pSphereShape2.o" "pSphere2FollicleShape5984.inm";
connectAttr "curveShape389.ws" "pSphere2FollicleShape5984.sp";
connectAttr "pSphere2FollicleShape5990.ot" "pSphere2Follicle5990.t" -l on;
connectAttr "pSphere2FollicleShape5990.or" "pSphere2Follicle5990.r" -l on;
connectAttr "pSphereShape2.wm" "pSphere2FollicleShape5990.iwm";
connectAttr "pSphereShape2.o" "pSphere2FollicleShape5990.inm";
connectAttr "curveShape390.ws" "pSphere2FollicleShape5990.sp";
connectAttr "pSphere2FollicleShape6509.ot" "pSphere2Follicle6509.t" -l on;
connectAttr "pSphere2FollicleShape6509.or" "pSphere2Follicle6509.r" -l on;
connectAttr "pSphereShape2.wm" "pSphere2FollicleShape6509.iwm";
connectAttr "pSphereShape2.o" "pSphere2FollicleShape6509.inm";
connectAttr "curveShape391.ws" "pSphere2FollicleShape6509.sp";
connectAttr "pSphere2FollicleShape6515.ot" "pSphere2Follicle6515.t" -l on;
connectAttr "pSphere2FollicleShape6515.or" "pSphere2Follicle6515.r" -l on;
connectAttr "pSphereShape2.wm" "pSphere2FollicleShape6515.iwm";
connectAttr "pSphereShape2.o" "pSphere2FollicleShape6515.inm";
connectAttr "curveShape392.ws" "pSphere2FollicleShape6515.sp";
connectAttr "pSphere2FollicleShape6522.ot" "pSphere2Follicle6522.t" -l on;
connectAttr "pSphere2FollicleShape6522.or" "pSphere2Follicle6522.r" -l on;
connectAttr "pSphereShape2.wm" "pSphere2FollicleShape6522.iwm";
connectAttr "pSphereShape2.o" "pSphere2FollicleShape6522.inm";
connectAttr "curveShape393.ws" "pSphere2FollicleShape6522.sp";
connectAttr "pSphere2FollicleShape6528.ot" "pSphere2Follicle6528.t" -l on;
connectAttr "pSphere2FollicleShape6528.or" "pSphere2Follicle6528.r" -l on;
connectAttr "pSphereShape2.wm" "pSphere2FollicleShape6528.iwm";
connectAttr "pSphereShape2.o" "pSphere2FollicleShape6528.inm";
connectAttr "curveShape394.ws" "pSphere2FollicleShape6528.sp";
connectAttr "pSphere2FollicleShape6534.ot" "pSphere2Follicle6534.t" -l on;
connectAttr "pSphere2FollicleShape6534.or" "pSphere2Follicle6534.r" -l on;
connectAttr "pSphereShape2.wm" "pSphere2FollicleShape6534.iwm";
connectAttr "pSphereShape2.o" "pSphere2FollicleShape6534.inm";
connectAttr "curveShape395.ws" "pSphere2FollicleShape6534.sp";
connectAttr "pSphere2FollicleShape6540.ot" "pSphere2Follicle6540.t" -l on;
connectAttr "pSphere2FollicleShape6540.or" "pSphere2Follicle6540.r" -l on;
connectAttr "pSphereShape2.wm" "pSphere2FollicleShape6540.iwm";
connectAttr "pSphereShape2.o" "pSphere2FollicleShape6540.inm";
connectAttr "curveShape396.ws" "pSphere2FollicleShape6540.sp";
connectAttr "pSphere2FollicleShape6546.ot" "pSphere2Follicle6546.t" -l on;
connectAttr "pSphere2FollicleShape6546.or" "pSphere2Follicle6546.r" -l on;
connectAttr "pSphereShape2.wm" "pSphere2FollicleShape6546.iwm";
connectAttr "pSphereShape2.o" "pSphere2FollicleShape6546.inm";
connectAttr "curveShape397.ws" "pSphere2FollicleShape6546.sp";
connectAttr "pSphere2FollicleShape6553.ot" "pSphere2Follicle6553.t" -l on;
connectAttr "pSphere2FollicleShape6553.or" "pSphere2Follicle6553.r" -l on;
connectAttr "pSphereShape2.wm" "pSphere2FollicleShape6553.iwm";
connectAttr "pSphereShape2.o" "pSphere2FollicleShape6553.inm";
connectAttr "curveShape398.ws" "pSphere2FollicleShape6553.sp";
connectAttr "pSphere2FollicleShape6559.ot" "pSphere2Follicle6559.t" -l on;
connectAttr "pSphere2FollicleShape6559.or" "pSphere2Follicle6559.r" -l on;
connectAttr "pSphereShape2.wm" "pSphere2FollicleShape6559.iwm";
connectAttr "pSphereShape2.o" "pSphere2FollicleShape6559.inm";
connectAttr "curveShape399.ws" "pSphere2FollicleShape6559.sp";
connectAttr "pSphere2FollicleShape6565.ot" "pSphere2Follicle6565.t" -l on;
connectAttr "pSphere2FollicleShape6565.or" "pSphere2Follicle6565.r" -l on;
connectAttr "pSphereShape2.wm" "pSphere2FollicleShape6565.iwm";
connectAttr "pSphereShape2.o" "pSphere2FollicleShape6565.inm";
connectAttr "curveShape400.ws" "pSphere2FollicleShape6565.sp";
connectAttr "pSphere2FollicleShape6571.ot" "pSphere2Follicle6571.t" -l on;
connectAttr "pSphere2FollicleShape6571.or" "pSphere2Follicle6571.r" -l on;
connectAttr "pSphereShape2.wm" "pSphere2FollicleShape6571.iwm";
connectAttr "pSphereShape2.o" "pSphere2FollicleShape6571.inm";
connectAttr "curveShape401.ws" "pSphere2FollicleShape6571.sp";
connectAttr "pSphere2FollicleShape6577.ot" "pSphere2Follicle6577.t" -l on;
connectAttr "pSphere2FollicleShape6577.or" "pSphere2Follicle6577.r" -l on;
connectAttr "pSphereShape2.wm" "pSphere2FollicleShape6577.iwm";
connectAttr "pSphereShape2.o" "pSphere2FollicleShape6577.inm";
connectAttr "curveShape402.ws" "pSphere2FollicleShape6577.sp";
connectAttr "pSphere2FollicleShape6584.ot" "pSphere2Follicle6584.t" -l on;
connectAttr "pSphere2FollicleShape6584.or" "pSphere2Follicle6584.r" -l on;
connectAttr "pSphereShape2.wm" "pSphere2FollicleShape6584.iwm";
connectAttr "pSphereShape2.o" "pSphere2FollicleShape6584.inm";
connectAttr "curveShape403.ws" "pSphere2FollicleShape6584.sp";
connectAttr "pSphere2FollicleShape6590.ot" "pSphere2Follicle6590.t" -l on;
connectAttr "pSphere2FollicleShape6590.or" "pSphere2Follicle6590.r" -l on;
connectAttr "pSphereShape2.wm" "pSphere2FollicleShape6590.iwm";
connectAttr "pSphereShape2.o" "pSphere2FollicleShape6590.inm";
connectAttr "curveShape404.ws" "pSphere2FollicleShape6590.sp";
connectAttr "pSphere2FollicleShape7103.ot" "pSphere2Follicle7103.t" -l on;
connectAttr "pSphere2FollicleShape7103.or" "pSphere2Follicle7103.r" -l on;
connectAttr "pSphereShape2.wm" "pSphere2FollicleShape7103.iwm";
connectAttr "pSphereShape2.o" "pSphere2FollicleShape7103.inm";
connectAttr "curveShape405.ws" "pSphere2FollicleShape7103.sp";
connectAttr "pSphere2FollicleShape7109.ot" "pSphere2Follicle7109.t" -l on;
connectAttr "pSphere2FollicleShape7109.or" "pSphere2Follicle7109.r" -l on;
connectAttr "pSphereShape2.wm" "pSphere2FollicleShape7109.iwm";
connectAttr "pSphereShape2.o" "pSphere2FollicleShape7109.inm";
connectAttr "curveShape406.ws" "pSphere2FollicleShape7109.sp";
connectAttr "pSphere2FollicleShape7115.ot" "pSphere2Follicle7115.t" -l on;
connectAttr "pSphere2FollicleShape7115.or" "pSphere2Follicle7115.r" -l on;
connectAttr "pSphereShape2.wm" "pSphere2FollicleShape7115.iwm";
connectAttr "pSphereShape2.o" "pSphere2FollicleShape7115.inm";
connectAttr "curveShape407.ws" "pSphere2FollicleShape7115.sp";
connectAttr "pSphere2FollicleShape7122.ot" "pSphere2Follicle7122.t" -l on;
connectAttr "pSphere2FollicleShape7122.or" "pSphere2Follicle7122.r" -l on;
connectAttr "pSphereShape2.wm" "pSphere2FollicleShape7122.iwm";
connectAttr "pSphereShape2.o" "pSphere2FollicleShape7122.inm";
connectAttr "curveShape408.ws" "pSphere2FollicleShape7122.sp";
connectAttr "pSphere2FollicleShape7128.ot" "pSphere2Follicle7128.t" -l on;
connectAttr "pSphere2FollicleShape7128.or" "pSphere2Follicle7128.r" -l on;
connectAttr "pSphereShape2.wm" "pSphere2FollicleShape7128.iwm";
connectAttr "pSphereShape2.o" "pSphere2FollicleShape7128.inm";
connectAttr "curveShape409.ws" "pSphere2FollicleShape7128.sp";
connectAttr "pSphere2FollicleShape7134.ot" "pSphere2Follicle7134.t" -l on;
connectAttr "pSphere2FollicleShape7134.or" "pSphere2Follicle7134.r" -l on;
connectAttr "pSphereShape2.wm" "pSphere2FollicleShape7134.iwm";
connectAttr "pSphereShape2.o" "pSphere2FollicleShape7134.inm";
connectAttr "curveShape410.ws" "pSphere2FollicleShape7134.sp";
connectAttr "pSphere2FollicleShape7140.ot" "pSphere2Follicle7140.t" -l on;
connectAttr "pSphere2FollicleShape7140.or" "pSphere2Follicle7140.r" -l on;
connectAttr "pSphereShape2.wm" "pSphere2FollicleShape7140.iwm";
connectAttr "pSphereShape2.o" "pSphere2FollicleShape7140.inm";
connectAttr "curveShape411.ws" "pSphere2FollicleShape7140.sp";
connectAttr "pSphere2FollicleShape7146.ot" "pSphere2Follicle7146.t" -l on;
connectAttr "pSphere2FollicleShape7146.or" "pSphere2Follicle7146.r" -l on;
connectAttr "pSphereShape2.wm" "pSphere2FollicleShape7146.iwm";
connectAttr "pSphereShape2.o" "pSphere2FollicleShape7146.inm";
connectAttr "curveShape412.ws" "pSphere2FollicleShape7146.sp";
connectAttr "pSphere2FollicleShape7153.ot" "pSphere2Follicle7153.t" -l on;
connectAttr "pSphere2FollicleShape7153.or" "pSphere2Follicle7153.r" -l on;
connectAttr "pSphereShape2.wm" "pSphere2FollicleShape7153.iwm";
connectAttr "pSphereShape2.o" "pSphere2FollicleShape7153.inm";
connectAttr "curveShape413.ws" "pSphere2FollicleShape7153.sp";
connectAttr "pSphere2FollicleShape7159.ot" "pSphere2Follicle7159.t" -l on;
connectAttr "pSphere2FollicleShape7159.or" "pSphere2Follicle7159.r" -l on;
connectAttr "pSphereShape2.wm" "pSphere2FollicleShape7159.iwm";
connectAttr "pSphereShape2.o" "pSphere2FollicleShape7159.inm";
connectAttr "curveShape414.ws" "pSphere2FollicleShape7159.sp";
connectAttr "pSphere2FollicleShape7165.ot" "pSphere2Follicle7165.t" -l on;
connectAttr "pSphere2FollicleShape7165.or" "pSphere2Follicle7165.r" -l on;
connectAttr "pSphereShape2.wm" "pSphere2FollicleShape7165.iwm";
connectAttr "pSphereShape2.o" "pSphere2FollicleShape7165.inm";
connectAttr "curveShape415.ws" "pSphere2FollicleShape7165.sp";
connectAttr "pSphere2FollicleShape7171.ot" "pSphere2Follicle7171.t" -l on;
connectAttr "pSphere2FollicleShape7171.or" "pSphere2Follicle7171.r" -l on;
connectAttr "pSphereShape2.wm" "pSphere2FollicleShape7171.iwm";
connectAttr "pSphereShape2.o" "pSphere2FollicleShape7171.inm";
connectAttr "curveShape416.ws" "pSphere2FollicleShape7171.sp";
connectAttr "pSphere2FollicleShape7177.ot" "pSphere2Follicle7177.t" -l on;
connectAttr "pSphere2FollicleShape7177.or" "pSphere2Follicle7177.r" -l on;
connectAttr "pSphereShape2.wm" "pSphere2FollicleShape7177.iwm";
connectAttr "pSphereShape2.o" "pSphere2FollicleShape7177.inm";
connectAttr "curveShape417.ws" "pSphere2FollicleShape7177.sp";
connectAttr "pSphere2FollicleShape7184.ot" "pSphere2Follicle7184.t" -l on;
connectAttr "pSphere2FollicleShape7184.or" "pSphere2Follicle7184.r" -l on;
connectAttr "pSphereShape2.wm" "pSphere2FollicleShape7184.iwm";
connectAttr "pSphereShape2.o" "pSphere2FollicleShape7184.inm";
connectAttr "curveShape418.ws" "pSphere2FollicleShape7184.sp";
connectAttr "pSphere2FollicleShape7190.ot" "pSphere2Follicle7190.t" -l on;
connectAttr "pSphere2FollicleShape7190.or" "pSphere2Follicle7190.r" -l on;
connectAttr "pSphereShape2.wm" "pSphere2FollicleShape7190.iwm";
connectAttr "pSphereShape2.o" "pSphere2FollicleShape7190.inm";
connectAttr "curveShape419.ws" "pSphere2FollicleShape7190.sp";
connectAttr "pSphere2FollicleShape7196.ot" "pSphere2Follicle7196.t" -l on;
connectAttr "pSphere2FollicleShape7196.or" "pSphere2Follicle7196.r" -l on;
connectAttr "pSphereShape2.wm" "pSphere2FollicleShape7196.iwm";
connectAttr "pSphereShape2.o" "pSphere2FollicleShape7196.inm";
connectAttr "curveShape420.ws" "pSphere2FollicleShape7196.sp";
connectAttr "pSphere2FollicleShape7703.ot" "pSphere2Follicle7703.t" -l on;
connectAttr "pSphere2FollicleShape7703.or" "pSphere2Follicle7703.r" -l on;
connectAttr "pSphereShape2.wm" "pSphere2FollicleShape7703.iwm";
connectAttr "pSphereShape2.o" "pSphere2FollicleShape7703.inm";
connectAttr "curveShape421.ws" "pSphere2FollicleShape7703.sp";
connectAttr "pSphere2FollicleShape7709.ot" "pSphere2Follicle7709.t" -l on;
connectAttr "pSphere2FollicleShape7709.or" "pSphere2Follicle7709.r" -l on;
connectAttr "pSphereShape2.wm" "pSphere2FollicleShape7709.iwm";
connectAttr "pSphereShape2.o" "pSphere2FollicleShape7709.inm";
connectAttr "curveShape422.ws" "pSphere2FollicleShape7709.sp";
connectAttr "pSphere2FollicleShape7715.ot" "pSphere2Follicle7715.t" -l on;
connectAttr "pSphere2FollicleShape7715.or" "pSphere2Follicle7715.r" -l on;
connectAttr "pSphereShape2.wm" "pSphere2FollicleShape7715.iwm";
connectAttr "pSphereShape2.o" "pSphere2FollicleShape7715.inm";
connectAttr "curveShape423.ws" "pSphere2FollicleShape7715.sp";
connectAttr "pSphere2FollicleShape7722.ot" "pSphere2Follicle7722.t" -l on;
connectAttr "pSphere2FollicleShape7722.or" "pSphere2Follicle7722.r" -l on;
connectAttr "pSphereShape2.wm" "pSphere2FollicleShape7722.iwm";
connectAttr "pSphereShape2.o" "pSphere2FollicleShape7722.inm";
connectAttr "curveShape424.ws" "pSphere2FollicleShape7722.sp";
connectAttr "pSphere2FollicleShape7728.ot" "pSphere2Follicle7728.t" -l on;
connectAttr "pSphere2FollicleShape7728.or" "pSphere2Follicle7728.r" -l on;
connectAttr "pSphereShape2.wm" "pSphere2FollicleShape7728.iwm";
connectAttr "pSphereShape2.o" "pSphere2FollicleShape7728.inm";
connectAttr "curveShape425.ws" "pSphere2FollicleShape7728.sp";
connectAttr "pSphere2FollicleShape7734.ot" "pSphere2Follicle7734.t" -l on;
connectAttr "pSphere2FollicleShape7734.or" "pSphere2Follicle7734.r" -l on;
connectAttr "pSphereShape2.wm" "pSphere2FollicleShape7734.iwm";
connectAttr "pSphereShape2.o" "pSphere2FollicleShape7734.inm";
connectAttr "curveShape426.ws" "pSphere2FollicleShape7734.sp";
connectAttr "pSphere2FollicleShape7740.ot" "pSphere2Follicle7740.t" -l on;
connectAttr "pSphere2FollicleShape7740.or" "pSphere2Follicle7740.r" -l on;
connectAttr "pSphereShape2.wm" "pSphere2FollicleShape7740.iwm";
connectAttr "pSphereShape2.o" "pSphere2FollicleShape7740.inm";
connectAttr "curveShape427.ws" "pSphere2FollicleShape7740.sp";
connectAttr "pSphere2FollicleShape7746.ot" "pSphere2Follicle7746.t" -l on;
connectAttr "pSphere2FollicleShape7746.or" "pSphere2Follicle7746.r" -l on;
connectAttr "pSphereShape2.wm" "pSphere2FollicleShape7746.iwm";
connectAttr "pSphereShape2.o" "pSphere2FollicleShape7746.inm";
connectAttr "curveShape428.ws" "pSphere2FollicleShape7746.sp";
connectAttr "pSphere2FollicleShape7753.ot" "pSphere2Follicle7753.t" -l on;
connectAttr "pSphere2FollicleShape7753.or" "pSphere2Follicle7753.r" -l on;
connectAttr "pSphereShape2.wm" "pSphere2FollicleShape7753.iwm";
connectAttr "pSphereShape2.o" "pSphere2FollicleShape7753.inm";
connectAttr "curveShape429.ws" "pSphere2FollicleShape7753.sp";
connectAttr "pSphere2FollicleShape7759.ot" "pSphere2Follicle7759.t" -l on;
connectAttr "pSphere2FollicleShape7759.or" "pSphere2Follicle7759.r" -l on;
connectAttr "pSphereShape2.wm" "pSphere2FollicleShape7759.iwm";
connectAttr "pSphereShape2.o" "pSphere2FollicleShape7759.inm";
connectAttr "curveShape430.ws" "pSphere2FollicleShape7759.sp";
connectAttr "pSphere2FollicleShape7765.ot" "pSphere2Follicle7765.t" -l on;
connectAttr "pSphere2FollicleShape7765.or" "pSphere2Follicle7765.r" -l on;
connectAttr "pSphereShape2.wm" "pSphere2FollicleShape7765.iwm";
connectAttr "pSphereShape2.o" "pSphere2FollicleShape7765.inm";
connectAttr "curveShape431.ws" "pSphere2FollicleShape7765.sp";
connectAttr "pSphere2FollicleShape7771.ot" "pSphere2Follicle7771.t" -l on;
connectAttr "pSphere2FollicleShape7771.or" "pSphere2Follicle7771.r" -l on;
connectAttr "pSphereShape2.wm" "pSphere2FollicleShape7771.iwm";
connectAttr "pSphereShape2.o" "pSphere2FollicleShape7771.inm";
connectAttr "curveShape432.ws" "pSphere2FollicleShape7771.sp";
connectAttr "pSphere2FollicleShape7777.ot" "pSphere2Follicle7777.t" -l on;
connectAttr "pSphere2FollicleShape7777.or" "pSphere2Follicle7777.r" -l on;
connectAttr "pSphereShape2.wm" "pSphere2FollicleShape7777.iwm";
connectAttr "pSphereShape2.o" "pSphere2FollicleShape7777.inm";
connectAttr "curveShape433.ws" "pSphere2FollicleShape7777.sp";
connectAttr "pSphere2FollicleShape7784.ot" "pSphere2Follicle7784.t" -l on;
connectAttr "pSphere2FollicleShape7784.or" "pSphere2Follicle7784.r" -l on;
connectAttr "pSphereShape2.wm" "pSphere2FollicleShape7784.iwm";
connectAttr "pSphereShape2.o" "pSphere2FollicleShape7784.inm";
connectAttr "curveShape434.ws" "pSphere2FollicleShape7784.sp";
connectAttr "pSphere2FollicleShape7790.ot" "pSphere2Follicle7790.t" -l on;
connectAttr "pSphere2FollicleShape7790.or" "pSphere2Follicle7790.r" -l on;
connectAttr "pSphereShape2.wm" "pSphere2FollicleShape7790.iwm";
connectAttr "pSphereShape2.o" "pSphere2FollicleShape7790.inm";
connectAttr "curveShape435.ws" "pSphere2FollicleShape7790.sp";
connectAttr "pSphere2FollicleShape7796.ot" "pSphere2Follicle7796.t" -l on;
connectAttr "pSphere2FollicleShape7796.or" "pSphere2Follicle7796.r" -l on;
connectAttr "pSphereShape2.wm" "pSphere2FollicleShape7796.iwm";
connectAttr "pSphereShape2.o" "pSphere2FollicleShape7796.inm";
connectAttr "curveShape436.ws" "pSphere2FollicleShape7796.sp";
connectAttr "pSphere2FollicleShape8409.ot" "pSphere2Follicle8409.t" -l on;
connectAttr "pSphere2FollicleShape8409.or" "pSphere2Follicle8409.r" -l on;
connectAttr "pSphereShape2.wm" "pSphere2FollicleShape8409.iwm";
connectAttr "pSphereShape2.o" "pSphere2FollicleShape8409.inm";
connectAttr "curveShape437.ws" "pSphere2FollicleShape8409.sp";
connectAttr "pSphere2FollicleShape8415.ot" "pSphere2Follicle8415.t" -l on;
connectAttr "pSphere2FollicleShape8415.or" "pSphere2Follicle8415.r" -l on;
connectAttr "pSphereShape2.wm" "pSphere2FollicleShape8415.iwm";
connectAttr "pSphereShape2.o" "pSphere2FollicleShape8415.inm";
connectAttr "curveShape438.ws" "pSphere2FollicleShape8415.sp";
connectAttr "pSphere2FollicleShape8422.ot" "pSphere2Follicle8422.t" -l on;
connectAttr "pSphere2FollicleShape8422.or" "pSphere2Follicle8422.r" -l on;
connectAttr "pSphereShape2.wm" "pSphere2FollicleShape8422.iwm";
connectAttr "pSphereShape2.o" "pSphere2FollicleShape8422.inm";
connectAttr "curveShape439.ws" "pSphere2FollicleShape8422.sp";
connectAttr "pSphere2FollicleShape8428.ot" "pSphere2Follicle8428.t" -l on;
connectAttr "pSphere2FollicleShape8428.or" "pSphere2Follicle8428.r" -l on;
connectAttr "pSphereShape2.wm" "pSphere2FollicleShape8428.iwm";
connectAttr "pSphereShape2.o" "pSphere2FollicleShape8428.inm";
connectAttr "curveShape440.ws" "pSphere2FollicleShape8428.sp";
connectAttr "pSphere2FollicleShape8434.ot" "pSphere2Follicle8434.t" -l on;
connectAttr "pSphere2FollicleShape8434.or" "pSphere2Follicle8434.r" -l on;
connectAttr "pSphereShape2.wm" "pSphere2FollicleShape8434.iwm";
connectAttr "pSphereShape2.o" "pSphere2FollicleShape8434.inm";
connectAttr "curveShape441.ws" "pSphere2FollicleShape8434.sp";
connectAttr "pSphere2FollicleShape8440.ot" "pSphere2Follicle8440.t" -l on;
connectAttr "pSphere2FollicleShape8440.or" "pSphere2Follicle8440.r" -l on;
connectAttr "pSphereShape2.wm" "pSphere2FollicleShape8440.iwm";
connectAttr "pSphereShape2.o" "pSphere2FollicleShape8440.inm";
connectAttr "curveShape442.ws" "pSphere2FollicleShape8440.sp";
connectAttr "pSphere2FollicleShape8446.ot" "pSphere2Follicle8446.t" -l on;
connectAttr "pSphere2FollicleShape8446.or" "pSphere2Follicle8446.r" -l on;
connectAttr "pSphereShape2.wm" "pSphere2FollicleShape8446.iwm";
connectAttr "pSphereShape2.o" "pSphere2FollicleShape8446.inm";
connectAttr "curveShape443.ws" "pSphere2FollicleShape8446.sp";
connectAttr "pSphere2FollicleShape8453.ot" "pSphere2Follicle8453.t" -l on;
connectAttr "pSphere2FollicleShape8453.or" "pSphere2Follicle8453.r" -l on;
connectAttr "pSphereShape2.wm" "pSphere2FollicleShape8453.iwm";
connectAttr "pSphereShape2.o" "pSphere2FollicleShape8453.inm";
connectAttr "curveShape444.ws" "pSphere2FollicleShape8453.sp";
connectAttr "pSphere2FollicleShape8459.ot" "pSphere2Follicle8459.t" -l on;
connectAttr "pSphere2FollicleShape8459.or" "pSphere2Follicle8459.r" -l on;
connectAttr "pSphereShape2.wm" "pSphere2FollicleShape8459.iwm";
connectAttr "pSphereShape2.o" "pSphere2FollicleShape8459.inm";
connectAttr "curveShape445.ws" "pSphere2FollicleShape8459.sp";
connectAttr "pSphere2FollicleShape8465.ot" "pSphere2Follicle8465.t" -l on;
connectAttr "pSphere2FollicleShape8465.or" "pSphere2Follicle8465.r" -l on;
connectAttr "pSphereShape2.wm" "pSphere2FollicleShape8465.iwm";
connectAttr "pSphereShape2.o" "pSphere2FollicleShape8465.inm";
connectAttr "curveShape446.ws" "pSphere2FollicleShape8465.sp";
connectAttr "pSphere2FollicleShape8471.ot" "pSphere2Follicle8471.t" -l on;
connectAttr "pSphere2FollicleShape8471.or" "pSphere2Follicle8471.r" -l on;
connectAttr "pSphereShape2.wm" "pSphere2FollicleShape8471.iwm";
connectAttr "pSphereShape2.o" "pSphere2FollicleShape8471.inm";
connectAttr "curveShape447.ws" "pSphere2FollicleShape8471.sp";
connectAttr "pSphere2FollicleShape8477.ot" "pSphere2Follicle8477.t" -l on;
connectAttr "pSphere2FollicleShape8477.or" "pSphere2Follicle8477.r" -l on;
connectAttr "pSphereShape2.wm" "pSphere2FollicleShape8477.iwm";
connectAttr "pSphereShape2.o" "pSphere2FollicleShape8477.inm";
connectAttr "curveShape448.ws" "pSphere2FollicleShape8477.sp";
connectAttr "pSphere2FollicleShape8484.ot" "pSphere2Follicle8484.t" -l on;
connectAttr "pSphere2FollicleShape8484.or" "pSphere2Follicle8484.r" -l on;
connectAttr "pSphereShape2.wm" "pSphere2FollicleShape8484.iwm";
connectAttr "pSphereShape2.o" "pSphere2FollicleShape8484.inm";
connectAttr "curveShape449.ws" "pSphere2FollicleShape8484.sp";
connectAttr "pSphere2FollicleShape8490.ot" "pSphere2Follicle8490.t" -l on;
connectAttr "pSphere2FollicleShape8490.or" "pSphere2Follicle8490.r" -l on;
connectAttr "pSphereShape2.wm" "pSphere2FollicleShape8490.iwm";
connectAttr "pSphereShape2.o" "pSphere2FollicleShape8490.inm";
connectAttr "curveShape450.ws" "pSphere2FollicleShape8490.sp";
connectAttr "pSphere2FollicleShape9009.ot" "pSphere2Follicle9009.t" -l on;
connectAttr "pSphere2FollicleShape9009.or" "pSphere2Follicle9009.r" -l on;
connectAttr "pSphereShape2.wm" "pSphere2FollicleShape9009.iwm";
connectAttr "pSphereShape2.o" "pSphere2FollicleShape9009.inm";
connectAttr "curveShape451.ws" "pSphere2FollicleShape9009.sp";
connectAttr "pSphere2FollicleShape9015.ot" "pSphere2Follicle9015.t" -l on;
connectAttr "pSphere2FollicleShape9015.or" "pSphere2Follicle9015.r" -l on;
connectAttr "pSphereShape2.wm" "pSphere2FollicleShape9015.iwm";
connectAttr "pSphereShape2.o" "pSphere2FollicleShape9015.inm";
connectAttr "curveShape452.ws" "pSphere2FollicleShape9015.sp";
connectAttr "pSphere2FollicleShape9022.ot" "pSphere2Follicle9022.t" -l on;
connectAttr "pSphere2FollicleShape9022.or" "pSphere2Follicle9022.r" -l on;
connectAttr "pSphereShape2.wm" "pSphere2FollicleShape9022.iwm";
connectAttr "pSphereShape2.o" "pSphere2FollicleShape9022.inm";
connectAttr "curveShape453.ws" "pSphere2FollicleShape9022.sp";
connectAttr "pSphere2FollicleShape9028.ot" "pSphere2Follicle9028.t" -l on;
connectAttr "pSphere2FollicleShape9028.or" "pSphere2Follicle9028.r" -l on;
connectAttr "pSphereShape2.wm" "pSphere2FollicleShape9028.iwm";
connectAttr "pSphereShape2.o" "pSphere2FollicleShape9028.inm";
connectAttr "curveShape454.ws" "pSphere2FollicleShape9028.sp";
connectAttr "pSphere2FollicleShape9034.ot" "pSphere2Follicle9034.t" -l on;
connectAttr "pSphere2FollicleShape9034.or" "pSphere2Follicle9034.r" -l on;
connectAttr "pSphereShape2.wm" "pSphere2FollicleShape9034.iwm";
connectAttr "pSphereShape2.o" "pSphere2FollicleShape9034.inm";
connectAttr "curveShape455.ws" "pSphere2FollicleShape9034.sp";
connectAttr "pSphere2FollicleShape9040.ot" "pSphere2Follicle9040.t" -l on;
connectAttr "pSphere2FollicleShape9040.or" "pSphere2Follicle9040.r" -l on;
connectAttr "pSphereShape2.wm" "pSphere2FollicleShape9040.iwm";
connectAttr "pSphereShape2.o" "pSphere2FollicleShape9040.inm";
connectAttr "curveShape456.ws" "pSphere2FollicleShape9040.sp";
connectAttr "pSphere2FollicleShape9046.ot" "pSphere2Follicle9046.t" -l on;
connectAttr "pSphere2FollicleShape9046.or" "pSphere2Follicle9046.r" -l on;
connectAttr "pSphereShape2.wm" "pSphere2FollicleShape9046.iwm";
connectAttr "pSphereShape2.o" "pSphere2FollicleShape9046.inm";
connectAttr "curveShape457.ws" "pSphere2FollicleShape9046.sp";
connectAttr "pSphere2FollicleShape9053.ot" "pSphere2Follicle9053.t" -l on;
connectAttr "pSphere2FollicleShape9053.or" "pSphere2Follicle9053.r" -l on;
connectAttr "pSphereShape2.wm" "pSphere2FollicleShape9053.iwm";
connectAttr "pSphereShape2.o" "pSphere2FollicleShape9053.inm";
connectAttr "curveShape458.ws" "pSphere2FollicleShape9053.sp";
connectAttr "pSphere2FollicleShape9059.ot" "pSphere2Follicle9059.t" -l on;
connectAttr "pSphere2FollicleShape9059.or" "pSphere2Follicle9059.r" -l on;
connectAttr "pSphereShape2.wm" "pSphere2FollicleShape9059.iwm";
connectAttr "pSphereShape2.o" "pSphere2FollicleShape9059.inm";
connectAttr "curveShape459.ws" "pSphere2FollicleShape9059.sp";
connectAttr "pSphere2FollicleShape9065.ot" "pSphere2Follicle9065.t" -l on;
connectAttr "pSphere2FollicleShape9065.or" "pSphere2Follicle9065.r" -l on;
connectAttr "pSphereShape2.wm" "pSphere2FollicleShape9065.iwm";
connectAttr "pSphereShape2.o" "pSphere2FollicleShape9065.inm";
connectAttr "curveShape460.ws" "pSphere2FollicleShape9065.sp";
connectAttr "pSphere2FollicleShape9071.ot" "pSphere2Follicle9071.t" -l on;
connectAttr "pSphere2FollicleShape9071.or" "pSphere2Follicle9071.r" -l on;
connectAttr "pSphereShape2.wm" "pSphere2FollicleShape9071.iwm";
connectAttr "pSphereShape2.o" "pSphere2FollicleShape9071.inm";
connectAttr "curveShape461.ws" "pSphere2FollicleShape9071.sp";
connectAttr "pSphere2FollicleShape9077.ot" "pSphere2Follicle9077.t" -l on;
connectAttr "pSphere2FollicleShape9077.or" "pSphere2Follicle9077.r" -l on;
connectAttr "pSphereShape2.wm" "pSphere2FollicleShape9077.iwm";
connectAttr "pSphereShape2.o" "pSphere2FollicleShape9077.inm";
connectAttr "curveShape462.ws" "pSphere2FollicleShape9077.sp";
connectAttr "pSphere2FollicleShape9084.ot" "pSphere2Follicle9084.t" -l on;
connectAttr "pSphere2FollicleShape9084.or" "pSphere2Follicle9084.r" -l on;
connectAttr "pSphereShape2.wm" "pSphere2FollicleShape9084.iwm";
connectAttr "pSphereShape2.o" "pSphere2FollicleShape9084.inm";
connectAttr "curveShape463.ws" "pSphere2FollicleShape9084.sp";
connectAttr "pSphere2FollicleShape9090.ot" "pSphere2Follicle9090.t" -l on;
connectAttr "pSphere2FollicleShape9090.or" "pSphere2Follicle9090.r" -l on;
connectAttr "pSphereShape2.wm" "pSphere2FollicleShape9090.iwm";
connectAttr "pSphereShape2.o" "pSphere2FollicleShape9090.inm";
connectAttr "curveShape464.ws" "pSphere2FollicleShape9090.sp";
connectAttr "pSphere2FollicleShape9603.ot" "pSphere2Follicle9603.t" -l on;
connectAttr "pSphere2FollicleShape9603.or" "pSphere2Follicle9603.r" -l on;
connectAttr "pSphereShape2.wm" "pSphere2FollicleShape9603.iwm";
connectAttr "pSphereShape2.o" "pSphere2FollicleShape9603.inm";
connectAttr "curveShape465.ws" "pSphere2FollicleShape9603.sp";
connectAttr "pSphere2FollicleShape9609.ot" "pSphere2Follicle9609.t" -l on;
connectAttr "pSphere2FollicleShape9609.or" "pSphere2Follicle9609.r" -l on;
connectAttr "pSphereShape2.wm" "pSphere2FollicleShape9609.iwm";
connectAttr "pSphereShape2.o" "pSphere2FollicleShape9609.inm";
connectAttr "curveShape466.ws" "pSphere2FollicleShape9609.sp";
connectAttr "pSphere2FollicleShape9615.ot" "pSphere2Follicle9615.t" -l on;
connectAttr "pSphere2FollicleShape9615.or" "pSphere2Follicle9615.r" -l on;
connectAttr "pSphereShape2.wm" "pSphere2FollicleShape9615.iwm";
connectAttr "pSphereShape2.o" "pSphere2FollicleShape9615.inm";
connectAttr "curveShape467.ws" "pSphere2FollicleShape9615.sp";
connectAttr "pSphere2FollicleShape9622.ot" "pSphere2Follicle9622.t" -l on;
connectAttr "pSphere2FollicleShape9622.or" "pSphere2Follicle9622.r" -l on;
connectAttr "pSphereShape2.wm" "pSphere2FollicleShape9622.iwm";
connectAttr "pSphereShape2.o" "pSphere2FollicleShape9622.inm";
connectAttr "curveShape468.ws" "pSphere2FollicleShape9622.sp";
connectAttr "pSphere2FollicleShape9628.ot" "pSphere2Follicle9628.t" -l on;
connectAttr "pSphere2FollicleShape9628.or" "pSphere2Follicle9628.r" -l on;
connectAttr "pSphereShape2.wm" "pSphere2FollicleShape9628.iwm";
connectAttr "pSphereShape2.o" "pSphere2FollicleShape9628.inm";
connectAttr "curveShape469.ws" "pSphere2FollicleShape9628.sp";
connectAttr "pSphere2FollicleShape9634.ot" "pSphere2Follicle9634.t" -l on;
connectAttr "pSphere2FollicleShape9634.or" "pSphere2Follicle9634.r" -l on;
connectAttr "pSphereShape2.wm" "pSphere2FollicleShape9634.iwm";
connectAttr "pSphereShape2.o" "pSphere2FollicleShape9634.inm";
connectAttr "curveShape470.ws" "pSphere2FollicleShape9634.sp";
connectAttr "pSphere2FollicleShape9640.ot" "pSphere2Follicle9640.t" -l on;
connectAttr "pSphere2FollicleShape9640.or" "pSphere2Follicle9640.r" -l on;
connectAttr "pSphereShape2.wm" "pSphere2FollicleShape9640.iwm";
connectAttr "pSphereShape2.o" "pSphere2FollicleShape9640.inm";
connectAttr "curveShape471.ws" "pSphere2FollicleShape9640.sp";
connectAttr "pSphere2FollicleShape9646.ot" "pSphere2Follicle9646.t" -l on;
connectAttr "pSphere2FollicleShape9646.or" "pSphere2Follicle9646.r" -l on;
connectAttr "pSphereShape2.wm" "pSphere2FollicleShape9646.iwm";
connectAttr "pSphereShape2.o" "pSphere2FollicleShape9646.inm";
connectAttr "curveShape472.ws" "pSphere2FollicleShape9646.sp";
connectAttr "pSphere2FollicleShape9653.ot" "pSphere2Follicle9653.t" -l on;
connectAttr "pSphere2FollicleShape9653.or" "pSphere2Follicle9653.r" -l on;
connectAttr "pSphereShape2.wm" "pSphere2FollicleShape9653.iwm";
connectAttr "pSphereShape2.o" "pSphere2FollicleShape9653.inm";
connectAttr "curveShape473.ws" "pSphere2FollicleShape9653.sp";
connectAttr "pSphere2FollicleShape9659.ot" "pSphere2Follicle9659.t" -l on;
connectAttr "pSphere2FollicleShape9659.or" "pSphere2Follicle9659.r" -l on;
connectAttr "pSphereShape2.wm" "pSphere2FollicleShape9659.iwm";
connectAttr "pSphereShape2.o" "pSphere2FollicleShape9659.inm";
connectAttr "curveShape474.ws" "pSphere2FollicleShape9659.sp";
connectAttr "pSphere2FollicleShape9665.ot" "pSphere2Follicle9665.t" -l on;
connectAttr "pSphere2FollicleShape9665.or" "pSphere2Follicle9665.r" -l on;
connectAttr "pSphereShape2.wm" "pSphere2FollicleShape9665.iwm";
connectAttr "pSphereShape2.o" "pSphere2FollicleShape9665.inm";
connectAttr "curveShape475.ws" "pSphere2FollicleShape9665.sp";
connectAttr "pSphere2FollicleShape9671.ot" "pSphere2Follicle9671.t" -l on;
connectAttr "pSphere2FollicleShape9671.or" "pSphere2Follicle9671.r" -l on;
connectAttr "pSphereShape2.wm" "pSphere2FollicleShape9671.iwm";
connectAttr "pSphereShape2.o" "pSphere2FollicleShape9671.inm";
connectAttr "curveShape476.ws" "pSphere2FollicleShape9671.sp";
connectAttr "pSphere2FollicleShape9677.ot" "pSphere2Follicle9677.t" -l on;
connectAttr "pSphere2FollicleShape9677.or" "pSphere2Follicle9677.r" -l on;
connectAttr "pSphereShape2.wm" "pSphere2FollicleShape9677.iwm";
connectAttr "pSphereShape2.o" "pSphere2FollicleShape9677.inm";
connectAttr "curveShape477.ws" "pSphere2FollicleShape9677.sp";
connectAttr "pSphere2FollicleShape9684.ot" "pSphere2Follicle9684.t" -l on;
connectAttr "pSphere2FollicleShape9684.or" "pSphere2Follicle9684.r" -l on;
connectAttr "pSphereShape2.wm" "pSphere2FollicleShape9684.iwm";
connectAttr "pSphereShape2.o" "pSphere2FollicleShape9684.inm";
connectAttr "curveShape478.ws" "pSphere2FollicleShape9684.sp";
connectAttr "pSphere2FollicleShape9690.ot" "pSphere2Follicle9690.t" -l on;
connectAttr "pSphere2FollicleShape9690.or" "pSphere2Follicle9690.r" -l on;
connectAttr "pSphereShape2.wm" "pSphere2FollicleShape9690.iwm";
connectAttr "pSphereShape2.o" "pSphere2FollicleShape9690.inm";
connectAttr "curveShape479.ws" "pSphere2FollicleShape9690.sp";
connectAttr "pSphere2FollicleShape9696.ot" "pSphere2Follicle9696.t" -l on;
connectAttr "pSphere2FollicleShape9696.or" "pSphere2Follicle9696.r" -l on;
connectAttr "pSphereShape2.wm" "pSphere2FollicleShape9696.iwm";
connectAttr "pSphereShape2.o" "pSphere2FollicleShape9696.inm";
connectAttr "curveShape480.ws" "pSphere2FollicleShape9696.sp";
connectAttr "hairSystemShape2.orh" "pfxHairShape2.rhs";
relationship "link" ":lightLinker1" ":initialShadingGroup.message" ":defaultLightSet.message";
relationship "link" ":lightLinker1" ":initialParticleSE.message" ":defaultLightSet.message";
relationship "link" ":lightLinker1" "aiStandard1SG.message" ":defaultLightSet.message";
relationship "link" ":lightLinker1" "aiUtility1SG.message" ":defaultLightSet.message";
relationship "link" ":lightLinker1" "aiStandard2SG.message" ":defaultLightSet.message";
relationship "link" ":lightLinker1" "aiHair1SG.message" ":defaultLightSet.message";
relationship "link" ":lightLinker1" "aiStandard4SG.message" ":defaultLightSet.message";
relationship "shadowLink" ":lightLinker1" ":initialShadingGroup.message" ":defaultLightSet.message";
relationship "shadowLink" ":lightLinker1" ":initialParticleSE.message" ":defaultLightSet.message";
relationship "shadowLink" ":lightLinker1" "aiStandard1SG.message" ":defaultLightSet.message";
relationship "shadowLink" ":lightLinker1" "aiUtility1SG.message" ":defaultLightSet.message";
relationship "shadowLink" ":lightLinker1" "aiStandard2SG.message" ":defaultLightSet.message";
relationship "shadowLink" ":lightLinker1" "aiHair1SG.message" ":defaultLightSet.message";
relationship "shadowLink" ":lightLinker1" "aiStandard4SG.message" ":defaultLightSet.message";
connectAttr "layerManager.dli[0]" "defaultLayer.id";
connectAttr "renderLayerManager.rlmi[0]" "defaultRenderLayer.rlid";
connectAttr ":defaultArnoldFilter.msg" ":defaultArnoldRenderOptions.filt";
connectAttr ":defaultArnoldDriver.msg" ":defaultArnoldRenderOptions.drvr";
connectAttr "aiStandard1SG.msg" "materialInfo1.sg";
connectAttr "aiUtility1SG.msg" "materialInfo2.sg";
connectAttr "aiStandard2SG.msg" "materialInfo3.sg";
connectAttr "aiHair1.out" "aiHair1SG.ss";
connectAttr "aiHair1SG.msg" "materialInfo4.sg";
connectAttr "aiHair1.msg" "materialInfo4.m";
connectAttr "aiHair1.msg" "materialInfo4.t" -na;
connectAttr "aiStandard4.out" "aiStandard4SG.ss";
connectAttr "pSphereShape1.iog" "aiStandard4SG.dsm" -na;
connectAttr "pSphereShape2.iog" "aiStandard4SG.dsm" -na;
connectAttr "aiStandard4SG.msg" "materialInfo5.sg";
connectAttr "aiStandard4.msg" "materialInfo5.m";
connectAttr "aiStandard4.msg" "materialInfo5.t" -na;
connectAttr "aiStandard1SG.pa" ":renderPartition.st" -na;
connectAttr "aiUtility1SG.pa" ":renderPartition.st" -na;
connectAttr "aiStandard2SG.pa" ":renderPartition.st" -na;
connectAttr "aiHair1SG.pa" ":renderPartition.st" -na;
connectAttr "aiStandard4SG.pa" ":renderPartition.st" -na;
connectAttr "aiHair1.msg" ":defaultShaderList1.s" -na;
connectAttr "aiStandard3.msg" ":defaultShaderList1.s" -na;
connectAttr "aiStandard4.msg" ":defaultShaderList1.s" -na;
connectAttr "directionalLightShape1.ltd" ":lightList1.l" -na;
connectAttr "directionalLight1.iog" ":defaultLightSet.dsm" -na;
// End of test.ma

//Maya ASCII 2017ff05 scene
//Name: test.ma
//Last modified: Fri, Mar 09, 2018 11:17:41 AM
//Codeset: 1252
requires maya "2017ff05";
requires -nodeType "aiOptions" -nodeType "aiAOVDriver" -nodeType "aiAOVFilter" -nodeType "aiStandardSurface"
		 "mtoa" "2.1.1.wip";
requires "stereoCamera" "10.0";
currentUnit -l centimeter -a degree -t film;
fileInfo "application" "maya";
fileInfo "product" "Maya 2017";
fileInfo "version" "2017";
fileInfo "cutIdentifier" "201706020738-1017329";
fileInfo "osv" "Microsoft Windows 7 Enterprise Edition, 64-bit Windows 7 Service Pack 1 (Build 7601)\n";
createNode transform -s -n "persp";
	rename -uid "DBFEEF08-4DFA-FE5D-9971-309B435371BD";
	setAttr ".v" no;
	setAttr ".t" -type "double3" 13.511425177544998 36.14081972808065 7.0588861436796373 ;
	setAttr ".r" -type "double3" -91.53835272960464 0.2000000000000258 -1.2424117416441811e-017 ;
createNode camera -s -n "perspShape" -p "persp";
	rename -uid "4222E920-4591-30A3-3B7F-9FBDD38ADCBE";
	setAttr -k off ".v" no;
	setAttr ".fl" 34.999999999999986;
	setAttr ".coi" 36.587757769529887;
	setAttr ".imn" -type "string" "persp";
	setAttr ".den" -type "string" "persp_depth";
	setAttr ".man" -type "string" "persp_mask";
	setAttr ".tp" -type "double3" 8.5728802034224074 0 2.3337462221316123 ;
	setAttr ".hc" -type "string" "viewSet -p %camera";
	setAttr ".ai_translator" -type "string" "perspective";
createNode transform -s -n "top";
	rename -uid "1446AE5B-474F-7897-0211-B993139BA969";
	setAttr ".v" no;
	setAttr ".t" -type "double3" 0 100.1 0 ;
	setAttr ".r" -type "double3" -89.999999999999986 0 0 ;
createNode camera -s -n "topShape" -p "top";
	rename -uid "093D7012-410C-49A9-56A2-F3B6889643B4";
	setAttr -k off ".v" no;
	setAttr ".rnd" no;
	setAttr ".coi" 100.1;
	setAttr ".ow" 30;
	setAttr ".imn" -type "string" "top";
	setAttr ".den" -type "string" "top_depth";
	setAttr ".man" -type "string" "top_mask";
	setAttr ".hc" -type "string" "viewSet -t %camera";
	setAttr ".o" yes;
	setAttr ".ai_translator" -type "string" "orthographic";
createNode transform -s -n "front";
	rename -uid "04535717-4825-17F8-9ACA-628F1E54C323";
	setAttr ".v" no;
	setAttr ".t" -type "double3" 0 0 100.1 ;
createNode camera -s -n "frontShape" -p "front";
	rename -uid "32A59482-4177-3113-ECD2-91A70669DCDA";
	setAttr -k off ".v" no;
	setAttr ".rnd" no;
	setAttr ".coi" 100.1;
	setAttr ".ow" 30;
	setAttr ".imn" -type "string" "front";
	setAttr ".den" -type "string" "front_depth";
	setAttr ".man" -type "string" "front_mask";
	setAttr ".hc" -type "string" "viewSet -f %camera";
	setAttr ".o" yes;
	setAttr ".ai_translator" -type "string" "orthographic";
createNode transform -s -n "side";
	rename -uid "A8A5135E-4673-D291-BAEB-319786FDDA8A";
	setAttr ".v" no;
	setAttr ".t" -type "double3" 100.1 0 0 ;
	setAttr ".r" -type "double3" 0 89.999999999999986 0 ;
createNode camera -s -n "sideShape" -p "side";
	rename -uid "E60C9757-4F7F-CBF1-31FD-8FBE4DD21E17";
	setAttr -k off ".v" no;
	setAttr ".rnd" no;
	setAttr ".coi" 100.1;
	setAttr ".ow" 30;
	setAttr ".imn" -type "string" "side";
	setAttr ".den" -type "string" "side_depth";
	setAttr ".man" -type "string" "side_mask";
	setAttr ".hc" -type "string" "viewSet -s %camera";
	setAttr ".o" yes;
	setAttr ".ai_translator" -type "string" "orthographic";
createNode transform -n "pPlane1";
	rename -uid "0DDEE7A1-464A-CC54-38FB-C2BC0270A831";
	setAttr ".t" -type "double3" 1.8991104903154916 0 2.3337462221316119 ;
createNode mesh -n "pPlaneShape1" -p "pPlane1";
	rename -uid "CAE4593B-40A1-4EE0-6A3E-D3B891EE554C";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-min 0 -smx 1 -at "float";
	addAttr -s false -ci true -k true -sn "ai_subdiv_dicing_camera" -ln "aiSubdivDicingCamera" 
		-at "message";
	addAttr -ci true -k true -sn "ai_volume_container_mode" -ln "aiVolumeContainerMode" 
		-min 0 -max 1 -en "Mesh:Bounding Box" -at "enum";
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
	setAttr ".ugsdt" no;
	setAttr -s 9 ".pt[0:8]" -type "float3"  0 -2.3841858e-007 0 0 -2.3841858e-007 
		0 0 -2.3841858e-007 0 0 2.646978e-023 6.6174449e-024 0 2.646978e-023 6.6174449e-024 
		0 2.646978e-023 6.6174449e-024 0 2.3841858e-007 0 0 2.3841858e-007 0 0 2.3841858e-007 
		0;
	setAttr ".vnm" 0;
	setAttr ".ai_translator" -type "string" "polymesh";
createNode transform -n "directionalLight1";
	rename -uid "4C7E4AA9-4FA7-483F-80B5-488ACC21189A";
	setAttr ".t" -type "double3" 0 5.1628475931277249 3.1280933277221914 ;
	setAttr ".r" -type "double3" -38.968848577268517 0 0 ;
createNode directionalLight -n "directionalLightShape1" -p "directionalLight1";
	rename -uid "1785AA63-4593-C8D4-9960-ECBFC2E7ED24";
	addAttr -ci true -sn "ai_affect_volumetrics" -ln "aiAffectVolumetrics" -dv 1 -min 
		0 -max 1 -at "bool";
	addAttr -ci true -sn "ai_bounce_factor" -ln "aiBounceFactor" -dv 1 -min 0 -smx 20 
		-at "float";
	addAttr -ci true -sn "ai_bounces" -ln "aiBounces" -dv 999 -min 0 -max 10000 -at "long";
	setAttr -k off ".v";
	setAttr ".in" 3;
	setAttr ".urs" no;
	setAttr ".rdl" 1;
createNode transform -n "pPlane2";
	rename -uid "9F2D76D2-4818-68A0-CE2B-D38C45D00104";
	setAttr ".t" -type "double3" 13.680371051832623 0 2.3337462221316119 ;
createNode mesh -n "pPlaneShape2" -p "pPlane2";
	rename -uid "76290018-487F-1744-7A08-4587E246B00D";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-min 0 -smx 1 -at "float";
	addAttr -s false -ci true -k true -sn "ai_subdiv_dicing_camera" -ln "aiSubdivDicingCamera" 
		-at "message";
	addAttr -ci true -k true -sn "ai_volume_container_mode" -ln "aiVolumeContainerMode" 
		-min 0 -max 1 -en "Mesh:Bounding Box" -at "enum";
	addAttr -ci true -sn "mso" -ln "miShadingSamplesOverride" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "msh" -ln "miShadingSamples" -min 0 -smx 8 -at "float";
	addAttr -ci true -sn "mdo" -ln "miMaxDisplaceOverride" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "mmd" -ln "miMaxDisplace" -min 0 -smx 1 -at "float";
	setAttr -k off ".v";
	setAttr ".vir" yes;
	setAttr ".vif" yes;
	setAttr ".pv" -type "double2" 1.0315449237823486 1.5064982054507841 ;
	setAttr ".uvst[0].uvsn" -type "string" "map1";
	setAttr -s 16 ".uvst[0].uvsp[0:15]" -type "float2" 1.24386108 1.30308712
		 1.74386108 1.30308712 0.81922877 1.27871132 0.29912809 0.29390275 1.23789382 0.30627072
		 0.81922877 1.70990944 0.29912809 0.72510093 1.23789382 0.73746872 1.73789382 0.73746872
		 0.79912812 0.72510093 0.31922877 1.70990944 1.74386108 1.734285 0.79912812 0.29390275
		 1.73789382 0.30627072 1.24386108 1.734285 0.31922877 1.27871132;
	setAttr ".cuvs" -type "string" "map1";
	setAttr ".dcc" -type "string" "Ambient+Diffuse";
	setAttr ".covm[0]"  0 1 1;
	setAttr ".cdvm[0]"  0 1 1;
	setAttr ".ugsdt" no;
	setAttr -s 9 ".pt[0:8]" -type "float3"  0 -2.3841858e-007 0 0 -2.3841858e-007 
		0 0 -2.3841858e-007 0 0 2.646978e-023 6.6174449e-024 0 2.646978e-023 6.6174449e-024 
		0 2.646978e-023 6.6174449e-024 0 2.3841858e-007 0 0 2.3841858e-007 0 0 2.3841858e-007 
		0;
	setAttr -s 9 ".vt[0:8]"  -5.78140497 -2.3841858e-007 4.98586178 0 -2.3841858e-007 4.98586178
		 5.78140497 -2.3841858e-007 4.98586178 -5.78140497 2.646978e-023 6.6174449e-024 0 2.646978e-023 6.6174449e-024
		 5.78140497 2.646978e-023 6.6174449e-024 -5.78140497 2.3841858e-007 -4.98586178 0 2.3841858e-007 -4.98586178
		 5.78140497 2.3841858e-007 -4.98586178;
	setAttr -s 12 ".ed[0:11]"  0 1 0 0 3 0 1 2 0 1 4 1 2 5 0 3 4 1 3 6 0
		 4 5 1 4 7 1 5 8 0 6 7 0 7 8 0;
	setAttr -s 4 -ch 16 ".fc[0:3]" -type "polyFaces" 
		f 4 0 3 -6 -2
		mu 0 4 0 1 11 14
		f 4 2 4 -8 -4
		mu 0 4 15 2 5 10
		f 4 5 8 -11 -7
		mu 0 4 3 12 9 6
		f 4 7 9 -12 -9
		mu 0 4 4 13 8 7;
	setAttr ".cd" -type "dataPolyComponent" Index_Data Edge 0 ;
	setAttr ".cvd" -type "dataPolyComponent" Index_Data Vertex 0 ;
	setAttr ".pd[0]" -type "dataPolyComponent" Index_Data UV 0 ;
	setAttr ".hfd" -type "dataPolyComponent" Index_Data Face 0 ;
	setAttr ".vnm" 0;
	setAttr ".ai_translator" -type "string" "polymesh";
createNode transform -n "group";
	rename -uid "B0BB80BA-4E63-AFC1-2FC5-20BB2418B732";
	setAttr ".t" -type "double3" 0 0 10.530631381358173 ;
	setAttr ".rp" -type "double3" 1.8991104903154916 0 2.3337462221316123 ;
	setAttr ".sp" -type "double3" 1.8991104903154916 0 2.3337462221316123 ;
createNode transform -n "pPlane3";
	rename -uid "979F988D-49F1-0967-C2F9-85BFF0FEA1E1";
	setAttr ".t" -type "double3" -1.2499089037731976 0 10.804117128822247 ;
	setAttr ".s" -type "double3" 1.1583769136445605 1.1583769136445605 1.1583769136445605 ;
createNode mesh -n "pPlaneShape3" -p "pPlane3";
	rename -uid "1198D903-4948-FE4F-84C7-C3848E4928D9";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-min 0 -smx 1 -at "float";
	addAttr -s false -ci true -k true -sn "ai_subdiv_dicing_camera" -ln "aiSubdivDicingCamera" 
		-at "message";
	addAttr -ci true -k true -sn "ai_volume_container_mode" -ln "aiVolumeContainerMode" 
		-min 0 -max 1 -en "Mesh:Bounding Box" -at "enum";
	addAttr -ci true -sn "mtoa_constant_myString" -ln "mtoa_constant_myString" -dt "string";
	setAttr -k off ".v";
	setAttr ".vir" yes;
	setAttr ".vif" yes;
	setAttr ".uvst[0].uvsn" -type "string" "map1";
	setAttr ".cuvs" -type "string" "map1";
	setAttr ".dcc" -type "string" "Ambient+Diffuse";
	setAttr ".covm[0]"  0 1 1;
	setAttr ".cdvm[0]"  0 1 1;
	setAttr ".ugsdt" no;
	setAttr ".vnm" 0;
	setAttr ".ai_translator" -type "string" "polymesh";
	setAttr -k on ".mtoa_constant_myString" -type "string" "1001";
createNode transform -n "pPlane4";
	rename -uid "6223ECB8-46AD-224B-A01E-E987F5023D28";
	setAttr ".t" -type "double3" 4.8807076606846103 0 10.804117128822247 ;
	setAttr ".s" -type "double3" 1.1583769136445605 1.1583769136445605 1.1583769136445605 ;
createNode mesh -n "pPlaneShape4" -p "pPlane4";
	rename -uid "B879BA2D-41CB-82E7-26F8-D0B569243088";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-min 0 -smx 1 -at "float";
	addAttr -s false -ci true -k true -sn "ai_subdiv_dicing_camera" -ln "aiSubdivDicingCamera" 
		-at "message";
	addAttr -ci true -k true -sn "ai_volume_container_mode" -ln "aiVolumeContainerMode" 
		-min 0 -max 1 -en "Mesh:Bounding Box" -at "enum";
	addAttr -ci true -sn "mtoa_constant_myString" -ln "mtoa_constant_myString" -dt "string";
	setAttr -k off ".v";
	setAttr ".vir" yes;
	setAttr ".vif" yes;
	setAttr ".uvst[0].uvsn" -type "string" "map1";
	setAttr -s 4 ".uvst[0].uvsp[0:3]" -type "float2" 0 0 1 0 0 0.96074474
		 1 0.96074474;
	setAttr ".cuvs" -type "string" "map1";
	setAttr ".dcc" -type "string" "Ambient+Diffuse";
	setAttr ".covm[0]"  0 1 1;
	setAttr ".cdvm[0]"  0 1 1;
	setAttr ".ugsdt" no;
	setAttr -s 4 ".vt[0:3]"  -2.49554539 -5.3237016e-016 2.39758205 2.49554539 -5.3237016e-016 2.39758205
		 -2.49554539 5.3237016e-016 -2.39758205 2.49554539 5.3237016e-016 -2.39758205;
	setAttr -s 4 ".ed[0:3]"  0 1 0 0 2 0 1 3 0 2 3 0;
	setAttr -ch 4 ".fc[0]" -type "polyFaces" 
		f 4 0 2 -4 -2
		mu 0 4 0 1 3 2;
	setAttr ".cd" -type "dataPolyComponent" Index_Data Edge 0 ;
	setAttr ".cvd" -type "dataPolyComponent" Index_Data Vertex 0 ;
	setAttr ".pd[0]" -type "dataPolyComponent" Index_Data UV 0 ;
	setAttr ".hfd" -type "dataPolyComponent" Index_Data Face 0 ;
	setAttr ".vnm" 0;
	setAttr ".ai_translator" -type "string" "polymesh";
	setAttr -k on ".mtoa_constant_myString" -type "string" "1002";
createNode transform -n "pPlane5";
	rename -uid "81F0B76C-4817-77BC-A4C2-93BEECCFAA50";
	setAttr ".t" -type "double3" -1.2726613752561029 0 16.628541820402706 ;
	setAttr ".s" -type "double3" 1.1583769136445605 1.1583769136445605 1.1583769136445605 ;
createNode mesh -n "pPlaneShape5" -p "pPlane5";
	rename -uid "EED7B72A-46D1-6E85-54E7-5D8F4DCCD09E";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-min 0 -smx 1 -at "float";
	addAttr -s false -ci true -k true -sn "ai_subdiv_dicing_camera" -ln "aiSubdivDicingCamera" 
		-at "message";
	addAttr -ci true -k true -sn "ai_volume_container_mode" -ln "aiVolumeContainerMode" 
		-min 0 -max 1 -en "Mesh:Bounding Box" -at "enum";
	addAttr -ci true -sn "mtoa_constant_myString" -ln "mtoa_constant_myString" -dt "string";
	setAttr -k off ".v";
	setAttr ".vir" yes;
	setAttr ".vif" yes;
	setAttr ".uvst[0].uvsn" -type "string" "map1";
	setAttr -s 4 ".uvst[0].uvsp[0:3]" -type "float2" 0 0 1 0 0 0.96074474
		 1 0.96074474;
	setAttr ".cuvs" -type "string" "map1";
	setAttr ".dcc" -type "string" "Ambient+Diffuse";
	setAttr ".covm[0]"  0 1 1;
	setAttr ".cdvm[0]"  0 1 1;
	setAttr ".ugsdt" no;
	setAttr -s 4 ".vt[0:3]"  -2.49554539 -5.3237016e-016 2.39758205 2.49554539 -5.3237016e-016 2.39758205
		 -2.49554539 5.3237016e-016 -2.39758205 2.49554539 5.3237016e-016 -2.39758205;
	setAttr -s 4 ".ed[0:3]"  0 1 0 0 2 0 1 3 0 2 3 0;
	setAttr -ch 4 ".fc[0]" -type "polyFaces" 
		f 4 0 2 -4 -2
		mu 0 4 0 1 3 2;
	setAttr ".cd" -type "dataPolyComponent" Index_Data Edge 0 ;
	setAttr ".cvd" -type "dataPolyComponent" Index_Data Vertex 0 ;
	setAttr ".pd[0]" -type "dataPolyComponent" Index_Data UV 0 ;
	setAttr ".hfd" -type "dataPolyComponent" Index_Data Face 0 ;
	setAttr ".vnm" 0;
	setAttr ".ai_translator" -type "string" "polymesh";
	setAttr -k on ".mtoa_constant_myString" -type "string" "1003";
createNode transform -n "pPlane6";
	rename -uid "7992DD59-44E3-136E-FDF7-2DA93A1DF375";
	setAttr ".t" -type "double3" 4.9255348223961999 0 16.675014370506702 ;
	setAttr ".s" -type "double3" 1.1583769136445605 1.1583769136445605 1.1583769136445605 ;
createNode mesh -n "pPlaneShape6" -p "pPlane6";
	rename -uid "807638D4-4672-5309-39F9-93B735089ED8";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-min 0 -smx 1 -at "float";
	addAttr -s false -ci true -k true -sn "ai_subdiv_dicing_camera" -ln "aiSubdivDicingCamera" 
		-at "message";
	addAttr -ci true -k true -sn "ai_volume_container_mode" -ln "aiVolumeContainerMode" 
		-min 0 -max 1 -en "Mesh:Bounding Box" -at "enum";
	addAttr -ci true -sn "mtoa_constant_myString" -ln "mtoa_constant_myString" -dt "string";
	setAttr -k off ".v";
	setAttr ".vir" yes;
	setAttr ".vif" yes;
	setAttr ".uvst[0].uvsn" -type "string" "map1";
	setAttr -s 4 ".uvst[0].uvsp[0:3]" -type "float2" 0 0 1 0 0 0.96074474
		 1 0.96074474;
	setAttr ".cuvs" -type "string" "map1";
	setAttr ".dcc" -type "string" "Ambient+Diffuse";
	setAttr ".covm[0]"  0 1 1;
	setAttr ".cdvm[0]"  0 1 1;
	setAttr ".ugsdt" no;
	setAttr -s 4 ".vt[0:3]"  -2.49554539 -5.3237016e-016 2.39758205 2.49554539 -5.3237016e-016 2.39758205
		 -2.49554539 5.3237016e-016 -2.39758205 2.49554539 5.3237016e-016 -2.39758205;
	setAttr -s 4 ".ed[0:3]"  0 1 0 0 2 0 1 3 0 2 3 0;
	setAttr -ch 4 ".fc[0]" -type "polyFaces" 
		f 4 0 2 -4 -2
		mu 0 4 0 1 3 2;
	setAttr ".cd" -type "dataPolyComponent" Index_Data Edge 0 ;
	setAttr ".cvd" -type "dataPolyComponent" Index_Data Vertex 0 ;
	setAttr ".pd[0]" -type "dataPolyComponent" Index_Data UV 0 ;
	setAttr ".hfd" -type "dataPolyComponent" Index_Data Face 0 ;
	setAttr ".vnm" 0;
	setAttr ".ai_translator" -type "string" "polymesh";
	setAttr -k on ".mtoa_constant_myString" -type "string" "1004";
createNode transform -n "pPlane7";
	rename -uid "76F903DB-47B9-D058-1122-48A69458BFD2";
	setAttr ".t" -type "double3" 10.880730227047321 0 10.804117128822247 ;
	setAttr ".s" -type "double3" 1.1583769136445605 1.1583769136445605 1.1583769136445605 ;
createNode mesh -n "pPlaneShape1001" -p "|pPlane7";
	rename -uid "EDB25402-46C2-1472-8D27-16BBA0F7FF1B";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-min 0 -smx 1 -at "float";
	addAttr -s false -ci true -k true -sn "ai_subdiv_dicing_camera" -ln "aiSubdivDicingCamera" 
		-at "message";
	addAttr -ci true -k true -sn "ai_volume_container_mode" -ln "aiVolumeContainerMode" 
		-min 0 -max 1 -en "Mesh:Bounding Box" -at "enum";
	addAttr -ci true -sn "mtoa_constant_myString" -ln "mtoa_constant_myString" -dt "string";
	setAttr -k off ".v";
	setAttr ".vir" yes;
	setAttr ".vif" yes;
	setAttr ".uvst[0].uvsn" -type "string" "map1";
	setAttr -s 4 ".uvst[0].uvsp[0:3]" -type "float2" 0 0 1 0 0 0.96074474
		 1 0.96074474;
	setAttr ".cuvs" -type "string" "map1";
	setAttr ".dcc" -type "string" "Ambient+Diffuse";
	setAttr ".covm[0]"  0 1 1;
	setAttr ".cdvm[0]"  0 1 1;
	setAttr ".ugsdt" no;
	setAttr -s 4 ".vt[0:3]"  -2.49554539 -5.3237016e-016 2.39758205 2.49554539 -5.3237016e-016 2.39758205
		 -2.49554539 5.3237016e-016 -2.39758205 2.49554539 5.3237016e-016 -2.39758205;
	setAttr -s 4 ".ed[0:3]"  0 1 0 0 2 0 1 3 0 2 3 0;
	setAttr -ch 4 ".fc[0]" -type "polyFaces" 
		f 4 0 2 -4 -2
		mu 0 4 0 1 3 2;
	setAttr ".cd" -type "dataPolyComponent" Index_Data Edge 0 ;
	setAttr ".cvd" -type "dataPolyComponent" Index_Data Vertex 0 ;
	setAttr ".pd[0]" -type "dataPolyComponent" Index_Data UV 0 ;
	setAttr ".hfd" -type "dataPolyComponent" Index_Data Face 0 ;
	setAttr ".vnm" 0;
	setAttr ".ai_translator" -type "string" "polymesh";
	setAttr -k on ".mtoa_constant_myString" -type "string" "1001";
createNode transform -n "group1";
	rename -uid "6AC891CF-4E3B-55A5-F505-90BDD5932048";
	setAttr ".t" -type "double3" 6.0639464973609503 0 0 ;
	setAttr ".rp" -type "double3" 11.41947298636258 0 10.804117128822247 ;
	setAttr ".sp" -type "double3" 11.41947298636258 0 10.804117128822247 ;
createNode transform -n "pasted__pPlane7" -p "group1";
	rename -uid "560D2DD8-47AB-4428-4918-978F3EA8E593";
	setAttr ".t" -type "double3" 10.880730227047321 0 10.804117128822247 ;
	setAttr ".s" -type "double3" 1.1583769136445605 1.1583769136445605 1.1583769136445605 ;
createNode mesh -n "pPlaneShape1002" -p "pasted__pPlane7";
	rename -uid "F13583B2-4B4A-831A-28DE-E3A541E74FB2";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-min 0 -smx 1 -at "float";
	addAttr -s false -ci true -k true -sn "ai_subdiv_dicing_camera" -ln "aiSubdivDicingCamera" 
		-at "message";
	addAttr -ci true -k true -sn "ai_volume_container_mode" -ln "aiVolumeContainerMode" 
		-min 0 -max 1 -en "Mesh:Bounding Box" -at "enum";
	addAttr -ci true -sn "mtoa_constant_myString" -ln "mtoa_constant_myString" -dt "string";
	addAttr -ci true -sn "mso" -ln "miShadingSamplesOverride" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "msh" -ln "miShadingSamples" -min 0 -smx 8 -at "float";
	addAttr -ci true -sn "mdo" -ln "miMaxDisplaceOverride" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "mmd" -ln "miMaxDisplace" -min 0 -smx 1 -at "float";
	setAttr -k off ".v";
	setAttr ".vir" yes;
	setAttr ".vif" yes;
	setAttr ".uvst[0].uvsn" -type "string" "map1";
	setAttr -s 4 ".uvst[0].uvsp[0:3]" -type "float2" 0 0 1 0 0 0.96074474
		 1 0.96074474;
	setAttr ".cuvs" -type "string" "map1";
	setAttr ".dcc" -type "string" "Ambient+Diffuse";
	setAttr ".covm[0]"  0 1 1;
	setAttr ".cdvm[0]"  0 1 1;
	setAttr ".ugsdt" no;
	setAttr -s 4 ".vt[0:3]"  -2.49554539 -5.3237016e-016 2.39758205 2.49554539 -5.3237016e-016 2.39758205
		 -2.49554539 5.3237016e-016 -2.39758205 2.49554539 5.3237016e-016 -2.39758205;
	setAttr -s 4 ".ed[0:3]"  0 1 0 0 2 0 1 3 0 2 3 0;
	setAttr -ch 4 ".fc[0]" -type "polyFaces" 
		f 4 0 2 -4 -2
		mu 0 4 0 1 3 2;
	setAttr ".cd" -type "dataPolyComponent" Index_Data Edge 0 ;
	setAttr ".cvd" -type "dataPolyComponent" Index_Data Vertex 0 ;
	setAttr ".pd[0]" -type "dataPolyComponent" Index_Data UV 0 ;
	setAttr ".hfd" -type "dataPolyComponent" Index_Data Face 0 ;
	setAttr ".vnm" 0;
	setAttr ".ai_translator" -type "string" "polymesh";
	setAttr -k on ".mtoa_constant_myString" -type "string" "1001";
createNode transform -n "group2";
	rename -uid "C6BBC2D2-4E22-33D7-3A9C-9BB14DD1CE56";
	setAttr ".t" -type "double3" 0 0 5.8014485910920097 ;
	setAttr ".rp" -type "double3" 11.41947298636258 0 10.804117128822247 ;
	setAttr ".sp" -type "double3" 11.41947298636258 0 10.804117128822247 ;
createNode transform -n "pPlane7" -p "group2";
	rename -uid "0383199D-482E-CF12-EBC8-42A9DD5C9E03";
	setAttr ".t" -type "double3" 10.880730227047321 0 10.804117128822247 ;
	setAttr ".s" -type "double3" 1.1583769136445605 1.1583769136445605 1.1583769136445605 ;
createNode mesh -n "pPlaneShape1003" -p "|group2|pPlane7";
	rename -uid "E36273A3-40BA-AE15-3628-5989FD85A2C2";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-min 0 -smx 1 -at "float";
	addAttr -s false -ci true -k true -sn "ai_subdiv_dicing_camera" -ln "aiSubdivDicingCamera" 
		-at "message";
	addAttr -ci true -k true -sn "ai_volume_container_mode" -ln "aiVolumeContainerMode" 
		-min 0 -max 1 -en "Mesh:Bounding Box" -at "enum";
	addAttr -ci true -sn "mtoa_constant_myString" -ln "mtoa_constant_myString" -dt "string";
	setAttr -k off ".v";
	setAttr ".vir" yes;
	setAttr ".vif" yes;
	setAttr ".uvst[0].uvsn" -type "string" "map1";
	setAttr -s 4 ".uvst[0].uvsp[0:3]" -type "float2" 0 0 1 0 0 0.96074474
		 1 0.96074474;
	setAttr ".cuvs" -type "string" "map1";
	setAttr ".dcc" -type "string" "Ambient+Diffuse";
	setAttr ".covm[0]"  0 1 1;
	setAttr ".cdvm[0]"  0 1 1;
	setAttr ".ugsdt" no;
	setAttr -s 4 ".vt[0:3]"  -2.49554539 -5.3237016e-016 2.39758205 2.49554539 -5.3237016e-016 2.39758205
		 -2.49554539 5.3237016e-016 -2.39758205 2.49554539 5.3237016e-016 -2.39758205;
	setAttr -s 4 ".ed[0:3]"  0 1 0 0 2 0 1 3 0 2 3 0;
	setAttr -ch 4 ".fc[0]" -type "polyFaces" 
		f 4 0 2 -4 -2
		mu 0 4 0 1 3 2;
	setAttr ".cd" -type "dataPolyComponent" Index_Data Edge 0 ;
	setAttr ".cvd" -type "dataPolyComponent" Index_Data Vertex 0 ;
	setAttr ".pd[0]" -type "dataPolyComponent" Index_Data UV 0 ;
	setAttr ".hfd" -type "dataPolyComponent" Index_Data Face 0 ;
	setAttr ".vnm" 0;
	setAttr ".ai_translator" -type "string" "polymesh";
	setAttr -k on ".mtoa_constant_myString" -type "string" "1001";
createNode transform -n "group3";
	rename -uid "73CA57EF-49A2-AB5D-AC48-CDBC2BACB4AE";
	setAttr ".t" -type "double3" 5.9381250254779854 0 5.8367052810465854 ;
	setAttr ".rp" -type "double3" 11.41947298636258 0 10.804117128822247 ;
	setAttr ".sp" -type "double3" 11.41947298636258 0 10.804117128822247 ;
createNode transform -n "pPlane7" -p "group3";
	rename -uid "D30ECDD3-4C67-E1AE-4FF1-2FAB876B1499";
	setAttr ".t" -type "double3" 10.880730227047321 0 10.804117128822247 ;
	setAttr ".s" -type "double3" 1.1583769136445605 1.1583769136445605 1.1583769136445605 ;
createNode mesh -n "pPlaneShape1003" -p "|group3|pPlane7";
	rename -uid "8DE064BC-4F1D-B190-6138-1A9683FE80E8";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-min 0 -smx 1 -at "float";
	addAttr -s false -ci true -k true -sn "ai_subdiv_dicing_camera" -ln "aiSubdivDicingCamera" 
		-at "message";
	addAttr -ci true -k true -sn "ai_volume_container_mode" -ln "aiVolumeContainerMode" 
		-min 0 -max 1 -en "Mesh:Bounding Box" -at "enum";
	addAttr -ci true -sn "mtoa_constant_myString" -ln "mtoa_constant_myString" -dt "string";
	addAttr -ci true -sn "mso" -ln "miShadingSamplesOverride" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "msh" -ln "miShadingSamples" -min 0 -smx 8 -at "float";
	addAttr -ci true -sn "mdo" -ln "miMaxDisplaceOverride" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "mmd" -ln "miMaxDisplace" -min 0 -smx 1 -at "float";
	setAttr -k off ".v";
	setAttr ".vir" yes;
	setAttr ".vif" yes;
	setAttr ".uvst[0].uvsn" -type "string" "map1";
	setAttr -s 4 ".uvst[0].uvsp[0:3]" -type "float2" 0 0 1 0 0 0.96074474
		 1 0.96074474;
	setAttr ".cuvs" -type "string" "map1";
	setAttr ".dcc" -type "string" "Ambient+Diffuse";
	setAttr ".covm[0]"  0 1 1;
	setAttr ".cdvm[0]"  0 1 1;
	setAttr ".ugsdt" no;
	setAttr -s 4 ".vt[0:3]"  -2.49554539 -5.3237016e-016 2.39758205 2.49554539 -5.3237016e-016 2.39758205
		 -2.49554539 5.3237016e-016 -2.39758205 2.49554539 5.3237016e-016 -2.39758205;
	setAttr -s 4 ".ed[0:3]"  0 1 0 0 2 0 1 3 0 2 3 0;
	setAttr -ch 4 ".fc[0]" -type "polyFaces" 
		f 4 0 2 -4 -2
		mu 0 4 0 1 3 2;
	setAttr ".cd" -type "dataPolyComponent" Index_Data Edge 0 ;
	setAttr ".cvd" -type "dataPolyComponent" Index_Data Vertex 0 ;
	setAttr ".pd[0]" -type "dataPolyComponent" Index_Data UV 0 ;
	setAttr ".hfd" -type "dataPolyComponent" Index_Data Face 0 ;
	setAttr ".vnm" 0;
	setAttr ".ai_translator" -type "string" "polymesh";
	setAttr -k on ".mtoa_constant_myString" -type "string" "1001";
createNode transform -n "pPlane8";
	rename -uid "DB176252-4719-6A76-9EB5-CAADEA90F5FF";
	setAttr ".t" -type "double3" 25.498345327022669 0 2.3337462221316119 ;
createNode mesh -n "pPlaneShape8" -p "pPlane8";
	rename -uid "E81DAB8D-4456-9E09-01C6-C589B7DB3B25";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-min 0 -smx 1 -at "float";
	addAttr -s false -ci true -k true -sn "ai_subdiv_dicing_camera" -ln "aiSubdivDicingCamera" 
		-at "message";
	addAttr -ci true -k true -sn "ai_volume_container_mode" -ln "aiVolumeContainerMode" 
		-min 0 -max 1 -en "Mesh:Bounding Box" -at "enum";
	addAttr -ci true -sn "mso" -ln "miShadingSamplesOverride" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "msh" -ln "miShadingSamples" -min 0 -smx 8 -at "float";
	addAttr -ci true -sn "mdo" -ln "miMaxDisplaceOverride" -min 0 -max 1 -at "bool";
	addAttr -ci true -sn "mmd" -ln "miMaxDisplace" -min 0 -smx 1 -at "float";
	setAttr -k off ".v";
	setAttr ".vir" yes;
	setAttr ".vif" yes;
	setAttr ".pv" -type "double2" 1.0315449237823486 1.5064982054507841 ;
	setAttr ".uvst[0].uvsn" -type "string" "map1";
	setAttr -s 16 ".uvst[0].uvsp[0:15]" -type "float2" 1.24386108 1.30308712
		 1.74386108 1.30308712 0.81922877 1.27871132 0.29912809 0.29390275 1.23789382 0.30627072
		 0.81922877 1.70990944 0.29912809 0.72510093 1.23789382 0.73746872 1.73789382 0.73746872
		 0.79912812 0.72510093 0.31922877 1.70990944 1.74386108 1.734285 0.79912812 0.29390275
		 1.73789382 0.30627072 1.24386108 1.734285 0.31922877 1.27871132;
	setAttr ".cuvs" -type "string" "map1";
	setAttr ".dcc" -type "string" "Ambient+Diffuse";
	setAttr ".covm[0]"  0 1 1;
	setAttr ".cdvm[0]"  0 1 1;
	setAttr ".ugsdt" no;
	setAttr -s 9 ".pt[0:8]" -type "float3"  0 -2.3841858e-007 0 0 -2.3841858e-007 
		0 0 -2.3841858e-007 0 0 2.646978e-023 6.6174449e-024 0 2.646978e-023 6.6174449e-024 
		0 2.646978e-023 6.6174449e-024 0 2.3841858e-007 0 0 2.3841858e-007 0 0 2.3841858e-007 
		0;
	setAttr -s 9 ".vt[0:8]"  -5.78140497 -2.3841858e-007 4.98586178 0 -2.3841858e-007 4.98586178
		 5.78140497 -2.3841858e-007 4.98586178 -5.78140497 2.646978e-023 6.6174449e-024 0 2.646978e-023 6.6174449e-024
		 5.78140497 2.646978e-023 6.6174449e-024 -5.78140497 2.3841858e-007 -4.98586178 0 2.3841858e-007 -4.98586178
		 5.78140497 2.3841858e-007 -4.98586178;
	setAttr -s 12 ".ed[0:11]"  0 1 0 0 3 0 1 2 0 1 4 1 2 5 0 3 4 1 3 6 0
		 4 5 1 4 7 1 5 8 0 6 7 0 7 8 0;
	setAttr -s 4 -ch 16 ".fc[0:3]" -type "polyFaces" 
		f 4 0 3 -6 -2
		mu 0 4 0 1 11 14
		f 4 2 4 -8 -4
		mu 0 4 15 2 5 10
		f 4 5 8 -11 -7
		mu 0 4 3 12 9 6
		f 4 7 9 -12 -9
		mu 0 4 4 13 8 7;
	setAttr ".cd" -type "dataPolyComponent" Index_Data Edge 0 ;
	setAttr ".cvd" -type "dataPolyComponent" Index_Data Vertex 0 ;
	setAttr ".pd[0]" -type "dataPolyComponent" Index_Data UV 0 ;
	setAttr ".hfd" -type "dataPolyComponent" Index_Data Face 0 ;
	setAttr ".vnm" 0;
	setAttr ".ai_translator" -type "string" "polymesh";
createNode lightLinker -s -n "lightLinker1";
	rename -uid "1BBA68FC-4637-27BA-883E-8B936D772C71";
	setAttr -s 11 ".lnk";
	setAttr -s 11 ".slnk";
createNode displayLayerManager -n "layerManager";
	rename -uid "3AFECD50-4C32-3289-A98C-FDB37BDBC195";
createNode displayLayer -n "defaultLayer";
	rename -uid "71885654-41C5-9415-6E9F-569357D832CA";
createNode renderLayerManager -n "renderLayerManager";
	rename -uid "6EBE27E8-47C9-2E38-01DB-78889108B320";
createNode renderLayer -n "defaultRenderLayer";
	rename -uid "F6A31905-4628-FD38-3F24-559DB12C0D29";
	setAttr ".g" yes;
createNode polyPlane -n "polyPlane1";
	rename -uid "07B6496C-42D2-C8FB-654A-1087448FFB1A";
	setAttr ".w" 11.562810012906045;
	setAttr ".h" 9.9717231649193288;
	setAttr ".sw" 2;
	setAttr ".sh" 2;
	setAttr ".cuv" 2;
createNode aiOptions -s -n "defaultArnoldRenderOptions";
	rename -uid "156D0B0A-4CF5-BFDD-9646-D59BAC61E37C";
	addAttr -ci true -sn "ARV_options" -ln "ARV_options" -dt "string";
	setAttr ".ARV_options" -type "string" "Display Settings=0;Show Status Bar=1;Display Pixel Information=0;3D Manipulation=0;Show AOVs list=1;Show Cameras list=1;Show RGBA icon=1;Show Crop Region icon=1;Show 3D Manipulation icon=0;Show Debug Shading icon=0;Show Exposure icon=1;Show Gamma icon=0;Darken Out-Of-Region=0;Show Render Tiles=0;AOVs=Beauty;Test Resolution=100%;Log=Last Progressive Step;Save UI Threads=1;Debug Shading=Disabled;Color Management.Gamma=1;Color Management.Exposure=0;Background.BG=BG Color;Background.Color=0 0 0;Background.Image=;Background.Scale=1  1;Background.Offset=0  0;Background.Apply Color Management=1;Foreground.Enable FG=0;Foreground.Image=;Foreground.Scale=1  1;Foreground.Offset=0  0;Foreground.Apply Color Management=1;";
createNode aiAOVFilter -s -n "defaultArnoldFilter";
	rename -uid "E9098BD3-459B-774E-5188-7E80C690492E";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVDriver -s -n "defaultArnoldDriver";
	rename -uid "EF64B324-4FAF-C4F4-8A9B-388AE095091C";
	addAttr -ci true -sn "gamma" -ln "gamma" -dv 2.2000000476837158 -min 9.9999997473787516e-005 
		-smx 5 -at "float";
	addAttr -ci true -sn "dither_amplitude" -ln "ditherAmplitude" -dv 1 -at "float";
	addAttr -ci true -sn "host_name" -ln "hostName" -dt "string";
	setAttr ".ai_translator" -type "string" "tif";
createNode shadingEngine -n "aiStandard1SG";
	rename -uid "33F032F4-4D5B-4FA5-8A02-95AA119C82EC";
	addAttr -ci true -k true -sn "ai_user_options" -ln "aiUserOptions" -dt "string";
	setAttr ".ihi" 0;
	setAttr ".ro" yes;
createNode materialInfo -n "materialInfo1";
	rename -uid "9F17C50C-4998-8F26-294C-739F485AD090";
createNode file -n "file_udim";
	rename -uid "C56026E6-4D36-B02E-9800-31B089BA6623";
	setAttr ".ftn" -type "string" "./test/mari.<udim>.tif";
	setAttr ".cs" -type "string" "sRGB";
createNode place2dTexture -n "place2dTexture1";
	rename -uid "3B261CBA-4E79-51D3-3B68-02AB2E633418";
createNode script -n "sceneConfigurationScriptNode";
	rename -uid "57A3BC76-4F7B-B9C3-5120-FBA130D7BD71";
	setAttr ".b" -type "string" "playbackOptions -min 1 -max 24 -ast 1 -aet 48 ";
	setAttr ".st" 6;
createNode polyTweakUV -n "polyTweakUV1";
	rename -uid "2C416A95-4AD7-6CCA-2551-1EB232864C21";
	setAttr ".uopa" yes;
	setAttr -s 9 ".uvtk[0:8]" -type "float2" 0.033340443 0.038897183 0.033340458
		 0.038897183 0.033340398 0.038897183 0.033340443 0.03889719 0.033340458 0.03889719
		 0.033340398 0.03889719 0.033340443 0.03889716 0.033340458 0.03889716 0.033340398
		 0.03889716;
createNode polyMapCut -n "polyMapCut1";
	rename -uid "DE12B193-40E7-718E-0112-149B33A59B44";
	setAttr ".uopa" yes;
	setAttr ".ics" -type "componentList" 2 "e[8]" "e[10:11]";
createNode polyMapCut -n "polyMapCut2";
	rename -uid "F4B154E2-4AD3-CB16-7B8F-D8B07DA0B249";
	setAttr ".uopa" yes;
	setAttr ".ics" -type "componentList" 2 "e[8]" "e[10:11]";
createNode polyMapCut -n "polyMapCut3";
	rename -uid "0D29B340-4673-493B-9AA3-C5BC37B407FA";
	setAttr ".uopa" yes;
	setAttr ".ics" -type "componentList" 3 "e[3]" "e[5]" "e[7:8]";
createNode polyTweakUV -n "polyTweakUV2";
	rename -uid "676FD345-4D7C-3474-B07B-21AB6F6CA697";
	setAttr ".uopa" yes;
	setAttr -s 16 ".uvtk[0:15]" -type "float2" 3.11106253 0.26085815 3.11106253
		 0.26085815 1.63819313 0.26542473 0.26578763 -0.17619254 0.70455343 -0.16382457 1.63819313
		 0.26542473 0.26578763 -0.17619248 0.70455343 -0.16382466 0.70455343 -0.16382466 0.26578766
		 -0.17619248 1.63819313 0.26542473 3.11106253 0.260858 0.26578766 -0.17619254 0.70455343
		 -0.16382457 3.11106253 0.260858 1.63819313 0.26542473;
createNode shadingEngine -n "aiStandard2SG";
	rename -uid "42D0C806-4CA6-08D0-8581-BF81208A87CF";
	addAttr -ci true -k true -sn "ai_user_options" -ln "aiUserOptions" -dt "string";
	setAttr ".ihi" 0;
	setAttr ".ro" yes;
createNode materialInfo -n "materialInfo2";
	rename -uid "142F28FA-4CC9-7B0F-7A4C-BEA5D351EF8B";
createNode file -n "file_tile";
	rename -uid "9C58BA00-455A-8FBE-2F91-AC81AF8D9CF2";
	setAttr ".ftn" -type "string" "./test/mudbox.<tile>.tif";
	setAttr ".cs" -type "string" "sRGB";
createNode place2dTexture -n "place2dTexture2";
	rename -uid "43063722-4D9E-6F40-7271-6B9A78586FCE";
createNode materialInfo -n "pasted__materialInfo1";
	rename -uid "5C93C585-4BFD-EB82-770C-B7AD0E5AE6DB";
createNode shadingEngine -n "pasted__aiStandard1SG";
	rename -uid "636376A7-471B-F2E6-66C2-5096C4DB1F51";
	addAttr -ci true -k true -sn "ai_user_options" -ln "aiUserOptions" -dt "string";
	setAttr ".ihi" 0;
	setAttr ".ro" yes;
createNode file -n "pasted__file1";
	rename -uid "2421F4CC-4474-4760-2928-05A232F3C9AA";
	setAttr ".ftn" -type "string" "./test/mari.<udim>.tif";
	setAttr ".cs" -type "string" "sRGB";
createNode place2dTexture -n "pasted__place2dTexture1";
	rename -uid "A2DA241F-48FE-88B4-564E-56B72B3AD458";
createNode polyPlane -n "polyPlane2";
	rename -uid "7AA9E0D7-4172-F983-64BE-789B8F72F26F";
	setAttr ".w" 4.9910908449713203;
	setAttr ".h" 4.7951639057995674;
	setAttr ".sw" 1;
	setAttr ".sh" 1;
	setAttr ".cuv" 2;
createNode shadingEngine -n "aiStandard3SG";
	rename -uid "F8307035-4A22-E14A-4C97-41B35D72EC6D";
	addAttr -ci true -k true -sn "ai_user_options" -ln "aiUserOptions" -dt "string";
	setAttr ".ihi" 0;
	setAttr -s 4 ".dsm";
	setAttr ".ro" yes;
createNode materialInfo -n "materialInfo3";
	rename -uid "7D76D932-4FDA-FB82-6B8E-E2AF8D3C993C";
createNode file -n "file_attr";
	rename -uid "558F5419-4E69-B827-A415-0D85FEB80C90";
	setAttr ".ftn" -type "string" "./test/mari.<attr:myString>.tif";
	setAttr ".cs" -type "string" "sRGB";
createNode place2dTexture -n "place2dTexture3";
	rename -uid "FC251094-4E5F-D932-AB68-9EA6F9B3F7A9";
createNode shadingEngine -n "aiStandard4SG";
	rename -uid "AA85A0B1-4EAF-800C-1A3D-1289C0CAF10B";
	addAttr -ci true -k true -sn "ai_user_options" -ln "aiUserOptions" -dt "string";
	setAttr ".ihi" 0;
	setAttr ".ro" yes;
createNode materialInfo -n "materialInfo4";
	rename -uid "A49D7582-49DE-0D00-4520-8A847B53D890";
createNode file -n "file_utile_vtile";
	rename -uid "F91DDB7A-4DB1-BB7B-9E02-B49ACA312698";
	setAttr ".ftn" -type "string" "./test/mudbox._u<utile>_v<vtile>.tif";
	setAttr ".cs" -type "string" "sRGB";
createNode place2dTexture -n "place2dTexture4";
	rename -uid "C9DD8895-4642-B089-EFC4-AB96936306BC";
createNode materialInfo -n "pasted__materialInfo4";
	rename -uid "CE87CA4E-4D83-F91B-A3E0-B09644018909";
createNode shadingEngine -n "pasted__aiStandard4SG";
	rename -uid "F213EE03-41EF-0B70-97E2-34957EEF976D";
	addAttr -ci true -k true -sn "ai_user_options" -ln "aiUserOptions" -dt "string";
	setAttr ".ihi" 0;
	setAttr -s 2 ".dsm";
	setAttr ".ro" yes;
createNode file -n "file_shapeName";
	rename -uid "A49270BF-4F70-BD08-B223-5CB217271C60";
	setAttr ".ftn" -type "string" "./test/mari.<shapeName>.tif";
	setAttr ".cs" -type "string" "sRGB";
createNode place2dTexture -n "pasted__place2dTexture4";
	rename -uid "3A1290A0-462A-D0FB-1ED4-C2B8685D7BB1";
createNode materialInfo -n "pasted__materialInfo5";
	rename -uid "122456D7-454A-5B55-87A7-469D7A5B75A1";
createNode shadingEngine -n "pasted__aiStandard4SG1";
	rename -uid "01EB70B1-464B-2FA5-FB58-3A939BA4FF80";
	addAttr -ci true -k true -sn "ai_user_options" -ln "aiUserOptions" -dt "string";
	setAttr ".ihi" 0;
	setAttr ".ro" yes;
createNode file -n "pasted__file5";
	rename -uid "B56DB7A9-411E-D3DF-B85E-308D70761F28";
	setAttr ".ftn" -type "string" "./test/mari.<shapeName>.tif";
	setAttr ".cs" -type "string" "sRGB";
createNode place2dTexture -n "pasted__place2dTexture5";
	rename -uid "9671F7F1-4717-A4A0-613D-B3BB9487D380";
createNode materialInfo -n "pasted__materialInfo6";
	rename -uid "176C2DBE-40FD-5F2C-276C-00BF7162996F";
createNode shadingEngine -n "pasted__aiStandard4SG2";
	rename -uid "F1E5CBAD-4979-B2FA-7234-F88DE38360E1";
	addAttr -ci true -k true -sn "ai_user_options" -ln "aiUserOptions" -dt "string";
	setAttr ".ihi" 0;
	setAttr ".ro" yes;
createNode file -n "pasted__file6";
	rename -uid "AF8931B8-4E55-8C27-5EC4-5190473DB585";
	setAttr ".ftn" -type "string" "./test/mari.<shapeName>.tif";
	setAttr ".cs" -type "string" "sRGB";
createNode place2dTexture -n "pasted__place2dTexture6";
	rename -uid "AB5E24DF-4340-F7DF-5645-D8A1529BDED9";
createNode shadingEngine -n "aiStandard5SG";
	rename -uid "B59368C9-4C68-88D8-3C9D-F19C0CADD692";
	addAttr -ci true -k true -sn "ai_user_options" -ln "aiUserOptions" -dt "string";
	setAttr ".ihi" 0;
	setAttr -s 2 ".dsm";
	setAttr ".ro" yes;
createNode materialInfo -n "materialInfo5";
	rename -uid "4A49B4DE-49FA-966F-7839-178DA0B15A15";
createNode file -n "file_shapePath";
	rename -uid "B7CE3895-4D3B-41C4-ADE6-42B707C4E7D7";
	setAttr ".ftn" -type "string" "./test/mari.<shapePath>.tif";
	setAttr ".cs" -type "string" "sRGB";
createNode place2dTexture -n "place2dTexture5";
	rename -uid "583916F2-4974-DF28-99D4-5BACA1770BD9";
createNode shapeEditorManager -n "shapeEditorManager";
	rename -uid "A6D2FDAC-4391-B495-DF50-AF919A86062E";
createNode poseInterpolatorManager -n "poseInterpolatorManager";
	rename -uid "417944E0-49BE-9A10-B0B1-37984C5FE8C0";
createNode aiAOVDriver -s -n "defaultArnoldDisplayDriver";
	rename -uid "E400FB3A-4E67-4C8F-F753-C0BB1E6E55B7";
	setAttr ".output_mode" 0;
	setAttr ".ai_translator" -type "string" "maya";
createNode script -n "uiConfigurationScriptNode";
	rename -uid "E6531675-4C7C-0042-9DFB-FFAF019F4E25";
	setAttr ".b" -type "string" (
		"// Maya Mel UI Configuration File.\n//\n//  This script is machine generated.  Edit at your own risk.\n//\n//\n\nglobal string $gMainPane;\nif (`paneLayout -exists $gMainPane`) {\n\n\tglobal int $gUseScenePanelConfig;\n\tint    $useSceneConfig = $gUseScenePanelConfig;\n\tint    $menusOkayInPanels = `optionVar -q allowMenusInPanels`;\tint    $nVisPanes = `paneLayout -q -nvp $gMainPane`;\n\tint    $nPanes = 0;\n\tstring $editorName;\n\tstring $panelName;\n\tstring $itemFilterName;\n\tstring $panelConfig;\n\n\t//\n\t//  get current state of the UI\n\t//\n\tsceneUIReplacement -update $gMainPane;\n\n\t$panelName = `sceneUIReplacement -getNextPanel \"modelPanel\" (localizedPanelLabel(\"Top View\")) `;\n\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tmodelPanel -edit -l (localizedPanelLabel(\"Top View\")) -mbv $menusOkayInPanels  $panelName;\n\t\t$editorName = $panelName;\n        modelEditor -e \n            -camera \"top\" \n            -useInteractiveMode 0\n            -displayLights \"default\" \n            -displayAppearance \"wireframe\" \n            -activeOnly 0\n"
		+ "            -ignorePanZoom 0\n            -wireframeOnShaded 0\n            -headsUpDisplay 1\n            -holdOuts 1\n            -selectionHiliteDisplay 1\n            -useDefaultMaterial 0\n            -bufferMode \"double\" \n            -twoSidedLighting 1\n            -backfaceCulling 0\n            -xray 0\n            -jointXray 0\n            -activeComponentsXray 0\n            -displayTextures 0\n            -smoothWireframe 0\n            -lineWidth 1\n            -textureAnisotropic 0\n            -textureHilight 1\n            -textureSampling 2\n            -textureDisplay \"modulate\" \n            -textureMaxSize 16384\n            -fogging 0\n            -fogSource \"fragment\" \n            -fogMode \"linear\" \n            -fogStart 0\n            -fogEnd 100\n            -fogDensity 0.1\n            -fogColor 0.5 0.5 0.5 1 \n            -depthOfFieldPreview 1\n            -maxConstantTransparency 1\n            -rendererName \"base_OpenGL_Renderer\" \n            -objectFilterShowInHUD 1\n            -isFiltered 0\n            -colorResolution 256 256 \n"
		+ "            -bumpResolution 512 512 \n            -textureCompression 0\n            -transparencyAlgorithm \"frontAndBackCull\" \n            -transpInShadows 0\n            -cullingOverride \"none\" \n            -lowQualityLighting 0\n            -maximumNumHardwareLights 1\n            -occlusionCulling 0\n            -shadingModel 0\n            -useBaseRenderer 0\n            -useReducedRenderer 0\n            -smallObjectCulling 0\n            -smallObjectThreshold -1 \n            -interactiveDisableShadows 0\n            -interactiveBackFaceCull 0\n            -sortTransparent 1\n            -nurbsCurves 1\n            -nurbsSurfaces 1\n            -polymeshes 1\n            -subdivSurfaces 1\n            -planes 1\n            -lights 1\n            -cameras 1\n            -controlVertices 1\n            -hulls 1\n            -grid 1\n            -imagePlane 1\n            -joints 1\n            -ikHandles 1\n            -deformers 1\n            -dynamics 1\n            -particleInstancers 1\n            -fluids 1\n            -hairSystems 1\n"
		+ "            -follicles 1\n            -nCloths 1\n            -nParticles 1\n            -nRigids 1\n            -dynamicConstraints 1\n            -locators 1\n            -manipulators 1\n            -pluginShapes 1\n            -dimensions 1\n            -handles 1\n            -pivots 1\n            -textures 1\n            -strokes 1\n            -motionTrails 1\n            -clipGhosts 1\n            -greasePencils 1\n            -shadows 0\n            -captureSequenceNumber -1\n            -width 1\n            -height 1\n            -sceneRenderFilter 0\n            $editorName;\n        modelEditor -e -viewSelected 0 $editorName;\n        modelEditor -e \n            -pluginObjects \"gpuCacheDisplayFilter\" 1 \n            $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextPanel \"modelPanel\" (localizedPanelLabel(\"Side View\")) `;\n\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tmodelPanel -edit -l (localizedPanelLabel(\"Side View\")) -mbv $menusOkayInPanels  $panelName;\n"
		+ "\t\t$editorName = $panelName;\n        modelEditor -e \n            -camera \"side\" \n            -useInteractiveMode 0\n            -displayLights \"default\" \n            -displayAppearance \"wireframe\" \n            -activeOnly 0\n            -ignorePanZoom 0\n            -wireframeOnShaded 0\n            -headsUpDisplay 1\n            -holdOuts 1\n            -selectionHiliteDisplay 1\n            -useDefaultMaterial 0\n            -bufferMode \"double\" \n            -twoSidedLighting 1\n            -backfaceCulling 0\n            -xray 0\n            -jointXray 0\n            -activeComponentsXray 0\n            -displayTextures 0\n            -smoothWireframe 0\n            -lineWidth 1\n            -textureAnisotropic 0\n            -textureHilight 1\n            -textureSampling 2\n            -textureDisplay \"modulate\" \n            -textureMaxSize 16384\n            -fogging 0\n            -fogSource \"fragment\" \n            -fogMode \"linear\" \n            -fogStart 0\n            -fogEnd 100\n            -fogDensity 0.1\n            -fogColor 0.5 0.5 0.5 1 \n"
		+ "            -depthOfFieldPreview 1\n            -maxConstantTransparency 1\n            -rendererName \"base_OpenGL_Renderer\" \n            -objectFilterShowInHUD 1\n            -isFiltered 0\n            -colorResolution 256 256 \n            -bumpResolution 512 512 \n            -textureCompression 0\n            -transparencyAlgorithm \"frontAndBackCull\" \n            -transpInShadows 0\n            -cullingOverride \"none\" \n            -lowQualityLighting 0\n            -maximumNumHardwareLights 1\n            -occlusionCulling 0\n            -shadingModel 0\n            -useBaseRenderer 0\n            -useReducedRenderer 0\n            -smallObjectCulling 0\n            -smallObjectThreshold -1 \n            -interactiveDisableShadows 0\n            -interactiveBackFaceCull 0\n            -sortTransparent 1\n            -nurbsCurves 1\n            -nurbsSurfaces 1\n            -polymeshes 1\n            -subdivSurfaces 1\n            -planes 1\n            -lights 1\n            -cameras 1\n            -controlVertices 1\n            -hulls 1\n"
		+ "            -grid 1\n            -imagePlane 1\n            -joints 1\n            -ikHandles 1\n            -deformers 1\n            -dynamics 1\n            -particleInstancers 1\n            -fluids 1\n            -hairSystems 1\n            -follicles 1\n            -nCloths 1\n            -nParticles 1\n            -nRigids 1\n            -dynamicConstraints 1\n            -locators 1\n            -manipulators 1\n            -pluginShapes 1\n            -dimensions 1\n            -handles 1\n            -pivots 1\n            -textures 1\n            -strokes 1\n            -motionTrails 1\n            -clipGhosts 1\n            -greasePencils 1\n            -shadows 0\n            -captureSequenceNumber -1\n            -width 1\n            -height 1\n            -sceneRenderFilter 0\n            $editorName;\n        modelEditor -e -viewSelected 0 $editorName;\n        modelEditor -e \n            -pluginObjects \"gpuCacheDisplayFilter\" 1 \n            $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextPanel \"modelPanel\" (localizedPanelLabel(\"Front View\")) `;\n"
		+ "\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tmodelPanel -edit -l (localizedPanelLabel(\"Front View\")) -mbv $menusOkayInPanels  $panelName;\n\t\t$editorName = $panelName;\n        modelEditor -e \n            -camera \"front\" \n            -useInteractiveMode 0\n            -displayLights \"default\" \n            -displayAppearance \"wireframe\" \n            -activeOnly 0\n            -ignorePanZoom 0\n            -wireframeOnShaded 0\n            -headsUpDisplay 1\n            -holdOuts 1\n            -selectionHiliteDisplay 1\n            -useDefaultMaterial 0\n            -bufferMode \"double\" \n            -twoSidedLighting 1\n            -backfaceCulling 0\n            -xray 0\n            -jointXray 0\n            -activeComponentsXray 0\n            -displayTextures 0\n            -smoothWireframe 0\n            -lineWidth 1\n            -textureAnisotropic 0\n            -textureHilight 1\n            -textureSampling 2\n            -textureDisplay \"modulate\" \n            -textureMaxSize 16384\n            -fogging 0\n"
		+ "            -fogSource \"fragment\" \n            -fogMode \"linear\" \n            -fogStart 0\n            -fogEnd 100\n            -fogDensity 0.1\n            -fogColor 0.5 0.5 0.5 1 \n            -depthOfFieldPreview 1\n            -maxConstantTransparency 1\n            -rendererName \"base_OpenGL_Renderer\" \n            -objectFilterShowInHUD 1\n            -isFiltered 0\n            -colorResolution 256 256 \n            -bumpResolution 512 512 \n            -textureCompression 0\n            -transparencyAlgorithm \"frontAndBackCull\" \n            -transpInShadows 0\n            -cullingOverride \"none\" \n            -lowQualityLighting 0\n            -maximumNumHardwareLights 1\n            -occlusionCulling 0\n            -shadingModel 0\n            -useBaseRenderer 0\n            -useReducedRenderer 0\n            -smallObjectCulling 0\n            -smallObjectThreshold -1 \n            -interactiveDisableShadows 0\n            -interactiveBackFaceCull 0\n            -sortTransparent 1\n            -nurbsCurves 1\n            -nurbsSurfaces 1\n"
		+ "            -polymeshes 1\n            -subdivSurfaces 1\n            -planes 1\n            -lights 1\n            -cameras 1\n            -controlVertices 1\n            -hulls 1\n            -grid 1\n            -imagePlane 1\n            -joints 1\n            -ikHandles 1\n            -deformers 1\n            -dynamics 1\n            -particleInstancers 1\n            -fluids 1\n            -hairSystems 1\n            -follicles 1\n            -nCloths 1\n            -nParticles 1\n            -nRigids 1\n            -dynamicConstraints 1\n            -locators 1\n            -manipulators 1\n            -pluginShapes 1\n            -dimensions 1\n            -handles 1\n            -pivots 1\n            -textures 1\n            -strokes 1\n            -motionTrails 1\n            -clipGhosts 1\n            -greasePencils 1\n            -shadows 0\n            -captureSequenceNumber -1\n            -width 1\n            -height 1\n            -sceneRenderFilter 0\n            $editorName;\n        modelEditor -e -viewSelected 0 $editorName;\n"
		+ "        modelEditor -e \n            -pluginObjects \"gpuCacheDisplayFilter\" 1 \n            $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextPanel \"modelPanel\" (localizedPanelLabel(\"Persp View\")) `;\n\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tmodelPanel -edit -l (localizedPanelLabel(\"Persp View\")) -mbv $menusOkayInPanels  $panelName;\n\t\t$editorName = $panelName;\n        modelEditor -e \n            -camera \"persp\" \n            -useInteractiveMode 0\n            -displayLights \"default\" \n            -displayAppearance \"wireframe\" \n            -activeOnly 0\n            -ignorePanZoom 0\n            -wireframeOnShaded 0\n            -headsUpDisplay 1\n            -holdOuts 1\n            -selectionHiliteDisplay 1\n            -useDefaultMaterial 0\n            -bufferMode \"double\" \n            -twoSidedLighting 1\n            -backfaceCulling 0\n            -xray 0\n            -jointXray 0\n            -activeComponentsXray 0\n            -displayTextures 0\n"
		+ "            -smoothWireframe 0\n            -lineWidth 1\n            -textureAnisotropic 0\n            -textureHilight 1\n            -textureSampling 2\n            -textureDisplay \"modulate\" \n            -textureMaxSize 16384\n            -fogging 0\n            -fogSource \"fragment\" \n            -fogMode \"linear\" \n            -fogStart 0\n            -fogEnd 100\n            -fogDensity 0.1\n            -fogColor 0.5 0.5 0.5 1 \n            -depthOfFieldPreview 1\n            -maxConstantTransparency 1\n            -rendererName \"base_OpenGL_Renderer\" \n            -objectFilterShowInHUD 1\n            -isFiltered 0\n            -colorResolution 256 256 \n            -bumpResolution 512 512 \n            -textureCompression 0\n            -transparencyAlgorithm \"frontAndBackCull\" \n            -transpInShadows 0\n            -cullingOverride \"none\" \n            -lowQualityLighting 0\n            -maximumNumHardwareLights 1\n            -occlusionCulling 0\n            -shadingModel 0\n            -useBaseRenderer 0\n            -useReducedRenderer 0\n"
		+ "            -smallObjectCulling 0\n            -smallObjectThreshold -1 \n            -interactiveDisableShadows 0\n            -interactiveBackFaceCull 0\n            -sortTransparent 1\n            -nurbsCurves 1\n            -nurbsSurfaces 1\n            -polymeshes 1\n            -subdivSurfaces 1\n            -planes 1\n            -lights 1\n            -cameras 1\n            -controlVertices 1\n            -hulls 1\n            -grid 1\n            -imagePlane 1\n            -joints 1\n            -ikHandles 1\n            -deformers 1\n            -dynamics 1\n            -particleInstancers 1\n            -fluids 1\n            -hairSystems 1\n            -follicles 1\n            -nCloths 1\n            -nParticles 1\n            -nRigids 1\n            -dynamicConstraints 1\n            -locators 1\n            -manipulators 1\n            -pluginShapes 1\n            -dimensions 1\n            -handles 1\n            -pivots 1\n            -textures 1\n            -strokes 1\n            -motionTrails 1\n            -clipGhosts 1\n            -greasePencils 1\n"
		+ "            -shadows 0\n            -captureSequenceNumber -1\n            -width 765\n            -height 536\n            -sceneRenderFilter 0\n            $editorName;\n        modelEditor -e -viewSelected 0 $editorName;\n        modelEditor -e \n            -pluginObjects \"gpuCacheDisplayFilter\" 1 \n            $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextPanel \"outlinerPanel\" (localizedPanelLabel(\"Outliner\")) `;\n\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\toutlinerPanel -edit -l (localizedPanelLabel(\"Outliner\")) -mbv $menusOkayInPanels  $panelName;\n\t\t$editorName = $panelName;\n        outlinerEditor -e \n            -docTag \"isolOutln_fromSeln\" \n            -showShapes 1\n            -showAssignedMaterials 0\n            -showTimeEditor 1\n            -showReferenceNodes 0\n            -showReferenceMembers 0\n            -showAttributes 0\n            -showConnected 0\n            -showAnimCurvesOnly 0\n            -showMuteInfo 0\n"
		+ "            -organizeByLayer 1\n            -showAnimLayerWeight 1\n            -autoExpandLayers 1\n            -autoExpand 0\n            -showDagOnly 0\n            -showAssets 1\n            -showContainedOnly 1\n            -showPublishedAsConnected 0\n            -showContainerContents 1\n            -ignoreDagHierarchy 0\n            -expandConnections 0\n            -showUpstreamCurves 1\n            -showUnitlessCurves 1\n            -showCompounds 1\n            -showLeafs 1\n            -showNumericAttrsOnly 0\n            -highlightActive 1\n            -autoSelectNewObjects 0\n            -doNotSelectNewObjects 0\n            -dropIsParent 1\n            -transmitFilters 0\n            -setFilter \"defaultSetFilter\" \n            -showSetMembers 1\n            -allowMultiSelection 1\n            -alwaysToggleSelect 0\n            -directSelect 0\n            -isSet 0\n            -isSetMember 0\n            -displayMode \"DAG\" \n            -expandObjects 0\n            -setsIgnoreFilters 1\n            -containersIgnoreFilters 0\n"
		+ "            -editAttrName 0\n            -showAttrValues 0\n            -highlightSecondary 0\n            -showUVAttrsOnly 0\n            -showTextureNodesOnly 0\n            -attrAlphaOrder \"default\" \n            -animLayerFilterOptions \"allAffecting\" \n            -sortOrder \"none\" \n            -longNames 0\n            -niceNames 1\n            -showNamespace 1\n            -showPinIcons 0\n            -mapMotionTrails 0\n            -ignoreHiddenAttribute 0\n            -ignoreOutlinerColor 0\n            -renderFilterVisible 0\n            -renderFilterIndex 0\n            -selectionOrder \"chronological\" \n            -expandAttribute 0\n            $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextPanel \"outlinerPanel\" (localizedPanelLabel(\"Outliner\")) `;\n\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\toutlinerPanel -edit -l (localizedPanelLabel(\"Outliner\")) -mbv $menusOkayInPanels  $panelName;\n\t\t$editorName = $panelName;\n        outlinerEditor -e \n"
		+ "            -docTag \"isolOutln_fromSeln\" \n            -showShapes 1\n            -showAssignedMaterials 0\n            -showTimeEditor 1\n            -showReferenceNodes 0\n            -showReferenceMembers 0\n            -showAttributes 0\n            -showConnected 0\n            -showAnimCurvesOnly 0\n            -showMuteInfo 0\n            -organizeByLayer 1\n            -showAnimLayerWeight 1\n            -autoExpandLayers 1\n            -autoExpand 0\n            -showDagOnly 0\n            -showAssets 1\n            -showContainedOnly 1\n            -showPublishedAsConnected 0\n            -showContainerContents 1\n            -ignoreDagHierarchy 0\n            -expandConnections 0\n            -showUpstreamCurves 1\n            -showUnitlessCurves 1\n            -showCompounds 1\n            -showLeafs 1\n            -showNumericAttrsOnly 0\n            -highlightActive 1\n            -autoSelectNewObjects 0\n            -doNotSelectNewObjects 0\n            -dropIsParent 1\n            -transmitFilters 0\n            -setFilter \"defaultSetFilter\" \n"
		+ "            -showSetMembers 1\n            -allowMultiSelection 1\n            -alwaysToggleSelect 0\n            -directSelect 0\n            -displayMode \"DAG\" \n            -expandObjects 0\n            -setsIgnoreFilters 1\n            -containersIgnoreFilters 0\n            -editAttrName 0\n            -showAttrValues 0\n            -highlightSecondary 0\n            -showUVAttrsOnly 0\n            -showTextureNodesOnly 0\n            -attrAlphaOrder \"default\" \n            -animLayerFilterOptions \"allAffecting\" \n            -sortOrder \"none\" \n            -longNames 0\n            -niceNames 1\n            -showNamespace 1\n            -showPinIcons 0\n            -mapMotionTrails 0\n            -ignoreHiddenAttribute 0\n            -ignoreOutlinerColor 0\n            -renderFilterVisible 0\n            $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"graphEditor\" (localizedPanelLabel(\"Graph Editor\")) `;\n\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n"
		+ "\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Graph Editor\")) -mbv $menusOkayInPanels  $panelName;\n\n\t\t\t$editorName = ($panelName+\"OutlineEd\");\n            outlinerEditor -e \n                -showShapes 1\n                -showAssignedMaterials 0\n                -showTimeEditor 1\n                -showReferenceNodes 0\n                -showReferenceMembers 0\n                -showAttributes 1\n                -showConnected 1\n                -showAnimCurvesOnly 1\n                -showMuteInfo 0\n                -organizeByLayer 1\n                -showAnimLayerWeight 1\n                -autoExpandLayers 1\n                -autoExpand 1\n                -showDagOnly 0\n                -showAssets 1\n                -showContainedOnly 0\n                -showPublishedAsConnected 0\n                -showContainerContents 0\n                -ignoreDagHierarchy 0\n                -expandConnections 1\n                -showUpstreamCurves 1\n                -showUnitlessCurves 1\n                -showCompounds 0\n                -showLeafs 1\n"
		+ "                -showNumericAttrsOnly 1\n                -highlightActive 0\n                -autoSelectNewObjects 1\n                -doNotSelectNewObjects 0\n                -dropIsParent 1\n                -transmitFilters 1\n                -setFilter \"0\" \n                -showSetMembers 0\n                -allowMultiSelection 1\n                -alwaysToggleSelect 0\n                -directSelect 0\n                -displayMode \"DAG\" \n                -expandObjects 0\n                -setsIgnoreFilters 1\n                -containersIgnoreFilters 0\n                -editAttrName 0\n                -showAttrValues 0\n                -highlightSecondary 0\n                -showUVAttrsOnly 0\n                -showTextureNodesOnly 0\n                -attrAlphaOrder \"default\" \n                -animLayerFilterOptions \"allAffecting\" \n                -sortOrder \"none\" \n                -longNames 0\n                -niceNames 1\n                -showNamespace 1\n                -showPinIcons 1\n                -mapMotionTrails 1\n                -ignoreHiddenAttribute 0\n"
		+ "                -ignoreOutlinerColor 0\n                -renderFilterVisible 0\n                $editorName;\n\n\t\t\t$editorName = ($panelName+\"GraphEd\");\n            animCurveEditor -e \n                -displayKeys 1\n                -displayTangents 0\n                -displayActiveKeys 0\n                -displayActiveKeyTangents 1\n                -displayInfinities 0\n                -displayValues 0\n                -autoFit 1\n                -snapTime \"integer\" \n                -snapValue \"none\" \n                -showResults \"off\" \n                -showBufferCurves \"off\" \n                -smoothness \"fine\" \n                -resultSamples 1\n                -resultScreenSamples 0\n                -resultUpdate \"delayed\" \n                -showUpstreamCurves 1\n                -showCurveNames 0\n                -showActiveCurveNames 0\n                -stackedCurves 0\n                -stackedCurvesMin -1\n                -stackedCurvesMax 1\n                -stackedCurvesSpace 0.2\n                -displayNormalized 0\n                -preSelectionHighlight 0\n"
		+ "                -constrainDrag 0\n                -classicMode 1\n                -valueLinesToggle 1\n                $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"dopeSheetPanel\" (localizedPanelLabel(\"Dope Sheet\")) `;\n\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Dope Sheet\")) -mbv $menusOkayInPanels  $panelName;\n\n\t\t\t$editorName = ($panelName+\"OutlineEd\");\n            outlinerEditor -e \n                -showShapes 1\n                -showAssignedMaterials 0\n                -showTimeEditor 1\n                -showReferenceNodes 0\n                -showReferenceMembers 0\n                -showAttributes 1\n                -showConnected 1\n                -showAnimCurvesOnly 1\n                -showMuteInfo 0\n                -organizeByLayer 1\n                -showAnimLayerWeight 1\n                -autoExpandLayers 1\n                -autoExpand 0\n                -showDagOnly 0\n"
		+ "                -showAssets 1\n                -showContainedOnly 0\n                -showPublishedAsConnected 0\n                -showContainerContents 0\n                -ignoreDagHierarchy 0\n                -expandConnections 1\n                -showUpstreamCurves 1\n                -showUnitlessCurves 0\n                -showCompounds 1\n                -showLeafs 1\n                -showNumericAttrsOnly 1\n                -highlightActive 0\n                -autoSelectNewObjects 0\n                -doNotSelectNewObjects 1\n                -dropIsParent 1\n                -transmitFilters 0\n                -setFilter \"0\" \n                -showSetMembers 0\n                -allowMultiSelection 1\n                -alwaysToggleSelect 0\n                -directSelect 0\n                -displayMode \"DAG\" \n                -expandObjects 0\n                -setsIgnoreFilters 1\n                -containersIgnoreFilters 0\n                -editAttrName 0\n                -showAttrValues 0\n                -highlightSecondary 0\n                -showUVAttrsOnly 0\n"
		+ "                -showTextureNodesOnly 0\n                -attrAlphaOrder \"default\" \n                -animLayerFilterOptions \"allAffecting\" \n                -sortOrder \"none\" \n                -longNames 0\n                -niceNames 1\n                -showNamespace 1\n                -showPinIcons 0\n                -mapMotionTrails 1\n                -ignoreHiddenAttribute 0\n                -ignoreOutlinerColor 0\n                -renderFilterVisible 0\n                $editorName;\n\n\t\t\t$editorName = ($panelName+\"DopeSheetEd\");\n            dopeSheetEditor -e \n                -displayKeys 1\n                -displayTangents 0\n                -displayActiveKeys 0\n                -displayActiveKeyTangents 0\n                -displayInfinities 0\n                -displayValues 0\n                -autoFit 0\n                -snapTime \"integer\" \n                -snapValue \"none\" \n                -outliner \"dopeSheetPanel1OutlineEd\" \n                -showSummary 1\n                -showScene 0\n                -hierarchyBelow 0\n                -showTicks 1\n"
		+ "                -selectionWindow 0 0 0 0 \n                $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"timeEditorPanel\" (localizedPanelLabel(\"Time Editor\")) `;\n\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Time Editor\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"clipEditorPanel\" (localizedPanelLabel(\"Trax Editor\")) `;\n\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Trax Editor\")) -mbv $menusOkayInPanels  $panelName;\n\n\t\t\t$editorName = clipEditorNameFromPanel($panelName);\n            clipEditor -e \n                -displayKeys 0\n                -displayTangents 0\n                -displayActiveKeys 0\n                -displayActiveKeyTangents 0\n                -displayInfinities 0\n"
		+ "                -displayValues 0\n                -autoFit 0\n                -snapTime \"none\" \n                -snapValue \"none\" \n                -initialized 0\n                -manageSequencer 0 \n                $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"sequenceEditorPanel\" (localizedPanelLabel(\"Camera Sequencer\")) `;\n\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Camera Sequencer\")) -mbv $menusOkayInPanels  $panelName;\n\n\t\t\t$editorName = sequenceEditorNameFromPanel($panelName);\n            clipEditor -e \n                -displayKeys 0\n                -displayTangents 0\n                -displayActiveKeys 0\n                -displayActiveKeyTangents 0\n                -displayInfinities 0\n                -displayValues 0\n                -autoFit 0\n                -snapTime \"none\" \n                -snapValue \"none\" \n                -initialized 0\n                -manageSequencer 1 \n"
		+ "                $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"hyperGraphPanel\" (localizedPanelLabel(\"Hypergraph Hierarchy\")) `;\n\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Hypergraph Hierarchy\")) -mbv $menusOkayInPanels  $panelName;\n\n\t\t\t$editorName = ($panelName+\"HyperGraphEd\");\n            hyperGraph -e \n                -graphLayoutStyle \"hierarchicalLayout\" \n                -orientation \"horiz\" \n                -mergeConnections 0\n                -zoom 1\n                -animateTransition 0\n                -showRelationships 1\n                -showShapes 0\n                -showDeformers 0\n                -showExpressions 0\n                -showConstraints 0\n                -showConnectionFromSelected 0\n                -showConnectionToSelected 0\n                -showConstraintLabels 0\n                -showUnderworld 0\n                -showInvisible 0\n"
		+ "                -transitionFrames 1\n                -opaqueContainers 0\n                -freeform 0\n                -imagePosition 0 0 \n                -imageScale 1\n                -imageEnabled 0\n                -graphType \"DAG\" \n                -heatMapDisplay 0\n                -updateSelection 1\n                -updateNodeAdded 1\n                -useDrawOverrideColor 0\n                -limitGraphTraversal -1\n                -range -1 -1 \n                -iconSize \"smallIcons\" \n                -showCachedConnections 0\n                $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"visorPanel\" (localizedPanelLabel(\"Visor\")) `;\n\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Visor\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"createNodePanel\" (localizedPanelLabel(\"Create Node\")) `;\n"
		+ "\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Create Node\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"polyTexturePlacementPanel\" (localizedPanelLabel(\"UV Editor\")) `;\n\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"UV Editor\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"renderWindowPanel\" (localizedPanelLabel(\"Render View\")) `;\n\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Render View\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextPanel \"shapePanel\" (localizedPanelLabel(\"Shape Editor\")) `;\n"
		+ "\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tshapePanel -edit -l (localizedPanelLabel(\"Shape Editor\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextPanel \"posePanel\" (localizedPanelLabel(\"Pose Editor\")) `;\n\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tposePanel -edit -l (localizedPanelLabel(\"Pose Editor\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"dynRelEdPanel\" (localizedPanelLabel(\"Dynamic Relationships\")) `;\n\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Dynamic Relationships\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"relationshipPanel\" (localizedPanelLabel(\"Relationship Editor\")) `;\n"
		+ "\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Relationship Editor\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"referenceEditorPanel\" (localizedPanelLabel(\"Reference Editor\")) `;\n\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Reference Editor\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"componentEditorPanel\" (localizedPanelLabel(\"Component Editor\")) `;\n\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Component Editor\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"dynPaintScriptedPanelType\" (localizedPanelLabel(\"Paint Effects\")) `;\n"
		+ "\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Paint Effects\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"scriptEditorPanel\" (localizedPanelLabel(\"Script Editor\")) `;\n\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Script Editor\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"profilerPanel\" (localizedPanelLabel(\"Profiler Tool\")) `;\n\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Profiler Tool\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"contentBrowserPanel\" (localizedPanelLabel(\"Content Browser\")) `;\n"
		+ "\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Content Browser\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"Stereo\" (localizedPanelLabel(\"Stereo\")) `;\n\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Stereo\")) -mbv $menusOkayInPanels  $panelName;\nstring $editorName = ($panelName+\"Editor\");\n            stereoCameraView -e \n                -camera \"persp\" \n                -useInteractiveMode 0\n                -displayLights \"default\" \n                -displayAppearance \"wireframe\" \n                -activeOnly 0\n                -ignorePanZoom 0\n                -wireframeOnShaded 0\n                -headsUpDisplay 1\n                -holdOuts 1\n                -selectionHiliteDisplay 1\n                -useDefaultMaterial 0\n                -bufferMode \"double\" \n                -twoSidedLighting 1\n"
		+ "                -backfaceCulling 0\n                -xray 0\n                -jointXray 0\n                -activeComponentsXray 0\n                -displayTextures 0\n                -smoothWireframe 0\n                -lineWidth 1\n                -textureAnisotropic 0\n                -textureHilight 1\n                -textureSampling 2\n                -textureDisplay \"modulate\" \n                -textureMaxSize 16384\n                -fogging 0\n                -fogSource \"fragment\" \n                -fogMode \"linear\" \n                -fogStart 0\n                -fogEnd 100\n                -fogDensity 0.1\n                -fogColor 0.5 0.5 0.5 1 \n                -depthOfFieldPreview 1\n                -maxConstantTransparency 1\n                -objectFilterShowInHUD 1\n                -isFiltered 0\n                -colorResolution 4 4 \n                -bumpResolution 4 4 \n                -textureCompression 0\n                -transparencyAlgorithm \"frontAndBackCull\" \n                -transpInShadows 0\n                -cullingOverride \"none\" \n"
		+ "                -lowQualityLighting 0\n                -maximumNumHardwareLights 0\n                -occlusionCulling 0\n                -shadingModel 0\n                -useBaseRenderer 0\n                -useReducedRenderer 0\n                -smallObjectCulling 0\n                -smallObjectThreshold -1 \n                -interactiveDisableShadows 0\n                -interactiveBackFaceCull 0\n                -sortTransparent 1\n                -nurbsCurves 1\n                -nurbsSurfaces 1\n                -polymeshes 1\n                -subdivSurfaces 1\n                -planes 1\n                -lights 1\n                -cameras 1\n                -controlVertices 1\n                -hulls 1\n                -grid 1\n                -imagePlane 1\n                -joints 1\n                -ikHandles 1\n                -deformers 1\n                -dynamics 1\n                -particleInstancers 1\n                -fluids 1\n                -hairSystems 1\n                -follicles 1\n                -nCloths 1\n                -nParticles 1\n"
		+ "                -nRigids 1\n                -dynamicConstraints 1\n                -locators 1\n                -manipulators 1\n                -pluginShapes 1\n                -dimensions 1\n                -handles 1\n                -pivots 1\n                -textures 1\n                -strokes 1\n                -motionTrails 1\n                -clipGhosts 1\n                -greasePencils 1\n                -shadows 0\n                -captureSequenceNumber -1\n                -width 0\n                -height 0\n                -sceneRenderFilter 0\n                -displayMode \"centerEye\" \n                -viewColor 0 0 0 1 \n                -useCustomBackground 1\n                $editorName;\n            stereoCameraView -e -viewSelected 0 $editorName;\n            stereoCameraView -e \n                -pluginObjects \"gpuCacheDisplayFilter\" 1 \n                $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextPanel \"outlinerPanel\" (localizedPanelLabel(\"ToggledOutliner\")) `;\n"
		+ "\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\toutlinerPanel -edit -l (localizedPanelLabel(\"ToggledOutliner\")) -mbv $menusOkayInPanels  $panelName;\n\t\t$editorName = $panelName;\n        outlinerEditor -e \n            -showShapes 0\n            -showAssignedMaterials 0\n            -showTimeEditor 1\n            -showReferenceNodes 0\n            -showReferenceMembers 0\n            -showAttributes 0\n            -showConnected 0\n            -showAnimCurvesOnly 0\n            -showMuteInfo 0\n            -organizeByLayer 1\n            -showAnimLayerWeight 1\n            -autoExpandLayers 1\n            -autoExpand 0\n            -showDagOnly 1\n            -showAssets 1\n            -showContainedOnly 1\n            -showPublishedAsConnected 0\n            -showContainerContents 1\n            -ignoreDagHierarchy 0\n            -expandConnections 0\n            -showUpstreamCurves 1\n            -showUnitlessCurves 1\n            -showCompounds 1\n            -showLeafs 1\n            -showNumericAttrsOnly 0\n            -highlightActive 1\n"
		+ "            -autoSelectNewObjects 0\n            -doNotSelectNewObjects 0\n            -dropIsParent 1\n            -transmitFilters 0\n            -setFilter \"defaultSetFilter\" \n            -showSetMembers 1\n            -allowMultiSelection 1\n            -alwaysToggleSelect 0\n            -directSelect 0\n            -isSet 0\n            -isSetMember 0\n            -displayMode \"DAG\" \n            -expandObjects 0\n            -setsIgnoreFilters 1\n            -containersIgnoreFilters 0\n            -editAttrName 0\n            -showAttrValues 0\n            -highlightSecondary 0\n            -showUVAttrsOnly 0\n            -showTextureNodesOnly 0\n            -attrAlphaOrder \"default\" \n            -animLayerFilterOptions \"allAffecting\" \n            -sortOrder \"none\" \n            -longNames 0\n            -niceNames 1\n            -showNamespace 1\n            -showPinIcons 0\n            -mapMotionTrails 0\n            -ignoreHiddenAttribute 0\n            -ignoreOutlinerColor 0\n            -renderFilterVisible 0\n            -renderFilterIndex 0\n"
		+ "            -selectionOrder \"chronological\" \n            -expandAttribute 0\n            $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"hyperShadePanel\" (localizedPanelLabel(\"Hypershade\")) `;\n\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Hypershade\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"nodeEditorPanel\" (localizedPanelLabel(\"Node Editor\")) `;\n\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Node Editor\")) -mbv $menusOkayInPanels  $panelName;\n\n\t\t\t$editorName = ($panelName+\"NodeEditorEd\");\n            nodeEditor -e \n                -allAttributes 0\n                -allNodes 0\n                -autoSizeNodes 1\n                -consistentNameSize 1\n                -createNodeCommand \"nodeEdCreateNodeCommand\" \n"
		+ "                -defaultPinnedState 0\n                -additiveGraphingMode 0\n                -settingsChangedCallback \"nodeEdSyncControls\" \n                -traversalDepthLimit -1\n                -keyPressCommand \"nodeEdKeyPressCommand\" \n                -nodeTitleMode \"name\" \n                -gridSnap 0\n                -gridVisibility 1\n                -popupMenuScript \"nodeEdBuildPanelMenus\" \n                -showNamespace 1\n                -showShapes 1\n                -showSGShapes 0\n                -showTransforms 1\n                -useAssets 1\n                -syncedSelection 1\n                -extendToShapes 1\n                -activeTab -1\n                -editorMode \"default\" \n                $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\tif ($useSceneConfig) {\n        string $configName = `getPanel -cwl (localizedPanelLabel(\"Current Layout\"))`;\n        if (\"\" != $configName) {\n\t\t\tpanelConfiguration -edit -label (localizedPanelLabel(\"Current Layout\")) \n\t\t\t\t-userCreated false\n"
		+ "\t\t\t\t-defaultImage \"vacantCell.xP:/\"\n\t\t\t\t-image \"\"\n\t\t\t\t-sc false\n\t\t\t\t-configString \"global string $gMainPane; paneLayout -e -cn \\\"single\\\" -ps 1 100 100 $gMainPane;\"\n\t\t\t\t-removeAllPanels\n\t\t\t\t-ap false\n\t\t\t\t\t(localizedPanelLabel(\"Persp View\")) \n\t\t\t\t\t\"modelPanel\"\n"
		+ "\t\t\t\t\t\"$panelName = `modelPanel -unParent -l (localizedPanelLabel(\\\"Persp View\\\")) -mbv $menusOkayInPanels `;\\n$editorName = $panelName;\\nmodelEditor -e \\n    -cam `findStartUpCamera persp` \\n    -useInteractiveMode 0\\n    -displayLights \\\"default\\\" \\n    -displayAppearance \\\"wireframe\\\" \\n    -activeOnly 0\\n    -ignorePanZoom 0\\n    -wireframeOnShaded 0\\n    -headsUpDisplay 1\\n    -holdOuts 1\\n    -selectionHiliteDisplay 1\\n    -useDefaultMaterial 0\\n    -bufferMode \\\"double\\\" \\n    -twoSidedLighting 1\\n    -backfaceCulling 0\\n    -xray 0\\n    -jointXray 0\\n    -activeComponentsXray 0\\n    -displayTextures 0\\n    -smoothWireframe 0\\n    -lineWidth 1\\n    -textureAnisotropic 0\\n    -textureHilight 1\\n    -textureSampling 2\\n    -textureDisplay \\\"modulate\\\" \\n    -textureMaxSize 16384\\n    -fogging 0\\n    -fogSource \\\"fragment\\\" \\n    -fogMode \\\"linear\\\" \\n    -fogStart 0\\n    -fogEnd 100\\n    -fogDensity 0.1\\n    -fogColor 0.5 0.5 0.5 1 \\n    -depthOfFieldPreview 1\\n    -maxConstantTransparency 1\\n    -rendererName \\\"base_OpenGL_Renderer\\\" \\n    -objectFilterShowInHUD 1\\n    -isFiltered 0\\n    -colorResolution 256 256 \\n    -bumpResolution 512 512 \\n    -textureCompression 0\\n    -transparencyAlgorithm \\\"frontAndBackCull\\\" \\n    -transpInShadows 0\\n    -cullingOverride \\\"none\\\" \\n    -lowQualityLighting 0\\n    -maximumNumHardwareLights 1\\n    -occlusionCulling 0\\n    -shadingModel 0\\n    -useBaseRenderer 0\\n    -useReducedRenderer 0\\n    -smallObjectCulling 0\\n    -smallObjectThreshold -1 \\n    -interactiveDisableShadows 0\\n    -interactiveBackFaceCull 0\\n    -sortTransparent 1\\n    -nurbsCurves 1\\n    -nurbsSurfaces 1\\n    -polymeshes 1\\n    -subdivSurfaces 1\\n    -planes 1\\n    -lights 1\\n    -cameras 1\\n    -controlVertices 1\\n    -hulls 1\\n    -grid 1\\n    -imagePlane 1\\n    -joints 1\\n    -ikHandles 1\\n    -deformers 1\\n    -dynamics 1\\n    -particleInstancers 1\\n    -fluids 1\\n    -hairSystems 1\\n    -follicles 1\\n    -nCloths 1\\n    -nParticles 1\\n    -nRigids 1\\n    -dynamicConstraints 1\\n    -locators 1\\n    -manipulators 1\\n    -pluginShapes 1\\n    -dimensions 1\\n    -handles 1\\n    -pivots 1\\n    -textures 1\\n    -strokes 1\\n    -motionTrails 1\\n    -clipGhosts 1\\n    -greasePencils 1\\n    -shadows 0\\n    -captureSequenceNumber -1\\n    -width 765\\n    -height 536\\n    -sceneRenderFilter 0\\n    $editorName;\\nmodelEditor -e -viewSelected 0 $editorName;\\nmodelEditor -e \\n    -pluginObjects \\\"gpuCacheDisplayFilter\\\" 1 \\n    $editorName\"\n"
		+ "\t\t\t\t\t\"modelPanel -edit -l (localizedPanelLabel(\\\"Persp View\\\")) -mbv $menusOkayInPanels  $panelName;\\n$editorName = $panelName;\\nmodelEditor -e \\n    -cam `findStartUpCamera persp` \\n    -useInteractiveMode 0\\n    -displayLights \\\"default\\\" \\n    -displayAppearance \\\"wireframe\\\" \\n    -activeOnly 0\\n    -ignorePanZoom 0\\n    -wireframeOnShaded 0\\n    -headsUpDisplay 1\\n    -holdOuts 1\\n    -selectionHiliteDisplay 1\\n    -useDefaultMaterial 0\\n    -bufferMode \\\"double\\\" \\n    -twoSidedLighting 1\\n    -backfaceCulling 0\\n    -xray 0\\n    -jointXray 0\\n    -activeComponentsXray 0\\n    -displayTextures 0\\n    -smoothWireframe 0\\n    -lineWidth 1\\n    -textureAnisotropic 0\\n    -textureHilight 1\\n    -textureSampling 2\\n    -textureDisplay \\\"modulate\\\" \\n    -textureMaxSize 16384\\n    -fogging 0\\n    -fogSource \\\"fragment\\\" \\n    -fogMode \\\"linear\\\" \\n    -fogStart 0\\n    -fogEnd 100\\n    -fogDensity 0.1\\n    -fogColor 0.5 0.5 0.5 1 \\n    -depthOfFieldPreview 1\\n    -maxConstantTransparency 1\\n    -rendererName \\\"base_OpenGL_Renderer\\\" \\n    -objectFilterShowInHUD 1\\n    -isFiltered 0\\n    -colorResolution 256 256 \\n    -bumpResolution 512 512 \\n    -textureCompression 0\\n    -transparencyAlgorithm \\\"frontAndBackCull\\\" \\n    -transpInShadows 0\\n    -cullingOverride \\\"none\\\" \\n    -lowQualityLighting 0\\n    -maximumNumHardwareLights 1\\n    -occlusionCulling 0\\n    -shadingModel 0\\n    -useBaseRenderer 0\\n    -useReducedRenderer 0\\n    -smallObjectCulling 0\\n    -smallObjectThreshold -1 \\n    -interactiveDisableShadows 0\\n    -interactiveBackFaceCull 0\\n    -sortTransparent 1\\n    -nurbsCurves 1\\n    -nurbsSurfaces 1\\n    -polymeshes 1\\n    -subdivSurfaces 1\\n    -planes 1\\n    -lights 1\\n    -cameras 1\\n    -controlVertices 1\\n    -hulls 1\\n    -grid 1\\n    -imagePlane 1\\n    -joints 1\\n    -ikHandles 1\\n    -deformers 1\\n    -dynamics 1\\n    -particleInstancers 1\\n    -fluids 1\\n    -hairSystems 1\\n    -follicles 1\\n    -nCloths 1\\n    -nParticles 1\\n    -nRigids 1\\n    -dynamicConstraints 1\\n    -locators 1\\n    -manipulators 1\\n    -pluginShapes 1\\n    -dimensions 1\\n    -handles 1\\n    -pivots 1\\n    -textures 1\\n    -strokes 1\\n    -motionTrails 1\\n    -clipGhosts 1\\n    -greasePencils 1\\n    -shadows 0\\n    -captureSequenceNumber -1\\n    -width 765\\n    -height 536\\n    -sceneRenderFilter 0\\n    $editorName;\\nmodelEditor -e -viewSelected 0 $editorName;\\nmodelEditor -e \\n    -pluginObjects \\\"gpuCacheDisplayFilter\\\" 1 \\n    $editorName\"\n"
		+ "\t\t\t\t$configName;\n\n            setNamedPanelLayout (localizedPanelLabel(\"Current Layout\"));\n        }\n\n        panelHistory -e -clear mainPanelHistory;\n        sceneUIReplacement -clear;\n\t}\n\n\ngrid -spacing 5 -size 12 -divisions 5 -displayAxes yes -displayGridLines yes -displayDivisionLines yes -displayPerspectiveLabels no -displayOrthographicLabels no -displayAxesBold yes -perspectiveLabelPosition axis -orthographicLabelPosition edge;\nviewManip -drawCompass 0 -compassAngle 0 -frontParameters \"\" -homeParameters \"\" -selectionLockParameters \"\";\n}\n");
	setAttr ".st" 3;
createNode nodeGraphEditorInfo -n "hyperShadePrimaryNodeEditorSavedTabsInfo";
	rename -uid "B2E9BDFB-44CB-B631-62A7-048BE124F984";
	setAttr ".tgi[0].tn" -type "string" "Untitled_1";
	setAttr ".tgi[0].vl" -type "double2" -327.38093937200267 -349.99998609225008 ;
	setAttr ".tgi[0].vh" -type "double2" 316.66665408346444 307.14284493809708 ;
createNode aiStandardSurface -n "aiStandard1";
	rename -uid "EBA31742-4009-ED09-04FC-86A96DBAA746";
	setAttr ".base" 1;
	setAttr ".specular" 0;
	setAttr ".specular_roughness" 0.46690469980239868;
	setAttr ".specular_IOR" 10;
	setAttr ".subsurface_radius" -type "float3" 0.1 0.1 0.1 ;
	setAttr ".coat_roughness" 0;
	setAttr ".coat_IOR" 10;
createNode aiStandardSurface -n "aiStandard2";
	rename -uid "1E9B5478-4D15-BD96-9782-089509451004";
	setAttr ".base" 1;
	setAttr ".specular" 0;
	setAttr ".specular_roughness" 0.46690469980239868;
	setAttr ".specular_IOR" 10;
	setAttr ".subsurface_radius" -type "float3" 0.1 0.1 0.1 ;
	setAttr ".coat_roughness" 0;
	setAttr ".coat_IOR" 10;
createNode aiStandardSurface -n "aiStandard3";
	rename -uid "F585F2C9-426A-4D6F-D843-B68AC61600F9";
	setAttr ".base" 1;
	setAttr ".specular" 0;
	setAttr ".specular_roughness" 0.46690469980239868;
	setAttr ".specular_IOR" 10;
	setAttr ".subsurface_radius" -type "float3" 0.1 0.1 0.1 ;
	setAttr ".coat_roughness" 0;
	setAttr ".coat_IOR" 10;
createNode aiStandardSurface -n "aiStandard4";
	rename -uid "F6985C25-4403-AE9F-33B5-A2935DA6D140";
	setAttr ".base" 1;
	setAttr ".specular" 0;
	setAttr ".specular_roughness" 0.46690469980239868;
	setAttr ".specular_IOR" 10;
	setAttr ".subsurface_radius" -type "float3" 0.1 0.1 0.1 ;
	setAttr ".coat_roughness" 0;
	setAttr ".coat_IOR" 10;
createNode aiStandardSurface -n "aiStandard5";
	rename -uid "A410682F-4D3F-DE71-D549-73863EE3384E";
	setAttr ".base" 1;
	setAttr ".specular" 0;
	setAttr ".specular_roughness" 0.46690469980239868;
	setAttr ".specular_IOR" 10;
	setAttr ".subsurface_radius" -type "float3" 0.1 0.1 0.1 ;
	setAttr ".coat_roughness" 0;
	setAttr ".coat_IOR" 10;
createNode aiStandardSurface -n "pasted__aiStandard1";
	rename -uid "AEAAF092-41C6-FB20-C2CA-B3B76A3CD49F";
	setAttr ".base" 1;
	setAttr ".specular" 0;
	setAttr ".specular_roughness" 0.46690469980239868;
	setAttr ".specular_IOR" 10;
	setAttr ".subsurface_radius" -type "float3" 0.1 0.1 0.1 ;
	setAttr ".coat_roughness" 0;
	setAttr ".coat_IOR" 10;
createNode aiStandardSurface -n "pasted__aiStandard4";
	rename -uid "69802532-4DEA-36F4-FD4D-7E92EFAFB49A";
	setAttr ".base" 1;
	setAttr ".specular" 0;
	setAttr ".specular_roughness" 0.46690469980239868;
	setAttr ".specular_IOR" 10;
	setAttr ".subsurface_radius" -type "float3" 0.1 0.1 0.1 ;
	setAttr ".coat_roughness" 0;
	setAttr ".coat_IOR" 10;
createNode aiStandardSurface -n "pasted__aiStandard5";
	rename -uid "4A64BBEC-4391-DE30-DED4-D099EAD608A9";
	setAttr ".base" 1;
	setAttr ".specular" 0;
	setAttr ".specular_roughness" 0.46690469980239868;
	setAttr ".specular_IOR" 10;
	setAttr ".subsurface_radius" -type "float3" 0.1 0.1 0.1 ;
	setAttr ".coat_roughness" 0;
	setAttr ".coat_IOR" 10;
createNode aiStandardSurface -n "pasted__aiStandard6";
	rename -uid "61BEDD3E-4D7E-FC55-10D6-E9A92F4EDAF1";
	setAttr ".base" 1;
	setAttr ".specular" 0;
	setAttr ".specular_roughness" 0.46690469980239868;
	setAttr ".specular_IOR" 10;
	setAttr ".subsurface_radius" -type "float3" 0.1 0.1 0.1 ;
	setAttr ".coat_roughness" 0;
	setAttr ".coat_IOR" 10;
select -ne :time1;
	setAttr -av -k on ".cch";
	setAttr -cb on ".ihi";
	setAttr -k on ".nds";
	setAttr -cb on ".bnm";
	setAttr -k on ".o" 1;
	setAttr ".unw" 1;
select -ne :hardwareRenderingGlobals;
	setAttr ".vac" 2;
	setAttr ".fprt" yes;
select -ne :renderPartition;
	setAttr -k on ".cch";
	setAttr -cb on ".ihi";
	setAttr -k on ".nds";
	setAttr -cb on ".bnm";
	setAttr -s 11 ".st";
	setAttr -cb on ".an";
	setAttr -cb on ".pt";
select -ne :renderGlobalsList1;
	setAttr -k on ".cch";
	setAttr -cb on ".ihi";
	setAttr -k on ".nds";
	setAttr -cb on ".bnm";
select -ne :defaultShaderList1;
	setAttr -k on ".cch";
	setAttr -cb on ".ihi";
	setAttr -k on ".nds";
	setAttr -cb on ".bnm";
	setAttr -s 13 ".s";
select -ne :postProcessList1;
	setAttr -k on ".cch";
	setAttr -cb on ".ihi";
	setAttr -k on ".nds";
	setAttr -cb on ".bnm";
	setAttr -s 2 ".p";
select -ne :defaultRenderUtilityList1;
	setAttr -s 9 ".u";
select -ne :defaultRenderingList1;
select -ne :lightList1;
select -ne :defaultTextureList1;
	setAttr -s 9 ".tx";
select -ne :initialShadingGroup;
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
select -ne :initialParticleSE;
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
	setAttr ".ep" 1;
	setAttr -k on ".fec";
	setAttr -k on ".ofc";
	setAttr -cb on ".ofe";
	setAttr -cb on ".efe";
	setAttr -k on ".oft";
	setAttr -cb on ".umfn";
	setAttr -cb on ".ufe";
	setAttr -cb on ".peie";
	setAttr -cb on ".ifp" -type "string" "testrender";
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
	addAttr -ci true -k true -sn "ai_user_options" -ln "aiUserOptions" -dt "string";
	setAttr ".ro" yes;
select -ne :hardwareRenderGlobals;
	setAttr ".ctrs" 256;
	setAttr ".btrs" 512;
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
connectAttr ":defaultArnoldDisplayDriver.msg" ":defaultArnoldRenderOptions.drivers"
		 -na;
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
connectAttr ":defaultColorMgtGlobals.cme" "file_udim.cme";
connectAttr ":defaultColorMgtGlobals.cfe" "file_udim.cmcf";
connectAttr ":defaultColorMgtGlobals.cfp" "file_udim.cmcp";
connectAttr ":defaultColorMgtGlobals.wsn" "file_udim.ws";
connectAttr "polyPlane1.out" "polyTweakUV1.ip";
connectAttr "polyTweakUV1.out" "polyMapCut1.ip";
connectAttr "polyMapCut1.out" "polyMapCut2.ip";
connectAttr "polyMapCut2.out" "polyMapCut3.ip";
connectAttr "polyMapCut3.out" "polyTweakUV2.ip";
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
connectAttr ":defaultColorMgtGlobals.cme" "file_tile.cme";
connectAttr ":defaultColorMgtGlobals.cfe" "file_tile.cmcf";
connectAttr ":defaultColorMgtGlobals.cfp" "file_tile.cmcp";
connectAttr ":defaultColorMgtGlobals.wsn" "file_tile.ws";
connectAttr "pasted__aiStandard1SG.msg" "pasted__materialInfo1.sg";
connectAttr "pasted__aiStandard1.msg" "pasted__materialInfo1.m";
connectAttr "pasted__aiStandard1.msg" "pasted__materialInfo1.t" -na;
connectAttr "pasted__aiStandard1.out" "pasted__aiStandard1SG.ss";
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
connectAttr ":defaultColorMgtGlobals.cme" "pasted__file1.cme";
connectAttr ":defaultColorMgtGlobals.cfe" "pasted__file1.cmcf";
connectAttr ":defaultColorMgtGlobals.cfp" "pasted__file1.cmcp";
connectAttr ":defaultColorMgtGlobals.wsn" "pasted__file1.ws";
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
connectAttr ":defaultColorMgtGlobals.cme" "file_attr.cme";
connectAttr ":defaultColorMgtGlobals.cfe" "file_attr.cmcf";
connectAttr ":defaultColorMgtGlobals.cfp" "file_attr.cmcp";
connectAttr ":defaultColorMgtGlobals.wsn" "file_attr.ws";
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
connectAttr ":defaultColorMgtGlobals.cme" "file_utile_vtile.cme";
connectAttr ":defaultColorMgtGlobals.cfe" "file_utile_vtile.cmcf";
connectAttr ":defaultColorMgtGlobals.cfp" "file_utile_vtile.cmcp";
connectAttr ":defaultColorMgtGlobals.wsn" "file_utile_vtile.ws";
connectAttr "pasted__aiStandard4SG.msg" "pasted__materialInfo4.sg";
connectAttr "pasted__aiStandard4.msg" "pasted__materialInfo4.m";
connectAttr "pasted__aiStandard4.msg" "pasted__materialInfo4.t" -na;
connectAttr "pasted__aiStandard4.out" "pasted__aiStandard4SG.ss";
connectAttr "pPlaneShape1002.iog" "pasted__aiStandard4SG.dsm" -na;
connectAttr "pPlaneShape1001.iog" "pasted__aiStandard4SG.dsm" -na;
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
connectAttr ":defaultColorMgtGlobals.cme" "file_shapeName.cme";
connectAttr ":defaultColorMgtGlobals.cfe" "file_shapeName.cmcf";
connectAttr ":defaultColorMgtGlobals.cfp" "file_shapeName.cmcp";
connectAttr ":defaultColorMgtGlobals.wsn" "file_shapeName.ws";
connectAttr "pasted__aiStandard4SG1.msg" "pasted__materialInfo5.sg";
connectAttr "pasted__aiStandard5.msg" "pasted__materialInfo5.m";
connectAttr "pasted__aiStandard5.msg" "pasted__materialInfo5.t" -na;
connectAttr "pasted__aiStandard5.out" "pasted__aiStandard4SG1.ss";
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
connectAttr ":defaultColorMgtGlobals.cme" "pasted__file5.cme";
connectAttr ":defaultColorMgtGlobals.cfe" "pasted__file5.cmcf";
connectAttr ":defaultColorMgtGlobals.cfp" "pasted__file5.cmcp";
connectAttr ":defaultColorMgtGlobals.wsn" "pasted__file5.ws";
connectAttr "pasted__aiStandard4SG2.msg" "pasted__materialInfo6.sg";
connectAttr "pasted__aiStandard6.msg" "pasted__materialInfo6.m";
connectAttr "pasted__aiStandard6.msg" "pasted__materialInfo6.t" -na;
connectAttr "pasted__aiStandard6.out" "pasted__aiStandard4SG2.ss";
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
connectAttr ":defaultColorMgtGlobals.cme" "pasted__file6.cme";
connectAttr ":defaultColorMgtGlobals.cfe" "pasted__file6.cmcf";
connectAttr ":defaultColorMgtGlobals.cfp" "pasted__file6.cmcp";
connectAttr ":defaultColorMgtGlobals.wsn" "pasted__file6.ws";
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
connectAttr ":defaultColorMgtGlobals.cme" "file_shapePath.cme";
connectAttr ":defaultColorMgtGlobals.cfe" "file_shapePath.cmcf";
connectAttr ":defaultColorMgtGlobals.cfp" "file_shapePath.cmcp";
connectAttr ":defaultColorMgtGlobals.wsn" "file_shapePath.ws";
connectAttr "file_udim.oc" "aiStandard1.base_color";
connectAttr "file_tile.oc" "aiStandard2.base_color";
connectAttr "file_attr.oc" "aiStandard3.base_color";
connectAttr "file_utile_vtile.oc" "aiStandard4.base_color";
connectAttr "file_shapePath.oc" "aiStandard5.base_color";
connectAttr "pasted__file1.oc" "pasted__aiStandard1.base_color";
connectAttr "file_shapeName.oc" "pasted__aiStandard4.base_color";
connectAttr "pasted__file5.oc" "pasted__aiStandard5.base_color";
connectAttr "pasted__file6.oc" "pasted__aiStandard6.base_color";
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
connectAttr "aiStandard3.msg" ":defaultShaderList1.s" -na;
connectAttr "aiStandard4.msg" ":defaultShaderList1.s" -na;
connectAttr "aiStandard5.msg" ":defaultShaderList1.s" -na;
connectAttr "pasted__aiStandard1.msg" ":defaultShaderList1.s" -na;
connectAttr "pasted__aiStandard4.msg" ":defaultShaderList1.s" -na;
connectAttr "pasted__aiStandard5.msg" ":defaultShaderList1.s" -na;
connectAttr "pasted__aiStandard6.msg" ":defaultShaderList1.s" -na;
connectAttr "place2dTexture1.msg" ":defaultRenderUtilityList1.u" -na;
connectAttr "place2dTexture2.msg" ":defaultRenderUtilityList1.u" -na;
connectAttr "pasted__place2dTexture1.msg" ":defaultRenderUtilityList1.u" -na;
connectAttr "place2dTexture3.msg" ":defaultRenderUtilityList1.u" -na;
connectAttr "place2dTexture4.msg" ":defaultRenderUtilityList1.u" -na;
connectAttr "pasted__place2dTexture4.msg" ":defaultRenderUtilityList1.u" -na;
connectAttr "pasted__place2dTexture5.msg" ":defaultRenderUtilityList1.u" -na;
connectAttr "pasted__place2dTexture6.msg" ":defaultRenderUtilityList1.u" -na;
connectAttr "place2dTexture5.msg" ":defaultRenderUtilityList1.u" -na;
connectAttr "defaultRenderLayer.msg" ":defaultRenderingList1.r" -na;
connectAttr "directionalLightShape1.ltd" ":lightList1.l" -na;
connectAttr "file_udim.msg" ":defaultTextureList1.tx" -na;
connectAttr "file_tile.msg" ":defaultTextureList1.tx" -na;
connectAttr "pasted__file1.msg" ":defaultTextureList1.tx" -na;
connectAttr "file_attr.msg" ":defaultTextureList1.tx" -na;
connectAttr "file_utile_vtile.msg" ":defaultTextureList1.tx" -na;
connectAttr "file_shapeName.msg" ":defaultTextureList1.tx" -na;
connectAttr "pasted__file5.msg" ":defaultTextureList1.tx" -na;
connectAttr "pasted__file6.msg" ":defaultTextureList1.tx" -na;
connectAttr "file_shapePath.msg" ":defaultTextureList1.tx" -na;
connectAttr "directionalLight1.iog" ":defaultLightSet.dsm" -na;
// End of test.ma

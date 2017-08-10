//Maya ASCII 2017ff05 scene
//Name: test.ma
//Last modified: Thu, Jun 29, 2017 11:12:18 PM
//Codeset: 1252
requires maya "2017ff05";
requires -nodeType "aiOptions" -nodeType "aiAOVDriver" -nodeType "aiAOVFilter" -nodeType "aiStandard"
		 -nodeType "aiHair" "mtoa" "2.0.2.wip";
requires "stereoCamera" "10.0";
currentUnit -l centimeter -a degree -t film;
fileInfo "application" "maya";
fileInfo "product" "Maya 2017";
fileInfo "version" "2017";
fileInfo "cutIdentifier" "201706020738-1017329";
fileInfo "osv" "Microsoft Windows 7 Enterprise Edition, 64-bit Windows 7 Service Pack 1 (Build 7601)\n";
createNode transform -s -n "persp";
	rename -uid "4BAB678E-4F7D-C6E9-2EF1-5C9EBC01175A";
	setAttr ".v" no;
	setAttr ".t" -type "double3" 23 12 0 ;
	setAttr ".r" -type "double3" -24.000000000000004 90 0 ;
	setAttr ".rp" -type "double3" -3.5804692544161291e-015 -7.1054273576010019e-015 
		-1.4210854715202004e-014 ;
	setAttr ".rpt" -type "double3" -8.7783779362275568e-015 1.1498476670238775e-014 
		9.4020555375448207e-016 ;
createNode camera -s -n "perspShape" -p "persp";
	rename -uid "E81BBA3A-410C-BEF4-DD42-E5B2294742ED";
	setAttr -k off ".v" no;
	setAttr ".fl" 34.999999999999979;
	setAttr ".coi" 21.832126661049116;
	setAttr ".imn" -type "string" "persp";
	setAttr ".den" -type "string" "persp_depth";
	setAttr ".man" -type "string" "persp_mask";
	setAttr ".tp" -type "double3" 3.529134343177768 5.7733093735849303e-016 2.6000673943571693 ;
	setAttr ".hc" -type "string" "viewSet -p %camera";
	setAttr ".ai_translator" -type "string" "perspective";
createNode transform -s -n "top";
	rename -uid "A7A6C2F5-4AD7-7493-6EA7-A39D7A75E861";
	setAttr ".v" no;
	setAttr ".t" -type "double3" 0 100.1 0 ;
	setAttr ".r" -type "double3" -89.999999999999986 0 0 ;
createNode camera -s -n "topShape" -p "top";
	rename -uid "80ACF8AE-4B53-A0BC-9C42-B5B56C3C47DD";
	setAttr -k off ".v" no;
	setAttr ".rnd" no;
	setAttr ".coi" 100.1;
	setAttr ".ow" 36.150126532178255;
	setAttr ".imn" -type "string" "top";
	setAttr ".den" -type "string" "top_depth";
	setAttr ".man" -type "string" "top_mask";
	setAttr ".hc" -type "string" "viewSet -t %camera";
	setAttr ".o" yes;
	setAttr ".ai_translator" -type "string" "orthographic";
createNode transform -s -n "front";
	rename -uid "1404793E-4F09-5AF4-AA6E-ABB1A4F39595";
	setAttr ".v" no;
	setAttr ".t" -type "double3" 0 0 100.1 ;
createNode camera -s -n "frontShape" -p "front";
	rename -uid "C681218D-4733-56D5-29BC-07A983AF8E6A";
	setAttr -k off ".v" no;
	setAttr ".rnd" no;
	setAttr ".coi" 100.1;
	setAttr ".ow" 144.46730469819329;
	setAttr ".imn" -type "string" "front";
	setAttr ".den" -type "string" "front_depth";
	setAttr ".man" -type "string" "front_mask";
	setAttr ".hc" -type "string" "viewSet -f %camera";
	setAttr ".o" yes;
	setAttr ".ai_translator" -type "string" "orthographic";
createNode transform -s -n "side";
	rename -uid "BB55BF38-4EAE-5E67-5684-419093785A28";
	setAttr ".v" no;
	setAttr ".t" -type "double3" 100.1 1.6871751536864319 12.910557697774426 ;
	setAttr ".r" -type "double3" 0 89.999999999999986 0 ;
createNode camera -s -n "sideShape" -p "side";
	rename -uid "7AB253DD-450A-CF41-7B16-48BEBA348E41";
	setAttr -k off ".v" no;
	setAttr ".rnd" no;
	setAttr ".coi" 100.1;
	setAttr ".ow" 51.715586232562323;
	setAttr ".imn" -type "string" "side";
	setAttr ".den" -type "string" "side_depth";
	setAttr ".man" -type "string" "side_mask";
	setAttr ".hc" -type "string" "viewSet -s %camera";
	setAttr ".o" yes;
	setAttr ".ai_translator" -type "string" "orthographic";
createNode transform -n "pSphere1";
	rename -uid "622C5280-482B-76D9-1FB0-F9A1F50A91F5";
	setAttr ".t" -type "double3" 0 2 8 ;
createNode mesh -n "pSphereShape1" -p "pSphere1";
	rename -uid "8825BFAE-4DF0-6AA1-1715-82B77CEBDCD2";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-min 0 -smx 1 -at "float";
	addAttr -ci true -k true -sn "ai_vid" -ln "aiVisibleInDiffuse" -dv 1 -min 0 -max 
		1 -at "bool";
	addAttr -ci true -k true -sn "ai_vig" -ln "aiVisibleInGlossy" -dv 1 -min 0 -max 
		1 -at "bool";
	addAttr -s false -ci true -k true -sn "ai_subdiv_dicing_camera" -ln "aiSubdivDicingCamera" 
		-at "message";
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
	setAttr ".ugsdt" no;
	setAttr ".vnm" 0;
	setAttr ".ai_translator" -type "string" "polymesh";
createNode transform -n "pSphere2";
	rename -uid "68FEAB3F-4BA1-8F3B-39DD-79B81D8B6902";
	setAttr ".t" -type "double3" 0 2 -8 ;
createNode mesh -n "pSphereShape2" -p "pSphere2";
	rename -uid "691129F6-4380-7860-F80F-92B0CC4CC818";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-min 0 -smx 1 -at "float";
	addAttr -ci true -k true -sn "ai_vid" -ln "aiVisibleInDiffuse" -dv 1 -min 0 -max 
		1 -at "bool";
	addAttr -ci true -k true -sn "ai_vig" -ln "aiVisibleInGlossy" -dv 1 -min 0 -max 
		1 -at "bool";
	addAttr -s false -ci true -k true -sn "ai_subdiv_dicing_camera" -ln "aiSubdivDicingCamera" 
		-at "message";
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
createNode transform -n "hairSystem1";
	rename -uid "6AB43C8A-4223-644E-B13E-1E8305253561";
createNode hairSystem -n "hairSystemShape1" -p "hairSystem1";
	rename -uid "4FCAC006-4353-0977-2604-F6AA1C093DAF";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	addAttr -ci true -k true -sn "ai_vid" -ln "aiVisibleInDiffuse" -dv 1 -min 0 -max 
		1 -at "bool";
	addAttr -ci true -k true -sn "ai_vig" -ln "aiVisibleInGlossy" -dv 1 -min 0 -max 
		1 -at "bool";
	addAttr -ci true -k true -sn "ai_export_hair_ids" -ln "aiExportHairIDs" -dv 1 -min 
		0 -max 1 -at "bool";
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
	setAttr ".ai_override_hair" yes;
createNode transform -n "hairSystem1Follicles";
	rename -uid "58879706-42C1-6828-A67C-F1B5877B043F";
createNode transform -n "pSphere1Follicle303" -p "hairSystem1Follicles";
	rename -uid "0B79058E-4D4B-AEF5-F205-7DA646FD6D4E";
createNode follicle -n "pSphere1FollicleShape303" -p "pSphere1Follicle303";
	rename -uid "B9A4B955-48F1-7DAA-5CF6-97921717809D";
	setAttr -k off ".v";
	setAttr ".pu" 0.03125;
	setAttr ".pv" 0.03125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve1" -p "pSphere1Follicle303";
	rename -uid "53E5C66F-4445-6BC0-B6E2-77888F87ECA9";
createNode nurbsCurve -n "curveShape1" -p "curve1";
	rename -uid "ACC5368D-4AB5-9745-68E2-449545CFCCB3";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "243ABB60-4A0D-7402-1B1E-1E8205D2A03A";
createNode follicle -n "pSphere1FollicleShape309" -p "pSphere1Follicle309";
	rename -uid "2A05D300-432D-7260-67A8-B6A0D3B9AF0E";
	setAttr -k off ".v";
	setAttr ".pu" 0.03125;
	setAttr ".pv" 0.09375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve2" -p "pSphere1Follicle309";
	rename -uid "5CDDD9DF-4B95-C85D-7591-7198B8F0AFB5";
createNode nurbsCurve -n "curveShape2" -p "curve2";
	rename -uid "C41BA98B-4C45-E500-F47B-1A9E54C5D215";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "05FCAB1D-49E3-8BA8-81DE-F7B8722B0D24";
createNode follicle -n "pSphere1FollicleShape315" -p "pSphere1Follicle315";
	rename -uid "2D196BCF-48EB-F696-0DF1-B59F1A9B9EBF";
	setAttr -k off ".v";
	setAttr ".pu" 0.03125;
	setAttr ".pv" 0.15625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve3" -p "pSphere1Follicle315";
	rename -uid "E57D8104-4415-3AA3-0C8D-D08159084F0B";
createNode nurbsCurve -n "curveShape3" -p "curve3";
	rename -uid "0CE20A93-4AF2-22CB-31F8-0BB8446BF532";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "12380B2F-44E9-743E-B82D-3E9C02818393";
createNode follicle -n "pSphere1FollicleShape322" -p "pSphere1Follicle322";
	rename -uid "940A297C-433F-E855-C5C7-BBB825058D1C";
	setAttr -k off ".v";
	setAttr ".pu" 0.03125;
	setAttr ".pv" 0.21875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve4" -p "pSphere1Follicle322";
	rename -uid "6CC962A0-42CB-0875-A2DC-B6BAB274876B";
createNode nurbsCurve -n "curveShape4" -p "curve4";
	rename -uid "BA45660F-48FF-3C59-BFE8-EE93C20C77EB";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "E75EB85F-408D-B6DF-F847-A1B29302F0B8";
createNode follicle -n "pSphere1FollicleShape328" -p "pSphere1Follicle328";
	rename -uid "2587A1A0-4246-3ADD-A8D9-47A32AB24A17";
	setAttr -k off ".v";
	setAttr ".pu" 0.03125;
	setAttr ".pv" 0.28125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve5" -p "pSphere1Follicle328";
	rename -uid "201A067F-4981-E494-3EB6-0F8351676135";
createNode nurbsCurve -n "curveShape5" -p "curve5";
	rename -uid "CDE2DCC8-4FE8-858B-C07D-C7B9550B62A6";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "D8594883-4FB5-BC9B-9B48-7D849195BCDF";
createNode follicle -n "pSphere1FollicleShape334" -p "pSphere1Follicle334";
	rename -uid "DBE53D3E-4F13-7A34-ECC1-0B87A8EF213B";
	setAttr -k off ".v";
	setAttr ".pu" 0.03125;
	setAttr ".pv" 0.34375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve6" -p "pSphere1Follicle334";
	rename -uid "4EDD16C4-4074-F77F-FFD6-8BAEC3160BAC";
createNode nurbsCurve -n "curveShape6" -p "curve6";
	rename -uid "36776FE5-47CE-3E10-60CF-F2B4503FF210";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "76ABBF9C-429D-0632-E5E1-11B573789154";
createNode follicle -n "pSphere1FollicleShape340" -p "pSphere1Follicle340";
	rename -uid "1D538014-45B6-C3C4-61AE-AE9D35E75546";
	setAttr -k off ".v";
	setAttr ".pu" 0.03125;
	setAttr ".pv" 0.40625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve7" -p "pSphere1Follicle340";
	rename -uid "F82E9B49-453B-20F5-035C-7CB0A5D490B7";
createNode nurbsCurve -n "curveShape7" -p "curve7";
	rename -uid "E27560D4-4E07-1423-830E-F1B5D3FF1A61";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "21458B35-4FE4-B484-4DED-958118AF2EDD";
createNode follicle -n "pSphere1FollicleShape346" -p "pSphere1Follicle346";
	rename -uid "85ACA788-491E-72C1-B2BA-1CA92C63CF89";
	setAttr -k off ".v";
	setAttr ".pu" 0.03125;
	setAttr ".pv" 0.46875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve8" -p "pSphere1Follicle346";
	rename -uid "0560350F-4244-615A-3C31-4692646382A3";
createNode nurbsCurve -n "curveShape8" -p "curve8";
	rename -uid "21E10CF0-4200-F6A9-2597-61AF83A00929";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "8E5A185D-4008-03D7-9AE2-E69AEAA2F099";
createNode follicle -n "pSphere1FollicleShape353" -p "pSphere1Follicle353";
	rename -uid "D4822A37-4F42-5E65-F352-47938573BC04";
	setAttr -k off ".v";
	setAttr ".pu" 0.03125;
	setAttr ".pv" 0.53125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve9" -p "pSphere1Follicle353";
	rename -uid "FBD4A8B7-4EE6-4137-E197-4C82D8A2D91F";
createNode nurbsCurve -n "curveShape9" -p "curve9";
	rename -uid "E19703DD-41D8-63D2-7C5A-C593603565FC";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "695D4CD6-4FA6-C28B-2FBD-D7819334D06A";
createNode follicle -n "pSphere1FollicleShape359" -p "pSphere1Follicle359";
	rename -uid "14462BA7-47D0-75CD-4F07-68932AAA5965";
	setAttr -k off ".v";
	setAttr ".pu" 0.03125;
	setAttr ".pv" 0.59375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve10" -p "pSphere1Follicle359";
	rename -uid "B3C70DEA-48F8-8D49-563D-C39DE2D3AEB5";
createNode nurbsCurve -n "curveShape10" -p "curve10";
	rename -uid "779D0A3A-4D04-8CE4-01A5-AA8E30640A13";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "47CD591E-4DB5-BE7D-D202-6E950B99C690";
createNode follicle -n "pSphere1FollicleShape365" -p "pSphere1Follicle365";
	rename -uid "A055CD69-4DEE-8604-9FFA-42AFF19FCB35";
	setAttr -k off ".v";
	setAttr ".pu" 0.03125;
	setAttr ".pv" 0.65625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve11" -p "pSphere1Follicle365";
	rename -uid "DC5414CC-4837-0F92-C526-69879E642041";
createNode nurbsCurve -n "curveShape11" -p "curve11";
	rename -uid "9746DB41-41D0-114A-78AF-64BC2E1D55F9";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "B69DB5A7-4860-9B35-4006-C2A3C39A12E2";
createNode follicle -n "pSphere1FollicleShape371" -p "pSphere1Follicle371";
	rename -uid "97135B72-4959-3F87-253E-318430EF4214";
	setAttr -k off ".v";
	setAttr ".pu" 0.03125;
	setAttr ".pv" 0.71875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve12" -p "pSphere1Follicle371";
	rename -uid "1E1C3019-4E03-04D1-1CF3-06ADEF483BB9";
createNode nurbsCurve -n "curveShape12" -p "curve12";
	rename -uid "31ECA3A4-46D4-DD94-9D2D-02BB778AFF48";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "37D0E919-42B8-BAA1-57E9-AD93905E32EB";
createNode follicle -n "pSphere1FollicleShape377" -p "pSphere1Follicle377";
	rename -uid "51D4250A-4343-1F05-3074-4583933B3915";
	setAttr -k off ".v";
	setAttr ".pu" 0.03125;
	setAttr ".pv" 0.78125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve13" -p "pSphere1Follicle377";
	rename -uid "C5C8F8AB-4568-9FC8-B543-E89AF15E1097";
createNode nurbsCurve -n "curveShape13" -p "curve13";
	rename -uid "2A4FA459-40FB-DFB8-65B7-7C83A7477DB6";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "FEAFDE37-43C3-23EB-B808-1BAB865C03E8";
createNode follicle -n "pSphere1FollicleShape384" -p "pSphere1Follicle384";
	rename -uid "17FB2CB0-466D-2BFA-8B29-CF9581E38530";
	setAttr -k off ".v";
	setAttr ".pu" 0.03125;
	setAttr ".pv" 0.84375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve14" -p "pSphere1Follicle384";
	rename -uid "3C2B4FBE-490A-3A80-769F-B4AB7E31FC49";
createNode nurbsCurve -n "curveShape14" -p "curve14";
	rename -uid "42944383-4EAA-E363-EC28-2A89EB6B0926";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "B344FAED-44A2-F3E9-08B3-6A8070A790FD";
createNode follicle -n "pSphere1FollicleShape390" -p "pSphere1Follicle390";
	rename -uid "5BD66E62-4B48-AF89-51F6-F1A81876B62B";
	setAttr -k off ".v";
	setAttr ".pu" 0.03125;
	setAttr ".pv" 0.90625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve15" -p "pSphere1Follicle390";
	rename -uid "EA5B2ADF-4BE5-24FD-BB81-17993FC7E479";
createNode nurbsCurve -n "curveShape15" -p "curve15";
	rename -uid "8C573EA2-4971-9519-449B-0F8AA4A00488";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "917201D0-432E-5492-DE63-0F96CB32B25A";
createNode follicle -n "pSphere1FollicleShape396" -p "pSphere1Follicle396";
	rename -uid "69B33824-43D3-3E3A-8444-5396A4FE828C";
	setAttr -k off ".v";
	setAttr ".pu" 0.03125;
	setAttr ".pv" 0.96875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve16" -p "pSphere1Follicle396";
	rename -uid "505C35D9-47E6-1DC1-772C-A0A2B393FED2";
createNode nurbsCurve -n "curveShape16" -p "curve16";
	rename -uid "C256645C-4213-A6E4-F621-D8973AFA4986";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "4F131F83-47C3-BD74-AB09-529D8153671B";
createNode follicle -n "pSphere1FollicleShape909" -p "pSphere1Follicle909";
	rename -uid "DDC754D3-4AF9-E498-AD1D-118AF5B0FC57";
	setAttr -k off ".v";
	setAttr ".pu" 0.09375;
	setAttr ".pv" 0.09375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve17" -p "pSphere1Follicle909";
	rename -uid "669B96E1-41CF-B9A1-F532-86A15D77FD68";
createNode nurbsCurve -n "curveShape17" -p "curve17";
	rename -uid "94C64463-408F-B908-92FF-978D9338854F";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "B6D0CD97-4F23-D9E3-668C-4286CD3034CC";
createNode follicle -n "pSphere1FollicleShape915" -p "pSphere1Follicle915";
	rename -uid "D4497A97-40F0-E673-C590-4BBF54E2EFBD";
	setAttr -k off ".v";
	setAttr ".pu" 0.09375;
	setAttr ".pv" 0.15625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve18" -p "pSphere1Follicle915";
	rename -uid "18516102-43A3-FD26-40B5-21BFA72FC7C6";
createNode nurbsCurve -n "curveShape18" -p "curve18";
	rename -uid "78216289-4008-4174-6957-71AECFBDE05C";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "8C78F95A-4974-CDC9-1007-C08FC0EB67C8";
createNode follicle -n "pSphere1FollicleShape922" -p "pSphere1Follicle922";
	rename -uid "370BAC66-4A73-F153-3934-54A6EEB8C988";
	setAttr -k off ".v";
	setAttr ".pu" 0.09375;
	setAttr ".pv" 0.21875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve19" -p "pSphere1Follicle922";
	rename -uid "5A074F47-465F-29C9-E1A2-41BC847DFC90";
createNode nurbsCurve -n "curveShape19" -p "curve19";
	rename -uid "873C5D52-4EF4-7947-1F5C-BF9F34BEE8AF";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "E08D761E-439C-8591-B903-DA81EBBCDCEB";
createNode follicle -n "pSphere1FollicleShape928" -p "pSphere1Follicle928";
	rename -uid "1881A8AA-4C3A-1153-2B30-009FC21BAF9B";
	setAttr -k off ".v";
	setAttr ".pu" 0.09375;
	setAttr ".pv" 0.28125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve20" -p "pSphere1Follicle928";
	rename -uid "4BFE7B12-47B8-6636-3C94-92A07A064CC9";
createNode nurbsCurve -n "curveShape20" -p "curve20";
	rename -uid "8F5903A8-4543-B35B-5AC4-C1A91CF66743";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "9805ABD1-4A7F-FC0D-C47D-CFB507F18A27";
createNode follicle -n "pSphere1FollicleShape934" -p "pSphere1Follicle934";
	rename -uid "08C54B1A-4671-0104-A7A6-81B54C2EC530";
	setAttr -k off ".v";
	setAttr ".pu" 0.09375;
	setAttr ".pv" 0.34375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve21" -p "pSphere1Follicle934";
	rename -uid "8D4061A6-406F-2207-3FB0-A5BC487E90B4";
createNode nurbsCurve -n "curveShape21" -p "curve21";
	rename -uid "F34804DD-499C-A859-0B71-F58652C2119A";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "29431D7C-4898-BEBA-5E2E-C19EDDFB6E7A";
createNode follicle -n "pSphere1FollicleShape940" -p "pSphere1Follicle940";
	rename -uid "3006CAFE-45CA-0FAF-2AA4-92B76460553F";
	setAttr -k off ".v";
	setAttr ".pu" 0.09375;
	setAttr ".pv" 0.40625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve22" -p "pSphere1Follicle940";
	rename -uid "FFE7B3F9-4BEA-F35C-ED17-E0A7B67FE53F";
createNode nurbsCurve -n "curveShape22" -p "curve22";
	rename -uid "85D08339-4281-D072-52CC-829D2A6C80BE";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "88F13903-4F63-A3A0-B0AF-CE9DB78071E8";
createNode follicle -n "pSphere1FollicleShape946" -p "pSphere1Follicle946";
	rename -uid "BE78157B-4ECA-7953-D441-1480126571EF";
	setAttr -k off ".v";
	setAttr ".pu" 0.09375;
	setAttr ".pv" 0.46875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve23" -p "pSphere1Follicle946";
	rename -uid "8C13C449-41A7-B953-F3AB-A292205B0111";
createNode nurbsCurve -n "curveShape23" -p "curve23";
	rename -uid "1C4AF0B6-4775-34BA-2E33-6B82945791FB";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "C19C3455-438E-067C-E007-FAA28CD43A84";
createNode follicle -n "pSphere1FollicleShape953" -p "pSphere1Follicle953";
	rename -uid "1EA9DA89-4F54-EAD5-AC1C-8FAB3CEC108E";
	setAttr -k off ".v";
	setAttr ".pu" 0.09375;
	setAttr ".pv" 0.53125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve24" -p "pSphere1Follicle953";
	rename -uid "430ECD4D-48CC-A619-9437-E2BAE5C5D9AD";
createNode nurbsCurve -n "curveShape24" -p "curve24";
	rename -uid "FCC22CC4-4893-226A-04DD-D49E7395F07D";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "0EE7B104-48EB-EAD8-DA03-D297050CB01E";
createNode follicle -n "pSphere1FollicleShape959" -p "pSphere1Follicle959";
	rename -uid "C1B3386C-4800-E440-45FF-A5B2977C28D0";
	setAttr -k off ".v";
	setAttr ".pu" 0.09375;
	setAttr ".pv" 0.59375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve25" -p "pSphere1Follicle959";
	rename -uid "711549AD-4EFA-735B-D8AB-359BCDF04CE7";
createNode nurbsCurve -n "curveShape25" -p "curve25";
	rename -uid "25AB327B-4207-3781-7019-75B21BE07ECA";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "27BDA761-406C-0B07-157A-60AC1F8F6D15";
createNode follicle -n "pSphere1FollicleShape965" -p "pSphere1Follicle965";
	rename -uid "74CAE17E-4644-7EF1-F34F-E39588EF9503";
	setAttr -k off ".v";
	setAttr ".pu" 0.09375;
	setAttr ".pv" 0.65625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve26" -p "pSphere1Follicle965";
	rename -uid "6EAB4438-4F00-AFA1-DA30-D2918D41BFF8";
createNode nurbsCurve -n "curveShape26" -p "curve26";
	rename -uid "A349C518-492A-5BC7-04FC-3A9F3D8ACF81";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "8946B759-4A4B-14BE-B46C-B98A69615C0C";
createNode follicle -n "pSphere1FollicleShape971" -p "pSphere1Follicle971";
	rename -uid "750ECD2C-4775-E441-D0B3-8BB79E9403CE";
	setAttr -k off ".v";
	setAttr ".pu" 0.09375;
	setAttr ".pv" 0.71875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve27" -p "pSphere1Follicle971";
	rename -uid "9AEBB955-4153-837E-B7EC-268145298DEC";
createNode nurbsCurve -n "curveShape27" -p "curve27";
	rename -uid "8BB58185-4948-9AFC-4C90-74BB00A8C798";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "B2AD2FA5-4F1F-4A7F-7BF0-3690730FA9C4";
createNode follicle -n "pSphere1FollicleShape977" -p "pSphere1Follicle977";
	rename -uid "880CCE6B-4643-C816-AF6C-348477DC2D42";
	setAttr -k off ".v";
	setAttr ".pu" 0.09375;
	setAttr ".pv" 0.78125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve28" -p "pSphere1Follicle977";
	rename -uid "93733107-491E-BFFF-E016-67920B518772";
createNode nurbsCurve -n "curveShape28" -p "curve28";
	rename -uid "2774DEBA-4456-ADE0-64ED-B688C0D95C1A";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "8359C572-4AF5-28FB-C3CA-EE80920CC0A0";
createNode follicle -n "pSphere1FollicleShape984" -p "pSphere1Follicle984";
	rename -uid "6EB8A570-4B87-DB61-DAAB-78A103F80299";
	setAttr -k off ".v";
	setAttr ".pu" 0.09375;
	setAttr ".pv" 0.84375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve29" -p "pSphere1Follicle984";
	rename -uid "D423EA9E-44A9-7AE7-8E45-39AA450CAC52";
createNode nurbsCurve -n "curveShape29" -p "curve29";
	rename -uid "EED7A017-415E-3D6D-29B9-469CDEDC406F";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "8837F9CA-4EE9-3319-6EFE-06AC57834F73";
createNode follicle -n "pSphere1FollicleShape990" -p "pSphere1Follicle990";
	rename -uid "D8F446E7-4110-2291-E59C-3C965AAAFAB6";
	setAttr -k off ".v";
	setAttr ".pu" 0.09375;
	setAttr ".pv" 0.90625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve30" -p "pSphere1Follicle990";
	rename -uid "ABB92720-45C2-78F4-6840-4EA2FEFECCA3";
createNode nurbsCurve -n "curveShape30" -p "curve30";
	rename -uid "11010A78-4D41-5AAC-601C-EBBEC98E6E5A";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "808E3F7C-437D-7048-1E74-1A9E83B35995";
createNode follicle -n "pSphere1FollicleShape1509" -p "pSphere1Follicle1509";
	rename -uid "C3CE6837-4FE2-43B9-770A-66ABD2835316";
	setAttr -k off ".v";
	setAttr ".pu" 0.15625;
	setAttr ".pv" 0.09375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve31" -p "pSphere1Follicle1509";
	rename -uid "F28F021C-427C-FCDD-442C-8F8015E16C04";
createNode nurbsCurve -n "curveShape31" -p "curve31";
	rename -uid "03B9BCB4-4585-0875-85CB-C39F26AA11EF";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "DBAD0561-46A4-7652-683E-F2B5F0C90520";
createNode follicle -n "pSphere1FollicleShape1515" -p "pSphere1Follicle1515";
	rename -uid "BBFD46A3-436F-4054-B2AE-27AC70DC9CF9";
	setAttr -k off ".v";
	setAttr ".pu" 0.15625;
	setAttr ".pv" 0.15625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve32" -p "pSphere1Follicle1515";
	rename -uid "13CEF378-47B0-729F-B118-46B4D04DFCA5";
createNode nurbsCurve -n "curveShape32" -p "curve32";
	rename -uid "8BB16F60-4D9D-C772-217A-E2BF35467A7D";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "4A315E6A-4F8F-FC04-1065-26ACC629C1D5";
createNode follicle -n "pSphere1FollicleShape1522" -p "pSphere1Follicle1522";
	rename -uid "A54BD5CF-4F42-0A10-1969-E584DEC46A04";
	setAttr -k off ".v";
	setAttr ".pu" 0.15625;
	setAttr ".pv" 0.21875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve33" -p "pSphere1Follicle1522";
	rename -uid "48949C83-4E9A-4C8F-2EAD-3B8A0D215140";
createNode nurbsCurve -n "curveShape33" -p "curve33";
	rename -uid "B8B60C6B-4A33-E071-8172-5EABE32A8842";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "EEFE597B-455B-4909-C78B-0889A9453F41";
createNode follicle -n "pSphere1FollicleShape1528" -p "pSphere1Follicle1528";
	rename -uid "71FD5CBF-4114-9908-1F9A-D799A78CF2D5";
	setAttr -k off ".v";
	setAttr ".pu" 0.15625;
	setAttr ".pv" 0.28125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve34" -p "pSphere1Follicle1528";
	rename -uid "C99E83E3-47FD-4C24-9360-63A34631FA79";
createNode nurbsCurve -n "curveShape34" -p "curve34";
	rename -uid "6024ABEC-4BB4-0376-2C96-8589A245504C";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "98476F7E-48F1-658A-9226-7B89E7035F8C";
createNode follicle -n "pSphere1FollicleShape1534" -p "pSphere1Follicle1534";
	rename -uid "015C47C5-4638-F8BA-96B0-0F8106525C46";
	setAttr -k off ".v";
	setAttr ".pu" 0.15625;
	setAttr ".pv" 0.34375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve35" -p "pSphere1Follicle1534";
	rename -uid "261E58DF-4A7B-A92A-A930-7D95BCC65F8F";
createNode nurbsCurve -n "curveShape35" -p "curve35";
	rename -uid "E3B91115-42AC-5376-5D8F-339C927EEE45";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "AC2407AD-4E82-EF4A-3BFA-78B62312F73F";
createNode follicle -n "pSphere1FollicleShape1540" -p "pSphere1Follicle1540";
	rename -uid "987A3B6B-44F8-77EF-C72F-0EB6636C4406";
	setAttr -k off ".v";
	setAttr ".pu" 0.15625;
	setAttr ".pv" 0.40625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve36" -p "pSphere1Follicle1540";
	rename -uid "A7AD9B17-42B9-1674-E754-928DD55DD0D6";
createNode nurbsCurve -n "curveShape36" -p "curve36";
	rename -uid "C1F966E0-4CA7-9A13-201E-D4B6AC1C52F0";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "91FC8AD4-4D19-7687-C78C-A685E526E66B";
createNode follicle -n "pSphere1FollicleShape1546" -p "pSphere1Follicle1546";
	rename -uid "65B601CB-406A-0D53-C997-6A8EBE1F52F2";
	setAttr -k off ".v";
	setAttr ".pu" 0.15625;
	setAttr ".pv" 0.46875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve37" -p "pSphere1Follicle1546";
	rename -uid "2C10A8ED-4438-4853-087D-7AA00D50131D";
createNode nurbsCurve -n "curveShape37" -p "curve37";
	rename -uid "DDBB6872-47A6-35AC-16D4-7D8C0A0B397E";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "71FFC24B-45B9-2CEC-53E4-F1AE4A95164E";
createNode follicle -n "pSphere1FollicleShape1553" -p "pSphere1Follicle1553";
	rename -uid "D2B70174-42E9-D47B-F906-D7904F465AA2";
	setAttr -k off ".v";
	setAttr ".pu" 0.15625;
	setAttr ".pv" 0.53125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve38" -p "pSphere1Follicle1553";
	rename -uid "C0196481-4B0A-A17E-A83D-D0835571B1D9";
createNode nurbsCurve -n "curveShape38" -p "curve38";
	rename -uid "5B447352-4F7A-4456-88CE-2D840E3AA27A";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "DA9531C9-4242-0F8C-DDFD-4A984D05EDBF";
createNode follicle -n "pSphere1FollicleShape1559" -p "pSphere1Follicle1559";
	rename -uid "AE997684-44D8-D930-78EC-5ABC9124D2C4";
	setAttr -k off ".v";
	setAttr ".pu" 0.15625;
	setAttr ".pv" 0.59375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve39" -p "pSphere1Follicle1559";
	rename -uid "C5DF43F6-4034-A7C8-17E2-E593EF8DEBEA";
createNode nurbsCurve -n "curveShape39" -p "curve39";
	rename -uid "8E39B76D-4452-89B4-6D22-00B2EF0A97F2";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "98C2FBF7-4955-79C7-8DE0-75BBB8902211";
createNode follicle -n "pSphere1FollicleShape1565" -p "pSphere1Follicle1565";
	rename -uid "BAC54529-4241-A462-A1AA-EE83F0C796C9";
	setAttr -k off ".v";
	setAttr ".pu" 0.15625;
	setAttr ".pv" 0.65625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve40" -p "pSphere1Follicle1565";
	rename -uid "0E65B705-4B27-FD22-FEAE-F4AC4AEC9CB3";
createNode nurbsCurve -n "curveShape40" -p "curve40";
	rename -uid "6673B3A2-4E5E-C3D5-9274-B1A6EF88FC57";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "AA82A3C2-477C-1970-E0BC-51844D55DBB2";
createNode follicle -n "pSphere1FollicleShape1571" -p "pSphere1Follicle1571";
	rename -uid "7DA02590-4E8D-B717-337E-37BEAC177FAC";
	setAttr -k off ".v";
	setAttr ".pu" 0.15625;
	setAttr ".pv" 0.71875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve41" -p "pSphere1Follicle1571";
	rename -uid "534894D0-40D0-07C8-7709-CB8B65FF1645";
createNode nurbsCurve -n "curveShape41" -p "curve41";
	rename -uid "AFFFD65D-4F74-4F94-B059-EBA1417C827D";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "256A08BE-4BF7-1A3A-9D54-C2BAD8281920";
createNode follicle -n "pSphere1FollicleShape1577" -p "pSphere1Follicle1577";
	rename -uid "BABC753D-4F61-B904-79F1-F4B63A3502B8";
	setAttr -k off ".v";
	setAttr ".pu" 0.15625;
	setAttr ".pv" 0.78125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve42" -p "pSphere1Follicle1577";
	rename -uid "01439099-469B-4D5D-0E3A-D6AA84F66349";
createNode nurbsCurve -n "curveShape42" -p "curve42";
	rename -uid "D5237963-4F5B-FE0D-9D02-E6BA0A1996F5";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "5A415B23-4700-EF20-A11A-00946F183AB3";
createNode follicle -n "pSphere1FollicleShape1584" -p "pSphere1Follicle1584";
	rename -uid "3BCEF99E-455B-CB90-1751-C28D045714D8";
	setAttr -k off ".v";
	setAttr ".pu" 0.15625;
	setAttr ".pv" 0.84375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve43" -p "pSphere1Follicle1584";
	rename -uid "4576EA60-4A81-6D0C-1430-1A93AD8F3C2C";
createNode nurbsCurve -n "curveShape43" -p "curve43";
	rename -uid "EC5C1A88-4813-C8CC-44EC-7D8DB9C01E8D";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "08E85413-45AD-FFD2-D2EE-988896593838";
createNode follicle -n "pSphere1FollicleShape1590" -p "pSphere1Follicle1590";
	rename -uid "B9553D15-437E-2F44-84CD-61AD464ECF47";
	setAttr -k off ".v";
	setAttr ".pu" 0.15625;
	setAttr ".pv" 0.90625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve44" -p "pSphere1Follicle1590";
	rename -uid "30E7D5B4-4FAA-B79B-21A8-6E96A36E4B68";
createNode nurbsCurve -n "curveShape44" -p "curve44";
	rename -uid "7423512C-4005-08F7-B72A-CA985B90FAA7";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "AD4D8073-4436-EF6D-A97C-95A597E68A77";
createNode follicle -n "pSphere1FollicleShape2203" -p "pSphere1Follicle2203";
	rename -uid "A4E7E15A-406A-3461-4480-0F9D26F8B55C";
	setAttr -k off ".v";
	setAttr ".pu" 0.21875;
	setAttr ".pv" 0.03125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve45" -p "pSphere1Follicle2203";
	rename -uid "19AB165A-487F-A0D2-8FBA-56A09205E89B";
createNode nurbsCurve -n "curveShape45" -p "curve45";
	rename -uid "80DB4EC9-4672-6D42-49E8-BB9E52643870";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "B3143B4B-4A57-539E-E90C-9F817B9D02B7";
createNode follicle -n "pSphere1FollicleShape2209" -p "pSphere1Follicle2209";
	rename -uid "A4125386-4C5D-8C64-593C-8D937C0F6526";
	setAttr -k off ".v";
	setAttr ".pu" 0.21875;
	setAttr ".pv" 0.09375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve46" -p "pSphere1Follicle2209";
	rename -uid "3F7A8FDB-4A86-60DB-53FB-55A11B5E5035";
createNode nurbsCurve -n "curveShape46" -p "curve46";
	rename -uid "FD190957-48AC-CBC9-D391-03AD035C903D";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "91236C8C-4926-93DE-D615-9D8CBF910005";
createNode follicle -n "pSphere1FollicleShape2215" -p "pSphere1Follicle2215";
	rename -uid "3A5EBFF5-4AA3-21DC-B5A0-418E829DE67B";
	setAttr -k off ".v";
	setAttr ".pu" 0.21875;
	setAttr ".pv" 0.15625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve47" -p "pSphere1Follicle2215";
	rename -uid "86F8D9AB-4010-D646-0CFF-E98DFE956F70";
createNode nurbsCurve -n "curveShape47" -p "curve47";
	rename -uid "3E5A9DF1-445C-EE1B-993A-5BAB1AD1C6BA";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "56675964-48DF-FACD-7BA3-CAA847121A59";
createNode follicle -n "pSphere1FollicleShape2222" -p "pSphere1Follicle2222";
	rename -uid "4D42D676-45CA-A0C7-B9BC-A2A41732455F";
	setAttr -k off ".v";
	setAttr ".pu" 0.21875;
	setAttr ".pv" 0.21875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve48" -p "pSphere1Follicle2222";
	rename -uid "EACC96B5-485D-7B8B-0801-5FBB4B4EDF9E";
createNode nurbsCurve -n "curveShape48" -p "curve48";
	rename -uid "B7060210-4EDF-78DB-8FFE-77ADC3620295";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "7E503343-497A-9A55-35BE-18907466B7F5";
createNode follicle -n "pSphere1FollicleShape2228" -p "pSphere1Follicle2228";
	rename -uid "9AECDA90-4E5E-75BF-2CD1-0A8E5BD29B04";
	setAttr -k off ".v";
	setAttr ".pu" 0.21875;
	setAttr ".pv" 0.28125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve49" -p "pSphere1Follicle2228";
	rename -uid "2AA185AA-4EB1-5373-8BE9-B6A1BFEC4EE4";
createNode nurbsCurve -n "curveShape49" -p "curve49";
	rename -uid "01ACBDA2-47F3-6383-AD1A-1D934F0A5805";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "44E01AE8-482B-775C-C7FF-F99CB6A415CD";
createNode follicle -n "pSphere1FollicleShape2234" -p "pSphere1Follicle2234";
	rename -uid "AB958886-498B-0383-43D5-348020EAB6B5";
	setAttr -k off ".v";
	setAttr ".pu" 0.21875;
	setAttr ".pv" 0.34375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve50" -p "pSphere1Follicle2234";
	rename -uid "04237716-420F-B361-44AA-0A893D06815A";
createNode nurbsCurve -n "curveShape50" -p "curve50";
	rename -uid "6A28224F-4F40-9040-D409-04B56673BBFF";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "A00FED92-4C88-555F-1D54-9EA8032E5C77";
createNode follicle -n "pSphere1FollicleShape2240" -p "pSphere1Follicle2240";
	rename -uid "F4AA1634-41B3-0EEF-7A42-C9A146326611";
	setAttr -k off ".v";
	setAttr ".pu" 0.21875;
	setAttr ".pv" 0.40625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve51" -p "pSphere1Follicle2240";
	rename -uid "37F858E5-411D-1E6D-16C2-2C85286391AD";
createNode nurbsCurve -n "curveShape51" -p "curve51";
	rename -uid "4A271C2C-45F5-B865-857C-9F84E24EFF54";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "C66BAF94-41DD-3310-35CF-3687469A18B5";
createNode follicle -n "pSphere1FollicleShape2246" -p "pSphere1Follicle2246";
	rename -uid "876476C1-410E-C271-E006-22B188CED637";
	setAttr -k off ".v";
	setAttr ".pu" 0.21875;
	setAttr ".pv" 0.46875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve52" -p "pSphere1Follicle2246";
	rename -uid "050864E5-439F-E9FF-21EC-26A60B654E01";
createNode nurbsCurve -n "curveShape52" -p "curve52";
	rename -uid "A238AD81-4DEC-DEE5-FFBC-2089CFB9AA0E";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "8276E167-49AC-A548-A089-EF8DC2F84E1E";
createNode follicle -n "pSphere1FollicleShape2253" -p "pSphere1Follicle2253";
	rename -uid "98D23ED3-4B65-8FB2-3406-4AA647FB7173";
	setAttr -k off ".v";
	setAttr ".pu" 0.21875;
	setAttr ".pv" 0.53125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve53" -p "pSphere1Follicle2253";
	rename -uid "63BB48E4-4D98-26E4-B106-21B560296999";
createNode nurbsCurve -n "curveShape53" -p "curve53";
	rename -uid "9D415DDC-48AD-A7AA-D8E0-6C8527055AC2";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "A0965EAD-4C7A-4C28-A0D1-4E91C930E895";
createNode follicle -n "pSphere1FollicleShape2259" -p "pSphere1Follicle2259";
	rename -uid "335F9ABC-4794-9AAF-627B-8A8678E5F6DC";
	setAttr -k off ".v";
	setAttr ".pu" 0.21875;
	setAttr ".pv" 0.59375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve54" -p "pSphere1Follicle2259";
	rename -uid "8D2BCEB0-467C-8948-F354-C8A95BA4A769";
createNode nurbsCurve -n "curveShape54" -p "curve54";
	rename -uid "44624F79-44DF-2FD8-8838-09A9454E563E";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "7118F4F6-4481-17DF-8843-5CB8398AB3DD";
createNode follicle -n "pSphere1FollicleShape2265" -p "pSphere1Follicle2265";
	rename -uid "A1510069-451E-7E59-58D7-C7828150404C";
	setAttr -k off ".v";
	setAttr ".pu" 0.21875;
	setAttr ".pv" 0.65625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve55" -p "pSphere1Follicle2265";
	rename -uid "DF9E465F-4899-5633-7572-BC80ECDA9375";
createNode nurbsCurve -n "curveShape55" -p "curve55";
	rename -uid "4C1706AB-445F-B6A0-9C31-AC99129B1160";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "04C97E0B-4743-C786-66A5-78BDF262D0EF";
createNode follicle -n "pSphere1FollicleShape2271" -p "pSphere1Follicle2271";
	rename -uid "71DA603E-4DDC-E4FE-2C3E-9B81961AA70A";
	setAttr -k off ".v";
	setAttr ".pu" 0.21875;
	setAttr ".pv" 0.71875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve56" -p "pSphere1Follicle2271";
	rename -uid "4586282B-48E6-E68B-1167-7099F8B9B377";
createNode nurbsCurve -n "curveShape56" -p "curve56";
	rename -uid "242EECA9-4E49-012F-12A4-F989FC06A77C";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "68BBDB25-4A1F-31C9-D624-589188810194";
createNode follicle -n "pSphere1FollicleShape2277" -p "pSphere1Follicle2277";
	rename -uid "C5623E84-466D-C566-D8FF-76869CE1C121";
	setAttr -k off ".v";
	setAttr ".pu" 0.21875;
	setAttr ".pv" 0.78125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve57" -p "pSphere1Follicle2277";
	rename -uid "FB66024E-45EA-BF68-D603-E782275ADFBD";
createNode nurbsCurve -n "curveShape57" -p "curve57";
	rename -uid "AEDB90FD-4C85-4397-5F1A-DB8A00CF4309";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "D6A37A6C-4A7B-A3FA-C815-679D34201D5C";
createNode follicle -n "pSphere1FollicleShape2284" -p "pSphere1Follicle2284";
	rename -uid "05214BD0-4318-FD12-5677-86930B77C918";
	setAttr -k off ".v";
	setAttr ".pu" 0.21875;
	setAttr ".pv" 0.84375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve58" -p "pSphere1Follicle2284";
	rename -uid "F35A5DD0-44C5-EB83-0547-C28D9793A339";
createNode nurbsCurve -n "curveShape58" -p "curve58";
	rename -uid "6B822B12-425C-2A07-CB77-9CBB8B84E5F9";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "C085A28C-4334-56B7-9316-A4B23F98DA4A";
createNode follicle -n "pSphere1FollicleShape2290" -p "pSphere1Follicle2290";
	rename -uid "F03AF863-4D18-ACB4-5B09-7E9BD5091497";
	setAttr -k off ".v";
	setAttr ".pu" 0.21875;
	setAttr ".pv" 0.90625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve59" -p "pSphere1Follicle2290";
	rename -uid "4027FC0E-44D8-B6EA-CCBF-AE8E2D8AA13A";
createNode nurbsCurve -n "curveShape59" -p "curve59";
	rename -uid "6949591B-4DB1-73C9-D918-238D7897EE2B";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "75D1BF45-4B1B-E0D4-563B-52A4E1EAA584";
createNode follicle -n "pSphere1FollicleShape2296" -p "pSphere1Follicle2296";
	rename -uid "ADE9A972-4BCA-76F9-75B5-DF832FEB2225";
	setAttr -k off ".v";
	setAttr ".pu" 0.21875;
	setAttr ".pv" 0.96875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve60" -p "pSphere1Follicle2296";
	rename -uid "6C5BEE27-4842-E96A-7A83-568302C47FCD";
createNode nurbsCurve -n "curveShape60" -p "curve60";
	rename -uid "E3D6FE8F-4021-2769-2E23-A280E1374D97";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "69DB2914-4924-70DF-99DE-F0B7D476DFA7";
createNode follicle -n "pSphere1FollicleShape2803" -p "pSphere1Follicle2803";
	rename -uid "1CB0869D-46FD-6224-1715-96902C770EFC";
	setAttr -k off ".v";
	setAttr ".pu" 0.28125;
	setAttr ".pv" 0.03125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve61" -p "pSphere1Follicle2803";
	rename -uid "2EA756C4-406C-7F0A-B703-1BB3707C54ED";
createNode nurbsCurve -n "curveShape61" -p "curve61";
	rename -uid "21933167-4142-12A0-CA90-A1B0B2DBBCB7";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "9F3E98A7-4710-E161-A40B-B0BE1A946434";
createNode follicle -n "pSphere1FollicleShape2809" -p "pSphere1Follicle2809";
	rename -uid "8E7C0C61-430A-B19C-E8C0-DE865E6F4445";
	setAttr -k off ".v";
	setAttr ".pu" 0.28125;
	setAttr ".pv" 0.09375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve62" -p "pSphere1Follicle2809";
	rename -uid "44FF7469-4327-4C9E-3893-29AF9B2B59E9";
createNode nurbsCurve -n "curveShape62" -p "curve62";
	rename -uid "494A76D8-4C48-D28A-BBD1-A58668C7FF3C";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "111599E5-4649-8EA3-C647-6E8232AA10ED";
createNode follicle -n "pSphere1FollicleShape2815" -p "pSphere1Follicle2815";
	rename -uid "BAE75B0C-477B-8193-28A0-C1B6102769ED";
	setAttr -k off ".v";
	setAttr ".pu" 0.28125;
	setAttr ".pv" 0.15625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve63" -p "pSphere1Follicle2815";
	rename -uid "C9A0A6A1-4316-EE09-F4AB-9CB3EF215E04";
createNode nurbsCurve -n "curveShape63" -p "curve63";
	rename -uid "4D61DA61-488B-C0C3-0D5E-DFB8F9C3E778";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "521AAF05-4146-B1CA-5073-7EB1C7C67845";
createNode follicle -n "pSphere1FollicleShape2822" -p "pSphere1Follicle2822";
	rename -uid "27940DA0-4704-9241-9780-779752A700BA";
	setAttr -k off ".v";
	setAttr ".pu" 0.28125;
	setAttr ".pv" 0.21875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve64" -p "pSphere1Follicle2822";
	rename -uid "79FB5750-440A-4CF1-1818-FEB14806CF30";
createNode nurbsCurve -n "curveShape64" -p "curve64";
	rename -uid "B02C610D-436D-2634-CAF5-D18A5E7596A8";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "B66B6FDF-40DD-52DA-2D98-D5B8F1DCD1A8";
createNode follicle -n "pSphere1FollicleShape2828" -p "pSphere1Follicle2828";
	rename -uid "AB035072-443F-D0CD-BFEB-95807AC92A0F";
	setAttr -k off ".v";
	setAttr ".pu" 0.28125;
	setAttr ".pv" 0.28125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve65" -p "pSphere1Follicle2828";
	rename -uid "80CACC70-4DF7-D25E-1E6F-47BC90A0520F";
createNode nurbsCurve -n "curveShape65" -p "curve65";
	rename -uid "D46CA7B0-4FD8-E59D-4375-99AF08711880";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "B4B2F6C4-4F13-6269-85B3-AAB1D61695EB";
createNode follicle -n "pSphere1FollicleShape2834" -p "pSphere1Follicle2834";
	rename -uid "919FD77A-4724-9C49-BB95-0F863DE162B6";
	setAttr -k off ".v";
	setAttr ".pu" 0.28125;
	setAttr ".pv" 0.34375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve66" -p "pSphere1Follicle2834";
	rename -uid "45FF8307-4ECC-FB56-53B0-E1B7B03B6858";
createNode nurbsCurve -n "curveShape66" -p "curve66";
	rename -uid "55424862-439E-BB7A-CE76-01A5821236E4";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "107C66AE-4E4B-6503-6616-928C02BB5EAF";
createNode follicle -n "pSphere1FollicleShape2840" -p "pSphere1Follicle2840";
	rename -uid "118D5657-4639-8794-19BD-1E9EBDBC6F7F";
	setAttr -k off ".v";
	setAttr ".pu" 0.28125;
	setAttr ".pv" 0.40625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve67" -p "pSphere1Follicle2840";
	rename -uid "080E4267-45E6-0752-E3D7-3F878DB96C4F";
createNode nurbsCurve -n "curveShape67" -p "curve67";
	rename -uid "F236D751-431F-B1F3-C901-299D81B07900";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "25FC7707-4CA6-D909-68AD-95BDE76BC40D";
createNode follicle -n "pSphere1FollicleShape2846" -p "pSphere1Follicle2846";
	rename -uid "4B37A211-40E1-1D21-9617-B2AB6EF1A15A";
	setAttr -k off ".v";
	setAttr ".pu" 0.28125;
	setAttr ".pv" 0.46875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve68" -p "pSphere1Follicle2846";
	rename -uid "2355FCDB-419A-115D-C578-F4AAFD3C4956";
createNode nurbsCurve -n "curveShape68" -p "curve68";
	rename -uid "90A818A5-4D44-2221-85BF-82986634CD62";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "6406BF0F-4189-C9B4-F734-E08E8810E2E7";
createNode follicle -n "pSphere1FollicleShape2853" -p "pSphere1Follicle2853";
	rename -uid "EDF8A783-436D-A7DA-9C3F-279BBE345B23";
	setAttr -k off ".v";
	setAttr ".pu" 0.28125;
	setAttr ".pv" 0.53125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve69" -p "pSphere1Follicle2853";
	rename -uid "0DB30809-4716-88DD-EA99-858786D2498A";
createNode nurbsCurve -n "curveShape69" -p "curve69";
	rename -uid "48DFFC21-42D6-B6C2-37DE-838AA17D7658";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "7A89D037-4A38-506F-8E67-ABA39DCDDC56";
createNode follicle -n "pSphere1FollicleShape2859" -p "pSphere1Follicle2859";
	rename -uid "BF4F84FB-4D30-B81A-A268-749D15A68217";
	setAttr -k off ".v";
	setAttr ".pu" 0.28125;
	setAttr ".pv" 0.59375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve70" -p "pSphere1Follicle2859";
	rename -uid "33CEFC86-4896-BF34-BB34-2FAAD5AE5771";
createNode nurbsCurve -n "curveShape70" -p "curve70";
	rename -uid "86CC73DF-4314-4F09-AE7F-1BB70A8D716F";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "20C630DF-405D-60EE-C6CA-22B76E3BA03D";
createNode follicle -n "pSphere1FollicleShape2865" -p "pSphere1Follicle2865";
	rename -uid "BF27F583-4861-5CB8-88E3-03B5C62A56E4";
	setAttr -k off ".v";
	setAttr ".pu" 0.28125;
	setAttr ".pv" 0.65625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve71" -p "pSphere1Follicle2865";
	rename -uid "A82ECF65-4F33-C3DB-7F50-22A71FA54781";
createNode nurbsCurve -n "curveShape71" -p "curve71";
	rename -uid "473BDBC7-4777-2D7D-41CB-BAB4EA89B43B";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "93A97E72-4C3E-D6F6-F371-B496AD004B72";
createNode follicle -n "pSphere1FollicleShape2871" -p "pSphere1Follicle2871";
	rename -uid "92805908-4F33-8F89-83A7-5CB75CA65699";
	setAttr -k off ".v";
	setAttr ".pu" 0.28125;
	setAttr ".pv" 0.71875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve72" -p "pSphere1Follicle2871";
	rename -uid "65E882DA-4A12-D8B2-1308-37A206D6927E";
createNode nurbsCurve -n "curveShape72" -p "curve72";
	rename -uid "A58326E5-418B-658B-EB3D-51B1392A7114";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "42F828E8-43EA-7038-4129-8BAF71F01D36";
createNode follicle -n "pSphere1FollicleShape2877" -p "pSphere1Follicle2877";
	rename -uid "CB2E958C-41E7-C46F-4D88-FB81A61EC283";
	setAttr -k off ".v";
	setAttr ".pu" 0.28125;
	setAttr ".pv" 0.78125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve73" -p "pSphere1Follicle2877";
	rename -uid "47B5D31C-49DC-7962-9998-28B24550ACAD";
createNode nurbsCurve -n "curveShape73" -p "curve73";
	rename -uid "5B14B3D7-4DB9-B726-7776-42895F908088";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "6114EF0A-42E5-E37E-2898-7F8EFB5C1B72";
createNode follicle -n "pSphere1FollicleShape2884" -p "pSphere1Follicle2884";
	rename -uid "6C3FE953-4E3D-6299-ACD6-46869E269A03";
	setAttr -k off ".v";
	setAttr ".pu" 0.28125;
	setAttr ".pv" 0.84375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve74" -p "pSphere1Follicle2884";
	rename -uid "C8454030-4CD1-FD80-E38D-8E82EEFBE078";
createNode nurbsCurve -n "curveShape74" -p "curve74";
	rename -uid "EDB9884D-4518-3225-6548-599482909F94";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "EA9206ED-4488-AA6E-F1DE-76A76B577FF5";
createNode follicle -n "pSphere1FollicleShape2890" -p "pSphere1Follicle2890";
	rename -uid "59A4AE98-441B-462D-C79E-D8B145AC864E";
	setAttr -k off ".v";
	setAttr ".pu" 0.28125;
	setAttr ".pv" 0.90625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve75" -p "pSphere1Follicle2890";
	rename -uid "BC7C8127-447E-D097-3842-1E84407B9C57";
createNode nurbsCurve -n "curveShape75" -p "curve75";
	rename -uid "7C0FA49D-4969-76FC-75BF-C6817EFBB658";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "74AE597A-4657-1B0D-D5D4-8FBFA892E103";
createNode follicle -n "pSphere1FollicleShape2896" -p "pSphere1Follicle2896";
	rename -uid "EA18F7CA-413D-AF8D-E2C2-B5AC746428C7";
	setAttr -k off ".v";
	setAttr ".pu" 0.28125;
	setAttr ".pv" 0.96875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve76" -p "pSphere1Follicle2896";
	rename -uid "8C4666E5-46F4-71DD-1AAC-DE9C75A94CBB";
createNode nurbsCurve -n "curveShape76" -p "curve76";
	rename -uid "E5C9333E-4D5B-1CCC-6E3E-B49B031BCA35";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "90A9258C-4B6C-F576-0D64-419096D21AFF";
createNode follicle -n "pSphere1FollicleShape3409" -p "pSphere1Follicle3409";
	rename -uid "FC69D35B-4C6F-80B5-6C19-28B3CF8AC94D";
	setAttr -k off ".v";
	setAttr ".pu" 0.34375;
	setAttr ".pv" 0.09375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve77" -p "pSphere1Follicle3409";
	rename -uid "3CEA8D8C-4A68-765D-4A7A-508368FD515E";
createNode nurbsCurve -n "curveShape77" -p "curve77";
	rename -uid "380B07A2-43AC-DF3D-5E20-279E9E189036";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "D7C33AF1-4F11-9FEF-3DA9-4FA9E43D5A32";
createNode follicle -n "pSphere1FollicleShape3415" -p "pSphere1Follicle3415";
	rename -uid "2A4AD016-4A97-E0C3-4C82-E78747D93465";
	setAttr -k off ".v";
	setAttr ".pu" 0.34375;
	setAttr ".pv" 0.15625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve78" -p "pSphere1Follicle3415";
	rename -uid "3F254E89-41B5-479F-85A5-4285F86DF1DF";
createNode nurbsCurve -n "curveShape78" -p "curve78";
	rename -uid "AE159334-4E77-80F1-E563-1EAA49B9239B";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "2CD9AFA5-4CCA-D522-7C63-37BD4D601A8D";
createNode follicle -n "pSphere1FollicleShape3422" -p "pSphere1Follicle3422";
	rename -uid "3E26862D-44ED-9E40-DA44-A1ABB7916705";
	setAttr -k off ".v";
	setAttr ".pu" 0.34375;
	setAttr ".pv" 0.21875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve79" -p "pSphere1Follicle3422";
	rename -uid "5AC999D5-4CBD-9F45-7857-D09078F6D4BB";
createNode nurbsCurve -n "curveShape79" -p "curve79";
	rename -uid "04353324-481A-9B97-799B-33A2BEE534D4";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "4DC4FBBA-4D01-EE0A-F2D4-F4868E6AC609";
createNode follicle -n "pSphere1FollicleShape3428" -p "pSphere1Follicle3428";
	rename -uid "B542BBD8-42BD-7071-366C-399CB0AC83E7";
	setAttr -k off ".v";
	setAttr ".pu" 0.34375;
	setAttr ".pv" 0.28125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve80" -p "pSphere1Follicle3428";
	rename -uid "F2FC1ADE-46D9-D88B-4067-A38EE898A5FD";
createNode nurbsCurve -n "curveShape80" -p "curve80";
	rename -uid "DC3443BF-45B9-CEF2-5C57-23B309AFB7E7";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "B4D6EA7B-408E-5D28-0321-889A0902DAE8";
createNode follicle -n "pSphere1FollicleShape3434" -p "pSphere1Follicle3434";
	rename -uid "D5315C63-4BF3-195E-C7CE-758413EDBB16";
	setAttr -k off ".v";
	setAttr ".pu" 0.34375;
	setAttr ".pv" 0.34375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve81" -p "pSphere1Follicle3434";
	rename -uid "E9166BC1-4F85-29A9-3D16-938A18625A44";
createNode nurbsCurve -n "curveShape81" -p "curve81";
	rename -uid "D459D11A-4987-E490-1313-509EA7FA78C9";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "4824D378-49B7-9634-CB3D-95A00A009DA3";
createNode follicle -n "pSphere1FollicleShape3440" -p "pSphere1Follicle3440";
	rename -uid "14BCF6D5-45B6-81B9-8FF3-D083951C727D";
	setAttr -k off ".v";
	setAttr ".pu" 0.34375;
	setAttr ".pv" 0.40625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve82" -p "pSphere1Follicle3440";
	rename -uid "D8BA11DB-4C44-BC57-4334-88B5DB80A84B";
createNode nurbsCurve -n "curveShape82" -p "curve82";
	rename -uid "11D692AE-415D-2818-9A75-AE9D6D4938E7";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "84511027-4DAA-7D77-FC74-58AED1FD19F8";
createNode follicle -n "pSphere1FollicleShape3446" -p "pSphere1Follicle3446";
	rename -uid "949BB80A-49A7-E8C0-64BA-E997FEEB8EDE";
	setAttr -k off ".v";
	setAttr ".pu" 0.34375;
	setAttr ".pv" 0.46875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve83" -p "pSphere1Follicle3446";
	rename -uid "3EC5B9C6-469C-C1BF-8335-79A33F9327F2";
createNode nurbsCurve -n "curveShape83" -p "curve83";
	rename -uid "3C7A47E9-4F83-F6A8-8647-238B9D6E8F1E";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "3C483A14-4727-C903-F5A7-4A9D959C4A84";
createNode follicle -n "pSphere1FollicleShape3453" -p "pSphere1Follicle3453";
	rename -uid "B5D5BE90-4860-F11C-30CB-56A66B16777D";
	setAttr -k off ".v";
	setAttr ".pu" 0.34375;
	setAttr ".pv" 0.53125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve84" -p "pSphere1Follicle3453";
	rename -uid "90B25411-4FB1-52CF-A7C5-1DA165EB7B37";
createNode nurbsCurve -n "curveShape84" -p "curve84";
	rename -uid "B184C8D9-4AB2-D390-6A01-B196C92D03D2";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "F4DBFB2C-48A4-FD51-A8AA-8EB5ABDF952C";
createNode follicle -n "pSphere1FollicleShape3459" -p "pSphere1Follicle3459";
	rename -uid "2E4E0AAF-40C2-FF01-4FD6-BB970DD74A95";
	setAttr -k off ".v";
	setAttr ".pu" 0.34375;
	setAttr ".pv" 0.59375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve85" -p "pSphere1Follicle3459";
	rename -uid "1F8C56DF-4BE9-1098-6114-D0A4DCBD2900";
createNode nurbsCurve -n "curveShape85" -p "curve85";
	rename -uid "F695A031-4720-53B7-99A8-4BA509C91DB9";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "6CE1A5B9-4CEA-2D12-13BE-E79030A23660";
createNode follicle -n "pSphere1FollicleShape3465" -p "pSphere1Follicle3465";
	rename -uid "24DD3A41-44D7-5669-AEFF-5C8141C079B2";
	setAttr -k off ".v";
	setAttr ".pu" 0.34375;
	setAttr ".pv" 0.65625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve86" -p "pSphere1Follicle3465";
	rename -uid "9B914126-4184-8D78-909A-23A5696E4B69";
createNode nurbsCurve -n "curveShape86" -p "curve86";
	rename -uid "BF0D969B-452F-A5AD-AB37-4A810F674709";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "1FAFBFD7-48DF-C623-F2C9-DB9E5CA8D319";
createNode follicle -n "pSphere1FollicleShape3471" -p "pSphere1Follicle3471";
	rename -uid "5AFB36DA-4125-C00E-8C61-71861F215F6F";
	setAttr -k off ".v";
	setAttr ".pu" 0.34375;
	setAttr ".pv" 0.71875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve87" -p "pSphere1Follicle3471";
	rename -uid "45B9C287-42E9-81D3-47DB-14A38AB9251D";
createNode nurbsCurve -n "curveShape87" -p "curve87";
	rename -uid "D62FB08F-45F4-BD3E-EFB7-8A907C6F181A";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "11650438-4C70-016E-08E9-CDA5FF341D7B";
createNode follicle -n "pSphere1FollicleShape3477" -p "pSphere1Follicle3477";
	rename -uid "BA102B64-43E6-EA18-317F-E0A94A75DC5B";
	setAttr -k off ".v";
	setAttr ".pu" 0.34375;
	setAttr ".pv" 0.78125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve88" -p "pSphere1Follicle3477";
	rename -uid "DBA01772-4CCF-EC97-54B5-1695FE86B1EF";
createNode nurbsCurve -n "curveShape88" -p "curve88";
	rename -uid "03384FE0-4A46-2122-358E-5C8261250F52";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "C2399348-45ED-8486-C760-4590FDA93E3D";
createNode follicle -n "pSphere1FollicleShape3484" -p "pSphere1Follicle3484";
	rename -uid "F6535BC3-4BFD-F57E-01B2-B9BE0F370559";
	setAttr -k off ".v";
	setAttr ".pu" 0.34375;
	setAttr ".pv" 0.84375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve89" -p "pSphere1Follicle3484";
	rename -uid "F07B954C-4F72-AD7E-08BE-E7A13B107ADB";
createNode nurbsCurve -n "curveShape89" -p "curve89";
	rename -uid "BB453329-4A4B-0BF3-38FD-1F96ABB4D7C6";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "E1B41882-40C1-0B38-E488-9BB1405EE4F8";
createNode follicle -n "pSphere1FollicleShape3490" -p "pSphere1Follicle3490";
	rename -uid "F0512FE0-478F-C85A-3FDE-B5B12980E0A6";
	setAttr -k off ".v";
	setAttr ".pu" 0.34375;
	setAttr ".pv" 0.90625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve90" -p "pSphere1Follicle3490";
	rename -uid "B0613215-4659-4513-5382-2F9B5268FD45";
createNode nurbsCurve -n "curveShape90" -p "curve90";
	rename -uid "132F9548-4288-01A0-2B7B-4F8E0047B4EC";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "A3F926C1-4803-BF0F-AE64-F2BB49F07DC2";
createNode follicle -n "pSphere1FollicleShape4009" -p "pSphere1Follicle4009";
	rename -uid "4B370062-4E84-6202-D828-FCAC034CF0DC";
	setAttr -k off ".v";
	setAttr ".pu" 0.40625;
	setAttr ".pv" 0.09375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve91" -p "pSphere1Follicle4009";
	rename -uid "E5E03346-446B-215E-09EB-4BB1AA5D0442";
createNode nurbsCurve -n "curveShape91" -p "curve91";
	rename -uid "FD2EDBAB-43E4-45C9-427E-E68020966538";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "59703B6C-48EE-FB81-7DC1-0B82926F468D";
createNode follicle -n "pSphere1FollicleShape4015" -p "pSphere1Follicle4015";
	rename -uid "FFCD0E4C-4D35-1400-9BE8-3B84D638C115";
	setAttr -k off ".v";
	setAttr ".pu" 0.40625;
	setAttr ".pv" 0.15625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve92" -p "pSphere1Follicle4015";
	rename -uid "D610972F-4702-AB71-CE0A-198A98006F7D";
createNode nurbsCurve -n "curveShape92" -p "curve92";
	rename -uid "773492AE-456C-685B-354B-6688C2639884";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "DB69153F-4B89-0F81-8014-FFA388F5293D";
createNode follicle -n "pSphere1FollicleShape4022" -p "pSphere1Follicle4022";
	rename -uid "680415C4-4892-F0EE-DDC5-9693819B702E";
	setAttr -k off ".v";
	setAttr ".pu" 0.40625;
	setAttr ".pv" 0.21875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve93" -p "pSphere1Follicle4022";
	rename -uid "0E46FA2B-487F-1E09-7ABD-4590D42A2D61";
createNode nurbsCurve -n "curveShape93" -p "curve93";
	rename -uid "36D8C2A9-4B64-A7D4-922F-23822DBC2F06";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "04562C52-4CD7-77E2-9C7D-ABB5B4DE60A8";
createNode follicle -n "pSphere1FollicleShape4028" -p "pSphere1Follicle4028";
	rename -uid "72F38453-4631-F756-6DC4-0AB89F9E4034";
	setAttr -k off ".v";
	setAttr ".pu" 0.40625;
	setAttr ".pv" 0.28125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve94" -p "pSphere1Follicle4028";
	rename -uid "FDBD7626-4F8C-EA0B-86B8-028615408407";
createNode nurbsCurve -n "curveShape94" -p "curve94";
	rename -uid "D44EC9E6-4C48-0D9C-F85F-7F8866A5C08B";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "6262B3D0-4677-D486-35F9-16884420201F";
createNode follicle -n "pSphere1FollicleShape4034" -p "pSphere1Follicle4034";
	rename -uid "C18A36D0-49BD-555F-19F5-A4AE857E3980";
	setAttr -k off ".v";
	setAttr ".pu" 0.40625;
	setAttr ".pv" 0.34375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve95" -p "pSphere1Follicle4034";
	rename -uid "D11027D0-4A60-77E0-2840-EA8627C77962";
createNode nurbsCurve -n "curveShape95" -p "curve95";
	rename -uid "F9CAC58E-4329-0285-CC94-12A2689EA722";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "94482D2A-4FA0-314B-3963-86B0441FCBD2";
createNode follicle -n "pSphere1FollicleShape4040" -p "pSphere1Follicle4040";
	rename -uid "5E34B3DB-45C0-E110-A653-05817443C6C7";
	setAttr -k off ".v";
	setAttr ".pu" 0.40625;
	setAttr ".pv" 0.40625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve96" -p "pSphere1Follicle4040";
	rename -uid "8992133D-45CF-8AFF-0042-2D8359B6CB05";
createNode nurbsCurve -n "curveShape96" -p "curve96";
	rename -uid "AA821C64-43D4-550C-5FD5-2AAA7076E980";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "122A2B6C-455E-C405-B0DF-DFA648557315";
createNode follicle -n "pSphere1FollicleShape4046" -p "pSphere1Follicle4046";
	rename -uid "186BB82F-4775-1A6F-F263-698EA001AEED";
	setAttr -k off ".v";
	setAttr ".pu" 0.40625;
	setAttr ".pv" 0.46875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve97" -p "pSphere1Follicle4046";
	rename -uid "BDE59971-4B4A-4FA9-36EA-D5B85634FBEF";
createNode nurbsCurve -n "curveShape97" -p "curve97";
	rename -uid "9C0B4995-42D0-C38B-9FF5-8EB68693C3E6";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "FF710B87-41F5-D1AB-3396-E69E3A98F40D";
createNode follicle -n "pSphere1FollicleShape4053" -p "pSphere1Follicle4053";
	rename -uid "9388E810-4F48-6429-78A0-E3BBFB7D9BB8";
	setAttr -k off ".v";
	setAttr ".pu" 0.40625;
	setAttr ".pv" 0.53125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve98" -p "pSphere1Follicle4053";
	rename -uid "87E64906-4FA6-8B6D-1C49-79B40335BBB8";
createNode nurbsCurve -n "curveShape98" -p "curve98";
	rename -uid "EFFD452C-4EC4-80FD-49B5-19B12CAD6761";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "499781EC-4AFF-696F-2748-F292B2D41330";
createNode follicle -n "pSphere1FollicleShape4059" -p "pSphere1Follicle4059";
	rename -uid "62A94944-4E23-574C-5913-B9BA24AD8B57";
	setAttr -k off ".v";
	setAttr ".pu" 0.40625;
	setAttr ".pv" 0.59375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve99" -p "pSphere1Follicle4059";
	rename -uid "0B755B88-46CA-2967-A982-85AF7957A9EE";
createNode nurbsCurve -n "curveShape99" -p "curve99";
	rename -uid "A0EF7642-4053-5701-90F9-88A0B732519C";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "3B3828AF-43C5-04C9-3AB4-57BB062FC88F";
createNode follicle -n "pSphere1FollicleShape4065" -p "pSphere1Follicle4065";
	rename -uid "82DD9566-496D-3731-0183-CA88849A1E7D";
	setAttr -k off ".v";
	setAttr ".pu" 0.40625;
	setAttr ".pv" 0.65625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve100" -p "pSphere1Follicle4065";
	rename -uid "67F25438-4DA3-C83A-3853-DC9E7501FBB2";
createNode nurbsCurve -n "curveShape100" -p "curve100";
	rename -uid "08A279F2-4E02-AB63-7D53-1C9C6C538EEF";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "8983ACD4-489D-F113-AC1B-9495DDF9F6FD";
createNode follicle -n "pSphere1FollicleShape4071" -p "pSphere1Follicle4071";
	rename -uid "3056DFD3-498E-A147-991D-AF9820C2CD9F";
	setAttr -k off ".v";
	setAttr ".pu" 0.40625;
	setAttr ".pv" 0.71875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve101" -p "pSphere1Follicle4071";
	rename -uid "EFFE7B6C-4EBB-094F-BEF2-038718B85DB9";
createNode nurbsCurve -n "curveShape101" -p "curve101";
	rename -uid "CA82F880-473A-CEFB-F97D-2D9E121F3C88";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "F96E20B7-443F-0600-9B0E-8FAB6EDB6F68";
createNode follicle -n "pSphere1FollicleShape4077" -p "pSphere1Follicle4077";
	rename -uid "CCF61354-47EA-C57A-2B19-159EC0E83FA6";
	setAttr -k off ".v";
	setAttr ".pu" 0.40625;
	setAttr ".pv" 0.78125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve102" -p "pSphere1Follicle4077";
	rename -uid "D7B23EAC-4216-5949-BEB6-A7B9F6C25FBC";
createNode nurbsCurve -n "curveShape102" -p "curve102";
	rename -uid "B5D9044C-4CAC-05E3-263B-43B270136F4E";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "EA842E85-4B3B-ADAF-5A7A-4EABDE22264A";
createNode follicle -n "pSphere1FollicleShape4084" -p "pSphere1Follicle4084";
	rename -uid "80719AC8-4EFD-75EB-26B9-E495E8D66B0C";
	setAttr -k off ".v";
	setAttr ".pu" 0.40625;
	setAttr ".pv" 0.84375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve103" -p "pSphere1Follicle4084";
	rename -uid "F6D560D7-4E13-846D-316D-1B8CA40D98CD";
createNode nurbsCurve -n "curveShape103" -p "curve103";
	rename -uid "D93402FB-43F4-AC9E-3455-DFB189E24C52";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "5AA27B13-46B4-22E1-4D5D-4F89BF6DD842";
createNode follicle -n "pSphere1FollicleShape4090" -p "pSphere1Follicle4090";
	rename -uid "5077900F-401E-54D6-2FAA-23A7B08134F9";
	setAttr -k off ".v";
	setAttr ".pu" 0.40625;
	setAttr ".pv" 0.90625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve104" -p "pSphere1Follicle4090";
	rename -uid "68404C8E-4F73-323A-CA6D-04A4088C6465";
createNode nurbsCurve -n "curveShape104" -p "curve104";
	rename -uid "3F912339-4F39-C92F-D70F-358A2A039A96";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "059E0F14-4FB2-3535-C29A-E4816EF8713F";
createNode follicle -n "pSphere1FollicleShape4603" -p "pSphere1Follicle4603";
	rename -uid "2ED0FE28-4219-D20E-5920-A6B56FA4154E";
	setAttr -k off ".v";
	setAttr ".pu" 0.46875;
	setAttr ".pv" 0.03125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve105" -p "pSphere1Follicle4603";
	rename -uid "0E4CE594-4928-98E4-15C8-7484A5FA334A";
createNode nurbsCurve -n "curveShape105" -p "curve105";
	rename -uid "966C959F-45B4-9E27-2503-52BF0B03F7C1";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "E9B02642-4B60-ACA2-AE9D-729FAA5BA875";
createNode follicle -n "pSphere1FollicleShape4609" -p "pSphere1Follicle4609";
	rename -uid "8E11BCEA-4504-13E4-7035-41A0E2B8F254";
	setAttr -k off ".v";
	setAttr ".pu" 0.46875;
	setAttr ".pv" 0.09375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve106" -p "pSphere1Follicle4609";
	rename -uid "D2CACFCC-40DE-F618-62C8-36944B51D2A3";
createNode nurbsCurve -n "curveShape106" -p "curve106";
	rename -uid "EEA98198-4173-A49B-5861-1BBE6407DE33";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "9C99BA7A-401E-C271-F1BF-40BFD8BEAD99";
createNode follicle -n "pSphere1FollicleShape4615" -p "pSphere1Follicle4615";
	rename -uid "8BB049A8-4B15-00BD-DEFE-B39E3D96786B";
	setAttr -k off ".v";
	setAttr ".pu" 0.46875;
	setAttr ".pv" 0.15625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve107" -p "pSphere1Follicle4615";
	rename -uid "E68AAF53-464B-CDA7-63C5-D6AD074185F1";
createNode nurbsCurve -n "curveShape107" -p "curve107";
	rename -uid "264535A1-4C41-3DD3-6E06-239ADD636A31";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "43C77B6E-4643-7516-2CCE-FAB4346F0EBD";
createNode follicle -n "pSphere1FollicleShape4622" -p "pSphere1Follicle4622";
	rename -uid "CEE1EE8F-474E-DC80-1726-3BBBFBF3ECF2";
	setAttr -k off ".v";
	setAttr ".pu" 0.46875;
	setAttr ".pv" 0.21875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve108" -p "pSphere1Follicle4622";
	rename -uid "2E27C214-4DA3-C5FE-A93D-D7AFFD3E53ED";
createNode nurbsCurve -n "curveShape108" -p "curve108";
	rename -uid "7696D945-4BE2-87C4-2A15-BCB8D147D4ED";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "3EE01387-48E6-C0D5-FFB5-C8AB714B0696";
createNode follicle -n "pSphere1FollicleShape4628" -p "pSphere1Follicle4628";
	rename -uid "72208FEE-4928-F3D8-CF14-29BE050E714F";
	setAttr -k off ".v";
	setAttr ".pu" 0.46875;
	setAttr ".pv" 0.28125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve109" -p "pSphere1Follicle4628";
	rename -uid "C0C68726-4C0E-54C1-E9DD-60AE1820413F";
createNode nurbsCurve -n "curveShape109" -p "curve109";
	rename -uid "8519C2BC-4ACA-A662-9743-25B383B754DA";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "AB3FD013-403F-48E2-7471-1F8042529318";
createNode follicle -n "pSphere1FollicleShape4634" -p "pSphere1Follicle4634";
	rename -uid "AB87C156-4CEE-8620-8A5F-228810D61C1A";
	setAttr -k off ".v";
	setAttr ".pu" 0.46875;
	setAttr ".pv" 0.34375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve110" -p "pSphere1Follicle4634";
	rename -uid "25FD3795-4C8B-8A10-53E3-A1A41A5D0FFE";
createNode nurbsCurve -n "curveShape110" -p "curve110";
	rename -uid "7CF76684-493F-4643-5D03-3C9C823C36F8";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "4168FC0D-4A85-5E42-8943-1987BFA1AF08";
createNode follicle -n "pSphere1FollicleShape4640" -p "pSphere1Follicle4640";
	rename -uid "1DBBD927-4E6A-45F3-012C-20889DE1AA15";
	setAttr -k off ".v";
	setAttr ".pu" 0.46875;
	setAttr ".pv" 0.40625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve111" -p "pSphere1Follicle4640";
	rename -uid "7648F060-4D57-CBAC-F3BC-1691B032025F";
createNode nurbsCurve -n "curveShape111" -p "curve111";
	rename -uid "338C6F66-4881-0C64-1A63-1986764E88D4";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "2529CEE0-4219-76DE-2130-FF9FF14FFB8F";
createNode follicle -n "pSphere1FollicleShape4646" -p "pSphere1Follicle4646";
	rename -uid "4C15E878-4407-E4CF-D627-AD8E8E0CDF00";
	setAttr -k off ".v";
	setAttr ".pu" 0.46875;
	setAttr ".pv" 0.46875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve112" -p "pSphere1Follicle4646";
	rename -uid "6138F2FE-4676-F918-9234-A29DF86BFC97";
createNode nurbsCurve -n "curveShape112" -p "curve112";
	rename -uid "83E19681-462F-D368-2444-96805C865E64";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "545FDDAE-4B9A-5A0F-F016-BAAD9D340133";
createNode follicle -n "pSphere1FollicleShape4653" -p "pSphere1Follicle4653";
	rename -uid "1CEEFBA3-44C0-F83E-F190-BAB84C2DC470";
	setAttr -k off ".v";
	setAttr ".pu" 0.46875;
	setAttr ".pv" 0.53125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve113" -p "pSphere1Follicle4653";
	rename -uid "A60ABE3D-4586-8DF3-95A8-8FBE339017A8";
createNode nurbsCurve -n "curveShape113" -p "curve113";
	rename -uid "C8BD8A80-4F6C-A381-CF0F-4A85130CC3E9";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "61974A3F-4433-EEDC-A78B-1BA45AB5F7A7";
createNode follicle -n "pSphere1FollicleShape4659" -p "pSphere1Follicle4659";
	rename -uid "38144E86-4DC5-4FEB-22BC-4DA4F30F0DE0";
	setAttr -k off ".v";
	setAttr ".pu" 0.46875;
	setAttr ".pv" 0.59375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve114" -p "pSphere1Follicle4659";
	rename -uid "A107AEF9-4D35-EC65-922F-73984DBBBC97";
createNode nurbsCurve -n "curveShape114" -p "curve114";
	rename -uid "C2C4A671-45AC-604E-E707-219BDE18DFF2";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "5BE0EC3B-4549-5381-6A16-9C933E4D4C7A";
createNode follicle -n "pSphere1FollicleShape4665" -p "pSphere1Follicle4665";
	rename -uid "13D1D282-4854-9B7C-BCDF-3584E8EB3808";
	setAttr -k off ".v";
	setAttr ".pu" 0.46875;
	setAttr ".pv" 0.65625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve115" -p "pSphere1Follicle4665";
	rename -uid "D0644A39-4880-79AA-905F-3EB27A0C4F1E";
createNode nurbsCurve -n "curveShape115" -p "curve115";
	rename -uid "418CC496-452F-90CF-680D-7BA077A8C946";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "BEED7481-49DA-C43E-E204-069FAFBA505D";
createNode follicle -n "pSphere1FollicleShape4671" -p "pSphere1Follicle4671";
	rename -uid "B412B7BD-4D5D-1D7D-A1E4-F2A748945284";
	setAttr -k off ".v";
	setAttr ".pu" 0.46875;
	setAttr ".pv" 0.71875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve116" -p "pSphere1Follicle4671";
	rename -uid "360F2B33-4A6B-F20E-B539-DFB83D89F092";
createNode nurbsCurve -n "curveShape116" -p "curve116";
	rename -uid "38EA0DB7-4ACB-C014-ACAC-838A9E9C047D";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "917D38B3-43B5-A88F-9CF8-66B7B93F5934";
createNode follicle -n "pSphere1FollicleShape4677" -p "pSphere1Follicle4677";
	rename -uid "CF1C910A-439C-E454-5148-6FB15AF9F9F4";
	setAttr -k off ".v";
	setAttr ".pu" 0.46875;
	setAttr ".pv" 0.78125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve117" -p "pSphere1Follicle4677";
	rename -uid "601B7424-4DBB-F838-DC6C-0EBF8E7286E0";
createNode nurbsCurve -n "curveShape117" -p "curve117";
	rename -uid "1D15F38A-4FBB-3E70-A00E-5A9D7DFEA2F4";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "CF2FE481-40A3-0CDD-7CBD-8B8E9499B75D";
createNode follicle -n "pSphere1FollicleShape4684" -p "pSphere1Follicle4684";
	rename -uid "6EBDB3DC-43D2-D3BF-E6D3-AE8284B9C7AB";
	setAttr -k off ".v";
	setAttr ".pu" 0.46875;
	setAttr ".pv" 0.84375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve118" -p "pSphere1Follicle4684";
	rename -uid "28D8DCA2-4DD6-8C2C-37F3-F1B63ED78E77";
createNode nurbsCurve -n "curveShape118" -p "curve118";
	rename -uid "7A01711C-4691-56BF-D46A-50880F249A18";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "A532BDF7-4E86-1347-A7BC-B09530CA13A1";
createNode follicle -n "pSphere1FollicleShape4690" -p "pSphere1Follicle4690";
	rename -uid "7012A153-404C-8DF8-6B19-2CB59977DD84";
	setAttr -k off ".v";
	setAttr ".pu" 0.46875;
	setAttr ".pv" 0.90625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve119" -p "pSphere1Follicle4690";
	rename -uid "8D3BB133-433A-A08A-154F-ECAE3A52EDE1";
createNode nurbsCurve -n "curveShape119" -p "curve119";
	rename -uid "A0D4503C-46B4-91C3-FFD7-F58CDCCFDE55";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "123BC5CE-417B-1B1A-5A03-DA841AEA32EB";
createNode follicle -n "pSphere1FollicleShape4696" -p "pSphere1Follicle4696";
	rename -uid "802F4BF4-41C4-252B-1C3E-70BAB2811DA1";
	setAttr -k off ".v";
	setAttr ".pu" 0.46875;
	setAttr ".pv" 0.96875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve120" -p "pSphere1Follicle4696";
	rename -uid "1A94A1FB-42D3-A4CB-6880-E4B1BE7D49DB";
createNode nurbsCurve -n "curveShape120" -p "curve120";
	rename -uid "B3681C4A-410A-D95B-2D0E-7BACE5AAD1A6";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "672FA867-4F4E-D3C0-CC60-8A8C5A21A038";
createNode follicle -n "pSphere1FollicleShape5303" -p "pSphere1Follicle5303";
	rename -uid "FE0D8D55-446D-2231-B615-99A9B314DE62";
	setAttr -k off ".v";
	setAttr ".pu" 0.53125;
	setAttr ".pv" 0.03125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve121" -p "pSphere1Follicle5303";
	rename -uid "F12F3E1E-4DBC-8862-8298-69A9914713D8";
createNode nurbsCurve -n "curveShape121" -p "curve121";
	rename -uid "EA1E71E9-4FCA-294E-4B54-3CAC257561DF";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "80CE0F41-496E-6F5D-998D-CA990FE9EF9A";
createNode follicle -n "pSphere1FollicleShape5309" -p "pSphere1Follicle5309";
	rename -uid "27CA8DB5-4F0B-CAE6-60E7-FFBEC3BC8F32";
	setAttr -k off ".v";
	setAttr ".pu" 0.53125;
	setAttr ".pv" 0.09375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve122" -p "pSphere1Follicle5309";
	rename -uid "80FA9D72-49BB-3357-AC3C-5E928E001497";
createNode nurbsCurve -n "curveShape122" -p "curve122";
	rename -uid "EB1B0572-4B4C-4BAD-FA7D-B3BFFC78E7AC";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "78B040A9-4710-9401-9575-ADB362D25289";
createNode follicle -n "pSphere1FollicleShape5315" -p "pSphere1Follicle5315";
	rename -uid "DA755075-4BFD-4F8D-1D42-6B874112768E";
	setAttr -k off ".v";
	setAttr ".pu" 0.53125;
	setAttr ".pv" 0.15625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve123" -p "pSphere1Follicle5315";
	rename -uid "67A826A4-4188-7BA0-7689-A5B6E084102F";
createNode nurbsCurve -n "curveShape123" -p "curve123";
	rename -uid "1AB65846-4AE4-77D5-0322-178FF4EFB51B";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "7F8031E8-4B79-406F-1388-969CC776042F";
createNode follicle -n "pSphere1FollicleShape5322" -p "pSphere1Follicle5322";
	rename -uid "B4C0534F-4153-0C81-0197-86A92FCB2771";
	setAttr -k off ".v";
	setAttr ".pu" 0.53125;
	setAttr ".pv" 0.21875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve124" -p "pSphere1Follicle5322";
	rename -uid "2B76F4D1-48DD-0B15-CBA8-F4A4CA2561C7";
createNode nurbsCurve -n "curveShape124" -p "curve124";
	rename -uid "45450524-4703-BA53-260A-F99754446B38";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "11606496-468A-B385-A00C-6E8251095B85";
createNode follicle -n "pSphere1FollicleShape5328" -p "pSphere1Follicle5328";
	rename -uid "B33DA487-4242-33CE-1C8D-BDA25AFB7AD3";
	setAttr -k off ".v";
	setAttr ".pu" 0.53125;
	setAttr ".pv" 0.28125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve125" -p "pSphere1Follicle5328";
	rename -uid "D9A3634B-43FC-7E4E-F642-0987F402B316";
createNode nurbsCurve -n "curveShape125" -p "curve125";
	rename -uid "EA231898-449B-CC32-DAF4-318E67CF8A4E";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "CFF00B71-46E2-5E5B-A830-AEBB0D2C6BA2";
createNode follicle -n "pSphere1FollicleShape5334" -p "pSphere1Follicle5334";
	rename -uid "F3FBAA75-47C3-1287-02DB-DD8755DDA46A";
	setAttr -k off ".v";
	setAttr ".pu" 0.53125;
	setAttr ".pv" 0.34375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve126" -p "pSphere1Follicle5334";
	rename -uid "09ADEC14-409B-A094-62AA-8BAF8C0F8C97";
createNode nurbsCurve -n "curveShape126" -p "curve126";
	rename -uid "C6679272-4317-9A04-315D-8CA8FCDF7C65";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "8855D65F-4F50-AC68-81BB-799D66844B0D";
createNode follicle -n "pSphere1FollicleShape5340" -p "pSphere1Follicle5340";
	rename -uid "F97E5017-4696-40F3-789E-FB9714862D9E";
	setAttr -k off ".v";
	setAttr ".pu" 0.53125;
	setAttr ".pv" 0.40625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve127" -p "pSphere1Follicle5340";
	rename -uid "1179977A-481C-A3FC-E334-FC989805098C";
createNode nurbsCurve -n "curveShape127" -p "curve127";
	rename -uid "32CC094E-43C9-D056-92F9-EAB21E12916E";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "C988305B-4D35-2EC6-1662-27B41261E829";
createNode follicle -n "pSphere1FollicleShape5346" -p "pSphere1Follicle5346";
	rename -uid "365DD992-4CE9-E0D2-3511-0A9A986FE7AD";
	setAttr -k off ".v";
	setAttr ".pu" 0.53125;
	setAttr ".pv" 0.46875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve128" -p "pSphere1Follicle5346";
	rename -uid "22567510-4678-DBCA-4747-E3A40737F3AD";
createNode nurbsCurve -n "curveShape128" -p "curve128";
	rename -uid "6C05728B-4481-45BA-F0DD-CDBFBE71A14B";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "C2C802B6-4260-BEAF-B6B6-D18B4B95AD27";
createNode follicle -n "pSphere1FollicleShape5353" -p "pSphere1Follicle5353";
	rename -uid "1F31E0CE-4A91-2B8F-01FC-1B8B0A987CF8";
	setAttr -k off ".v";
	setAttr ".pu" 0.53125;
	setAttr ".pv" 0.53125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve129" -p "pSphere1Follicle5353";
	rename -uid "29419845-45DB-6977-B850-0CA62C6F9801";
createNode nurbsCurve -n "curveShape129" -p "curve129";
	rename -uid "9D142139-4E61-C975-534F-D5BDFBB5407C";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "29E1AA59-454F-5732-89E8-E1862E87CCE2";
createNode follicle -n "pSphere1FollicleShape5359" -p "pSphere1Follicle5359";
	rename -uid "AEA97657-4D6A-5DDB-BE85-FD839E335402";
	setAttr -k off ".v";
	setAttr ".pu" 0.53125;
	setAttr ".pv" 0.59375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve130" -p "pSphere1Follicle5359";
	rename -uid "C5ECED18-499B-2752-9A9D-E7953EA89D89";
createNode nurbsCurve -n "curveShape130" -p "curve130";
	rename -uid "C494FB85-4018-A9E1-91CC-119A63FD937B";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "A39B3D33-45C4-7DC1-56AF-8CA0DA0DE762";
createNode follicle -n "pSphere1FollicleShape5365" -p "pSphere1Follicle5365";
	rename -uid "D96C4E1C-4519-7A05-0218-429742ADF8DE";
	setAttr -k off ".v";
	setAttr ".pu" 0.53125;
	setAttr ".pv" 0.65625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve131" -p "pSphere1Follicle5365";
	rename -uid "3C83E678-4245-3D91-7FCA-779D0FD2D005";
createNode nurbsCurve -n "curveShape131" -p "curve131";
	rename -uid "91A09151-4401-F190-EF71-51B7EAAF0EA5";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "14ECDB1F-4200-08BE-19A2-C6A5AA9E8AF3";
createNode follicle -n "pSphere1FollicleShape5371" -p "pSphere1Follicle5371";
	rename -uid "389B3CAA-4499-34C5-A98A-378075F56DBB";
	setAttr -k off ".v";
	setAttr ".pu" 0.53125;
	setAttr ".pv" 0.71875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve132" -p "pSphere1Follicle5371";
	rename -uid "C21A7552-42BA-773B-3F6B-EE992CE36A32";
createNode nurbsCurve -n "curveShape132" -p "curve132";
	rename -uid "373486BC-4672-6598-D368-46B25EC4282A";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "B5AE26FB-426F-CBE7-5668-34B857DFD54B";
createNode follicle -n "pSphere1FollicleShape5377" -p "pSphere1Follicle5377";
	rename -uid "257C6711-4877-C494-6474-7BA51480CF03";
	setAttr -k off ".v";
	setAttr ".pu" 0.53125;
	setAttr ".pv" 0.78125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve133" -p "pSphere1Follicle5377";
	rename -uid "7A84E56D-41E1-748B-5855-2D8F5753BA42";
createNode nurbsCurve -n "curveShape133" -p "curve133";
	rename -uid "D03D1298-47B2-87DD-A20A-28AFE067EEA3";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "7B4CA2EE-41E6-EBA8-95A0-DDA854670F74";
createNode follicle -n "pSphere1FollicleShape5384" -p "pSphere1Follicle5384";
	rename -uid "6088B160-47FC-0925-354F-CA8FB3BC961F";
	setAttr -k off ".v";
	setAttr ".pu" 0.53125;
	setAttr ".pv" 0.84375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve134" -p "pSphere1Follicle5384";
	rename -uid "445913DC-424B-3459-49A2-2FB280F98D70";
createNode nurbsCurve -n "curveShape134" -p "curve134";
	rename -uid "6D1FCA4E-444E-A9B9-D5BE-56BFCC8E9E48";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "E0236A91-4217-78FB-3BC8-7196C85C74E3";
createNode follicle -n "pSphere1FollicleShape5390" -p "pSphere1Follicle5390";
	rename -uid "B5A6548C-40AD-8381-D34C-068ACCFD76F6";
	setAttr -k off ".v";
	setAttr ".pu" 0.53125;
	setAttr ".pv" 0.90625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve135" -p "pSphere1Follicle5390";
	rename -uid "456A0573-4B3F-1629-4AAD-A8B8C4C7A1CB";
createNode nurbsCurve -n "curveShape135" -p "curve135";
	rename -uid "D21C9EFE-40F6-4675-A6B5-A3A367842C9C";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "7049BC3B-40F7-C720-07CD-A7B56C9A85A2";
createNode follicle -n "pSphere1FollicleShape5396" -p "pSphere1Follicle5396";
	rename -uid "F28C9302-40FF-170E-ABBC-B29A861CE5BC";
	setAttr -k off ".v";
	setAttr ".pu" 0.53125;
	setAttr ".pv" 0.96875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve136" -p "pSphere1Follicle5396";
	rename -uid "3D247CAD-4164-2E0D-FE96-DF9BB7CED17A";
createNode nurbsCurve -n "curveShape136" -p "curve136";
	rename -uid "C1A25FED-4501-D3BB-E0B1-A980662B8F6F";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "89FA0E95-4ACC-3913-159E-57B05D427E30";
createNode follicle -n "pSphere1FollicleShape5909" -p "pSphere1Follicle5909";
	rename -uid "79CBD802-4351-B177-06D4-45AD6BD779B5";
	setAttr -k off ".v";
	setAttr ".pu" 0.59375;
	setAttr ".pv" 0.09375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve137" -p "pSphere1Follicle5909";
	rename -uid "887FA889-4E66-9661-1D74-549048251BA7";
createNode nurbsCurve -n "curveShape137" -p "curve137";
	rename -uid "09BCA9AA-4382-CA49-FA9D-D8BA8175F3BD";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "AA7115E5-43F5-252C-38B4-E5AB31B6A518";
createNode follicle -n "pSphere1FollicleShape5915" -p "pSphere1Follicle5915";
	rename -uid "342BD1CF-48C3-0416-0CBD-D6B3D7FF3356";
	setAttr -k off ".v";
	setAttr ".pu" 0.59375;
	setAttr ".pv" 0.15625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve138" -p "pSphere1Follicle5915";
	rename -uid "F16BECF2-426E-0850-B5DF-D7881B2041A3";
createNode nurbsCurve -n "curveShape138" -p "curve138";
	rename -uid "046F957A-4187-3030-8BDF-FABBFE625607";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "6A6F8CDF-4C50-F8DF-9C1B-C79A82608226";
createNode follicle -n "pSphere1FollicleShape5922" -p "pSphere1Follicle5922";
	rename -uid "5D4A539E-460E-F362-74F9-E98C591A2465";
	setAttr -k off ".v";
	setAttr ".pu" 0.59375;
	setAttr ".pv" 0.21875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve139" -p "pSphere1Follicle5922";
	rename -uid "A6C9B176-4A26-3639-93F5-3E8FE27752EF";
createNode nurbsCurve -n "curveShape139" -p "curve139";
	rename -uid "8CC45C1C-49CE-51CE-A9B2-6193ED43C379";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "012D3F09-41F7-E239-9FED-AB9EEC257540";
createNode follicle -n "pSphere1FollicleShape5928" -p "pSphere1Follicle5928";
	rename -uid "CA363436-4F29-383D-3638-BD9169EFCC62";
	setAttr -k off ".v";
	setAttr ".pu" 0.59375;
	setAttr ".pv" 0.28125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve140" -p "pSphere1Follicle5928";
	rename -uid "BAC3F853-477E-DC56-C168-F38B3DE530A6";
createNode nurbsCurve -n "curveShape140" -p "curve140";
	rename -uid "0CCF09B1-4E36-0BB7-FDA8-65BCA3B65BF7";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "6E8FBC9D-457A-67AE-DBE2-219022958046";
createNode follicle -n "pSphere1FollicleShape5934" -p "pSphere1Follicle5934";
	rename -uid "D1240603-4583-D02A-8595-46BA867A3019";
	setAttr -k off ".v";
	setAttr ".pu" 0.59375;
	setAttr ".pv" 0.34375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve141" -p "pSphere1Follicle5934";
	rename -uid "FC4F297E-43C1-7E79-5E9A-C683A72564E7";
createNode nurbsCurve -n "curveShape141" -p "curve141";
	rename -uid "1D168454-40D7-88EE-8649-039CF07C335A";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "C3B153DD-4039-45B0-DA31-22A5512533A9";
createNode follicle -n "pSphere1FollicleShape5940" -p "pSphere1Follicle5940";
	rename -uid "7E629851-49CD-C64F-823B-3F86FB27D714";
	setAttr -k off ".v";
	setAttr ".pu" 0.59375;
	setAttr ".pv" 0.40625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve142" -p "pSphere1Follicle5940";
	rename -uid "5AE4D969-48C4-9C2F-0363-979330366E43";
createNode nurbsCurve -n "curveShape142" -p "curve142";
	rename -uid "1507E32E-463F-4B5B-899A-6F8FB7E87CFD";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "B023C929-4028-FC97-6F08-C88295CE4086";
createNode follicle -n "pSphere1FollicleShape5946" -p "pSphere1Follicle5946";
	rename -uid "685CD082-4006-7D33-90CD-378F9FF329D3";
	setAttr -k off ".v";
	setAttr ".pu" 0.59375;
	setAttr ".pv" 0.46875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve143" -p "pSphere1Follicle5946";
	rename -uid "3D46F4FD-4CDA-E33B-C620-A180EF2E2C68";
createNode nurbsCurve -n "curveShape143" -p "curve143";
	rename -uid "83B6DCC5-4042-B63A-2F99-CCBF32A9D1DC";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "10F162F6-4A50-8D31-6036-5BAEB0764570";
createNode follicle -n "pSphere1FollicleShape5953" -p "pSphere1Follicle5953";
	rename -uid "1D7081E5-4D8F-B992-0D84-91B7DE3C0E76";
	setAttr -k off ".v";
	setAttr ".pu" 0.59375;
	setAttr ".pv" 0.53125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve144" -p "pSphere1Follicle5953";
	rename -uid "BA5A7B6A-4B59-3CFF-C02B-D2BF3DAD8D7A";
createNode nurbsCurve -n "curveShape144" -p "curve144";
	rename -uid "2D90CC9C-46FF-8CBE-2C1E-75A0A16C6F37";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "A723BF90-47BB-59FF-F15C-26AC1CC04EAE";
createNode follicle -n "pSphere1FollicleShape5959" -p "pSphere1Follicle5959";
	rename -uid "8D011BDB-435E-8760-B231-36808F32CD55";
	setAttr -k off ".v";
	setAttr ".pu" 0.59375;
	setAttr ".pv" 0.59375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve145" -p "pSphere1Follicle5959";
	rename -uid "22794B7A-4208-F83F-4E4B-239240AFA7B6";
createNode nurbsCurve -n "curveShape145" -p "curve145";
	rename -uid "D3C61996-4C60-AF64-F558-B8AD2975C874";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "7D3644D2-4C40-22C2-9E32-1586D138D1FB";
createNode follicle -n "pSphere1FollicleShape5965" -p "pSphere1Follicle5965";
	rename -uid "C93EFFA7-4DB8-81F8-1692-72A6D939D255";
	setAttr -k off ".v";
	setAttr ".pu" 0.59375;
	setAttr ".pv" 0.65625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve146" -p "pSphere1Follicle5965";
	rename -uid "DBBD4323-4ADE-3676-D30F-C080968234BE";
createNode nurbsCurve -n "curveShape146" -p "curve146";
	rename -uid "807B07AF-4F33-5B5A-5F8A-D28F63DB6886";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "1C204FA4-474D-6DC8-D9E1-CF9C43F0F473";
createNode follicle -n "pSphere1FollicleShape5971" -p "pSphere1Follicle5971";
	rename -uid "B66EA769-4991-091D-F039-2696FF815DAB";
	setAttr -k off ".v";
	setAttr ".pu" 0.59375;
	setAttr ".pv" 0.71875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve147" -p "pSphere1Follicle5971";
	rename -uid "A65565A1-43F8-45D8-6C4F-078357F27570";
createNode nurbsCurve -n "curveShape147" -p "curve147";
	rename -uid "BEEFFD35-46BA-82D6-17D4-28B83CA5472A";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "C200C592-48AE-61A1-F153-C2A6124514D9";
createNode follicle -n "pSphere1FollicleShape5977" -p "pSphere1Follicle5977";
	rename -uid "BCAAF275-4480-8921-EEB3-5CB231D2E90B";
	setAttr -k off ".v";
	setAttr ".pu" 0.59375;
	setAttr ".pv" 0.78125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve148" -p "pSphere1Follicle5977";
	rename -uid "B9C85DAE-459E-C97D-E1A4-BAA656001EC5";
createNode nurbsCurve -n "curveShape148" -p "curve148";
	rename -uid "286E357D-4201-F579-FC2E-2DB6120C2C25";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "4FF20844-4C14-10AD-D531-0AAA44B4066B";
createNode follicle -n "pSphere1FollicleShape5984" -p "pSphere1Follicle5984";
	rename -uid "9E463A47-4C47-662C-E3CE-E3ADE877F35E";
	setAttr -k off ".v";
	setAttr ".pu" 0.59375;
	setAttr ".pv" 0.84375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve149" -p "pSphere1Follicle5984";
	rename -uid "FA8E5D37-4473-8BF6-91D5-9694F6B3EEA7";
createNode nurbsCurve -n "curveShape149" -p "curve149";
	rename -uid "AB5404DB-471C-4F39-6408-3D8C3E756634";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "51B3436E-4E32-E25B-96E4-A28D56672553";
createNode follicle -n "pSphere1FollicleShape5990" -p "pSphere1Follicle5990";
	rename -uid "8ADA0939-4369-E17C-CA94-66939ACCBB94";
	setAttr -k off ".v";
	setAttr ".pu" 0.59375;
	setAttr ".pv" 0.90625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve150" -p "pSphere1Follicle5990";
	rename -uid "B4CEEED7-4873-3C0F-9BA7-73BD8068B6EB";
createNode nurbsCurve -n "curveShape150" -p "curve150";
	rename -uid "EF3D0271-4C6D-7CBE-0E35-0E9CF75E0D3E";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "33AEA295-451E-5AB8-A958-6DA60BAE81B4";
createNode follicle -n "pSphere1FollicleShape6509" -p "pSphere1Follicle6509";
	rename -uid "836CE8C8-4BE1-0FD1-815E-A59D36F8F9C6";
	setAttr -k off ".v";
	setAttr ".pu" 0.65625;
	setAttr ".pv" 0.09375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve151" -p "pSphere1Follicle6509";
	rename -uid "8C5F40D9-4A0F-9509-A9E2-63BE1D84DCA3";
createNode nurbsCurve -n "curveShape151" -p "curve151";
	rename -uid "0B37C8B4-4F1F-5B82-7ABB-BFB4AA8096ED";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "2B4A16F2-4A72-15FB-3859-8993EE621833";
createNode follicle -n "pSphere1FollicleShape6515" -p "pSphere1Follicle6515";
	rename -uid "F467440D-48F1-9271-0307-57969461CCE0";
	setAttr -k off ".v";
	setAttr ".pu" 0.65625;
	setAttr ".pv" 0.15625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve152" -p "pSphere1Follicle6515";
	rename -uid "64CBD005-4C4B-0EF8-1AC6-308CB1813B44";
createNode nurbsCurve -n "curveShape152" -p "curve152";
	rename -uid "02696677-4667-C41E-B29A-D99431B983DF";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "B741ACEF-4771-F204-9C31-3E88C2E0BA7B";
createNode follicle -n "pSphere1FollicleShape6522" -p "pSphere1Follicle6522";
	rename -uid "8AEEB26F-4F47-CE9F-A54A-78B6E246B0BE";
	setAttr -k off ".v";
	setAttr ".pu" 0.65625;
	setAttr ".pv" 0.21875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve153" -p "pSphere1Follicle6522";
	rename -uid "2C07009F-4844-FBDE-9E03-4E9BCFF012F5";
createNode nurbsCurve -n "curveShape153" -p "curve153";
	rename -uid "CA35BA2B-40C8-150E-9A06-D695396A8E43";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "EC57C949-4FB6-778D-900D-B6820FCAAE83";
createNode follicle -n "pSphere1FollicleShape6528" -p "pSphere1Follicle6528";
	rename -uid "64B16240-4B5F-F9BB-4484-99BB8B3939CF";
	setAttr -k off ".v";
	setAttr ".pu" 0.65625;
	setAttr ".pv" 0.28125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve154" -p "pSphere1Follicle6528";
	rename -uid "4CE892C8-4AFA-5EEF-E741-51A66C2CD202";
createNode nurbsCurve -n "curveShape154" -p "curve154";
	rename -uid "287DFB57-4CE2-99E2-7420-47AFDC9356BB";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "E3823E13-4643-CB9C-E9B8-FD97999AB031";
createNode follicle -n "pSphere1FollicleShape6534" -p "pSphere1Follicle6534";
	rename -uid "5F90D7AC-4DD9-EACE-BC4C-3D8EBA9EE8BF";
	setAttr -k off ".v";
	setAttr ".pu" 0.65625;
	setAttr ".pv" 0.34375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve155" -p "pSphere1Follicle6534";
	rename -uid "AEDB1264-4DC7-2CE9-FB95-5399D99E4BCE";
createNode nurbsCurve -n "curveShape155" -p "curve155";
	rename -uid "1D5B4759-4A5B-BB46-D0AA-A18A4255533F";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "89C1B44B-4520-A36B-6562-07BF4FA8EF2E";
createNode follicle -n "pSphere1FollicleShape6540" -p "pSphere1Follicle6540";
	rename -uid "3E02DCA9-47AA-B1A9-AC31-1988D07B3BF7";
	setAttr -k off ".v";
	setAttr ".pu" 0.65625;
	setAttr ".pv" 0.40625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve156" -p "pSphere1Follicle6540";
	rename -uid "02984584-4E95-395C-32E6-92BD5043B39D";
createNode nurbsCurve -n "curveShape156" -p "curve156";
	rename -uid "E0B2F8B8-4A95-8D98-30E4-549DA239D08A";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "F12FF1B8-4CCF-F244-C48F-B892FC356666";
createNode follicle -n "pSphere1FollicleShape6546" -p "pSphere1Follicle6546";
	rename -uid "2C9F5F59-4E92-A3B0-15D2-B7804F52AF91";
	setAttr -k off ".v";
	setAttr ".pu" 0.65625;
	setAttr ".pv" 0.46875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve157" -p "pSphere1Follicle6546";
	rename -uid "0077FB2E-4317-AE74-13C4-41BFB6AEF268";
createNode nurbsCurve -n "curveShape157" -p "curve157";
	rename -uid "4A945D40-41E7-0128-A8E1-0996A140A122";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "22B7BDC0-493E-0147-EB46-81A54A32BE44";
createNode follicle -n "pSphere1FollicleShape6553" -p "pSphere1Follicle6553";
	rename -uid "592FA4AF-4AC9-E05A-BA55-5AA8CADA6785";
	setAttr -k off ".v";
	setAttr ".pu" 0.65625;
	setAttr ".pv" 0.53125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve158" -p "pSphere1Follicle6553";
	rename -uid "58BE1583-44C0-409F-CC9B-F7AAFF271BCC";
createNode nurbsCurve -n "curveShape158" -p "curve158";
	rename -uid "5D344EEB-4BD9-D9C1-D44B-8FAFFF5794B4";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "AF682522-443A-0330-7629-3FA2E84B144D";
createNode follicle -n "pSphere1FollicleShape6559" -p "pSphere1Follicle6559";
	rename -uid "5A4F12CB-4E9A-2F1E-E186-DF900D0CA822";
	setAttr -k off ".v";
	setAttr ".pu" 0.65625;
	setAttr ".pv" 0.59375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve159" -p "pSphere1Follicle6559";
	rename -uid "3FC131AF-4717-7DF1-ACCF-638C5467FF0D";
createNode nurbsCurve -n "curveShape159" -p "curve159";
	rename -uid "56C6151F-4E11-5D2E-19EE-A0B8C1DA2B34";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "E089071B-4C68-1009-2A55-6BB3AFE12284";
createNode follicle -n "pSphere1FollicleShape6565" -p "pSphere1Follicle6565";
	rename -uid "B560B581-4139-30F6-3700-B5AC73488582";
	setAttr -k off ".v";
	setAttr ".pu" 0.65625;
	setAttr ".pv" 0.65625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve160" -p "pSphere1Follicle6565";
	rename -uid "C6B77CB4-423E-0B23-62EB-57A15B2C4BCE";
createNode nurbsCurve -n "curveShape160" -p "curve160";
	rename -uid "72501576-468D-9149-68D7-4E9FAE284C66";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "97F224AA-4CCF-FF93-5CB9-0AAA574CB3AB";
createNode follicle -n "pSphere1FollicleShape6571" -p "pSphere1Follicle6571";
	rename -uid "3893ECCA-4389-F870-3EEC-C6A5E8463E98";
	setAttr -k off ".v";
	setAttr ".pu" 0.65625;
	setAttr ".pv" 0.71875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve161" -p "pSphere1Follicle6571";
	rename -uid "D7D3FBAA-4D48-5E9C-2B0E-FD966F9C896A";
createNode nurbsCurve -n "curveShape161" -p "curve161";
	rename -uid "394C72E5-46FE-2535-1938-6E812B02CBCA";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "805C24C3-4263-0618-386F-C3A96444362F";
createNode follicle -n "pSphere1FollicleShape6577" -p "pSphere1Follicle6577";
	rename -uid "722D5268-4F0B-9696-7D6F-88866AB8523C";
	setAttr -k off ".v";
	setAttr ".pu" 0.65625;
	setAttr ".pv" 0.78125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve162" -p "pSphere1Follicle6577";
	rename -uid "46C62153-4E5D-65AF-91E3-FCB191BC07C3";
createNode nurbsCurve -n "curveShape162" -p "curve162";
	rename -uid "2277F9CA-4C8B-652E-1535-1A9EC54A3D66";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "946B5590-4F0A-4CEA-4C81-908BFF7DC4D9";
createNode follicle -n "pSphere1FollicleShape6584" -p "pSphere1Follicle6584";
	rename -uid "7857C282-4671-7815-E026-649BDEFF14A1";
	setAttr -k off ".v";
	setAttr ".pu" 0.65625;
	setAttr ".pv" 0.84375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve163" -p "pSphere1Follicle6584";
	rename -uid "BB982907-44AB-E6CD-310E-23AC8ACABCD3";
createNode nurbsCurve -n "curveShape163" -p "curve163";
	rename -uid "B68AB1EA-4FCF-E322-F3E1-80B4D7070EC4";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "10E30CFA-4D9C-2844-250A-7C831CBAF77A";
createNode follicle -n "pSphere1FollicleShape6590" -p "pSphere1Follicle6590";
	rename -uid "9DD1D313-40E2-E3CA-6947-8090B234DA2C";
	setAttr -k off ".v";
	setAttr ".pu" 0.65625;
	setAttr ".pv" 0.90625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve164" -p "pSphere1Follicle6590";
	rename -uid "E9276758-4102-6544-A05B-7D9855FF6FD4";
createNode nurbsCurve -n "curveShape164" -p "curve164";
	rename -uid "72225DB4-43AD-3AA6-8A06-0F9C3A5AA888";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "BF88799C-4003-2146-3197-26A191CE39C9";
createNode follicle -n "pSphere1FollicleShape7103" -p "pSphere1Follicle7103";
	rename -uid "353E31C7-4A2D-721E-0E4D-BA91F4327B1B";
	setAttr -k off ".v";
	setAttr ".pu" 0.71875;
	setAttr ".pv" 0.03125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve165" -p "pSphere1Follicle7103";
	rename -uid "8BD992D8-4613-6F50-C347-6B805AB2516B";
createNode nurbsCurve -n "curveShape165" -p "curve165";
	rename -uid "65933E71-4E84-9B18-2971-3EA8E0FCB20C";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "4403F09B-4FDC-1620-7ED0-429445C7CE6D";
createNode follicle -n "pSphere1FollicleShape7109" -p "pSphere1Follicle7109";
	rename -uid "BA368F63-45BE-93C8-796C-83AA47DC0CD3";
	setAttr -k off ".v";
	setAttr ".pu" 0.71875;
	setAttr ".pv" 0.09375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve166" -p "pSphere1Follicle7109";
	rename -uid "5B35C702-4454-34D5-1CFA-1A8CB0A0E730";
createNode nurbsCurve -n "curveShape166" -p "curve166";
	rename -uid "E5678FC1-4D74-5056-0565-FEAED6FEF85A";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "3F42F8E7-4A69-382B-44A1-DC8ECE837805";
createNode follicle -n "pSphere1FollicleShape7115" -p "pSphere1Follicle7115";
	rename -uid "AA351695-4CA1-C1E8-26D8-ED995585A35F";
	setAttr -k off ".v";
	setAttr ".pu" 0.71875;
	setAttr ".pv" 0.15625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve167" -p "pSphere1Follicle7115";
	rename -uid "59DF94FF-4F3E-4262-55CF-DAAFF836E48E";
createNode nurbsCurve -n "curveShape167" -p "curve167";
	rename -uid "F75D1A36-42C6-D289-BA0A-A4AC2F41D48B";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "C9422045-4174-B378-5460-A1B14BE668B7";
createNode follicle -n "pSphere1FollicleShape7122" -p "pSphere1Follicle7122";
	rename -uid "74AFB7B8-4533-2FC3-B5F2-6CA98A30C988";
	setAttr -k off ".v";
	setAttr ".pu" 0.71875;
	setAttr ".pv" 0.21875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve168" -p "pSphere1Follicle7122";
	rename -uid "D8259577-4A0B-EDBB-8410-4F9EF8BE5752";
createNode nurbsCurve -n "curveShape168" -p "curve168";
	rename -uid "FD27F264-4E7F-9D63-E398-CE9F0D074498";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "7AB3D754-457B-22C3-20DF-AAA198852377";
createNode follicle -n "pSphere1FollicleShape7128" -p "pSphere1Follicle7128";
	rename -uid "E3DFB8C3-4E12-5F24-30B0-208F56310AC5";
	setAttr -k off ".v";
	setAttr ".pu" 0.71875;
	setAttr ".pv" 0.28125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve169" -p "pSphere1Follicle7128";
	rename -uid "13347463-4A84-D94F-126D-BAB1C1490AED";
createNode nurbsCurve -n "curveShape169" -p "curve169";
	rename -uid "FB978171-4C66-B68D-EF3B-68861E441FB8";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "ADAE2441-4F1A-B50E-F0AA-6EBC3EEEFE21";
createNode follicle -n "pSphere1FollicleShape7134" -p "pSphere1Follicle7134";
	rename -uid "9C46AB89-4FD2-1BA4-837E-CEAEDA2EE23B";
	setAttr -k off ".v";
	setAttr ".pu" 0.71875;
	setAttr ".pv" 0.34375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve170" -p "pSphere1Follicle7134";
	rename -uid "9019CB74-4D32-2C55-7C08-F28452742BBA";
createNode nurbsCurve -n "curveShape170" -p "curve170";
	rename -uid "CF9C9D08-40F9-794D-D396-ADA372FAAE54";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "73745EC4-41EC-ABB5-0190-A68B78D885CE";
createNode follicle -n "pSphere1FollicleShape7140" -p "pSphere1Follicle7140";
	rename -uid "395A3C15-4923-389B-71D2-5FA1E07865D7";
	setAttr -k off ".v";
	setAttr ".pu" 0.71875;
	setAttr ".pv" 0.40625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve171" -p "pSphere1Follicle7140";
	rename -uid "CDF4CA9F-4512-735F-6BC4-E591FEEFBB0C";
createNode nurbsCurve -n "curveShape171" -p "curve171";
	rename -uid "64D7F5BB-486F-06FB-6CD9-E2B97A3B2A06";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "B2F71D95-4E44-5B32-2BE8-2CB0BBA9F013";
createNode follicle -n "pSphere1FollicleShape7146" -p "pSphere1Follicle7146";
	rename -uid "029E7959-4E4F-FEBF-BEC3-708087AC5E96";
	setAttr -k off ".v";
	setAttr ".pu" 0.71875;
	setAttr ".pv" 0.46875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve172" -p "pSphere1Follicle7146";
	rename -uid "BB64E38F-4E86-4F78-2D1B-4B8E1E450D2A";
createNode nurbsCurve -n "curveShape172" -p "curve172";
	rename -uid "A78266B2-4EF4-1B49-5732-70A038B2F787";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "B72C1B25-43EE-576B-160A-6992495F2FC8";
createNode follicle -n "pSphere1FollicleShape7153" -p "pSphere1Follicle7153";
	rename -uid "E5056B46-46FC-8C0A-2D56-AF80A4F4FDAF";
	setAttr -k off ".v";
	setAttr ".pu" 0.71875;
	setAttr ".pv" 0.53125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve173" -p "pSphere1Follicle7153";
	rename -uid "85B8E329-499F-5CA2-7C6C-8A90698CC359";
createNode nurbsCurve -n "curveShape173" -p "curve173";
	rename -uid "9E931931-4672-7960-4020-03ADE2A9B4A7";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "365218E0-4852-CE14-67F8-A497BCAACAE3";
createNode follicle -n "pSphere1FollicleShape7159" -p "pSphere1Follicle7159";
	rename -uid "64384589-4669-F1F5-F70E-5EB0D1B29142";
	setAttr -k off ".v";
	setAttr ".pu" 0.71875;
	setAttr ".pv" 0.59375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve174" -p "pSphere1Follicle7159";
	rename -uid "5D1EFB32-4AC0-94B9-9759-FE9D9AD923A6";
createNode nurbsCurve -n "curveShape174" -p "curve174";
	rename -uid "A72F8A08-4BF8-A7D2-45E4-4E8B7653E505";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "21E3CA37-4DB9-621B-0575-92A13FBF8061";
createNode follicle -n "pSphere1FollicleShape7165" -p "pSphere1Follicle7165";
	rename -uid "ADFFF9E7-4765-CFED-BBC6-949845A9BFCB";
	setAttr -k off ".v";
	setAttr ".pu" 0.71875;
	setAttr ".pv" 0.65625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve175" -p "pSphere1Follicle7165";
	rename -uid "49E0F796-4E0D-A56C-0557-8EBA3929499C";
createNode nurbsCurve -n "curveShape175" -p "curve175";
	rename -uid "1C5DF311-4836-32C5-DA98-BF94004328B6";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "F4DB21AA-4D1A-8F8F-3FE6-53A018B2641C";
createNode follicle -n "pSphere1FollicleShape7171" -p "pSphere1Follicle7171";
	rename -uid "E8C4F870-4E26-8D25-575B-0EB9872C278C";
	setAttr -k off ".v";
	setAttr ".pu" 0.71875;
	setAttr ".pv" 0.71875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve176" -p "pSphere1Follicle7171";
	rename -uid "E48DE819-4380-1809-3029-F98F4DEB3FD0";
createNode nurbsCurve -n "curveShape176" -p "curve176";
	rename -uid "F6307AB2-4C28-8F7C-FABD-03A1EFBC2D9B";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "26D44507-4375-C3BE-73BB-92BBD74157C4";
createNode follicle -n "pSphere1FollicleShape7177" -p "pSphere1Follicle7177";
	rename -uid "3163F4DA-4027-22A4-F9DF-62A8149A06CE";
	setAttr -k off ".v";
	setAttr ".pu" 0.71875;
	setAttr ".pv" 0.78125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve177" -p "pSphere1Follicle7177";
	rename -uid "CC8E2C7B-4874-7FD5-81DE-CA9DF05E6DEA";
createNode nurbsCurve -n "curveShape177" -p "curve177";
	rename -uid "F53BEC3E-44AF-1E51-8F2A-AF9A3BECBCC8";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "181C6701-465E-7962-176D-0FAA05B709E0";
createNode follicle -n "pSphere1FollicleShape7184" -p "pSphere1Follicle7184";
	rename -uid "E53C8C6E-40F7-BD84-92F0-439AC43D5D91";
	setAttr -k off ".v";
	setAttr ".pu" 0.71875;
	setAttr ".pv" 0.84375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve178" -p "pSphere1Follicle7184";
	rename -uid "11589CEC-439A-AE00-6064-6EB762203885";
createNode nurbsCurve -n "curveShape178" -p "curve178";
	rename -uid "0D73DA26-4465-A9EC-ED0C-788F5BF3AD77";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "89A0A4DA-4C10-3FDA-D8FE-1290CEF280E2";
createNode follicle -n "pSphere1FollicleShape7190" -p "pSphere1Follicle7190";
	rename -uid "0F560D76-4940-0776-AE8A-3A8B5FB356F3";
	setAttr -k off ".v";
	setAttr ".pu" 0.71875;
	setAttr ".pv" 0.90625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve179" -p "pSphere1Follicle7190";
	rename -uid "6EB0C78F-42E1-3ED2-D682-60B82B17FB77";
createNode nurbsCurve -n "curveShape179" -p "curve179";
	rename -uid "BBF3EF75-4C9E-EA33-F0E3-0CA53B63A05F";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "5B8DF337-49CB-3E55-E149-CB9EADC06EE3";
createNode follicle -n "pSphere1FollicleShape7196" -p "pSphere1Follicle7196";
	rename -uid "A7F347BF-4B71-B890-D188-9DBA09739BD0";
	setAttr -k off ".v";
	setAttr ".pu" 0.71875;
	setAttr ".pv" 0.96875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve180" -p "pSphere1Follicle7196";
	rename -uid "AE6D3A6D-4427-D988-FFFD-9B9137C89E5B";
createNode nurbsCurve -n "curveShape180" -p "curve180";
	rename -uid "6979C91E-4199-C107-6F4A-AF8F3CDD4F0E";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "C9349FA7-48E5-3B03-8865-FFAB949DAC04";
createNode follicle -n "pSphere1FollicleShape7703" -p "pSphere1Follicle7703";
	rename -uid "13625DE2-453D-02EF-7D57-2D90FFA3CE84";
	setAttr -k off ".v";
	setAttr ".pu" 0.78125;
	setAttr ".pv" 0.03125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve181" -p "pSphere1Follicle7703";
	rename -uid "3ADABA36-40A5-AAE3-B319-5986D790401C";
createNode nurbsCurve -n "curveShape181" -p "curve181";
	rename -uid "97D1ECA9-4E05-A7EA-065A-B58360A6CAD2";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "193260E3-47E8-1FCA-ECF6-58AA9FDD4BCB";
createNode follicle -n "pSphere1FollicleShape7709" -p "pSphere1Follicle7709";
	rename -uid "F2895910-4462-8B47-B85D-43BF1C24A5D6";
	setAttr -k off ".v";
	setAttr ".pu" 0.78125;
	setAttr ".pv" 0.09375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve182" -p "pSphere1Follicle7709";
	rename -uid "8869B5BF-4408-1CF2-7ADD-FE8ABBE2445B";
createNode nurbsCurve -n "curveShape182" -p "curve182";
	rename -uid "934C8D66-4612-9DE6-6074-D4933DA09C47";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "9023A10A-4365-C023-24C1-C5B83D7210C0";
createNode follicle -n "pSphere1FollicleShape7715" -p "pSphere1Follicle7715";
	rename -uid "AF40505F-4420-D3AF-570A-11A2233321B0";
	setAttr -k off ".v";
	setAttr ".pu" 0.78125;
	setAttr ".pv" 0.15625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve183" -p "pSphere1Follicle7715";
	rename -uid "9FB62C0D-404E-B7EE-B64C-4087F0E6F1A3";
createNode nurbsCurve -n "curveShape183" -p "curve183";
	rename -uid "84034D13-4A5B-2AAD-E2FC-F7A88A4B2195";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "A5100D44-4AB5-3398-0695-97AABD9C0A1F";
createNode follicle -n "pSphere1FollicleShape7722" -p "pSphere1Follicle7722";
	rename -uid "F8835E95-4397-0D35-B27A-D2A5B5F8D6F5";
	setAttr -k off ".v";
	setAttr ".pu" 0.78125;
	setAttr ".pv" 0.21875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve184" -p "pSphere1Follicle7722";
	rename -uid "F888C6B6-4FD8-00F4-DAA6-4CAC9CC6E684";
createNode nurbsCurve -n "curveShape184" -p "curve184";
	rename -uid "0F9607FC-415C-1362-89E5-7CBD0EB41316";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "3EEDD46E-4661-20AD-A34A-D799BABDA8DC";
createNode follicle -n "pSphere1FollicleShape7728" -p "pSphere1Follicle7728";
	rename -uid "C8DB2CA4-40AC-8BA6-2220-5EB262ADE74B";
	setAttr -k off ".v";
	setAttr ".pu" 0.78125;
	setAttr ".pv" 0.28125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve185" -p "pSphere1Follicle7728";
	rename -uid "0174407D-4246-9419-EC1C-248E07E9343F";
createNode nurbsCurve -n "curveShape185" -p "curve185";
	rename -uid "65016918-4DF4-D897-C70E-99BA07352024";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "D072D4E3-42D2-5A19-295E-53A2C714A633";
createNode follicle -n "pSphere1FollicleShape7734" -p "pSphere1Follicle7734";
	rename -uid "B282C9C6-4F21-9B3C-4569-96AD05A6AB49";
	setAttr -k off ".v";
	setAttr ".pu" 0.78125;
	setAttr ".pv" 0.34375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve186" -p "pSphere1Follicle7734";
	rename -uid "9AAFD6FF-48A6-0026-C2E1-E599187037DC";
createNode nurbsCurve -n "curveShape186" -p "curve186";
	rename -uid "B994B5EB-4E2F-BDDA-1CB9-DCA481218F96";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "BDF99B70-486A-4C8A-4C3F-F3AB2F3935E7";
createNode follicle -n "pSphere1FollicleShape7740" -p "pSphere1Follicle7740";
	rename -uid "B65597A9-484D-A119-740A-AEB80A080709";
	setAttr -k off ".v";
	setAttr ".pu" 0.78125;
	setAttr ".pv" 0.40625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve187" -p "pSphere1Follicle7740";
	rename -uid "C2A121E9-4BA3-555C-6F6C-F9B4DBEA7BB3";
createNode nurbsCurve -n "curveShape187" -p "curve187";
	rename -uid "E28366EC-4DDF-36F9-8DD8-04BDC7BE2E2C";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "5F7DBD08-4890-78FD-3BE9-0BB67C9A3C30";
createNode follicle -n "pSphere1FollicleShape7746" -p "pSphere1Follicle7746";
	rename -uid "658DFBA3-40D6-81D1-7C8C-778F98884430";
	setAttr -k off ".v";
	setAttr ".pu" 0.78125;
	setAttr ".pv" 0.46875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve188" -p "pSphere1Follicle7746";
	rename -uid "886D5C00-45DD-8D32-8568-3B9A6B297627";
createNode nurbsCurve -n "curveShape188" -p "curve188";
	rename -uid "07B6B8B8-489D-F8F0-46C3-ECBAF41989E0";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "547741EC-4622-2694-5124-068D0CC87054";
createNode follicle -n "pSphere1FollicleShape7753" -p "pSphere1Follicle7753";
	rename -uid "E2808399-4DA8-BE9D-3766-0EB558FD3A46";
	setAttr -k off ".v";
	setAttr ".pu" 0.78125;
	setAttr ".pv" 0.53125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve189" -p "pSphere1Follicle7753";
	rename -uid "B22EB1B1-4284-19CB-345F-2EA943FA4EF8";
createNode nurbsCurve -n "curveShape189" -p "curve189";
	rename -uid "B43F2B40-44ED-8A6C-8A0A-C59D042A193C";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "05234A58-4640-65AE-B969-04A992D37697";
createNode follicle -n "pSphere1FollicleShape7759" -p "pSphere1Follicle7759";
	rename -uid "A54A9BE5-4A40-1CD2-7454-559C681916CD";
	setAttr -k off ".v";
	setAttr ".pu" 0.78125;
	setAttr ".pv" 0.59375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve190" -p "pSphere1Follicle7759";
	rename -uid "50934D8C-4F3E-6E4C-1E58-63A31369BA31";
createNode nurbsCurve -n "curveShape190" -p "curve190";
	rename -uid "90FF079B-42B9-2FAB-4BE2-D7AF7D89F5B7";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "BF387FD0-4B49-25AA-4B09-CA988172FDE9";
createNode follicle -n "pSphere1FollicleShape7765" -p "pSphere1Follicle7765";
	rename -uid "D7A94365-4BA1-CA86-75E6-4F98212E6055";
	setAttr -k off ".v";
	setAttr ".pu" 0.78125;
	setAttr ".pv" 0.65625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve191" -p "pSphere1Follicle7765";
	rename -uid "E407847E-4AED-7719-C62A-73A9C45CBC4D";
createNode nurbsCurve -n "curveShape191" -p "curve191";
	rename -uid "7700FCC2-45BF-F6D6-57DB-0E8F061E43A4";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "88B1D052-4C75-97BC-BF73-9A974630F05D";
createNode follicle -n "pSphere1FollicleShape7771" -p "pSphere1Follicle7771";
	rename -uid "2E2EF4B0-464C-28AA-FED1-94ADEA4226A5";
	setAttr -k off ".v";
	setAttr ".pu" 0.78125;
	setAttr ".pv" 0.71875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve192" -p "pSphere1Follicle7771";
	rename -uid "3138F42F-41C0-28B6-8715-3180E928618F";
createNode nurbsCurve -n "curveShape192" -p "curve192";
	rename -uid "F6A79579-4D29-434C-9718-1B9225D6C796";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "F0F40C45-4FB9-D385-9216-709ED9FF8629";
createNode follicle -n "pSphere1FollicleShape7777" -p "pSphere1Follicle7777";
	rename -uid "9A79485C-45EB-57A0-9240-EBA7E55527C3";
	setAttr -k off ".v";
	setAttr ".pu" 0.78125;
	setAttr ".pv" 0.78125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve193" -p "pSphere1Follicle7777";
	rename -uid "AAC07D60-4031-CEF7-CD84-74999096FEF5";
createNode nurbsCurve -n "curveShape193" -p "curve193";
	rename -uid "AC4D5992-4232-D341-2C59-15BFA1E3F78F";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "570830B6-4CED-4277-A4BD-F4A3B20AF9E7";
createNode follicle -n "pSphere1FollicleShape7784" -p "pSphere1Follicle7784";
	rename -uid "C03CD75D-410D-95A2-05FD-089E7A08EC58";
	setAttr -k off ".v";
	setAttr ".pu" 0.78125;
	setAttr ".pv" 0.84375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve194" -p "pSphere1Follicle7784";
	rename -uid "D06EEB0B-496F-2743-9F29-81980A5B539E";
createNode nurbsCurve -n "curveShape194" -p "curve194";
	rename -uid "B9AE1B50-442F-C574-D532-6591502F89C0";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "25B95940-4294-7D20-F4B1-1FABEB1C1FD5";
createNode follicle -n "pSphere1FollicleShape7790" -p "pSphere1Follicle7790";
	rename -uid "7E2BB225-470A-5595-964B-AD8E2838906E";
	setAttr -k off ".v";
	setAttr ".pu" 0.78125;
	setAttr ".pv" 0.90625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve195" -p "pSphere1Follicle7790";
	rename -uid "88B6287F-4DB4-AA7E-7511-FBB9F6E1824F";
createNode nurbsCurve -n "curveShape195" -p "curve195";
	rename -uid "34D6C8D4-4573-F424-7D58-52BBFDC4398B";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "01AFD0F2-4B31-AEB9-8788-A3A383315CF9";
createNode follicle -n "pSphere1FollicleShape7796" -p "pSphere1Follicle7796";
	rename -uid "290A4309-4FC4-BB74-07D9-1B83A3A1B2F4";
	setAttr -k off ".v";
	setAttr ".pu" 0.78125;
	setAttr ".pv" 0.96875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve196" -p "pSphere1Follicle7796";
	rename -uid "82D50A49-48ED-B23D-77D6-DCBB19C2B662";
createNode nurbsCurve -n "curveShape196" -p "curve196";
	rename -uid "FEED6ECD-4227-1CCB-749D-08B7284635F4";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "9A4C081C-48DE-B9C2-31C7-61B50F32E9E9";
createNode follicle -n "pSphere1FollicleShape8409" -p "pSphere1Follicle8409";
	rename -uid "E8E71FB2-4F46-5B1C-B0D9-CA925CD37CF6";
	setAttr -k off ".v";
	setAttr ".pu" 0.84375;
	setAttr ".pv" 0.09375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve197" -p "pSphere1Follicle8409";
	rename -uid "1941A3B7-4F51-7327-4FA9-84B3E1F108F3";
createNode nurbsCurve -n "curveShape197" -p "curve197";
	rename -uid "727CA4A0-47DE-0682-D33B-B9B691FCE83F";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "7BA83C35-4301-BCC1-120E-7DB23DD23C64";
createNode follicle -n "pSphere1FollicleShape8415" -p "pSphere1Follicle8415";
	rename -uid "D745D122-4F9C-3206-8CD1-B7878EDBD8AE";
	setAttr -k off ".v";
	setAttr ".pu" 0.84375;
	setAttr ".pv" 0.15625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve198" -p "pSphere1Follicle8415";
	rename -uid "F0B703B5-4275-1BD8-B775-28A9CCABA228";
createNode nurbsCurve -n "curveShape198" -p "curve198";
	rename -uid "CD1AD74C-4613-92CB-1718-BA88947A69CC";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "E52CB879-4D01-E772-01E3-0784D6F5F926";
createNode follicle -n "pSphere1FollicleShape8422" -p "pSphere1Follicle8422";
	rename -uid "BF69D028-4D3C-E80A-8588-BCB3A786A9AB";
	setAttr -k off ".v";
	setAttr ".pu" 0.84375;
	setAttr ".pv" 0.21875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve199" -p "pSphere1Follicle8422";
	rename -uid "D3B0042E-4723-58BF-2852-1BB4121A847C";
createNode nurbsCurve -n "curveShape199" -p "curve199";
	rename -uid "BB2268AD-4473-AF5E-B460-E3B0AFAF4339";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "02DF0B26-4E1E-9FCB-A363-0F9E3C5678E8";
createNode follicle -n "pSphere1FollicleShape8428" -p "pSphere1Follicle8428";
	rename -uid "558FBFCC-4827-6909-5F89-8FB5F0BE118F";
	setAttr -k off ".v";
	setAttr ".pu" 0.84375;
	setAttr ".pv" 0.28125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve200" -p "pSphere1Follicle8428";
	rename -uid "65A01F5C-4A9E-060F-0110-19A1C00F4359";
createNode nurbsCurve -n "curveShape200" -p "curve200";
	rename -uid "D6AE92D6-4B49-60F7-0A6D-19968ABEDDF8";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "8B9C6B17-4F3E-CE34-F98A-ADBEC0DE3F57";
createNode follicle -n "pSphere1FollicleShape8434" -p "pSphere1Follicle8434";
	rename -uid "04429C56-4B74-C530-88D5-D9894383D439";
	setAttr -k off ".v";
	setAttr ".pu" 0.84375;
	setAttr ".pv" 0.34375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve201" -p "pSphere1Follicle8434";
	rename -uid "42880F9E-4C6D-E5BC-6059-108F3124D161";
createNode nurbsCurve -n "curveShape201" -p "curve201";
	rename -uid "6D0A23B2-41A4-1F3A-F183-E296808EE56F";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "89BDAACE-4AA8-5E21-025F-A08B8B761274";
createNode follicle -n "pSphere1FollicleShape8440" -p "pSphere1Follicle8440";
	rename -uid "F38AF89D-449D-C9BD-2FFD-C399D966F104";
	setAttr -k off ".v";
	setAttr ".pu" 0.84375;
	setAttr ".pv" 0.40625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve202" -p "pSphere1Follicle8440";
	rename -uid "1865BE08-4948-7693-777A-D7BC00792635";
createNode nurbsCurve -n "curveShape202" -p "curve202";
	rename -uid "7F994E60-4E72-1FB7-405B-64B783648A5B";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "63B58291-4587-F578-84B3-84ABA022F1E1";
createNode follicle -n "pSphere1FollicleShape8446" -p "pSphere1Follicle8446";
	rename -uid "5FD06DED-494C-F11C-1674-2A8F81CCD96B";
	setAttr -k off ".v";
	setAttr ".pu" 0.84375;
	setAttr ".pv" 0.46875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve203" -p "pSphere1Follicle8446";
	rename -uid "DF0AD929-433B-58D2-E55A-148411C630BB";
createNode nurbsCurve -n "curveShape203" -p "curve203";
	rename -uid "349C9CC4-4CE4-26E2-2029-549F594BCB6B";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "EA87D129-4BB3-3CD8-72A1-D6A1D3B21C6E";
createNode follicle -n "pSphere1FollicleShape8453" -p "pSphere1Follicle8453";
	rename -uid "568DF689-4F2D-BA65-24B6-48A773F19A48";
	setAttr -k off ".v";
	setAttr ".pu" 0.84375;
	setAttr ".pv" 0.53125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve204" -p "pSphere1Follicle8453";
	rename -uid "67B21D35-43C0-27CC-F090-C99FFBDB6F94";
createNode nurbsCurve -n "curveShape204" -p "curve204";
	rename -uid "01C8322E-44AA-D026-58E5-DCBE2EDCAF93";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "884408F0-49DD-E758-2D94-5EB7B2833A28";
createNode follicle -n "pSphere1FollicleShape8459" -p "pSphere1Follicle8459";
	rename -uid "2DE8DE5F-4CAC-52B1-1ED5-CA9859C741F5";
	setAttr -k off ".v";
	setAttr ".pu" 0.84375;
	setAttr ".pv" 0.59375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve205" -p "pSphere1Follicle8459";
	rename -uid "23C17F95-4A9D-86E9-93FB-4489978AC604";
createNode nurbsCurve -n "curveShape205" -p "curve205";
	rename -uid "33371566-497A-EE4E-EDE1-34AEE0F6DC3F";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "3AF1E9E5-4D6C-8218-6F0F-BF9993E53782";
createNode follicle -n "pSphere1FollicleShape8465" -p "pSphere1Follicle8465";
	rename -uid "27F42458-4CA6-0D97-9105-51AC9A5DEF6B";
	setAttr -k off ".v";
	setAttr ".pu" 0.84375;
	setAttr ".pv" 0.65625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve206" -p "pSphere1Follicle8465";
	rename -uid "9705B2D7-40AC-AFAA-E082-07B8CC165D73";
createNode nurbsCurve -n "curveShape206" -p "curve206";
	rename -uid "9804D065-4330-8485-6A00-5BB3CB1D4247";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "FA7197D6-4AD8-C5DC-7BF7-D5AB46E1882E";
createNode follicle -n "pSphere1FollicleShape8471" -p "pSphere1Follicle8471";
	rename -uid "1B1DA498-44D6-C16D-A7A5-2CA42118CE20";
	setAttr -k off ".v";
	setAttr ".pu" 0.84375;
	setAttr ".pv" 0.71875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve207" -p "pSphere1Follicle8471";
	rename -uid "984A77D8-4791-1508-808B-598A07B50252";
createNode nurbsCurve -n "curveShape207" -p "curve207";
	rename -uid "C959CF84-4D64-8DAC-630E-509A4D2D3BDA";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "1EB0A09C-4E89-D184-B3C4-8FA2C9F02B58";
createNode follicle -n "pSphere1FollicleShape8477" -p "pSphere1Follicle8477";
	rename -uid "9708F8CC-485E-B9AC-8B40-EEAD26C17304";
	setAttr -k off ".v";
	setAttr ".pu" 0.84375;
	setAttr ".pv" 0.78125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve208" -p "pSphere1Follicle8477";
	rename -uid "DE8A1F56-450E-D91B-4A6A-B5AC88796800";
createNode nurbsCurve -n "curveShape208" -p "curve208";
	rename -uid "45BFAB42-4EA8-CBF6-1DD3-39B55E1554B8";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "AB9CADB7-4BF5-2E23-EA41-13BBC9BD0E8F";
createNode follicle -n "pSphere1FollicleShape8484" -p "pSphere1Follicle8484";
	rename -uid "62193763-4EE9-9B2F-20B4-FAAAB9100CAD";
	setAttr -k off ".v";
	setAttr ".pu" 0.84375;
	setAttr ".pv" 0.84375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve209" -p "pSphere1Follicle8484";
	rename -uid "F704350E-46A0-622C-EB84-0AB91889E9AE";
createNode nurbsCurve -n "curveShape209" -p "curve209";
	rename -uid "0D8A9A43-4666-E613-6449-5EB326FDB481";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "9C46EAA9-4CB2-F5D6-DF5F-2DAAD198CBBA";
createNode follicle -n "pSphere1FollicleShape8490" -p "pSphere1Follicle8490";
	rename -uid "3B893F65-4813-9312-0E85-C4B5E0C01215";
	setAttr -k off ".v";
	setAttr ".pu" 0.84375;
	setAttr ".pv" 0.90625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve210" -p "pSphere1Follicle8490";
	rename -uid "A9A46E30-4A37-A01E-D682-3894F1FCA32E";
createNode nurbsCurve -n "curveShape210" -p "curve210";
	rename -uid "28147975-4D32-A2A9-9E0B-8DB8FAD5CBA4";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "D7C18849-4529-1D71-5B2F-ED84A06D0314";
createNode follicle -n "pSphere1FollicleShape9009" -p "pSphere1Follicle9009";
	rename -uid "1B2C53D4-46D9-B58C-44BF-1B95D0DE3A67";
	setAttr -k off ".v";
	setAttr ".pu" 0.90625;
	setAttr ".pv" 0.09375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve211" -p "pSphere1Follicle9009";
	rename -uid "16374BEB-476E-AFBA-3204-138AD955423E";
createNode nurbsCurve -n "curveShape211" -p "curve211";
	rename -uid "35B0EE91-4BAE-78AC-54B1-4FA0DA9C4A33";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "8A356E90-40A8-A8D9-65E9-A6ACA7B2B38A";
createNode follicle -n "pSphere1FollicleShape9015" -p "pSphere1Follicle9015";
	rename -uid "D7559079-4781-D1F2-0D59-C2A8B2B48FE6";
	setAttr -k off ".v";
	setAttr ".pu" 0.90625;
	setAttr ".pv" 0.15625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve212" -p "pSphere1Follicle9015";
	rename -uid "1D969832-40EE-E4BB-97E4-EDBF4B333060";
createNode nurbsCurve -n "curveShape212" -p "curve212";
	rename -uid "24F77BA5-4763-6D59-9705-E6911836976C";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "215B530B-4A31-D25D-1414-62B3B31C7533";
createNode follicle -n "pSphere1FollicleShape9022" -p "pSphere1Follicle9022";
	rename -uid "838EEE30-4265-3548-2171-B69DDD9FAEF9";
	setAttr -k off ".v";
	setAttr ".pu" 0.90625;
	setAttr ".pv" 0.21875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve213" -p "pSphere1Follicle9022";
	rename -uid "DDE6D6B2-4150-2090-CC4A-E1BA96468C1E";
createNode nurbsCurve -n "curveShape213" -p "curve213";
	rename -uid "FB3E3504-44D2-3938-BD2B-EF98094FBAF1";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "36B28576-4371-8397-9BDB-C69BB89A7212";
createNode follicle -n "pSphere1FollicleShape9028" -p "pSphere1Follicle9028";
	rename -uid "9290B0EF-4CA8-81A4-AE80-4397C1CD9EA6";
	setAttr -k off ".v";
	setAttr ".pu" 0.90625;
	setAttr ".pv" 0.28125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve214" -p "pSphere1Follicle9028";
	rename -uid "697BF9EA-4455-16BF-EA20-D4B49E902285";
createNode nurbsCurve -n "curveShape214" -p "curve214";
	rename -uid "EE7669BC-43E3-F43C-B577-B79BF878E9C5";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "29A27560-4D36-1695-A3F7-E097079E665F";
createNode follicle -n "pSphere1FollicleShape9034" -p "pSphere1Follicle9034";
	rename -uid "0404EF2C-4F28-CE3D-0489-7A9FD6D5FE7A";
	setAttr -k off ".v";
	setAttr ".pu" 0.90625;
	setAttr ".pv" 0.34375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve215" -p "pSphere1Follicle9034";
	rename -uid "43ABB097-499E-A4D6-FDF5-58A0D9FDECA2";
createNode nurbsCurve -n "curveShape215" -p "curve215";
	rename -uid "B533997E-4C2C-212E-3984-269DD359C9E6";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "5252B80A-449A-A8F3-DFA0-BA9F72D90385";
createNode follicle -n "pSphere1FollicleShape9040" -p "pSphere1Follicle9040";
	rename -uid "C8BCF88E-4605-163B-53FC-ED8B616847B5";
	setAttr -k off ".v";
	setAttr ".pu" 0.90625;
	setAttr ".pv" 0.40625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve216" -p "pSphere1Follicle9040";
	rename -uid "BB746559-42BF-F420-99A1-77BBC961BACF";
createNode nurbsCurve -n "curveShape216" -p "curve216";
	rename -uid "D9CC48F7-4D41-69A5-25D0-8CA910CE17CB";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "A76E8F1B-42CD-25AA-A223-65A124EDE198";
createNode follicle -n "pSphere1FollicleShape9046" -p "pSphere1Follicle9046";
	rename -uid "248DDAB8-47BD-463E-DDFC-9E992A6C7E6F";
	setAttr -k off ".v";
	setAttr ".pu" 0.90625;
	setAttr ".pv" 0.46875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve217" -p "pSphere1Follicle9046";
	rename -uid "BF92E77D-4AC8-0D8F-C4FB-37A71EF8CC23";
createNode nurbsCurve -n "curveShape217" -p "curve217";
	rename -uid "24EEA71A-4A7C-CB42-FB7F-A292624D8BB7";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "C5C753B7-4DA6-D35B-F065-C98EC58FE5EA";
createNode follicle -n "pSphere1FollicleShape9053" -p "pSphere1Follicle9053";
	rename -uid "1768FC7A-42B6-C9AA-770F-6D8420A610B0";
	setAttr -k off ".v";
	setAttr ".pu" 0.90625;
	setAttr ".pv" 0.53125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve218" -p "pSphere1Follicle9053";
	rename -uid "9C34A4E9-42FA-379E-B677-6DB4E18F6E82";
createNode nurbsCurve -n "curveShape218" -p "curve218";
	rename -uid "5465B0A5-4CAB-DC58-5B46-878C1183CA84";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "65DE59C4-4474-1F88-1698-9E82AEC79839";
createNode follicle -n "pSphere1FollicleShape9059" -p "pSphere1Follicle9059";
	rename -uid "BD03D0F1-4802-B034-1B8E-A3B3DD800D29";
	setAttr -k off ".v";
	setAttr ".pu" 0.90625;
	setAttr ".pv" 0.59375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve219" -p "pSphere1Follicle9059";
	rename -uid "3E053B5F-44F8-D8EA-1EEF-F58417BD1BEA";
createNode nurbsCurve -n "curveShape219" -p "curve219";
	rename -uid "20D4099A-4782-27E5-BC84-32AD9C8100EF";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "D0398345-4FB6-62AF-BD71-0081D57018B1";
createNode follicle -n "pSphere1FollicleShape9065" -p "pSphere1Follicle9065";
	rename -uid "DA130DAC-49D6-8D45-817A-35AD2C955C49";
	setAttr -k off ".v";
	setAttr ".pu" 0.90625;
	setAttr ".pv" 0.65625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve220" -p "pSphere1Follicle9065";
	rename -uid "9FDFF12E-466D-F370-A172-9FA077D2E4BF";
createNode nurbsCurve -n "curveShape220" -p "curve220";
	rename -uid "9557092A-4A63-AE15-0E12-C3835988B231";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "F85E6D55-4E78-FDA4-A995-6CB9C560644C";
createNode follicle -n "pSphere1FollicleShape9071" -p "pSphere1Follicle9071";
	rename -uid "4E025193-40C2-7992-68FB-E487504B4FC2";
	setAttr -k off ".v";
	setAttr ".pu" 0.90625;
	setAttr ".pv" 0.71875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve221" -p "pSphere1Follicle9071";
	rename -uid "A1751D08-4354-5C44-E74F-6487943318C1";
createNode nurbsCurve -n "curveShape221" -p "curve221";
	rename -uid "41DFFA87-4091-5AE4-B935-73A687FE9AC9";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "D7EDA99A-4DF5-20C8-50A0-83A7D4F0C3E0";
createNode follicle -n "pSphere1FollicleShape9077" -p "pSphere1Follicle9077";
	rename -uid "352E0770-4203-10DE-6C4F-AD961FE8D70A";
	setAttr -k off ".v";
	setAttr ".pu" 0.90625;
	setAttr ".pv" 0.78125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve222" -p "pSphere1Follicle9077";
	rename -uid "95BF97BB-4A61-D080-7398-C2A47E286C59";
createNode nurbsCurve -n "curveShape222" -p "curve222";
	rename -uid "F94D6930-42AD-E439-219E-62AACC7BC401";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "39436E58-4088-DA5B-92E5-2E92F62CB13B";
createNode follicle -n "pSphere1FollicleShape9084" -p "pSphere1Follicle9084";
	rename -uid "135218B4-48D9-B05F-37C3-CA97CEBDE4EB";
	setAttr -k off ".v";
	setAttr ".pu" 0.90625;
	setAttr ".pv" 0.84375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve223" -p "pSphere1Follicle9084";
	rename -uid "B012EFB3-425D-BB27-0991-20ACD735D89D";
createNode nurbsCurve -n "curveShape223" -p "curve223";
	rename -uid "4850D811-4C00-CCB1-5AE1-17839D65E923";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "A45DC573-4C02-3F56-DA69-F185C544DA6B";
createNode follicle -n "pSphere1FollicleShape9090" -p "pSphere1Follicle9090";
	rename -uid "ADF08685-4DFE-41F8-1771-AE8B966214F2";
	setAttr -k off ".v";
	setAttr ".pu" 0.90625;
	setAttr ".pv" 0.90625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve224" -p "pSphere1Follicle9090";
	rename -uid "471157B7-4265-7083-F77E-18B3F25CB3E0";
createNode nurbsCurve -n "curveShape224" -p "curve224";
	rename -uid "F05AE270-40EC-30A2-E3B1-D4A35B04BDBA";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "A763FEEB-4D9C-8E36-3B5E-91A3A28CA360";
createNode follicle -n "pSphere1FollicleShape9603" -p "pSphere1Follicle9603";
	rename -uid "86A0BD73-48AE-EC67-7612-96B28DEDBF46";
	setAttr -k off ".v";
	setAttr ".pu" 0.96875;
	setAttr ".pv" 0.03125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve225" -p "pSphere1Follicle9603";
	rename -uid "91699A1D-4C17-CDDA-5668-BBAF7F85B7BC";
createNode nurbsCurve -n "curveShape225" -p "curve225";
	rename -uid "70356725-4DE8-9FFC-4AA9-94BF1912EDD4";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "A8353040-46DA-284E-1FF5-3CA4C6AABFFE";
createNode follicle -n "pSphere1FollicleShape9609" -p "pSphere1Follicle9609";
	rename -uid "F92A800F-442A-6C33-8F83-68AE8D8B2FF8";
	setAttr -k off ".v";
	setAttr ".pu" 0.96875;
	setAttr ".pv" 0.09375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve226" -p "pSphere1Follicle9609";
	rename -uid "9588270C-41EF-B8F1-0163-78977CF8ECB0";
createNode nurbsCurve -n "curveShape226" -p "curve226";
	rename -uid "21F0CD93-4BDE-2D46-E859-EE94B0194B0F";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "F769EB93-47F6-888A-CFEB-9DAC678C4AEC";
createNode follicle -n "pSphere1FollicleShape9615" -p "pSphere1Follicle9615";
	rename -uid "221C6030-4C51-BB80-E758-3F9CB65C8EAF";
	setAttr -k off ".v";
	setAttr ".pu" 0.96875;
	setAttr ".pv" 0.15625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve227" -p "pSphere1Follicle9615";
	rename -uid "D0536A7A-4076-66BF-2265-DBA33175CD53";
createNode nurbsCurve -n "curveShape227" -p "curve227";
	rename -uid "96DE8F71-4269-2927-8484-E4AE7932B842";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "71717B5C-43F1-B867-0B77-9F8FAE4D7443";
createNode follicle -n "pSphere1FollicleShape9622" -p "pSphere1Follicle9622";
	rename -uid "3EDD2F55-4D33-5826-41F5-8980244418C2";
	setAttr -k off ".v";
	setAttr ".pu" 0.96875;
	setAttr ".pv" 0.21875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve228" -p "pSphere1Follicle9622";
	rename -uid "E5E274F9-4E37-0908-215A-1285BC9ACA7C";
createNode nurbsCurve -n "curveShape228" -p "curve228";
	rename -uid "82AD7ABA-489B-543B-2B3C-5DAE9073E15C";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "CBE43342-4F79-0143-FCFB-899A322ED0E7";
createNode follicle -n "pSphere1FollicleShape9628" -p "pSphere1Follicle9628";
	rename -uid "0E154A2A-400C-54B2-0972-31BAA77E4EFE";
	setAttr -k off ".v";
	setAttr ".pu" 0.96875;
	setAttr ".pv" 0.28125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve229" -p "pSphere1Follicle9628";
	rename -uid "4D5BD3F2-4B64-FA12-49F1-4896D8752185";
createNode nurbsCurve -n "curveShape229" -p "curve229";
	rename -uid "CFAF415A-42D5-75DB-368F-8AB6E82508FA";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "AFF2CCD2-4012-A9C9-2FB8-03A5284BBF5D";
createNode follicle -n "pSphere1FollicleShape9634" -p "pSphere1Follicle9634";
	rename -uid "5D77C82E-4CD8-2A09-5EA6-61AB8F6016A3";
	setAttr -k off ".v";
	setAttr ".pu" 0.96875;
	setAttr ".pv" 0.34375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve230" -p "pSphere1Follicle9634";
	rename -uid "9CCE3F18-4B8F-CC49-7374-5CBA2836C503";
createNode nurbsCurve -n "curveShape230" -p "curve230";
	rename -uid "E8315649-4C02-76BB-B85E-1B8F2EC50E15";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "1D272146-4C81-66E6-1934-94896644E112";
createNode follicle -n "pSphere1FollicleShape9640" -p "pSphere1Follicle9640";
	rename -uid "9F423900-46F6-DE6D-6B0B-8FA6BAE0DD47";
	setAttr -k off ".v";
	setAttr ".pu" 0.96875;
	setAttr ".pv" 0.40625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve231" -p "pSphere1Follicle9640";
	rename -uid "DCA90793-4799-DB64-3E5D-B48DEFEF1431";
createNode nurbsCurve -n "curveShape231" -p "curve231";
	rename -uid "AE1BBB63-4EBE-D8DB-7E2C-1DA0C6D1DA6D";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "CB2F695F-43D6-6DC6-11A8-9BA24D9F5A25";
createNode follicle -n "pSphere1FollicleShape9646" -p "pSphere1Follicle9646";
	rename -uid "0B332319-4A33-84DE-9921-E4B8EE9096AB";
	setAttr -k off ".v";
	setAttr ".pu" 0.96875;
	setAttr ".pv" 0.46875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve232" -p "pSphere1Follicle9646";
	rename -uid "FEE2A11D-4318-C741-7D8F-35818D78BF31";
createNode nurbsCurve -n "curveShape232" -p "curve232";
	rename -uid "2E27F41F-4915-D201-C4F2-EE80DEE434B0";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "7BFFA6F1-4199-1047-4BF4-4F94A543AC7F";
createNode follicle -n "pSphere1FollicleShape9653" -p "pSphere1Follicle9653";
	rename -uid "A49C8D20-4DF7-037E-2D59-2E9117958C88";
	setAttr -k off ".v";
	setAttr ".pu" 0.96875;
	setAttr ".pv" 0.53125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve233" -p "pSphere1Follicle9653";
	rename -uid "E294A2E2-49DC-02D9-5AAD-1E933F27076E";
createNode nurbsCurve -n "curveShape233" -p "curve233";
	rename -uid "A600F263-4EEA-2063-8AA3-3C83A8EA679C";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "B3DA04A7-4A14-8762-3CD9-E1B60B1948A4";
createNode follicle -n "pSphere1FollicleShape9659" -p "pSphere1Follicle9659";
	rename -uid "6A96C9AE-4570-993D-412E-ECB3BA97784B";
	setAttr -k off ".v";
	setAttr ".pu" 0.96875;
	setAttr ".pv" 0.59375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve234" -p "pSphere1Follicle9659";
	rename -uid "20958651-4FC2-3F4E-7E69-DA92AE44A947";
createNode nurbsCurve -n "curveShape234" -p "curve234";
	rename -uid "918752EE-4766-914A-DFBA-F7A6255870EF";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "0DC78016-4F17-5AB1-88CE-21A31F19BB6E";
createNode follicle -n "pSphere1FollicleShape9665" -p "pSphere1Follicle9665";
	rename -uid "7C390233-4955-7E75-8D48-389AC0A347E8";
	setAttr -k off ".v";
	setAttr ".pu" 0.96875;
	setAttr ".pv" 0.65625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve235" -p "pSphere1Follicle9665";
	rename -uid "44A32E9B-4DEA-BB4D-51AD-448DA1155132";
createNode nurbsCurve -n "curveShape235" -p "curve235";
	rename -uid "1E6EB9C6-4F7A-BD7D-BA55-6EB552A60CDD";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "34B1104E-4F55-301E-F4A7-5289C28BE9D8";
createNode follicle -n "pSphere1FollicleShape9671" -p "pSphere1Follicle9671";
	rename -uid "5699B9EF-46C5-1C2B-9D33-61A6C46F9AF8";
	setAttr -k off ".v";
	setAttr ".pu" 0.96875;
	setAttr ".pv" 0.71875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve236" -p "pSphere1Follicle9671";
	rename -uid "EB8D380C-4E05-3AFC-357B-FBA86C539659";
createNode nurbsCurve -n "curveShape236" -p "curve236";
	rename -uid "BD5C9B73-4410-0AEE-B5FB-AE8CEFDB961A";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "E2DDDB28-4A39-13F8-FAA8-4893AD6E2016";
createNode follicle -n "pSphere1FollicleShape9677" -p "pSphere1Follicle9677";
	rename -uid "1372EB42-4E9E-D358-91AE-53A58F041EC0";
	setAttr -k off ".v";
	setAttr ".pu" 0.96875;
	setAttr ".pv" 0.78125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve237" -p "pSphere1Follicle9677";
	rename -uid "56F1A63A-42DD-0A2D-6928-59831879F965";
createNode nurbsCurve -n "curveShape237" -p "curve237";
	rename -uid "CE517739-42EE-6AC8-5C4D-FEA7BE6B81F7";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "C52C8FE8-43A8-5C4D-7FEB-ADB99E476292";
createNode follicle -n "pSphere1FollicleShape9684" -p "pSphere1Follicle9684";
	rename -uid "D0894D1D-4BA3-4445-9580-5F90B9F54E98";
	setAttr -k off ".v";
	setAttr ".pu" 0.96875;
	setAttr ".pv" 0.84375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve238" -p "pSphere1Follicle9684";
	rename -uid "C46A8FFD-4F58-64DC-DB36-AA8DA927B548";
createNode nurbsCurve -n "curveShape238" -p "curve238";
	rename -uid "53664BCE-4DE4-91EB-2F7A-6CBC02E5EC39";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "B84E3390-431D-F39B-43D5-9499CFF52959";
createNode follicle -n "pSphere1FollicleShape9690" -p "pSphere1Follicle9690";
	rename -uid "4F534FF0-46D9-52E4-EC16-73B272CB9BB4";
	setAttr -k off ".v";
	setAttr ".pu" 0.96875;
	setAttr ".pv" 0.90625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve239" -p "pSphere1Follicle9690";
	rename -uid "54637520-401A-B5A1-7082-C18F57B40ADC";
createNode nurbsCurve -n "curveShape239" -p "curve239";
	rename -uid "DDF66159-44B5-3DE5-2D4B-5CB7DCFB2D86";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "5DD6B394-4A99-B1BB-8045-6E84FC5C0FE5";
createNode follicle -n "pSphere1FollicleShape9696" -p "pSphere1Follicle9696";
	rename -uid "4D126B4B-4809-856E-02B2-9C849F6BFE0E";
	setAttr -k off ".v";
	setAttr ".pu" 0.96875;
	setAttr ".pv" 0.96875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve240" -p "pSphere1Follicle9696";
	rename -uid "4E9BD519-4B2A-9D8C-BCB2-18890C3FB2B9";
createNode nurbsCurve -n "curveShape240" -p "curve240";
	rename -uid "8B5236C5-43F0-4D44-D7E3-F5A23FE81345";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "62FF7A0A-4D50-E254-112E-E088F473B6F6";
createNode pfxHair -n "pfxHairShape1" -p "pfxHair1";
	rename -uid "4C9A5951-416B-3ACA-E9F0-8291F1C477C2";
	setAttr -k off ".v";
	setAttr ".dpc" 100;
	setAttr ".dam" no;
createNode transform -n "hairSystem2";
	rename -uid "9D33DFBA-48FE-FCAC-C981-7089F3BB28AF";
createNode hairSystem -n "hairSystemShape2" -p "hairSystem2";
	rename -uid "6A950394-477E-19D8-970C-DAAB2B950EE4";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	addAttr -ci true -k true -sn "ai_vid" -ln "aiVisibleInDiffuse" -dv 1 -min 0 -max 
		1 -at "bool";
	addAttr -ci true -k true -sn "ai_vig" -ln "aiVisibleInGlossy" -dv 1 -min 0 -max 
		1 -at "bool";
	addAttr -ci true -k true -sn "ai_export_hair_ids" -ln "aiExportHairIDs" -dv 1 -min 
		0 -max 1 -at "bool";
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
	setAttr ".ai_override_hair" yes;
createNode transform -n "hairSystem2Follicles";
	rename -uid "7A7CCF58-4188-5ACC-B795-31AD5FB83C21";
createNode transform -n "pSphere2Follicle303" -p "hairSystem2Follicles";
	rename -uid "45B377AA-4D47-DBA2-BFCA-179FF58D4EA7";
createNode follicle -n "pSphere2FollicleShape303" -p "pSphere2Follicle303";
	rename -uid "496AF64A-49A9-3199-FCAA-FCADF4E08693";
	setAttr -k off ".v";
	setAttr ".pu" 0.03125;
	setAttr ".pv" 0.03125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve241" -p "pSphere2Follicle303";
	rename -uid "8EC44FFE-455B-76DB-A1FB-EBAE85615C21";
createNode nurbsCurve -n "curveShape241" -p "curve241";
	rename -uid "C28E0772-480A-9749-B55A-DB8F805BA80F";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "43681745-41D1-4147-64D2-D887693CA3A1";
createNode follicle -n "pSphere2FollicleShape309" -p "pSphere2Follicle309";
	rename -uid "A9529340-425A-D243-A7F4-779D7740BA6E";
	setAttr -k off ".v";
	setAttr ".pu" 0.03125;
	setAttr ".pv" 0.09375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve242" -p "pSphere2Follicle309";
	rename -uid "4F0A4A03-4295-D2DD-F2C8-7190985391ED";
createNode nurbsCurve -n "curveShape242" -p "curve242";
	rename -uid "BB3741D0-4348-0182-372F-2D801B378FD9";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "CBE3946A-48D7-C205-6A87-1D98A56C8DDF";
createNode follicle -n "pSphere2FollicleShape315" -p "pSphere2Follicle315";
	rename -uid "48EF5AD8-4A56-E98A-6464-4294F98A32BA";
	setAttr -k off ".v";
	setAttr ".pu" 0.03125;
	setAttr ".pv" 0.15625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve243" -p "pSphere2Follicle315";
	rename -uid "541CC803-4003-60C8-B1C3-15A480AD6124";
createNode nurbsCurve -n "curveShape243" -p "curve243";
	rename -uid "B70ACF6E-4B1F-5027-C05D-4A941B7BEBEC";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "32CAA781-4022-426E-4AED-3091A3C2C14A";
createNode follicle -n "pSphere2FollicleShape322" -p "pSphere2Follicle322";
	rename -uid "278C4B63-475D-3B30-80DF-BA8100C78103";
	setAttr -k off ".v";
	setAttr ".pu" 0.03125;
	setAttr ".pv" 0.21875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve244" -p "pSphere2Follicle322";
	rename -uid "9CAF74FB-4426-6C76-B871-49BAEBC3E12A";
createNode nurbsCurve -n "curveShape244" -p "curve244";
	rename -uid "7CD22EAA-4214-F67A-F42B-DF93481C0F4A";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "D6CE21FB-477D-5050-4F31-CDA54F16C4F2";
createNode follicle -n "pSphere2FollicleShape328" -p "pSphere2Follicle328";
	rename -uid "1835A837-4100-91F5-09C3-C09B5C1C24E5";
	setAttr -k off ".v";
	setAttr ".pu" 0.03125;
	setAttr ".pv" 0.28125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve245" -p "pSphere2Follicle328";
	rename -uid "A408E5B4-4F97-BF1C-9580-BC815D0D2C5D";
createNode nurbsCurve -n "curveShape245" -p "curve245";
	rename -uid "BADFEEF4-4FC8-1D91-F89D-6FBAA60DD1E3";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "B38DED02-4FD4-BBEB-C026-B88644882368";
createNode follicle -n "pSphere2FollicleShape334" -p "pSphere2Follicle334";
	rename -uid "2A563A6E-4260-8EA9-6C51-CCA6DA946C4D";
	setAttr -k off ".v";
	setAttr ".pu" 0.03125;
	setAttr ".pv" 0.34375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve246" -p "pSphere2Follicle334";
	rename -uid "EC6FE268-44DD-3B75-5F7D-2DA5373638EB";
createNode nurbsCurve -n "curveShape246" -p "curve246";
	rename -uid "A0C03067-411D-8B08-6C7E-DBAD649093D6";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "224D4C21-4772-CD28-EADE-90A4BD7C39DE";
createNode follicle -n "pSphere2FollicleShape340" -p "pSphere2Follicle340";
	rename -uid "8C30B620-4352-B825-339D-0DA5AD2973C5";
	setAttr -k off ".v";
	setAttr ".pu" 0.03125;
	setAttr ".pv" 0.40625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve247" -p "pSphere2Follicle340";
	rename -uid "C20EA793-49F9-692F-FD70-4698BBE4C146";
createNode nurbsCurve -n "curveShape247" -p "curve247";
	rename -uid "C6CF871C-4A0F-8B24-7A29-23812C6DB41E";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "4227FB46-4CF1-325C-B9E1-C4894BDDCEE8";
createNode follicle -n "pSphere2FollicleShape346" -p "pSphere2Follicle346";
	rename -uid "B7324A67-45D9-817C-9717-F2983F61B76A";
	setAttr -k off ".v";
	setAttr ".pu" 0.03125;
	setAttr ".pv" 0.46875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve248" -p "pSphere2Follicle346";
	rename -uid "6A2DE207-4BE1-B3DD-A30C-269DF3FB55F3";
createNode nurbsCurve -n "curveShape248" -p "curve248";
	rename -uid "7CA20664-4B2D-5C18-277C-5BBB33060B96";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "57593F6E-4A0F-D70E-808F-DFA8082F4D5E";
createNode follicle -n "pSphere2FollicleShape353" -p "pSphere2Follicle353";
	rename -uid "7851231D-46C6-A332-C590-25B03928F6F2";
	setAttr -k off ".v";
	setAttr ".pu" 0.03125;
	setAttr ".pv" 0.53125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve249" -p "pSphere2Follicle353";
	rename -uid "733DC14D-4368-15DA-33C7-1A9F487D51D2";
createNode nurbsCurve -n "curveShape249" -p "curve249";
	rename -uid "25115F02-4E41-DC6F-DCC8-39BEC91BD6A9";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "FF7DA6AE-400F-6F1F-2CEC-549594E5E0FC";
createNode follicle -n "pSphere2FollicleShape359" -p "pSphere2Follicle359";
	rename -uid "B63A5986-4DDB-A8EB-9624-9F9DBD9742F0";
	setAttr -k off ".v";
	setAttr ".pu" 0.03125;
	setAttr ".pv" 0.59375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve250" -p "pSphere2Follicle359";
	rename -uid "9DB31213-44A4-12D8-6BF0-45B5C5BAB2C5";
createNode nurbsCurve -n "curveShape250" -p "curve250";
	rename -uid "1F9E0CFD-44EE-3B63-30B7-EA8CECB70909";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "91E41C91-4C53-D774-7F7C-DF978E617AFF";
createNode follicle -n "pSphere2FollicleShape365" -p "pSphere2Follicle365";
	rename -uid "ACF1787F-4BA9-1C4B-D63B-B487511E71D5";
	setAttr -k off ".v";
	setAttr ".pu" 0.03125;
	setAttr ".pv" 0.65625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve251" -p "pSphere2Follicle365";
	rename -uid "5F7D7429-4EF7-648C-8B5F-2591A8C35A7F";
createNode nurbsCurve -n "curveShape251" -p "curve251";
	rename -uid "8FC8B0BA-44B3-6F5F-8A00-6288CDA47CF1";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "F757899A-4061-E99A-7B79-74950ED402EC";
createNode follicle -n "pSphere2FollicleShape371" -p "pSphere2Follicle371";
	rename -uid "2AF0495D-4031-5257-BE24-0B820B6DC24B";
	setAttr -k off ".v";
	setAttr ".pu" 0.03125;
	setAttr ".pv" 0.71875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve252" -p "pSphere2Follicle371";
	rename -uid "399E72D2-40E6-5C5E-61D6-E6B2967CB97F";
createNode nurbsCurve -n "curveShape252" -p "curve252";
	rename -uid "A192A9DF-4C21-008B-55EA-6990BEC86909";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "F150DB68-45FA-F6C3-81EE-09ACAC161AAA";
createNode follicle -n "pSphere2FollicleShape377" -p "pSphere2Follicle377";
	rename -uid "11119F10-4525-09A9-3711-A79A413FF622";
	setAttr -k off ".v";
	setAttr ".pu" 0.03125;
	setAttr ".pv" 0.78125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve253" -p "pSphere2Follicle377";
	rename -uid "16D368AD-4255-DD3F-BE04-9D9E0E0A9ECE";
createNode nurbsCurve -n "curveShape253" -p "curve253";
	rename -uid "47BD2BF0-42CB-7157-956B-79B5A4C80EF3";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "708D6671-41AC-7570-D828-8F962CE6C476";
createNode follicle -n "pSphere2FollicleShape384" -p "pSphere2Follicle384";
	rename -uid "BB4E07D6-4867-56D1-5F52-6D973F4E11E7";
	setAttr -k off ".v";
	setAttr ".pu" 0.03125;
	setAttr ".pv" 0.84375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve254" -p "pSphere2Follicle384";
	rename -uid "68DF6A12-4CE6-C7F9-BAD0-C9937105D4ED";
createNode nurbsCurve -n "curveShape254" -p "curve254";
	rename -uid "26145635-4F7C-F877-6633-B7ADF9FE4586";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "F5BEFA9E-4749-4D99-20B2-F2A0EB61281E";
createNode follicle -n "pSphere2FollicleShape390" -p "pSphere2Follicle390";
	rename -uid "AAD3FD62-4A49-B323-00E4-6D94CE21F504";
	setAttr -k off ".v";
	setAttr ".pu" 0.03125;
	setAttr ".pv" 0.90625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve255" -p "pSphere2Follicle390";
	rename -uid "54BF8076-40AE-F497-D3F3-B4AC65A0BDCB";
createNode nurbsCurve -n "curveShape255" -p "curve255";
	rename -uid "10819C3D-4F51-6E31-6BBB-75AE431D2139";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "E2AB8EB1-4547-B4F2-B187-B8A0F550727B";
createNode follicle -n "pSphere2FollicleShape396" -p "pSphere2Follicle396";
	rename -uid "F177258C-41BA-1C00-5D6D-33A68AC04E91";
	setAttr -k off ".v";
	setAttr ".pu" 0.03125;
	setAttr ".pv" 0.96875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve256" -p "pSphere2Follicle396";
	rename -uid "013E91AC-42FF-E575-AE84-DBB447DCFA2A";
createNode nurbsCurve -n "curveShape256" -p "curve256";
	rename -uid "8A2FEF9B-45A7-B0DE-CAD9-2B93CB3AB39D";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "99B1CE83-4762-0CF0-AC7E-24A3974E2CE3";
createNode follicle -n "pSphere2FollicleShape909" -p "pSphere2Follicle909";
	rename -uid "105E74CE-4679-75A4-4A3A-B7AB694A6C1E";
	setAttr -k off ".v";
	setAttr ".pu" 0.09375;
	setAttr ".pv" 0.09375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve257" -p "pSphere2Follicle909";
	rename -uid "69FB7F0A-4F3A-C150-8EA6-E9BE45D44AEF";
createNode nurbsCurve -n "curveShape257" -p "curve257";
	rename -uid "2FFC55B8-48C4-923E-222B-2AB9D63E1560";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "F0535F3B-4200-E23E-4CE9-969630DEF2D4";
createNode follicle -n "pSphere2FollicleShape915" -p "pSphere2Follicle915";
	rename -uid "F6D4685F-4628-63AE-66E6-BF8D26B63C36";
	setAttr -k off ".v";
	setAttr ".pu" 0.09375;
	setAttr ".pv" 0.15625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve258" -p "pSphere2Follicle915";
	rename -uid "C2583274-4ABE-FC54-9DE2-6BB14611BC2F";
createNode nurbsCurve -n "curveShape258" -p "curve258";
	rename -uid "13EED041-4BA6-4CEE-7C7A-B9906BB7C6CA";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "CE23B657-43CE-F0EF-CA8B-AEAAD525BE17";
createNode follicle -n "pSphere2FollicleShape922" -p "pSphere2Follicle922";
	rename -uid "5480C255-4B9C-E5BB-DA01-1E8D5DCC4BA8";
	setAttr -k off ".v";
	setAttr ".pu" 0.09375;
	setAttr ".pv" 0.21875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve259" -p "pSphere2Follicle922";
	rename -uid "A789D014-4BCE-8451-0535-85894C4FCB7E";
createNode nurbsCurve -n "curveShape259" -p "curve259";
	rename -uid "13CB626A-4726-F54B-09A5-419C2423664D";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "D0D58697-42D1-F018-0702-14941D19B69E";
createNode follicle -n "pSphere2FollicleShape928" -p "pSphere2Follicle928";
	rename -uid "92D5112F-4713-FF1C-0A16-7D995C2D97CF";
	setAttr -k off ".v";
	setAttr ".pu" 0.09375;
	setAttr ".pv" 0.28125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve260" -p "pSphere2Follicle928";
	rename -uid "6DE3952D-45DF-90F6-A0D7-31BE9FCDDB73";
createNode nurbsCurve -n "curveShape260" -p "curve260";
	rename -uid "392C599D-406D-45BF-8621-3EAF0532B5DA";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "B9B32623-4BBC-64B8-30D2-73AE4393E48E";
createNode follicle -n "pSphere2FollicleShape934" -p "pSphere2Follicle934";
	rename -uid "EBB06412-4564-A96D-2449-139700E1EBFA";
	setAttr -k off ".v";
	setAttr ".pu" 0.09375;
	setAttr ".pv" 0.34375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve261" -p "pSphere2Follicle934";
	rename -uid "AF6AD7B5-4B42-8A39-BE51-44914BC0BFBD";
createNode nurbsCurve -n "curveShape261" -p "curve261";
	rename -uid "19C701FC-4CF3-9A5B-4173-8C867F63706B";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "1F877FCC-4473-F6BD-4DFB-A49055BDDB86";
createNode follicle -n "pSphere2FollicleShape940" -p "pSphere2Follicle940";
	rename -uid "EEFA9722-48DB-5B9C-5EB8-6AAAAD189E1A";
	setAttr -k off ".v";
	setAttr ".pu" 0.09375;
	setAttr ".pv" 0.40625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve262" -p "pSphere2Follicle940";
	rename -uid "87183934-4A92-B6F7-91C3-1589BA13CCCA";
createNode nurbsCurve -n "curveShape262" -p "curve262";
	rename -uid "CA7B8ECD-4F0F-AC76-732B-A28D3F7807F9";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "EACBE23F-4F76-83F3-60B7-CF9C4D19250C";
createNode follicle -n "pSphere2FollicleShape946" -p "pSphere2Follicle946";
	rename -uid "98D1720E-4AD2-B785-D835-2DA7585E6A1C";
	setAttr -k off ".v";
	setAttr ".pu" 0.09375;
	setAttr ".pv" 0.46875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve263" -p "pSphere2Follicle946";
	rename -uid "4C6F66F8-4040-AA41-C466-2689D70791B1";
createNode nurbsCurve -n "curveShape263" -p "curve263";
	rename -uid "7854B659-4E20-DF67-64CC-728ED5108DB0";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "67358345-4A50-9FA8-BEF9-7BA70BCDE11E";
createNode follicle -n "pSphere2FollicleShape953" -p "pSphere2Follicle953";
	rename -uid "008E5496-4F8A-E82D-AC6C-91B88AAD3202";
	setAttr -k off ".v";
	setAttr ".pu" 0.09375;
	setAttr ".pv" 0.53125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve264" -p "pSphere2Follicle953";
	rename -uid "A2D7A949-4922-D377-9AE0-F99606031EA9";
createNode nurbsCurve -n "curveShape264" -p "curve264";
	rename -uid "FF4DD393-497F-511E-F684-188174E061DC";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "A6FFC2DD-4651-878D-89B7-EEA89D75CF4D";
createNode follicle -n "pSphere2FollicleShape959" -p "pSphere2Follicle959";
	rename -uid "A3737237-4483-8BAE-B471-4BBD621C55AE";
	setAttr -k off ".v";
	setAttr ".pu" 0.09375;
	setAttr ".pv" 0.59375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve265" -p "pSphere2Follicle959";
	rename -uid "9B5F6CA4-4E41-609A-857A-61B4169D7A37";
createNode nurbsCurve -n "curveShape265" -p "curve265";
	rename -uid "4B0749EA-4066-C2DD-DB81-1AA8D64D9197";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "7D7749AA-41B0-B1E2-3D4B-EDA01C4D65AE";
createNode follicle -n "pSphere2FollicleShape965" -p "pSphere2Follicle965";
	rename -uid "6229E67D-48A3-FB5C-D289-41AFB72239AC";
	setAttr -k off ".v";
	setAttr ".pu" 0.09375;
	setAttr ".pv" 0.65625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve266" -p "pSphere2Follicle965";
	rename -uid "BBA80695-4CB0-B414-8E43-869AF6A7E4EF";
createNode nurbsCurve -n "curveShape266" -p "curve266";
	rename -uid "CC4983F9-445C-325E-0FF4-6C87167BC420";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "85214FD2-491F-AD75-0FD2-87AB3BB58710";
createNode follicle -n "pSphere2FollicleShape971" -p "pSphere2Follicle971";
	rename -uid "FE663B28-4987-4C05-E70C-2A99908FE5F7";
	setAttr -k off ".v";
	setAttr ".pu" 0.09375;
	setAttr ".pv" 0.71875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve267" -p "pSphere2Follicle971";
	rename -uid "3B948F99-44D0-FB0F-CBA6-3394CA01EADB";
createNode nurbsCurve -n "curveShape267" -p "curve267";
	rename -uid "93629A13-4604-2F2D-C8F6-AF8FEACDDEBF";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "B07A22EA-4914-B951-3479-82966F41F65F";
createNode follicle -n "pSphere2FollicleShape977" -p "pSphere2Follicle977";
	rename -uid "94B0A801-4C27-097E-520C-E48F686937CE";
	setAttr -k off ".v";
	setAttr ".pu" 0.09375;
	setAttr ".pv" 0.78125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve268" -p "pSphere2Follicle977";
	rename -uid "2AC8826C-477F-07B1-F0E9-108431A7A23D";
createNode nurbsCurve -n "curveShape268" -p "curve268";
	rename -uid "A52D670E-4930-F2AB-C1AB-5D84D8BA190C";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "EE8F2FEF-4E43-87D3-27D6-A2823DB6FC54";
createNode follicle -n "pSphere2FollicleShape984" -p "pSphere2Follicle984";
	rename -uid "1B2B2E39-499B-3F66-4803-F0835660E436";
	setAttr -k off ".v";
	setAttr ".pu" 0.09375;
	setAttr ".pv" 0.84375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve269" -p "pSphere2Follicle984";
	rename -uid "BA4FB050-4736-9D6F-C436-4489D9A84DDB";
createNode nurbsCurve -n "curveShape269" -p "curve269";
	rename -uid "B58FE3C3-447C-A197-507C-42BF8D79A1E4";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "6BBF4F6B-48AD-0466-94B8-6CB378C216D5";
createNode follicle -n "pSphere2FollicleShape990" -p "pSphere2Follicle990";
	rename -uid "20887CB4-4B97-B1E5-F8A0-9E81854411E4";
	setAttr -k off ".v";
	setAttr ".pu" 0.09375;
	setAttr ".pv" 0.90625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve270" -p "pSphere2Follicle990";
	rename -uid "AA3DE6C3-4A01-E832-6DE6-85847467D87F";
createNode nurbsCurve -n "curveShape270" -p "curve270";
	rename -uid "2223A35A-40F3-394F-2B51-039311BAE34F";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "70274AA0-4024-7A56-911D-A88D37F8C2A2";
createNode follicle -n "pSphere2FollicleShape1509" -p "pSphere2Follicle1509";
	rename -uid "23874683-4B47-A007-8FA4-FB888A6747DD";
	setAttr -k off ".v";
	setAttr ".pu" 0.15625;
	setAttr ".pv" 0.09375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve271" -p "pSphere2Follicle1509";
	rename -uid "F0B895FB-4784-502C-29D9-BE89993EFA84";
createNode nurbsCurve -n "curveShape271" -p "curve271";
	rename -uid "979653F2-452D-4C30-586E-0CB1B298C2D2";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "2669DC0A-494A-15A8-E31C-538B14520C4F";
createNode follicle -n "pSphere2FollicleShape1515" -p "pSphere2Follicle1515";
	rename -uid "EB15CB31-41DB-0698-78F6-6EAFD02AF46F";
	setAttr -k off ".v";
	setAttr ".pu" 0.15625;
	setAttr ".pv" 0.15625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve272" -p "pSphere2Follicle1515";
	rename -uid "B5E34BA8-4342-BB6E-06FA-9B835B774D17";
createNode nurbsCurve -n "curveShape272" -p "curve272";
	rename -uid "849D061D-4C92-8707-4BE9-90870FC4A1D1";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "A79BF1F9-4441-5F26-E153-71AA397F436C";
createNode follicle -n "pSphere2FollicleShape1522" -p "pSphere2Follicle1522";
	rename -uid "40FEAB82-4584-24E9-5058-51B7327E6701";
	setAttr -k off ".v";
	setAttr ".pu" 0.15625;
	setAttr ".pv" 0.21875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve273" -p "pSphere2Follicle1522";
	rename -uid "3E299484-46E0-E557-03C4-11A5279882F1";
createNode nurbsCurve -n "curveShape273" -p "curve273";
	rename -uid "13171088-4194-5616-55CA-E5959C6A0DBB";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "F5985803-41BD-4A0B-13DF-6DACFCB49E95";
createNode follicle -n "pSphere2FollicleShape1528" -p "pSphere2Follicle1528";
	rename -uid "572522A0-4E19-7A59-5BDE-E6A0A7CC7BA1";
	setAttr -k off ".v";
	setAttr ".pu" 0.15625;
	setAttr ".pv" 0.28125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve274" -p "pSphere2Follicle1528";
	rename -uid "081E9633-4681-0A7B-CF30-CBB1D0E81F5A";
createNode nurbsCurve -n "curveShape274" -p "curve274";
	rename -uid "E546049B-4300-2084-F84F-8383F05387F6";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "D71E0D6F-4AB7-8DE9-4411-27912398AB2C";
createNode follicle -n "pSphere2FollicleShape1534" -p "pSphere2Follicle1534";
	rename -uid "8EA86D80-4DBA-C448-0BD3-E3B4377345AD";
	setAttr -k off ".v";
	setAttr ".pu" 0.15625;
	setAttr ".pv" 0.34375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve275" -p "pSphere2Follicle1534";
	rename -uid "C67406F3-40F5-0099-EEF5-B08A37CC76D0";
createNode nurbsCurve -n "curveShape275" -p "curve275";
	rename -uid "414A842E-4C77-0279-0799-34A0D4413B4D";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "C9E25CAD-45F4-6281-4825-129319C13967";
createNode follicle -n "pSphere2FollicleShape1540" -p "pSphere2Follicle1540";
	rename -uid "C5F4968E-429C-FED2-DB57-5DA75C5A6549";
	setAttr -k off ".v";
	setAttr ".pu" 0.15625;
	setAttr ".pv" 0.40625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve276" -p "pSphere2Follicle1540";
	rename -uid "51463AB2-49C1-B863-84E9-DA9075CC380E";
createNode nurbsCurve -n "curveShape276" -p "curve276";
	rename -uid "7801D7B1-4CC4-F984-B990-C2930FCD6889";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "AB82FFF9-4474-3C5A-54D2-D88F68F2AA18";
createNode follicle -n "pSphere2FollicleShape1546" -p "pSphere2Follicle1546";
	rename -uid "D5ED0D50-4EE6-640B-26DC-AA8AF6C6D5D6";
	setAttr -k off ".v";
	setAttr ".pu" 0.15625;
	setAttr ".pv" 0.46875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve277" -p "pSphere2Follicle1546";
	rename -uid "7AB6A9F0-4E51-27CB-C03A-74BA6F9943E6";
createNode nurbsCurve -n "curveShape277" -p "curve277";
	rename -uid "9C77E649-466D-0FBC-7BD3-188BF68285DE";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "2DEE41CA-4538-CE3B-4398-57B2D473E4A2";
createNode follicle -n "pSphere2FollicleShape1553" -p "pSphere2Follicle1553";
	rename -uid "EF85FC8F-44B1-27A1-5B8B-D595086AF379";
	setAttr -k off ".v";
	setAttr ".pu" 0.15625;
	setAttr ".pv" 0.53125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve278" -p "pSphere2Follicle1553";
	rename -uid "0C442DED-4C0B-0FC8-AEE9-1E8B77833C8A";
createNode nurbsCurve -n "curveShape278" -p "curve278";
	rename -uid "2B091B5E-44B6-9B55-B257-F48114265043";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "F29BAA97-41C3-8589-3922-8DB04B5EEA6A";
createNode follicle -n "pSphere2FollicleShape1559" -p "pSphere2Follicle1559";
	rename -uid "CC61476F-481C-C77E-20B4-2FA959E250CD";
	setAttr -k off ".v";
	setAttr ".pu" 0.15625;
	setAttr ".pv" 0.59375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve279" -p "pSphere2Follicle1559";
	rename -uid "2D5D5ABD-4293-4913-4A0A-BA9F4643B008";
createNode nurbsCurve -n "curveShape279" -p "curve279";
	rename -uid "BEF9C917-4BB9-1972-DD97-75A09C5CABB9";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "1C6634F1-4ABF-34E0-D71F-FBA6D4FFEF81";
createNode follicle -n "pSphere2FollicleShape1565" -p "pSphere2Follicle1565";
	rename -uid "0D02EF19-43ED-9A15-3E94-6695B8F58DC9";
	setAttr -k off ".v";
	setAttr ".pu" 0.15625;
	setAttr ".pv" 0.65625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve280" -p "pSphere2Follicle1565";
	rename -uid "AA031CF8-43B6-3238-DE52-C1AF00734334";
createNode nurbsCurve -n "curveShape280" -p "curve280";
	rename -uid "50E96D03-4A23-EC90-05F3-C9A888D434EC";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "B5144CE3-41D6-F66D-3CC5-BBA308692F9F";
createNode follicle -n "pSphere2FollicleShape1571" -p "pSphere2Follicle1571";
	rename -uid "A3A86B11-41AB-0098-DAB1-FA8DEEC14A07";
	setAttr -k off ".v";
	setAttr ".pu" 0.15625;
	setAttr ".pv" 0.71875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve281" -p "pSphere2Follicle1571";
	rename -uid "1704A320-42C3-220F-84E8-40B84534FDCF";
createNode nurbsCurve -n "curveShape281" -p "curve281";
	rename -uid "E5E76C4C-45F0-8E7E-20FD-DEBAE2F3BAC1";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "E5A93CF8-45FC-5D87-3874-3CAC4394DFD4";
createNode follicle -n "pSphere2FollicleShape1577" -p "pSphere2Follicle1577";
	rename -uid "28F02F59-43BD-C6F6-7615-5994BC692F1D";
	setAttr -k off ".v";
	setAttr ".pu" 0.15625;
	setAttr ".pv" 0.78125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve282" -p "pSphere2Follicle1577";
	rename -uid "282640D0-4DD9-2D46-DF56-7F8FCDB0892B";
createNode nurbsCurve -n "curveShape282" -p "curve282";
	rename -uid "0EB8FD20-43B8-1467-F74B-B19598E52C49";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "392110AF-4444-CD1B-FFF5-04B99C9FEC92";
createNode follicle -n "pSphere2FollicleShape1584" -p "pSphere2Follicle1584";
	rename -uid "6DD86E1C-47B9-2E45-C741-25BAB896B6F2";
	setAttr -k off ".v";
	setAttr ".pu" 0.15625;
	setAttr ".pv" 0.84375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve283" -p "pSphere2Follicle1584";
	rename -uid "62EC0BF4-45BF-BCC2-2E50-819813AAE52C";
createNode nurbsCurve -n "curveShape283" -p "curve283";
	rename -uid "5822AE93-479A-BDF5-DA6B-41800E706B6E";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "902F0EF4-43BC-2035-037B-ABA641E01BD6";
createNode follicle -n "pSphere2FollicleShape1590" -p "pSphere2Follicle1590";
	rename -uid "F4122565-4ED0-482B-7684-44952AA5BF97";
	setAttr -k off ".v";
	setAttr ".pu" 0.15625;
	setAttr ".pv" 0.90625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve284" -p "pSphere2Follicle1590";
	rename -uid "B9C3B8CA-429C-64E4-468D-04A30391DCB2";
createNode nurbsCurve -n "curveShape284" -p "curve284";
	rename -uid "276C4390-4A8D-8EEA-6304-F8AC4D9EAA52";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "F9CBC240-4547-FF6D-4723-9F881ACEA814";
createNode follicle -n "pSphere2FollicleShape2203" -p "pSphere2Follicle2203";
	rename -uid "74ADFF6A-4270-BDCD-9582-C4B4D02C3348";
	setAttr -k off ".v";
	setAttr ".pu" 0.21875;
	setAttr ".pv" 0.03125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve285" -p "pSphere2Follicle2203";
	rename -uid "929C3D2C-4802-4430-155D-25AECD5F4AE2";
createNode nurbsCurve -n "curveShape285" -p "curve285";
	rename -uid "63D7E032-4F30-2EC4-88CF-09AEF1361D87";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "51B13A31-4CDB-7F49-169C-6AA7D18B7BAC";
createNode follicle -n "pSphere2FollicleShape2209" -p "pSphere2Follicle2209";
	rename -uid "4B59CDB2-415D-B207-1AB4-3DB448369994";
	setAttr -k off ".v";
	setAttr ".pu" 0.21875;
	setAttr ".pv" 0.09375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve286" -p "pSphere2Follicle2209";
	rename -uid "804BBDB1-4403-2882-FEE7-65805C4BC686";
createNode nurbsCurve -n "curveShape286" -p "curve286";
	rename -uid "9477DED1-424E-CAF1-10D3-64858AC8230B";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "EB9833C2-4DDE-69FE-B90F-9D81CE778472";
createNode follicle -n "pSphere2FollicleShape2215" -p "pSphere2Follicle2215";
	rename -uid "8441CB04-48AE-3C70-8BF5-CA94001CC05A";
	setAttr -k off ".v";
	setAttr ".pu" 0.21875;
	setAttr ".pv" 0.15625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve287" -p "pSphere2Follicle2215";
	rename -uid "A45A2C98-4FB4-9176-5402-C68B743FE81F";
createNode nurbsCurve -n "curveShape287" -p "curve287";
	rename -uid "BBD16F6F-49A1-C4A6-9036-25B3B7389AE1";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "16AEC2BA-4B11-B437-2633-3B8AE346C99F";
createNode follicle -n "pSphere2FollicleShape2222" -p "pSphere2Follicle2222";
	rename -uid "431F5D26-4D6E-76C4-F3B4-DBAE180E9667";
	setAttr -k off ".v";
	setAttr ".pu" 0.21875;
	setAttr ".pv" 0.21875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve288" -p "pSphere2Follicle2222";
	rename -uid "0E9BFBF1-4A80-7FE8-A3D4-5EBBF6F957C6";
createNode nurbsCurve -n "curveShape288" -p "curve288";
	rename -uid "ECCC4B96-40D7-D618-6C89-35A5D85F24A1";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "42021471-4E08-679F-0626-89AB4AE179FF";
createNode follicle -n "pSphere2FollicleShape2228" -p "pSphere2Follicle2228";
	rename -uid "30D6232C-43DA-FB9C-7560-AEAD5470A313";
	setAttr -k off ".v";
	setAttr ".pu" 0.21875;
	setAttr ".pv" 0.28125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve289" -p "pSphere2Follicle2228";
	rename -uid "130A0AB2-4410-447F-29E2-8999FED76F1E";
createNode nurbsCurve -n "curveShape289" -p "curve289";
	rename -uid "0988B3BC-4CC5-DAE3-2DB7-22A5E1421DE5";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "E1EA0E75-481F-C049-E069-C8AC63DE8274";
createNode follicle -n "pSphere2FollicleShape2234" -p "pSphere2Follicle2234";
	rename -uid "CA8C79F0-427C-C0AB-4652-9A97E3390A64";
	setAttr -k off ".v";
	setAttr ".pu" 0.21875;
	setAttr ".pv" 0.34375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve290" -p "pSphere2Follicle2234";
	rename -uid "26200804-4CC5-789C-6105-6F8EBFCDE227";
createNode nurbsCurve -n "curveShape290" -p "curve290";
	rename -uid "7D402BA5-44E0-FCAF-E1E3-AD8EA96109AA";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "29F8650D-468C-D797-3D72-27B0EE5D185B";
createNode follicle -n "pSphere2FollicleShape2240" -p "pSphere2Follicle2240";
	rename -uid "6340392A-4E3C-C4AA-C9D1-66B98BAA7078";
	setAttr -k off ".v";
	setAttr ".pu" 0.21875;
	setAttr ".pv" 0.40625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve291" -p "pSphere2Follicle2240";
	rename -uid "8545C048-4194-861B-B7F4-E39F679D7597";
createNode nurbsCurve -n "curveShape291" -p "curve291";
	rename -uid "8AD9254F-4F0B-9F55-758E-E5B845D09B94";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "47C40E38-4A7B-D46E-CB2C-4E958EE01C00";
createNode follicle -n "pSphere2FollicleShape2246" -p "pSphere2Follicle2246";
	rename -uid "866C5536-42CA-3639-0B11-16AC26D33B2B";
	setAttr -k off ".v";
	setAttr ".pu" 0.21875;
	setAttr ".pv" 0.46875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve292" -p "pSphere2Follicle2246";
	rename -uid "9ADE8A88-4876-0483-1661-D69E8041666E";
createNode nurbsCurve -n "curveShape292" -p "curve292";
	rename -uid "8C8599D7-4CC9-72C5-C21C-30988478C12C";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "CB7C665D-4374-72AB-F74E-34A6A41D9668";
createNode follicle -n "pSphere2FollicleShape2253" -p "pSphere2Follicle2253";
	rename -uid "B52BE4C6-410A-7491-EEDB-BDA4AE48A387";
	setAttr -k off ".v";
	setAttr ".pu" 0.21875;
	setAttr ".pv" 0.53125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve293" -p "pSphere2Follicle2253";
	rename -uid "584E052D-4A46-71D1-0818-7CAAFCC66383";
createNode nurbsCurve -n "curveShape293" -p "curve293";
	rename -uid "29BBCDC2-405F-07AA-17FA-39B516369A95";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "C2BA5BE2-43BA-BFE2-8E85-BD9671D5C635";
createNode follicle -n "pSphere2FollicleShape2259" -p "pSphere2Follicle2259";
	rename -uid "29165FC6-4A20-DD85-79B6-D2860959A768";
	setAttr -k off ".v";
	setAttr ".pu" 0.21875;
	setAttr ".pv" 0.59375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve294" -p "pSphere2Follicle2259";
	rename -uid "7149580A-4B75-70A5-71A9-709626C21DB1";
createNode nurbsCurve -n "curveShape294" -p "curve294";
	rename -uid "31085443-4903-70E8-5718-4785AC02F907";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "A905C465-4F3C-8DAF-9E17-74B119230592";
createNode follicle -n "pSphere2FollicleShape2265" -p "pSphere2Follicle2265";
	rename -uid "3D00B159-4FA7-4AB4-C44A-659EF1B924B6";
	setAttr -k off ".v";
	setAttr ".pu" 0.21875;
	setAttr ".pv" 0.65625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve295" -p "pSphere2Follicle2265";
	rename -uid "57D367BD-4E54-FEC0-6CD4-7487854C4C50";
createNode nurbsCurve -n "curveShape295" -p "curve295";
	rename -uid "BC797508-4E90-6B84-22CE-DEBA956D47F6";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "91C1DEE9-406D-ABEA-C768-379C368FE15E";
createNode follicle -n "pSphere2FollicleShape2271" -p "pSphere2Follicle2271";
	rename -uid "77F1A1C4-43FC-66FA-6939-B8A42D1C9205";
	setAttr -k off ".v";
	setAttr ".pu" 0.21875;
	setAttr ".pv" 0.71875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve296" -p "pSphere2Follicle2271";
	rename -uid "19CD1649-469B-D19C-61AF-FE89FFADB59C";
createNode nurbsCurve -n "curveShape296" -p "curve296";
	rename -uid "FAA36678-4F14-D461-53D1-89ADA56D1D75";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "5996046A-4A70-EB51-6D3F-4E8EC89C1A22";
createNode follicle -n "pSphere2FollicleShape2277" -p "pSphere2Follicle2277";
	rename -uid "42D9A286-4CA9-32D0-BEBF-9A8607D1C163";
	setAttr -k off ".v";
	setAttr ".pu" 0.21875;
	setAttr ".pv" 0.78125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve297" -p "pSphere2Follicle2277";
	rename -uid "A6858058-4E7D-27B5-9035-ECBC6F67A566";
createNode nurbsCurve -n "curveShape297" -p "curve297";
	rename -uid "4C8182BC-41CE-6529-DE13-78A50A637388";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "8E3E5917-45C1-3AF5-1B64-C9B11E80645F";
createNode follicle -n "pSphere2FollicleShape2284" -p "pSphere2Follicle2284";
	rename -uid "2DA0A68B-4559-3EDA-6BAD-8B96E1119A6B";
	setAttr -k off ".v";
	setAttr ".pu" 0.21875;
	setAttr ".pv" 0.84375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve298" -p "pSphere2Follicle2284";
	rename -uid "02FE6BF9-431E-E780-8960-4AB7BAFCA96B";
createNode nurbsCurve -n "curveShape298" -p "curve298";
	rename -uid "2653B9E7-4B59-BAF8-9FB7-0E86D2B22E0A";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "2A6CD7B6-4251-E8A1-EC42-4DA308CA6EFD";
createNode follicle -n "pSphere2FollicleShape2290" -p "pSphere2Follicle2290";
	rename -uid "0830F424-48FD-67B2-AC2D-85B0294E51A0";
	setAttr -k off ".v";
	setAttr ".pu" 0.21875;
	setAttr ".pv" 0.90625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve299" -p "pSphere2Follicle2290";
	rename -uid "084B6A25-4498-B49B-2B7F-C79998E4FDEC";
createNode nurbsCurve -n "curveShape299" -p "curve299";
	rename -uid "39D716D3-41DE-3860-FC5C-079B82BD56F8";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "695C529A-4864-5D47-AC6E-76920DEDE2AA";
createNode follicle -n "pSphere2FollicleShape2296" -p "pSphere2Follicle2296";
	rename -uid "D5B5A75E-4DF9-DA18-B06D-31B4C3739189";
	setAttr -k off ".v";
	setAttr ".pu" 0.21875;
	setAttr ".pv" 0.96875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve300" -p "pSphere2Follicle2296";
	rename -uid "917AEE72-4E1A-9A87-66D7-9D81D398DC1E";
createNode nurbsCurve -n "curveShape300" -p "curve300";
	rename -uid "8E02CD3D-4D52-43C7-CDF0-BFAEFCB7AF4F";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "48B0B323-415E-5BE2-96CD-9786E60D7FBB";
createNode follicle -n "pSphere2FollicleShape2803" -p "pSphere2Follicle2803";
	rename -uid "44755C3B-42E0-C33E-7B55-6CA4314F927C";
	setAttr -k off ".v";
	setAttr ".pu" 0.28125;
	setAttr ".pv" 0.03125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve301" -p "pSphere2Follicle2803";
	rename -uid "D8760767-4329-7AC4-D3BF-5DB715E56D3F";
createNode nurbsCurve -n "curveShape301" -p "curve301";
	rename -uid "25B4A4AA-4CE3-F517-8287-D2B3E88BAE74";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "B99FCA18-40FE-C255-2D39-C8B26E6CFBC5";
createNode follicle -n "pSphere2FollicleShape2809" -p "pSphere2Follicle2809";
	rename -uid "4A505121-4374-CC94-77F6-6E983C404D3A";
	setAttr -k off ".v";
	setAttr ".pu" 0.28125;
	setAttr ".pv" 0.09375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve302" -p "pSphere2Follicle2809";
	rename -uid "906CFA28-4D8F-7BF7-99F4-388756F02678";
createNode nurbsCurve -n "curveShape302" -p "curve302";
	rename -uid "6727BE16-4169-DBCC-461D-D6BA4E800D2F";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "1F2E419D-4BE2-788D-7643-608ACD3897B8";
createNode follicle -n "pSphere2FollicleShape2815" -p "pSphere2Follicle2815";
	rename -uid "69EA60C9-48D8-08A3-6587-53AC5D9A145D";
	setAttr -k off ".v";
	setAttr ".pu" 0.28125;
	setAttr ".pv" 0.15625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve303" -p "pSphere2Follicle2815";
	rename -uid "9D31A4E5-4AF4-4F27-BF47-ED8CA2A9B429";
createNode nurbsCurve -n "curveShape303" -p "curve303";
	rename -uid "297C9C1F-4D98-D7DD-1ABD-B6BFBBE7C788";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "41D3A3D3-495A-0597-1390-838C798FE609";
createNode follicle -n "pSphere2FollicleShape2822" -p "pSphere2Follicle2822";
	rename -uid "627664EC-4CF0-CFB8-A1C0-8EA53640BEEE";
	setAttr -k off ".v";
	setAttr ".pu" 0.28125;
	setAttr ".pv" 0.21875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve304" -p "pSphere2Follicle2822";
	rename -uid "C91805D0-428D-6B85-EA89-E3941887A623";
createNode nurbsCurve -n "curveShape304" -p "curve304";
	rename -uid "6D8184CD-412B-9DF6-EA92-5E9563A52937";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "134505FF-435F-C22F-2D27-4483B7CF0EAE";
createNode follicle -n "pSphere2FollicleShape2828" -p "pSphere2Follicle2828";
	rename -uid "0AB59219-4DA2-19C1-FA88-1AA74BC9AB5D";
	setAttr -k off ".v";
	setAttr ".pu" 0.28125;
	setAttr ".pv" 0.28125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve305" -p "pSphere2Follicle2828";
	rename -uid "40033997-4C22-5B03-93AC-1BAB74EF178B";
createNode nurbsCurve -n "curveShape305" -p "curve305";
	rename -uid "C65C95D2-483A-5B63-2343-56BB6602C79B";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "0D8D1845-4465-F858-8DDB-68AD9F13B95A";
createNode follicle -n "pSphere2FollicleShape2834" -p "pSphere2Follicle2834";
	rename -uid "5C41C605-4C53-40E3-503B-26893986AD69";
	setAttr -k off ".v";
	setAttr ".pu" 0.28125;
	setAttr ".pv" 0.34375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve306" -p "pSphere2Follicle2834";
	rename -uid "FDC5EAF1-4C49-0CA1-ABEC-99A7EA7590B8";
createNode nurbsCurve -n "curveShape306" -p "curve306";
	rename -uid "7094D37E-4F98-6A04-E37D-C4B455647F4D";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "F11C6B22-429B-8A39-5EBE-E59E0B5DFECB";
createNode follicle -n "pSphere2FollicleShape2840" -p "pSphere2Follicle2840";
	rename -uid "39DF2000-47D9-C263-6A56-29904058FA4E";
	setAttr -k off ".v";
	setAttr ".pu" 0.28125;
	setAttr ".pv" 0.40625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve307" -p "pSphere2Follicle2840";
	rename -uid "F1552A5B-4EBD-A6B0-E4D1-D7A6A381FF00";
createNode nurbsCurve -n "curveShape307" -p "curve307";
	rename -uid "1CB4D8EA-4186-B094-5578-0E87F2807481";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "995410BA-42C0-0E23-4AC2-90A87A740EEC";
createNode follicle -n "pSphere2FollicleShape2846" -p "pSphere2Follicle2846";
	rename -uid "2E18FED1-48FC-2DAE-FE9E-09996015160D";
	setAttr -k off ".v";
	setAttr ".pu" 0.28125;
	setAttr ".pv" 0.46875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve308" -p "pSphere2Follicle2846";
	rename -uid "6C617764-4F47-13C6-9A1C-C3B5D6CC9DF2";
createNode nurbsCurve -n "curveShape308" -p "curve308";
	rename -uid "535031D0-41A3-96AC-3B4E-0B87249823EF";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "90C53004-463D-0EAA-00B2-2BB20B0E586A";
createNode follicle -n "pSphere2FollicleShape2853" -p "pSphere2Follicle2853";
	rename -uid "01798735-4808-63E2-C1CA-BD8216F46424";
	setAttr -k off ".v";
	setAttr ".pu" 0.28125;
	setAttr ".pv" 0.53125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve309" -p "pSphere2Follicle2853";
	rename -uid "E2B297A4-4BF9-19F4-4BBD-BCB5E3984F31";
createNode nurbsCurve -n "curveShape309" -p "curve309";
	rename -uid "0864FD7C-41DB-7132-246D-308B4DFB558A";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "F00EA99E-476B-6FC4-84B5-ABAA49FC4F43";
createNode follicle -n "pSphere2FollicleShape2859" -p "pSphere2Follicle2859";
	rename -uid "AD6CDB06-4971-027A-BA3E-B396A05D0CCC";
	setAttr -k off ".v";
	setAttr ".pu" 0.28125;
	setAttr ".pv" 0.59375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve310" -p "pSphere2Follicle2859";
	rename -uid "97E7D70A-4547-C2F0-0E40-B7AD467FE690";
createNode nurbsCurve -n "curveShape310" -p "curve310";
	rename -uid "406C39AD-41AB-7306-59DD-2DBD9EF05616";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "FC4529C1-4F50-A825-05D6-C68F14557F61";
createNode follicle -n "pSphere2FollicleShape2865" -p "pSphere2Follicle2865";
	rename -uid "9230E414-43B5-4444-7183-E3B2EC31FCA3";
	setAttr -k off ".v";
	setAttr ".pu" 0.28125;
	setAttr ".pv" 0.65625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve311" -p "pSphere2Follicle2865";
	rename -uid "1456A94F-45F4-D4AE-9C87-1DA41857D294";
createNode nurbsCurve -n "curveShape311" -p "curve311";
	rename -uid "1D32F3C6-4A19-9FC5-9855-DD8C3513623D";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "C7D8B5B0-4E2C-2D07-0009-C8A52F922DDF";
createNode follicle -n "pSphere2FollicleShape2871" -p "pSphere2Follicle2871";
	rename -uid "E028CF9B-44A9-8E26-9A3E-6E98413DF68E";
	setAttr -k off ".v";
	setAttr ".pu" 0.28125;
	setAttr ".pv" 0.71875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve312" -p "pSphere2Follicle2871";
	rename -uid "37916D1D-4C53-6F98-CFF7-C7AFC8A9D314";
createNode nurbsCurve -n "curveShape312" -p "curve312";
	rename -uid "7D41EA0F-4BFB-806E-2893-1C8071CC3A7C";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "E774872A-4B6B-C97F-DDF9-D689E4FFC8BA";
createNode follicle -n "pSphere2FollicleShape2877" -p "pSphere2Follicle2877";
	rename -uid "0F5F229F-4E31-DE04-6BC5-4199C5C78A7E";
	setAttr -k off ".v";
	setAttr ".pu" 0.28125;
	setAttr ".pv" 0.78125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve313" -p "pSphere2Follicle2877";
	rename -uid "950C4F42-4FDE-FE9F-CDC4-FF85834ED6BC";
createNode nurbsCurve -n "curveShape313" -p "curve313";
	rename -uid "12D825DF-47A7-32F8-EA3E-E383A35B40D8";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "63A9B5ED-4F9E-BD39-C70C-BF87994824C7";
createNode follicle -n "pSphere2FollicleShape2884" -p "pSphere2Follicle2884";
	rename -uid "41F7A706-4FD1-831B-7794-80BB786FEF34";
	setAttr -k off ".v";
	setAttr ".pu" 0.28125;
	setAttr ".pv" 0.84375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve314" -p "pSphere2Follicle2884";
	rename -uid "D6D22351-4F15-C10B-5334-AB85B4A06D09";
createNode nurbsCurve -n "curveShape314" -p "curve314";
	rename -uid "B8AFBCE1-47CC-4FBA-6CC3-C2843C0B3819";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "6831FDA3-4012-27AC-9D77-D39ECB8663FA";
createNode follicle -n "pSphere2FollicleShape2890" -p "pSphere2Follicle2890";
	rename -uid "EED8C68F-4E6B-7F80-EA5E-FF991902D564";
	setAttr -k off ".v";
	setAttr ".pu" 0.28125;
	setAttr ".pv" 0.90625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve315" -p "pSphere2Follicle2890";
	rename -uid "DE63EA1B-4F67-4F4D-7B32-ADAFAA88A0E4";
createNode nurbsCurve -n "curveShape315" -p "curve315";
	rename -uid "4512E437-422A-305D-9F76-0BA127137669";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "DEDFBEC1-433F-ED7F-A5AB-B3A52CB4EB77";
createNode follicle -n "pSphere2FollicleShape2896" -p "pSphere2Follicle2896";
	rename -uid "B9D189A3-4A52-171D-4BA4-B1834E747D7E";
	setAttr -k off ".v";
	setAttr ".pu" 0.28125;
	setAttr ".pv" 0.96875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve316" -p "pSphere2Follicle2896";
	rename -uid "A83A7462-4163-4704-2752-29BD396EB5C6";
createNode nurbsCurve -n "curveShape316" -p "curve316";
	rename -uid "B2931883-4B24-EF0D-92F9-8E951535A059";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "E053985C-4006-8DC5-9AAC-EB9C51326E1D";
createNode follicle -n "pSphere2FollicleShape3409" -p "pSphere2Follicle3409";
	rename -uid "4865CD8E-46A0-8CA1-B72D-D1899169B5B3";
	setAttr -k off ".v";
	setAttr ".pu" 0.34375;
	setAttr ".pv" 0.09375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve317" -p "pSphere2Follicle3409";
	rename -uid "E978AE49-4CA4-312B-E297-0390260BB916";
createNode nurbsCurve -n "curveShape317" -p "curve317";
	rename -uid "CB4DA42C-4945-1F71-CC61-0C8CF29A6390";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "E64D7806-4116-CD6C-420C-218CDD63CF5C";
createNode follicle -n "pSphere2FollicleShape3415" -p "pSphere2Follicle3415";
	rename -uid "EB3B83D6-4CCA-CE79-5F25-F3B724533335";
	setAttr -k off ".v";
	setAttr ".pu" 0.34375;
	setAttr ".pv" 0.15625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve318" -p "pSphere2Follicle3415";
	rename -uid "0B924B43-40F9-C553-88D6-D692B152B94D";
createNode nurbsCurve -n "curveShape318" -p "curve318";
	rename -uid "989AEA49-46E9-9736-A798-ADBF95C0BF4B";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "737D725C-4E6E-F8DB-6009-D8BBD777AE6E";
createNode follicle -n "pSphere2FollicleShape3422" -p "pSphere2Follicle3422";
	rename -uid "86EBE385-4886-3F27-AC7F-20A8557E4A07";
	setAttr -k off ".v";
	setAttr ".pu" 0.34375;
	setAttr ".pv" 0.21875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve319" -p "pSphere2Follicle3422";
	rename -uid "D7A48CFF-415A-72E7-5FA0-57BB1DAD4A5E";
createNode nurbsCurve -n "curveShape319" -p "curve319";
	rename -uid "3B42E074-4BA8-FD20-15E9-039A9BA671B3";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "112559A1-4F56-0DEE-3F30-4896DEC85B41";
createNode follicle -n "pSphere2FollicleShape3428" -p "pSphere2Follicle3428";
	rename -uid "9799D130-4D63-83EA-CD17-4E84862C5E9C";
	setAttr -k off ".v";
	setAttr ".pu" 0.34375;
	setAttr ".pv" 0.28125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve320" -p "pSphere2Follicle3428";
	rename -uid "DCDC804C-41E4-C4E2-D454-E9BFBA1D98D5";
createNode nurbsCurve -n "curveShape320" -p "curve320";
	rename -uid "594C4F2F-4DE6-1164-FBF9-608EF4541194";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "F660D63B-4D0F-421B-51C8-6EBC75720013";
createNode follicle -n "pSphere2FollicleShape3434" -p "pSphere2Follicle3434";
	rename -uid "58384ACB-4EB5-6295-AE95-C68F9F154043";
	setAttr -k off ".v";
	setAttr ".pu" 0.34375;
	setAttr ".pv" 0.34375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve321" -p "pSphere2Follicle3434";
	rename -uid "E2B846A8-48ED-B7E5-9801-77A8C80AF107";
createNode nurbsCurve -n "curveShape321" -p "curve321";
	rename -uid "CAD66511-4BD5-A0BE-4080-58A1730EA4AA";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "2F4594F5-4C61-5F4F-CBA9-B981A40A8025";
createNode follicle -n "pSphere2FollicleShape3440" -p "pSphere2Follicle3440";
	rename -uid "A6552F88-467F-DF94-97B6-BCB9B6A42DB1";
	setAttr -k off ".v";
	setAttr ".pu" 0.34375;
	setAttr ".pv" 0.40625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve322" -p "pSphere2Follicle3440";
	rename -uid "EBFADF37-46A5-790B-AB30-1CB2E2B64B60";
createNode nurbsCurve -n "curveShape322" -p "curve322";
	rename -uid "A5DEE438-40EE-BAC9-3790-E5A057FEED1B";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "554C0D87-4192-8829-4DEF-0191BEBFE0FA";
createNode follicle -n "pSphere2FollicleShape3446" -p "pSphere2Follicle3446";
	rename -uid "5E6668FB-41C8-F2F5-F3F3-4DBC4CBAD522";
	setAttr -k off ".v";
	setAttr ".pu" 0.34375;
	setAttr ".pv" 0.46875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve323" -p "pSphere2Follicle3446";
	rename -uid "7B31EAA3-4BAE-FDC6-EDFE-0EA87A6D6779";
createNode nurbsCurve -n "curveShape323" -p "curve323";
	rename -uid "7874C2A2-4DED-CAD4-FBB2-EA887EBA7C98";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "7CEAFE60-40F0-CC7C-96CF-C89DAA1A95D8";
createNode follicle -n "pSphere2FollicleShape3453" -p "pSphere2Follicle3453";
	rename -uid "8BED94F8-4E3A-CDDE-109B-47B641A6AE5B";
	setAttr -k off ".v";
	setAttr ".pu" 0.34375;
	setAttr ".pv" 0.53125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve324" -p "pSphere2Follicle3453";
	rename -uid "A8F454EE-4E1D-5D8E-C427-7383493E9211";
createNode nurbsCurve -n "curveShape324" -p "curve324";
	rename -uid "0674CFD2-4614-2E40-1B1C-55A38836E68A";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "76C23C2E-4BBF-05D0-4D57-6B8C57930542";
createNode follicle -n "pSphere2FollicleShape3459" -p "pSphere2Follicle3459";
	rename -uid "13193549-41C4-714B-B0A8-9D922874D1A0";
	setAttr -k off ".v";
	setAttr ".pu" 0.34375;
	setAttr ".pv" 0.59375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve325" -p "pSphere2Follicle3459";
	rename -uid "440C3CA3-49F8-C984-E6DE-BEB447118158";
createNode nurbsCurve -n "curveShape325" -p "curve325";
	rename -uid "2175CF66-4C3F-3D19-AB70-56B899C659AB";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "60980F11-4945-85BC-0A17-8A8D4933990F";
createNode follicle -n "pSphere2FollicleShape3465" -p "pSphere2Follicle3465";
	rename -uid "9E76C600-4F16-03BE-D193-8185308F6910";
	setAttr -k off ".v";
	setAttr ".pu" 0.34375;
	setAttr ".pv" 0.65625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve326" -p "pSphere2Follicle3465";
	rename -uid "1A462827-4FB6-8BFE-01D3-E1A0BD511660";
createNode nurbsCurve -n "curveShape326" -p "curve326";
	rename -uid "D514F453-454B-46BF-450B-50B2A465A008";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "A363C207-442F-E97A-ABDB-1AB349609E2C";
createNode follicle -n "pSphere2FollicleShape3471" -p "pSphere2Follicle3471";
	rename -uid "57EA1204-490E-DA9E-6E81-CCB71A6A37F9";
	setAttr -k off ".v";
	setAttr ".pu" 0.34375;
	setAttr ".pv" 0.71875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve327" -p "pSphere2Follicle3471";
	rename -uid "EF16454C-4E1A-21EB-4BCD-E9B14CDA8E5D";
createNode nurbsCurve -n "curveShape327" -p "curve327";
	rename -uid "8A861507-426F-7ADF-7B24-7E88FCA389BC";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "C1D495C6-41AC-D622-4185-599D0474DE36";
createNode follicle -n "pSphere2FollicleShape3477" -p "pSphere2Follicle3477";
	rename -uid "6F761779-4D67-3304-FCD5-8483E9B1D4D2";
	setAttr -k off ".v";
	setAttr ".pu" 0.34375;
	setAttr ".pv" 0.78125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve328" -p "pSphere2Follicle3477";
	rename -uid "5208CBF8-434E-1537-9646-FFBAFA5AF1C5";
createNode nurbsCurve -n "curveShape328" -p "curve328";
	rename -uid "C081C148-4B30-88FC-500D-7982E9025926";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "0054B99C-4561-991F-C448-C2984B014000";
createNode follicle -n "pSphere2FollicleShape3484" -p "pSphere2Follicle3484";
	rename -uid "A4080231-4292-5A31-4435-9FA7C6C46B24";
	setAttr -k off ".v";
	setAttr ".pu" 0.34375;
	setAttr ".pv" 0.84375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve329" -p "pSphere2Follicle3484";
	rename -uid "403044EB-4D81-7A94-1461-019788C28802";
createNode nurbsCurve -n "curveShape329" -p "curve329";
	rename -uid "AC50EB55-45E1-98CE-494A-6EB167BE6410";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "74E06398-4657-F2C6-4D8F-D88F8B8750A4";
createNode follicle -n "pSphere2FollicleShape3490" -p "pSphere2Follicle3490";
	rename -uid "BD280532-4EF4-1BED-B16E-CC924E92F733";
	setAttr -k off ".v";
	setAttr ".pu" 0.34375;
	setAttr ".pv" 0.90625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve330" -p "pSphere2Follicle3490";
	rename -uid "6B250E00-4417-DC3F-5B53-858B021BDAE2";
createNode nurbsCurve -n "curveShape330" -p "curve330";
	rename -uid "3187D54C-4248-0ABF-0BD1-36AE904DC3F2";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "02184980-4E90-8844-F66C-32B78909A9F0";
createNode follicle -n "pSphere2FollicleShape4009" -p "pSphere2Follicle4009";
	rename -uid "AC948227-4668-1B56-246F-63AEAC3F8288";
	setAttr -k off ".v";
	setAttr ".pu" 0.40625;
	setAttr ".pv" 0.09375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve331" -p "pSphere2Follicle4009";
	rename -uid "07C69901-4FB6-8B68-6615-9EA2A5234705";
createNode nurbsCurve -n "curveShape331" -p "curve331";
	rename -uid "8E2B4BC0-4DD9-E909-0E70-A08577B55BDD";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "ADBA1428-46B8-D672-1FEB-F4963366363D";
createNode follicle -n "pSphere2FollicleShape4015" -p "pSphere2Follicle4015";
	rename -uid "CAAEF657-4700-1E4D-DDDC-A6A2DC7AB04B";
	setAttr -k off ".v";
	setAttr ".pu" 0.40625;
	setAttr ".pv" 0.15625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve332" -p "pSphere2Follicle4015";
	rename -uid "E796D892-4C6A-52AD-62BD-E48466891B72";
createNode nurbsCurve -n "curveShape332" -p "curve332";
	rename -uid "722CA21E-433B-8F0C-74BE-AC937968B266";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "18136D90-4F0E-0DC4-7580-50A879BAFA6A";
createNode follicle -n "pSphere2FollicleShape4022" -p "pSphere2Follicle4022";
	rename -uid "AE8A9823-42BF-6289-63B6-2EA49A36E9D4";
	setAttr -k off ".v";
	setAttr ".pu" 0.40625;
	setAttr ".pv" 0.21875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve333" -p "pSphere2Follicle4022";
	rename -uid "E1BAA576-4FF1-1362-CCAD-F8B0E90C90FA";
createNode nurbsCurve -n "curveShape333" -p "curve333";
	rename -uid "0EDFE4A4-4947-0615-7183-E9B06B221A7E";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "9E7A4394-4063-AB6F-792F-999D79A0100D";
createNode follicle -n "pSphere2FollicleShape4028" -p "pSphere2Follicle4028";
	rename -uid "BA378DAB-4FF2-B489-5F2D-3492B09E5248";
	setAttr -k off ".v";
	setAttr ".pu" 0.40625;
	setAttr ".pv" 0.28125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve334" -p "pSphere2Follicle4028";
	rename -uid "6144B31B-4843-346B-B6F0-319F0C0B682D";
createNode nurbsCurve -n "curveShape334" -p "curve334";
	rename -uid "9EAB9785-47AB-3060-9C1B-D789DAB13379";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "3B3B0A56-49B7-F80E-E600-1E862A10A266";
createNode follicle -n "pSphere2FollicleShape4034" -p "pSphere2Follicle4034";
	rename -uid "6D43C977-4574-B9A6-CF4D-27994CF90540";
	setAttr -k off ".v";
	setAttr ".pu" 0.40625;
	setAttr ".pv" 0.34375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve335" -p "pSphere2Follicle4034";
	rename -uid "09DDD5E0-4FC0-22AC-605E-B29F84DFBF17";
createNode nurbsCurve -n "curveShape335" -p "curve335";
	rename -uid "A000BE5E-46B7-4A89-792B-F4AD898574FB";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "B224994D-4D7B-738E-9478-B39CB7E234B5";
createNode follicle -n "pSphere2FollicleShape4040" -p "pSphere2Follicle4040";
	rename -uid "5222BEEF-4BF9-B0E4-0F83-379CFB21573A";
	setAttr -k off ".v";
	setAttr ".pu" 0.40625;
	setAttr ".pv" 0.40625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve336" -p "pSphere2Follicle4040";
	rename -uid "A9B28535-4B1E-3456-A409-618B9691E98F";
createNode nurbsCurve -n "curveShape336" -p "curve336";
	rename -uid "D246DAF6-467F-B559-05FF-44BA131940AF";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "88CE1731-4E1C-31A8-06EA-5F8D6ACD0774";
createNode follicle -n "pSphere2FollicleShape4046" -p "pSphere2Follicle4046";
	rename -uid "95ECDD16-41B1-B8A1-67FB-64AA358161B0";
	setAttr -k off ".v";
	setAttr ".pu" 0.40625;
	setAttr ".pv" 0.46875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve337" -p "pSphere2Follicle4046";
	rename -uid "1373435E-49B6-D437-90D3-C6A534A7B84E";
createNode nurbsCurve -n "curveShape337" -p "curve337";
	rename -uid "866FD06C-4DA0-9A4A-81BA-F5996B99E16E";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "12FF5716-4FB0-029D-A045-A6B868CE9954";
createNode follicle -n "pSphere2FollicleShape4053" -p "pSphere2Follicle4053";
	rename -uid "AF728011-4B5B-7A54-7B6A-2491951FA05C";
	setAttr -k off ".v";
	setAttr ".pu" 0.40625;
	setAttr ".pv" 0.53125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve338" -p "pSphere2Follicle4053";
	rename -uid "2DDAC67A-4129-1129-ECD2-7BADDA939A4A";
createNode nurbsCurve -n "curveShape338" -p "curve338";
	rename -uid "149E7D67-4833-157D-A120-8D8421C4C34C";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "D83EA22E-471F-9F61-F005-DB8036D08491";
createNode follicle -n "pSphere2FollicleShape4059" -p "pSphere2Follicle4059";
	rename -uid "AAAF63E3-4031-A16E-B9AA-73B61F7B471F";
	setAttr -k off ".v";
	setAttr ".pu" 0.40625;
	setAttr ".pv" 0.59375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve339" -p "pSphere2Follicle4059";
	rename -uid "0B4D5547-452C-23A3-C7E6-98B4D17D0D5D";
createNode nurbsCurve -n "curveShape339" -p "curve339";
	rename -uid "B0C06323-4916-AC33-0A9E-619B781EDBCD";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "4250C454-4A8A-C08B-2B3B-A78109535BEA";
createNode follicle -n "pSphere2FollicleShape4065" -p "pSphere2Follicle4065";
	rename -uid "350A1757-4DA4-D084-B138-E6B35A5E7747";
	setAttr -k off ".v";
	setAttr ".pu" 0.40625;
	setAttr ".pv" 0.65625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve340" -p "pSphere2Follicle4065";
	rename -uid "BD52035D-42AA-E965-A350-0AA9705DF2FC";
createNode nurbsCurve -n "curveShape340" -p "curve340";
	rename -uid "11FFD29B-4539-E682-BC5D-8ABE739E5396";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "ECCDB0CB-4C1D-8913-53D6-70B84511458C";
createNode follicle -n "pSphere2FollicleShape4071" -p "pSphere2Follicle4071";
	rename -uid "C02B5D37-4342-3894-9283-CA8A7B5CB995";
	setAttr -k off ".v";
	setAttr ".pu" 0.40625;
	setAttr ".pv" 0.71875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve341" -p "pSphere2Follicle4071";
	rename -uid "4C9B4D0F-47A4-8D0C-1E75-2480EF105EB9";
createNode nurbsCurve -n "curveShape341" -p "curve341";
	rename -uid "276AE980-450A-CB5D-44C2-FA8FB974AB8B";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "32FB2ECB-4248-9C70-1278-1DA9ABF3E84A";
createNode follicle -n "pSphere2FollicleShape4077" -p "pSphere2Follicle4077";
	rename -uid "73EFD631-466F-A7A2-B78A-ACB385754B06";
	setAttr -k off ".v";
	setAttr ".pu" 0.40625;
	setAttr ".pv" 0.78125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve342" -p "pSphere2Follicle4077";
	rename -uid "ED1E112D-4A61-3584-F558-6C91A217343A";
createNode nurbsCurve -n "curveShape342" -p "curve342";
	rename -uid "2F019A8E-4E83-A889-CB4E-E39D45E7E405";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "73F98625-40F5-069D-1BDF-E6B37B9439B7";
createNode follicle -n "pSphere2FollicleShape4084" -p "pSphere2Follicle4084";
	rename -uid "99B46F47-451F-3695-7ED7-C5975AEEA3CC";
	setAttr -k off ".v";
	setAttr ".pu" 0.40625;
	setAttr ".pv" 0.84375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve343" -p "pSphere2Follicle4084";
	rename -uid "D5844225-47D7-B969-111C-80877CA0A792";
createNode nurbsCurve -n "curveShape343" -p "curve343";
	rename -uid "ABCB4FC3-413E-DDE9-AAE7-67B4806907B0";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "68AC528F-474C-BFC8-DDC2-37B7008C7605";
createNode follicle -n "pSphere2FollicleShape4090" -p "pSphere2Follicle4090";
	rename -uid "A7F9929A-476A-C71A-B507-97B99B526311";
	setAttr -k off ".v";
	setAttr ".pu" 0.40625;
	setAttr ".pv" 0.90625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve344" -p "pSphere2Follicle4090";
	rename -uid "75D6492E-4207-3C06-C132-0BA5778B7EB5";
createNode nurbsCurve -n "curveShape344" -p "curve344";
	rename -uid "43E17C19-4F35-9C4D-9335-8F81E5896AE9";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "F8337D02-4F47-3B5F-7EB1-EC8B8F52245F";
createNode follicle -n "pSphere2FollicleShape4603" -p "pSphere2Follicle4603";
	rename -uid "6BB906AE-4E25-5440-335A-BD8B568E2F85";
	setAttr -k off ".v";
	setAttr ".pu" 0.46875;
	setAttr ".pv" 0.03125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve345" -p "pSphere2Follicle4603";
	rename -uid "35CC0B48-402F-8674-A219-CE82FB8BE19D";
createNode nurbsCurve -n "curveShape345" -p "curve345";
	rename -uid "AF20356C-4B63-DB18-632E-5D8B9F981B06";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "455CBF18-40FD-D57C-BBCC-289D8756F422";
createNode follicle -n "pSphere2FollicleShape4609" -p "pSphere2Follicle4609";
	rename -uid "63B3645C-4B24-28B7-9AAA-99B5695C2484";
	setAttr -k off ".v";
	setAttr ".pu" 0.46875;
	setAttr ".pv" 0.09375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve346" -p "pSphere2Follicle4609";
	rename -uid "4F1C22E3-41B8-243E-46E3-D19E7387D270";
createNode nurbsCurve -n "curveShape346" -p "curve346";
	rename -uid "584393D4-408C-620B-F25B-BA8EB2D99BE8";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "E5E3C4E7-4F5B-0F2E-1162-5BB5BCF9355D";
createNode follicle -n "pSphere2FollicleShape4615" -p "pSphere2Follicle4615";
	rename -uid "10DBF839-425C-B0E0-8DFC-61967C6E5641";
	setAttr -k off ".v";
	setAttr ".pu" 0.46875;
	setAttr ".pv" 0.15625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve347" -p "pSphere2Follicle4615";
	rename -uid "84551088-4A6D-F0EC-EB5D-489900702BE4";
createNode nurbsCurve -n "curveShape347" -p "curve347";
	rename -uid "CCAEFD69-4C32-F54C-584E-D19851425478";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "1C880855-489A-B6A8-D560-FA9272218DDB";
createNode follicle -n "pSphere2FollicleShape4622" -p "pSphere2Follicle4622";
	rename -uid "127C6896-429A-8F33-4CD8-FF90CCFA62B9";
	setAttr -k off ".v";
	setAttr ".pu" 0.46875;
	setAttr ".pv" 0.21875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve348" -p "pSphere2Follicle4622";
	rename -uid "6CC841B0-4673-FA3B-689E-5A826F1B99AF";
createNode nurbsCurve -n "curveShape348" -p "curve348";
	rename -uid "5CA64605-454C-7BBD-DCD4-D4903F96A73A";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "E6CEE410-470E-1C91-2D21-7499010F5412";
createNode follicle -n "pSphere2FollicleShape4628" -p "pSphere2Follicle4628";
	rename -uid "3F7B0026-4293-79F5-BDAD-22BFCEE3ED1E";
	setAttr -k off ".v";
	setAttr ".pu" 0.46875;
	setAttr ".pv" 0.28125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve349" -p "pSphere2Follicle4628";
	rename -uid "712F0DFC-467D-D052-DFA0-FBA57A4FFCA6";
createNode nurbsCurve -n "curveShape349" -p "curve349";
	rename -uid "B465D5A7-4C39-0EC9-B9F4-9B9F0B1E443C";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "DAABB016-4794-3E29-F2AE-EEA3486B9673";
createNode follicle -n "pSphere2FollicleShape4634" -p "pSphere2Follicle4634";
	rename -uid "30CAEEDF-4B10-45A3-7EAE-0BB413569869";
	setAttr -k off ".v";
	setAttr ".pu" 0.46875;
	setAttr ".pv" 0.34375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve350" -p "pSphere2Follicle4634";
	rename -uid "DD91089A-45E1-C4AB-9E7C-E18AD8B4B9D1";
createNode nurbsCurve -n "curveShape350" -p "curve350";
	rename -uid "7F654B1A-4BEC-9CFD-A418-D2BBB7D08665";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "572C7B12-4C80-2E97-395B-98B8567687DB";
createNode follicle -n "pSphere2FollicleShape4640" -p "pSphere2Follicle4640";
	rename -uid "AE442578-407D-9CFE-A71A-A2837F79B2DC";
	setAttr -k off ".v";
	setAttr ".pu" 0.46875;
	setAttr ".pv" 0.40625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve351" -p "pSphere2Follicle4640";
	rename -uid "CDE39DCA-4280-2AF7-F32D-1399E5D3EED4";
createNode nurbsCurve -n "curveShape351" -p "curve351";
	rename -uid "6FBB4E7D-4A9C-A931-BF00-7290BB596AC6";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "41CDA235-4D87-164B-EF7A-C8BC7C58FBF4";
createNode follicle -n "pSphere2FollicleShape4646" -p "pSphere2Follicle4646";
	rename -uid "BC71B1D1-46CB-3B27-B1EF-52BC2D4E233A";
	setAttr -k off ".v";
	setAttr ".pu" 0.46875;
	setAttr ".pv" 0.46875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve352" -p "pSphere2Follicle4646";
	rename -uid "2C7C0F30-4DF5-D120-293A-96B7A4CEC30F";
createNode nurbsCurve -n "curveShape352" -p "curve352";
	rename -uid "921682F0-4D44-CA59-D0D1-9D80EA12B807";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "D6A10392-4867-3241-E62D-DA8581B46CD8";
createNode follicle -n "pSphere2FollicleShape4653" -p "pSphere2Follicle4653";
	rename -uid "108D189E-4DE7-93EA-42A0-2E8C62997C73";
	setAttr -k off ".v";
	setAttr ".pu" 0.46875;
	setAttr ".pv" 0.53125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve353" -p "pSphere2Follicle4653";
	rename -uid "9651D311-4FAC-E9F6-21C3-C4B4651B4D31";
createNode nurbsCurve -n "curveShape353" -p "curve353";
	rename -uid "9AD2CAC0-4A60-265C-732D-54A3F5E71D27";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "55E8CDE3-4C63-F6FA-30CB-43B3F196EB4F";
createNode follicle -n "pSphere2FollicleShape4659" -p "pSphere2Follicle4659";
	rename -uid "8C7DFD74-4E48-E1B9-4CE8-D695F6DACF68";
	setAttr -k off ".v";
	setAttr ".pu" 0.46875;
	setAttr ".pv" 0.59375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve354" -p "pSphere2Follicle4659";
	rename -uid "8F1DC591-4114-9639-54BD-7E8E9BF884D5";
createNode nurbsCurve -n "curveShape354" -p "curve354";
	rename -uid "D05EFF12-456B-9554-7C1E-2CAB7DD04E9A";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "C410149C-4E9C-33E9-51A9-6B808B9DEDCA";
createNode follicle -n "pSphere2FollicleShape4665" -p "pSphere2Follicle4665";
	rename -uid "EFD7DBCB-46B7-7C75-EAF8-508828DE5D95";
	setAttr -k off ".v";
	setAttr ".pu" 0.46875;
	setAttr ".pv" 0.65625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve355" -p "pSphere2Follicle4665";
	rename -uid "D5F9EB74-4F8B-B1EF-7778-D594ABD816B5";
createNode nurbsCurve -n "curveShape355" -p "curve355";
	rename -uid "1C54D058-4DFB-808C-97AC-42939EFCFF2E";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "CAD5F967-41E4-0AB4-ECF4-E58F3DFA2323";
createNode follicle -n "pSphere2FollicleShape4671" -p "pSphere2Follicle4671";
	rename -uid "6709653A-40E6-BD0B-22AE-C698B11AB208";
	setAttr -k off ".v";
	setAttr ".pu" 0.46875;
	setAttr ".pv" 0.71875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve356" -p "pSphere2Follicle4671";
	rename -uid "F2462B93-42B7-FD1C-A708-C5B6E32C872E";
createNode nurbsCurve -n "curveShape356" -p "curve356";
	rename -uid "16AD7451-464F-EA59-36CB-5D9A792C1B3F";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "5EA569BD-46A6-46BE-9E34-3384799F58DB";
createNode follicle -n "pSphere2FollicleShape4677" -p "pSphere2Follicle4677";
	rename -uid "5C6C5252-45F9-DE9B-3039-C499699CA1A7";
	setAttr -k off ".v";
	setAttr ".pu" 0.46875;
	setAttr ".pv" 0.78125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve357" -p "pSphere2Follicle4677";
	rename -uid "290BF711-4FCC-B59E-507C-F88F372A6C02";
createNode nurbsCurve -n "curveShape357" -p "curve357";
	rename -uid "DDA6BE1F-4508-0764-A78D-C59BBF2B2BE6";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "3FB313C4-4F9E-0043-F131-62AB3B5D65F9";
createNode follicle -n "pSphere2FollicleShape4684" -p "pSphere2Follicle4684";
	rename -uid "EA18AD59-4E12-C1E2-B45C-7CA8696C04A1";
	setAttr -k off ".v";
	setAttr ".pu" 0.46875;
	setAttr ".pv" 0.84375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve358" -p "pSphere2Follicle4684";
	rename -uid "A287474C-4D18-B71D-6E57-0BB4260855DF";
createNode nurbsCurve -n "curveShape358" -p "curve358";
	rename -uid "1C385488-495D-C057-4803-E6AF5195B228";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "37C1B01D-42C9-A3B2-031E-7593FD87B226";
createNode follicle -n "pSphere2FollicleShape4690" -p "pSphere2Follicle4690";
	rename -uid "C2C76538-4984-95FD-48B9-B0A41027021F";
	setAttr -k off ".v";
	setAttr ".pu" 0.46875;
	setAttr ".pv" 0.90625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve359" -p "pSphere2Follicle4690";
	rename -uid "74537E22-4FCE-E911-2250-B1A167F9F19F";
createNode nurbsCurve -n "curveShape359" -p "curve359";
	rename -uid "686FFC72-40A6-D853-F6B3-66AF3FBB1026";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "12D2D7D3-4112-2DA8-4F2A-89A4E00D9088";
createNode follicle -n "pSphere2FollicleShape4696" -p "pSphere2Follicle4696";
	rename -uid "69DE0583-42E8-3051-C662-CEA2089967A7";
	setAttr -k off ".v";
	setAttr ".pu" 0.46875;
	setAttr ".pv" 0.96875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve360" -p "pSphere2Follicle4696";
	rename -uid "3AEDD4A3-4F04-1E7D-F240-EABEDFA2F11C";
createNode nurbsCurve -n "curveShape360" -p "curve360";
	rename -uid "1DF85ED7-49E4-C168-454D-18A0D55FBC59";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "4E121EDC-4862-5518-F17D-72BE09E15053";
createNode follicle -n "pSphere2FollicleShape5303" -p "pSphere2Follicle5303";
	rename -uid "1EECDF6C-4C82-FEAE-D35A-7593EC7EFC0F";
	setAttr -k off ".v";
	setAttr ".pu" 0.53125;
	setAttr ".pv" 0.03125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve361" -p "pSphere2Follicle5303";
	rename -uid "494DDD86-4AEC-CDE6-E66D-3C8596F7D57F";
createNode nurbsCurve -n "curveShape361" -p "curve361";
	rename -uid "5B8D0D78-46CC-793B-1F49-338F6D48215E";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "B8A36858-4D09-FC58-FACE-0B94D485D077";
createNode follicle -n "pSphere2FollicleShape5309" -p "pSphere2Follicle5309";
	rename -uid "5A6F0A78-4CB7-18E7-73F3-67A9C2AC4805";
	setAttr -k off ".v";
	setAttr ".pu" 0.53125;
	setAttr ".pv" 0.09375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve362" -p "pSphere2Follicle5309";
	rename -uid "86736460-4766-4AFC-D9DA-83B70660BD8F";
createNode nurbsCurve -n "curveShape362" -p "curve362";
	rename -uid "9ECD98FF-4812-8EC4-F9F9-35B3220595BA";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "4B6CA0F8-4E00-35BC-F0E3-77AA2169D4E1";
createNode follicle -n "pSphere2FollicleShape5315" -p "pSphere2Follicle5315";
	rename -uid "D5D428BC-46E0-266B-07A3-9388F66513C9";
	setAttr -k off ".v";
	setAttr ".pu" 0.53125;
	setAttr ".pv" 0.15625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve363" -p "pSphere2Follicle5315";
	rename -uid "74AA75DF-42C9-E4E5-F5A2-FF9BDB1AEEEE";
createNode nurbsCurve -n "curveShape363" -p "curve363";
	rename -uid "94E528A9-4C1D-72B6-2070-6C9459CC8379";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "1F64967C-40FA-8794-8DC7-E48EF04245C9";
createNode follicle -n "pSphere2FollicleShape5322" -p "pSphere2Follicle5322";
	rename -uid "BB880DE5-4D17-7C7B-A99F-00A4F34E44F8";
	setAttr -k off ".v";
	setAttr ".pu" 0.53125;
	setAttr ".pv" 0.21875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve364" -p "pSphere2Follicle5322";
	rename -uid "A2C2E8D8-4816-4456-B4DC-EA81C05D75D5";
createNode nurbsCurve -n "curveShape364" -p "curve364";
	rename -uid "864A652F-4882-1E45-56CF-CEB4C25C0F7E";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "7D40ACCC-40F8-7804-C0AB-3689A6F2E029";
createNode follicle -n "pSphere2FollicleShape5328" -p "pSphere2Follicle5328";
	rename -uid "586AB2F9-4F2B-7BEF-48E8-EE8EEE76C3CB";
	setAttr -k off ".v";
	setAttr ".pu" 0.53125;
	setAttr ".pv" 0.28125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve365" -p "pSphere2Follicle5328";
	rename -uid "DC591D59-4675-A6F4-CD60-A59CCCA14DB6";
createNode nurbsCurve -n "curveShape365" -p "curve365";
	rename -uid "1869D0C0-4338-7C80-F666-74900FFA42F4";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "3D6F3DEA-4F30-3AB2-77CD-A8BD34444EBA";
createNode follicle -n "pSphere2FollicleShape5334" -p "pSphere2Follicle5334";
	rename -uid "899E9DFC-4894-0308-8D63-DDAB59A967FB";
	setAttr -k off ".v";
	setAttr ".pu" 0.53125;
	setAttr ".pv" 0.34375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve366" -p "pSphere2Follicle5334";
	rename -uid "C5BD9048-437D-744C-0B07-6D9A17C8D65D";
createNode nurbsCurve -n "curveShape366" -p "curve366";
	rename -uid "9DA6D523-4C24-BF4E-FA3C-88B6E08D1195";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "3230B866-4B1E-6738-9281-FC825FE4E22D";
createNode follicle -n "pSphere2FollicleShape5340" -p "pSphere2Follicle5340";
	rename -uid "3F87F92E-4200-DB58-593F-BF897C71B653";
	setAttr -k off ".v";
	setAttr ".pu" 0.53125;
	setAttr ".pv" 0.40625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve367" -p "pSphere2Follicle5340";
	rename -uid "4D48C60E-403C-3CF1-A81D-338FA9EC9948";
createNode nurbsCurve -n "curveShape367" -p "curve367";
	rename -uid "7B62E5EB-4AFE-C679-95A1-23853239B08D";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "6AD2841E-4BF8-5A66-469C-F8B5F604E414";
createNode follicle -n "pSphere2FollicleShape5346" -p "pSphere2Follicle5346";
	rename -uid "BB3341F4-4A15-3277-8322-5D9D7684B002";
	setAttr -k off ".v";
	setAttr ".pu" 0.53125;
	setAttr ".pv" 0.46875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve368" -p "pSphere2Follicle5346";
	rename -uid "3C81F0E8-4AD9-C60D-D9D1-C8A5E85EE5EA";
createNode nurbsCurve -n "curveShape368" -p "curve368";
	rename -uid "5FD8F1BB-43C7-2860-8984-6685DF43BE7E";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "EBD3CD3A-449D-AB41-2A4C-BD9968FC4904";
createNode follicle -n "pSphere2FollicleShape5353" -p "pSphere2Follicle5353";
	rename -uid "2A4D9BCD-44D5-7056-BBBC-A8ABF2E079D5";
	setAttr -k off ".v";
	setAttr ".pu" 0.53125;
	setAttr ".pv" 0.53125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve369" -p "pSphere2Follicle5353";
	rename -uid "C5EFE0FC-4CC1-9A78-7EE7-21B21BFB2BCA";
createNode nurbsCurve -n "curveShape369" -p "curve369";
	rename -uid "EC021D33-4EF5-64D3-E527-6E9DE2486E9D";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "6290F8D6-485C-E7A5-97FA-DAAADC8F9CEF";
createNode follicle -n "pSphere2FollicleShape5359" -p "pSphere2Follicle5359";
	rename -uid "A77F9A75-4FB4-4D3D-F5D5-56BCF71B98D3";
	setAttr -k off ".v";
	setAttr ".pu" 0.53125;
	setAttr ".pv" 0.59375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve370" -p "pSphere2Follicle5359";
	rename -uid "B2C8D0B4-4CD1-BD5D-4191-C58357C7B202";
createNode nurbsCurve -n "curveShape370" -p "curve370";
	rename -uid "62306CE9-4EAC-3143-2114-52B9087B20DB";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "7267A36F-47C3-DDA2-E717-04BC5150CE1B";
createNode follicle -n "pSphere2FollicleShape5365" -p "pSphere2Follicle5365";
	rename -uid "FFDC460E-4B73-26C4-4A39-C0A31AF42A10";
	setAttr -k off ".v";
	setAttr ".pu" 0.53125;
	setAttr ".pv" 0.65625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve371" -p "pSphere2Follicle5365";
	rename -uid "302D673F-4773-E88B-128E-6981860C80D6";
createNode nurbsCurve -n "curveShape371" -p "curve371";
	rename -uid "46BF6F6D-4BB9-7543-1768-BAA5E6E45EB3";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "C1CD25F3-42FC-827E-D4C1-93906621B8CA";
createNode follicle -n "pSphere2FollicleShape5371" -p "pSphere2Follicle5371";
	rename -uid "A039590F-4167-D30D-A592-F2B54DD158B2";
	setAttr -k off ".v";
	setAttr ".pu" 0.53125;
	setAttr ".pv" 0.71875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve372" -p "pSphere2Follicle5371";
	rename -uid "43E6B359-44D9-9A55-9FFE-FFB98F24D29D";
createNode nurbsCurve -n "curveShape372" -p "curve372";
	rename -uid "1AFC53CF-4C9D-C435-CB77-F690733B1D0F";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "E70A878A-44B3-D27B-364C-E19F8148FD9C";
createNode follicle -n "pSphere2FollicleShape5377" -p "pSphere2Follicle5377";
	rename -uid "71724486-4EA1-BB81-24C5-57A5CDAF71C0";
	setAttr -k off ".v";
	setAttr ".pu" 0.53125;
	setAttr ".pv" 0.78125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve373" -p "pSphere2Follicle5377";
	rename -uid "E7F03850-406D-C022-1DDD-C2B2684C371C";
createNode nurbsCurve -n "curveShape373" -p "curve373";
	rename -uid "10BFA140-45BF-205E-78A0-3DB17DA7A837";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "5BD219BE-49E0-4828-79E4-B38B7BFEFF3C";
createNode follicle -n "pSphere2FollicleShape5384" -p "pSphere2Follicle5384";
	rename -uid "8B5CAFC3-479B-0CF8-0D82-639373968A2B";
	setAttr -k off ".v";
	setAttr ".pu" 0.53125;
	setAttr ".pv" 0.84375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve374" -p "pSphere2Follicle5384";
	rename -uid "7D3D34D2-4057-F007-C30F-16A9CFDB8639";
createNode nurbsCurve -n "curveShape374" -p "curve374";
	rename -uid "2BD21F1C-4298-04A3-5B03-17BD8DCEAD28";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "F4AD4BCC-4290-5C56-5EAC-AF8D1A1AB178";
createNode follicle -n "pSphere2FollicleShape5390" -p "pSphere2Follicle5390";
	rename -uid "EE6746CE-454C-9B27-AECF-C9916FD89082";
	setAttr -k off ".v";
	setAttr ".pu" 0.53125;
	setAttr ".pv" 0.90625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve375" -p "pSphere2Follicle5390";
	rename -uid "073A0AEA-493D-2BED-60D5-B99D32388B94";
createNode nurbsCurve -n "curveShape375" -p "curve375";
	rename -uid "ED5D80A0-44B2-90A4-767A-8EA026486D54";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "D676FB68-40C9-0BB5-B7A6-6DAEF05ACAD4";
createNode follicle -n "pSphere2FollicleShape5396" -p "pSphere2Follicle5396";
	rename -uid "382A4E25-4C5D-894D-AAB8-DE96A253FF7C";
	setAttr -k off ".v";
	setAttr ".pu" 0.53125;
	setAttr ".pv" 0.96875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve376" -p "pSphere2Follicle5396";
	rename -uid "4891AC76-40F2-C580-35E1-C290A5E0B50C";
createNode nurbsCurve -n "curveShape376" -p "curve376";
	rename -uid "995F29E1-4D98-586E-2FBF-06B9BBA14B9E";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "E525164C-40D3-521F-3220-74BBF60B7B3D";
createNode follicle -n "pSphere2FollicleShape5909" -p "pSphere2Follicle5909";
	rename -uid "4A057E1D-4182-74C4-A7D1-17BF8F7D2606";
	setAttr -k off ".v";
	setAttr ".pu" 0.59375;
	setAttr ".pv" 0.09375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve377" -p "pSphere2Follicle5909";
	rename -uid "8AD5D371-44DC-A7E9-E326-EE9315E16137";
createNode nurbsCurve -n "curveShape377" -p "curve377";
	rename -uid "99C242FB-4C88-BA09-675A-83A0A29337AE";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "30EFB188-4B4B-3FD7-8044-8785245AEC7C";
createNode follicle -n "pSphere2FollicleShape5915" -p "pSphere2Follicle5915";
	rename -uid "6BAFAA3B-4C7C-8614-04F9-5BA77D97ACD4";
	setAttr -k off ".v";
	setAttr ".pu" 0.59375;
	setAttr ".pv" 0.15625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve378" -p "pSphere2Follicle5915";
	rename -uid "55F28A52-4580-15EF-8F33-E9BA1CED31FD";
createNode nurbsCurve -n "curveShape378" -p "curve378";
	rename -uid "511AEC4E-4BCA-D28A-958B-678E9C7AB62E";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "29E200E5-463D-B1CE-5A98-F3AF92DD642A";
createNode follicle -n "pSphere2FollicleShape5922" -p "pSphere2Follicle5922";
	rename -uid "D4A69A38-4113-C562-A5FE-A4A72B0D3B49";
	setAttr -k off ".v";
	setAttr ".pu" 0.59375;
	setAttr ".pv" 0.21875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve379" -p "pSphere2Follicle5922";
	rename -uid "44338860-439E-6E61-F42E-F5BBDF3FEDAC";
createNode nurbsCurve -n "curveShape379" -p "curve379";
	rename -uid "49DFD084-440A-5CBE-00D5-0ABE7CB7BECF";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "646C351F-4C5E-FB63-F56E-E5B9680E2B6B";
createNode follicle -n "pSphere2FollicleShape5928" -p "pSphere2Follicle5928";
	rename -uid "0441D471-40FF-7976-A925-B0B45AFCC488";
	setAttr -k off ".v";
	setAttr ".pu" 0.59375;
	setAttr ".pv" 0.28125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve380" -p "pSphere2Follicle5928";
	rename -uid "C4233071-48C3-47E4-0537-3E8532EA9F79";
createNode nurbsCurve -n "curveShape380" -p "curve380";
	rename -uid "E44D1952-47EA-80F7-7109-2099B4C6D2B9";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "9DDFDB5A-42C9-DC28-33C2-4DAB5FC8ED75";
createNode follicle -n "pSphere2FollicleShape5934" -p "pSphere2Follicle5934";
	rename -uid "25DC9549-4A2E-200D-C245-A8AAD735EDC9";
	setAttr -k off ".v";
	setAttr ".pu" 0.59375;
	setAttr ".pv" 0.34375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve381" -p "pSphere2Follicle5934";
	rename -uid "B2279F47-4A68-5445-BE1B-AF82B6FA1B01";
createNode nurbsCurve -n "curveShape381" -p "curve381";
	rename -uid "92B97458-4F61-8ECE-7DF7-77953A528E3D";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "733F99B6-40CB-13A2-D292-86993F8C025F";
createNode follicle -n "pSphere2FollicleShape5940" -p "pSphere2Follicle5940";
	rename -uid "873CD28E-4347-2383-C5FF-459855C88615";
	setAttr -k off ".v";
	setAttr ".pu" 0.59375;
	setAttr ".pv" 0.40625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve382" -p "pSphere2Follicle5940";
	rename -uid "BC7AC8C6-4B7B-0E1C-FE90-67A71E58F34C";
createNode nurbsCurve -n "curveShape382" -p "curve382";
	rename -uid "AB01B5E0-4FDE-1B52-ED0F-9881232C482F";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "1E73E450-4FB3-C340-2CD3-4FBFBCADA4AF";
createNode follicle -n "pSphere2FollicleShape5946" -p "pSphere2Follicle5946";
	rename -uid "F1CFA801-4F68-F692-7507-68806B780045";
	setAttr -k off ".v";
	setAttr ".pu" 0.59375;
	setAttr ".pv" 0.46875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve383" -p "pSphere2Follicle5946";
	rename -uid "E33B40C4-42D0-1D2D-B935-D9A7D03F1834";
createNode nurbsCurve -n "curveShape383" -p "curve383";
	rename -uid "BEED245A-4F99-AF4A-E4BE-33803C4CF2B2";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "8FC906B7-4270-68ED-2896-3DAC9E311B29";
createNode follicle -n "pSphere2FollicleShape5953" -p "pSphere2Follicle5953";
	rename -uid "879C590B-4189-0331-290D-E89650C72CD3";
	setAttr -k off ".v";
	setAttr ".pu" 0.59375;
	setAttr ".pv" 0.53125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve384" -p "pSphere2Follicle5953";
	rename -uid "36B299BE-485E-F4C3-3F37-F9A9567F1F1A";
createNode nurbsCurve -n "curveShape384" -p "curve384";
	rename -uid "5C90AF7A-40ED-7397-8241-16B9D5193315";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "6A6F0F8C-49BB-68DB-B9A3-C7A77F507D5E";
createNode follicle -n "pSphere2FollicleShape5959" -p "pSphere2Follicle5959";
	rename -uid "7B842AFD-4BC4-F890-F8D4-448F031CA7F6";
	setAttr -k off ".v";
	setAttr ".pu" 0.59375;
	setAttr ".pv" 0.59375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve385" -p "pSphere2Follicle5959";
	rename -uid "0351C21E-4FDF-BA65-F5AB-C3A4E74676CB";
createNode nurbsCurve -n "curveShape385" -p "curve385";
	rename -uid "0C194CE6-4071-EB4F-EDCA-3DAE8432AB7D";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "3169B8B6-49BF-349B-DA78-05A74E4E65E3";
createNode follicle -n "pSphere2FollicleShape5965" -p "pSphere2Follicle5965";
	rename -uid "CB87E616-4F85-D1C6-3EF2-338C53A2C4E1";
	setAttr -k off ".v";
	setAttr ".pu" 0.59375;
	setAttr ".pv" 0.65625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve386" -p "pSphere2Follicle5965";
	rename -uid "3D413CD1-4681-569C-C402-5EBC3547237C";
createNode nurbsCurve -n "curveShape386" -p "curve386";
	rename -uid "697EF3A5-44BC-514B-9065-FEB6BE727668";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "69A41481-40E5-DF92-BBCB-51997689F97B";
createNode follicle -n "pSphere2FollicleShape5971" -p "pSphere2Follicle5971";
	rename -uid "3BDBB9D0-435E-25AC-5EE7-57BDD6338371";
	setAttr -k off ".v";
	setAttr ".pu" 0.59375;
	setAttr ".pv" 0.71875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve387" -p "pSphere2Follicle5971";
	rename -uid "1D2E2158-46AC-21D7-3101-029105D506CC";
createNode nurbsCurve -n "curveShape387" -p "curve387";
	rename -uid "6E2C30F3-4E92-5249-7A1E-5896A1CD2C8E";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "FB9767E2-445D-F2FF-B633-6E84FFBC8AC5";
createNode follicle -n "pSphere2FollicleShape5977" -p "pSphere2Follicle5977";
	rename -uid "859F09D5-4BF6-3605-41B1-E091922E5393";
	setAttr -k off ".v";
	setAttr ".pu" 0.59375;
	setAttr ".pv" 0.78125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve388" -p "pSphere2Follicle5977";
	rename -uid "8238404C-4257-3A25-95C7-D4A5DFA0FAB3";
createNode nurbsCurve -n "curveShape388" -p "curve388";
	rename -uid "6DB9D6E6-4614-2F13-CB94-CC86D13D79A5";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "6CF50BE4-4443-338D-116D-9183ADDFBFBF";
createNode follicle -n "pSphere2FollicleShape5984" -p "pSphere2Follicle5984";
	rename -uid "CD5BA0BA-4491-A7AF-8326-F5B06BB9DB02";
	setAttr -k off ".v";
	setAttr ".pu" 0.59375;
	setAttr ".pv" 0.84375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve389" -p "pSphere2Follicle5984";
	rename -uid "76F78AC4-4FCA-3CA6-8DDE-719712539318";
createNode nurbsCurve -n "curveShape389" -p "curve389";
	rename -uid "96E6291E-424F-1EBC-6E7C-4EB628EA7A5B";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "BE015B1A-4B79-ED3E-C79D-769E5D2CF4BC";
createNode follicle -n "pSphere2FollicleShape5990" -p "pSphere2Follicle5990";
	rename -uid "A9F1524A-4018-7232-3F98-2E979C33D61C";
	setAttr -k off ".v";
	setAttr ".pu" 0.59375;
	setAttr ".pv" 0.90625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve390" -p "pSphere2Follicle5990";
	rename -uid "1A07EBBC-45AF-9C80-3EB5-ECB65CD022AF";
createNode nurbsCurve -n "curveShape390" -p "curve390";
	rename -uid "90288507-45E2-40E7-294A-EFB95C2E35CC";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "EC675BC1-44D5-1404-7918-FEBE3CAA3AA3";
createNode follicle -n "pSphere2FollicleShape6509" -p "pSphere2Follicle6509";
	rename -uid "2AFBD71E-480F-E6B2-4310-F08B307379CA";
	setAttr -k off ".v";
	setAttr ".pu" 0.65625;
	setAttr ".pv" 0.09375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve391" -p "pSphere2Follicle6509";
	rename -uid "D13C08F6-4739-34EF-2656-DDB733036CC5";
createNode nurbsCurve -n "curveShape391" -p "curve391";
	rename -uid "740E8E58-4867-9576-15DC-E9B1D05C9DE8";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "8C1133E9-43CB-7A5F-E7E1-CEBEC1EC980B";
createNode follicle -n "pSphere2FollicleShape6515" -p "pSphere2Follicle6515";
	rename -uid "7E964359-41E0-4E60-82FE-ED9AEBDBC702";
	setAttr -k off ".v";
	setAttr ".pu" 0.65625;
	setAttr ".pv" 0.15625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve392" -p "pSphere2Follicle6515";
	rename -uid "2850B45E-47B6-A87A-6C04-0AA3F3FB8D98";
createNode nurbsCurve -n "curveShape392" -p "curve392";
	rename -uid "2212DD3A-4EDF-0C8B-F40C-B79DB5DA5014";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "EB5CB8A8-4834-AA72-BD84-11B1A32CC5B6";
createNode follicle -n "pSphere2FollicleShape6522" -p "pSphere2Follicle6522";
	rename -uid "098218F1-4ABD-2F85-B28C-2D889BEC3954";
	setAttr -k off ".v";
	setAttr ".pu" 0.65625;
	setAttr ".pv" 0.21875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve393" -p "pSphere2Follicle6522";
	rename -uid "109C9E6F-4600-CFBF-9230-E8820D44873D";
createNode nurbsCurve -n "curveShape393" -p "curve393";
	rename -uid "448E2CFF-46B7-1F40-F0FC-19A38EA34F4B";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "837CB1A8-4438-12E4-7BBD-2B931FE20A2F";
createNode follicle -n "pSphere2FollicleShape6528" -p "pSphere2Follicle6528";
	rename -uid "7A307F3C-4929-99BD-987C-33A2D9ECE81F";
	setAttr -k off ".v";
	setAttr ".pu" 0.65625;
	setAttr ".pv" 0.28125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve394" -p "pSphere2Follicle6528";
	rename -uid "26DA6C08-4AB5-C7DE-F228-1680DEC40D21";
createNode nurbsCurve -n "curveShape394" -p "curve394";
	rename -uid "26FDAB0B-4958-8E8F-E02D-A8BE0FA37A91";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "B5B1EB34-4B8C-7204-7DD8-36823D516CFF";
createNode follicle -n "pSphere2FollicleShape6534" -p "pSphere2Follicle6534";
	rename -uid "E40763B9-4D2E-E133-33C7-D69A52C07DD0";
	setAttr -k off ".v";
	setAttr ".pu" 0.65625;
	setAttr ".pv" 0.34375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve395" -p "pSphere2Follicle6534";
	rename -uid "718AC11A-499A-6736-C597-4CA475BEBF8C";
createNode nurbsCurve -n "curveShape395" -p "curve395";
	rename -uid "B92743FE-41CF-13AD-3C77-58A9A7E30C7E";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "3875CBC6-4264-6C88-2070-65875CF150F7";
createNode follicle -n "pSphere2FollicleShape6540" -p "pSphere2Follicle6540";
	rename -uid "71430CDF-4D60-A876-330E-9E8B8CA51DAE";
	setAttr -k off ".v";
	setAttr ".pu" 0.65625;
	setAttr ".pv" 0.40625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve396" -p "pSphere2Follicle6540";
	rename -uid "46BCCDAE-45A4-DB2B-73F9-0D855A15377E";
createNode nurbsCurve -n "curveShape396" -p "curve396";
	rename -uid "8215AA80-4CC4-6843-09B7-0AAA7CCB2A2D";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "0E2DAEA4-49DA-3BF0-8B9F-FEA8434EB4AE";
createNode follicle -n "pSphere2FollicleShape6546" -p "pSphere2Follicle6546";
	rename -uid "707B4940-4770-02CA-1E54-CBB9C32D6461";
	setAttr -k off ".v";
	setAttr ".pu" 0.65625;
	setAttr ".pv" 0.46875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve397" -p "pSphere2Follicle6546";
	rename -uid "7CDDAD6A-45B7-48BA-3B76-398D3AD7F6B9";
createNode nurbsCurve -n "curveShape397" -p "curve397";
	rename -uid "C69EAB2A-4A4C-F866-1BE6-05B6DA662241";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "DA016C6C-401B-9651-D439-E9A1CCAA1704";
createNode follicle -n "pSphere2FollicleShape6553" -p "pSphere2Follicle6553";
	rename -uid "D5FE4B43-488A-6F89-C44D-F595AC85D734";
	setAttr -k off ".v";
	setAttr ".pu" 0.65625;
	setAttr ".pv" 0.53125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve398" -p "pSphere2Follicle6553";
	rename -uid "BA17A695-4C71-547D-9898-A3A08713740F";
createNode nurbsCurve -n "curveShape398" -p "curve398";
	rename -uid "8B4A61B8-42F6-F5A7-A375-68AD020E6DC2";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "94A4E37C-4357-92F2-9E7D-34954A7F7D8A";
createNode follicle -n "pSphere2FollicleShape6559" -p "pSphere2Follicle6559";
	rename -uid "946BF06F-4257-4B06-90E0-34B3AF8E87C4";
	setAttr -k off ".v";
	setAttr ".pu" 0.65625;
	setAttr ".pv" 0.59375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve399" -p "pSphere2Follicle6559";
	rename -uid "08E090A0-4B34-18D6-D9FD-829B46A04E05";
createNode nurbsCurve -n "curveShape399" -p "curve399";
	rename -uid "93015A1F-473C-C671-5540-B4962A76F8D2";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "2FBE4D38-4F09-956D-0B44-83A345E7C4DE";
createNode follicle -n "pSphere2FollicleShape6565" -p "pSphere2Follicle6565";
	rename -uid "3BDC3CF7-4F93-2698-4630-C195E5D16E2B";
	setAttr -k off ".v";
	setAttr ".pu" 0.65625;
	setAttr ".pv" 0.65625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve400" -p "pSphere2Follicle6565";
	rename -uid "B238D666-438F-1B22-7D9B-F2AFAC4DD996";
createNode nurbsCurve -n "curveShape400" -p "curve400";
	rename -uid "B2513452-4271-B6B3-EEAB-FE8D282E3755";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "110E04A1-4545-C280-1E3E-23BD59E83AA7";
createNode follicle -n "pSphere2FollicleShape6571" -p "pSphere2Follicle6571";
	rename -uid "67FEF842-4F0D-B08D-C2F2-59A78E20D5B8";
	setAttr -k off ".v";
	setAttr ".pu" 0.65625;
	setAttr ".pv" 0.71875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve401" -p "pSphere2Follicle6571";
	rename -uid "C54DCC3C-464D-64FD-2810-2EBCE5623ED8";
createNode nurbsCurve -n "curveShape401" -p "curve401";
	rename -uid "0FBB4C96-4786-3859-D99D-8A95A1EE1AE9";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "454A3409-4343-354B-D48E-BC9D6E58F67D";
createNode follicle -n "pSphere2FollicleShape6577" -p "pSphere2Follicle6577";
	rename -uid "9CA80184-455E-05E6-D032-65B489721300";
	setAttr -k off ".v";
	setAttr ".pu" 0.65625;
	setAttr ".pv" 0.78125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve402" -p "pSphere2Follicle6577";
	rename -uid "99D2A0E4-4225-B6A5-9E1A-1DAB45ECF9E2";
createNode nurbsCurve -n "curveShape402" -p "curve402";
	rename -uid "15075893-4A2D-757A-9F98-26B5C0CE1367";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "325B8C5D-4AD8-9C2A-11D0-5F850032B602";
createNode follicle -n "pSphere2FollicleShape6584" -p "pSphere2Follicle6584";
	rename -uid "39592051-49AE-356C-EDA5-2E8C80743E79";
	setAttr -k off ".v";
	setAttr ".pu" 0.65625;
	setAttr ".pv" 0.84375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve403" -p "pSphere2Follicle6584";
	rename -uid "DAD3C59E-45C9-52D7-57DA-15B562174787";
createNode nurbsCurve -n "curveShape403" -p "curve403";
	rename -uid "0ACF8625-4425-DD87-A048-84884A3113EC";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "366C3623-45AE-3351-DC30-2B8D7566DCEE";
createNode follicle -n "pSphere2FollicleShape6590" -p "pSphere2Follicle6590";
	rename -uid "16A888B3-48DA-2AD9-5908-75A17F0E9BA2";
	setAttr -k off ".v";
	setAttr ".pu" 0.65625;
	setAttr ".pv" 0.90625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve404" -p "pSphere2Follicle6590";
	rename -uid "5238958D-4366-D8DF-41FB-8FABD564DAEF";
createNode nurbsCurve -n "curveShape404" -p "curve404";
	rename -uid "003837F2-4D88-A0DE-162F-B0947D94D995";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "1F98017F-4CEF-872E-1298-EE8BB6D21466";
createNode follicle -n "pSphere2FollicleShape7103" -p "pSphere2Follicle7103";
	rename -uid "D35F0227-4AC8-E98F-FD8F-2EB6AC3DF5D5";
	setAttr -k off ".v";
	setAttr ".pu" 0.71875;
	setAttr ".pv" 0.03125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve405" -p "pSphere2Follicle7103";
	rename -uid "735F918C-43E0-CE35-2023-B49FB2260540";
createNode nurbsCurve -n "curveShape405" -p "curve405";
	rename -uid "710098D9-49BB-E0C6-4DC3-1CA1F1725475";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "45E61B8D-471F-9743-F066-F58CCCB8A47A";
createNode follicle -n "pSphere2FollicleShape7109" -p "pSphere2Follicle7109";
	rename -uid "506A6434-4448-7E6C-61B0-AA87B83F2020";
	setAttr -k off ".v";
	setAttr ".pu" 0.71875;
	setAttr ".pv" 0.09375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve406" -p "pSphere2Follicle7109";
	rename -uid "945FCEC2-45BD-F4AB-E932-08BDC896D554";
createNode nurbsCurve -n "curveShape406" -p "curve406";
	rename -uid "958F0BDE-4C20-09EE-8427-31A78C1BEC63";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "501E2738-4582-9E6D-9EED-CAB83D3A105F";
createNode follicle -n "pSphere2FollicleShape7115" -p "pSphere2Follicle7115";
	rename -uid "B8C90826-4D72-8443-2414-D88AA20C7E23";
	setAttr -k off ".v";
	setAttr ".pu" 0.71875;
	setAttr ".pv" 0.15625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve407" -p "pSphere2Follicle7115";
	rename -uid "A520A165-42C2-8A46-55FA-CEAF935E244F";
createNode nurbsCurve -n "curveShape407" -p "curve407";
	rename -uid "5650EF1D-41DA-C43B-F679-C0B286D4EA52";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "98FB5BD0-4C6D-7CC2-753C-3A848FA63421";
createNode follicle -n "pSphere2FollicleShape7122" -p "pSphere2Follicle7122";
	rename -uid "FB875C90-4C19-FBB3-4F50-26AA07929488";
	setAttr -k off ".v";
	setAttr ".pu" 0.71875;
	setAttr ".pv" 0.21875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve408" -p "pSphere2Follicle7122";
	rename -uid "7640B7E6-408C-3C9F-0FCA-CBA0B06F67D2";
createNode nurbsCurve -n "curveShape408" -p "curve408";
	rename -uid "2C3C3175-4D0C-8B5F-FE88-77B2B42EEF3C";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "18E42F09-4E69-A000-4349-50A18B5B4B23";
createNode follicle -n "pSphere2FollicleShape7128" -p "pSphere2Follicle7128";
	rename -uid "C40FFC31-418D-C4E1-CBF7-D6A1B3698A08";
	setAttr -k off ".v";
	setAttr ".pu" 0.71875;
	setAttr ".pv" 0.28125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve409" -p "pSphere2Follicle7128";
	rename -uid "F7913599-4E38-378D-ECD6-87935460CF20";
createNode nurbsCurve -n "curveShape409" -p "curve409";
	rename -uid "A1120793-470E-9380-0C7A-2EAE2F64D62B";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "DF743313-4076-0921-D89D-BC91D6D5465C";
createNode follicle -n "pSphere2FollicleShape7134" -p "pSphere2Follicle7134";
	rename -uid "47248020-4082-E4B2-4DA3-16A7CCECA947";
	setAttr -k off ".v";
	setAttr ".pu" 0.71875;
	setAttr ".pv" 0.34375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve410" -p "pSphere2Follicle7134";
	rename -uid "AE3260D7-4263-9CD4-342C-DD9C8376A89D";
createNode nurbsCurve -n "curveShape410" -p "curve410";
	rename -uid "E80F21CE-40D5-7DDC-3D81-F6AB33E98618";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "0EE8E50F-4FB5-8C16-3208-2682FDB16854";
createNode follicle -n "pSphere2FollicleShape7140" -p "pSphere2Follicle7140";
	rename -uid "F9D3D19F-4E97-921D-DB52-E0A9F6FA8425";
	setAttr -k off ".v";
	setAttr ".pu" 0.71875;
	setAttr ".pv" 0.40625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve411" -p "pSphere2Follicle7140";
	rename -uid "AB3BDFCD-4552-1B31-E4CB-B592517EBF70";
createNode nurbsCurve -n "curveShape411" -p "curve411";
	rename -uid "022399E2-4EF5-2487-8D7A-0B9E1BCCB002";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "25479993-475F-E71F-2189-6D8D05894D64";
createNode follicle -n "pSphere2FollicleShape7146" -p "pSphere2Follicle7146";
	rename -uid "03F2FC88-4A79-2D01-2D65-949CDB6F4453";
	setAttr -k off ".v";
	setAttr ".pu" 0.71875;
	setAttr ".pv" 0.46875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve412" -p "pSphere2Follicle7146";
	rename -uid "29242F21-4B3F-8BC0-1ECA-46AF47756D9F";
createNode nurbsCurve -n "curveShape412" -p "curve412";
	rename -uid "FE466B4B-4A58-29D5-651B-F4B8B9E44952";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "B2A347F2-4775-6BF6-8124-4E9D1346E777";
createNode follicle -n "pSphere2FollicleShape7153" -p "pSphere2Follicle7153";
	rename -uid "46430192-4F4F-940E-1273-E88EED1975AC";
	setAttr -k off ".v";
	setAttr ".pu" 0.71875;
	setAttr ".pv" 0.53125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve413" -p "pSphere2Follicle7153";
	rename -uid "4DB96D63-4726-91BB-D98E-6FAAE6E55A5A";
createNode nurbsCurve -n "curveShape413" -p "curve413";
	rename -uid "A465BEF7-4942-21BE-1F47-7BB8BEE372EA";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "5BF89D79-479C-0A6D-67C8-5E9371B69F59";
createNode follicle -n "pSphere2FollicleShape7159" -p "pSphere2Follicle7159";
	rename -uid "1079A90C-4F41-7865-016A-6CAD78DE91C3";
	setAttr -k off ".v";
	setAttr ".pu" 0.71875;
	setAttr ".pv" 0.59375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve414" -p "pSphere2Follicle7159";
	rename -uid "99D9447D-4EE6-8139-CDB4-48BE5749E9EF";
createNode nurbsCurve -n "curveShape414" -p "curve414";
	rename -uid "7A42DDBA-4D80-7159-0558-FABF71CD23D9";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "5C4E3EDB-4A03-18C2-64F0-21BDD4A36079";
createNode follicle -n "pSphere2FollicleShape7165" -p "pSphere2Follicle7165";
	rename -uid "CA7F18B4-4F23-05E7-2CD9-899998E807B5";
	setAttr -k off ".v";
	setAttr ".pu" 0.71875;
	setAttr ".pv" 0.65625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve415" -p "pSphere2Follicle7165";
	rename -uid "51CEBE23-4988-41F4-DD88-92B6D292EB03";
createNode nurbsCurve -n "curveShape415" -p "curve415";
	rename -uid "C0AF802B-4561-53E8-7653-A686DFC46527";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "AC8708CF-4160-2108-E872-58887E8E1076";
createNode follicle -n "pSphere2FollicleShape7171" -p "pSphere2Follicle7171";
	rename -uid "DB01879E-43B3-E19D-5E54-B5A310E4AE77";
	setAttr -k off ".v";
	setAttr ".pu" 0.71875;
	setAttr ".pv" 0.71875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve416" -p "pSphere2Follicle7171";
	rename -uid "D76972F3-4308-D6E5-3461-5EB29B1B98C9";
createNode nurbsCurve -n "curveShape416" -p "curve416";
	rename -uid "E849A8E8-4C35-A199-D718-96BCCC589B77";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "A46B706E-4403-B41D-2552-9990C3367098";
createNode follicle -n "pSphere2FollicleShape7177" -p "pSphere2Follicle7177";
	rename -uid "74A47BA7-4081-4EED-78AB-669EF0C232FC";
	setAttr -k off ".v";
	setAttr ".pu" 0.71875;
	setAttr ".pv" 0.78125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve417" -p "pSphere2Follicle7177";
	rename -uid "C5E984C0-4322-7206-4558-F8B24946A119";
createNode nurbsCurve -n "curveShape417" -p "curve417";
	rename -uid "96A16C64-42B2-96B3-7F8E-08A779D397D9";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "7F86FB8C-41AE-911E-EF63-348DD78B9C9A";
createNode follicle -n "pSphere2FollicleShape7184" -p "pSphere2Follicle7184";
	rename -uid "B96D12BB-464F-F595-56E3-BD9715E4FEDB";
	setAttr -k off ".v";
	setAttr ".pu" 0.71875;
	setAttr ".pv" 0.84375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve418" -p "pSphere2Follicle7184";
	rename -uid "AC8093A5-45E0-392B-5A3C-8E97154B4EB6";
createNode nurbsCurve -n "curveShape418" -p "curve418";
	rename -uid "08ECE392-427A-9EFF-1178-7A92B3E7D7A1";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "3FCD4A66-4AAD-457E-EDE5-5B9F4F1EC9A7";
createNode follicle -n "pSphere2FollicleShape7190" -p "pSphere2Follicle7190";
	rename -uid "09973951-4DAC-4591-2437-BBB3097236AF";
	setAttr -k off ".v";
	setAttr ".pu" 0.71875;
	setAttr ".pv" 0.90625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve419" -p "pSphere2Follicle7190";
	rename -uid "89D18F0F-4539-38BD-2335-F9AED20D2F90";
createNode nurbsCurve -n "curveShape419" -p "curve419";
	rename -uid "0C0026EE-4B2E-2C30-1119-76BC0B5759FE";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "A2A50B01-48AE-9D83-146A-87A79C954808";
createNode follicle -n "pSphere2FollicleShape7196" -p "pSphere2Follicle7196";
	rename -uid "FEF900C8-4DA2-BFF2-36E7-6DA6166A591D";
	setAttr -k off ".v";
	setAttr ".pu" 0.71875;
	setAttr ".pv" 0.96875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve420" -p "pSphere2Follicle7196";
	rename -uid "D2637C64-4197-8BCC-6F9F-59998E605F79";
createNode nurbsCurve -n "curveShape420" -p "curve420";
	rename -uid "2A1A2E1B-47D5-8594-0223-68B3B0F4FFBC";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "39297199-4EA7-ADD3-EBA7-DA8044E07EE8";
createNode follicle -n "pSphere2FollicleShape7703" -p "pSphere2Follicle7703";
	rename -uid "2A4E5690-41A9-B7DB-2848-B8B1D7FEDB6E";
	setAttr -k off ".v";
	setAttr ".pu" 0.78125;
	setAttr ".pv" 0.03125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve421" -p "pSphere2Follicle7703";
	rename -uid "7F998ED1-4BD5-247C-8CE0-1CB40E0A2D95";
createNode nurbsCurve -n "curveShape421" -p "curve421";
	rename -uid "882F749B-42DB-5B13-F036-B8A11473BE61";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "C87D9D56-47F0-3A01-9C18-B4A8ECB546C0";
createNode follicle -n "pSphere2FollicleShape7709" -p "pSphere2Follicle7709";
	rename -uid "4D39F84E-4AA2-9F53-CAF0-48AFF454373D";
	setAttr -k off ".v";
	setAttr ".pu" 0.78125;
	setAttr ".pv" 0.09375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve422" -p "pSphere2Follicle7709";
	rename -uid "492D1338-4AFE-FDAA-1222-A5A7710770DE";
createNode nurbsCurve -n "curveShape422" -p "curve422";
	rename -uid "55446A4F-4E63-36D1-2B80-4FA7AD39B2F0";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "80868403-43E4-EC67-B50A-A0A6B04FCA87";
createNode follicle -n "pSphere2FollicleShape7715" -p "pSphere2Follicle7715";
	rename -uid "F9964DD5-436C-671A-0446-AFA19C37D86C";
	setAttr -k off ".v";
	setAttr ".pu" 0.78125;
	setAttr ".pv" 0.15625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve423" -p "pSphere2Follicle7715";
	rename -uid "87BC6F1F-4796-AB69-10A4-4E92CADEA894";
createNode nurbsCurve -n "curveShape423" -p "curve423";
	rename -uid "A1DA831E-4884-2F08-6B73-ECBA476B8C2F";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "8643F5A3-4417-F5DC-D6CE-3086A0FC8DBE";
createNode follicle -n "pSphere2FollicleShape7722" -p "pSphere2Follicle7722";
	rename -uid "60219000-433F-1AEE-D19D-558D9477CAE9";
	setAttr -k off ".v";
	setAttr ".pu" 0.78125;
	setAttr ".pv" 0.21875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve424" -p "pSphere2Follicle7722";
	rename -uid "2630F0BB-41CF-05DD-20A4-C598A6F9F730";
createNode nurbsCurve -n "curveShape424" -p "curve424";
	rename -uid "4733039D-469B-6C21-AD35-32B030B6D619";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "72176556-4326-D9C5-8799-20AAB00C7DCD";
createNode follicle -n "pSphere2FollicleShape7728" -p "pSphere2Follicle7728";
	rename -uid "DD7F4A9F-4BF3-36F0-30E3-889BCB84B1F3";
	setAttr -k off ".v";
	setAttr ".pu" 0.78125;
	setAttr ".pv" 0.28125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve425" -p "pSphere2Follicle7728";
	rename -uid "1A977503-44D4-8CC6-8F47-F09E4E982FF6";
createNode nurbsCurve -n "curveShape425" -p "curve425";
	rename -uid "DEFD724A-4072-BCD5-C45F-AD9B6DA0FD06";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "1551CBFC-476C-304C-D876-D2967E136F9C";
createNode follicle -n "pSphere2FollicleShape7734" -p "pSphere2Follicle7734";
	rename -uid "C6640824-480D-B2F2-C711-B48AB7D1ED1D";
	setAttr -k off ".v";
	setAttr ".pu" 0.78125;
	setAttr ".pv" 0.34375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve426" -p "pSphere2Follicle7734";
	rename -uid "ADF1C569-45C9-0AB7-8150-2AACD4C490B2";
createNode nurbsCurve -n "curveShape426" -p "curve426";
	rename -uid "AE84C605-4A40-E017-0B27-C68E2AD0554B";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "BA4175D0-4915-0801-89F3-AA9C0063B4A1";
createNode follicle -n "pSphere2FollicleShape7740" -p "pSphere2Follicle7740";
	rename -uid "28614AC4-45A3-F650-F465-88AC21C2BCE0";
	setAttr -k off ".v";
	setAttr ".pu" 0.78125;
	setAttr ".pv" 0.40625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve427" -p "pSphere2Follicle7740";
	rename -uid "C43438F9-477E-1A19-8D1F-8DA9497C349F";
createNode nurbsCurve -n "curveShape427" -p "curve427";
	rename -uid "4CE83E23-4821-F17A-6F7F-94B6DF34692B";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "B993D1F8-4FC3-308E-0F00-8BB99368A725";
createNode follicle -n "pSphere2FollicleShape7746" -p "pSphere2Follicle7746";
	rename -uid "2D377DE6-4515-492A-F81B-60807132F49B";
	setAttr -k off ".v";
	setAttr ".pu" 0.78125;
	setAttr ".pv" 0.46875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve428" -p "pSphere2Follicle7746";
	rename -uid "A81B278A-4E59-B01F-16E3-77A4D9849A29";
createNode nurbsCurve -n "curveShape428" -p "curve428";
	rename -uid "2A49F324-48A8-4B0A-3C33-5BAD6302B7C0";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "AF3159F3-4BFC-1D65-007B-71851D1A2D6D";
createNode follicle -n "pSphere2FollicleShape7753" -p "pSphere2Follicle7753";
	rename -uid "8F649845-4717-6EB7-3AE7-0F95C4E26E85";
	setAttr -k off ".v";
	setAttr ".pu" 0.78125;
	setAttr ".pv" 0.53125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve429" -p "pSphere2Follicle7753";
	rename -uid "EB5D9372-4AAD-49D9-79AA-E39F17FBDAD7";
createNode nurbsCurve -n "curveShape429" -p "curve429";
	rename -uid "2FF27527-4F78-FB7B-7F57-E6AF512FF326";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "B01AFE43-46CA-CDA7-9B6B-26A0C40F885C";
createNode follicle -n "pSphere2FollicleShape7759" -p "pSphere2Follicle7759";
	rename -uid "B8E74FE8-40AA-BA7C-88F8-9F9EB9F58854";
	setAttr -k off ".v";
	setAttr ".pu" 0.78125;
	setAttr ".pv" 0.59375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve430" -p "pSphere2Follicle7759";
	rename -uid "48152E6E-488B-87F4-E493-0B8B265D85AD";
createNode nurbsCurve -n "curveShape430" -p "curve430";
	rename -uid "6FAEEB5F-4C8E-FD4D-DFE5-0CB6CB0B5A1C";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "99F8421A-4207-9844-9CD9-1B855AD5F603";
createNode follicle -n "pSphere2FollicleShape7765" -p "pSphere2Follicle7765";
	rename -uid "7E75AE55-4CB1-96F7-D94F-ED98DA25F073";
	setAttr -k off ".v";
	setAttr ".pu" 0.78125;
	setAttr ".pv" 0.65625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve431" -p "pSphere2Follicle7765";
	rename -uid "48672BBE-40C2-8A36-8E96-6C9EF510E987";
createNode nurbsCurve -n "curveShape431" -p "curve431";
	rename -uid "FD7D1E2D-4A16-BA89-50FE-0DB386114851";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "67BC3504-4C67-EF4C-5FAE-8284051647FD";
createNode follicle -n "pSphere2FollicleShape7771" -p "pSphere2Follicle7771";
	rename -uid "385B529D-4E8A-CC83-FE7D-E8962E31538F";
	setAttr -k off ".v";
	setAttr ".pu" 0.78125;
	setAttr ".pv" 0.71875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve432" -p "pSphere2Follicle7771";
	rename -uid "EC21F07E-492A-4B59-3EF7-1392CC68E294";
createNode nurbsCurve -n "curveShape432" -p "curve432";
	rename -uid "0715E4FE-4EC9-B1E1-D695-9FB885F9023A";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "6C92ABFE-4C60-0567-10A0-E3980C8A12CF";
createNode follicle -n "pSphere2FollicleShape7777" -p "pSphere2Follicle7777";
	rename -uid "5CE49A31-4271-F7CD-DBFA-2DAFDE41CD41";
	setAttr -k off ".v";
	setAttr ".pu" 0.78125;
	setAttr ".pv" 0.78125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve433" -p "pSphere2Follicle7777";
	rename -uid "BB065D4A-4900-470F-EB96-F9B9B93742AF";
createNode nurbsCurve -n "curveShape433" -p "curve433";
	rename -uid "3C411D94-4AE6-7193-6EBB-8FBA9DA465A6";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "D7CAEB50-4074-5969-FE70-4FADFAB83A33";
createNode follicle -n "pSphere2FollicleShape7784" -p "pSphere2Follicle7784";
	rename -uid "F3BA5CBC-4B1B-43F5-330D-5E9D3F46599B";
	setAttr -k off ".v";
	setAttr ".pu" 0.78125;
	setAttr ".pv" 0.84375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve434" -p "pSphere2Follicle7784";
	rename -uid "60A8F6B2-45E3-56C8-407F-7C8332749E76";
createNode nurbsCurve -n "curveShape434" -p "curve434";
	rename -uid "BA4C55F8-40C0-197C-F4B8-559E44D0D7AB";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "1723680B-49DF-5026-D25E-B7BE87FB46C0";
createNode follicle -n "pSphere2FollicleShape7790" -p "pSphere2Follicle7790";
	rename -uid "A5A60738-4321-2CC0-244D-1A892E6B0380";
	setAttr -k off ".v";
	setAttr ".pu" 0.78125;
	setAttr ".pv" 0.90625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve435" -p "pSphere2Follicle7790";
	rename -uid "DCCE747E-4E7C-BBE6-18CE-6BA499BE7F6A";
createNode nurbsCurve -n "curveShape435" -p "curve435";
	rename -uid "5353B138-437D-792B-9B74-8CAE64FB673B";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "3F77A8EA-48E3-510E-64EB-11894C8253A8";
createNode follicle -n "pSphere2FollicleShape7796" -p "pSphere2Follicle7796";
	rename -uid "7CBE52D2-4401-EF95-83A8-50B0743345DD";
	setAttr -k off ".v";
	setAttr ".pu" 0.78125;
	setAttr ".pv" 0.96875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve436" -p "pSphere2Follicle7796";
	rename -uid "DF0B4CEC-4E04-AA7C-3250-B092F1ADCED0";
createNode nurbsCurve -n "curveShape436" -p "curve436";
	rename -uid "06DABDE7-4245-19EE-F203-5D84F51154A1";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "86B9217A-42E3-169B-798F-C49E77BA01D8";
createNode follicle -n "pSphere2FollicleShape8409" -p "pSphere2Follicle8409";
	rename -uid "D0D61AB0-4834-68F9-E19A-55BDEECADA30";
	setAttr -k off ".v";
	setAttr ".pu" 0.84375;
	setAttr ".pv" 0.09375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve437" -p "pSphere2Follicle8409";
	rename -uid "67EBA913-4DB4-3B55-5B3F-8E95BCCADCDF";
createNode nurbsCurve -n "curveShape437" -p "curve437";
	rename -uid "97A12C8E-4CBC-ED41-5AFE-28ACE9221BAA";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "E2CF3630-4766-A030-44A0-7C8AA1C27C79";
createNode follicle -n "pSphere2FollicleShape8415" -p "pSphere2Follicle8415";
	rename -uid "AEF12971-40F4-044E-6AD6-8EAF6350E7DE";
	setAttr -k off ".v";
	setAttr ".pu" 0.84375;
	setAttr ".pv" 0.15625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve438" -p "pSphere2Follicle8415";
	rename -uid "A7D327F3-4550-9E1F-15DC-9CBE6608A3E4";
createNode nurbsCurve -n "curveShape438" -p "curve438";
	rename -uid "DF646C89-4081-95E9-1A99-F2AE17A6ECF9";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "B4FAD063-436B-4C37-514D-BEBB988A162E";
createNode follicle -n "pSphere2FollicleShape8422" -p "pSphere2Follicle8422";
	rename -uid "194F8C46-4256-A4DB-0966-7EA548EEB8A7";
	setAttr -k off ".v";
	setAttr ".pu" 0.84375;
	setAttr ".pv" 0.21875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve439" -p "pSphere2Follicle8422";
	rename -uid "9AAAD843-4DB5-94F2-1C4C-21A3DF07A094";
createNode nurbsCurve -n "curveShape439" -p "curve439";
	rename -uid "97868511-4792-B044-F62E-BFA91813102F";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "B12303A9-4484-F831-CA57-F6B5E6F761C1";
createNode follicle -n "pSphere2FollicleShape8428" -p "pSphere2Follicle8428";
	rename -uid "5C3754F5-4D54-C3CF-2233-1A9DBE1C8999";
	setAttr -k off ".v";
	setAttr ".pu" 0.84375;
	setAttr ".pv" 0.28125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve440" -p "pSphere2Follicle8428";
	rename -uid "45B26537-4A74-088E-2FB5-35AA5E51EEDD";
createNode nurbsCurve -n "curveShape440" -p "curve440";
	rename -uid "376757CD-4616-F07B-9B36-C3B8405F76B5";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "7C768733-468A-1160-531D-FCA74D4616CE";
createNode follicle -n "pSphere2FollicleShape8434" -p "pSphere2Follicle8434";
	rename -uid "70E23B04-4A68-0F9B-F15F-75BE1C773E47";
	setAttr -k off ".v";
	setAttr ".pu" 0.84375;
	setAttr ".pv" 0.34375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve441" -p "pSphere2Follicle8434";
	rename -uid "4D29F97F-4919-F887-141A-3C9A65B75424";
createNode nurbsCurve -n "curveShape441" -p "curve441";
	rename -uid "15310BAD-4A9E-4415-7528-06954D539735";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "86A3F8DA-456F-796C-44A0-DCB2F80836F1";
createNode follicle -n "pSphere2FollicleShape8440" -p "pSphere2Follicle8440";
	rename -uid "4F0A8669-4E67-0908-7B5A-48B2E4AE1DC7";
	setAttr -k off ".v";
	setAttr ".pu" 0.84375;
	setAttr ".pv" 0.40625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve442" -p "pSphere2Follicle8440";
	rename -uid "81610DE2-4939-7F22-A70D-14B9DCEECDEA";
createNode nurbsCurve -n "curveShape442" -p "curve442";
	rename -uid "7D87C4F1-45AB-277A-62B1-9EAAC060FDE7";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "CCF20BFC-4C1A-1B05-5DE0-789786BF6E1F";
createNode follicle -n "pSphere2FollicleShape8446" -p "pSphere2Follicle8446";
	rename -uid "155AB776-4AD4-4464-2616-C882DD7CF51F";
	setAttr -k off ".v";
	setAttr ".pu" 0.84375;
	setAttr ".pv" 0.46875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve443" -p "pSphere2Follicle8446";
	rename -uid "9F5FDC65-484F-1F4C-B3B5-1F899A2BE741";
createNode nurbsCurve -n "curveShape443" -p "curve443";
	rename -uid "CE15DE75-40DF-6C1F-D075-7296D0CA9499";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "D0804B4C-4084-3240-9355-198BB69B1AAB";
createNode follicle -n "pSphere2FollicleShape8453" -p "pSphere2Follicle8453";
	rename -uid "4067B3E8-4114-250C-52E4-7DB241C15BD0";
	setAttr -k off ".v";
	setAttr ".pu" 0.84375;
	setAttr ".pv" 0.53125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve444" -p "pSphere2Follicle8453";
	rename -uid "87740DAC-453B-0019-A161-05BC1FA6FDCB";
createNode nurbsCurve -n "curveShape444" -p "curve444";
	rename -uid "3E9E2D27-45FE-15F9-8D1D-E69F7407666D";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "AEC81A52-45FB-0A31-E57B-8BB74A48FA46";
createNode follicle -n "pSphere2FollicleShape8459" -p "pSphere2Follicle8459";
	rename -uid "28BC0310-4CE9-6FDC-95D9-95901340710A";
	setAttr -k off ".v";
	setAttr ".pu" 0.84375;
	setAttr ".pv" 0.59375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve445" -p "pSphere2Follicle8459";
	rename -uid "EB7E90CF-40A9-8AB8-7A17-C49514C1F093";
createNode nurbsCurve -n "curveShape445" -p "curve445";
	rename -uid "7C05B078-413F-F840-EC37-7A829E713DD6";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "697F56AB-4355-6502-FE89-E180396F99CB";
createNode follicle -n "pSphere2FollicleShape8465" -p "pSphere2Follicle8465";
	rename -uid "6A08AC10-4B52-EA28-7F0C-0099B5BAED1C";
	setAttr -k off ".v";
	setAttr ".pu" 0.84375;
	setAttr ".pv" 0.65625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve446" -p "pSphere2Follicle8465";
	rename -uid "7E73A705-432D-2E1F-D154-E8B700FF9C46";
createNode nurbsCurve -n "curveShape446" -p "curve446";
	rename -uid "12F0AA67-4EC2-38CF-42C8-2EA4E2487C93";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "2E509F9E-4776-ADEC-DCD2-5AB1B0801B10";
createNode follicle -n "pSphere2FollicleShape8471" -p "pSphere2Follicle8471";
	rename -uid "84387395-4B0E-D63C-D762-60B8A12EEC85";
	setAttr -k off ".v";
	setAttr ".pu" 0.84375;
	setAttr ".pv" 0.71875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve447" -p "pSphere2Follicle8471";
	rename -uid "64A2DEA9-4DAB-ADB9-5C04-6FB6AD72029D";
createNode nurbsCurve -n "curveShape447" -p "curve447";
	rename -uid "A5F78271-4621-200D-3473-B2B816859AA0";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "A99E0050-4CB8-7052-299C-EA8ADCE2FE90";
createNode follicle -n "pSphere2FollicleShape8477" -p "pSphere2Follicle8477";
	rename -uid "92F3C97C-4CA8-BE72-86CA-13B3185A35D5";
	setAttr -k off ".v";
	setAttr ".pu" 0.84375;
	setAttr ".pv" 0.78125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve448" -p "pSphere2Follicle8477";
	rename -uid "12F3407D-4796-2DC4-D39E-60B6A3F3D6E5";
createNode nurbsCurve -n "curveShape448" -p "curve448";
	rename -uid "5C4783A7-4AF3-ED23-F04D-54AEC3B24AFB";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "8B978254-4038-46BF-51EB-8AAE9A17DD8D";
createNode follicle -n "pSphere2FollicleShape8484" -p "pSphere2Follicle8484";
	rename -uid "091B66EF-4B9A-7FC4-E1FE-D8B0690B03D4";
	setAttr -k off ".v";
	setAttr ".pu" 0.84375;
	setAttr ".pv" 0.84375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve449" -p "pSphere2Follicle8484";
	rename -uid "D7A5E0FD-4EA1-936D-0EFF-41ADD0E9478F";
createNode nurbsCurve -n "curveShape449" -p "curve449";
	rename -uid "7A9426F6-473B-F610-C386-1585EC6DD224";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "A75B8A2A-4BE6-A8A6-BEAB-F5853D9305CF";
createNode follicle -n "pSphere2FollicleShape8490" -p "pSphere2Follicle8490";
	rename -uid "1BF5FDDC-4B90-A823-7767-7DB84CA97997";
	setAttr -k off ".v";
	setAttr ".pu" 0.84375;
	setAttr ".pv" 0.90625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve450" -p "pSphere2Follicle8490";
	rename -uid "D2B22C9C-484F-79BA-14D0-77B9051407CF";
createNode nurbsCurve -n "curveShape450" -p "curve450";
	rename -uid "CDD62148-4067-D07B-7D53-638701DBFA4F";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "F69EB1F2-4C05-575B-1328-4E876C6A4000";
createNode follicle -n "pSphere2FollicleShape9009" -p "pSphere2Follicle9009";
	rename -uid "3F62095E-462F-C698-2639-69BBE16F380A";
	setAttr -k off ".v";
	setAttr ".pu" 0.90625;
	setAttr ".pv" 0.09375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve451" -p "pSphere2Follicle9009";
	rename -uid "EED1F68F-440E-1D6F-A008-E6A2DFD53E68";
createNode nurbsCurve -n "curveShape451" -p "curve451";
	rename -uid "00926384-4777-4F20-CB76-23B3DF8631EF";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "4EC85768-42F4-77D8-4056-03970F5F6F09";
createNode follicle -n "pSphere2FollicleShape9015" -p "pSphere2Follicle9015";
	rename -uid "1505C687-4D27-D20E-BC5F-9BBD2458B454";
	setAttr -k off ".v";
	setAttr ".pu" 0.90625;
	setAttr ".pv" 0.15625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve452" -p "pSphere2Follicle9015";
	rename -uid "571AA4E6-4A28-6FCB-3ED0-C1B39B0CC852";
createNode nurbsCurve -n "curveShape452" -p "curve452";
	rename -uid "5DA60614-425D-B31E-C244-178C5002674A";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "4EA98B21-4F00-1F86-E878-2C8027A76196";
createNode follicle -n "pSphere2FollicleShape9022" -p "pSphere2Follicle9022";
	rename -uid "27BF0421-4671-1F9C-5708-E8B0DD8AB783";
	setAttr -k off ".v";
	setAttr ".pu" 0.90625;
	setAttr ".pv" 0.21875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve453" -p "pSphere2Follicle9022";
	rename -uid "29A141A8-4162-D479-78DB-9A95A3A29E31";
createNode nurbsCurve -n "curveShape453" -p "curve453";
	rename -uid "DE75D692-41AF-D37D-7AC1-378DBA1E39B2";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "31A21F1A-4F68-BC7D-B1F0-CD9F5E279051";
createNode follicle -n "pSphere2FollicleShape9028" -p "pSphere2Follicle9028";
	rename -uid "7B6C6ED3-4664-FA6A-1102-DD929176F177";
	setAttr -k off ".v";
	setAttr ".pu" 0.90625;
	setAttr ".pv" 0.28125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve454" -p "pSphere2Follicle9028";
	rename -uid "5F342938-433B-C0B1-6280-7F8DF0B08F0B";
createNode nurbsCurve -n "curveShape454" -p "curve454";
	rename -uid "F0518633-4E13-3F1B-8DB8-838F0AE310CC";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "0D0ED5DB-4BEC-3C01-55ED-F39F37985886";
createNode follicle -n "pSphere2FollicleShape9034" -p "pSphere2Follicle9034";
	rename -uid "3FF8FC39-4230-6C61-14CD-37BC66CD7088";
	setAttr -k off ".v";
	setAttr ".pu" 0.90625;
	setAttr ".pv" 0.34375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve455" -p "pSphere2Follicle9034";
	rename -uid "2FB358CD-4D34-34F1-95DA-B58DED418B5E";
createNode nurbsCurve -n "curveShape455" -p "curve455";
	rename -uid "C760ED27-45A0-0738-36F5-4B99BFEE461D";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "6428490B-4893-C03F-A134-158381C4A65C";
createNode follicle -n "pSphere2FollicleShape9040" -p "pSphere2Follicle9040";
	rename -uid "714BF71D-4F2B-8EA7-C265-7182191C3A17";
	setAttr -k off ".v";
	setAttr ".pu" 0.90625;
	setAttr ".pv" 0.40625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve456" -p "pSphere2Follicle9040";
	rename -uid "B0BACA1E-4DA2-6430-AA14-D39984C938BC";
createNode nurbsCurve -n "curveShape456" -p "curve456";
	rename -uid "FFDC9045-44C2-EA10-39EB-47B39F53B1BB";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "DAC5DF31-4829-B5E3-9C7C-1ABFA23C0194";
createNode follicle -n "pSphere2FollicleShape9046" -p "pSphere2Follicle9046";
	rename -uid "B3F5EE84-436E-B0D1-AE85-04859C60DF12";
	setAttr -k off ".v";
	setAttr ".pu" 0.90625;
	setAttr ".pv" 0.46875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve457" -p "pSphere2Follicle9046";
	rename -uid "B411BF7F-4DED-03A9-3901-22868EC77E7C";
createNode nurbsCurve -n "curveShape457" -p "curve457";
	rename -uid "3623B0E6-420C-314B-80F5-62B6863E28CE";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "9BA5AA69-457C-DFD4-711D-68A19D77C9E6";
createNode follicle -n "pSphere2FollicleShape9053" -p "pSphere2Follicle9053";
	rename -uid "6B499092-4737-14E2-1EED-A3AE957BE568";
	setAttr -k off ".v";
	setAttr ".pu" 0.90625;
	setAttr ".pv" 0.53125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve458" -p "pSphere2Follicle9053";
	rename -uid "8B8B36C4-4184-46B6-D59A-55A3CF2D4B89";
createNode nurbsCurve -n "curveShape458" -p "curve458";
	rename -uid "6FD6894A-45A8-3AC2-116C-4EA9F5DBCEA2";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "28E9F121-4ED3-6DBD-4B81-1CB5491D19A0";
createNode follicle -n "pSphere2FollicleShape9059" -p "pSphere2Follicle9059";
	rename -uid "89C9CFCB-4618-AB7C-0F79-AC9A41C524CE";
	setAttr -k off ".v";
	setAttr ".pu" 0.90625;
	setAttr ".pv" 0.59375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve459" -p "pSphere2Follicle9059";
	rename -uid "667D1E19-4B9B-8E8E-2834-A0810CEF20C9";
createNode nurbsCurve -n "curveShape459" -p "curve459";
	rename -uid "29E51C95-4E05-EF4A-0DDA-159C9C6EE347";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "DD051A4F-4C89-DB3E-93C2-D08C8739F7C5";
createNode follicle -n "pSphere2FollicleShape9065" -p "pSphere2Follicle9065";
	rename -uid "29FE8DA5-413D-F3A0-3BA8-1A9BB6562D8C";
	setAttr -k off ".v";
	setAttr ".pu" 0.90625;
	setAttr ".pv" 0.65625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve460" -p "pSphere2Follicle9065";
	rename -uid "901C15E6-43C3-7927-1CEA-8DB0CB0C955E";
createNode nurbsCurve -n "curveShape460" -p "curve460";
	rename -uid "1B150CEC-4EC0-BE86-1BC0-A98352E168B8";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "C74974EC-4B65-DC26-E284-38A5E1CBEF80";
createNode follicle -n "pSphere2FollicleShape9071" -p "pSphere2Follicle9071";
	rename -uid "C8836730-4314-ACA5-8C6B-B1B87B1CE242";
	setAttr -k off ".v";
	setAttr ".pu" 0.90625;
	setAttr ".pv" 0.71875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve461" -p "pSphere2Follicle9071";
	rename -uid "95E8950C-4F69-F782-7E35-00A659395B67";
createNode nurbsCurve -n "curveShape461" -p "curve461";
	rename -uid "64346762-4314-7049-747C-27A1964CD280";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "D211C621-41F4-9EE7-757A-1F89961E8FAD";
createNode follicle -n "pSphere2FollicleShape9077" -p "pSphere2Follicle9077";
	rename -uid "6A3DA6C9-4ADC-12E5-2D2D-97AA09ADC6B1";
	setAttr -k off ".v";
	setAttr ".pu" 0.90625;
	setAttr ".pv" 0.78125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve462" -p "pSphere2Follicle9077";
	rename -uid "4B766109-4543-89CA-9B18-2DB536E6B543";
createNode nurbsCurve -n "curveShape462" -p "curve462";
	rename -uid "899DB425-41F4-A0C4-404C-C887DD8D0C29";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "ADA75ED4-4D17-EBF8-6211-81971BFD9F10";
createNode follicle -n "pSphere2FollicleShape9084" -p "pSphere2Follicle9084";
	rename -uid "C7527C12-415F-4AFB-3A1E-569DD7E5059B";
	setAttr -k off ".v";
	setAttr ".pu" 0.90625;
	setAttr ".pv" 0.84375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve463" -p "pSphere2Follicle9084";
	rename -uid "057AFA72-4F76-8104-0B45-E3868C678002";
createNode nurbsCurve -n "curveShape463" -p "curve463";
	rename -uid "D8EE0097-4291-21CF-4B10-ECBE4D50B91E";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "9EB7C787-4A95-4BAB-3D51-6091525152FD";
createNode follicle -n "pSphere2FollicleShape9090" -p "pSphere2Follicle9090";
	rename -uid "7F3FA8CE-4630-96D8-70ED-9B9A648AF06B";
	setAttr -k off ".v";
	setAttr ".pu" 0.90625;
	setAttr ".pv" 0.90625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve464" -p "pSphere2Follicle9090";
	rename -uid "CE011574-4023-0D93-48B1-8D8D43456FDA";
createNode nurbsCurve -n "curveShape464" -p "curve464";
	rename -uid "131FD82A-4830-54B1-E012-7AB98BF54C82";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "DA54AAC3-4067-CB83-2F0A-2C8D3C5CA6DB";
createNode follicle -n "pSphere2FollicleShape9603" -p "pSphere2Follicle9603";
	rename -uid "A7DFFAF5-4217-83FE-EE67-F5AF4A3FA3C5";
	setAttr -k off ".v";
	setAttr ".pu" 0.96875;
	setAttr ".pv" 0.03125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve465" -p "pSphere2Follicle9603";
	rename -uid "D4A63805-41BE-6F8D-0CDC-2A874B988EF0";
createNode nurbsCurve -n "curveShape465" -p "curve465";
	rename -uid "E3BCF176-4E8C-D20D-9355-1683F653E960";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "4381D40C-4D83-2AE1-7050-27BD2D8005E1";
createNode follicle -n "pSphere2FollicleShape9609" -p "pSphere2Follicle9609";
	rename -uid "8AC6F168-4B81-DC81-AE44-3FAE65564C78";
	setAttr -k off ".v";
	setAttr ".pu" 0.96875;
	setAttr ".pv" 0.09375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve466" -p "pSphere2Follicle9609";
	rename -uid "B53D355F-458D-93E6-F7EF-DC8C6E6D5233";
createNode nurbsCurve -n "curveShape466" -p "curve466";
	rename -uid "E68324D4-4432-0134-E726-E285E86E5121";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "0ECB2CFF-482E-8A3F-C83E-B2A9725FED2E";
createNode follicle -n "pSphere2FollicleShape9615" -p "pSphere2Follicle9615";
	rename -uid "D54F5CE4-4462-6595-C4F2-A8B911D9297A";
	setAttr -k off ".v";
	setAttr ".pu" 0.96875;
	setAttr ".pv" 0.15625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve467" -p "pSphere2Follicle9615";
	rename -uid "2A08D602-4577-2560-DE17-5C9004761FC8";
createNode nurbsCurve -n "curveShape467" -p "curve467";
	rename -uid "509EB85C-4149-5D3E-A88B-B0892F46CF98";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "5B946754-4782-6826-06BB-D394AA0A9E0A";
createNode follicle -n "pSphere2FollicleShape9622" -p "pSphere2Follicle9622";
	rename -uid "D80272F7-4F04-A3AF-0DE5-BAAFAAF6A65B";
	setAttr -k off ".v";
	setAttr ".pu" 0.96875;
	setAttr ".pv" 0.21875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve468" -p "pSphere2Follicle9622";
	rename -uid "A638D0CF-4028-98A9-818A-D4A267CD5DB6";
createNode nurbsCurve -n "curveShape468" -p "curve468";
	rename -uid "DCC94117-4F7C-4EF4-804E-0E9E918BEE96";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "45D6EC84-489E-2BC7-5A9B-6E841CF755B7";
createNode follicle -n "pSphere2FollicleShape9628" -p "pSphere2Follicle9628";
	rename -uid "BD033677-4115-A108-D17B-F6BEE9DADFD9";
	setAttr -k off ".v";
	setAttr ".pu" 0.96875;
	setAttr ".pv" 0.28125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve469" -p "pSphere2Follicle9628";
	rename -uid "02BBA6B4-496C-47A1-CF70-309784C2E2C7";
createNode nurbsCurve -n "curveShape469" -p "curve469";
	rename -uid "F7F4446B-4A9C-9549-2A07-3484355A645A";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "0208A3FF-4487-D2A8-D5F7-019BC638576B";
createNode follicle -n "pSphere2FollicleShape9634" -p "pSphere2Follicle9634";
	rename -uid "053EF5FA-4B40-B0DD-BE81-8F945AF367E4";
	setAttr -k off ".v";
	setAttr ".pu" 0.96875;
	setAttr ".pv" 0.34375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve470" -p "pSphere2Follicle9634";
	rename -uid "4B52680A-48D8-0959-4D83-8E853E9CD2B8";
createNode nurbsCurve -n "curveShape470" -p "curve470";
	rename -uid "0EC21AAE-4B13-D80F-DC00-21B543EE48F0";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "FD69B319-478C-76AE-EC71-37B99DC5B11B";
createNode follicle -n "pSphere2FollicleShape9640" -p "pSphere2Follicle9640";
	rename -uid "ABFD2596-4EB7-5BE2-351A-9981E45DCFF5";
	setAttr -k off ".v";
	setAttr ".pu" 0.96875;
	setAttr ".pv" 0.40625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve471" -p "pSphere2Follicle9640";
	rename -uid "AF4BC7A6-407C-4E6F-6537-11B2C0AF3B16";
createNode nurbsCurve -n "curveShape471" -p "curve471";
	rename -uid "7625C00B-4B92-4F31-3BD0-6FBDB1CF3791";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "74FBC2FA-4C99-EF1B-A91F-50AD62F91AC9";
createNode follicle -n "pSphere2FollicleShape9646" -p "pSphere2Follicle9646";
	rename -uid "28D2156A-4B77-A480-97AC-F0BF1B000B89";
	setAttr -k off ".v";
	setAttr ".pu" 0.96875;
	setAttr ".pv" 0.46875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve472" -p "pSphere2Follicle9646";
	rename -uid "FFC76516-46D3-B55E-5425-DEBE7989FFD2";
createNode nurbsCurve -n "curveShape472" -p "curve472";
	rename -uid "3D99DEAE-4DA5-EAB0-8E02-D7A2646B8FD3";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "D5A4CF9E-491B-B06E-B0AA-C7BCDBD2FF22";
createNode follicle -n "pSphere2FollicleShape9653" -p "pSphere2Follicle9653";
	rename -uid "DB379AD7-4BBB-766E-B824-EFAB83C60869";
	setAttr -k off ".v";
	setAttr ".pu" 0.96875;
	setAttr ".pv" 0.53125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve473" -p "pSphere2Follicle9653";
	rename -uid "91E9ED50-4D40-378E-FD11-FAAFBEFDEC92";
createNode nurbsCurve -n "curveShape473" -p "curve473";
	rename -uid "4A303935-42AE-9F27-1C0C-99BEBF36983E";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "6A4E87B3-4BBA-935F-EC09-C8902633A22A";
createNode follicle -n "pSphere2FollicleShape9659" -p "pSphere2Follicle9659";
	rename -uid "03846B96-4654-9C51-E8E0-6CAC8C149CDF";
	setAttr -k off ".v";
	setAttr ".pu" 0.96875;
	setAttr ".pv" 0.59375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve474" -p "pSphere2Follicle9659";
	rename -uid "9A46957D-4197-3504-D2DC-A2AC21507E69";
createNode nurbsCurve -n "curveShape474" -p "curve474";
	rename -uid "BA78F65E-48DA-FB72-7DA1-87A176252C3D";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "C7120F80-4AB4-FD31-0094-BC80FD737975";
createNode follicle -n "pSphere2FollicleShape9665" -p "pSphere2Follicle9665";
	rename -uid "D4A7DCAE-4303-D4AC-FA96-4A8A2E70C4E2";
	setAttr -k off ".v";
	setAttr ".pu" 0.96875;
	setAttr ".pv" 0.65625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve475" -p "pSphere2Follicle9665";
	rename -uid "0591E2FD-4FFB-13EA-68CF-F9A3E72F1F53";
createNode nurbsCurve -n "curveShape475" -p "curve475";
	rename -uid "D3ABE39E-4A41-8FF3-BE59-A58C7C5FC91E";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "3685DDD5-4484-D2E9-2597-BCB52E709EDA";
createNode follicle -n "pSphere2FollicleShape9671" -p "pSphere2Follicle9671";
	rename -uid "7909C04D-403A-937D-D4E8-24B1DCFAC926";
	setAttr -k off ".v";
	setAttr ".pu" 0.96875;
	setAttr ".pv" 0.71875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve476" -p "pSphere2Follicle9671";
	rename -uid "FF859008-4BFB-8FD7-BBAB-678505770AB3";
createNode nurbsCurve -n "curveShape476" -p "curve476";
	rename -uid "58316342-4B45-CB13-B8C4-63B5E4A4AC3C";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "16A7E2C5-460B-6A5D-CDDB-11982333034A";
createNode follicle -n "pSphere2FollicleShape9677" -p "pSphere2Follicle9677";
	rename -uid "FC9E06D2-4B34-C329-1981-A08DF0624AFE";
	setAttr -k off ".v";
	setAttr ".pu" 0.96875;
	setAttr ".pv" 0.78125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve477" -p "pSphere2Follicle9677";
	rename -uid "8C1037DA-4EAD-E966-E2A1-1BB652C59F61";
createNode nurbsCurve -n "curveShape477" -p "curve477";
	rename -uid "FEFA83F0-45C1-0FEA-A40A-1799752CCAF8";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "C4CFC681-4CCC-301B-44B3-3197842002AE";
createNode follicle -n "pSphere2FollicleShape9684" -p "pSphere2Follicle9684";
	rename -uid "D56DD8DD-4BDA-58E1-9B32-7A9C77A77ACD";
	setAttr -k off ".v";
	setAttr ".pu" 0.96875;
	setAttr ".pv" 0.84375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve478" -p "pSphere2Follicle9684";
	rename -uid "A96A7829-424D-905D-F7FE-639E7CC2048A";
createNode nurbsCurve -n "curveShape478" -p "curve478";
	rename -uid "2BE7B571-46A6-0B35-087C-3F8DE186C9DC";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "E1E255F3-4BA5-7F54-393F-4389F3E2B6BD";
createNode follicle -n "pSphere2FollicleShape9690" -p "pSphere2Follicle9690";
	rename -uid "1372EB81-4291-148D-A62F-31AB3D045B69";
	setAttr -k off ".v";
	setAttr ".pu" 0.96875;
	setAttr ".pv" 0.90625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve479" -p "pSphere2Follicle9690";
	rename -uid "DFE9E7B3-4E64-CC1B-1303-A18A750B94CC";
createNode nurbsCurve -n "curveShape479" -p "curve479";
	rename -uid "14987D3C-4378-1736-F914-18B40A226D04";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "4F518199-45FE-4AB7-E7D1-A4B94E3AC621";
createNode follicle -n "pSphere2FollicleShape9696" -p "pSphere2Follicle9696";
	rename -uid "F42AADA7-42ED-9F3D-B8D6-1095DC9A7B70";
	setAttr -k off ".v";
	setAttr ".pu" 0.96875;
	setAttr ".pv" 0.96875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve480" -p "pSphere2Follicle9696";
	rename -uid "3F5B5103-4EAA-7ED9-D77C-2F9652BD233A";
createNode nurbsCurve -n "curveShape480" -p "curve480";
	rename -uid "6427D2A1-46B9-F489-0225-FA9012531321";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.22222222220000001
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
	rename -uid "A58D839D-4F5F-D86B-6B34-B5A95B4E0227";
createNode pfxHair -n "pfxHairShape2" -p "pfxHair2";
	rename -uid "7765119C-47B5-F0D8-CDBD-D4BA91697894";
	setAttr -k off ".v";
	setAttr ".dpc" 100;
	setAttr ".dam" no;
createNode transform -n "directionalLight1";
	rename -uid "F3068B9A-43C6-3700-C237-6382F03B4FDC";
	setAttr ".r" -type "double3" 36.395093627554211 109.71510970921861 84.947632226673704 ;
	setAttr ".s" -type "double3" 5.5144173827393006 5.5144173827393006 5.5144173827393006 ;
createNode directionalLight -n "directionalLightShape1" -p "directionalLight1";
	rename -uid "9EEB1882-4F49-D43F-464B-18B4DEB7C1DD";
	addAttr -ci true -sn "ai_bounce_factor" -ln "aiBounceFactor" -dv 1 -min 0 -smx 
		20 -at "float";
	addAttr -ci true -sn "ai_bounces" -ln "aiBounces" -dv 999 -min 0 -max 10000 -at "long";
	setAttr -k off ".v";
	setAttr ".urs" no;
	setAttr ".rdl" 1;
createNode lightLinker -s -n "lightLinker1";
	rename -uid "29EAD626-4074-ACEC-E9AE-30AD27B57AD0";
	setAttr -s 7 ".lnk";
	setAttr -s 7 ".slnk";
createNode displayLayerManager -n "layerManager";
	rename -uid "FFE862C5-4B57-583A-EECB-7BBB35C437AD";
createNode displayLayer -n "defaultLayer";
	rename -uid "FC51E6C1-476B-986E-9EA9-D382CE24ADB6";
createNode renderLayerManager -n "renderLayerManager";
	rename -uid "5D30BABE-497C-DBF8-DC11-A29460A46D57";
createNode renderLayer -n "defaultRenderLayer";
	rename -uid "E1567478-4C4F-1765-FC7A-2F8DEDE72285";
	setAttr ".g" yes;
createNode aiOptions -s -n "defaultArnoldRenderOptions";
	rename -uid "A103151E-4870-E1A7-F4E5-32856DDEF67B";
createNode script -n "uiConfigurationScriptNode";
	rename -uid "6AFC226F-4C80-2BE1-0184-AAA092443CBE";
	setAttr ".b" -type "string" (
		"// Maya Mel UI Configuration File.\n//\n//  This script is machine generated.  Edit at your own risk.\n//\n//\n\nglobal string $gMainPane;\nif (`paneLayout -exists $gMainPane`) {\n\n\tglobal int $gUseScenePanelConfig;\n\tint    $useSceneConfig = $gUseScenePanelConfig;\n\tint    $menusOkayInPanels = `optionVar -q allowMenusInPanels`;\tint    $nVisPanes = `paneLayout -q -nvp $gMainPane`;\n\tint    $nPanes = 0;\n\tstring $editorName;\n\tstring $panelName;\n\tstring $itemFilterName;\n\tstring $panelConfig;\n\n\t//\n\t//  get current state of the UI\n\t//\n\tsceneUIReplacement -update $gMainPane;\n\n\t$panelName = `sceneUIReplacement -getNextPanel \"modelPanel\" (localizedPanelLabel(\"Top View\")) `;\n\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tmodelPanel -edit -l (localizedPanelLabel(\"Top View\")) -mbv $menusOkayInPanels  $panelName;\n\t\t$editorName = $panelName;\n        modelEditor -e \n            -camera \"top\" \n            -useInteractiveMode 0\n            -displayLights \"default\" \n            -displayAppearance \"wireframe\" \n            -activeOnly 0\n"
		+ "            -ignorePanZoom 0\n            -wireframeOnShaded 0\n            -headsUpDisplay 1\n            -holdOuts 1\n            -selectionHiliteDisplay 1\n            -useDefaultMaterial 0\n            -bufferMode \"double\" \n            -twoSidedLighting 1\n            -backfaceCulling 0\n            -xray 0\n            -jointXray 0\n            -activeComponentsXray 0\n            -displayTextures 0\n            -smoothWireframe 0\n            -lineWidth 1\n            -textureAnisotropic 0\n            -textureHilight 1\n            -textureSampling 2\n            -textureDisplay \"modulate\" \n            -textureMaxSize 16384\n            -fogging 0\n            -fogSource \"fragment\" \n            -fogMode \"linear\" \n            -fogStart 0\n            -fogEnd 100\n            -fogDensity 0.1\n            -fogColor 0.5 0.5 0.5 1 \n            -depthOfFieldPreview 1\n            -maxConstantTransparency 1\n            -rendererName \"base_OpenGL_Renderer\" \n            -objectFilterShowInHUD 1\n            -isFiltered 0\n            -colorResolution 256 256 \n"
		+ "            -bumpResolution 512 512 \n            -textureCompression 0\n            -transparencyAlgorithm \"frontAndBackCull\" \n            -transpInShadows 0\n            -cullingOverride \"none\" \n            -lowQualityLighting 0\n            -maximumNumHardwareLights 1\n            -occlusionCulling 0\n            -shadingModel 0\n            -useBaseRenderer 0\n            -useReducedRenderer 0\n            -smallObjectCulling 0\n            -smallObjectThreshold -1 \n            -interactiveDisableShadows 0\n            -interactiveBackFaceCull 0\n            -sortTransparent 1\n            -nurbsCurves 1\n            -nurbsSurfaces 1\n            -polymeshes 1\n            -subdivSurfaces 1\n            -planes 1\n            -lights 1\n            -cameras 1\n            -controlVertices 1\n            -hulls 1\n            -grid 1\n            -imagePlane 1\n            -joints 1\n            -ikHandles 1\n            -deformers 1\n            -dynamics 1\n            -particleInstancers 1\n            -fluids 1\n            -hairSystems 1\n"
		+ "            -follicles 1\n            -nCloths 1\n            -nParticles 1\n            -nRigids 1\n            -dynamicConstraints 1\n            -locators 1\n            -manipulators 1\n            -pluginShapes 1\n            -dimensions 1\n            -handles 1\n            -pivots 1\n            -textures 1\n            -strokes 1\n            -motionTrails 1\n            -clipGhosts 1\n            -greasePencils 1\n            -shadows 0\n            -captureSequenceNumber -1\n            -width 229\n            -height 336\n            -sceneRenderFilter 0\n            $editorName;\n        modelEditor -e -viewSelected 0 $editorName;\n        modelEditor -e \n            -pluginObjects \"gpuCacheDisplayFilter\" 1 \n            $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextPanel \"modelPanel\" (localizedPanelLabel(\"Side View\")) `;\n\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tmodelPanel -edit -l (localizedPanelLabel(\"Side View\")) -mbv $menusOkayInPanels  $panelName;\n"
		+ "\t\t$editorName = $panelName;\n        modelEditor -e \n            -camera \"side\" \n            -useInteractiveMode 0\n            -displayLights \"default\" \n            -displayAppearance \"wireframe\" \n            -activeOnly 0\n            -ignorePanZoom 0\n            -wireframeOnShaded 0\n            -headsUpDisplay 1\n            -holdOuts 1\n            -selectionHiliteDisplay 1\n            -useDefaultMaterial 0\n            -bufferMode \"double\" \n            -twoSidedLighting 1\n            -backfaceCulling 0\n            -xray 0\n            -jointXray 0\n            -activeComponentsXray 0\n            -displayTextures 0\n            -smoothWireframe 0\n            -lineWidth 1\n            -textureAnisotropic 0\n            -textureHilight 1\n            -textureSampling 2\n            -textureDisplay \"modulate\" \n            -textureMaxSize 16384\n            -fogging 0\n            -fogSource \"fragment\" \n            -fogMode \"linear\" \n            -fogStart 0\n            -fogEnd 100\n            -fogDensity 0.1\n            -fogColor 0.5 0.5 0.5 1 \n"
		+ "            -depthOfFieldPreview 1\n            -maxConstantTransparency 1\n            -rendererName \"base_OpenGL_Renderer\" \n            -objectFilterShowInHUD 1\n            -isFiltered 0\n            -colorResolution 256 256 \n            -bumpResolution 512 512 \n            -textureCompression 0\n            -transparencyAlgorithm \"frontAndBackCull\" \n            -transpInShadows 0\n            -cullingOverride \"none\" \n            -lowQualityLighting 0\n            -maximumNumHardwareLights 1\n            -occlusionCulling 0\n            -shadingModel 0\n            -useBaseRenderer 0\n            -useReducedRenderer 0\n            -smallObjectCulling 0\n            -smallObjectThreshold -1 \n            -interactiveDisableShadows 0\n            -interactiveBackFaceCull 0\n            -sortTransparent 1\n            -nurbsCurves 1\n            -nurbsSurfaces 1\n            -polymeshes 1\n            -subdivSurfaces 1\n            -planes 1\n            -lights 1\n            -cameras 1\n            -controlVertices 1\n            -hulls 1\n"
		+ "            -grid 1\n            -imagePlane 1\n            -joints 1\n            -ikHandles 1\n            -deformers 1\n            -dynamics 1\n            -particleInstancers 1\n            -fluids 1\n            -hairSystems 1\n            -follicles 1\n            -nCloths 1\n            -nParticles 1\n            -nRigids 1\n            -dynamicConstraints 1\n            -locators 1\n            -manipulators 1\n            -pluginShapes 1\n            -dimensions 1\n            -handles 1\n            -pivots 1\n            -textures 1\n            -strokes 1\n            -motionTrails 1\n            -clipGhosts 1\n            -greasePencils 1\n            -shadows 0\n            -captureSequenceNumber -1\n            -width 228\n            -height 336\n            -sceneRenderFilter 0\n            $editorName;\n        modelEditor -e -viewSelected 0 $editorName;\n        modelEditor -e \n            -pluginObjects \"gpuCacheDisplayFilter\" 1 \n            $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n"
		+ "\n\n\t$panelName = `sceneUIReplacement -getNextPanel \"modelPanel\" (localizedPanelLabel(\"Front View\")) `;\n\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tmodelPanel -edit -l (localizedPanelLabel(\"Front View\")) -mbv $menusOkayInPanels  $panelName;\n\t\t$editorName = $panelName;\n        modelEditor -e \n            -camera \"front\" \n            -useInteractiveMode 0\n            -displayLights \"default\" \n            -displayAppearance \"wireframe\" \n            -activeOnly 0\n            -ignorePanZoom 0\n            -wireframeOnShaded 0\n            -headsUpDisplay 1\n            -holdOuts 1\n            -selectionHiliteDisplay 1\n            -useDefaultMaterial 0\n            -bufferMode \"double\" \n            -twoSidedLighting 1\n            -backfaceCulling 0\n            -xray 0\n            -jointXray 0\n            -activeComponentsXray 0\n            -displayTextures 0\n            -smoothWireframe 0\n            -lineWidth 1\n            -textureAnisotropic 0\n            -textureHilight 1\n            -textureSampling 2\n"
		+ "            -textureDisplay \"modulate\" \n            -textureMaxSize 16384\n            -fogging 0\n            -fogSource \"fragment\" \n            -fogMode \"linear\" \n            -fogStart 0\n            -fogEnd 100\n            -fogDensity 0.1\n            -fogColor 0.5 0.5 0.5 1 \n            -depthOfFieldPreview 1\n            -maxConstantTransparency 1\n            -rendererName \"base_OpenGL_Renderer\" \n            -objectFilterShowInHUD 1\n            -isFiltered 0\n            -colorResolution 256 256 \n            -bumpResolution 512 512 \n            -textureCompression 0\n            -transparencyAlgorithm \"frontAndBackCull\" \n            -transpInShadows 0\n            -cullingOverride \"none\" \n            -lowQualityLighting 0\n            -maximumNumHardwareLights 1\n            -occlusionCulling 0\n            -shadingModel 0\n            -useBaseRenderer 0\n            -useReducedRenderer 0\n            -smallObjectCulling 0\n            -smallObjectThreshold -1 \n            -interactiveDisableShadows 0\n            -interactiveBackFaceCull 0\n"
		+ "            -sortTransparent 1\n            -nurbsCurves 1\n            -nurbsSurfaces 1\n            -polymeshes 1\n            -subdivSurfaces 1\n            -planes 1\n            -lights 1\n            -cameras 1\n            -controlVertices 1\n            -hulls 1\n            -grid 1\n            -imagePlane 1\n            -joints 1\n            -ikHandles 1\n            -deformers 1\n            -dynamics 1\n            -particleInstancers 1\n            -fluids 1\n            -hairSystems 1\n            -follicles 1\n            -nCloths 1\n            -nParticles 1\n            -nRigids 1\n            -dynamicConstraints 1\n            -locators 1\n            -manipulators 1\n            -pluginShapes 1\n            -dimensions 1\n            -handles 1\n            -pivots 1\n            -textures 1\n            -strokes 1\n            -motionTrails 1\n            -clipGhosts 1\n            -greasePencils 1\n            -shadows 0\n            -captureSequenceNumber -1\n            -width 229\n            -height 336\n            -sceneRenderFilter 0\n"
		+ "            $editorName;\n        modelEditor -e -viewSelected 0 $editorName;\n        modelEditor -e \n            -pluginObjects \"gpuCacheDisplayFilter\" 1 \n            $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextPanel \"modelPanel\" (localizedPanelLabel(\"Persp View\")) `;\n\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tmodelPanel -edit -l (localizedPanelLabel(\"Persp View\")) -mbv $menusOkayInPanels  $panelName;\n\t\t$editorName = $panelName;\n        modelEditor -e \n            -camera \"persp\" \n            -useInteractiveMode 0\n            -displayLights \"default\" \n            -displayAppearance \"smoothShaded\" \n            -activeOnly 0\n            -ignorePanZoom 0\n            -wireframeOnShaded 0\n            -headsUpDisplay 1\n            -holdOuts 1\n            -selectionHiliteDisplay 1\n            -useDefaultMaterial 0\n            -bufferMode \"double\" \n            -twoSidedLighting 1\n            -backfaceCulling 0\n            -xray 0\n"
		+ "            -jointXray 0\n            -activeComponentsXray 0\n            -displayTextures 0\n            -smoothWireframe 0\n            -lineWidth 1\n            -textureAnisotropic 0\n            -textureHilight 1\n            -textureSampling 2\n            -textureDisplay \"modulate\" \n            -textureMaxSize 16384\n            -fogging 0\n            -fogSource \"fragment\" \n            -fogMode \"linear\" \n            -fogStart 0\n            -fogEnd 100\n            -fogDensity 0.1\n            -fogColor 0.5 0.5 0.5 1 \n            -depthOfFieldPreview 1\n            -maxConstantTransparency 1\n            -rendererName \"base_OpenGL_Renderer\" \n            -objectFilterShowInHUD 1\n            -isFiltered 0\n            -colorResolution 256 256 \n            -bumpResolution 512 512 \n            -textureCompression 0\n            -transparencyAlgorithm \"frontAndBackCull\" \n            -transpInShadows 0\n            -cullingOverride \"none\" \n            -lowQualityLighting 0\n            -maximumNumHardwareLights 1\n            -occlusionCulling 0\n"
		+ "            -shadingModel 0\n            -useBaseRenderer 0\n            -useReducedRenderer 0\n            -smallObjectCulling 0\n            -smallObjectThreshold -1 \n            -interactiveDisableShadows 0\n            -interactiveBackFaceCull 0\n            -sortTransparent 1\n            -nurbsCurves 1\n            -nurbsSurfaces 1\n            -polymeshes 1\n            -subdivSurfaces 1\n            -planes 1\n            -lights 1\n            -cameras 1\n            -controlVertices 1\n            -hulls 1\n            -grid 1\n            -imagePlane 1\n            -joints 1\n            -ikHandles 1\n            -deformers 1\n            -dynamics 1\n            -particleInstancers 1\n            -fluids 1\n            -hairSystems 1\n            -follicles 1\n            -nCloths 1\n            -nParticles 1\n            -nRigids 1\n            -dynamicConstraints 1\n            -locators 1\n            -manipulators 1\n            -pluginShapes 1\n            -dimensions 1\n            -handles 1\n            -pivots 1\n            -textures 1\n"
		+ "            -strokes 1\n            -motionTrails 1\n            -clipGhosts 1\n            -greasePencils 1\n            -shadows 0\n            -captureSequenceNumber -1\n            -width 464\n            -height 716\n            -sceneRenderFilter 0\n            $editorName;\n        modelEditor -e -viewSelected 0 $editorName;\n        modelEditor -e \n            -pluginObjects \"gpuCacheDisplayFilter\" 1 \n            $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextPanel \"outlinerPanel\" (localizedPanelLabel(\"Outliner\")) `;\n\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\toutlinerPanel -edit -l (localizedPanelLabel(\"Outliner\")) -mbv $menusOkayInPanels  $panelName;\n\t\t$editorName = $panelName;\n        outlinerEditor -e \n            -docTag \"isolOutln_fromSeln\" \n            -showShapes 1\n            -showAssignedMaterials 0\n            -showTimeEditor 1\n            -showReferenceNodes 1\n            -showReferenceMembers 1\n            -showAttributes 0\n"
		+ "            -showConnected 0\n            -showAnimCurvesOnly 0\n            -showMuteInfo 0\n            -organizeByLayer 1\n            -showAnimLayerWeight 1\n            -autoExpandLayers 1\n            -autoExpand 0\n            -showDagOnly 0\n            -showAssets 1\n            -showContainedOnly 1\n            -showPublishedAsConnected 0\n            -showContainerContents 1\n            -ignoreDagHierarchy 0\n            -expandConnections 0\n            -showUpstreamCurves 1\n            -showUnitlessCurves 1\n            -showCompounds 1\n            -showLeafs 1\n            -showNumericAttrsOnly 0\n            -highlightActive 1\n            -autoSelectNewObjects 0\n            -doNotSelectNewObjects 0\n            -dropIsParent 1\n            -transmitFilters 0\n            -setFilter \"defaultSetFilter\" \n            -showSetMembers 1\n            -allowMultiSelection 1\n            -alwaysToggleSelect 0\n            -directSelect 0\n            -isSet 0\n            -isSetMember 0\n            -displayMode \"DAG\" \n            -expandObjects 0\n"
		+ "            -setsIgnoreFilters 1\n            -containersIgnoreFilters 0\n            -editAttrName 0\n            -showAttrValues 0\n            -highlightSecondary 0\n            -showUVAttrsOnly 0\n            -showTextureNodesOnly 0\n            -attrAlphaOrder \"default\" \n            -animLayerFilterOptions \"allAffecting\" \n            -sortOrder \"none\" \n            -longNames 0\n            -niceNames 1\n            -showNamespace 1\n            -showPinIcons 0\n            -mapMotionTrails 0\n            -ignoreHiddenAttribute 0\n            -ignoreOutlinerColor 0\n            -renderFilterVisible 0\n            -renderFilterIndex 0\n            -selectionOrder \"chronological\" \n            -expandAttribute 0\n            $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"graphEditor\" (localizedPanelLabel(\"Graph Editor\")) `;\n\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Graph Editor\")) -mbv $menusOkayInPanels  $panelName;\n"
		+ "\n\t\t\t$editorName = ($panelName+\"OutlineEd\");\n            outlinerEditor -e \n                -showShapes 1\n                -showAssignedMaterials 0\n                -showTimeEditor 1\n                -showReferenceNodes 0\n                -showReferenceMembers 0\n                -showAttributes 1\n                -showConnected 1\n                -showAnimCurvesOnly 1\n                -showMuteInfo 0\n                -organizeByLayer 1\n                -showAnimLayerWeight 1\n                -autoExpandLayers 1\n                -autoExpand 1\n                -showDagOnly 0\n                -showAssets 1\n                -showContainedOnly 0\n                -showPublishedAsConnected 0\n                -showContainerContents 0\n                -ignoreDagHierarchy 0\n                -expandConnections 1\n                -showUpstreamCurves 1\n                -showUnitlessCurves 1\n                -showCompounds 0\n                -showLeafs 1\n                -showNumericAttrsOnly 1\n                -highlightActive 0\n                -autoSelectNewObjects 1\n"
		+ "                -doNotSelectNewObjects 0\n                -dropIsParent 1\n                -transmitFilters 1\n                -setFilter \"0\" \n                -showSetMembers 0\n                -allowMultiSelection 1\n                -alwaysToggleSelect 0\n                -directSelect 0\n                -displayMode \"DAG\" \n                -expandObjects 0\n                -setsIgnoreFilters 1\n                -containersIgnoreFilters 0\n                -editAttrName 0\n                -showAttrValues 0\n                -highlightSecondary 0\n                -showUVAttrsOnly 0\n                -showTextureNodesOnly 0\n                -attrAlphaOrder \"default\" \n                -animLayerFilterOptions \"allAffecting\" \n                -sortOrder \"none\" \n                -longNames 0\n                -niceNames 1\n                -showNamespace 1\n                -showPinIcons 1\n                -mapMotionTrails 1\n                -ignoreHiddenAttribute 0\n                -ignoreOutlinerColor 0\n                -renderFilterVisible 0\n                $editorName;\n"
		+ "\n\t\t\t$editorName = ($panelName+\"GraphEd\");\n            animCurveEditor -e \n                -displayKeys 1\n                -displayTangents 0\n                -displayActiveKeys 0\n                -displayActiveKeyTangents 1\n                -displayInfinities 0\n                -displayValues 0\n                -autoFit 1\n                -snapTime \"integer\" \n                -snapValue \"none\" \n                -showResults \"off\" \n                -showBufferCurves \"off\" \n                -smoothness \"fine\" \n                -resultSamples 1\n                -resultScreenSamples 0\n                -resultUpdate \"delayed\" \n                -showUpstreamCurves 1\n                -showCurveNames 0\n                -showActiveCurveNames 0\n                -stackedCurves 0\n                -stackedCurvesMin -1\n                -stackedCurvesMax 1\n                -stackedCurvesSpace 0.2\n                -displayNormalized 0\n                -preSelectionHighlight 0\n                -constrainDrag 0\n                -classicMode 1\n                -valueLinesToggle 1\n"
		+ "                $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"dopeSheetPanel\" (localizedPanelLabel(\"Dope Sheet\")) `;\n\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Dope Sheet\")) -mbv $menusOkayInPanels  $panelName;\n\n\t\t\t$editorName = ($panelName+\"OutlineEd\");\n            outlinerEditor -e \n                -showShapes 1\n                -showAssignedMaterials 0\n                -showTimeEditor 1\n                -showReferenceNodes 0\n                -showReferenceMembers 0\n                -showAttributes 1\n                -showConnected 1\n                -showAnimCurvesOnly 1\n                -showMuteInfo 0\n                -organizeByLayer 1\n                -showAnimLayerWeight 1\n                -autoExpandLayers 1\n                -autoExpand 0\n                -showDagOnly 0\n                -showAssets 1\n                -showContainedOnly 0\n                -showPublishedAsConnected 0\n"
		+ "                -showContainerContents 0\n                -ignoreDagHierarchy 0\n                -expandConnections 1\n                -showUpstreamCurves 1\n                -showUnitlessCurves 0\n                -showCompounds 1\n                -showLeafs 1\n                -showNumericAttrsOnly 1\n                -highlightActive 0\n                -autoSelectNewObjects 0\n                -doNotSelectNewObjects 1\n                -dropIsParent 1\n                -transmitFilters 0\n                -setFilter \"0\" \n                -showSetMembers 0\n                -allowMultiSelection 1\n                -alwaysToggleSelect 0\n                -directSelect 0\n                -displayMode \"DAG\" \n                -expandObjects 0\n                -setsIgnoreFilters 1\n                -containersIgnoreFilters 0\n                -editAttrName 0\n                -showAttrValues 0\n                -highlightSecondary 0\n                -showUVAttrsOnly 0\n                -showTextureNodesOnly 0\n                -attrAlphaOrder \"default\" \n                -animLayerFilterOptions \"allAffecting\" \n"
		+ "                -sortOrder \"none\" \n                -longNames 0\n                -niceNames 1\n                -showNamespace 1\n                -showPinIcons 0\n                -mapMotionTrails 1\n                -ignoreHiddenAttribute 0\n                -ignoreOutlinerColor 0\n                -renderFilterVisible 0\n                $editorName;\n\n\t\t\t$editorName = ($panelName+\"DopeSheetEd\");\n            dopeSheetEditor -e \n                -displayKeys 1\n                -displayTangents 0\n                -displayActiveKeys 0\n                -displayActiveKeyTangents 0\n                -displayInfinities 0\n                -displayValues 0\n                -autoFit 0\n                -snapTime \"integer\" \n                -snapValue \"none\" \n                -outliner \"dopeSheetPanel1OutlineEd\" \n                -showSummary 1\n                -showScene 0\n                -hierarchyBelow 0\n                -showTicks 1\n                -selectionWindow 0 0 0 0 \n                $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n"
		+ "\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"clipEditorPanel\" (localizedPanelLabel(\"Trax Editor\")) `;\n\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Trax Editor\")) -mbv $menusOkayInPanels  $panelName;\n\n\t\t\t$editorName = clipEditorNameFromPanel($panelName);\n            clipEditor -e \n                -displayKeys 0\n                -displayTangents 0\n                -displayActiveKeys 0\n                -displayActiveKeyTangents 0\n                -displayInfinities 0\n                -displayValues 0\n                -autoFit 0\n                -snapTime \"none\" \n                -snapValue \"none\" \n                -initialized 0\n                -manageSequencer 0 \n                $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"sequenceEditorPanel\" (localizedPanelLabel(\"Camera Sequencer\")) `;\n\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n"
		+ "\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Camera Sequencer\")) -mbv $menusOkayInPanels  $panelName;\n\n\t\t\t$editorName = sequenceEditorNameFromPanel($panelName);\n            clipEditor -e \n                -displayKeys 0\n                -displayTangents 0\n                -displayActiveKeys 0\n                -displayActiveKeyTangents 0\n                -displayInfinities 0\n                -displayValues 0\n                -autoFit 0\n                -snapTime \"none\" \n                -snapValue \"none\" \n                -initialized 0\n                -manageSequencer 1 \n                $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"hyperGraphPanel\" (localizedPanelLabel(\"Hypergraph Hierarchy\")) `;\n\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Hypergraph Hierarchy\")) -mbv $menusOkayInPanels  $panelName;\n\n\t\t\t$editorName = ($panelName+\"HyperGraphEd\");\n            hyperGraph -e \n"
		+ "                -graphLayoutStyle \"hierarchicalLayout\" \n                -orientation \"horiz\" \n                -mergeConnections 0\n                -zoom 1\n                -animateTransition 0\n                -showRelationships 1\n                -showShapes 0\n                -showDeformers 0\n                -showExpressions 0\n                -showConstraints 0\n                -showConnectionFromSelected 0\n                -showConnectionToSelected 0\n                -showConstraintLabels 0\n                -showUnderworld 0\n                -showInvisible 0\n                -transitionFrames 1\n                -opaqueContainers 0\n                -freeform 0\n                -imagePosition 0 0 \n                -imageScale 1\n                -imageEnabled 0\n                -graphType \"DAG\" \n                -heatMapDisplay 0\n                -updateSelection 1\n                -updateNodeAdded 1\n                -useDrawOverrideColor 0\n                -limitGraphTraversal -1\n                -range 0 0 \n                -iconSize \"smallIcons\" \n"
		+ "                -showCachedConnections 0\n                $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"visorPanel\" (localizedPanelLabel(\"Visor\")) `;\n\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Visor\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"polyTexturePlacementPanel\" (localizedPanelLabel(\"UV Editor\")) `;\n\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"UV Editor\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"renderWindowPanel\" (localizedPanelLabel(\"Render View\")) `;\n\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Render View\")) -mbv $menusOkayInPanels  $panelName;\n"
		+ "\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"dynRelEdPanel\" (localizedPanelLabel(\"Dynamic Relationships\")) `;\n\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Dynamic Relationships\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"relationshipPanel\" (localizedPanelLabel(\"Relationship Editor\")) `;\n\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Relationship Editor\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"referenceEditorPanel\" (localizedPanelLabel(\"Reference Editor\")) `;\n\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Reference Editor\")) -mbv $menusOkayInPanels  $panelName;\n"
		+ "\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"componentEditorPanel\" (localizedPanelLabel(\"Component Editor\")) `;\n\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Component Editor\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"dynPaintScriptedPanelType\" (localizedPanelLabel(\"Paint Effects\")) `;\n\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Paint Effects\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"scriptEditorPanel\" (localizedPanelLabel(\"Script Editor\")) `;\n\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Script Editor\")) -mbv $menusOkayInPanels  $panelName;\n"
		+ "\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"contentBrowserPanel\" (localizedPanelLabel(\"Content Browser\")) `;\n\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Content Browser\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"Stereo\" (localizedPanelLabel(\"Stereo\")) `;\n\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Stereo\")) -mbv $menusOkayInPanels  $panelName;\nstring $editorName = ($panelName+\"Editor\");\n            stereoCameraView -e \n                -camera \"persp\" \n                -useInteractiveMode 0\n                -displayLights \"default\" \n                -displayAppearance \"wireframe\" \n                -activeOnly 0\n                -ignorePanZoom 0\n                -wireframeOnShaded 0\n"
		+ "                -headsUpDisplay 1\n                -holdOuts 1\n                -selectionHiliteDisplay 1\n                -useDefaultMaterial 0\n                -bufferMode \"double\" \n                -twoSidedLighting 1\n                -backfaceCulling 0\n                -xray 0\n                -jointXray 0\n                -activeComponentsXray 0\n                -displayTextures 0\n                -smoothWireframe 0\n                -lineWidth 1\n                -textureAnisotropic 0\n                -textureHilight 1\n                -textureSampling 2\n                -textureDisplay \"modulate\" \n                -textureMaxSize 16384\n                -fogging 0\n                -fogSource \"fragment\" \n                -fogMode \"linear\" \n                -fogStart 0\n                -fogEnd 100\n                -fogDensity 0.1\n                -fogColor 0.5 0.5 0.5 1 \n                -depthOfFieldPreview 1\n                -maxConstantTransparency 1\n                -objectFilterShowInHUD 1\n                -isFiltered 0\n                -colorResolution 4 4 \n"
		+ "                -bumpResolution 4 4 \n                -textureCompression 0\n                -transparencyAlgorithm \"frontAndBackCull\" \n                -transpInShadows 0\n                -cullingOverride \"none\" \n                -lowQualityLighting 0\n                -maximumNumHardwareLights 0\n                -occlusionCulling 0\n                -shadingModel 0\n                -useBaseRenderer 0\n                -useReducedRenderer 0\n                -smallObjectCulling 0\n                -smallObjectThreshold -1 \n                -interactiveDisableShadows 0\n                -interactiveBackFaceCull 0\n                -sortTransparent 1\n                -nurbsCurves 1\n                -nurbsSurfaces 1\n                -polymeshes 1\n                -subdivSurfaces 1\n                -planes 1\n                -lights 1\n                -cameras 1\n                -controlVertices 1\n                -hulls 1\n                -grid 1\n                -imagePlane 1\n                -joints 1\n                -ikHandles 1\n                -deformers 1\n"
		+ "                -dynamics 1\n                -particleInstancers 1\n                -fluids 1\n                -hairSystems 1\n                -follicles 1\n                -nCloths 1\n                -nParticles 1\n                -nRigids 1\n                -dynamicConstraints 1\n                -locators 1\n                -manipulators 1\n                -pluginShapes 1\n                -dimensions 1\n                -handles 1\n                -pivots 1\n                -textures 1\n                -strokes 1\n                -motionTrails 1\n                -clipGhosts 1\n                -greasePencils 1\n                -shadows 0\n                -captureSequenceNumber -1\n                -width 0\n                -height 0\n                -sceneRenderFilter 0\n                -displayMode \"centerEye\" \n                -viewColor 0 0 0 1 \n                -useCustomBackground 1\n                $editorName;\n            stereoCameraView -e -viewSelected 0 $editorName;\n            stereoCameraView -e \n                -pluginObjects \"gpuCacheDisplayFilter\" 1 \n"
		+ "                $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextPanel \"blendShapePanel\" (localizedPanelLabel(\"Blend Shape\")) `;\n\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tblendShapePanel -edit -l (localizedPanelLabel(\"Blend Shape\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"hyperShadePanel\" (localizedPanelLabel(\"Hypershade\")) `;\n\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Hypershade\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"nodeEditorPanel\" (localizedPanelLabel(\"Node Editor\")) `;\n\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Node Editor\")) -mbv $menusOkayInPanels  $panelName;\n"
		+ "\n\t\t\t$editorName = ($panelName+\"NodeEditorEd\");\n            nodeEditor -e \n                -allAttributes 0\n                -allNodes 0\n                -autoSizeNodes 1\n                -consistentNameSize 1\n                -createNodeCommand \"nodeEdCreateNodeCommand\" \n                -defaultPinnedState 0\n                -additiveGraphingMode 0\n                -settingsChangedCallback \"nodeEdSyncControls\" \n                -traversalDepthLimit -1\n                -keyPressCommand \"nodeEdKeyPressCommand\" \n                -nodeTitleMode \"name\" \n                -gridSnap 0\n                -gridVisibility 1\n                -popupMenuScript \"nodeEdBuildPanelMenus\" \n                -showNamespace 1\n                -showShapes 1\n                -showSGShapes 0\n                -showTransforms 1\n                -useAssets 1\n                -syncedSelection 1\n                -extendToShapes 1\n                -activeTab -1\n                -editorMode \"default\" \n                $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n"
		+ "\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"createNodePanel\" (localizedPanelLabel(\"Create Node\")) `;\n\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Create Node\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextPanel \"shapePanel\" (localizedPanelLabel(\"Shape Editor\")) `;\n\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tshapePanel -edit -l (localizedPanelLabel(\"Shape Editor\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextPanel \"posePanel\" (localizedPanelLabel(\"Pose Editor\")) `;\n\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tposePanel -edit -l (localizedPanelLabel(\"Pose Editor\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"profilerPanel\" (localizedPanelLabel(\"Profiler Tool\")) `;\n"
		+ "\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Profiler Tool\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"timeEditorPanel\" (localizedPanelLabel(\"Time Editor\")) `;\n\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Time Editor\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextPanel \"outlinerPanel\" (localizedPanelLabel(\"ToggledOutliner\")) `;\n\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\toutlinerPanel -edit -l (localizedPanelLabel(\"ToggledOutliner\")) -mbv $menusOkayInPanels  $panelName;\n\t\t$editorName = $panelName;\n        outlinerEditor -e \n            -showShapes 0\n            -showAssignedMaterials 0\n            -showTimeEditor 1\n            -showReferenceNodes 0\n"
		+ "            -showReferenceMembers 0\n            -showAttributes 0\n            -showConnected 0\n            -showAnimCurvesOnly 0\n            -showMuteInfo 0\n            -organizeByLayer 1\n            -showAnimLayerWeight 1\n            -autoExpandLayers 1\n            -autoExpand 0\n            -showDagOnly 1\n            -showAssets 1\n            -showContainedOnly 1\n            -showPublishedAsConnected 0\n            -showContainerContents 1\n            -ignoreDagHierarchy 0\n            -expandConnections 0\n            -showUpstreamCurves 1\n            -showUnitlessCurves 1\n            -showCompounds 1\n            -showLeafs 1\n            -showNumericAttrsOnly 0\n            -highlightActive 1\n            -autoSelectNewObjects 0\n            -doNotSelectNewObjects 0\n            -dropIsParent 1\n            -transmitFilters 0\n            -setFilter \"0\" \n            -showSetMembers 1\n            -allowMultiSelection 1\n            -alwaysToggleSelect 0\n            -directSelect 0\n            -isSet 0\n            -isSetMember 0\n"
		+ "            -displayMode \"DAG\" \n            -expandObjects 0\n            -setsIgnoreFilters 1\n            -containersIgnoreFilters 0\n            -editAttrName 0\n            -showAttrValues 0\n            -highlightSecondary 0\n            -showUVAttrsOnly 0\n            -showTextureNodesOnly 0\n            -attrAlphaOrder \"default\" \n            -animLayerFilterOptions \"allAffecting\" \n            -sortOrder \"none\" \n            -longNames 0\n            -niceNames 1\n            -showNamespace 1\n            -showPinIcons 0\n            -mapMotionTrails 0\n            -ignoreHiddenAttribute 0\n            -ignoreOutlinerColor 0\n            -renderFilterVisible 0\n            -renderFilterIndex 0\n            -selectionOrder \"chronological\" \n            -expandAttribute 0\n            $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\tif ($useSceneConfig) {\n        string $configName = `getPanel -cwl (localizedPanelLabel(\"Current Layout\"))`;\n        if (\"\" != $configName) {\n\t\t\tpanelConfiguration -edit -label (localizedPanelLabel(\"Current Layout\")) \n"
		+ "\t\t\t\t-userCreated false\n\t\t\t\t-defaultImage \"vacantCell.xP:/\"\n\t\t\t\t-image \"\"\n\t\t\t\t-sc false\n\t\t\t\t-configString \"global string $gMainPane; paneLayout -e -cn \\\"single\\\" -ps 1 100 100 $gMainPane;\"\n\t\t\t\t-removeAllPanels\n\t\t\t\t-ap false\n\t\t\t\t\t(localizedPanelLabel(\"Persp View\")) \n\t\t\t\t\t\"modelPanel\"\n"
		+ "\t\t\t\t\t\"$panelName = `modelPanel -unParent -l (localizedPanelLabel(\\\"Persp View\\\")) -mbv $menusOkayInPanels `;\\n$editorName = $panelName;\\nmodelEditor -e \\n    -cam `findStartUpCamera persp` \\n    -useInteractiveMode 0\\n    -displayLights \\\"default\\\" \\n    -displayAppearance \\\"smoothShaded\\\" \\n    -activeOnly 0\\n    -ignorePanZoom 0\\n    -wireframeOnShaded 0\\n    -headsUpDisplay 1\\n    -holdOuts 1\\n    -selectionHiliteDisplay 1\\n    -useDefaultMaterial 0\\n    -bufferMode \\\"double\\\" \\n    -twoSidedLighting 1\\n    -backfaceCulling 0\\n    -xray 0\\n    -jointXray 0\\n    -activeComponentsXray 0\\n    -displayTextures 0\\n    -smoothWireframe 0\\n    -lineWidth 1\\n    -textureAnisotropic 0\\n    -textureHilight 1\\n    -textureSampling 2\\n    -textureDisplay \\\"modulate\\\" \\n    -textureMaxSize 16384\\n    -fogging 0\\n    -fogSource \\\"fragment\\\" \\n    -fogMode \\\"linear\\\" \\n    -fogStart 0\\n    -fogEnd 100\\n    -fogDensity 0.1\\n    -fogColor 0.5 0.5 0.5 1 \\n    -depthOfFieldPreview 1\\n    -maxConstantTransparency 1\\n    -rendererName \\\"base_OpenGL_Renderer\\\" \\n    -objectFilterShowInHUD 1\\n    -isFiltered 0\\n    -colorResolution 256 256 \\n    -bumpResolution 512 512 \\n    -textureCompression 0\\n    -transparencyAlgorithm \\\"frontAndBackCull\\\" \\n    -transpInShadows 0\\n    -cullingOverride \\\"none\\\" \\n    -lowQualityLighting 0\\n    -maximumNumHardwareLights 1\\n    -occlusionCulling 0\\n    -shadingModel 0\\n    -useBaseRenderer 0\\n    -useReducedRenderer 0\\n    -smallObjectCulling 0\\n    -smallObjectThreshold -1 \\n    -interactiveDisableShadows 0\\n    -interactiveBackFaceCull 0\\n    -sortTransparent 1\\n    -nurbsCurves 1\\n    -nurbsSurfaces 1\\n    -polymeshes 1\\n    -subdivSurfaces 1\\n    -planes 1\\n    -lights 1\\n    -cameras 1\\n    -controlVertices 1\\n    -hulls 1\\n    -grid 1\\n    -imagePlane 1\\n    -joints 1\\n    -ikHandles 1\\n    -deformers 1\\n    -dynamics 1\\n    -particleInstancers 1\\n    -fluids 1\\n    -hairSystems 1\\n    -follicles 1\\n    -nCloths 1\\n    -nParticles 1\\n    -nRigids 1\\n    -dynamicConstraints 1\\n    -locators 1\\n    -manipulators 1\\n    -pluginShapes 1\\n    -dimensions 1\\n    -handles 1\\n    -pivots 1\\n    -textures 1\\n    -strokes 1\\n    -motionTrails 1\\n    -clipGhosts 1\\n    -greasePencils 1\\n    -shadows 0\\n    -captureSequenceNumber -1\\n    -width 464\\n    -height 716\\n    -sceneRenderFilter 0\\n    $editorName;\\nmodelEditor -e -viewSelected 0 $editorName;\\nmodelEditor -e \\n    -pluginObjects \\\"gpuCacheDisplayFilter\\\" 1 \\n    $editorName\"\n"
		+ "\t\t\t\t\t\"modelPanel -edit -l (localizedPanelLabel(\\\"Persp View\\\")) -mbv $menusOkayInPanels  $panelName;\\n$editorName = $panelName;\\nmodelEditor -e \\n    -cam `findStartUpCamera persp` \\n    -useInteractiveMode 0\\n    -displayLights \\\"default\\\" \\n    -displayAppearance \\\"smoothShaded\\\" \\n    -activeOnly 0\\n    -ignorePanZoom 0\\n    -wireframeOnShaded 0\\n    -headsUpDisplay 1\\n    -holdOuts 1\\n    -selectionHiliteDisplay 1\\n    -useDefaultMaterial 0\\n    -bufferMode \\\"double\\\" \\n    -twoSidedLighting 1\\n    -backfaceCulling 0\\n    -xray 0\\n    -jointXray 0\\n    -activeComponentsXray 0\\n    -displayTextures 0\\n    -smoothWireframe 0\\n    -lineWidth 1\\n    -textureAnisotropic 0\\n    -textureHilight 1\\n    -textureSampling 2\\n    -textureDisplay \\\"modulate\\\" \\n    -textureMaxSize 16384\\n    -fogging 0\\n    -fogSource \\\"fragment\\\" \\n    -fogMode \\\"linear\\\" \\n    -fogStart 0\\n    -fogEnd 100\\n    -fogDensity 0.1\\n    -fogColor 0.5 0.5 0.5 1 \\n    -depthOfFieldPreview 1\\n    -maxConstantTransparency 1\\n    -rendererName \\\"base_OpenGL_Renderer\\\" \\n    -objectFilterShowInHUD 1\\n    -isFiltered 0\\n    -colorResolution 256 256 \\n    -bumpResolution 512 512 \\n    -textureCompression 0\\n    -transparencyAlgorithm \\\"frontAndBackCull\\\" \\n    -transpInShadows 0\\n    -cullingOverride \\\"none\\\" \\n    -lowQualityLighting 0\\n    -maximumNumHardwareLights 1\\n    -occlusionCulling 0\\n    -shadingModel 0\\n    -useBaseRenderer 0\\n    -useReducedRenderer 0\\n    -smallObjectCulling 0\\n    -smallObjectThreshold -1 \\n    -interactiveDisableShadows 0\\n    -interactiveBackFaceCull 0\\n    -sortTransparent 1\\n    -nurbsCurves 1\\n    -nurbsSurfaces 1\\n    -polymeshes 1\\n    -subdivSurfaces 1\\n    -planes 1\\n    -lights 1\\n    -cameras 1\\n    -controlVertices 1\\n    -hulls 1\\n    -grid 1\\n    -imagePlane 1\\n    -joints 1\\n    -ikHandles 1\\n    -deformers 1\\n    -dynamics 1\\n    -particleInstancers 1\\n    -fluids 1\\n    -hairSystems 1\\n    -follicles 1\\n    -nCloths 1\\n    -nParticles 1\\n    -nRigids 1\\n    -dynamicConstraints 1\\n    -locators 1\\n    -manipulators 1\\n    -pluginShapes 1\\n    -dimensions 1\\n    -handles 1\\n    -pivots 1\\n    -textures 1\\n    -strokes 1\\n    -motionTrails 1\\n    -clipGhosts 1\\n    -greasePencils 1\\n    -shadows 0\\n    -captureSequenceNumber -1\\n    -width 464\\n    -height 716\\n    -sceneRenderFilter 0\\n    $editorName;\\nmodelEditor -e -viewSelected 0 $editorName;\\nmodelEditor -e \\n    -pluginObjects \\\"gpuCacheDisplayFilter\\\" 1 \\n    $editorName\"\n"
		+ "\t\t\t\t$configName;\n\n            setNamedPanelLayout (localizedPanelLabel(\"Current Layout\"));\n        }\n\n        panelHistory -e -clear mainPanelHistory;\n        sceneUIReplacement -clear;\n\t}\n\n\ngrid -spacing 5 -size 12 -divisions 5 -displayAxes yes -displayGridLines yes -displayDivisionLines yes -displayPerspectiveLabels no -displayOrthographicLabels no -displayAxesBold yes -perspectiveLabelPosition axis -orthographicLabelPosition edge;\nviewManip -drawCompass 0 -compassAngle 0 -frontParameters \"\" -homeParameters \"\" -selectionLockParameters \"\";\n}\n");
	setAttr ".st" 3;
createNode script -n "sceneConfigurationScriptNode";
	rename -uid "35270671-4418-2722-8739-5FB587F9A777";
	setAttr ".b" -type "string" "playbackOptions -min 1 -max 24 -ast 1 -aet 48 ";
	setAttr ".st" 6;
createNode shadingEngine -n "aiStandard1SG";
	rename -uid "B519F008-45D9-FF6D-686B-11BB4BF78E08";
	setAttr ".ihi" 0;
	setAttr ".ro" yes;
createNode materialInfo -n "materialInfo1";
	rename -uid "9B13163D-45B2-11DE-71AC-D5B08D9435A2";
createNode shadingEngine -n "aiUtility1SG";
	rename -uid "0F68927A-458F-4F0F-50A5-669944F71CCB";
	setAttr ".ihi" 0;
	setAttr ".ro" yes;
createNode materialInfo -n "materialInfo2";
	rename -uid "0C11F56E-4F3E-1D43-B1D9-E0840218F519";
createNode shadingEngine -n "aiStandard2SG";
	rename -uid "E16E15BB-4DF9-0C01-AFF4-69A2A846CAFB";
	setAttr ".ihi" 0;
	setAttr ".ro" yes;
createNode materialInfo -n "materialInfo3";
	rename -uid "18F65F9A-439F-81AF-94E2-EC9BDB2C88FB";
createNode polySphere -n "polySphere1";
	rename -uid "496EE20E-42EA-BF3F-8416-09954CBAAA94";
	setAttr ".r" 2;
createNode polySphere -n "polySphere2";
	rename -uid "0656BA96-4102-D882-4EFD-6997E0C35A19";
	setAttr ".r" 2;
createNode aiAOVFilter -s -n "defaultArnoldFilter";
	rename -uid "3CB7E50A-47B7-BE75-32AF-FDA609623795";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVDriver -s -n "defaultArnoldDriver";
	rename -uid "85B44F8D-4272-910F-662D-5CA55C2427B2";
	addAttr -ci true -sn "gamma" -ln "gamma" -dv 2.2000000476837158 -min 9.9999997473787516e-005 
		-smx 5 -at "float";
	addAttr -ci true -sn "dither_amplitude" -ln "ditherAmplitude" -dv 1 -at "float";
	setAttr ".ai_translator" -type "string" "tif";
createNode aiHair -n "aiHair1";
	rename -uid "CE7676B0-401E-DFB1-8FE7-F08602F3D204";
	setAttr ".rootcolor" -type "float3" 1 1 0 ;
	setAttr ".tipcolor" -type "float3" 0 1 0 ;
	setAttr ".kd_ind" 10;
createNode aiStandard -n "aiStandard3";
	rename -uid "BAB3379E-4625-EB91-12C5-68AAB5214D48";
	setAttr ".Kd_color" -type "float3" 1 0 0 ;
	setAttr ".emission" 1;
	setAttr ".emission_color" -type "float3" 0 1 1 ;
createNode aiStandard -n "aiStandard4";
	rename -uid "3367EE33-4DF2-5116-51AC-94B4EB5CE970";
createNode shadingEngine -n "aiHair1SG";
	rename -uid "EA17F46F-4C3F-C695-3974-D98B565A91BB";
	setAttr ".ihi" 0;
	setAttr ".ro" yes;
createNode materialInfo -n "materialInfo4";
	rename -uid "42839F7C-491C-6EB4-F81F-B29A51833C20";
createNode shadingEngine -n "aiStandard4SG";
	rename -uid "D2724DD5-4EA0-FA1E-6ACD-AAA7D20557D5";
	setAttr ".ihi" 0;
	setAttr -s 2 ".dsm";
	setAttr ".ro" yes;
createNode materialInfo -n "materialInfo5";
	rename -uid "E1CC2343-49A4-5891-9787-E6932E2590F1";
createNode shapeEditorManager -n "shapeEditorManager";
	rename -uid "B9923CAD-4D19-B7A3-A236-2B8F8DDDEB08";
createNode poseInterpolatorManager -n "poseInterpolatorManager";
	rename -uid "39D8EC89-4F0D-A9BF-81F5-6494FA6EBA62";
createNode aiAOVDriver -s -n "defaultArnoldDisplayDriver";
	rename -uid "B6DAABFE-4518-9611-1D88-DAAD8E7AE2A4";
	setAttr ".output_mode" 0;
	setAttr ".ai_translator" -type "string" "maya";
createNode nodeGraphEditorInfo -n "hyperShadePrimaryNodeEditorSavedTabsInfo";
	rename -uid "79101FEA-4B2B-AF07-2009-0CBD8DC2A2DB";
	setAttr ".tgi[0].tn" -type "string" "Untitled_1";
	setAttr ".tgi[0].vl" -type "double2" -333.33332008785726 -326.19046322883179 ;
	setAttr ".tgi[0].vh" -type "double2" 320.2380825129772 340.47617694688279 ;
select -ne :time1;
	setAttr -av -k on ".cch";
	setAttr -cb on ".ihi";
	setAttr -k on ".nds";
	setAttr -cb on ".bnm";
	setAttr -k on ".o" 1;
	setAttr ".unw" 1;
select -ne :hardwareRenderingGlobals;
	setAttr ".vac" 2;
select -ne :renderPartition;
	setAttr -k on ".cch";
	setAttr -cb on ".ihi";
	setAttr -k on ".nds";
	setAttr -cb on ".bnm";
	setAttr -s 7 ".st";
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
	setAttr -s 7 ".s";
select -ne :postProcessList1;
	setAttr -k on ".cch";
	setAttr -cb on ".ihi";
	setAttr -k on ".nds";
	setAttr -cb on ".bnm";
	setAttr -s 2 ".p";
select -ne :defaultRenderingList1;
select -ne :lightList1;
select -ne :initialShadingGroup;
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
	setAttr ".ro" yes;
select -ne :defaultColorMgtGlobals;
	setAttr ".cme" no;
select -ne :hardwareRenderGlobals;
	setAttr ".ctrs" 256;
	setAttr ".btrs" 512;
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
connectAttr ":defaultArnoldDisplayDriver.msg" ":defaultArnoldRenderOptions.drivers"
		 -na;
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
connectAttr "defaultRenderLayer.msg" ":defaultRenderingList1.r" -na;
connectAttr "directionalLightShape1.ltd" ":lightList1.l" -na;
connectAttr "directionalLight1.iog" ":defaultLightSet.dsm" -na;
// End of test.ma

//Maya ASCII 2018ff09 scene
//Name: test.ma
//Last modified: Wed, Mar 06, 2019 11:41:36 AM
//Codeset: 1252
requires maya "2018ff09";
requires "stereoCamera" "10.0";
requires -nodeType "aiOptions" -nodeType "aiAOVDriver" -nodeType "aiAOVFilter" -nodeType "aiStandard"
		 "mtoa" "3.2.0.wip";
requires "stereoCamera" "10.0";
currentUnit -l centimeter -a degree -t film;
fileInfo "application" "maya";
fileInfo "product" "Maya 2018";
fileInfo "version" "2018";
fileInfo "cutIdentifier" "201807182215-4524384375";
fileInfo "osv" "Microsoft Windows 7 Enterprise Edition, 64-bit Windows 7 Service Pack 1 (Build 7601)\n";
createNode transform -s -n "persp";
	rename -uid "29DCAF39-4BF7-2C37-2EC3-C1BA794D5ACA";
	setAttr ".v" no;
	setAttr ".t" -type "double3" 27.795094234407376 18.971162668046485 0 ;
	setAttr ".r" -type "double3" -24.000000000000004 90 0 ;
	setAttr ".rp" -type "double3" -3.5804692544161291e-15 -7.1054273576010019e-15 -1.4210854715202004e-14 ;
	setAttr ".rpt" -type "double3" -8.7783779362275568e-15 1.1498476670238775e-14 9.4020555375448207e-16 ;
createNode camera -s -n "perspShape" -p "persp";
	rename -uid "BD25D336-41F3-DC0E-D0CB-2FAC32789FED";
	setAttr -k off ".v" no;
	setAttr ".fl" 34.999999999999979;
	setAttr ".coi" 28.069721972594404;
	setAttr ".imn" -type "string" "persp";
	setAttr ".den" -type "string" "persp_depth";
	setAttr ".man" -type "string" "persp_mask";
	setAttr ".tp" -type "double3" 3.529134343177768 5.7733093735849303e-16 2.6000673943571693 ;
	setAttr ".hc" -type "string" "viewSet -p %camera";
	setAttr ".ai_translator" -type "string" "perspective";
createNode transform -s -n "top";
	rename -uid "C23D7D76-4BD1-3347-8959-14ADE13E9694";
	setAttr ".v" no;
	setAttr ".t" -type "double3" 0 100.1 0 ;
	setAttr ".r" -type "double3" -89.999999999999986 0 0 ;
createNode camera -s -n "topShape" -p "top";
	rename -uid "675FCA46-4118-6DB6-F5B7-59804A4BEECA";
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
	rename -uid "2570872E-40B7-B8EC-DB16-0B849D76C580";
	setAttr ".v" no;
	setAttr ".t" -type "double3" 0 0 100.1 ;
createNode camera -s -n "frontShape" -p "front";
	rename -uid "C79753C2-47C0-5FB4-E6C9-72A89DC5B813";
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
	rename -uid "0E7D730D-4427-CE2A-95B6-E6A36EFA1FE2";
	setAttr ".v" no;
	setAttr ".t" -type "double3" 100.1 1.6871751536864319 12.910557697774426 ;
	setAttr ".r" -type "double3" 0 89.999999999999986 0 ;
createNode camera -s -n "sideShape" -p "side";
	rename -uid "77AD584D-4EEE-9D19-A437-96ABD3EB7261";
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
	rename -uid "A94A0972-439B-BE50-0B5A-6B973AB7FDBA";
	setAttr ".t" -type "double3" -0.020665766309299727 6.3190084466120737 0.27269214094604877 ;
createNode mesh -n "pSphereShape1" -p "pSphere1";
	rename -uid "D01FF6CF-48DF-4600-CF65-CAB8AB3A628C";
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
createNode transform -n "directionalLight1";
	rename -uid "49C58441-44BA-3A1E-C1E5-4ABF517679AA";
	setAttr ".r" -type "double3" -48.156552041713944 1.3059010322141336 -9.5202672381429512 ;
	setAttr ".s" -type "double3" 30.769077416337016 30.769077416337016 30.769077416337016 ;
createNode directionalLight -n "directionalLightShape1" -p "directionalLight1";
	rename -uid "7CE3D8DC-47D4-0E9F-9CF6-5286ECB4A461";
	addAttr -ci true -sn "ai_bounce_factor" -ln "aiBounceFactor" -dv 1 -min 0 -smx 
		20 -at "float";
	addAttr -ci true -sn "ai_bounces" -ln "aiBounces" -dv 999 -min 0 -max 10000 -at "long";
	setAttr -k off ".v";
	setAttr ".in" 2;
	setAttr ".urs" no;
	setAttr ".rdl" 1;
createNode transform -n "hairSystem1";
	rename -uid "CA34B853-4A6C-BEEB-69B1-8D92E7FF7096";
createNode hairSystem -n "hairSystemShape1" -p "hairSystem1";
	rename -uid "688E5F4B-43A6-C32C-D667-129E46BAADC0";
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
	setAttr ".cwd" 4;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 1 3;
	setAttr -s 3 ".clc[0:2]"  0 0.50999999 1 0.5 0.49000001 1 1
		 0.50999999 1;
	setAttr ".cfl[0]"  0 0 1;
	setAttr ".hwd" 0.03;
	setAttr -s 2 ".hws[0:1]"  0.80000001 1 3 1 0.2 3;
	setAttr ".opc" 0.88571428579411338;
	setAttr ".hcl" -type "float3" 1 1 1 ;
	setAttr -s 3 ".hcs";
	setAttr ".hcs[0].hcsp" 0;
	setAttr ".hcs[0].hcsc" -type "float3" 1 0 0 ;
	setAttr ".hcs[0].hcsi" 3;
	setAttr ".hcs[1].hcsp" 0.5;
	setAttr ".hcs[1].hcsc" -type "float3" 1 1 0 ;
	setAttr ".hcs[1].hcsi" 3;
	setAttr ".hcs[2].hcsp" 1;
	setAttr ".hcs[2].hcsc" -type "float3" 1 0 0 ;
	setAttr ".hcs[2].hcsi" 3;
	setAttr ".hpc" 8;
	setAttr ".spc" -type "float3" 1 1 0.85716027 ;
	setAttr ".chr" 0.4;
	setAttr ".dsc[0]"  0 1 1;
	setAttr ".ai_opaque" no;
createNode transform -n "hairSystem1Follicles";
	rename -uid "80DD662A-46A6-5E3F-44CA-B09EBA6E172A";
createNode transform -n "pSphere1Follicle303" -p "hairSystem1Follicles";
	rename -uid "6444364F-4A2A-748D-9B78-8EBB792E11A0";
createNode follicle -n "pSphere1FollicleShape303" -p "pSphere1Follicle303";
	rename -uid "58F861D3-419E-5AFC-80A8-EEB822917DE4";
	setAttr -k off ".v";
	setAttr ".pu" 0.03125;
	setAttr ".pv" 0.03125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve1" -p "pSphere1Follicle303";
	rename -uid "6627010C-4A77-87A4-DF18-C5B4CE06C52F";
createNode nurbsCurve -n "curveShape1" -p "curve1";
	rename -uid "BC95CDC2-4FF0-D220-AF5A-97B87117D463";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
		0 0 0.66666666669999997
		0 0 1
		0 0 1.3333333329999999
		0 0 1.6666666670000001
		0 0 2
		0 0 2.3333333330000001
		0 0 2.6666666669999999
		0 0 3
		;
createNode transform -n "pSphere1Follicle309" -p "hairSystem1Follicles";
	rename -uid "8F76DC57-411E-9704-4388-78939DC46039";
createNode follicle -n "pSphere1FollicleShape309" -p "pSphere1Follicle309";
	rename -uid "DBFD2CC8-476C-8B6B-0018-0A9065C57189";
	setAttr -k off ".v";
	setAttr ".pu" 0.03125;
	setAttr ".pv" 0.09375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve2" -p "pSphere1Follicle309";
	rename -uid "392B6856-4B5F-DA4D-5227-5FB032B30ADF";
createNode nurbsCurve -n "curveShape2" -p "curve2";
	rename -uid "835F65B8-44D0-7666-9ADB-E0AB004598B9";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
		0 0 0.66666666669999997
		0 0 1
		0 0 1.3333333329999999
		0 0 1.6666666670000001
		0 0 2
		0 0 2.3333333330000001
		0 0 2.6666666669999999
		0 0 3
		;
createNode transform -n "pSphere1Follicle315" -p "hairSystem1Follicles";
	rename -uid "19E18543-4AB1-206B-9A9A-348476ECD12A";
createNode follicle -n "pSphere1FollicleShape315" -p "pSphere1Follicle315";
	rename -uid "F0C139C1-4F6D-A884-9635-37ADB774D8D3";
	setAttr -k off ".v";
	setAttr ".pu" 0.03125;
	setAttr ".pv" 0.15625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve3" -p "pSphere1Follicle315";
	rename -uid "B8732084-4B35-8907-3AE4-59869E71FE40";
createNode nurbsCurve -n "curveShape3" -p "curve3";
	rename -uid "2BBF2C78-4C82-3619-9C33-87B9269E8F5E";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
		0 0 0.66666666669999997
		0 0 1
		0 0 1.3333333329999999
		0 0 1.6666666670000001
		0 0 2
		0 0 2.3333333330000001
		0 0 2.6666666669999999
		0 0 3
		;
createNode transform -n "pSphere1Follicle322" -p "hairSystem1Follicles";
	rename -uid "287C2B09-4E73-5A5B-B807-D0A12051A094";
createNode follicle -n "pSphere1FollicleShape322" -p "pSphere1Follicle322";
	rename -uid "C6AD93A6-4BAA-3A5C-763A-1D8E885847DB";
	setAttr -k off ".v";
	setAttr ".pu" 0.03125;
	setAttr ".pv" 0.21875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve4" -p "pSphere1Follicle322";
	rename -uid "2DBA5F31-4FC1-C00B-4D04-7C958732652D";
createNode nurbsCurve -n "curveShape4" -p "curve4";
	rename -uid "5038DAFB-4922-3820-F464-36A12B02A448";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
		0 0 0.66666666669999997
		0 0 1
		0 0 1.3333333329999999
		0 0 1.6666666670000001
		0 0 2
		0 0 2.3333333330000001
		0 0 2.6666666669999999
		0 0 3
		;
createNode transform -n "pSphere1Follicle328" -p "hairSystem1Follicles";
	rename -uid "FD3EA3FC-434C-C620-62A6-08BCE89AFD4E";
createNode follicle -n "pSphere1FollicleShape328" -p "pSphere1Follicle328";
	rename -uid "5DA5897F-49D5-DCDF-BE57-81926F57BAC4";
	setAttr -k off ".v";
	setAttr ".pu" 0.03125;
	setAttr ".pv" 0.28125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve5" -p "pSphere1Follicle328";
	rename -uid "B3867193-40C9-3FEB-16E0-A299C9BBF28E";
createNode nurbsCurve -n "curveShape5" -p "curve5";
	rename -uid "F0AA8294-44E0-9DB5-BE60-709E7054DBC2";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
		0 0 0.66666666669999997
		0 0 1
		0 0 1.3333333329999999
		0 0 1.6666666670000001
		0 0 2
		0 0 2.3333333330000001
		0 0 2.6666666669999999
		0 0 3
		;
createNode transform -n "pSphere1Follicle334" -p "hairSystem1Follicles";
	rename -uid "5A7945B1-4D0C-8C08-D87D-4ABD5294D535";
createNode follicle -n "pSphere1FollicleShape334" -p "pSphere1Follicle334";
	rename -uid "E1C37F04-4B10-B947-41D9-DF8A64B0BFD3";
	setAttr -k off ".v";
	setAttr ".pu" 0.03125;
	setAttr ".pv" 0.34375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve6" -p "pSphere1Follicle334";
	rename -uid "FEB84701-45D9-CFE0-94B4-50BD5C5AB297";
createNode nurbsCurve -n "curveShape6" -p "curve6";
	rename -uid "B9B19C6D-47DB-C409-4E0E-BCB2820E328E";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
		0 0 0.66666666669999997
		0 0 1
		0 0 1.3333333329999999
		0 0 1.6666666670000001
		0 0 2
		0 0 2.3333333330000001
		0 0 2.6666666669999999
		0 0 3
		;
createNode transform -n "pSphere1Follicle340" -p "hairSystem1Follicles";
	rename -uid "56492035-42C1-CA7A-2363-7DBC8B9590B5";
createNode follicle -n "pSphere1FollicleShape340" -p "pSphere1Follicle340";
	rename -uid "1AD9DB81-4380-9AE0-2ABD-F491983733AE";
	setAttr -k off ".v";
	setAttr ".pu" 0.03125;
	setAttr ".pv" 0.40625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve7" -p "pSphere1Follicle340";
	rename -uid "8AA4D31C-4201-1520-AA71-C9A0DA720E0F";
createNode nurbsCurve -n "curveShape7" -p "curve7";
	rename -uid "9C3585E7-4ABE-BC1F-FF4D-068CFB6F561B";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
		0 0 0.66666666669999997
		0 0 1
		0 0 1.3333333329999999
		0 0 1.6666666670000001
		0 0 2
		0 0 2.3333333330000001
		0 0 2.6666666669999999
		0 0 3
		;
createNode transform -n "pSphere1Follicle346" -p "hairSystem1Follicles";
	rename -uid "357AB146-4C50-C460-2438-5FA0E072E3D3";
createNode follicle -n "pSphere1FollicleShape346" -p "pSphere1Follicle346";
	rename -uid "71781755-4B7D-106C-6DE4-E48602BE06CF";
	setAttr -k off ".v";
	setAttr ".pu" 0.03125;
	setAttr ".pv" 0.46875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve8" -p "pSphere1Follicle346";
	rename -uid "FEF04447-4C0D-D1D4-80D4-F781A47F6FF7";
createNode nurbsCurve -n "curveShape8" -p "curve8";
	rename -uid "1B619B1F-41FA-3452-24DB-A7A2FFE3B807";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
		0 0 0.66666666669999997
		0 0 1
		0 0 1.3333333329999999
		0 0 1.6666666670000001
		0 0 2
		0 0 2.3333333330000001
		0 0 2.6666666669999999
		0 0 3
		;
createNode transform -n "pSphere1Follicle353" -p "hairSystem1Follicles";
	rename -uid "7D910889-41BF-ED7D-45A4-AC86E16F5B61";
createNode follicle -n "pSphere1FollicleShape353" -p "pSphere1Follicle353";
	rename -uid "8CC681F8-4BA0-7407-A984-D2986C564DF6";
	setAttr -k off ".v";
	setAttr ".pu" 0.03125;
	setAttr ".pv" 0.53125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve9" -p "pSphere1Follicle353";
	rename -uid "F230BEE0-48A3-DBAA-F864-29A2A979CC61";
createNode nurbsCurve -n "curveShape9" -p "curve9";
	rename -uid "087B14F7-4BB0-46CB-42AF-AAAB6EA3FC02";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
		0 0 0.66666666669999997
		0 0 1
		0 0 1.3333333329999999
		0 0 1.6666666670000001
		0 0 2
		0 0 2.3333333330000001
		0 0 2.6666666669999999
		0 0 3
		;
createNode transform -n "pSphere1Follicle359" -p "hairSystem1Follicles";
	rename -uid "3CE72696-4D9A-093F-47D8-4DB3C4175C26";
createNode follicle -n "pSphere1FollicleShape359" -p "pSphere1Follicle359";
	rename -uid "CB910C74-42CE-106A-B286-9A8F12A176FD";
	setAttr -k off ".v";
	setAttr ".pu" 0.03125;
	setAttr ".pv" 0.59375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve10" -p "pSphere1Follicle359";
	rename -uid "A7DF6C8C-46B9-D0EF-938B-DDB0521C0B94";
createNode nurbsCurve -n "curveShape10" -p "curve10";
	rename -uid "8B38D861-444F-A15D-2057-2BBFC9B23C5B";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
		0 0 0.66666666669999997
		0 0 1
		0 0 1.3333333329999999
		0 0 1.6666666670000001
		0 0 2
		0 0 2.3333333330000001
		0 0 2.6666666669999999
		0 0 3
		;
createNode transform -n "pSphere1Follicle365" -p "hairSystem1Follicles";
	rename -uid "7591A2E9-435E-3868-5127-AA8B0A6D9B9F";
createNode follicle -n "pSphere1FollicleShape365" -p "pSphere1Follicle365";
	rename -uid "A7FC08DF-4E7A-D6ED-8CC0-929BFE9CB1E6";
	setAttr -k off ".v";
	setAttr ".pu" 0.03125;
	setAttr ".pv" 0.65625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve11" -p "pSphere1Follicle365";
	rename -uid "F5217885-424F-E7B4-A334-84BF7DCD81FC";
createNode nurbsCurve -n "curveShape11" -p "curve11";
	rename -uid "6C20F5DC-4E85-9D79-4065-86AB83637A38";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
		0 0 0.66666666669999997
		0 0 1
		0 0 1.3333333329999999
		0 0 1.6666666670000001
		0 0 2
		0 0 2.3333333330000001
		0 0 2.6666666669999999
		0 0 3
		;
createNode transform -n "pSphere1Follicle371" -p "hairSystem1Follicles";
	rename -uid "F95C0BEB-4E40-9AA5-8B54-8FB6781B7AEB";
createNode follicle -n "pSphere1FollicleShape371" -p "pSphere1Follicle371";
	rename -uid "C481927C-486D-4441-C721-A89A563427FB";
	setAttr -k off ".v";
	setAttr ".pu" 0.03125;
	setAttr ".pv" 0.71875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve12" -p "pSphere1Follicle371";
	rename -uid "86E65F73-4F54-DB87-0043-54844EDAA454";
createNode nurbsCurve -n "curveShape12" -p "curve12";
	rename -uid "42E76923-49D0-D1F9-611A-56BFEE482DCF";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
		0 0 0.66666666669999997
		0 0 1
		0 0 1.3333333329999999
		0 0 1.6666666670000001
		0 0 2
		0 0 2.3333333330000001
		0 0 2.6666666669999999
		0 0 3
		;
createNode transform -n "pSphere1Follicle377" -p "hairSystem1Follicles";
	rename -uid "0F80229A-480C-7983-4C8C-50AB90C6188A";
createNode follicle -n "pSphere1FollicleShape377" -p "pSphere1Follicle377";
	rename -uid "0CF6CAC3-4E25-518F-3371-4E94F0E0FA0C";
	setAttr -k off ".v";
	setAttr ".pu" 0.03125;
	setAttr ".pv" 0.78125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve13" -p "pSphere1Follicle377";
	rename -uid "585D7C3F-4EFF-364A-D8B8-D481E4399B63";
createNode nurbsCurve -n "curveShape13" -p "curve13";
	rename -uid "E84C12C7-4145-07A7-C004-6AB7CCA33D8C";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
		0 0 0.66666666669999997
		0 0 1
		0 0 1.3333333329999999
		0 0 1.6666666670000001
		0 0 2
		0 0 2.3333333330000001
		0 0 2.6666666669999999
		0 0 3
		;
createNode transform -n "pSphere1Follicle384" -p "hairSystem1Follicles";
	rename -uid "E49BEA0A-44A9-AC77-ADFF-2FAC5ECF9FC5";
createNode follicle -n "pSphere1FollicleShape384" -p "pSphere1Follicle384";
	rename -uid "2D3DEF32-4217-F706-DAE3-91800CFF738F";
	setAttr -k off ".v";
	setAttr ".pu" 0.03125;
	setAttr ".pv" 0.84375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve14" -p "pSphere1Follicle384";
	rename -uid "3F9830A1-40D8-A921-30EE-50B2BF4E7380";
createNode nurbsCurve -n "curveShape14" -p "curve14";
	rename -uid "42121663-498B-587D-8B74-28B204B2C855";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
		0 0 0.66666666669999997
		0 0 1
		0 0 1.3333333329999999
		0 0 1.6666666670000001
		0 0 2
		0 0 2.3333333330000001
		0 0 2.6666666669999999
		0 0 3
		;
createNode transform -n "pSphere1Follicle390" -p "hairSystem1Follicles";
	rename -uid "385C2330-49A6-0AED-24F0-F19891BF6798";
createNode follicle -n "pSphere1FollicleShape390" -p "pSphere1Follicle390";
	rename -uid "1DFEBD38-4AF1-51C1-D3B2-4B9315AC2E83";
	setAttr -k off ".v";
	setAttr ".pu" 0.03125;
	setAttr ".pv" 0.90625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve15" -p "pSphere1Follicle390";
	rename -uid "16F8C60C-4A74-5E20-35DC-E5AEA57589F0";
createNode nurbsCurve -n "curveShape15" -p "curve15";
	rename -uid "D10F59E4-4C21-7359-BEF6-B08FC9D1B1B7";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
		0 0 0.66666666669999997
		0 0 1
		0 0 1.3333333329999999
		0 0 1.6666666670000001
		0 0 2
		0 0 2.3333333330000001
		0 0 2.6666666669999999
		0 0 3
		;
createNode transform -n "pSphere1Follicle396" -p "hairSystem1Follicles";
	rename -uid "97628B33-40E6-1DCC-DF39-30812E1E6815";
createNode follicle -n "pSphere1FollicleShape396" -p "pSphere1Follicle396";
	rename -uid "D10A8346-442C-66DA-5438-7F988EF3CA82";
	setAttr -k off ".v";
	setAttr ".pu" 0.03125;
	setAttr ".pv" 0.96875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve16" -p "pSphere1Follicle396";
	rename -uid "5AD76535-47FB-888D-F259-6BA1A97B20E4";
createNode nurbsCurve -n "curveShape16" -p "curve16";
	rename -uid "4FE4B5E0-4A78-B25F-AA06-6EBC0C32304F";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
		0 0 0.66666666669999997
		0 0 1
		0 0 1.3333333329999999
		0 0 1.6666666670000001
		0 0 2
		0 0 2.3333333330000001
		0 0 2.6666666669999999
		0 0 3
		;
createNode transform -n "pSphere1Follicle909" -p "hairSystem1Follicles";
	rename -uid "38C8304B-43FB-F185-2D20-1FA9660DBA5A";
createNode follicle -n "pSphere1FollicleShape909" -p "pSphere1Follicle909";
	rename -uid "6776848E-4272-83E6-262A-C19037C517BC";
	setAttr -k off ".v";
	setAttr ".pu" 0.09375;
	setAttr ".pv" 0.09375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve17" -p "pSphere1Follicle909";
	rename -uid "05D84591-418C-082D-88B6-7A9F646A8D71";
createNode nurbsCurve -n "curveShape17" -p "curve17";
	rename -uid "3BFEC6EE-43AE-4DB7-DEBA-06A9C497EE44";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
		0 0 0.66666666669999997
		0 0 1
		0 0 1.3333333329999999
		0 0 1.6666666670000001
		0 0 2
		0 0 2.3333333330000001
		0 0 2.6666666669999999
		0 0 3
		;
createNode transform -n "pSphere1Follicle915" -p "hairSystem1Follicles";
	rename -uid "50E395D3-489C-EC9C-AB83-378D9DB9F2B0";
createNode follicle -n "pSphere1FollicleShape915" -p "pSphere1Follicle915";
	rename -uid "B1B350C4-49DA-02F7-3DCE-C8BC9B1DF554";
	setAttr -k off ".v";
	setAttr ".pu" 0.09375;
	setAttr ".pv" 0.15625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve18" -p "pSphere1Follicle915";
	rename -uid "76243D86-4E21-30B9-FB94-F0BE3A9C8C8E";
createNode nurbsCurve -n "curveShape18" -p "curve18";
	rename -uid "E37059B4-4CB5-9475-36D9-45A10BC48890";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
		0 0 0.66666666669999997
		0 0 1
		0 0 1.3333333329999999
		0 0 1.6666666670000001
		0 0 2
		0 0 2.3333333330000001
		0 0 2.6666666669999999
		0 0 3
		;
createNode transform -n "pSphere1Follicle922" -p "hairSystem1Follicles";
	rename -uid "9D77FBF2-4351-DFC5-3C58-38A6D2103EF4";
createNode follicle -n "pSphere1FollicleShape922" -p "pSphere1Follicle922";
	rename -uid "4E2EB0C0-4FE4-EB53-1D2D-56A26EB2DA25";
	setAttr -k off ".v";
	setAttr ".pu" 0.09375;
	setAttr ".pv" 0.21875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve19" -p "pSphere1Follicle922";
	rename -uid "5AB4CED9-4023-638A-36BB-B2AC011537F3";
createNode nurbsCurve -n "curveShape19" -p "curve19";
	rename -uid "A8507C39-42DB-0754-47F5-55A8BDB25E13";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
		0 0 0.66666666669999997
		0 0 1
		0 0 1.3333333329999999
		0 0 1.6666666670000001
		0 0 2
		0 0 2.3333333330000001
		0 0 2.6666666669999999
		0 0 3
		;
createNode transform -n "pSphere1Follicle928" -p "hairSystem1Follicles";
	rename -uid "0D53EBA9-49F5-7D4D-1045-C0A0631FB1DE";
createNode follicle -n "pSphere1FollicleShape928" -p "pSphere1Follicle928";
	rename -uid "A9747612-47D8-B8B5-871B-86AF3DCF6D91";
	setAttr -k off ".v";
	setAttr ".pu" 0.09375;
	setAttr ".pv" 0.28125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve20" -p "pSphere1Follicle928";
	rename -uid "81E57B3F-4C4C-5B60-E98F-A99B79DF5CF9";
createNode nurbsCurve -n "curveShape20" -p "curve20";
	rename -uid "A3D16D16-46AF-5A53-18F3-5994B3E1B41D";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
		0 0 0.66666666669999997
		0 0 1
		0 0 1.3333333329999999
		0 0 1.6666666670000001
		0 0 2
		0 0 2.3333333330000001
		0 0 2.6666666669999999
		0 0 3
		;
createNode transform -n "pSphere1Follicle934" -p "hairSystem1Follicles";
	rename -uid "74D0F2C7-4B45-42BF-AB0B-079827626882";
createNode follicle -n "pSphere1FollicleShape934" -p "pSphere1Follicle934";
	rename -uid "9B78EC29-4F3F-215B-C5EF-A7801B1C5E7F";
	setAttr -k off ".v";
	setAttr ".pu" 0.09375;
	setAttr ".pv" 0.34375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve21" -p "pSphere1Follicle934";
	rename -uid "A2B4ECB6-47D1-F652-5325-F386AB98E744";
createNode nurbsCurve -n "curveShape21" -p "curve21";
	rename -uid "41E98360-4CDC-EBDC-6227-82B01856E731";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
		0 0 0.66666666669999997
		0 0 1
		0 0 1.3333333329999999
		0 0 1.6666666670000001
		0 0 2
		0 0 2.3333333330000001
		0 0 2.6666666669999999
		0 0 3
		;
createNode transform -n "pSphere1Follicle940" -p "hairSystem1Follicles";
	rename -uid "B1940AF0-48F9-E468-973B-159C1995CC72";
createNode follicle -n "pSphere1FollicleShape940" -p "pSphere1Follicle940";
	rename -uid "7F6D01D6-446B-6A3C-57B2-8B857E2DF6AD";
	setAttr -k off ".v";
	setAttr ".pu" 0.09375;
	setAttr ".pv" 0.40625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve22" -p "pSphere1Follicle940";
	rename -uid "3E9A5DFD-4434-765D-6C41-DA9AED0083B2";
createNode nurbsCurve -n "curveShape22" -p "curve22";
	rename -uid "98FB60D4-48D7-06E3-90C4-379BEB74E919";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
		0 0 0.66666666669999997
		0 0 1
		0 0 1.3333333329999999
		0 0 1.6666666670000001
		0 0 2
		0 0 2.3333333330000001
		0 0 2.6666666669999999
		0 0 3
		;
createNode transform -n "pSphere1Follicle946" -p "hairSystem1Follicles";
	rename -uid "62D01523-4C75-3899-5277-9A88F7D810D5";
createNode follicle -n "pSphere1FollicleShape946" -p "pSphere1Follicle946";
	rename -uid "71544B5E-4C53-E385-E81E-CDBC3F320AB5";
	setAttr -k off ".v";
	setAttr ".pu" 0.09375;
	setAttr ".pv" 0.46875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve23" -p "pSphere1Follicle946";
	rename -uid "48EB300A-4A09-F78F-5473-3C8A47355B17";
createNode nurbsCurve -n "curveShape23" -p "curve23";
	rename -uid "5A85AF97-4647-4519-E108-BDA5691C0EAE";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
		0 0 0.66666666669999997
		0 0 1
		0 0 1.3333333329999999
		0 0 1.6666666670000001
		0 0 2
		0 0 2.3333333330000001
		0 0 2.6666666669999999
		0 0 3
		;
createNode transform -n "pSphere1Follicle953" -p "hairSystem1Follicles";
	rename -uid "742F701B-4E06-825B-E608-0DAE81B09885";
createNode follicle -n "pSphere1FollicleShape953" -p "pSphere1Follicle953";
	rename -uid "C676747A-4377-A58D-D772-CB92DCBA4D37";
	setAttr -k off ".v";
	setAttr ".pu" 0.09375;
	setAttr ".pv" 0.53125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve24" -p "pSphere1Follicle953";
	rename -uid "455E01BA-43E1-89D1-CF68-5A967BE5CC33";
createNode nurbsCurve -n "curveShape24" -p "curve24";
	rename -uid "F21876F2-4E89-4A4D-0F7D-DD8F898EB86C";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
		0 0 0.66666666669999997
		0 0 1
		0 0 1.3333333329999999
		0 0 1.6666666670000001
		0 0 2
		0 0 2.3333333330000001
		0 0 2.6666666669999999
		0 0 3
		;
createNode transform -n "pSphere1Follicle959" -p "hairSystem1Follicles";
	rename -uid "3FB34A48-43F2-5ECA-DCC9-45992697D7E7";
createNode follicle -n "pSphere1FollicleShape959" -p "pSphere1Follicle959";
	rename -uid "C38F9CDF-483E-9AC5-498A-4D81A12BEDAA";
	setAttr -k off ".v";
	setAttr ".pu" 0.09375;
	setAttr ".pv" 0.59375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve25" -p "pSphere1Follicle959";
	rename -uid "9C530B24-4788-05A9-42EE-33A33C06A28D";
createNode nurbsCurve -n "curveShape25" -p "curve25";
	rename -uid "80845B94-4B6A-4DDF-84B8-318256632F8A";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
		0 0 0.66666666669999997
		0 0 1
		0 0 1.3333333329999999
		0 0 1.6666666670000001
		0 0 2
		0 0 2.3333333330000001
		0 0 2.6666666669999999
		0 0 3
		;
createNode transform -n "pSphere1Follicle965" -p "hairSystem1Follicles";
	rename -uid "3D97A909-427A-6F7E-66DD-9E8C662FA6EF";
createNode follicle -n "pSphere1FollicleShape965" -p "pSphere1Follicle965";
	rename -uid "FA4E05CC-4ADF-F964-5C27-77BEEE326EC0";
	setAttr -k off ".v";
	setAttr ".pu" 0.09375;
	setAttr ".pv" 0.65625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve26" -p "pSphere1Follicle965";
	rename -uid "CB847807-426C-C17C-32D4-62A1629F3363";
createNode nurbsCurve -n "curveShape26" -p "curve26";
	rename -uid "ED811F71-47FF-BE6E-5574-2F86F7F3414F";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
		0 0 0.66666666669999997
		0 0 1
		0 0 1.3333333329999999
		0 0 1.6666666670000001
		0 0 2
		0 0 2.3333333330000001
		0 0 2.6666666669999999
		0 0 3
		;
createNode transform -n "pSphere1Follicle971" -p "hairSystem1Follicles";
	rename -uid "3C695BB1-4E77-0AC7-D734-A1AA4DADE5FA";
createNode follicle -n "pSphere1FollicleShape971" -p "pSphere1Follicle971";
	rename -uid "AFAD0A1E-4AB4-2BA3-5EE9-E9B9738632DC";
	setAttr -k off ".v";
	setAttr ".pu" 0.09375;
	setAttr ".pv" 0.71875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve27" -p "pSphere1Follicle971";
	rename -uid "08660EAF-41AA-43A3-1C51-CC8CD1C3ABD3";
createNode nurbsCurve -n "curveShape27" -p "curve27";
	rename -uid "988DE8C7-411C-1389-85A9-A593D6EAE073";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
		0 0 0.66666666669999997
		0 0 1
		0 0 1.3333333329999999
		0 0 1.6666666670000001
		0 0 2
		0 0 2.3333333330000001
		0 0 2.6666666669999999
		0 0 3
		;
createNode transform -n "pSphere1Follicle977" -p "hairSystem1Follicles";
	rename -uid "E0341527-4CA7-1F26-13A3-2989FE33131A";
createNode follicle -n "pSphere1FollicleShape977" -p "pSphere1Follicle977";
	rename -uid "7E631B31-4F35-7957-E989-11A1A742F05C";
	setAttr -k off ".v";
	setAttr ".pu" 0.09375;
	setAttr ".pv" 0.78125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve28" -p "pSphere1Follicle977";
	rename -uid "26A81D64-4C3D-08FB-A3E2-9CA64BC8F5C4";
createNode nurbsCurve -n "curveShape28" -p "curve28";
	rename -uid "3CDEEC36-4738-CDF1-A067-1AB0419A2508";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
		0 0 0.66666666669999997
		0 0 1
		0 0 1.3333333329999999
		0 0 1.6666666670000001
		0 0 2
		0 0 2.3333333330000001
		0 0 2.6666666669999999
		0 0 3
		;
createNode transform -n "pSphere1Follicle984" -p "hairSystem1Follicles";
	rename -uid "02A82A26-4B3C-1726-4D4D-9EA541CFA5C0";
createNode follicle -n "pSphere1FollicleShape984" -p "pSphere1Follicle984";
	rename -uid "F3B9CD62-4B4E-1F66-5850-A38473C3A77D";
	setAttr -k off ".v";
	setAttr ".pu" 0.09375;
	setAttr ".pv" 0.84375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve29" -p "pSphere1Follicle984";
	rename -uid "C8D3770A-4F74-F2EB-B45D-A5AF0CB2586C";
createNode nurbsCurve -n "curveShape29" -p "curve29";
	rename -uid "0ABB3000-4885-ADC8-E712-08ABF59295E8";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
		0 0 0.66666666669999997
		0 0 1
		0 0 1.3333333329999999
		0 0 1.6666666670000001
		0 0 2
		0 0 2.3333333330000001
		0 0 2.6666666669999999
		0 0 3
		;
createNode transform -n "pSphere1Follicle990" -p "hairSystem1Follicles";
	rename -uid "15E1A082-42A2-EA41-C87D-B1AEF9846A37";
createNode follicle -n "pSphere1FollicleShape990" -p "pSphere1Follicle990";
	rename -uid "AF350A25-44B8-031A-CF7F-BFA6AD2CF7AF";
	setAttr -k off ".v";
	setAttr ".pu" 0.09375;
	setAttr ".pv" 0.90625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve30" -p "pSphere1Follicle990";
	rename -uid "FA7CCFF8-4733-F4DD-13D8-A99CFF38723C";
createNode nurbsCurve -n "curveShape30" -p "curve30";
	rename -uid "ACB50B27-4D8E-48FB-61F3-39B59FDE7201";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
		0 0 0.66666666669999997
		0 0 1
		0 0 1.3333333329999999
		0 0 1.6666666670000001
		0 0 2
		0 0 2.3333333330000001
		0 0 2.6666666669999999
		0 0 3
		;
createNode transform -n "pSphere1Follicle1509" -p "hairSystem1Follicles";
	rename -uid "3DE9A10D-47D5-8BE2-1603-8DB3D61637B0";
createNode follicle -n "pSphere1FollicleShape1509" -p "pSphere1Follicle1509";
	rename -uid "ED4496FC-4716-241E-0253-C2BE22532148";
	setAttr -k off ".v";
	setAttr ".pu" 0.15625;
	setAttr ".pv" 0.09375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve31" -p "pSphere1Follicle1509";
	rename -uid "36FDC528-4976-EE78-AED7-F79EFDDB9F54";
createNode nurbsCurve -n "curveShape31" -p "curve31";
	rename -uid "9C55ECF1-4FE2-935E-30B5-4E9721BD2AF8";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
		0 0 0.66666666669999997
		0 0 1
		0 0 1.3333333329999999
		0 0 1.6666666670000001
		0 0 2
		0 0 2.3333333330000001
		0 0 2.6666666669999999
		0 0 3
		;
createNode transform -n "pSphere1Follicle1515" -p "hairSystem1Follicles";
	rename -uid "DC4DCB76-4E72-13C0-60A2-D2AF81CEBC20";
createNode follicle -n "pSphere1FollicleShape1515" -p "pSphere1Follicle1515";
	rename -uid "0BFC73FF-4D04-DCD9-6A71-BFB62E417E53";
	setAttr -k off ".v";
	setAttr ".pu" 0.15625;
	setAttr ".pv" 0.15625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve32" -p "pSphere1Follicle1515";
	rename -uid "A40735EC-49E9-7B4E-A485-76B5E5958C63";
createNode nurbsCurve -n "curveShape32" -p "curve32";
	rename -uid "5AA38041-4EA4-60FB-D845-A4B45E357D53";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
		0 0 0.66666666669999997
		0 0 1
		0 0 1.3333333329999999
		0 0 1.6666666670000001
		0 0 2
		0 0 2.3333333330000001
		0 0 2.6666666669999999
		0 0 3
		;
createNode transform -n "pSphere1Follicle1522" -p "hairSystem1Follicles";
	rename -uid "01C410AE-42A9-D532-4BAE-95B2F15C952C";
createNode follicle -n "pSphere1FollicleShape1522" -p "pSphere1Follicle1522";
	rename -uid "B2725AA7-4CF5-3389-D7E1-ADB7E2F77F59";
	setAttr -k off ".v";
	setAttr ".pu" 0.15625;
	setAttr ".pv" 0.21875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve33" -p "pSphere1Follicle1522";
	rename -uid "C61D4853-4F91-11A3-9E47-4097E3DDD88A";
createNode nurbsCurve -n "curveShape33" -p "curve33";
	rename -uid "F868F4C7-4A0C-B63B-B8DD-AC89F9392BC3";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
		0 0 0.66666666669999997
		0 0 1
		0 0 1.3333333329999999
		0 0 1.6666666670000001
		0 0 2
		0 0 2.3333333330000001
		0 0 2.6666666669999999
		0 0 3
		;
createNode transform -n "pSphere1Follicle1528" -p "hairSystem1Follicles";
	rename -uid "88D28A90-40AF-1970-B62F-CFB47C74CE24";
createNode follicle -n "pSphere1FollicleShape1528" -p "pSphere1Follicle1528";
	rename -uid "2370F058-4D12-6518-DDD8-248C0B216C0A";
	setAttr -k off ".v";
	setAttr ".pu" 0.15625;
	setAttr ".pv" 0.28125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve34" -p "pSphere1Follicle1528";
	rename -uid "F647CA01-429D-43E8-CCEC-5E8AFE4C5832";
createNode nurbsCurve -n "curveShape34" -p "curve34";
	rename -uid "92884FED-4D5E-FDEC-0243-48A0AEE2EB54";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
		0 0 0.66666666669999997
		0 0 1
		0 0 1.3333333329999999
		0 0 1.6666666670000001
		0 0 2
		0 0 2.3333333330000001
		0 0 2.6666666669999999
		0 0 3
		;
createNode transform -n "pSphere1Follicle1534" -p "hairSystem1Follicles";
	rename -uid "7558B760-4DDF-6AB1-E0E8-F4994B309B30";
createNode follicle -n "pSphere1FollicleShape1534" -p "pSphere1Follicle1534";
	rename -uid "5FD4B742-40C8-DD55-B985-E6889361632E";
	setAttr -k off ".v";
	setAttr ".pu" 0.15625;
	setAttr ".pv" 0.34375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve35" -p "pSphere1Follicle1534";
	rename -uid "65B6CB0A-4AC4-554C-1F52-54827E0B8406";
createNode nurbsCurve -n "curveShape35" -p "curve35";
	rename -uid "1091CF21-42FD-76B2-E39F-089FD21D137A";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
		0 0 0.66666666669999997
		0 0 1
		0 0 1.3333333329999999
		0 0 1.6666666670000001
		0 0 2
		0 0 2.3333333330000001
		0 0 2.6666666669999999
		0 0 3
		;
createNode transform -n "pSphere1Follicle1540" -p "hairSystem1Follicles";
	rename -uid "E9986174-43B2-9030-B946-43AC423FDF83";
createNode follicle -n "pSphere1FollicleShape1540" -p "pSphere1Follicle1540";
	rename -uid "DC36AC7D-44E2-EF86-C676-9D8AA07DBB45";
	setAttr -k off ".v";
	setAttr ".pu" 0.15625;
	setAttr ".pv" 0.40625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve36" -p "pSphere1Follicle1540";
	rename -uid "A5D89E2D-42EA-C03D-3F6A-C7A17321DEB4";
createNode nurbsCurve -n "curveShape36" -p "curve36";
	rename -uid "D88D273A-4562-D31A-242D-3392DE49B7CD";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
		0 0 0.66666666669999997
		0 0 1
		0 0 1.3333333329999999
		0 0 1.6666666670000001
		0 0 2
		0 0 2.3333333330000001
		0 0 2.6666666669999999
		0 0 3
		;
createNode transform -n "pSphere1Follicle1546" -p "hairSystem1Follicles";
	rename -uid "41895639-46DF-23B3-DC39-529F234D3F7C";
createNode follicle -n "pSphere1FollicleShape1546" -p "pSphere1Follicle1546";
	rename -uid "16E4FB2A-4F2B-85FE-A06B-6CB4D9C3BF31";
	setAttr -k off ".v";
	setAttr ".pu" 0.15625;
	setAttr ".pv" 0.46875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve37" -p "pSphere1Follicle1546";
	rename -uid "93C1D431-4FBE-6DB1-2C6D-D88649B73D9B";
createNode nurbsCurve -n "curveShape37" -p "curve37";
	rename -uid "F48C22FE-43A7-340B-9BBE-68AB6D55B837";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
		0 0 0.66666666669999997
		0 0 1
		0 0 1.3333333329999999
		0 0 1.6666666670000001
		0 0 2
		0 0 2.3333333330000001
		0 0 2.6666666669999999
		0 0 3
		;
createNode transform -n "pSphere1Follicle1553" -p "hairSystem1Follicles";
	rename -uid "0E1D46C3-4200-05F7-C847-5E9F399915D4";
createNode follicle -n "pSphere1FollicleShape1553" -p "pSphere1Follicle1553";
	rename -uid "8210AD22-47C6-9F7D-79F5-508F4618ABF3";
	setAttr -k off ".v";
	setAttr ".pu" 0.15625;
	setAttr ".pv" 0.53125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve38" -p "pSphere1Follicle1553";
	rename -uid "0649FE8E-4890-CCE3-7D22-7785ECF0F2DB";
createNode nurbsCurve -n "curveShape38" -p "curve38";
	rename -uid "5B2AAA4D-4F2D-D5A5-1A90-CA9C5C5C09D7";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
		0 0 0.66666666669999997
		0 0 1
		0 0 1.3333333329999999
		0 0 1.6666666670000001
		0 0 2
		0 0 2.3333333330000001
		0 0 2.6666666669999999
		0 0 3
		;
createNode transform -n "pSphere1Follicle1559" -p "hairSystem1Follicles";
	rename -uid "74157D08-4AA4-930D-9EFA-2282BEC36DB1";
createNode follicle -n "pSphere1FollicleShape1559" -p "pSphere1Follicle1559";
	rename -uid "4C131499-42D7-6C25-6BCA-44A633F8071E";
	setAttr -k off ".v";
	setAttr ".pu" 0.15625;
	setAttr ".pv" 0.59375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve39" -p "pSphere1Follicle1559";
	rename -uid "878FEC05-4589-D02A-9B85-9DBC7ADF1330";
createNode nurbsCurve -n "curveShape39" -p "curve39";
	rename -uid "65FA478F-48D5-C6E8-B69E-33BFB40F31E1";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
		0 0 0.66666666669999997
		0 0 1
		0 0 1.3333333329999999
		0 0 1.6666666670000001
		0 0 2
		0 0 2.3333333330000001
		0 0 2.6666666669999999
		0 0 3
		;
createNode transform -n "pSphere1Follicle1565" -p "hairSystem1Follicles";
	rename -uid "569E99A1-433F-C512-C722-5B8C1E159014";
createNode follicle -n "pSphere1FollicleShape1565" -p "pSphere1Follicle1565";
	rename -uid "1C4CF840-44FA-49B4-418A-DD866AEEF4CC";
	setAttr -k off ".v";
	setAttr ".pu" 0.15625;
	setAttr ".pv" 0.65625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve40" -p "pSphere1Follicle1565";
	rename -uid "DEC90C17-4E21-798E-F99D-8F91EE666E0C";
createNode nurbsCurve -n "curveShape40" -p "curve40";
	rename -uid "1830D1AC-471A-48AB-4AED-119560A8DEE4";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
		0 0 0.66666666669999997
		0 0 1
		0 0 1.3333333329999999
		0 0 1.6666666670000001
		0 0 2
		0 0 2.3333333330000001
		0 0 2.6666666669999999
		0 0 3
		;
createNode transform -n "pSphere1Follicle1571" -p "hairSystem1Follicles";
	rename -uid "18A2F18F-4D81-E428-00C0-68A3BD9EC237";
createNode follicle -n "pSphere1FollicleShape1571" -p "pSphere1Follicle1571";
	rename -uid "CEE8367C-4422-378F-042B-A48AA4F47B02";
	setAttr -k off ".v";
	setAttr ".pu" 0.15625;
	setAttr ".pv" 0.71875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve41" -p "pSphere1Follicle1571";
	rename -uid "850C205F-42F2-9EEA-7049-8293B0BD3C59";
createNode nurbsCurve -n "curveShape41" -p "curve41";
	rename -uid "A1A0B64C-4388-03C9-C1B2-19834C8D2E64";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
		0 0 0.66666666669999997
		0 0 1
		0 0 1.3333333329999999
		0 0 1.6666666670000001
		0 0 2
		0 0 2.3333333330000001
		0 0 2.6666666669999999
		0 0 3
		;
createNode transform -n "pSphere1Follicle1577" -p "hairSystem1Follicles";
	rename -uid "6AB13EC3-4E46-DC17-8C29-A194664E8C29";
createNode follicle -n "pSphere1FollicleShape1577" -p "pSphere1Follicle1577";
	rename -uid "AC05C004-41E2-19E0-4B27-289E68C0E4DE";
	setAttr -k off ".v";
	setAttr ".pu" 0.15625;
	setAttr ".pv" 0.78125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve42" -p "pSphere1Follicle1577";
	rename -uid "D35E2EBF-4E1E-D5A6-CD73-6489F0906DBD";
createNode nurbsCurve -n "curveShape42" -p "curve42";
	rename -uid "2A7827A8-4D00-D207-DB05-C98D5AD00D48";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
		0 0 0.66666666669999997
		0 0 1
		0 0 1.3333333329999999
		0 0 1.6666666670000001
		0 0 2
		0 0 2.3333333330000001
		0 0 2.6666666669999999
		0 0 3
		;
createNode transform -n "pSphere1Follicle1584" -p "hairSystem1Follicles";
	rename -uid "61A98DE0-422A-3252-0769-2DAD6239BF6B";
createNode follicle -n "pSphere1FollicleShape1584" -p "pSphere1Follicle1584";
	rename -uid "0C5EFDEA-4528-93C1-71DF-86B6D15C65AD";
	setAttr -k off ".v";
	setAttr ".pu" 0.15625;
	setAttr ".pv" 0.84375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve43" -p "pSphere1Follicle1584";
	rename -uid "F394D9DC-488B-5737-B89F-A983869E27E9";
createNode nurbsCurve -n "curveShape43" -p "curve43";
	rename -uid "172CFE93-4AE0-D846-D204-ECA670BF1AAE";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
		0 0 0.66666666669999997
		0 0 1
		0 0 1.3333333329999999
		0 0 1.6666666670000001
		0 0 2
		0 0 2.3333333330000001
		0 0 2.6666666669999999
		0 0 3
		;
createNode transform -n "pSphere1Follicle1590" -p "hairSystem1Follicles";
	rename -uid "7368BFF2-4EA5-D2AD-ECEF-B49428C619D9";
createNode follicle -n "pSphere1FollicleShape1590" -p "pSphere1Follicle1590";
	rename -uid "93BD74F3-4860-8414-40AD-1BAEF5307D15";
	setAttr -k off ".v";
	setAttr ".pu" 0.15625;
	setAttr ".pv" 0.90625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve44" -p "pSphere1Follicle1590";
	rename -uid "8326D571-4AA3-63E8-5832-8D8D70DDA7E6";
createNode nurbsCurve -n "curveShape44" -p "curve44";
	rename -uid "58800A77-4015-311A-4264-EE907636F7B4";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
		0 0 0.66666666669999997
		0 0 1
		0 0 1.3333333329999999
		0 0 1.6666666670000001
		0 0 2
		0 0 2.3333333330000001
		0 0 2.6666666669999999
		0 0 3
		;
createNode transform -n "pSphere1Follicle2203" -p "hairSystem1Follicles";
	rename -uid "9B896AD4-4F4F-B807-D11E-E99F973478DE";
createNode follicle -n "pSphere1FollicleShape2203" -p "pSphere1Follicle2203";
	rename -uid "30042106-42C2-8CA8-A43B-06897BD79CE5";
	setAttr -k off ".v";
	setAttr ".pu" 0.21875;
	setAttr ".pv" 0.03125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve45" -p "pSphere1Follicle2203";
	rename -uid "6F8AF710-4D8D-A324-0CE6-5494BE74E390";
createNode nurbsCurve -n "curveShape45" -p "curve45";
	rename -uid "54F4CF4E-4D07-8512-A590-BFAC7A9F5712";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
		0 0 0.66666666669999997
		0 0 1
		0 0 1.3333333329999999
		0 0 1.6666666670000001
		0 0 2
		0 0 2.3333333330000001
		0 0 2.6666666669999999
		0 0 3
		;
createNode transform -n "pSphere1Follicle2209" -p "hairSystem1Follicles";
	rename -uid "80368FA2-40CE-E457-FB1A-2EBEE8DA82CB";
createNode follicle -n "pSphere1FollicleShape2209" -p "pSphere1Follicle2209";
	rename -uid "7331B129-43C5-D054-3525-CE8A7645F1FB";
	setAttr -k off ".v";
	setAttr ".pu" 0.21875;
	setAttr ".pv" 0.09375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve46" -p "pSphere1Follicle2209";
	rename -uid "FAF837F7-4FB7-5FD2-D5F1-C5A834E34F48";
createNode nurbsCurve -n "curveShape46" -p "curve46";
	rename -uid "3AB44708-4C6B-C795-1B7C-77AE59E59A1F";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
		0 0 0.66666666669999997
		0 0 1
		0 0 1.3333333329999999
		0 0 1.6666666670000001
		0 0 2
		0 0 2.3333333330000001
		0 0 2.6666666669999999
		0 0 3
		;
createNode transform -n "pSphere1Follicle2215" -p "hairSystem1Follicles";
	rename -uid "C832B980-4FAF-91F1-4388-65BAC6CEFB46";
createNode follicle -n "pSphere1FollicleShape2215" -p "pSphere1Follicle2215";
	rename -uid "141693C1-429B-B8B0-2AB6-7DA499D47DC4";
	setAttr -k off ".v";
	setAttr ".pu" 0.21875;
	setAttr ".pv" 0.15625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve47" -p "pSphere1Follicle2215";
	rename -uid "9C65E9CA-4568-2FF3-901A-D288D7BCFF34";
createNode nurbsCurve -n "curveShape47" -p "curve47";
	rename -uid "79B617D1-439A-7A01-50E9-04B09D3C0410";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
		0 0 0.66666666669999997
		0 0 1
		0 0 1.3333333329999999
		0 0 1.6666666670000001
		0 0 2
		0 0 2.3333333330000001
		0 0 2.6666666669999999
		0 0 3
		;
createNode transform -n "pSphere1Follicle2222" -p "hairSystem1Follicles";
	rename -uid "B971F747-469C-28DE-0A4F-82B4A6FE8000";
createNode follicle -n "pSphere1FollicleShape2222" -p "pSphere1Follicle2222";
	rename -uid "D89AC78C-4CF7-5181-699D-98A0ABC1F4DF";
	setAttr -k off ".v";
	setAttr ".pu" 0.21875;
	setAttr ".pv" 0.21875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve48" -p "pSphere1Follicle2222";
	rename -uid "E78DB7CB-4780-D0FE-B973-C3BD2061A3F0";
createNode nurbsCurve -n "curveShape48" -p "curve48";
	rename -uid "A59232E5-4CE9-D9BC-34DB-F7A9C3A7561F";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
		0 0 0.66666666669999997
		0 0 1
		0 0 1.3333333329999999
		0 0 1.6666666670000001
		0 0 2
		0 0 2.3333333330000001
		0 0 2.6666666669999999
		0 0 3
		;
createNode transform -n "pSphere1Follicle2228" -p "hairSystem1Follicles";
	rename -uid "75BFE6AA-4CBB-3651-62FD-A9A15131B4CC";
createNode follicle -n "pSphere1FollicleShape2228" -p "pSphere1Follicle2228";
	rename -uid "4AF40596-4D9A-32B0-DD4F-7D919605AC13";
	setAttr -k off ".v";
	setAttr ".pu" 0.21875;
	setAttr ".pv" 0.28125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve49" -p "pSphere1Follicle2228";
	rename -uid "07D2D19B-47D0-42C2-96B2-B9B66D22C540";
createNode nurbsCurve -n "curveShape49" -p "curve49";
	rename -uid "0CE1257E-4487-769C-1FCD-9CBB77845E6E";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
		0 0 0.66666666669999997
		0 0 1
		0 0 1.3333333329999999
		0 0 1.6666666670000001
		0 0 2
		0 0 2.3333333330000001
		0 0 2.6666666669999999
		0 0 3
		;
createNode transform -n "pSphere1Follicle2234" -p "hairSystem1Follicles";
	rename -uid "AA875199-489D-DBFF-1098-2F873034283A";
createNode follicle -n "pSphere1FollicleShape2234" -p "pSphere1Follicle2234";
	rename -uid "4521AC23-4739-59D8-86DB-F7BFD23E4C96";
	setAttr -k off ".v";
	setAttr ".pu" 0.21875;
	setAttr ".pv" 0.34375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve50" -p "pSphere1Follicle2234";
	rename -uid "4A50CB30-445E-A8F0-0A05-158DF42886C8";
createNode nurbsCurve -n "curveShape50" -p "curve50";
	rename -uid "91DDFE3E-4DEA-C50E-56C3-29B54C907AF1";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
		0 0 0.66666666669999997
		0 0 1
		0 0 1.3333333329999999
		0 0 1.6666666670000001
		0 0 2
		0 0 2.3333333330000001
		0 0 2.6666666669999999
		0 0 3
		;
createNode transform -n "pSphere1Follicle2240" -p "hairSystem1Follicles";
	rename -uid "28B8370E-48BC-6C1C-1AC3-A784958CC761";
createNode follicle -n "pSphere1FollicleShape2240" -p "pSphere1Follicle2240";
	rename -uid "0A943804-485D-C291-3471-8A82D47E8D07";
	setAttr -k off ".v";
	setAttr ".pu" 0.21875;
	setAttr ".pv" 0.40625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve51" -p "pSphere1Follicle2240";
	rename -uid "FAA66C56-479D-22F6-C441-20B8C54C889D";
createNode nurbsCurve -n "curveShape51" -p "curve51";
	rename -uid "F0EBF54D-46C6-F9CB-4B77-3A9DF3D397F6";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
		0 0 0.66666666669999997
		0 0 1
		0 0 1.3333333329999999
		0 0 1.6666666670000001
		0 0 2
		0 0 2.3333333330000001
		0 0 2.6666666669999999
		0 0 3
		;
createNode transform -n "pSphere1Follicle2246" -p "hairSystem1Follicles";
	rename -uid "914BBF5A-43E4-8CB3-F275-28A9C13B3B07";
createNode follicle -n "pSphere1FollicleShape2246" -p "pSphere1Follicle2246";
	rename -uid "2CAA74E9-44ED-051E-21E2-76984BB3C5AD";
	setAttr -k off ".v";
	setAttr ".pu" 0.21875;
	setAttr ".pv" 0.46875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve52" -p "pSphere1Follicle2246";
	rename -uid "C560A795-4BD4-E753-7086-24A84E771930";
createNode nurbsCurve -n "curveShape52" -p "curve52";
	rename -uid "7B853190-4044-C374-502C-EEB8B1D44E27";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
		0 0 0.66666666669999997
		0 0 1
		0 0 1.3333333329999999
		0 0 1.6666666670000001
		0 0 2
		0 0 2.3333333330000001
		0 0 2.6666666669999999
		0 0 3
		;
createNode transform -n "pSphere1Follicle2253" -p "hairSystem1Follicles";
	rename -uid "9C871989-48AB-44FC-2D62-42B68B18D9C2";
createNode follicle -n "pSphere1FollicleShape2253" -p "pSphere1Follicle2253";
	rename -uid "A0C8EF05-4C77-5F7C-748F-BB8C402C1330";
	setAttr -k off ".v";
	setAttr ".pu" 0.21875;
	setAttr ".pv" 0.53125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve53" -p "pSphere1Follicle2253";
	rename -uid "D4BC341F-414C-8E06-365B-739AD3BE5B94";
createNode nurbsCurve -n "curveShape53" -p "curve53";
	rename -uid "BBDC8159-433F-56E7-68D7-6A86A6985129";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
		0 0 0.66666666669999997
		0 0 1
		0 0 1.3333333329999999
		0 0 1.6666666670000001
		0 0 2
		0 0 2.3333333330000001
		0 0 2.6666666669999999
		0 0 3
		;
createNode transform -n "pSphere1Follicle2259" -p "hairSystem1Follicles";
	rename -uid "DB9B22CC-428B-1C76-81D5-10A79DEC00B3";
createNode follicle -n "pSphere1FollicleShape2259" -p "pSphere1Follicle2259";
	rename -uid "1B48DE99-4724-DD5D-2F50-B0B2F3200733";
	setAttr -k off ".v";
	setAttr ".pu" 0.21875;
	setAttr ".pv" 0.59375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve54" -p "pSphere1Follicle2259";
	rename -uid "E4EFB6AE-4EE8-4022-5A1D-9E976CC0AE44";
createNode nurbsCurve -n "curveShape54" -p "curve54";
	rename -uid "92AC101C-4E69-ABAB-5303-15B5DA671992";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
		0 0 0.66666666669999997
		0 0 1
		0 0 1.3333333329999999
		0 0 1.6666666670000001
		0 0 2
		0 0 2.3333333330000001
		0 0 2.6666666669999999
		0 0 3
		;
createNode transform -n "pSphere1Follicle2265" -p "hairSystem1Follicles";
	rename -uid "145101D3-4D70-081A-8764-088204738A52";
createNode follicle -n "pSphere1FollicleShape2265" -p "pSphere1Follicle2265";
	rename -uid "19336C2B-49B8-A4C4-4A7B-9987602F739B";
	setAttr -k off ".v";
	setAttr ".pu" 0.21875;
	setAttr ".pv" 0.65625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve55" -p "pSphere1Follicle2265";
	rename -uid "D4D98F29-4E98-2A36-4EFD-AFAC2A401064";
createNode nurbsCurve -n "curveShape55" -p "curve55";
	rename -uid "42AF1560-4B07-D719-1D87-06AFD3FC70B0";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
		0 0 0.66666666669999997
		0 0 1
		0 0 1.3333333329999999
		0 0 1.6666666670000001
		0 0 2
		0 0 2.3333333330000001
		0 0 2.6666666669999999
		0 0 3
		;
createNode transform -n "pSphere1Follicle2271" -p "hairSystem1Follicles";
	rename -uid "77AB230F-4B9C-D910-C23C-6FA58A0AC65B";
createNode follicle -n "pSphere1FollicleShape2271" -p "pSphere1Follicle2271";
	rename -uid "069B69D5-43FB-30B2-70F7-FBAACEF72671";
	setAttr -k off ".v";
	setAttr ".pu" 0.21875;
	setAttr ".pv" 0.71875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve56" -p "pSphere1Follicle2271";
	rename -uid "C682591D-42E3-7842-79BF-BD84913861BB";
createNode nurbsCurve -n "curveShape56" -p "curve56";
	rename -uid "68DD354D-4280-B866-84F4-8C8B34E78291";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
		0 0 0.66666666669999997
		0 0 1
		0 0 1.3333333329999999
		0 0 1.6666666670000001
		0 0 2
		0 0 2.3333333330000001
		0 0 2.6666666669999999
		0 0 3
		;
createNode transform -n "pSphere1Follicle2277" -p "hairSystem1Follicles";
	rename -uid "CBCE97D0-4943-AD79-B1E5-E6AD23E82E08";
createNode follicle -n "pSphere1FollicleShape2277" -p "pSphere1Follicle2277";
	rename -uid "00DBF879-4AD2-D4A9-E937-E2B00C824485";
	setAttr -k off ".v";
	setAttr ".pu" 0.21875;
	setAttr ".pv" 0.78125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve57" -p "pSphere1Follicle2277";
	rename -uid "54074BB1-4B5A-F8ED-1E6F-FDB589AD76D8";
createNode nurbsCurve -n "curveShape57" -p "curve57";
	rename -uid "60748122-45FD-115C-75CF-2FBC6401BDDC";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
		0 0 0.66666666669999997
		0 0 1
		0 0 1.3333333329999999
		0 0 1.6666666670000001
		0 0 2
		0 0 2.3333333330000001
		0 0 2.6666666669999999
		0 0 3
		;
createNode transform -n "pSphere1Follicle2284" -p "hairSystem1Follicles";
	rename -uid "FE399487-49AB-710C-8DE4-8995B90EA6E3";
createNode follicle -n "pSphere1FollicleShape2284" -p "pSphere1Follicle2284";
	rename -uid "AB0078EE-4733-5291-D38E-94BAEEF75AE6";
	setAttr -k off ".v";
	setAttr ".pu" 0.21875;
	setAttr ".pv" 0.84375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve58" -p "pSphere1Follicle2284";
	rename -uid "7E1D8B1C-4336-6095-D8C1-818BAF9EA5E3";
createNode nurbsCurve -n "curveShape58" -p "curve58";
	rename -uid "9C1A7E29-493D-E33E-036D-09B32CC0AC33";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
		0 0 0.66666666669999997
		0 0 1
		0 0 1.3333333329999999
		0 0 1.6666666670000001
		0 0 2
		0 0 2.3333333330000001
		0 0 2.6666666669999999
		0 0 3
		;
createNode transform -n "pSphere1Follicle2290" -p "hairSystem1Follicles";
	rename -uid "4ADB3A8E-46ED-2347-9A4F-85A0694D84AD";
createNode follicle -n "pSphere1FollicleShape2290" -p "pSphere1Follicle2290";
	rename -uid "4B1265F0-49F5-6190-A989-E892B4176D15";
	setAttr -k off ".v";
	setAttr ".pu" 0.21875;
	setAttr ".pv" 0.90625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve59" -p "pSphere1Follicle2290";
	rename -uid "BDB73F23-435E-49F2-B9C9-D19DCFF53CE2";
createNode nurbsCurve -n "curveShape59" -p "curve59";
	rename -uid "50BA9E79-491D-D727-1D8A-239D722F4399";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
		0 0 0.66666666669999997
		0 0 1
		0 0 1.3333333329999999
		0 0 1.6666666670000001
		0 0 2
		0 0 2.3333333330000001
		0 0 2.6666666669999999
		0 0 3
		;
createNode transform -n "pSphere1Follicle2296" -p "hairSystem1Follicles";
	rename -uid "AC455AC9-4DD0-7486-8986-C287EE304B6A";
createNode follicle -n "pSphere1FollicleShape2296" -p "pSphere1Follicle2296";
	rename -uid "C41A7A78-4FFF-1F8B-B55B-DF83B87BE4C0";
	setAttr -k off ".v";
	setAttr ".pu" 0.21875;
	setAttr ".pv" 0.96875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve60" -p "pSphere1Follicle2296";
	rename -uid "71D82394-4F94-DAD1-2849-FD8C8C9E7992";
createNode nurbsCurve -n "curveShape60" -p "curve60";
	rename -uid "861E2728-40D9-DEC0-C114-9EBC65EFA526";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
		0 0 0.66666666669999997
		0 0 1
		0 0 1.3333333329999999
		0 0 1.6666666670000001
		0 0 2
		0 0 2.3333333330000001
		0 0 2.6666666669999999
		0 0 3
		;
createNode transform -n "pSphere1Follicle2803" -p "hairSystem1Follicles";
	rename -uid "BBA1B99F-4B4A-AF21-D652-419215BE25C7";
createNode follicle -n "pSphere1FollicleShape2803" -p "pSphere1Follicle2803";
	rename -uid "78747287-49A0-E4AC-C516-E7A9BB82D83D";
	setAttr -k off ".v";
	setAttr ".pu" 0.28125;
	setAttr ".pv" 0.03125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve61" -p "pSphere1Follicle2803";
	rename -uid "8EED7874-42D8-2612-AB1A-5DB8AAED750C";
createNode nurbsCurve -n "curveShape61" -p "curve61";
	rename -uid "FF150532-49DE-078B-A7DD-1CA8F45BA3CB";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
		0 0 0.66666666669999997
		0 0 1
		0 0 1.3333333329999999
		0 0 1.6666666670000001
		0 0 2
		0 0 2.3333333330000001
		0 0 2.6666666669999999
		0 0 3
		;
createNode transform -n "pSphere1Follicle2809" -p "hairSystem1Follicles";
	rename -uid "603DE590-44C8-AE0E-4660-7281A1F14286";
createNode follicle -n "pSphere1FollicleShape2809" -p "pSphere1Follicle2809";
	rename -uid "C3C599FE-46CF-3CA6-1859-52AE37A53ED9";
	setAttr -k off ".v";
	setAttr ".pu" 0.28125;
	setAttr ".pv" 0.09375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve62" -p "pSphere1Follicle2809";
	rename -uid "55A14183-4F1E-D6E7-370F-1A9ECA223DDF";
createNode nurbsCurve -n "curveShape62" -p "curve62";
	rename -uid "7368F996-4658-FB46-37A9-3394FEBFD1D6";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
		0 0 0.66666666669999997
		0 0 1
		0 0 1.3333333329999999
		0 0 1.6666666670000001
		0 0 2
		0 0 2.3333333330000001
		0 0 2.6666666669999999
		0 0 3
		;
createNode transform -n "pSphere1Follicle2815" -p "hairSystem1Follicles";
	rename -uid "C5D86D36-44DF-79F7-62A8-959BB9144129";
createNode follicle -n "pSphere1FollicleShape2815" -p "pSphere1Follicle2815";
	rename -uid "DDC3EDB5-4022-2811-8C93-07A1050F4CD1";
	setAttr -k off ".v";
	setAttr ".pu" 0.28125;
	setAttr ".pv" 0.15625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve63" -p "pSphere1Follicle2815";
	rename -uid "A7D09B67-4CE8-186E-4DC8-94BC635962E6";
createNode nurbsCurve -n "curveShape63" -p "curve63";
	rename -uid "8FA74FBE-4233-DC64-DB7D-9DB9AFE034D6";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
		0 0 0.66666666669999997
		0 0 1
		0 0 1.3333333329999999
		0 0 1.6666666670000001
		0 0 2
		0 0 2.3333333330000001
		0 0 2.6666666669999999
		0 0 3
		;
createNode transform -n "pSphere1Follicle2822" -p "hairSystem1Follicles";
	rename -uid "EE918A86-49C5-68DE-0CB5-6CBCCFEBF65A";
createNode follicle -n "pSphere1FollicleShape2822" -p "pSphere1Follicle2822";
	rename -uid "A22F30D2-4DBE-D675-6653-7D98638D7910";
	setAttr -k off ".v";
	setAttr ".pu" 0.28125;
	setAttr ".pv" 0.21875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve64" -p "pSphere1Follicle2822";
	rename -uid "9AF36007-4E4B-F634-49AE-8394F0AF1C53";
createNode nurbsCurve -n "curveShape64" -p "curve64";
	rename -uid "3BDABD7C-4A7C-1FA7-26A0-12A842FEC935";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
		0 0 0.66666666669999997
		0 0 1
		0 0 1.3333333329999999
		0 0 1.6666666670000001
		0 0 2
		0 0 2.3333333330000001
		0 0 2.6666666669999999
		0 0 3
		;
createNode transform -n "pSphere1Follicle2828" -p "hairSystem1Follicles";
	rename -uid "71CFC0A8-4852-0149-F7BC-B1A77CE1F8E2";
createNode follicle -n "pSphere1FollicleShape2828" -p "pSphere1Follicle2828";
	rename -uid "05E42983-4D1E-099A-DF5A-BF9B719066A4";
	setAttr -k off ".v";
	setAttr ".pu" 0.28125;
	setAttr ".pv" 0.28125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve65" -p "pSphere1Follicle2828";
	rename -uid "17AC49B8-400C-A6F0-FAAA-1A98C44A4E3C";
createNode nurbsCurve -n "curveShape65" -p "curve65";
	rename -uid "988DD93D-467B-6461-CE31-919DD65F672C";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
		0 0 0.66666666669999997
		0 0 1
		0 0 1.3333333329999999
		0 0 1.6666666670000001
		0 0 2
		0 0 2.3333333330000001
		0 0 2.6666666669999999
		0 0 3
		;
createNode transform -n "pSphere1Follicle2834" -p "hairSystem1Follicles";
	rename -uid "A6AC0FA7-4D2D-8256-EB2B-69BE925CB826";
createNode follicle -n "pSphere1FollicleShape2834" -p "pSphere1Follicle2834";
	rename -uid "9664DFDB-4D4F-9E1B-45C1-8FB92A95A695";
	setAttr -k off ".v";
	setAttr ".pu" 0.28125;
	setAttr ".pv" 0.34375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve66" -p "pSphere1Follicle2834";
	rename -uid "2D2AA3F7-4D70-12E4-3B89-9588878853E2";
createNode nurbsCurve -n "curveShape66" -p "curve66";
	rename -uid "42D4FE06-46A4-EC6C-C3BD-B09C617FC7BE";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
		0 0 0.66666666669999997
		0 0 1
		0 0 1.3333333329999999
		0 0 1.6666666670000001
		0 0 2
		0 0 2.3333333330000001
		0 0 2.6666666669999999
		0 0 3
		;
createNode transform -n "pSphere1Follicle2840" -p "hairSystem1Follicles";
	rename -uid "81A2CCD1-4002-91CD-30AE-D4A756A242BE";
createNode follicle -n "pSphere1FollicleShape2840" -p "pSphere1Follicle2840";
	rename -uid "FEC3C877-4F23-8687-EA88-D899107076B8";
	setAttr -k off ".v";
	setAttr ".pu" 0.28125;
	setAttr ".pv" 0.40625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve67" -p "pSphere1Follicle2840";
	rename -uid "6E77F141-43FF-C883-AF64-32BFD90F54E6";
createNode nurbsCurve -n "curveShape67" -p "curve67";
	rename -uid "A1F58CA5-4E28-3DB8-487D-7F8D4E1C65C1";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
		0 0 0.66666666669999997
		0 0 1
		0 0 1.3333333329999999
		0 0 1.6666666670000001
		0 0 2
		0 0 2.3333333330000001
		0 0 2.6666666669999999
		0 0 3
		;
createNode transform -n "pSphere1Follicle2846" -p "hairSystem1Follicles";
	rename -uid "02C223D7-4200-125A-B1E4-FB9CC8F442F1";
createNode follicle -n "pSphere1FollicleShape2846" -p "pSphere1Follicle2846";
	rename -uid "9643BB44-4EE9-D94A-B103-A3837C5D47FD";
	setAttr -k off ".v";
	setAttr ".pu" 0.28125;
	setAttr ".pv" 0.46875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve68" -p "pSphere1Follicle2846";
	rename -uid "404E61EB-4EEE-3F7A-31A1-2F91A6D03D57";
createNode nurbsCurve -n "curveShape68" -p "curve68";
	rename -uid "5970C73E-43CF-ECFD-1C72-DE9DB7C28820";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
		0 0 0.66666666669999997
		0 0 1
		0 0 1.3333333329999999
		0 0 1.6666666670000001
		0 0 2
		0 0 2.3333333330000001
		0 0 2.6666666669999999
		0 0 3
		;
createNode transform -n "pSphere1Follicle2853" -p "hairSystem1Follicles";
	rename -uid "A9CB77FF-4A25-B566-6CFB-35AD5D81EE7B";
createNode follicle -n "pSphere1FollicleShape2853" -p "pSphere1Follicle2853";
	rename -uid "BED87979-45CA-A1DF-9527-6C83FF8BFA6B";
	setAttr -k off ".v";
	setAttr ".pu" 0.28125;
	setAttr ".pv" 0.53125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve69" -p "pSphere1Follicle2853";
	rename -uid "3399D921-43B4-16E9-1805-63B479648F27";
createNode nurbsCurve -n "curveShape69" -p "curve69";
	rename -uid "267CC739-41B3-8D95-056D-A19471E9FEA9";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
		0 0 0.66666666669999997
		0 0 1
		0 0 1.3333333329999999
		0 0 1.6666666670000001
		0 0 2
		0 0 2.3333333330000001
		0 0 2.6666666669999999
		0 0 3
		;
createNode transform -n "pSphere1Follicle2859" -p "hairSystem1Follicles";
	rename -uid "8ED0F1F0-4B62-CF75-944D-51B64FD2DA73";
createNode follicle -n "pSphere1FollicleShape2859" -p "pSphere1Follicle2859";
	rename -uid "5C0C27C1-4ADB-4F52-A189-81864DB9F472";
	setAttr -k off ".v";
	setAttr ".pu" 0.28125;
	setAttr ".pv" 0.59375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve70" -p "pSphere1Follicle2859";
	rename -uid "7BA45C61-483D-2EF8-A1BA-889DF8D2808B";
createNode nurbsCurve -n "curveShape70" -p "curve70";
	rename -uid "2B56D165-43AA-767A-9576-F795A0BE0983";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
		0 0 0.66666666669999997
		0 0 1
		0 0 1.3333333329999999
		0 0 1.6666666670000001
		0 0 2
		0 0 2.3333333330000001
		0 0 2.6666666669999999
		0 0 3
		;
createNode transform -n "pSphere1Follicle2865" -p "hairSystem1Follicles";
	rename -uid "C1CCE143-4694-E407-FA31-9A98131881BB";
createNode follicle -n "pSphere1FollicleShape2865" -p "pSphere1Follicle2865";
	rename -uid "D00AC6F7-438D-41E9-0307-ABB0497AE13A";
	setAttr -k off ".v";
	setAttr ".pu" 0.28125;
	setAttr ".pv" 0.65625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve71" -p "pSphere1Follicle2865";
	rename -uid "3C8E95AB-4E09-DA99-1B4B-A1BE034C589F";
createNode nurbsCurve -n "curveShape71" -p "curve71";
	rename -uid "B13CA33D-4883-D250-CCE1-8A802420AA24";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
		0 0 0.66666666669999997
		0 0 1
		0 0 1.3333333329999999
		0 0 1.6666666670000001
		0 0 2
		0 0 2.3333333330000001
		0 0 2.6666666669999999
		0 0 3
		;
createNode transform -n "pSphere1Follicle2871" -p "hairSystem1Follicles";
	rename -uid "DEAC329B-4400-0D6D-07C1-DCB09A5230C0";
createNode follicle -n "pSphere1FollicleShape2871" -p "pSphere1Follicle2871";
	rename -uid "72097007-4F73-5AA3-4360-F8BF0602AF78";
	setAttr -k off ".v";
	setAttr ".pu" 0.28125;
	setAttr ".pv" 0.71875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve72" -p "pSphere1Follicle2871";
	rename -uid "F9A7AB1A-44BA-126B-AD16-1F868E977C2E";
createNode nurbsCurve -n "curveShape72" -p "curve72";
	rename -uid "536019CC-4B31-1CF2-F72A-BFB72628E4DA";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
		0 0 0.66666666669999997
		0 0 1
		0 0 1.3333333329999999
		0 0 1.6666666670000001
		0 0 2
		0 0 2.3333333330000001
		0 0 2.6666666669999999
		0 0 3
		;
createNode transform -n "pSphere1Follicle2877" -p "hairSystem1Follicles";
	rename -uid "D95BB52D-4E92-2B02-339E-7DA4E6F3A8C6";
createNode follicle -n "pSphere1FollicleShape2877" -p "pSphere1Follicle2877";
	rename -uid "00D62099-4E7C-5F4D-A2E1-C68E68CEDC25";
	setAttr -k off ".v";
	setAttr ".pu" 0.28125;
	setAttr ".pv" 0.78125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve73" -p "pSphere1Follicle2877";
	rename -uid "19FA6DE5-43A4-EAB5-5622-2D8199525358";
createNode nurbsCurve -n "curveShape73" -p "curve73";
	rename -uid "65665429-45B4-4753-9C9E-AEBE7F0822F7";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
		0 0 0.66666666669999997
		0 0 1
		0 0 1.3333333329999999
		0 0 1.6666666670000001
		0 0 2
		0 0 2.3333333330000001
		0 0 2.6666666669999999
		0 0 3
		;
createNode transform -n "pSphere1Follicle2884" -p "hairSystem1Follicles";
	rename -uid "A3DB4F5A-40EB-22C5-7ECD-30950782F5E9";
createNode follicle -n "pSphere1FollicleShape2884" -p "pSphere1Follicle2884";
	rename -uid "37A278E1-4C34-9952-7E2A-798202DA5C8B";
	setAttr -k off ".v";
	setAttr ".pu" 0.28125;
	setAttr ".pv" 0.84375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve74" -p "pSphere1Follicle2884";
	rename -uid "8BDF8E7D-4705-4EA8-1337-5285ADD3F8DF";
createNode nurbsCurve -n "curveShape74" -p "curve74";
	rename -uid "A818B098-4B00-2ED5-427A-9FA42A3260D5";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
		0 0 0.66666666669999997
		0 0 1
		0 0 1.3333333329999999
		0 0 1.6666666670000001
		0 0 2
		0 0 2.3333333330000001
		0 0 2.6666666669999999
		0 0 3
		;
createNode transform -n "pSphere1Follicle2890" -p "hairSystem1Follicles";
	rename -uid "AD821016-4F0B-8F27-744A-56AF569A132D";
createNode follicle -n "pSphere1FollicleShape2890" -p "pSphere1Follicle2890";
	rename -uid "83F79E5C-4DB0-1A6B-C7F9-77A59A2A8BAF";
	setAttr -k off ".v";
	setAttr ".pu" 0.28125;
	setAttr ".pv" 0.90625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve75" -p "pSphere1Follicle2890";
	rename -uid "AEA1744F-4E41-9978-34B7-9E8B1448A2BC";
createNode nurbsCurve -n "curveShape75" -p "curve75";
	rename -uid "0D48403F-4D97-AD9A-51BF-D9B2F6EF921B";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
		0 0 0.66666666669999997
		0 0 1
		0 0 1.3333333329999999
		0 0 1.6666666670000001
		0 0 2
		0 0 2.3333333330000001
		0 0 2.6666666669999999
		0 0 3
		;
createNode transform -n "pSphere1Follicle2896" -p "hairSystem1Follicles";
	rename -uid "049A17C7-4354-2EE6-3EDF-02A028FE2645";
createNode follicle -n "pSphere1FollicleShape2896" -p "pSphere1Follicle2896";
	rename -uid "D295A6C7-43D7-1F6B-2E79-35AA68D72466";
	setAttr -k off ".v";
	setAttr ".pu" 0.28125;
	setAttr ".pv" 0.96875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve76" -p "pSphere1Follicle2896";
	rename -uid "7868191A-4728-8F65-D32F-0D9451478AA6";
createNode nurbsCurve -n "curveShape76" -p "curve76";
	rename -uid "020DF09E-4158-1FC5-3043-558A33047976";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
		0 0 0.66666666669999997
		0 0 1
		0 0 1.3333333329999999
		0 0 1.6666666670000001
		0 0 2
		0 0 2.3333333330000001
		0 0 2.6666666669999999
		0 0 3
		;
createNode transform -n "pSphere1Follicle3409" -p "hairSystem1Follicles";
	rename -uid "1A2F2735-4BF8-CC79-63F1-86841693D938";
createNode follicle -n "pSphere1FollicleShape3409" -p "pSphere1Follicle3409";
	rename -uid "AFB28CC7-4FFC-E430-2B5C-089E22BFDAD2";
	setAttr -k off ".v";
	setAttr ".pu" 0.34375;
	setAttr ".pv" 0.09375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve77" -p "pSphere1Follicle3409";
	rename -uid "B678A028-47C1-04CD-2CBB-23A557BC885A";
createNode nurbsCurve -n "curveShape77" -p "curve77";
	rename -uid "D06EE7F0-4A99-0DFF-4F97-B68146D40617";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
		0 0 0.66666666669999997
		0 0 1
		0 0 1.3333333329999999
		0 0 1.6666666670000001
		0 0 2
		0 0 2.3333333330000001
		0 0 2.6666666669999999
		0 0 3
		;
createNode transform -n "pSphere1Follicle3415" -p "hairSystem1Follicles";
	rename -uid "C8883827-4245-9640-11AC-FCBCBCFC11F4";
createNode follicle -n "pSphere1FollicleShape3415" -p "pSphere1Follicle3415";
	rename -uid "EC71D527-4803-D6B4-A5A9-AA96C1561D96";
	setAttr -k off ".v";
	setAttr ".pu" 0.34375;
	setAttr ".pv" 0.15625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve78" -p "pSphere1Follicle3415";
	rename -uid "B9DA728F-494C-7B24-1FDA-8FAE3798F6C5";
createNode nurbsCurve -n "curveShape78" -p "curve78";
	rename -uid "D29EB2B0-4594-DB2B-3D3C-E680CF760D03";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
		0 0 0.66666666669999997
		0 0 1
		0 0 1.3333333329999999
		0 0 1.6666666670000001
		0 0 2
		0 0 2.3333333330000001
		0 0 2.6666666669999999
		0 0 3
		;
createNode transform -n "pSphere1Follicle3422" -p "hairSystem1Follicles";
	rename -uid "11D7E796-4ED3-2928-6097-CDA6A4439A2E";
createNode follicle -n "pSphere1FollicleShape3422" -p "pSphere1Follicle3422";
	rename -uid "EA2EDC3D-4254-0AAA-04FA-1A8ED15D2575";
	setAttr -k off ".v";
	setAttr ".pu" 0.34375;
	setAttr ".pv" 0.21875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve79" -p "pSphere1Follicle3422";
	rename -uid "B6F20B14-4D92-A66A-11EC-C6BF98A5C84D";
createNode nurbsCurve -n "curveShape79" -p "curve79";
	rename -uid "F9E9928D-454B-34FB-BC1E-F3B54ECDFF47";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
		0 0 0.66666666669999997
		0 0 1
		0 0 1.3333333329999999
		0 0 1.6666666670000001
		0 0 2
		0 0 2.3333333330000001
		0 0 2.6666666669999999
		0 0 3
		;
createNode transform -n "pSphere1Follicle3428" -p "hairSystem1Follicles";
	rename -uid "3ADCF020-4694-7E6E-1959-EEBF1D38A4B7";
createNode follicle -n "pSphere1FollicleShape3428" -p "pSphere1Follicle3428";
	rename -uid "AD3B51CA-4361-24BD-E93E-5AACA12867B6";
	setAttr -k off ".v";
	setAttr ".pu" 0.34375;
	setAttr ".pv" 0.28125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve80" -p "pSphere1Follicle3428";
	rename -uid "A7AA2584-4ED5-DF79-E731-C9BB2692F99A";
createNode nurbsCurve -n "curveShape80" -p "curve80";
	rename -uid "3E3147C2-4873-A5E7-6CB4-01914F1DE21F";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
		0 0 0.66666666669999997
		0 0 1
		0 0 1.3333333329999999
		0 0 1.6666666670000001
		0 0 2
		0 0 2.3333333330000001
		0 0 2.6666666669999999
		0 0 3
		;
createNode transform -n "pSphere1Follicle3434" -p "hairSystem1Follicles";
	rename -uid "B8CD774A-441D-D9FA-1C43-83943362064D";
createNode follicle -n "pSphere1FollicleShape3434" -p "pSphere1Follicle3434";
	rename -uid "B06DC439-4322-ED58-7353-308E05BE1FC7";
	setAttr -k off ".v";
	setAttr ".pu" 0.34375;
	setAttr ".pv" 0.34375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve81" -p "pSphere1Follicle3434";
	rename -uid "1347248C-45CC-2A74-3263-8DBB09BF15EA";
createNode nurbsCurve -n "curveShape81" -p "curve81";
	rename -uid "476B537E-4AB9-B5A0-A669-EBA0F9435A69";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
		0 0 0.66666666669999997
		0 0 1
		0 0 1.3333333329999999
		0 0 1.6666666670000001
		0 0 2
		0 0 2.3333333330000001
		0 0 2.6666666669999999
		0 0 3
		;
createNode transform -n "pSphere1Follicle3440" -p "hairSystem1Follicles";
	rename -uid "06365774-4CF5-84F3-6C18-91B1D3C3E8B6";
createNode follicle -n "pSphere1FollicleShape3440" -p "pSphere1Follicle3440";
	rename -uid "4E07017D-4377-B07A-8BFE-C99E770E89D0";
	setAttr -k off ".v";
	setAttr ".pu" 0.34375;
	setAttr ".pv" 0.40625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve82" -p "pSphere1Follicle3440";
	rename -uid "B1D9294C-47DC-4661-F34F-519F6B2BE236";
createNode nurbsCurve -n "curveShape82" -p "curve82";
	rename -uid "764DEC22-4568-27AE-2054-9FAFB480D91A";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
		0 0 0.66666666669999997
		0 0 1
		0 0 1.3333333329999999
		0 0 1.6666666670000001
		0 0 2
		0 0 2.3333333330000001
		0 0 2.6666666669999999
		0 0 3
		;
createNode transform -n "pSphere1Follicle3446" -p "hairSystem1Follicles";
	rename -uid "61CE8520-407F-AC45-A316-2FA396D65725";
createNode follicle -n "pSphere1FollicleShape3446" -p "pSphere1Follicle3446";
	rename -uid "5A42BFC5-4DD6-C633-5AE4-E5ADF76FDCB0";
	setAttr -k off ".v";
	setAttr ".pu" 0.34375;
	setAttr ".pv" 0.46875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve83" -p "pSphere1Follicle3446";
	rename -uid "1DD3104C-4B1F-00F2-8FF7-AF82DF666750";
createNode nurbsCurve -n "curveShape83" -p "curve83";
	rename -uid "2C1300C0-466C-6448-3525-14A73C01CA16";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
		0 0 0.66666666669999997
		0 0 1
		0 0 1.3333333329999999
		0 0 1.6666666670000001
		0 0 2
		0 0 2.3333333330000001
		0 0 2.6666666669999999
		0 0 3
		;
createNode transform -n "pSphere1Follicle3453" -p "hairSystem1Follicles";
	rename -uid "098F7F9A-44E6-CE38-C105-5EB21BF30897";
createNode follicle -n "pSphere1FollicleShape3453" -p "pSphere1Follicle3453";
	rename -uid "0DAD69FF-4D49-B6F4-6B8F-7CB095AF5E9D";
	setAttr -k off ".v";
	setAttr ".pu" 0.34375;
	setAttr ".pv" 0.53125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve84" -p "pSphere1Follicle3453";
	rename -uid "2CA56A07-43E4-0235-5DA9-43AF63C959C1";
createNode nurbsCurve -n "curveShape84" -p "curve84";
	rename -uid "0E5105F0-445C-F572-44E9-51A58483117A";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
		0 0 0.66666666669999997
		0 0 1
		0 0 1.3333333329999999
		0 0 1.6666666670000001
		0 0 2
		0 0 2.3333333330000001
		0 0 2.6666666669999999
		0 0 3
		;
createNode transform -n "pSphere1Follicle3459" -p "hairSystem1Follicles";
	rename -uid "C72104DF-4FE2-D645-F0F2-75A96B35A8B9";
createNode follicle -n "pSphere1FollicleShape3459" -p "pSphere1Follicle3459";
	rename -uid "62FF8C40-4B73-858A-61D8-0A8770ED65AF";
	setAttr -k off ".v";
	setAttr ".pu" 0.34375;
	setAttr ".pv" 0.59375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve85" -p "pSphere1Follicle3459";
	rename -uid "599BCCFC-4DBC-32C9-871D-ADBA4ED358CF";
createNode nurbsCurve -n "curveShape85" -p "curve85";
	rename -uid "A892D80F-4F63-131F-A571-B9ADBD88CE26";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
		0 0 0.66666666669999997
		0 0 1
		0 0 1.3333333329999999
		0 0 1.6666666670000001
		0 0 2
		0 0 2.3333333330000001
		0 0 2.6666666669999999
		0 0 3
		;
createNode transform -n "pSphere1Follicle3465" -p "hairSystem1Follicles";
	rename -uid "8E07CBA3-4109-8D97-12F0-8487FEF7E777";
createNode follicle -n "pSphere1FollicleShape3465" -p "pSphere1Follicle3465";
	rename -uid "FAA199FE-4552-A881-7A67-2BB65C1FDCB3";
	setAttr -k off ".v";
	setAttr ".pu" 0.34375;
	setAttr ".pv" 0.65625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve86" -p "pSphere1Follicle3465";
	rename -uid "501A4ED8-408C-BA89-05DE-1DA25777378D";
createNode nurbsCurve -n "curveShape86" -p "curve86";
	rename -uid "1C8E090C-4D51-8FF8-1731-FF83C24C2959";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
		0 0 0.66666666669999997
		0 0 1
		0 0 1.3333333329999999
		0 0 1.6666666670000001
		0 0 2
		0 0 2.3333333330000001
		0 0 2.6666666669999999
		0 0 3
		;
createNode transform -n "pSphere1Follicle3471" -p "hairSystem1Follicles";
	rename -uid "DB7D0D02-47F4-C3FD-CEDA-CEB6273B2876";
createNode follicle -n "pSphere1FollicleShape3471" -p "pSphere1Follicle3471";
	rename -uid "C862B46F-40DA-D4F2-1519-2FAA386F588B";
	setAttr -k off ".v";
	setAttr ".pu" 0.34375;
	setAttr ".pv" 0.71875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve87" -p "pSphere1Follicle3471";
	rename -uid "6EBA0223-409A-A80D-F6A0-56AA17C16F1E";
createNode nurbsCurve -n "curveShape87" -p "curve87";
	rename -uid "CC048262-4CA8-FCED-5CE8-E881CD3BF991";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
		0 0 0.66666666669999997
		0 0 1
		0 0 1.3333333329999999
		0 0 1.6666666670000001
		0 0 2
		0 0 2.3333333330000001
		0 0 2.6666666669999999
		0 0 3
		;
createNode transform -n "pSphere1Follicle3477" -p "hairSystem1Follicles";
	rename -uid "597F6F31-4E2D-69DA-1C88-EBAE3ACC8B51";
createNode follicle -n "pSphere1FollicleShape3477" -p "pSphere1Follicle3477";
	rename -uid "02DAE258-4771-803B-B52D-B88441FA00DD";
	setAttr -k off ".v";
	setAttr ".pu" 0.34375;
	setAttr ".pv" 0.78125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve88" -p "pSphere1Follicle3477";
	rename -uid "4E0BA79A-4672-38DB-8F52-26B9935BE68E";
createNode nurbsCurve -n "curveShape88" -p "curve88";
	rename -uid "93680F70-401E-E09E-C09B-A3BA2D4C7B60";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
		0 0 0.66666666669999997
		0 0 1
		0 0 1.3333333329999999
		0 0 1.6666666670000001
		0 0 2
		0 0 2.3333333330000001
		0 0 2.6666666669999999
		0 0 3
		;
createNode transform -n "pSphere1Follicle3484" -p "hairSystem1Follicles";
	rename -uid "D07661BD-4F15-73AD-4E69-DF93454AC5A2";
createNode follicle -n "pSphere1FollicleShape3484" -p "pSphere1Follicle3484";
	rename -uid "8F69D1A2-4E09-B64C-C1E5-74AA9DD12A7A";
	setAttr -k off ".v";
	setAttr ".pu" 0.34375;
	setAttr ".pv" 0.84375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve89" -p "pSphere1Follicle3484";
	rename -uid "92A66385-461F-9B9E-FEE4-F5B826D6E227";
createNode nurbsCurve -n "curveShape89" -p "curve89";
	rename -uid "3EF07856-48D2-E992-B644-478F5FD0465E";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
		0 0 0.66666666669999997
		0 0 1
		0 0 1.3333333329999999
		0 0 1.6666666670000001
		0 0 2
		0 0 2.3333333330000001
		0 0 2.6666666669999999
		0 0 3
		;
createNode transform -n "pSphere1Follicle3490" -p "hairSystem1Follicles";
	rename -uid "1F5698EB-4B1E-69AC-C5F6-5B94F91CE08A";
createNode follicle -n "pSphere1FollicleShape3490" -p "pSphere1Follicle3490";
	rename -uid "D8C9C3E7-4E1E-8DFF-9D93-F494B78B07A3";
	setAttr -k off ".v";
	setAttr ".pu" 0.34375;
	setAttr ".pv" 0.90625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve90" -p "pSphere1Follicle3490";
	rename -uid "9CA2CF67-4250-6A2E-D586-13998EF2321F";
createNode nurbsCurve -n "curveShape90" -p "curve90";
	rename -uid "9E6FB213-466F-7C4B-EAB7-C4B29EB107A4";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
		0 0 0.66666666669999997
		0 0 1
		0 0 1.3333333329999999
		0 0 1.6666666670000001
		0 0 2
		0 0 2.3333333330000001
		0 0 2.6666666669999999
		0 0 3
		;
createNode transform -n "pSphere1Follicle4009" -p "hairSystem1Follicles";
	rename -uid "CC3A2ED4-43B2-B7EE-F08B-74B5BBDEEB68";
createNode follicle -n "pSphere1FollicleShape4009" -p "pSphere1Follicle4009";
	rename -uid "B99D567C-4ADC-BFF5-9E4C-AEAF17F01C9C";
	setAttr -k off ".v";
	setAttr ".pu" 0.40625;
	setAttr ".pv" 0.09375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve91" -p "pSphere1Follicle4009";
	rename -uid "0E7F837F-42B3-6813-BD0D-AE9A4ECC329C";
createNode nurbsCurve -n "curveShape91" -p "curve91";
	rename -uid "C80416DF-499F-2BA9-DE91-0EB8006FBA5D";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
		0 0 0.66666666669999997
		0 0 1
		0 0 1.3333333329999999
		0 0 1.6666666670000001
		0 0 2
		0 0 2.3333333330000001
		0 0 2.6666666669999999
		0 0 3
		;
createNode transform -n "pSphere1Follicle4015" -p "hairSystem1Follicles";
	rename -uid "EFF7E0D8-44D5-6629-59D6-A6BBC5FC9191";
createNode follicle -n "pSphere1FollicleShape4015" -p "pSphere1Follicle4015";
	rename -uid "0F48A60A-4D36-DCBF-3EB6-B4826F8F19A0";
	setAttr -k off ".v";
	setAttr ".pu" 0.40625;
	setAttr ".pv" 0.15625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve92" -p "pSphere1Follicle4015";
	rename -uid "39A460DA-4D8F-F405-03F7-BDA5C1CBDE6D";
createNode nurbsCurve -n "curveShape92" -p "curve92";
	rename -uid "2D5BCCB6-42E5-7CDB-42A9-94882CD40E79";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
		0 0 0.66666666669999997
		0 0 1
		0 0 1.3333333329999999
		0 0 1.6666666670000001
		0 0 2
		0 0 2.3333333330000001
		0 0 2.6666666669999999
		0 0 3
		;
createNode transform -n "pSphere1Follicle4022" -p "hairSystem1Follicles";
	rename -uid "2C1588DB-48C3-B39A-C569-0DA7E551C631";
createNode follicle -n "pSphere1FollicleShape4022" -p "pSphere1Follicle4022";
	rename -uid "86EDA138-4CA0-DD1A-4A6D-9E8B281F4371";
	setAttr -k off ".v";
	setAttr ".pu" 0.40625;
	setAttr ".pv" 0.21875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve93" -p "pSphere1Follicle4022";
	rename -uid "8BF72DA3-4F7F-8D03-9AE9-CB8BBA47AFD9";
createNode nurbsCurve -n "curveShape93" -p "curve93";
	rename -uid "FC1E770B-4D08-55B5-C549-708A39338B91";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
		0 0 0.66666666669999997
		0 0 1
		0 0 1.3333333329999999
		0 0 1.6666666670000001
		0 0 2
		0 0 2.3333333330000001
		0 0 2.6666666669999999
		0 0 3
		;
createNode transform -n "pSphere1Follicle4028" -p "hairSystem1Follicles";
	rename -uid "A107BC3B-49BA-0087-D297-4FA03A8884C2";
createNode follicle -n "pSphere1FollicleShape4028" -p "pSphere1Follicle4028";
	rename -uid "4C662113-4B19-FD87-73E3-F7B7845EBC28";
	setAttr -k off ".v";
	setAttr ".pu" 0.40625;
	setAttr ".pv" 0.28125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve94" -p "pSphere1Follicle4028";
	rename -uid "6E6462E2-4BB5-78DC-9331-438523775F04";
createNode nurbsCurve -n "curveShape94" -p "curve94";
	rename -uid "CF27FAC7-4431-51D8-26DE-78BB60ED558A";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
		0 0 0.66666666669999997
		0 0 1
		0 0 1.3333333329999999
		0 0 1.6666666670000001
		0 0 2
		0 0 2.3333333330000001
		0 0 2.6666666669999999
		0 0 3
		;
createNode transform -n "pSphere1Follicle4034" -p "hairSystem1Follicles";
	rename -uid "DBE4549B-4F4B-4CA4-B19F-309D24882284";
createNode follicle -n "pSphere1FollicleShape4034" -p "pSphere1Follicle4034";
	rename -uid "448B4507-42EB-5A4B-7190-C88A07043D07";
	setAttr -k off ".v";
	setAttr ".pu" 0.40625;
	setAttr ".pv" 0.34375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve95" -p "pSphere1Follicle4034";
	rename -uid "7CB55C65-4BF3-E9A3-596F-1A93FA2A2444";
createNode nurbsCurve -n "curveShape95" -p "curve95";
	rename -uid "08D25601-4CEF-CAAD-92B8-8697F352FCAD";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
		0 0 0.66666666669999997
		0 0 1
		0 0 1.3333333329999999
		0 0 1.6666666670000001
		0 0 2
		0 0 2.3333333330000001
		0 0 2.6666666669999999
		0 0 3
		;
createNode transform -n "pSphere1Follicle4040" -p "hairSystem1Follicles";
	rename -uid "F83FB372-40A6-50FD-7202-968CB888C704";
createNode follicle -n "pSphere1FollicleShape4040" -p "pSphere1Follicle4040";
	rename -uid "FD101CE0-4EBD-6017-AD92-7D98E82E10AC";
	setAttr -k off ".v";
	setAttr ".pu" 0.40625;
	setAttr ".pv" 0.40625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve96" -p "pSphere1Follicle4040";
	rename -uid "B52948BD-4D59-F377-D338-808AB1A494C8";
createNode nurbsCurve -n "curveShape96" -p "curve96";
	rename -uid "2FF31551-408A-EE95-1B12-758DAD4A33F4";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
		0 0 0.66666666669999997
		0 0 1
		0 0 1.3333333329999999
		0 0 1.6666666670000001
		0 0 2
		0 0 2.3333333330000001
		0 0 2.6666666669999999
		0 0 3
		;
createNode transform -n "pSphere1Follicle4046" -p "hairSystem1Follicles";
	rename -uid "B5C84F8A-4D3E-B748-17C1-75A3D76BCF7D";
createNode follicle -n "pSphere1FollicleShape4046" -p "pSphere1Follicle4046";
	rename -uid "55F0CDA0-4D06-E836-3415-7CBA1A968B6F";
	setAttr -k off ".v";
	setAttr ".pu" 0.40625;
	setAttr ".pv" 0.46875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve97" -p "pSphere1Follicle4046";
	rename -uid "A2E71544-405D-7F3B-7362-97BCFE040A32";
createNode nurbsCurve -n "curveShape97" -p "curve97";
	rename -uid "34192425-44E3-04B0-C87A-E98AF63B5C18";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
		0 0 0.66666666669999997
		0 0 1
		0 0 1.3333333329999999
		0 0 1.6666666670000001
		0 0 2
		0 0 2.3333333330000001
		0 0 2.6666666669999999
		0 0 3
		;
createNode transform -n "pSphere1Follicle4053" -p "hairSystem1Follicles";
	rename -uid "01F680FF-4F28-ACD0-4C7B-A086399AA5AE";
createNode follicle -n "pSphere1FollicleShape4053" -p "pSphere1Follicle4053";
	rename -uid "9C5E916D-4E3E-F8EC-ED0E-91A08B9902AA";
	setAttr -k off ".v";
	setAttr ".pu" 0.40625;
	setAttr ".pv" 0.53125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve98" -p "pSphere1Follicle4053";
	rename -uid "4DC954D2-45F3-A01C-06B2-E483A2BF5AA6";
createNode nurbsCurve -n "curveShape98" -p "curve98";
	rename -uid "45440565-414B-105C-E578-3EA9D149D93D";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
		0 0 0.66666666669999997
		0 0 1
		0 0 1.3333333329999999
		0 0 1.6666666670000001
		0 0 2
		0 0 2.3333333330000001
		0 0 2.6666666669999999
		0 0 3
		;
createNode transform -n "pSphere1Follicle4059" -p "hairSystem1Follicles";
	rename -uid "5F0B837B-44FD-BD3B-6AAA-E2A87B28854B";
createNode follicle -n "pSphere1FollicleShape4059" -p "pSphere1Follicle4059";
	rename -uid "09C53476-4F79-47C0-2CF4-2088E070F33B";
	setAttr -k off ".v";
	setAttr ".pu" 0.40625;
	setAttr ".pv" 0.59375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve99" -p "pSphere1Follicle4059";
	rename -uid "23477F71-4B22-F6B2-C285-DFB720E73305";
createNode nurbsCurve -n "curveShape99" -p "curve99";
	rename -uid "B32E46D2-4235-131E-244B-3CAFC540FEA9";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
		0 0 0.66666666669999997
		0 0 1
		0 0 1.3333333329999999
		0 0 1.6666666670000001
		0 0 2
		0 0 2.3333333330000001
		0 0 2.6666666669999999
		0 0 3
		;
createNode transform -n "pSphere1Follicle4065" -p "hairSystem1Follicles";
	rename -uid "EEDE1C8A-4104-84A7-D0FB-83B077C80EC1";
createNode follicle -n "pSphere1FollicleShape4065" -p "pSphere1Follicle4065";
	rename -uid "BE27ECE2-4781-7A06-1FAE-E79E6F89FDBD";
	setAttr -k off ".v";
	setAttr ".pu" 0.40625;
	setAttr ".pv" 0.65625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve100" -p "pSphere1Follicle4065";
	rename -uid "DDA76545-4C93-649C-87F0-ED8BE667DE63";
createNode nurbsCurve -n "curveShape100" -p "curve100";
	rename -uid "2E04595F-4CC8-23C8-BB38-DDA008C8CF34";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
		0 0 0.66666666669999997
		0 0 1
		0 0 1.3333333329999999
		0 0 1.6666666670000001
		0 0 2
		0 0 2.3333333330000001
		0 0 2.6666666669999999
		0 0 3
		;
createNode transform -n "pSphere1Follicle4071" -p "hairSystem1Follicles";
	rename -uid "37FAF0BC-433C-379C-4906-F5A4C92211B5";
createNode follicle -n "pSphere1FollicleShape4071" -p "pSphere1Follicle4071";
	rename -uid "1D264978-4931-C2C7-8A88-6186EB8D57CD";
	setAttr -k off ".v";
	setAttr ".pu" 0.40625;
	setAttr ".pv" 0.71875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve101" -p "pSphere1Follicle4071";
	rename -uid "FBBFD1D6-48D5-387A-B9D6-EB9292C778C0";
createNode nurbsCurve -n "curveShape101" -p "curve101";
	rename -uid "8531A2C7-4052-F88D-7017-B683299DA1BF";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
		0 0 0.66666666669999997
		0 0 1
		0 0 1.3333333329999999
		0 0 1.6666666670000001
		0 0 2
		0 0 2.3333333330000001
		0 0 2.6666666669999999
		0 0 3
		;
createNode transform -n "pSphere1Follicle4077" -p "hairSystem1Follicles";
	rename -uid "503D1BBE-449A-E0A7-58C9-15BB5E5D0F3E";
createNode follicle -n "pSphere1FollicleShape4077" -p "pSphere1Follicle4077";
	rename -uid "A4A520E8-438C-74FB-D50F-4D815B7C1954";
	setAttr -k off ".v";
	setAttr ".pu" 0.40625;
	setAttr ".pv" 0.78125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve102" -p "pSphere1Follicle4077";
	rename -uid "23204639-4A74-4F52-169D-74B2315FE61D";
createNode nurbsCurve -n "curveShape102" -p "curve102";
	rename -uid "D5B99E9E-4F7A-E3A4-A0FD-0EA9E1436ED0";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
		0 0 0.66666666669999997
		0 0 1
		0 0 1.3333333329999999
		0 0 1.6666666670000001
		0 0 2
		0 0 2.3333333330000001
		0 0 2.6666666669999999
		0 0 3
		;
createNode transform -n "pSphere1Follicle4084" -p "hairSystem1Follicles";
	rename -uid "EC320F60-45E6-EF05-9EF6-CEA1B20B8CC5";
createNode follicle -n "pSphere1FollicleShape4084" -p "pSphere1Follicle4084";
	rename -uid "101C6F65-48CE-EEF5-2E90-AF8D7D762A87";
	setAttr -k off ".v";
	setAttr ".pu" 0.40625;
	setAttr ".pv" 0.84375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve103" -p "pSphere1Follicle4084";
	rename -uid "D0B5CFC5-4257-CEF1-2E91-A2B3EE49FB82";
createNode nurbsCurve -n "curveShape103" -p "curve103";
	rename -uid "9B4C25F5-4DA9-2A90-618F-D8A6B755E4E6";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
		0 0 0.66666666669999997
		0 0 1
		0 0 1.3333333329999999
		0 0 1.6666666670000001
		0 0 2
		0 0 2.3333333330000001
		0 0 2.6666666669999999
		0 0 3
		;
createNode transform -n "pSphere1Follicle4090" -p "hairSystem1Follicles";
	rename -uid "73E1D7A0-4737-A656-E5C3-FFA35F709364";
createNode follicle -n "pSphere1FollicleShape4090" -p "pSphere1Follicle4090";
	rename -uid "1B411907-4D2F-B8CC-4B9A-5582DFB02544";
	setAttr -k off ".v";
	setAttr ".pu" 0.40625;
	setAttr ".pv" 0.90625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve104" -p "pSphere1Follicle4090";
	rename -uid "484F344E-4C2A-72AB-4577-8283B0F9DD38";
createNode nurbsCurve -n "curveShape104" -p "curve104";
	rename -uid "FDB0A928-4113-C970-BFDE-4EBE1A04F408";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
		0 0 0.66666666669999997
		0 0 1
		0 0 1.3333333329999999
		0 0 1.6666666670000001
		0 0 2
		0 0 2.3333333330000001
		0 0 2.6666666669999999
		0 0 3
		;
createNode transform -n "pSphere1Follicle4603" -p "hairSystem1Follicles";
	rename -uid "53F9AF36-4356-6958-A981-9AB12F89957B";
createNode follicle -n "pSphere1FollicleShape4603" -p "pSphere1Follicle4603";
	rename -uid "62B3E266-4BB1-EC0B-2D65-0D80E1A55918";
	setAttr -k off ".v";
	setAttr ".pu" 0.46875;
	setAttr ".pv" 0.03125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve105" -p "pSphere1Follicle4603";
	rename -uid "D5F2CF33-4EB0-A6C9-222A-6F95967320FB";
createNode nurbsCurve -n "curveShape105" -p "curve105";
	rename -uid "C13D6AE1-48B4-AA4B-C543-34BE480A8F8E";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
		0 0 0.66666666669999997
		0 0 1
		0 0 1.3333333329999999
		0 0 1.6666666670000001
		0 0 2
		0 0 2.3333333330000001
		0 0 2.6666666669999999
		0 0 3
		;
createNode transform -n "pSphere1Follicle4609" -p "hairSystem1Follicles";
	rename -uid "E05DAFB6-421B-CEC6-7E68-06B1D9DCADE4";
createNode follicle -n "pSphere1FollicleShape4609" -p "pSphere1Follicle4609";
	rename -uid "19CAE37E-409A-1032-50DF-01A3C21A603E";
	setAttr -k off ".v";
	setAttr ".pu" 0.46875;
	setAttr ".pv" 0.09375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve106" -p "pSphere1Follicle4609";
	rename -uid "4F947648-4797-859C-92D9-7A847E2E5632";
createNode nurbsCurve -n "curveShape106" -p "curve106";
	rename -uid "86B1C6A5-4480-CCCB-F51F-21BF3E2012EB";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
		0 0 0.66666666669999997
		0 0 1
		0 0 1.3333333329999999
		0 0 1.6666666670000001
		0 0 2
		0 0 2.3333333330000001
		0 0 2.6666666669999999
		0 0 3
		;
createNode transform -n "pSphere1Follicle4615" -p "hairSystem1Follicles";
	rename -uid "D86C4F43-4C90-5B96-61C1-48BC09D9B436";
createNode follicle -n "pSphere1FollicleShape4615" -p "pSphere1Follicle4615";
	rename -uid "A8A91253-4616-5EC4-3BD3-CAA9CF751BA1";
	setAttr -k off ".v";
	setAttr ".pu" 0.46875;
	setAttr ".pv" 0.15625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve107" -p "pSphere1Follicle4615";
	rename -uid "29669368-4188-537B-2256-4595BBB6ADC2";
createNode nurbsCurve -n "curveShape107" -p "curve107";
	rename -uid "23E216FC-4056-EA1B-2517-9889C611D137";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
		0 0 0.66666666669999997
		0 0 1
		0 0 1.3333333329999999
		0 0 1.6666666670000001
		0 0 2
		0 0 2.3333333330000001
		0 0 2.6666666669999999
		0 0 3
		;
createNode transform -n "pSphere1Follicle4622" -p "hairSystem1Follicles";
	rename -uid "2CC23813-4303-158B-0E0F-C98E197B99CB";
createNode follicle -n "pSphere1FollicleShape4622" -p "pSphere1Follicle4622";
	rename -uid "251AA952-47AC-CAF9-6102-2EB578A33967";
	setAttr -k off ".v";
	setAttr ".pu" 0.46875;
	setAttr ".pv" 0.21875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve108" -p "pSphere1Follicle4622";
	rename -uid "D6DE5F83-4458-83BB-7191-C59CBB08DA62";
createNode nurbsCurve -n "curveShape108" -p "curve108";
	rename -uid "50C3E613-4D57-7333-EB3B-2EBF3A2E06EF";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
		0 0 0.66666666669999997
		0 0 1
		0 0 1.3333333329999999
		0 0 1.6666666670000001
		0 0 2
		0 0 2.3333333330000001
		0 0 2.6666666669999999
		0 0 3
		;
createNode transform -n "pSphere1Follicle4628" -p "hairSystem1Follicles";
	rename -uid "14F565AE-4ACC-5DD0-38AB-00AE776E9070";
createNode follicle -n "pSphere1FollicleShape4628" -p "pSphere1Follicle4628";
	rename -uid "43738BDA-4069-6580-5B1D-57A81F970D7A";
	setAttr -k off ".v";
	setAttr ".pu" 0.46875;
	setAttr ".pv" 0.28125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve109" -p "pSphere1Follicle4628";
	rename -uid "41606738-455E-3529-27C2-B99A815C805E";
createNode nurbsCurve -n "curveShape109" -p "curve109";
	rename -uid "1B7A75F3-4F24-6FCE-C85F-6E95EA17DB51";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
		0 0 0.66666666669999997
		0 0 1
		0 0 1.3333333329999999
		0 0 1.6666666670000001
		0 0 2
		0 0 2.3333333330000001
		0 0 2.6666666669999999
		0 0 3
		;
createNode transform -n "pSphere1Follicle4634" -p "hairSystem1Follicles";
	rename -uid "33EBC5FA-4E4F-A896-5682-EBBB5358F576";
createNode follicle -n "pSphere1FollicleShape4634" -p "pSphere1Follicle4634";
	rename -uid "17EA4849-4771-13C3-FB01-6A80E2934BCA";
	setAttr -k off ".v";
	setAttr ".pu" 0.46875;
	setAttr ".pv" 0.34375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve110" -p "pSphere1Follicle4634";
	rename -uid "9DB1B974-48AC-243A-E0B5-3DB6A1209BA1";
createNode nurbsCurve -n "curveShape110" -p "curve110";
	rename -uid "993B75EC-46FF-AC80-0EE2-BD84C90F9E7F";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
		0 0 0.66666666669999997
		0 0 1
		0 0 1.3333333329999999
		0 0 1.6666666670000001
		0 0 2
		0 0 2.3333333330000001
		0 0 2.6666666669999999
		0 0 3
		;
createNode transform -n "pSphere1Follicle4640" -p "hairSystem1Follicles";
	rename -uid "FE2BE4A4-4BD6-8AB6-7492-93861D7D3A2A";
createNode follicle -n "pSphere1FollicleShape4640" -p "pSphere1Follicle4640";
	rename -uid "8BFB02C3-4DE4-D65F-B4CF-7D8D14AB363E";
	setAttr -k off ".v";
	setAttr ".pu" 0.46875;
	setAttr ".pv" 0.40625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve111" -p "pSphere1Follicle4640";
	rename -uid "27EEE958-4CF1-6060-FDC0-CCA23C09C26A";
createNode nurbsCurve -n "curveShape111" -p "curve111";
	rename -uid "084B7C24-448C-C296-569E-0BA59E3BFE4B";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
		0 0 0.66666666669999997
		0 0 1
		0 0 1.3333333329999999
		0 0 1.6666666670000001
		0 0 2
		0 0 2.3333333330000001
		0 0 2.6666666669999999
		0 0 3
		;
createNode transform -n "pSphere1Follicle4646" -p "hairSystem1Follicles";
	rename -uid "11A369AF-4CA3-1D6C-81B9-D1B5D5C4CE22";
createNode follicle -n "pSphere1FollicleShape4646" -p "pSphere1Follicle4646";
	rename -uid "5BE28E56-4D7C-B46A-756B-01A50879EB6B";
	setAttr -k off ".v";
	setAttr ".pu" 0.46875;
	setAttr ".pv" 0.46875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve112" -p "pSphere1Follicle4646";
	rename -uid "D18CDC3A-4483-2046-C1C6-C7A2C1BCA315";
createNode nurbsCurve -n "curveShape112" -p "curve112";
	rename -uid "1569BDB2-4503-9A46-8A8A-1DB011285C75";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
		0 0 0.66666666669999997
		0 0 1
		0 0 1.3333333329999999
		0 0 1.6666666670000001
		0 0 2
		0 0 2.3333333330000001
		0 0 2.6666666669999999
		0 0 3
		;
createNode transform -n "pSphere1Follicle4653" -p "hairSystem1Follicles";
	rename -uid "7D4D49CA-4802-AF50-C870-338873BFD2FB";
createNode follicle -n "pSphere1FollicleShape4653" -p "pSphere1Follicle4653";
	rename -uid "5D6D4F08-49A8-6153-484B-6295730BAD1A";
	setAttr -k off ".v";
	setAttr ".pu" 0.46875;
	setAttr ".pv" 0.53125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve113" -p "pSphere1Follicle4653";
	rename -uid "C73820CD-4219-4A9C-7419-DF992E82B607";
createNode nurbsCurve -n "curveShape113" -p "curve113";
	rename -uid "C84264A3-4B1B-9C1D-7572-27999DF69942";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
		0 0 0.66666666669999997
		0 0 1
		0 0 1.3333333329999999
		0 0 1.6666666670000001
		0 0 2
		0 0 2.3333333330000001
		0 0 2.6666666669999999
		0 0 3
		;
createNode transform -n "pSphere1Follicle4659" -p "hairSystem1Follicles";
	rename -uid "E36782F5-486D-B805-DA04-09AB8F60FAC6";
createNode follicle -n "pSphere1FollicleShape4659" -p "pSphere1Follicle4659";
	rename -uid "B1C033D7-410B-FD96-0088-98A8F75112BB";
	setAttr -k off ".v";
	setAttr ".pu" 0.46875;
	setAttr ".pv" 0.59375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve114" -p "pSphere1Follicle4659";
	rename -uid "6845C29D-4AFD-F91E-4A56-1CB7D9D5EBDE";
createNode nurbsCurve -n "curveShape114" -p "curve114";
	rename -uid "7BC027A3-4302-7BCF-510D-5A98836321FE";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
		0 0 0.66666666669999997
		0 0 1
		0 0 1.3333333329999999
		0 0 1.6666666670000001
		0 0 2
		0 0 2.3333333330000001
		0 0 2.6666666669999999
		0 0 3
		;
createNode transform -n "pSphere1Follicle4665" -p "hairSystem1Follicles";
	rename -uid "19F9A94E-4DD6-80DC-E9BD-8FB78188B07D";
createNode follicle -n "pSphere1FollicleShape4665" -p "pSphere1Follicle4665";
	rename -uid "F82FF414-49A9-A37B-44C4-C59DE9D9C64C";
	setAttr -k off ".v";
	setAttr ".pu" 0.46875;
	setAttr ".pv" 0.65625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve115" -p "pSphere1Follicle4665";
	rename -uid "D74ABDE3-47E8-F849-B361-068B3DA83D77";
createNode nurbsCurve -n "curveShape115" -p "curve115";
	rename -uid "BA96D309-45CC-EE55-AB70-10B0CA7CF6BC";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
		0 0 0.66666666669999997
		0 0 1
		0 0 1.3333333329999999
		0 0 1.6666666670000001
		0 0 2
		0 0 2.3333333330000001
		0 0 2.6666666669999999
		0 0 3
		;
createNode transform -n "pSphere1Follicle4671" -p "hairSystem1Follicles";
	rename -uid "62072056-4E81-C988-5D13-FBBAB668BBA1";
createNode follicle -n "pSphere1FollicleShape4671" -p "pSphere1Follicle4671";
	rename -uid "20110AA9-49ED-769B-3E2D-239EA2E3F14F";
	setAttr -k off ".v";
	setAttr ".pu" 0.46875;
	setAttr ".pv" 0.71875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve116" -p "pSphere1Follicle4671";
	rename -uid "BF8A76D9-4CE6-B3C9-2FC3-66AF5E768F4E";
createNode nurbsCurve -n "curveShape116" -p "curve116";
	rename -uid "9E1FD6EF-4B93-B10E-80A4-2899E91FFCD5";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
		0 0 0.66666666669999997
		0 0 1
		0 0 1.3333333329999999
		0 0 1.6666666670000001
		0 0 2
		0 0 2.3333333330000001
		0 0 2.6666666669999999
		0 0 3
		;
createNode transform -n "pSphere1Follicle4677" -p "hairSystem1Follicles";
	rename -uid "84773EFE-4964-DCF6-80FC-2DB07653D018";
createNode follicle -n "pSphere1FollicleShape4677" -p "pSphere1Follicle4677";
	rename -uid "09665E47-4A07-A24E-D45F-08AEA5ECC7F2";
	setAttr -k off ".v";
	setAttr ".pu" 0.46875;
	setAttr ".pv" 0.78125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve117" -p "pSphere1Follicle4677";
	rename -uid "8C3CE3D8-4703-5194-E292-83AC6DC69C01";
createNode nurbsCurve -n "curveShape117" -p "curve117";
	rename -uid "56D8F9AC-42FC-DA1D-917D-56AA614B047B";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
		0 0 0.66666666669999997
		0 0 1
		0 0 1.3333333329999999
		0 0 1.6666666670000001
		0 0 2
		0 0 2.3333333330000001
		0 0 2.6666666669999999
		0 0 3
		;
createNode transform -n "pSphere1Follicle4684" -p "hairSystem1Follicles";
	rename -uid "DE13EDB4-4422-A87C-92AB-2DB0094B80BF";
createNode follicle -n "pSphere1FollicleShape4684" -p "pSphere1Follicle4684";
	rename -uid "BDE395C6-4FCC-3AEF-9C6D-19A377F346DA";
	setAttr -k off ".v";
	setAttr ".pu" 0.46875;
	setAttr ".pv" 0.84375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve118" -p "pSphere1Follicle4684";
	rename -uid "A36338D2-495C-6A78-07F1-5695824E639D";
createNode nurbsCurve -n "curveShape118" -p "curve118";
	rename -uid "B780C6B7-4D80-5AB8-7270-01BFB06CB1D4";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
		0 0 0.66666666669999997
		0 0 1
		0 0 1.3333333329999999
		0 0 1.6666666670000001
		0 0 2
		0 0 2.3333333330000001
		0 0 2.6666666669999999
		0 0 3
		;
createNode transform -n "pSphere1Follicle4690" -p "hairSystem1Follicles";
	rename -uid "566266A4-4F03-48A8-3B7F-7F898CB281B0";
createNode follicle -n "pSphere1FollicleShape4690" -p "pSphere1Follicle4690";
	rename -uid "10757B48-4492-D4F2-21EE-07B25CB189E6";
	setAttr -k off ".v";
	setAttr ".pu" 0.46875;
	setAttr ".pv" 0.90625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve119" -p "pSphere1Follicle4690";
	rename -uid "14589EFF-42B6-A709-6191-E89CEE111A58";
createNode nurbsCurve -n "curveShape119" -p "curve119";
	rename -uid "B9DDEA6B-4FC6-6280-6392-E0A3C7116946";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
		0 0 0.66666666669999997
		0 0 1
		0 0 1.3333333329999999
		0 0 1.6666666670000001
		0 0 2
		0 0 2.3333333330000001
		0 0 2.6666666669999999
		0 0 3
		;
createNode transform -n "pSphere1Follicle4696" -p "hairSystem1Follicles";
	rename -uid "9040DD0E-45AD-846B-CBD2-6FAD1668045E";
createNode follicle -n "pSphere1FollicleShape4696" -p "pSphere1Follicle4696";
	rename -uid "A8B87121-4CD0-00BD-7DE3-319C60835E8F";
	setAttr -k off ".v";
	setAttr ".pu" 0.46875;
	setAttr ".pv" 0.96875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve120" -p "pSphere1Follicle4696";
	rename -uid "85C0DD67-48D1-1B21-2DA4-4684E1EDC787";
createNode nurbsCurve -n "curveShape120" -p "curve120";
	rename -uid "BA2AA838-4F93-E2EC-E896-82BB0BB4FB83";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
		0 0 0.66666666669999997
		0 0 1
		0 0 1.3333333329999999
		0 0 1.6666666670000001
		0 0 2
		0 0 2.3333333330000001
		0 0 2.6666666669999999
		0 0 3
		;
createNode transform -n "pSphere1Follicle5303" -p "hairSystem1Follicles";
	rename -uid "45F80137-42E0-DE65-2BC3-3BB664B2CC7F";
createNode follicle -n "pSphere1FollicleShape5303" -p "pSphere1Follicle5303";
	rename -uid "857C26D5-43E3-ADCD-34FA-C89207ECB6B7";
	setAttr -k off ".v";
	setAttr ".pu" 0.53125;
	setAttr ".pv" 0.03125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve121" -p "pSphere1Follicle5303";
	rename -uid "439C00D4-4159-D419-A767-50BF86FE730A";
createNode nurbsCurve -n "curveShape121" -p "curve121";
	rename -uid "1B0F6FD1-4511-7B3A-A89C-EDA0E966E939";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
		0 0 0.66666666669999997
		0 0 1
		0 0 1.3333333329999999
		0 0 1.6666666670000001
		0 0 2
		0 0 2.3333333330000001
		0 0 2.6666666669999999
		0 0 3
		;
createNode transform -n "pSphere1Follicle5309" -p "hairSystem1Follicles";
	rename -uid "FF2D4526-4EB5-900F-2200-23B413143448";
createNode follicle -n "pSphere1FollicleShape5309" -p "pSphere1Follicle5309";
	rename -uid "96568382-41F5-09AB-941D-4CB83D20C245";
	setAttr -k off ".v";
	setAttr ".pu" 0.53125;
	setAttr ".pv" 0.09375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve122" -p "pSphere1Follicle5309";
	rename -uid "0FF2C7EF-459D-32FC-D64B-CCA8823C9B2A";
createNode nurbsCurve -n "curveShape122" -p "curve122";
	rename -uid "31E18130-4F40-A164-A97A-36AF0D4B58DF";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
		0 0 0.66666666669999997
		0 0 1
		0 0 1.3333333329999999
		0 0 1.6666666670000001
		0 0 2
		0 0 2.3333333330000001
		0 0 2.6666666669999999
		0 0 3
		;
createNode transform -n "pSphere1Follicle5315" -p "hairSystem1Follicles";
	rename -uid "06C23D0E-4C07-974E-6451-18BEEA7BF770";
createNode follicle -n "pSphere1FollicleShape5315" -p "pSphere1Follicle5315";
	rename -uid "5D45A276-4C3E-D225-2965-FB928A836565";
	setAttr -k off ".v";
	setAttr ".pu" 0.53125;
	setAttr ".pv" 0.15625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve123" -p "pSphere1Follicle5315";
	rename -uid "C4AD6BBF-47E7-2ED9-CAC9-06BB37310870";
createNode nurbsCurve -n "curveShape123" -p "curve123";
	rename -uid "1021BBA1-41EC-7979-668E-60BEC6C1CD49";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
		0 0 0.66666666669999997
		0 0 1
		0 0 1.3333333329999999
		0 0 1.6666666670000001
		0 0 2
		0 0 2.3333333330000001
		0 0 2.6666666669999999
		0 0 3
		;
createNode transform -n "pSphere1Follicle5322" -p "hairSystem1Follicles";
	rename -uid "E20F3AA9-4C25-EBEC-2CE6-398A89281F98";
createNode follicle -n "pSphere1FollicleShape5322" -p "pSphere1Follicle5322";
	rename -uid "0B3D2567-4AD5-2CDB-61F3-5D91F3688869";
	setAttr -k off ".v";
	setAttr ".pu" 0.53125;
	setAttr ".pv" 0.21875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve124" -p "pSphere1Follicle5322";
	rename -uid "A8838D4A-4CB1-CBC8-2948-15868B818E56";
createNode nurbsCurve -n "curveShape124" -p "curve124";
	rename -uid "CCF9465A-40E9-A897-6DAB-E7AC28203C6D";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
		0 0 0.66666666669999997
		0 0 1
		0 0 1.3333333329999999
		0 0 1.6666666670000001
		0 0 2
		0 0 2.3333333330000001
		0 0 2.6666666669999999
		0 0 3
		;
createNode transform -n "pSphere1Follicle5328" -p "hairSystem1Follicles";
	rename -uid "2D3A7E0F-4D32-89AA-BE44-32A51CD21412";
createNode follicle -n "pSphere1FollicleShape5328" -p "pSphere1Follicle5328";
	rename -uid "E7CC2B4E-4637-9CF2-AFD6-7FAEAC4B7C38";
	setAttr -k off ".v";
	setAttr ".pu" 0.53125;
	setAttr ".pv" 0.28125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve125" -p "pSphere1Follicle5328";
	rename -uid "CC2333C0-4B15-D856-D312-5B8221528376";
createNode nurbsCurve -n "curveShape125" -p "curve125";
	rename -uid "4783374B-4156-8F47-7808-0C9A33C16F52";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
		0 0 0.66666666669999997
		0 0 1
		0 0 1.3333333329999999
		0 0 1.6666666670000001
		0 0 2
		0 0 2.3333333330000001
		0 0 2.6666666669999999
		0 0 3
		;
createNode transform -n "pSphere1Follicle5334" -p "hairSystem1Follicles";
	rename -uid "F34BA6FA-4974-73F6-9DB9-1AB43D4C7261";
createNode follicle -n "pSphere1FollicleShape5334" -p "pSphere1Follicle5334";
	rename -uid "9EA7F3A9-45AE-88B3-C99E-1E80261FFEFB";
	setAttr -k off ".v";
	setAttr ".pu" 0.53125;
	setAttr ".pv" 0.34375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve126" -p "pSphere1Follicle5334";
	rename -uid "DDFB641E-47EC-91E9-8E04-308F14581C00";
createNode nurbsCurve -n "curveShape126" -p "curve126";
	rename -uid "A2AFBB7F-4C9B-71B1-1B5A-F899D6F3D52B";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
		0 0 0.66666666669999997
		0 0 1
		0 0 1.3333333329999999
		0 0 1.6666666670000001
		0 0 2
		0 0 2.3333333330000001
		0 0 2.6666666669999999
		0 0 3
		;
createNode transform -n "pSphere1Follicle5340" -p "hairSystem1Follicles";
	rename -uid "8A89C0CF-4B29-5F8C-9A84-A9B3C1D0BD1D";
createNode follicle -n "pSphere1FollicleShape5340" -p "pSphere1Follicle5340";
	rename -uid "CD857616-4AFF-611A-1D53-D4850789AFED";
	setAttr -k off ".v";
	setAttr ".pu" 0.53125;
	setAttr ".pv" 0.40625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve127" -p "pSphere1Follicle5340";
	rename -uid "40D3E0C1-411E-BB51-C5BA-7E842D9C3247";
createNode nurbsCurve -n "curveShape127" -p "curve127";
	rename -uid "EC879C18-4C2B-6E5D-5C85-DCB5305D3A6B";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
		0 0 0.66666666669999997
		0 0 1
		0 0 1.3333333329999999
		0 0 1.6666666670000001
		0 0 2
		0 0 2.3333333330000001
		0 0 2.6666666669999999
		0 0 3
		;
createNode transform -n "pSphere1Follicle5346" -p "hairSystem1Follicles";
	rename -uid "2DC7201B-4768-88C0-E8EE-BAA5F803D83D";
createNode follicle -n "pSphere1FollicleShape5346" -p "pSphere1Follicle5346";
	rename -uid "EF4F7D1D-479B-1FF7-BC64-EE9E8E803620";
	setAttr -k off ".v";
	setAttr ".pu" 0.53125;
	setAttr ".pv" 0.46875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve128" -p "pSphere1Follicle5346";
	rename -uid "4D130626-4FBB-05A1-D4E2-689A918ADDAD";
createNode nurbsCurve -n "curveShape128" -p "curve128";
	rename -uid "6410B4CA-4FF6-5EBD-2C7E-049B9F8D1FE2";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
		0 0 0.66666666669999997
		0 0 1
		0 0 1.3333333329999999
		0 0 1.6666666670000001
		0 0 2
		0 0 2.3333333330000001
		0 0 2.6666666669999999
		0 0 3
		;
createNode transform -n "pSphere1Follicle5353" -p "hairSystem1Follicles";
	rename -uid "D616A6D5-4E76-467E-D963-D1BE11F64749";
createNode follicle -n "pSphere1FollicleShape5353" -p "pSphere1Follicle5353";
	rename -uid "E2716646-4346-6B08-FCB6-D8B5E0188035";
	setAttr -k off ".v";
	setAttr ".pu" 0.53125;
	setAttr ".pv" 0.53125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve129" -p "pSphere1Follicle5353";
	rename -uid "0769571B-48BE-2FB2-C191-BEBD4ED49623";
createNode nurbsCurve -n "curveShape129" -p "curve129";
	rename -uid "C48BB160-4226-F35D-C8E1-8E8F730E8588";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
		0 0 0.66666666669999997
		0 0 1
		0 0 1.3333333329999999
		0 0 1.6666666670000001
		0 0 2
		0 0 2.3333333330000001
		0 0 2.6666666669999999
		0 0 3
		;
createNode transform -n "pSphere1Follicle5359" -p "hairSystem1Follicles";
	rename -uid "81185819-44FA-CF8C-9377-798403BE922E";
createNode follicle -n "pSphere1FollicleShape5359" -p "pSphere1Follicle5359";
	rename -uid "1DA67D24-4925-9572-6957-29A457111262";
	setAttr -k off ".v";
	setAttr ".pu" 0.53125;
	setAttr ".pv" 0.59375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve130" -p "pSphere1Follicle5359";
	rename -uid "9E07A43E-49B6-683C-2FB0-06BEBC6567A1";
createNode nurbsCurve -n "curveShape130" -p "curve130";
	rename -uid "6004A294-494D-C407-BB48-BD8B45BBA673";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
		0 0 0.66666666669999997
		0 0 1
		0 0 1.3333333329999999
		0 0 1.6666666670000001
		0 0 2
		0 0 2.3333333330000001
		0 0 2.6666666669999999
		0 0 3
		;
createNode transform -n "pSphere1Follicle5365" -p "hairSystem1Follicles";
	rename -uid "D202B111-442A-B562-E4F7-098CCD39161E";
createNode follicle -n "pSphere1FollicleShape5365" -p "pSphere1Follicle5365";
	rename -uid "AD515F24-4969-86B0-6F61-B89AD145A4E1";
	setAttr -k off ".v";
	setAttr ".pu" 0.53125;
	setAttr ".pv" 0.65625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve131" -p "pSphere1Follicle5365";
	rename -uid "1AA622A2-4989-224E-2205-2EB93C707FD5";
createNode nurbsCurve -n "curveShape131" -p "curve131";
	rename -uid "EEEE5C11-4DC7-1977-F5CB-CB8F22EADDB1";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
		0 0 0.66666666669999997
		0 0 1
		0 0 1.3333333329999999
		0 0 1.6666666670000001
		0 0 2
		0 0 2.3333333330000001
		0 0 2.6666666669999999
		0 0 3
		;
createNode transform -n "pSphere1Follicle5371" -p "hairSystem1Follicles";
	rename -uid "2CC27C68-4C13-94E1-8BDE-409C1CB9870A";
createNode follicle -n "pSphere1FollicleShape5371" -p "pSphere1Follicle5371";
	rename -uid "0DB458DF-41D9-5685-41D1-4FADD128091D";
	setAttr -k off ".v";
	setAttr ".pu" 0.53125;
	setAttr ".pv" 0.71875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve132" -p "pSphere1Follicle5371";
	rename -uid "A2B0CC8D-418D-B975-D4B0-6A9D2F1C3932";
createNode nurbsCurve -n "curveShape132" -p "curve132";
	rename -uid "6D0CB0C1-494E-60B0-3F94-D1AF6842EEE4";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
		0 0 0.66666666669999997
		0 0 1
		0 0 1.3333333329999999
		0 0 1.6666666670000001
		0 0 2
		0 0 2.3333333330000001
		0 0 2.6666666669999999
		0 0 3
		;
createNode transform -n "pSphere1Follicle5377" -p "hairSystem1Follicles";
	rename -uid "7B648B1A-45DF-F8E7-37B6-C3B45E7BBC7B";
createNode follicle -n "pSphere1FollicleShape5377" -p "pSphere1Follicle5377";
	rename -uid "DB6DFEB6-4EB4-4575-4FF7-ADB9A40E3C79";
	setAttr -k off ".v";
	setAttr ".pu" 0.53125;
	setAttr ".pv" 0.78125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve133" -p "pSphere1Follicle5377";
	rename -uid "4F7F84D6-4D1E-201D-8DA7-A99A3913782B";
createNode nurbsCurve -n "curveShape133" -p "curve133";
	rename -uid "9D38C8E1-4254-3245-667F-1387EF7C1BC3";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
		0 0 0.66666666669999997
		0 0 1
		0 0 1.3333333329999999
		0 0 1.6666666670000001
		0 0 2
		0 0 2.3333333330000001
		0 0 2.6666666669999999
		0 0 3
		;
createNode transform -n "pSphere1Follicle5384" -p "hairSystem1Follicles";
	rename -uid "C2602913-4F43-3850-54E0-2798D7D4B34F";
createNode follicle -n "pSphere1FollicleShape5384" -p "pSphere1Follicle5384";
	rename -uid "189ABD95-4FFD-5EC5-5347-FDB4D26B224F";
	setAttr -k off ".v";
	setAttr ".pu" 0.53125;
	setAttr ".pv" 0.84375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve134" -p "pSphere1Follicle5384";
	rename -uid "5C3B86FB-40CD-4AB0-95B3-1EB90C6D44AE";
createNode nurbsCurve -n "curveShape134" -p "curve134";
	rename -uid "1E376B08-48F4-D0B2-2DAA-6F9458FA2527";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
		0 0 0.66666666669999997
		0 0 1
		0 0 1.3333333329999999
		0 0 1.6666666670000001
		0 0 2
		0 0 2.3333333330000001
		0 0 2.6666666669999999
		0 0 3
		;
createNode transform -n "pSphere1Follicle5390" -p "hairSystem1Follicles";
	rename -uid "7E444BBF-404A-4FA2-92E7-80B87EA27632";
createNode follicle -n "pSphere1FollicleShape5390" -p "pSphere1Follicle5390";
	rename -uid "1A3327D6-41BF-0039-3522-398BE6C8EEEA";
	setAttr -k off ".v";
	setAttr ".pu" 0.53125;
	setAttr ".pv" 0.90625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve135" -p "pSphere1Follicle5390";
	rename -uid "9971D5B5-4AA5-52BD-4FB6-2CA4284A9E57";
createNode nurbsCurve -n "curveShape135" -p "curve135";
	rename -uid "5012C77E-43DF-C95D-A757-48AEB36C1150";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
		0 0 0.66666666669999997
		0 0 1
		0 0 1.3333333329999999
		0 0 1.6666666670000001
		0 0 2
		0 0 2.3333333330000001
		0 0 2.6666666669999999
		0 0 3
		;
createNode transform -n "pSphere1Follicle5396" -p "hairSystem1Follicles";
	rename -uid "7D6D12A9-47B1-F9EC-D9E5-1EBB71134F15";
createNode follicle -n "pSphere1FollicleShape5396" -p "pSphere1Follicle5396";
	rename -uid "70B52B63-4F7A-3F81-600C-11AD4DE2D071";
	setAttr -k off ".v";
	setAttr ".pu" 0.53125;
	setAttr ".pv" 0.96875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve136" -p "pSphere1Follicle5396";
	rename -uid "355271BA-430D-A47E-B837-23BBCF85CB3F";
createNode nurbsCurve -n "curveShape136" -p "curve136";
	rename -uid "F7A2A0F9-461E-DFC6-0369-80A26BAC684F";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
		0 0 0.66666666669999997
		0 0 1
		0 0 1.3333333329999999
		0 0 1.6666666670000001
		0 0 2
		0 0 2.3333333330000001
		0 0 2.6666666669999999
		0 0 3
		;
createNode transform -n "pSphere1Follicle5909" -p "hairSystem1Follicles";
	rename -uid "BC41C550-4036-DC34-A1DD-B2A2FA23B233";
createNode follicle -n "pSphere1FollicleShape5909" -p "pSphere1Follicle5909";
	rename -uid "C400E114-421F-4928-E66F-7C92EF469671";
	setAttr -k off ".v";
	setAttr ".pu" 0.59375;
	setAttr ".pv" 0.09375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve137" -p "pSphere1Follicle5909";
	rename -uid "BFD6E76F-46B1-BA27-3141-7DB548F358CB";
createNode nurbsCurve -n "curveShape137" -p "curve137";
	rename -uid "53D68A4F-4928-F2C8-5ADC-3E9209475E75";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
		0 0 0.66666666669999997
		0 0 1
		0 0 1.3333333329999999
		0 0 1.6666666670000001
		0 0 2
		0 0 2.3333333330000001
		0 0 2.6666666669999999
		0 0 3
		;
createNode transform -n "pSphere1Follicle5915" -p "hairSystem1Follicles";
	rename -uid "97756BFE-4FA0-E7FB-79F7-9C8076E7AC90";
createNode follicle -n "pSphere1FollicleShape5915" -p "pSphere1Follicle5915";
	rename -uid "DACEE7DA-4070-C5B6-FD96-7AB3C2732EBB";
	setAttr -k off ".v";
	setAttr ".pu" 0.59375;
	setAttr ".pv" 0.15625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve138" -p "pSphere1Follicle5915";
	rename -uid "19C353BE-4137-C4A9-4330-13B2856FFBB5";
createNode nurbsCurve -n "curveShape138" -p "curve138";
	rename -uid "70213B01-400B-6F1A-CCF8-80BFE8166ABC";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
		0 0 0.66666666669999997
		0 0 1
		0 0 1.3333333329999999
		0 0 1.6666666670000001
		0 0 2
		0 0 2.3333333330000001
		0 0 2.6666666669999999
		0 0 3
		;
createNode transform -n "pSphere1Follicle5922" -p "hairSystem1Follicles";
	rename -uid "77321F34-4A5C-5A57-16D9-B5ADFCC82147";
createNode follicle -n "pSphere1FollicleShape5922" -p "pSphere1Follicle5922";
	rename -uid "FF87123D-48A2-E81A-F4DA-B485952DD2D1";
	setAttr -k off ".v";
	setAttr ".pu" 0.59375;
	setAttr ".pv" 0.21875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve139" -p "pSphere1Follicle5922";
	rename -uid "491E481F-42C0-CE97-F258-D4B2BD940287";
createNode nurbsCurve -n "curveShape139" -p "curve139";
	rename -uid "3E190FDB-47BE-C855-2B41-CBB133149B2E";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
		0 0 0.66666666669999997
		0 0 1
		0 0 1.3333333329999999
		0 0 1.6666666670000001
		0 0 2
		0 0 2.3333333330000001
		0 0 2.6666666669999999
		0 0 3
		;
createNode transform -n "pSphere1Follicle5928" -p "hairSystem1Follicles";
	rename -uid "CE4706D8-49E8-590F-CA78-CD9A840D6EB3";
createNode follicle -n "pSphere1FollicleShape5928" -p "pSphere1Follicle5928";
	rename -uid "FFCEB5FC-4220-5EB9-E393-58AAB584EB49";
	setAttr -k off ".v";
	setAttr ".pu" 0.59375;
	setAttr ".pv" 0.28125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve140" -p "pSphere1Follicle5928";
	rename -uid "5ACDE7C2-44C7-0717-7AF9-40BF588C5EF3";
createNode nurbsCurve -n "curveShape140" -p "curve140";
	rename -uid "E40CDBD5-4FD0-E433-FA8B-A8999BFC9499";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
		0 0 0.66666666669999997
		0 0 1
		0 0 1.3333333329999999
		0 0 1.6666666670000001
		0 0 2
		0 0 2.3333333330000001
		0 0 2.6666666669999999
		0 0 3
		;
createNode transform -n "pSphere1Follicle5934" -p "hairSystem1Follicles";
	rename -uid "9489BF8F-477F-58E9-8298-629DC6A92836";
createNode follicle -n "pSphere1FollicleShape5934" -p "pSphere1Follicle5934";
	rename -uid "AF50E00A-4555-BDE4-A3BE-2893D893AC9F";
	setAttr -k off ".v";
	setAttr ".pu" 0.59375;
	setAttr ".pv" 0.34375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve141" -p "pSphere1Follicle5934";
	rename -uid "830E3586-4CA0-DF2D-FC10-93912BD7DAFE";
createNode nurbsCurve -n "curveShape141" -p "curve141";
	rename -uid "795DD1D4-4A31-3AC6-7522-5A973401E9C7";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
		0 0 0.66666666669999997
		0 0 1
		0 0 1.3333333329999999
		0 0 1.6666666670000001
		0 0 2
		0 0 2.3333333330000001
		0 0 2.6666666669999999
		0 0 3
		;
createNode transform -n "pSphere1Follicle5940" -p "hairSystem1Follicles";
	rename -uid "1EA574B0-44E4-D444-5CF6-BDA878E93F7B";
createNode follicle -n "pSphere1FollicleShape5940" -p "pSphere1Follicle5940";
	rename -uid "D18D0D3C-4D88-47A0-BCDC-D9BF90A7E2BF";
	setAttr -k off ".v";
	setAttr ".pu" 0.59375;
	setAttr ".pv" 0.40625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve142" -p "pSphere1Follicle5940";
	rename -uid "2AB6722E-4012-7533-86FB-4797AE167A8D";
createNode nurbsCurve -n "curveShape142" -p "curve142";
	rename -uid "07344D62-4F1E-0493-49C0-CCBD976908D7";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
		0 0 0.66666666669999997
		0 0 1
		0 0 1.3333333329999999
		0 0 1.6666666670000001
		0 0 2
		0 0 2.3333333330000001
		0 0 2.6666666669999999
		0 0 3
		;
createNode transform -n "pSphere1Follicle5946" -p "hairSystem1Follicles";
	rename -uid "8A2E45D4-4B8B-C837-74A1-708303DF30B5";
createNode follicle -n "pSphere1FollicleShape5946" -p "pSphere1Follicle5946";
	rename -uid "E5ABAC5C-40CC-A49B-68FE-268C33AB54D7";
	setAttr -k off ".v";
	setAttr ".pu" 0.59375;
	setAttr ".pv" 0.46875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve143" -p "pSphere1Follicle5946";
	rename -uid "2C2B0934-45F4-6BCB-1453-DC9F62436533";
createNode nurbsCurve -n "curveShape143" -p "curve143";
	rename -uid "7AC19F06-4E03-A9DA-2849-CC9185BB278F";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
		0 0 0.66666666669999997
		0 0 1
		0 0 1.3333333329999999
		0 0 1.6666666670000001
		0 0 2
		0 0 2.3333333330000001
		0 0 2.6666666669999999
		0 0 3
		;
createNode transform -n "pSphere1Follicle5953" -p "hairSystem1Follicles";
	rename -uid "62D36079-4BD1-D6BC-3F98-97B0EA3BFB0F";
createNode follicle -n "pSphere1FollicleShape5953" -p "pSphere1Follicle5953";
	rename -uid "9F18830B-4537-24D6-184B-DBB9631600B9";
	setAttr -k off ".v";
	setAttr ".pu" 0.59375;
	setAttr ".pv" 0.53125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve144" -p "pSphere1Follicle5953";
	rename -uid "5E668FFB-4FF7-2ADC-E1A8-0782D9E4515D";
createNode nurbsCurve -n "curveShape144" -p "curve144";
	rename -uid "DF5A6750-4B7D-9DF0-DB2C-66865B743323";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
		0 0 0.66666666669999997
		0 0 1
		0 0 1.3333333329999999
		0 0 1.6666666670000001
		0 0 2
		0 0 2.3333333330000001
		0 0 2.6666666669999999
		0 0 3
		;
createNode transform -n "pSphere1Follicle5959" -p "hairSystem1Follicles";
	rename -uid "C0B301D5-41EB-8A98-B304-E8BEF325E71A";
createNode follicle -n "pSphere1FollicleShape5959" -p "pSphere1Follicle5959";
	rename -uid "D86857BE-4615-1968-1A8C-54BB81692111";
	setAttr -k off ".v";
	setAttr ".pu" 0.59375;
	setAttr ".pv" 0.59375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve145" -p "pSphere1Follicle5959";
	rename -uid "637D9C19-43D8-C5A6-B16A-6C9A75574ACE";
createNode nurbsCurve -n "curveShape145" -p "curve145";
	rename -uid "8CB8C195-43E2-8221-AF57-248D753C0297";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
		0 0 0.66666666669999997
		0 0 1
		0 0 1.3333333329999999
		0 0 1.6666666670000001
		0 0 2
		0 0 2.3333333330000001
		0 0 2.6666666669999999
		0 0 3
		;
createNode transform -n "pSphere1Follicle5965" -p "hairSystem1Follicles";
	rename -uid "99CCCD8C-4F67-CD4D-CBE6-728DA960B60D";
createNode follicle -n "pSphere1FollicleShape5965" -p "pSphere1Follicle5965";
	rename -uid "B251FB5A-4FF8-08F6-51EC-6BAF808F1202";
	setAttr -k off ".v";
	setAttr ".pu" 0.59375;
	setAttr ".pv" 0.65625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve146" -p "pSphere1Follicle5965";
	rename -uid "38E65D34-4B20-1AAA-2CC4-0FB0030752EF";
createNode nurbsCurve -n "curveShape146" -p "curve146";
	rename -uid "0ADBD491-46E7-C319-8CBF-03844A44E84A";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
		0 0 0.66666666669999997
		0 0 1
		0 0 1.3333333329999999
		0 0 1.6666666670000001
		0 0 2
		0 0 2.3333333330000001
		0 0 2.6666666669999999
		0 0 3
		;
createNode transform -n "pSphere1Follicle5971" -p "hairSystem1Follicles";
	rename -uid "1250C8C8-4780-8E38-BFF8-878426B1D341";
createNode follicle -n "pSphere1FollicleShape5971" -p "pSphere1Follicle5971";
	rename -uid "60AF42E1-4ABB-2037-5821-1FA79414BFAF";
	setAttr -k off ".v";
	setAttr ".pu" 0.59375;
	setAttr ".pv" 0.71875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve147" -p "pSphere1Follicle5971";
	rename -uid "68A3413D-4EFC-F8BA-7243-8781402C0807";
createNode nurbsCurve -n "curveShape147" -p "curve147";
	rename -uid "E3BF43A6-4849-CCB6-CC10-4BA8FEAC9411";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
		0 0 0.66666666669999997
		0 0 1
		0 0 1.3333333329999999
		0 0 1.6666666670000001
		0 0 2
		0 0 2.3333333330000001
		0 0 2.6666666669999999
		0 0 3
		;
createNode transform -n "pSphere1Follicle5977" -p "hairSystem1Follicles";
	rename -uid "479BF7FB-4CF5-4507-0052-989914638705";
createNode follicle -n "pSphere1FollicleShape5977" -p "pSphere1Follicle5977";
	rename -uid "DFE302F8-4E7A-D07D-5660-999E345333D9";
	setAttr -k off ".v";
	setAttr ".pu" 0.59375;
	setAttr ".pv" 0.78125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve148" -p "pSphere1Follicle5977";
	rename -uid "679514ED-48A8-280D-5D9D-9E96F18DFD86";
createNode nurbsCurve -n "curveShape148" -p "curve148";
	rename -uid "39B06055-448B-759E-6C6F-5EA5A65E3B9B";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
		0 0 0.66666666669999997
		0 0 1
		0 0 1.3333333329999999
		0 0 1.6666666670000001
		0 0 2
		0 0 2.3333333330000001
		0 0 2.6666666669999999
		0 0 3
		;
createNode transform -n "pSphere1Follicle5984" -p "hairSystem1Follicles";
	rename -uid "A501E40D-4DC3-0BDC-5AE0-16ACA0C02172";
createNode follicle -n "pSphere1FollicleShape5984" -p "pSphere1Follicle5984";
	rename -uid "39F794AF-4C08-7954-AACE-33B31E7C9158";
	setAttr -k off ".v";
	setAttr ".pu" 0.59375;
	setAttr ".pv" 0.84375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve149" -p "pSphere1Follicle5984";
	rename -uid "AE5C9D9A-4073-6C94-EA1A-3DB1BD5A91C2";
createNode nurbsCurve -n "curveShape149" -p "curve149";
	rename -uid "E9485DE5-4CA7-8C7D-6315-0785943FC45B";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
		0 0 0.66666666669999997
		0 0 1
		0 0 1.3333333329999999
		0 0 1.6666666670000001
		0 0 2
		0 0 2.3333333330000001
		0 0 2.6666666669999999
		0 0 3
		;
createNode transform -n "pSphere1Follicle5990" -p "hairSystem1Follicles";
	rename -uid "B368CEE1-4171-4106-A530-659FBDCDEAD8";
createNode follicle -n "pSphere1FollicleShape5990" -p "pSphere1Follicle5990";
	rename -uid "19E0288A-4DDC-C410-B64C-CC99A1250C7A";
	setAttr -k off ".v";
	setAttr ".pu" 0.59375;
	setAttr ".pv" 0.90625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve150" -p "pSphere1Follicle5990";
	rename -uid "E51FBC22-4E41-4B06-37AC-958B8AB8B52D";
createNode nurbsCurve -n "curveShape150" -p "curve150";
	rename -uid "B567522E-41D5-0661-9B44-C9806B0BD2BF";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
		0 0 0.66666666669999997
		0 0 1
		0 0 1.3333333329999999
		0 0 1.6666666670000001
		0 0 2
		0 0 2.3333333330000001
		0 0 2.6666666669999999
		0 0 3
		;
createNode transform -n "pSphere1Follicle6509" -p "hairSystem1Follicles";
	rename -uid "C91509F0-4245-5890-0747-C3A2A67AF4B6";
createNode follicle -n "pSphere1FollicleShape6509" -p "pSphere1Follicle6509";
	rename -uid "A649BF13-4AFE-7BBE-E5B1-9CA0E3ACB51D";
	setAttr -k off ".v";
	setAttr ".pu" 0.65625;
	setAttr ".pv" 0.09375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve151" -p "pSphere1Follicle6509";
	rename -uid "68CAC11C-402C-67C5-A407-D89A668C6BCD";
createNode nurbsCurve -n "curveShape151" -p "curve151";
	rename -uid "7F1BE431-441C-0EB6-A157-A1BFA769B615";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
		0 0 0.66666666669999997
		0 0 1
		0 0 1.3333333329999999
		0 0 1.6666666670000001
		0 0 2
		0 0 2.3333333330000001
		0 0 2.6666666669999999
		0 0 3
		;
createNode transform -n "pSphere1Follicle6515" -p "hairSystem1Follicles";
	rename -uid "F5AB2880-47B9-DFC9-1BE1-E6B2EFFF42D0";
createNode follicle -n "pSphere1FollicleShape6515" -p "pSphere1Follicle6515";
	rename -uid "6DFA35B8-4F34-761F-A25A-47AC2072A4D0";
	setAttr -k off ".v";
	setAttr ".pu" 0.65625;
	setAttr ".pv" 0.15625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve152" -p "pSphere1Follicle6515";
	rename -uid "D7F323D7-45CF-4356-DAAF-3EBEE0AC27DB";
createNode nurbsCurve -n "curveShape152" -p "curve152";
	rename -uid "28A8B9BA-485C-B540-AEC1-27ADD352FB59";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
		0 0 0.66666666669999997
		0 0 1
		0 0 1.3333333329999999
		0 0 1.6666666670000001
		0 0 2
		0 0 2.3333333330000001
		0 0 2.6666666669999999
		0 0 3
		;
createNode transform -n "pSphere1Follicle6522" -p "hairSystem1Follicles";
	rename -uid "AC18B4B0-4827-108B-80D6-BE95104C1D19";
createNode follicle -n "pSphere1FollicleShape6522" -p "pSphere1Follicle6522";
	rename -uid "94920859-43EE-BB96-D706-F9896A7E324F";
	setAttr -k off ".v";
	setAttr ".pu" 0.65625;
	setAttr ".pv" 0.21875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve153" -p "pSphere1Follicle6522";
	rename -uid "66119F54-469D-F320-08A0-EC94DFF469C4";
createNode nurbsCurve -n "curveShape153" -p "curve153";
	rename -uid "B66130F6-4063-4392-16E6-699A225BAFCD";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
		0 0 0.66666666669999997
		0 0 1
		0 0 1.3333333329999999
		0 0 1.6666666670000001
		0 0 2
		0 0 2.3333333330000001
		0 0 2.6666666669999999
		0 0 3
		;
createNode transform -n "pSphere1Follicle6528" -p "hairSystem1Follicles";
	rename -uid "34A2F660-4298-C441-AB58-288C0BF91879";
createNode follicle -n "pSphere1FollicleShape6528" -p "pSphere1Follicle6528";
	rename -uid "94957754-4555-7E90-C184-A28D8B1CD198";
	setAttr -k off ".v";
	setAttr ".pu" 0.65625;
	setAttr ".pv" 0.28125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve154" -p "pSphere1Follicle6528";
	rename -uid "E2635066-4A63-2D0E-8E95-8DBFE7E4C277";
createNode nurbsCurve -n "curveShape154" -p "curve154";
	rename -uid "B8941B08-4DEB-0679-BDDE-218449C11318";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
		0 0 0.66666666669999997
		0 0 1
		0 0 1.3333333329999999
		0 0 1.6666666670000001
		0 0 2
		0 0 2.3333333330000001
		0 0 2.6666666669999999
		0 0 3
		;
createNode transform -n "pSphere1Follicle6534" -p "hairSystem1Follicles";
	rename -uid "7AC1C738-4DD7-CF2B-D542-9CB46EC3F138";
createNode follicle -n "pSphere1FollicleShape6534" -p "pSphere1Follicle6534";
	rename -uid "F5B2B176-4ACC-4899-76CB-A88A41A88B81";
	setAttr -k off ".v";
	setAttr ".pu" 0.65625;
	setAttr ".pv" 0.34375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve155" -p "pSphere1Follicle6534";
	rename -uid "DCBD0278-44F3-7FB5-2716-C4A720CA2302";
createNode nurbsCurve -n "curveShape155" -p "curve155";
	rename -uid "A94EE615-40BF-F5DB-E465-498F909352B8";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
		0 0 0.66666666669999997
		0 0 1
		0 0 1.3333333329999999
		0 0 1.6666666670000001
		0 0 2
		0 0 2.3333333330000001
		0 0 2.6666666669999999
		0 0 3
		;
createNode transform -n "pSphere1Follicle6540" -p "hairSystem1Follicles";
	rename -uid "1DEA469A-4946-1D80-33A0-C58837108B9A";
createNode follicle -n "pSphere1FollicleShape6540" -p "pSphere1Follicle6540";
	rename -uid "002EA496-48E7-23BF-0508-0D8F6ECDEF18";
	setAttr -k off ".v";
	setAttr ".pu" 0.65625;
	setAttr ".pv" 0.40625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve156" -p "pSphere1Follicle6540";
	rename -uid "D1C14222-4128-1841-3DC2-14A42145317B";
createNode nurbsCurve -n "curveShape156" -p "curve156";
	rename -uid "7EE60969-4834-8C9C-2FCF-31A7457624E8";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
		0 0 0.66666666669999997
		0 0 1
		0 0 1.3333333329999999
		0 0 1.6666666670000001
		0 0 2
		0 0 2.3333333330000001
		0 0 2.6666666669999999
		0 0 3
		;
createNode transform -n "pSphere1Follicle6546" -p "hairSystem1Follicles";
	rename -uid "5DB19C25-4568-CA33-291F-7C8BF8E8DA69";
createNode follicle -n "pSphere1FollicleShape6546" -p "pSphere1Follicle6546";
	rename -uid "F1CDF6F4-44C7-B2BB-6F7E-AC85B5EA2E9F";
	setAttr -k off ".v";
	setAttr ".pu" 0.65625;
	setAttr ".pv" 0.46875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve157" -p "pSphere1Follicle6546";
	rename -uid "92A127A7-4CE2-7079-0CB8-2CA8657B62F8";
createNode nurbsCurve -n "curveShape157" -p "curve157";
	rename -uid "04371A2D-4EFC-F307-04E1-4F95E92CC1F4";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
		0 0 0.66666666669999997
		0 0 1
		0 0 1.3333333329999999
		0 0 1.6666666670000001
		0 0 2
		0 0 2.3333333330000001
		0 0 2.6666666669999999
		0 0 3
		;
createNode transform -n "pSphere1Follicle6553" -p "hairSystem1Follicles";
	rename -uid "4EDBF1CD-4106-1F5C-63F5-FAAA8C188220";
createNode follicle -n "pSphere1FollicleShape6553" -p "pSphere1Follicle6553";
	rename -uid "F6CA1DE0-462D-7CE0-251F-D8963094DE70";
	setAttr -k off ".v";
	setAttr ".pu" 0.65625;
	setAttr ".pv" 0.53125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve158" -p "pSphere1Follicle6553";
	rename -uid "DA9455C8-40A9-E557-237D-A68718B051DD";
createNode nurbsCurve -n "curveShape158" -p "curve158";
	rename -uid "8520E7BA-430F-5E1C-0743-02A86737C251";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
		0 0 0.66666666669999997
		0 0 1
		0 0 1.3333333329999999
		0 0 1.6666666670000001
		0 0 2
		0 0 2.3333333330000001
		0 0 2.6666666669999999
		0 0 3
		;
createNode transform -n "pSphere1Follicle6559" -p "hairSystem1Follicles";
	rename -uid "043E9C4D-41F2-164A-18A9-689C097D178A";
createNode follicle -n "pSphere1FollicleShape6559" -p "pSphere1Follicle6559";
	rename -uid "DF48BBA9-481D-E5A3-47E4-FFACC0C15FFF";
	setAttr -k off ".v";
	setAttr ".pu" 0.65625;
	setAttr ".pv" 0.59375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve159" -p "pSphere1Follicle6559";
	rename -uid "3A04970E-4535-41D6-F9CE-6F8F8A26882D";
createNode nurbsCurve -n "curveShape159" -p "curve159";
	rename -uid "9C2EDA12-41E0-7162-22A5-85AC6914633B";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
		0 0 0.66666666669999997
		0 0 1
		0 0 1.3333333329999999
		0 0 1.6666666670000001
		0 0 2
		0 0 2.3333333330000001
		0 0 2.6666666669999999
		0 0 3
		;
createNode transform -n "pSphere1Follicle6565" -p "hairSystem1Follicles";
	rename -uid "2D5DD043-4154-39E5-511B-F1ACB76E1139";
createNode follicle -n "pSphere1FollicleShape6565" -p "pSphere1Follicle6565";
	rename -uid "EE7EA820-4104-E230-3C8A-C09A7EF1E3AC";
	setAttr -k off ".v";
	setAttr ".pu" 0.65625;
	setAttr ".pv" 0.65625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve160" -p "pSphere1Follicle6565";
	rename -uid "85A259B8-4834-21F3-2531-CEB1969F08F0";
createNode nurbsCurve -n "curveShape160" -p "curve160";
	rename -uid "31D415C7-486B-EF96-2BFE-6E93971B9F21";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
		0 0 0.66666666669999997
		0 0 1
		0 0 1.3333333329999999
		0 0 1.6666666670000001
		0 0 2
		0 0 2.3333333330000001
		0 0 2.6666666669999999
		0 0 3
		;
createNode transform -n "pSphere1Follicle6571" -p "hairSystem1Follicles";
	rename -uid "3A1B19C1-4B08-F6B5-3E9A-65842AF3BDC9";
createNode follicle -n "pSphere1FollicleShape6571" -p "pSphere1Follicle6571";
	rename -uid "54AD43EA-4FEE-7CA2-C5DF-68AE43E0AC37";
	setAttr -k off ".v";
	setAttr ".pu" 0.65625;
	setAttr ".pv" 0.71875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve161" -p "pSphere1Follicle6571";
	rename -uid "66DDDFE9-4842-E694-007F-28B91C239EF1";
createNode nurbsCurve -n "curveShape161" -p "curve161";
	rename -uid "20A4D4FE-4759-5C9D-9F47-B4A736119B5F";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
		0 0 0.66666666669999997
		0 0 1
		0 0 1.3333333329999999
		0 0 1.6666666670000001
		0 0 2
		0 0 2.3333333330000001
		0 0 2.6666666669999999
		0 0 3
		;
createNode transform -n "pSphere1Follicle6577" -p "hairSystem1Follicles";
	rename -uid "0A3FC3D6-4B38-CBC0-2983-24862081F687";
createNode follicle -n "pSphere1FollicleShape6577" -p "pSphere1Follicle6577";
	rename -uid "99A3D64D-4ECA-0600-3569-5BB3A0095BA8";
	setAttr -k off ".v";
	setAttr ".pu" 0.65625;
	setAttr ".pv" 0.78125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve162" -p "pSphere1Follicle6577";
	rename -uid "CBEB40B0-404B-9010-0FBB-92BD4F967739";
createNode nurbsCurve -n "curveShape162" -p "curve162";
	rename -uid "4D0002F4-4BBD-C9A6-0743-638169B49356";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
		0 0 0.66666666669999997
		0 0 1
		0 0 1.3333333329999999
		0 0 1.6666666670000001
		0 0 2
		0 0 2.3333333330000001
		0 0 2.6666666669999999
		0 0 3
		;
createNode transform -n "pSphere1Follicle6584" -p "hairSystem1Follicles";
	rename -uid "EE3FC81B-4300-3C42-CEA4-E9BBA6560047";
createNode follicle -n "pSphere1FollicleShape6584" -p "pSphere1Follicle6584";
	rename -uid "51AE3D8A-425A-0C7B-9921-C997AA0B8CFE";
	setAttr -k off ".v";
	setAttr ".pu" 0.65625;
	setAttr ".pv" 0.84375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve163" -p "pSphere1Follicle6584";
	rename -uid "D708C60D-4586-A3C2-B4A8-689EE11D0B90";
createNode nurbsCurve -n "curveShape163" -p "curve163";
	rename -uid "C2C21769-43D8-E622-1ADD-5791880FF245";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
		0 0 0.66666666669999997
		0 0 1
		0 0 1.3333333329999999
		0 0 1.6666666670000001
		0 0 2
		0 0 2.3333333330000001
		0 0 2.6666666669999999
		0 0 3
		;
createNode transform -n "pSphere1Follicle6590" -p "hairSystem1Follicles";
	rename -uid "7982E66A-4E54-87D2-A6D9-A68BC1E5BCF3";
createNode follicle -n "pSphere1FollicleShape6590" -p "pSphere1Follicle6590";
	rename -uid "F24DB7D3-4D96-BE13-F3F0-1B8773FD3227";
	setAttr -k off ".v";
	setAttr ".pu" 0.65625;
	setAttr ".pv" 0.90625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve164" -p "pSphere1Follicle6590";
	rename -uid "6A9DFC87-4C9C-8760-7502-04BA909A9520";
createNode nurbsCurve -n "curveShape164" -p "curve164";
	rename -uid "41FBDF1A-4711-7EE9-7165-2B947D5B5D75";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
		0 0 0.66666666669999997
		0 0 1
		0 0 1.3333333329999999
		0 0 1.6666666670000001
		0 0 2
		0 0 2.3333333330000001
		0 0 2.6666666669999999
		0 0 3
		;
createNode transform -n "pSphere1Follicle7103" -p "hairSystem1Follicles";
	rename -uid "9FE82F35-48B0-2DB3-43F4-6DAE7B85988B";
createNode follicle -n "pSphere1FollicleShape7103" -p "pSphere1Follicle7103";
	rename -uid "629D014F-41FB-3F6E-D938-99A97632EAEF";
	setAttr -k off ".v";
	setAttr ".pu" 0.71875;
	setAttr ".pv" 0.03125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve165" -p "pSphere1Follicle7103";
	rename -uid "851210A7-4F66-1349-CADA-1AAF46EFEC9D";
createNode nurbsCurve -n "curveShape165" -p "curve165";
	rename -uid "F35240F6-4894-8E91-DF76-6986D6A674F5";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
		0 0 0.66666666669999997
		0 0 1
		0 0 1.3333333329999999
		0 0 1.6666666670000001
		0 0 2
		0 0 2.3333333330000001
		0 0 2.6666666669999999
		0 0 3
		;
createNode transform -n "pSphere1Follicle7109" -p "hairSystem1Follicles";
	rename -uid "749FD4F7-4439-0BEF-E49F-038528A64A64";
createNode follicle -n "pSphere1FollicleShape7109" -p "pSphere1Follicle7109";
	rename -uid "F5FC026B-4F97-0684-0121-6F97877C4727";
	setAttr -k off ".v";
	setAttr ".pu" 0.71875;
	setAttr ".pv" 0.09375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve166" -p "pSphere1Follicle7109";
	rename -uid "9A4BC376-48D9-CB89-3487-99B8E0D5DC24";
createNode nurbsCurve -n "curveShape166" -p "curve166";
	rename -uid "3E5E04C5-41A9-4848-2C6F-CFBAD1F36E7D";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
		0 0 0.66666666669999997
		0 0 1
		0 0 1.3333333329999999
		0 0 1.6666666670000001
		0 0 2
		0 0 2.3333333330000001
		0 0 2.6666666669999999
		0 0 3
		;
createNode transform -n "pSphere1Follicle7115" -p "hairSystem1Follicles";
	rename -uid "D8A07A7D-40AE-3721-622A-DC8F6568F024";
createNode follicle -n "pSphere1FollicleShape7115" -p "pSphere1Follicle7115";
	rename -uid "142F1718-4C2C-8826-9C2E-BA9C59E1E59F";
	setAttr -k off ".v";
	setAttr ".pu" 0.71875;
	setAttr ".pv" 0.15625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve167" -p "pSphere1Follicle7115";
	rename -uid "E5D3E59C-44E0-2AAB-4A66-69899FE41988";
createNode nurbsCurve -n "curveShape167" -p "curve167";
	rename -uid "76235893-43B8-440A-55DE-7DBFEB79AFE3";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
		0 0 0.66666666669999997
		0 0 1
		0 0 1.3333333329999999
		0 0 1.6666666670000001
		0 0 2
		0 0 2.3333333330000001
		0 0 2.6666666669999999
		0 0 3
		;
createNode transform -n "pSphere1Follicle7122" -p "hairSystem1Follicles";
	rename -uid "2A0C56E4-4E0A-CA34-3E8E-37A13116D58C";
createNode follicle -n "pSphere1FollicleShape7122" -p "pSphere1Follicle7122";
	rename -uid "A2120867-45A3-9491-A6A8-1EB180C4EEAF";
	setAttr -k off ".v";
	setAttr ".pu" 0.71875;
	setAttr ".pv" 0.21875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve168" -p "pSphere1Follicle7122";
	rename -uid "35422888-4AE4-2259-4149-ED9018B35762";
createNode nurbsCurve -n "curveShape168" -p "curve168";
	rename -uid "66790843-4B64-0E2A-3CA4-C8848468CFEB";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
		0 0 0.66666666669999997
		0 0 1
		0 0 1.3333333329999999
		0 0 1.6666666670000001
		0 0 2
		0 0 2.3333333330000001
		0 0 2.6666666669999999
		0 0 3
		;
createNode transform -n "pSphere1Follicle7128" -p "hairSystem1Follicles";
	rename -uid "E4D72D6C-4CD5-908D-C6FE-D9A2B982DDFE";
createNode follicle -n "pSphere1FollicleShape7128" -p "pSphere1Follicle7128";
	rename -uid "705B3662-4C8C-1ACB-F5FA-3BB6271208CA";
	setAttr -k off ".v";
	setAttr ".pu" 0.71875;
	setAttr ".pv" 0.28125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve169" -p "pSphere1Follicle7128";
	rename -uid "0ED49561-4838-81D6-71B5-24B7E61CB775";
createNode nurbsCurve -n "curveShape169" -p "curve169";
	rename -uid "974F2D18-4412-A499-72F8-368C40C99210";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
		0 0 0.66666666669999997
		0 0 1
		0 0 1.3333333329999999
		0 0 1.6666666670000001
		0 0 2
		0 0 2.3333333330000001
		0 0 2.6666666669999999
		0 0 3
		;
createNode transform -n "pSphere1Follicle7134" -p "hairSystem1Follicles";
	rename -uid "B55EA959-49A2-5019-D26F-03A8B126B6D2";
createNode follicle -n "pSphere1FollicleShape7134" -p "pSphere1Follicle7134";
	rename -uid "5356593F-424F-15F9-3093-069B1954091C";
	setAttr -k off ".v";
	setAttr ".pu" 0.71875;
	setAttr ".pv" 0.34375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve170" -p "pSphere1Follicle7134";
	rename -uid "5181B75E-436A-ACA2-B9B9-F5AE20311449";
createNode nurbsCurve -n "curveShape170" -p "curve170";
	rename -uid "D440B26A-424F-D67A-8B18-52A16A8134FC";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
		0 0 0.66666666669999997
		0 0 1
		0 0 1.3333333329999999
		0 0 1.6666666670000001
		0 0 2
		0 0 2.3333333330000001
		0 0 2.6666666669999999
		0 0 3
		;
createNode transform -n "pSphere1Follicle7140" -p "hairSystem1Follicles";
	rename -uid "48691828-44F6-CCA3-A4CF-6A8F2821E0EC";
createNode follicle -n "pSphere1FollicleShape7140" -p "pSphere1Follicle7140";
	rename -uid "F004D8DF-4AB6-F67B-A126-CDAD64AD1520";
	setAttr -k off ".v";
	setAttr ".pu" 0.71875;
	setAttr ".pv" 0.40625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve171" -p "pSphere1Follicle7140";
	rename -uid "1CCF31EB-4C1D-679F-A8F9-EB81966B5893";
createNode nurbsCurve -n "curveShape171" -p "curve171";
	rename -uid "AD46B1A3-4BDF-C9FB-0B20-DA88AC359539";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
		0 0 0.66666666669999997
		0 0 1
		0 0 1.3333333329999999
		0 0 1.6666666670000001
		0 0 2
		0 0 2.3333333330000001
		0 0 2.6666666669999999
		0 0 3
		;
createNode transform -n "pSphere1Follicle7146" -p "hairSystem1Follicles";
	rename -uid "C88D5337-4348-2E37-A517-5791C883DA7E";
createNode follicle -n "pSphere1FollicleShape7146" -p "pSphere1Follicle7146";
	rename -uid "4D82EBE1-48F4-7C74-C430-6193394FAD83";
	setAttr -k off ".v";
	setAttr ".pu" 0.71875;
	setAttr ".pv" 0.46875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve172" -p "pSphere1Follicle7146";
	rename -uid "C010F2C1-4059-3B7D-9170-96B2720A0629";
createNode nurbsCurve -n "curveShape172" -p "curve172";
	rename -uid "620611F2-46DF-9E80-F112-ACAEF3AC69E9";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
		0 0 0.66666666669999997
		0 0 1
		0 0 1.3333333329999999
		0 0 1.6666666670000001
		0 0 2
		0 0 2.3333333330000001
		0 0 2.6666666669999999
		0 0 3
		;
createNode transform -n "pSphere1Follicle7153" -p "hairSystem1Follicles";
	rename -uid "E074E607-4D58-8777-EA77-28B812A65918";
createNode follicle -n "pSphere1FollicleShape7153" -p "pSphere1Follicle7153";
	rename -uid "61CE7A5A-4D56-686E-9B61-078E71CD28D1";
	setAttr -k off ".v";
	setAttr ".pu" 0.71875;
	setAttr ".pv" 0.53125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve173" -p "pSphere1Follicle7153";
	rename -uid "9B86187B-45A9-EF3A-42B9-2484A0C02E0C";
createNode nurbsCurve -n "curveShape173" -p "curve173";
	rename -uid "2B98DD08-4005-DF07-B3FB-509B21BF2F93";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
		0 0 0.66666666669999997
		0 0 1
		0 0 1.3333333329999999
		0 0 1.6666666670000001
		0 0 2
		0 0 2.3333333330000001
		0 0 2.6666666669999999
		0 0 3
		;
createNode transform -n "pSphere1Follicle7159" -p "hairSystem1Follicles";
	rename -uid "F78D37D4-40C4-E9AE-DA38-39A7ADBB9BC6";
createNode follicle -n "pSphere1FollicleShape7159" -p "pSphere1Follicle7159";
	rename -uid "096550AF-43D8-ED87-6BD8-C392BA449D06";
	setAttr -k off ".v";
	setAttr ".pu" 0.71875;
	setAttr ".pv" 0.59375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve174" -p "pSphere1Follicle7159";
	rename -uid "34B2D71E-49BF-B184-383F-038D86A342C5";
createNode nurbsCurve -n "curveShape174" -p "curve174";
	rename -uid "EFC9C220-4179-961E-7BD8-CCAD9A4D19AE";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
		0 0 0.66666666669999997
		0 0 1
		0 0 1.3333333329999999
		0 0 1.6666666670000001
		0 0 2
		0 0 2.3333333330000001
		0 0 2.6666666669999999
		0 0 3
		;
createNode transform -n "pSphere1Follicle7165" -p "hairSystem1Follicles";
	rename -uid "426671A4-4532-F068-5FB0-2D874129CF1D";
createNode follicle -n "pSphere1FollicleShape7165" -p "pSphere1Follicle7165";
	rename -uid "0366A313-401F-3C2F-FA59-23B0CBE27319";
	setAttr -k off ".v";
	setAttr ".pu" 0.71875;
	setAttr ".pv" 0.65625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve175" -p "pSphere1Follicle7165";
	rename -uid "87268CEB-4164-16AC-363D-A4A559AE52C4";
createNode nurbsCurve -n "curveShape175" -p "curve175";
	rename -uid "A71526C9-4D21-A776-8FF6-A2B262272329";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
		0 0 0.66666666669999997
		0 0 1
		0 0 1.3333333329999999
		0 0 1.6666666670000001
		0 0 2
		0 0 2.3333333330000001
		0 0 2.6666666669999999
		0 0 3
		;
createNode transform -n "pSphere1Follicle7171" -p "hairSystem1Follicles";
	rename -uid "C481215C-4AE5-7A07-2CDD-5DA90D6A0BFB";
createNode follicle -n "pSphere1FollicleShape7171" -p "pSphere1Follicle7171";
	rename -uid "2DAA4775-41BA-D3C7-7F0B-B5AFAC370547";
	setAttr -k off ".v";
	setAttr ".pu" 0.71875;
	setAttr ".pv" 0.71875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve176" -p "pSphere1Follicle7171";
	rename -uid "27750E31-4860-91DE-AB61-B1BA7CB5773F";
createNode nurbsCurve -n "curveShape176" -p "curve176";
	rename -uid "3FE91A03-4DB7-0EEE-C6D8-6388DDAE0D60";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
		0 0 0.66666666669999997
		0 0 1
		0 0 1.3333333329999999
		0 0 1.6666666670000001
		0 0 2
		0 0 2.3333333330000001
		0 0 2.6666666669999999
		0 0 3
		;
createNode transform -n "pSphere1Follicle7177" -p "hairSystem1Follicles";
	rename -uid "3F7B4CDC-4022-D8E0-E91D-1FBCD0C04DD8";
createNode follicle -n "pSphere1FollicleShape7177" -p "pSphere1Follicle7177";
	rename -uid "8E56E70E-4905-ED96-8186-D8B2C2E40108";
	setAttr -k off ".v";
	setAttr ".pu" 0.71875;
	setAttr ".pv" 0.78125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve177" -p "pSphere1Follicle7177";
	rename -uid "0E84905E-4AE4-8ADD-E548-68B1E4FF652F";
createNode nurbsCurve -n "curveShape177" -p "curve177";
	rename -uid "DC8DB7EE-4E21-3628-55CF-2098CE519A04";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
		0 0 0.66666666669999997
		0 0 1
		0 0 1.3333333329999999
		0 0 1.6666666670000001
		0 0 2
		0 0 2.3333333330000001
		0 0 2.6666666669999999
		0 0 3
		;
createNode transform -n "pSphere1Follicle7184" -p "hairSystem1Follicles";
	rename -uid "F94B4ECC-4343-6119-C640-DDADECC4E6FE";
createNode follicle -n "pSphere1FollicleShape7184" -p "pSphere1Follicle7184";
	rename -uid "FFA93981-475A-DB02-BB4E-A28A1341F989";
	setAttr -k off ".v";
	setAttr ".pu" 0.71875;
	setAttr ".pv" 0.84375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve178" -p "pSphere1Follicle7184";
	rename -uid "9542F09C-4815-C735-45EC-449843268D66";
createNode nurbsCurve -n "curveShape178" -p "curve178";
	rename -uid "7C30779F-40E3-790E-11C4-5298813128E8";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
		0 0 0.66666666669999997
		0 0 1
		0 0 1.3333333329999999
		0 0 1.6666666670000001
		0 0 2
		0 0 2.3333333330000001
		0 0 2.6666666669999999
		0 0 3
		;
createNode transform -n "pSphere1Follicle7190" -p "hairSystem1Follicles";
	rename -uid "6ED0FA63-4FAE-145C-20C7-E3BE73951D6F";
createNode follicle -n "pSphere1FollicleShape7190" -p "pSphere1Follicle7190";
	rename -uid "14DD6BD9-4444-61CF-E8AD-BEB73F36B6E9";
	setAttr -k off ".v";
	setAttr ".pu" 0.71875;
	setAttr ".pv" 0.90625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve179" -p "pSphere1Follicle7190";
	rename -uid "4E157843-452C-A1FA-40CF-1899B1D233A1";
createNode nurbsCurve -n "curveShape179" -p "curve179";
	rename -uid "EA5B8730-4DA7-9FE5-A25E-78BD61223264";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
		0 0 0.66666666669999997
		0 0 1
		0 0 1.3333333329999999
		0 0 1.6666666670000001
		0 0 2
		0 0 2.3333333330000001
		0 0 2.6666666669999999
		0 0 3
		;
createNode transform -n "pSphere1Follicle7196" -p "hairSystem1Follicles";
	rename -uid "31241237-4D08-4D58-0FD0-B896CBD8070B";
createNode follicle -n "pSphere1FollicleShape7196" -p "pSphere1Follicle7196";
	rename -uid "0469A69C-4CC9-3B9A-B38B-B3B7807DD6B4";
	setAttr -k off ".v";
	setAttr ".pu" 0.71875;
	setAttr ".pv" 0.96875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve180" -p "pSphere1Follicle7196";
	rename -uid "78A0C6DA-4B19-FE34-AFF2-3FB48F0B95A3";
createNode nurbsCurve -n "curveShape180" -p "curve180";
	rename -uid "28E7811F-4B9A-A439-9FDB-1E9CAC6FA7B3";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
		0 0 0.66666666669999997
		0 0 1
		0 0 1.3333333329999999
		0 0 1.6666666670000001
		0 0 2
		0 0 2.3333333330000001
		0 0 2.6666666669999999
		0 0 3
		;
createNode transform -n "pSphere1Follicle7703" -p "hairSystem1Follicles";
	rename -uid "FC78424B-4D1E-DA74-C3D2-99A2F5A5C031";
createNode follicle -n "pSphere1FollicleShape7703" -p "pSphere1Follicle7703";
	rename -uid "F4619E38-4B86-FA22-7BF9-419D1788D223";
	setAttr -k off ".v";
	setAttr ".pu" 0.78125;
	setAttr ".pv" 0.03125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve181" -p "pSphere1Follicle7703";
	rename -uid "EF2C0D3D-44CF-CB89-6CE1-408163BFF6D9";
createNode nurbsCurve -n "curveShape181" -p "curve181";
	rename -uid "544B5226-4CE5-AA5C-7BC0-EC985A7AC616";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
		0 0 0.66666666669999997
		0 0 1
		0 0 1.3333333329999999
		0 0 1.6666666670000001
		0 0 2
		0 0 2.3333333330000001
		0 0 2.6666666669999999
		0 0 3
		;
createNode transform -n "pSphere1Follicle7709" -p "hairSystem1Follicles";
	rename -uid "B71F654A-4E14-97B3-04FB-8A919D8697DE";
createNode follicle -n "pSphere1FollicleShape7709" -p "pSphere1Follicle7709";
	rename -uid "BC517C87-4122-921A-6A52-CEB306FF7484";
	setAttr -k off ".v";
	setAttr ".pu" 0.78125;
	setAttr ".pv" 0.09375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve182" -p "pSphere1Follicle7709";
	rename -uid "F1CA4A96-47CF-FA9A-7283-409A95E60934";
createNode nurbsCurve -n "curveShape182" -p "curve182";
	rename -uid "63AC3078-4BF0-E9FD-4F4D-10822469B40F";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
		0 0 0.66666666669999997
		0 0 1
		0 0 1.3333333329999999
		0 0 1.6666666670000001
		0 0 2
		0 0 2.3333333330000001
		0 0 2.6666666669999999
		0 0 3
		;
createNode transform -n "pSphere1Follicle7715" -p "hairSystem1Follicles";
	rename -uid "C0DDEE2F-4BA0-27F2-7866-25B2BFF0CE34";
createNode follicle -n "pSphere1FollicleShape7715" -p "pSphere1Follicle7715";
	rename -uid "156A4E09-4C22-CDAF-1658-02AD853EFABE";
	setAttr -k off ".v";
	setAttr ".pu" 0.78125;
	setAttr ".pv" 0.15625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve183" -p "pSphere1Follicle7715";
	rename -uid "045BEEF2-4F37-EF30-F0A8-778F094DD60E";
createNode nurbsCurve -n "curveShape183" -p "curve183";
	rename -uid "90024D0B-423D-D5CC-0439-98A7A47E6811";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
		0 0 0.66666666669999997
		0 0 1
		0 0 1.3333333329999999
		0 0 1.6666666670000001
		0 0 2
		0 0 2.3333333330000001
		0 0 2.6666666669999999
		0 0 3
		;
createNode transform -n "pSphere1Follicle7722" -p "hairSystem1Follicles";
	rename -uid "763D4C90-44E5-C690-2CC7-C59419388144";
createNode follicle -n "pSphere1FollicleShape7722" -p "pSphere1Follicle7722";
	rename -uid "4B78BE64-4585-656C-D321-AD8449709C6B";
	setAttr -k off ".v";
	setAttr ".pu" 0.78125;
	setAttr ".pv" 0.21875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve184" -p "pSphere1Follicle7722";
	rename -uid "9D9D0ACE-4631-DE6D-505A-4599466E55DC";
createNode nurbsCurve -n "curveShape184" -p "curve184";
	rename -uid "36020187-4A2F-F82D-2C6B-84BBF2022826";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
		0 0 0.66666666669999997
		0 0 1
		0 0 1.3333333329999999
		0 0 1.6666666670000001
		0 0 2
		0 0 2.3333333330000001
		0 0 2.6666666669999999
		0 0 3
		;
createNode transform -n "pSphere1Follicle7728" -p "hairSystem1Follicles";
	rename -uid "DAAEEFEA-4C06-DE93-75B7-07AAAD9B6021";
createNode follicle -n "pSphere1FollicleShape7728" -p "pSphere1Follicle7728";
	rename -uid "419FF040-4526-A6C3-5F62-46B6733D6662";
	setAttr -k off ".v";
	setAttr ".pu" 0.78125;
	setAttr ".pv" 0.28125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve185" -p "pSphere1Follicle7728";
	rename -uid "0786F0B1-4669-3BEE-69AA-76864E889501";
createNode nurbsCurve -n "curveShape185" -p "curve185";
	rename -uid "326FE4EC-4DB1-9A11-1635-D5BA957CE84A";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
		0 0 0.66666666669999997
		0 0 1
		0 0 1.3333333329999999
		0 0 1.6666666670000001
		0 0 2
		0 0 2.3333333330000001
		0 0 2.6666666669999999
		0 0 3
		;
createNode transform -n "pSphere1Follicle7734" -p "hairSystem1Follicles";
	rename -uid "D4A7EA9F-4837-9272-BE30-2999ADAD643A";
createNode follicle -n "pSphere1FollicleShape7734" -p "pSphere1Follicle7734";
	rename -uid "4006BC00-416D-5EEF-EE74-70ACF0D32C63";
	setAttr -k off ".v";
	setAttr ".pu" 0.78125;
	setAttr ".pv" 0.34375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve186" -p "pSphere1Follicle7734";
	rename -uid "805E70D6-407E-B490-D211-88A552ABF898";
createNode nurbsCurve -n "curveShape186" -p "curve186";
	rename -uid "31222B34-4C11-0A72-E579-19AB124DF897";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
		0 0 0.66666666669999997
		0 0 1
		0 0 1.3333333329999999
		0 0 1.6666666670000001
		0 0 2
		0 0 2.3333333330000001
		0 0 2.6666666669999999
		0 0 3
		;
createNode transform -n "pSphere1Follicle7740" -p "hairSystem1Follicles";
	rename -uid "4A59993B-4AC9-16C4-DE9F-83820C1E9CB4";
createNode follicle -n "pSphere1FollicleShape7740" -p "pSphere1Follicle7740";
	rename -uid "990F3F15-40B0-C8AA-D466-1B981A13C83A";
	setAttr -k off ".v";
	setAttr ".pu" 0.78125;
	setAttr ".pv" 0.40625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve187" -p "pSphere1Follicle7740";
	rename -uid "CA885156-437D-E1F4-A5A2-75BF9613EB2E";
createNode nurbsCurve -n "curveShape187" -p "curve187";
	rename -uid "592D8C00-4B51-7A32-2EBE-FEAEE8C1098F";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
		0 0 0.66666666669999997
		0 0 1
		0 0 1.3333333329999999
		0 0 1.6666666670000001
		0 0 2
		0 0 2.3333333330000001
		0 0 2.6666666669999999
		0 0 3
		;
createNode transform -n "pSphere1Follicle7746" -p "hairSystem1Follicles";
	rename -uid "BBC03A53-4CB3-2D8F-DCDA-2BB75884C511";
createNode follicle -n "pSphere1FollicleShape7746" -p "pSphere1Follicle7746";
	rename -uid "140D7494-4146-9876-7376-1EB5BC5A88B7";
	setAttr -k off ".v";
	setAttr ".pu" 0.78125;
	setAttr ".pv" 0.46875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve188" -p "pSphere1Follicle7746";
	rename -uid "1F56F9D3-4C7C-A3D8-9CED-EE9DF6FAF1ED";
createNode nurbsCurve -n "curveShape188" -p "curve188";
	rename -uid "380921EE-405D-10C3-24EE-C08A52B01098";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
		0 0 0.66666666669999997
		0 0 1
		0 0 1.3333333329999999
		0 0 1.6666666670000001
		0 0 2
		0 0 2.3333333330000001
		0 0 2.6666666669999999
		0 0 3
		;
createNode transform -n "pSphere1Follicle7753" -p "hairSystem1Follicles";
	rename -uid "B541FF82-4455-115A-FC7C-E38BCE02C118";
createNode follicle -n "pSphere1FollicleShape7753" -p "pSphere1Follicle7753";
	rename -uid "6167B10F-4889-768C-FD77-F590AFCF6105";
	setAttr -k off ".v";
	setAttr ".pu" 0.78125;
	setAttr ".pv" 0.53125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve189" -p "pSphere1Follicle7753";
	rename -uid "66BCDF3C-4586-1DF1-401C-54A328116EF8";
createNode nurbsCurve -n "curveShape189" -p "curve189";
	rename -uid "B1496F87-4336-6A73-6270-AA9238290495";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
		0 0 0.66666666669999997
		0 0 1
		0 0 1.3333333329999999
		0 0 1.6666666670000001
		0 0 2
		0 0 2.3333333330000001
		0 0 2.6666666669999999
		0 0 3
		;
createNode transform -n "pSphere1Follicle7759" -p "hairSystem1Follicles";
	rename -uid "232DD8C2-4E3E-2567-D1C4-EABE5B7F4538";
createNode follicle -n "pSphere1FollicleShape7759" -p "pSphere1Follicle7759";
	rename -uid "DCAF8D32-4162-9499-47B1-52A1867280B5";
	setAttr -k off ".v";
	setAttr ".pu" 0.78125;
	setAttr ".pv" 0.59375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve190" -p "pSphere1Follicle7759";
	rename -uid "6F555C9F-41CC-92C9-07A9-40BF58C94537";
createNode nurbsCurve -n "curveShape190" -p "curve190";
	rename -uid "DB8A159C-4905-9504-BD91-32AC2FF41C61";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
		0 0 0.66666666669999997
		0 0 1
		0 0 1.3333333329999999
		0 0 1.6666666670000001
		0 0 2
		0 0 2.3333333330000001
		0 0 2.6666666669999999
		0 0 3
		;
createNode transform -n "pSphere1Follicle7765" -p "hairSystem1Follicles";
	rename -uid "4568EA36-49C9-449B-F609-319DAF25C64F";
createNode follicle -n "pSphere1FollicleShape7765" -p "pSphere1Follicle7765";
	rename -uid "C1070527-4237-6FF0-5A82-E8A1DE3D9A86";
	setAttr -k off ".v";
	setAttr ".pu" 0.78125;
	setAttr ".pv" 0.65625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve191" -p "pSphere1Follicle7765";
	rename -uid "31ABEB80-4FEC-525A-2A8B-8BA1EB2A114D";
createNode nurbsCurve -n "curveShape191" -p "curve191";
	rename -uid "5992AE27-438B-33E7-295E-C892FA66E2C8";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
		0 0 0.66666666669999997
		0 0 1
		0 0 1.3333333329999999
		0 0 1.6666666670000001
		0 0 2
		0 0 2.3333333330000001
		0 0 2.6666666669999999
		0 0 3
		;
createNode transform -n "pSphere1Follicle7771" -p "hairSystem1Follicles";
	rename -uid "7989C19F-4750-D7BD-956E-7895A34FE574";
createNode follicle -n "pSphere1FollicleShape7771" -p "pSphere1Follicle7771";
	rename -uid "B49F6BA5-4F16-06FB-9514-9195503EE0F6";
	setAttr -k off ".v";
	setAttr ".pu" 0.78125;
	setAttr ".pv" 0.71875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve192" -p "pSphere1Follicle7771";
	rename -uid "BE897779-4A6D-CC83-28BB-1FA6B31FD85D";
createNode nurbsCurve -n "curveShape192" -p "curve192";
	rename -uid "63AF0E91-48C5-A8D7-3B04-AE834347E7A4";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
		0 0 0.66666666669999997
		0 0 1
		0 0 1.3333333329999999
		0 0 1.6666666670000001
		0 0 2
		0 0 2.3333333330000001
		0 0 2.6666666669999999
		0 0 3
		;
createNode transform -n "pSphere1Follicle7777" -p "hairSystem1Follicles";
	rename -uid "D00B7CAE-4EFD-A699-FB2A-FCA16F707AB8";
createNode follicle -n "pSphere1FollicleShape7777" -p "pSphere1Follicle7777";
	rename -uid "FB2DDE29-4C9D-9113-964C-EE9F9EA7760C";
	setAttr -k off ".v";
	setAttr ".pu" 0.78125;
	setAttr ".pv" 0.78125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve193" -p "pSphere1Follicle7777";
	rename -uid "40512589-45D6-0B0F-9D42-16B7056A7E41";
createNode nurbsCurve -n "curveShape193" -p "curve193";
	rename -uid "DCFCF579-4C6F-6AAC-B005-4E809126F21A";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
		0 0 0.66666666669999997
		0 0 1
		0 0 1.3333333329999999
		0 0 1.6666666670000001
		0 0 2
		0 0 2.3333333330000001
		0 0 2.6666666669999999
		0 0 3
		;
createNode transform -n "pSphere1Follicle7784" -p "hairSystem1Follicles";
	rename -uid "4643BFBC-43CE-9A7A-A0A3-FBAFD30F29F2";
createNode follicle -n "pSphere1FollicleShape7784" -p "pSphere1Follicle7784";
	rename -uid "2921AFC0-44FC-6040-4C0F-8A9EA40D59D5";
	setAttr -k off ".v";
	setAttr ".pu" 0.78125;
	setAttr ".pv" 0.84375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve194" -p "pSphere1Follicle7784";
	rename -uid "36EE2A24-4FD2-7705-157E-29A4E63C8DF3";
createNode nurbsCurve -n "curveShape194" -p "curve194";
	rename -uid "4E353DA6-44D7-BFE1-83CD-DE933729EDA8";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
		0 0 0.66666666669999997
		0 0 1
		0 0 1.3333333329999999
		0 0 1.6666666670000001
		0 0 2
		0 0 2.3333333330000001
		0 0 2.6666666669999999
		0 0 3
		;
createNode transform -n "pSphere1Follicle7790" -p "hairSystem1Follicles";
	rename -uid "68D889DD-4672-C813-8927-49B3EAF494BB";
createNode follicle -n "pSphere1FollicleShape7790" -p "pSphere1Follicle7790";
	rename -uid "B0BEF8FC-418D-7072-3C04-3791BC90F5E9";
	setAttr -k off ".v";
	setAttr ".pu" 0.78125;
	setAttr ".pv" 0.90625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve195" -p "pSphere1Follicle7790";
	rename -uid "31853C68-4C2A-009B-5EFD-22A16F8A068C";
createNode nurbsCurve -n "curveShape195" -p "curve195";
	rename -uid "117B86D0-4CCF-B61D-EE44-5FB067195402";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
		0 0 0.66666666669999997
		0 0 1
		0 0 1.3333333329999999
		0 0 1.6666666670000001
		0 0 2
		0 0 2.3333333330000001
		0 0 2.6666666669999999
		0 0 3
		;
createNode transform -n "pSphere1Follicle7796" -p "hairSystem1Follicles";
	rename -uid "F9FCC003-4185-BA06-96FF-A9A33F3064FE";
createNode follicle -n "pSphere1FollicleShape7796" -p "pSphere1Follicle7796";
	rename -uid "8620B403-41E7-52A7-F437-A5B756A0A45C";
	setAttr -k off ".v";
	setAttr ".pu" 0.78125;
	setAttr ".pv" 0.96875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve196" -p "pSphere1Follicle7796";
	rename -uid "34EB0C1D-4F17-0EC7-8EDD-1CBC165B6EB4";
createNode nurbsCurve -n "curveShape196" -p "curve196";
	rename -uid "6D014782-43FA-DB24-4322-639CD0D5C576";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
		0 0 0.66666666669999997
		0 0 1
		0 0 1.3333333329999999
		0 0 1.6666666670000001
		0 0 2
		0 0 2.3333333330000001
		0 0 2.6666666669999999
		0 0 3
		;
createNode transform -n "pSphere1Follicle8409" -p "hairSystem1Follicles";
	rename -uid "0CB21711-4ED2-876C-E777-4391BB4D714A";
createNode follicle -n "pSphere1FollicleShape8409" -p "pSphere1Follicle8409";
	rename -uid "CFE31227-4448-2EA6-5416-59A8F9062F02";
	setAttr -k off ".v";
	setAttr ".pu" 0.84375;
	setAttr ".pv" 0.09375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve197" -p "pSphere1Follicle8409";
	rename -uid "221ACB27-4300-F6AE-642E-FBA3936AD08A";
createNode nurbsCurve -n "curveShape197" -p "curve197";
	rename -uid "D0912537-400E-BD6C-D412-BBA4C34C46D2";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
		0 0 0.66666666669999997
		0 0 1
		0 0 1.3333333329999999
		0 0 1.6666666670000001
		0 0 2
		0 0 2.3333333330000001
		0 0 2.6666666669999999
		0 0 3
		;
createNode transform -n "pSphere1Follicle8415" -p "hairSystem1Follicles";
	rename -uid "03D6B199-4036-FA9B-5F39-67AD7BB31B1E";
createNode follicle -n "pSphere1FollicleShape8415" -p "pSphere1Follicle8415";
	rename -uid "FCD39001-4FEC-F5C3-306A-789A0D22D475";
	setAttr -k off ".v";
	setAttr ".pu" 0.84375;
	setAttr ".pv" 0.15625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve198" -p "pSphere1Follicle8415";
	rename -uid "F4C25BF9-419C-FF3D-DC77-BB995488C429";
createNode nurbsCurve -n "curveShape198" -p "curve198";
	rename -uid "C4949D76-43DE-8EF5-E476-B68C07A4DDDB";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
		0 0 0.66666666669999997
		0 0 1
		0 0 1.3333333329999999
		0 0 1.6666666670000001
		0 0 2
		0 0 2.3333333330000001
		0 0 2.6666666669999999
		0 0 3
		;
createNode transform -n "pSphere1Follicle8422" -p "hairSystem1Follicles";
	rename -uid "418D7E3E-4D28-6D0A-B9CF-4A9C90DBC7FA";
createNode follicle -n "pSphere1FollicleShape8422" -p "pSphere1Follicle8422";
	rename -uid "ED72F0F9-4DBF-0E0F-9759-3B86B9DF26C7";
	setAttr -k off ".v";
	setAttr ".pu" 0.84375;
	setAttr ".pv" 0.21875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve199" -p "pSphere1Follicle8422";
	rename -uid "59C8B726-4529-10C4-2AF0-3897BE138562";
createNode nurbsCurve -n "curveShape199" -p "curve199";
	rename -uid "DFF0B200-424F-D377-C17F-BEB8FDE1DD47";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
		0 0 0.66666666669999997
		0 0 1
		0 0 1.3333333329999999
		0 0 1.6666666670000001
		0 0 2
		0 0 2.3333333330000001
		0 0 2.6666666669999999
		0 0 3
		;
createNode transform -n "pSphere1Follicle8428" -p "hairSystem1Follicles";
	rename -uid "507DAB61-4BDD-D505-F51A-8FA2CA9C750E";
createNode follicle -n "pSphere1FollicleShape8428" -p "pSphere1Follicle8428";
	rename -uid "37D237C6-4D25-DDBC-E988-75AB244ACA26";
	setAttr -k off ".v";
	setAttr ".pu" 0.84375;
	setAttr ".pv" 0.28125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve200" -p "pSphere1Follicle8428";
	rename -uid "F24FFEFA-4200-34C9-DF78-EF96B479D534";
createNode nurbsCurve -n "curveShape200" -p "curve200";
	rename -uid "5047716E-4AD5-C051-C606-F69DD9A3460A";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
		0 0 0.66666666669999997
		0 0 1
		0 0 1.3333333329999999
		0 0 1.6666666670000001
		0 0 2
		0 0 2.3333333330000001
		0 0 2.6666666669999999
		0 0 3
		;
createNode transform -n "pSphere1Follicle8434" -p "hairSystem1Follicles";
	rename -uid "55EB393F-4EF2-EF4B-237F-BBA3EFE54666";
createNode follicle -n "pSphere1FollicleShape8434" -p "pSphere1Follicle8434";
	rename -uid "75D517B9-4DD4-1C17-4F88-DEA9DC12CDFD";
	setAttr -k off ".v";
	setAttr ".pu" 0.84375;
	setAttr ".pv" 0.34375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve201" -p "pSphere1Follicle8434";
	rename -uid "56ADDA51-45E7-372F-5437-659FA9B898C1";
createNode nurbsCurve -n "curveShape201" -p "curve201";
	rename -uid "3878CDF0-4F3C-7274-87F8-9DAA5C7F021C";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
		0 0 0.66666666669999997
		0 0 1
		0 0 1.3333333329999999
		0 0 1.6666666670000001
		0 0 2
		0 0 2.3333333330000001
		0 0 2.6666666669999999
		0 0 3
		;
createNode transform -n "pSphere1Follicle8440" -p "hairSystem1Follicles";
	rename -uid "7FA18FA6-4ADD-0707-7911-559027800BA6";
createNode follicle -n "pSphere1FollicleShape8440" -p "pSphere1Follicle8440";
	rename -uid "E45F2D8A-43A9-47EF-889A-56A8AC26EE99";
	setAttr -k off ".v";
	setAttr ".pu" 0.84375;
	setAttr ".pv" 0.40625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve202" -p "pSphere1Follicle8440";
	rename -uid "F9E15C07-4438-B642-EA21-13944BF951BB";
createNode nurbsCurve -n "curveShape202" -p "curve202";
	rename -uid "8F27667C-4CCC-7987-8EE2-508A4CAB4CFF";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
		0 0 0.66666666669999997
		0 0 1
		0 0 1.3333333329999999
		0 0 1.6666666670000001
		0 0 2
		0 0 2.3333333330000001
		0 0 2.6666666669999999
		0 0 3
		;
createNode transform -n "pSphere1Follicle8446" -p "hairSystem1Follicles";
	rename -uid "BE63BEFB-4115-84CA-4BFE-0585C61339DA";
createNode follicle -n "pSphere1FollicleShape8446" -p "pSphere1Follicle8446";
	rename -uid "8C77F9C9-4975-8088-7610-22B4E2B85B04";
	setAttr -k off ".v";
	setAttr ".pu" 0.84375;
	setAttr ".pv" 0.46875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve203" -p "pSphere1Follicle8446";
	rename -uid "403553C9-47C7-AB6F-F087-4BAC859EEC5E";
createNode nurbsCurve -n "curveShape203" -p "curve203";
	rename -uid "3670825E-454B-31BC-D098-828D248EDA13";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
		0 0 0.66666666669999997
		0 0 1
		0 0 1.3333333329999999
		0 0 1.6666666670000001
		0 0 2
		0 0 2.3333333330000001
		0 0 2.6666666669999999
		0 0 3
		;
createNode transform -n "pSphere1Follicle8453" -p "hairSystem1Follicles";
	rename -uid "61BF6B38-49F6-35C6-8C3B-06A4E6264965";
createNode follicle -n "pSphere1FollicleShape8453" -p "pSphere1Follicle8453";
	rename -uid "ECBDBFBC-4E2D-B2E4-A59F-06ADD0D7B387";
	setAttr -k off ".v";
	setAttr ".pu" 0.84375;
	setAttr ".pv" 0.53125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve204" -p "pSphere1Follicle8453";
	rename -uid "77517854-46E2-F85D-1681-11810099AD99";
createNode nurbsCurve -n "curveShape204" -p "curve204";
	rename -uid "5E328BFF-4C33-5D8C-FD98-BAA8CA6A9EE3";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
		0 0 0.66666666669999997
		0 0 1
		0 0 1.3333333329999999
		0 0 1.6666666670000001
		0 0 2
		0 0 2.3333333330000001
		0 0 2.6666666669999999
		0 0 3
		;
createNode transform -n "pSphere1Follicle8459" -p "hairSystem1Follicles";
	rename -uid "79C7C2B9-46A1-8E57-1A40-2890AD1CD8FA";
createNode follicle -n "pSphere1FollicleShape8459" -p "pSphere1Follicle8459";
	rename -uid "82BCB74D-498A-23AD-4B37-ECBAE5F31575";
	setAttr -k off ".v";
	setAttr ".pu" 0.84375;
	setAttr ".pv" 0.59375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve205" -p "pSphere1Follicle8459";
	rename -uid "D6797696-40FE-77FD-6A43-AE8A9F0E07B7";
createNode nurbsCurve -n "curveShape205" -p "curve205";
	rename -uid "5654C824-4923-10C2-CA5A-9AB6485EE93E";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
		0 0 0.66666666669999997
		0 0 1
		0 0 1.3333333329999999
		0 0 1.6666666670000001
		0 0 2
		0 0 2.3333333330000001
		0 0 2.6666666669999999
		0 0 3
		;
createNode transform -n "pSphere1Follicle8465" -p "hairSystem1Follicles";
	rename -uid "351CBE55-4971-53E5-AFF9-1AB1086B9FCE";
createNode follicle -n "pSphere1FollicleShape8465" -p "pSphere1Follicle8465";
	rename -uid "4C522978-4F00-CDA8-C269-CD8ADA09C86D";
	setAttr -k off ".v";
	setAttr ".pu" 0.84375;
	setAttr ".pv" 0.65625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve206" -p "pSphere1Follicle8465";
	rename -uid "09D92F50-46D9-19B2-5C9E-5CBC17D57B27";
createNode nurbsCurve -n "curveShape206" -p "curve206";
	rename -uid "330A4152-43DD-61C2-144B-26A83FDEAB95";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
		0 0 0.66666666669999997
		0 0 1
		0 0 1.3333333329999999
		0 0 1.6666666670000001
		0 0 2
		0 0 2.3333333330000001
		0 0 2.6666666669999999
		0 0 3
		;
createNode transform -n "pSphere1Follicle8471" -p "hairSystem1Follicles";
	rename -uid "EAC58C98-4190-6EB2-0BBF-D28EF1AF6DFB";
createNode follicle -n "pSphere1FollicleShape8471" -p "pSphere1Follicle8471";
	rename -uid "27020124-4459-B250-A711-238F5385CC60";
	setAttr -k off ".v";
	setAttr ".pu" 0.84375;
	setAttr ".pv" 0.71875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve207" -p "pSphere1Follicle8471";
	rename -uid "2B14EFAD-47D1-223D-9AC5-EA8237CBEC54";
createNode nurbsCurve -n "curveShape207" -p "curve207";
	rename -uid "25621FE4-44F7-5BEF-96D6-B6A72C37DF81";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
		0 0 0.66666666669999997
		0 0 1
		0 0 1.3333333329999999
		0 0 1.6666666670000001
		0 0 2
		0 0 2.3333333330000001
		0 0 2.6666666669999999
		0 0 3
		;
createNode transform -n "pSphere1Follicle8477" -p "hairSystem1Follicles";
	rename -uid "32AA7527-4754-5EA3-2768-BDA6E48F26D0";
createNode follicle -n "pSphere1FollicleShape8477" -p "pSphere1Follicle8477";
	rename -uid "37538243-4D9C-8671-8E5B-328003D3F0D2";
	setAttr -k off ".v";
	setAttr ".pu" 0.84375;
	setAttr ".pv" 0.78125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve208" -p "pSphere1Follicle8477";
	rename -uid "B5031C99-4911-DC3C-D78A-318D98DD8B72";
createNode nurbsCurve -n "curveShape208" -p "curve208";
	rename -uid "9FFE416C-4752-45CA-9F57-81A220343F69";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
		0 0 0.66666666669999997
		0 0 1
		0 0 1.3333333329999999
		0 0 1.6666666670000001
		0 0 2
		0 0 2.3333333330000001
		0 0 2.6666666669999999
		0 0 3
		;
createNode transform -n "pSphere1Follicle8484" -p "hairSystem1Follicles";
	rename -uid "F3318007-443F-AD4F-1361-B88301071697";
createNode follicle -n "pSphere1FollicleShape8484" -p "pSphere1Follicle8484";
	rename -uid "88504228-457E-3B42-2256-998A68038C19";
	setAttr -k off ".v";
	setAttr ".pu" 0.84375;
	setAttr ".pv" 0.84375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve209" -p "pSphere1Follicle8484";
	rename -uid "C58CE4AA-4EC8-B537-26A8-C1B4D66B3F44";
createNode nurbsCurve -n "curveShape209" -p "curve209";
	rename -uid "30A36601-4062-368F-83FB-1FB66D59F158";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
		0 0 0.66666666669999997
		0 0 1
		0 0 1.3333333329999999
		0 0 1.6666666670000001
		0 0 2
		0 0 2.3333333330000001
		0 0 2.6666666669999999
		0 0 3
		;
createNode transform -n "pSphere1Follicle8490" -p "hairSystem1Follicles";
	rename -uid "FE3E8B48-4B24-FB11-A62E-089636164178";
createNode follicle -n "pSphere1FollicleShape8490" -p "pSphere1Follicle8490";
	rename -uid "D2C79852-4E3F-D6AA-FE6A-6F9A6A4483A0";
	setAttr -k off ".v";
	setAttr ".pu" 0.84375;
	setAttr ".pv" 0.90625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve210" -p "pSphere1Follicle8490";
	rename -uid "3738FF1B-4067-B9CA-40AF-8A92B9F74491";
createNode nurbsCurve -n "curveShape210" -p "curve210";
	rename -uid "B4C0EF4C-42A1-78DB-44A7-AAA1BBF9164C";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
		0 0 0.66666666669999997
		0 0 1
		0 0 1.3333333329999999
		0 0 1.6666666670000001
		0 0 2
		0 0 2.3333333330000001
		0 0 2.6666666669999999
		0 0 3
		;
createNode transform -n "pSphere1Follicle9009" -p "hairSystem1Follicles";
	rename -uid "42761B08-440F-B5CD-CF37-A1A47296B916";
createNode follicle -n "pSphere1FollicleShape9009" -p "pSphere1Follicle9009";
	rename -uid "3A93A56B-466C-70A6-0E13-DB959F33B599";
	setAttr -k off ".v";
	setAttr ".pu" 0.90625;
	setAttr ".pv" 0.09375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve211" -p "pSphere1Follicle9009";
	rename -uid "F5125BD5-4959-29A5-2A7A-50BB278AA814";
createNode nurbsCurve -n "curveShape211" -p "curve211";
	rename -uid "0C4D8895-49EC-7B11-406E-F2A85BB47828";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
		0 0 0.66666666669999997
		0 0 1
		0 0 1.3333333329999999
		0 0 1.6666666670000001
		0 0 2
		0 0 2.3333333330000001
		0 0 2.6666666669999999
		0 0 3
		;
createNode transform -n "pSphere1Follicle9015" -p "hairSystem1Follicles";
	rename -uid "DC0783A3-4B8F-799B-8B77-5AAF4585119C";
createNode follicle -n "pSphere1FollicleShape9015" -p "pSphere1Follicle9015";
	rename -uid "D77B44CB-41F6-878D-FF26-CEAF9080D892";
	setAttr -k off ".v";
	setAttr ".pu" 0.90625;
	setAttr ".pv" 0.15625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve212" -p "pSphere1Follicle9015";
	rename -uid "E7FCA8A7-4130-5669-7997-94AA5F77AF94";
createNode nurbsCurve -n "curveShape212" -p "curve212";
	rename -uid "2FE388C7-45A8-CE70-CBC5-B480FB0850CD";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
		0 0 0.66666666669999997
		0 0 1
		0 0 1.3333333329999999
		0 0 1.6666666670000001
		0 0 2
		0 0 2.3333333330000001
		0 0 2.6666666669999999
		0 0 3
		;
createNode transform -n "pSphere1Follicle9022" -p "hairSystem1Follicles";
	rename -uid "FFB6FB2C-4A09-5D71-258C-A8B48256473F";
createNode follicle -n "pSphere1FollicleShape9022" -p "pSphere1Follicle9022";
	rename -uid "4B981240-4C37-135A-CF7D-8496B49FD59D";
	setAttr -k off ".v";
	setAttr ".pu" 0.90625;
	setAttr ".pv" 0.21875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve213" -p "pSphere1Follicle9022";
	rename -uid "A84238CB-40B4-3955-2D06-C78FE28FF7E3";
createNode nurbsCurve -n "curveShape213" -p "curve213";
	rename -uid "2769903E-497F-71BD-0959-D8A8BC9EF35F";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
		0 0 0.66666666669999997
		0 0 1
		0 0 1.3333333329999999
		0 0 1.6666666670000001
		0 0 2
		0 0 2.3333333330000001
		0 0 2.6666666669999999
		0 0 3
		;
createNode transform -n "pSphere1Follicle9028" -p "hairSystem1Follicles";
	rename -uid "B9C17473-484B-8AF6-79CE-779A7A29A6FE";
createNode follicle -n "pSphere1FollicleShape9028" -p "pSphere1Follicle9028";
	rename -uid "0454EFD4-4B19-DBB7-705A-6D9CC8FB4428";
	setAttr -k off ".v";
	setAttr ".pu" 0.90625;
	setAttr ".pv" 0.28125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve214" -p "pSphere1Follicle9028";
	rename -uid "4A36A209-4EFD-C3BC-379F-2E858A4EE882";
createNode nurbsCurve -n "curveShape214" -p "curve214";
	rename -uid "9368023F-4061-E382-FAA7-B5BDB736D327";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
		0 0 0.66666666669999997
		0 0 1
		0 0 1.3333333329999999
		0 0 1.6666666670000001
		0 0 2
		0 0 2.3333333330000001
		0 0 2.6666666669999999
		0 0 3
		;
createNode transform -n "pSphere1Follicle9034" -p "hairSystem1Follicles";
	rename -uid "88FAE78A-450C-84C4-9914-099530D13B26";
createNode follicle -n "pSphere1FollicleShape9034" -p "pSphere1Follicle9034";
	rename -uid "7CC9F558-4DC9-9EFE-3E53-7AAF99D1F7AB";
	setAttr -k off ".v";
	setAttr ".pu" 0.90625;
	setAttr ".pv" 0.34375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve215" -p "pSphere1Follicle9034";
	rename -uid "3471D0AE-4ACE-F4C7-3F74-80B4B2F2E629";
createNode nurbsCurve -n "curveShape215" -p "curve215";
	rename -uid "5A95F9E5-4234-F795-EBC8-30BCE4EBCA63";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
		0 0 0.66666666669999997
		0 0 1
		0 0 1.3333333329999999
		0 0 1.6666666670000001
		0 0 2
		0 0 2.3333333330000001
		0 0 2.6666666669999999
		0 0 3
		;
createNode transform -n "pSphere1Follicle9040" -p "hairSystem1Follicles";
	rename -uid "BADD84CA-4035-F957-94EF-D3BA472CDE22";
createNode follicle -n "pSphere1FollicleShape9040" -p "pSphere1Follicle9040";
	rename -uid "3A5EAD81-40AD-D35B-CBF8-4BA071DE79D7";
	setAttr -k off ".v";
	setAttr ".pu" 0.90625;
	setAttr ".pv" 0.40625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve216" -p "pSphere1Follicle9040";
	rename -uid "F00FC242-49CA-9CC5-B6BD-3783D0633226";
createNode nurbsCurve -n "curveShape216" -p "curve216";
	rename -uid "67093AFC-4980-0F84-8BC6-EB983F94E91A";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
		0 0 0.66666666669999997
		0 0 1
		0 0 1.3333333329999999
		0 0 1.6666666670000001
		0 0 2
		0 0 2.3333333330000001
		0 0 2.6666666669999999
		0 0 3
		;
createNode transform -n "pSphere1Follicle9046" -p "hairSystem1Follicles";
	rename -uid "90E0833B-4728-F33A-8F0B-81BFA5908DDA";
createNode follicle -n "pSphere1FollicleShape9046" -p "pSphere1Follicle9046";
	rename -uid "37802B59-4156-A1DF-F768-B0BDCCF5038A";
	setAttr -k off ".v";
	setAttr ".pu" 0.90625;
	setAttr ".pv" 0.46875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve217" -p "pSphere1Follicle9046";
	rename -uid "AB8B690C-4994-0F46-B8C9-F6BA75E645CA";
createNode nurbsCurve -n "curveShape217" -p "curve217";
	rename -uid "E0546175-4B8D-5542-9AC9-E989D3CB692D";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
		0 0 0.66666666669999997
		0 0 1
		0 0 1.3333333329999999
		0 0 1.6666666670000001
		0 0 2
		0 0 2.3333333330000001
		0 0 2.6666666669999999
		0 0 3
		;
createNode transform -n "pSphere1Follicle9053" -p "hairSystem1Follicles";
	rename -uid "92FE2A21-4258-7716-93FA-579B5A8552AC";
createNode follicle -n "pSphere1FollicleShape9053" -p "pSphere1Follicle9053";
	rename -uid "14A47BBC-402B-EDE2-82C4-358855C2FAA9";
	setAttr -k off ".v";
	setAttr ".pu" 0.90625;
	setAttr ".pv" 0.53125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve218" -p "pSphere1Follicle9053";
	rename -uid "A2390408-4D98-C17C-A476-10B8D0F79CAE";
createNode nurbsCurve -n "curveShape218" -p "curve218";
	rename -uid "940641A3-4159-1989-A9A0-0EA6C4314DA7";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
		0 0 0.66666666669999997
		0 0 1
		0 0 1.3333333329999999
		0 0 1.6666666670000001
		0 0 2
		0 0 2.3333333330000001
		0 0 2.6666666669999999
		0 0 3
		;
createNode transform -n "pSphere1Follicle9059" -p "hairSystem1Follicles";
	rename -uid "C94ADC11-4CDE-B948-E643-0882A3BA3AA3";
createNode follicle -n "pSphere1FollicleShape9059" -p "pSphere1Follicle9059";
	rename -uid "09D75668-45A6-157B-78C1-9CAA35EBA058";
	setAttr -k off ".v";
	setAttr ".pu" 0.90625;
	setAttr ".pv" 0.59375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve219" -p "pSphere1Follicle9059";
	rename -uid "2D88BAB9-4FC0-8941-B093-D2B2D56052DA";
createNode nurbsCurve -n "curveShape219" -p "curve219";
	rename -uid "1E037C6E-4B02-EDEE-56D8-C4BA0A471861";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
		0 0 0.66666666669999997
		0 0 1
		0 0 1.3333333329999999
		0 0 1.6666666670000001
		0 0 2
		0 0 2.3333333330000001
		0 0 2.6666666669999999
		0 0 3
		;
createNode transform -n "pSphere1Follicle9065" -p "hairSystem1Follicles";
	rename -uid "07FADAE5-4307-2FAA-3135-E2A053EF98C1";
createNode follicle -n "pSphere1FollicleShape9065" -p "pSphere1Follicle9065";
	rename -uid "D6EB90E0-4ACE-DF09-AD70-3FBFD77C3020";
	setAttr -k off ".v";
	setAttr ".pu" 0.90625;
	setAttr ".pv" 0.65625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve220" -p "pSphere1Follicle9065";
	rename -uid "69E8A3B7-4C14-5D8D-9883-CB8B27E6A309";
createNode nurbsCurve -n "curveShape220" -p "curve220";
	rename -uid "D2C60BA1-41AF-744F-C1AA-0392FC420B04";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
		0 0 0.66666666669999997
		0 0 1
		0 0 1.3333333329999999
		0 0 1.6666666670000001
		0 0 2
		0 0 2.3333333330000001
		0 0 2.6666666669999999
		0 0 3
		;
createNode transform -n "pSphere1Follicle9071" -p "hairSystem1Follicles";
	rename -uid "0AE5DD36-4CA9-95C5-04D0-789AEDAC8E63";
createNode follicle -n "pSphere1FollicleShape9071" -p "pSphere1Follicle9071";
	rename -uid "60AB9C62-43E3-8F14-330A-E0BEEE947882";
	setAttr -k off ".v";
	setAttr ".pu" 0.90625;
	setAttr ".pv" 0.71875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve221" -p "pSphere1Follicle9071";
	rename -uid "0EFB7730-42DC-C35D-8DE1-8BB32E9835FD";
createNode nurbsCurve -n "curveShape221" -p "curve221";
	rename -uid "D406A137-436C-93BE-1C16-C68AF99343BC";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
		0 0 0.66666666669999997
		0 0 1
		0 0 1.3333333329999999
		0 0 1.6666666670000001
		0 0 2
		0 0 2.3333333330000001
		0 0 2.6666666669999999
		0 0 3
		;
createNode transform -n "pSphere1Follicle9077" -p "hairSystem1Follicles";
	rename -uid "620E42C4-4ED7-6849-32AB-77AB3C39CCFA";
createNode follicle -n "pSphere1FollicleShape9077" -p "pSphere1Follicle9077";
	rename -uid "B8C4C318-4657-EEB6-7B3D-2080A06B1828";
	setAttr -k off ".v";
	setAttr ".pu" 0.90625;
	setAttr ".pv" 0.78125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve222" -p "pSphere1Follicle9077";
	rename -uid "08C8A981-4EB5-A027-B028-2BAEE69351AE";
createNode nurbsCurve -n "curveShape222" -p "curve222";
	rename -uid "B30FD3CA-4383-32B3-1E7E-ADAE1C95A186";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
		0 0 0.66666666669999997
		0 0 1
		0 0 1.3333333329999999
		0 0 1.6666666670000001
		0 0 2
		0 0 2.3333333330000001
		0 0 2.6666666669999999
		0 0 3
		;
createNode transform -n "pSphere1Follicle9084" -p "hairSystem1Follicles";
	rename -uid "BA245FC7-44CC-DC0A-D161-63B409DD9173";
createNode follicle -n "pSphere1FollicleShape9084" -p "pSphere1Follicle9084";
	rename -uid "15772027-4DED-0A2A-ED72-46946FD4A35E";
	setAttr -k off ".v";
	setAttr ".pu" 0.90625;
	setAttr ".pv" 0.84375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve223" -p "pSphere1Follicle9084";
	rename -uid "1796B1F8-4FFA-BD42-3372-ED985FC848FC";
createNode nurbsCurve -n "curveShape223" -p "curve223";
	rename -uid "19822C32-4C3E-1ED6-5D62-E4AAA23D8FC5";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
		0 0 0.66666666669999997
		0 0 1
		0 0 1.3333333329999999
		0 0 1.6666666670000001
		0 0 2
		0 0 2.3333333330000001
		0 0 2.6666666669999999
		0 0 3
		;
createNode transform -n "pSphere1Follicle9090" -p "hairSystem1Follicles";
	rename -uid "5686C99F-4B39-DA8F-1DBA-74B0BCAF5496";
createNode follicle -n "pSphere1FollicleShape9090" -p "pSphere1Follicle9090";
	rename -uid "5457CEFC-4458-E517-D490-719FC9C94A08";
	setAttr -k off ".v";
	setAttr ".pu" 0.90625;
	setAttr ".pv" 0.90625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve224" -p "pSphere1Follicle9090";
	rename -uid "33A88533-4737-64BD-7151-9DB299E62D59";
createNode nurbsCurve -n "curveShape224" -p "curve224";
	rename -uid "AF1F3B1C-46FC-20F0-3C46-27AA23623CF4";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
		0 0 0.66666666669999997
		0 0 1
		0 0 1.3333333329999999
		0 0 1.6666666670000001
		0 0 2
		0 0 2.3333333330000001
		0 0 2.6666666669999999
		0 0 3
		;
createNode transform -n "pSphere1Follicle9603" -p "hairSystem1Follicles";
	rename -uid "A94B0EDD-44D7-77DA-BA6D-7EB861E7FDC9";
createNode follicle -n "pSphere1FollicleShape9603" -p "pSphere1Follicle9603";
	rename -uid "7E97A911-44C2-79B2-8483-628A56F2E254";
	setAttr -k off ".v";
	setAttr ".pu" 0.96875;
	setAttr ".pv" 0.03125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve225" -p "pSphere1Follicle9603";
	rename -uid "AD038578-4EB1-8946-A822-27B3D48B3745";
createNode nurbsCurve -n "curveShape225" -p "curve225";
	rename -uid "73C81A81-4BCF-3E57-D781-E7A04E34FD18";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
		0 0 0.66666666669999997
		0 0 1
		0 0 1.3333333329999999
		0 0 1.6666666670000001
		0 0 2
		0 0 2.3333333330000001
		0 0 2.6666666669999999
		0 0 3
		;
createNode transform -n "pSphere1Follicle9609" -p "hairSystem1Follicles";
	rename -uid "DB64502E-4BEC-3D22-ADC5-A5A352E1A15C";
createNode follicle -n "pSphere1FollicleShape9609" -p "pSphere1Follicle9609";
	rename -uid "063C1ED8-4335-D180-9A24-57BD5056E4C9";
	setAttr -k off ".v";
	setAttr ".pu" 0.96875;
	setAttr ".pv" 0.09375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve226" -p "pSphere1Follicle9609";
	rename -uid "6F84FBB8-4A8F-1AB4-72F4-90ADA165DFD3";
createNode nurbsCurve -n "curveShape226" -p "curve226";
	rename -uid "44391367-4300-7325-A179-B6866DFC1DAC";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
		0 0 0.66666666669999997
		0 0 1
		0 0 1.3333333329999999
		0 0 1.6666666670000001
		0 0 2
		0 0 2.3333333330000001
		0 0 2.6666666669999999
		0 0 3
		;
createNode transform -n "pSphere1Follicle9615" -p "hairSystem1Follicles";
	rename -uid "78794458-46D1-A43B-6124-318026DA8618";
createNode follicle -n "pSphere1FollicleShape9615" -p "pSphere1Follicle9615";
	rename -uid "49E48669-4592-01E7-7EE2-83BE73E2CBBE";
	setAttr -k off ".v";
	setAttr ".pu" 0.96875;
	setAttr ".pv" 0.15625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve227" -p "pSphere1Follicle9615";
	rename -uid "9160E1B2-4E60-2660-BCBE-D7892F188415";
createNode nurbsCurve -n "curveShape227" -p "curve227";
	rename -uid "E66EAF72-4721-28A0-D517-338B5D50C08A";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
		0 0 0.66666666669999997
		0 0 1
		0 0 1.3333333329999999
		0 0 1.6666666670000001
		0 0 2
		0 0 2.3333333330000001
		0 0 2.6666666669999999
		0 0 3
		;
createNode transform -n "pSphere1Follicle9622" -p "hairSystem1Follicles";
	rename -uid "7C530AF3-4315-56F5-D0D5-23A134153E5C";
createNode follicle -n "pSphere1FollicleShape9622" -p "pSphere1Follicle9622";
	rename -uid "C51B8CB3-4636-3E8B-173F-2F8EB60E96A6";
	setAttr -k off ".v";
	setAttr ".pu" 0.96875;
	setAttr ".pv" 0.21875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve228" -p "pSphere1Follicle9622";
	rename -uid "3032A254-4487-53CC-71F1-3DA25E09F4C4";
createNode nurbsCurve -n "curveShape228" -p "curve228";
	rename -uid "11AA1934-4E18-3FD7-72EC-A1B2E978CEBA";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
		0 0 0.66666666669999997
		0 0 1
		0 0 1.3333333329999999
		0 0 1.6666666670000001
		0 0 2
		0 0 2.3333333330000001
		0 0 2.6666666669999999
		0 0 3
		;
createNode transform -n "pSphere1Follicle9628" -p "hairSystem1Follicles";
	rename -uid "06C0537B-4CF3-98E3-B82C-24B74D8AE6A7";
createNode follicle -n "pSphere1FollicleShape9628" -p "pSphere1Follicle9628";
	rename -uid "09645722-43B9-C9F9-BC4A-8A9F920AE42D";
	setAttr -k off ".v";
	setAttr ".pu" 0.96875;
	setAttr ".pv" 0.28125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve229" -p "pSphere1Follicle9628";
	rename -uid "736961C8-4FA7-2263-DFF7-27A75FF15091";
createNode nurbsCurve -n "curveShape229" -p "curve229";
	rename -uid "82961BBD-4F04-347A-2E96-FB953A305EEB";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
		0 0 0.66666666669999997
		0 0 1
		0 0 1.3333333329999999
		0 0 1.6666666670000001
		0 0 2
		0 0 2.3333333330000001
		0 0 2.6666666669999999
		0 0 3
		;
createNode transform -n "pSphere1Follicle9634" -p "hairSystem1Follicles";
	rename -uid "509F58DE-4132-3701-F2FE-B193898B831D";
createNode follicle -n "pSphere1FollicleShape9634" -p "pSphere1Follicle9634";
	rename -uid "6B5B9BC7-4822-5126-55D3-A3AEFDEA5DA2";
	setAttr -k off ".v";
	setAttr ".pu" 0.96875;
	setAttr ".pv" 0.34375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve230" -p "pSphere1Follicle9634";
	rename -uid "732F8EEF-46C5-E6B5-C4B1-90B3DB0AD1F1";
createNode nurbsCurve -n "curveShape230" -p "curve230";
	rename -uid "7E50C257-4F77-0A0A-3242-E78D59BAB76E";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
		0 0 0.66666666669999997
		0 0 1
		0 0 1.3333333329999999
		0 0 1.6666666670000001
		0 0 2
		0 0 2.3333333330000001
		0 0 2.6666666669999999
		0 0 3
		;
createNode transform -n "pSphere1Follicle9640" -p "hairSystem1Follicles";
	rename -uid "29F1D0F6-4811-6BCB-C2A4-0FA9ACB733C5";
createNode follicle -n "pSphere1FollicleShape9640" -p "pSphere1Follicle9640";
	rename -uid "A136511B-4DF2-43E4-B357-259249B9FA01";
	setAttr -k off ".v";
	setAttr ".pu" 0.96875;
	setAttr ".pv" 0.40625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve231" -p "pSphere1Follicle9640";
	rename -uid "147E9663-45E9-2F37-A1F3-9C913C66D95E";
createNode nurbsCurve -n "curveShape231" -p "curve231";
	rename -uid "EA23A775-430A-52EB-C1DA-8B9D8699790F";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
		0 0 0.66666666669999997
		0 0 1
		0 0 1.3333333329999999
		0 0 1.6666666670000001
		0 0 2
		0 0 2.3333333330000001
		0 0 2.6666666669999999
		0 0 3
		;
createNode transform -n "pSphere1Follicle9646" -p "hairSystem1Follicles";
	rename -uid "26CBDF6F-45E7-6003-D03D-83A035695B9E";
createNode follicle -n "pSphere1FollicleShape9646" -p "pSphere1Follicle9646";
	rename -uid "7899768B-43E7-8C36-BC12-3F87124F0B25";
	setAttr -k off ".v";
	setAttr ".pu" 0.96875;
	setAttr ".pv" 0.46875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve232" -p "pSphere1Follicle9646";
	rename -uid "51FAEDE3-418F-032C-FAFE-49B3FC11FB41";
createNode nurbsCurve -n "curveShape232" -p "curve232";
	rename -uid "FFB90E4A-45A2-8A90-4B02-E0B11C7ACD24";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
		0 0 0.66666666669999997
		0 0 1
		0 0 1.3333333329999999
		0 0 1.6666666670000001
		0 0 2
		0 0 2.3333333330000001
		0 0 2.6666666669999999
		0 0 3
		;
createNode transform -n "pSphere1Follicle9653" -p "hairSystem1Follicles";
	rename -uid "0BCA621C-46B2-4CE1-50E5-73BAE7F7D9D9";
createNode follicle -n "pSphere1FollicleShape9653" -p "pSphere1Follicle9653";
	rename -uid "B0C0CC7E-48ED-F93F-38D1-3AA28A3FB5D7";
	setAttr -k off ".v";
	setAttr ".pu" 0.96875;
	setAttr ".pv" 0.53125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve233" -p "pSphere1Follicle9653";
	rename -uid "123AEF4A-4716-9371-3684-C1BB77D0C5BE";
createNode nurbsCurve -n "curveShape233" -p "curve233";
	rename -uid "4DE5A8B4-4300-8E90-0E63-FE9152E6590A";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
		0 0 0.66666666669999997
		0 0 1
		0 0 1.3333333329999999
		0 0 1.6666666670000001
		0 0 2
		0 0 2.3333333330000001
		0 0 2.6666666669999999
		0 0 3
		;
createNode transform -n "pSphere1Follicle9659" -p "hairSystem1Follicles";
	rename -uid "09E947F5-4C28-20A6-4F45-4186FDA5C914";
createNode follicle -n "pSphere1FollicleShape9659" -p "pSphere1Follicle9659";
	rename -uid "2604D43B-4A1A-3183-D57E-AF910E92D51A";
	setAttr -k off ".v";
	setAttr ".pu" 0.96875;
	setAttr ".pv" 0.59375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve234" -p "pSphere1Follicle9659";
	rename -uid "2D4FAE21-446C-7AC0-D91A-A8BB8DFD0DA5";
createNode nurbsCurve -n "curveShape234" -p "curve234";
	rename -uid "2804968A-41EB-5C14-2EF1-4CAD6F36E98E";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
		0 0 0.66666666669999997
		0 0 1
		0 0 1.3333333329999999
		0 0 1.6666666670000001
		0 0 2
		0 0 2.3333333330000001
		0 0 2.6666666669999999
		0 0 3
		;
createNode transform -n "pSphere1Follicle9665" -p "hairSystem1Follicles";
	rename -uid "B8E6C792-4A8B-04F8-81B2-B29A6D5C0FFD";
createNode follicle -n "pSphere1FollicleShape9665" -p "pSphere1Follicle9665";
	rename -uid "78D6D497-4D7D-4FCD-6EF3-1195AA467EE4";
	setAttr -k off ".v";
	setAttr ".pu" 0.96875;
	setAttr ".pv" 0.65625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve235" -p "pSphere1Follicle9665";
	rename -uid "BE0C0124-4FEA-FDD2-5A09-039DBF3BA580";
createNode nurbsCurve -n "curveShape235" -p "curve235";
	rename -uid "4708CA43-4925-CD0D-D2B5-D0894CBC3A4B";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
		0 0 0.66666666669999997
		0 0 1
		0 0 1.3333333329999999
		0 0 1.6666666670000001
		0 0 2
		0 0 2.3333333330000001
		0 0 2.6666666669999999
		0 0 3
		;
createNode transform -n "pSphere1Follicle9671" -p "hairSystem1Follicles";
	rename -uid "158DABDF-49F2-853D-2087-05A85EC454D3";
createNode follicle -n "pSphere1FollicleShape9671" -p "pSphere1Follicle9671";
	rename -uid "EB4AC02E-4C39-98A4-64FC-19B16430CFAA";
	setAttr -k off ".v";
	setAttr ".pu" 0.96875;
	setAttr ".pv" 0.71875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve236" -p "pSphere1Follicle9671";
	rename -uid "4BFE5D29-415B-ABCA-B18D-6489F12D4CB4";
createNode nurbsCurve -n "curveShape236" -p "curve236";
	rename -uid "D6C422AC-4581-1449-41FA-5EA05D2434A1";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
		0 0 0.66666666669999997
		0 0 1
		0 0 1.3333333329999999
		0 0 1.6666666670000001
		0 0 2
		0 0 2.3333333330000001
		0 0 2.6666666669999999
		0 0 3
		;
createNode transform -n "pSphere1Follicle9677" -p "hairSystem1Follicles";
	rename -uid "C9816C3E-479C-9913-0A2D-4282C9F05248";
createNode follicle -n "pSphere1FollicleShape9677" -p "pSphere1Follicle9677";
	rename -uid "1DE89FE2-4CE0-A986-010C-4E96D60B6FBC";
	setAttr -k off ".v";
	setAttr ".pu" 0.96875;
	setAttr ".pv" 0.78125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve237" -p "pSphere1Follicle9677";
	rename -uid "F7499196-4895-8F2F-02A6-FFBD6DD451BA";
createNode nurbsCurve -n "curveShape237" -p "curve237";
	rename -uid "D480B267-4652-44C2-12A5-9099926B7246";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
		0 0 0.66666666669999997
		0 0 1
		0 0 1.3333333329999999
		0 0 1.6666666670000001
		0 0 2
		0 0 2.3333333330000001
		0 0 2.6666666669999999
		0 0 3
		;
createNode transform -n "pSphere1Follicle9684" -p "hairSystem1Follicles";
	rename -uid "D2E85255-450B-2D7C-AD46-C68DA60118E3";
createNode follicle -n "pSphere1FollicleShape9684" -p "pSphere1Follicle9684";
	rename -uid "B67CAA1C-430A-8C47-5948-D08AF5D01ED7";
	setAttr -k off ".v";
	setAttr ".pu" 0.96875;
	setAttr ".pv" 0.84375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve238" -p "pSphere1Follicle9684";
	rename -uid "CD47E845-4154-37EC-8B3C-E8A0360E8A84";
createNode nurbsCurve -n "curveShape238" -p "curve238";
	rename -uid "DBB0FB4B-45E8-A042-387A-BE801F27E0F3";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
		0 0 0.66666666669999997
		0 0 1
		0 0 1.3333333329999999
		0 0 1.6666666670000001
		0 0 2
		0 0 2.3333333330000001
		0 0 2.6666666669999999
		0 0 3
		;
createNode transform -n "pSphere1Follicle9690" -p "hairSystem1Follicles";
	rename -uid "ABB7841F-4EB3-2184-9C77-4584D0E14E34";
createNode follicle -n "pSphere1FollicleShape9690" -p "pSphere1Follicle9690";
	rename -uid "F910DE7F-4FEB-504E-5E50-3FB8582A7CF2";
	setAttr -k off ".v";
	setAttr ".pu" 0.96875;
	setAttr ".pv" 0.90625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve239" -p "pSphere1Follicle9690";
	rename -uid "E40C79A4-42BD-35F4-A95C-5EB70021D217";
createNode nurbsCurve -n "curveShape239" -p "curve239";
	rename -uid "6986B1BB-4CFC-2C2C-5B1E-C0A981271DFE";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
		0 0 0.66666666669999997
		0 0 1
		0 0 1.3333333329999999
		0 0 1.6666666670000001
		0 0 2
		0 0 2.3333333330000001
		0 0 2.6666666669999999
		0 0 3
		;
createNode transform -n "pSphere1Follicle9696" -p "hairSystem1Follicles";
	rename -uid "2F36EDA9-464C-D345-9FD5-3C95158825CD";
createNode follicle -n "pSphere1FollicleShape9696" -p "pSphere1Follicle9696";
	rename -uid "9852D0C5-4CFC-9B02-DB77-3AA4B841FE09";
	setAttr -k off ".v";
	setAttr ".pu" 0.96875;
	setAttr ".pv" 0.96875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve240" -p "pSphere1Follicle9696";
	rename -uid "A05D2912-4B27-1CD5-13DC-43802AA1FE4C";
createNode nurbsCurve -n "curveShape240" -p "curve240";
	rename -uid "06096420-4482-2183-DB44-299560D9B6A4";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
		0 0 0.66666666669999997
		0 0 1
		0 0 1.3333333329999999
		0 0 1.6666666670000001
		0 0 2
		0 0 2.3333333330000001
		0 0 2.6666666669999999
		0 0 3
		;
createNode transform -n "pfxHair1";
	rename -uid "54D049B2-4C1A-9F30-D186-2B9906C19425";
createNode pfxHair -n "pfxHairShape1" -p "pfxHair1";
	rename -uid "E47F81E5-42B3-F32A-ABF9-3C8AA8E3BA60";
	setAttr -k off ".v";
	setAttr ".dpc" 100;
	setAttr ".dam" no;
createNode lightLinker -s -n "lightLinker1";
	rename -uid "BD28303B-4099-A191-09BD-C2A70A72D719";
	setAttr -s 4 ".lnk";
	setAttr -s 4 ".slnk";
createNode displayLayerManager -n "layerManager";
	rename -uid "1CFBEE8A-471A-C3F6-8535-92AAED11C29E";
createNode displayLayer -n "defaultLayer";
	rename -uid "DB139F6F-4A91-85ED-5529-F7B17DD65B3A";
createNode renderLayerManager -n "renderLayerManager";
	rename -uid "38F1FC8F-44E1-5DCD-444A-2A802B4BCA56";
createNode renderLayer -n "defaultRenderLayer";
	rename -uid "6C9146D9-4D77-DB59-CAA7-11B8134E65C3";
	setAttr ".g" yes;
createNode aiOptions -s -n "defaultArnoldRenderOptions";
	rename -uid "8F4516EF-49DB-7F3F-1C54-E8B1D628B22B";
	addAttr -ci true -sn "ARV_options" -ln "ARV_options" -dt "string";
	setAttr ".AA_samples" 16;
	setAttr ".mb_en" yes;
	setAttr ".ARV_options" -type "string" "Test Resolution=100%;Camera=perspShape;Color Management.Gamma=1;Color Management.Exposure=0;Background.BG=BG Color;Background.Color=0 0 0;Background.Image=;Background.Scale=1 1;Background.Offset=0 0;Background.Apply Color Management=1;Foreground.Enable FG=0;Foreground.Image=;Foreground.Scale=1 1;Foreground.Offset=0 0;Foreground.Apply Color Management=1;";
createNode script -n "uiConfigurationScriptNode";
	rename -uid "85B705F8-4EBC-10D9-8B36-1E8FD6C1355D";
	setAttr ".b" -type "string" (
		"// Maya Mel UI Configuration File.\n//\n//  This script is machine generated.  Edit at your own risk.\n//\n//\n\nglobal string $gMainPane;\nif (`paneLayout -exists $gMainPane`) {\n\n\tglobal int $gUseScenePanelConfig;\n\tint    $useSceneConfig = $gUseScenePanelConfig;\n\tint    $nodeEditorPanelVisible = stringArrayContains(\"nodeEditorPanel1\", `getPanel -vis`);\n\tint    $nodeEditorWorkspaceControlOpen = (`workspaceControl -exists nodeEditorPanel1Window` && `workspaceControl -q -visible nodeEditorPanel1Window`);\n\tint    $menusOkayInPanels = `optionVar -q allowMenusInPanels`;\n\tint    $nVisPanes = `paneLayout -q -nvp $gMainPane`;\n\tint    $nPanes = 0;\n\tstring $editorName;\n\tstring $panelName;\n\tstring $itemFilterName;\n\tstring $panelConfig;\n\n\t//\n\t//  get current state of the UI\n\t//\n\tsceneUIReplacement -update $gMainPane;\n\n\t$panelName = `sceneUIReplacement -getNextPanel \"modelPanel\" (localizedPanelLabel(\"Top View\")) `;\n\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tmodelPanel -edit -l (localizedPanelLabel(\"Top View\")) -mbv $menusOkayInPanels  $panelName;\n"
		+ "\t\t$editorName = $panelName;\n        modelEditor -e \n            -camera \"top\" \n            -useInteractiveMode 0\n            -displayLights \"default\" \n            -displayAppearance \"wireframe\" \n            -activeOnly 0\n            -ignorePanZoom 0\n            -wireframeOnShaded 0\n            -headsUpDisplay 1\n            -holdOuts 1\n            -selectionHiliteDisplay 1\n            -useDefaultMaterial 0\n            -bufferMode \"double\" \n            -twoSidedLighting 1\n            -backfaceCulling 0\n            -xray 0\n            -jointXray 0\n            -activeComponentsXray 0\n            -displayTextures 0\n            -smoothWireframe 0\n            -lineWidth 1\n            -textureAnisotropic 0\n            -textureHilight 1\n            -textureSampling 2\n            -textureDisplay \"modulate\" \n            -textureMaxSize 16384\n            -fogging 0\n            -fogSource \"fragment\" \n            -fogMode \"linear\" \n            -fogStart 0\n            -fogEnd 100\n            -fogDensity 0.1\n            -fogColor 0.5 0.5 0.5 1 \n"
		+ "            -depthOfFieldPreview 1\n            -maxConstantTransparency 1\n            -rendererName \"vp2Renderer\" \n            -objectFilterShowInHUD 1\n            -isFiltered 0\n            -colorResolution 256 256 \n            -bumpResolution 512 512 \n            -textureCompression 0\n            -transparencyAlgorithm \"frontAndBackCull\" \n            -transpInShadows 0\n            -cullingOverride \"none\" \n            -lowQualityLighting 0\n            -maximumNumHardwareLights 1\n            -occlusionCulling 0\n            -shadingModel 0\n            -useBaseRenderer 0\n            -useReducedRenderer 0\n            -smallObjectCulling 0\n            -smallObjectThreshold -1 \n            -interactiveDisableShadows 0\n            -interactiveBackFaceCull 0\n            -sortTransparent 1\n            -controllers 1\n            -nurbsCurves 1\n            -nurbsSurfaces 1\n            -polymeshes 1\n            -subdivSurfaces 1\n            -planes 1\n            -lights 1\n            -cameras 1\n            -controlVertices 1\n"
		+ "            -hulls 1\n            -grid 1\n            -imagePlane 1\n            -joints 1\n            -ikHandles 1\n            -deformers 1\n            -dynamics 1\n            -particleInstancers 1\n            -fluids 1\n            -hairSystems 1\n            -follicles 1\n            -nCloths 1\n            -nParticles 1\n            -nRigids 1\n            -dynamicConstraints 1\n            -locators 1\n            -manipulators 1\n            -pluginShapes 1\n            -dimensions 1\n            -handles 1\n            -pivots 1\n            -textures 1\n            -strokes 1\n            -motionTrails 1\n            -clipGhosts 1\n            -greasePencils 1\n            -shadows 0\n            -captureSequenceNumber -1\n            -width 1\n            -height 1\n            -sceneRenderFilter 0\n            $editorName;\n        modelEditor -e -viewSelected 0 $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextPanel \"modelPanel\" (localizedPanelLabel(\"Side View\")) `;\n"
		+ "\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tmodelPanel -edit -l (localizedPanelLabel(\"Side View\")) -mbv $menusOkayInPanels  $panelName;\n\t\t$editorName = $panelName;\n        modelEditor -e \n            -camera \"side\" \n            -useInteractiveMode 0\n            -displayLights \"default\" \n            -displayAppearance \"wireframe\" \n            -activeOnly 0\n            -ignorePanZoom 0\n            -wireframeOnShaded 0\n            -headsUpDisplay 1\n            -holdOuts 1\n            -selectionHiliteDisplay 1\n            -useDefaultMaterial 0\n            -bufferMode \"double\" \n            -twoSidedLighting 1\n            -backfaceCulling 0\n            -xray 0\n            -jointXray 0\n            -activeComponentsXray 0\n            -displayTextures 0\n            -smoothWireframe 0\n            -lineWidth 1\n            -textureAnisotropic 0\n            -textureHilight 1\n            -textureSampling 2\n            -textureDisplay \"modulate\" \n            -textureMaxSize 16384\n            -fogging 0\n"
		+ "            -fogSource \"fragment\" \n            -fogMode \"linear\" \n            -fogStart 0\n            -fogEnd 100\n            -fogDensity 0.1\n            -fogColor 0.5 0.5 0.5 1 \n            -depthOfFieldPreview 1\n            -maxConstantTransparency 1\n            -rendererName \"vp2Renderer\" \n            -objectFilterShowInHUD 1\n            -isFiltered 0\n            -colorResolution 256 256 \n            -bumpResolution 512 512 \n            -textureCompression 0\n            -transparencyAlgorithm \"frontAndBackCull\" \n            -transpInShadows 0\n            -cullingOverride \"none\" \n            -lowQualityLighting 0\n            -maximumNumHardwareLights 1\n            -occlusionCulling 0\n            -shadingModel 0\n            -useBaseRenderer 0\n            -useReducedRenderer 0\n            -smallObjectCulling 0\n            -smallObjectThreshold -1 \n            -interactiveDisableShadows 0\n            -interactiveBackFaceCull 0\n            -sortTransparent 1\n            -controllers 1\n            -nurbsCurves 1\n"
		+ "            -nurbsSurfaces 1\n            -polymeshes 1\n            -subdivSurfaces 1\n            -planes 1\n            -lights 1\n            -cameras 1\n            -controlVertices 1\n            -hulls 1\n            -grid 1\n            -imagePlane 1\n            -joints 1\n            -ikHandles 1\n            -deformers 1\n            -dynamics 1\n            -particleInstancers 1\n            -fluids 1\n            -hairSystems 1\n            -follicles 1\n            -nCloths 1\n            -nParticles 1\n            -nRigids 1\n            -dynamicConstraints 1\n            -locators 1\n            -manipulators 1\n            -pluginShapes 1\n            -dimensions 1\n            -handles 1\n            -pivots 1\n            -textures 1\n            -strokes 1\n            -motionTrails 1\n            -clipGhosts 1\n            -greasePencils 1\n            -shadows 0\n            -captureSequenceNumber -1\n            -width 1\n            -height 1\n            -sceneRenderFilter 0\n            $editorName;\n        modelEditor -e -viewSelected 0 $editorName;\n"
		+ "\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextPanel \"modelPanel\" (localizedPanelLabel(\"Front View\")) `;\n\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tmodelPanel -edit -l (localizedPanelLabel(\"Front View\")) -mbv $menusOkayInPanels  $panelName;\n\t\t$editorName = $panelName;\n        modelEditor -e \n            -camera \"front\" \n            -useInteractiveMode 0\n            -displayLights \"default\" \n            -displayAppearance \"wireframe\" \n            -activeOnly 0\n            -ignorePanZoom 0\n            -wireframeOnShaded 0\n            -headsUpDisplay 1\n            -holdOuts 1\n            -selectionHiliteDisplay 1\n            -useDefaultMaterial 0\n            -bufferMode \"double\" \n            -twoSidedLighting 1\n            -backfaceCulling 0\n            -xray 0\n            -jointXray 0\n            -activeComponentsXray 0\n            -displayTextures 0\n            -smoothWireframe 0\n            -lineWidth 1\n            -textureAnisotropic 0\n"
		+ "            -textureHilight 1\n            -textureSampling 2\n            -textureDisplay \"modulate\" \n            -textureMaxSize 16384\n            -fogging 0\n            -fogSource \"fragment\" \n            -fogMode \"linear\" \n            -fogStart 0\n            -fogEnd 100\n            -fogDensity 0.1\n            -fogColor 0.5 0.5 0.5 1 \n            -depthOfFieldPreview 1\n            -maxConstantTransparency 1\n            -rendererName \"vp2Renderer\" \n            -objectFilterShowInHUD 1\n            -isFiltered 0\n            -colorResolution 256 256 \n            -bumpResolution 512 512 \n            -textureCompression 0\n            -transparencyAlgorithm \"frontAndBackCull\" \n            -transpInShadows 0\n            -cullingOverride \"none\" \n            -lowQualityLighting 0\n            -maximumNumHardwareLights 1\n            -occlusionCulling 0\n            -shadingModel 0\n            -useBaseRenderer 0\n            -useReducedRenderer 0\n            -smallObjectCulling 0\n            -smallObjectThreshold -1 \n            -interactiveDisableShadows 0\n"
		+ "            -interactiveBackFaceCull 0\n            -sortTransparent 1\n            -controllers 1\n            -nurbsCurves 1\n            -nurbsSurfaces 1\n            -polymeshes 1\n            -subdivSurfaces 1\n            -planes 1\n            -lights 1\n            -cameras 1\n            -controlVertices 1\n            -hulls 1\n            -grid 1\n            -imagePlane 1\n            -joints 1\n            -ikHandles 1\n            -deformers 1\n            -dynamics 1\n            -particleInstancers 1\n            -fluids 1\n            -hairSystems 1\n            -follicles 1\n            -nCloths 1\n            -nParticles 1\n            -nRigids 1\n            -dynamicConstraints 1\n            -locators 1\n            -manipulators 1\n            -pluginShapes 1\n            -dimensions 1\n            -handles 1\n            -pivots 1\n            -textures 1\n            -strokes 1\n            -motionTrails 1\n            -clipGhosts 1\n            -greasePencils 1\n            -shadows 0\n            -captureSequenceNumber -1\n"
		+ "            -width 1\n            -height 1\n            -sceneRenderFilter 0\n            $editorName;\n        modelEditor -e -viewSelected 0 $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextPanel \"modelPanel\" (localizedPanelLabel(\"Persp View\")) `;\n\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tmodelPanel -edit -l (localizedPanelLabel(\"Persp View\")) -mbv $menusOkayInPanels  $panelName;\n\t\t$editorName = $panelName;\n        modelEditor -e \n            -camera \"persp\" \n            -useInteractiveMode 0\n            -displayLights \"default\" \n            -displayAppearance \"smoothShaded\" \n            -activeOnly 0\n            -ignorePanZoom 0\n            -wireframeOnShaded 0\n            -headsUpDisplay 1\n            -holdOuts 1\n            -selectionHiliteDisplay 1\n            -useDefaultMaterial 0\n            -bufferMode \"double\" \n            -twoSidedLighting 1\n            -backfaceCulling 0\n            -xray 0\n            -jointXray 0\n"
		+ "            -activeComponentsXray 0\n            -displayTextures 0\n            -smoothWireframe 0\n            -lineWidth 1\n            -textureAnisotropic 0\n            -textureHilight 1\n            -textureSampling 2\n            -textureDisplay \"modulate\" \n            -textureMaxSize 16384\n            -fogging 0\n            -fogSource \"fragment\" \n            -fogMode \"linear\" \n            -fogStart 0\n            -fogEnd 100\n            -fogDensity 0.1\n            -fogColor 0.5 0.5 0.5 1 \n            -depthOfFieldPreview 1\n            -maxConstantTransparency 1\n            -rendererName \"vp2Renderer\" \n            -objectFilterShowInHUD 1\n            -isFiltered 0\n            -colorResolution 256 256 \n            -bumpResolution 512 512 \n            -textureCompression 0\n            -transparencyAlgorithm \"frontAndBackCull\" \n            -transpInShadows 0\n            -cullingOverride \"none\" \n            -lowQualityLighting 0\n            -maximumNumHardwareLights 1\n            -occlusionCulling 0\n            -shadingModel 0\n"
		+ "            -useBaseRenderer 0\n            -useReducedRenderer 0\n            -smallObjectCulling 0\n            -smallObjectThreshold -1 \n            -interactiveDisableShadows 0\n            -interactiveBackFaceCull 0\n            -sortTransparent 1\n            -controllers 1\n            -nurbsCurves 1\n            -nurbsSurfaces 1\n            -polymeshes 1\n            -subdivSurfaces 1\n            -planes 1\n            -lights 1\n            -cameras 1\n            -controlVertices 1\n            -hulls 1\n            -grid 1\n            -imagePlane 1\n            -joints 1\n            -ikHandles 1\n            -deformers 1\n            -dynamics 1\n            -particleInstancers 1\n            -fluids 1\n            -hairSystems 1\n            -follicles 1\n            -nCloths 1\n            -nParticles 1\n            -nRigids 1\n            -dynamicConstraints 1\n            -locators 1\n            -manipulators 1\n            -pluginShapes 1\n            -dimensions 1\n            -handles 1\n            -pivots 1\n            -textures 1\n"
		+ "            -strokes 1\n            -motionTrails 1\n            -clipGhosts 1\n            -greasePencils 1\n            -shadows 0\n            -captureSequenceNumber -1\n            -width 741\n            -height 675\n            -sceneRenderFilter 0\n            $editorName;\n        modelEditor -e -viewSelected 0 $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextPanel \"outlinerPanel\" (localizedPanelLabel(\"ToggledOutliner\")) `;\n\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\toutlinerPanel -edit -l (localizedPanelLabel(\"ToggledOutliner\")) -mbv $menusOkayInPanels  $panelName;\n\t\t$editorName = $panelName;\n        outlinerEditor -e \n            -docTag \"isolOutln_fromSeln\" \n            -showShapes 0\n            -showAssignedMaterials 0\n            -showTimeEditor 1\n            -showReferenceNodes 0\n            -showReferenceMembers 0\n            -showAttributes 0\n            -showConnected 0\n            -showAnimCurvesOnly 0\n            -showMuteInfo 0\n"
		+ "            -organizeByLayer 1\n            -organizeByClip 1\n            -showAnimLayerWeight 1\n            -autoExpandLayers 1\n            -autoExpand 0\n            -showDagOnly 1\n            -showAssets 1\n            -showContainedOnly 1\n            -showPublishedAsConnected 0\n            -showParentContainers 0\n            -showContainerContents 1\n            -ignoreDagHierarchy 0\n            -expandConnections 0\n            -showUpstreamCurves 1\n            -showUnitlessCurves 1\n            -showCompounds 1\n            -showLeafs 1\n            -showNumericAttrsOnly 0\n            -highlightActive 1\n            -autoSelectNewObjects 0\n            -doNotSelectNewObjects 0\n            -dropIsParent 1\n            -transmitFilters 0\n            -setFilter \"defaultSetFilter\" \n            -showSetMembers 1\n            -allowMultiSelection 1\n            -alwaysToggleSelect 0\n            -directSelect 0\n            -isSet 0\n            -isSetMember 0\n            -displayMode \"DAG\" \n            -expandObjects 0\n            -setsIgnoreFilters 1\n"
		+ "            -containersIgnoreFilters 0\n            -editAttrName 0\n            -showAttrValues 0\n            -highlightSecondary 0\n            -showUVAttrsOnly 0\n            -showTextureNodesOnly 0\n            -attrAlphaOrder \"default\" \n            -animLayerFilterOptions \"allAffecting\" \n            -sortOrder \"none\" \n            -longNames 0\n            -niceNames 1\n            -showNamespace 1\n            -showPinIcons 0\n            -mapMotionTrails 0\n            -ignoreHiddenAttribute 0\n            -ignoreOutlinerColor 0\n            -renderFilterVisible 0\n            -renderFilterIndex 0\n            -selectionOrder \"chronological\" \n            -expandAttribute 0\n            $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextPanel \"outlinerPanel\" (localizedPanelLabel(\"Outliner\")) `;\n\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\toutlinerPanel -edit -l (localizedPanelLabel(\"Outliner\")) -mbv $menusOkayInPanels  $panelName;\n"
		+ "\t\t$editorName = $panelName;\n        outlinerEditor -e \n            -docTag \"isolOutln_fromSeln\" \n            -showShapes 1\n            -showAssignedMaterials 0\n            -showTimeEditor 1\n            -showReferenceNodes 1\n            -showReferenceMembers 1\n            -showAttributes 0\n            -showConnected 0\n            -showAnimCurvesOnly 0\n            -showMuteInfo 0\n            -organizeByLayer 1\n            -organizeByClip 1\n            -showAnimLayerWeight 1\n            -autoExpandLayers 1\n            -autoExpand 0\n            -showDagOnly 0\n            -showAssets 1\n            -showContainedOnly 1\n            -showPublishedAsConnected 0\n            -showParentContainers 0\n            -showContainerContents 1\n            -ignoreDagHierarchy 0\n            -expandConnections 0\n            -showUpstreamCurves 1\n            -showUnitlessCurves 1\n            -showCompounds 1\n            -showLeafs 1\n            -showNumericAttrsOnly 0\n            -highlightActive 1\n            -autoSelectNewObjects 0\n"
		+ "            -doNotSelectNewObjects 0\n            -dropIsParent 1\n            -transmitFilters 0\n            -setFilter \"defaultSetFilter\" \n            -showSetMembers 1\n            -allowMultiSelection 1\n            -alwaysToggleSelect 0\n            -directSelect 0\n            -displayMode \"DAG\" \n            -expandObjects 0\n            -setsIgnoreFilters 1\n            -containersIgnoreFilters 0\n            -editAttrName 0\n            -showAttrValues 0\n            -highlightSecondary 0\n            -showUVAttrsOnly 0\n            -showTextureNodesOnly 0\n            -attrAlphaOrder \"default\" \n            -animLayerFilterOptions \"allAffecting\" \n            -sortOrder \"none\" \n            -longNames 0\n            -niceNames 1\n            -showNamespace 1\n            -showPinIcons 0\n            -mapMotionTrails 0\n            -ignoreHiddenAttribute 0\n            -ignoreOutlinerColor 0\n            -renderFilterVisible 0\n            $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"graphEditor\" (localizedPanelLabel(\"Graph Editor\")) `;\n"
		+ "\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Graph Editor\")) -mbv $menusOkayInPanels  $panelName;\n\n\t\t\t$editorName = ($panelName+\"OutlineEd\");\n            outlinerEditor -e \n                -showShapes 1\n                -showAssignedMaterials 0\n                -showTimeEditor 1\n                -showReferenceNodes 0\n                -showReferenceMembers 0\n                -showAttributes 1\n                -showConnected 1\n                -showAnimCurvesOnly 1\n                -showMuteInfo 0\n                -organizeByLayer 1\n                -organizeByClip 1\n                -showAnimLayerWeight 1\n                -autoExpandLayers 1\n                -autoExpand 1\n                -showDagOnly 0\n                -showAssets 1\n                -showContainedOnly 0\n                -showPublishedAsConnected 0\n                -showParentContainers 1\n                -showContainerContents 0\n                -ignoreDagHierarchy 0\n                -expandConnections 1\n"
		+ "                -showUpstreamCurves 1\n                -showUnitlessCurves 1\n                -showCompounds 0\n                -showLeafs 1\n                -showNumericAttrsOnly 1\n                -highlightActive 0\n                -autoSelectNewObjects 1\n                -doNotSelectNewObjects 0\n                -dropIsParent 1\n                -transmitFilters 1\n                -setFilter \"0\" \n                -showSetMembers 0\n                -allowMultiSelection 1\n                -alwaysToggleSelect 0\n                -directSelect 0\n                -displayMode \"DAG\" \n                -expandObjects 0\n                -setsIgnoreFilters 1\n                -containersIgnoreFilters 0\n                -editAttrName 0\n                -showAttrValues 0\n                -highlightSecondary 0\n                -showUVAttrsOnly 0\n                -showTextureNodesOnly 0\n                -attrAlphaOrder \"default\" \n                -animLayerFilterOptions \"allAffecting\" \n                -sortOrder \"none\" \n                -longNames 0\n"
		+ "                -niceNames 1\n                -showNamespace 1\n                -showPinIcons 1\n                -mapMotionTrails 1\n                -ignoreHiddenAttribute 0\n                -ignoreOutlinerColor 0\n                -renderFilterVisible 0\n                $editorName;\n\n\t\t\t$editorName = ($panelName+\"GraphEd\");\n            animCurveEditor -e \n                -displayKeys 1\n                -displayTangents 0\n                -displayActiveKeys 0\n                -displayActiveKeyTangents 1\n                -displayInfinities 0\n                -displayValues 0\n                -autoFit 1\n                -autoFitTime 0\n                -snapTime \"integer\" \n                -snapValue \"none\" \n                -showResults \"off\" \n                -showBufferCurves \"off\" \n                -smoothness \"fine\" \n                -resultSamples 1\n                -resultScreenSamples 0\n                -resultUpdate \"delayed\" \n                -showUpstreamCurves 1\n                -showCurveNames 0\n                -showActiveCurveNames 0\n"
		+ "                -stackedCurves 0\n                -stackedCurvesMin -1\n                -stackedCurvesMax 1\n                -stackedCurvesSpace 0.2\n                -displayNormalized 0\n                -preSelectionHighlight 0\n                -constrainDrag 0\n                -classicMode 1\n                -valueLinesToggle 1\n                $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"dopeSheetPanel\" (localizedPanelLabel(\"Dope Sheet\")) `;\n\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Dope Sheet\")) -mbv $menusOkayInPanels  $panelName;\n\n\t\t\t$editorName = ($panelName+\"OutlineEd\");\n            outlinerEditor -e \n                -showShapes 1\n                -showAssignedMaterials 0\n                -showTimeEditor 1\n                -showReferenceNodes 0\n                -showReferenceMembers 0\n                -showAttributes 1\n                -showConnected 1\n                -showAnimCurvesOnly 1\n"
		+ "                -showMuteInfo 0\n                -organizeByLayer 1\n                -organizeByClip 1\n                -showAnimLayerWeight 1\n                -autoExpandLayers 1\n                -autoExpand 0\n                -showDagOnly 0\n                -showAssets 1\n                -showContainedOnly 0\n                -showPublishedAsConnected 0\n                -showParentContainers 1\n                -showContainerContents 0\n                -ignoreDagHierarchy 0\n                -expandConnections 1\n                -showUpstreamCurves 1\n                -showUnitlessCurves 0\n                -showCompounds 1\n                -showLeafs 1\n                -showNumericAttrsOnly 1\n                -highlightActive 0\n                -autoSelectNewObjects 0\n                -doNotSelectNewObjects 1\n                -dropIsParent 1\n                -transmitFilters 0\n                -setFilter \"0\" \n                -showSetMembers 0\n                -allowMultiSelection 1\n                -alwaysToggleSelect 0\n                -directSelect 0\n"
		+ "                -displayMode \"DAG\" \n                -expandObjects 0\n                -setsIgnoreFilters 1\n                -containersIgnoreFilters 0\n                -editAttrName 0\n                -showAttrValues 0\n                -highlightSecondary 0\n                -showUVAttrsOnly 0\n                -showTextureNodesOnly 0\n                -attrAlphaOrder \"default\" \n                -animLayerFilterOptions \"allAffecting\" \n                -sortOrder \"none\" \n                -longNames 0\n                -niceNames 1\n                -showNamespace 1\n                -showPinIcons 0\n                -mapMotionTrails 1\n                -ignoreHiddenAttribute 0\n                -ignoreOutlinerColor 0\n                -renderFilterVisible 0\n                $editorName;\n\n\t\t\t$editorName = ($panelName+\"DopeSheetEd\");\n            dopeSheetEditor -e \n                -displayKeys 1\n                -displayTangents 0\n                -displayActiveKeys 0\n                -displayActiveKeyTangents 0\n                -displayInfinities 0\n"
		+ "                -displayValues 0\n                -autoFit 0\n                -autoFitTime 0\n                -snapTime \"integer\" \n                -snapValue \"none\" \n                -outliner \"dopeSheetPanel1OutlineEd\" \n                -showSummary 1\n                -showScene 0\n                -hierarchyBelow 0\n                -showTicks 1\n                -selectionWindow 0 0 0 0 \n                $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"timeEditorPanel\" (localizedPanelLabel(\"Time Editor\")) `;\n\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Time Editor\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"clipEditorPanel\" (localizedPanelLabel(\"Trax Editor\")) `;\n\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Trax Editor\")) -mbv $menusOkayInPanels  $panelName;\n"
		+ "\n\t\t\t$editorName = clipEditorNameFromPanel($panelName);\n            clipEditor -e \n                -displayKeys 0\n                -displayTangents 0\n                -displayActiveKeys 0\n                -displayActiveKeyTangents 0\n                -displayInfinities 0\n                -displayValues 0\n                -autoFit 0\n                -autoFitTime 0\n                -snapTime \"none\" \n                -snapValue \"none\" \n                -initialized 0\n                -manageSequencer 0 \n                $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"sequenceEditorPanel\" (localizedPanelLabel(\"Camera Sequencer\")) `;\n\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Camera Sequencer\")) -mbv $menusOkayInPanels  $panelName;\n\n\t\t\t$editorName = sequenceEditorNameFromPanel($panelName);\n            clipEditor -e \n                -displayKeys 0\n                -displayTangents 0\n"
		+ "                -displayActiveKeys 0\n                -displayActiveKeyTangents 0\n                -displayInfinities 0\n                -displayValues 0\n                -autoFit 0\n                -autoFitTime 0\n                -snapTime \"none\" \n                -snapValue \"none\" \n                -initialized 0\n                -manageSequencer 1 \n                $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"hyperGraphPanel\" (localizedPanelLabel(\"Hypergraph Hierarchy\")) `;\n\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Hypergraph Hierarchy\")) -mbv $menusOkayInPanels  $panelName;\n\n\t\t\t$editorName = ($panelName+\"HyperGraphEd\");\n            hyperGraph -e \n                -graphLayoutStyle \"hierarchicalLayout\" \n                -orientation \"horiz\" \n                -mergeConnections 0\n                -zoom 1\n                -animateTransition 0\n                -showRelationships 1\n"
		+ "                -showShapes 0\n                -showDeformers 0\n                -showExpressions 0\n                -showConstraints 0\n                -showConnectionFromSelected 0\n                -showConnectionToSelected 0\n                -showConstraintLabels 0\n                -showUnderworld 0\n                -showInvisible 0\n                -transitionFrames 1\n                -opaqueContainers 0\n                -freeform 0\n                -imagePosition 0 0 \n                -imageScale 1\n                -imageEnabled 0\n                -graphType \"DAG\" \n                -heatMapDisplay 0\n                -updateSelection 1\n                -updateNodeAdded 1\n                -useDrawOverrideColor 0\n                -limitGraphTraversal -1\n                -range 0 0 \n                -iconSize \"smallIcons\" \n                -showCachedConnections 0\n                $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"hyperShadePanel\" (localizedPanelLabel(\"Hypershade\")) `;\n"
		+ "\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Hypershade\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"visorPanel\" (localizedPanelLabel(\"Visor\")) `;\n\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Visor\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"nodeEditorPanel\" (localizedPanelLabel(\"Node Editor\")) `;\n\tif ($nodeEditorPanelVisible || $nodeEditorWorkspaceControlOpen) {\n\t\tif (\"\" == $panelName) {\n\t\t\tif ($useSceneConfig) {\n\t\t\t\t$panelName = `scriptedPanel -unParent  -type \"nodeEditorPanel\" -l (localizedPanelLabel(\"Node Editor\")) -mbv $menusOkayInPanels `;\n\n\t\t\t$editorName = ($panelName+\"NodeEditorEd\");\n            nodeEditor -e \n                -allAttributes 0\n"
		+ "                -allNodes 0\n                -autoSizeNodes 1\n                -consistentNameSize 1\n                -createNodeCommand \"nodeEdCreateNodeCommand\" \n                -connectNodeOnCreation 0\n                -connectOnDrop 0\n                -copyConnectionsOnPaste 0\n                -defaultPinnedState 0\n                -additiveGraphingMode 0\n                -settingsChangedCallback \"nodeEdSyncControls\" \n                -traversalDepthLimit -1\n                -keyPressCommand \"nodeEdKeyPressCommand\" \n                -nodeTitleMode \"name\" \n                -gridSnap 0\n                -gridVisibility 1\n                -crosshairOnEdgeDragging 0\n                -popupMenuScript \"nodeEdBuildPanelMenus\" \n                -showNamespace 1\n                -showShapes 1\n                -showSGShapes 0\n                -showTransforms 1\n                -useAssets 1\n                -syncedSelection 1\n                -extendToShapes 1\n                -editorMode \"default\" \n                $editorName;\n\t\t\t}\n\t\t} else {\n"
		+ "\t\t\t$label = `panel -q -label $panelName`;\n\t\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Node Editor\")) -mbv $menusOkayInPanels  $panelName;\n\n\t\t\t$editorName = ($panelName+\"NodeEditorEd\");\n            nodeEditor -e \n                -allAttributes 0\n                -allNodes 0\n                -autoSizeNodes 1\n                -consistentNameSize 1\n                -createNodeCommand \"nodeEdCreateNodeCommand\" \n                -connectNodeOnCreation 0\n                -connectOnDrop 0\n                -copyConnectionsOnPaste 0\n                -defaultPinnedState 0\n                -additiveGraphingMode 0\n                -settingsChangedCallback \"nodeEdSyncControls\" \n                -traversalDepthLimit -1\n                -keyPressCommand \"nodeEdKeyPressCommand\" \n                -nodeTitleMode \"name\" \n                -gridSnap 0\n                -gridVisibility 1\n                -crosshairOnEdgeDragging 0\n                -popupMenuScript \"nodeEdBuildPanelMenus\" \n                -showNamespace 1\n                -showShapes 1\n"
		+ "                -showSGShapes 0\n                -showTransforms 1\n                -useAssets 1\n                -syncedSelection 1\n                -extendToShapes 1\n                -editorMode \"default\" \n                $editorName;\n\t\t\tif (!$useSceneConfig) {\n\t\t\t\tpanel -e -l $label $panelName;\n\t\t\t}\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"createNodePanel\" (localizedPanelLabel(\"Create Node\")) `;\n\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Create Node\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"polyTexturePlacementPanel\" (localizedPanelLabel(\"UV Editor\")) `;\n\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"UV Editor\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"renderWindowPanel\" (localizedPanelLabel(\"Render View\")) `;\n"
		+ "\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Render View\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextPanel \"shapePanel\" (localizedPanelLabel(\"Shape Editor\")) `;\n\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tshapePanel -edit -l (localizedPanelLabel(\"Shape Editor\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextPanel \"posePanel\" (localizedPanelLabel(\"Pose Editor\")) `;\n\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tposePanel -edit -l (localizedPanelLabel(\"Pose Editor\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"dynRelEdPanel\" (localizedPanelLabel(\"Dynamic Relationships\")) `;\n\tif (\"\" != $panelName) {\n"
		+ "\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Dynamic Relationships\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"relationshipPanel\" (localizedPanelLabel(\"Relationship Editor\")) `;\n\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Relationship Editor\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"referenceEditorPanel\" (localizedPanelLabel(\"Reference Editor\")) `;\n\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Reference Editor\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"componentEditorPanel\" (localizedPanelLabel(\"Component Editor\")) `;\n"
		+ "\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Component Editor\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"dynPaintScriptedPanelType\" (localizedPanelLabel(\"Paint Effects\")) `;\n\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Paint Effects\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"scriptEditorPanel\" (localizedPanelLabel(\"Script Editor\")) `;\n\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Script Editor\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"profilerPanel\" (localizedPanelLabel(\"Profiler Tool\")) `;\n"
		+ "\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Profiler Tool\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"contentBrowserPanel\" (localizedPanelLabel(\"Content Browser\")) `;\n\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Content Browser\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"Stereo\" (localizedPanelLabel(\"Stereo\")) `;\n\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Stereo\")) -mbv $menusOkayInPanels  $panelName;\nstring $editorName = ($panelName+\"Editor\");\n            stereoCameraView -e \n                -camera \"persp\" \n                -useInteractiveMode 0\n                -displayLights \"default\" \n"
		+ "                -displayAppearance \"wireframe\" \n                -activeOnly 0\n                -ignorePanZoom 0\n                -wireframeOnShaded 0\n                -headsUpDisplay 1\n                -holdOuts 1\n                -selectionHiliteDisplay 1\n                -useDefaultMaterial 0\n                -bufferMode \"double\" \n                -twoSidedLighting 1\n                -backfaceCulling 0\n                -xray 0\n                -jointXray 0\n                -activeComponentsXray 0\n                -displayTextures 0\n                -smoothWireframe 0\n                -lineWidth 1\n                -textureAnisotropic 0\n                -textureHilight 1\n                -textureSampling 2\n                -textureDisplay \"modulate\" \n                -textureMaxSize 16384\n                -fogging 0\n                -fogSource \"fragment\" \n                -fogMode \"linear\" \n                -fogStart 0\n                -fogEnd 100\n                -fogDensity 0.1\n                -fogColor 0.5 0.5 0.5 1 \n                -depthOfFieldPreview 1\n"
		+ "                -maxConstantTransparency 1\n                -objectFilterShowInHUD 1\n                -isFiltered 0\n                -colorResolution 4 4 \n                -bumpResolution 4 4 \n                -textureCompression 0\n                -transparencyAlgorithm \"frontAndBackCull\" \n                -transpInShadows 0\n                -cullingOverride \"none\" \n                -lowQualityLighting 0\n                -maximumNumHardwareLights 0\n                -occlusionCulling 0\n                -shadingModel 0\n                -useBaseRenderer 0\n                -useReducedRenderer 0\n                -smallObjectCulling 0\n                -smallObjectThreshold -1 \n                -interactiveDisableShadows 0\n                -interactiveBackFaceCull 0\n                -sortTransparent 1\n                -controllers 1\n                -nurbsCurves 1\n                -nurbsSurfaces 1\n                -polymeshes 1\n                -subdivSurfaces 1\n                -planes 1\n                -lights 1\n                -cameras 1\n"
		+ "                -controlVertices 1\n                -hulls 1\n                -grid 1\n                -imagePlane 1\n                -joints 1\n                -ikHandles 1\n                -deformers 1\n                -dynamics 1\n                -particleInstancers 1\n                -fluids 1\n                -hairSystems 1\n                -follicles 1\n                -nCloths 1\n                -nParticles 1\n                -nRigids 1\n                -dynamicConstraints 1\n                -locators 1\n                -manipulators 1\n                -pluginShapes 1\n                -dimensions 1\n                -handles 1\n                -pivots 1\n                -textures 1\n                -strokes 1\n                -motionTrails 1\n                -clipGhosts 1\n                -greasePencils 1\n                -shadows 0\n                -captureSequenceNumber -1\n                -width 0\n                -height 0\n                -sceneRenderFilter 0\n                -displayMode \"centerEye\" \n                -viewColor 0 0 0 1 \n"
		+ "                -useCustomBackground 1\n                $editorName;\n            stereoCameraView -e -viewSelected 0 $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\tif ($useSceneConfig) {\n        string $configName = `getPanel -cwl (localizedPanelLabel(\"Current Layout\"))`;\n        if (\"\" != $configName) {\n\t\t\tpanelConfiguration -edit -label (localizedPanelLabel(\"Current Layout\")) \n\t\t\t\t-userCreated false\n\t\t\t\t-defaultImage \"vacantCell.xP:/\"\n\t\t\t\t-image \"\"\n\t\t\t\t-sc false\n\t\t\t\t-configString \"global string $gMainPane; paneLayout -e -cn \\\"single\\\" -ps 1 100 100 $gMainPane;\"\n\t\t\t\t-removeAllPanels\n\t\t\t\t-ap false\n\t\t\t\t\t(localizedPanelLabel(\"Persp View\")) \n\t\t\t\t\t\"modelPanel\"\n"
		+ "\t\t\t\t\t\"$panelName = `modelPanel -unParent -l (localizedPanelLabel(\\\"Persp View\\\")) -mbv $menusOkayInPanels `;\\n$editorName = $panelName;\\nmodelEditor -e \\n    -cam `findStartUpCamera persp` \\n    -useInteractiveMode 0\\n    -displayLights \\\"default\\\" \\n    -displayAppearance \\\"smoothShaded\\\" \\n    -activeOnly 0\\n    -ignorePanZoom 0\\n    -wireframeOnShaded 0\\n    -headsUpDisplay 1\\n    -holdOuts 1\\n    -selectionHiliteDisplay 1\\n    -useDefaultMaterial 0\\n    -bufferMode \\\"double\\\" \\n    -twoSidedLighting 1\\n    -backfaceCulling 0\\n    -xray 0\\n    -jointXray 0\\n    -activeComponentsXray 0\\n    -displayTextures 0\\n    -smoothWireframe 0\\n    -lineWidth 1\\n    -textureAnisotropic 0\\n    -textureHilight 1\\n    -textureSampling 2\\n    -textureDisplay \\\"modulate\\\" \\n    -textureMaxSize 16384\\n    -fogging 0\\n    -fogSource \\\"fragment\\\" \\n    -fogMode \\\"linear\\\" \\n    -fogStart 0\\n    -fogEnd 100\\n    -fogDensity 0.1\\n    -fogColor 0.5 0.5 0.5 1 \\n    -depthOfFieldPreview 1\\n    -maxConstantTransparency 1\\n    -rendererName \\\"vp2Renderer\\\" \\n    -objectFilterShowInHUD 1\\n    -isFiltered 0\\n    -colorResolution 256 256 \\n    -bumpResolution 512 512 \\n    -textureCompression 0\\n    -transparencyAlgorithm \\\"frontAndBackCull\\\" \\n    -transpInShadows 0\\n    -cullingOverride \\\"none\\\" \\n    -lowQualityLighting 0\\n    -maximumNumHardwareLights 1\\n    -occlusionCulling 0\\n    -shadingModel 0\\n    -useBaseRenderer 0\\n    -useReducedRenderer 0\\n    -smallObjectCulling 0\\n    -smallObjectThreshold -1 \\n    -interactiveDisableShadows 0\\n    -interactiveBackFaceCull 0\\n    -sortTransparent 1\\n    -controllers 1\\n    -nurbsCurves 1\\n    -nurbsSurfaces 1\\n    -polymeshes 1\\n    -subdivSurfaces 1\\n    -planes 1\\n    -lights 1\\n    -cameras 1\\n    -controlVertices 1\\n    -hulls 1\\n    -grid 1\\n    -imagePlane 1\\n    -joints 1\\n    -ikHandles 1\\n    -deformers 1\\n    -dynamics 1\\n    -particleInstancers 1\\n    -fluids 1\\n    -hairSystems 1\\n    -follicles 1\\n    -nCloths 1\\n    -nParticles 1\\n    -nRigids 1\\n    -dynamicConstraints 1\\n    -locators 1\\n    -manipulators 1\\n    -pluginShapes 1\\n    -dimensions 1\\n    -handles 1\\n    -pivots 1\\n    -textures 1\\n    -strokes 1\\n    -motionTrails 1\\n    -clipGhosts 1\\n    -greasePencils 1\\n    -shadows 0\\n    -captureSequenceNumber -1\\n    -width 741\\n    -height 675\\n    -sceneRenderFilter 0\\n    $editorName;\\nmodelEditor -e -viewSelected 0 $editorName\"\n"
		+ "\t\t\t\t\t\"modelPanel -edit -l (localizedPanelLabel(\\\"Persp View\\\")) -mbv $menusOkayInPanels  $panelName;\\n$editorName = $panelName;\\nmodelEditor -e \\n    -cam `findStartUpCamera persp` \\n    -useInteractiveMode 0\\n    -displayLights \\\"default\\\" \\n    -displayAppearance \\\"smoothShaded\\\" \\n    -activeOnly 0\\n    -ignorePanZoom 0\\n    -wireframeOnShaded 0\\n    -headsUpDisplay 1\\n    -holdOuts 1\\n    -selectionHiliteDisplay 1\\n    -useDefaultMaterial 0\\n    -bufferMode \\\"double\\\" \\n    -twoSidedLighting 1\\n    -backfaceCulling 0\\n    -xray 0\\n    -jointXray 0\\n    -activeComponentsXray 0\\n    -displayTextures 0\\n    -smoothWireframe 0\\n    -lineWidth 1\\n    -textureAnisotropic 0\\n    -textureHilight 1\\n    -textureSampling 2\\n    -textureDisplay \\\"modulate\\\" \\n    -textureMaxSize 16384\\n    -fogging 0\\n    -fogSource \\\"fragment\\\" \\n    -fogMode \\\"linear\\\" \\n    -fogStart 0\\n    -fogEnd 100\\n    -fogDensity 0.1\\n    -fogColor 0.5 0.5 0.5 1 \\n    -depthOfFieldPreview 1\\n    -maxConstantTransparency 1\\n    -rendererName \\\"vp2Renderer\\\" \\n    -objectFilterShowInHUD 1\\n    -isFiltered 0\\n    -colorResolution 256 256 \\n    -bumpResolution 512 512 \\n    -textureCompression 0\\n    -transparencyAlgorithm \\\"frontAndBackCull\\\" \\n    -transpInShadows 0\\n    -cullingOverride \\\"none\\\" \\n    -lowQualityLighting 0\\n    -maximumNumHardwareLights 1\\n    -occlusionCulling 0\\n    -shadingModel 0\\n    -useBaseRenderer 0\\n    -useReducedRenderer 0\\n    -smallObjectCulling 0\\n    -smallObjectThreshold -1 \\n    -interactiveDisableShadows 0\\n    -interactiveBackFaceCull 0\\n    -sortTransparent 1\\n    -controllers 1\\n    -nurbsCurves 1\\n    -nurbsSurfaces 1\\n    -polymeshes 1\\n    -subdivSurfaces 1\\n    -planes 1\\n    -lights 1\\n    -cameras 1\\n    -controlVertices 1\\n    -hulls 1\\n    -grid 1\\n    -imagePlane 1\\n    -joints 1\\n    -ikHandles 1\\n    -deformers 1\\n    -dynamics 1\\n    -particleInstancers 1\\n    -fluids 1\\n    -hairSystems 1\\n    -follicles 1\\n    -nCloths 1\\n    -nParticles 1\\n    -nRigids 1\\n    -dynamicConstraints 1\\n    -locators 1\\n    -manipulators 1\\n    -pluginShapes 1\\n    -dimensions 1\\n    -handles 1\\n    -pivots 1\\n    -textures 1\\n    -strokes 1\\n    -motionTrails 1\\n    -clipGhosts 1\\n    -greasePencils 1\\n    -shadows 0\\n    -captureSequenceNumber -1\\n    -width 741\\n    -height 675\\n    -sceneRenderFilter 0\\n    $editorName;\\nmodelEditor -e -viewSelected 0 $editorName\"\n"
		+ "\t\t\t\t$configName;\n\n            setNamedPanelLayout (localizedPanelLabel(\"Current Layout\"));\n        }\n\n        panelHistory -e -clear mainPanelHistory;\n        sceneUIReplacement -clear;\n\t}\n\n\ngrid -spacing 5 -size 12 -divisions 5 -displayAxes yes -displayGridLines yes -displayDivisionLines yes -displayPerspectiveLabels no -displayOrthographicLabels no -displayAxesBold yes -perspectiveLabelPosition axis -orthographicLabelPosition edge;\nviewManip -drawCompass 0 -compassAngle 0 -frontParameters \"\" -homeParameters \"\" -selectionLockParameters \"\";\n}\n");
	setAttr ".st" 3;
createNode script -n "sceneConfigurationScriptNode";
	rename -uid "380B334D-4EF0-D110-7907-E68B23082042";
	setAttr ".b" -type "string" "playbackOptions -min 1 -max 24 -ast 1 -aet 48 ";
	setAttr ".st" 6;
createNode shadingEngine -n "aiStandard1SG";
	rename -uid "4445A0C8-4C94-7C3F-D378-0F8F02A82049";
	setAttr ".ihi" 0;
	setAttr ".ro" yes;
createNode materialInfo -n "materialInfo1";
	rename -uid "0F8E2705-47F2-31C9-1CED-8F91231C2C60";
createNode polySphere -n "polySphere1";
	rename -uid "9369B880-4C12-67AD-40A6-609802E0E5C8";
	setAttr ".r" 6.3190084466120737;
createNode aiAOVFilter -n "aiAOVFilter348";
	rename -uid "8D8C1893-435F-D773-229C-F78225FEE9CA";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -s -n "defaultArnoldFilter";
	rename -uid "98BA4ADB-46BC-90FC-1E11-83AF54A3BC8F";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVDriver -s -n "defaultArnoldDriver";
	rename -uid "F5B122C1-4882-40B4-EFCF-E5966A83DFA9";
	addAttr -ci true -sn "gamma" -ln "gamma" -dv 2.2000000476837158 -min 9.9999997473787516e-05 
		-smx 5 -at "float";
	addAttr -ci true -sn "dither_amplitude" -ln "ditherAmplitude" -dv 1 -at "float";
	setAttr ".ai_translator" -type "string" "tif";
createNode materialInfo -n "materialInfo2";
	rename -uid "9EBB4C92-4467-B0C8-D97A-D0955D068155";
createNode aiStandard -n "aiStandard2";
	rename -uid "D7CC54FC-48A2-4633-DA26-9B971E802D22";
createNode shadingEngine -n "aiStandard2SG";
	rename -uid "3EEE546D-4530-7ABC-9DDE-9D859AAD14DE";
	setAttr ".ihi" 0;
	setAttr ".ro" yes;
createNode materialInfo -n "materialInfo3";
	rename -uid "21C8E3B9-41F2-E126-8633-44AAEE2B1476";
createNode animCurveTL -n "pfxHair1_translateZ";
	rename -uid "4F16BD17-4CEA-CB30-1832-96A7703B0877";
	setAttr ".tan" 10;
	setAttr ".wgt" no;
	setAttr -s 2 ".ktv[0:1]"  1 4 3 -4;
createNode shapeEditorManager -n "shapeEditorManager";
	rename -uid "365CD5EE-4DA2-2612-98CC-A7AEDAE8961E";
createNode poseInterpolatorManager -n "poseInterpolatorManager";
	rename -uid "99851A0C-49F8-DC82-839F-9397542DDEE5";
createNode aiAOVDriver -s -n "defaultArnoldDisplayDriver";
	rename -uid "D787491C-4DF1-2072-2762-13B65F90AF7C";
	setAttr ".output_mode" 0;
	setAttr ".ai_translator" -type "string" "maya";
createNode nodeGraphEditorInfo -n "hyperShadePrimaryNodeEditorSavedTabsInfo";
	rename -uid "671A19A6-4660-30C3-6707-60A1222155D9";
	setAttr ".tgi[0].tn" -type "string" "Untitled_1";
	setAttr ".tgi[0].vl" -type "double2" -361.90474752395932 -203.57142048222713 ;
	setAttr ".tgi[0].vh" -type "double2" 363.09522366713026 202.38094433905621 ;
select -ne :time1;
	setAttr -av -k on ".cch";
	setAttr -cb on ".ihi";
	setAttr -k on ".nds";
	setAttr -cb on ".bnm";
	setAttr -k on ".o" 2;
	setAttr ".unw" 2;
select -ne :hardwareRenderingGlobals;
	setAttr ".vac" 2;
	setAttr ".etmr" no;
	setAttr ".tmr" 4096;
	setAttr ".fprt" yes;
select -ne :renderPartition;
	setAttr -k on ".cch";
	setAttr -cb on ".ihi";
	setAttr -k on ".nds";
	setAttr -cb on ".bnm";
	setAttr -s 4 ".st";
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
	setAttr -s 5 ".s";
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
select -ne :hardwareRenderGlobals;
	setAttr ".ctrs" 256;
	setAttr ".btrs" 512;
connectAttr "polySphere1.out" "pSphereShape1.i";
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
connectAttr "pfxHair1_translateZ.o" "pfxHair1.tz";
connectAttr "hairSystemShape1.orh" "pfxHairShape1.rhs";
relationship "link" ":lightLinker1" ":initialShadingGroup.message" ":defaultLightSet.message";
relationship "link" ":lightLinker1" ":initialParticleSE.message" ":defaultLightSet.message";
relationship "link" ":lightLinker1" "aiStandard1SG.message" ":defaultLightSet.message";
relationship "link" ":lightLinker1" "aiStandard2SG.message" ":defaultLightSet.message";
relationship "shadowLink" ":lightLinker1" ":initialShadingGroup.message" ":defaultLightSet.message";
relationship "shadowLink" ":lightLinker1" ":initialParticleSE.message" ":defaultLightSet.message";
relationship "shadowLink" ":lightLinker1" "aiStandard1SG.message" ":defaultLightSet.message";
relationship "shadowLink" ":lightLinker1" "aiStandard2SG.message" ":defaultLightSet.message";
connectAttr "layerManager.dli[0]" "defaultLayer.id";
connectAttr "renderLayerManager.rlmi[0]" "defaultRenderLayer.rlid";
connectAttr ":defaultArnoldFilter.msg" ":defaultArnoldRenderOptions.filt";
connectAttr ":defaultArnoldDriver.msg" ":defaultArnoldRenderOptions.drvr";
connectAttr ":defaultArnoldDisplayDriver.msg" ":defaultArnoldRenderOptions.drivers"
		 -na;
connectAttr "aiStandard1SG.msg" "materialInfo1.sg";
connectAttr "aiStandard2.out" "aiStandard2SG.ss";
connectAttr "pSphereShape1.iog" "aiStandard2SG.dsm" -na;
connectAttr "aiStandard2SG.msg" "materialInfo3.sg";
connectAttr "aiStandard2.msg" "materialInfo3.m";
connectAttr "aiStandard2.msg" "materialInfo3.t" -na;
connectAttr "aiStandard1SG.pa" ":renderPartition.st" -na;
connectAttr "aiStandard2SG.pa" ":renderPartition.st" -na;
connectAttr "aiStandard2.msg" ":defaultShaderList1.s" -na;
connectAttr "defaultRenderLayer.msg" ":defaultRenderingList1.r" -na;
connectAttr "directionalLightShape1.ltd" ":lightList1.l" -na;
connectAttr "directionalLight1.iog" ":defaultLightSet.dsm" -na;
// End of test.ma

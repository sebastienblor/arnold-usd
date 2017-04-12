//Maya ASCII 2017 scene
//Name: test.ma
//Last modified: Fri, Feb 17, 2017 05:08:21 PM
//Codeset: 1252
requires maya "2017";
requires -nodeType "aiOptions" -nodeType "aiAOVDriver" -nodeType "aiAOVFilter" -nodeType "aiStandard"
		 "mtoa" "2.0.0.0";
requires "stereoCamera" "10.0";
currentUnit -l centimeter -a degree -t film;
fileInfo "application" "maya";
fileInfo "product" "Maya 2017";
fileInfo "version" "2017";
fileInfo "cutIdentifier" "201606150345-997974";
fileInfo "osv" "Microsoft Windows 7 Enterprise Edition, 64-bit Windows 7 Service Pack 1 (Build 7601)\n";
createNode transform -s -n "persp";
	rename -uid "D2B48016-46D2-7458-13DC-939C5C08B2E2";
	setAttr ".v" no;
	setAttr ".t" -type "double3" 27.795094234407376 18.971162668046485 0 ;
	setAttr ".r" -type "double3" -24.000000000000004 90 0 ;
	setAttr ".rp" -type "double3" -3.5804692544161291e-015 -7.1054273576010019e-015 
		-1.4210854715202004e-014 ;
	setAttr ".rpt" -type "double3" -8.7783779362275568e-015 1.1498476670238775e-014 
		9.4020555375448207e-016 ;
createNode camera -s -n "perspShape" -p "persp";
	rename -uid "40B1762C-4337-E5BA-F538-619F60441BD0";
	setAttr -k off ".v" no;
	setAttr ".fl" 34.999999999999979;
	setAttr ".coi" 28.069721972594404;
	setAttr ".imn" -type "string" "persp";
	setAttr ".den" -type "string" "persp_depth";
	setAttr ".man" -type "string" "persp_mask";
	setAttr ".tp" -type "double3" 3.529134343177768 5.7733093735849303e-016 2.6000673943571693 ;
	setAttr ".hc" -type "string" "viewSet -p %camera";
	setAttr ".ai_translator" -type "string" "perspective";
createNode transform -s -n "top";
	rename -uid "4C3C3681-456E-31D0-1BE3-FE9F29EB115F";
	setAttr ".v" no;
	setAttr ".t" -type "double3" 0 100.1 0 ;
	setAttr ".r" -type "double3" -89.999999999999986 0 0 ;
createNode camera -s -n "topShape" -p "top";
	rename -uid "92D13FC4-42EF-7A42-890D-478433FDFC30";
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
	rename -uid "A88A6430-436E-A3AB-7D50-808777F40F8A";
	setAttr ".v" no;
	setAttr ".t" -type "double3" 0 0 100.1 ;
createNode camera -s -n "frontShape" -p "front";
	rename -uid "DF136FC4-48DA-C49E-5B8D-6393E9F21DA6";
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
	rename -uid "A305B910-444D-1A12-14CB-AE85D00EE012";
	setAttr ".v" no;
	setAttr ".t" -type "double3" 100.1 1.6871751536864319 12.910557697774426 ;
	setAttr ".r" -type "double3" 0 89.999999999999986 0 ;
createNode camera -s -n "sideShape" -p "side";
	rename -uid "8E38C9D0-4AC9-FD31-6946-5FAE8E8E94CE";
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
	rename -uid "0CB2BB8C-47BC-0D03-9DC0-FB944036C57F";
	setAttr ".t" -type "double3" -0.020665766309299727 6.3190084466120737 0.27269214094604877 ;
createNode mesh -n "pSphereShape1" -p "pSphere1";
	rename -uid "5001C560-4416-909B-43DB-EEAED7BD3161";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-min 0 -smx 1 -at "float";
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
	rename -uid "952E801D-411B-55C6-7D70-F895CC2908E2";
	setAttr ".r" -type "double3" -48.156552041713944 1.3059010322141336 -9.5202672381429512 ;
	setAttr ".s" -type "double3" 30.769077416337016 30.769077416337016 30.769077416337016 ;
createNode directionalLight -n "directionalLightShape1" -p "directionalLight1";
	rename -uid "F67C2CE8-46B1-FE60-92F4-B881913CCF85";
	addAttr -ci true -sn "ai_bounce_factor" -ln "aiBounceFactor" -dv 1 -min 0 -smx 
		20 -at "float";
	addAttr -ci true -sn "ai_bounces" -ln "aiBounces" -dv 999 -min 0 -max 10000 -at "long";
	setAttr -k off ".v";
	setAttr ".urs" no;
	setAttr ".rdl" 1;
createNode transform -n "hairSystem1";
	rename -uid "484304D3-4454-1626-28B3-8CB75DC34834";
createNode hairSystem -n "hairSystemShape1" -p "hairSystem1";
	rename -uid "347A226A-48D2-9E11-94DA-46916336D642";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
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
	setAttr ".opc" 0.9;
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
	rename -uid "0A92A183-43EE-DCC3-29AD-C4B35102E236";
createNode transform -n "pSphere1Follicle303" -p "hairSystem1Follicles";
	rename -uid "A7BFEA7E-425C-2C6F-8D4B-0799F83D6C12";
createNode follicle -n "pSphere1FollicleShape303" -p "pSphere1Follicle303";
	rename -uid "3424DC56-4A80-CC78-C8EC-FF8C740DB5F0";
	setAttr -k off ".v";
	setAttr ".pu" 0.03125;
	setAttr ".pv" 0.03125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve1" -p "pSphere1Follicle303";
	rename -uid "19E01C46-453A-AA50-12B4-7AA9E36472D7";
createNode nurbsCurve -n "curveShape1" -p "curve1";
	rename -uid "85FD7F63-4804-BEE8-42F4-E68292A18711";
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
	rename -uid "E6C35B2E-4902-DB66-A6B2-B2A96706506B";
createNode follicle -n "pSphere1FollicleShape309" -p "pSphere1Follicle309";
	rename -uid "71AB51ED-478E-AA00-61AD-8883948EE148";
	setAttr -k off ".v";
	setAttr ".pu" 0.03125;
	setAttr ".pv" 0.09375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve2" -p "pSphere1Follicle309";
	rename -uid "8B0DF7F2-4615-F529-5C58-3E94A8EBE401";
createNode nurbsCurve -n "curveShape2" -p "curve2";
	rename -uid "205DB7DA-4D96-BBA8-C7FE-A08F75B187FD";
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
	rename -uid "E2FF0B7F-4CC1-A34E-3867-C59FD239F5B6";
createNode follicle -n "pSphere1FollicleShape315" -p "pSphere1Follicle315";
	rename -uid "579FF0BB-4A02-8D56-30A9-00A2D45201F6";
	setAttr -k off ".v";
	setAttr ".pu" 0.03125;
	setAttr ".pv" 0.15625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve3" -p "pSphere1Follicle315";
	rename -uid "6E33CF6B-4DE0-F3DA-BBAA-A3B0C52EDE80";
createNode nurbsCurve -n "curveShape3" -p "curve3";
	rename -uid "31A5C7ED-46CF-BEEA-E586-6FB5FEF7F583";
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
	rename -uid "8557C205-4356-F177-4EA8-698910448831";
createNode follicle -n "pSphere1FollicleShape322" -p "pSphere1Follicle322";
	rename -uid "4D78C483-475F-33D4-CAA1-8EAE08D004F4";
	setAttr -k off ".v";
	setAttr ".pu" 0.03125;
	setAttr ".pv" 0.21875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve4" -p "pSphere1Follicle322";
	rename -uid "1FC3762A-45B1-C290-9E29-BC8114118651";
createNode nurbsCurve -n "curveShape4" -p "curve4";
	rename -uid "69321188-48F4-F051-A4CE-03AD05BA7B37";
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
	rename -uid "B090D1F8-43A2-BEE6-9199-F487B99CA52A";
createNode follicle -n "pSphere1FollicleShape328" -p "pSphere1Follicle328";
	rename -uid "6C3C0B5B-4877-1BD1-F734-6FBAE35168C6";
	setAttr -k off ".v";
	setAttr ".pu" 0.03125;
	setAttr ".pv" 0.28125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve5" -p "pSphere1Follicle328";
	rename -uid "9BFA7E1B-4DF3-B978-F02B-1FBAEF6F06C1";
createNode nurbsCurve -n "curveShape5" -p "curve5";
	rename -uid "365A59F0-4937-CCDA-4582-359DB3D699C2";
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
	rename -uid "95A20C02-4442-B4EB-EA5E-8D86108964E4";
createNode follicle -n "pSphere1FollicleShape334" -p "pSphere1Follicle334";
	rename -uid "D7B5B436-4ED9-5829-6CBB-DF834E25EEBA";
	setAttr -k off ".v";
	setAttr ".pu" 0.03125;
	setAttr ".pv" 0.34375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve6" -p "pSphere1Follicle334";
	rename -uid "EDE48ACA-49EE-F607-E354-789F1ACE9DA8";
createNode nurbsCurve -n "curveShape6" -p "curve6";
	rename -uid "88A728B4-4E85-F881-D5E0-2C8CB6416413";
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
	rename -uid "D0843166-4D09-AE0C-F53F-638DACC15A75";
createNode follicle -n "pSphere1FollicleShape340" -p "pSphere1Follicle340";
	rename -uid "AB28A4B9-48D7-7632-14DC-65B0E278E647";
	setAttr -k off ".v";
	setAttr ".pu" 0.03125;
	setAttr ".pv" 0.40625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve7" -p "pSphere1Follicle340";
	rename -uid "357DDCA5-4B76-9618-8587-70B2C1E175CD";
createNode nurbsCurve -n "curveShape7" -p "curve7";
	rename -uid "069CECD6-4442-6A56-AC21-8E889307E961";
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
	rename -uid "D7C16EE9-4D71-F325-4A36-0E8759B16FCE";
createNode follicle -n "pSphere1FollicleShape346" -p "pSphere1Follicle346";
	rename -uid "68BEB974-48DA-CDFF-ECC1-0CB87F24C2DB";
	setAttr -k off ".v";
	setAttr ".pu" 0.03125;
	setAttr ".pv" 0.46875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve8" -p "pSphere1Follicle346";
	rename -uid "484A5CB0-4DDE-5895-F4E9-B6A7D8CC8A9F";
createNode nurbsCurve -n "curveShape8" -p "curve8";
	rename -uid "7A2E0F47-46F9-B872-E8B9-61910DC0EA05";
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
	rename -uid "687D801C-421B-987B-6011-71AC1EFFF072";
createNode follicle -n "pSphere1FollicleShape353" -p "pSphere1Follicle353";
	rename -uid "65B38EFC-4D25-15DF-E11E-CEBEEEDD5436";
	setAttr -k off ".v";
	setAttr ".pu" 0.03125;
	setAttr ".pv" 0.53125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve9" -p "pSphere1Follicle353";
	rename -uid "0C0B9D51-47E7-2D77-C8F7-3CBC0BFA735F";
createNode nurbsCurve -n "curveShape9" -p "curve9";
	rename -uid "C5842A08-4D96-94ED-7322-35BBB5CF4EF1";
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
	rename -uid "1E0C453E-4870-F036-FB35-01843F8FFE68";
createNode follicle -n "pSphere1FollicleShape359" -p "pSphere1Follicle359";
	rename -uid "2AFFB21D-4954-C671-7AA2-149D7B87B625";
	setAttr -k off ".v";
	setAttr ".pu" 0.03125;
	setAttr ".pv" 0.59375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve10" -p "pSphere1Follicle359";
	rename -uid "66F6E1A7-4DFE-9D1A-6B7E-2EA8DE6E2EB8";
createNode nurbsCurve -n "curveShape10" -p "curve10";
	rename -uid "9B617C86-49F2-DFD2-F017-DCB1960CC34C";
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
	rename -uid "F8ED0381-41F3-8B8E-0995-B79D5C6CFC2A";
createNode follicle -n "pSphere1FollicleShape365" -p "pSphere1Follicle365";
	rename -uid "4B93EE1D-46D9-B6E9-747D-B18AA445024B";
	setAttr -k off ".v";
	setAttr ".pu" 0.03125;
	setAttr ".pv" 0.65625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve11" -p "pSphere1Follicle365";
	rename -uid "E6D4B8B3-44CA-212E-92B5-65B1DA002CB2";
createNode nurbsCurve -n "curveShape11" -p "curve11";
	rename -uid "E4A08F50-4D1F-641F-CDC5-E887738E4CD6";
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
	rename -uid "1B720F3D-4A4C-BC95-838A-C8BB4C358AF1";
createNode follicle -n "pSphere1FollicleShape371" -p "pSphere1Follicle371";
	rename -uid "A856384A-4121-6219-06A1-3EAD2F9D4524";
	setAttr -k off ".v";
	setAttr ".pu" 0.03125;
	setAttr ".pv" 0.71875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve12" -p "pSphere1Follicle371";
	rename -uid "D38BAD26-4611-EAC9-0646-669736426BE8";
createNode nurbsCurve -n "curveShape12" -p "curve12";
	rename -uid "4FE1D79D-496C-1804-8E31-B08C7E2B2296";
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
	rename -uid "38EC4053-4854-F308-11AC-9493A0792177";
createNode follicle -n "pSphere1FollicleShape377" -p "pSphere1Follicle377";
	rename -uid "A8092B9D-4156-6442-730B-FA9DF10989FD";
	setAttr -k off ".v";
	setAttr ".pu" 0.03125;
	setAttr ".pv" 0.78125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve13" -p "pSphere1Follicle377";
	rename -uid "D5E8F5D9-4D66-F2AA-A404-E3B7887A9E6B";
createNode nurbsCurve -n "curveShape13" -p "curve13";
	rename -uid "F7FE78B9-46C0-DE1F-D5E6-E2B7200099C6";
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
	rename -uid "02ABB795-4D5A-192E-C317-9B91AA34E886";
createNode follicle -n "pSphere1FollicleShape384" -p "pSphere1Follicle384";
	rename -uid "CD560C30-4781-83C4-F739-7EB30A4D9BD9";
	setAttr -k off ".v";
	setAttr ".pu" 0.03125;
	setAttr ".pv" 0.84375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve14" -p "pSphere1Follicle384";
	rename -uid "E0397D5B-48C0-5FA7-11DB-C89E41B8FDB7";
createNode nurbsCurve -n "curveShape14" -p "curve14";
	rename -uid "655A416A-47E4-0A16-4008-57828A65FCD1";
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
	rename -uid "DD3A72BF-4F9E-A5DD-A220-5FA85FC2547E";
createNode follicle -n "pSphere1FollicleShape390" -p "pSphere1Follicle390";
	rename -uid "C4B9BC62-4458-5418-DEEE-4A954381CFB7";
	setAttr -k off ".v";
	setAttr ".pu" 0.03125;
	setAttr ".pv" 0.90625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve15" -p "pSphere1Follicle390";
	rename -uid "95EE2164-414D-6BFB-F537-759D49EC52DB";
createNode nurbsCurve -n "curveShape15" -p "curve15";
	rename -uid "2CF6FE88-4657-D6C4-F570-D5A2CADB6B66";
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
	rename -uid "8C1B285C-4DE8-4D13-80E9-489D88BE5DE8";
createNode follicle -n "pSphere1FollicleShape396" -p "pSphere1Follicle396";
	rename -uid "2ED9CFE9-4F65-7413-BC77-60B183B468B3";
	setAttr -k off ".v";
	setAttr ".pu" 0.03125;
	setAttr ".pv" 0.96875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve16" -p "pSphere1Follicle396";
	rename -uid "BAE117B9-4E7B-9BA0-287C-A0AEB0CAF68E";
createNode nurbsCurve -n "curveShape16" -p "curve16";
	rename -uid "C7B18CEB-416D-A87A-586C-DF8BCD15E48A";
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
	rename -uid "A28FF8EB-420A-B551-0E49-0795003D1B31";
createNode follicle -n "pSphere1FollicleShape909" -p "pSphere1Follicle909";
	rename -uid "F4819BB7-4885-08A3-A423-7C9B0C53B8E8";
	setAttr -k off ".v";
	setAttr ".pu" 0.09375;
	setAttr ".pv" 0.09375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve17" -p "pSphere1Follicle909";
	rename -uid "9538EFDA-40B2-84AF-4754-BCA3C9CC142C";
createNode nurbsCurve -n "curveShape17" -p "curve17";
	rename -uid "EC6E5989-4529-6CC8-E35E-5BA0B9D92B6C";
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
	rename -uid "E4E25B1B-4240-7FB9-74F6-39875565D9EC";
createNode follicle -n "pSphere1FollicleShape915" -p "pSphere1Follicle915";
	rename -uid "8D7C69E9-4957-CBC1-CC76-DEADD4365507";
	setAttr -k off ".v";
	setAttr ".pu" 0.09375;
	setAttr ".pv" 0.15625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve18" -p "pSphere1Follicle915";
	rename -uid "137566E7-4C61-88F6-0D7E-FDADD69512F6";
createNode nurbsCurve -n "curveShape18" -p "curve18";
	rename -uid "CAACB64D-4131-891F-8818-58AFCF908560";
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
	rename -uid "5B0FC893-4E28-26E3-349E-958034FA3ACA";
createNode follicle -n "pSphere1FollicleShape922" -p "pSphere1Follicle922";
	rename -uid "1236739D-4B6B-94A4-B862-6CA41EDF55AF";
	setAttr -k off ".v";
	setAttr ".pu" 0.09375;
	setAttr ".pv" 0.21875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve19" -p "pSphere1Follicle922";
	rename -uid "B771896E-4F35-D663-B5EB-22A759C55F97";
createNode nurbsCurve -n "curveShape19" -p "curve19";
	rename -uid "E9BF9A5C-46F9-F760-E0D3-4C805DCB5D7E";
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
	rename -uid "0BAFCE3B-4B7A-4944-85BC-70ADEB317403";
createNode follicle -n "pSphere1FollicleShape928" -p "pSphere1Follicle928";
	rename -uid "F66A7F1B-4AD6-BA61-7BD4-758D69351029";
	setAttr -k off ".v";
	setAttr ".pu" 0.09375;
	setAttr ".pv" 0.28125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve20" -p "pSphere1Follicle928";
	rename -uid "31E308B0-4968-18C3-E3B1-7D944809774C";
createNode nurbsCurve -n "curveShape20" -p "curve20";
	rename -uid "781A1641-4FE4-71F1-E270-8FB02FA3AC4E";
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
	rename -uid "EF788A63-4167-5A96-85CC-4BA55239CE29";
createNode follicle -n "pSphere1FollicleShape934" -p "pSphere1Follicle934";
	rename -uid "C6626C13-4FBA-4656-62B5-A1A75F4BA36E";
	setAttr -k off ".v";
	setAttr ".pu" 0.09375;
	setAttr ".pv" 0.34375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve21" -p "pSphere1Follicle934";
	rename -uid "5DBF40A2-4A3A-C44D-8363-B9935E51AE34";
createNode nurbsCurve -n "curveShape21" -p "curve21";
	rename -uid "B87F27C6-4CB9-C1CE-E4FD-D391928C939E";
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
	rename -uid "191EBA0A-44BB-0F5C-B1B5-AC8CEF5CB79B";
createNode follicle -n "pSphere1FollicleShape940" -p "pSphere1Follicle940";
	rename -uid "B93CBDE8-4E4F-B372-3633-51908FBE03CE";
	setAttr -k off ".v";
	setAttr ".pu" 0.09375;
	setAttr ".pv" 0.40625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve22" -p "pSphere1Follicle940";
	rename -uid "29C29F03-4A41-D47A-5294-2384EF2E524F";
createNode nurbsCurve -n "curveShape22" -p "curve22";
	rename -uid "E4EF9A0B-495A-F0FB-D889-17BB0D5101CB";
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
	rename -uid "562E215A-4097-B38A-13C5-F1A117026693";
createNode follicle -n "pSphere1FollicleShape946" -p "pSphere1Follicle946";
	rename -uid "EC415ACD-4C4D-A080-E7E8-098ED470E4B0";
	setAttr -k off ".v";
	setAttr ".pu" 0.09375;
	setAttr ".pv" 0.46875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve23" -p "pSphere1Follicle946";
	rename -uid "36F34664-421E-1AA0-45C9-BCAC130B1369";
createNode nurbsCurve -n "curveShape23" -p "curve23";
	rename -uid "6FFE48F7-45EE-16D9-7D47-958D50D548DA";
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
	rename -uid "78C71C3D-48F7-9E19-245A-098C9546A949";
createNode follicle -n "pSphere1FollicleShape953" -p "pSphere1Follicle953";
	rename -uid "97FA2009-4275-2FF3-55D5-75BF2C6CE432";
	setAttr -k off ".v";
	setAttr ".pu" 0.09375;
	setAttr ".pv" 0.53125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve24" -p "pSphere1Follicle953";
	rename -uid "661366A7-4B5E-F87F-61DC-44B063FA159C";
createNode nurbsCurve -n "curveShape24" -p "curve24";
	rename -uid "7C29A2C0-4F85-28A3-346F-1B8AA09AB33C";
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
	rename -uid "58772367-4E38-0E5D-714A-22BE0C47A4BB";
createNode follicle -n "pSphere1FollicleShape959" -p "pSphere1Follicle959";
	rename -uid "5D8D415B-41B9-F9D9-1A4D-658B0B39B8F4";
	setAttr -k off ".v";
	setAttr ".pu" 0.09375;
	setAttr ".pv" 0.59375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve25" -p "pSphere1Follicle959";
	rename -uid "64951E58-439D-79B5-B62B-06AA55E0A9FD";
createNode nurbsCurve -n "curveShape25" -p "curve25";
	rename -uid "EB8B7EE7-4BE7-401C-845F-5EA07B8D55DE";
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
	rename -uid "C9466EEB-4F86-E733-EF18-559A156392D5";
createNode follicle -n "pSphere1FollicleShape965" -p "pSphere1Follicle965";
	rename -uid "6908A7D8-4CA1-B549-2767-43AB85A26891";
	setAttr -k off ".v";
	setAttr ".pu" 0.09375;
	setAttr ".pv" 0.65625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve26" -p "pSphere1Follicle965";
	rename -uid "477F6BC9-4B18-CE70-BB42-2896EBFD10E4";
createNode nurbsCurve -n "curveShape26" -p "curve26";
	rename -uid "2CCEEF2C-4463-A4C6-CF2C-9A8B295A7DBE";
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
	rename -uid "B6C247A1-447D-3221-E42A-36976F70A94B";
createNode follicle -n "pSphere1FollicleShape971" -p "pSphere1Follicle971";
	rename -uid "6223DBF8-4D16-EC56-D3D0-37A504EAAA89";
	setAttr -k off ".v";
	setAttr ".pu" 0.09375;
	setAttr ".pv" 0.71875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve27" -p "pSphere1Follicle971";
	rename -uid "95F245CD-41AF-FC62-EA49-B897AB0C3669";
createNode nurbsCurve -n "curveShape27" -p "curve27";
	rename -uid "193F9E15-42F2-C5D5-24A1-02935F528C11";
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
	rename -uid "B43F6AA0-4EE0-7349-288C-409B544E85BA";
createNode follicle -n "pSphere1FollicleShape977" -p "pSphere1Follicle977";
	rename -uid "B310D605-42A9-5F57-A83B-7CA1CE589949";
	setAttr -k off ".v";
	setAttr ".pu" 0.09375;
	setAttr ".pv" 0.78125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve28" -p "pSphere1Follicle977";
	rename -uid "18B61868-424A-7311-35D9-E280D0E70358";
createNode nurbsCurve -n "curveShape28" -p "curve28";
	rename -uid "BB798C4E-4D76-8BD0-887B-8F88000CB0D6";
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
	rename -uid "BD1466B1-4040-2890-71DF-BD9588AC107A";
createNode follicle -n "pSphere1FollicleShape984" -p "pSphere1Follicle984";
	rename -uid "F5EA03AA-4A86-AD34-9728-F4A17B609DA6";
	setAttr -k off ".v";
	setAttr ".pu" 0.09375;
	setAttr ".pv" 0.84375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve29" -p "pSphere1Follicle984";
	rename -uid "2E95BD55-4515-4E20-B689-57AD7895D6DF";
createNode nurbsCurve -n "curveShape29" -p "curve29";
	rename -uid "EB674B0E-4C24-2E20-91A0-C2965CD60191";
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
	rename -uid "7725093E-448C-2D98-C5AE-71B06F69EBA4";
createNode follicle -n "pSphere1FollicleShape990" -p "pSphere1Follicle990";
	rename -uid "933CF816-49E0-7875-013F-3F84B9602320";
	setAttr -k off ".v";
	setAttr ".pu" 0.09375;
	setAttr ".pv" 0.90625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve30" -p "pSphere1Follicle990";
	rename -uid "F1098918-45D3-3FBD-4641-D09B1656692C";
createNode nurbsCurve -n "curveShape30" -p "curve30";
	rename -uid "2FFC05FA-41FA-6412-7ECE-F68E72DB6093";
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
	rename -uid "2FBE18B8-47AF-76B7-2080-2485112737F7";
createNode follicle -n "pSphere1FollicleShape1509" -p "pSphere1Follicle1509";
	rename -uid "3B75053C-457F-CC87-5D29-18821358DA2C";
	setAttr -k off ".v";
	setAttr ".pu" 0.15625;
	setAttr ".pv" 0.09375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve31" -p "pSphere1Follicle1509";
	rename -uid "86A78286-4FCD-67E3-1A1C-E0B09EEB7A58";
createNode nurbsCurve -n "curveShape31" -p "curve31";
	rename -uid "CE5132C2-4877-DD12-B800-51BDA8777622";
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
	rename -uid "DB8E0B3B-4D57-A4B0-A2A1-CC9F5B79A498";
createNode follicle -n "pSphere1FollicleShape1515" -p "pSphere1Follicle1515";
	rename -uid "46F79E36-4A46-24B1-8D0A-F1BBD6131B65";
	setAttr -k off ".v";
	setAttr ".pu" 0.15625;
	setAttr ".pv" 0.15625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve32" -p "pSphere1Follicle1515";
	rename -uid "A7913C4F-40B4-1D6C-5AE1-E8ACB27A0B28";
createNode nurbsCurve -n "curveShape32" -p "curve32";
	rename -uid "C2092D8E-4A33-F3B4-284B-3891C9C5890E";
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
	rename -uid "FAD8D5B2-4AC8-36B8-13F7-21B0C4AE0133";
createNode follicle -n "pSphere1FollicleShape1522" -p "pSphere1Follicle1522";
	rename -uid "D6EEFB65-45C7-1D38-04AD-AB847D118749";
	setAttr -k off ".v";
	setAttr ".pu" 0.15625;
	setAttr ".pv" 0.21875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve33" -p "pSphere1Follicle1522";
	rename -uid "088D78D6-4CA4-847D-1F94-609F30C81127";
createNode nurbsCurve -n "curveShape33" -p "curve33";
	rename -uid "91D93177-4B3A-7600-8D6E-0E9262C0C551";
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
	rename -uid "0FB51692-4596-F3CE-0343-82ABE73E7883";
createNode follicle -n "pSphere1FollicleShape1528" -p "pSphere1Follicle1528";
	rename -uid "A91064E2-48B2-29C2-30B6-8DB86EA87D05";
	setAttr -k off ".v";
	setAttr ".pu" 0.15625;
	setAttr ".pv" 0.28125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve34" -p "pSphere1Follicle1528";
	rename -uid "EC5AA716-4F48-8EAB-820D-CCA9BAA8CFE5";
createNode nurbsCurve -n "curveShape34" -p "curve34";
	rename -uid "EEC57756-45BD-5287-BDB1-6C882E80D7BB";
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
	rename -uid "C574CE38-4EF2-BAF9-FBD4-9EA3971C550A";
createNode follicle -n "pSphere1FollicleShape1534" -p "pSphere1Follicle1534";
	rename -uid "0632F48D-4D42-7EA5-8414-ADB1FA1E8A0D";
	setAttr -k off ".v";
	setAttr ".pu" 0.15625;
	setAttr ".pv" 0.34375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve35" -p "pSphere1Follicle1534";
	rename -uid "800E7409-4E27-E202-1D44-DABEB2DE9AB3";
createNode nurbsCurve -n "curveShape35" -p "curve35";
	rename -uid "079D117C-4EB3-EDAE-033F-559538C9284D";
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
	rename -uid "F8E0AC62-4A8C-0A0E-0209-4F9C49A285B2";
createNode follicle -n "pSphere1FollicleShape1540" -p "pSphere1Follicle1540";
	rename -uid "F6068281-4A25-D50D-C2E6-9BB639EADB36";
	setAttr -k off ".v";
	setAttr ".pu" 0.15625;
	setAttr ".pv" 0.40625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve36" -p "pSphere1Follicle1540";
	rename -uid "F3C7D840-46FD-272E-0164-618AA4824622";
createNode nurbsCurve -n "curveShape36" -p "curve36";
	rename -uid "9C85A0B6-4274-854B-B7AD-F3B044C7D214";
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
	rename -uid "6170DA3D-4FC0-B70B-B522-79B67DE129AD";
createNode follicle -n "pSphere1FollicleShape1546" -p "pSphere1Follicle1546";
	rename -uid "549E90A4-4BF1-6210-D71D-BE835CC297F8";
	setAttr -k off ".v";
	setAttr ".pu" 0.15625;
	setAttr ".pv" 0.46875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve37" -p "pSphere1Follicle1546";
	rename -uid "5FCD492A-41DD-5704-E815-13924A8993E6";
createNode nurbsCurve -n "curveShape37" -p "curve37";
	rename -uid "8CDA1ED3-464B-7466-6CE5-8692FD41881D";
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
	rename -uid "E6FEBCCE-4833-D3F8-C2AD-B29C2392D764";
createNode follicle -n "pSphere1FollicleShape1553" -p "pSphere1Follicle1553";
	rename -uid "70C544FA-4B48-B86C-0931-7DBEA0834E4C";
	setAttr -k off ".v";
	setAttr ".pu" 0.15625;
	setAttr ".pv" 0.53125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve38" -p "pSphere1Follicle1553";
	rename -uid "8B6C7E69-454A-20E7-3989-B294BE28CF97";
createNode nurbsCurve -n "curveShape38" -p "curve38";
	rename -uid "9F55E30B-4B2B-0000-E84A-3689569B6A3E";
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
	rename -uid "24B86605-41B0-480E-E196-D0A40C3C43BB";
createNode follicle -n "pSphere1FollicleShape1559" -p "pSphere1Follicle1559";
	rename -uid "7C7E3E7A-474D-559B-07D0-BD878651EEDE";
	setAttr -k off ".v";
	setAttr ".pu" 0.15625;
	setAttr ".pv" 0.59375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve39" -p "pSphere1Follicle1559";
	rename -uid "06AA242D-42C8-013E-B5DB-1691DB07A445";
createNode nurbsCurve -n "curveShape39" -p "curve39";
	rename -uid "25F041B1-41A5-598B-1A20-99B9FFC062E3";
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
	rename -uid "A788D4E3-44AF-2896-A59B-EB8E98EFEE49";
createNode follicle -n "pSphere1FollicleShape1565" -p "pSphere1Follicle1565";
	rename -uid "FC19056B-4040-6A44-0DC4-C4B772E58689";
	setAttr -k off ".v";
	setAttr ".pu" 0.15625;
	setAttr ".pv" 0.65625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve40" -p "pSphere1Follicle1565";
	rename -uid "3D9BB031-4FEE-892C-8211-EE9F976624B2";
createNode nurbsCurve -n "curveShape40" -p "curve40";
	rename -uid "D6704DAC-46B4-312D-FCDC-39AE97297BE7";
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
	rename -uid "852F533F-469E-E55B-F32A-4AA2DB438B33";
createNode follicle -n "pSphere1FollicleShape1571" -p "pSphere1Follicle1571";
	rename -uid "71C64D2E-48EF-ED83-4CF6-0389092034E0";
	setAttr -k off ".v";
	setAttr ".pu" 0.15625;
	setAttr ".pv" 0.71875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve41" -p "pSphere1Follicle1571";
	rename -uid "5A5DB415-468F-7563-6362-7EB57AAD2BAC";
createNode nurbsCurve -n "curveShape41" -p "curve41";
	rename -uid "E70EB138-4E55-C66E-A116-D3829FD665E0";
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
	rename -uid "DA228F30-46F7-69A2-EAD3-86ABB0D9B40D";
createNode follicle -n "pSphere1FollicleShape1577" -p "pSphere1Follicle1577";
	rename -uid "CECA5687-437D-C3B7-6175-958BC026F481";
	setAttr -k off ".v";
	setAttr ".pu" 0.15625;
	setAttr ".pv" 0.78125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve42" -p "pSphere1Follicle1577";
	rename -uid "0DCBC732-4D0D-2EBC-82C2-779EDFA0278B";
createNode nurbsCurve -n "curveShape42" -p "curve42";
	rename -uid "CCD693EE-48B4-3B0E-091B-A89086E5DCBF";
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
	rename -uid "F555AD3C-4BF8-38CE-5BA8-50A7CB0491DA";
createNode follicle -n "pSphere1FollicleShape1584" -p "pSphere1Follicle1584";
	rename -uid "B8E4A167-41C2-7DE9-097C-D59C3A4BC508";
	setAttr -k off ".v";
	setAttr ".pu" 0.15625;
	setAttr ".pv" 0.84375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve43" -p "pSphere1Follicle1584";
	rename -uid "031A5002-46CC-CA75-DBE0-AFA6CC32B60D";
createNode nurbsCurve -n "curveShape43" -p "curve43";
	rename -uid "8041D884-495F-9E31-B6C2-A882E973FDF8";
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
	rename -uid "C3A2E95C-4B34-2DA4-BB00-8E948DC123B1";
createNode follicle -n "pSphere1FollicleShape1590" -p "pSphere1Follicle1590";
	rename -uid "51EFC626-4937-4F14-EAEA-18ADF403993A";
	setAttr -k off ".v";
	setAttr ".pu" 0.15625;
	setAttr ".pv" 0.90625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve44" -p "pSphere1Follicle1590";
	rename -uid "4E74CC53-4202-FF9F-D341-F0AB7B587CFA";
createNode nurbsCurve -n "curveShape44" -p "curve44";
	rename -uid "4A59047D-4707-9482-1B1F-65A5B7E42329";
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
	rename -uid "726A6DC6-4594-9EC4-D008-F4B87921BDDB";
createNode follicle -n "pSphere1FollicleShape2203" -p "pSphere1Follicle2203";
	rename -uid "22FA6B01-4BAC-AA63-38B6-01BF3FC4EF3E";
	setAttr -k off ".v";
	setAttr ".pu" 0.21875;
	setAttr ".pv" 0.03125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve45" -p "pSphere1Follicle2203";
	rename -uid "3282799D-4363-AA9F-3DB9-CC86735AFA28";
createNode nurbsCurve -n "curveShape45" -p "curve45";
	rename -uid "D25D0FFE-4EB6-52E4-6C6A-1A8B623EAA7A";
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
	rename -uid "B1C4A3CD-4CBB-E7F9-99D9-298D2BC7B591";
createNode follicle -n "pSphere1FollicleShape2209" -p "pSphere1Follicle2209";
	rename -uid "BBF3F3E0-4ECF-8089-86E7-5A8D943EEE23";
	setAttr -k off ".v";
	setAttr ".pu" 0.21875;
	setAttr ".pv" 0.09375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve46" -p "pSphere1Follicle2209";
	rename -uid "D4EF156E-42B3-B549-7410-F885DA9BDB3B";
createNode nurbsCurve -n "curveShape46" -p "curve46";
	rename -uid "BA94DE7C-4333-6D29-73B9-48A5A87619A2";
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
	rename -uid "AE8E459C-4FDA-82F1-269A-88928BCFFCFE";
createNode follicle -n "pSphere1FollicleShape2215" -p "pSphere1Follicle2215";
	rename -uid "F21C86C3-4C4F-ACB7-2E32-ADB325881CAD";
	setAttr -k off ".v";
	setAttr ".pu" 0.21875;
	setAttr ".pv" 0.15625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve47" -p "pSphere1Follicle2215";
	rename -uid "F3D85674-4C71-A0B5-5AF9-8BA2841D0C8F";
createNode nurbsCurve -n "curveShape47" -p "curve47";
	rename -uid "C8D12F6A-486B-908B-344D-1CAC32F40BB0";
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
	rename -uid "E3FEB215-4F01-0C94-E449-F48D37EA75D9";
createNode follicle -n "pSphere1FollicleShape2222" -p "pSphere1Follicle2222";
	rename -uid "010B2140-40F2-CE50-6A26-8986E8B1C3C9";
	setAttr -k off ".v";
	setAttr ".pu" 0.21875;
	setAttr ".pv" 0.21875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve48" -p "pSphere1Follicle2222";
	rename -uid "2AF44C5F-47FF-F9C1-3C98-48B812DDE0DE";
createNode nurbsCurve -n "curveShape48" -p "curve48";
	rename -uid "C2E46CB9-4E85-E3FD-C218-7B8E64B5E59E";
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
	rename -uid "BCC12E3C-4917-16F8-8D79-26A140386153";
createNode follicle -n "pSphere1FollicleShape2228" -p "pSphere1Follicle2228";
	rename -uid "44BAD4CE-4776-ABE8-3D4B-7EB3AF261B2A";
	setAttr -k off ".v";
	setAttr ".pu" 0.21875;
	setAttr ".pv" 0.28125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve49" -p "pSphere1Follicle2228";
	rename -uid "9D4E0982-415E-49B7-9D9E-D5AFFBE8DE27";
createNode nurbsCurve -n "curveShape49" -p "curve49";
	rename -uid "96A6C9FB-47D3-CC4B-773E-35BF28D5E83E";
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
	rename -uid "370F1EE5-4495-1E42-6EC3-C98BC167E03A";
createNode follicle -n "pSphere1FollicleShape2234" -p "pSphere1Follicle2234";
	rename -uid "69E07D1A-4276-8CC7-E1A2-A28184F2A6AC";
	setAttr -k off ".v";
	setAttr ".pu" 0.21875;
	setAttr ".pv" 0.34375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve50" -p "pSphere1Follicle2234";
	rename -uid "460807D6-4071-E9B5-4C88-E38A4023887B";
createNode nurbsCurve -n "curveShape50" -p "curve50";
	rename -uid "23301255-4CD0-5103-1331-51BB43DE53A3";
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
	rename -uid "DC894873-4327-6213-10D5-F2992FAAD269";
createNode follicle -n "pSphere1FollicleShape2240" -p "pSphere1Follicle2240";
	rename -uid "7BBAA08D-4FDF-872F-BF9F-FC8ACD5CBAA9";
	setAttr -k off ".v";
	setAttr ".pu" 0.21875;
	setAttr ".pv" 0.40625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve51" -p "pSphere1Follicle2240";
	rename -uid "C5BEC814-4A72-C4DB-16AE-0F96E4F232B2";
createNode nurbsCurve -n "curveShape51" -p "curve51";
	rename -uid "9A73378B-443D-E767-4F33-5B946471F2C7";
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
	rename -uid "F16B181D-46A1-6FEF-0D09-A590CA13DCBC";
createNode follicle -n "pSphere1FollicleShape2246" -p "pSphere1Follicle2246";
	rename -uid "26E4BBAE-4FCB-B8CE-0ADA-6A92C3928F38";
	setAttr -k off ".v";
	setAttr ".pu" 0.21875;
	setAttr ".pv" 0.46875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve52" -p "pSphere1Follicle2246";
	rename -uid "01A18B82-4D59-F19E-CB21-758F40A9EEFB";
createNode nurbsCurve -n "curveShape52" -p "curve52";
	rename -uid "939DD1F1-4416-78A9-5925-3D907D18032B";
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
	rename -uid "67CA9C1B-4EC8-7A40-37D6-2BB892AFE183";
createNode follicle -n "pSphere1FollicleShape2253" -p "pSphere1Follicle2253";
	rename -uid "E8D4EFEB-4C1B-5BAF-7583-7AA45C63BC7F";
	setAttr -k off ".v";
	setAttr ".pu" 0.21875;
	setAttr ".pv" 0.53125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve53" -p "pSphere1Follicle2253";
	rename -uid "2FF21372-4B13-0D83-F67B-13A55F83BEAA";
createNode nurbsCurve -n "curveShape53" -p "curve53";
	rename -uid "86FE01ED-429D-F676-95BF-C38171810C80";
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
	rename -uid "3F9DC369-4FC1-B96A-10D0-D480219D4863";
createNode follicle -n "pSphere1FollicleShape2259" -p "pSphere1Follicle2259";
	rename -uid "8A38EAAC-442F-2575-5022-E09DAAB9E95C";
	setAttr -k off ".v";
	setAttr ".pu" 0.21875;
	setAttr ".pv" 0.59375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve54" -p "pSphere1Follicle2259";
	rename -uid "1C464087-4D08-D7FF-F5FA-FFA46B7928C3";
createNode nurbsCurve -n "curveShape54" -p "curve54";
	rename -uid "E1F3F97F-443D-E687-9706-388AFEB326E4";
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
	rename -uid "8A98B13B-420A-F39D-7D40-C283B1A9B6DB";
createNode follicle -n "pSphere1FollicleShape2265" -p "pSphere1Follicle2265";
	rename -uid "725C2D24-4510-240D-84EC-2C83DECADFA6";
	setAttr -k off ".v";
	setAttr ".pu" 0.21875;
	setAttr ".pv" 0.65625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve55" -p "pSphere1Follicle2265";
	rename -uid "FD22DC19-4D04-B96C-14AB-19BBFA16B54F";
createNode nurbsCurve -n "curveShape55" -p "curve55";
	rename -uid "E33FAD8A-4FA1-6925-18BE-DF9A25C327F8";
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
	rename -uid "E880AAB7-4DB9-B807-7E4C-BBA5C108F915";
createNode follicle -n "pSphere1FollicleShape2271" -p "pSphere1Follicle2271";
	rename -uid "750F91BB-4B5E-C205-7317-AF8A990FF675";
	setAttr -k off ".v";
	setAttr ".pu" 0.21875;
	setAttr ".pv" 0.71875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve56" -p "pSphere1Follicle2271";
	rename -uid "928B4717-4433-F137-3439-7BAC56205F54";
createNode nurbsCurve -n "curveShape56" -p "curve56";
	rename -uid "A4B7951D-45F6-CD08-BD9A-B985BEA57695";
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
	rename -uid "6A9CEF7F-40A5-F9BF-F277-7EBEA9CB74DA";
createNode follicle -n "pSphere1FollicleShape2277" -p "pSphere1Follicle2277";
	rename -uid "A09CDADF-4E67-4C72-21D8-1CB55C18B9C6";
	setAttr -k off ".v";
	setAttr ".pu" 0.21875;
	setAttr ".pv" 0.78125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve57" -p "pSphere1Follicle2277";
	rename -uid "EBE14318-4C63-50C9-42ED-F2B4202C3A83";
createNode nurbsCurve -n "curveShape57" -p "curve57";
	rename -uid "93B40A97-42E0-2BF0-5CA8-96B8C3C0453C";
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
	rename -uid "E2A74ACA-4818-EE3B-139A-43892BC2D5D2";
createNode follicle -n "pSphere1FollicleShape2284" -p "pSphere1Follicle2284";
	rename -uid "8CEDBB55-467B-03AC-00F3-9A818993DFDD";
	setAttr -k off ".v";
	setAttr ".pu" 0.21875;
	setAttr ".pv" 0.84375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve58" -p "pSphere1Follicle2284";
	rename -uid "CBE14095-4A89-271B-391A-378EC03FCFA4";
createNode nurbsCurve -n "curveShape58" -p "curve58";
	rename -uid "E0C59757-4983-5F65-EE38-6781CF19CA33";
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
	rename -uid "127749D2-4F24-F9DA-AA80-E481801BCE08";
createNode follicle -n "pSphere1FollicleShape2290" -p "pSphere1Follicle2290";
	rename -uid "305C4175-480E-49DD-1946-47BD24C8B6F2";
	setAttr -k off ".v";
	setAttr ".pu" 0.21875;
	setAttr ".pv" 0.90625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve59" -p "pSphere1Follicle2290";
	rename -uid "A078430B-4B4E-B722-2AB9-F88F88DFD2A8";
createNode nurbsCurve -n "curveShape59" -p "curve59";
	rename -uid "C3383EA7-46A0-7D1E-7F7D-2EAA3C21908B";
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
	rename -uid "392E839E-4CBB-5DDA-0B7E-53BA17C7E86C";
createNode follicle -n "pSphere1FollicleShape2296" -p "pSphere1Follicle2296";
	rename -uid "D89675D2-4654-6BB8-CB5E-EC9B45410860";
	setAttr -k off ".v";
	setAttr ".pu" 0.21875;
	setAttr ".pv" 0.96875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve60" -p "pSphere1Follicle2296";
	rename -uid "0074D4EC-40C6-760D-8BDE-92AE7FF3AF17";
createNode nurbsCurve -n "curveShape60" -p "curve60";
	rename -uid "C6545076-4FB9-89EE-BDDF-D6BC577A20AC";
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
	rename -uid "2B1C64E5-475E-72E3-1D0F-46AF8E260AA0";
createNode follicle -n "pSphere1FollicleShape2803" -p "pSphere1Follicle2803";
	rename -uid "44BA9EB7-4F77-31B0-A0B6-A29DA173304A";
	setAttr -k off ".v";
	setAttr ".pu" 0.28125;
	setAttr ".pv" 0.03125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve61" -p "pSphere1Follicle2803";
	rename -uid "7F6EC10A-4C6E-5A5A-CAD9-99A46B52A192";
createNode nurbsCurve -n "curveShape61" -p "curve61";
	rename -uid "EA5B4DE0-4D35-B854-D08B-91842416289D";
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
	rename -uid "76253F62-4D57-AD96-37A6-C6A847AA8CA4";
createNode follicle -n "pSphere1FollicleShape2809" -p "pSphere1Follicle2809";
	rename -uid "34601358-4876-9BE3-B579-D0B9AD70046D";
	setAttr -k off ".v";
	setAttr ".pu" 0.28125;
	setAttr ".pv" 0.09375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve62" -p "pSphere1Follicle2809";
	rename -uid "4564C3E9-4E08-BE36-92C8-AA933E3D2B67";
createNode nurbsCurve -n "curveShape62" -p "curve62";
	rename -uid "A9E9F87B-45D9-5525-7F5D-F58D56363E72";
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
	rename -uid "1174DA4D-463E-E1BF-F385-F89F9986B89A";
createNode follicle -n "pSphere1FollicleShape2815" -p "pSphere1Follicle2815";
	rename -uid "F8D67157-43BA-7735-3817-6596C359E710";
	setAttr -k off ".v";
	setAttr ".pu" 0.28125;
	setAttr ".pv" 0.15625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve63" -p "pSphere1Follicle2815";
	rename -uid "E32C62F0-4C52-B49A-EEA3-76B52FCCAAA9";
createNode nurbsCurve -n "curveShape63" -p "curve63";
	rename -uid "F32EA26D-44FA-2695-9978-11A9B55544F0";
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
	rename -uid "7193513C-4D50-3C7E-80F9-159D9F364DC3";
createNode follicle -n "pSphere1FollicleShape2822" -p "pSphere1Follicle2822";
	rename -uid "7E47CC0F-498E-7343-50F9-138039322600";
	setAttr -k off ".v";
	setAttr ".pu" 0.28125;
	setAttr ".pv" 0.21875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve64" -p "pSphere1Follicle2822";
	rename -uid "8BB63F5D-4DF1-5D19-2194-1BAA3D989A2B";
createNode nurbsCurve -n "curveShape64" -p "curve64";
	rename -uid "6F2FAF9F-4EB6-7043-BC6B-A68ADDF7344C";
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
	rename -uid "072CEFC6-497D-D452-6F30-D49C7F91D4C5";
createNode follicle -n "pSphere1FollicleShape2828" -p "pSphere1Follicle2828";
	rename -uid "0E539686-4AA6-1173-A44B-BE9BFDAB6CE7";
	setAttr -k off ".v";
	setAttr ".pu" 0.28125;
	setAttr ".pv" 0.28125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve65" -p "pSphere1Follicle2828";
	rename -uid "5FDD3C8A-428A-D1D4-B5C9-719275891E57";
createNode nurbsCurve -n "curveShape65" -p "curve65";
	rename -uid "CF4BA852-4384-1487-4897-938A16A21A15";
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
	rename -uid "321FF946-4208-7E2D-127A-03AC157A9802";
createNode follicle -n "pSphere1FollicleShape2834" -p "pSphere1Follicle2834";
	rename -uid "48E09FB6-449B-6B24-E138-0F962AC80BBC";
	setAttr -k off ".v";
	setAttr ".pu" 0.28125;
	setAttr ".pv" 0.34375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve66" -p "pSphere1Follicle2834";
	rename -uid "8EF544C8-41B4-2988-0ECA-F69C4BF2239B";
createNode nurbsCurve -n "curveShape66" -p "curve66";
	rename -uid "693F6306-4933-B1D4-55AC-169929B79241";
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
	rename -uid "6BFC25DF-4E19-257F-9F73-DB8C5068DEFD";
createNode follicle -n "pSphere1FollicleShape2840" -p "pSphere1Follicle2840";
	rename -uid "683B3CB1-4254-3FB3-0412-B19E2114035F";
	setAttr -k off ".v";
	setAttr ".pu" 0.28125;
	setAttr ".pv" 0.40625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve67" -p "pSphere1Follicle2840";
	rename -uid "84C1B351-4060-203D-E3AC-01A1F9EBBD73";
createNode nurbsCurve -n "curveShape67" -p "curve67";
	rename -uid "7E0DB532-440F-6D35-4FEC-C5A002E5564D";
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
	rename -uid "F36F8376-4F15-F0CE-E81F-6DA70CFF9832";
createNode follicle -n "pSphere1FollicleShape2846" -p "pSphere1Follicle2846";
	rename -uid "968CC512-4F39-E1C4-DC4C-81AF5B6C6A0B";
	setAttr -k off ".v";
	setAttr ".pu" 0.28125;
	setAttr ".pv" 0.46875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve68" -p "pSphere1Follicle2846";
	rename -uid "B8A94BF4-45B0-8C4B-330D-D5921F21D2B6";
createNode nurbsCurve -n "curveShape68" -p "curve68";
	rename -uid "C109E082-4AA8-B5A1-061D-F281521FC0EC";
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
	rename -uid "CC69C68D-4685-513F-6073-C69CD150A94C";
createNode follicle -n "pSphere1FollicleShape2853" -p "pSphere1Follicle2853";
	rename -uid "7CDE7D7C-4857-5117-7B4F-818511D7A224";
	setAttr -k off ".v";
	setAttr ".pu" 0.28125;
	setAttr ".pv" 0.53125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve69" -p "pSphere1Follicle2853";
	rename -uid "28E1EE87-453A-E2BE-ABFD-12AE7E856B17";
createNode nurbsCurve -n "curveShape69" -p "curve69";
	rename -uid "E82A09FF-4386-47DF-554E-E48EB4731C2A";
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
	rename -uid "EDA16F87-4084-DCC2-755B-B49D63BC1D1E";
createNode follicle -n "pSphere1FollicleShape2859" -p "pSphere1Follicle2859";
	rename -uid "87025971-46E4-5A63-2608-DA93A501082C";
	setAttr -k off ".v";
	setAttr ".pu" 0.28125;
	setAttr ".pv" 0.59375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve70" -p "pSphere1Follicle2859";
	rename -uid "294E98E4-4267-9A43-AAE8-F8866D18C177";
createNode nurbsCurve -n "curveShape70" -p "curve70";
	rename -uid "31A7DB89-46E0-BB5C-336E-C480EECE40B0";
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
	rename -uid "FA6CE379-45BA-0AE9-C5BE-97883F930660";
createNode follicle -n "pSphere1FollicleShape2865" -p "pSphere1Follicle2865";
	rename -uid "979BFEC2-402A-DF55-0E39-5CBD5B091431";
	setAttr -k off ".v";
	setAttr ".pu" 0.28125;
	setAttr ".pv" 0.65625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve71" -p "pSphere1Follicle2865";
	rename -uid "B1C3EE83-41DC-7FF0-090C-73B307572EC2";
createNode nurbsCurve -n "curveShape71" -p "curve71";
	rename -uid "DC2227C4-41A9-185D-8308-3592B19FB961";
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
	rename -uid "5E279C83-4764-468C-8F6D-568D5F4B15D7";
createNode follicle -n "pSphere1FollicleShape2871" -p "pSphere1Follicle2871";
	rename -uid "EC6C9DB4-4A35-2840-5F1B-0EA2E468C113";
	setAttr -k off ".v";
	setAttr ".pu" 0.28125;
	setAttr ".pv" 0.71875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve72" -p "pSphere1Follicle2871";
	rename -uid "BBF534C7-46A5-9F97-7E8D-268147D79D12";
createNode nurbsCurve -n "curveShape72" -p "curve72";
	rename -uid "182541C0-41C8-4141-17B2-DAA03A758469";
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
	rename -uid "A0A93A9A-40DA-7E7D-ED47-E6AAB0EB9DC9";
createNode follicle -n "pSphere1FollicleShape2877" -p "pSphere1Follicle2877";
	rename -uid "6338A6C3-4130-2044-01D5-159D05E514D0";
	setAttr -k off ".v";
	setAttr ".pu" 0.28125;
	setAttr ".pv" 0.78125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve73" -p "pSphere1Follicle2877";
	rename -uid "3CF08822-4D80-A7BB-9D1F-D8ACC1EAF1D1";
createNode nurbsCurve -n "curveShape73" -p "curve73";
	rename -uid "7A21CCF2-4C11-01E7-995C-D9BD609C4DF8";
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
	rename -uid "35199867-4585-9CEB-79F6-7A9562AA1723";
createNode follicle -n "pSphere1FollicleShape2884" -p "pSphere1Follicle2884";
	rename -uid "648271F8-4DC6-12D6-3D08-A08CD3DA68FA";
	setAttr -k off ".v";
	setAttr ".pu" 0.28125;
	setAttr ".pv" 0.84375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve74" -p "pSphere1Follicle2884";
	rename -uid "A3B27DA6-4F37-70E6-1F48-F685E62EDAB8";
createNode nurbsCurve -n "curveShape74" -p "curve74";
	rename -uid "260DB3F6-41A5-1B41-8493-41883E2094D5";
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
	rename -uid "09727238-4A0F-A11C-4B81-88B2E08BFB16";
createNode follicle -n "pSphere1FollicleShape2890" -p "pSphere1Follicle2890";
	rename -uid "CC8E6189-4E30-0DC6-F14D-088CA0E7A5A3";
	setAttr -k off ".v";
	setAttr ".pu" 0.28125;
	setAttr ".pv" 0.90625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve75" -p "pSphere1Follicle2890";
	rename -uid "58B88FAF-4303-7189-62E1-328C48997E61";
createNode nurbsCurve -n "curveShape75" -p "curve75";
	rename -uid "61C3EF27-456E-B238-83C1-9FB2BE30AE5A";
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
	rename -uid "A7B06F9F-4EFA-A977-8B40-81AC9C97401C";
createNode follicle -n "pSphere1FollicleShape2896" -p "pSphere1Follicle2896";
	rename -uid "37AF063F-4F42-9149-4B61-2B8EDA7B1DD6";
	setAttr -k off ".v";
	setAttr ".pu" 0.28125;
	setAttr ".pv" 0.96875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve76" -p "pSphere1Follicle2896";
	rename -uid "8E9CA718-4FB9-1EFE-78A1-98A2614FA3D3";
createNode nurbsCurve -n "curveShape76" -p "curve76";
	rename -uid "524F34B8-470C-5E56-C067-3087D926A79C";
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
	rename -uid "8B8B25F4-48AE-AB95-CEA5-0CA5325E24C0";
createNode follicle -n "pSphere1FollicleShape3409" -p "pSphere1Follicle3409";
	rename -uid "F5390B57-4822-F15A-37DA-969AF1B48772";
	setAttr -k off ".v";
	setAttr ".pu" 0.34375;
	setAttr ".pv" 0.09375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve77" -p "pSphere1Follicle3409";
	rename -uid "1EDC3F5A-4A66-791B-F73E-D4A31151968F";
createNode nurbsCurve -n "curveShape77" -p "curve77";
	rename -uid "1F3F8AEE-428D-D375-9B9D-E4AB6C01ADDE";
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
	rename -uid "DD784238-462B-5CD5-FF41-7CB2C11A6AB0";
createNode follicle -n "pSphere1FollicleShape3415" -p "pSphere1Follicle3415";
	rename -uid "EC3319F5-4DFF-CF66-828D-39AA0EC94915";
	setAttr -k off ".v";
	setAttr ".pu" 0.34375;
	setAttr ".pv" 0.15625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve78" -p "pSphere1Follicle3415";
	rename -uid "4E22410A-4E0D-94D1-E059-5B9519F1D72E";
createNode nurbsCurve -n "curveShape78" -p "curve78";
	rename -uid "2065B4E1-4B7B-6B7F-29DF-B38A5F031645";
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
	rename -uid "3288089E-48A0-F646-CFFA-DC98014FF9FB";
createNode follicle -n "pSphere1FollicleShape3422" -p "pSphere1Follicle3422";
	rename -uid "1418AC9B-4CB0-9864-0F6E-67A55ED5036D";
	setAttr -k off ".v";
	setAttr ".pu" 0.34375;
	setAttr ".pv" 0.21875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve79" -p "pSphere1Follicle3422";
	rename -uid "360B1274-44DD-C84D-1D7B-1BB2B2BC0833";
createNode nurbsCurve -n "curveShape79" -p "curve79";
	rename -uid "42F33CA1-4D5E-8E40-B28F-ABB412331E74";
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
	rename -uid "A91B747B-487C-F2FB-7599-21AA9621FE26";
createNode follicle -n "pSphere1FollicleShape3428" -p "pSphere1Follicle3428";
	rename -uid "30FA74FB-4839-A895-83E5-8AA660778CDD";
	setAttr -k off ".v";
	setAttr ".pu" 0.34375;
	setAttr ".pv" 0.28125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve80" -p "pSphere1Follicle3428";
	rename -uid "4A44297D-44C6-5F91-CD5B-4EAD1C3F5035";
createNode nurbsCurve -n "curveShape80" -p "curve80";
	rename -uid "C700BD5F-4739-F097-3608-B2B77F28677F";
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
	rename -uid "3425DB65-4E33-67ED-9F59-E080D0366FB6";
createNode follicle -n "pSphere1FollicleShape3434" -p "pSphere1Follicle3434";
	rename -uid "B3D92D84-4B04-6ACE-ADFC-2899C3F5353D";
	setAttr -k off ".v";
	setAttr ".pu" 0.34375;
	setAttr ".pv" 0.34375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve81" -p "pSphere1Follicle3434";
	rename -uid "711C63AF-49DC-8409-9F2B-D096A37899F4";
createNode nurbsCurve -n "curveShape81" -p "curve81";
	rename -uid "C18017EB-4B9B-6F05-6A17-249B5121D652";
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
	rename -uid "AB2A2492-4145-461B-8AE0-178AF553D14B";
createNode follicle -n "pSphere1FollicleShape3440" -p "pSphere1Follicle3440";
	rename -uid "81E76DF4-4EF0-5B5C-8883-11A838D451B6";
	setAttr -k off ".v";
	setAttr ".pu" 0.34375;
	setAttr ".pv" 0.40625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve82" -p "pSphere1Follicle3440";
	rename -uid "A03471CC-4A2D-3902-61C7-1DA0A9F6A75B";
createNode nurbsCurve -n "curveShape82" -p "curve82";
	rename -uid "3A30EA73-4C38-6A6E-A898-85B119E7E1E6";
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
	rename -uid "37784BD7-49C2-FAF8-6E57-A99AA46969F2";
createNode follicle -n "pSphere1FollicleShape3446" -p "pSphere1Follicle3446";
	rename -uid "A059AA12-4194-07A0-AE45-29B285F32DD6";
	setAttr -k off ".v";
	setAttr ".pu" 0.34375;
	setAttr ".pv" 0.46875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve83" -p "pSphere1Follicle3446";
	rename -uid "6ABFD7F0-47E4-8D46-32C1-9ABD385FDD8E";
createNode nurbsCurve -n "curveShape83" -p "curve83";
	rename -uid "2B9EFEC6-4F8B-CAAC-9149-159955D6BA02";
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
	rename -uid "F3166B6A-4AF4-5A08-4B2B-CDB8563D8313";
createNode follicle -n "pSphere1FollicleShape3453" -p "pSphere1Follicle3453";
	rename -uid "2941E192-40DD-45DA-76B9-199D5C778BC4";
	setAttr -k off ".v";
	setAttr ".pu" 0.34375;
	setAttr ".pv" 0.53125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve84" -p "pSphere1Follicle3453";
	rename -uid "A83210EE-4110-C74D-5E4C-E18D297A7CCB";
createNode nurbsCurve -n "curveShape84" -p "curve84";
	rename -uid "CF372A43-40A9-395F-7218-5DA452FF8492";
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
	rename -uid "1D344C27-4893-4C5C-9467-FD84EDB84503";
createNode follicle -n "pSphere1FollicleShape3459" -p "pSphere1Follicle3459";
	rename -uid "0DE793C4-4D17-B6AF-F400-9297C5D272C8";
	setAttr -k off ".v";
	setAttr ".pu" 0.34375;
	setAttr ".pv" 0.59375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve85" -p "pSphere1Follicle3459";
	rename -uid "9DF4FADF-46AC-51B2-8E7C-778A29E51D11";
createNode nurbsCurve -n "curveShape85" -p "curve85";
	rename -uid "68569C58-4028-0691-C4D8-9C98AB214787";
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
	rename -uid "C77B7D10-4420-9571-898D-1D9DED38B7A2";
createNode follicle -n "pSphere1FollicleShape3465" -p "pSphere1Follicle3465";
	rename -uid "85F5DC89-4327-36D4-8AA1-CBBE2DDD5A4C";
	setAttr -k off ".v";
	setAttr ".pu" 0.34375;
	setAttr ".pv" 0.65625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve86" -p "pSphere1Follicle3465";
	rename -uid "D17AE0F9-4BF9-2453-8610-31B7861DA436";
createNode nurbsCurve -n "curveShape86" -p "curve86";
	rename -uid "DC269571-42E2-CCD9-DC9C-298DEC859809";
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
	rename -uid "68EEBE11-4F2A-CE72-3C0B-59B9CC20D9D6";
createNode follicle -n "pSphere1FollicleShape3471" -p "pSphere1Follicle3471";
	rename -uid "8CCBB2E5-4CA9-A137-A5DC-498D25DA2097";
	setAttr -k off ".v";
	setAttr ".pu" 0.34375;
	setAttr ".pv" 0.71875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve87" -p "pSphere1Follicle3471";
	rename -uid "35556000-4400-539C-040A-98868BDB2094";
createNode nurbsCurve -n "curveShape87" -p "curve87";
	rename -uid "724DB18A-4CE9-57E4-76D8-F6B32DA8E905";
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
	rename -uid "0D27873A-4842-1798-4870-1D86B525400B";
createNode follicle -n "pSphere1FollicleShape3477" -p "pSphere1Follicle3477";
	rename -uid "8E9D19DA-420B-EBE5-19D4-2DAB2FADD47A";
	setAttr -k off ".v";
	setAttr ".pu" 0.34375;
	setAttr ".pv" 0.78125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve88" -p "pSphere1Follicle3477";
	rename -uid "39CA9B4C-4125-750D-31B0-138415B6C84F";
createNode nurbsCurve -n "curveShape88" -p "curve88";
	rename -uid "FA2DC2A8-484D-BEAB-EED8-8F87EBF2F683";
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
	rename -uid "3CC07A1E-4CCC-F3BC-849D-ECA71A0FFF78";
createNode follicle -n "pSphere1FollicleShape3484" -p "pSphere1Follicle3484";
	rename -uid "79E37FCC-4950-2CF0-CFE9-14AFF8B55BAD";
	setAttr -k off ".v";
	setAttr ".pu" 0.34375;
	setAttr ".pv" 0.84375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve89" -p "pSphere1Follicle3484";
	rename -uid "4B07DA2E-460E-BA32-8B1E-28BF2C38FCDE";
createNode nurbsCurve -n "curveShape89" -p "curve89";
	rename -uid "2925C2F3-4C08-9D0D-F851-7A9CAAD446D9";
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
	rename -uid "E5092E51-469C-5721-F3A0-4A9ABE53FFF2";
createNode follicle -n "pSphere1FollicleShape3490" -p "pSphere1Follicle3490";
	rename -uid "612F8378-46AE-1144-ED73-D7A00C91491B";
	setAttr -k off ".v";
	setAttr ".pu" 0.34375;
	setAttr ".pv" 0.90625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve90" -p "pSphere1Follicle3490";
	rename -uid "4A33CAAD-4ED7-26EE-0062-4BB95F423BE0";
createNode nurbsCurve -n "curveShape90" -p "curve90";
	rename -uid "F5EEB330-4A1F-7DA7-194A-D989C812B06A";
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
	rename -uid "DE5E55B3-4353-D740-3399-B296173E914B";
createNode follicle -n "pSphere1FollicleShape4009" -p "pSphere1Follicle4009";
	rename -uid "7DC0C612-4D63-ED91-B16C-DB9C55CB8D89";
	setAttr -k off ".v";
	setAttr ".pu" 0.40625;
	setAttr ".pv" 0.09375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve91" -p "pSphere1Follicle4009";
	rename -uid "8D69C11E-4024-E22C-1A22-3EA4D1A44457";
createNode nurbsCurve -n "curveShape91" -p "curve91";
	rename -uid "091CB7DE-4266-1D3C-5E5E-188BF1D49C17";
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
	rename -uid "FA5EB495-47CD-AED6-A548-22A6D583A8CF";
createNode follicle -n "pSphere1FollicleShape4015" -p "pSphere1Follicle4015";
	rename -uid "13EF648C-4263-3E21-067B-54A3BF9C6AAB";
	setAttr -k off ".v";
	setAttr ".pu" 0.40625;
	setAttr ".pv" 0.15625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve92" -p "pSphere1Follicle4015";
	rename -uid "DF178491-4371-2C14-5EF2-F19B10D9E8C0";
createNode nurbsCurve -n "curveShape92" -p "curve92";
	rename -uid "79EDC22C-456D-E5A5-7A7D-55B991911441";
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
	rename -uid "F3FEF185-4737-806E-8669-DB985568C7CE";
createNode follicle -n "pSphere1FollicleShape4022" -p "pSphere1Follicle4022";
	rename -uid "C14EEBA3-40D0-A4A8-E547-65BBDF5AD505";
	setAttr -k off ".v";
	setAttr ".pu" 0.40625;
	setAttr ".pv" 0.21875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve93" -p "pSphere1Follicle4022";
	rename -uid "1362B700-424D-E3FA-39D9-AC824589C822";
createNode nurbsCurve -n "curveShape93" -p "curve93";
	rename -uid "CE5E9AFA-4AE9-0C9E-EC5C-7696E9EF5994";
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
	rename -uid "37B3BB29-4754-0AD4-CA40-E1AE7B7756BB";
createNode follicle -n "pSphere1FollicleShape4028" -p "pSphere1Follicle4028";
	rename -uid "0A5F1DA1-4C11-93EC-249A-8CBB94157083";
	setAttr -k off ".v";
	setAttr ".pu" 0.40625;
	setAttr ".pv" 0.28125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve94" -p "pSphere1Follicle4028";
	rename -uid "6469AF99-44DC-AC81-BF81-9D8E0046963B";
createNode nurbsCurve -n "curveShape94" -p "curve94";
	rename -uid "7032CD1F-4D6E-3794-FDC3-FAAA816B43B9";
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
	rename -uid "14572C22-4F6C-3211-C19E-E59296D04B64";
createNode follicle -n "pSphere1FollicleShape4034" -p "pSphere1Follicle4034";
	rename -uid "BC007281-42C7-6398-9A67-B0B323331EA2";
	setAttr -k off ".v";
	setAttr ".pu" 0.40625;
	setAttr ".pv" 0.34375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve95" -p "pSphere1Follicle4034";
	rename -uid "6FE5FF5E-49C4-2F96-E842-D88FF1DD57D1";
createNode nurbsCurve -n "curveShape95" -p "curve95";
	rename -uid "FE0BE5F0-4CF1-8239-88D3-2B9C267ED9B6";
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
	rename -uid "9939CDE5-49B2-6242-D5AA-37AC3B2615E1";
createNode follicle -n "pSphere1FollicleShape4040" -p "pSphere1Follicle4040";
	rename -uid "0881BE7B-4205-9D22-7E67-C7BAD38B3EA6";
	setAttr -k off ".v";
	setAttr ".pu" 0.40625;
	setAttr ".pv" 0.40625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve96" -p "pSphere1Follicle4040";
	rename -uid "98AD2C9C-4B0F-A9CC-EFA3-73ACA71090AC";
createNode nurbsCurve -n "curveShape96" -p "curve96";
	rename -uid "E6BDBB41-4616-C5B5-1D47-EAB2608A0A43";
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
	rename -uid "BE92D9A3-4141-4CB1-362A-A78A8ACD9D3C";
createNode follicle -n "pSphere1FollicleShape4046" -p "pSphere1Follicle4046";
	rename -uid "37268EA6-4D74-EB3C-9F0D-B3B836FC5D4B";
	setAttr -k off ".v";
	setAttr ".pu" 0.40625;
	setAttr ".pv" 0.46875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve97" -p "pSphere1Follicle4046";
	rename -uid "C1191174-40E8-CA6E-193F-05820892A4F2";
createNode nurbsCurve -n "curveShape97" -p "curve97";
	rename -uid "6CC88BFF-436D-B386-C450-499F2EFB0191";
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
	rename -uid "39506EF6-42CC-1F19-324D-A1AA3B08A5B4";
createNode follicle -n "pSphere1FollicleShape4053" -p "pSphere1Follicle4053";
	rename -uid "830627F8-44D7-5205-4777-38A743440D14";
	setAttr -k off ".v";
	setAttr ".pu" 0.40625;
	setAttr ".pv" 0.53125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve98" -p "pSphere1Follicle4053";
	rename -uid "819C362D-4B45-3783-F92B-B29450A3F0FB";
createNode nurbsCurve -n "curveShape98" -p "curve98";
	rename -uid "CA13A99A-4887-867E-D1EA-2AB51E6F2BBA";
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
	rename -uid "69877431-4425-6AB3-B1FE-96B10A8801B7";
createNode follicle -n "pSphere1FollicleShape4059" -p "pSphere1Follicle4059";
	rename -uid "7BD0B6A4-424D-D00D-AACB-FD854B648D01";
	setAttr -k off ".v";
	setAttr ".pu" 0.40625;
	setAttr ".pv" 0.59375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve99" -p "pSphere1Follicle4059";
	rename -uid "F034587E-48C2-6DE2-F06F-EAB5CDEFE27E";
createNode nurbsCurve -n "curveShape99" -p "curve99";
	rename -uid "E504D322-4018-EB09-B0C4-33BEA3E1D66C";
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
	rename -uid "A738DBE2-4D16-332C-627D-6EBA35C40F5C";
createNode follicle -n "pSphere1FollicleShape4065" -p "pSphere1Follicle4065";
	rename -uid "B084E273-4B22-437F-69ED-7B8B18C542F8";
	setAttr -k off ".v";
	setAttr ".pu" 0.40625;
	setAttr ".pv" 0.65625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve100" -p "pSphere1Follicle4065";
	rename -uid "47105278-4D00-726B-6EB0-9CA40AA9BCDF";
createNode nurbsCurve -n "curveShape100" -p "curve100";
	rename -uid "E14503A8-4508-3178-F1D9-CDA5309F515A";
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
	rename -uid "197E7ED3-4354-8A9C-8322-4C9E57AF68C9";
createNode follicle -n "pSphere1FollicleShape4071" -p "pSphere1Follicle4071";
	rename -uid "42DFF5F7-47AB-9C43-D102-178246372102";
	setAttr -k off ".v";
	setAttr ".pu" 0.40625;
	setAttr ".pv" 0.71875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve101" -p "pSphere1Follicle4071";
	rename -uid "4B5A5B47-4E0A-5243-ABF6-69AAC327A9FA";
createNode nurbsCurve -n "curveShape101" -p "curve101";
	rename -uid "44174CF4-4156-9EFC-F3E3-6EA390FD12D2";
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
	rename -uid "39203EF0-46A6-B0C9-9A67-65923298CA80";
createNode follicle -n "pSphere1FollicleShape4077" -p "pSphere1Follicle4077";
	rename -uid "572116AD-40AA-38D8-9D60-42BDCD61F4F7";
	setAttr -k off ".v";
	setAttr ".pu" 0.40625;
	setAttr ".pv" 0.78125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve102" -p "pSphere1Follicle4077";
	rename -uid "2A290A56-4FF3-F29A-8ED5-94AC41A32518";
createNode nurbsCurve -n "curveShape102" -p "curve102";
	rename -uid "5C5D7CDE-4A79-2418-8856-4892AA269C9C";
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
	rename -uid "3954E373-4F89-A619-A43F-9894B815BCF6";
createNode follicle -n "pSphere1FollicleShape4084" -p "pSphere1Follicle4084";
	rename -uid "E59591C7-4124-29AC-BAEF-D180FCB73578";
	setAttr -k off ".v";
	setAttr ".pu" 0.40625;
	setAttr ".pv" 0.84375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve103" -p "pSphere1Follicle4084";
	rename -uid "91A801DC-4D0C-F13B-29B5-8798ADCD419B";
createNode nurbsCurve -n "curveShape103" -p "curve103";
	rename -uid "F9B7C7F6-48C5-A3C8-C2A4-E0976469E804";
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
	rename -uid "D2355BFB-44FB-80E8-D4B1-A08019F56672";
createNode follicle -n "pSphere1FollicleShape4090" -p "pSphere1Follicle4090";
	rename -uid "4AAFE09A-4CCB-880D-51F5-D186CEB64290";
	setAttr -k off ".v";
	setAttr ".pu" 0.40625;
	setAttr ".pv" 0.90625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve104" -p "pSphere1Follicle4090";
	rename -uid "3171B37F-4F31-F655-6658-2E8C9726DE4D";
createNode nurbsCurve -n "curveShape104" -p "curve104";
	rename -uid "D8FB5845-4C81-2199-A442-D497BA2DA1F7";
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
	rename -uid "CF6A214D-4F48-6ED5-A35B-F28140EF434A";
createNode follicle -n "pSphere1FollicleShape4603" -p "pSphere1Follicle4603";
	rename -uid "6B652413-4B4D-0300-3B31-D8917BED6E2F";
	setAttr -k off ".v";
	setAttr ".pu" 0.46875;
	setAttr ".pv" 0.03125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve105" -p "pSphere1Follicle4603";
	rename -uid "0CBC53F2-4BE7-042C-C9B1-2EA6B98345A4";
createNode nurbsCurve -n "curveShape105" -p "curve105";
	rename -uid "991D6242-4B47-CEE4-5FDF-23929BDE594E";
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
	rename -uid "8D4AF55A-4978-953C-536B-B6B07E370185";
createNode follicle -n "pSphere1FollicleShape4609" -p "pSphere1Follicle4609";
	rename -uid "D8348006-4130-C84F-2ACA-AE880FFCA4F1";
	setAttr -k off ".v";
	setAttr ".pu" 0.46875;
	setAttr ".pv" 0.09375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve106" -p "pSphere1Follicle4609";
	rename -uid "0DC80C19-4390-426C-FBA3-429C4F1D4027";
createNode nurbsCurve -n "curveShape106" -p "curve106";
	rename -uid "3F97BCCA-4565-1E56-05B2-D68B8CD51870";
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
	rename -uid "208CF97C-4242-5C4F-452D-37A59E530FCA";
createNode follicle -n "pSphere1FollicleShape4615" -p "pSphere1Follicle4615";
	rename -uid "FB65F2CA-4700-3E37-21CE-988C80AB1EB6";
	setAttr -k off ".v";
	setAttr ".pu" 0.46875;
	setAttr ".pv" 0.15625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve107" -p "pSphere1Follicle4615";
	rename -uid "C3871DF0-4B11-8232-36CD-54AC0A0F14A5";
createNode nurbsCurve -n "curveShape107" -p "curve107";
	rename -uid "DF23A822-430F-1B38-8810-43808A2A0D5D";
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
	rename -uid "6951FD22-4D43-D4B3-DF3B-8FB54DEF8001";
createNode follicle -n "pSphere1FollicleShape4622" -p "pSphere1Follicle4622";
	rename -uid "7A2F2CDA-4F78-9B74-591E-2DBCADA4FD0B";
	setAttr -k off ".v";
	setAttr ".pu" 0.46875;
	setAttr ".pv" 0.21875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve108" -p "pSphere1Follicle4622";
	rename -uid "792F40E3-48E0-5C11-90EC-E88C99995D8A";
createNode nurbsCurve -n "curveShape108" -p "curve108";
	rename -uid "F44A155D-462D-F609-594F-1CA16A857278";
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
	rename -uid "E05438EF-4DBB-3D40-5209-CE82A7A99E54";
createNode follicle -n "pSphere1FollicleShape4628" -p "pSphere1Follicle4628";
	rename -uid "9C7223D6-4920-A29F-4B38-D6848CB78BD6";
	setAttr -k off ".v";
	setAttr ".pu" 0.46875;
	setAttr ".pv" 0.28125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve109" -p "pSphere1Follicle4628";
	rename -uid "8254713B-459A-066A-F6DB-848F22051FEB";
createNode nurbsCurve -n "curveShape109" -p "curve109";
	rename -uid "1D2D40B1-4F11-6D60-D6F6-17B4E04FC4DE";
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
	rename -uid "EC785924-43F0-FFC2-D303-5E9DC5FD125B";
createNode follicle -n "pSphere1FollicleShape4634" -p "pSphere1Follicle4634";
	rename -uid "1B56CAF3-430B-C068-B156-3FAA61DE11BF";
	setAttr -k off ".v";
	setAttr ".pu" 0.46875;
	setAttr ".pv" 0.34375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve110" -p "pSphere1Follicle4634";
	rename -uid "A8754E57-41FE-F257-FF3E-648313953825";
createNode nurbsCurve -n "curveShape110" -p "curve110";
	rename -uid "BA9799A1-4F62-1C2A-3E72-BC89D5898BFC";
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
	rename -uid "40C210DA-4DAA-667B-0DE6-01AAD2AD1DFB";
createNode follicle -n "pSphere1FollicleShape4640" -p "pSphere1Follicle4640";
	rename -uid "205C74AE-4B0F-FA61-33AE-BC9662598432";
	setAttr -k off ".v";
	setAttr ".pu" 0.46875;
	setAttr ".pv" 0.40625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve111" -p "pSphere1Follicle4640";
	rename -uid "CE2B0A25-49B6-D1E0-5EA7-0BA3D935CDE9";
createNode nurbsCurve -n "curveShape111" -p "curve111";
	rename -uid "5BBD24A8-4B35-2F95-F58E-BCADDDF25991";
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
	rename -uid "B9979976-430C-3A04-9FD7-B99319646FD0";
createNode follicle -n "pSphere1FollicleShape4646" -p "pSphere1Follicle4646";
	rename -uid "2E47B908-41C0-5643-E495-6F913165A6AF";
	setAttr -k off ".v";
	setAttr ".pu" 0.46875;
	setAttr ".pv" 0.46875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve112" -p "pSphere1Follicle4646";
	rename -uid "35029ED3-4635-E1EE-C9F7-E3BB3825D10F";
createNode nurbsCurve -n "curveShape112" -p "curve112";
	rename -uid "FF46A861-4C14-8459-AFB2-34B6C2CC1B29";
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
	rename -uid "964766A5-47C9-4994-E14F-10A0C3748525";
createNode follicle -n "pSphere1FollicleShape4653" -p "pSphere1Follicle4653";
	rename -uid "83343CE8-4B47-1082-A2EA-0894843A69AC";
	setAttr -k off ".v";
	setAttr ".pu" 0.46875;
	setAttr ".pv" 0.53125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve113" -p "pSphere1Follicle4653";
	rename -uid "AD446837-48F3-DFB3-A3E4-42BEA57A37F7";
createNode nurbsCurve -n "curveShape113" -p "curve113";
	rename -uid "F557FB34-40D2-8555-0EAB-07B8CA7D4399";
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
	rename -uid "C538BF2E-4748-D457-FA39-EE8FAF8711B7";
createNode follicle -n "pSphere1FollicleShape4659" -p "pSphere1Follicle4659";
	rename -uid "F43AA4E1-43FA-7DD0-E25E-43933FFF9F45";
	setAttr -k off ".v";
	setAttr ".pu" 0.46875;
	setAttr ".pv" 0.59375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve114" -p "pSphere1Follicle4659";
	rename -uid "EFF104F2-4FA3-FBC6-CDB7-ED941F676D86";
createNode nurbsCurve -n "curveShape114" -p "curve114";
	rename -uid "811F99F3-4ACF-485A-2F44-3C82FC872B79";
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
	rename -uid "88304A61-46CB-DF2C-93E8-0D94B00A49C0";
createNode follicle -n "pSphere1FollicleShape4665" -p "pSphere1Follicle4665";
	rename -uid "270FDFB0-4997-4C33-AE55-FC882E849931";
	setAttr -k off ".v";
	setAttr ".pu" 0.46875;
	setAttr ".pv" 0.65625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve115" -p "pSphere1Follicle4665";
	rename -uid "3E57ADA0-4C50-FC07-55A6-D380AFB45327";
createNode nurbsCurve -n "curveShape115" -p "curve115";
	rename -uid "4B6F4088-42B5-DA3C-C35C-53A18F5AE2D7";
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
	rename -uid "48E5F149-4DCC-7775-725A-64A9C6302E2B";
createNode follicle -n "pSphere1FollicleShape4671" -p "pSphere1Follicle4671";
	rename -uid "C75872D0-4E30-DCA5-D71C-EBA8BF0BCA2E";
	setAttr -k off ".v";
	setAttr ".pu" 0.46875;
	setAttr ".pv" 0.71875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve116" -p "pSphere1Follicle4671";
	rename -uid "7F4ED057-4544-DAD1-1B9C-518A834344F8";
createNode nurbsCurve -n "curveShape116" -p "curve116";
	rename -uid "91EBF4BF-4249-1D98-F54F-ACA72B1EAAAF";
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
	rename -uid "DC0272DC-498A-8AD1-43AC-63A064131FA9";
createNode follicle -n "pSphere1FollicleShape4677" -p "pSphere1Follicle4677";
	rename -uid "D594A477-49B0-C2BA-D987-35BA25FA4C11";
	setAttr -k off ".v";
	setAttr ".pu" 0.46875;
	setAttr ".pv" 0.78125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve117" -p "pSphere1Follicle4677";
	rename -uid "EEE5F5B8-47B7-292F-A4C3-10AEFD68495C";
createNode nurbsCurve -n "curveShape117" -p "curve117";
	rename -uid "A7401F67-4E80-7740-1C27-3296408AE28E";
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
	rename -uid "0D089CEA-4D37-D43A-9012-AD9C01556C78";
createNode follicle -n "pSphere1FollicleShape4684" -p "pSphere1Follicle4684";
	rename -uid "7F212752-4DCF-E82A-8FCE-6E879EE5B870";
	setAttr -k off ".v";
	setAttr ".pu" 0.46875;
	setAttr ".pv" 0.84375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve118" -p "pSphere1Follicle4684";
	rename -uid "FC5083D8-484C-7ED1-8343-37A4539AC7E4";
createNode nurbsCurve -n "curveShape118" -p "curve118";
	rename -uid "23433995-4944-FE35-0832-F3A650522920";
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
	rename -uid "1F97999F-464F-57EC-66C1-7DA55A61CA7A";
createNode follicle -n "pSphere1FollicleShape4690" -p "pSphere1Follicle4690";
	rename -uid "CD4E67BB-40C0-AFF4-C6EF-C986FD5B0927";
	setAttr -k off ".v";
	setAttr ".pu" 0.46875;
	setAttr ".pv" 0.90625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve119" -p "pSphere1Follicle4690";
	rename -uid "A2FD0CA2-49E6-01D7-B491-AA8A49F99638";
createNode nurbsCurve -n "curveShape119" -p "curve119";
	rename -uid "63602910-429B-4EA3-AAF1-698C77B47C79";
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
	rename -uid "ED41F9A5-4EBA-DD37-AABC-F2BB42B48C1F";
createNode follicle -n "pSphere1FollicleShape4696" -p "pSphere1Follicle4696";
	rename -uid "9A41EF5F-4B15-1E04-EA42-18BF01D8B439";
	setAttr -k off ".v";
	setAttr ".pu" 0.46875;
	setAttr ".pv" 0.96875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve120" -p "pSphere1Follicle4696";
	rename -uid "DC127E94-41A4-A48E-B2F9-5BBE89F6E443";
createNode nurbsCurve -n "curveShape120" -p "curve120";
	rename -uid "DFB8557A-4800-C2B3-4B7E-A4A999D51139";
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
	rename -uid "C29ECE68-4837-4CCB-D050-6FB906135E76";
createNode follicle -n "pSphere1FollicleShape5303" -p "pSphere1Follicle5303";
	rename -uid "E590BC71-4A62-7D7E-DD6D-D59BAC238E34";
	setAttr -k off ".v";
	setAttr ".pu" 0.53125;
	setAttr ".pv" 0.03125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve121" -p "pSphere1Follicle5303";
	rename -uid "1466B279-4131-5321-ED9C-8A933CF8B0CE";
createNode nurbsCurve -n "curveShape121" -p "curve121";
	rename -uid "FFA7A127-45D7-D387-CCFA-BF8B3B003810";
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
	rename -uid "AB9190F6-4AA3-7EDF-34FB-49AC53825D1C";
createNode follicle -n "pSphere1FollicleShape5309" -p "pSphere1Follicle5309";
	rename -uid "B120BBF0-4185-DA8A-59B7-DE818ABC26DD";
	setAttr -k off ".v";
	setAttr ".pu" 0.53125;
	setAttr ".pv" 0.09375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve122" -p "pSphere1Follicle5309";
	rename -uid "C2070663-48BB-621C-50AC-848EB71EA146";
createNode nurbsCurve -n "curveShape122" -p "curve122";
	rename -uid "F70A4CCD-469F-A3AF-E9B6-7A9869E1882E";
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
	rename -uid "041F2C77-414D-1A80-F9BF-4DAF5ABE3350";
createNode follicle -n "pSphere1FollicleShape5315" -p "pSphere1Follicle5315";
	rename -uid "9715BA2E-48B3-E1EA-F874-64B7BB6AD6FF";
	setAttr -k off ".v";
	setAttr ".pu" 0.53125;
	setAttr ".pv" 0.15625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve123" -p "pSphere1Follicle5315";
	rename -uid "B2EAC6F7-41D9-49ED-7D2B-B0ADFED6A334";
createNode nurbsCurve -n "curveShape123" -p "curve123";
	rename -uid "9ABE0325-45E2-4FAB-D040-2A90B8B68F46";
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
	rename -uid "45DA026E-4699-4282-120F-EF8BC385DAD0";
createNode follicle -n "pSphere1FollicleShape5322" -p "pSphere1Follicle5322";
	rename -uid "99B1206F-48C1-4E57-0011-B1885E9B185D";
	setAttr -k off ".v";
	setAttr ".pu" 0.53125;
	setAttr ".pv" 0.21875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve124" -p "pSphere1Follicle5322";
	rename -uid "87977556-498B-1D56-D896-F7888CADBB40";
createNode nurbsCurve -n "curveShape124" -p "curve124";
	rename -uid "019F2027-4C92-E69B-D0C3-52910DA04893";
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
	rename -uid "92FF5598-4A24-032D-73E0-D6A262941F07";
createNode follicle -n "pSphere1FollicleShape5328" -p "pSphere1Follicle5328";
	rename -uid "3690384F-41DD-E667-8822-9FA665AE2323";
	setAttr -k off ".v";
	setAttr ".pu" 0.53125;
	setAttr ".pv" 0.28125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve125" -p "pSphere1Follicle5328";
	rename -uid "C762F98D-4655-3DEE-07E9-1A86E4AA66EF";
createNode nurbsCurve -n "curveShape125" -p "curve125";
	rename -uid "71F11018-4A10-66BB-95B5-B69C57D26BCF";
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
	rename -uid "4CCD1C0C-4CE2-30C1-CA63-5188CBA108EE";
createNode follicle -n "pSphere1FollicleShape5334" -p "pSphere1Follicle5334";
	rename -uid "45B7EF24-4E7E-4B58-E4E7-D393DD7110D4";
	setAttr -k off ".v";
	setAttr ".pu" 0.53125;
	setAttr ".pv" 0.34375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve126" -p "pSphere1Follicle5334";
	rename -uid "91594674-4D06-16B3-F4A7-56A7EEC5AA36";
createNode nurbsCurve -n "curveShape126" -p "curve126";
	rename -uid "9C477318-4161-1952-B094-E79014031089";
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
	rename -uid "3066FD34-4794-814F-9136-13895E33821F";
createNode follicle -n "pSphere1FollicleShape5340" -p "pSphere1Follicle5340";
	rename -uid "E53B1A17-4F8C-9804-C089-19A6CE41A4C3";
	setAttr -k off ".v";
	setAttr ".pu" 0.53125;
	setAttr ".pv" 0.40625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve127" -p "pSphere1Follicle5340";
	rename -uid "B3E5F441-4088-EFF0-5A12-E891BA88E6A8";
createNode nurbsCurve -n "curveShape127" -p "curve127";
	rename -uid "140EBDBA-49E0-0824-3BB7-57A9B2A70FC4";
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
	rename -uid "041367D6-4CA2-D7D3-C25D-7A92F07D8FD8";
createNode follicle -n "pSphere1FollicleShape5346" -p "pSphere1Follicle5346";
	rename -uid "BB3AEF8F-41B5-C841-1C6D-57B1EBBEC1B4";
	setAttr -k off ".v";
	setAttr ".pu" 0.53125;
	setAttr ".pv" 0.46875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve128" -p "pSphere1Follicle5346";
	rename -uid "1DAC33EB-482E-3840-8521-50A0440FCFD6";
createNode nurbsCurve -n "curveShape128" -p "curve128";
	rename -uid "EA8CBC95-4BFA-EFE4-30F1-EF9CCD73E9BE";
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
	rename -uid "0B5FA137-42B1-8321-53AB-DE8A3AD316AB";
createNode follicle -n "pSphere1FollicleShape5353" -p "pSphere1Follicle5353";
	rename -uid "DF5B44CC-4E58-4A66-B971-4386EBDEEAB5";
	setAttr -k off ".v";
	setAttr ".pu" 0.53125;
	setAttr ".pv" 0.53125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve129" -p "pSphere1Follicle5353";
	rename -uid "96FCD926-4910-457F-6DF5-8E82CABCB760";
createNode nurbsCurve -n "curveShape129" -p "curve129";
	rename -uid "9B9910C7-4AEE-7D13-5AEE-FBA718A3134B";
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
	rename -uid "57B303C8-4645-8392-9FBE-F4B3947A6C39";
createNode follicle -n "pSphere1FollicleShape5359" -p "pSphere1Follicle5359";
	rename -uid "93843705-4F2F-6429-4B01-98B9703A1AC6";
	setAttr -k off ".v";
	setAttr ".pu" 0.53125;
	setAttr ".pv" 0.59375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve130" -p "pSphere1Follicle5359";
	rename -uid "21CADB64-4D25-BE85-B1A8-CA82878934BA";
createNode nurbsCurve -n "curveShape130" -p "curve130";
	rename -uid "6A4131E6-46B1-DF80-4B00-FE8901773D42";
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
	rename -uid "4C7102FA-44D9-8DB6-1301-F1B71D56566E";
createNode follicle -n "pSphere1FollicleShape5365" -p "pSphere1Follicle5365";
	rename -uid "DFCBE978-4B39-49F2-8A87-888819CE9AD8";
	setAttr -k off ".v";
	setAttr ".pu" 0.53125;
	setAttr ".pv" 0.65625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve131" -p "pSphere1Follicle5365";
	rename -uid "95D7A763-49CF-DB77-438F-D7BF2AA0BD14";
createNode nurbsCurve -n "curveShape131" -p "curve131";
	rename -uid "E03BF3CC-43D0-D4C5-B68E-E9960177900A";
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
	rename -uid "BEE85EC9-4F3F-BE6B-73E8-918119CCCD0C";
createNode follicle -n "pSphere1FollicleShape5371" -p "pSphere1Follicle5371";
	rename -uid "C676D1C3-4957-E3E8-2F0C-23999BED40ED";
	setAttr -k off ".v";
	setAttr ".pu" 0.53125;
	setAttr ".pv" 0.71875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve132" -p "pSphere1Follicle5371";
	rename -uid "5DFD743D-4A6D-DFEB-2F9A-FA81B98B7E74";
createNode nurbsCurve -n "curveShape132" -p "curve132";
	rename -uid "A96A05C3-42CA-A9E6-D9E3-6D803A35285D";
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
	rename -uid "C5FD36E4-45AE-BD2D-2CA6-9EB998B377BC";
createNode follicle -n "pSphere1FollicleShape5377" -p "pSphere1Follicle5377";
	rename -uid "BFF92445-4274-EBE5-1841-478A14D2D157";
	setAttr -k off ".v";
	setAttr ".pu" 0.53125;
	setAttr ".pv" 0.78125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve133" -p "pSphere1Follicle5377";
	rename -uid "1A103C4A-42E0-E46D-B2F3-3790C4964EDC";
createNode nurbsCurve -n "curveShape133" -p "curve133";
	rename -uid "1623B658-4733-7F54-2C3D-B68B9F82701D";
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
	rename -uid "7DD63E7B-4DD5-CDD8-DB75-FB8735342025";
createNode follicle -n "pSphere1FollicleShape5384" -p "pSphere1Follicle5384";
	rename -uid "8EE50FEC-40AF-F739-C8C5-F1BB132A897E";
	setAttr -k off ".v";
	setAttr ".pu" 0.53125;
	setAttr ".pv" 0.84375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve134" -p "pSphere1Follicle5384";
	rename -uid "CBFAE96A-4A1D-96A2-F278-5E979D0893F7";
createNode nurbsCurve -n "curveShape134" -p "curve134";
	rename -uid "10F67B8D-46D2-5327-D3DC-CE831F72A8C6";
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
	rename -uid "B559B749-43DF-DFF8-8443-3AA6FF7C8833";
createNode follicle -n "pSphere1FollicleShape5390" -p "pSphere1Follicle5390";
	rename -uid "9E4070BC-407A-3E5A-B920-DF887A9F1371";
	setAttr -k off ".v";
	setAttr ".pu" 0.53125;
	setAttr ".pv" 0.90625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve135" -p "pSphere1Follicle5390";
	rename -uid "DDC64A0B-474E-D0AB-7355-41AF851036A9";
createNode nurbsCurve -n "curveShape135" -p "curve135";
	rename -uid "6597313E-4EC5-ABD3-5D34-5D939802DB87";
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
	rename -uid "C2375143-4D21-729C-275F-EB9EF8426D62";
createNode follicle -n "pSphere1FollicleShape5396" -p "pSphere1Follicle5396";
	rename -uid "C7979CBC-47A4-7A46-DDC5-4E88AF9413DC";
	setAttr -k off ".v";
	setAttr ".pu" 0.53125;
	setAttr ".pv" 0.96875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve136" -p "pSphere1Follicle5396";
	rename -uid "C41C03DB-44CD-F79B-D45B-FCBC078A67AA";
createNode nurbsCurve -n "curveShape136" -p "curve136";
	rename -uid "B7CD4B43-4263-96C2-C6B8-D2A852DB6EB5";
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
	rename -uid "78115A4F-43D9-875D-8BEA-EAB4AACAAAD2";
createNode follicle -n "pSphere1FollicleShape5909" -p "pSphere1Follicle5909";
	rename -uid "0E88437C-41F9-B3C3-75A3-62B242159899";
	setAttr -k off ".v";
	setAttr ".pu" 0.59375;
	setAttr ".pv" 0.09375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve137" -p "pSphere1Follicle5909";
	rename -uid "89199A6F-4179-A4D5-86FF-B984EC0CD351";
createNode nurbsCurve -n "curveShape137" -p "curve137";
	rename -uid "3F48B51C-4C88-582E-B5DE-6D8C86C3E4D3";
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
	rename -uid "B14663A8-43FA-CDB0-687A-C1AC19F75066";
createNode follicle -n "pSphere1FollicleShape5915" -p "pSphere1Follicle5915";
	rename -uid "2197BCAA-44CD-C477-F650-46B2035D3D1C";
	setAttr -k off ".v";
	setAttr ".pu" 0.59375;
	setAttr ".pv" 0.15625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve138" -p "pSphere1Follicle5915";
	rename -uid "C40A068F-45B6-75DC-7F7A-DB97976FFF26";
createNode nurbsCurve -n "curveShape138" -p "curve138";
	rename -uid "B0398EC1-4168-D185-AB9E-E9BD82F5977A";
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
	rename -uid "6777FA0F-4CFC-6DF8-318B-A6871A8A116B";
createNode follicle -n "pSphere1FollicleShape5922" -p "pSphere1Follicle5922";
	rename -uid "2C909AEE-47CD-76F8-C81A-AB822E35B3F4";
	setAttr -k off ".v";
	setAttr ".pu" 0.59375;
	setAttr ".pv" 0.21875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve139" -p "pSphere1Follicle5922";
	rename -uid "97656A61-4077-C15C-34A9-0D8C708DDBD7";
createNode nurbsCurve -n "curveShape139" -p "curve139";
	rename -uid "832E4946-4C6E-4BF8-E5D7-4FB16AE12696";
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
	rename -uid "1CCBB372-4E58-2E89-49D2-949F87A64A30";
createNode follicle -n "pSphere1FollicleShape5928" -p "pSphere1Follicle5928";
	rename -uid "988F317D-4DF6-902F-A6A9-8EB23541F9AC";
	setAttr -k off ".v";
	setAttr ".pu" 0.59375;
	setAttr ".pv" 0.28125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve140" -p "pSphere1Follicle5928";
	rename -uid "366096EB-4C0E-728E-9138-33B45263398D";
createNode nurbsCurve -n "curveShape140" -p "curve140";
	rename -uid "B5647124-4EA7-8109-CE81-A6A62E33A6D7";
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
	rename -uid "D6DCDBEA-48D3-2E4A-9F66-AF9CE2341985";
createNode follicle -n "pSphere1FollicleShape5934" -p "pSphere1Follicle5934";
	rename -uid "57AA7774-4D85-A799-53D8-F8BEEF092EF6";
	setAttr -k off ".v";
	setAttr ".pu" 0.59375;
	setAttr ".pv" 0.34375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve141" -p "pSphere1Follicle5934";
	rename -uid "E05CF6B0-4680-4A8E-807A-DFBDC78B08FE";
createNode nurbsCurve -n "curveShape141" -p "curve141";
	rename -uid "CDAF894B-43BB-2F2B-7167-8CBE8F7B9BBD";
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
	rename -uid "3BD295BC-4670-63DA-C2F8-EE83D22ECEA5";
createNode follicle -n "pSphere1FollicleShape5940" -p "pSphere1Follicle5940";
	rename -uid "405465AB-4B49-134D-3ACD-248CA299CAA5";
	setAttr -k off ".v";
	setAttr ".pu" 0.59375;
	setAttr ".pv" 0.40625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve142" -p "pSphere1Follicle5940";
	rename -uid "C7433A82-4C7C-460E-DA0B-5DB7268549B7";
createNode nurbsCurve -n "curveShape142" -p "curve142";
	rename -uid "C5403F00-4A3B-D712-DA09-3FBE5FA1E454";
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
	rename -uid "668BDBB0-4D04-8AFC-0A2C-96A200384357";
createNode follicle -n "pSphere1FollicleShape5946" -p "pSphere1Follicle5946";
	rename -uid "F7FBEA30-4CD8-C920-2A25-8B80FB4279BC";
	setAttr -k off ".v";
	setAttr ".pu" 0.59375;
	setAttr ".pv" 0.46875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve143" -p "pSphere1Follicle5946";
	rename -uid "0AFD9AA5-451E-CC55-EBAF-4EA99BA5572E";
createNode nurbsCurve -n "curveShape143" -p "curve143";
	rename -uid "BBBA9FF1-47DF-6BE1-369F-558023A4C98E";
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
	rename -uid "34E50336-42F3-B61A-C68C-DA9E8A5593B7";
createNode follicle -n "pSphere1FollicleShape5953" -p "pSphere1Follicle5953";
	rename -uid "40B43A43-47F8-44DB-463F-C2B7C587E53A";
	setAttr -k off ".v";
	setAttr ".pu" 0.59375;
	setAttr ".pv" 0.53125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve144" -p "pSphere1Follicle5953";
	rename -uid "36E6F067-461D-C045-7672-2197E274D3E2";
createNode nurbsCurve -n "curveShape144" -p "curve144";
	rename -uid "EE7BFA0C-49DD-8B92-F984-EDAA641923C1";
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
	rename -uid "581E7A34-4000-8D56-4389-179B7E65F1FB";
createNode follicle -n "pSphere1FollicleShape5959" -p "pSphere1Follicle5959";
	rename -uid "81CA74AB-45D3-2EDA-5079-2CB3F476D3B6";
	setAttr -k off ".v";
	setAttr ".pu" 0.59375;
	setAttr ".pv" 0.59375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve145" -p "pSphere1Follicle5959";
	rename -uid "97D8D2C5-4BC7-4866-327C-FBA98C46F85B";
createNode nurbsCurve -n "curveShape145" -p "curve145";
	rename -uid "F4C79139-4CD5-E5C6-8C4D-519225DCDC05";
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
	rename -uid "399C9E7E-4431-69D2-476A-89B5EAB9C4D6";
createNode follicle -n "pSphere1FollicleShape5965" -p "pSphere1Follicle5965";
	rename -uid "262B5470-4F03-CBB0-0AEE-D59BA72AAF58";
	setAttr -k off ".v";
	setAttr ".pu" 0.59375;
	setAttr ".pv" 0.65625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve146" -p "pSphere1Follicle5965";
	rename -uid "0B7E797B-44A1-E724-0015-37A0FEBDAC8F";
createNode nurbsCurve -n "curveShape146" -p "curve146";
	rename -uid "764A8E59-4C56-8A33-B851-B49EC48D5AD5";
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
	rename -uid "04E1F00D-410A-8BE2-5D6C-B9BE6C9F25EC";
createNode follicle -n "pSphere1FollicleShape5971" -p "pSphere1Follicle5971";
	rename -uid "6991BBBC-4D95-F8B5-3CF9-B08019DE8A85";
	setAttr -k off ".v";
	setAttr ".pu" 0.59375;
	setAttr ".pv" 0.71875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve147" -p "pSphere1Follicle5971";
	rename -uid "B235A665-4FDF-FF16-C235-E387784828D6";
createNode nurbsCurve -n "curveShape147" -p "curve147";
	rename -uid "6F9AB7A1-4148-14A0-F3AD-56AE343F45EB";
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
	rename -uid "DE79BCDB-4169-F634-C3B1-99BEA30A43F1";
createNode follicle -n "pSphere1FollicleShape5977" -p "pSphere1Follicle5977";
	rename -uid "C9FB0940-4D38-1007-7701-6C802660B18C";
	setAttr -k off ".v";
	setAttr ".pu" 0.59375;
	setAttr ".pv" 0.78125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve148" -p "pSphere1Follicle5977";
	rename -uid "4F62582F-44A2-6245-F129-84AE1910C4DF";
createNode nurbsCurve -n "curveShape148" -p "curve148";
	rename -uid "BE332FA1-4EC1-57F4-B7BC-BA8284F0CEDA";
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
	rename -uid "1C714DCF-4D4E-B0DF-CB7F-7A8A9D82BB32";
createNode follicle -n "pSphere1FollicleShape5984" -p "pSphere1Follicle5984";
	rename -uid "EB705B24-442B-8052-7AE9-19A472FEB0B6";
	setAttr -k off ".v";
	setAttr ".pu" 0.59375;
	setAttr ".pv" 0.84375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve149" -p "pSphere1Follicle5984";
	rename -uid "C2136764-4A80-B962-634C-41A48D7E31C8";
createNode nurbsCurve -n "curveShape149" -p "curve149";
	rename -uid "5B4BE0BB-4C52-4CF1-9717-67886BC3C04B";
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
	rename -uid "E29152FC-42FC-24A4-A116-4C98DB8E6E47";
createNode follicle -n "pSphere1FollicleShape5990" -p "pSphere1Follicle5990";
	rename -uid "173AA4EF-49D2-48D4-67CA-3F9E32241645";
	setAttr -k off ".v";
	setAttr ".pu" 0.59375;
	setAttr ".pv" 0.90625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve150" -p "pSphere1Follicle5990";
	rename -uid "0FD5CB82-447B-A08A-F5D7-0482746D7D8F";
createNode nurbsCurve -n "curveShape150" -p "curve150";
	rename -uid "5BB3F9AC-46EC-93C0-B97C-57A8DBB3B1B1";
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
	rename -uid "293A5189-48A0-C1B1-7537-2080DAD18367";
createNode follicle -n "pSphere1FollicleShape6509" -p "pSphere1Follicle6509";
	rename -uid "1C31C4DD-4E7D-2212-C010-F69BAA28A20D";
	setAttr -k off ".v";
	setAttr ".pu" 0.65625;
	setAttr ".pv" 0.09375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve151" -p "pSphere1Follicle6509";
	rename -uid "4BE15838-4421-4C81-DA82-098879818F9F";
createNode nurbsCurve -n "curveShape151" -p "curve151";
	rename -uid "885AAB43-499B-1039-5ECA-2DA8AAFAD4BD";
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
	rename -uid "17A94380-41BE-316D-C6E5-B8B823F6EF9A";
createNode follicle -n "pSphere1FollicleShape6515" -p "pSphere1Follicle6515";
	rename -uid "1940C0A6-4B27-B70E-D2BD-7DA3A3906E6D";
	setAttr -k off ".v";
	setAttr ".pu" 0.65625;
	setAttr ".pv" 0.15625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve152" -p "pSphere1Follicle6515";
	rename -uid "27E13EB1-4B26-FA7E-0511-2395A7D4480A";
createNode nurbsCurve -n "curveShape152" -p "curve152";
	rename -uid "8712EDA3-4F89-D046-E32E-F2A3E0856020";
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
	rename -uid "7F23EF80-4925-9549-1C52-288AA02435B3";
createNode follicle -n "pSphere1FollicleShape6522" -p "pSphere1Follicle6522";
	rename -uid "68E82BB0-413E-A2CC-7E90-C3946C06B3DB";
	setAttr -k off ".v";
	setAttr ".pu" 0.65625;
	setAttr ".pv" 0.21875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve153" -p "pSphere1Follicle6522";
	rename -uid "D97F1600-45A8-F602-5E56-1A83BE8252B0";
createNode nurbsCurve -n "curveShape153" -p "curve153";
	rename -uid "7CC72852-4FE2-A938-BA22-708A7C18F2D2";
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
	rename -uid "6E4D2B0B-4A76-BFE3-39BF-F7A506D8B3EC";
createNode follicle -n "pSphere1FollicleShape6528" -p "pSphere1Follicle6528";
	rename -uid "170A5136-4BFA-17F3-A41A-89923B87BF2B";
	setAttr -k off ".v";
	setAttr ".pu" 0.65625;
	setAttr ".pv" 0.28125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve154" -p "pSphere1Follicle6528";
	rename -uid "B7B6F9FD-433B-6EB6-7921-7287ED673647";
createNode nurbsCurve -n "curveShape154" -p "curve154";
	rename -uid "883B075F-48EC-3CAB-A841-FFB90070F855";
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
	rename -uid "9E939549-4E1E-A080-579B-5F9EB0DA11C1";
createNode follicle -n "pSphere1FollicleShape6534" -p "pSphere1Follicle6534";
	rename -uid "86274043-4E73-6C28-3D6B-1BB472BD8D85";
	setAttr -k off ".v";
	setAttr ".pu" 0.65625;
	setAttr ".pv" 0.34375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve155" -p "pSphere1Follicle6534";
	rename -uid "2A988C91-4A11-2F49-3497-42A8EFA4FB3B";
createNode nurbsCurve -n "curveShape155" -p "curve155";
	rename -uid "1D48FE73-4C9D-D1BC-78F0-ABAE08B00D78";
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
	rename -uid "D29E63A6-41AE-8B7A-D834-B3A752D326DB";
createNode follicle -n "pSphere1FollicleShape6540" -p "pSphere1Follicle6540";
	rename -uid "BB469FCF-4D1B-2212-CED2-CC9092249DB0";
	setAttr -k off ".v";
	setAttr ".pu" 0.65625;
	setAttr ".pv" 0.40625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve156" -p "pSphere1Follicle6540";
	rename -uid "FE6B8EF6-4D5F-634F-A5AA-2A87338FD18D";
createNode nurbsCurve -n "curveShape156" -p "curve156";
	rename -uid "8C1BC89A-4F42-29E1-F9C7-04B22BC16013";
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
	rename -uid "67E61FF2-4DAA-979B-C10B-A6BCA319F635";
createNode follicle -n "pSphere1FollicleShape6546" -p "pSphere1Follicle6546";
	rename -uid "847F3FBF-42FA-02BB-95E6-F289E7F84B6A";
	setAttr -k off ".v";
	setAttr ".pu" 0.65625;
	setAttr ".pv" 0.46875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve157" -p "pSphere1Follicle6546";
	rename -uid "3F3EFD7D-49F7-114F-F144-BEB1AF315646";
createNode nurbsCurve -n "curveShape157" -p "curve157";
	rename -uid "377F69B0-4591-E878-B1A3-30A8A98BB36D";
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
	rename -uid "E841E5A4-4B66-543E-4965-08B81EE0309A";
createNode follicle -n "pSphere1FollicleShape6553" -p "pSphere1Follicle6553";
	rename -uid "EB74D7F1-4EFC-AE35-EB86-3289D8B250D4";
	setAttr -k off ".v";
	setAttr ".pu" 0.65625;
	setAttr ".pv" 0.53125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve158" -p "pSphere1Follicle6553";
	rename -uid "A67F6CBD-43FF-5CC0-DB94-D9AB13959AC5";
createNode nurbsCurve -n "curveShape158" -p "curve158";
	rename -uid "C4FC34F4-4660-7060-B727-CE85A46EF1DE";
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
	rename -uid "D9DEF344-49E1-700A-7CDF-33BEEC72B5F5";
createNode follicle -n "pSphere1FollicleShape6559" -p "pSphere1Follicle6559";
	rename -uid "395503EE-4822-2253-F475-979E2A548E8F";
	setAttr -k off ".v";
	setAttr ".pu" 0.65625;
	setAttr ".pv" 0.59375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve159" -p "pSphere1Follicle6559";
	rename -uid "270F6D95-4ACE-5B17-3A69-A595CD991479";
createNode nurbsCurve -n "curveShape159" -p "curve159";
	rename -uid "ABE85686-4989-B03F-FEBC-7C91FB222DC0";
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
	rename -uid "EDAF2CF9-4C93-9338-424C-00B920B5EE11";
createNode follicle -n "pSphere1FollicleShape6565" -p "pSphere1Follicle6565";
	rename -uid "8092D464-49EC-BC26-4B9A-C7B895DCAC8E";
	setAttr -k off ".v";
	setAttr ".pu" 0.65625;
	setAttr ".pv" 0.65625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve160" -p "pSphere1Follicle6565";
	rename -uid "9E509EF4-4509-67FD-3DB3-2A85AA6B313A";
createNode nurbsCurve -n "curveShape160" -p "curve160";
	rename -uid "7A35C591-4B82-D1FE-FB4D-E48D6614B7E4";
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
	rename -uid "64EFFA0A-48A9-FF85-6C5F-8FB21D4805BA";
createNode follicle -n "pSphere1FollicleShape6571" -p "pSphere1Follicle6571";
	rename -uid "EFD9785F-4B39-293F-DCB7-20BB22307BEC";
	setAttr -k off ".v";
	setAttr ".pu" 0.65625;
	setAttr ".pv" 0.71875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve161" -p "pSphere1Follicle6571";
	rename -uid "B6C3A57B-4F3A-B38F-0C0D-B6899DA4A3CF";
createNode nurbsCurve -n "curveShape161" -p "curve161";
	rename -uid "DD9B0892-4CDB-9E1E-FD01-ED9F48561D23";
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
	rename -uid "9D10588F-4029-07F7-69C9-A59F81BBEEAE";
createNode follicle -n "pSphere1FollicleShape6577" -p "pSphere1Follicle6577";
	rename -uid "BB63A5CF-498C-D32D-60E8-71874BDF5760";
	setAttr -k off ".v";
	setAttr ".pu" 0.65625;
	setAttr ".pv" 0.78125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve162" -p "pSphere1Follicle6577";
	rename -uid "73CD73B6-4E73-FB7C-D615-98AC89B3B5F8";
createNode nurbsCurve -n "curveShape162" -p "curve162";
	rename -uid "8D4D8D8F-4BE1-F538-AAED-72A80999B76A";
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
	rename -uid "41079AC0-4CDB-29F0-170F-4EBEA874232C";
createNode follicle -n "pSphere1FollicleShape6584" -p "pSphere1Follicle6584";
	rename -uid "F2D6471D-4DDA-B753-E1B3-DC86665DACF9";
	setAttr -k off ".v";
	setAttr ".pu" 0.65625;
	setAttr ".pv" 0.84375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve163" -p "pSphere1Follicle6584";
	rename -uid "83F6AEF0-4B86-B3FA-1F41-708C116026FF";
createNode nurbsCurve -n "curveShape163" -p "curve163";
	rename -uid "44261982-490A-518E-5304-ADA1C192866F";
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
	rename -uid "4E2E09BC-4978-AE72-A26A-82AA522A002F";
createNode follicle -n "pSphere1FollicleShape6590" -p "pSphere1Follicle6590";
	rename -uid "D88334DF-4F86-5061-44FB-F0A9AEADDDD7";
	setAttr -k off ".v";
	setAttr ".pu" 0.65625;
	setAttr ".pv" 0.90625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve164" -p "pSphere1Follicle6590";
	rename -uid "91B1621A-4304-1132-FA73-F0854E18062B";
createNode nurbsCurve -n "curveShape164" -p "curve164";
	rename -uid "47064921-4BCE-2420-D66B-BCA114C59028";
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
	rename -uid "96902195-41C5-1A64-B182-21BCF5DA42E1";
createNode follicle -n "pSphere1FollicleShape7103" -p "pSphere1Follicle7103";
	rename -uid "14A976C4-4B51-7192-9B53-238EDA67A0CB";
	setAttr -k off ".v";
	setAttr ".pu" 0.71875;
	setAttr ".pv" 0.03125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve165" -p "pSphere1Follicle7103";
	rename -uid "B0E96504-42C2-A3AD-3C3F-5993E2DF6B72";
createNode nurbsCurve -n "curveShape165" -p "curve165";
	rename -uid "38E7F180-40CA-E2EC-0BDD-FAA8C1348592";
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
	rename -uid "215314FD-4DAD-C5C1-E030-F0B734362F2A";
createNode follicle -n "pSphere1FollicleShape7109" -p "pSphere1Follicle7109";
	rename -uid "725187C8-4E2D-9F67-448F-71B9A2F68420";
	setAttr -k off ".v";
	setAttr ".pu" 0.71875;
	setAttr ".pv" 0.09375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve166" -p "pSphere1Follicle7109";
	rename -uid "083DC4FF-40A9-85A7-846B-F9BBBB301A4C";
createNode nurbsCurve -n "curveShape166" -p "curve166";
	rename -uid "C95CA0F0-4DA0-E828-DE03-D0B60E233553";
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
	rename -uid "16A3D4E5-43A5-B7C3-6437-BC86F5EC0275";
createNode follicle -n "pSphere1FollicleShape7115" -p "pSphere1Follicle7115";
	rename -uid "7180E3F5-4A06-ADE7-1D75-7E9487CA2A96";
	setAttr -k off ".v";
	setAttr ".pu" 0.71875;
	setAttr ".pv" 0.15625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve167" -p "pSphere1Follicle7115";
	rename -uid "933C0EBE-459A-994B-ED1E-FB8C5C4CB000";
createNode nurbsCurve -n "curveShape167" -p "curve167";
	rename -uid "8076A69F-410B-5492-A7B2-D5A82C628AB9";
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
	rename -uid "80F20032-49A6-036C-8E9D-0AA4B51A9361";
createNode follicle -n "pSphere1FollicleShape7122" -p "pSphere1Follicle7122";
	rename -uid "9D07696D-4F87-7CD6-7206-0A9C9D1C470B";
	setAttr -k off ".v";
	setAttr ".pu" 0.71875;
	setAttr ".pv" 0.21875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve168" -p "pSphere1Follicle7122";
	rename -uid "BBFB2781-4E10-8171-4CA0-7E9E9A3254D7";
createNode nurbsCurve -n "curveShape168" -p "curve168";
	rename -uid "514DE4B7-4CCF-FCCB-D01B-B28231CACCE3";
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
	rename -uid "E7C4E109-4B20-63FE-843B-B9B59B6E5A6C";
createNode follicle -n "pSphere1FollicleShape7128" -p "pSphere1Follicle7128";
	rename -uid "0A23BCB4-4997-6B63-D347-69BAD44F50C6";
	setAttr -k off ".v";
	setAttr ".pu" 0.71875;
	setAttr ".pv" 0.28125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve169" -p "pSphere1Follicle7128";
	rename -uid "A146C0B0-401C-8BAB-B1FF-17896F9D4E0F";
createNode nurbsCurve -n "curveShape169" -p "curve169";
	rename -uid "3C88EC9D-48F6-22A0-CD44-E7A4B8D39314";
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
	rename -uid "214BFD03-4266-1EA7-773D-62BE0FD4C5E1";
createNode follicle -n "pSphere1FollicleShape7134" -p "pSphere1Follicle7134";
	rename -uid "9DD54E78-4A97-456F-B17D-F0B660BEB272";
	setAttr -k off ".v";
	setAttr ".pu" 0.71875;
	setAttr ".pv" 0.34375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve170" -p "pSphere1Follicle7134";
	rename -uid "B7D07AA8-4618-686C-39C0-FCB1AD3E06AA";
createNode nurbsCurve -n "curveShape170" -p "curve170";
	rename -uid "E11F5F22-44E9-1794-3E30-CFA8BEE5F7A3";
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
	rename -uid "68823C4A-49B5-7736-E8DF-BDB368D4790B";
createNode follicle -n "pSphere1FollicleShape7140" -p "pSphere1Follicle7140";
	rename -uid "4C1C9478-4763-6822-4751-339C8EB8AF12";
	setAttr -k off ".v";
	setAttr ".pu" 0.71875;
	setAttr ".pv" 0.40625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve171" -p "pSphere1Follicle7140";
	rename -uid "E06974A5-45D6-C389-A180-3FB88EE2612B";
createNode nurbsCurve -n "curveShape171" -p "curve171";
	rename -uid "02DAEB17-4F3E-4D42-9DA6-809A4861C62E";
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
	rename -uid "E34A4B2A-45C5-195B-899C-B9A7FB647C21";
createNode follicle -n "pSphere1FollicleShape7146" -p "pSphere1Follicle7146";
	rename -uid "BBC46E10-4B33-ECE5-A06E-89861B742C21";
	setAttr -k off ".v";
	setAttr ".pu" 0.71875;
	setAttr ".pv" 0.46875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve172" -p "pSphere1Follicle7146";
	rename -uid "369265A7-4312-BE7E-9962-50844D5CB258";
createNode nurbsCurve -n "curveShape172" -p "curve172";
	rename -uid "29532071-4A22-13B1-09F6-7CA35EA3DD55";
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
	rename -uid "EA96C1C5-4FC3-702F-893B-53990666B4C6";
createNode follicle -n "pSphere1FollicleShape7153" -p "pSphere1Follicle7153";
	rename -uid "E95D2191-4321-5496-3FB2-BCB037A2B38F";
	setAttr -k off ".v";
	setAttr ".pu" 0.71875;
	setAttr ".pv" 0.53125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve173" -p "pSphere1Follicle7153";
	rename -uid "90C7C7CF-43BA-8DE6-C7D3-8784C9319AA2";
createNode nurbsCurve -n "curveShape173" -p "curve173";
	rename -uid "097FEA57-44A9-5BC1-6596-4E8E6F748EAF";
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
	rename -uid "6610157F-45C1-F6B6-64AA-D3893A42EFA4";
createNode follicle -n "pSphere1FollicleShape7159" -p "pSphere1Follicle7159";
	rename -uid "684F28F9-4239-9492-EAEC-EFA47B658F6F";
	setAttr -k off ".v";
	setAttr ".pu" 0.71875;
	setAttr ".pv" 0.59375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve174" -p "pSphere1Follicle7159";
	rename -uid "DBA802BB-498B-C808-A804-29BF5B590D14";
createNode nurbsCurve -n "curveShape174" -p "curve174";
	rename -uid "D1F38171-40AB-6879-0139-1D9DC007497B";
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
	rename -uid "CBA2AD39-4389-46CE-AEB6-9B943223A3A8";
createNode follicle -n "pSphere1FollicleShape7165" -p "pSphere1Follicle7165";
	rename -uid "A0F08D01-4886-FCEF-CE71-4CA00EB248C4";
	setAttr -k off ".v";
	setAttr ".pu" 0.71875;
	setAttr ".pv" 0.65625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve175" -p "pSphere1Follicle7165";
	rename -uid "CDCA4D23-454A-9BC0-77A8-8A9EF52FE837";
createNode nurbsCurve -n "curveShape175" -p "curve175";
	rename -uid "50F8B80A-4A5D-7137-91EB-53884EBE02EC";
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
	rename -uid "1FC8C485-4662-440A-03FC-C9B4C84447A9";
createNode follicle -n "pSphere1FollicleShape7171" -p "pSphere1Follicle7171";
	rename -uid "CDA79FB5-4508-D12E-FAEE-C9925CDEE181";
	setAttr -k off ".v";
	setAttr ".pu" 0.71875;
	setAttr ".pv" 0.71875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve176" -p "pSphere1Follicle7171";
	rename -uid "96A37B66-4533-8FA1-3CE2-CBA3148F857E";
createNode nurbsCurve -n "curveShape176" -p "curve176";
	rename -uid "D006CEC9-4904-85D4-23F5-B1BED28C0E56";
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
	rename -uid "CFBA4DF6-4250-B728-29A7-72885EE0E5C2";
createNode follicle -n "pSphere1FollicleShape7177" -p "pSphere1Follicle7177";
	rename -uid "EADD99D1-475D-013D-40B6-1C8C42CB60B6";
	setAttr -k off ".v";
	setAttr ".pu" 0.71875;
	setAttr ".pv" 0.78125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve177" -p "pSphere1Follicle7177";
	rename -uid "CBB5BAF8-4996-E148-1AFD-E599D5A806DA";
createNode nurbsCurve -n "curveShape177" -p "curve177";
	rename -uid "0A8C45EE-4283-D3D6-8736-26B79BE399DB";
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
	rename -uid "5AD6C54C-4493-BB47-2D60-549DC4C79978";
createNode follicle -n "pSphere1FollicleShape7184" -p "pSphere1Follicle7184";
	rename -uid "5055DB64-4981-4E60-2D5E-1F9C66161585";
	setAttr -k off ".v";
	setAttr ".pu" 0.71875;
	setAttr ".pv" 0.84375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve178" -p "pSphere1Follicle7184";
	rename -uid "7D440F76-4AF2-386C-63F2-F2A5EDBB4313";
createNode nurbsCurve -n "curveShape178" -p "curve178";
	rename -uid "AE9A9CD6-4BCC-AD0F-5888-1AAD958FFF5A";
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
	rename -uid "FF9900B3-45A0-08DC-C449-F3B9FDD0E06D";
createNode follicle -n "pSphere1FollicleShape7190" -p "pSphere1Follicle7190";
	rename -uid "90F42E3A-41D5-E3C6-FE24-DAB0355DD34F";
	setAttr -k off ".v";
	setAttr ".pu" 0.71875;
	setAttr ".pv" 0.90625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve179" -p "pSphere1Follicle7190";
	rename -uid "F405CE4B-470B-BCFB-DC1C-BE80D3F92A66";
createNode nurbsCurve -n "curveShape179" -p "curve179";
	rename -uid "6B053B28-474A-0F5C-934D-15B5581127A1";
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
	rename -uid "D7DFB611-4B53-DA93-003E-5BAC2DAA6596";
createNode follicle -n "pSphere1FollicleShape7196" -p "pSphere1Follicle7196";
	rename -uid "70515961-494E-0A37-E42F-F8BD727B6FAF";
	setAttr -k off ".v";
	setAttr ".pu" 0.71875;
	setAttr ".pv" 0.96875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve180" -p "pSphere1Follicle7196";
	rename -uid "6E66E895-4D11-11AB-C9AB-ACA0B1BA529A";
createNode nurbsCurve -n "curveShape180" -p "curve180";
	rename -uid "85DF6E47-4652-6A30-1C7C-F990D926007C";
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
	rename -uid "6A6CEB05-4D92-6A01-A34A-FC9C00DEC3CD";
createNode follicle -n "pSphere1FollicleShape7703" -p "pSphere1Follicle7703";
	rename -uid "7B5011F0-463B-B417-2902-FD8B20813667";
	setAttr -k off ".v";
	setAttr ".pu" 0.78125;
	setAttr ".pv" 0.03125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve181" -p "pSphere1Follicle7703";
	rename -uid "F4C65566-4779-9650-2712-70AAFDE22DE2";
createNode nurbsCurve -n "curveShape181" -p "curve181";
	rename -uid "1F302647-469B-2F9F-8816-58935C997867";
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
	rename -uid "BA48CE90-4327-F758-7A7D-04B72DC3C112";
createNode follicle -n "pSphere1FollicleShape7709" -p "pSphere1Follicle7709";
	rename -uid "FF8D7A0A-4BCB-A2E1-8056-D18F051085CA";
	setAttr -k off ".v";
	setAttr ".pu" 0.78125;
	setAttr ".pv" 0.09375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve182" -p "pSphere1Follicle7709";
	rename -uid "417D9C95-49E4-3164-E8CD-98B8092D87E2";
createNode nurbsCurve -n "curveShape182" -p "curve182";
	rename -uid "D9BA3F2D-41CC-011C-8612-F189C966DFC3";
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
	rename -uid "9C71B262-41DE-A048-EFD2-EC9F6736203D";
createNode follicle -n "pSphere1FollicleShape7715" -p "pSphere1Follicle7715";
	rename -uid "A9635A37-49F2-BF0F-B5BB-169778BF43EB";
	setAttr -k off ".v";
	setAttr ".pu" 0.78125;
	setAttr ".pv" 0.15625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve183" -p "pSphere1Follicle7715";
	rename -uid "B2BBF5F7-46FF-A357-5A36-1C8920383D3F";
createNode nurbsCurve -n "curveShape183" -p "curve183";
	rename -uid "29F387C6-4B88-C00C-D683-81A78CB6CCF7";
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
	rename -uid "57B86341-4F32-BF08-A50A-35ABA31CD585";
createNode follicle -n "pSphere1FollicleShape7722" -p "pSphere1Follicle7722";
	rename -uid "B47AC76B-476A-9F33-0542-CAA51934DB66";
	setAttr -k off ".v";
	setAttr ".pu" 0.78125;
	setAttr ".pv" 0.21875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve184" -p "pSphere1Follicle7722";
	rename -uid "84F94742-47CA-BF4A-C7C6-AA8DB567F210";
createNode nurbsCurve -n "curveShape184" -p "curve184";
	rename -uid "BE9F5EEF-42BD-E8AC-4423-EB881C66BD25";
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
	rename -uid "97FE9657-4B90-46B1-0106-878149421DEB";
createNode follicle -n "pSphere1FollicleShape7728" -p "pSphere1Follicle7728";
	rename -uid "F96B3831-463C-7B5A-A51F-23956A272FCB";
	setAttr -k off ".v";
	setAttr ".pu" 0.78125;
	setAttr ".pv" 0.28125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve185" -p "pSphere1Follicle7728";
	rename -uid "38F028E9-42F6-F465-786D-EB974B05B9F4";
createNode nurbsCurve -n "curveShape185" -p "curve185";
	rename -uid "5A1D9649-4341-EA24-3E01-3CB7C6023CA5";
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
	rename -uid "47E39D6F-4204-5A09-9B2F-078B80F828F0";
createNode follicle -n "pSphere1FollicleShape7734" -p "pSphere1Follicle7734";
	rename -uid "0FFD4558-4E31-EC72-E897-17B1249BF2D9";
	setAttr -k off ".v";
	setAttr ".pu" 0.78125;
	setAttr ".pv" 0.34375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve186" -p "pSphere1Follicle7734";
	rename -uid "E8C2C52D-46CD-133B-D536-5786E8B0450F";
createNode nurbsCurve -n "curveShape186" -p "curve186";
	rename -uid "9B29DEF4-4FD8-1871-84AE-3983DF3FF253";
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
	rename -uid "4BBCCEF5-47FD-FA69-E7E2-0EBAD1B237A7";
createNode follicle -n "pSphere1FollicleShape7740" -p "pSphere1Follicle7740";
	rename -uid "062DF579-44DC-6CB3-5B81-E1A9B2177690";
	setAttr -k off ".v";
	setAttr ".pu" 0.78125;
	setAttr ".pv" 0.40625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve187" -p "pSphere1Follicle7740";
	rename -uid "A110C65E-4F8B-EC77-71DA-45A5CE72FD7C";
createNode nurbsCurve -n "curveShape187" -p "curve187";
	rename -uid "C505E1CC-4714-1BE7-459F-34B969651CDF";
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
	rename -uid "5F1D2150-49B3-549F-2099-E3A39B00A41A";
createNode follicle -n "pSphere1FollicleShape7746" -p "pSphere1Follicle7746";
	rename -uid "D325208E-4528-4037-1E3A-5AA37C9AA683";
	setAttr -k off ".v";
	setAttr ".pu" 0.78125;
	setAttr ".pv" 0.46875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve188" -p "pSphere1Follicle7746";
	rename -uid "591AEC6A-42A0-6EF4-EFD8-8780722FC573";
createNode nurbsCurve -n "curveShape188" -p "curve188";
	rename -uid "EB4E4F7B-4116-49D1-469A-E68D704BE775";
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
	rename -uid "1AAF5C43-4599-41BD-0FCD-59BAD7EB9484";
createNode follicle -n "pSphere1FollicleShape7753" -p "pSphere1Follicle7753";
	rename -uid "387947D6-4668-9790-2A41-E2AC88EA0D3A";
	setAttr -k off ".v";
	setAttr ".pu" 0.78125;
	setAttr ".pv" 0.53125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve189" -p "pSphere1Follicle7753";
	rename -uid "38DEB21F-4866-3A3E-D188-44B838AFB715";
createNode nurbsCurve -n "curveShape189" -p "curve189";
	rename -uid "90C4B7FB-4E71-7172-EF0B-D9918B4DB74A";
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
	rename -uid "1FCFCB48-4945-529A-A12C-3B97C2C0868C";
createNode follicle -n "pSphere1FollicleShape7759" -p "pSphere1Follicle7759";
	rename -uid "BB80DCE5-4464-9DF1-A83A-CB8FE8D73298";
	setAttr -k off ".v";
	setAttr ".pu" 0.78125;
	setAttr ".pv" 0.59375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve190" -p "pSphere1Follicle7759";
	rename -uid "1FD5C483-44D9-CE17-AE31-A791AD4E0508";
createNode nurbsCurve -n "curveShape190" -p "curve190";
	rename -uid "D31486FB-4B37-DFC3-92EC-A684D6202838";
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
	rename -uid "6E1B2346-4B9A-6E48-4E85-AEA45DA40FFF";
createNode follicle -n "pSphere1FollicleShape7765" -p "pSphere1Follicle7765";
	rename -uid "C073AF1A-41FF-F318-1052-5493621F2D51";
	setAttr -k off ".v";
	setAttr ".pu" 0.78125;
	setAttr ".pv" 0.65625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve191" -p "pSphere1Follicle7765";
	rename -uid "31005A0A-4FBA-0BB3-18A1-B08CF579071E";
createNode nurbsCurve -n "curveShape191" -p "curve191";
	rename -uid "E2223382-44CC-84D0-F2A2-B291F88BB7C6";
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
	rename -uid "8BAB3706-4337-24D8-4E8F-67A5169DDBBC";
createNode follicle -n "pSphere1FollicleShape7771" -p "pSphere1Follicle7771";
	rename -uid "515558BB-4367-AAEB-7CBC-41B3FB82A3AE";
	setAttr -k off ".v";
	setAttr ".pu" 0.78125;
	setAttr ".pv" 0.71875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve192" -p "pSphere1Follicle7771";
	rename -uid "6F513A50-44DC-81DD-D15C-8BA54A224F78";
createNode nurbsCurve -n "curveShape192" -p "curve192";
	rename -uid "A3E3E270-4F1C-C651-9023-9FB47E435250";
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
	rename -uid "E06717FB-4767-9363-439F-CBB2B73C5C8D";
createNode follicle -n "pSphere1FollicleShape7777" -p "pSphere1Follicle7777";
	rename -uid "A3F3B094-4F35-6C93-F512-8C9ABBE844D8";
	setAttr -k off ".v";
	setAttr ".pu" 0.78125;
	setAttr ".pv" 0.78125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve193" -p "pSphere1Follicle7777";
	rename -uid "0566ECAD-4A85-4433-6A5A-3B9F75ADB538";
createNode nurbsCurve -n "curveShape193" -p "curve193";
	rename -uid "CBB7C60B-4393-041C-D2B5-6DB2FCC45053";
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
	rename -uid "3919B02F-49E3-4FAD-20C2-B083A18D6AB3";
createNode follicle -n "pSphere1FollicleShape7784" -p "pSphere1Follicle7784";
	rename -uid "CF1BD1EC-42CC-814D-03B1-7C887136DF8B";
	setAttr -k off ".v";
	setAttr ".pu" 0.78125;
	setAttr ".pv" 0.84375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve194" -p "pSphere1Follicle7784";
	rename -uid "57EF96B7-45DC-A19A-DA41-B09E512E0971";
createNode nurbsCurve -n "curveShape194" -p "curve194";
	rename -uid "D4788C96-49DB-B0C4-3ADC-0BA3F29A0C67";
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
	rename -uid "8B2AAF07-4C87-304D-C148-07B378DC3BA3";
createNode follicle -n "pSphere1FollicleShape7790" -p "pSphere1Follicle7790";
	rename -uid "C3247715-43E8-FC26-2004-D18E05296CA2";
	setAttr -k off ".v";
	setAttr ".pu" 0.78125;
	setAttr ".pv" 0.90625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve195" -p "pSphere1Follicle7790";
	rename -uid "886A0323-48F9-3B70-FEB7-BB9C879989CE";
createNode nurbsCurve -n "curveShape195" -p "curve195";
	rename -uid "591494DF-4323-4CFD-50B8-A9AC63E37E42";
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
	rename -uid "450EF574-4B03-5AA2-410C-9CA3A676B75B";
createNode follicle -n "pSphere1FollicleShape7796" -p "pSphere1Follicle7796";
	rename -uid "0DCEBF9B-4FE8-2D50-A0BB-4AAFFBD94324";
	setAttr -k off ".v";
	setAttr ".pu" 0.78125;
	setAttr ".pv" 0.96875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve196" -p "pSphere1Follicle7796";
	rename -uid "49A389F2-40AF-B2D4-8BCE-59929D2465B0";
createNode nurbsCurve -n "curveShape196" -p "curve196";
	rename -uid "BA1ABDAF-4909-1BEB-3525-00A05CA03C25";
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
	rename -uid "67CCF50E-4E95-E19C-CBE7-069592646F74";
createNode follicle -n "pSphere1FollicleShape8409" -p "pSphere1Follicle8409";
	rename -uid "4BD28F5D-4ED6-36B6-A148-D4BF0B9421A1";
	setAttr -k off ".v";
	setAttr ".pu" 0.84375;
	setAttr ".pv" 0.09375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve197" -p "pSphere1Follicle8409";
	rename -uid "C3C4F77A-4FF9-9664-5DE0-62A7E4BFAD08";
createNode nurbsCurve -n "curveShape197" -p "curve197";
	rename -uid "1DEEDFEE-4AC5-D8C2-A1FE-74B4AB04CD90";
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
	rename -uid "6E723BD7-42F6-7A8B-5F55-0287A2252736";
createNode follicle -n "pSphere1FollicleShape8415" -p "pSphere1Follicle8415";
	rename -uid "F4089E24-4D64-C3BA-1B4F-3D8F29846892";
	setAttr -k off ".v";
	setAttr ".pu" 0.84375;
	setAttr ".pv" 0.15625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve198" -p "pSphere1Follicle8415";
	rename -uid "CBA8CA68-4F5C-6251-8570-E89675B45889";
createNode nurbsCurve -n "curveShape198" -p "curve198";
	rename -uid "41A182A7-48D6-7A68-9A10-C2BE11181BC5";
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
	rename -uid "E29ABD33-4DA8-B837-1E0F-C4A531C4F61F";
createNode follicle -n "pSphere1FollicleShape8422" -p "pSphere1Follicle8422";
	rename -uid "203CF881-4109-56A0-495E-819E2FC3E0F2";
	setAttr -k off ".v";
	setAttr ".pu" 0.84375;
	setAttr ".pv" 0.21875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve199" -p "pSphere1Follicle8422";
	rename -uid "A5D17325-47F6-CA36-DA45-E796573DE79C";
createNode nurbsCurve -n "curveShape199" -p "curve199";
	rename -uid "4AA6A46B-499B-C026-55FA-C3916E618AC6";
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
	rename -uid "6E1B2246-4FF4-3474-593D-C79112A9F8FD";
createNode follicle -n "pSphere1FollicleShape8428" -p "pSphere1Follicle8428";
	rename -uid "8FE1523A-44D4-C4E7-D01C-A8B977816D8A";
	setAttr -k off ".v";
	setAttr ".pu" 0.84375;
	setAttr ".pv" 0.28125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve200" -p "pSphere1Follicle8428";
	rename -uid "A27DE22A-462C-5BEE-AED5-6A9A71C7926E";
createNode nurbsCurve -n "curveShape200" -p "curve200";
	rename -uid "57D5672C-494F-E5F5-5204-83A149ABCFF4";
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
	rename -uid "9A5FC954-41A8-28E4-7CA3-14A310AD8F3A";
createNode follicle -n "pSphere1FollicleShape8434" -p "pSphere1Follicle8434";
	rename -uid "C7E34F70-4F89-194F-0AE8-7F956AAB9554";
	setAttr -k off ".v";
	setAttr ".pu" 0.84375;
	setAttr ".pv" 0.34375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve201" -p "pSphere1Follicle8434";
	rename -uid "3405458B-4B79-6B1C-B97B-1FA3DD64B4BA";
createNode nurbsCurve -n "curveShape201" -p "curve201";
	rename -uid "4F3FEF13-4260-E72A-0DEE-97851F99619B";
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
	rename -uid "0484C233-414C-1570-82B1-9AA6913C4B41";
createNode follicle -n "pSphere1FollicleShape8440" -p "pSphere1Follicle8440";
	rename -uid "74B4A280-45FA-70D0-DFD7-449CE3E4F045";
	setAttr -k off ".v";
	setAttr ".pu" 0.84375;
	setAttr ".pv" 0.40625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve202" -p "pSphere1Follicle8440";
	rename -uid "1F6B6D0F-41C5-7CCB-1800-CEBFECDAEFCB";
createNode nurbsCurve -n "curveShape202" -p "curve202";
	rename -uid "FFBA6084-4F4D-079A-2C7A-FCB2163D6BF0";
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
	rename -uid "1C3119FE-436F-B178-3093-FAB58B25409D";
createNode follicle -n "pSphere1FollicleShape8446" -p "pSphere1Follicle8446";
	rename -uid "5BC76F5C-4115-1EF6-76A2-EFA626DB76BF";
	setAttr -k off ".v";
	setAttr ".pu" 0.84375;
	setAttr ".pv" 0.46875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve203" -p "pSphere1Follicle8446";
	rename -uid "5180FD64-473F-7C80-8C15-A58CB8D23EBA";
createNode nurbsCurve -n "curveShape203" -p "curve203";
	rename -uid "E9124600-4072-4952-3A9D-EF89BB06C6D0";
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
	rename -uid "92600BE4-4280-56B5-D958-98846E925DA2";
createNode follicle -n "pSphere1FollicleShape8453" -p "pSphere1Follicle8453";
	rename -uid "3A518AE6-44F7-72B5-2DB9-AAB8D5431E61";
	setAttr -k off ".v";
	setAttr ".pu" 0.84375;
	setAttr ".pv" 0.53125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve204" -p "pSphere1Follicle8453";
	rename -uid "87F60C63-46ED-021A-B24C-9AA93607AEB2";
createNode nurbsCurve -n "curveShape204" -p "curve204";
	rename -uid "02FCA11A-4C30-AFED-5AFA-E8B94A1FA3EB";
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
	rename -uid "A14CC565-437B-8225-779A-FB8BC4325C88";
createNode follicle -n "pSphere1FollicleShape8459" -p "pSphere1Follicle8459";
	rename -uid "87F49CF9-4DDC-78CC-070D-B2A3C1646CB9";
	setAttr -k off ".v";
	setAttr ".pu" 0.84375;
	setAttr ".pv" 0.59375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve205" -p "pSphere1Follicle8459";
	rename -uid "E29A17E6-4502-21FB-00D9-969AAA6F158C";
createNode nurbsCurve -n "curveShape205" -p "curve205";
	rename -uid "B18B84D3-41D0-90AF-B4EA-F1B45AB1F030";
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
	rename -uid "E82A6480-418E-EEAB-8E62-3AA226BB98C6";
createNode follicle -n "pSphere1FollicleShape8465" -p "pSphere1Follicle8465";
	rename -uid "B47127B1-44C2-293C-C477-6C851CE775BD";
	setAttr -k off ".v";
	setAttr ".pu" 0.84375;
	setAttr ".pv" 0.65625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve206" -p "pSphere1Follicle8465";
	rename -uid "DFCD48C7-4338-7D7C-A67C-4E9AD2C8965D";
createNode nurbsCurve -n "curveShape206" -p "curve206";
	rename -uid "D87A9155-4FA8-EE79-447D-31908033B807";
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
	rename -uid "5B55DA35-42DF-83FD-B9A8-E0892E0A27E5";
createNode follicle -n "pSphere1FollicleShape8471" -p "pSphere1Follicle8471";
	rename -uid "E3698AC3-4E8A-8FCF-250D-CA9B7560083E";
	setAttr -k off ".v";
	setAttr ".pu" 0.84375;
	setAttr ".pv" 0.71875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve207" -p "pSphere1Follicle8471";
	rename -uid "AFE063A9-411C-B0AA-910F-1E82E6BF1BE8";
createNode nurbsCurve -n "curveShape207" -p "curve207";
	rename -uid "BBC791CB-4312-6C82-5378-04B697B1821F";
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
	rename -uid "E3AE701F-427D-1941-2114-C4800F9C666B";
createNode follicle -n "pSphere1FollicleShape8477" -p "pSphere1Follicle8477";
	rename -uid "F52DC671-4EFD-25E0-C9EC-859D5DD1B7CA";
	setAttr -k off ".v";
	setAttr ".pu" 0.84375;
	setAttr ".pv" 0.78125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve208" -p "pSphere1Follicle8477";
	rename -uid "73234FB9-43A0-F32F-7E83-FE90C59D37A0";
createNode nurbsCurve -n "curveShape208" -p "curve208";
	rename -uid "BC8BBB6C-4C43-17D8-54EB-C5BEF4116F80";
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
	rename -uid "E70BFFA4-4154-9157-99BE-E597C4E08303";
createNode follicle -n "pSphere1FollicleShape8484" -p "pSphere1Follicle8484";
	rename -uid "5286DB08-481F-D59A-8834-4C961A33E9E9";
	setAttr -k off ".v";
	setAttr ".pu" 0.84375;
	setAttr ".pv" 0.84375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve209" -p "pSphere1Follicle8484";
	rename -uid "2C83878E-4671-EB6C-25F3-DC8F0D987CC4";
createNode nurbsCurve -n "curveShape209" -p "curve209";
	rename -uid "5A83629E-4C62-0B00-1B3D-83BA1690BD82";
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
	rename -uid "A8612388-490C-CE0A-8F28-CEA81F001E86";
createNode follicle -n "pSphere1FollicleShape8490" -p "pSphere1Follicle8490";
	rename -uid "9EA93F8D-416D-79E6-0714-CD8C0EE5DE3E";
	setAttr -k off ".v";
	setAttr ".pu" 0.84375;
	setAttr ".pv" 0.90625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve210" -p "pSphere1Follicle8490";
	rename -uid "584F6F3E-411D-D8EC-6C3A-9BAF7A025DD2";
createNode nurbsCurve -n "curveShape210" -p "curve210";
	rename -uid "CB62CEF6-47EB-D601-D1B8-DE92094C5C09";
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
	rename -uid "5FD617D8-4E89-15BF-6E0F-91BFA32053DE";
createNode follicle -n "pSphere1FollicleShape9009" -p "pSphere1Follicle9009";
	rename -uid "9D7AB4A3-4291-DC44-4839-B89076D1142D";
	setAttr -k off ".v";
	setAttr ".pu" 0.90625;
	setAttr ".pv" 0.09375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve211" -p "pSphere1Follicle9009";
	rename -uid "5935F021-412B-E16D-D2C4-48991D5B2946";
createNode nurbsCurve -n "curveShape211" -p "curve211";
	rename -uid "761F8669-4ED2-D8FC-A127-6387C59CE37A";
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
	rename -uid "B2FA3ED9-4971-9082-120B-159749221F1D";
createNode follicle -n "pSphere1FollicleShape9015" -p "pSphere1Follicle9015";
	rename -uid "28EC3BF5-445E-FB37-613C-E2836AF892EA";
	setAttr -k off ".v";
	setAttr ".pu" 0.90625;
	setAttr ".pv" 0.15625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve212" -p "pSphere1Follicle9015";
	rename -uid "AA613CEB-435E-DEAF-56E9-1E80DE9F7486";
createNode nurbsCurve -n "curveShape212" -p "curve212";
	rename -uid "22A1ABCD-4A89-182C-B205-19A410A1A5D1";
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
	rename -uid "87F1181F-42A5-A86D-C0A0-C1994617135D";
createNode follicle -n "pSphere1FollicleShape9022" -p "pSphere1Follicle9022";
	rename -uid "09732B7B-435D-EB2D-EA8C-AAB7CBF976A1";
	setAttr -k off ".v";
	setAttr ".pu" 0.90625;
	setAttr ".pv" 0.21875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve213" -p "pSphere1Follicle9022";
	rename -uid "A046B67C-4531-F3AB-8A44-E88E9B89A543";
createNode nurbsCurve -n "curveShape213" -p "curve213";
	rename -uid "E4925720-4C6E-2406-EA71-83870DDEDC94";
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
	rename -uid "84CA66F1-447C-A4CC-5F98-D9926118BC66";
createNode follicle -n "pSphere1FollicleShape9028" -p "pSphere1Follicle9028";
	rename -uid "666F78C0-4078-96B9-2D2D-A3959D81C6EA";
	setAttr -k off ".v";
	setAttr ".pu" 0.90625;
	setAttr ".pv" 0.28125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve214" -p "pSphere1Follicle9028";
	rename -uid "18B57965-45C9-A69B-332B-80871B0ECDBB";
createNode nurbsCurve -n "curveShape214" -p "curve214";
	rename -uid "C46092E9-4552-B54F-1967-8697ED4A9643";
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
	rename -uid "11E0A017-48A2-1919-D36F-CA81DD01E265";
createNode follicle -n "pSphere1FollicleShape9034" -p "pSphere1Follicle9034";
	rename -uid "36F34B6C-4E42-D068-3931-F7A0CDCD9E49";
	setAttr -k off ".v";
	setAttr ".pu" 0.90625;
	setAttr ".pv" 0.34375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve215" -p "pSphere1Follicle9034";
	rename -uid "0CA949A8-4964-9318-2D09-16BEEC9EE9EC";
createNode nurbsCurve -n "curveShape215" -p "curve215";
	rename -uid "BEE53032-40F6-AF7E-598D-BCAF7C8BD028";
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
	rename -uid "4401E74D-4730-1BEB-AE83-3A8765F49AD1";
createNode follicle -n "pSphere1FollicleShape9040" -p "pSphere1Follicle9040";
	rename -uid "4F9D020A-4768-000D-F503-FE93849A82A9";
	setAttr -k off ".v";
	setAttr ".pu" 0.90625;
	setAttr ".pv" 0.40625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve216" -p "pSphere1Follicle9040";
	rename -uid "2E847028-4353-A4A3-ADAA-A6B96D7E938C";
createNode nurbsCurve -n "curveShape216" -p "curve216";
	rename -uid "F1BC870D-4EDC-3475-3B88-09991474644A";
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
	rename -uid "3770FD50-4671-A0CA-46E9-5DA6BA741017";
createNode follicle -n "pSphere1FollicleShape9046" -p "pSphere1Follicle9046";
	rename -uid "BCD0738F-4F08-E287-7DB9-70A8C8152293";
	setAttr -k off ".v";
	setAttr ".pu" 0.90625;
	setAttr ".pv" 0.46875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve217" -p "pSphere1Follicle9046";
	rename -uid "583FD818-46CC-AF9A-90B3-AF81AE20B8AF";
createNode nurbsCurve -n "curveShape217" -p "curve217";
	rename -uid "1D98DE35-4186-FF31-CCDD-88A9F14DF038";
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
	rename -uid "FC2F6D1A-4B07-3A98-54BE-EAB5ADCDD5D5";
createNode follicle -n "pSphere1FollicleShape9053" -p "pSphere1Follicle9053";
	rename -uid "1B78D960-4058-7009-0ED3-72A4E0705C7D";
	setAttr -k off ".v";
	setAttr ".pu" 0.90625;
	setAttr ".pv" 0.53125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve218" -p "pSphere1Follicle9053";
	rename -uid "C30BEDCB-4D96-F655-3DC2-509B06CF9A19";
createNode nurbsCurve -n "curveShape218" -p "curve218";
	rename -uid "0CC07AEC-4A08-7903-0903-CCA6168F1EC2";
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
	rename -uid "5EE6DB95-4E40-3F6F-D59E-C09A8199F008";
createNode follicle -n "pSphere1FollicleShape9059" -p "pSphere1Follicle9059";
	rename -uid "9EB6718C-436B-4A0A-A3B7-0BA6F2B3C4CF";
	setAttr -k off ".v";
	setAttr ".pu" 0.90625;
	setAttr ".pv" 0.59375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve219" -p "pSphere1Follicle9059";
	rename -uid "6D03AE59-447E-3C80-DB5B-7F91095D1725";
createNode nurbsCurve -n "curveShape219" -p "curve219";
	rename -uid "B8D9B2E1-4557-483D-7698-D094E53A45EA";
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
	rename -uid "511B51E5-4BD3-1A5F-DF93-6185A8647A67";
createNode follicle -n "pSphere1FollicleShape9065" -p "pSphere1Follicle9065";
	rename -uid "2DCDFE2A-4F39-20F6-D733-D3BD07427916";
	setAttr -k off ".v";
	setAttr ".pu" 0.90625;
	setAttr ".pv" 0.65625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve220" -p "pSphere1Follicle9065";
	rename -uid "56AEC921-4583-6007-0053-55B433CE4084";
createNode nurbsCurve -n "curveShape220" -p "curve220";
	rename -uid "B7A6A68E-4DEF-143C-640F-699F49667509";
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
	rename -uid "77FB1264-4D15-9D05-6749-A1B9C188438F";
createNode follicle -n "pSphere1FollicleShape9071" -p "pSphere1Follicle9071";
	rename -uid "98EDFBB4-4321-6D52-7090-7BB4A7CD6B6F";
	setAttr -k off ".v";
	setAttr ".pu" 0.90625;
	setAttr ".pv" 0.71875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve221" -p "pSphere1Follicle9071";
	rename -uid "6B0EFD21-490A-6D19-0E8A-CE8287D7F101";
createNode nurbsCurve -n "curveShape221" -p "curve221";
	rename -uid "D0AEC4D7-4784-4BB8-C8D8-78A5A41675DC";
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
	rename -uid "F9AD168E-47B2-256C-70C9-A1B3A1C1189E";
createNode follicle -n "pSphere1FollicleShape9077" -p "pSphere1Follicle9077";
	rename -uid "8853BBF2-4E6C-3EC9-3E63-49A25D9CBD80";
	setAttr -k off ".v";
	setAttr ".pu" 0.90625;
	setAttr ".pv" 0.78125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve222" -p "pSphere1Follicle9077";
	rename -uid "10266E40-4666-A4AA-66FE-51A1A75E9E8C";
createNode nurbsCurve -n "curveShape222" -p "curve222";
	rename -uid "EBC8153E-45A7-3F9E-1587-12AE93AA644A";
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
	rename -uid "D6E14E6B-4FE7-1E3C-A85C-A68B1810DABD";
createNode follicle -n "pSphere1FollicleShape9084" -p "pSphere1Follicle9084";
	rename -uid "277FF49C-4F56-E0E2-78EC-8DAFA7A90B17";
	setAttr -k off ".v";
	setAttr ".pu" 0.90625;
	setAttr ".pv" 0.84375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve223" -p "pSphere1Follicle9084";
	rename -uid "59F09CB6-4525-1F85-C109-4BAF79F8B39E";
createNode nurbsCurve -n "curveShape223" -p "curve223";
	rename -uid "C8C032A9-4D94-76A5-5188-C380FF70F222";
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
	rename -uid "D17634AD-4596-F2FD-C94F-429E866E3CC0";
createNode follicle -n "pSphere1FollicleShape9090" -p "pSphere1Follicle9090";
	rename -uid "D2CB103B-413A-4AA4-A3F3-EC9B5867F7D8";
	setAttr -k off ".v";
	setAttr ".pu" 0.90625;
	setAttr ".pv" 0.90625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve224" -p "pSphere1Follicle9090";
	rename -uid "CF06C963-4B33-357F-54DA-1782AF99F853";
createNode nurbsCurve -n "curveShape224" -p "curve224";
	rename -uid "FF782190-4DDE-D76B-DC62-05AB758FAD88";
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
	rename -uid "50B14B96-4511-C806-0B26-2AAD7FB9CD3F";
createNode follicle -n "pSphere1FollicleShape9603" -p "pSphere1Follicle9603";
	rename -uid "19ECA66B-4903-FD1B-383D-89969E54DB61";
	setAttr -k off ".v";
	setAttr ".pu" 0.96875;
	setAttr ".pv" 0.03125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve225" -p "pSphere1Follicle9603";
	rename -uid "6AEDFF03-408E-6275-1158-B7AFD489433C";
createNode nurbsCurve -n "curveShape225" -p "curve225";
	rename -uid "3D4632E8-4361-8E26-0277-AD99FD1B407A";
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
	rename -uid "7A915815-4FB5-4974-4B08-418BBD7B5591";
createNode follicle -n "pSphere1FollicleShape9609" -p "pSphere1Follicle9609";
	rename -uid "D6C3662C-43B3-678F-F3BA-1681DC2CBDF2";
	setAttr -k off ".v";
	setAttr ".pu" 0.96875;
	setAttr ".pv" 0.09375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve226" -p "pSphere1Follicle9609";
	rename -uid "BF479E38-43C8-99F2-10EC-6E8A9E415FFB";
createNode nurbsCurve -n "curveShape226" -p "curve226";
	rename -uid "0DC11C53-489B-C482-60B4-0AB29DFA062F";
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
	rename -uid "5501610C-42BD-3185-EA8E-AA8305C18B3F";
createNode follicle -n "pSphere1FollicleShape9615" -p "pSphere1Follicle9615";
	rename -uid "ED187AE2-471D-6891-ACA1-86A29FE23DC8";
	setAttr -k off ".v";
	setAttr ".pu" 0.96875;
	setAttr ".pv" 0.15625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve227" -p "pSphere1Follicle9615";
	rename -uid "E5629E05-477A-7916-0256-B2898F59712A";
createNode nurbsCurve -n "curveShape227" -p "curve227";
	rename -uid "D8930C26-47D2-9257-C32D-028C821EC52D";
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
	rename -uid "BE881B8C-4F36-4913-29ED-D2995AD00750";
createNode follicle -n "pSphere1FollicleShape9622" -p "pSphere1Follicle9622";
	rename -uid "22943EFA-4728-FCE7-3108-F4BC6666F7AF";
	setAttr -k off ".v";
	setAttr ".pu" 0.96875;
	setAttr ".pv" 0.21875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve228" -p "pSphere1Follicle9622";
	rename -uid "4CE9F60F-42E4-C54B-9891-F2A1D905A289";
createNode nurbsCurve -n "curveShape228" -p "curve228";
	rename -uid "C39BEE6A-49A1-0032-F011-BDB6602FC978";
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
	rename -uid "1C02312E-497E-EB22-B4E7-CBAD477DEF3C";
createNode follicle -n "pSphere1FollicleShape9628" -p "pSphere1Follicle9628";
	rename -uid "8D38E181-46C1-91B6-9303-D2BD00EB143F";
	setAttr -k off ".v";
	setAttr ".pu" 0.96875;
	setAttr ".pv" 0.28125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve229" -p "pSphere1Follicle9628";
	rename -uid "2BCC069C-46FB-0C6B-D33D-93859D07BE3C";
createNode nurbsCurve -n "curveShape229" -p "curve229";
	rename -uid "85751560-481B-D828-ED06-EEBA2AF8A612";
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
	rename -uid "6F1E832D-4700-4E66-3E80-B88331DCEAE9";
createNode follicle -n "pSphere1FollicleShape9634" -p "pSphere1Follicle9634";
	rename -uid "CDAC275E-4367-B01D-0B95-DE8A7BDED254";
	setAttr -k off ".v";
	setAttr ".pu" 0.96875;
	setAttr ".pv" 0.34375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve230" -p "pSphere1Follicle9634";
	rename -uid "03828C5A-4FB2-71F7-EFA4-AA8D9BC81ACF";
createNode nurbsCurve -n "curveShape230" -p "curve230";
	rename -uid "DD073BD6-4A81-A36D-3CDA-34BF433971B7";
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
	rename -uid "453F4303-48D6-31D5-1628-188AC730365A";
createNode follicle -n "pSphere1FollicleShape9640" -p "pSphere1Follicle9640";
	rename -uid "0DB50F54-48EE-BEB7-E844-23A964D659B3";
	setAttr -k off ".v";
	setAttr ".pu" 0.96875;
	setAttr ".pv" 0.40625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve231" -p "pSphere1Follicle9640";
	rename -uid "85DDD0D6-473E-A201-8F2A-C1B404355411";
createNode nurbsCurve -n "curveShape231" -p "curve231";
	rename -uid "CC697856-4191-83AC-BBC0-8E99CB9F8E58";
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
	rename -uid "2BE3E7EC-4C2D-5F47-E67F-FB9B71828E0F";
createNode follicle -n "pSphere1FollicleShape9646" -p "pSphere1Follicle9646";
	rename -uid "6C0A8198-4F9C-91C8-A50F-78ABB0D03EA4";
	setAttr -k off ".v";
	setAttr ".pu" 0.96875;
	setAttr ".pv" 0.46875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve232" -p "pSphere1Follicle9646";
	rename -uid "FCC01518-4010-945E-40EF-6DBA52914442";
createNode nurbsCurve -n "curveShape232" -p "curve232";
	rename -uid "00911ED8-481E-EF9D-82A0-44B7CE28C480";
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
	rename -uid "9B578BB5-4632-78A6-6904-8EB2ADA228AB";
createNode follicle -n "pSphere1FollicleShape9653" -p "pSphere1Follicle9653";
	rename -uid "5DAFE48C-4F75-2944-1483-FE849FCC4CCA";
	setAttr -k off ".v";
	setAttr ".pu" 0.96875;
	setAttr ".pv" 0.53125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve233" -p "pSphere1Follicle9653";
	rename -uid "738380C4-4D82-91E8-6162-F790211A05F3";
createNode nurbsCurve -n "curveShape233" -p "curve233";
	rename -uid "A56A7729-484D-BE90-1B09-578382A558AD";
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
	rename -uid "13F5B6CF-441B-D9BA-91B7-29B30E2895D7";
createNode follicle -n "pSphere1FollicleShape9659" -p "pSphere1Follicle9659";
	rename -uid "9984ABA8-4896-38E6-F772-E2AEC38065CA";
	setAttr -k off ".v";
	setAttr ".pu" 0.96875;
	setAttr ".pv" 0.59375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve234" -p "pSphere1Follicle9659";
	rename -uid "758B5990-4D85-429C-5A02-32BA2BDF90BC";
createNode nurbsCurve -n "curveShape234" -p "curve234";
	rename -uid "E1F485DE-4D83-2290-AF48-F29DDACECC4D";
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
	rename -uid "D5F9E15A-42C7-A0AC-FF9D-EFAB56AFCEFE";
createNode follicle -n "pSphere1FollicleShape9665" -p "pSphere1Follicle9665";
	rename -uid "5846D190-4460-D000-A119-E1A084D58BBA";
	setAttr -k off ".v";
	setAttr ".pu" 0.96875;
	setAttr ".pv" 0.65625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve235" -p "pSphere1Follicle9665";
	rename -uid "D3F34B9B-480A-F7DA-4D3E-27BCDDD740DC";
createNode nurbsCurve -n "curveShape235" -p "curve235";
	rename -uid "D83A69E2-4E5E-2FDE-49F1-BF9EC7969972";
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
	rename -uid "A91E31F8-40CA-F511-C2A0-D3BA4D62A764";
createNode follicle -n "pSphere1FollicleShape9671" -p "pSphere1Follicle9671";
	rename -uid "22A0E06D-42C6-5A91-E112-1DA77A4C2214";
	setAttr -k off ".v";
	setAttr ".pu" 0.96875;
	setAttr ".pv" 0.71875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve236" -p "pSphere1Follicle9671";
	rename -uid "2626DF17-438E-5B23-C755-DEBAEF12E434";
createNode nurbsCurve -n "curveShape236" -p "curve236";
	rename -uid "1CF46121-43F6-A1CC-97AA-10B2904763BB";
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
	rename -uid "01F8F08E-4652-2A7E-25B0-37B05E75D107";
createNode follicle -n "pSphere1FollicleShape9677" -p "pSphere1Follicle9677";
	rename -uid "5AD39BE9-4CA3-BC4A-3775-F5BE06E99462";
	setAttr -k off ".v";
	setAttr ".pu" 0.96875;
	setAttr ".pv" 0.78125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve237" -p "pSphere1Follicle9677";
	rename -uid "C5D347F8-4873-8712-8EEB-94871FFBB146";
createNode nurbsCurve -n "curveShape237" -p "curve237";
	rename -uid "AC8D9A6A-469E-1A0B-18F9-EBA817837294";
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
	rename -uid "66761D13-4402-7BDC-9D19-E28958EAFAD7";
createNode follicle -n "pSphere1FollicleShape9684" -p "pSphere1Follicle9684";
	rename -uid "80730C0D-4BAA-6371-EFBB-FA9AD51D4BF9";
	setAttr -k off ".v";
	setAttr ".pu" 0.96875;
	setAttr ".pv" 0.84375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve238" -p "pSphere1Follicle9684";
	rename -uid "928DE235-44ED-864E-D85D-4084EE483760";
createNode nurbsCurve -n "curveShape238" -p "curve238";
	rename -uid "65C12FC7-407C-A994-4085-06AD99D75589";
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
	rename -uid "416C8D50-431A-19E6-C426-CAB3FE259416";
createNode follicle -n "pSphere1FollicleShape9690" -p "pSphere1Follicle9690";
	rename -uid "8249843D-42D5-B778-F310-869A49C39F9A";
	setAttr -k off ".v";
	setAttr ".pu" 0.96875;
	setAttr ".pv" 0.90625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve239" -p "pSphere1Follicle9690";
	rename -uid "764CEFDC-4F92-8C50-1096-DD881E220C4F";
createNode nurbsCurve -n "curveShape239" -p "curve239";
	rename -uid "1D57C876-40EB-A69C-78AD-A3934479B6B4";
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
	rename -uid "D7CDF3FD-4BF9-9815-A5A1-3494A71CF288";
createNode follicle -n "pSphere1FollicleShape9696" -p "pSphere1Follicle9696";
	rename -uid "11F56289-4A83-8784-D484-2793BE70040C";
	setAttr -k off ".v";
	setAttr ".pu" 0.96875;
	setAttr ".pv" 0.96875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve240" -p "pSphere1Follicle9696";
	rename -uid "3F5A1D31-4B2E-00B1-F27B-BBB7F120C280";
createNode nurbsCurve -n "curveShape240" -p "curve240";
	rename -uid "D8C89CF4-42F9-E414-70AF-7AA4FDEB130C";
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
	rename -uid "2A86997C-4A2C-4E23-96FE-8F857F225EAF";
	setAttr ".v" no;
createNode pfxHair -n "pfxHairShape1" -p "pfxHair1";
	rename -uid "DE235196-4F36-33D4-1B7D-7A82BF25FD88";
	setAttr -k off ".v";
	setAttr ".dpc" 100;
	setAttr ".dam" no;
createNode lightLinker -s -n "lightLinker1";
	rename -uid "D9EF1280-45C1-8D4C-3D53-1683A7512801";
	setAttr -s 4 ".lnk";
	setAttr -s 4 ".slnk";
createNode displayLayerManager -n "layerManager";
	rename -uid "1CF9EB90-45B9-9E0A-4442-D68BA78F0326";
createNode displayLayer -n "defaultLayer";
	rename -uid "3622410D-4D2B-0A72-8949-2392B7C12AFD";
createNode renderLayerManager -n "renderLayerManager";
	rename -uid "EF93446A-4ADF-79AC-FDDA-FDB6EF5EE685";
createNode renderLayer -n "defaultRenderLayer";
	rename -uid "FB0B969F-4430-6BE6-8FB0-46A94E455A2D";
	setAttr ".g" yes;
createNode aiOptions -s -n "defaultArnoldRenderOptions";
	rename -uid "D006CEAD-4C7E-F4FC-0989-7AAD7C7F9B6B";
	addAttr -ci true -sn "ARV_options" -ln "ARV_options" -dt "string";
createNode script -n "uiConfigurationScriptNode";
	rename -uid "2F2CEB64-4FDB-1E78-E45E-D19F61C20A8A";
	setAttr ".b" -type "string" (
		"// Maya Mel UI Configuration File.\n//\n//  This script is machine generated.  Edit at your own risk.\n//\n//\n\nglobal string $gMainPane;\nif (`paneLayout -exists $gMainPane`) {\n\n\tglobal int $gUseScenePanelConfig;\n\tint    $useSceneConfig = $gUseScenePanelConfig;\n\tint    $menusOkayInPanels = `optionVar -q allowMenusInPanels`;\tint    $nVisPanes = `paneLayout -q -nvp $gMainPane`;\n\tint    $nPanes = 0;\n\tstring $editorName;\n\tstring $panelName;\n\tstring $itemFilterName;\n\tstring $panelConfig;\n\n\t//\n\t//  get current state of the UI\n\t//\n\tsceneUIReplacement -update $gMainPane;\n\n\t$panelName = `sceneUIReplacement -getNextPanel \"modelPanel\" (localizedPanelLabel(\"Top View\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `modelPanel -unParent -l (localizedPanelLabel(\"Top View\")) -mbv $menusOkayInPanels `;\n\t\t\t$editorName = $panelName;\n            modelEditor -e \n                -camera \"top\" \n                -useInteractiveMode 0\n                -displayLights \"default\" \n                -displayAppearance \"wireframe\" \n"
		+ "                -activeOnly 0\n                -ignorePanZoom 0\n                -wireframeOnShaded 0\n                -headsUpDisplay 1\n                -holdOuts 1\n                -selectionHiliteDisplay 1\n                -useDefaultMaterial 0\n                -bufferMode \"double\" \n                -twoSidedLighting 1\n                -backfaceCulling 0\n                -xray 0\n                -jointXray 0\n                -activeComponentsXray 0\n                -displayTextures 0\n                -smoothWireframe 0\n                -lineWidth 1\n                -textureAnisotropic 0\n                -textureHilight 1\n                -textureSampling 2\n                -textureDisplay \"modulate\" \n                -textureMaxSize 16384\n                -fogging 0\n                -fogSource \"fragment\" \n                -fogMode \"linear\" \n                -fogStart 0\n                -fogEnd 100\n                -fogDensity 0.1\n                -fogColor 0.5 0.5 0.5 1 \n                -depthOfFieldPreview 1\n                -maxConstantTransparency 1\n"
		+ "                -rendererName \"base_OpenGL_Renderer\" \n                -objectFilterShowInHUD 1\n                -isFiltered 0\n                -colorResolution 256 256 \n                -bumpResolution 512 512 \n                -textureCompression 0\n                -transparencyAlgorithm \"frontAndBackCull\" \n                -transpInShadows 0\n                -cullingOverride \"none\" \n                -lowQualityLighting 0\n                -maximumNumHardwareLights 1\n                -occlusionCulling 0\n                -shadingModel 0\n                -useBaseRenderer 0\n                -useReducedRenderer 0\n                -smallObjectCulling 0\n                -smallObjectThreshold -1 \n                -interactiveDisableShadows 0\n                -interactiveBackFaceCull 0\n                -sortTransparent 1\n                -nurbsCurves 1\n                -nurbsSurfaces 1\n                -polymeshes 1\n                -subdivSurfaces 1\n                -planes 1\n                -lights 1\n                -cameras 1\n                -controlVertices 1\n"
		+ "                -hulls 1\n                -grid 1\n                -imagePlane 1\n                -joints 1\n                -ikHandles 1\n                -deformers 1\n                -dynamics 1\n                -particleInstancers 1\n                -fluids 1\n                -hairSystems 1\n                -follicles 1\n                -nCloths 1\n                -nParticles 1\n                -nRigids 1\n                -dynamicConstraints 1\n                -locators 1\n                -manipulators 1\n                -pluginShapes 1\n                -dimensions 1\n                -handles 1\n                -pivots 1\n                -textures 1\n                -strokes 1\n                -motionTrails 1\n                -clipGhosts 1\n                -greasePencils 1\n                -shadows 0\n                -captureSequenceNumber -1\n                -width 238\n                -height 332\n                -sceneRenderFilter 0\n                $editorName;\n            modelEditor -e -viewSelected 0 $editorName;\n            modelEditor -e \n"
		+ "                -pluginObjects \"gpuCacheDisplayFilter\" 1 \n                $editorName;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tmodelPanel -edit -l (localizedPanelLabel(\"Top View\")) -mbv $menusOkayInPanels  $panelName;\n\t\t$editorName = $panelName;\n        modelEditor -e \n            -camera \"top\" \n            -useInteractiveMode 0\n            -displayLights \"default\" \n            -displayAppearance \"wireframe\" \n            -activeOnly 0\n            -ignorePanZoom 0\n            -wireframeOnShaded 0\n            -headsUpDisplay 1\n            -holdOuts 1\n            -selectionHiliteDisplay 1\n            -useDefaultMaterial 0\n            -bufferMode \"double\" \n            -twoSidedLighting 1\n            -backfaceCulling 0\n            -xray 0\n            -jointXray 0\n            -activeComponentsXray 0\n            -displayTextures 0\n            -smoothWireframe 0\n            -lineWidth 1\n            -textureAnisotropic 0\n            -textureHilight 1\n            -textureSampling 2\n            -textureDisplay \"modulate\" \n"
		+ "            -textureMaxSize 16384\n            -fogging 0\n            -fogSource \"fragment\" \n            -fogMode \"linear\" \n            -fogStart 0\n            -fogEnd 100\n            -fogDensity 0.1\n            -fogColor 0.5 0.5 0.5 1 \n            -depthOfFieldPreview 1\n            -maxConstantTransparency 1\n            -rendererName \"base_OpenGL_Renderer\" \n            -objectFilterShowInHUD 1\n            -isFiltered 0\n            -colorResolution 256 256 \n            -bumpResolution 512 512 \n            -textureCompression 0\n            -transparencyAlgorithm \"frontAndBackCull\" \n            -transpInShadows 0\n            -cullingOverride \"none\" \n            -lowQualityLighting 0\n            -maximumNumHardwareLights 1\n            -occlusionCulling 0\n            -shadingModel 0\n            -useBaseRenderer 0\n            -useReducedRenderer 0\n            -smallObjectCulling 0\n            -smallObjectThreshold -1 \n            -interactiveDisableShadows 0\n            -interactiveBackFaceCull 0\n            -sortTransparent 1\n"
		+ "            -nurbsCurves 1\n            -nurbsSurfaces 1\n            -polymeshes 1\n            -subdivSurfaces 1\n            -planes 1\n            -lights 1\n            -cameras 1\n            -controlVertices 1\n            -hulls 1\n            -grid 1\n            -imagePlane 1\n            -joints 1\n            -ikHandles 1\n            -deformers 1\n            -dynamics 1\n            -particleInstancers 1\n            -fluids 1\n            -hairSystems 1\n            -follicles 1\n            -nCloths 1\n            -nParticles 1\n            -nRigids 1\n            -dynamicConstraints 1\n            -locators 1\n            -manipulators 1\n            -pluginShapes 1\n            -dimensions 1\n            -handles 1\n            -pivots 1\n            -textures 1\n            -strokes 1\n            -motionTrails 1\n            -clipGhosts 1\n            -greasePencils 1\n            -shadows 0\n            -captureSequenceNumber -1\n            -width 238\n            -height 332\n            -sceneRenderFilter 0\n            $editorName;\n"
		+ "        modelEditor -e -viewSelected 0 $editorName;\n        modelEditor -e \n            -pluginObjects \"gpuCacheDisplayFilter\" 1 \n            $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextPanel \"modelPanel\" (localizedPanelLabel(\"Side View\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `modelPanel -unParent -l (localizedPanelLabel(\"Side View\")) -mbv $menusOkayInPanels `;\n\t\t\t$editorName = $panelName;\n            modelEditor -e \n                -camera \"side\" \n                -useInteractiveMode 0\n                -displayLights \"default\" \n                -displayAppearance \"wireframe\" \n                -activeOnly 0\n                -ignorePanZoom 0\n                -wireframeOnShaded 0\n                -headsUpDisplay 1\n                -holdOuts 1\n                -selectionHiliteDisplay 1\n                -useDefaultMaterial 0\n                -bufferMode \"double\" \n                -twoSidedLighting 1\n                -backfaceCulling 0\n"
		+ "                -xray 0\n                -jointXray 0\n                -activeComponentsXray 0\n                -displayTextures 0\n                -smoothWireframe 0\n                -lineWidth 1\n                -textureAnisotropic 0\n                -textureHilight 1\n                -textureSampling 2\n                -textureDisplay \"modulate\" \n                -textureMaxSize 16384\n                -fogging 0\n                -fogSource \"fragment\" \n                -fogMode \"linear\" \n                -fogStart 0\n                -fogEnd 100\n                -fogDensity 0.1\n                -fogColor 0.5 0.5 0.5 1 \n                -depthOfFieldPreview 1\n                -maxConstantTransparency 1\n                -rendererName \"base_OpenGL_Renderer\" \n                -objectFilterShowInHUD 1\n                -isFiltered 0\n                -colorResolution 256 256 \n                -bumpResolution 512 512 \n                -textureCompression 0\n                -transparencyAlgorithm \"frontAndBackCull\" \n                -transpInShadows 0\n"
		+ "                -cullingOverride \"none\" \n                -lowQualityLighting 0\n                -maximumNumHardwareLights 1\n                -occlusionCulling 0\n                -shadingModel 0\n                -useBaseRenderer 0\n                -useReducedRenderer 0\n                -smallObjectCulling 0\n                -smallObjectThreshold -1 \n                -interactiveDisableShadows 0\n                -interactiveBackFaceCull 0\n                -sortTransparent 1\n                -nurbsCurves 1\n                -nurbsSurfaces 1\n                -polymeshes 1\n                -subdivSurfaces 1\n                -planes 1\n                -lights 1\n                -cameras 1\n                -controlVertices 1\n                -hulls 1\n                -grid 1\n                -imagePlane 1\n                -joints 1\n                -ikHandles 1\n                -deformers 1\n                -dynamics 1\n                -particleInstancers 1\n                -fluids 1\n                -hairSystems 1\n                -follicles 1\n                -nCloths 1\n"
		+ "                -nParticles 1\n                -nRigids 1\n                -dynamicConstraints 1\n                -locators 1\n                -manipulators 1\n                -pluginShapes 1\n                -dimensions 1\n                -handles 1\n                -pivots 1\n                -textures 1\n                -strokes 1\n                -motionTrails 1\n                -clipGhosts 1\n                -greasePencils 1\n                -shadows 0\n                -captureSequenceNumber -1\n                -width 238\n                -height 332\n                -sceneRenderFilter 0\n                $editorName;\n            modelEditor -e -viewSelected 0 $editorName;\n            modelEditor -e \n                -pluginObjects \"gpuCacheDisplayFilter\" 1 \n                $editorName;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tmodelPanel -edit -l (localizedPanelLabel(\"Side View\")) -mbv $menusOkayInPanels  $panelName;\n\t\t$editorName = $panelName;\n        modelEditor -e \n            -camera \"side\" \n            -useInteractiveMode 0\n"
		+ "            -displayLights \"default\" \n            -displayAppearance \"wireframe\" \n            -activeOnly 0\n            -ignorePanZoom 0\n            -wireframeOnShaded 0\n            -headsUpDisplay 1\n            -holdOuts 1\n            -selectionHiliteDisplay 1\n            -useDefaultMaterial 0\n            -bufferMode \"double\" \n            -twoSidedLighting 1\n            -backfaceCulling 0\n            -xray 0\n            -jointXray 0\n            -activeComponentsXray 0\n            -displayTextures 0\n            -smoothWireframe 0\n            -lineWidth 1\n            -textureAnisotropic 0\n            -textureHilight 1\n            -textureSampling 2\n            -textureDisplay \"modulate\" \n            -textureMaxSize 16384\n            -fogging 0\n            -fogSource \"fragment\" \n            -fogMode \"linear\" \n            -fogStart 0\n            -fogEnd 100\n            -fogDensity 0.1\n            -fogColor 0.5 0.5 0.5 1 \n            -depthOfFieldPreview 1\n            -maxConstantTransparency 1\n            -rendererName \"base_OpenGL_Renderer\" \n"
		+ "            -objectFilterShowInHUD 1\n            -isFiltered 0\n            -colorResolution 256 256 \n            -bumpResolution 512 512 \n            -textureCompression 0\n            -transparencyAlgorithm \"frontAndBackCull\" \n            -transpInShadows 0\n            -cullingOverride \"none\" \n            -lowQualityLighting 0\n            -maximumNumHardwareLights 1\n            -occlusionCulling 0\n            -shadingModel 0\n            -useBaseRenderer 0\n            -useReducedRenderer 0\n            -smallObjectCulling 0\n            -smallObjectThreshold -1 \n            -interactiveDisableShadows 0\n            -interactiveBackFaceCull 0\n            -sortTransparent 1\n            -nurbsCurves 1\n            -nurbsSurfaces 1\n            -polymeshes 1\n            -subdivSurfaces 1\n            -planes 1\n            -lights 1\n            -cameras 1\n            -controlVertices 1\n            -hulls 1\n            -grid 1\n            -imagePlane 1\n            -joints 1\n            -ikHandles 1\n            -deformers 1\n"
		+ "            -dynamics 1\n            -particleInstancers 1\n            -fluids 1\n            -hairSystems 1\n            -follicles 1\n            -nCloths 1\n            -nParticles 1\n            -nRigids 1\n            -dynamicConstraints 1\n            -locators 1\n            -manipulators 1\n            -pluginShapes 1\n            -dimensions 1\n            -handles 1\n            -pivots 1\n            -textures 1\n            -strokes 1\n            -motionTrails 1\n            -clipGhosts 1\n            -greasePencils 1\n            -shadows 0\n            -captureSequenceNumber -1\n            -width 238\n            -height 332\n            -sceneRenderFilter 0\n            $editorName;\n        modelEditor -e -viewSelected 0 $editorName;\n        modelEditor -e \n            -pluginObjects \"gpuCacheDisplayFilter\" 1 \n            $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextPanel \"modelPanel\" (localizedPanelLabel(\"Front View\")) `;\n\tif (\"\" == $panelName) {\n"
		+ "\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `modelPanel -unParent -l (localizedPanelLabel(\"Front View\")) -mbv $menusOkayInPanels `;\n\t\t\t$editorName = $panelName;\n            modelEditor -e \n                -camera \"front\" \n                -useInteractiveMode 0\n                -displayLights \"default\" \n                -displayAppearance \"wireframe\" \n                -activeOnly 0\n                -ignorePanZoom 0\n                -wireframeOnShaded 0\n                -headsUpDisplay 1\n                -holdOuts 1\n                -selectionHiliteDisplay 1\n                -useDefaultMaterial 0\n                -bufferMode \"double\" \n                -twoSidedLighting 1\n                -backfaceCulling 0\n                -xray 0\n                -jointXray 0\n                -activeComponentsXray 0\n                -displayTextures 0\n                -smoothWireframe 0\n                -lineWidth 1\n                -textureAnisotropic 0\n                -textureHilight 1\n                -textureSampling 2\n                -textureDisplay \"modulate\" \n"
		+ "                -textureMaxSize 16384\n                -fogging 0\n                -fogSource \"fragment\" \n                -fogMode \"linear\" \n                -fogStart 0\n                -fogEnd 100\n                -fogDensity 0.1\n                -fogColor 0.5 0.5 0.5 1 \n                -depthOfFieldPreview 1\n                -maxConstantTransparency 1\n                -rendererName \"base_OpenGL_Renderer\" \n                -objectFilterShowInHUD 1\n                -isFiltered 0\n                -colorResolution 256 256 \n                -bumpResolution 512 512 \n                -textureCompression 0\n                -transparencyAlgorithm \"frontAndBackCull\" \n                -transpInShadows 0\n                -cullingOverride \"none\" \n                -lowQualityLighting 0\n                -maximumNumHardwareLights 1\n                -occlusionCulling 0\n                -shadingModel 0\n                -useBaseRenderer 0\n                -useReducedRenderer 0\n                -smallObjectCulling 0\n                -smallObjectThreshold -1 \n"
		+ "                -interactiveDisableShadows 0\n                -interactiveBackFaceCull 0\n                -sortTransparent 1\n                -nurbsCurves 1\n                -nurbsSurfaces 1\n                -polymeshes 1\n                -subdivSurfaces 1\n                -planes 1\n                -lights 1\n                -cameras 1\n                -controlVertices 1\n                -hulls 1\n                -grid 1\n                -imagePlane 1\n                -joints 1\n                -ikHandles 1\n                -deformers 1\n                -dynamics 1\n                -particleInstancers 1\n                -fluids 1\n                -hairSystems 1\n                -follicles 1\n                -nCloths 1\n                -nParticles 1\n                -nRigids 1\n                -dynamicConstraints 1\n                -locators 1\n                -manipulators 1\n                -pluginShapes 1\n                -dimensions 1\n                -handles 1\n                -pivots 1\n                -textures 1\n                -strokes 1\n"
		+ "                -motionTrails 1\n                -clipGhosts 1\n                -greasePencils 1\n                -shadows 0\n                -captureSequenceNumber -1\n                -width 238\n                -height 332\n                -sceneRenderFilter 0\n                $editorName;\n            modelEditor -e -viewSelected 0 $editorName;\n            modelEditor -e \n                -pluginObjects \"gpuCacheDisplayFilter\" 1 \n                $editorName;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tmodelPanel -edit -l (localizedPanelLabel(\"Front View\")) -mbv $menusOkayInPanels  $panelName;\n\t\t$editorName = $panelName;\n        modelEditor -e \n            -camera \"front\" \n            -useInteractiveMode 0\n            -displayLights \"default\" \n            -displayAppearance \"wireframe\" \n            -activeOnly 0\n            -ignorePanZoom 0\n            -wireframeOnShaded 0\n            -headsUpDisplay 1\n            -holdOuts 1\n            -selectionHiliteDisplay 1\n            -useDefaultMaterial 0\n            -bufferMode \"double\" \n"
		+ "            -twoSidedLighting 1\n            -backfaceCulling 0\n            -xray 0\n            -jointXray 0\n            -activeComponentsXray 0\n            -displayTextures 0\n            -smoothWireframe 0\n            -lineWidth 1\n            -textureAnisotropic 0\n            -textureHilight 1\n            -textureSampling 2\n            -textureDisplay \"modulate\" \n            -textureMaxSize 16384\n            -fogging 0\n            -fogSource \"fragment\" \n            -fogMode \"linear\" \n            -fogStart 0\n            -fogEnd 100\n            -fogDensity 0.1\n            -fogColor 0.5 0.5 0.5 1 \n            -depthOfFieldPreview 1\n            -maxConstantTransparency 1\n            -rendererName \"base_OpenGL_Renderer\" \n            -objectFilterShowInHUD 1\n            -isFiltered 0\n            -colorResolution 256 256 \n            -bumpResolution 512 512 \n            -textureCompression 0\n            -transparencyAlgorithm \"frontAndBackCull\" \n            -transpInShadows 0\n            -cullingOverride \"none\" \n            -lowQualityLighting 0\n"
		+ "            -maximumNumHardwareLights 1\n            -occlusionCulling 0\n            -shadingModel 0\n            -useBaseRenderer 0\n            -useReducedRenderer 0\n            -smallObjectCulling 0\n            -smallObjectThreshold -1 \n            -interactiveDisableShadows 0\n            -interactiveBackFaceCull 0\n            -sortTransparent 1\n            -nurbsCurves 1\n            -nurbsSurfaces 1\n            -polymeshes 1\n            -subdivSurfaces 1\n            -planes 1\n            -lights 1\n            -cameras 1\n            -controlVertices 1\n            -hulls 1\n            -grid 1\n            -imagePlane 1\n            -joints 1\n            -ikHandles 1\n            -deformers 1\n            -dynamics 1\n            -particleInstancers 1\n            -fluids 1\n            -hairSystems 1\n            -follicles 1\n            -nCloths 1\n            -nParticles 1\n            -nRigids 1\n            -dynamicConstraints 1\n            -locators 1\n            -manipulators 1\n            -pluginShapes 1\n            -dimensions 1\n"
		+ "            -handles 1\n            -pivots 1\n            -textures 1\n            -strokes 1\n            -motionTrails 1\n            -clipGhosts 1\n            -greasePencils 1\n            -shadows 0\n            -captureSequenceNumber -1\n            -width 238\n            -height 332\n            -sceneRenderFilter 0\n            $editorName;\n        modelEditor -e -viewSelected 0 $editorName;\n        modelEditor -e \n            -pluginObjects \"gpuCacheDisplayFilter\" 1 \n            $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextPanel \"modelPanel\" (localizedPanelLabel(\"Persp View\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `modelPanel -unParent -l (localizedPanelLabel(\"Persp View\")) -mbv $menusOkayInPanels `;\n\t\t\t$editorName = $panelName;\n            modelEditor -e \n                -camera \"persp\" \n                -useInteractiveMode 0\n                -displayLights \"default\" \n                -displayAppearance \"wireframe\" \n"
		+ "                -activeOnly 0\n                -ignorePanZoom 0\n                -wireframeOnShaded 0\n                -headsUpDisplay 1\n                -holdOuts 1\n                -selectionHiliteDisplay 1\n                -useDefaultMaterial 0\n                -bufferMode \"double\" \n                -twoSidedLighting 1\n                -backfaceCulling 0\n                -xray 0\n                -jointXray 0\n                -activeComponentsXray 0\n                -displayTextures 0\n                -smoothWireframe 0\n                -lineWidth 1\n                -textureAnisotropic 0\n                -textureHilight 1\n                -textureSampling 2\n                -textureDisplay \"modulate\" \n                -textureMaxSize 16384\n                -fogging 0\n                -fogSource \"fragment\" \n                -fogMode \"linear\" \n                -fogStart 0\n                -fogEnd 100\n                -fogDensity 0.1\n                -fogColor 0.5 0.5 0.5 1 \n                -depthOfFieldPreview 1\n                -maxConstantTransparency 1\n"
		+ "                -rendererName \"base_OpenGL_Renderer\" \n                -objectFilterShowInHUD 1\n                -isFiltered 0\n                -colorResolution 256 256 \n                -bumpResolution 512 512 \n                -textureCompression 0\n                -transparencyAlgorithm \"frontAndBackCull\" \n                -transpInShadows 0\n                -cullingOverride \"none\" \n                -lowQualityLighting 0\n                -maximumNumHardwareLights 1\n                -occlusionCulling 0\n                -shadingModel 0\n                -useBaseRenderer 0\n                -useReducedRenderer 0\n                -smallObjectCulling 0\n                -smallObjectThreshold -1 \n                -interactiveDisableShadows 0\n                -interactiveBackFaceCull 0\n                -sortTransparent 1\n                -nurbsCurves 1\n                -nurbsSurfaces 1\n                -polymeshes 1\n                -subdivSurfaces 1\n                -planes 1\n                -lights 1\n                -cameras 1\n                -controlVertices 1\n"
		+ "                -hulls 1\n                -grid 1\n                -imagePlane 1\n                -joints 1\n                -ikHandles 1\n                -deformers 1\n                -dynamics 1\n                -particleInstancers 1\n                -fluids 1\n                -hairSystems 1\n                -follicles 1\n                -nCloths 1\n                -nParticles 1\n                -nRigids 1\n                -dynamicConstraints 1\n                -locators 1\n                -manipulators 1\n                -pluginShapes 1\n                -dimensions 1\n                -handles 1\n                -pivots 1\n                -textures 1\n                -strokes 1\n                -motionTrails 1\n                -clipGhosts 1\n                -greasePencils 1\n                -shadows 0\n                -captureSequenceNumber -1\n                -width 483\n                -height 709\n                -sceneRenderFilter 0\n                $editorName;\n            modelEditor -e -viewSelected 0 $editorName;\n            modelEditor -e \n"
		+ "                -pluginObjects \"gpuCacheDisplayFilter\" 1 \n                $editorName;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tmodelPanel -edit -l (localizedPanelLabel(\"Persp View\")) -mbv $menusOkayInPanels  $panelName;\n\t\t$editorName = $panelName;\n        modelEditor -e \n            -camera \"persp\" \n            -useInteractiveMode 0\n            -displayLights \"default\" \n            -displayAppearance \"wireframe\" \n            -activeOnly 0\n            -ignorePanZoom 0\n            -wireframeOnShaded 0\n            -headsUpDisplay 1\n            -holdOuts 1\n            -selectionHiliteDisplay 1\n            -useDefaultMaterial 0\n            -bufferMode \"double\" \n            -twoSidedLighting 1\n            -backfaceCulling 0\n            -xray 0\n            -jointXray 0\n            -activeComponentsXray 0\n            -displayTextures 0\n            -smoothWireframe 0\n            -lineWidth 1\n            -textureAnisotropic 0\n            -textureHilight 1\n            -textureSampling 2\n            -textureDisplay \"modulate\" \n"
		+ "            -textureMaxSize 16384\n            -fogging 0\n            -fogSource \"fragment\" \n            -fogMode \"linear\" \n            -fogStart 0\n            -fogEnd 100\n            -fogDensity 0.1\n            -fogColor 0.5 0.5 0.5 1 \n            -depthOfFieldPreview 1\n            -maxConstantTransparency 1\n            -rendererName \"base_OpenGL_Renderer\" \n            -objectFilterShowInHUD 1\n            -isFiltered 0\n            -colorResolution 256 256 \n            -bumpResolution 512 512 \n            -textureCompression 0\n            -transparencyAlgorithm \"frontAndBackCull\" \n            -transpInShadows 0\n            -cullingOverride \"none\" \n            -lowQualityLighting 0\n            -maximumNumHardwareLights 1\n            -occlusionCulling 0\n            -shadingModel 0\n            -useBaseRenderer 0\n            -useReducedRenderer 0\n            -smallObjectCulling 0\n            -smallObjectThreshold -1 \n            -interactiveDisableShadows 0\n            -interactiveBackFaceCull 0\n            -sortTransparent 1\n"
		+ "            -nurbsCurves 1\n            -nurbsSurfaces 1\n            -polymeshes 1\n            -subdivSurfaces 1\n            -planes 1\n            -lights 1\n            -cameras 1\n            -controlVertices 1\n            -hulls 1\n            -grid 1\n            -imagePlane 1\n            -joints 1\n            -ikHandles 1\n            -deformers 1\n            -dynamics 1\n            -particleInstancers 1\n            -fluids 1\n            -hairSystems 1\n            -follicles 1\n            -nCloths 1\n            -nParticles 1\n            -nRigids 1\n            -dynamicConstraints 1\n            -locators 1\n            -manipulators 1\n            -pluginShapes 1\n            -dimensions 1\n            -handles 1\n            -pivots 1\n            -textures 1\n            -strokes 1\n            -motionTrails 1\n            -clipGhosts 1\n            -greasePencils 1\n            -shadows 0\n            -captureSequenceNumber -1\n            -width 483\n            -height 709\n            -sceneRenderFilter 0\n            $editorName;\n"
		+ "        modelEditor -e -viewSelected 0 $editorName;\n        modelEditor -e \n            -pluginObjects \"gpuCacheDisplayFilter\" 1 \n            $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextPanel \"outlinerPanel\" (localizedPanelLabel(\"Outliner\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `outlinerPanel -unParent -l (localizedPanelLabel(\"Outliner\")) -mbv $menusOkayInPanels `;\n\t\t\t$editorName = $panelName;\n            outlinerEditor -e \n                -docTag \"isolOutln_fromSeln\" \n                -showShapes 1\n                -showAssignedMaterials 0\n                -showTimeEditor 1\n                -showReferenceNodes 1\n                -showReferenceMembers 1\n                -showAttributes 0\n                -showConnected 0\n                -showAnimCurvesOnly 0\n                -showMuteInfo 0\n                -organizeByLayer 1\n                -showAnimLayerWeight 1\n                -autoExpandLayers 1\n                -autoExpand 0\n"
		+ "                -showDagOnly 0\n                -showAssets 1\n                -showContainedOnly 1\n                -showPublishedAsConnected 0\n                -showContainerContents 1\n                -ignoreDagHierarchy 0\n                -expandConnections 0\n                -showUpstreamCurves 1\n                -showUnitlessCurves 1\n                -showCompounds 1\n                -showLeafs 1\n                -showNumericAttrsOnly 0\n                -highlightActive 1\n                -autoSelectNewObjects 0\n                -doNotSelectNewObjects 0\n                -dropIsParent 1\n                -transmitFilters 0\n                -setFilter \"defaultSetFilter\" \n                -showSetMembers 1\n                -allowMultiSelection 1\n                -alwaysToggleSelect 0\n                -directSelect 0\n                -displayMode \"DAG\" \n                -expandObjects 0\n                -setsIgnoreFilters 1\n                -containersIgnoreFilters 0\n                -editAttrName 0\n                -showAttrValues 0\n                -highlightSecondary 0\n"
		+ "                -showUVAttrsOnly 0\n                -showTextureNodesOnly 0\n                -attrAlphaOrder \"default\" \n                -animLayerFilterOptions \"allAffecting\" \n                -sortOrder \"none\" \n                -longNames 0\n                -niceNames 1\n                -showNamespace 1\n                -showPinIcons 0\n                -mapMotionTrails 0\n                -ignoreHiddenAttribute 0\n                -ignoreOutlinerColor 0\n                -renderFilterVisible 0\n                $editorName;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\toutlinerPanel -edit -l (localizedPanelLabel(\"Outliner\")) -mbv $menusOkayInPanels  $panelName;\n\t\t$editorName = $panelName;\n        outlinerEditor -e \n            -docTag \"isolOutln_fromSeln\" \n            -showShapes 1\n            -showAssignedMaterials 0\n            -showTimeEditor 1\n            -showReferenceNodes 1\n            -showReferenceMembers 1\n            -showAttributes 0\n            -showConnected 0\n            -showAnimCurvesOnly 0\n            -showMuteInfo 0\n"
		+ "            -organizeByLayer 1\n            -showAnimLayerWeight 1\n            -autoExpandLayers 1\n            -autoExpand 0\n            -showDagOnly 0\n            -showAssets 1\n            -showContainedOnly 1\n            -showPublishedAsConnected 0\n            -showContainerContents 1\n            -ignoreDagHierarchy 0\n            -expandConnections 0\n            -showUpstreamCurves 1\n            -showUnitlessCurves 1\n            -showCompounds 1\n            -showLeafs 1\n            -showNumericAttrsOnly 0\n            -highlightActive 1\n            -autoSelectNewObjects 0\n            -doNotSelectNewObjects 0\n            -dropIsParent 1\n            -transmitFilters 0\n            -setFilter \"defaultSetFilter\" \n            -showSetMembers 1\n            -allowMultiSelection 1\n            -alwaysToggleSelect 0\n            -directSelect 0\n            -displayMode \"DAG\" \n            -expandObjects 0\n            -setsIgnoreFilters 1\n            -containersIgnoreFilters 0\n            -editAttrName 0\n            -showAttrValues 0\n"
		+ "            -highlightSecondary 0\n            -showUVAttrsOnly 0\n            -showTextureNodesOnly 0\n            -attrAlphaOrder \"default\" \n            -animLayerFilterOptions \"allAffecting\" \n            -sortOrder \"none\" \n            -longNames 0\n            -niceNames 1\n            -showNamespace 1\n            -showPinIcons 0\n            -mapMotionTrails 0\n            -ignoreHiddenAttribute 0\n            -ignoreOutlinerColor 0\n            -renderFilterVisible 0\n            $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"graphEditor\" (localizedPanelLabel(\"Graph Editor\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"graphEditor\" -l (localizedPanelLabel(\"Graph Editor\")) -mbv $menusOkayInPanels `;\n\n\t\t\t$editorName = ($panelName+\"OutlineEd\");\n            outlinerEditor -e \n                -showShapes 1\n                -showAssignedMaterials 0\n                -showTimeEditor 1\n"
		+ "                -showReferenceNodes 0\n                -showReferenceMembers 0\n                -showAttributes 1\n                -showConnected 1\n                -showAnimCurvesOnly 1\n                -showMuteInfo 0\n                -organizeByLayer 1\n                -showAnimLayerWeight 1\n                -autoExpandLayers 1\n                -autoExpand 1\n                -showDagOnly 0\n                -showAssets 1\n                -showContainedOnly 0\n                -showPublishedAsConnected 0\n                -showContainerContents 0\n                -ignoreDagHierarchy 0\n                -expandConnections 1\n                -showUpstreamCurves 1\n                -showUnitlessCurves 1\n                -showCompounds 0\n                -showLeafs 1\n                -showNumericAttrsOnly 1\n                -highlightActive 0\n                -autoSelectNewObjects 1\n                -doNotSelectNewObjects 0\n                -dropIsParent 1\n                -transmitFilters 1\n                -setFilter \"0\" \n                -showSetMembers 0\n"
		+ "                -allowMultiSelection 1\n                -alwaysToggleSelect 0\n                -directSelect 0\n                -displayMode \"DAG\" \n                -expandObjects 0\n                -setsIgnoreFilters 1\n                -containersIgnoreFilters 0\n                -editAttrName 0\n                -showAttrValues 0\n                -highlightSecondary 0\n                -showUVAttrsOnly 0\n                -showTextureNodesOnly 0\n                -attrAlphaOrder \"default\" \n                -animLayerFilterOptions \"allAffecting\" \n                -sortOrder \"none\" \n                -longNames 0\n                -niceNames 1\n                -showNamespace 1\n                -showPinIcons 1\n                -mapMotionTrails 1\n                -ignoreHiddenAttribute 0\n                -ignoreOutlinerColor 0\n                -renderFilterVisible 0\n                $editorName;\n\n\t\t\t$editorName = ($panelName+\"GraphEd\");\n            animCurveEditor -e \n                -displayKeys 1\n                -displayTangents 0\n                -displayActiveKeys 0\n"
		+ "                -displayActiveKeyTangents 1\n                -displayInfinities 0\n                -displayValues 0\n                -autoFit 1\n                -snapTime \"integer\" \n                -snapValue \"none\" \n                -showResults \"off\" \n                -showBufferCurves \"off\" \n                -smoothness \"fine\" \n                -resultSamples 1\n                -resultScreenSamples 0\n                -resultUpdate \"delayed\" \n                -showUpstreamCurves 1\n                -showCurveNames 0\n                -showActiveCurveNames 0\n                -stackedCurves 0\n                -stackedCurvesMin -1\n                -stackedCurvesMax 1\n                -stackedCurvesSpace 0.2\n                -displayNormalized 0\n                -preSelectionHighlight 0\n                -constrainDrag 0\n                -classicMode 1\n                -valueLinesToggle 1\n                $editorName;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Graph Editor\")) -mbv $menusOkayInPanels  $panelName;\n"
		+ "\n\t\t\t$editorName = ($panelName+\"OutlineEd\");\n            outlinerEditor -e \n                -showShapes 1\n                -showAssignedMaterials 0\n                -showTimeEditor 1\n                -showReferenceNodes 0\n                -showReferenceMembers 0\n                -showAttributes 1\n                -showConnected 1\n                -showAnimCurvesOnly 1\n                -showMuteInfo 0\n                -organizeByLayer 1\n                -showAnimLayerWeight 1\n                -autoExpandLayers 1\n                -autoExpand 1\n                -showDagOnly 0\n                -showAssets 1\n                -showContainedOnly 0\n                -showPublishedAsConnected 0\n                -showContainerContents 0\n                -ignoreDagHierarchy 0\n                -expandConnections 1\n                -showUpstreamCurves 1\n                -showUnitlessCurves 1\n                -showCompounds 0\n                -showLeafs 1\n                -showNumericAttrsOnly 1\n                -highlightActive 0\n                -autoSelectNewObjects 1\n"
		+ "                -doNotSelectNewObjects 0\n                -dropIsParent 1\n                -transmitFilters 1\n                -setFilter \"0\" \n                -showSetMembers 0\n                -allowMultiSelection 1\n                -alwaysToggleSelect 0\n                -directSelect 0\n                -displayMode \"DAG\" \n                -expandObjects 0\n                -setsIgnoreFilters 1\n                -containersIgnoreFilters 0\n                -editAttrName 0\n                -showAttrValues 0\n                -highlightSecondary 0\n                -showUVAttrsOnly 0\n                -showTextureNodesOnly 0\n                -attrAlphaOrder \"default\" \n                -animLayerFilterOptions \"allAffecting\" \n                -sortOrder \"none\" \n                -longNames 0\n                -niceNames 1\n                -showNamespace 1\n                -showPinIcons 1\n                -mapMotionTrails 1\n                -ignoreHiddenAttribute 0\n                -ignoreOutlinerColor 0\n                -renderFilterVisible 0\n                $editorName;\n"
		+ "\n\t\t\t$editorName = ($panelName+\"GraphEd\");\n            animCurveEditor -e \n                -displayKeys 1\n                -displayTangents 0\n                -displayActiveKeys 0\n                -displayActiveKeyTangents 1\n                -displayInfinities 0\n                -displayValues 0\n                -autoFit 1\n                -snapTime \"integer\" \n                -snapValue \"none\" \n                -showResults \"off\" \n                -showBufferCurves \"off\" \n                -smoothness \"fine\" \n                -resultSamples 1\n                -resultScreenSamples 0\n                -resultUpdate \"delayed\" \n                -showUpstreamCurves 1\n                -showCurveNames 0\n                -showActiveCurveNames 0\n                -stackedCurves 0\n                -stackedCurvesMin -1\n                -stackedCurvesMax 1\n                -stackedCurvesSpace 0.2\n                -displayNormalized 0\n                -preSelectionHighlight 0\n                -constrainDrag 0\n                -classicMode 1\n                -valueLinesToggle 1\n"
		+ "                $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"dopeSheetPanel\" (localizedPanelLabel(\"Dope Sheet\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"dopeSheetPanel\" -l (localizedPanelLabel(\"Dope Sheet\")) -mbv $menusOkayInPanels `;\n\n\t\t\t$editorName = ($panelName+\"OutlineEd\");\n            outlinerEditor -e \n                -showShapes 1\n                -showAssignedMaterials 0\n                -showTimeEditor 1\n                -showReferenceNodes 0\n                -showReferenceMembers 0\n                -showAttributes 1\n                -showConnected 1\n                -showAnimCurvesOnly 1\n                -showMuteInfo 0\n                -organizeByLayer 1\n                -showAnimLayerWeight 1\n                -autoExpandLayers 1\n                -autoExpand 0\n                -showDagOnly 0\n                -showAssets 1\n                -showContainedOnly 0\n                -showPublishedAsConnected 0\n"
		+ "                -showContainerContents 0\n                -ignoreDagHierarchy 0\n                -expandConnections 1\n                -showUpstreamCurves 1\n                -showUnitlessCurves 0\n                -showCompounds 1\n                -showLeafs 1\n                -showNumericAttrsOnly 1\n                -highlightActive 0\n                -autoSelectNewObjects 0\n                -doNotSelectNewObjects 1\n                -dropIsParent 1\n                -transmitFilters 0\n                -setFilter \"0\" \n                -showSetMembers 0\n                -allowMultiSelection 1\n                -alwaysToggleSelect 0\n                -directSelect 0\n                -displayMode \"DAG\" \n                -expandObjects 0\n                -setsIgnoreFilters 1\n                -containersIgnoreFilters 0\n                -editAttrName 0\n                -showAttrValues 0\n                -highlightSecondary 0\n                -showUVAttrsOnly 0\n                -showTextureNodesOnly 0\n                -attrAlphaOrder \"default\" \n                -animLayerFilterOptions \"allAffecting\" \n"
		+ "                -sortOrder \"none\" \n                -longNames 0\n                -niceNames 1\n                -showNamespace 1\n                -showPinIcons 0\n                -mapMotionTrails 1\n                -ignoreHiddenAttribute 0\n                -ignoreOutlinerColor 0\n                -renderFilterVisible 0\n                $editorName;\n\n\t\t\t$editorName = ($panelName+\"DopeSheetEd\");\n            dopeSheetEditor -e \n                -displayKeys 1\n                -displayTangents 0\n                -displayActiveKeys 0\n                -displayActiveKeyTangents 0\n                -displayInfinities 0\n                -displayValues 0\n                -autoFit 0\n                -snapTime \"integer\" \n                -snapValue \"none\" \n                -outliner \"dopeSheetPanel1OutlineEd\" \n                -showSummary 1\n                -showScene 0\n                -hierarchyBelow 0\n                -showTicks 1\n                -selectionWindow 0 0 0 0 \n                $editorName;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n"
		+ "\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Dope Sheet\")) -mbv $menusOkayInPanels  $panelName;\n\n\t\t\t$editorName = ($panelName+\"OutlineEd\");\n            outlinerEditor -e \n                -showShapes 1\n                -showAssignedMaterials 0\n                -showTimeEditor 1\n                -showReferenceNodes 0\n                -showReferenceMembers 0\n                -showAttributes 1\n                -showConnected 1\n                -showAnimCurvesOnly 1\n                -showMuteInfo 0\n                -organizeByLayer 1\n                -showAnimLayerWeight 1\n                -autoExpandLayers 1\n                -autoExpand 0\n                -showDagOnly 0\n                -showAssets 1\n                -showContainedOnly 0\n                -showPublishedAsConnected 0\n                -showContainerContents 0\n                -ignoreDagHierarchy 0\n                -expandConnections 1\n                -showUpstreamCurves 1\n                -showUnitlessCurves 0\n                -showCompounds 1\n                -showLeafs 1\n"
		+ "                -showNumericAttrsOnly 1\n                -highlightActive 0\n                -autoSelectNewObjects 0\n                -doNotSelectNewObjects 1\n                -dropIsParent 1\n                -transmitFilters 0\n                -setFilter \"0\" \n                -showSetMembers 0\n                -allowMultiSelection 1\n                -alwaysToggleSelect 0\n                -directSelect 0\n                -displayMode \"DAG\" \n                -expandObjects 0\n                -setsIgnoreFilters 1\n                -containersIgnoreFilters 0\n                -editAttrName 0\n                -showAttrValues 0\n                -highlightSecondary 0\n                -showUVAttrsOnly 0\n                -showTextureNodesOnly 0\n                -attrAlphaOrder \"default\" \n                -animLayerFilterOptions \"allAffecting\" \n                -sortOrder \"none\" \n                -longNames 0\n                -niceNames 1\n                -showNamespace 1\n                -showPinIcons 0\n                -mapMotionTrails 1\n                -ignoreHiddenAttribute 0\n"
		+ "                -ignoreOutlinerColor 0\n                -renderFilterVisible 0\n                $editorName;\n\n\t\t\t$editorName = ($panelName+\"DopeSheetEd\");\n            dopeSheetEditor -e \n                -displayKeys 1\n                -displayTangents 0\n                -displayActiveKeys 0\n                -displayActiveKeyTangents 0\n                -displayInfinities 0\n                -displayValues 0\n                -autoFit 0\n                -snapTime \"integer\" \n                -snapValue \"none\" \n                -outliner \"dopeSheetPanel1OutlineEd\" \n                -showSummary 1\n                -showScene 0\n                -hierarchyBelow 0\n                -showTicks 1\n                -selectionWindow 0 0 0 0 \n                $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"clipEditorPanel\" (localizedPanelLabel(\"Trax Editor\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"clipEditorPanel\" -l (localizedPanelLabel(\"Trax Editor\")) -mbv $menusOkayInPanels `;\n"
		+ "\n\t\t\t$editorName = clipEditorNameFromPanel($panelName);\n            clipEditor -e \n                -displayKeys 0\n                -displayTangents 0\n                -displayActiveKeys 0\n                -displayActiveKeyTangents 0\n                -displayInfinities 0\n                -displayValues 0\n                -autoFit 0\n                -snapTime \"none\" \n                -snapValue \"none\" \n                -initialized 0\n                -manageSequencer 0 \n                $editorName;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Trax Editor\")) -mbv $menusOkayInPanels  $panelName;\n\n\t\t\t$editorName = clipEditorNameFromPanel($panelName);\n            clipEditor -e \n                -displayKeys 0\n                -displayTangents 0\n                -displayActiveKeys 0\n                -displayActiveKeyTangents 0\n                -displayInfinities 0\n                -displayValues 0\n                -autoFit 0\n                -snapTime \"none\" \n                -snapValue \"none\" \n"
		+ "                -initialized 0\n                -manageSequencer 0 \n                $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"sequenceEditorPanel\" (localizedPanelLabel(\"Camera Sequencer\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"sequenceEditorPanel\" -l (localizedPanelLabel(\"Camera Sequencer\")) -mbv $menusOkayInPanels `;\n\n\t\t\t$editorName = sequenceEditorNameFromPanel($panelName);\n            clipEditor -e \n                -displayKeys 0\n                -displayTangents 0\n                -displayActiveKeys 0\n                -displayActiveKeyTangents 0\n                -displayInfinities 0\n                -displayValues 0\n                -autoFit 0\n                -snapTime \"none\" \n                -snapValue \"none\" \n                -initialized 0\n                -manageSequencer 1 \n                $editorName;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n"
		+ "\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Camera Sequencer\")) -mbv $menusOkayInPanels  $panelName;\n\n\t\t\t$editorName = sequenceEditorNameFromPanel($panelName);\n            clipEditor -e \n                -displayKeys 0\n                -displayTangents 0\n                -displayActiveKeys 0\n                -displayActiveKeyTangents 0\n                -displayInfinities 0\n                -displayValues 0\n                -autoFit 0\n                -snapTime \"none\" \n                -snapValue \"none\" \n                -initialized 0\n                -manageSequencer 1 \n                $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"hyperGraphPanel\" (localizedPanelLabel(\"Hypergraph Hierarchy\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"hyperGraphPanel\" -l (localizedPanelLabel(\"Hypergraph Hierarchy\")) -mbv $menusOkayInPanels `;\n\n\t\t\t$editorName = ($panelName+\"HyperGraphEd\");\n"
		+ "            hyperGraph -e \n                -graphLayoutStyle \"hierarchicalLayout\" \n                -orientation \"horiz\" \n                -mergeConnections 0\n                -zoom 1\n                -animateTransition 0\n                -showRelationships 1\n                -showShapes 0\n                -showDeformers 0\n                -showExpressions 0\n                -showConstraints 0\n                -showConnectionFromSelected 0\n                -showConnectionToSelected 0\n                -showConstraintLabels 0\n                -showUnderworld 0\n                -showInvisible 0\n                -transitionFrames 1\n                -opaqueContainers 0\n                -freeform 0\n                -imagePosition 0 0 \n                -imageScale 1\n                -imageEnabled 0\n                -graphType \"DAG\" \n                -heatMapDisplay 0\n                -updateSelection 1\n                -updateNodeAdded 1\n                -useDrawOverrideColor 0\n                -limitGraphTraversal -1\n                -range 0 0 \n"
		+ "                -iconSize \"smallIcons\" \n                -showCachedConnections 0\n                $editorName;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Hypergraph Hierarchy\")) -mbv $menusOkayInPanels  $panelName;\n\n\t\t\t$editorName = ($panelName+\"HyperGraphEd\");\n            hyperGraph -e \n                -graphLayoutStyle \"hierarchicalLayout\" \n                -orientation \"horiz\" \n                -mergeConnections 0\n                -zoom 1\n                -animateTransition 0\n                -showRelationships 1\n                -showShapes 0\n                -showDeformers 0\n                -showExpressions 0\n                -showConstraints 0\n                -showConnectionFromSelected 0\n                -showConnectionToSelected 0\n                -showConstraintLabels 0\n                -showUnderworld 0\n                -showInvisible 0\n                -transitionFrames 1\n                -opaqueContainers 0\n                -freeform 0\n                -imagePosition 0 0 \n"
		+ "                -imageScale 1\n                -imageEnabled 0\n                -graphType \"DAG\" \n                -heatMapDisplay 0\n                -updateSelection 1\n                -updateNodeAdded 1\n                -useDrawOverrideColor 0\n                -limitGraphTraversal -1\n                -range 0 0 \n                -iconSize \"smallIcons\" \n                -showCachedConnections 0\n                $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"visorPanel\" (localizedPanelLabel(\"Visor\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"visorPanel\" -l (localizedPanelLabel(\"Visor\")) -mbv $menusOkayInPanels `;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Visor\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"polyTexturePlacementPanel\" (localizedPanelLabel(\"UV Editor\")) `;\n"
		+ "\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"polyTexturePlacementPanel\" -l (localizedPanelLabel(\"UV Editor\")) -mbv $menusOkayInPanels `;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"UV Editor\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"renderWindowPanel\" (localizedPanelLabel(\"Render View\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"renderWindowPanel\" -l (localizedPanelLabel(\"Render View\")) -mbv $menusOkayInPanels `;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Render View\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"dynRelEdPanel\" (localizedPanelLabel(\"Dynamic Relationships\")) `;\n"
		+ "\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"dynRelEdPanel\" -l (localizedPanelLabel(\"Dynamic Relationships\")) -mbv $menusOkayInPanels `;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Dynamic Relationships\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"relationshipPanel\" (localizedPanelLabel(\"Relationship Editor\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"relationshipPanel\" -l (localizedPanelLabel(\"Relationship Editor\")) -mbv $menusOkayInPanels `;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Relationship Editor\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"referenceEditorPanel\" (localizedPanelLabel(\"Reference Editor\")) `;\n"
		+ "\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"referenceEditorPanel\" -l (localizedPanelLabel(\"Reference Editor\")) -mbv $menusOkayInPanels `;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Reference Editor\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"componentEditorPanel\" (localizedPanelLabel(\"Component Editor\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"componentEditorPanel\" -l (localizedPanelLabel(\"Component Editor\")) -mbv $menusOkayInPanels `;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Component Editor\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"dynPaintScriptedPanelType\" (localizedPanelLabel(\"Paint Effects\")) `;\n"
		+ "\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"dynPaintScriptedPanelType\" -l (localizedPanelLabel(\"Paint Effects\")) -mbv $menusOkayInPanels `;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Paint Effects\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"scriptEditorPanel\" (localizedPanelLabel(\"Script Editor\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"scriptEditorPanel\" -l (localizedPanelLabel(\"Script Editor\")) -mbv $menusOkayInPanels `;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Script Editor\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"contentBrowserPanel\" (localizedPanelLabel(\"Content Browser\")) `;\n"
		+ "\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"contentBrowserPanel\" -l (localizedPanelLabel(\"Content Browser\")) -mbv $menusOkayInPanels `;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Content Browser\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"Stereo\" (localizedPanelLabel(\"Stereo\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"Stereo\" -l (localizedPanelLabel(\"Stereo\")) -mbv $menusOkayInPanels `;\nstring $editorName = ($panelName+\"Editor\");\n            stereoCameraView -e \n                -camera \"persp\" \n                -useInteractiveMode 0\n                -displayLights \"default\" \n                -displayAppearance \"wireframe\" \n                -activeOnly 0\n                -ignorePanZoom 0\n                -wireframeOnShaded 0\n"
		+ "                -headsUpDisplay 1\n                -holdOuts 1\n                -selectionHiliteDisplay 1\n                -useDefaultMaterial 0\n                -bufferMode \"double\" \n                -twoSidedLighting 1\n                -backfaceCulling 0\n                -xray 0\n                -jointXray 0\n                -activeComponentsXray 0\n                -displayTextures 0\n                -smoothWireframe 0\n                -lineWidth 1\n                -textureAnisotropic 0\n                -textureHilight 1\n                -textureSampling 2\n                -textureDisplay \"modulate\" \n                -textureMaxSize 16384\n                -fogging 0\n                -fogSource \"fragment\" \n                -fogMode \"linear\" \n                -fogStart 0\n                -fogEnd 100\n                -fogDensity 0.1\n                -fogColor 0.5 0.5 0.5 1 \n                -depthOfFieldPreview 1\n                -maxConstantTransparency 1\n                -objectFilterShowInHUD 1\n                -isFiltered 0\n                -colorResolution 4 4 \n"
		+ "                -bumpResolution 4 4 \n                -textureCompression 0\n                -transparencyAlgorithm \"frontAndBackCull\" \n                -transpInShadows 0\n                -cullingOverride \"none\" \n                -lowQualityLighting 0\n                -maximumNumHardwareLights 0\n                -occlusionCulling 0\n                -shadingModel 0\n                -useBaseRenderer 0\n                -useReducedRenderer 0\n                -smallObjectCulling 0\n                -smallObjectThreshold -1 \n                -interactiveDisableShadows 0\n                -interactiveBackFaceCull 0\n                -sortTransparent 1\n                -nurbsCurves 1\n                -nurbsSurfaces 1\n                -polymeshes 1\n                -subdivSurfaces 1\n                -planes 1\n                -lights 1\n                -cameras 1\n                -controlVertices 1\n                -hulls 1\n                -grid 1\n                -imagePlane 1\n                -joints 1\n                -ikHandles 1\n                -deformers 1\n"
		+ "                -dynamics 1\n                -particleInstancers 1\n                -fluids 1\n                -hairSystems 1\n                -follicles 1\n                -nCloths 1\n                -nParticles 1\n                -nRigids 1\n                -dynamicConstraints 1\n                -locators 1\n                -manipulators 1\n                -pluginShapes 1\n                -dimensions 1\n                -handles 1\n                -pivots 1\n                -textures 1\n                -strokes 1\n                -motionTrails 1\n                -clipGhosts 1\n                -greasePencils 1\n                -shadows 0\n                -captureSequenceNumber -1\n                -width 0\n                -height 0\n                -sceneRenderFilter 0\n                -displayMode \"centerEye\" \n                -viewColor 0 0 0 1 \n                -useCustomBackground 1\n                $editorName;\n            stereoCameraView -e -viewSelected 0 $editorName;\n            stereoCameraView -e \n                -pluginObjects \"gpuCacheDisplayFilter\" 1 \n"
		+ "                $editorName;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Stereo\")) -mbv $menusOkayInPanels  $panelName;\nstring $editorName = ($panelName+\"Editor\");\n            stereoCameraView -e \n                -camera \"persp\" \n                -useInteractiveMode 0\n                -displayLights \"default\" \n                -displayAppearance \"wireframe\" \n                -activeOnly 0\n                -ignorePanZoom 0\n                -wireframeOnShaded 0\n                -headsUpDisplay 1\n                -holdOuts 1\n                -selectionHiliteDisplay 1\n                -useDefaultMaterial 0\n                -bufferMode \"double\" \n                -twoSidedLighting 1\n                -backfaceCulling 0\n                -xray 0\n                -jointXray 0\n                -activeComponentsXray 0\n                -displayTextures 0\n                -smoothWireframe 0\n                -lineWidth 1\n                -textureAnisotropic 0\n                -textureHilight 1\n"
		+ "                -textureSampling 2\n                -textureDisplay \"modulate\" \n                -textureMaxSize 16384\n                -fogging 0\n                -fogSource \"fragment\" \n                -fogMode \"linear\" \n                -fogStart 0\n                -fogEnd 100\n                -fogDensity 0.1\n                -fogColor 0.5 0.5 0.5 1 \n                -depthOfFieldPreview 1\n                -maxConstantTransparency 1\n                -objectFilterShowInHUD 1\n                -isFiltered 0\n                -colorResolution 4 4 \n                -bumpResolution 4 4 \n                -textureCompression 0\n                -transparencyAlgorithm \"frontAndBackCull\" \n                -transpInShadows 0\n                -cullingOverride \"none\" \n                -lowQualityLighting 0\n                -maximumNumHardwareLights 0\n                -occlusionCulling 0\n                -shadingModel 0\n                -useBaseRenderer 0\n                -useReducedRenderer 0\n                -smallObjectCulling 0\n                -smallObjectThreshold -1 \n"
		+ "                -interactiveDisableShadows 0\n                -interactiveBackFaceCull 0\n                -sortTransparent 1\n                -nurbsCurves 1\n                -nurbsSurfaces 1\n                -polymeshes 1\n                -subdivSurfaces 1\n                -planes 1\n                -lights 1\n                -cameras 1\n                -controlVertices 1\n                -hulls 1\n                -grid 1\n                -imagePlane 1\n                -joints 1\n                -ikHandles 1\n                -deformers 1\n                -dynamics 1\n                -particleInstancers 1\n                -fluids 1\n                -hairSystems 1\n                -follicles 1\n                -nCloths 1\n                -nParticles 1\n                -nRigids 1\n                -dynamicConstraints 1\n                -locators 1\n                -manipulators 1\n                -pluginShapes 1\n                -dimensions 1\n                -handles 1\n                -pivots 1\n                -textures 1\n                -strokes 1\n"
		+ "                -motionTrails 1\n                -clipGhosts 1\n                -greasePencils 1\n                -shadows 0\n                -captureSequenceNumber -1\n                -width 0\n                -height 0\n                -sceneRenderFilter 0\n                -displayMode \"centerEye\" \n                -viewColor 0 0 0 1 \n                -useCustomBackground 1\n                $editorName;\n            stereoCameraView -e -viewSelected 0 $editorName;\n            stereoCameraView -e \n                -pluginObjects \"gpuCacheDisplayFilter\" 1 \n                $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextPanel \"blendShapePanel\" (localizedPanelLabel(\"Blend Shape\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\tblendShapePanel -unParent -l (localizedPanelLabel(\"Blend Shape\")) -mbv $menusOkayInPanels ;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tblendShapePanel -edit -l (localizedPanelLabel(\"Blend Shape\")) -mbv $menusOkayInPanels  $panelName;\n"
		+ "\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"hyperShadePanel\" (localizedPanelLabel(\"Hypershade\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"hyperShadePanel\" -l (localizedPanelLabel(\"Hypershade\")) -mbv $menusOkayInPanels `;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Hypershade\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"nodeEditorPanel\" (localizedPanelLabel(\"Node Editor\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"nodeEditorPanel\" -l (localizedPanelLabel(\"Node Editor\")) -mbv $menusOkayInPanels `;\n\n\t\t\t$editorName = ($panelName+\"NodeEditorEd\");\n            nodeEditor -e \n                -allAttributes 0\n                -allNodes 0\n                -autoSizeNodes 1\n"
		+ "                -consistentNameSize 1\n                -createNodeCommand \"nodeEdCreateNodeCommand\" \n                -defaultPinnedState 0\n                -additiveGraphingMode 0\n                -settingsChangedCallback \"nodeEdSyncControls\" \n                -traversalDepthLimit -1\n                -keyPressCommand \"nodeEdKeyPressCommand\" \n                -nodeTitleMode \"name\" \n                -gridSnap 0\n                -gridVisibility 1\n                -popupMenuScript \"nodeEdBuildPanelMenus\" \n                -showNamespace 1\n                -showShapes 1\n                -showSGShapes 0\n                -showTransforms 1\n                -useAssets 1\n                -syncedSelection 1\n                -extendToShapes 1\n                -activeTab -1\n                -editorMode \"default\" \n                $editorName;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Node Editor\")) -mbv $menusOkayInPanels  $panelName;\n\n\t\t\t$editorName = ($panelName+\"NodeEditorEd\");\n            nodeEditor -e \n"
		+ "                -allAttributes 0\n                -allNodes 0\n                -autoSizeNodes 1\n                -consistentNameSize 1\n                -createNodeCommand \"nodeEdCreateNodeCommand\" \n                -defaultPinnedState 0\n                -additiveGraphingMode 0\n                -settingsChangedCallback \"nodeEdSyncControls\" \n                -traversalDepthLimit -1\n                -keyPressCommand \"nodeEdKeyPressCommand\" \n                -nodeTitleMode \"name\" \n                -gridSnap 0\n                -gridVisibility 1\n                -popupMenuScript \"nodeEdBuildPanelMenus\" \n                -showNamespace 1\n                -showShapes 1\n                -showSGShapes 0\n                -showTransforms 1\n                -useAssets 1\n                -syncedSelection 1\n                -extendToShapes 1\n                -activeTab -1\n                -editorMode \"default\" \n                $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"createNodePanel\" (localizedPanelLabel(\"Create Node\")) `;\n"
		+ "\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"createNodePanel\" -l (localizedPanelLabel(\"Create Node\")) -mbv $menusOkayInPanels `;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Create Node\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextPanel \"shapePanel\" (localizedPanelLabel(\"Shape Editor\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\tshapePanel -unParent -l (localizedPanelLabel(\"Shape Editor\")) -mbv $menusOkayInPanels ;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tshapePanel -edit -l (localizedPanelLabel(\"Shape Editor\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextPanel \"posePanel\" (localizedPanelLabel(\"Pose Editor\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\tposePanel -unParent -l (localizedPanelLabel(\"Pose Editor\")) -mbv $menusOkayInPanels ;\n"
		+ "\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tposePanel -edit -l (localizedPanelLabel(\"Pose Editor\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"profilerPanel\" (localizedPanelLabel(\"Profiler Tool\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"profilerPanel\" -l (localizedPanelLabel(\"Profiler Tool\")) -mbv $menusOkayInPanels `;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Profiler Tool\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"timeEditorPanel\" (localizedPanelLabel(\"Time Editor\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"timeEditorPanel\" -l (localizedPanelLabel(\"Time Editor\")) -mbv $menusOkayInPanels `;\n"
		+ "\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Time Editor\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextPanel \"outlinerPanel\" (localizedPanelLabel(\"ToggledOutliner\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `outlinerPanel -unParent -l (localizedPanelLabel(\"ToggledOutliner\")) -mbv $menusOkayInPanels `;\n\t\t\t$editorName = $panelName;\n            outlinerEditor -e \n                -showShapes 0\n                -showAssignedMaterials 0\n                -showTimeEditor 1\n                -showReferenceNodes 0\n                -showReferenceMembers 0\n                -showAttributes 0\n                -showConnected 0\n                -showAnimCurvesOnly 0\n                -showMuteInfo 0\n                -organizeByLayer 1\n                -showAnimLayerWeight 1\n                -autoExpandLayers 1\n                -autoExpand 0\n                -showDagOnly 1\n"
		+ "                -showAssets 1\n                -showContainedOnly 1\n                -showPublishedAsConnected 0\n                -showContainerContents 1\n                -ignoreDagHierarchy 0\n                -expandConnections 0\n                -showUpstreamCurves 1\n                -showUnitlessCurves 1\n                -showCompounds 1\n                -showLeafs 1\n                -showNumericAttrsOnly 0\n                -highlightActive 1\n                -autoSelectNewObjects 0\n                -doNotSelectNewObjects 0\n                -dropIsParent 1\n                -transmitFilters 0\n                -setFilter \"0\" \n                -showSetMembers 1\n                -allowMultiSelection 1\n                -alwaysToggleSelect 0\n                -directSelect 0\n                -isSet 0\n                -isSetMember 0\n                -displayMode \"DAG\" \n                -expandObjects 0\n                -setsIgnoreFilters 1\n                -containersIgnoreFilters 0\n                -editAttrName 0\n                -showAttrValues 0\n"
		+ "                -highlightSecondary 0\n                -showUVAttrsOnly 0\n                -showTextureNodesOnly 0\n                -attrAlphaOrder \"default\" \n                -animLayerFilterOptions \"allAffecting\" \n                -sortOrder \"none\" \n                -longNames 0\n                -niceNames 1\n                -showNamespace 1\n                -showPinIcons 0\n                -mapMotionTrails 0\n                -ignoreHiddenAttribute 0\n                -ignoreOutlinerColor 0\n                -renderFilterVisible 0\n                -renderFilterIndex 0\n                -selectionOrder \"chronological\" \n                -expandAttribute 0\n                $editorName;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\toutlinerPanel -edit -l (localizedPanelLabel(\"ToggledOutliner\")) -mbv $menusOkayInPanels  $panelName;\n\t\t$editorName = $panelName;\n        outlinerEditor -e \n            -showShapes 0\n            -showAssignedMaterials 0\n            -showTimeEditor 1\n            -showReferenceNodes 0\n            -showReferenceMembers 0\n"
		+ "            -showAttributes 0\n            -showConnected 0\n            -showAnimCurvesOnly 0\n            -showMuteInfo 0\n            -organizeByLayer 1\n            -showAnimLayerWeight 1\n            -autoExpandLayers 1\n            -autoExpand 0\n            -showDagOnly 1\n            -showAssets 1\n            -showContainedOnly 1\n            -showPublishedAsConnected 0\n            -showContainerContents 1\n            -ignoreDagHierarchy 0\n            -expandConnections 0\n            -showUpstreamCurves 1\n            -showUnitlessCurves 1\n            -showCompounds 1\n            -showLeafs 1\n            -showNumericAttrsOnly 0\n            -highlightActive 1\n            -autoSelectNewObjects 0\n            -doNotSelectNewObjects 0\n            -dropIsParent 1\n            -transmitFilters 0\n            -setFilter \"0\" \n            -showSetMembers 1\n            -allowMultiSelection 1\n            -alwaysToggleSelect 0\n            -directSelect 0\n            -isSet 0\n            -isSetMember 0\n            -displayMode \"DAG\" \n"
		+ "            -expandObjects 0\n            -setsIgnoreFilters 1\n            -containersIgnoreFilters 0\n            -editAttrName 0\n            -showAttrValues 0\n            -highlightSecondary 0\n            -showUVAttrsOnly 0\n            -showTextureNodesOnly 0\n            -attrAlphaOrder \"default\" \n            -animLayerFilterOptions \"allAffecting\" \n            -sortOrder \"none\" \n            -longNames 0\n            -niceNames 1\n            -showNamespace 1\n            -showPinIcons 0\n            -mapMotionTrails 0\n            -ignoreHiddenAttribute 0\n            -ignoreOutlinerColor 0\n            -renderFilterVisible 0\n            -renderFilterIndex 0\n            -selectionOrder \"chronological\" \n            -expandAttribute 0\n            $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\tif ($useSceneConfig) {\n        string $configName = `getPanel -cwl (localizedPanelLabel(\"Current Layout\"))`;\n        if (\"\" != $configName) {\n\t\t\tpanelConfiguration -edit -label (localizedPanelLabel(\"Current Layout\")) \n"
		+ "\t\t\t\t-userCreated false\n\t\t\t\t-defaultImage \"vacantCell.xP:/\"\n\t\t\t\t-image \"\"\n\t\t\t\t-sc false\n\t\t\t\t-configString \"global string $gMainPane; paneLayout -e -cn \\\"single\\\" -ps 1 100 100 $gMainPane;\"\n\t\t\t\t-removeAllPanels\n\t\t\t\t-ap false\n\t\t\t\t\t(localizedPanelLabel(\"Persp View\")) \n\t\t\t\t\t\"modelPanel\"\n"
		+ "\t\t\t\t\t\"$panelName = `modelPanel -unParent -l (localizedPanelLabel(\\\"Persp View\\\")) -mbv $menusOkayInPanels `;\\n$editorName = $panelName;\\nmodelEditor -e \\n    -cam `findStartUpCamera persp` \\n    -useInteractiveMode 0\\n    -displayLights \\\"default\\\" \\n    -displayAppearance \\\"wireframe\\\" \\n    -activeOnly 0\\n    -ignorePanZoom 0\\n    -wireframeOnShaded 0\\n    -headsUpDisplay 1\\n    -holdOuts 1\\n    -selectionHiliteDisplay 1\\n    -useDefaultMaterial 0\\n    -bufferMode \\\"double\\\" \\n    -twoSidedLighting 1\\n    -backfaceCulling 0\\n    -xray 0\\n    -jointXray 0\\n    -activeComponentsXray 0\\n    -displayTextures 0\\n    -smoothWireframe 0\\n    -lineWidth 1\\n    -textureAnisotropic 0\\n    -textureHilight 1\\n    -textureSampling 2\\n    -textureDisplay \\\"modulate\\\" \\n    -textureMaxSize 16384\\n    -fogging 0\\n    -fogSource \\\"fragment\\\" \\n    -fogMode \\\"linear\\\" \\n    -fogStart 0\\n    -fogEnd 100\\n    -fogDensity 0.1\\n    -fogColor 0.5 0.5 0.5 1 \\n    -depthOfFieldPreview 1\\n    -maxConstantTransparency 1\\n    -rendererName \\\"base_OpenGL_Renderer\\\" \\n    -objectFilterShowInHUD 1\\n    -isFiltered 0\\n    -colorResolution 256 256 \\n    -bumpResolution 512 512 \\n    -textureCompression 0\\n    -transparencyAlgorithm \\\"frontAndBackCull\\\" \\n    -transpInShadows 0\\n    -cullingOverride \\\"none\\\" \\n    -lowQualityLighting 0\\n    -maximumNumHardwareLights 1\\n    -occlusionCulling 0\\n    -shadingModel 0\\n    -useBaseRenderer 0\\n    -useReducedRenderer 0\\n    -smallObjectCulling 0\\n    -smallObjectThreshold -1 \\n    -interactiveDisableShadows 0\\n    -interactiveBackFaceCull 0\\n    -sortTransparent 1\\n    -nurbsCurves 1\\n    -nurbsSurfaces 1\\n    -polymeshes 1\\n    -subdivSurfaces 1\\n    -planes 1\\n    -lights 1\\n    -cameras 1\\n    -controlVertices 1\\n    -hulls 1\\n    -grid 1\\n    -imagePlane 1\\n    -joints 1\\n    -ikHandles 1\\n    -deformers 1\\n    -dynamics 1\\n    -particleInstancers 1\\n    -fluids 1\\n    -hairSystems 1\\n    -follicles 1\\n    -nCloths 1\\n    -nParticles 1\\n    -nRigids 1\\n    -dynamicConstraints 1\\n    -locators 1\\n    -manipulators 1\\n    -pluginShapes 1\\n    -dimensions 1\\n    -handles 1\\n    -pivots 1\\n    -textures 1\\n    -strokes 1\\n    -motionTrails 1\\n    -clipGhosts 1\\n    -greasePencils 1\\n    -shadows 0\\n    -captureSequenceNumber -1\\n    -width 483\\n    -height 709\\n    -sceneRenderFilter 0\\n    $editorName;\\nmodelEditor -e -viewSelected 0 $editorName;\\nmodelEditor -e \\n    -pluginObjects \\\"gpuCacheDisplayFilter\\\" 1 \\n    $editorName\"\n"
		+ "\t\t\t\t\t\"modelPanel -edit -l (localizedPanelLabel(\\\"Persp View\\\")) -mbv $menusOkayInPanels  $panelName;\\n$editorName = $panelName;\\nmodelEditor -e \\n    -cam `findStartUpCamera persp` \\n    -useInteractiveMode 0\\n    -displayLights \\\"default\\\" \\n    -displayAppearance \\\"wireframe\\\" \\n    -activeOnly 0\\n    -ignorePanZoom 0\\n    -wireframeOnShaded 0\\n    -headsUpDisplay 1\\n    -holdOuts 1\\n    -selectionHiliteDisplay 1\\n    -useDefaultMaterial 0\\n    -bufferMode \\\"double\\\" \\n    -twoSidedLighting 1\\n    -backfaceCulling 0\\n    -xray 0\\n    -jointXray 0\\n    -activeComponentsXray 0\\n    -displayTextures 0\\n    -smoothWireframe 0\\n    -lineWidth 1\\n    -textureAnisotropic 0\\n    -textureHilight 1\\n    -textureSampling 2\\n    -textureDisplay \\\"modulate\\\" \\n    -textureMaxSize 16384\\n    -fogging 0\\n    -fogSource \\\"fragment\\\" \\n    -fogMode \\\"linear\\\" \\n    -fogStart 0\\n    -fogEnd 100\\n    -fogDensity 0.1\\n    -fogColor 0.5 0.5 0.5 1 \\n    -depthOfFieldPreview 1\\n    -maxConstantTransparency 1\\n    -rendererName \\\"base_OpenGL_Renderer\\\" \\n    -objectFilterShowInHUD 1\\n    -isFiltered 0\\n    -colorResolution 256 256 \\n    -bumpResolution 512 512 \\n    -textureCompression 0\\n    -transparencyAlgorithm \\\"frontAndBackCull\\\" \\n    -transpInShadows 0\\n    -cullingOverride \\\"none\\\" \\n    -lowQualityLighting 0\\n    -maximumNumHardwareLights 1\\n    -occlusionCulling 0\\n    -shadingModel 0\\n    -useBaseRenderer 0\\n    -useReducedRenderer 0\\n    -smallObjectCulling 0\\n    -smallObjectThreshold -1 \\n    -interactiveDisableShadows 0\\n    -interactiveBackFaceCull 0\\n    -sortTransparent 1\\n    -nurbsCurves 1\\n    -nurbsSurfaces 1\\n    -polymeshes 1\\n    -subdivSurfaces 1\\n    -planes 1\\n    -lights 1\\n    -cameras 1\\n    -controlVertices 1\\n    -hulls 1\\n    -grid 1\\n    -imagePlane 1\\n    -joints 1\\n    -ikHandles 1\\n    -deformers 1\\n    -dynamics 1\\n    -particleInstancers 1\\n    -fluids 1\\n    -hairSystems 1\\n    -follicles 1\\n    -nCloths 1\\n    -nParticles 1\\n    -nRigids 1\\n    -dynamicConstraints 1\\n    -locators 1\\n    -manipulators 1\\n    -pluginShapes 1\\n    -dimensions 1\\n    -handles 1\\n    -pivots 1\\n    -textures 1\\n    -strokes 1\\n    -motionTrails 1\\n    -clipGhosts 1\\n    -greasePencils 1\\n    -shadows 0\\n    -captureSequenceNumber -1\\n    -width 483\\n    -height 709\\n    -sceneRenderFilter 0\\n    $editorName;\\nmodelEditor -e -viewSelected 0 $editorName;\\nmodelEditor -e \\n    -pluginObjects \\\"gpuCacheDisplayFilter\\\" 1 \\n    $editorName\"\n"
		+ "\t\t\t\t$configName;\n\n            setNamedPanelLayout (localizedPanelLabel(\"Current Layout\"));\n        }\n\n        panelHistory -e -clear mainPanelHistory;\n        setFocus `paneLayout -q -p1 $gMainPane`;\n        sceneUIReplacement -deleteRemaining;\n        sceneUIReplacement -clear;\n\t}\n\n\ngrid -spacing 5 -size 12 -divisions 5 -displayAxes yes -displayGridLines yes -displayDivisionLines yes -displayPerspectiveLabels no -displayOrthographicLabels no -displayAxesBold yes -perspectiveLabelPosition axis -orthographicLabelPosition edge;\nviewManip -drawCompass 0 -compassAngle 0 -frontParameters \"\" -homeParameters \"\" -selectionLockParameters \"\";\n}\n");
	setAttr ".st" 3;
createNode script -n "sceneConfigurationScriptNode";
	rename -uid "F211ABEE-433C-8ABB-3925-E091242BB054";
	setAttr ".b" -type "string" "playbackOptions -min 1 -max 24 -ast 1 -aet 48 ";
	setAttr ".st" 6;
createNode shadingEngine -n "aiStandard1SG";
	rename -uid "8C097BC1-48C8-2410-3C43-9F98B6B87069";
	setAttr ".ihi" 0;
	setAttr ".ro" yes;
createNode materialInfo -n "materialInfo1";
	rename -uid "A3E797F3-4C93-2158-70CE-8697639B7F99";
createNode polySphere -n "polySphere1";
	rename -uid "E6C1106A-40C9-6A92-CD0E-CBAF56082151";
	setAttr ".r" 6.3190084466120737;
createNode aiAOVFilter -s -n "defaultArnoldFilter";
	rename -uid "D3B19325-49D6-AFA5-5657-7DB3DC1E9949";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVDriver -s -n "defaultArnoldDriver";
	rename -uid "593EC056-44A7-EEB4-DBDD-FCBFE78EBE1B";
	addAttr -ci true -sn "gamma" -ln "gamma" -dv 2.2000000476837158 -min 9.9999997473787516e-005 
		-smx 5 -at "float";
	addAttr -ci true -sn "dither_amplitude" -ln "ditherAmplitude" -dv 1 -at "float";
	setAttr ".ai_translator" -type "string" "tif";
createNode materialInfo -n "materialInfo2";
	rename -uid "789A319F-4CC4-C7DE-EA03-A1A8C5423F7E";
createNode aiStandard -n "aiStandard2";
	rename -uid "392A42AA-4577-9F32-4E0A-FBA66566BBEA";
createNode shadingEngine -n "aiStandard2SG";
	rename -uid "2C2A1752-480D-53BC-9D1A-D89233EA639F";
	setAttr ".ihi" 0;
	setAttr ".ro" yes;
createNode materialInfo -n "materialInfo3";
	rename -uid "C164BCB8-47BA-68D5-1EDE-D8BE3248ED10";
createNode shapeEditorManager -n "shapeEditorManager";
	rename -uid "B3F6E044-494A-B530-871C-3AA305E3CF93";
createNode poseInterpolatorManager -n "poseInterpolatorManager";
	rename -uid "7EFF0989-4493-37D0-E674-2B97380E5F05";
createNode aiAOVDriver -s -n "defaultArnoldDisplayDriver";
	rename -uid "84B2CA08-4FF9-1D6E-C08B-0B89C6AEA3A7";
	setAttr ".output_mode" 0;
	setAttr ".ai_translator" -type "string" "maya";
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

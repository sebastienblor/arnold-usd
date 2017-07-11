//Maya ASCII 2017ff05 scene
//Name: test.ma
//Last modified: Thu, Jun 29, 2017 11:09:52 PM
//Codeset: 1252
requires maya "2017ff05";
requires -nodeType "aiOptions" -nodeType "aiAOVDriver" -nodeType "aiAOVFilter" -nodeType "aiStandard"
		 "mtoa" "2.0.2.wip";
requires "stereoCamera" "10.0";
currentUnit -l centimeter -a degree -t film;
fileInfo "application" "maya";
fileInfo "product" "Maya 2017";
fileInfo "version" "2017";
fileInfo "cutIdentifier" "201706020738-1017329";
fileInfo "osv" "Microsoft Windows 7 Enterprise Edition, 64-bit Windows 7 Service Pack 1 (Build 7601)\n";
createNode transform -s -n "persp";
	rename -uid "D878A15F-481C-8DEB-7966-17BA14B00E70";
	setAttr ".v" no;
	setAttr ".t" -type "double3" 27.795094234407376 18.971162668046485 0 ;
	setAttr ".r" -type "double3" -24.000000000000004 90 0 ;
	setAttr ".rp" -type "double3" -3.5804692544161291e-015 -7.1054273576010019e-015 
		-1.4210854715202004e-014 ;
	setAttr ".rpt" -type "double3" -8.7783779362275568e-015 1.1498476670238775e-014 
		9.4020555375448207e-016 ;
createNode camera -s -n "perspShape" -p "persp";
	rename -uid "174B3B72-45CA-3712-8A6C-8B9E9C5B914E";
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
	rename -uid "A81DE207-4092-35BC-C70D-229B0FBEAD3D";
	setAttr ".v" no;
	setAttr ".t" -type "double3" 0 100.1 0 ;
	setAttr ".r" -type "double3" -89.999999999999986 0 0 ;
createNode camera -s -n "topShape" -p "top";
	rename -uid "71BC9EB8-413C-8B79-C6D8-CDBFC136915F";
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
	rename -uid "3E1274DD-4C2D-F762-95E8-D686DAFC2DF9";
	setAttr ".v" no;
	setAttr ".t" -type "double3" 0 0 100.1 ;
createNode camera -s -n "frontShape" -p "front";
	rename -uid "BBAA6C65-442A-9E0D-189B-B386683C0E8F";
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
	rename -uid "CE3DA2AF-41E3-9017-86C1-D38F349E9A25";
	setAttr ".v" no;
	setAttr ".t" -type "double3" 100.1 1.6871751536864319 12.910557697774426 ;
	setAttr ".r" -type "double3" 0 89.999999999999986 0 ;
createNode camera -s -n "sideShape" -p "side";
	rename -uid "F4139135-4CD2-4E87-B512-B2B890449238";
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
	rename -uid "630544EA-4B0C-79CA-0013-B3A163A9A4F8";
	setAttr ".t" -type "double3" -0.020665766309299727 6.3190084466120737 0.27269214094604877 ;
createNode mesh -n "pSphereShape1" -p "pSphere1";
	rename -uid "0899DE0F-4C96-25D6-1B9E-EDAC854E1086";
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
	rename -uid "5398FE3B-42A7-8AE7-4514-16A759C0E10E";
	setAttr ".r" -type "double3" -48.156552041713944 1.3059010322141336 -9.5202672381429512 ;
	setAttr ".s" -type "double3" 30.769077416337016 30.769077416337016 30.769077416337016 ;
createNode directionalLight -n "directionalLightShape1" -p "directionalLight1";
	rename -uid "C45B445F-4D52-4C1C-CDC6-BFB3070D2AD2";
	addAttr -ci true -sn "ai_bounce_factor" -ln "aiBounceFactor" -dv 1 -min 0 -smx 
		20 -at "float";
	addAttr -ci true -sn "ai_bounces" -ln "aiBounces" -dv 999 -min 0 -max 10000 -at "long";
	setAttr -k off ".v";
	setAttr ".urs" no;
	setAttr ".rdl" 1;
createNode transform -n "hairSystem1";
	rename -uid "7F6DF513-4140-0598-B08F-D18B9F78983A";
createNode hairSystem -n "hairSystemShape1" -p "hairSystem1";
	rename -uid "26CDA608-450D-24A4-01E0-D8A01AE1C7E3";
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
	rename -uid "D6DCF8C7-4914-CED6-7BD9-EDB5BF0B2706";
createNode transform -n "pSphere1Follicle303" -p "hairSystem1Follicles";
	rename -uid "B2FE140E-46C2-9C2D-2C0E-34A67D2F688B";
createNode follicle -n "pSphere1FollicleShape303" -p "pSphere1Follicle303";
	rename -uid "794BD87D-47EE-911D-1184-A2AA52623D0C";
	setAttr -k off ".v";
	setAttr ".pu" 0.03125;
	setAttr ".pv" 0.03125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve1" -p "pSphere1Follicle303";
	rename -uid "E83AC253-41B0-0763-F6E4-E9B38FA17EC7";
createNode nurbsCurve -n "curveShape1" -p "curve1";
	rename -uid "3FCD1E0E-4D9F-813C-1B9A-77B1240CA219";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "F6E7B598-4AB5-4A6F-C19F-ECA05A51BB50";
createNode follicle -n "pSphere1FollicleShape309" -p "pSphere1Follicle309";
	rename -uid "C181BB60-489C-DA83-552E-2DBCBA463963";
	setAttr -k off ".v";
	setAttr ".pu" 0.03125;
	setAttr ".pv" 0.09375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve2" -p "pSphere1Follicle309";
	rename -uid "C8FFC6A6-4193-28D4-CBCA-9E82B445F03C";
createNode nurbsCurve -n "curveShape2" -p "curve2";
	rename -uid "65630A49-4DA4-8D49-19F2-9F88F4F087D1";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "7DCD3694-41B8-AE50-2A27-3FAA23CE5CFD";
createNode follicle -n "pSphere1FollicleShape315" -p "pSphere1Follicle315";
	rename -uid "9E4B2AAB-410F-0B5B-1061-16A09C5E2FB7";
	setAttr -k off ".v";
	setAttr ".pu" 0.03125;
	setAttr ".pv" 0.15625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve3" -p "pSphere1Follicle315";
	rename -uid "F7059339-4F7B-9590-F9AE-75ACC276114C";
createNode nurbsCurve -n "curveShape3" -p "curve3";
	rename -uid "7FDFBFD6-4617-C1C8-66B9-ED86AD1D6A43";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "434A07E3-47BC-7DCD-5811-849812E78A30";
createNode follicle -n "pSphere1FollicleShape322" -p "pSphere1Follicle322";
	rename -uid "D16E3D8E-4C7A-78F9-B1B3-56A331D3DB05";
	setAttr -k off ".v";
	setAttr ".pu" 0.03125;
	setAttr ".pv" 0.21875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve4" -p "pSphere1Follicle322";
	rename -uid "8986784A-4228-9CBE-232E-D0B77EC8B007";
createNode nurbsCurve -n "curveShape4" -p "curve4";
	rename -uid "50D62A76-41CA-5085-A2A4-6BB1316D74DB";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "975B1D36-4515-8004-C9AC-E9AD57FDD68D";
createNode follicle -n "pSphere1FollicleShape328" -p "pSphere1Follicle328";
	rename -uid "2CFE37DE-48EB-ACD2-ABE8-F38B4059489C";
	setAttr -k off ".v";
	setAttr ".pu" 0.03125;
	setAttr ".pv" 0.28125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve5" -p "pSphere1Follicle328";
	rename -uid "0D5384DE-4C38-CB22-2371-77AB161C1913";
createNode nurbsCurve -n "curveShape5" -p "curve5";
	rename -uid "10979B80-418E-EB7C-DBA3-01B19D36C8DE";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "C75E8897-4C5B-7FAC-ED3F-50ABDF116249";
createNode follicle -n "pSphere1FollicleShape334" -p "pSphere1Follicle334";
	rename -uid "5ADA90B2-40A5-0E19-F6C5-11A4A26149D7";
	setAttr -k off ".v";
	setAttr ".pu" 0.03125;
	setAttr ".pv" 0.34375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve6" -p "pSphere1Follicle334";
	rename -uid "A485EE0E-4B2B-1251-6A0E-8B9FA5722685";
createNode nurbsCurve -n "curveShape6" -p "curve6";
	rename -uid "988FB769-4B23-3C8D-F935-8BBFA97D008F";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "088C45A3-4CDE-2BE2-44FF-8C9594951F67";
createNode follicle -n "pSphere1FollicleShape340" -p "pSphere1Follicle340";
	rename -uid "18775419-4F06-288E-B323-3986A4695906";
	setAttr -k off ".v";
	setAttr ".pu" 0.03125;
	setAttr ".pv" 0.40625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve7" -p "pSphere1Follicle340";
	rename -uid "2488B705-4A1A-F536-842C-02A7B6AE26B9";
createNode nurbsCurve -n "curveShape7" -p "curve7";
	rename -uid "03F0A8A2-4EF2-1147-7DE7-3F80B550B4E8";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "8D20B8AA-4122-67C2-6960-14B6A6B69905";
createNode follicle -n "pSphere1FollicleShape346" -p "pSphere1Follicle346";
	rename -uid "E85EEACB-4868-820B-D3E3-F1A87C528118";
	setAttr -k off ".v";
	setAttr ".pu" 0.03125;
	setAttr ".pv" 0.46875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve8" -p "pSphere1Follicle346";
	rename -uid "C97552C2-476F-67B0-0582-EAB8487E1D09";
createNode nurbsCurve -n "curveShape8" -p "curve8";
	rename -uid "33173A61-44A0-43E2-61BA-05AAA9051B2D";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "55DDA3B7-4148-68AD-668E-6884169F0DC9";
createNode follicle -n "pSphere1FollicleShape353" -p "pSphere1Follicle353";
	rename -uid "A1D84429-42A5-9523-2B86-4FA595B7D530";
	setAttr -k off ".v";
	setAttr ".pu" 0.03125;
	setAttr ".pv" 0.53125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve9" -p "pSphere1Follicle353";
	rename -uid "E705B80F-416B-11AA-0E8D-EC8B1175645C";
createNode nurbsCurve -n "curveShape9" -p "curve9";
	rename -uid "12B1AA74-4EAE-748F-850F-B0B98B7959E7";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "0761973C-4E0A-4619-FE63-49ABDE09FCD1";
createNode follicle -n "pSphere1FollicleShape359" -p "pSphere1Follicle359";
	rename -uid "BCF8CBC6-4E60-CC50-3C30-BAAD80CF16AD";
	setAttr -k off ".v";
	setAttr ".pu" 0.03125;
	setAttr ".pv" 0.59375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve10" -p "pSphere1Follicle359";
	rename -uid "BC10189B-4B11-D709-0203-F28337A2741D";
createNode nurbsCurve -n "curveShape10" -p "curve10";
	rename -uid "D7AFA793-4463-C0DF-D501-7383534F69CF";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "E56456A3-4B09-B7C2-8111-28923B4ADF22";
createNode follicle -n "pSphere1FollicleShape365" -p "pSphere1Follicle365";
	rename -uid "6D77DD61-4948-0E85-1AD0-FE88A17B0483";
	setAttr -k off ".v";
	setAttr ".pu" 0.03125;
	setAttr ".pv" 0.65625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve11" -p "pSphere1Follicle365";
	rename -uid "1569D187-4294-EDED-F622-B388D1BAB757";
createNode nurbsCurve -n "curveShape11" -p "curve11";
	rename -uid "7823FF2C-44CC-C112-004B-939E7E2E8F8C";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "2B8BED1C-4A00-3F3B-51D6-82B51CD7DCE4";
createNode follicle -n "pSphere1FollicleShape371" -p "pSphere1Follicle371";
	rename -uid "03C0B1C6-44AA-0628-9693-80B1BC8C3949";
	setAttr -k off ".v";
	setAttr ".pu" 0.03125;
	setAttr ".pv" 0.71875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve12" -p "pSphere1Follicle371";
	rename -uid "BE30BA53-4BF2-C93B-C716-728B740B23C3";
createNode nurbsCurve -n "curveShape12" -p "curve12";
	rename -uid "D02EB90C-49C5-E25D-BBDA-CBB4C7A1261D";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "EB44EFC2-41D1-8E9C-1E0D-408FD1823DF8";
createNode follicle -n "pSphere1FollicleShape377" -p "pSphere1Follicle377";
	rename -uid "8EEF99E0-464E-1F9E-F76A-368747A613C6";
	setAttr -k off ".v";
	setAttr ".pu" 0.03125;
	setAttr ".pv" 0.78125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve13" -p "pSphere1Follicle377";
	rename -uid "6C3FCC3E-4CB4-FFB3-4168-C2A872C49930";
createNode nurbsCurve -n "curveShape13" -p "curve13";
	rename -uid "719482A2-4C6B-615D-7277-7F95B00772CA";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "2DF7F0BE-4B44-7FF3-3056-E0A71AFD31AE";
createNode follicle -n "pSphere1FollicleShape384" -p "pSphere1Follicle384";
	rename -uid "1A3CB63B-4AA4-83E9-64CE-E6A00E525304";
	setAttr -k off ".v";
	setAttr ".pu" 0.03125;
	setAttr ".pv" 0.84375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve14" -p "pSphere1Follicle384";
	rename -uid "735641DF-4098-3D90-BE13-1BAEE44AF958";
createNode nurbsCurve -n "curveShape14" -p "curve14";
	rename -uid "B9609F5C-4141-DA86-F1A3-46982B1BD742";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "2A769699-4167-18E0-E743-07A47AE11495";
createNode follicle -n "pSphere1FollicleShape390" -p "pSphere1Follicle390";
	rename -uid "44C8A523-4C17-D37E-D903-A48DD56F11C5";
	setAttr -k off ".v";
	setAttr ".pu" 0.03125;
	setAttr ".pv" 0.90625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve15" -p "pSphere1Follicle390";
	rename -uid "5A32B99F-4AEA-67A0-BD0D-8C9198823E2B";
createNode nurbsCurve -n "curveShape15" -p "curve15";
	rename -uid "E4E62E78-436A-7DA2-7A40-7887A6679284";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "C124A2A3-44D1-690A-8C3B-92985AEAC86F";
createNode follicle -n "pSphere1FollicleShape396" -p "pSphere1Follicle396";
	rename -uid "6D64FF7C-451F-4BB6-BD56-5B990F84BEE0";
	setAttr -k off ".v";
	setAttr ".pu" 0.03125;
	setAttr ".pv" 0.96875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve16" -p "pSphere1Follicle396";
	rename -uid "FC2A693C-4E0A-9270-C9B6-FEB87BD86D81";
createNode nurbsCurve -n "curveShape16" -p "curve16";
	rename -uid "ABA0C84C-45C4-AAF5-9A9C-C8823A8BC444";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "6BFF9C64-4839-B386-5F8A-69AE626AC0C2";
createNode follicle -n "pSphere1FollicleShape909" -p "pSphere1Follicle909";
	rename -uid "62CF09D3-4F05-6C5E-AC44-C5BC5FF2CC08";
	setAttr -k off ".v";
	setAttr ".pu" 0.09375;
	setAttr ".pv" 0.09375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve17" -p "pSphere1Follicle909";
	rename -uid "7E31BFDD-424B-5388-DE6C-0DB7BFD77E91";
createNode nurbsCurve -n "curveShape17" -p "curve17";
	rename -uid "0E111735-46EE-9670-F2E5-2D911E2BC976";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "9A38643B-46E8-88B7-7F7A-F986263C0620";
createNode follicle -n "pSphere1FollicleShape915" -p "pSphere1Follicle915";
	rename -uid "08BB83D9-446A-8BA0-5A6F-4FAD4F41C5B4";
	setAttr -k off ".v";
	setAttr ".pu" 0.09375;
	setAttr ".pv" 0.15625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve18" -p "pSphere1Follicle915";
	rename -uid "07180A4C-4BA6-167D-3193-819C4790BF55";
createNode nurbsCurve -n "curveShape18" -p "curve18";
	rename -uid "527B9A87-4DEA-3AF0-7734-84894BEABE19";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "B667DDC9-46BF-F1BA-1554-4C916E9A15AC";
createNode follicle -n "pSphere1FollicleShape922" -p "pSphere1Follicle922";
	rename -uid "5B7F8067-4FEA-22EF-96E6-5285AB9E866A";
	setAttr -k off ".v";
	setAttr ".pu" 0.09375;
	setAttr ".pv" 0.21875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve19" -p "pSphere1Follicle922";
	rename -uid "87727A1F-44D0-EAB8-77D1-C9BD07237C81";
createNode nurbsCurve -n "curveShape19" -p "curve19";
	rename -uid "5A91AFA0-4E53-141D-B5BF-538401D0D1F2";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "18219137-4851-3AF4-EED9-76846684B5BB";
createNode follicle -n "pSphere1FollicleShape928" -p "pSphere1Follicle928";
	rename -uid "87F7E01C-4D38-6393-BBD7-9DBEEA8E89B1";
	setAttr -k off ".v";
	setAttr ".pu" 0.09375;
	setAttr ".pv" 0.28125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve20" -p "pSphere1Follicle928";
	rename -uid "C1C6F318-4955-F8E4-9B82-1988C7CEB567";
createNode nurbsCurve -n "curveShape20" -p "curve20";
	rename -uid "8DEB3C00-49A5-0A52-3E23-C1965D238EC6";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "5CD3F4E6-4A59-0934-D3AF-CBBBD6401CDF";
createNode follicle -n "pSphere1FollicleShape934" -p "pSphere1Follicle934";
	rename -uid "E7CE0647-431F-87A8-280C-B4B6563932E1";
	setAttr -k off ".v";
	setAttr ".pu" 0.09375;
	setAttr ".pv" 0.34375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve21" -p "pSphere1Follicle934";
	rename -uid "807D55F6-449A-D34B-1CAB-FC88D0200B31";
createNode nurbsCurve -n "curveShape21" -p "curve21";
	rename -uid "DBC94194-4CAD-8033-B0A2-8990CBD88783";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "D680328C-4FB7-5847-A314-DFB8369F9307";
createNode follicle -n "pSphere1FollicleShape940" -p "pSphere1Follicle940";
	rename -uid "6B72B994-4A1D-610B-0D18-7497D259C50F";
	setAttr -k off ".v";
	setAttr ".pu" 0.09375;
	setAttr ".pv" 0.40625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve22" -p "pSphere1Follicle940";
	rename -uid "E33AA93B-448C-E4AF-7132-F6AC7F13F722";
createNode nurbsCurve -n "curveShape22" -p "curve22";
	rename -uid "61DFD866-49B8-BD22-AF0D-D6A7FE046198";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "A13AF89E-4015-01BF-6199-36A3F4DD46D7";
createNode follicle -n "pSphere1FollicleShape946" -p "pSphere1Follicle946";
	rename -uid "97322C21-4CA4-1DDC-5DC9-FC828E940380";
	setAttr -k off ".v";
	setAttr ".pu" 0.09375;
	setAttr ".pv" 0.46875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve23" -p "pSphere1Follicle946";
	rename -uid "8EAED763-4638-B0BF-64AA-AFB73F1E95B5";
createNode nurbsCurve -n "curveShape23" -p "curve23";
	rename -uid "E439B72B-47C2-8266-8A81-0BA23B384BC8";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "31670D31-4A9C-7BE9-095B-2DA91ADDE707";
createNode follicle -n "pSphere1FollicleShape953" -p "pSphere1Follicle953";
	rename -uid "E4CFA070-4954-EC7E-2873-5BA5C1C1ACFF";
	setAttr -k off ".v";
	setAttr ".pu" 0.09375;
	setAttr ".pv" 0.53125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve24" -p "pSphere1Follicle953";
	rename -uid "2E1636A2-41B7-1727-4B5C-DD8F50E2576F";
createNode nurbsCurve -n "curveShape24" -p "curve24";
	rename -uid "BB3CE594-41C8-97F9-F23E-00B99C2E634C";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "5B2341D7-4B84-675A-E278-26BF78F127DB";
createNode follicle -n "pSphere1FollicleShape959" -p "pSphere1Follicle959";
	rename -uid "B9F28D7A-4EFF-15CA-5572-F0B99374E81A";
	setAttr -k off ".v";
	setAttr ".pu" 0.09375;
	setAttr ".pv" 0.59375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve25" -p "pSphere1Follicle959";
	rename -uid "AB52A1A6-4EF0-55D5-BBAD-659211FD0CB0";
createNode nurbsCurve -n "curveShape25" -p "curve25";
	rename -uid "C085F1A7-4B15-A39E-21B0-C7ABE23A4CD5";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "4377CD2B-450E-8297-E986-1494F17E18EE";
createNode follicle -n "pSphere1FollicleShape965" -p "pSphere1Follicle965";
	rename -uid "E8DD0C84-46BD-2E9D-18DE-6E86959BF813";
	setAttr -k off ".v";
	setAttr ".pu" 0.09375;
	setAttr ".pv" 0.65625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve26" -p "pSphere1Follicle965";
	rename -uid "E31AE53B-446B-C24D-3DAE-1988A134A59B";
createNode nurbsCurve -n "curveShape26" -p "curve26";
	rename -uid "E6BFEB66-4821-1551-7E8B-FF88204F072F";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "2DF0A7A2-4F4F-FB96-15CD-62BF39BC066E";
createNode follicle -n "pSphere1FollicleShape971" -p "pSphere1Follicle971";
	rename -uid "1386D109-468C-DA3B-D464-2EB07D01060D";
	setAttr -k off ".v";
	setAttr ".pu" 0.09375;
	setAttr ".pv" 0.71875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve27" -p "pSphere1Follicle971";
	rename -uid "0838377A-4B06-D38B-38AC-AE86CC5577F6";
createNode nurbsCurve -n "curveShape27" -p "curve27";
	rename -uid "2A708AD7-49F0-4B33-0C32-11A17A75E32F";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "0928E2A8-4E1C-B483-D83B-799B77BAF7E1";
createNode follicle -n "pSphere1FollicleShape977" -p "pSphere1Follicle977";
	rename -uid "D513E110-4E0E-A5BC-977B-CAB55665D367";
	setAttr -k off ".v";
	setAttr ".pu" 0.09375;
	setAttr ".pv" 0.78125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve28" -p "pSphere1Follicle977";
	rename -uid "44AA5B94-4A5C-FD37-9D0B-9380A81A65B2";
createNode nurbsCurve -n "curveShape28" -p "curve28";
	rename -uid "051C3427-4905-0CFC-88F8-AFA01B923242";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "8E4C7714-44E5-CC8B-C3F0-439ED71F4B3B";
createNode follicle -n "pSphere1FollicleShape984" -p "pSphere1Follicle984";
	rename -uid "D5057D73-4E1A-2D7E-27DA-2DACC29508B5";
	setAttr -k off ".v";
	setAttr ".pu" 0.09375;
	setAttr ".pv" 0.84375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve29" -p "pSphere1Follicle984";
	rename -uid "6641B6DC-42FC-1769-B3FA-E28EA2301950";
createNode nurbsCurve -n "curveShape29" -p "curve29";
	rename -uid "13F48AEC-45E0-F61F-A86E-798060B75493";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "D572355D-4486-825F-C6ED-469EB51B5481";
createNode follicle -n "pSphere1FollicleShape990" -p "pSphere1Follicle990";
	rename -uid "BAACBC01-4BF8-7E55-DF25-7393A5A37021";
	setAttr -k off ".v";
	setAttr ".pu" 0.09375;
	setAttr ".pv" 0.90625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve30" -p "pSphere1Follicle990";
	rename -uid "D02FBB03-4407-A592-0A7C-5F963F4182C5";
createNode nurbsCurve -n "curveShape30" -p "curve30";
	rename -uid "2FE9B329-46F7-CCFF-02DA-5F91D940B9B8";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "BC66CFD6-4C61-2265-4B78-448155EE6684";
createNode follicle -n "pSphere1FollicleShape1509" -p "pSphere1Follicle1509";
	rename -uid "70A2D0F3-4160-25F4-F34B-10B6E3CC1B40";
	setAttr -k off ".v";
	setAttr ".pu" 0.15625;
	setAttr ".pv" 0.09375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve31" -p "pSphere1Follicle1509";
	rename -uid "4C397654-471F-9E07-21FD-7180F938C1A9";
createNode nurbsCurve -n "curveShape31" -p "curve31";
	rename -uid "D825157F-4B36-848B-AEBF-29A00283C225";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "0B224993-4A1E-F2C0-F060-12B0444FB10E";
createNode follicle -n "pSphere1FollicleShape1515" -p "pSphere1Follicle1515";
	rename -uid "1A6B37BC-4836-0E89-4ECF-B3A0A9C8DC30";
	setAttr -k off ".v";
	setAttr ".pu" 0.15625;
	setAttr ".pv" 0.15625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve32" -p "pSphere1Follicle1515";
	rename -uid "AC223E09-4BD9-842E-044C-2EAE107033B7";
createNode nurbsCurve -n "curveShape32" -p "curve32";
	rename -uid "6E569460-4F27-535C-5A90-C3B1FAD84D3F";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "8A98822E-4520-332F-04BF-9C93100E6154";
createNode follicle -n "pSphere1FollicleShape1522" -p "pSphere1Follicle1522";
	rename -uid "1A2929BB-43FB-C983-78DA-E6BCF8102C3F";
	setAttr -k off ".v";
	setAttr ".pu" 0.15625;
	setAttr ".pv" 0.21875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve33" -p "pSphere1Follicle1522";
	rename -uid "D485D0D9-4D8A-BBB2-E583-17B25CB2DEC9";
createNode nurbsCurve -n "curveShape33" -p "curve33";
	rename -uid "8984903D-4627-98AB-B88C-BCB1D7A009E5";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "57D3ECE6-4261-D8B3-DD72-EDBE72A22C99";
createNode follicle -n "pSphere1FollicleShape1528" -p "pSphere1Follicle1528";
	rename -uid "19510E9A-4673-8428-EE32-F3A58AB8F351";
	setAttr -k off ".v";
	setAttr ".pu" 0.15625;
	setAttr ".pv" 0.28125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve34" -p "pSphere1Follicle1528";
	rename -uid "99BC70CF-4F0E-FD1B-A252-BE9DF1D8CA98";
createNode nurbsCurve -n "curveShape34" -p "curve34";
	rename -uid "8D2AE097-4518-9C36-4D6D-A08B7BC5FBC3";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "C23B23C4-4AC5-88E2-D4FB-D18A9966A073";
createNode follicle -n "pSphere1FollicleShape1534" -p "pSphere1Follicle1534";
	rename -uid "85F1CD7D-4D42-E520-773A-1F8E93F74C9F";
	setAttr -k off ".v";
	setAttr ".pu" 0.15625;
	setAttr ".pv" 0.34375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve35" -p "pSphere1Follicle1534";
	rename -uid "6B65D23F-4FB8-E83C-8BCD-E99EAE5C30E2";
createNode nurbsCurve -n "curveShape35" -p "curve35";
	rename -uid "06EB1D8E-475B-4145-0857-33A6E1AB2480";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "FE9467CF-444C-C06F-59E0-7DADD9A402F7";
createNode follicle -n "pSphere1FollicleShape1540" -p "pSphere1Follicle1540";
	rename -uid "3C7DD0A1-481B-F519-36A1-538DE63CA3DD";
	setAttr -k off ".v";
	setAttr ".pu" 0.15625;
	setAttr ".pv" 0.40625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve36" -p "pSphere1Follicle1540";
	rename -uid "8142E96E-4E8B-71E2-CB15-E3A862051444";
createNode nurbsCurve -n "curveShape36" -p "curve36";
	rename -uid "7C35A621-41DB-6878-ABAA-CC92C3B9404F";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "A889D6CE-4FFE-8CFB-B820-B89DB917BF4A";
createNode follicle -n "pSphere1FollicleShape1546" -p "pSphere1Follicle1546";
	rename -uid "61EB7B4A-4ADF-E2E6-DA5D-7CAD805A6453";
	setAttr -k off ".v";
	setAttr ".pu" 0.15625;
	setAttr ".pv" 0.46875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve37" -p "pSphere1Follicle1546";
	rename -uid "055D1F92-4918-7C66-0EFF-F29FEC9C02C7";
createNode nurbsCurve -n "curveShape37" -p "curve37";
	rename -uid "5BAD8A35-4F24-D3B8-B2C4-1B9C0102D725";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "2CF755C4-4A47-6858-DC8A-A287F6E8E882";
createNode follicle -n "pSphere1FollicleShape1553" -p "pSphere1Follicle1553";
	rename -uid "F4554CA4-46EB-0699-40E5-268ED9CE3DE2";
	setAttr -k off ".v";
	setAttr ".pu" 0.15625;
	setAttr ".pv" 0.53125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve38" -p "pSphere1Follicle1553";
	rename -uid "FECA2203-4A99-806F-A841-C0B5D42E88AF";
createNode nurbsCurve -n "curveShape38" -p "curve38";
	rename -uid "74E84B33-4F83-F81F-FF9B-C79243980C46";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "EA97C57C-44A3-B8E4-17B6-7A80680B938B";
createNode follicle -n "pSphere1FollicleShape1559" -p "pSphere1Follicle1559";
	rename -uid "24CEFF66-4D1C-7442-DFEE-4FA79839D173";
	setAttr -k off ".v";
	setAttr ".pu" 0.15625;
	setAttr ".pv" 0.59375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve39" -p "pSphere1Follicle1559";
	rename -uid "B751964D-4BD6-D459-3BC7-6D81CF523374";
createNode nurbsCurve -n "curveShape39" -p "curve39";
	rename -uid "E1E07B1A-4E84-C712-A1BB-E4940A6914A1";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "D090A5F9-44C3-EB29-A6D3-6B85C1F701ED";
createNode follicle -n "pSphere1FollicleShape1565" -p "pSphere1Follicle1565";
	rename -uid "C5818FB9-41F2-13CA-7CEE-C1B8F6C9C7D2";
	setAttr -k off ".v";
	setAttr ".pu" 0.15625;
	setAttr ".pv" 0.65625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve40" -p "pSphere1Follicle1565";
	rename -uid "76596762-47C4-A991-8308-8488A2A03FCC";
createNode nurbsCurve -n "curveShape40" -p "curve40";
	rename -uid "20FAAC58-4C11-4BA6-E417-29AEA9877D51";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "0621DFB4-4809-0CE8-D35F-B8BEB3DC5E63";
createNode follicle -n "pSphere1FollicleShape1571" -p "pSphere1Follicle1571";
	rename -uid "33631B36-4E13-F4CC-A225-FD96483CF34A";
	setAttr -k off ".v";
	setAttr ".pu" 0.15625;
	setAttr ".pv" 0.71875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve41" -p "pSphere1Follicle1571";
	rename -uid "34ECAD4E-4724-D45B-3F58-0E9A5B384216";
createNode nurbsCurve -n "curveShape41" -p "curve41";
	rename -uid "AFF2D4D2-4D80-1EAE-A21F-FB8B02DF8490";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "7A409008-4AE2-6878-7118-DCB4FF54F7BE";
createNode follicle -n "pSphere1FollicleShape1577" -p "pSphere1Follicle1577";
	rename -uid "7B98F93F-4D0F-B7D9-3319-2D9C27DFFFD4";
	setAttr -k off ".v";
	setAttr ".pu" 0.15625;
	setAttr ".pv" 0.78125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve42" -p "pSphere1Follicle1577";
	rename -uid "F17A35B8-481C-00E7-C0FA-54B3E2D8218C";
createNode nurbsCurve -n "curveShape42" -p "curve42";
	rename -uid "BBC90713-4C14-59B7-560F-2FBD7D7B401A";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "8E61C593-40AD-A51B-E8AB-75982278E0DF";
createNode follicle -n "pSphere1FollicleShape1584" -p "pSphere1Follicle1584";
	rename -uid "263FBADE-4D3B-3438-D951-DD80A8CE6F23";
	setAttr -k off ".v";
	setAttr ".pu" 0.15625;
	setAttr ".pv" 0.84375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve43" -p "pSphere1Follicle1584";
	rename -uid "7DF832EB-43CE-7B7E-1FAD-C6908EDB2CAE";
createNode nurbsCurve -n "curveShape43" -p "curve43";
	rename -uid "EDD5B97E-44AB-6A4E-DB5C-0F8F2A6C30BC";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "5B2DC05E-4966-8B27-7194-5DBFC44DDFB0";
createNode follicle -n "pSphere1FollicleShape1590" -p "pSphere1Follicle1590";
	rename -uid "A0EEB231-400D-10D8-0FB5-CDA75ABF5A0E";
	setAttr -k off ".v";
	setAttr ".pu" 0.15625;
	setAttr ".pv" 0.90625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve44" -p "pSphere1Follicle1590";
	rename -uid "606DBC52-412B-82CD-E89F-17AFC2C67DF9";
createNode nurbsCurve -n "curveShape44" -p "curve44";
	rename -uid "E9565222-4615-8FD9-1B9A-E6AC9A239440";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "8A884903-4F3D-F581-4831-B9B480FC590A";
createNode follicle -n "pSphere1FollicleShape2203" -p "pSphere1Follicle2203";
	rename -uid "97A59D75-4683-B65E-1E97-48BE7E3678A3";
	setAttr -k off ".v";
	setAttr ".pu" 0.21875;
	setAttr ".pv" 0.03125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve45" -p "pSphere1Follicle2203";
	rename -uid "64AE261D-45E1-EA8D-6146-7DAB66A4E43C";
createNode nurbsCurve -n "curveShape45" -p "curve45";
	rename -uid "46725E12-4A69-0DA5-8D35-A3B9256CB28A";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "CBD8F3C2-4838-842E-88EA-FA85EB134AD6";
createNode follicle -n "pSphere1FollicleShape2209" -p "pSphere1Follicle2209";
	rename -uid "7A447172-43F5-37E9-1AE5-A1ACC5C62E70";
	setAttr -k off ".v";
	setAttr ".pu" 0.21875;
	setAttr ".pv" 0.09375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve46" -p "pSphere1Follicle2209";
	rename -uid "A7BD064F-407E-F077-5B1A-70848DD1F282";
createNode nurbsCurve -n "curveShape46" -p "curve46";
	rename -uid "20FAE1FD-420F-7EA7-33B8-7B87628B8C26";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "2BA0DAD9-4E15-E21C-C411-1884BF650F5E";
createNode follicle -n "pSphere1FollicleShape2215" -p "pSphere1Follicle2215";
	rename -uid "8F776242-4AD0-C5EE-FCE1-44B1845FF172";
	setAttr -k off ".v";
	setAttr ".pu" 0.21875;
	setAttr ".pv" 0.15625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve47" -p "pSphere1Follicle2215";
	rename -uid "AB5D94BF-4196-5FC6-0784-C3932BA4E183";
createNode nurbsCurve -n "curveShape47" -p "curve47";
	rename -uid "38985D50-4B3F-E8C3-9DAC-598EC27F6AA6";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "08F1F008-48AF-BDD7-D411-E892F6B78257";
createNode follicle -n "pSphere1FollicleShape2222" -p "pSphere1Follicle2222";
	rename -uid "4CF4F2ED-47C7-B5B1-877B-1CB99C20781A";
	setAttr -k off ".v";
	setAttr ".pu" 0.21875;
	setAttr ".pv" 0.21875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve48" -p "pSphere1Follicle2222";
	rename -uid "E2110F95-487E-2DC7-DC8F-49A6B72024B8";
createNode nurbsCurve -n "curveShape48" -p "curve48";
	rename -uid "79F7DA3D-411D-8ACF-A400-FB937ECA67CE";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "A48E3FBA-45CC-A26C-6759-3DAC31E6EEB7";
createNode follicle -n "pSphere1FollicleShape2228" -p "pSphere1Follicle2228";
	rename -uid "4BB25A63-40D5-8C35-D26C-B0AC16225B09";
	setAttr -k off ".v";
	setAttr ".pu" 0.21875;
	setAttr ".pv" 0.28125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve49" -p "pSphere1Follicle2228";
	rename -uid "1AD20B89-45BB-F7ED-F800-1283C9B6D7E7";
createNode nurbsCurve -n "curveShape49" -p "curve49";
	rename -uid "E0C6D094-4501-F64A-14B9-3FBAFCE6C134";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "73B0C389-44D2-D97A-CC3E-E0901A1F83E5";
createNode follicle -n "pSphere1FollicleShape2234" -p "pSphere1Follicle2234";
	rename -uid "08C05778-4CD5-B51B-406A-F6A20C6A5A81";
	setAttr -k off ".v";
	setAttr ".pu" 0.21875;
	setAttr ".pv" 0.34375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve50" -p "pSphere1Follicle2234";
	rename -uid "9C467498-4381-721A-5292-B2AE3C304624";
createNode nurbsCurve -n "curveShape50" -p "curve50";
	rename -uid "566C34F2-4838-DF64-F252-789C6E4B830A";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "60C3BA10-4562-2D5F-4344-A3B39B60444C";
createNode follicle -n "pSphere1FollicleShape2240" -p "pSphere1Follicle2240";
	rename -uid "148E0CF3-45D1-E3A2-36EF-BB8D871173DC";
	setAttr -k off ".v";
	setAttr ".pu" 0.21875;
	setAttr ".pv" 0.40625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve51" -p "pSphere1Follicle2240";
	rename -uid "9F89F99A-449D-4E21-F6D1-DDBBF0ECFFB7";
createNode nurbsCurve -n "curveShape51" -p "curve51";
	rename -uid "CF4FC49C-41C4-C7A1-99AF-098BA6496D0A";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "897A9520-4C1E-803F-6B9D-AFB4A852F487";
createNode follicle -n "pSphere1FollicleShape2246" -p "pSphere1Follicle2246";
	rename -uid "E0BF8F09-437A-BFF3-9696-C6B62EF491C9";
	setAttr -k off ".v";
	setAttr ".pu" 0.21875;
	setAttr ".pv" 0.46875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve52" -p "pSphere1Follicle2246";
	rename -uid "AC6E247E-4B21-1006-BC17-7A83FB6FDC5D";
createNode nurbsCurve -n "curveShape52" -p "curve52";
	rename -uid "AEAB0E45-4857-BB77-E7D9-52A55BCE9379";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "3144A2B8-41F8-1C9D-581A-39ACAAE0A004";
createNode follicle -n "pSphere1FollicleShape2253" -p "pSphere1Follicle2253";
	rename -uid "1BB82772-40F9-C453-1BA1-A18F9B5EC26C";
	setAttr -k off ".v";
	setAttr ".pu" 0.21875;
	setAttr ".pv" 0.53125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve53" -p "pSphere1Follicle2253";
	rename -uid "AAF8412B-46AA-6E6A-865B-3F9413AFFD6E";
createNode nurbsCurve -n "curveShape53" -p "curve53";
	rename -uid "79E8FA7B-43C7-F257-EFE1-37980EF50F12";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "521784F7-4CA6-D15E-AD43-20BDA93558D3";
createNode follicle -n "pSphere1FollicleShape2259" -p "pSphere1Follicle2259";
	rename -uid "960D8AA8-46CB-5256-D811-DE8616256579";
	setAttr -k off ".v";
	setAttr ".pu" 0.21875;
	setAttr ".pv" 0.59375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve54" -p "pSphere1Follicle2259";
	rename -uid "49544011-4BCE-34D7-D1D2-2BB2905BD228";
createNode nurbsCurve -n "curveShape54" -p "curve54";
	rename -uid "623C6B76-4D20-72D0-8B36-4EA55ABCB5DA";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "9EE0B5F7-4006-BF81-1E3A-F89B457159B4";
createNode follicle -n "pSphere1FollicleShape2265" -p "pSphere1Follicle2265";
	rename -uid "B4B3CD8E-4C2A-83C6-5A8C-66AAEB353CF2";
	setAttr -k off ".v";
	setAttr ".pu" 0.21875;
	setAttr ".pv" 0.65625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve55" -p "pSphere1Follicle2265";
	rename -uid "26C7FB50-4C21-09FA-FC9E-90B08080C6C1";
createNode nurbsCurve -n "curveShape55" -p "curve55";
	rename -uid "8216199A-496D-3684-A46F-24A1087FA444";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "C34486F9-4A69-3BA1-664F-8C9A61AE8201";
createNode follicle -n "pSphere1FollicleShape2271" -p "pSphere1Follicle2271";
	rename -uid "CEFD77CE-4818-94CD-AFCC-BB95B62E763D";
	setAttr -k off ".v";
	setAttr ".pu" 0.21875;
	setAttr ".pv" 0.71875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve56" -p "pSphere1Follicle2271";
	rename -uid "4B8E725E-433A-2A99-9770-EDAC28EADA0B";
createNode nurbsCurve -n "curveShape56" -p "curve56";
	rename -uid "BCD7C69C-48FC-D458-0B09-13BE06B93C5D";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "64BAC0B3-4AF2-0FE0-EF28-05BE2C773403";
createNode follicle -n "pSphere1FollicleShape2277" -p "pSphere1Follicle2277";
	rename -uid "A51BFE0A-48C3-B630-EBC8-A99978ABBEFF";
	setAttr -k off ".v";
	setAttr ".pu" 0.21875;
	setAttr ".pv" 0.78125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve57" -p "pSphere1Follicle2277";
	rename -uid "1EF0F030-4AFF-91D5-84DD-E684A821003F";
createNode nurbsCurve -n "curveShape57" -p "curve57";
	rename -uid "B7BA6071-4846-10B8-BF71-70ABDDA51139";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "8B9230F1-47ED-B15A-B185-4C9041086863";
createNode follicle -n "pSphere1FollicleShape2284" -p "pSphere1Follicle2284";
	rename -uid "4E67CE58-4BBD-030B-65CD-969F965667AE";
	setAttr -k off ".v";
	setAttr ".pu" 0.21875;
	setAttr ".pv" 0.84375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve58" -p "pSphere1Follicle2284";
	rename -uid "D49CDAD9-491A-1183-C4DB-E9B4B85788E0";
createNode nurbsCurve -n "curveShape58" -p "curve58";
	rename -uid "884717E6-4006-4EC9-5814-A7AE1B743452";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "846B73CB-458C-8E93-77F7-BC926DC8E9A9";
createNode follicle -n "pSphere1FollicleShape2290" -p "pSphere1Follicle2290";
	rename -uid "EEE44C81-46C6-B09F-CD99-31AF9FF84ED2";
	setAttr -k off ".v";
	setAttr ".pu" 0.21875;
	setAttr ".pv" 0.90625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve59" -p "pSphere1Follicle2290";
	rename -uid "18A70AA0-449D-38E0-F004-BC97C1B25F97";
createNode nurbsCurve -n "curveShape59" -p "curve59";
	rename -uid "F95676C9-40CB-02CB-0C15-30A9FA6E3237";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "5B05D135-4B63-6C4A-1FB9-25A99E7F2A55";
createNode follicle -n "pSphere1FollicleShape2296" -p "pSphere1Follicle2296";
	rename -uid "30070BD1-4E2D-6EA7-53A1-0194AD11F819";
	setAttr -k off ".v";
	setAttr ".pu" 0.21875;
	setAttr ".pv" 0.96875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve60" -p "pSphere1Follicle2296";
	rename -uid "B7FE3235-4A41-8687-7A79-CE87B8725CBA";
createNode nurbsCurve -n "curveShape60" -p "curve60";
	rename -uid "65479A29-4CE3-F84E-F6B8-FF9D48E61270";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "86D75201-4320-0594-A8D1-9D8E7456B791";
createNode follicle -n "pSphere1FollicleShape2803" -p "pSphere1Follicle2803";
	rename -uid "B8E19EF8-405E-A4AC-5C11-E9A1F1A6385B";
	setAttr -k off ".v";
	setAttr ".pu" 0.28125;
	setAttr ".pv" 0.03125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve61" -p "pSphere1Follicle2803";
	rename -uid "75FF10AC-4461-E3FA-08AA-AF9A4C35B4E1";
createNode nurbsCurve -n "curveShape61" -p "curve61";
	rename -uid "77225278-4A46-E165-367A-E08CD1EFAF93";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "2C968578-4EB2-9A50-BD84-C998245A378E";
createNode follicle -n "pSphere1FollicleShape2809" -p "pSphere1Follicle2809";
	rename -uid "4FFB8092-405A-D10F-9D95-D9947D0F62B7";
	setAttr -k off ".v";
	setAttr ".pu" 0.28125;
	setAttr ".pv" 0.09375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve62" -p "pSphere1Follicle2809";
	rename -uid "20885719-4003-9B1C-86AC-CF9556890C19";
createNode nurbsCurve -n "curveShape62" -p "curve62";
	rename -uid "BB934CBF-41C2-B7CF-4E01-CCBB845B693C";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "FF14FB8E-4354-E9AF-4B3E-43A44B6018D6";
createNode follicle -n "pSphere1FollicleShape2815" -p "pSphere1Follicle2815";
	rename -uid "038B9183-4C19-53A2-565A-4886DF9E8110";
	setAttr -k off ".v";
	setAttr ".pu" 0.28125;
	setAttr ".pv" 0.15625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve63" -p "pSphere1Follicle2815";
	rename -uid "063AAD1A-4DA1-C4DB-44DF-528176877AC3";
createNode nurbsCurve -n "curveShape63" -p "curve63";
	rename -uid "1777D419-4CCE-6600-880E-D28908C363BD";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "59909156-4939-E898-566E-919B916FE2BF";
createNode follicle -n "pSphere1FollicleShape2822" -p "pSphere1Follicle2822";
	rename -uid "CFF15714-442A-3CF8-21E8-A99A3AC08938";
	setAttr -k off ".v";
	setAttr ".pu" 0.28125;
	setAttr ".pv" 0.21875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve64" -p "pSphere1Follicle2822";
	rename -uid "B45AF02F-4092-81C0-2D84-4BBE853565DA";
createNode nurbsCurve -n "curveShape64" -p "curve64";
	rename -uid "971A1655-4491-1A24-F7C5-59A28261549F";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "E1FC89DB-4DBA-77AE-2AB2-4C84FE4C1C47";
createNode follicle -n "pSphere1FollicleShape2828" -p "pSphere1Follicle2828";
	rename -uid "78E03CA5-442F-E0AC-A722-9AAF037EE04D";
	setAttr -k off ".v";
	setAttr ".pu" 0.28125;
	setAttr ".pv" 0.28125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve65" -p "pSphere1Follicle2828";
	rename -uid "9842461A-4F54-DDFB-652A-73A229B8A244";
createNode nurbsCurve -n "curveShape65" -p "curve65";
	rename -uid "79FD142D-465A-CA3E-01CC-A7BD892759BC";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "4B5CCE55-4EDA-F60F-52B9-B4AB0124EF39";
createNode follicle -n "pSphere1FollicleShape2834" -p "pSphere1Follicle2834";
	rename -uid "51D5D3FD-4BBC-DCD0-3466-7C913328576B";
	setAttr -k off ".v";
	setAttr ".pu" 0.28125;
	setAttr ".pv" 0.34375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve66" -p "pSphere1Follicle2834";
	rename -uid "E15F2531-4CCA-967D-4BB2-2582CDD91074";
createNode nurbsCurve -n "curveShape66" -p "curve66";
	rename -uid "FD7B39F3-4BB9-E1A0-FB5B-8DB0DEA98EC0";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "67CC5727-4E6C-3C0D-81A9-E5B4DDCD640A";
createNode follicle -n "pSphere1FollicleShape2840" -p "pSphere1Follicle2840";
	rename -uid "48997DAF-4614-DCB8-9344-43B9C9CC5ED6";
	setAttr -k off ".v";
	setAttr ".pu" 0.28125;
	setAttr ".pv" 0.40625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve67" -p "pSphere1Follicle2840";
	rename -uid "CB3F20D4-4D77-AF0A-7675-9EB200BCB3E6";
createNode nurbsCurve -n "curveShape67" -p "curve67";
	rename -uid "80E7E864-4E72-1791-1B93-FC9768A93C91";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "AC2E3F57-46EC-240A-6016-3AA82EA12F7C";
createNode follicle -n "pSphere1FollicleShape2846" -p "pSphere1Follicle2846";
	rename -uid "060CCE91-4731-96C3-09C7-C5904B822C57";
	setAttr -k off ".v";
	setAttr ".pu" 0.28125;
	setAttr ".pv" 0.46875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve68" -p "pSphere1Follicle2846";
	rename -uid "EFD7931A-43F2-3487-82DE-6EA024BAADA7";
createNode nurbsCurve -n "curveShape68" -p "curve68";
	rename -uid "27F727BB-4365-A89A-8F40-1DAD6A954DF5";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "09C8BB18-499F-234F-8FDA-52B187EAB6C0";
createNode follicle -n "pSphere1FollicleShape2853" -p "pSphere1Follicle2853";
	rename -uid "5C743363-4580-DA6F-8606-86B779B07D4A";
	setAttr -k off ".v";
	setAttr ".pu" 0.28125;
	setAttr ".pv" 0.53125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve69" -p "pSphere1Follicle2853";
	rename -uid "B3CD73A7-4AC0-613E-2BEE-90B363D5B1A5";
createNode nurbsCurve -n "curveShape69" -p "curve69";
	rename -uid "94C55113-4EC8-D0FE-0838-5BBC0E31697A";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "6A15DD66-4A91-C1D7-AF4D-0EB609BEDB45";
createNode follicle -n "pSphere1FollicleShape2859" -p "pSphere1Follicle2859";
	rename -uid "2263322A-4180-4132-5C3C-52B2F27A6881";
	setAttr -k off ".v";
	setAttr ".pu" 0.28125;
	setAttr ".pv" 0.59375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve70" -p "pSphere1Follicle2859";
	rename -uid "477CB018-4D34-A10E-FC61-84AC453DB961";
createNode nurbsCurve -n "curveShape70" -p "curve70";
	rename -uid "E0DF6534-4FCE-38BF-0FDB-84B4587583D3";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "6B79E529-4E6F-A1F9-9FBA-5AB98DF650A1";
createNode follicle -n "pSphere1FollicleShape2865" -p "pSphere1Follicle2865";
	rename -uid "30496001-4BE6-CF19-CDCD-5984B68DF61C";
	setAttr -k off ".v";
	setAttr ".pu" 0.28125;
	setAttr ".pv" 0.65625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve71" -p "pSphere1Follicle2865";
	rename -uid "56D1AD3D-4B45-7FFE-EE25-4DAD8FC9F46E";
createNode nurbsCurve -n "curveShape71" -p "curve71";
	rename -uid "3566C505-44CC-9D86-6646-4DA38FF1455B";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "7DA18DBC-405C-6448-AE61-F7BFE8203BA4";
createNode follicle -n "pSphere1FollicleShape2871" -p "pSphere1Follicle2871";
	rename -uid "C8FAA4D2-443B-C264-9400-84838D0E4A17";
	setAttr -k off ".v";
	setAttr ".pu" 0.28125;
	setAttr ".pv" 0.71875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve72" -p "pSphere1Follicle2871";
	rename -uid "C36EEF99-4CC8-5D8B-6424-9586E0623322";
createNode nurbsCurve -n "curveShape72" -p "curve72";
	rename -uid "BF13CFAD-4A46-C9A1-23EC-3497FBF8365C";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "00162C27-4F6C-41EE-56D9-4C8FDA8902D4";
createNode follicle -n "pSphere1FollicleShape2877" -p "pSphere1Follicle2877";
	rename -uid "07CD5B6D-4DA9-D30E-AC03-05A9B55E8D33";
	setAttr -k off ".v";
	setAttr ".pu" 0.28125;
	setAttr ".pv" 0.78125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve73" -p "pSphere1Follicle2877";
	rename -uid "DA6C1373-45D0-0AC9-7EDB-329192D33C41";
createNode nurbsCurve -n "curveShape73" -p "curve73";
	rename -uid "1846B505-406F-D3CB-A4A7-0CA2B7B5F3F9";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "6F9284C0-46CB-421F-22A6-4DAA3C2F8983";
createNode follicle -n "pSphere1FollicleShape2884" -p "pSphere1Follicle2884";
	rename -uid "CC8D2F21-4DB6-C234-BA25-489D8EF338E8";
	setAttr -k off ".v";
	setAttr ".pu" 0.28125;
	setAttr ".pv" 0.84375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve74" -p "pSphere1Follicle2884";
	rename -uid "6E5C8B4F-4EE4-179F-C340-E2AFBB58287F";
createNode nurbsCurve -n "curveShape74" -p "curve74";
	rename -uid "6A57BED5-45B7-4C3B-D0CC-F398A22EA67F";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "88C59F0F-4BE6-598C-298D-39A9454A6019";
createNode follicle -n "pSphere1FollicleShape2890" -p "pSphere1Follicle2890";
	rename -uid "AB94A50F-43CD-5BD3-15BB-24AEEE3D904A";
	setAttr -k off ".v";
	setAttr ".pu" 0.28125;
	setAttr ".pv" 0.90625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve75" -p "pSphere1Follicle2890";
	rename -uid "F29FDFC7-4EF7-E11F-B72C-ACBED90BD714";
createNode nurbsCurve -n "curveShape75" -p "curve75";
	rename -uid "C08BC1B0-4EAF-BDCF-81EB-1D8BEB9E96B3";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "AB65C09C-4A15-B37B-512C-E6AD258E958F";
createNode follicle -n "pSphere1FollicleShape2896" -p "pSphere1Follicle2896";
	rename -uid "4AF40DE5-48E8-1DB2-66CC-0CBAAEDE6851";
	setAttr -k off ".v";
	setAttr ".pu" 0.28125;
	setAttr ".pv" 0.96875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve76" -p "pSphere1Follicle2896";
	rename -uid "E1C7928B-4CD3-7984-DAD2-A88D99541353";
createNode nurbsCurve -n "curveShape76" -p "curve76";
	rename -uid "5CF60E2B-4EB9-CD27-4948-D99D54B1F918";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "CFBBF94F-4FF1-4BF4-CF54-6EAE8FF864EC";
createNode follicle -n "pSphere1FollicleShape3409" -p "pSphere1Follicle3409";
	rename -uid "B484712F-4946-62D2-7816-D4B02E09AFF6";
	setAttr -k off ".v";
	setAttr ".pu" 0.34375;
	setAttr ".pv" 0.09375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve77" -p "pSphere1Follicle3409";
	rename -uid "14612802-4610-32C1-8302-A88ECDF664F9";
createNode nurbsCurve -n "curveShape77" -p "curve77";
	rename -uid "C0CC5ACD-46CD-503B-841E-D0A7F893CE35";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "5660BF69-4C52-F1E0-3B03-5CB68E72277A";
createNode follicle -n "pSphere1FollicleShape3415" -p "pSphere1Follicle3415";
	rename -uid "5D6A5120-4196-809B-9EB5-808123773289";
	setAttr -k off ".v";
	setAttr ".pu" 0.34375;
	setAttr ".pv" 0.15625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve78" -p "pSphere1Follicle3415";
	rename -uid "9C3774A7-4D1E-70D2-EECE-47AA0D563FCA";
createNode nurbsCurve -n "curveShape78" -p "curve78";
	rename -uid "519B980C-4D0B-6D4A-014B-08AE31BA3F8F";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "E3C69028-40BE-F571-4FDD-17A8F76DA9EA";
createNode follicle -n "pSphere1FollicleShape3422" -p "pSphere1Follicle3422";
	rename -uid "02C696FA-4AB9-2985-C670-25BFCB16E5F9";
	setAttr -k off ".v";
	setAttr ".pu" 0.34375;
	setAttr ".pv" 0.21875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve79" -p "pSphere1Follicle3422";
	rename -uid "BA8208FE-47DB-08F5-6DC6-2393C4E8F4D8";
createNode nurbsCurve -n "curveShape79" -p "curve79";
	rename -uid "6A57C80E-44F2-6EF4-FC5D-48A89B49F39E";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "3E33FA50-498B-B047-E78F-7DBB75BADA87";
createNode follicle -n "pSphere1FollicleShape3428" -p "pSphere1Follicle3428";
	rename -uid "479ACC01-4E81-CFE3-7DB2-B0ACC52AFB83";
	setAttr -k off ".v";
	setAttr ".pu" 0.34375;
	setAttr ".pv" 0.28125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve80" -p "pSphere1Follicle3428";
	rename -uid "3FF48C1C-4C97-AA3C-A69E-81BF8BB51B47";
createNode nurbsCurve -n "curveShape80" -p "curve80";
	rename -uid "72E76018-41DC-A4E1-336B-E78B16C0D5BC";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "3B5BEF50-498B-62E2-ADFE-55B90E555424";
createNode follicle -n "pSphere1FollicleShape3434" -p "pSphere1Follicle3434";
	rename -uid "9A4FF572-4852-0E2F-BFAA-E89321466407";
	setAttr -k off ".v";
	setAttr ".pu" 0.34375;
	setAttr ".pv" 0.34375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve81" -p "pSphere1Follicle3434";
	rename -uid "F6269A6A-4712-8F22-B725-4094466856EE";
createNode nurbsCurve -n "curveShape81" -p "curve81";
	rename -uid "FA4867F1-4971-D8DE-CF37-7DB44B6EB7AB";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "EE34BDC1-4E18-9334-AEB4-6AA44D88B818";
createNode follicle -n "pSphere1FollicleShape3440" -p "pSphere1Follicle3440";
	rename -uid "C54AAF65-4522-CD1B-3D4B-6CB9E6BB531D";
	setAttr -k off ".v";
	setAttr ".pu" 0.34375;
	setAttr ".pv" 0.40625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve82" -p "pSphere1Follicle3440";
	rename -uid "644C2FB7-49D4-6695-7072-B2B02BDEBF6C";
createNode nurbsCurve -n "curveShape82" -p "curve82";
	rename -uid "0A48590A-444B-B680-26B8-A1ADF74833C3";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "87AE22FC-4988-7940-E2C3-2EBD0E12D8FE";
createNode follicle -n "pSphere1FollicleShape3446" -p "pSphere1Follicle3446";
	rename -uid "68BCB31C-4AE1-724F-55EB-F2929A6A38DC";
	setAttr -k off ".v";
	setAttr ".pu" 0.34375;
	setAttr ".pv" 0.46875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve83" -p "pSphere1Follicle3446";
	rename -uid "04EC9B5B-4852-5168-D1E2-1B829E051B49";
createNode nurbsCurve -n "curveShape83" -p "curve83";
	rename -uid "A48CA41B-4F72-1BAC-A7C1-67BA486CFF57";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "43C2F635-4DD8-82A2-7302-AEA644FD010E";
createNode follicle -n "pSphere1FollicleShape3453" -p "pSphere1Follicle3453";
	rename -uid "5F4646A8-49B6-32C9-FF67-38B482B8A14E";
	setAttr -k off ".v";
	setAttr ".pu" 0.34375;
	setAttr ".pv" 0.53125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve84" -p "pSphere1Follicle3453";
	rename -uid "5C26ABB8-4D69-12B8-EB9F-A99985E70945";
createNode nurbsCurve -n "curveShape84" -p "curve84";
	rename -uid "E5D71A53-47C3-9F28-1CF9-AAB9EBD01E15";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "E7BC6FF1-44B7-27F3-B545-34B39AAB5334";
createNode follicle -n "pSphere1FollicleShape3459" -p "pSphere1Follicle3459";
	rename -uid "8412923F-4889-A53C-C37E-54AC7E0C7919";
	setAttr -k off ".v";
	setAttr ".pu" 0.34375;
	setAttr ".pv" 0.59375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve85" -p "pSphere1Follicle3459";
	rename -uid "615DBEA2-4BD3-6242-B09D-AC9FDEE2FC49";
createNode nurbsCurve -n "curveShape85" -p "curve85";
	rename -uid "4AB355CE-4C68-EFE5-4E85-94B562736DED";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "F3FF0D36-40D6-ED65-C068-A5B9F5439840";
createNode follicle -n "pSphere1FollicleShape3465" -p "pSphere1Follicle3465";
	rename -uid "E5325515-478D-4134-68DB-3FB356F84FFF";
	setAttr -k off ".v";
	setAttr ".pu" 0.34375;
	setAttr ".pv" 0.65625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve86" -p "pSphere1Follicle3465";
	rename -uid "DFE4EE5D-43F6-4BAB-7B41-A691586065F2";
createNode nurbsCurve -n "curveShape86" -p "curve86";
	rename -uid "A77E1F43-434A-37C1-6C2F-FE9ECC5DD9EE";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "C32FC0C5-4A5F-9672-B977-0E88B71E70B5";
createNode follicle -n "pSphere1FollicleShape3471" -p "pSphere1Follicle3471";
	rename -uid "C089528A-42F3-FBE8-550C-A8A22629D2BA";
	setAttr -k off ".v";
	setAttr ".pu" 0.34375;
	setAttr ".pv" 0.71875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve87" -p "pSphere1Follicle3471";
	rename -uid "FE94A51B-44FF-2234-43BB-B0888957B017";
createNode nurbsCurve -n "curveShape87" -p "curve87";
	rename -uid "DD3B5ADB-4414-0573-363C-C78DA3507EBA";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "DDC88DAA-43AD-421C-7AA9-4FBBBF9BD38F";
createNode follicle -n "pSphere1FollicleShape3477" -p "pSphere1Follicle3477";
	rename -uid "BDEA3405-40E4-A30C-1B14-96A460EC63FA";
	setAttr -k off ".v";
	setAttr ".pu" 0.34375;
	setAttr ".pv" 0.78125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve88" -p "pSphere1Follicle3477";
	rename -uid "21A1F28F-45A7-1AFE-BB01-6897ACC8BC81";
createNode nurbsCurve -n "curveShape88" -p "curve88";
	rename -uid "75DD7AFC-40B5-84BB-FDDD-96BED22227A7";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "6A58E6F8-4D91-2EE9-7E65-5FBBD1ED03C3";
createNode follicle -n "pSphere1FollicleShape3484" -p "pSphere1Follicle3484";
	rename -uid "AB941C56-47C0-CAE8-F637-02A2E8FD08DC";
	setAttr -k off ".v";
	setAttr ".pu" 0.34375;
	setAttr ".pv" 0.84375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve89" -p "pSphere1Follicle3484";
	rename -uid "78891EE0-4728-0DF3-64DA-46BF0435DFB8";
createNode nurbsCurve -n "curveShape89" -p "curve89";
	rename -uid "E509034A-4337-AA01-6F66-179408E4153C";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "00D123CA-4FB9-1692-360F-E3AD913F3D04";
createNode follicle -n "pSphere1FollicleShape3490" -p "pSphere1Follicle3490";
	rename -uid "F5989E72-4A0D-C5DB-78C6-6CB166BE2854";
	setAttr -k off ".v";
	setAttr ".pu" 0.34375;
	setAttr ".pv" 0.90625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve90" -p "pSphere1Follicle3490";
	rename -uid "73CEF38F-44F8-9684-12A2-65B1C02DE5F3";
createNode nurbsCurve -n "curveShape90" -p "curve90";
	rename -uid "05FB0FC9-4957-4BC2-9178-379D669A3706";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "E87C9E70-488C-45E3-2461-768EA8DED1E3";
createNode follicle -n "pSphere1FollicleShape4009" -p "pSphere1Follicle4009";
	rename -uid "0569DE01-46EA-F9D9-5626-E1BEF406F2D0";
	setAttr -k off ".v";
	setAttr ".pu" 0.40625;
	setAttr ".pv" 0.09375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve91" -p "pSphere1Follicle4009";
	rename -uid "CB7C8F27-466D-483C-9B99-BCB9528641BA";
createNode nurbsCurve -n "curveShape91" -p "curve91";
	rename -uid "D77E7661-4D05-9316-DC07-278D4B2FD82A";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "58E49595-4191-0DD1-3110-72815E02C33B";
createNode follicle -n "pSphere1FollicleShape4015" -p "pSphere1Follicle4015";
	rename -uid "26978ED3-40A1-6D21-5F12-90BCCAB32400";
	setAttr -k off ".v";
	setAttr ".pu" 0.40625;
	setAttr ".pv" 0.15625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve92" -p "pSphere1Follicle4015";
	rename -uid "C3167A7C-4265-D789-A9F9-F4B3B00B03A9";
createNode nurbsCurve -n "curveShape92" -p "curve92";
	rename -uid "3BA48C21-4FDD-71AA-87C1-D996237F8E8B";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "FDD82241-4E18-E4A6-FE5F-3F8617F2F145";
createNode follicle -n "pSphere1FollicleShape4022" -p "pSphere1Follicle4022";
	rename -uid "5583E0F2-43B3-A57D-B362-6889146BD22F";
	setAttr -k off ".v";
	setAttr ".pu" 0.40625;
	setAttr ".pv" 0.21875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve93" -p "pSphere1Follicle4022";
	rename -uid "51516BF7-4162-AA0F-94FC-588D97DB5966";
createNode nurbsCurve -n "curveShape93" -p "curve93";
	rename -uid "9AEC84E2-44A2-EF88-114E-95AD7C6F8755";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "63D77317-4B23-A4CC-8F82-87B0489CC608";
createNode follicle -n "pSphere1FollicleShape4028" -p "pSphere1Follicle4028";
	rename -uid "AD225406-4D5B-A967-E4FB-268C43E67140";
	setAttr -k off ".v";
	setAttr ".pu" 0.40625;
	setAttr ".pv" 0.28125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve94" -p "pSphere1Follicle4028";
	rename -uid "85AD657E-4A3A-3BC0-0947-1D91B41C2C9E";
createNode nurbsCurve -n "curveShape94" -p "curve94";
	rename -uid "BB3A45D2-4AC5-1C7B-C34F-E28A6B4AD0E3";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "2591DBC1-45D8-06B2-1152-819BEF3F626A";
createNode follicle -n "pSphere1FollicleShape4034" -p "pSphere1Follicle4034";
	rename -uid "D83B17B9-48A0-AB46-3341-16A4F422303D";
	setAttr -k off ".v";
	setAttr ".pu" 0.40625;
	setAttr ".pv" 0.34375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve95" -p "pSphere1Follicle4034";
	rename -uid "FA5C0D42-44C2-5B2C-DAFC-C9A04EB2CBE3";
createNode nurbsCurve -n "curveShape95" -p "curve95";
	rename -uid "910F7DDD-44A4-91B7-4EEA-B48072AA3E0B";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "1C2EA519-4366-774D-7913-509EAED9D7CC";
createNode follicle -n "pSphere1FollicleShape4040" -p "pSphere1Follicle4040";
	rename -uid "63CECEC6-4C5C-BE97-94CD-428EFA8D8D0F";
	setAttr -k off ".v";
	setAttr ".pu" 0.40625;
	setAttr ".pv" 0.40625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve96" -p "pSphere1Follicle4040";
	rename -uid "42BC1802-459E-FA6D-EE00-749E360903B6";
createNode nurbsCurve -n "curveShape96" -p "curve96";
	rename -uid "6C623F93-4A04-1890-ED2E-14A38F526333";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "20619BB7-49B8-6774-5246-2B9AEEC11AFF";
createNode follicle -n "pSphere1FollicleShape4046" -p "pSphere1Follicle4046";
	rename -uid "FBA7F900-477F-B652-6194-0E97116F11D6";
	setAttr -k off ".v";
	setAttr ".pu" 0.40625;
	setAttr ".pv" 0.46875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve97" -p "pSphere1Follicle4046";
	rename -uid "B151FFD2-481F-B8FF-2CDA-4FADE1E2BF47";
createNode nurbsCurve -n "curveShape97" -p "curve97";
	rename -uid "8B2AA001-418A-60C3-4223-CAA601F9D815";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "31A3ABD6-4E80-5CF1-0CB1-27A29C3C2099";
createNode follicle -n "pSphere1FollicleShape4053" -p "pSphere1Follicle4053";
	rename -uid "B922EC0D-4449-0890-A316-94B74D9D1DF3";
	setAttr -k off ".v";
	setAttr ".pu" 0.40625;
	setAttr ".pv" 0.53125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve98" -p "pSphere1Follicle4053";
	rename -uid "15D4F702-4F40-5A1C-DBE3-AA94E968FA97";
createNode nurbsCurve -n "curveShape98" -p "curve98";
	rename -uid "2E3EFC66-4B82-3A2A-6309-10B36668447F";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "74A75C0A-40A2-EF09-0451-72A45B20AC22";
createNode follicle -n "pSphere1FollicleShape4059" -p "pSphere1Follicle4059";
	rename -uid "97871C67-4F02-7803-69C0-55BD908A9539";
	setAttr -k off ".v";
	setAttr ".pu" 0.40625;
	setAttr ".pv" 0.59375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve99" -p "pSphere1Follicle4059";
	rename -uid "756BB008-44EF-2E31-9FFC-7593535EF1AA";
createNode nurbsCurve -n "curveShape99" -p "curve99";
	rename -uid "33199D1D-43E5-9E22-8E78-BE833368988E";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "2D3E4B2C-4E34-E5B0-A252-5C906E464FEC";
createNode follicle -n "pSphere1FollicleShape4065" -p "pSphere1Follicle4065";
	rename -uid "68F167DC-4FA1-1E54-2D6B-719E2E081F8D";
	setAttr -k off ".v";
	setAttr ".pu" 0.40625;
	setAttr ".pv" 0.65625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve100" -p "pSphere1Follicle4065";
	rename -uid "07FD498E-4D7B-F798-C178-AEBBBACD9541";
createNode nurbsCurve -n "curveShape100" -p "curve100";
	rename -uid "698210EC-4C31-21F7-8D6D-1A9D802AAC09";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "3D51DC37-47EE-8D9E-DBB5-35B02066D05B";
createNode follicle -n "pSphere1FollicleShape4071" -p "pSphere1Follicle4071";
	rename -uid "C9A8DECC-4092-F47C-441D-498E1F3C8BC8";
	setAttr -k off ".v";
	setAttr ".pu" 0.40625;
	setAttr ".pv" 0.71875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve101" -p "pSphere1Follicle4071";
	rename -uid "C7C57C3E-4B7E-D036-C444-CF92976EF808";
createNode nurbsCurve -n "curveShape101" -p "curve101";
	rename -uid "ABECCC5D-49C4-DBDA-863A-0C90B0CDC66D";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "02FF8E42-4504-13A7-2B47-399360D5C351";
createNode follicle -n "pSphere1FollicleShape4077" -p "pSphere1Follicle4077";
	rename -uid "99724241-43C4-0DEA-A976-6E983DB044C4";
	setAttr -k off ".v";
	setAttr ".pu" 0.40625;
	setAttr ".pv" 0.78125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve102" -p "pSphere1Follicle4077";
	rename -uid "619C2A2C-48C0-CA03-554A-F3A926B8F1EF";
createNode nurbsCurve -n "curveShape102" -p "curve102";
	rename -uid "A0A763B3-405D-84C1-7FBD-DAAA538409CD";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "255B0B80-497F-A5CE-09CC-3895184E6347";
createNode follicle -n "pSphere1FollicleShape4084" -p "pSphere1Follicle4084";
	rename -uid "923CD435-41BE-3EB0-A3DD-558AD07529B2";
	setAttr -k off ".v";
	setAttr ".pu" 0.40625;
	setAttr ".pv" 0.84375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve103" -p "pSphere1Follicle4084";
	rename -uid "A55CFC5E-42D8-4138-5D4A-19B4D4D93680";
createNode nurbsCurve -n "curveShape103" -p "curve103";
	rename -uid "988A390E-4E44-C536-EECB-7F91DEF512A2";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "4D5E594B-4831-B865-79E2-4D8CCD42B569";
createNode follicle -n "pSphere1FollicleShape4090" -p "pSphere1Follicle4090";
	rename -uid "2D87C3E2-494F-3494-0893-4093EB065085";
	setAttr -k off ".v";
	setAttr ".pu" 0.40625;
	setAttr ".pv" 0.90625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve104" -p "pSphere1Follicle4090";
	rename -uid "3BD77BDF-4621-3BDE-201D-FDAF54885E9A";
createNode nurbsCurve -n "curveShape104" -p "curve104";
	rename -uid "DD652F3C-4983-0D96-00E2-4EAEC1094682";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "D559EFD5-4830-71CF-0ABC-4384625D2AF0";
createNode follicle -n "pSphere1FollicleShape4603" -p "pSphere1Follicle4603";
	rename -uid "0BCCE343-4B8C-6C86-81E8-FEB54444631A";
	setAttr -k off ".v";
	setAttr ".pu" 0.46875;
	setAttr ".pv" 0.03125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve105" -p "pSphere1Follicle4603";
	rename -uid "76FAD1BC-4336-AC16-6887-A983989232E3";
createNode nurbsCurve -n "curveShape105" -p "curve105";
	rename -uid "FF84BA8C-4C65-1D1F-6659-2BA4D215E8D4";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "07D2F77A-4080-7565-466A-67B83D9BD87E";
createNode follicle -n "pSphere1FollicleShape4609" -p "pSphere1Follicle4609";
	rename -uid "F78C5F2E-4192-AE33-0EAF-D7B7047BEE18";
	setAttr -k off ".v";
	setAttr ".pu" 0.46875;
	setAttr ".pv" 0.09375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve106" -p "pSphere1Follicle4609";
	rename -uid "25989330-49D5-CC0F-5C65-72A063C7DFC5";
createNode nurbsCurve -n "curveShape106" -p "curve106";
	rename -uid "511F6491-4488-CCD1-2F8D-20B9D8065CF9";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "0485457B-4673-8DC7-155C-B688B27B6149";
createNode follicle -n "pSphere1FollicleShape4615" -p "pSphere1Follicle4615";
	rename -uid "2A023592-4635-EA84-DA03-5F9D43C37382";
	setAttr -k off ".v";
	setAttr ".pu" 0.46875;
	setAttr ".pv" 0.15625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve107" -p "pSphere1Follicle4615";
	rename -uid "2882AD27-4910-4588-638E-548FB674BB79";
createNode nurbsCurve -n "curveShape107" -p "curve107";
	rename -uid "0195A61A-47A0-CA7F-FC01-A6BA98EECF71";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "3B7805CA-4B5C-A472-FD17-029B87D3D191";
createNode follicle -n "pSphere1FollicleShape4622" -p "pSphere1Follicle4622";
	rename -uid "98E2C39E-4970-18C6-1F02-ED99F2AFBA3E";
	setAttr -k off ".v";
	setAttr ".pu" 0.46875;
	setAttr ".pv" 0.21875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve108" -p "pSphere1Follicle4622";
	rename -uid "38E02045-4789-2344-3C90-02B274B62744";
createNode nurbsCurve -n "curveShape108" -p "curve108";
	rename -uid "2E44719A-4B45-3CA1-7F8D-1698623A4D01";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "EA3A9640-44C4-0B45-5F64-A9A22526486D";
createNode follicle -n "pSphere1FollicleShape4628" -p "pSphere1Follicle4628";
	rename -uid "8C8B77B7-48BB-493D-4126-E9BA8C58CD89";
	setAttr -k off ".v";
	setAttr ".pu" 0.46875;
	setAttr ".pv" 0.28125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve109" -p "pSphere1Follicle4628";
	rename -uid "1ED7B336-4B78-FC49-E5E5-149BB3B8C740";
createNode nurbsCurve -n "curveShape109" -p "curve109";
	rename -uid "2A93E5EE-4EF4-D54B-602F-769E37592A21";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "C9B9AFB5-4165-849D-F77C-FF84817A8B25";
createNode follicle -n "pSphere1FollicleShape4634" -p "pSphere1Follicle4634";
	rename -uid "8620C66C-41BD-AE08-6E2C-1FBD7C3DD89E";
	setAttr -k off ".v";
	setAttr ".pu" 0.46875;
	setAttr ".pv" 0.34375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve110" -p "pSphere1Follicle4634";
	rename -uid "EE3A02B3-4E09-98AA-7140-5B8805334F7B";
createNode nurbsCurve -n "curveShape110" -p "curve110";
	rename -uid "5086A78A-4AF0-2657-0F13-CFABE73D1F71";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "604D895A-4B3E-20BE-9A92-888B61EEE667";
createNode follicle -n "pSphere1FollicleShape4640" -p "pSphere1Follicle4640";
	rename -uid "D6315436-44BC-5115-6184-8DB2C4097C6B";
	setAttr -k off ".v";
	setAttr ".pu" 0.46875;
	setAttr ".pv" 0.40625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve111" -p "pSphere1Follicle4640";
	rename -uid "B820AFDF-45A1-A6B1-A8AD-359FBA6EA938";
createNode nurbsCurve -n "curveShape111" -p "curve111";
	rename -uid "EDEFD18F-4AB8-1111-DF38-F1894FE72398";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "E9597235-4D2D-4583-3AC8-4EA8E4601275";
createNode follicle -n "pSphere1FollicleShape4646" -p "pSphere1Follicle4646";
	rename -uid "4FC28687-41E7-B567-86F8-C7A98666D99D";
	setAttr -k off ".v";
	setAttr ".pu" 0.46875;
	setAttr ".pv" 0.46875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve112" -p "pSphere1Follicle4646";
	rename -uid "87775B16-4F29-9A78-2705-2988FD6C95AB";
createNode nurbsCurve -n "curveShape112" -p "curve112";
	rename -uid "6D012412-41E5-E418-E081-018959A16AB9";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "AD93180C-40E2-FB28-21CA-13B92CC7675F";
createNode follicle -n "pSphere1FollicleShape4653" -p "pSphere1Follicle4653";
	rename -uid "B0EEC595-4784-480F-7850-05A35A93FF1D";
	setAttr -k off ".v";
	setAttr ".pu" 0.46875;
	setAttr ".pv" 0.53125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve113" -p "pSphere1Follicle4653";
	rename -uid "983BF978-4F5D-39B2-B89A-86920CD13CCD";
createNode nurbsCurve -n "curveShape113" -p "curve113";
	rename -uid "D840C348-4651-05E0-9DC2-629557720124";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "A2AA8B4D-4F5F-2ACD-5DD7-F28DCD075155";
createNode follicle -n "pSphere1FollicleShape4659" -p "pSphere1Follicle4659";
	rename -uid "66BBB5EE-4AA8-6371-349C-9482495C2D35";
	setAttr -k off ".v";
	setAttr ".pu" 0.46875;
	setAttr ".pv" 0.59375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve114" -p "pSphere1Follicle4659";
	rename -uid "274923EA-4646-8BB5-3518-D6A3F107A41F";
createNode nurbsCurve -n "curveShape114" -p "curve114";
	rename -uid "710A8917-43EF-4811-9111-9BA49BFEEA8E";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "9B1AC773-483C-5564-6713-EBB2A9D794E1";
createNode follicle -n "pSphere1FollicleShape4665" -p "pSphere1Follicle4665";
	rename -uid "861FEC39-4195-F017-6EDE-5D97B1D44D58";
	setAttr -k off ".v";
	setAttr ".pu" 0.46875;
	setAttr ".pv" 0.65625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve115" -p "pSphere1Follicle4665";
	rename -uid "6D21A0DA-4C80-B49F-B6FF-73A2F89440C7";
createNode nurbsCurve -n "curveShape115" -p "curve115";
	rename -uid "956A9300-41BB-B06B-CE63-3BAC46E7B8E3";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "340F99B3-4B47-2D35-676C-FDB9FFE20614";
createNode follicle -n "pSphere1FollicleShape4671" -p "pSphere1Follicle4671";
	rename -uid "BB406EB0-40F0-F200-992C-61AD89518535";
	setAttr -k off ".v";
	setAttr ".pu" 0.46875;
	setAttr ".pv" 0.71875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve116" -p "pSphere1Follicle4671";
	rename -uid "171AF78D-4F4F-2FE6-5CAF-09847AA43B14";
createNode nurbsCurve -n "curveShape116" -p "curve116";
	rename -uid "A0185163-47F1-9D8E-B529-DF9DE98848C7";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "3C81853A-4AD2-0039-BABE-B884AE0449EC";
createNode follicle -n "pSphere1FollicleShape4677" -p "pSphere1Follicle4677";
	rename -uid "99FD21DF-4182-552E-490B-A587765897D9";
	setAttr -k off ".v";
	setAttr ".pu" 0.46875;
	setAttr ".pv" 0.78125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve117" -p "pSphere1Follicle4677";
	rename -uid "CB193A90-4858-52D4-BD26-AA98A5D7C6BE";
createNode nurbsCurve -n "curveShape117" -p "curve117";
	rename -uid "CEE6B76C-484F-50BE-15D8-749743CEA4F8";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "F977EA8D-4B1B-1B67-3FBA-59A41F15088E";
createNode follicle -n "pSphere1FollicleShape4684" -p "pSphere1Follicle4684";
	rename -uid "47A190A2-4722-9187-5FD0-728DF15403C0";
	setAttr -k off ".v";
	setAttr ".pu" 0.46875;
	setAttr ".pv" 0.84375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve118" -p "pSphere1Follicle4684";
	rename -uid "D24AF67E-4613-D2F2-69D7-5880F3EF8C4B";
createNode nurbsCurve -n "curveShape118" -p "curve118";
	rename -uid "9E216049-4A0B-5DF6-4086-C9A339F1F2D2";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "4AF2411A-4DE6-8062-CF6F-4380798E8B4D";
createNode follicle -n "pSphere1FollicleShape4690" -p "pSphere1Follicle4690";
	rename -uid "4710FCFB-4E45-50E6-E552-8AB9A2C620CC";
	setAttr -k off ".v";
	setAttr ".pu" 0.46875;
	setAttr ".pv" 0.90625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve119" -p "pSphere1Follicle4690";
	rename -uid "CC6CAB7B-40A8-1BA8-E8EE-0BA97C7A4604";
createNode nurbsCurve -n "curveShape119" -p "curve119";
	rename -uid "90416FE5-4D33-4A99-8A5E-9FA619AA2F09";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "71B9FB8C-4C92-FDA5-D2E1-8CA60BE13BD8";
createNode follicle -n "pSphere1FollicleShape4696" -p "pSphere1Follicle4696";
	rename -uid "D1D1695D-4EC1-AA36-E82B-61AA8538A1C1";
	setAttr -k off ".v";
	setAttr ".pu" 0.46875;
	setAttr ".pv" 0.96875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve120" -p "pSphere1Follicle4696";
	rename -uid "42F9B5DA-4D28-6384-AB62-DA8E9D559D7F";
createNode nurbsCurve -n "curveShape120" -p "curve120";
	rename -uid "29F221B1-4175-D0CA-8F1A-DE8524C8C2DF";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "47BDF970-4690-C565-8EFB-FDA74865D419";
createNode follicle -n "pSphere1FollicleShape5303" -p "pSphere1Follicle5303";
	rename -uid "A83258CA-4862-FBBA-0D25-218C7B07BDC6";
	setAttr -k off ".v";
	setAttr ".pu" 0.53125;
	setAttr ".pv" 0.03125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve121" -p "pSphere1Follicle5303";
	rename -uid "3B371845-47A7-731D-D3F8-67ADD9D16C0B";
createNode nurbsCurve -n "curveShape121" -p "curve121";
	rename -uid "9F696832-48A2-1E01-EB61-8F920CDA5937";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "A2511DCB-4E2C-C004-F873-4D94CB3992AA";
createNode follicle -n "pSphere1FollicleShape5309" -p "pSphere1Follicle5309";
	rename -uid "1A9D6FC0-4FA1-45FE-180E-D8B715CB4DEF";
	setAttr -k off ".v";
	setAttr ".pu" 0.53125;
	setAttr ".pv" 0.09375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve122" -p "pSphere1Follicle5309";
	rename -uid "D04CA139-4CE1-A55B-7A2E-79A043D5F6FA";
createNode nurbsCurve -n "curveShape122" -p "curve122";
	rename -uid "65D3821D-462E-7006-77BB-53AEC1B75D3D";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "BCB4EFA3-499D-D83F-41E9-AAAD617C7FDD";
createNode follicle -n "pSphere1FollicleShape5315" -p "pSphere1Follicle5315";
	rename -uid "98C5BC43-4441-6783-C8DB-15B065C8B644";
	setAttr -k off ".v";
	setAttr ".pu" 0.53125;
	setAttr ".pv" 0.15625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve123" -p "pSphere1Follicle5315";
	rename -uid "21E189BC-4484-F4DF-4A52-75B2014F2769";
createNode nurbsCurve -n "curveShape123" -p "curve123";
	rename -uid "0881918F-477A-3A1D-00CC-05BE64735136";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "7DC67D4E-4AA6-70CD-959D-BC985CE137A8";
createNode follicle -n "pSphere1FollicleShape5322" -p "pSphere1Follicle5322";
	rename -uid "8CDEFE1F-4E24-913D-E73E-4AA2F6E970A6";
	setAttr -k off ".v";
	setAttr ".pu" 0.53125;
	setAttr ".pv" 0.21875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve124" -p "pSphere1Follicle5322";
	rename -uid "08BEDA38-44C5-ED67-17E4-8AB4F82E6B83";
createNode nurbsCurve -n "curveShape124" -p "curve124";
	rename -uid "EA5CCFDC-4549-87CA-568A-C199B184759A";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "8C060C85-4791-FE61-97F9-7599269C620A";
createNode follicle -n "pSphere1FollicleShape5328" -p "pSphere1Follicle5328";
	rename -uid "FC67FACF-469A-0C76-1DCD-E5AD3B1B435F";
	setAttr -k off ".v";
	setAttr ".pu" 0.53125;
	setAttr ".pv" 0.28125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve125" -p "pSphere1Follicle5328";
	rename -uid "CD3DC615-4467-9913-69C6-CC9CB8669EEB";
createNode nurbsCurve -n "curveShape125" -p "curve125";
	rename -uid "A5C69762-4604-CF8A-EFFB-8EB5C832AFA6";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "FAB06693-4E68-A8B8-9E90-B987B2BB6BFC";
createNode follicle -n "pSphere1FollicleShape5334" -p "pSphere1Follicle5334";
	rename -uid "1DBE1070-41C3-E183-88AD-D6931274FB9A";
	setAttr -k off ".v";
	setAttr ".pu" 0.53125;
	setAttr ".pv" 0.34375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve126" -p "pSphere1Follicle5334";
	rename -uid "6136E208-4C7C-52C7-0C16-EE97F078AE0B";
createNode nurbsCurve -n "curveShape126" -p "curve126";
	rename -uid "484C9278-466A-B75F-4E21-71A3C2AA6636";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "C5AB79E4-45A1-7919-0A42-5FA20F80C009";
createNode follicle -n "pSphere1FollicleShape5340" -p "pSphere1Follicle5340";
	rename -uid "4BBF68B8-4FB7-DADB-F710-20846621F237";
	setAttr -k off ".v";
	setAttr ".pu" 0.53125;
	setAttr ".pv" 0.40625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve127" -p "pSphere1Follicle5340";
	rename -uid "F623EFEF-42EC-AD15-A978-FDB65450F91B";
createNode nurbsCurve -n "curveShape127" -p "curve127";
	rename -uid "1D2DFD34-4589-1AE6-1C70-6D86ABEE9028";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "3BF2BC6B-4A46-A2E4-C74E-1892238CFCFE";
createNode follicle -n "pSphere1FollicleShape5346" -p "pSphere1Follicle5346";
	rename -uid "28E5D571-47AC-B323-69D1-748A46E5EBE0";
	setAttr -k off ".v";
	setAttr ".pu" 0.53125;
	setAttr ".pv" 0.46875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve128" -p "pSphere1Follicle5346";
	rename -uid "4683910E-4308-F18D-289C-659C8E5BEE3D";
createNode nurbsCurve -n "curveShape128" -p "curve128";
	rename -uid "CE62AD60-49A6-896D-9396-CD9480B44412";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "5A4C0A44-4619-1C84-9279-BA980747CF46";
createNode follicle -n "pSphere1FollicleShape5353" -p "pSphere1Follicle5353";
	rename -uid "9F47D956-43C3-BC8E-98EF-2586845501EC";
	setAttr -k off ".v";
	setAttr ".pu" 0.53125;
	setAttr ".pv" 0.53125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve129" -p "pSphere1Follicle5353";
	rename -uid "05B4FC75-4258-631E-8435-7F9EC7E31477";
createNode nurbsCurve -n "curveShape129" -p "curve129";
	rename -uid "541DF963-4123-AD47-65E6-668DB75AA3C7";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "C2BFEAFE-43D1-79AA-FF49-33B07BBCC4DA";
createNode follicle -n "pSphere1FollicleShape5359" -p "pSphere1Follicle5359";
	rename -uid "90440FB7-4F5C-4350-ED12-F9B18AB2C858";
	setAttr -k off ".v";
	setAttr ".pu" 0.53125;
	setAttr ".pv" 0.59375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve130" -p "pSphere1Follicle5359";
	rename -uid "BB9B1D5A-4534-F11A-DBF2-88B13CD183D0";
createNode nurbsCurve -n "curveShape130" -p "curve130";
	rename -uid "28BC279B-4A62-462D-425E-699001C08C82";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "B69CCBB9-43BF-75A3-1BD4-CE89757E9297";
createNode follicle -n "pSphere1FollicleShape5365" -p "pSphere1Follicle5365";
	rename -uid "5DD53708-4864-9E97-F9FD-5CAD9C600DC1";
	setAttr -k off ".v";
	setAttr ".pu" 0.53125;
	setAttr ".pv" 0.65625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve131" -p "pSphere1Follicle5365";
	rename -uid "54BE4EA3-4923-8A6D-EF87-52BBA63C0EC4";
createNode nurbsCurve -n "curveShape131" -p "curve131";
	rename -uid "0C30EBB9-4B93-36E2-0882-E5865D4B63EC";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "E94E84B4-49C9-4C24-A42F-79951656988A";
createNode follicle -n "pSphere1FollicleShape5371" -p "pSphere1Follicle5371";
	rename -uid "514E6713-46B2-5293-662E-61B3B03CB910";
	setAttr -k off ".v";
	setAttr ".pu" 0.53125;
	setAttr ".pv" 0.71875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve132" -p "pSphere1Follicle5371";
	rename -uid "6C44AB09-4721-DCCF-2E2E-B2AB52578E89";
createNode nurbsCurve -n "curveShape132" -p "curve132";
	rename -uid "DA15D9AC-4A5C-D4BF-F7AC-4B84EA3FD6A0";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "7A9D528D-450C-882D-56BC-AB8B8161728C";
createNode follicle -n "pSphere1FollicleShape5377" -p "pSphere1Follicle5377";
	rename -uid "27249C07-4B3D-96A0-7614-84B2B156DD19";
	setAttr -k off ".v";
	setAttr ".pu" 0.53125;
	setAttr ".pv" 0.78125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve133" -p "pSphere1Follicle5377";
	rename -uid "ED573B8D-422A-82A4-05C6-5AAE828E8B7B";
createNode nurbsCurve -n "curveShape133" -p "curve133";
	rename -uid "E95E9E31-4AA9-BECD-E5DD-12A2E8380081";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "23856F65-4ADD-D538-F08E-59A3739A9678";
createNode follicle -n "pSphere1FollicleShape5384" -p "pSphere1Follicle5384";
	rename -uid "0EE4AEDF-4F00-535B-5608-038C57F1AB7C";
	setAttr -k off ".v";
	setAttr ".pu" 0.53125;
	setAttr ".pv" 0.84375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve134" -p "pSphere1Follicle5384";
	rename -uid "E1A67B20-40C1-32DF-4F7C-B4BC580C65D3";
createNode nurbsCurve -n "curveShape134" -p "curve134";
	rename -uid "E9D2C762-4F2A-DED5-7CF3-01AC0C5C4BE9";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "4E7ED6CD-456B-2074-37EE-B0889B26DF7D";
createNode follicle -n "pSphere1FollicleShape5390" -p "pSphere1Follicle5390";
	rename -uid "0D0EE823-4135-FE24-AE38-1495FF550D00";
	setAttr -k off ".v";
	setAttr ".pu" 0.53125;
	setAttr ".pv" 0.90625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve135" -p "pSphere1Follicle5390";
	rename -uid "5F9EB88C-4217-3038-C122-F6A5C3E3D86D";
createNode nurbsCurve -n "curveShape135" -p "curve135";
	rename -uid "EBA0A56F-4D39-D237-D691-EE80623A8992";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "744B967F-4C1A-0563-7F05-A89B3CAA0EF2";
createNode follicle -n "pSphere1FollicleShape5396" -p "pSphere1Follicle5396";
	rename -uid "C726494E-43E8-6E03-7749-ED9039FA51FC";
	setAttr -k off ".v";
	setAttr ".pu" 0.53125;
	setAttr ".pv" 0.96875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve136" -p "pSphere1Follicle5396";
	rename -uid "2BE951AA-4698-A90C-18ED-39A284CD2162";
createNode nurbsCurve -n "curveShape136" -p "curve136";
	rename -uid "A595071B-4A97-C028-5509-60B715F4BBFB";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "87C39390-4EF4-D88B-4201-3D8B9A227B14";
createNode follicle -n "pSphere1FollicleShape5909" -p "pSphere1Follicle5909";
	rename -uid "926AF6CD-463D-D391-505E-03BEFE4B2285";
	setAttr -k off ".v";
	setAttr ".pu" 0.59375;
	setAttr ".pv" 0.09375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve137" -p "pSphere1Follicle5909";
	rename -uid "ADB616F0-4929-B03F-5F36-01B1720DB865";
createNode nurbsCurve -n "curveShape137" -p "curve137";
	rename -uid "9082BECA-4583-EF66-F050-28BEC77EC45D";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "DAAA5034-4059-0AE4-B640-758090AD1762";
createNode follicle -n "pSphere1FollicleShape5915" -p "pSphere1Follicle5915";
	rename -uid "222F4BFF-493F-DA3C-7C88-50B3B2CB381A";
	setAttr -k off ".v";
	setAttr ".pu" 0.59375;
	setAttr ".pv" 0.15625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve138" -p "pSphere1Follicle5915";
	rename -uid "39E4C933-4FB1-CCA7-A3DE-7E845AEAAB1D";
createNode nurbsCurve -n "curveShape138" -p "curve138";
	rename -uid "DF7BE856-444F-78DA-8DA7-9DB033B93FAB";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "3801B87A-4773-4F04-DE73-9FA893A365E7";
createNode follicle -n "pSphere1FollicleShape5922" -p "pSphere1Follicle5922";
	rename -uid "1136C5C9-4B5A-0870-7DBB-06870CB64E82";
	setAttr -k off ".v";
	setAttr ".pu" 0.59375;
	setAttr ".pv" 0.21875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve139" -p "pSphere1Follicle5922";
	rename -uid "0C785575-40EC-2442-0930-2BBBC74BA0C2";
createNode nurbsCurve -n "curveShape139" -p "curve139";
	rename -uid "1E3BFDD8-4B6F-3D7D-6C0D-D3AC33B11414";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "796F6A7F-411C-1765-C65B-9BB94689485F";
createNode follicle -n "pSphere1FollicleShape5928" -p "pSphere1Follicle5928";
	rename -uid "3BC3014D-4886-6CAC-82BE-97BC2EC022B8";
	setAttr -k off ".v";
	setAttr ".pu" 0.59375;
	setAttr ".pv" 0.28125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve140" -p "pSphere1Follicle5928";
	rename -uid "DF4F4E9D-41EA-E4B8-F45C-7AA06B69A40B";
createNode nurbsCurve -n "curveShape140" -p "curve140";
	rename -uid "4230FD73-44D1-9D3A-D26A-A98118CCD723";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "FADA23B3-4C25-565A-2AB0-60856C3625A3";
createNode follicle -n "pSphere1FollicleShape5934" -p "pSphere1Follicle5934";
	rename -uid "97FC11BD-45BD-2D44-3D52-54931AF41DDD";
	setAttr -k off ".v";
	setAttr ".pu" 0.59375;
	setAttr ".pv" 0.34375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve141" -p "pSphere1Follicle5934";
	rename -uid "7D78468F-4A43-85BB-41B3-848005858A29";
createNode nurbsCurve -n "curveShape141" -p "curve141";
	rename -uid "E1BCA840-4264-88F4-543A-FF8B879D3D95";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "8DF96492-42B4-F7D3-446C-F3A02E9D353F";
createNode follicle -n "pSphere1FollicleShape5940" -p "pSphere1Follicle5940";
	rename -uid "BAE5E23E-4A7A-C344-1901-AD963CAFDFA0";
	setAttr -k off ".v";
	setAttr ".pu" 0.59375;
	setAttr ".pv" 0.40625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve142" -p "pSphere1Follicle5940";
	rename -uid "857B7106-4FB7-BF00-CB05-FC851D0C699F";
createNode nurbsCurve -n "curveShape142" -p "curve142";
	rename -uid "27F1D1A2-46F9-47B2-2AB6-3A97091FC239";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "B550FE00-45E4-94BC-79A4-F9A54B395C5A";
createNode follicle -n "pSphere1FollicleShape5946" -p "pSphere1Follicle5946";
	rename -uid "FDFEB499-4775-C822-EA43-E4ADD0625C9F";
	setAttr -k off ".v";
	setAttr ".pu" 0.59375;
	setAttr ".pv" 0.46875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve143" -p "pSphere1Follicle5946";
	rename -uid "F55583E3-4798-26C4-70DC-0EB548A3893C";
createNode nurbsCurve -n "curveShape143" -p "curve143";
	rename -uid "96F08A20-42AD-4495-6E7F-6BBDE7525D0F";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "9F6470FE-466A-1019-FFBF-D2A9F14C920A";
createNode follicle -n "pSphere1FollicleShape5953" -p "pSphere1Follicle5953";
	rename -uid "296F4B79-412C-1ED9-319E-F0BC63358299";
	setAttr -k off ".v";
	setAttr ".pu" 0.59375;
	setAttr ".pv" 0.53125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve144" -p "pSphere1Follicle5953";
	rename -uid "4679F0CD-4572-3DF5-2C57-7289C5B75412";
createNode nurbsCurve -n "curveShape144" -p "curve144";
	rename -uid "8E26F436-403E-8BAF-E944-1989DDC2B218";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "4D4076CF-4EE1-0643-0BF0-07AF265B7226";
createNode follicle -n "pSphere1FollicleShape5959" -p "pSphere1Follicle5959";
	rename -uid "9F7195ED-4350-F5BF-8FBA-318AA43DABC1";
	setAttr -k off ".v";
	setAttr ".pu" 0.59375;
	setAttr ".pv" 0.59375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve145" -p "pSphere1Follicle5959";
	rename -uid "FD703CC0-4E39-D774-6AB4-EDA49B2B8045";
createNode nurbsCurve -n "curveShape145" -p "curve145";
	rename -uid "AF10827A-4F10-0AD9-670B-0CBF8A412F8C";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "1F488ED1-4870-A6D3-300B-CE9B1DAE9E5F";
createNode follicle -n "pSphere1FollicleShape5965" -p "pSphere1Follicle5965";
	rename -uid "4F7AF565-4A4A-9C10-F6CB-12B92913EC25";
	setAttr -k off ".v";
	setAttr ".pu" 0.59375;
	setAttr ".pv" 0.65625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve146" -p "pSphere1Follicle5965";
	rename -uid "764C4BE0-4B4F-D619-77A0-55BB4EC51D6F";
createNode nurbsCurve -n "curveShape146" -p "curve146";
	rename -uid "FC755DDD-40BB-88B9-0BBB-7F89C429290B";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "B182EDE8-45BF-C7F8-0D58-E499F9465187";
createNode follicle -n "pSphere1FollicleShape5971" -p "pSphere1Follicle5971";
	rename -uid "81BB7751-407A-4208-0160-83AB39ACCC36";
	setAttr -k off ".v";
	setAttr ".pu" 0.59375;
	setAttr ".pv" 0.71875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve147" -p "pSphere1Follicle5971";
	rename -uid "DE40F122-41A9-DFB9-5761-16B4DAC7C882";
createNode nurbsCurve -n "curveShape147" -p "curve147";
	rename -uid "A52476DD-4313-001B-EE64-F2B8FEEB9B22";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "797A7FEA-411D-DE0C-A05A-699B525575B6";
createNode follicle -n "pSphere1FollicleShape5977" -p "pSphere1Follicle5977";
	rename -uid "65903650-4C7E-2184-58B8-409D8FB6C493";
	setAttr -k off ".v";
	setAttr ".pu" 0.59375;
	setAttr ".pv" 0.78125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve148" -p "pSphere1Follicle5977";
	rename -uid "3AE8BB9D-4648-CB03-8E11-C0BA8E6C2594";
createNode nurbsCurve -n "curveShape148" -p "curve148";
	rename -uid "4455FA82-4739-6CEA-971D-49BB660AE0CE";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "35D2C4EE-406D-A2BD-E918-D68A9A5EA488";
createNode follicle -n "pSphere1FollicleShape5984" -p "pSphere1Follicle5984";
	rename -uid "3EC78944-41BA-54B8-8C17-F6BD1ADAFBE1";
	setAttr -k off ".v";
	setAttr ".pu" 0.59375;
	setAttr ".pv" 0.84375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve149" -p "pSphere1Follicle5984";
	rename -uid "86261EBE-4ABB-E501-198F-64ACB0E36B5B";
createNode nurbsCurve -n "curveShape149" -p "curve149";
	rename -uid "53FFB7D3-4FFA-B56B-F717-CFB6F76D332E";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "157E73E9-4568-401D-8CBF-6D81491341BB";
createNode follicle -n "pSphere1FollicleShape5990" -p "pSphere1Follicle5990";
	rename -uid "6FA4C5B1-4F21-3AF5-A94C-1A84DB514E26";
	setAttr -k off ".v";
	setAttr ".pu" 0.59375;
	setAttr ".pv" 0.90625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve150" -p "pSphere1Follicle5990";
	rename -uid "1E000014-4BCE-9AD8-CD12-2CBEE422AD98";
createNode nurbsCurve -n "curveShape150" -p "curve150";
	rename -uid "71816806-4EA0-99F8-C9E8-87B54FB7F2A3";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "820FB214-4C91-633E-7093-7E8630A66A54";
createNode follicle -n "pSphere1FollicleShape6509" -p "pSphere1Follicle6509";
	rename -uid "23644EE6-4D59-9D28-AAD2-279CDEE9914E";
	setAttr -k off ".v";
	setAttr ".pu" 0.65625;
	setAttr ".pv" 0.09375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve151" -p "pSphere1Follicle6509";
	rename -uid "B2573D3B-4CC3-C2B0-019B-12A9C59DFFD7";
createNode nurbsCurve -n "curveShape151" -p "curve151";
	rename -uid "77B6DEFC-4F68-4814-8122-1FBBC457E778";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "F670D620-470B-9C80-C7BE-D387C6954DDF";
createNode follicle -n "pSphere1FollicleShape6515" -p "pSphere1Follicle6515";
	rename -uid "00544734-4C75-8A92-38F8-D68789257017";
	setAttr -k off ".v";
	setAttr ".pu" 0.65625;
	setAttr ".pv" 0.15625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve152" -p "pSphere1Follicle6515";
	rename -uid "E5575456-4506-8789-EA8C-FD9AC5811B02";
createNode nurbsCurve -n "curveShape152" -p "curve152";
	rename -uid "E4D57B89-42E6-5A51-E7E3-078981D0A22F";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "415EE41B-4AEB-D4A6-C3F0-B4828D5676C5";
createNode follicle -n "pSphere1FollicleShape6522" -p "pSphere1Follicle6522";
	rename -uid "39132BE3-4D6A-A0FA-0956-928E87061298";
	setAttr -k off ".v";
	setAttr ".pu" 0.65625;
	setAttr ".pv" 0.21875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve153" -p "pSphere1Follicle6522";
	rename -uid "0E09DCE4-42B9-D940-D239-57A93DAE37A6";
createNode nurbsCurve -n "curveShape153" -p "curve153";
	rename -uid "3A6AD67D-4A6B-D9FA-2D15-80B3C7E159F8";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "B0EDA086-4FCC-5F71-87F0-568AE54E315D";
createNode follicle -n "pSphere1FollicleShape6528" -p "pSphere1Follicle6528";
	rename -uid "8D9590D6-4529-7ECE-0BF5-5289705E193E";
	setAttr -k off ".v";
	setAttr ".pu" 0.65625;
	setAttr ".pv" 0.28125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve154" -p "pSphere1Follicle6528";
	rename -uid "79DC93B9-4CC8-3CE4-6618-929929F3E4C1";
createNode nurbsCurve -n "curveShape154" -p "curve154";
	rename -uid "D8B03E3A-4F8B-A30D-F170-B2877A79A4DD";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "E2550078-4E1C-7F2F-CC99-EEA0D5DA3176";
createNode follicle -n "pSphere1FollicleShape6534" -p "pSphere1Follicle6534";
	rename -uid "18E20FE7-417F-7585-A01C-E5B0FFC4C066";
	setAttr -k off ".v";
	setAttr ".pu" 0.65625;
	setAttr ".pv" 0.34375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve155" -p "pSphere1Follicle6534";
	rename -uid "8B5E249E-4CF5-E302-C1FC-33B324D97D22";
createNode nurbsCurve -n "curveShape155" -p "curve155";
	rename -uid "53D4F9B7-429D-7868-D61D-3FB47CEC0FCE";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "B226EB18-4C96-41AB-C54F-E882B8133740";
createNode follicle -n "pSphere1FollicleShape6540" -p "pSphere1Follicle6540";
	rename -uid "50E81425-496A-5B8B-A8E1-7E9763B97E3B";
	setAttr -k off ".v";
	setAttr ".pu" 0.65625;
	setAttr ".pv" 0.40625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve156" -p "pSphere1Follicle6540";
	rename -uid "34E37311-4053-706B-8C4E-4A8D6D62686D";
createNode nurbsCurve -n "curveShape156" -p "curve156";
	rename -uid "9B705C5A-41B6-7B8C-DCE5-92A9C61ED44E";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "7AF3AE9B-41B4-A42E-C293-FABA60117ABB";
createNode follicle -n "pSphere1FollicleShape6546" -p "pSphere1Follicle6546";
	rename -uid "822F3BA1-4E86-5EF1-86FE-3493E33063F5";
	setAttr -k off ".v";
	setAttr ".pu" 0.65625;
	setAttr ".pv" 0.46875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve157" -p "pSphere1Follicle6546";
	rename -uid "0C5FB21B-4EAB-35BB-19D8-04A82F722783";
createNode nurbsCurve -n "curveShape157" -p "curve157";
	rename -uid "B4574EDA-4450-A59A-6573-06BE14DB57D1";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "E893708D-4CAE-E2DE-309A-B68612C9D2E3";
createNode follicle -n "pSphere1FollicleShape6553" -p "pSphere1Follicle6553";
	rename -uid "1E8EC592-4F54-A39A-20DE-F096D0326792";
	setAttr -k off ".v";
	setAttr ".pu" 0.65625;
	setAttr ".pv" 0.53125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve158" -p "pSphere1Follicle6553";
	rename -uid "C7047AFF-4AF2-4D72-0449-659A616ADB52";
createNode nurbsCurve -n "curveShape158" -p "curve158";
	rename -uid "6E033074-47D5-5EE2-FAB8-CDBEB0EAF207";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "79DE296D-43A5-5B32-942F-889A953D50AB";
createNode follicle -n "pSphere1FollicleShape6559" -p "pSphere1Follicle6559";
	rename -uid "386C33B8-4EDB-0947-CE37-5285C903CFAB";
	setAttr -k off ".v";
	setAttr ".pu" 0.65625;
	setAttr ".pv" 0.59375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve159" -p "pSphere1Follicle6559";
	rename -uid "1946BEE2-418B-DDB2-021F-D9B827E6D12A";
createNode nurbsCurve -n "curveShape159" -p "curve159";
	rename -uid "1616824B-427C-059C-E799-E0AC289204D5";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "9CFA2068-486C-A47F-8A26-6FBCDD0F1F25";
createNode follicle -n "pSphere1FollicleShape6565" -p "pSphere1Follicle6565";
	rename -uid "47794443-4DE2-EE4B-DC2A-70BE3A9FF790";
	setAttr -k off ".v";
	setAttr ".pu" 0.65625;
	setAttr ".pv" 0.65625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve160" -p "pSphere1Follicle6565";
	rename -uid "573C204B-4ADD-D412-E34C-FA952676CBD1";
createNode nurbsCurve -n "curveShape160" -p "curve160";
	rename -uid "D5F57377-46A8-00FF-8C08-79B2D038A074";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "F10BC3EC-4EE0-DBB6-5CB7-40A88E88A6D1";
createNode follicle -n "pSphere1FollicleShape6571" -p "pSphere1Follicle6571";
	rename -uid "D83C8CF1-4657-7D19-DBE3-02B2089AC79A";
	setAttr -k off ".v";
	setAttr ".pu" 0.65625;
	setAttr ".pv" 0.71875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve161" -p "pSphere1Follicle6571";
	rename -uid "47E5E201-451A-7E29-4D43-E4895D49E11E";
createNode nurbsCurve -n "curveShape161" -p "curve161";
	rename -uid "B6B8970C-414F-D5F4-8B71-80A634F1CD0C";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "E29F38C0-4133-6692-F629-1AB44FEB9382";
createNode follicle -n "pSphere1FollicleShape6577" -p "pSphere1Follicle6577";
	rename -uid "8717A64E-40F0-31E4-EF11-0DABD17142D5";
	setAttr -k off ".v";
	setAttr ".pu" 0.65625;
	setAttr ".pv" 0.78125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve162" -p "pSphere1Follicle6577";
	rename -uid "1B55B3C3-4BD8-E5D9-8809-5E8265C93908";
createNode nurbsCurve -n "curveShape162" -p "curve162";
	rename -uid "FD0B9E49-4C80-B628-728A-D7B3D2868F13";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "2806C162-4578-69C8-CA8E-AC99221059A6";
createNode follicle -n "pSphere1FollicleShape6584" -p "pSphere1Follicle6584";
	rename -uid "E3FA951C-433D-D8C0-6594-3DB063A965E6";
	setAttr -k off ".v";
	setAttr ".pu" 0.65625;
	setAttr ".pv" 0.84375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve163" -p "pSphere1Follicle6584";
	rename -uid "CC171A7C-4F84-67BB-791A-FE87D3A03C45";
createNode nurbsCurve -n "curveShape163" -p "curve163";
	rename -uid "0EC0D74B-4E2A-2AF7-ED36-66A8559CC115";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "67CC08F7-49F1-DB57-1077-B1B0E6970E40";
createNode follicle -n "pSphere1FollicleShape6590" -p "pSphere1Follicle6590";
	rename -uid "C6FEA275-4E9C-46EE-A899-51B25D60EAF5";
	setAttr -k off ".v";
	setAttr ".pu" 0.65625;
	setAttr ".pv" 0.90625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve164" -p "pSphere1Follicle6590";
	rename -uid "1619E9FC-4675-1995-8ABF-E88BCFFAE9A5";
createNode nurbsCurve -n "curveShape164" -p "curve164";
	rename -uid "75A08C62-4B8D-12E5-52D4-B99CA94BCF4D";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "F6242A8B-48D5-5B37-E0C4-43B8389BC091";
createNode follicle -n "pSphere1FollicleShape7103" -p "pSphere1Follicle7103";
	rename -uid "35C75BFF-41CB-18DA-F2E6-60AFC5387BD2";
	setAttr -k off ".v";
	setAttr ".pu" 0.71875;
	setAttr ".pv" 0.03125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve165" -p "pSphere1Follicle7103";
	rename -uid "1423434D-4245-3F0A-31BE-9092A21FE136";
createNode nurbsCurve -n "curveShape165" -p "curve165";
	rename -uid "A16FB391-4F26-5D69-F1AD-07AF89E34AF9";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "857011D2-4702-95D8-3190-109D948988EB";
createNode follicle -n "pSphere1FollicleShape7109" -p "pSphere1Follicle7109";
	rename -uid "0B37E991-44AE-8F2E-3B06-F48B3314E421";
	setAttr -k off ".v";
	setAttr ".pu" 0.71875;
	setAttr ".pv" 0.09375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve166" -p "pSphere1Follicle7109";
	rename -uid "1A6A2AD0-4500-EA8C-4BED-1AAD5264E86E";
createNode nurbsCurve -n "curveShape166" -p "curve166";
	rename -uid "C68B9139-40F5-925D-8343-5D8DC95A4DD2";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "AC6D6EA5-49FF-43B8-980F-FFAB7434AAC3";
createNode follicle -n "pSphere1FollicleShape7115" -p "pSphere1Follicle7115";
	rename -uid "348D4F15-4C24-9E63-C6CA-429BB0E6A74E";
	setAttr -k off ".v";
	setAttr ".pu" 0.71875;
	setAttr ".pv" 0.15625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve167" -p "pSphere1Follicle7115";
	rename -uid "DB44AE9B-4CC8-EFD5-C935-A5BCFD95C14C";
createNode nurbsCurve -n "curveShape167" -p "curve167";
	rename -uid "3C951E94-498A-A115-D713-03B64E274E6C";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "F6710B7D-4F3F-D27E-D715-CFA6B328D7A9";
createNode follicle -n "pSphere1FollicleShape7122" -p "pSphere1Follicle7122";
	rename -uid "50856DC3-4D25-94E3-21F4-E484C80D7BF9";
	setAttr -k off ".v";
	setAttr ".pu" 0.71875;
	setAttr ".pv" 0.21875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve168" -p "pSphere1Follicle7122";
	rename -uid "B42B5273-4EDB-A361-3BF1-978643E34669";
createNode nurbsCurve -n "curveShape168" -p "curve168";
	rename -uid "C0CFBB4A-407D-75DE-DA8F-51B0DA54E1E0";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "5D11E59E-45C7-97B8-C770-54AFAC373A87";
createNode follicle -n "pSphere1FollicleShape7128" -p "pSphere1Follicle7128";
	rename -uid "5D24AB32-4B96-F690-8E6A-9AB78152EC90";
	setAttr -k off ".v";
	setAttr ".pu" 0.71875;
	setAttr ".pv" 0.28125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve169" -p "pSphere1Follicle7128";
	rename -uid "99C470AB-4226-427A-B48B-2DA84A44A8DB";
createNode nurbsCurve -n "curveShape169" -p "curve169";
	rename -uid "D0E8742F-4FDB-1B44-79A3-4F86CCE8443D";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "E48123E2-4035-07EB-B46B-71A1050AF7B4";
createNode follicle -n "pSphere1FollicleShape7134" -p "pSphere1Follicle7134";
	rename -uid "285A250E-4202-A345-237F-DF8E1FA94F23";
	setAttr -k off ".v";
	setAttr ".pu" 0.71875;
	setAttr ".pv" 0.34375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve170" -p "pSphere1Follicle7134";
	rename -uid "57BDCC72-4FDC-6884-A7E5-44A11A7F6A3A";
createNode nurbsCurve -n "curveShape170" -p "curve170";
	rename -uid "F23D949C-47B7-C5A5-AB64-02B04DA00382";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "16101694-4765-CE5E-CB2B-F1921AABED06";
createNode follicle -n "pSphere1FollicleShape7140" -p "pSphere1Follicle7140";
	rename -uid "E9984FB8-4D27-6DD8-8EDC-09AF28935F3C";
	setAttr -k off ".v";
	setAttr ".pu" 0.71875;
	setAttr ".pv" 0.40625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve171" -p "pSphere1Follicle7140";
	rename -uid "98C55DEE-4966-59AE-1883-188251CB0C7A";
createNode nurbsCurve -n "curveShape171" -p "curve171";
	rename -uid "06EEC6A9-482B-003A-74CB-D6961461E603";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "2D290D40-4B91-59C3-1172-79B6C3FF46A6";
createNode follicle -n "pSphere1FollicleShape7146" -p "pSphere1Follicle7146";
	rename -uid "7A173B2C-41F2-93B4-32A1-3E8E0E18706A";
	setAttr -k off ".v";
	setAttr ".pu" 0.71875;
	setAttr ".pv" 0.46875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve172" -p "pSphere1Follicle7146";
	rename -uid "598D9359-4BAC-D2C6-7E4E-99A7910ABAD1";
createNode nurbsCurve -n "curveShape172" -p "curve172";
	rename -uid "FFF1CB36-48BA-74C5-78FB-47BAC54109CA";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "3865E82D-4148-BB2E-4973-878CD47697F8";
createNode follicle -n "pSphere1FollicleShape7153" -p "pSphere1Follicle7153";
	rename -uid "831FD40F-4500-C53E-8244-F6AC4580C463";
	setAttr -k off ".v";
	setAttr ".pu" 0.71875;
	setAttr ".pv" 0.53125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve173" -p "pSphere1Follicle7153";
	rename -uid "0E41AB5E-47A2-4EF5-E294-A1AB7D615CCF";
createNode nurbsCurve -n "curveShape173" -p "curve173";
	rename -uid "5268DEFD-421F-390E-30C5-25AC86097325";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "27F8B7F9-4D7B-D69B-4081-97BB1F4B14E6";
createNode follicle -n "pSphere1FollicleShape7159" -p "pSphere1Follicle7159";
	rename -uid "037261D8-4085-51F7-1DBE-E19C34D4E505";
	setAttr -k off ".v";
	setAttr ".pu" 0.71875;
	setAttr ".pv" 0.59375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve174" -p "pSphere1Follicle7159";
	rename -uid "BBF6ED0F-4029-1CC3-68DC-5EAD20026F15";
createNode nurbsCurve -n "curveShape174" -p "curve174";
	rename -uid "93923C5A-49E4-B1AE-DECC-23AC5B3E19DE";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "D48AC198-4C47-701A-E949-24878009CE13";
createNode follicle -n "pSphere1FollicleShape7165" -p "pSphere1Follicle7165";
	rename -uid "6636F152-4E16-36F0-2A24-D6B99E55AA2D";
	setAttr -k off ".v";
	setAttr ".pu" 0.71875;
	setAttr ".pv" 0.65625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve175" -p "pSphere1Follicle7165";
	rename -uid "0B46715F-48F1-30BF-B866-3E9A225C0D4C";
createNode nurbsCurve -n "curveShape175" -p "curve175";
	rename -uid "B7F10449-49A8-D300-1479-438D53282E4E";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "9AD8FDD8-4126-D605-CC1E-CA91562B795A";
createNode follicle -n "pSphere1FollicleShape7171" -p "pSphere1Follicle7171";
	rename -uid "EC29B3A8-4C6C-D96E-B349-2B8F8E9F7754";
	setAttr -k off ".v";
	setAttr ".pu" 0.71875;
	setAttr ".pv" 0.71875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve176" -p "pSphere1Follicle7171";
	rename -uid "1B9A4E59-4975-7354-7905-CD801465F35C";
createNode nurbsCurve -n "curveShape176" -p "curve176";
	rename -uid "478CF7CC-452F-A229-8625-68A0184E1346";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "2048EB6B-401F-9112-58CB-23B21C9D1D0E";
createNode follicle -n "pSphere1FollicleShape7177" -p "pSphere1Follicle7177";
	rename -uid "3C3B5811-40AD-CF4F-5843-18BE04D6720E";
	setAttr -k off ".v";
	setAttr ".pu" 0.71875;
	setAttr ".pv" 0.78125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve177" -p "pSphere1Follicle7177";
	rename -uid "8C5E2E1B-4EEF-1ADD-ED61-3C8EFCF6627C";
createNode nurbsCurve -n "curveShape177" -p "curve177";
	rename -uid "C4E07651-4128-9777-B968-9ABD08AC7923";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "D8B8201C-47CC-474F-8416-D091247FF512";
createNode follicle -n "pSphere1FollicleShape7184" -p "pSphere1Follicle7184";
	rename -uid "064989B2-4B8B-0B90-DBC6-E99FEC0E61CF";
	setAttr -k off ".v";
	setAttr ".pu" 0.71875;
	setAttr ".pv" 0.84375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve178" -p "pSphere1Follicle7184";
	rename -uid "5640C7CB-4C53-A346-8B70-14BAFF4FD34D";
createNode nurbsCurve -n "curveShape178" -p "curve178";
	rename -uid "B3984734-4713-1E10-B832-6E9C5B6BA42A";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "E1242C83-4A5B-3E83-3EBF-A1882A6D9580";
createNode follicle -n "pSphere1FollicleShape7190" -p "pSphere1Follicle7190";
	rename -uid "691983E5-4C6E-247C-DCD1-4CAC67A9B756";
	setAttr -k off ".v";
	setAttr ".pu" 0.71875;
	setAttr ".pv" 0.90625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve179" -p "pSphere1Follicle7190";
	rename -uid "D876F515-45D0-5A94-A0DC-97BE738525D9";
createNode nurbsCurve -n "curveShape179" -p "curve179";
	rename -uid "6A116ED7-48D5-4E94-B6AA-438C88F1A1C2";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "0D2DB937-4050-8163-93A6-FBB996D22322";
createNode follicle -n "pSphere1FollicleShape7196" -p "pSphere1Follicle7196";
	rename -uid "8945405F-4526-A4C3-FE42-0D84D489AFB0";
	setAttr -k off ".v";
	setAttr ".pu" 0.71875;
	setAttr ".pv" 0.96875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve180" -p "pSphere1Follicle7196";
	rename -uid "F49B4C09-486F-B64C-91F1-EF8E9FC97BBA";
createNode nurbsCurve -n "curveShape180" -p "curve180";
	rename -uid "B9F2BFAF-48C6-77D9-D53C-538AFFAE0227";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "79836939-460F-5AD8-1F5E-1D893305474D";
createNode follicle -n "pSphere1FollicleShape7703" -p "pSphere1Follicle7703";
	rename -uid "35CE5C9F-46BE-D678-DF23-55B989F65B92";
	setAttr -k off ".v";
	setAttr ".pu" 0.78125;
	setAttr ".pv" 0.03125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve181" -p "pSphere1Follicle7703";
	rename -uid "33AD5C6A-4A95-91D6-59CC-83861654A785";
createNode nurbsCurve -n "curveShape181" -p "curve181";
	rename -uid "8315A010-4898-D518-44A5-2EA523995F09";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "D3BD7F43-47F0-2C65-1024-57A75AFAFA0F";
createNode follicle -n "pSphere1FollicleShape7709" -p "pSphere1Follicle7709";
	rename -uid "ED63593D-438B-406E-0F77-BDA64A0FB9D4";
	setAttr -k off ".v";
	setAttr ".pu" 0.78125;
	setAttr ".pv" 0.09375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve182" -p "pSphere1Follicle7709";
	rename -uid "15E790EC-47FB-49A3-9F6F-A08C608AD04E";
createNode nurbsCurve -n "curveShape182" -p "curve182";
	rename -uid "BAF8182D-464B-EFF4-70E8-ABA4CB9C29DB";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "57DB1602-48AF-A808-5531-25A813586A19";
createNode follicle -n "pSphere1FollicleShape7715" -p "pSphere1Follicle7715";
	rename -uid "CFEB8145-4B6A-9020-45B4-1F9C448F7651";
	setAttr -k off ".v";
	setAttr ".pu" 0.78125;
	setAttr ".pv" 0.15625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve183" -p "pSphere1Follicle7715";
	rename -uid "BCD009A7-4A85-49A6-BBAB-DC9186D69709";
createNode nurbsCurve -n "curveShape183" -p "curve183";
	rename -uid "FD115A77-4069-6D63-2081-C8B5E73D3C04";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "64B88D82-4630-BAFF-3EC0-7B8CFE231774";
createNode follicle -n "pSphere1FollicleShape7722" -p "pSphere1Follicle7722";
	rename -uid "37856F10-4F96-D29F-FFA0-A7A90C08D273";
	setAttr -k off ".v";
	setAttr ".pu" 0.78125;
	setAttr ".pv" 0.21875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve184" -p "pSphere1Follicle7722";
	rename -uid "8C3BB1BC-465D-4AB3-BEE1-BC968F9446D2";
createNode nurbsCurve -n "curveShape184" -p "curve184";
	rename -uid "9AC6A00A-426D-061B-BF12-949A9ECBEE72";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "EAD408F5-4C98-AFD1-E5E9-6082846962BD";
createNode follicle -n "pSphere1FollicleShape7728" -p "pSphere1Follicle7728";
	rename -uid "1F2C9F82-442B-1279-1613-DAAD91D29D12";
	setAttr -k off ".v";
	setAttr ".pu" 0.78125;
	setAttr ".pv" 0.28125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve185" -p "pSphere1Follicle7728";
	rename -uid "3299C7D4-403D-E51F-D9C5-14BFFE9F110A";
createNode nurbsCurve -n "curveShape185" -p "curve185";
	rename -uid "0761BD9B-486D-43A8-0249-18A81533422F";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "8EC404A9-48ED-45BB-CF71-A6AC969B8D42";
createNode follicle -n "pSphere1FollicleShape7734" -p "pSphere1Follicle7734";
	rename -uid "F692E791-4B9A-FE98-4855-69853A4481CC";
	setAttr -k off ".v";
	setAttr ".pu" 0.78125;
	setAttr ".pv" 0.34375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve186" -p "pSphere1Follicle7734";
	rename -uid "67DCF76F-4DCC-E164-2698-C387353F28BE";
createNode nurbsCurve -n "curveShape186" -p "curve186";
	rename -uid "2319CA64-4B3C-D68F-20EC-C4BF271AD636";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "F8E1CEF1-4AD2-4D46-F9BB-6A8D11964608";
createNode follicle -n "pSphere1FollicleShape7740" -p "pSphere1Follicle7740";
	rename -uid "3C985684-4F11-EAD0-78CB-90B137590AAF";
	setAttr -k off ".v";
	setAttr ".pu" 0.78125;
	setAttr ".pv" 0.40625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve187" -p "pSphere1Follicle7740";
	rename -uid "16A23CED-4F76-2E8C-11F5-028F048E1488";
createNode nurbsCurve -n "curveShape187" -p "curve187";
	rename -uid "F0B72F95-4782-C7D0-CB9C-11AD72054BC2";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "AA6C0E4A-469D-8DC9-2D76-6D8A6FF85C4B";
createNode follicle -n "pSphere1FollicleShape7746" -p "pSphere1Follicle7746";
	rename -uid "08E97B8B-41E6-64DE-2F0F-84BAE04EC061";
	setAttr -k off ".v";
	setAttr ".pu" 0.78125;
	setAttr ".pv" 0.46875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve188" -p "pSphere1Follicle7746";
	rename -uid "F66ECF88-4D49-354C-4083-478070E69E9C";
createNode nurbsCurve -n "curveShape188" -p "curve188";
	rename -uid "4639E734-4C78-DF2C-8C93-CFB04E0FDE92";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "3C3B5AB5-49F8-498A-790C-9EBB95AFA0C5";
createNode follicle -n "pSphere1FollicleShape7753" -p "pSphere1Follicle7753";
	rename -uid "59C33FB2-4905-CC47-49FB-13AA7EE6638C";
	setAttr -k off ".v";
	setAttr ".pu" 0.78125;
	setAttr ".pv" 0.53125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve189" -p "pSphere1Follicle7753";
	rename -uid "00B612C0-480D-A90F-055D-738383063F33";
createNode nurbsCurve -n "curveShape189" -p "curve189";
	rename -uid "8D69FEDE-414A-B127-0104-5492C75BAED7";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "171C4A83-4021-49CD-FAA6-92938884AA65";
createNode follicle -n "pSphere1FollicleShape7759" -p "pSphere1Follicle7759";
	rename -uid "72BCF0F6-4881-266B-70E5-0DA0643154B1";
	setAttr -k off ".v";
	setAttr ".pu" 0.78125;
	setAttr ".pv" 0.59375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve190" -p "pSphere1Follicle7759";
	rename -uid "D6A08924-4506-4684-AC30-93BBEA76E9D8";
createNode nurbsCurve -n "curveShape190" -p "curve190";
	rename -uid "390CEC05-4373-C1FF-1F71-E4B4DFA76D92";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "D338FBB9-4BC3-C4B2-4A02-3CB72337AD71";
createNode follicle -n "pSphere1FollicleShape7765" -p "pSphere1Follicle7765";
	rename -uid "8213B59C-470F-71AF-5465-C4BBA2A2B3D7";
	setAttr -k off ".v";
	setAttr ".pu" 0.78125;
	setAttr ".pv" 0.65625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve191" -p "pSphere1Follicle7765";
	rename -uid "CDACEB70-4544-2B8A-D437-EAA7505BB59C";
createNode nurbsCurve -n "curveShape191" -p "curve191";
	rename -uid "5E9C9F1B-441B-1B46-6F31-B5A80501E915";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "2696C0AA-44E5-FDE8-A188-A08D7C572AEB";
createNode follicle -n "pSphere1FollicleShape7771" -p "pSphere1Follicle7771";
	rename -uid "7FC0168B-42F5-9715-26EF-DB9C46559F42";
	setAttr -k off ".v";
	setAttr ".pu" 0.78125;
	setAttr ".pv" 0.71875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve192" -p "pSphere1Follicle7771";
	rename -uid "0CED8B0B-4DAD-117A-1EA8-74A0E2512E85";
createNode nurbsCurve -n "curveShape192" -p "curve192";
	rename -uid "74037162-412E-AA36-990D-279F58E0F465";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "E1D0B626-4E53-4979-6A9F-1C990683D3C2";
createNode follicle -n "pSphere1FollicleShape7777" -p "pSphere1Follicle7777";
	rename -uid "979A134B-4C7D-13A3-368B-50AD4A112D2F";
	setAttr -k off ".v";
	setAttr ".pu" 0.78125;
	setAttr ".pv" 0.78125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve193" -p "pSphere1Follicle7777";
	rename -uid "EAF52665-42F9-0E66-E7F0-7BB1F0027AC7";
createNode nurbsCurve -n "curveShape193" -p "curve193";
	rename -uid "14CC5767-4630-D441-A1C0-B28320647245";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "CF51AABF-45C5-25A1-7686-1BA42BDC53FB";
createNode follicle -n "pSphere1FollicleShape7784" -p "pSphere1Follicle7784";
	rename -uid "67D4FEA0-4F7D-E594-B4C6-49BAB17D4247";
	setAttr -k off ".v";
	setAttr ".pu" 0.78125;
	setAttr ".pv" 0.84375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve194" -p "pSphere1Follicle7784";
	rename -uid "9E0D9C0A-4600-4A5D-F299-A9B56BE4ED45";
createNode nurbsCurve -n "curveShape194" -p "curve194";
	rename -uid "3EF6E13F-48EA-E1B5-6B9B-37A11E1A5863";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "739FAF03-4F77-9150-CFC2-2A950349CE43";
createNode follicle -n "pSphere1FollicleShape7790" -p "pSphere1Follicle7790";
	rename -uid "2E643ED1-4085-45FE-FB6D-159D5AB13464";
	setAttr -k off ".v";
	setAttr ".pu" 0.78125;
	setAttr ".pv" 0.90625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve195" -p "pSphere1Follicle7790";
	rename -uid "42BB34C7-4220-FD5D-A05A-9B99665C2064";
createNode nurbsCurve -n "curveShape195" -p "curve195";
	rename -uid "E6D378CD-4B99-A4F3-365A-49876F94538E";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "3F5D6152-4C19-B06F-01EB-0B8493BE6D22";
createNode follicle -n "pSphere1FollicleShape7796" -p "pSphere1Follicle7796";
	rename -uid "0B5E2995-45D5-2D17-E26D-89B0B5C33359";
	setAttr -k off ".v";
	setAttr ".pu" 0.78125;
	setAttr ".pv" 0.96875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve196" -p "pSphere1Follicle7796";
	rename -uid "3C671DC9-4B17-6B3D-3B9D-10AC68BF01A5";
createNode nurbsCurve -n "curveShape196" -p "curve196";
	rename -uid "FF4DD4E7-440B-C2B4-B4A6-539F62C1D487";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "96557B98-49A7-D44E-FB7F-86813F2936A0";
createNode follicle -n "pSphere1FollicleShape8409" -p "pSphere1Follicle8409";
	rename -uid "2A83BFFD-4475-4281-B618-0DAA4A60488A";
	setAttr -k off ".v";
	setAttr ".pu" 0.84375;
	setAttr ".pv" 0.09375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve197" -p "pSphere1Follicle8409";
	rename -uid "7B93C26E-4C21-5B74-F496-CA998496D91D";
createNode nurbsCurve -n "curveShape197" -p "curve197";
	rename -uid "5EFBF56B-4E08-91DF-33A5-2B9F211EDA8B";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "AE048BC1-4B0C-B07B-6DC9-D9A7CBBD431C";
createNode follicle -n "pSphere1FollicleShape8415" -p "pSphere1Follicle8415";
	rename -uid "44C3759C-4831-8FA9-7C78-7D9868F67FA1";
	setAttr -k off ".v";
	setAttr ".pu" 0.84375;
	setAttr ".pv" 0.15625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve198" -p "pSphere1Follicle8415";
	rename -uid "A136AEDD-459B-E3CA-E790-9ABC4C6DA894";
createNode nurbsCurve -n "curveShape198" -p "curve198";
	rename -uid "917860C6-4161-9637-EAB8-0589ECEAD232";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "1CC42BC0-4787-7E66-047F-F891DD94EAAA";
createNode follicle -n "pSphere1FollicleShape8422" -p "pSphere1Follicle8422";
	rename -uid "FB5E13B5-4A8F-21C9-8D3D-09A31BB485C1";
	setAttr -k off ".v";
	setAttr ".pu" 0.84375;
	setAttr ".pv" 0.21875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve199" -p "pSphere1Follicle8422";
	rename -uid "E1E31C4F-4B6E-9DC5-1804-BE938452931D";
createNode nurbsCurve -n "curveShape199" -p "curve199";
	rename -uid "B48B5B15-4380-FA50-E125-B7B9223D899A";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "C272B2C6-4BF5-1985-43F7-DFA79D9884F5";
createNode follicle -n "pSphere1FollicleShape8428" -p "pSphere1Follicle8428";
	rename -uid "B2CC9B98-40E4-3373-F699-668B0F34DC3C";
	setAttr -k off ".v";
	setAttr ".pu" 0.84375;
	setAttr ".pv" 0.28125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve200" -p "pSphere1Follicle8428";
	rename -uid "73DD875D-449C-DB74-C1E6-758EA5F788DE";
createNode nurbsCurve -n "curveShape200" -p "curve200";
	rename -uid "12E45469-4701-6FE3-B4D8-0C81067B5D29";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "F0FBB903-4679-5397-8802-CDA28C5F3A64";
createNode follicle -n "pSphere1FollicleShape8434" -p "pSphere1Follicle8434";
	rename -uid "0EA5DDFA-4E48-B0EE-7054-B88A129B586A";
	setAttr -k off ".v";
	setAttr ".pu" 0.84375;
	setAttr ".pv" 0.34375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve201" -p "pSphere1Follicle8434";
	rename -uid "E80AA519-4504-F6C9-1001-E483390717BA";
createNode nurbsCurve -n "curveShape201" -p "curve201";
	rename -uid "51A6544A-4A48-46B3-AA47-9A9323DCDA12";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "98B08CB9-4360-37E6-9444-CCBAF6917089";
createNode follicle -n "pSphere1FollicleShape8440" -p "pSphere1Follicle8440";
	rename -uid "5AC25748-4F58-1AAF-05D8-7F9B5575C2D7";
	setAttr -k off ".v";
	setAttr ".pu" 0.84375;
	setAttr ".pv" 0.40625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve202" -p "pSphere1Follicle8440";
	rename -uid "822B233D-4854-9AA5-65CD-C7998361F461";
createNode nurbsCurve -n "curveShape202" -p "curve202";
	rename -uid "2B540BDB-4AF8-E530-EF6E-C4ABD57B277D";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "2AC5FD08-4C2C-2E15-0836-0DB37D8FE5FE";
createNode follicle -n "pSphere1FollicleShape8446" -p "pSphere1Follicle8446";
	rename -uid "861F6865-4605-B848-2804-62AFBED76BDB";
	setAttr -k off ".v";
	setAttr ".pu" 0.84375;
	setAttr ".pv" 0.46875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve203" -p "pSphere1Follicle8446";
	rename -uid "761DA7EC-4CB7-E456-4FF0-E2BDF90DBBE4";
createNode nurbsCurve -n "curveShape203" -p "curve203";
	rename -uid "1A48C5C0-4026-463E-F3EA-66B52778358C";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "BD5D43B7-4579-6F72-5262-C5A647150CBA";
createNode follicle -n "pSphere1FollicleShape8453" -p "pSphere1Follicle8453";
	rename -uid "997ADBE6-4784-7FD7-86F5-5BBDF54FFB6B";
	setAttr -k off ".v";
	setAttr ".pu" 0.84375;
	setAttr ".pv" 0.53125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve204" -p "pSphere1Follicle8453";
	rename -uid "B49B1101-488A-77AE-B5AD-29AFFA444CEB";
createNode nurbsCurve -n "curveShape204" -p "curve204";
	rename -uid "49BCE4AE-46FF-11DA-8109-4F880B3AEEFA";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "7D82DC7E-4A5A-F81D-9939-82B053067CBD";
createNode follicle -n "pSphere1FollicleShape8459" -p "pSphere1Follicle8459";
	rename -uid "4F0B6FAC-47C4-13E4-FD15-7BA60AC960CB";
	setAttr -k off ".v";
	setAttr ".pu" 0.84375;
	setAttr ".pv" 0.59375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve205" -p "pSphere1Follicle8459";
	rename -uid "06370EC5-4D59-40D3-6BA2-9D96E98FC119";
createNode nurbsCurve -n "curveShape205" -p "curve205";
	rename -uid "6C97C93F-477B-CDC8-FA18-449C91B877F6";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "3E3F883A-4AFA-5A8B-B17F-11BADB5AFA54";
createNode follicle -n "pSphere1FollicleShape8465" -p "pSphere1Follicle8465";
	rename -uid "EB09BCD3-4583-8E04-254A-A7B2BBDB918F";
	setAttr -k off ".v";
	setAttr ".pu" 0.84375;
	setAttr ".pv" 0.65625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve206" -p "pSphere1Follicle8465";
	rename -uid "88822319-492B-3E06-295F-938715BA4E22";
createNode nurbsCurve -n "curveShape206" -p "curve206";
	rename -uid "1627F6C0-419B-6828-0C3B-C58B238F2CC2";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "D75D3102-4757-3636-95BA-428C5526D09B";
createNode follicle -n "pSphere1FollicleShape8471" -p "pSphere1Follicle8471";
	rename -uid "9B5C7A91-4218-CEB1-5C49-AFB6E61C3475";
	setAttr -k off ".v";
	setAttr ".pu" 0.84375;
	setAttr ".pv" 0.71875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve207" -p "pSphere1Follicle8471";
	rename -uid "018B19BF-448C-7EA9-0DBA-7792E8913035";
createNode nurbsCurve -n "curveShape207" -p "curve207";
	rename -uid "EEF377E4-4BBE-679A-D785-0DA95CF6BDB7";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "544AB3E5-4C9E-F3B7-454E-17AC424CC0AF";
createNode follicle -n "pSphere1FollicleShape8477" -p "pSphere1Follicle8477";
	rename -uid "FCCDAAD0-43C7-2AF3-83ED-6FA2840C0C57";
	setAttr -k off ".v";
	setAttr ".pu" 0.84375;
	setAttr ".pv" 0.78125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve208" -p "pSphere1Follicle8477";
	rename -uid "80B361AD-48DD-8A09-DB3F-838E133A3096";
createNode nurbsCurve -n "curveShape208" -p "curve208";
	rename -uid "ED68E62C-4F36-D214-ED5C-D19A30266F62";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "82EA81E6-43BF-94AA-7D33-F69FE2D8487B";
createNode follicle -n "pSphere1FollicleShape8484" -p "pSphere1Follicle8484";
	rename -uid "395C945A-45EE-FA09-DE41-55A68E5B7B3D";
	setAttr -k off ".v";
	setAttr ".pu" 0.84375;
	setAttr ".pv" 0.84375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve209" -p "pSphere1Follicle8484";
	rename -uid "B5016B35-4233-88E6-59DE-66AB3861498B";
createNode nurbsCurve -n "curveShape209" -p "curve209";
	rename -uid "0E821B77-40F3-B70A-C47F-E2925459D929";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "85BD482E-4865-9D9F-EB65-608B83F6130B";
createNode follicle -n "pSphere1FollicleShape8490" -p "pSphere1Follicle8490";
	rename -uid "DF0BCFA4-4213-ADEC-BA35-ECA7CABD6577";
	setAttr -k off ".v";
	setAttr ".pu" 0.84375;
	setAttr ".pv" 0.90625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve210" -p "pSphere1Follicle8490";
	rename -uid "2E8C02DB-452B-F8CD-0394-76AD6697A1D9";
createNode nurbsCurve -n "curveShape210" -p "curve210";
	rename -uid "2EABC790-45DB-217C-9570-158CD1DB1132";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "E09937FF-48C0-688D-9BDC-E0882F10F849";
createNode follicle -n "pSphere1FollicleShape9009" -p "pSphere1Follicle9009";
	rename -uid "6D1E80F3-494B-FF7B-BC12-7BADA325906B";
	setAttr -k off ".v";
	setAttr ".pu" 0.90625;
	setAttr ".pv" 0.09375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve211" -p "pSphere1Follicle9009";
	rename -uid "27A845DA-4E68-E188-35C0-5B801BB6FAC7";
createNode nurbsCurve -n "curveShape211" -p "curve211";
	rename -uid "9C366874-456C-7164-B37E-E2814B58DF23";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "533005E7-422B-29AC-9077-1CAB76BFDD2D";
createNode follicle -n "pSphere1FollicleShape9015" -p "pSphere1Follicle9015";
	rename -uid "63F5C02C-493B-9EAB-FA1B-5BBF170E6F82";
	setAttr -k off ".v";
	setAttr ".pu" 0.90625;
	setAttr ".pv" 0.15625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve212" -p "pSphere1Follicle9015";
	rename -uid "B40ED0E9-4F60-0880-E01E-FD879E2C4633";
createNode nurbsCurve -n "curveShape212" -p "curve212";
	rename -uid "C0532984-415F-0319-F544-A287766EFBA8";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "67FE73A2-4B7A-D140-B7CA-48839BEB62BA";
createNode follicle -n "pSphere1FollicleShape9022" -p "pSphere1Follicle9022";
	rename -uid "DFAEBE95-4AA9-E947-7075-3F82C11F1B4E";
	setAttr -k off ".v";
	setAttr ".pu" 0.90625;
	setAttr ".pv" 0.21875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve213" -p "pSphere1Follicle9022";
	rename -uid "9D1F0068-43B0-EE46-77FD-AAA7A7AB9CE0";
createNode nurbsCurve -n "curveShape213" -p "curve213";
	rename -uid "28720E9A-4815-CC60-D6E4-28AF6F43C598";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "9B06A2DF-43E7-5692-74CA-0385171819CC";
createNode follicle -n "pSphere1FollicleShape9028" -p "pSphere1Follicle9028";
	rename -uid "9E65B2D6-483A-30B1-88F2-03B6AB74B7E7";
	setAttr -k off ".v";
	setAttr ".pu" 0.90625;
	setAttr ".pv" 0.28125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve214" -p "pSphere1Follicle9028";
	rename -uid "895DCF0E-4557-E685-7365-AFB3FAEAE83B";
createNode nurbsCurve -n "curveShape214" -p "curve214";
	rename -uid "BDDA275D-4458-9468-EFB8-2FA8E4ADE98B";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "34DED66D-4E85-EE02-BAFC-F383C9BF4D23";
createNode follicle -n "pSphere1FollicleShape9034" -p "pSphere1Follicle9034";
	rename -uid "4B98AF18-4C12-FA5F-8EC4-7A8F7B8B31BB";
	setAttr -k off ".v";
	setAttr ".pu" 0.90625;
	setAttr ".pv" 0.34375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve215" -p "pSphere1Follicle9034";
	rename -uid "4D7F8B73-413E-1FD9-6FF6-A58997CA949D";
createNode nurbsCurve -n "curveShape215" -p "curve215";
	rename -uid "0F786F74-47D3-D9D4-8920-8CBF001D3DBB";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "A131E98A-43E6-1410-1C4C-7FA9685E911E";
createNode follicle -n "pSphere1FollicleShape9040" -p "pSphere1Follicle9040";
	rename -uid "41085F0E-47BE-881C-163D-8CAB186A0700";
	setAttr -k off ".v";
	setAttr ".pu" 0.90625;
	setAttr ".pv" 0.40625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve216" -p "pSphere1Follicle9040";
	rename -uid "2AC77273-4BED-C560-701B-B5944AB517CE";
createNode nurbsCurve -n "curveShape216" -p "curve216";
	rename -uid "010C6175-4EEF-8091-F8E1-EAB6C03C3D07";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "82158818-44A5-2626-390E-259ACDFAB556";
createNode follicle -n "pSphere1FollicleShape9046" -p "pSphere1Follicle9046";
	rename -uid "2993B2B4-4196-A8AC-73A2-B9881B1FD207";
	setAttr -k off ".v";
	setAttr ".pu" 0.90625;
	setAttr ".pv" 0.46875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve217" -p "pSphere1Follicle9046";
	rename -uid "7E792222-40EB-F343-0077-FB9598211841";
createNode nurbsCurve -n "curveShape217" -p "curve217";
	rename -uid "90C72684-4B21-C60D-E86D-C0A96251FC97";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "A85C45D0-48D0-9EA5-A638-B49AD8C78D99";
createNode follicle -n "pSphere1FollicleShape9053" -p "pSphere1Follicle9053";
	rename -uid "2DE16461-4F28-7CE6-A94A-D9B6B31BE5C6";
	setAttr -k off ".v";
	setAttr ".pu" 0.90625;
	setAttr ".pv" 0.53125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve218" -p "pSphere1Follicle9053";
	rename -uid "58B23220-4DF0-2DEE-D913-598649E5493D";
createNode nurbsCurve -n "curveShape218" -p "curve218";
	rename -uid "BB8C38F5-408A-43BB-7732-EB8249993054";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "DBA792F2-4C36-EF46-29A9-3584AE0ECA22";
createNode follicle -n "pSphere1FollicleShape9059" -p "pSphere1Follicle9059";
	rename -uid "601244A5-4813-1E82-A1DB-088ABC4451DE";
	setAttr -k off ".v";
	setAttr ".pu" 0.90625;
	setAttr ".pv" 0.59375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve219" -p "pSphere1Follicle9059";
	rename -uid "6D1A2D83-4E06-BDE1-A1CD-DF924110B173";
createNode nurbsCurve -n "curveShape219" -p "curve219";
	rename -uid "527B5D33-4B61-EE83-9D5B-A79D15F566BF";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "980A22FB-4A98-79EC-1EFB-EDA8E31E5EAD";
createNode follicle -n "pSphere1FollicleShape9065" -p "pSphere1Follicle9065";
	rename -uid "A7D8FB10-421B-F69D-A759-09A5D21615CD";
	setAttr -k off ".v";
	setAttr ".pu" 0.90625;
	setAttr ".pv" 0.65625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve220" -p "pSphere1Follicle9065";
	rename -uid "9D69DF06-4BDD-B784-6696-C392626AA9C4";
createNode nurbsCurve -n "curveShape220" -p "curve220";
	rename -uid "A87AF9AB-4DF7-BC85-A676-968CAE3457EA";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "D7DEB58B-4296-A0C4-4A12-6C996B9F305D";
createNode follicle -n "pSphere1FollicleShape9071" -p "pSphere1Follicle9071";
	rename -uid "A5D838FF-4907-045D-C42D-75A726855E1B";
	setAttr -k off ".v";
	setAttr ".pu" 0.90625;
	setAttr ".pv" 0.71875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve221" -p "pSphere1Follicle9071";
	rename -uid "863E9F7D-42A4-CF86-959E-1FAA7E75FD1F";
createNode nurbsCurve -n "curveShape221" -p "curve221";
	rename -uid "126FB983-4F2E-FDC0-9DEF-0EB6456460E8";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "D0602A60-410A-F6AF-5947-31A1118FA03F";
createNode follicle -n "pSphere1FollicleShape9077" -p "pSphere1Follicle9077";
	rename -uid "AF3A00D1-42B1-D2EE-DCF9-E996FED5BF48";
	setAttr -k off ".v";
	setAttr ".pu" 0.90625;
	setAttr ".pv" 0.78125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve222" -p "pSphere1Follicle9077";
	rename -uid "DDA348D7-4EF7-7334-6B8D-CE86560F633E";
createNode nurbsCurve -n "curveShape222" -p "curve222";
	rename -uid "816071F4-41B5-54CB-16D0-D59AE955F892";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "C522BF79-46E6-7F0C-8983-A9B977A05291";
createNode follicle -n "pSphere1FollicleShape9084" -p "pSphere1Follicle9084";
	rename -uid "3EC29996-463A-BDB3-44D3-DDB9C76DFCFC";
	setAttr -k off ".v";
	setAttr ".pu" 0.90625;
	setAttr ".pv" 0.84375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve223" -p "pSphere1Follicle9084";
	rename -uid "2840F665-41E3-3D59-D44B-BC9EDA845F33";
createNode nurbsCurve -n "curveShape223" -p "curve223";
	rename -uid "0527409B-4F5F-31E6-B85B-A2A71CDF0581";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "BBCDE48E-4FA0-584D-8FB5-4A8C047D4D97";
createNode follicle -n "pSphere1FollicleShape9090" -p "pSphere1Follicle9090";
	rename -uid "2B13735F-4B8D-994F-C7B8-CAA15DD7AAAC";
	setAttr -k off ".v";
	setAttr ".pu" 0.90625;
	setAttr ".pv" 0.90625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve224" -p "pSphere1Follicle9090";
	rename -uid "D7522F23-4ABA-E133-9B1F-04ACA09CC0F8";
createNode nurbsCurve -n "curveShape224" -p "curve224";
	rename -uid "470E3300-4B3D-60EC-31E8-42BAAE9A1970";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "A82B4BD5-4790-7CC6-506C-13BD9ECC43C8";
createNode follicle -n "pSphere1FollicleShape9603" -p "pSphere1Follicle9603";
	rename -uid "829A3226-4DB7-8666-783D-00AE72362993";
	setAttr -k off ".v";
	setAttr ".pu" 0.96875;
	setAttr ".pv" 0.03125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve225" -p "pSphere1Follicle9603";
	rename -uid "54C6F3C2-47E1-EAA2-CCCE-6FBF45E977F2";
createNode nurbsCurve -n "curveShape225" -p "curve225";
	rename -uid "CDDF7393-4F8C-E359-1D4A-55A97157299B";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "5C652261-4B93-856E-062C-06BBCCF348C2";
createNode follicle -n "pSphere1FollicleShape9609" -p "pSphere1Follicle9609";
	rename -uid "4EC9E49F-4F4D-DEB5-147F-44AE20E21342";
	setAttr -k off ".v";
	setAttr ".pu" 0.96875;
	setAttr ".pv" 0.09375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve226" -p "pSphere1Follicle9609";
	rename -uid "AE3744DA-4D81-5E77-2A64-36B66E193319";
createNode nurbsCurve -n "curveShape226" -p "curve226";
	rename -uid "EA9F1435-4AE8-E7EA-12B0-8F980B2AA855";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "1F42082B-4B0D-1828-A76C-EB9FAE8D8594";
createNode follicle -n "pSphere1FollicleShape9615" -p "pSphere1Follicle9615";
	rename -uid "FF9840A3-4C8B-4465-419F-F39D17F7CA87";
	setAttr -k off ".v";
	setAttr ".pu" 0.96875;
	setAttr ".pv" 0.15625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve227" -p "pSphere1Follicle9615";
	rename -uid "737131E4-4AB3-F86E-2F96-65B0E5F9B947";
createNode nurbsCurve -n "curveShape227" -p "curve227";
	rename -uid "0BED2F27-4E50-9D18-4A9B-3DAB2ADC0DE7";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "9207C703-4D00-9F3C-092A-5292FB745957";
createNode follicle -n "pSphere1FollicleShape9622" -p "pSphere1Follicle9622";
	rename -uid "301A12B1-4E85-9F46-518A-09B0E21D1EDD";
	setAttr -k off ".v";
	setAttr ".pu" 0.96875;
	setAttr ".pv" 0.21875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve228" -p "pSphere1Follicle9622";
	rename -uid "F9EA1673-4429-4EB6-B36E-F1BA85EB53A4";
createNode nurbsCurve -n "curveShape228" -p "curve228";
	rename -uid "6E5EAF7F-4A98-2E55-2252-1FA619CAFC02";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "B52FE4C4-400B-4705-9A65-3AAFEAECAAC0";
createNode follicle -n "pSphere1FollicleShape9628" -p "pSphere1Follicle9628";
	rename -uid "1EF01531-4CB4-9BB2-3001-5895AF105C2B";
	setAttr -k off ".v";
	setAttr ".pu" 0.96875;
	setAttr ".pv" 0.28125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve229" -p "pSphere1Follicle9628";
	rename -uid "DDFF78B0-42FC-A202-379D-D889707477FE";
createNode nurbsCurve -n "curveShape229" -p "curve229";
	rename -uid "2F2B46C9-4E00-C653-EF76-D294CEF4E2FF";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "6A5FF5B8-4D4D-CF94-98F1-108F331FE00C";
createNode follicle -n "pSphere1FollicleShape9634" -p "pSphere1Follicle9634";
	rename -uid "4878D954-4077-0CF0-9164-13ADE21AFD3F";
	setAttr -k off ".v";
	setAttr ".pu" 0.96875;
	setAttr ".pv" 0.34375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve230" -p "pSphere1Follicle9634";
	rename -uid "C673EC97-452F-015D-E9CB-BF9288E7A175";
createNode nurbsCurve -n "curveShape230" -p "curve230";
	rename -uid "1D8200C9-42EE-CC34-0C15-46AB1AA4486C";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "DBFA3AA2-423B-3EC8-A913-878F20ED4093";
createNode follicle -n "pSphere1FollicleShape9640" -p "pSphere1Follicle9640";
	rename -uid "115714BE-4ED2-B672-AB4A-6793BBB6C805";
	setAttr -k off ".v";
	setAttr ".pu" 0.96875;
	setAttr ".pv" 0.40625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve231" -p "pSphere1Follicle9640";
	rename -uid "D02479A8-42C4-B983-759B-5DAA9D2153F9";
createNode nurbsCurve -n "curveShape231" -p "curve231";
	rename -uid "F236521E-47AB-0FD6-7058-68956ED0C807";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "4FCF8CF3-4D8D-A89B-5166-37A9B68ED43A";
createNode follicle -n "pSphere1FollicleShape9646" -p "pSphere1Follicle9646";
	rename -uid "40C25308-4E5B-AAD1-08E5-A68E50512C25";
	setAttr -k off ".v";
	setAttr ".pu" 0.96875;
	setAttr ".pv" 0.46875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve232" -p "pSphere1Follicle9646";
	rename -uid "C42FBA2B-4458-92C4-3E84-A0B26F43F026";
createNode nurbsCurve -n "curveShape232" -p "curve232";
	rename -uid "66CE8F2A-4533-CA02-D090-26924F6645F1";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "CD1D2921-4B9C-6693-3172-7686F60F0B7C";
createNode follicle -n "pSphere1FollicleShape9653" -p "pSphere1Follicle9653";
	rename -uid "30353420-49C8-1F86-9F09-32A0BFFAA38B";
	setAttr -k off ".v";
	setAttr ".pu" 0.96875;
	setAttr ".pv" 0.53125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve233" -p "pSphere1Follicle9653";
	rename -uid "E1ED5AC1-48DD-A645-AE3A-65B4B59DACD1";
createNode nurbsCurve -n "curveShape233" -p "curve233";
	rename -uid "8B15D585-480F-010F-C9FD-678DBBED07EF";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "FB463FA0-428A-5E2F-3FF0-BA93C4F784C7";
createNode follicle -n "pSphere1FollicleShape9659" -p "pSphere1Follicle9659";
	rename -uid "7B87CD43-4296-97D1-7A58-9491DFAB61FE";
	setAttr -k off ".v";
	setAttr ".pu" 0.96875;
	setAttr ".pv" 0.59375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve234" -p "pSphere1Follicle9659";
	rename -uid "EA729541-4F64-2E29-E41F-F59F8F5542E6";
createNode nurbsCurve -n "curveShape234" -p "curve234";
	rename -uid "F40138AF-4692-96D4-C883-8490868C9A72";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "9774A083-4D10-E100-F6B5-2C89505ED035";
createNode follicle -n "pSphere1FollicleShape9665" -p "pSphere1Follicle9665";
	rename -uid "BE4089A2-4D58-F76C-BB5B-0FA54D580F03";
	setAttr -k off ".v";
	setAttr ".pu" 0.96875;
	setAttr ".pv" 0.65625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve235" -p "pSphere1Follicle9665";
	rename -uid "3272A420-43D8-3A81-D980-389516D7A36A";
createNode nurbsCurve -n "curveShape235" -p "curve235";
	rename -uid "9D15B3B6-4C7D-C156-6BDF-FE92D1CCBF9E";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "87B7F8EA-4827-D905-95AE-32934C998AC5";
createNode follicle -n "pSphere1FollicleShape9671" -p "pSphere1Follicle9671";
	rename -uid "A6AE4E97-4953-255A-7E16-279EA173B45F";
	setAttr -k off ".v";
	setAttr ".pu" 0.96875;
	setAttr ".pv" 0.71875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve236" -p "pSphere1Follicle9671";
	rename -uid "ED9E74DB-4FC7-4240-A5C6-B69C03F2F683";
createNode nurbsCurve -n "curveShape236" -p "curve236";
	rename -uid "B763F1D9-4F0E-DCAA-4330-4787128C791C";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "AB254B41-4B87-57DD-E976-2AB8B74E466A";
createNode follicle -n "pSphere1FollicleShape9677" -p "pSphere1Follicle9677";
	rename -uid "AC640456-4AB2-2210-C599-4FB6B137B14A";
	setAttr -k off ".v";
	setAttr ".pu" 0.96875;
	setAttr ".pv" 0.78125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve237" -p "pSphere1Follicle9677";
	rename -uid "031F7F05-44B3-67FC-321C-43A895217225";
createNode nurbsCurve -n "curveShape237" -p "curve237";
	rename -uid "C4B836C2-4040-F8B5-5E52-3F975324DEBA";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "8419BD89-441E-6F44-C99B-50A2290BDC80";
createNode follicle -n "pSphere1FollicleShape9684" -p "pSphere1Follicle9684";
	rename -uid "0F8A76C2-4565-DA1E-4631-44870D3DCCC5";
	setAttr -k off ".v";
	setAttr ".pu" 0.96875;
	setAttr ".pv" 0.84375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve238" -p "pSphere1Follicle9684";
	rename -uid "8C05468E-4CBE-C9EA-750C-D1A8D0537BD0";
createNode nurbsCurve -n "curveShape238" -p "curve238";
	rename -uid "49F1E176-4437-D572-E76D-268628121413";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "0D814351-4848-A61C-55B5-5691E5AC6B98";
createNode follicle -n "pSphere1FollicleShape9690" -p "pSphere1Follicle9690";
	rename -uid "AE2311A5-4412-D486-3B3D-F387B42F3E61";
	setAttr -k off ".v";
	setAttr ".pu" 0.96875;
	setAttr ".pv" 0.90625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve239" -p "pSphere1Follicle9690";
	rename -uid "4A368825-4F3E-6C33-7AEE-6FA9C13BE103";
createNode nurbsCurve -n "curveShape239" -p "curve239";
	rename -uid "6BB42FA4-4D42-DDBF-2C48-828FD8A1D2C0";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "0FF45DDE-4B2C-8335-D995-D88F4F616616";
createNode follicle -n "pSphere1FollicleShape9696" -p "pSphere1Follicle9696";
	rename -uid "0627203F-4B1A-90B5-1DCD-689F7D5F4B58";
	setAttr -k off ".v";
	setAttr ".pu" 0.96875;
	setAttr ".pv" 0.96875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve240" -p "pSphere1Follicle9696";
	rename -uid "C5D074AB-404A-37B8-0A97-B7BCD2A7D7A7";
createNode nurbsCurve -n "curveShape240" -p "curve240";
	rename -uid "1A04A8FE-4989-52EE-85F6-36AAFA9FF56C";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "C4D579A4-44C2-59A0-41F5-57A25DFFEBCA";
	setAttr ".t" -type "double3" 1.8146580747476841 9.8912227379992164 7.86214094757102 ;
	setAttr ".r" -type "double3" -5.4425294815625227 8.3967523894246963 -15.503503478680965 ;
	setAttr ".s" -type "double3" 0.46149274766668419 0.46149274766668419 0.46149274766668419 ;
createNode pfxHair -n "pfxHairShape1" -p "pfxHair1";
	rename -uid "3FC0613A-44BE-8FB3-212C-0A9731D33901";
	setAttr -k off ".v";
	setAttr ".dpc" 100;
	setAttr ".dam" no;
createNode lightLinker -s -n "lightLinker1";
	rename -uid "93454D4E-4A90-C40E-57E3-178716D51164";
	setAttr -s 4 ".lnk";
	setAttr -s 4 ".slnk";
createNode displayLayerManager -n "layerManager";
	rename -uid "5D451D80-486B-EC71-527A-B68C08EB0836";
createNode displayLayer -n "defaultLayer";
	rename -uid "5C1D97A2-488B-F0F3-3828-368DF31076C9";
createNode renderLayerManager -n "renderLayerManager";
	rename -uid "354D0A2C-4C7C-179A-53FE-EDAD1A652FB6";
createNode renderLayer -n "defaultRenderLayer";
	rename -uid "8DA60B50-4BAE-1BF1-9E8F-80AF357F0E0D";
	setAttr ".g" yes;
createNode aiOptions -s -n "defaultArnoldRenderOptions";
	rename -uid "433D81F7-44E1-8BB8-BE29-FBB32D0C131E";
createNode script -n "uiConfigurationScriptNode";
	rename -uid "C3E07368-452E-9A77-5F1D-6EACD568DEE2";
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
	rename -uid "2D59E3AD-433B-4266-6E6E-DF9658857D3A";
	setAttr ".b" -type "string" "playbackOptions -min 1 -max 24 -ast 1 -aet 48 ";
	setAttr ".st" 6;
createNode shadingEngine -n "aiStandard1SG";
	rename -uid "5F487FA8-4E7B-205A-2D59-61AE5E58A036";
	setAttr ".ihi" 0;
	setAttr ".ro" yes;
createNode materialInfo -n "materialInfo1";
	rename -uid "9845962B-4196-BB6D-FA1C-5CAD99F76ACE";
createNode polySphere -n "polySphere1";
	rename -uid "641D30C2-4C52-F4BA-C024-0E9510C685BF";
	setAttr ".r" 6.3190084466120737;
createNode aiAOVFilter -s -n "defaultArnoldFilter";
	rename -uid "B5480C15-4A2A-8B1B-2A3A-5698A843DED5";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVDriver -s -n "defaultArnoldDriver";
	rename -uid "882EC5B7-4698-9158-115D-669DD5C93B37";
	addAttr -ci true -sn "gamma" -ln "gamma" -dv 2.2000000476837158 -min 9.9999997473787516e-005 
		-smx 5 -at "float";
	addAttr -ci true -sn "dither_amplitude" -ln "ditherAmplitude" -dv 1 -at "float";
	setAttr ".ai_translator" -type "string" "tif";
createNode materialInfo -n "materialInfo2";
	rename -uid "15590058-416F-0B6E-E300-238977059DF8";
createNode aiStandard -n "aiStandard2";
	rename -uid "BE1D4E96-477F-EBA2-7F9A-6F9B66AD1435";
createNode shadingEngine -n "aiStandard2SG";
	rename -uid "C80612EF-4CFB-9FB5-53D2-4CA3792F04BC";
	setAttr ".ihi" 0;
	setAttr ".ro" yes;
createNode materialInfo -n "materialInfo3";
	rename -uid "83947684-43CA-1384-7468-E0B8931C6A25";
createNode shapeEditorManager -n "shapeEditorManager";
	rename -uid "793E87CB-4BAA-B1F1-32EC-9A80E3A39975";
createNode poseInterpolatorManager -n "poseInterpolatorManager";
	rename -uid "7D23B2A1-49AE-286D-366E-A4A89B4E6A27";
createNode aiAOVDriver -s -n "defaultArnoldDisplayDriver";
	rename -uid "066BC911-47B7-693C-0197-03A80E4E221E";
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
select -ne :defaultColorMgtGlobals;
	setAttr ".cme" no;
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

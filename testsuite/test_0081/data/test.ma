//Maya ASCII 2018ff09 scene
//Name: test.ma
//Last modified: Wed, Mar 06, 2019 11:44:06 AM
//Codeset: 1252
requires maya "2018ff09";
requires "stereoCamera" "10.0";
requires -nodeType "aiOptions" -nodeType "aiAOVDriver" -nodeType "aiAOVFilter" -nodeType "aiStandard"
		 "mtoa" "3.2.0.wip";
currentUnit -l centimeter -a degree -t film;
fileInfo "application" "maya";
fileInfo "product" "Maya 2018";
fileInfo "version" "2018";
fileInfo "cutIdentifier" "201807182215-4524384375";
fileInfo "osv" "Microsoft Windows 7 Enterprise Edition, 64-bit Windows 7 Service Pack 1 (Build 7601)\n";
createNode transform -s -n "persp";
	rename -uid "812CB7A0-4C1A-A154-3125-7C8FD1DC2216";
	setAttr ".v" no;
	setAttr ".t" -type "double3" 27.795094234407376 18.971162668046485 0 ;
	setAttr ".r" -type "double3" -24.000000000000004 90 0 ;
	setAttr ".rp" -type "double3" -3.5804692544161291e-15 -7.1054273576010019e-15 -1.4210854715202004e-14 ;
	setAttr ".rpt" -type "double3" -8.7783779362275568e-15 1.1498476670238775e-14 9.4020555375448207e-16 ;
createNode camera -s -n "perspShape" -p "persp";
	rename -uid "DC2680B1-4722-FAD9-0DB8-BEB1AA229CAF";
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
	rename -uid "6767D39B-48F6-235C-5EF7-0183A3EDC567";
	setAttr ".v" no;
	setAttr ".t" -type "double3" 0 100.1 0 ;
	setAttr ".r" -type "double3" -89.999999999999986 0 0 ;
createNode camera -s -n "topShape" -p "top";
	rename -uid "C3A3399D-400A-32F8-8783-93BB78DF158A";
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
	rename -uid "B49E2D75-4368-C9F3-BB9E-8EB47383C6D5";
	setAttr ".v" no;
	setAttr ".t" -type "double3" 0 0 100.1 ;
createNode camera -s -n "frontShape" -p "front";
	rename -uid "0C1CBE36-4CE4-F0F3-6E6A-2AA76C5CA26B";
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
	rename -uid "3F7D3B39-4688-018D-6825-12BAECBE82CF";
	setAttr ".v" no;
	setAttr ".t" -type "double3" 100.1 1.6871751536864319 12.910557697774426 ;
	setAttr ".r" -type "double3" 0 89.999999999999986 0 ;
createNode camera -s -n "sideShape" -p "side";
	rename -uid "0D0AB4A0-4CDB-1DF2-C96B-C7A0E85B03A5";
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
	rename -uid "D796DCC2-4272-676E-A924-FB9C1D636F06";
	setAttr ".t" -type "double3" -0.020665766309299727 6.3190084466120737 0.27269214094604877 ;
createNode mesh -n "pSphereShape1" -p "pSphere1";
	rename -uid "C66A6B9D-440B-B939-0367-BCBBAFD9CBE7";
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
	rename -uid "567B7D8D-4989-E007-FCAE-A5A715F041DB";
	setAttr ".r" -type "double3" -48.156552041713944 1.3059010322141336 -9.5202672381429512 ;
	setAttr ".s" -type "double3" 30.769077416337016 30.769077416337016 30.769077416337016 ;
createNode directionalLight -n "directionalLightShape1" -p "directionalLight1";
	rename -uid "AED9BB77-40CC-D830-FBC2-89980B8D70A5";
	addAttr -ci true -sn "ai_bounce_factor" -ln "aiBounceFactor" -dv 1 -min 0 -smx 
		20 -at "float";
	addAttr -ci true -sn "ai_bounces" -ln "aiBounces" -dv 999 -min 0 -max 10000 -at "long";
	setAttr -k off ".v";
	setAttr ".in" 2;
	setAttr ".urs" no;
	setAttr ".rdl" 1;
createNode transform -n "hairSystem1";
	rename -uid "F8A4F69B-4349-E7E0-4F94-9EAAAFE37736";
createNode hairSystem -n "hairSystemShape1" -p "hairSystem1";
	rename -uid "8B0ACA37-47E2-2C84-7F20-AEB592D0966D";
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
	rename -uid "E84569F7-4F01-E829-80E6-999C81C9E007";
createNode transform -n "pSphere1Follicle303" -p "hairSystem1Follicles";
	rename -uid "E63BA840-4CAE-A358-DF6A-2987110CEE0B";
createNode follicle -n "pSphere1FollicleShape303" -p "pSphere1Follicle303";
	rename -uid "140670FF-44EF-1AF3-B857-12A54650610A";
	setAttr -k off ".v";
	setAttr ".pu" 0.03125;
	setAttr ".pv" 0.03125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve1" -p "pSphere1Follicle303";
	rename -uid "03DC39F1-42BD-204C-25BC-F49491185BB9";
createNode nurbsCurve -n "curveShape1" -p "curve1";
	rename -uid "D29193D9-47D7-772A-9704-A9A3DAC58A91";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "54827085-4F7F-E165-CA83-F4BBA70ECEA4";
createNode follicle -n "pSphere1FollicleShape309" -p "pSphere1Follicle309";
	rename -uid "C6158DAA-42BB-DD08-4E5F-5087715A3BCA";
	setAttr -k off ".v";
	setAttr ".pu" 0.03125;
	setAttr ".pv" 0.09375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve2" -p "pSphere1Follicle309";
	rename -uid "C4F71B15-47C4-CE24-6BB6-F395F5E7A1B4";
createNode nurbsCurve -n "curveShape2" -p "curve2";
	rename -uid "B8F41BF2-4185-7924-586A-C88DF8D072A8";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "4D68917B-4EE8-37E2-FCC6-A0B36F5639B3";
createNode follicle -n "pSphere1FollicleShape315" -p "pSphere1Follicle315";
	rename -uid "0FDCAC46-4A31-E704-99CB-DAAA11B4E83A";
	setAttr -k off ".v";
	setAttr ".pu" 0.03125;
	setAttr ".pv" 0.15625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve3" -p "pSphere1Follicle315";
	rename -uid "C66DF5AA-4A4D-9758-2032-8E9A8D03F793";
createNode nurbsCurve -n "curveShape3" -p "curve3";
	rename -uid "DB7E5762-42B8-AB07-9A1A-378596EFC614";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "AB88CC97-4583-889E-E536-7897B5485A91";
createNode follicle -n "pSphere1FollicleShape322" -p "pSphere1Follicle322";
	rename -uid "8D459675-4C7A-E313-1775-6EA04F6ADB69";
	setAttr -k off ".v";
	setAttr ".pu" 0.03125;
	setAttr ".pv" 0.21875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve4" -p "pSphere1Follicle322";
	rename -uid "02C9C419-4F91-EE08-A41A-FAB0A4344186";
createNode nurbsCurve -n "curveShape4" -p "curve4";
	rename -uid "924D32CC-419F-0E43-4081-E0B51098AC20";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "F3A50C57-4DE8-83FE-C4F1-BCB45A1D8C98";
createNode follicle -n "pSphere1FollicleShape328" -p "pSphere1Follicle328";
	rename -uid "14F6606C-40E5-06D0-D6C0-029AEF141BB1";
	setAttr -k off ".v";
	setAttr ".pu" 0.03125;
	setAttr ".pv" 0.28125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve5" -p "pSphere1Follicle328";
	rename -uid "14C49959-4119-1935-DC8F-32BF1D3FCC80";
createNode nurbsCurve -n "curveShape5" -p "curve5";
	rename -uid "6315A5E3-4154-1269-AE81-B49EC98382F8";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "E8FCF628-4ABF-BCE2-C433-7DB46FD1DE28";
createNode follicle -n "pSphere1FollicleShape334" -p "pSphere1Follicle334";
	rename -uid "DA303B98-425F-6513-E906-2D9A95FC3D46";
	setAttr -k off ".v";
	setAttr ".pu" 0.03125;
	setAttr ".pv" 0.34375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve6" -p "pSphere1Follicle334";
	rename -uid "BE9EBE71-4066-A3DB-B77D-78B8E697FB15";
createNode nurbsCurve -n "curveShape6" -p "curve6";
	rename -uid "A30CE96B-4B2D-C3FC-C204-F0876A3251C0";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "E9BD239C-4CF8-7710-93C9-17903B8C3980";
createNode follicle -n "pSphere1FollicleShape340" -p "pSphere1Follicle340";
	rename -uid "AB6F57EE-4200-ED0D-6A13-ADAE734D86C5";
	setAttr -k off ".v";
	setAttr ".pu" 0.03125;
	setAttr ".pv" 0.40625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve7" -p "pSphere1Follicle340";
	rename -uid "27910C36-49DA-5982-27FE-A681D7E62A5C";
createNode nurbsCurve -n "curveShape7" -p "curve7";
	rename -uid "53FCD23E-41E6-F7B6-AB95-FD851AFF5E24";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "6F6262D7-4048-2FCB-297B-71AC28DA9BF2";
createNode follicle -n "pSphere1FollicleShape346" -p "pSphere1Follicle346";
	rename -uid "4CE90D16-4B11-577A-D359-25BC155DEE48";
	setAttr -k off ".v";
	setAttr ".pu" 0.03125;
	setAttr ".pv" 0.46875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve8" -p "pSphere1Follicle346";
	rename -uid "81896D78-47F4-E7B1-C534-EDBAC75E9928";
createNode nurbsCurve -n "curveShape8" -p "curve8";
	rename -uid "6915EA88-4FD5-3A08-78E4-38824AB4FCC2";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "FA101D2A-483F-4863-69D6-D4A6EFD2D659";
createNode follicle -n "pSphere1FollicleShape353" -p "pSphere1Follicle353";
	rename -uid "B3D8B17E-452B-DA13-6997-BC99CA7BA04F";
	setAttr -k off ".v";
	setAttr ".pu" 0.03125;
	setAttr ".pv" 0.53125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve9" -p "pSphere1Follicle353";
	rename -uid "0F003C4A-4C5F-C334-8102-609CAC5F008C";
createNode nurbsCurve -n "curveShape9" -p "curve9";
	rename -uid "27FE6D5C-42E8-2A74-C6FE-8292A7DF094D";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "5D645967-44AD-8D55-BAC5-21A6FAD34B3D";
createNode follicle -n "pSphere1FollicleShape359" -p "pSphere1Follicle359";
	rename -uid "D6A389B5-48A0-F25C-0B88-5D8CFAA041BF";
	setAttr -k off ".v";
	setAttr ".pu" 0.03125;
	setAttr ".pv" 0.59375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve10" -p "pSphere1Follicle359";
	rename -uid "BEE57DDC-4B28-15CB-373B-95AF959EF7EB";
createNode nurbsCurve -n "curveShape10" -p "curve10";
	rename -uid "7E893324-46FB-E683-F7B7-8CBC0E423B8C";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "E081EE84-4521-9DDC-A02A-A9985FCA1C7B";
createNode follicle -n "pSphere1FollicleShape365" -p "pSphere1Follicle365";
	rename -uid "8CAD73F8-4B7F-0696-0A2E-F8B1F1AB10CD";
	setAttr -k off ".v";
	setAttr ".pu" 0.03125;
	setAttr ".pv" 0.65625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve11" -p "pSphere1Follicle365";
	rename -uid "BC57A6B1-448A-A006-07D1-6CA764CD8010";
createNode nurbsCurve -n "curveShape11" -p "curve11";
	rename -uid "D5D8ABF3-4B6A-865A-A06E-22B43EE70EB3";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "E2BB36B3-4F1F-824F-FCF8-288F6401653B";
createNode follicle -n "pSphere1FollicleShape371" -p "pSphere1Follicle371";
	rename -uid "A4961F82-47AE-F044-82EE-EBBC1B6A1ACC";
	setAttr -k off ".v";
	setAttr ".pu" 0.03125;
	setAttr ".pv" 0.71875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve12" -p "pSphere1Follicle371";
	rename -uid "8DBB444D-48C1-F98D-D303-0EA12F33B8CB";
createNode nurbsCurve -n "curveShape12" -p "curve12";
	rename -uid "B981897D-4E8D-B590-1EE0-AC948B697E99";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "8453BC0E-4E56-9724-FFC4-61B2AE764DEA";
createNode follicle -n "pSphere1FollicleShape377" -p "pSphere1Follicle377";
	rename -uid "EE740E7F-47D8-B186-4C02-448AA795B040";
	setAttr -k off ".v";
	setAttr ".pu" 0.03125;
	setAttr ".pv" 0.78125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve13" -p "pSphere1Follicle377";
	rename -uid "48FDAC82-41B8-7167-E8DF-7C8C99397F2F";
createNode nurbsCurve -n "curveShape13" -p "curve13";
	rename -uid "9474B5C2-4363-1DAA-155E-3E9714C238CD";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "60506BAC-456E-5029-9151-25A32683ED56";
createNode follicle -n "pSphere1FollicleShape384" -p "pSphere1Follicle384";
	rename -uid "D4256B74-4A3C-A69A-29DE-E7A7C5CD1963";
	setAttr -k off ".v";
	setAttr ".pu" 0.03125;
	setAttr ".pv" 0.84375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve14" -p "pSphere1Follicle384";
	rename -uid "02E09663-43E6-261A-5F66-B5B966B13102";
createNode nurbsCurve -n "curveShape14" -p "curve14";
	rename -uid "912E69F0-4214-8B37-2DAF-6F9B4722AC5A";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "34D4CE6D-482A-0064-DD85-D097CA1F80FD";
createNode follicle -n "pSphere1FollicleShape390" -p "pSphere1Follicle390";
	rename -uid "1098450B-4F58-9DB3-A7DE-5D998767EE0C";
	setAttr -k off ".v";
	setAttr ".pu" 0.03125;
	setAttr ".pv" 0.90625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve15" -p "pSphere1Follicle390";
	rename -uid "B3E2181D-403D-0C45-F3F9-D3B0B2A78C3F";
createNode nurbsCurve -n "curveShape15" -p "curve15";
	rename -uid "ADA3536B-4965-75AB-8CDC-54840BBF4F98";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "1B606109-476F-D7F4-25A3-E3AC1616F329";
createNode follicle -n "pSphere1FollicleShape396" -p "pSphere1Follicle396";
	rename -uid "8FA4017C-4019-5B2B-D643-E3AF61C2195D";
	setAttr -k off ".v";
	setAttr ".pu" 0.03125;
	setAttr ".pv" 0.96875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve16" -p "pSphere1Follicle396";
	rename -uid "11EB7A72-4ED2-4B5A-C10B-43A13D04A95A";
createNode nurbsCurve -n "curveShape16" -p "curve16";
	rename -uid "C059E3E8-4F78-9715-AA95-9BA384933F5B";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "40D73F95-4DEE-50BA-546B-17A42D4A7096";
createNode follicle -n "pSphere1FollicleShape909" -p "pSphere1Follicle909";
	rename -uid "DB5D6DBE-4CB2-6147-3214-47B2B8BBBF4C";
	setAttr -k off ".v";
	setAttr ".pu" 0.09375;
	setAttr ".pv" 0.09375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve17" -p "pSphere1Follicle909";
	rename -uid "83754CBA-4F99-BF57-15CA-B8B3CE8E9A2F";
createNode nurbsCurve -n "curveShape17" -p "curve17";
	rename -uid "4F3D179A-45D5-084A-DBF5-43B3945D9487";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "DEF3075C-4F25-93A6-98B1-4197AF561AB6";
createNode follicle -n "pSphere1FollicleShape915" -p "pSphere1Follicle915";
	rename -uid "EF861B00-4EE5-D942-F6C4-E6AE72B02659";
	setAttr -k off ".v";
	setAttr ".pu" 0.09375;
	setAttr ".pv" 0.15625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve18" -p "pSphere1Follicle915";
	rename -uid "2C8D4FD7-4804-8D77-B1B0-368B2E7BE1F7";
createNode nurbsCurve -n "curveShape18" -p "curve18";
	rename -uid "EF6C4DD2-4883-BE83-1DE6-74ACAB3FF008";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "4255F4AB-4DBE-157E-023C-8CB40DE871A0";
createNode follicle -n "pSphere1FollicleShape922" -p "pSphere1Follicle922";
	rename -uid "198E8513-433F-9994-158F-0A9976CD0827";
	setAttr -k off ".v";
	setAttr ".pu" 0.09375;
	setAttr ".pv" 0.21875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve19" -p "pSphere1Follicle922";
	rename -uid "F1E31D94-47B0-11A2-ACD2-A080465EE70D";
createNode nurbsCurve -n "curveShape19" -p "curve19";
	rename -uid "52EE4405-4ECF-A7D3-F36D-2CB002FDBD37";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "6DF16640-4B08-28CA-CCE0-8F9ADF00D814";
createNode follicle -n "pSphere1FollicleShape928" -p "pSphere1Follicle928";
	rename -uid "5CA3F5A5-4448-660A-64E7-6CBBA9B4BBA1";
	setAttr -k off ".v";
	setAttr ".pu" 0.09375;
	setAttr ".pv" 0.28125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve20" -p "pSphere1Follicle928";
	rename -uid "E9BA377F-4C2F-1065-DE7E-C39FE5D0AA6F";
createNode nurbsCurve -n "curveShape20" -p "curve20";
	rename -uid "82A40717-4FF9-493C-0178-54AE444A7C1E";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "99A1F290-43A7-9488-CAFA-F58243597566";
createNode follicle -n "pSphere1FollicleShape934" -p "pSphere1Follicle934";
	rename -uid "8842951C-4858-1F6A-2906-02B5D203F66C";
	setAttr -k off ".v";
	setAttr ".pu" 0.09375;
	setAttr ".pv" 0.34375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve21" -p "pSphere1Follicle934";
	rename -uid "9845B1E7-4D02-6FCA-8516-03BE63E80F9C";
createNode nurbsCurve -n "curveShape21" -p "curve21";
	rename -uid "70579A9E-4A52-F552-6022-7C8DCBE81E75";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "1C412BE0-4A2D-5B65-23EA-BE8A51CEAA44";
createNode follicle -n "pSphere1FollicleShape940" -p "pSphere1Follicle940";
	rename -uid "0B275648-486E-7BCD-C5FE-4197D73AED64";
	setAttr -k off ".v";
	setAttr ".pu" 0.09375;
	setAttr ".pv" 0.40625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve22" -p "pSphere1Follicle940";
	rename -uid "DCCBA6F0-45C4-0EAC-7878-1A82230E01D0";
createNode nurbsCurve -n "curveShape22" -p "curve22";
	rename -uid "EDB7230D-4B30-C70C-9801-E2BA9A4DEEB6";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "60127AD0-4676-64CF-1417-D6BA5E06F282";
createNode follicle -n "pSphere1FollicleShape946" -p "pSphere1Follicle946";
	rename -uid "520D20BB-4657-3E36-E8CF-4A8D0947573D";
	setAttr -k off ".v";
	setAttr ".pu" 0.09375;
	setAttr ".pv" 0.46875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve23" -p "pSphere1Follicle946";
	rename -uid "AA79CB37-485B-75EF-1799-198314F69D0B";
createNode nurbsCurve -n "curveShape23" -p "curve23";
	rename -uid "423A44B7-4298-96EF-1872-BC8E6EDC309A";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "4D283AEF-44D6-8CFD-CB02-208D97DAC546";
createNode follicle -n "pSphere1FollicleShape953" -p "pSphere1Follicle953";
	rename -uid "49354037-4903-F4DF-5920-9C8FEBCD4D29";
	setAttr -k off ".v";
	setAttr ".pu" 0.09375;
	setAttr ".pv" 0.53125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve24" -p "pSphere1Follicle953";
	rename -uid "052F2D88-424E-B36A-8380-AFABFC2FCC16";
createNode nurbsCurve -n "curveShape24" -p "curve24";
	rename -uid "CC8DB496-450E-E788-3CFB-9C9B65613455";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "5BE3A9AC-40C0-0429-93F7-2D9BF18AFBCF";
createNode follicle -n "pSphere1FollicleShape959" -p "pSphere1Follicle959";
	rename -uid "7E5B0126-4835-9996-99F1-499ED997E1B3";
	setAttr -k off ".v";
	setAttr ".pu" 0.09375;
	setAttr ".pv" 0.59375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve25" -p "pSphere1Follicle959";
	rename -uid "8755A6A1-449D-B6F5-1402-90A1B9D39818";
createNode nurbsCurve -n "curveShape25" -p "curve25";
	rename -uid "12BCD649-46C8-1014-C155-05A37B043DE0";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "69401127-4D5E-0256-0EBB-2BBDD19306B6";
createNode follicle -n "pSphere1FollicleShape965" -p "pSphere1Follicle965";
	rename -uid "3F86813C-4382-3121-29D0-D6B5C515BF57";
	setAttr -k off ".v";
	setAttr ".pu" 0.09375;
	setAttr ".pv" 0.65625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve26" -p "pSphere1Follicle965";
	rename -uid "9E3FE94F-4BCD-8DA9-2C7A-69A919B50425";
createNode nurbsCurve -n "curveShape26" -p "curve26";
	rename -uid "A273F177-4626-174D-8D68-359A37CE940A";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "D862DF31-4C08-5806-8211-4E8FCFE41224";
createNode follicle -n "pSphere1FollicleShape971" -p "pSphere1Follicle971";
	rename -uid "2254F318-4DAD-FD58-2FE1-9E9501E09C8E";
	setAttr -k off ".v";
	setAttr ".pu" 0.09375;
	setAttr ".pv" 0.71875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve27" -p "pSphere1Follicle971";
	rename -uid "4726CD2F-4AE9-F6E3-1100-B4928517D854";
createNode nurbsCurve -n "curveShape27" -p "curve27";
	rename -uid "4542BABB-4CC0-2C3D-4C54-90BE7AF0A112";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "79A8DA5A-4534-056A-A416-03887ACAB57F";
createNode follicle -n "pSphere1FollicleShape977" -p "pSphere1Follicle977";
	rename -uid "0D4A1625-4D2B-D3F2-49AF-9BBDDD390E5A";
	setAttr -k off ".v";
	setAttr ".pu" 0.09375;
	setAttr ".pv" 0.78125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve28" -p "pSphere1Follicle977";
	rename -uid "54C12DF8-40E1-CEE4-0722-45A4A28E3B33";
createNode nurbsCurve -n "curveShape28" -p "curve28";
	rename -uid "1D584873-41FA-8F9F-D88C-2892DDB7CC90";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "AB8A6110-465D-054E-65E1-68AC65F22F7E";
createNode follicle -n "pSphere1FollicleShape984" -p "pSphere1Follicle984";
	rename -uid "59CB1B95-4828-6791-B634-DC828FD33B00";
	setAttr -k off ".v";
	setAttr ".pu" 0.09375;
	setAttr ".pv" 0.84375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve29" -p "pSphere1Follicle984";
	rename -uid "1BE9BDA5-4CF8-10B2-BD59-F1B096A0CB70";
createNode nurbsCurve -n "curveShape29" -p "curve29";
	rename -uid "0868905E-4CC4-1BDD-4F5B-18ADA4EA085B";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "A1C5B996-45DF-9A25-AA46-FC8D4DB83895";
createNode follicle -n "pSphere1FollicleShape990" -p "pSphere1Follicle990";
	rename -uid "41D7D3EA-401C-8E06-6FDE-E3981790D93D";
	setAttr -k off ".v";
	setAttr ".pu" 0.09375;
	setAttr ".pv" 0.90625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve30" -p "pSphere1Follicle990";
	rename -uid "F9526AF1-42BA-CFC4-7189-AB885B2DF9BB";
createNode nurbsCurve -n "curveShape30" -p "curve30";
	rename -uid "6014C3E1-4A72-EDF7-DCCD-518A33218FD1";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "2ADCEE63-4010-04B4-0055-C4B552C2A750";
createNode follicle -n "pSphere1FollicleShape1509" -p "pSphere1Follicle1509";
	rename -uid "0E353FD3-4E82-27C9-653E-1E913AC6A64F";
	setAttr -k off ".v";
	setAttr ".pu" 0.15625;
	setAttr ".pv" 0.09375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve31" -p "pSphere1Follicle1509";
	rename -uid "C1A06771-4A5C-75CD-6066-E89EB57CA27D";
createNode nurbsCurve -n "curveShape31" -p "curve31";
	rename -uid "561EA592-4D02-744F-FA62-57B3A4B8CA67";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "70875BEB-4522-B1BA-6CB0-A0A331CE7AB5";
createNode follicle -n "pSphere1FollicleShape1515" -p "pSphere1Follicle1515";
	rename -uid "5F779328-47E1-3ED2-F68B-CA9E5060AAE3";
	setAttr -k off ".v";
	setAttr ".pu" 0.15625;
	setAttr ".pv" 0.15625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve32" -p "pSphere1Follicle1515";
	rename -uid "7C48EDBC-478D-194F-7FE4-26B0B837ADFF";
createNode nurbsCurve -n "curveShape32" -p "curve32";
	rename -uid "CF00286A-499D-C3DD-BBDA-1DBBCB2A6835";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "26D33698-4801-1F1F-7CC3-829101FB0138";
createNode follicle -n "pSphere1FollicleShape1522" -p "pSphere1Follicle1522";
	rename -uid "A8558857-4F5E-4975-9441-20BCEBD61871";
	setAttr -k off ".v";
	setAttr ".pu" 0.15625;
	setAttr ".pv" 0.21875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve33" -p "pSphere1Follicle1522";
	rename -uid "D931E1F2-4D6A-FDE7-8D2D-A4900B027943";
createNode nurbsCurve -n "curveShape33" -p "curve33";
	rename -uid "A63F9E5A-452D-4666-C9BF-3EB004C83B35";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "DB539A54-49FF-CF10-956B-37A5CC65FD43";
createNode follicle -n "pSphere1FollicleShape1528" -p "pSphere1Follicle1528";
	rename -uid "D14E9CCE-4D1C-1E91-A10E-F783D8EC76A3";
	setAttr -k off ".v";
	setAttr ".pu" 0.15625;
	setAttr ".pv" 0.28125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve34" -p "pSphere1Follicle1528";
	rename -uid "6D724F27-4396-7486-F1E5-25AEFB74240C";
createNode nurbsCurve -n "curveShape34" -p "curve34";
	rename -uid "2F690194-4648-B731-7FD8-48851699E105";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "FB5BA961-489A-4915-CA8C-578B11A9BD17";
createNode follicle -n "pSphere1FollicleShape1534" -p "pSphere1Follicle1534";
	rename -uid "8E1C5BDB-4E9F-A446-718B-D2BB3DD3DAFF";
	setAttr -k off ".v";
	setAttr ".pu" 0.15625;
	setAttr ".pv" 0.34375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve35" -p "pSphere1Follicle1534";
	rename -uid "C9302BD8-4434-13F4-E15D-6DAFDB2F3649";
createNode nurbsCurve -n "curveShape35" -p "curve35";
	rename -uid "F1A820AC-4CD9-E329-B171-4B8591E1001D";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "F0F9D1CE-4A23-0458-3C84-91B33959D30B";
createNode follicle -n "pSphere1FollicleShape1540" -p "pSphere1Follicle1540";
	rename -uid "C580FBA7-4652-26D0-5B0C-179C02296D7A";
	setAttr -k off ".v";
	setAttr ".pu" 0.15625;
	setAttr ".pv" 0.40625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve36" -p "pSphere1Follicle1540";
	rename -uid "63150949-453D-1DDD-699C-1F9EDBC608FC";
createNode nurbsCurve -n "curveShape36" -p "curve36";
	rename -uid "D29324C4-46C4-EAF9-A0A6-FCBFC1B40689";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "987847B7-44A4-A04E-9C7E-4FAB83FA3461";
createNode follicle -n "pSphere1FollicleShape1546" -p "pSphere1Follicle1546";
	rename -uid "1ED17152-4197-1E2C-EEC1-D4AD10E387C8";
	setAttr -k off ".v";
	setAttr ".pu" 0.15625;
	setAttr ".pv" 0.46875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve37" -p "pSphere1Follicle1546";
	rename -uid "AB1B2E9F-4B33-B32D-E1EB-B997C0B2FA89";
createNode nurbsCurve -n "curveShape37" -p "curve37";
	rename -uid "0B79B7B3-4D74-E277-F2C2-718E088150FD";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "D49F5BA2-49F2-FC8B-8D2C-1CADBF0B9932";
createNode follicle -n "pSphere1FollicleShape1553" -p "pSphere1Follicle1553";
	rename -uid "5E067771-4E3B-4816-ACC6-E780D1B44C10";
	setAttr -k off ".v";
	setAttr ".pu" 0.15625;
	setAttr ".pv" 0.53125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve38" -p "pSphere1Follicle1553";
	rename -uid "1AEB2284-4C6B-D09C-B8E4-69B4F07DAEC0";
createNode nurbsCurve -n "curveShape38" -p "curve38";
	rename -uid "55004255-43C2-7B53-26DA-9C957F15A8D1";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "7B8AB46D-43DD-E8CF-9258-C4968D92B4D5";
createNode follicle -n "pSphere1FollicleShape1559" -p "pSphere1Follicle1559";
	rename -uid "5ECF9C43-4482-3E4C-BCF3-F59EF954DC5C";
	setAttr -k off ".v";
	setAttr ".pu" 0.15625;
	setAttr ".pv" 0.59375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve39" -p "pSphere1Follicle1559";
	rename -uid "EE77557B-479E-2F69-31EF-1CA43EED3A61";
createNode nurbsCurve -n "curveShape39" -p "curve39";
	rename -uid "D6CA96E0-4B60-AAFF-17B0-1CBE9BF301A7";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "0A3612CD-496D-C4A8-F30E-4EA307DEF983";
createNode follicle -n "pSphere1FollicleShape1565" -p "pSphere1Follicle1565";
	rename -uid "66DB6C67-49AF-AE63-71F2-57BD37343E25";
	setAttr -k off ".v";
	setAttr ".pu" 0.15625;
	setAttr ".pv" 0.65625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve40" -p "pSphere1Follicle1565";
	rename -uid "15E35926-4417-DF05-8E14-709055CBE43A";
createNode nurbsCurve -n "curveShape40" -p "curve40";
	rename -uid "6223634C-47DB-92D7-4B0D-33A448B2F038";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "42C00B73-4C5C-5F28-75BA-659A6188DB27";
createNode follicle -n "pSphere1FollicleShape1571" -p "pSphere1Follicle1571";
	rename -uid "29C853FC-435A-E6C1-0428-78ABBD1EB524";
	setAttr -k off ".v";
	setAttr ".pu" 0.15625;
	setAttr ".pv" 0.71875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve41" -p "pSphere1Follicle1571";
	rename -uid "21DA14A6-4844-F73C-9C33-60AC9570F2C5";
createNode nurbsCurve -n "curveShape41" -p "curve41";
	rename -uid "3E5BDE80-4853-D6FA-5BF8-8183B8D78025";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "9D7F2DBD-4A21-3C0C-B0CF-E6BE3BA94BA3";
createNode follicle -n "pSphere1FollicleShape1577" -p "pSphere1Follicle1577";
	rename -uid "7411FA53-42B9-8741-08BE-3E86B9D5B37A";
	setAttr -k off ".v";
	setAttr ".pu" 0.15625;
	setAttr ".pv" 0.78125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve42" -p "pSphere1Follicle1577";
	rename -uid "0D74471E-4351-B980-4620-198561F38804";
createNode nurbsCurve -n "curveShape42" -p "curve42";
	rename -uid "3F9EFD63-4C18-93A3-838E-5590491FD49B";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "983B25A5-4254-3242-FE9D-DFBEBD1F20D9";
createNode follicle -n "pSphere1FollicleShape1584" -p "pSphere1Follicle1584";
	rename -uid "D6AE288E-40E3-C9B1-F85D-EDB91EDBD8C2";
	setAttr -k off ".v";
	setAttr ".pu" 0.15625;
	setAttr ".pv" 0.84375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve43" -p "pSphere1Follicle1584";
	rename -uid "DCD87339-4D62-B35F-6F68-A8BD7DA3CDF1";
createNode nurbsCurve -n "curveShape43" -p "curve43";
	rename -uid "FF50CE36-4319-4C85-D25E-77B34139446D";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "5C1D595A-402C-FFFC-AF8A-18B7890CDE6C";
createNode follicle -n "pSphere1FollicleShape1590" -p "pSphere1Follicle1590";
	rename -uid "B2220B9A-4A10-7CC9-7F0A-49A2BB3E58B6";
	setAttr -k off ".v";
	setAttr ".pu" 0.15625;
	setAttr ".pv" 0.90625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve44" -p "pSphere1Follicle1590";
	rename -uid "21915CED-433B-9946-F24C-F482840B2AF3";
createNode nurbsCurve -n "curveShape44" -p "curve44";
	rename -uid "8AE99D60-4D6B-6FE9-5930-B5A6E27FC2A7";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "A21C7169-47F3-8EE7-7BCA-C0B829BCD2D4";
createNode follicle -n "pSphere1FollicleShape2203" -p "pSphere1Follicle2203";
	rename -uid "C11028DB-426C-5495-9F32-BBADF8B462DD";
	setAttr -k off ".v";
	setAttr ".pu" 0.21875;
	setAttr ".pv" 0.03125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve45" -p "pSphere1Follicle2203";
	rename -uid "78ED6F62-4904-B5E0-1347-909147067533";
createNode nurbsCurve -n "curveShape45" -p "curve45";
	rename -uid "EDCCA273-4891-6A25-C298-1DAE3620AE1D";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "B0DC73FB-4559-1086-EBF5-9FB219186D2A";
createNode follicle -n "pSphere1FollicleShape2209" -p "pSphere1Follicle2209";
	rename -uid "EDC6E344-40B9-543C-A18D-00B02A0A7834";
	setAttr -k off ".v";
	setAttr ".pu" 0.21875;
	setAttr ".pv" 0.09375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve46" -p "pSphere1Follicle2209";
	rename -uid "23C260F2-474C-8E46-9D3C-0EBFFC5AFAEA";
createNode nurbsCurve -n "curveShape46" -p "curve46";
	rename -uid "212C1DB9-4A53-2B17-6A1D-62AFCCDB4323";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "B00C60EC-4740-20A9-0DF8-D19400D5A725";
createNode follicle -n "pSphere1FollicleShape2215" -p "pSphere1Follicle2215";
	rename -uid "9B31C784-4351-803F-7B13-2180E2FCED27";
	setAttr -k off ".v";
	setAttr ".pu" 0.21875;
	setAttr ".pv" 0.15625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve47" -p "pSphere1Follicle2215";
	rename -uid "BCF885C9-4659-69FC-3E53-2997EC901C9D";
createNode nurbsCurve -n "curveShape47" -p "curve47";
	rename -uid "CEA753AE-47BF-46E5-1939-49896FD649B0";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "B9B1BB06-4DC0-25F6-4A09-35A07551EA1D";
createNode follicle -n "pSphere1FollicleShape2222" -p "pSphere1Follicle2222";
	rename -uid "F290E1BE-4C0C-9240-DA23-22BB66A321FF";
	setAttr -k off ".v";
	setAttr ".pu" 0.21875;
	setAttr ".pv" 0.21875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve48" -p "pSphere1Follicle2222";
	rename -uid "219B3676-413B-DACD-5CA7-D29E2B82B43F";
createNode nurbsCurve -n "curveShape48" -p "curve48";
	rename -uid "44DFDD65-4A30-9E86-3300-D99AAA549DAB";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "F01A54C3-43F8-E376-B7B4-2C8B427884DC";
createNode follicle -n "pSphere1FollicleShape2228" -p "pSphere1Follicle2228";
	rename -uid "FBA63889-45BD-EB52-535E-618894662F5C";
	setAttr -k off ".v";
	setAttr ".pu" 0.21875;
	setAttr ".pv" 0.28125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve49" -p "pSphere1Follicle2228";
	rename -uid "43D8DFAA-47D6-29A9-D53E-3A9B1D12A60D";
createNode nurbsCurve -n "curveShape49" -p "curve49";
	rename -uid "28DA338A-4698-C85B-319D-75AA286576F3";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "B984FB3D-4B17-2DD5-AC8B-9B9E12A84D0F";
createNode follicle -n "pSphere1FollicleShape2234" -p "pSphere1Follicle2234";
	rename -uid "BC525B70-40ED-A898-6E8F-328A36B6FB59";
	setAttr -k off ".v";
	setAttr ".pu" 0.21875;
	setAttr ".pv" 0.34375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve50" -p "pSphere1Follicle2234";
	rename -uid "556C53D6-4C22-B01A-1163-37889D02DCDE";
createNode nurbsCurve -n "curveShape50" -p "curve50";
	rename -uid "DEEBB689-4E7C-D7B7-5BF2-E0B295C7FAF9";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "CE523811-4ABC-0E7A-BBDB-30BDC36E8FAA";
createNode follicle -n "pSphere1FollicleShape2240" -p "pSphere1Follicle2240";
	rename -uid "44FF2047-419D-39B9-611B-A7BD6E18F8A5";
	setAttr -k off ".v";
	setAttr ".pu" 0.21875;
	setAttr ".pv" 0.40625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve51" -p "pSphere1Follicle2240";
	rename -uid "24E07872-4F9D-7A0A-3CD7-FD857938C472";
createNode nurbsCurve -n "curveShape51" -p "curve51";
	rename -uid "DF6474DA-4A6E-7F30-7A9D-53892EF63C8E";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "061945CE-4059-556F-4057-4D9973D168B6";
createNode follicle -n "pSphere1FollicleShape2246" -p "pSphere1Follicle2246";
	rename -uid "005A649C-4E02-00B6-02E4-27B724BD278F";
	setAttr -k off ".v";
	setAttr ".pu" 0.21875;
	setAttr ".pv" 0.46875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve52" -p "pSphere1Follicle2246";
	rename -uid "A27BB964-4CDD-1933-699B-E3BEB1D3D4ED";
createNode nurbsCurve -n "curveShape52" -p "curve52";
	rename -uid "89BE5D11-49BA-721F-E2DE-9B85DC4C668F";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "ED39926F-4680-C6C9-EE50-4DA7080C9E6D";
createNode follicle -n "pSphere1FollicleShape2253" -p "pSphere1Follicle2253";
	rename -uid "3C9E9BC5-4151-E67D-EA6B-9FAC728B5FCB";
	setAttr -k off ".v";
	setAttr ".pu" 0.21875;
	setAttr ".pv" 0.53125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve53" -p "pSphere1Follicle2253";
	rename -uid "78D0AE60-44B8-9FAC-47EF-8498B1A33ACE";
createNode nurbsCurve -n "curveShape53" -p "curve53";
	rename -uid "6981C80D-4F69-BE16-1605-3B808F940677";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "65BD90EF-4299-6EFB-4C7D-3E9866985C2D";
createNode follicle -n "pSphere1FollicleShape2259" -p "pSphere1Follicle2259";
	rename -uid "E0639E1E-4000-2F4F-0115-6881F13C18EC";
	setAttr -k off ".v";
	setAttr ".pu" 0.21875;
	setAttr ".pv" 0.59375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve54" -p "pSphere1Follicle2259";
	rename -uid "A98D6209-434E-B144-F9EA-0D958E58F023";
createNode nurbsCurve -n "curveShape54" -p "curve54";
	rename -uid "EC5ECAD1-49CF-DBC6-E935-2D9D4CE0C0EC";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "2289B04D-41A4-FD34-681B-059DCE40C684";
createNode follicle -n "pSphere1FollicleShape2265" -p "pSphere1Follicle2265";
	rename -uid "94A97CA9-44A4-049C-B8D4-0591302AC206";
	setAttr -k off ".v";
	setAttr ".pu" 0.21875;
	setAttr ".pv" 0.65625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve55" -p "pSphere1Follicle2265";
	rename -uid "B307AEF3-4BF9-EE02-0D05-55A6E1899D93";
createNode nurbsCurve -n "curveShape55" -p "curve55";
	rename -uid "4D758B27-4891-4273-3F10-938DC3E1E9A7";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "8A5B4BAE-48E8-B82A-A093-0AAE22920516";
createNode follicle -n "pSphere1FollicleShape2271" -p "pSphere1Follicle2271";
	rename -uid "E4F7253B-4EA9-C81F-493E-6FB3169D6E7E";
	setAttr -k off ".v";
	setAttr ".pu" 0.21875;
	setAttr ".pv" 0.71875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve56" -p "pSphere1Follicle2271";
	rename -uid "813ECBFB-4B53-F4FE-655A-0DA1188E5BA2";
createNode nurbsCurve -n "curveShape56" -p "curve56";
	rename -uid "8D4AD3EC-4256-1AB6-D8FE-DB805A7BE1B7";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "B31BA34F-4D7F-CE93-2985-03A55FA7A132";
createNode follicle -n "pSphere1FollicleShape2277" -p "pSphere1Follicle2277";
	rename -uid "BF411802-4E0D-0C5C-F945-BAA4453C6A30";
	setAttr -k off ".v";
	setAttr ".pu" 0.21875;
	setAttr ".pv" 0.78125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve57" -p "pSphere1Follicle2277";
	rename -uid "6B26B32F-462A-6F96-2E8F-F38A9FE7EFAF";
createNode nurbsCurve -n "curveShape57" -p "curve57";
	rename -uid "67323A5F-4A11-FC95-2582-8191C0FA3E24";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "483798BF-4912-2AB9-AF16-38908D23AE70";
createNode follicle -n "pSphere1FollicleShape2284" -p "pSphere1Follicle2284";
	rename -uid "13B9CBC6-4D44-15FB-AC06-F99F76136440";
	setAttr -k off ".v";
	setAttr ".pu" 0.21875;
	setAttr ".pv" 0.84375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve58" -p "pSphere1Follicle2284";
	rename -uid "3692EB58-4BD1-172A-5C35-C5813C8BA66D";
createNode nurbsCurve -n "curveShape58" -p "curve58";
	rename -uid "B8C8A8AC-49FB-AE07-BAB4-6A972AAA5C00";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "6A62EE1A-48FC-5E66-7896-B1856CF99B94";
createNode follicle -n "pSphere1FollicleShape2290" -p "pSphere1Follicle2290";
	rename -uid "51261126-4994-E113-F67D-8C9F3BD2E636";
	setAttr -k off ".v";
	setAttr ".pu" 0.21875;
	setAttr ".pv" 0.90625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve59" -p "pSphere1Follicle2290";
	rename -uid "6290A079-4FEB-5FB3-E811-279FECBE3129";
createNode nurbsCurve -n "curveShape59" -p "curve59";
	rename -uid "9F2D48C5-4C9F-A934-9B7D-17B674EEA887";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "A7A3E067-4960-59BB-8CA3-85915C3A4C19";
createNode follicle -n "pSphere1FollicleShape2296" -p "pSphere1Follicle2296";
	rename -uid "24CBCAB2-47B9-D137-794E-449D5EB76672";
	setAttr -k off ".v";
	setAttr ".pu" 0.21875;
	setAttr ".pv" 0.96875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve60" -p "pSphere1Follicle2296";
	rename -uid "F4700938-47C0-2762-ECE6-96934C8C8A0C";
createNode nurbsCurve -n "curveShape60" -p "curve60";
	rename -uid "FC5C5FEE-4BD1-DEB9-3E87-37973E040BFD";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "075A2249-4104-6A46-0DD4-57BE01CA5947";
createNode follicle -n "pSphere1FollicleShape2803" -p "pSphere1Follicle2803";
	rename -uid "B4EEDFB8-4D11-F6DC-F79B-6F948922FC40";
	setAttr -k off ".v";
	setAttr ".pu" 0.28125;
	setAttr ".pv" 0.03125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve61" -p "pSphere1Follicle2803";
	rename -uid "07E5B548-41FD-CC67-6627-ABBC44E71E61";
createNode nurbsCurve -n "curveShape61" -p "curve61";
	rename -uid "1FDAFD78-48FA-C588-A703-F8AFE7B888C0";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "49366C10-49F7-EDFB-9E95-68B2935693DE";
createNode follicle -n "pSphere1FollicleShape2809" -p "pSphere1Follicle2809";
	rename -uid "5C143491-433A-21C1-6854-FA9FA3C294BA";
	setAttr -k off ".v";
	setAttr ".pu" 0.28125;
	setAttr ".pv" 0.09375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve62" -p "pSphere1Follicle2809";
	rename -uid "51E0908D-4EDE-3742-CC72-148D0C976FE6";
createNode nurbsCurve -n "curveShape62" -p "curve62";
	rename -uid "8F2E64D6-46E9-2F09-C3C1-389535246394";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "A4F2C0A6-49E0-98BE-3C5F-0690D45E124F";
createNode follicle -n "pSphere1FollicleShape2815" -p "pSphere1Follicle2815";
	rename -uid "9A058F80-41A3-C905-0992-958637D54758";
	setAttr -k off ".v";
	setAttr ".pu" 0.28125;
	setAttr ".pv" 0.15625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve63" -p "pSphere1Follicle2815";
	rename -uid "5EC448B7-4EB1-32AA-22A0-609F3E4B3F05";
createNode nurbsCurve -n "curveShape63" -p "curve63";
	rename -uid "C4CD9D13-4491-E6F4-1800-08BD1B0C371E";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "7755DDDD-4C7D-D829-A9E2-6191569E6356";
createNode follicle -n "pSphere1FollicleShape2822" -p "pSphere1Follicle2822";
	rename -uid "EB2E95AE-4E0D-EE7D-14FB-3FAE01AD4997";
	setAttr -k off ".v";
	setAttr ".pu" 0.28125;
	setAttr ".pv" 0.21875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve64" -p "pSphere1Follicle2822";
	rename -uid "9D60D33D-4EE7-5820-C094-9A8924D8BF8A";
createNode nurbsCurve -n "curveShape64" -p "curve64";
	rename -uid "EA1B2864-4154-6D94-8842-4F8A3D378DD6";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "FF33EDE8-4E7D-299A-3F9B-BEB0A6AF1EB0";
createNode follicle -n "pSphere1FollicleShape2828" -p "pSphere1Follicle2828";
	rename -uid "1F325F50-4DA2-52CD-4153-34A776BE8C76";
	setAttr -k off ".v";
	setAttr ".pu" 0.28125;
	setAttr ".pv" 0.28125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve65" -p "pSphere1Follicle2828";
	rename -uid "9420D7C2-42C5-F81D-C0BC-6B951D3B22B1";
createNode nurbsCurve -n "curveShape65" -p "curve65";
	rename -uid "894E68EF-4E32-1A9F-EA10-5A90F00D74CF";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "018BDE6C-4500-6C56-A876-1994ECF22E89";
createNode follicle -n "pSphere1FollicleShape2834" -p "pSphere1Follicle2834";
	rename -uid "D55C0B24-423B-0962-0F3D-458334C2AD4E";
	setAttr -k off ".v";
	setAttr ".pu" 0.28125;
	setAttr ".pv" 0.34375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve66" -p "pSphere1Follicle2834";
	rename -uid "AC286D8B-49AB-2DCA-8BFC-AFA1B6266933";
createNode nurbsCurve -n "curveShape66" -p "curve66";
	rename -uid "EB8988F9-4287-737D-8575-8AA834C07BD0";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "DC564B58-4963-E19C-E0BD-EB8230F561B9";
createNode follicle -n "pSphere1FollicleShape2840" -p "pSphere1Follicle2840";
	rename -uid "741FFC36-46EC-AC63-1023-6AB7CBFBF9C7";
	setAttr -k off ".v";
	setAttr ".pu" 0.28125;
	setAttr ".pv" 0.40625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve67" -p "pSphere1Follicle2840";
	rename -uid "702BC265-4CF3-D5ED-63BD-3B9D471B20DD";
createNode nurbsCurve -n "curveShape67" -p "curve67";
	rename -uid "56CBA0A6-4A11-87D0-2DA1-75A3BC79DA0E";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "081F5616-41F3-C10C-197F-66974EC1E90D";
createNode follicle -n "pSphere1FollicleShape2846" -p "pSphere1Follicle2846";
	rename -uid "1D6784D8-4270-A081-CD74-3FB4A4567FBE";
	setAttr -k off ".v";
	setAttr ".pu" 0.28125;
	setAttr ".pv" 0.46875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve68" -p "pSphere1Follicle2846";
	rename -uid "622E685B-4DBA-BE9A-74B7-43A4FDB0AF49";
createNode nurbsCurve -n "curveShape68" -p "curve68";
	rename -uid "4A075384-4100-0205-FBF0-1697D94B2221";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "3718F5B0-4E2E-F071-065C-8FA4D3DEB53C";
createNode follicle -n "pSphere1FollicleShape2853" -p "pSphere1Follicle2853";
	rename -uid "FDD481D5-410D-79E2-B4A9-198396B0C766";
	setAttr -k off ".v";
	setAttr ".pu" 0.28125;
	setAttr ".pv" 0.53125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve69" -p "pSphere1Follicle2853";
	rename -uid "B745A5F5-446C-D4A5-094C-488C60794F76";
createNode nurbsCurve -n "curveShape69" -p "curve69";
	rename -uid "FA2A2D1E-4AD2-46B7-DF3E-DCAF315A9AD5";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "B69CB898-4F76-4F59-C7B1-D7B9EC71F02A";
createNode follicle -n "pSphere1FollicleShape2859" -p "pSphere1Follicle2859";
	rename -uid "C25794E7-4D05-A546-E4DB-6BBE57F1A746";
	setAttr -k off ".v";
	setAttr ".pu" 0.28125;
	setAttr ".pv" 0.59375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve70" -p "pSphere1Follicle2859";
	rename -uid "49A47C30-47B6-004C-85ED-12815B0AC6C7";
createNode nurbsCurve -n "curveShape70" -p "curve70";
	rename -uid "FE628360-4353-CE4B-818B-87A529CED4E6";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "3BAAB24D-431D-F7E5-68BF-45AC54FF5864";
createNode follicle -n "pSphere1FollicleShape2865" -p "pSphere1Follicle2865";
	rename -uid "788342F4-401B-AD12-0EEA-A5B3602EC554";
	setAttr -k off ".v";
	setAttr ".pu" 0.28125;
	setAttr ".pv" 0.65625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve71" -p "pSphere1Follicle2865";
	rename -uid "6C33AF72-4DE1-C7C7-F485-D9A6A7B4FC84";
createNode nurbsCurve -n "curveShape71" -p "curve71";
	rename -uid "1D344F13-4307-7C4D-BE85-7F9A91BF1E32";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "D6A8E05F-4CC2-7500-0132-15A32E4DE5A5";
createNode follicle -n "pSphere1FollicleShape2871" -p "pSphere1Follicle2871";
	rename -uid "D407E78E-4C78-9450-2D7D-718233210CCC";
	setAttr -k off ".v";
	setAttr ".pu" 0.28125;
	setAttr ".pv" 0.71875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve72" -p "pSphere1Follicle2871";
	rename -uid "F6442CFE-4C34-81ED-5BDC-40BB145A0868";
createNode nurbsCurve -n "curveShape72" -p "curve72";
	rename -uid "D0D1B741-4766-2E13-C179-B5AFDA85D913";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "6D19101B-4616-F549-9974-66BC44331725";
createNode follicle -n "pSphere1FollicleShape2877" -p "pSphere1Follicle2877";
	rename -uid "8E54BC15-429A-81AD-C0E2-A39E7C643537";
	setAttr -k off ".v";
	setAttr ".pu" 0.28125;
	setAttr ".pv" 0.78125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve73" -p "pSphere1Follicle2877";
	rename -uid "012ECE18-40E6-A672-558C-34AB966B313A";
createNode nurbsCurve -n "curveShape73" -p "curve73";
	rename -uid "9BAADB99-49DE-5F97-B1D2-99873C110F4C";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "4884DC15-45FD-7CD8-643F-74BA14BA11A6";
createNode follicle -n "pSphere1FollicleShape2884" -p "pSphere1Follicle2884";
	rename -uid "1F2DF5B6-4885-89B9-03E4-58AE2765F7A7";
	setAttr -k off ".v";
	setAttr ".pu" 0.28125;
	setAttr ".pv" 0.84375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve74" -p "pSphere1Follicle2884";
	rename -uid "8F513820-424E-73D0-56A9-FCAE27E0A2CB";
createNode nurbsCurve -n "curveShape74" -p "curve74";
	rename -uid "8FFAA461-46FE-D14C-F9E7-B9841867B5BB";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "DF9C9A35-43C3-AB0F-578C-E7900095B049";
createNode follicle -n "pSphere1FollicleShape2890" -p "pSphere1Follicle2890";
	rename -uid "3CF324BB-4650-8290-6F30-E19A5C6976E7";
	setAttr -k off ".v";
	setAttr ".pu" 0.28125;
	setAttr ".pv" 0.90625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve75" -p "pSphere1Follicle2890";
	rename -uid "F267396E-4093-4C05-AFE1-C5BB0F42DA31";
createNode nurbsCurve -n "curveShape75" -p "curve75";
	rename -uid "3BE9634D-4CDF-13A5-E2F2-70BF65E45F7A";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "034D6C9C-4DCF-CA6C-8625-B3845D139201";
createNode follicle -n "pSphere1FollicleShape2896" -p "pSphere1Follicle2896";
	rename -uid "BA97D9DE-4497-F62D-70AF-C0B91AD2C9BD";
	setAttr -k off ".v";
	setAttr ".pu" 0.28125;
	setAttr ".pv" 0.96875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve76" -p "pSphere1Follicle2896";
	rename -uid "7EC9DFC7-4C2F-D096-DBD5-D98872C0305E";
createNode nurbsCurve -n "curveShape76" -p "curve76";
	rename -uid "3A01C8C7-4993-31D1-0588-BBBAAC066ACB";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "5A8FBC67-4002-0997-B4D0-6DB622E822EC";
createNode follicle -n "pSphere1FollicleShape3409" -p "pSphere1Follicle3409";
	rename -uid "0CE4201E-4ED7-515B-D631-ABA17AF8B12C";
	setAttr -k off ".v";
	setAttr ".pu" 0.34375;
	setAttr ".pv" 0.09375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve77" -p "pSphere1Follicle3409";
	rename -uid "84FB0335-4C4A-7D4F-1501-0DA01DC565D0";
createNode nurbsCurve -n "curveShape77" -p "curve77";
	rename -uid "69A2E6C6-4569-29ED-D7D8-E6B58B26031E";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "CADF7F67-4179-9AD1-64CE-4EB9957DCD62";
createNode follicle -n "pSphere1FollicleShape3415" -p "pSphere1Follicle3415";
	rename -uid "A5D71D3E-4136-6611-E815-DE82F7CBC44F";
	setAttr -k off ".v";
	setAttr ".pu" 0.34375;
	setAttr ".pv" 0.15625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve78" -p "pSphere1Follicle3415";
	rename -uid "067025F0-4F69-12FF-7B53-10B0B8C066DB";
createNode nurbsCurve -n "curveShape78" -p "curve78";
	rename -uid "D1B4FD54-41FC-B81E-DCC9-DA9430F345C4";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "70FFF915-4114-8D8A-1E6A-CD80F29C469B";
createNode follicle -n "pSphere1FollicleShape3422" -p "pSphere1Follicle3422";
	rename -uid "AC4A300B-4FE1-B85A-4C57-DEA5536DB813";
	setAttr -k off ".v";
	setAttr ".pu" 0.34375;
	setAttr ".pv" 0.21875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve79" -p "pSphere1Follicle3422";
	rename -uid "215DAD65-400B-B82F-B308-4697D388766F";
createNode nurbsCurve -n "curveShape79" -p "curve79";
	rename -uid "1D7DEE93-4ECA-9E86-DE84-27A1C8BA4F29";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "2EF2B9CB-41C6-1E6D-48DD-0C8628C300A2";
createNode follicle -n "pSphere1FollicleShape3428" -p "pSphere1Follicle3428";
	rename -uid "4097A016-45DB-CE27-A326-0B805B23EAE2";
	setAttr -k off ".v";
	setAttr ".pu" 0.34375;
	setAttr ".pv" 0.28125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve80" -p "pSphere1Follicle3428";
	rename -uid "0D240B3F-47B2-DA8C-20DB-9182B20FD14F";
createNode nurbsCurve -n "curveShape80" -p "curve80";
	rename -uid "A61EC5F1-44FB-9EA7-8CB9-449904C0D387";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "AF167C43-4326-F446-A23D-719911F984E6";
createNode follicle -n "pSphere1FollicleShape3434" -p "pSphere1Follicle3434";
	rename -uid "7537C63B-4C79-71E2-C67A-859F8F650A04";
	setAttr -k off ".v";
	setAttr ".pu" 0.34375;
	setAttr ".pv" 0.34375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve81" -p "pSphere1Follicle3434";
	rename -uid "058C38A0-4EFA-28A2-8F62-1EB79BE7AF3A";
createNode nurbsCurve -n "curveShape81" -p "curve81";
	rename -uid "81060B33-4FC3-E130-744A-2C8B7A49ACCD";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "7D332AED-42DE-3E91-D2EA-E4A2B3E8D7F4";
createNode follicle -n "pSphere1FollicleShape3440" -p "pSphere1Follicle3440";
	rename -uid "7A1E65B8-4CAF-ABCA-8FF2-E28843D4DD59";
	setAttr -k off ".v";
	setAttr ".pu" 0.34375;
	setAttr ".pv" 0.40625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve82" -p "pSphere1Follicle3440";
	rename -uid "48182486-4E73-3F36-9AD7-26A6BCC885B1";
createNode nurbsCurve -n "curveShape82" -p "curve82";
	rename -uid "F6DC787A-4F60-30B2-27C4-7EB6B6E4EDB2";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "62FBCB2F-4C79-A29D-EF6A-66AC1A7B4D3B";
createNode follicle -n "pSphere1FollicleShape3446" -p "pSphere1Follicle3446";
	rename -uid "DBCBD926-4CB8-79EA-609E-FB8E7DFC037B";
	setAttr -k off ".v";
	setAttr ".pu" 0.34375;
	setAttr ".pv" 0.46875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve83" -p "pSphere1Follicle3446";
	rename -uid "0C9BA394-4158-DEAB-EFC1-6699EA59D555";
createNode nurbsCurve -n "curveShape83" -p "curve83";
	rename -uid "1B0808D0-4F86-0647-F7C7-358C6F9C34EA";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "959857B2-4E28-FE8D-FBEC-EAB7AA285000";
createNode follicle -n "pSphere1FollicleShape3453" -p "pSphere1Follicle3453";
	rename -uid "C4E14B3B-4376-0E86-5A7A-5CB4BE2BD9AE";
	setAttr -k off ".v";
	setAttr ".pu" 0.34375;
	setAttr ".pv" 0.53125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve84" -p "pSphere1Follicle3453";
	rename -uid "B815A463-4A98-FA4C-4973-74A02326E11B";
createNode nurbsCurve -n "curveShape84" -p "curve84";
	rename -uid "3B108CE5-4678-8D05-C226-4D809441CC14";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "27679ED7-41DE-F611-8EB1-7BBCDDAD89F2";
createNode follicle -n "pSphere1FollicleShape3459" -p "pSphere1Follicle3459";
	rename -uid "50AC3CC2-4A14-B8A8-1DA1-6EB0F7922D5F";
	setAttr -k off ".v";
	setAttr ".pu" 0.34375;
	setAttr ".pv" 0.59375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve85" -p "pSphere1Follicle3459";
	rename -uid "8A55FDD6-449A-9DBC-1520-2FAB8DD48DC6";
createNode nurbsCurve -n "curveShape85" -p "curve85";
	rename -uid "1E32D833-4A43-1149-891B-2988D292B310";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "3526E1CE-4D73-DEDA-174F-E0B943FAAADE";
createNode follicle -n "pSphere1FollicleShape3465" -p "pSphere1Follicle3465";
	rename -uid "D3DB13BC-4ECB-716E-0B18-AB89D0815782";
	setAttr -k off ".v";
	setAttr ".pu" 0.34375;
	setAttr ".pv" 0.65625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve86" -p "pSphere1Follicle3465";
	rename -uid "D823AB0F-46AB-E6EA-F323-448834030AC5";
createNode nurbsCurve -n "curveShape86" -p "curve86";
	rename -uid "C813FE54-4D5E-ED77-7816-7AA5BA3BD8A9";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "ECEA9AB9-4DBD-1112-5742-4DA48648EC52";
createNode follicle -n "pSphere1FollicleShape3471" -p "pSphere1Follicle3471";
	rename -uid "E32B4C81-4071-A61B-297E-1CAF832E3D11";
	setAttr -k off ".v";
	setAttr ".pu" 0.34375;
	setAttr ".pv" 0.71875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve87" -p "pSphere1Follicle3471";
	rename -uid "EFAC0E12-4F02-C9CF-D5CB-8E82CB2C87A4";
createNode nurbsCurve -n "curveShape87" -p "curve87";
	rename -uid "6662D188-4ADB-A98F-DD02-F4B9B7CCACED";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "C94CFD65-407C-4785-97B9-509029FE970D";
createNode follicle -n "pSphere1FollicleShape3477" -p "pSphere1Follicle3477";
	rename -uid "33A17EC9-406E-CA5D-DD54-C5A286478B3A";
	setAttr -k off ".v";
	setAttr ".pu" 0.34375;
	setAttr ".pv" 0.78125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve88" -p "pSphere1Follicle3477";
	rename -uid "79178CBD-4D75-6B5D-BF92-D2A3BC14F540";
createNode nurbsCurve -n "curveShape88" -p "curve88";
	rename -uid "C37BBB67-4A51-A24F-F52E-7896F0C7FB22";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "B7C7F621-4CE9-8DCE-6C57-CEAD067AAEE3";
createNode follicle -n "pSphere1FollicleShape3484" -p "pSphere1Follicle3484";
	rename -uid "D1D5553A-4477-0E44-6F2C-8FB6D0F089C3";
	setAttr -k off ".v";
	setAttr ".pu" 0.34375;
	setAttr ".pv" 0.84375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve89" -p "pSphere1Follicle3484";
	rename -uid "072BF08E-4143-3468-12F4-E392B13524A3";
createNode nurbsCurve -n "curveShape89" -p "curve89";
	rename -uid "D4C7B982-4B6D-738C-B238-D6B67668117F";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "ADC2B287-447B-8D65-B424-F0896BF8B6F2";
createNode follicle -n "pSphere1FollicleShape3490" -p "pSphere1Follicle3490";
	rename -uid "6FFBC800-46BD-5CF9-7962-B6949044398D";
	setAttr -k off ".v";
	setAttr ".pu" 0.34375;
	setAttr ".pv" 0.90625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve90" -p "pSphere1Follicle3490";
	rename -uid "57E4FE57-441F-AB88-00AF-24849E1CD68A";
createNode nurbsCurve -n "curveShape90" -p "curve90";
	rename -uid "0286BD44-47CE-8094-20F4-A6A33B0DEC18";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "85ABE3B9-4829-CF73-5079-9DB71F30DCA6";
createNode follicle -n "pSphere1FollicleShape4009" -p "pSphere1Follicle4009";
	rename -uid "69E4C2BE-4331-0CD0-CCC5-9D8D76425AE2";
	setAttr -k off ".v";
	setAttr ".pu" 0.40625;
	setAttr ".pv" 0.09375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve91" -p "pSphere1Follicle4009";
	rename -uid "6E2AF263-42B5-6D4A-16EF-268139A7B3BB";
createNode nurbsCurve -n "curveShape91" -p "curve91";
	rename -uid "363F09FA-4A37-177A-3B36-4A9EEE301B01";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "2A8CE6FF-4FCE-021A-B8CB-DFB19473A0A3";
createNode follicle -n "pSphere1FollicleShape4015" -p "pSphere1Follicle4015";
	rename -uid "7556341A-444F-5469-E6FA-5F9E1276F1AB";
	setAttr -k off ".v";
	setAttr ".pu" 0.40625;
	setAttr ".pv" 0.15625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve92" -p "pSphere1Follicle4015";
	rename -uid "3517A242-45EE-262F-6753-279545AB03AC";
createNode nurbsCurve -n "curveShape92" -p "curve92";
	rename -uid "22705CBC-4B88-FD38-B2E1-19A7A2863B75";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "65A9CCF0-428E-E92A-A214-BFBDE70BCE03";
createNode follicle -n "pSphere1FollicleShape4022" -p "pSphere1Follicle4022";
	rename -uid "64A95E0F-4551-C91B-4440-539ECCDA189E";
	setAttr -k off ".v";
	setAttr ".pu" 0.40625;
	setAttr ".pv" 0.21875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve93" -p "pSphere1Follicle4022";
	rename -uid "A1EA2B1A-4195-087E-B341-0B9FB8B0BD7A";
createNode nurbsCurve -n "curveShape93" -p "curve93";
	rename -uid "59E13FB7-41C5-22CA-F1C3-2CA91BFDC933";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "993E20A5-4F32-831B-F4EF-D7A09D568576";
createNode follicle -n "pSphere1FollicleShape4028" -p "pSphere1Follicle4028";
	rename -uid "88E2B12C-483D-D2CF-1EC3-2C99228EDCB8";
	setAttr -k off ".v";
	setAttr ".pu" 0.40625;
	setAttr ".pv" 0.28125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve94" -p "pSphere1Follicle4028";
	rename -uid "A192D58B-40CC-030A-5438-BAB95A03D9DA";
createNode nurbsCurve -n "curveShape94" -p "curve94";
	rename -uid "2B9E2A58-445D-9769-1705-2C8CD87F7AF0";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "02284111-4AF0-7A74-3391-B08BCDF56A51";
createNode follicle -n "pSphere1FollicleShape4034" -p "pSphere1Follicle4034";
	rename -uid "82BF1095-4C91-BF07-CE94-E5A1689F8191";
	setAttr -k off ".v";
	setAttr ".pu" 0.40625;
	setAttr ".pv" 0.34375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve95" -p "pSphere1Follicle4034";
	rename -uid "4A776323-4627-6937-FCE1-AE86E33F6911";
createNode nurbsCurve -n "curveShape95" -p "curve95";
	rename -uid "FD13EA6D-44EC-9BE8-6180-2A93105D6F20";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "3ECF2F7A-4E78-F3E2-9F25-C8A7A27A33C4";
createNode follicle -n "pSphere1FollicleShape4040" -p "pSphere1Follicle4040";
	rename -uid "E6361F6F-4F86-08CC-9EF2-B8B7BA734488";
	setAttr -k off ".v";
	setAttr ".pu" 0.40625;
	setAttr ".pv" 0.40625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve96" -p "pSphere1Follicle4040";
	rename -uid "26AACFE6-48EF-1947-1FAD-AEA50019B76B";
createNode nurbsCurve -n "curveShape96" -p "curve96";
	rename -uid "46E23AD9-4938-A3DF-2B16-43B42A914668";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "0C8F6543-439F-39F0-B5A9-FCA318B7B527";
createNode follicle -n "pSphere1FollicleShape4046" -p "pSphere1Follicle4046";
	rename -uid "B2802BE6-4B15-7405-745A-4986F0610F83";
	setAttr -k off ".v";
	setAttr ".pu" 0.40625;
	setAttr ".pv" 0.46875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve97" -p "pSphere1Follicle4046";
	rename -uid "F003DC2C-4C8D-567E-9721-7996A0298CB6";
createNode nurbsCurve -n "curveShape97" -p "curve97";
	rename -uid "0231D164-49AB-AA78-12B8-85B4AF81A29A";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "43169D60-467C-C576-B0FB-C2947B60322E";
createNode follicle -n "pSphere1FollicleShape4053" -p "pSphere1Follicle4053";
	rename -uid "2FD02B95-4F87-5A00-78D6-22BC3B0D4F63";
	setAttr -k off ".v";
	setAttr ".pu" 0.40625;
	setAttr ".pv" 0.53125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve98" -p "pSphere1Follicle4053";
	rename -uid "7F29A27D-4DEB-F6CF-DE2D-378C5B39DE75";
createNode nurbsCurve -n "curveShape98" -p "curve98";
	rename -uid "18AD5853-4DFF-EAB0-63AC-D4BAC4101039";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "BB4F6D2C-4198-4BA6-ED5D-4BBEC331A498";
createNode follicle -n "pSphere1FollicleShape4059" -p "pSphere1Follicle4059";
	rename -uid "EDA8866E-45DB-641B-A368-5487DFA148FA";
	setAttr -k off ".v";
	setAttr ".pu" 0.40625;
	setAttr ".pv" 0.59375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve99" -p "pSphere1Follicle4059";
	rename -uid "B6D0CAA4-4775-7ECB-A6F5-078B72DD8EB4";
createNode nurbsCurve -n "curveShape99" -p "curve99";
	rename -uid "83798287-4DDB-C3A1-1DE6-FF95E2989FEC";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "03DDF359-4650-6769-961C-7C8C14EFB6F6";
createNode follicle -n "pSphere1FollicleShape4065" -p "pSphere1Follicle4065";
	rename -uid "20A2F363-462E-F2E1-926F-018F17FEEC71";
	setAttr -k off ".v";
	setAttr ".pu" 0.40625;
	setAttr ".pv" 0.65625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve100" -p "pSphere1Follicle4065";
	rename -uid "E8D34262-4F42-0FD3-2B4B-3C8DD8583F7B";
createNode nurbsCurve -n "curveShape100" -p "curve100";
	rename -uid "B03A4D7B-43AD-8D13-99B2-70B4CD1D8583";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "6A47A182-4D81-12C5-6F3E-A3892CE58B28";
createNode follicle -n "pSphere1FollicleShape4071" -p "pSphere1Follicle4071";
	rename -uid "88818ED6-4460-265B-89EA-57B2F11BFF92";
	setAttr -k off ".v";
	setAttr ".pu" 0.40625;
	setAttr ".pv" 0.71875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve101" -p "pSphere1Follicle4071";
	rename -uid "46FD0485-4AC2-E444-CA5F-54BD38BE709D";
createNode nurbsCurve -n "curveShape101" -p "curve101";
	rename -uid "D09E7801-4933-B6B8-7F51-87AE29C9F3DA";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "90957797-484B-13AA-75FF-B7A0166FA131";
createNode follicle -n "pSphere1FollicleShape4077" -p "pSphere1Follicle4077";
	rename -uid "219E7EE4-45A2-BA96-1C9E-37B45FFA8F1F";
	setAttr -k off ".v";
	setAttr ".pu" 0.40625;
	setAttr ".pv" 0.78125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve102" -p "pSphere1Follicle4077";
	rename -uid "FDD6F96D-41A7-112D-A22A-6695BDC3CB0D";
createNode nurbsCurve -n "curveShape102" -p "curve102";
	rename -uid "A8A5ECCE-4A9C-0337-5345-DBA746D77704";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "1A928C18-4B74-2C4F-B7E7-DBB86CF2BE66";
createNode follicle -n "pSphere1FollicleShape4084" -p "pSphere1Follicle4084";
	rename -uid "43DE6F42-44C1-7A7D-5F76-F9BE0D91C057";
	setAttr -k off ".v";
	setAttr ".pu" 0.40625;
	setAttr ".pv" 0.84375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve103" -p "pSphere1Follicle4084";
	rename -uid "1370072B-45C3-6775-0379-2797A0869C7D";
createNode nurbsCurve -n "curveShape103" -p "curve103";
	rename -uid "8CFC4A5B-4FFC-7B50-EB3E-24A881D88509";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "E6D1886F-4238-ED3D-24A5-F99340151967";
createNode follicle -n "pSphere1FollicleShape4090" -p "pSphere1Follicle4090";
	rename -uid "79DCFEDF-42A2-E5C4-B29C-85837EC06AF9";
	setAttr -k off ".v";
	setAttr ".pu" 0.40625;
	setAttr ".pv" 0.90625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve104" -p "pSphere1Follicle4090";
	rename -uid "D6F28F4E-4EE7-975A-2B48-9281EE8A897A";
createNode nurbsCurve -n "curveShape104" -p "curve104";
	rename -uid "EC208C16-4666-6973-8837-D6AB499C53CC";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "28110BD9-49AF-C5E2-D516-87A0B050C356";
createNode follicle -n "pSphere1FollicleShape4603" -p "pSphere1Follicle4603";
	rename -uid "5DB003A0-420D-B7E3-8CBD-47A647531080";
	setAttr -k off ".v";
	setAttr ".pu" 0.46875;
	setAttr ".pv" 0.03125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve105" -p "pSphere1Follicle4603";
	rename -uid "B028883B-45F0-EE3C-7E6C-579E233B47C7";
createNode nurbsCurve -n "curveShape105" -p "curve105";
	rename -uid "525BF36B-453D-8DD0-F046-B3B7D6C1B4FD";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "B10D257A-4938-D2EB-D95D-64B6826B0D10";
createNode follicle -n "pSphere1FollicleShape4609" -p "pSphere1Follicle4609";
	rename -uid "38D8FAC2-4BC0-2CAA-49CA-92912333BECE";
	setAttr -k off ".v";
	setAttr ".pu" 0.46875;
	setAttr ".pv" 0.09375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve106" -p "pSphere1Follicle4609";
	rename -uid "457FB10D-4461-153F-F10E-22BFAE90D86D";
createNode nurbsCurve -n "curveShape106" -p "curve106";
	rename -uid "BC687D54-48DA-AACD-3595-9FB048A8F773";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "99FADA08-43B7-2A2C-2AC2-AFA3D7783463";
createNode follicle -n "pSphere1FollicleShape4615" -p "pSphere1Follicle4615";
	rename -uid "15ABEA41-46B7-65F0-D40D-BAACC7D3FDC4";
	setAttr -k off ".v";
	setAttr ".pu" 0.46875;
	setAttr ".pv" 0.15625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve107" -p "pSphere1Follicle4615";
	rename -uid "4E2876EE-4686-097B-9BCC-E38DFCB35EA3";
createNode nurbsCurve -n "curveShape107" -p "curve107";
	rename -uid "98D8AB9C-416B-F4F7-03A7-CE9643014966";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "DE976A6E-489D-CFE2-9EE7-D490CDB1E294";
createNode follicle -n "pSphere1FollicleShape4622" -p "pSphere1Follicle4622";
	rename -uid "8D171964-4005-0B22-AC0D-EAAF31B6A931";
	setAttr -k off ".v";
	setAttr ".pu" 0.46875;
	setAttr ".pv" 0.21875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve108" -p "pSphere1Follicle4622";
	rename -uid "DF715E8F-4EE2-2EC8-D097-7597DEEFF57F";
createNode nurbsCurve -n "curveShape108" -p "curve108";
	rename -uid "D88658DD-4801-1E0B-6A87-9CA699D9E5E2";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "8E7EF5C2-4477-6270-A599-7A9B2528F767";
createNode follicle -n "pSphere1FollicleShape4628" -p "pSphere1Follicle4628";
	rename -uid "81D0E032-4798-5D85-76D8-FA9463F4FD4B";
	setAttr -k off ".v";
	setAttr ".pu" 0.46875;
	setAttr ".pv" 0.28125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve109" -p "pSphere1Follicle4628";
	rename -uid "BA667A71-4677-4665-1CAE-BB9C854920BD";
createNode nurbsCurve -n "curveShape109" -p "curve109";
	rename -uid "03A16D1B-4558-0AF4-8499-DDA5D3262013";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "CB24F6AE-4A04-8822-E336-CB8147329CAD";
createNode follicle -n "pSphere1FollicleShape4634" -p "pSphere1Follicle4634";
	rename -uid "F2D9BA36-4803-A3E9-9F47-A39136FCEBE6";
	setAttr -k off ".v";
	setAttr ".pu" 0.46875;
	setAttr ".pv" 0.34375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve110" -p "pSphere1Follicle4634";
	rename -uid "AF44FE19-456B-2EE9-89FA-D3B404FBEB8D";
createNode nurbsCurve -n "curveShape110" -p "curve110";
	rename -uid "74748AC0-46E0-9AAC-12D1-B8A1247F7BF5";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "0718D16C-4687-C823-C448-D6B140D56B97";
createNode follicle -n "pSphere1FollicleShape4640" -p "pSphere1Follicle4640";
	rename -uid "561ACF77-42F5-65D2-7469-C28C47DACB17";
	setAttr -k off ".v";
	setAttr ".pu" 0.46875;
	setAttr ".pv" 0.40625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve111" -p "pSphere1Follicle4640";
	rename -uid "43ECFF7A-42CF-9D7D-4855-6EAAF082079F";
createNode nurbsCurve -n "curveShape111" -p "curve111";
	rename -uid "5DB070CE-4F83-9CBA-F715-7798F45024F9";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "85F66EF3-4967-56B7-463B-629368180F61";
createNode follicle -n "pSphere1FollicleShape4646" -p "pSphere1Follicle4646";
	rename -uid "1E6812C6-4C3F-70AD-6303-D6A4EE59FF7D";
	setAttr -k off ".v";
	setAttr ".pu" 0.46875;
	setAttr ".pv" 0.46875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve112" -p "pSphere1Follicle4646";
	rename -uid "658507B1-464B-4F57-6C2D-469F03E79446";
createNode nurbsCurve -n "curveShape112" -p "curve112";
	rename -uid "FB36ADA0-4E84-8719-E112-BF8BC9F74281";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "00BCF1F1-4A73-60F1-6D93-64874E44CB4E";
createNode follicle -n "pSphere1FollicleShape4653" -p "pSphere1Follicle4653";
	rename -uid "9AA2627A-4A8E-E548-53F8-28BDCF305ED1";
	setAttr -k off ".v";
	setAttr ".pu" 0.46875;
	setAttr ".pv" 0.53125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve113" -p "pSphere1Follicle4653";
	rename -uid "8E7FB630-4E90-D430-EF04-52AA51731ADC";
createNode nurbsCurve -n "curveShape113" -p "curve113";
	rename -uid "FF650977-424A-8D08-6858-55B5D0153AC4";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "AC414F4E-449B-51B5-F99E-5F9430A05576";
createNode follicle -n "pSphere1FollicleShape4659" -p "pSphere1Follicle4659";
	rename -uid "AAD13A87-48CB-AD84-A075-59887DBC9A4B";
	setAttr -k off ".v";
	setAttr ".pu" 0.46875;
	setAttr ".pv" 0.59375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve114" -p "pSphere1Follicle4659";
	rename -uid "D6411918-429B-73EE-BAB8-B9A05A84B78B";
createNode nurbsCurve -n "curveShape114" -p "curve114";
	rename -uid "586089EA-4D11-A780-AAFD-A9885610E286";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "0851FE6D-40FD-24D0-FD4F-1DB32B8EEE9D";
createNode follicle -n "pSphere1FollicleShape4665" -p "pSphere1Follicle4665";
	rename -uid "22AE7A1E-499F-D522-32FD-AA808196B57E";
	setAttr -k off ".v";
	setAttr ".pu" 0.46875;
	setAttr ".pv" 0.65625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve115" -p "pSphere1Follicle4665";
	rename -uid "02BE2AAE-4879-4E9C-F228-858E83ED046E";
createNode nurbsCurve -n "curveShape115" -p "curve115";
	rename -uid "38A42D5A-4902-5346-D9A3-99920D69FEA7";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "1EB836A9-4093-AB1B-2A11-DCBAFBC2AB41";
createNode follicle -n "pSphere1FollicleShape4671" -p "pSphere1Follicle4671";
	rename -uid "4B959E8D-4D4B-B436-12AE-CCA7D6EB2850";
	setAttr -k off ".v";
	setAttr ".pu" 0.46875;
	setAttr ".pv" 0.71875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve116" -p "pSphere1Follicle4671";
	rename -uid "9AB25251-42FB-A911-79E2-5CA55D6B14F7";
createNode nurbsCurve -n "curveShape116" -p "curve116";
	rename -uid "B7607EC2-4DC5-B55E-A7C8-1288DBA7CC4F";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "8F80D52B-498F-B025-CF1D-759265D45D52";
createNode follicle -n "pSphere1FollicleShape4677" -p "pSphere1Follicle4677";
	rename -uid "D510C475-4BFE-6FA1-D0A4-69ABED198BF7";
	setAttr -k off ".v";
	setAttr ".pu" 0.46875;
	setAttr ".pv" 0.78125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve117" -p "pSphere1Follicle4677";
	rename -uid "F8F9C050-41C2-D2E7-C17B-E48D63DD7F23";
createNode nurbsCurve -n "curveShape117" -p "curve117";
	rename -uid "EF27E65B-4238-1E32-361C-2C81ACC92585";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "9986C837-45F9-80AF-C8A2-5791A38AB6B1";
createNode follicle -n "pSphere1FollicleShape4684" -p "pSphere1Follicle4684";
	rename -uid "0345B555-441A-255E-7ED4-139ECE7CDEAD";
	setAttr -k off ".v";
	setAttr ".pu" 0.46875;
	setAttr ".pv" 0.84375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve118" -p "pSphere1Follicle4684";
	rename -uid "C1244023-4EB3-886B-BE4D-B28B23964F8B";
createNode nurbsCurve -n "curveShape118" -p "curve118";
	rename -uid "434CCB15-4199-761A-CA57-10B6FDAB9D27";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "6434C67B-4CB9-71D7-EA3D-239853469114";
createNode follicle -n "pSphere1FollicleShape4690" -p "pSphere1Follicle4690";
	rename -uid "12D2B996-4737-6EAA-743E-B3B2F1ABE539";
	setAttr -k off ".v";
	setAttr ".pu" 0.46875;
	setAttr ".pv" 0.90625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve119" -p "pSphere1Follicle4690";
	rename -uid "493A64A6-4127-7DDF-FB50-0297C73F8425";
createNode nurbsCurve -n "curveShape119" -p "curve119";
	rename -uid "B8876FEC-46D4-64F2-2152-B5BD4837FA24";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "D37A72B1-4376-30D5-B1BF-D99AA0F44C41";
createNode follicle -n "pSphere1FollicleShape4696" -p "pSphere1Follicle4696";
	rename -uid "32E851FA-4D25-CBC9-038B-A982E6B5EC2E";
	setAttr -k off ".v";
	setAttr ".pu" 0.46875;
	setAttr ".pv" 0.96875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve120" -p "pSphere1Follicle4696";
	rename -uid "DD2E5E13-4D9C-8B10-CA2C-F1B77DFCC32E";
createNode nurbsCurve -n "curveShape120" -p "curve120";
	rename -uid "96DF156B-412E-2C53-55AB-A0BE694E38EB";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "D94F307C-4AA9-C41D-4AAA-A5B0D7E3A999";
createNode follicle -n "pSphere1FollicleShape5303" -p "pSphere1Follicle5303";
	rename -uid "C068529B-4B23-1802-C107-D0B369770555";
	setAttr -k off ".v";
	setAttr ".pu" 0.53125;
	setAttr ".pv" 0.03125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve121" -p "pSphere1Follicle5303";
	rename -uid "AF5F3C0D-4395-BE1A-332A-A4A81C4ABE67";
createNode nurbsCurve -n "curveShape121" -p "curve121";
	rename -uid "AA6CB575-4945-5BA9-4073-749E90DC83F4";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "31858469-4EEA-63B6-A560-8A8A0BD73F9F";
createNode follicle -n "pSphere1FollicleShape5309" -p "pSphere1Follicle5309";
	rename -uid "D4CF075B-4884-E172-538C-6B9316C95B4F";
	setAttr -k off ".v";
	setAttr ".pu" 0.53125;
	setAttr ".pv" 0.09375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve122" -p "pSphere1Follicle5309";
	rename -uid "DC4C2874-4D9C-15DE-455E-289E46CD976E";
createNode nurbsCurve -n "curveShape122" -p "curve122";
	rename -uid "C430C81E-45BE-8349-59DE-99B850B0E02B";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "589EF921-4F63-B8F7-2852-E9BBED1D274D";
createNode follicle -n "pSphere1FollicleShape5315" -p "pSphere1Follicle5315";
	rename -uid "3962162D-418A-D89F-861B-7687AC400F75";
	setAttr -k off ".v";
	setAttr ".pu" 0.53125;
	setAttr ".pv" 0.15625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve123" -p "pSphere1Follicle5315";
	rename -uid "5F48ACE6-4DDE-11B3-A50E-2A98AB8CC9E9";
createNode nurbsCurve -n "curveShape123" -p "curve123";
	rename -uid "4370F237-4A38-747B-8245-D6AADD5D6369";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "6C46039B-4037-B9B0-2B43-DEA3B508D357";
createNode follicle -n "pSphere1FollicleShape5322" -p "pSphere1Follicle5322";
	rename -uid "18A0A2B8-44DD-4BC3-13F6-57B8E13245EC";
	setAttr -k off ".v";
	setAttr ".pu" 0.53125;
	setAttr ".pv" 0.21875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve124" -p "pSphere1Follicle5322";
	rename -uid "8981B487-4BCF-4F7B-30BC-A2A0D7F8E092";
createNode nurbsCurve -n "curveShape124" -p "curve124";
	rename -uid "06F2212C-43E3-2505-6E92-C4947BDE24A4";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "C47C0B72-4C58-8267-8D87-A8A7882B4F9C";
createNode follicle -n "pSphere1FollicleShape5328" -p "pSphere1Follicle5328";
	rename -uid "A249FB84-4BE0-F312-C5F8-CFAFAB17AC5A";
	setAttr -k off ".v";
	setAttr ".pu" 0.53125;
	setAttr ".pv" 0.28125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve125" -p "pSphere1Follicle5328";
	rename -uid "F1F022AD-4EB6-C8C2-B1E4-95A2D19B3A1C";
createNode nurbsCurve -n "curveShape125" -p "curve125";
	rename -uid "7B303B8A-40CA-CCC5-6895-89A8E8695730";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "97B13107-439D-4661-14AE-B0B124AA8F44";
createNode follicle -n "pSphere1FollicleShape5334" -p "pSphere1Follicle5334";
	rename -uid "69A60338-4688-70D6-267F-3EAF0773C6A6";
	setAttr -k off ".v";
	setAttr ".pu" 0.53125;
	setAttr ".pv" 0.34375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve126" -p "pSphere1Follicle5334";
	rename -uid "D9F4F9C8-4E61-C9A8-6F6E-D58BDA0B61F9";
createNode nurbsCurve -n "curveShape126" -p "curve126";
	rename -uid "E096F429-4FF1-40DC-3763-E08B4B5E1445";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "0B53A0D6-4E1F-2E2E-D971-4CA9692B3CC3";
createNode follicle -n "pSphere1FollicleShape5340" -p "pSphere1Follicle5340";
	rename -uid "8022D77F-4AF1-04B9-C46F-568160870520";
	setAttr -k off ".v";
	setAttr ".pu" 0.53125;
	setAttr ".pv" 0.40625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve127" -p "pSphere1Follicle5340";
	rename -uid "E727D5A7-4386-A83C-3C71-2FAF240D113D";
createNode nurbsCurve -n "curveShape127" -p "curve127";
	rename -uid "B8079C94-4DD2-D05A-4D61-D79ECCC19544";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "4F82AE65-4F51-14F2-C8BC-FB8429185B2F";
createNode follicle -n "pSphere1FollicleShape5346" -p "pSphere1Follicle5346";
	rename -uid "8EF4408D-4996-A78D-ADF9-6EA55828ED7E";
	setAttr -k off ".v";
	setAttr ".pu" 0.53125;
	setAttr ".pv" 0.46875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve128" -p "pSphere1Follicle5346";
	rename -uid "0DB14FDE-46E0-76AE-FDB0-36B1A71B4496";
createNode nurbsCurve -n "curveShape128" -p "curve128";
	rename -uid "67294098-4025-B988-8C52-3EBA04402D7E";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "C0DEABE4-4891-11F9-FDD8-7DB1CBC8A544";
createNode follicle -n "pSphere1FollicleShape5353" -p "pSphere1Follicle5353";
	rename -uid "0B0E1FE4-443E-C6F1-2BD5-F8A3A327E896";
	setAttr -k off ".v";
	setAttr ".pu" 0.53125;
	setAttr ".pv" 0.53125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve129" -p "pSphere1Follicle5353";
	rename -uid "40E9520A-417D-37B8-8DD3-6DB28A958BA4";
createNode nurbsCurve -n "curveShape129" -p "curve129";
	rename -uid "C275EB9D-46BB-157F-85D4-5A9B96035971";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "9F7B9C67-4652-4CC3-7469-D2B988A8FDFC";
createNode follicle -n "pSphere1FollicleShape5359" -p "pSphere1Follicle5359";
	rename -uid "37CB5981-4889-655E-FE66-479918E97E0D";
	setAttr -k off ".v";
	setAttr ".pu" 0.53125;
	setAttr ".pv" 0.59375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve130" -p "pSphere1Follicle5359";
	rename -uid "246F4F73-496E-CFFF-227B-1991586CA4EF";
createNode nurbsCurve -n "curveShape130" -p "curve130";
	rename -uid "AED5F92C-4848-FA50-3A0A-18B1F05FA428";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "70FA5A09-477D-10B7-0A16-87B2BD173FF3";
createNode follicle -n "pSphere1FollicleShape5365" -p "pSphere1Follicle5365";
	rename -uid "D8F150F5-4C9E-1902-7E12-7EA2C09B8984";
	setAttr -k off ".v";
	setAttr ".pu" 0.53125;
	setAttr ".pv" 0.65625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve131" -p "pSphere1Follicle5365";
	rename -uid "A2562389-42CD-4ED0-EAD8-B583896A85E0";
createNode nurbsCurve -n "curveShape131" -p "curve131";
	rename -uid "B8C452BB-437E-B2CF-F164-29961C9370B1";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "F2C3A4F5-41A7-C689-5F09-0D932DDBD1C6";
createNode follicle -n "pSphere1FollicleShape5371" -p "pSphere1Follicle5371";
	rename -uid "66763A36-4C42-FC67-98AE-19A944E88CA9";
	setAttr -k off ".v";
	setAttr ".pu" 0.53125;
	setAttr ".pv" 0.71875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve132" -p "pSphere1Follicle5371";
	rename -uid "DBF00D4F-4A30-F9D9-ABDB-7885CA7AB0BE";
createNode nurbsCurve -n "curveShape132" -p "curve132";
	rename -uid "FAEC50D8-450C-90C8-2C2B-27AB0E5BC5AE";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "7D997423-4B85-367B-EC03-55878D62DD34";
createNode follicle -n "pSphere1FollicleShape5377" -p "pSphere1Follicle5377";
	rename -uid "E3A12F00-400C-CA8F-1D35-90A8E35D8002";
	setAttr -k off ".v";
	setAttr ".pu" 0.53125;
	setAttr ".pv" 0.78125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve133" -p "pSphere1Follicle5377";
	rename -uid "A7D8A5E0-4BF0-E384-D685-E4B62B0E68D3";
createNode nurbsCurve -n "curveShape133" -p "curve133";
	rename -uid "74B62F6B-4128-37F4-4E38-9D897A89F89E";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "0BE0F8CF-4353-B49C-3300-32BF28045C3D";
createNode follicle -n "pSphere1FollicleShape5384" -p "pSphere1Follicle5384";
	rename -uid "46CDBA72-4ACE-956D-8305-62A105259E51";
	setAttr -k off ".v";
	setAttr ".pu" 0.53125;
	setAttr ".pv" 0.84375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve134" -p "pSphere1Follicle5384";
	rename -uid "EC05C53E-42B4-5A42-9E38-29A6B71C8061";
createNode nurbsCurve -n "curveShape134" -p "curve134";
	rename -uid "EA2200B4-4070-8276-4AB8-6CB6F8DEEB64";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "35C902B8-4564-5630-7CD6-29BD75A2751A";
createNode follicle -n "pSphere1FollicleShape5390" -p "pSphere1Follicle5390";
	rename -uid "3F026D9C-48B9-8AA5-0D02-61A93B517199";
	setAttr -k off ".v";
	setAttr ".pu" 0.53125;
	setAttr ".pv" 0.90625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve135" -p "pSphere1Follicle5390";
	rename -uid "397F2AA0-4AA3-5CE4-2714-B2946B27DA6D";
createNode nurbsCurve -n "curveShape135" -p "curve135";
	rename -uid "8BA0D850-438E-0021-4E24-57975A316487";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "CD01EC6E-406B-8BBD-8379-15B3215FC886";
createNode follicle -n "pSphere1FollicleShape5396" -p "pSphere1Follicle5396";
	rename -uid "E0BDB42A-4B4C-EA4F-C7E4-50934CF7C50D";
	setAttr -k off ".v";
	setAttr ".pu" 0.53125;
	setAttr ".pv" 0.96875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve136" -p "pSphere1Follicle5396";
	rename -uid "BC94289A-41A0-C164-9144-3A97FCFE70EE";
createNode nurbsCurve -n "curveShape136" -p "curve136";
	rename -uid "8C972299-4790-3588-930B-8C92E692F8E9";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "266794F2-42C2-B00A-842E-8CA9CD398162";
createNode follicle -n "pSphere1FollicleShape5909" -p "pSphere1Follicle5909";
	rename -uid "C4F441BA-4C20-7A1A-2282-869A74A3E895";
	setAttr -k off ".v";
	setAttr ".pu" 0.59375;
	setAttr ".pv" 0.09375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve137" -p "pSphere1Follicle5909";
	rename -uid "EB5064DA-4213-35BD-1CED-2C8AE61CBF29";
createNode nurbsCurve -n "curveShape137" -p "curve137";
	rename -uid "61D870DB-44C4-A01D-0F9A-559617B650CA";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "368DB31B-4267-661C-A452-B0AC2E1B2BDA";
createNode follicle -n "pSphere1FollicleShape5915" -p "pSphere1Follicle5915";
	rename -uid "B4171407-4196-A97E-48A8-E3BDE175E065";
	setAttr -k off ".v";
	setAttr ".pu" 0.59375;
	setAttr ".pv" 0.15625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve138" -p "pSphere1Follicle5915";
	rename -uid "13369D98-4DDD-2A18-EA94-D1BC2F53052C";
createNode nurbsCurve -n "curveShape138" -p "curve138";
	rename -uid "86053860-473E-D508-7933-3EAFAFA2F75D";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "5353128F-44D5-C262-E78A-A1B9B5EED0A5";
createNode follicle -n "pSphere1FollicleShape5922" -p "pSphere1Follicle5922";
	rename -uid "349A9BC6-4E61-D6B6-6C89-D2A86F9E1AE4";
	setAttr -k off ".v";
	setAttr ".pu" 0.59375;
	setAttr ".pv" 0.21875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve139" -p "pSphere1Follicle5922";
	rename -uid "8666093B-4F80-7623-3C91-7AA9AAB382A3";
createNode nurbsCurve -n "curveShape139" -p "curve139";
	rename -uid "898A1C28-418C-A963-ADA3-DCAD658628A2";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "F95CB21A-4068-C0B5-ECE7-038DB57D61A3";
createNode follicle -n "pSphere1FollicleShape5928" -p "pSphere1Follicle5928";
	rename -uid "58CB5653-4486-18BA-1E30-528BDD169CA8";
	setAttr -k off ".v";
	setAttr ".pu" 0.59375;
	setAttr ".pv" 0.28125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve140" -p "pSphere1Follicle5928";
	rename -uid "3527838B-4BF7-FA97-E488-4BA9F2F46F69";
createNode nurbsCurve -n "curveShape140" -p "curve140";
	rename -uid "0C40C5FC-47C4-FC38-DD36-3D8C806519F0";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "D2C38045-44B9-799B-0CBE-6FB0EB09085C";
createNode follicle -n "pSphere1FollicleShape5934" -p "pSphere1Follicle5934";
	rename -uid "F8BCADD5-453E-D060-6CD1-EEA1F339B4D2";
	setAttr -k off ".v";
	setAttr ".pu" 0.59375;
	setAttr ".pv" 0.34375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve141" -p "pSphere1Follicle5934";
	rename -uid "A140BA38-49B3-587D-A4B1-0AA184612965";
createNode nurbsCurve -n "curveShape141" -p "curve141";
	rename -uid "64E8A7CD-4E0E-F6C8-E13C-C4B6F291D91F";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "BB1FF52D-4BEC-D29C-7BC7-CB92E99FF10F";
createNode follicle -n "pSphere1FollicleShape5940" -p "pSphere1Follicle5940";
	rename -uid "CB3B6DC0-4B9B-AFD6-6EBC-508E016CF4AA";
	setAttr -k off ".v";
	setAttr ".pu" 0.59375;
	setAttr ".pv" 0.40625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve142" -p "pSphere1Follicle5940";
	rename -uid "57D7461C-4211-A2EE-10FC-92819623A5F5";
createNode nurbsCurve -n "curveShape142" -p "curve142";
	rename -uid "6D024FAF-48BD-9D4C-4B58-E5AE581B0DFA";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "36ED4CCA-4242-6A9B-0820-899AF3A3EBA8";
createNode follicle -n "pSphere1FollicleShape5946" -p "pSphere1Follicle5946";
	rename -uid "619CD480-4414-1911-BCBB-EABE5403B0B8";
	setAttr -k off ".v";
	setAttr ".pu" 0.59375;
	setAttr ".pv" 0.46875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve143" -p "pSphere1Follicle5946";
	rename -uid "541B29CF-4870-96BB-0CDE-8B82DCC64852";
createNode nurbsCurve -n "curveShape143" -p "curve143";
	rename -uid "8F802362-4CC4-15A0-D357-3590C68D14E8";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "71E29EA8-41BE-C35F-A03A-54A89BE5A835";
createNode follicle -n "pSphere1FollicleShape5953" -p "pSphere1Follicle5953";
	rename -uid "474CC2EF-434D-7E0C-EFAE-EA95E22B2D68";
	setAttr -k off ".v";
	setAttr ".pu" 0.59375;
	setAttr ".pv" 0.53125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve144" -p "pSphere1Follicle5953";
	rename -uid "A3CC163F-4109-CE1C-8F73-4388C1911892";
createNode nurbsCurve -n "curveShape144" -p "curve144";
	rename -uid "733F1F5A-45CC-2ADD-8455-30986F397719";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "55590410-43E8-EF78-1CD6-0B82ADE12FA8";
createNode follicle -n "pSphere1FollicleShape5959" -p "pSphere1Follicle5959";
	rename -uid "DDD72A6B-4994-2831-0AA4-DF832D717BAB";
	setAttr -k off ".v";
	setAttr ".pu" 0.59375;
	setAttr ".pv" 0.59375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve145" -p "pSphere1Follicle5959";
	rename -uid "BAFD5C45-44CE-BDFD-2FCC-AE9ECA3178D5";
createNode nurbsCurve -n "curveShape145" -p "curve145";
	rename -uid "B9346712-45FC-6885-20AA-AE884E22DD08";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "331FC706-4140-255A-0241-ED95E24BCD24";
createNode follicle -n "pSphere1FollicleShape5965" -p "pSphere1Follicle5965";
	rename -uid "73710CBA-4409-D5C7-E274-22BB68F29875";
	setAttr -k off ".v";
	setAttr ".pu" 0.59375;
	setAttr ".pv" 0.65625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve146" -p "pSphere1Follicle5965";
	rename -uid "75757F53-4471-C397-535A-BE90EAD34FA4";
createNode nurbsCurve -n "curveShape146" -p "curve146";
	rename -uid "640387ED-45FD-ED1C-9D4B-0AB0768CB8CD";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "83965D8E-4F50-CA50-B4FC-15A047286702";
createNode follicle -n "pSphere1FollicleShape5971" -p "pSphere1Follicle5971";
	rename -uid "A1CEC817-45F4-8F64-10FA-C0A0957D132E";
	setAttr -k off ".v";
	setAttr ".pu" 0.59375;
	setAttr ".pv" 0.71875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve147" -p "pSphere1Follicle5971";
	rename -uid "B10B19FD-4772-A324-7CFD-6FA2B459F23F";
createNode nurbsCurve -n "curveShape147" -p "curve147";
	rename -uid "F08B8F34-4D16-EA9E-FEEC-73A5DD8F4DE9";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "C82FFD3A-42A7-2E6E-9227-D486633A2435";
createNode follicle -n "pSphere1FollicleShape5977" -p "pSphere1Follicle5977";
	rename -uid "36FF6C59-4DED-F983-4958-B5BA8C9B93EB";
	setAttr -k off ".v";
	setAttr ".pu" 0.59375;
	setAttr ".pv" 0.78125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve148" -p "pSphere1Follicle5977";
	rename -uid "ABC9283C-4B23-A6D7-C7F1-EDA391723404";
createNode nurbsCurve -n "curveShape148" -p "curve148";
	rename -uid "7A2934FB-4DE1-D4B9-B907-DEA4B34A1814";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "0A1D0B8E-4B3A-9B60-EB4D-0193707E2DEE";
createNode follicle -n "pSphere1FollicleShape5984" -p "pSphere1Follicle5984";
	rename -uid "A14DFBDD-49BA-85CE-BC35-0D9024ABE96C";
	setAttr -k off ".v";
	setAttr ".pu" 0.59375;
	setAttr ".pv" 0.84375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve149" -p "pSphere1Follicle5984";
	rename -uid "74167446-449A-CE33-D64D-0BA3719AE60A";
createNode nurbsCurve -n "curveShape149" -p "curve149";
	rename -uid "1A5FA29E-4EC2-6071-7CB3-39AE0D115080";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "B8748001-416E-4A79-B9F9-B89241DDED75";
createNode follicle -n "pSphere1FollicleShape5990" -p "pSphere1Follicle5990";
	rename -uid "F5402A49-4C4A-C3EF-D135-AE81B7810A93";
	setAttr -k off ".v";
	setAttr ".pu" 0.59375;
	setAttr ".pv" 0.90625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve150" -p "pSphere1Follicle5990";
	rename -uid "627221C4-4283-4E8C-E608-1A93747EF5EC";
createNode nurbsCurve -n "curveShape150" -p "curve150";
	rename -uid "0B4F0D76-4594-6615-31F1-6CBD845CCA6A";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "1994D0E3-4991-89EA-8D66-2787A0690A84";
createNode follicle -n "pSphere1FollicleShape6509" -p "pSphere1Follicle6509";
	rename -uid "2AA9C8D8-4BA8-B910-48DE-CDAB27D86359";
	setAttr -k off ".v";
	setAttr ".pu" 0.65625;
	setAttr ".pv" 0.09375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve151" -p "pSphere1Follicle6509";
	rename -uid "FA8DEACC-4D3F-1A82-02A1-8C88051642EA";
createNode nurbsCurve -n "curveShape151" -p "curve151";
	rename -uid "04FE7EE3-496A-F128-74D0-048BB7F1061F";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "A275104A-4568-E25F-D528-8B8B57FD3FF5";
createNode follicle -n "pSphere1FollicleShape6515" -p "pSphere1Follicle6515";
	rename -uid "36BFD2D1-465C-F8E7-5335-31A4DCB3A35A";
	setAttr -k off ".v";
	setAttr ".pu" 0.65625;
	setAttr ".pv" 0.15625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve152" -p "pSphere1Follicle6515";
	rename -uid "E06E2894-497C-8F0C-EB47-078B85974368";
createNode nurbsCurve -n "curveShape152" -p "curve152";
	rename -uid "21305C70-4EA0-F8FE-245D-5D97B9004DDE";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "3ABEBF84-4B1C-A011-AD90-5A8C3A390630";
createNode follicle -n "pSphere1FollicleShape6522" -p "pSphere1Follicle6522";
	rename -uid "0E7BB773-4267-DFBA-70BA-FBAEB8402B9C";
	setAttr -k off ".v";
	setAttr ".pu" 0.65625;
	setAttr ".pv" 0.21875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve153" -p "pSphere1Follicle6522";
	rename -uid "159653FD-4DDC-12DF-3DF9-26B0799DBE23";
createNode nurbsCurve -n "curveShape153" -p "curve153";
	rename -uid "FC5F5B64-4421-EAD1-F3CE-8E84E300DAF1";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "A01C6695-4C17-6052-DDFB-B5AE87F4AB24";
createNode follicle -n "pSphere1FollicleShape6528" -p "pSphere1Follicle6528";
	rename -uid "4C2D45F3-45F0-B43F-3904-5FAB61C79899";
	setAttr -k off ".v";
	setAttr ".pu" 0.65625;
	setAttr ".pv" 0.28125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve154" -p "pSphere1Follicle6528";
	rename -uid "41DF3A0F-4D15-DEA9-3633-F5BD7219E1C4";
createNode nurbsCurve -n "curveShape154" -p "curve154";
	rename -uid "AE420E07-4E15-2A46-C97B-9F9D7DAD0C9D";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "5097D5BC-4C86-C929-0CB7-C2B0BCCDC4D1";
createNode follicle -n "pSphere1FollicleShape6534" -p "pSphere1Follicle6534";
	rename -uid "01870DA0-4491-D8EE-D382-AEB30EC9F185";
	setAttr -k off ".v";
	setAttr ".pu" 0.65625;
	setAttr ".pv" 0.34375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve155" -p "pSphere1Follicle6534";
	rename -uid "CCBCF15B-4B52-E6FA-2E80-BCBFAAAB5549";
createNode nurbsCurve -n "curveShape155" -p "curve155";
	rename -uid "686DB105-4B93-DCAD-5F1F-449C0EF83500";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "593FF8B4-4C2D-CB72-60CC-D58503F13722";
createNode follicle -n "pSphere1FollicleShape6540" -p "pSphere1Follicle6540";
	rename -uid "0291593F-490A-E2A1-FE1B-A0848840D435";
	setAttr -k off ".v";
	setAttr ".pu" 0.65625;
	setAttr ".pv" 0.40625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve156" -p "pSphere1Follicle6540";
	rename -uid "F03173E8-4265-E948-DF46-FF8CCA773E18";
createNode nurbsCurve -n "curveShape156" -p "curve156";
	rename -uid "C7610815-43A1-131D-E585-B99E5AF06F26";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "FF8A0707-4D7D-BC4A-1928-F3B72DE0BA8A";
createNode follicle -n "pSphere1FollicleShape6546" -p "pSphere1Follicle6546";
	rename -uid "C189F98A-4418-DA3F-4B49-AAA13B27F4C2";
	setAttr -k off ".v";
	setAttr ".pu" 0.65625;
	setAttr ".pv" 0.46875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve157" -p "pSphere1Follicle6546";
	rename -uid "483C6ABE-44E4-29EE-B154-F2815D82474A";
createNode nurbsCurve -n "curveShape157" -p "curve157";
	rename -uid "3F438FA8-4486-95AF-C182-73A6790F441D";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "3357D401-4C9E-0B47-F377-C49A0C24A287";
createNode follicle -n "pSphere1FollicleShape6553" -p "pSphere1Follicle6553";
	rename -uid "F096A4CA-4D2C-879F-3236-36A897B5A94E";
	setAttr -k off ".v";
	setAttr ".pu" 0.65625;
	setAttr ".pv" 0.53125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve158" -p "pSphere1Follicle6553";
	rename -uid "E7A1AA96-4CC7-B430-5DDD-37B86B7BB774";
createNode nurbsCurve -n "curveShape158" -p "curve158";
	rename -uid "E4625137-4914-E479-361B-46B6B9AD70E0";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "A222C497-4A78-E574-CB2C-8B85BE5636B6";
createNode follicle -n "pSphere1FollicleShape6559" -p "pSphere1Follicle6559";
	rename -uid "548EFEAF-498E-51C6-A8EE-EABDE5FA7046";
	setAttr -k off ".v";
	setAttr ".pu" 0.65625;
	setAttr ".pv" 0.59375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve159" -p "pSphere1Follicle6559";
	rename -uid "4D038ED4-48E0-9047-1A1E-8CB8B7491819";
createNode nurbsCurve -n "curveShape159" -p "curve159";
	rename -uid "3E1A50C9-46A2-78D0-8365-54BA67023E60";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "98CA70E4-4DF2-B505-0AFD-7280E916E9E6";
createNode follicle -n "pSphere1FollicleShape6565" -p "pSphere1Follicle6565";
	rename -uid "E98B0EC7-48B2-E32A-CDA4-2F8ADA1271AC";
	setAttr -k off ".v";
	setAttr ".pu" 0.65625;
	setAttr ".pv" 0.65625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve160" -p "pSphere1Follicle6565";
	rename -uid "218A7493-4A4A-D6B0-1241-978DA89B9879";
createNode nurbsCurve -n "curveShape160" -p "curve160";
	rename -uid "7A062A65-40CD-6526-9F54-1FB27A6AD635";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "845A7187-4E78-4EAC-4269-B08E67FC22CC";
createNode follicle -n "pSphere1FollicleShape6571" -p "pSphere1Follicle6571";
	rename -uid "DBECD112-46E5-C8E3-5E7F-138DD3C22490";
	setAttr -k off ".v";
	setAttr ".pu" 0.65625;
	setAttr ".pv" 0.71875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve161" -p "pSphere1Follicle6571";
	rename -uid "2190354F-4B12-DD70-7AB2-F49346DD7204";
createNode nurbsCurve -n "curveShape161" -p "curve161";
	rename -uid "0092E169-42BB-4AAB-E397-AF9ECCB9EC08";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "E1A793E8-49A8-4C24-F021-308BDB2F084A";
createNode follicle -n "pSphere1FollicleShape6577" -p "pSphere1Follicle6577";
	rename -uid "419FF584-492E-F7B9-4EED-3694AC1E63DA";
	setAttr -k off ".v";
	setAttr ".pu" 0.65625;
	setAttr ".pv" 0.78125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve162" -p "pSphere1Follicle6577";
	rename -uid "1CE12D39-44C8-25AB-E68E-DFBE71C1FB48";
createNode nurbsCurve -n "curveShape162" -p "curve162";
	rename -uid "6F4EE053-4700-4AD9-52BA-71AAA5F510D4";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "E51D8E3B-4FC9-F1EC-F0A2-EEBE514BBCED";
createNode follicle -n "pSphere1FollicleShape6584" -p "pSphere1Follicle6584";
	rename -uid "BFCACE79-4CE5-D1FF-5DDB-C5B6DFEBA857";
	setAttr -k off ".v";
	setAttr ".pu" 0.65625;
	setAttr ".pv" 0.84375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve163" -p "pSphere1Follicle6584";
	rename -uid "241A6CBD-41F7-5C2B-7BA2-D984607BD614";
createNode nurbsCurve -n "curveShape163" -p "curve163";
	rename -uid "52202C66-42F7-96FE-2158-B09AB82F0B0E";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "C2FB2433-43DB-8E00-E30A-9B933440FC1F";
createNode follicle -n "pSphere1FollicleShape6590" -p "pSphere1Follicle6590";
	rename -uid "FE6A9C7E-41DB-F6C6-1352-E6815FEA6EC3";
	setAttr -k off ".v";
	setAttr ".pu" 0.65625;
	setAttr ".pv" 0.90625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve164" -p "pSphere1Follicle6590";
	rename -uid "A3BF6128-4C55-EFD9-B52E-389316D9263B";
createNode nurbsCurve -n "curveShape164" -p "curve164";
	rename -uid "5B50ED89-483F-40F2-95C1-71AFAA16B00A";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "8314ACAB-4DD1-29E2-2C6B-6BAC4B832D16";
createNode follicle -n "pSphere1FollicleShape7103" -p "pSphere1Follicle7103";
	rename -uid "0C03C78E-4EDF-75B9-047A-33B1E00D6DBF";
	setAttr -k off ".v";
	setAttr ".pu" 0.71875;
	setAttr ".pv" 0.03125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve165" -p "pSphere1Follicle7103";
	rename -uid "362A7C8D-4F98-0C21-6D24-71A663C81A8D";
createNode nurbsCurve -n "curveShape165" -p "curve165";
	rename -uid "0A395FAF-4D13-E54C-A2CE-499E65F5CE41";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "DB21150D-4EA0-5942-29EE-AF8DBAF2F4E5";
createNode follicle -n "pSphere1FollicleShape7109" -p "pSphere1Follicle7109";
	rename -uid "BD4B5A8F-44F2-5A22-A431-91A93EC390FE";
	setAttr -k off ".v";
	setAttr ".pu" 0.71875;
	setAttr ".pv" 0.09375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve166" -p "pSphere1Follicle7109";
	rename -uid "FA57753E-44A0-9408-760E-40A3B47E54A0";
createNode nurbsCurve -n "curveShape166" -p "curve166";
	rename -uid "E2D7F347-4056-0FC3-ACED-D1BBF9D77C2D";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "56850474-464F-D01B-FC82-45BC65A60D84";
createNode follicle -n "pSphere1FollicleShape7115" -p "pSphere1Follicle7115";
	rename -uid "E45FF454-40A7-915B-BD8D-31A4C003AFD1";
	setAttr -k off ".v";
	setAttr ".pu" 0.71875;
	setAttr ".pv" 0.15625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve167" -p "pSphere1Follicle7115";
	rename -uid "AE5C4747-4E00-CE52-DD42-699DC021C369";
createNode nurbsCurve -n "curveShape167" -p "curve167";
	rename -uid "0EF9B52F-47A6-E3AD-54FB-09B32B4A70A1";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "B275F79F-448F-3D6D-4A70-D8AE8B1652F0";
createNode follicle -n "pSphere1FollicleShape7122" -p "pSphere1Follicle7122";
	rename -uid "88CF255F-4A69-490B-7B6E-79B5D7932F46";
	setAttr -k off ".v";
	setAttr ".pu" 0.71875;
	setAttr ".pv" 0.21875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve168" -p "pSphere1Follicle7122";
	rename -uid "07D87EE7-4D3D-0038-8E29-7793B24D2D11";
createNode nurbsCurve -n "curveShape168" -p "curve168";
	rename -uid "BA8D490F-4C7C-A5B5-721A-669DDD4A6876";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "A70FC2C6-4157-5032-EF09-89A2483DC163";
createNode follicle -n "pSphere1FollicleShape7128" -p "pSphere1Follicle7128";
	rename -uid "C5F8F264-4D04-0FCA-8757-86AE589C874D";
	setAttr -k off ".v";
	setAttr ".pu" 0.71875;
	setAttr ".pv" 0.28125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve169" -p "pSphere1Follicle7128";
	rename -uid "4B4EF517-4D35-0304-056F-4380BB57D43E";
createNode nurbsCurve -n "curveShape169" -p "curve169";
	rename -uid "3EEDE674-4FFC-29A2-EF53-DCB866BE3C9A";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "64E3CC2F-4C69-21DB-9E3E-A5837C88FB95";
createNode follicle -n "pSphere1FollicleShape7134" -p "pSphere1Follicle7134";
	rename -uid "CAFE5D64-4BD1-21D7-52DE-E0AB72883B75";
	setAttr -k off ".v";
	setAttr ".pu" 0.71875;
	setAttr ".pv" 0.34375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve170" -p "pSphere1Follicle7134";
	rename -uid "C6037237-453A-577B-881A-B988B67BBC83";
createNode nurbsCurve -n "curveShape170" -p "curve170";
	rename -uid "A20490B1-4423-6F4E-3776-6EAE6BBC9382";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "9FF70C6C-4A6F-4788-240D-57AF38D69A3A";
createNode follicle -n "pSphere1FollicleShape7140" -p "pSphere1Follicle7140";
	rename -uid "6079E959-4BA9-3EC2-9C5C-71B4E9750C5F";
	setAttr -k off ".v";
	setAttr ".pu" 0.71875;
	setAttr ".pv" 0.40625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve171" -p "pSphere1Follicle7140";
	rename -uid "C8500149-42F0-22D1-DE15-9BAD1B70311C";
createNode nurbsCurve -n "curveShape171" -p "curve171";
	rename -uid "270B2DA5-4A93-DDD6-85EE-A5B00359586A";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "6EEB27EA-42E1-D5BF-84B9-B5B902DB7E54";
createNode follicle -n "pSphere1FollicleShape7146" -p "pSphere1Follicle7146";
	rename -uid "D565B55B-40E4-4898-566E-21BE3D698D7C";
	setAttr -k off ".v";
	setAttr ".pu" 0.71875;
	setAttr ".pv" 0.46875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve172" -p "pSphere1Follicle7146";
	rename -uid "463374D1-4A78-AB65-D485-4AAB662E4A6D";
createNode nurbsCurve -n "curveShape172" -p "curve172";
	rename -uid "671D8649-4105-4A1F-7374-A9BAF4C4E26E";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "2CE73B78-47C0-B387-C753-63AD2F7D6626";
createNode follicle -n "pSphere1FollicleShape7153" -p "pSphere1Follicle7153";
	rename -uid "0945DDC2-4E8C-35BC-A396-7DBC490A1085";
	setAttr -k off ".v";
	setAttr ".pu" 0.71875;
	setAttr ".pv" 0.53125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve173" -p "pSphere1Follicle7153";
	rename -uid "D710AFD7-476E-A2B4-9215-81AA7ADF87B1";
createNode nurbsCurve -n "curveShape173" -p "curve173";
	rename -uid "8E980CC2-4CBE-5E6B-2804-1D92CCA339C4";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "DCF9F95C-4101-2E1D-3531-C48A5C975DE2";
createNode follicle -n "pSphere1FollicleShape7159" -p "pSphere1Follicle7159";
	rename -uid "C7382681-4857-ACD8-E77C-AA877C7D3985";
	setAttr -k off ".v";
	setAttr ".pu" 0.71875;
	setAttr ".pv" 0.59375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve174" -p "pSphere1Follicle7159";
	rename -uid "1580638F-4830-3661-698F-75BED42D226A";
createNode nurbsCurve -n "curveShape174" -p "curve174";
	rename -uid "C965F0BE-4E9A-84B8-D984-D48D31589859";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "CA0219E8-41B8-965F-3244-678D19EB1CB3";
createNode follicle -n "pSphere1FollicleShape7165" -p "pSphere1Follicle7165";
	rename -uid "715A8972-4906-FCC3-010D-61A786395FE2";
	setAttr -k off ".v";
	setAttr ".pu" 0.71875;
	setAttr ".pv" 0.65625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve175" -p "pSphere1Follicle7165";
	rename -uid "A7D27063-4064-7A74-CB90-4AB5FC053E18";
createNode nurbsCurve -n "curveShape175" -p "curve175";
	rename -uid "E43B48D2-489C-B3CF-EF67-5AB970833286";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "4CB4EF91-4381-2342-B29C-CBB66639861E";
createNode follicle -n "pSphere1FollicleShape7171" -p "pSphere1Follicle7171";
	rename -uid "30980D4A-4CF6-2BF7-53A4-C1A2E920C4A3";
	setAttr -k off ".v";
	setAttr ".pu" 0.71875;
	setAttr ".pv" 0.71875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve176" -p "pSphere1Follicle7171";
	rename -uid "D96CD4FB-44BF-A4B7-F4AC-E59EDD1A0B13";
createNode nurbsCurve -n "curveShape176" -p "curve176";
	rename -uid "8601E5A6-4D3A-8A35-4646-71B04AAC1F47";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "6A026C89-482C-469E-1559-45A18940923D";
createNode follicle -n "pSphere1FollicleShape7177" -p "pSphere1Follicle7177";
	rename -uid "3654FB0B-49F2-BE1C-4D9D-AC9FEA3F5201";
	setAttr -k off ".v";
	setAttr ".pu" 0.71875;
	setAttr ".pv" 0.78125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve177" -p "pSphere1Follicle7177";
	rename -uid "B526D213-4886-8D08-410F-1C956CC03C61";
createNode nurbsCurve -n "curveShape177" -p "curve177";
	rename -uid "52AEC080-41DE-5B32-3749-609BA5226AEC";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "C49EF62D-4D29-D2F2-695E-5CAFACE4F751";
createNode follicle -n "pSphere1FollicleShape7184" -p "pSphere1Follicle7184";
	rename -uid "B4AF19B7-4BEC-FCD4-F623-4A9D8F6CFFB7";
	setAttr -k off ".v";
	setAttr ".pu" 0.71875;
	setAttr ".pv" 0.84375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve178" -p "pSphere1Follicle7184";
	rename -uid "ECB835AA-469A-70D8-9467-C48F859D0C60";
createNode nurbsCurve -n "curveShape178" -p "curve178";
	rename -uid "DA643F52-4C61-EA8E-6EF6-B7AA26951856";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "7D962741-4CB0-6BD7-B66C-458312B38CCD";
createNode follicle -n "pSphere1FollicleShape7190" -p "pSphere1Follicle7190";
	rename -uid "4202E773-4C5F-A5ED-7A49-6C9C1E522698";
	setAttr -k off ".v";
	setAttr ".pu" 0.71875;
	setAttr ".pv" 0.90625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve179" -p "pSphere1Follicle7190";
	rename -uid "728D0C44-4B4F-EEAA-E2E7-778D9D65C0F4";
createNode nurbsCurve -n "curveShape179" -p "curve179";
	rename -uid "82CD4880-40F1-E3A1-D948-81BBACA7ACC8";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "1A75057B-40CE-B13C-A93C-DC92E4AEE29D";
createNode follicle -n "pSphere1FollicleShape7196" -p "pSphere1Follicle7196";
	rename -uid "21D73683-4574-BB2A-2BC8-84A961277C36";
	setAttr -k off ".v";
	setAttr ".pu" 0.71875;
	setAttr ".pv" 0.96875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve180" -p "pSphere1Follicle7196";
	rename -uid "EDAA360C-4481-A14B-9584-8C900D7FBF2D";
createNode nurbsCurve -n "curveShape180" -p "curve180";
	rename -uid "E1230450-4450-D9A3-FE90-BC8CE7406E66";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "DFE50018-4661-1E48-3325-6DAD2CE45E15";
createNode follicle -n "pSphere1FollicleShape7703" -p "pSphere1Follicle7703";
	rename -uid "F82F7C81-400A-3487-BF8C-E994C1550313";
	setAttr -k off ".v";
	setAttr ".pu" 0.78125;
	setAttr ".pv" 0.03125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve181" -p "pSphere1Follicle7703";
	rename -uid "4174C98A-4205-4BF4-E5AF-2DACA33B83CE";
createNode nurbsCurve -n "curveShape181" -p "curve181";
	rename -uid "BDBAF4ED-4684-E130-0450-A799ED611F6A";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "77C7B6C3-4A58-5F52-195C-65A46FFC818D";
createNode follicle -n "pSphere1FollicleShape7709" -p "pSphere1Follicle7709";
	rename -uid "2D381EAA-4E28-C357-99F5-F2B02E2874D5";
	setAttr -k off ".v";
	setAttr ".pu" 0.78125;
	setAttr ".pv" 0.09375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve182" -p "pSphere1Follicle7709";
	rename -uid "60CDAFF3-4A21-56C1-4238-3C9AB54411B7";
createNode nurbsCurve -n "curveShape182" -p "curve182";
	rename -uid "E6FB46B3-4015-30D6-DAC1-088BC352EBED";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "78F30DDF-4346-E604-1140-D681791E6A31";
createNode follicle -n "pSphere1FollicleShape7715" -p "pSphere1Follicle7715";
	rename -uid "1DDD497C-4794-33C0-A15E-A282A2BA1DD3";
	setAttr -k off ".v";
	setAttr ".pu" 0.78125;
	setAttr ".pv" 0.15625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve183" -p "pSphere1Follicle7715";
	rename -uid "0883EDAC-45DD-7B59-5E3C-F297F9914F69";
createNode nurbsCurve -n "curveShape183" -p "curve183";
	rename -uid "8AB9C22E-4924-2032-5C6E-069B2915E92C";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "ABBDF45C-4193-575E-C352-F480E5FD35EF";
createNode follicle -n "pSphere1FollicleShape7722" -p "pSphere1Follicle7722";
	rename -uid "E02A9182-4BA4-1018-DECE-55A3BBAEBFAB";
	setAttr -k off ".v";
	setAttr ".pu" 0.78125;
	setAttr ".pv" 0.21875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve184" -p "pSphere1Follicle7722";
	rename -uid "763A6088-4009-9A4B-20EC-A9A943C442BE";
createNode nurbsCurve -n "curveShape184" -p "curve184";
	rename -uid "2C1C2619-4355-62E7-E2EA-EAA200C8DEFE";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "9A608BAD-419F-F4BB-8686-2AA63633ADC8";
createNode follicle -n "pSphere1FollicleShape7728" -p "pSphere1Follicle7728";
	rename -uid "A660CB30-4A6C-94AC-825E-2E9D6F56E5E1";
	setAttr -k off ".v";
	setAttr ".pu" 0.78125;
	setAttr ".pv" 0.28125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve185" -p "pSphere1Follicle7728";
	rename -uid "046727AE-4F67-F0C6-F427-3B9A9555CDE6";
createNode nurbsCurve -n "curveShape185" -p "curve185";
	rename -uid "8AF03A3B-49FC-43DF-8C3B-7B9E38A9A24E";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "8BCAFCAE-4AE3-C9CB-F41B-4B9314816899";
createNode follicle -n "pSphere1FollicleShape7734" -p "pSphere1Follicle7734";
	rename -uid "ABC9CB52-4053-64FA-2DE5-B586FAD6F319";
	setAttr -k off ".v";
	setAttr ".pu" 0.78125;
	setAttr ".pv" 0.34375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve186" -p "pSphere1Follicle7734";
	rename -uid "FA97D574-41E8-F8E4-E09F-DB99959480F4";
createNode nurbsCurve -n "curveShape186" -p "curve186";
	rename -uid "DD634CF1-40F4-3FBB-40F3-94A8643196D8";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "92319A2F-4550-B41F-72CA-1694F6FCB162";
createNode follicle -n "pSphere1FollicleShape7740" -p "pSphere1Follicle7740";
	rename -uid "78D4E54A-4B1B-27C5-525D-D89889B16932";
	setAttr -k off ".v";
	setAttr ".pu" 0.78125;
	setAttr ".pv" 0.40625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve187" -p "pSphere1Follicle7740";
	rename -uid "E19211BA-4729-ADB9-9E7E-769FFB0A3825";
createNode nurbsCurve -n "curveShape187" -p "curve187";
	rename -uid "BD62DF08-4E23-6B0D-664C-1D82E3CA3F97";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "1C4E81CE-42D3-1F92-442B-A0A0B7526E2A";
createNode follicle -n "pSphere1FollicleShape7746" -p "pSphere1Follicle7746";
	rename -uid "900C5CB4-4945-B396-D560-F98BEDB1AEE4";
	setAttr -k off ".v";
	setAttr ".pu" 0.78125;
	setAttr ".pv" 0.46875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve188" -p "pSphere1Follicle7746";
	rename -uid "A69F2C58-4D11-4657-975D-CEAB04BCD147";
createNode nurbsCurve -n "curveShape188" -p "curve188";
	rename -uid "958564FA-49F2-0600-C196-5BA30989EE69";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "B56D3FB2-4F30-8208-ABF2-AAA6C22C1EF8";
createNode follicle -n "pSphere1FollicleShape7753" -p "pSphere1Follicle7753";
	rename -uid "F6062FAF-49D7-8074-E5E2-E8815D5BBDF6";
	setAttr -k off ".v";
	setAttr ".pu" 0.78125;
	setAttr ".pv" 0.53125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve189" -p "pSphere1Follicle7753";
	rename -uid "5DAF515E-4F8E-A8CF-14C3-1CBCD858C207";
createNode nurbsCurve -n "curveShape189" -p "curve189";
	rename -uid "34F02938-4AC3-2FA7-83B8-FDB5CA1C165A";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "4601BE66-40D9-F3C5-A6E1-408EBCB581B8";
createNode follicle -n "pSphere1FollicleShape7759" -p "pSphere1Follicle7759";
	rename -uid "34850800-444A-EE9E-6CB2-C39F432F0EDB";
	setAttr -k off ".v";
	setAttr ".pu" 0.78125;
	setAttr ".pv" 0.59375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve190" -p "pSphere1Follicle7759";
	rename -uid "AFB9E9BE-4CFD-6806-A824-05B3B2A99A30";
createNode nurbsCurve -n "curveShape190" -p "curve190";
	rename -uid "1F196D4D-4BD2-4001-27FF-688503B48274";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "2D737328-491F-E8C6-090C-8ABDBACD9F74";
createNode follicle -n "pSphere1FollicleShape7765" -p "pSphere1Follicle7765";
	rename -uid "A5E90D37-4FFC-554C-096D-8FAA246937CA";
	setAttr -k off ".v";
	setAttr ".pu" 0.78125;
	setAttr ".pv" 0.65625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve191" -p "pSphere1Follicle7765";
	rename -uid "2010BF53-4B4F-56AE-7830-2E93243DCCBD";
createNode nurbsCurve -n "curveShape191" -p "curve191";
	rename -uid "6CBA94AB-46B3-FF0E-9126-A0BD74779AFA";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "EC885E1B-4D5D-AE94-545A-D3BED5E01EF2";
createNode follicle -n "pSphere1FollicleShape7771" -p "pSphere1Follicle7771";
	rename -uid "464F2C9A-4240-7836-166E-7EB437984F0D";
	setAttr -k off ".v";
	setAttr ".pu" 0.78125;
	setAttr ".pv" 0.71875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve192" -p "pSphere1Follicle7771";
	rename -uid "C77A831A-437E-6AE0-F5CD-C8A7837D685D";
createNode nurbsCurve -n "curveShape192" -p "curve192";
	rename -uid "75C0C8A3-4BEC-CC3E-3936-B88E05248E5B";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "B153E0D9-41F3-FE21-EE5D-339BCA391767";
createNode follicle -n "pSphere1FollicleShape7777" -p "pSphere1Follicle7777";
	rename -uid "102B09C0-409A-82DE-7BCA-67B2322D4593";
	setAttr -k off ".v";
	setAttr ".pu" 0.78125;
	setAttr ".pv" 0.78125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve193" -p "pSphere1Follicle7777";
	rename -uid "31A0E30F-4A80-92B9-1B32-7B90B4D60D8C";
createNode nurbsCurve -n "curveShape193" -p "curve193";
	rename -uid "59160B13-4BCD-5C27-9035-9EB7388D842C";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "6BED4735-47C6-3243-3E4A-7390EF05F9A2";
createNode follicle -n "pSphere1FollicleShape7784" -p "pSphere1Follicle7784";
	rename -uid "E0AEE7BA-4B49-D11E-4464-03A9F9A10729";
	setAttr -k off ".v";
	setAttr ".pu" 0.78125;
	setAttr ".pv" 0.84375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve194" -p "pSphere1Follicle7784";
	rename -uid "19A4E03C-4024-0661-ACD7-629AE0AD0870";
createNode nurbsCurve -n "curveShape194" -p "curve194";
	rename -uid "39399A25-4B59-BB97-4DC4-8BBBE5229B59";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "D1AC1502-494F-5006-903F-0ABF07C94539";
createNode follicle -n "pSphere1FollicleShape7790" -p "pSphere1Follicle7790";
	rename -uid "81D4419D-4E16-FD45-3169-6DB039B4C614";
	setAttr -k off ".v";
	setAttr ".pu" 0.78125;
	setAttr ".pv" 0.90625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve195" -p "pSphere1Follicle7790";
	rename -uid "72921719-4E3F-011E-0BE6-D0905D92E6EA";
createNode nurbsCurve -n "curveShape195" -p "curve195";
	rename -uid "E8FD5DE0-425D-3CAD-E031-2A8F12E6AD73";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "4E0084F1-4ACD-79E0-A79B-A2AAECB180F7";
createNode follicle -n "pSphere1FollicleShape7796" -p "pSphere1Follicle7796";
	rename -uid "E5A603C0-436E-53E8-4923-548E71D190E5";
	setAttr -k off ".v";
	setAttr ".pu" 0.78125;
	setAttr ".pv" 0.96875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve196" -p "pSphere1Follicle7796";
	rename -uid "CF107AE6-44E5-DF61-AA16-0A81C83F6AAA";
createNode nurbsCurve -n "curveShape196" -p "curve196";
	rename -uid "2A158DB0-4F62-E0AD-C93E-03A1D51C38CC";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "BA276ED9-498D-C43D-A216-ED9735577D2A";
createNode follicle -n "pSphere1FollicleShape8409" -p "pSphere1Follicle8409";
	rename -uid "87D5FB63-4943-E0C5-17C3-21942278E6C6";
	setAttr -k off ".v";
	setAttr ".pu" 0.84375;
	setAttr ".pv" 0.09375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve197" -p "pSphere1Follicle8409";
	rename -uid "85C75E60-46CF-439B-A3AE-F6B3F424DFAD";
createNode nurbsCurve -n "curveShape197" -p "curve197";
	rename -uid "55DE83DF-46BD-7842-20C6-6EB2543C7301";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "CABE84C9-4753-1243-A2E9-74B3C9D579FB";
createNode follicle -n "pSphere1FollicleShape8415" -p "pSphere1Follicle8415";
	rename -uid "2CAD011D-4E2F-224D-E83C-1CA774DB7694";
	setAttr -k off ".v";
	setAttr ".pu" 0.84375;
	setAttr ".pv" 0.15625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve198" -p "pSphere1Follicle8415";
	rename -uid "E946258E-4566-1D67-D77C-80A3DE998004";
createNode nurbsCurve -n "curveShape198" -p "curve198";
	rename -uid "0626D57E-4793-F29D-AB7D-D193735ADC50";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "827AB40C-408C-8754-FE02-889C247BD428";
createNode follicle -n "pSphere1FollicleShape8422" -p "pSphere1Follicle8422";
	rename -uid "B898A88F-43A7-7EB7-034C-3BBD06E45189";
	setAttr -k off ".v";
	setAttr ".pu" 0.84375;
	setAttr ".pv" 0.21875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve199" -p "pSphere1Follicle8422";
	rename -uid "6FE868F9-4F0F-9034-3E91-78B34125DC5A";
createNode nurbsCurve -n "curveShape199" -p "curve199";
	rename -uid "C91366AA-42CA-E434-296B-71BF975B6E90";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "EA493FEC-4E9B-AA07-2519-658583D87A6C";
createNode follicle -n "pSphere1FollicleShape8428" -p "pSphere1Follicle8428";
	rename -uid "EF37F4B0-489D-A0F7-3F6B-3B89A757F4EB";
	setAttr -k off ".v";
	setAttr ".pu" 0.84375;
	setAttr ".pv" 0.28125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve200" -p "pSphere1Follicle8428";
	rename -uid "0349B1B7-432D-02B9-2EFD-EBB5333E9DC9";
createNode nurbsCurve -n "curveShape200" -p "curve200";
	rename -uid "328DAA4A-4C4F-A0C3-4C2C-9C8C04E91063";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "61C11FB8-4364-5361-447B-919CFB9AF950";
createNode follicle -n "pSphere1FollicleShape8434" -p "pSphere1Follicle8434";
	rename -uid "52A1E208-4D65-46FE-7AA7-78B74590C616";
	setAttr -k off ".v";
	setAttr ".pu" 0.84375;
	setAttr ".pv" 0.34375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve201" -p "pSphere1Follicle8434";
	rename -uid "A75296E1-4A93-0974-47C6-089B56C3F33A";
createNode nurbsCurve -n "curveShape201" -p "curve201";
	rename -uid "F9A156D0-4E38-E099-B455-4ABFB9BAC734";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "A821CEFB-484C-1AAD-4A49-798D5C8D62E3";
createNode follicle -n "pSphere1FollicleShape8440" -p "pSphere1Follicle8440";
	rename -uid "C71DC8B9-43A4-046E-6FAE-D0AE82161BFE";
	setAttr -k off ".v";
	setAttr ".pu" 0.84375;
	setAttr ".pv" 0.40625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve202" -p "pSphere1Follicle8440";
	rename -uid "341565F5-42EF-0D62-5061-31BDDCDBF1C6";
createNode nurbsCurve -n "curveShape202" -p "curve202";
	rename -uid "6CA5A171-46E5-A842-2332-27A492762A06";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "939ED419-4CFB-876E-40AA-A5BAB24AE2E0";
createNode follicle -n "pSphere1FollicleShape8446" -p "pSphere1Follicle8446";
	rename -uid "F3554164-4E74-4CA5-5B84-2B936C2F01D6";
	setAttr -k off ".v";
	setAttr ".pu" 0.84375;
	setAttr ".pv" 0.46875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve203" -p "pSphere1Follicle8446";
	rename -uid "25E86E00-44D4-E89B-64D5-8188C4569498";
createNode nurbsCurve -n "curveShape203" -p "curve203";
	rename -uid "4361B7B7-487D-2689-D6E0-4D8E92DD5D33";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "F69390B3-46C9-AA96-B854-5C88577F59DE";
createNode follicle -n "pSphere1FollicleShape8453" -p "pSphere1Follicle8453";
	rename -uid "44ADC2C9-48EC-C4B9-BA81-1F8651A854AB";
	setAttr -k off ".v";
	setAttr ".pu" 0.84375;
	setAttr ".pv" 0.53125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve204" -p "pSphere1Follicle8453";
	rename -uid "13DD70F3-45ED-0CA6-CDAA-58B1E159285F";
createNode nurbsCurve -n "curveShape204" -p "curve204";
	rename -uid "5E0BFAA1-48C4-DC9F-F693-B59AB56C4A15";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "1AE8B302-4C66-8B35-7FB4-FB9D1C9FCB43";
createNode follicle -n "pSphere1FollicleShape8459" -p "pSphere1Follicle8459";
	rename -uid "A41C4A4D-41F9-968D-5A4D-DAAD56BEC117";
	setAttr -k off ".v";
	setAttr ".pu" 0.84375;
	setAttr ".pv" 0.59375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve205" -p "pSphere1Follicle8459";
	rename -uid "2DB15EB4-4082-E6F8-266A-75ACC5DFC60B";
createNode nurbsCurve -n "curveShape205" -p "curve205";
	rename -uid "C3CDB572-4F99-D52D-58A9-75A0ED9B276E";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "5A90602A-4C9E-E31C-5CF4-B0A4F82C94E2";
createNode follicle -n "pSphere1FollicleShape8465" -p "pSphere1Follicle8465";
	rename -uid "6197437C-435F-0852-6828-D1A6F5AEB3C4";
	setAttr -k off ".v";
	setAttr ".pu" 0.84375;
	setAttr ".pv" 0.65625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve206" -p "pSphere1Follicle8465";
	rename -uid "16AF0D32-4D7F-16ED-2561-629C6C1E32DB";
createNode nurbsCurve -n "curveShape206" -p "curve206";
	rename -uid "4366B8A8-42B6-23FB-3208-3383B755445D";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "BBA4CF83-46DB-D97B-886D-98BA3E8EE8FE";
createNode follicle -n "pSphere1FollicleShape8471" -p "pSphere1Follicle8471";
	rename -uid "0FD87CE6-4C90-8730-7912-D6B243A19A28";
	setAttr -k off ".v";
	setAttr ".pu" 0.84375;
	setAttr ".pv" 0.71875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve207" -p "pSphere1Follicle8471";
	rename -uid "2BFEE660-40C7-A9AA-0C65-A8B3DF8B44DD";
createNode nurbsCurve -n "curveShape207" -p "curve207";
	rename -uid "1770BE15-4B37-8B87-2755-7FB8EB20FBB3";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "82CE595F-450C-F030-CB9C-EF823F9E1DFE";
createNode follicle -n "pSphere1FollicleShape8477" -p "pSphere1Follicle8477";
	rename -uid "B9873C6C-4E4B-41BA-70D3-8DBBC8EF5426";
	setAttr -k off ".v";
	setAttr ".pu" 0.84375;
	setAttr ".pv" 0.78125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve208" -p "pSphere1Follicle8477";
	rename -uid "B757BEF5-4688-B312-4BD2-68967A0EAA10";
createNode nurbsCurve -n "curveShape208" -p "curve208";
	rename -uid "73831FF8-497A-CF74-876C-DABE8B301A21";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "9D354A65-4FBD-8F97-DB0E-0FAA7F0FE101";
createNode follicle -n "pSphere1FollicleShape8484" -p "pSphere1Follicle8484";
	rename -uid "3B7899A3-42B7-B55D-2B09-0E8CE3E31DE0";
	setAttr -k off ".v";
	setAttr ".pu" 0.84375;
	setAttr ".pv" 0.84375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve209" -p "pSphere1Follicle8484";
	rename -uid "F7D4A699-438C-1786-72F3-E498346669FA";
createNode nurbsCurve -n "curveShape209" -p "curve209";
	rename -uid "F4E4A092-428A-C29A-0920-9C9266F85B31";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "7CEF8418-4E19-4F3C-0C33-46B45CB620D3";
createNode follicle -n "pSphere1FollicleShape8490" -p "pSphere1Follicle8490";
	rename -uid "33A968FA-446A-7AEF-6747-D68AC35D5CFD";
	setAttr -k off ".v";
	setAttr ".pu" 0.84375;
	setAttr ".pv" 0.90625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve210" -p "pSphere1Follicle8490";
	rename -uid "1F337F82-4B69-82D4-F79A-29812BA5B927";
createNode nurbsCurve -n "curveShape210" -p "curve210";
	rename -uid "2FC7D498-4D25-FE76-342A-8EB51C9933F9";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "C82BFA7D-491F-6BED-9F0A-97AFF9E180AA";
createNode follicle -n "pSphere1FollicleShape9009" -p "pSphere1Follicle9009";
	rename -uid "BE425B0B-4B47-3B0F-0C69-6498E67CFEC7";
	setAttr -k off ".v";
	setAttr ".pu" 0.90625;
	setAttr ".pv" 0.09375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve211" -p "pSphere1Follicle9009";
	rename -uid "D055FBF5-4FB1-E155-A88F-818071B9AEF1";
createNode nurbsCurve -n "curveShape211" -p "curve211";
	rename -uid "08909C04-47BD-7DAE-981F-5F825BDFD260";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "EE33017B-4CF6-2354-A709-C19832E2D40F";
createNode follicle -n "pSphere1FollicleShape9015" -p "pSphere1Follicle9015";
	rename -uid "FBF5AA09-4D22-C9A7-B8D6-85AFDBC635AB";
	setAttr -k off ".v";
	setAttr ".pu" 0.90625;
	setAttr ".pv" 0.15625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve212" -p "pSphere1Follicle9015";
	rename -uid "0C1D35A4-4B47-182B-7FE2-2E850F43A011";
createNode nurbsCurve -n "curveShape212" -p "curve212";
	rename -uid "8A46C51B-494B-DF19-16E3-61AEDC74D3FA";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "9ECBB9A4-40B0-E78B-AECC-72B22B0C093E";
createNode follicle -n "pSphere1FollicleShape9022" -p "pSphere1Follicle9022";
	rename -uid "CD3525EB-4E07-70B9-A6EA-78ACFBA5FDFF";
	setAttr -k off ".v";
	setAttr ".pu" 0.90625;
	setAttr ".pv" 0.21875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve213" -p "pSphere1Follicle9022";
	rename -uid "8F5714C0-4FC6-BFD2-45F4-6898E4CBE0A1";
createNode nurbsCurve -n "curveShape213" -p "curve213";
	rename -uid "99925857-4057-47F9-F0C2-7BA13B805E7A";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "EB49839F-4F49-4C34-4397-ACA7AB8701BC";
createNode follicle -n "pSphere1FollicleShape9028" -p "pSphere1Follicle9028";
	rename -uid "44F251EB-4DC9-7B5B-32E8-15ADC5380CAA";
	setAttr -k off ".v";
	setAttr ".pu" 0.90625;
	setAttr ".pv" 0.28125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve214" -p "pSphere1Follicle9028";
	rename -uid "F002B7CC-4FEC-0D22-D48E-38B57ED63386";
createNode nurbsCurve -n "curveShape214" -p "curve214";
	rename -uid "08C4CF5F-47C0-EC7B-18FB-0CAF6A7AB4DE";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "301B6561-4244-A25F-55DE-A495FA4C28DF";
createNode follicle -n "pSphere1FollicleShape9034" -p "pSphere1Follicle9034";
	rename -uid "DBB2747B-4E21-A3A7-21EF-DDBB288EFAD5";
	setAttr -k off ".v";
	setAttr ".pu" 0.90625;
	setAttr ".pv" 0.34375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve215" -p "pSphere1Follicle9034";
	rename -uid "4822D6EA-4AA5-FA7F-DBC3-A3AB28E22E28";
createNode nurbsCurve -n "curveShape215" -p "curve215";
	rename -uid "ED7E7F47-4A74-569C-2F89-3CADB660F75B";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "6C60A6BD-4129-D7AF-8328-EEAB7A110B6B";
createNode follicle -n "pSphere1FollicleShape9040" -p "pSphere1Follicle9040";
	rename -uid "EF2EBE42-455C-717D-AAF8-5589D946D1BF";
	setAttr -k off ".v";
	setAttr ".pu" 0.90625;
	setAttr ".pv" 0.40625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve216" -p "pSphere1Follicle9040";
	rename -uid "859A0119-4BD8-1E76-D96F-76B36714FD16";
createNode nurbsCurve -n "curveShape216" -p "curve216";
	rename -uid "1768F63A-4319-3DFF-1598-FAAAB6D02E80";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "76739A33-4B49-3E26-B3B0-24A2544B7C71";
createNode follicle -n "pSphere1FollicleShape9046" -p "pSphere1Follicle9046";
	rename -uid "D7DC8409-4315-C2AA-D809-5F8D57A8B8D5";
	setAttr -k off ".v";
	setAttr ".pu" 0.90625;
	setAttr ".pv" 0.46875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve217" -p "pSphere1Follicle9046";
	rename -uid "39A65B23-47A3-3901-3379-9E9CEA70A96E";
createNode nurbsCurve -n "curveShape217" -p "curve217";
	rename -uid "983952E1-4574-11C1-D52D-1A846A6ED7DD";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "C2FFFC9A-4D2A-77D5-53E8-57A77E524E58";
createNode follicle -n "pSphere1FollicleShape9053" -p "pSphere1Follicle9053";
	rename -uid "50B650F2-4461-2F54-2D40-16AD783376B1";
	setAttr -k off ".v";
	setAttr ".pu" 0.90625;
	setAttr ".pv" 0.53125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve218" -p "pSphere1Follicle9053";
	rename -uid "CBA6108D-4C17-79F7-0F92-A2859F989E55";
createNode nurbsCurve -n "curveShape218" -p "curve218";
	rename -uid "968C1984-4EEB-4F58-F474-4DB066D759AE";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "063C826C-4070-F778-589E-EFA611DFA7FD";
createNode follicle -n "pSphere1FollicleShape9059" -p "pSphere1Follicle9059";
	rename -uid "531E191E-4750-B69A-EA09-C08A8076A353";
	setAttr -k off ".v";
	setAttr ".pu" 0.90625;
	setAttr ".pv" 0.59375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve219" -p "pSphere1Follicle9059";
	rename -uid "493FFAB0-46B3-9725-5205-85B07D004E6E";
createNode nurbsCurve -n "curveShape219" -p "curve219";
	rename -uid "E9EE559A-4AEF-6245-BAB6-2B8CE284B337";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "EC306A9B-46E8-E9B4-C6E0-6285BDFFB621";
createNode follicle -n "pSphere1FollicleShape9065" -p "pSphere1Follicle9065";
	rename -uid "D1C41894-4983-1BF6-DC82-2EA1E29331AB";
	setAttr -k off ".v";
	setAttr ".pu" 0.90625;
	setAttr ".pv" 0.65625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve220" -p "pSphere1Follicle9065";
	rename -uid "DFBBE5D6-4829-F8E7-C106-6B83F8C77884";
createNode nurbsCurve -n "curveShape220" -p "curve220";
	rename -uid "485CA024-4103-E34B-25CF-3F84B9E5E8EC";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "762C7206-49F2-B751-C117-F28DFD2D1F9D";
createNode follicle -n "pSphere1FollicleShape9071" -p "pSphere1Follicle9071";
	rename -uid "C8727B2D-45C8-C599-A12E-0FA4BE01DA42";
	setAttr -k off ".v";
	setAttr ".pu" 0.90625;
	setAttr ".pv" 0.71875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve221" -p "pSphere1Follicle9071";
	rename -uid "6A316888-4A63-DB66-9EEE-05B7328B6E2D";
createNode nurbsCurve -n "curveShape221" -p "curve221";
	rename -uid "85B0A226-4A19-B152-ED72-51A7DCBD48AA";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "C0CD2F54-4D65-1337-3079-8A8F26AC8881";
createNode follicle -n "pSphere1FollicleShape9077" -p "pSphere1Follicle9077";
	rename -uid "3387704A-4141-B544-7135-D2B94EF8B132";
	setAttr -k off ".v";
	setAttr ".pu" 0.90625;
	setAttr ".pv" 0.78125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve222" -p "pSphere1Follicle9077";
	rename -uid "0D238865-4FEF-AAAE-0B20-A9AD17FC3228";
createNode nurbsCurve -n "curveShape222" -p "curve222";
	rename -uid "15D823F5-4553-F0B6-27AF-42B3E253D7CE";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "6D7F0547-47B2-FE16-1CFB-F681FB042364";
createNode follicle -n "pSphere1FollicleShape9084" -p "pSphere1Follicle9084";
	rename -uid "80EA136C-4DA7-F926-6086-ED8A4BE2661E";
	setAttr -k off ".v";
	setAttr ".pu" 0.90625;
	setAttr ".pv" 0.84375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve223" -p "pSphere1Follicle9084";
	rename -uid "C000CF84-42F3-9532-BCEC-149C64429478";
createNode nurbsCurve -n "curveShape223" -p "curve223";
	rename -uid "9A56A1A2-484C-E0E2-8B4A-A68E3D819313";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "CEDAF51F-40EB-46F1-B929-58A41C4A5DE1";
createNode follicle -n "pSphere1FollicleShape9090" -p "pSphere1Follicle9090";
	rename -uid "863BA0A9-459D-CF78-8C9C-718B3641F184";
	setAttr -k off ".v";
	setAttr ".pu" 0.90625;
	setAttr ".pv" 0.90625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve224" -p "pSphere1Follicle9090";
	rename -uid "B71EE115-4DA0-9441-DD21-B5A18ABDAC3E";
createNode nurbsCurve -n "curveShape224" -p "curve224";
	rename -uid "E9562580-49F0-2D71-35E0-4AB94CD51BF3";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "55BBD1CD-4628-0BCB-BD01-878782D19AB8";
createNode follicle -n "pSphere1FollicleShape9603" -p "pSphere1Follicle9603";
	rename -uid "58908809-4CEC-7FF7-B10D-0CB001163366";
	setAttr -k off ".v";
	setAttr ".pu" 0.96875;
	setAttr ".pv" 0.03125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve225" -p "pSphere1Follicle9603";
	rename -uid "7F734598-4057-AF72-DD1F-E488C9E45502";
createNode nurbsCurve -n "curveShape225" -p "curve225";
	rename -uid "7FDA63B4-414E-92CF-9545-698E066E960D";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "7F1D4D27-4C80-8E49-1171-5EA406356504";
createNode follicle -n "pSphere1FollicleShape9609" -p "pSphere1Follicle9609";
	rename -uid "7F29BB2B-48BD-6964-3DE8-09BF965EB180";
	setAttr -k off ".v";
	setAttr ".pu" 0.96875;
	setAttr ".pv" 0.09375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve226" -p "pSphere1Follicle9609";
	rename -uid "6493A0B7-4860-6305-9403-08A09BFD9B63";
createNode nurbsCurve -n "curveShape226" -p "curve226";
	rename -uid "B998744C-4A46-E699-2AF0-85A6FF21C57A";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "4F70592A-403D-BC40-CD5D-F0BA45BB5B83";
createNode follicle -n "pSphere1FollicleShape9615" -p "pSphere1Follicle9615";
	rename -uid "D7A8A068-46CC-07E3-A6EE-86BD24E3AA84";
	setAttr -k off ".v";
	setAttr ".pu" 0.96875;
	setAttr ".pv" 0.15625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve227" -p "pSphere1Follicle9615";
	rename -uid "CEE80B89-41C6-28A2-0979-168EC3B82411";
createNode nurbsCurve -n "curveShape227" -p "curve227";
	rename -uid "43D8C60E-46FA-3553-664C-959C88F6F2D4";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "8F9D6A98-466E-E754-7AD1-E9B95731D0F3";
createNode follicle -n "pSphere1FollicleShape9622" -p "pSphere1Follicle9622";
	rename -uid "9E7E387E-4E8F-9CDE-9E29-BC9CA0152879";
	setAttr -k off ".v";
	setAttr ".pu" 0.96875;
	setAttr ".pv" 0.21875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve228" -p "pSphere1Follicle9622";
	rename -uid "72721C61-4679-3450-D088-1A9E4F947D43";
createNode nurbsCurve -n "curveShape228" -p "curve228";
	rename -uid "7EFED237-48AA-8A51-BED9-B59BFAC74039";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "66D6DA1E-422B-61F0-9921-E8830CC58A41";
createNode follicle -n "pSphere1FollicleShape9628" -p "pSphere1Follicle9628";
	rename -uid "7FC9E2D4-41E2-3563-E3C5-D8946A5034CC";
	setAttr -k off ".v";
	setAttr ".pu" 0.96875;
	setAttr ".pv" 0.28125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve229" -p "pSphere1Follicle9628";
	rename -uid "53BDA074-41E6-20B8-5E78-1AA9C99785A1";
createNode nurbsCurve -n "curveShape229" -p "curve229";
	rename -uid "D7AFC977-42BA-3B8E-0DD0-6DB07404F677";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "4FD8D9E9-431E-2A70-7FE2-D08083CDAE1C";
createNode follicle -n "pSphere1FollicleShape9634" -p "pSphere1Follicle9634";
	rename -uid "ED1557C4-46E5-5374-BD2C-B3B43F22773D";
	setAttr -k off ".v";
	setAttr ".pu" 0.96875;
	setAttr ".pv" 0.34375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve230" -p "pSphere1Follicle9634";
	rename -uid "0061FC2C-4298-DDC4-6D93-159FA5AF7A6B";
createNode nurbsCurve -n "curveShape230" -p "curve230";
	rename -uid "7841222F-431F-D931-65A9-4B87BEDEB52B";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "F4CD9D00-4D1E-5662-4B96-148B296734BE";
createNode follicle -n "pSphere1FollicleShape9640" -p "pSphere1Follicle9640";
	rename -uid "5B9BC16D-4544-B2CE-1562-83A1942E13B3";
	setAttr -k off ".v";
	setAttr ".pu" 0.96875;
	setAttr ".pv" 0.40625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve231" -p "pSphere1Follicle9640";
	rename -uid "B83E7904-4F36-65F6-9C52-63B9EC23301D";
createNode nurbsCurve -n "curveShape231" -p "curve231";
	rename -uid "04FDF806-4C1D-9BF3-05C9-10AE3DBA8239";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "B5C530EC-441B-38CE-95E8-F6AA538E183B";
createNode follicle -n "pSphere1FollicleShape9646" -p "pSphere1Follicle9646";
	rename -uid "D7243C5C-4E2B-8DA4-161B-A3903BF00197";
	setAttr -k off ".v";
	setAttr ".pu" 0.96875;
	setAttr ".pv" 0.46875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve232" -p "pSphere1Follicle9646";
	rename -uid "C6A0A7F1-477C-63BC-FCB2-3ABC0A11AFFE";
createNode nurbsCurve -n "curveShape232" -p "curve232";
	rename -uid "43F248F3-4F33-3B07-BDD5-14A12BE0E6BF";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "C7ACF87E-4EF3-E9EF-C0D0-9AB6C18F99EC";
createNode follicle -n "pSphere1FollicleShape9653" -p "pSphere1Follicle9653";
	rename -uid "593A2FC7-44DA-1D35-3506-FF97793B9E25";
	setAttr -k off ".v";
	setAttr ".pu" 0.96875;
	setAttr ".pv" 0.53125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve233" -p "pSphere1Follicle9653";
	rename -uid "30F14228-4A4A-8542-0A00-3C900A2E500E";
createNode nurbsCurve -n "curveShape233" -p "curve233";
	rename -uid "21CD1896-4129-BE1F-6793-EA9B921DFCCE";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "87E089EC-48C4-6E33-6163-4FA04B0FE360";
createNode follicle -n "pSphere1FollicleShape9659" -p "pSphere1Follicle9659";
	rename -uid "43990C3C-4B04-6352-037D-EFA636772F24";
	setAttr -k off ".v";
	setAttr ".pu" 0.96875;
	setAttr ".pv" 0.59375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve234" -p "pSphere1Follicle9659";
	rename -uid "4556AB81-41AC-7B14-1BCF-0D884951DD1A";
createNode nurbsCurve -n "curveShape234" -p "curve234";
	rename -uid "802CC559-4C4C-CF16-57B2-259818456403";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "E2666852-43C6-20B0-489F-5FB64474FB02";
createNode follicle -n "pSphere1FollicleShape9665" -p "pSphere1Follicle9665";
	rename -uid "CB2F9123-417D-3CEC-40DB-43AD05719A2E";
	setAttr -k off ".v";
	setAttr ".pu" 0.96875;
	setAttr ".pv" 0.65625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve235" -p "pSphere1Follicle9665";
	rename -uid "1144FBCA-4EA3-6CE6-FDA1-73B5EB0052EB";
createNode nurbsCurve -n "curveShape235" -p "curve235";
	rename -uid "60DEE917-4875-0553-208F-2EAD56463C2E";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "A8CACA3C-446F-604F-8B98-BDADF364897E";
createNode follicle -n "pSphere1FollicleShape9671" -p "pSphere1Follicle9671";
	rename -uid "B6BE9FCC-4C3A-B67F-33DB-079D8D20F595";
	setAttr -k off ".v";
	setAttr ".pu" 0.96875;
	setAttr ".pv" 0.71875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve236" -p "pSphere1Follicle9671";
	rename -uid "B225B897-4A0B-A9DD-5CA7-08A59FDBFB75";
createNode nurbsCurve -n "curveShape236" -p "curve236";
	rename -uid "55D0A712-4773-5042-D4D3-208CD1715193";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "F07ABC1E-4FC4-3D8F-2024-0085D7E0D212";
createNode follicle -n "pSphere1FollicleShape9677" -p "pSphere1Follicle9677";
	rename -uid "EA9CE0C5-4155-95FE-1082-FAA778008EE4";
	setAttr -k off ".v";
	setAttr ".pu" 0.96875;
	setAttr ".pv" 0.78125;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve237" -p "pSphere1Follicle9677";
	rename -uid "41401594-452E-939E-5550-E59FC9883280";
createNode nurbsCurve -n "curveShape237" -p "curve237";
	rename -uid "76796E66-4BAD-E02D-BAE6-46AA326CFB72";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "D3EC9442-4015-DA89-9F7E-4EAD860BCA63";
createNode follicle -n "pSphere1FollicleShape9684" -p "pSphere1Follicle9684";
	rename -uid "D7C300CD-4975-D3AB-5EB2-96BDAB7D2EDA";
	setAttr -k off ".v";
	setAttr ".pu" 0.96875;
	setAttr ".pv" 0.84375;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve238" -p "pSphere1Follicle9684";
	rename -uid "A95A1294-4139-BC6B-E427-3CBA3FD22A84";
createNode nurbsCurve -n "curveShape238" -p "curve238";
	rename -uid "A4374A57-46BC-C49E-E88C-2BAF695232C4";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "27135DF7-4C8C-2E4C-A196-F39CDD190EC5";
createNode follicle -n "pSphere1FollicleShape9690" -p "pSphere1Follicle9690";
	rename -uid "525621C5-49CE-835A-26C7-48A835A02D06";
	setAttr -k off ".v";
	setAttr ".pu" 0.96875;
	setAttr ".pv" 0.90625;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve239" -p "pSphere1Follicle9690";
	rename -uid "91244384-42CA-17F9-9F3D-34B60F2067EC";
createNode nurbsCurve -n "curveShape239" -p "curve239";
	rename -uid "C08D464C-4B86-0E60-3A62-749CA561C02D";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "CCA2F060-47C7-3978-EEAC-C7A39748252F";
createNode follicle -n "pSphere1FollicleShape9696" -p "pSphere1Follicle9696";
	rename -uid "3D3DB24C-457E-0B56-CB4A-83B6E090CD07";
	setAttr -k off ".v";
	setAttr ".pu" 0.96875;
	setAttr ".pv" 0.96875;
	setAttr -s 2 ".sts[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 0.2 3;
	setAttr -s 2 ".ats[0:1]"  0 1 3 1 0.2 3;
createNode transform -n "curve240" -p "pSphere1Follicle9696";
	rename -uid "E09F05CD-4286-EDCC-4E24-34BF9DA19084";
createNode nurbsCurve -n "curveShape240" -p "curve240";
	rename -uid "32FE877D-47EB-0702-6AC1-2094EE6F84AA";
	addAttr -ci true -sn "ai_sss_sample_distribution" -ln "aiSssSampleDistribution" 
		-min 0 -max 3 -en "blue_noise:blue_noise_Pref:triangle_midpoint:polygon_midpoint" 
		-at "enum";
	addAttr -ci true -sn "ai_sss_sample_spacing" -ln "aiSssSampleSpacing" -dv 0.10000000149011612 
		-at "float";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr ".cc" -type "nurbsCurve" 
		1 9 0 no 3
		10 0 1 2 3 4 5 6 7 8 9
		10
		0 0 0
		0 0 0.33333333329999998
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
	rename -uid "9754430B-4E17-A50A-2C87-399565B7A73B";
createNode pfxHair -n "pfxHairShape1" -p "pfxHair1";
	rename -uid "8718F7BB-4F43-145F-E098-9A81D7929A7C";
	setAttr -k off ".v";
	setAttr ".dpc" 100;
	setAttr ".dam" no;
createNode lightLinker -s -n "lightLinker1";
	rename -uid "AA7DF391-4C24-B2E2-4D39-699322B89D2D";
	setAttr -s 5 ".lnk";
	setAttr -s 5 ".slnk";
createNode displayLayerManager -n "layerManager";
	rename -uid "0009329A-4F53-1E20-1EF1-23B501731286";
createNode displayLayer -n "defaultLayer";
	rename -uid "668FFBFA-46E1-6A24-B9F3-2098A30D6495";
createNode renderLayerManager -n "renderLayerManager";
	rename -uid "0632970B-4ED3-D299-D236-DFBBB7C8C3AB";
createNode renderLayer -n "defaultRenderLayer";
	rename -uid "6B7157AB-458A-F311-C1E0-B6B70D92CD7B";
	setAttr ".g" yes;
createNode aiOptions -s -n "defaultArnoldRenderOptions";
	rename -uid "8564B8D6-455C-A86C-6042-BD8471ABE105";
	addAttr -ci true -sn "ARV_options" -ln "ARV_options" -dt "string";
createNode script -n "uiConfigurationScriptNode";
	rename -uid "F6BEDCAD-4E82-0CB8-B16D-4389FE2828B6";
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
		+ "            -width 1\n            -height 1\n            -sceneRenderFilter 0\n            $editorName;\n        modelEditor -e -viewSelected 0 $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextPanel \"modelPanel\" (localizedPanelLabel(\"Persp View\")) `;\n\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tmodelPanel -edit -l (localizedPanelLabel(\"Persp View\")) -mbv $menusOkayInPanels  $panelName;\n\t\t$editorName = $panelName;\n        modelEditor -e \n            -camera \"persp\" \n            -useInteractiveMode 0\n            -displayLights \"default\" \n            -displayAppearance \"wireframe\" \n            -activeOnly 0\n            -ignorePanZoom 0\n            -wireframeOnShaded 0\n            -headsUpDisplay 1\n            -holdOuts 1\n            -selectionHiliteDisplay 1\n            -useDefaultMaterial 0\n            -bufferMode \"double\" \n            -twoSidedLighting 1\n            -backfaceCulling 0\n            -xray 0\n            -jointXray 0\n"
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
		+ "                -useCustomBackground 1\n                $editorName;\n            stereoCameraView -e -viewSelected 0 $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextPanel \"blendShapePanel\" (localizedPanelLabel(\"Blend Shape\")) `;\n\tif (\"\" != $panelName) {\n\t\t$label = `panel -q -label $panelName`;\n\t\tblendShapePanel -edit -l (localizedPanelLabel(\"Blend Shape\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\tif ($useSceneConfig) {\n        string $configName = `getPanel -cwl (localizedPanelLabel(\"Current Layout\"))`;\n        if (\"\" != $configName) {\n\t\t\tpanelConfiguration -edit -label (localizedPanelLabel(\"Current Layout\")) \n\t\t\t\t-userCreated false\n\t\t\t\t-defaultImage \"vacantCell.xP:/\"\n\t\t\t\t-image \"\"\n\t\t\t\t-sc false\n\t\t\t\t-configString \"global string $gMainPane; paneLayout -e -cn \\\"single\\\" -ps 1 100 100 $gMainPane;\"\n\t\t\t\t-removeAllPanels\n\t\t\t\t-ap false\n\t\t\t\t\t(localizedPanelLabel(\"Persp View\")) \n\t\t\t\t\t\"modelPanel\"\n"
		+ "\t\t\t\t\t\"$panelName = `modelPanel -unParent -l (localizedPanelLabel(\\\"Persp View\\\")) -mbv $menusOkayInPanels `;\\n$editorName = $panelName;\\nmodelEditor -e \\n    -cam `findStartUpCamera persp` \\n    -useInteractiveMode 0\\n    -displayLights \\\"default\\\" \\n    -displayAppearance \\\"wireframe\\\" \\n    -activeOnly 0\\n    -ignorePanZoom 0\\n    -wireframeOnShaded 0\\n    -headsUpDisplay 1\\n    -holdOuts 1\\n    -selectionHiliteDisplay 1\\n    -useDefaultMaterial 0\\n    -bufferMode \\\"double\\\" \\n    -twoSidedLighting 1\\n    -backfaceCulling 0\\n    -xray 0\\n    -jointXray 0\\n    -activeComponentsXray 0\\n    -displayTextures 0\\n    -smoothWireframe 0\\n    -lineWidth 1\\n    -textureAnisotropic 0\\n    -textureHilight 1\\n    -textureSampling 2\\n    -textureDisplay \\\"modulate\\\" \\n    -textureMaxSize 16384\\n    -fogging 0\\n    -fogSource \\\"fragment\\\" \\n    -fogMode \\\"linear\\\" \\n    -fogStart 0\\n    -fogEnd 100\\n    -fogDensity 0.1\\n    -fogColor 0.5 0.5 0.5 1 \\n    -depthOfFieldPreview 1\\n    -maxConstantTransparency 1\\n    -rendererName \\\"vp2Renderer\\\" \\n    -objectFilterShowInHUD 1\\n    -isFiltered 0\\n    -colorResolution 256 256 \\n    -bumpResolution 512 512 \\n    -textureCompression 0\\n    -transparencyAlgorithm \\\"frontAndBackCull\\\" \\n    -transpInShadows 0\\n    -cullingOverride \\\"none\\\" \\n    -lowQualityLighting 0\\n    -maximumNumHardwareLights 1\\n    -occlusionCulling 0\\n    -shadingModel 0\\n    -useBaseRenderer 0\\n    -useReducedRenderer 0\\n    -smallObjectCulling 0\\n    -smallObjectThreshold -1 \\n    -interactiveDisableShadows 0\\n    -interactiveBackFaceCull 0\\n    -sortTransparent 1\\n    -controllers 1\\n    -nurbsCurves 1\\n    -nurbsSurfaces 1\\n    -polymeshes 1\\n    -subdivSurfaces 1\\n    -planes 1\\n    -lights 1\\n    -cameras 1\\n    -controlVertices 1\\n    -hulls 1\\n    -grid 1\\n    -imagePlane 1\\n    -joints 1\\n    -ikHandles 1\\n    -deformers 1\\n    -dynamics 1\\n    -particleInstancers 1\\n    -fluids 1\\n    -hairSystems 1\\n    -follicles 1\\n    -nCloths 1\\n    -nParticles 1\\n    -nRigids 1\\n    -dynamicConstraints 1\\n    -locators 1\\n    -manipulators 1\\n    -pluginShapes 1\\n    -dimensions 1\\n    -handles 1\\n    -pivots 1\\n    -textures 1\\n    -strokes 1\\n    -motionTrails 1\\n    -clipGhosts 1\\n    -greasePencils 1\\n    -shadows 0\\n    -captureSequenceNumber -1\\n    -width 741\\n    -height 675\\n    -sceneRenderFilter 0\\n    $editorName;\\nmodelEditor -e -viewSelected 0 $editorName\"\n"
		+ "\t\t\t\t\t\"modelPanel -edit -l (localizedPanelLabel(\\\"Persp View\\\")) -mbv $menusOkayInPanels  $panelName;\\n$editorName = $panelName;\\nmodelEditor -e \\n    -cam `findStartUpCamera persp` \\n    -useInteractiveMode 0\\n    -displayLights \\\"default\\\" \\n    -displayAppearance \\\"wireframe\\\" \\n    -activeOnly 0\\n    -ignorePanZoom 0\\n    -wireframeOnShaded 0\\n    -headsUpDisplay 1\\n    -holdOuts 1\\n    -selectionHiliteDisplay 1\\n    -useDefaultMaterial 0\\n    -bufferMode \\\"double\\\" \\n    -twoSidedLighting 1\\n    -backfaceCulling 0\\n    -xray 0\\n    -jointXray 0\\n    -activeComponentsXray 0\\n    -displayTextures 0\\n    -smoothWireframe 0\\n    -lineWidth 1\\n    -textureAnisotropic 0\\n    -textureHilight 1\\n    -textureSampling 2\\n    -textureDisplay \\\"modulate\\\" \\n    -textureMaxSize 16384\\n    -fogging 0\\n    -fogSource \\\"fragment\\\" \\n    -fogMode \\\"linear\\\" \\n    -fogStart 0\\n    -fogEnd 100\\n    -fogDensity 0.1\\n    -fogColor 0.5 0.5 0.5 1 \\n    -depthOfFieldPreview 1\\n    -maxConstantTransparency 1\\n    -rendererName \\\"vp2Renderer\\\" \\n    -objectFilterShowInHUD 1\\n    -isFiltered 0\\n    -colorResolution 256 256 \\n    -bumpResolution 512 512 \\n    -textureCompression 0\\n    -transparencyAlgorithm \\\"frontAndBackCull\\\" \\n    -transpInShadows 0\\n    -cullingOverride \\\"none\\\" \\n    -lowQualityLighting 0\\n    -maximumNumHardwareLights 1\\n    -occlusionCulling 0\\n    -shadingModel 0\\n    -useBaseRenderer 0\\n    -useReducedRenderer 0\\n    -smallObjectCulling 0\\n    -smallObjectThreshold -1 \\n    -interactiveDisableShadows 0\\n    -interactiveBackFaceCull 0\\n    -sortTransparent 1\\n    -controllers 1\\n    -nurbsCurves 1\\n    -nurbsSurfaces 1\\n    -polymeshes 1\\n    -subdivSurfaces 1\\n    -planes 1\\n    -lights 1\\n    -cameras 1\\n    -controlVertices 1\\n    -hulls 1\\n    -grid 1\\n    -imagePlane 1\\n    -joints 1\\n    -ikHandles 1\\n    -deformers 1\\n    -dynamics 1\\n    -particleInstancers 1\\n    -fluids 1\\n    -hairSystems 1\\n    -follicles 1\\n    -nCloths 1\\n    -nParticles 1\\n    -nRigids 1\\n    -dynamicConstraints 1\\n    -locators 1\\n    -manipulators 1\\n    -pluginShapes 1\\n    -dimensions 1\\n    -handles 1\\n    -pivots 1\\n    -textures 1\\n    -strokes 1\\n    -motionTrails 1\\n    -clipGhosts 1\\n    -greasePencils 1\\n    -shadows 0\\n    -captureSequenceNumber -1\\n    -width 741\\n    -height 675\\n    -sceneRenderFilter 0\\n    $editorName;\\nmodelEditor -e -viewSelected 0 $editorName\"\n"
		+ "\t\t\t\t$configName;\n\n            setNamedPanelLayout (localizedPanelLabel(\"Current Layout\"));\n        }\n\n        panelHistory -e -clear mainPanelHistory;\n        sceneUIReplacement -clear;\n\t}\n\n\ngrid -spacing 5 -size 12 -divisions 5 -displayAxes yes -displayGridLines yes -displayDivisionLines yes -displayPerspectiveLabels no -displayOrthographicLabels no -displayAxesBold yes -perspectiveLabelPosition axis -orthographicLabelPosition edge;\nviewManip -drawCompass 0 -compassAngle 0 -frontParameters \"\" -homeParameters \"\" -selectionLockParameters \"\";\n}\n");
	setAttr ".st" 3;
createNode script -n "sceneConfigurationScriptNode";
	rename -uid "EF69508E-414F-C726-14CF-43B4613475F7";
	setAttr ".b" -type "string" "playbackOptions -min 1 -max 24 -ast 1 -aet 48 ";
	setAttr ".st" 6;
createNode shadingEngine -n "aiStandard1SG";
	rename -uid "C0BEB0FE-4583-C878-47D9-DDBEB0DD8B18";
	addAttr -ci true -k true -sn "ai_user_options" -ln "aiUserOptions" -dt "string";
	setAttr ".ihi" 0;
	setAttr ".ro" yes;
createNode materialInfo -n "materialInfo1";
	rename -uid "735C854D-4170-6869-9AAE-5EB8B8BFFA6A";
createNode polySphere -n "polySphere1";
	rename -uid "67D71562-4EDF-80A7-825F-CE9F6FE0F8A1";
	setAttr ".r" 6.3190084466120737;
createNode aiAOVFilter -s -n "defaultArnoldFilter";
	rename -uid "BD0204FE-4314-2BDF-4F7F-DEA16B3A173C";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVDriver -s -n "defaultArnoldDriver";
	rename -uid "12A88215-4D1F-B232-27B2-C2BB5508E00D";
	addAttr -ci true -sn "gamma" -ln "gamma" -dv 2.2000000476837158 -min 9.9999997473787516e-05 
		-smx 5 -at "float";
	addAttr -ci true -sn "dither_amplitude" -ln "ditherAmplitude" -dv 1 -at "float";
	setAttr ".ai_translator" -type "string" "tif";
createNode shadingEngine -n "aiUtility1SG";
	rename -uid "F8790BA1-4D33-C818-2880-2788CF6D690D";
	addAttr -ci true -k true -sn "ai_user_options" -ln "aiUserOptions" -dt "string";
	setAttr ".ihi" 0;
	setAttr ".ro" yes;
createNode materialInfo -n "materialInfo2";
	rename -uid "C61B9555-4430-A0C6-7331-938C7F0DF330";
createNode aiStandard -n "aiStandard2";
	rename -uid "3464E137-46CF-46C5-61B3-839761331B53";
createNode shadingEngine -n "aiStandard2SG";
	rename -uid "FAEAE9D9-43A4-8754-9024-5BB5165976D5";
	addAttr -ci true -k true -sn "ai_user_options" -ln "aiUserOptions" -dt "string";
	setAttr ".ihi" 0;
	setAttr ".ro" yes;
createNode materialInfo -n "materialInfo3";
	rename -uid "50412022-416C-429A-8A65-AC91969EFE50";
createNode shapeEditorManager -n "shapeEditorManager";
	rename -uid "5E9C4805-4CBC-2F29-2A53-1B84B220D56B";
createNode poseInterpolatorManager -n "poseInterpolatorManager";
	rename -uid "96353675-420E-4BD7-11EA-55A73AA46554";
createNode aiAOVDriver -s -n "defaultArnoldDisplayDriver";
	rename -uid "11C2BF12-4D68-5A94-F2F8-58A66091C641";
	setAttr ".output_mode" 0;
	setAttr ".ai_translator" -type "string" "maya";
createNode nodeGraphEditorInfo -n "hyperShadePrimaryNodeEditorSavedTabsInfo";
	rename -uid "472B85E4-4D95-C547-3D4D-DF9FFA71E6A8";
	setAttr ".tgi[0].tn" -type "string" "Untitled_1";
	setAttr ".tgi[0].vl" -type "double2" -582.93357549570601 -346.42855766273732 ;
	setAttr ".tgi[0].vh" -type "double2" 575.79071863668048 302.38094036541338 ;
select -ne :time1;
	setAttr -av -k on ".cch";
	setAttr -cb on ".ihi";
	setAttr -k on ".nds";
	setAttr -cb on ".bnm";
	setAttr -k on ".o" 1;
	setAttr ".unw" 1;
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
	setAttr -s 5 ".st";
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
connectAttr "hairSystemShape1.orh" "pfxHairShape1.rhs";
relationship "link" ":lightLinker1" ":initialShadingGroup.message" ":defaultLightSet.message";
relationship "link" ":lightLinker1" ":initialParticleSE.message" ":defaultLightSet.message";
relationship "link" ":lightLinker1" "aiStandard1SG.message" ":defaultLightSet.message";
relationship "link" ":lightLinker1" "aiUtility1SG.message" ":defaultLightSet.message";
relationship "link" ":lightLinker1" "aiStandard2SG.message" ":defaultLightSet.message";
relationship "shadowLink" ":lightLinker1" ":initialShadingGroup.message" ":defaultLightSet.message";
relationship "shadowLink" ":lightLinker1" ":initialParticleSE.message" ":defaultLightSet.message";
relationship "shadowLink" ":lightLinker1" "aiStandard1SG.message" ":defaultLightSet.message";
relationship "shadowLink" ":lightLinker1" "aiUtility1SG.message" ":defaultLightSet.message";
relationship "shadowLink" ":lightLinker1" "aiStandard2SG.message" ":defaultLightSet.message";
connectAttr "layerManager.dli[0]" "defaultLayer.id";
connectAttr "renderLayerManager.rlmi[0]" "defaultRenderLayer.rlid";
connectAttr ":defaultArnoldFilter.msg" ":defaultArnoldRenderOptions.filt";
connectAttr ":defaultArnoldDriver.msg" ":defaultArnoldRenderOptions.drvr";
connectAttr ":defaultArnoldDisplayDriver.msg" ":defaultArnoldRenderOptions.drivers"
		 -na;
connectAttr "aiStandard1SG.msg" "materialInfo1.sg";
connectAttr "aiUtility1SG.msg" "materialInfo2.sg";
connectAttr "aiStandard2.out" "aiStandard2SG.ss";
connectAttr "pSphereShape1.iog" "aiStandard2SG.dsm" -na;
connectAttr "aiStandard2SG.msg" "materialInfo3.sg";
connectAttr "aiStandard2.msg" "materialInfo3.m";
connectAttr "aiStandard2.msg" "materialInfo3.t" -na;
connectAttr "aiStandard1SG.pa" ":renderPartition.st" -na;
connectAttr "aiUtility1SG.pa" ":renderPartition.st" -na;
connectAttr "aiStandard2SG.pa" ":renderPartition.st" -na;
connectAttr "aiStandard2.msg" ":defaultShaderList1.s" -na;
connectAttr "defaultRenderLayer.msg" ":defaultRenderingList1.r" -na;
connectAttr "directionalLightShape1.ltd" ":lightList1.l" -na;
connectAttr "directionalLight1.iog" ":defaultLightSet.dsm" -na;
// End of test.ma

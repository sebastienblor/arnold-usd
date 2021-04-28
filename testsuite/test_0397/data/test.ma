//Maya ASCII 2020 scene
//Name: test.ma
//Last modified: Mon, Mar 08, 2021 04:29:10 pm
//Codeset: UTF-8
requires maya "2020";
requires "stereoCamera" "10.0";
requires -nodeType "aiOptions" -nodeType "aiAOVDriver" -nodeType "aiAOVFilter" -nodeType "aiSkyDomeLight"
		 -nodeType "aiStandardSurface" "mtoa" "4.2.1.1";
requires "stereoCamera" "10.0";
currentUnit -l centimeter -a degree -t film;
fileInfo "application" "maya";
fileInfo "product" "Maya 2020";
fileInfo "version" "2020";
fileInfo "cutIdentifier" "202011110415-b1e20b88e2";
fileInfo "osv" "Linux 3.10.0-1160.11.1.el7.x86_64 #1 SMP Fri Dec 18 16:34:56 UTC 2020 x86_64";
fileInfo "UUID" "92014C80-0001-CD34-6046-50D600000396";
createNode transform -s -n "persp";
	rename -uid "A89275AB-48AC-B5E8-752B-028CB1D83F3A";
	setAttr ".t" -type "double3" -3.2959798661420621 -12.596405043326275 -0.61453056729251987 ;
	setAttr ".r" -type "double3" 67.848544366761161 17.878043963060485 -41.183227946232464 ;
	setAttr ".rp" -type "double3" 1.7763568394002505e-15 4.4408920985006262e-16 -1.7763568394002505e-15 ;
	setAttr ".rpt" -type "double3" 7.4440375845222674e-16 -1.447000687845756e-16 1.5229113450630277e-15 ;
createNode camera -s -n "perspShape" -p "persp";
	rename -uid "59CBE729-4D00-B326-8ECC-6F84FBA33EC7";
	setAttr -k off ".v";
	setAttr ".rnd" no;
	setAttr ".ovr" 1.3;
	setAttr ".fl" 34.999999999999979;
	setAttr ".coi" 29.735580586773544;
	setAttr ".imn" -type "string" "persp";
	setAttr ".den" -type "string" "persp_depth";
	setAttr ".man" -type "string" "persp_mask";
	setAttr ".tp" -type "double3" 4.089077788491025 18.212064343194882 -5.1437003047843284 ;
	setAttr ".hc" -type "string" "viewSet -p %camera";
	setAttr ".dr" yes;
	setAttr ".ai_translator" -type "string" "perspective";
createNode transform -s -n "top";
	rename -uid "6BBFF2FD-4DBB-202C-6FDB-3BB7F3AC727A";
	setAttr ".t" -type "double3" 180.9520263671875 1.2283935546875 1017.969891818786 ;
createNode camera -s -n "topShape" -p "top";
	rename -uid "D2B91A00-4F5A-7EDE-BAA8-44873D97154F";
	setAttr -k off ".v";
	setAttr ".rnd" no;
	setAttr ".coi" 925.14487961175473;
	setAttr ".ow" 391.47795024671052;
	setAttr ".imn" -type "string" "top";
	setAttr ".den" -type "string" "top_depth";
	setAttr ".man" -type "string" "top_mask";
	setAttr ".tp" -type "double3" 180.9520263671875 1.2283935546875 92.82501220703125 ;
	setAttr ".hc" -type "string" "viewSet -t %camera";
	setAttr ".o" yes;
	setAttr ".ai_translator" -type "string" "orthographic";
createNode transform -s -n "front";
	rename -uid "A12BD06F-497F-39DC-1A7A-9ABA18C13E4D";
	setAttr ".t" -type "double3" 180.9520263671875 -1020.5439247611093 92.82501220703125 ;
	setAttr ".r" -type "double3" 90 0 0 ;
createNode camera -s -n "frontShape" -p "front";
	rename -uid "F3BFE4D8-4CD3-BDE4-FC6D-6F8682692047";
	setAttr -k off ".v";
	setAttr ".rnd" no;
	setAttr ".coi" 1021.7723183157968;
	setAttr ".ow" 391.47795024671052;
	setAttr ".imn" -type "string" "front";
	setAttr ".den" -type "string" "front_depth";
	setAttr ".man" -type "string" "front_mask";
	setAttr ".tp" -type "double3" 180.9520263671875 1.2283935546875 92.82501220703125 ;
	setAttr ".hc" -type "string" "viewSet -f %camera";
	setAttr ".o" yes;
	setAttr ".ai_translator" -type "string" "orthographic";
createNode transform -s -n "side";
	rename -uid "7309E49E-41DC-335E-1FE6-42B4FA2AA182";
	setAttr ".t" -type "double3" 1005.3435562537479 1.2283935546875 92.82501220703125 ;
	setAttr ".r" -type "double3" 90 0 90 ;
createNode camera -s -n "sideShape" -p "side";
	rename -uid "FAE1DD17-4181-8E33-3259-F9A731037458";
	setAttr -k off ".v";
	setAttr ".rnd" no;
	setAttr ".coi" 824.39152988656042;
	setAttr ".ow" 207.26318359375;
	setAttr ".imn" -type "string" "side";
	setAttr ".den" -type "string" "side_depth";
	setAttr ".man" -type "string" "side_mask";
	setAttr ".tp" -type "double3" 180.9520263671875 1.2283935546875 92.82501220703125 ;
	setAttr ".hc" -type "string" "viewSet -s %camera";
	setAttr ".o" yes;
	setAttr ".ai_translator" -type "string" "orthographic";
createNode place3dTexture -n "tx_TestMaterial_place3dTexture_";
	rename -uid "E918AD6F-4160-5F41-A6DD-C0A1D2EE35BA";
	setAttr ".r" -type "double3" 0 90 0 ;
	setAttr ".s" -type "double3" 2 2 2 ;
createNode transform -n "aiSkyDomeLight1";
	rename -uid "69F901A7-4D0B-2B76-7FCE-D0A4746BCEE6";
createNode aiSkyDomeLight -n "aiSkyDomeLightShape1" -p "aiSkyDomeLight1";
	rename -uid "60273C02-4996-3795-62D8-03AE40990A44";
	addAttr -ci true -h true -sn "aal" -ln "attributeAliasList" -dt "attributeAlias";
	setAttr -k off ".v";
	setAttr ".csh" no;
	setAttr ".rcsh" no;
	setAttr ".aal" -type "attributeAlias" {"exposure","aiExposure"} ;
createNode transform -n "CAM";
	rename -uid "2763112B-458A-9138-1C2D-E996710A1DC6";
	setAttr ".t" -type "double3" -6.6621435994641161 -35.633443647746688 16.299516498794617 ;
	setAttr -l on ".tx";
	setAttr -l on ".ty";
	setAttr -l on ".tz";
	setAttr ".r" -type "double3" 72.082924915395623 -1.5902773407317584e-15 -23.60509381861236 ;
	setAttr -l on ".rx";
	setAttr -l on ".ry";
	setAttr -l on ".rz";
	setAttr ".s" -type "double3" 5 5 5 ;
createNode camera -n "CAMShape" -p "CAM";
	rename -uid "8F0F8DB0-4248-AE63-5BC4-AD91C68A48B6";
	setAttr -k off ".v";
	setAttr ".fl" 34.999999999999993;
	setAttr ".coi" 71.307532200931433;
	setAttr ".imn" -type "string" "persp";
	setAttr ".den" -type "string" "persp_depth";
	setAttr ".man" -type "string" "persp_mask";
	setAttr ".tp" -type "double3" 354.9429931640625 -58.667083740234375 178.05303955078125 ;
	setAttr ".hc" -type "string" "viewSet -p %camera";
	setAttr ".ai_translator" -type "string" "perspective";
createNode transform -n "pCube1";
	rename -uid "B5B7536B-44FF-323B-6AD6-D4B831E6A87D";
	setAttr ".t" -type "double3" 9.6639726607106695 16.171427052669557 -3.326752506163416 ;
	setAttr ".r" -type "double3" 0 21.316621535051489 0 ;
	setAttr ".s" -type "double3" 13.10355679144814 41.479718744928022 59.500460719251244 ;
createNode mesh -n "pCubeShape1" -p "pCube1";
	rename -uid "96AB0420-431F-154F-6922-72812294E678";
	setAttr -k off ".v";
	setAttr ".vir" yes;
	setAttr ".vif" yes;
	setAttr ".uvst[0].uvsn" -type "string" "map1";
	setAttr ".cuvs" -type "string" "map1";
	setAttr ".dcc" -type "string" "Ambient+Diffuse";
	setAttr ".covm[0]"  0 1 1;
	setAttr ".cdvm[0]"  0 1 1;
	setAttr -s 124 ".vt[0:123]"  0.50000197 -0.16666633 -0.16666698 0.50000185 -0.16666633 0.0078687798
		 0.50000197 0.16666695 -0.16666698 0.50000185 0.16666695 0.0078687798 -0.49999616 -0.16666633 -0.16666698
		 -0.49999431 -0.16666633 0.0078687668 -0.49999616 0.16666695 -0.16666698 -0.49999431 0.16666695 0.0078687668
		 -0.46339053 -0.49145967 0.47251603 -0.37501144 -0.50305623 0.47255701 -0.37744331 -0.48991627 0.49209115
		 -0.3795855 -0.46196085 0.50000012 -0.46475041 -0.46213311 0.49192423 -0.49999249 -0.46346265 0.47247499
		 0.46339804 -0.49145967 0.47251603 0.49999958 -0.46346265 0.47247452 0.4633978 -0.46130472 0.49193856
		 0.37501341 -0.46051568 0.49999997 0.37501341 -0.48949575 0.49208644 0.37501341 -0.50305623 0.47255701
		 -0.46339244 0.48843572 0.47247404 -0.49999249 0.4605163 0.47247499 -0.46339232 0.4605163 0.49193764
		 -0.37500966 0.4605163 0.49999991 -0.37501335 0.48843572 0.49193764 -0.37501347 0.49999997 0.47247404
		 0.46339804 0.48843572 0.47247505 0.37501878 0.49999997 0.47247401 0.37501699 0.48843572 0.49193856
		 0.37501341 0.4605163 0.49999997 0.46339411 0.4605163 0.49193808 0.49999958 0.4605163 0.47247452
		 -0.46339238 0.4605163 -0.49193805 -0.49999994 0.4605163 -0.47247458 -0.46339226 0.48843572 -0.47247452
		 -0.37500578 0.49999997 -0.47247404 -0.37500584 0.48843572 -0.49193755 -0.37501335 0.4605163 -0.49999994
		 0.4633922 0.4605163 -0.49193767 0.37501353 0.4605163 -0.50000012 0.37501341 0.48843572 -0.49193764
		 0.37501341 0.49999997 -0.4724741 0.46339232 0.48843572 -0.4724741 0.50000757 0.4605163 -0.47247505
		 -0.46339226 -0.38333026 -0.47247452 -0.49999994 -0.35541087 -0.47247458 -0.46339238 -0.35541087 -0.49193805
		 -0.37501335 -0.35541087 -0.49999994 -0.37500584 -0.38333026 -0.49193755 -0.37500578 -0.39489466 -0.47247404
		 0.46339232 -0.38333026 -0.4724741 0.37501341 -0.39489466 -0.4724741 0.37501341 -0.38333026 -0.49193764
		 0.37501353 -0.35541087 -0.50000012 0.4633922 -0.35541087 -0.49193767 0.50000757 -0.35541087 -0.47247505
		 -0.37500966 -0.16666633 0.49999991 -0.46339232 -0.16666633 0.49193764 -0.49999821 -0.16666633 0.47247401
		 0.37501341 -0.16666633 0.49999997 0.46339411 -0.16666633 0.49193808 0.49999958 -0.16666633 0.47247452
		 -0.37500966 0.16666695 0.49999991 -0.46339232 0.16666695 0.49193764 -0.49999821 0.16666695 0.47247401
		 0.37501341 0.16666695 0.49999997 0.46339411 0.16666695 0.49193808 0.49999958 0.16666695 0.47247452
		 -0.37501335 0.49999997 0.0078682955 -0.46339223 0.48843572 0.0078682955 -0.49999431 0.4605163 0.0078687668
		 0.37501913 0.49999997 0.0078687798 0.46339613 0.48843572 0.0078687798 0.50000185 0.4605163 0.0078687798
		 -0.37501335 0.49999997 -0.16666602 -0.46339229 0.48843572 -0.16666648 -0.49999616 0.4605163 -0.16666698
		 0.37501723 0.49999997 -0.16666697 0.46339422 0.48843572 -0.16666697 0.50000197 0.4605163 -0.16666698
		 -0.37501335 0.16666684 -0.49999994 -0.46339238 0.16666684 -0.49193805 -0.49999994 0.16666684 -0.47247458
		 0.37501353 0.16666684 -0.50000012 0.4633922 0.16666684 -0.49193767 0.50000757 0.16666684 -0.47247505
		 -0.37501335 -0.16666666 -0.49999994 -0.46339238 -0.16666666 -0.49193805 -0.49999994 -0.16666666 -0.47247458
		 0.37501353 -0.16666666 -0.50000012 0.4633922 -0.16666666 -0.49193767 0.50000757 -0.16666666 -0.47247505
		 -0.37501335 -0.39489466 -0.16666602 -0.46339038 -0.38333026 -0.16431138 -0.49999815 -0.35541087 -0.15862653
		 0.37501723 -0.39489466 -0.16666697 0.46339232 -0.38333026 -0.16431138 0.50000387 -0.35541099 -0.15862654
		 -0.37501335 -0.55481011 0.0078682955 -0.46339226 -0.54298043 0.0099344309 -0.49999994 -0.51442176 0.014922146
		 0.37501913 -0.55481011 0.0078687798 0.46339196 -0.54298043 0.0099344309 0.50000554 -0.51442176 0.014922155
		 -0.4481793 -0.4850015 0.48845145 0.44715101 -0.48465782 0.48845723 -0.44714552 0.48330435 0.48836133
		 0.44715315 0.48330435 0.48836133 -0.4471454 0.48330435 -0.48836088 0.44714934 0.48330435 -0.48836043
		 -0.4471454 -0.37819877 -0.48836088 0.44714934 -0.37819877 -0.48836043 -0.45552817 -0.15130991 -0.15627623
		 -0.41105843 -0.13595334 -0.14588547 -0.36659431 -0.120597 -0.13549519 -0.45552826 -0.15130991 -0.0025219829
		 -0.41105843 -0.13595334 -0.012912748 -0.36659244 -0.120597 -0.023303984 -0.45552826 0.15131041 -0.0025219829
		 -0.41105843 0.13595411 -0.012912748 -0.36659244 0.12059763 -0.023303984 -0.45552817 0.15131041 -0.15627623
		 -0.41105843 0.13595411 -0.14588547 -0.36659431 0.12059763 -0.13549519;
	setAttr -s 244 ".ed";
	setAttr ".ed[0:165]"  0 1 1 2 3 1 0 2 1 1 3 1 4 5 0 6 7 0 4 6 0 5 7 0 9 8 1
		 8 99 0 99 98 0 98 9 1 8 13 1 13 100 1 100 99 1 11 10 1 10 18 0 18 17 1 17 11 1 10 9 1
		 9 19 1 19 18 1 13 12 1 58 13 1 12 11 1 11 56 1 15 14 1 103 15 1 14 19 1 19 101 1
		 17 16 1 16 60 0 60 59 1 59 17 1 16 15 1 15 61 1 61 60 1 21 20 1 70 21 1 20 25 1 25 68 1
		 23 22 1 22 63 0 63 62 1 62 23 1 22 21 1 21 64 1 64 63 1 25 24 1 24 28 0 28 27 1 27 25 1
		 24 23 1 23 29 1 29 28 1 27 26 1 26 72 0 72 71 1 71 27 1 26 31 1 31 73 1 73 72 1 31 30 1
		 67 31 1 30 29 1 29 65 1 33 32 1 82 33 1 32 37 1 37 80 1 35 34 1 34 75 0 75 74 1 74 35 1
		 34 33 1 33 76 1 76 75 1 37 36 1 36 40 0 40 39 1 39 37 1 36 35 1 35 41 1 41 40 1 39 38 1
		 38 84 0 84 83 1 83 39 1 38 43 1 43 85 1 85 84 1 43 42 1 79 43 1 42 41 1 41 77 1 45 44 1
		 94 45 1 44 49 1 49 92 1 47 46 1 46 87 0 87 86 1 86 47 1 46 45 1 45 88 1 88 87 1 49 48 1
		 48 52 0 52 51 1 51 49 1 48 47 1 47 53 1 53 52 1 51 50 1 50 96 0 96 95 0 95 51 1 50 55 1
		 55 97 1 97 96 1 55 54 1 91 55 1 54 53 1 53 89 1 58 57 1 64 58 1 57 56 1 56 62 1 66 65 1
		 65 59 1 61 67 1 67 66 1 70 69 1 76 70 1 69 68 1 68 74 1 78 77 1 77 71 1 73 79 1 79 78 1
		 82 81 1 88 82 1 81 80 1 80 86 1 90 89 1 89 83 1 85 91 1 91 90 1 94 93 1 100 94 1
		 93 92 0 92 98 1 102 101 0 101 95 1 97 103 1 103 102 1 59 56 1 65 62 1 71 68 1 77 74 1
		 83 80 1 89 86 1 95 92 0 101 98 0 91 0 1 0 97 1;
	setAttr ".ed[166:243]" 1 103 1 1 61 1 85 2 1 3 67 1 79 2 1 73 3 1 94 4 1 4 88 1
		 100 5 1 58 5 1 6 82 1 64 7 1 6 76 1 7 70 1 12 57 0 60 66 0 57 63 0 30 66 0 20 69 0
		 72 78 0 69 75 0 42 78 0 32 81 0 84 90 0 81 87 0 54 90 0 44 93 0 96 102 0 93 99 0
		 14 102 0 8 104 0 104 12 0 10 104 0 14 105 0 105 18 0 16 105 0 20 106 0 106 24 0 22 106 0
		 26 107 0 107 30 0 28 107 0 32 108 0 108 36 0 34 108 0 38 109 0 109 42 0 40 109 0
		 44 110 0 110 48 0 46 110 0 50 111 0 111 54 0 52 111 0 4 112 0 112 113 0 113 114 0
		 5 115 0 115 116 0 116 117 0 112 115 1 113 116 1 114 117 0 7 118 0 118 119 0 119 120 0
		 115 118 1 116 119 1 117 120 0 6 121 0 121 122 0 122 123 0 121 118 1 122 119 1 123 120 0
		 112 121 1 113 122 1 114 123 0;
	setAttr -s 122 -ch 488 ".fc[0:121]" -type "polyFaces" 
		f 4 -1 2 1 -4
		f 4 228 234 -241 -244
		f 4 8 9 10 11
		f 4 12 13 14 -10
		f 4 15 16 17 18
		f 4 19 20 21 -17
		f 4 30 31 32 33
		f 4 34 35 36 -32
		f 4 41 42 43 44
		f 4 45 46 47 -43
		f 4 48 49 50 51
		f 4 52 53 54 -50
		f 4 55 56 57 58
		f 4 59 60 61 -57
		f 4 70 71 72 73
		f 4 74 75 76 -72
		f 4 77 78 79 80
		f 4 81 82 83 -79
		f 4 84 85 86 87
		f 4 88 89 90 -86
		f 4 99 100 101 102
		f 4 103 104 105 -101
		f 4 106 107 108 109
		f 4 110 111 112 -108
		f 4 113 114 115 116
		f 4 117 118 119 -115
		f 4 -19 -34 156 -26
		f 4 -157 -130 157 -128
		f 4 -158 -66 -54 -45
		f 4 -52 -59 158 -41
		f 4 -159 -138 159 -136
		f 4 -160 -95 -83 -74
		f 4 -81 -88 160 -70
		f 4 -161 -146 161 -144
		f 4 -162 -124 -112 -103
		f 4 -110 -117 162 -99
		f 4 -163 -154 163 -152
		f 4 -164 -30 -21 -12
		f 4 -119 -122 164 165
		f 4 -155 -166 0 166
		f 4 -28 -167 167 -36
		f 4 -165 -147 168 -3
		f 4 -168 3 169 -131
		f 4 -169 -90 -93 170
		f 4 -2 -171 -139 171
		f 4 -170 -172 -61 -64
		f 4 -97 172 173 -105
		f 4 -150 174 -5 -173
		f 4 -14 -24 175 -175
		f 4 -174 6 176 -142
		f 4 -176 -126 177 -8
		f 4 -177 178 -76 -68
		f 4 5 179 -134 -179
		f 4 -178 -47 -39 -180
		f 4 22 180 -125 23
		f 4 24 25 -127 -181
		f 4 -33 181 128 129
		f 4 -37 130 131 -182
		f 4 124 182 -48 125
		f 4 126 127 -44 -183
		f 4 62 183 -132 63
		f 4 64 65 -129 -184
		f 4 37 184 -133 38
		f 4 39 40 -135 -185
		f 4 -58 185 136 137
		f 4 -62 138 139 -186
		f 4 132 186 -77 133
		f 4 134 135 -73 -187
		f 4 91 187 -140 92
		f 4 93 94 -137 -188
		f 4 66 188 -141 67
		f 4 68 69 -143 -189
		f 4 -87 189 144 145
		f 4 -91 146 147 -190
		f 4 140 190 -106 141
		f 4 142 143 -102 -191
		f 4 120 191 -148 121
		f 4 122 123 -145 -192
		f 4 95 192 -149 96
		f 4 97 98 -151 -193
		f 4 -116 193 152 153
		f 4 -120 154 155 -194
		f 4 148 194 -15 149
		f 4 150 151 -11 -195
		f 4 26 195 -156 27
		f 4 28 29 -153 -196
		f 4 -23 -13 196 197
		f 4 -9 -20 198 -197
		f 4 -16 -25 -198 -199
		f 4 -22 -29 199 200
		f 4 -27 -35 201 -200
		f 4 -31 -18 -201 -202
		f 4 -49 -40 202 203
		f 4 -38 -46 204 -203
		f 4 -42 -53 -204 -205
		f 4 -63 -60 205 206
		f 4 -56 -51 207 -206
		f 4 -55 -65 -207 -208
		f 4 -78 -69 208 209
		f 4 -67 -75 210 -209
		f 4 -71 -82 -210 -211
		f 4 -92 -89 211 212
		f 4 -85 -80 213 -212
		f 4 -84 -94 -213 -214
		f 4 -107 -98 214 215
		f 4 -96 -104 216 -215
		f 4 -100 -111 -216 -217
		f 4 -121 -118 217 218
		f 4 -114 -109 219 -218
		f 4 -113 -123 -219 -220
		f 4 4 223 -227 -221
		f 4 226 224 -228 -222
		f 4 227 225 -229 -223
		f 4 7 229 -233 -224
		f 4 232 230 -234 -225
		f 4 233 231 -235 -226
		f 4 -6 235 238 -230
		f 4 -239 236 239 -231
		f 4 -240 237 240 -232
		f 4 -7 220 241 -236
		f 4 -242 221 242 -237
		f 4 -243 222 243 -238;
	setAttr ".cd" -type "dataPolyComponent" Index_Data Edge 0 ;
	setAttr ".cvd" -type "dataPolyComponent" Index_Data Vertex 0 ;
	setAttr ".pd[0]" -type "dataPolyComponent" Index_Data UV 0 ;
	setAttr ".hfd" -type "dataPolyComponent" Index_Data Face 0 ;
	setAttr ".ai_translator" -type "string" "polymesh";
createNode lightLinker -s -n "lightLinker1";
	rename -uid "9F008799-4079-507D-E919-62A8D034A0B6";
	setAttr -s 3 ".lnk";
	setAttr -s 3 ".slnk";
createNode shapeEditorManager -n "shapeEditorManager";
	rename -uid "2475F761-453E-2924-1240-26B0B096CA4F";
createNode poseInterpolatorManager -n "poseInterpolatorManager";
	rename -uid "7DC5FC00-4BE4-A128-7672-D99DDD52C99A";
createNode displayLayerManager -n "layerManager";
	rename -uid "3BFEEAF7-420A-C645-D0C1-148A93F1656B";
createNode displayLayer -n "defaultLayer";
	rename -uid "487FB8B4-4564-A110-30C8-B8A2A8226DD6";
createNode renderLayerManager -n "renderLayerManager";
	rename -uid "53CF5E54-4B05-277D-D9FF-FBB0A621B709";
createNode renderLayer -n "defaultRenderLayer";
	rename -uid "E073A4DF-41B7-15C6-98D8-F0BA943A8310";
	setAttr ".g" yes;
createNode script -n "sceneConfigurationScriptNode";
	rename -uid "5DE50467-4287-5147-E5C9-9BAB32A057F0";
	setAttr ".b" -type "string" "playbackOptions -min 1 -max 120 -ast 1 -aet 200 ";
	setAttr ".st" 6;
createNode aiOptions -s -n "defaultArnoldRenderOptions";
	rename -uid "945DFD14-4154-E209-BF2E-7BB241D594B1";
	addAttr -ci true -sn "ARV_options" -ln "ARV_options" -dt "string";
	setAttr ".version" -type "string" "4.0.3";
	setAttr ".logv" 3;
createNode aiAOVFilter -s -n "defaultArnoldFilter";
	rename -uid "76932DC8-4FF8-B2C8-6B30-28B18C9FF713";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVDriver -s -n "defaultArnoldDriver";
	rename -uid "D69016AE-4544-2A9E-4D91-E6A44F2E52CD";
	setAttr ".color_management" 1;
	setAttr ".tiff_compression" 0;
	setAttr ".ai_translator" -type "string" "tif";
createNode aiAOVDriver -s -n "defaultArnoldDisplayDriver";
	rename -uid "15CB16B3-4C1F-44C7-AA5E-4AB565687774";
	setAttr ".output_mode" 0;
	setAttr ".ai_translator" -type "string" "maya";
createNode aiStandardSurface -n "TestMaterial_";
	rename -uid "C33D9E8C-4D2E-3365-A419-96A606D16C98";
	setAttr ".n" -type "float3" 0 0 1 ;
	setAttr ".specular" 0.25;
	setAttr ".specular_color" -type "float3" 0 0 0 ;
	setAttr ".specular_roughness" 0;
	setAttr ".transmission_color" -type "float3" 0 0 0 ;
	setAttr ".emission_color" -type "float3" 0 0 0 ;
createNode projection -n "tx_TestMaterial_projection_color1";
	rename -uid "1F644579-4B5A-AF78-FCAE-EE8EF38967EC";
	setAttr ".t" 6;
	setAttr ".vt1" -type "float2" 0.5 0.5 ;
	setAttr ".vt2" -type "float2" 0.5 0.5 ;
	setAttr ".vt3" -type "float2" 0.5 0.5 ;
	setAttr ".ai_use_reference_object" no;
createNode file -n "tx_TestMaterial_file_color1";
	rename -uid "4480ED46-4D94-7A9C-53FC-4393DF606D86";
	setAttr ".ftn" -type "string" "test_color.tif";
	setAttr ".cs" -type "string" "sRGB";
createNode place2dTexture -n "tx_TestMaterial_place2dTexture1";
	rename -uid "642ABE85-4D41-EE8F-040A-8587205F20EF";
	setAttr ".re" -type "float2" 1.75 1.75 ;
createNode ramp -n "tx_TestMaterial_ramp_Blend1";
	rename -uid "03DB3EB0-42EF-A4AC-A828-B597D0146DFC";
	setAttr -s 2 ".cel";
	setAttr ".cel[0].ep" 0;
	setAttr ".cel[0].ec" -type "float3" 0 0 0 ;
	setAttr ".cel[1].ep" 1;
	setAttr ".cel[1].ec" -type "float3" 1 1 1 ;
createNode samplerInfo -n "tx_TestMaterial_samplerInfo1";
	rename -uid "338253DF-478D-2D8D-6AB4-788307107542";
createNode gammaCorrect -n "tx_TestMaterial_gammaCorrect1";
	rename -uid "344EAA09-4436-122C-218A-43A6C315DC62";
	setAttr ".g" -type "float3" 1.8 1.8 1.8 ;
createNode shadingEngine -n "TX_TestMaterial_SG";
	rename -uid "24535919-4AEB-7F63-A5B8-D6B2D2928310";
	setAttr ".ihi" 0;
	setAttr ".ro" yes;
createNode materialInfo -n "materialInfo4";
	rename -uid "C760FB4E-4BA5-CFAB-5FBC-7597B5F57FB6";
createNode layeredTexture -n "tx_TestMaterial_layeredTexture1";
	rename -uid "55DC4284-444D-EB35-1204-B4BD2ED5BC0F";
	setAttr -s 2 ".cs";
	setAttr ".cs[0].bm" 1;
	setAttr ".cs[0].iv" yes;
	setAttr ".cs[1].a" 1;
	setAttr ".cs[1].bm" 0;
	setAttr ".cs[1].iv" yes;
createNode nodeGraphEditorInfo -n "hyperShadePrimaryNodeEditorSavedTabsInfo";
	rename -uid "92014C80-0001-CD34-6046-50D600000397";
	setAttr ".tgi[0].tn" -type "string" "Untitled_1";
	setAttr ".tgi[0].vl" -type "double2" 1892.2337732487274 -82.336624156420328 ;
	setAttr ".tgi[0].vh" -type "double2" 3632.7688330240558 1810.8199800395057 ;
	setAttr -s 10 ".tgi[0].ni";
	setAttr ".tgi[0].ni[0].x" 2214.28564453125;
	setAttr ".tgi[0].ni[0].y" 1244.2857666015625;
	setAttr ".tgi[0].ni[0].nvs" 1923;
	setAttr ".tgi[0].ni[1].x" 2904.28564453125;
	setAttr ".tgi[0].ni[1].y" 1090;
	setAttr ".tgi[0].ni[1].nvs" 2387;
	setAttr ".tgi[0].ni[2].x" 1148.889892578125;
	setAttr ".tgi[0].ni[2].y" 589.1990966796875;
	setAttr ".tgi[0].ni[2].nvs" 1923;
	setAttr ".tgi[0].ni[3].x" 1906.151123046875;
	setAttr ".tgi[0].ni[3].y" 979.9544677734375;
	setAttr ".tgi[0].ni[3].nvs" 1923;
	setAttr ".tgi[0].ni[4].x" 2214.28564453125;
	setAttr ".tgi[0].ni[4].y" 1068.5714111328125;
	setAttr ".tgi[0].ni[4].nvs" 1923;
	setAttr ".tgi[0].ni[5].x" 2667.75244140625;
	setAttr ".tgi[0].ni[5].y" 1334.9500732421875;
	setAttr ".tgi[0].ni[5].nvs" 1923;
	setAttr ".tgi[0].ni[6].x" 1535.7142333984375;
	setAttr ".tgi[0].ni[6].y" 1084.2857666015625;
	setAttr ".tgi[0].ni[6].nvs" 1923;
	setAttr ".tgi[0].ni[7].x" 1525.963623046875;
	setAttr ".tgi[0].ni[7].y" 858.90814208984375;
	setAttr ".tgi[0].ni[7].nvs" 1923;
	setAttr ".tgi[0].ni[8].x" 3252.857177734375;
	setAttr ".tgi[0].ni[8].y" 1090;
	setAttr ".tgi[0].ni[8].nvs" 1923;
	setAttr ".tgi[0].ni[9].x" 1870;
	setAttr ".tgi[0].ni[9].y" 1370;
	setAttr ".tgi[0].ni[9].nvs" 1923;
select -ne :time1;
	setAttr ".o" 1;
	setAttr ".unw" 1;
select -ne :hardwareRenderingGlobals;
	setAttr ".otfna" -type "stringArray" 22 "NURBS Curves" "NURBS Surfaces" "Polygons" "Subdiv Surface" "Particles" "Particle Instance" "Fluids" "Strokes" "Image Planes" "UI" "Lights" "Cameras" "Locators" "Joints" "IK Handles" "Deformers" "Motion Trails" "Components" "Hair Systems" "Follicles" "Misc. UI" "Ornaments"  ;
	setAttr ".otfva" -type "Int32Array" 22 0 1 1 1 1 1
		 1 1 1 0 0 0 0 0 0 0 0 0
		 0 0 0 0 ;
	setAttr ".fprt" yes;
select -ne :renderPartition;
	setAttr -s 3 ".st";
select -ne :renderGlobalsList1;
select -ne :defaultShaderList1;
	setAttr -s 6 ".s";
select -ne :postProcessList1;
	setAttr -s 2 ".p";
select -ne :defaultRenderUtilityList1;
	setAttr -s 6 ".u";
select -ne :defaultRenderingList1;
select -ne :lightList1;
select -ne :defaultTextureList1;
	setAttr -s 2 ".tx";
select -ne :initialShadingGroup;
	setAttr ".ro" yes;
select -ne :initialParticleSE;
	setAttr ".ro" yes;
select -ne :defaultRenderGlobals;
	addAttr -ci true -h true -sn "dss" -ln "defaultSurfaceShader" -dt "string";
	setAttr ".ren" -type "string" "arnold";
	setAttr ".outf" 51;
	setAttr ".imfkey" -type "string" "tif";
	setAttr ".ifp" -type "string" "testrender";
	setAttr ".dss" -type "string" "lambert1";
select -ne :defaultResolution;
	setAttr ".pa" 1;
select -ne :defaultLightSet;
select -ne :hardwareRenderGlobals;
	setAttr ".ctrs" 256;
	setAttr ".btrs" 512;
relationship "link" ":lightLinker1" ":initialShadingGroup.message" ":defaultLightSet.message";
relationship "link" ":lightLinker1" ":initialParticleSE.message" ":defaultLightSet.message";
relationship "link" ":lightLinker1" "TX_TestMaterial_SG.message" ":defaultLightSet.message";
relationship "shadowLink" ":lightLinker1" ":initialShadingGroup.message" ":defaultLightSet.message";
relationship "shadowLink" ":lightLinker1" ":initialParticleSE.message" ":defaultLightSet.message";
relationship "shadowLink" ":lightLinker1" "TX_TestMaterial_SG.message" ":defaultLightSet.message";
connectAttr "layerManager.dli[0]" "defaultLayer.id";
connectAttr "renderLayerManager.rlmi[0]" "defaultRenderLayer.rlid";
connectAttr ":defaultArnoldDisplayDriver.msg" ":defaultArnoldRenderOptions.drivers"
		 -na;
connectAttr ":defaultArnoldFilter.msg" ":defaultArnoldRenderOptions.filt";
connectAttr ":defaultArnoldDriver.msg" ":defaultArnoldRenderOptions.drvr";
connectAttr "tx_TestMaterial_projection_color1.oc" "TestMaterial_.base_color";
connectAttr "tx_TestMaterial_file_color1.oc" "tx_TestMaterial_projection_color1.im"
		;
connectAttr "tx_TestMaterial_place3dTexture_.wim" "tx_TestMaterial_projection_color1.pm"
		;
connectAttr ":defaultColorMgtGlobals.cme" "tx_TestMaterial_file_color1.cme";
connectAttr ":defaultColorMgtGlobals.cfe" "tx_TestMaterial_file_color1.cmcf";
connectAttr ":defaultColorMgtGlobals.cfp" "tx_TestMaterial_file_color1.cmcp";
connectAttr ":defaultColorMgtGlobals.wsn" "tx_TestMaterial_file_color1.ws";
connectAttr "tx_TestMaterial_place2dTexture1.o" "tx_TestMaterial_file_color1.uv"
		;
connectAttr "tx_TestMaterial_samplerInfo1.fr" "tx_TestMaterial_ramp_Blend1.v";
connectAttr "tx_TestMaterial_projection_color1.oc" "tx_TestMaterial_gammaCorrect1.v"
		;
connectAttr "TestMaterial_.out" "TX_TestMaterial_SG.ss";
connectAttr "pCubeShape1.iog" "TX_TestMaterial_SG.dsm" -na;
connectAttr "TX_TestMaterial_SG.msg" "materialInfo4.sg";
connectAttr "TestMaterial_.msg" "materialInfo4.m";
connectAttr "TestMaterial_.msg" "materialInfo4.t" -na;
connectAttr "tx_TestMaterial_projection_color1.oc" "tx_TestMaterial_layeredTexture1.cs[0].c"
		;
connectAttr "tx_TestMaterial_ramp_Blend1.oa" "tx_TestMaterial_layeredTexture1.cs[0].a"
		;
connectAttr "tx_TestMaterial_gammaCorrect1.o" "tx_TestMaterial_layeredTexture1.cs[1].c"
		;
connectAttr "tx_TestMaterial_ramp_Blend1.msg" "hyperShadePrimaryNodeEditorSavedTabsInfo.tgi[0].ni[0].dn"
		;
connectAttr "TestMaterial_.msg" "hyperShadePrimaryNodeEditorSavedTabsInfo.tgi[0].ni[1].dn"
		;
connectAttr "tx_TestMaterial_place2dTexture1.msg" "hyperShadePrimaryNodeEditorSavedTabsInfo.tgi[0].ni[2].dn"
		;
connectAttr "tx_TestMaterial_projection_color1.msg" "hyperShadePrimaryNodeEditorSavedTabsInfo.tgi[0].ni[3].dn"
		;
connectAttr "tx_TestMaterial_gammaCorrect1.msg" "hyperShadePrimaryNodeEditorSavedTabsInfo.tgi[0].ni[4].dn"
		;
connectAttr "tx_TestMaterial_layeredTexture1.msg" "hyperShadePrimaryNodeEditorSavedTabsInfo.tgi[0].ni[5].dn"
		;
connectAttr "tx_TestMaterial_file_color1.msg" "hyperShadePrimaryNodeEditorSavedTabsInfo.tgi[0].ni[6].dn"
		;
connectAttr "tx_TestMaterial_place3dTexture_.msg" "hyperShadePrimaryNodeEditorSavedTabsInfo.tgi[0].ni[7].dn"
		;
connectAttr "TX_TestMaterial_SG.msg" "hyperShadePrimaryNodeEditorSavedTabsInfo.tgi[0].ni[8].dn"
		;
connectAttr "tx_TestMaterial_samplerInfo1.msg" "hyperShadePrimaryNodeEditorSavedTabsInfo.tgi[0].ni[9].dn"
		;
connectAttr "TX_TestMaterial_SG.pa" ":renderPartition.st" -na;
connectAttr "TestMaterial_.msg" ":defaultShaderList1.s" -na;
connectAttr "tx_TestMaterial_place3dTexture_.msg" ":defaultRenderUtilityList1.u"
		 -na;
connectAttr "tx_TestMaterial_layeredTexture1.msg" ":defaultRenderUtilityList1.u"
		 -na;
connectAttr "tx_TestMaterial_projection_color1.msg" ":defaultRenderUtilityList1.u"
		 -na;
connectAttr "tx_TestMaterial_place2dTexture1.msg" ":defaultRenderUtilityList1.u"
		 -na;
connectAttr "tx_TestMaterial_samplerInfo1.msg" ":defaultRenderUtilityList1.u" -na
		;
connectAttr "tx_TestMaterial_gammaCorrect1.msg" ":defaultRenderUtilityList1.u" -na
		;
connectAttr "defaultRenderLayer.msg" ":defaultRenderingList1.r" -na;
connectAttr "aiSkyDomeLightShape1.ltd" ":lightList1.l" -na;
connectAttr "tx_TestMaterial_file_color1.msg" ":defaultTextureList1.tx" -na;
connectAttr "tx_TestMaterial_ramp_Blend1.msg" ":defaultTextureList1.tx" -na;
connectAttr "aiSkyDomeLight1.iog" ":defaultLightSet.dsm" -na;
// End of test.ma

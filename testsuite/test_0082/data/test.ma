//Maya ASCII 2013 scene
//Name: test.ma
//Last modified: Mon, Jun 18, 2012 01:47:30 PM
//Codeset: UTF-8
requires maya "2013";
requires "mtoa" "0.16.0";
requires "Mayatomr" "2013.0 - 3.10.1.4 ";
requires "stereoCamera" "10.0";
currentUnit -l centimeter -a degree -t film;
fileInfo "application" "maya";
fileInfo "product" "Maya 2013";
fileInfo "version" "2013 x64";
fileInfo "cutIdentifier" "201202220216-825135";
fileInfo "osv" "Linux 2.6.32-220.el6.x86_64 #1 SMP Tue Dec 6 19:48:22 GMT 2011 x86_64";
createNode transform -s -n "persp";
	setAttr ".v" no;
	setAttr ".t" -type "double3" 27.795094234407376 18.971162668046485 0 ;
	setAttr ".r" -type "double3" -24.000000000000004 90 0 ;
	setAttr ".rp" -type "double3" -3.5804692544161291e-15 -7.1054273576010019e-15 -1.4210854715202004e-14 ;
	setAttr ".rpt" -type "double3" -8.7783779362275568e-15 1.1498476670238775e-14 9.4020555375448207e-16 ;
createNode camera -s -n "perspShape" -p "persp";
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
	setAttr ".v" no;
	setAttr ".t" -type "double3" 0 100.1 0 ;
	setAttr ".r" -type "double3" -89.999999999999986 0 0 ;
createNode camera -s -n "topShape" -p "top";
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
	setAttr ".v" no;
	setAttr ".t" -type "double3" 0 0 100.1 ;
createNode camera -s -n "frontShape" -p "front";
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
	setAttr ".v" no;
	setAttr ".t" -type "double3" 100.1 1.6871751536864319 12.910557697774426 ;
	setAttr ".r" -type "double3" 0 89.999999999999986 0 ;
createNode camera -s -n "sideShape" -p "side";
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
createNode transform -n "transform1";
createNode transform -n "transform2";
createNode transform -n "transform3";
createNode transform -n "transform4";
createNode transform -n "transform5";
createNode transform -n "transform6";
createNode transform -n "transform7";
createNode transform -n "transform8";
createNode transform -n "transform9";
createNode transform -n "transform10";
createNode transform -n "transform11";
createNode transform -n "transform12";
createNode transform -n "transform13";
createNode transform -n "transform14";
createNode transform -n "transform15";
createNode transform -n "transform16";
createNode transform -n "transform17";
createNode transform -n "transform18";
createNode transform -n "transform19";
createNode transform -n "transform20";
createNode transform -n "transform21";
createNode transform -n "transform22";
createNode transform -n "transform23";
createNode transform -n "transform24";
createNode transform -n "transform25";
createNode transform -n "transform26";
createNode transform -n "transform27";
createNode transform -n "transform28";
createNode transform -n "transform29";
createNode transform -n "transform30";
createNode transform -n "transform31";
createNode transform -n "transform32";
createNode transform -n "transform33";
createNode transform -n "transform34";
createNode transform -n "transform35";
createNode transform -n "transform36";
createNode transform -n "transform37";
createNode transform -n "transform38";
createNode transform -n "transform39";
createNode transform -n "transform40";
createNode transform -n "transform41";
createNode transform -n "transform42";
createNode transform -n "transform43";
createNode transform -n "transform44";
createNode transform -n "transform45";
createNode transform -n "transform46";
createNode transform -n "transform47";
createNode transform -n "transform48";
createNode transform -n "transform49";
createNode transform -n "transform50";
createNode transform -n "transform51";
createNode transform -n "transform52";
createNode transform -n "transform53";
createNode transform -n "transform54";
createNode transform -n "transform55";
createNode transform -n "transform56";
createNode transform -n "transform57";
createNode transform -n "transform58";
createNode transform -n "pSphere1";
	setAttr ".t" -type "double3" -0.020665766309299727 6.3190084466120737 0.27269214094604877 ;
createNode mesh -n "pSphereShape1" -p "pSphere1";
	setAttr -k off ".v";
	setAttr ".vir" yes;
	setAttr ".vif" yes;
	setAttr ".uvst[0].uvsn" -type "string" "map1";
	setAttr ".cuvs" -type "string" "map1";
	setAttr ".dcc" -type "string" "Ambient+Diffuse";
	setAttr ".covm[0]"  0 1 1;
	setAttr ".cdvm[0]"  0 1 1;
createNode transform -n "transform59";
createNode transform -n "transform60";
createNode transform -n "transform61";
createNode transform -n "transform62";
createNode transform -n "transform63";
createNode transform -n "transform64";
createNode transform -n "transform65";
createNode transform -n "transform66";
createNode transform -n "transform67";
createNode transform -n "transform68";
createNode transform -n "transform69";
createNode transform -n "transform70";
createNode transform -n "transform71";
createNode transform -n "transform72";
createNode transform -n "transform73";
createNode transform -n "transform74";
createNode transform -n "transform75";
createNode transform -n "transform76";
createNode transform -n "transform77";
createNode transform -n "transform78";
createNode transform -n "transform79";
createNode transform -n "transform80";
createNode transform -n "transform81";
createNode transform -n "transform82";
createNode transform -n "transform83";
createNode transform -n "transform84";
createNode transform -n "transform85";
createNode transform -n "transform86";
createNode transform -n "transform87";
createNode transform -n "transform88";
createNode transform -n "transform89";
createNode transform -n "transform90";
createNode transform -n "transform91";
createNode transform -n "transform92";
createNode transform -n "transform93";
createNode transform -n "transform94";
createNode transform -n "transform95";
createNode transform -n "transform96";
createNode transform -n "transform97";
createNode transform -n "transform98";
createNode transform -n "transform99";
createNode transform -n "transform100";
createNode transform -n "transform101";
createNode transform -n "transform102";
createNode transform -n "transform103";
createNode transform -n "transform104";
createNode transform -n "transform105";
createNode transform -n "transform106";
createNode transform -n "transform107";
createNode transform -n "transform108";
createNode transform -n "transform109";
createNode transform -n "transform110";
createNode transform -n "transform111";
createNode transform -n "transform112";
createNode transform -n "transform113";
createNode transform -n "transform114";
createNode transform -n "transform115";
createNode transform -n "transform116";
createNode transform -n "transform117";
createNode transform -n "transform118";
createNode transform -n "transform119";
createNode transform -n "transform120";
createNode transform -n "transform121";
createNode transform -n "transform122";
createNode transform -n "transform123";
createNode transform -n "transform124";
createNode transform -n "transform125";
createNode transform -n "transform126";
createNode transform -n "transform127";
createNode transform -n "transform128";
createNode transform -n "transform129";
createNode transform -n "transform130";
createNode transform -n "transform131";
createNode transform -n "transform132";
createNode transform -n "transform133";
createNode transform -n "transform134";
createNode transform -n "transform135";
createNode transform -n "transform136";
createNode transform -n "transform137";
createNode transform -n "transform138";
createNode transform -n "transform139";
createNode transform -n "transform140";
createNode transform -n "transform141";
createNode transform -n "transform142";
createNode transform -n "transform143";
createNode transform -n "transform144";
createNode transform -n "transform145";
createNode transform -n "transform146";
createNode transform -n "transform147";
createNode transform -n "transform148";
createNode transform -n "transform149";
createNode transform -n "transform150";
createNode transform -n "transform151";
createNode transform -n "transform152";
createNode transform -n "transform153";
createNode transform -n "transform154";
createNode transform -n "transform155";
createNode transform -n "transform156";
createNode transform -n "transform157";
createNode transform -n "transform158";
createNode transform -n "transform159";
createNode transform -n "transform160";
createNode transform -n "transform161";
createNode transform -n "transform162";
createNode transform -n "transform163";
createNode transform -n "transform164";
createNode transform -n "transform165";
createNode transform -n "transform166";
createNode transform -n "transform167";
createNode transform -n "transform168";
createNode transform -n "transform169";
createNode transform -n "transform170";
createNode transform -n "transform171";
createNode transform -n "transform172";
createNode transform -n "transform173";
createNode transform -n "transform174";
createNode transform -n "transform175";
createNode transform -n "transform176";
createNode transform -n "transform177";
createNode transform -n "transform178";
createNode transform -n "transform179";
createNode transform -n "transform180";
createNode transform -n "transform181";
createNode transform -n "transform182";
createNode transform -n "transform183";
createNode transform -n "transform184";
createNode transform -n "transform185";
createNode transform -n "transform186";
createNode transform -n "transform187";
createNode transform -n "transform188";
createNode transform -n "transform189";
createNode transform -n "transform190";
createNode transform -n "transform191";
createNode transform -n "transform192";
createNode transform -n "transform193";
createNode transform -n "transform194";
createNode transform -n "transform195";
createNode transform -n "transform196";
createNode transform -n "transform197";
createNode transform -n "transform198";
createNode transform -n "transform199";
createNode transform -n "transform200";
createNode transform -n "transform201";
createNode transform -n "transform202";
createNode transform -n "transform203";
createNode transform -n "transform204";
createNode transform -n "transform205";
createNode transform -n "transform206";
createNode transform -n "transform207";
createNode transform -n "transform208";
createNode transform -n "transform209";
createNode transform -n "transform210";
createNode transform -n "transform211";
createNode transform -n "transform212";
createNode transform -n "transform213";
createNode transform -n "transform214";
createNode transform -n "transform215";
createNode transform -n "transform216";
createNode transform -n "transform217";
createNode transform -n "transform218";
createNode transform -n "transform219";
createNode transform -n "transform220";
createNode transform -n "transform221";
createNode transform -n "transform222";
createNode transform -n "transform223";
createNode transform -n "transform224";
createNode transform -n "transform225";
createNode transform -n "transform226";
createNode transform -n "transform227";
createNode transform -n "transform228";
createNode transform -n "transform229";
createNode transform -n "transform230";
createNode transform -n "transform231";
createNode transform -n "transform232";
createNode transform -n "transform233";
createNode transform -n "transform234";
createNode transform -n "transform235";
createNode transform -n "transform236";
createNode transform -n "transform237";
createNode transform -n "transform238";
createNode transform -n "transform239";
createNode transform -n "transform240";
createNode transform -n "transform241";
createNode transform -n "transform242";
createNode transform -n "transform243";
createNode transform -n "transform244";
createNode transform -n "transform245";
createNode transform -n "transform246";
createNode transform -n "transform247";
createNode transform -n "transform248";
createNode transform -n "transform249";
createNode transform -n "transform250";
createNode transform -n "transform251";
createNode transform -n "transform252";
createNode transform -n "transform253";
createNode transform -n "transform254";
createNode transform -n "transform255";
createNode transform -n "transform256";
createNode transform -n "transform257";
createNode transform -n "transform258";
createNode transform -n "transform259";
createNode transform -n "transform260";
createNode transform -n "transform261";
createNode transform -n "transform262";
createNode transform -n "transform263";
createNode transform -n "transform264";
createNode transform -n "transform265";
createNode transform -n "transform266";
createNode transform -n "transform267";
createNode transform -n "transform268";
createNode transform -n "transform269";
createNode transform -n "transform270";
createNode transform -n "transform271";
createNode transform -n "transform272";
createNode transform -n "transform273";
createNode transform -n "transform274";
createNode transform -n "transform275";
createNode transform -n "transform276";
createNode transform -n "transform277";
createNode transform -n "transform278";
createNode transform -n "transform279";
createNode transform -n "transform280";
createNode transform -n "transform281";
createNode transform -n "transform282";
createNode transform -n "transform283";
createNode transform -n "transform284";
createNode transform -n "transform285";
createNode transform -n "transform286";
createNode transform -n "transform287";
createNode transform -n "transform288";
createNode transform -n "transform289";
createNode transform -n "transform290";
createNode transform -n "transform291";
createNode transform -n "transform292";
createNode transform -n "transform293";
createNode transform -n "transform294";
createNode transform -n "transform295";
createNode transform -n "transform296";
createNode transform -n "transform297";
createNode transform -n "transform298";
createNode transform -n "transform299";
createNode transform -n "transform300";
createNode transform -n "transform301";
createNode transform -n "transform302";
createNode transform -n "transform303";
createNode transform -n "transform304";
createNode transform -n "transform305";
createNode transform -n "transform306";
createNode transform -n "transform307";
createNode transform -n "transform308";
createNode transform -n "transform309";
createNode transform -n "transform310";
createNode transform -n "transform311";
createNode transform -n "transform312";
createNode transform -n "transform313";
createNode transform -n "transform314";
createNode transform -n "transform315";
createNode transform -n "transform316";
createNode transform -n "transform317";
createNode transform -n "transform318";
createNode transform -n "transform319";
createNode transform -n "transform320";
createNode transform -n "transform321";
createNode transform -n "transform322";
createNode transform -n "transform323";
createNode transform -n "transform324";
createNode transform -n "transform325";
createNode transform -n "transform326";
createNode transform -n "transform327";
createNode transform -n "transform328";
createNode transform -n "transform329";
createNode transform -n "transform330";
createNode transform -n "transform331";
createNode transform -n "transform332";
createNode transform -n "transform333";
createNode transform -n "transform334";
createNode transform -n "transform335";
createNode transform -n "transform336";
createNode transform -n "transform337";
createNode transform -n "transform338";
createNode transform -n "transform339";
createNode transform -n "transform340";
createNode transform -n "transform341";
createNode transform -n "transform342";
createNode transform -n "transform343";
createNode transform -n "transform344";
createNode transform -n "transform345";
createNode transform -n "transform346";
createNode transform -n "transform347";
createNode transform -n "transform348";
createNode transform -n "transform349";
createNode transform -n "transform350";
createNode transform -n "transform351";
createNode transform -n "transform352";
createNode transform -n "transform353";
createNode transform -n "transform354";
createNode transform -n "transform355";
createNode transform -n "transform356";
createNode transform -n "transform357";
createNode transform -n "transform358";
createNode transform -n "transform359";
createNode transform -n "transform360";
createNode transform -n "transform361";
createNode transform -n "transform362";
createNode transform -n "transform363";
createNode transform -n "transform364";
createNode transform -n "transform365";
createNode transform -n "transform366";
createNode transform -n "transform367";
createNode transform -n "transform368";
createNode transform -n "transform369";
createNode transform -n "transform370";
createNode transform -n "transform371";
createNode transform -n "transform372";
createNode transform -n "transform373";
createNode transform -n "transform374";
createNode transform -n "transform375";
createNode transform -n "transform376";
createNode transform -n "transform377";
createNode transform -n "transform378";
createNode transform -n "transform379";
createNode transform -n "transform380";
createNode transform -n "transform381";
createNode transform -n "transform382";
createNode transform -n "transform383";
createNode transform -n "transform384";
createNode transform -n "transform385";
createNode transform -n "transform386";
createNode transform -n "transform387";
createNode transform -n "transform388";
createNode transform -n "transform389";
createNode transform -n "transform390";
createNode transform -n "transform391";
createNode transform -n "transform392";
createNode transform -n "transform393";
createNode transform -n "transform394";
createNode transform -n "transform395";
createNode transform -n "transform396";
createNode transform -n "transform397";
createNode transform -n "transform398";
createNode transform -n "transform399";
createNode transform -n "transform400";
createNode transform -n "transform401";
createNode transform -n "transform402";
createNode transform -n "transform403";
createNode transform -n "transform404";
createNode transform -n "transform405";
createNode transform -n "transform406";
createNode transform -n "transform407";
createNode transform -n "transform408";
createNode transform -n "transform409";
createNode transform -n "transform410";
createNode transform -n "transform411";
createNode transform -n "transform412";
createNode transform -n "transform413";
createNode transform -n "transform414";
createNode transform -n "transform415";
createNode transform -n "transform416";
createNode transform -n "transform417";
createNode transform -n "transform418";
createNode transform -n "transform419";
createNode transform -n "transform420";
createNode transform -n "transform421";
createNode transform -n "transform422";
createNode transform -n "transform423";
createNode transform -n "transform424";
createNode transform -n "transform425";
createNode transform -n "transform426";
createNode transform -n "transform427";
createNode transform -n "transform428";
createNode transform -n "transform429";
createNode transform -n "transform430";
createNode transform -n "transform431";
createNode transform -n "transform432";
createNode transform -n "transform433";
createNode transform -n "transform434";
createNode transform -n "transform435";
createNode transform -n "transform436";
createNode transform -n "transform437";
createNode transform -n "transform438";
createNode transform -n "transform439";
createNode transform -n "transform440";
createNode transform -n "transform441";
createNode transform -n "transform442";
createNode transform -n "transform443";
createNode transform -n "transform444";
createNode transform -n "transform445";
createNode transform -n "transform446";
createNode transform -n "transform447";
createNode transform -n "transform448";
createNode transform -n "transform449";
createNode transform -n "transform450";
createNode transform -n "transform451";
createNode transform -n "transform452";
createNode transform -n "transform453";
createNode transform -n "transform454";
createNode transform -n "transform455";
createNode transform -n "transform456";
createNode transform -n "transform457";
createNode transform -n "transform458";
createNode transform -n "transform459";
createNode transform -n "transform460";
createNode transform -n "transform461";
createNode transform -n "transform462";
createNode transform -n "transform463";
createNode transform -n "transform464";
createNode transform -n "transform465";
createNode transform -n "transform466";
createNode transform -n "transform467";
createNode transform -n "transform468";
createNode transform -n "transform469";
createNode transform -n "transform470";
createNode transform -n "transform471";
createNode transform -n "transform472";
createNode transform -n "transform473";
createNode transform -n "transform474";
createNode transform -n "transform475";
createNode transform -n "transform476";
createNode transform -n "transform477";
createNode transform -n "transform478";
createNode transform -n "transform479";
createNode transform -n "transform480";
createNode transform -n "transform481";
createNode transform -n "transform482";
createNode transform -n "transform483";
createNode transform -n "transform484";
createNode transform -n "transform485";
createNode transform -n "transform486";
createNode transform -n "transform487";
createNode transform -n "transform488";
createNode transform -n "transform489";
createNode transform -n "transform490";
createNode transform -n "transform491";
createNode transform -n "transform492";
createNode transform -n "transform493";
createNode transform -n "transform494";
createNode transform -n "transform495";
createNode transform -n "transform496";
createNode transform -n "transform497";
createNode transform -n "transform498";
createNode transform -n "transform499";
createNode transform -n "transform500";
createNode transform -n "transform501";
createNode transform -n "transform502";
createNode transform -n "transform503";
createNode transform -n "transform504";
createNode transform -n "transform505";
createNode transform -n "transform506";
createNode transform -n "transform507";
createNode transform -n "transform508";
createNode transform -n "transform509";
createNode transform -n "transform510";
createNode transform -n "transform511";
createNode transform -n "transform512";
createNode transform -n "transform513";
createNode transform -n "transform514";
createNode transform -n "transform515";
createNode transform -n "transform516";
createNode transform -n "transform517";
createNode transform -n "transform518";
createNode transform -n "transform519";
createNode transform -n "transform520";
createNode transform -n "transform521";
createNode transform -n "transform522";
createNode transform -n "transform523";
createNode transform -n "transform524";
createNode transform -n "transform525";
createNode transform -n "transform526";
createNode transform -n "transform527";
createNode transform -n "transform528";
createNode transform -n "transform529";
createNode transform -n "transform530";
createNode transform -n "transform531";
createNode transform -n "transform532";
createNode transform -n "transform533";
createNode transform -n "transform534";
createNode transform -n "transform535";
createNode transform -n "transform536";
createNode transform -n "transform537";
createNode transform -n "transform538";
createNode transform -n "transform539";
createNode transform -n "transform540";
createNode transform -n "transform541";
createNode transform -n "transform542";
createNode transform -n "transform543";
createNode transform -n "transform544";
createNode transform -n "transform545";
createNode transform -n "transform546";
createNode transform -n "transform547";
createNode transform -n "transform548";
createNode transform -n "transform549";
createNode transform -n "transform550";
createNode transform -n "transform551";
createNode transform -n "transform552";
createNode transform -n "transform553";
createNode transform -n "transform554";
createNode transform -n "transform555";
createNode transform -n "transform556";
createNode transform -n "transform557";
createNode transform -n "transform558";
createNode transform -n "transform559";
createNode transform -n "transform560";
createNode transform -n "transform561";
createNode transform -n "transform562";
createNode transform -n "transform563";
createNode transform -n "transform564";
createNode transform -n "transform565";
createNode transform -n "transform566";
createNode transform -n "transform567";
createNode transform -n "transform568";
createNode transform -n "transform569";
createNode transform -n "transform570";
createNode transform -n "transform571";
createNode transform -n "transform572";
createNode transform -n "transform573";
createNode transform -n "transform574";
createNode transform -n "transform575";
createNode transform -n "transform576";
createNode transform -n "transform577";
createNode transform -n "transform578";
createNode transform -n "transform579";
createNode transform -n "transform580";
createNode transform -n "transform581";
createNode transform -n "transform582";
createNode transform -n "transform583";
createNode transform -n "transform584";
createNode transform -n "transform585";
createNode transform -n "transform586";
createNode transform -n "transform587";
createNode transform -n "transform588";
createNode transform -n "transform589";
createNode transform -n "transform590";
createNode transform -n "transform591";
createNode transform -n "transform592";
createNode transform -n "transform593";
createNode transform -n "transform594";
createNode transform -n "transform595";
createNode transform -n "transform596";
createNode transform -n "transform597";
createNode transform -n "transform598";
createNode transform -n "transform599";
createNode transform -n "transform600";
createNode transform -n "transform601";
createNode transform -n "transform602";
createNode transform -n "transform603";
createNode transform -n "transform604";
createNode transform -n "transform605";
createNode transform -n "transform606";
createNode transform -n "transform607";
createNode transform -n "transform608";
createNode transform -n "transform609";
createNode transform -n "transform610";
createNode transform -n "transform611";
createNode transform -n "transform612";
createNode transform -n "transform613";
createNode transform -n "transform614";
createNode transform -n "transform615";
createNode transform -n "transform616";
createNode transform -n "transform617";
createNode transform -n "transform618";
createNode transform -n "transform619";
createNode transform -n "transform620";
createNode transform -n "transform621";
createNode transform -n "transform622";
createNode transform -n "transform623";
createNode transform -n "transform624";
createNode transform -n "transform625";
createNode transform -n "transform626";
createNode transform -n "transform627";
createNode transform -n "transform628";
createNode transform -n "transform629";
createNode transform -n "transform630";
createNode transform -n "transform631";
createNode transform -n "transform632";
createNode transform -n "transform633";
createNode transform -n "transform634";
createNode transform -n "transform635";
createNode transform -n "transform636";
createNode transform -n "transform637";
createNode transform -n "transform638";
createNode transform -n "transform639";
createNode transform -n "transform640";
createNode transform -n "transform641";
createNode transform -n "transform642";
createNode transform -n "transform643";
createNode transform -n "transform644";
createNode transform -n "transform645";
createNode transform -n "transform646";
createNode transform -n "transform647";
createNode transform -n "transform648";
createNode transform -n "transform649";
createNode transform -n "transform650";
createNode transform -n "transform651";
createNode transform -n "transform652";
createNode transform -n "transform653";
createNode transform -n "transform654";
createNode transform -n "transform655";
createNode transform -n "transform656";
createNode transform -n "transform657";
createNode transform -n "transform658";
createNode transform -n "transform659";
createNode transform -n "transform660";
createNode transform -n "transform661";
createNode transform -n "transform662";
createNode transform -n "transform663";
createNode transform -n "transform664";
createNode transform -n "transform665";
createNode transform -n "transform666";
createNode transform -n "transform667";
createNode transform -n "transform668";
createNode transform -n "transform669";
createNode transform -n "transform670";
createNode transform -n "transform671";
createNode transform -n "transform672";
createNode transform -n "transform673";
createNode transform -n "transform674";
createNode transform -n "transform675";
createNode transform -n "transform676";
createNode transform -n "transform677";
createNode transform -n "transform678";
createNode transform -n "transform679";
createNode transform -n "transform680";
createNode transform -n "transform681";
createNode transform -n "transform682";
createNode transform -n "transform683";
createNode transform -n "transform684";
createNode transform -n "transform685";
createNode transform -n "transform686";
createNode transform -n "transform687";
createNode transform -n "transform688";
createNode transform -n "transform689";
createNode transform -n "transform690";
createNode transform -n "transform691";
createNode transform -n "transform692";
createNode transform -n "transform693";
createNode transform -n "transform694";
createNode transform -n "transform695";
createNode transform -n "transform696";
createNode transform -n "transform697";
createNode transform -n "transform698";
createNode transform -n "transform699";
createNode transform -n "transform700";
createNode transform -n "transform701";
createNode transform -n "transform702";
createNode transform -n "transform703";
createNode transform -n "transform704";
createNode transform -n "transform705";
createNode transform -n "transform706";
createNode transform -n "transform707";
createNode transform -n "transform708";
createNode transform -n "transform709";
createNode transform -n "transform710";
createNode transform -n "transform711";
createNode transform -n "transform712";
createNode transform -n "transform713";
createNode transform -n "transform714";
createNode transform -n "transform715";
createNode transform -n "transform716";
createNode transform -n "transform717";
createNode transform -n "transform718";
createNode transform -n "transform719";
createNode transform -n "transform720";
createNode transform -n "transform721";
createNode transform -n "transform722";
createNode transform -n "transform723";
createNode transform -n "transform724";
createNode transform -n "transform725";
createNode transform -n "transform726";
createNode transform -n "transform727";
createNode transform -n "transform728";
createNode transform -n "transform729";
createNode transform -n "transform730";
createNode transform -n "transform731";
createNode transform -n "transform732";
createNode transform -n "transform733";
createNode transform -n "transform734";
createNode transform -n "transform735";
createNode transform -n "transform736";
createNode transform -n "transform737";
createNode transform -n "transform738";
createNode transform -n "transform739";
createNode transform -n "transform740";
createNode transform -n "transform741";
createNode transform -n "transform742";
createNode transform -n "transform743";
createNode transform -n "transform744";
createNode transform -n "transform745";
createNode transform -n "transform746";
createNode transform -n "transform747";
createNode transform -n "transform748";
createNode transform -n "transform749";
createNode transform -n "transform750";
createNode transform -n "transform751";
createNode transform -n "transform752";
createNode transform -n "transform753";
createNode transform -n "transform754";
createNode transform -n "transform755";
createNode transform -n "transform756";
createNode transform -n "transform757";
createNode transform -n "transform758";
createNode transform -n "transform759";
createNode transform -n "transform760";
createNode transform -n "transform761";
createNode transform -n "transform762";
createNode transform -n "transform763";
createNode transform -n "transform764";
createNode transform -n "transform765";
createNode transform -n "transform766";
createNode transform -n "transform767";
createNode transform -n "transform768";
createNode transform -n "transform769";
createNode transform -n "transform770";
createNode transform -n "transform771";
createNode transform -n "transform772";
createNode transform -n "transform773";
createNode transform -n "transform774";
createNode transform -n "transform775";
createNode transform -n "transform776";
createNode transform -n "transform777";
createNode transform -n "transform778";
createNode transform -n "transform779";
createNode transform -n "transform780";
createNode transform -n "transform781";
createNode transform -n "transform782";
createNode transform -n "transform783";
createNode transform -n "transform784";
createNode transform -n "transform785";
createNode transform -n "transform786";
createNode transform -n "transform787";
createNode transform -n "transform788";
createNode transform -n "transform789";
createNode transform -n "transform790";
createNode transform -n "transform791";
createNode transform -n "transform792";
createNode transform -n "transform793";
createNode transform -n "transform794";
createNode transform -n "transform795";
createNode transform -n "transform796";
createNode transform -n "transform797";
createNode transform -n "transform798";
createNode transform -n "transform799";
createNode transform -n "transform800";
createNode transform -n "transform801";
createNode transform -n "transform802";
createNode transform -n "transform803";
createNode transform -n "transform804";
createNode transform -n "transform805";
createNode transform -n "transform806";
createNode transform -n "transform807";
createNode transform -n "transform808";
createNode transform -n "transform809";
createNode transform -n "transform810";
createNode transform -n "transform811";
createNode transform -n "transform812";
createNode transform -n "transform813";
createNode transform -n "transform814";
createNode transform -n "transform815";
createNode transform -n "transform816";
createNode transform -n "transform817";
createNode transform -n "transform818";
createNode transform -n "transform819";
createNode transform -n "transform820";
createNode transform -n "transform821";
createNode transform -n "transform822";
createNode transform -n "transform823";
createNode transform -n "transform824";
createNode transform -n "transform825";
createNode transform -n "transform826";
createNode transform -n "transform827";
createNode transform -n "transform828";
createNode transform -n "transform829";
createNode transform -n "transform830";
createNode transform -n "transform831";
createNode transform -n "transform832";
createNode transform -n "transform833";
createNode transform -n "transform834";
createNode transform -n "transform835";
createNode transform -n "transform836";
createNode transform -n "transform837";
createNode transform -n "transform838";
createNode transform -n "transform839";
createNode transform -n "transform840";
createNode transform -n "transform841";
createNode transform -n "transform842";
createNode transform -n "transform843";
createNode transform -n "transform844";
createNode transform -n "transform845";
createNode transform -n "transform846";
createNode transform -n "transform847";
createNode transform -n "transform848";
createNode transform -n "transform849";
createNode transform -n "transform850";
createNode transform -n "transform851";
createNode transform -n "transform852";
createNode transform -n "transform853";
createNode transform -n "transform854";
createNode transform -n "transform855";
createNode transform -n "transform856";
createNode transform -n "transform857";
createNode transform -n "transform858";
createNode transform -n "transform859";
createNode transform -n "transform860";
createNode transform -n "transform861";
createNode transform -n "transform862";
createNode transform -n "transform863";
createNode transform -n "transform864";
createNode transform -n "transform865";
createNode transform -n "transform866";
createNode transform -n "transform867";
createNode transform -n "transform868";
createNode transform -n "transform869";
createNode transform -n "transform870";
createNode transform -n "transform871";
createNode transform -n "transform872";
createNode transform -n "transform873";
createNode transform -n "transform874";
createNode transform -n "transform875";
createNode transform -n "transform876";
createNode transform -n "transform877";
createNode transform -n "transform878";
createNode transform -n "transform879";
createNode transform -n "transform880";
createNode transform -n "transform881";
createNode transform -n "transform882";
createNode transform -n "transform883";
createNode transform -n "transform884";
createNode transform -n "transform885";
createNode transform -n "transform886";
createNode transform -n "transform887";
createNode transform -n "transform888";
createNode transform -n "transform889";
createNode transform -n "transform890";
createNode transform -n "transform891";
createNode transform -n "transform892";
createNode transform -n "transform893";
createNode transform -n "transform894";
createNode transform -n "transform895";
createNode transform -n "transform896";
createNode transform -n "transform897";
createNode transform -n "transform898";
createNode transform -n "transform899";
createNode transform -n "transform900";
createNode transform -n "transform901";
createNode transform -n "transform902";
createNode transform -n "transform903";
createNode transform -n "transform904";
createNode transform -n "transform905";
createNode transform -n "transform906";
createNode transform -n "transform907";
createNode transform -n "transform908";
createNode transform -n "transform909";
createNode transform -n "transform910";
createNode transform -n "transform911";
createNode transform -n "transform912";
createNode transform -n "transform913";
createNode transform -n "transform914";
createNode transform -n "transform915";
createNode transform -n "transform916";
createNode transform -n "transform917";
createNode transform -n "transform918";
createNode transform -n "transform919";
createNode transform -n "transform920";
createNode transform -n "transform921";
createNode transform -n "transform922";
createNode transform -n "transform923";
createNode transform -n "transform924";
createNode transform -n "transform925";
createNode transform -n "transform926";
createNode transform -n "transform927";
createNode transform -n "transform928";
createNode transform -n "transform929";
createNode transform -n "transform930";
createNode transform -n "transform931";
createNode transform -n "transform932";
createNode transform -n "transform933";
createNode transform -n "transform934";
createNode transform -n "transform935";
createNode transform -n "transform936";
createNode transform -n "transform937";
createNode transform -n "transform938";
createNode transform -n "transform939";
createNode transform -n "transform940";
createNode transform -n "transform941";
createNode transform -n "transform942";
createNode transform -n "transform943";
createNode transform -n "transform944";
createNode transform -n "transform945";
createNode transform -n "transform946";
createNode transform -n "transform947";
createNode transform -n "transform948";
createNode transform -n "transform949";
createNode transform -n "transform950";
createNode transform -n "transform951";
createNode transform -n "transform952";
createNode transform -n "transform953";
createNode transform -n "transform954";
createNode transform -n "transform955";
createNode transform -n "transform956";
createNode transform -n "transform957";
createNode transform -n "transform958";
createNode transform -n "transform959";
createNode transform -n "transform960";
createNode transform -n "transform961";
createNode transform -n "transform962";
createNode transform -n "transform963";
createNode transform -n "transform964";
createNode transform -n "transform965";
createNode transform -n "transform966";
createNode transform -n "transform967";
createNode transform -n "transform968";
createNode transform -n "transform969";
createNode transform -n "transform970";
createNode transform -n "transform971";
createNode transform -n "transform972";
createNode transform -n "transform973";
createNode transform -n "transform974";
createNode transform -n "transform975";
createNode transform -n "transform976";
createNode transform -n "transform977";
createNode transform -n "transform978";
createNode transform -n "transform979";
createNode transform -n "transform980";
createNode transform -n "transform981";
createNode transform -n "transform982";
createNode transform -n "transform983";
createNode transform -n "transform984";
createNode transform -n "transform985";
createNode transform -n "transform986";
createNode transform -n "transform987";
createNode transform -n "transform988";
createNode transform -n "transform989";
createNode transform -n "transform990";
createNode transform -n "transform991";
createNode transform -n "transform992";
createNode transform -n "transform993";
createNode transform -n "transform994";
createNode transform -n "transform995";
createNode transform -n "transform996";
createNode transform -n "transform997";
createNode transform -n "transform998";
createNode transform -n "transform999";
createNode transform -n "transform1000";
createNode transform -n "transform1001";
createNode transform -n "transform1002";
createNode transform -n "transform1003";
createNode transform -n "transform1004";
createNode transform -n "transform1005";
createNode transform -n "transform1006";
createNode transform -n "transform1007";
createNode transform -n "transform1008";
createNode transform -n "transform1009";
createNode transform -n "transform1010";
createNode transform -n "transform1011";
createNode transform -n "transform1012";
createNode transform -n "transform1013";
createNode transform -n "transform1014";
createNode transform -n "transform1015";
createNode transform -n "transform1016";
createNode transform -n "transform1017";
createNode transform -n "transform1018";
createNode transform -n "transform1019";
createNode transform -n "transform1020";
createNode transform -n "transform1021";
createNode transform -n "transform1022";
createNode transform -n "transform1023";
createNode transform -n "transform1024";
createNode transform -n "transform1025";
createNode transform -n "transform1026";
createNode transform -n "transform1027";
createNode transform -n "transform1028";
createNode transform -n "transform1029";
createNode transform -n "transform1030";
createNode transform -n "transform1031";
createNode transform -n "transform1032";
createNode transform -n "transform1033";
createNode transform -n "transform1034";
createNode transform -n "transform1035";
createNode transform -n "transform1036";
createNode transform -n "transform1037";
createNode transform -n "transform1038";
createNode transform -n "transform1039";
createNode transform -n "transform1040";
createNode transform -n "transform1041";
createNode transform -n "transform1042";
createNode transform -n "transform1043";
createNode transform -n "transform1044";
createNode transform -n "transform1045";
createNode transform -n "transform1046";
createNode transform -n "transform1047";
createNode transform -n "transform1048";
createNode transform -n "transform1049";
createNode transform -n "transform1050";
createNode transform -n "transform1051";
createNode transform -n "transform1052";
createNode transform -n "transform1053";
createNode transform -n "transform1054";
createNode transform -n "transform1055";
createNode transform -n "transform1056";
createNode transform -n "transform1057";
createNode transform -n "transform1058";
createNode transform -n "transform1059";
createNode transform -n "transform1060";
createNode transform -n "transform1061";
createNode transform -n "transform1062";
createNode transform -n "transform1063";
createNode transform -n "transform1064";
createNode transform -n "transform1065";
createNode transform -n "transform1066";
createNode transform -n "transform1067";
createNode transform -n "transform1068";
createNode transform -n "transform1069";
createNode transform -n "transform1070";
createNode transform -n "transform1071";
createNode transform -n "transform1072";
createNode transform -n "transform1073";
createNode transform -n "transform1074";
createNode transform -n "transform1075";
createNode transform -n "transform1076";
createNode transform -n "transform1077";
createNode transform -n "transform1078";
createNode transform -n "transform1079";
createNode transform -n "transform1080";
createNode transform -n "transform1081";
createNode transform -n "transform1082";
createNode transform -n "transform1083";
createNode transform -n "transform1084";
createNode transform -n "transform1085";
createNode transform -n "transform1086";
createNode transform -n "transform1087";
createNode transform -n "transform1088";
createNode transform -n "transform1089";
createNode transform -n "transform1090";
createNode transform -n "transform1091";
createNode transform -n "transform1092";
createNode transform -n "transform1093";
createNode transform -n "transform1094";
createNode transform -n "transform1095";
createNode transform -n "transform1096";
createNode transform -n "transform1097";
createNode transform -n "transform1098";
createNode transform -n "transform1099";
createNode transform -n "transform1100";
createNode transform -n "transform1101";
createNode transform -n "transform1102";
createNode transform -n "hairSystem1";
createNode hairSystem -n "hairSystemShape1" -p "hairSystem1";
	setAttr -k off ".v";
	setAttr -s 240 ".ih";
	setAttr ".evo" 0;
	setAttr ".sfn" 0.1;
	setAttr -s 2 ".sts[0:1]"  0 1 1 1 0.2 1;
	setAttr -s 2 ".ats[0:1]"  0 1 1 1 0.2 1;
	setAttr ".cwd" 2;
	setAttr -s 2 ".cws[0:1]"  0 1 3 1 1 3;
	setAttr -s 3 ".clc[0:2]"  0 0.50999999 1 0.5 0.47999999 1 1 0.51999998 1;
	setAttr ".cfl[0]"  0 0 1;
	setAttr ".hwd" 0.03;
	setAttr -s 2 ".hws[0:1]"  0.80000001 1 1 1 0.2 1;
	setAttr ".hcl" -type "float3" 1 1 1 ;
	setAttr -s 3 ".hcs";
	setAttr ".hcs[0].hcsp" 0;
	setAttr ".hcs[0].hcsc" -type "float3" 1 0 0 ;
	setAttr ".hcs[0].hcsi" 1;
	setAttr ".hcs[1].hcsp" 0.5;
	setAttr ".hcs[1].hcsc" -type "float3" 1 1 0 ;
	setAttr ".hcs[1].hcsi" 1;
	setAttr ".hcs[2].hcsp" 1;
	setAttr ".hcs[2].hcsc" -type "float3" 1 0 0 ;
	setAttr ".hcs[2].hcsi" 1;
	setAttr ".hpc" 16;
	setAttr ".spc" -type "float3" 0.30769819 0.30769819 0.26375219 ;
	setAttr ".spp" 8;
	setAttr ".dfr" 0.4;
	setAttr ".sra" 0.8;
	setAttr ".chr" 0.05;
	setAttr ".cvr" 0.6;
	setAttr ".dsc[0]"  0 1 1;
	setAttr ".actv" yes;
	setAttr ".ai_opaque" no;
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
		0 0 0.44444444440000003
		0 0 0.88888888889999995
		0 0 1.3333333329999999
		0 0 1.7777777779999999
		0 0 2.2222222220000001
		0 0 2.6666666669999999
		0 0 3.111111111
		0 0 3.5555555559999998
		0 0 4
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
		0 0 0.44444444440000003
		0 0 0.88888888889999995
		0 0 1.3333333329999999
		0 0 1.7777777779999999
		0 0 2.2222222220000001
		0 0 2.6666666669999999
		0 0 3.111111111
		0 0 3.5555555559999998
		0 0 4
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
		0 0 0.44444444440000003
		0 0 0.88888888889999995
		0 0 1.3333333329999999
		0 0 1.7777777779999999
		0 0 2.2222222220000001
		0 0 2.6666666669999999
		0 0 3.111111111
		0 0 3.5555555559999998
		0 0 4
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
		0 0 0.44444444440000003
		0 0 0.88888888889999995
		0 0 1.3333333329999999
		0 0 1.7777777779999999
		0 0 2.2222222220000001
		0 0 2.6666666669999999
		0 0 3.111111111
		0 0 3.5555555559999998
		0 0 4
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
		0 0 0.44444444440000003
		0 0 0.88888888889999995
		0 0 1.3333333329999999
		0 0 1.7777777779999999
		0 0 2.2222222220000001
		0 0 2.6666666669999999
		0 0 3.111111111
		0 0 3.5555555559999998
		0 0 4
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
		0 0 0.44444444440000003
		0 0 0.88888888889999995
		0 0 1.3333333329999999
		0 0 1.7777777779999999
		0 0 2.2222222220000001
		0 0 2.6666666669999999
		0 0 3.111111111
		0 0 3.5555555559999998
		0 0 4
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
		0 0 0.44444444440000003
		0 0 0.88888888889999995
		0 0 1.3333333329999999
		0 0 1.7777777779999999
		0 0 2.2222222220000001
		0 0 2.6666666669999999
		0 0 3.111111111
		0 0 3.5555555559999998
		0 0 4
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
		0 0 0.44444444440000003
		0 0 0.88888888889999995
		0 0 1.3333333329999999
		0 0 1.7777777779999999
		0 0 2.2222222220000001
		0 0 2.6666666669999999
		0 0 3.111111111
		0 0 3.5555555559999998
		0 0 4
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
		0 0 0.44444444440000003
		0 0 0.88888888889999995
		0 0 1.3333333329999999
		0 0 1.7777777779999999
		0 0 2.2222222220000001
		0 0 2.6666666669999999
		0 0 3.111111111
		0 0 3.5555555559999998
		0 0 4
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
		0 0 0.44444444440000003
		0 0 0.88888888889999995
		0 0 1.3333333329999999
		0 0 1.7777777779999999
		0 0 2.2222222220000001
		0 0 2.6666666669999999
		0 0 3.111111111
		0 0 3.5555555559999998
		0 0 4
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
		0 0 0.44444444440000003
		0 0 0.88888888889999995
		0 0 1.3333333329999999
		0 0 1.7777777779999999
		0 0 2.2222222220000001
		0 0 2.6666666669999999
		0 0 3.111111111
		0 0 3.5555555559999998
		0 0 4
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
		0 0 0.44444444440000003
		0 0 0.88888888889999995
		0 0 1.3333333329999999
		0 0 1.7777777779999999
		0 0 2.2222222220000001
		0 0 2.6666666669999999
		0 0 3.111111111
		0 0 3.5555555559999998
		0 0 4
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
		0 0 0.44444444440000003
		0 0 0.88888888889999995
		0 0 1.3333333329999999
		0 0 1.7777777779999999
		0 0 2.2222222220000001
		0 0 2.6666666669999999
		0 0 3.111111111
		0 0 3.5555555559999998
		0 0 4
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
		0 0 0.44444444440000003
		0 0 0.88888888889999995
		0 0 1.3333333329999999
		0 0 1.7777777779999999
		0 0 2.2222222220000001
		0 0 2.6666666669999999
		0 0 3.111111111
		0 0 3.5555555559999998
		0 0 4
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
		0 0 0.44444444440000003
		0 0 0.88888888889999995
		0 0 1.3333333329999999
		0 0 1.7777777779999999
		0 0 2.2222222220000001
		0 0 2.6666666669999999
		0 0 3.111111111
		0 0 3.5555555559999998
		0 0 4
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
		0 0 0.44444444440000003
		0 0 0.88888888889999995
		0 0 1.3333333329999999
		0 0 1.7777777779999999
		0 0 2.2222222220000001
		0 0 2.6666666669999999
		0 0 3.111111111
		0 0 3.5555555559999998
		0 0 4
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
		0 0 0.44444444440000003
		0 0 0.88888888889999995
		0 0 1.3333333329999999
		0 0 1.7777777779999999
		0 0 2.2222222220000001
		0 0 2.6666666669999999
		0 0 3.111111111
		0 0 3.5555555559999998
		0 0 4
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
		0 0 0.44444444440000003
		0 0 0.88888888889999995
		0 0 1.3333333329999999
		0 0 1.7777777779999999
		0 0 2.2222222220000001
		0 0 2.6666666669999999
		0 0 3.111111111
		0 0 3.5555555559999998
		0 0 4
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
		0 0 0.44444444440000003
		0 0 0.88888888889999995
		0 0 1.3333333329999999
		0 0 1.7777777779999999
		0 0 2.2222222220000001
		0 0 2.6666666669999999
		0 0 3.111111111
		0 0 3.5555555559999998
		0 0 4
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
		0 0 0.44444444440000003
		0 0 0.88888888889999995
		0 0 1.3333333329999999
		0 0 1.7777777779999999
		0 0 2.2222222220000001
		0 0 2.6666666669999999
		0 0 3.111111111
		0 0 3.5555555559999998
		0 0 4
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
		0 0 0.44444444440000003
		0 0 0.88888888889999995
		0 0 1.3333333329999999
		0 0 1.7777777779999999
		0 0 2.2222222220000001
		0 0 2.6666666669999999
		0 0 3.111111111
		0 0 3.5555555559999998
		0 0 4
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
		0 0 0.44444444440000003
		0 0 0.88888888889999995
		0 0 1.3333333329999999
		0 0 1.7777777779999999
		0 0 2.2222222220000001
		0 0 2.6666666669999999
		0 0 3.111111111
		0 0 3.5555555559999998
		0 0 4
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
		0 0 0.44444444440000003
		0 0 0.88888888889999995
		0 0 1.3333333329999999
		0 0 1.7777777779999999
		0 0 2.2222222220000001
		0 0 2.6666666669999999
		0 0 3.111111111
		0 0 3.5555555559999998
		0 0 4
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
		0 0 0.44444444440000003
		0 0 0.88888888889999995
		0 0 1.3333333329999999
		0 0 1.7777777779999999
		0 0 2.2222222220000001
		0 0 2.6666666669999999
		0 0 3.111111111
		0 0 3.5555555559999998
		0 0 4
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
		0 0 0.44444444440000003
		0 0 0.88888888889999995
		0 0 1.3333333329999999
		0 0 1.7777777779999999
		0 0 2.2222222220000001
		0 0 2.6666666669999999
		0 0 3.111111111
		0 0 3.5555555559999998
		0 0 4
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
		0 0 0.44444444440000003
		0 0 0.88888888889999995
		0 0 1.3333333329999999
		0 0 1.7777777779999999
		0 0 2.2222222220000001
		0 0 2.6666666669999999
		0 0 3.111111111
		0 0 3.5555555559999998
		0 0 4
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
		0 0 0.44444444440000003
		0 0 0.88888888889999995
		0 0 1.3333333329999999
		0 0 1.7777777779999999
		0 0 2.2222222220000001
		0 0 2.6666666669999999
		0 0 3.111111111
		0 0 3.5555555559999998
		0 0 4
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
		0 0 0.44444444440000003
		0 0 0.88888888889999995
		0 0 1.3333333329999999
		0 0 1.7777777779999999
		0 0 2.2222222220000001
		0 0 2.6666666669999999
		0 0 3.111111111
		0 0 3.5555555559999998
		0 0 4
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
		0 0 0.44444444440000003
		0 0 0.88888888889999995
		0 0 1.3333333329999999
		0 0 1.7777777779999999
		0 0 2.2222222220000001
		0 0 2.6666666669999999
		0 0 3.111111111
		0 0 3.5555555559999998
		0 0 4
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
		0 0 0.44444444440000003
		0 0 0.88888888889999995
		0 0 1.3333333329999999
		0 0 1.7777777779999999
		0 0 2.2222222220000001
		0 0 2.6666666669999999
		0 0 3.111111111
		0 0 3.5555555559999998
		0 0 4
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
		0 0 0.44444444440000003
		0 0 0.88888888889999995
		0 0 1.3333333329999999
		0 0 1.7777777779999999
		0 0 2.2222222220000001
		0 0 2.6666666669999999
		0 0 3.111111111
		0 0 3.5555555559999998
		0 0 4
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
		0 0 0.44444444440000003
		0 0 0.88888888889999995
		0 0 1.3333333329999999
		0 0 1.7777777779999999
		0 0 2.2222222220000001
		0 0 2.6666666669999999
		0 0 3.111111111
		0 0 3.5555555559999998
		0 0 4
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
		0 0 0.44444444440000003
		0 0 0.88888888889999995
		0 0 1.3333333329999999
		0 0 1.7777777779999999
		0 0 2.2222222220000001
		0 0 2.6666666669999999
		0 0 3.111111111
		0 0 3.5555555559999998
		0 0 4
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
		0 0 0.44444444440000003
		0 0 0.88888888889999995
		0 0 1.3333333329999999
		0 0 1.7777777779999999
		0 0 2.2222222220000001
		0 0 2.6666666669999999
		0 0 3.111111111
		0 0 3.5555555559999998
		0 0 4
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
		0 0 0.44444444440000003
		0 0 0.88888888889999995
		0 0 1.3333333329999999
		0 0 1.7777777779999999
		0 0 2.2222222220000001
		0 0 2.6666666669999999
		0 0 3.111111111
		0 0 3.5555555559999998
		0 0 4
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
		0 0 0.44444444440000003
		0 0 0.88888888889999995
		0 0 1.3333333329999999
		0 0 1.7777777779999999
		0 0 2.2222222220000001
		0 0 2.6666666669999999
		0 0 3.111111111
		0 0 3.5555555559999998
		0 0 4
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
		0 0 0.44444444440000003
		0 0 0.88888888889999995
		0 0 1.3333333329999999
		0 0 1.7777777779999999
		0 0 2.2222222220000001
		0 0 2.6666666669999999
		0 0 3.111111111
		0 0 3.5555555559999998
		0 0 4
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
		0 0 0.44444444440000003
		0 0 0.88888888889999995
		0 0 1.3333333329999999
		0 0 1.7777777779999999
		0 0 2.2222222220000001
		0 0 2.6666666669999999
		0 0 3.111111111
		0 0 3.5555555559999998
		0 0 4
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
		0 0 0.44444444440000003
		0 0 0.88888888889999995
		0 0 1.3333333329999999
		0 0 1.7777777779999999
		0 0 2.2222222220000001
		0 0 2.6666666669999999
		0 0 3.111111111
		0 0 3.5555555559999998
		0 0 4
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
		0 0 0.44444444440000003
		0 0 0.88888888889999995
		0 0 1.3333333329999999
		0 0 1.7777777779999999
		0 0 2.2222222220000001
		0 0 2.6666666669999999
		0 0 3.111111111
		0 0 3.5555555559999998
		0 0 4
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
		0 0 0.44444444440000003
		0 0 0.88888888889999995
		0 0 1.3333333329999999
		0 0 1.7777777779999999
		0 0 2.2222222220000001
		0 0 2.6666666669999999
		0 0 3.111111111
		0 0 3.5555555559999998
		0 0 4
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
		0 0 0.44444444440000003
		0 0 0.88888888889999995
		0 0 1.3333333329999999
		0 0 1.7777777779999999
		0 0 2.2222222220000001
		0 0 2.6666666669999999
		0 0 3.111111111
		0 0 3.5555555559999998
		0 0 4
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
		0 0 0.44444444440000003
		0 0 0.88888888889999995
		0 0 1.3333333329999999
		0 0 1.7777777779999999
		0 0 2.2222222220000001
		0 0 2.6666666669999999
		0 0 3.111111111
		0 0 3.5555555559999998
		0 0 4
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
		0 0 0.44444444440000003
		0 0 0.88888888889999995
		0 0 1.3333333329999999
		0 0 1.7777777779999999
		0 0 2.2222222220000001
		0 0 2.6666666669999999
		0 0 3.111111111
		0 0 3.5555555559999998
		0 0 4
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
		0 0 0.44444444440000003
		0 0 0.88888888889999995
		0 0 1.3333333329999999
		0 0 1.7777777779999999
		0 0 2.2222222220000001
		0 0 2.6666666669999999
		0 0 3.111111111
		0 0 3.5555555559999998
		0 0 4
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
		0 0 0.44444444440000003
		0 0 0.88888888889999995
		0 0 1.3333333329999999
		0 0 1.7777777779999999
		0 0 2.2222222220000001
		0 0 2.6666666669999999
		0 0 3.111111111
		0 0 3.5555555559999998
		0 0 4
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
		0 0 0.44444444440000003
		0 0 0.88888888889999995
		0 0 1.3333333329999999
		0 0 1.7777777779999999
		0 0 2.2222222220000001
		0 0 2.6666666669999999
		0 0 3.111111111
		0 0 3.5555555559999998
		0 0 4
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
		0 0 0.44444444440000003
		0 0 0.88888888889999995
		0 0 1.3333333329999999
		0 0 1.7777777779999999
		0 0 2.2222222220000001
		0 0 2.6666666669999999
		0 0 3.111111111
		0 0 3.5555555559999998
		0 0 4
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
		0 0 0.44444444440000003
		0 0 0.88888888889999995
		0 0 1.3333333329999999
		0 0 1.7777777779999999
		0 0 2.2222222220000001
		0 0 2.6666666669999999
		0 0 3.111111111
		0 0 3.5555555559999998
		0 0 4
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
		0 0 0.44444444440000003
		0 0 0.88888888889999995
		0 0 1.3333333329999999
		0 0 1.7777777779999999
		0 0 2.2222222220000001
		0 0 2.6666666669999999
		0 0 3.111111111
		0 0 3.5555555559999998
		0 0 4
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
		0 0 0.44444444440000003
		0 0 0.88888888889999995
		0 0 1.3333333329999999
		0 0 1.7777777779999999
		0 0 2.2222222220000001
		0 0 2.6666666669999999
		0 0 3.111111111
		0 0 3.5555555559999998
		0 0 4
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
		0 0 0.44444444440000003
		0 0 0.88888888889999995
		0 0 1.3333333329999999
		0 0 1.7777777779999999
		0 0 2.2222222220000001
		0 0 2.6666666669999999
		0 0 3.111111111
		0 0 3.5555555559999998
		0 0 4
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
		0 0 0.44444444440000003
		0 0 0.88888888889999995
		0 0 1.3333333329999999
		0 0 1.7777777779999999
		0 0 2.2222222220000001
		0 0 2.6666666669999999
		0 0 3.111111111
		0 0 3.5555555559999998
		0 0 4
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
		0 0 0.44444444440000003
		0 0 0.88888888889999995
		0 0 1.3333333329999999
		0 0 1.7777777779999999
		0 0 2.2222222220000001
		0 0 2.6666666669999999
		0 0 3.111111111
		0 0 3.5555555559999998
		0 0 4
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
		0 0 0.44444444440000003
		0 0 0.88888888889999995
		0 0 1.3333333329999999
		0 0 1.7777777779999999
		0 0 2.2222222220000001
		0 0 2.6666666669999999
		0 0 3.111111111
		0 0 3.5555555559999998
		0 0 4
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
		0 0 0.44444444440000003
		0 0 0.88888888889999995
		0 0 1.3333333329999999
		0 0 1.7777777779999999
		0 0 2.2222222220000001
		0 0 2.6666666669999999
		0 0 3.111111111
		0 0 3.5555555559999998
		0 0 4
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
		0 0 0.44444444440000003
		0 0 0.88888888889999995
		0 0 1.3333333329999999
		0 0 1.7777777779999999
		0 0 2.2222222220000001
		0 0 2.6666666669999999
		0 0 3.111111111
		0 0 3.5555555559999998
		0 0 4
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
		0 0 0.44444444440000003
		0 0 0.88888888889999995
		0 0 1.3333333329999999
		0 0 1.7777777779999999
		0 0 2.2222222220000001
		0 0 2.6666666669999999
		0 0 3.111111111
		0 0 3.5555555559999998
		0 0 4
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
		0 0 0.44444444440000003
		0 0 0.88888888889999995
		0 0 1.3333333329999999
		0 0 1.7777777779999999
		0 0 2.2222222220000001
		0 0 2.6666666669999999
		0 0 3.111111111
		0 0 3.5555555559999998
		0 0 4
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
		0 0 0.44444444440000003
		0 0 0.88888888889999995
		0 0 1.3333333329999999
		0 0 1.7777777779999999
		0 0 2.2222222220000001
		0 0 2.6666666669999999
		0 0 3.111111111
		0 0 3.5555555559999998
		0 0 4
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
		0 0 0.44444444440000003
		0 0 0.88888888889999995
		0 0 1.3333333329999999
		0 0 1.7777777779999999
		0 0 2.2222222220000001
		0 0 2.6666666669999999
		0 0 3.111111111
		0 0 3.5555555559999998
		0 0 4
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
		0 0 0.44444444440000003
		0 0 0.88888888889999995
		0 0 1.3333333329999999
		0 0 1.7777777779999999
		0 0 2.2222222220000001
		0 0 2.6666666669999999
		0 0 3.111111111
		0 0 3.5555555559999998
		0 0 4
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
		0 0 0.44444444440000003
		0 0 0.88888888889999995
		0 0 1.3333333329999999
		0 0 1.7777777779999999
		0 0 2.2222222220000001
		0 0 2.6666666669999999
		0 0 3.111111111
		0 0 3.5555555559999998
		0 0 4
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
		0 0 0.44444444440000003
		0 0 0.88888888889999995
		0 0 1.3333333329999999
		0 0 1.7777777779999999
		0 0 2.2222222220000001
		0 0 2.6666666669999999
		0 0 3.111111111
		0 0 3.5555555559999998
		0 0 4
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
		0 0 0.44444444440000003
		0 0 0.88888888889999995
		0 0 1.3333333329999999
		0 0 1.7777777779999999
		0 0 2.2222222220000001
		0 0 2.6666666669999999
		0 0 3.111111111
		0 0 3.5555555559999998
		0 0 4
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
		0 0 0.44444444440000003
		0 0 0.88888888889999995
		0 0 1.3333333329999999
		0 0 1.7777777779999999
		0 0 2.2222222220000001
		0 0 2.6666666669999999
		0 0 3.111111111
		0 0 3.5555555559999998
		0 0 4
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
		0 0 0.44444444440000003
		0 0 0.88888888889999995
		0 0 1.3333333329999999
		0 0 1.7777777779999999
		0 0 2.2222222220000001
		0 0 2.6666666669999999
		0 0 3.111111111
		0 0 3.5555555559999998
		0 0 4
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
		0 0 0.44444444440000003
		0 0 0.88888888889999995
		0 0 1.3333333329999999
		0 0 1.7777777779999999
		0 0 2.2222222220000001
		0 0 2.6666666669999999
		0 0 3.111111111
		0 0 3.5555555559999998
		0 0 4
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
		0 0 0.44444444440000003
		0 0 0.88888888889999995
		0 0 1.3333333329999999
		0 0 1.7777777779999999
		0 0 2.2222222220000001
		0 0 2.6666666669999999
		0 0 3.111111111
		0 0 3.5555555559999998
		0 0 4
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
		0 0 0.44444444440000003
		0 0 0.88888888889999995
		0 0 1.3333333329999999
		0 0 1.7777777779999999
		0 0 2.2222222220000001
		0 0 2.6666666669999999
		0 0 3.111111111
		0 0 3.5555555559999998
		0 0 4
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
		0 0 0.44444444440000003
		0 0 0.88888888889999995
		0 0 1.3333333329999999
		0 0 1.7777777779999999
		0 0 2.2222222220000001
		0 0 2.6666666669999999
		0 0 3.111111111
		0 0 3.5555555559999998
		0 0 4
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
		0 0 0.44444444440000003
		0 0 0.88888888889999995
		0 0 1.3333333329999999
		0 0 1.7777777779999999
		0 0 2.2222222220000001
		0 0 2.6666666669999999
		0 0 3.111111111
		0 0 3.5555555559999998
		0 0 4
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
		0 0 0.44444444440000003
		0 0 0.88888888889999995
		0 0 1.3333333329999999
		0 0 1.7777777779999999
		0 0 2.2222222220000001
		0 0 2.6666666669999999
		0 0 3.111111111
		0 0 3.5555555559999998
		0 0 4
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
		0 0 0.44444444440000003
		0 0 0.88888888889999995
		0 0 1.3333333329999999
		0 0 1.7777777779999999
		0 0 2.2222222220000001
		0 0 2.6666666669999999
		0 0 3.111111111
		0 0 3.5555555559999998
		0 0 4
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
		0 0 0.44444444440000003
		0 0 0.88888888889999995
		0 0 1.3333333329999999
		0 0 1.7777777779999999
		0 0 2.2222222220000001
		0 0 2.6666666669999999
		0 0 3.111111111
		0 0 3.5555555559999998
		0 0 4
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
		0 0 0.44444444440000003
		0 0 0.88888888889999995
		0 0 1.3333333329999999
		0 0 1.7777777779999999
		0 0 2.2222222220000001
		0 0 2.6666666669999999
		0 0 3.111111111
		0 0 3.5555555559999998
		0 0 4
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
		0 0 0.44444444440000003
		0 0 0.88888888889999995
		0 0 1.3333333329999999
		0 0 1.7777777779999999
		0 0 2.2222222220000001
		0 0 2.6666666669999999
		0 0 3.111111111
		0 0 3.5555555559999998
		0 0 4
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
		0 0 0.44444444440000003
		0 0 0.88888888889999995
		0 0 1.3333333329999999
		0 0 1.7777777779999999
		0 0 2.2222222220000001
		0 0 2.6666666669999999
		0 0 3.111111111
		0 0 3.5555555559999998
		0 0 4
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
		0 0 0.44444444440000003
		0 0 0.88888888889999995
		0 0 1.3333333329999999
		0 0 1.7777777779999999
		0 0 2.2222222220000001
		0 0 2.6666666669999999
		0 0 3.111111111
		0 0 3.5555555559999998
		0 0 4
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
		0 0 0.44444444440000003
		0 0 0.88888888889999995
		0 0 1.3333333329999999
		0 0 1.7777777779999999
		0 0 2.2222222220000001
		0 0 2.6666666669999999
		0 0 3.111111111
		0 0 3.5555555559999998
		0 0 4
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
		0 0 0.44444444440000003
		0 0 0.88888888889999995
		0 0 1.3333333329999999
		0 0 1.7777777779999999
		0 0 2.2222222220000001
		0 0 2.6666666669999999
		0 0 3.111111111
		0 0 3.5555555559999998
		0 0 4
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
		0 0 0.44444444440000003
		0 0 0.88888888889999995
		0 0 1.3333333329999999
		0 0 1.7777777779999999
		0 0 2.2222222220000001
		0 0 2.6666666669999999
		0 0 3.111111111
		0 0 3.5555555559999998
		0 0 4
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
		0 0 0.44444444440000003
		0 0 0.88888888889999995
		0 0 1.3333333329999999
		0 0 1.7777777779999999
		0 0 2.2222222220000001
		0 0 2.6666666669999999
		0 0 3.111111111
		0 0 3.5555555559999998
		0 0 4
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
		0 0 0.44444444440000003
		0 0 0.88888888889999995
		0 0 1.3333333329999999
		0 0 1.7777777779999999
		0 0 2.2222222220000001
		0 0 2.6666666669999999
		0 0 3.111111111
		0 0 3.5555555559999998
		0 0 4
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
		0 0 0.44444444440000003
		0 0 0.88888888889999995
		0 0 1.3333333329999999
		0 0 1.7777777779999999
		0 0 2.2222222220000001
		0 0 2.6666666669999999
		0 0 3.111111111
		0 0 3.5555555559999998
		0 0 4
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
		0 0 0.44444444440000003
		0 0 0.88888888889999995
		0 0 1.3333333329999999
		0 0 1.7777777779999999
		0 0 2.2222222220000001
		0 0 2.6666666669999999
		0 0 3.111111111
		0 0 3.5555555559999998
		0 0 4
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
		0 0 0.44444444440000003
		0 0 0.88888888889999995
		0 0 1.3333333329999999
		0 0 1.7777777779999999
		0 0 2.2222222220000001
		0 0 2.6666666669999999
		0 0 3.111111111
		0 0 3.5555555559999998
		0 0 4
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
		0 0 0.44444444440000003
		0 0 0.88888888889999995
		0 0 1.3333333329999999
		0 0 1.7777777779999999
		0 0 2.2222222220000001
		0 0 2.6666666669999999
		0 0 3.111111111
		0 0 3.5555555559999998
		0 0 4
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
		0 0 0.44444444440000003
		0 0 0.88888888889999995
		0 0 1.3333333329999999
		0 0 1.7777777779999999
		0 0 2.2222222220000001
		0 0 2.6666666669999999
		0 0 3.111111111
		0 0 3.5555555559999998
		0 0 4
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
		0 0 0.44444444440000003
		0 0 0.88888888889999995
		0 0 1.3333333329999999
		0 0 1.7777777779999999
		0 0 2.2222222220000001
		0 0 2.6666666669999999
		0 0 3.111111111
		0 0 3.5555555559999998
		0 0 4
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
		0 0 0.44444444440000003
		0 0 0.88888888889999995
		0 0 1.3333333329999999
		0 0 1.7777777779999999
		0 0 2.2222222220000001
		0 0 2.6666666669999999
		0 0 3.111111111
		0 0 3.5555555559999998
		0 0 4
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
		0 0 0.44444444440000003
		0 0 0.88888888889999995
		0 0 1.3333333329999999
		0 0 1.7777777779999999
		0 0 2.2222222220000001
		0 0 2.6666666669999999
		0 0 3.111111111
		0 0 3.5555555559999998
		0 0 4
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
		0 0 0.44444444440000003
		0 0 0.88888888889999995
		0 0 1.3333333329999999
		0 0 1.7777777779999999
		0 0 2.2222222220000001
		0 0 2.6666666669999999
		0 0 3.111111111
		0 0 3.5555555559999998
		0 0 4
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
		0 0 0.44444444440000003
		0 0 0.88888888889999995
		0 0 1.3333333329999999
		0 0 1.7777777779999999
		0 0 2.2222222220000001
		0 0 2.6666666669999999
		0 0 3.111111111
		0 0 3.5555555559999998
		0 0 4
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
		0 0 0.44444444440000003
		0 0 0.88888888889999995
		0 0 1.3333333329999999
		0 0 1.7777777779999999
		0 0 2.2222222220000001
		0 0 2.6666666669999999
		0 0 3.111111111
		0 0 3.5555555559999998
		0 0 4
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
		0 0 0.44444444440000003
		0 0 0.88888888889999995
		0 0 1.3333333329999999
		0 0 1.7777777779999999
		0 0 2.2222222220000001
		0 0 2.6666666669999999
		0 0 3.111111111
		0 0 3.5555555559999998
		0 0 4
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
		0 0 0.44444444440000003
		0 0 0.88888888889999995
		0 0 1.3333333329999999
		0 0 1.7777777779999999
		0 0 2.2222222220000001
		0 0 2.6666666669999999
		0 0 3.111111111
		0 0 3.5555555559999998
		0 0 4
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
		0 0 0.44444444440000003
		0 0 0.88888888889999995
		0 0 1.3333333329999999
		0 0 1.7777777779999999
		0 0 2.2222222220000001
		0 0 2.6666666669999999
		0 0 3.111111111
		0 0 3.5555555559999998
		0 0 4
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
		0 0 0.44444444440000003
		0 0 0.88888888889999995
		0 0 1.3333333329999999
		0 0 1.7777777779999999
		0 0 2.2222222220000001
		0 0 2.6666666669999999
		0 0 3.111111111
		0 0 3.5555555559999998
		0 0 4
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
		0 0 0.44444444440000003
		0 0 0.88888888889999995
		0 0 1.3333333329999999
		0 0 1.7777777779999999
		0 0 2.2222222220000001
		0 0 2.6666666669999999
		0 0 3.111111111
		0 0 3.5555555559999998
		0 0 4
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
		0 0 0.44444444440000003
		0 0 0.88888888889999995
		0 0 1.3333333329999999
		0 0 1.7777777779999999
		0 0 2.2222222220000001
		0 0 2.6666666669999999
		0 0 3.111111111
		0 0 3.5555555559999998
		0 0 4
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
		0 0 0.44444444440000003
		0 0 0.88888888889999995
		0 0 1.3333333329999999
		0 0 1.7777777779999999
		0 0 2.2222222220000001
		0 0 2.6666666669999999
		0 0 3.111111111
		0 0 3.5555555559999998
		0 0 4
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
		0 0 0.44444444440000003
		0 0 0.88888888889999995
		0 0 1.3333333329999999
		0 0 1.7777777779999999
		0 0 2.2222222220000001
		0 0 2.6666666669999999
		0 0 3.111111111
		0 0 3.5555555559999998
		0 0 4
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
		0 0 0.44444444440000003
		0 0 0.88888888889999995
		0 0 1.3333333329999999
		0 0 1.7777777779999999
		0 0 2.2222222220000001
		0 0 2.6666666669999999
		0 0 3.111111111
		0 0 3.5555555559999998
		0 0 4
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
		0 0 0.44444444440000003
		0 0 0.88888888889999995
		0 0 1.3333333329999999
		0 0 1.7777777779999999
		0 0 2.2222222220000001
		0 0 2.6666666669999999
		0 0 3.111111111
		0 0 3.5555555559999998
		0 0 4
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
		0 0 0.44444444440000003
		0 0 0.88888888889999995
		0 0 1.3333333329999999
		0 0 1.7777777779999999
		0 0 2.2222222220000001
		0 0 2.6666666669999999
		0 0 3.111111111
		0 0 3.5555555559999998
		0 0 4
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
		0 0 0.44444444440000003
		0 0 0.88888888889999995
		0 0 1.3333333329999999
		0 0 1.7777777779999999
		0 0 2.2222222220000001
		0 0 2.6666666669999999
		0 0 3.111111111
		0 0 3.5555555559999998
		0 0 4
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
		0 0 0.44444444440000003
		0 0 0.88888888889999995
		0 0 1.3333333329999999
		0 0 1.7777777779999999
		0 0 2.2222222220000001
		0 0 2.6666666669999999
		0 0 3.111111111
		0 0 3.5555555559999998
		0 0 4
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
		0 0 0.44444444440000003
		0 0 0.88888888889999995
		0 0 1.3333333329999999
		0 0 1.7777777779999999
		0 0 2.2222222220000001
		0 0 2.6666666669999999
		0 0 3.111111111
		0 0 3.5555555559999998
		0 0 4
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
		0 0 0.44444444440000003
		0 0 0.88888888889999995
		0 0 1.3333333329999999
		0 0 1.7777777779999999
		0 0 2.2222222220000001
		0 0 2.6666666669999999
		0 0 3.111111111
		0 0 3.5555555559999998
		0 0 4
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
		0 0 0.44444444440000003
		0 0 0.88888888889999995
		0 0 1.3333333329999999
		0 0 1.7777777779999999
		0 0 2.2222222220000001
		0 0 2.6666666669999999
		0 0 3.111111111
		0 0 3.5555555559999998
		0 0 4
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
		0 0 0.44444444440000003
		0 0 0.88888888889999995
		0 0 1.3333333329999999
		0 0 1.7777777779999999
		0 0 2.2222222220000001
		0 0 2.6666666669999999
		0 0 3.111111111
		0 0 3.5555555559999998
		0 0 4
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
		0 0 0.44444444440000003
		0 0 0.88888888889999995
		0 0 1.3333333329999999
		0 0 1.7777777779999999
		0 0 2.2222222220000001
		0 0 2.6666666669999999
		0 0 3.111111111
		0 0 3.5555555559999998
		0 0 4
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
		0 0 0.44444444440000003
		0 0 0.88888888889999995
		0 0 1.3333333329999999
		0 0 1.7777777779999999
		0 0 2.2222222220000001
		0 0 2.6666666669999999
		0 0 3.111111111
		0 0 3.5555555559999998
		0 0 4
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
		0 0 0.44444444440000003
		0 0 0.88888888889999995
		0 0 1.3333333329999999
		0 0 1.7777777779999999
		0 0 2.2222222220000001
		0 0 2.6666666669999999
		0 0 3.111111111
		0 0 3.5555555559999998
		0 0 4
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
		0 0 0.44444444440000003
		0 0 0.88888888889999995
		0 0 1.3333333329999999
		0 0 1.7777777779999999
		0 0 2.2222222220000001
		0 0 2.6666666669999999
		0 0 3.111111111
		0 0 3.5555555559999998
		0 0 4
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
		0 0 0.44444444440000003
		0 0 0.88888888889999995
		0 0 1.3333333329999999
		0 0 1.7777777779999999
		0 0 2.2222222220000001
		0 0 2.6666666669999999
		0 0 3.111111111
		0 0 3.5555555559999998
		0 0 4
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
		0 0 0.44444444440000003
		0 0 0.88888888889999995
		0 0 1.3333333329999999
		0 0 1.7777777779999999
		0 0 2.2222222220000001
		0 0 2.6666666669999999
		0 0 3.111111111
		0 0 3.5555555559999998
		0 0 4
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
		0 0 0.44444444440000003
		0 0 0.88888888889999995
		0 0 1.3333333329999999
		0 0 1.7777777779999999
		0 0 2.2222222220000001
		0 0 2.6666666669999999
		0 0 3.111111111
		0 0 3.5555555559999998
		0 0 4
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
		0 0 0.44444444440000003
		0 0 0.88888888889999995
		0 0 1.3333333329999999
		0 0 1.7777777779999999
		0 0 2.2222222220000001
		0 0 2.6666666669999999
		0 0 3.111111111
		0 0 3.5555555559999998
		0 0 4
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
		0 0 0.44444444440000003
		0 0 0.88888888889999995
		0 0 1.3333333329999999
		0 0 1.7777777779999999
		0 0 2.2222222220000001
		0 0 2.6666666669999999
		0 0 3.111111111
		0 0 3.5555555559999998
		0 0 4
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
		0 0 0.44444444440000003
		0 0 0.88888888889999995
		0 0 1.3333333329999999
		0 0 1.7777777779999999
		0 0 2.2222222220000001
		0 0 2.6666666669999999
		0 0 3.111111111
		0 0 3.5555555559999998
		0 0 4
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
		0 0 0.44444444440000003
		0 0 0.88888888889999995
		0 0 1.3333333329999999
		0 0 1.7777777779999999
		0 0 2.2222222220000001
		0 0 2.6666666669999999
		0 0 3.111111111
		0 0 3.5555555559999998
		0 0 4
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
		0 0 0.44444444440000003
		0 0 0.88888888889999995
		0 0 1.3333333329999999
		0 0 1.7777777779999999
		0 0 2.2222222220000001
		0 0 2.6666666669999999
		0 0 3.111111111
		0 0 3.5555555559999998
		0 0 4
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
		0 0 0.44444444440000003
		0 0 0.88888888889999995
		0 0 1.3333333329999999
		0 0 1.7777777779999999
		0 0 2.2222222220000001
		0 0 2.6666666669999999
		0 0 3.111111111
		0 0 3.5555555559999998
		0 0 4
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
		0 0 0.44444444440000003
		0 0 0.88888888889999995
		0 0 1.3333333329999999
		0 0 1.7777777779999999
		0 0 2.2222222220000001
		0 0 2.6666666669999999
		0 0 3.111111111
		0 0 3.5555555559999998
		0 0 4
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
		0 0 0.44444444440000003
		0 0 0.88888888889999995
		0 0 1.3333333329999999
		0 0 1.7777777779999999
		0 0 2.2222222220000001
		0 0 2.6666666669999999
		0 0 3.111111111
		0 0 3.5555555559999998
		0 0 4
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
		0 0 0.44444444440000003
		0 0 0.88888888889999995
		0 0 1.3333333329999999
		0 0 1.7777777779999999
		0 0 2.2222222220000001
		0 0 2.6666666669999999
		0 0 3.111111111
		0 0 3.5555555559999998
		0 0 4
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
		0 0 0.44444444440000003
		0 0 0.88888888889999995
		0 0 1.3333333329999999
		0 0 1.7777777779999999
		0 0 2.2222222220000001
		0 0 2.6666666669999999
		0 0 3.111111111
		0 0 3.5555555559999998
		0 0 4
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
		0 0 0.44444444440000003
		0 0 0.88888888889999995
		0 0 1.3333333329999999
		0 0 1.7777777779999999
		0 0 2.2222222220000001
		0 0 2.6666666669999999
		0 0 3.111111111
		0 0 3.5555555559999998
		0 0 4
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
		0 0 0.44444444440000003
		0 0 0.88888888889999995
		0 0 1.3333333329999999
		0 0 1.7777777779999999
		0 0 2.2222222220000001
		0 0 2.6666666669999999
		0 0 3.111111111
		0 0 3.5555555559999998
		0 0 4
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
		0 0 0.44444444440000003
		0 0 0.88888888889999995
		0 0 1.3333333329999999
		0 0 1.7777777779999999
		0 0 2.2222222220000001
		0 0 2.6666666669999999
		0 0 3.111111111
		0 0 3.5555555559999998
		0 0 4
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
		0 0 0.44444444440000003
		0 0 0.88888888889999995
		0 0 1.3333333329999999
		0 0 1.7777777779999999
		0 0 2.2222222220000001
		0 0 2.6666666669999999
		0 0 3.111111111
		0 0 3.5555555559999998
		0 0 4
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
		0 0 0.44444444440000003
		0 0 0.88888888889999995
		0 0 1.3333333329999999
		0 0 1.7777777779999999
		0 0 2.2222222220000001
		0 0 2.6666666669999999
		0 0 3.111111111
		0 0 3.5555555559999998
		0 0 4
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
		0 0 0.44444444440000003
		0 0 0.88888888889999995
		0 0 1.3333333329999999
		0 0 1.7777777779999999
		0 0 2.2222222220000001
		0 0 2.6666666669999999
		0 0 3.111111111
		0 0 3.5555555559999998
		0 0 4
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
		0 0 0.44444444440000003
		0 0 0.88888888889999995
		0 0 1.3333333329999999
		0 0 1.7777777779999999
		0 0 2.2222222220000001
		0 0 2.6666666669999999
		0 0 3.111111111
		0 0 3.5555555559999998
		0 0 4
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
		0 0 0.44444444440000003
		0 0 0.88888888889999995
		0 0 1.3333333329999999
		0 0 1.7777777779999999
		0 0 2.2222222220000001
		0 0 2.6666666669999999
		0 0 3.111111111
		0 0 3.5555555559999998
		0 0 4
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
		0 0 0.44444444440000003
		0 0 0.88888888889999995
		0 0 1.3333333329999999
		0 0 1.7777777779999999
		0 0 2.2222222220000001
		0 0 2.6666666669999999
		0 0 3.111111111
		0 0 3.5555555559999998
		0 0 4
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
		0 0 0.44444444440000003
		0 0 0.88888888889999995
		0 0 1.3333333329999999
		0 0 1.7777777779999999
		0 0 2.2222222220000001
		0 0 2.6666666669999999
		0 0 3.111111111
		0 0 3.5555555559999998
		0 0 4
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
		0 0 0.44444444440000003
		0 0 0.88888888889999995
		0 0 1.3333333329999999
		0 0 1.7777777779999999
		0 0 2.2222222220000001
		0 0 2.6666666669999999
		0 0 3.111111111
		0 0 3.5555555559999998
		0 0 4
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
		0 0 0.44444444440000003
		0 0 0.88888888889999995
		0 0 1.3333333329999999
		0 0 1.7777777779999999
		0 0 2.2222222220000001
		0 0 2.6666666669999999
		0 0 3.111111111
		0 0 3.5555555559999998
		0 0 4
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
		0 0 0.44444444440000003
		0 0 0.88888888889999995
		0 0 1.3333333329999999
		0 0 1.7777777779999999
		0 0 2.2222222220000001
		0 0 2.6666666669999999
		0 0 3.111111111
		0 0 3.5555555559999998
		0 0 4
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
		0 0 0.44444444440000003
		0 0 0.88888888889999995
		0 0 1.3333333329999999
		0 0 1.7777777779999999
		0 0 2.2222222220000001
		0 0 2.6666666669999999
		0 0 3.111111111
		0 0 3.5555555559999998
		0 0 4
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
		0 0 0.44444444440000003
		0 0 0.88888888889999995
		0 0 1.3333333329999999
		0 0 1.7777777779999999
		0 0 2.2222222220000001
		0 0 2.6666666669999999
		0 0 3.111111111
		0 0 3.5555555559999998
		0 0 4
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
		0 0 0.44444444440000003
		0 0 0.88888888889999995
		0 0 1.3333333329999999
		0 0 1.7777777779999999
		0 0 2.2222222220000001
		0 0 2.6666666669999999
		0 0 3.111111111
		0 0 3.5555555559999998
		0 0 4
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
		0 0 0.44444444440000003
		0 0 0.88888888889999995
		0 0 1.3333333329999999
		0 0 1.7777777779999999
		0 0 2.2222222220000001
		0 0 2.6666666669999999
		0 0 3.111111111
		0 0 3.5555555559999998
		0 0 4
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
		0 0 0.44444444440000003
		0 0 0.88888888889999995
		0 0 1.3333333329999999
		0 0 1.7777777779999999
		0 0 2.2222222220000001
		0 0 2.6666666669999999
		0 0 3.111111111
		0 0 3.5555555559999998
		0 0 4
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
		0 0 0.44444444440000003
		0 0 0.88888888889999995
		0 0 1.3333333329999999
		0 0 1.7777777779999999
		0 0 2.2222222220000001
		0 0 2.6666666669999999
		0 0 3.111111111
		0 0 3.5555555559999998
		0 0 4
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
		0 0 0.44444444440000003
		0 0 0.88888888889999995
		0 0 1.3333333329999999
		0 0 1.7777777779999999
		0 0 2.2222222220000001
		0 0 2.6666666669999999
		0 0 3.111111111
		0 0 3.5555555559999998
		0 0 4
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
		0 0 0.44444444440000003
		0 0 0.88888888889999995
		0 0 1.3333333329999999
		0 0 1.7777777779999999
		0 0 2.2222222220000001
		0 0 2.6666666669999999
		0 0 3.111111111
		0 0 3.5555555559999998
		0 0 4
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
		0 0 0.44444444440000003
		0 0 0.88888888889999995
		0 0 1.3333333329999999
		0 0 1.7777777779999999
		0 0 2.2222222220000001
		0 0 2.6666666669999999
		0 0 3.111111111
		0 0 3.5555555559999998
		0 0 4
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
		0 0 0.44444444440000003
		0 0 0.88888888889999995
		0 0 1.3333333329999999
		0 0 1.7777777779999999
		0 0 2.2222222220000001
		0 0 2.6666666669999999
		0 0 3.111111111
		0 0 3.5555555559999998
		0 0 4
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
		0 0 0.44444444440000003
		0 0 0.88888888889999995
		0 0 1.3333333329999999
		0 0 1.7777777779999999
		0 0 2.2222222220000001
		0 0 2.6666666669999999
		0 0 3.111111111
		0 0 3.5555555559999998
		0 0 4
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
		0 0 0.44444444440000003
		0 0 0.88888888889999995
		0 0 1.3333333329999999
		0 0 1.7777777779999999
		0 0 2.2222222220000001
		0 0 2.6666666669999999
		0 0 3.111111111
		0 0 3.5555555559999998
		0 0 4
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
		0 0 0.44444444440000003
		0 0 0.88888888889999995
		0 0 1.3333333329999999
		0 0 1.7777777779999999
		0 0 2.2222222220000001
		0 0 2.6666666669999999
		0 0 3.111111111
		0 0 3.5555555559999998
		0 0 4
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
		0 0 0.44444444440000003
		0 0 0.88888888889999995
		0 0 1.3333333329999999
		0 0 1.7777777779999999
		0 0 2.2222222220000001
		0 0 2.6666666669999999
		0 0 3.111111111
		0 0 3.5555555559999998
		0 0 4
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
		0 0 0.44444444440000003
		0 0 0.88888888889999995
		0 0 1.3333333329999999
		0 0 1.7777777779999999
		0 0 2.2222222220000001
		0 0 2.6666666669999999
		0 0 3.111111111
		0 0 3.5555555559999998
		0 0 4
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
		0 0 0.44444444440000003
		0 0 0.88888888889999995
		0 0 1.3333333329999999
		0 0 1.7777777779999999
		0 0 2.2222222220000001
		0 0 2.6666666669999999
		0 0 3.111111111
		0 0 3.5555555559999998
		0 0 4
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
		0 0 0.44444444440000003
		0 0 0.88888888889999995
		0 0 1.3333333329999999
		0 0 1.7777777779999999
		0 0 2.2222222220000001
		0 0 2.6666666669999999
		0 0 3.111111111
		0 0 3.5555555559999998
		0 0 4
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
		0 0 0.44444444440000003
		0 0 0.88888888889999995
		0 0 1.3333333329999999
		0 0 1.7777777779999999
		0 0 2.2222222220000001
		0 0 2.6666666669999999
		0 0 3.111111111
		0 0 3.5555555559999998
		0 0 4
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
		0 0 0.44444444440000003
		0 0 0.88888888889999995
		0 0 1.3333333329999999
		0 0 1.7777777779999999
		0 0 2.2222222220000001
		0 0 2.6666666669999999
		0 0 3.111111111
		0 0 3.5555555559999998
		0 0 4
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
		0 0 0.44444444440000003
		0 0 0.88888888889999995
		0 0 1.3333333329999999
		0 0 1.7777777779999999
		0 0 2.2222222220000001
		0 0 2.6666666669999999
		0 0 3.111111111
		0 0 3.5555555559999998
		0 0 4
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
		0 0 0.44444444440000003
		0 0 0.88888888889999995
		0 0 1.3333333329999999
		0 0 1.7777777779999999
		0 0 2.2222222220000001
		0 0 2.6666666669999999
		0 0 3.111111111
		0 0 3.5555555559999998
		0 0 4
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
		0 0 0.44444444440000003
		0 0 0.88888888889999995
		0 0 1.3333333329999999
		0 0 1.7777777779999999
		0 0 2.2222222220000001
		0 0 2.6666666669999999
		0 0 3.111111111
		0 0 3.5555555559999998
		0 0 4
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
		0 0 0.44444444440000003
		0 0 0.88888888889999995
		0 0 1.3333333329999999
		0 0 1.7777777779999999
		0 0 2.2222222220000001
		0 0 2.6666666669999999
		0 0 3.111111111
		0 0 3.5555555559999998
		0 0 4
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
		0 0 0.44444444440000003
		0 0 0.88888888889999995
		0 0 1.3333333329999999
		0 0 1.7777777779999999
		0 0 2.2222222220000001
		0 0 2.6666666669999999
		0 0 3.111111111
		0 0 3.5555555559999998
		0 0 4
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
		0 0 0.44444444440000003
		0 0 0.88888888889999995
		0 0 1.3333333329999999
		0 0 1.7777777779999999
		0 0 2.2222222220000001
		0 0 2.6666666669999999
		0 0 3.111111111
		0 0 3.5555555559999998
		0 0 4
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
		0 0 0.44444444440000003
		0 0 0.88888888889999995
		0 0 1.3333333329999999
		0 0 1.7777777779999999
		0 0 2.2222222220000001
		0 0 2.6666666669999999
		0 0 3.111111111
		0 0 3.5555555559999998
		0 0 4
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
		0 0 0.44444444440000003
		0 0 0.88888888889999995
		0 0 1.3333333329999999
		0 0 1.7777777779999999
		0 0 2.2222222220000001
		0 0 2.6666666669999999
		0 0 3.111111111
		0 0 3.5555555559999998
		0 0 4
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
		0 0 0.44444444440000003
		0 0 0.88888888889999995
		0 0 1.3333333329999999
		0 0 1.7777777779999999
		0 0 2.2222222220000001
		0 0 2.6666666669999999
		0 0 3.111111111
		0 0 3.5555555559999998
		0 0 4
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
		0 0 0.44444444440000003
		0 0 0.88888888889999995
		0 0 1.3333333329999999
		0 0 1.7777777779999999
		0 0 2.2222222220000001
		0 0 2.6666666669999999
		0 0 3.111111111
		0 0 3.5555555559999998
		0 0 4
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
		0 0 0.44444444440000003
		0 0 0.88888888889999995
		0 0 1.3333333329999999
		0 0 1.7777777779999999
		0 0 2.2222222220000001
		0 0 2.6666666669999999
		0 0 3.111111111
		0 0 3.5555555559999998
		0 0 4
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
		0 0 0.44444444440000003
		0 0 0.88888888889999995
		0 0 1.3333333329999999
		0 0 1.7777777779999999
		0 0 2.2222222220000001
		0 0 2.6666666669999999
		0 0 3.111111111
		0 0 3.5555555559999998
		0 0 4
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
		0 0 0.44444444440000003
		0 0 0.88888888889999995
		0 0 1.3333333329999999
		0 0 1.7777777779999999
		0 0 2.2222222220000001
		0 0 2.6666666669999999
		0 0 3.111111111
		0 0 3.5555555559999998
		0 0 4
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
		0 0 0.44444444440000003
		0 0 0.88888888889999995
		0 0 1.3333333329999999
		0 0 1.7777777779999999
		0 0 2.2222222220000001
		0 0 2.6666666669999999
		0 0 3.111111111
		0 0 3.5555555559999998
		0 0 4
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
		0 0 0.44444444440000003
		0 0 0.88888888889999995
		0 0 1.3333333329999999
		0 0 1.7777777779999999
		0 0 2.2222222220000001
		0 0 2.6666666669999999
		0 0 3.111111111
		0 0 3.5555555559999998
		0 0 4
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
		0 0 0.44444444440000003
		0 0 0.88888888889999995
		0 0 1.3333333329999999
		0 0 1.7777777779999999
		0 0 2.2222222220000001
		0 0 2.6666666669999999
		0 0 3.111111111
		0 0 3.5555555559999998
		0 0 4
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
		0 0 0.44444444440000003
		0 0 0.88888888889999995
		0 0 1.3333333329999999
		0 0 1.7777777779999999
		0 0 2.2222222220000001
		0 0 2.6666666669999999
		0 0 3.111111111
		0 0 3.5555555559999998
		0 0 4
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
		0 0 0.44444444440000003
		0 0 0.88888888889999995
		0 0 1.3333333329999999
		0 0 1.7777777779999999
		0 0 2.2222222220000001
		0 0 2.6666666669999999
		0 0 3.111111111
		0 0 3.5555555559999998
		0 0 4
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
		0 0 0.44444444440000003
		0 0 0.88888888889999995
		0 0 1.3333333329999999
		0 0 1.7777777779999999
		0 0 2.2222222220000001
		0 0 2.6666666669999999
		0 0 3.111111111
		0 0 3.5555555559999998
		0 0 4
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
		0 0 0.44444444440000003
		0 0 0.88888888889999995
		0 0 1.3333333329999999
		0 0 1.7777777779999999
		0 0 2.2222222220000001
		0 0 2.6666666669999999
		0 0 3.111111111
		0 0 3.5555555559999998
		0 0 4
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
		0 0 0.44444444440000003
		0 0 0.88888888889999995
		0 0 1.3333333329999999
		0 0 1.7777777779999999
		0 0 2.2222222220000001
		0 0 2.6666666669999999
		0 0 3.111111111
		0 0 3.5555555559999998
		0 0 4
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
		0 0 0.44444444440000003
		0 0 0.88888888889999995
		0 0 1.3333333329999999
		0 0 1.7777777779999999
		0 0 2.2222222220000001
		0 0 2.6666666669999999
		0 0 3.111111111
		0 0 3.5555555559999998
		0 0 4
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
		0 0 0.44444444440000003
		0 0 0.88888888889999995
		0 0 1.3333333329999999
		0 0 1.7777777779999999
		0 0 2.2222222220000001
		0 0 2.6666666669999999
		0 0 3.111111111
		0 0 3.5555555559999998
		0 0 4
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
		0 0 0.44444444440000003
		0 0 0.88888888889999995
		0 0 1.3333333329999999
		0 0 1.7777777779999999
		0 0 2.2222222220000001
		0 0 2.6666666669999999
		0 0 3.111111111
		0 0 3.5555555559999998
		0 0 4
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
		0 0 0.44444444440000003
		0 0 0.88888888889999995
		0 0 1.3333333329999999
		0 0 1.7777777779999999
		0 0 2.2222222220000001
		0 0 2.6666666669999999
		0 0 3.111111111
		0 0 3.5555555559999998
		0 0 4
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
		0 0 0.44444444440000003
		0 0 0.88888888889999995
		0 0 1.3333333329999999
		0 0 1.7777777779999999
		0 0 2.2222222220000001
		0 0 2.6666666669999999
		0 0 3.111111111
		0 0 3.5555555559999998
		0 0 4
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
		0 0 0.44444444440000003
		0 0 0.88888888889999995
		0 0 1.3333333329999999
		0 0 1.7777777779999999
		0 0 2.2222222220000001
		0 0 2.6666666669999999
		0 0 3.111111111
		0 0 3.5555555559999998
		0 0 4
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
		0 0 0.44444444440000003
		0 0 0.88888888889999995
		0 0 1.3333333329999999
		0 0 1.7777777779999999
		0 0 2.2222222220000001
		0 0 2.6666666669999999
		0 0 3.111111111
		0 0 3.5555555559999998
		0 0 4
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
		0 0 0.44444444440000003
		0 0 0.88888888889999995
		0 0 1.3333333329999999
		0 0 1.7777777779999999
		0 0 2.2222222220000001
		0 0 2.6666666669999999
		0 0 3.111111111
		0 0 3.5555555559999998
		0 0 4
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
		0 0 0.44444444440000003
		0 0 0.88888888889999995
		0 0 1.3333333329999999
		0 0 1.7777777779999999
		0 0 2.2222222220000001
		0 0 2.6666666669999999
		0 0 3.111111111
		0 0 3.5555555559999998
		0 0 4
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
		0 0 0.44444444440000003
		0 0 0.88888888889999995
		0 0 1.3333333329999999
		0 0 1.7777777779999999
		0 0 2.2222222220000001
		0 0 2.6666666669999999
		0 0 3.111111111
		0 0 3.5555555559999998
		0 0 4
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
		0 0 0.44444444440000003
		0 0 0.88888888889999995
		0 0 1.3333333329999999
		0 0 1.7777777779999999
		0 0 2.2222222220000001
		0 0 2.6666666669999999
		0 0 3.111111111
		0 0 3.5555555559999998
		0 0 4
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
		0 0 0.44444444440000003
		0 0 0.88888888889999995
		0 0 1.3333333329999999
		0 0 1.7777777779999999
		0 0 2.2222222220000001
		0 0 2.6666666669999999
		0 0 3.111111111
		0 0 3.5555555559999998
		0 0 4
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
		0 0 0.44444444440000003
		0 0 0.88888888889999995
		0 0 1.3333333329999999
		0 0 1.7777777779999999
		0 0 2.2222222220000001
		0 0 2.6666666669999999
		0 0 3.111111111
		0 0 3.5555555559999998
		0 0 4
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
		0 0 0.44444444440000003
		0 0 0.88888888889999995
		0 0 1.3333333329999999
		0 0 1.7777777779999999
		0 0 2.2222222220000001
		0 0 2.6666666669999999
		0 0 3.111111111
		0 0 3.5555555559999998
		0 0 4
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
		0 0 0.44444444440000003
		0 0 0.88888888889999995
		0 0 1.3333333329999999
		0 0 1.7777777779999999
		0 0 2.2222222220000001
		0 0 2.6666666669999999
		0 0 3.111111111
		0 0 3.5555555559999998
		0 0 4
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
		0 0 0.44444444440000003
		0 0 0.88888888889999995
		0 0 1.3333333329999999
		0 0 1.7777777779999999
		0 0 2.2222222220000001
		0 0 2.6666666669999999
		0 0 3.111111111
		0 0 3.5555555559999998
		0 0 4
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
		0 0 0.44444444440000003
		0 0 0.88888888889999995
		0 0 1.3333333329999999
		0 0 1.7777777779999999
		0 0 2.2222222220000001
		0 0 2.6666666669999999
		0 0 3.111111111
		0 0 3.5555555559999998
		0 0 4
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
		0 0 0.44444444440000003
		0 0 0.88888888889999995
		0 0 1.3333333329999999
		0 0 1.7777777779999999
		0 0 2.2222222220000001
		0 0 2.6666666669999999
		0 0 3.111111111
		0 0 3.5555555559999998
		0 0 4
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
		0 0 0.44444444440000003
		0 0 0.88888888889999995
		0 0 1.3333333329999999
		0 0 1.7777777779999999
		0 0 2.2222222220000001
		0 0 2.6666666669999999
		0 0 3.111111111
		0 0 3.5555555559999998
		0 0 4
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
		0 0 0.44444444440000003
		0 0 0.88888888889999995
		0 0 1.3333333329999999
		0 0 1.7777777779999999
		0 0 2.2222222220000001
		0 0 2.6666666669999999
		0 0 3.111111111
		0 0 3.5555555559999998
		0 0 4
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
		0 0 0.44444444440000003
		0 0 0.88888888889999995
		0 0 1.3333333329999999
		0 0 1.7777777779999999
		0 0 2.2222222220000001
		0 0 2.6666666669999999
		0 0 3.111111111
		0 0 3.5555555559999998
		0 0 4
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
		0 0 0.44444444440000003
		0 0 0.88888888889999995
		0 0 1.3333333329999999
		0 0 1.7777777779999999
		0 0 2.2222222220000001
		0 0 2.6666666669999999
		0 0 3.111111111
		0 0 3.5555555559999998
		0 0 4
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
		0 0 0.44444444440000003
		0 0 0.88888888889999995
		0 0 1.3333333329999999
		0 0 1.7777777779999999
		0 0 2.2222222220000001
		0 0 2.6666666669999999
		0 0 3.111111111
		0 0 3.5555555559999998
		0 0 4
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
		0 0 0.44444444440000003
		0 0 0.88888888889999995
		0 0 1.3333333329999999
		0 0 1.7777777779999999
		0 0 2.2222222220000001
		0 0 2.6666666669999999
		0 0 3.111111111
		0 0 3.5555555559999998
		0 0 4
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
		0 0 0.44444444440000003
		0 0 0.88888888889999995
		0 0 1.3333333329999999
		0 0 1.7777777779999999
		0 0 2.2222222220000001
		0 0 2.6666666669999999
		0 0 3.111111111
		0 0 3.5555555559999998
		0 0 4
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
		0 0 0.44444444440000003
		0 0 0.88888888889999995
		0 0 1.3333333329999999
		0 0 1.7777777779999999
		0 0 2.2222222220000001
		0 0 2.6666666669999999
		0 0 3.111111111
		0 0 3.5555555559999998
		0 0 4
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
		0 0 0.44444444440000003
		0 0 0.88888888889999995
		0 0 1.3333333329999999
		0 0 1.7777777779999999
		0 0 2.2222222220000001
		0 0 2.6666666669999999
		0 0 3.111111111
		0 0 3.5555555559999998
		0 0 4
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
		0 0 0.44444444440000003
		0 0 0.88888888889999995
		0 0 1.3333333329999999
		0 0 1.7777777779999999
		0 0 2.2222222220000001
		0 0 2.6666666669999999
		0 0 3.111111111
		0 0 3.5555555559999998
		0 0 4
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
		0 0 0.44444444440000003
		0 0 0.88888888889999995
		0 0 1.3333333329999999
		0 0 1.7777777779999999
		0 0 2.2222222220000001
		0 0 2.6666666669999999
		0 0 3.111111111
		0 0 3.5555555559999998
		0 0 4
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
		0 0 0.44444444440000003
		0 0 0.88888888889999995
		0 0 1.3333333329999999
		0 0 1.7777777779999999
		0 0 2.2222222220000001
		0 0 2.6666666669999999
		0 0 3.111111111
		0 0 3.5555555559999998
		0 0 4
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
		0 0 0.44444444440000003
		0 0 0.88888888889999995
		0 0 1.3333333329999999
		0 0 1.7777777779999999
		0 0 2.2222222220000001
		0 0 2.6666666669999999
		0 0 3.111111111
		0 0 3.5555555559999998
		0 0 4
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
		0 0 0.44444444440000003
		0 0 0.88888888889999995
		0 0 1.3333333329999999
		0 0 1.7777777779999999
		0 0 2.2222222220000001
		0 0 2.6666666669999999
		0 0 3.111111111
		0 0 3.5555555559999998
		0 0 4
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
		0 0 0.44444444440000003
		0 0 0.88888888889999995
		0 0 1.3333333329999999
		0 0 1.7777777779999999
		0 0 2.2222222220000001
		0 0 2.6666666669999999
		0 0 3.111111111
		0 0 3.5555555559999998
		0 0 4
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
		0 0 0.44444444440000003
		0 0 0.88888888889999995
		0 0 1.3333333329999999
		0 0 1.7777777779999999
		0 0 2.2222222220000001
		0 0 2.6666666669999999
		0 0 3.111111111
		0 0 3.5555555559999998
		0 0 4
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
		0 0 0.44444444440000003
		0 0 0.88888888889999995
		0 0 1.3333333329999999
		0 0 1.7777777779999999
		0 0 2.2222222220000001
		0 0 2.6666666669999999
		0 0 3.111111111
		0 0 3.5555555559999998
		0 0 4
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
		0 0 0.44444444440000003
		0 0 0.88888888889999995
		0 0 1.3333333329999999
		0 0 1.7777777779999999
		0 0 2.2222222220000001
		0 0 2.6666666669999999
		0 0 3.111111111
		0 0 3.5555555559999998
		0 0 4
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
		0 0 0.44444444440000003
		0 0 0.88888888889999995
		0 0 1.3333333329999999
		0 0 1.7777777779999999
		0 0 2.2222222220000001
		0 0 2.6666666669999999
		0 0 3.111111111
		0 0 3.5555555559999998
		0 0 4
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
		0 0 0.44444444440000003
		0 0 0.88888888889999995
		0 0 1.3333333329999999
		0 0 1.7777777779999999
		0 0 2.2222222220000001
		0 0 2.6666666669999999
		0 0 3.111111111
		0 0 3.5555555559999998
		0 0 4
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
		0 0 0.44444444440000003
		0 0 0.88888888889999995
		0 0 1.3333333329999999
		0 0 1.7777777779999999
		0 0 2.2222222220000001
		0 0 2.6666666669999999
		0 0 3.111111111
		0 0 3.5555555559999998
		0 0 4
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
		0 0 0.44444444440000003
		0 0 0.88888888889999995
		0 0 1.3333333329999999
		0 0 1.7777777779999999
		0 0 2.2222222220000001
		0 0 2.6666666669999999
		0 0 3.111111111
		0 0 3.5555555559999998
		0 0 4
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
		0 0 0.44444444440000003
		0 0 0.88888888889999995
		0 0 1.3333333329999999
		0 0 1.7777777779999999
		0 0 2.2222222220000001
		0 0 2.6666666669999999
		0 0 3.111111111
		0 0 3.5555555559999998
		0 0 4
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
		0 0 0.44444444440000003
		0 0 0.88888888889999995
		0 0 1.3333333329999999
		0 0 1.7777777779999999
		0 0 2.2222222220000001
		0 0 2.6666666669999999
		0 0 3.111111111
		0 0 3.5555555559999998
		0 0 4
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
		0 0 0.44444444440000003
		0 0 0.88888888889999995
		0 0 1.3333333329999999
		0 0 1.7777777779999999
		0 0 2.2222222220000001
		0 0 2.6666666669999999
		0 0 3.111111111
		0 0 3.5555555559999998
		0 0 4
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
		0 0 0.44444444440000003
		0 0 0.88888888889999995
		0 0 1.3333333329999999
		0 0 1.7777777779999999
		0 0 2.2222222220000001
		0 0 2.6666666669999999
		0 0 3.111111111
		0 0 3.5555555559999998
		0 0 4
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
		0 0 0.44444444440000003
		0 0 0.88888888889999995
		0 0 1.3333333329999999
		0 0 1.7777777779999999
		0 0 2.2222222220000001
		0 0 2.6666666669999999
		0 0 3.111111111
		0 0 3.5555555559999998
		0 0 4
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
		0 0 0.44444444440000003
		0 0 0.88888888889999995
		0 0 1.3333333329999999
		0 0 1.7777777779999999
		0 0 2.2222222220000001
		0 0 2.6666666669999999
		0 0 3.111111111
		0 0 3.5555555559999998
		0 0 4
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
		0 0 0.44444444440000003
		0 0 0.88888888889999995
		0 0 1.3333333329999999
		0 0 1.7777777779999999
		0 0 2.2222222220000001
		0 0 2.6666666669999999
		0 0 3.111111111
		0 0 3.5555555559999998
		0 0 4
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
		0 0 0.44444444440000003
		0 0 0.88888888889999995
		0 0 1.3333333329999999
		0 0 1.7777777779999999
		0 0 2.2222222220000001
		0 0 2.6666666669999999
		0 0 3.111111111
		0 0 3.5555555559999998
		0 0 4
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
		0 0 0.44444444440000003
		0 0 0.88888888889999995
		0 0 1.3333333329999999
		0 0 1.7777777779999999
		0 0 2.2222222220000001
		0 0 2.6666666669999999
		0 0 3.111111111
		0 0 3.5555555559999998
		0 0 4
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
		0 0 0.44444444440000003
		0 0 0.88888888889999995
		0 0 1.3333333329999999
		0 0 1.7777777779999999
		0 0 2.2222222220000001
		0 0 2.6666666669999999
		0 0 3.111111111
		0 0 3.5555555559999998
		0 0 4
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
		0 0 0.44444444440000003
		0 0 0.88888888889999995
		0 0 1.3333333329999999
		0 0 1.7777777779999999
		0 0 2.2222222220000001
		0 0 2.6666666669999999
		0 0 3.111111111
		0 0 3.5555555559999998
		0 0 4
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
		0 0 0.44444444440000003
		0 0 0.88888888889999995
		0 0 1.3333333329999999
		0 0 1.7777777779999999
		0 0 2.2222222220000001
		0 0 2.6666666669999999
		0 0 3.111111111
		0 0 3.5555555559999998
		0 0 4
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
		0 0 0.44444444440000003
		0 0 0.88888888889999995
		0 0 1.3333333329999999
		0 0 1.7777777779999999
		0 0 2.2222222220000001
		0 0 2.6666666669999999
		0 0 3.111111111
		0 0 3.5555555559999998
		0 0 4
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
		0 0 0.44444444440000003
		0 0 0.88888888889999995
		0 0 1.3333333329999999
		0 0 1.7777777779999999
		0 0 2.2222222220000001
		0 0 2.6666666669999999
		0 0 3.111111111
		0 0 3.5555555559999998
		0 0 4
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
		0 0 0.44444444440000003
		0 0 0.88888888889999995
		0 0 1.3333333329999999
		0 0 1.7777777779999999
		0 0 2.2222222220000001
		0 0 2.6666666669999999
		0 0 3.111111111
		0 0 3.5555555559999998
		0 0 4
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
		0 0 0.44444444440000003
		0 0 0.88888888889999995
		0 0 1.3333333329999999
		0 0 1.7777777779999999
		0 0 2.2222222220000001
		0 0 2.6666666669999999
		0 0 3.111111111
		0 0 3.5555555559999998
		0 0 4
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
		0 0 0.44444444440000003
		0 0 0.88888888889999995
		0 0 1.3333333329999999
		0 0 1.7777777779999999
		0 0 2.2222222220000001
		0 0 2.6666666669999999
		0 0 3.111111111
		0 0 3.5555555559999998
		0 0 4
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
		0 0 0.44444444440000003
		0 0 0.88888888889999995
		0 0 1.3333333329999999
		0 0 1.7777777779999999
		0 0 2.2222222220000001
		0 0 2.6666666669999999
		0 0 3.111111111
		0 0 3.5555555559999998
		0 0 4
		;
createNode transform -n "pfxHair1";
	setAttr ".v" no;
createNode pfxHair -n "pfxHairShape1" -p "pfxHair1";
	setAttr -k off ".v";
	setAttr ".dpc" 100;
	setAttr ".dam" no;
createNode nucleus -n "nucleus1";
createNode transform -n "directionalLight1";
	setAttr ".r" -type "double3" -48.156552041713944 1.3059010322141331 -9.5202672381429512 ;
	setAttr ".s" -type "double3" 30.769077416337016 30.769077416337016 30.769077416337016 ;
createNode directionalLight -n "directionalLightShape1" -p "directionalLight1";
	setAttr -k off ".v";
createNode transform -n "transform1103";
createNode transform -n "transform1104";
createNode transform -n "transform1105";
createNode transform -n "transform1106";
createNode transform -n "transform1107";
createNode transform -n "transform1108";
createNode transform -n "transform1109";
createNode transform -n "transform1110";
createNode transform -n "transform1111";
createNode transform -n "transform1112";
createNode transform -n "transform1113";
createNode transform -n "transform1114";
createNode transform -n "transform1115";
createNode transform -n "transform1116";
createNode transform -n "transform1117";
createNode transform -n "transform1118";
createNode transform -n "transform1119";
createNode transform -n "transform1120";
createNode transform -n "transform1121";
createNode transform -n "transform1122";
createNode transform -n "transform1123";
createNode transform -n "transform1124";
createNode transform -n "transform1125";
createNode transform -n "transform1126";
createNode transform -n "transform1127";
createNode transform -n "transform1128";
createNode transform -n "transform1129";
createNode transform -n "transform1130";
createNode transform -n "transform1131";
createNode transform -n "transform1132";
createNode transform -n "transform1133";
createNode transform -n "transform1134";
createNode transform -n "transform1135";
createNode transform -n "transform1136";
createNode transform -n "transform1137";
createNode transform -n "transform1138";
createNode transform -n "transform1139";
createNode transform -n "transform1140";
createNode transform -n "transform1141";
createNode transform -n "transform1142";
createNode transform -n "transform1143";
createNode transform -n "transform1144";
createNode transform -n "transform1145";
createNode transform -n "transform1146";
createNode transform -n "transform1147";
createNode transform -n "transform1148";
createNode transform -n "transform1149";
createNode transform -n "transform1150";
createNode transform -n "transform1151";
createNode transform -n "transform1152";
createNode transform -n "transform1153";
createNode transform -n "transform1154";
createNode transform -n "transform1155";
createNode transform -n "transform1156";
createNode transform -n "transform1157";
createNode transform -n "transform1158";
createNode transform -n "transform1159";
createNode transform -n "transform1160";
createNode transform -n "transform1161";
createNode transform -n "transform1162";
createNode transform -n "transform1163";
createNode transform -n "transform1164";
createNode transform -n "transform1165";
createNode transform -n "transform1166";
createNode transform -n "transform1167";
createNode transform -n "transform1168";
createNode transform -n "transform1169";
createNode transform -n "transform1170";
createNode transform -n "transform1171";
createNode transform -n "transform1172";
createNode transform -n "transform1173";
createNode transform -n "transform1174";
createNode transform -n "transform1175";
createNode transform -n "transform1176";
createNode transform -n "transform1177";
createNode transform -n "transform1178";
createNode transform -n "transform1179";
createNode transform -n "transform1180";
createNode transform -n "transform1181";
createNode transform -n "transform1182";
createNode transform -n "transform1183";
createNode transform -n "transform1184";
createNode transform -n "transform1185";
createNode transform -n "transform1186";
createNode transform -n "transform1187";
createNode transform -n "transform1188";
createNode transform -n "transform1189";
createNode transform -n "transform1190";
createNode transform -n "transform1191";
createNode transform -n "transform1192";
createNode transform -n "transform1193";
createNode transform -n "transform1194";
createNode transform -n "transform1195";
createNode transform -n "transform1196";
createNode transform -n "transform1197";
createNode transform -n "transform1198";
createNode transform -n "transform1199";
createNode transform -n "transform1200";
createNode transform -n "transform1201";
createNode transform -n "transform1202";
createNode transform -n "transform1203";
createNode transform -n "transform1204";
createNode transform -n "transform1205";
createNode transform -n "transform1206";
createNode transform -n "transform1207";
createNode transform -n "transform1208";
createNode transform -n "transform1209";
createNode transform -n "transform1210";
createNode transform -n "transform1211";
createNode transform -n "transform1212";
createNode transform -n "transform1213";
createNode transform -n "transform1214";
createNode transform -n "transform1215";
createNode transform -n "transform1216";
createNode transform -n "transform1217";
createNode transform -n "transform1218";
createNode transform -n "transform1219";
createNode transform -n "transform1220";
createNode transform -n "transform1221";
createNode transform -n "transform1222";
createNode transform -n "transform1223";
createNode transform -n "transform1224";
createNode transform -n "transform1225";
createNode transform -n "transform1226";
createNode transform -n "transform1227";
createNode transform -n "transform1228";
createNode transform -n "transform1229";
createNode transform -n "transform1230";
createNode transform -n "transform1231";
createNode transform -n "transform1232";
createNode transform -n "transform1233";
createNode transform -n "transform1234";
createNode transform -n "transform1235";
createNode transform -n "transform1236";
createNode transform -n "transform1237";
createNode transform -n "transform1238";
createNode transform -n "transform1239";
createNode transform -n "transform1240";
createNode transform -n "transform1241";
createNode transform -n "transform1242";
createNode transform -n "transform1243";
createNode transform -n "transform1244";
createNode transform -n "transform1245";
createNode transform -n "transform1246";
createNode transform -n "transform1247";
createNode transform -n "transform1248";
createNode transform -n "transform1249";
createNode transform -n "transform1250";
createNode transform -n "transform1251";
createNode transform -n "transform1252";
createNode transform -n "transform1253";
createNode transform -n "transform1254";
createNode transform -n "transform1255";
createNode transform -n "transform1256";
createNode transform -n "transform1257";
createNode transform -n "transform1258";
createNode transform -n "transform1259";
createNode transform -n "transform1260";
createNode transform -n "transform1261";
createNode transform -n "transform1262";
createNode transform -n "transform1263";
createNode transform -n "transform1264";
createNode transform -n "transform1265";
createNode transform -n "transform1266";
createNode transform -n "transform1267";
createNode transform -n "transform1268";
createNode transform -n "transform1269";
createNode transform -n "transform1270";
createNode transform -n "transform1271";
createNode transform -n "transform1272";
createNode transform -n "transform1273";
createNode transform -n "transform1274";
createNode transform -n "transform1275";
createNode transform -n "transform1276";
createNode lightLinker -s -n "lightLinker1";
	setAttr -s 5 ".lnk";
	setAttr -s 5 ".slnk";
createNode displayLayerManager -n "layerManager";
createNode displayLayer -n "defaultLayer";
createNode renderLayerManager -n "renderLayerManager";
createNode renderLayer -n "defaultRenderLayer";
	setAttr ".g" yes;
createNode aiOptions -n "aiOptions464";
createNode aiOptions -n "aiOptions463";
createNode aiOptions -n "aiOptions462";
createNode aiOptions -n "aiOptions461";
createNode aiOptions -n "aiOptions460";
createNode aiOptions -n "aiOptions459";
createNode aiOptions -n "aiOptions458";
createNode aiOptions -n "aiOptions457";
createNode aiOptions -n "aiOptions456";
createNode aiOptions -n "aiOptions455";
createNode aiOptions -n "aiOptions454";
createNode aiOptions -n "aiOptions453";
createNode aiOptions -n "aiOptions452";
createNode aiOptions -n "aiOptions451";
createNode aiOptions -n "aiOptions450";
createNode aiOptions -n "aiOptions449";
createNode aiOptions -n "aiOptions448";
createNode aiOptions -n "aiOptions447";
createNode aiOptions -n "aiOptions446";
createNode aiOptions -n "aiOptions445";
createNode aiOptions -n "aiOptions444";
createNode aiOptions -n "aiOptions443";
createNode aiOptions -n "aiOptions442";
createNode aiOptions -n "aiOptions441";
createNode aiOptions -n "aiOptions440";
createNode aiOptions -n "aiOptions439";
createNode aiOptions -n "aiOptions438";
createNode aiOptions -n "aiOptions437";
createNode aiOptions -n "aiOptions436";
createNode aiOptions -n "aiOptions435";
createNode aiOptions -n "aiOptions434";
createNode aiOptions -n "aiOptions433";
createNode aiOptions -n "aiOptions432";
createNode aiOptions -n "aiOptions431";
createNode aiOptions -n "aiOptions430";
createNode aiOptions -n "aiOptions429";
createNode aiOptions -n "aiOptions428";
createNode aiOptions -n "aiOptions427";
createNode aiOptions -n "aiOptions426";
createNode aiOptions -n "aiOptions425";
createNode aiOptions -n "aiOptions424";
createNode aiOptions -n "aiOptions423";
createNode aiOptions -n "aiOptions422";
createNode aiOptions -n "aiOptions421";
createNode aiOptions -n "aiOptions420";
createNode aiOptions -n "aiOptions419";
createNode aiOptions -n "aiOptions418";
createNode aiOptions -n "aiOptions417";
createNode aiOptions -n "aiOptions416";
createNode aiOptions -n "aiOptions415";
createNode aiOptions -n "aiOptions414";
createNode aiOptions -n "aiOptions413";
createNode aiOptions -n "aiOptions412";
createNode aiOptions -n "aiOptions411";
createNode aiOptions -n "aiOptions410";
createNode aiOptions -n "aiOptions409";
createNode aiOptions -n "aiOptions408";
createNode aiOptions -n "aiOptions407";
createNode aiOptions -n "aiOptions406";
createNode aiOptions -n "aiOptions405";
createNode aiOptions -n "aiOptions404";
createNode aiOptions -n "aiOptions403";
createNode aiOptions -n "aiOptions402";
createNode aiOptions -n "aiOptions401";
createNode aiOptions -n "aiOptions400";
createNode aiOptions -n "aiOptions399";
createNode aiOptions -n "aiOptions398";
createNode aiOptions -n "aiOptions397";
createNode aiOptions -n "aiOptions396";
createNode aiOptions -n "aiOptions395";
createNode aiOptions -n "aiOptions394";
createNode aiOptions -n "aiOptions393";
createNode aiOptions -n "aiOptions392";
createNode aiOptions -n "aiOptions391";
createNode aiOptions -n "aiOptions390";
createNode aiOptions -n "aiOptions389";
createNode aiOptions -n "aiOptions388";
createNode aiOptions -n "aiOptions387";
createNode aiOptions -n "aiOptions386";
createNode aiOptions -n "aiOptions385";
createNode aiOptions -n "aiOptions384";
createNode aiOptions -n "aiOptions383";
createNode aiOptions -n "aiOptions382";
createNode aiOptions -n "aiOptions381";
createNode aiOptions -n "aiOptions380";
createNode aiOptions -n "aiOptions379";
createNode aiOptions -n "aiOptions378";
createNode aiOptions -n "aiOptions377";
createNode aiOptions -n "aiOptions376";
createNode aiOptions -n "aiOptions375";
createNode aiOptions -n "aiOptions374";
createNode aiOptions -n "aiOptions373";
createNode aiOptions -n "aiOptions372";
createNode aiOptions -n "aiOptions371";
createNode aiOptions -n "aiOptions370";
createNode aiOptions -n "aiOptions369";
createNode aiOptions -n "aiOptions368";
createNode aiOptions -n "aiOptions367";
createNode aiOptions -n "aiOptions366";
createNode aiOptions -n "aiOptions365";
createNode aiOptions -n "aiOptions364";
createNode aiOptions -n "aiOptions363";
createNode aiOptions -n "aiOptions362";
createNode aiOptions -n "aiOptions361";
createNode aiOptions -n "aiOptions360";
createNode aiOptions -n "aiOptions359";
createNode aiOptions -n "aiOptions358";
createNode aiOptions -n "aiOptions357";
createNode aiOptions -n "aiOptions356";
createNode aiOptions -n "aiOptions355";
createNode aiOptions -n "aiOptions354";
createNode aiOptions -n "aiOptions353";
createNode aiOptions -n "aiOptions352";
createNode aiOptions -n "aiOptions351";
createNode aiOptions -n "aiOptions350";
createNode aiOptions -n "aiOptions349";
createNode aiOptions -n "aiOptions348";
createNode aiOptions -n "aiOptions347";
createNode aiOptions -n "aiOptions346";
createNode aiOptions -n "aiOptions345";
createNode aiOptions -n "aiOptions344";
createNode aiOptions -n "aiOptions343";
createNode aiOptions -n "aiOptions342";
createNode aiOptions -n "aiOptions341";
createNode aiOptions -n "aiOptions340";
createNode aiOptions -n "aiOptions339";
createNode aiOptions -n "aiOptions338";
createNode aiOptions -n "aiOptions337";
createNode aiOptions -n "aiOptions336";
createNode aiOptions -n "aiOptions335";
createNode aiOptions -n "aiOptions334";
createNode aiOptions -n "aiOptions333";
createNode aiOptions -n "aiOptions332";
createNode aiOptions -n "aiOptions331";
createNode aiOptions -n "aiOptions330";
createNode aiOptions -n "aiOptions329";
createNode aiOptions -n "aiOptions328";
createNode aiOptions -n "aiOptions327";
createNode aiOptions -n "aiOptions326";
createNode aiOptions -n "aiOptions325";
createNode aiOptions -n "aiOptions324";
createNode aiOptions -n "aiOptions323";
createNode aiOptions -n "aiOptions322";
createNode aiOptions -n "aiOptions321";
createNode aiOptions -n "aiOptions320";
createNode aiOptions -n "aiOptions319";
createNode aiOptions -n "aiOptions318";
createNode aiOptions -n "aiOptions317";
createNode aiOptions -n "aiOptions316";
createNode aiOptions -n "aiOptions315";
createNode aiOptions -n "aiOptions314";
createNode aiOptions -n "aiOptions313";
createNode aiOptions -n "aiOptions312";
createNode aiOptions -n "aiOptions311";
createNode aiOptions -n "aiOptions310";
createNode aiOptions -n "aiOptions309";
createNode aiOptions -n "aiOptions308";
createNode aiOptions -n "aiOptions307";
createNode aiOptions -n "aiOptions306";
createNode aiOptions -n "aiOptions305";
createNode aiOptions -n "aiOptions304";
createNode aiOptions -n "aiOptions303";
createNode aiOptions -n "aiOptions302";
createNode aiOptions -n "aiOptions301";
createNode aiOptions -n "aiOptions300";
createNode aiOptions -n "aiOptions299";
createNode aiOptions -n "aiOptions298";
createNode aiOptions -n "aiOptions297";
createNode aiOptions -n "aiOptions296";
createNode aiOptions -n "aiOptions295";
createNode aiOptions -n "aiOptions294";
createNode aiOptions -n "aiOptions293";
createNode aiOptions -n "aiOptions292";
createNode aiOptions -n "aiOptions291";
createNode aiOptions -n "aiOptions290";
createNode aiOptions -n "aiOptions289";
createNode aiOptions -n "aiOptions288";
createNode aiOptions -n "aiOptions287";
createNode aiOptions -n "aiOptions286";
createNode aiOptions -n "aiOptions285";
createNode aiOptions -n "aiOptions284";
createNode aiOptions -n "aiOptions283";
createNode aiOptions -n "aiOptions282";
createNode aiOptions -n "aiOptions281";
createNode aiOptions -n "aiOptions280";
createNode aiOptions -n "aiOptions279";
createNode aiOptions -n "aiOptions278";
createNode aiOptions -n "aiOptions277";
createNode aiOptions -n "aiOptions276";
createNode aiOptions -n "aiOptions275";
createNode aiOptions -n "aiOptions274";
createNode aiOptions -n "aiOptions273";
createNode aiOptions -n "aiOptions272";
createNode aiOptions -n "aiOptions271";
createNode aiOptions -n "aiOptions270";
createNode aiOptions -n "aiOptions269";
createNode aiOptions -n "aiOptions268";
createNode aiOptions -n "aiOptions267";
createNode aiOptions -n "aiOptions266";
createNode aiOptions -n "aiOptions265";
createNode aiOptions -n "aiOptions264";
createNode aiOptions -n "aiOptions263";
createNode aiOptions -n "aiOptions262";
createNode aiOptions -n "aiOptions261";
createNode aiOptions -n "aiOptions260";
createNode aiOptions -n "aiOptions259";
createNode aiOptions -n "aiOptions258";
createNode aiOptions -n "aiOptions257";
createNode aiOptions -n "aiOptions256";
createNode aiOptions -n "aiOptions255";
createNode aiOptions -n "aiOptions254";
createNode aiOptions -n "aiOptions253";
createNode aiOptions -n "aiOptions252";
createNode aiOptions -n "aiOptions251";
createNode aiOptions -n "aiOptions250";
createNode aiOptions -n "aiOptions249";
createNode aiOptions -n "aiOptions248";
createNode aiOptions -n "aiOptions247";
createNode aiOptions -n "aiOptions246";
createNode aiOptions -n "aiOptions245";
createNode aiOptions -n "aiOptions244";
createNode aiOptions -n "aiOptions243";
createNode aiOptions -n "aiOptions242";
createNode aiOptions -n "aiOptions241";
createNode aiOptions -n "aiOptions240";
createNode aiOptions -n "aiOptions239";
createNode aiOptions -n "aiOptions238";
createNode aiOptions -n "aiOptions237";
createNode aiOptions -n "aiOptions236";
createNode aiOptions -n "aiOptions235";
createNode aiOptions -n "aiOptions234";
createNode aiOptions -n "aiOptions233";
createNode aiOptions -n "aiOptions232";
createNode aiOptions -n "aiOptions231";
createNode aiOptions -n "aiOptions230";
createNode aiOptions -n "aiOptions229";
createNode aiOptions -n "aiOptions228";
createNode aiOptions -n "aiOptions227";
createNode aiOptions -n "aiOptions226";
createNode aiOptions -n "aiOptions225";
createNode aiOptions -n "aiOptions224";
createNode aiOptions -n "aiOptions223";
createNode aiOptions -n "aiOptions222";
createNode aiOptions -n "aiOptions221";
createNode aiOptions -n "aiOptions220";
createNode aiOptions -n "aiOptions219";
createNode aiOptions -n "aiOptions218";
createNode aiOptions -n "aiOptions217";
createNode aiOptions -n "aiOptions216";
createNode aiOptions -n "aiOptions215";
createNode aiOptions -n "aiOptions214";
createNode aiOptions -n "aiOptions213";
createNode aiOptions -n "aiOptions212";
createNode aiOptions -n "aiOptions211";
createNode aiOptions -n "aiOptions210";
createNode aiOptions -n "aiOptions209";
createNode aiOptions -n "aiOptions208";
createNode aiOptions -n "aiOptions207";
createNode aiOptions -n "aiOptions206";
createNode aiOptions -n "aiOptions205";
createNode aiOptions -n "aiOptions204";
createNode aiOptions -n "aiOptions203";
createNode aiOptions -n "aiOptions202";
createNode aiOptions -n "aiOptions201";
createNode aiOptions -n "aiOptions200";
createNode aiOptions -n "aiOptions199";
createNode aiOptions -n "aiOptions198";
createNode aiOptions -n "aiOptions197";
createNode aiOptions -n "aiOptions196";
createNode aiOptions -n "aiOptions195";
createNode aiOptions -n "aiOptions194";
createNode aiOptions -n "aiOptions193";
createNode aiOptions -n "aiOptions192";
createNode aiOptions -n "aiOptions191";
createNode aiOptions -n "aiOptions190";
createNode aiOptions -n "aiOptions189";
createNode aiOptions -n "aiOptions188";
createNode aiOptions -n "aiOptions187";
createNode aiOptions -n "aiOptions186";
createNode aiOptions -n "aiOptions185";
createNode aiOptions -n "aiOptions184";
createNode aiOptions -n "aiOptions183";
createNode aiOptions -n "aiOptions182";
createNode aiOptions -n "aiOptions181";
createNode aiOptions -n "aiOptions180";
createNode aiOptions -n "aiOptions179";
createNode aiOptions -n "aiOptions178";
createNode aiOptions -n "aiOptions177";
createNode aiOptions -n "aiOptions176";
createNode aiOptions -n "aiOptions175";
createNode aiOptions -n "aiOptions174";
createNode aiOptions -n "aiOptions173";
createNode aiOptions -n "aiOptions172";
createNode aiOptions -n "aiOptions171";
createNode aiOptions -n "aiOptions170";
createNode aiOptions -n "aiOptions169";
createNode aiOptions -n "aiOptions168";
createNode aiOptions -n "aiOptions167";
createNode aiOptions -n "aiOptions166";
createNode aiOptions -n "aiOptions165";
createNode aiOptions -n "aiOptions164";
createNode aiOptions -n "aiOptions163";
createNode aiOptions -n "aiOptions162";
createNode aiOptions -n "aiOptions161";
createNode aiOptions -n "aiOptions160";
createNode aiOptions -n "aiOptions159";
createNode aiOptions -n "aiOptions158";
createNode aiOptions -n "aiOptions157";
createNode aiOptions -n "aiOptions156";
createNode aiOptions -n "aiOptions155";
createNode aiOptions -n "aiOptions154";
createNode aiOptions -n "aiOptions153";
createNode aiOptions -n "aiOptions152";
createNode aiOptions -n "aiOptions151";
createNode aiOptions -n "aiOptions150";
createNode aiOptions -n "aiOptions149";
createNode aiOptions -n "aiOptions148";
createNode aiOptions -n "aiOptions147";
createNode aiOptions -n "aiOptions146";
createNode aiOptions -n "aiOptions145";
createNode aiOptions -n "aiOptions144";
createNode aiOptions -n "aiOptions143";
createNode aiOptions -n "aiOptions142";
createNode aiOptions -n "aiOptions141";
createNode aiOptions -n "aiOptions140";
createNode aiOptions -n "aiOptions139";
createNode aiOptions -n "aiOptions138";
createNode aiOptions -n "aiOptions137";
createNode aiOptions -n "aiOptions136";
createNode aiOptions -n "aiOptions135";
createNode aiOptions -n "aiOptions134";
createNode aiOptions -n "aiOptions133";
createNode aiOptions -n "aiOptions132";
createNode aiOptions -n "aiOptions131";
createNode aiOptions -n "aiOptions130";
createNode aiOptions -n "aiOptions129";
createNode aiOptions -n "aiOptions128";
createNode aiOptions -n "aiOptions127";
createNode aiOptions -n "aiOptions126";
createNode aiOptions -n "aiOptions125";
createNode aiOptions -n "aiOptions124";
createNode aiOptions -n "aiOptions123";
createNode aiOptions -n "aiOptions122";
createNode aiOptions -n "aiOptions121";
createNode aiOptions -n "aiOptions120";
createNode aiOptions -n "aiOptions119";
createNode aiOptions -n "aiOptions118";
createNode aiOptions -n "aiOptions117";
createNode aiOptions -n "aiOptions116";
createNode aiOptions -n "aiOptions115";
createNode aiOptions -n "aiOptions114";
createNode aiOptions -n "aiOptions113";
createNode aiOptions -n "aiOptions112";
createNode aiOptions -n "aiOptions111";
createNode aiOptions -n "aiOptions110";
createNode aiOptions -n "aiOptions109";
createNode aiOptions -n "aiOptions108";
createNode aiOptions -n "aiOptions107";
createNode aiOptions -n "aiOptions106";
createNode aiOptions -n "aiOptions105";
createNode aiOptions -n "aiOptions104";
createNode aiOptions -n "aiOptions103";
createNode aiOptions -n "aiOptions102";
createNode aiOptions -n "aiOptions101";
createNode aiOptions -n "aiOptions100";
createNode aiOptions -n "aiOptions99";
createNode aiOptions -n "aiOptions98";
createNode aiOptions -n "aiOptions97";
createNode aiOptions -n "aiOptions96";
createNode aiOptions -n "aiOptions95";
createNode aiOptions -n "aiOptions94";
createNode aiOptions -n "aiOptions93";
createNode aiOptions -n "aiOptions92";
createNode aiOptions -n "aiOptions91";
createNode aiOptions -n "aiOptions90";
createNode aiOptions -n "aiOptions89";
createNode aiOptions -n "aiOptions88";
createNode aiOptions -n "aiOptions87";
createNode aiOptions -n "aiOptions86";
createNode aiOptions -n "aiOptions85";
createNode aiOptions -n "aiOptions84";
createNode aiOptions -n "aiOptions83";
createNode aiOptions -n "aiOptions82";
createNode aiOptions -n "aiOptions81";
createNode aiOptions -n "aiOptions80";
createNode aiOptions -n "aiOptions79";
createNode aiOptions -n "aiOptions78";
createNode aiOptions -n "aiOptions77";
createNode aiOptions -n "aiOptions76";
createNode aiOptions -n "aiOptions75";
createNode aiOptions -n "aiOptions74";
createNode aiOptions -n "aiOptions73";
createNode aiOptions -n "aiOptions72";
createNode aiOptions -n "aiOptions71";
createNode aiOptions -n "aiOptions70";
createNode aiOptions -n "aiOptions69";
createNode aiOptions -n "aiOptions68";
createNode aiOptions -n "aiOptions67";
createNode aiOptions -n "aiOptions66";
createNode aiOptions -n "aiOptions65";
createNode aiOptions -n "aiOptions64";
createNode aiOptions -n "aiOptions63";
createNode aiOptions -n "aiOptions62";
createNode aiOptions -n "aiOptions61";
createNode aiOptions -n "aiOptions60";
createNode aiOptions -n "aiOptions59";
createNode aiOptions -n "aiOptions58";
createNode aiOptions -n "aiOptions57";
createNode aiOptions -n "aiOptions56";
createNode aiOptions -n "aiOptions55";
createNode aiOptions -n "aiOptions54";
createNode aiOptions -n "aiOptions53";
createNode aiOptions -n "aiOptions52";
createNode aiOptions -n "aiOptions51";
createNode aiOptions -n "aiOptions50";
createNode aiOptions -n "aiOptions49";
createNode aiOptions -n "aiOptions48";
createNode aiOptions -n "aiOptions47";
createNode aiOptions -n "aiOptions46";
createNode aiOptions -n "aiOptions45";
createNode aiOptions -n "aiOptions44";
createNode aiOptions -n "aiOptions43";
createNode aiOptions -n "aiOptions42";
createNode aiOptions -n "aiOptions41";
createNode aiOptions -n "aiOptions40";
createNode aiOptions -n "aiOptions39";
createNode aiOptions -n "aiOptions38";
createNode aiOptions -n "aiOptions37";
createNode aiOptions -n "aiOptions36";
createNode aiOptions -n "aiOptions35";
createNode aiOptions -n "aiOptions34";
createNode aiOptions -n "aiOptions33";
createNode aiOptions -n "aiOptions32";
createNode aiOptions -n "aiOptions31";
createNode aiOptions -n "aiOptions30";
createNode aiOptions -n "aiOptions29";
createNode aiOptions -n "aiOptions28";
createNode aiOptions -n "aiOptions27";
createNode aiOptions -n "aiOptions26";
createNode aiOptions -n "aiOptions25";
createNode aiOptions -n "aiOptions24";
createNode aiOptions -n "aiOptions23";
createNode aiOptions -n "aiOptions22";
createNode aiOptions -n "aiOptions21";
createNode aiOptions -n "aiOptions20";
createNode aiOptions -n "aiOptions19";
createNode aiOptions -n "aiOptions18";
createNode aiOptions -n "aiOptions17";
createNode aiOptions -n "aiOptions16";
createNode aiOptions -n "aiOptions15";
createNode aiOptions -n "aiOptions14";
createNode aiOptions -n "aiOptions13";
createNode aiOptions -n "aiOptions12";
createNode aiOptions -n "aiOptions11";
createNode aiOptions -n "aiOptions10";
createNode aiOptions -n "aiOptions9";
createNode aiOptions -n "aiOptions8";
createNode aiOptions -n "aiOptions7";
createNode aiOptions -n "aiOptions6";
createNode aiOptions -n "aiOptions5";
createNode aiOptions -n "aiOptions4";
createNode aiOptions -n "aiOptions3";
createNode aiOptions -n "aiOptions2";
createNode aiOptions -n "aiOptions1";
createNode aiOptions -s -n "defaultArnoldRenderOptions";
	setAttr ".dgamma" 1;
	setAttr ".lgamma" 1;
	setAttr ".sgamma" 1;
	setAttr ".tgamma" 1;
createNode script -n "uiConfigurationScriptNode";
	setAttr ".b" -type "string" (
		"// Maya Mel UI Configuration File.\n//\n//  This script is machine generated.  Edit at your own risk.\n//\n//\n\nglobal string $gMainPane;\nif (`paneLayout -exists $gMainPane`) {\n\n\tglobal int $gUseScenePanelConfig;\n\tint    $useSceneConfig = $gUseScenePanelConfig;\n\tint    $menusOkayInPanels = `optionVar -q allowMenusInPanels`;\tint    $nVisPanes = `paneLayout -q -nvp $gMainPane`;\n\tint    $nPanes = 0;\n\tstring $editorName;\n\tstring $panelName;\n\tstring $itemFilterName;\n\tstring $panelConfig;\n\n\t//\n\t//  get current state of the UI\n\t//\n\tsceneUIReplacement -update $gMainPane;\n\n\t$panelName = `sceneUIReplacement -getNextPanel \"modelPanel\" (localizedPanelLabel(\"Top View\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `modelPanel -unParent -l (localizedPanelLabel(\"Top View\")) -mbv $menusOkayInPanels `;\n\t\t\t$editorName = $panelName;\n            modelEditor -e \n                -camera \"top\" \n                -useInteractiveMode 0\n                -displayLights \"default\" \n                -displayAppearance \"wireframe\" \n"
		+ "                -activeOnly 0\n                -ignorePanZoom 0\n                -wireframeOnShaded 0\n                -headsUpDisplay 1\n                -selectionHiliteDisplay 1\n                -useDefaultMaterial 0\n                -bufferMode \"double\" \n                -twoSidedLighting 1\n                -backfaceCulling 0\n                -xray 0\n                -jointXray 0\n                -activeComponentsXray 0\n                -displayTextures 0\n                -smoothWireframe 0\n                -lineWidth 1\n                -textureAnisotropic 0\n                -textureHilight 1\n                -textureSampling 2\n                -textureDisplay \"modulate\" \n                -textureMaxSize 16384\n                -fogging 0\n                -fogSource \"fragment\" \n                -fogMode \"linear\" \n                -fogStart 0\n                -fogEnd 100\n                -fogDensity 0.1\n                -fogColor 0.5 0.5 0.5 1 \n                -maxConstantTransparency 1\n                -rendererName \"base_OpenGL_Renderer\" \n"
		+ "                -objectFilterShowInHUD 1\n                -isFiltered 0\n                -colorResolution 256 256 \n                -bumpResolution 512 512 \n                -textureCompression 0\n                -transparencyAlgorithm \"frontAndBackCull\" \n                -transpInShadows 0\n                -cullingOverride \"none\" \n                -lowQualityLighting 0\n                -maximumNumHardwareLights 1\n                -occlusionCulling 0\n                -shadingModel 0\n                -useBaseRenderer 0\n                -useReducedRenderer 0\n                -smallObjectCulling 0\n                -smallObjectThreshold -1 \n                -interactiveDisableShadows 0\n                -interactiveBackFaceCull 0\n                -sortTransparent 1\n                -nurbsCurves 1\n                -nurbsSurfaces 1\n                -polymeshes 1\n                -subdivSurfaces 1\n                -planes 1\n                -lights 1\n                -cameras 1\n                -controlVertices 1\n                -hulls 1\n                -grid 1\n"
		+ "                -imagePlane 1\n                -joints 1\n                -ikHandles 1\n                -deformers 1\n                -dynamics 1\n                -fluids 1\n                -hairSystems 1\n                -follicles 1\n                -nCloths 1\n                -nParticles 1\n                -nRigids 1\n                -dynamicConstraints 1\n                -locators 1\n                -manipulators 1\n                -dimensions 1\n                -handles 1\n                -pivots 1\n                -textures 1\n                -strokes 1\n                -motionTrails 1\n                -clipGhosts 1\n                -shadows 0\n                $editorName;\nmodelEditor -e -viewSelected 0 $editorName;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tmodelPanel -edit -l (localizedPanelLabel(\"Top View\")) -mbv $menusOkayInPanels  $panelName;\n\t\t$editorName = $panelName;\n        modelEditor -e \n            -camera \"top\" \n            -useInteractiveMode 0\n            -displayLights \"default\" \n            -displayAppearance \"wireframe\" \n"
		+ "            -activeOnly 0\n            -ignorePanZoom 0\n            -wireframeOnShaded 0\n            -headsUpDisplay 1\n            -selectionHiliteDisplay 1\n            -useDefaultMaterial 0\n            -bufferMode \"double\" \n            -twoSidedLighting 1\n            -backfaceCulling 0\n            -xray 0\n            -jointXray 0\n            -activeComponentsXray 0\n            -displayTextures 0\n            -smoothWireframe 0\n            -lineWidth 1\n            -textureAnisotropic 0\n            -textureHilight 1\n            -textureSampling 2\n            -textureDisplay \"modulate\" \n            -textureMaxSize 16384\n            -fogging 0\n            -fogSource \"fragment\" \n            -fogMode \"linear\" \n            -fogStart 0\n            -fogEnd 100\n            -fogDensity 0.1\n            -fogColor 0.5 0.5 0.5 1 \n            -maxConstantTransparency 1\n            -rendererName \"base_OpenGL_Renderer\" \n            -objectFilterShowInHUD 1\n            -isFiltered 0\n            -colorResolution 256 256 \n            -bumpResolution 512 512 \n"
		+ "            -textureCompression 0\n            -transparencyAlgorithm \"frontAndBackCull\" \n            -transpInShadows 0\n            -cullingOverride \"none\" \n            -lowQualityLighting 0\n            -maximumNumHardwareLights 1\n            -occlusionCulling 0\n            -shadingModel 0\n            -useBaseRenderer 0\n            -useReducedRenderer 0\n            -smallObjectCulling 0\n            -smallObjectThreshold -1 \n            -interactiveDisableShadows 0\n            -interactiveBackFaceCull 0\n            -sortTransparent 1\n            -nurbsCurves 1\n            -nurbsSurfaces 1\n            -polymeshes 1\n            -subdivSurfaces 1\n            -planes 1\n            -lights 1\n            -cameras 1\n            -controlVertices 1\n            -hulls 1\n            -grid 1\n            -imagePlane 1\n            -joints 1\n            -ikHandles 1\n            -deformers 1\n            -dynamics 1\n            -fluids 1\n            -hairSystems 1\n            -follicles 1\n            -nCloths 1\n            -nParticles 1\n"
		+ "            -nRigids 1\n            -dynamicConstraints 1\n            -locators 1\n            -manipulators 1\n            -dimensions 1\n            -handles 1\n            -pivots 1\n            -textures 1\n            -strokes 1\n            -motionTrails 1\n            -clipGhosts 1\n            -shadows 0\n            $editorName;\nmodelEditor -e -viewSelected 0 $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextPanel \"modelPanel\" (localizedPanelLabel(\"Side View\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `modelPanel -unParent -l (localizedPanelLabel(\"Side View\")) -mbv $menusOkayInPanels `;\n\t\t\t$editorName = $panelName;\n            modelEditor -e \n                -camera \"side\" \n                -useInteractiveMode 0\n                -displayLights \"default\" \n                -displayAppearance \"wireframe\" \n                -activeOnly 0\n                -ignorePanZoom 0\n                -wireframeOnShaded 0\n                -headsUpDisplay 1\n"
		+ "                -selectionHiliteDisplay 1\n                -useDefaultMaterial 0\n                -bufferMode \"double\" \n                -twoSidedLighting 1\n                -backfaceCulling 0\n                -xray 0\n                -jointXray 0\n                -activeComponentsXray 0\n                -displayTextures 0\n                -smoothWireframe 0\n                -lineWidth 1\n                -textureAnisotropic 0\n                -textureHilight 1\n                -textureSampling 2\n                -textureDisplay \"modulate\" \n                -textureMaxSize 16384\n                -fogging 0\n                -fogSource \"fragment\" \n                -fogMode \"linear\" \n                -fogStart 0\n                -fogEnd 100\n                -fogDensity 0.1\n                -fogColor 0.5 0.5 0.5 1 \n                -maxConstantTransparency 1\n                -rendererName \"base_OpenGL_Renderer\" \n                -objectFilterShowInHUD 1\n                -isFiltered 0\n                -colorResolution 256 256 \n                -bumpResolution 512 512 \n"
		+ "                -textureCompression 0\n                -transparencyAlgorithm \"frontAndBackCull\" \n                -transpInShadows 0\n                -cullingOverride \"none\" \n                -lowQualityLighting 0\n                -maximumNumHardwareLights 1\n                -occlusionCulling 0\n                -shadingModel 0\n                -useBaseRenderer 0\n                -useReducedRenderer 0\n                -smallObjectCulling 0\n                -smallObjectThreshold -1 \n                -interactiveDisableShadows 0\n                -interactiveBackFaceCull 0\n                -sortTransparent 1\n                -nurbsCurves 1\n                -nurbsSurfaces 1\n                -polymeshes 1\n                -subdivSurfaces 1\n                -planes 1\n                -lights 1\n                -cameras 1\n                -controlVertices 1\n                -hulls 1\n                -grid 1\n                -imagePlane 1\n                -joints 1\n                -ikHandles 1\n                -deformers 1\n                -dynamics 1\n"
		+ "                -fluids 1\n                -hairSystems 1\n                -follicles 1\n                -nCloths 1\n                -nParticles 1\n                -nRigids 1\n                -dynamicConstraints 1\n                -locators 1\n                -manipulators 1\n                -dimensions 1\n                -handles 1\n                -pivots 1\n                -textures 1\n                -strokes 1\n                -motionTrails 1\n                -clipGhosts 1\n                -shadows 0\n                $editorName;\nmodelEditor -e -viewSelected 0 $editorName;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tmodelPanel -edit -l (localizedPanelLabel(\"Side View\")) -mbv $menusOkayInPanels  $panelName;\n\t\t$editorName = $panelName;\n        modelEditor -e \n            -camera \"side\" \n            -useInteractiveMode 0\n            -displayLights \"default\" \n            -displayAppearance \"wireframe\" \n            -activeOnly 0\n            -ignorePanZoom 0\n            -wireframeOnShaded 0\n            -headsUpDisplay 1\n"
		+ "            -selectionHiliteDisplay 1\n            -useDefaultMaterial 0\n            -bufferMode \"double\" \n            -twoSidedLighting 1\n            -backfaceCulling 0\n            -xray 0\n            -jointXray 0\n            -activeComponentsXray 0\n            -displayTextures 0\n            -smoothWireframe 0\n            -lineWidth 1\n            -textureAnisotropic 0\n            -textureHilight 1\n            -textureSampling 2\n            -textureDisplay \"modulate\" \n            -textureMaxSize 16384\n            -fogging 0\n            -fogSource \"fragment\" \n            -fogMode \"linear\" \n            -fogStart 0\n            -fogEnd 100\n            -fogDensity 0.1\n            -fogColor 0.5 0.5 0.5 1 \n            -maxConstantTransparency 1\n            -rendererName \"base_OpenGL_Renderer\" \n            -objectFilterShowInHUD 1\n            -isFiltered 0\n            -colorResolution 256 256 \n            -bumpResolution 512 512 \n            -textureCompression 0\n            -transparencyAlgorithm \"frontAndBackCull\" \n            -transpInShadows 0\n"
		+ "            -cullingOverride \"none\" \n            -lowQualityLighting 0\n            -maximumNumHardwareLights 1\n            -occlusionCulling 0\n            -shadingModel 0\n            -useBaseRenderer 0\n            -useReducedRenderer 0\n            -smallObjectCulling 0\n            -smallObjectThreshold -1 \n            -interactiveDisableShadows 0\n            -interactiveBackFaceCull 0\n            -sortTransparent 1\n            -nurbsCurves 1\n            -nurbsSurfaces 1\n            -polymeshes 1\n            -subdivSurfaces 1\n            -planes 1\n            -lights 1\n            -cameras 1\n            -controlVertices 1\n            -hulls 1\n            -grid 1\n            -imagePlane 1\n            -joints 1\n            -ikHandles 1\n            -deformers 1\n            -dynamics 1\n            -fluids 1\n            -hairSystems 1\n            -follicles 1\n            -nCloths 1\n            -nParticles 1\n            -nRigids 1\n            -dynamicConstraints 1\n            -locators 1\n            -manipulators 1\n            -dimensions 1\n"
		+ "            -handles 1\n            -pivots 1\n            -textures 1\n            -strokes 1\n            -motionTrails 1\n            -clipGhosts 1\n            -shadows 0\n            $editorName;\nmodelEditor -e -viewSelected 0 $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextPanel \"modelPanel\" (localizedPanelLabel(\"Front View\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `modelPanel -unParent -l (localizedPanelLabel(\"Front View\")) -mbv $menusOkayInPanels `;\n\t\t\t$editorName = $panelName;\n            modelEditor -e \n                -camera \"front\" \n                -useInteractiveMode 0\n                -displayLights \"default\" \n                -displayAppearance \"wireframe\" \n                -activeOnly 0\n                -ignorePanZoom 0\n                -wireframeOnShaded 0\n                -headsUpDisplay 1\n                -selectionHiliteDisplay 1\n                -useDefaultMaterial 0\n                -bufferMode \"double\" \n"
		+ "                -twoSidedLighting 1\n                -backfaceCulling 0\n                -xray 0\n                -jointXray 0\n                -activeComponentsXray 0\n                -displayTextures 0\n                -smoothWireframe 0\n                -lineWidth 1\n                -textureAnisotropic 0\n                -textureHilight 1\n                -textureSampling 2\n                -textureDisplay \"modulate\" \n                -textureMaxSize 16384\n                -fogging 0\n                -fogSource \"fragment\" \n                -fogMode \"linear\" \n                -fogStart 0\n                -fogEnd 100\n                -fogDensity 0.1\n                -fogColor 0.5 0.5 0.5 1 \n                -maxConstantTransparency 1\n                -rendererName \"base_OpenGL_Renderer\" \n                -objectFilterShowInHUD 1\n                -isFiltered 0\n                -colorResolution 256 256 \n                -bumpResolution 512 512 \n                -textureCompression 0\n                -transparencyAlgorithm \"frontAndBackCull\" \n"
		+ "                -transpInShadows 0\n                -cullingOverride \"none\" \n                -lowQualityLighting 0\n                -maximumNumHardwareLights 1\n                -occlusionCulling 0\n                -shadingModel 0\n                -useBaseRenderer 0\n                -useReducedRenderer 0\n                -smallObjectCulling 0\n                -smallObjectThreshold -1 \n                -interactiveDisableShadows 0\n                -interactiveBackFaceCull 0\n                -sortTransparent 1\n                -nurbsCurves 1\n                -nurbsSurfaces 1\n                -polymeshes 1\n                -subdivSurfaces 1\n                -planes 1\n                -lights 1\n                -cameras 1\n                -controlVertices 1\n                -hulls 1\n                -grid 1\n                -imagePlane 1\n                -joints 1\n                -ikHandles 1\n                -deformers 1\n                -dynamics 1\n                -fluids 1\n                -hairSystems 1\n                -follicles 1\n                -nCloths 1\n"
		+ "                -nParticles 1\n                -nRigids 1\n                -dynamicConstraints 1\n                -locators 1\n                -manipulators 1\n                -dimensions 1\n                -handles 1\n                -pivots 1\n                -textures 1\n                -strokes 1\n                -motionTrails 1\n                -clipGhosts 1\n                -shadows 0\n                $editorName;\nmodelEditor -e -viewSelected 0 $editorName;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tmodelPanel -edit -l (localizedPanelLabel(\"Front View\")) -mbv $menusOkayInPanels  $panelName;\n\t\t$editorName = $panelName;\n        modelEditor -e \n            -camera \"front\" \n            -useInteractiveMode 0\n            -displayLights \"default\" \n            -displayAppearance \"wireframe\" \n            -activeOnly 0\n            -ignorePanZoom 0\n            -wireframeOnShaded 0\n            -headsUpDisplay 1\n            -selectionHiliteDisplay 1\n            -useDefaultMaterial 0\n            -bufferMode \"double\" \n"
		+ "            -twoSidedLighting 1\n            -backfaceCulling 0\n            -xray 0\n            -jointXray 0\n            -activeComponentsXray 0\n            -displayTextures 0\n            -smoothWireframe 0\n            -lineWidth 1\n            -textureAnisotropic 0\n            -textureHilight 1\n            -textureSampling 2\n            -textureDisplay \"modulate\" \n            -textureMaxSize 16384\n            -fogging 0\n            -fogSource \"fragment\" \n            -fogMode \"linear\" \n            -fogStart 0\n            -fogEnd 100\n            -fogDensity 0.1\n            -fogColor 0.5 0.5 0.5 1 \n            -maxConstantTransparency 1\n            -rendererName \"base_OpenGL_Renderer\" \n            -objectFilterShowInHUD 1\n            -isFiltered 0\n            -colorResolution 256 256 \n            -bumpResolution 512 512 \n            -textureCompression 0\n            -transparencyAlgorithm \"frontAndBackCull\" \n            -transpInShadows 0\n            -cullingOverride \"none\" \n            -lowQualityLighting 0\n            -maximumNumHardwareLights 1\n"
		+ "            -occlusionCulling 0\n            -shadingModel 0\n            -useBaseRenderer 0\n            -useReducedRenderer 0\n            -smallObjectCulling 0\n            -smallObjectThreshold -1 \n            -interactiveDisableShadows 0\n            -interactiveBackFaceCull 0\n            -sortTransparent 1\n            -nurbsCurves 1\n            -nurbsSurfaces 1\n            -polymeshes 1\n            -subdivSurfaces 1\n            -planes 1\n            -lights 1\n            -cameras 1\n            -controlVertices 1\n            -hulls 1\n            -grid 1\n            -imagePlane 1\n            -joints 1\n            -ikHandles 1\n            -deformers 1\n            -dynamics 1\n            -fluids 1\n            -hairSystems 1\n            -follicles 1\n            -nCloths 1\n            -nParticles 1\n            -nRigids 1\n            -dynamicConstraints 1\n            -locators 1\n            -manipulators 1\n            -dimensions 1\n            -handles 1\n            -pivots 1\n            -textures 1\n            -strokes 1\n"
		+ "            -motionTrails 1\n            -clipGhosts 1\n            -shadows 0\n            $editorName;\nmodelEditor -e -viewSelected 0 $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextPanel \"modelPanel\" (localizedPanelLabel(\"Persp View\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `modelPanel -unParent -l (localizedPanelLabel(\"Persp View\")) -mbv $menusOkayInPanels `;\n\t\t\t$editorName = $panelName;\n            modelEditor -e \n                -camera \"persp\" \n                -useInteractiveMode 0\n                -displayLights \"default\" \n                -displayAppearance \"wireframe\" \n                -activeOnly 0\n                -ignorePanZoom 0\n                -wireframeOnShaded 0\n                -headsUpDisplay 1\n                -selectionHiliteDisplay 1\n                -useDefaultMaterial 0\n                -bufferMode \"double\" \n                -twoSidedLighting 1\n                -backfaceCulling 0\n                -xray 0\n"
		+ "                -jointXray 0\n                -activeComponentsXray 0\n                -displayTextures 0\n                -smoothWireframe 0\n                -lineWidth 1\n                -textureAnisotropic 0\n                -textureHilight 1\n                -textureSampling 2\n                -textureDisplay \"modulate\" \n                -textureMaxSize 16384\n                -fogging 0\n                -fogSource \"fragment\" \n                -fogMode \"linear\" \n                -fogStart 0\n                -fogEnd 100\n                -fogDensity 0.1\n                -fogColor 0.5 0.5 0.5 1 \n                -maxConstantTransparency 1\n                -rendererName \"base_OpenGL_Renderer\" \n                -objectFilterShowInHUD 1\n                -isFiltered 0\n                -colorResolution 256 256 \n                -bumpResolution 512 512 \n                -textureCompression 0\n                -transparencyAlgorithm \"frontAndBackCull\" \n                -transpInShadows 0\n                -cullingOverride \"none\" \n                -lowQualityLighting 0\n"
		+ "                -maximumNumHardwareLights 1\n                -occlusionCulling 0\n                -shadingModel 0\n                -useBaseRenderer 0\n                -useReducedRenderer 0\n                -smallObjectCulling 0\n                -smallObjectThreshold -1 \n                -interactiveDisableShadows 0\n                -interactiveBackFaceCull 0\n                -sortTransparent 1\n                -nurbsCurves 1\n                -nurbsSurfaces 1\n                -polymeshes 1\n                -subdivSurfaces 1\n                -planes 1\n                -lights 1\n                -cameras 1\n                -controlVertices 1\n                -hulls 1\n                -grid 1\n                -imagePlane 1\n                -joints 1\n                -ikHandles 1\n                -deformers 1\n                -dynamics 1\n                -fluids 1\n                -hairSystems 1\n                -follicles 1\n                -nCloths 1\n                -nParticles 1\n                -nRigids 1\n                -dynamicConstraints 1\n"
		+ "                -locators 1\n                -manipulators 1\n                -dimensions 1\n                -handles 1\n                -pivots 1\n                -textures 1\n                -strokes 1\n                -motionTrails 1\n                -clipGhosts 1\n                -shadows 0\n                $editorName;\nmodelEditor -e -viewSelected 0 $editorName;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tmodelPanel -edit -l (localizedPanelLabel(\"Persp View\")) -mbv $menusOkayInPanels  $panelName;\n\t\t$editorName = $panelName;\n        modelEditor -e \n            -camera \"persp\" \n            -useInteractiveMode 0\n            -displayLights \"default\" \n            -displayAppearance \"wireframe\" \n            -activeOnly 0\n            -ignorePanZoom 0\n            -wireframeOnShaded 0\n            -headsUpDisplay 1\n            -selectionHiliteDisplay 1\n            -useDefaultMaterial 0\n            -bufferMode \"double\" \n            -twoSidedLighting 1\n            -backfaceCulling 0\n            -xray 0\n            -jointXray 0\n"
		+ "            -activeComponentsXray 0\n            -displayTextures 0\n            -smoothWireframe 0\n            -lineWidth 1\n            -textureAnisotropic 0\n            -textureHilight 1\n            -textureSampling 2\n            -textureDisplay \"modulate\" \n            -textureMaxSize 16384\n            -fogging 0\n            -fogSource \"fragment\" \n            -fogMode \"linear\" \n            -fogStart 0\n            -fogEnd 100\n            -fogDensity 0.1\n            -fogColor 0.5 0.5 0.5 1 \n            -maxConstantTransparency 1\n            -rendererName \"base_OpenGL_Renderer\" \n            -objectFilterShowInHUD 1\n            -isFiltered 0\n            -colorResolution 256 256 \n            -bumpResolution 512 512 \n            -textureCompression 0\n            -transparencyAlgorithm \"frontAndBackCull\" \n            -transpInShadows 0\n            -cullingOverride \"none\" \n            -lowQualityLighting 0\n            -maximumNumHardwareLights 1\n            -occlusionCulling 0\n            -shadingModel 0\n            -useBaseRenderer 0\n"
		+ "            -useReducedRenderer 0\n            -smallObjectCulling 0\n            -smallObjectThreshold -1 \n            -interactiveDisableShadows 0\n            -interactiveBackFaceCull 0\n            -sortTransparent 1\n            -nurbsCurves 1\n            -nurbsSurfaces 1\n            -polymeshes 1\n            -subdivSurfaces 1\n            -planes 1\n            -lights 1\n            -cameras 1\n            -controlVertices 1\n            -hulls 1\n            -grid 1\n            -imagePlane 1\n            -joints 1\n            -ikHandles 1\n            -deformers 1\n            -dynamics 1\n            -fluids 1\n            -hairSystems 1\n            -follicles 1\n            -nCloths 1\n            -nParticles 1\n            -nRigids 1\n            -dynamicConstraints 1\n            -locators 1\n            -manipulators 1\n            -dimensions 1\n            -handles 1\n            -pivots 1\n            -textures 1\n            -strokes 1\n            -motionTrails 1\n            -clipGhosts 1\n            -shadows 0\n            $editorName;\n"
		+ "modelEditor -e -viewSelected 0 $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextPanel \"outlinerPanel\" (localizedPanelLabel(\"Outliner\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `outlinerPanel -unParent -l (localizedPanelLabel(\"Outliner\")) -mbv $menusOkayInPanels `;\n\t\t\t$editorName = $panelName;\n            outlinerEditor -e \n                -docTag \"isolOutln_fromSeln\" \n                -showShapes 1\n                -showReferenceNodes 1\n                -showReferenceMembers 1\n                -showAttributes 0\n                -showConnected 0\n                -showAnimCurvesOnly 0\n                -showMuteInfo 0\n                -organizeByLayer 1\n                -showAnimLayerWeight 1\n                -autoExpandLayers 1\n                -autoExpand 0\n                -showDagOnly 0\n                -showAssets 1\n                -showContainedOnly 1\n                -showPublishedAsConnected 0\n                -showContainerContents 1\n"
		+ "                -ignoreDagHierarchy 0\n                -expandConnections 0\n                -showUpstreamCurves 1\n                -showUnitlessCurves 1\n                -showCompounds 1\n                -showLeafs 1\n                -showNumericAttrsOnly 0\n                -highlightActive 1\n                -autoSelectNewObjects 0\n                -doNotSelectNewObjects 0\n                -dropIsParent 1\n                -transmitFilters 0\n                -setFilter \"defaultSetFilter\" \n                -showSetMembers 1\n                -allowMultiSelection 1\n                -alwaysToggleSelect 0\n                -directSelect 0\n                -displayMode \"DAG\" \n                -expandObjects 0\n                -setsIgnoreFilters 1\n                -containersIgnoreFilters 0\n                -editAttrName 0\n                -showAttrValues 0\n                -highlightSecondary 0\n                -showUVAttrsOnly 0\n                -showTextureNodesOnly 0\n                -attrAlphaOrder \"default\" \n                -animLayerFilterOptions \"allAffecting\" \n"
		+ "                -sortOrder \"none\" \n                -longNames 0\n                -niceNames 1\n                -showNamespace 1\n                -showPinIcons 0\n                -mapMotionTrails 0\n                $editorName;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\toutlinerPanel -edit -l (localizedPanelLabel(\"Outliner\")) -mbv $menusOkayInPanels  $panelName;\n\t\t$editorName = $panelName;\n        outlinerEditor -e \n            -docTag \"isolOutln_fromSeln\" \n            -showShapes 1\n            -showReferenceNodes 1\n            -showReferenceMembers 1\n            -showAttributes 0\n            -showConnected 0\n            -showAnimCurvesOnly 0\n            -showMuteInfo 0\n            -organizeByLayer 1\n            -showAnimLayerWeight 1\n            -autoExpandLayers 1\n            -autoExpand 0\n            -showDagOnly 0\n            -showAssets 1\n            -showContainedOnly 1\n            -showPublishedAsConnected 0\n            -showContainerContents 1\n            -ignoreDagHierarchy 0\n            -expandConnections 0\n"
		+ "            -showUpstreamCurves 1\n            -showUnitlessCurves 1\n            -showCompounds 1\n            -showLeafs 1\n            -showNumericAttrsOnly 0\n            -highlightActive 1\n            -autoSelectNewObjects 0\n            -doNotSelectNewObjects 0\n            -dropIsParent 1\n            -transmitFilters 0\n            -setFilter \"defaultSetFilter\" \n            -showSetMembers 1\n            -allowMultiSelection 1\n            -alwaysToggleSelect 0\n            -directSelect 0\n            -displayMode \"DAG\" \n            -expandObjects 0\n            -setsIgnoreFilters 1\n            -containersIgnoreFilters 0\n            -editAttrName 0\n            -showAttrValues 0\n            -highlightSecondary 0\n            -showUVAttrsOnly 0\n            -showTextureNodesOnly 0\n            -attrAlphaOrder \"default\" \n            -animLayerFilterOptions \"allAffecting\" \n            -sortOrder \"none\" \n            -longNames 0\n            -niceNames 1\n            -showNamespace 1\n            -showPinIcons 0\n            -mapMotionTrails 0\n"
		+ "            $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\tif ($useSceneConfig) {\n\t\toutlinerPanel -e -to $panelName;\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"graphEditor\" (localizedPanelLabel(\"Graph Editor\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"graphEditor\" -l (localizedPanelLabel(\"Graph Editor\")) -mbv $menusOkayInPanels `;\n\n\t\t\t$editorName = ($panelName+\"OutlineEd\");\n            outlinerEditor -e \n                -showShapes 1\n                -showReferenceNodes 0\n                -showReferenceMembers 0\n                -showAttributes 1\n                -showConnected 1\n                -showAnimCurvesOnly 1\n                -showMuteInfo 0\n                -organizeByLayer 1\n                -showAnimLayerWeight 1\n                -autoExpandLayers 1\n                -autoExpand 1\n                -showDagOnly 0\n                -showAssets 1\n                -showContainedOnly 0\n                -showPublishedAsConnected 0\n"
		+ "                -showContainerContents 0\n                -ignoreDagHierarchy 0\n                -expandConnections 1\n                -showUpstreamCurves 1\n                -showUnitlessCurves 1\n                -showCompounds 0\n                -showLeafs 1\n                -showNumericAttrsOnly 1\n                -highlightActive 0\n                -autoSelectNewObjects 1\n                -doNotSelectNewObjects 0\n                -dropIsParent 1\n                -transmitFilters 1\n                -setFilter \"0\" \n                -showSetMembers 0\n                -allowMultiSelection 1\n                -alwaysToggleSelect 0\n                -directSelect 0\n                -displayMode \"DAG\" \n                -expandObjects 0\n                -setsIgnoreFilters 1\n                -containersIgnoreFilters 0\n                -editAttrName 0\n                -showAttrValues 0\n                -highlightSecondary 0\n                -showUVAttrsOnly 0\n                -showTextureNodesOnly 0\n                -attrAlphaOrder \"default\" \n                -animLayerFilterOptions \"allAffecting\" \n"
		+ "                -sortOrder \"none\" \n                -longNames 0\n                -niceNames 1\n                -showNamespace 1\n                -showPinIcons 1\n                -mapMotionTrails 1\n                $editorName;\n\n\t\t\t$editorName = ($panelName+\"GraphEd\");\n            animCurveEditor -e \n                -displayKeys 1\n                -displayTangents 0\n                -displayActiveKeys 0\n                -displayActiveKeyTangents 1\n                -displayInfinities 0\n                -autoFit 0\n                -snapTime \"integer\" \n                -snapValue \"none\" \n                -showResults \"off\" \n                -showBufferCurves \"off\" \n                -smoothness \"fine\" \n                -resultSamples 1\n                -resultScreenSamples 0\n                -resultUpdate \"delayed\" \n                -showUpstreamCurves 1\n                -stackedCurves 0\n                -stackedCurvesMin -1\n                -stackedCurvesMax 1\n                -stackedCurvesSpace 0.2\n                -displayNormalized 0\n"
		+ "                -preSelectionHighlight 0\n                -constrainDrag 0\n                -classicMode 1\n                $editorName;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Graph Editor\")) -mbv $menusOkayInPanels  $panelName;\n\n\t\t\t$editorName = ($panelName+\"OutlineEd\");\n            outlinerEditor -e \n                -showShapes 1\n                -showReferenceNodes 0\n                -showReferenceMembers 0\n                -showAttributes 1\n                -showConnected 1\n                -showAnimCurvesOnly 1\n                -showMuteInfo 0\n                -organizeByLayer 1\n                -showAnimLayerWeight 1\n                -autoExpandLayers 1\n                -autoExpand 1\n                -showDagOnly 0\n                -showAssets 1\n                -showContainedOnly 0\n                -showPublishedAsConnected 0\n                -showContainerContents 0\n                -ignoreDagHierarchy 0\n                -expandConnections 1\n                -showUpstreamCurves 1\n"
		+ "                -showUnitlessCurves 1\n                -showCompounds 0\n                -showLeafs 1\n                -showNumericAttrsOnly 1\n                -highlightActive 0\n                -autoSelectNewObjects 1\n                -doNotSelectNewObjects 0\n                -dropIsParent 1\n                -transmitFilters 1\n                -setFilter \"0\" \n                -showSetMembers 0\n                -allowMultiSelection 1\n                -alwaysToggleSelect 0\n                -directSelect 0\n                -displayMode \"DAG\" \n                -expandObjects 0\n                -setsIgnoreFilters 1\n                -containersIgnoreFilters 0\n                -editAttrName 0\n                -showAttrValues 0\n                -highlightSecondary 0\n                -showUVAttrsOnly 0\n                -showTextureNodesOnly 0\n                -attrAlphaOrder \"default\" \n                -animLayerFilterOptions \"allAffecting\" \n                -sortOrder \"none\" \n                -longNames 0\n                -niceNames 1\n                -showNamespace 1\n"
		+ "                -showPinIcons 1\n                -mapMotionTrails 1\n                $editorName;\n\n\t\t\t$editorName = ($panelName+\"GraphEd\");\n            animCurveEditor -e \n                -displayKeys 1\n                -displayTangents 0\n                -displayActiveKeys 0\n                -displayActiveKeyTangents 1\n                -displayInfinities 0\n                -autoFit 0\n                -snapTime \"integer\" \n                -snapValue \"none\" \n                -showResults \"off\" \n                -showBufferCurves \"off\" \n                -smoothness \"fine\" \n                -resultSamples 1\n                -resultScreenSamples 0\n                -resultUpdate \"delayed\" \n                -showUpstreamCurves 1\n                -stackedCurves 0\n                -stackedCurvesMin -1\n                -stackedCurvesMax 1\n                -stackedCurvesSpace 0.2\n                -displayNormalized 0\n                -preSelectionHighlight 0\n                -constrainDrag 0\n                -classicMode 1\n                $editorName;\n"
		+ "\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"dopeSheetPanel\" (localizedPanelLabel(\"Dope Sheet\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"dopeSheetPanel\" -l (localizedPanelLabel(\"Dope Sheet\")) -mbv $menusOkayInPanels `;\n\n\t\t\t$editorName = ($panelName+\"OutlineEd\");\n            outlinerEditor -e \n                -showShapes 1\n                -showReferenceNodes 0\n                -showReferenceMembers 0\n                -showAttributes 1\n                -showConnected 1\n                -showAnimCurvesOnly 1\n                -showMuteInfo 0\n                -organizeByLayer 1\n                -showAnimLayerWeight 1\n                -autoExpandLayers 1\n                -autoExpand 0\n                -showDagOnly 0\n                -showAssets 1\n                -showContainedOnly 0\n                -showPublishedAsConnected 0\n                -showContainerContents 0\n                -ignoreDagHierarchy 0\n"
		+ "                -expandConnections 1\n                -showUpstreamCurves 1\n                -showUnitlessCurves 0\n                -showCompounds 1\n                -showLeafs 1\n                -showNumericAttrsOnly 1\n                -highlightActive 0\n                -autoSelectNewObjects 0\n                -doNotSelectNewObjects 1\n                -dropIsParent 1\n                -transmitFilters 0\n                -setFilter \"0\" \n                -showSetMembers 0\n                -allowMultiSelection 1\n                -alwaysToggleSelect 0\n                -directSelect 0\n                -displayMode \"DAG\" \n                -expandObjects 0\n                -setsIgnoreFilters 1\n                -containersIgnoreFilters 0\n                -editAttrName 0\n                -showAttrValues 0\n                -highlightSecondary 0\n                -showUVAttrsOnly 0\n                -showTextureNodesOnly 0\n                -attrAlphaOrder \"default\" \n                -animLayerFilterOptions \"allAffecting\" \n                -sortOrder \"none\" \n"
		+ "                -longNames 0\n                -niceNames 1\n                -showNamespace 1\n                -showPinIcons 0\n                -mapMotionTrails 1\n                $editorName;\n\n\t\t\t$editorName = ($panelName+\"DopeSheetEd\");\n            dopeSheetEditor -e \n                -displayKeys 1\n                -displayTangents 0\n                -displayActiveKeys 0\n                -displayActiveKeyTangents 0\n                -displayInfinities 0\n                -autoFit 0\n                -snapTime \"integer\" \n                -snapValue \"none\" \n                -outliner \"dopeSheetPanel1OutlineEd\" \n                -showSummary 1\n                -showScene 0\n                -hierarchyBelow 0\n                -showTicks 1\n                -selectionWindow 0 0 0 0 \n                $editorName;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Dope Sheet\")) -mbv $menusOkayInPanels  $panelName;\n\n\t\t\t$editorName = ($panelName+\"OutlineEd\");\n            outlinerEditor -e \n                -showShapes 1\n"
		+ "                -showReferenceNodes 0\n                -showReferenceMembers 0\n                -showAttributes 1\n                -showConnected 1\n                -showAnimCurvesOnly 1\n                -showMuteInfo 0\n                -organizeByLayer 1\n                -showAnimLayerWeight 1\n                -autoExpandLayers 1\n                -autoExpand 0\n                -showDagOnly 0\n                -showAssets 1\n                -showContainedOnly 0\n                -showPublishedAsConnected 0\n                -showContainerContents 0\n                -ignoreDagHierarchy 0\n                -expandConnections 1\n                -showUpstreamCurves 1\n                -showUnitlessCurves 0\n                -showCompounds 1\n                -showLeafs 1\n                -showNumericAttrsOnly 1\n                -highlightActive 0\n                -autoSelectNewObjects 0\n                -doNotSelectNewObjects 1\n                -dropIsParent 1\n                -transmitFilters 0\n                -setFilter \"0\" \n                -showSetMembers 0\n"
		+ "                -allowMultiSelection 1\n                -alwaysToggleSelect 0\n                -directSelect 0\n                -displayMode \"DAG\" \n                -expandObjects 0\n                -setsIgnoreFilters 1\n                -containersIgnoreFilters 0\n                -editAttrName 0\n                -showAttrValues 0\n                -highlightSecondary 0\n                -showUVAttrsOnly 0\n                -showTextureNodesOnly 0\n                -attrAlphaOrder \"default\" \n                -animLayerFilterOptions \"allAffecting\" \n                -sortOrder \"none\" \n                -longNames 0\n                -niceNames 1\n                -showNamespace 1\n                -showPinIcons 0\n                -mapMotionTrails 1\n                $editorName;\n\n\t\t\t$editorName = ($panelName+\"DopeSheetEd\");\n            dopeSheetEditor -e \n                -displayKeys 1\n                -displayTangents 0\n                -displayActiveKeys 0\n                -displayActiveKeyTangents 0\n                -displayInfinities 0\n                -autoFit 0\n"
		+ "                -snapTime \"integer\" \n                -snapValue \"none\" \n                -outliner \"dopeSheetPanel1OutlineEd\" \n                -showSummary 1\n                -showScene 0\n                -hierarchyBelow 0\n                -showTicks 1\n                -selectionWindow 0 0 0 0 \n                $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"clipEditorPanel\" (localizedPanelLabel(\"Trax Editor\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"clipEditorPanel\" -l (localizedPanelLabel(\"Trax Editor\")) -mbv $menusOkayInPanels `;\n\n\t\t\t$editorName = clipEditorNameFromPanel($panelName);\n            clipEditor -e \n                -displayKeys 0\n                -displayTangents 0\n                -displayActiveKeys 0\n                -displayActiveKeyTangents 0\n                -displayInfinities 0\n                -autoFit 0\n                -snapTime \"none\" \n                -snapValue \"none\" \n"
		+ "                -manageSequencer 0 \n                $editorName;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Trax Editor\")) -mbv $menusOkayInPanels  $panelName;\n\n\t\t\t$editorName = clipEditorNameFromPanel($panelName);\n            clipEditor -e \n                -displayKeys 0\n                -displayTangents 0\n                -displayActiveKeys 0\n                -displayActiveKeyTangents 0\n                -displayInfinities 0\n                -autoFit 0\n                -snapTime \"none\" \n                -snapValue \"none\" \n                -manageSequencer 0 \n                $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"sequenceEditorPanel\" (localizedPanelLabel(\"Camera Sequencer\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"sequenceEditorPanel\" -l (localizedPanelLabel(\"Camera Sequencer\")) -mbv $menusOkayInPanels `;\n"
		+ "\t\t\t$editorName = sequenceEditorNameFromPanel($panelName);\n            clipEditor -e \n                -displayKeys 0\n                -displayTangents 0\n                -displayActiveKeys 0\n                -displayActiveKeyTangents 0\n                -displayInfinities 0\n                -autoFit 0\n                -snapTime \"none\" \n                -snapValue \"none\" \n                -manageSequencer 1 \n                $editorName;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Camera Sequencer\")) -mbv $menusOkayInPanels  $panelName;\n\n\t\t\t$editorName = sequenceEditorNameFromPanel($panelName);\n            clipEditor -e \n                -displayKeys 0\n                -displayTangents 0\n                -displayActiveKeys 0\n                -displayActiveKeyTangents 0\n                -displayInfinities 0\n                -autoFit 0\n                -snapTime \"none\" \n                -snapValue \"none\" \n                -manageSequencer 1 \n                $editorName;\n\t\tif (!$useSceneConfig) {\n"
		+ "\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"hyperGraphPanel\" (localizedPanelLabel(\"Hypergraph Hierarchy\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"hyperGraphPanel\" -l (localizedPanelLabel(\"Hypergraph Hierarchy\")) -mbv $menusOkayInPanels `;\n\n\t\t\t$editorName = ($panelName+\"HyperGraphEd\");\n            hyperGraph -e \n                -graphLayoutStyle \"hierarchicalLayout\" \n                -orientation \"horiz\" \n                -mergeConnections 0\n                -zoom 1\n                -animateTransition 0\n                -showRelationships 1\n                -showShapes 0\n                -showDeformers 0\n                -showExpressions 0\n                -showConstraints 0\n                -showUnderworld 0\n                -showInvisible 0\n                -transitionFrames 1\n                -opaqueContainers 0\n                -freeform 0\n                -imagePosition 0 0 \n                -imageScale 1\n"
		+ "                -imageEnabled 0\n                -graphType \"DAG\" \n                -heatMapDisplay 0\n                -updateSelection 1\n                -updateNodeAdded 1\n                -useDrawOverrideColor 0\n                -limitGraphTraversal -1\n                -range -1 -1 \n                -iconSize \"smallIcons\" \n                -showCachedConnections 0\n                $editorName;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Hypergraph Hierarchy\")) -mbv $menusOkayInPanels  $panelName;\n\n\t\t\t$editorName = ($panelName+\"HyperGraphEd\");\n            hyperGraph -e \n                -graphLayoutStyle \"hierarchicalLayout\" \n                -orientation \"horiz\" \n                -mergeConnections 0\n                -zoom 1\n                -animateTransition 0\n                -showRelationships 1\n                -showShapes 0\n                -showDeformers 0\n                -showExpressions 0\n                -showConstraints 0\n                -showUnderworld 0\n                -showInvisible 0\n"
		+ "                -transitionFrames 1\n                -opaqueContainers 0\n                -freeform 0\n                -imagePosition 0 0 \n                -imageScale 1\n                -imageEnabled 0\n                -graphType \"DAG\" \n                -heatMapDisplay 0\n                -updateSelection 1\n                -updateNodeAdded 1\n                -useDrawOverrideColor 0\n                -limitGraphTraversal -1\n                -range -1 -1 \n                -iconSize \"smallIcons\" \n                -showCachedConnections 0\n                $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"hyperShadePanel\" (localizedPanelLabel(\"Hypershade\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"hyperShadePanel\" -l (localizedPanelLabel(\"Hypershade\")) -mbv $menusOkayInPanels `;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Hypershade\")) -mbv $menusOkayInPanels  $panelName;\n"
		+ "\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\tif ($useSceneConfig) {\n\t\tscriptedPanel -e -to $panelName;\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"visorPanel\" (localizedPanelLabel(\"Visor\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"visorPanel\" -l (localizedPanelLabel(\"Visor\")) -mbv $menusOkayInPanels `;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Visor\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"nodeEditorPanel\" (localizedPanelLabel(\"Node Editor\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"nodeEditorPanel\" -l (localizedPanelLabel(\"Node Editor\")) -mbv $menusOkayInPanels `;\n\n\t\t\t$editorName = ($panelName+\"NodeEditorEd\");\n            nodeEditor -e \n                -allAttributes 0\n"
		+ "                -allNodes 0\n                -autoSizeNodes 1\n                -createNodeCommand \"nodeEdCreateNodeCommand\" \n                -ignoreAssets 1\n                -additiveGraphingMode 0\n                -settingsChangedCallback \"nodeEdSyncControls\" \n                -traversalDepthLimit -1\n                -keyPressCommand \"nodeEdKeyPressCommand\" \n                -popupMenuScript \"nodeEdBuildPanelMenus\" \n                -island 0\n                -showShapes 1\n                -showSGShapes 0\n                -showTransforms 1\n                -syncedSelection 1\n                -extendToShapes 1\n                $editorName;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Node Editor\")) -mbv $menusOkayInPanels  $panelName;\n\n\t\t\t$editorName = ($panelName+\"NodeEditorEd\");\n            nodeEditor -e \n                -allAttributes 0\n                -allNodes 0\n                -autoSizeNodes 1\n                -createNodeCommand \"nodeEdCreateNodeCommand\" \n                -ignoreAssets 1\n"
		+ "                -additiveGraphingMode 0\n                -settingsChangedCallback \"nodeEdSyncControls\" \n                -traversalDepthLimit -1\n                -keyPressCommand \"nodeEdKeyPressCommand\" \n                -popupMenuScript \"nodeEdBuildPanelMenus\" \n                -island 0\n                -showShapes 1\n                -showSGShapes 0\n                -showTransforms 1\n                -syncedSelection 1\n                -extendToShapes 1\n                $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"createNodePanel\" (localizedPanelLabel(\"Create Node\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"createNodePanel\" -l (localizedPanelLabel(\"Create Node\")) -mbv $menusOkayInPanels `;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Create Node\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n"
		+ "\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"polyTexturePlacementPanel\" (localizedPanelLabel(\"UV Texture Editor\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"polyTexturePlacementPanel\" -l (localizedPanelLabel(\"UV Texture Editor\")) -mbv $menusOkayInPanels `;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"UV Texture Editor\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"renderWindowPanel\" (localizedPanelLabel(\"Render View\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"renderWindowPanel\" -l (localizedPanelLabel(\"Render View\")) -mbv $menusOkayInPanels `;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Render View\")) -mbv $menusOkayInPanels  $panelName;\n"
		+ "\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\tif ($useSceneConfig) {\n\t\tscriptedPanel -e -to $panelName;\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextPanel \"blendShapePanel\" (localizedPanelLabel(\"Blend Shape\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\tblendShapePanel -unParent -l (localizedPanelLabel(\"Blend Shape\")) -mbv $menusOkayInPanels ;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tblendShapePanel -edit -l (localizedPanelLabel(\"Blend Shape\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"dynRelEdPanel\" (localizedPanelLabel(\"Dynamic Relationships\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"dynRelEdPanel\" -l (localizedPanelLabel(\"Dynamic Relationships\")) -mbv $menusOkayInPanels `;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Dynamic Relationships\")) -mbv $menusOkayInPanels  $panelName;\n"
		+ "\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"relationshipPanel\" (localizedPanelLabel(\"Relationship Editor\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"relationshipPanel\" -l (localizedPanelLabel(\"Relationship Editor\")) -mbv $menusOkayInPanels `;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Relationship Editor\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"referenceEditorPanel\" (localizedPanelLabel(\"Reference Editor\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"referenceEditorPanel\" -l (localizedPanelLabel(\"Reference Editor\")) -mbv $menusOkayInPanels `;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Reference Editor\")) -mbv $menusOkayInPanels  $panelName;\n"
		+ "\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"componentEditorPanel\" (localizedPanelLabel(\"Component Editor\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"componentEditorPanel\" -l (localizedPanelLabel(\"Component Editor\")) -mbv $menusOkayInPanels `;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Component Editor\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"dynPaintScriptedPanelType\" (localizedPanelLabel(\"Paint Effects\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"dynPaintScriptedPanelType\" -l (localizedPanelLabel(\"Paint Effects\")) -mbv $menusOkayInPanels `;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Paint Effects\")) -mbv $menusOkayInPanels  $panelName;\n"
		+ "\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"scriptEditorPanel\" (localizedPanelLabel(\"Script Editor\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"scriptEditorPanel\" -l (localizedPanelLabel(\"Script Editor\")) -mbv $menusOkayInPanels `;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Script Editor\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"Stereo\" (localizedPanelLabel(\"Stereo\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"Stereo\" -l (localizedPanelLabel(\"Stereo\")) -mbv $menusOkayInPanels `;\nstring $editorName = ($panelName+\"Editor\");\n            stereoCameraView -e \n                -camera \"persp\" \n                -useInteractiveMode 0\n                -displayLights \"default\" \n"
		+ "                -displayAppearance \"wireframe\" \n                -activeOnly 0\n                -ignorePanZoom 0\n                -wireframeOnShaded 0\n                -headsUpDisplay 1\n                -selectionHiliteDisplay 1\n                -useDefaultMaterial 0\n                -bufferMode \"double\" \n                -twoSidedLighting 1\n                -backfaceCulling 0\n                -xray 0\n                -jointXray 0\n                -activeComponentsXray 0\n                -displayTextures 0\n                -smoothWireframe 0\n                -lineWidth 1\n                -textureAnisotropic 0\n                -textureHilight 1\n                -textureSampling 2\n                -textureDisplay \"modulate\" \n                -textureMaxSize 16384\n                -fogging 0\n                -fogSource \"fragment\" \n                -fogMode \"linear\" \n                -fogStart 0\n                -fogEnd 100\n                -fogDensity 0.1\n                -fogColor 0.5 0.5 0.5 1 \n                -maxConstantTransparency 1\n"
		+ "                -objectFilterShowInHUD 1\n                -isFiltered 0\n                -colorResolution 4 4 \n                -bumpResolution 4 4 \n                -textureCompression 0\n                -transparencyAlgorithm \"frontAndBackCull\" \n                -transpInShadows 0\n                -cullingOverride \"none\" \n                -lowQualityLighting 0\n                -maximumNumHardwareLights 0\n                -occlusionCulling 0\n                -shadingModel 0\n                -useBaseRenderer 0\n                -useReducedRenderer 0\n                -smallObjectCulling 0\n                -smallObjectThreshold -1 \n                -interactiveDisableShadows 0\n                -interactiveBackFaceCull 0\n                -sortTransparent 1\n                -nurbsCurves 1\n                -nurbsSurfaces 1\n                -polymeshes 1\n                -subdivSurfaces 1\n                -planes 1\n                -lights 1\n                -cameras 1\n                -controlVertices 1\n                -hulls 1\n                -grid 1\n"
		+ "                -imagePlane 1\n                -joints 1\n                -ikHandles 1\n                -deformers 1\n                -dynamics 1\n                -fluids 1\n                -hairSystems 1\n                -follicles 1\n                -nCloths 1\n                -nParticles 1\n                -nRigids 1\n                -dynamicConstraints 1\n                -locators 1\n                -manipulators 1\n                -dimensions 1\n                -handles 1\n                -pivots 1\n                -textures 1\n                -strokes 1\n                -motionTrails 1\n                -clipGhosts 1\n                -shadows 0\n                -displayMode \"centerEye\" \n                -viewColor 0 0 0 1 \n                $editorName;\nstereoCameraView -e -viewSelected 0 $editorName;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Stereo\")) -mbv $menusOkayInPanels  $panelName;\nstring $editorName = ($panelName+\"Editor\");\n            stereoCameraView -e \n                -camera \"persp\" \n"
		+ "                -useInteractiveMode 0\n                -displayLights \"default\" \n                -displayAppearance \"wireframe\" \n                -activeOnly 0\n                -ignorePanZoom 0\n                -wireframeOnShaded 0\n                -headsUpDisplay 1\n                -selectionHiliteDisplay 1\n                -useDefaultMaterial 0\n                -bufferMode \"double\" \n                -twoSidedLighting 1\n                -backfaceCulling 0\n                -xray 0\n                -jointXray 0\n                -activeComponentsXray 0\n                -displayTextures 0\n                -smoothWireframe 0\n                -lineWidth 1\n                -textureAnisotropic 0\n                -textureHilight 1\n                -textureSampling 2\n                -textureDisplay \"modulate\" \n                -textureMaxSize 16384\n                -fogging 0\n                -fogSource \"fragment\" \n                -fogMode \"linear\" \n                -fogStart 0\n                -fogEnd 100\n                -fogDensity 0.1\n                -fogColor 0.5 0.5 0.5 1 \n"
		+ "                -maxConstantTransparency 1\n                -objectFilterShowInHUD 1\n                -isFiltered 0\n                -colorResolution 4 4 \n                -bumpResolution 4 4 \n                -textureCompression 0\n                -transparencyAlgorithm \"frontAndBackCull\" \n                -transpInShadows 0\n                -cullingOverride \"none\" \n                -lowQualityLighting 0\n                -maximumNumHardwareLights 0\n                -occlusionCulling 0\n                -shadingModel 0\n                -useBaseRenderer 0\n                -useReducedRenderer 0\n                -smallObjectCulling 0\n                -smallObjectThreshold -1 \n                -interactiveDisableShadows 0\n                -interactiveBackFaceCull 0\n                -sortTransparent 1\n                -nurbsCurves 1\n                -nurbsSurfaces 1\n                -polymeshes 1\n                -subdivSurfaces 1\n                -planes 1\n                -lights 1\n                -cameras 1\n                -controlVertices 1\n"
		+ "                -hulls 1\n                -grid 1\n                -imagePlane 1\n                -joints 1\n                -ikHandles 1\n                -deformers 1\n                -dynamics 1\n                -fluids 1\n                -hairSystems 1\n                -follicles 1\n                -nCloths 1\n                -nParticles 1\n                -nRigids 1\n                -dynamicConstraints 1\n                -locators 1\n                -manipulators 1\n                -dimensions 1\n                -handles 1\n                -pivots 1\n                -textures 1\n                -strokes 1\n                -motionTrails 1\n                -clipGhosts 1\n                -shadows 0\n                -displayMode \"centerEye\" \n                -viewColor 0 0 0 1 \n                $editorName;\nstereoCameraView -e -viewSelected 0 $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\tif ($useSceneConfig) {\n        string $configName = `getPanel -cwl (localizedPanelLabel(\"Current Layout\"))`;\n        if (\"\" != $configName) {\n"
		+ "\t\t\tpanelConfiguration -edit -label (localizedPanelLabel(\"Current Layout\")) \n\t\t\t\t-defaultImage \"\"\n\t\t\t\t-image \"\"\n\t\t\t\t-sc false\n\t\t\t\t-configString \"global string $gMainPane; paneLayout -e -cn \\\"single\\\" -ps 1 100 100 $gMainPane;\"\n\t\t\t\t-removeAllPanels\n\t\t\t\t-ap false\n\t\t\t\t\t(localizedPanelLabel(\"Persp View\")) \n\t\t\t\t\t\"modelPanel\"\n"
		+ "\t\t\t\t\t\"$panelName = `modelPanel -unParent -l (localizedPanelLabel(\\\"Persp View\\\")) -mbv $menusOkayInPanels `;\\n$editorName = $panelName;\\nmodelEditor -e \\n    -cam `findStartUpCamera persp` \\n    -useInteractiveMode 0\\n    -displayLights \\\"default\\\" \\n    -displayAppearance \\\"wireframe\\\" \\n    -activeOnly 0\\n    -ignorePanZoom 0\\n    -wireframeOnShaded 0\\n    -headsUpDisplay 1\\n    -selectionHiliteDisplay 1\\n    -useDefaultMaterial 0\\n    -bufferMode \\\"double\\\" \\n    -twoSidedLighting 1\\n    -backfaceCulling 0\\n    -xray 0\\n    -jointXray 0\\n    -activeComponentsXray 0\\n    -displayTextures 0\\n    -smoothWireframe 0\\n    -lineWidth 1\\n    -textureAnisotropic 0\\n    -textureHilight 1\\n    -textureSampling 2\\n    -textureDisplay \\\"modulate\\\" \\n    -textureMaxSize 16384\\n    -fogging 0\\n    -fogSource \\\"fragment\\\" \\n    -fogMode \\\"linear\\\" \\n    -fogStart 0\\n    -fogEnd 100\\n    -fogDensity 0.1\\n    -fogColor 0.5 0.5 0.5 1 \\n    -maxConstantTransparency 1\\n    -rendererName \\\"base_OpenGL_Renderer\\\" \\n    -objectFilterShowInHUD 1\\n    -isFiltered 0\\n    -colorResolution 256 256 \\n    -bumpResolution 512 512 \\n    -textureCompression 0\\n    -transparencyAlgorithm \\\"frontAndBackCull\\\" \\n    -transpInShadows 0\\n    -cullingOverride \\\"none\\\" \\n    -lowQualityLighting 0\\n    -maximumNumHardwareLights 1\\n    -occlusionCulling 0\\n    -shadingModel 0\\n    -useBaseRenderer 0\\n    -useReducedRenderer 0\\n    -smallObjectCulling 0\\n    -smallObjectThreshold -1 \\n    -interactiveDisableShadows 0\\n    -interactiveBackFaceCull 0\\n    -sortTransparent 1\\n    -nurbsCurves 1\\n    -nurbsSurfaces 1\\n    -polymeshes 1\\n    -subdivSurfaces 1\\n    -planes 1\\n    -lights 1\\n    -cameras 1\\n    -controlVertices 1\\n    -hulls 1\\n    -grid 1\\n    -imagePlane 1\\n    -joints 1\\n    -ikHandles 1\\n    -deformers 1\\n    -dynamics 1\\n    -fluids 1\\n    -hairSystems 1\\n    -follicles 1\\n    -nCloths 1\\n    -nParticles 1\\n    -nRigids 1\\n    -dynamicConstraints 1\\n    -locators 1\\n    -manipulators 1\\n    -dimensions 1\\n    -handles 1\\n    -pivots 1\\n    -textures 1\\n    -strokes 1\\n    -motionTrails 1\\n    -clipGhosts 1\\n    -shadows 0\\n    $editorName;\\nmodelEditor -e -viewSelected 0 $editorName\"\n"
		+ "\t\t\t\t\t\"modelPanel -edit -l (localizedPanelLabel(\\\"Persp View\\\")) -mbv $menusOkayInPanels  $panelName;\\n$editorName = $panelName;\\nmodelEditor -e \\n    -cam `findStartUpCamera persp` \\n    -useInteractiveMode 0\\n    -displayLights \\\"default\\\" \\n    -displayAppearance \\\"wireframe\\\" \\n    -activeOnly 0\\n    -ignorePanZoom 0\\n    -wireframeOnShaded 0\\n    -headsUpDisplay 1\\n    -selectionHiliteDisplay 1\\n    -useDefaultMaterial 0\\n    -bufferMode \\\"double\\\" \\n    -twoSidedLighting 1\\n    -backfaceCulling 0\\n    -xray 0\\n    -jointXray 0\\n    -activeComponentsXray 0\\n    -displayTextures 0\\n    -smoothWireframe 0\\n    -lineWidth 1\\n    -textureAnisotropic 0\\n    -textureHilight 1\\n    -textureSampling 2\\n    -textureDisplay \\\"modulate\\\" \\n    -textureMaxSize 16384\\n    -fogging 0\\n    -fogSource \\\"fragment\\\" \\n    -fogMode \\\"linear\\\" \\n    -fogStart 0\\n    -fogEnd 100\\n    -fogDensity 0.1\\n    -fogColor 0.5 0.5 0.5 1 \\n    -maxConstantTransparency 1\\n    -rendererName \\\"base_OpenGL_Renderer\\\" \\n    -objectFilterShowInHUD 1\\n    -isFiltered 0\\n    -colorResolution 256 256 \\n    -bumpResolution 512 512 \\n    -textureCompression 0\\n    -transparencyAlgorithm \\\"frontAndBackCull\\\" \\n    -transpInShadows 0\\n    -cullingOverride \\\"none\\\" \\n    -lowQualityLighting 0\\n    -maximumNumHardwareLights 1\\n    -occlusionCulling 0\\n    -shadingModel 0\\n    -useBaseRenderer 0\\n    -useReducedRenderer 0\\n    -smallObjectCulling 0\\n    -smallObjectThreshold -1 \\n    -interactiveDisableShadows 0\\n    -interactiveBackFaceCull 0\\n    -sortTransparent 1\\n    -nurbsCurves 1\\n    -nurbsSurfaces 1\\n    -polymeshes 1\\n    -subdivSurfaces 1\\n    -planes 1\\n    -lights 1\\n    -cameras 1\\n    -controlVertices 1\\n    -hulls 1\\n    -grid 1\\n    -imagePlane 1\\n    -joints 1\\n    -ikHandles 1\\n    -deformers 1\\n    -dynamics 1\\n    -fluids 1\\n    -hairSystems 1\\n    -follicles 1\\n    -nCloths 1\\n    -nParticles 1\\n    -nRigids 1\\n    -dynamicConstraints 1\\n    -locators 1\\n    -manipulators 1\\n    -dimensions 1\\n    -handles 1\\n    -pivots 1\\n    -textures 1\\n    -strokes 1\\n    -motionTrails 1\\n    -clipGhosts 1\\n    -shadows 0\\n    $editorName;\\nmodelEditor -e -viewSelected 0 $editorName\"\n"
		+ "\t\t\t\t$configName;\n\n            setNamedPanelLayout (localizedPanelLabel(\"Current Layout\"));\n        }\n\n        panelHistory -e -clear mainPanelHistory;\n        setFocus `paneLayout -q -p1 $gMainPane`;\n        sceneUIReplacement -deleteRemaining;\n        sceneUIReplacement -clear;\n\t}\n\n\ngrid -spacing 5 -size 12 -divisions 5 -displayAxes yes -displayGridLines yes -displayDivisionLines yes -displayPerspectiveLabels no -displayOrthographicLabels no -displayAxesBold yes -perspectiveLabelPosition axis -orthographicLabelPosition edge;\nviewManip -drawCompass 0 -compassAngle 0 -frontParameters \"\" -homeParameters \"\" -selectionLockParameters \"\";\n}\n");
	setAttr ".st" 3;
createNode script -n "sceneConfigurationScriptNode";
	setAttr ".b" -type "string" "playbackOptions -min 1 -max 24 -ast 1 -aet 48 ";
	setAttr ".st" 6;
createNode shadingEngine -n "aiStandard1SG";
	setAttr ".ihi" 0;
	setAttr ".ro" yes;
createNode materialInfo -n "materialInfo1";
createNode polySphere -n "polySphere1";
	setAttr ".r" 6.3190084466120737;
createNode aiAOVFilter -n "aiAOVFilter406";
createNode aiAOVFilter -n "aiAOVFilter405";
createNode aiAOVFilter -n "aiAOVFilter404";
createNode aiAOVFilter -n "aiAOVFilter403";
createNode aiAOVFilter -n "aiAOVFilter402";
createNode aiAOVFilter -n "aiAOVFilter401";
createNode aiAOVFilter -n "aiAOVFilter400";
createNode aiAOVFilter -n "aiAOVFilter399";
createNode aiAOVFilter -n "aiAOVFilter398";
createNode aiAOVFilter -n "aiAOVFilter397";
createNode aiAOVFilter -n "aiAOVFilter396";
createNode aiAOVFilter -n "aiAOVFilter395";
createNode aiAOVFilter -n "aiAOVFilter394";
createNode aiAOVFilter -n "aiAOVFilter393";
createNode aiAOVFilter -n "aiAOVFilter392";
createNode aiAOVFilter -n "aiAOVFilter391";
createNode aiAOVFilter -n "aiAOVFilter390";
createNode aiAOVFilter -n "aiAOVFilter389";
createNode aiAOVFilter -n "aiAOVFilter388";
createNode aiAOVFilter -n "aiAOVFilter387";
createNode aiAOVFilter -n "aiAOVFilter386";
createNode aiAOVFilter -n "aiAOVFilter385";
createNode aiAOVFilter -n "aiAOVFilter384";
createNode aiAOVFilter -n "aiAOVFilter383";
createNode aiAOVFilter -n "aiAOVFilter382";
createNode aiAOVFilter -n "aiAOVFilter381";
createNode aiAOVFilter -n "aiAOVFilter380";
createNode aiAOVFilter -n "aiAOVFilter379";
createNode aiAOVFilter -n "aiAOVFilter378";
createNode aiAOVFilter -n "aiAOVFilter377";
createNode aiAOVFilter -n "aiAOVFilter376";
createNode aiAOVFilter -n "aiAOVFilter375";
createNode aiAOVFilter -n "aiAOVFilter374";
createNode aiAOVFilter -n "aiAOVFilter373";
createNode aiAOVFilter -n "aiAOVFilter372";
createNode aiAOVFilter -n "aiAOVFilter371";
createNode aiAOVFilter -n "aiAOVFilter370";
createNode aiAOVFilter -n "aiAOVFilter369";
createNode aiAOVFilter -n "aiAOVFilter368";
createNode aiAOVFilter -n "aiAOVFilter367";
createNode aiAOVFilter -n "aiAOVFilter366";
createNode aiAOVFilter -n "aiAOVFilter365";
createNode aiAOVFilter -n "aiAOVFilter364";
createNode aiAOVFilter -n "aiAOVFilter363";
createNode aiAOVFilter -n "aiAOVFilter362";
createNode aiAOVFilter -n "aiAOVFilter361";
createNode aiAOVFilter -n "aiAOVFilter360";
createNode aiAOVFilter -n "aiAOVFilter359";
createNode aiAOVFilter -n "aiAOVFilter358";
createNode aiAOVFilter -n "aiAOVFilter357";
createNode aiAOVFilter -n "aiAOVFilter356";
createNode aiAOVFilter -n "aiAOVFilter355";
createNode aiAOVFilter -n "aiAOVFilter354";
createNode aiAOVFilter -n "aiAOVFilter353";
createNode aiAOVFilter -n "aiAOVFilter352";
createNode aiAOVFilter -n "aiAOVFilter351";
createNode aiAOVFilter -n "aiAOVFilter350";
createNode aiAOVFilter -n "aiAOVFilter349";
createNode aiAOVFilter -n "aiAOVFilter348";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter347";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter346";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter345";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter344";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter343";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter342";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter341";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter340";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter339";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter338";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter337";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter336";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter335";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter334";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter333";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter332";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter331";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter330";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter329";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter328";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter327";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter326";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter325";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter324";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter323";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter322";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter321";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter320";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter319";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter318";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter317";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter316";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter315";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter314";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter313";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter312";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter311";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter310";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter309";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter308";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter307";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter306";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter305";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter304";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter303";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter302";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter301";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter300";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter299";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter298";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter297";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter296";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter295";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter294";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter293";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter292";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter291";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter290";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter289";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter288";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter287";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter286";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter285";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter284";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter283";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter282";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter281";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter280";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter279";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter278";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter277";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter276";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter275";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter274";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter273";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter272";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter271";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter270";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter269";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter268";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter267";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter266";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter265";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter264";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter263";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter262";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter261";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter260";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter259";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter258";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter257";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter256";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter255";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter254";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter253";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter252";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter251";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter250";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter249";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter248";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter247";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter246";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter245";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter244";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter243";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter242";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter241";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter240";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter239";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter238";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter237";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter236";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter235";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter234";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter233";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter232";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter231";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter230";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter229";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter228";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter227";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter226";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter225";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter224";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter223";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter222";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter221";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter220";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter219";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter218";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter217";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter216";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter215";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter214";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter213";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter212";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter211";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter210";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter209";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter208";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter207";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter206";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter205";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter204";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter203";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter202";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter201";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter200";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter199";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter198";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter197";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter196";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter195";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter194";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter193";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter192";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter191";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter190";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter189";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter188";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter187";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter186";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter185";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter184";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter183";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter182";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter181";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter180";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter179";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter178";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter177";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter176";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter175";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter174";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter173";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter172";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter171";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter170";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter169";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter168";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter167";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter166";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter165";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter164";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter163";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter162";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter161";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter160";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter159";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter158";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter157";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter156";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter155";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter154";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter153";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter152";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter151";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter150";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter149";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter148";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter147";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter146";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter145";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter144";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter143";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter142";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter141";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter140";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter139";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter138";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter137";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter136";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter135";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter134";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter133";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter132";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter131";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter130";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter129";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter128";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter127";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter126";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter125";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter124";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter123";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter122";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter121";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter120";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter119";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter118";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter117";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter116";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter115";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter114";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter113";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter112";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter111";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter110";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter109";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter108";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter107";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter106";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter105";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter104";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter103";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter102";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter101";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter100";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter99";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter98";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter97";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter96";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter95";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter94";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter93";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter92";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter91";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter90";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter89";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter88";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter87";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter86";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter85";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter84";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter83";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter82";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter81";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter80";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter79";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter78";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter77";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter76";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter75";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter74";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter73";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter72";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter71";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter70";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter69";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter68";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter67";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter66";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter65";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter64";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter63";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter62";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter61";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter60";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter59";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter58";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter57";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter56";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter55";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter54";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter53";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter52";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter51";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter50";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter49";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter48";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter47";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter46";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter45";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter44";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter43";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter42";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter41";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter40";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter39";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter38";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter37";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter36";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter35";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter34";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter33";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter32";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter31";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter30";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter29";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter28";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter27";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter26";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter25";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter24";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter23";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter22";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter21";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter20";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter19";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter18";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter17";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter16";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter15";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter14";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter13";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter12";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter11";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter10";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter9";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter8";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter7";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter6";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter5";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter4";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter3";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter2";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -n "aiAOVFilter1";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVFilter -s -n "defaultArnoldFilter";
	setAttr ".ai_translator" -type "string" "gaussian";
createNode aiAOVDriver -n "aiAOVDriver406";
createNode aiAOVDriver -n "aiAOVDriver405";
createNode aiAOVDriver -n "aiAOVDriver404";
createNode aiAOVDriver -n "aiAOVDriver403";
createNode aiAOVDriver -n "aiAOVDriver402";
createNode aiAOVDriver -n "aiAOVDriver401";
createNode aiAOVDriver -n "aiAOVDriver400";
createNode aiAOVDriver -n "aiAOVDriver399";
createNode aiAOVDriver -n "aiAOVDriver398";
createNode aiAOVDriver -n "aiAOVDriver397";
createNode aiAOVDriver -n "aiAOVDriver396";
createNode aiAOVDriver -n "aiAOVDriver395";
createNode aiAOVDriver -n "aiAOVDriver394";
createNode aiAOVDriver -n "aiAOVDriver393";
createNode aiAOVDriver -n "aiAOVDriver392";
createNode aiAOVDriver -n "aiAOVDriver391";
createNode aiAOVDriver -n "aiAOVDriver390";
createNode aiAOVDriver -n "aiAOVDriver389";
createNode aiAOVDriver -n "aiAOVDriver388";
createNode aiAOVDriver -n "aiAOVDriver387";
createNode aiAOVDriver -n "aiAOVDriver386";
createNode aiAOVDriver -n "aiAOVDriver385";
createNode aiAOVDriver -n "aiAOVDriver384";
createNode aiAOVDriver -n "aiAOVDriver383";
createNode aiAOVDriver -n "aiAOVDriver382";
createNode aiAOVDriver -n "aiAOVDriver381";
createNode aiAOVDriver -n "aiAOVDriver380";
createNode aiAOVDriver -n "aiAOVDriver379";
createNode aiAOVDriver -n "aiAOVDriver378";
createNode aiAOVDriver -n "aiAOVDriver377";
createNode aiAOVDriver -n "aiAOVDriver376";
createNode aiAOVDriver -n "aiAOVDriver375";
createNode aiAOVDriver -n "aiAOVDriver374";
createNode aiAOVDriver -n "aiAOVDriver373";
createNode aiAOVDriver -n "aiAOVDriver372";
createNode aiAOVDriver -n "aiAOVDriver371";
createNode aiAOVDriver -n "aiAOVDriver370";
createNode aiAOVDriver -n "aiAOVDriver369";
createNode aiAOVDriver -n "aiAOVDriver368";
createNode aiAOVDriver -n "aiAOVDriver367";
createNode aiAOVDriver -n "aiAOVDriver366";
createNode aiAOVDriver -n "aiAOVDriver365";
createNode aiAOVDriver -n "aiAOVDriver364";
createNode aiAOVDriver -n "aiAOVDriver363";
createNode aiAOVDriver -n "aiAOVDriver362";
createNode aiAOVDriver -n "aiAOVDriver361";
createNode aiAOVDriver -n "aiAOVDriver360";
createNode aiAOVDriver -n "aiAOVDriver359";
createNode aiAOVDriver -n "aiAOVDriver358";
createNode aiAOVDriver -n "aiAOVDriver357";
createNode aiAOVDriver -n "aiAOVDriver356";
createNode aiAOVDriver -n "aiAOVDriver355";
createNode aiAOVDriver -n "aiAOVDriver354";
createNode aiAOVDriver -n "aiAOVDriver353";
createNode aiAOVDriver -n "aiAOVDriver352";
createNode aiAOVDriver -n "aiAOVDriver351";
createNode aiAOVDriver -n "aiAOVDriver350";
createNode aiAOVDriver -n "aiAOVDriver349";
createNode aiAOVDriver -n "aiAOVDriver348";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver347";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver346";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver345";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver344";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver343";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver342";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver341";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver340";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver339";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver338";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver337";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver336";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver335";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver334";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver333";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver332";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver331";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver330";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver329";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver328";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver327";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver326";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver325";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver324";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver323";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver322";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver321";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver320";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver319";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver318";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver317";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver316";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver315";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver314";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver313";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver312";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver311";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver310";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver309";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver308";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver307";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver306";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver305";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver304";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver303";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver302";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver301";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver300";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver299";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver298";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver297";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver296";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver295";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver294";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver293";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver292";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver291";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver290";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver289";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver288";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver287";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver286";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver285";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver284";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver283";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver282";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver281";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver280";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver279";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver278";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver277";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver276";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver275";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver274";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver273";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver272";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver271";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver270";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver269";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver268";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver267";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver266";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver265";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver264";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver263";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver262";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver261";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver260";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver259";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver258";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver257";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver256";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver255";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver254";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver253";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver252";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver251";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver250";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver249";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver248";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver247";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver246";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver245";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver244";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver243";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver242";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver241";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver240";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver239";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver238";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver237";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver236";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver235";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver234";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver233";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver232";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver231";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver230";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver229";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver228";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver227";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver226";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver225";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver224";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver223";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver222";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver221";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver220";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver219";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver218";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver217";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver216";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver215";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver214";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver213";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver212";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver211";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver210";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver209";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver208";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver207";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver206";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver205";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver204";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver203";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver202";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver201";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver200";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver199";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver198";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver197";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver196";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver195";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver194";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver193";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver192";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver191";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver190";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver189";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver188";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver187";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver186";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver185";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver184";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver183";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver182";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver181";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver180";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver179";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver178";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver177";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver176";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver175";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver174";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver173";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver172";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver171";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver170";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver169";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver168";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver167";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver166";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver165";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver164";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver163";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver162";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver161";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver160";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver159";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver158";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver157";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver156";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver155";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver154";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver153";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver152";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver151";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver150";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver149";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver148";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver147";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver146";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver145";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver144";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver143";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver142";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver141";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver140";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver139";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver138";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver137";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver136";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver135";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver134";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver133";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver132";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver131";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver130";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver129";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver128";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver127";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver126";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver125";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver124";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver123";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver122";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver121";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver120";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver119";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver118";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver117";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver116";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver115";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver114";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver113";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver112";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver111";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver110";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver109";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver108";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver107";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver106";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver105";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver104";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver103";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver102";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver101";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver100";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver99";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver98";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver97";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver96";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver95";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver94";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver93";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver92";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver91";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver90";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver89";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver88";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver87";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver86";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver85";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver84";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver83";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver82";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver81";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver80";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver79";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver78";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver77";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver76";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver75";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver74";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver73";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver72";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver71";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver70";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver69";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver68";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver67";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver66";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver65";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver64";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver63";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver62";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver61";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver60";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver59";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver58";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver57";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver56";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver55";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver54";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver53";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver52";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver51";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver50";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver49";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver48";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver47";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver46";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver45";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver44";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver43";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver42";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver41";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver40";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver39";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver38";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver37";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver36";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver35";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver34";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver33";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver32";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver31";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver30";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver29";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver28";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver27";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver26";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver25";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver24";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver23";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver22";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver21";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver20";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver19";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver18";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver17";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver16";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver15";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver14";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver13";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver12";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver11";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver10";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver9";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver8";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver7";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver6";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver5";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver4";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver3";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver2";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -n "aiAOVDriver1";
	setAttr ".ai_translator" -type "string" "exr";
createNode aiAOVDriver -s -n "defaultArnoldDriver";
	setAttr ".tiled" no;
	setAttr ".ai_translator" -type "string" "tif";
createNode aiShadowCatcher -n "aiShadowCatcher1";
	setAttr ".shadowColor" -type "float3" 1 1 1 ;
	setAttr ".enableTransparency" yes;
createNode aiUtility -n "aiUtility1";
	setAttr ".shade_mode" 2;
createNode shadingEngine -n "aiUtility1SG";
	setAttr ".ihi" 0;
	setAttr ".ro" yes;
createNode materialInfo -n "materialInfo2";
createNode aiStandard -n "aiStandard2";
createNode shadingEngine -n "aiStandard2SG";
	setAttr ".ihi" 0;
	setAttr ".ro" yes;
createNode materialInfo -n "materialInfo3";
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
select -ne :defaultRenderingList1;
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
connectAttr "nucleus1.noao[0]" "hairSystemShape1.nxst";
connectAttr "nucleus1.stf" "hairSystemShape1.stf";
connectAttr "pSphere1FollicleShape303.ot" "pSphere1Follicle303.t" -l on;
connectAttr "pSphere1FollicleShape303.or" "pSphere1Follicle303.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape303.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape303.inm";
connectAttr "curveShape1.l" "pSphere1FollicleShape303.sp";
connectAttr "curve1.wm" "pSphere1FollicleShape303.spm";
connectAttr "pSphere1FollicleShape309.ot" "pSphere1Follicle309.t" -l on;
connectAttr "pSphere1FollicleShape309.or" "pSphere1Follicle309.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape309.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape309.inm";
connectAttr "curveShape2.l" "pSphere1FollicleShape309.sp";
connectAttr "curve2.wm" "pSphere1FollicleShape309.spm";
connectAttr "pSphere1FollicleShape315.ot" "pSphere1Follicle315.t" -l on;
connectAttr "pSphere1FollicleShape315.or" "pSphere1Follicle315.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape315.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape315.inm";
connectAttr "curveShape3.l" "pSphere1FollicleShape315.sp";
connectAttr "curve3.wm" "pSphere1FollicleShape315.spm";
connectAttr "pSphere1FollicleShape322.ot" "pSphere1Follicle322.t" -l on;
connectAttr "pSphere1FollicleShape322.or" "pSphere1Follicle322.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape322.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape322.inm";
connectAttr "curveShape4.l" "pSphere1FollicleShape322.sp";
connectAttr "curve4.wm" "pSphere1FollicleShape322.spm";
connectAttr "pSphere1FollicleShape328.ot" "pSphere1Follicle328.t" -l on;
connectAttr "pSphere1FollicleShape328.or" "pSphere1Follicle328.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape328.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape328.inm";
connectAttr "curveShape5.l" "pSphere1FollicleShape328.sp";
connectAttr "curve5.wm" "pSphere1FollicleShape328.spm";
connectAttr "pSphere1FollicleShape334.ot" "pSphere1Follicle334.t" -l on;
connectAttr "pSphere1FollicleShape334.or" "pSphere1Follicle334.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape334.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape334.inm";
connectAttr "curveShape6.l" "pSphere1FollicleShape334.sp";
connectAttr "curve6.wm" "pSphere1FollicleShape334.spm";
connectAttr "pSphere1FollicleShape340.ot" "pSphere1Follicle340.t" -l on;
connectAttr "pSphere1FollicleShape340.or" "pSphere1Follicle340.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape340.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape340.inm";
connectAttr "curveShape7.l" "pSphere1FollicleShape340.sp";
connectAttr "curve7.wm" "pSphere1FollicleShape340.spm";
connectAttr "pSphere1FollicleShape346.ot" "pSphere1Follicle346.t" -l on;
connectAttr "pSphere1FollicleShape346.or" "pSphere1Follicle346.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape346.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape346.inm";
connectAttr "curveShape8.l" "pSphere1FollicleShape346.sp";
connectAttr "curve8.wm" "pSphere1FollicleShape346.spm";
connectAttr "pSphere1FollicleShape353.ot" "pSphere1Follicle353.t" -l on;
connectAttr "pSphere1FollicleShape353.or" "pSphere1Follicle353.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape353.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape353.inm";
connectAttr "curveShape9.l" "pSphere1FollicleShape353.sp";
connectAttr "curve9.wm" "pSphere1FollicleShape353.spm";
connectAttr "pSphere1FollicleShape359.ot" "pSphere1Follicle359.t" -l on;
connectAttr "pSphere1FollicleShape359.or" "pSphere1Follicle359.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape359.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape359.inm";
connectAttr "curveShape10.l" "pSphere1FollicleShape359.sp";
connectAttr "curve10.wm" "pSphere1FollicleShape359.spm";
connectAttr "pSphere1FollicleShape365.ot" "pSphere1Follicle365.t" -l on;
connectAttr "pSphere1FollicleShape365.or" "pSphere1Follicle365.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape365.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape365.inm";
connectAttr "curveShape11.l" "pSphere1FollicleShape365.sp";
connectAttr "curve11.wm" "pSphere1FollicleShape365.spm";
connectAttr "pSphere1FollicleShape371.ot" "pSphere1Follicle371.t" -l on;
connectAttr "pSphere1FollicleShape371.or" "pSphere1Follicle371.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape371.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape371.inm";
connectAttr "curveShape12.l" "pSphere1FollicleShape371.sp";
connectAttr "curve12.wm" "pSphere1FollicleShape371.spm";
connectAttr "pSphere1FollicleShape377.ot" "pSphere1Follicle377.t" -l on;
connectAttr "pSphere1FollicleShape377.or" "pSphere1Follicle377.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape377.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape377.inm";
connectAttr "curveShape13.l" "pSphere1FollicleShape377.sp";
connectAttr "curve13.wm" "pSphere1FollicleShape377.spm";
connectAttr "pSphere1FollicleShape384.ot" "pSphere1Follicle384.t" -l on;
connectAttr "pSphere1FollicleShape384.or" "pSphere1Follicle384.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape384.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape384.inm";
connectAttr "curveShape14.l" "pSphere1FollicleShape384.sp";
connectAttr "curve14.wm" "pSphere1FollicleShape384.spm";
connectAttr "pSphere1FollicleShape390.ot" "pSphere1Follicle390.t" -l on;
connectAttr "pSphere1FollicleShape390.or" "pSphere1Follicle390.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape390.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape390.inm";
connectAttr "curveShape15.l" "pSphere1FollicleShape390.sp";
connectAttr "curve15.wm" "pSphere1FollicleShape390.spm";
connectAttr "pSphere1FollicleShape396.ot" "pSphere1Follicle396.t" -l on;
connectAttr "pSphere1FollicleShape396.or" "pSphere1Follicle396.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape396.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape396.inm";
connectAttr "curveShape16.l" "pSphere1FollicleShape396.sp";
connectAttr "curve16.wm" "pSphere1FollicleShape396.spm";
connectAttr "pSphere1FollicleShape909.ot" "pSphere1Follicle909.t" -l on;
connectAttr "pSphere1FollicleShape909.or" "pSphere1Follicle909.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape909.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape909.inm";
connectAttr "curveShape17.l" "pSphere1FollicleShape909.sp";
connectAttr "curve17.wm" "pSphere1FollicleShape909.spm";
connectAttr "pSphere1FollicleShape915.ot" "pSphere1Follicle915.t" -l on;
connectAttr "pSphere1FollicleShape915.or" "pSphere1Follicle915.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape915.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape915.inm";
connectAttr "curveShape18.l" "pSphere1FollicleShape915.sp";
connectAttr "curve18.wm" "pSphere1FollicleShape915.spm";
connectAttr "pSphere1FollicleShape922.ot" "pSphere1Follicle922.t" -l on;
connectAttr "pSphere1FollicleShape922.or" "pSphere1Follicle922.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape922.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape922.inm";
connectAttr "curveShape19.l" "pSphere1FollicleShape922.sp";
connectAttr "curve19.wm" "pSphere1FollicleShape922.spm";
connectAttr "pSphere1FollicleShape928.ot" "pSphere1Follicle928.t" -l on;
connectAttr "pSphere1FollicleShape928.or" "pSphere1Follicle928.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape928.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape928.inm";
connectAttr "curveShape20.l" "pSphere1FollicleShape928.sp";
connectAttr "curve20.wm" "pSphere1FollicleShape928.spm";
connectAttr "pSphere1FollicleShape934.ot" "pSphere1Follicle934.t" -l on;
connectAttr "pSphere1FollicleShape934.or" "pSphere1Follicle934.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape934.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape934.inm";
connectAttr "curveShape21.l" "pSphere1FollicleShape934.sp";
connectAttr "curve21.wm" "pSphere1FollicleShape934.spm";
connectAttr "pSphere1FollicleShape940.ot" "pSphere1Follicle940.t" -l on;
connectAttr "pSphere1FollicleShape940.or" "pSphere1Follicle940.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape940.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape940.inm";
connectAttr "curveShape22.l" "pSphere1FollicleShape940.sp";
connectAttr "curve22.wm" "pSphere1FollicleShape940.spm";
connectAttr "pSphere1FollicleShape946.ot" "pSphere1Follicle946.t" -l on;
connectAttr "pSphere1FollicleShape946.or" "pSphere1Follicle946.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape946.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape946.inm";
connectAttr "curveShape23.l" "pSphere1FollicleShape946.sp";
connectAttr "curve23.wm" "pSphere1FollicleShape946.spm";
connectAttr "pSphere1FollicleShape953.ot" "pSphere1Follicle953.t" -l on;
connectAttr "pSphere1FollicleShape953.or" "pSphere1Follicle953.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape953.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape953.inm";
connectAttr "curveShape24.l" "pSphere1FollicleShape953.sp";
connectAttr "curve24.wm" "pSphere1FollicleShape953.spm";
connectAttr "pSphere1FollicleShape959.ot" "pSphere1Follicle959.t" -l on;
connectAttr "pSphere1FollicleShape959.or" "pSphere1Follicle959.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape959.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape959.inm";
connectAttr "curveShape25.l" "pSphere1FollicleShape959.sp";
connectAttr "curve25.wm" "pSphere1FollicleShape959.spm";
connectAttr "pSphere1FollicleShape965.ot" "pSphere1Follicle965.t" -l on;
connectAttr "pSphere1FollicleShape965.or" "pSphere1Follicle965.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape965.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape965.inm";
connectAttr "curveShape26.l" "pSphere1FollicleShape965.sp";
connectAttr "curve26.wm" "pSphere1FollicleShape965.spm";
connectAttr "pSphere1FollicleShape971.ot" "pSphere1Follicle971.t" -l on;
connectAttr "pSphere1FollicleShape971.or" "pSphere1Follicle971.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape971.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape971.inm";
connectAttr "curveShape27.l" "pSphere1FollicleShape971.sp";
connectAttr "curve27.wm" "pSphere1FollicleShape971.spm";
connectAttr "pSphere1FollicleShape977.ot" "pSphere1Follicle977.t" -l on;
connectAttr "pSphere1FollicleShape977.or" "pSphere1Follicle977.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape977.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape977.inm";
connectAttr "curveShape28.l" "pSphere1FollicleShape977.sp";
connectAttr "curve28.wm" "pSphere1FollicleShape977.spm";
connectAttr "pSphere1FollicleShape984.ot" "pSphere1Follicle984.t" -l on;
connectAttr "pSphere1FollicleShape984.or" "pSphere1Follicle984.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape984.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape984.inm";
connectAttr "curveShape29.l" "pSphere1FollicleShape984.sp";
connectAttr "curve29.wm" "pSphere1FollicleShape984.spm";
connectAttr "pSphere1FollicleShape990.ot" "pSphere1Follicle990.t" -l on;
connectAttr "pSphere1FollicleShape990.or" "pSphere1Follicle990.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape990.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape990.inm";
connectAttr "curveShape30.l" "pSphere1FollicleShape990.sp";
connectAttr "curve30.wm" "pSphere1FollicleShape990.spm";
connectAttr "pSphere1FollicleShape1509.ot" "pSphere1Follicle1509.t" -l on;
connectAttr "pSphere1FollicleShape1509.or" "pSphere1Follicle1509.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape1509.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape1509.inm";
connectAttr "curveShape31.l" "pSphere1FollicleShape1509.sp";
connectAttr "curve31.wm" "pSphere1FollicleShape1509.spm";
connectAttr "pSphere1FollicleShape1515.ot" "pSphere1Follicle1515.t" -l on;
connectAttr "pSphere1FollicleShape1515.or" "pSphere1Follicle1515.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape1515.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape1515.inm";
connectAttr "curveShape32.l" "pSphere1FollicleShape1515.sp";
connectAttr "curve32.wm" "pSphere1FollicleShape1515.spm";
connectAttr "pSphere1FollicleShape1522.ot" "pSphere1Follicle1522.t" -l on;
connectAttr "pSphere1FollicleShape1522.or" "pSphere1Follicle1522.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape1522.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape1522.inm";
connectAttr "curveShape33.l" "pSphere1FollicleShape1522.sp";
connectAttr "curve33.wm" "pSphere1FollicleShape1522.spm";
connectAttr "pSphere1FollicleShape1528.ot" "pSphere1Follicle1528.t" -l on;
connectAttr "pSphere1FollicleShape1528.or" "pSphere1Follicle1528.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape1528.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape1528.inm";
connectAttr "curveShape34.l" "pSphere1FollicleShape1528.sp";
connectAttr "curve34.wm" "pSphere1FollicleShape1528.spm";
connectAttr "pSphere1FollicleShape1534.ot" "pSphere1Follicle1534.t" -l on;
connectAttr "pSphere1FollicleShape1534.or" "pSphere1Follicle1534.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape1534.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape1534.inm";
connectAttr "curveShape35.l" "pSphere1FollicleShape1534.sp";
connectAttr "curve35.wm" "pSphere1FollicleShape1534.spm";
connectAttr "pSphere1FollicleShape1540.ot" "pSphere1Follicle1540.t" -l on;
connectAttr "pSphere1FollicleShape1540.or" "pSphere1Follicle1540.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape1540.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape1540.inm";
connectAttr "curveShape36.l" "pSphere1FollicleShape1540.sp";
connectAttr "curve36.wm" "pSphere1FollicleShape1540.spm";
connectAttr "pSphere1FollicleShape1546.ot" "pSphere1Follicle1546.t" -l on;
connectAttr "pSphere1FollicleShape1546.or" "pSphere1Follicle1546.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape1546.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape1546.inm";
connectAttr "curveShape37.l" "pSphere1FollicleShape1546.sp";
connectAttr "curve37.wm" "pSphere1FollicleShape1546.spm";
connectAttr "pSphere1FollicleShape1553.ot" "pSphere1Follicle1553.t" -l on;
connectAttr "pSphere1FollicleShape1553.or" "pSphere1Follicle1553.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape1553.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape1553.inm";
connectAttr "curveShape38.l" "pSphere1FollicleShape1553.sp";
connectAttr "curve38.wm" "pSphere1FollicleShape1553.spm";
connectAttr "pSphere1FollicleShape1559.ot" "pSphere1Follicle1559.t" -l on;
connectAttr "pSphere1FollicleShape1559.or" "pSphere1Follicle1559.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape1559.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape1559.inm";
connectAttr "curveShape39.l" "pSphere1FollicleShape1559.sp";
connectAttr "curve39.wm" "pSphere1FollicleShape1559.spm";
connectAttr "pSphere1FollicleShape1565.ot" "pSphere1Follicle1565.t" -l on;
connectAttr "pSphere1FollicleShape1565.or" "pSphere1Follicle1565.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape1565.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape1565.inm";
connectAttr "curveShape40.l" "pSphere1FollicleShape1565.sp";
connectAttr "curve40.wm" "pSphere1FollicleShape1565.spm";
connectAttr "pSphere1FollicleShape1571.ot" "pSphere1Follicle1571.t" -l on;
connectAttr "pSphere1FollicleShape1571.or" "pSphere1Follicle1571.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape1571.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape1571.inm";
connectAttr "curveShape41.l" "pSphere1FollicleShape1571.sp";
connectAttr "curve41.wm" "pSphere1FollicleShape1571.spm";
connectAttr "pSphere1FollicleShape1577.ot" "pSphere1Follicle1577.t" -l on;
connectAttr "pSphere1FollicleShape1577.or" "pSphere1Follicle1577.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape1577.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape1577.inm";
connectAttr "curveShape42.l" "pSphere1FollicleShape1577.sp";
connectAttr "curve42.wm" "pSphere1FollicleShape1577.spm";
connectAttr "pSphere1FollicleShape1584.ot" "pSphere1Follicle1584.t" -l on;
connectAttr "pSphere1FollicleShape1584.or" "pSphere1Follicle1584.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape1584.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape1584.inm";
connectAttr "curveShape43.l" "pSphere1FollicleShape1584.sp";
connectAttr "curve43.wm" "pSphere1FollicleShape1584.spm";
connectAttr "pSphere1FollicleShape1590.ot" "pSphere1Follicle1590.t" -l on;
connectAttr "pSphere1FollicleShape1590.or" "pSphere1Follicle1590.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape1590.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape1590.inm";
connectAttr "curveShape44.l" "pSphere1FollicleShape1590.sp";
connectAttr "curve44.wm" "pSphere1FollicleShape1590.spm";
connectAttr "pSphere1FollicleShape2203.ot" "pSphere1Follicle2203.t" -l on;
connectAttr "pSphere1FollicleShape2203.or" "pSphere1Follicle2203.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape2203.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape2203.inm";
connectAttr "curveShape45.l" "pSphere1FollicleShape2203.sp";
connectAttr "curve45.wm" "pSphere1FollicleShape2203.spm";
connectAttr "pSphere1FollicleShape2209.ot" "pSphere1Follicle2209.t" -l on;
connectAttr "pSphere1FollicleShape2209.or" "pSphere1Follicle2209.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape2209.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape2209.inm";
connectAttr "curveShape46.l" "pSphere1FollicleShape2209.sp";
connectAttr "curve46.wm" "pSphere1FollicleShape2209.spm";
connectAttr "pSphere1FollicleShape2215.ot" "pSphere1Follicle2215.t" -l on;
connectAttr "pSphere1FollicleShape2215.or" "pSphere1Follicle2215.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape2215.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape2215.inm";
connectAttr "curveShape47.l" "pSphere1FollicleShape2215.sp";
connectAttr "curve47.wm" "pSphere1FollicleShape2215.spm";
connectAttr "pSphere1FollicleShape2222.ot" "pSphere1Follicle2222.t" -l on;
connectAttr "pSphere1FollicleShape2222.or" "pSphere1Follicle2222.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape2222.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape2222.inm";
connectAttr "curveShape48.l" "pSphere1FollicleShape2222.sp";
connectAttr "curve48.wm" "pSphere1FollicleShape2222.spm";
connectAttr "pSphere1FollicleShape2228.ot" "pSphere1Follicle2228.t" -l on;
connectAttr "pSphere1FollicleShape2228.or" "pSphere1Follicle2228.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape2228.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape2228.inm";
connectAttr "curveShape49.l" "pSphere1FollicleShape2228.sp";
connectAttr "curve49.wm" "pSphere1FollicleShape2228.spm";
connectAttr "pSphere1FollicleShape2234.ot" "pSphere1Follicle2234.t" -l on;
connectAttr "pSphere1FollicleShape2234.or" "pSphere1Follicle2234.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape2234.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape2234.inm";
connectAttr "curveShape50.l" "pSphere1FollicleShape2234.sp";
connectAttr "curve50.wm" "pSphere1FollicleShape2234.spm";
connectAttr "pSphere1FollicleShape2240.ot" "pSphere1Follicle2240.t" -l on;
connectAttr "pSphere1FollicleShape2240.or" "pSphere1Follicle2240.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape2240.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape2240.inm";
connectAttr "curveShape51.l" "pSphere1FollicleShape2240.sp";
connectAttr "curve51.wm" "pSphere1FollicleShape2240.spm";
connectAttr "pSphere1FollicleShape2246.ot" "pSphere1Follicle2246.t" -l on;
connectAttr "pSphere1FollicleShape2246.or" "pSphere1Follicle2246.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape2246.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape2246.inm";
connectAttr "curveShape52.l" "pSphere1FollicleShape2246.sp";
connectAttr "curve52.wm" "pSphere1FollicleShape2246.spm";
connectAttr "pSphere1FollicleShape2253.ot" "pSphere1Follicle2253.t" -l on;
connectAttr "pSphere1FollicleShape2253.or" "pSphere1Follicle2253.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape2253.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape2253.inm";
connectAttr "curveShape53.l" "pSphere1FollicleShape2253.sp";
connectAttr "curve53.wm" "pSphere1FollicleShape2253.spm";
connectAttr "pSphere1FollicleShape2259.ot" "pSphere1Follicle2259.t" -l on;
connectAttr "pSphere1FollicleShape2259.or" "pSphere1Follicle2259.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape2259.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape2259.inm";
connectAttr "curveShape54.l" "pSphere1FollicleShape2259.sp";
connectAttr "curve54.wm" "pSphere1FollicleShape2259.spm";
connectAttr "pSphere1FollicleShape2265.ot" "pSphere1Follicle2265.t" -l on;
connectAttr "pSphere1FollicleShape2265.or" "pSphere1Follicle2265.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape2265.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape2265.inm";
connectAttr "curveShape55.l" "pSphere1FollicleShape2265.sp";
connectAttr "curve55.wm" "pSphere1FollicleShape2265.spm";
connectAttr "pSphere1FollicleShape2271.ot" "pSphere1Follicle2271.t" -l on;
connectAttr "pSphere1FollicleShape2271.or" "pSphere1Follicle2271.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape2271.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape2271.inm";
connectAttr "curveShape56.l" "pSphere1FollicleShape2271.sp";
connectAttr "curve56.wm" "pSphere1FollicleShape2271.spm";
connectAttr "pSphere1FollicleShape2277.ot" "pSphere1Follicle2277.t" -l on;
connectAttr "pSphere1FollicleShape2277.or" "pSphere1Follicle2277.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape2277.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape2277.inm";
connectAttr "curveShape57.l" "pSphere1FollicleShape2277.sp";
connectAttr "curve57.wm" "pSphere1FollicleShape2277.spm";
connectAttr "pSphere1FollicleShape2284.ot" "pSphere1Follicle2284.t" -l on;
connectAttr "pSphere1FollicleShape2284.or" "pSphere1Follicle2284.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape2284.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape2284.inm";
connectAttr "curveShape58.l" "pSphere1FollicleShape2284.sp";
connectAttr "curve58.wm" "pSphere1FollicleShape2284.spm";
connectAttr "pSphere1FollicleShape2290.ot" "pSphere1Follicle2290.t" -l on;
connectAttr "pSphere1FollicleShape2290.or" "pSphere1Follicle2290.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape2290.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape2290.inm";
connectAttr "curveShape59.l" "pSphere1FollicleShape2290.sp";
connectAttr "curve59.wm" "pSphere1FollicleShape2290.spm";
connectAttr "pSphere1FollicleShape2296.ot" "pSphere1Follicle2296.t" -l on;
connectAttr "pSphere1FollicleShape2296.or" "pSphere1Follicle2296.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape2296.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape2296.inm";
connectAttr "curveShape60.l" "pSphere1FollicleShape2296.sp";
connectAttr "curve60.wm" "pSphere1FollicleShape2296.spm";
connectAttr "pSphere1FollicleShape2803.ot" "pSphere1Follicle2803.t" -l on;
connectAttr "pSphere1FollicleShape2803.or" "pSphere1Follicle2803.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape2803.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape2803.inm";
connectAttr "curveShape61.l" "pSphere1FollicleShape2803.sp";
connectAttr "curve61.wm" "pSphere1FollicleShape2803.spm";
connectAttr "pSphere1FollicleShape2809.ot" "pSphere1Follicle2809.t" -l on;
connectAttr "pSphere1FollicleShape2809.or" "pSphere1Follicle2809.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape2809.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape2809.inm";
connectAttr "curveShape62.l" "pSphere1FollicleShape2809.sp";
connectAttr "curve62.wm" "pSphere1FollicleShape2809.spm";
connectAttr "pSphere1FollicleShape2815.ot" "pSphere1Follicle2815.t" -l on;
connectAttr "pSphere1FollicleShape2815.or" "pSphere1Follicle2815.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape2815.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape2815.inm";
connectAttr "curveShape63.l" "pSphere1FollicleShape2815.sp";
connectAttr "curve63.wm" "pSphere1FollicleShape2815.spm";
connectAttr "pSphere1FollicleShape2822.ot" "pSphere1Follicle2822.t" -l on;
connectAttr "pSphere1FollicleShape2822.or" "pSphere1Follicle2822.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape2822.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape2822.inm";
connectAttr "curveShape64.l" "pSphere1FollicleShape2822.sp";
connectAttr "curve64.wm" "pSphere1FollicleShape2822.spm";
connectAttr "pSphere1FollicleShape2828.ot" "pSphere1Follicle2828.t" -l on;
connectAttr "pSphere1FollicleShape2828.or" "pSphere1Follicle2828.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape2828.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape2828.inm";
connectAttr "curveShape65.l" "pSphere1FollicleShape2828.sp";
connectAttr "curve65.wm" "pSphere1FollicleShape2828.spm";
connectAttr "pSphere1FollicleShape2834.ot" "pSphere1Follicle2834.t" -l on;
connectAttr "pSphere1FollicleShape2834.or" "pSphere1Follicle2834.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape2834.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape2834.inm";
connectAttr "curveShape66.l" "pSphere1FollicleShape2834.sp";
connectAttr "curve66.wm" "pSphere1FollicleShape2834.spm";
connectAttr "pSphere1FollicleShape2840.ot" "pSphere1Follicle2840.t" -l on;
connectAttr "pSphere1FollicleShape2840.or" "pSphere1Follicle2840.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape2840.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape2840.inm";
connectAttr "curveShape67.l" "pSphere1FollicleShape2840.sp";
connectAttr "curve67.wm" "pSphere1FollicleShape2840.spm";
connectAttr "pSphere1FollicleShape2846.ot" "pSphere1Follicle2846.t" -l on;
connectAttr "pSphere1FollicleShape2846.or" "pSphere1Follicle2846.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape2846.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape2846.inm";
connectAttr "curveShape68.l" "pSphere1FollicleShape2846.sp";
connectAttr "curve68.wm" "pSphere1FollicleShape2846.spm";
connectAttr "pSphere1FollicleShape2853.ot" "pSphere1Follicle2853.t" -l on;
connectAttr "pSphere1FollicleShape2853.or" "pSphere1Follicle2853.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape2853.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape2853.inm";
connectAttr "curveShape69.l" "pSphere1FollicleShape2853.sp";
connectAttr "curve69.wm" "pSphere1FollicleShape2853.spm";
connectAttr "pSphere1FollicleShape2859.ot" "pSphere1Follicle2859.t" -l on;
connectAttr "pSphere1FollicleShape2859.or" "pSphere1Follicle2859.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape2859.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape2859.inm";
connectAttr "curveShape70.l" "pSphere1FollicleShape2859.sp";
connectAttr "curve70.wm" "pSphere1FollicleShape2859.spm";
connectAttr "pSphere1FollicleShape2865.ot" "pSphere1Follicle2865.t" -l on;
connectAttr "pSphere1FollicleShape2865.or" "pSphere1Follicle2865.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape2865.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape2865.inm";
connectAttr "curveShape71.l" "pSphere1FollicleShape2865.sp";
connectAttr "curve71.wm" "pSphere1FollicleShape2865.spm";
connectAttr "pSphere1FollicleShape2871.ot" "pSphere1Follicle2871.t" -l on;
connectAttr "pSphere1FollicleShape2871.or" "pSphere1Follicle2871.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape2871.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape2871.inm";
connectAttr "curveShape72.l" "pSphere1FollicleShape2871.sp";
connectAttr "curve72.wm" "pSphere1FollicleShape2871.spm";
connectAttr "pSphere1FollicleShape2877.ot" "pSphere1Follicle2877.t" -l on;
connectAttr "pSphere1FollicleShape2877.or" "pSphere1Follicle2877.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape2877.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape2877.inm";
connectAttr "curveShape73.l" "pSphere1FollicleShape2877.sp";
connectAttr "curve73.wm" "pSphere1FollicleShape2877.spm";
connectAttr "pSphere1FollicleShape2884.ot" "pSphere1Follicle2884.t" -l on;
connectAttr "pSphere1FollicleShape2884.or" "pSphere1Follicle2884.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape2884.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape2884.inm";
connectAttr "curveShape74.l" "pSphere1FollicleShape2884.sp";
connectAttr "curve74.wm" "pSphere1FollicleShape2884.spm";
connectAttr "pSphere1FollicleShape2890.ot" "pSphere1Follicle2890.t" -l on;
connectAttr "pSphere1FollicleShape2890.or" "pSphere1Follicle2890.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape2890.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape2890.inm";
connectAttr "curveShape75.l" "pSphere1FollicleShape2890.sp";
connectAttr "curve75.wm" "pSphere1FollicleShape2890.spm";
connectAttr "pSphere1FollicleShape2896.ot" "pSphere1Follicle2896.t" -l on;
connectAttr "pSphere1FollicleShape2896.or" "pSphere1Follicle2896.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape2896.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape2896.inm";
connectAttr "curveShape76.l" "pSphere1FollicleShape2896.sp";
connectAttr "curve76.wm" "pSphere1FollicleShape2896.spm";
connectAttr "pSphere1FollicleShape3409.ot" "pSphere1Follicle3409.t" -l on;
connectAttr "pSphere1FollicleShape3409.or" "pSphere1Follicle3409.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape3409.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape3409.inm";
connectAttr "curveShape77.l" "pSphere1FollicleShape3409.sp";
connectAttr "curve77.wm" "pSphere1FollicleShape3409.spm";
connectAttr "pSphere1FollicleShape3415.ot" "pSphere1Follicle3415.t" -l on;
connectAttr "pSphere1FollicleShape3415.or" "pSphere1Follicle3415.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape3415.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape3415.inm";
connectAttr "curveShape78.l" "pSphere1FollicleShape3415.sp";
connectAttr "curve78.wm" "pSphere1FollicleShape3415.spm";
connectAttr "pSphere1FollicleShape3422.ot" "pSphere1Follicle3422.t" -l on;
connectAttr "pSphere1FollicleShape3422.or" "pSphere1Follicle3422.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape3422.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape3422.inm";
connectAttr "curveShape79.l" "pSphere1FollicleShape3422.sp";
connectAttr "curve79.wm" "pSphere1FollicleShape3422.spm";
connectAttr "pSphere1FollicleShape3428.ot" "pSphere1Follicle3428.t" -l on;
connectAttr "pSphere1FollicleShape3428.or" "pSphere1Follicle3428.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape3428.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape3428.inm";
connectAttr "curveShape80.l" "pSphere1FollicleShape3428.sp";
connectAttr "curve80.wm" "pSphere1FollicleShape3428.spm";
connectAttr "pSphere1FollicleShape3434.ot" "pSphere1Follicle3434.t" -l on;
connectAttr "pSphere1FollicleShape3434.or" "pSphere1Follicle3434.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape3434.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape3434.inm";
connectAttr "curveShape81.l" "pSphere1FollicleShape3434.sp";
connectAttr "curve81.wm" "pSphere1FollicleShape3434.spm";
connectAttr "pSphere1FollicleShape3440.ot" "pSphere1Follicle3440.t" -l on;
connectAttr "pSphere1FollicleShape3440.or" "pSphere1Follicle3440.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape3440.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape3440.inm";
connectAttr "curveShape82.l" "pSphere1FollicleShape3440.sp";
connectAttr "curve82.wm" "pSphere1FollicleShape3440.spm";
connectAttr "pSphere1FollicleShape3446.ot" "pSphere1Follicle3446.t" -l on;
connectAttr "pSphere1FollicleShape3446.or" "pSphere1Follicle3446.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape3446.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape3446.inm";
connectAttr "curveShape83.l" "pSphere1FollicleShape3446.sp";
connectAttr "curve83.wm" "pSphere1FollicleShape3446.spm";
connectAttr "pSphere1FollicleShape3453.ot" "pSphere1Follicle3453.t" -l on;
connectAttr "pSphere1FollicleShape3453.or" "pSphere1Follicle3453.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape3453.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape3453.inm";
connectAttr "curveShape84.l" "pSphere1FollicleShape3453.sp";
connectAttr "curve84.wm" "pSphere1FollicleShape3453.spm";
connectAttr "pSphere1FollicleShape3459.ot" "pSphere1Follicle3459.t" -l on;
connectAttr "pSphere1FollicleShape3459.or" "pSphere1Follicle3459.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape3459.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape3459.inm";
connectAttr "curveShape85.l" "pSphere1FollicleShape3459.sp";
connectAttr "curve85.wm" "pSphere1FollicleShape3459.spm";
connectAttr "pSphere1FollicleShape3465.ot" "pSphere1Follicle3465.t" -l on;
connectAttr "pSphere1FollicleShape3465.or" "pSphere1Follicle3465.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape3465.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape3465.inm";
connectAttr "curveShape86.l" "pSphere1FollicleShape3465.sp";
connectAttr "curve86.wm" "pSphere1FollicleShape3465.spm";
connectAttr "pSphere1FollicleShape3471.ot" "pSphere1Follicle3471.t" -l on;
connectAttr "pSphere1FollicleShape3471.or" "pSphere1Follicle3471.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape3471.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape3471.inm";
connectAttr "curveShape87.l" "pSphere1FollicleShape3471.sp";
connectAttr "curve87.wm" "pSphere1FollicleShape3471.spm";
connectAttr "pSphere1FollicleShape3477.ot" "pSphere1Follicle3477.t" -l on;
connectAttr "pSphere1FollicleShape3477.or" "pSphere1Follicle3477.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape3477.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape3477.inm";
connectAttr "curveShape88.l" "pSphere1FollicleShape3477.sp";
connectAttr "curve88.wm" "pSphere1FollicleShape3477.spm";
connectAttr "pSphere1FollicleShape3484.ot" "pSphere1Follicle3484.t" -l on;
connectAttr "pSphere1FollicleShape3484.or" "pSphere1Follicle3484.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape3484.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape3484.inm";
connectAttr "curveShape89.l" "pSphere1FollicleShape3484.sp";
connectAttr "curve89.wm" "pSphere1FollicleShape3484.spm";
connectAttr "pSphere1FollicleShape3490.ot" "pSphere1Follicle3490.t" -l on;
connectAttr "pSphere1FollicleShape3490.or" "pSphere1Follicle3490.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape3490.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape3490.inm";
connectAttr "curveShape90.l" "pSphere1FollicleShape3490.sp";
connectAttr "curve90.wm" "pSphere1FollicleShape3490.spm";
connectAttr "pSphere1FollicleShape4009.ot" "pSphere1Follicle4009.t" -l on;
connectAttr "pSphere1FollicleShape4009.or" "pSphere1Follicle4009.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape4009.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape4009.inm";
connectAttr "curveShape91.l" "pSphere1FollicleShape4009.sp";
connectAttr "curve91.wm" "pSphere1FollicleShape4009.spm";
connectAttr "pSphere1FollicleShape4015.ot" "pSphere1Follicle4015.t" -l on;
connectAttr "pSphere1FollicleShape4015.or" "pSphere1Follicle4015.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape4015.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape4015.inm";
connectAttr "curveShape92.l" "pSphere1FollicleShape4015.sp";
connectAttr "curve92.wm" "pSphere1FollicleShape4015.spm";
connectAttr "pSphere1FollicleShape4022.ot" "pSphere1Follicle4022.t" -l on;
connectAttr "pSphere1FollicleShape4022.or" "pSphere1Follicle4022.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape4022.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape4022.inm";
connectAttr "curveShape93.l" "pSphere1FollicleShape4022.sp";
connectAttr "curve93.wm" "pSphere1FollicleShape4022.spm";
connectAttr "pSphere1FollicleShape4028.ot" "pSphere1Follicle4028.t" -l on;
connectAttr "pSphere1FollicleShape4028.or" "pSphere1Follicle4028.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape4028.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape4028.inm";
connectAttr "curveShape94.l" "pSphere1FollicleShape4028.sp";
connectAttr "curve94.wm" "pSphere1FollicleShape4028.spm";
connectAttr "pSphere1FollicleShape4034.ot" "pSphere1Follicle4034.t" -l on;
connectAttr "pSphere1FollicleShape4034.or" "pSphere1Follicle4034.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape4034.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape4034.inm";
connectAttr "curveShape95.l" "pSphere1FollicleShape4034.sp";
connectAttr "curve95.wm" "pSphere1FollicleShape4034.spm";
connectAttr "pSphere1FollicleShape4040.ot" "pSphere1Follicle4040.t" -l on;
connectAttr "pSphere1FollicleShape4040.or" "pSphere1Follicle4040.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape4040.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape4040.inm";
connectAttr "curveShape96.l" "pSphere1FollicleShape4040.sp";
connectAttr "curve96.wm" "pSphere1FollicleShape4040.spm";
connectAttr "pSphere1FollicleShape4046.ot" "pSphere1Follicle4046.t" -l on;
connectAttr "pSphere1FollicleShape4046.or" "pSphere1Follicle4046.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape4046.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape4046.inm";
connectAttr "curveShape97.l" "pSphere1FollicleShape4046.sp";
connectAttr "curve97.wm" "pSphere1FollicleShape4046.spm";
connectAttr "pSphere1FollicleShape4053.ot" "pSphere1Follicle4053.t" -l on;
connectAttr "pSphere1FollicleShape4053.or" "pSphere1Follicle4053.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape4053.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape4053.inm";
connectAttr "curveShape98.l" "pSphere1FollicleShape4053.sp";
connectAttr "curve98.wm" "pSphere1FollicleShape4053.spm";
connectAttr "pSphere1FollicleShape4059.ot" "pSphere1Follicle4059.t" -l on;
connectAttr "pSphere1FollicleShape4059.or" "pSphere1Follicle4059.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape4059.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape4059.inm";
connectAttr "curveShape99.l" "pSphere1FollicleShape4059.sp";
connectAttr "curve99.wm" "pSphere1FollicleShape4059.spm";
connectAttr "pSphere1FollicleShape4065.ot" "pSphere1Follicle4065.t" -l on;
connectAttr "pSphere1FollicleShape4065.or" "pSphere1Follicle4065.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape4065.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape4065.inm";
connectAttr "curveShape100.l" "pSphere1FollicleShape4065.sp";
connectAttr "curve100.wm" "pSphere1FollicleShape4065.spm";
connectAttr "pSphere1FollicleShape4071.ot" "pSphere1Follicle4071.t" -l on;
connectAttr "pSphere1FollicleShape4071.or" "pSphere1Follicle4071.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape4071.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape4071.inm";
connectAttr "curveShape101.l" "pSphere1FollicleShape4071.sp";
connectAttr "curve101.wm" "pSphere1FollicleShape4071.spm";
connectAttr "pSphere1FollicleShape4077.ot" "pSphere1Follicle4077.t" -l on;
connectAttr "pSphere1FollicleShape4077.or" "pSphere1Follicle4077.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape4077.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape4077.inm";
connectAttr "curveShape102.l" "pSphere1FollicleShape4077.sp";
connectAttr "curve102.wm" "pSphere1FollicleShape4077.spm";
connectAttr "pSphere1FollicleShape4084.ot" "pSphere1Follicle4084.t" -l on;
connectAttr "pSphere1FollicleShape4084.or" "pSphere1Follicle4084.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape4084.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape4084.inm";
connectAttr "curveShape103.l" "pSphere1FollicleShape4084.sp";
connectAttr "curve103.wm" "pSphere1FollicleShape4084.spm";
connectAttr "pSphere1FollicleShape4090.ot" "pSphere1Follicle4090.t" -l on;
connectAttr "pSphere1FollicleShape4090.or" "pSphere1Follicle4090.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape4090.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape4090.inm";
connectAttr "curveShape104.l" "pSphere1FollicleShape4090.sp";
connectAttr "curve104.wm" "pSphere1FollicleShape4090.spm";
connectAttr "pSphere1FollicleShape4603.ot" "pSphere1Follicle4603.t" -l on;
connectAttr "pSphere1FollicleShape4603.or" "pSphere1Follicle4603.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape4603.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape4603.inm";
connectAttr "curveShape105.l" "pSphere1FollicleShape4603.sp";
connectAttr "curve105.wm" "pSphere1FollicleShape4603.spm";
connectAttr "pSphere1FollicleShape4609.ot" "pSphere1Follicle4609.t" -l on;
connectAttr "pSphere1FollicleShape4609.or" "pSphere1Follicle4609.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape4609.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape4609.inm";
connectAttr "curveShape106.l" "pSphere1FollicleShape4609.sp";
connectAttr "curve106.wm" "pSphere1FollicleShape4609.spm";
connectAttr "pSphere1FollicleShape4615.ot" "pSphere1Follicle4615.t" -l on;
connectAttr "pSphere1FollicleShape4615.or" "pSphere1Follicle4615.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape4615.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape4615.inm";
connectAttr "curveShape107.l" "pSphere1FollicleShape4615.sp";
connectAttr "curve107.wm" "pSphere1FollicleShape4615.spm";
connectAttr "pSphere1FollicleShape4622.ot" "pSphere1Follicle4622.t" -l on;
connectAttr "pSphere1FollicleShape4622.or" "pSphere1Follicle4622.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape4622.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape4622.inm";
connectAttr "curveShape108.l" "pSphere1FollicleShape4622.sp";
connectAttr "curve108.wm" "pSphere1FollicleShape4622.spm";
connectAttr "pSphere1FollicleShape4628.ot" "pSphere1Follicle4628.t" -l on;
connectAttr "pSphere1FollicleShape4628.or" "pSphere1Follicle4628.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape4628.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape4628.inm";
connectAttr "curveShape109.l" "pSphere1FollicleShape4628.sp";
connectAttr "curve109.wm" "pSphere1FollicleShape4628.spm";
connectAttr "pSphere1FollicleShape4634.ot" "pSphere1Follicle4634.t" -l on;
connectAttr "pSphere1FollicleShape4634.or" "pSphere1Follicle4634.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape4634.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape4634.inm";
connectAttr "curveShape110.l" "pSphere1FollicleShape4634.sp";
connectAttr "curve110.wm" "pSphere1FollicleShape4634.spm";
connectAttr "pSphere1FollicleShape4640.ot" "pSphere1Follicle4640.t" -l on;
connectAttr "pSphere1FollicleShape4640.or" "pSphere1Follicle4640.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape4640.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape4640.inm";
connectAttr "curveShape111.l" "pSphere1FollicleShape4640.sp";
connectAttr "curve111.wm" "pSphere1FollicleShape4640.spm";
connectAttr "pSphere1FollicleShape4646.ot" "pSphere1Follicle4646.t" -l on;
connectAttr "pSphere1FollicleShape4646.or" "pSphere1Follicle4646.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape4646.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape4646.inm";
connectAttr "curveShape112.l" "pSphere1FollicleShape4646.sp";
connectAttr "curve112.wm" "pSphere1FollicleShape4646.spm";
connectAttr "pSphere1FollicleShape4653.ot" "pSphere1Follicle4653.t" -l on;
connectAttr "pSphere1FollicleShape4653.or" "pSphere1Follicle4653.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape4653.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape4653.inm";
connectAttr "curveShape113.l" "pSphere1FollicleShape4653.sp";
connectAttr "curve113.wm" "pSphere1FollicleShape4653.spm";
connectAttr "pSphere1FollicleShape4659.ot" "pSphere1Follicle4659.t" -l on;
connectAttr "pSphere1FollicleShape4659.or" "pSphere1Follicle4659.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape4659.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape4659.inm";
connectAttr "curveShape114.l" "pSphere1FollicleShape4659.sp";
connectAttr "curve114.wm" "pSphere1FollicleShape4659.spm";
connectAttr "pSphere1FollicleShape4665.ot" "pSphere1Follicle4665.t" -l on;
connectAttr "pSphere1FollicleShape4665.or" "pSphere1Follicle4665.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape4665.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape4665.inm";
connectAttr "curveShape115.l" "pSphere1FollicleShape4665.sp";
connectAttr "curve115.wm" "pSphere1FollicleShape4665.spm";
connectAttr "pSphere1FollicleShape4671.ot" "pSphere1Follicle4671.t" -l on;
connectAttr "pSphere1FollicleShape4671.or" "pSphere1Follicle4671.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape4671.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape4671.inm";
connectAttr "curveShape116.l" "pSphere1FollicleShape4671.sp";
connectAttr "curve116.wm" "pSphere1FollicleShape4671.spm";
connectAttr "pSphere1FollicleShape4677.ot" "pSphere1Follicle4677.t" -l on;
connectAttr "pSphere1FollicleShape4677.or" "pSphere1Follicle4677.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape4677.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape4677.inm";
connectAttr "curveShape117.l" "pSphere1FollicleShape4677.sp";
connectAttr "curve117.wm" "pSphere1FollicleShape4677.spm";
connectAttr "pSphere1FollicleShape4684.ot" "pSphere1Follicle4684.t" -l on;
connectAttr "pSphere1FollicleShape4684.or" "pSphere1Follicle4684.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape4684.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape4684.inm";
connectAttr "curveShape118.l" "pSphere1FollicleShape4684.sp";
connectAttr "curve118.wm" "pSphere1FollicleShape4684.spm";
connectAttr "pSphere1FollicleShape4690.ot" "pSphere1Follicle4690.t" -l on;
connectAttr "pSphere1FollicleShape4690.or" "pSphere1Follicle4690.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape4690.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape4690.inm";
connectAttr "curveShape119.l" "pSphere1FollicleShape4690.sp";
connectAttr "curve119.wm" "pSphere1FollicleShape4690.spm";
connectAttr "pSphere1FollicleShape4696.ot" "pSphere1Follicle4696.t" -l on;
connectAttr "pSphere1FollicleShape4696.or" "pSphere1Follicle4696.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape4696.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape4696.inm";
connectAttr "curveShape120.l" "pSphere1FollicleShape4696.sp";
connectAttr "curve120.wm" "pSphere1FollicleShape4696.spm";
connectAttr "pSphere1FollicleShape5303.ot" "pSphere1Follicle5303.t" -l on;
connectAttr "pSphere1FollicleShape5303.or" "pSphere1Follicle5303.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape5303.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape5303.inm";
connectAttr "curveShape121.l" "pSphere1FollicleShape5303.sp";
connectAttr "curve121.wm" "pSphere1FollicleShape5303.spm";
connectAttr "pSphere1FollicleShape5309.ot" "pSphere1Follicle5309.t" -l on;
connectAttr "pSphere1FollicleShape5309.or" "pSphere1Follicle5309.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape5309.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape5309.inm";
connectAttr "curveShape122.l" "pSphere1FollicleShape5309.sp";
connectAttr "curve122.wm" "pSphere1FollicleShape5309.spm";
connectAttr "pSphere1FollicleShape5315.ot" "pSphere1Follicle5315.t" -l on;
connectAttr "pSphere1FollicleShape5315.or" "pSphere1Follicle5315.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape5315.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape5315.inm";
connectAttr "curveShape123.l" "pSphere1FollicleShape5315.sp";
connectAttr "curve123.wm" "pSphere1FollicleShape5315.spm";
connectAttr "pSphere1FollicleShape5322.ot" "pSphere1Follicle5322.t" -l on;
connectAttr "pSphere1FollicleShape5322.or" "pSphere1Follicle5322.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape5322.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape5322.inm";
connectAttr "curveShape124.l" "pSphere1FollicleShape5322.sp";
connectAttr "curve124.wm" "pSphere1FollicleShape5322.spm";
connectAttr "pSphere1FollicleShape5328.ot" "pSphere1Follicle5328.t" -l on;
connectAttr "pSphere1FollicleShape5328.or" "pSphere1Follicle5328.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape5328.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape5328.inm";
connectAttr "curveShape125.l" "pSphere1FollicleShape5328.sp";
connectAttr "curve125.wm" "pSphere1FollicleShape5328.spm";
connectAttr "pSphere1FollicleShape5334.ot" "pSphere1Follicle5334.t" -l on;
connectAttr "pSphere1FollicleShape5334.or" "pSphere1Follicle5334.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape5334.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape5334.inm";
connectAttr "curveShape126.l" "pSphere1FollicleShape5334.sp";
connectAttr "curve126.wm" "pSphere1FollicleShape5334.spm";
connectAttr "pSphere1FollicleShape5340.ot" "pSphere1Follicle5340.t" -l on;
connectAttr "pSphere1FollicleShape5340.or" "pSphere1Follicle5340.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape5340.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape5340.inm";
connectAttr "curveShape127.l" "pSphere1FollicleShape5340.sp";
connectAttr "curve127.wm" "pSphere1FollicleShape5340.spm";
connectAttr "pSphere1FollicleShape5346.ot" "pSphere1Follicle5346.t" -l on;
connectAttr "pSphere1FollicleShape5346.or" "pSphere1Follicle5346.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape5346.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape5346.inm";
connectAttr "curveShape128.l" "pSphere1FollicleShape5346.sp";
connectAttr "curve128.wm" "pSphere1FollicleShape5346.spm";
connectAttr "pSphere1FollicleShape5353.ot" "pSphere1Follicle5353.t" -l on;
connectAttr "pSphere1FollicleShape5353.or" "pSphere1Follicle5353.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape5353.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape5353.inm";
connectAttr "curveShape129.l" "pSphere1FollicleShape5353.sp";
connectAttr "curve129.wm" "pSphere1FollicleShape5353.spm";
connectAttr "pSphere1FollicleShape5359.ot" "pSphere1Follicle5359.t" -l on;
connectAttr "pSphere1FollicleShape5359.or" "pSphere1Follicle5359.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape5359.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape5359.inm";
connectAttr "curveShape130.l" "pSphere1FollicleShape5359.sp";
connectAttr "curve130.wm" "pSphere1FollicleShape5359.spm";
connectAttr "pSphere1FollicleShape5365.ot" "pSphere1Follicle5365.t" -l on;
connectAttr "pSphere1FollicleShape5365.or" "pSphere1Follicle5365.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape5365.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape5365.inm";
connectAttr "curveShape131.l" "pSphere1FollicleShape5365.sp";
connectAttr "curve131.wm" "pSphere1FollicleShape5365.spm";
connectAttr "pSphere1FollicleShape5371.ot" "pSphere1Follicle5371.t" -l on;
connectAttr "pSphere1FollicleShape5371.or" "pSphere1Follicle5371.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape5371.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape5371.inm";
connectAttr "curveShape132.l" "pSphere1FollicleShape5371.sp";
connectAttr "curve132.wm" "pSphere1FollicleShape5371.spm";
connectAttr "pSphere1FollicleShape5377.ot" "pSphere1Follicle5377.t" -l on;
connectAttr "pSphere1FollicleShape5377.or" "pSphere1Follicle5377.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape5377.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape5377.inm";
connectAttr "curveShape133.l" "pSphere1FollicleShape5377.sp";
connectAttr "curve133.wm" "pSphere1FollicleShape5377.spm";
connectAttr "pSphere1FollicleShape5384.ot" "pSphere1Follicle5384.t" -l on;
connectAttr "pSphere1FollicleShape5384.or" "pSphere1Follicle5384.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape5384.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape5384.inm";
connectAttr "curveShape134.l" "pSphere1FollicleShape5384.sp";
connectAttr "curve134.wm" "pSphere1FollicleShape5384.spm";
connectAttr "pSphere1FollicleShape5390.ot" "pSphere1Follicle5390.t" -l on;
connectAttr "pSphere1FollicleShape5390.or" "pSphere1Follicle5390.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape5390.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape5390.inm";
connectAttr "curveShape135.l" "pSphere1FollicleShape5390.sp";
connectAttr "curve135.wm" "pSphere1FollicleShape5390.spm";
connectAttr "pSphere1FollicleShape5396.ot" "pSphere1Follicle5396.t" -l on;
connectAttr "pSphere1FollicleShape5396.or" "pSphere1Follicle5396.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape5396.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape5396.inm";
connectAttr "curveShape136.l" "pSphere1FollicleShape5396.sp";
connectAttr "curve136.wm" "pSphere1FollicleShape5396.spm";
connectAttr "pSphere1FollicleShape5909.ot" "pSphere1Follicle5909.t" -l on;
connectAttr "pSphere1FollicleShape5909.or" "pSphere1Follicle5909.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape5909.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape5909.inm";
connectAttr "curveShape137.l" "pSphere1FollicleShape5909.sp";
connectAttr "curve137.wm" "pSphere1FollicleShape5909.spm";
connectAttr "pSphere1FollicleShape5915.ot" "pSphere1Follicle5915.t" -l on;
connectAttr "pSphere1FollicleShape5915.or" "pSphere1Follicle5915.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape5915.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape5915.inm";
connectAttr "curveShape138.l" "pSphere1FollicleShape5915.sp";
connectAttr "curve138.wm" "pSphere1FollicleShape5915.spm";
connectAttr "pSphere1FollicleShape5922.ot" "pSphere1Follicle5922.t" -l on;
connectAttr "pSphere1FollicleShape5922.or" "pSphere1Follicle5922.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape5922.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape5922.inm";
connectAttr "curveShape139.l" "pSphere1FollicleShape5922.sp";
connectAttr "curve139.wm" "pSphere1FollicleShape5922.spm";
connectAttr "pSphere1FollicleShape5928.ot" "pSphere1Follicle5928.t" -l on;
connectAttr "pSphere1FollicleShape5928.or" "pSphere1Follicle5928.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape5928.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape5928.inm";
connectAttr "curveShape140.l" "pSphere1FollicleShape5928.sp";
connectAttr "curve140.wm" "pSphere1FollicleShape5928.spm";
connectAttr "pSphere1FollicleShape5934.ot" "pSphere1Follicle5934.t" -l on;
connectAttr "pSphere1FollicleShape5934.or" "pSphere1Follicle5934.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape5934.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape5934.inm";
connectAttr "curveShape141.l" "pSphere1FollicleShape5934.sp";
connectAttr "curve141.wm" "pSphere1FollicleShape5934.spm";
connectAttr "pSphere1FollicleShape5940.ot" "pSphere1Follicle5940.t" -l on;
connectAttr "pSphere1FollicleShape5940.or" "pSphere1Follicle5940.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape5940.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape5940.inm";
connectAttr "curveShape142.l" "pSphere1FollicleShape5940.sp";
connectAttr "curve142.wm" "pSphere1FollicleShape5940.spm";
connectAttr "pSphere1FollicleShape5946.ot" "pSphere1Follicle5946.t" -l on;
connectAttr "pSphere1FollicleShape5946.or" "pSphere1Follicle5946.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape5946.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape5946.inm";
connectAttr "curveShape143.l" "pSphere1FollicleShape5946.sp";
connectAttr "curve143.wm" "pSphere1FollicleShape5946.spm";
connectAttr "pSphere1FollicleShape5953.ot" "pSphere1Follicle5953.t" -l on;
connectAttr "pSphere1FollicleShape5953.or" "pSphere1Follicle5953.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape5953.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape5953.inm";
connectAttr "curveShape144.l" "pSphere1FollicleShape5953.sp";
connectAttr "curve144.wm" "pSphere1FollicleShape5953.spm";
connectAttr "pSphere1FollicleShape5959.ot" "pSphere1Follicle5959.t" -l on;
connectAttr "pSphere1FollicleShape5959.or" "pSphere1Follicle5959.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape5959.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape5959.inm";
connectAttr "curveShape145.l" "pSphere1FollicleShape5959.sp";
connectAttr "curve145.wm" "pSphere1FollicleShape5959.spm";
connectAttr "pSphere1FollicleShape5965.ot" "pSphere1Follicle5965.t" -l on;
connectAttr "pSphere1FollicleShape5965.or" "pSphere1Follicle5965.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape5965.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape5965.inm";
connectAttr "curveShape146.l" "pSphere1FollicleShape5965.sp";
connectAttr "curve146.wm" "pSphere1FollicleShape5965.spm";
connectAttr "pSphere1FollicleShape5971.ot" "pSphere1Follicle5971.t" -l on;
connectAttr "pSphere1FollicleShape5971.or" "pSphere1Follicle5971.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape5971.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape5971.inm";
connectAttr "curveShape147.l" "pSphere1FollicleShape5971.sp";
connectAttr "curve147.wm" "pSphere1FollicleShape5971.spm";
connectAttr "pSphere1FollicleShape5977.ot" "pSphere1Follicle5977.t" -l on;
connectAttr "pSphere1FollicleShape5977.or" "pSphere1Follicle5977.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape5977.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape5977.inm";
connectAttr "curveShape148.l" "pSphere1FollicleShape5977.sp";
connectAttr "curve148.wm" "pSphere1FollicleShape5977.spm";
connectAttr "pSphere1FollicleShape5984.ot" "pSphere1Follicle5984.t" -l on;
connectAttr "pSphere1FollicleShape5984.or" "pSphere1Follicle5984.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape5984.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape5984.inm";
connectAttr "curveShape149.l" "pSphere1FollicleShape5984.sp";
connectAttr "curve149.wm" "pSphere1FollicleShape5984.spm";
connectAttr "pSphere1FollicleShape5990.ot" "pSphere1Follicle5990.t" -l on;
connectAttr "pSphere1FollicleShape5990.or" "pSphere1Follicle5990.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape5990.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape5990.inm";
connectAttr "curveShape150.l" "pSphere1FollicleShape5990.sp";
connectAttr "curve150.wm" "pSphere1FollicleShape5990.spm";
connectAttr "pSphere1FollicleShape6509.ot" "pSphere1Follicle6509.t" -l on;
connectAttr "pSphere1FollicleShape6509.or" "pSphere1Follicle6509.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape6509.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape6509.inm";
connectAttr "curveShape151.l" "pSphere1FollicleShape6509.sp";
connectAttr "curve151.wm" "pSphere1FollicleShape6509.spm";
connectAttr "pSphere1FollicleShape6515.ot" "pSphere1Follicle6515.t" -l on;
connectAttr "pSphere1FollicleShape6515.or" "pSphere1Follicle6515.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape6515.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape6515.inm";
connectAttr "curveShape152.l" "pSphere1FollicleShape6515.sp";
connectAttr "curve152.wm" "pSphere1FollicleShape6515.spm";
connectAttr "pSphere1FollicleShape6522.ot" "pSphere1Follicle6522.t" -l on;
connectAttr "pSphere1FollicleShape6522.or" "pSphere1Follicle6522.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape6522.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape6522.inm";
connectAttr "curveShape153.l" "pSphere1FollicleShape6522.sp";
connectAttr "curve153.wm" "pSphere1FollicleShape6522.spm";
connectAttr "pSphere1FollicleShape6528.ot" "pSphere1Follicle6528.t" -l on;
connectAttr "pSphere1FollicleShape6528.or" "pSphere1Follicle6528.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape6528.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape6528.inm";
connectAttr "curveShape154.l" "pSphere1FollicleShape6528.sp";
connectAttr "curve154.wm" "pSphere1FollicleShape6528.spm";
connectAttr "pSphere1FollicleShape6534.ot" "pSphere1Follicle6534.t" -l on;
connectAttr "pSphere1FollicleShape6534.or" "pSphere1Follicle6534.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape6534.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape6534.inm";
connectAttr "curveShape155.l" "pSphere1FollicleShape6534.sp";
connectAttr "curve155.wm" "pSphere1FollicleShape6534.spm";
connectAttr "pSphere1FollicleShape6540.ot" "pSphere1Follicle6540.t" -l on;
connectAttr "pSphere1FollicleShape6540.or" "pSphere1Follicle6540.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape6540.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape6540.inm";
connectAttr "curveShape156.l" "pSphere1FollicleShape6540.sp";
connectAttr "curve156.wm" "pSphere1FollicleShape6540.spm";
connectAttr "pSphere1FollicleShape6546.ot" "pSphere1Follicle6546.t" -l on;
connectAttr "pSphere1FollicleShape6546.or" "pSphere1Follicle6546.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape6546.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape6546.inm";
connectAttr "curveShape157.l" "pSphere1FollicleShape6546.sp";
connectAttr "curve157.wm" "pSphere1FollicleShape6546.spm";
connectAttr "pSphere1FollicleShape6553.ot" "pSphere1Follicle6553.t" -l on;
connectAttr "pSphere1FollicleShape6553.or" "pSphere1Follicle6553.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape6553.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape6553.inm";
connectAttr "curveShape158.l" "pSphere1FollicleShape6553.sp";
connectAttr "curve158.wm" "pSphere1FollicleShape6553.spm";
connectAttr "pSphere1FollicleShape6559.ot" "pSphere1Follicle6559.t" -l on;
connectAttr "pSphere1FollicleShape6559.or" "pSphere1Follicle6559.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape6559.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape6559.inm";
connectAttr "curveShape159.l" "pSphere1FollicleShape6559.sp";
connectAttr "curve159.wm" "pSphere1FollicleShape6559.spm";
connectAttr "pSphere1FollicleShape6565.ot" "pSphere1Follicle6565.t" -l on;
connectAttr "pSphere1FollicleShape6565.or" "pSphere1Follicle6565.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape6565.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape6565.inm";
connectAttr "curveShape160.l" "pSphere1FollicleShape6565.sp";
connectAttr "curve160.wm" "pSphere1FollicleShape6565.spm";
connectAttr "pSphere1FollicleShape6571.ot" "pSphere1Follicle6571.t" -l on;
connectAttr "pSphere1FollicleShape6571.or" "pSphere1Follicle6571.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape6571.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape6571.inm";
connectAttr "curveShape161.l" "pSphere1FollicleShape6571.sp";
connectAttr "curve161.wm" "pSphere1FollicleShape6571.spm";
connectAttr "pSphere1FollicleShape6577.ot" "pSphere1Follicle6577.t" -l on;
connectAttr "pSphere1FollicleShape6577.or" "pSphere1Follicle6577.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape6577.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape6577.inm";
connectAttr "curveShape162.l" "pSphere1FollicleShape6577.sp";
connectAttr "curve162.wm" "pSphere1FollicleShape6577.spm";
connectAttr "pSphere1FollicleShape6584.ot" "pSphere1Follicle6584.t" -l on;
connectAttr "pSphere1FollicleShape6584.or" "pSphere1Follicle6584.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape6584.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape6584.inm";
connectAttr "curveShape163.l" "pSphere1FollicleShape6584.sp";
connectAttr "curve163.wm" "pSphere1FollicleShape6584.spm";
connectAttr "pSphere1FollicleShape6590.ot" "pSphere1Follicle6590.t" -l on;
connectAttr "pSphere1FollicleShape6590.or" "pSphere1Follicle6590.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape6590.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape6590.inm";
connectAttr "curveShape164.l" "pSphere1FollicleShape6590.sp";
connectAttr "curve164.wm" "pSphere1FollicleShape6590.spm";
connectAttr "pSphere1FollicleShape7103.ot" "pSphere1Follicle7103.t" -l on;
connectAttr "pSphere1FollicleShape7103.or" "pSphere1Follicle7103.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape7103.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape7103.inm";
connectAttr "curveShape165.l" "pSphere1FollicleShape7103.sp";
connectAttr "curve165.wm" "pSphere1FollicleShape7103.spm";
connectAttr "pSphere1FollicleShape7109.ot" "pSphere1Follicle7109.t" -l on;
connectAttr "pSphere1FollicleShape7109.or" "pSphere1Follicle7109.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape7109.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape7109.inm";
connectAttr "curveShape166.l" "pSphere1FollicleShape7109.sp";
connectAttr "curve166.wm" "pSphere1FollicleShape7109.spm";
connectAttr "pSphere1FollicleShape7115.ot" "pSphere1Follicle7115.t" -l on;
connectAttr "pSphere1FollicleShape7115.or" "pSphere1Follicle7115.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape7115.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape7115.inm";
connectAttr "curveShape167.l" "pSphere1FollicleShape7115.sp";
connectAttr "curve167.wm" "pSphere1FollicleShape7115.spm";
connectAttr "pSphere1FollicleShape7122.ot" "pSphere1Follicle7122.t" -l on;
connectAttr "pSphere1FollicleShape7122.or" "pSphere1Follicle7122.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape7122.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape7122.inm";
connectAttr "curveShape168.l" "pSphere1FollicleShape7122.sp";
connectAttr "curve168.wm" "pSphere1FollicleShape7122.spm";
connectAttr "pSphere1FollicleShape7128.ot" "pSphere1Follicle7128.t" -l on;
connectAttr "pSphere1FollicleShape7128.or" "pSphere1Follicle7128.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape7128.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape7128.inm";
connectAttr "curveShape169.l" "pSphere1FollicleShape7128.sp";
connectAttr "curve169.wm" "pSphere1FollicleShape7128.spm";
connectAttr "pSphere1FollicleShape7134.ot" "pSphere1Follicle7134.t" -l on;
connectAttr "pSphere1FollicleShape7134.or" "pSphere1Follicle7134.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape7134.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape7134.inm";
connectAttr "curveShape170.l" "pSphere1FollicleShape7134.sp";
connectAttr "curve170.wm" "pSphere1FollicleShape7134.spm";
connectAttr "pSphere1FollicleShape7140.ot" "pSphere1Follicle7140.t" -l on;
connectAttr "pSphere1FollicleShape7140.or" "pSphere1Follicle7140.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape7140.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape7140.inm";
connectAttr "curveShape171.l" "pSphere1FollicleShape7140.sp";
connectAttr "curve171.wm" "pSphere1FollicleShape7140.spm";
connectAttr "pSphere1FollicleShape7146.ot" "pSphere1Follicle7146.t" -l on;
connectAttr "pSphere1FollicleShape7146.or" "pSphere1Follicle7146.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape7146.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape7146.inm";
connectAttr "curveShape172.l" "pSphere1FollicleShape7146.sp";
connectAttr "curve172.wm" "pSphere1FollicleShape7146.spm";
connectAttr "pSphere1FollicleShape7153.ot" "pSphere1Follicle7153.t" -l on;
connectAttr "pSphere1FollicleShape7153.or" "pSphere1Follicle7153.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape7153.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape7153.inm";
connectAttr "curveShape173.l" "pSphere1FollicleShape7153.sp";
connectAttr "curve173.wm" "pSphere1FollicleShape7153.spm";
connectAttr "pSphere1FollicleShape7159.ot" "pSphere1Follicle7159.t" -l on;
connectAttr "pSphere1FollicleShape7159.or" "pSphere1Follicle7159.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape7159.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape7159.inm";
connectAttr "curveShape174.l" "pSphere1FollicleShape7159.sp";
connectAttr "curve174.wm" "pSphere1FollicleShape7159.spm";
connectAttr "pSphere1FollicleShape7165.ot" "pSphere1Follicle7165.t" -l on;
connectAttr "pSphere1FollicleShape7165.or" "pSphere1Follicle7165.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape7165.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape7165.inm";
connectAttr "curveShape175.l" "pSphere1FollicleShape7165.sp";
connectAttr "curve175.wm" "pSphere1FollicleShape7165.spm";
connectAttr "pSphere1FollicleShape7171.ot" "pSphere1Follicle7171.t" -l on;
connectAttr "pSphere1FollicleShape7171.or" "pSphere1Follicle7171.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape7171.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape7171.inm";
connectAttr "curveShape176.l" "pSphere1FollicleShape7171.sp";
connectAttr "curve176.wm" "pSphere1FollicleShape7171.spm";
connectAttr "pSphere1FollicleShape7177.ot" "pSphere1Follicle7177.t" -l on;
connectAttr "pSphere1FollicleShape7177.or" "pSphere1Follicle7177.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape7177.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape7177.inm";
connectAttr "curveShape177.l" "pSphere1FollicleShape7177.sp";
connectAttr "curve177.wm" "pSphere1FollicleShape7177.spm";
connectAttr "pSphere1FollicleShape7184.ot" "pSphere1Follicle7184.t" -l on;
connectAttr "pSphere1FollicleShape7184.or" "pSphere1Follicle7184.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape7184.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape7184.inm";
connectAttr "curveShape178.l" "pSphere1FollicleShape7184.sp";
connectAttr "curve178.wm" "pSphere1FollicleShape7184.spm";
connectAttr "pSphere1FollicleShape7190.ot" "pSphere1Follicle7190.t" -l on;
connectAttr "pSphere1FollicleShape7190.or" "pSphere1Follicle7190.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape7190.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape7190.inm";
connectAttr "curveShape179.l" "pSphere1FollicleShape7190.sp";
connectAttr "curve179.wm" "pSphere1FollicleShape7190.spm";
connectAttr "pSphere1FollicleShape7196.ot" "pSphere1Follicle7196.t" -l on;
connectAttr "pSphere1FollicleShape7196.or" "pSphere1Follicle7196.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape7196.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape7196.inm";
connectAttr "curveShape180.l" "pSphere1FollicleShape7196.sp";
connectAttr "curve180.wm" "pSphere1FollicleShape7196.spm";
connectAttr "pSphere1FollicleShape7703.ot" "pSphere1Follicle7703.t" -l on;
connectAttr "pSphere1FollicleShape7703.or" "pSphere1Follicle7703.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape7703.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape7703.inm";
connectAttr "curveShape181.l" "pSphere1FollicleShape7703.sp";
connectAttr "curve181.wm" "pSphere1FollicleShape7703.spm";
connectAttr "pSphere1FollicleShape7709.ot" "pSphere1Follicle7709.t" -l on;
connectAttr "pSphere1FollicleShape7709.or" "pSphere1Follicle7709.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape7709.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape7709.inm";
connectAttr "curveShape182.l" "pSphere1FollicleShape7709.sp";
connectAttr "curve182.wm" "pSphere1FollicleShape7709.spm";
connectAttr "pSphere1FollicleShape7715.ot" "pSphere1Follicle7715.t" -l on;
connectAttr "pSphere1FollicleShape7715.or" "pSphere1Follicle7715.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape7715.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape7715.inm";
connectAttr "curveShape183.l" "pSphere1FollicleShape7715.sp";
connectAttr "curve183.wm" "pSphere1FollicleShape7715.spm";
connectAttr "pSphere1FollicleShape7722.ot" "pSphere1Follicle7722.t" -l on;
connectAttr "pSphere1FollicleShape7722.or" "pSphere1Follicle7722.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape7722.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape7722.inm";
connectAttr "curveShape184.l" "pSphere1FollicleShape7722.sp";
connectAttr "curve184.wm" "pSphere1FollicleShape7722.spm";
connectAttr "pSphere1FollicleShape7728.ot" "pSphere1Follicle7728.t" -l on;
connectAttr "pSphere1FollicleShape7728.or" "pSphere1Follicle7728.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape7728.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape7728.inm";
connectAttr "curveShape185.l" "pSphere1FollicleShape7728.sp";
connectAttr "curve185.wm" "pSphere1FollicleShape7728.spm";
connectAttr "pSphere1FollicleShape7734.ot" "pSphere1Follicle7734.t" -l on;
connectAttr "pSphere1FollicleShape7734.or" "pSphere1Follicle7734.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape7734.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape7734.inm";
connectAttr "curveShape186.l" "pSphere1FollicleShape7734.sp";
connectAttr "curve186.wm" "pSphere1FollicleShape7734.spm";
connectAttr "pSphere1FollicleShape7740.ot" "pSphere1Follicle7740.t" -l on;
connectAttr "pSphere1FollicleShape7740.or" "pSphere1Follicle7740.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape7740.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape7740.inm";
connectAttr "curveShape187.l" "pSphere1FollicleShape7740.sp";
connectAttr "curve187.wm" "pSphere1FollicleShape7740.spm";
connectAttr "pSphere1FollicleShape7746.ot" "pSphere1Follicle7746.t" -l on;
connectAttr "pSphere1FollicleShape7746.or" "pSphere1Follicle7746.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape7746.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape7746.inm";
connectAttr "curveShape188.l" "pSphere1FollicleShape7746.sp";
connectAttr "curve188.wm" "pSphere1FollicleShape7746.spm";
connectAttr "pSphere1FollicleShape7753.ot" "pSphere1Follicle7753.t" -l on;
connectAttr "pSphere1FollicleShape7753.or" "pSphere1Follicle7753.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape7753.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape7753.inm";
connectAttr "curveShape189.l" "pSphere1FollicleShape7753.sp";
connectAttr "curve189.wm" "pSphere1FollicleShape7753.spm";
connectAttr "pSphere1FollicleShape7759.ot" "pSphere1Follicle7759.t" -l on;
connectAttr "pSphere1FollicleShape7759.or" "pSphere1Follicle7759.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape7759.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape7759.inm";
connectAttr "curveShape190.l" "pSphere1FollicleShape7759.sp";
connectAttr "curve190.wm" "pSphere1FollicleShape7759.spm";
connectAttr "pSphere1FollicleShape7765.ot" "pSphere1Follicle7765.t" -l on;
connectAttr "pSphere1FollicleShape7765.or" "pSphere1Follicle7765.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape7765.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape7765.inm";
connectAttr "curveShape191.l" "pSphere1FollicleShape7765.sp";
connectAttr "curve191.wm" "pSphere1FollicleShape7765.spm";
connectAttr "pSphere1FollicleShape7771.ot" "pSphere1Follicle7771.t" -l on;
connectAttr "pSphere1FollicleShape7771.or" "pSphere1Follicle7771.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape7771.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape7771.inm";
connectAttr "curveShape192.l" "pSphere1FollicleShape7771.sp";
connectAttr "curve192.wm" "pSphere1FollicleShape7771.spm";
connectAttr "pSphere1FollicleShape7777.ot" "pSphere1Follicle7777.t" -l on;
connectAttr "pSphere1FollicleShape7777.or" "pSphere1Follicle7777.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape7777.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape7777.inm";
connectAttr "curveShape193.l" "pSphere1FollicleShape7777.sp";
connectAttr "curve193.wm" "pSphere1FollicleShape7777.spm";
connectAttr "pSphere1FollicleShape7784.ot" "pSphere1Follicle7784.t" -l on;
connectAttr "pSphere1FollicleShape7784.or" "pSphere1Follicle7784.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape7784.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape7784.inm";
connectAttr "curveShape194.l" "pSphere1FollicleShape7784.sp";
connectAttr "curve194.wm" "pSphere1FollicleShape7784.spm";
connectAttr "pSphere1FollicleShape7790.ot" "pSphere1Follicle7790.t" -l on;
connectAttr "pSphere1FollicleShape7790.or" "pSphere1Follicle7790.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape7790.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape7790.inm";
connectAttr "curveShape195.l" "pSphere1FollicleShape7790.sp";
connectAttr "curve195.wm" "pSphere1FollicleShape7790.spm";
connectAttr "pSphere1FollicleShape7796.ot" "pSphere1Follicle7796.t" -l on;
connectAttr "pSphere1FollicleShape7796.or" "pSphere1Follicle7796.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape7796.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape7796.inm";
connectAttr "curveShape196.l" "pSphere1FollicleShape7796.sp";
connectAttr "curve196.wm" "pSphere1FollicleShape7796.spm";
connectAttr "pSphere1FollicleShape8409.ot" "pSphere1Follicle8409.t" -l on;
connectAttr "pSphere1FollicleShape8409.or" "pSphere1Follicle8409.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape8409.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape8409.inm";
connectAttr "curveShape197.l" "pSphere1FollicleShape8409.sp";
connectAttr "curve197.wm" "pSphere1FollicleShape8409.spm";
connectAttr "pSphere1FollicleShape8415.ot" "pSphere1Follicle8415.t" -l on;
connectAttr "pSphere1FollicleShape8415.or" "pSphere1Follicle8415.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape8415.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape8415.inm";
connectAttr "curveShape198.l" "pSphere1FollicleShape8415.sp";
connectAttr "curve198.wm" "pSphere1FollicleShape8415.spm";
connectAttr "pSphere1FollicleShape8422.ot" "pSphere1Follicle8422.t" -l on;
connectAttr "pSphere1FollicleShape8422.or" "pSphere1Follicle8422.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape8422.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape8422.inm";
connectAttr "curveShape199.l" "pSphere1FollicleShape8422.sp";
connectAttr "curve199.wm" "pSphere1FollicleShape8422.spm";
connectAttr "pSphere1FollicleShape8428.ot" "pSphere1Follicle8428.t" -l on;
connectAttr "pSphere1FollicleShape8428.or" "pSphere1Follicle8428.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape8428.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape8428.inm";
connectAttr "curveShape200.l" "pSphere1FollicleShape8428.sp";
connectAttr "curve200.wm" "pSphere1FollicleShape8428.spm";
connectAttr "pSphere1FollicleShape8434.ot" "pSphere1Follicle8434.t" -l on;
connectAttr "pSphere1FollicleShape8434.or" "pSphere1Follicle8434.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape8434.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape8434.inm";
connectAttr "curveShape201.l" "pSphere1FollicleShape8434.sp";
connectAttr "curve201.wm" "pSphere1FollicleShape8434.spm";
connectAttr "pSphere1FollicleShape8440.ot" "pSphere1Follicle8440.t" -l on;
connectAttr "pSphere1FollicleShape8440.or" "pSphere1Follicle8440.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape8440.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape8440.inm";
connectAttr "curveShape202.l" "pSphere1FollicleShape8440.sp";
connectAttr "curve202.wm" "pSphere1FollicleShape8440.spm";
connectAttr "pSphere1FollicleShape8446.ot" "pSphere1Follicle8446.t" -l on;
connectAttr "pSphere1FollicleShape8446.or" "pSphere1Follicle8446.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape8446.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape8446.inm";
connectAttr "curveShape203.l" "pSphere1FollicleShape8446.sp";
connectAttr "curve203.wm" "pSphere1FollicleShape8446.spm";
connectAttr "pSphere1FollicleShape8453.ot" "pSphere1Follicle8453.t" -l on;
connectAttr "pSphere1FollicleShape8453.or" "pSphere1Follicle8453.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape8453.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape8453.inm";
connectAttr "curveShape204.l" "pSphere1FollicleShape8453.sp";
connectAttr "curve204.wm" "pSphere1FollicleShape8453.spm";
connectAttr "pSphere1FollicleShape8459.ot" "pSphere1Follicle8459.t" -l on;
connectAttr "pSphere1FollicleShape8459.or" "pSphere1Follicle8459.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape8459.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape8459.inm";
connectAttr "curveShape205.l" "pSphere1FollicleShape8459.sp";
connectAttr "curve205.wm" "pSphere1FollicleShape8459.spm";
connectAttr "pSphere1FollicleShape8465.ot" "pSphere1Follicle8465.t" -l on;
connectAttr "pSphere1FollicleShape8465.or" "pSphere1Follicle8465.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape8465.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape8465.inm";
connectAttr "curveShape206.l" "pSphere1FollicleShape8465.sp";
connectAttr "curve206.wm" "pSphere1FollicleShape8465.spm";
connectAttr "pSphere1FollicleShape8471.ot" "pSphere1Follicle8471.t" -l on;
connectAttr "pSphere1FollicleShape8471.or" "pSphere1Follicle8471.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape8471.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape8471.inm";
connectAttr "curveShape207.l" "pSphere1FollicleShape8471.sp";
connectAttr "curve207.wm" "pSphere1FollicleShape8471.spm";
connectAttr "pSphere1FollicleShape8477.ot" "pSphere1Follicle8477.t" -l on;
connectAttr "pSphere1FollicleShape8477.or" "pSphere1Follicle8477.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape8477.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape8477.inm";
connectAttr "curveShape208.l" "pSphere1FollicleShape8477.sp";
connectAttr "curve208.wm" "pSphere1FollicleShape8477.spm";
connectAttr "pSphere1FollicleShape8484.ot" "pSphere1Follicle8484.t" -l on;
connectAttr "pSphere1FollicleShape8484.or" "pSphere1Follicle8484.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape8484.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape8484.inm";
connectAttr "curveShape209.l" "pSphere1FollicleShape8484.sp";
connectAttr "curve209.wm" "pSphere1FollicleShape8484.spm";
connectAttr "pSphere1FollicleShape8490.ot" "pSphere1Follicle8490.t" -l on;
connectAttr "pSphere1FollicleShape8490.or" "pSphere1Follicle8490.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape8490.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape8490.inm";
connectAttr "curveShape210.l" "pSphere1FollicleShape8490.sp";
connectAttr "curve210.wm" "pSphere1FollicleShape8490.spm";
connectAttr "pSphere1FollicleShape9009.ot" "pSphere1Follicle9009.t" -l on;
connectAttr "pSphere1FollicleShape9009.or" "pSphere1Follicle9009.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape9009.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape9009.inm";
connectAttr "curveShape211.l" "pSphere1FollicleShape9009.sp";
connectAttr "curve211.wm" "pSphere1FollicleShape9009.spm";
connectAttr "pSphere1FollicleShape9015.ot" "pSphere1Follicle9015.t" -l on;
connectAttr "pSphere1FollicleShape9015.or" "pSphere1Follicle9015.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape9015.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape9015.inm";
connectAttr "curveShape212.l" "pSphere1FollicleShape9015.sp";
connectAttr "curve212.wm" "pSphere1FollicleShape9015.spm";
connectAttr "pSphere1FollicleShape9022.ot" "pSphere1Follicle9022.t" -l on;
connectAttr "pSphere1FollicleShape9022.or" "pSphere1Follicle9022.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape9022.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape9022.inm";
connectAttr "curveShape213.l" "pSphere1FollicleShape9022.sp";
connectAttr "curve213.wm" "pSphere1FollicleShape9022.spm";
connectAttr "pSphere1FollicleShape9028.ot" "pSphere1Follicle9028.t" -l on;
connectAttr "pSphere1FollicleShape9028.or" "pSphere1Follicle9028.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape9028.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape9028.inm";
connectAttr "curveShape214.l" "pSphere1FollicleShape9028.sp";
connectAttr "curve214.wm" "pSphere1FollicleShape9028.spm";
connectAttr "pSphere1FollicleShape9034.ot" "pSphere1Follicle9034.t" -l on;
connectAttr "pSphere1FollicleShape9034.or" "pSphere1Follicle9034.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape9034.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape9034.inm";
connectAttr "curveShape215.l" "pSphere1FollicleShape9034.sp";
connectAttr "curve215.wm" "pSphere1FollicleShape9034.spm";
connectAttr "pSphere1FollicleShape9040.ot" "pSphere1Follicle9040.t" -l on;
connectAttr "pSphere1FollicleShape9040.or" "pSphere1Follicle9040.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape9040.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape9040.inm";
connectAttr "curveShape216.l" "pSphere1FollicleShape9040.sp";
connectAttr "curve216.wm" "pSphere1FollicleShape9040.spm";
connectAttr "pSphere1FollicleShape9046.ot" "pSphere1Follicle9046.t" -l on;
connectAttr "pSphere1FollicleShape9046.or" "pSphere1Follicle9046.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape9046.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape9046.inm";
connectAttr "curveShape217.l" "pSphere1FollicleShape9046.sp";
connectAttr "curve217.wm" "pSphere1FollicleShape9046.spm";
connectAttr "pSphere1FollicleShape9053.ot" "pSphere1Follicle9053.t" -l on;
connectAttr "pSphere1FollicleShape9053.or" "pSphere1Follicle9053.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape9053.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape9053.inm";
connectAttr "curveShape218.l" "pSphere1FollicleShape9053.sp";
connectAttr "curve218.wm" "pSphere1FollicleShape9053.spm";
connectAttr "pSphere1FollicleShape9059.ot" "pSphere1Follicle9059.t" -l on;
connectAttr "pSphere1FollicleShape9059.or" "pSphere1Follicle9059.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape9059.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape9059.inm";
connectAttr "curveShape219.l" "pSphere1FollicleShape9059.sp";
connectAttr "curve219.wm" "pSphere1FollicleShape9059.spm";
connectAttr "pSphere1FollicleShape9065.ot" "pSphere1Follicle9065.t" -l on;
connectAttr "pSphere1FollicleShape9065.or" "pSphere1Follicle9065.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape9065.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape9065.inm";
connectAttr "curveShape220.l" "pSphere1FollicleShape9065.sp";
connectAttr "curve220.wm" "pSphere1FollicleShape9065.spm";
connectAttr "pSphere1FollicleShape9071.ot" "pSphere1Follicle9071.t" -l on;
connectAttr "pSphere1FollicleShape9071.or" "pSphere1Follicle9071.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape9071.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape9071.inm";
connectAttr "curveShape221.l" "pSphere1FollicleShape9071.sp";
connectAttr "curve221.wm" "pSphere1FollicleShape9071.spm";
connectAttr "pSphere1FollicleShape9077.ot" "pSphere1Follicle9077.t" -l on;
connectAttr "pSphere1FollicleShape9077.or" "pSphere1Follicle9077.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape9077.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape9077.inm";
connectAttr "curveShape222.l" "pSphere1FollicleShape9077.sp";
connectAttr "curve222.wm" "pSphere1FollicleShape9077.spm";
connectAttr "pSphere1FollicleShape9084.ot" "pSphere1Follicle9084.t" -l on;
connectAttr "pSphere1FollicleShape9084.or" "pSphere1Follicle9084.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape9084.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape9084.inm";
connectAttr "curveShape223.l" "pSphere1FollicleShape9084.sp";
connectAttr "curve223.wm" "pSphere1FollicleShape9084.spm";
connectAttr "pSphere1FollicleShape9090.ot" "pSphere1Follicle9090.t" -l on;
connectAttr "pSphere1FollicleShape9090.or" "pSphere1Follicle9090.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape9090.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape9090.inm";
connectAttr "curveShape224.l" "pSphere1FollicleShape9090.sp";
connectAttr "curve224.wm" "pSphere1FollicleShape9090.spm";
connectAttr "pSphere1FollicleShape9603.ot" "pSphere1Follicle9603.t" -l on;
connectAttr "pSphere1FollicleShape9603.or" "pSphere1Follicle9603.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape9603.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape9603.inm";
connectAttr "curveShape225.l" "pSphere1FollicleShape9603.sp";
connectAttr "curve225.wm" "pSphere1FollicleShape9603.spm";
connectAttr "pSphere1FollicleShape9609.ot" "pSphere1Follicle9609.t" -l on;
connectAttr "pSphere1FollicleShape9609.or" "pSphere1Follicle9609.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape9609.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape9609.inm";
connectAttr "curveShape226.l" "pSphere1FollicleShape9609.sp";
connectAttr "curve226.wm" "pSphere1FollicleShape9609.spm";
connectAttr "pSphere1FollicleShape9615.ot" "pSphere1Follicle9615.t" -l on;
connectAttr "pSphere1FollicleShape9615.or" "pSphere1Follicle9615.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape9615.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape9615.inm";
connectAttr "curveShape227.l" "pSphere1FollicleShape9615.sp";
connectAttr "curve227.wm" "pSphere1FollicleShape9615.spm";
connectAttr "pSphere1FollicleShape9622.ot" "pSphere1Follicle9622.t" -l on;
connectAttr "pSphere1FollicleShape9622.or" "pSphere1Follicle9622.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape9622.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape9622.inm";
connectAttr "curveShape228.l" "pSphere1FollicleShape9622.sp";
connectAttr "curve228.wm" "pSphere1FollicleShape9622.spm";
connectAttr "pSphere1FollicleShape9628.ot" "pSphere1Follicle9628.t" -l on;
connectAttr "pSphere1FollicleShape9628.or" "pSphere1Follicle9628.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape9628.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape9628.inm";
connectAttr "curveShape229.l" "pSphere1FollicleShape9628.sp";
connectAttr "curve229.wm" "pSphere1FollicleShape9628.spm";
connectAttr "pSphere1FollicleShape9634.ot" "pSphere1Follicle9634.t" -l on;
connectAttr "pSphere1FollicleShape9634.or" "pSphere1Follicle9634.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape9634.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape9634.inm";
connectAttr "curveShape230.l" "pSphere1FollicleShape9634.sp";
connectAttr "curve230.wm" "pSphere1FollicleShape9634.spm";
connectAttr "pSphere1FollicleShape9640.ot" "pSphere1Follicle9640.t" -l on;
connectAttr "pSphere1FollicleShape9640.or" "pSphere1Follicle9640.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape9640.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape9640.inm";
connectAttr "curveShape231.l" "pSphere1FollicleShape9640.sp";
connectAttr "curve231.wm" "pSphere1FollicleShape9640.spm";
connectAttr "pSphere1FollicleShape9646.ot" "pSphere1Follicle9646.t" -l on;
connectAttr "pSphere1FollicleShape9646.or" "pSphere1Follicle9646.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape9646.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape9646.inm";
connectAttr "curveShape232.l" "pSphere1FollicleShape9646.sp";
connectAttr "curve232.wm" "pSphere1FollicleShape9646.spm";
connectAttr "pSphere1FollicleShape9653.ot" "pSphere1Follicle9653.t" -l on;
connectAttr "pSphere1FollicleShape9653.or" "pSphere1Follicle9653.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape9653.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape9653.inm";
connectAttr "curveShape233.l" "pSphere1FollicleShape9653.sp";
connectAttr "curve233.wm" "pSphere1FollicleShape9653.spm";
connectAttr "pSphere1FollicleShape9659.ot" "pSphere1Follicle9659.t" -l on;
connectAttr "pSphere1FollicleShape9659.or" "pSphere1Follicle9659.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape9659.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape9659.inm";
connectAttr "curveShape234.l" "pSphere1FollicleShape9659.sp";
connectAttr "curve234.wm" "pSphere1FollicleShape9659.spm";
connectAttr "pSphere1FollicleShape9665.ot" "pSphere1Follicle9665.t" -l on;
connectAttr "pSphere1FollicleShape9665.or" "pSphere1Follicle9665.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape9665.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape9665.inm";
connectAttr "curveShape235.l" "pSphere1FollicleShape9665.sp";
connectAttr "curve235.wm" "pSphere1FollicleShape9665.spm";
connectAttr "pSphere1FollicleShape9671.ot" "pSphere1Follicle9671.t" -l on;
connectAttr "pSphere1FollicleShape9671.or" "pSphere1Follicle9671.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape9671.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape9671.inm";
connectAttr "curveShape236.l" "pSphere1FollicleShape9671.sp";
connectAttr "curve236.wm" "pSphere1FollicleShape9671.spm";
connectAttr "pSphere1FollicleShape9677.ot" "pSphere1Follicle9677.t" -l on;
connectAttr "pSphere1FollicleShape9677.or" "pSphere1Follicle9677.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape9677.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape9677.inm";
connectAttr "curveShape237.l" "pSphere1FollicleShape9677.sp";
connectAttr "curve237.wm" "pSphere1FollicleShape9677.spm";
connectAttr "pSphere1FollicleShape9684.ot" "pSphere1Follicle9684.t" -l on;
connectAttr "pSphere1FollicleShape9684.or" "pSphere1Follicle9684.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape9684.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape9684.inm";
connectAttr "curveShape238.l" "pSphere1FollicleShape9684.sp";
connectAttr "curve238.wm" "pSphere1FollicleShape9684.spm";
connectAttr "pSphere1FollicleShape9690.ot" "pSphere1Follicle9690.t" -l on;
connectAttr "pSphere1FollicleShape9690.or" "pSphere1Follicle9690.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape9690.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape9690.inm";
connectAttr "curveShape239.l" "pSphere1FollicleShape9690.sp";
connectAttr "curve239.wm" "pSphere1FollicleShape9690.spm";
connectAttr "pSphere1FollicleShape9696.ot" "pSphere1Follicle9696.t" -l on;
connectAttr "pSphere1FollicleShape9696.or" "pSphere1Follicle9696.r" -l on;
connectAttr "pSphereShape1.wm" "pSphere1FollicleShape9696.iwm";
connectAttr "pSphereShape1.o" "pSphere1FollicleShape9696.inm";
connectAttr "curveShape240.l" "pSphere1FollicleShape9696.sp";
connectAttr "curve240.wm" "pSphere1FollicleShape9696.spm";
connectAttr "hairSystemShape1.orh" "pfxHairShape1.rhs";
connectAttr ":time1.o" "nucleus1.cti";
connectAttr "hairSystemShape1.cust" "nucleus1.niao[0]";
connectAttr "hairSystemShape1.stst" "nucleus1.nias[0]";
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
connectAttr "aiStandard1SG.msg" "materialInfo1.sg";
connectAttr "aiShadowCatcher1.out" "aiUtility1.color";
connectAttr "aiUtility1.out" "aiUtility1SG.ss";
connectAttr "aiUtility1SG.msg" "materialInfo2.sg";
connectAttr "aiUtility1.msg" "materialInfo2.m";
connectAttr "aiUtility1.msg" "materialInfo2.t" -na;
connectAttr "aiStandard2.out" "aiStandard2SG.ss";
connectAttr "pSphereShape1.iog" "aiStandard2SG.dsm" -na;
connectAttr "aiStandard2SG.msg" "materialInfo3.sg";
connectAttr "aiStandard2.msg" "materialInfo3.m";
connectAttr "aiStandard2.msg" "materialInfo3.t" -na;
connectAttr "aiStandard1SG.pa" ":renderPartition.st" -na;
connectAttr "aiUtility1SG.pa" ":renderPartition.st" -na;
connectAttr "aiStandard2SG.pa" ":renderPartition.st" -na;
connectAttr "aiShadowCatcher1.msg" ":defaultShaderList1.s" -na;
connectAttr "aiUtility1.msg" ":defaultShaderList1.s" -na;
connectAttr "aiStandard2.msg" ":defaultShaderList1.s" -na;
connectAttr "directionalLightShape1.ltd" ":lightList1.l" -na;
connectAttr "defaultRenderLayer.msg" ":defaultRenderingList1.r" -na;
connectAttr "directionalLight1.iog" ":defaultLightSet.dsm" -na;
// End of test.ma

!include "MUI2.nsh"

Name "MtoA 0.25.2 Maya 2014"
OutFile "MtoA-0.25.2-win64-2014.exe"

;Default installation folder
InstallDir "C:\solidangle\mtoadeploy\2014"

;Get installation folder from registry if available
InstallDirRegKey HKCU "Software\MtoA2014" ""

;Request application privileges for Windows Vista
RequestExecutionLevel user

Var StartMenuFolder

!define MUI_ICON "SA.ico"
!define MUI_UNICON "SA.ico"

!define MUI_HEADERIMAGE
!define MUI_HEADERIMAGE_BITMAP "top.bmp"
!define MUI_HEADERIMAGE_UNBITMAP "top.bmp"
!define MUI_WELCOMEFINISHPAGE_BITMAP "left.bmp"
!define MUI_UNWELCOMEFINISHPAGE_BITMAP "left.bmp"
!define MUI_ABORTWARNING

!insertmacro MUI_PAGE_WELCOME
!insertmacro MUI_PAGE_LICENSE "MtoAEULA.txt"
!insertmacro MUI_PAGE_COMPONENTS
!insertmacro MUI_PAGE_DIRECTORY
!insertmacro MUI_PAGE_STARTMENU Application $StartMenuFolder
!insertmacro MUI_PAGE_INSTFILES
!insertmacro MUI_PAGE_FINISH

!insertmacro MUI_UNPAGE_WELCOME
!insertmacro MUI_UNPAGE_CONFIRM
!insertmacro MUI_UNPAGE_INSTFILES
!insertmacro MUI_UNPAGE_FINISH

!insertmacro MUI_LANGUAGE "English"


Section "MtoA for Maya 2014" MtoA2014

  ; Check to see if already installed
  SetRegView 64
  ReadRegStr $R0 HKCU "SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\MtoA2014" "UninstallString"
  StrCmp $R0 "" NotInstalled Installed
  Installed:
  MessageBox MB_TOPMOST|MB_OKCANCEL  \
    "MtoA for Maya 2014 is already installed. Remove installed version?" \
    IDOK Uninstall IDCANCEL QuitPart
  Uninstall:
    Exec $R0
  QuitPart:
    Quit

  NotInstalled:
  SetOutPath "$INSTDIR"
  File /r /x *.nsi *.*

  ;Add a mtoa.mod file in the Maya modules folder
  ReadRegStr $R1 HKCU "SOFTWARE\Microsoft\Windows\CurrentVersion\Explorer\Shell Folders" Personal
  CreateDirectory "$R1\maya\2014-x64\modules"
  FileOpen $0 "$R1\maya\2014-x64\modules\mtoa.mod" w
  FileWrite $0 "+ mtoa any $INSTDIR"
  FileClose $0
  
  ;Store installation folder
  SetRegView 32
  WriteRegStr HKCU "Software\MtoA2014" "" $INSTDIR
  
  ;Create uninstaller
  WriteUninstaller "$INSTDIR\Uninstall.exe"
  
  !insertmacro MUI_STARTMENU_WRITE_BEGIN Application
    
    ;Create shortcuts
    CreateDirectory "$SMPROGRAMS\$StartMenuFolder"
    CreateShortCut "$SMPROGRAMS\$StartMenuFolder\Uninstall.lnk" "$INSTDIR\Uninstall.exe"
  
  !insertmacro MUI_STARTMENU_WRITE_END
  
  SetRegView 64
  WriteRegStr HKCU "Software\Microsoft\Windows\CurrentVersion\Uninstall\MtoA2014" \
                 "DisplayName" "MtoA 0.25.2 Maya 2014"
  WriteRegStr HKCU "Software\Microsoft\Windows\CurrentVersion\Uninstall\MtoA2014" \
                 "UninstallString" "$\"$INSTDIR\uninstall.exe$\""

SectionEnd

Section "MtoA for Maya 2014 Env Variables" MtoA2014EnvVariables

  SetOutPath "$INSTDIR"
  
   ;Create .mod file
    SetRegView 64
    ReadRegStr $R0 HKCU "Software\MtoA2014" ""
    
    
    ;Create a backup of Maya.env
    CreateDirectory "$PROFILE\Documents\maya\2014-x64\MtoA_backup"
    CopyFiles "$PROFILE\Documents\maya\2014-x64\Maya.env" "$PROFILE\Documents\maya\2014-x64\MtoA_backup\Maya.env"
    FileOpen $0 "$PROFILE\Documents\maya\2014-x64\MtoA_backup\Maya.env" r
    FileOpen $1 "$PROFILE\Documents\maya\2014-x64\Maya.env" w
    readLine:
    FileRead $0 $2
    FileWrite $1 $2
    StrCmp $2 "" end readLine
    end:
    FileClose $0
    
    ;Add new enviroment variables to Maya.env
    FileWrite $1 "$\r$\nMAYA_RENDER_DESC_PATH = $R0"
    FileWrite $1 "$\r$\nPATH = %PATH%;$R0\bin;$\r$\n"
    FileClose $1

SectionEnd

;--------------------------------
;Descriptions

  ;Language strings
  LangString DESC_MtoA2014 ${LANG_ENGLISH} "Install all MtoA for Maya 2014 files."
  LangString DESC_MtoA2014EnvVariables ${LANG_ENGLISH} "Configure MtoA for Maya 2014 Maya Environment variables."

  ;Assign language strings to sections
  !insertmacro MUI_FUNCTION_DESCRIPTION_BEGIN
    !insertmacro MUI_DESCRIPTION_TEXT ${MtoA2014} $(DESC_MtoA2014)
    !insertmacro MUI_DESCRIPTION_TEXT ${MtoA2014EnvVariables} $(DESC_MtoA2014EnvVariables)
  !insertmacro MUI_FUNCTION_DESCRIPTION_END
   


Section "Uninstall"

  ;ADD YOUR OWN FILES HERE...
  
  RMDir /r /REBOOTOK $INSTDIR
  
  SetRegView 32
  DeleteRegKey /ifempty HKCU "Software\MtoA2014"
  
  SetRegView 64
  ReadRegStr $R1 HKCU "SOFTWARE\Microsoft\Windows\CurrentVersion\Explorer\Shell Folders" Personal
  Delete "$R1\maya\2014-x64\modules\mtoa.mod"
  
  IfFileExists "$PROFILE\Documents\maya\2014-x64\MtoA_backup\Maya.env" deleteMayaEnv removeMenu
  deleteMayaEnv:
  Delete "$PROFILE\Documents\maya\2014-x64\Maya.env"
  CopyFiles "$PROFILE\Documents\maya\2014-x64\MtoA_backup\Maya.env" "$PROFILE\Documents\maya\2014-x64\Maya.env"
  RMDir /r "$PROFILE\Documents\maya\2014-x64\MtoA_backup"
  
  removeMenu:
  !insertmacro MUI_STARTMENU_GETFOLDER Application $StartMenuFolder
    Delete "$SMPROGRAMS\$StartMenuFolder\Uninstall.lnk"
    RMDir "$SMPROGRAMS\$StartMenuFolder"
  
  SetRegView 64
  DeleteRegValue HKCU "Software\Microsoft\Windows\CurrentVersion\Uninstall\MtoA2014" "DisplayName"
  DeleteRegValue HKCU "Software\Microsoft\Windows\CurrentVersion\Uninstall\MtoA2014" "UninstallString"
  DeleteRegKey HKCU "Software\Microsoft\Windows\CurrentVersion\Uninstall\MtoA2014"

SectionEnd


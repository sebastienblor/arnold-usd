!include "MUI2.nsh"
!include "LogicLib.nsh"

Name "MtoA $%MTOA_VERSION_NAME% Maya $%MAYA_VERSION%"
OutFile "MtoA.exe"

;Default installation folder
InstallDir "C:\solidangle\mtoadeploy\$%MAYA_VERSION%"

;Get installation folder from registry if available
InstallDirRegKey HKCU "Software\MtoA$%MAYA_VERSION%" ""

;Request application privileges for Windows Vista
RequestExecutionLevel admin

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


Section "MtoA for Maya $%MAYA_VERSION%" MtoA$%MAYA_VERSION%

  ; Check to see if already installed
  SetRegView 64
  ReadRegStr $R0 HKCU "SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\MtoA$%MAYA_VERSION%" "UninstallString"
  StrCmp $R0 "" NotInstalled Installed
  Installed:
  MessageBox MB_TOPMOST|MB_OKCANCEL  \
    "MtoA for Maya $%MAYA_VERSION% is already installed. Remove installed version?" \
    IDOK Uninstall IDCANCEL QuitPart
  Uninstall:
    Exec $R0
  QuitPart:
    Quit

  NotInstalled:
  SetOutPath "$INSTDIR"
  File /r /x *.nsi /x mtoa.mod *.*

  ;Add a mtoa.mod file in the installer folder
  FileOpen $0 "$INSTDIR\mtoa.mod" w
  FileWrite $0 "+ mtoa any $INSTDIR$\r$\n"
  ${If} "$%MAYA_VERSION%" != "2012"
  FileWrite $0 "PATH +:= bin$\r$\n"
  FileWrite $0 "MAYA_CUSTOM_TEMPLATE_PATH +:= scripts/mtoa/ui/templates$\r$\n"
  ${EndIf}
  FileClose $0
  
  ;Store installation folder
  SetRegView 32
  WriteRegStr HKCU "Software\MtoA$%MAYA_VERSION%" "" $INSTDIR
  
  ;Create uninstaller
  WriteUninstaller "$INSTDIR\Uninstall.exe"
  
  !insertmacro MUI_STARTMENU_WRITE_BEGIN Application
    
    ;Create shortcuts
    CreateDirectory "$SMPROGRAMS\$StartMenuFolder"
    CreateShortCut "$SMPROGRAMS\$StartMenuFolder\Uninstall.lnk" "$INSTDIR\Uninstall.exe"
  
  !insertmacro MUI_STARTMENU_WRITE_END
  
  SetRegView 64
  WriteRegStr HKCU "Software\Microsoft\Windows\CurrentVersion\Uninstall\MtoA$%MAYA_VERSION%" \
                 "DisplayName" "MtoA for Maya $%MAYA_VERSION%"
  WriteRegStr HKCU "Software\Microsoft\Windows\CurrentVersion\Uninstall\MtoA$%MAYA_VERSION%" \
                 "UninstallString" "$\"$INSTDIR\uninstall.exe$\""
  WriteRegStr HKCU "Software\Microsoft\Windows\CurrentVersion\Uninstall\MtoA$%MAYA_VERSION%" \
                 "DisplayVersion" "$%MTOA_VERSION_NAME%"
  WriteRegStr HKCU "Software\Microsoft\Windows\CurrentVersion\Uninstall\MtoA$%MAYA_VERSION%" \
                 "DisplayIcon" "$INSTDIR\uninstall.exe"
  WriteRegStr HKCU "Software\Microsoft\Windows\CurrentVersion\Uninstall\MtoA$%MAYA_VERSION%" \
                 "Publisher" "Solid Angle"

SectionEnd

Section "Configure MtoA for Maya $%MAYA_VERSION%" MtoA$%MAYA_VERSION%EnvVariables

  SetOutPath "$INSTDIR"
  
   ;Create .mod file
    SetRegView 64
    ReadRegStr $R0 HKCU "Software\MtoA$%MAYA_VERSION%" ""
    
    ${If} "$%MAYA_VERSION%" == "2012"
    ;Create a backup of Maya.env
    CreateDirectory "$PROFILE\Documents\maya\$%MAYA_VERSION%-x64\MtoA_backup"
    CopyFiles "$PROFILE\Documents\maya\$%MAYA_VERSION%-x64\Maya.env" "$PROFILE\Documents\maya\$%MAYA_VERSION%-x64\MtoA_backup\Maya.env"
    FileOpen $0 "$PROFILE\Documents\maya\$%MAYA_VERSION%-x64\MtoA_backup\Maya.env" r
    FileOpen $1 "$PROFILE\Documents\maya\$%MAYA_VERSION%-x64\Maya.env" w
    readLine:
    FileRead $0 $2
    FileWrite $1 $2
    StrCmp $2 "" end readLine
    end:
    FileClose $0
    
    ;Add new enviroment variables to Maya.env
    FileWrite $1 "$\r$\nPATH = %PATH%;$R0\bin;$\r$\n"
    FileClose $1
    
    ${EndIf}
    
    ;Add a mtoa.mod file in the Maya modules folder
    ReadRegStr $R1 HKCU "SOFTWARE\Microsoft\Windows\CurrentVersion\Explorer\Shell Folders" Personal
    ${If} "$%MAYA_VERSION%" == "2016"
    CreateDirectory "$R1\maya\$%MAYA_VERSION%\modules"
    FileOpen $0 "$R1\maya\$%MAYA_VERSION%\modules\mtoa.mod" w
    ${Else}
    CreateDirectory "$R1\maya\$%MAYA_VERSION%-x64\modules"
    FileOpen $0 "$R1\maya\$%MAYA_VERSION%-x64\modules\mtoa.mod" w
    ${EndIf}
    FileWrite $0 "+ mtoa any $INSTDIR$\r$\n"
    ${If} "$%MAYA_VERSION%" != "2012"
    FileWrite $0 "PATH +:= bin$\r$\n"
    FileWrite $0 "MAYA_CUSTOM_TEMPLATE_PATH +:= scripts/mtoa/ui/templates$\r$\n"
    ${EndIf}
    FileClose $0
    
    ReadRegStr $R1 HKLM "SOFTWARE\Autodesk\Maya\$%MAYA_VERSION%\Setup\InstallPath" MAYA_INSTALL_LOCATION
    StrCpy $R2 "bin\rendererDesc\arnoldRenderer.xml"
    CopyFiles "$INSTDIR\arnoldRenderer.xml" "$R1$R2"

SectionEnd

;--------------------------------
;Descriptions

  ;Language strings
  LangString DESC_MtoA$%MAYA_VERSION% ${LANG_ENGLISH} "Install MtoA for Maya $%MAYA_VERSION% files."
  LangString DESC_MtoA$%MAYA_VERSION%EnvVariables ${LANG_ENGLISH} "Setup MtoA module configuration files for Maya $%MAYA_VERSION%."

  ;Assign language strings to sections
  !insertmacro MUI_FUNCTION_DESCRIPTION_BEGIN
    !insertmacro MUI_DESCRIPTION_TEXT ${MtoA$%MAYA_VERSION%} $(DESC_MtoA$%MAYA_VERSION%)
    !insertmacro MUI_DESCRIPTION_TEXT ${MtoA$%MAYA_VERSION%EnvVariables} $(DESC_MtoA$%MAYA_VERSION%EnvVariables)
  !insertmacro MUI_FUNCTION_DESCRIPTION_END
   


Section "Uninstall"

  ;ADD YOUR OWN FILES HERE...
  
  RMDir /r /REBOOTOK $INSTDIR
  
  SetRegView 32
  DeleteRegKey /ifempty HKCU "Software\MtoA$%MAYA_VERSION%"
  
  SetRegView 64
  ReadRegStr $R1 HKCU "SOFTWARE\Microsoft\Windows\CurrentVersion\Explorer\Shell Folders" Personal
  ${If} "$%MAYA_VERSION%" == "2016"
  Delete "$R1\maya\$%MAYA_VERSION%\modules\mtoa.mod"
  ${Else}
  Delete "$R1\maya\$%MAYA_VERSION%-x64\modules\mtoa.mod"
  ${EndIf}
  
  ReadRegStr $R1 HKLM "SOFTWARE\Autodesk\Maya\$%MAYA_VERSION%\Setup\InstallPath" MAYA_INSTALL_LOCATION
  StrCpy $R2 "bin\rendererDesc\arnoldRenderer.xml"
  Delete "$R1$R2"
  
  IfFileExists "$PROFILE\Documents\maya\$%MAYA_VERSION%-x64\MtoA_backup\Maya.env" deleteMayaEnv removeMenu
  deleteMayaEnv:
  Delete "$PROFILE\Documents\maya\$%MAYA_VERSION%-x64\Maya.env"
  CopyFiles "$PROFILE\Documents\maya\$%MAYA_VERSION%-x64\MtoA_backup\Maya.env" "$PROFILE\Documents\maya\$%MAYA_VERSION%-x64\Maya.env"
  RMDir /r "$PROFILE\Documents\maya\$%MAYA_VERSION%-x64\MtoA_backup"
  
  removeMenu:
  !insertmacro MUI_STARTMENU_GETFOLDER Application $StartMenuFolder
    Delete "$SMPROGRAMS\$StartMenuFolder\Uninstall.lnk"
    RMDir "$SMPROGRAMS\$StartMenuFolder"
  
  SetRegView 64
  DeleteRegValue HKCU "Software\Microsoft\Windows\CurrentVersion\Uninstall\MtoA$%MAYA_VERSION%" "DisplayName"
  DeleteRegValue HKCU "Software\Microsoft\Windows\CurrentVersion\Uninstall\MtoA$%MAYA_VERSION%" "UninstallString"
  DeleteRegValue HKCU "Software\Microsoft\Windows\CurrentVersion\Uninstall\MtoA$%MAYA_VERSION%" "DisplayVersion"
  DeleteRegValue HKCU "Software\Microsoft\Windows\CurrentVersion\Uninstall\MtoA$%MAYA_VERSION%" "DisplayIcon"
  DeleteRegValue HKCU "Software\Microsoft\Windows\CurrentVersion\Uninstall\MtoA$%MAYA_VERSION%" "Publisher"
  DeleteRegKey HKCU "Software\Microsoft\Windows\CurrentVersion\Uninstall\MtoA$%MAYA_VERSION%"

SectionEnd


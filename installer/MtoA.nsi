!include "MUI2.nsh"
!include "LogicLib.nsh"

!include "FileFunc.nsh"
!insertmacro GetParameters
!insertmacro GetOptions

Function .onInit
  ${GetParameters} $R0
  ClearErrors
  ${GetOptions} $R0 /FORCE_UNINSTALL= $3
FunctionEnd

Name "MtoA $%MTOA_VERSION_NAME% Maya $%MAYA_VERSION%"
OutFile "MtoA.exe"

;Default installation folder
InstallDir "C:\solidangle\mtoadeploy\$%MAYA_VERSION%"

;Get installation folder from registry if available
InstallDirRegKey HKCU "Software\MtoA$%MAYA_VERSION%" ""

;Request application privileges for Windows Vista
RequestExecutionLevel admin

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
  IfSilent 0 +5
    IntCmp $3 1 0 QuitPart QuitPart
      ExecWait "$R0 /S"
      Sleep 3000
      Goto NotInstalled
    
  MessageBox MB_TOPMOST|MB_OKCANCEL  \
    "MtoA for Maya $%MAYA_VERSION% is already installed. Replace existing version ?" \
    IDOK Uninstall IDCANCEL QuitPart
  Uninstall:

    ; Copied from the Uninstall section at the end of this file
    ; FIXME is there a way to put both in a single piece of code ?

    RMDir /r /REBOOTOK $INSTDIR
  
    SetRegView 32
    DeleteRegKey /ifempty HKCU "Software\MtoA$%MAYA_VERSION%"
    ReadRegStr $R1 HKLM "SOFTWARE\Microsoft\Windows\CurrentVersion" CommonW6432Dir
    
    Delete "$R1\Autodesk Shared\Modules\Maya\$%MAYA_VERSION%\mtoa.mod"
    
    SetRegView 64
    ReadRegStr $R1 HKLM "SOFTWARE\Autodesk\Maya\$%MAYA_VERSION%\Setup\InstallPath" MAYA_INSTALL_LOCATION
    StrCpy $R2 "bin\rendererDesc\arnoldRenderer.xml"
    Delete "$R1$R2"
    
    ReadRegStr $R1 HKCU "SOFTWARE\Microsoft\Windows\CurrentVersion\Explorer\Shell Folders" Personal
    ${If} "$%MAYA_VERSION%" >= "2017"
    Delete "$R1\maya\RSTemplates\MatteOverride-Arnold.json"
    Delete "$R1\maya\RSTemplates\RenderLayerExample-Arnold.json"
    ${EndIf}
    
    Delete "$SMPROGRAMS\Arnold for Maya $%MAYA_VERSION%\Uninstall.lnk"
    RMDir "$SMPROGRAMS\Arnold for Maya $%MAYA_VERSION%"
  
    SetRegView 64
    DeleteRegValue HKCU "Software\Microsoft\Windows\CurrentVersion\Uninstall\MtoA$%MAYA_VERSION%" "DisplayName"
    DeleteRegValue HKCU "Software\Microsoft\Windows\CurrentVersion\Uninstall\MtoA$%MAYA_VERSION%" "UninstallString"
    DeleteRegValue HKCU "Software\Microsoft\Windows\CurrentVersion\Uninstall\MtoA$%MAYA_VERSION%" "DisplayVersion"
    DeleteRegValue HKCU "Software\Microsoft\Windows\CurrentVersion\Uninstall\MtoA$%MAYA_VERSION%" "DisplayIcon"
    DeleteRegValue HKCU "Software\Microsoft\Windows\CurrentVersion\Uninstall\MtoA$%MAYA_VERSION%" "Publisher"
    DeleteRegKey HKCU "Software\Microsoft\Windows\CurrentVersion\Uninstall\MtoA$%MAYA_VERSION%"

    CreateDirectory $INSTDIR
    Goto NotInstalled

  QuitPart:
    Quit

  NotInstalled:
  SetOutPath "$INSTDIR"
  File /r /x *.nsi /x mtoa.mod *.*
  File /x *.nsi /x mtoa.mod *.*
  

  ;Add a mtoa.mod file in the installer folder
  FileOpen $0 "$INSTDIR\mtoa.mod" w
  FileWrite $0 "+ mtoa any $INSTDIR$\r$\n"
  FileWrite $0 "PATH +:= bin$\r$\n"
  FileWrite $0 "MAYA_CUSTOM_TEMPLATE_PATH +:= scripts/mtoa/ui/templates$\r$\n"
  FileWrite $0 "MAYA_SCRIPT_PATH +:= scripts/mtoa/mel$\r$\n"
  FileWrite $0 "MAYA_RENDER_DESC_PATH += $INSTDIR$\r$\n"
  
  FileClose $0
  
  ;Store installation folder
  SetRegView 32
  WriteRegStr HKCU "Software\MtoA$%MAYA_VERSION%" "" $INSTDIR
  
  ;Create uninstaller
  WriteUninstaller "$INSTDIR\Uninstall.exe"
  
  ;Create shortcuts
  CreateDirectory "$SMPROGRAMS\Arnold for Maya $%MAYA_VERSION%"
  CreateShortCut "$SMPROGRAMS\Arnold for Maya $%MAYA_VERSION%\Uninstall.lnk" "$INSTDIR\Uninstall.exe"
  
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
    
    ;Add a mtoa.mod file in the Maya modules folder
    SetRegView 32
    ReadRegStr $R1 HKLM "SOFTWARE\Microsoft\Windows\CurrentVersion" CommonW6432Dir
    
    CreateDirectory "$R1\Autodesk Shared\Modules\Maya\$%MAYA_VERSION%"
    FileOpen $0 "$R1\Autodesk Shared\Modules\Maya\$%MAYA_VERSION%\mtoa.mod" w
    FileWrite $0 "+ mtoa any $INSTDIR$\r$\n"
    FileWrite $0 "PATH +:= bin$\r$\n"
    FileWrite $0 "MAYA_CUSTOM_TEMPLATE_PATH +:= scripts/mtoa/ui/templates$\r$\n"
    FileWrite $0 "MAYA_SCRIPT_PATH +:= scripts/mtoa/mel$\r$\n"
    FileWrite $0 "MAYA_RENDER_DESC_PATH += $INSTDIR$\r$\n"
    FileClose $0
    
    ${If} "$%MAYA_VERSION%" < "2018"
    SetRegView 64
    ReadRegStr $R1 HKLM "SOFTWARE\Autodesk\Maya\$%MAYA_VERSION%\Setup\InstallPath" MAYA_INSTALL_LOCATION
    StrCpy $R2 "bin\rendererDesc\arnoldRenderer.xml"
    CopyFiles "$INSTDIR\arnoldRenderer.xml" "$R1$R2"
    ${EndIf}


    ReadRegStr $R1 HKCU "SOFTWARE\Microsoft\Windows\CurrentVersion\Explorer\Shell Folders" Personal
    ${If} "$%MAYA_VERSION%" >= "2017"
    CreateDirectory "$R1\maya\RSTemplates"
    CopyFiles "$INSTDIR\RSTemplates\*.json" "$R1\maya\RSTemplates"
    ${EndIf}

    ; run the install script

    StrCpy $R3 "$INSTDIR\license\ArnoldLicensing-8.1.0.951_RC6-win.exe"
    ${If} ${FileExists} "$R3"
      ExecWait '$INSTDIR\license\ArnoldLicensing-8.1.0.951_RC6-win.exe --silent'
    ${EndIf}
    

    StrCpy $R4 "$INSTDIR\license\pitreg.exe"
    ${If} ${FileExists} "$R4"
        ExecWait '"$INSTDIR\license\pitreg.exe"' $0
        StrCpy $R3 "Error $0"
        ${If} "$0" == "2"
           StrCpy $R3 "File could not be opened"
        ${ElseIf} "$0" == "24"
          StrCpy $R3 "File not found"
        ${ElseIf} "$0" == "25"
          StrCpy $R3 "Error while parsing .pit file"
        ${ElseIf} "$0" == "27"
          StrCpy $R3 "Invalid PIT File"
        ${ElseIf} "$0" == "32"
          StrCpy $R3 "Unable to set write access for all user in Linux and MAC"
        ${EndIf}    

        ${If} "$0" > "0"
          MessageBox MB_TOPMOST|MB_OK  \
            "Couldn't register Arnold renderer in Maya PIT file ($R3). Please contact support@solidangle.com"
        ${EndIf}
    ${EndIf}

     
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

  ; If you do some changes here, don't forget to port them above in the Uninstall: part
  ; FIXME is there a way to put both in a single piece of code ?

  ;add your own files here...
  
  RMDir /r /REBOOTOK $INSTDIR
  
  SetRegView 32
  DeleteRegKey /ifempty HKCU "Software\MtoA$%MAYA_VERSION%"
  ReadRegStr $R1 HKLM "SOFTWARE\Microsoft\Windows\CurrentVersion" CommonW6432Dir
  
  Delete "$R1\Autodesk Shared\Modules\Maya\$%MAYA_VERSION%\mtoa.mod"
  
  SetRegView 64
  ReadRegStr $R1 HKLM "SOFTWARE\Autodesk\Maya\$%MAYA_VERSION%\Setup\InstallPath" MAYA_INSTALL_LOCATION
  StrCpy $R2 "bin\rendererDesc\arnoldRenderer.xml"
  Delete "$R1$R2"
  
  ReadRegStr $R1 HKCU "SOFTWARE\Microsoft\Windows\CurrentVersion\Explorer\Shell Folders" Personal
  ${If} "$%MAYA_VERSION%" >= "2017"
  Delete "$R1\maya\RSTemplates\MatteOverride-Arnold.json"
  Delete "$R1\maya\RSTemplates\RenderLayerExample-Arnold.json"
  ${EndIf}
  
  Delete "$SMPROGRAMS\Arnold for Maya $%MAYA_VERSION%\Uninstall.lnk"
  RMDir "$SMPROGRAMS\Arnold for Maya $%MAYA_VERSION%"

  SetRegView 64
  DeleteRegValue HKCU "Software\Microsoft\Windows\CurrentVersion\Uninstall\MtoA$%MAYA_VERSION%" "DisplayName"
  DeleteRegValue HKCU "Software\Microsoft\Windows\CurrentVersion\Uninstall\MtoA$%MAYA_VERSION%" "UninstallString"
  DeleteRegValue HKCU "Software\Microsoft\Windows\CurrentVersion\Uninstall\MtoA$%MAYA_VERSION%" "DisplayVersion"
  DeleteRegValue HKCU "Software\Microsoft\Windows\CurrentVersion\Uninstall\MtoA$%MAYA_VERSION%" "DisplayIcon"
  DeleteRegValue HKCU "Software\Microsoft\Windows\CurrentVersion\Uninstall\MtoA$%MAYA_VERSION%" "Publisher"
  DeleteRegKey HKCU "Software\Microsoft\Windows\CurrentVersion\Uninstall\MtoA$%MAYA_VERSION%"

SectionEnd


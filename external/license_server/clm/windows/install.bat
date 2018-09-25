set SCRIPT_DIR=%~dp0

set VERSION=8.0.1.879
set PIT_FILE=%SCRIPT_DIR%\ArnoldConfig.pit

:: install Autodesk Licensing (Service, Agent, Adlm and FlexNET client) silently
"%SCRIPT_DIR%\AdskLicensingInstaller-%VERSION%\AdskLicensing-installer.exe" --mode unattended --unattendedmodeui none
@echo off
if %errorlevel% neq 0 (
    echo "[ERROR] Failed to install Autodesk Licensing"
    exit /b %errorlevel%
) else (
    echo "Autodesk Licensing install done"
)
@echo on

:: install AdSSO (Single Sign On)
msiexec /i "%SCRIPT_DIR%\AdSSO.msi" /passive ALLUSERS=2 /norestart
@echo off
if %errorlevel% neq 0 (
    echo "[ERROR] Failed to install the Autodesk Single Sign On component"
    exit /b %errorlevel%
) else (
    echo "Autodesk Single Sign On install done"
)
@echo on

:: register the Arnold product
"c:\Program Files (x86)\Common Files\Autodesk Shared\AdskLicensing\Current\helper\AdskLicensingInstHelper.exe" register --prod_key C0PK1 --prod_ver 2019.0.0.P --config_file "%PIT_FILE%" --eula_locale en_US
@echo off
if %errorlevel% neq 0 (
    echo "[ERROR] Failed to register the Arnold product"
    exit /b %errorlevel%
) else (
    echo "Arnold product registration done"
)
@echo on


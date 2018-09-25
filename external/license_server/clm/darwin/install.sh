#!/bin/bash

export SCRIPT_DIR=$(dirname $0)
export VERSION=8.0.1.879
export PIT_FILE=$SCRIPT_DIR/ArnoldConfig.pit

install_pkg()
{
    filename=$1
    name=$2
    pkgid=$3

    pkgutil --pkgs=$pkgid
    if [ $? -ne 0 ]; then
        installer -pkg "$SCRIPT_DIR/$filename.pkg" -target / 
        errcode=$?
        if [ $errcode -ne 0 ]; then
            echo "[ERROR] Failed to install $name ($errcode)"
            exit $errcode
        else
            echo "$name install done"
        fi
    fi
}

# install Autodesk Licensing (Service, Agent)
install_pkg "AdskLicensing-$VERSION-mac-installer" "Autodesk Licensing" "com.autodesk.AdskLicensing" 

# install Adlm and FlexNET client
install_pkg "adlmframework16" "Adlm framework" "com.Autodesk.adlmframework16"
install_pkg "adlmapps16" "Adlm apps" "com.Autodesk.adlmapps16"
install_pkg "adlmflexnetclient" "FlexNET client" "com.Autodesk.adlmflexnetclient16"

# install AdSSO (Single Sing On)
install_pkg "AdSSO-v2" "Autodesk Single Sign On" "com.autodesk.AdSSO-v2"

# register the Arnold product
"/Library/Application Support/Autodesk/AdskLicensing/Current/helper/AdskLicensingInstHelper" register --prod_key C0PK1 --prod_ver 2019.0.0.P --config_file "$PIT_FILE" --eula_locale en_US
errcode=$?
if [ $errcode -ne 0 ]; then
    echo "[ERROR] Failed to register the Arnold product ($errcode)"
    exit $errcode
else
    echo "Arnold product registration done"
fi

# successful install
echo "OK" > "$SCRIPT_DIR/.success"


#!/bin/sh

export SCRIPT_DIR=$(dirname $0)
export VERSION=8.0.1.879
export PIT_FILE=$SCRIPT_DIR/ArnoldConfig.pit

install_rpm()
{
    filename=$1
    name=$2

    rpm -q "$filename"
    if [ $? -ne 0 ]; then
        rpm -Uvh "$SCRIPT_DIR/x86_64/$filename.rpm"
        errcode=$?
        if [ $errcode -ne 0 ]; then
            echo "[ERROR] Failed to install $name ($errcode)"
            exit $errcode
        else
            echo "$name install done"
        fi
    fi
}

# unzip the archive
tar xvzf "$SCRIPT_DIR/AdskLicensingInstaller-8.0.1.879.tar.gz" -C "$SCRIPT_DIR/"

# install Autodesk Licensing (Service, Agent)
install_rpm "adsklicensing$VERSION-0-0.x86_64" "Autodesk Licensing"

# install Adlm and FlexNET client
install_rpm "adlmapps16-16.0.79-0.x86_64" "Adlm apps"
install_rpm "adlmflexnetclient-16.0.79-0.x86_64" "FlexNET client"

# register the Arnold product
"/opt/Autodesk/AdskLicensing/$VERSION/helper/AdskLicensingInstHelper" register --prod_key C0PK1 --prod_ver 2019.0.0.P --config_file "$SCRIPT_DIR/ArnoldConfig.pit" --eula_locale en_US
errcode=$?
if [ $errcode -ne 0 ]; then
    echo "[ERROR] Failed to register the Arnold product ($errcode)"
    exit $errcode
else
    echo "Arnold product registration done"
fi

# successful install
echo "OK" > "$SCRIPT_DIR/.success"


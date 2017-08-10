import os
import sys

# Compute all paths to run kick

maya_root_path    = sys.argv[1]
mtoa_root_path    = sys.argv[2]

makeTxPath        = os.path.join(mtoa_root_path, 'bin', 'maketx')
synColor_path     = os.path.join(maya_root_path, 'synColor')

# Create a syncolorConfig file with the path to maya syncolor native catalogs
synColorConfig = '<?xml version="1.0" encoding="UTF-8"?> <SynColorConfig version="2.0"> <AutoConfigure graphicsMonitor="false" /> <TransformsHome dir="%s" /> <SharedHome dir="." /> <ReferenceTable> <Ref alias="OutputToSceneBridge" path="misc/identity.ctf" basePath="Autodesk" /> <Ref alias="SceneToOutputBridge" path="RRT+ODT/ACES_to_CIE-XYZ_v0.1.1.ctf" basePath="Autodesk" /> <Ref alias="broadcastMonitor" path="display/broadcast/CIE-XYZ_to_HD-video.ctf" basePath="Autodesk" /> <Ref alias="defaultLook" path="misc/identity.ctf" basePath="Autodesk" /> <Ref alias="graphicsMonitor" path="interchange/sRGB/CIE-XYZ_to_sRGB.ctf" basePath="Autodesk" /> </ReferenceTable> </SynColorConfig>' \
  % (synColor_path)

with open('synColorConfig.xml','w') as f:
    f.write(synColorConfig)

cmd = '%s --nomipmap --colorengine "syncolor" --colorconfig synColorConfig.xml --colorconvert "Sony SLog2" "ACEScg" texture.jpg -o testrender.tif' \
    % (makeTxPath)

print cmd

os.system(cmd)


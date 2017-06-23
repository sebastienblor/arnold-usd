import os
import sys
import tempfile

# Compute all paths to run kick

maya_root_path    = sys.argv[1]
makeTx_path       = os.path.join(sys.argv[2], 'bin', 'makeTx')
test_root_path    = sys.argv[3]
synColor_path     = os.path.join(maya_root_path, 'synColor')
input_file_path   = os.path.join(test_root_path, 'texture.jpg')
output_file_path  = os.path.join(test_root_path, 'testrender.tif')

# Execute the script

cmd = '%s --nomipmap --colorengine "syncolor" --colorconfig "%s" --colorconvert "Sony SLog2" "ACEScg" "%s" -o "%s"' \
    % (makeTx_path, synColor_path, input_file_path, output_file_path)

os.system(cmd)
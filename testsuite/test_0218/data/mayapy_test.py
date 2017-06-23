import os
import sys
import tempfile

# Compute all paths to run kick

maya_root_path    = sys.argv[1]
mtoa_shaders_path = os.path.join(sys.argv[2], 'shaders')
test_root_path    = sys.argv[3]
synColor_path     = os.path.join(maya_root_path, 'synColor')
output_file_path  = os.path.join(test_root_path, 'testrender.tif')
ocio_file_path    = os.path.join(test_root_path, 'ocio', 'config.ocio')

# Build the ass file with the right synColor catalog path

(fd, ass_filename) = tempfile.mkstemp('.ass')

refFile = open(os.path.join(test_root_path, 'ref.ass'), 'r')
for line in refFile:
    if '__REPLACE__' in line.split():
        os.write(fd, 'native_catalog_path "%s"\n' % synColor_path)
    elif '__OCIO__' in line.split():
        os.write(fd, 'ocioconfig_path "%s"\n' % ocio_file_path)
    else:
        os.write(fd, line)
os.close(fd)
refFile.close()

# Execute the script

cmd = 'kick -v 0 -t 64 -dp -dw -l %s %s -o %s' % (mtoa_shaders_path, ass_filename, output_file_path)
os.system(cmd)
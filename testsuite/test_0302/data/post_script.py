import sys
import os
oiiotool_path = sys.argv[2]

#cmd = ('%s -v --info %s') % (oiiotool_path, 'testrender_diffuse_direct.exr')
#print cmd
#os.system(cmd)
print 'Extracting channels from multi-layer images'
cmd = ('%s %s -ch "%s,%s,%s" -o %s') % (oiiotool_path, 'testrender.exr', 
	'R', 'G', 'B', 'testrender_beauty.tif')
os.system(cmd)
cmd = ('%s %s -ch "%s,%s,%s" -o %s') % (oiiotool_path, 'testrender.exr', 
	'N.X', 'N.Y', 'N.Z', 'testrender_n.tif')
os.system(cmd)

cmd = ('%s %s -ch "%s,%s,%s" -o %s') % (oiiotool_path, 'testrender.exr', 
	'N_1.X', 'N_1.Y', 'N_1.Z', 'testrender_n_1.tif')
os.system(cmd)

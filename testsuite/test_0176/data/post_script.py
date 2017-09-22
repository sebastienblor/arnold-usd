import sys

oiiotool_path = sys.argv[2]

#cmd = ('%s -v --info %s') % (oiiotool_path, 'testrender_diffuse_direct.exr')
#print cmd
#os.system(cmd)
print 'Extracting channels from multi-layer images'
cmd = ('%s %s -ch "%s,%s,%s" -o %s') % (oiiotool_path, 'testrender_diffuse_direct_lgroups.exr', 
	'diffuse_direct_red.R', 'diffuse_direct_red.G', 'diffuse_direct_red.B', 'testrender_diffuse_direct_red.tif')
os.system(cmd)
cmd = ('%s %s -ch "%s,%s,%s" -o %s') % (oiiotool_path, 'testrender_diffuse_direct_lgroups.exr', 
	'diffuse_direct_green.R', 'diffuse_direct_green.G', 'diffuse_direct_green.B', 'testrender_diffuse_direct_green.tif')
os.system(cmd)

cmd = ('%s %s -ch "%s,%s,%s" -o %s') % (oiiotool_path, 'testrender_diffuse_direct_lgroups.exr', 
	'diffuse_direct_blue.R', 'diffuse_direct_blue.G', 'diffuse_direct_blue.B', 'testrender_diffuse_direct_blue.tif')
os.system(cmd)
cmd = ('%s %s -ch "%s,%s,%s" -o %s') % (oiiotool_path, 'testrender_diffuse_direct_lgroups.exr', 
	'diffuse_direct_default.R', 'diffuse_direct_default.G', 'diffuse_direct_default.B', 'testrender_diffuse_direct_default.tif')
os.system(cmd)

cmd = ('%s %s -ch "%s,%s,%s" -o %s') % (oiiotool_path, 'testrender_diffuse_indirect_lgroups.exr', 
	'diffuse_indirect_red.R', 'diffuse_indirect_red.G', 'diffuse_indirect_red.B', 'testrender_diffuse_indirect_red.tif')
os.system(cmd)
cmd = ('%s %s -ch "%s,%s,%s" -o %s') % (oiiotool_path, 'testrender_diffuse_indirect_lgroups.exr', 
	'diffuse_indirect_green.R', 'diffuse_indirect_green.G', 'diffuse_indirect_green.B', 'testrender_diffuse_indirect_green.tif')

os.system(cmd)
cmd = ('%s %s -ch "%s,%s,%s" -o %s') % (oiiotool_path, 'testrender_diffuse_indirect_lgroups.exr', 
	'diffuse_indirect_default.R', 'diffuse_indirect_default.G', 'diffuse_indirect_default.B', 'testrender_diffuse_indirect_default.tif')
os.system(cmd)

cmd = ('%s %s -ch "%s,%s,%s" -o %s') % (oiiotool_path, 'testrender_diffuse_indirect_lgroups.exr', 
	'diffuse_indirect_blue.R', 'diffuse_indirect_blue.G', 'diffuse_indirect_blue.B', 'testrender_diffuse_indirect_blue.tif')

os.system(cmd)

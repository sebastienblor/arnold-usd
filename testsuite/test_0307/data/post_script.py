import sys
import os
import os.path

num_png = 0
files = [f for f in os.listdir('.') if os.path.isfile(f)]
for f in files:
	if f and len(f) > 5 and f[-4:] == '.png':
		num_png = num_png + 1

# We are expecting exactly 2 pngs : testrender_beauty.png and testrender_Z.png

if num_png == 2:
	print 'OK'
	shutil.copy('ok.tif', 'testrender.tif')
else:
	print 'FAIL'
	shutil.copy('fail.tif', 'testrender.tif')



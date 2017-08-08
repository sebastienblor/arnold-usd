import sys

test_root_path = sys.argv[1]
oiiotool_path  = sys.argv[2]

index = 0
for root, dirs, files in os.walk(test_root_path):
	for img in files:
		if not img.endswith('.exr'):
			continue
		if not 'testrender' in img:
			continue
		
		outImg = "testrender"
		outImg += str(index)
		outImg += ".tif"

		inputImg = os.path.join(test_root_path, img)
		outImg = os.path.join(test_root_path, outImg)

		if 'Aov' in img:
			cmd = ('%s %s -ch "R,G,B" -o %s') % (oiiotool_path, inputImg, outImg)
		else:
			cmd = ('%s %s -ch "R,G,B,A" -o %s') % (oiiotool_path, inputImg, outImg)

		os.system(cmd)
		index = index+1


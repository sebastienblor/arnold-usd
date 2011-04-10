#!/usr/bin/env python
"""
this script updates maya ascii files from mtoa version 0.4 to 0.5.  

so far, it corrects:
 - options short names

"""
import sys
import re
import os
import mtoa.metadata as meta

SET_REG = re.compile('setAttr "\.([^"]+)')


def main(filepath):
    paramData = meta.getAllParamData('options')
    fixMap = {}

    # create a map from original shortname to new shortname
    for param, data in paramData.items():
        if 'maya.orig_shortname' in data:
            fixMap[data['maya.orig_shortname']] = param

    print "reading '%s'..." % filepath
    
    active = False
    modified = False

    f = open(filepath, 'r')
    lines = f.readlines()
    f.close()
    for i, line in enumerate(lines):
        if line.startswith('createNode '):
            if line.startswith('createNode ArnoldRenderOptions -s -n "defaultArnoldRenderOptions"'):
                # entered the globals block
                active = True
            elif active == True:
                # all done
                break
        elif active == True:
            m = SET_REG.search(line)
            if m:
                attr = m.groups()[0]
                try:
                    newAttr = fixMap[attr] 
                    lines[i] = line.replace(attr, newAttr)
                    modified = True
                except KeyError:
                    pass
    if modified:
        fname, ext = os.path.splitext(filepath)
        newFilename = fname + '_upgraded' + ext
        print "writing upgraded file to '%s'" % newFilename
        f = open(newFilename, 'w')
        f.writelines(lines)
        f.close()
    else:
        print "file did not need to be fixed"
        
if __name__ == '__main__':
    try:
        f = sys.argv[1]
    except IndexError:
        "pass the name of a file to upgrade"
        sys.exit(1)
    main(f)

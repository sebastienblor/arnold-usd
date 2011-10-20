"""
create an arnold .mtd metadata file for a shader library.  

finds all shaders within a given shader library that have no 'maya.id' metadata,
and writes an mtd file that generates this metadata.
"""

import arnold
import ctypes
import os.path

def make_mtd(shaderlib, startid):
    arnold.AiBegin()
    arnold.AiLoadPlugins(os.getenv('ARNOLD_PLUGIN_PATH'))
    
    iter = arnold.AiUniverseGetNodeEntryIterator(arnold.AI_NODE_SHADER)
    s = ''
    while not arnold.AiNodeEntryIteratorFinished(iter):
        nentry = arnold.AiNodeEntryIteratorGetNext(iter)
        if shaderlib == arnold.AiNodeEntryGetFilename(nentry):
            hide = ctypes.c_ubyte()
            arnold.AiMetaDataGetBool(nentry, None, 'maya.hide', ctypes.byref(hide))
            if not bool(hide):
                nodeid =  ctypes.c_int()
                arnold.AiMetaDataGetInt(nentry, None, 'maya.id', ctypes.byref(nodeid))
                if nodeid.value == 0:
                    s += '[node %s]\n' % arnold.AiNodeEntryGetName(nentry)
                    s += '    maya.id                 INT     0x00%s\n\n' % hex(startid).split('x')[-1].upper()
                    startid+=1
            
    arnold.AiEnd()
    if s:
        print s
        mtd = os.path.splitext(shaderlib)[0] + '.mtd'
        print "writing", mtd
        with open(mtd, 'w+') as f:
            f.write(s)

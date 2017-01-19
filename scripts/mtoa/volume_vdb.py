#!/usr/bin/env python
import os
import ctypes
import platform
from arnold import *

_libext = {'windows'  :'.dll',
           'microsoft':'.dll',
           'linux'    :'.so',
           'darwin'   :'.dylib'}

try:
    dso_path = os.path.join(os.path.dirname(os.path.dirname(os.path.dirname(__file__))), 'procedurals', 'volume_openvdb' + _libext[platform.system().lower()])
    dso = ctypes.CDLL(dso_path)
    
    dso.channelNames.argtypes = [ctypes.c_char_p]
    dso.channelNames.restype = POINTER(AtArray)
    
    dso.channelBBox.argtypes = [ctypes.c_char_p, POINTER(AtArray)]
    dso.channelBBox.restype = AtBBox
    
    dso.minVoxelSize.argtypes = [ctypes.c_char_p, POINTER(AtArray)]
    dso.minVoxelSize.restype = ctypes.c_float
    
except:
    dso = None
    
def GetChannelNames(filename):
    if not dso:
        return []    

    channel_array = dso.channelNames(filename)
    channel_names = [AiArrayGetStr(channel_array, i) for i in xrange(channel_array.contents.nelements)]
    return channel_names


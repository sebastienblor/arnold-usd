
import yaml
import sys
import os
import pprint

_data = None

def _load():
    global _data
    if _data is None:
        mod = sys.modules[__name__]
        dir = os.path.split(mod.__file__)
        metafile = os.path.join(dir[0], 'builtin.yaml')
        f = open(metafile)
        try:
            _data = yaml.load(f)
        finally:
            f.close()

def hasParamData(node, parameter, name):
    global _data
    try:
        _data[node]['parameter_metadata'][parameter][name]
        return 1
    except KeyError:
        return 0
    
def getParamData(node, parameter, name):
    global _data
    try:
        return _data[node]['parameter_metadata'][parameter][name]
    except KeyError:
        pass

def hasNodeData(node, name):
    global _data
    try:
        _data[node]['node_metadata'][name]
        return 1
    except KeyError:
        return 0

def getNodeData(node, name):
    global _data
    try:
        return _data[node]['node_metadata'][name]
    except KeyError:
        pass

_load()
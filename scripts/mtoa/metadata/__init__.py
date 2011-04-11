"""
module for finding arnold metadata in plain-text yaml files

arnold yaml metadata files are formatted as follows::

sky:
    parameter_metadata:
        intensity:
            min: 0
            softmax: 10
            default: 1

options:
    node_metadata:
        maya.id: 0x00071000
    parameter_metadata:
        AA_samples:
            maya.keyable: false
            softmin: 1
            softmax: 10
            min: 1
            max: 100
            default: 3
        bucket_scanning:
            maya.keyable: false

"""

import yaml
import sys
import os
import glob
import pprint

_data = None

def _loadData():
    """
    load all metadata from yaml files found on MTOA_PLUGIN_PATH
    """
    global _data
    if _data is None:
        _data = {}
        # get builtin yaml file, which we keep internally, for now. should be 
        # moved into the MTOA_PLUGINS_PATH
        mod = sys.modules[__name__]
        dir = os.path.split(mod.__file__)
        files = [os.path.join(dir[0], 'builtin.yaml')]
        # get all yaml files on search path
        for path in os.environ.get('ARNOLD_PLUGIN_PATH', '').split(os.path.pathsep):
            for x in os.listdir(path):
                if not x.startswith('.') and x.endswith('.yaml') and os.path.isfile(x):
                    files.append(x)

        for fpath in files:
            f = open(fpath)
            try:
                _data.update(yaml.load(f))
            except yaml.YAMLError, err:
                print "error parsing yaml file '%s': %s" % (fpath, err)
            finally:
                f.close()
    return _data

def hasParamData(node, parameter, name):
    """
    return True if there is metadata for this parameter, else False 
    """
    _data = _loadData()
    try:
        _data[node]['parameter_metadata'][parameter][name]
        return 1
    except KeyError:
        return 0

def getParamData(node, parameter, name):
    """
    return the requested metadata for this parameter, else None
    """
    _data = _loadData()
    try:
        return _data[node]['parameter_metadata'][parameter][name]
    except KeyError:
        pass

def getAllParamData(node):
    """
    return a copy of the dictionary of metadata for this parameter
    """
    _data = _loadData()
    return _data[node]['parameter_metadata'].copy()

def hasNodeData(node, name):
    """
    return True if there is node-level metadata for this node, else False 
    """
    _data = _loadData()
    try:
        _data[node]['node_metadata'][name]
        return 1
    except KeyError:
        return 0

def getNodeData(node, name):
    """
    return the requested metadata for this node, else None
    """
    _data = _loadData()
    try:
        return _data[node]['node_metadata'][name]
    except KeyError:
        pass

import os

import maya.cmds as cmds

from arnold import AiGetVersion


def get_arnold_bifrost_plugins(path):
    plugins = []
    if os.path.exists(path):
        for p in os.listdir(path):
            if p.startswith("arnold"):
                plugins.append(p)
    return plugins


def get_arnold_bifrost_path():
    arnold_version = '.'.join(AiGetVersion())
    bifrost_path = cmds.getModulePath(moduleName='Bifrost')
    for path in get_arnold_bifrost_plugins(bifrost_path):
        if path.split('arnold-')[1][:3] == arnold_version[:3]:
            return os.path.join(bifrost_path, path)
    return bifrost_path

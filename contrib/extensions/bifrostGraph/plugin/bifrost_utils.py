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
    arnold_version_split = arnold_version.split('.')

    bifrost_path = cmds.getModulePath(moduleName='Bifrost')
    nearest_digit = -1
    nearest_cut = None

    for path in get_arnold_bifrost_plugins(bifrost_path):
        folder_split = path.split('arnold-')[1][:3].split('.')
        
        # the first digit should always be the same
        if folder_split[0] != arnold_version_split[0]:
            continue
        # the current arnold version is too old for this bifrost cut, let's discard it
        if int(folder_split[1]) > int(arnold_version_split[1]):
            continue

        # exact version, use this one
        if folder_split[1] == arnold_version_split[1]:
            return os.path.join(bifrost_path, path)

        # at this point, the second digit is higher in the arnold version 
        # as compated to the bifrost folder. We want to get the nearest one
        if int(folder_split[1]) > int(nearest_digit):
            nearest_digit = folder_split[1]
            nearest_cut = os.path.join(bifrost_path, path)

    if nearest_cut:
        return nearest_cut

    return bifrost_path

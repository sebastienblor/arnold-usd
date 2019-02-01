#!/usr/bin/env python
# -*- coding: utf-8 -*-

import maya.OpenMaya as OpenMaya
import maya.OpenMayaMPx as OpenMayaMPx

class setNode(OpenMayaMPx.MPxObjectSet):

    kPluginNodeName = "setNode"
    kPluginNodeId = OpenMaya.MTypeId(20485)

    def __init__(self):
        OpenMayaMPx.MPxObjectSet.__init__(self)

    @staticmethod
    def creator():
        return OpenMayaMPx.asMPxPtr(setNode())

    @staticmethod
    def initializer():
        pass

def initializePlugin(mobject):

    mplugin = OpenMayaMPx.MFnPlugin(mobject, "Joe", '1.0', "Any")

    try:
        mplugin.registerNode(setNode.kPluginNodeName,
                             setNode.kPluginNodeId,
                             setNode.creator,
                             setNode.initializer,
                             OpenMayaMPx.MPxNode.kObjectSet)
    except:
        raise Exception("Failed to register node: %s" % setNode.kPluginNodeName)

def uninitializePlugin(mobject):
    mplugin = OpenMayaMPx.MFnPlugin(mobject)

    try:
        mplugin.deregisterNode(setNode.kPluginNodeId)
    except:
        raise Exception("Failed to deregister node: %s", setNode.kPluginNodeName)
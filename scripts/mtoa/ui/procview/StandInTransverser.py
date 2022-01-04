import re
import maya.cmds as cmds
import maya.mel as mel
import os.path
import mtoa.melUtils as mu
from mtoa.ui.ae.utils import aeCallback
from mtoa.ui.ae.utils import resolveFilePathSequence
from mtoa.ui.ae.utils import expandEnvVars
import mtoa.core as core
from mtoa.ui.ae.shaderTemplate import ShaderAETemplate
import arnold as ai

from mtoa.ui.qt.Qt import QtCore
from mtoa.ui.qt.Qt import QtGui
from mtoa.ui.qt.Qt import QtWidgets
from mtoa.ui.qt.Qt import shiboken
from mtoa.ui.qt import toQtObject

CACHE_ATTR = 'ai_asscache'

from mtoa.ui.procview.ProceduralTreeView import ProceduralTreeView, ProceduralTreeModel, ProceduralItem
from mtoa.ui.procview.ProceduralWidgets import ProceduralPropertiesPanel
from mtoa.ui.procview.ProceduralTransverser import ProceduralTransverser, \
                           PROC_PATH, PROC_NAME, PROC_PARENT, PROC_VISIBILITY, \
                           PROC_INSTANCEPATH, PROC_ENTRY, PROC_ENTRY_TYPE, PROC_IOBJECT, \
                           PROC_NUM_CHILDREN, \
                           OVERRIDE_OP, DISABLE_OP, \
                           FILE_CACHE, BUILTIN_NODES

from mtoa.callbacks import *

###### UI to create a procedural operator

class StandInTransverser(ProceduralTransverser):
    """ StandIn  Transverser class """
    __instance = None

    def __new__(cls, *args, **kwargs):

        if not cls.__instance:
            instance = super(StandInTransverser, cls).__new__(cls, *args, **kwargs)
            cls.__instance = instance

        return cls.__instance

    def __init__(self):
        self.nodeName = ''
        self.universe = None
        self.nodeEntries = {}
        self.nodeEntryTypes = {}

        super(StandInTransverser, self).__init__()

    def getObjectInfo(self, iObj):
        if iObj == None:
            return ['/', '/', '', 'visible', '', None, None, None, ]

        global FILE_CACHE
        self.nodeName = node
        filename = self.getFileName(node)

        obj = []

        for i, v in enumerate(FILE_CACHE[filename]):
            if v[PROC_IOBJECT] == iObj:
                return v

        # name = iObj
        # nodeEntry = self.nodeEntries[iObj]
        # nodeEntryType = self.nodeEntryTypes[iObj]
        return None

    def getNumNodes(self, filename):

        if not ai.AiUniverseIsActive():
            ai.AiBegin()

        universe = ai.AiUniverse()
        ai.AiASSLoad(universe, filename, ai.AI_NODE_ALL)

        iter = ai.AiUniverseGetNodeIterator(universe, ai.AI_NODE_ALL)
        count = 0
        while not ai.AiNodeIteratorFinished(iter):
            node = ai.AiNodeIteratorGetNext(iter)
            nodeName = ai.AiNodeGetName(node)
            if nodeName not in BUILTIN_NODES:
                count += 1

        ai.AiNodeIteratorDestroy(iter)
        ai.AiUniverseDestroy(universe)

        return count

    def getRootObjectInfo(self, node):
        global FILE_CACHE
        self.nodeName = node
        filename = self.getFileName(node)
        if not filename:
            return None
        elif filename not in FILE_CACHE.keys():
            FILE_CACHE[filename] = []
            FILE_CACHE[filename].append(["/", "/", 'root', 'visible', '', 'procedural', None, 'shape', 1])

        return FILE_CACHE[filename][0]

    def getFileName(self, node):

        filenameAttr = node + '.dso'
        filename = cmds.getAttr(filenameAttr)
        filename = expandEnvVars(filename)

        if ('#' in filename) and cmds.getAttr(node + '.useFrameExtension'):
            frame = cmds.getAttr(node + '.frameNumber')
            extension_len = 0
            padding = 0
            for c in reversed(filename):
                if c == '#':
                    padding += 1
                elif padding > 0:
                    break # I already found numerical characters and they're finished now
                else:
                    # still haven't found a numerical parameter
                    extension_len += 1

            frame_str = str(frame)
            for i in range(1, padding):
                if frame < pow(10, i):
                    frame_str = '0' + frame_str

            filename = filename[:-extension_len - padding] + frame_str + filename[-extension_len:]

        return filename

    def dir(self, iobject):
        if iobject not in [None, '/'] or not cmds.objExists(self.nodeName):
            return []

        global FILE_CACHE
        filename = self.getFileName(self.nodeName)

        self.nodeEntries = {}
        if not filename:
            return []
        
        if not os.path.exists(str(filename)):
            return []
        
        if filename not in FILE_CACHE.keys():
            self.getRootObjectInfo(self.nodeName)

        if len(FILE_CACHE[filename]) == 1:
            numNodes = 0
            if filename:
                numNodes = self.getNumNodes(filename)
            FILE_CACHE[filename][0][PROC_NUM_CHILDREN] = numNodes

            if not ai.AiUniverseIsActive():
                ai.AiBegin()

            universe = ai.AiUniverse()
            ai.AiASSLoad(universe, filename, ai.AI_NODE_ALL)

            iter = ai.AiUniverseGetNodeIterator(universe, ai.AI_NODE_ALL);

            while not ai.AiNodeIteratorFinished(iter):
                node = ai.AiNodeIteratorGetNext(iter)
                nodeName = ai.AiNodeGetName(node)
                if nodeName in BUILTIN_NODES or nodeName == 'options':
                    continue

                nodeEntry = ai.AiNodeGetNodeEntry(node)
                entryName = ai.AiNodeEntryGetName(nodeEntry)

                name = ai.AiNodeGetName(node)
                nentry = ai.AiNodeGetNodeEntry(node)
                nodeEntry = ai.AiNodeEntryGetName(nentry)
                nodeEntryType = ai.AiNodeEntryGetTypeName(nentry)
                if nodeEntryType == 'shape':
                    derivedType = ai.AiNodeEntryGetDerivedType(nentry)
                    if derivedType == ai.AI_NODE_SHAPE_PROCEDURAL:
                        nodeEntryType = 'procedural'
                    elif derivedType == ai.AI_NODE_SHAPE_VOLUME:
                        nodeEntryType = 'volume'

                FILE_CACHE[filename].append([name, name, '/', 'visible', name, nodeEntry, name, nodeEntryType, 0])

            ai.AiNodeIteratorDestroy(iter)
            ai.AiUniverseDestroy(universe)

        return FILE_CACHE[filename][1:]
        # but we should consider nested procedurals....


################################################

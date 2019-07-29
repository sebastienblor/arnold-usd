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
                           OVERRIDE_OP, DISABLE_OP

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
            return ['/', '/', '', 'visible', '', '/', None, None]

        name = iObj
        nodeEntry = self.nodeEntries[iObj]
        nodeEntryType = self.nodeEntryTypes[iObj]
        return [name, name, '', 'visible', name, nodeEntry, name, nodeEntryType]

    def getRootObjectInfo(self, node):
        self.nodeName = node
        return ["/", "/", '', 'visible', '', '/', None, None]

    def dir(self, iobject):
        if iobject != None:
            return []

        filenameAttr = self.nodeName + '.dso'
        filename = cmds.getAttr(filenameAttr)
        filename = expandEnvVars(filename)

        if ('#' in filename) and cmds.getAttr(self.nodeName + '.useFrameExtension'):
            frame = cmds.getAttr(self.nodeName + '.frameNumber')
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

        self.nodeEntries = {} 
        if not os.path.exists(str(filename)):
            return

        ass_nodes = []

        universeCreated = False
        if not ai.AiUniverseIsActive():
            universeCreated = True
            ai.AiBegin()

        universe = ai.AiUniverse()
        ai.AiASSLoad(universe, filename, ai.AI_NODE_ALL)

        iter = ai.AiUniverseGetNodeIterator(universe, ai.AI_NODE_ALL);

        while not ai.AiNodeIteratorFinished(iter):
            node = ai.AiNodeIteratorGetNext(iter)
            nodeName = ai.AiNodeGetName(node)
            if nodeName == 'root' or nodeName == 'ai_default_reflection_shader' or nodeName == 'options':
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
                    
            ass_nodes.append([name, name, '', 'visible', name, nodeEntry, name, nodeEntryType])
            self.nodeEntries[name] = nodeEntry
            self.nodeEntryTypes[name] = nodeEntryType

        ai.AiNodeIteratorDestroy(iter)
        ai.AiUniverseDestroy(universe)

        if universeCreated:
            ai.AiEnd()
        return ass_nodes
        # but we should consider nested procedurals....


################################################

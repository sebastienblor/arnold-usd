import re
import maya.cmds as cmds
import maya.mel as mel
import os.path
import mtoa.melUtils as mu
from mtoa.ui.ae.utils import aeCallback
from mtoa.ui.ae.utils import resolveFilePathSequence
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
                           PROC_INSTANCEPATH, PROC_ENTRY_TYPE, PROC_IOBJECT, \
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
        self.cache = {}

        super(StandInTransverser, self).__init__()


    def getObjectInfo(self, iObj):
        if iObj == None:
            return ['root', 'root', '', 'visible', '', 'root', None]
        
        name = iObj
        entryName = self.cache[iObj]
        return [name, name, '', 'visible', name, entryName, name]

    def getRootObjectInfo(self, node):
        self.nodeName = node
        return ["root", "root", '', 'visible', '', 'root', None]
        
    def dir(self, iobject):
        if iobject != None:
            return []

        filenameAttr = self.nodeName + '.dso'
        filename = cmds.getAttr(filenameAttr)
        self.cache = {} # for now we're just storing the entry_type, but we might want to store more data 
        if not os.path.exists(filename):
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
            entryType = ai.AiNodeEntryGetName(ai.AiNodeGetNodeEntry(node))
            ass_nodes.append([name, name, '', 'visible', name, entryType, name])
            self.cache[name] = entryType

        ai.AiNodeIteratorDestroy(iter)
        ai.AiUniverseDestroy(universe)

        if universeCreated:
            ai.AiEnd()
        return ass_nodes
        # but we should consider nested procedurals....

    
################################################

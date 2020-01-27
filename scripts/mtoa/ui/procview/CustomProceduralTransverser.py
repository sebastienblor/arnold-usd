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
                           PROC_INSTANCEPATH, PROC_ENTRY, PROC_ENTRY_TYPE, PROC_IOBJECT, \
                           PROC_NUM_CHILDREN, \
                           OVERRIDE_OP, DISABLE_OP

from mtoa.callbacks import *

class CustomProcTreeItem():
    def __init__(self, data):
        self.children = [] # array of CustomProcItemTree children indices
        self.data = data  # tranverser data for this item

###### UI to create a procedural operator

class CustomProceduralTransverser(ProceduralTransverser):
    """ StandIn  Transverser class """
    __instance = None
    
    def __new__(cls, *args, **kwargs):

        if not cls.__instance:
            instance = super(CustomProceduralTransverser, cls).__new__(cls, *args, **kwargs)
            cls.__instance = instance

        return cls.__instance

    def __init__(self, procedural, filenameAttr, filename):
        self.nodeName = ''
        self.procedural = procedural
        self.proceduralFilename = filename
        self.proceduralFilenameAttr = filenameAttr
        self.items = []
        super(CustomProceduralTransverser, self).__init__()


    def getObjectInfo(self, iObj):
        # iObj is the index in  self.items
        return self.items[iObj].data


    def buildTree(self, parentIndex, nameSplit, nameSplitIndex, entry=None, entryType=None):
        if nameSplitIndex >= len(nameSplit):
            return

        name = nameSplit[nameSplitIndex] # name of the current item at this level
        childIndex = -1
        # search if one of the parent's child items has this name
        for child in self.items[parentIndex].children:
            if name == self.items[child].data[PROC_NAME]:
                childIndex = child
                break

        if childIndex < 0:
            # didn't find an existing entry
            childIndex = len(self.items) # this will be my child index
            parentPath = self.items[parentIndex].data[PROC_PATH]
            if len(parentPath) or nameSplitIndex > 0:
                path = '{}/{}'.format(parentPath, name)
            else:
                path = name
            
            if path != '/'.join(nameSplit):
                this_entry = 'xform'
                this_entryType = None
            else:
                this_entry = entry
                this_entryType = entryType

            node = ai.AiNodeLookUpByName(path)
            if node:
                entry = ai.AiNodeEntryGetName(ai.AiNodeGetNodeEntry(node))
                entryType = ai.AiNodeEntryGetTypeName(ai.AiNodeGetNodeEntry(node))
            self.items[parentIndex].children.append(childIndex)
            if PROC_NUM_CHILDREN >= len(self.items[parentIndex].data):
                self.items[parentIndex].data.append(0)
            self.items[parentIndex][PROC_NUM_CHILDREN] += 1
            self.items.append(CustomProcTreeItem([path, name, parentPath, 'visible', path, this_entry, childIndex, this_entryType, 0]))
            
        # now call buildTree recursively
        self.buildTree(childIndex, nameSplit, nameSplitIndex+1, entry, entryType)
        return 


    def getRootObjectInfo(self, node):
        if ai.AiUniverseIsActive():
            cmds.error("Cannot populate procedurals while a render is in progress")
            return
        self.nodeName = node
        ai.AiBegin(ai.AI_SESSION_INTERACTIVE)
        self.items.append(CustomProcTreeItem(['', '', '', 'visible', '', '', 0, None]))
        proc = ai.AiNode(self.procedural)
        ai.AiNodeSetStr(proc, self.proceduralFilenameAttr, self.proceduralFilename)
        ai.AiRender(ai.AI_RENDER_MODE_FREE)
        iter = ai.AiUniverseGetNodeIterator(None, ai.AI_NODE_ALL);
        
        while not ai.AiNodeIteratorFinished(iter):
            node = ai.AiNodeIteratorGetNext(iter)
            if node == proc:
                continue

            nodeName = ai.AiNodeGetName(node)
            if nodeName == 'root' or nodeName == 'ai_default_reflection_shader' or nodeName == 'options' or nodeName == 'ai_bad_shader':
                continue

            entryName = ai.AiNodeEntryGetName(ai.AiNodeGetNodeEntry(node))
            entryType = ai.AiNodeEntryGetTypeName(ai.AiNodeGetNodeEntry(node))
            nameSplit = nodeName.split('/')
            startIndex = 0
            if len(nameSplit[0]) == 0:
                startIndex = 1
            self.buildTree(0, nameSplit, startIndex, entryName, entryType)

        ai.AiNodeIteratorDestroy(iter)

        ai.AiRenderAbort()
        ai.AiEnd()
        return self.items[0].data
        
    def dir(self, iobject):
        if not iobject:
            iobject = 0
        children = []
        if iobject == 'NULL':
            return children
        if iobject >= len(self.items):
            return []
        for ich in self.items[iobject].children:
            children.append(self.getObjectInfo(ich))

        return children





    
################################################

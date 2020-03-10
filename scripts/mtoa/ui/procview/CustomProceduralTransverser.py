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
                           OVERRIDE_OP, DISABLE_OP, \
                           BUILTIN_NODES, FILE_CACHE

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
            instance = super(CustomProceduralTransverser, cls).__new__(cls)
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
        global FILE_CACHE
        if nameSplitIndex >= len(nameSplit):
            return

        name = nameSplit[nameSplitIndex] # name of the current item at this level
        childIndex = -1
        # search if one of the parent's child items has this name
        for child in FILE_CACHE[self.proceduralFilename][parentIndex]['children']:
            if name == FILE_CACHE[self.proceduralFilename][child]['data'][PROC_NAME]:
                childIndex = child
                break

        if childIndex < 0:
            # didn't find an existing entry
            childIndex = len(FILE_CACHE[self.proceduralFilename])  # this will be my child index
            parentPath = FILE_CACHE[self.proceduralFilename][parentIndex]['data'][PROC_PATH]
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
            FILE_CACHE[self.proceduralFilename][parentIndex]['children'].append(childIndex)
            if PROC_NUM_CHILDREN >= len(FILE_CACHE[self.proceduralFilename][parentIndex]['data']):
                FILE_CACHE[self.proceduralFilename][parentIndex]['data'].append(0)
            FILE_CACHE[self.proceduralFilename][parentIndex]['data'][PROC_NUM_CHILDREN] += 1
            FILE_CACHE[self.proceduralFilename].append({'children': [],
                                                        'data': [path, name, parentPath, 'visible', path, this_entry, childIndex, this_entryType, 0]})

        # now call buildTree recursively
        self.buildTree(childIndex, nameSplit, nameSplitIndex+1, entry, entryType)
        return 


    def getRootObjectInfo(self, node):
        global FILE_CACHE
        if ai.AiUniverseIsActive():
            cmds.error("Cannot populate procedurals while a render is in progress")
            return
        self.nodeName = node

        if self.proceduralFilename not in FILE_CACHE.keys():
            FILE_CACHE[self.proceduralFilename] = []
            FILE_CACHE[self.proceduralFilename].append({'children': [],
                                                        'data': ['', '', '', 'visible', '', '', 0, None]})
            beginSession = (not ai.AiUniverseIsActive())
            if beginSession:
                ai.AiBegin(ai.AI_SESSION_INTERACTIVE)

            universe = ai.AiUniverse()
            proc = ai.AiNode(universe, 'usd')
            ai.AiNodeSetStr(proc, self.proceduralFilenameAttr, self.proceduralFilename)
            paramMap = ai.AiParamValueMap()
            ai.AiParamValueMapSetBool(paramMap, 'list', True)
            ai.AiProceduralViewport(proc, universe, ai.AI_PROC_BOXES, paramMap)
            iter = ai.AiUniverseGetNodeIterator(universe, ai.AI_NODE_ALL)

            while not ai.AiNodeIteratorFinished(iter):
                node = ai.AiNodeIteratorGetNext(iter)
                if node == proc:
                    continue

                nodeName = ai.AiNodeGetName(node)
                if nodeName in BUILTIN_NODES or nodeName in ['', 'options']:
                    continue

                entryName = ai.AiNodeEntryGetName(ai.AiNodeGetNodeEntry(node))
                entryType = ai.AiNodeEntryGetTypeName(ai.AiNodeGetNodeEntry(node))
                nameSplit = nodeName.split('/')
                startIndex = 0
                if len(nameSplit[0]) == 0:
                    startIndex = 1
                self.buildTree(0, nameSplit, startIndex, entryName, entryType)

            ai.AiParamValueMapDestroy(paramMap)
            ai.AiNodeIteratorDestroy(iter)
            ai.AiUniverseDestroy(universe)
            if beginSession:
                ai.AiEnd()

        # populate the items
        for item in FILE_CACHE[self.proceduralFilename]:
            usd_item = CustomProcTreeItem(item['data'])
            usd_item.children = item['children']
            self.items.append(usd_item)

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

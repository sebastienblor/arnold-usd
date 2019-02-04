import os
import os.path

from mtoa.ui.qt.Qt import QtCore
from mtoa.ui.qt.Qt import QtGui
from mtoa.ui.qt.Qt import QtWidgets
from mtoa.ui.qt.Qt import shiboken
from mtoa.ui.qt import toQtObject

from mtoa.ui.procview.ProceduralTreeView import ProceduralTreeView, ProceduralTreeModel, ProceduralItem
from mtoa.ui.procview.ProceduralWidgets import ProceduralPropertiesPanel
from mtoa.ui.procview.ProceduralTransverser import ProceduralTransverser, \
                           PROC_PATH, PROC_NAME, PROC_PARENT, PROC_VISIBILITY, \
                           PROC_INSTANCEPATH, PROC_ENTRY_TYPE, PROC_IOBJECT, \
                           OVERRIDE_OP, DISABLE_OP


import maya.cmds as cmds
from alembic import Abc, AbcGeom

from alembic.AbcCoreAbstract import *
from alembic.Abc import *
from alembic.Util import *
from alembic.AbcGeom import *

from arnold import *

NODE_TYPES = ['polymesh', 'curves', 'nurbs', 'points']
TYPE_MATCH = {IPolyMesh: "polymesh", ISubD: "polymesh", IXform: "xform",
              INuPatch: "nurbs", ICurves: "curves", IPoints: "points",
              ICamera: "persp_camera"}

BLACK_LIST_PARAMS = ['id', 'visibility', 'name', 'sidedness', 'matrix',
                     'motion_start', 'motion_end', 'shader', 'disp_map',
                     'vidxs', 'vlist', 'nsides', 'uvidxs', 'shidxs',
                     'nlist', 'uvlist', 'crease_idxs', 'crease_sharpness',
                     'use_shadow_group', 'use_light_group', 'degree_u',
                     'degree_v', 'transform_type']

CACHE_ATTR = 'ai_abccache'

VISIBILITY = ['differed', 'hidden', 'visible']


def ArnoldUniverseOnlyBegin():
    if not AiUniverseIsActive():
        AiBegin()
        AiMsgSetConsoleFlags(AI_LOG_NONE)
        return True
    return False


def ArnoldUniverseEnd():
    if AiUniverseIsActive():
        if AiRendering():
            AiRenderInterrupt()
        if AiRendering():
            AiRenderAbort()
        AiEnd()


def ArnoldGetArrayType(nodeType, paramName):
    AiUniverseCreated = ArnoldUniverseOnlyBegin()
    node_entry = AiNodeEntryLookUp(nodeType)
    param_entry = AiNodeEntryLookUpParameter(node_entry, paramName)
    param_type = AiParamGetType(param_entry)
    if param_type == AI_TYPE_ARRAY:
        array_default = AiParamGetDefault(param).contents.ARRAY
        param_type = AiArrayGetType(array_default)
    if AiUniverseCreated:
        ArnoldUniverseEnd()

    return param_type


def abcToArnType(iObj):
    if not iObj:
        return

    md = iObj.getMetaData()
    if IPolyMesh.matches(md) or ISubD.matches(md):
        return 'polymesh'
    elif IPoints.matches(md):
        return 'points'
    elif ICurves.matches(md):
        return 'curves'
    else:
        return None


def getVisibilityValue(vis_flags=[True]*8):

    vis = AI_RAY_ALL

    if not vis_flags[0]:
        vis &= ~AI_RAY_CAMERA
    if not vis_flags[1]:
        vis &= ~AI_RAY_SHADOW
    if not vis_flags[2]:
        vis &= ~AI_RAY_DIFFUSE_TRANSMIT
    if not vis_flags[3]:
        vis &= ~AI_RAY_SPECULAR_TRANSMIT
    if not vis_flags[4]:
        vis &= ~AI_RAY_VOLUME
    if not vis_flags[5]:
        vis &= ~AI_RAY_DIFFUSE_REFLECT
    if not vis_flags[6]:
        vis &= ~AI_RAY_SPECULAR_REFLECT
    if not vis_flags[7]:
        vis &= ~AI_RAY_SUBSURFACE

    return vis


VISIBILITY = ['differed', 'hidden', 'visible']

# This part belongs in the ABC code
def abcToArnType(iObj):
    if not iObj:
        return

    md = iObj.getMetaData()
    if AbcGeom.IPolyMesh.matches(md) or AbcGeom.ISubD.matches(md):
        return 'polymesh'
    elif AbcGeom.IPoints.matches(md):
        return 'points'
    elif AbcGeom.ICurves.matches(md):
        return 'curves'
    elif AbcGeom.IXform.matches(md):
        return 'xform'
    else:
        return None


class AlembicTransverser(ProceduralTransverser):
    """ Alembic Transverser class """
    __instance = None

    def __new__(cls, *args, **kwargs):
        if not cls.__instance:
            instance = super(AlembicTransverser, cls).__new__(cls, *args, **kwargs)
            cls.__instance = instance

        return cls.__instance

    def __init__(self):
        super(AlembicTransverser, self).__init__()
        self.filenameAttr = "filename"

    
    def getArchivePath(self, node):
        return os.path.abspath(cmds.getAttr("{}.{}".format(node, self.filenameAttr)))

    def getArchive(self, node):
        abc_file = self.getArchivePath(node)
        iarch = Abc.IArchive(str(abc_file))
        return iarch

    def visitObject(self, iObj, parent="", visibility="visible"):
        abc_items = []
        obj_data = self.getObjectInfo(iObj)
        abc_items.append(obj_data)
        for child in iObj.children:
            abc_items += self.visitObject(child, obj_data[PROC_PATH], obj_data[PROC_VISIBILITY])

        return abc_items

    def getObjectInfo(self, iobject):
        path = iobject.getFullName()
        name = iobject.getName()
        parent = iobject.getParent().getFullName()
        instancedPath = iobject.instanceSourcePath()
        entity_type = abcToArnType(iobject)
        visibility = VISIBILITY[int(AbcGeom.GetVisibility(iobject))+1]
        return [path, name, parent, visibility, instancedPath, entity_type, iobject]

    def getRootObjectInfo(self, node):
        abc_file = self.getArchive(node) 
        return self.getObjectInfo(abc_file.getTop())
        
    def dir(self, iobject):
        children = []
        for ich in iobject.children:
            children.append(self.getObjectInfo(ich))

        return children

    def getNodeTypes(self, iObj):
        node_types = []
        children = self.visitObject(iObj)

        for child in children:
            if child[PROC_ENTRY_TYPE] not in node_types:
                node_types.append(child[PROC_ENTRY_TYPE])

        return node_types

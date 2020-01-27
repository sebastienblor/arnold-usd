import os
import os.path

from mtoa.ui.ae.utils import expandEnvVars
from mtoa.ui.qt.Qt import QtCore
from mtoa.ui.qt.Qt import QtGui
from mtoa.ui.qt.Qt import QtWidgets
from mtoa.ui.qt.Qt import shiboken
from mtoa.ui.qt import toQtObject

from mtoa.ui.procview.ProceduralTreeView import ProceduralTreeView, ProceduralTreeModel, ProceduralItem
from mtoa.ui.procview.ProceduralWidgets import ProceduralPropertiesPanel
from mtoa.ui.procview.ProceduralTransverser import ProceduralTransverser, \
                           PROC_PATH, PROC_NAME, PROC_PARENT, PROC_VISIBILITY, \
                           PROC_INSTANCEPATH, PROC_ENTRY, PROC_ENTRY_TYPE, PROC_IOBJECT, \
                           OVERRIDE_OP, DISABLE_OP


import maya.cmds as cmds
from alembic import Abc, AbcGeom

from alembic.AbcCoreAbstract import *
from alembic.Abc import *
from alembic.Util import *
from alembic.AbcGeom import *

from arnold import *

TYPE_MATCH = {IPolyMesh: "polymesh", ISubD: "polymesh", IXform: "xform",
              INuPatch: "nurbs", ICurves: "curves", IPoints: "points",
              ICamera: "persp_camera"}

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
        return 'alembic'


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
        self.layersAttr = "abc_layers"

    def getArchivePath(self, path):
        old_cwd = os.getcwd()
        projectRootDir = cmds.workspace(query=True, rootDirectory=True)
        os.chdir(projectRootDir)
        filename = expandEnvVars(path)
        if len(filename) == 0:
            return ''
        filename = os.path.abspath(filename)
        os.chdir(old_cwd)
        return filename

    def getArchive(self, node):
        projectRootDir = cmds.workspace(query=True, rootDirectory=True)

        filename = cmds.getAttr("{}.{}".format(node, self.filenameAttr))
        abc_file = self.getArchivePath(filename) or ''
        if len(abc_file) == 0:
            return None
        abc_files = [str(abc_file)]
        if cmds.attributeQuery(self.layersAttr, node=node, exists=True):
            layers = cmds.getAttr('.'.join([node, self.layersAttr])) or ''
            for abc in layers.split(';') or []:
                abc_path = self.getArchivePath(abc)
                if not os.path.isfile(abc_path):
                    abc_path = os.path.join(projectRootDir, abc_path)

                if os.path.isfile(abc_path):
                    abc_files.append(str(abc_path))

        iarch = Abc.IArchive(abc_files)
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
        nodeEntry = abcToArnType(iobject)
        visibility = VISIBILITY[int(AbcGeom.GetVisibility(iobject))+1]
        nodeEntryType = 'shape' if nodeEntry in ['points', 'polymesh', 'curves'] else None
        numchildren = iobject.getNumChildren()
        return [path, name, parent, visibility, instancedPath, nodeEntry, iobject, nodeEntryType, numchildren]

    def getRootObjectInfo(self, node):
        abc_file = self.getArchive(node)
        if not abc_file:
            return None
        return self.getObjectInfo(abc_file.getTop())

    def dir(self, iobject):
        children = []
        if iobject == 'NULL':
            return children

        for ich in iobject.children:
            children.append(self.getObjectInfo(ich))

        return children

    def getNodeTypes(self, iObj):
        node_types = []
        children = self.visitObject(iObj)

        for child in children:
            if child[PROC_ENTRY] not in node_types:
                node_types.append(child[PROC_ENTRY])

        return node_types

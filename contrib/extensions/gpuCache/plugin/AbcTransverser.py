import os
import maya.cmds as cmds
from mtoa.ui.qt import BaseTransverser
from alembic import Abc, AbcGeom

(ABC_PATH,
 ABC_NAME,
 ABC_PARENT,
 ABC_VISIBILITY,
 ABC_INSTANCEPATH,
 ABC_ENTIY_TYPE,
 ABC_IOBJECT) = range(7)

VISIBILITY = ['differed', 'hidden', 'visible']


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


class AlembicTransverser(BaseTransverser):
    """ Alembic Transverser class """

    __instance = None

    def __new__(cls, *args, **kwargs):

        if not cls.__instance:
            instance = super(AlembicTransverser, cls).__new__(cls, *args, **kwargs)

            instance.impl = AlembicTransverserImpl()

            cls.__instance = instance

        return cls.__instance

    def __init__(self):
        super(AlembicTransverser, self).__init__(self)

    def getArchive(self, abc_file):

        return self.impl.getArchive(abc_file)

    def visitObject(self, iObj, parent="", visibility="visible"):

        return self.impl.visitObject(iObj)

    def dir(self, *args):

        return self.impl.dir(*args)

    # def properties(self, node, path):

    #     return self.impl.properties(node, path)

    # def createOperator(self, node, path):

    #     return self.impl.createOperator(node, path)

    # def getOperator(self, node, path):

    #     return self.impl.getOperator(node, path)

    # def setOverride(self, node, path, param, value):

    #     return self.impl.setOverride(node, path)

    # def deleteOverride(self, node, path, param):

    #     return self.impl.deleteOverride(node, path)


class AlembicTransverserImpl(object):

    def getArchivePath(self, node):

        return os.path.abspath(cmds.getAttr("{}.cacheFileName".format(node)))

    def getArchive(self, node):

        abc_file = self.getArchivePath(node)
        iarch = Abc.IArchive(str(abc_file))
        return iarch

    def visitObject(self, iObj, parent="", visibility="visible"):

        abc_items = []
        path = iObj.getFullName()
        name = iObj.getName()
        instancedPath = iObj.instanceSourcePath()

        entity_type = abcToArnType(iObj)

        if visibility != VISIBILITY[1]:
            visibility = VISIBILITY[int(AbcGeom.GetVisibility(iObj))+1]

        abc_items.append(getObjectInfo(iObj))

        for child in iObj.children:
            abc_items += self.visitObject(child, path, visibility)

        return abc_items

    def getObjectInfo(self, iobject):
        path = iobject.getFullName()
        name = iobject.getName()
        parent = iobject.getParent().getFullName()
        instancedPath = iobject.instanceSourcePath()

        entity_type = abcToArnType(iobject)

        visibility = VISIBILITY[int(AbcGeom.GetVisibility(iobject))+1]

        return [path, name, parent, visibility, instancedPath, entity_type, iobject]

    def dir(self, iobject):

        children = []
        for ich in iobject.children:
            children.append(self.getObjectInfo(ich))

        return children

    def properties(self, node, path):

        pass

    def createOperator(self, node, path):

        pass

    def getOperator(self, node, path):

        pass

    def setOverride(self, node, path, ):

        pass

    def deleteOverride(self, node, path, ):

        pass
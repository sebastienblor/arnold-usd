import os
import re
import maya.cmds as cmds
import mtoa.melUtils as mu
from mtoa.ui.qt import BaseTransverser
from alembic import Abc, AbcGeom
from arnold import AI_TYPE_ENUM, AI_TYPE_STRING, AI_TYPE_POINTER, AI_TYPE_NODE

(ABC_PATH,
 ABC_NAME,
 ABC_PARENT,
 ABC_VISIBILITY,
 ABC_INSTANCEPATH,
 ABC_ENTIY_TYPE,
 ABC_IOBJECT) = range(7)

VISIBILITY = ['differed', 'hidden', 'visible']

SELECTION_REGEX = re.compile(r"([/\w]+)/*\**")

EXP_REGEX = re.compile(r"""(?P<param>\w+)\s* # parameter
                         (?P<op>=|\+=|-=|\*=)\s* # operation
                         (?P<value>.*\w) # value
                         """, re.X)


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

    def getObjectInfo(self, iObj):

        return self.impl.getObjectInfo(iObj)

    def dir(self, *args):

        return self.impl.dir(*args)

    # def properties(self, node, path):

    #     return self.impl.properties(node, path)

    def createOperator(self, node, iobject):

        return self.impl.createOperator(node, iobject)

    def getOperator(self, node, path):

        return self.impl.getOperator(node, path)

    def getOverrides(self, node, path):

        return self.impl.getOverrides(node, path)

    def setOverride(self, node, path, param, value):

        return self.impl.setOverride(node, path, param, value)

    def deleteOverride(self, node, path, param):

        return self.impl.deleteOverride(node, path, param)


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

    def createOperator(self, node, iobject):
        num_ops = mu.getAttrNumElements(node, 'operators')
        op_name = '{}_setParam{}'.format(node, num_ops)
        op = cmds.createNode("aiSetParameter", name=op_name, ss=True)
        path = iobject[ABC_PATH]
        if iobject[ABC_ENTIY_TYPE] == "xform":
            path += "/*"
        elif iobject[ABC_ENTIY_TYPE] == None:
            path += "*"
        cmds.setAttr(op + ".selection",
                     path,
                     type="string")

        newItem = '{}.operators[{}]'.format(node, num_ops)
        cmds.connectAttr(op + ".out", newItem)
        return op

    def getOperator(self, node, path):

        def walkInputs(op, path, plug):

            selection_exp = '{}*'.format(path)

            r_ipt = r_plug = None
            if cmds.attributeQuery('selection', node=op, exists=True):
                sel_exp = cmds.getAttr('{}.selection'.format(op))
                tokens = sel_exp.rsplit()
                for tok in tokens:
                    mat = SELECTION_REGEX.match(tok)
                    if mat and mat.group(0) == path:
                        return op, plug

            if cmds.attributeQuery('inputs', node=op, exists=True):
                inputs_raw = cmds.listConnections("{}.inputs".format(op), c=True) or []
                it = iter(inputs_raw)
                inputs = zip(it, it)
                for plug, ipt in inputs:
                    r_ipt, r_plug = walkInputs(ipt, path, plug)

            return r_ipt, r_plug

        operator = None
        operators = cmds.listConnections('{}.operators'.format(node)) or []
        for idx, op in enumerate(operators):
            out_op, plug = walkInputs(op, path, '{}.operators[{}]'.format(node, idx))
            if out_op:
                operator = out_op
                break

        return operator

    def getOverrides(self, node, path):

        op = self.getOperator(node, path)
        print op

        overrides = []
        if op:
            for c in cmds.getAttr('{}.assignment'.format(op), multiIndices=True) or []:
                ass_str = cmds.getAttr("{}.assignment[{}]".format(op, c))
                overrides.append(ass_str)

        return overrides

    def setOverride(self, node, path, param, value):

        op = self.getOperator(node, path)
        if op:
            n_conn = mu.getAttrNumElements(op, "assignment")
            c_idx = n_conn
            if param != 'NEWOVERRIDE':
                for c in cmds.getAttr('{}.assignment'.format(op), multiIndices=True) or []:
                    ass_str = cmds.getAttr("{}.assignment[{}]".format(op, c))
                    if ass_str.startswith(param):
                        c_idx = c
                        break
            if param_type in [AI_TYPE_ENUM, AI_TYPE_STRING, AI_TYPE_POINTER, AI_TYPE_NODE]:
                value = "'{}'".format(value)
            param_exp = "{}={}".format(param, value)
            cmds.setAttr("{}.assignment[{}]".format(op, c_idx),
                         param_exp,
                         type="string")
            return True

        return False

    def deleteOverride(self, node, path, index):

        op = self.getOperator(node, path)
        # index = -1
        # for c in cmds.getAttr('{}.assignment'.format(op), multiIndices=True) or []:
        #     ass_str = cmds.getAttr("{}.assignment[{}]".format(op, c))
        #     if ass_str.startswith(param):
        #         index = c
        #         break
        if index != -1 and op:
            print cmds.getAttr('{}.assignment[{}]'.format(op, index))
            status = cmds.removeMultiInstance('{}.assignment[{}]'.format(op, index))
            return status
        return False

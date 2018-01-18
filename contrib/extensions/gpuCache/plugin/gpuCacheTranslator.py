import mtoa.ui.ae.templates as templates
from mtoa.ui.ae.templates import AttributeTemplate, registerTranslatorUI
import maya.cmds as cmds
import os
import os.path

from alembic.AbcCoreAbstract import *
from alembic.Abc import *
from alembic.Util import *

class gpuCacheDescriptionTemplate(templates.ShapeTranslatorTemplate):

    def selectGeomPath(self):
        selectedGeomPath = cmds.textScrollList(self.abcInfoPath, q=True, si=True)
        if selectedGeomPath:
            selectedGeomPath = selectedGeomPath[0]
        for key in self.abcItems:
            if self.abcItems[key] == selectedGeomPath:
                cmds.setAttr('{}.cacheGeomPath'.format(self.nodeName), key, type='string')

    def abcInfoNew(self, nodeName) :
        cmds.rowLayout(nc=2)
        cmds.text(label='')
        self.inspectAlembicPath = cmds.button(align="center", label='Inspect Alembic File', command=lambda *args: self.inspectAlembic())
        cmds.setParent('..') # rowLayout
        self.abcInfoPath = cmds.textScrollList(height=300,allowMultiSelection=False, doubleClickCommand=self.selectGeomPath)
        self.abcInfoReplace(nodeName)

        fileAttr = self.nodeName + ".cacheFileName"
        cmds.scriptJob(attributeChange=[fileAttr,self.updateAlembicFile],
                     replacePrevious=True, parent=self.inspectAlembicPath)

        
    def updateAlembicFile(self):
        cmds.textScrollList(self.abcInfoPath, edit=True, visible=False)
        cmds.button(self.inspectAlembicPath, edit=True, visible=True)
        cmds.textScrollList(self.abcInfoPath, edit=True, removeAll=True)

    def visitObject(self, iObj, iIndent = 0 ):

        path = str(iObj)
        indent = "-" * iIndent
        
        strLine = "%s%s" % ( indent,  path )
        cmds.textScrollList(self.abcInfoPath, edit=True, append=str(strLine))
        geomPath = path.replace('/', '|')
        self.abcItems[geomPath] = strLine

        iIndent += 2
        for child in iObj.children:
            self.visitObject( child, iIndent )

    def inspectAlembic(self):
        filenameAttr = self.nodeName + '.cacheFileName'
        filename = cmds.getAttr(filenameAttr)
        cmds.textScrollList(self.abcInfoPath, edit=True, removeAll=True)
        cmds.textScrollList(self.abcInfoPath, edit=True, visible=True)
        cmds.button(self.inspectAlembicPath, edit=True, visible=False)
        if not os.path.exists(filename):
            return

        iArchive = IArchive(str(filename))
        self.visitObject( iArchive.getTop() )

        allItems = cmds.textScrollList(self.abcInfoPath, query=True, allItems=True) or []
        
        geomPathAttr = self.nodeName + '.cacheGeomPath'
        geomPath = cmds.getAttr(geomPathAttr)
        
        if geomPath in self.abcItems:
            cmds.textScrollList(self.abcInfoPath, edit=True, selectItem=self.abcItems[geomPath])


    def abcInfoReplace(self, nodeName) :
        self.abcItems = {}
        cmds.textScrollList(self.abcInfoPath, edit=True, visible=False)
        cmds.button(self.inspectAlembicPath, edit=True, visible=True)
        cmds.textScrollList(self.abcInfoPath, edit=True, removeAll=True)

    def setup(self):
        self.abcInfoPath = ''
        self.inspectAlembicPath = ''
        self.abcItems = {}

        self.commonShapeAttributes()
        self.addSeparator()
        self.addControl("aiPullUserParams", label="Pull User Params")
        self.addControl("aiUserOptions", label="User Options")
        self.beginLayout("Alembic Contents", collapse=False)
        self.addCustom('aiInfo', self.abcInfoNew, self.abcInfoReplace)
        self.endLayout()
        
templates.registerTranslatorUI(gpuCacheDescriptionTemplate, "gpuCache", "gpuCacheTranslator")

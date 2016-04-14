import maya.cmds as cmds
import maya.mel as mel
import mtoa.utils as utils
from mtoa.ui.nodeTreeLister import aiHyperShadeCreateMenu_BuildMenu, createArnoldNodesTreeLister_Content
import mtoa.ui.ae.templates as templates
import ctypes
import types

global arnoldAOVCallbacks

try:
    import maya.app.renderSetup.model.rendererCallbacks as rendererCallbacks
    import maya.app.renderSetup.model.selector as selector
    import maya.app.renderSetup.model.utils as renderSetupUtils
    from mtoa.aovs import AOVInterface
    import mtoa.ui.aoveditor as aoveditor
    import mtoa.callbacks as callbacks
    import json

    class ArnoldAOVCallbacks(rendererCallbacks.AOVCallbacks):
        DEFAULT_ARNOLD_DRIVER_NAME = "defaultArnoldDriver"
        DEFAULT_ARNOLD_FILTER_NAME = "defaultArnoldFilter"
    
        # The Arnold AOV nodes only exist if the unifiedRenderGlobalsWindow has been opened.
        def _createUnifiedRenderGlobalsWindowIfNeeded(self):
            if not cmds.window("unifiedRenderGlobalsWindow", exists=True):
                mel.eval("unifiedRenderGlobalsWindow()")
                cmds.window("unifiedRenderGlobalsWindow", edit=True, visible=False)
    
        def encode(self):
            self._createUnifiedRenderGlobalsWindowIfNeeded()
            aovsJSON = {}
            basicNodeExporter = rendererCallbacks.BasicNodeExporter()

            aovNodes = AOVInterface().getAOVNodes()
            outputs = []
            aovs = []
            drivers = []
            filters = []
            uniqueDrivers = set([])
            uniqueFilters = set([])

            # Iterate over our aovNodes (aiAOV nodes)
            for aovNode in aovNodes:
                aovNodeName = aovNode.name()
                numOutputs = cmds.getAttr(aovNodeName + ".outputs", size=True)
                currentOutputs = []
                
                # Iterate over our output connections which have driver and filter connections
                for outputNum in range(0, numOutputs):
                    driver = cmds.listConnections(aovNodeName + ".outputs[" + str(outputNum) + "].driver")[0]
                    filter = cmds.listConnections(aovNodeName + ".outputs[" + str(outputNum) + "].filter")[0]

                    # If we don't have this driver yet, add it to the list
                    if not driver in uniqueDrivers:
                        uniqueDrivers.add(driver)
                        basicNodeExporter.setNodes([driver])
                        drivers.append({driver : basicNodeExporter.encode()})

                    # If we don't have this filter yet, add it to the list
                    if not filter in uniqueFilters:
                        uniqueFilters.add(filter)
                        basicNodeExporter.setNodes([filter])
                        filters.append({filter : basicNodeExporter.encode()})
                    
                    # Append our current aovNode's driver and filter to the currentOutputs
                    currentOutputs.append( { "driver" : driver, "filter" : filter } )
                    
                # Set the outputs for the specified node name.
                outputs.append({ aovNodeName : currentOutputs })

                # Set the AOV information for the current AOV node name.
                basicNodeExporter.setNodes([aovNodeName])
                aovs.append( { aovNodeName : basicNodeExporter.encode() })
                
            # Set the AOV, filters, drivers, and outputs values
            aovsJSON["aovs"] = aovs
            aovsJSON["filters"] = filters
            aovsJSON["drivers"] = drivers
            aovsJSON["outputs"] = outputs
            return aovsJSON
         
        def decode(self, aovsJSON, decodeType):
            
            self._createUnifiedRenderGlobalsWindowIfNeeded()

            # We're doing a replace, so remove all previous AOVs
            if decodeType == self.DECODE_TYPE_OVERWRITE:
                AOVInterface().removeAOVs(AOVInterface().getAOVs())

            basicNodeExporter = rendererCallbacks.BasicNodeExporter()

            outputs = aovsJSON["outputs"]
            filterNewNameMap = {} # Used when we're doing a merge
            driverNewNameMap = {} # Used when we're doing a merge
            for output in outputs:
                for key, value in output.iteritems():

                    # If we're doing a merge, we need to delete the nodes that overlap with the nodes we are importing.
                    aovName = key.lstrip("aiAOV_")
                    if decodeType == self.DECODE_TYPE_MERGE and AOVInterface().getAOVNode(aovName) != None:
                        AOVInterface().removeAOV(aovName)

                    # Create a new aov node with the given aov name
                    aovNode = AOVInterface().addAOV(aovName)
                    
                    # Iterate over the output's value indexes
                    for outputIndex in range(0, len(value)):
                        filterName = value[outputIndex]["filter"]
                        driverName = value[outputIndex]["driver"]
                        
                        # Try to create a filter with the provided name, but we might end up getting a different name back.
                        # So store a mapping between the old filter name and our new name if they are different.
                        newFilterName = filterName
                        if filterName != self.DEFAULT_ARNOLD_FILTER_NAME:
                            newFilterName = cmds.createNode("aiAOVFilter", name=filterName)
                            if newFilterName != filterName:
                                filterNewNameMap[filterName] = newFilterName

                        # Try to create a driver with the provided name, but we might end up getting a different name back.
                        # So store a mapping between the old driver name and our new name if they are different.
                        newDriverName = driverName
                        if driverName != self.DEFAULT_ARNOLD_DRIVER_NAME:
                            newDriverName = cmds.createNode("aiAOVDriver", name=driverName)
                            if newDriverName != driverName:
                                driverNewNameMap[driverName] = newDriverName
                                
                        # Setup our filter and driver connections.
                        if outputIndex > 0 or cmds.listConnections(key + ".outputs[" + str(outputIndex) + "].filter")[0] != filterName:
                            cmds.connectAttr(newFilterName + ".message", key + ".outputs[" + str(outputIndex) + "].filter", force=True)
                        if outputIndex > 0 or cmds.listConnections(key + ".outputs[" + str(outputIndex) + "].driver")[0] != driverName:
                            cmds.connectAttr(newDriverName + ".message", key + ".outputs[" + str(outputIndex) + "].driver", force=True)

            # Iterate over our AOVs and decode them
            aovs = aovsJSON["aovs"]
            for aov in aovs:
                for aovName, aovJSON in aov.iteritems():
                    basicNodeExporter.decode(aovJSON)
                    
            # Iterate over our filters and decode them
            filters = aovsJSON["filters"]
            for filter in filters:
                for filterName, filterJSON in filter.iteritems():
                    # If our filter name doesn't match our new filter name, 
                    # then we need to rename the filter in our JSON data.
                    if filterName in filterNewNameMap:
                        filterJSON = json.loads(json.dumps(filterJSON).replace(filterName, filterNewNameMap[filterName]))
                    basicNodeExporter.decode(filterJSON)

            # Iterate over our drivers and decode them
            drivers = aovsJSON["drivers"]
            for driver in drivers:
                for driverName, driverJSON in driver.iteritems():
                    # If our driver name doesn't match our new driver name, 
                    #then we need to rename the driver in our JSON data.
                    if driverName in driverNewNameMap:
                        driverJSON = json.loads(json.dumps(driverJSON).replace(driverName, driverNewNameMap[driverName]))
                    basicNodeExporter.decode(driverJSON)
            
            # Rebuild Arnold AOV tab as it has changed.
            aoveditor.refreshArnoldAOVTab()

        # Displays the AOV unifiedRenderGlobalsWindow tab
        def displayMenu(self):
            mel.eval('unifiedRenderGlobalsWindow')
            mel.eval('setCurrentTabInRenderGlobalsWindow(\"AOVs\")')
            mel.eval('fillSelectedTabForCurrentRenderer')
            
        # Given an aovNode (aiAOV, aiAOVFilter, or aiAOVDriver type node), returns the aovName.
        def getAOVName(self, aovNode):
            if cmds.nodeType(aovNode) == "aiAOV":
                return cmds.getAttr(aovNode + ".name")
            else:
                aovNode2 = cmds.listConnections(aovNode)[0]
                if cmds.nodeType(aovNode2) == "aiAOV":
                    return self.getAOVName(aovNode2)
                else:
                    return ""

        # Creates a selector for the AOV Collection that selects all aovNodes (aiAOV, aiAOVDriver, aiAOVFilter nodes)
        def getCollectionSelector(self, selectorName):
            returnSelectorName = cmds.createNode(selector.BasicSelector.kTypeName, name=selectorName, skipSelect=True)
            currentSelector = renderSetupUtils.nameToUserNode(returnSelectorName)
            currentSelector.setPattern("*")
            currentSelector.setFilterType(selector.Filters.kCustom)
            currentSelector.setCustomFilterValue("aiAOV aiAOVDriver aiAOVFilter")
            currentSelector.setIncludeHierarchy(False)
            return returnSelectorName

        # Creates a selector for the AOV Child Collection for a particular AOV name.
        def getChildCollectionSelector(self, selectorName, aovName):
            returnSelectorName = cmds.createNode(selector.BasicSelector.kTypeName, name=selectorName, skipSelect=True)
            currentSelector = renderSetupUtils.nameToUserNode(returnSelectorName)
            aovNodeName = str(AOVInterface().getAOVNode(aovName))
            currentSelector.staticSelection.setWithoutExistenceCheck([aovNodeName])
            currentSelector.setIncludeHierarchy(True)
            return returnSelectorName

    arnoldAOVCallbacks = ArnoldAOVCallbacks()
except ImportError:
    arnoldAOVCallbacks = None

def aiHyperShadePanelBuildCreateMenuCallback() :
    if cmds.pluginInfo('mtoa', query=True, loaded=True) :
        aiHyperShadeCreateMenu_BuildMenu()
        cmds.menuItem(divider=True)

def aiHyperShadePanelBuildCreateSubMenuCallback() :  
    return "rendernode/arnold"

def aiRenderNodeClassificationCallback() :
    return "rendernode/arnold"

def aiCreateRenderNodePluginChangeCallback(classification) :
    return classification.startswith("rendernode/arnold")

def aiCreateRenderNodeSelectNodeCategoriesCallback(nodeTypesFlag, renderNodeTreeLister) :
    if (nodeTypesFlag == "allWithArnoldUp") :
        cmds.treeLister(renderNodeTreeLister, edit=True, selectPath="arnold")

def aiBuildRenderNodeTreeListerContentCallback(renderNodeTreeLister, postCommand, filterString) :
    filterClassArray = filterString
    createArnoldNodesTreeLister_Content(renderNodeTreeLister, postCommand, filterClassArray)

def aiProvideClassificationStringsForFilteredTreeListerCallback(classification) :
    return "rendernode/arnold/shader/surface"

def aiNodeCanBeUsedAsMaterialCallback(nodeId, nodeOwner ) :
    if nodeOwner == "Arnold":
        return 1
    else:
        return 0

def castSelf(selfid):
    # Can't pass self as an object.
    # It's cast to id(self) by the caller
    # and we convert it back to a python object here
    if isinstance(selfid,str):
        return ctypes.cast( int(selfid), ctypes.py_object ).value
    else:
        return selfid
      
def aiExportFrame( self, frame, objFilename ):
    # Export a single mental ray archive frame.
    cmds.currentTime( frame )

    cmds.arnoldExportAss(f=objFilename+".ass.gz", s=True, c=True, bb=True, mask=56)
    #cmds.Mayatomr( 	mi=True, exportFilter=7020488, 
    #            active=True, binary=True, compression=9, 
    #            fragmentExport=True, fragmentChildDag =True, passContributionMaps =True, 
    #            assembly=True, assemblyRootName="obj", exportPathNames="n", 
    #            file=objFilename + ".mi" )
    self.log( "assExport " + objFilename + ".ass.gz")

def aiExportAppendFile( self, assFilename, material, obj, lod ):
    lodList = self.tweakLodAppend( self.curFiles, lod  )
    for l in lodList:
        self.addArchiveFile( "ass", assFilename, material, "", l, 3 )
        
def aiExport( self, objs, filename, lod, materialNS ):
    filename = self.nestFilenameInDirectory( filename, "ass" )
    
    lastProgress = self.progress
    self.splitProgress( len(objs) )
    
    self.log( "assExport " + filename + lod )
    
    # force units to centimeters when exporting mi.
    #prevUnits = cmds.currentUnit( query=True, linear=True, fullName=True )
    #cmds.currentUnit( linear="centimeter" )
    
    prevTime = cmds.currentTime( query=True )

    for obj in objs:
        objFilename = filename + "_" + obj.replace("|", "_").replace(":", "_") + lod
        cmds.select( obj, r=True )

        filenames = []
        # Choose to export single file or a sequence.
        frameToken = ""
        if self.startFrame != self.endFrame:
            frameToken =".${FRAME}"

            dummyFrameFile = open( objFilename + frameToken + ".ass.gz", "wt" )
            dummyFrameFile.write( "STARTFRAME=%4.4d\nENDFRAME=%4.4d\n" % (int( self.startFrame), int(self.endFrame) ) )
            dummyFrameFile.close()

            for curFrame in range( int(self.startFrame), int(self.endFrame)+ 1 ):
                aiExportFrame( self, curFrame, objFilename + ".%4.4d" % int(curFrame) )
        else:
            aiExportFrame( self, self.startFrame, objFilename )
        
        if self.curFiles != None:
            materials = self.getSGsFromObj( obj )
            if materials and len(materials)>0 :
                assFilename = objFilename + frameToken + ".ass.gz"
                aiExportAppendFile( self, assFilename, materialNS+materials[0], obj, lod )
        self.incProgress()
    
    #cmds.currentUnit( linear=prevUnits )
    cmds.currentTime( prevTime )
    
    self.progress = lastProgress

def aiPreventDeletionFromCleanUpSceneCommandCallback(shader, plug, connection) :
    if (plug == "defaultArnoldRenderOptions"):
        return True
    else:
        return False
               
def xgaiArchiveExport(selfid) :
    self = castSelf(selfid)
    aiExport( self, self.invokeArgs[0], self.invokeArgs[1], self.invokeArgs[2], self.invokeArgs[3] )
    
def xgaiArchiveExportInfo(selfid) :
    self = castSelf(selfid)
    self.archiveDirs.append( "ass" )
    self.archiveLODBeforeExt.append( ".${FRAME}.ass" )
    self.archiveLODBeforeExt.append( ".${FRAME}.ass.gz" )
    self.archiveLODBeforeExt.append( ".ass" )
    self.archiveLODBeforeExt.append( ".ass.gz" )
    
#def xgaiArchiveExportInit(selfid) :
#    print "##### xgaiArchiveExportInit ",selfid
 

try:
    import xgenm as xg
    xg.registerCallback( "ArchiveExport", "mtoa.cmds.rendererCallbacks.xgaiArchiveExport" )
    xg.registerCallback( "ArchiveExportInfo", "mtoa.cmds.rendererCallbacks.xgaiArchiveExportInfo" )
    #xg.registerCallback( "ArchiveExportInit", "mtoa.cmds.rendererCallbacks.xgaiArchiveExportInit" )
except:
    pass
 

# Add the callbacks
def registerCallbacks():
    if cmds.about(batch=True):
        return
        
    if not arnoldAOVCallbacks is None:
        rendererCallbacks.registerCallbacks("arnold", rendererCallbacks.CALLBACKS_TYPE_AOVS, arnoldAOVCallbacks)
        
    cmds.callbacks(addCallback=aiHyperShadePanelBuildCreateMenuCallback,
                   hook="hyperShadePanelBuildCreateMenu",
                   owner="arnold")

    cmds.callbacks(addCallback=aiHyperShadePanelBuildCreateSubMenuCallback,
                   hook="hyperShadePanelBuildCreateSubMenu",
                   owner="arnold")

    cmds.callbacks(addCallback=aiCreateRenderNodeSelectNodeCategoriesCallback,
                   hook="createRenderNodeSelectNodeCategories",
                   owner="arnold")
               

    # FIXME: Maya doc is wrong
    #cmds.callbacks(addCallback=aiRenderNodeClassificationCallback,
    #               hook="addToRenderNodeTreeLister",
    #              owner="arnold")
    # Should be this instead

    cmds.callbacks(addCallback=aiRenderNodeClassificationCallback,
                   hook="renderNodeClassification",
                   owner="arnold")

    cmds.callbacks(addCallback=aiBuildRenderNodeTreeListerContentCallback,
                   hook="buildRenderNodeTreeListerContent",
                   owner="arnold")

    cmds.callbacks(addCallback=aiCreateRenderNodePluginChangeCallback,
                   hook="createRenderNodePluginChange",
                   owner="arnold")

    cmds.callbacks(addCallback=templates.loadArnoldTemplate,
                   hook="AETemplateCustomContent",
                   owner="arnold")

    cmds.callbacks(addCallback=aiProvideClassificationStringsForFilteredTreeListerCallback,
                   hook="provideClassificationStringsForFilteredTreeLister",
                   owner="arnold")

    cmds.callbacks(addCallback=aiNodeCanBeUsedAsMaterialCallback,
                   hook="nodeCanBeUsedAsMaterial",
                   owner="arnold")
    cmds.callbacks(addCallback=aiPreventDeletionFromCleanUpSceneCommandCallback,
               hook="preventMaterialDeletionFromCleanUpSceneCommand",
               owner="arnold")

def clearCallbacks():
    if cmds.about(batch=True):
        return
    try:
        cmds.callbacks(clearAllCallbacks=True, owner="arnold")
    except:
        pass


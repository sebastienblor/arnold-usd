#include "DagTranslator.h"

#include <maya/MPlugArray.h>
#include <maya/MFnSet.h>
#include <maya/MNodeMessage.h>
#include <maya/MDagPathArray.h>
#include <maya/MFnTransform.h>
#include "NodeTranslatorImpl.h"
void CDagTranslator::Export(AtNode* node)
{
   AtParamIterator* nodeParam = AiNodeEntryGetParamIterator(AiNodeGetNodeEntry(node));
   while (!AiParamIteratorFinished(nodeParam))
   {
      const AtParamEntry *paramEntry = AiParamIteratorGetNext(nodeParam);
      const char* paramName = AiParamGetName(paramEntry);

      if (strcmp(paramName, "name") != 0)
      {
         if (strcmp(paramName, "matrix") == 0)
            ExportMatrix(node);
         else
            ProcessParameter(node, paramName, AiParamGetType(paramEntry));
      }
   }
   AiParamIteratorDestroy(nodeParam);
}

// For Motion Blur only re-export the transform matrix
void CDagTranslator::ExportMotion(AtNode* node)
{
   if (AiNodeEntryLookUpParameter(AiNodeGetNodeEntry(node), "matrix"))
      ExportMatrix(node);   
}

/// get override sets containing the passed Maya dag path
/// and add them to the passed MObjectArray
/// and we also need to check the parent nodes, so groups are handled properly
MStatus CDagTranslator::GetOverrideSets(MDagPath path, MObjectArray &overrideSets)
{
   MStatus status;
   MDagPath pathRec = path;
   for(;pathRec.length();pathRec.pop(1))
   {
      MFnDagNode fnDag(pathRec);
      unsigned int instNum = pathRec.instanceNumber();
      MPlug instObjGroups = fnDag.findPlug("instObjGroups", true, &status).elementByLogicalIndex(instNum);
      CHECK_MSTATUS(status)
      MPlugArray connections;
      MFnSet fnSet;
      // MString plugName = instObjGroups.name();
      if (instObjGroups.connectedTo(connections, false, true, &status))
      {
         unsigned int nc = connections.length();
         for (unsigned int i=0; i<nc; i++)
         {
            MObject set = connections[i].node();
            MFnDependencyNode setDNode(set);
            if (setDNode.typeName() == MString("objectSet"))
            {
               if (!fnSet.setObject(set))
                  continue;
               // MString setName = fnSet.name();
               // Also add sets with override turned off to allow chaining
               // on these as well
               MPlug p = fnSet.findPlug("aiOverride", true, &status);
               if ((MStatus::kSuccess == status) && !p.isNull())
               {
                  overrideSets.append(set);
               }
            }
         }
      }
   }

   return status;
}

/// gather the active override sets containing this node
/// and export them
MStatus CDagTranslator::ExportOverrideSets()
{
   MStatus status;

   m_impl->m_overrideSets.clear();
   MDagPath path = m_dagPath;
   // Check for passed path
   MObjectArray overrideSetObjs;
   status = GetOverrideSets(path, overrideSetObjs);
   // If passed path is a shape, check for its transform as well
   // FIXME: do we want to consider full hierarchy ?
   // Also consider the sets the transform of that shape might be in
   const MObject transformObj = path.transform(&status);
   while ((MStatus::kSuccess == status) && (transformObj != path.node(&status)))
   {
      status = path.pop();
   }
   if (!(path == m_dagPath))
   {
      status = GetOverrideSets(path, overrideSetObjs);
   }
   // Exporting a set creates no Arnold object but allow IPR to track it
   MFnSet fnSet;
   unsigned int ns = overrideSetObjs.length();
   for (unsigned int i=0; i<ns; i++)
   {
      fnSet.setObject(overrideSetObjs[i]);
      m_impl->m_overrideSets.push_back(GetSession()->ExportNode(fnSet.findPlug("message")));
   }

   return status;
}

/// set the name of the arnold node
void CDagTranslator::SetArnoldNodeName(AtNode* arnoldNode, const char* tag)
{
   MString name = m_dagPath.partialPathName();
   // TODO: add a global option to control how names are exported
   // MString name = m_dagPath.fullPathName();
   if (DependsOnOutputPlug())
   {
      MString outputAttr = GetMayaAttributeName();

      if (outputAttr.numChars())
         name = name + AI_ATT_SEP + outputAttr;
   }
   if (strlen(tag))
      name = name + AI_TAG_SEP + tag;

   AiNodeSetStr(arnoldNode, "name", name.asChar());
}

void CDagTranslator::AddHierarchyCallbacks(const MDagPath & path)
{
   AiMsgDebug("[mtoa.translator.ipr] %-30s | %s: Add DAG parents update callbacks for translator %p",
      path.partialPathName().asChar(), GetTranslatorName().asChar(), this);

   // Loop through the whole dag path adding callbacks to them.
   MStatus status;
   MDagPath dag_path(path);
   dag_path.pop(); // Pop of the shape as that's handled by CNodeTranslator::AddUpdateCallbacks.
   for(; dag_path.length() > 0; dag_path.pop())
   {
      MObject node = dag_path.node();
      if (node != MObject::kNullObj)
      {
         // We can use the normal NodeDirtyCallback here.
         MCallbackId id = MNodeMessage::addNodeDirtyCallback(node,
                                                             NodeDirtyCallback,
                                                             this,
                                                             &status);
         if (MS::kSuccess == status) ManageUpdateCallback(id);
      }
   }
}


void CDagTranslator::AddUpdateCallbacks()
{
   AddHierarchyCallbacks(m_dagPath);

   // Call the base class to get the others.
   CNodeTranslator::AddUpdateCallbacks();
}

void CDagTranslator::Delete()
{

   AiRenderInterrupt();
   
   AiNodeDestroy(GetArnoldRootNode());

   // Arnold doesn't allow us to create nodes in between to calls to AiRender
   // for the moment. For IPR we still need to rely on setting the visibility for now.
   //AiNodeSetInt(GetArnoldRootNode(), "visibility",  AI_RAY_UNDEFINED);
   m_impl->m_atNode = NULL;
   m_impl->m_atNodes.clear();
}

/// Return whether the current dag object is the master instance.
///
/// The master is the first instance that is completely visible (including parent transforms)
/// for which full geometry should be exported.
///
/// Always returns true if this dagPath is not instanced.
/// If dagPath is instanced, this searches the preceding instances
/// for the first that is visible. if none are found, dagPath is considered the master.
///
/// This function caches the result on the first run and returns the cached results on
/// subsequent calls.
///
/// note: dagPath is assumed to be visible.
///
/// @return                  whether or not dagPath is a master
///
bool CDagTranslator::IsMasterInstance()
{
   if (!m_masterDag.isValid())
      m_isMasterDag = DoIsMasterInstance(m_dagPath, m_masterDag);
   return m_isMasterDag;
}

/// Return the master instance for the current dag object.
///
/// The master is the first instance that is completely visible (including parent transforms)
/// for which full geometry should be exported.
///
MDagPath& CDagTranslator::GetMasterInstance()
{
   if (!m_masterDag.isValid())
      m_isMasterDag = DoIsMasterInstance(m_dagPath, m_masterDag);
   return m_masterDag;
}

/// Like IsMasterInstance, but does not cache result
bool CDagTranslator::DoIsMasterInstance(const MDagPath& dagPath, MDagPath &masterDag)
{
   if (dagPath.isInstanced())
   {
      MObjectHandle handle = MObjectHandle(dagPath.node());
      unsigned int instNum = dagPath.instanceNumber();
      // first instance
      if (instNum == 0)
      {
         // first visible instance is always the master (passed dagPath is assumed to be visible)
         GetSession()->AddMasterInstanceHandle(handle, dagPath);
         return true;
      }
      else
      {
         // if handle is not in the map, a new entry will be made with a default value
         MDagPath currDag = GetSession()->GetMasterInstanceDagPath(handle);
         if (currDag.isValid())
         {
            // previously found the master
            masterDag.set(currDag);
            return false;
         }
         // find the master by searching preceding instances
         MDagPathArray allInstances;
         MDagPath::getAllPathsTo(dagPath.node(), allInstances);
         unsigned int master_index = 0;
         for (; (master_index < dagPath.instanceNumber()); master_index++)
         {
            currDag = allInstances[master_index];
            if (GetSession()->IsRenderablePath(currDag))
            {
               // found it
               GetSession()->AddMasterInstanceHandle(handle, currDag);
               masterDag.set(currDag);
               return false;
            }
         }
         // didn't find a master: dagPath is the master
         GetSession()->AddMasterInstanceHandle(handle, dagPath);
         return true;
      }
   }
   // not instanced: dagPath is the master
   return true;
}

void CDagTranslator::GetRotationMatrix(AtMatrix& matrix)
{
   MObject transform = m_dagPath.transform();
   MFnTransform mTransform;
   mTransform.setObject(transform);
   MTransformationMatrix mTransformMatrix = mTransform.transformation();

   MMatrix tm = mTransformMatrix.asRotateMatrix();
   for (int J = 0; (J < 4); ++J)
   {
      for (int I = 0; (I < 4); ++I)
      {
         matrix[I][J] = (float) tm[I][J];
      }
   }
}

void CDagTranslator::GetMatrix(AtMatrix& matrix, const MDagPath& path, CArnoldSession* session)
{
   MStatus stat;
   MMatrix tm = path.inclusiveMatrix(&stat);
   if (MStatus::kSuccess != stat)
   {
      AiMsgError("Failed to get transformation matrix for %s",  path.partialPathName().asChar());
   }
   ConvertMatrix(matrix, tm, session);
}

void CDagTranslator::GetMatrix(AtMatrix& matrix)
{
   GetMatrix(matrix, m_dagPath, GetSession());
}

// this is a utility method which handles the common tasks associated with
// exporting matrix information. it properly handles exporting a matrix array
// if motion blur is enabled and required by the node. it should be called
// at each motion step
void CDagTranslator::ExportMatrix(AtNode* node)
{
   int step = GetMotionStep();
   AtMatrix matrix;
   GetMatrix(matrix);
   if (step == 0)
   {
      // why not only RequiresMotionData() ??
      if (IsMotionBlurEnabled(MTOA_MBLUR_OBJECT) && RequiresMotionData())
      {
         AtArray* matrices = AiArrayAllocate(1, GetNumMotionSteps(), AI_TYPE_MATRIX);
         AiArraySetMtx(matrices, 0, matrix);
         AiNodeSetArray(node, "matrix", matrices);
      }
      else
      {
         AiNodeSetMatrix(node, "matrix", matrix);
      }
   }
   else if (IsMotionBlurEnabled(MTOA_MBLUR_OBJECT) && RequiresMotionData())
   {
      AtArray* matrices = AiNodeGetArray(node, "matrix");
      AiArraySetMtx(matrices, step, matrix);
   }
}

AtByte CDagTranslator::ComputeVisibility(const MDagPath& path)
{
   // Usually invisible nodes are not exported at all, just making sure here
   if (false == GetSession()->IsRenderablePath(path))
      return AI_RAY_UNDEFINED;

   AtByte visibility = AI_RAY_ALL;
   MPlug plug;

   plug = FindMayaPlug("visibility");

   if (!plug.isNull() && !plug.asBool())
      return 0;

   plug = FindMayaPlug("castsShadows");
   if (!plug.isNull() && !plug.asBool())
   {
      visibility &= ~AI_RAY_SHADOW;
   }

   plug = FindMayaPlug("primaryVisibility");
   MString plugName = plug.name();
   if (!plug.isNull() && !plug.asBool())
   {
      visibility &= ~AI_RAY_CAMERA;
   }

   plug = FindMayaPlug("visibleInReflections");
   if (!plug.isNull() && !plug.asBool())
   {
      visibility &= ~AI_RAY_REFLECTED;
   }

   plug = FindMayaPlug("visibleInRefractions");
   if (!plug.isNull() && !plug.asBool())
   {
      visibility &= ~AI_RAY_REFRACTED;
   }

   plug = FindMayaPlug("aiVisibleInDiffuse");
   if (!plug.isNull() && !plug.asBool())
   {
      visibility &= ~AI_RAY_DIFFUSE;
   }

   plug = FindMayaPlug("aiVisibleInGlossy");
   if (!plug.isNull() && !plug.asBool())
   {
      visibility &= ~AI_RAY_GLOSSY;
   }

   return visibility;
}

// use standardized render flag names to compute an arnold visibility mask
AtByte CDagTranslator::ComputeVisibility()
{
   return ComputeVisibility(m_dagPath);
}

// Create Maya visibility attributes with standardized render flag names
//
// These are the attributes that compute the "visibility" parameter. there are other
// attributes like self_shadow and opaque that are computed separately
//
// This is for custom DAG nodes where none of the standard maya visibility attributes
// are available. typically CDagTranslator::AddArnoldVisibilityAttrs() is the appropriate function.
//
void CDagTranslator::MakeMayaVisibilityFlags(CBaseAttrHelper& helper)
{
   CAttrData data;

   data.defaultValue.BOOL = true;
   data.name = "primaryVisibility";
   data.shortName = "vis";
   helper.MakeInputBoolean(data);

   data.defaultValue.BOOL = true;
   data.name = "receiveShadows";
   data.shortName = "rsh";
   helper.MakeInputBoolean(data);

   data.defaultValue.BOOL = true;
   data.name = "castsShadows";
   data.shortName = "csh";
   helper.MakeInputBoolean(data);

   data.defaultValue.BOOL = true;
   data.name = "visibleInReflections";
   data.shortName = "vir";
   helper.MakeInputBoolean(data);

   data.defaultValue.BOOL = true;
   data.name = "visibleInRefractions";
   data.shortName = "vif";
   helper.MakeInputBoolean(data);
}

// create arnold visibility attributes with standardized render flag names
//
// These are the attributes that help compute the "visibility" parameter. there are other
// attributes like self_shadow and opaque that are computed separately
//
// arnold's visibiltity mask adds several relationships not available by default in Maya.
// use in conjunction with CDagTranslator::ComputeVisibility() or CShapeTranslator::ProcessRenderFlags().
//
void CDagTranslator::MakeArnoldVisibilityFlags(CBaseAttrHelper& helper)
{
   CAttrData data;

   data.defaultValue.BOOL = true;
   data.name = "aiVisibleInDiffuse";
   data.shortName = "ai_vid";
   data.channelBox = false;
   data.keyable = false;
   helper.MakeInputBoolean(data);

   data.defaultValue.BOOL = true;
   data.name = "aiVisibleInGlossy";
   data.shortName = "ai_vig";
   data.channelBox = false;
   data.keyable = false;
   helper.MakeInputBoolean(data);
}

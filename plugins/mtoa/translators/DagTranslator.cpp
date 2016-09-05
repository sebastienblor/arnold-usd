#include "DagTranslator.h"
#include "DagTranslatorImpl.h"
#include "scene/MayaScene.h"
#include <maya/MPlugArray.h>
#include <maya/MFnSet.h>
#include <maya/MNodeMessage.h>
#include <maya/MDagPathArray.h>
#include <maya/MFnTransform.h>
#include "NodeTranslatorImpl.h"

void CDagTranslator::Init()
{
   //CNodeTranslator::Init(); // does nothing, but this could change
   // FIXME: maybe this could be overkill with tons of DAG instances.
   // we'll be able to solve this soon if we start having implementations inheriting from the base one
   MDagPathArray dagArray;
   MDagPath::getAllPathsTo(GetMayaObject(), dagArray);
   for (unsigned int i = 0; i < dagArray.length(); ++i)
   {
      if ((int)dagArray[i].instanceNumber() == m_impl->m_handle.instanceNum())
      {
         // this is the right instance
         m_dagPath = dagArray[i];
         break;
      }
   }
}
void CDagTranslator::CreateImplementation()
{
   m_impl = new CDagTranslatorImpl(*this);
}
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

/// set the name of the arnold node
void CDagTranslatorImpl::SetArnoldNodeName(AtNode* arnoldNode, const char* tag)
{
   CDagTranslator *dagTr = static_cast<CDagTranslator*>(&m_tr);

   MString name = dagTr->GetMayaDagPath().partialPathName();
   // TODO: add a global option to control how names are exported
   // MString name = m_dagPath.fullPathName();
   if (m_tr.DependsOnOutputPlug())
   {
      MString outputAttr = m_handle.attribute();

      if (outputAttr.numChars())
         name = name + AI_ATT_SEP + outputAttr;
   }
   if (tag != NULL && strlen(tag))
      name = name + AI_TAG_SEP + tag;

   AiNodeSetStr(arnoldNode, "name", name.asChar());
}



void CDagTranslator::AddUpdateCallbacks()
{
   // Add hierarchy callbacks

   // Loop through the whole dag path adding callbacks to them.
   MStatus status;
   MDagPath dag_path(m_dagPath);
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
         if (MS::kSuccess == status) RegisterUpdateCallback(id);
      }
   }


   // Call the base class to get the others.
   CNodeTranslator::AddUpdateCallbacks();
}

/// Like IsMasterInstance, but does not cache result
static bool DoIsMasterInstance(CDagTranslator *translator, CArnoldSession *session, const MDagPath& dagPath, MDagPath &masterDag)
{
   if (dagPath.isInstanced())
   {
      MObjectHandle handle = MObjectHandle(dagPath.node());
      unsigned int instNum = dagPath.instanceNumber();
      // first instance
      if (instNum == 0)
      {
         // first visible instance is always the master (passed dagPath is assumed to be visible)
         session->AddMasterInstanceHandle(handle, dagPath);
         return true;
      }
      else
      {
         // if handle is not in the map, a new entry will be made with a default value
         MDagPath currDag = session->GetMasterInstanceDagPath(handle);
         if (currDag.isValid())
         {
            // previously found the master
            masterDag.set(currDag);
            return false;
         }
         // find the master by searching preceding instances
         MDagPathArray allInstances;
         MDagPath::getAllPathsTo(dagPath.node(), allInstances);
         for (unsigned int master_index = 0; master_index < instNum; master_index++)
         {
            currDag = allInstances[master_index];
            if (translator->IsRenderable())
            {
               // found it
               session->AddMasterInstanceHandle(handle, currDag);
               masterDag.set(currDag);
               return false;
            }
         }
         // didn't find a master: dagPath is the master
         session->AddMasterInstanceHandle(handle, dagPath);
         return true;
      }
   }
   // not instanced: dagPath is the master
   return true;
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
   CDagTranslatorImpl *dagImpl = static_cast<CDagTranslatorImpl*>(m_impl);
   if (!dagImpl->m_masterDag.isValid())
      dagImpl->m_isMasterDag = DoIsMasterInstance(this, m_impl->m_session, m_dagPath, dagImpl->m_masterDag);
   return dagImpl->m_isMasterDag;
}

/// Return the master instance for the current dag object.
///
/// The master is the first instance that is completely visible (including parent transforms)
/// for which full geometry should be exported.
///
MDagPath& CDagTranslator::GetMasterInstance()
{
   CDagTranslatorImpl *dagImpl = static_cast<CDagTranslatorImpl*>(m_impl);
   if (!dagImpl->m_masterDag.isValid())
      dagImpl->m_isMasterDag = DoIsMasterInstance(this, m_impl->m_session, m_dagPath, dagImpl->m_masterDag);
   return dagImpl->m_masterDag;
}

void CDagTranslator::GetMatrix(AtMatrix& matrix)
{
   MStatus stat;
   MMatrix tm = m_dagPath.inclusiveMatrix(&stat);
   if (MStatus::kSuccess != stat)
   {
      AiMsgError("Failed to get transformation matrix for %s",  m_dagPath.partialPathName().asChar());
   }
   ConvertMatrix(matrix, tm);
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

bool CDagTranslator::IsRenderable() const
{
   return m_impl->m_session->IsRenderablePath(m_dagPath);
}

AtByte CDagTranslator::ComputeVisibility()
{
   // Usually invisible nodes are not exported at all, just making sure here
   if (false == m_impl->m_session->IsRenderablePath(m_dagPath))
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

CDagTranslator *CDagTranslator::ExportDagPath(const MDagPath &dagPath)
{
   return CMayaScene::GetArnoldSession()->ExportDagPath(dagPath);
}

void CDagTranslatorImpl::ExportUserAttribute(AtNode *anode)
{
   // testing if anode is ginstance instead of calling IsMasterInstance
   // for efficiency reasons.
   if (AiNodeIs(anode, "ginstance"))
   {
      CDagTranslator *dagTr = static_cast<CDagTranslator*>(&m_tr);

      CNodeTranslator::ExportUserAttributes(anode, dagTr->GetMayaDagPath().transform(), &m_tr);
      
      // FIXME below is what's being done in parent function for aiUserOptions
      // is that what we want to do here ?
      MPlug plug = m_tr.FindMayaPlug("aiUserOptions");
      if (!plug.isNull())
         AiNodeSetAttributes(anode, plug.asString().asChar());
   }
   else
      CNodeTranslatorImpl::ExportUserAttribute(anode);
}

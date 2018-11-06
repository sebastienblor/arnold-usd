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
bool CDagTranslator::IsTransformPlug(const MPlug &plug)
{
   MString plugName =  plug.partialName(false, false, false, false, false, true);
   if (plugName.length() == 0) return false;
   const char firstChar = plugName.asChar()[0];

   if (firstChar == 's')
      return (plugName == "scale" || plugName == "scaleX" || plugName == "scaleY" || plugName == "scaleZ" );
   if (firstChar == 't')
      return (plugName == "translate" || plugName == "translateX" || plugName == "translateY" || plugName == "translateZ");
   if (firstChar == 'r')
      return (plugName == "rotate" || plugName == "rotateX" || plugName == "rotateY" || plugName == "rotateZ");
   if (firstChar == 'p')
      return (plugName == "parentMatrix");
   if (firstChar == 'w')
      return (plugName == "worldMatrix");


   return false;        
}

MString CDagTranslator::GetArnoldNaming(const MDagPath &dagPath)
{
   MString name = GetSessionOptions().GetExportFullPath() ? 
      dagPath.fullPathName() : dagPath.partialPathName();

   const MString &prefix = GetSessionOptions().GetExportPrefix();
   if (prefix.length() > 0)
      name = prefix + name;
   return name;
}


MString CDagTranslatorImpl::MakeArnoldName(const char *nodeType, const char* tag)
{
   CDagTranslator *dagTr = static_cast<CDagTranslator*>(&m_tr);
   MString name = CDagTranslator::GetArnoldNaming(dagTr->GetMayaDagPath());

   if (DependsOnOutputPlug())
   {
      MString outputAttr = m_handle.attribute();
      if (outputAttr.numChars())
         name = name + AI_ATT_SEP + outputAttr;
   }
   if (tag != NULL && strlen(tag))
      name = name + AI_TAG_SEP + tag;

   return name;
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

static bool DoIsMasterInstance(CDagTranslator *translator, CArnoldSession *session, const MDagPath& dagPath, MDagPath &masterDag)
{
   if (dagPath.isInstanced())
   {
      MObjectHandle handle = MObjectHandle(dagPath.node());

      MDagPath srcParent;
      MFnDagNode(MFnDagNode(dagPath.node()).parent(0)).getPath(srcParent);
      srcParent.push(dagPath.node());

      masterDag.set(srcParent);
      // first instance
      if (srcParent.fullPathName() == dagPath.fullPathName())
      {
         // first visible instance is always the master (passed dagPath is assumed to be visible)
         session->AddMasterInstanceHandle(handle, srcParent);
         return true;
      }
      else
      {
         // didn't find a master: dagPath is the master
         session->AddMasterInstanceHandle(handle, dagPath);
         return false;
      }
   }
   // not instanced: dagPath is the master
   return true;
}


bool CDagTranslator::IsMasterInstance()
{
   CDagTranslatorImpl *dagImpl = static_cast<CDagTranslatorImpl*>(m_impl);
   if (!dagImpl->m_masterDag.isValid())
      dagImpl->m_isMasterDag = DoIsMasterInstance(this, m_impl->m_session, m_dagPath, dagImpl->m_masterDag);
   return dagImpl->m_isMasterDag;
}

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
   AtMatrix matrix;
   GetMatrix(matrix);
   if (!IsExportingMotion())
   {
      // why not only RequiresMotionData() ??
      if (IsMotionBlurEnabled(MTOA_MBLUR_OBJECT) && RequiresMotionData())
      {
         AtArray* matrices = AiArrayAllocate(1, GetNumMotionSteps(), AI_TYPE_MATRIX);
         AiArraySetMtx(matrices, GetMotionStep(), matrix);
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
      if (matrices)
      {
         int step = GetMotionStep();
         if (step >= (int)(AiArrayGetNumKeys(matrices) * AiArrayGetNumElements(matrices)))
         {
            AiMsgError("Matrix AtArray steps not set properly for %s",  m_dagPath.partialPathName().asChar());

         } else
            AiArraySetMtx(matrices, step, matrix);
         
      }
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
/*
  FIXME what do we do now ?
   plug = FindMayaPlug("visibleInReflections");
   if (!plug.isNull() && !plug.asBool())
   {
      visibility &= ~AI_RAY_SPECULAR_REFLECT;
   }

   plug = FindMayaPlug("visibleInRefractions");
   if (!plug.isNull() && !plug.asBool())
   {
      visibility &= ~AI_RAY_SPECULAR_TRANSMIT;
   }*/

   plug = FindMayaPlug("aiVisibleInDiffuseReflection");
   if (!plug.isNull() && !plug.asBool())
   {
      visibility &= ~(AI_RAY_DIFFUSE_REFLECT);
   }

   plug = FindMayaPlug("aiVisibleInSpecularReflection");
   if (!plug.isNull() && !plug.asBool())
   {
      visibility &= ~(AI_RAY_SPECULAR_REFLECT);
   }

   plug = FindMayaPlug("aiVisibleInDiffuseTransmission");
   if (!plug.isNull() && !plug.asBool())
   {
      visibility &= ~(AI_RAY_DIFFUSE_TRANSMIT);
   }

   plug = FindMayaPlug("aiVisibleInSpecularTransmission");
   if (!plug.isNull() && !plug.asBool())
   {
      visibility &= ~(AI_RAY_SPECULAR_TRANSMIT);
   }

   plug = FindMayaPlug("aiVisibleInVolume");
   if (!plug.isNull() && !plug.asBool())
   {
      visibility &= ~AI_RAY_VOLUME;
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

   data.defaultValue.BOOL() = true;
   data.name = "primaryVisibility";
   data.shortName = "vis";
   helper.MakeInputBoolean(data);

   data.defaultValue.BOOL() = true;
   data.name = "receiveShadows";
   data.shortName = "rsh";
   helper.MakeInputBoolean(data);

   data.defaultValue.BOOL() = true;
   data.name = "castsShadows";
   data.shortName = "csh";
   helper.MakeInputBoolean(data);

/*
   Can I stop creating these Arnold 4 attributes now ?
   data.defaultValue.BOOL() = true;
   data.name = "visibleInReflections";
   data.shortName = "vir";
   helper.MakeInputBoolean(data);

   data.defaultValue.BOOL() = true;
   data.name = "visibleInRefractions";
   data.shortName = "vif";
   helper.MakeInputBoolean(data);
   */
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

   data.defaultValue.BOOL() = true;
   data.name = "aiVisibleInDiffuseReflection";
   data.shortName = "ai_vidr";
   data.channelBox = false;
   data.keyable = false;
   helper.MakeInputBoolean(data);

   data.defaultValue.BOOL() = true;
   data.name = "aiVisibleInSpecularReflection";
   data.shortName = "ai_visr";
   data.channelBox = false;
   data.keyable = false;
   helper.MakeInputBoolean(data);

   data.defaultValue.BOOL() = true;
   data.name = "aiVisibleInDiffuseTransmission";
   data.shortName = "ai_vidt";
   data.channelBox = false;
   data.keyable = false;
   helper.MakeInputBoolean(data);

   data.defaultValue.BOOL() = true;
   data.name = "aiVisibleInSpecularTransmission";
   data.shortName = "ai_vist";
   data.channelBox = false;
   data.keyable = false;
   helper.MakeInputBoolean(data);


   data.defaultValue.BOOL() = true;
   data.name = "aiVisibleInVolume";
   data.shortName = "ai_viv";
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
   static const AtString ginstance_str("ginstance");
   if (AiNodeIs(anode, ginstance_str))
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

// Return the list of Sets containing this object, by checking the whole hierarchy.
// Only consider the Sets having aiOverride = true, the others can be ignored
static MStatus GetOverrideSets(MDagPath path, MObjectArray &overrideSets)
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

MStatus CDagTranslatorImpl::ExportOverrideSets()
{

   MStatus status;

   m_overrideSets.clear();
   CDagTranslator *dagTr = static_cast<CDagTranslator*>(&m_tr);
   MDagPath path = dagTr->GetMayaDagPath();
   // Check for passed path

   MObjectArray overrideSetObjs;
   // This will include the whole hierarchy
   status = GetOverrideSets(path, overrideSetObjs);
   
   // Exporting a set creates no Arnold object but allow IPR to track it
   MFnSet fnSet;
   const unsigned int ns = overrideSetObjs.length();
   for (unsigned int i = 0; i < ns; ++i)
   {
      fnSet.setObject(overrideSetObjs[i]);
      CNodeTranslator *translator = m_session->ExportNode(fnSet.findPlug("message"));
      if (translator)
         m_overrideSets.push_back(translator);
   }

   return status;
}

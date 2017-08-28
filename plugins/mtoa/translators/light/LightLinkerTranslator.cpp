#include "LightLinkerTranslator.h"
#include "utils/MtoaLog.h"
#include <maya/MPlugArray.h>
#include <maya/MDagPathArray.h>
#include <maya/MSelectionList.h>

// This translator doesn't create Arnold nodes,
// it's only purpose is to create additionnal attributes on Sets
AtNode* CLightLinkerTranslator::CreateArnoldNodes()
{
   return NULL;
}

void CLightLinkerTranslator::Export(AtNode *set)
{
}

void CLightLinkerTranslator::AttributeChangedCallback(MNodeMessage::AttributeMessage msg,
                                                    MPlug& plug, MPlug& otherPlug,
                                                    void* clientData)
{
   CLightLinkerTranslator * translator = static_cast< CLightLinkerTranslator* >(clientData);
   if (translator != NULL)
   {
      if (MtoaTranslationInfo())
         MtoaDebugLog("[mtoa.translator.ipr] "+translator->GetMayaNodeName()+" | "+translator->GetTranslatorName()+": LightLinkerAttributeChangedCallback "+plug.name()+" to or from "+otherPlug.name());

      // No need for full update when an object is added / removed from the linker
      // But needs a full update when a light is
      if (msg & (MNodeMessage::kConnectionMade | MNodeMessage::kConnectionBroken))
      {
         if ((msg & MNodeMessage::kIncomingDirection)
               && (msg & MNodeMessage::kOtherPlugSet))
         {
            MStringArray plugNameParts;
            MString plugName = plug.partialName();
            plugName.split('.', plugNameParts);
            MString leafAttrName = plugNameParts[plugNameParts.length()-1];
            if ((leafAttrName == "olnk") || (leafAttrName == "solk"))
            {
               // An object as been added or removed from set, partial update.
               MObject object(otherPlug.node());
               // Can be a dag node or not (if not probably a set or shading set)
               if (object.hasFn(MFn::kDagNode))
               {
                  MFnDagNode fnDag(object);
                  MStatus status;
                  unsigned int instanceNumber;
                  instanceNumber = otherPlug.logicalIndex(&status);
                  MDagPath path;
                  if (!status)
                  {
                     fnDag.getPath(path);
                  }
                  else
                  {
                     MDagPathArray allPaths;
                     fnDag.getAllPaths(allPaths);
                     path = allPaths[instanceNumber];
                  }
                  if (path.isValid())
                  {
                     CNodeTranslator *elemTr = CNodeTranslator::GetTranslator(path); 
                     if (elemTr)  elemTr->RequestUpdate();
                  }
                  // Check also for shapes
                  if (MStatus::kSuccess == path.extendToShape())
                  {
                     CNodeTranslator *elemTr = CNodeTranslator::GetTranslator(path); 
                     if (elemTr)  elemTr->RequestUpdate();
                  }
               } // if (object.hasFn(MFn::kDagNode))
               else
               {
                  // We don't need to explicitely request an update for all set members since we got
                  // a translator for sets
                  CNodeTranslator *elemTr = CNodeTranslator::GetTranslator(object); 
                  if (elemTr)  elemTr->RequestUpdate();
               }
            } // if ((leafAttrName == "olnk") || (leafAttrName == "solk"))
            else if ((leafAttrName == "llnk") || (leafAttrName == "sllk"))
            {
               // An light as been added or removed from set, partial update.
               // FIXME: When a light goes from illuminate by default to being linked
               // it's actually all dag nodes in scene that might be affected
               translator->RequestUpdate();
            } // else if ((leafAttrName == "llnk") || (leafAttrName == "sllk"))
            else
            {
               if (MtoaTranslationInfo())
                  MtoaDebugLog("[mtoa.translator.ipr] "+ translator->GetMayaNodeName()+"| "+
                     translator->GetTranslatorName()+": Connection made or broken on "+otherPlug.name()+"->"+plug.name()+" is ignored.");
            }
         }
         else
         {
            if (MtoaTranslationInfo())
               MtoaDebugLog("[mtoa.translator.ipr] "+translator->GetMayaNodeName()+" | "+translator->GetTranslatorName()+": Not an incoming connection on "+plug.name()+", ignored.");
         }
      }

   }
   else
   {
      // No translator in client data
      AiMsgError("[mtoa.translator.ipr] LightLinkerAttributeChangedCallback: no translator in client data: %p.", clientData);
   }
}


/// Sets have extra specific callback addLightLinkerAttributeChangedCallback
void CLightLinkerTranslator::AddUpdateCallbacks()
{
   MStatus status;
   MCallbackId id;

   MObject object = GetMayaObject();
   // So we update on attribute/input changes.
   id = MNodeMessage::addNodeDirtyCallback(object,
                                           NodeDirtyCallback,
                                           this,
                                           &status);
   if (MS::kSuccess == status) RegisterUpdateCallback(id);

   // In case we're deleted!
   id = MNodeMessage::addNodeAboutToDeleteCallback(object,
                                                   NodeAboutToBeDeletedCallback,
                                                   this,
                                                   &status);
   if (MS::kSuccess == status) RegisterUpdateCallback(id);

   // Set members change
   id = MNodeMessage::addAttributeChangedCallback(object,
                                                  AttributeChangedCallback,
                                                  this,
                                                  &status);
   if (MS::kSuccess == status) RegisterUpdateCallback(id);
}



/// Update a light linker means update all member objects (not lights)
// Note that this function RequestUpdate is local to this class.
// It will never be called from CNodeTranslator as we're overriding the callbacks here
void CLightLinkerTranslator::RequestUpdate()
{
   MStatus stat;

   // This call used to be in export, but it seems better to set this now
   RequestLightLinksUpdate();

   const CSessionOptions &options = GetSessionOptions();

   // Update means all members should be updated
   if (MtoaTranslationInfo())
      MtoaDebugLog("[mtoa.translator.ipr] "+GetMayaNodeName()+" | "+GetTranslatorName()+": RequestUpdate for light linker updates all objects in light linker.");

   MFnDependencyNode fnDep(GetMayaObject());

   MSelectionList list;
   if ((options.GetLightLinkMode() != MTOA_LIGHTLINK_NONE) || (options.GetShadowLinkMode() == MTOA_SHADOWLINK_LIGHT))
   {
      MPlug link = fnDep.findPlug("link", true, &stat);
      CHECK_MSTATUS(stat);
      if (!link.isNull()) GetMembers(list, link, false, true);
   }
   if (options.GetShadowLinkMode() == MTOA_SHADOWLINK_MAYA)
   {
      MPlug link = fnDep.findPlug("shadowLink", true, &stat);
      CHECK_MSTATUS(stat);
      if (!link.isNull()) GetMembers(list, link, false, true);
   }
   // TODO: No support for ignore / shadow ignore currently
   MObject element;
   MDagPath path;

   unsigned int l = list.length();
   if (l > 0)
   {
      // The code from CNodeTranslator::RequestUpdate was duplicated here
      // we're now just calling the base class. Only difference is that
      // CArnoldSession::Request is being called now, while before it
      // was called explicitely a few lines below. But does it make a difference,
      // since we're about to call RequestUpdate on several other translators ?
      CNodeTranslator::RequestUpdate();
      
      for (unsigned int i=0; i<l; i++)
      {
         if (MStatus::kSuccess == list.getDagPath(i, path))
         {
            CNodeTranslator *elemTr = GetTranslator(path); 
            if (elemTr)  elemTr->RequestUpdate();

            // No need to check for shape, it's always the shape that Maya connects to light linker(s)
            // NOTE: It's preferable to group the objects in sets and connect the sets though
         }
         else if (MStatus::kSuccess == list.getDependNode(i, element))
         {
            // Should be a set of objects logically
            CNodeTranslator *elemTr = GetTranslator(element); 
            if (elemTr)  elemTr->RequestUpdate();
         }
         else
         {
            AiMsgError("[mtoa.translator.ipr] %-30s | %s: Cannot get member %i of set.",
                   GetMayaNodeName().asChar(), GetTranslatorName().asChar(), i);
         }
      }
      // removed the explicit call to CArnoldSession::RequestUpdate 
      // as this is being done by each of the calls to CNodeTranslator::RequestUpdate Above

   }

}

unsigned int CLightLinkerTranslator::GetMembers(MSelectionList &list, const MPlug &plug, bool doLights, bool doObjects)
{
   MStatus stat;
   unsigned int count = list.length();

   MPlugArray inConnections;
   MPlug elt, child, msg;
   unsigned int cs = static_cast<unsigned int>(!doLights);
   unsigned int ce = static_cast<unsigned int>(doLights + 2 * doObjects);
   unsigned int ne = plug.numElements();
   for (unsigned int i=0; i<ne; i++)
   {
      elt = plug.elementByPhysicalIndex(i);
      for (unsigned int c=cs; c<ce; c++)
      {
         child = elt.child(c);
         child.connectedTo(inConnections, true, false, &stat);
         unsigned int nc = inConnections.length();
         for (unsigned int j=0; j<nc; j++)
         {
            stat = list.add(inConnections[j].node(), true);
            CHECK_MSTATUS(stat);
         }
      }
   }

   return (list.length() - count);
}

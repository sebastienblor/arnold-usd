#include "ArnoldLightLinks.h"

#include "SessionOptions.h"

#include <maya/MSelectionList.h>
#include <maya/MObject.h>
#include <maya/MFnDependencyNode.h>
#include <maya/MItDependencyNodes.h>
#include <maya/MPlugArray.h>
#include <maya/MPlug.h>
#include <maya/MFnSet.h>

#include <ai.h>

#include <algorithm>
#include <cstring>

// Little trick to speed-up erasure from a vector (at the expense of ordering)
template <class T>
static inline
void LightLinks_fastErase(std::vector<T>& vec, const typename std::vector<T>::iterator& it)
{
    std::swap(*it, vec.back());
    vec.pop_back();
}

/**
 *   Clear the vectors used here to store
 *   Light-linking informations.
 *   Notice that clearing the std::vector won't actually
 *   release the memory. So pushing lights in them
 *   won't allocate memory for each shape
 **/
void CArnoldLightLinks::ClearLightLinks()
{
   m_arnoldLights.clear();
   m_arnoldDefaultLights.clear();
   m_arnoldMeshLights.clear();
   m_numArnoldLights = 0;
   m_linkedLights.clear();
   m_linkedShadows.clear();
   m_ignoredLights.clear();
   m_ignoredShadows.clear();
   m_cachedObjectSets.clear();
   m_groupLights.clear();
   
   m_lightMode = MTOA_LIGHTLINK_NONE;
   m_shadowMode = MTOA_SHADOWLINK_NONE;
}

/** 
 *  Set the Linking modes. 
 *  Light-linking can follow Maya's light-linking or ignore it
 *  Shadow-linking can either follow Maya's shadow-linking, or ignore it, or match light-linking
 **/
void CArnoldLightLinks::SetLinkingMode(int light, int shadow)
{
   m_lightMode = light;
   m_shadowMode = shadow;
}

/**
 *   Parse All lights in the Arnold scene, and create
 *   a hash map to access each AtNode quickly by its name
 **/
void CArnoldLightLinks::ParseLights()
{
   AtNodeIterator* niter = AiUniverseGetNodeIterator(AI_NODE_LIGHT);
   // loop over all lights
   while(!AiNodeIteratorFinished(niter))
   {
      AtNode* node = AiNodeIteratorGetNext(niter);
      m_arnoldLights[AiNodeGetName(node)] = node;

      // keep a list of all Mesh-Lights in our scene, 
      // as they have to be treated in a specific way 
      // (they don't correspond to a light in maya)
      if (AiNodeIs(node, "mesh_light")) m_arnoldMeshLights.push_back(node);
   }
   AiNodeIteratorDestroy(niter);
   m_numArnoldLights = m_arnoldLights.size();
   if (m_numArnoldLights)
   {
      // These lists will be cleared and filled for each shape
      m_linkedLights.reserve(m_numArnoldLights);
      m_linkedShadows.reserve(m_numArnoldLights);
      m_ignoredLights.reserve(m_numArnoldLights);
      m_ignoredShadows.reserve(m_numArnoldLights);
      m_groupLights.reserve(m_numArnoldLights);
   }

   // make sure ArnoldDefaultLights list is built before we start
   MSelectionList slist;
   slist.add("defaultLightSet");
   if (!slist.isEmpty())
   {
      MObject nodeObj;
      slist.getDependNode (0, nodeObj);
      MFnDependencyNode dependNode(nodeObj);
      GetObjectsFromObjectSet(dependNode);
   }
}

/**
 *  Provide a LightSet Maya's Node, and return the list of corresponding Arnold Lights.
 *  Store this list in a hash map, by the group name
 **/
const std::vector<AtNode*>& CArnoldLightLinks::GetObjectsFromObjectSet(MFnDependencyNode& objectSet)
{
   std::string setName = objectSet.name().asChar();
   std::map<std::string, std::vector<AtNode*> >::iterator it = m_cachedObjectSets.find(setName);
   if (it == m_cachedObjectSets.end())
   {      
      std::vector<AtNode*> lights;
      lights.reserve(m_numArnoldLights);

      if (!m_arnoldDefaultLights.empty() && setName == "defaultLightSet")
      {
         // this set is called defaultLightSet
         // which is Maya's hardcoded name for a set containing ALL lights in the scene.
         // so instead of parsing all lights from this set, let's just copy our Arnold's Default Light list.
         // this list also contains the Mesh Lights, which wouldn't appear in Maya's set list.
         // Only do this if the m_defaultArnoldLights list has already been filled
         lights = m_arnoldDefaultLights;
      } else
      {
         MFnSet mayaObjectSet(objectSet.object());
         MSelectionList sList;
         mayaObjectSet.getMembers(sList, true);
         MStatus status;
         for (unsigned int i = 0; i < sList.length(); ++i)
         {
            MDagPath dgPath;
            if (!sList.getDagPath(i, dgPath))
               continue;
            unsigned int childCount = dgPath.childCount();
            for (unsigned int child = 0; child < childCount; ++child)
            {
               MObject childObject = dgPath.child(child);
               MDagPath childPath;
               status = MDagPath::getAPathTo(childObject, childPath);
               if (!status)
                  continue;            
               MFnDependencyNode linkedLight(childPath.node(), &status);            
               std::map<std::string, AtNode*>::iterator it2 = m_arnoldLights.find(linkedLight.name().asChar());
               if (it2 == m_arnoldLights.end())
                  it2 = m_arnoldLights.find(childPath.partialPathName().asChar()); //if the shapeName is not unique we are using the full path name
               if (it2 == m_arnoldLights.end())
                  it2 = m_arnoldLights.find(childPath.partialPathName().asChar() + 1); //if the shapeName is not unique we are using the full path name
               if (it2 == m_arnoldLights.end())
                  it2 = m_arnoldLights.find(childPath.fullPathName().asChar()); //if the shapeName is not unique we are using the full path name
               if (it2 == m_arnoldLights.end())
                  it2 = m_arnoldLights.find(childPath.fullPathName().asChar() + 1); //if the shapeName is not unique we are using the full path name
               if (it2 != m_arnoldLights.end())
                  lights.push_back(it2->second);               
            }
         }
        
         // note that lights having illuminate_by_default set to Off won't be in this list
         // we have to add all mesh lights that are not present in defaultLightSet
         if (setName == "defaultLightSet")
         {
            // append the mesh lights as they're not included in "defaultLightSet"
            if (!m_arnoldMeshLights.empty()) 
            {
               // Note that using the new system (#2385), mesh lights are now part of maya lights and no longer need to be 
               // considered separately.
               // So we shouldn't duplicate them here

               // Since old method is still supported, we need to loop over mesh lights to see if they're already in the list,
               // but this might be overkill on big scenes

               // so we first count the amount of meshLights in current list.
               int prevMeshLightCount = 0;
               for (size_t i = 0; i < lights.size(); ++i)
               {
                  if (AiNodeIs(lights[i], "mesh_light")) prevMeshLightCount++;
               }

               // if this amount is the same as our "mesh lights" list, then the user
               // is exclusively relying on the new method

               // otherwise, we need to add the missing ones.

               if (prevMeshLightCount == 0)
               {
                  // only using the old method
                  // copy all at once to optimize
                  lights.insert(lights.end(), m_arnoldMeshLights.begin(), m_arnoldMeshLights.end());

               } else if (prevMeshLightCount < (int) m_arnoldMeshLights.size())
               {
                  // we're using both methods in this scene
                  lights.reserve(lights.size() + m_arnoldMeshLights.size() - prevMeshLightCount);
                  for (size_t i = 0; i < m_arnoldMeshLights.size(); ++i)
                  {
                     if (std::find(lights.begin(), lights.end(), m_arnoldMeshLights[i]) == lights.end()) lights.push_back(m_arnoldMeshLights[i]);
                  }
               }
            }

            // if it's the first time defaultLightSet's lights list is filled,
            // keep the list in m_arnoldDefaultLights

            if (m_arnoldDefaultLights.empty()) m_arnoldDefaultLights = lights;
         }
      }
      m_cachedObjectSets.insert(std::make_pair(setName, lights));
      return m_cachedObjectSets[setName];
   }
   else return it->second;
}

void CArnoldLightLinks::AppendNodesToList(MFnDependencyNode& targetNode, std::vector<std::string>& nodeList, const std::vector<std::string> *existingList)
{
   // we can't directly "flatten" the ObjectSets as a list of lights
   // because they are considered differently as independent lights 
   // in the priority order.
   // So we have to store the group Names
   // and mess with the logic later

   if (targetNode.typeName() == MString("objectSet"))
   {

      std::string setName = targetNode.name().asChar();
      if (setName.empty()) return; //can this happen ?

      if (std::find(nodeList.begin(), nodeList.end(), setName) == nodeList.end())
      {
         if (existingList)
         {
            // if this list is provided
            // let's check if this item is already present here
            if (std::find(existingList->begin(), existingList->end(), setName) != existingList->end()) return;
         }

         nodeList.push_back(setName); 

         // add this objectSet to m_cachedObjectsSet
         GetObjectsFromObjectSet(targetNode); 
         
      }
   }
   else
   {
      std::string lightName = targetNode.name().asChar();
      if(lightName.empty()) return; // can this happen ?

      if (std::find(nodeList.begin(), nodeList.end(), lightName) == nodeList.end())
      {
         if (existingList)
         {
            // if this list is provided
            // let's check if this item is already present here
            if (std::find(existingList->begin(), existingList->end(), lightName) != existingList->end()) return;
         }
         std::map<std::string, AtNode*>::iterator it = m_arnoldLights.find(lightName);
         if (it == m_arnoldLights.end())
         {
            //if the shapeName is not unique we are testing the full path name
            MDagPath dgPath;
            MDagPath::getAPathTo(targetNode.object(), dgPath);
            lightName = dgPath.fullPathName().asChar();
            it = m_arnoldLights.find(lightName); 
            if (it == m_arnoldLights.end())
            {
               lightName = (dgPath.fullPathName().asChar() + 1);
               it = m_arnoldLights.find(lightName); 
               if (it == m_arnoldLights.end()) return;
            }
         }
         nodeList.push_back(lightName);
      }
   }
}

void CArnoldLightLinks::HandleLightLinker(MPlug& conn, bool checkExisting)
{
   MPlug parentPlug = conn.parent();
   std::string plugName = parentPlug.partialName(false, false, false, false, false, true).asChar();
   // sometimes this buffer is invalidated

   static MPlugArray conns2;
   parentPlug.child(0).connectedTo(conns2, true, false);

   if (conns2.length() == 0)
      return;

   MFnDependencyNode targetNode(conns2[0].node());

   // check the message type 
   if (strncmp(plugName.c_str(), "link", 4) == 0) AppendNodesToList(targetNode, m_linkedLights, (checkExisting) ? &m_ignoredLights : 0);
   else if (strncmp(plugName.c_str(), "ignore", 6) == 0) AppendNodesToList(targetNode, m_ignoredLights, (checkExisting) ? &m_linkedLights : 0);
   else if (strncmp(plugName.c_str(), "shadowLink", 10) == 0) AppendNodesToList(targetNode, m_linkedShadows, (checkExisting) ? &m_ignoredShadows : 0);
   else if (strncmp(plugName.c_str(), "shadowIgnore", 12) == 0) AppendNodesToList(targetNode, m_ignoredShadows, (checkExisting) ? &m_linkedShadows : 0);
  
}

bool CArnoldLightLinks::CheckMessage(MFnDependencyNode& dNode, bool checkExisting)
{
   // attribute "message" contains the linking information
   // for this MFnDependencyNode
   MPlug messagePlug = dNode.findPlug("message");
   static MPlugArray conns;
   messagePlug.connectedTo(conns, false, true);
   unsigned int numConnections = conns.length();
   bool ret = false;
   for (unsigned int i = 0; i < numConnections; ++i)
   {
      MPlug conn = conns[i];
      if (conn.node().hasFn(MFn::kLightLink))
      {
         // this is a light-linkging / shadow-linking message
         HandleLightLinker(conn, checkExisting);
         ret = true;
      }
   }
   return ret;
}

void CArnoldLightLinks::CheckNode(MObject node)
{
   static MPlugArray conns;

   MStatus status;     
   MFnDependencyNode dNode(node, &status);
   if (!status)
      return;
   
   CheckMessage(dNode); // checking the outgoing message
   // for the node
   
   MPlug instObjGroupsPlug = dNode.findPlug("instObjGroups", &status);
   instObjGroupsPlug = instObjGroupsPlug.elementByLogicalIndex(0);
   
   instObjGroupsPlug.connectedTo(conns, false, true);
   unsigned int numConnections = conns.length();
   // check for the per face assignment light linking
   // only check for the first one
   // because we don't support per face light linking
   if (numConnections == 0)
   {
      instObjGroupsPlug = instObjGroupsPlug.child(0, &status);
      if (status)
      {
         static MIntArray indicesArray;
         instObjGroupsPlug.getExistingArrayAttributeIndices(indicesArray);
         unsigned int numElements = indicesArray.length();
         for (unsigned int id = 0; id < numElements; ++id)
         {
            MPlug instObjGroupsPlugElement = instObjGroupsPlug.elementByLogicalIndex(indicesArray[id]);
            static MPlugArray conns2;
            instObjGroupsPlugElement.connectedTo(conns2, false, true);
            for(unsigned int id2 = 0; id2 < conns2.length(); ++id2)
            {
               conns.append(conns2[id2]);
               ++numConnections;
            }
         }
      }
   }

   for (unsigned int i = 0; i < numConnections; ++i)
   {      
      MPlug conn = conns[i];
      MObject outObject = conn.node();
      if (outObject.hasFn(MFn::kShadingEngine))
      {
         // checking the outgoing message
         // for the shadingEngine
         MFnDependencyNode shadingEngineNode(outObject);

         // the flag (checkExsting = TRUE) means that we will skip any item
         // that has been previously considered.
         // i.e. if a light is linked to its shape, ignoring it here through 
         // its shadingEngine must not be taken into account
         if (CheckMessage(shadingEngineNode, true))
         {
            // we don't support per-face light linking
            // so if anything was set here, let's get out
            if (!m_linkedLights.empty() || !m_linkedShadows.empty() ||
               !m_ignoredLights.empty() ||!m_ignoredLights.empty()) break;
         }
      }
      else
      {
         MFnDependencyNode outObjectNode(outObject);
         if (outObjectNode.typeName() == MString("objectSet"))
         {
            // checking the outgoing message
            // if it's an objectSet (this is for standins)

            if (CheckMessage(outObjectNode, true)) // should I check here the existing items ?
            {
               // we don't support per-face light linking
               // so if anything was set here, let's get out
               if (!m_linkedLights.empty() || !m_linkedShadows.empty() ||
               !m_ignoredLights.empty() ||!m_ignoredLights.empty()) break;
            }
         }
      }
   }
}

/**
 * Handle a list of linked lights and ignored lights, where Lights can also be LightSets
 * with a bunch of priority orders.
 * After some tests on .ma files here is what how it seems to behave on Maya Software :
 * - if something is linked to a shape, but also ignored -> ignore wins
 * - if something is linked/ignored to a shape, but also with the shadingEngine -> shape wins
 * - if a light is linked/ignored, but also a LightSet containing this light -> individual light information wins
 * - if something is linked/ignored to a shape, but also to a Group/Set containing it -> shape information wins
 **/

bool CArnoldLightLinks::FillLights(const std::vector<std::string> &linkList, const std::vector<std::string> &ignoreList)
{
   //clear m_groupLights before filling it
   m_groupLights.clear();

   // we could test equality, but for safety we're testing >=
   bool defaultAllLights = (m_arnoldDefaultLights.size() >= m_arnoldLights.size());

   // If no information was stored, we don't need to set anything
   if (linkList.empty() && ignoreList.empty() &&  defaultAllLights) return false;

   // if there is only a single link to "defaultLightSet", then our shape
   // is linked ot all lights in the scene and there's 
   // no need to use Arnold's light groups
   // Exception : if some lights have illuminate by default set to OFF, then we still need to use light_groups
   if (linkList.size() == 1 && ignoreList.empty() && linkList[0] == "defaultLightSet" && defaultAllLights) return false;
   
   if(linkList.empty())
   {
      // no linked lights specified, so
      // consider ALL lights but the ignored ones
      if (defaultAllLights)
      {
         m_groupLights.reserve(m_arnoldLights.size());

         for (std::map<std::string, AtNode*>::iterator it = m_arnoldLights.begin(); it != m_arnoldLights.end(); ++it)
         {
            m_groupLights.push_back(it->second);
         }
      } else
      {
         m_groupLights = m_arnoldDefaultLights;
      }
   }

   //=== First add the ObjectsSet linked lights
   for (size_t i = 0; i < linkList.size(); ++i)
   {
      const std::string &setName = linkList[i];
      std::map<std::string, std::vector<AtNode*> >::iterator it = m_cachedObjectSets.find(setName);
      if (it == m_cachedObjectSets.end()) continue;
   
      // This is an ObjectSet (Sets can't have the same name as Lights)
      const std::vector<AtNode*> &setLights = it->second;
      for (unsigned int j = 0; j < setLights.size(); ++j)
      {
         if (std::find(m_groupLights.begin(), m_groupLights.end(), setLights[j]) == m_groupLights.end())
            m_groupLights.push_back(setLights[j]);
      }
   }

   //==== Then remove the ObjectsSet ignored lights
   for (size_t i = 0; i < ignoreList.size(); ++i)
   {
      const std::string &setName = ignoreList[i];
      std::map<std::string, std::vector<AtNode*> >::iterator it = m_cachedObjectSets.find(setName);
      if (it == m_cachedObjectSets.end()) continue;

      // This is an ObjectSet (Sets can't have the same name as Lights)
      std::vector<AtNode*> &setLights = it->second;
      for (size_t j = 0; j < setLights.size(); ++j)
      {
         std::vector<AtNode*>::iterator it = std::find(m_groupLights.begin(), m_groupLights.end(), setLights[j]);
         if (it != m_groupLights.end()) LightLinks_fastErase(m_groupLights, it);
      }
   }

   //===== Then add the linked lights
   for (size_t i = 0; i < linkList.size(); ++i)
   {
      const std::string &lightName = linkList[i];
      // skip objectSets
      if (m_cachedObjectSets.find(lightName) != m_cachedObjectSets.end()) continue;
      
      std::map<std::string, AtNode*>::iterator it = m_arnoldLights.find(lightName);
      if (it != m_arnoldLights.end() &&
         std::find(m_groupLights.begin(), m_groupLights.end(), it->second) == m_groupLights.end())
      {
         m_groupLights.push_back(it->second);
      }
   }

   //====== Then remove the ignored lights
   for (size_t i = 0; i < ignoreList.size(); ++i)
   {
      const std::string &lightName = ignoreList[i];
      //skip ObjectSets
      if (m_cachedObjectSets.find(lightName) != m_cachedObjectSets.end()) continue;

      std::map<std::string, AtNode*>::iterator it = m_arnoldLights.find(lightName);
      if (it != m_arnoldLights.end())
      {
         std::vector<AtNode*>::iterator nodeIt = std::find(m_groupLights.begin(), m_groupLights.end(), it->second);
         if( nodeIt != m_groupLights.end()) LightLinks_fastErase(m_groupLights, nodeIt);
      }
   }

   // maybe we should compare the amount of lights in m_groupLights 
   // with m_numArnoldLights to return false if they're equal ?

   return true;
}

/**
 *   Export the Light-linking data for current shape
 **/
void CArnoldLightLinks::ExportLightLinking(AtNode* shape, const MDagPath& path)
{
   if ((m_lightMode == MTOA_LIGHTLINK_NONE && m_shadowMode == MTOA_SHADOWLINK_NONE) ||
           m_numArnoldLights == 0)
      return;
   MDagPath pathCopy = path;

   // clear all the temporary lists
   m_linkedLights.clear();
   m_ignoredLights.clear();
   m_linkedShadows.clear();
   m_ignoredShadows.clear();
   m_groupLights.clear();

   CheckNode(pathCopy.node());
   // it looks like here we're checking the same node twice.
   // we should propably do pathCopy.pop() here
   while (pathCopy.length())
   {
      CheckNode(pathCopy.transform());
      pathCopy.pop();
   }
   
   if (m_lightMode == MTOA_LIGHTLINK_MAYA)
   {   
      // Follow Maya's light linking in Arnold
      if (FillLights(m_linkedLights, m_ignoredLights))
      {
          AiNodeSetBool(shape, "use_light_group", true);

         // m_groupLights contains now the exact list of lights to be applied to current Shape
         AtArray* lightsArray = AiArrayAllocate((AtUInt32)m_groupLights.size(), 1, AI_TYPE_NODE);
         for (size_t i = 0; i < m_groupLights.size(); ++i)            
            AiArraySetPtr(lightsArray, (AtUInt32)i, m_groupLights[i]);

         AiNodeSetArray(shape, "light_group", lightsArray);

         if (m_shadowMode == MTOA_SHADOWLINK_LIGHT)
         {
            // copy light linking to shadow linking
            AiNodeSetBool(shape, "use_shadow_group", true);
            AiNodeSetArray(shape, "shadow_group", AiArrayCopy(lightsArray));
         }            
      }
   }

   if (m_shadowMode == MTOA_SHADOWLINK_MAYA)
   {
      //Follow Shadow linking from Maya
      if (FillLights(m_linkedShadows, m_ignoredShadows))
      {
         AiNodeSetBool(shape, "use_shadow_group", true);

         // m_groupLights contains now the exact list of lights to be applied to current Shape
         AtArray* lightsArray = AiArrayAllocate((AtUInt32)m_groupLights.size(), 1, AI_TYPE_NODE);
         for (size_t i = 0; i < m_groupLights.size(); ++i)            
            AiArraySetPtr(lightsArray, (AtUInt32)i, m_groupLights[i]);

         AiNodeSetArray(shape, "shadow_group", lightsArray);
      }
   }
}

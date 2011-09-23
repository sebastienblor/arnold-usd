#ifndef MOBJECTCOMPARE_H
#define MOBJECTCOMPARE_H

#include <maya/MObjectHandle.h>
#include <maya/MDagPath.h>
#include <maya/MPlug.h>
#include <maya/MFnAttribute.h>
#include <maya/MString.h>
#include <maya/MFnDependencyNode.h>

struct MObjectCompare
{
   bool operator()(MObjectHandle h1, MObjectHandle h2) const
   {
      return h1.hashCode() < h2.hashCode();
   }
};

/// Designed to serve as a key for a std::multimap.

/// The key can be as specific or generic as desired, specifying at least a depend node MObject,
/// and optionally a DAG instance number and/or attribute name. When comparing two instances of
/// CNodeAttrHandle, if all the properties of the second instance are matched, they are considered
/// equal.
class CNodeAttrHandle
{
public:
   CNodeAttrHandle(const MObject& nodeObject, const MString& attrName) :
         m_nodeHandle(nodeObject),
         m_attrName(attrName),
         m_instanceNum(-1)
   {}
   CNodeAttrHandle(const MPlug& plug, int instanceNum=-1) :
         m_nodeHandle(plug.node()),
         m_attrName(plug.partialName(false, false, false, false, false, true)),
         m_instanceNum(instanceNum)
   {}
   CNodeAttrHandle(const MDagPath& dagPath) :
         m_nodeHandle(dagPath.node()),
         m_attrName(""),
         m_instanceNum(dagPath.instanceNumber())
   {}
   // match any attribute
   CNodeAttrHandle(const MObject& nodeObject) :
         m_nodeHandle(nodeObject),
         m_attrName(""),
         m_instanceNum(-1)
   {}
   bool operator<(CNodeAttrHandle other) const
   {
      // check if same node
      if (m_nodeHandle.object() == other.m_nodeHandle.object())
      {
         // only check instance if other provided
         if (other.m_instanceNum >= 0)
         {
            // test if same dag node
            if (m_instanceNum == other.m_instanceNum)
            {
               // only check attributes if other provided
               if (other.m_attrName.length())
               {
                  // strcmp returns 0 if equal, or > 0 if str1 is greater than str2 (i.e. str2 is less than str1)
                  return strcmp(m_attrName.asChar(), other.m_attrName.asChar()) > 0;
               }
            }
            return (m_instanceNum < other.m_instanceNum);
         }
         // only check attributes if other provided
         if (other.m_attrName.length())
         {
            // strcmp returns 0 if equal, or > 0 if str1 is greater than str2 (i.e. str2 is less than str1)
            return strcmp(m_attrName.asChar(), other.m_attrName.asChar()) > 0;
         }
         return false; // they are same depend node
      }
      return m_nodeHandle.hashCode() < other.m_nodeHandle.hashCode();
   }

   bool operator==(CNodeAttrHandle other) const
   {
      // check if same node
      if (m_nodeHandle.object() == other.m_nodeHandle.object())
      {
         // only check instance if other provided
         if (other.m_instanceNum >= 0)
         {
            // test if same dag node
            if (m_instanceNum == other.m_instanceNum)
            {
               // only check attributes if other provided
               if (other.m_attrName.length())
                  // check if same attribute
                  return (m_attrName == other.m_attrName);
               return true; // they are same dag node
            }
            return false;
         }
         // only check attributes if other provided
         if (other.m_attrName.length())
            // check if same attribute
            return (m_attrName == other.m_attrName);
         return true; // they are same depend node
      }
      return false;
   }
public :
   const MObjectHandle m_nodeHandle;
   const MString m_attrName;
   const int m_instanceNum;
};

#endif // MOBJECTCOMPARE_H

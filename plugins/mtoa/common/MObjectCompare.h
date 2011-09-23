#ifndef MOBJECTCOMPARE_H
#define MOBJECTCOMPARE_H

#include <maya/MObjectHandle.h>
#include <maya/MFnAttribute.h>
#include <maya/MPlug.h>

struct MObjectCompare
{
   bool operator()(MObjectHandle h1, MObjectHandle h2) const
   {
      return h1.hashCode() < h2.hashCode();
   }
};

class CNodeAttrHandle
{
public:
   CNodeAttrHandle(const MObject& nodeObject, const MObject& attrObject) :
         m_nodeHandle(nodeObject),
         m_attrHandle(attrObject)
   {}
   CNodeAttrHandle(const MPlug& plug) :
         m_nodeHandle(plug.node()),
         m_attrHandle(plug.attribute())
   {}
   // match any attribute
   CNodeAttrHandle(const MObject& nodeObject) :
         m_nodeHandle(nodeObject)
   {}
   bool operator<(CNodeAttrHandle other) const
   {
      // check if same node
      if (m_nodeHandle.hashCode() == other.m_nodeHandle.hashCode())
      {
         // check if same attribute
         if (m_attrHandle.isValid() && other.m_attrHandle.isValid())
            return (m_attrHandle.hashCode() < other.m_attrHandle.hashCode());
         return false; // they are equal
      }
      return m_nodeHandle.hashCode() < other.m_nodeHandle.hashCode();
   }
   bool operator==(CNodeAttrHandle other) const
   {
      // check if same node
      if (m_nodeHandle.object() == other.m_nodeHandle.object())
      {
         // check if same attribute
         if (m_attrHandle.isValid() && other.m_attrHandle.isValid())
            return (m_attrHandle.object() == other.m_attrHandle.object());
         return true; // they are equal
      }
      return false;
   }
public :
   const MObjectHandle m_nodeHandle;
   const MObjectHandle m_attrHandle;
};

#endif // MOBJECTCOMPARE_H

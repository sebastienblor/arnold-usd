#pragma once
#include "../NodeTranslatorImpl.h"


class CShadingEngineTranslatorImpl : public CNodeTranslatorImpl
{
public :
   CShadingEngineTranslatorImpl(CNodeTranslator &translator) :
      CNodeTranslatorImpl(translator) {}
   ~CShadingEngineTranslatorImpl() {}

   virtual void ComputeAOVs()
   {

      MPlugArray connections;
      // loop through and export custom AOV networks
      MPlug arrayPlug = m_tr.FindMayaPlug("aiCustomAOVs");
      for (unsigned int i = 0; i < arrayPlug.numElements (); i++)
      {
         MPlug msgPlug = arrayPlug[i].child(1);
         msgPlug.connectedTo(connections, true, false);
         if (connections.length() > 0)
         {
            CAOV aov;
            MString value = arrayPlug[i].child(0).asString();
            aov.SetName(value);
            if (m_session->IsActiveAOV(aov))
            {
               m_localAOVs.insert(aov);
               m_customAOVPlugs.append(connections[0]);
            }
         }
      }
   }
   MPlugArray m_customAOVPlugs;
};

#ifndef EVENT_H 
#define EVENT_H

#include <windows.h>

class CEvent
{

public:

   CEvent(bool manual = true, bool initialValue = true)
   {
      m_event = CreateEvent(NULL, manual ? TRUE : FALSE, initialValue ? TRUE : FALSE, NULL);
   }

   ~CEvent()
   {
      CloseHandle(m_event);
   }

   void set()
   {
      SetEvent(m_event);
   }

   void unset()
   {
      ResetEvent(m_event);
   }

   bool wait(unsigned int ms = UINT_MAX)
   {
      return (WaitForSingleObject(m_event, static_cast<DWORD>(ms)) == WAIT_OBJECT_0);
   }

private:

   HANDLE   m_event;

};  // class CEvent

#endif // EVENT_H

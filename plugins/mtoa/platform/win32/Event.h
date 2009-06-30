#ifndef EVENT_H 
#define EVENT_H

#include <windows.h>

class CEvent
{

public:

   CEvent( bool manual = true, bool initialValue = true )
   {
      m_event = CreateEvent( NULL, manual ? TRUE : FALSE, initialValue ? TRUE : FALSE, NULL );
   }

   ~CEvent( void )
   {
      CloseHandle( m_event );
   }

   void set( void )
   {
      SetEvent( m_event );
   }

   void unset( void )
   {
      ResetEvent( m_event );
   }

   bool wait( DWORD ms = INFINITE )
   {
      return ( WaitForSingleObject( m_event, ms ) == WAIT_OBJECT_0 );
   }

private:

   HANDLE   m_event;

};  // class CEvent

#endif // EVENT_H

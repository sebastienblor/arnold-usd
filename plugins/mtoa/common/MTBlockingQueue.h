#ifndef MT_BLOCKING_QUEUE_H
#define MT_BLOCKING_QUEUE_H

#include "platform/Platform.h"
#include <ai_critsec.h>
#include <ai_types.h>
#include <queue>

template <typename T>
class CMTBlockingQueue
{

public:

   CMTBlockingQueue(AtUInt32 maxSize = 0)
   : m_maxSize(maxSize)
   , m_notEmpty(true, false)
   , m_notFull(true, true)
   {
      AiCritSecInit(&m_accessCritSec);
   }

   ~CMTBlockingQueue()
   {
      AiCritSecClose(&m_accessCritSec);
   }

   void reset()
   {
      AiCritSecEnter(&m_accessCritSec);
      m_notEmpty.unset();
      m_notFull.set();
      while (!m_queue.empty())
         m_queue.pop();
      AiCritSecLeave(&m_accessCritSec);
   }

   bool push(const T& data)
   {
      AiCritSecEnter(&m_accessCritSec);
      bool result = false;
      if (!isFull())
      {
         m_queue.push(data);
         result = true;
      }
      AiCritSecLeave(&m_accessCritSec);

      if (!isEmpty())
         m_notEmpty.set();
      if (isFull())
         m_notFull.unset();

      return result;
   }

   bool pop(T& data)
   {
      AiCritSecEnter(&m_accessCritSec);
      bool result = false;
      if (!isEmpty())
      {
         data = m_queue.front();
         m_queue.pop();
         result = true;
      }
      AiCritSecLeave(&m_accessCritSec);

      if (isEmpty())
         m_notEmpty.unset();
      if (!isFull())
         m_notFull.set();

      return result;
   }

   bool waitForNotEmpty(DWORD timeout = INFINITE)
   {
      return m_notEmpty.wait(timeout);
   }

   bool waitForNotFull(DWORD timeout = INFINITE)
   {
      return m_notFull.wait(timeout);
   }

   bool isEmpty() const
   {
      return m_queue.empty();
   }

   bool isFull() const
   {
      return ((m_maxSize > 0) && (m_queue.size() >= m_maxSize));
   }

protected:

   std::queue<T> m_queue;
   AtUInt32      m_maxSize;

   AtCritSec     m_accessCritSec;
   CEvent        m_notEmpty;
   CEvent        m_notFull;

}; // class CMTBlockingQueue

#endif // MT_BLOCKING_QUEUE_H

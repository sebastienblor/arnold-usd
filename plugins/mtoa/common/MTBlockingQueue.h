#ifndef MTBLOCKINGQUEUE_H
#define MTBLOCKINGQUEUE_H

#include "platform/Platform.h"
#include <ai_critsec.h>
#include <ai_types.h>
#include <queue>
#include <climits>

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

   void clear()
   {
      AiCritSecEnter(&m_accessCritSec);
      m_notEmpty.unset();
      m_notFull.set();
      while (!m_queue.empty())
      {
         m_queue.front().unlock();
         m_queue.pop();
      }
      AiCritSecLeave(&m_accessCritSec);
   }

   bool push(T& data)
   {
      AiCritSecEnter(&m_accessCritSec);
      bool result = false;
      if (!isFull())
      {
         data.lock();
         m_queue.push(data);
         m_queue.front().unlock();
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
         m_queue.front().lock();
         data = m_queue.front();
         m_queue.pop();
         data.unlock();
         result = true;
      }
      AiCritSecLeave(&m_accessCritSec);

      if (isEmpty())
         m_notEmpty.unset();
      if (!isFull())
         m_notFull.set();

      return result;
   }

   bool waitForNotEmpty(unsigned int timeout = UINT_MAX)
   {
      return m_notEmpty.wait(timeout);
   }

   bool waitForNotFull(unsigned int timeout = UINT_MAX)
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

   size_t size()
   {
      return m_queue.size();
   }

   
protected:

   std::queue<T> m_queue;
   AtUInt32      m_maxSize;

   AtCritSec     m_accessCritSec;
   CEvent        m_notEmpty;
   CEvent        m_notFull;

}; // class CMTBlockingQueue

#endif // MTBLOCKINGQUEUE_H

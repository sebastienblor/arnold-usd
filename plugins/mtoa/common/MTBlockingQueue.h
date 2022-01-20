#pragma once


#include "platform/Platform.h"
#include <ai_critsec.h>
#include <queue>
#include <climits>

template <typename T>
class CMTBlockingQueue
{

public:

   CMTBlockingQueue(unsigned int maxSize = 0)
   : m_maxSize(maxSize)
   , m_notEmpty(true, false)
   , m_notFull(true, true)
   {
   }

   ~CMTBlockingQueue()
   {
   }

   void clear()
   {
      std::lock_guard<AtMutex> guard(m_accessCritSec);
      m_notEmpty.unset();
      m_notFull.set();
      while (!m_queue.empty())
      {
         if (m_queue.front().pixels != NULL)
         {
            delete[] m_queue.front().pixels;
         }
         m_queue.pop();
      }
   }

   bool push(T& data)
   {
      std::lock_guard<AtMutex> guard(m_accessCritSec);
      bool result = false;
      if (!isFull())
      {
         m_queue.push(data);
         result = true;
      }

      if (!isEmpty())
         m_notEmpty.set();
      if (isFull())
         m_notFull.unset();

      return result;
   }

   bool pop(T& data)
   {
      bool result = false;
      std::lock_guard<AtMutex> guard(m_accessCritSec);
      if (!isEmpty())
      {
         data = m_queue.front();
         m_queue.pop();
         result = true;
      }
   

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
   unsigned int  m_maxSize;

   AtMutex       m_accessCritSec;
   CEvent        m_notEmpty;
   CEvent        m_notFull;

}; // class CMTBlockingQueue

#pragma once

#ifdef _WIN32
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#else
#include <sys/time.h>
#endif

#include <sys/timeb.h>
#include <stdio.h>
#include <ai_nodes.h>
#include <ai_node_entry.h>

#define MAX_NAME_SIZE 65535

inline size_t MtoaTicks()
{
#ifdef _WIN32
   LARGE_INTEGER PerformanceCount;
   LARGE_INTEGER Frequency;

   QueryPerformanceCounter(&PerformanceCount);
   QueryPerformanceFrequency(&Frequency);

   return PerformanceCount.QuadPart / (Frequency.QuadPart / 1000000);
#else
   // UNIX platforms: Linux and OS X
   struct timeval tp;
   gettimeofday(&tp, NULL);
   return ((size_t) tp.tv_sec * 1000000) + ((size_t) tp.tv_usec);
#endif
}


inline unsigned MtoaTime()
{
#ifdef _WIN32
   struct _timeb timebuffer;

   _ftime(&timebuffer);
   return (unsigned) (timebuffer.time * 1000) + timebuffer.millitm;
#else
   // UNIX platforms: Linux and OS X
   struct timeval tp;
   gettimeofday(&tp, NULL);
   return (unsigned) ((size_t) (tp.tv_sec * 1000) + (size_t) (tp.tv_usec / 1000));
#endif
}

/*
 * Creates a unique name string with the node type followed
 * by a double time-stamp (current time in milliseconds and
 * process ellapsed time in microseconds). Example:
 *
 *    sphere_34CEAD9B00052F21
 *           \______/\______/
 *           AiTime  AiTicks
 *
 * \param      node    the node we want to give a default name to
 * \param[out] string  pre-allocated buffer for the new name string
 * \return             the new name string (again)
 */
inline char *NodeUniqueName(AtNode *node, char *string)
{
   sprintf(string, "%s_%08X%08llX",
      AiNodeEntryGetName(AiNodeGetNodeEntry(node)), MtoaTime(), MtoaTicks());
   return string;
}


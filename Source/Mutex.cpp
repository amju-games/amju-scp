/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: Mutex.cpp,v $
Revision 1.1.10.3  2006/08/15 08:25:08  jay
Fix for .i386 build

Revision 1.1.10.2  2006/07/25 08:12:25  jay
Copied latest file from Uptime

Revision 1.1  2005/01/03 11:08:53  jay
Added to Uptime Overlord project

Revision 1.1  2004/09/08 15:43:19  jay
Added to repository
  
*/

// This source code originally written by JASON COLMAN 2000-2003. 
// You may use it as you wish, at your own risk!  jason@amju.com.

// Some threading code adapted from "Portable Thread Synchronization Using C++"
// http://world.std.com/~jimf/papers/c++sync/c++sync.html

#ifdef MACOSX
// Need this for .i386 but not .ppc ??!
#include "sys/errno.h"
#endif

#include "Mutex.h"
#include "SchAssert.h"

namespace Amju
{
Mutex::Mutex() 
{
#if defined(WIN32)
  InitializeCriticalSection(&m_crit);
#else
  m_nestCount = 0;
  pthread_mutex_init(&m_crit, 0);
  // m_owner does not need to be initialised
#endif
}

Mutex::~Mutex()
{
#if defined(WIN32)
#else
  pthread_mutex_destroy(&m_crit);
#endif
}

void Mutex::Lock() 
{
#if defined(WIN32)
  EnterCriticalSection(&m_crit);
#else
  switch (pthread_mutex_trylock(&m_crit)) 
  {
  case 0:                                        
    break;
  case EBUSY:                                    
    if (m_nestCount > 0) 
    {                     
      if (m_owner == pthread_self()) 
      {       
        ++m_nestCount;
        return;
      } 
    } 
    pthread_mutex_lock(&m_crit);               
    break;
  }
  m_owner = pthread_self();                      
  m_nestCount = 1;
#endif
}

void Mutex::Unlock() 
{
#if defined(WIN32)
  LeaveCriticalSection(&m_crit);
#else
  Assert(m_owner == pthread_self());
  Assert(m_nestCount > 0);
  if (--m_nestCount == 0) 
  {                                         
    pthread_mutex_unlock(&m_crit);
  } 
#endif
}
}

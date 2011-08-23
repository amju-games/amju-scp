/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: Thread.cpp,v $
Revision 1.1.10.2  2007/07/25 07:54:00  jay
Turn off debug out in release mode

Revision 1.1.10.1  2006/07/25 08:13:02  jay
Copied latest code from Uptime

Revision 1.4  2005/02/28 20:34:23  jay
Additions and fixes for Uptime Overlord v.1.0 release

Revision 1.3  2005/01/19 10:40:50  jay
Added stop flag

Revision 1.2  2005/01/09 12:29:45  jay
Added static count for diagnosis

Revision 1.1  2005/01/03 11:08:54  jay
Added to Uptime Overlord project

Revision 1.1  2004/09/08 15:43:20  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include <iostream>
#include "Thread.h"
#include "ThreadManager.h"

#if defined(WIN32)
#include <process.h>
#else

#endif

namespace Amju
{
#if defined(WIN32)
void ThreadFunction(void *p)
#endif
#if defined(MACOSX)
void* ThreadFunction(void* p)
#endif
{
  Thread* pThread = static_cast<Thread*>(p);
  pThread->Work();
  pThread->Finish();

  // This thread has finished. Tell the ThreadManager to delete it.
  ThreadManager::Instance()->DeleteThread(pThread->GetThreadId());
  
#if defined(MACOSX)
  return (void*)0;
#endif
}

Thread::Thread() : m_stop(false), m_threadId(0), m_threadHandle(0)
{
}

Thread::~Thread()
{
int c = ThreadManager::Instance()->GetThreadCount();
#ifdef _DEBUG
std::cout << "DESTROYING THREAD " << m_threadId << " COUNT IS NOW: "  << c << "\n";
#endif
}

void Thread::Start()
{
#if defined(WIN32)

  m_threadHandle = _beginthread(&ThreadFunction, 0, this);
  m_threadId = m_threadHandle;

#else

  pthread_t thrHandle;

  pthread_attr_t attr;
  pthread_attr_init(&attr);
  pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

  if (!pthread_create(&thrHandle, &attr, &ThreadFunction, this)) 
  {
      m_threadId = (int) thrHandle;
      m_threadHandle = thrHandle;
  }
  else
  {
std::cout << "FAILED TO CREATE THREAD\n";
  }

  // Add this Thread to the ThreadManager.
  ThreadManager::Instance()->AddThread(this);

#ifdef _DEBUG
int c = ThreadManager::Instance()->GetThreadCount();
std::cout << "STARTED THREAD " << m_threadId << "  COUNT IS NOW " << c << "\n";
#endif

#endif

}

void Thread::Stop()
{
  // The idea is that subclasses periodically test this variable in
  // their Work() function.
  // This doesn't need to be locked, because it's just set once
  // and then stays set until this object is destroyed.
  m_stop = true;
}

unsigned int Thread::GetThreadId() const
{
  return m_threadId;
}
}


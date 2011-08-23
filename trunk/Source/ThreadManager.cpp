/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: ThreadManager.cpp,v $
Revision 1.1.10.1  2006/07/25 08:13:41  jay
Copied latest code from Uptime
Looks like this wasn't thread safe before!

Revision 1.2  2005/01/19 10:41:48  jay
Added GetThreadCount(), and added a mutex so add/delete/count functions
are MT-safe (!)

Revision 1.1  2005/01/03 11:08:55  jay
Added to Uptime Overlord project

Revision 1.1  2004/09/08 15:43:20  jay
Added to repository
 
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "ThreadManager.h"

namespace Amju
{
ThreadManager* ThreadManager::Instance()
{
  static ThreadManager tm;
  return &tm;
}

void ThreadManager::AddThread(SharedPtr<Thread> pThread)
{
  MutexLocker lock(m_mutex);
  m_threads[pThread->GetThreadId()] = pThread;
}

void ThreadManager::DeleteThread(int threadId)
{
  MutexLocker lock(m_mutex);
  m_threads.erase(threadId);
}

int ThreadManager::GetThreadCount() const
{
  MutexLocker lock(m_mutex);
  return m_threads.size();
}
}


/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: ThreadManager.h,v $
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

#if !defined(SCHMICKEN_THREAD_MANAGER_H_INCLUDED)
#define SCHMICKEN_THREAD_MANAGER_H_INCLUDED

#include <map>
#include "Thread.h"
#include "SharedPtr.h"
#include "Mutex.h"

namespace Amju
{
// This object (Singleton) holds all Threads currently in existence.
// When a Thread finishes executing, the ThreadManager cleans it up.
class ThreadManager
{
public:
  static ThreadManager* Instance();

  // Called by the Thread ctor.
  // This ensures that a Thread object exists for the lifetime of 
  // the thread execution.
  void AddThread(SharedPtr<Thread> pThread);

  // Called by a thread function after Work() and Finish() have completed.
  // This decrements the ref count so the Thread object can be destroyed.
  void DeleteThread(int threadId);

  int GetThreadCount() const;

private:
  // Map thread IDs to Threads.
  typedef std::map<int, SharedPtr<Thread> > ThreadMap;
  ThreadMap m_threads;

private:
  ThreadManager() {}
  ThreadManager(const ThreadManager&);
  ThreadManager& operator=(const ThreadManager&);

  mutable Mutex m_mutex;
};
}

#endif

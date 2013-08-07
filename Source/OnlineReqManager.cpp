/*
Amju Games source code (c) Copyright Jason Colman 2000-2006
$Log: OnlineReqManager.cpp,v $
Revision 1.1.2.6  2006/08/12 07:40:20  jay
Changed AddReq() to return a bool, so we know if the max number of requests
was exceeded.

Revision 1.1.2.5  2006/08/07 20:46:11  jay
Fix iterator-invalidation bug

Revision 1.1.2.4  2006/07/30 21:38:07  jay
Count requests on queue with a given name; limit number of requests of a
  given name.

Revision 1.1.2.3  2006/07/26 21:19:16  jay
Pass RCPtr, not raw ptr - seemed like a good idea but not really necessary

Revision 1.1.2.2  2006/07/25 17:45:38  jay
Fix bad call to erase; add GetName()

Revision 1.1.2.1  2006/07/25 08:16:34  jay
Added OnlineReqManager to repository

*/

#include <iostream>
#include "OnlineReqManager.h"

#ifdef _DEBUG
//#define DEBUG_ONLINE
#endif

namespace Amju
{
OnlineReq::OnlineReq(
  const std::string& url,
  HttpClient::HttpMethod method,
  const std::string& name) : 
  HttpReq(url, method),
  m_name(name)
{
#ifdef DEBUG_ONLINE
std::cout << "Creating new OnlineReq: " << name.c_str() << ": " 
  << url.c_str() << "\n";
#endif
}

const char* OnlineReq::GetName() const
{
  return m_name.c_str();
}

void OnlineReqManager::Update()
{
  // HandleResult() for a request may add another request to m_reqs.
  // That would invalidate any iterator. So make a copy of m_reqs and iterate
  // over the copy.

  OnlineReqs reqs(m_reqs);
  OnlineReqs::iterator it;
  for (it = reqs.begin(); it != reqs.end(); ++it)
  {
#ifdef DEBUG_ONLINE_UPDATE
if (!it->GetPtr())
{
  std::cout << "OnlineReq: Bad req!\n";
}
else
{
  std::cout << "OnlineReq: checking " << (*it)->GetName() << "\n";
}
#endif

    if ((*it)->IsFinished())
    {
#ifdef DEBUG_ONLINE
std::cout << "OnlineReq: " << (*it)->GetName() << " has finished, calling HandleResult..\n";
#endif
      (*it)->HandleResult();
    }
  }

  // Iterate over m_reqs deleting any reqs which have had HandleResult()
  // called by the above loop. This will work because although we copied the
  // vector, the elements are RCPtrs.
  for (it = m_reqs.begin(); it != m_reqs.end();   )
  {
    if ((*it)->IsFinished())
    {
      it = m_reqs.erase(it);
    }
    else
    {
      ++it;
    } 
  }
}

int OnlineReqManager::CountAllReqs() const
{
  return m_reqs.size();
}

int OnlineReqManager::CountReqsWithName(const std::string& name)
{
  int count = 0;
  for (OnlineReqs::iterator it = m_reqs.begin(); it != m_reqs.end(); ++it)
  {
    if ((*it)->GetName() == name)
    {
      ++count;
    }
  }
  return count;
}

bool OnlineReqManager::AddReq(RCPtr<OnlineReq> pReq, int maxWithThisName)
{
  Assert(pReq.GetPtr());

  // TODO Count how many requests already exist with pReq->GetName
  int count = CountReqsWithName(pReq->GetName());
  if (count >= maxWithThisName)
  {
#ifdef DEBUG_ONLINE
std::cout << "OnlineReq: AddReq: limit reached for " << pReq->GetName()
  << ", DISCARDING request!\n";
#endif
    return false;
  }

  // Not discarded, so start the thread
#ifdef DEBUG_ONLINE
std::cout << "OnlineReq: Creating Worker for " << pReq->GetName() << "\n";
#endif
  pReq->CreateWorker();

  m_reqs.push_back(pReq);
#ifdef DEBUG_ONLINE
std::cout << "OnlineReq: num workers is now: " << m_reqs.size() << "\n";
#endif

  return true;
}
}



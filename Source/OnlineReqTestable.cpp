/*
Amju Games source code (c) Copyright Jason Colman 2000-2006
$Log: OnlineReqTestable.cpp,v $
Revision 1.1.2.2  2006/08/28 22:04:20  Administrator
Win: Added ifdef MACOSX

Revision 1.1.2.1  2006/08/07 20:42:50  jay
Online Pool - not working yet

*/

#include <iostream>
#include "OnlineReqTestable.h"
#include "File.h"

namespace Amju
{
OnlineReqTestable::OnlineReqTestable(
  const std::string& name,
  const std::string& testData) :
  OnlineReq("<no url>", HttpClient::GET, name)
{
  SetTestData(testData);
}

void OnlineReqTestable::SetTestData(const std::string& rawHtml)
{
  m_httpResult.SetString(rawHtml);
}

void OnlineReqTestable::SetTestDataFromFile(const std::string& filename)
{
  File f;
  if (!f.OpenRead(filename))
  {
    Assert(0);
    return;
  }
  std::string s;
  f.GetDataLine(&s);
  SetTestData(s); 
}

class TestWorker : public Thread 
{
public:
  TestWorker(HttpReq* pReq) : m_pReq(pReq)
  {
  }

  virtual void Work()
  {
#ifdef MACOSX
    sleep(1); // TODO TEMP TEST
#endif

std::cout << "Test worker has finished working...\n";

    HttpResult r = m_pReq->GetResult();
    // TODO random failures too 
    r.SetSuccess(true);
    m_pReq->SetResult(r); // Sets finished flag
  }
private:
  HttpReq* m_pReq;
};

void OnlineReqTestable::CreateWorker()
{
/*
  m_pThread = new TestWorker(this);
  m_pThread->Start();
*/
  // No thread, test threading issues
  GetResult().SetSuccess(true); 
}
}



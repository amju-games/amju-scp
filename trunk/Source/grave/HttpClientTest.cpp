/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: HttpClientTest.cpp,v $
Revision 1.1.10.4  2007/05/15 12:35:36  jay
Added #define to keep sending requests. This is to test for socket leak.
(Could also be used to find thread leaks)

Revision 1.1.10.3  2006/08/28 22:05:25  Administrator
Win: Added Windows Sleep() call - TODO platform wrapper for sleep

Revision 1.1.10.2  2006/07/27 08:43:29  jay
Tests ToUrlFormat()

Revision 1.1.10.1  2006/07/25 08:11:17  jay
Tests for new Http classes

Revision 1.1  2004/09/08 15:43:19  jay
Added to repository
  
*/

#include <iostream>
#include <string>
#include "HttpClient.h"
#include "HttpReq.h"
#include "OnlineReqManager.h"
#include "SocketService.h"
#include "UrlUtils.h"

#define TEST_HTTP_CLIENT
//#define TEST_HTTP_REQ
//#define TEST_ONLINE_REQ_MAN
#define KEEP_POLLING_FOREVER

using namespace Amju;

class TestReq : public OnlineReq
{
public:
  TestReq(
    const std::string& url,
    HttpClient::HttpMethod method,
    const std::string& name) : 
    OnlineReq(url, method, name)
  {
  }

  virtual void HandleResult()
  {
    std::cout << "\n\nTestReq has finished.\n";
    if (GetResult().GetSuccess())
    {
      std::cout << "SUCCESS! Got result: " 
        << GetResult().GetString().c_str() << "\n";
    }
    else
    {
      std::cout << "FAILED: " << GetResult().GetErrorString().c_str() << "\n";
    }
#ifndef KEEP_POLLING_FOREVER
    exit(0);
#endif
  }
};

int main(int argc, char** argv)
{
  if (argc != 2)
  {
    std::cout << "Usage: HttpClientTest <url> \n";
    return 1;
  }

  SocketService ss;

  std::string url = argv[1];

  std::cout << "Raw URL: " << url.c_str() << "\n";
  std::cout << "URL format: " << ToUrlFormat(url).c_str() << "\n";

#ifdef TEST_HTTP_CLIENT
#ifdef KEEP_POLLING_FOREVER
  while (1)
#endif
  {
    HttpClient hc;
    HttpResult res = hc.Get(url, HttpClient::GET);
    std::cout << res.GetString().c_str() << "\n";
  }
#endif  //TEST_HTTP_CLIENT

#ifdef TEST_HTTP_REQ
  HttpReq req(url, HttpClient::GET);
  std::cout << "waiting";
  while (!req.IsFinished())
  {
    std::cout << ".";
    sleep(1);
  } 
  std::cout << "FINISHED";
  std::cout << "\n";
  std::cout << req.GetResult().GetString().c_str() << "\n";
#endif

#ifdef TEST_ONLINE_REQ_MAN
  OnlineReqManager rm;
  SharedPtr<TestReq> pReq = new TestReq(url, HttpClient::GET, "test-request");
  rm.AddReq(pReq.GetPtr());
  while (true)
  {
    rm.Update();
    std::cout << ".";
    std::cout.flush();
#ifdef MACOSX
    sleep(3);
#else
    Sleep(3000);
#endif
  
#ifdef KEEP_POLLING_FOREVER
    // Keep creating more requests 
    SharedPtr<TestReq> pReq = new TestReq(url, HttpClient::GET, "test-request");
    rm.AddReq(pReq.GetPtr());
#endif
  }
#endif

  return 0;
}


/*
Amju Games source code (c) Copyright Jason Colman 2000-2006
$Log: OnlineReqTestable.h,v $
Revision 1.1.2.1  2006/08/07 20:42:50  jay
Online Pool - not working yet

*/

#ifndef ONLINE_REQ_TESTABLE_H_INCLUDED
#define ONLINE_REQ_TESTABLE_H_INCLUDED

#include "OnlineReqManager.h"

namespace Amju
{
// Http request which can be tested.
// Instead of really requesting data from a server, we can return
// data from a file instead.
class OnlineReqTestable : public OnlineReq
{
public:
  OnlineReqTestable(
    const std::string& name,
    const std::string& testData = "");

  void SetTestData(const std::string& rawHtml);
  void SetTestDataFromFile(const std::string& filename);

protected:
  virtual void CreateWorker();
};
}

#endif


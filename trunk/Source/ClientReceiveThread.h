/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: ClientReceiveThread.h,v $
Revision 1.1  2004/09/08 15:42:46  jay
Added to repository
 
*/

#if !defined(SCHMICKEN_CLIENT_RECEIVE_THREAD_H_INCLUDED)
#define SCHMICKEN_CLIENT_RECEIVE_THREAD_H_INCLUDED

#include "ReceiveThread.h"

namespace Amju
{
class ClientReceiveThread : public ReceiveThread 
{
protected:
  virtual void ProcessMessage(PMessage);

};
}

#endif


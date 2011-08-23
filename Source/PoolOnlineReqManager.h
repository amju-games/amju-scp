/*
Amju Games source code (c) Copyright Jason Colman 2000-2007
*/

#ifndef POOL_ONLINE_REQ_MANAGER_H_INCLUDED
#define POOL_ONLINE_REQ_MANAGER_H_INCLUDED

#include "OnlineReqManager.h"
#include "Singleton.h"

namespace Amju
{
// HTTP Request manager: currently used for online score table,
//  but can be used for online game later too.
class PoolOnlineReqManager : public OnlineReqManager
{
};

typedef Singleton<PoolOnlineReqManager> ThePlayerProfileManager;
}

#endif


/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: LevelWorkerFactory.h,v $
Revision 1.1  2004/09/08 15:42:39  jay
Added to repository
  
*/

#if !defined(LEVEL_WORKER_FACTORY_H_INCLUDED)
#define LEVEL_WORKER_FACTORY_H_INCLUDED

#include "Factory.h"
#include "Singleton.h"
#include "LevelWorker.h"

namespace Amju
{
class SingleLevelWorkerFactory : public Factory<LevelWorker>
{
public:
  SingleLevelWorkerFactory();
};

typedef Singleton<SingleLevelWorkerFactory> LevelWorkerFactory;
}

#endif

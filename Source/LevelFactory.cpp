/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: LevelFactory.cpp,v $
Revision 1.1.10.1  2005/09/17 22:39:19  Administrator
Fixed Object Delete in Scene Editor

Revision 1.1  2004/09/08 15:42:38  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "LevelFactory.h"

namespace Amju
{
void LevelFactory::SetWorkerForObject(RCPtr<LevelWorker> p, int id)
{
  m_idToWorker[id] = p;
}

void LevelFactory::ClearWorkerForObject(int id)
{
  m_idToWorker.erase(id);
}
}


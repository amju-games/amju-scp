/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: UniqueId.cpp,v $
Revision 1.1  2004/09/08 15:42:59  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "UniqueId.h"
#include "Engine.h"

namespace Amju
{
int UniqueId::s_highest = 1000;

bool UniqueId::IsUnique(int id)
{
  if (id < 1)
  {
    // This ID is reserved.
    return false;
  }

  const Amju::GameObjectMap& gos = Amju::Engine::Instance()->GetGameObjects();
  Amju::GameObjectMap::const_iterator it = gos.find(id);
  if (it != gos.end())
  {
    return false; 
  }

  return true;
}

int UniqueId::GetNewUniqueId(int lowerBound, int upperBound)
{
  // As users may swap level files etc we can't rely on unique IDs.
  // We can only make sure IDs are unique within a level.

  int uid = lowerBound;
  while (uid <= upperBound)
  {
    if (IsUnique(uid))
    {
      return uid;
    }
    int r = 1; //rand() % 100;
    uid += r;
  }
  return 0; // => no unique ID in range
}

int UniqueId::GetNewUniqueId()
{
  ++s_highest;
  return s_highest;
}

void UniqueId::SetUsedId(int id)
{
  if (id > s_highest)
  {
    s_highest = id;
  }
}

}


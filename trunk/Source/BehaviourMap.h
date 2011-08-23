/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: BehaviourMap.h,v $
Revision 1.1  2004/09/08 15:42:45  jay
Added to repository
  
*/

#if !defined(BEHAVIOUR_MAP_H_INCLUDED)
#define BEHAVIOUR_MAP_H_INCLUDED

#include <map>

namespace Amju
{
class Behaviour;
// Maps NPC state IDs to Behaviours.
typedef std::map<int, Behaviour*> BehaviourMap;
}

#endif

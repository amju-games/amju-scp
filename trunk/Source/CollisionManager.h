/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: CollisionManager.h,v $
Revision 1.1  2004/09/08 15:42:46  jay
Added to repository
  
*/

#if !defined(COLLISION_MANAGER_H_INCLUDED)
#define COLLISION_MANAGER_H_INCLUDED

namespace Amju
{
class SingleCollisionManager
{
public:
  // Perform collision testing between every pair of game objects in the
  // given level/room. Sadly this is O(n^2/2).
  void CollisionTest(int levelId, int roomId);
};
}

#endif


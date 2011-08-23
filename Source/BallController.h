/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: BallController.h,v $
Revision 1.1.10.2  2005/09/08 20:00:11  jay
Try to limmit POOL hacks to BallController

Revision 1.1.10.1  2005/04/11 21:15:53  jay
Improvements to pool physics.

Revision 1.1  2004/09/08 15:42:44  jay
Added to repository
  
*/

#if !defined(BALL_CONTROLLER_H_INCLUDED)
#define BALL_CONTROLLER_H_INCLUDED

#include "Controller.h"

namespace Amju
{
// Control a ball, or other object which travels in straight lines,
// bounces off things, etc.
class BallController : public Controller
{
public:
  virtual void UpdateObject(FreeMovingGameObject* pCharacter); 

protected:
  virtual void OnHitWall(  
    FreeMovingGameObject* pCharacter,
    WallPoly* pWp, 
    float oldx, float oldz, 
    float newx, float newz, 
    float* resultx, float* resultz);

  virtual void OnHitWalls(  
    FreeMovingGameObject* pCharacter,
    const WallVector& walls,
    float oldx, float oldz, 
    float newx, float newz, 
    float* resultx, float* resultz);

  virtual void RollDownSlope(FreeMovingGameObject*);

  virtual void UpdateCharacterYPos(FreeMovingGameObject* pCharacter, float stepHeight); 

};
}

#endif


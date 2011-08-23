/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: Controller.h,v $
Revision 1.1.10.3  2005/07/30 12:13:09  jay
Made useful function GetReflectDegs static+public

Revision 1.1.10.2  2005/04/12 20:05:12  jay
Attempt to fix bad reflection angle when ball hits multiple walls.

Revision 1.1.10.1  2005/04/11 21:15:54  jay
Improvements to pool physics.

Revision 1.1  2004/09/08 15:42:47  jay
Added to repository
  
*/

#if !defined(CONTROLLER_H_INCLUDED)
#define CONTROLLER_H_INCLUDED

#include <vector>
#include "SharedPtr.h"

namespace Amju
{
class FreeMovingGameObject;
class WallPoly;
class HeightServer;
class BoundingSphere;

// Controls Free Moving objects.
// Subclasses can control objects in different ways.
class Controller : public Shareable
{
public:
  virtual ~Controller() {}

  static void Init();

  // Used to get bounce reflection angle
  static float GetReflectionDegs(float incidenceDegs, const WallPoly* pWp);

  virtual void UpdateObject(FreeMovingGameObject* ) = 0; 

  bool IsIntersectingCeiling(
    FreeMovingGameObject* pCharacter, 
    const BoundingSphere& bsBefore,
    const BoundingSphere& bsAfter);

  // Get an object out of a wall if it gets stuck there. :-(
  void ExtractCharacterFromWall(
    FreeMovingGameObject* pCharacter);

protected:
  typedef std::vector<WallPoly> WallVector;

  virtual void UpdateCharacterYPos(FreeMovingGameObject* pCharacter, float stepHeight);

  virtual void HandleWallCollision(FreeMovingGameObject* pCharacter, float stepHeight);

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

  const WallPoly* GetWallCollision(
    FreeMovingGameObject* pCharacter, 
    const HeightServer* pHs);

  const WallPoly* GetWallCollision(
    const HeightServer* pHs, 
    const BoundingSphere& bsBefore,
    const BoundingSphere& bsAfter);

  // Bounce off a wall.
  void BounceOff(
    FreeMovingGameObject* pCharacter,
    WallPoly* pWp, 
    float oldx, float oldz, 
    float newx, float newz, 
    float* resultx, float* resultz);

  // Slide along a wall.  
  void Slide(
    FreeMovingGameObject* pCharacter,
    WallPoly* pWp, 
    float oldx, float oldz, 
    float newx, float newz, 
    float* resultx, float* resultz);

  void EnforceBoundary(
    FreeMovingGameObject* pCharacter);

protected:
  static float s_smallestReverseDistance;
  static float s_smallestFallingReverseDistance;
  static float s_smallestDotProduct;
  static float s_nearlyFallingTimeLimit;
  // 'step height' - max height diff which is treated as a step
  static float s_stepHeight;

};
}
#endif


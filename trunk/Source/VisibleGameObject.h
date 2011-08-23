/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: VisibleGameObject.h,v $
Revision 1.1  2004/09/08 15:43:00  jay
Added to repository
  
*/

#if !defined(VISIBLE_GAME_OBJECT_H_INCLUDED)
#define VISIBLE_GAME_OBJECT_H_INCLUDED

#include "GameObject.h"
#include "Shadow.h"
#include "HeightServer.h"

namespace Amju
{
class Frustum;
class Platform;

class VisibleGameObject : public GameObject
{
public:
  VisibleGameObject();

  virtual bool IsVisible() const { return true; }
  virtual void DrawShadow() const;
  float GetShadowSize() const { return m_shadowSize; }
  void SetShadowSize(float f) { m_shadowSize = f; }

  virtual void Update();

  // Draw the object, but cull it if it falls outside the frustum.
  // Override for effects etc.
  virtual void CullDraw(Frustum*);

  // Based on the current level, room and position, fill this GameObject's
  // HeightServer by pooling all the HeightServers whose Bounding Sphere
  // we intersect.
  virtual void RefreshHeightServer();
  // This is virtual in case we want to override for Characters: we want
  // to incorporate Platforms without recalculating every frame.
  virtual const HeightServer* GetHeightServer() const { return &m_heightServer; }

  // Add the polys in hs to the HeightServer belonging to this Game Object.
  // Override to do nothing if collision data is never used by the object.
  virtual bool AddToHeightServer(const HeightServer& hs);

  // To prevent Characters occupying the smae space, each one has a "Collision
  // Volume" which other characters are not allowed to enter.
  virtual const HeightServer* GetCollisionVol() const;

  // Force shadow to recalc itself.
  void RefreshShadow();

  // Default implementation makes a basic shadow. Override for special shadows.
  virtual void CreateShadow();

  // Platforms
  // ---------
  // I.e. Moving objects which affect the velocity of this one.

  // Set pointer to the Platform the character is on. If the character is not
  // on a platform, set to 0.
  void SetActivePlatform(Platform* p);
  
  // Get platform Character is on, or 0.
  Platform* GetActivePlatform() { return m_pActivePlatform; }

  // Redo the collision volume height server when the object moves.
  // NB Public so we can call this from EngineState when we need to reset
  // Collision Volumes.
  virtual void RecalcCollisionVol() {}

protected:
   // Specify which HeightServer gets the info. Used by virtual version above.
  void RefreshHeightServer(HeightServer* pHs);

  // Used by RefreshHeightServer, we include heights which intersect 
  // this sphere. By default this is the same as the sphere returned from
  // GetBoundingSphere(), but you can increase the size to get height info from
  // further away, or decrease it to improve performance if height info is
  // not important.
  virtual const BoundingSphere* GetHeightServerSphere();


protected:
  // Visible game objects cast a shadow on the ground. 
  PShadow m_pShadow;

  // HeightServer covering the parts of the scene this GameObject intersects.
  mutable HeightServer m_heightServer;
  // Cache old coords; we refresh the HeightServer only when they change.
  mutable float m_oldx, m_oldy, m_oldz;
  float m_shadowSize;

  // Define how other objects collide with this one:
  // This height server specifies the volume which no other object may occupy.
  HeightServer m_collisionVolume;

  // Platforms
  // ---------
  // Point to any platform this object is currently on.
  Platform* m_pActivePlatform; 


};
}

#endif



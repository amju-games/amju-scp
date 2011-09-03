/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: Shadow.h,v $
Revision 1.1.10.1  2007/12/12 10:27:41  jay
Add per-shadow height offset

Revision 1.1  2004/09/08 15:42:56  jay
Added to repository
  
*/

#if !defined(SHADOW_H_INCLUDED)
#define SHADOW_H_INCLUDED

#include <string>
#include <utility>
#include <vector>
#include "HeightServer.h"
#include "Polygon.h"
#include "SharedPtr.h"

namespace Amju
{
class PoolTexture;
class VisibleGameObject;

// Draws a shadow on the ground. This is complicated because the ground under 
// the shadow may cover more than one polygon, which may lie on different 
// planes.
// The texture is the same for all shadows.
// Use the same Shadow object for a particular solid. This allows the Shadow
// to cache stuff.
class Shadow : public Shareable
{
public:
  static bool Init(); 

  Shadow();
  virtual ~Shadow();

  virtual void SetCaster(VisibleGameObject* pVgo);

  // Draw shadow on the ground plane below point (x, y, z).
  // The size corresponds to the radius of the shadow.
  // The size is changed depending on the height y above the ground plane.
  // The HeightServer is used to get the ground polys.
  // NB This is virtual so you can add extra stuff before and after the call
  // to this base implementation.
  virtual void Draw(
    float x, float y, float z,
    float size,
    const HeightServer& hs);

  // Reset old coords, so Poly list will be refreshed.
  void Reset();

  // Set the y-offset - this is to avoid z-fighting.
  // Ideally we would do this automatically depending on distance to camera
  void SetYOffset(float yOffset); 

// These functions should be protected, but then we can't access them
//  for a container of Shadows :-(
//protected:
  virtual void BindTexture() = 0;

  virtual void DrawList() = 0;

  virtual void RecalculateList(
    float x, 
    float y, 
    float z,
    float size,
    const HeightServer& hs) = 0;

protected:
  // Cached position/size and polys, so we only recalculate when necessary.
  float m_oldx, m_oldy, m_oldz, m_oldsize;

  // Max height above ground an object can be and still cast a shadow. 
  static float s_maxHeight;


  // Default height above ground at which shadows are drawn
  static const float OFFSET;

protected:
  // Object-specific height offset
  float m_yOffset;
};

typedef SharedPtr<Shadow> PShadow;
}

#endif
